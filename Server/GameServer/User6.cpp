#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "GSSimLayer.h"
#include "Channel.h"
#include "GameServer.h"
#include "PetHelper.h"
#include <boost/algorithm/string.hpp> // boost::trim
#include "EnchantStrong.h"
#include "SlangFilter.h"
#include "GCHelper.h"
#include "SquareManager.h"
#include "Square.h"
#include "ChannelManager.h"
#include "SignBoard.h"
#include "StatisticsManager.h"
#include "StatEmoticon.h"
#include "RankManager.h"
#include "ItemManager.h"
#include "CollectionManager.h"
#include "support_User.h"
#include "LoudMessage.h"
#include "MiniGameRankManager.h"
#include "ResultManager.h"
#include "SkillManager.h"
#include "CnConnector.h"
#include "PreHackingCheckManager.h"
#include "VirtualCash.h"
#include "Rainbow.h"
#include "MissionManager.h"
#include "IPAdvantage.h"
#include "NewDonationManager.h"
#include "CharDefault.h"
#include "GuildManager.h"
#include "GCPoint.h"
#include "SHCheckPoint.h"
#include "HeroDungeonManager.h"
#include "AgitHelper.h"
#include "SocialCommerce.h"

#define CLASS_TYPE  KUser

// 유효한 진화가 가능한지 체크 하고 적용한다.
IMPL_ON_FUNC( EVENT_TRANSFORMATION_PET_REQ )
{
    // 상태가 올바른가.
    _VERIFY_STATE(( 3, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY ));

    KEVENT_TRANSFORMATION_PET_ACK kPacket;
    KEVENT_DB_TRANSFORMATION_PET_REQ kDBPacket;
    kDBPacket.m_cCurrentChar = GetCurrentCharType();
    KItemPtr pkItem;
    std::wstring strNewName;
    int nPromotionFactor = 0; // 진화 1단계 할지 톼화 1단계 할지 결정.
    int iThresholdLv = 0; // 진화 아이템에서 가져온 최소 진화 레벨 
    SET_ERROR( ERR_UNKNOWN );

    //해당 펫이 존재하는가?
    KPetInfo* pkPetInfo = NULL;
    if( !GetPetInfo( kPacket_.m_dwPetUID, &pkPetInfo ) )
    {
        // 펫이 없다.
        SET_ERR_GOTO( ERR_PET_07, END_PROC );
    }

    switch( kPacket_.m_ePTT )
    {
    case KUser::PTT_EVOLUTION:
        {
            if( SiKPetHelper()->GetEvolutionType( kPacket_.m_dwItemID )  != 0 )
            {
                SET_ERR_GOTO( ERR_PET_30, END_PROC );
            }

            nPromotionFactor = 1;
        }
        break;
    case KUser::PTT_DEVOLUTION:
        {
            //펫의 전직 상태가 1 이상인가?
            if( pkPetInfo->m_cPromotion < 1 )
            {
                //렙이 낮다.
                SET_ERR_GOTO( ERR_PET_24, END_PROC );
            }
            // 펫 전직 아이템 타입이 '퇴화' 인가
            if( SiKPetHelper()->GetEvolutionType( kPacket_.m_dwItemID )  != 1 )
            {
                SET_ERR_GOTO( ERR_PET_30, END_PROC );
            }

            nPromotionFactor = -1;
        }
        break;
    default:
        {
            SET_ERR_GOTO( ERR_PET_29, END_PROC );
        }
    }

    //펫 진화 카드가 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID );
    if( pkItem == NULL )
    {
        //아이템이 없다. (내 인벤에)
        SET_ERR_GOTO( ERR_PET_25, END_PROC );
    }

    if( !SiKPetHelper()->GetTransformLv( kPacket_.m_dwItemID, iThresholdLv ) )
    {
        //펫 전직 레벨 정보 가져오기 실패.
        SET_ERR_GOTO( ERR_PET_31, END_PROC );
    }

    pkPetInfo->m_iLevel = SiKPetHelper()->GetPetLevel( pkPetInfo->m_dwEXP );
    if( pkPetInfo->m_iLevel < iThresholdLv ) // 내 펫의 레벨이 진화 기준이 못미치는가?
    {
        SET_ERR_GOTO( ERR_PET_24, END_PROC );
    }

    //진화된 펫의 이름을 얻어낸다.
    if( !SiKLoudMessage()->GetItemName( static_cast<int>( pkPetInfo->m_dwID ), pkPetInfo->m_cPromotion + nPromotionFactor, strNewName, GetLanguageCode() ) )
    {
        // 아이템 이름이 존재하지 않음.
        SET_ERR_GOTO( ERR_PET_26, END_PROC );
    }

    //진화된 이후의 이름이나 현재의 이름이 아니면 해킹일까?
    if( kPacket_.m_strPetName != strNewName && kPacket_.m_strPetName != pkPetInfo->m_strName )
    {
        SET_ERR_GOTO( ERR_PET_27, END_PROC );
    }

    kDBPacket.m_kPacket = kPacket_;
    GetDBUpdatePetData( kDBPacket.m_kPetData ); //여기에 펫 정보도 있다.
    kDBPacket.m_cCurrentChar = GetCurrentCharType();

    QUEUEING_EVENT_TO_DB( EVENT_TRANSFORMATION_PET_REQ, kDBPacket );

    return;
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( ERR_PET_07, kPacket.m_nOK, 1 ); // 펫 정보가 존재하지 않음
        ERR_CASE( ERR_PET_24, kPacket.m_nOK, 2 ); // 펫 이 진화하기에 레벨이 낮다.
        ERR_CASE( ERR_PET_25, kPacket.m_nOK, 3 ); // 펫 진화 카드를 가지고 있지 않거나 부족함.
        ERR_CASE( ERR_PET_26, kPacket.m_nOK, 4 ); // 이름 정보가 없습니다.
        ERR_CASE( ERR_PET_27, kPacket.m_nOK, 5 ); // 이름 정보가 옳지 못함.
        ERR_CASE( ERR_PET_29, kPacket.m_nOK, 9 ); // 펫 전직 타입이 올바르지 않음.
        ERR_CASE( ERR_PET_30, kPacket.m_nOK, 11 ); // 잘못된 진화/퇴화 아이템 정보를 보냈음.
        ERR_CASE( ERR_PET_31, kPacket.m_nOK, 12 ); //펫 전직 레벨 정보 가져오기 실패 
    default:
        kPacket.m_nOK = 99;
        SET_ERROR( ERR_UNKNOWN );
        break;
    }
    SEND_PACKET( EVENT_TRANSFORMATION_PET_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L"진화 유효성 체크"
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_strPetName )
        << BUILD_LOG( kPacket_.m_dwPetUID )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_TRANSFORMATION_PET_ACK )
{
    SET_ERROR( ERR_UNKNOWN );
    KPetInfo* pkPetInfo = NULL;
    KItemPtr pkItem;

    std::map<char, DWORD>::iterator mit;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        START_LOG( cerr, L" DB 에서 오류... " << m_strName ) << END_LOG;
        goto END_PROC;
    }
    // 펫 이름과 전직 상태만 DB에서 올린상태이다.
    // 이 아래에서 실패를 하여도 자방해야한다. 이미 전직했기 때문이다.

    //전직 아이템을 제거 하자.
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwItemID );
    if( pkItem != NULL )
    {
        pkItem->m_nCount    -= 1;
    }
    else
    {
        //아이템이 없다.
        START_LOG( cerr, L"전직 아이템이 부족하거나 없다...하지만 자방해야 한다." << m_strName ) << END_LOG;
    }

    // 유저에게서 찾는다.
    if( !GetPetInfo( kPacket_.m_kPetInfo.m_dwUID, &pkPetInfo ) )
    {
        SET_ERR_GOTO( ERR_PET_07, END_PROC );
    }

    // 캐릭터에게서 펫 연결을 끈는다
    ReleasePetFromChar( pkPetInfo->m_dwUID );

    // 펫이 장착하고 있는 아이템을 제거한다.
    pkPetInfo->m_vecEquipItem.clear();

    pkPetInfo->m_strName        = kPacket_.m_kPetInfo.m_strName;
    pkPetInfo->m_cPromotion     = kPacket_.m_kPetInfo.m_cPromotion;

    mit = pkPetInfo->m_mapInitExp.find( pkPetInfo->m_cPromotion );
    if( mit != pkPetInfo->m_mapInitExp.end() )
    {
        pkPetInfo->m_dwEXP = mit->second ;
    }
    pkPetInfo->m_iLevel = SiKPetHelper()->GetPetLevel( pkPetInfo->m_dwEXP );
    pkPetInfo->m_vecEquipItem.clear();

    kPacket_.m_kPetInfo = *pkPetInfo;
    // 아이템을 넣어주자.
    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

    //방에서 전직했으면 장착 정보변경 패킷을 보내기 때문에 OK...

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_PET_07, kPacket_.m_nOK, 1 ); // 펫 정보가 존재하지 않음
        ERR_CASE( ERR_PET_03, kPacket_.m_nOK, 6 ); // 아이템 구매자 존재하지 않음
        ERR_CASE( ERR_PET_04, kPacket_.m_nOK, 7 ); // 구매할수 없는 아이템.. 존재하지 않음. 비매품. 캐쉬아이템아님. 패턴 아이템.
        ERR_CASE( ERR_PET_01, kPacket_.m_nOK, 8 ); // 아이템 구매 시도 실패
        ERR_CASE( ERR_PET_28, kPacket_.m_nOK, 10 ); // 펫 전직 레벨 오류
    default:
        kPacket_.m_nOK = -99;
        SET_ERROR( ERR_UNKNOWN );
    }

    SEND_RECEIVED_PACKET( EVENT_TRANSFORMATION_PET_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_dwID )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_strName )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_dwEXP )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_iLevel )
        << BUILD_LOGc( kPacket_.m_kPetInfo.m_cPromotion )
        << BUILD_LOG( kPacket_.m_kPetInfo.m_vecEquipItem.size() )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_FORCE_TRANSITION_STATE_REQ, int )
{
    int kPacket = -99;

    if( !m_pkCurrentState->CheckValidState( 12, KUserFSM::STATE_PLAYING, KUserFSM::STATE_ROOM,
                                               KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY,
                                               KUserFSM::STATE_SQUARE, KUserFSM::STATE_SQUARE_JOIN,
                                               KUserFSM::STATE_MINIGAME_PLAYING, KUserFSM::STATE_MINIGAME,
                                               KUserFSM::STATE_GUILD_ROOM, KUserFSM::STATE_GUILD_PLAYING,
                                               KUserFSM::STATE_AGIT, KUserFSM::STATE_AGIT_JOIN ) )
    { 
        kPacket = -1; //상태 변경 오류
        SEND_PACKET( EVENT_FORCE_TRANSITION_STATE_ACK );

        START_LOG( cerr, L"상태 오류. Name : " << m_strName )
            << L"    현재 상태 : " << GetStateIDString() << dbg::endl
            << L"    허용 상태 : KUserFSM::STATE_PLAYING, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL " << END_LOG;
        return;
    }

    if( !ForceTranslateState( kPacket_ ) )
    {
        kPacket = -1; //상태 변경 오류
        SEND_PACKET( EVENT_FORCE_TRANSITION_STATE_ACK );
        return;
    }

    kPacket = 0;
    SEND_PACKET( EVENT_FORCE_TRANSITION_STATE_ACK );
}

// GC PointSystem
_IMPL_ON_FUNC( EVENT_LOAD_POINTSYSTEM_INFO_ACK, KPointSystemInfo )
{
    KCalendarInfo kPacket;
    kPacket.m_nOK = 0;

    if( kPacket_.m_nOK != 0 )
    {
        kPacket.m_nOK = kPacket_.m_nOK;
        SEND_PACKET( EVENT_LOAD_POINTSYSTEM_INFO_ACK );
        START_LOG( cerr, L"포인트 시스템 얻어오지 못했음.. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
        return;
    }

    m_kCalendar.SetAccTime( kPacket_.m_nAccTime );
    START_LOG( clog, L"정상적으로 누적시간 세팅 ")
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nAccTime ) << END_LOG;

    m_kCalendar.LoadDailyInfo( kPacket_.m_vecCalendar );
    m_kGCPoint.SetInitGCPoint( kPacket_.m_nPoint );
    m_kGCPoint.SetGCPoint( kPacket_.m_nPoint );

    //------------------------------------------------    
    kPacket.m_nAccTime      = m_kCalendar.GetAccTime();
    kPacket.m_nPoint        = m_kGCPoint.GetGCPoint();

    m_kCalendar.GetToday( kPacket.m_kToday );
    m_kCalendar.GetCalendarInfo( kPacket.m_mapCalendar );

    kPacket.m_nAttendTime   = SiKGCPoint()->GetAttendTime();

    KSimpleDate kNoUseDate;
    // Get the first, last day in this month.
    m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear, kPacket.m_kToday.m_cMonth,
        kPacket.m_kFirstDay, kPacket.m_kLastDay );

    // Get the Last day of last month
    if( kPacket.m_kToday.m_cMonth == 1 ) // If the month is January,then set year - 1, month = 12
    {
        m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear-1, 12,
            kNoUseDate, kPacket.m_kLastDayOfLastMonth );
    }
    else
    {
        m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear, kPacket.m_kToday.m_cMonth-1,
            kNoUseDate, kPacket.m_kLastDayOfLastMonth );
    }

    SEND_COMPRESS_PACKET( EVENT_LOAD_POINTSYSTEM_INFO_ACK, kPacket );
    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_kCalendar.GetAccTime() )
        << BUILD_LOG( m_kGCPoint.GetGCPoint() )
        << BUILD_LOGc( kPacket.m_kToday.m_cMonth )
        << BUILD_LOGc( kPacket.m_kToday.m_cDay ) << END_LOG;

    if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
    {
        m_kCalendar.Dump();
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_GET_CURRENT_POINT_REQ )
{
    if( SiKGCPoint()->IsGCPointSwitch() == false ) // 쥐씨 포인트 시스템 사용하지 않으면 
    {
        return;
    }
    int kPacket = m_kGCPoint.GetGCPoint();
    SEND_PACKET( EVENT_GET_CURRENT_POINT_ACK );
    START_LOG( clog, L"포인트 전달.. Name : " << m_strName << L", Point : " << m_kGCPoint.GetGCPoint() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GET_CALENDAR_REQ )
{
    if( SiKGCPoint()->IsGCPointSwitch() == false ) // 쥐씨 포인트 시스템 사용하지 않으면 
    {
        return;
    }
    KCalendarInfo kPacket;
    kPacket.m_nOK = 0;
    if( !m_kCalendar.IsLoaded() )
    {
        kPacket.m_nOK = -1;
        SEND_PACKET( EVENT_GET_CALENDAR_ACK );
        START_LOG( cerr, L"달력 정보 보내주기 실패")
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
        return;
    }

    m_kCalendar.GetCalendarInfo( kPacket.m_mapCalendar );
    m_kCalendar.GetToday( kPacket.m_kToday );
    kPacket.m_nAccTime     = m_kCalendar.GetAccTime();
    kPacket.m_nPoint       = m_kGCPoint.GetGCPoint();
    kPacket.m_nAttendTime  = SiKGCPoint()->GetAttendTime();

    KSimpleDate kNoUseDate;
    // Get the first, last day in this month.
    m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear, kPacket.m_kToday.m_cMonth,
        kPacket.m_kFirstDay, kPacket.m_kLastDay );

    // Figure out the Last day of last month
    if( kPacket.m_kToday.m_cMonth == 1 ) // If the month is January, year - 1, month = 12
    {
        m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear-1, 12,
            kNoUseDate, kPacket.m_kLastDayOfLastMonth );
    }
    else
    {
        m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear, kPacket.m_kToday.m_cMonth-1,
            kNoUseDate, kPacket.m_kLastDayOfLastMonth );
    }

    SEND_COMPRESS_PACKET( EVENT_GET_CALENDAR_ACK, kPacket );
    START_LOG( clog, L"달력 정보 얻어서 보내주기")
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SEALLING_CALENDAR_REQ, KSimpleDate ) // 출석 도장 찍기
{ 
    if ( SiKGCPoint()->IsGCPointSwitch() == false ) {
        return;
    }

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_SEALLING_CALENDAR_ACK kPacket;
    kPacket.m_nOK               = -99;
    kPacket.m_nIncGCPoint       = 0;
    kPacket.m_nGCPoint          = m_kGCPoint.GetGCPoint();

    CTime tmDate;

	if(kPacket_.m_sYear < 1900)
	{
		goto END_PROC;
	}
    
    if ( false == m_kCalendar.IsLoaded() ) {
        SET_ERR_GOTO( ERR_CALENDAR_09, END_PROC );
    }

    // 오류 값이 안에서 설정되어 있다.
    if ( false == m_kCalendar.SetSealling( kPacket_.m_cDay, kPacket.m_kDailyInfo ) ) {
        goto END_PROC;
    }
    // 여기가 일간 출석 체크시 헬모드 입장권 주기.
    CheckHellItemAttendance( SiKGCHelper()->GetAttendanceHellCount() );
    UpdateRecom5th( kPacket_ );

    // 일일 출석 체크시 GC클럽 보너스 아이템 지급
    CheckGCClubmapDayAttendBonus();

    // 럭키데이 체크
    if ( m_kCalendar.CheckLuckyDay( kPacket_.m_cDay, kPacket.m_kDailyInfo.m_cSealType ) ) {
        // 성공일 때에만 NOT 보내준다
        KEVENT_LUCKYDAY_NOT kPacketLucky;
        kPacketLucky.m_kDailyInfo = kPacket.m_kDailyInfo;
        QUEUEING_EVENT_TO_DB( EVENT_LUCKYDAY_NOT, kPacketLucky );
    }

    //월개근 체크
    if ( m_kCalendar.SetMonthlyAttendance() ) {
        //월개근시 출석시 헬모드 입장권 지급
        CheckHellItemAttendance( SiKGCHelper()->GetMonthAttendHellCount() );

        KDailyInfo kDBPacket;
        kDBPacket = kPacket.m_kDailyInfo;
        QUEUEING_EVENT_TO_DB( EVENT_MONTHLY_ATTEND_NOT, kDBPacket );

        START_LOG( clog, L"월개근을 하여서 DB로 넘김... Name : " << m_strName )
            << BUILD_LOG( kDBPacket.m_kDate.m_sYear )
            << BUILD_LOGc( kDBPacket.m_kDate.m_cMonth )
            << BUILD_LOGc( kDBPacket.m_kDate.m_cDay ) << END_LOG;
    }
    else {
        START_LOG( clog, L"월개근 실패 요인?, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    // 길드 포인트 처리
    if ( GetGuildUID() != 0 ) {
        SiKGuildManager()->AddGuildPoint( GetGuildUID(), GetUID(), KGuildConfig::ATTEND );
        AddGuildContributePoint( SiKGuildManager()->GetGuildPointDiff( KGuildConfig::ATTEND ) );
        KEVENT_GET_GUILD_POINT_NOTICE kPacket;
        kPacket.m_nGetType = KEVENT_GET_GUILD_POINT_NOTICE::ATTEND;
        kPacket.m_nGuildPoint = SiKGuildManager()->GetGuildPointDiff( KGuildConfig::ATTEND );
        
        if ( kPacket.m_nGuildPoint > 0 ) {
            SEND_PACKET( EVENT_GET_GUILD_POINT_NOTICE );
        }
    }

    // 출석포인트 처리
    tmDate = CTime( kPacket_.m_sYear, kPacket_.m_cMonth, kPacket_.m_cDay, 0, 0, 0 );
    kPacket.m_nIncGCPoint = SiKGCPoint()->GetGCPointDay( KncUtil::TimeToInt( tmDate ) );
    kPacket.m_nIncGCPoint = std::max<int>( kPacket.m_nIncGCPoint, 0 );
    kPacket.m_nGCPoint = m_kGCPoint.GetGCPoint();
    kPacket.m_nAccTime = m_kCalendar.GetAccTime();

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SEALLING_CALENDAR_REQ, kPacket );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_CALENDAR_00, kPacket.m_nOK, 1 ); // 누적시간 미달로 출첵불가
        ERR_CASE( ERR_CALENDAR_09, kPacket.m_nOK, 2 ); // 달력 정보 로드 실패
        ERR_CASE( ERR_CALENDAR_10, kPacket.m_nOK, 3 ); // 업데이트 할려는 날짜의 주차 정보를 얻어 낼수 없음
        ERR_CASE( ERR_CALENDAR_11, kPacket.m_nOK, 4 ); // 업데이트 할려는 날짜의 정보를 얻어 낼수 없음
        ERR_CASE( ERR_CALENDAR_04, kPacket.m_nOK, 5 ); // 이미 월개근 했음
        ERR_CASE( ERR_CALENDAR_12, kPacket.m_nOK, 6 ); // 출석체크하는 날의 도장이 네모가 아님
        ERR_CASE( ERR_CALENDAR_13, kPacket.m_nOK, 7 ); // 일일출석포인트값이 0이하로 설정되어 있음
        ERR_CASE( ERR_CALENDAR_14, kPacket.m_nOK, 8 ); // 일일출석포인트 DB저장 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 헬모드 아이템 요청 중이다.
    default:
        kPacket.m_nOK = -99;
        SET_ERROR( ERR_UNKNOWN );
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_SEALLING_CALENDAR_ACK );
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 1 || kPacket.m_nOK == 7 || NetError::GetLastNetError() == NetError::ERR_NOT_FINISHED_YET)
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nIncGCPoint )
        << BUILD_LOG( kPacket.m_nGCPoint )
        << BUILD_LOG( m_kCalendar.GetAccTime() )  
        << BUILD_LOG( kPacket_.m_sYear )
        << BUILD_LOGc( kPacket_.m_cMonth )
        << BUILD_LOGc( kPacket_.m_cDay )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_SEALLING_CALENDAR_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 일일 출석포인트 보너스 지급 적용
    m_kGCPoint.SetGCPoint( kPacket_.m_nGCPoint );
    m_kGCPoint.SetInitGCPoint( kPacket_.m_nGCPoint );

    // 출석 n 회 캐릭터 획득 아이템 지급 이벤트, 브라질.
    if ( SiKGCHelper()->IsAttendToGetCharTerm() &&
        SiKGCHelper()->IsAttendToGetCharDay( kPacket_.m_kDailyInfo.m_kDate ) ) {
        
        QUEUEING_ID_TO_DB( EVENT_ATTEND_GET_CHAR_REQ );
    }


END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,          kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_CALENDAR_14, kPacket_.m_nOK, 8 ); // 일일출석포인트 DB저장 실패
    default:
        START_LOG( cerr, L"일일 출석포인트 보너스 지급 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"일일 출석포인트 보너스 지급 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGCPoint )
        << BUILD_LOG( kPacket_.m_nIncGCPoint )
        << BUILD_LOG( kPacket_.m_kDailyInfo.m_kDate.Str() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SEALLING_CALENDAR_ACK );
}

IMPL_ON_FUNC( EVENT_ATTEND_GET_CHAR_ACK )
{
    START_LOG( clog, L"출석n회 보상으로 캐릭터 아이템(카드) 지급, Login : " << GetName() )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << END_LOG;

    if( !kPacket_.m_vecRewardItem.empty() ) {
        m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

        SEND_RECEIVED_PACKET( EVENT_ATTEND_GET_CHAR_ACK );
    }    
}

IMPL_ON_FUNC( EVENT_MONTHLY_ATTEND_NOT ) //달개근시에 이쪽으로 들어 옴.
{
    //SiKGSSimLayer()->GetMonthlyRewardList( kPacket_.m_vecMonthlyRewardList );
    SiKGSSimLayer()->GetMonthlySelectRewardList( kPacket_.m_vecMonthlyRewardList );

    SEND_RECEIVED_PACKET( EVENT_MONTHLY_ATTEND_NOT );

    LOG_CONDITION( kPacket_.m_nOK == 0, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecMonthlyRewardList.size() )
        << BUILD_LOG( kPacket_.m_vecReservedReward.size() ) << END_LOG;
}

// 달이 바뀌었기 때문에 켈린더를 업데이트 한다.
_IMPL_ON_FUNC( EVENT_LOAD_NEW_CALENDAR_NOT, std::vector<KDailyInfo> )
{
    KCalendarInfo kPacket;
    kPacket.m_mapCalendar.clear();
    if( kPacket_.empty() )
    {
        CTime tmDate = CTime::GetCurrentTime();
        KDailyInfo kDailyInfo;
        kDailyInfo.m_kDate.m_sYear      = tmDate.GetYear();
        kDailyInfo.m_kDate.m_cMonth     = tmDate.GetMonth();
        kDailyInfo.m_kDate.m_cDay       = tmDate.GetDay();
        kDailyInfo.m_kDate.m_cWeekDay   = tmDate.GetDayOfWeek() -1;
        kDailyInfo.m_cSealType          = ST_TRI;
        kDailyInfo.m_cWeekID            = m_kCalendar.GetWeekNumber( tmDate.GetYear(), tmDate.GetMonth(), tmDate.GetDay() );
        kPacket_.push_back( kDailyInfo );

        START_LOG( clog, L"달력 정보가 비어서 오늘 날짜로 채워준다.")
            << BUILD_LOG( m_strName )
            << BUILD_LOGc( kDailyInfo.m_kDate.m_cMonth )
            << BUILD_LOGc( kDailyInfo.m_kDate.m_cDay ) << END_LOG;

    }
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_CALENDAR_09, kPacket.m_nOK, 1 ); //달력 정보 로드 실패

    }
    m_kCalendar.LoadDailyInfo( kPacket_ );

    m_kCalendar.SetAccTime( 0 ); // 070427. woosh. 누적시간 초기화...

    m_kCalendar.GetToday( kPacket.m_kToday );
    m_kCalendar.GetCalendarInfo( kPacket.m_mapCalendar ); // 인자로 들어간 맵에 달력맵 세팅.(클라이언트로..)
    kPacket.m_nAccTime  = m_kCalendar.GetAccTime();
    kPacket.m_nPoint    = m_kGCPoint.GetGCPoint();

    kPacket.m_nAttendTime = SiKGCPoint()->GetAttendTime();

    KSimpleDate kNoUseDate;
    // Get the first, last day in this month.
    m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear, kPacket.m_kToday.m_cMonth,
        kPacket.m_kFirstDay, kPacket.m_kLastDay );

    // Get the Last day of last month
    if( kPacket.m_kToday.m_cMonth == 1 ) // If the month is January,then set year - 1, month = 12
    {
        m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear-1, 12,
            kNoUseDate, kPacket.m_kLastDayOfLastMonth );
    }
    else
    {
        m_kCalendar.GetMonthInfo( kPacket.m_kToday.m_sYear, kPacket.m_kToday.m_cMonth-1,
            kNoUseDate, kPacket.m_kLastDayOfLastMonth );
    }
    // 클라이언트에 보내줘야 함 
    SEND_PACKET( EVENT_LOAD_NEW_CALENDAR_NOT );

    START_LOG( clog, L"달력 정보 갱신, Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_mapCalendar.size() )
        << BUILD_LOG( kPacket.m_nPoint )
        << BUILD_LOG( kPacket.m_nAccTime )
        << BUILD_LOG( kPacket.m_kToday.m_sYear  )
        << BUILD_LOGc( kPacket.m_kToday.m_cMonth  )
        << BUILD_LOGc( kPacket.m_kToday.m_cDay )
        << BUILD_LOGc( kPacket.m_kToday.m_cWeekDay )
        << BUILD_LOGc( kPacket.m_kFirstDay.m_cDay )
        << BUILD_LOGc( kPacket.m_kLastDay.m_cDay ) << END_LOG;

    if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
    {
        m_kCalendar.Dump();
    }
}

// 주개근은  서버에서 자동처리하지 않음.
IMPL_ON_FUNC( EVENT_WEEKLY_ATTEND_REQ )
{
    if ( false == SiKGCPoint()->IsGCPointSwitch() ) {
        return;
    }

    KEVENT_WEEKLY_ATTEND_ACK kPacket;
    kPacket.m_cMonth = -1;
    kPacket.m_cWeekID = kPacket_.m_cWeekID;
    kPacket.m_nIncGCPoint = 0;
    kPacket.m_nGCPoint = m_kGCPoint.GetGCPoint();
    kPacket.m_nFailResult = 0;
    kPacket.m_nOK = -99;

    std::vector<KItem> vecWeeklyAttendanceReward;
    vecWeeklyAttendanceReward.clear();

    std::vector<KItem>  vecBaseReward;
    KItem               kRandomReward;
    vecBaseReward.clear();

    // check week attendance and get month
    if ( false == m_kCalendar.SetWeeklyAttendance( kPacket_.m_cWeekID, kPacket.m_cMonth ) ) {
        // 내부에서 에러값 설정
        goto END_PROC;
    }

    //주간 출석시 헬모드 입장권 지급
    CheckHellItemAttendance( SiKGCHelper()->GetWeekAttendHellCount() );

    //주개근 성공시, 기본 보상 있으면 받아온다.
    if ( SiKGSSimLayer()->GetWeeklyBaseRewardItem( vecBaseReward ) ) {
        vecWeeklyAttendanceReward = vecBaseReward;
    }

    //주개근 랜덤 보상 있으면 받아온다.
    if ( SiKGSSimLayer()->GetWeeklyRandomRewardItem( kRandomReward ) ) {
        vecWeeklyAttendanceReward.push_back( kRandomReward );
    }

    //보내온 선택보상 아이템이 아이템 리스트에 존재하는지 확인한다.
    if ( SiKGSSimLayer()->GetWeeklySelectRewardItem( kPacket_.m_kReqItem.m_ItemID, kPacket_.m_kReqItem ) ) {
        vecWeeklyAttendanceReward.push_back( kPacket_.m_kReqItem );
    }
    else if ( kPacket_.m_kReqItem.m_ItemID == 0 ) {
        START_LOG( clog, L"설정된 주개근 선택보상이 없음." ) << END_LOG;
    }
    else {
        SiKGSSimLayer()->DumpWeeklyReward();
        START_LOG( cerr, L"요청한 주개근 보상이 보상리스트에 존재하지 않음. ItemID : " << kPacket_.m_kReqItem.m_ItemID << L", Login : " << GetName() ) << END_LOG;
        //SET_ERR_GOTO( ERR_GCPOINT_13, END_PROC );
    }

    // 주개근 GCPoint지급
    kPacket.m_nIncGCPoint = SiKGCPoint()->GetGCPointWeek( static_cast<int>( kPacket.m_cMonth ), static_cast<int>( kPacket_.m_cWeekID ) );
    kPacket.m_nIncGCPoint = std::max<int>( kPacket.m_nIncGCPoint, 0 );
    kPacket.m_nGCPoint = m_kGCPoint.GetGCPoint();

    {
        KDB_EVENT_WEEKLY_ATTEND_REWARD_REQ kDBPacket;
        kDBPacket.m_cMonth     = kPacket.m_cMonth;
        kDBPacket.m_cWeekID    = kPacket.m_cWeekID;
        kDBPacket.m_nIncGCPoint= kPacket.m_nIncGCPoint;
        kDBPacket.m_nGCPoint   = kPacket.m_nGCPoint;
        kDBPacket.m_vecReqItem = vecWeeklyAttendanceReward;
        kDBPacket.m_nAccTime   = m_kCalendar.GetAccTime();
        QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_WEEKLY_ATTEND_REWARD_REQ, kDBPacket );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,          kPacket.m_nOK, 0 ); // 주개근 성공
        ERR_CASE( ERR_CALENDAR_01, kPacket.m_nOK, 1 ); // 달력에 해당 주 출석 정보가 없다
        ERR_CASE( ERR_CALENDAR_02, kPacket.m_nOK, 2 ); // 해당 주의 출첵이 주개근 조건에 못미침
        ERR_CASE( ERR_CALENDAR_08, kPacket.m_nOK, 3 ); // 이미 주개근 했음
        ERR_CASE( ERR_CALENDAR_06, kPacket.m_nOK, 4 ); // 출석체크 할 수 있는 날 존재
        ERR_CASE( ERR_CALENDAR_07, kPacket.m_nOK, 5 ); // 오늘 누적시간 한시간 미만
        ERR_CASE( ERR_CALENDAR_09, kPacket.m_nOK, 6 ); // 달력 정보 로드되지 않음
        ERR_CASE( ERR_GCPOINT_12,  kPacket.m_nOK, 7 ); // 주개근 보상 Item얻기 실패
        ERR_CASE( ERR_GCPOINT_13,  kPacket.m_nOK, 8 ); // 요청한 주개근 보상이 보상리스트에 존재하지 않음
        ERR_CASE( ERR_CALENDAR_15, kPacket.m_nOK, 9 ); // 주개근 출석포인트 DB저장 실패
    default:
        kPacket.m_nOK = -99;
        SET_ERROR( ERR_UNKNOWN );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"주개근 처리 결과.. OK : " << kPacket.m_nOK )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( kPacket.m_cWeekID )
        << BUILD_LOG( kPacket.m_nIncGCPoint )
        << BUILD_LOG( kPacket.m_nGCPoint )
        << BUILD_LOGc( kPacket.m_cMonth )
        << BUILD_LOG( kPacket_.m_kReqItem.m_ItemID )
        << BUILD_LOGc( vecWeeklyAttendanceReward.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_WEEKLY_ATTEND_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_GCPOINT_RECOMMEND_REQ, std::wstring )
{
    if( SiKGCPoint()->IsGCPointSwitch() == false ) // 쥐씨 포인트 시스템 사용하지 않으면 
    {
        return;
    }
    boost::trim( kPacket_ );

    if( kPacket_.empty() )
    {
        QUEUEING_EVENT_TO_DB( EVENT_GCPOINT_RECOMMEND_REQ, kPacket_ );
        START_LOG( clog, L"친구 추천 하지 않음... Name : " << m_strName ) << END_LOG;
        return;
    }

    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_ ) )
    {
        int kPacket = 2; // 사용할수 없는 문자 포함.
        SEND_PACKET( EVENT_GCPOINT_RECOMMEND_ACK );
        START_LOG( cwarn, L"추천 대상 유저의 닉네임중 중에 부적절한 것이 있습니다.(빈칸, 탭, \' 등 )  Name : " << m_strName ) << END_LOG;
        return;
    }

    if( m_strNickName == kPacket_ )
    {
        int kPacket = 3; // 자기 자신.
        SEND_PACKET( EVENT_GCPOINT_RECOMMEND_ACK );
        START_LOG( cwarn, L"자기 자신을 추천 하려고 했음. " )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( m_strNickName ) << END_LOG;
        return;
    }

    QUEUEING_EVENT_TO_DB( EVENT_GCPOINT_RECOMMEND_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_GCPOINT_RECOMMEND_ACK, int )
{
    //    3 자기 자신.
    //    2 사용할수 없는 문자 포함.
    //    1 닉네임 길이가 0
    //    0 
    //    -1(추천하는 유저의 포인트 정보가 없음),
    //    -2(추천받은 유저가 존재하지 않음),
    //    -3(추천받은 유저와 추천한 유저가 동일함).
    //    -4(추천받은 유저의 포인트 정보가 없음),
    //    -11이하(트랜잭션 에러)

    KOKnValue kPacket;
    kPacket.m_nOK       = kPacket_;
    kPacket.m_nValue    = 0;
    int nIncGCPoint = 0;
    SET_ERROR( ERR_UNKNOWN );

    NetError::SetLastNetError( kPacket.m_nOK );
    switch( kPacket.m_nOK )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_GCPOINT_00, kPacket.m_nOK, -1 ); // 추천하는 유저의 포인트 정보가 없음
        ERR_CASE( ERR_GCPOINT_01, kPacket.m_nOK, -2 ); // 추천받은 유저가 존재하지 않음
        ERR_CASE( ERR_GCPOINT_02, kPacket.m_nOK, -3 ); // 추천받은 유저와 추천한 유저가 동일함
        ERR_CASE( ERR_GCPOINT_03, kPacket.m_nOK, -4 ); // 추천받은 유저의 포인트 정보가 없음
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"알수 없는 오류값.. Name : " << m_strName ) << END_LOG;
        break;
    }

    if( kPacket.m_nOK == 0 )
    {
        nIncGCPoint = SiKGCPoint()->GetRecommendGCPoint();
        m_kGCPoint.IncreaseGCPoint( IGT_RECOMMEND, nIncGCPoint );
        kPacket.m_nValue = m_kGCPoint.GetGCPoint();
    }

    SEND_PACKET( EVENT_GCPOINT_RECOMMEND_ACK );
    LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == -2 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK)
        << BUILD_LOG( nIncGCPoint )
        << BUILD_LOG( kPacket.m_nValue )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_MONTHLY_ATTEND_REWARD_REQ )
{
    if( SiKGCPoint()->IsGCPointSwitch() == false ) // 쥐씨 포인트 시스템 사용하지 않으면 
    {
        START_LOG( clog, L"GC Point System Not able" ) << END_LOG;
        return;
    }

    KDB_EVENT_MONTHLY_ATTEND_REWARD_REQ kDBPacket;
    KEVENT_MONTHLY_ATTEND_REWARD_ACK    kPacket;
    std::vector<KItem>                  vecBaseReward;
    KItem                               kRandomReward;

    kDBPacket.m_kReqDate = kPacket_.m_kReqDate;
    kDBPacket.m_vecReqItem.clear();
    vecBaseReward.clear();
    kPacket.m_nOK = -99;

    //월개근 기본 보상 있으면 받아온다.
    if( SiKGSSimLayer()->GetMonthlyBaseRewardItem( vecBaseReward ) )
    {
        kDBPacket.m_vecReqItem = vecBaseReward;
    }
    //월개근 랜덤 보상 있으면 받아온다.
    if( SiKGSSimLayer()->GetMonthlyRandomRewardItem( kRandomReward ) )
    {
        kDBPacket.m_vecReqItem.push_back( kRandomReward );
    }
    //보내온 선택보상 아이템이 아이템 리스트에 존재하는지 확인한다.(받드시 존재한다)
    if( SiKGSSimLayer()->GetMonthlySelectRewardItem( kPacket_.m_kReqItem.m_ItemID, kPacket_.m_kReqItem ) )
    {
        kDBPacket.m_vecReqItem.push_back( kPacket_.m_kReqItem );
    }
    else if( kPacket_.m_kReqItem.m_ItemID == -1 )
    {
        START_LOG( clog, L"설정된 월개근 선택보상이 없음." ) << END_LOG;
    }
    else
    {
        kPacket.m_nOK = 1;
        SEND_PACKET( EVENT_MONTHLY_ATTEND_REWARD_ACK );
        START_LOG( cerr, L"요청한 월개근 보상 아이템이 보상 리스트에 존재하지 않음., Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_kReqItem.m_ItemID ) << END_LOG;
        SiKGSSimLayer()->DumpMonthlyReward();
        return;
    }

    if( kDBPacket.m_vecReqItem.empty() )
    {
        kPacket.m_nOK = 1;
        SEND_PACKET( EVENT_MONTHLY_ATTEND_REWARD_ACK );
        START_LOG( cerr, L"월개근 보상 아이템이 없음,Name : " << m_strName ) << END_LOG;
        return;
    }

    // 존재한다면 해당 날짜와 함께 DB로 보낸다.
    START_LOG( clog, L"월개근 보상 아이템 DB에 요청,Name : " << m_strName )
        << BUILD_LOG( kDBPacket.m_vecReqItem.size() ) << END_LOG;

    QUEUEING_EVENT_TO_DB( DB_EVENT_MONTHLY_ATTEND_REWARD_REQ, kDBPacket );
}

IMPL_ON_FUNC( EVENT_MONTHLY_ATTEND_REWARD_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    // 오류값이 있는지 확인한다.
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( kPacket_.m_nOK != 0 )
    {
        goto END_PROC;
    }

    m_kInventory.AddItemList( kPacket_.m_vecInv );

END_PROC:
    switch( kPacket_.m_nOK )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GCPOINT_05, kPacket_.m_nOK, 2 ); // 포인트백 정보가 없음
        ERR_CASE( ERR_GCPOINT_06, kPacket_.m_nOK, 3 ); // 유저의 해당월 정보가 없음)
        ERR_CASE( ERR_GCPOINT_07, kPacket_.m_nOK, 4 ); // ItemID가 -1이 입력되었음)
        ERR_CASE( ERR_GCPOINT_08, kPacket_.m_nOK, 5 ); // 월개근 보상 Item얻기 실패)
    default:
        START_LOG( cerr, L" 월개근 보상시 알수 없는 에러.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        SET_ERROR( ERR_UNKNOWN );
    }
    SEND_RECEIVED_PACKET( EVENT_MONTHLY_ATTEND_REWARD_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( kPacket_.m_vecReservedReward.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT, std::vector<KSimpleDate> )
{
    SEND_RECEIVED_PACKET( EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT );
    START_LOG( clog, L"월개근 보상 받을것이 있는가??.. Name : " << m_strName )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_LUCKYDAY_NOT )
{
    JIF( PacketHackCheck( nFrom_ ) );
    // 럭키데이 아이템은 유저가 선택할 수 없다. 럭키데이 아이템 다 받게되므로, 인벤에 다 집어넣고 전달.
    // 에러값은 처리해야 할텐데.
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( kPacket_.m_nOK != 0 )
    {
        goto END_PROC;
    }
    m_kInventory.AddItemList( kPacket_.m_vecInv );

END_PROC:
    switch( kPacket_.m_nOK )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_LUCKYDAY_00, kPacket_.m_nOK, 1 ); // 럭키데이 아이템 없음
        ERR_CASE( ERR_LUCKYDAY_01, kPacket_.m_nOK, 2 ); // 럭키데이 아이템 얻기 실패
    default:
        START_LOG( cerr, L" 럭키데이 보상시 알 수 없는 에러.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        SET_ERROR( ERR_UNKNOWN );
    }

    SEND_RECEIVED_PACKET( EVENT_LUCKYDAY_NOT );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )    
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kDailyInfo.m_kDate.m_sYear )
        << BUILD_LOGc( kPacket_.m_kDailyInfo.m_kDate.m_cMonth )
        << BUILD_LOGc( kPacket_.m_kDailyInfo.m_kDate.m_cDay )
        << BUILD_LOGc( kPacket_.m_kDailyInfo.m_cSealType )
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

// 알 부화
_IMPL_ON_FUNC( EVENT_HATCHING_EGG_REQ, GCITEMUID )
{
    KPetInfo* pkPetInfo = NULL;
    KItemPtr pkItem;

    KEVENT_HATCHING_EGG_ACK kPacket;
    KEVENT_DB_HATCHING_EGG_REQ kDBPacket;

    SET_ERROR( ERR_UNKNOWN );
    // 해당 펫(알)이 존재하는가?
    if( !GetPetInfo( kPacket_, &pkPetInfo ) )
    {
        SET_ERR_GOTO( ERR_PET_07, END_PROC );
    }

    // 해당 펫이 알인가?
    if( !SiKPetHelper()->IsEggPet( pkPetInfo->m_dwID ) )
    {
        SET_ERR_GOTO( ERR_PET_32, END_PROC );
    }

    if( pkPetInfo->m_nHatchingID < 0 )
    {
        SET_ERR_GOTO( ERR_PET_33, END_PROC );
    }

    // 요청한 부화 아이템이 존재하는가?
    pkItem = m_kInventory.FindItemByItemID( HATCHING_ITEM_ID );
    if( pkItem == NULL )
    {
        SET_ERR_GOTO( ERR_INVEN_01, END_PROC );
    }

    if( pkItem->m_nCount <= 0 )
    {
        START_LOG( cerr, L" 아이템 개수가 모자라면 보내지 말란 말이다. Name : " << m_strName ) << END_LOG;
        SET_ERR_GOTO( ERR_INVEN_02, END_PROC );
    }

    // 해당 펫의 부화가능 레벨인가?
    if( SiKPetHelper()->GetHatchingLv( pkPetInfo->m_dwID ) > pkPetInfo->m_iLevel )
    {
        START_LOG( cerr, L"레벨이 부족하면 보내지 말란 말이다. Name : " << m_strName )
            << BUILD_LOG( pkPetInfo->m_iLevel )
            << BUILD_LOG( pkPetInfo->m_dwID ) << END_LOG;
        SET_ERR_GOTO( ERR_PET_34, END_PROC );
    }

    // 새 펫의 이름을 얻어 온다.
    if( !SiKLoudMessage()->GetItemName( static_cast<int>(pkPetInfo->m_nHatchingID), 0,  kDBPacket.m_strPetName, GetLanguageCode() ) )
    {
        START_LOG( cerr, L"팻 이름이 DB에 존재하지 않음. -.- Item ID : " << pkPetInfo->m_nHatchingID << L", Name : " << m_strName ) << END_LOG;
        SET_ERR_GOTO( ERR_PET_01, END_PROC );
    }

    //부화시에 필요한 정보를 셋팅한다.
    kDBPacket.m_dwNewPetID  = pkPetInfo->m_nHatchingID;
    kDBPacket.m_kOldPet     = *pkPetInfo;
    kDBPacket.m_cCurrentChar = GetCurrentCharType();

    QUEUEING_EVENT_TO_DB( EVENT_HATCHING_EGG_REQ, kDBPacket );

    START_LOG( clog, L" 펫 부화 합니다..." )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kDBPacket.m_kOldPet.m_dwID )
        << BUILD_LOG( kDBPacket.m_kOldPet.m_dwUID )
        << BUILD_LOG( kDBPacket.m_kOldPet.m_strName )
        << BUILD_LOG( kDBPacket.m_dwNewPetID )
        << BUILD_LOG( kDBPacket.m_strPetName ) << END_LOG;

    SET_ERROR( NET_OK );
    return;
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( ERR_PET_07, kPacket.m_nOK, 1 ); //펫 정보가 존재하지 않음
        ERR_CASE( ERR_PET_32, kPacket.m_nOK, 2 ); //알 펫이 아니라서 부화 불가능.
        ERR_CASE( ERR_PET_33, kPacket.m_nOK, 3 ); //부화 대상 정보가 존재하지 않음
        ERR_CASE( ERR_INVEN_01, kPacket.m_nOK, 4 ); //아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_INVEN_02, kPacket.m_nOK, 5 ); //아이템 수량이 부족.
        ERR_CASE( ERR_PET_34, kPacket.m_nOK, 6 ); //부화 하기에 레벨이 부족함. 
        ERR_CASE( ERR_PET_01, kPacket.m_nOK, 7 ); // 해당 펫 아이템을 가지고 있지 않음
    default:
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"알수 없는 오류 값 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_HATCHING_EGG_ACK );

    START_LOG( cerr, L" 펫 부화시 오류...Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_)
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_HATCHING_EGG_ACK )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }

    // 부화 아이템을 감소 시킨다.
    // 부화 아이템은 하나 뿐이니까...
    pkItem = m_kInventory.FindItemByItemID( HATCHING_ITEM_ID );
    if( pkItem == NULL )
    {
        START_LOG( cerr, L" 낙장 불입니다.. 그냥 가자.. name : " << m_strName )
            << BUILD_LOG( HATCHING_ITEM_ID ) << END_LOG;
    }
    else
    {
        pkItem->m_nCount -= 1;
    }

    // 옛날 펫을 제거하고
    if( !DeletePet( kPacket_.m_kOldPetInfo.m_dwUID ) )
    {
        START_LOG( cerr, L" 젠장 User에서 펫 제거하지 못했다.. 낙장불입.. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_kOldPetInfo.m_dwID )
            << BUILD_LOG( kPacket_.m_kOldPetInfo.m_dwUID ) << END_LOG;
    }

    {// 장착 정보를 해제 하고
        std::vector< char > vecChars;
        GetPetOwner( kPacket_.m_kOldPetInfo.m_dwUID, vecChars );

        std::vector<char>::iterator vit;
        for( vit = vecChars.begin() ; vit != vecChars.end() ; ++vit )
        {
            KCharacterInfo* pkCharacter;
            pkCharacter = GetCharacterInfo( *vit );
            if( pkCharacter == NULL )
            {
                START_LOG( cerr, L"펫 장착 정보를 제거하려는데 캐릭터가 존재하지 않음.. Name : " << m_strName )
                    << BUILD_LOGc( *vit ) << END_LOG;
                continue;
            }
            pkCharacter->m_kEquipPetInfo.Init();
        }
    }

    // 옛날 봉인 카드 제거하고
    m_kInventory.RemoveItem( kPacket_.m_kPetCard.m_ItemUID );

    // 새카드 넣고
    m_kInventory.AddItem( kPacket_.m_kPetCard );

    // 생성된것은 펫 인포에 추가한다.
    if( kPacket_.m_kPetInfo.m_dwUID > 0 )
    {
        // 검사하지 않겠다.
        m_mapPetInfo.insert( std::make_pair( kPacket_.m_kPetInfo.m_dwUID, kPacket_.m_kPetInfo ) );

        START_LOG( clog, L"따끈따끈한 펫의 정보는? " )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_dwUID )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_dwID )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_strName )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_mapInitExp.size() )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_dwEXP )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_iLevel )
            << BUILD_LOGc( kPacket_.m_kPetInfo.m_cPromotion )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_iInitSatiation )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_iSatiation )
            << BUILD_LOG( kPacket_.m_kPetInfo.m_vecEquipItem.size() ) << END_LOG;
    }

    // 보상으로 받은 아이템 적용하고
    m_kInventory.AddItemList( kPacket_.m_vecItem );
    kPacket_.m_dwHatchingID = HATCHING_ITEM_ID;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_PET_03, kPacket_.m_nOK, 8 ); //해당 유저가 존재하지 않음
        ERR_CASE( ERR_PET_04, kPacket_.m_nOK, 9 ); //펫 아이템이 아님..
        ERR_CASE( ERR_PET_06, kPacket_.m_nOK, 10 ); //펫 아이템(시민권)이 없음
        ERR_CASE( ERR_PET_07, kPacket_.m_nOK, 1 ); //펫 정보가 존재하지 않음
        ERR_CASE( ERR_PET_35, kPacket_.m_nOK, 11 ); //부화시 새펫 봉인 카드 넣기 실패.
        ERR_CASE( ERR_PET_01, kPacket_.m_nOK, 7 ); //해당 펫 아이템을 가지고 있지 않음
        ERR_CASE( ERR_PET_02, kPacket_.m_nOK, 12 ); //신규 생성하려는 팻이 이미 존재함
        ERR_CASE( ERR_PET_05, kPacket_.m_nOK, 13 ); //해당 캐릭터가 존재하지 않음.
    default:
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_HATCHING_EGG_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kPetCard.m_ItemID )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;
}

_IMPL_ON_FUNC(EVENT_BUY_GCPOINT_ITEM_REQ, KBuySellItemReq)
{
    KEVENT_BUY_GCPOINT_ITEM_ACK kPacket;
    KDB_EVENT_BUY_GCPOINT_ITEM_REQ kDBPacket;

    SET_ERROR(ERR_UNKNOWN);

    if (SiKGCPoint()->IsGCPointSwitch() == false)
        return;

    kDBPacket.m_kBuyInfo = kPacket_;
    GetDBUpdateData(kDBPacket.m_kUserData);
    FillBuyerData(kDBPacket.m_kBuyInfo);

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_BUY_GCPOINT_ITEM_REQ, kDBPacket);

    START_LOG(clog, L"GCPoint Log Name : " << m_strName)
        << BUILD_LOG(kPacket_.m_ItemID)
        << BUILD_LOG(kPacket_.m_nCount)
        << BUILD_LOG(kPacket_.m_nPeriod)
        << BUILD_LOG(kPacket_.m_nMoneyType)
        << BUILD_LOG(m_kGCPoint.GetInitGCPoint())
        << BUILD_LOG(m_kGCPoint.GetGCPoint()) << END_LOG;

    SET_ERROR(NET_OK);
END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98);
    default:
        kPacket.m_nOK = -99;
        START_LOG(cerr, L"GCPoint Error, Name : " << m_strName)
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
    }

    LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK || kPacket.m_nOK == -98)
        << NetError::GetLastNetErrMsg() << L", Name : " << m_strName << END_LOG;

    if (NetError::GetLastNetError() != NetError::NET_OK)
        SEND_PACKET(EVENT_BUY_GCPOINT_ITEM_ACK);
}

IMPL_ON_FUNC(EVENT_BUY_GCPOINT_ITEM_ACK)
{
    JIF(PacketHackCheck(nFrom_));

    if (kPacket_.m_nOK != 0)
        goto END_PROC;

    m_kGCPoint.DecreaseGCPoint(DGT_BUY_GPPOINT_ITEM, kPacket_.m_nGCPoint);
    m_kGCPoint.SetInitGCPoint(m_kGCPoint.GetGCPoint());
    kPacket_.m_nGCPoint = m_kGCPoint.GetGCPoint();

    m_kInventory.AddItemList(kPacket_.m_vecItem);

END_PROC:
    SEND_RECEIVED_PACKET(EVENT_BUY_GCPOINT_ITEM_ACK);

    LOG_SUCCESS(kPacket_.m_nOK == 0 || kPacket_.m_nOK == -5)
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_.m_nOK)
        << BUILD_LOG(kPacket_.m_nGCPoint)
        << BUILD_LOG(kPacket_.m_vecItem.size()) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SELL_GCPOINT_ITEM_REQ, KBuySellItemReq )
{
    if( SiKGCPoint()->IsGCPointSwitch() == false ) // 쥐씨 포인트 시스템 사용하지 않으면 
    {
        return;
    }
    KEVENT_SELL_GCPOINT_ITEM_ACK kPacket;
    kPacket.m_nOK = -99;
    
    if( m_kInventory.FindItem( kPacket_.m_ItemID, kPacket_.m_ItemUID ) == NULL )
    {
        SET_ERR_GOTO( ERR_INVEN_05, END_PROC );
    }

    QUEUEING_EVENT_TO_DB( EVENT_SELL_GCPOINT_ITEM_REQ, kPacket_ );
    SET_ERROR( NET_OK );

    START_LOG( clog, L"GC Point 아이템 제거.." )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOGc( kPacket_.m_nItemType ) << END_LOG;
    return;
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE(ERR_INVEN_05, kPacket.m_nOK, 1 ); // 아이템이 존재하지 않음.
        ERR_CASE(ERR_INVEN_08, kPacket.m_nOK, 2 ); // 아이템 타입 정보가 올바르지 않음
    default:
        kPacket.m_nOK = -99; 
    }

    SEND_PACKET( EVENT_SELL_GCPOINT_ITEM_ACK );
    START_LOG( cerr, L"GC Point 아이템 제거 실패.." )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nItemType )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_SELL_GCPOINT_ITEM_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    if( kPacket_.m_nOK != 0 )
    {
        SEND_RECEIVED_PACKET( EVENT_SELL_GCPOINT_ITEM_ACK );
        START_LOG( cerr, L"GP Point 아이템 제거 실패 " )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kPacket_.m_kItemInfo.m_dwID )
            << BUILD_LOG( kPacket_.m_kItemInfo.m_dwUID )
            << BUILD_LOGc( kPacket_.m_kItemInfo.m_cItemType ) << END_LOG;
        return;
    }

    if( m_kInventory.RemoveItem( kPacket_.m_kItemInfo.m_dwUID ) == false )
    {
        START_LOG( cerr, L"낙장 불입이다... 젠장. Name : " << m_strName )
            << BUILD_LOGc( kPacket_.m_kItemInfo.m_cItemType )
            << BUILD_LOG( kPacket_.m_kItemInfo.m_dwID )
            << BUILD_LOG( kPacket_.m_kItemInfo.m_dwUID ) << END_LOG;
    }
    ReleaseQuickSlot( kPacket_.m_kItemInfo.m_dwUID );

    SEND_RECEIVED_PACKET( EVENT_SELL_GCPOINT_ITEM_ACK );
    START_LOG( clog, L"GP Point 아이템 제거 " )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItemInfo.m_dwID )
        << BUILD_LOG( kPacket_.m_kItemInfo.m_dwUID )
        << BUILD_LOGc( kPacket_.m_kItemInfo.m_cItemType ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_RESET_WIN_LOSE_REQ, KResetWinLoseData )
{
    // 상태가 올바른가
    _VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
    kPacket_.m_vecChar.clear();
    SET_ERROR( ERR_UNKNOWN );

    KItemPtr pkResetItem;

    switch( kPacket_.m_ItemID )
    {
    case RESET_WIN_LOSE_ALL:
        {
            // 현재 캐릭터에 한해서 사용될 수 있도록 수정.
            kPacket_.m_vecChar.push_back( m_cCurrentCharacter );
        }
        break;
    case RESET_WIN_LOSE_ELESIS:
        if( !IsCharExist( GC_CHAR_ELESIS ) )
        {
            SET_ERR_GOTO( ERR_RESET_WINLOSE_01, END_PROC );
        }
        kPacket_.m_vecChar.push_back( GC_CHAR_ELESIS );
        break;
    case RESET_WIN_LOSE_LIRE:
        if( !IsCharExist( GC_CHAR_LIRE ) )
        {
            SET_ERR_GOTO( ERR_RESET_WINLOSE_01, END_PROC );
        }
        kPacket_.m_vecChar.push_back( GC_CHAR_ELESIS );
        break;
    case RESET_WIN_LOSE_ARME:
        if( !IsCharExist( GC_CHAR_ARME ) )
        {
            SET_ERR_GOTO( ERR_RESET_WINLOSE_01, END_PROC );
        }
        kPacket_.m_vecChar.push_back( GC_CHAR_ELESIS );
        break;
    case RESET_WIN_LOSE_LAS:
        if( !IsCharExist( GC_CHAR_LAS ) )
        {
            SET_ERR_GOTO( ERR_RESET_WINLOSE_01, END_PROC );
        }
        kPacket_.m_vecChar.push_back( GC_CHAR_ELESIS );
        break;
    case RESET_WIN_LOSE_RYAN:
        if( !IsCharExist( GC_CHAR_RYAN ) )
        {
            SET_ERR_GOTO( ERR_RESET_WINLOSE_01, END_PROC );
        }
        kPacket_.m_vecChar.push_back( GC_CHAR_ELESIS );
        break;
    default:
        SET_ERR_GOTO( ERR_RESET_WINLOSE_00, END_PROC );
    }

    // 아이템이 존재 하는가?
    pkResetItem = m_kInventory.FindItemByItemID( kPacket_.m_ItemID );
    if( pkResetItem == NULL || pkResetItem->m_nCount <= 0 )
    {
        SET_ERR_GOTO( ERR_INVEN_01, END_PROC );
    }

    QUEUEING_EVENT_TO_DB( EVENT_RESET_WIN_LOSE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RESET_WINLOSE_00, kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_RESET_WINLOSE_01, kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_RESET_WINLOSE_02, kPacket_.m_nOK, 3 );
    default:
        START_LOG( cerr, NetError::GetLastNetErrMsg() << L"Name : " << m_strName ) << END_LOG;
    }

    if( kPacket_.m_nOK != 0 )
        SEND_RECEIVED_PACKET( EVENT_RESET_WIN_LOSE_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << L"승률 초기화 요청 패킷.. Name : " << m_strName << dbg::endl
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_vecChar.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_RESET_WIN_LOSE_ACK, KResetWinLoseData )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    //캐릭터들 초기화 하기...
    { // goto 땜시롱 브래이스 친다...
        std::vector<char>::iterator vit;
        for( vit = kPacket_.m_vecChar.begin() ; vit != kPacket_.m_vecChar.end() ; ++vit )
        {
            KCharacterInfo* pkCharacterInfo = NULL;
            pkCharacterInfo = GetCharacterInfo( *vit );
            if( pkCharacterInfo == NULL )
            {
                START_LOG( cerr, L"승률 초기화할 캐릭터를 찾을수 없음 Name : " << m_strName 
                    << L", Char : " << static_cast<int>(*vit)  ) << END_LOG;
                continue;
            }

            pkCharacterInfo->m_iLose = 0;
            pkCharacterInfo->m_iWin = 0;
            pkCharacterInfo->m_iInitWin = 0;
            pkCharacterInfo->m_iInitLose = 0;
        }
    }

    {// 아이템을 까봅시다...
        KItemPtr pkResetItem = m_kInventory.FindItemByItemID( kPacket_.m_ItemID );

        if( pkResetItem == NULL )
        {
            START_LOG( cerr, L"캐릭터 승률 초기화시 해당 아이템이 존재하지 않음 '낙장불입' " )
                << BUILD_LOG( m_strName )
                << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;
            goto END_PROC;
        }

        if( pkResetItem->m_nCount <= 0 )
        {
            START_LOG( cerr, L"캐릭터 승률 초기화시 해당 아이템 수량이 부족 '낙장불입' " )
                << BUILD_LOG( m_strName )
                << BUILD_LOG( kPacket_.m_ItemID )
                << BUILD_LOG( pkResetItem->m_nCount ) << END_LOG;
            goto END_PROC;
        }

        if( pkResetItem->m_nCount > 0 )
            pkResetItem->m_nCount -= 1;
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_RESET_WINLOSE_00, kPacket_.m_nOK, 1 ); // 승률 초기화 아이템이 아님니다.
        ERR_CASE( ERR_RESET_WINLOSE_01, kPacket_.m_nOK, 2 ); // 아이템과 캐릭터의 쌍이 맞지 않음.
        ERR_CASE( ERR_RESET_WINLOSE_02, kPacket_.m_nOK, 3 ); // 승률 초기화 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_RESET_WINLOSE_03, kPacket_.m_nOK, 4 ); // 캐릭터 정보가 없습니다.
        ERR_CASE( ERR_RESET_WINLOSE_04, kPacket_.m_nOK, 5 ); // 해당 유저가 존재하지 않습니다.
        ERR_CASE( ERR_RESET_WINLOSE_05, kPacket_.m_nOK, 6 ); // 캐릭터 타입이 잘못 되었습니다.
        ERR_CASE( ERR_RESET_WINLOSE_06, kPacket_.m_nOK, 7 ); // 캐릭터테이블에 캐릭터 정보가 없습니다.
    default:
        kPacket_.m_nOK = 8; //알수 없는 오류
        START_LOG( cerr, NetError::GetLastNetErrMsg() << L", Name : " << m_strName ) << END_LOG;
    }
    SEND_RECEIVED_PACKET( EVENT_RESET_WIN_LOSE_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_vecChar.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_ROOM_OPTION_REQ, KRoomOption )
{
    _VERIFY_STATE(( 1, KUserFSM::STATE_ROOM )); // 이 패킷은 방장만이 보내기 때문에 타이밍 틀리다면 문제 있음..

    SET_ERROR( ERR_UNKNOWN );
    JIF( m_pkChannel );
    JIF( m_pkRoom );

    if( !m_pkRoom->GetHostPtr() )
    {
        SET_ERR_GOTO( ERR_ROOM_39, END_PROC ); // 방장 또는 운영자가 아님.
    }

    if( m_pkRoom->GetHostPtr() != this && !IsAdmin() )
    {
        SET_ERR_GOTO( ERR_ROOM_20, END_PROC ); // 방장 또는 운영자가 아님.
    }

    // 방제는 올바른가?
    if( kPacket_.m_strRoomTitle.compare( m_pkRoom->GetName() ) != 0 && SiKSlangFilter()->HasSlang( kPacket_.m_strRoomTitle ) )
    {
        SET_ERR_GOTO( ERR_ROOM_46, END_PROC );
    }

    if( kPacket_.m_strPassWord.compare( m_pkRoom->m_strPasswd ) != 0 && SiKSlangFilter()->HasSlang( kPacket_.m_strPassWord ) )
    {
        // 비번은 방유저들 모두 보기 때문에 필터링 필요
        SET_ERR_GOTO( ERR_ROOM_46, END_PROC );
    }

    kPacket_.m_nDeathKillCount = std::max<int>( 0, kPacket_.m_nDeathKillCount );
    kPacket_.m_nDeathLimitTime = std::max<int>( 0, kPacket_.m_nDeathLimitTime );

    if( !m_pkRoom->ChangeRoomOption( kPacket_ ) )
    {
        goto END_PROC;
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ROOM_39, kPacket_.m_nOK, 1 ); // 방정보 변경 실패. 방장 부재
        ERR_CASE( ERR_ROOM_20, kPacket_.m_nOK, 2 ); // 방정보 변경 실패. 방장 또는 운영자가 아님
        ERR_CASE( ERR_ROOM_46, kPacket_.m_nOK, 3 ); // 방제나 비번에 금칙어 포함.
        ERR_CASE( ERR_ROOM_55, kPacket_.m_nOK, 4 ); // 올바른 상태가 아님.
        ERR_CASE( ERR_ROOM_58, kPacket_.m_nOK, 5 ); // 데쓰매치 옵션 설정이 잘못 되었음.
    default:
        START_LOG( cerr, L"알수없는 오류 정보, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    if( kPacket_.m_nOK == 0 )
    {   // 나를 제외하고 방송
        GetRoom()->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_CHANGE_ROOM_OPTION_BROAD, kPacket_ );
    }
    else
    {
        SEND_RECEIVED_PACKET( EVENT_CHANGE_ROOM_OPTION_ACK );
    }

    // pkRoom 은 존재한다..
    LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 3 )
        << L"방 옵션 변경 오청 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strRoomTitle )
        << BUILD_LOG( kPacket_.m_strPassWord )
        << BUILD_LOG( kPacket_.m_nDeathLimitTime )
        << BUILD_LOG( kPacket_.m_nDeathKillCount )
        << BUILD_LOG( kPacket_.m_bDeathMatchIntrudable )
        << BUILD_LOG( kPacket_.m_bDeathMatchBalancing )
        << BUILD_LOG( m_pkRoom->GetName() )
        << BUILD_LOG( m_pkRoom->m_strPasswd ) << END_LOG;
        
}

_IMPL_ON_FUNC( EVENT_YAHOO_BENEFIT_NOT, KInven_GP )
{
    kPacket_.m_strComment = SiKGameServer()->GetPubEventComment();
    m_kInventory.AddItemList( kPacket_.m_vecItem );
    IncreaseGP( KGpStatInfo::GIT_YAHOO_BENEFIT, false, kPacket_.m_nGP );

    SEND_RECEIVED_PACKET( EVENT_YAHOO_BENEFIT_NOT );

    START_LOG( clog, L"야후 이벤트.. Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kPacket_.m_strComment ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_STAT_LOADING_TIME_NOT, DWORD )
{
    SiKGSSimLayer()->AddLogingCount( kPacket_ );
}

_IMPL_ON_FUNC( EVENT_MSG_NOT, std::wstring )
{
    SEND_RECEIVED_PACKET( EVENT_MSG_NOT );

    START_LOG( clog, L"유저에게 알림 메세지.. Name : " << GetName() )
        << BUILD_LOG( kPacket_) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GET_NASTY_INFO_NOT )
{
    m_nReportCount = kPacket_.m_nReport; // 오늘 신고한 횟수
    kPacket_.m_nReport = SiKGSSimLayer()->m_nMaxReportNum - m_nReportCount;

    SEND_RECEIVED_PACKET( EVENT_GET_NASTY_INFO_NOT );

    std::wstringstream stm;
    std::vector< std::pair<int,int> >::iterator vit;
    for( vit = kPacket_.m_vecHackCount.begin() ; vit != kPacket_.m_vecHackCount.end() ; ++vit )
    {
        stm << L"(" << vit->first << L"," << vit->second << L"),";
    }

    START_LOG( clog, L"납흔 쉐퀴들... Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_vecHackCount.size() )
        << stm.str() << dbg::endl
        << BUILD_LOG( kPacket_.m_nReported )
        << BUILD_LOG( m_nReportCount )
        << BUILD_LOG( kPacket_.m_nReport ) << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

IMPL_ON_FUNC( EVENT_REPORT_USER_REQ ) // 신고..
{
    // 상태 확인.
    _VERIFY_STATE((3, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING, KUserFSM::STATE_SQUARE ));
    KEVENT_REPORT_USER_ACK kPacket;
    kPacket.m_nOK           = -99;
    kPacket.m_dwSerialNum   = 0;
    kPacket.m_nCount        = SiKGSSimLayer()->m_nMaxReportNum - m_nReportCount; // 남은 신고 횟수 = 최대신고 - 누적신고

    // 신고 횟수 확인..
    if( SiKGSSimLayer()->m_nMaxReportNum <= m_nReportCount )
    {
        // 너무 많이 신고 했음.
        SET_ERR_GOTO( ERR_REPORT_00, END_PROC ); // 이미 신고 횟수 초과
    }

    if( kPacket_.m_vecUser.empty() )
    {
        SET_ERR_GOTO( ERR_REPORT_01, END_PROC ); //신고 대상이 존재하지 않음.
    }

    if( kPacket_.m_nReason < KEVENT_REPORT_USER_REQ::RPT_SLANDER ||
        kPacket_.m_nReason >= KEVENT_REPORT_USER_REQ::RPT_MAX )
    {
        SET_ERR_GOTO( ERR_REPORT_02, END_PROC ); //신고 타입이 등록된 타입이 아님.
    }

    // 신고 처리.. ( to DB)
    QUEUEING_EVENT_TO_DB( EVENT_REPORT_USER_REQ, kPacket_ );
    return; // 여기서 리턴하면 아래  로그 안남을텐데..

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( ERR_REPORT_00, kPacket.m_nOK, 1 );   //이미 신고 횟수 초과.
        ERR_CASE( ERR_REPORT_01, kPacket.m_nOK, 2 );   //신고 대상이 존재하지 않음.
        ERR_CASE( ERR_REPORT_02, kPacket.m_nOK, 3 );   //신고 타입이 등록된 타입이 아님.
        ERR_CASE( ERR_REPORT_03, kPacket.m_nOK, 4 );   //신고한 유저 정보 없음 
        ERR_CASE( ERR_REPORT_04, kPacket.m_nOK, 5 );   //해당 신고 타입이 없음
        ERR_CASE( ERR_REPORT_05, kPacket.m_nOK, 6 );   //신고당한 유저 정보 없음
    default:
        START_LOG( cerr, L"알수 없는 오류 " )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    };
    // 
    SEND_PACKET( EVENT_REPORT_USER_ACK );

    START_LOG( clog, L"유저가 신고함... Name : " << m_strName )
        << BUILD_LOG( SiKGSSimLayer()->m_nMaxReportNum )
        << BUILD_LOG( m_nReportCount )
        << BUILD_LOG( kPacket.m_nCount )
        << BUILD_LOG( kPacket_.m_nReason )
        << BUILD_LOG( kPacket_.m_vecUser.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_REPORT_USER_ACK, KEVENT_DB_REPORT_USER_ACK )
{
    // DB 에서 받아온 값을 셋팅한다.
    KEVENT_REPORT_USER_ACK kPacket;
    kPacket.m_nOK           = kPacket_.m_nOK;
    kPacket.m_dwSerialNum   = kPacket_.m_dwSerialNum;
    kPacket.m_nCount        = SiKGSSimLayer()->m_nMaxReportNum - m_nReportCount; //남은 신고 횟수 = 최대 신고 횟수 - 누적된 신고 횟수

    NetError::SetLastNetError( kPacket_.m_nOK );
    // 틀린 오류 값이면 오류 처리 한다.
    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        goto END_PROC;
    }

    // 신고 카운트를 하나 올린다.
    ++m_nReportCount;
    kPacket.m_nCount = SiKGSSimLayer()->m_nMaxReportNum - m_nReportCount; // 누적된 신고 횟수

    // 방원 에게 브로드 캐스팅 한다.
    if( m_pkRoom )
    {
        kPacket_.m_kReqPacket.m_strName = m_strNickName;
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_REPORT_USER_NOT, kPacket_.m_kReqPacket );
    }

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_REPORT_00, kPacket.m_nOK, 1 );   //이미 신고 횟수 초과.
        ERR_CASE( ERR_REPORT_01, kPacket.m_nOK, 2 );   //신고 대상이 존재하지 않음.
        ERR_CASE( ERR_REPORT_02, kPacket.m_nOK, 3 );   //신고 타입이 등록된 타입이 아님.
        ERR_CASE( ERR_REPORT_03, kPacket.m_nOK, 4 );   //신고한 유저 정보 없음 
        ERR_CASE( ERR_REPORT_04, kPacket.m_nOK, 5 );   //해당 신고 타입이 없음
        ERR_CASE( ERR_REPORT_05, kPacket.m_nOK, 6 );   //신고당한 유저 정보 없음
    default:
        START_LOG( cerr, L"알수 없는 오류 " )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    };

    // 본인에게 시리얼 넘버를 보낸다.
    SEND_PACKET( EVENT_REPORT_USER_ACK );

    START_LOG( clog, L"유저가 신고함... Name : " << m_strName )
        << BUILD_LOG( SiKGSSimLayer()->m_nMaxReportNum )
        << BUILD_LOG( m_nReportCount )
        << BUILD_LOG( kPacket.m_nCount ) // 남은 횟수
        << BUILD_LOG( kPacket_.m_kReqPacket.m_nReason )
        << BUILD_LOG( kPacket_.m_kReqPacket.m_strUser ) << END_LOG; // 신고당한 사람
}

_IMPL_ON_FUNC( EVENT_CHATTING_EVENT_CORRECT_NOT, int )
{
    _VERIFY_STATE((2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING ));

    if( GetStateID() == KUserFSM::STATE_PLAYING )
    {
        START_LOG( cwarn, L"이미 게임이 시작되어서 무시됨.. Name : " << m_strName ) << END_LOG;
        return;
    }
    JIF( kPacket_ > 0 );
    JIF( m_pkChannel );
    JIF( m_pkRoom );

    if( m_pkRoom->IsChatEventReward() )
    {
        START_LOG( clog, L" 이 방은 이미 채팅 이벤트 혜택이 결정된 방이다." ) << END_LOG;
        return;
    }

    if( !m_pkRoom->IsMaximumUser() )
    {
        START_LOG( clog, L"아직 방에 사람 수가 부족하다." ) << END_LOG;
        return;
    }

    if( !m_pkRoom->IsNormalUserAllReady() )
    {
        START_LOG( clog, L"모든 유저가 레디 상태가 아니다.. 안습.. Name : " << m_strName ) << END_LOG;
        return;
    }

    if( ::GetTickCount() - m_pkRoom->GetAllReadyTick() > SiKGSSimLayer()->GetChatEventAllowedTime() )
    {
        START_LOG( clog, L"모든 방유저 레디 후 30초 지났음.. Name : " << m_strName ) << END_LOG;
        m_pkRoom->ClearChatEvent();

        return;
    }

    m_pkRoom->RegChatEventCorrect( m_strName, kPacket_ );

    if( m_pkRoom->IsChatEventReward() )
    {
        bool kPacket = m_pkRoom->IsChatEventReward();
        LIF( m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_CHAT_EVENT_ENABLE_NOT, kPacket ) );
    }

}
_IMPL_ON_FUNC( EVENT_ENCHANT_REQ, KEVENT_ENCHANT_REQ )
{
    _VERIFY_STATE((2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY));

    KEVENT_ENCHANT_ACK kPacket;
    KDB_EVENT_ENCHANT_REQ kDBPacket;
    KItemPtr pkItemInfo;
    KItemPtr pkStuff;
    UINT uiCoreCount = 999999;
    KRainBowData kRainbow;
    bool bRainbow = SiKRainbow()->GetEventByID( KRainbow::RE_LUNE_STRONG, kRainbow );
    float fAdditionalRatio = 0.f;
    float fGuildBonus = 0.f;
    float fFailRatio = 0.f;
    float fAddRatio = 0.f;
    int nLimitLevel = 0;

    SET_ERROR( ERR_UNKNOWN );

    JIF( m_pkChannel );

    // 요청한 아이템이 룬이냐>
    if( !SiKEnchantStrong()->IsEnchantEnableItem( kPacket_.m_ItemID ) )
    {
        START_LOG( cerr, L"강화 가능한 아이템이 아니다.. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_ItemID )
            << BUILD_LOG( kPacket_.m_ItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_ENCHANT_00, END_PROC );
    }

    // 요청한 아이템이 비쥬이냐?
    if ( kPacket_.m_EnchantStuffID != SiKEnchantStrong()->GetCoreItemID() )
    {
        START_LOG( cerr, L"인챈트 재료아이템 요청 ID 다름 : " << kPacket_.m_EnchantStuffID ) << END_LOG;
        SET_ERR_GOTO( ERR_ENCHANT_02, END_PROC );
    }

    // 요청한 아이템이 강화 보조 아니템이 맞냐?
    if( kPacket_.m_AssistItemID != 0xFFFFFFFF &&
        !SiKEnchantStrong()->IsEnchantAssistItem( kPacket_.m_AssistItemID ) )
    {
        SET_ERR_GOTO( ERR_ENCHANT_01, END_PROC );
    }

    // 룬을 보유 하고 있나?
    pkItemInfo = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if ( pkItemInfo == NULL ) 
    {
        SET_ERR_GOTO( ERR_ENCHANT_00, END_PROC );
    }

    // 강화 보조 아이템 보유 하고 있나?
    if ( kPacket_.m_AssistItemID != 0xFFFFFFFF )
    {
        if ( GetDurItemCount( kPacket_.m_AssistItemID ) <= 0 ) 
        {
            SET_ERR_GOTO( ERR_ENCHANT_01, END_PROC );
        }
    }

    // 특정 강화 업그레이드 아이템에 대한 레벨 체크.
    if( SiKEnchantStrong()->CheckSpecificLevelItem( kPacket_.m_AssistItemID ) ) {
        if( !SiKEnchantStrong()->CheckEnchantLevel( kPacket_.m_AssistItemID, pkItemInfo->m_cEnchantLevel ) ) {
            SET_ERR_GOTO( ERR_ENCHANT_05, END_PROC );
        }
    }

    // 비쥬를 보유 하고 있나?
    pkStuff = m_kInventory.FindItemByItemID( kPacket_.m_EnchantStuffID );
    if( pkStuff == NULL )
    {
        START_LOG( cerr, L"인챈트 재료아이템 없음. : " << kPacket_.m_EnchantStuffID ) << END_LOG;
        SET_ERR_GOTO( ERR_ENCHANT_02, END_PROC );
    }

    // 필요한 비쥬 개수 가져옴.
    if( bRainbow )
    {
        uiCoreCount = SiKEnchantStrong()->GetNeedCoreCount( kPacket_.m_ItemID, (UINT)pkItemInfo->m_cEnchantLevel, kRainbow.m_fSecondaryRatio );
        fAdditionalRatio += kRainbow.m_fPrimaryRatio;
    }
    else
    {
        uiCoreCount = SiKEnchantStrong()->GetNeedCoreCount( kPacket_.m_ItemID, (UINT)pkItemInfo->m_cEnchantLevel, 1.f );
    }

    if( IsPCBangUser() )
    {
        fAdditionalRatio += SiKIPAdvantage()->GetLuneStrongBonus( KIPAdvantage::PCBANG_PUBLISHER );
    }

    // 길드 보너스
    if ( m_kGuildUserInfo.m_dwGuildUID > 0 ) {
        fGuildBonus = GetGuildBonus( KGuildConfig::GA_RUNE_ENCHANT_BONUS );
        fAdditionalRatio += fGuildBonus;
    }

    if( IsTH3BBUser() ) { // 태국 3BB 유저인지에 대한 체크.
        fAdditionalRatio += SiKIPAdvantage()->GetLuneStrongBonus( KIPAdvantage::TH3BB_USER );
    }

    //// 요청한 비쥬 사용 수량이 올바르냐?
    //if ( uiCoreCount != kPacket_.m_uiEnchantStuffCount )
    //{
    //    SET_ERR_GOTO( ERR_ENCHANT_04, END_PROC );
    //}
    // 요청한 수량 만큼 비쥬를 보유 하고 있나?
    if ( pkStuff->m_nCount < (int)uiCoreCount )
    {
        START_LOG( cerr, L"강화 재료 아이템 갯수가 모자람" ) 
            << L"   강화 아이템 ID : " << kPacket_.m_ItemID << dbg::endl
            << L"   강화 아이템 강화레벨 : " << (UINT)pkItemInfo->m_cEnchantLevel << dbg::endl
            << L"   필요 강화 재료아이템 갯수 : " << uiCoreCount << dbg::endl
            << L"   가지고 있는 재료 아이템 갯수 : " << GetDurItemCount( SiKEnchantStrong()->GetCoreItemID() ) << END_LOG;
        SET_ERR_GOTO( ERR_ENCHANT_03, END_PROC );
    }

    //====================================================================================
    // 2007-11-02 오후 1:54:26 : Asirion 
    // 오류 점검 끝! 강화 시작
    int nLevel = 0;
    KEnchantStrong::ENCHANT_ACTION eAction = SiKEnchantStrong()->Enchant( kPacket_.m_ItemID, pkItemInfo->m_cEnchantLevel, kPacket_.m_AssistItemID, fAdditionalRatio, false, fFailRatio, fAddRatio, nLevel );
    kDBPacket = kPacket_;
    kDBPacket.m_bBreak = (eAction == KEnchantStrong::EA_BREAK);
    kDBPacket.m_uiEnchantStuffCount = uiCoreCount;

    nLimitLevel = static_cast<int>( SiKEnchantStrong()->GetStrongLimitLevel() );

    if( pkItemInfo->m_cEnchantLevel > nLimitLevel && eAction == KEnchantStrong::EA_SUCCESS ) {
        eAction = KEnchantStrong::EA_WEAKNESS;
        START_LOG( cerr, L"강화 버그 사용 유저. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_dwLoginUID )
            << BUILD_LOG( kPacket_.m_ItemID )
            << BUILD_LOG( fFailRatio )
            << BUILD_LOG( fAddRatio ) << END_LOG;
    }
    kDBPacket.m_eEnchantAction = (int)eAction;

    switch( eAction )
    {
    case KEnchantStrong::EA_DROP:
    case KEnchantStrong::EA_BREAK:
        {
            kDBPacket.m_iEnchantLevel = 0;
            break;
        }
    case KEnchantStrong::EA_SUCCESS:
        {
            kDBPacket.m_iEnchantLevel = pkItemInfo->m_cEnchantLevel + 1;
            break;
        }
    case KEnchantStrong::EA_WEAKNESS:
        {
            kDBPacket.m_iEnchantLevel = pkItemInfo->m_cEnchantLevel - 1;
            break;
        }
    case KEnchantStrong::EA_SPECIFIC:
        {
            kDBPacket.m_iEnchantLevel = nLevel;
            break;
        }
    default: 
        {
            START_LOG( cerr, L"강화 오류 : SiKEnchantStrong()->Enchant에서 예기치 못한 결과 발생" ) << END_LOG;
            goto END_PROC;
        }
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ENCHANT_REQ, kDBPacket );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ENCHANT_00, kPacket.m_nOK, 4 );   //강화 아이템 없음( ID )
        ERR_CASE( ERR_ENCHANT_01, kPacket.m_nOK, 5 );   //강화 보조 아이템 없음( ID )
        ERR_CASE( ERR_ENCHANT_02, kPacket.m_nOK, 6 );   //강화 재료 아이템 없음( ID )
        ERR_CASE( ERR_ENCHANT_03, kPacket.m_nOK, 7 );   //강화 재료 아이템 갯수부족
        ERR_CASE( ERR_ENCHANT_04, kPacket.m_nOK, 8 );   //C/S갯수 다름
        ERR_CASE( ERR_ENCHANT_05, kPacket.m_nOK, 9 );   //특정 레벨 강화 주문서 레벨이 현재 강화 레벨보다 같거나 낮음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"강화 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    };
    //====================================================================================
    // 2007-10-23 오전 2:09:20 : Asirion 
    // 오류 패킷 보낸다.
    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EVENT_ENCHANT_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || kPacket.m_nOK == -98 )
        << NetError::GetLastNetErrMsg() << L", Name : " << m_strName << dbg::endl
        << BUILD_LOG( kDBPacket.m_eEnchantAction )
        << BUILD_LOG( kPacket_.m_dwLoginUID )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_AssistItemID )
        << BUILD_LOG( kPacket_.m_AssistItemUID )
        << BUILD_LOG( kPacket_.m_EnchantStuffID )
        << BUILD_LOG( kPacket_.m_EnchantStuffUID )
        << BUILD_LOG( kPacket_.m_uiEnchantStuffCount )
        << BUILD_LOG( kDBPacket.m_dwLoginUID )
        << BUILD_LOG( kDBPacket.m_ItemUID )
        << BUILD_LOG( kDBPacket.m_ItemID )
        << BUILD_LOG( kDBPacket.m_AssistItemID )
        << BUILD_LOG( kDBPacket.m_AssistItemUID )
        << BUILD_LOG( kDBPacket.m_EnchantStuffID )
        << BUILD_LOG( kDBPacket.m_EnchantStuffUID )
        << BUILD_LOG( kDBPacket.m_uiEnchantStuffCount )
        << BUILD_LOG( kRainbow.m_fSecondaryRatio )
        << BUILD_LOG( kRainbow.m_fPrimaryRatio )
        << BUILD_LOG( IsPCBangUser() )
        << BUILD_LOG( fGuildBonus ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_ENCHANT_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    KItemPtr pkItemInfo;
    kPacket_.m_vecItem.clear();

    // 정상 적인 강화 결과라면..
    if ( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1 || kPacket_.m_nOK == 2 || kPacket_.m_nOK == 3 || kPacket_.m_nOK == 4 ) {
        // 사용 한 비쥬
        KItemPtr pkStuffItem = m_kInventory.FindItemByItemUID( kPacket_.m_StuffItemUID );
        if( pkStuffItem ) {
            pkStuffItem->m_nInitCount    -= kPacket_.m_dwReduceStuffItemCount;
            pkStuffItem->m_nCount        -= kPacket_.m_dwReduceStuffItemCount;
            kPacket_.m_vecItem.push_back( *pkStuffItem );

            if ( pkStuffItem->m_nCount != kPacket_.m_iRemainCoreCount ) {
                START_LOG( clog, L"강화 재료 아이템 남은 갯수 다름" )
                    << BUILD_LOG( L"SERVER : " << pkStuffItem->m_nCount )
                    << BUILD_LOG( L"DB : " << kPacket_.m_iRemainCoreCount ) << END_LOG;
            }
        }

        if ( kPacket_.m_EnchantAssistItemID != 0xFFFFFFFF ) {
            KItemPtr pkAssistItem = m_kInventory.FindItemByItemUID( kPacket_.m_EnchantAssistItemUID );
            if( pkAssistItem ) {
                pkAssistItem->m_nCount       -= 1;
                pkAssistItem->m_nInitCount   -= 1;
                kPacket_.m_vecItem.push_back( *pkAssistItem );
            }
        }

        pkItemInfo = m_kInventory.FindItemByItemUID( kPacket_.m_EnchantItemUID );
        if( pkItemInfo ) {
            switch( kPacket_.m_nOK )
            {
            case 0: pkItemInfo->m_cEnchantLevel++; break;
            case 1: pkItemInfo->m_cEnchantLevel--; break;
            case 2: pkItemInfo->m_cEnchantLevel = 0; break;
            case 3: m_kInventory.RemoveItem( kPacket_.m_EnchantItemUID ); break;
            case 4: pkItemInfo->m_cEnchantLevel = kPacket_.m_nEnchantLevel; break;
            }
            kPacket_.m_vecItem.push_back( *pkItemInfo );
        }
        else {
            START_LOG( cerr, L"강화 오류 : DB에 요청했던 강화아이템이 사라짐" ) << END_LOG;
            kPacket_.m_nOK = 12;
        }
    }
/*
OK 설명
0 : 성공
-1 : 유저가 존재하지 않음
-2 : 재료 아이템이 존재하지 않거나 있더라도 차감 개수보다 작음
-3 : 보조 아이템이 존재하지 않음
-4 : 강화될 수 있는 아이템이 아님(강화 아이템 리스트에 아이템이 등록되어 있지 않음)
-5 : 강화 아이템이 존재하지 않음
-101이하 : 트랜잭션 에러
*/
    switch( kPacket_.m_nOK )
    {
        case 0: 
        case 1: 
        case 2: 
        case 3:  
        case 4:  break; // 여기까지는 위에서 결정.. ㅡㅡ...
        case -1: kPacket_.m_nOK = 10; break;
        case -2: kPacket_.m_nOK = 7; break;
        case -3: kPacket_.m_nOK = 5; break;
        case -4: kPacket_.m_nOK = 4; break;
        case -5: kPacket_.m_nOK = 4; break;
        default: kPacket_.m_nOK = 9; break;
    }

    SEND_RECEIVED_PACKET( EVENT_ENCHANT_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1 || kPacket_.m_nOK == 2 || kPacket_.m_nOK == 3 || kPacket_.m_nOK == 4,
        L"룬 강화 결과 Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_EnchantAssistItemID )
        << BUILD_LOG( kPacket_.m_dwReduceStuffItemCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_SERVER_LIST_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_SERVER_LIST_ACK  kPacket;
    if( ::GetTickCount() - GetTick( SEND_SERVER_LIST ) < KBaseServer::ms_dwSendServerListGap )
    {
        SET_ERR_GOTO( ERR_LIST_00, END_PROC );
    }

    SiKGameServer()->GetServerList( kPacket.m_vecServerList );
    CheckServerListByAuthLevel( kPacket.m_vecServerList );
    SetTick( SEND_SERVER_LIST );
    DumpServerList( kPacket.m_vecServerList );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_LIST_03, kPacket.m_nOK, 1 );
    default:
        START_LOG( cerr, L"서버 리스트 목록 요청중 알수 없는 오류. Name : " << m_strName )
            << L" 오류 메세지 : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_SERVER_LIST_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L" 서버 리스트 목록 요청 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( GetAuthLevel() )
        << BUILD_LOG( kPacket.m_vecServerList.size() ) << END_LOG;
}

IMPL_ON_FUNC( DB_EVENT_BONUS_POINT_REFILL_ACK )
{
    START_LOG( clog, L"리필 받은 량.. Name : " << m_strName )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwSlotNum )
        << BUILD_LOG( kPacket_.m_nDiffBonusCount )
        << END_LOG;

    if ( kPacket_.m_nDiffBonusCount <= 0 ) {
        return;
    }

    m_mapBaseBonus[ kPacket_.m_cCharType ].IncreaseAllValue( kPacket_.m_nDiffBonusCount );

    KEVENT_BONUS_POINT_REFILL_NOT kPacket;
    kPacket.m_cCharType = kPacket_.m_cCharType;
    GetBonusInfo( kPacket.m_cCharType, kPacket.m_kBonusPointInfo );

    SEND_PACKET( EVENT_BONUS_POINT_REFILL_NOT );
}

IMPL_ON_FUNC( EVENT_CHANGE_BONUS_COIN_REQ )
{
    _VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ) );

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_CHANGE_BONUS_COIN_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cCharType = GetCurrentCharType();
    kPacket.m_nSpecialBonusDiff = 0;
    KCharacterInfo* pkCurrentChar = NULL;
    KItemPtr pkItem;

    // 요청한 아이템이 보너스 아이템인지 확인
    if ( false == SiKGCHelper()->IsBonusItem( kPacket_.m_kItem.m_dwID ) ) {
        SET_ERR_GOTO( ERR_BONUS_02, END_PROC );
    }

    // 현재 플레이하고 있는 캐릭터 정보가 있는지 확인
    pkCurrentChar = GetCurrentCharacter();
    if ( NULL == pkCurrentChar ) {
        SET_ERR_GOTO( ERR_BONUS_01, END_PROC );
    }

    kPacket_.m_cCharType = pkCurrentChar->m_cCharType;
    kPacket_.m_dwSlotNum = pkCurrentChar->m_dwSlotNum;

    // 보너스 아이템을 보유하고 있는지 확인
    pkItem = m_kInventory.FindDurationItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );
    if ( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_BONUS_03, END_PROC ); // 해당 보너스 아이템을 보유하고 있지 않음
    }

    if ( 0 > kPacket_.m_ucCount ) {
        SET_ERR_GOTO( ERR_BONUS_04, END_PROC );
    }

    // 보너스 아이템의 수량이 충분한지 확인
    if ( pkItem->m_nCount < kPacket_.m_ucCount ) {
        SET_ERR_GOTO( ERR_BONUS_04, END_PROC );
    }

    // (최대 보유량 - 최대 기본 보유량) >= 현재 스페셜 보유량 + 요청 보유량
    if ( SiKGCHelper()->GetMaxBonusCount() - m_mapBaseBonus[ kPacket_.m_cCharType ] < kPacket_.m_ucCount + m_mapSpecialBonus[ kPacket_.m_cCharType ] ) {
        SET_ERR_GOTO( ERR_BONUS_05, END_PROC );
    }

    kPacket_.m_nSpecialBonusDiff = m_mapSpecialBonus[ kPacket_.m_cCharType ].GetValue() - m_mapSpecialBonus[ kPacket_.m_cCharType ].GetInitValue();
    if ( 0 > kPacket_.m_nSpecialBonusDiff + m_mapSpecialBonus[ kPacket_.m_cCharType ].GetInitValue() ) {
        kPacket_.m_nSpecialBonusDiff = 0;
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHANGE_BONUS_COIN_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, NetError::NET_OK );       // 성공
        ERR_CASE( ERR_BONUS_01, kPacket.m_nOK, NetError::ERR_BONUS_01 ); // Best Character가 존재하지 않음_캐릭터 맵이 비어 있음.
        ERR_CASE( ERR_BONUS_02, kPacket.m_nOK, NetError::ERR_BONUS_02 ); // 요청한 아이템은 보너스 코인 아이템이 아님.
        ERR_CASE( ERR_BONUS_03, kPacket.m_nOK, NetError::ERR_BONUS_03 ); // 해당 보너스 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_BONUS_04, kPacket.m_nOK, NetError::ERR_BONUS_04 ); // 보너스 코인 아이템의 수량 부족.
        ERR_CASE( ERR_BONUS_05, kPacket.m_nOK, NetError::ERR_BONUS_05 ); // 더이상 보너스 포인트를 보유할수 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업

    default:
        START_LOG( cerr, L"보너스 코인 교환 중 알 수 없는 오류. Name: " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
            << BUILD_LOG( NetError::GetLastNetError() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    GetBonusInfo( kPacket_.m_cCharType, kPacket.m_kCurrentBonus );

    if ( NetError::NET_OK != kPacket.m_nOK ) {
        SEND_PACKET( EVENT_CHANGE_BONUS_COIN_ACK );
    }

    LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK )
        << L"보너스 코인 교환 결과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_mapBaseBonus[ kPacket_.m_cCharType ] )
        << BUILD_LOG( m_mapSpecialBonus[ kPacket_.m_cCharType ] )
        << BUILD_LOG( GetCurrentBonusCount() )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID )
        << BUILD_LOGc( kPacket_.m_kItem.m_cItemType )
        << BUILD_LOGc( kPacket_.m_ucCount )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwSlotNum )
        << BUILD_LOG( kPacket_.m_nSpecialBonusDiff )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kDurItem.m_nCount )
        << BUILD_LOG( kPacket.m_kCurrentBonus.m_nBaseBonus )
        << BUILD_LOG( kPacket.m_kCurrentBonus.m_nSpecialBonus )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHANGE_BONUS_COIN_ACK )
{
    int nChangeCount = kPacket_.m_kDurItem.m_nCount;
    KItemPtr pkItem;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    /* 수량 아이템 인벤터리에서 찾아 수량 깎고 남은 수량이 0개 이하면 인벤터리 컨테이너에서 제거하는
       함수를 만들어 사용하면 좋을 것 같다 */
    pkItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kDurItem.m_ItemUID );
    if ( NULL != pkItem ) {
        pkItem->m_nCount -= kPacket_.m_kDurItem.m_nCount;
        pkItem->m_nInitCount = pkItem->m_nCount;

        kPacket_.m_kDurItem = *pkItem;

        if ( 0 >= pkItem->m_nCount ) {
            m_kInventory.RemoveItem( kPacket_.m_kDurItem.m_ItemUID );
        }
    }

    m_mapSpecialBonus[ kPacket_.m_cCharType ].IncreaseValue( nChangeCount );
    m_mapSpecialBonus[ kPacket_.m_cCharType ].IncreaseInitValue( kPacket_.m_nSpecialBonusDiff );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_BONUS_06, kPacket_.m_nOK, NetError::ERR_BONUS_06 ); // 보너스 충전 아이템 DB 동기화 실패.
        ERR_CASE( ERR_BONUS_07, kPacket_.m_nOK, NetError::ERR_BONUS_07 ); // 보너스 충전 아이템 사용시 포인트 DB 동기화 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업

    default:
        START_LOG( cerr, L"보너스 포인트 교환 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    GetBonusInfo( kPacket_.m_cCharType, kPacket_.m_kCurrentBonus );

    SEND_RECEIVED_PACKET( EVENT_CHANGE_BONUS_COIN_ACK );

    _LOG_SUCCESS( NetError::NET_OK == kPacket_.m_nOK, L"보너스 포인트 교환 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kDurItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kDurItem.m_nCount )
        << BUILD_LOG( kPacket_.m_kCurrentBonus.m_nBaseBonus )
        << BUILD_LOG( kPacket_.m_kCurrentBonus.m_nSpecialBonus )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_USE_BONUS_POINT_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_PLAYING ) );
    KEVENT_USE_BONUS_POINT_ACK kPacket;
    kPacket.m_nOK = 1;

    if ( true == ReduceBonusCount() ) {
        kPacket.m_nOK = 0;
    }

    GetBonusInfo( GetCurrentCharType(), kPacket.m_kCurrentBonus );

    SEND_PACKET( EVENT_USE_BONUS_POINT_ACK );

	START_LOG( clog, L" 보너스 포인트 사용 MSG : " << ( kPacket.m_nOK == 0 ? L"성공": L"실패" ) )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kCurrentBonus.m_nBaseBonus )
        << BUILD_LOG( kPacket.m_kCurrentBonus.m_nSpecialBonus ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SLOT_INFO_NOT, KQuickSlot )
{
    // DB에서 받아 왔다.
    m_mapQuickSlot[m_cCurrentCharacter] = kPacket_;
    SEND_RECEIVED_PACKET( EVENT_SLOT_INFO_NOT );
}

_IMPL_ON_FUNC( EVENT_CHANGE_SLOT_EQUIP_REQ, KQuickSlot )
{
    KEVENT_CHANGE_SLOT_EQUIP_ACK kPacket;
    SET_ERROR( ERR_UNKNOWN );
    if( !CheckValidateSlot( kPacket_.m_vecEmoticonSlot, true ) )
    {
        SET_ERR_GOTO( ERR_QUICKSLOT_00, END_PROC );
    }

    if( !CheckValidateSlot( kPacket_.m_vecItemSlot, false ) )
    {
        SET_ERR_GOTO( ERR_QUICKSLOT_01, END_PROC );
    }

    m_mapQuickSlot[m_cCurrentCharacter] = kPacket_;

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_QUICKSLOT_00, kPacket.m_nOK, 1 ); // 퀵슬롯 장착 변경시 이모티콘 오류. )
        ERR_CASE( ERR_QUICKSLOT_01, kPacket.m_nOK, 2 ); // 퀵슬롯 장착 변경시 사용 아이템 오류. )
    default:
        START_LOG( cerr, L"퀵슬롯 장착 정보 변경시 알수 없는 오류.. Nmae : " << m_strName )
            << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = 99;
        SET_ERROR( ERR_UNKNOWN );
    }

    GetCharQuickSlotInfo( kPacket.m_kQuickSlot );

    SEND_PACKET( EVENT_CHANGE_SLOT_EQUIP_ACK );

    if( kPacket.m_nOK == 0 && GetStateID() == KUserFSM::STATE_ROOM && m_pkRoom != NULL )
    {
        KEVENT_QUICKSLOT_UPDATE_NOT kPacket;
        kPacket.m_strLogin = GetName();
        kPacket.m_kQuickSlot = kPacket_;

        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_QUICKSLOT_UPDATE_NOT, kPacket, true );
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L"퀵 슬롯 장착 변경 : "  << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_vecEmoticonSlot.size() )
        << BUILD_LOG( kPacket_.m_vecItemSlot.size() ) 
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kQuickSlot.m_vecEmoticonSlot.size() )
        << BUILD_LOG( kPacket.m_kQuickSlot.m_vecItemSlot.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_USE_QUICK_SLOT_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_PLAYING ) );
    SET_ERROR( ERR_UNKNOWN );
    JIF( m_pkRoom );
    KEVENT_USE_QUICK_SLOT_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkItem;
    KQuickSlot kQuickSlot;
    GetCharQuickSlotInfo( kQuickSlot );

    std::vector< KSlotData >& vecSlot = ( kPacket_.m_nSlotType == SCS_ITEM ? kQuickSlot.m_vecItemSlot : kQuickSlot.m_vecEmoticonSlot );

    if( !IsInSlot( vecSlot, kPacket_.m_kSlotData ) ) // 내부에서 Set Error 하고있음.
    {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kSlotData.m_kItem.m_dwUID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_QUICKSLOT_04, END_PROC );
    }

    if( pkItem->m_nCount != KItem::UNLIMITED_ITEM && pkItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_QUICKSLOT_06, END_PROC );
    }
    else
    {
        pkItem->m_nCount -= 1;

        if( pkItem->m_nCount <= 0 ) // 수량을 다썼으면 슬롯에서 지운다.
            ReleaseQuickSlot( kPacket_.m_kSlotData.m_kItem.m_dwUID );
    }

    if( !SiKGCHelper()->CheckPositionGroupType( kPacket_.m_nPositionType, kPacket_.m_kSlotData.m_kItem.m_dwID ) ) {
        START_LOG( cerr, L" 서버에서 포션 아이템 설정이 잘못되어 있음 " )
            << BUILD_LOG( kPacket_.m_nPositionType )
            << BUILD_LOG( kPacket_.m_kSlotData.m_kItem.m_dwID )
            << END_LOG;
    }

    // 룸 객체에 해당유저의 포션 사용 정보 세팅.
    m_pkRoom->SetUserUsePosition( GetUID(), kPacket_.m_nPositionType );

    // 슬롯 정보를 준다..
    kPacket.m_nSlotType = kPacket_.m_nSlotType;
    kPacket.m_kSlotData = kPacket_.m_kSlotData;
    kPacket.m_nPositionType = kPacket_.m_nPositionType;
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_QUICKSLOT_02, kPacket.m_nOK, 1 ); // 사용할려는 슬롯이 슬롯에 등록되어 있지 않음.
        ERR_CASE( ERR_QUICKSLOT_03, kPacket.m_nOK, 2 ); // 사용할려는 슬롯 아이템 정보와, 등록된 아이템 정보가 같지 않음.
        ERR_CASE( ERR_QUICKSLOT_04, kPacket.m_nOK, 3 ); // 사용할려는 슬롯 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_QUICKSLOT_05, kPacket.m_nOK, 4 ); // 사용할려는 슬롯 아이템타입이 옳지 않음.
        ERR_CASE( ERR_QUICKSLOT_06, kPacket.m_nOK, 5 ); // 사용할려는 슬롯 아이템 수량이 부족함
    }

    SEND_PACKET( EVENT_USE_QUICK_SLOT_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L" 퀵 슬롯 사용시 Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nSlotType )
        << BUILD_LOG( kPacket_.m_kSlotData.m_nSlotIndex )
        << BUILD_LOGc( kPacket_.m_kSlotData.m_kItem.m_cItemType )
        << BUILD_LOG( kPacket_.m_kSlotData.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kSlotData.m_kItem.m_dwUID ) << END_LOG;
}


_IMPL_ON_FUNC( EVENT_SQUARE_LIST_REQ, DWORD )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_ROOM ));

    SET_ERROR( ERR_UNKNOWN );

    const DWORD& dwGuildUID = kPacket_;

    PAIR_DWORD kPacket; // pair[useruid, guilduid]
    kPacket.first = GetUID();
    kPacket.second = dwGuildUID;

    // 길드정보 체크
    if ( dwGuildUID != 0 ) {
        if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
            START_LOG( cwarn, L"길드원인데 길드 정보가 없다. GuildUID : " << dwGuildUID ) << END_LOG;
            goto END_PROC;
        }

        if ( dwGuildUID != m_kGuildUserInfo.m_dwGuildUID ) {
            START_LOG( cwarn, L"클라이언트와 서버 길드 정보가 다르다. client GuildUID : " << dwGuildUID << L", server GuildUID : " << m_kGuildUserInfo.m_dwGuildUID ) << END_LOG;
            goto END_PROC;
        }

        if ( SiKGuildManager()->GetGuild( m_kGuildUserInfo.m_dwGuildUID ) == NULL ) {
            START_LOG( cwarn, L"길드UID가 있지만 서버에 길드정보가 없음. GuildUID : " << m_kGuildUserInfo.m_dwGuildUID ) << END_LOG;
            goto END_PROC;
        }
    }

    // 센터서버에 리스트 요청
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_SQUARE_LIST_REQ, kPacket );
    SET_ERROR( NET_OK );

END_PROC:
    // 서버에 길드정보가 없으면 DB에 요청
    if ( !IS_CORRECT( NET_OK ) ) {
        QUEUEING_ID_TO_DB( EVENT_SQUARE_LIST_REQ );
    }

    START_LOG( clog, L"광장 리스트 요청. Login : " << GetName() )
        << BUILD_LOG( dwGuildUID )
        << BUILD_LOG( m_kGuildUserInfo.m_dwGuildUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SQUARE_LIST_ACK, KNGuildUserInfo )
{
    // 길드정보 설정
    SetGuildInfo( kPacket_ );

    // 센터서버에 리스트 요청
    PAIR_DWORD kPacket; // pair[useruid, guilduid]
    kPacket.first = GetUID();
    kPacket.second = m_kGuildUserInfo.m_dwGuildUID;
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_SQUARE_LIST_REQ, kPacket );
}

IMPL_ON_FUNC( EVENT_ENTER_SQUARE_REQ )      // #EVENT_ENTER_SQUARE_REQ
{
    _VERIFY_STATE(( 1, KUserFSM::STATE_CHANNELLOBBY ));

    KSquareUserInfo kUserData;
    KEVENT_ENTER_SQUARE_ACK kPacket;
    kPacket.m_nOK = -99;
    JIF( !m_spSquare ); // 광장 객체가 있으면 안됨. 유저의 광장 스마트포인터
    SET_ERROR( ERR_UNKNOWN );

    // 유저의 광장 입장 시간 세팅.
    SetEnterSquareTime();
    do 
    {
        if ( m_strNickName.empty() ) {
            SET_ERROR( ERR_SQUARE_06 );
            break;
        }

        if ( !IsCharExist( kPacket_.m_cSelCharType ) ) {
            SET_ERROR( ERR_SQUARE_00 );
            break;
        }

        if ( m_vecMiniGameInfo.empty() ) {
            QUEUEING_ID_TO_DB( EVENT_MINIGAME_INFO_NOT );
        }

        kUserData.m_dwSquareID = kPacket_.m_dwSquareID;
        if ( GetSquareUserInfo( kUserData ) == false ) {
            SET_ERROR( ERR_SQUARE_00 );
            break;
        }

        
        switch( kPacket_.m_iSquareType ) {
            case KSquareInfo::ST_GUILD: {
                KECN_ENTER_GUILD_SQUARE_REQ kPacket;
                kPacket.m_iGuildSquareIndex = kPacket_.m_iSquareIndex;
                kPacket.m_kSquareUserInfo = kUserData;
                if ( SiKCnConnector()->SendPacket( KCenterEvent::ECN_ENTER_GUILD_SQUARE_REQ, kPacket ) == false ) {
                    // send ack packet with error value
                }

                START_LOG( clog, L"유저가 클릭한 길드 광장의 인덱스 : " << kPacket_.m_iSquareIndex )
                    << BUILD_LOG( GetUID() )
                    << BUILD_LOG( kPacket.m_kSquareUserInfo.m_dwGuildUID ) << END_LOG;
            }

            break;

            case KSquareInfo::ST_NORMAL:            
            default:
                SiKCnConnector()->SendPacket( KCenterEvent::ECN_ENTER_SQUARE_REQ, kUserData );
                break;
        }

        return;
    } while (false);

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SQUARE_00, kPacket.m_nOK, 1 );  // 광장 진입 예약시 없는 캐릭터 선택 했음. )
        ERR_CASE( ERR_SQUARE_01, kPacket.m_nOK, 2 );  // 요청한 광장을 찾을수 없음. )
        ERR_CASE( ERR_SQUARE_02, kPacket.m_nOK, 3 );  // 입장 하려는 광장에 사람이  찼습니다. )
        ERR_CASE( ERR_SQUARE_03, kPacket.m_nOK, 4 );  // 입장하려는 광장에 이미 들어 있음. 게임종료 유도 )
        ERR_CASE( ERR_SQUARE_06, kPacket.m_nOK, 5 );  // 아직 닉네임을 만들지 않았음

    default:
        START_LOG( cerr, L"알수 없는 오류  Name : " << m_strName << L", Msg : "<< NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_ENTER_SQUARE_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == 2 || kPacket.m_nOK == 3, clog, cerr )
        << L"MSG : " << NetError::GetLastNetErrMsg()
        << L", Name : " << m_strName << dbg::endl
        << BUILD_LOGc( kPacket_.m_cSelCharType )
        << BUILD_LOG( kPacket_.m_dwSquareID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecUserInfo.size() )
        << BUILD_LOG( kPacket.m_dwRelayServerIP)
        << BUILD_LOG( kPacket.m_usRelayServerPort ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_ENTER_SQUARE_ACK )
{    
    START_LOG( clog, L"광장 입장 응답" )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwRelayServerIP )
        << BUILD_LOG( kPacket_.m_usRelayServerPort )
        << BUILD_LOG( kPacket_.m_vecUserInfo.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ENTER_SQUARE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_SQUARE_LOADING_COMPLETE_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE_JOIN ) );
    KEVENT_SQUARE_LOADING_COMPLETE_ACK  kPacket;
    // 상태 변경
    StateTransition( KUserFSM::INPUT_ENTER_SQUARE );
    //그동안 못받았던출/입 패킷을 준비해서 전송한다..
    kPacket.m_vecLeaveUsers.swap( m_vecLeaveUser );
    kPacket.m_vecSquareUsers.swap( m_vecEnterUser );

    kPacket.m_nOK = NetError::NET_OK;
    SEND_COMPRESS_PACKET( EVENT_SQUARE_LOADING_COMPLETE_ACK, kPacket );

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
        DumpSquareUserInfo( kPacket.m_vecSquareUsers );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L"Square Loading Complete... Name : " << m_strName << dbg::endl
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecSquareUsers.size() )
        << BUILD_LOG( kPacket.m_vecLeaveUsers.size() )
        << BUILD_LOG( m_vecEnterUser.size() )
        << BUILD_LOG( m_vecLeaveUser.size() ) << END_LOG;

    // 가지고 있던 정보 클리어.
    m_vecEnterUser.clear();
    m_vecLeaveUser.clear();

    // 시간 이벤트 얻어오기
    if( kPacket.m_nOK == 0 )
    {
        std::set<int> setActiveEventIDList;
        SiKTimeEventManager()->GetSimpleTimeEvents( setActiveEventIDList );
        SEND_DATA( EVENT_TIME_EVENT_NOT, setActiveEventIDList );
    }
}

_IMPL_ON_FUNC( EVENT_NEW_SQUARE_USER_NOT, KSquareUserInfo )
{
    // 타이밍 차이로 종종 발생할것 같음..
    VERIFY_STATE_LOG( cwarn, ( 2, KUserFSM::STATE_SQUARE_JOIN, KUserFSM::STATE_SQUARE ) );
    JIF( m_spSquare );

    // 광장 로딩 중이라면.. 킵 하고 있자.. 
    if( GetStateID() == KUserFSM::STATE_SQUARE_JOIN )
    {
        START_LOG( clog, L"노딩중 1명 추가요... Id : " << kPacket_.m_dwUID ) << END_LOG;
        AddSquareNewUser( kPacket_ );
        return;
    }

    SEND_RECEIVED_PACKET( EVENT_NEW_SQUARE_USER_NOT );

    START_LOG( clog, L" 누군가가 광장에 들어 옴.. 누구? Name : " << kPacket_.m_dwUID )
        << BUILD_LOG( m_spSquare->GetName() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_LEAVE_SQUARE_USER_NOT, DWORD )
{
    // 타이밍 차이로 종종 발생할것 같음..
    VERIFY_STATE_LOG( cwarn, ( 2, KUserFSM::STATE_SQUARE_JOIN, KUserFSM::STATE_SQUARE ) );
    JIF( m_spSquare );

    // 광장 로딩 중이라면.. 킵 하고 있자.. 
    if( GetStateID() == KUserFSM::STATE_SQUARE_JOIN )
    {
        AddSquareLeaveUser( kPacket_ );
        return;
    }

    // 떠나는 것은 무조건 알려 주자.
    SEND_RECEIVED_PACKET( EVENT_LEAVE_SQUARE_USER_NOT );
    START_LOG( clog, L" 누군가가 광장에서 떠났군... 누구? UID : " << kPacket_ )
        << BUILD_LOG( m_spSquare->GetName() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_LEAVE_SQUARE_REQ )
{
    //if( GetStateID() == KUserFSM::STATE_MINIGAME_PLAYING ) // 이런 예외를 허용해도 되는지 잘 모르겠다..
    //{
    //    std::cout << L"STATE_MINIGAME_PLAYING이면 EVENT_LEAVE_SQUARE_REQ 무시...-_-?" << dbg::endl;
    //    return;
    //}
    //if( !m_pkCurrentState->CheckValidState ( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING ) )

    VERIFY_STATE_LOG( cwarn, ( 2, KUserFSM::STATE_SQUARE_JOIN, KUserFSM::STATE_SQUARE ) );

    LeaveSquare();
    int kPacket = 0;
    DWORD dwLeaveSquareTime = GetLeaveSquareTime();
    dwLeaveSquareTime = dwLeaveSquareTime / 1000;
    QUEUEING_EVENT_TO_DB( DB_EVENT_SQUARE_STAT_TIME, dwLeaveSquareTime );

    SEND_PACKET( EVENT_LEAVE_SQUARE_ACK );
}

_IMPL_ON_FUNC( EVENT_UPDATE_MY_SQUARE_POS, KSquarePos )
{
    VERIFY_STATE_LOG( cwarn, ( 1, KUserFSM::STATE_SQUARE ) );
    JIF( m_spSquare );

    kPacket_.m_dwUID = GetUID();
    SetSquarePos( kPacket_.m_fXpos, kPacket_.m_fYpos );
    kPacket_.m_dwSquareID = m_spSquare->GetUID();
    // 로그 너무 많이 남음. 주석처리
    //START_LOG( clog, L"광장 위치정보 갱신. UserUID : " << kPacket_.m_dwUID << L",SquareID : " << kPacket_.m_dwSquareID << L",Pos(x,y) : " << kPacket_.m_fXpos << L"," << kPacket_.m_fYpos ) << END_LOG;

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_USER_SQUARE_POS_NOT, kPacket_ );
// 
//     std::vector<DWORD> kPacket;
// 
//     int nOldIndex = GetSquareIndex( m_kSquarePos ); // int형변환 된 x_pos 를 넘겨줌
//     int nNewIndex = GetSquareIndex( kPacket_ );
// 
//     if( nOldIndex != nNewIndex )
//     {
//         m_spSquare->UpdateUserPos( nOldIndex, nNewIndex, GetUID() );
//     }
//     
//     m_kSquarePos = kPacket_;
//     m_spSquare->GetNeighborhoodUser( nNewIndex, kPacket ); // 이웃 유저의 uid를 모아서 보내주는군
// 
//     if( !kPacket.empty() )
//         SEND_PACKET( EVENT_UPDATE_SQUARE_USER_NOT );

    //START_LOG( clog, L"광장 내에서 내 좌표 업데이트, Name : " << m_strName )
    //    << BUILD_LOG( nOldIndex )
    //    << BUILD_LOG( m_kSquarePos.m_fXpos )
    //    << BUILD_LOG( m_kSquarePos.m_fYpos )
    //    << BUILD_LOG( nNewIndex )
    //    << BUILD_LOG( kPacket_.m_fXpos )
    //    << BUILD_LOG( kPacket_.m_fYpos )
    //    << BUILD_LOG( kPacket.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SQUARE_CHAT_REQ, KChatData )
{
    VERIFY_STATE_LOG( cwarn, ( 1, KUserFSM::STATE_SQUARE ) );
    JIF( m_spSquare );

    int kPacket = 99;
    KSquareInfo kInfo;
    KSerBuffer kEventBuff;

    SET_ERROR( ERR_UNKNOWN );

    if( !CheckChatEnable() ) { // 채팅 블럭 상태.
        SET_ERR_GOTO( ERR_CHAT_13, END_PROC );
    }

    kPacket_.m_dwSenderUID = GetUID();
    kPacket_.m_strSenderNick = m_strNickName;
    kPacket_.m_strSenderNickColor = m_strNickColor;

    switch( kPacket_.m_cChatType )
    {
    //    case KChatData::MSG_PARTY:  // 미 구현..
    case KChatData::MSG_LOUD:
        {
            if( ::GetTickCount() - GetTick(CHAT_TICK) < 3000 )
                return;
            //m_spSquare->SendToAll( KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_ );
            SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_, kEventBuff );
            SiKCnConnector()->SendSquareRelayPacket( KUserEvent::EVENT_SQUARE_CHAT_NOT, kEventBuff, GetUID(), m_spSquare->GetUID(), false );
        }
        break;
    case KChatData::MSG_COMMON:
        {
            if( ::GetTickCount() - GetTick(CHAT_TICK) < 1000 )
                return;
            SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_, kEventBuff );
            SiKCnConnector()->SendSquareRelayPacket( KUserEvent::EVENT_SQUARE_CHAT_NOT, kEventBuff, GetUID(), m_spSquare->GetUID(), true );

            //m_spSquare->SendToNeighborhood( GetSquareIndex(), KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_ );
        }
        break;
    case KChatData::MSG_NOTIFY:
        {
            if( !IsAdmin() && kPacket_.m_cChatType == KChatData::MSG_NOTIFY )
            {
                SET_ERR_GOTO( ERR_SQUARE_07, END_PROC );
            }

            //m_spSquare->SendToAll( KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_ );
            SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_, kEventBuff );
            SiKCnConnector()->SendSquareRelayPacket( KUserEvent::EVENT_SQUARE_CHAT_NOT, kEventBuff, GetUID(), m_spSquare->GetUID(), false );
        }
        break;
    //case KChatData::MSG_WHISPER:
    //case KChatData::MSG_WHISPER_EX:
    //    {
    //        if( !IsAdmin() && kPacket_.m_cChatType == KChatData::MSG_WHISPER_EX )
    //        {
    //            SET_ERR_GOTO( ERR_SQUARE_07, END_PROC );
    //        }

    //        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwReceiverUID );
    //        if( !spUser )
    //        {
    //            SET_ERR_GOTO( ERR_SQUARE_08, END_PROC );
    //        }

    //        if( !m_spSquare->IsInSquare( spUser->GetUID() ) )
    //        {
    //            SET_ERR_GOTO( ERR_SQUARE_04, END_PROC );
    //        }
    //        spUser->SendPacket( EVENT_TYPE::EVENT_SQUARE_CHAT_NOT, kPacket_ );
    //    }
    //    break;
    case KChatData::MSG_COLOR_CHAT:
        {
            if( ::GetTickCount() - GetTick(CHAT_TICK) < 1000 )
                return;
            if( !IsAdmin() && ( IsPremiumUser() == false && m_cPCBangType == KIPAdvantage::PCBANG_NONE ) )
            {
                SET_ERR_GOTO( ERR_SQUARE_09, END_PROC );
            }

            SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_, kEventBuff );
            SiKCnConnector()->SendSquareRelayPacket( KUserEvent::EVENT_SQUARE_CHAT_NOT, kEventBuff, GetUID(), m_spSquare->GetUID(), true );
        }
        break;
    case KChatData::MSG_ADMIN_COMMON:
        {
            if( ::GetTickCount() - GetTick(CHAT_TICK) < 1000 )
                return;

            if( !IsAdmin() )
            {
                SET_ERR_GOTO( ERR_SQUARE_07, END_PROC );
            }

            SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_SQUARE_CHAT_NOT, kPacket_, kEventBuff );
            SiKCnConnector()->SendSquareRelayPacket( KUserEvent::EVENT_SQUARE_CHAT_NOT, kEventBuff, GetUID(), m_spSquare->GetUID(), true );
        }
        break;
    default:
        SET_ERR_GOTO( ERR_SQUARE_10, END_PROC );
    }

    SetTick(CHAT_TICK);

    // 채팅기록 남기기(내부에서 옵션 체크)
    m_spSquare->GetInfo( kInfo );
    SendChatLogNot( kInfo.m_strName, -1, kPacket_.m_strChatMsg );

    SET_ERROR( NET_OK );
    return;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( ERR_SQUARE_07, kPacket, 1 ); // 운영자가 아님.
        ERR_CASE( ERR_SQUARE_08, kPacket, 2 ); // 채팅 대상 유저가 광장에 존대하지 않음.
        ERR_CASE( ERR_SQUARE_09, kPacket, 3 ); // GCClub이 없이 컬러 채팅을 사용하려 했음.
        ERR_CASE( ERR_SQUARE_10, kPacket, 4 ); // 채팅 타입이 올바르지 않음.
        ERR_CASE( ERR_SQUARE_12, kPacket, 5 ); // 채팅 블럭 상태임.
    default:
        START_LOG( cerr, L"광장 채팅 요청시 알수 없는 오류.. Name : " << m_strName )
            << L" Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
            << BUILD_LOGc( kPacket_.m_cChatType ) << END_LOG;
        kPacket = 99;
    }

    START_LOG( cerr, L" 광장 채팅 요청시 오류 .. Name : " << m_strName )
        << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( kPacket )
        << BUILD_LOGc( kPacket_.m_cChatType )
        << BUILD_LOG( kPacket_.m_dwChatColor )
        << BUILD_LOG( kPacket_.m_dwReceiverUID )
        << BUILD_LOG( kPacket_.m_strChatMsg ) << END_LOG;

    SEND_PACKET( EVENT_SQUARE_CHAT_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_SQUARE_USER_SYNC_REQ )
{
    VERIFY_STATE_LOG( cwarn, ( 1, KUserFSM::STATE_SQUARE ) );
    JIF( m_spSquare );

    KSquarePos kPacket;
    kPacket.m_dwUID = GetUID();
    kPacket.m_dwSquareID = m_spSquare->GetUID();
    kPacket.m_fXpos = (float)GetSquareIndex();
    
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_SQUARE_USER_SYNC_REQ, kPacket );

    START_LOG( clog, L"광장 유저 싱크 요청,, Name : "  << m_strName << L", X_Position : " << kPacket.m_fXpos ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_NEW_CHARACTER_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_ROOM ));
    KEVENT_NEW_CHARACTER_ACK kPacket;
    KMissionInfo kMissionInfo;
    KMissionData* pkMissionData = NULL;
    kPacket.m_nOK = 99;
    std::set<DWORD> setMissionInfo;
    std::set<DWORD>::iterator sit;
    SET_ERROR( ERR_UNKNOWN );

    if( !SiKGCHelper()->GetCharacterCard( kPacket_.m_ItemID, kPacket_.m_cCharType, setMissionInfo ) ) {
        SET_ERR_GOTO( ERR_CHARACTER_01, END_PROC );
    }

    if( IsCharExist( kPacket_.m_cCharType ) ) {
        SET_ERR_GOTO( ERR_CHARACTER_00, END_PROC );
    }

    if( m_kInventory.FindItem( kPacket_.m_ItemID, kPacket_.m_ItemUID ) == NULL ) {
        SET_ERR_GOTO( ERR_INVEN_01, END_PROC );
    }

    if( !setMissionInfo.empty() ) {
        for( sit = setMissionInfo.begin(); sit != setMissionInfo.end(); ++sit ) {
            // 스크립트에 등록된 미션이 존재하는가?
            if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, *sit ) ) {
                SET_ERR_GOTO( ERR_CHARACTER_03, END_PROC ); // 존재하는 미션이 아님.
            }

            //해당 미션이 나의 미션슬롯에 있는가?
            pkMissionData = GetMissionDataFromSlot( *sit );
            if( !pkMissionData ) {
                kPacket_.m_mapMissionState[*sit] = 0; // 미션 슬롯에 등록되어 있지 않음.
            } else {
                kPacket_.m_mapMissionState[*sit] = 1; // 미션 슬롯에 현재 등록되어 있음.
            }
        }
    }

    SiKCharDefault()->GetDefaultItemInfo( kPacket_.m_cCharType, kPacket_.m_vecDefaultReward );
    SiKCharDefault()->GetDefaultSlotItemInfo( kPacket_.m_cCharType, kPacket_.m_vecDefaultSlotReward );

    QUEUEING_EVENT_TO_DB( EVENT_NEW_CHARACTER_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_INVEN_01,     kPacket.m_nOK, 1 ); // 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_CHARACTER_00, kPacket.m_nOK, 2 ); // 획득 하려는 캐릭터가 이미 존재함. )
        ERR_CASE( ERR_CHARACTER_01, kPacket.m_nOK, 3 ); // 요청한 아이템은 캐릭터 획득 카드가 아님. )
        ERR_CASE( ERR_CHARACTER_02, kPacket.m_nOK, 4 ); // 캐릭터 획득 쿼리실패. )
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"캐릭터 획득 카드 사용중 알수 없는 오류, Name : " << m_strName )
            << L"MSG : " << NetError::GetLastNetErrMsg() << END_LOG;
    }
    
    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_NEW_CHARACTER_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 2 )
        << L"Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_NEW_CHARACTER_ACK )
{
    SET_ERROR( ERR_UNKNOWN );
    NetError::SetLastNetError( kPacket_.m_nOK );
    std::set<int> setDungeonInfo;
    std::map<DWORD,int>::iterator mit;
    std::map<int,KDungeonUserInfo> mapDifficulty;
    mapDifficulty.clear();
    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        goto END_PROC;
    }

    // 캐릭터 집어 넣고.
    LIF( m_mapCharacterInfo.insert( std::make_pair( kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo ) ).second );
    LIF( AddStatInitExpInfo( kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo.m_biInitExp, kPacket_.m_kCharacterInfo.m_biInitTotalExp ) );

    // 획득한 캐릭터의 활력을 일별 최대치로 지급한다
    if ( true == SiKVitalityManager()->IsRun() ) {
        int nVitalityForCharacterCreation = 0;
        if ( 1 == m_mapCharacterInfo.size() ) { // 생성한 캐릭터가 하나인 경우 신규 유저로 판단하여 별도 수치의 활력을 지급한다
            nVitalityForCharacterCreation = SiKVitalityManager()->GetVitalityForFirstCharacterCreation();
        }
        else {
            nVitalityForCharacterCreation = SiKVitalityManager()->GetVitalityForCharacterCreation();
        }
        m_kVitalityUserData.SetVitality( static_cast< int >( kPacket_.m_kCharacterInfo.m_cCharType ), nVitalityForCharacterCreation );

        // 클라이언트에 별도의 패킷으로 알리고
        SendVitalityInfo( KVitalityManager::EVST_CREATE_CHAR );

        // 활력 충전 로그용 정보를 DB에 기록한다
        {
            KDB_EVENT_VITALITY_RECHARGE_NOT kDBPacket;

            kDBPacket.m_nVitalityUpdateType             = KVitalityManager::EVUT_INIT;
            kDBPacket.m_nRechargePoint                  = m_kVitalityUserData.m_nRechargePoint;
            kDBPacket.m_nRechargePointCounter           = m_kVitalityUserData.m_nRechargePointCounter;
            kDBPacket.m_nRemainVitality                 = m_kVitalityUserData.GetVitality( static_cast< int >( kPacket_.m_kCharacterInfo.m_cCharType ) );
            kDBPacket.m_kCharacterInfo                  = kPacket_.m_kCharacterInfo;

            QUEUEING_EVENT_TO_DB( DB_EVENT_VITALITY_RECHARGE_NOT, kDBPacket );
        }
    }

    // 신규 캐릭터 획득시 룩 정보 추가.
    {
        KLookEquipChar kLookEquip;
        kLookEquip.SetCharacter( kPacket_.m_kCharacterInfo.m_cCharType );
        m_mapLookEquip.insert( std::map<char,KLookEquipChar>::value_type( kLookEquip.GetCharacter(), kLookEquip ) );
    }

    // 신규 캐릭터 무기교체 잠금
    {
        bool bWeaponLock = false;
        m_mapChangeWeapon[kPacket_.m_kCharacterInfo.m_cCharType] = std::make_pair( bWeaponLock, 0 );
        kPacket_.m_kCharacterInfo.m_bChangeWeaponLock = bWeaponLock;
    }

    // 아이템 제거 하자.    
    LIF( m_kInventory.RemoveItem( kPacket_.m_ItemUID ) );


    for ( int i = 0 ; i < NUM_GC_GAME_MODE ; ++i ) {
        const int& nModeID = i;

        if( false == KRoom::IsDifficultyMode( nModeID ) ) {
            continue;
        }

        if ( mapDifficulty.find( nModeID ) == mapDifficulty.end() ) {
            KDungeonUserInfo kInfo;
            mapDifficulty.insert( std::make_pair( nModeID, kInfo ) );
        }

        // 던전 최대 난이도 클리어 설정 (헬모드 제외)
        int nLastDifficulty = SiKResultManager()->GetLastDifficulty( nModeID );
        if ( nLastDifficulty > -1 ) {
            if ( mapDifficulty[nModeID].m_kModeDifficulty.Get( nLastDifficulty ) ) {
                mapDifficulty[nModeID].m_kClearData.m_bClear = true;
            }
        }

        KHeroDungeon kInfo;
        bool bHeroDungeon = false;
        if( SiKHeroDungeonManager()->IsHeroDungeon( nModeID ) )
            bHeroDungeon = SiKHeroDungeonManager()->GetHeroDungeonInfo( nModeID, kInfo );

        // 일반던전이거나 난이도 리셋하지않는 영던일 경우.
        if ( !bHeroDungeon || (bHeroDungeon && !kInfo.m_bResetDifficulty) ) {
            // 플레이가능한 난이도 클리어 설정
            for ( int i = GC_GMD_NORMAL_BEGIN ; i < nLastDifficulty ; ++i ) {
                if ( false == mapDifficulty[nModeID].m_kModeDifficulty.Get( i ) ) {
                    break;
                }

                mapDifficulty[nModeID].m_kClearData.m_shMaxDifficulty = i;
            }
        }
    }

    // 신규 캐릭터 생성시 Default던전 정보가 있으면 세팅해 주자.
    if( SiKCharDefault()->CheckCharDungeonInfo( kPacket_.m_kCharacterInfo.m_cCharType, setDungeonInfo) ) {
        std::set<int>::iterator sit;
        for( sit = setDungeonInfo.begin(); sit != setDungeonInfo.end(); ++sit ) {
            std::map<int,KDungeonUserInfo>::iterator mit;
            mit  = mapDifficulty.find( *sit );
            if( mit == mapDifficulty.end() ) { // 현재 해당 던전이 오픈되지 않았을 경우에만 오픈해 주자.
                KDungeonUserInfo kInfo;
                kInfo.m_kModeDifficulty.Set(0);
                mapDifficulty.insert( std::make_pair( *sit, kInfo ) );
            } else {
                mit->second.m_kModeDifficulty.Set(0);
            }
        }
    }
    UpdateCharDifficultInfo( kPacket_.m_kCharacterInfo.m_cCharType, mapDifficulty );
    kPacket_.m_mapDifficulty = mapDifficulty;

    for( mit = kPacket_.m_mapMissionState.begin(); mit != kPacket_.m_mapMissionState.end(); ++mit ) {
        if( mit->second == 1 ) {
            EraseMissionDataFromSlot( mit->first );
            UpdateCompleteMission( mit->first );
            kPacket_.m_dwMissionID = mit->first;
        }
    }

    // 해당 캐릭터의 상하이 정보 추가.
    // 신규유저 상하이 지급
    {
        KSHCheckPoint::SHChecData shDataNew;
        shDataNew.m_cCurrentCharID = kPacket_.m_kCharacterInfo.m_cCharType;
        shDataNew.m_cOldLevel = static_cast<char>( kPacket_.m_kCharacterInfo.m_dwLevel );
        shDataNew.m_cLevel = static_cast<char>( kPacket_.m_kCharacterInfo.m_dwLevel );
        shDataNew.m_strLogin = GetName();

        KEVENT_SHANGHAI_DROP_REQ shReqNew;
        shReqNew.m_vecDropItems.clear();
        shReqNew.m_vecDropPostItems.clear();
        shReqNew.m_vecAutoMission.clear();
        SiKSHCheckPoint()->NewCharacterDropCheck( shReqNew.m_vecDropItems, shReqNew.m_vecDropPostItems, shReqNew.m_vecAutoMission, shDataNew, KUser::USER_CT_TOTAL );
        SiKSHCheckPoint()->NewCharacterDropCheck( shReqNew.m_vecDropItems, shReqNew.m_vecDropPostItems, shReqNew.m_vecAutoMission, shDataNew, GetUserChannelType() );

        if ( !shReqNew.m_vecDropItems.empty() || !shReqNew.m_vecDropPostItems.empty() || !shReqNew.m_vecAutoMission.empty() ) {
            shReqNew.m_cCharType = kPacket_.m_kCharacterInfo.m_cCharType;
            CheckLevelUpDrop( shReqNew );
        }
    }

    // 유저 인벤토리 갱신.
    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_INVEN_01,     kPacket_.m_nOK, 1 ); // 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_CHARACTER_00, kPacket_.m_nOK, 2 ); // 획득 하려는 캐릭터가 이미 존재함. )
        ERR_CASE( ERR_CHARACTER_01, kPacket_.m_nOK, 3 ); // 요청한 아이템은 캐릭터 획득 카드가 아님. )
        ERR_CASE( ERR_CHARACTER_02, kPacket_.m_nOK, 4 ); // 캐릭터 획득 쿼리실패. )
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"캐릭터 획득 카드 사용중 알수 없는 오류, Name : " << m_strName )
            << L"MSG : " << NetError::GetLastNetErrMsg() << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_NEW_CHARACTER_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 2 )
        << L"Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOGc( kPacket_.m_kCharacterInfo.m_cCharType )
        << BUILD_LOG( kPacket_.m_mapDifficulty.size() )
        << BUILD_LOG( setDungeonInfo.size() ) << END_LOG;
}

//0 : 성공
//-1 : 해당 유저 정보가 존재하지 않음
//-2 : 해당 유저의 동의 정보가 존재하지 않음
//-3 : 지정된 동의값이 없음
//-101이하 : 트랜잭션 에러
_IMPL_ON_FUNC( EVENT_TW_USER_AGREEMENT_REQ, int ) // 080215 tslayer. 대만 정형화 Client -> Server
{
    START_LOG( clog, L"유저 동의, Name : " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;
    
    if( kPacket_ >= AGREEMENT_NUM || kPacket_ <= 0 )
    {
        int nOK = -3; // err.계약 동의값을 벗어남
        SEND_DATA( EVENT_TW_USER_AGREEMENT_ACK, nOK );
        return;
    }

    QUEUEING_EVENT_TO_DB( EVENT_TW_USER_AGREEMENT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_TW_USER_AGREEMENT_ACK, int ) // 080215 tslayer. 대만 정형화
{
    START_LOG( clog, L"대만 정형화 ack")
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( m_strName ) << END_LOG;

    // send ret  to client
    SEND_RECEIVED_PACKET( EVENT_TW_USER_AGREEMENT_ACK );
}

_IMPL_ON_FUNC( EVENT_USE_EMOTICON_REQ, KUserEmoticon )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));
    JIF( m_spSquare );

    kPacket_.m_dwUserUID = GetUID();
    KQuickSlot kQuickSlot;
    GetCharQuickSlotInfo( kQuickSlot );
    // 해당 유저가 이 아이템을 이모티콘으로 장착하고 있는가?
    if( !IsInSlotByItemID( kQuickSlot.m_vecEmoticonSlot, kPacket_.m_dwItemID ) )
    {
        START_LOG( cerr, L"해당 이모티콘 아이템을 가지고 있지 않음. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_dwItemID )
            << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
        int nOK = 1;
        SEND_DATA( EVENT_USE_EMOTICON_ACK, nOK );
        return;
    }

    KSerBuffer kEventBuff;

    SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_USE_EMOTICON_NOT, kPacket_, kEventBuff );
    SiKCnConnector()->SendSquareRelayPacket( KUserEvent::EVENT_USE_EMOTICON_NOT, kEventBuff, GetUID(), m_spSquare->GetUID(), false );

}

IMPL_ON_FUNC( EVENT_INVITED_ROOM_FROM_SQUARE_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));
    JIF( m_spSquare );
    SET_ERROR( ERR_UNKNOWN );
    KJoinRoomReqInfo kReq;
    KJoinRoomAckInfo kAck;
    kAck.m_nOK = 99;
    KSquarePtr spSquare = m_spSquare; // 임시로 저장해둔다.


    // 해당 유저를 찾아서
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwHostUID );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_SQUARE_JOIN_ERR_00, END_PROC );
    }
    // 채널 있는지, 같은지..
    KChannel* pkChannel = spUser->GetChannel();
    if( !pkChannel || pkChannel->GetUID() != kPacket_.m_dwChannelUID )
    {
        SET_ERR_GOTO( ERR_SQUARE_JOIN_ERR_01, END_PROC );
    }
    KRoomManager* pkRoomMng = pkChannel->GetRoomManager();
    if( !pkRoomMng )
    {
        SET_ERR_GOTO( ERR_SQUARE_JOIN_ERR_02, END_PROC );
    }
    // 룸 존재 하는지 같은지..
    KRoom* pkRoom = spUser->GetRoom();
    if( !pkRoom || pkRoom->GetRoomID() != kPacket_.m_usRoomID )
    {
        SET_ERR_GOTO( ERR_SQUARE_JOIN_ERR_02,END_PROC );
    }
    // 이녀석 방장인지..
    KUser* pkUser = pkRoom->GetHostPtr();
    if( !pkUser || pkUser->GetUID() != kPacket_.m_dwHostUID )
    {
        SET_ERR_GOTO( ERR_SQUARE_JOIN_ERR_03, END_PROC );
    }

    if( pkRoom->GetStateID() != KRoomFSM::STATE_WAITING )
    {
        SET_ERR_GOTO( ERR_ROOM_05, END_PROC );
    }
    
    if( pkRoom->m_strPasswd != kPacket_.m_strRoomPasswd )
    {
        SET_ERR_GOTO( ERR_ROOM_06, END_PROC );
    }
    // 확인하고.. 방으로 입장. 처리..
    // 계급은 맞는지.. -- 이녀석 확실히..

    LeaveSquare();
    if( !SiKChannelManager()->EnterChannel( kPacket_.m_dwChannelUID, this ) )
    {
        START_LOG( cerr, L" 광장에서 방 접속시  채널 입장 실패.. Name : " << m_strName )
            << L"Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        if( !RollBackToSquare( spSquare ) )
        {
            SET_ERR_GOTO( ERR_SQUARE_JOIN_ERR_04, END_PROC );
        }
        SET_ERR_GOTO( ERR_ROOM_05, END_PROC );
    }

    kReq.m_uiJoinType           = KJoinRoomReqInfo::JOIN_NORMAL;
    kReq.m_usRoomID             = kPacket_.m_usRoomID;
    kReq.m_strRoomPasswd        = kPacket_.m_strRoomPasswd;
    kReq.m_cQuickJoinCategory   = -1;
    kReq.m_nQuickJoinModeID     = -1;
    kReq.m_vecKickedRoomID.clear();   
    kReq.m_nDifficult           = -1;
    kReq.m_kInDoorUserInfo      = kPacket_.m_kIndoorUserInfo;

    if ( GetCurrentCharType() != kReq.m_kInDoorUserInfo.m_cCharIndex ) { // 이경우는 발생하면 안된다.
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_ROOM_35, END_PROC );
    }
    MakeCorrectInDoorInfo( kReq.m_kInDoorUserInfo );

    if( !pkRoomMng->JoinRoom( this, kReq, kAck ) ) // 안에서 SetError 하고 있음.
    {
        START_LOG( cwarn, L"광장에서 방입장 실패.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( NetError::GetLastNetError() ) << END_LOG;
        if( !RollBackToSquare( spSquare ) )
        {
            SET_ERR_GOTO( ERR_SQUARE_JOIN_ERR_04, END_PROC );
        }
        SET_ERR_GOTO( ERR_ROOM_05, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kAck.m_nOK, 0 ); 
        ERR_CASE( ERR_SQUARE_JOIN_ERR_00, kAck.m_nOK, 1 );  // 초대 요청한 유저가 존재하지 안음. )
        ERR_CASE( ERR_SQUARE_JOIN_ERR_01, kAck.m_nOK, 2 );  // 해당 채널과 요청 채널이 맞지 않음. )
        ERR_CASE( ERR_SQUARE_JOIN_ERR_02, kAck.m_nOK, 3 );  // 해당 Room과 요청 Room이 맞지 않음. )
        ERR_CASE( ERR_SQUARE_JOIN_ERR_03, kAck.m_nOK, 4 );  // 초대 요청자가 이미 방장이 아님. )
        ERR_CASE( ERR_SQUARE_JOIN_ERR_04, kAck.m_nOK, 5 );  // 상태 전이중 문제 발생 채널 로비로 강제 퇴장 )
        ERR_CASE( ERR_ROOM_04,            kAck.m_nOK, 6 );  // 방 참여 실패. 잘못된 정보로 접속을 시도함.
        ERR_CASE( ERR_ROOM_19,            kAck.m_nOK, 7 );  // 방 참여 실패. P2P 버전이 맞지 않음. )
        ERR_CASE( ERR_ROOM_05,            kAck.m_nOK, 8 );  // 방 참여 실패. 현재 방이 대기상태가 아님. )
        ERR_CASE( ERR_ROOM_06,            kAck.m_nOK, 9 );  // 방 참여 실패. 비밀번호가 다름. )
        ERR_CASE( ERR_ROOM_03,            kAck.m_nOK, 10 ); // 방 접속 불가능. 방장 부재중 )
        ERR_CASE( ERR_ROOM_17,            kAck.m_nOK, 11 ); // 방 참여 실패. 방장에게 접속 알림 패킷을 보내지 못함. )
        ERR_CASE( ERR_ROOM_35,            kAck.m_nOK, 12 ); // 존재하지 않는 캐릭터가 선택되었습니다. )
        ERR_CASE( ERR_ROOM_36,            kAck.m_nOK, 13 ); // 입장할수 있는 올바른 레벨이 아닙니다. )
        ERR_CASE( ERR_ROOM_01,            kAck.m_nOK, 14 ); // 방 접속 불가능. 정원이 모두 찾습니다. )   // 접속자가 운영자 권한이 없음)
        ERR_CASE( ERR_ROOM_13,            kAck.m_nOK, 15 ); // 방 참여 실패. 운영자 이벤트 기간중 이벤트 아이템을 이미 가지고 있음. )
        ERR_CASE( ERR_ROOM_30,            kAck.m_nOK, 16 ); // 옵저버가 이미 존재하는 방에 옵저버가 들어갈려고 했다. )
        ERR_CASE( ERR_ROOM_33,            kAck.m_nOK, 17 ); // 더이상 다른 길드는 입장할수 없습니다. )
        ERR_CASE( ERR_ROOM_34,            kAck.m_nOK, 18 ); // 이미 동일한 길드원이 3명 있습니다. )
        ERR_CASE( ERR_ROOM_02,            kAck.m_nOK, 19 ); // 방 접속 불가능. 현재 방에 동시에 다른 사용자가 접속중 임.)  // 현재 방에 똑같은 UID를 가진 사용자가 접속중임
    default:
        START_LOG( cerr, L"광장에서 룸 접속시 알수 없는 오류 : " << m_strName )
            << L"MSG : " << NetError::GetLastNetErrMsg() << END_LOG;
        kAck.m_nOK = 99;
    }

    if( kAck.m_nOK == 0 )
    {
        const KInDoorUserInfo* pkInDoorUserInfo = m_pkRoom->FindInDoorUserInfo( m_strName );
        if( pkInDoorUserInfo )
        {
            m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_JOIN_ROOM_BROAD, (*pkInDoorUserInfo), true );

            START_LOG( clog, L"광장에서 방으로 조인 .. Name : " << m_strName )
                << BUILD_LOG( pkInDoorUserInfo->m_nState )
                << BUILD_LOG( pkInDoorUserInfo->m_iPlayerIndex ) 
                << BUILD_LOG( pkInDoorUserInfo->m_strGuildName )
                << BUILD_LOG( pkInDoorUserInfo->m_bIsHost )
                << BUILD_LOG( pkInDoorUserInfo->m_bIsLive)
                << BUILD_LOG( pkInDoorUserInfo->m_bIsObserver )
                << BUILD_LOGc( pkInDoorUserInfo->m_cAuthLevel )
                << BUILD_LOGc( pkInDoorUserInfo->m_cCharIndex )
                << BUILD_LOGc( pkInDoorUserInfo->m_cPromotion )
                << BUILD_LOG( pkInDoorUserInfo->m_dwGuildUID ) << END_LOG;

            DumpInDoorCharInfo( pkInDoorUserInfo->m_vecCharInfo );
        }
    }

    SEND_COMPRESS_PACKET( EVENT_INVITED_ROOM_FROM_SQUARE_ACK, kAck );

    LOG_CONDITION( kAck.m_nOK == 0, clog, cwarn )
        << L" 광장에서의 방으로 접속 : " << kAck.m_nOK << dbg::endl
        << BUILD_LOG( kPacket_.m_dwChannelUID )
        << BUILD_LOG( kPacket_.m_dwHostUID )
        << BUILD_LOG( kPacket_.m_usRoomID )
        << BUILD_LOG( kPacket_.m_strRoomPasswd ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SQUARE_USER_LIST_REQ, PAIR_DWORD_INT )
{
    if( !IsAdmin() )
    {
        _VERIFY_STATE(( 1, KUserFSM::STATE_ROOM )); // 이제는 방에서 초대할때만 유저 리스트 요청한다.
        JIF( m_pkRoom );
    }
    JIF( m_pkChannel );

    KPageInfo kPacket;
    kPacket.m_nMaxPage = 0;
    kPacket.m_nCurrentPage = 0;
    kPacket.m_buffCompList.Clear();
    KSquarePtr spSquare = SiKSquareManager()->GetSquare( kPacket_.first );
    if( !spSquare )
    {
        kPacket.m_nOK = 1;
        START_LOG( cerr, L"요청한 광장이 존재 하지 않음. Name : " << m_strName )
            << BUILD_LOG( kPacket_.first )
            << BUILD_LOG( kPacket_.second ) << END_LOG;
        SEND_PACKET( EVENT_SQUARE_USER_LIST_ACK );
        return;
    }

    spSquare->GetUserList( kPacket_.second, kPacket );
    kPacket.m_nOK = 0;
    SEND_PACKET( EVENT_SQUARE_USER_LIST_ACK );
    START_LOG( clog, L"룸안에서 광장내 유저 리스트 요청, Name : " << m_strName )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nCurrentPage )
        << BUILD_LOG( kPacket.m_nMaxPage )
        << BUILD_LOG( kPacket.m_buffCompList.GetLength() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_INVITE_DENY_NOT, bool )
{
    m_bInviteDeny = kPacket_;
}

IMPL_ON_FUNC_NOPARAM( EVENT_SIGN_BOARD_NUM_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    std::pair<DWORD,DWORD> kPacket;
    kPacket.first = SiKSignBoard()->GetCapacity();
    kPacket.second = SiKSignBoard()->GetSize();

    SEND_PACKET( EVENT_SIGN_BOARD_NUM_ACK );
}

_IMPL_ON_FUNC( EVENT_SIGN_BOARD_REG_REQ, KSignBoardData )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_SQUARE, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KEVENT_SIGN_BOARD_REG_ACK kPacket;
    kPacket.m_nOK           = 99;
    kPacket.m_dwOrder       = 0;
    kPacket.m_dwItemUID     = 0;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 메세지가 비어 있나?
    if( kPacket_.m_strMsg.empty() )
    {
        SET_ERR_GOTO( ERR_SIGN_BOARD_00, END_PROC ); // 메세지가 비어 있음.
    }

    if( !SiKSignBoard()->IsCorrectScope( kPacket_.m_eType, kPacket_.m_ItemID ) )
    {
        SET_ERR_GOTO( ERR_SIGN_BOARD_01, END_PROC ); // Item의 요청 범위가 맞지 않습니다.
    }

    // Item이 존재 하느냐?
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_ItemID );
    if( !pkItem )
    {
        SET_ERR_GOTO( ERR_INVEN_02, END_PROC ); // 아이템 수량이 부족.
    }

    kPacket_.m_dwSenderUID = GetUID();
    kPacket_.m_strSenderNick = m_strNickName;

    if( pkItem->m_nCount > 0 )
        pkItem->m_nCount -= 1;
    kPacket.m_dwOrder       = SiKSignBoard()->QueueingData( kPacket_ );
    kPacket.m_dwItemUID     = pkItem->m_ItemUID;

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SIGN_BOARD_01,    kPacket.m_nOK, 1 ); // Item의 요청 범위가 맞지 않습니다.
        ERR_CASE( ERR_INVEN_02,         kPacket.m_nOK, 2 ); // 아이템 수량이 부족.
        ERR_CASE( ERR_SIGN_BOARD_00,    kPacket.m_nOK, 3 ); // 메세지가 비어 있음.
    default:
        START_LOG( cerr, L"알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_SIGN_BOARD_REG_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L" 전광판 메세지 등록 : " << (kPacket.m_nOK==0? L"성공" : L"실패" ) << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_eType )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_strSenderNick )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_strMsg )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwItemUID )
        << BUILD_LOG( kPacket.m_dwOrder ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_SIGN_BOARD_NOTIFY )
{
    JIF( IsAdmin() );

    JIF ( kPacket_.m_strMsg.length() < 410 ); // 클라이언트의 버퍼 제한

    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID        = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = kPacket_.m_eType;
    kData.m_strMsg          = kPacket_.m_strMsg;
    kData.m_strSenderNick.clear();
    SiKSignBoard()->QueueingAdminData( kData );
}

// 이모티콘
IMPL_ON_FUNC( EVENT_EMOTICON_MIX_REQ )
{
    // 이동작은 어느 상태에서 할것이냐?
    _VERIFY_STATE((2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    SET_ERROR( ERR_UNKNOWN );

    std::set<GCITEMID>::iterator sit;
    KEVENT_EMOTICON_MIX_ACK kPacket;
    kPacket.m_nOK = 99;

    // kPa세개의 다른 이모티콘 아이템이냐? (set은 같은 값이 들어갈 수 없음)
    if( kPacket_.m_setMixMaterial.size() != 3 )
    {
        SET_ERR_GOTO( ERR_EMOTICON_MIX_00, END_PROC );
    }
    // GP가 충분한가?
    if( GetGP() < 3000 )
    {
        SET_ERR_GOTO( ERR_EMOTICON_MIX_01, END_PROC );
    }

    // 아이템이 존재하느냐?
    for( sit = kPacket_.m_setMixMaterial.begin() ; sit != kPacket_.m_setMixMaterial.end() ; ++sit )
    {
        if( NULL == m_kInventory.FindItemByItemID( *sit ) )
        {
            SET_ERR_GOTO( ERR_EMOTICON_MIX_02, END_PROC );
        }
    }
    // 조합 Item ID는 10 부터 시작 한다.
    if( !SiKGCHelper()->EmotconMixResult( kPacket_.m_dwResultItem ) || kPacket_.m_dwResultItem < 10 )
    {
        SET_ERR_GOTO( ERR_EMOTICON_MIX_03, END_PROC );
    }

    // DB로 보내자.
    QUEUEING_EVENT_TO_DB( EVENT_EMOTICON_MIX_REQ, kPacket_ );

    SET_ERROR( NET_OK );


END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                 kPacket.m_nOK, 0 );
        ERR_CASE( ERR_EMOTICON_MIX_00,    kPacket.m_nOK, 1 );
        ERR_CASE( ERR_EMOTICON_MIX_01,    kPacket.m_nOK, 2 );
        ERR_CASE( ERR_EMOTICON_MIX_02,    kPacket.m_nOK, 3 );
        ERR_CASE( ERR_EMOTICON_MIX_03,    kPacket.m_nOK, 4 );
        ERR_CASE( ERR_EMOTICON_MIX_04,    kPacket.m_nOK, 5 );
    default:
        START_LOG( cerr, L"알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_EMOTICON_MIX_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L"이모티콘 조합 요청 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwResultItem )
        << BUILD_LOG( kPacket_.m_setMixMaterial.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_EMOTICON_MIX_ACK )
{
    std::set<GCITEMID>::iterator sit;
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }

    // 사용한 아이템 수량 감소.
    for( sit = kPacket_.m_setMixMaterial.begin() ; sit != kPacket_.m_setMixMaterial.end() ; ++sit )
    {
        KItemPtr pkItem = m_kInventory.FindItemByItemID( *sit );
        if( pkItem != NULL )
        {
            pkItem->m_nCount -= 1;
        }
    }
    // GP 감소.
    bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
    DecreaseGP( KGpStatInfo::GDT_EMOTICON_MIX, bDBUpdate, 3000 );
    kPacket_.m_nGamePoint = GetGP();
    // 얻어온 아이템 인벤토리에 삽입.
    LIF( m_kInventory.AddItemList( kPacket_.m_vecResultItem ) );

    // 통계
    {
        const KCharacterInfo* pkChar = GetBestCharacter();
        KStatEmoticonPtr spStat = GET_STAT( KStatEmoticon, SI_STAT_EMOTICON );
        if( spStat && pkChar )
        {
            spStat->AddStat( (int)pkChar->m_dwLevel );
        }
    }
    

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                 kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_EMOTICON_MIX_00,    kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_EMOTICON_MIX_01,    kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_EMOTICON_MIX_02,    kPacket_.m_nOK, 3 );
        ERR_CASE( ERR_EMOTICON_MIX_03,    kPacket_.m_nOK, 4 );
        ERR_CASE( ERR_EMOTICON_MIX_04,    kPacket_.m_nOK, 5 );
    default:
        START_LOG( cerr, L"알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_EMOTICON_MIX_ACK );
}

_IMPL_ON_FUNC( EVENT_CHANGE_OBSERVER_REQ, bool )
{
    JIF( IsAdmin() ); // 운영자만 보낼수 있다.

    m_bIsObserver = kPacket_;
    int kPacket = (kPacket_? 0 : 1 );
    SEND_PACKET( EVENT_CHANGE_OBSERVER_ACK );
}

_IMPL_ON_FUNC( EVENT_CONNECTION_MSG_ACK, std::vector<std::wstring> )
{
    SEND_COMPRESS_PACKET( EVENT_CONNECTION_MSG_ACK, kPacket_ );
}

IMPL_ON_FUNC( EVENT_RANK_PAGE_REQ )
{
    VERIFY_STATE((1, KUserFSM::STATE_SQUARE ));
    JIF( m_spSquare );
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_RANK_PAGE_ACK kPacket;
    KListPage kPage;

    if( !SiKRankManager()->GetPageData( kPacket_.m_cRnkType, kPacket_.m_cCharType, kPacket_.m_cPageNum, kPacket ) )
    {
        goto END_PROC;
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 ); // 
        ERR_CASE( ERR_RANK_00,  kPacket.m_nOK, 1 ); // 해당 랭킹 타입이 존재하지 않음.
        ERR_CASE( ERR_RANK_01,  kPacket.m_nOK, 2 ); // 해당 캐릭터 타입이 존재하지 않음.
        ERR_CASE( ERR_RANK_02,  kPacket.m_nOK, 3 ); // 해당 페이지가 존재하지 않음.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"랭킹 정보 요청시 알수 없는 오류,, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_RANK_PAGE_ACK );
    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cRnkType )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cPageNum )
        << BUILD_LOGc( kPacket.m_cTotalPageCount )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_MY_RANK_INFO_NOT, KMyRankInfo )
{
    m_kRankInfo = kPacket_;
    SEND_RECEIVED_PACKET( EVENT_MY_RANK_INFO_NOT );
}

IMPL_ON_FUNC( EVENT_RANK_SEARCH_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));

    KEVENT_RANK_SEARCH_ACK kPacket;
    kPacket.m_nOK = -99;

    // 시간 체크.
    if( ::GetTickCount() - GetTick( RANK_SEARCH_TICK ) < KRankManager::ms_uiRankSearchGap )
    {
        SET_ERR_GOTO( ERR_RANK_09, END_PROC );
    }

    SET_ERROR( ERR_UNKNOWN );
    // 랭킹 타입 체크
    if( kPacket_.m_cRankType < KRankManager::RT_DAILY_RANK ||
        kPacket_.m_cRankType >= KRankManager::RT_MAX )
    {
        SET_ERR_GOTO( ERR_RANK_05, END_PROC );
    }

    // 캐릭터 타입 체크
    if( kPacket_.m_cCharType < GC_CHAR_INVALID ||
        kPacket_.m_cCharType >= GC_CHAR_NUM )
    {
        SET_ERR_GOTO( ERR_RANK_06, END_PROC );
    }

    // 닉네임 길이 체크
    if( kPacket_.m_strNickName.empty() )
    {
        SET_ERR_GOTO( ERR_RANK_03, END_PROC );
    }

    // 쿼리에 사용할수 없는 문자 포함 되어 있는지 체크
    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strNickName ) )
    {
        SET_ERR_GOTO( ERR_RANK_04, END_PROC );
    }

    // 나의 닉네임과 같은가?( 길드 랭킹 검색 )
    if( kPacket_.m_strNickName == m_strNickName ) {
        SET_ERR_GOTO( ERR_RANK_07, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_RANK_SEARCH_REQ, kPacket_ );

    SetTick( KUser::RANK_SEARCH_TICK );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RANK_03,  kPacket.m_nOK, 1 ); // 랭킹 조사시 닉네임이 비어 있음. 
        ERR_CASE( ERR_RANK_04,  kPacket.m_nOK, 2 ); // 랭킹 조사시 닉네임에 사용할수 없는 문자 포함. 
        ERR_CASE( ERR_RANK_05,  kPacket.m_nOK, 3 ); // 랭킹 조사시 없는 랭킹 타입 입니다. 
        ERR_CASE( ERR_RANK_06,  kPacket.m_nOK, 4 ); // 랭킹 조사시 없는 캐릭터 타입 입니다. 
        ERR_CASE( ERR_RANK_07,  kPacket.m_nOK, 5 ); // 랭킹 조사시 자기 자신을 조사하려 했음. 
        ERR_CASE( ERR_RANK_08,  kPacket.m_nOK, 6 ); // 랭킹 정보가 없음. 
        ERR_CASE( ERR_RANK_09,  kPacket.m_nOK, 7 ); // 랭킹 검색 주기가 짧음. 
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"유저 랭킹 검색시 알수 없는 오류 발생, Name : " << m_strName )
            << BUILD_LOGc( kPacket_.m_cRankType )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( kPacket_.m_strNickName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_RANK_SEARCH_ACK );

    LOG_CONDITION( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_NOT_FINISHED_YET, clog, cwarn )
        << L"유저 랭킹 검색요청 : " << (kPacket.m_nOK == 0? L"성공":L"실패" ) << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cRankType )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_RANK_SEARCH_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RANK_03,  kPacket_.m_nOK, 1 ); // 랭킹 조사시 닉네임이 비어 있음. 
        ERR_CASE( ERR_RANK_04,  kPacket_.m_nOK, 2 ); // 랭킹 조사시 닉네임에 사용할수 없는 문자 포함. 
        ERR_CASE( ERR_RANK_05,  kPacket_.m_nOK, 3 ); // 랭킹 조사시 없는 랭킹 타입 입니다. 
        ERR_CASE( ERR_RANK_06,  kPacket_.m_nOK, 4 ); // 랭킹 조사시 없는 캐릭터 타입 입니다. 
        ERR_CASE( ERR_RANK_07,  kPacket_.m_nOK, 5 ); // 랭킹 조사시 자기 자신을 조사하려 했음. 
        ERR_CASE( ERR_RANK_08,  kPacket_.m_nOK, 6 ); // 랭킹 정보가 없음. 

    default:
        START_LOG( cerr, L"유저 랭킹 검색시 알수 없는 오류 발생, Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOGc( kPacket_.m_cRankType )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_RANK_SEARCH_ACK );

    LOG_CONDITION( kPacket_.m_nOK == 0 , clog, cwarn )
        << L"유저 랭킹 검색 결과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOGc( kPacket_.m_cRankType )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_kExpRank.m_nRank )
        << BUILD_LOG( kPacket_.m_kWinRank.m_nRank ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_GET_HELL_ITEM_ACK, std::vector<KItem> )
{
    JIF( PacketHackCheck( nFrom_ ) );
    JIF( !kPacket_.empty() );
    m_kInventory.AddItemList( kPacket_ );
    SEND_RECEIVED_PACKET( EVENT_GET_HELL_ITEM_ACK );
}

_IMPL_ON_FUNC( EVENT_SELL_INFO_REQ, GCITEMID )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_SELL_INFO_ACK kPacket;
    kPacket.m_nOK = 99;
    kPacket.m_ItemID = kPacket_;

    if( !SiKItemManager()->GetSellInfo( kPacket_, kPacket.m_kSellInfo ) )
    {
        SET_ERR_GOTO( ERR_SELL_COUNT_ITEM_00, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SELL_COUNT_ITEM_00, kPacket.m_nOK, 1 ); // 해당 아이템 정보가 서버에 존재하지 않음
    default:
        START_LOG( cerr, L"수량 아이템 판매 정보 얻는중 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_ ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << L" 수량 아이템 판매 정보 얻는중 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    SEND_PACKET( EVENT_SELL_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_SELL_COUNT_ITEM_REQ, KBuySellItemReq )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KDBSellCountItemData kDBPacket;
    KEVENT_SELL_ITEM_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkItem;

    if( kPacket_.m_nCount <= 0 )
    {
        SET_ERR_GOTO( ERR_SELL_COUNT_ITEM_03, END_PROC );
    }

    // 해당 아이템을 보유 하고 있는가?
    pkItem = m_kInventory.FindItem( kPacket_.m_ItemID, kPacket_.m_ItemUID );
    if( !pkItem )
    {
        SET_ERR_GOTO( ERR_SELL_COUNT_ITEM_01, END_PROC );
    }

    if ( pkItem->m_nPeriod != KItem::UNLIMITED_ITEM ) {
        SET_ERR_GOTO( ERR_SELL_COUNT_ITEM_01, END_PROC );
    }

    if( pkItem->m_nCount < kPacket_.m_nCount )
    {
        SET_ERR_GOTO( ERR_SELL_COUNT_ITEM_02, END_PROC );
    }

    kDBPacket.m_kReqItem    = *pkItem;
    kDBPacket.m_nSellFactor = kPacket_.m_nCount;
    GetDBUpdateData( kDBPacket.m_kData );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SELL_COUNT_ITEM_REQ, kDBPacket );

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SELL_COUNT_ITEM_01, kPacket.m_nOK, 1 );   // 팔려고 하는 수량아이템이 없음
        ERR_CASE( ERR_SELL_COUNT_ITEM_02, kPacket.m_nOK, 2 );   // 팔려고 하는 수량아이템의 수량이 부족함
        ERR_CASE( ERR_SELL_COUNT_ITEM_03, kPacket.m_nOK, 3 );   // 0개 혹은 음수개를 팔려고 함.
        ERR_CASE( ERR_SELL_COUNT_ITEM_04, kPacket.m_nOK, 4 );   // 수량 아이템 판매시 해당 유저가 존재하지 않음
        ERR_CASE( ERR_SELL_COUNT_ITEM_05, kPacket.m_nOK, 5 );   // 수량 아이템 판매시 해당 아이템 정보가 DB에 없음
        ERR_CASE( ERR_INVEN_13, kPacket.m_nOK, 6 );             // Item 동기화 실패
        ERR_CASE( ERR_DB_COMMON_03, kPacket.m_nOK, 7 );         // DB와 User Data 동기화 실패
        ERR_CASE( ERR_DB_COMMON_00, kPacket.m_nOK, 8 );         // 트랜잭션 에러
        ERR_CASE( ERR_SELL_COUNT_ITEM_06, kPacket.m_nOK, 9 );   // DB 작업 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"수량 아이템 판매 요청시 알수 없는 오류 : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( NetError::GetLastNetError() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
    {
        SEND_PACKET( EVENT_SELL_COUNT_ITEM_ACK );
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << L" 수량 아이템 판매 요청 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nCount ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SELL_COUNT_ITEM_ACK, KDBSellCountItemData )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    KEVENT_SELL_COUNT_ITEM_ACK kPacket;

    if( NetError::GetLastNetError() != NetError::ERR_DB_COMMON_03 && 
        NetError::GetLastNetError() != NetError::ERR_INVEN_13 )
    {
        // 성공 실패의 여부와 관계 없이 위의 오류가 아니면 아이템 동기화 해준다.
        KItemPtr pkItem = m_kInventory.FindItem( kPacket_.m_kReqItem.m_ItemID, kPacket_.m_kReqItem.m_ItemUID );
        if( pkItem && kPacket_.m_kReqItem.m_nCount != kPacket_.m_kReqItem.m_nInitCount )
        {
            // 왜 패킷에 있는 값으로 동기화 하냐면.. 흠...
            // 지연으로 인해서 타임아웃이 걸렸을때, 해당 아이템을 쓰거나 했을때를 대비..
            pkItem->m_nInitCount = kPacket_.m_kReqItem.m_nCount;
        }
    }

    if( NetError::GetLastNetError() != NetError::NET_OK ) goto END_PROC;

    // 처리 합시다.. 여기서 부터는 낙장 불입.
    // 팔려고 했던 아이템을 업데이트 합니다.

    { // 아래 변수를 지역으로 한정 시키기 위해 Brace 사용.
        // 수량에 상관 없이 해당 아이템을 가져 온다.
        KItemPtr pkReqItem = m_kInventory.FindItem( kPacket_.m_kReqItem.m_ItemID, kPacket_.m_kReqItem.m_ItemUID );
        if( pkReqItem )
        {
            *pkReqItem = kPacket_.m_kReqItem;
            if( kPacket_.m_kReqItem.m_nCount <= 0 )
            {
                __LIF( m_kInventory.RemoveItem( kPacket_.m_kReqItem.m_ItemID, kPacket_.m_kReqItem.m_ItemUID ) );
            }
        }
        else
        {
            START_LOG( cerr, L"DB 작업이 끝난후, 처리할때 해당 아이템을 찾지 못함. Name : " << m_strName )
                << BUILD_LOG( kPacket_.m_kReqItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_kReqItem.m_ItemUID ) << END_LOG;
        }
    }

    m_kInventory.AddItemList( kPacket_.m_vecDurationItem );

    if ( kPacket_.m_nGP > 0 ) { // 판매 보상 이 GP 인 경우
        SetGP( kPacket_.m_nGP );
        SetInitGP( GetGP() );
    }

    kPacket.m_nGP               = GetGP();
    kPacket.m_vecDurationItem   = kPacket_.m_vecDurationItem;
    kPacket.m_kReqItem          = kPacket_.m_kReqItem; // 우선 안전한 데이터를 쓰자.

    // 처음 함수 진입시 SetError 해?기 때문에 일부러 또 할필요 없음.
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SELL_COUNT_ITEM_01,   kPacket.m_nOK, 1 ); // 팔려고 하는 수량아이템이 없음
        ERR_CASE( ERR_SELL_COUNT_ITEM_02,   kPacket.m_nOK, 2 ); // 팔려고 하는 수량아이템의 수량이 부족함
        ERR_CASE( ERR_SELL_COUNT_ITEM_03,   kPacket.m_nOK, 3 ); // 0개 혹은 음수개를 팔려고 함.
        ERR_CASE( ERR_SELL_COUNT_ITEM_04,   kPacket.m_nOK, 4 ); // 수량 아이템 판매시 해당 유저가 존재하지 않음
        ERR_CASE( ERR_SELL_COUNT_ITEM_05,   kPacket.m_nOK, 5 ); // 수량 아이템 판매시 해당 아이템 정보가 DB에 없음
        ERR_CASE( ERR_INVEN_13,             kPacket.m_nOK, 6 ); // Item 동기화 실패
        ERR_CASE( ERR_DB_COMMON_03,         kPacket.m_nOK, 7 ); // DB와 User Data 동기화 실패
        ERR_CASE( ERR_DB_COMMON_00,         kPacket.m_nOK, 8 ); // 트랜잭션 에러
        ERR_CASE( ERR_SELL_COUNT_ITEM_06,   kPacket.m_nOK, 9 ); // DB 작업 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"수량 아이템 판매 요청시 알수 없는 오류 : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( NetError::GetLastNetError() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_SELL_COUNT_ITEM_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L" 수량 아이템 판매 별과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGP )
        << BUILD_LOG( kPacket.m_kReqItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kReqItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kReqItem.m_nCount )
        << BUILD_LOG( kPacket.m_vecDurationItem.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CURRENT_VIRTUAL_CASH_NOT, int )
{
    if( kPacket_ >= 0 )
        m_nVirtualCashPoint = kPacket_;
    // 잔액이 음수가 나오면 문제가 있다..

    START_LOG( clog, L"가상캐쉬 잔액 , name : " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CURRENT_VIRTUAL_CASH_NOT );
}

IMPL_ON_FUNC( EVENT_BUY_VIRTUAL_CASH_REQ )
{
    _VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_AGIT, KUserFSM::STATE_SQUARE ));

    SET_ERROR( ERR_UNKNOWN );
    KEVENT_BUY_VIRTUAL_CASH_ACK kPacket;
    KDB_EVENT_BUY_VIRTUAL_CASH_REQ kDBPacket;
    START_LOG( clog, L"가상캐쉬로 아이템 구매 합니다. ID:" << m_strName )
        << BUILD_LOG( kPacket_.m_kBuySellItemReq.m_ItemID ) << END_LOG;

    if( SiKVirtualCash()->FindRestrictItem( kPacket_.m_kBuySellItemReq.m_ItemID ) == true && SiKGSSimLayer()->m_bVCMPCantBuy == true ) // 홍콩은 가상캐쉬(혼용포함) 미션,패키지 모두 구매불가
    {
        START_LOG( cerr, L"가상캐쉬 구매시 구매 불가 아이템 검출됨" << m_strName )
            << BUILD_LOG( kPacket_.m_kBuySellItemReq.m_ItemID )
            << END_LOG;

        SET_ERR_GOTO( ERR_BILL_11, END_PROC );
    }

    kDBPacket.m_kBuyInfo = kPacket_.m_kBuySellItemReq;
    FillBuyerData( kDBPacket.m_kBuyInfo );

    kDBPacket.m_dwEventUID = kPacket_.m_kBuySellItemReq.m_dwEventUID;
    kDBPacket.m_bPresent = kPacket_.m_bPresent;
    kDBPacket.m_strComment = kPacket_.m_strComment;
    kDBPacket.m_strOwnerNickName = kPacket_.m_strOwnerNickName;
    kDBPacket.m_strSendNickName = m_strNickName;

    GetDBUpdateData( kDBPacket.m_kUserData );

    // 클라이언트에서 보내준 CharType에 대한 보정 처리.
    if( kDBPacket.m_kBuyInfo.m_cCharType != GetCurrentCharType() ) {
        kDBPacket.m_kBuyInfo.m_cCharType = GetCurrentCharType();
    }

    // 아지트 아이템인 경우 아이템의 캐릭터 타입을 변경한다
    if ( true == SiKAgitHelper()->IsAgitItem( kPacket_.m_kBuySellItemReq.m_ItemID ) ) {
        kDBPacket.m_kBuyInfo.m_cCharType = KItem::EICT_AGIT;
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_VIRTUAL_CASH_REQ, kDBPacket );

    SET_ERROR( NET_OK );

    return;
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_BILL_05,          kPacket.m_nOK, 1 );    // 가격 정보가 존재하지 않음  
        ERR_CASE( ERR_BILL_07,          kPacket.m_nOK, 2 );    // 가상캐쉬 정보가 존재하지 않거나 해당 아이템을 구매할 캐쉬량이 부족함  
        ERR_CASE( ERR_BILL_09,          kPacket.m_nOK, 3 );    // 유저 정보가 존재하지 않음.
        ERR_CASE( ERR_BILL_10,          kPacket.m_nOK, 4 );    // 아이템 목록이 없음.
        ERR_CASE( ERR_BILL_11,          kPacket.m_nOK, 5 );    // 가상캐쉬로 패키지 아이템 구매 불가
        ERR_CASE( ERR_BILL_12,          kPacket.m_nOK, 6 );    // 가상캐쉬 아이템 가격이 잔액을 초과
        ERR_CASE( ERR_BILL_13,          kPacket.m_nOK, 7 );    // 구매 횟수 제한에 걸림.
        ERR_CASE( ERR_BILL_14,          kPacket.m_nOK, 8 );    // 가상캐시로 구매하는 아이템이 아님.
        ERR_CASE( ERR_BILL_01,          kPacket.m_nOK, 9 );    // 아이템 구매 시도 실패.
        ERR_CASE( ERR_BILL_17,          kPacket.m_nOK, 10 );   // 선물받을 닉네임이 비어있음.
        ERR_CASE( ERR_BILL_18,          kPacket.m_nOK, 11 );   // 선물받을 대상이 존재하지 않음.
        ERR_CASE( ERR_BILL_19,          kPacket.m_nOK, 12 );   // 선물받을 대상이 보낼 대상과 동일함.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );  // 처리중.
    default:
        START_LOG( cerr, L"가상캐쉬 구매요청중 알 수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        SET_ERROR( ERR_UNKNOWN );
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_BUY_VIRTUAL_CASH_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BUY_VIRTUAL_CASH_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( kPacket_.m_nOK != 0 )
    {
        goto END_PROC;
    }

    LIF( m_kInventory.AddItemList( kPacket_.m_vecInv ) );

    m_nVirtualCashPoint = kPacket_.m_kVirtualCashInfo.m_dwCashPoint;

    kPacket_.m_bPremium = IsPremiumUser();

    // 아이템 구매 성공 후 소셜커머스 객체에 DB갱신 처리 요청
    if ( true == SiKSocialCommerce()->IsRun() && 0 != kPacket_.m_dwEventUID ) {
        KDB_EVENT_SOSCIAL_COMMERCE_BUY_REQ kPacket;
        if ( true == SiKSocialCommerce()->GetSocialCommerceItemInfoFromCurrentBuyItem( kPacket_.m_dwEventUID, kPacket ) ) {
            SiKSocialCommerce()->SendSellCountUpdateReqToDB( GetUID(), kPacket );
        } // else면 일반 캐시 아이템 구매
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_BILL_05,          kPacket_.m_nOK, 1 );    // 가격 정보가 존재하지 않음  
        ERR_CASE( ERR_BILL_07,          kPacket_.m_nOK, 2 );    // 가상캐쉬 정보가 존재하지 않거나 해당 아이템을 구매할 캐쉬량이 부족함  
        ERR_CASE( ERR_BILL_09,          kPacket_.m_nOK, 3 );    // 유저 정보가 존재하지 않음.
        ERR_CASE( ERR_BILL_10,          kPacket_.m_nOK, 4 );    // 아이템 목록이 없음.
        ERR_CASE( ERR_BILL_11,          kPacket_.m_nOK, 5 );    // 가상캐쉬로 패키지 아이템 구매 불가
        ERR_CASE( ERR_BILL_12,          kPacket_.m_nOK, 6 );    // 가상캐쉬 아이템 가격이 잔액을 초과
        ERR_CASE( ERR_BILL_13,          kPacket_.m_nOK, 7 );    // 구매 횟수 제한에 걸림.
        ERR_CASE( ERR_BILL_14,          kPacket_.m_nOK, 8 );    // 가상캐시로 구매하는 아이템이 아님.
        ERR_CASE( ERR_BILL_01,          kPacket_.m_nOK, 9 );    // 아이템 구매 시도 실패.
        ERR_CASE( ERR_BILL_17,          kPacket_.m_nOK, 10 );   // 선물받을 닉네임이 비어있음.
        ERR_CASE( ERR_BILL_18,          kPacket_.m_nOK, 11 );   // 선물받을 대상이 존재하지 않음.
        ERR_CASE( ERR_BILL_19,          kPacket_.m_nOK, 12 );   // 선물받을 대상이 보낼 대상과 동일함.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );  // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"가상캐쉬 구매중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        SET_ERROR( ERR_UNKNOWN );
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_BUY_VIRTUAL_CASH_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 ) << L"가상 캐쉬 구매 아이템 , Name : " << m_strName
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( kPacket_.m_bPremium ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_COLLECTION_COMPLETE_REQ, int )
{
    // 상태 체크
    _VERIFY_STATE(( 5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL,
        KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE, KUserFSM::STATE_SQUARE_JOIN ));

    SET_ERROR( ERR_UNKNOWN );
    KEVENT_COLLECTION_COMPLETE_ACK kPacket;
    KCollectionData* pkCollectionData = NULL;
    
    KMissionInfo kMissionInfo;
    std::vector<KMissionReward>::iterator vit;

    if( !SiKCollectionManager()->GetMissionInfo( kMissionInfo, kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_TITLE_05, END_PROC ); // 존재하는 콜렉션 미션이 아님
    }
    
    pkCollectionData = GetCollectionDataFromSlot( kPacket_ );
    if( pkCollectionData == NULL )
    {
        SET_ERR_GOTO( ERR_TITLE_07, END_PROC ); // 해당 미션이 슬롯에 존재하지 않음
    }
    //해당 컬렉션 미션의 완료 조건이 되는가?
    if( !SiKCollectionManager()->CheckMissionComplete( *pkCollectionData, m_kInventory ) )
    {
        SET_ERR_GOTO( ERR_TITLE_06, END_PROC );
    }
    
    for( vit = kMissionInfo.m_vecRewardItem.begin(); vit != kMissionInfo.m_vecRewardItem.end() ; ++vit )
    {   // 테스트 해야함 //  080703  woosh. 
        // 보상 아이템이 칭호라면
        // 그리고 칭호 아이템이 이미 가지고 있는 것이라면  미션 완료를 할 수 없다
        if( SiKCollectionManager()->IsTitleItem( vit->m_ItemID ) )
        {
            KItemPtr pkItemInfo;
            pkItemInfo = m_kInventory.FindItemByItemID( vit->m_ItemID );
            if( pkItemInfo != NULL ) // 클라이언트에서는 미션 완료를 요청할 수 없는 상황이지만, 
            {
                kPacket.m_vecInv.push_back( *pkItemInfo );

                START_LOG( cwarn, L"인벤토리에 있는 칭호 아이템을 미션완료 하려고 함")
                    << BUILD_LOG( m_strName )
                    << BUILD_LOG( pkItemInfo->m_ItemID ) << END_LOG;

                SET_ERR_GOTO( ERR_TITLE_09, END_PROC ); // 안타깝게도 요청을 하게 된 경우, 
            }

            // 칭호획득 우선조건의 칭호리스트 받아오기.
            std::vector<GCITEMID> vecPrevCTList;
            std::vector<GCITEMID>::const_iterator cvit;
            SiKCollectionManager()->CheckGetPrevTitle( vit->m_ItemID, vecPrevCTList );
            // 우선 가지고있어야할 칭호아이템을 소지하고 있는지?
            for( cvit = vecPrevCTList.begin() ; cvit != vecPrevCTList.end() ; ++cvit )
            {
                pkItemInfo = m_kInventory.FindItemByItemID( *cvit );
                if( pkItemInfo == NULL )
                {
                    START_LOG( cwarn, L"칭호획득의 우선조건으로 획득해야할 칭호를 가지고 있지 않다.")
                        << BUILD_LOG( m_strName )
                        << BUILD_LOG( vecPrevCTList.size() )
                        << BUILD_LOG( vit->m_ItemID )  //획득하려는놈
                        << BUILD_LOG( *cvit ) << END_LOG;      //필요한데없는놈

                    if ( SiKGSSimLayer()->CheckAuthTypeFlag( KGSSimLayer::AT_FUNBOX_TH ) == true ) {
                        SET_ERR_GOTO( ERR_TITLE_10, END_PROC );
                    }
                    else {
                        SET_ERR_GOTO( ERR_TITLE_06, END_PROC );
                    }
                }
            }

        }
    }

    // DB로 넘김
    QUEUEING_EVENT_TO_DB( EVENT_COLLECTION_COMPLETE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK,       0 ); // 성공
        ERR_CASE( ERR_TITLE_05, kPacket.m_nOK, 5 ); // 스크립트에 미션 정보가 없는 경우
        ERR_CASE( ERR_TITLE_06, kPacket.m_nOK, 6 ); // 컬렉션 미션 완료 조건이 되지 않음
        ERR_CASE( ERR_TITLE_07, kPacket.m_nOK, 7 ); // 해당 컬렉션 미션이 슬롯에 존재하지 않음
        ERR_CASE( ERR_TITLE_09, kPacket.m_nOK, 8 ); // 이미 가지고 있는 칭호
        ERR_CASE( ERR_TITLE_10, kPacket.m_nOK, 9 ); // 칭호획득 선행조건이 완료되지 않음.
        default:
            START_LOG( cerr, L"컬렉션 미션 완료요청시 알 수 없는 오류, Name : " << m_strName )
                << L" Err Str : " << NetError::GetLastNetErrMsg() << END_LOG;
            kPacket.m_nOK = -99;
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << NetError::GetLastNetErrMsg() << L", Name : " << m_strName << dbg::endl
        << BUILD_LOG( kPacket_ ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK ) // 실패시에만 보냄
        SEND_PACKET( EVENT_COLLECTION_COMPLETE_ACK );
}

IMPL_ON_FUNC( EVENT_COLLECTION_COMPLETE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    std::wstring strMsg;
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_TITLE_00, kPacket_.m_nOK, 1 ); // 해당 유저 정보가 없음
        ERR_CASE( ERR_TITLE_01, kPacket_.m_nOK, 2 ); // 칭호 미션 정보가 없거나 완료한 상태임
        ERR_CASE( ERR_TITLE_02, kPacket_.m_nOK, 3 ); // 유저가 존재하지 않음
        ERR_CASE( ERR_TITLE_03, kPacket_.m_nOK, 4 ); // 칭호 보상 아이템이 DB에 존재하지 않음

    default:
        START_LOG( cerr, L"칭호 미션 완료중 알 수 없는 에러 MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        SET_ERROR( ERR_UNKNOWN );
        kPacket_.m_nOK = -99;
    }

    if( NetError::GetLastNetError() == NetError::NET_OK )
    {
        m_kInventory.AddItemList( kPacket_.m_vecInv );

        std::vector<KCollectionData>::iterator vit;
        for( vit = m_vecCollectionSlot.begin(); vit != m_vecCollectionSlot.end(); ++vit )
        {
            if( vit->m_dwMissionID == kPacket_.m_dwMissionID )
                vit->m_dwState = 2; // 완료된 칭호 미션은 상태를 2로 변경
        }

        std::vector<KItem>::iterator vitInv;
        for( vitInv = kPacket_.m_vecInv.begin(); vitInv != kPacket_.m_vecInv.end() ; ++vitInv )
        {   // 칭호 아이템은 기간제라고 가정하고 있음
            if( SiKCollectionManager()->IsTitleItem( vitInv->m_ItemID ) ) // 칭호 중의 하나이고, 메세지 전송 설정이면.
            {
                if( SiKCollectionManager()->GetBroadcastCheck() == false ) continue;

                std::wstring strDungeon;// = L"어떤던전";
                std::wstring strMethod;// = L"미션완료보상";
                strMethod = SiKCollectionManager()->HowGetTitle( L"str_00" ); // 어떻게 얻은 칭호인지,미션클리어
                SiKLoudMessage()->GetItemName( vitInv->m_ItemID, 1, strDungeon, GetLanguageCode() );
                if( SiKLoudMessage()->GetLoudMsg( vitInv->m_ItemID, 0,  m_strNickName, strMsg, strDungeon , strMethod, GetLanguageCode() ) )
                {
                    ChatNotice( strMsg );
                }
            }
        }
    }

    SEND_RECEIVED_PACKET( EVENT_COLLECTION_COMPLETE_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 ) << ", Name :" << m_strName << dbg::endl// 로그 추가 하기 
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwMissionID )
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( SiKCollectionManager()->GetBroadcastCheck() )
        << BUILD_LOG( strMsg ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_JOIN_GAME_REQ )
{
    KEVENT_START_GAME_BROAD kPacket;
    kPacket.m_dwRamdomSeed = 0;
    kPacket.m_dwUniqueNumber = 0;
    _VERIFY_STATE((1, KUserFSM::STATE_ROOM));
    JIF( m_pkRoom );

    if( SiKGSSimLayer()->GetLoadingCheck()  &&  m_pkRoom->IsLoading() )
    {
        SET_ERR_GOTO( ERR_ROOM_55, END_PROC );
    }

    if( m_pkRoom->m_iGameMode != GC_GM_DEATH_TEAM &&
        m_pkRoom->m_iGameMode != GC_GM_DEATH_SURVIVAL )
    {
        SET_ERR_GOTO( ERR_ROOM_53, END_PROC );
    }

    if( m_pkRoom->GetStateID() != KRoomFSM::STATE_PLAYING )
    {
        SET_ERR_GOTO( ERR_ROOM_54, END_PROC );
    }

    if( !m_pkRoom->JoinGameEnable() )
    {
        SET_ERR_GOTO( ERR_ROOM_56, END_PROC );
    }

    kPacket.m_dwRamdomSeed      = m_pkRoom->GetRandomSeed();
    kPacket.m_dwUniqueNumber    = m_pkRoom->GetUniqueNumber();
    m_pkRoom->StartJoinGame( *this );
    kPacket.m_vecStartingUsers.push_back( GetUID() );
    kPacket.m_dwHostUID = m_pkRoom->GetPlayingHostUID();

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ROOM_53,  kPacket.m_nOK, 1 ); // 현재 룸이 데쓰매치가 아님
        ERR_CASE( ERR_ROOM_54,  kPacket.m_nOK, 2 ); // 현재 룸 상태가 게임중이 아님
        ERR_CASE( ERR_ROOM_56,  kPacket.m_nOK, 3 ); // 게임 막바지.
        ERR_CASE( ERR_ROOM_55,  kPacket.m_nOK, 4 ); // 로딩중 막음.
    default:
        START_LOG( cerr, L" Join Game 중에 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK == 0 )
    {
        AddEvents( kPacket );
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_JOIN_GAME_BROAD, kPacket );
    }
    else
    {
        KDeathMatchScore kData;
        m_pkRoom->GetDeathScore( kData );
        kData.m_nOK = kPacket.m_nOK;
        SEND_DATA( EVENT_JOIN_GAME_ACK, kData );
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << L" Join Game 결과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwRamdomSeed )
        << BUILD_LOG( kPacket.m_dwUniqueNumber )
        << BUILD_LOG( kPacket.m_vecStartingUsers.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_P2P_STAT_INFO )
{
    SiKGSSimLayer()->AddP2PStat( kPacket_.m_nPing, kPacket_.m_nPingLossCount );

    START_LOG( clog, L"클라이언트로부터 온 P2P 정보 기록 , Name :" << m_strName )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nPing )
        << BUILD_LOG( kPacket_.m_nPingLossCount ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHECK_OUT_RECORD_UPDATE, unsigned short )
{
    if( kPacket_ == 0 ) return;
    m_kEventCheck.EventOut( kPacket_ );
    START_LOG( clog, L"체크아웃 확인. evnetID : " << kPacket_ << L", LoginID : " << m_strName ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_P2P_UNIQUE_NUM_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_ROOM ) );

    JIF( m_pkRoom );
    m_pkRoom->SetUniqueNumber( SiKGSSimLayer()->GetUniqueNumber() );
    DWORD kPacket = m_pkRoom->GetUniqueNumber();

    m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_P2P_UNIQUE_NUM_BROAD, kPacket );
}

_IMPL_ON_FUNC( EVENT_UPDATE_DEATH_SCORE_NOT, KDeathMatchScore )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_PLAYING ));
    JIF( m_pkRoom );
    JIF( m_pkRoom->GetPlayingHost() == this );
    JIF( m_pkRoom->m_cGameCategory == GC_GMC_DEATHMATCH || m_pkRoom->m_cGameCategory == GC_GMC_FATAL_DEATHMATCH );

    m_pkRoom->SetDeathScore( kPacket_ );
}

_IMPL_ON_FUNC( EVENT_RELAY_DATA, std::vector<DWORD> )
{
    std::vector<DWORD>::iterator vit;
    for( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit )
    {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vit );

        if( spUser == NULL )
        {
            KSerBuffer kbuff;
            kbuff.Clear();
            SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_RELAY_DATA, m_dwUID, kbuff );
            SiKCnConnector()->SendServerRelayPacket( KUserEvent::EVENT_RELAY_DATA, kbuff, GetUID(), L"", (*vit), L"", true, true );
        }
        else
        {
            spUser->SendPacket( KUserEvent::EVENT_RELAY_DATA, m_dwUID );
        }
    }
    //JIF( m_pkRoom );
    //m_pkRoom->BroadPacket( kPacket_, KUserEvent::EVENT_RELAY_DATA, m_dwUID );
    
    //AddRelayLog( kPacket_, m_dwUID );
}

IMPL_ON_FUNC( EVENT_MINIGAME_INFO_NOT )
{
    // 순위 계산은 서버가 가진 데이터로부터 구하기 , 100위권 안에만 가능...
    std::vector<KMiniGameRankInfo>::iterator vit;
    for( vit = kPacket_.m_vecMiniGameInfo.begin(); vit != kPacket_.m_vecMiniGameInfo.end(); ++vit )
    {
        //std::clog << L"미니게임 랭킹 주기, 닉넴 : "<< m_strNickName
        //    << L", 게임모드 : " << static_cast<int>(vit->m_cGameMode)
        //    << L", 캐릭터 타입 :" << static_cast<int>(vit->m_cCharType)
        //    << L", 점수 :" << vit->m_dwBestScore << dbg::endl;

        vit->m_dwUserUID =  m_dwUID; 

        if( SiKMiniGameRankManager()->GetUserRank( vit->m_cGameMode, m_dwUID, *vit ) )
        {
            START_LOG( clog, L"미니 게임 랭킹 Nick : " << vit->m_strNick << ", Rank : " << vit->m_nRank << ", GameMode" << static_cast<int>(vit->m_cGameMode)  ) << END_LOG;
        }
    }
    m_vecMiniGameInfo = kPacket_.m_vecMiniGameInfo;

    SEND_RECEIVED_PACKET( EVENT_MINIGAME_INFO_NOT );

    START_LOG( clog, L"광장 입장시 유저의 미니게임 정보 보내주기, Name: " << m_strName )
        << BUILD_LOG( kPacket_.m_vecMiniGameInfo.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_START_MINIGAME_REQ, char ) // 게임모드를 파라미터로 받고
{
    VERIFY_STATE_LOG( cwarn, ( 1, KUserFSM::STATE_SQUARE ) );

    SET_ERROR( NET_OK );
    int nOK;
    // LeaveSquare 이것과 비슷하지만 다른 컨테이너로 옮기면 됨.
    if( StartMiniGame() == false )  // 상태 변환은 StartMiniGame() 안에 있음, ERR_MINIGAME_03
    {
        goto END_PROC;
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,          nOK, 0 );
        ERR_CASE( ERR_MINIGAME_03, nOK, 1 );
    default:
        nOK = 99;
        SET_ERROR( ERR_UNKNOWN );
        break;
    }

    SEND_DATA( EVENT_START_MINIGAME_ACK, nOK );

    m_dwStartMiniGameDuration = ::GetTickCount();

    START_LOG( clog, L"미니게임 시작 , Name : "  << m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOGc( kPacket_ )
        << BUILD_LOG( nOK ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_END_MINIGAME_REQ, KMiniGameRankInfo )
{
    VERIFY_STATE_LOG( cwarn, ( 1, KUserFSM::STATE_MINIGAME_PLAYING ) );
    
    SET_ERROR( ERR_UNKNOWN );

    kPacket_.m_dwUserUID = GetUID();
    kPacket_.m_strNick = m_strNickName;

    m_dwLastMiniGameDuration = ::GetTickCount() - m_dwStartMiniGameDuration;

    if( kPacket_.m_cCharType < 0 || kPacket_.m_cGameMode < 0 )
    {
        START_LOG( cwarn, L"잘못된 데이터 도착, Name : " << m_strName ) << END_LOG;
        SET_ERR_GOTO( ERR_MINIGAME_06, END_PROC );
    }

    // 해킹으로 의심되는 미니게임 데이터 거르기.
    int nType = SiKPreHackingCheckManager()->CheckMiniGameScore( m_dwLastMiniGameDuration / 1000, kPacket_.m_dwScore );
    if( nType != KPreHackingCheckManager::GHT_NORMAL )
    {
        // DB기록
        KDB_EVENT_DUNGEON_HACKING_USER_NOT kNot;
        kNot.m_nGameModeID  = (int)kPacket_.m_cGameMode;
        kNot.m_nDifficult   = -1;
        kNot.m_nHackingType = nType;
        QUEUEING_EVENT_TO_DB( DB_EVENT_DUNGEON_HACKING_USER_NOT, kNot );

        START_LOG( cwarn, L"해킹으로 의심되는 데이터 도착, Name : " << m_strName ) << END_LOG;
        SET_ERR_GOTO( ERR_MINIGAME_06, END_PROC );
    }

    if( GetMiniGameBestScore( kPacket_ ) )
    {
        START_LOG( clog, L"자신의 최고기록 정보를 클라이언트로.. 최고점수 : " << kPacket_.m_dwBestScore  ) << END_LOG;
    }

    // 랭킹 100위 안에 들게 되었다면 랭킹 정보를 업데이트 한다. 로긴서버와 정보 교환은 정해진 주기를 이용
    SiKMiniGameRankManager()->AddRank( kPacket_.m_cGameMode, kPacket_ ); // 이 안에 더하고, 정렬, 순위 산정, 랭킹 얻어오는 부분까지 포함-_-
    
    // gp 보상
    if( SiKResultManager()->MiniGameGPReward( kPacket_ ) == false )
    {
        //SET_ERROR( ); // 보상중 에러
        //START_LOG( clog, L"미니게임으로 GP보상 받았음" << kPacket_.m_nGP  ) << END_LOG;
    }

    if( SiKMissionManager()->ProcessMiniGame( kPacket_, m_vecMissionSlot, m_strName ) )
    {
        SEND_DATA( EVENT_MISSIONSLOT_NOT, m_vecMissionSlot );
    }

    //  080715  woosh.  통계
    const KCharacterInfo* pkPlayCharacterInfo = GetCharacterInfo( kPacket_.m_cCharType ); // 레벨 구하기 위함
    if( pkPlayCharacterInfo == NULL )
        goto END_PROC;

    if( IsBestMiniGameRecord( kPacket_ ) ) // 자신의 최고 점수를 갱신했다면 DB에 기록, 서버 최고 기록도 갱신
    {
        KEVENT_DB_UPDATE_MINIGAME_SCORE kDBPacket;
        kDBPacket.m_kMiniGameRankInfo = kPacket_;
        kDBPacket.m_dwPlayTime = ( m_dwLastMiniGameDuration / (60 * 1000) ) + 1; // 플레이 시간은 분 단위, 60초이하는 1분으로 취급 ( +1 이유)        
        QUEUEING_EVENT_TO_DB( EVENT_DB_UPDATE_MINIGAME_SCORE, kDBPacket );
        AddStatisticsMiniGame( kPacket_, pkPlayCharacterInfo->m_dwLevel, m_dwLastMiniGameDuration ); // 미니게임 정보와 캐릭터 레벨

        return; // DB로 간 경우는 EVENT_END_MINIGAME_ACK 에서 클라이언트로..
        //QUEUEING_ID_TO_DB( EVENT_MINIGAME_INFO_NOT ); // DB에 의지할까 아니면 서버에 있는 데이터를 신뢰해서 보내줄까, 어쩔까
    }

    AddStatisticsMiniGame( kPacket_, pkPlayCharacterInfo->m_dwLevel, m_dwLastMiniGameDuration ); // 미니게임 정보와 캐릭터 레벨

    SET_ERROR( NET_OK );
    kPacket_.m_nOK = 0;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK         , kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_MINIGAME_04, kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_MINIGAME_06, kPacket_.m_nOK, 2 );
    default:
        SET_ERROR( ERR_UNKNOWN );
        START_LOG( cerr, L"미니게임 한 판 종료 후 알 수 없는 에러  MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket_.m_nOK = 99;
    }

    StateTransition( KUserFSM::INPUT_TO_MINIGAME ); // 미니게임 종료 ( 한판 끝나면..) -> 상태 변환

    SEND_DATA( EVENT_END_MINIGAME_ACK, kPacket_ );

    LOG_SUCCESS( kPacket_.m_nOK == 0 ) 
        << L"미니게임 끝, Name : " << m_strName
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwScore )
        << BUILD_LOG( kPacket_.m_dwBestScore )
        << BUILD_LOGc( kPacket_.m_cGameMode )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_nRank )
        << BUILD_LOG( kPacket_.m_nClearStage )
        << BUILD_LOG( m_dwLastMiniGameDuration )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_END_MINIGAME_ACK, KMiniGameRankInfo )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,          kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_MINIGAME_01, kPacket_.m_nOK, 1 );  // 플레이 한 유저가 없음
        ERR_CASE( ERR_MINIGAME_02, kPacket_.m_nOK, 2 );  // 현재 최고 기록보다 갱신하려는 Point가 작음
    
    default:
        SET_ERROR( ERR_UNKNOWN );
        START_LOG( cerr, L"미니게임 한 판 종료 후 알 수 없는 에러  MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket_.m_nOK = 99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_dwScore )
        << BUILD_LOG( kPacket_.m_dwBestScore )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_END_MINIGAME_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_EXIT_MINIGAME_REQ )
{
    VERIFY_STATE_LOG( cwarn, ( 2, KUserFSM::STATE_MINIGAME, KUserFSM::STATE_MINIGAME_PLAYING ) ); // 미니게임 로비에서 광장으로 

    DWORD dwUserUID = GetUID();
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_EXIT_MINIGAME_NOT, dwUserUID );
    return;
}
 
// 클라이언트가  미니게임 모드를 보내면서, Top 5를 요청하면 보내준다.
_IMPL_ON_FUNC( EVENT_MINIGAME_TOPRANK_REQ, char )
{
    KEVENT_MINIGAME_TOPRANK_ACK kPacket;
    kPacket.m_cGameMode = kPacket_;
    //EVENT_MINIGAME_TOPRANK_ACK 여기에 ok 값을 추가하자 

    kPacket.m_nOK = 0; // 성공
    if( SiKMiniGameRankManager()->GetUserRankRange( kPacket_, 0, 5, kPacket.m_vecRank ) == false ) // 1~5위까지 잘라서 보내주기
    {
        // 실패한다면
        kPacket.m_nOK = 1; // 실패
    }
    
    SEND_DATA( EVENT_MINIGAME_TOPRANK_ACK, kPacket );

    START_LOG( clog, L"미니게임 랭크 탑5 보내주기, Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket_ )
        << BUILD_LOG( kPacket.m_vecRank.size() ) << END_LOG;        
}

_IMPL_ON_FUNC( EVENT_MINIGAME_RESTART_REQ, char ) // 미니게임 계속하기
{
    VERIFY_STATE_LOG( cwarn, ( 1, KUserFSM::STATE_MINIGAME ) ); // 미니게임 로비
    int nOK = 0;
    StateTransition( KUserFSM::INPUT_START_MINIGAME );

    START_LOG( clog, L"미니게임 로비에서 미니게임 다시 시작 , Name : "  << m_strName )
        << BUILD_LOGc( kPacket_ )
        << BUILD_LOG( nOK ) << END_LOG;

    m_dwStartMiniGameDuration = ::GetTickCount();

    SEND_DATA( EVENT_MINIGAME_RESTART_ACK, nOK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_MINIGAME_MY_RANK_INFO_REQ )
{
    KEVENT_MINIGAME_INFO_NOT kPacket;
    std::vector<KMiniGameRankInfo>::iterator vit;
    for( vit = m_vecMiniGameInfo.begin(); vit != m_vecMiniGameInfo.end(); ++vit )
    {
        if( SiKMiniGameRankManager()->GetUserRank( vit->m_cGameMode, m_dwUID, *vit ) )
        {
            START_LOG( clog, L"미니 게임 랭킹 Nick : " << vit->m_strNick << ", Rank : " << vit->m_nRank << ", GameMode" << vit->m_cGameMode  ) << END_LOG;            
        }
    }
    kPacket.m_vecMiniGameInfo = m_vecMiniGameInfo;

    SEND_DATA( EVENT_MINIGAME_MY_RANK_INFO_ACK, kPacket );
    
    START_LOG( clog, L"유저의 미니게임 랭킹 보내주기, Name: " << m_strName )
        << BUILD_LOG( kPacket.m_vecMiniGameInfo.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GET_FULL_SP_INFO_REQ )
{
    KFullSPInfo kPacket;
    GetSkillSetEquip( kPacket.m_mapSetting );
    kPacket.m_mapTrainedSkill   = m_mapTrainedSkill;
    kPacket.m_mapSkillSets      = m_mapSkillSets;
    kPacket.m_mapUnLockGroupID  = m_mapUnLockGroupID;
    kPacket.m_mapSlotOpenInfo   = m_mapSlotOpenInfo;
    SiKSkillManager()->GetSkillMissionInfo( kPacket.m_mapSkillMissionInfo );

    DumpSkillSet( kPacket.m_mapSkillSets );
    SEND_COMPRESS_PACKET( EVENT_GET_FULL_SP_INFO_ACK, kPacket );
}

_IMPL_ON_FUNC( EVENT_SKILL_TRAINING_REQ, int )
{
    VERIFY_STATE( ( 5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL,
        KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE, KUserFSM::STATE_SQUARE_JOIN ) );

    SET_ERROR( ERR_UNKNOWN );

    KOKnValue kPacket;
    kPacket.m_nOK = -99;
    const KCharacterInfo* pkChar = NULL;
    SSkillElement kSkill;
    std::vector<DWORD> vecMissionInfo;
    KChangeUserSkillBroad kChangeUserSkillBroad;
    bool bMissionRewardSkill = false;

    // 이 스킬이 존재 하는가?
    if( !SiKSkillManager()->GetSkill( kPacket_, kSkill ) )
    {
        SET_ERR_GOTO( ERR_SKILL_00, END_PROC );
    }

    if( SiKSkillManager()->IsOldSkillTreeUseCharacter( kSkill.m_cCharType ) == false ) {
        SET_ERR_GOTO( ERR_SKILL_34, END_PROC );
    }

    // Lock Group 오픈이 안되었는가?
    if( kSkill.m_nLockGroupID >= 0 && IsLockedGroup( kSkill ) )
    {
        SET_ERR_GOTO( ERR_SKILL_25, END_PROC );
    }

    // 이미 획득한 스킬인가?
    if( IsTrainedSkill( kSkill ) )
    {
        SET_ERR_GOTO( ERR_SKILL_01, END_PROC );
    }

    // 해당 캐릭터가 존재 하는가?
    pkChar = GetCharacterInfo( kSkill.m_cCharType );
    if( !pkChar )
    {
        SET_ERR_GOTO( ERR_SKILL_03, END_PROC );
    }

    // 미션완료를 해서 획득한 스킬인가?
    if( SiKSkillManager()->CheckMissionRewardSkill( kPacket_, vecMissionInfo ) ) {
        if( !CheckCompleteMission( vecMissionInfo ) ) {
            SET_ERR_GOTO( ERR_SKILL_31, END_PROC );
        }
        bMissionRewardSkill = true;
    } else {
        // 해당 전직의 맥스 스킬 포인트를 넘어 섰는가?
        if( !SiKSkillManager()->CheckExceptionCharType( kSkill.m_cCharType ) ) {
            if( kSkill.m_cPromotion >= 0 &&
                GetSpentSP( kSkill.m_cCharType, kSkill.m_cPromotion ) + kSkill.m_nLearnSP > SiKSkillManager()->GetMaxTrainSP( kSkill.m_cCharType, pkChar->m_cPromotion ) ) {
                SET_ERR_GOTO( ERR_SKILL_20, END_PROC );
            }
        } 
        else {
            if ( GetSpentSP( kSkill.m_cCharType, kSkill.m_cPromotion ) + kSkill.m_nLearnSP > SiKSkillManager()->GetMaxTrainSP( kSkill.m_cCharType, pkChar->m_cPromotion ) ) {
                SET_ERR_GOTO( ERR_SKILL_20, END_PROC );
            }
        }
        // 스킬 포인트는 충분한가?
        if( GetLeftSpPoint( kSkill.m_cCharType ) < kSkill.m_nLearnSP )
        {
            SET_ERR_GOTO( ERR_SKILL_06, END_PROC );
        }
    }

    // 선행 스킬을 완료 하였는가?
    if( !IsTrainedPreSkill( kSkill ) )
    {
        SET_ERR_GOTO( ERR_SKILL_02, END_PROC );
    }

    // 전직은 만족하는가?
    if( pkChar->m_cPromotion < kSkill.m_cPromotion )
    {
        SET_ERR_GOTO( ERR_SKILL_04, END_PROC );
    }

    // 해당 캐릭터 레벨이 만족 하는가?
    if( pkChar->m_dwLevel < (DWORD)kSkill.m_nCharLv )
    {
        SET_ERR_GOTO( ERR_SKILL_05, END_PROC );
    }

    kPacket.m_nValue = kPacket_;
    // 해당 캐릭터 전직의 배운 스킬에 넣어 준다.
    if( !bMissionRewardSkill ) {
    AddTrainedSkill( kSkill );
    } else { // 미션 보상 스킬 처리.
        InsertSkillInfo( kSkill );
        QUEUEING_EVENT_TO_DB( EVENT_SKILL_TRAINING_REQ, kSkill.m_nID );
    }

    // 스킬정보를 방원에게 전송해 줘야 하는지 체크.
    if( SiKSkillManager()->CheckAbilitySkill( kSkill.m_cCharType, kSkill.m_nID ) ) {
        kChangeUserSkillBroad.m_dwUserUID = GetUID();
        GetSkillInfoData( kSkill.m_cCharType, kChangeUserSkillBroad.m_vecTrainedSkill );
        if( m_pkRoom ) {
            m_pkRoom->UpdateSkillInfo( m_strName, kSkill.m_cCharType );

            LIF( m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_SKILL_UPDATE_BROAD, kChangeUserSkillBroad, true ) );
        }
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_00,         kPacket.m_nOK, 1 );    // 존재하지 않는 스킬입니다.
        ERR_CASE( ERR_SKILL_01,         kPacket.m_nOK, 2 );    // 이미 획득한 스킬 입니다.
        ERR_CASE( ERR_SKILL_02,         kPacket.m_nOK, 3 );    // 선행 스킬을 완료하지 않았습니다.
        ERR_CASE( ERR_SKILL_03,         kPacket.m_nOK, 4 );    // 해당 캐릭터가 존재하지 않음.
        ERR_CASE( ERR_SKILL_04,         kPacket.m_nOK, 5 );    // 해당 전직을 만족하지 못함.
        ERR_CASE( ERR_SKILL_05,         kPacket.m_nOK, 6 );    // 레벨을 만족하지 못함.
        ERR_CASE( ERR_SKILL_06,         kPacket.m_nOK, 7 );    // 남은 스킬 포인트가 부족함.
        ERR_CASE( ERR_SKILL_07,         kPacket.m_nOK, 8 );    // 스킬 획득시 DB 처리 완료후 스킬을 찾지 못함.
        ERR_CASE( ERR_SKILL_08,         kPacket.m_nOK, 9 );    // 스킬 획득후 추가에 실패
        ERR_CASE( ERR_SKILL_09,         kPacket.m_nOK, 10 );   // 해당 유저가 존재하지 않음.
        ERR_CASE( ERR_SKILL_10,         kPacket.m_nOK, 11 );   // 데이터 파싱 에러.
        ERR_CASE( ERR_SKILL_11,         kPacket.m_nOK, 12 );   // 유저의 스킬 기본 정보가 존재하지 않음.
        ERR_CASE( ERR_SKILL_12,         kPacket.m_nOK, 13 );   // 이미 보유하고 있는 스킬이 있음.
        ERR_CASE( ERR_SKILL_13,         kPacket.m_nOK, 14 );   // 삭제해야 할 보유하고 있는 스킬이 없음.
        ERR_CASE( ERR_SKILL_20,         kPacket.m_nOK, 15 );   // 더이상 스킬을 배울수 없습니다.
        ERR_CASE( ERR_SKILL_25,         kPacket.m_nOK, 16 );   // Lock이 해제되지 않은 스킬을 배우려고 함.
        ERR_CASE( ERR_SKILL_31,         kPacket.m_nOK, 31 );   // 미션 보상 스킬인데 미션완료하지 못했음.
        ERR_CASE( ERR_SKILL_34,         kPacket.m_nOK, NetError::ERR_SKILL_34 );   // 이전 스킬시스템을 사용하는 캐릭터가 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"스킬 학습시 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_SKILL_TRAINING_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L" 스킬 학습 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nValue )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SET_SKILL_REQ, KChangeSkillSet )
{
    DumpSkillSet( kPacket_.m_mapSkillSets );
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    kPacket_.m_dwUID = GetUID();

    DumpSetSkillReq( kPacket_ );

    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
    for( mit = kPacket_.m_mapSkillSets.begin() ; mit != kPacket_.m_mapSkillSets.end() ; ++mit )
    {
        std::vector<KSPSetInfo>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            if( !VerifySetInfo( mit->first.first, mit->first.second, *vit ) )
            {
                goto END_PROC;
            }
        }
    }

    if( !CheckSettingID( kPacket_.m_mapSetting ) )
    {
        goto END_PROC;
    }

    if( !ApplyChangeSkillSet( kPacket_ ) )
    {
        goto END_PROC;
    }

    if( m_pkRoom )
        m_pkRoom->UpdateSkillSetInfo( m_strName );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK      , kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_14, kPacket_.m_nOK, 1 ); // 세트 변경시 캐릭터. 전직 존재하지 않음.. )
        ERR_CASE( ERR_SKILL_15, kPacket_.m_nOK, 2 ); // 세트 변경시 세트 개수 초과. )
        ERR_CASE( ERR_SKILL_00, kPacket_.m_nOK, 3 ); // 존재하지 않는 스킬입니다.
        ERR_CASE( ERR_SKILL_16, kPacket_.m_nOK, 4 ); // 아직 배우지 않은 스킬 입니다.

    default:
        START_LOG( cerr, L"스킬 장착시 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    MakeChangeSkillSet( kPacket_ );

    if( kPacket_.m_nOK == 0 )
    {
        if( m_pkRoom )
        {
            LIF( m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_SET_SKILL_BROAD, kPacket_, true ) );
        }
        else
        {
            SEND_COMPRESS_PACKET( EVENT_SET_SKILL_BROAD, kPacket_ );
        }
    }
    else
    {
        SEND_COMPRESS_PACKET( EVENT_SET_SKILL_ACK, kPacket_ );
    }
    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << L" 스킬 장착 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_mapSetting.size() )
        << BUILD_LOG( kPacket_.m_mapSkillSets.size() )
        << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_REMOVE_SKILL_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_REMOVE_SKILL_ACK kPacket;
    kPacket.m_nOK = 99;
    kPacket.m_nSkillID = kPacket_.m_nSkillID;
    SSkillElement sSkill;
    KItemPtr pkItem;
    KChangeUserSkillBroad kChangeUserSkillBroad;

    // 보내온 아이템이 제거 아이템인지.
    if( !SiKSkillManager()->IsRemoveSkillItem( kPacket_.m_dwItemID ) )
    {
        SET_ERR_GOTO( ERR_SKILL_26, END_PROC );
    }

    if( !SiKSkillManager()->GetSkill( kPacket_.m_nSkillID, sSkill ) )
    {
        SET_ERR_GOTO( ERR_SKILL_00, END_PROC );
    }

    if( GetDurItemCount( kPacket_.m_dwItemID ) <= 0 )
    {
        SET_ERR_GOTO( ERR_SKILL_19, END_PROC );
    }

    if( ExistTrainedNextSkill( sSkill ) )
    {
        SET_ERR_GOTO( ERR_SKILL_18, END_PROC );
    }

    if( !IsTrainedSkill( sSkill ) )
    {
        SET_ERR_GOTO( ERR_SKILL_16, END_PROC );
    }

    ReleaseEquipSkill( sSkill );
    LIF( RemoveTrainedSkill( sSkill ) );
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_dwItemID );
    if( pkItem )
    {
        if( pkItem->m_nCount > 1 )
        {
            pkItem->m_nCount -= 1;
        }
        else
        {
            pkItem->m_nCount = 0;
        }

        kPacket.m_kItem = *pkItem;
    }

    // 스킬정보를 방원에게 전송해 줘야 하는지 체크.
    if( SiKSkillManager()->CheckAbilitySkill( sSkill.m_cCharType, sSkill.m_nID ) ) {
        kChangeUserSkillBroad.m_dwUserUID = GetUID();
        GetSkillInfoData( sSkill.m_cCharType, kChangeUserSkillBroad.m_vecTrainedSkill );
        if( m_pkRoom ) {
            m_pkRoom->UpdateSkillInfo( m_strName, sSkill.m_cCharType );

            LIF( m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_SKILL_UPDATE_BROAD, kChangeUserSkillBroad, true ) );
        }
    }

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_00, kPacket.m_nOK, 1 );
        ERR_CASE( ERR_SKILL_01, kPacket.m_nOK, 2 );
        ERR_CASE( ERR_SKILL_02, kPacket.m_nOK, 3 );
        ERR_CASE( ERR_SKILL_03, kPacket.m_nOK, 4 );
        ERR_CASE( ERR_SKILL_04, kPacket.m_nOK, 5 );
        ERR_CASE( ERR_SKILL_05, kPacket.m_nOK, 6 );
        ERR_CASE( ERR_SKILL_06, kPacket.m_nOK, 7 );
        ERR_CASE( ERR_SKILL_07, kPacket.m_nOK, 8 );
        ERR_CASE( ERR_SKILL_08, kPacket.m_nOK, 9 );
        ERR_CASE( ERR_SKILL_09, kPacket.m_nOK, 10 );
        ERR_CASE( ERR_SKILL_10, kPacket.m_nOK, 11 );
        ERR_CASE( ERR_SKILL_11, kPacket.m_nOK, 12 );
        ERR_CASE( ERR_SKILL_12, kPacket.m_nOK, 13 );
        ERR_CASE( ERR_SKILL_13, kPacket.m_nOK, 14 );
        ERR_CASE( ERR_SKILL_16, kPacket.m_nOK, 15 );
        ERR_CASE( ERR_SKILL_18, kPacket.m_nOK, 16 );
        ERR_CASE( ERR_SKILL_19, kPacket.m_nOK, 17 );
        ERR_CASE( ERR_SKILL_17, kPacket.m_nOK, 18 );
        ERR_CASE( ERR_SKILL_26, kPacket.m_nOK, 19 );

        ERR_CASE( ERR_NOT_FINISHED_YET,kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"스킬 제거시 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_REMOVE_SKILL_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L"스킬 제거 요청 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nSkillID )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOG( kPacket.m_nSkillID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kItem.m_nCount ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_UNLOCK_SKILL_REQ, int )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_UNLOCK_SKILL_ACK kPacket;

    KDB_EVENT_UNLOCK_SKILL_REQ kDBPacket;
    //PAIR_CHAR_INT CharAndUnlockGroupID;
    kDBPacket.m_CharUnlockGroupID.first = -1;
    kDBPacket.m_CharUnlockGroupID.second = kPacket_;

    KItemPtr pkUnlockItem;

    //kDBPacket.m_CharUnlockGroupID = CharAndUnlockGroupID;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKSkillManager()->IsLockGroupID( kPacket_, kDBPacket.m_CharUnlockGroupID.first ) )
    {
        SET_ERR_GOTO( ERR_SKILL_21, END_PROC );
    }

    if( IsInUnLockedGroup( kDBPacket.m_CharUnlockGroupID.first, kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_SKILL_22, END_PROC );
    }

    pkUnlockItem = m_kInventory.FindItemByItemID( SiKSkillManager()->GetUnLockItemID() );
    if ( !pkUnlockItem || pkUnlockItem->m_nCount < SiKSkillManager()->GetUnLockItemCount() ) {
        SET_ERR_GOTO( ERR_SKILL_23, END_PROC );
    }
    kDBPacket.m_kItem = *pkUnlockItem;

    //if( GetDurItemCount( SiKSkillManager()->GetUnLockItemID() ) < (DWORD)SiKSkillManager()->GetUnLockItemCount() )
    //{
    //    SET_ERR_GOTO( ERR_SKILL_23, END_PROC );
    //}

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_UNLOCK_SKILL_REQ, kDBPacket );
    
    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_21, kPacket.m_nOK, 1 ); // 해당 ID는 LockGroupID가 아닙니다.
        ERR_CASE( ERR_SKILL_22, kPacket.m_nOK, 2 ); // 해당 ID는 이미 UnLock한 Group ID 입니다.
        ERR_CASE( ERR_SKILL_23, kPacket.m_nOK, 3 ); // 해당 Lock Group을 해제하기 위한 아이템이 부족
        ERR_CASE( ERR_SKILL_09, kPacket.m_nOK, 4 ); // 해당 유저 정보가 존재하지 않음
        ERR_CASE( ERR_SKILL_10, kPacket.m_nOK, 5 ); // 데이터 파싱중 에러1
        ERR_CASE( ERR_SKILL_11, kPacket.m_nOK, 6 ); // 해당 유저의 스킬트리 정보가 없음
        ERR_CASE( ERR_SKILL_24, kPacket.m_nOK, 7 ); // 이미 등록된 그룹ID가 있음

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"LockGroup 등록중 알수 없는 오류 : " << m_strName )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EVENT_UNLOCK_SKILL_ACK );

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L" Name : " << m_strName )
        << L" UnLock 요청 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_UNLOCK_SKILL_ACK )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_SKILL_30 ) ) {
        goto END_PROC;
    }

    //kPacket_.m_kItem.m_ItemID = SiKSkillManager()->GetUnLockItemID();
    //kPacket_.m_kItem.m_ItemUID = 0xffffffff;
    //kPacket_.m_kItem.m_nDuration = 0;

    pkItem = m_kInventory.FindItemByItemID( SiKSkillManager()->GetUnLockItemID() );
    if ( pkItem ) {
        //if( pkItem->m_nCount > SiKSkillManager()->GetUnLockItemCount() )
        //{
        //    pkItem->m_nCount -= SiKSkillManager()->GetUnLockItemCount();
        //}
        //else
        //{
        //    pkItem->m_nCount = 0;
        //}
        pkItem->m_nCount = kPacket_.m_kItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kItem.m_nCount;
        kPacket_.m_kItem = *pkItem;
    }

    if ( pkItem && pkItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kItem.m_ItemID, kPacket_.m_kItem.m_ItemUID );
    }

    AddUnLockedGroup( kPacket_.m_cCharType, kPacket_.m_nUnLockGroupID );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,       kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_21, kPacket_.m_nOK, 1 ); // 해당 ID는 LockGroupID가 아닙니다.
        ERR_CASE( ERR_SKILL_22, kPacket_.m_nOK, 2 ); // 해당 ID는 이미 UnLock한 Group ID 입니다.
        ERR_CASE( ERR_SKILL_23, kPacket_.m_nOK, 3 ); // 해당 Lock Group을 해제하기 위한 아이템이 부족
        ERR_CASE( ERR_SKILL_09, kPacket_.m_nOK, 4 ); // 해당 유저 정보가 존재하지 않음
        ERR_CASE( ERR_SKILL_10, kPacket_.m_nOK, 5 ); // 데이터 파싱중 에러1
        ERR_CASE( ERR_SKILL_11, kPacket_.m_nOK, 6 ); // 해당 유저의 스킬트리 정보가 없음
        ERR_CASE( ERR_SKILL_24, kPacket_.m_nOK, 7 ); // 이미 등록된 그룹ID가 있음
        ERR_CASE( ERR_SKILL_30, kPacket_.m_nOK, 8 ); // 스킬 Unlock 아이템 DB 동기화 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG( cerr, L"LockGroup 등록 완료후 알수 없는 오류 : " << m_strName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOGc( kPacket_.m_cCharType)
            << BUILD_LOG( kPacket_.m_nUnLockGroupID )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_UNLOCK_SKILL_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << L"LockGroup 등록 완료후 결과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cCharType)
        << BUILD_LOG( kPacket_.m_nUnLockGroupID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_nCount )
        << BUILD_LOG( NetError::GetLastNetError() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_COUPON_REQ, KComplexItem )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_CHANGE_COUPON_ACK kPacket;
    kPacket.m_kItem = kPacket_;
    kPacket.m_nGP = 0;

    KCouponInfo kCouponInfo;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 해당 아이템이 쿠폰 아이템인지.
    if( !SiKGCHelper()->GetCouponInfo( kPacket_.m_dwID, kCouponInfo ) )
    {
        SET_ERR_GOTO( ERR_COUPON_00, END_PROC );
    }

    kCouponInfo.m_nRewardGP *= kPacket_.m_iDuration;

    // 해당 아이템을 사용 하면 GP 오버 되는지
    if( kCouponInfo.m_nRewardGP + GetGP() > SiKGSSimLayer()->m_iMaxGamePoint )
    {
        SET_ERR_GOTO( ERR_COUPON_01, END_PROC );
    }

    kPacket.m_nGP = kCouponInfo.m_nRewardGP;

    // 해당 아이템이 내 인벤에 존재하는지.
    pkItem = m_kInventory.FindItem( kPacket_.m_dwID, kPacket_.m_dwUID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_COUPON_02, END_PROC );
    }
    if ( pkItem->m_nCount < kPacket_.m_iDuration )
    {
        SET_ERR_GOTO( ERR_COUPON_02, END_PROC );
    }

    kPacket.m_kCouponItem = *pkItem;
    GetDBUpdateData( kPacket.m_kData );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHANGE_COUPON_REQ, kPacket );

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPON_00, kPacket.m_nOK, 1 );  // 해당 아이템은 쿠폰 아이템이 아님.
        ERR_CASE( ERR_COUPON_01, kPacket.m_nOK, 2 );  // 해당 쿠폰을 사용하면 최대 GP 보유량을 초과 합니다.
        ERR_CASE( ERR_COUPON_02, kPacket.m_nOK, 3 );  // 쿠폰 교환시 해당 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_COUPON_03, kPacket.m_nOK, 4 );  // 쿠폰 아이템 제거시 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"쿠폰 사용 요청중 알수 없는 오류 name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_dwID )
            << BUILD_LOG( kPacket_.m_dwUID )
            << BUILD_LOG( kPacket_.m_iDuration ) << END_LOG;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_CHANGE_COUPON_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" 쿠폰 사용 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwID )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_iDuration )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket.m_kItem.m_cItemType ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHANGE_COUPON_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    KItemPtr pkItem;
    bool bDBUpdate = false;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_COUPON_04 ) )
        goto END_PROC;

    pkItem = m_kInventory.FindItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );

    if( pkItem != NULL )
    {
        if( pkItem->m_nCount != KItem::UNLIMITED_ITEM ) {
            pkItem->m_nCount = std::max<int>(0, pkItem->m_nCount - kPacket_.m_kItem.m_iDuration );
            kPacket_.m_vecDurationItem.push_back( *pkItem );
        }
        else
        {
            LIF( m_kInventory.RemoveItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID ) );
            kPacket_.m_vecDelItem.push_back( kPacket_.m_kItem.m_dwUID );
        }
    }

    if( !IS_CORRECT( ERR_COUPON_04 ) ) {
        bDBUpdate = true;
    }
    IncreaseGP( KGpStatInfo::GIT_GP_COUPON, bDBUpdate, kPacket_.m_nGP );

    kPacket_.m_nGP = GetGP();
    kPacket_.m_nOK = NetError::NET_OK;

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_COUPON_00, kPacket_.m_nOK, 1 );  // 해당 아이템은 쿠폰 아이템이 아님.
        ERR_CASE( ERR_COUPON_01, kPacket_.m_nOK, 2 );  // 해당 쿠폰을 사용하면 최대 GP 보유량을 초과 합니다.
        ERR_CASE( ERR_COUPON_02, kPacket_.m_nOK, 3 );  // 쿠폰 교환시 해당 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_COUPON_03, kPacket_.m_nOK, 4 );  // 쿠폰 아이템 제거시 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG( cerr, L"쿠폰 사용 요청중 알수 없는 오류 name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_kItem.m_dwID )
            << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_CHANGE_COUPON_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"쿠폰 교환 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_NEW_YEAR_DONATION_INFO_REQ, DWORD )
{
    KEVENT_NEW_YEAR_DONATION_INFO_ACK kPacket;
    kPacket.Clear();
    DWORD dwMinType = 0;
    DWORD dwMaxType = 0;
    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간인가?
    if ( !SiKNewDonationManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_00, END_PROC );
    }

    // 전송한 Type이 서버에 정의되어 있는 Type인가?
    SiKNewDonationManager()->GetMinMaxType( dwMinType, dwMaxType );
    if( kPacket_ < dwMinType || kPacket_ > dwMaxType ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_05, END_PROC );
    }

    // 해당 NPC Type의 기부 총액 가져오기.
    SiKNewDonationManager()->GetTypeTotalDonation( kPacket_, kPacket.m_nTotalDonation );
    SiKNewDonationManager()->GetDonationItemID( kPacket.m_dwDonationItemID);
    kPacket.m_dwType = kPacket_;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_NEW_YEAR_DONATION_00, kPacket.m_nOK, 1 );    // 이벤트 기간이 아님.
        ERR_CASE( ERR_NEW_YEAR_DONATION_05, kPacket.m_nOK, 6 );    // 서버에 정의된 NPC Type이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"신년 기부 내용 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L" 신년 기부 이벤트 내용 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwType )
        << BUILD_LOG( kPacket.m_nTotalDonation ) << END_LOG;

    SEND_PACKET( EVENT_NEW_YEAR_DONATION_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_NEW_YEAR_DONATION_REQ )
{
    KEVENT_NEW_YEAR_DONATION_ACK kPacket;
    kPacket.Clear();
    KItemPtr pkItem;
    DWORD dwMinType = 0;
    DWORD dwMaxType = 0;
    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간인가?
    if ( !SiKNewDonationManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_00, END_PROC );
    }

    // 기부 ItemID가 동일한가?
    if( !SiKNewDonationManager()->CheckDonationItemID( kPacket_.m_dwItemID ) ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_01, END_PROC );
    }

    // 기부 Item 아이템이 존재 하는가?
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_dwItemID );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_02, END_PROC );
    }

    // 아이템 수량은 옳은가?
    if ( pkItem->m_nCount < kPacket_.m_nItemCount ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_03, END_PROC );
    }

    // 전송한 Type이 서버에 정의되어 있는 Type인가?
    SiKNewDonationManager()->GetMinMaxType( dwMinType, dwMaxType );
    if( kPacket_.m_dwType < dwMinType || kPacket_.m_dwType > dwMaxType ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_05, END_PROC );
    }

    // 기부한 아이템 갯수를 더해주자.
    SiKNewDonationManager()->AddDeltaDonationPoint( kPacket_.m_dwType, kPacket_.m_nItemCount );

    // DB에 큐잉.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_NEW_YEAR_DONATION_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_NEW_YEAR_DONATION_00, kPacket.m_nOK, 1 );    // 이벤트 기간이 아님.
        ERR_CASE( ERR_NEW_YEAR_DONATION_01, kPacket.m_nOK, 2 );    // 서버 클라이언트간 기부 아이템 번호 불일치
        ERR_CASE( ERR_NEW_YEAR_DONATION_02, kPacket.m_nOK, 3 );    // 기부 아이템 가져오기 실패
        ERR_CASE( ERR_NEW_YEAR_DONATION_03, kPacket.m_nOK, 4 );    // 기부 아이템 갯수가 부족하다.
        ERR_CASE( ERR_NEW_YEAR_DONATION_04, kPacket.m_nOK, 5 );    // DB에 기부 정보 넣기 실패.
        ERR_CASE( ERR_NEW_YEAR_DONATION_05, kPacket.m_nOK, 6 );    // 서버에 정의된 NPC Type이 아님.

        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"신년 기부 내용 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L" 신년 기부 이벤트 내용 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwType )
        << BUILD_LOG( kPacket.m_nTotalDonation ) << END_LOG;

    if( !IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_NEW_YEAR_DONATION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_NEW_YEAR_DONATION_ACK )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 기부 Item 아이템이 존재 하는가?
    // 기부한 아이템 갯수만큼 감소시켜주고 클라이언트에게 해당 패킷 전송.
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_dwItemID );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_02, END_PROC );
    }
    pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_nItemCount );
    kPacket_.m_vecDurationItemInv.push_back( *pkItem );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );                      // 성공.
        ERR_CASE( ERR_NEW_YEAR_DONATION_00, kPacket_.m_nOK, 1 );    // 이벤트 기간이 아님.
        ERR_CASE( ERR_NEW_YEAR_DONATION_01, kPacket_.m_nOK, 2 );    // 서버 클라이언트간 기부 아이템 번호 불일치
        ERR_CASE( ERR_NEW_YEAR_DONATION_02, kPacket_.m_nOK, 3 );    // 기부 아이템 가져오기 실패
        ERR_CASE( ERR_NEW_YEAR_DONATION_03, kPacket_.m_nOK, 4 );    // 기부 아이템 갯수가 부족하다.
        ERR_CASE( ERR_NEW_YEAR_DONATION_04, kPacket_.m_nOK, 5 );    // DB에 기부 정보 넣기 실패.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"신년 기부 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_dwType ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"기부 아이템 구매 체크 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwType )
        << BUILD_LOG( kPacket_.m_nTotalDonation ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_NEW_YEAR_DONATION_ACK );
}

IMPL_ON_FUNC( EVENT_RECOMMEND_LEVELUP_NOT )
{
    // 아이템이 존재할 경우에만 클라이언트에게 패킷을 전송하자.
    if( kPacket_.m_nCount > 0 ) {
        m_kInventory.AddItemList( kPacket_.m_vecItem );

        SEND_RECEIVED_PACKET( EVENT_RECOMMEND_LEVELUP_NOT );

        START_LOG( clog, L" 추천받은유저에게 아이템 전송. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_nCount )
            << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;
    }
}