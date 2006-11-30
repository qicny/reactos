///////////////////////////////////////////////////////////////////////////////
//
//            Alex's Big Ol' List of FIXMEs, bugs and regressions
//              If you see something here, Alex *KNOWS ABOUT IT*.
//                         Do NOT bug him about it.
//                      Do NOT ask if he knows about it.
//                        Do NOT complain about it.
//                     Do NOT ask when it will be fixed.
//              Failure to respect this will *ACHIEVE NOTHING*.
//
// Io:
//  - See why queueing IRPs and cancelling them causes crashes.
//  - Add Access Checks in IopParseDevice.
//  - Add validation checks in IoCreateFile.
//  - Add probe/alignment checks for Query/Set routines.
//  - Add tracing to iofunc.c
//  - Add tracing to file.c
//  - Add support for some fast-paths when querying/setting data.
//  - Verify ShareAccess APIs, XP added some new semantics.
//  - Add support for Fast Dispatch I/O.
//
// Ob:
//  - Fix bug related to Deferred Loading (don't requeue active work item).
//  - Add Directory Lock.
//
// Fstub:
//  - Implement IoAssignDriveLetters using mount manager support.
//
// Ke:
//  - Figure out why the DPC stack doesn't really work.
//  - Add DR macro/save and VM macro/save.
//  - New optimized table-based tick-hashed timer implementation.
//  - New Thread Scheduler based on 2003.
//  - Implement KiCallbackReturn, KiGetTickCount, KiRaiseAssertion.
//
// Hal:
//  - New IRQL Implementation.
//  - CMOS Initialization and CMOS Spinlock.
//  - Report resource usage to kernel (HalReportResourceUsage).
//
// Lpc:
//  - Activate new NTLPC and delete old implementation.
//  - Figure out why LPC-processes won't die anymore.
//
// Ex:
//  - Implement Generic Callback mechanism.
//  - Use pushlocks for handle implementation.
//
// Kd:
//  - Implement new KDCOM with KD support.
//  - Implement KD Kernel Debugging and WinDBG support.
//
// Native:
//  - Rewrite loader.
//  - Make smss NT-compatible.
//
///////////////////////////////////////////////////////////////////////////////

