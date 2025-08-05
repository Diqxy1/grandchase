#include "stdafx.h"
#include "KMsgBox.h"

HWND KMsgBox::ms_hDlg = NULL;
UINT KMsgBox::ms_uType = KMsgBox::MBOX_NOTHING;
char KMsgBox::ms_szTxtLn1[LINE_LENGTH] = { 0, };
char KMsgBox::ms_szTxtLn2[LINE_LENGTH] = { 0, };

INT_PTR KMsgBox::Open(const char* szTxtLn1,
                      const char* szTxtLn2,
                      HWND hParentWnd /* = NULL */,
                      UINT uType /* = MBOX_DEFAULT */)
{
    ::ZeroMemory(ms_szTxtLn1, sizeof(ms_szTxtLn1));
    ::ZeroMemory(ms_szTxtLn2, sizeof(ms_szTxtLn2));
    size_t len1 = ::strlen(szTxtLn1);
    size_t len2 = ::strlen(szTxtLn2);
    ::memcpy(ms_szTxtLn1, szTxtLn1, (len1 < LINE_LENGTH ? len1 : LINE_LENGTH));
    ::memcpy(ms_szTxtLn2, szTxtLn2, (len2 < LINE_LENGTH ? len2 : LINE_LENGTH));
    LPTSTR szRes;
    switch (ms_uType = uType)
    {
        case MBOX_DEFAULT:  szRes = MAKEINTRESOURCE(IDD_MSGBOX_DEFAULT);  break;
        case MBOX_OK:       szRes = MAKEINTRESOURCE(IDD_MSGBOX_OK);       break;
        case MBOX_OKCANCEL: szRes = MAKEINTRESOURCE(IDD_MSGBOX_OKCANCEL); break;
        default: ASSERT(false && _T("Invalid message box type!"));        break;
    }
    if (MBOX_DEFAULT == uType)
    {
        ::CreateDialog(::AfxGetInstanceHandle(), szRes, hParentWnd, KMsgBox::MsgBoxProc);
        return TRUE;
    }
    else
    {
        return ::DialogBox(::AfxGetInstanceHandle(), szRes, hParentWnd, KMsgBox::MsgBoxProc);
    }
}

BOOL KMsgBox::Close(int iResult /* = 0 */)
{
    if (NULL != ms_hDlg)
    {
        if (MBOX_DEFAULT == ms_uType)
        {
            return ::DestroyWindow(ms_hDlg);
        }
        else
        {
            return ::EndDialog(ms_hDlg, iResult);
        }
    }
    return FALSE;
}

BOOL CALLBACK KMsgBox::MsgBoxProc(HWND hDlg,
                                  UINT uMsg,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    static HBITMAP bkgndBmp = NULL;
    static HBITMAP okBtnBmp = NULL;
    static HBITMAP cancelBtnBmp = NULL;

    switch (uMsg)
    {
        case WM_INITDIALOG:
            ms_hDlg = hDlg;
            bkgndBmp = ::LoadBitmap(::AfxGetInstanceHandle(),
                                    MAKEINTRESOURCE(IDB_MSGBOX));
            ASSERT(NULL != bkgndBmp);
            okBtnBmp = ::LoadBitmap(::AfxGetInstanceHandle(),
                                    MAKEINTRESOURCE(IDB_BTN_OK));
            ASSERT(NULL != okBtnBmp);
            cancelBtnBmp = ::LoadBitmap(::AfxGetInstanceHandle(),
                                        MAKEINTRESOURCE(IDB_BTN_CANCEL));
            ASSERT(NULL != cancelBtnBmp);
            switch (ms_uType)
            {
                case MBOX_OKCANCEL:
                    ::SendDlgItemMessage(hDlg, IDCANCEL, BM_SETIMAGE, IMAGE_BITMAP,
                                         reinterpret_cast<LPARAM>(cancelBtnBmp));
                case MBOX_OK:
                    ::SendDlgItemMessage(hDlg, IDOK, BM_SETIMAGE, IMAGE_BITMAP,
                                         reinterpret_cast<LPARAM>(okBtnBmp));
            }
            return TRUE;
        case WM_DESTROY:
            ASSERT(NULL != bkgndBmp);
            ::DeleteObject(bkgndBmp);
            bkgndBmp = NULL;
            ASSERT(NULL != okBtnBmp);
            ::DeleteObject(okBtnBmp);
            okBtnBmp = NULL;
            ASSERT(NULL != cancelBtnBmp);
            ::DeleteObject(cancelBtnBmp);
            cancelBtnBmp = NULL;
            ms_hDlg = NULL;
            return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    ::EndDialog(hDlg, IDOK);
                    return TRUE;
                case IDCANCEL:
                    ::EndDialog(hDlg, IDCANCEL);
                    return TRUE;
            }
            return FALSE;
        case WM_MOUSEMOVE:
            if (wParam & MK_LBUTTON)
            {
                ::SendMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
            }
            return TRUE;
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
                for(int x = 0; x < 5; x++)
                {
                    for(int y = 0; y < 5; y++)
                    {
                        ::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
                                  y + FIRST_LINE - 2 + iRevise,
                                  ms_szTxtLn1, ::strlen(ms_szTxtLn1));
                        ::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
                                  y + SECOND_LINE - 2 + iRevise,
                                  ms_szTxtLn2, ::strlen(ms_szTxtLn2));
                    }
                }
                ::SetTextColor(ps.hdc, 0x00ffffff);
                ::TextOut(ps.hdc, MBOX_WIDTH / 2, FIRST_LINE + iRevise,
                          ms_szTxtLn1, ::strlen(ms_szTxtLn1));
                ::TextOut(ps.hdc, MBOX_WIDTH / 2, SECOND_LINE + iRevise,
                          ms_szTxtLn2, ::strlen(ms_szTxtLn2));

                ::DeleteDC(memDC);
                ::EndPaint(hDlg, &ps);
            }
            return TRUE;
    }

    return FALSE;
}