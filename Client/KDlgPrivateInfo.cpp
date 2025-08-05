#include "stdafx.h"
/// @file   KInfoBox.cpp
/// @author pch413@kogstudios.com
/// @since  2004-03-03 오전 11:11:29

#include "KDlgPrivateInfo.h"

//



/// Application-defined dialog box callback function.
/// @param hDlg handle to dialog box
/// @param uMsg message
/// @param wParam first message parameter
/// @param lParam second message parameter
/// @return TRUE if it processed the message, and FALSE if it did not
static BOOL CALLBACK DlgPrivateInfoProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_INITDIALOG:
        {
            SetWindowText( hDlg, g_pkStrLoader->GetString( STR_ID_USERAGREEMENT_CAPTION ).c_str() );

            std::wstring strContext = g_pkStrLoader->GetString( STR_ID_USERAGREEMENT ).c_str();

            //// 2008/02/01 대만 인증서 길이가 strLoader의 최대길이(2048)의 범위를 넘어 5개(1000개)로 쪼갬.
            #if defined( NATION_TAIWAN )
            {
                strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT2 );
                strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT3 );
                strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT4 );
                strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT5 );
            }
			#elif defined( NATION_BRAZIL)
			{
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT2 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT3 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT4 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT5 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT6 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT7 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT8 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT9 );
				strContext += g_pkStrLoader->GetString( STR_ID_USERAGREEMENT10 );
			}
            #endif

            HWND hEdit = GetDlgItem(hDlg,IDC_MEMO);
            SendMessage(hEdit,WM_SETTEXT,0,reinterpret_cast<LPARAM>( strContext.c_str() ));

            HWND hButton = GetDlgItem(hDlg,IDOK);
            SendMessage(hButton,WM_SETTEXT,0,reinterpret_cast<LPARAM>( g_pkStrLoader->GetString( STR_ID_USERAGREEMENT_YES ).c_str() ));
            hButton = GetDlgItem(hDlg,IDCANCEL);
            SendMessage(hButton,WM_SETTEXT,0,reinterpret_cast<LPARAM>( g_pkStrLoader->GetString( STR_ID_USERAGREEMENT_NO ).c_str() ));

            SetWindowPos( hDlg,HWND_TOPMOST,0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE );

            return TRUE;
        }
        case WM_DESTROY:
        {           
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch ( LOWORD( wParam ) )
            {
                case IDOK:
                {
                    ::EndDialog( hDlg, IDOK );
                    return TRUE;
                }
	            case IDCANCEL:
                {   					
                    ::EndDialog( hDlg, IDCANCEL );
                    return TRUE;
                }
            }
            return FALSE;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            ::BeginPaint( hDlg, &ps );
            ::EndPaint( hDlg, &ps );

            return TRUE;
        }
    }
    return FALSE;
}

static BOOL CALLBACK DlgPrivateInfoProc_Shop( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
    case WM_INITDIALOG:
        {
            SetWindowText( hDlg, g_pkStrLoader->GetString( STR_ID_USERAGREEMENT_CAPTION ).c_str() );

            std::wstring strContext = g_pkStrLoader->GetString( STR_ID_USERAGREEMENT_SHOP ).c_str();

            HWND hEdit = GetDlgItem(hDlg,IDC_MEMO);
            SendMessage(hEdit,WM_SETTEXT,0,reinterpret_cast<LPARAM>( strContext.c_str() ));

            HWND hButton = GetDlgItem(hDlg,IDOK);
            SendMessage(hButton,WM_SETTEXT,0,reinterpret_cast<LPARAM>( g_pkStrLoader->GetString( STR_ID_USERAGREEMENT_YES ).c_str() ));
            hButton = GetDlgItem(hDlg,IDCANCEL);
            SendMessage(hButton,WM_SETTEXT,0,reinterpret_cast<LPARAM>( g_pkStrLoader->GetString( STR_ID_USERAGREEMENT_NO ).c_str() ));

            SetWindowPos( hDlg,HWND_TOPMOST,0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE );

            return TRUE;
        }
    case WM_DESTROY:
        {           
            return TRUE;
        }
    case WM_COMMAND:
        {
            switch ( LOWORD( wParam ) )
            {
            case IDOK:
                {
                    ::EndDialog( hDlg, IDOK );
                    return TRUE;
                }
            case IDCANCEL:
                {   					
                    ::EndDialog( hDlg, IDCANCEL );
                    return TRUE;
                }
            }
            return FALSE;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            ::BeginPaint( hDlg, &ps );
            ::EndPaint( hDlg, &ps );

            return TRUE;
        }
    }
    return FALSE;
}

INT_PTR KDlgPrivateInfo::DoModal( HWND hParentWnd /* = NULL */, int iType )
{
    // 보기 결정 파일이 존재하면 열지 않는다.
    // if ( 0 == ::_access( "SHOW.DAT", 0 ) ) return IDOK;

    // 부모 윈도우가 설정되어 있지 않다면 데스크탑 윈도우를 부모로 한다.
    if ( NULL == hParentWnd ) hParentWnd = ::GetDesktopWindow();

    switch( iType )
    {
        case USER_AGREEMENT:
            {
#if defined( NATION_BRAZIL)
				// 사용자 동의서 다이얼로그 박스를 띄운다.
				return ::DialogBox( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( IDD_USERAGREE_BRAZIL ),
					hParentWnd, DlgPrivateInfoProc );
#else		
                // 사용자 동의서 다이얼로그 박스를 띄운다.
                return ::DialogBox( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( IDD_USERAGREE ),
                    hParentWnd, DlgPrivateInfoProc );
#endif
            }
            break;
        case USER_AGREEMENT_SHOP:
            {
                // 샵 동의서 다이얼로그 박스를 띄운다.
                return ::DialogBox( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( IDD_USERAGREE ),
                    hParentWnd, DlgPrivateInfoProc_Shop );
            }
            break;
        default:
            {
                return ::DialogBox( ::GetModuleHandle( NULL ), MAKEINTRESOURCE( IDD_USERAGREE ),
                    hParentWnd, DlgPrivateInfoProc );
            }
            break;
    }
}
