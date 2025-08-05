#ifndef _KGCSERVERBAR_H_
#define _KGCSERVERBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "CommonPacket.h"

class KD3DStatic;

class KGCServerBar: public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCServerBar );
    /// Default constructor
    KGCServerBar( void );
    /// Default destructor
    virtual ~KGCServerBar( void );
    /// Copy constructor
    KGCServerBar( const KGCServerBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCServerBar& operator=( const KGCServerBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void ActionPerformed( const KActionEvent& event );
    void OnCreateComplete( void );
    void OnCreate( void );

    void SetServerInfo( KServerInfo* pServerInfo );
    void SetServerName( std::wstring& strServerName );
    void SetGauge( int iPercent );

	KServerInfo GetServerInfo()
	{
		return m_kServerInfo;
	};

    enum
    {
        NUM_SERVER_STATUS = 5,

#if defined( NATION_TAIWAN )
        MAX_SERVER_BAR_WIDTH = 136,
#else
        MAX_SERVER_BAR_WIDTH = 130,
#endif

        MIN_PERCENT = 6,
    };

    void OnSelectServer();

	virtual void FrameMoveInEnabledState( void );
private:
    D3DXVECTOR4 m_vTextureCoords;
    KServerInfo m_kServerInfo;

    KD3DStatic* m_pkServerName;
    KSafeArray<KD3DWnd*,NUM_SERVER_STATUS> m_apkStatus;
    
    KD3DWnd*    m_pkBalanceMark;
    KD3DWnd*    m_pkMatchServerEdge;

    bool ConnectGameServer();
	int m_iServerState;

public:
	int GetServerState() { return m_iServerState; }
    bool IsServerUID(DWORD dwServerUID);
    bool RemainServer();
	void OptimizationPos();

	void ReSetInfo();

public:
	bool m_bSendHackCheckPacket;
};

DEFINE_WND_FACTORY( KGCServerBar );
DECLARE_WND_FACTORY( KGCServerBar );
DECLARE_WND_FACTORY_NAME( KGCServerBar );

#endif // _KGCSERVERBAR_H_