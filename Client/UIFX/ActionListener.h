#ifndef _ACTIONLISTENER_H_
#define _ACTIONLISTENER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <string>

class KD3DWnd;

typedef INT64 KEVETPARM; 

class KActionEvent
{
public:
    KActionEvent()
        : m_pWnd( NULL )
        , m_dwCode( 0 )
        , m_dwlParam( 0 )
        , m_dwlParam2( 0 )
    {
    }

    KActionEvent( KD3DWnd* pWnd, DWORD dwCode = 0, KEVETPARM dwlParam = 0, KEVETPARM dwlParam2 = 0 )
    {
        m_pWnd = pWnd;
        m_dwCode = dwCode;
		m_dwlParam = dwlParam;
		m_dwlParam2 = dwlParam2;
    }
    ~KActionEvent( void )
    {
        m_pWnd = NULL;
        m_dwCode = 0;
		m_dwlParam = 0;
		m_dwlParam2 = 0;
    }
    KActionEvent( const KActionEvent& src )
    {
        m_pWnd = src.m_pWnd;
        m_dwCode = src.m_dwCode;
		m_dwlParam = src.m_dwlParam;
		m_dwlParam2 = src.m_dwlParam2;
    }
    const KActionEvent& operator=( const KActionEvent& src )
    {
        m_pWnd = src.m_pWnd;
        m_dwCode = src.m_dwCode;
		m_dwlParam = src.m_dwlParam;
		m_dwlParam2 = src.m_dwlParam2;
        return *this;
    }

public:
    KD3DWnd*    m_pWnd;
    DWORD       m_dwCode;
	KEVETPARM		m_dwlParam;
	KEVETPARM		m_dwlParam2;
};

class KActionListener
{
public:
    virtual void ActionPerformed( const KActionEvent& ) = 0;
};

#endif // _ACTIONLISTENER_H_