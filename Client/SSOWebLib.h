// SSOWebLib.h: interface for the CSSOWebLib class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_)
//#define AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_

#ifndef _SSOWEBLIB_H_
#define _SSOWEBLIB_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////
typedef void* HWEBCTRL;

// SSO 웹브라우저 초기화 함수
BOOL InitSSOWebBrowser(HWND hWndMain, const char* strAuthCookie, const char* strDataCookie, const char* strCpCookie);

// 테스트용 초기화 함수
BOOL InitSSOWebBrowser2(const char* strBaseDir, HWND hWndMain, const char* strAuthCookie, const char* strDataCookie, const char* strCpCookie);

// SSO 웹브라우저 파괴 함수(메인 윈도우가 파괴되기 전에 호출해야한다)
BOOL UnInitSSOWebBrowser();

// 쿠키 활성화 페이지 URL을 얻는 함수
LPCTSTR GetActCookieURL();

// 쿠키 리프레쉬 페이지 URL을 얻는 함수
LPCTSTR GetRefreshCookieURL();

// SSO 웹브라우저로부터 쿠키를 얻는 함수(내부적으로 변환되기 때문에 얻은 그대로 put_cookie하면 된다)
BOOL GetSSOWebCookieString(char* pstrAuth, char* pstrData, char* pstrCp);

//////////////////////////////////////////////////////////////////////
// SSO 웹브라우저 클래스

class CSSOWebBrowser  
{
public:

	HWEBCTRL		m_hWeb;

	CSSOWebBrowser();
	virtual ~CSSOWebBrowser();

	// 웹 브라우저 초기화 및 생성
	BOOL Create(DWORD dwStyle, RECT& rect, HWND hWndParent, UINT nID);
	void Destroy();

	void SetOption(BOOL bHideBorder);
	BOOL PutCharSet(const char* strCharset);
	BOOL InsertHTML(const char* strHtml);
	BOOL ExecJavascript(const char* strScript);

	BOOL Navigate(const char* strURL);
	BOOL GetNetmarbleCookie(char* pstrAuthCookie, char* pstrDataCookie, char* pstrCpCookie);

	void* GetWnd();
	int  GetUserNavigatedCount();
	void SetUserNavigatedCount(int count);
	void ResetUserNavigatedCount();

	static HMODULE	m_hDllModule;
	static BOOL		m_bInitialized;
};

#endif // _SSOWEBLIB_H_
