/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            ntoskrnl/ps/kill.c
 * PURPOSE:         Process Manager: Process and Thread Termination
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 *                  Filip Navara (xnavara@reactos.org)
 *                  Thomas Weidenmueller (w3seek@reactos.org
 */

/* INCLUDES *****************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <internal/debug.h>

/* GLOBALS *******************************************************************/

LIST_ENTRY PspReaperListHead = {0};
WORK_QUEUE_ITEM PspReaperWorkItem;
extern LIST_ENTRY PsActiveProcessHead;
extern FAST_MUTEX PspActiveProcessMutex;
extern PHANDLE_TABLE PspCidTable;
extern PKWIN32_PROCESS_CALLOUT PspW32ProcessCallout;
extern PKWIN32_THREAD_CALLOUT PspW32ThreadCallout;
extern PEPROCESS PsInitialSystemProcess;
extern PEPROCESS PsIdleProcess;

/* PRIVATE FUNCTIONS *********************************************************/

NTSTATUS
NTAPI
PspTerminateProcess(IN PEPROCESS Process,
                    IN NTSTATUS ExitStatus)
{
    PAGED_CODE();
    PETHREAD Thread = NULL;

    /* Check if this is a Critical Process, and Bugcheck */
    if (Process->BreakOnTermination)
    {
        /* FIXME: Add critical Process support */
        DPRINT1("A critical Process is being terminated\n");
        KEBUGCHECK(0);
    }

    /* Set the delete flag */
    InterlockedOr((PLONG)&Process->Flags, 8);

    /* Get the first thread */
    Thread = PsGetNextProcessThread(Process, Thread);
    while (Thread)
    {
        /* Kill it */
        PspTerminateThreadByPointer(Thread, ExitStatus, FALSE);
        Thread = PsGetNextProcessThread(Process, Thread);
    }

    /* Clear the handle table */
    if (Process->ObjectTable) ObClearProcessHandleTable(Process);

    /* Return success*/
    return STATUS_SUCCESS;
}

VOID
NTAPI
PspShutdownProcessManager(VOID)
{
    PEPROCESS Process = NULL;

    /* Loop every process */
    Process == PsGetNextProcess(Process);
    while (Process)
    {
        /* Make sure this isn't the idle or initial process */
        if ((Process != PsInitialSystemProcess) && (Process != PsIdleProcess))
        {
            /* Kill it */
            PspTerminateProcess(Process, STATUS_SYSTEM_SHUTDOWN);
        }

        /* Get the next process */
        Process == PsGetNextProcess(Process);
    }
}

VOID
NTAPI
PspExitApcRundown(IN PKAPC Apc)
{
    PAGED_CODE();

    /* Free the APC */
    ExFreePool(Apc);
}

VOID
NTAPI
PspReapRoutine(PVOID Context)
{
    PLIST_ENTRY *ListAddr;
    PLIST_ENTRY NextEntry;
    PETHREAD Thread;

    /* Get the Reaper Address Pointer */
    ListAddr = &PspReaperListHead.Flink;

    /* Start main loop */
    do
    {
        /* Write magic value and return the next entry to process */
        NextEntry = InterlockedExchangePointer(ListAddr, (PVOID)1);
        ASSERT((NextEntry != NULL) && (NextEntry != (PVOID)1));

        /* Start inner loop */
        do
        {
            /* Get the first Thread Entry */
            Thread = CONTAINING_RECORD(NextEntry, ETHREAD, ReaperLink);

            /* Delete this entry's kernel stack */
            MmDeleteKernelStack((PVOID)Thread->Tcb.StackLimit,
                                Thread->Tcb.LargeStack);
            Thread->Tcb.InitialStack = NULL;

            /* Move to the next entry */
            NextEntry = NextEntry->Flink;

            /* Dereference this thread */
            ObDereferenceObject(Thread);
        } while ((NextEntry != NULL) && (NextEntry != (PVOID)1));

        /* Remove magic value, keep looping if it got changed */
    } while (InterlockedCompareExchangePointer(ListAddr, 0, 1) != (PVOID)1);
}

VOID
NTAPI
PspDeleteProcess(PVOID ObjectBody)
{
    PEPROCESS Process = (PEPROCESS)ObjectBody;
    KAPC_STATE ApcState;
    PAGED_CODE();

    /* Check if it has an Active Process Link */
    if (Process->ActiveProcessLinks.Flink)
    {
        /* Remove it from the Active List */
        ExAcquireFastMutex(&PspActiveProcessMutex);
        RemoveEntryList(&Process->ActiveProcessLinks);
        ExReleaseFastMutex(&PspActiveProcessMutex);
    }

    /* Check for Auditing information */
    if (Process->SeAuditProcessCreationInfo.ImageFileName)
    {
        /* Free it */
        ExFreePool(Process->SeAuditProcessCreationInfo.ImageFileName);
        Process->SeAuditProcessCreationInfo.ImageFileName = NULL;
    }

    /* Check if we have a job */
    if (Process->Job)
    {
        /* Remove the process from the job */
        PspRemoveProcessFromJob(Process, Process->Job);

        /* Dereference it */
        ObDereferenceObject(Process->Job);
        Process->Job = NULL;
    }

    /* Increase the stack count */
    Process->Pcb.StackCount++;

    /* Check if we have a debug port */
    if (Process->DebugPort)
    {
        /* Dererence the Debug Port */
        ObDereferenceObject(Process->DebugPort);
        Process->DebugPort = NULL;
    }

    /* Check if we have an exception port */
    if (Process->ExceptionPort)
    {
        /* Dererence the Exception Port */
        ObDereferenceObject(Process->ExceptionPort);
        Process->ExceptionPort = NULL;
    }

    /* Check if we have a section object */
    if (Process->SectionObject)
    {
        /* Dererence the Section Object */
        ObDereferenceObject(Process->SectionObject);
        Process->SectionObject = NULL;
    }

    /* Clean LDT and VDM_OBJECTS */
    PspDeleteLdt(Process);
    PspDeleteVdmObjects(Process);

    /* Delete the Object Table */
    if (Process->ObjectTable)
    {
        /* Attach to the process */
        KeStackAttachProcess(&Process->Pcb, &ApcState);

        /* Kill the Object Info */
        ObKillProcess(Process);

        /* Dettach */
        KeUnstackDetachProcess(&ApcState);
    }

    /* KDB hook */
    KDB_DELETEPROCESS_HOOK(Process);

    /* Check if we have an address space, and clean it */
    if (Process->HasAddressSpace)
    {
        /* Attach to the process */
        KeStackAttachProcess(&Process->Pcb, &ApcState);

        /* Clean the Address Space */
        PspExitProcess(FALSE, Process);

        /* Dettach */
        KeUnstackDetachProcess(&ApcState);

        /* Completely delete the Address Space */
        MmDeleteProcessAddressSpace(Process);
    }

    /* See if we have a PID */
    if(Process->UniqueProcessId)
    {
        /* Delete the PID */
        if (!(ExDestroyHandle(PspCidTable, Process->UniqueProcessId)))
        {
            /* Something wrong happened, bugcheck */
            KEBUGCHECK(CID_HANDLE_DELETION);
        }
    }

    /* Cleanup security information */
    PspDeleteProcessSecurity(Process);

    /* Check if we have kept information on the Working Set */
    if (Process->WorkingSetWatch)
    {
        /* Free it */
        ExFreePool(Process->WorkingSetWatch);

        /* And return the quota it was taking up */
        PsReturnProcessNonPagedPoolQuota(Process, 0x2000);
    }

    /* Dereference the Device Map */
    ObDereferenceDeviceMap(Process);

    /* Destroy the Quota Block */
    PspDestroyQuotaBlock(Process);
}

VOID
NTAPI
PspDeleteThread(PVOID ObjectBody)
{
    PETHREAD Thread = (PETHREAD)ObjectBody;
    PEPROCESS Process = Thread->ThreadsProcess;
    PAGED_CODE();
    ASSERT(Thread->Tcb.Win32Thread == NULL);

    /* Check if we have a stack */
    if (Thread->Tcb.InitialStack)
    {
        /* Release it */
        MmDeleteKernelStack((PVOID)Thread->Tcb.StackLimit,
                            Thread->Tcb.LargeStack);
    }

    /* Check if we have a CID Handle */
    if (Thread->Cid.UniqueThread)
    {
        /* Delete the CID Handle */
        if (!(ExDestroyHandle(PspCidTable, Thread->Cid.UniqueThread)))
        {
            /* Something wrong happened, bugcheck */
            KEBUGCHECK(CID_HANDLE_DELETION);
        }
    }

    /* Cleanup impersionation information */
    PspDeleteThreadSecurity(Thread);

    /* Make sure the thread was inserted, before continuing */
    if (!Process) return;

    /* Check if the thread list is valid */
    if (Thread->ThreadListEntry.Flink)
    {
        /* Lock the thread's process */
        PsLockProcess(Process, FALSE);

        /* Remove us from the list */
        RemoveEntryList(&Thread->ThreadListEntry);

        /* Release the lock */
        PsUnlockProcess(Process);
    }

    /* Dereference the Process */
    ObDereferenceObject(Process);
}

/*
 * FUNCTION: Terminates the current thread
 * See "Windows Internals" - Chapter 13, Page 50-53
 */
VOID
NTAPI
PspExitThread(NTSTATUS ExitStatus)
{
    CLIENT_DIED_MSG TerminationMsg;
    NTSTATUS Status;
    PTEB Teb;
    PEPROCESS CurrentProcess;
    PETHREAD Thread;
    PVOID DeallocationStack;
    ULONG Dummy;
    BOOLEAN Last = FALSE;
    PTERMINATION_PORT TerminationPort, NextPort;
    PLIST_ENTRY FirstEntry, CurrentEntry;
    PKAPC Apc;
    PTOKEN PrimaryToken;
    PAGED_CODE();

    /* Get the Current Thread and Process */
    Thread = PsGetCurrentThread();
    CurrentProcess = Thread->ThreadsProcess;
    ASSERT((Thread) == PsGetCurrentThread());

    /* Can't terminate a thread if it attached another process */
    if (KeIsAttachedProcess())
    {
        /* Bugcheck */
        KEBUGCHECKEX(INVALID_PROCESS_ATTACH_ATTEMPT,
                     (ULONG_PTR)CurrentProcess,
                     (ULONG_PTR)Thread->Tcb.ApcState.Process,
                     (ULONG_PTR)Thread->Tcb.ApcStateIndex,
                     (ULONG_PTR)Thread);
    }

    /* Lower to Passive Level */
    KfLowerIrql(PASSIVE_LEVEL);

    /* Can't be a worker thread */
    if (Thread->ActiveExWorker)
    {
        /* Bugcheck */
        KEBUGCHECKEX(ACTIVE_EX_WORKER_THREAD_TERMINATION,
                     (ULONG_PTR)Thread,
                     0,
                     0,
                     0);
    }

    /* Can't have pending APCs */
    if (Thread->Tcb.CombinedApcDisable != 0)
    {
        /* Bugcheck */
        KEBUGCHECKEX(KERNEL_APC_PENDING_DURING_EXIT,
                     0,
                     Thread->Tcb.KernelApcDisable,
                     APC_LEVEL,
                     0);
    }

    /* Lock the thread */
    ExWaitForRundownProtectionRelease(&Thread->RundownProtect);

    /* Cleanup the power state */
    PopCleanupPowerState((PPOWER_STATE)&Thread->Tcb.PowerState);

    /* Call the WMI Callback for Threads */
    //WmiTraceThread(Thread, NULL, FALSE);

    /* Run Thread Notify Routines before we desintegrate the thread */
    PspRunCreateThreadNotifyRoutines(Thread, FALSE);

    /* Lock the Process before we modify its thread entries */
    PsLockProcess(CurrentProcess, FALSE);

    /* Wake up the thread so we don't deadlock on PsLockProcess */
    KeForceResumeThread(&Thread->Tcb);

    /* Decrease the active thread count, and check if it's 0 */
    if (!(--CurrentProcess->ActiveThreads))
    {
        /* Set the delete flag */
        InterlockedOr((PLONG)&CurrentProcess->Flags, 8);

        /* Remember we are last */
        Last = TRUE;

        /* Check if this termination is due to the thread dying */
        if (ExitStatus == STATUS_THREAD_IS_TERMINATING)
        {
            /* Check if the last thread was pending */
            if (CurrentProcess->ExitStatus == STATUS_PENDING)
            {
                /* Use the last exit status */
                CurrentProcess->ExitStatus = CurrentProcess->
                                             LastThreadExitStatus;
            }
        }
        else
        {
            /* Just a normal exit, write the code */
            CurrentProcess->ExitStatus = ExitStatus;
        }

        /* FIXME: Wait on the other threads to finish */
    }
    else if (ExitStatus != STATUS_THREAD_IS_TERMINATING)
    {
        /* Write down the exit status of the last thread to get killed */
        CurrentProcess->LastThreadExitStatus = ExitStatus;
    }

    /* Unlock the Process */
    PsUnlockProcess(CurrentProcess);

    /* Check if the process has a debug port and if this is a user thread */
    if ((CurrentProcess->DebugPort) && !(Thread->SystemThread))
    {
        /* Notify the Debug API. */
        Last ? DbgkExitProcess(CurrentProcess->ExitStatus) :
               DbgkExitThread(ExitStatus);
    }

    /* Check if this is a Critical Thread */
    if ((KdDebuggerEnabled) && (Thread->BreakOnTermination))
    {
        /* FIXME: Add critical thread support */
        KEBUGCHECK(0);
    }

    /* Check if it's the last thread and this is a Critical Process */
    if ((Last) && (CurrentProcess->BreakOnTermination))
    {
        /* Check if a debugger is here to handle this */
        if (KdDebuggerEnabled)
        {
            /* FIXME: Add critical process support */
            DbgBreakPoint();
        }
        else
        {
            /* Bugcheck, we can't allow this */
            KEBUGCHECKEX(CRITICAL_PROCESS_DIED,
                         (ULONG_PTR)CurrentProcess,
                         0,
                         0,
                         0);
        }
    }

    /* Sanity check */
    ASSERT(Thread->Tcb.CombinedApcDisable == 0);

    /* Process the Termination Ports */
    TerminationPort = Thread->TerminationPort;
    if (TerminationPort)
    {
        /* Setup the message header */
        TerminationMsg.h.u2.s2.Type = LPC_CLIENT_DIED;
        TerminationMsg.h.u1.s1.TotalLength = sizeof(TerminationMsg);
        TerminationMsg.h.u1.s1.DataLength = sizeof(TerminationMsg) -
                                            sizeof(PORT_MESSAGE);

        /* Loop each port */
        do
        {
            /* Save the Create Time */
            TerminationMsg.CreateTime = Thread->CreateTime;

TryAgain:
            /* Send the LPC Message */
            Status = LpcRequestPort(TerminationPort->Port, &TerminationMsg.h);
            if ((Status == STATUS_NO_MEMORY) ||
                (Status == STATUS_INSUFFICIENT_RESOURCES))
            {
                /* Wait a bit and try again */
                KeDelayExecutionThread(KernelMode, FALSE, &ShortPsLockDelay);
                goto TryAgain;
            }

            /* Dereference this LPC Port */
            ObDereferenceObject(TerminationPort->Port);

            /* Move to the next one */
            NextPort = TerminationPort->Next;

            /* Free the Termination Port Object */
            ExFreePool(TerminationPort);

            /* Keep looping as long as there is a port */
        } while ((TerminationPort = NextPort));
    }
    else if (((ExitStatus == STATUS_THREAD_IS_TERMINATING) &&
              (Thread->DeadThread)) ||
             !(Thread->DeadThread))
    {
        /*
         * This case is special and deserves some extra comments. What
         * basically happens here is that this thread doesn't have a termination
         * port, which means that it died before being fully created. Since we
         * still have to notify an LPC Server, we'll use the exception port,
         * which we know exists. However, we need to know how far the thread
         * actually got created. We have three possibilites:
         *
         *  - NtCreateThread returned an error really early: DeadThread is set.
         *  - NtCreateThread managed to create the thread: DeadThread is off.
         *  - NtCreateThread was creating the thread (with Deadthread set,
         *    but the thread got killed prematurely: STATUS_THREAD_IS_TERMINATING
         *    is our exit code.)
         *
         * For the 2 & 3rd scenarios, the thread has been created far enough to
         * warrant notification to the LPC Server.
         */

        /* Setup the message header */
        TerminationMsg.h.u2.s2.Type = LPC_CLIENT_DIED;
        TerminationMsg.h.u1.s1.TotalLength = sizeof(TerminationMsg);
        TerminationMsg.h.u1.s1.DataLength = sizeof(TerminationMsg) -
                                            sizeof(PORT_MESSAGE);

        /* Make sure the process has an exception port */
        if (CurrentProcess->ExceptionPort)
        {
            /* Save the Create Time */
            TerminationMsg.CreateTime = Thread->CreateTime;

TryAgain2:
            /* Send the LPC Message */
            Status = LpcRequestPort(CurrentProcess->ExceptionPort,
                                    &TerminationMsg.h);
            if ((Status == STATUS_NO_MEMORY) ||
                (Status == STATUS_INSUFFICIENT_RESOURCES))
            {
                /* Wait a bit and try again */
                KeDelayExecutionThread(KernelMode, FALSE, &ShortPsLockDelay);
                goto TryAgain2;
            }
        }
    }

    /* Rundown Win32 Thread if there is one */
    if (Thread->Tcb.Win32Thread) PspW32ThreadCallout(Thread,
                                                     PsW32ThreadCalloutExit);

    /* If we are the last thread and have a W32 Process */
    if ((Last) && (CurrentProcess->Win32Process))
    {
        /* Run it down too */
        PspW32ProcessCallout(CurrentProcess, FALSE);
    }

    /* Make sure Stack Swap isn't enabled */
    if (Thread->Tcb.EnableStackSwap)
    {
        /* Stack swap really shouldn't be on during exit !*/
        KEBUGCHECKEX(KERNEL_STACK_LOCKED_AT_EXIT, 0, 0, 0, 0);
    }

    /* Cancel I/O for the thread. */
    IoCancelThreadIo(Thread);

    /* Rundown Timers */
    ExTimerRundown();

    /* FIXME: Rundown Registry Notifications (NtChangeNotify)
    CmNotifyRunDown(Thread); */

    /* Rundown Mutexes */
    KeRundownThread();

    /* Check if we have a TEB */
    Teb = Thread->Tcb.Teb;
    if(Teb)
    {
        /* Check if the thread isn't terminated and if we should free stack */
        if (!(Thread->Terminated) && (Teb->FreeStackOnTermination))
        {
            /* Set the TEB's Deallocation Stack as the Base Address */
            Dummy = 0;
            DeallocationStack = Teb->DeallocationStack;

            /* Free the Thread's Stack */
            ZwFreeVirtualMemory(NtCurrentProcess(),
                                &DeallocationStack,
                                &Dummy,
                                MEM_RELEASE);
        }

        /* Free the debug handle */
        if (Teb->DbgSsReserved[1]) ObCloseHandle(Teb->DbgSsReserved[1],
                                                 UserMode);

        /* Decommit the TEB */
        MmDeleteTeb(CurrentProcess, Teb);
        Thread->Tcb.Teb = NULL;
    }

    /* Free LPC Data */
    LpcExitThread(Thread);

    /* Save the exit status and exit time */
    Thread->ExitStatus = ExitStatus;
    KeQuerySystemTime(&Thread->ExitTime);

    /* Sanity check */
    ASSERT(Thread->Tcb.CombinedApcDisable == 0);

    /* Check if this is the final thread or not */
    if (Last)
    {
        /* Set the process exit time */
        CurrentProcess->ExitTime = Thread->ExitTime;

        /* Exit the process */
        PspExitProcess(TRUE, CurrentProcess);

        /* Get the process token and check if we need to audit */
        PrimaryToken = PsReferencePrimaryToken(CurrentProcess);
        if (SeDetailedAuditingWithToken(PrimaryToken))
        {
            /* Audit the exit */
            SeAuditProcessExit(CurrentProcess);
        }

        /* Dereference the process token */
        ObFastDereferenceObject(&CurrentProcess->Token, PrimaryToken);

        /* Check if this is a VDM Process and rundown the VDM DPCs if so */
        if (CurrentProcess->VdmObjects);// VdmRundownDpcs(CurrentProcess);

        /* Kill the process in the Object Manager */
        ObKillProcess(CurrentProcess);

        /* Check if we have a section object */
        if (CurrentProcess->SectionObject)
        {
            /* Dereference and clear the Section Object */
            ObDereferenceObject(CurrentProcess->SectionObject);
            CurrentProcess->SectionObject = NULL;
        }

        /* Check if the process is part of a job */
        if (CurrentProcess->Job)
        {
            /* Remove the process from the job */
            PspExitProcessFromJob(CurrentProcess->Job, CurrentProcess);
        }
    }

    /* Disable APCs */
    KeEnterCriticalRegion();

    /* Disable APC queueing, force a resumption */
    Thread->Tcb.ApcQueueable = FALSE;
    KeForceResumeThread(&Thread->Tcb);

    /* Re-enable APCs */
    KeLeaveCriticalRegion();

    /* Flush the User APCs */
    FirstEntry = KeFlushQueueApc(&Thread->Tcb, UserMode);
    if (FirstEntry)
    {
        CurrentEntry = FirstEntry;
        do
        {
           /* Get the APC */
           Apc = CONTAINING_RECORD(CurrentEntry, KAPC, ApcListEntry);

           /* Move to the next one */
           CurrentEntry = CurrentEntry->Flink;

           /* Rundown the APC or de-allocate it */
           if (Apc->RundownRoutine)
           {
              /* Call its own routine */
              (Apc->RundownRoutine)(Apc);
           }
           else
           {
              /* Do it ourselves */
              ExFreePool(Apc);
           }
        }
        while (CurrentEntry != FirstEntry);
    }

    /* Clean address space if this was the last thread */
    if (Last) MmCleanProcessAddressSpace(CurrentProcess);

    /* Call the Lego routine */
    if (Thread->Tcb.LegoData) PspRunLegoRoutine(&Thread->Tcb);

    /* Flush the APC queue, which should be empty */
    FirstEntry = KeFlushQueueApc(&Thread->Tcb, KernelMode);
    if (FirstEntry)
    {
        /* Bugcheck time */
        KEBUGCHECKEX(KERNEL_APC_PENDING_DURING_EXIT,
                     (ULONG_PTR)FirstEntry,
                     Thread->Tcb.KernelApcDisable,
                     KeGetCurrentIrql(),
                     0);
    }

    /* Signal the process if this was the last thread */
    if (Last) KeSetProcess(&CurrentProcess->Pcb, 0, FALSE);

    /* Terminate the Thread from the Scheduler */
    KeTerminateThread(0);
}

VOID
NTAPI
PsExitSpecialApc(PKAPC Apc,
                 PKNORMAL_ROUTINE* NormalRoutine,
                 PVOID* NormalContext,
                 PVOID* SystemArgument1,
                 PVOID* SystemArguemnt2)
{
    NTSTATUS Status;
    PAGED_CODE();

    /* Don't do anything unless we are in User-Mode */
    if (Apc->SystemArgument2)
    {
        /* Free the APC */
        Status = (NTSTATUS)Apc->NormalContext;
        PspExitApcRundown(Apc);

        /* Terminate the Thread */
        PspExitThread(Status);
    }
}

VOID
NTAPI
PspExitNormalApc(PVOID NormalContext,
                 PVOID SystemArgument1,
                 PVOID SystemArgument2)
{
    PKAPC Apc = (PKAPC)SystemArgument1;
    PETHREAD Thread = PsGetCurrentThread();
    PAGED_CODE();

    /* This should never happen */
    ASSERT(!(((ULONG_PTR)SystemArgument2) & 1));

    /* If we're here, this is not a System Thread, so kill it from User-Mode */
    KeInitializeApc(Apc,
                    &Thread->Tcb,
                    OriginalApcEnvironment,
                    PsExitSpecialApc,
                    PspExitApcRundown,
                    PspExitNormalApc,
                    UserMode,
                    NormalContext);

    /* Now insert the APC with the User-Mode Flag */
    if (!(KeInsertQueueApc(Apc,
                           Apc,
                           (PVOID)((ULONG_PTR)SystemArgument2 | 1),
                           2)))
    {
        /* Failed to insert, free the APC */
        PspExitApcRundown(Apc);
    }

    /* Set the APC Pending flag */
    Thread->Tcb.ApcState.UserApcPending = TRUE;
}

/*
 * See "Windows Internals" - Chapter 13, Page 49
 */
NTSTATUS
NTAPI
PspTerminateThreadByPointer(PETHREAD Thread,
                            NTSTATUS ExitStatus,
                            BOOLEAN bSelf)
{
    PKAPC Apc;
    NTSTATUS Status = STATUS_SUCCESS;
    ULONG Flags;
    PAGED_CODE();

    /* Check if this is a Critical Thread, and Bugcheck */
    if (Thread->BreakOnTermination)
    {
        /* FIXME: Add critical thread support */
        DPRINT1("A critical thread is being terminated\n");
        KEBUGCHECK(0);
    }

    /* Check if we are already inside the thread */
    if ((bSelf) || (PsGetCurrentThread() == Thread))
    {
        /* This should only happen at passive */
        ASSERT_IRQL(PASSIVE_LEVEL);

        /* Mark it as terminated */
        InterlockedOr((PLONG)&Thread->CrossThreadFlags, 1);

        /* Directly terminate the thread */
        PspExitThread(ExitStatus);
    }

    /* This shouldn't be a system thread */
    if (Thread->SystemThread) return STATUS_ACCESS_DENIED;

    /* Allocate the APC */
    Apc = ExAllocatePoolWithTag(NonPagedPool, sizeof(KAPC), TAG_TERMINATE_APC);

    /* Set the Terminated Flag */
    Flags = Thread->CrossThreadFlags | 1;

    /* Set it, and check if it was already set while we were running */
    if (!(InterlockedExchange((PLONG)&Thread->CrossThreadFlags, Flags) & 1))
    {
        /* Initialize a Kernel Mode APC to Kill the Thread */
        KeInitializeApc(Apc,
                        &Thread->Tcb,
                        OriginalApcEnvironment,
                        PsExitSpecialApc,
                        PspExitApcRundown,
                        PspExitNormalApc,
                        KernelMode,
                        (PVOID)ExitStatus);

        /* Insert it into the APC Queue */
        if (!KeInsertQueueApc(Apc, Apc, NULL, 2))
        {
            /* The APC was already in the queue, fail */
            ExFreePool(Apc);
            Status = STATUS_UNSUCCESSFUL;
        }
        else
        {
            /* Forcefully resume the thread and return */
            KeForceResumeThread(&Thread->Tcb);
            return Status;
        }
    }

    /* We failed, free the APC */
    ExFreePool(Apc);

    /* Return Status */
    return Status;
}

VOID
NTAPI
PspExitProcess(IN BOOLEAN LastThread,
               IN PEPROCESS Process)
{
    ULONG Actual;
    PAGED_CODE();

    /* Set Process Delete flag */
    InterlockedOr((PLONG)&Process->Flags, 4);

    /* Check if we are the last thread */
    if (LastThread)
    {
        /* Notify the WMI Process Callback */
        //WmiTraceProcess(Process, FALSE);

        /* Run the Notification Routines */
        PspRunCreateProcessNotifyRoutines(Process, FALSE);
    }

    /* Cleanup the power state */
    PopCleanupPowerState((PPOWER_STATE)&Process->Pcb.PowerState);

    /* Clear the security port */
    if (!Process->SecurityPort)
    {
        /* So we don't double-dereference */
        Process->SecurityPort = (PVOID)1;
    }
    else if (Process->SecurityPort != (PVOID)1)
    {
        /* Dereference it */
        ObDereferenceObject(Process->SecurityPort);
        Process->SecurityPort = (PVOID)1;
    }

    /* Check if we are the last thread */
    if (LastThread)
    {
        /* Check if we have to set the Timer Resolution */
        if (Process->SetTimerResolution)
        {
            /* Set it to default */
            ZwSetTimerResolution(KeMaximumIncrement, 0, &Actual);
        }

        /* Check if we are part of a Job that has a completion port */
        if ((Process->Job) && (Process->Job->CompletionPort))
        {
            /* FIXME: Check job status code and do I/O completion if needed */
        }

        /* FIXME: Notify the Prefetcher */
    }
    else
    {
        /* Clear process' address space here */
        MmCleanProcessAddressSpace(Process);
    }
}

/* PUBLIC FUNCTIONS **********************************************************/

/*
 * @implemented
 */
NTSTATUS
NTAPI
PsTerminateSystemThread(NTSTATUS ExitStatus)
{
    PETHREAD Thread = PsGetCurrentThread();

    /* Make sure this is a system thread */
    if (Thread->SystemThread)
    {
        DPRINT1("Trying to Terminate a non-system thread!\n");
        return STATUS_INVALID_PARAMETER;
    }

    /* Terminate it for real */
    return PspTerminateThreadByPointer(Thread, ExitStatus, TRUE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
NtTerminateProcess(IN HANDLE ProcessHandle OPTIONAL,
                   IN NTSTATUS ExitStatus)
{
    NTSTATUS Status;
    PEPROCESS Process, CurrentProcess = PsGetCurrentProcess();
    PETHREAD Thread, CurrentThread = PsGetCurrentThread();
    BOOLEAN KillByHandle;
    PAGED_CODE();

    /* Remember how we will kill it */
    KillByHandle = (ProcessHandle != NULL);

    /* Get the Process Object */
    Status = ObReferenceObjectByHandle((KillByHandle) ?
                                       ProcessHandle : NtCurrentProcess(),
                                       PROCESS_TERMINATE,
                                       PsProcessType,
                                       KeGetPreviousMode(),
                                       (PVOID*)&Process,
                                       NULL);
    if (!NT_SUCCESS(Status)) return(Status);

    /* Check if this is a Critical Process, and Bugcheck */
    if (Process->BreakOnTermination)
    {
        /* FIXME: Add critical Process support */
        DPRINT1("A critical Process is being terminated\n");
        KEBUGCHECK(0);
    }

    /* Lock the Process */
    ExAcquireRundownProtection(&Process->RundownProtect);

    /* Set the exit flag */
    if (!KillByHandle) InterlockedOr((PLONG)&Process->Flags, 8);

    /* Get the first thread */
    Status = STATUS_NOTHING_TO_TERMINATE;
    Thread = PsGetNextProcessThread(Process, NULL);
    if (Thread)
    {
        /* We know we have at least a thread */
        Status = STATUS_SUCCESS;

        /* Loop and kill the others */
        do
        {
            /* Ensure it's not ours*/
            if (Thread != CurrentThread)
            {
                /* Kill it */
                PspTerminateThreadByPointer(Thread, ExitStatus, FALSE);
            }

            /* Move to the next thread */
        } while((Thread = PsGetNextProcessThread(Process, Thread)));
    }

    /* Unlock the process */
    ExReleaseRundownProtection(&Process->RundownProtect);

    /* Check if we are killing ourselves */
    if (Process != CurrentProcess)
    {
        /* Check for the DBG_TERMINATE_PROCESS exit code */
        if (ExitStatus == DBG_TERMINATE_PROCESS)
        {
            /* FIXME: Disable debugging on this process */
        }
    }
    /* Make sure that we got a handle */
    else if (KillByHandle)
    {
        /* Dereference the project */
        ObDereferenceObject(Process);

        /* Terminate ourselves */
        PspTerminateThreadByPointer(CurrentThread, ExitStatus, TRUE);
    }

    /* Check if there was nothing to terminate, or if we have a Debug Port */
    if ((Status == STATUS_NOTHING_TO_TERMINATE) ||
        ((Process->DebugPort) && (KillByHandle)))
    {
        /* Clear the handle table */
        ObClearProcessHandleTable(Process);

        /* Return status now */
        Status = STATUS_SUCCESS;
    }

    /* Decrease the reference count we added */
    ObDereferenceObject(Process);

    /* Return status */
    return Status;
}

NTSTATUS
NTAPI
NtTerminateThread(IN HANDLE ThreadHandle,
                  IN NTSTATUS ExitStatus)
{
    PETHREAD Thread;
    PETHREAD CurrentThread = PsGetCurrentThread();
    NTSTATUS Status;
    PAGED_CODE();

    /* Handle the special NULL case */
    if (!ThreadHandle)
    {
        /* Check if we're the only thread left */
        if (PsGetCurrentProcess()->ActiveThreads == 1)
        {
            /* This is invalid */
            return STATUS_CANT_TERMINATE_SELF;
        }

        /* Terminate us directly */
        goto TerminateSelf;
    }
    else if (ThreadHandle == NtCurrentThread())
    {
TerminateSelf:
        /* Terminate this thread */
        return PspTerminateThreadByPointer(CurrentThread,
                                           ExitStatus,
                                           TRUE);
    }

    /* We are terminating another thread, get the Thread Object */
    Status = ObReferenceObjectByHandle(ThreadHandle,
                                       THREAD_TERMINATE,
                                       PsThreadType,
                                       KeGetPreviousMode(),
                                       (PVOID*)&Thread,
                                       NULL);
    if (!NT_SUCCESS(Status)) return Status;

    /* Check to see if we're running in the same thread */
    if (Thread != CurrentThread)
    {
        /* Terminate it */
        Status = PspTerminateThreadByPointer(Thread, ExitStatus, FALSE);

        /* Dereference the Thread and return */
        ObDereferenceObject(Thread);
    }
    else
    {
        /* Dereference the thread and terminate ourselves */
        ObDereferenceObject(Thread);
        goto TerminateSelf;
    }

    /* Return status */
    return Status;
}

NTSTATUS
NTAPI
NtRegisterThreadTerminatePort(IN HANDLE PortHandle)
{
    NTSTATUS Status;
    PTERMINATION_PORT TerminationPort;
    PVOID TerminationLpcPort;
    PETHREAD Thread;
    PAGED_CODE();

    /* Get the Port */
    Status = ObReferenceObjectByHandle(PortHandle,
                                       PORT_ALL_ACCESS,
                                       LpcPortObjectType,
                                       KeGetPreviousMode(),
                                       &TerminationLpcPort,
                                       NULL);
    if (!NT_SUCCESS(Status)) return(Status);

    /* Allocate the Port and make sure it suceeded */
    TerminationPort = ExAllocatePoolWithTag(NonPagedPool,
                                            sizeof(TERMINATION_PORT),
                                            TAG('P', 's', 'T', '='));
    if(TerminationPort)
    {
        /* Associate the Port */
        Thread = PsGetCurrentThread();
        TerminationPort->Port = TerminationLpcPort;
        TerminationPort->Next = Thread->TerminationPort;
        Thread->TerminationPort = TerminationPort;

        /* Return success */
        return STATUS_SUCCESS;
    }

    /* Dereference and Fail */
    ObDereferenceObject(TerminationPort);
    return STATUS_INSUFFICIENT_RESOURCES;
}
