#include "stdafx.h"
// file   : KPCHWebBrowser.cpp
// since  : 2005-06-02 오후 3:36:40
// author : pch413@kogstudios.com
// This requires IE 5.0 ( or better ).

// Park Chang-hyeon's web browser class implementation

#include "KPCHWebBrowser.h"

#include <wininet.h>
//
//

#pragma comment( lib, "wininet.lib" )

// in Browser.c file ( from http://www.codeproject.com/ )
extern "C"
{
	long DisplayHTMLPage( HWND hWnd, const char* webPageName, const char* strPostString_, const wchar_t* wstrTargetFrameName_ = L"" );
    void ResizeBrowser( HWND hWnd, DWORD width, DWORD height );
    long EmbedBrowserObject( HWND hWnd );
    void UnEmbedBrowserObject( HWND hWnd );
}

static std::string f_strIDCookie;
static std::string f_strNickNameCookie;
static std::string f_strEtcCookie;

#define BROWSER_ID 112233

LRESULT CALLBACK WrapperWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_CREATE:
        {
            int i = 0;
            break;
        }
        case WM_SIZE:
        {
            // Resize the browser child window, leaving room for our buttons.
            HWND hBrowserWnd = ::GetDlgItem( hWnd, BROWSER_ID );
            if ( NULL != hBrowserWnd )
            {
                ::MoveWindow( hBrowserWnd, 0, 0, LOWORD( lParam ), HIWORD( lParam ), TRUE );
            }

            return 0;
        }

        case WM_ERASEBKGND:
        {
            return 0;
        }

        case WM_CLOSE:
        {
            // Close this window. This will also cause the child window hosting the browser
            // control to receive its WM_DESTROY
            ::DestroyWindow( hWnd );

            return 0;
        }
    }

    return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK BrowserWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_SIZE:
        {
            // Resize the browser object to fit the window
            ResizeBrowser( hWnd, LOWORD( lParam ), HIWORD( lParam ) );

            return 0;
        }

        case WM_CREATE:
        {
            // Embed the browser object into our host window. We need do this only
            // once. Note that the browser object will start calling some of our
            // IOleInPlaceFrame and IOleClientSite functions as soon as we start
            // calling browser object functions in EmbedBrowserObject().
            if ( EmbedBrowserObject( hWnd ) ) return -1;

            return 0; // Success
        }

        case WM_DESTROY:
        {
            // Detach the browser object from this window, and free resources.
            UnEmbedBrowserObject( hWnd );

            return 0;
        }
    }

    return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
}

BOOL InitPCHWebBrowser( void )
{
    f_strIDCookie.erase();
    f_strNickNameCookie.erase();
    f_strEtcCookie.erase();

    ::OleInitialize( NULL );

    WNDCLASSEX wc;
    ::ZeroMemory( &wc, sizeof( WNDCLASSEX ) );

    wc.cbSize        = sizeof( WNDCLASSEX );
    wc.hInstance     = ::GetModuleHandle( NULL );
    wc.lpfnWndProc   = WrapperWndProc;
    wc.lpszClassName = _T( "WrapperWindow" );
    wc.style         = CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;
    ::RegisterClassEx( &wc );

    wc.lpfnWndProc   = BrowserWndProc;
    wc.lpszClassName = _T( "BrowserObject" );
    wc.style         = CS_HREDRAW|CS_VREDRAW;
    ::RegisterClassEx( &wc );

    return TRUE;
}

BOOL UnInitPCHWebBrowser( void )
{
    ::UnregisterClass( _T( "BrowserObject" ), ::GetModuleHandle( NULL ) );
    ::UnregisterClass( _T( "WrapperWindow" ), ::GetModuleHandle( NULL ) );

    ::OleUninitialize();

    f_strIDCookie.erase();
    f_strNickNameCookie.erase();
    f_strEtcCookie.erase();

    return TRUE;
}

void SetPCHWebBrowserCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc )
{
    f_strIDCookie       = strID;
    f_strNickNameCookie = strNickName;
    f_strEtcCookie      = strEtc;

    // Cookie Parameter:
    // - nxid : 넥슨ID
    // - gcnic : 게임상의 닉네임
    // - etc : 게임기동시 넘겨주는 값

    std::ostringstream strmIDCookie;
    std::ostringstream strmNickNameCookie;
    std::ostringstream strmEtcCookie;

    strmIDCookie        << "nxid="  << f_strIDCookie.c_str();
    strmNickNameCookie  << "gcnic=" << f_strNickNameCookie.c_str();
    strmEtcCookie       << "etc="   << f_strEtcCookie.c_str();

#ifdef __PATH__
	char* strURL = "http://nexon.co.jp";
#else
	//char* strURL = "http://test.grandchase.nexon.co.jp";
	char* strURL = "http://nexon.co.jp";
#endif

	::InternetSetCookieA( strURL, NULL, strmIDCookie.str().c_str() );
	::InternetSetCookieA( strURL, NULL, strmNickNameCookie.str().c_str() );
	::InternetSetCookieA( strURL, NULL, strmEtcCookie.str().c_str() );
}

KPCHWebBrowser::KPCHWebBrowser( void )
{
    m_hWrapperWnd = NULL;
    m_hBrowserWnd = NULL;
}

KPCHWebBrowser::~KPCHWebBrowser( void )
{
    // empty
}

BOOL KPCHWebBrowser::Create( DWORD dwStyle, const RECT& rc, HWND hWndParent, UINT id )
{
    if ( NULL == ( m_hWrapperWnd = ::CreateWindowEx( 0,
                                                     _T( "WrapperWindow" ),
                                                     _T( "" ),
                                                     dwStyle,
                                                     0, 0, 0, 0,
                                                     hWndParent,
                                                     reinterpret_cast<HMENU>( id ),
                                                     ::GetModuleHandle( NULL ),
                                                     NULL ) ) )
	{
        return FALSE;
    }

    if ( NULL == ( m_hBrowserWnd = ::CreateWindowEx( 0,
                                                     _T( "BrowserObject" ),
                                                     _T( "" ),
                                                     WS_CHILD|WS_VISIBLE,
                                                     0, 0, 0, 0,
                                                     m_hWrapperWnd,
                                                     reinterpret_cast<HMENU>( BROWSER_ID ),
                                                     ::GetModuleHandle( NULL ),
                                                     NULL ) ) )
	{
        return FALSE;
    }

    ::MoveWindow( m_hWrapperWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE );
    ::ShowWindow( m_hWrapperWnd, SW_SHOW );

    return TRUE;
}

void KPCHWebBrowser::Destroy( void )
{
    ::DestroyWindow( m_hWrapperWnd );

    m_hWrapperWnd = NULL;
    m_hBrowserWnd = NULL;
}

BOOL KPCHWebBrowser::Navigate( const char* szURL, const char* strPostString_, const wchar_t* wstrTargetFrameName_ )
{
    if ( NULL != m_hBrowserWnd )
    {
        DisplayHTMLPage( m_hBrowserWnd, szURL, strPostString_, wstrTargetFrameName_ );

        return TRUE;
    }

    return FALSE;
}
