#include "stdafx.h"
#include "resource.h"
#include "NMWebCookieCtrlLib.h"

KBrowserWrapper* g_pBrowser = NULL;

BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_INITDIALOG:
        {
            // 브라우저 열기
            g_pBrowser->OpenBrowser( hDlg );
            g_pkUIScene->SetAllStop(true);
#if !defined( NATION_USA ) && !defined( NATION_EU )
            // 마우스 클립
            RECT rc;
            ::GetWindowRect( hDlg, &rc );
            ::ClipCursor( &rc );
#endif
            return TRUE;
        }

        case WM_COMMAND:
        {
            // 브라우저 닫기 및 마우스 클립 해제
            switch ( LOWORD( wParam ) )
            {
                case IDOK:
                {
                    if( false == g_pBrowser->IsClosed() )
                    {
                        g_pBrowser->CloseBrowser();
                    }
                    ::EndDialog( hDlg, IDOK );
                    ::ClipCursor( NULL );
                    if( g_pkUIScene->m_pkSkillTree->IsEnable() )
                    {
                        g_pkUIScene->Stop( true );
                    }
                    g_pkUIScene->SetAllStop(false);

                    return TRUE;
                }
                case IDCANCEL:
                {
                    if( false == g_pBrowser->IsClosed() )
                    {
                        g_pBrowser->CloseBrowser();
                    }
                    ::EndDialog( hDlg, IDCANCEL );
                    ::ClipCursor( NULL );
                    if( g_pkUIScene->m_pkSkillTree->IsEnable() )
                    {
                        g_pkUIScene->Stop( true );
                    }
                    g_pkUIScene->SetAllStop(false);

                    return TRUE;
                }
            }
            return FALSE;
        }
    }

    return FALSE;
}

KBrowserWrapper::KBrowserWrapper( void )
: m_dwLastReqBuyGoodsID(-1)
{
    g_pBrowser  = this;

    ::ZeroMemory( &m_rect, sizeof( RECT ) );
    m_bClosed   = true;
    m_hDlg      = NULL;
	m_pEndFunc	= NULL;

    m_pPCHBrowser = NULL;
	m_bInit = false;

}

KBrowserWrapper::~KBrowserWrapper( void )
{
	if( m_bInit )
		UnInit();
    DeleteBrowserObj();

    g_pBrowser = NULL;
}

void KBrowserWrapper::DestroyWin( )
{
    CloseBrowser();
    ::ClipCursor( NULL );
    ::DestroyWindow( m_hDlg );
}

BOOL KBrowserWrapper::Create( RECT& rect, HWND hWndParent )
{
    m_rect      = rect;
    m_bClosed   = false;

    if ( NULL == ( m_hDlg = ::CreateDialog( ::GetModuleHandle( NULL ),
                                            MAKEINTRESOURCE( IDD_DIALOG_BROWSER ),
                                            hWndParent, DlgProc ) ) )
    {
        return FALSE;
    }

    if ( !g_MyD3D->IsWindowMode() )
    {
        ::SendMessage( g_MyD3D->m_hWnd, WM_COMMAND, IDM_TOGGLEFULLSCREEN, 0 );
        g_MyD3D->m_bWebBrowser = TRUE;
    }

    return TRUE;
}

bool KBrowserWrapper::OpenBrowser( HWND hDlg )
{
    if ( false == g_pBrowser->AllocateBrowserObj() )
    {
        return false;
    }

    int w = m_rect.right - m_rect.left;
    int h = m_rect.bottom - m_rect.top;

    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = w - 5;
    rect.bottom = h - 30;

    
    if ( FALSE == m_pPCHBrowser->Create( WS_VISIBLE|WS_CHILD, rect, hDlg, 10000 ) )
    {
        ::MessageBox( hDlg,
                      g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TEXT ).c_str(),
                      g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ).c_str(),
                      MB_OK );
    }


    ::MoveWindow( hDlg, m_rect.left, m_rect.top, w, h, TRUE );
    ::ShowWindow( hDlg, SW_SHOW );

    // 닫기 버튼 위치 교정 및 이름 설정
    int btnx = ( w - 100 ) / 2;
    int btny = h - 25;
    ::MoveWindow( ::GetDlgItem( hDlg, IDOK ), btnx, btny, 80, 20, TRUE );
    ::SetWindowText( ::GetDlgItem( hDlg, IDOK ),
                     g_pkStrLoader->GetString( STR_ID_CASH_BUY_CLOSE_BUTTON ).c_str() );

    return true;
}

void KBrowserWrapper::CloseBrowser( void )
{
    m_bClosed = true;
	if( m_pEndFunc != NULL )
		(*m_pEndFunc)();
	m_pEndFunc = NULL;

    m_pPCHBrowser->Destroy();

    DeleteBrowserObj();
}

BOOL KBrowserWrapper::Init( HWND hWndMain, const char* szAuthCookie, const char* szDataCookie, const char* szCpCookie )
{
	m_bInit = true;
	UNREFERENCED_PARAMETER( hWndMain );
	UNREFERENCED_PARAMETER( szAuthCookie );
	UNREFERENCED_PARAMETER( szDataCookie );
	UNREFERENCED_PARAMETER( szCpCookie );
	return InitPCHWebBrowser();

}

BOOL KBrowserWrapper::UnInit( void )
{
	m_bInit = false;
	return UnInitPCHWebBrowser();
}

void KBrowserWrapper::SetCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc )
{
	SetPCHWebBrowserCookie( strID, strNickName, strEtc );
}

bool KBrowserWrapper::AllocateBrowserObj( void )
{
	DeleteBrowserObj();
	m_pPCHBrowser = new KPCHWebBrowser();
	return ( m_pPCHBrowser != NULL );

}

void KBrowserWrapper::DeleteBrowserObj( void )
{
	SAFE_DELETE( m_pPCHBrowser );
}

BOOL KBrowserWrapper::Navigate( const char* strURL, const char* strPostString_, const wchar_t* wstrTargetFrameName_ )
{
    if ( NULL == m_pPCHBrowser ) return FALSE;
    return m_pPCHBrowser->Navigate( strURL, strPostString_, wstrTargetFrameName_ );
}

void KBrowserWrapper::SetReqGoodsID( DWORD _dwGoodsID )
{
    m_dwLastReqBuyGoodsID = _dwGoodsID;
}

KNetmarbleBrowser::KNetmarbleBrowser( void )
{
	m_pSSOBrowser = NULL;
}


bool KNetmarbleBrowser::OpenBrowser( HWND hDlg )
{
	if ( false == g_pBrowser->AllocateBrowserObj() )
	{
		return false;
	}
 
	int w = m_rect.right - m_rect.left;
	int h = m_rect.bottom - m_rect.top;

	RECT rect;
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = w - 5;
	rect.bottom = h - 30;

	{
		if ( FALSE == m_pSSOBrowser->Create( WS_VISIBLE|WS_CHILD, rect, hDlg, 10000 ) )
		{
			::MessageBox( hDlg,
				g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TEXT ).c_str(),
				g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ).c_str(),
				MB_OK );
		}
	}

	::MoveWindow( hDlg, m_rect.left, m_rect.top, w, h, TRUE );
	::ShowWindow( hDlg, SW_SHOW );

	// 닫기 버튼 위치 교정 및 이름 설정
	int btnx = ( w - 100 ) / 2;
	int btny = h - 25;
	::MoveWindow( ::GetDlgItem( hDlg, IDOK ), btnx, btny, 80, 20, TRUE );
	::SetWindowText( ::GetDlgItem( hDlg, IDOK ),
		g_pkStrLoader->GetString( STR_ID_CASH_BUY_CLOSE_BUTTON ).c_str() );

	return true;
}

BOOL KNetmarbleBrowser::Init( HWND hWndMain, const char* szAuthCookie, const char* szDataCookie, const char* szCpCookie )
{
	m_bInit = true;
	return InitSSOWebBrowser( hWndMain, szAuthCookie, szDataCookie, szCpCookie );
}

BOOL KNetmarbleBrowser::UnInit( void )
{
	m_bInit = false;
	return UnInitSSOWebBrowser();
}

void KNetmarbleBrowser::SetCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc )
{
	UNREFERENCED_PARAMETER( strID );
	UNREFERENCED_PARAMETER( strNickName );
	UNREFERENCED_PARAMETER( strEtc );
}

bool KNetmarbleBrowser::AllocateBrowserObj( void )
{
	DeleteBrowserObj();
	m_pSSOBrowser = new CSSOWebBrowser();
	return ( m_pSSOBrowser != NULL );
	
}

void KNetmarbleBrowser::DeleteBrowserObj( void )
{
	SAFE_DELETE( m_pSSOBrowser );
}

BOOL KNetmarbleBrowser::Navigate( const char* strURL, const char* strPostString_, const wchar_t* wstrTargetFrameName_ )
{
	if ( NULL == m_pSSOBrowser ) return FALSE;
	return m_pSSOBrowser->Navigate( strURL );
}

void KNetmarbleBrowser::CloseBrowser( void )
{
	m_bClosed = true;
	if( m_pEndFunc != NULL )
		(*m_pEndFunc)();
	m_pEndFunc = NULL;
	
    if( m_pSSOBrowser ) 
	DeleteBrowserObj();

    if ( g_MyD3D->m_bWebBrowser )
    {
        ::SendMessage( g_MyD3D->m_hWnd, WM_COMMAND, IDM_TOGGLEFULLSCREEN, 0 );
        g_MyD3D->m_bWebBrowser = FALSE;
    }
}

KNetmarbleBrowser::~KNetmarbleBrowser( void )
{

}

BOOL KToonilandBrowser::Init( HWND hWndMain, const char* szAuthCookie, const char* szDataCookie, const char* szCpCookie )
{
	m_bInit = true;
//#ifdef NETMARBLE_BUY_TEST
//  if( 2 == g_kGlobalValue.m_dwChannelType )
//  	NMWebCookieCtrl_Init("chase-dev.tooniland.com", "http://chase-dev.tooniland.com/shop/gameshop/buy_item.asp");
//  else if( 4 == g_kGlobalValue.m_dwChannelType )
//      NMWebCookieCtrl_Init("Playnetwork.co.kr", "http://alpha-chase.playnetwork.co.kr/shop/gameshop/buy_item.asp");
//#else
    if( 2 == g_kGlobalValue.m_dwChannelType )
        NMWebCookieCtrl_Init("chase.tooniland.com", "http://chase.tooniland.com/shop/gameshop/buy_item.asp");
    else if( 4 == g_kGlobalValue.m_dwChannelType )
        NMWebCookieCtrl_Init("Playnetwork.co.kr", "http://chase.playnetwork.co.kr/shop/gameshop/buy_item.asp");
//#endif
	
	return KBrowserWrapper::Init( hWndMain, szAuthCookie, szDataCookie, szCpCookie );
}

BOOL KToonilandBrowser::UnInit( void )
{
	m_bInit = false;
	NMWebCookieCtrl_Destroy();
	return KBrowserWrapper::UnInit();
}

void KToonilandBrowser::SetCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc )
{
	NMWebCookieCtrl_SetDomainCookie( strID.c_str(), strNickName.c_str() );
}
