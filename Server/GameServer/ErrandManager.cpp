#include "GSSimLayer.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include "ErrandManager.h"
#include "AvoidOverlappingLottery.h"
#include "UserEvent.h"

ImplementSingleton( KErrandManager );
ImplOstmOperatorW2A( KErrandManager );
NiImplementRootRTTI( KErrandManager );

ImplToStringW( KErrandManager )
{
    KLocker lock( m_csErrand );

    return START_TOSTRINGW
        << TOSTRINGW( m_nVersion )
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_nPlayTime )
        << TOSTRINGW( m_nDelayTime )
        << TOSTRINGW( m_nTimeIncreaseRate )
        << TOSTRINGW( m_nMaxStage )
        << TOSTRINGW( m_nMarryAppearInterval )
        << TOSTRINGW( m_nMarryKeepAliveTime )
        << TOSTRINGW( m_mapErrandStageInfo.size() )
        << TOSTRINGW( m_vecRatioRewardInfo.size() )
        << TOSTRINGW( m_vecSpecialRewardInfo.size() );
}

KErrandManager::KErrandManager(void)
: m_nVersion( 1 )
, m_tmBegin( 0 )
, m_tmEnd( 0 )
, m_nPlayTime( 0 )
, m_nDelayTime( 60 * 1000 )
, m_nTimeIncreaseRate( 20 )
, m_nMaxStage( 9 )
, m_nMarryAppearInterval( 1000 * 5 )
, m_nMarryKeepAliveTime( 1000 * 2 )
{
    m_mapErrandStageInfo.clear();
    m_vecRatioRewardInfo.clear();
    m_vecSpecialRewardInfo.clear();
}

KErrandManager::~KErrandManager(void)
{
}


bool KErrandManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KErrandManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitErrand.lua";
    strScript_ = strScriptName;

    int    nVersion;
    time_t tmBegin;
    time_t tmEnd;

    int nPlayTime; // 한 판 플레이 하는 시간
    int nDelayTime; // 다음 플레이 까지 기다려야 하는 시간
    int nTimeIncreaseRate; // 심부름 한 번 성공 시 증가하는 시간 비율
    int nMaxStageNum; // 최대 스테이지 숫자(심부름 성공 한계치)
    int nMarryAppearInterval; // 메리 출현 주기
    int nMarryKeepAliveTime; // 메리 출현 유지 시간

    std::map<int, KErrandStageInfo>  mapErrandStageInfo;
    std::vector< KRatioRewardInfo > vecRewardInfo;
    std::vector< std::pair< KDropItemInfo, int > > vecSpecialRewardInfo;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "Version", nVersion ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string( "BeginTime" ), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string( "EndTime" ), tmEnd ), return false );

    _JIF( kLuaMng.GetValue( "PlayTime", nPlayTime ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DelayTime", nDelayTime ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TimeIncreaseRate", nTimeIncreaseRate ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxStageNum", nMaxStageNum ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MarryAppearInterval", nMarryAppearInterval ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MarryKeepAliveTime", nMarryKeepAliveTime ) == S_OK, return false );

    _JIF( LoadStageInfo( kLuaMng, std::string("StageInfo"), mapErrandStageInfo ), return false );
    _JIF( LoadRewardInfo( kLuaMng, std::string("RewardInfo"), vecRewardInfo ), return false );
    _JIF( LoadSpecialRewardInfo( kLuaMng, std::string("SpecialRewardInfo"), vecSpecialRewardInfo ), return false );

    {
        KLocker lock( m_csErrand );

        m_nVersion = nVersion;
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_nPlayTime = nPlayTime;
        m_nDelayTime = nDelayTime;
        m_nTimeIncreaseRate = nTimeIncreaseRate;
        m_nMaxStage = nMaxStageNum;
        m_nMarryAppearInterval = nMarryAppearInterval;
        m_nMarryKeepAliveTime = nMarryKeepAliveTime;
        
        m_mapErrandStageInfo.swap( mapErrandStageInfo );
        m_vecRatioRewardInfo.swap( vecRewardInfo );
        m_vecSpecialRewardInfo.swap( vecSpecialRewardInfo );

        START_LOG( cerr, L"심부름왕 스크립트 로드 완료." )
            << BUILD_LOG( m_nVersion )
            << BUILD_LOGtm( CTime( m_tmBegin ) )
            << BUILD_LOGtm( CTime( m_tmEnd ) )
            << BUILD_LOG( m_nPlayTime )
            << BUILD_LOG( m_nDelayTime )
            << BUILD_LOG( m_nTimeIncreaseRate )
            << BUILD_LOG( m_nMaxStage )
            << BUILD_LOG( nMarryAppearInterval )
            << BUILD_LOG( nMarryKeepAliveTime )
            << BUILD_LOG( m_mapErrandStageInfo.size() )
            << BUILD_LOG( m_vecRatioRewardInfo.size() )
            << BUILD_LOG( m_vecSpecialRewardInfo.size() )
            << END_LOG;
    }

    return true;
}

bool KErrandManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    if ( 0 == nYear || 0 == nMonth || 0 == nDay // 스크립트에서 0으로 설정하면 서버 죽음
        || nMonth > 12 || nDay > 31 ) { // 월, 일 최대값 검사
            START_LOG( cerr, L"심부름왕 이벤트 스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KErrandManager::LoadStageInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, KErrandStageInfo >& mapErrandStageInfo_ )
{
    mapErrandStageInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KErrandStageInfo kStageInfo;

        _JIF( kLuaMng_.GetValue( "StageNum", kStageInfo.m_nStageNum ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "BuyListMinCount", kStageInfo.m_nBuyListMinCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "BuyListMaxCount", kStageInfo.m_nBuyListMaxCount ) == S_OK, return false );

        _JIF( LoadBuyList( kLuaMng_, std::string("BuyList"), kStageInfo.m_vecBuyInfoList ), return false );

        if ( false == mapErrandStageInfo_.insert( std::make_pair( kStageInfo.m_nStageNum, kStageInfo ) ).second ) {
            START_LOG( cerr, L"심부름왕 이벤트 스크립트에 중복된 스테이지 정보가 있습니다." ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KErrandManager::LoadBuyList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KErrandBuyInfo >& vecBuyInfoList_ )
{
    vecBuyInfoList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KErrandBuyInfo kBuyInfo;

        _JIF( kLuaMng_.GetValue( 1, kBuyInfo.m_nType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kBuyInfo.m_nMinCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kBuyInfo.m_nMaxCount ) == S_OK, return false );

        vecBuyInfoList_.push_back( kBuyInfo );        

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KErrandManager::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KRatioRewardInfo >& vecRatioRewardInfo_ )
{
    vecRatioRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KRatioRewardInfo kRewardInfo;

        _JIF( kLuaMng_.GetValue( 1, kRewardInfo.m_dwItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kRewardInfo.m_nCount ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kRewardInfo.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kRewardInfo.m_nRatio ) == S_OK, return false );

        vecRatioRewardInfo_.push_back( kRewardInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KErrandManager::LoadSpecialRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< std::pair< KDropItemInfo, int > >& vecSpecialRewardInfo )
{
    vecSpecialRewardInfo.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KDropItemInfo kRewardInfo;
        int nProvideCount = 0;

        _JIF( kLuaMng_.GetValue( 1, kRewardInfo.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, kRewardInfo.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kRewardInfo.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nProvideCount ) == S_OK, return false );
        if ( 0 == nProvideCount ) {
            START_LOG( cerr, L"SpecialRewardInfo 테이블 최대 지급 수량 설정 오류. 최대 지급 수량 : " << nProvideCount ) << END_LOG;
        }

        vecSpecialRewardInfo.push_back( std::make_pair( kRewardInfo, nProvideCount ) );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KErrandManager::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csErrand );

    return ( m_tmBegin <= tmCurrent && tmCurrent <= m_tmEnd );
}

bool KErrandManager::GetRewardItem( IN int& nSuccessCount_, IN OUT std::vector< PAIR_INT >& vecReceivedSpecialReward_, OUT KDB_EVENT_ERRAND_REWARD_REQ& kDBPacket_ )
{
    // 비율에 따른 비중복 랜덤 보상을 획득
    if ( false == SiKAvoidOverlappingLottery()->GetNonOverlappedRandomRewardByRatio( m_vecRatioRewardInfo, nSuccessCount_, kDBPacket_.m_vecReward ) ) {
        return false;
    }
    // 칭찬 횟수가 최대 수치를 만족하는지 확인
    if ( false == CheckFinalSuccess( nSuccessCount_ ) ) {
        // 이 부분은 만족 안해도 위에서 보상 획득 성공했으므로 true
        return true;
    }

    KLocker lock( m_csErrand );

    std::vector< std::pair< KDropItemInfo, int > >::iterator vitSpecialReward;
    std::vector< PAIR_INT >::iterator vitReceived;
    kDBPacket_.m_vecSpecialReward.clear();

    // 받은게 하나라도 있는지 체크
    if ( false == vecReceivedSpecialReward_.empty() ) {
        for ( vitSpecialReward = m_vecSpecialRewardInfo.begin(); vitSpecialReward != m_vecSpecialRewardInfo.end(); vitSpecialReward++ ) {
            // 마지막으로 받은 아이템ID로 스페셜 컨테이너에서 찾는다
            PAIR_INT prReceived = vecReceivedSpecialReward_.back();
            if ( prReceived.first == vitSpecialReward->first.m_ItemID ) {
                // -1로 설정한 아이템인지 확인(-1이면 계속 이 아이템만 지급)
                if ( -1 == vitSpecialReward->second ) {
                    vitReceived = std::find( vecReceivedSpecialReward_.begin(), vecReceivedSpecialReward_.end(), prReceived );
                    if ( vecReceivedSpecialReward_.end() != vitReceived ) {
                        ProvideCurrentItemAgainForSpecialReward( vitSpecialReward->first, vitReceived, kDBPacket_ );
                    }
                    break;
                }
                // 지급 받은 아이템이 최대 지급 수량 제한에 걸리는지 확인
                else if ( prReceived.second >= vitSpecialReward->second ) {
                    if ( m_vecSpecialRewardInfo.end() != ( vitSpecialReward+1 ) ) {
                        ProvideNewItemForSpecialReward( ( vitSpecialReward+1 )->first, vecReceivedSpecialReward_, kDBPacket_ );
                    }
                    break;
                }
                // -1이 아니고 최대 지급 수량 제한에 안걸리면 동일 아이템 지급
                else {
                    vitReceived = std::find( vecReceivedSpecialReward_.begin(), vecReceivedSpecialReward_.end(), prReceived );
                    if ( vecReceivedSpecialReward_.end() != vitReceived ) {
                        ProvideCurrentItemAgainForSpecialReward( vitSpecialReward->first, vitReceived, kDBPacket_ );
                    }
                    break;
                }
            }
        }
    }
    else { // 받은게 하나도 없다면 바로 추가
        vitSpecialReward = m_vecSpecialRewardInfo.begin();
        if ( vitSpecialReward != m_vecSpecialRewardInfo.end() ) {
            ProvideNewItemForSpecialReward( vitSpecialReward->first, vecReceivedSpecialReward_, kDBPacket_ );
        }
        else {
            START_LOG( cerr, L"심부름 이벤트 최종 보상 컨테이너가 비정상적임. m_vecSpecialRewardInfo.begin() 포인터가 NULL" )
                << BUILD_LOG( m_vecSpecialRewardInfo.size() )
                << END_LOG;
            return false;
        }
    }

    START_LOG( clog, L"심부름 최종 보상까지 획득 함." )
        << BUILD_LOG( kDBPacket_.m_nTotalReceivedCount )
        << BUILD_LOG( kDBPacket_.m_vecReward.size() )
        << BUILD_LOG( kDBPacket_.m_vecSpecialReward.size() )
        << END_LOG;

    return true;

}

void KErrandManager::GetClientInitInfo( OUT KEVENT_ERRAND_INIT_INFO_ACK& kPacket_ )
{
    std::vector< KRatioRewardInfo >::iterator vitRewardInfo;

    KLocker lock( m_csErrand );

    kPacket_.m_nPlayTime = m_nPlayTime;
    kPacket_.m_nDelayTime = m_nDelayTime;
    kPacket_.m_nTimeIncreaseRate = m_nTimeIncreaseRate;
    kPacket_.m_nMaxStage = m_nMaxStage;
    kPacket_.m_mapErrandStageInfo = m_mapErrandStageInfo;
    kPacket_.m_nMarryAppearInterval = m_nMarryAppearInterval;
    kPacket_.m_nMarryKeepAliveTime = m_nMarryKeepAliveTime;

    // 클라이언트 디스플레이용 보상 정보 컨테이너 재구성
    kPacket_.m_vecRewardItemForDisplay.clear();
    for( vitRewardInfo = m_vecRatioRewardInfo.begin(); vitRewardInfo != m_vecRatioRewardInfo.end(); vitRewardInfo++ ) {
        kPacket_.m_vecRewardItemForDisplay.push_back( vitRewardInfo->m_dwItemID );
    }
}

const bool KErrandManager::IsValidCount( IN int& nReqCount_ )
{
    KLocker lock( m_csErrand );

    if ( 0 >= nReqCount_ || m_nMaxStage < nReqCount_ ) {
        START_LOG( clog, L"심부름 미니게임 완료 후 요청 한 보상 개수가 유효하지 않다." )
            << BUILD_LOG( nReqCount_ )
            << END_LOG;
        return false;
    }

    return true;
}

const bool KErrandManager::CheckFinalSuccess( IN const int nSuccess_ )
{
    KLocker lock( m_csErrand );

    // 최대 칭찬(성공) 횟수를 만족하는지 확인
    if ( nSuccess_ == m_nMaxStage ) {
        return true;
    }

    return false;
}

void KErrandManager::ProvideNewItemForSpecialReward( IN const KDropItemInfo& kItemInfo_, IN OUT std::vector< PAIR_INT >& vecReceivedSpecialReward_, OUT KDB_EVENT_ERRAND_REWARD_REQ& kDBPacket_ )
{
    // 기획상으로 한번에 하나씩만 받기 때문에 매직넘버 1 사용할 변수
    int nRecvCount = 1;
    PAIR_INT prNewRecv;

    kDBPacket_.m_vecSpecialReward.push_back( kItemInfo_ );
    prNewRecv = PAIR_INT( kItemInfo_.m_ItemID, nRecvCount );
    vecReceivedSpecialReward_.push_back( prNewRecv );
    kDBPacket_.m_nTotalReceivedCount = prNewRecv.second;
}

void KErrandManager::ProvideCurrentItemAgainForSpecialReward( IN const KDropItemInfo& kItemInfo_, IN OUT std::vector< PAIR_INT >::iterator vitReceived_, OUT KDB_EVENT_ERRAND_REWARD_REQ& kDBPacket_ )
{
    // 기획상으로 한번에 하나씩만 받기 때문에 매직넘버 1 사용할 변수
    int nRecvCount = 1;

    kDBPacket_.m_vecSpecialReward.push_back( kItemInfo_ );
    vitReceived_->second += nRecvCount;
    vitReceived_->second = std::min< int > ( INT_MAX, vitReceived_->second );
    kDBPacket_.m_nTotalReceivedCount = vitReceived_->second;
}

bool KErrandManager::Get_Errand_UserData_FromDB( IN const std::string& strScript_ )
{
    DWORD dwTemp = 0;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::DB_EVENT_ERRAND_USER_INFO_REQ, dwTemp );

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Errand User Data Class
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

KErrandUserData::KErrandUserData(void)
: m_nRemainTime( 1000 * 60 )
, m_bPlayable( false )
, m_nTotalPlayCount( 0 )
, m_nTotalSuccessCount( 0 )
, m_bRewardEnableState( false )
{
    m_vecReceivedSpecialRewardInfo.clear();
}

KErrandUserData::~KErrandUserData(void)
{
}

bool KErrandUserData::CheckErrandPlayableStateUpdate()
{
    // 현재 플레이 가능한 상태라면 시간 체크가 필요 없다
    if ( true == m_bPlayable ) {
        return false;
    }

    // 남은 시간 감소 (m_kErrandTimer 설정 된 시간만큼 감소)
    m_nRemainTime -= (1000 * 10);
    m_nRemainTime = std::max<int>( INT_MIN, m_nRemainTime );

    // 남은 시간이 다 깎였을 때 플레이 가능하다고 알려줌
    if ( 0 >= m_nRemainTime ) {
        return true;
    }

    return false;
}

void KErrandUserData::ResetRemainTime()
{
    // 보상 지급 완료 후 호출한다. (보상 지급 완료 시점부터 10분 기다리면 플레이 가능)
    m_nRemainTime = SiKErrandManager()->GetDelayTime();
}

void KErrandUserData::GetUserDataForClient( OUT KEVENT_ERRAND_USER_INFO_ACK& kPacket_ )
{
    kPacket_.m_nRemainTime = m_nRemainTime;
    kPacket_.m_bPlayable = m_bPlayable;
}

void KErrandUserData::GetUserDataForDB( OUT KErrandUserInfoDataStruct& kUserInfoData_ )
{
    kUserInfoData_.m_nRemainTime = m_nRemainTime;
    kUserInfoData_.m_bPlayable = m_bPlayable;
    kUserInfoData_.m_nTotalPlayCount = m_nTotalPlayCount;
    kUserInfoData_.m_nTotalSuccessCount = m_nTotalSuccessCount;
}

void KErrandUserData::UpdateUserDateFromDB( IN const KErrandUserInfoDataStruct& kUserData )
{
    m_nRemainTime = kUserData.m_nRemainTime;
    m_bPlayable = kUserData.m_bPlayable;
    m_nTotalPlayCount = kUserData.m_nTotalPlayCount;
    m_nTotalSuccessCount = kUserData.m_nTotalSuccessCount;
    m_vecReceivedSpecialRewardInfo = kUserData.m_vecReceivedSpecialRewardInfo;

    if ( 0 >= m_nRemainTime && false == m_bPlayable ) {
        m_bPlayable = true;
    }
}

void KErrandUserData::IncreaseDataForLog( IN const int& nPlayCountDiff_, IN const int& nSuccessCountDiff_ )
{
    m_nTotalPlayCount += nPlayCountDiff_;
    m_nTotalPlayCount = std::min<int>( INT_MAX, m_nTotalPlayCount );

    m_nTotalSuccessCount += nSuccessCountDiff_;
    m_nTotalSuccessCount = std::min<int>( INT_MAX, m_nTotalSuccessCount );
}