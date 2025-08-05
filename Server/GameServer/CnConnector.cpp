#include ".\CnConnector.h"
#include "GSSimLayer.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnConnector );

KCnConnector::KCnConnector(void)
//:m_kLastConnectTick(30*1000)
{
    m_spCnPtr.reset();
    m_strIP.clear();
    m_usPort = 0;
    m_dwLastConnectTick = ::GetTickCount();
}

KCnConnector::~KCnConnector(void)
{
    ShutDown();
}

ImplOstmOperatorW2A( KCnConnector ); // GameServer.cpp 의 85라인과 중복(GameServer수정)
ImplToStringW( KCnConnector )
{
    m_spCnPtr->ToString( stm_ );
    return stm_;
}

void KCnConnector::Init()
{
    if( m_spCnPtr ) {
        m_spCnPtr->OnDestroy();
        m_spCnPtr.reset();
    }

    m_spCnPtr.reset( new KCenterProxy );
}

void KCnConnector::Tick()
{
    if( m_spCnPtr ) // Null인 상태로 여기를 지나가게 하지 말자.
        m_spCnPtr->Tick(); // 연결 종료시 OnDestroy호출 되므로 연결 끊긴 상태

    if( m_spCnPtr && !m_spCnPtr->IsConnected() )
    {
        // 어떤 액션이 있을까?
        if( ::GetTickCount() - m_dwLastConnectTick > 30 * 1000 )
        {
            m_spCnPtr.reset( new KCenterProxy );
            if( !Connect() ) return;

            std::vector<DWORD> vecUIDs;
            SiKGSSimLayer()->GetUserUIDs( vecUIDs );
            if( vecUIDs.empty() ) return;

            LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_ADD_MULTI_USER_REQ, vecUIDs, true, true ) ); // 센터 서버에 전체 유저 등록.
            START_LOG( cerr, L" Reconnect To Center.. : " << vecUIDs.size() ) << END_LOG;
        }
    }
}

//bool KCnConnector::InitConnect()
//{
//    m_spThreadKeepConnect = std::shared_ptr< KTThread<KCnConnector> >( new KTThread<KCnConnector>( *this, &KCnConnector::KeepConnectionThread, 10000 ) );
//
//    if ( m_spThreadKeepConnect ) {
//        m_spThreadKeepConnect->Begin();
//        START_LOG( cerr, L"CnConnector 재접속 Thread 시작" ) << END_LOG;
//        return true;
//    }
//
//    START_LOG( cerr, L"CnConnector 재접속 Thread 생성 실패" ) << END_LOG;
//    return false;
//}

bool KCnConnector::Connect()
{
    m_dwLastConnectTick = ::GetTickCount();

    _JIF( m_spCnPtr, return false );
    __JIF( !m_strIP.empty(), return false );
    __JIF( m_usPort > 0, return false );
    __JIF( !m_spCnPtr->IsConnected(), return true );

    m_spCnPtr->SetName( m_strServerName );
    if( !m_spCnPtr->Connect( m_strIP, m_usPort ) )
    {
        START_LOG( cerr, L"센터 서버로 접속 실패.. IP : "
            << KncUtil::toWideString(m_strIP) << L", Port : " << m_usPort ) << END_LOG;
        return false;
    }

    return true;
}

void KCnConnector::ShutDown()
{
    // 일단 재접속 Thread부터 죽인다.
    //m_spThreadKeepConnect->End( 3000 );

    if( m_spCnPtr )
    {
        m_spCnPtr->OnDestroy();
        m_spCnPtr.reset();
    }
}

void KCnConnector::SetRemoteAddress( const char* szIP_, unsigned short usPort_ )
{
    m_strIP = szIP_;
    m_usPort = usPort_;
}

void KCnConnector::SetName( const std::wstring& strServerName_ )
{
    m_strServerName = strServerName_;
}

bool KCnConnector::SendServerRelayPacket( const unsigned short usEventID_, const KSerBuffer& kEventBuff_, const DWORD dwSenderUID_, const std::wstring& strSenderLogin_,
                                          const DWORD dwRecverUID_, const std::wstring& strRecverLogin_, const bool bUID_, const bool bDirectSend_ )
{
    __JIF( m_spCnPtr, return false );
    __JIF( m_spCnPtr->IsConnected(), return false );
    __JIF( m_spCnPtr->IsAfterVerify(), return false );

    KServerRelayPacket kPacket;
    kPacket.m_dwSenderUID       = dwSenderUID_;
    kPacket.m_strSenderLogin    = strSenderLogin_;
    kPacket.m_dwRecverUID       = dwRecverUID_;
    kPacket.m_strRecverLogin    = strRecverLogin_;
    kPacket.m_bUseUID           = bUID_;
    kPacket.m_bDirectSend       = bDirectSend_;
    kPacket.m_usEventID         = usEventID_;
    kPacket.m_kEventBuff        = kEventBuff_;

    return m_spCnPtr->SendPacket( KCenterEvent::ECN_SERVER_RELAY_PACKET_REQ, kPacket );
}

bool KCnConnector::SendSquareRelayPacket( const unsigned short usEventID_, const KSerBuffer& kEventBuff_, const DWORD dwSenderUID_, const DWORD dwSquareUID_, const bool bNeighborhoodSend_ )
{
    __JIF( m_spCnPtr, return false );
    __JIF( m_spCnPtr->IsConnected(), return false );
    __JIF( m_spCnPtr->IsAfterVerify(), return false );

    KSquareRelayPacket kPacket;
    kPacket.m_dwSenderUID       = dwSenderUID_;
    kPacket.m_dwSquareUID       = dwSquareUID_;
    kPacket.m_usEventID         = usEventID_;
    kPacket.m_kEventBuff        = kEventBuff_;
    kPacket.m_bNeighborhoodSend = bNeighborhoodSend_;

    return m_spCnPtr->SendPacket( KCenterEvent::ECN_SQUARE_RELAY_PACKET_REQ, kPacket );        
}

bool KCnConnector::SendID( unsigned short usEventID_ )
{
    __JIF( m_spCnPtr, return false );
    __JIF( m_spCnPtr->IsConnected(), return false );
    __JIF( m_spCnPtr->IsAfterVerify(), return false );
    return m_spCnPtr->SendID( usEventID_ );
}

//void KCnConnector::KeepConnectionThread()
//{
//    if ( m_spCnPtr->IsConnected() ) {
//        return;
//    }
//
//    if ( !m_kLastConnectTick.CheckTime() ) {
//        return;
//    }
//
//    m_spCnPtr.reset( new KCenterProxy );
//    if ( !Connect() ) {
//        return;
//    }
//
//    // 센터 서버에 전체 유저 등록.
//    SendAddMuliUserReq();
//}
//
//void KCnConnector::SendAddMuliUserReq()
//{
//    std::vector<DWORD> vecUIDs;
//    SiKGSSimLayer()->GetUserUIDs( vecUIDs );
//    if ( vecUIDs.empty() ) return;
//
//    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_ADD_MULTI_USER_REQ, vecUIDs, true, true ) );
//    START_LOG( cerr, L" Reconnect To Center.. : " << vecUIDs.size() ) << END_LOG;
//}