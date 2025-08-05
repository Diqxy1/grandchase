#include "CenterSimLayer.h"
#include "CnAdventure.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "KncTimer.h"
#include "CenterDBThread.h"
#include "Log4.h"

ImplementSingleton( KCnAdventure );
ImplOstmOperatorW2A( KCnAdventure );
NiImplementRootRTTI( KCnAdventure );

KCnAdventure::KCnAdventure(void)
: m_kUpdateCheckTimer( 60 * 1000 )
, m_bChanged( true )
, m_nVersion( -1 )
, m_tmStartDate( 1 )
, m_tmEndDate( 1 )
{
    m_mapAdventureMonsterInfo.clear();
    m_mapContinentRewardInfo.clear();
    m_mapAdventureEventDate.clear();
}

KCnAdventure::~KCnAdventure(void)
{
}

ImplToStringW( KCnAdventure )
{
    KLocker lock( m_csAdventure );

    START_TOSTRINGW
        << TOSTRINGW( m_nVersion )
        << TOSTRINGWt( CTime( m_tmStartDate ) )
        << TOSTRINGWt( CTime( m_tmEndDate ) )
        << TOSTRINGW( m_mapAdventureMonsterInfo.size() )
        << TOSTRINGW( m_mapContinentRewardInfo.size() )
        << TOSTRINGW( m_kFinalRewardInfo.m_ItemID )
        << TOSTRINGWc( m_kFinalRewardInfo.m_cGradeID )
        << TOSTRINGW( m_kFinalRewardInfo.m_nPeriod )
        << TOSTRINGW( m_kFinalRewardInfo.m_nDuration )
        << TOSTRINGW( m_mapAdventureEventDate.size() );

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
                << L", nnEventID: " << mit->second.m_nEventID
                << L", tmStartDate: " << ( LPCWSTR )CTime( mit->second.m_tmStartDate ).Format( KNC_TIME_FORMAT )
                << L", tmEndDate: " << ( LPCWSTR )CTime( mit->second.m_tmEndDate).Format( KNC_TIME_FORMAT ) << std::endl;
        }
    }

    return stm_;
}

bool KCnAdventure::LoadScript()
{
    KLuaManager kLuaMng;

    std::map< PAIR_DWORD, KAdventureMonsterInfo >   mapAdventureMonsterInfo;     // 몬스터 정보;     key: pair< m_dwDungeonID, m_dwMonsterID >
    std::map< int, KContinentRewardInfo >           mapContinentRewardInfo;      // 대륙 보상 정보;  key: m_nContinentID
    KDropItemInfo                                   kFinalRewardInfo;            // 최종 보상 정보;
    DWORD dwTickGap = 60 * 1000;

    _JIF( kLuaMng.DoFile( "InitCnAdventureEvent.lua" ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "AdventureEventInfo" ) == S_OK, return false );

    _JIF( LoadMonsterInfo( kLuaMng, std::string( "MonsterInfo" ), mapAdventureMonsterInfo ), return false );
    _JIF( LoadContinentRewardInfo( kLuaMng, std::string( "ContinentRewardInfo" ), mapContinentRewardInfo ), return false );
    _JIF( LoadFinalRewardInfo( kLuaMng, std::string( "FinalRewardInfo" ), kFinalRewardInfo ), return false );

    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"모험 이벤트 스크립트 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csAdventure );

        m_mapAdventureMonsterInfo.swap( mapAdventureMonsterInfo );
        m_mapContinentRewardInfo.swap( mapContinentRewardInfo );
        m_kFinalRewardInfo = kFinalRewardInfo;

        m_bChanged = true;
    }
    m_kUpdateCheckTimer.SetInterval( dwTickGap );

    return true;
}

bool KCnAdventure::LoadDropItemInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT PAIR_INT_DWORD& prDropItemInfo_ )
{
    prDropItemInfo_.first = -1;
    prDropItemInfo_.second = 0;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( 1, prDropItemInfo_.first ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, prDropItemInfo_.second ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnAdventure::LoadStrings( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< GCITEMID >& vecDisplayString_ )
{
    vecDisplayString_.clear();
    int nStringID = -1;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.GetValue( i, nStringID ) != S_OK ) break;
        vecDisplayString_.push_back( nStringID );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnAdventure::LoadMonsterInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT std::map< PAIR_DWORD, KAdventureMonsterInfo >& mapAdventureMonsterInfo_ )
{
    mapAdventureMonsterInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        DWORD dwDungeonID = 0;

        _JIF( kLuaMng_.GetValue( "DungeonID", dwDungeonID ) == S_OK, return false );

        _JIF( kLuaMng_.BeginTable( "Monsters" ) == S_OK, return false );

        for ( int j = 1; ; ++j ) {
            if( kLuaMng_.BeginTable( j ) != S_OK ) break;
            KAdventureMonsterInfo kAdventureMonsterInfo;

            _JIF( kLuaMng_.GetValue( "MonsterID", kAdventureMonsterInfo.m_dwMonsterID ) == S_OK, return false );
            _JIF( LoadDropItemInfo( kLuaMng_, std::string("DropItemInfo"), kAdventureMonsterInfo.m_prDropItemInfo ), return false );
            _JIF( kLuaMng_.GetValue( "DropChance", kAdventureMonsterInfo.m_fDropChance ) == S_OK, return false );
            _JIF( kLuaMng_.GetValue( "DropCount", kAdventureMonsterInfo.m_dwDropCount ) == S_OK, return false );
            _JIF( LoadStrings( kLuaMng_, std::string("BiteMeString"), kAdventureMonsterInfo.m_vecBiteMeString ), return false );
            kAdventureMonsterInfo.m_dwDungeonID = dwDungeonID;

            PAIR_DWORD prKey = std::make_pair( kAdventureMonsterInfo.m_dwDungeonID, kAdventureMonsterInfo.m_dwMonsterID );
            if ( !mapAdventureMonsterInfo_.insert( std::make_pair( prKey, kAdventureMonsterInfo ) ).second ) {
                START_LOG( cerr, L" 중복된 (던전 ID, 몬스터 ID)가 있음. DungeonID : " << kAdventureMonsterInfo.m_dwDungeonID
                    << ", MonsterID : " << kAdventureMonsterInfo.m_dwMonsterID ) << END_LOG;
                return false;
            }

            _JIF( kLuaMng_.EndTable() == S_OK, return false ); // j
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );  // monsters

        _JIF( kLuaMng_.EndTable() == S_OK, return false );  // i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnAdventure::LoadContinentRewardInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT std::map< int, KContinentRewardInfo >& mapContinentRewardInfo_ )
{
    mapContinentRewardInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KContinentRewardInfo kContinentRewardInfo;

        _JIF( kLuaMng_.GetValue( "ContinentID", kContinentRewardInfo.m_nContinentID ) == S_OK, return false );

        _JIF( LoadCollectItems( kLuaMng_, std::string("CollectItems"), kContinentRewardInfo.m_mapCollectItems ), return false );

        _JIF( LoadRewards( kLuaMng_, std::string("Rewards"), kContinentRewardInfo.m_vecContinentRewards ), return false );

        if ( !mapContinentRewardInfo_.insert( std::make_pair( kContinentRewardInfo.m_nContinentID, kContinentRewardInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 대륙 ID가 있음. ContinentID : " << kContinentRewardInfo.m_nContinentID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnAdventure::LoadCollectItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KCollectItem >& mapCollectItems_ )
{
    mapCollectItems_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KCollectItem kCollectItem;

        _JIF( kLuaMng_.GetValue( "CollectID", kCollectItem.m_dwCollectID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DisplayItemID", kCollectItem.m_dwDisplayItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MaxCount", kCollectItem.m_dwMaxCount ) == S_OK, return false );
        
        if ( !mapCollectItems_.insert( std::make_pair( kCollectItem.m_dwCollectID, kCollectItem ) ).second ) {
            START_LOG( cerr, L"중복된 전리품 ID가 있음. ContinentID : " << kCollectItem.m_dwCollectID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnAdventure::LoadRewards( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecContinentRewards_ )
{
    vecContinentRewards_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KDropItemInfo kData;
        int nGradeID = -1;
        int nEquipLevel = -1;       // 이후에 KDropItemInfo 구조체에 장착 레벨 정보가 추가될지도 몰라 넣어두었습니다

        _JIF( kLuaMng_.GetValue( 1, kData.m_ItemID) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nGradeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kData.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 5, nEquipLevel ) == S_OK, return false );
        kData.m_cGradeID = static_cast< char >( nGradeID );
        vecContinentRewards_.push_back( kData );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnAdventure::LoadFinalRewardInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT KDropItemInfo& kFinalRewardInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int nGradeID = -1;
    int nEquipLevel = -1;       // 이후에 KDropItemInfo 구조체에 장착 레벨 정보가 추가될지도 몰라 넣어두었습니다

    _JIF( kLuaMng_.GetValue( 1, kFinalRewardInfo_.m_ItemID) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nGradeID ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kFinalRewardInfo_.m_nPeriod ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, kFinalRewardInfo_.m_nDuration ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 5, nEquipLevel ) == S_OK, return false );
    kFinalRewardInfo_.m_cGradeID = static_cast< char >( nGradeID );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnAdventure::Tick()
{
    if ( m_kUpdateCheckTimer.CheckTime() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_ADVENTURE_DATA_NOT , L"", 0 );
    }

    if ( m_bChanged ) {
        KECN_ADVENTURE_DATA_NOT kPacket;
        GetAdventureData( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_ADVENTURE_DATA_NOT, kPacket );

        m_bChanged = false;
    }
}

bool KCnAdventure::SetAdventureEventDate( IN const int nVersion_, IN const time_t tmStartDate_, IN const time_t tmEndDate_,
                                          IN const std::map< int, KAdventureEventDate >& mapAdventureEventDate_ )
{
    KLocker kLocker( m_csAdventure );

    m_nVersion = nVersion_;
    m_tmStartDate = tmStartDate_;
    m_tmEndDate = tmEndDate_;

    m_mapAdventureEventDate = mapAdventureEventDate_;
    m_bChanged = true;

    return true;
}

void KCnAdventure::GetAdventureData( OUT KECN_ADVENTURE_DATA_NOT& kPacket_ )
{
    kPacket_.m_mapAdventureMonsterInfo = m_mapAdventureMonsterInfo;
    kPacket_.m_mapContinentRewardInfo = m_mapContinentRewardInfo;
    kPacket_.m_kFinalRewardInfo = m_kFinalRewardInfo;
    kPacket_.m_mapAdventureEventDate = m_mapAdventureEventDate;
}
