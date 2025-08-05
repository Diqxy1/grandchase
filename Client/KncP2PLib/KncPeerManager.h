#ifndef _KNCPEERMANAGER_H_
#define _KNCPEERNAMAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KncCommonHeader.h"
#include "./RakNetSrc/RakNetworkFactory.h"
#include "./RakNetSrc/RakServerInterface.h"
#include "./RakNetSrc/RakClientInterface.h"
#include "KncPeer.h"
#include "KncP2PError.h"

class KncPeerManager
{
public:
    enum
    {
        DEFAULT_INIT_PING = 0,
        LIMIT_PING_LOSS_COUNT = 5,
        LIMIT_PING_TIME = 1000,
    };

    // 변수
    KncPeer*                        m_pMyPeer;  
    //====================================================================================
    // 2006.07.04 : Asirion
    // KncPeerManager 수정작업.
    KncMap< DWORD, KncPeer*>        m_mapPeer;
    KConnectInfo                    m_kConnectInfo;
    // 함수
    KncPeerManager(void);
    ~KncPeerManager(void);
    //====================================================================================
    // 2006.07.04 : Asirion
    // KncPeerManager 수정작업.
    virtual KncPeer* CreatePeer( DWORD dwUID_, const PlayerID& kRakPeerID_, bool bLocal_ = false, bool bRelay_ = false );
    virtual KncPeer* GetMyPeer() { return m_pMyPeer; }
    virtual KncPeer* GetPeer( DWORD dwUID_ );

    virtual void RemovePeer( DWORD dwUID_ );
    virtual bool IsIn( DWORD dwUID_ );
    virtual void SetConnectInfo( const KConnectInfo& kConnectInfo_ );
    virtual void GetConnectInfo( KConnectInfo* pkConnectInfo_ );
    virtual int  GetUserCount( KncUserCount::EUserCount eUserCount_ );
    virtual void CleanUp();
    //====================================================================================
    // 2006.11.15 : Asirion
    // Error 통계 모듈.
    virtual std::vector< DWORD > GetPeerUID();
    
    virtual DWORD GetAvgPingTime( bool bRelay );
	virtual DWORD GetAvgPingTime( DWORD dwUID_ );
    virtual DWORD GetAvgPartPingTime();
    virtual DWORD GetAvgPartPingTime( DWORD dwUID_ );

    virtual bool IsRelay( DWORD dwUID_ );

};

#endif // _KNCPEERNAMAGER_H_