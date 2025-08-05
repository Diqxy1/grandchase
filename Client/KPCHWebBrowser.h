// file   : KPCHWebBrowser.h
// since  : 2005-06-02 오후 3:36:40
// author : pch413@kogstudios.com
// This requires IE 5.0 ( or better ).

// Park Chang-hyeon's web browser class header

#ifndef _KPCHWEBBROWSER_H_
#define _KPCHWEBBROWSER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
//#include <string>

// Parkch's 웹브라우저 초기화 함수
BOOL InitPCHWebBrowser( void );

// Parkch's 웹브라우저 파괴 함수
BOOL UnInitPCHWebBrowser( void );

// Parkch's 웹브라우저 쿠키 셋팅 함수
void SetPCHWebBrowserCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc );

class KPCHWebBrowser
{
public:
    // Constructor
    KPCHWebBrowser( void );
    // Destructor
    ~KPCHWebBrowser( void );

    // Methods
    BOOL Create( DWORD dwStyle, const RECT& rc, HWND hWndParent, UINT id );
    void Destroy( void );
	BOOL Navigate( const char* szURL, const char* strPostString_, const wchar_t* wstrTargetFrameName_ = L"" );

private:
    HWND    m_hWrapperWnd;
    HWND    m_hBrowserWnd;
};

#endif // _KPCHWEBBROWSER_H_