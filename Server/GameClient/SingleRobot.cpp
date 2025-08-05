#include <WinSock2.h>
#include "SingleRobot.h"
#include "MassiveRobot.h"
#include "SOCKET/NetCommon.h"
#include "UserFSM.h"
#include "Gamble.h"
#include "NetError.h"
#include "DefaultFSM.h"
#include "FailRate.h"


bool WINAPI KncP2PMessageHandler( DWORD dwMessageID, PVOID pMsgBuffer/*, int iSingleRobotID*/ );

#define rbCASE( id_ack ) \
    case EVENT_TYPE##::##id_ack: \
    { \
        K##id_ack kPacket; \
        if( ks.Get(kPacket) ) \
        { \
            START_LOG( clog, L"rb>> " L#id_ack L" " ) << END_LOG; \
            ON_##id_ack( kPacket, EVENT_TYPE##::##id_ack ); \
        } \
        else \
        { \
            START_LOG( cerr, L"K" L#id_ack L" deserialize 실패. opcode:" \
            << spEvent->m_usEventID \
            << L",(" << spEvent->GetEventIDString() << L")" ) \
            << END_LOG; \
        } \
        break; \
    }

#define _rbCASE( id_ack, packet ) \
    case EVENT_TYPE##::##id_ack: \
    { \
        packet kPacket; \
        if( ks.Get(kPacket) ) \
        { \
            START_LOG( clog, L"rb>> " L#id_ack L" " ) << END_LOG; \
            ON_##id_ack( kPacket, EVENT_TYPE##::##id_ack ); \
        } \
        else \
        { \
            START_LOG( cerr, L"K" L#id_ack L" deserialize 실패. opcode:" \
            << spEvent->m_usEventID \
            << ",(" << spEvent->GetEventIDString() << ")" ) \
            << END_LOG; \
        } \
        break; \
    }
#define NUserCASE( id_ack ) \
    case KNUserEvent::##id_ack: \
    { \
        K##id_ack kPacket; \
        if( ks.Get(kPacket) ) \
        { \
            START_LOG( clog, L"rb>> " L#id_ack L" " ) << END_LOG; \
            ON_##id_ack( kPacket ); \
        } \
        else \
        { \
            START_LOG( cerr, L"K" L#id_ack L" deserialize 실패. opcode:" \
            << spEvent->m_usEventID \
            << ",(" << spEvent->GetEventIDString() << ")" ) \
            << END_LOG; \
        } \
        break; \
    }
#define rbCASE_WAIT( id_ack, id_req ) \
    case EVENT_TYPE##::##id_ack: \
    { \
        if( m_usWaitingID == EVENT_TYPE##::##id_req ) \
        { \
            m_bOnWaitingAck = false; \
        } \
        else \
        { \
            START_LOG( cerr, L"" << GetName() << dbg::endl << L"도착한 이벤트 : " L#id_ack << dbg::endl \
            << L"기다리는 이벤트 : " << EVENT_TYPE##::##ms_szEventID[ m_usWaitingID ] << L" 에 대한 ack." ) << END_LOG; \
        } \
        K##id_ack kPacket; \
        if( ks.Get(kPacket) ) \
        { \
            START_LOG( cerr,  L"K" L#id_ack L"ks.get(kpacket) == true" ) \
            << END_LOG; \
            START_LOG( clog, L"rb>> " L#id_ack L" " ) << END_LOG; \
            ON_##id_ack( kPacket, EVENT_TYPE##::##id_ack ); \
        } \
        else \
        { \
            START_LOG( cerr, L"K" L#id_ack L" deserialize 실패. opcode:" \
            << spEvent->m_usEventID \
            << L",(" << spEvent->GetEventIDString() << L")" ) \
            << END_LOG; \
        } \
        break; \
    }

#define _rbCASE_WAIT( id_ack, packet, id_req ) \
    case EVENT_TYPE##::##id_ack: \
    { \
        if( m_usWaitingID == EVENT_TYPE##::##id_req ) \
        { \
            m_bOnWaitingAck = false; \
        } \
        else \
        { \
            START_LOG( cerr, GetName() << dbg::endl << L"도착한 이벤트 : " L#id_ack << dbg::endl \
            << L"기다리는 이벤트 : " << EVENT_TYPE##::##ms_szEventID[ m_usWaitingID ] << L" 에 대한 ack." ) << END_LOG; \
        } \
        packet kPacket; \
        if( ks.Get(kPacket) ) \
        { \
        START_LOG( cerr, L"_ks.get(kpacket) == true" ) \
        << END_LOG; \
            START_LOG( clog, L"rb>> " L#id_ack L" " ) << END_LOG; \
            ON_##id_ack( kPacket, EVENT_TYPE##::##id_ack ); \
        } \
        else \
        { \
            START_LOG( cerr, L"K" L#id_ack L" deserialize 실패abc. opcode:" \
            << spEvent->m_usEventID \
            << L",(" << spEvent->GetEventIDString() << L")" ) \
            << END_LOG; \
        } \
        break; \
    }

#define EVENT_TYPE KUserEvent
#define CLASS_TYPE KSingleRobot

NiImplementRTTI( KSingleRobot, KUserProxy );

// init static member data & macro
//KActionDecider  KSingleRobot::ms_kActionDecider( ACTION_SENTINEL );
KActionDecider  KSingleRobot::ms_kActionDecider( 0 );
bool            KSingleRobot::ms_bAutoAction    ( true );
int             KSingleRobot::ms_nProtocolVer   ( -1 );
std::string     KSingleRobot::ms_strRemoteIP    ( "127.0.0.1" );
std::string     KSingleRobot::ms_strCenterRemoteIP    ( "127.0.0.1" );
unsigned short  KSingleRobot::ms_usRemoteGameServerPort   ( -1 );
unsigned short  KSingleRobot::ms_usRemoteCenterServerPort ( -1 );
std::wstring    KSingleRobot::ms_strRobotTestMsg( L"." );
DWORD           KSingleRobot::ms_dwRobotTestMsgSendTime( 0 );
KSerBuffer      KSingleRobot::ms_kSerBuffer;

std::string     KSingleRobot::ms_strMsgRemoteIP    ( "127.0.0.1" );
unsigned short  KSingleRobot::ms_usRemoteMsgServerPort   ( -1 );

std::string     KSingleRobot::ms_strMsgRemoteIP2nd    ( "127.0.0.1" );
unsigned short  KSingleRobot::ms_usRemoteMsgServerPort2nd   ( -1 );
bool            KSingleRobot::ms_bLoginOnly(false);

#undef KENUM
#define KENUM( id ) #id,
const char* KSingleRobot::ms_szAction[] = {
#include "RobotAction_def.h"
};

#define CONNECT_DEFER_TIME      200    // 접속이 끊기고 재접속 하기까지의 지연시간.
#define ACTION_INTERVAL_TIME    200    // 이전 액션과 다음 액션 사이의 시간간격

KSingleRobot::KSingleRobot(void)
{
    m_bConnectToCenter      = false;
    m_bOnWaitingAck         = false;    
    m_dwDisconnectedTick    = ::GetTickCount() - CONNECT_DEFER_TIME;
    m_dwLatestActionTick    = ::GetTickCount() - ACTION_INTERVAL_TIME;
    m_pKncP2P               = NULL;
    m_usWaitingID           = 0;
    m_dwLastSendTime        = 0;
    m_countCommand			= 0;//명령어 진행 변수
    m_iCheckCountClinet		= 0;
    ClientLive				= true;
    checkChannelAck			= false;
    m_iChannelID			= -1;
    m_iRoomMode				= 0;
    m_bcheckDisconnect		= false;
    m_pkNUserProxy = new KNUserProxy();
    m_pkMSProxy    = new KMSProxy();
    //m_pkUserProxy  = new KUserProxy();

    m_pkNUserProxy->Init( false );
    m_pkMSProxy->Init( true );
    //m_pkUserProxy->Init(false);

    m_vecLocation.push_back(L" Free1 / Room1 / On ");
    m_vecLocation.push_back(L" Free2 / Room2 / OFF ");
    m_vecLocation.push_back(L" Free3 / Room3 / On ");
    m_vecLocation.push_back(L" Royal / Room3 / OFF ");
    m_vecLocation.push_back(L" Tower / Room3 / On ");
    srand( time(NULL) );

    m_bFirstRun = true;
}

KSingleRobot::~KSingleRobot(void)
{
    SAFE_DELETE( m_pKncP2P );
    SAFE_DELETE( m_pkNUserProxy );
    SAFE_DELETE( m_pkMSProxy );
    //SAFE_DELETE( m_pkUserProxy );

}

bool KSingleRobot::GetAlive(){
    return ClientLive;
}

int KSingleRobot::GetCheckCount(){
    m_iCheckCountClinet++;

    return m_iCheckCountClinet-1;
}

ImplToStringW( KSingleRobot )
{
    return START_TOSTRING_PARENTW( KUserProxy )
        << TOSTRINGW( m_dwLatestActionTick )
        << TOSTRINGW( m_bOnWaitingAck );        
}

void KSingleRobot::ReleaseRobot()
{
}

int KSingleRobot::DecideAction()
{
    int iAction = ms_kActionDecider.MakeDecision( GetStateID() );

    return static_cast<eACTION>( iAction );
}

void KSingleRobot::WriteFileClientEnterChannelList(){

    std::ofstream fout;
    fout.open("BotTestChannel.txt");
    char channelName[256];

    std::vector<KChannelInfo>::iterator viter = m_vecChannelInfo.begin();
    for(;viter != m_vecChannelInfo.end() ; viter++){
        sprintf_s(channelName, 256,"%S",viter->m_strChannelName.c_str());
        if(viter->m_cType == 0)
            fout << channelName << "." << viter->m_dwChannelUID << "." << "0" << "." <<viter->m_usMaxUsers << "." << viter->m_usUsers << "." << viter->m_bHide << "\n";
        else
            fout << channelName << "." << viter->m_dwChannelUID << "." << viter->m_cType  << "." <<viter->m_usMaxUsers << "." << viter->m_usUsers << "." << viter->m_bHide << "\n";
    
    }

    fout.close();

    return;
}

void KSingleRobot::SetChannel( DWORD ChannelID){
    m_iChannelID = ChannelID;
}

void KSingleRobot::Tick()
{
    KUserProxy::Tick();

    // ms_bAutoAction이 켜져 있을 때 random action 실행.
    if( !ms_bAutoAction )
    {
        return;
    }

    // 액션과 액션 사이에 일정 시간 지연을 둔다.
    if( ::GetTickCount() - m_dwLatestActionTick < ACTION_INTERVAL_TIME )
    {
        return;
    }

    // 접속이 종료되고 난 뒤 재접속 하기까지 일정 시간 지연을 둔다.
    if( ::GetTickCount() - m_dwDisconnectedTick < CONNECT_DEFER_TIME )
    {
        return;
    }

    if(!m_bOnWaitingAck){
        if ( m_countCommand < static_cast< int >( CommandQue.size() ) ){
            m_countCommand++;
            DoAction(CommandQue[m_countCommand-1]);
            
        } else {//명령을 초기화 시킨다. 
            //CommandQue.clear();
            if(m_bcheckDisconnect){//명령어를 모두 실행하고 disconnect가 되었다면
                ClientLive = false;
            }
        }
    }	
    HandleRecvMsg();
        //아래는 2013 8 28일 오전까지 존재했으나
    //명령을 모두 완료한후 자동으로 종료되는 것이 이상하다 판단되어 오후에 제거된 부분
    //현재는 명령을 모두 완료한 후에 DISCONNECT명령이 있지 않으면 연결을 종료하지 않는다.
    //if(CommandQue.size() == 0){
    //	//KUserProxy::OnDestroy();
    // //   
    //	//KSession::Init(true);
    //	//m_pkNUserProxy->Init(false);
    //	ClientLive = false;
    //}
}

bool KSingleRobot::SetClientCommandQue(std::vector<int> comque){
    CommandQue = comque;

    return true;
}

int KSingleRobot::GetComqueCount(){
    return m_countCommand;
}

void KSingleRobot::DoAction(  int nAction_ )
{
    if( nAction_ == ACTION_SENTINEL ) return;
    
    switch( nAction_ )
    {
        // EVENTID가 아닌 Action ID이기 때문에 이전에 쓰던 매크로를 사용하기 힘들다.
#define CLIENT_CASE( id )       case id: ON_##id( id ); break

        // STATE pre-CHANNELLOBBY
        CLIENT_CASE( ACTION_CONNECT );
        CLIENT_CASE( ACTION_VERIFY );
        CLIENT_CASE( ACTION_MSG_CONNECT );
        
        CLIENT_CASE( ACTION_CHANGE_STATE ); // change my location for msg server test
        CLIENT_CASE( ACTION_UPDATE_MYDATA ); // 
        CLIENT_CASE( ACTION_WAIT );
        // STATE CHANNELLOBBY
        CLIENT_CASE( ACTION_CH_LIST );
        CLIENT_CASE( ACTION_NOTIFY );
        CLIENT_CASE( ACTION_CHATTING );
        CLIENT_CASE( ACTION_WHISPER );
        CLIENT_CASE( ACTION_ENTER_CH );

        // STATE CHANNEL
        CLIENT_CASE( ACTION_ROOM_LIST );
        CLIENT_CASE( ACTION_USER_LIST );
        //CLIENT_CASE( ACTION_WHERE_USER );
        CLIENT_CASE( ACTION_LEAVE_CH );
        CLIENT_CASE( ACTION_JOIN_ROOM );
        CLIENT_CASE( ACTION_CREATE_ROOM );

        //BUY Item
        CLIENT_CASE( ACTION_BUY_FOR_GP );
        CLIENT_CASE( ACTION_BUY_FOR_GEM );
        CLIENT_CASE( ACTION_BUY_FOR_CRYSTAL );
        //CLIENT_CASE( ACTION_BUY_COUNT_FOR_GP );
        CLIENT_CASE( ACTION_QUERY_INVENTORY );
        //CLIENT_CASE( ACTION_EQUIP_ITEM );
        CLIENT_CASE( ACTION_SELL_ITEM );
        CLIENT_CASE( ACTION_STRONG );

        //STATE ROOM
        CLIENT_CASE( ACTION_INVITE );
        CLIENT_CASE( ACTION_LEAVE_ROOM );
        CLIENT_CASE( ACTION_START_GAME );

        //STATE GAME
        CLIENT_CASE( ACTION_USE_ITEM );
        CLIENT_CASE( ACTION_END_GAME );

        CLIENT_CASE( ACTION_DISCONNECT );

        case -1:
            checkChannelAck = true;
            break;

        default:
            return;
    }

    //MessageBox(NULL,TEXT("SWITCH1"),TEXT("SWITCH1"),MB_OK);
    // 클라이어트 이름, 액션 이름, 이전 액션과의 시간차 (초 단위)
    START_LOG( clog, L"Name : " << m_strName 
        << ", Action : " << ms_szAction[ nAction_ ] 
        << ", Interval : " << (GetTickCount() - m_dwLatestActionTick) * 0.001f ) << END_LOG;

    m_dwLatestActionTick = ::GetTickCount();
    //MessageBox(NULL,TEXT("SWITCH2"),TEXT("SWITCH2"),MB_OK);
}


void KSingleRobot::OnDestroy()
{
    //050528. microcat.
//    START_LOG_WITH_NAME( cwarn ) << END_LOG;
//    std::vector<KTrace>::const_iterator  vit;
//    for(vit = m_vecTrace.begin(); vit != m_vecTrace.end(); vit++)
//    {
//        dbg::cwarn << vit->m_dwID << "(" << vit->m_dwState <<") -> ";
//    }
//    dbg::cwarn << END_LOG;


    KUserProxy::OnDestroy();

    // 일부러 끊든지, 끊어지든지, 이 함수 불림.
    // 의도적으로 끊었을 때 IOCP 에서 감지해 끊어진걸로 인식될 수 있으나,
    // 이 경우 Destroy()가 두번 처리 될 뿐, 이상 없음.

    m_pkCurrentState = m_spFSM->GetState( KUserFSM::STATE_EXIT );

    m_bOnWaitingAck = false; 
    m_dwDisconnectedTick = GetTickCount();
    
    //m_pkCurrentState = m_spFSM->GetState( KUserFSM::STATE_INIT );
}

void KSingleRobot::HandleRecvMsg()
{
    KSerializer ks;
    
    KEventPtr spEvent;
    {
        KLocker lock( m_csRecvMsg ); 
        while( !m_queRecvMsg.empty() )
        {
            spEvent = m_queRecvMsg.front();
            ks.BeginReading( const_cast<KSerBuffer*>( &spEvent->m_kbuff ) );

            switch( spEvent->m_usEventID ) {
                rbCASE_WAIT( EVENT_VERIFY_ACCOUNT_ACK, EVENT_VERIFY_ACCOUNT_REQ );
                //rbCASE( EVENT_CHANNEL_LIST_NOT );
                _rbCASE_WAIT( EVENT_ENTER_CHANNEL_ACK, int, EVENT_ENTER_CHANNEL_REQ );

                _rbCASE_WAIT( EVENT_REGISTER_NICKNAME_ACK,KPacketNameOK, EVENT_REGISTER_NICKNAME_REQ );

                //rbCASE( EVENT_CHAT_NOT );
                rbCASE( EVENT_WHISPER_ACK );

                rbCASE_WAIT(EVENT_CHANNEL_LIST_ACK,EVENT_CHANNEL_LIST_REQ);

                //rbCASE( EVENT_ROOM_LIST_NOT );
                //rbCASE( EVENT_USER_LIST_NOT );
                //rbCASE( EVENT_INVITE_NOT );
                //rbCASE_WAIT( EVENT_JOIN_ROOM_ACK, EVENT_JOIN_ROOM_REQ );
                rbCASE( EVENT_INFORM_USER_LEAVE_ROOM_NOT );
                _rbCASE( EVENT_HOST_MIGRATED_NOT, DWORD );
                rbCASE_WAIT( EVENT_CREATE_ROOM_ACK, EVENT_CREATE_ROOM_REQ );
                //rbCASE_WAIT( EVENT_END_GAME_ACK, EVENT_END_GAME_REQ );
                rbCASE_WAIT( EVENT_START_GAME_BROAD, EVENT_START_GAME_REQ );
                _rbCASE_WAIT( EVENT_LEAVE_ROOM_ACK, int, EVENT_LEAVE_ROOM_REQ );

                //BUY Item
                rbCASE_WAIT( EVENT_BUY_FOR_GP_ACK, EVENT_BUY_FOR_GP_REQ );
                rbCASE_WAIT( EVENT_BUY_FOR_GEM_ACK, EVENT_BUY_FOR_GEM_REQ );
                rbCASE_WAIT( EVENT_BUY_FOR_CRYSTAL_ACK, EVENT_BUY_FOR_CRYSTAL_REQ );
                //rbCASE_WAIT( EVENT_BUY_COUNT_FOR_GP_ACK, EVENT_BUY_COUNT_FOR_GP_REQ );
                rbCASE_WAIT( EVENT_QUERY_INVENTORY_INFO_ACK, EVENT_QUERY_INVENTORY_INFO_REQ );
                rbCASE_WAIT( EVENT_SELL_ITEM_ACK, EVENT_SELL_ITEM_REQ );
                rbCASE_WAIT( EVENT_STRONG_LEVELUP_ACK, EVENT_STRONG_LEVELUP_REQ );

                // not used in GC yet.
                // rbCASE_WAIT( EVENT_WHERE_ACK, EVENT_WHERE_REQ );
                // rbCASE( EMS_VERIFY_ACCOUNT_ACK );
            }
            m_queRecvMsg.pop();
        }
    }
    if( m_pkMSProxy != NULL)
    {
        KLocker lock( m_pkMSProxy->m_csRecvMsg );
        while ( !m_pkMSProxy->m_queRecvMsg.empty() )
        {
            spEvent = m_pkMSProxy->m_queRecvMsg.front();
            ks.BeginReading( const_cast<KSerBuffer*>( &spEvent->m_kbuff ) );
            
            switch( spEvent->m_usEventID )
            {
                case KMSEvent::EMS_VERIFY_ACCOUNT_ACK:
                {
                    KEMS_VERIFY_ACCOUNT_ACK kPacket__;
                    if( ks.Get(kPacket__) )
                    {
                        ON_EMS_VERIFY_ACCOUNT_ACK(kPacket__, KEvent::FT_INNER );
                    }
                    break;
                }
                
            }
            m_pkMSProxy->m_queRecvMsg.pop();
        }

    }
    if ( m_pkNUserProxy != NULL )
    {
        KLocker lock( m_pkNUserProxy->m_csRecvMsg ); 
        while ( !m_pkNUserProxy->m_queRecvMsg.empty() )
        {
            //====================================================================================
            // 200.10.26 : Asirion
            // ENU Packet은 이것만 핸들링 하면 된다.
            spEvent = m_pkNUserProxy->m_queRecvMsg.front();
            ks.BeginReading( const_cast<KSerBuffer*>( &spEvent->m_kbuff ) );

            switch( spEvent->m_usEventID ) 
            {
                case KNUserEvent::ENU_VERIFY_ACCOUNT_ACK:
                {
                    KENU_VERIFY_ACCOUNT_ACK kPacket__;
                    if( ks.Get(kPacket__) )
                    {
                        ON_ENU_VERIFY_ACCOUNT_ACK( kPacket__, KEvent::FT_INNER );
                    }
                    break;
                }
                case KNUserEvent::ENU_WAIT_TIME_NOT:
                {
                    int temp = 0;
                    ON_ENU_WAIT_TIME_NOT( temp, KEvent::FT_INNER );
                    break;
                }
                case KNUserEvent::ENU_SERVER_LIST_NOT:
                {
                    int temp = 0;
                    ON_ENU_SERVER_LIST_NOT( temp, KEvent::FT_INNER );
                    break;
                }
            }
            m_pkNUserProxy->m_queRecvMsg.pop();
        }
    }
}

template < typename T >
bool KSingleRobot::RobotSendPacket( unsigned short usEventID, T& tData, bool bSetWaiting, bool bCheckWaiting )
 {
    if( m_bOnWaitingAck )
    {
        START_LOG( cwarn, EVENT_TYPE::ms_szEventID[ m_usWaitingID ] << L" 를 기다리는 상태인데 패킷 보내기를 시도함." )
            << BUILD_LOG( EVENT_TYPE::ms_szEventID[ usEventID ] )
            << BUILD_LOG( bCheckWaiting )
            << BUILD_LOG( bSetWaiting ) << END_LOG;

        if( bCheckWaiting )
        {
            return false;
        }
    }

    if( SendPacket( usEventID, tData ) )
    {
        if( bSetWaiting )
        {
            m_bOnWaitingAck = true;
            m_usWaitingID = usEventID;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool KSingleRobot::RobotSendID( unsigned short usEventID, bool bSetWaiting, bool bCheckWaiting )
{
    if( m_bOnWaitingAck )
    {
        START_LOG( cwarn, EVENT_TYPE::ms_szEventID[ m_usWaitingID ] << " 를 기다리는 상태인데 패킷 보내기를 시도함." )
            << BUILD_LOG( EVENT_TYPE::ms_szEventID[ usEventID ] )
            << BUILD_LOGc( bCheckWaiting )
            << BUILD_LOGc( bSetWaiting ) << END_LOG;

        if( bCheckWaiting )
        {
            return false;
        }
    }

    if( SendID( usEventID ) )
    {
        if( bSetWaiting )
        {
            START_LOG( cerr, L"robotsendid  bsetwaiting == true" ) \
                << END_LOG; \
            m_bOnWaitingAck = true;
            m_usWaitingID = usEventID;
        }

        START_LOG( cerr, L"robotsendid sendid(useventid)" ) \
            << END_LOG; \
        return true;
    }
    else
    {
        return false;
    }
}

IMPL_ON_FUNC_NOPARAM( ACTION_CONNECT )
{
    //StateTransition( KUserFSM::INPUT_CONNECT );

    //====================================================================================
    // 2006.10.26 : Asirion
    // CenterServer에 접속 하는 모듈로 변경.
    if ( IsConnected() ) {
        return;
    }
    if ( !m_bFirstRun ) {
        return;
    }

    //VERIFY_STATE(( 1, KUserFSM::STATE_INIT ));
    if( m_pkNUserProxy->IsConnected() ) {
        StateTransition( KUserFSM::INPUT_CONNECT );
        return;
    }

    if( Connect( ms_strRemoteIP.c_str(), ms_usRemoteGameServerPort, true ) == false )//서버 연결에 실패한 경우 
    {
        //if ( m_pkNUserProxy->Connect( NULL, NULL, true ) == true ) {
        //    StateTransition( KUserFSM::INPUT_CONNECT );
        //    KENU_VERIFY_ACCOUNT_REQ kPacket;
        //    kPacket.m_strLogin      = KncUtil::toNarrowString( GetName() );
        //    kPacket.m_strPasswd     = KncUtil::toNarrowString( GetName() );
        //    kPacket.m_bMale         = ( ( rand() % 2 ) == 1 ) ? true : false;
        //    kPacket.m_nProtocolVer  = 2;
        //    kPacket.m_dwAuthType    = 0x00002000; // KSimLayer::AT_CYOU
        //    m_pkNUserProxy->SendPacket( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, kPacket, false, true );
        //}
        
        return;
    }

    //StateTransition( KDefaultFSM::INPUT_CONNECT );

    StateTransition( KUserFSM::INPUT_CONNECT );

    //m_kMyInfo.m_dwUserUID     = GetUID();
    //m_kMyInfo.m_strLogin      = GetName();
    //m_kMyInfo.m_strNickName   = GetName();
    //m_kMyInfo.m_strServerName = L"GameServer";
    //m_kMyInfo.m_bOnline = false;
    //m_kMyInfo.m_bStandby = false;
    //m_kMyInfo.m_strLocation = L"";
    //m_kMyInfo.m_iLocationNum = 0;
    //m_kMyInfo.m_bGamming = false;

    //KEMS_VERIFY_ACCOUNT_REQ kPacket;
    //kPacket.m_dwUserUID     = m_kMyInfo.m_dwUserUID; 
    //kPacket.m_strLogin      = m_kMyInfo.m_strLogin;
    //kPacket.m_strNickName   = m_kMyInfo.m_strNickName;
    //kPacket.m_strPasswd     = TEXT("");//m_kMyInfo.m_strLogin;
    //kPacket.m_strServerName = m_kMyInfo.m_strServerName;
    //kPacket.m_strLocation   = m_kMyInfo.m_strLocation;
    //kPacket.m_iLocationNum  = 0;
    //kPacket.m_bGamming      = false;

    //RobotSendPacket( KMSEvent::EMS_VERIFY_ACCOUNT_REQ, kPacket, true );

    //MessageBox(NULL,TEXT("msgcon"),TEXT("msgcon"),MB_OK);
    /*
    KENU_VERIFY_ACCOUNT_REQ kPacket;
    kPacket.m_strLogin      = KncUtil::toNarrowString( GetName() );
    kPacket.m_strPasswd     = "123456";//KncUtil::toNarrowString( GetName() );
    kPacket.m_bMale         = ( ( rand() % 2 ) == 1 ) ? true : false;
    kPacket.m_nProtocolVer  = 2;
    kPacket.m_dwAuthType    = 0x00002000; // KSimLayer::AT_CYOU
    SendPacket( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, kPacket, false, true );
    //NUser 는 ENU_VERIFY_ACCOUNT_ACK 의 OK 값은 1 이 정상.
    //m_pkNUserProxy->ReserveDestroy();*/

    m_bFirstRun = false;
    //MessageBox(NULL,TEXT("CONNECT"),TEXT("CONNECT"),MB_OK);


    //Sleep( 1000 ); 
}

IMPL_ON_FUNC_NOPARAM( ACTION_MSG_CONNECT )
{   // 070403. Woosh. For Connection to Messenger Server 
    VERIFY_STATE(( 1, KDefaultFSM::STATE_INIT ));
    m_kMyInfo.m_dwUserUID     = GetUID();
    m_kMyInfo.m_strLogin      = GetName();
    m_kMyInfo.m_strNickName   = GetName();
    m_kMyInfo.m_strServerName = L"GameServer";
    m_kMyInfo.m_bOnline = false;
    m_kMyInfo.m_bStandby = false;
    m_kMyInfo.m_strLocation = L"";
    m_kMyInfo.m_iLocationNum = 0;
    m_kMyInfo.m_bGamming = false;
    
    START_LOG( clog, L"MyName : " << m_kMyInfo.m_strLogin ) << END_LOG;

    if ( m_pkMSProxy->IsConnected() ) {
        return;
    }

    if( m_kMyInfo.m_dwUserUID % 2 ) // 두 개의 메신저 서버로 나누기 위한 딱딱한 코딩, lua로 빼자.
    {
        if( m_pkMSProxy->Connect( ms_strMsgRemoteIP.c_str(), ms_usRemoteMsgServerPort, true) == false )
        {
            START_LOG( cerr, L"Msg Server connect failed. ")
                << BUILD_LOG( KncUtil::toWideString( ms_strMsgRemoteIP ) )
                << BUILD_LOG( ms_usRemoteMsgServerPort )
                << BUILD_LOG( m_strName ) << END_LOG;
            return;
        }
    }
    else
    {
        if( m_pkMSProxy->Connect( ms_strMsgRemoteIP2nd.c_str(), ms_usRemoteMsgServerPort2nd, true) == false )
        {
            START_LOG( cerr, L"Msg Server connect failed. ")
                << BUILD_LOG( KncUtil::toWideString( ms_strMsgRemoteIP2nd ) )
                << BUILD_LOG( ms_usRemoteMsgServerPort2nd )
                << BUILD_LOG( m_strName ) << END_LOG;
            return;
        }
    }    
    StateTransition( KDefaultFSM::INPUT_CONNECT );
    KEMS_VERIFY_ACCOUNT_REQ kPacket;
    kPacket.m_dwUserUID     = m_kMyInfo.m_dwUserUID; 
    kPacket.m_strLogin      = m_kMyInfo.m_strLogin;
    kPacket.m_strNickName   = m_kMyInfo.m_strNickName;
    kPacket.m_strPasswd     = L"";
    kPacket.m_strServerName = m_kMyInfo.m_strServerName;
    kPacket.m_strLocation   = m_kMyInfo.m_strLocation;
    kPacket.m_iLocationNum  = 0;
    kPacket.m_bGamming      = false;

    m_pkMSProxy->SendPacket( KMSEvent::EMS_VERIFY_ACCOUNT_REQ, kPacket, false, true );

    MessageBox(NULL,TEXT("msgcon"),TEXT("msgcon"),MB_OK);
    //Sleep(100); // 한꺼번에 몰려서 접속할 때  메신저 서버에서 상태 변경이 안되는 것 같아서 잠깐 쉬기로 함 070406.woosh
}
IMPL_ON_FUNC_NOPARAM( ACTION_WAIT )
{
}

IMPL_ON_FUNC_NOPARAM( ACTION_CHANGE_STATE )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_CONNECTED )); // 실제 클라이언트의 FSM은 여기서 무엇일꼬.. STATE_LOGINED ?
    int iLocation = ( rand() % 5 );

    m_kMyInfo.m_strLocation = m_vecLocation[iLocation];
    
    ON_ACTION_UPDATE_MYDATA( KEvent::FT_INNER );
}
IMPL_ON_FUNC_NOPARAM( ACTION_UPDATE_MYDATA ) // Send 가 더 적절한 이름, 갱신된 나의 정보를(위치) 전송하는 부분. 
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_CONNECTED ));
    KFriendInfo kPacket;
    kPacket = m_kMyInfo; 

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_kMyInfo.m_strLocation ) << END_LOG;

    m_pkMSProxy->SendPacket( KMSEvent::EMS_UPDATE_MYDATA_NOT, kPacket, false, true );    
}
IMPL_ON_FUNC( EMS_VERIFY_ACCOUNT_ACK ) // 
{
    //VERIFY_STATE(( 1, KDefaultFSM::INPUT_CONNECT ));
    START_LOG( clog, "EMS_VERIFY_ACCOUNT_ACK ")
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_mapFriendList.size() ) << END_LOG;
    // 친구 수는 본섭 평균치로 쓰는 것이 실제 유저 환경에 맞다고 생각됨. 070406. woosh
    // 스트레스 테스트라면 100명으로 꽉 채우는 것이 맞음.
    if( kPacket_.m_mapFriendList.size() > 100 )
    {
        int imapIteratorCount = 0;
        KEMS_DELETE_FRIEND_REQ kPacket;
        std::map<DWORD, KFriendInfo>::iterator mit;
        for(mit = kPacket_.m_mapFriendList.begin(); mit != kPacket_.m_mapFriendList.end() ; mit++)
        {
            if( imapIteratorCount >= 100 )
            {
                kPacket.m_dwDeleteUID = mit->second.m_dwUserUID;
                kPacket.m_bBlock = false;
                ON_EMS_DELETE_FRIEND_REQ( kPacket, KEvent::FT_INNER );
            }
            ++imapIteratorCount;
        }
        if( kPacket_.m_mapFriendList.size() < 101 )
        {
            START_LOG( clog, "Delete Friends! Under 100 ")
                << BUILD_LOG( kPacket_.m_mapFriendList.size() ) << END_LOG;
        }
        // 맵 처음부터 100번째까지 아무것도 안하고 돌리고, 100 넘어가면 삭제 요청
        //for( repeat m_mapFriendList.size under 100)
    }
    m_mapFriendList = kPacket_.m_mapFriendList;
    //StateTransition( KUserFSM::INPUT_VERIFICATION_OK );
    ForceStateTransitionTo(KDefaultFSM::STATE_CONNECTED); // 상태를 강제로 바꿈
}
IMPL_ON_FUNC( EMS_DELETE_FRIEND_REQ )
{
    START_LOG_WITH_NAME( clog ) << END_LOG;
    m_pkMSProxy->SendPacket( KMSEvent::EMS_DELETE_FRIEND_REQ, kPacket_ , false, true );    
}
IMPL_ON_FUNC_NOPARAM( ACTION_VERIFY )
{
//    START_LOG( cwarn, GetName() << "(사용자 인증 요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    //VERIFY_STATE(( 1, KUserFSM::STATE_CONNECTED ));
     
    //if ( m_pkNUserProxy->IsConnected() ) {
        //Sleep(2000);
        //m_pkNUserProxy->ReserveDestroy();
    //}
    /*
    if ( IsConnected() || (m_pkNUserProxy->IsConnected())) {
        return;
    }*/

    KEVENT_VERIFY_ACCOUNT_REQ kPacket;
    kPacket.m_strLogin         = KncUtil::toNarrowString( GetName() );
    kPacket.m_strPasswd        = KncUtil::toNarrowString( GetName() );
    kPacket.m_bMale            = ( ( rand() % 2 ) == 1 ) ? true : false;
    kPacket.m_iVersion         = ms_nProtocolVer;
    kPacket.m_iP2PVersion      = 0;
    kPacket.m_dwChecksum       = 0L;
    kPacket.m_dwAuthType       = 0x00002000;
    kPacket.m_nConnectType     = 0;
    kPacket.m_iAge			   = 25;
    //    LANGUAGE_CODE_EU_EN = 0,// -- 유럽 영어
    //    LANGUAGE_CODE_EU_GE = 1,// -- 독일어
    //    LANGUAGE_CODE_KR_KO = 2,// -- 한국어
    //    LANGUAGE_CODE_TW_TW = 3,// -- 대만어
    //    LANGUAGE_CODE_BR_PT = 4,// -- 포루트뇰
    //    LANGUAGE_CODE_US_EN = 5,// -- 미국 영어
    //    LANGUAGE_CODE_PH_EN = 6,// -- 필리핀 영어
    //    LANGUAGE_CODE_TH_TH = 7,// -- 태국어
    //    LANGUAGE_CODE_ID_ID = 8,// -- 인도네시아어
    //    LANGUAGE_CODE_LA_ES = 9,// -- 에스파뇰
    kPacket.m_nLanguageCode		= 2;
    kPacket.m_dwChannellingType	= 2; //임의  2  or  4
    
    
    //SendPacket( KUserEvent::EVENT_VERIFY_ACCOUNT_REQ, kPacket, true );
    RobotSendPacket(KUserEvent::EVENT_VERIFY_ACCOUNT_REQ, kPacket, true );
    //MessageBox(NULL,TEXT("SendPacket"),TEXT("SendPacket"),MB_OK);
        //m_bOnWaitingAck = true;

    StateTransition( KUserFSM::INPUT_VERIFICATION_REQ );
    //MessageBox(NULL,TEXT("VERIFY"),TEXT("VERIFY"),MB_OK);
}

IMPL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK )
{
//    START_LOG( cwarn, GetName() << "(사용자 인증 완료)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;
    if( kPacket_.m_ucOK != 0 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOGc( kPacket_.m_ucOK )
            << BUILD_LOG( NetCommon::CalcIP( kPacket_.m_dwIP ) )
            << END_LOG;
        return;
    }

    //m_pkNUserProxy->ReserveDestroy();
    m_pkMSProxy->ReserveDestroy();
    m_kUserData = kPacket_;

    m_kUserInfo.m_strLogin              = kPacket_.m_strLogin;
    m_kUserInfo.m_dwUserUID             = kPacket_.m_dwUserUID;
    m_kUserInfo.m_strNickName           = kPacket_.m_strNickName;
    m_kUserInfo.m_iPlayerIndex          = 0;
    m_kUserInfo.m_cCharIndex            = 0;
    m_kUserInfo.m_cPromotion            = 0;
    //m_kUserInfo.m_acTagModeInfo[3][2];  // 태그모드에서 선택한 캐릭터/전직 정보.
    m_kUserInfo.m_iTeam                 = 0;
    //m_kUserInfo.m_iRP                   = kPacket_.m_iRP;
    //m_kUserInfo.m_iIndigoWin            = kPacket_.m_iIndigoWin;
    //m_kUserInfo.m_iIndigoLose           = kPacket_.m_iIndigoLose;

    //m_kUserInfo.m_iGetRP                = 0;
    //m_kUserInfo.m_iRPRank               = kPacket_.m_iRPRank;
    
    m_kUserInfo.m_bMale                 = kPacket_.m_bMale;
    m_kUserInfo.m_iAge                  = kPacket_.m_iAge;
    m_kUserInfo.m_cAuthLevel            = kPacket_.m_cAuthLevel;
    m_kUserInfo.m_bIsObserver           = false;
    //m_kUserInfo.m_iGP                   = kPacket_.m_iGamePoint;
    m_kUserInfo.m_cPCBangType           = kPacket_.m_cPCBangType;
    m_kUserInfo.m_mapDifficulty         = kPacket_.m_mapDifficulty;

    //m_kUserInfo.m_iWin                  = kPacket_.m_iWin;
    //m_kUserInfo.m_iLose                 = kPacket_.m_iLose;
    m_kUserInfo.m_bIsHost               = false;
    m_kUserInfo.m_bIsLive               = true;

    m_kUserInfo.m_dwGuildUID               = m_kUserData.m_kGuildUserInfo.m_dwGuildUID;
    if ( m_kUserInfo.m_dwGuildUID > 0 )
    {
        //m_kUserInfo.m_GuildScore            = m_kUserData.m_kGuildInfo.m_iPoint;

        //m_kUserInfo.m_strGuildName          = m_kUserData.m_kGuildInfo.m_strName;
    }

    m_dwLastSendTime  = GetTickCount();
    
    //KPacketNameOK AAAT;
    //AAAT.m_strName = TEXT("atj0000");
    //
    if(m_kUserInfo.m_strNickName == TEXT("")){
        std::wstring kPacketNick = m_kUserInfo.m_strLogin;
        RobotSendPacket(KUserEvent::EVENT_REGISTER_NICKNAME_REQ, kPacketNick, true );
    }

    StateTransition( KUserFSM::INPUT_VERIFICATION_OK );

    if( !m_kUserData.m_bAgreePrivateInfo )
    {
        RobotSendID( KUserEvent::EVENT_AGREEMENT_PRIVATEINFO_NOT, true );//원래false
    }

    RobotSendID( KUserEvent::EVENT_CHANNEL_LIST_REQ, true );//원래false
    {
        //bool bHb = false;
        //RobotSendPacket( KUserEvent::EVENT_SET_HBCHECK_FLAG, bHb, false );
    }
    //m_pKncP2P = new KncP2P();
    //m_pKncP2P->InitKncP2P( m_kUserInfo.m_dwUserUID, KncP2PMessageHandler, 8, ::inet_addr(ms_strRemoteIP.c_str()), 
    //                       m_kUserData.m_usUdpPort );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_kUserData.m_strLogin )
        << BUILD_LOG( NetCommon::CalcIP( kPacket_.m_dwIP ) ) << END_LOG;

    START_LOG( cerr, L"EVENT_VERIFY_ACCOUNT_ACK" ) \
        << END_LOG; \
}

_IMPL_ON_FUNC(EVENT_REGISTER_NICKNAME_ACK, KPacketNameOK){
    if ( kPacket_.m_nOK == 0 ) {   
        m_kUserInfo.m_strNickName = kPacket_.m_strName;

        START_LOG( cerr, L"닉네임이 성공적으로 등록되었습니다." )
            << BUILD_LOG( kPacket_.m_strName )
            << BUILD_LOG( m_kUserInfo.m_strNickName ) << END_LOG;
    }
    else
    {
        START_LOG( cerr, L"닉네임 등록이 실패했습니다. OK값을 확인하세요." )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_.m_strName )
            << END_LOG;
    }

    //  0 : 성공
    // -1 : 아이디와 기존닉네임으로 해당 유저를 찾을 수 없음
    // -2 : 신규닉네임이 이미 사용중임
    // -3 : 닉네임변경아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음
    // -4 : 닉네임 중에 사용할 수 없는 단어나 문자가 있음.
    // -5 : 닉네임 길이가 조건에 맞지 않음.
    // -101이하 : DB처리 실패
}

IMPL_ON_FUNC_NOPARAM( ACTION_CH_LIST )
{
//    START_LOG( clog, "채널 정보 요청" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    RobotSendID( KUserEvent::EVENT_CHANNEL_LIST_REQ, false );
}

//IMPL_ON_FUNC( EVENT_CHANNEL_LIST_NOT )
//{
////    START_LOG( cwarn, GetName() << "(채널 정보 받음)" )
////        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;
//
//    m_vecChannelInfo = kPacket_.m_vecChannelInfo;
//
//    START_LOG( clog, "채널 정보 받음" ) << END_LOG;
//}

IMPL_ON_FUNC_NOPARAM( ACTION_NOTIFY )
{
    // 공지
    VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING ));

    std::wstring kPacket = L"안녕. 나 운영자야.";;

    RobotSendPacket( KUserEvent::EVENT_NOTIFY_REQ, kPacket, false );
}

IMPL_ON_FUNC_NOPARAM( ACTION_CHATTING )
{
    // 일반 채팅
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CONNECTED ));
    KFriendMsgInfo kPacket;
    std::map<DWORD, KFriendInfo>::iterator mit;
    for( mit = m_mapFriendList.begin(); mit != m_mapFriendList.end(); ++mit )
    {
        kPacket.m_dwReceiverUID = mit->second.m_dwUserUID;
        kPacket.m_strMessage = L"밥 먹었냐...........................";
        
        if( rand() % 10 == 0 )
            m_pkMSProxy->SendPacket( KMSEvent::EMS_CHAT_REQ, kPacket, false, true );
    }
    //m_pkMSProxy->SendPacket( KMSEvent::EMS_CHAT_REQ, kPacket, false, true );    
    //RobotSendPacket( KMSEvent::EMS_CHAT_REQ, kPacket, false );
}

//IMPL_ON_FUNC( EVENT_CHAT_NOT )
//{
//    START_LOG( clog, L"채팅중인가 귓말인가 공지인가?..." )
//        << BUILD_LOG( static_cast<int>( kPacket_.m_cMsgType ) )
//        << BUILD_LOG( kPacket_.m_strLogin )
//        << BUILD_LOG( kPacket_.m_strMessage ) << END_LOG;
//}

IMPL_ON_FUNC_NOPARAM( ACTION_WHISPER )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNEL,
                      KUserFSM::STATE_ROOM,
                      KUserFSM::STATE_PLAYING ));
    
    if( m_vecUserInfo.empty() )
    {
        START_LOG_WITH_NAME( clog )
            << BUILD_LOG( "귓속말을 위한 사용자 벡터가 비었음." )
            << END_LOG;
        return;
    }

    KPacketNameMsg kPacket;
    std::random_shuffle( m_vecUserInfo.begin(), m_vecUserInfo.end() );
    kPacket.m_strName      = m_vecUserInfo.begin()->m_strLogin;
    kPacket.m_strMsg       = L"밥 많이 먹었냐";

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket.m_strName )
        << BUILD_LOG( kPacket.m_strMsg ) << END_LOG;

    RobotSendPacket( KUserEvent::EVENT_WHISPER_REQ, kPacket, false );
}

IMPL_ON_FUNC( EVENT_WHISPER_ACK )
{
    //START_LOG( clog, "귓속말 보냄 확인." )
    //    << BUILD_LOG( kPacket_.m_nOK )
    //    << BUILD_LOG( kPacket_.m_strLogin )
    //    << BUILD_LOG( kPacket_.m_strMessage ) << END_LOG;
}

IMPL_ON_FUNC(EVENT_CHANNEL_LIST_ACK){
    KSerializer ks;

    if(kPacket_.m_nOK == 0){
        kPacket_.m_buffCompChannelList.UnCompress();
        ks.BeginReading( &kPacket_.m_buffCompChannelList );
        ks.Get( m_vecChannelInfo );
        ks.EndReading();

        if(checkChannelAck || CommandQue[2] == -1){
            WriteFileClientEnterChannelList();
        }
    }
}

IMPL_ON_FUNC_NOPARAM( ACTION_ENTER_CH )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNELLOBBY ));

    if( m_vecChannelInfo.empty() )
    {
        START_LOG_WITH_NAME( clog )
            << BUILD_LOG( "채널 정보 벡터가 비었음." )
            << END_LOG;
        return;
    }

    if(m_iChannelID == -1){
        std::vector<KChannelInfo> vecTempChannelInfo = m_vecChannelInfo;
        std::random_shuffle( vecTempChannelInfo.begin(), vecTempChannelInfo.end() );
        m_kChannelInfo = *( vecTempChannelInfo.begin() );

        START_LOG_WITH_NAME( clog )
            << BUILD_LOG( m_kChannelInfo.m_strChannelName ) << END_LOG;
    } else {

        for ( int i = 0 ; i < static_cast< int >( m_vecChannelInfo.size() ) ; i++ ){
            if ( m_vecChannelInfo[i].m_dwChannelUID == m_iChannelID ){
                m_kChannelInfo = m_vecChannelInfo[i];
                break;
            }
        }
    }

    RobotSendPacket( KUserEvent::EVENT_ENTER_CHANNEL_REQ,  m_kChannelInfo.m_dwChannelUID , true );
}

_IMPL_ON_FUNC( EVENT_ENTER_CHANNEL_ACK, int )
{
//    START_LOG( cwarn, GetName() << "(ChannelLobby -> Channel)" ) << END_LOG;

    if( kPacket_== 0 )
    {
        StateTransition( KUserFSM::INPUT_TO_CHANNEL );

        RobotSendID( KUserEvent::EVENT_ROOM_LIST_REQ, false );
        RobotSendID( KUserEvent::EVENT_USER_LIST_REQ, false );
        

        //m_pKncP2P->Close();
        //m_pKncP2P->InitKncP2P( m_kUserInfo.m_dwUserUID, KncP2PMessageHandler, 8, ::inet_addr(ms_strRemoteIP.c_str()), 
        //                       m_kUserData.m_usUdpPort );
    }
    START_LOG( cerr, L"enter channel ack" ) \
        << END_LOG; \
}

IMPL_ON_FUNC_NOPARAM( ACTION_ROOM_LIST )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNEL ));

    RobotSendID( KUserEvent::EVENT_ROOM_LIST_REQ, false );
}

IMPL_ON_FUNC_NOPARAM( ACTION_USER_LIST )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    RobotSendID( KUserEvent::EVENT_USER_LIST_REQ, false );
}

IMPL_ON_FUNC_NOPARAM( ACTION_LEAVE_CH )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNEL ));

    if( RobotSendID( KUserEvent::EVENT_LEAVE_CHANNEL_NOT, false ) )
    {
        StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
        RobotSendID( KUserEvent::EVENT_CHANNEL_LIST_REQ, false );
    }
}

IMPL_ON_FUNC_NOPARAM( ACTION_JOIN_ROOM )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_kRoomInfo.m_usRoomID )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNEL ));

    if( m_vecRoomInfo.empty() )
    {
        START_LOG_WITH_NAME( clog )
            << BUILD_LOG( "아직 방 정보 벡터가 비어있음." )
            << END_LOG;
        return;
    }

    KJoinRoomReqInfo kPacket;
    std::vector<KSimpleRoomInfo> vecTempRoomInfo = m_vecRoomInfo;
    std::random_shuffle( vecTempRoomInfo.begin(), vecTempRoomInfo.end() );
    KSimpleRoomInfo* pkSimpleRoomInfo = NULL;
    pkSimpleRoomInfo = (&(*( vecTempRoomInfo.begin() )));


    m_kRoomInfo.m_usRoomID          = pkSimpleRoomInfo->m_usRoomID;
    m_kRoomInfo.m_strRoomName       = pkSimpleRoomInfo->m_strRoomName;
    m_kRoomInfo.m_bPublic           = pkSimpleRoomInfo->m_bPublic;
    m_kRoomInfo.m_bGuild            = pkSimpleRoomInfo->m_bGuild;
    m_kRoomInfo.m_strRoomPasswd     = pkSimpleRoomInfo->m_strRoomPasswd;
    m_kRoomInfo.m_usUsers           = pkSimpleRoomInfo->m_usUsers;
    m_kRoomInfo.m_usMaxUsers        = pkSimpleRoomInfo->m_usMaxUsers;
    m_kRoomInfo.m_bPlaying          = pkSimpleRoomInfo->m_bPlaying;
    m_kRoomInfo.m_cGrade            = pkSimpleRoomInfo->m_cGrade;
    m_kRoomInfo.m_cGameCategory     = pkSimpleRoomInfo->m_cGameCategory;
    m_kRoomInfo.m_iP2PVersion       = 0;

    kPacket.m_usRoomID = m_kRoomInfo.m_usRoomID;
    kPacket.m_strRoomPasswd = m_kRoomInfo.m_strRoomPasswd;
    kPacket.m_uiJoinType = KJoinRoomReqInfo::JOIN_NORMAL;

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket.m_usRoomID )
        << BUILD_LOG( kPacket.m_strRoomPasswd ) << END_LOG;

    RobotSendPacket( KUserEvent::EVENT_JOIN_ROOM_REQ, kPacket, true );
}

_IMPL_ON_FUNC( EVENT_JOIN_ROOM_ACK, KJoinRoomAckInfo )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_kRoomInfo.m_usRoomID )
        << END_LOG;
        //<< L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) ) << END_LOG;

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        START_LOG_WITH_NAME( clog )
            << BUILD_LOG( static_cast<int>( kPacket_.m_nOK ) )
            << END_LOG;
        return;
    }

    StateTransition( KUserFSM::INPUT_JOIN_ROOM );
    START_LOG_WITH_NAME( clog ) << BUILD_LOG( "ConnectP2P" ) << END_LOG;

    m_kUserInfo.m_bIsHost = false;

    //m_pKncP2P->ResetConnectQ();
    //m_pKncP2P->SetupRelayServer( kPacket_.m_kRoomInfo.m_dwRelayServerIP,
    //                             kPacket_.m_kRoomInfo.m_usRelayServerPort );

    KChangeRoomUserInfo kPacket;
    kPacket.m_strLogin      = m_kUserInfo.m_strLogin;
    //kPacket.m_ucChangeType  = KChangeRoomUserInfo::RUI_READY;
    RobotSendPacket( KUserEvent::EVENT_CHANGE_ROOMUSER_INFO_REQ, kPacket, false );
}

void KSingleRobot::SetRoomMode(int RoomMode){//룸모드 지정
    m_iRoomMode = RoomMode;
}

IMPL_ON_FUNC_NOPARAM( ACTION_CREATE_ROOM )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNEL ));

    KEVENT_CREATE_ROOM_REQ kPacket;

    KRoomInfo kRoomInfo;
    kRoomInfo.m_strRoomName     = m_kUserInfo.m_strLogin ;
    kRoomInfo.m_strRoomPasswd   = m_kUserInfo.m_strLogin ;
    kRoomInfo.m_bPublic         = 0;
    kRoomInfo.m_usMaxUsers      = 6;
    //kRoomInfo.m_cDifficulty     = 0;
    kRoomInfo.m_iP2PVersion     = 0;

    if(m_iRoomMode != -1){//게임 모드를 지정해주는 경우
        kRoomInfo.m_cGameCategory   = m_iRoomMode;
        kRoomInfo.m_iGameMode       = m_iRoomMode;
        kRoomInfo.m_iMapID          = m_iRoomMode;
        kRoomInfo.m_iSubGameMode    = m_iRoomMode;
    }else{//지정해주지 않을경우
        if(m_kChannelInfo.m_dwChannelUID == 1){//대전
            kRoomInfo.m_cGameCategory   = GC_GM_TEAM_MODE;
            kRoomInfo.m_iGameMode       = GC_GM_TEAM_MODE;
            kRoomInfo.m_iMapID          = GC_GM_TEAM_MODE;
            kRoomInfo.m_iSubGameMode    = GC_GM_TEAM_MODE;
        }else{//던전
            kRoomInfo.m_cGameCategory   = GC_GM_QUEST0;
            kRoomInfo.m_iGameMode       = GC_GM_QUEST0;
            kRoomInfo.m_iMapID          = GC_GM_QUEST0;
            kRoomInfo.m_iSubGameMode    = GC_GM_QUEST0;
        }
    }

    m_kUserInfo.m_vecIP.clear();
    //m_kUserInfo.m_vecIP         = m_pKncP2P->GetLocalIP();
    //if( std::find( m_kUserInfo.m_vecIP.begin(), m_kUserInfo.m_vecIP.end(), m_kUserData.m_dwIP ) == m_kUserInfo.m_vecIP.end() )
    //{
    //    m_kUserInfo.m_vecIP.push_back( m_kUserData.m_dwIP );
    //}


    //m_kUserInfo.m_vecPort   = m_pKncP2P->GetLocalPort();
    m_kRoomInfo = kPacket.m_kRoomInfo = kRoomInfo;
    kPacket.m_kInDoorUserInfo= m_kUserInfo;

    RobotSendPacket( KUserEvent::EVENT_CREATE_ROOM_REQ, kPacket, true );
}

IMPL_ON_FUNC( EVENT_CREATE_ROOM_ACK )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    if( kPacket_.m_nOK != 0 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( static_cast<int>( kPacket_.m_nOK ) )
            << END_LOG;
        return;
    }

    m_kRoomInfo = kPacket_.m_kRoomInfo;
    m_kUserInfo.m_bIsHost = true;

    StateTransition( KUserFSM::INPUT_JOIN_ROOM );

    //m_pKncP2P->ResetConnectQ();
    //m_pKncP2P->SetupRelayServer( kPacket_.m_kRoomInfo.m_dwRelayServerIP,
    //                             kPacket_.m_kRoomInfo.m_usRelayServerPort );
}


IMPL_ON_FUNC_NOPARAM( ACTION_INVITE )
{
    //====================================================================================
    // 2006.10.10 : Asirion
    // 방장 외에는 초대를 할 수 없다.
    if ( m_kUserInfo.m_bIsHost == false )
        return;

    VERIFY_STATE(( 1, KUserFSM::STATE_ROOM ));
    if( m_kChannelInfo.m_cType == 1 )
    {
        // 추후 추가.
        START_LOG_WITH_NAME( cerr ) << END_LOG;
    }
    else
    {
        std::vector<DWORD> kPacket;
        std::vector<KUserInfo> vecTempUserInfo = m_vecUserInfo;
        std::random_shuffle( vecTempUserInfo.begin(), vecTempUserInfo.end() );
        std::vector<KUserInfo>::iterator itUserList;
        for( itUserList = vecTempUserInfo.begin(); itUserList != vecTempUserInfo.end(); itUserList++ )
        {
            if( itUserList->m_cState == -1 )
            {
                kPacket.push_back( itUserList->m_dwUserUID );
                //kPacket.push_back( itUserList->m_strLogin );
                break;
            }
        }

        for( ; itUserList != vecTempUserInfo.end(); itUserList++ )
        {
            if( itUserList->m_cState == -1 && GAMBLE( 0.1 ) )
            {
                kPacket.push_back( itUserList->m_dwUserUID );
                //kPacket.m_vecLogin.push_back( itUserList->m_strLogin );
            }
        }

        RobotSendPacket( KUserEvent::EVENT_INVITE_REQ, kPacket, false );
    }
}

//_IMPL_ON_FUNC( EVENT_INVITE_NOT, KSerBuffer )
//{
//    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNEL ));
//
//    KEVENT_JOIN_ROOM_REQ kPacket;
//    kPacket.m_usRoomID = kPacket_.m_usRoomID;
//    kPacket.m_strRoomPasswd = kPacket_.m_strRoomPasswd;
//
//    START_LOG_WITH_NAME( clog )
//        << BUILD_LOG( kPacket_.m_strLogin )
//        << BUILD_LOG( GetName() )
//        << BUILD_LOG( kPacket.m_usRoomID )
//        << BUILD_LOG( kPacket.m_strRoomPasswd ) << END_LOG;
//
//    RobotSendPacket( KUserEvent::EVENT_JOIN_ROOM_REQ, kPacket, true );
//}

IMPL_ON_FUNC_NOPARAM( ACTION_LEAVE_ROOM )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    //VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING )); - 상태오류 발생으로 주석처리 2013 . 8. 13

    KEVENT_LEAVE_ROOM_REQ kPacket;
    kPacket.m_bSubtractRP   = false;
    kPacket.m_iP2PError     = KFailRate::P2P_NO_ERROR;
    RobotSendPacket( KUserEvent::EVENT_LEAVE_ROOM_REQ, kPacket, true );
}

_IMPL_ON_FUNC( EVENT_LEAVE_ROOM_ACK, int )
{
    if ( kPacket_ == 0 )
    {
        StateTransition( KUserFSM::INPUT_TO_CHANNEL );
    }
    else
    {
        START_LOG_WITH_NAME( cerr ) << L"EVENT_LEAVE_ROOM_ACK에서 실패" << kPacket_  
            << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;
    }
}

IMPL_ON_FUNC( EVENT_INFORM_USER_LEAVE_ROOM_NOT )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOG( kPacket_.m_nReasonWhy )
        << BUILD_LOG( m_kUserInfo.m_bIsHost ) << END_LOG;

    if( GetName() == kPacket_.m_strName )
    {
        StateTransition( KUserFSM::INPUT_TO_CHANNEL );
        //m_pKncP2P->Close();
        //m_pKncP2P->InitKncP2P( m_kUserInfo.m_dwUserUID, KncP2PMessageHandler, 8, ::inet_addr(ms_strRemoteIP.c_str()), 
        //                       m_kUserData.m_usUdpPort );
    }
    else
    {
        //====================================================================================
        // 2006.10.11 : Asirion
        // 현재 커넥션을 맺지 않는다. 그러므로 Disconnect도 필요 없다.
        //m_pKncP2P->DisConnectPeer( kPacket_.m_dwUserUID );
    }
}

_IMPL_ON_FUNC( EVENT_HOST_MIGRATED_NOT, DWORD )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( m_kUserInfo.m_bIsHost ) << END_LOG;

    if( GetUID() == kPacket_ )
    {       
        m_kUserInfo.m_bIsHost = true;
        START_LOG_WITH_NAME( cwarn ) << BUILD_LOG( L"현재 Host에게 EVENT_HOST_MIGRATED_NOT가 들어옴!" ) << END_LOG;
    }
    else
    {
        m_kUserInfo.m_bIsHost = false;
    }
}

IMPL_ON_FUNC( EVENT_START_GAME_BROAD )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;
    if( kPacket_.m_nOK == 0 )
    {

        KEVENT_LOAD_COMPLETE_NOT kPacket;
        kPacket.m_iP2PUser = 0;
        kPacket.m_iRelayUser = 0;
        RobotSendPacket( KUserEvent::EVENT_LOAD_COMPLETE_NOT, kPacket, false );
        StateTransition( KUserFSM::INPUT_START_GAME_OK );
    }
}

IMPL_ON_FUNC_NOPARAM( ACTION_START_GAME )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    VERIFY_STATE(( 1, KUserFSM::STATE_ROOM ));

    if( m_kUserInfo.m_bIsHost == false )
    {
        START_LOG(clog, L"방장이 아닌데 게임 시작 시도" )
            << BUILD_LOG( m_kUserInfo.m_strLogin )
            << END_LOG;

        return;
    }

    KStartGameReq kPacket;
    //kPacket.m_iGameMode = GC_GM_QUEST1;
    //kPacket.m_cSelChar  = 0;

    START_LOG( clog, L"게임 시작 모드 : "  ) << END_LOG;

    RobotSendPacket( KUserEvent::EVENT_START_GAME_REQ, kPacket, true );
}

IMPL_ON_FUNC_NOPARAM( ACTION_USE_ITEM )
{
    //KEVENT_USE_DURATION_ITEM_REQ kPacket;

    //std::map<char, KCharacterInfo>::const_iterator mit;
    //for(mit = m_kUserData.m_mapCharacterInfo.begin(); mit != m_kUserData.m_mapCharacterInfo.end(); mit++)
    //{
    //    std::vector<int>::const_iterator vit;
    //    for(vit = mit->second.m_vecEquippedItems.begin(); vit != mit->second.m_vecEquippedItems.end(); vit++)
    //    {
    //        if( *vit < 0 )
    //        {
    //            continue;
    //        }

    //        KDurationItemInfo kDurationItemInfo;
    //        kDurationItemInfo.m_dwGoodsID = *vit;

    //        std::vector<KDurationItemInfo>::const_iterator  vit2;
    //        vit2 = std::find( m_kUserData.m_vecDurationItemInv.begin(), m_kUserData.m_vecDurationItemInv.end(), kDurationItemInfo );
    //        if( vit2 != m_kUserData.m_vecDurationItemInv.end() )
    //        {
    //            kPacket.m_vecDurationUIDs.push_back( vit2->m_dwDurationItemUID );

    //            RobotSendPacket( KUserEvent::EVENT_USE_DURATION_ITEM_REQ, kPacket, false );
    //            return;
    //        }
    //    }
    //}
}

IMPL_ON_FUNC_NOPARAM( ACTION_END_GAME )
{

    if( ::GetTickCount() - m_dwLatestActionTick < 5000 )
    {
        return;
    }

    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << (LPCTSTR)CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;

    if( m_kUserInfo.m_bIsHost == false )
    {
        START_LOG( cwarn, L"방장이 아닌데 게임 종료 시도" )
            << BUILD_LOG( m_strName )
            << END_LOG;
        return;
    }

    VERIFY_STATE(( 1, KUserFSM::STATE_PLAYING ));

    KEndGameReq kPacket;

    KGameResultIn kGameResult;
    kGameResult.m_strLogin = m_strName;
    //kGameResult.m_nExp = rand() % 15;
    //kGameResult.m_nGamePoint = rand() % 10;
    kGameResult.m_iNumKills = rand() % 6;   
    kGameResult.m_iNumDies = rand() % 3;
    kGameResult.m_iContPoint = 0;
    kGameResult.m_bWin = true;
    kGameResult.m_nScore = rand() % 20;

    kPacket.m_vecGameResult.push_back( kGameResult );

    RobotSendPacket( KUserEvent::EVENT_END_GAME_REQ, kPacket, true );
}

_IMPL_ON_FUNC( EVENT_END_GAME_ACK, KEndGameAck )
{
    START_LOG_WITH_NAME( cwarn )
        << L"발생 시간 : " << CTime::GetCurrentTime().Format(KNC_TIME_FORMAT) << END_LOG;
    StateTransition( KUserFSM::INPUT_END_GAME_OK );
}

//BUY Item

IMPL_ON_FUNC_NOPARAM( ACTION_BUY_FOR_GP )
{
//    START_LOG( cwarn, GetName() << "(GP아이템 구매요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    KEVENT_BUY_FOR_GP_REQ   kPacket;

    static int siItemAvaiableForGP[] =
    {
        390,  //평범한 망또
        1710, //로마기사 복장세트
        8880, //GEM
        8980, //바람기사 목걸이(0레벨)
        9080, //바람궁수 목걸이(0레벨)
        9180, //바람법사 목걸이(0레벨)
        9760, //바람기사 발찌(0레벨)
        9860, //바람궁수 발찌(0레벨)
        9960, //바람법사 발찌(0레벨)
    };

    kPacket.m_ItemID     = siItemAvaiableForGP[rand() % 9];
    kPacket.m_nDuration    = 1;
    kPacket.m_nPeriod	 = 1;

    RobotSendPacket( KUserEvent::EVENT_BUY_FOR_GP_REQ, kPacket, true );
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GP_ACK )
{
//    START_LOG( cwarn, GetName() << "(GP아이템 구매완료)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") )
//        << BUILD_LOG( static_cast<int>(kPacket_.m_cOK) ); << END_LOG;

    if(kPacket_.m_nOK == 0)
    {
        //LIF( InsertIntoUserInventory( kPacket_.m_vecItem, kPacket_.m_vecDurationItem ) );

        //m_kUserData.m_iGamePoint    = kPacket_.m_dwGP;

        START_LOG( clog, "GP 아이템 구매 완료" )
            << BUILD_LOG( static_cast<int>(kPacket_.m_nOK) ) << END_LOG;
            
    }
}

IMPL_ON_FUNC_NOPARAM( ACTION_BUY_FOR_GEM )
{
//    START_LOG( cwarn, GetName() << "(GEM아이템 구매 요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    KEVENT_BUY_FOR_GEM_REQ kPacket;

    static int siItemAvaiableForGEM[] = 
    {
        8890, //1단계 마법서
        8900, //2단계 마법서
        8910, //3단계 마법서
        8920, //4단계 마법서
        8930, //5단계 마법서
        8940, //6단계 마법서
        8950, //7단계 마법서
        8960, //8단계 마법서
        8970, //9단계 마법서
    };

//    kPacket.m_dwGoodsID     = siItemAvaiableForGEM[rand() % 9];

    RobotSendPacket( KUserEvent::EVENT_BUY_FOR_GEM_REQ, kPacket, true );
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GEM_ACK )
{
//    START_LOG( cwarn, GetName() << "(GEM아이템 구매완료)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") )
//        << BUILD_LOG( static_cast<int>(kPacket_.m_cOK) ) << END_LOG;

    if(kPacket_.m_nOK == 0)
    {
//        LIF( InsertIntoUserInventory( kPacket_.m_vecItem, kPacket_.m_vecDurationItem ) );

        START_LOG( clog, "Gem 아이템 구매 완료" )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
            //<< BUILD_LOG( kPacket_.m_dwGem ) << END_LOG;
    }
}

IMPL_ON_FUNC_NOPARAM( ACTION_BUY_FOR_CRYSTAL )
{
//    START_LOG( cwarn, GetName() << "(Crystal아이템 구매 요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    KEVENT_BUY_FOR_GEM_REQ kPacket;

    static int siItemAvaiableForCrystal[] = 
    {
        7700, //고르고스 검(200회)
        7710, //고르고스 활(200회)
        7720, //고르고스 지팡이(200회)
        7730, //고르고스 창(200회)
        7740, //고르고스 석궁(200회)
        7750, //고르고스 항아리(200회)
    };

//    kPacket.m_dwGoodsID     = siItemAvaiableForCrystal[rand() % 6];
//    kPacket.m_iDuration     = 200;
    RobotSendPacket( KUserEvent::EVENT_BUY_FOR_CRYSTAL_REQ, kPacket, true );
}

IMPL_ON_FUNC( EVENT_BUY_FOR_CRYSTAL_ACK )
{
    if(kPacket_.m_nOK == 0)
    {
//        LIF( InsertIntoUserInventory( kPacket_.m_vecItem, kPacket_.m_vecDurationItem ) );

        START_LOG( clog, "CRYSTAL로 아이템 구매 완료" )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
    }
}

//IMPL_ON_FUNC_NOPARAM( ACTION_BUY_COUNT_FOR_GP )
//{
//    START_LOG( cwarn, GetName() << "(GP아이템 수량지정 구매요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

//    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    //KEVENT_BUY_COUNT_FOR_GP_REQ   kPacket;

    //kPacket.m_dwGoodsID     = 8880; //현재는 GEM만 수량지정 가능
    //kPacket.m_iCount        = (rand() % 10) + 1; //1~10개

    //RobotSendPacket( KUserEvent::EVENT_BUY_COUNT_FOR_GP_REQ, kPacket, true );
//}

//IMPL_ON_FUNC( EVENT_BUY_COUNT_FOR_GP_ACK )
//{
////    START_LOG( cwarn, GetName() << "(GP아이템 수량지정 구매완료)" )
////        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") )
////        << BUILD_LOG( static_cast<int>(kPacket_.m_cOK) ) << END_LOG;
//
//    if(kPacket_.m_cOK == 0)
//    {
//        LIF( InsertIntoUserInventory( kPacket_.m_vecItem, kPacket_.m_vecDurationItem ) );
// 
//        m_kUserData.m_iGamePoint    = kPacket_.m_dwGP;
//
//        START_LOG( clog, "GP로 수량지정 아이템 구매 완료" )
//            << BUILD_LOG( static_cast<int>(kPacket_.m_cOK) )
//            << BUILD_LOG( kPacket_.m_dwGP ) << END_LOG;
//    }
//}

IMPL_ON_FUNC_NOPARAM( ACTION_QUERY_INVENTORY )
{
//    START_LOG( cwarn, GetName() << "(인벤토리 정보 요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    RobotSendID( KUserEvent::EVENT_QUERY_INVENTORY_INFO_REQ, true );
}

IMPL_ON_FUNC( EVENT_QUERY_INVENTORY_INFO_ACK )
{
//    START_LOG( cwarn, GetName() << "(인벤토리 정보 받음)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    //m_kUserData.m_vecInv.clear();
    //m_kUserData.m_vecDurationItemInv.clear();

    //m_kUserData.m_vecInv = kPacket_.m_vecInv;
    //m_kUserData.m_vecDurationItemInv = kPacket_.m_vecDurationItemInv;
}

//IMPL_ON_FUNC_NOPARAM( ACTION_EQUIP_ITEM )
//{
////    START_LOG( cwarn, GetName() << "(아이템 장착 요청)" )
////        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;
//
//    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
//
//    KEquipItemInfo kPacket;
//    
//    kPacket.m_strLogin = m_kUserInfo.m_strLogin;
//    kPacket.m_cCharIndex = rand() % 3;
//    std::map<char, KCharacterInfo>::const_iterator mit;
//    for ( int i = 0; i < m_kUserData.m_mapCharacterInfo.size(); i++ )
//    {
//        std::vector<KEquipElement> vecInDoorItemInfo;
//        for( int j = 0; j < m_kUserData.m_mapCharacterInfo[i].m_vecEquippedItems.size(); j++ )
//        {
//            KEquipElement kEquipElement;
//            kEquipElement.m_dwID  = m_kUserData.m_mapCharacterInfo[i].m_vecEquippedItems[j];
//            kEquipElement.m_dwUID = 0;
//            vecInDoorItemInfo.push_back( kEquipElement );
//        }
//        kPacket.m_vecEquipInfo
//        std::pair<char, std::vector<KEquipElement> > kEquipInfo;
//    }
//    for(mit = m_kUserData.m_mapCharacterInfo.begin(); mit != m_kUserData.m_mapCharacterInfo.end(); mit++)
//    {
//        kPacket.m_mapEquipItems[mit->second.m_cCharType] = mit->second.m_vecEquippedItems;
//    }
//
//    RobotSendPacket( KUserEvent::EVENT_EQUIP_ITEM_REQ, kPacket, false );
//}

IMPL_ON_FUNC_NOPARAM( ACTION_SELL_ITEM )
{
//    START_LOG( cwarn, GetName() << "(아이템 판매 요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KSimpleItem    kPacket;
    kPacket.m_dwID = 0;
    kPacket.m_dwUID = 0;

    //if( m_kUserData.m_vecInv.empty() && m_kUserData.m_vecDurationItemInv.empty() )
    //{
    //    START_LOG( clog, "가지고 있는 아이템이 없음." ) << END_LOG;
    //    m_bOnWaitingAck = false;
    //    return;
    //}

    //unsigned int iGamble = rand() % ( m_kUserData.m_vecInv.size() + m_kUserData.m_vecDurationItemInv.size() );

    //if( iGamble < m_kUserData.m_vecInv.size() )
    //{
    //    kPacket.m_dwGoodsUID = m_kUserData.m_vecInv[iGamble].m_dwUID;
    //    kPacket.m_dwGoodsID = m_kUserData.m_vecInv[iGamble].m_dwID;
    //    //050525. microcat. GS에서 client로 판매한 아이템 목록을 보내주지 않으므로 여기서 지워줘야 한다?
    //    std::vector<KItemInfo>::iterator    vit;
    //    for(vit = m_kUserData.m_vecInv.begin(); vit != m_kUserData.m_vecInv.end(); vit++)
    //    {
    //        if( kPacket.m_dwGoodsUID == vit->m_dwUID && kPacket.m_dwGoodsID == vit->m_dwID )
    //            break;
    //    }
    //    m_kUserData.m_vecInv.erase( vit );
    //}
    //else
    //{
    //    iGamble -= m_kUserData.m_vecInv.size();
    //    kPacket.m_dwGoodsUID = m_kUserData.m_vecDurationItemInv[iGamble].m_dwDurationItemUID;
    //    kPacket.m_dwGoodsID = m_kUserData.m_vecDurationItemInv[iGamble].m_dwGoodsID;
    //    //050525. microcat. GS에서 client로 판매한 아이템 목록을 보내주지 않으므로 여기서 지워줘야 한다?
    //    std::vector<KDurationItemInfo>::iterator    vdit;
    //    for(vdit = m_kUserData.m_vecDurationItemInv.begin(); vdit != m_kUserData.m_vecDurationItemInv.end(); vdit++)
    //    {
    //        if( kPacket.m_dwGoodsUID == vdit->m_dwDurationItemUID && kPacket.m_dwGoodsID == vdit->m_dwGoodsID )
    //            break;
    //    }
    //    m_kUserData.m_vecDurationItemInv.erase( vdit );
    //}

    RobotSendPacket( KUserEvent::EVENT_SELL_ITEM_REQ, kPacket, true );
}

IMPL_ON_FUNC( EVENT_SELL_ITEM_ACK )
{
//    START_LOG( cwarn, GetName() << "(아이템 판매 완료)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    if(kPacket_.m_nOK == 0)
    {
//        LIF( InsertIntoUserDurationInventory( kPacket_.m_vecDurationItem ) );

//        m_kUserData.m_iGamePoint    = kPacket_.m_dwGP;
    }
}

IMPL_ON_FUNC_NOPARAM( ACTION_STRONG )
{
//    START_LOG( cwarn, GetName() << "(아이템 강화 요청)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") ) << END_LOG;

    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KEVENT_STRONG_LEVELUP_REQ   kPacket;

    static int siItemAvaiableForStrong[] =
    {
        8980, //바람기사 목걸이(0레벨)
        9080, //바람궁수 목걸이(0레벨)
        9180, //바람법사 목걸이(0레벨)
        9760, //바람기사 발찌(0레벨)
        9860, //바람궁수 발찌(0레벨)
        9960, //바람법사 발찌(0레벨)
    };

    //std::vector<KItemInfo>::iterator    vit;

    kPacket.m_dwGoodsID = 0;
    kPacket.m_dwGoodsUID = 0;

    //for(vit = m_kUserData.m_vecInv.begin(); vit != m_kUserData.m_vecInv.end(); vit++)
    //{
    //    for(int i = 0; i < 6; i++)
    //    {
    //        if( vit->m_dwID == siItemAvaiableForStrong[i] )
    //        {
    //            kPacket.m_dwGoodsID = vit->m_dwID;
    //            kPacket.m_dwGoodsUID = vit->m_dwUID;
    //        }
    //    }
    //}

    if( kPacket.m_dwGoodsID == 0 && kPacket.m_dwGoodsUID == 0 )
    {
        START_LOG( clog, "강화할 아이템이 없습니다." )
            << END_LOG;
    }

    RobotSendPacket( KUserEvent::EVENT_STRONG_LEVELUP_REQ, kPacket, true );
}

IMPL_ON_FUNC( EVENT_STRONG_LEVELUP_ACK )
{
//    START_LOG( cwarn, GetName() << "(아이템 강화 완료)" )
//        << BUILD_LOG( CTime::GetCurrentTime().Format("%H:%M:%S") )
//        << BUILD_LOG( static_cast<int>( kPacket_.m_cOK ) ) << END_LOG;

 /*   LIF( InsertIntoUserInventory( kPacket_.m_vecAddItem, kPacket_.m_vecAddDurationItem ) );

    std::vector<KItemInfo>::iterator      vit;
    for(vit = kPacket_.m_vecDelItem.begin(); vit != kPacket_.m_vecDelItem.end(); vit++)
    {
        std::vector<KItemInfo>::iterator    vit2;
        for( vit2 = m_kUserData.m_vecInv.begin(); vit2 != m_kUserData.m_vecInv.end(); vit2++)
        {
            if( vit2->m_dwID == vit->m_dwID &&
                vit2->m_dwUID == vit->m_dwUID )
            {
                m_kUserData.m_vecInv.erase( vit2 );
                break;
            }
        }
    }

    std::vector<KDurationItemInfo>::iterator    vdit;
    for(vdit = kPacket_.m_vecDelDurationItem.begin(); vdit != kPacket_.m_vecDelDurationItem.end(); vdit++)
    {
        std::vector<KDurationItemInfo>::const_iterator    vdit2;
        for(vdit2 = m_kUserData.m_vecDurationItemInv.begin(); vdit2 != m_kUserData.m_vecDurationItemInv.end(); vdit2++)
        {
            if( vdit->m_dwDurationItemUID == vdit2->m_dwDurationItemUID )
            {
                break;
            }
        }

        if( vdit2 != m_kUserData.m_vecDurationItemInv.end() )
        {
            vdit->m_dwDurationNum = vdit2->m_dwDurationNum;
        }
    }*/
}

IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_ACK )
{
    StateTransition( KUserFSM::INPUT_CONNECT );
}
_IMPL_ON_FUNC( ENU_WAIT_TIME_NOT, int )
{
}
_IMPL_ON_FUNC( ENU_SERVER_LIST_NOT, int )
{
    m_bConnectToCenter = true;
}

IMPL_ON_FUNC_NOPARAM( ACTION_DISCONNECT )
{
    ReserveDestroy();
    if ( m_pkNUserProxy ) {
        m_pkNUserProxy->ReserveDestroy();
        

    }

    if(m_countCommand == CommandQue.size()){//명령어를 모두 마치고 하는 disconnect가 진짜 disconnect

        m_bcheckDisconnect = true;
    }
}
/*
bool KSingleRobot::InsertIntoUserInventory( IN std::vector<KItemInfo>& kItemInfo,
                              IN std::vector<KDurationItemInfo>& kDurationItemInfo )
{
    int iInsertkItemInfoCount = 0;

    std::vector<KItemInfo>::const_iterator      vit;
    for(vit = kItemInfo.begin(); vit != kItemInfo.end(); vit++)
    {
        m_kUserData.m_vecInv.push_back( *vit );
        iInsertkItemInfoCount++;
    }

    LIF( InsertIntoUserDurationInventory( kDurationItemInfo ) );

    return ( kItemInfo.size() == iInsertkItemInfoCount );
    return true;
}
*/
bool KSingleRobot::InsertIntoUserDurationInventory( IN std::vector<KDurationItemInfo>& kDurationItemInfo )
{
    //int iInsertkDurationItemInfoCount = 0;

    //std::vector<KDurationItemInfo>::const_iterator  vdit;
    //for(vdit = kDurationItemInfo.begin(); vdit != kDurationItemInfo.end(); vdit++)
    //{
    //    std::vector<KDurationItemInfo>::iterator    vdit2;
    //    for(vdit2 = m_kUserData.m_vecDurationItemInv.begin(); vdit2 != m_kUserData.m_vecDurationItemInv.end(); vdit2++)
    //    {
    //        if( vdit->m_dwDurationItemUID == vdit2->m_dwDurationItemUID )
    //        {
    //            break;
    //        }
    //    }

    //    if( vdit2 != m_kUserData.m_vecDurationItemInv.end() )
    //    {
    //        vdit2->m_dwDurationNum = vdit->m_dwDurationNum;
    //        iInsertkDurationItemInfoCount++;
    //    }
    //    else
    //    {
    //        m_kUserData.m_vecDurationItemInv.push_back( *vdit );
    //        iInsertkDurationItemInfoCount++;
    //    }
    //}

    //return ( kDurationItemInfo.size() == iInsertkDurationItemInfoCount );
    return true;
}

void KSingleRobot::SetRobotID( int iSingleRobotID_ )
{
   m_iSingleRobotID = iSingleRobotID_;
}


bool WINAPI KncP2PMessageHandler( DWORD dwMessageID, PVOID pMsgBuffer/*, int iSingleRobotID_ */)
{   
    //KSingleRobot* pkSingleRobot = SiKMassiveRobot()->GetInstance()->GetSingleRobot( iSingleRobotID_ );
    //switch( dwMessageID )
    //{
    //}
    return true;
}
