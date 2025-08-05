#include "GSSimLayer.h"
#include "UserEvent.h"
#include "AccumulateAttendance.h"
#include <dbg/dbg.hpp >
#include <kncutil.h>
#include "Log4.h"
#include "Lua/KLuaManager.h"

ImplementSingleton( KAccumulateAttendanceManager );
ImplOstmOperatorW2A( KAccumulateAttendanceManager );
NiImplementRootRTTI( KAccumulateAttendanceManager );
ImplToStringW( KAccumulateAttendanceManager )
{
    KLocker lock( m_csAccumulateAttendance );

    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_nVersion )
        << TOSTRINGW( m_bUseImmediatelyRegisterMission )
        << TOSTRINGW( m_mapCumulativeAttendanceRewardList.size() );
}

KAccumulateAttendanceManager::KAccumulateAttendanceManager(void)
:m_tmBegin(0)
,m_tmEnd(0)
,m_nVersion(0)
,m_bUseImmediatelyRegisterMission(false)
,m_nClientUIType(0)
{
    m_mapCumulativeAttendanceRewardList.clear();
}

KAccumulateAttendanceManager::~KAccumulateAttendanceManager(void)
{
}

bool KAccumulateAttendanceManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KAccumulateAttendanceManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitAccumulateAttendance.lua";
    strScript_ = strScriptName;

    bool bUseImmediatelyRegisterMission = false; // 보상 즉시 미션 등록 여부
    int nClientUIType = 0;
    std::map<int, VEC_REWARD> mapCumulativeAttendanceRewardList; // 누적 출석 아이템 정보

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "UseImmediatelyRegisterMission", bUseImmediatelyRegisterMission ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ClientUIType", nClientUIType ) == S_OK, return false );
    _JIF( LoadAttendanceRewardList( kLuaMng, std::string("CumulativeAttendanceRewardList"), mapCumulativeAttendanceRewardList ), return false );

    {
        KLocker lock( m_csAccumulateAttendance );
        m_bUseImmediatelyRegisterMission = bUseImmediatelyRegisterMission;
        m_nClientUIType = nClientUIType;
        m_mapCumulativeAttendanceRewardList.swap( mapCumulativeAttendanceRewardList );
    }

    START_LOG( cerr, L"누적 출석 스크립트 로드 완료." )
        << BUILD_LOG( m_bUseImmediatelyRegisterMission )
        << BUILD_LOG( m_nClientUIType )
        << BUILD_LOG( m_mapCumulativeAttendanceRewardList.size() )
        << END_LOG;

    return true;
}

bool KAccumulateAttendanceManager::LoadAttendanceRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, VEC_REWARD>& mapCumulativeAttendanceRewardList_ )
{
    mapCumulativeAttendanceRewardList_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true ) {
        int nAccumulateDay;
        VEC_REWARD vecRewardInfo;

        if( kLuaMng_.BeginTable( i++ ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "AccumulateDay", nAccumulateDay ) == S_OK, return false );
        _JIF( LoadRewardItemList( kLuaMng_, std::string("ItemList"), vecRewardInfo ), return false );

        if( !mapCumulativeAttendanceRewardList_.insert( std::make_pair( nAccumulateDay, vecRewardInfo ) ).second ) {
            START_LOG( cerr, L" 중복된 보상날짜 정보가 있음. nAccumulateDay : " << nAccumulateDay ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KAccumulateAttendanceManager::LoadRewardItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
{
    vecRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 1;
    while( true ) {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardInfo_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KAccumulateAttendanceManager::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csAccumulateAttendance );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KAccumulateAttendanceManager::UpdateInfoFromCenter( IN const time_t& tmBegin_, IN const time_t& tmEnd_, IN const int& nVersion_ )
{
    KLocker lock( m_csAccumulateAttendance );
    m_tmBegin = tmBegin_;
    m_tmEnd = tmEnd_;
    m_nVersion = nVersion_;
}

void KAccumulateAttendanceManager::GetAccumulateAttendanceScriptInfo( OUT KEVENT_ACCUMULATE_ATTENDANCE_INFO_ACK& kPacket_ )
{
    // 클라에 보내 줄 스크립트 데이터 세팅
    KLocker lock( m_csAccumulateAttendance );
    kPacket_.m_bUseImmediatelyRegisterMission = m_bUseImmediatelyRegisterMission;
    kPacket_.m_nClientUIType = m_nClientUIType;
    kPacket_.m_mapCumulativeAttendanceRewardList = m_mapCumulativeAttendanceRewardList;
}

bool KAccumulateAttendanceManager::CheckRewardDayValid( IN const int& nRewardReqDayIndex_ )
{
    KLocker lock( m_csAccumulateAttendance );
    // 스크립트에 설정 한 보상일 최대치
    int nMaxRewardableDayCount = m_mapCumulativeAttendanceRewardList.size();
    if ( nRewardReqDayIndex_ > nMaxRewardableDayCount || nRewardReqDayIndex_ <= 0 ) {
        return false;
    }

    return true;
}

bool KAccumulateAttendanceManager::GetReward( IN const int& nRewardReqDayIndex_, OUT VEC_REWARD& vecReward_ )
{
    KLocker lock( m_csAccumulateAttendance );
    std::map<int, VEC_REWARD>::iterator mitReward;
    mitReward = m_mapCumulativeAttendanceRewardList.find( nRewardReqDayIndex_ );
    if ( m_mapCumulativeAttendanceRewardList.end() == mitReward ) {
        START_LOG( cerr, L"누적 출석. 보상을 요청 한 날짜가 유효하지 않음" )
            << BUILD_LOG( nRewardReqDayIndex_ )
            << BUILD_LOG( m_mapCumulativeAttendanceRewardList.size() )
            << END_LOG;

        return false;
    }

    vecReward_ = mitReward->second;

    return true;
}

bool KAccumulateAttendanceManager::GetAccumulateAttendanceUserData_FromDB( IN const std::string& strScript_ )
{
    DWORD dwTemp = 0;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_ACCUMULATE_ATTENDANCE_REQ, dwTemp );

    return true;
}

//////////////////////////////////////////////////////////////////////////
/////////////// ↓↓ 누적 출석 유저 데이터 객체 클래스↓↓ ///////////////
//////////////////////////////////////////////////////////////////////////
KAccumulateAttendanceUserData::KAccumulateAttendanceUserData(void)
:m_nCurrentAccumulateDay(0)
{
    m_vecRewardCompleteDay.clear();
}

KAccumulateAttendanceUserData::~KAccumulateAttendanceUserData(void)
{
}

void KAccumulateAttendanceUserData::UpdateUserAccumulateAttendanceData( IN const KEVENT_ACCUMULATE_ATTENDANCE_ACK& kPacket_ )
{
    m_nCurrentAccumulateDay = kPacket_.m_nAccumulateCount;
    m_vecRewardCompleteDay = kPacket_.m_vecRewardCompleteDay;
}

void KAccumulateAttendanceUserData::GetUserAccumulateAttendanceData( OUT KEVENT_ACCUMULATE_ATTENDANCE_ACK& kPacket_ )
{
    kPacket_.m_nAccumulateCount = m_nCurrentAccumulateDay;
    kPacket_.m_vecRewardCompleteDay = m_vecRewardCompleteDay;
}

void KAccumulateAttendanceUserData::UpdateUserAttendanceRewardCompleteData( IN const std::vector<int>& vecRewardComplete_ )
{
    m_vecRewardCompleteDay = vecRewardComplete_;
}

bool KAccumulateAttendanceUserData::IsRewardCompleteDay( IN const int& nRewardReqDayIndex_ )
{
    std::vector<int>::iterator vitRewardCompleteDay;
    // 클라에서 요청한 날짜가 이미 보상 지급 완료한 날짜인지 확인
    vitRewardCompleteDay = std::find( m_vecRewardCompleteDay.begin(), m_vecRewardCompleteDay.end(), nRewardReqDayIndex_ );
    if ( m_vecRewardCompleteDay.end() != vitRewardCompleteDay ) {
        return true;
    }

    // 보상을 받지 않은 날짜이다
    return false;
}