#include "Adventure.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "CenterPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "GSDBLayer.h"
#include <boost/bind.hpp>
#include "Log4.h"

ImplementSingleton( KAdventure );
ImplOstmOperatorW2A( KAdventure );
NiImplementRootRTTI( KAdventure );

KAdventure::KAdventure( void )
{
}

KAdventure::~KAdventure( void )
{
}

ImplToStringW( KAdventure )
{
    KLocker lock( m_csAdventure );

    START_TOSTRINGW
        << TOSTRINGW( m_mapAdventureMonsterInfo.size() )
        << TOSTRINGW( m_mapContinentRewardInfo.size() )
        << TOSTRINGW( m_kFinalRewardInfo.m_ItemID )
        << TOSTRINGWc( m_kFinalRewardInfo.m_cGradeID )
        << TOSTRINGW( m_kFinalRewardInfo.m_nPeriod )
        << TOSTRINGW( m_kFinalRewardInfo.m_nDuration )
        << TOSTRINGW( m_mapAdventureEventDate.size() )
        << TOSTRINGWb( IsRun() );
    {
        std::map< PAIR_DWORD, KAdventureMonsterInfo >::const_iterator mit = m_mapAdventureMonsterInfo.begin();

        stm_ << L"-------------- KAdventureMonsterInfo --------------" << std::endl;
        for ( ; mit != m_mapAdventureMonsterInfo.end(); ++mit ) {
            stm_ << L"dwDungeonID: " << mit->second.m_dwDungeonID
                << L", dwMonsterID: " << mit->second.m_dwMonsterID
                << L", ContinentID: " << mit->second.m_prDropItemInfo.first
                << L", CollectID: " << mit->second.m_prDropItemInfo.second
                << L", fDropChance: " << mit->second.m_fDropChance
                << L", dwDropCount: " << mit->second.m_dwDropCount << L", BiteMeString: ";

            std::vector< DWORD >::const_iterator vit = mit->second.m_vecBiteMeString.begin();
            for ( ; vit != mit->second.m_vecBiteMeString.end(); ++vit ) {
                stm_ << static_cast< DWORD >(*vit) << L", ";
            }
            stm_ << std::endl;
        }
    }
    {
        std::map< int, KContinentRewardInfo >::const_iterator mit = m_mapContinentRewardInfo.begin();

        stm_ << L"-------------- KContinentRewardInfo --------------" << std::endl;
        for ( ; mit != m_mapContinentRewardInfo.end(); ++mit ) {
            stm_ << L"nContinentID: " << mit->second.m_nContinentID << L":= CollectItems: ";

            std::map< DWORD, KCollectItem >::const_iterator mit2 = mit->second.m_mapCollectItems.begin();
            for ( ; mit2 != mit->second.m_mapCollectItems.end(); ++mit2 ) {
                stm_ << L"(" << mit2->second.m_dwCollectID << L", "
                    << mit2->second.m_dwDisplayItemID << L", "
                    << mit2->second.m_dwMaxCount << "), ";
            }
            stm_ << L", Rewards: ";
            std::vector< KDropItemInfo >::const_iterator vit = mit->second.m_vecContinentRewards.begin();
            for ( ; vit != mit->second.m_vecContinentRewards.end(); ++vit ) {
                stm_ << vit->m_ItemID << L", "
                    << static_cast< int >( vit->m_cGradeID ) << L", "
                    << vit->m_nPeriod << L", "
                    << vit->m_nDuration << L", ";
            }
            stm_ << std::endl;
        }
    }

    {
        stm_ << L"-------------- KFinalRewardInfo --------------" << std::endl;
        stm_ << m_kFinalRewardInfo.m_ItemID << L", "
            << static_cast< int >( m_kFinalRewardInfo.m_cGradeID ) << L", "
            << m_kFinalRewardInfo.m_nPeriod << L", "
            << m_kFinalRewardInfo.m_nDuration << L", ";
        stm_ << std::endl;
    }

    {
        std::map< int, KAdventureEventDate >::const_iterator mit = m_mapAdventureEventDate.begin();

        stm_ << L"-------------- KAdventureEventDate --------------" << std::endl;
        for ( ; mit != m_mapAdventureEventDate.end(); ++mit ) {
            stm_ << L"nVersion: " << mit->second.m_nVersion
                << L"\nnEventID: " << mit->second.m_nEventID
                << L"\ntmStartDate: " << ( LPCWSTR )CTime( mit->second.m_tmStartDate ).Format( KNC_TIME_FORMAT )
                << L"\ntmEndDate: " << ( LPCWSTR )CTime( mit->second.m_tmEndDate).Format( KNC_TIME_FORMAT ) << std::endl;
        }
    }

    return stm_;
}

void KAdventure::SetAdventureEventData( IN const KECN_ADVENTURE_DATA_NOT& kPacket_ )
{
    KLocker kLocker( m_csAdventure );

    m_mapAdventureMonsterInfo.clear();
    m_mapContinentRewardInfo.clear();
    m_mapAdventureEventDate.clear();

    m_mapAdventureMonsterInfo = kPacket_.m_mapAdventureMonsterInfo;
    m_mapContinentRewardInfo = kPacket_.m_mapContinentRewardInfo;
    m_kFinalRewardInfo = kPacket_.m_kFinalRewardInfo;
    m_mapAdventureEventDate = kPacket_.m_mapAdventureEventDate;
}

bool KAdventure::IsRun() const
{
    KLocker kLocker( m_csAdventure );

    CTime ctmCurrent = CTime::GetCurrentTime();
    time_t tmCurrent = KncUtil::TimeToInt( CTime( ctmCurrent.GetYear(), ctmCurrent.GetMonth(), ctmCurrent.GetDay(), ctmCurrent.GetHour(), ctmCurrent.GetMinute(), 0 ) );

    std::map< int, KAdventureEventDate >::const_iterator mit = m_mapAdventureEventDate.begin();
    for ( ; mit != m_mapAdventureEventDate.end(); ++mit ) {
        if ( mit->second.m_tmStartDate <= tmCurrent && tmCurrent <= mit->second.m_tmEndDate ) {
            START_LOG( clog, L"모험 이벤트 기간임" )
                << BUILD_LOGtm( ctmCurrent )
                << BUILD_LOG( mit->second.m_nEventID )
                << BUILD_LOG( mit->second.m_nVersion )
                << BUILD_LOGtm( CTime( mit->second.m_tmStartDate ) )
                << BUILD_LOGtm( CTime( mit->second.m_tmEndDate ) )
                << END_LOG;
            return true;
        }
    }
    START_LOG( clog, L"모험 이벤트 기간이 아님")
        << BUILD_LOGtm( ctmCurrent )
        << END_LOG;
    return false;
}

int KAdventure::GetVersion()
{
    KLocker kLocker( m_csAdventure );

    CTime ctmCurrent = CTime::GetCurrentTime();
    time_t tmCurrent = KncUtil::TimeToInt( CTime( ctmCurrent.GetYear(), ctmCurrent.GetMonth(), ctmCurrent.GetDay(), ctmCurrent.GetHour(), ctmCurrent.GetMinute(), 0 ) );

    std::map< int, KAdventureEventDate >::const_iterator mit = m_mapAdventureEventDate.begin();
    for ( ; mit != m_mapAdventureEventDate.end(); ++mit ) {
        if ( mit->second.m_tmStartDate <= tmCurrent && tmCurrent <= mit->second.m_tmEndDate ) {
            return mit->second.m_nVersion;
        }
    }
    return -1;
}

bool KAdventure::GetAdventureData( OUT KEVENT_ADVENTURE_DATA_ACK& kPacket_ )
{
    KLocker kLocker( m_csAdventure );

    if ( m_mapContinentRewardInfo.empty() || m_kFinalRewardInfo.m_ItemID == 0 ) {
        return false;
    }

    kPacket_.m_mapContinentRewardInfo.clear();

    kPacket_.m_mapContinentRewardInfo = m_mapContinentRewardInfo;
    kPacket_.m_kFinalRewardInfo = m_kFinalRewardInfo;

    return true;
}

bool KAdventure::GetContinentRewardInfo( IN const int& nContinentID_, OUT KContinentRewardInfo& kContinentRewardInfo_ )
{
    KLocker kLocker( m_csAdventure );

    std::map< int, KContinentRewardInfo >::iterator mit;
    mit = m_mapContinentRewardInfo.find( nContinentID_ );
    if ( mit != m_mapContinentRewardInfo.end() ) {
        kContinentRewardInfo_ = mit->second;
        return true;
    }

    return false;
}

bool KAdventure::GetContinentRewardListInfo( OUT std::map< int, KContinentRewardInfo >& mapContinentRewardInfo_ )
{
    KLocker kLocker( m_csAdventure );

    if ( m_mapContinentRewardInfo.empty() ) {
        return false;
    }

    mapContinentRewardInfo_.clear();

    mapContinentRewardInfo_ = m_mapContinentRewardInfo;

    return true;
}

bool KAdventure::GetFinalRewardInfo( OUT KDropItemInfo& kFinalRewardInfo_ )
{
    KLocker kLocker( m_csAdventure );

    kFinalRewardInfo_ = m_kFinalRewardInfo;

    return true;
}

bool KAdventure::CheckContidionContinentReward( IN const int& nContinentID_, IN std::map< DWORD, int >& mapCollectData_ )
{
    KLocker kLocker( m_csAdventure );

    // 해당 대륙의 정보가 있는지 확인하고
    std::map< int, KContinentRewardInfo >::iterator mitContinentRewardInfo;
    mitContinentRewardInfo = m_mapContinentRewardInfo.find( nContinentID_ );
    if ( mitContinentRewardInfo == m_mapContinentRewardInfo.end() ) {
        START_LOG( cerr, L"대륙 보상 정보가 없음. 대륙ID: " << nContinentID_ ) << END_LOG;
        return false;
    }

    std::map< DWORD, KCollectItem >::iterator mitCollectItems = mitContinentRewardInfo->second.m_mapCollectItems.begin();
    for ( ; mitCollectItems != mitContinentRewardInfo->second.m_mapCollectItems.end(); ++mitCollectItems ) {
        // mitCollectItems->first; // collect id
        // mitCollectItems->second.m_dwMaxCount; // max count
        std::map< DWORD, int >::iterator mitCollectData;
        mitCollectData = mapCollectData_.find( mitCollectItems->first );
        // collectID가 없으면 대륙 보상 받을 수 없음
        // 서버는 스크립트 정보 기준으로 모든 대륙별 collectID에 대하여 항상 0 이상의 값을 유지함. 데이터가 없는 경우는 없도록 구현되어있음
        // 따라서 아래 조건에 걸릴일은 없겠지만 혹시나 해서 넣어둠
        if ( mitCollectData == mapCollectData_.end() ) {
            START_LOG( cerr, L"대륙 보상 받을 수 없음. 대륙ID: " << mitContinentRewardInfo->second.m_nContinentID << ", CollectID: " << mitCollectData->first ) << END_LOG;
            return false;
        }

        if ( mitCollectData->second >= static_cast< int >( mitCollectItems->second.m_dwMaxCount ) ) {
            continue;
        }

        START_LOG( cerr, L"대륙 보상 받을 수 없음. 수량이 모자람. 대륙ID: " << mitContinentRewardInfo->second.m_nContinentID << ", CollectID: " << mitCollectData->first ) << END_LOG;
        return false;
    }
    return true;
}

bool KAdventure::CheckContidionFinalReward( IN std::map< int, bool >& mapContinentData_ )
{
    KLocker kLocker( m_csAdventure );

    if ( m_mapContinentRewardInfo.size() != mapContinentData_.size() ) {
        return false;
    }
    std::set< int > setA;
    std::set< int > setB;
    setA.clear();
    setB.clear();

    std::map< int, KContinentRewardInfo >::iterator mit = m_mapContinentRewardInfo.begin();
    for (; mit != m_mapContinentRewardInfo.end(); ++mit ) {
        setA.insert( mit->first );
    }

    std::map< int, bool >::iterator mit2 = mapContinentData_.begin();
    for (; mit2 != mapContinentData_.end(); ++mit2 ) {
        if ( mit2->second ) {
            setB.insert( mit2->first );
        }
    }

    START_LOG( clog, L"최종 보상 받을 수 있는지 확인" )
        << BUILD_LOG( m_mapContinentRewardInfo.size() )
        << BUILD_LOG( mapContinentData_.size() )
        << BUILD_LOG( setA.size() )
        << BUILD_LOG( setB.size() )
        << END_LOG;

    return ( setA == setB );
}

void KAdventure::InitAdventureUserData( OUT std::map< int, std::map< DWORD, int > >& mapCollectData_, OUT std::map< int, bool >& mapContinentData_ )
{
    KLocker kLocker( m_csAdventure );

    // 스크립트 정보 참고하여 0으로 전부 초기화
    std::map< int, KContinentRewardInfo >::iterator mitContinent = m_mapContinentRewardInfo.begin();
    for ( ; mitContinent != m_mapContinentRewardInfo.end(); ++mitContinent ) {
        mapContinentData_[ mitContinent->first ] = false;

        std::map< DWORD, KCollectItem >::iterator mitCollect = mitContinent->second.m_mapCollectItems.begin();
        for ( ; mitCollect!= mitContinent->second.m_mapCollectItems.end(); ++mitCollect ) {
            mapCollectData_[ mitContinent->first ][ mitCollect->first ] = 0;
        }
    }
}

bool KAdventure::GetAdventureMonsterInfo( OUT std::map< PAIR_DWORD, KAdventureMonsterInfo >& mapAdventureMonsterInfo_ )
{
    KLocker kLocker( m_csAdventure );

    mapAdventureMonsterInfo_ = m_mapAdventureMonsterInfo;

    return true;
}

DWORD KAdventure::GetMaxCollectCount( IN const int& nContinentID_, IN const DWORD& dwCollectID_ )
{
    KLocker kLocker( m_csAdventure );

    std::map< int, KContinentRewardInfo >::iterator mitContinent;
    mitContinent = m_mapContinentRewardInfo.find( nContinentID_ );
    if ( mitContinent != m_mapContinentRewardInfo.end() ) {
        std::map< DWORD, KCollectItem >::iterator mitCollectItems;
        mitCollectItems = mitContinent->second.m_mapCollectItems.find( dwCollectID_ );
        if ( mitCollectItems != mitContinent->second.m_mapCollectItems.end() ) {
            return mitCollectItems->second.m_dwMaxCount;
        }
    }
    return 0;
}

void KAdventure::GetCollectDataDiff( IN std::map< int, std::map< DWORD, int > >& mapCollectDataPrev_, OUT std::map< int, std::map< DWORD, int > >& mapCollectData_ )
{
    // mapCollectData_의 내용에서 mapCollectDataPrev_의 내용을 빼면 증가한 수치가 나온다
    // 그걸 mapCollectData_에 넣는다. 즉 mapCollectData_에 Diff 값을 저장한다
    std::map< int, std::map< DWORD, int > >::iterator mitCollectData = mapCollectData_.begin();
    for ( ; mitCollectData != mapCollectData_.end(); ++mitCollectData ) {
        std::map< int, std::map< DWORD, int > >::iterator mitCollectDataPrev;
        mitCollectDataPrev = mapCollectDataPrev_.find( mitCollectData->first );
        if ( mitCollectDataPrev != mapCollectDataPrev_.end() ) {
            GetCollectDiff( mitCollectDataPrev->second, mitCollectData->second );
        }
    }
}

void KAdventure::GetCollectDiff( IN std::map< DWORD, int >& mapCollectPrev_, OUT std::map< DWORD, int >& mapCollect_ )
{
    std::map< DWORD, int >::iterator mitCollect = mapCollect_.begin();
    for ( ; mitCollect != mapCollect_.end(); ++mitCollect ) {
        std::map< DWORD, int >::iterator mitCollectPrev;
        mitCollectPrev = mapCollectPrev_.find( mitCollect->first );
        if ( mitCollectPrev != mapCollectPrev_.end() ) {
            // 찾으면 diff 계산해서 저장함
            int nDiffCount = mitCollect->second - mitCollectPrev->second;
            nDiffCount = std::max< int >( 0, nDiffCount );
            mitCollect->second = nDiffCount;
        }
    }
}

bool KAdventure::AdjustCollectData( OUT std::map< int, std::map< DWORD, int > >& mapCollectData_ )
{
    bool bReturn = true;

    std::map< int, std::map< DWORD, int > >::iterator mitCollectData = mapCollectData_.begin();
    for ( ; mitCollectData != mapCollectData_.end(); ++mitCollectData ) {
        std::map< DWORD, int >::iterator mitCollect = mitCollectData->second.begin();
        for ( ; mitCollect != mitCollectData->second.end(); ++mitCollect ) {
            int nMaxCount = static_cast< int >( GetMaxCollectCount( mitCollectData->first, mitCollect->first ) );
            if ( mitCollect->second > nMaxCount ) {
                mitCollect->second = nMaxCount;
                START_LOG( cerr, L"모험 이벤트 게임 나가기에서 유저 전리품 수 MaxCount로 보정됨" )
                    << BUILD_LOG( nMaxCount )
                    << BUILD_LOG( mitCollect->second )
                    << END_LOG;
                bReturn = false;
            }

        }
    }
    return bReturn;
}

//===========================================================================================

KAdventureUserData::KAdventureUserData()
{
    m_mapCollectData.clear();
    m_mapCollectDataPrev.clear();
    m_mapContinentData.clear();
}

KAdventureUserData::~KAdventureUserData()
{
}

void KAdventureUserData::SetAdventureUserData( IN OUT std::map< int, std::map< DWORD, int > >& mapCollectData_, IN OUT std::map< int, bool >& mapContinentData_ )
{
    m_mapCollectData.clear();
    m_mapCollectDataPrev.clear();
    m_mapContinentData.clear();

    // DB에 데이터가 없는 경우 0으로 초기화 해서 서버가 유지, 클라에게도 전달함
    SiKAdventure()->InitAdventureUserData( m_mapCollectData, m_mapContinentData );

    // DB에서 받아온 정보를 멤버에 덧붙임
    std::map< int, std::map< DWORD, int > >::iterator mitCollectData = mapCollectData_.begin();
    for ( ; mitCollectData != mapCollectData_.end(); ++mitCollectData ) {
        std::map< DWORD, int >::iterator mitCollectID = mitCollectData->second.begin();
        for ( ; mitCollectID != mitCollectData->second.end(); ++mitCollectID ) {
            m_mapCollectData[ mitCollectData->first ][ mitCollectID->first ] = mitCollectID->second;
        }
    }

    std::map< int, bool >::iterator mitContinent = mapContinentData_.begin();
    for ( ; mitContinent != mapContinentData_.end(); ++mitContinent ) {
        m_mapContinentData[ mitContinent->first ] = mitContinent->second;
    }

    mapCollectData_ = m_mapCollectData;
    mapContinentData_ = m_mapContinentData;
}

bool KAdventureUserData::IsRecievedContinentReward( IN const int& nContinentID_ )
{
    // 보상 받은 대륙이 있으면 mit->second가 true이고.. mit->second가 fale이면 안받은 것, 해당 대륙 아이디에 대한 정보가 없으면 안받은 것
    std::map< int, bool >::const_iterator mit;
    mit = m_mapContinentData.find( nContinentID_ );
    if ( mit != m_mapContinentData.end() ) {
        return mit->second;
    }
    return true;
}

bool KAdventureUserData::SetRecieveContinentReward( IN const int& nContinentID_, IN const bool& bRecieved_ )
{
    m_mapContinentData[ nContinentID_ ] = bRecieved_;

    return true;
}
