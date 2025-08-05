#ifndef _KNCP2P_H_
#define _KNCP2P_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment( lib, "Iphlpapi.lib")
#pragma comment( lib, "ws2_32.lib")
#include "KncCommonHeader.h"

class RakPeer;
class KncPeerManager;
class KncThreadRecv;
class KncPeer;
class KncP2P
{
public:
// 함수

    KncP2P();
    virtual ~KncP2P();

    virtual bool InitKncP2P( const DWORD dwUID_ , const KNCMSGHANDLER pFuncMsgHandler_, 
                            unsigned int uiMaxPeer_, DWORD dwServerIP_, u_short usServerPort_, unsigned short usLocalPort_ = DEFAULT_PORT );
    virtual bool ConnectP2P();
	virtual bool ConnectP2P( DWORD dwUID_ );

    virtual void Close();

    virtual void Send( void* pPakcet_, int iPacketSize_, DWORD dwUID_, SendReliability optReliability_ );
    virtual void DisConnectPeer( DWORD dwUID_ );
    virtual void FrameMove();
    
    virtual std::vector<u_short>    GetLocalPort();
    virtual std::vector<DWORD>      GetLocalIP();

    virtual void SetEchoPort( DWORD dwIP_, u_short usPort_ );
    virtual void SetupRelayServer( DWORD dwIP_, USHORT usPort_ );

	virtual void SetServerDown( bool bDown_ );
    virtual bool IsIn( KNCID kncPeerID_ );
    virtual int  GetUserCount( KncUserCount::EUserCount eUserCount_ = KncUserCount::ID_TOTAL );
    virtual DWORD GetMyUID();

    virtual bool AddConnectQ( KConnectInfo& kInfo_ );
    virtual void ResetConnectQ();

    virtual std::vector<DWORD> GetPeerUID();
    //====================================================================================
    // 2006.07.07 : Asirion
    // Debug용 함수들
    virtual void KncP2PLog();
    virtual UINT GetPacketQSize();
    virtual void CreateConsoleWnd();


    virtual DWORD GetAvgPingTime( bool bRelay );
	virtual DWORD GetAvgPingTime( DWORD dwUID_ );
    virtual DWORD GetAvgPartPingTime();
    virtual DWORD GetAvgPartPingTime( DWORD dwUID_ );


    virtual bool IsRelay( DWORD dwUID_ );

	//====================================================================================
	// 2008.05.12 : milennium9
	// 게임마다 고유한 UID를 부여하여 예전에 보낸 패킷을 무시하기 위한 코드
	virtual void SetConnectionUID( DWORD dwUID );
	virtual KncPeerManager*         GetPeerManager();

	void SetRelayOnly( bool bRelay_ );
    void SetDefaultLocalPort( unsigned short _usDefaultLocalPort ) { m_usDefaultLocalPort = _usDefaultLocalPort; }
    unsigned short GetDefaultLocalPort() { return m_usDefaultLocalPort; }

protected:
    // 변수
    unsigned short          m_usLocalPort;
    unsigned short          m_usDefaultLocalPort;   //  디폴트 포트를 게임 켤때마다 바꾼다.
    unsigned int            m_uiMaxPeer;

    bool                    m_bInit;
    RakPeer*                m_pRakPeer;
    KncPeerManager*         m_pPeerManager;
    KncThreadRecv*          m_pThreadRecv;
	DWORD					m_dwConnectionUID;

    FILE*                   m_fpStream;


    // 함수
    KncMsgHandler_CS*       m_pMsgHandler;

    virtual bool GetValidPort();

};

#endif // _KNCP2P_H_
