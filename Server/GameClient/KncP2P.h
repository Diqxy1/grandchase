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

    bool InitKncP2P( const DWORD dwUID_ , const KNCMSGHANDLER pFuncMsgHandler_, 
                            unsigned int uiMaxPeer_, DWORD dwServerIP_, u_short usServerPort_, unsigned short usLocalPort_ = DEFAULT_PORT );
    bool ConnectP2P();
    void Close();

    void Send( void* pPakcet_, int iPacketSize_, DWORD dwUID_, SendReliability optReliability_ );
    void DisConnectPeer( DWORD dwUID_ );
    void FrameMove();
    
    std::vector<u_short>    GetLocalPort();
    std::vector<DWORD>      GetLocalIP();

    void SetEchoPort( DWORD dwIP_, u_short usPort_ );
    void SetupRelayServer( DWORD dwIP_, USHORT usPort_ );

	void SetServerDown( bool bDown_ );
    bool IsIn( KNCID kncPeerID_ );
    int  GetUserCount( KncUserCount::EUserCount eUserCount_ = KncUserCount::ID_TOTAL );
    DWORD GetMyUID();

    bool AddConnectQ( KConnectInfo& kInfo_ );
    void ResetConnectQ();

    std::vector<DWORD> GetPeerUID();
    //====================================================================================
    // 2006.07.07 : Asirion
    // Debug용 함수들
    std::vector<DWORD> GetConnectQUID();
    void KncP2PLog();
    UINT GetPacketQSize();
    UINT GetErrorInfo();
    void CreateConsoleWnd();
    DWORD GetAvgPingTime();
	DWORD GetAvgPingTime( DWORD dwUID_ );
private:
    // 변수
    unsigned short          m_usLocalPort;
    unsigned int            m_uiMaxPeer;

    bool                    m_bInit;
    RakPeer*                m_pRakPeer;
    KncPeerManager*         m_pPeerManager;
    KncThreadRecv*          m_pThreadRecv;

    FILE*                   m_fpStream;
    // 함수
    KncMsgHandler_CS*       m_pMsgHandler;

    bool GetValidPort();
};

#endif // _KNCP2P_H_
