#include "stdafx.h"
#include "KMsgBox.h"
//


//

HWND KMsgBox::ms_hDlg  = NULL;                       ///< Handle to message box.
UINT KMsgBox::ms_uType = KMsgBox::MBOX_NOTHING;      ///< Type of message box.
WCHAR KMsgBox::ms_szTxtLn1[LINE_LENGTH + 1] = { 0, }; ///< First line message.
WCHAR KMsgBox::ms_szTxtLn2[LINE_LENGTH + 1] = { 0, }; ///< Second line message.

/// Open message box.
/// @param szTxtLn1 first line message
/// @param szTxtLn2 second line message
/// @param hParentWnd handle to parent window
/// @param uType type of message box
/// @return the nResult parameter in the call to the EndDialog function used to terminate the dialog box, zero, 1, or -1
INT_PTR KMsgBox::Open(const WCHAR* szTxtLn1,
                      const WCHAR* szTxtLn2,
                      UINT uType /* = MBOX_DEFAULT */,
                      HWND hParentWnd /* = NULL */)
{
    if (NULL == hParentWnd) hParentWnd = ::GetDesktopWindow();
    ::ZeroMemory(ms_szTxtLn1, sizeof(ms_szTxtLn1));
    ::ZeroMemory(ms_szTxtLn2, sizeof(ms_szTxtLn2));
    size_t len1 = ::wcslen(szTxtLn1);
    size_t len2 = ::wcslen(szTxtLn2);
    StringCchCopyW(ms_szTxtLn1, LINE_LENGTH, szTxtLn1);
    StringCchCopyW(ms_szTxtLn2, LINE_LENGTH, szTxtLn2);
    WCHAR* szRes;
    switch (ms_uType = uType)
    {
        case MBOX_DEFAULT:  szRes = MAKEINTRESOURCE(IDD_MSGBOX_DEFAULT);  break;
        case MBOX_OK:       szRes = MAKEINTRESOURCE(IDD_MSGBOX_OK);       break;
        case MBOX_OKCANCEL: szRes = MAKEINTRESOURCE(IDD_MSGBOX_OKCANCEL); break;
        default: ASSERT(false && _T("Invalid message box type!"));        break;
    }
    if (MBOX_DEFAULT == ms_uType)
    {
        ::CreateDialog(::GetModuleHandle(NULL), szRes, hParentWnd, MsgBoxProc);
        return TRUE;
    }
    else
    {
        return ::DialogBox(::GetModuleHandle(NULL), szRes, hParentWnd, MsgBoxProc);
    }
}

/// Close message box.
/// @param iResult return value of message box.
/// @return return value of message box.
BOOL KMsgBox::Close(int iResult /* = IDOK */)
{
    if (NULL != ms_hDlg)
    {
        if (MBOX_DEFAULT == ms_uType)
        {
            ::SendMessage(ms_hDlg, WM_CLOSE, 0, 0);
            return TRUE;
        }
        else
        {
            return ::EndDialog(ms_hDlg, iResult);
        }
    }
    return FALSE;
}

/// Application-defined dialog box callback function.
/// @param hDlg handle to dialog box
/// @param uMsg message
/// @param wParam first message parameter
/// @param lParam second message parameter
/// @return TRUE if it processed the message, and FALSE if it did not
BOOL CALLBACK KMsgBox::MsgBoxProc(HWND hDlg,
                                  UINT uMsg,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    static HBITMAP bkgndBmp     = NULL;
    static HBITMAP okBtnBmp     = NULL;
    static HBITMAP cancelBtnBmp = NULL;

    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            ms_hDlg = hDlg;

//#ifdef NATION_CHINA
//			bkgndBmp = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_MSGBOX_CS));
//			ASSERT(NULL != bkgndBmp);
//			okBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BTN_OK_CS));
//			ASSERT(NULL != okBtnBmp);
//			cancelBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BTN_CANCEL_CS));
//			ASSERT(NULL != cancelBtnBmp);
//#else
			bkgndBmp = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_MSGBOX));
			ASSERT(NULL != bkgndBmp);
			okBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BTN_OK));
			ASSERT(NULL != okBtnBmp);
			cancelBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BTN_CANCEL));
			ASSERT(NULL != cancelBtnBmp);
//#endif

			switch (ms_uType)
            {
                case MBOX_OKCANCEL:
                    ::SendDlgItemMessage(hDlg, IDCANCEL, BM_SETIMAGE, IMAGE_BITMAP,
                                         reinterpret_cast<LPARAM>(cancelBtnBmp));
                case MBOX_OK:
                    ::SendDlgItemMessage(hDlg, IDOK, BM_SETIMAGE, IMAGE_BITMAP,
                                         reinterpret_cast<LPARAM>(okBtnBmp));
            }
            ::SetWindowText(hDlg, g_pkStrLoader->GetString( STR_ID_GRANDCHASE_TITLE ).c_str());
            ::SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            ::UpdateWindow(hDlg);
            return TRUE;
        }
        case WM_CLOSE:
        {
            ::DestroyWindow(hDlg);
            return TRUE;
        }
        case WM_DESTROY:
        {
            if (NULL != bkgndBmp)     { ::DeleteObject(bkgndBmp);     bkgndBmp     = NULL; }
            if (NULL != okBtnBmp)     { ::DeleteObject(okBtnBmp);     okBtnBmp     = NULL; }
            if (NULL != cancelBtnBmp) { ::DeleteObject(cancelBtnBmp); cancelBtnBmp = NULL; }
            ms_uType = MBOX_NOTHING;
            ms_hDlg  = NULL;
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK:     ::EndDialog(hDlg, IDOK);     return TRUE;
                case IDCANCEL: ::EndDialog(hDlg, IDCANCEL); return TRUE;
            }
            return FALSE;
        }
        case WM_MOUSEMOVE:
        {
            if (wParam & MK_LBUTTON)
            {
                ::SendMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
            }
            return TRUE;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            ::BeginPaint(hDlg, &ps);
            HDC memDC = ::CreateCompatibleDC(ps.hdc);

            // Draw background.
            HBITMAP oldBmp = reinterpret_cast<HBITMAP>(::SelectObject(memDC, bkgndBmp));
            ::BitBlt(ps.hdc, 0, 0, MBOX_WIDTH, MBOX_HEIGHT, memDC, 0, 0, SRCCOPY);
            ::SelectObject(memDC, oldBmp);

            // Print message.
            ::SetBkMode(ps.hdc, TRANSPARENT);
            ::SetTextAlign(ps.hdc, TA_CENTER);
            ::SetTextColor(ps.hdc, 0x00996644);
            int iRevise = (MBOX_DEFAULT == ms_uType ? 12 : 0);
            for (int x = 0; x < 5; x++)
            {
                for (int y = 0; y < 5; y++)
                {
                    ::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
                              y + FIRST_LINE - 2 + iRevise,
                              ms_szTxtLn1, (int)::wcslen(ms_szTxtLn1));
                    ::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
                              y + SECOND_LINE - 2 + iRevise,
                              ms_szTxtLn2, (int)::wcslen(ms_szTxtLn2));
                }
            }
            ::SetTextColor(ps.hdc, 0x00ffffff);
            ::TextOut(ps.hdc, MBOX_WIDTH / 2, FIRST_LINE + iRevise,
                      ms_szTxtLn1, (int)::wcslen(ms_szTxtLn1));
            ::TextOut(ps.hdc, MBOX_WIDTH / 2, SECOND_LINE + iRevise,
                      ms_szTxtLn2, (int)::wcslen(ms_szTxtLn2));

            ::DeleteDC(memDC);
            ::EndPaint(hDlg, &ps);
            return TRUE;
        }
    }

    return FALSE;
}