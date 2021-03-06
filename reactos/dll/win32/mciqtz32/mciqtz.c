/*
 * DirectShow MCI Driver
 *
 * Copyright 2009 Christian Costa
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#define WIN32_NO_STATUS
#define WIN32_LEAN_AND_MEAN

#include <stdarg.h>
#include <windef.h>
//#include "winbase.h"
//#include "winuser.h"
#include <mmddk.h>
#include <wine/debug.h>
#include "mciqtz_private.h"
#include <digitalv.h>
#include <wownt32.h>

WINE_DEFAULT_DEBUG_CHANNEL(mciqtz);

static DWORD CALLBACK MCIQTZ_taskThread(LPVOID arg);
static DWORD MCIQTZ_mciStop(UINT, DWORD, LPMCI_GENERIC_PARMS);

/*======================================================================*
 *                          MCI QTZ implementation                      *
 *======================================================================*/

static HINSTANCE MCIQTZ_hInstance = 0;

/***********************************************************************
 *              DllMain (MCIQTZ.0)
 */
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID fImpLoad)
{
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hInstDLL);
        MCIQTZ_hInstance = hInstDLL;
        break;
    }
    return TRUE;
}

/**************************************************************************
 *                              MCIQTZ_mciGetOpenDev            [internal]
 */
static WINE_MCIQTZ* MCIQTZ_mciGetOpenDev(UINT wDevID)
{
    WINE_MCIQTZ* wma = (WINE_MCIQTZ*)mciGetDriverData(wDevID);

    if (!wma) {
        WARN("Invalid wDevID=%u\n", wDevID);
        return NULL;
    }
    return wma;
}

/***************************************************************************
 *                              MCIQTZ_relayTaskMessage         [internal]
 */
static LRESULT MCIQTZ_relayTaskMessage(DWORD_PTR dwDevID, UINT wMsg,
                                      DWORD dwFlags, LPARAM lpParms)
{
    WINE_MCIQTZ *wma;
    LRESULT res;
    HANDLE handles[2];
    DWORD ret;
    TRACE("(%08lX, %08x, %08x, %08lx)\n", dwDevID, wMsg, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(dwDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;
    EnterCriticalSection(&wma->cs);
    wma->task.devid = dwDevID;
    wma->task.msg   = wMsg;
    wma->task.flags = dwFlags;
    wma->task.parms = lpParms;
    SetEvent(wma->task.notify);
    handles[0] = wma->task.done;
    handles[1] = wma->task.thread;
    ret = WaitForMultipleObjects(sizeof(handles)/sizeof(handles[0]), handles,
                                 FALSE, INFINITE);
    if (ret == WAIT_OBJECT_0)
        res = wma->task.res;
    else
        res = MCIERR_INTERNAL;
    LeaveCriticalSection(&wma->cs);

    return res;
}

/**************************************************************************
 *                              MCIQTZ_drvOpen                  [internal]
 */
static DWORD MCIQTZ_drvOpen(LPCWSTR str, LPMCI_OPEN_DRIVER_PARMSW modp)
{
    WINE_MCIQTZ* wma;
    static const WCHAR mciAviWStr[] = {'M','C','I','A','V','I',0};

    TRACE("(%s, %p)\n", debugstr_w(str), modp);

    /* session instance */
    if (!modp)
        return 0xFFFFFFFF;

    wma = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(WINE_MCIQTZ));
    if (!wma)
        return 0;

    wma->stop_event = CreateEventW(NULL, FALSE, FALSE, NULL);
    wma->task.notify = CreateEventW(NULL, FALSE, FALSE, NULL);
    if (!wma->task.notify) goto err;
    wma->task.done = CreateEventW(NULL, FALSE, FALSE, NULL);
    if (!wma->task.done)   goto err;
    wma->task.thread = CreateThread(NULL, 0, MCIQTZ_taskThread, &wma->task, 0, NULL);
    if (!wma->task.thread) goto err;
    InitializeCriticalSection(&wma->cs);
    wma->cs.DebugInfo->Spare[0] = (DWORD_PTR)(__FILE__ ": WINE_MCIQTZ");
    modp->wType = MCI_DEVTYPE_DIGITAL_VIDEO;
    wma->wDevID = modp->wDeviceID;
    modp->wCustomCommandTable = wma->command_table = mciLoadCommandResource(MCIQTZ_hInstance, mciAviWStr, 0);
    mciSetDriverData(wma->wDevID, (DWORD_PTR)wma);

    return modp->wDeviceID;
err:
    if (wma->task.notify) CloseHandle(wma->task.notify);
    if (wma->task.done)   CloseHandle(wma->task.done);
    HeapFree(GetProcessHeap(), 0, wma);
    return 0;
}

/**************************************************************************
 *                              MCIQTZ_drvClose         [internal]
 */
static DWORD MCIQTZ_drvClose(DWORD dwDevID)
{
    WINE_MCIQTZ* wma;

    TRACE("(%04x)\n", dwDevID);

    wma = MCIQTZ_mciGetOpenDev(dwDevID);

    if (wma) {
        /* finish all outstanding things */
        MCIQTZ_relayTaskMessage(dwDevID, MCI_CLOSE_DRIVER, MCI_WAIT, 0);

        mciFreeCommandResource(wma->command_table);
        MCIQTZ_relayTaskMessage(dwDevID, MCI_CLOSE, MCI_WAIT, 0);
        WaitForSingleObject(wma->task.thread, INFINITE);
        CloseHandle(wma->task.notify);
        CloseHandle(wma->task.done);
        CloseHandle(wma->task.thread);
        DeleteCriticalSection(&wma->cs);
        mciSetDriverData(dwDevID, 0);
        CloseHandle(wma->stop_event);
        HeapFree(GetProcessHeap(), 0, wma);
        return 1;
    }

    return (dwDevID == 0xFFFFFFFF) ? 1 : 0;
}

/**************************************************************************
 *                              MCIQTZ_drvConfigure             [internal]
 */
static DWORD MCIQTZ_drvConfigure(DWORD dwDevID)
{
    WINE_MCIQTZ* wma;

    TRACE("(%04x)\n", dwDevID);

    wma = MCIQTZ_mciGetOpenDev(dwDevID);
    if (!wma)
        return 0;

    MCIQTZ_mciStop(dwDevID, MCI_WAIT, NULL);

    MessageBoxA(0, "Sample QTZ Wine Driver !", "MM-Wine Driver", MB_OK);

    return 1;
}

/**************************************************************************
 *                              MCIQTZ_mciNotify                [internal]
 *
 * Notifications in MCI work like a 1-element queue.
 * Each new notification request supersedes the previous one.
 */
static void MCIQTZ_mciNotify(DWORD_PTR hWndCallBack, WINE_MCIQTZ* wma, UINT wStatus)
{
    MCIDEVICEID wDevID = wma->notify_devid;
    HANDLE old = InterlockedExchangePointer(&wma->callback, NULL);
    if (old) mciDriverNotify(old, wDevID, MCI_NOTIFY_SUPERSEDED);
    mciDriverNotify(HWND_32(LOWORD(hWndCallBack)), wDevID, wStatus);
}

/***************************************************************************
 *                              MCIQTZ_mciOpen                  [internal]
 */
static DWORD MCIQTZ_mciOpen(UINT wDevID, DWORD dwFlags,
                            LPMCI_DGV_OPEN_PARMSW lpOpenParms)
{
    WINE_MCIQTZ* wma;
    HRESULT hr;
    DWORD style = 0;
    RECT rc = { 0, 0, 0, 0 };

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpOpenParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    MCIQTZ_mciStop(wDevID, MCI_WAIT, NULL);

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    wma->uninit = SUCCEEDED(hr);

    hr = CoCreateInstance(&CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, &IID_IGraphBuilder, (LPVOID*)&wma->pgraph);
    if (FAILED(hr)) {
        TRACE("Cannot create filtergraph (hr = %x)\n", hr);
        goto err;
    }

    hr = IGraphBuilder_QueryInterface(wma->pgraph, &IID_IMediaControl, (LPVOID*)&wma->pmctrl);
    if (FAILED(hr)) {
        TRACE("Cannot get IMediaControl interface (hr = %x)\n", hr);
        goto err;
    }

    hr = IGraphBuilder_QueryInterface(wma->pgraph, &IID_IMediaSeeking, (void**)&wma->seek);
    if (FAILED(hr)) {
        TRACE("Cannot get IMediaSeeking interface (hr = %x)\n", hr);
        goto err;
    }

    hr = IGraphBuilder_QueryInterface(wma->pgraph, &IID_IMediaEvent, (void**)&wma->mevent);
    if (FAILED(hr)) {
        TRACE("Cannot get IMediaEvent interface (hr = %x)\n", hr);
        goto err;
    }

    hr = IGraphBuilder_QueryInterface(wma->pgraph, &IID_IVideoWindow, (void**)&wma->vidwin);
    if (FAILED(hr)) {
        TRACE("Cannot get IVideoWindow interface (hr = %x)\n", hr);
        goto err;
    }

    hr = IGraphBuilder_QueryInterface(wma->pgraph, &IID_IBasicVideo, (void**)&wma->vidbasic);
    if (FAILED(hr)) {
        TRACE("Cannot get IBasicVideo interface (hr = %x)\n", hr);
        goto err;
    }

    hr = IGraphBuilder_QueryInterface(wma->pgraph, &IID_IBasicAudio, (void**)&wma->audio);
    if (FAILED(hr)) {
        TRACE("Cannot get IBasicAudio interface (hr = %x)\n", hr);
        goto err;
    }

    if (!(dwFlags & MCI_OPEN_ELEMENT) || (dwFlags & MCI_OPEN_ELEMENT_ID)) {
        TRACE("Wrong dwFlags %x\n", dwFlags);
        goto err;
    }

    if (!lpOpenParms->lpstrElementName || !lpOpenParms->lpstrElementName[0]) {
        TRACE("Invalid filename specified\n");
        goto err;
    }

    TRACE("Open file %s\n", debugstr_w(lpOpenParms->lpstrElementName));

    hr = IGraphBuilder_RenderFile(wma->pgraph, lpOpenParms->lpstrElementName, NULL);
    if (FAILED(hr)) {
        TRACE("Cannot render file (hr = %x)\n", hr);
        goto err;
    }

    IVideoWindow_put_AutoShow(wma->vidwin, OAFALSE);
    IVideoWindow_put_Visible(wma->vidwin, OAFALSE);
    if (dwFlags & MCI_DGV_OPEN_WS)
        style = lpOpenParms->dwStyle;
    if (dwFlags & MCI_DGV_OPEN_PARENT) {
        IVideoWindow_put_MessageDrain(wma->vidwin, (OAHWND)lpOpenParms->hWndParent);
        IVideoWindow_put_WindowState(wma->vidwin, SW_HIDE);
        IVideoWindow_put_WindowStyle(wma->vidwin, style|WS_CHILD);
        IVideoWindow_put_Owner(wma->vidwin, (OAHWND)lpOpenParms->hWndParent);
        GetClientRect(lpOpenParms->hWndParent, &rc);
        IVideoWindow_SetWindowPosition(wma->vidwin, rc.left, rc.top, rc.right - rc.top, rc.bottom - rc.top);
        wma->parent = (HWND)lpOpenParms->hWndParent;
    }
    else if (style)
        IVideoWindow_put_WindowStyle(wma->vidwin, style);
    IBasicVideo_GetVideoSize(wma->vidbasic, &rc.right, &rc.bottom);
    wma->opened = TRUE;

    if (dwFlags & MCI_NOTIFY)
        mciDriverNotify(HWND_32(LOWORD(lpOpenParms->dwCallback)), wDevID, MCI_NOTIFY_SUCCESSFUL);

    return 0;

err:
    if (wma->audio)
        IBasicAudio_Release(wma->audio);
    wma->audio = NULL;
    if (wma->vidbasic)
        IBasicVideo_Release(wma->vidbasic);
    wma->vidbasic = NULL;
    if (wma->seek)
        IMediaSeeking_Release(wma->seek);
    wma->seek = NULL;
    if (wma->vidwin)
        IVideoWindow_Release(wma->vidwin);
    wma->vidwin = NULL;
    if (wma->pgraph)
        IGraphBuilder_Release(wma->pgraph);
    wma->pgraph = NULL;
    if (wma->mevent)
        IMediaEvent_Release(wma->mevent);
    wma->mevent = NULL;
    if (wma->pmctrl)
        IMediaControl_Release(wma->pmctrl);
    wma->pmctrl = NULL;

    if (wma->uninit)
        CoUninitialize();
    wma->uninit = FALSE;

    return MCIERR_INTERNAL;
}

/***************************************************************************
 *                              MCIQTZ_mciClose                 [internal]
 */
static DWORD MCIQTZ_mciClose(UINT wDevID, DWORD dwFlags, LPMCI_GENERIC_PARMS lpParms)
{
    WINE_MCIQTZ* wma;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    MCIQTZ_mciStop(wDevID, MCI_WAIT, NULL);

    if (wma->opened) {
        IVideoWindow_Release(wma->vidwin);
        IBasicVideo_Release(wma->vidbasic);
        IBasicAudio_Release(wma->audio);
        IMediaSeeking_Release(wma->seek);
        IMediaEvent_Release(wma->mevent);
        IGraphBuilder_Release(wma->pgraph);
        IMediaControl_Release(wma->pmctrl);
        if (wma->uninit)
            CoUninitialize();
        wma->opened = FALSE;
    }

    return 0;
}

/***************************************************************************
 *                              MCIQTZ_notifyThread             [internal]
 */
static DWORD CALLBACK MCIQTZ_notifyThread(LPVOID parm)
{
    WINE_MCIQTZ* wma = (WINE_MCIQTZ *)parm;
    HRESULT hr;
    HANDLE handle[2];
    DWORD n = 0, ret = 0;

    handle[n++] = wma->stop_event;
    IMediaEvent_GetEventHandle(wma->mevent, (OAEVENT *)&handle[n++]);

    for (;;) {
        DWORD r;
        HANDLE old;

        r = WaitForMultipleObjects(n, handle, FALSE, INFINITE);
        if (r == WAIT_OBJECT_0) {
            TRACE("got stop event\n");
            old = InterlockedExchangePointer(&wma->callback, NULL);
            if (old)
                mciDriverNotify(old, wma->notify_devid, MCI_NOTIFY_ABORTED);
            break;
        }
        else if (r == WAIT_OBJECT_0+1) {
            LONG event_code;
            LONG_PTR p1, p2;
            do {
                hr = IMediaEvent_GetEvent(wma->mevent, &event_code, &p1, &p2, 0);
                if (SUCCEEDED(hr)) {
                    TRACE("got event_code = 0x%02x\n", event_code);
                    IMediaEvent_FreeEventParams(wma->mevent, event_code, p1, p2);
                }
            } while (hr == S_OK && event_code != EC_COMPLETE);
            if (hr == S_OK && event_code == EC_COMPLETE) {
                old = InterlockedExchangePointer(&wma->callback, NULL);
                if (old)
                    mciDriverNotify(old, wma->notify_devid, MCI_NOTIFY_SUCCESSFUL);
                break;
            }
        }
        else {
            TRACE("Unknown error (%d)\n", (int)r);
            break;
        }
    }

    hr = IMediaControl_Stop(wma->pmctrl);
    if (FAILED(hr)) {
        TRACE("Cannot stop filtergraph (hr = %x)\n", hr);
        ret = MCIERR_INTERNAL;
    }

    return ret;
}

/***************************************************************************
 *                              MCIQTZ_mciPlay                  [internal]
 */
static DWORD MCIQTZ_mciPlay(UINT wDevID, DWORD dwFlags, LPMCI_PLAY_PARMS lpParms)
{
    WINE_MCIQTZ* wma;
    HRESULT hr;
    REFERENCE_TIME time1 = 0, time2 = 0;
    GUID format;
    DWORD pos1;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    ResetEvent(wma->stop_event);
    if (dwFlags & MCI_NOTIFY) {
        HANDLE old;
        old = InterlockedExchangePointer(&wma->callback, HWND_32(LOWORD(lpParms->dwCallback)));
        if (old)
            mciDriverNotify(old, wma->notify_devid, MCI_NOTIFY_ABORTED);
    }

    IMediaSeeking_GetTimeFormat(wma->seek, &format);
    if (dwFlags & MCI_FROM) {
        if (IsEqualGUID(&format, &TIME_FORMAT_MEDIA_TIME))
            time1 = lpParms->dwFrom * 10000;
        else
            time1 = lpParms->dwFrom;
        pos1 = AM_SEEKING_AbsolutePositioning;
    } else
        pos1 = AM_SEEKING_NoPositioning;
    if (dwFlags & MCI_TO) {
        if (IsEqualGUID(&format, &TIME_FORMAT_MEDIA_TIME))
            time2 = lpParms->dwTo * 10000;
        else
            time2 = lpParms->dwTo;
    } else
        IMediaSeeking_GetDuration(wma->seek, &time2);
    IMediaSeeking_SetPositions(wma->seek, &time1, pos1, &time2, AM_SEEKING_AbsolutePositioning);

    hr = IMediaControl_Run(wma->pmctrl);
    if (FAILED(hr)) {
        TRACE("Cannot run filtergraph (hr = %x)\n", hr);
        return MCIERR_INTERNAL;
    }

    IVideoWindow_put_Visible(wma->vidwin, OATRUE);

    wma->thread = CreateThread(NULL, 0, MCIQTZ_notifyThread, wma, 0, NULL);
    if (!wma->thread) {
        TRACE("Can't create thread\n");
        return MCIERR_INTERNAL;
    }
    return 0;
}

/***************************************************************************
 *                              MCIQTZ_mciSeek                  [internal]
 */
static DWORD MCIQTZ_mciSeek(UINT wDevID, DWORD dwFlags, LPMCI_SEEK_PARMS lpParms)
{
    WINE_MCIQTZ* wma;
    HRESULT hr;
    LONGLONG newpos;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    MCIQTZ_mciStop(wDevID, MCI_WAIT, NULL);

    if (dwFlags & MCI_SEEK_TO_START) {
        newpos = 0;
    } else if (dwFlags & MCI_SEEK_TO_END) {
        FIXME("MCI_SEEK_TO_END not implemented yet\n");
        return MCIERR_INTERNAL;
    } else if (dwFlags & MCI_TO) {
        FIXME("MCI_TO not implemented yet\n");
        return MCIERR_INTERNAL;
    } else {
        WARN("dwFlag doesn't tell where to seek to...\n");
        return MCIERR_MISSING_PARAMETER;
    }

    hr = IMediaSeeking_SetPositions(wma->seek, &newpos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
    if (FAILED(hr)) {
        FIXME("Cannot set position (hr = %x)\n", hr);
        return MCIERR_INTERNAL;
    }

    if (dwFlags & MCI_NOTIFY)
        MCIQTZ_mciNotify(lpParms->dwCallback, wma, MCI_NOTIFY_SUCCESSFUL);

    return 0;
}

/***************************************************************************
 *                              MCIQTZ_mciStop                  [internal]
 */
static DWORD MCIQTZ_mciStop(UINT wDevID, DWORD dwFlags, LPMCI_GENERIC_PARMS lpParms)
{
    WINE_MCIQTZ* wma;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    if (!wma->opened)
        return 0;

    if (wma->thread) {
        SetEvent(wma->stop_event);
        WaitForSingleObject(wma->thread, INFINITE);
        CloseHandle(wma->thread);
        wma->thread = NULL;
    }

    if (!wma->parent)
        IVideoWindow_put_Visible(wma->vidwin, OAFALSE);

    return 0;
}

/***************************************************************************
 *                              MCIQTZ_mciPause                 [internal]
 */
static DWORD MCIQTZ_mciPause(UINT wDevID, DWORD dwFlags, LPMCI_GENERIC_PARMS lpParms)
{
    WINE_MCIQTZ* wma;
    HRESULT hr;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    hr = IMediaControl_Pause(wma->pmctrl);
    if (FAILED(hr)) {
        TRACE("Cannot pause filtergraph (hr = %x)\n", hr);
        return MCIERR_INTERNAL;
    }

    return 0;
}

/***************************************************************************
 *                              MCIQTZ_mciGetDevCaps            [internal]
 */
static DWORD MCIQTZ_mciGetDevCaps(UINT wDevID, DWORD dwFlags, LPMCI_GETDEVCAPS_PARMS lpParms)
{
    WINE_MCIQTZ* wma;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    if (!(dwFlags & MCI_GETDEVCAPS_ITEM))
        return MCIERR_MISSING_PARAMETER;

    switch (lpParms->dwItem) {
        case MCI_GETDEVCAPS_CAN_RECORD:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_GETDEVCAPS_CAN_RECORD = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_HAS_AUDIO:
            lpParms->dwReturn = MAKEMCIRESOURCE(TRUE, MCI_TRUE);
            TRACE("MCI_GETDEVCAPS_HAS_AUDIO = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_HAS_VIDEO:
            lpParms->dwReturn = MAKEMCIRESOURCE(TRUE, MCI_TRUE);
            TRACE("MCI_GETDEVCAPS_HAS_VIDEO = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_DEVICE_TYPE:
            lpParms->dwReturn = MAKEMCIRESOURCE(MCI_DEVTYPE_DIGITAL_VIDEO, MCI_DEVTYPE_DIGITAL_VIDEO);
            TRACE("MCI_GETDEVCAPS_DEVICE_TYPE = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_USES_FILES:
            lpParms->dwReturn = MAKEMCIRESOURCE(TRUE, MCI_TRUE);
            TRACE("MCI_GETDEVCAPS_USES_FILES = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_COMPOUND_DEVICE:
            lpParms->dwReturn = MAKEMCIRESOURCE(TRUE, MCI_TRUE);
            TRACE("MCI_GETDEVCAPS_COMPOUND_DEVICE = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_CAN_EJECT:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_GETDEVCAPS_EJECT = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_CAN_PLAY:
            lpParms->dwReturn = MAKEMCIRESOURCE(TRUE, MCI_TRUE);
            TRACE("MCI_GETDEVCAPS_CAN_PLAY = %08x\n", lpParms->dwReturn);
            break;
        case MCI_GETDEVCAPS_CAN_SAVE:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_GETDEVCAPS_CAN_SAVE = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_CAN_REVERSE:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_DGV_GETDEVCAPS_CAN_REVERSE = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_CAN_STRETCH:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE); /* FIXME */
            TRACE("MCI_DGV_GETDEVCAPS_CAN_STRETCH = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_CAN_LOCK:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_DGV_GETDEVCAPS_CAN_LOCK = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_CAN_FREEZE:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_DGV_GETDEVCAPS_CAN_FREEZE = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_CAN_STR_IN:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_DGV_GETDEVCAPS_CAN_STRETCH_INPUT = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_HAS_STILL:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE);
            TRACE("MCI_DGV_GETDEVCAPS_HAS_STILL = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_CAN_TEST:
            lpParms->dwReturn = MAKEMCIRESOURCE(FALSE, MCI_FALSE); /* FIXME */
            TRACE("MCI_DGV_GETDEVCAPS_CAN_TEST = %08x\n", lpParms->dwReturn);
            break;
        case MCI_DGV_GETDEVCAPS_MAX_WINDOWS:
            lpParms->dwReturn = 1;
            TRACE("MCI_DGV_GETDEVCAPS_MAX_WINDOWS = %u\n", lpParms->dwReturn);
            return 0;
        default:
            WARN("Unknown capability %08x\n", lpParms->dwItem);
            /* Fall through */
        case MCI_DGV_GETDEVCAPS_MAXIMUM_RATE: /* unknown to w2k */
        case MCI_DGV_GETDEVCAPS_MINIMUM_RATE: /* unknown to w2k */
            return MCIERR_UNSUPPORTED_FUNCTION;
    }

    return MCI_RESOURCE_RETURNED;
}

/***************************************************************************
 *                              MCIQTZ_mciSet                   [internal]
 */
static DWORD MCIQTZ_mciSet(UINT wDevID, DWORD dwFlags, LPMCI_DGV_SET_PARMS lpParms)
{
    WINE_MCIQTZ* wma;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    if (dwFlags & MCI_SET_TIME_FORMAT) {
        switch (lpParms->dwTimeFormat) {
            case MCI_FORMAT_MILLISECONDS:
                TRACE("MCI_SET_TIME_FORMAT = MCI_FORMAT_MILLISECONDS\n");
                wma->time_format = MCI_FORMAT_MILLISECONDS;
                break;
            case MCI_FORMAT_FRAMES:
                TRACE("MCI_SET_TIME_FORMAT = MCI_FORMAT_FRAMES\n");
                wma->time_format = MCI_FORMAT_FRAMES;
                break;
            default:
                WARN("Bad time format %u\n", lpParms->dwTimeFormat);
                return MCIERR_BAD_TIME_FORMAT;
        }
    }

    if (dwFlags & MCI_SET_DOOR_OPEN)
        FIXME("MCI_SET_DOOR_OPEN not implemented yet\n");
    if (dwFlags & MCI_SET_DOOR_CLOSED)
        FIXME("MCI_SET_DOOR_CLOSED not implemented yet\n");
    if (dwFlags & MCI_SET_AUDIO)
        FIXME("MCI_SET_AUDIO not implemented yet\n");
    if (dwFlags & MCI_SET_VIDEO)
        FIXME("MCI_SET_VIDEO not implemented yet\n");
    if (dwFlags & MCI_SET_ON)
        FIXME("MCI_SET_ON not implemented yet\n");
    if (dwFlags & MCI_SET_OFF)
        FIXME("MCI_SET_OFF not implemented yet\n");
    if (dwFlags & MCI_SET_AUDIO_LEFT)
        FIXME("MCI_SET_AUDIO_LEFT not implemented yet\n");
    if (dwFlags & MCI_SET_AUDIO_RIGHT)
        FIXME("MCI_SET_AUDIO_RIGHT not implemented yet\n");

    if (dwFlags & ~0x7f03 /* All MCI_SET flags mask */)
        ERR("Unknown flags %08x\n", dwFlags & ~0x7f03);

    return 0;
}

/***************************************************************************
 *                              MCIQTZ_mciStatus                [internal]
 */
static DWORD MCIQTZ_mciStatus(UINT wDevID, DWORD dwFlags, LPMCI_DGV_STATUS_PARMSW lpParms)
{
    WINE_MCIQTZ* wma;
    HRESULT hr;
    DWORD ret = MCI_INTEGER_RETURNED;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    if (!(dwFlags & MCI_STATUS_ITEM)) {
        WARN("No status item specified\n");
        return MCIERR_UNRECOGNIZED_COMMAND;
    }

    switch (lpParms->dwItem) {
        case MCI_STATUS_LENGTH: {
            LONGLONG duration = -1;
            GUID format;
            switch (wma->time_format) {
                case MCI_FORMAT_MILLISECONDS: format = TIME_FORMAT_MEDIA_TIME; break;
                case MCI_FORMAT_FRAMES: format = TIME_FORMAT_FRAME; break;
                default: ERR("Unhandled format %x\n", wma->time_format); break;
            }
            hr = IMediaSeeking_SetTimeFormat(wma->seek, &format);
            if (FAILED(hr)) {
                FIXME("Cannot set time format (hr = %x)\n", hr);
                lpParms->dwReturn = 0;
                break;
            }
            hr = IMediaSeeking_GetDuration(wma->seek, &duration);
            if (FAILED(hr) || duration < 0) {
                FIXME("Cannot read duration (hr = %x)\n", hr);
                lpParms->dwReturn = 0;
            } else if (wma->time_format != MCI_FORMAT_MILLISECONDS)
                lpParms->dwReturn = duration;
            else
                lpParms->dwReturn = duration / 10000;
            break;
        }
        case MCI_STATUS_POSITION: {
            REFERENCE_TIME curpos;

            hr = IMediaSeeking_GetCurrentPosition(wma->seek, &curpos);
            if (FAILED(hr)) {
                FIXME("Cannot get position (hr = %x)\n", hr);
                return MCIERR_INTERNAL;
            }
            lpParms->dwReturn = curpos / 10000;
            break;
        }
        case MCI_STATUS_NUMBER_OF_TRACKS:
            FIXME("MCI_STATUS_NUMBER_OF_TRACKS not implemented yet\n");
            return MCIERR_UNRECOGNIZED_COMMAND;
        case MCI_STATUS_MODE: {
            LONG state = State_Stopped;
            IMediaControl_GetState(wma->pmctrl, -1, &state);
            if (state == State_Stopped)
                lpParms->dwReturn = MAKEMCIRESOURCE(MCI_MODE_STOP, MCI_MODE_STOP);
            else if (state == State_Running) {
                lpParms->dwReturn = MAKEMCIRESOURCE(MCI_MODE_PLAY, MCI_MODE_PLAY);
                if (!wma->thread || WaitForSingleObject(wma->thread, 0) == WAIT_OBJECT_0)
                    lpParms->dwReturn = MAKEMCIRESOURCE(MCI_MODE_STOP, MCI_MODE_STOP);
            } else if (state == State_Paused)
                lpParms->dwReturn = MAKEMCIRESOURCE(MCI_MODE_PAUSE, MCI_MODE_PAUSE);
            ret = MCI_RESOURCE_RETURNED;
            break;
        }
        case MCI_STATUS_MEDIA_PRESENT:
            FIXME("MCI_STATUS_MEDIA_PRESENT not implemented yet\n");
            return MCIERR_UNRECOGNIZED_COMMAND;
        case MCI_STATUS_TIME_FORMAT:
            lpParms->dwReturn = MAKEMCIRESOURCE(wma->time_format,
                                                MCI_FORMAT_RETURN_BASE + wma->time_format);
            ret = MCI_RESOURCE_RETURNED;
            break;
        case MCI_STATUS_READY:
            FIXME("MCI_STATUS_READY not implemented yet\n");
            return MCIERR_UNRECOGNIZED_COMMAND;
        case MCI_STATUS_CURRENT_TRACK:
            FIXME("MCI_STATUS_CURRENT_TRACK not implemented yet\n");
            return MCIERR_UNRECOGNIZED_COMMAND;
        default:
            FIXME("Unknown command %08X\n", lpParms->dwItem);
            return MCIERR_UNRECOGNIZED_COMMAND;
    }

    if (dwFlags & MCI_NOTIFY)
        MCIQTZ_mciNotify(lpParms->dwCallback, wma, MCI_NOTIFY_SUCCESSFUL);

    return ret;
}

/***************************************************************************
 *                              MCIQTZ_mciWhere                 [internal]
 */
static DWORD MCIQTZ_mciWhere(UINT wDevID, DWORD dwFlags, LPMCI_DGV_RECT_PARMS lpParms)
{
    WINE_MCIQTZ* wma;
    HRESULT hr;
    HWND hWnd;
    RECT rc;
    DWORD ret = MCIERR_UNRECOGNIZED_COMMAND;

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    hr = IVideoWindow_get_Owner(wma->vidwin, (OAHWND*)&hWnd);
    if (FAILED(hr)) {
        TRACE("No video stream, returning no window error\n");
        return MCIERR_NO_WINDOW;
    }

    if (dwFlags & MCI_DGV_WHERE_SOURCE) {
        if (dwFlags & MCI_DGV_WHERE_MAX)
            FIXME("MCI_DGV_WHERE_SOURCE_MAX stub\n");
        IBasicVideo_GetSourcePosition(wma->vidbasic, &rc.left, &rc.top, &rc.right, &rc.bottom);
        TRACE("MCI_DGV_WHERE_SOURCE %s\n", wine_dbgstr_rect(&rc));
    }
    if (dwFlags & MCI_DGV_WHERE_DESTINATION) {
        if (dwFlags & MCI_DGV_WHERE_MAX)
            FIXME("MCI_DGV_WHERE_DESTINATION_MAX stub\n");
        IBasicVideo_GetDestinationPosition(wma->vidbasic, &rc.left, &rc.top, &rc.right, &rc.bottom);
        TRACE("MCI_DGV_WHERE_DESTINATION %s\n", wine_dbgstr_rect(&rc));
    }
    if (dwFlags & MCI_DGV_WHERE_FRAME) {
        if (dwFlags & MCI_DGV_WHERE_MAX)
            FIXME("MCI_DGV_WHERE_FRAME_MAX not supported yet\n");
        else
            FIXME("MCI_DGV_WHERE_FRAME not supported yet\n");
        goto out;
    }
    if (dwFlags & MCI_DGV_WHERE_VIDEO) {
        if (dwFlags & MCI_DGV_WHERE_MAX)
            FIXME("MCI_DGV_WHERE_VIDEO_MAX not supported yet\n");
        else
            FIXME("MCI_DGV_WHERE_VIDEO not supported yet\n");
        goto out;
    }
    if (dwFlags & MCI_DGV_WHERE_WINDOW) {
        if (dwFlags & MCI_DGV_WHERE_MAX) {
            GetWindowRect(GetDesktopWindow(), &rc);
            rc.right -= rc.left;
            rc.bottom -= rc.top;
            TRACE("MCI_DGV_WHERE_WINDOW_MAX %s\n", wine_dbgstr_rect(&rc));
        } else {
            IVideoWindow_GetWindowPosition(wma->vidwin, &rc.left, &rc.top, &rc.right, &rc.bottom);
            TRACE("MCI_DGV_WHERE_WINDOW %s\n", wine_dbgstr_rect(&rc));
        }
    }
    ret = 0;
out:
    lpParms->rc = rc;
    return ret;
}

/***************************************************************************
 *                              MCIQTZ_mciWindow                [internal]
 */
static DWORD MCIQTZ_mciWindow(UINT wDevID, DWORD dwFlags, LPMCI_DGV_WINDOW_PARMSW lpParms)
{
    WINE_MCIQTZ *wma = MCIQTZ_mciGetOpenDev(wDevID);

    TRACE("(%04x, %08X, %p)\n", wDevID, dwFlags, lpParms);

    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;
    if (dwFlags & MCI_TEST)
        return 0;

    if (dwFlags & MCI_DGV_WINDOW_HWND && (IsWindow(lpParms->hWnd) || !lpParms->hWnd)) {
        LONG visible = OATRUE;
        LONG style = 0;
        TRACE("Setting hWnd to %p\n", lpParms->hWnd);
        IVideoWindow_get_Visible(wma->vidwin, &visible);
        IVideoWindow_put_Visible(wma->vidwin, OAFALSE);
        IVideoWindow_get_WindowStyle(wma->vidwin, &style);
        style &= ~WS_CHILD;
        if (lpParms->hWnd)
            IVideoWindow_put_WindowStyle(wma->vidwin, style|WS_CHILD);
        else
            IVideoWindow_put_WindowStyle(wma->vidwin, style);
        IVideoWindow_put_Owner(wma->vidwin, (OAHWND)lpParms->hWnd);
        IVideoWindow_put_MessageDrain(wma->vidwin, (OAHWND)lpParms->hWnd);
        IVideoWindow_put_Visible(wma->vidwin, visible);
        wma->parent = lpParms->hWnd;
    }
    if (dwFlags & MCI_DGV_WINDOW_STATE) {
        TRACE("Setting nCmdShow to %d\n", lpParms->nCmdShow);
        IVideoWindow_put_WindowState(wma->vidwin, lpParms->nCmdShow);
    }
    if (dwFlags & MCI_DGV_WINDOW_TEXT) {
        TRACE("Setting caption to %s\n", debugstr_w(lpParms->lpstrText));
        IVideoWindow_put_Caption(wma->vidwin, lpParms->lpstrText);
    }
    return 0;
}

/******************************************************************************
 *              MCIAVI_mciUpdate            [internal]
 */
static DWORD MCIQTZ_mciUpdate(UINT wDevID, DWORD dwFlags, LPMCI_DGV_UPDATE_PARMS lpParms)
{
    WINE_MCIQTZ *wma;
    DWORD res = 0;

    TRACE("%04x, %08x, %p\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    if (dwFlags & MCI_DGV_UPDATE_HDC) {
        LONG state, size;
        BYTE *data;
        BITMAPINFO *info;
        HRESULT hr;
        RECT src, dest;
        LONG visible = OATRUE;

        res = MCIERR_INTERNAL;
        IMediaControl_GetState(wma->pmctrl, -1, &state);
        if (state == State_Running)
            return MCIERR_UNSUPPORTED_FUNCTION;
        /* If in stopped state, nothing has been drawn to screen
         * moving to pause, which is needed for the old dib renderer, will result
         * in a single frame drawn, so hide the window here */
        IVideoWindow_get_Visible(wma->vidwin, &visible);
        if (wma->parent)
            IVideoWindow_put_Visible(wma->vidwin, OAFALSE);
        /* FIXME: Should we check the original state and restore it? */
        IMediaControl_Pause(wma->pmctrl);
        IMediaControl_GetState(wma->pmctrl, -1, &state);
        if (FAILED(hr = IBasicVideo_GetCurrentImage(wma->vidbasic, &size, NULL))) {
            WARN("Could not get image size (hr = %x)\n", hr);
            goto out;
        }
        data = HeapAlloc(GetProcessHeap(), 0, size);
        info = (BITMAPINFO*)data;
        IBasicVideo_GetCurrentImage(wma->vidbasic, &size, (LONG*)data);
        data += info->bmiHeader.biSize;

        IBasicVideo_GetSourcePosition(wma->vidbasic, &src.left, &src.top, &src.right, &src.bottom);
        IBasicVideo_GetDestinationPosition(wma->vidbasic, &dest.left, &dest.top, &dest.right, &dest.bottom);
        StretchDIBits(lpParms->hDC,
              dest.left, dest.top, dest.right + dest.left, dest.bottom + dest.top,
              src.left, src.top, src.right + src.left, src.bottom + src.top,
              data, info, DIB_RGB_COLORS, SRCCOPY);
        HeapFree(GetProcessHeap(), 0, data);
        res = 0;
out:
        if (wma->parent)
            IVideoWindow_put_Visible(wma->vidwin, visible);
    }
    else if (dwFlags)
        FIXME("Unhandled flags %x\n", dwFlags);
    return res;
}

/***************************************************************************
 *                              MCIQTZ_mciSetAudio              [internal]
 */
static DWORD MCIQTZ_mciSetAudio(UINT wDevID, DWORD dwFlags, LPMCI_DGV_SETAUDIO_PARMSW lpParms)
{
    WINE_MCIQTZ *wma;
    DWORD ret = 0;

    TRACE("(%04x, %08x, %p)\n", wDevID, dwFlags, lpParms);

    wma = MCIQTZ_mciGetOpenDev(wDevID);
    if (!wma)
        return MCIERR_INVALID_DEVICE_ID;

    if (!(dwFlags & MCI_DGV_SETAUDIO_ITEM)) {
        FIXME("Unknown flags (%08x)\n", dwFlags);
        return 0;
    }

    if (dwFlags & MCI_DGV_SETAUDIO_ITEM) {
        switch (lpParms->dwItem) {
        case MCI_DGV_SETAUDIO_VOLUME:
            if (dwFlags & MCI_DGV_SETAUDIO_VALUE) {
                long vol = -10000;
                HRESULT hr;
                if (lpParms->dwValue > 1000) {
                    ret = MCIERR_OUTOFRANGE;
                    break;
                }
                if (dwFlags & MCI_TEST)
                    break;
                vol += (long)lpParms->dwValue * 10;
                hr = IBasicAudio_put_Volume(wma->audio, vol);
                if (FAILED(hr)) {
                    WARN("Cannot set volume (hr = %x)\n", hr);
                    ret = MCIERR_INTERNAL;
                }
            }
            break;
        default:
            FIXME("Unknown item %08x\n", lpParms->dwItem);
            break;
        }
    }

    return ret;
}

/***************************************************************************
 *                              MCIQTZ_taskThread               [internal]
 */
static DWORD CALLBACK MCIQTZ_taskThread(LPVOID arg)
{
    WINE_MCIQTZ_TASK *task = (WINE_MCIQTZ_TASK *)arg;

    for (;;) {
        DWORD ret = WaitForSingleObject(task->notify, INFINITE);
        if (ret != WAIT_OBJECT_0) {
            TRACE("Got error (%u)\n", ret);
            continue;
        }

        switch (task->msg) {
        case MCI_OPEN_DRIVER:
            task->res = MCIQTZ_mciOpen(task->devid, task->flags, (LPMCI_DGV_OPEN_PARMSW)task->parms);
            break;
        case MCI_CLOSE_DRIVER:
            task->res = MCIQTZ_mciClose(task->devid, task->flags, (LPMCI_GENERIC_PARMS)task->parms);
            break;
        case MCI_PLAY:
            task->res = MCIQTZ_mciPlay(task->devid, task->flags, (LPMCI_PLAY_PARMS)task->parms);
            break;
        case MCI_SEEK:
            task->res = MCIQTZ_mciSeek(task->devid, task->flags, (LPMCI_SEEK_PARMS)task->parms);
            break;
        case MCI_STOP:
            task->res = MCIQTZ_mciStop(task->devid, task->flags, (LPMCI_GENERIC_PARMS)task->parms);
            break;
        case MCI_PAUSE:
            task->res = MCIQTZ_mciPause(task->devid, task->flags, (LPMCI_GENERIC_PARMS)task->parms);
            break;
        case MCI_GETDEVCAPS:
            task->res = MCIQTZ_mciGetDevCaps(task->devid, task->flags, (LPMCI_GETDEVCAPS_PARMS)task->parms);
            break;
        case MCI_SET:
            task->res = MCIQTZ_mciSet(task->devid, task->flags, (LPMCI_DGV_SET_PARMS)task->parms);
            break;
        case MCI_STATUS:
            task->res = MCIQTZ_mciStatus(task->devid, task->flags, (LPMCI_DGV_STATUS_PARMSW)task->parms);
            break;
        case MCI_WHERE:
            task->res = MCIQTZ_mciWhere(task->devid, task->flags, (LPMCI_DGV_RECT_PARMS)task->parms);
            break;
        case MCI_SETAUDIO:
            task->res = MCIQTZ_mciSetAudio(task->devid, task->flags, (LPMCI_DGV_SETAUDIO_PARMSW)task->parms);
            break;
        case MCI_UPDATE:
            task->res = MCIQTZ_mciUpdate(task->devid, task->flags, (LPMCI_DGV_UPDATE_PARMS)task->parms);
            break;
        case MCI_WINDOW:
            task->res = MCIQTZ_mciWindow(task->devid, task->flags, (LPMCI_DGV_WINDOW_PARMSW)task->parms);
            break;
        case MCI_CLOSE:
            /* Special internal message */
            SetEvent(task->done);
            goto end;
        default:
            FIXME("Shouldn't receive another message (%04x)\n", task->msg);
            task->res = MCIERR_UNRECOGNIZED_COMMAND;
            break;
        }
        SetEvent(task->done);
    }

end:
    return 0;
}

/*======================================================================*
 *                          MCI QTZ entry points                        *
 *======================================================================*/

/**************************************************************************
 *                              DriverProc (MCIQTZ.@)
 */
LRESULT CALLBACK MCIQTZ_DriverProc(DWORD_PTR dwDevID, HDRVR hDriv, UINT wMsg,
                                   LPARAM dwParam1, LPARAM dwParam2)
{
    TRACE("(%08lX, %p, %08X, %08lX, %08lX)\n",
          dwDevID, hDriv, wMsg, dwParam1, dwParam2);

    switch (wMsg) {
        case DRV_LOAD:                  return 1;
        case DRV_FREE:                  return 1;
        case DRV_OPEN:                  return MCIQTZ_drvOpen((LPCWSTR)dwParam1, (LPMCI_OPEN_DRIVER_PARMSW)dwParam2);
        case DRV_CLOSE:                 return MCIQTZ_drvClose(dwDevID);
        case DRV_ENABLE:                return 1;
        case DRV_DISABLE:               return 1;
        case DRV_QUERYCONFIGURE:        return 1;
        case DRV_CONFIGURE:             return MCIQTZ_drvConfigure(dwDevID);
        case DRV_INSTALL:               return DRVCNF_RESTART;
        case DRV_REMOVE:                return DRVCNF_RESTART;
    }

    /* session instance */
    if (dwDevID == 0xFFFFFFFF)
        return 1;

    switch (wMsg) {
        case MCI_OPEN_DRIVER:
        case MCI_PLAY:
        case MCI_SEEK:
        case MCI_GETDEVCAPS:
        case MCI_SET:
        case MCI_STATUS:
        case MCI_WHERE:
            if (!dwParam2) return MCIERR_NULL_PARAMETER_BLOCK;
            return MCIQTZ_relayTaskMessage(dwDevID, wMsg, dwParam1, dwParam2);
        case MCI_CLOSE_DRIVER:
        case MCI_STOP:
        case MCI_PAUSE:
            return MCIQTZ_relayTaskMessage(dwDevID, wMsg, dwParam1, dwParam2);
        /* Digital Video specific */
        case MCI_SETAUDIO:
        case MCI_UPDATE:
        case MCI_WINDOW:
            if (!dwParam2) return MCIERR_NULL_PARAMETER_BLOCK;
            return MCIQTZ_relayTaskMessage(dwDevID, wMsg, dwParam1, dwParam2);
        case MCI_PUT:
        case MCI_RECORD:
        case MCI_RESUME:
        case MCI_INFO:
        case MCI_LOAD:
        case MCI_SAVE:
        case MCI_FREEZE:
        case MCI_REALIZE:
        case MCI_UNFREEZE:
        case MCI_STEP:
        case MCI_COPY:
        case MCI_CUT:
        case MCI_DELETE:
        case MCI_PASTE:
        case MCI_CUE:
        /* Digital Video specific */
        case MCI_CAPTURE:
        case MCI_MONITOR:
        case MCI_RESERVE:
        case MCI_SIGNAL:
        case MCI_SETVIDEO:
        case MCI_QUALITY:
        case MCI_LIST:
        case MCI_UNDO:
        case MCI_CONFIGURE:
        case MCI_RESTORE:
            FIXME("Unimplemented command [%08X]\n", wMsg);
            break;
        case MCI_SPIN:
        case MCI_ESCAPE:
            WARN("Unsupported command [%08X]\n", wMsg);
            break;
        case MCI_OPEN:
        case MCI_CLOSE:
            FIXME("Shouldn't receive a MCI_OPEN or CLOSE message\n");
            break;
        default:
            TRACE("Sending msg [%08X] to default driver proc\n", wMsg);
            return DefDriverProc(dwDevID, hDriv, wMsg, dwParam1, dwParam2);
    }

    return MCIERR_UNRECOGNIZED_COMMAND;
}
