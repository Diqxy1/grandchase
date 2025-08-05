#include "CenterSimLayer.h"
#include "CnDungeonRankManager.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
#include <boost/bind.hpp>
#include "GCEnum.h"
#include "CenterEvent.h"
#include "CenterDBThread.h"
#include <boost/foreach.hpp>

ImplementSingleton( KCnDungeonRankManager );
ImplOstmOperatorW2A( KCnDungeonRankManager );
NiImplementRootRTTI( KCnDungeonRankManager );

ImplToStringW( KCnDungeonRankManager )
{
    KLocker lock( m_csCnDungeonRank );

    return START_TOSTRINGW
        << TOSTRINGW( m_nRankEntryPerPage )
        << TOSTRINGW( m_setRankingModes.size() )
        << TOSTRINGWt( m_ctSeasonReset )
        << TOSTRINGW( m_mapCurrentDungeonRankList.size() )
        << TOSTRINGW( m_mapReservedCurrentDungeonRankList.size() )
        << TOSTRINGW( m_mapPreviousDungeonRankList.size() )
        << TOSTRINGW( m_mapReservedPreviousDungeonRankList.size() );
}

namespace
{
    class KDungeonRankComp
    {
    public:
        bool operator() ( const KDungeonRankInfo& a, const KDungeonRankInfo& b ) {
            if ( a.m_nStageNum > b.m_nStageNum ) {
                return true;
            }

            if ( a.m_nStageNum == b.m_nStageNum
                && a.m_dwStageClearTime < b.m_dwStageClearTime ) {
                return true;
            }

            if ( a.m_nStageNum == b.m_nStageNum
                && a.m_dwStageClearTime == b.m_dwStageClearTime
                && a.m_dwLoginUID < b.m_dwLoginUID ) {
                return true;
            }

            return false;
        }
    };
}

KCnDungeonRankManager::KCnDungeonRankManager( void )
: m_kRankingTimer( 60 * 1000 )
, m_nRankEntryPerPage( 8 )
, m_ctSeasonReset( CTime::GetCurrentTime() )
{
    m_setRankingModes.clear();
    m_setRankingModes.insert( GC_GM_QUEST81 ); // 루아 바인드로 넣고 뺄 수 있도록 수정하자

    m_mapCurrentDungeonRankList.clear();
    m_mapReservedCurrentDungeonRankList.clear();

    m_mapPreviousDungeonRankList.clear();
    m_mapReservedPreviousDungeonRankList.clear();
}

KCnDungeonRankManager::~KCnDungeonRankManager( void )
{
}

bool KCnDungeonRankManager::GetRankingModes( OUT std::set< int >& setRankingModes_ )
{
    setRankingModes_.clear();

    KLocker lock( m_csCnDungeonRank );

    setRankingModes_ = m_setRankingModes;

    return true;
}

bool KCnDungeonRankManager::SetCurrentSeasonTopRank( IN const std::map< int, std::list< KDungeonRankInfo > >& mapCurrentDungeonRankList_ )
{
    KLocker lock( m_csCnDungeonRank );

    m_mapCurrentDungeonRankList = mapCurrentDungeonRankList_;

    SortCurrentSeasonTopRank();

    return true;
}

bool KCnDungeonRankManager::SetPreviousSeasonTopRank( IN const std::map< int, std::list< KDungeonRankInfo > >& mapPreviousDungeonRankList_ )
{
    KLocker lock( m_csCnDungeonRank );

    m_mapReservedPreviousDungeonRankList = mapPreviousDungeonRankList_;

    return true;
}

bool KCnDungeonRankManager::SortCurrentSeasonTopRank()
{
    // 함수 호출부에서 lock을 걸었다는 전체로 한다

    /* 리스트를 정렬하고 순위 숫자를 계산해야 한다
       STL list sort()의 O(NLogN) 복잡도 + 순위 숫자 계산에 N 순회가 필요하다
       이 부분을 Bucket Sort를 사용하도록 수정한다면 속도가 개선될 수 있을 것 같다 */
    typedef std::map< int, std::list< KDungeonRankInfo > >::value_type mapCurrentDungeonRankListType;
    BOOST_FOREACH( mapCurrentDungeonRankListType& rDungeonRankInfoValue, m_mapCurrentDungeonRankList ) {
        std::list< KDungeonRankInfo >& rlstDungeonRankInfo = rDungeonRankInfoValue.second;

        if ( true == rlstDungeonRankInfo.empty() ) {
            continue;
        }

        rlstDungeonRankInfo.sort( KDungeonRankComp() );

        // 동일 순위를 고려하여 순위 숫자를 계산한다
        // 예) 1 1 3 4 4 6 ...
        int nBeforeStageNum = rlstDungeonRankInfo.begin()->m_nStageNum;
        DWORD dwBeforeStageClearTime = rlstDungeonRankInfo.begin()->m_dwStageClearTime;
        int nEntryNum = 1;
        int nRankNum = nEntryNum;
        BOOST_FOREACH( KDungeonRankInfo& rDungeonRankInfo, rlstDungeonRankInfo ) {
            if ( nBeforeStageNum != rDungeonRankInfo.m_nStageNum || dwBeforeStageClearTime != rDungeonRankInfo.m_dwStageClearTime ) {
                nRankNum = nEntryNum;
            }

            rDungeonRankInfo.m_nRank = nRankNum;

            nBeforeStageNum = rDungeonRankInfo.m_nStageNum;
            dwBeforeStageClearTime = rDungeonRankInfo.m_dwStageClearTime;

            ++nEntryNum;
        }

        /* 순위 계산 이후 상위 100명의 정보만 유지한다 */
        std::list< KDungeonRankInfo > lstCroppedDungeonRankInfo;
        lstCroppedDungeonRankInfo.clear();
        int nCount = 0;

        std::list< KDungeonRankInfo >::iterator lit = rlstDungeonRankInfo.begin();
        while ( rlstDungeonRankInfo.end() != lit ) {
            if ( 100 <= nCount ) {
                break;
            }

            ++lit;
            ++nCount;
        }

        lstCroppedDungeonRankInfo.insert( lstCroppedDungeonRankInfo.begin(), rlstDungeonRankInfo.begin(), lit );
        rlstDungeonRankInfo.swap( lstCroppedDungeonRankInfo );
    }

    return true;
}

bool KCnDungeonRankManager::AddCurrentSeasonNewEntry( IN const std::vector< KDungeonRankInfo >& vecNewEntry_ )
{
    KLocker lock( m_csCnDungeonRank );

    BOOST_FOREACH( const KDungeonRankInfo& rDungeonRankInfo, vecNewEntry_ ) {
        m_mapReservedCurrentDungeonRankList[ rDungeonRankInfo.m_nModeID ].push_back( rDungeonRankInfo );
    }

    return true;
}

bool KCnDungeonRankManager::AddCurrentSeasonNewEntryAfterDBWrite( IN const std::vector< KDungeonRankInfo >& vecNewEntryAfterDBWrite_ )
{
    KLocker lock( m_csCnDungeonRank );

    // 중복 랭킹을 허용하지 않기 때문에 이미 랭킹에 포함되어 있는지 확인하여 갱신한다
    BOOST_FOREACH( const KDungeonRankInfo& rDungeonRankInfo, vecNewEntryAfterDBWrite_ ) {
        std::list< KDungeonRankInfo >& lstDungeonRankInfo = m_mapCurrentDungeonRankList[ rDungeonRankInfo.m_nModeID ];

        std::list< KDungeonRankInfo >::iterator lit;
        lit = std::find_if( lstDungeonRankInfo.begin(), lstDungeonRankInfo.end(),
            boost::bind( &KDungeonRankInfo::m_dwLoginUID, _1 ) == rDungeonRankInfo.m_dwLoginUID
            && boost::bind( &KDungeonRankInfo::m_cCharType, _1 ) == rDungeonRankInfo.m_cCharType );

        if ( lstDungeonRankInfo.end() == lit ) {
            lstDungeonRankInfo.push_back( rDungeonRankInfo );
        }
        else {
            if ( rDungeonRankInfo.m_nStageNum > lit->m_nStageNum ) {
                *lit = rDungeonRankInfo;
            }
            else if ( rDungeonRankInfo.m_nStageNum == lit->m_nStageNum
                && rDungeonRankInfo.m_dwStageClearTime < lit->m_dwStageClearTime ) {
                    *lit = rDungeonRankInfo;
            }
        }
    }

    SortCurrentSeasonTopRank();

    /* 이 코드가 실행되는 곳이 현재는 센터 DB 스레드인데 여러 스레드에서 실행될 수 있는 만큼
       문제가 발생할 여지가 있지 않을까 라는 찝찝한 느낌이 든다
       문제가 생기는경우 여기서 현재 시즌 정보가 변경되었다는 flag를 세팅하고
       Tick() 함수에서 이를 감지해 모든 서버들에게 패킷을 전달하는 방식으로 변경하는 방법으로 수정할 수 있을 것 같다 */
    SendDungeonRankCurrentSeasonTopRankNot();

    return true;
}

bool KCnDungeonRankManager::GetCurrentSeasonNewEntryToDB( OUT KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ& kPacket_ )
{
    // 함수 호출부에서 lock을 걸었다는 전체로 한다

    time_t tmSeasonReset = KncUtil::TimeToInt( m_ctSeasonReset );
    std::map< int, std::list< KDungeonRankInfo > >::iterator mit = m_mapReservedCurrentDungeonRankList.begin();
    for ( ; m_mapReservedCurrentDungeonRankList.end() != mit; ++mit ) {
        std::list< KDungeonRankInfo >::iterator lit = mit->second.begin();
        for ( ; mit->second.end() != lit; ++lit ) {
            if ( tmSeasonReset <= lit->m_tmDungeonStart ) {
                kPacket_.push_back( *lit );
            }
        }
    }

    m_mapReservedCurrentDungeonRankList.clear();

    return true;
}

bool KCnDungeonRankManager::GetCurrentSeasonNewEntryToDBWithLock( OUT KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ& kPacket_ )
{
    KLocker lock( m_csCnDungeonRank );

    return GetCurrentSeasonNewEntryToDB( kPacket_ );
}

bool KCnDungeonRankManager::SendDungeonRankCurrentSeasonTopRankNot()
{
    // 함수 호출부에서 lock을 걸었다는 전체로 한다

    KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT kPacket = m_mapCurrentDungeonRankList;
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT, kPacket, true );

    return true;
}

void KCnDungeonRankManager::Tick()
{
    if ( false == m_kRankingTimer.CheckTime() ) {
        return;
    }

    KLocker lock( m_csCnDungeonRank );

    bool bReservedCurrentDungeonRankListChanged = ( false == m_mapReservedCurrentDungeonRankList.empty() );
    bool bReservedPreviousDungeonRankListChanged = ( false == m_mapReservedPreviousDungeonRankList.empty() );

    if ( true == bReservedCurrentDungeonRankListChanged ) {
        /* 서버와 DB의 랭킹 리스트 동기화를 위해
           현재 시즌의 새 기록들은 먼저 DB에 기록한 후
           DB 기록(write)을 성공한 기록(record; new entry)들만 랭킹 리스트에 반영한다
           DB측 요청으로 새 기록의 던전 시작 시간이 이전 시즌의 데이터라면 랭킹에 반영하지 않는다 */
        KECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ kPacket;
        GetCurrentSeasonNewEntryToDB( kPacket );

        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_DUNGEON_RANK_CURRENT_SEASON_ADD_RANK_REQ, L"", 0, -1, kPacket );
    }

    if ( true == bReservedPreviousDungeonRankListChanged ) {
        m_mapPreviousDungeonRankList.swap( m_mapReservedPreviousDungeonRankList );

        KECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT kPacket ( m_mapPreviousDungeonRankList );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT, kPacket, true );

        m_mapReservedPreviousDungeonRankList.clear();
    }

    bool bForceReset = false;
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_DB_DUNGEON_RANK_SEASON_RESET_CHECK_REQ, L"", 0, -1, bForceReset );
}

bool KCnDungeonRankManager::GetDungeonRankRankingConfigNotPacket( OUT KECN_DUNGEON_RANK_RANKING_CONFIG_NOT& kRankingConfig_ )
{
    KLocker lock( m_csCnDungeonRank );

    kRankingConfig_.m_nRankEntryPerPage = m_nRankEntryPerPage;
    kRankingConfig_.m_setRankingModes = m_setRankingModes;

    return true;
}

bool KCnDungeonRankManager::GetDungeonRankCurrentSeasonTopRankNotPacket( OUT KECN_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_NOT& kCurrentRankPacket_ )
{
    kCurrentRankPacket_.clear();

    KLocker lock( m_csCnDungeonRank );

    kCurrentRankPacket_ = m_mapCurrentDungeonRankList;

    return true;
}

bool KCnDungeonRankManager::GetDungeonRankPreviousSeasonTopRankNotPacket( OUT KECN_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_NOT& kPreviousRankPacket_ )
{
    kPreviousRankPacket_.clear();

    KLocker lock( m_csCnDungeonRank );

    if ( true == m_mapPreviousDungeonRankList.empty() ) {
        return false;
    }

    kPreviousRankPacket_ = m_mapPreviousDungeonRankList;

    return true;
}

CTime KCnDungeonRankManager::SetSeasonReset()
{
    KLocker lock( m_csCnDungeonRank );

    m_mapCurrentDungeonRankList.clear();

    SendDungeonRankCurrentSeasonTopRankNot();

    m_ctSeasonReset = CTime::GetCurrentTime();

    return m_ctSeasonReset;
}

bool KCnDungeonRankManager::SetSeasonDelimitTime( IN const time_t tmSeasonDelimit_ )
{
    KLocker lock( m_csCnDungeonRank );

    m_ctSeasonReset = CTime( tmSeasonDelimit_ );

    return true;
}

bool KCnDungeonRankManager::DoForceSeasonReset()
{
    bool bForceReset = true;
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_DB_DUNGEON_RANK_SEASON_RESET_CHECK_REQ, L"", 0, -1, bForceReset );

    return true;
}
