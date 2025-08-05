#include "CenterSimLayer.h"
#include "CnBuffDonation.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h"
#include "Log4.h"
#include "GCEnum.h"

ImplementSingleton( KCnBuffDonation );
ImplOstmOperatorW2A( KCnBuffDonation );
NiImplementRootRTTI( KCnBuffDonation );

ImplToStringW( KCnBuffDonation )
{
    KLocker lock( m_csBuffDonation );
    return START_TOSTRINGW
        << TOSTRINGW( m_biActiveBuffPoint )
        << TOSTRINGW( m_dwBuffRetainTime )
        << TOSTRINGW( m_nDecreaseCount )
        << TOSTRINGW( m_dwType )
        << TOSTRINGW( m_vecSyncPointList.size() )
        << TOSTRINGW( m_biTotalDonationPoint )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_bBuffActive );
}

KCnBuffDonation::KCnBuffDonation(void)
:m_biActiveBuffPoint(0)
,m_dwBuffRetainTime(0)
,m_nDecreaseCount(0)
,m_dwType(1)
,m_biTotalDonationPoint(0)
,m_bBuffActive(false)
,m_kUpdateCheckTimer( 60 * 1000 )
{
    m_dwLastTick = ::GetTickCount();
    m_vecSyncPointList.clear();
    ResetSecondDataAtSyncPointVector();
}

KCnBuffDonation::~KCnBuffDonation(void)
{
}

bool KCnBuffDonation::LoadScript()
{
    KLuaManager kLuaMng;

    time_t tmBegin;
    __int64 biActiveBuffPoint;
    DWORD dwBuffRetainTime;
    int nDecreaseCount;
    DWORD dwType;
    std::vector< PAIR_INT_BOOL > vecSyncPointList;
    DWORD dwTickGap = 60 * 1000;

    _JIF( kLuaMng.DoFile( "CnBuffDonation.lua" ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( kLuaMng.GetValue( "ActiveBuffPoint", biActiveBuffPoint ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "BuffRetainTime", dwBuffRetainTime ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DonationPointDecreaseCount", nDecreaseCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TypeNumber", dwType ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );

    _JIF( LoadSyncPointListInfo( kLuaMng, std::string( "SyncPointList" ), vecSyncPointList ), return false );

    {
        KLocker lock( m_csBuffDonation );
        m_tmBegin = tmBegin;
        m_biActiveBuffPoint = biActiveBuffPoint;
        m_dwBuffRetainTime = dwBuffRetainTime;
        m_nDecreaseCount = nDecreaseCount;
        m_dwType = dwType;
        m_vecSyncPointList.swap(vecSyncPointList);
    }
    m_kUpdateCheckTimer.SetInterval( dwTickGap );

    START_LOG( cerr, L"센터 발렌타인 버프 기부 스크립트 읽기 완료. " ) << END_LOG;

    return true;
}

bool KCnBuffDonation::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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
            START_LOG( cerr, L"발렌타인 버프 기부 이벤트 스크립트 날짜 정보 오류" ) << END_LOG;
            return false;
    }

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

bool KCnBuffDonation::LoadSyncPointListInfo( IN KLuaManager& kLuaMng_, std::string& strTable_, OUT std::vector< PAIR_INT_BOOL >& vecSyncPointList_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int nSyncPoint;
    for ( int i = 1; ; ++i ) {
        if( S_OK != kLuaMng_.GetValue( i, nSyncPoint ) ) {
            START_LOG( clog, L"동기화 시점 읽는 루프 빠져 나감. index : " << i ) << END_LOG;
            break;
        }
        vecSyncPointList_.push_back( std::make_pair( nSyncPoint, false ) );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnBuffDonation::Tick() // 버프 타이머
{
    if ( m_kUpdateCheckTimer.CheckTime() ) {
        int nTypeFlag = FLAG_INVALID;
        // BuffStateUpdateProcess() 에서 버프 발동 시각 기록함(m_dwLastTick)
        if ( true == IsBuffActive() ) {
            DWORD dwCurrentTick = ::GetTickCount();
            if( dwCurrentTick - m_dwLastTick >= m_dwBuffRetainTime ) {
                // 버프 시간이 끝났으므로 true로 함수 호출
                BuffStateUpdateProcess( true, nTypeFlag );
            }
        }
        else {
            BuffStateUpdateProcess( false, nTypeFlag );
        }

        SendingEventToDBThread( nTypeFlag );
    }
}

void KCnBuffDonation::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock( m_csBuffDonation );
    tmDate_ = m_tmBegin;
}

void KCnBuffDonation::GetDonationTotalData( OUT KECN_BUFF_DONATION_TOTAL_DATA_NOT& kPacket_ )
{
    // 게임서버 연결 시에만 Get하는 함수라서... 그냥 Flag 여기서 설정 하도록 함
    KLocker lock( m_csBuffDonation );
    kPacket_.m_biTotalDonationPoint = m_biTotalDonationPoint;
    int nFlag = -1;
    if ( true == IsBuffActive() ) {
        nFlag = FLAG_BUFF_ON;
    }
    else {
        nFlag = FLAG_BUFF_OFF;
    }
    kPacket_.m_nBuffStateFlag = nFlag;
}

void KCnBuffDonation::UpdateDonationPointFromDB( IN const __int64& biTotalPoint_ )
{
    // DB에서 수치 얻어서 갱신할 때 씀
    KLocker lock( m_csBuffDonation );
    m_biTotalDonationPoint = biTotalPoint_;
}

void KCnBuffDonation::UpdateDonationPointDiff( IN const int& pointDiff_ )
{
    // 클라에서 기부 수량 diff 값 얻으면 캐싱
    KLocker lock( m_csBuffDonation );
    m_biTotalDonationPoint += pointDiff_;
    if ( 0 >= m_biTotalDonationPoint ) { // 최소값 보장
        m_biTotalDonationPoint = std::max<__int64>( 0, m_biTotalDonationPoint );
    }
    else if ( _I64_MAX <= m_biTotalDonationPoint ) { // 최대값 보장
        m_biTotalDonationPoint = std::min<__int64>( _I64_MAX, m_biTotalDonationPoint );
    }

    int nFlagType = FLAG_SYNC_ONLY_GAME_SERVER;
    BuffStateUpdateProcess( false, nFlagType );
    SendingEventToDBThread( nFlagType );
}

void KCnBuffDonation::BuffStateUpdateProcess( IN const bool& bEndBuffTime_, OUT int& nFlagType_ )
{
    // 전체 유저 데이터 씽크 할 시점인지 확인
    if ( true == IsSyncPointToAllUser() ) {
        if( true == IsBuffActive() ) {
            nFlagType_ = FLAG_BUFF_ON;
        }
        else {
            nFlagType_ = FLAG_BUFF_OFF;
        }
    }

    // 버프 타이머에서 버프 시간이 다 된거면
    if ( true == bEndBuffTime_ ) {
        // 버프 상태 Off
        m_bBuffActive = false;
        // 일단 flag변수 버프 종료로 설정
        nFlagType_ = FLAG_BUFF_OFF;
        START_LOG( clog, L"버프 시간 종료 됨." )
            << BUILD_LOGtm( CTime ( m_dwLastTick ) )
            << BUILD_LOG( m_bBuffActive )
            << BUILD_LOG( nFlagType_ )
            << END_LOG;
    }

    // 포인트가 버프를 걸어 줘야 하는지 확인 (버프가 끝나고 바로 걸어줄 수 있도록 밑에서 확인)
    if( true == IsBuffActivationEnable() ) {
        // 현재 시점으로 타이머 기준 설정
        m_dwLastTick = ::GetTickCount();
        // 버프 상태 ON으로 저장
        m_bBuffActive = true;
        // flag 변수 버프 On 설정
        nFlagType_ = FLAG_BUFF_START_NOT;

        START_LOG( clog, L"버프기부 기부 수치가 일정량 넘어서 버프 발동 됨." )
            << BUILD_LOGtm( CTime( m_dwLastTick ) )
            << BUILD_LOG( m_bBuffActive )
            << BUILD_LOG( nFlagType_ )
            << END_LOG;
    }
}

void KCnBuffDonation::SendingEventToDBThread( IN const int& nFlagType_ )
{
    // flag가 버프 on/off, 버프 처음on, 게임서버만 씽크일 때 db데이터 퍼와서 전송
    if ( FLAG_BUFF_ON == nFlagType_ || FLAG_BUFF_OFF == nFlagType_ || 
        FLAG_BUFF_START_NOT == nFlagType_ || FLAG_SYNC_ONLY_GAME_SERVER == nFlagType_ ) {
        KECN_BUFF_DONATION_TOTAL_DATA_NOT kPacket;
        kPacket.m_nBuffStateFlag = nFlagType_;
        // DB데이터 퍼오기
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_BUFF_DONATION_TOTAL_DATA_REQ, NULL, 0, 0, kPacket );

        START_LOG( clog, L"버프 기부 데이터 센터 DB스레드 처리 요청." )
            << BUILD_LOG( nFlagType_ )
            << END_LOG;
    }
}

void KCnBuffDonation::SendingDataToAllGameServer( IN const KECN_BUFF_DONATION_TOTAL_DATA_NOT& kPacket_ )
{
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_BUFF_DONATION_TOTAL_DATA_NOT, kPacket_ );

    START_LOG( clog, L"게임서버 전체로 버프 기부 데이터 전송." )
        << BUILD_LOG( kPacket_.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket_.m_nBuffStateFlag )
        << END_LOG;
}

void KCnBuffDonation::UpdateDonationPointByBuffActivation( IN __int64& biDBPoint_, OUT __int64& biComputedPoint_ )
{
    KLocker lock( m_csBuffDonation );

    __int64 biDecreasePoint = GetDecreasePoint();

    biComputedPoint_ = biDBPoint_ + biDecreasePoint;

    if ( 0 > biComputedPoint_ ) { // 최소값 보장
        biComputedPoint_ = std::max<__int64>( 0, biComputedPoint_ );
    }

    m_biTotalDonationPoint = biComputedPoint_;
}

const bool KCnBuffDonation::IsBuffActivationEnable()
{
    KLocker lock( m_csBuffDonation );
    if ( false == IsBuffActive() && GetActiveBuffPoint() <= m_biTotalDonationPoint ) {
        return true;
    }
    else {
        return false;
    }
}

const __int64 KCnBuffDonation::GetDecreasePoint()
{
    KLocker lock( m_csBuffDonation );
    // 버프 발동에 사용 되는 감소 수치를 얻는다
    __int64 biDecreasePoint = 0;
    // 스크립트에 감소 수치 0으로 설정하면 기부 수량 무조건 전쳬(0으로) 초기화
    if ( 0 == m_nDecreaseCount ) {
        biDecreasePoint = -m_biTotalDonationPoint;
    }
    else {
        biDecreasePoint = -m_nDecreaseCount;
    }

    return biDecreasePoint;
}

const bool KCnBuffDonation::IsSyncPointToAllUser()
{
    std::vector< PAIR_INT_BOOL >::iterator vit;

    for ( vit = m_vecSyncPointList.begin(); vit != m_vecSyncPointList.end(); vit++ ) {
        // 해당 점수에 도달했고 아직 NOT안보냈으면 true
        if ( false == vit->second && m_biTotalDonationPoint >= vit->first ) {
            vit->second = true;
            return true;
        }
    }

    return false;
}

void KCnBuffDonation::ResetSecondDataAtSyncPointVector()
{
    std::vector< PAIR_INT_BOOL >::iterator vit;

    for ( vit = m_vecSyncPointList.begin(); vit != m_vecSyncPointList.end(); vit++ ) {
        vit->second = false;
    }
}