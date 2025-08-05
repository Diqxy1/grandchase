#include "GSSimLayer.h"
#include "VitalityManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include "UserEvent.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

ImplementSingleton( KVitalityManager );
ImplOstmOperatorW2A( KVitalityManager );
NiImplementRootRTTI( KVitalityManager );

KVitalityManager::KVitalityManager(void)
: m_bUseVitalitySystem( false )
, m_kOneMinutesTimer( 60 * 1000 )
, m_tmNextInitialize( 0 )
, m_nVitalityForFirstCharacterCreation( 500 )
, m_nVitalityForCharacterCreation( 250 )
, m_nMaxVitalityPerDay( 250 )
, m_nVitalityPerRechargePoint( 10 )
, m_kVitalityResetSpanTime( 0, 6, 0, 0 ) // 매일 오전 6시. 스크립트에서는 설정받지 않는다

, uint32( 1, UINT_MAX )
, die( rng, uint32 )
{
    m_mapVitalityCheckDungeons.clear();

    rng.seed( ::GetTickCount() );
}

KVitalityManager::~KVitalityManager(void)
{
}

ImplToStringW( KVitalityManager )
{
    KLocker lock( m_csVitality );

    return START_TOSTRINGW
        << TOSTRINGW( m_bUseVitalitySystem )
        << TOSTRINGWt( CTime( m_tmNextInitialize ) )
        << TOSTRINGW( m_nMaxVitalityPerDay )
        << TOSTRINGW( m_nVitalityPerRechargePoint )
        << TOSTRINGW( m_kVitalityResetSpanTime.GetHours() )
        << TOSTRINGW( m_kVitalityResetSpanTime.GetMinutes() )
        << TOSTRINGW( m_mapVitalityCheckDungeons.size() );
}

bool KVitalityManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KVitalityManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitVitalityManager.lua";
    strScript_ = strScriptName;

    bool bUseVitalitySystem = false;
    int nVitalityForFirstCharacterCreation = 0;
    int nVitalityForCharacterCreation = 0;
    int nMaxVitalityPerDay = 0;
    int nVitalityPerRechargePoint = 0;
    std::map< PAIR_INT, KVitalityCheckDungeon > mapVitalityCheckDungeons;
    mapVitalityCheckDungeons.clear();

    _JIF( kLuaMng.DoFile( "Enum.stg" ) == S_OK, return false );
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "UseVitalitySystem", bUseVitalitySystem ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "VitalityForFirstCharacterCreation", nVitalityForFirstCharacterCreation ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "VitalityForCharacterCreation", nVitalityForCharacterCreation ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxVitalityPerDay", nMaxVitalityPerDay ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "VitalityPerRechargePoint", nVitalityPerRechargePoint ) == S_OK, return false );

    _JIF( LoadVitalityCheckDungeon( kLuaMng, std::string( "VitalityCheckDungeon" ), mapVitalityCheckDungeons ), return false );

    {
        KLocker lock( m_csVitality );

        m_bUseVitalitySystem = bUseVitalitySystem;
        m_nVitalityForFirstCharacterCreation = nVitalityForFirstCharacterCreation;
        m_nVitalityForCharacterCreation = nVitalityForCharacterCreation;
        m_nMaxVitalityPerDay = nMaxVitalityPerDay;
        m_nVitalityPerRechargePoint = nVitalityPerRechargePoint;
        m_mapVitalityCheckDungeons.swap( mapVitalityCheckDungeons );

        CalcNextInitializeTime();
    }

    START_LOG( cerr, L"Vitality Manager script loaded." ) << END_LOG;

    return true;
}

bool KVitalityManager::LoadVitalityCheckDungeon( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_INT, KVitalityCheckDungeon >& mapVitalityCheckDungeons_ )
{
    mapVitalityCheckDungeons_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KVitalityCheckDungeon kVitalityCheckDungeon;

        _JIF( kLuaMng_.GetValue( "ModeID", kVitalityCheckDungeon.m_nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Difficulty", kVitalityCheckDungeon.m_nDifficulty ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DeductVitality", kVitalityCheckDungeon.m_nDeductVitality ) == S_OK, return false );

        _JIF( LoadRatioNoVitality( kLuaMng_, std::string( "RatioNoVitality" ), kVitalityCheckDungeon ), return false );

        PAIR_INT prModeDifficulty( kVitalityCheckDungeon.m_nModeID, kVitalityCheckDungeon.m_nDifficulty );
        if ( false == mapVitalityCheckDungeons_.insert( std::make_pair( prModeDifficulty, kVitalityCheckDungeon ) ).second ) {
            START_LOG( cerr, L"중복된 ( ModeID, Difficulty )가 있습니다. ModeID: " << prModeDifficulty.first << ", Difficulty: " << prModeDifficulty.second ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KVitalityManager::LoadRatioNoVitality( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KVitalityCheckDungeon& kVitalityCheckDungeon_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "EXP", kVitalityCheckDungeon_.m_nExpGainRatioNoVitality ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "GP", kVitalityCheckDungeon_.m_nGpGainRatioNoVitality ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "ItemDrop", kVitalityCheckDungeon_.m_nItemDropRatioNoVitality ) == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KVitalityManager::IsRun()
{
    KLocker lock( m_csVitality );

    return m_bUseVitalitySystem;
}

int KVitalityManager::GetVitalityForFirstCharacterCreation()
{
    KLocker lock( m_csVitality );

    return m_nVitalityForFirstCharacterCreation;
}

int KVitalityManager::GetVitalityForCharacterCreation()
{
    KLocker lock( m_csVitality );

    return m_nVitalityForCharacterCreation;
}

int KVitalityManager::GetMaxVitalityPerDay()
{
    KLocker lock( m_csVitality );

    return m_nMaxVitalityPerDay;
}

int KVitalityManager::GetVitalityPerRechargePoint()
{
    KLocker lock( m_csVitality );

    return m_nVitalityPerRechargePoint;
}

bool KVitalityManager::IsVitalityCheckDungeon( IN const int nGameMode_, const IN int nDifficulty_ )
{
    PAIR_INT prKey( nGameMode_, nDifficulty_ );

    KLocker lock( m_csVitality );

    std::map< PAIR_INT, KVitalityCheckDungeon >::iterator mitVitalityCheckDungeons;
    mitVitalityCheckDungeons = m_mapVitalityCheckDungeons.find( prKey );
    if ( mitVitalityCheckDungeons != m_mapVitalityCheckDungeons.end() ) {
        return true;
    }

    return false;
}

bool KVitalityManager::GetDeductVitality( IN const int nGameMode_, const IN int nDifficulty_, OUT int& nDeductVitality_ )
{
    nDeductVitality_ = 0;
    PAIR_INT prKey( nGameMode_, nDifficulty_ );

    KLocker lock( m_csVitality );

    std::map< PAIR_INT, KVitalityCheckDungeon >::iterator mitVitalityCheckDungeons;
    mitVitalityCheckDungeons = m_mapVitalityCheckDungeons.find( prKey );
    if ( mitVitalityCheckDungeons == m_mapVitalityCheckDungeons.end() ) {
        return false;
    }

    nDeductVitality_ = mitVitalityCheckDungeons->second.m_nDeductVitality;

    return true;
}

int KVitalityManager::GetExpGainRatioNoVitality( IN const int nGameMode_, const IN int nDifficulty_ )
{
    PAIR_INT prKey( nGameMode_, nDifficulty_ );

    KLocker lock( m_csVitality );

    std::map< PAIR_INT, KVitalityCheckDungeon >::iterator mitVitalityCheckDungeons;
    mitVitalityCheckDungeons = m_mapVitalityCheckDungeons.find( prKey );
    if ( mitVitalityCheckDungeons != m_mapVitalityCheckDungeons.end() ) {
        return mitVitalityCheckDungeons->second.m_nExpGainRatioNoVitality;
    }

    return 100;
}

int KVitalityManager::GetGpGainRatioNoVitality( IN const int nGameMode_, const IN int nDifficulty_ )
{
    PAIR_INT prKey( nGameMode_, nDifficulty_ );

    KLocker lock( m_csVitality );

    std::map< PAIR_INT, KVitalityCheckDungeon >::iterator mitVitalityCheckDungeons;
    mitVitalityCheckDungeons = m_mapVitalityCheckDungeons.find( prKey );
    if ( mitVitalityCheckDungeons != m_mapVitalityCheckDungeons.end() ) {
        return mitVitalityCheckDungeons->second.m_nGpGainRatioNoVitality;
    }

    return 100;
}

int KVitalityManager::GetItemDropRatioNoVitality( IN const int nGameMode_, const IN int nDifficulty_ )
{
    PAIR_INT prKey( nGameMode_, nDifficulty_ );

    KLocker lock( m_csVitality );

    std::map< PAIR_INT, KVitalityCheckDungeon >::iterator mitVitalityCheckDungeons;
    mitVitalityCheckDungeons = m_mapVitalityCheckDungeons.find( prKey );
    if ( mitVitalityCheckDungeons != m_mapVitalityCheckDungeons.end() ) {
        return mitVitalityCheckDungeons->second.m_nItemDropRatioNoVitality;
    }

    return 100;
}

void KVitalityManager::Tick()
{
    if ( false == IsRun() ) {
        return;
    }

    if ( false == m_kOneMinutesTimer.CheckTime() ) {
        return;
    }

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csVitality );

    // 초기화 시간이 되었는지 확인한다
    if ( m_tmNextInitialize < tmCurrent ) {
        // 다음 초기화 시간을 재계산하고 활력을 리셋한다
        CalcNextInitializeTime();

        bool bNeedForceInit = true;
        SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::DB_EVENT_VITALITY_INFO_REQ, bNeedForceInit ); // 활력 및 재충전 포인트 초기화 요청
    }
}

void KVitalityManager::UseVitalitySystem( IN bool bFlag_ )
{
    KLocker lock( m_csVitality );

    // 활력 시스템 사용으로 변경 시 접속중인 모든 유저들의 활력 정보를 DB에서 가져오도록 한다
    if ( true == bFlag_ && false == m_bUseVitalitySystem ) {
        bool bNeedForceInit = false;
        SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::DB_EVENT_VITALITY_INFO_REQ, bNeedForceInit );
    }

    m_bUseVitalitySystem = bFlag_;
}

void KVitalityManager::CalcNextInitializeTime()
{
    // lock은 함수 바깥에서 건다
    CTime kCurrentTime( CTime::GetCurrentTime() );
    CTime kNextInitializeTime( kCurrentTime.GetYear(), kCurrentTime.GetMonth(), kCurrentTime.GetDay(), 0, 0, 0 );
    kNextInitializeTime += m_kVitalityResetSpanTime; // 오늘 초기화 시간

    // 오늘의 초기화 시간이 이미 지났다면 내일의 초기화 시간을 계산한다
    time_t tmNextInitialize = KncUtil::TimeToInt( kNextInitializeTime );
    time_t tmCurrent = KncUtil::TimeToInt( kCurrentTime );
    if ( tmNextInitialize < tmCurrent ) {
        tmNextInitialize = KncUtil::TimeToInt( kNextInitializeTime + CTimeSpan( 1, 0, 0, 0 ) );
    }

    m_tmNextInitialize = tmNextInitialize;
}

void KVitalityManager::GetVitalityResetSpanTime( OUT CTimeSpan& kVitalityResetSpanTime_ )
{
    KLocker lock( m_csVitality );

    kVitalityResetSpanTime_ = m_kVitalityResetSpanTime;
}

void KVitalityManager::ApplyNoVitalityRatioToItemDropList( IN const KGameInfo& kGameInfo_, OUT std::vector< KGameDropElement >& vecMonsterDrop_ )
{
    // 잔여 활력이 0인 유저 리스트 만들고
    std::vector< DWORD > vecNoVitalityUsers;
    vecNoVitalityUsers.clear();
    std::map< PAIR_DWORD_INT, int >::const_iterator mitRemainVitalityOnStartGame = kGameInfo_.m_mapRemainVitalityOnStartGame.begin();
    for ( ; mitRemainVitalityOnStartGame != kGameInfo_.m_mapRemainVitalityOnStartGame.end(); ++mitRemainVitalityOnStartGame ) {
        if ( 0 >= mitRemainVitalityOnStartGame->second ) {
            vecNoVitalityUsers.push_back( mitRemainVitalityOnStartGame->first.first );
        }
    }
    if ( true == vecNoVitalityUsers.empty() ) {
        return;
    }

    // 잔여 활력이 0인 유저들에게 적용할 아이템 드랍 비율을 가져온다
    float fItemDropRatioNoVitality = GetItemDropRatioNoVitality( kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty ) / 100.f;

    // 활력이 0인 유저에게 할당된 드랍 아이템들에 위 드랍 비율을 적용한다(드랍 리스트에서 빼낼 리스트를 만든다)
    std::vector< DWORD > vecRemoveDropElements;
    vecRemoveDropElements.clear();
    BOOST_FOREACH( DWORD& rUserUID, vecNoVitalityUsers ) {
        BOOST_FOREACH( KGameDropElement& rGameDropElements, vecMonsterDrop_ ) {
            if ( rGameDropElements.m_dwUserUID == rUserUID ) {
                // 드랍 O: 랜덤 값 < 비율
                // 드랍 X: 랜덤 값 >= 비율
                if ( GetRatio() >= fItemDropRatioNoVitality ) {
                    vecRemoveDropElements.push_back( rGameDropElements.m_dwUID );
                }
            }
        }
    }

    // 드랍 리스트에 적용한다
    BOOST_FOREACH( DWORD& rRemoveUID, vecRemoveDropElements ) {
        std::vector< KGameDropElement >::iterator vitMonsterDrop;
        vitMonsterDrop = std::find_if( vecMonsterDrop_.begin(), vecMonsterDrop_.end(),
            boost::bind( &KGameDropElement::m_dwUID, _1 ) == rRemoveUID );

        vecMonsterDrop_.erase( vitMonsterDrop );
    }
}

void KVitalityManager::GetVitalityCheckDungeonInfoNot( OUT KEVENT_VITALITY_CHECK_DUNGEON_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csVitality );

    kPacket_ = m_mapVitalityCheckDungeons;
}

//=========================================================================================================================================================

KVitalityUserData::KVitalityUserData()
: m_nRechargePoint( 0 )
, m_nRechargePointCounter( 0 )
{
    m_mapRemainVitality.clear();
}

KVitalityUserData::~KVitalityUserData()
{
}

void KVitalityUserData::GetVitalityUserData( OUT KVitalityUpdateData& kVitalityUpdateData_ )
{
    kVitalityUpdateData_.m_nRechargePoint         = m_nRechargePoint;
    kVitalityUpdateData_.m_nRechargePointCounter  = m_nRechargePointCounter;
    kVitalityUpdateData_.m_mapRemainVitality      = m_mapRemainVitality;
}

bool KVitalityUserData::DecreaseVitalityOnPlay( IN const int nCharType_, IN const int nVitalityDecreaseAmount_ )
{
    /* true: 활력 정보가 변경됨
       false: 활력 정보가 변경되지 않음 */
    if ( 0 == nVitalityDecreaseAmount_ ) {
        return false;
    }

    std::map< int, int >::iterator mitRemainVitality;
    mitRemainVitality = m_mapRemainVitality.find( nCharType_ );
    if ( mitRemainVitality == m_mapRemainVitality.end() ) {
        return false;
    }

    int& rRemainVitality = mitRemainVitality->second;

    // 남아있는 활력이 감소할 수치보다 적을 경우 활력을 차감하지 않는다
    if ( rRemainVitality < nVitalityDecreaseAmount_ ) {
        return false;
    }

    // 해당 캐릭터의 활력을 감소시킨다
    rRemainVitality = rRemainVitality - nVitalityDecreaseAmount_;
    rRemainVitality = std::max< int >( 0, rRemainVitality );

    // 감소시킨 활력으로 재충전 카운터를 갱신한다
    m_nRechargePointCounter = m_nRechargePointCounter + nVitalityDecreaseAmount_;
    int nVitalityPerRechargePoint = SiKVitalityManager()->GetVitalityPerRechargePoint();
    if ( nVitalityPerRechargePoint <= m_nRechargePointCounter ) {
        m_nRechargePointCounter = m_nRechargePointCounter - nVitalityPerRechargePoint;
        m_nRechargePoint += 1;
        m_nRechargePoint = std::min< int >( SiKVitalityManager()->GetMaxVitalityPerDay(), m_nRechargePoint );
    }

    return true;
}

int KVitalityUserData::GetVitality( IN const int nCharType_ )
{
    std::map< int, int >::iterator mitRemainVitality;
    mitRemainVitality = m_mapRemainVitality.find( nCharType_ );
    if ( mitRemainVitality != m_mapRemainVitality.end() ) {
        return mitRemainVitality->second;
    }

    return 0;
}

bool KVitalityUserData::SetVitality( IN const int nCharType_, IN const int nVitality_ )
{
    m_mapRemainVitality[ nCharType_ ] = nVitality_;

    return true;
}

bool KVitalityUserData::IsRemainVitality( IN const int nCharType_ )
{
    std::map< int, int >::iterator mitRemainVitality;
    mitRemainVitality = m_mapRemainVitality.find( nCharType_ );
    if ( mitRemainVitality != m_mapRemainVitality.end() ) {
        if ( 0 < mitRemainVitality->second ) {
            return true;
        }
    }

    return false;
}

bool KVitalityUserData::RechargeVitality( IN const int nCharType_ )
{
    std::map< int, int >::iterator mitRemainVitality;
    mitRemainVitality = m_mapRemainVitality.find( nCharType_ );
    if ( mitRemainVitality == m_mapRemainVitality.end() ) {
        START_LOG( clog, L"활력 충전요청 중 해당 캐릭터의 활력 유저 데이터가 없음. nCharType: " << nCharType_ ) << END_LOG;

        return false;
    }

    int& rRemainVitality = mitRemainVitality->second;
    int nMaxVitalityPerDay = SiKVitalityManager()->GetMaxVitalityPerDay();

    // 잔여 활력이 하루 최대 활력 수치를 넘으면 충전하지 못한다
    if ( rRemainVitality >= nMaxVitalityPerDay ) {
        return false;
    }

    rRemainVitality += m_nRechargePoint;
    if ( rRemainVitality > nMaxVitalityPerDay ) {
        m_nRechargePoint = rRemainVitality - nMaxVitalityPerDay;
        rRemainVitality = nMaxVitalityPerDay;
    }
    else {
        m_nRechargePoint = 0;
    }

    return true;

}
