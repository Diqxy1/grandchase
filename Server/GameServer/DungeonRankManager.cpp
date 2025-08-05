#include "CnConnector.h"
#include "DungeonRankManager.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
#include "CenterEvent.h"
#include "CenterPacket.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

ImplementSingleton( KDungeonRankManager );
ImplOstmOperatorW2A( KDungeonRankManager );
NiImplementRootRTTI( KDungeonRankManager );

ImplToStringW( KDungeonRankManager )
{
    KLocker lock( m_csDungeonRank );

    return START_TOSTRINGW
        << TOSTRINGW( m_nRankEntryPerPage )
        << TOSTRINGW( m_setRankingModes.size() )
        << TOSTRINGW( m_mapCurrentDungeonRankList.size() )
        << TOSTRINGW( m_mapPreviousDungeonRankList.size() );
}

KDungeonRankManager::KDungeonRankManager( void )
/* 혹시라도 게임 서버 크래시가 발생한 뒤 센터에 연결되기 전에 페이지 계산을 위해 0으로 나누는 상황이 발생할 수 있어서
   게임 서버에서도 변수의 기본 값을 8로 초기화 한다
   일반적인 상황에서는 센터 서버로부터 정보를 받아서 사용한다 */
: m_nRankEntryPerPage( 8 )
{
    m_setRankingModes.clear();
    m_mapCurrentDungeonRankList.clear();
    m_mapPreviousDungeonRankList.clear();
}

KDungeonRankManager::~KDungeonRankManager( void )
{
}

bool KDungeonRankManager::GetRankingModes( OUT std::set< int >& setRankingModes_ )
{
    setRankingModes_.clear();

    KLocker lock( m_csDungeonRank );

    setRankingModes_ = m_setRankingModes;

    return true;
}

bool KDungeonRankManager::SetRankingConfig( IN const KECN_DUNGEON_RANK_RANKING_CONFIG_NOT& kRankingConfig_ )
{
    KLocker lock( m_csDungeonRank );

    m_nRankEntryPerPage = kRankingConfig_.m_nRankEntryPerPage;
    m_setRankingModes = kRankingConfig_.m_setRankingModes;

    return true;
}

bool KDungeonRankManager::SetCurrentDungeonRankList( IN const KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT& kCurrentRankPacket_ )
{
    KLocker lock( m_csDungeonRank );

    m_mapCurrentDungeonRankList = kCurrentRankPacket_;

    return true;
}

bool KDungeonRankManager::SetPreviousDungeonRankList( IN const KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT& kPreviousRankPacket )
{
    KLocker lock( m_csDungeonRank );

    m_mapPreviousDungeonRankList = kPreviousRankPacket;

    return true;
}

bool KDungeonRankManager::IsRankingModes( IN const int nModeID_ )
{
    KLocker lock( m_csDungeonRank );

    std::set< int >::iterator sit = m_setRankingModes.find( nModeID_ );

    if ( m_setRankingModes.end() == sit ) {
        return false;
    }

    return true;
}

int KDungeonRankManager::GetRankEntryPerPage()
{
    KLocker lock( m_csDungeonRank );

    if ( 0 == m_nRankEntryPerPage ) {
        m_nRankEntryPerPage = 8; // 혹시나 0으로 나누는 상황이 발생할지 몰라 보정용으로 넣어둔다
    }

    return m_nRankEntryPerPage;
}

bool KDungeonRankManager::GetCurrentDungeonRankList( IN const int nModeID_, OUT std::list< KDungeonRankInfo >& lstCurrentDungeonRankList_ )
{
    lstCurrentDungeonRankList_.clear();

    KLocker lock( m_csDungeonRank );

    std::map< int, std::list< KDungeonRankInfo > >::iterator mit = m_mapCurrentDungeonRankList.find( nModeID_ );

    if ( m_mapCurrentDungeonRankList.end() == mit ) {
        return false;
    }

    lstCurrentDungeonRankList_ = mit->second;

    return true;
}

bool KDungeonRankManager::GetPreviousDungeonRankList( IN const int nModeID_, OUT std::list< KDungeonRankInfo >& lstPreviousDungeonRankList_ )
{
    lstPreviousDungeonRankList_.clear();

    KLocker lock( m_csDungeonRank );

    std::map< int, std::list< KDungeonRankInfo > >::iterator mit = m_mapPreviousDungeonRankList.find( nModeID_ );

    if ( m_mapPreviousDungeonRankList.end() == mit ) {
        return false;
    }

    lstPreviousDungeonRankList_ = mit->second;

    return true;
}

bool KDungeonRankManager::AddDungeonRankCurrentSeason( IN const int nModeID_, IN const std::vector< KDungeonRankInfo >& vecDungeonRankInfo_ )
{
    if ( true == vecDungeonRankInfo_.empty() ) {
        return false;
    }

    KLocker lock( m_csDungeonRank );

    std::set< int >::iterator sit( m_setRankingModes.find( nModeID_ ) );
    if ( m_setRankingModes.end() == sit ) {
        return false;
    }

    KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ kPacket;
    kPacket.clear();

    std::list< KDungeonRankInfo >& lstDungeonRankInfo = m_mapCurrentDungeonRankList[ nModeID_ ];

    /* 갱신되어야 할 기록인지 판단하여 센터 서버에 전달한다
       센터 서버에서는 게임 시작 시간을 체크하여 DB 기록 여부를 결정한다 */
    BOOST_FOREACH( const KDungeonRankInfo& rDungeonRankInfo, vecDungeonRankInfo_ ) {
        std::list< KDungeonRankInfo >::iterator lit;
        lit = std::find_if( lstDungeonRankInfo.begin(), lstDungeonRankInfo.end(),
            boost::bind( &KDungeonRankInfo::m_dwLoginUID, _1 ) == rDungeonRankInfo.m_dwLoginUID
            && boost::bind( &KDungeonRankInfo::m_cCharType, _1) == rDungeonRankInfo.m_cCharType );

        if ( lstDungeonRankInfo.end() == lit ) {
            kPacket.push_back( rDungeonRankInfo );
        }
        else {
            if ( rDungeonRankInfo.m_nStageNum > lit->m_nStageNum ) {
                kPacket.push_back( rDungeonRankInfo );
            }
            else if ( rDungeonRankInfo.m_nStageNum == lit->m_nStageNum
                && rDungeonRankInfo.m_dwStageClearTime < lit->m_dwStageClearTime ) {
                kPacket.push_back( rDungeonRankInfo );
            }
        }
    }

    if ( false == kPacket.empty() ) {
        SiKCnConnector()->SendPacket( KCenterEvent::ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ, kPacket );
    }

    return true;
}
