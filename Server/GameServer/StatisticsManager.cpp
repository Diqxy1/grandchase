#include ".\statisticsmanager.h"
#include "Statistics.h"
#include "CPLStatistics.h"
#include "PlayTimeStat.h"
#include "MapStatistics.h"
#include "LoginOutStat.h"
#include "StatEmoticon.h"
#include "StatItemGet.h"
#include "StatGameLoading.h"
#include "StatDeathMatch.h"
#include "CheckInStat.h"
#include "StatP2PCritical.h"
#include "StatMiniGame.h"
#include "StatSpx.h"
#include "StatDisconnect.h"
#include "StatLeaveGame.h"
#include "StatCoupleRoom.h"
#include "StatGameStartGap.h"
#include "KStatPing.h"
#include "StatDungeon.h"
#include "StatLoadingInfo.h"
#include "StatRoomJoin.h"
#include "StatGP.h"
#include "StatCharacter.h"
#include "StatCharacterEquip.h"
#include "StatNetwork.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
#include "StatPvPPlayCharacter.h"

ImplementSingleton( KStatisticsManager );
NiImplementRootRTTI( KStatisticsManager );
ImplOstmOperatorW2A( KStatisticsManager );

#undef SI_ENUM
#define SI_ENUM( id, comment ) L#id,
const wchar_t KStatisticsManager::ms_szSITitle[][SI_MAX_STRING] = {
#include "StatisticsID_def.h"
};

#undef SI_ENUM
#define SI_ENUM( id, comment ) L#comment,
const wchar_t KStatisticsManager::ms_szSIDesc[][SI_MAX_STRING] = {
#include "StatisticsID_def.h"
};

ImplToStringW( KStatisticsManager )
{
    KLocker lock( m_csStatistics );
    START_TOSTRINGW << TOSTRINGW( m_mapStatistics.size() );

    std::map< int, KStatisticsPtr >::const_iterator mit;
    for( mit = m_mapStatistics.begin() ; mit != m_mapStatistics.end() ; ++mit )
    {
        mit->second->Dump( stm_ );
    }
    return stm_;
}

KStatisticsManager::KStatisticsManager(void)
{
}

KStatisticsManager::~KStatisticsManager(void)
{
}

void KStatisticsManager::AddStatistics( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ ) // config_server.lua 에서 호출
{
    KLocker lock( m_csStatistics );

    JIF( nID_ >= 0 && nID_ < SI_SENTINEL );
    JIF( m_mapStatistics.find( nID_ ) == m_mapStatistics.end() ); // 이미 있으면 쌩..
    KStatisticsPtr spStatistics = CreateStatObject( nID_ );
    JIF( spStatistics );
    spStatistics->Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
    spStatistics->SetDesc( std::wstring(KStatisticsManager::ms_szSITitle[nID_]), std::wstring(KStatisticsManager::ms_szSIDesc[nID_]) );
    m_mapStatistics.insert( std::make_pair( nID_, spStatistics ) );
}

void KStatisticsManager::ModifyStatistics( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    std::map< int, KStatisticsPtr >::iterator mit;
    mit = m_mapStatistics.find( nID_ );
    JIF( mit != m_mapStatistics.end() );
    mit->second->Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
}

void KStatisticsManager::Tick()
{
    KLocker lock( m_csStatistics );
    std::map< int, KStatisticsPtr >::iterator mit;
    for( mit = m_mapStatistics.begin() ; mit != m_mapStatistics.end() ; ++mit )
    {
        mit->second->Tick();
    }
}

const wchar_t* KStatisticsManager::GetSiName( int nID_ )
{
    _JIF( nID_ < SI_SENTINEL && nID_ >= 0, return &KStatisticsManager::ms_szSITitle[ SI_SENTINEL ][0] );
    return &KStatisticsManager::ms_szSITitle[ nID_ ][0];
}

KStatisticsPtr KStatisticsManager::FindStatistics( int nID_ ) // 통계 포인터 얻을 때 사용
{
    std::map< int, KStatisticsPtr >::iterator mit;

    KLocker lock( m_csStatistics );
    mit = m_mapStatistics.find( nID_ );
    if( mit == m_mapStatistics.end() ) return KStatisticsPtr();

    return mit->second;
}

KStatisticsPtr KStatisticsManager::CreateStatObject( int nID_ )
{
    switch( nID_ )
    {
    case SI_CPL:
        {
            KCPLStatisticsPtr spStat( new KCPLStatistics );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_PLAY_TIME:
        {
            KPlayTimeStatPtr spStat( new KPlayTimeStat );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_MAP_COUNT:
        {
            KMapStatisticsPtr spStat( new KMapStatistics );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_LOGINOUT_STAT:
        {
            KLoginOutStatPtr spStat( new KLoginOutStat );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_EMOTICON:
        {
            KStatEmoticonPtr spStat( new KStatEmoticon );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_ITEM_GET:
        {
            KStatItemGetPtr spStat( new KStatItemGet );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_GAMELOADING:
        {
            KStatGameLoadingPtr spStat( new KStatGameLoading );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_DEATH:
        {
            KStatDeathMatchPtr spStat( new KStatDeathMatch );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_CHECK_IN:
        {
            KCheckInStatPtr spStat( new KCheckInStat );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_P2P_CRITICAL:
        {
            KStatP2PCriticalPtr spStat( new KStatP2PCritical );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_MINIGAME:
        {
            KStatMiniGamePtr spStat( new KStatMiniGame );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_SPX:
        {
            KStatSpxPtr spStat( new KStatSpx );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_DISCONNECT:
        {
            KStatDisconnectPtr spStat( new KStatDisconnect );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_LEAVE_GAME:
        {
            KStatLeaveGamePtr spStat( new KStatLeaveGame );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_COUPLE_ROOM:
        {
            KStatCoupleRoomPtr spStat( new KStatCoupleRoom );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_GAME_START_GAP:
        {
            KStatGameStartGapPtr spStat( new KStatGameStartGap );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_PING_STAT:
        {
            KStatPingPtr spStat( new KStatPing );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_DUNGEON:
        {
            KStatDungeonPtr spStat( new KStatDungeon );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_LOADINGINFO:
        {
            KStatLoadingInfoPtr spStat( new KStatLoadingInfo );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_ROOMJOIN:
        {
            KStatRoomJoinPtr spStat( new KStatRoomJoin );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_GP:
        {
            KStatGPPtr spStat( new KStatGP );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_CHARACTER:
        {
            KStatCharacterPtr spStat( new KStatCharacter );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_CHAR_EQUIP:
        {
            KStatCharacterEquipPtr spStat( new KStatCharacterEquip );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_NETWORK:
        {
            KStatNetworkPtr spStat( new KStatNetwork );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_FATAL_DEATH:
        {
            KStatDeathMatchPtr spStat( new KStatDeathMatch );
            return std::static_pointer_cast<KStatistics>(spStat);
        }
    case SI_STAT_PVP_PLAY_CHARACTER:
        {
            KStatPvPPlayCharacterPtr spStat( new KStatPvPPlayCharacter );
            return std::static_pointer_cast<KStatistics>(spStat);
        }

    default:
        START_LOG( cerr, L" 알수 없는 통계 객체를 생성하려 했음.. ID : " << nID_ ) << END_LOG;
    }

    return KStatisticsPtr();
}

void KStatisticsManager::AddStatDisconn( int DisconnReason_ )
{
    KStatDisconnectPtr spStat = std::static_pointer_cast<KStatDisconnect>( FindStatistics( SI_STAT_DISCONNECT ) );
    if( !spStat ) return;
    spStat->AddStat( DisconnReason_ );
}
