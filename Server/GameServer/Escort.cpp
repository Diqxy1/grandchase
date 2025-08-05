#include "Escort.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "UserEvent.h"
#include "Log4.h"
#include "boost/foreach.hpp"

ImplementSingleton( KEscort );
ImplOstmOperatorW2A( KEscort );
NiImplementRootRTTI( KEscort );

KEscort::KEscort( void )
: m_nVersion( 1 )
, m_tmBegin( 1 )
, m_tmEnd( 1 )
{
    m_mapEscortMapInfo.clear();
    m_mapEscortMonsterInfo.clear();
    m_mapEscortRewardInfo.clear();
}

KEscort::~KEscort( void )
{
}

ImplToStringW( KEscort )
{
    KLocker lock( m_csEscort);
    return START_TOSTRINGW
        << TOSTRINGW( m_nVersion )
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_mapEscortMapInfo.size() )
        << TOSTRINGW( m_mapEscortMonsterInfo.size() )
        << TOSTRINGW( m_mapEscortRewardInfo.size() );
}

bool KEscort::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csEscort );
    return ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

bool KEscort::LoadScript()
{
    return _LoadScript(std::string());
}

bool KEscort::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitEscortEvent.lua";
    strScript_ = strScriptName;

    int nVersion;
    time_t tmBegin;
    time_t tmEnd;
    std::map< PAIR_DWORD, KEscortMapInfo > mapEscortInfo;
    std::map< DWORD, KEscortInfo > mapEscortMonsterInfo;
    std::map< DWORD, KEscortRewardInfo > mapEscortRewardInfo;

    mapEscortInfo.clear();
    mapEscortMonsterInfo.clear();
    mapEscortRewardInfo.clear();

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "Version", nVersion ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string( "BeginTime" ), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string( "EndTime" ), tmEnd ), return false );

    _JIF( LoadEscortInfo( kLuaMng, std::string( "EscortInfo" ), mapEscortInfo ), return false );
    _JIF( LoadEscortMonsterInfo( kLuaMng, std::string( "EscortMonsterInfo" ), mapEscortMonsterInfo ), return false );
    _JIF( LoadEscortRewardInfo( kLuaMng, std::string( "EscortRewardInfo" ), mapEscortRewardInfo ), return false );

    START_LOG( cerr, L"호위 이벤트 스크립트 읽기 완료." ) << END_LOG;
    {
        KLocker lock( m_csEscort );
        m_nVersion = nVersion;
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_mapEscortMapInfo.swap( mapEscortInfo );
        m_mapEscortMonsterInfo.swap( mapEscortMonsterInfo );
        m_mapEscortRewardInfo.swap( mapEscortRewardInfo );
    }
    //DebugPrint();

    return true;
}

bool KEscort::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear = 0, nMonth = 0, nDay = 0, nHour = 0;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
	
	 if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"호위 이벤트 날짜 정보 스크립트 오류" ) << END_LOG;
            return false;
    }
	
    tmTime_ = KncUtil::TimeToInt( CTime( nYear, nMonth, nDay, nHour, 0, 0 ) );

    return true;
}

bool KEscort::LoadEscortInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_DWORD, KEscortMapInfo >& mapEscortInfo_ )
{
    mapEscortInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwDungeonID = 0;
        DWORD dwDifficulty = 0;
        int nMinLevel = 0;
        int nMaxLevel = 0;

        _JIF( kLuaMng_.GetValue( 1, dwDungeonID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, dwDifficulty ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nMinLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nMaxLevel ) == S_OK, return false );

        std::vector< KEscortMonAppearChance >  vecAppearChances;
        _JIF( LoadEscortMonAppearChanceInfo( kLuaMng_, std::string( "AppearChances" ), vecAppearChances ), return false );

        KEscortMapInfo kEscortMapInfo;
        kEscortMapInfo.m_prDungeonInfo = std::make_pair( dwDungeonID, dwDifficulty );   // 던전 난이도는 0부터 시작한다, 현재 255는 챔피언 모드이다
        kEscortMapInfo.m_prLevelInfo = std::make_pair( nMinLevel, nMaxLevel );
        kEscortMapInfo.m_vecAppearChances.swap( vecAppearChances );

        if ( false == mapEscortInfo_.insert( std::make_pair( kEscortMapInfo.m_prDungeonInfo, kEscortMapInfo ) ).second ) {
            START_LOG( cerr, L"호위 이벤트: 중복된 던전 등록. DungeonID: " << dwDungeonID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KEscort::LoadEscortMonsterInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KEscortInfo >& mapEscortMonsterInfo_ )
{
    mapEscortMonsterInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KEscortInfo kEscortMonsterInfo;
        _JIF( kLuaMng_.GetValue( "MonsterID", kEscortMonsterInfo.m_dwMonsterID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MonsterHealth", kEscortMonsterInfo.m_nMonsterHealth ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "TakeDamage", kEscortMonsterInfo.m_nTakeDamage ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "InvincibleTime", kEscortMonsterInfo.m_nInvincibleTime ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "PotionID", kEscortMonsterInfo.m_PotionID ) == S_OK, return false );
        
        _JIF( kLuaMng_.BeginTable( "AnnounceStrings" ) == S_OK, return false );
        for ( int j= 1; ; ++j ) {
            int nStrID = -1;
            if ( kLuaMng_.GetValue( j, nStrID ) != S_OK) break;
            kEscortMonsterInfo.m_vecStrNum.push_back( nStrID );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable AnnounceStrings

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
        
        if ( !mapEscortMonsterInfo_.insert( std::make_pair( kEscortMonsterInfo.m_dwMonsterID, kEscortMonsterInfo ) ).second ) {
            START_LOG( cerr, L"호위 이벤트: 중복된 몬스터 등록. MonsterID: " << kEscortMonsterInfo.m_dwMonsterID ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KEscort::LoadEscortRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KEscortRewardInfo >& mapEscortRewardInfo_ )
{
    mapEscortRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KEscortRewardInfo kEscortRewardInfo;

        _JIF( kLuaMng_.GetValue( "MonsterID", kEscortRewardInfo.m_dwMonsterID ) == S_OK, return false );
        _JIF( LoadRewardInfo( kLuaMng_, std::string( "AliveReward" ), kEscortRewardInfo.m_kAliveReward ), return false );
        _JIF( LoadRewardInfo( kLuaMng_, std::string( "DeadReward" ), kEscortRewardInfo.m_kDeadReward ), return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
        
        if ( !mapEscortRewardInfo_.insert( std::make_pair( kEscortRewardInfo.m_dwMonsterID, kEscortRewardInfo ) ).second ) {
            START_LOG( cerr, L"호위 이벤트: 중복된 몬스터 등록. MonsterID: " << kEscortRewardInfo.m_dwMonsterID ) << END_LOG;
            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KEscort::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KEscortReward& kRewardInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "ExtraExp", kRewardInfo_.m_fExtraExp ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "ExtraGp", kRewardInfo_.m_fExtraGp ) == S_OK, return false );
    _JIF( LoadRewardList( kLuaMng_, std::string( "ClearRewardList" ), kRewardInfo_.m_vecKairoClearReward ), return false );
    _JIF( LoadRewardList( kLuaMng_, std::string( "ExtraRewardList" ), kRewardInfo_.m_vecKairoExtraReward ), return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    kRewardInfo_.m_fExtraExp /= 100;
    kRewardInfo_.m_fExtraGp /= 100;

    return true;
}

bool KEscort::LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< Kairo >& vecKairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        Kairo kKairo;

        int j = 1;
        while ( true ) {
            GCITEMID itemID = 0;
            int nDuration = 0;
            int nPeriod = 0;
            float fProb = 0.f;

            if ( kLuaMng_.GetValue( j++, itemID ) != S_OK ) break;
            _JIF( kLuaMng_.GetValue( j++, nDuration ) == S_OK, return false );
            _JIF( kLuaMng_.GetValue( j++, nPeriod ) == S_OK, return false );
            _JIF( kLuaMng_.GetValue( j++, fProb ) == S_OK, return false );

            _JIF( kKairo.SetPostItem( itemID, nDuration, nPeriod, fProb ), return false );
        }
        vecKairo_.push_back( kKairo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KEscort::GetEscortInfoAll( OUT KEVENT_ESCORT_INFO_NOT& kPacket_ )
{
    kPacket_.clear();

    KLocker lock( m_csEscort );

    kPacket_ = m_mapEscortMonsterInfo;

    return true;
}

bool KEscort::GetEscortRewardInfo( IN DWORD dwMonID_, OUT KEscortRewardInfo& kEscortRewardInfo_ )
{
    std::map< DWORD, KEscortRewardInfo >::iterator mitReward;

    KLocker lock( m_csEscort );

    mitReward = m_mapEscortRewardInfo.find( dwMonID_ );
    if ( mitReward == m_mapEscortRewardInfo.end() ) {
        START_LOG( cerr, L"호위 이벤트 보상 정보를 찾을 수 없음. MonsterID: " << dwMonID_ ) << END_LOG;
        kEscortRewardInfo_.m_dwMonsterID = -1;
        return false;
    }
    else {
        kEscortRewardInfo_ = mitReward->second;
        return true;
    }
    return true;
}

void KEscort::DebugPrint()
{
    KLocker lock( m_csEscort );

    START_LOG( clog, L"Escort Script Loaded:\n"
                << "Version: " << m_nVersion
                << ", BeginTime: " << m_tmBegin
                << ", EndTime: " << m_tmEnd
                << ", m_mapEscortMapInfo.size(): " << m_mapEscortMapInfo.size()
                << ", m_mapEscortMonsterInfo.size(): " << m_mapEscortMonsterInfo.size()
                << ", m_mapEscortRewardInfo.size(): " << m_mapEscortRewardInfo.size() )  << END_LOG;

    std::map< PAIR_DWORD, KEscortMapInfo >::iterator mit = m_mapEscortMapInfo.begin();
    for ( ; mit != m_mapEscortMapInfo.end(); ++mit ) {
        START_LOG( clog, L"( " << mit->first.first << ", " << mit->first.second << " )"
                << ", ( " << mit->second.m_prLevelInfo.first << ", " << mit->second.m_prLevelInfo.second << " )"
                << ", AppearChances: " << mit->second.m_vecAppearChances.size() ) << END_LOG;
        
        BOOST_FOREACH( KEscortMonAppearChance& rEscortMonAppearChance, mit->second.m_vecAppearChances ) {
                std::cout << "( " << rEscortMonAppearChance.m_dwMonsterID << ", " << rEscortMonAppearChance.m_fAppearChance << " ), ";
        }
        std::cout << std::endl;
    }

    std::map< DWORD, KEscortInfo >::iterator mitMon = m_mapEscortMonsterInfo.begin();
    for ( ; mitMon != m_mapEscortMonsterInfo.end(); ++mitMon ) {
        START_LOG( clog, L"MonID: " << mitMon->second.m_dwMonsterID
                << ", MonHealth: " << mitMon->second.m_nMonsterHealth
                << ", TakeDamage: " << mitMon->second.m_nTakeDamage
                << ", InvincibleTime: " << mitMon->second.m_nInvincibleTime )  << END_LOG;
    }

    std::map< DWORD, KEscortRewardInfo >::iterator mitReward = m_mapEscortRewardInfo.begin();
    for ( ; mitReward != m_mapEscortRewardInfo.end(); ++mitReward ) {
        START_LOG( clog, L"MonsterID: " << mitReward->second.m_dwMonsterID
                << "\nAlive Reward:"
                << ", ExtraExp: " << mitReward->second.m_kAliveReward.m_fExtraExp
                << ", ExtraGp: " << mitReward->second.m_kAliveReward.m_fExtraGp
                << ", ClearRewardListSize: " << mitReward->second.m_kAliveReward.m_vecKairoClearReward.size()
                << ", ExtraRewardListSize: " << mitReward->second.m_kAliveReward.m_vecKairoExtraReward.size()

                << "\nDead Reward:"
                << ", ExtraExp: " << mitReward->second.m_kDeadReward.m_fExtraExp
                << ", ExtraGp: " << mitReward->second.m_kDeadReward.m_fExtraGp
                << ", ClearRewardListSize: " << mitReward->second.m_kDeadReward.m_vecKairoClearReward.size()
                << ", ExtraRewardListSize: " << mitReward->second.m_kDeadReward.m_vecKairoExtraReward.size() )  << END_LOG;
    }

}

bool KEscort::IsValidUserLevel( IN PAIR_DWORD& prKey_, IN DWORD dwlevel_ )
{
    KLocker lock( m_csEscort );

    if ( m_mapEscortRewardInfo.empty() )
        return false;

    std::map< PAIR_DWORD, KEscortMapInfo >::iterator mit = m_mapEscortMapInfo.find( prKey_ );
    if ( mit != m_mapEscortMapInfo.end() ) {    // 호위 이벤트 던전인 경우
        return ( dwlevel_ >= mit->second.m_prLevelInfo.first && dwlevel_ <= mit->second.m_prLevelInfo.second );
    }
    else {
        return false;
    }

}

bool KEscort::IsHaveRewardInfo( IN DWORD dwGameModeID_, IN DWORD dwDifficulty_, IN DWORD dwMonID_ )
{
    PAIR_DWORD prKey = std::make_pair( dwGameModeID_, dwDifficulty_ );
    std::map< DWORD, KEscortRewardInfo >::iterator mitReward;
    
	std::map< PAIR_DWORD, KEscortMapInfo >::iterator mit = m_mapEscortMapInfo.find( prKey );
    if ( mit == m_mapEscortMapInfo.end() ) {
        START_LOG( clog, L"호위 이벤트 보상 정보를 찾는 중 모드 정보를 찾을 수 없음. MonsterID: " )
            << BUILD_LOG( dwGameModeID_ )
            << BUILD_LOG( dwDifficulty_ )
            << BUILD_LOG( dwMonID_ )
            << END_LOG;
        return false;
    }
	
	mitReward = m_mapEscortRewardInfo.find( dwMonID_ );
    if ( mitReward == m_mapEscortRewardInfo.end() ) {
        START_LOG( clog, L"호위 이벤트 보상 정보를 찾을 수 없음. MonsterID: " << dwMonID_ ) << END_LOG;
        return false;
    }
    return true;
}

bool KEscort::GetAliveReward( IN DWORD dwMonID_, OUT KEscortReward& kAliveReward_ )
{
    std::map< DWORD, KEscortRewardInfo >::iterator mitReward;

    KLocker lock( m_csEscort );

    mitReward = m_mapEscortRewardInfo.find( dwMonID_ );
    if ( mitReward == m_mapEscortRewardInfo.end() ) {
        START_LOG( clog, L"호위 이벤트 ALIVE 보상 정보를 찾을 수 없음. MonsterID: " << dwMonID_ ) << END_LOG;
        return false;
    }
    kAliveReward_ = mitReward->second.m_kAliveReward;

    return true;
}

bool KEscort::GetDeadReward( IN DWORD dwMonID_, OUT KEscortReward& kDeadReward_ )
{
    std::map< DWORD, KEscortRewardInfo >::iterator mitReward;

    KLocker lock( m_csEscort );

    mitReward = m_mapEscortRewardInfo.find( dwMonID_ );
    if ( mitReward == m_mapEscortRewardInfo.end() ) {
        START_LOG( clog, L"호위 이벤트 DEAD 보상 정보를 찾을 수 없음. MonsterID: " << dwMonID_ ) << END_LOG;
        return false;
    }
    kDeadReward_ = mitReward->second.m_kDeadReward;

    return true;
}

bool KEscort::IsHaveEscortInfo( IN PAIR_DWORD& prKey_, OUT DWORD& dwEscortMonID_ )
{
    dwEscortMonID_ = 0;
    std::map< PAIR_DWORD, KEscortMapInfo >::iterator mitMap;

    KLocker lock( m_csEscort );

    mitMap = m_mapEscortMapInfo.find( prKey_ );
    if ( mitMap != m_mapEscortMapInfo.end() ) {    // 해당 던전에 호위 이벤트가 있는 경우
        // 등장 확률을 먼저 확인
        KLottery kLottery;

        BOOST_FOREACH( KEscortMonAppearChance& rEscortMonAppearChance, mitMap->second.m_vecAppearChances ) {
            kLottery.AddCase( rEscortMonAppearChance.m_dwMonsterID, rEscortMonAppearChance.m_fAppearChance );
        }

        int nDecision = kLottery.Decision();
        if ( nDecision == KLottery::CASE_BLANK ) {
            return false;
        }

        std::map< DWORD, KEscortInfo >::iterator mitMonster;
        mitMonster = m_mapEscortMonsterInfo.find( static_cast< DWORD >( nDecision ) );   // 몬스터 정보까지 존재하는지 확인함

        if ( mitMonster != m_mapEscortMonsterInfo.end() ) {
            dwEscortMonID_ = nDecision;

            return true;
        }

        return false;
    }
    else {
        return false;
    }
}

bool KEscort::LoadEscortMonAppearChanceInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KEscortMonAppearChance >& vecAppearChances_ )
{
    vecAppearChances_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KEscortMonAppearChance kEscortMonAppearChance;
        kEscortMonAppearChance.m_dwMonsterID = 0;
        kEscortMonAppearChance.m_fAppearChance = 0;

        _JIF( kLuaMng_.GetValue( 1, kEscortMonAppearChance.m_dwMonsterID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kEscortMonAppearChance.m_fAppearChance ) == S_OK, return false );

        vecAppearChances_.push_back( kEscortMonAppearChance );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KEscort::IsEscortMonster( IN const DWORD dwMonID_ )
{
    KLocker lock( m_csEscort );

    std::map< DWORD, KEscortInfo >::iterator mitEscortMonsterInfo = m_mapEscortMonsterInfo.find( dwMonID_ );

    return ( mitEscortMonsterInfo != m_mapEscortMonsterInfo.end() );
}
