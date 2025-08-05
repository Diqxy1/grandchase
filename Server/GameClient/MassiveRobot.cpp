#define LUABIND_NO_RTTI
#include "MassiveRobot.h"
#include "SingleRobot.h"
#include "ActionDecider.h"
#include "UserFSM.h"
#include "UserPacket.h"
#include "tchar.h"
#include "NetError.h"
#include <dbg/dbg.hpp>

#include "RobotManager.h" // be problem ? 


UINT KMassiveRobot::ms_uiSingleRobotCount = 1;
UINT KMassiveRobot::ms_uiMassiveRobotUIDDiff = 0;
KMassiveRobot::KMassiveRobot()
{
    InitializeCriticalSection( &m_csClient );
	m_CheckCountDeadClient = 0;
}

KMassiveRobot::~KMassiveRobot()
{
    DeleteCriticalSection( &m_csClient );
}

ImplToStringW( KMassiveRobot )
{
    KLocker lock( m_csClient );
    return stm_ << L"KMassiveRobot dump. " << std::endl
                << TOSTRINGW( KncUtil::toWideString(m_strRobotLogin) )
                << TOSTRINGW( m_vecClient.size() )
                << TOSTRINGW( KSingleRobot::ms_bAutoAction )
                << TOSTRINGW( KSingleRobot::ms_nProtocolVer )
                << TOSTRINGW( KncUtil::toWideString(KSingleRobot::ms_strRemoteIP) )
                << TOSTRINGW( KSingleRobot::ms_usRemoteGameServerPort )
                << TOSTRINGW( KSingleRobot::ms_usRemoteCenterServerPort );
}
ImplOstmOperatorW2A( KMassiveRobot );
/*
void KMassiveRobot::IncreaseClient(  int nIncrease_ )
{
    SetNum( m_vecClient.size() + nIncrease_ );
}

void KMassiveRobot::DecreaseClient(  int nDecrease_ )
{
    KLocker lock( m_csClient );
    if( nDecrease_ > (int)m_vecClient.size() )
        nDecrease_ = m_vecClient.size();
    lock.Unlock();

    SetNum( m_vecClient.size() - nDecrease_ );
}*/

void KMassiveRobot::Init()
{
    //srand( (unsigned) time( NULL ) );

    //dbg::logfile::SetFileNameAtTime( true );

    KUserProxy::InitForClient();    // init winsock, security
    
    m_spFSM.reset(new KUserFSM());

    //SiKIocp()->Init( 4 );
	m_iProtocolVer = 0;
	m_cIP = NULL;
	m_usPort = 0;
	m_cMsgIP = NULL;
	m_usMsgPort = 0;
	m_cMsgIP2nd = NULL;
	m_usMsgPort2nd = 0;
	m_dTestMsgTime = 0;

    m_pthWorker = boost::shared_ptr< KTThread<KMassiveRobot> >( new KTThread<KMassiveRobot>( *this, &KMassiveRobot::Tick, 10 ) );
}

//ImplOstmOperatorW2A( KSingleRobot );

void KMassiveRobot::Run()
{
    // iocp 쓰레드 실행
    //SiKIocp()->BeginThread();

	while(m_vecClient.size() != m_CheckCountDeadClient){
		Tick();
	}

 //   ShutDown();

	//::WSACleanup();
}

void KMassiveRobot::ShutDown()
{

    KLocker lock( m_csClient );
    START_LOG( cout, "종료처리. 현재 클라이언트 수 : " << m_vecClient.size() ) << END_LOG;
    lock.Unlock();

    //SetNum( 0 );
	m_vecClient.erase( m_vecClient.begin() + 0, m_vecClient.end() );
    //::WSACleanup();

    m_pthWorker->End();

    //SiKIocp()->EndThread();
    //KIocp::ReleaseInstance();

    KUserProxy::ReleaseForClient();
    KSingleRobot::ReleaseRobot();

	::WSACleanup();
}

void KMassiveRobot::SetDefaultData(dpParam sdData){
	m_iProtocolVer = sdData.m_iProtocolVer;
	m_cIP = sdData.m_cIP;
	m_usPort = sdData.m_usPort;
	m_cMsgIP = sdData.m_cMsgIP;
	m_usMsgPort = sdData.m_usMsgPort;
	m_cMsgIP2nd = sdData.m_cMsgIP2nd;
	m_usMsgPort2nd = sdData.m_usMsgPort2nd;
	m_dTestMsgTime = sdData.m_dTestMsgTime;
}

void  KMassiveRobot::SingSet(KSingleRobotPtr spClient){
	if(m_iProtocolVer == -1)
		spClient->SetProtocolVer(328);
	else
		spClient->SetProtocolVer(m_iProtocolVer);

	if(!m_cIP)
		spClient->SetRemoteIP("127.0.0.1");
	else
		spClient->SetRemoteIP(m_cIP);

	if(!m_usPort)
		spClient->SetRemoteGameServerPort(9400);
	else
		spClient->SetRemoteGameServerPort(m_usPort);

	if(!m_dTestMsgTime)
		spClient->SetRobotTestMsgTime(1500);
	else
		spClient->SetRobotTestMsgTime(m_dTestMsgTime);

	if(!m_cMsgIP)
		spClient->SetMsgRemoteIP("192.168.199.104");
	else
		spClient->SetMsgRemoteIP(m_cMsgIP);

	if(!m_usMsgPort)
		spClient->SetRemoteMsgServerPort(9300);
	else
		spClient->SetRemoteMsgServerPort(m_usMsgPort);

	if(!m_cMsgIP2nd)
		spClient->SetMsgRemoteIP2nd("192.168.199.104");
	else
		spClient->SetMsgRemoteIP2nd(m_cMsgIP2nd);

	if(!m_usMsgPort2nd)
		spClient->SetRemoteMsgServerPort2nd(9310);
	else
		spClient->SetRemoteMsgServerPort2nd(m_usMsgPort2nd);

}

void KMassiveRobot::SetNum( size_t nClientNum_, std::map<int,std::vector<int>> comque, int *RoomMode )//기본적인 내용으로 초기화 
{
    KLocker lock( m_csClient );

	m_iAllClients = new int[nClientNum_];

    for( size_t i = 0 ; i < nClientNum_; i++ )
    {
        KSingleRobotPtr spClient( new KSingleRobot );

        char buff[128] = {};

        sprintf_s( buff, "%s%04d", m_strRobotLogin.c_str(), ms_uiSingleRobotCount++ );

        spClient->SetName( KncUtil::toWideString(buff).c_str() );
        spClient->SetFSM( m_spFSM );
        spClient->SetRobotID( ms_uiSingleRobotCount );
        spClient->Init( true );
        spClient->SetUID( DWORD(ms_uiMassiveRobotUIDDiff+1013840) );

		SingSet(spClient);
		spClient->SetRoomMode(RoomMode[i]);

		spClient->SetClientCommandQue(comque[i]);

        m_vecClient.push_back( spClient );

        ms_uiMassiveRobotUIDDiff += 10000;

		m_iAllClients[i] = 0;
    }		
}

void KMassiveRobot::SetNum( size_t nClientNum_, std::map<int,std::vector<int>> comque,  DWORD ChannelID, int *RoomMode){
	KLocker lock( m_csClient );

	m_iAllClients = new int[nClientNum_];

	for( size_t i = 0 ; i < nClientNum_; i++ )
	{
		KSingleRobotPtr spClient( new KSingleRobot );

		char buff[128] = {};

		sprintf_s( buff, "%s%04d", m_strRobotLogin.c_str(), ms_uiSingleRobotCount++ );

		spClient->SetName( KncUtil::toWideString(buff).c_str() );
		spClient->SetFSM( m_spFSM );
		spClient->SetRobotID( ms_uiSingleRobotCount );
		spClient->Init( true );
		spClient->SetUID( DWORD(ms_uiMassiveRobotUIDDiff+1013840) );

		SingSet(spClient);
		spClient->SetRoomMode(RoomMode[i]);

		spClient->SetClientCommandQue(comque[i]);
		spClient->SetChannel(ChannelID);

		m_vecClient.push_back( spClient );

		ms_uiMassiveRobotUIDDiff += 10000;
		m_iAllClients[i] = 0;
	}
}

void KMassiveRobot::SetNum( size_t nClientNum_, std::map<int,std::vector<int>> comque,  DWORD ChannelID, CString *ClientID, int *RoomMode){
	KLocker lock( m_csClient );

	m_iAllClients = new int[nClientNum_];
	m_vecClient.clear();
	for( size_t i = 0 ; i < nClientNum_; i++ )
	{
		KSingleRobotPtr spClient( new KSingleRobot );

		char buff[128] = {};

		sprintf_s(buff,"%S", ClientID[i]);

		spClient->SetName(  KncUtil::toWideString(buff).c_str() );
		spClient->SetFSM( m_spFSM );
		spClient->SetRobotID( ms_uiSingleRobotCount );
		spClient->Init( true );
		spClient->SetUID( DWORD(ms_uiMassiveRobotUIDDiff+1013840) );

		SingSet(spClient);
		spClient->SetRoomMode(RoomMode[i]);

		spClient->SetClientCommandQue(comque[i]);
		spClient->SetChannel(ChannelID);

		m_vecClient.push_back( spClient );

		ms_uiMassiveRobotUIDDiff += 10000;
		m_iAllClients[i] = 0;
	}
}

void KMassiveRobot::Tick()
{
    std::vector<KSingleRobotPtr>::iterator  vit;

    KLocker lock( m_csClient );
	int indexClient = 0;
	//START_LOG( cerr, L"클라 수 :" << m_vecClient.size() ) << END_LOG;
    for( vit = m_vecClient.begin() ; vit != m_vecClient.end(); vit++,indexClient++ )
    {
		if( (*vit) == NULL ) {
			continue;
		}
		if(vit != m_vecClient.end()) {
			if((*vit)->GetAlive()) {
				(*vit)->Tick();

				m_iAllClients[indexClient] = (*vit)->GetComqueCount();

			}else {
				if(!((*vit)->GetCheckCount()))
					m_CheckCountDeadClient++;
			}
		}
		//Sleep(1000);
        //Sleep(3);
    } // for
}

int* KMassiveRobot::GetAllClientCom(){
	return m_iAllClients;
}

void KMassiveRobot::FindZombie()
{
    START_LOG( cout, "좀비 유저 찾기!" ) << END_LOG;
    int nNormalUser = 0;

    KLocker lock( m_csClient );

    std::vector<KSingleRobotPtr>::iterator vit;
    for( vit = m_vecClient.begin(); vit != m_vecClient.end(); vit++ )
    {
        //if( (*vit)->GetStateID() == KUserFSM::STATE_EXIT ||
        //    (*vit)->GetStateID() == KUserFSM::STATE_INIT )
        //    continue;

        if ( !(*vit)->IsConnected() ) {   // zombie.
            START_LOG( cerr, L"Zombie : " << (*vit)->GetName() << " - " << (*vit)->GetStateIDString() ) << END_LOG;
        }
        else {
            nNormalUser++;
        }
    }

    START_LOG( cerr, L"좀비 체크 종료 : 정상 " << nNormalUser << L", 전체 " << m_vecClient.size() ) << END_LOG;

}

#include "time.h" // _strtime()

void KMassiveRobot::DumpRobot()
{
    char szbuff[128] = {0};
    int State[ KUserFSM::STATE_SENTINEL ] = {0};

    KLocker lock( m_csClient );
    START_LOG( cerr, L"로봇 덤프 : " << _strtime_s( szbuff ) )
        << BUILD_LOG( m_vecClient.size() ) << END_LOG;

    std::vector<KSingleRobotPtr>::iterator vit;
    for( vit = m_vecClient.begin(); vit != m_vecClient.end(); vit++ )
    {                
        //START_LOG( cout, "Dump : " << (*vit)->GetName() << " - " << (*vit)->GetStateIDString() ) << END_LOG;     
        if( (*vit)->IsConnected() == false && (*vit)->GetStateID() > 1 && (*vit)->GetStateID()  < 8 )
            continue;

        State[ (*vit)->GetStateID() ] ++;        
    }

    for( int i = 0; i < KUserFSM::STATE_SENTINEL ; i++ )
    {
        START_LOG( cerr, L"STATE : "
            << State[ i ]
            << L" - " 
            << m_spFSM->GetStateIDString( i ) ) << END_LOG;
    }
}

KSingleRobot* KMassiveRobot::GetSingleRobot(  int nID_ )
{
    KLocker lock( m_csClient );

    if( nID_ >= (int)m_vecClient.size() )
    {
        START_LOG( cerr, L"해당 아이디의 싱글로봇이 생성되지 않았습니다." )
            << BUILD_LOG( nID_ )
            << BUILD_LOG( m_vecClient.size() ) << END_LOG;

        return NULL;
    }

    return m_vecClient[ nID_ ].get();
}
