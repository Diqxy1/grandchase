#include "stdafx.h"
/// @file   KInfoBox.cpp
/// @author pch413@kogstudios.com
/// @since  2004-03-03 오전 11:11:29

#include "KInfoBox.h"
#include "KMsgBox.h"
//

#include <io.h>

/// Application-defined dialog box callback function.
/// @param hDlg handle to dialog box
/// @param uMsg message
/// @param wParam first message parameter
/// @param lParam second message parameter
/// @return TRUE if it processed the message, and FALSE if it did not
static BOOL CALLBACK InfoBoxProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    static HBITMAP bkgndBmp = NULL;

    switch ( uMsg )
    {
        case WM_INITDIALOG:
        {
            bkgndBmp = static_cast<HBITMAP>( ::LoadBitmap( ::GetModuleHandle( NULL ),
                                                           MAKEINTRESOURCE( IDB_DIALOG ) ) );
            ::SetWindowPos( hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
            return TRUE;
        }
        case WM_DESTROY:
        {
            if ( NULL != bkgndBmp )
            {
                ::DeleteObject(bkgndBmp);
                bkgndBmp = NULL;
            }
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch ( LOWORD( wParam ) )
            {
                case IDC_BUTTON_INIT:
                {
                    // 옵션 파일을 삭제한다.
                    ::DeleteFileA( "NewOptionVer1.dat" );
                    // 키설정 파일을 삭제한다.
                    ::DeleteFileA( "KeySettingNew.dat" );
                    // not break!
                }
                case IDOK:
                {
                    // '다시 표시안함' 체크 박스가 체크되어 있다면
                    // SHOW.DAT 파일을 생성한다.
                    if ( BST_CHECKED == ::SendMessage( ::GetDlgItem( hDlg, IDC_CHECK_NEXT ),
                                                       BM_GETCHECK, 0, 0 ) )
                    {
                        CreateFileA( "SHOW.DAT", GENERIC_WRITE, 0, NULL,
                                    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
                    }
                    ::EndDialog( hDlg, IDOK );
                    return TRUE;
                }
//                case IDCANCEL:
//                {
//                    // 윈도우 닫기 버튼을 눌렀다면 종료한다.
//                    ::EndDialog( hDlg, IDCANCEL );
//                    return TRUE;
//                }
                case IDC_CHECK_NEXT:
                {
                    // '다시 표시안함' 체크 박스를 클릭하면 다음 메세지를 표시한다.
                    if ( BN_CLICKED == HIWORD( wParam ) &&
                         BST_CHECKED == ::SendMessage( ::GetDlgItem( hDlg, IDC_CHECK_NEXT ),
                                                       BM_GETCHECK, 0, 0 ) )
                    {
                        KMsgBox::Open( g_pkStrLoader->GetString( STR_ID_APPMSG_INFO_NEXT_ESC_KEY1 ).c_str(),
                                       g_pkStrLoader->GetString( STR_ID_APPMSG_INFO_NEXT_ESC_KEY2 ).c_str(),
                                       KMsgBox::MBOX_OK, hDlg );
                    }
                    break;
                }
            }
            return FALSE;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;

            ::BeginPaint( hDlg, &ps );

            // 다이얼로그의 배경 그림을 그린다.
            HDC memDC = ::CreateCompatibleDC( ps.hdc );
            HBITMAP oldBmp = reinterpret_cast<HBITMAP>( ::SelectObject( memDC, bkgndBmp ) );
            ::BitBlt( ps.hdc, 0, 0, 412, 274, memDC, 0, 0, SRCCOPY );
            ::SelectObject( memDC, oldBmp );
            ::DeleteDC( memDC );

            ::EndPaint( hDlg, &ps );

            return TRUE;
        }
    }

    return FALSE;
}

/// Open Infomation box.
/// @param handle to parent window
/// @return return value of dialog box.
INT_PTR KInfoBox::DoModal( HWND hParentWnd /* = NULL */ )
{
    // 보기 결정 파일이 존재하면 열지 않는다.
    if ( 0 == ::_access( "SHOW.DAT", 0 ) ) 
		return IDOK;

    // 부모 윈도우가 설정되어 있지 않다면 데스크탑 윈도우를 부모로 한다.
    if ( NULL == hParentWnd ) 
		hParentWnd = ::GetDesktopWindow();

    // 다이얼로그 박스를 띄운다.
    return ::DialogBox( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( IDD_DIALOG_INFOBOX ),
                        hParentWnd, InfoBoxProc );
}

/// Open Infomation box in force.
/// @param handle to parent window
/// @return return value of dialog box.
INT_PTR KInfoBox::ForceModal( HWND hParentWnd /* = NULL  */ )
{
    // 보기 결정 파일을 제거한다.
    ::DeleteFileA( "SHOW.DAT" );

    // 다이얼로그 박스를 뛰운다.
    return DoModal( hParentWnd );
}

/** @example KInfoBox

#include "KInfoBox.h"

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
    switch ( KInfoBox::DoModal() )
    {
        case IDOK:
        {
            // do something...
            break;
        }
        case IDCANCEL:
        {
            // do something...
            break;
        }
    }
    return 0;
}

*/