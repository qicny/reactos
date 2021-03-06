/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS Clipboard Viewer
 * FILE:            base/applications/clipbrd/winutils.c
 * PURPOSE:         Miscellaneous helper functions.
 * PROGRAMMERS:     Ricardo Hanke
 */

#include "precomp.h"

void ShowLastWin32Error(HWND hwndParent)
{
    DWORD dwError;
    LPWSTR lpMsgBuf = NULL;

    dwError = GetLastError();
    if (dwError == NO_ERROR)
        return;

    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL, dwError, 0, (LPWSTR)&lpMsgBuf, 0,  NULL);
    MessageBoxW(hwndParent, lpMsgBuf, NULL, MB_OK | MB_ICONERROR);
    LocalFree(lpMsgBuf);
}

void BringWindowToFront(HWND hWnd)
{
    if (IsIconic(hWnd))
    {
        ShowWindow(hWnd, SW_RESTORE);
        SetForegroundWindow(hWnd);
    }
    else
    {
        SetForegroundWindow(hWnd);
    }
}

int DrawTextFromResource(HINSTANCE hInstance, UINT uID, HDC hDC, LPRECT lpRect, UINT uFormat)
{
    LPWSTR lpBuffer;
    int nCount;

    nCount = LoadStringW(hInstance, uID, (LPWSTR)&lpBuffer, 0);
    if (nCount)
    {
        return DrawTextW(hDC, lpBuffer, nCount, lpRect, uFormat);
    }
    else
    {
        return 0;
    }
}

int MessageBoxRes(HWND hWnd, HINSTANCE hInstance, UINT uText, UINT uCaption, UINT uType)
{
    MSGBOXPARAMSW mb;

    ZeroMemory(&mb, sizeof(mb));
    mb.cbSize = sizeof(mb);
    mb.hwndOwner = hWnd;
    mb.hInstance = hInstance;
    mb.lpszText = MAKEINTRESOURCEW(uText);
    mb.lpszCaption = MAKEINTRESOURCEW(uCaption);
    mb.dwStyle = uType;
    mb.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);

    return MessageBoxIndirectW(&mb);
}

void DrawTextFromClipboard(HDC hDC, LPRECT lpRect, UINT uFormat)
{
    HGLOBAL hGlobal;
    LPWSTR lpchText;

    if (!OpenClipboard(NULL))
        return;

    hGlobal = GetClipboardData(CF_UNICODETEXT);
    if (!hGlobal)
        return;

    lpchText = GlobalLock(hGlobal);
    if (!lpchText)
        return;

    DrawTextW(hDC, lpchText, -1, lpRect, uFormat);
    GlobalUnlock(hGlobal);
    CloseClipboard();
}

void BitBltFromClipboard(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, DWORD dwRop)
{
    HDC hdcMem;
    HBITMAP hbm;

    if (!OpenClipboard(NULL))
        return;

    hdcMem = CreateCompatibleDC(hdcDest);
    if (hdcMem)
    {
        hbm = (HBITMAP)GetClipboardData(CF_BITMAP);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcMem, nXSrc, nYSrc, dwRop);
        DeleteDC(hdcMem);
    }
    CloseClipboard();
}

void SetDIBitsToDeviceFromClipboard(UINT uFormat, HDC hdc, int XDest, int YDest, int XSrc, int YSrc, UINT uStartScan, UINT fuColorUse)
{
    LPBITMAPINFOHEADER lpInfoHeader;
    LPBYTE lpBits;
    HGLOBAL hGlobal;
    INT iPalSize;

    if (!OpenClipboard(NULL))
        return;

    hGlobal = GetClipboardData(uFormat);
    if (!hGlobal)
        return;

    lpInfoHeader = GlobalLock(hGlobal);
    if (!lpInfoHeader)
        return;

    if (lpInfoHeader->biBitCount < 16)
    {
        iPalSize = (1 << lpInfoHeader->biBitCount) * 4;
    }
    else
    {
        iPalSize = 0;
    }

    lpBits = (LPBYTE)lpInfoHeader + lpInfoHeader->biSize + iPalSize;

    SetDIBitsToDevice(hdc, XDest, YDest, lpInfoHeader->biWidth, lpInfoHeader->biHeight, XSrc, YSrc, uStartScan, lpInfoHeader->biHeight, lpBits, (LPBITMAPINFO)lpInfoHeader, fuColorUse);

    GlobalUnlock(hGlobal);
    CloseClipboard();
}

void PlayMetaFileFromClipboard(HDC hdc, const RECT *lpRect)
{
    LPMETAFILEPICT mp;
    HGLOBAL hGlobal;

    if (!OpenClipboard(NULL))
        return;

    hGlobal = GetClipboardData(CF_METAFILEPICT);
    if (!hGlobal)
        return;

    mp = (LPMETAFILEPICT)GlobalLock(hGlobal);
    if (!mp)
        return;

    SetMapMode(hdc, mp->mm);
    SetViewportExtEx(hdc, lpRect->right, lpRect->bottom, NULL);
    SetViewportOrgEx(hdc, lpRect->left, lpRect->top, NULL);
    PlayMetaFile(hdc, mp->hMF);
    GlobalUnlock(hGlobal);
    CloseClipboard();
}

void PlayEnhMetaFileFromClipboard(HDC hdc, const RECT *lpRect)
{
    HENHMETAFILE hEmf;

    if (!OpenClipboard(NULL))
        return;

    hEmf = GetClipboardData(CF_ENHMETAFILE);
    PlayEnhMetaFile(hdc, hEmf, lpRect);
    CloseClipboard();
}
