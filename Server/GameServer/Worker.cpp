#include "GSSimLayer.h" // #include <WinSock2.h>
#include "Worker.h"
#include "UserPacket.h"
#include <dbg/dbg.hpp>
#include "ThreadManager.h"
#include <KncUtil.h>
//#include "StatAgent.h"
#include "LogManager.h"
#include "Log4.h"

#define CLASS_TYPE KWorker

KWorker::KWorker(void)
{
    LIF( m_logFile.OpenFile( std::wstring ( L"ChatLog" ) ) );
}

KWorker::~KWorker(void)
{
    End();
}

ImplToStringW( KWorker )
{
    return stm_;
}

void KWorker::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {
       _INT_CASE( EVENT_CHAT_LOG_NOT, KChatLog );
        INT_CASE_NOPARAM( EVENT_CHANGE_CHAT_LOG_NOT );
        //INT_CASE_NOPARAM( EVENT_STAT_AGENT );
       _INT_CASE( EVENT_LONG_PROCESS_LOG, std::wstring );
       _INT_CASE( EVENT_TICK_MANAGER_LOG, std::wstring );
       _INT_CASE( EVENT_DB_QUEUE_SIZE_LOG, std::wstring );
       _INT_CASE( EVENT_LT_TRAFFIC_ATTACK, std::wstring );
       _INT_CASE( EVENT_LT_CLIENT_ERR, std::wstring );
       _INT_CASE( EVENT_LT_CLIENT_ERR_COMMENT, std::wstring );
       //_INT_CASE( EVENT_LT_29_ERR_COMMENT, std::wstring );
       _INT_CASE( EVENT_LT_DUNGEON_HACK, std::wstring );
       _INT_CASE( EVENT_LT_SEND_BUFF_FULL, std::wstring );
       _INT_CASE( EVENT_LT_PACKET_HACK, std::wstring );
       _INT_CASE( EVENT_LT_LOW_PING, std::wstring );
       _INT_CASE( EVENT_LT_EXP_CALC_ERR, std::wstring );
       _INT_CASE( EVENT_LT_DEATH_KILL_HACK, std::wstring );
       _INT_CASE( EVENT_LT_P2P, std::wstring );
       _INT_CASE( EVENT_LT_WIM_WIN, std::wstring );
       _INT_CASE( EVENT_LT_ABUSE, std::wstring );
       _INT_CASE( EVENT_LT_RECV_PACKET, std::vector< std::wstring > );
       _INT_CASE( EVENT_TICK_MANAGER_QUEUE_SIZE, std::wstring );
        INT_CASE( EVENT_LT_PACKET_COUNT );
       _INT_CASE( EVENT_GP_STAT_LOG, KGpLog );
       _INT_CASE( EVENT_ALERT_EXCEED_DB_QUEUE, std::wstring );
       _INT_CASE( EVENT_USER_EVENT_PROCESS_TIME, std::wstring );
       _INT_CASE( EVENT_LT_PVP_HACK, std::wstring );
       _INT_CASE( EVENT_ST_MEMBER_NOT_ING_P2P, std::wstring );
       _INT_CASE( EVENT_USER_START_FAIL_STATE, std::wstring );
       //_INT_CASE( EVENT_NEW_DELETE_USERCOUNT, std::wstring );
       _INT_CASE( EVENT_LT_USER_INVENTORY_ITEM_LIST, std::wstring );

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " 
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) ) << END_LOG;
    }
}

_INT_IMPL_ON_FUNC( EVENT_CHAT_LOG_NOT, KChatLog )
{
    //최대한 가볍게.
    m_logFile << (LPCTSTR)CTime( kPacket_.m_tmTime ).Format( KNC_TIME_FORMAT ) << L"|"
        << kPacket_.m_strNick << L"|"
        << strLogin_ << L"|"
        << kPacket_.m_strChannel << L"|"
        << kPacket_.m_nRoomID+1 << L"|"
        << kPacket_.m_strMsg << GLOG::endl;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CHANGE_CHAT_LOG_NOT )
{
    LIF( m_logFile.ChangeFile() );
}

//INT_IMPL_ON_FUNC_NOPARAM( EVENT_STAT_AGENT )
//{
//    //SiKStatAgent()->Tick();
//}

_INT_IMPL_ON_FUNC( EVENT_LONG_PROCESS_LOG, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_LONG_PROCESS ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_TICK_MANAGER_LOG, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_TICK_MANAGER ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_DB_QUEUE_SIZE_LOG, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_DB_QUEUE_SIZE ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_TRAFFIC_ATTACK, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_TRAFFIC_ATTACK ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_CLIENT_ERR, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_CLIENT_ERR ) << kPacket_ << GLOG::endl;

}

_INT_IMPL_ON_FUNC( EVENT_LT_CLIENT_ERR_COMMENT, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_CLIENT_ERR_COMMENT ) << kPacket_ << GLOG::endl;
}

//_INT_IMPL_ON_FUNC( EVENT_LT_29_ERR_COMMENT, std::wstring )
//{
//    SiKLogManager()->Get( KLogManager::LT_29_ERR_COMMENT ) << kPacket_ << GLOG::endl;
//}

_INT_IMPL_ON_FUNC( EVENT_LT_DUNGEON_HACK, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_DUNGEON_HACK ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_SEND_BUFF_FULL, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_SEND_BUFF_FULL ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_PACKET_HACK, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_PACKET_HACK ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_LOW_PING, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_LOW_PING ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_EXP_CALC_ERR, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_EXP_CALC_ERR ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_DEATH_KILL_HACK, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_DEATH_KILL_HACK ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_P2P, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_P2P ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_WIM_WIN, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_WIM_WIN ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_ABUSE, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_ABUSE ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_RECV_PACKET, std::vector< std::wstring > )
{
    std::vector< std::wstring >::const_iterator vit;
    for ( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {
        SiKLogManager()->Get( KLogManager::LT_RECV_PACKET ) << *vit << GLOG::endl; 
    }
}

_INT_IMPL_ON_FUNC( EVENT_TICK_MANAGER_QUEUE_SIZE, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_TICK_MANAGER_QUEUE ) << kPacket_ << GLOG::endl;
}

INT_IMPL_ON_FUNC( EVENT_LT_PACKET_COUNT )
{
    __int64 biPacketCountSum = 0;
    std::map<__int64,USHORT> mapTemp;
    mapTemp.clear();
    // 패킷 많은 수부터 내림차순으로 기록하기 위해서 map의 순서를 바꿔서 구성.
    if ( !kPacket_.empty() ) {
        std::map<USHORT,__int64>::iterator mit;
        for ( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit ) {
            mapTemp[mit->second] = mit->first;
            biPacketCountSum += mit->second;
        }
    }

    std::wstringstream stm;
    stm << KNC_TIME_STRING << L" (" << biPacketCountSum;

    if ( !mapTemp.empty() ) {
        std::map<__int64,USHORT>::reverse_iterator rmit;
        for ( rmit = mapTemp.rbegin() ; rmit != mapTemp.rend() ; ++rmit ) {
            stm << L"|" << rmit->second << L")" << rmit->first;
        }
    }

    SiKLogManager()->Get( KLogManager::LT_PACKET_COUNT ) << stm.str() << GLOG::endl;
    stm.clear();
    stm.str(L"");
}

_INT_IMPL_ON_FUNC( EVENT_GP_STAT_LOG, KGpLog )
{
    if ( !kPacket_.m_strGpIncLog.empty() ) {
        SiKLogManager()->Get( KLogManager::LT_GP_INC ) << kPacket_.m_strGpIncLog << GLOG::endl;
    }

    if ( !kPacket_.m_strGpDecLog.empty() ) {
        SiKLogManager()->Get( KLogManager::LT_GP_DEC ) << kPacket_.m_strGpDecLog << GLOG::endl;
    }
}

_INT_IMPL_ON_FUNC( EVENT_ALERT_EXCEED_DB_QUEUE, std::wstring )
{
    std::stringstream stm;
    stm << "SMSender.exe " << KncUtil::toNarrowString(kPacket_);
    system( stm.str().c_str() );

    START_LOG( clog, L"Call SMSender.exe " << kPacket_ ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_USER_EVENT_PROCESS_TIME, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_USER_EVENT_PROCESS_TIME ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_LT_PVP_HACK, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_PVP_HACK ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_ST_MEMBER_NOT_ING_P2P, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_ST_MEMBER_NOT_ING_P2P ) << kPacket_ << GLOG::endl;
}

_INT_IMPL_ON_FUNC( EVENT_USER_START_FAIL_STATE, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_USER_START_FAIL_STATE ) << kPacket_ << GLOG::endl;
}

//_INT_IMPL_ON_FUNC( EVENT_NEW_DELETE_USERCOUNT, std::wstring )
//{
//    SiKLogManager()->Get( KLogManager::LT_NEW_DELETE_USER ) << kPacket_ << GLOG::endl;
//}

_INT_IMPL_ON_FUNC( EVENT_LT_USER_INVENTORY_ITEM_LIST, std::wstring )
{
    SiKLogManager()->Get( KLogManager::LT_USER_INVENTORY_ITEM_LIST ) << kPacket_ << GLOG::endl;
}
