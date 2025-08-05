#include "User.h"
#include "CnConnector.h" // winsock
#include "GSSimLayer.h"
#include "BuffDonation.h"
#include <dbg/dbg.hpp >
#include <kncutil.h>
#include "Log4.h"
#include "Lua/KLuaManager.h"
#include "LoudMessage.h"
#include "SignBoard.h"
#include "GameServer.h"

ImplementSingleton( KBuffDonation );
ImplOstmOperatorW2A( KBuffDonation );
NiImplementRootRTTI( KBuffDonation );
ImplToStringW( KBuffDonation )
{
    KLocker lock( m_csBuffDonation );

    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_dwDonationItem )
        << TOSTRINGW( m_vecRewardItem.size() )
        << TOSTRINGW( m_biActiveBuffPoint )
        << TOSTRINGW( m_nRecoveryChance )
        << TOSTRINGW( m_dwType )
        << TOSTRINGW( m_bUseIndividualBuffSystem )
        << TOSTRINGW( m_nBuffStateFlag );
}

KBuffDonation::KBuffDonation(void)
:m_tmBegin(0)
,m_tmEnd(0)
,m_dwDonationItem(0)
,m_biActiveBuffPoint(0)
,m_dwType(1)
,m_bUseIndividualBuffSystem(false)
,m_nBuffStateFlag(FLAG_INVALID)
{
    m_vecRewardItem.clear();
}

KBuffDonation::~KBuffDonation(void)
{
}

bool KBuffDonation::LoadScript()
{
    return _LoadScript(std::string());
}

bool KBuffDonation::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "BuffDonation.lua";
    strScript_ = strScriptName;

    time_t tmBegin; // 기부 시작 날짜
    time_t tmEnd; // 기부 종료 날짜

    GCITEMID dwDonationItem; // 기부에 사용 되는 아이템
    __int64 biActiveBuffPoint; // 버프 걸어주는 기준치
    int nRecoveryChance; // 스킬 사용시 HP/MP회복 확률
    DWORD dwType; // 키로 사용 하는 타입
    bool bUseIndividualBuffSystem; // 개별 버프 여부
    VEC_REWARD vecRewardItem; // 기부하고 교환 받는 아이템

    std::wstring strBuffStartNotifyMsg; // 버프 발동 메세지

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( kLuaMng.GetValue( "DonationItem", dwDonationItem ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ActiveBuffPoint", biActiveBuffPoint ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "RecoveryChance", nRecoveryChance ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UseIndividualBuffSystem", bUseIndividualBuffSystem ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TypeNumber", dwType ) == S_OK, return false );
    _JIF( LoadRewardItem( kLuaMng, std::string("RewardItem"), vecRewardItem ), return false );
    strBuffStartNotifyMsg = GetBuffNotString(L"str_00");

    {
        KLocker lock( m_csBuffDonation );
        m_tmBegin = tmBegin;
        m_tmEnd = tmEnd;
        m_dwDonationItem = dwDonationItem;
        m_biActiveBuffPoint = biActiveBuffPoint;
        m_nRecoveryChance = nRecoveryChance;
        m_dwType = dwType;
        m_bUseIndividualBuffSystem = bUseIndividualBuffSystem;
        m_vecRewardItem.swap( vecRewardItem );
        m_strBuffStartNotifyMsg = strBuffStartNotifyMsg;
    }

    START_LOG( cerr, L"발렌타인 버프 기부 이벤트 스크립트 로드 완료." ) << END_LOG;

    return true;
}

bool KBuffDonation::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KBuffDonation::LoadRewardItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
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

bool KBuffDonation::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csBuffDonation );

    return ( m_tmBegin <= tmCurrent && tmCurrent <= m_tmEnd );
}

bool KBuffDonation::IsUseIndividualBuffSystem()
{
    KLocker lock( m_csBuffDonation );
    return m_bUseIndividualBuffSystem;
}

void KBuffDonation::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock( m_csBuffDonation );
    tmDate_ = m_tmBegin;
}

void KBuffDonation::GetDonationDataForFirstLogin( OUT KEVENT_BUFF_DONATION_LOGIN_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csBuffDonation );
    kPacket_.m_biTotalDonationPoint = m_biTotalDonationPoint;
    kPacket_.m_nBuffStateFlag = m_nBuffStateFlag;
    kPacket_.m_dwDonationItemID = m_dwDonationItem;
    kPacket_.m_biActiveBuffPoint = m_biActiveBuffPoint;
    kPacket_.m_nRecoveryChance = m_nRecoveryChance;
}

void KBuffDonation::GetRewardItemList( OUT VEC_REWARD& vecReward_ )
{
    KLocker lock( m_csBuffDonation );
    vecReward_ = m_vecRewardItem;
}

bool KBuffDonation::CheckDonationItemValidation( IN const GCITEMUID& dwItemID_ )
{
    if ( dwItemID_ != m_dwDonationItem ) {
        START_LOG( cwarn, L"기부로 요청 한 아이템이 스크립트에 등록 되어 있지 않다.")
            <<BUILD_LOG( dwItemID_ )
            <<BUILD_LOG( m_dwDonationItem )
            <<END_LOG;

        return false;
    }

    return true;
}

void KBuffDonation::GetDonationTotalData( OUT KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kPacket_ )
{
    KLocker lock( m_csBuffDonation );
    kPacket_.m_biTotalDonationPoint = m_biTotalDonationPoint;
    kPacket_.m_nBuffStateFlag = m_nBuffStateFlag;
}

void KBuffDonation::GetTotalDonationPoint( OUT __int64& biTotalPoint_ )
{
    KLocker lock( m_csBuffDonation );
    biTotalPoint_ = m_biTotalDonationPoint;
}

void KBuffDonation::UpdateTotalDonationPointFromCenter( IN const KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kPacket  )
{
    // 센터서버에서 받은 전체 서버 기부 포인트를 갱신한다.
    m_biTotalDonationPoint = kPacket.m_biTotalDonationPoint;
    m_nBuffStateFlag = kPacket.m_nBuffStateFlag;
}

void KBuffDonation::NotifyDonationInfoToAllUser( IN const KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kPacket_ )
{
    // 전체 유저에 대해 기부 정보 갱신 요청을 한다.
    // 클라이언트에서 flag에 따라서 버프를 걸어줄지 풀어줄지 결정한다.
    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_BUFF_DONATION_TOTAL_DATA_NOT, kPacket_ );
}

void KBuffDonation::UpdateDonationPointDiffReqToCenter( IN const int& nDonationPointDiff_ )
{
    // 유저가 기부한 수치를 센터서버에 갱신 요청
    KLocker lock( m_csBuffDonation );
    m_biTotalDonationPoint += nDonationPointDiff_;
    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_BUFF_DONATION_DATA_UPDATE_REQ, nDonationPointDiff_ ) );
}

void KBuffDonation::NotifyBuffStartMsgFromCenter()
{
    // 이 함수가 호출 되었다는 것은 버프 발동 공지를 출력해야 한다는 것이다.
    KLocker lock( m_csBuffDonation );

        KSignBoardData kData;
        kData.m_dwColor         = 0L;
        kData.m_ItemID          = 0L;
        kData.m_dwSenderUID     = 0L;
        kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
        kData.m_strSenderNick.clear();
    kData.m_strMsg = m_strBuffStartNotifyMsg;
        SiKSignBoard()->QueueingAdminData( kData );
}

std::wstring KBuffDonation::GetBuffNotString( IN std::wstring strKey_ )
{
    wchar_t szTemp[255] = {0};

    GetPrivateProfileStringW( L"BuffDonation", strKey_.c_str(), L"", szTemp, 255, SiKGameServer()->m_stringTable.GetCurrentFile().c_str() );
    std::wstring strBuffNotString(szTemp);

    return strBuffNotString;
}