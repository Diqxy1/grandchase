#ifndef _KBROWSERWRAPPER_H_
#define _KBROWSERWRAPPER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NATION_KOREA
#   define __NETMARBLE_SERVICE__
#	pragma message("__NETMARBLE_SERVICE__ macro is defined!")
#endif // _JAPANESE


#   include "SSOWebLib.h"
#   include "KPCHWebBrowser.h"
#include "NMWebCookieCtrlLib.h"


typedef void (*BrowserEndEvent)();

class KBrowserWrapper
{
public:
    // Constructor
    KBrowserWrapper( void );
    // Destructor
    virtual ~KBrowserWrapper( void );

    virtual BOOL Create( RECT& rect, HWND hWndParent );
    virtual bool OpenBrowser( HWND hDlg );
    virtual void CloseBrowser( void );

    virtual BOOL Init( HWND hWndMain, const char* szAuthCookie, const char* szDataCookie, const char* szCpCookie );    
    virtual BOOL UnInit( void );    
    virtual void SetCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc );    
    virtual bool AllocateBrowserObj( void );    
    virtual void DeleteBrowserObj( void );    
	// strPostString_ <- Post할 String
	virtual BOOL Navigate( const char* strURL, const char* strPostString_, const wchar_t* wstrTargetFrameName_ = L"" );
    
    inline bool IsClosed( void ) const
    {
        return m_bClosed;
    }
    inline HWND GetWndHandle( void ) const
    {
        return m_hDlg;
    }
	inline void SetEndFunction( BrowserEndEvent func )
	{
		m_pEndFunc = func;
	}
    void DestroyWin( );

    void SetReqGoodsID( DWORD _dwGoodsID );
    DWORD GetReqGoodsID( ) { return m_dwLastReqBuyGoodsID; };

protected:
    RECT            m_rect;
    bool            m_bClosed;
    HWND            m_hDlg;
	BrowserEndEvent m_pEndFunc;
	bool			m_bInit;
private:
    KPCHWebBrowser* m_pPCHBrowser;
    
    DWORD           m_dwLastReqBuyGoodsID;

};

class KNetmarbleBrowser : public KBrowserWrapper
{
public:
	// Constructor
	KNetmarbleBrowser( void );
	// Destructor
	virtual ~KNetmarbleBrowser( void );


	virtual bool OpenBrowser( HWND hDlg );
	virtual void CloseBrowser( void );

	virtual BOOL Init( HWND hWndMain, const char* szAuthCookie, const char* szDataCookie, const char* szCpCookie );    
	virtual BOOL UnInit( void );    
	virtual void SetCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc );    
	virtual bool AllocateBrowserObj( void );    
	virtual void DeleteBrowserObj( void );    
	// strPostString_ <- Post할 String
    virtual BOOL Navigate( const char* strURL, const char* strPostString_, const wchar_t* wstrTargetFrameName_ = L"" );
private:
	CSSOWebBrowser* m_pSSOBrowser;

};



class KToonilandBrowser : public KBrowserWrapper
{
public:
	// Constructor
	KToonilandBrowser( void ){};
	// Destructor
	virtual ~KToonilandBrowser( void ){};


	virtual BOOL Init( HWND hWndMain, const char* szAuthCookie, const char* szDataCookie, const char* szCpCookie );    
	virtual BOOL UnInit( void );    
	virtual void SetCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc );    

private:
	CSSOWebBrowser* m_pSSOBrowser;

};


extern KBrowserWrapper* g_pBrowser;


#endif // _KBROWSERWRAPPER_H_
