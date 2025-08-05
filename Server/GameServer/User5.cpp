#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "GSSimLayer.h"
#include "Channel.h"
#include "ChannelManager.h"
#include "GameServer.h"
#include "FailRate.h"
#include "MissionInfo.h"
#include "MissionManager.h"
#include "GashBilling.h"
#include "PetHelper.h"
#include "CnConnector.h"
#include <boost/algorithm/string.hpp> // boost::trim
#include "EnchantStrong.h"
#include "SlangFilter.h"
#include "SquareManager.h"
#include "StatisticsManager.h"
#include "StatGameLoading.h"
#include "support_User.h"
#include "GCHelper.h"
#include "LoudMessage.h"
#include "PayLetterBilling.h"
#include "PreHackingCheckManager.h"
#include "TH_PayLetterBilling.h"
#include "IPAdvantage.h"
#include "LogManager.h"
#include "NCashBilling.h"
#include "StrengthManager.h"
#include "FatigueSystem.h"
#include "CYOUBilling/CYOUBilling.h"
#include "SocialCommerce.h"
#include "DungeonRankManager.h"
#include <boost/bind.hpp>
#include "ItemManager.h"

#define CLASS_TYPE  KUser


IMPL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_CONNECTED ));

    KPacketNameOK kPacket;
    kPacket.m_strName   = kPacket_.m_strLogin;
    kPacket.m_nOK       = NetError::ERR_UNKNOWN;

    //서버-클라이언트간 버전 비교
    int iVersionServer = SiKGameServer()->m_nProtocolVer;
    if(kPacket_.m_nProtocolVer != iVersionServer)
    {
        kPacket.m_nOK = NetError::ERR_VERIFY_07; //프로토콜 버전이 다름.

        START_LOG( cwarn, L"프로토콜 버전 다름. ip : " << GetIPStr() ) // 이름이 아직 없으므로 아이피 출력.
            << BUILD_LOG( kPacket_.m_strLogin )
            << BUILD_LOG( kPacket.m_nOK )
            << BUILD_LOG( NetError::GetErrStr( kPacket.m_nOK ) )
            << BUILD_LOG( iVersionServer )
            << BUILD_LOG( kPacket_.m_nProtocolVer )
            << END_LOG;

        SEND_DATA( EVENT_GMTOOL_AUTHENTICATION_ACK, kPacket );
        return;
    }

    SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::EVENT_GMTOOL_AUTHENTICATION_REQ, m_strName.c_str(), 0, 0, kPacket_ );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( kPacket_.m_strPasswd )
        << BUILD_LOG( GetIPStr() ) << END_LOG;

    StateTransition( KUserFSM::INPUT_VERIFICATION_REQ );
}

IMPL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_ACK )
{
    // 이중접속 처리
    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUID ) );
    int nOK;
    if( spUser != NULL  )  // 현재 서버에 똑같은 이름의 접속 유저가 있다
    {
        kPacket_.m_nOK  = NetError::ERR_VERIFY_04;  // 이중 접속 시도

        spUser->ReserveDestroy( DRW_DUPLICATE_CONNECTION );   // 이전 접속자를 종료하는 부분.
    }
    NetError::SetLastNetError( kPacket_.m_nOK );

    nOK = kPacket_.m_nOK;

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        switch( NetError::GetLastNetError() )
        {
        ERR_CASE( ERR_VERIFY_02, kPacket_.m_nOK, 1 );// 이미 존재하는 사용자 이나 패스워드가 다름
        ERR_CASE( ERR_VERIFY_00, kPacket_.m_nOK, 2 );// 존재 하지 않는 사용자
        ERR_CASE( ERR_VERIFY_06, kPacket_.m_nOK, 3 );// 운영자 권한이 없음
        ERR_CASE( ERR_VERIFY_03, kPacket_.m_nOK, 4 );// 운영자 - 닉네임이 등록되어있지 않음
        default:
            kPacket_.m_nOK = -99;      // UNKNOWN ERROR 
        }

        SEND_RECEIVED_PACKET( EVENT_VERIFY_ACCOUNT_ACK );
        ReserveDestroy();
        StateTransition( KUserFSM::INPUT_VERIFICATION_FAIL );

        START_LOG( cwarn, L"인증 실패. Name : " << m_strName )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( kPacket_.m_strLogin )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( NetError::GetErrStr( nOK ) ) << END_LOG;

        return;
    }

    // 2006. 6. 26. florist. 운영툴 접속 계정이라 해도 캐릭터정보가 하나도 없으면 문제를 일으킨다.
    m_mapCharacterInfo = kPacket_.m_mapCharacterInfo;

    m_strNickName   = kPacket_.m_strNickName;
    m_cAuthLevel    = kPacket_.m_cAuthLevel;
    SiKGSSimLayer()->AddNickToLogin( kPacket_.m_strLogin, m_strNickName );
    SetUID( kPacket_.m_dwUID );

    LIF( SiKGSSimLayer()->m_kActorManager.Rename( GetName(), kPacket_.m_strLogin ) );
    LIF( SiKGSSimLayer()->m_kActorManager.RegByUID( *this ) );  // UID 등록.
    UpdateGrade();// 계급 정보를 갱신 한다.

    SEND_RECEIVED_PACKET( EVENT_GMTOOL_AUTHENTICATION_ACK );

    KChannel::ms_spChannelLobby->Enter( *this );

    // 050224 유저 인증 완료 시간 저장
    m_cConnectionTime = CTime::GetCurrentTime();

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOGc( kPacket_.m_cAuthLevel )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUID ) << END_LOG;

    StateTransition( KUserFSM::INPUT_VERIFICATION_OK );
    m_bGMuser = true; // 070504. woosh. GM 툴 접속이라면 true 세팅
}

IMPL_ON_FUNC( EVENT_BUY_FOR_CASH_REQ )
{
    KEVENT_BUY_FOR_CASH_ACK kPacket;

    if( !m_pkCurrentState->CheckValidState( 4, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_AGIT, KUserFSM::STATE_SQUARE ) )
    {
        START_LOG( cerr, L"상태 오류. Name : " << m_strName )
            << L"    현재 상태 : " << GetStateIDString() << dbg::endl
            << L"    허용 상태 : KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL" << END_LOG; \

        kPacket.m_nOK = 2;
        QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_ACK, kPacket, KUserEvent::FT_INNER );
        return;
    }

    // 채널링 적용시 로그인시 클라이언트가 빌링타입을 보내주삼.
    kPacket_.m_cashInfo.m_dwBillingType = SiKGSSimLayer()->GetBillingType();

    if( kPacket_.m_cashInfo.m_dwBillingType != SiKGSSimLayer()->GetBillingType() ||
        SiKGSSimLayer()->GetBillingType() == KGSSimLayer::BT_WEB )
    {

        START_LOG( cerr, L"빌링 타입이 다름" )
            << BUILD_LOG( SiKGSSimLayer()->GetBillingTypeStr() )
            << BUILD_LOG( SiKGSSimLayer()->GetBillingType() )
            << BUILD_LOG( kPacket_.m_cashInfo.m_dwBillingType ) << END_LOG;
    
        kPacket.m_nOK = 1;
        QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_ACK, kPacket, KUserEvent::FT_INNER );
        return;
    }

    if( kPacket_.m_bPresent ) { // 유저 선물일 경우.
        if( !SiKGCHelper()->CheckPresentLimitItem( kPacket_.m_kBuyInfo.m_ItemID ) ) { // 선물 가능 아이템인지 체크.
            kPacket.m_nOK = 7;
            QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_ACK, kPacket, KUserEvent::FT_INNER );
            return;
        }
    }

    kPacket_.m_dwBuyerUID = GetUID(); // 구매자의 UID를 얻어냄.
    kPacket_.m_cashInfo.m_strIP = GetIPStr(); // 구매자의 IP를 얻어냄 ( for GASH )
    kPacket_.m_kBuyInfo.m_cCharType = m_cCurrentCharacter; // 현재 플레이중인 캐릭터 정보 넣어주기.

    // 아지트 아이템인 경우 아이템의 캐릭터 타입을 변경한다
    if ( true == SiKAgitHelper()->IsObjectItem( kPacket_.m_kBuyInfo.m_ItemID ) ) {
        kPacket_.m_kBuyInfo.m_cCharType = KItem::EICT_AGIT;
    }

    switch( SiKGSSimLayer()->GetBillingType() )
    {
    case KGSSimLayer::BT_GASH:
        KTThreadManager<KGashBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket_ );
        _EVENT_CHECK_IN( EVENT_BUY_FOR_CASH_REQ );
        break;
    case KGSSimLayer::BT_BR_PAYLETTER:
        KTThreadManager<KBR_PayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket_ );
        _EVENT_CHECK_IN( EVENT_BUY_FOR_CASH_REQ );
        break;
    case KGSSimLayer::BT_PAYLETTER:
        KTThreadManager<KPayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket_ );
        _EVENT_CHECK_IN( EVENT_BUY_FOR_CASH_REQ );
        break;
    case KGSSimLayer::BT_TH_PAYLETTER:
        KTThreadManager<KTH_PayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket_ );
        _EVENT_CHECK_IN( EVENT_BUY_FOR_CASH_REQ );
        break;
    case KGSSimLayer::BT_ID_PAYLETTER:
        KTThreadManager<KID_PayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket_ );
        _EVENT_CHECK_IN( EVENT_BUY_FOR_CASH_REQ );
        break;
    case KGSSimLayer::BT_LA_NCASH:
        KTThreadManager<KNCashBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket_ );
        _EVENT_CHECK_IN( EVENT_BUY_FOR_CASH_REQ );
        break;
    case KGSSimLayer::BT_CN_CYOU:
        //KTThreadManager<CYOUBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_BUY_FOR_CASH_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket_ );
        //_EVENT_CHECK_IN( EVENT_BUY_FOR_CASH_REQ );
		throw new std::exception("BT_CN_CYOU not supported (PCI.lib can't link)");
        break;

    default:
        START_LOG( cerr, L"이벤트를 던질 빌링 클래스가 없음." )
            << BUILD_LOG( SiKGSSimLayer()->GetBillingType() )
            << END_LOG;
        break;
    }

    START_LOG( clog, L"캐쉬 아이템 구매 요청 Name : " << GetName() )
        << BUILD_LOG( kPacket_.m_bPresent )
        << BUILD_LOG( kPacket_.m_strOwnerNickName ) // 선물받는 사람 닉네임
        << BUILD_LOG( kPacket_.m_strComment )
        << BUILD_LOG( kPacket_.m_kBuyInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_kBuyInfo.m_nCount )
        << BUILD_LOG( kPacket_.m_kBuyInfo.m_nPeriod ) << END_LOG;

    return;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_BUY_FOR_CASH_ACK );
}

// 111 ( 상품 정보가 없거나 게임DB와 빌링 DB의 가격 정보가 틀림 )
// 110 ( 캐쉬 알수 없는 오류 )
// 103 ( 캐쉬 차감 실패 )
// 102 (캐쉬 포인트 얻기 실패 )
// 101 (빌링 시스템 오류)
// 100 ( 빌링 시스템 과부하 )
// 8 ( 판매가 종료 된 소셜 커머스 아이템 )
// 7 ( 선물하기 불가능 아이템 )
// 6 ( 가상 캐시 사용 기간이 지났음 )
// 5 ( 원어데이 한정 판매 품절 )
// 4 ( 인벤토리 아이템 갱신 실패. 서버 이동이 필요 )
// 3 ( 더이상 구매할 수 없음 )
// 2 ( 허용되는 상태가 아님 )
// 1 (빌링 타입이 올바르지 않음)
//-1(OwnerLogin 없음)
//-2(BuyerLogin 없음, BuyerCheck가 1일때 작동)
//-3(입력받은 아이템이 캐쉬가 아니거나 비매품이거나 아예 없거나 패턴임)
//-4(IPBP에 가격 정보가 존재하지 않음)
//-5(입력할 아이템이 없음)
//-6(Gash가 부족함, 보유Gash량에 0보다 큰값이 있을 경우 작동)
//-7 ( 가상캐쉬와 혼합사용시 패키지 아이템 or 전직미션 구매 시도 )
// -10 ( 쿼리 실패 )
// -11이하(트랜잭션 에러)
IMPL_ON_FUNC( EVENT_BUY_FOR_CASH_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    if( kPacket_.m_nOK != 0 && kPacket_.m_nOK != 4 )
    {
        LOG_CONDITION( kPacket_.m_nOK == 2 || kPacket_.m_nOK == 1 ||
            kPacket_.m_nOK == -1 || kPacket_.m_nOK == -6 ||
            kPacket_.m_nOK == 104 || kPacket_.m_nOK == -7 || kPacket_.m_nOK == 3 || kPacket_.m_nOK == 111, cwarn, cerr )
            << L"캐쉬 아이템 구매 실패 Name : " << m_strName << dbg::endl
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
        SEND_RECEIVED_PACKET( EVENT_BUY_FOR_CASH_ACK );
        return;
    }

    m_kInventory.AddItemList( kPacket_.m_vecItem );
    // 가상캐쉬 잔액 
    m_nVirtualCashPoint = kPacket_.m_kVirtualCashInfo.m_dwCashPoint;
    // 프리미엄 유저인가(GC클럽 보유자인가?)
    kPacket_.m_bPremium = IsPremiumUser();

    //if ( kPacket_.m_bOneaday == true ) {
    //    SiKOneaday()->UpdateLimitedCount( kPacket_.m_ItemID );
    //}
    if ( SiKGSSimLayer()->GetBillingType() == KGSSimLayer::BT_CN_CYOU ) {
        CYOUSendBuyLog( kPacket_ );
    }

    // 아이템 구매 성공 후 소셜커머스 객체에 DB갱신 처리 요청
    if ( true == SiKSocialCommerce()->IsRun() && 0 != kPacket_.m_dwEventUID ) {
        KDB_EVENT_SOSCIAL_COMMERCE_BUY_REQ kPacket;
        if ( true == SiKSocialCommerce()->GetSocialCommerceItemInfoFromCurrentBuyItem( kPacket_.m_dwEventUID, kPacket ) ) {
            SiKSocialCommerce()->SendSellCountUpdateReqToDB( GetUID(), kPacket );
        } // else면 일반 캐시 아이템 구매
    }

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_CASH_ACK );

    START_LOG( clog, L"캐쉬 구매 아이템 " )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kPacket_.m_bPremium )
        << BUILD_LOG( kPacket_.m_cashInfo.m_dwCashPoint )
        << BUILD_LOG( kPacket_.m_kVirtualCashInfo.m_dwCashPoint )
        << END_LOG;

    SendVipEventUserRewardUpdateNot();
}

IMPL_ON_FUNC_NOPARAM( EVENT_CURRENT_CASH_POINT_REQ )
{
    KCashInfo kPacket;
    if( SiKGSSimLayer()->GetBillingType() == KGSSimLayer::BT_WEB )
    {
        START_LOG( cwarn, L"빌링 타입이 다름 : " << m_strName )
            << BUILD_LOG( SiKGSSimLayer()->GetBillingTypeStr() )
            << BUILD_LOG( SiKGSSimLayer()->GetBillingType() ) << END_LOG;

        kPacket.m_nOK = 1;
        kPacket.m_dwCashPoint = 0;
        SEND_PACKET( EVENT_CURRENT_CASH_POINT_ACK );
        return;
    }

    kPacket.m_strIP = GetIPStr();
    switch( SiKGSSimLayer()->GetBillingType() )
    {
    case KGSSimLayer::BT_GASH:
        KTThreadManager<KGashBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket );
        break;
    case KGSSimLayer::BT_BR_PAYLETTER:
        KTThreadManager<KBR_PayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket );
        break;
    case KGSSimLayer::BT_PAYLETTER:
        KTThreadManager<KPayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket );
        break;
    case KGSSimLayer::BT_TH_PAYLETTER:
        KTThreadManager<KTH_PayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket );
        break;
    case KGSSimLayer::BT_ID_PAYLETTER:
        KTThreadManager<KID_PayLetterBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket );
        break;
    case KGSSimLayer::BT_LA_NCASH:
        KTThreadManager<KNCashBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket );
        break;
    case KGSSimLayer::BT_CN_CYOU:
        //KTThreadManager<CYOUBilling>::GetInstance()->QueueingEvent( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, kPacket );
		throw new std::exception("BT_CN_CYOU not supported (PCI.lib can't link)");
        break;

    default:
        START_LOG( cerr, L"이벤트를 던질 빌링 클래스가 없음." )
            << BUILD_LOG( SiKGSSimLayer()->GetBillingType() )
            << END_LOG;
        break;
    }
}

// 110 ( 캐쉬 알수 없는 오류 )
// 104 ( 게쉬에서는 체험계정 ) 오류아님.
// 103 ( 캐쉬 차감 실패 )
// 102 (캐쉬 포인트 얻기 실패 )
// 101 (빌링 시스템 오류)
// 100 ( 빌링 시스템 과부하 )
// 1 (빌링 타입이 올바르지 않음)
// PayLetter , 100,200,300,400,500
_IMPL_ON_FUNC( EVENT_CURRENT_CASH_POINT_ACK, KCashInfo )
{
    SEND_RECEIVED_PACKET( EVENT_CURRENT_CASH_POINT_ACK );
    if( kPacket_.m_nOK == 0 )
    {
        START_LOG( clog, L"케쉬 포인트 얻기 성공 : " << m_strName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kPacket_.m_dwCashPoint ) << END_LOG;
        return;
    }

    // 104 게쉬에서의 체험계정
    LOG_CONDITION( kPacket_.m_nOK != 104, cerr, cwarn )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwCashPoint ) << END_LOG;

    //PayLetter Err면 Login 기록.
    //if( SiKGSSimLayer()->GetBillingType() == KGSSimLayer::BT_PAYLETTER )
    //{
    //    if( kPacket_.m_nOK == KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_01 ||
    //        kPacket_.m_nOK == KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_02 ||
    //        kPacket_.m_nOK == KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_03 ||
    //        kPacket_.m_nOK == KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_04 ||
    //        kPacket_.m_nOK == KPayLetterEvent::EVENT_PAYLETTER_ERROR_NOT_05 )
    //    {
    //        SiKGSSimLayer()->AddPayLetterErrStat( kPacket_.m_nOK, m_strName );
    //    }
    //}
}

IMPL_ON_FUNC_NOPARAM( EVENT_ENCHANT_GEMCOUNT_REQ )
{
    std::map<DWORD,int> mapMaterialCount;
    SiKEnchantStrong()->MaterialCountList( mapMaterialCount );
    SEND_DATA( EVENT_ENCHANT_GEMCOUNT_ACK, mapMaterialCount );
}

IMPL_ON_FUNC( DB_EVENT_CREATE_INDIGO_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_bIndigoDataExist )
        << BUILD_LOGc( kPacket_.m_kData.m_cOK )
        << BUILD_LOGc( kPacket_.m_kData.m_iWin )
        << BUILD_LOGc( kPacket_.m_kData.m_iLose )
        << BUILD_LOGc( kPacket_.m_kData.m_iRP )
        << BUILD_LOGc( kPacket_.m_kData.m_iRank )
        << BUILD_LOG( GetStateIDString() )
        << BUILD_LOG( m_iIndigoWin )
        << BUILD_LOG( m_iIndigoLose )
        << BUILD_LOG( m_iRP ) << END_LOG;

    if( !SetIndigoInfo( kPacket_.m_kData ) )
    {
        int kPacket = -1;
        SEND_PACKET( EVENT_ENTER_CHANNEL_ACK );
        return;
    }
    else
    {
        m_bIndigoDataExist = true;
    }

    QueueingEvent( KUserEvent::EVENT_ENTER_CHANNEL_REQ, kPacket_.m_dwChannelUID );
}

_IMPL_ON_FUNC( DB_EVENT_UPDATE_INDIGO_ACK, KIndigoData )
{
    LIF( SetIndigoInfo( kPacket_ ) );

    if( kPacket_.m_cOK != 0 )
    {
        const KCharacterInfo* pkCharInfo = GetBestCharacter();

        JIF( pkCharInfo );

        START_LOG_WITH_NAME( cerr )
            << BUILD_LOGc( pkCharInfo->m_cCharType )
            << BUILD_LOGc( pkCharInfo->m_cPromotion )
            << BUILD_LOG( pkCharInfo->m_biExp )
            << BUILD_LOG( pkCharInfo->m_dwLevel )
            << BUILD_LOG( CheckIfAllItemExist( KUser::INDIGO_TICKET_ID, -1 ) )
            << BUILD_LOG( m_bIndigoDataExist ) << END_LOG;
    }
}

_IMPL_ON_FUNC( EVENT_SET_HBCHECK_FLAG, bool ) 
{
    if( !SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( m_bCheckHBTick )
            << END_LOG;
        return;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_bCheckHBTick )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    m_bCheckHBTick = kPacket_;
}

_IMPL_ON_FUNC(EVENT_CHANGE_ROOMUSER_INFO_REQ, KChangeRoomUserInfo)
{
    kPacket_.m_nOK = -100;
    kPacket_.m_dwUserUID = GetUID();

    if (GetStateID() != KUserFSM::STATE_ROOM)
    {
        START_LOG(cwarn, L"EVENT_CHANGE_ROOMUSER_INFO_REQ Warning! : " << GetName()) << END_LOG;
        kPacket_.m_nOK = -1;
        SEND_RECEIVED_PACKET(EVENT_CHANGE_ROOMUSER_INFO_BROAD);
        return;
    }

    _VERIFY_STATE((1, KUserFSM::STATE_ROOM));
    JIF(m_pkRoom);

    m_pkRoom->ChangeRoomUserInfo(kPacket_);

    if (kPacket_.m_nOK == 0)
    {
        m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_CHANGE_ROOMUSER_INFO_BROAD, kPacket_, false);
    }

    SEND_RECEIVED_PACKET(EVENT_CHANGE_ROOMUSER_INFO_BROAD);
}

IMPL_ON_FUNC( EVENT_LOAD_COMPLETE_NOT )
{
    _VERIFY_STATE(( 1,KUserFSM::STATE_PLAYING ));

    // 로딩 완료 패킷을 받으면 시간 초기화를 한번 하겠다.
    m_kPingInfo.UpdateLastTick();

    if( GetLoadingComplete() ) // 이미 로딩 실패로 방에서 나간 상태. ( 관련 카운트를 올리지 않는다. )
        return;

    KStatGameLoadingPtr spStat = GET_STAT( KStatGameLoading, SI_STAT_GAMELOADING );
    if( spStat )
    {
        spStat->AddStat( ::GetTickCount() - GetTick( GAME_START_TICK ) );
    }
    m_bRecvLoadingComplete = true;
    SiKFailRate()->IncreaseCount( KFailRate::P2P_USER, kPacket_.m_iP2PUser );
    SiKFailRate()->IncreaseCount( KFailRate::RELAY_USER, kPacket_.m_iRelayUser );

    JIF( m_pkRoom );
    m_pkRoom->CheckLoadingCompleteUser();
}

IMPL_ON_FUNC( EVENT_SHANGHAI_DROP_REQ )
{

    if( !kPacket_.m_vecAutoMission.empty() ) // 요청한 미션에 대한 유효성 검사.
    {
        std::vector<DWORD>::iterator vit;
        vit = kPacket_.m_vecAutoMission.begin();

        while( vit != kPacket_.m_vecAutoMission.end() ) 
        {
            if( IsRegistedMission( *vit ) || SiKMissionManager()->IsInInvalidMission( m_vecMissionSlot, *vit ) )
            {
                vit = kPacket_.m_vecAutoMission.erase( vit );
                continue;
            }

            KMissionInfo kMissionInfo;
            if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, *vit  ) )
            {
                vit = kPacket_.m_vecAutoMission.erase( vit );
                continue;
            }

            if( kMissionInfo.m_cCharType >= 0 )
            {
                // 캐릭터 정보 확인
                const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( kMissionInfo.m_cCharType );
                if( !pkCharacterInfo )
                {
                    vit = kPacket_.m_vecAutoMission.erase( vit );
                    continue;
                }

                //해당 캐릭의 전직 상태
                if( kMissionInfo.m_cJobLv != -1 && pkCharacterInfo->m_cPromotion < kMissionInfo.m_cJobLv )
                {
                    vit = kPacket_.m_vecAutoMission.erase( vit );
                    continue;
                }

                // 제한 레벨 이상인지 확인
                if( kMissionInfo.m_prLevelRage.first >= 0 &&
                    (int)pkCharacterInfo->m_dwLevel < kMissionInfo.m_prLevelRage.first )
                {
                    vit = kPacket_.m_vecAutoMission.erase( vit );
                    continue;
                }

                if( kMissionInfo.m_prLevelRage.second >= 0 &&
                    (int)pkCharacterInfo->m_dwLevel > kMissionInfo.m_prLevelRage.second )
                {
                    vit = kPacket_.m_vecAutoMission.erase( vit );
                    continue;
                }

                // 전직이면.
                if( kMissionInfo.m_cChangeJob > 0 && pkCharacterInfo->m_cPromotion >= kMissionInfo.m_cChangeJob )
                {
                    vit = kPacket_.m_vecAutoMission.erase( vit );
                    continue;
                }
            }

            if( kMissionInfo.m_cNewChar > 0 && IsCharExist( kMissionInfo.m_cNewChar ) )
            {
                vit = kPacket_.m_vecAutoMission.erase( vit );
            }

            ++vit;
        }
    }

    if( !kPacket_.m_vecDropItems.empty() || !kPacket_.m_vecDropPostItems.empty() || !kPacket_.m_vecAutoMission.empty() )
    {
        DumpAutoMission( std::wstring(L"EVENT_SHANGHAI_DROP_REQ"), kPacket_.m_vecAutoMission );
        QUEUEING_EVENT_TO_DB( EVENT_SHANGHAI_DROP_REQ, kPacket_ );
    }

}

IMPL_ON_FUNC( EVENT_SHANGHAI_DROP_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    m_kInventory.AddItemList( kPacket_.m_vecInv );
    m_vecMissionSlot.insert( m_vecMissionSlot.end(), kPacket_.m_vecAutoMission.begin(), kPacket_.m_vecAutoMission.end() );
    SEND_RECEIVED_PACKET( EVENT_SHANGHAI_DROP_ACK );

    //  우편 갱신 요청
    if ( kPacket_.m_bPostItem ) 
    {
//        QUEUEING_ID_TO_DB_CHECK( EVENT_NEW_POST_LETTER_INFO_REQ );
        START_LOG( clog, L"새 우편 리스트 DB에 요청.. LoginID : " << GetName() << L", UserUID : " << GetUID() ) << END_LOG;
    }

    START_LOG( clog, L" 상하이 드랍")
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( kPacket_.m_bPostItem )
        << BUILD_LOG( kPacket_.m_vecAutoMission.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SHANGHAI_CHECK_ACK, KncBitStream )
{
    m_bitCheckPoint = kPacket_;
}

_IMPL_ON_FUNC( EVENT_CHANGE_INDOOR_USERINFO_REQ, KInDoorUserInfo )
{
    _VERIFY_STATE(( 1,KUserFSM::STATE_ROOM));
    JIF( m_pkRoom );

    const KInDoorUserInfo* pKInDoorUserInfo = m_pkRoom->FindInDoorUserInfo( m_strName );
    JIF( pKInDoorUserInfo );

    kPacket_.m_bIsObserver  = pKInDoorUserInfo->m_bIsObserver;
    kPacket_.m_iPlayerIndex = pKInDoorUserInfo->m_iPlayerIndex;

    DumpIndoorUserInfo( kPacket_ );
    JIF( m_pkRoom->AssignInDoorUserInfo( kPacket_ ) );
    m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_CHANGE_INDOOR_USERINFO_BROAD, kPacket_, true );
}

_IMPL_ON_FUNC( EVENT_RECOMMENDER_REQ, KPacketNameOK )
{
    _VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    if( SiKGSSimLayer()->m_bRecommendEnable == false )
    {
        int kPacket;
        kPacket = 3;
        SEND_PACKET( EVENT_RECOMMENDER_ACK );
        return;
    }

    if( kPacket_.m_nOK != -1 && kPacket_.m_strName.empty() )
    {
        int kPacket;
        kPacket = 2;
        SEND_PACKET( EVENT_RECOMMENDER_ACK );
        return;
    }

    QUEUEING_EVENT_TO_DB( EVENT_RECOMMENDER_REQ, kPacket_ );

    START_LOG( clog, L"추천인 요청" )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_strName ) // 추천한 유저
        << BUILD_LOG( kPacket_.m_strName ) << END_LOG; // 추천 받는 사람
}

// 3  추천인 시스템 없음
// 2  추천인 닉네임이 비어 있음
// 1  비추하였음
// 0  정상적으로 추천인이 등록 되었음
// -1 추천하려는 사람의 정보가 없음 --> 이녀석인 경우 심각한 문제이다. 넘어가시오.
// -2 이미 추천하였습니다.(이중접속등으로 인해서 다른넘이 먼저 등록했다. 일수 있음)
// -3 다시 추천하세욤.(대상이 존재하지 않음)

_IMPL_ON_FUNC( EVENT_RECOMMENDER_ACK, int )
{
    SEND_RECEIVED_PACKET( EVENT_RECOMMENDER_ACK );
    // 0:추천인 미등록, 1:추천인 등록했음, 2:추천인 없음, 3:추천인 시스템없음, 4: 그이외
    if( kPacket_ == 0 )
        m_cRecommendUser = 1;
    else if( kPacket_ == 1 )
        m_cRecommendUser = 2;

    //-2에 대해서는 처리하지 않음 해당 서버에서 처리할테니까

    LOG_CONDITION( kPacket_ == 0 || kPacket_ == 1, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHECK_ROOM_USERLIST_NOT )
{
    //이녀석은 방에서 게임 시작할때 P2P에서 얻어낸 유저의 UID 리스트를 보내온다.
    if( GetStateID() != KUserFSM::STATE_PLAYING || m_pkRoom == NULL )
    {
        SiKFailRate()->IncreaseCount( KFailRate::INACCURATE_PLAY_STATE );
        return;
    }

    if( m_pkRoom->GetCount() != kPacket_.m_vecP2PUser.size() )
    {
        SiKFailRate()->IncreaseCount( KFailRate::INACCURATE_P2P_USERNUM );
        return;
    }

    if( m_pkRoom->GetCount() != kPacket_.m_vecSlotUser.size() )
    {
        SiKFailRate()->IncreaseCount( KFailRate::INACCURATE_SLOT_USERNUM );
        return;
    }

    std::vector<DWORD> vecRoomList;
    m_pkRoom->GetUserUIDList( vecRoomList );

    std::vector<DWORD>::iterator vitp2p;

    for( vitp2p = kPacket_.m_vecP2PUser.begin() ; vitp2p != kPacket_.m_vecP2PUser.end() ; ++vitp2p )
    {
        if( std::find( vecRoomList.begin(), vecRoomList.end(), *vitp2p ) == vecRoomList.end() )
        {
            SiKFailRate()->IncreaseCount( KFailRate::DIFFERENT_P2P_PLAY_USERLIST );
            return;
        }
    }

    std::vector<DWORD>::iterator vitslot;
    for( vitslot = kPacket_.m_vecSlotUser.begin() ; vitslot != kPacket_.m_vecSlotUser.end() ; ++vitslot )
    {
        if( std::find( vecRoomList.begin(), vecRoomList.end(), *vitslot ) == vecRoomList.end() )
        {
            SiKFailRate()->IncreaseCount( KFailRate::DIFFERENT_SLOT_PLAY_USERLIST );
            return;
        }
    }
}

IMPL_ON_FUNC( EVENT_ACCUMULRATION_TIME_NOT )
{
    //m_nAccOfflineTime   = kPacket_.m_nAccOfflineTime;
    //m_nAccOnlineTime    = kPacket_.m_nAccOnlineTime;

    SiKFatigueSystem()->GetInfo( kPacket_.m_mapFatigueTable, kPacket_.m_nMaxOnlineTime, kPacket_.m_nResetOfflineTime );
    
    SEND_RECEIVED_PACKET( EVENT_ACCUMULRATION_TIME_NOT );
}

_IMPL_ON_FUNC( EVENT_SET_CURRENT_CHARACTER_REQ, char )
{
    _VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    int kPacket = -99; //오류
    if( !IsCharExist( kPacket_ ) ) {
        //오류 없는 캐릭
        START_LOG( cerr, L"존재하지 않는 캐릭터를 선택하려 했습니다." )
            << BUILD_LOG( GetName() )
            << BUILD_LOGc( kPacket_ ) << END_LOG;

        kPacket = -1;
        SEND_PACKET( EVENT_SET_CURRENT_CHARACTER_ACK );
        return;
    }

    kPacket = 0;
    SEND_PACKET( EVENT_SET_CURRENT_CHARACTER_ACK );
    START_LOG( clog, L"캐릭터 변경 " << GetName() << L" : " << static_cast<int>(kPacket_) ) << END_LOG;
}

_IMPL_ON_FUNC(EVENT_KAIRO_REQ, GCITEMUID)
{
    _VERIFY_STATE((3, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY));
    KDB_EVENT_KAIRO_REQ kDBPacket;
    KEVENT_KAIRO_ACK kPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkItem;
    DWORD dwGrade = 0;
    DWORD dwEquipType = 0;
    std::vector<KDropItemInfo>::iterator vit;
    SET_ERROR(ERR_UNKNOWN);

    pkItem = m_kInventory.FindDurationItemByItemUID(kPacket_);
    if (pkItem == NULL) {
        SET_ERR_GOTO(ERR_KAIRO_00, END_PROC);
    }

    if (pkItem->m_nCount <= 0) {
        SET_ERR_GOTO(ERR_KAIRO_00, END_PROC);
    }

    if (!SiKGSSimLayer()->CheckKairo(pkItem->m_ItemID, kDBPacket.m_vecPostItem, kDBPacket.m_dwType)) {
        kDBPacket.m_kReqItemInfo = *pkItem;
        SET_ERR_GOTO(ERR_KAIRO_02, END_PROC);
    }

    if (kDBPacket.m_vecPostItem.empty()) {
        --pkItem->m_nCount;
        kPacket.m_kReqItemInfo = *pkItem;
        SET_ERR_GOTO(ERR_KAIRO_01, END_PROC);
    }

    for (vit = kDBPacket.m_vecPostItem.begin(); vit != kDBPacket.m_vecPostItem.end(); ++vit) {
        if (kDBPacket.m_dwType == Kairo::RT_STRENGTH) {
            if (vit->m_ItemID == STRENGTH_ID) {
                dwGrade = vit->m_cGradeID;
            }
            else {
                dwEquipType = vit->m_cEquipType;
            }
        }
    }

    if (kDBPacket.m_dwType == Kairo::RT_STRENGTH) {
        if (dwGrade > KItem::GRADE_NORMAL) {
            if (!SiKStrengthManager()->GetStrengthAttributeInfo(dwGrade, dwEquipType, kDBPacket.m_listAttributeList)) {
                SET_ERR_GOTO(ERR_KAIRO_05, END_PROC);
            }
        }
    }

    kDBPacket.m_kReqItemInfo = *pkItem;

    QUEUEING_EVENT_TO_DB_CHECK(DB_EVENT_KAIRO_REQ, kDBPacket);
    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_KAIRO_00, kPacket.m_nOK, 1);
        ERR_CASE(ERR_KAIRO_01, kPacket.m_nOK, 2);
        ERR_CASE(ERR_KAIRO_02, kPacket.m_nOK, 3);
        ERR_CASE(ERR_KAIRO_05, kPacket.m_nOK, 6);
        ERR_CASE(ERR_KAIRO_06, kPacket.m_nOK, 7);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98);
    default:
        kPacket.m_nOK = -99;
    }

    if (NetError::GetLastNetError() != NetError::NET_OK) {
        SEND_PACKET(EVENT_KAIRO_ACK);
    }

    LOG_CONDITION(kPacket.m_nOK == 0, clog, cerr)
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kDBPacket.m_vecPostItem.size())
        << BUILD_LOG(kDBPacket.m_dwType)
        << BUILD_LOG(kDBPacket.m_listAttributeList.size())
        << BUILD_LOG(NetError::GetLastNetError())
        << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
}

IMPL_ON_FUNC(EVENT_KAIRO_ONESHOT_REQ)
{
    _VERIFY_STATE((3, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY));

    KDB_EVENT_KAIRO_ONESHOT_REQ kDBPacket;
    KEVENT_KAIRO_ONESHOT_ACK kPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkItem;
    DWORD dwGrade = 0;
    DWORD dwEquipType = 0;
    std::vector<KDropItemInfo>::iterator vit;

    SET_ERROR(ERR_UNKNOWN);

    pkItem = m_kInventory.FindDurationItemByItemUID(kPacket_.m_iItemUID);
    if (NULL == pkItem) {
        SET_ERR_GOTO(ERR_KAIRO_00, END_PROC);
    }

    kDBPacket.m_iCount = kPacket_.m_iItemDuration >= 100 ? 100 : kPacket_.m_iItemDuration;

    if (0 >= pkItem->m_nCount) {
        SET_ERR_GOTO(ERR_KAIRO_00, END_PROC);
    }

    if (false == SiKGSSimLayer()->CheckOneShotKairo(pkItem->m_ItemID, kDBPacket.m_iCount, kDBPacket.m_vecPostItem, kDBPacket.m_dwType)) {
        kDBPacket.m_kReqItemInfo = *pkItem;

        SET_ERR_GOTO(ERR_KAIRO_02, END_PROC);
    }

    if (true == kDBPacket.m_vecPostItem.empty()) {
        --pkItem->m_nCount;
        kPacket.m_kReqItemInfo = *pkItem;
        SET_ERR_GOTO(ERR_KAIRO_01, END_PROC);
    }

    if (Kairo::RT_STRENGTH == kDBPacket.m_dwType) {
        SET_ERR_GOTO(ERR_KAIRO_01, END_PROC);
    }

    kDBPacket.m_kReqItemInfo = *pkItem;

    QUEUEING_EVENT_TO_DB_CHECK(DB_EVENT_KAIRO_ONESHOT_REQ, kDBPacket);

    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError()) {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_KAIRO_00, kPacket.m_nOK, 1);
        ERR_CASE(ERR_KAIRO_01, kPacket.m_nOK, 2);
        ERR_CASE(ERR_KAIRO_02, kPacket.m_nOK, 3);
        ERR_CASE(ERR_KAIRO_05, kPacket.m_nOK, 6);
        ERR_CASE(ERR_KAIRO_06, kPacket.m_nOK, 7);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98);
    default:
        kPacket.m_nOK = -99;
    }

    if (NetError::NET_OK != NetError::GetLastNetError()) {
        SEND_PACKET(EVENT_KAIRO_ONESHOT_ACK);
    }

    LOG_CONDITION(kPacket.m_nOK == 0, clog, cerr)
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_.m_iItemUID)
        << BUILD_LOG(kPacket_.m_iItemDuration)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kDBPacket.m_vecPostItem.size())
        << BUILD_LOG(kDBPacket.m_dwType)
        << BUILD_LOG(kDBPacket.m_listAttributeList.size())
        << BUILD_LOG(NetError::GetLastNetError())
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << END_LOG;
}

IMPL_ON_FUNC(EVENT_KAIRO_ACK)
{
    JIF(PacketHackCheck(nFrom_));

    KItemPtr pkItem;
    KItemPtr pkStrengthItem;
    KItemPtr pkEquipItem;
    std::wstring strMsg;
    bool bNoticeEnable = false;
    std::vector<KItem>::iterator vit;

    NetError::SetLastNetError(kPacket_.m_nOK);

    if (false == IS_CORRECT(NET_OK)) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemUID(kPacket_.m_kReqItemInfo.m_ItemUID);
    if (NULL == pkItem) {
        START_LOG(cerr, L"아이템을 찾을 수 없음, Name : " << m_strName)
            << BUILD_LOG(kPacket_.m_kReqItemInfo.m_ItemUID) << END_LOG;
    }
    else {
        pkItem->m_nCount = kPacket_.m_kReqItemInfo.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kReqItemInfo.m_nCount;
    }

    if (NULL != pkItem && 0 >= pkItem->m_nCount) {
        m_kInventory.RemoveItem(pkItem->m_ItemID, pkItem->m_ItemUID);

        START_LOG(clog, L"보물상자 아이템이 0이 되어서 아이템 삭제 처리 Name : " << m_strName)
            << BUILD_LOG(pkItem->m_nCount)
            << BUILD_LOG(pkItem->m_ItemID)
            << BUILD_LOG(pkItem->m_ItemUID) << END_LOG;
    }

    if (Kairo::RT_STRENGTH == kPacket_.m_dwType) {
        kPacket_.m_vecItem.clear();
        pkStrengthItem = m_kInventory.FindItemByItemUID(kPacket_.m_kStrengthItem.m_ItemUID);
        if (NULL != pkStrengthItem) {
            pkStrengthItem->m_EnchantEquipItemUID = kPacket_.m_kStrengthItem.m_EnchantEquipItemUID;
            pkStrengthItem->m_cEnchantEquipGradeID = kPacket_.m_kStrengthItem.m_cEnchantEquipGradeID;
            pkStrengthItem->m_vecAttribute = kPacket_.m_kStrengthItem.m_vecAttribute;
        }

        pkEquipItem = m_kInventory.FindItemByItemUID(kPacket_.m_kEquipItem.m_ItemUID);
        if (NULL != pkEquipItem) {
            pkEquipItem->m_EnchantEquipItemUID = kPacket_.m_kEquipItem.m_EnchantEquipItemUID;
            pkEquipItem->m_cEnchantLevel = kPacket_.m_kEquipItem.m_cEnchantLevel;
            pkEquipItem->m_cEnchantEquipGradeID = kPacket_.m_kEquipItem.m_cEnchantEquipGradeID;
        }

        kPacket_.m_vecItem.push_back(kPacket_.m_kStrengthItem);
        kPacket_.m_vecItem.push_back(kPacket_.m_kEquipItem);
    }

    m_kInventory.AddItemList(kPacket_.m_vecItem);

    if (NULL != pkItem) {
        if (SiKGCHelper()->CheckKairoBox(pkItem->m_ItemID)) {
            for (vit = kPacket_.m_vecItem.begin(); vit != kPacket_.m_vecItem.end(); ++vit) {
                if (SiKGCHelper()->CheckNoticeItem(pkItem->m_ItemID, vit->m_ItemID)) {
                    bNoticeEnable = true;
                }
            }
        }
    }

    if (true == bNoticeEnable) {
        for (vit = kPacket_.m_vecItem.begin(); vit != kPacket_.m_vecItem.end(); ++vit) {
            if (true == SiKLoudMessage()->GetLoudMsg((int)vit->m_ItemID, 0, m_strNickName, strMsg, GetLanguageCode())) {
                ChatNotice(strMsg);
            }
        }
    }

    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError()) {
        ERR_CASE(NET_OK, kPacket_.m_nOK, 0);
        ERR_CASE(ERR_KAIRO_00, kPacket_.m_nOK, 1);
        ERR_CASE(ERR_KAIRO_01, kPacket_.m_nOK, 2);
        ERR_CASE(ERR_KAIRO_02, kPacket_.m_nOK, 3);
        ERR_CASE(ERR_KAIRO_03, kPacket_.m_nOK, 4);
        ERR_CASE(ERR_KAIRO_04, kPacket_.m_nOK, 5);
        ERR_CASE(ERR_KAIRO_05, kPacket_.m_nOK, 6);
        ERR_CASE(ERR_KAIRO_06, kPacket_.m_nOK, 7);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98);
    default:
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET(EVENT_KAIRO_ACK);

    _LOG_SUCCESS(kPacket_.m_nOK == 0 || kPacket_.m_nOK == 2, L"카이로 응답  : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_.m_nOK)
        << BUILD_LOG(kPacket_.m_dwType)
        << BUILD_LOG(kPacket_.m_vecItem.size())
        << END_LOG;
}

IMPL_ON_FUNC(EVENT_KAIRO_ONESHOT_ACK)
{
    JIF(PacketHackCheck(nFrom_));

    KItemPtr pkItem;
    KItemPtr pkStrengthItem;
    KItemPtr pkEquipItem;
    std::wstring strMsg;
    bool bNoticeEnable = false;
    std::vector<KItem>::iterator vit;

    NetError::SetLastNetError(kPacket_.m_nOK);

    if (!IS_CORRECT(NET_OK)) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemUID(kPacket_.m_kReqItemInfo.m_ItemUID);
    if (pkItem == NULL) {
        START_LOG(cerr, L"아이템을 찾을 수 없음, Name : " << m_strName)
            << BUILD_LOG(kPacket_.m_kReqItemInfo.m_ItemUID) << END_LOG;
    }
    else {
        pkItem->m_nCount = kPacket_.m_kReqItemInfo.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kReqItemInfo.m_nCount;
    }

    if (pkItem && pkItem->m_nCount <= 0) {
        m_kInventory.RemoveItem(pkItem->m_ItemID, pkItem->m_ItemUID);

        START_LOG(clog, L"보물상자 아이템이 0이 되어서 아이템 삭제 처리 Name : " << m_strName)
            << BUILD_LOG(pkItem->m_nCount)
            << BUILD_LOG(pkItem->m_ItemID)
            << BUILD_LOG(pkItem->m_ItemUID) << END_LOG;
    }

    m_kInventory.AddItemList(kPacket_.m_vecItem);

    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket_.m_nOK, 0);
        ERR_CASE(ERR_KAIRO_00, kPacket_.m_nOK, 1);
        ERR_CASE(ERR_KAIRO_01, kPacket_.m_nOK, 2);
        ERR_CASE(ERR_KAIRO_02, kPacket_.m_nOK, 3);
        ERR_CASE(ERR_KAIRO_03, kPacket_.m_nOK, 4);
        ERR_CASE(ERR_KAIRO_04, kPacket_.m_nOK, 5);
        ERR_CASE(ERR_KAIRO_05, kPacket_.m_nOK, 6);
        ERR_CASE(ERR_KAIRO_06, kPacket_.m_nOK, 7);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98);
    default:
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET(EVENT_KAIRO_ONESHOT_ACK);

    _LOG_SUCCESS(kPacket_.m_nOK == 0 || kPacket_.m_nOK == 2, L"카이로 응답 : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_.m_nOK)
        << BUILD_LOG(kPacket_.m_dwType)
        << BUILD_LOG(kPacket_.m_vecItem.size())
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_CREATE_PET_REQ )
{
    VERIFY_STATE((3, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL));

    KDB_EVENT_CREATE_PET kDBPacket;
    kDBPacket.m_nOK = -99;
    kDBPacket.m_kPetCreateInfo.Init();

    KEVENT_CREATE_PET_ACK  kPacket;
    kPacket.m_nOK = -99;

    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN ); // 기본 에러값

    if( kPacket_.m_cCharType != GetCurrentCharType() ) {
        kPacket_.m_cCharType = GetCurrentCharType();
    }

    // 귀속 펫일 경우 캐릭터 존재하는지 체크.
    if( !IsCharExist( kPacket_.m_cCharType ) ) {
        SET_ERR_GOTO( ERR_PET_00, END_PROC ); // 캐릭터가 존재하지 않음
    }

    // 해당 아이템 존재
    pkItem = m_kInventory.FindItem(  kPacket_.m_dwPetID, kPacket_.m_dwPetUID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_PET_01, END_PROC ); //펫 아이템이 존재하지 않음
    }

    // 해당 아이템이 영구 아이템인가?
    if ( pkItem->m_nCount != KItem::UNLIMITED_ITEM ||
         pkItem->m_nPeriod != KItem::UNLIMITED_ITEM ) {
        // 실제로 가지고 있는 아이템이 영구가 아니다.
        SET_ERR_GOTO( ERR_PET_01, END_PROC ); //펫 아이템이 존재하지 않음
    }

    // 이미 유저에게 존재하는 펫인지 체크
    {
        KPetInfo* pkPetInfo = NULL;
        if( GetPetInfo( kPacket_.m_dwPetUID, &pkPetInfo ) ) {
            SET_ERR_GOTO( ERR_PET_02, END_PROC ); //신규 생성하려는 팻이 이미 존재함
        }
    }

    if( !SiKLoudMessage()->GetItemName( static_cast<int>(kPacket_.m_dwPetID), 0,  kPacket_.m_strPetName, GetLanguageCode() ) )
    {
        START_LOG( cerr, L"팻 이름이 DB에 존재하지 않음. -.- Item ID : " << kPacket_.m_dwPetID << L", Name : " << m_strName ) << END_LOG;
        SET_ERR_GOTO( ERR_PET_01, END_PROC );
    }

    kDBPacket.m_kPacket = kPacket_;
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CREATE_PET_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK,     kPacket.m_nOK, 0 ); // 성공
    ERR_CASE( ERR_PET_00, kPacket.m_nOK, 1 ); // 펫 존재하지 않음
    ERR_CASE( ERR_PET_01, kPacket.m_nOK, 2 ); // 펫 아이템을 가지고 있지 않음
    ERR_CASE( ERR_PET_02, kPacket.m_nOK, 3 ); // 신규 생성하려는 펫이 이미 존재함
    ERR_CASE( ERR_PET_03, kPacket.m_nOK, 4 ); // 해당 유저가 존재하지 않음
    ERR_CASE( ERR_PET_04, kPacket.m_nOK, 5 ); // 펫 아이템이 아님
    ERR_CASE( ERR_PET_05, kPacket.m_nOK, 6 ); // 해당 캐릭터가 존재하지 않음
    ERR_CASE( ERR_PET_40, kPacket.m_nOK, 7 ); // 펫 귀속 아이템이 아닌데 캐릭터 타입이 전달됨.
    ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"정의되지 않은 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_CREATE_PET_ACK );
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwPetID )
        << BUILD_LOG( kPacket_.m_dwPetUID )
        << BUILD_LOG( kPacket_.m_strPetName )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CREATE_PET_ACK, KDB_EVENT_CREATE_PET )
{
    //오류값 4부터 사용
    KPetInfo kTmpPetInfo;
    // PetInfo 초기화
    kTmpPetInfo.Init();

    KEVENT_CREATE_PET_ACK kPacket;
    kPacket.m_nOK           = -99;
    kPacket.m_pairCreatePet = std::make_pair( -1, kTmpPetInfo );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if( !kPacket_.m_vecRewardItem.empty() ) {
        int nInsert;
        nInsert = m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
        START_LOG( clog, L"펫 만들때 끼워주는 아이템 수 : " << nInsert ) << END_LOG;
    }

    // 생성된것은 펫 인포에 추가한다.
    if( kPacket_.m_kPetCreateInfo.m_dwUID > 0 ) {
        // 검사하지 않겠다.
        m_mapPetInfo.insert( std::make_pair( kPacket_.m_kPetCreateInfo.m_dwUID, kPacket_.m_kPetCreateInfo ) );

        START_LOG( clog, L"따끈따끈한 펫의 정보는? " )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_dwUID )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_dwID )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_strName )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_mapInitExp.size() )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_dwEXP )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_iLevel )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_nHatchingID )
            << BUILD_LOGc( kPacket_.m_kPetCreateInfo.m_cPromotion )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_iInitSatiation )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_iSatiation )
            << BUILD_LOG( kPacket_.m_kPetCreateInfo.m_vecEquipItem.size() ) << END_LOG;
    }

    // ACK데이터 생성
    kPacket.m_pairCreatePet = std::make_pair( kPacket_.m_kPacket.m_cCharType, kPacket_.m_kPetCreateInfo );
    kPacket.m_vecRewardItem = kPacket_.m_vecRewardItem;
/*
    if( !EquipPet( kPacket_.m_kPetCreateInfo.m_dwUID, kPacket_.m_kPacket.m_cCharType ) ) {
        // 이런일은 절때로 있어서는 안된다!!!
        START_LOG( cerr, L"생성된 펫 장착 실패" ) << END_LOG;
    }
*/
END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,     kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_PET_00, kPacket.m_nOK, 1 ); // 펫 존재하지 않음
        ERR_CASE( ERR_PET_01, kPacket.m_nOK, 2 ); // 펫 아이템을 가지고 있지 않음
        ERR_CASE( ERR_PET_02, kPacket.m_nOK, 3 ); // 신규 생성하려는 펫이 이미 존재함
        ERR_CASE( ERR_PET_03, kPacket.m_nOK, 4 ); // 해당 유저가 존재하지 않음
        ERR_CASE( ERR_PET_04, kPacket.m_nOK, 5 ); // 펫 아이템이 아님
        ERR_CASE( ERR_PET_05, kPacket.m_nOK, 6 ); // 해당 캐릭터가 존재하지 않음
        ERR_CASE( ERR_PET_40, kPacket.m_nOK, 7 ); // 펫 귀속 아이템이 아닌데 캐릭터 타입이 전달됨.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"정의되지 않은 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( IS_CORRECT( NET_OK ), L" 펫 생성 응답. OK : " << kPacket.m_nOK )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_pairCreatePet.second.m_dwID ) << END_LOG;
    // 펫 생성은 브로드할 필요가 없음
    SEND_PACKET( EVENT_CREATE_PET_ACK );
}

IMPL_ON_FUNC( EVENT_DELETE_PET_REQ )
{
    // DB로 보낼 펫 제거 패킷
    KDB_EVENT_DELETE_PET kDBPacket;
    KEVENT_DELETE_PET_ACK kPacket;
    kPacket.m_iGP = -1;
    kPacket.m_vecRewardItem.clear();

    KItemPtr pkItem;
    KPetInfo* pkPetInfo = NULL;


    if ( kPacket_.m_PetItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
        SET_ERR_GOTO( ERR_PET_37, END_PROC );
    }

    // 아이템 확인
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_PetItemUID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_PET_06, END_PROC );
    }

    // 펫 존재 확인
    if( !GetPetInfo( kPacket_.m_PetItemUID, &pkPetInfo ) ) {
        SET_ERR_GOTO( ERR_PET_07, END_PROC );
    }

    // 공용 펫이 아닐 경우 현재 선택된 캐릭터가 펫의 소유권을 가지고 있는지 체크.
    if( pkPetInfo->m_cCharType != -1 && pkPetInfo->m_cCharType != kPacket_.m_cCharType ) {
        SET_ERR_GOTO( ERR_PET_39, END_PROC );
    }

    //여기 까지 펫도 존재하고 아이템도 존재한다.
    kDBPacket.m_kItemInfo       = *pkItem;
    kDBPacket.m_kPetDeleteInfo  = *pkPetInfo;
    GetDBUpdateData( kDBPacket.m_kUserData );

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_DELETE_PET_REQ, kDBPacket );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() ) {

        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_PET_06,       kPacket.m_nOK, 1 ); // 펫 아이템(시민권)이 없음
        ERR_CASE( ERR_PET_07,       kPacket.m_nOK, 2 ); // 펫 정보가 존재하지 않음
        ERR_CASE( ERR_PET_03,       kPacket.m_nOK, 3 ); // 해당 유저가 존재하지 않음
        ERR_CASE( ERR_PET_04,       kPacket.m_nOK, 4 ); // 펫 아이템이 아님
        ERR_CASE( ERR_PET_08,       kPacket.m_nOK, 5 ); // 시민권을 팔다가 오류 발생
        ERR_CASE( ERR_DB_COMMON_03, kPacket.m_nOK, 6 ); // DB와 User Data 동기화 실패
        ERR_CASE( ERR_PET_37,       kPacket.m_nOK, 7 ); // PC 방 펫 제거 하려고 함.
        ERR_CASE( ERR_PET_39,       kPacket.m_nOK, 8 ); // 펫의 소유권을 가진 캐릭터가 아닌데 펫 삭제 시도했음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
        default:
            START_LOG( cerr, L"펫 제거 응답 중 알수 없는 오류. Name : " << m_strName )
                << BUILD_LOG( NetError::GetLastNetError() )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
            kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_DELETE_PET_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"펫 삭제 요청, Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_PetItemUID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_DELETE_PET_ACK, KDB_EVENT_DELETE_PET )
{
    KEVENT_DELETE_PET_ACK kPacket;
    kPacket.m_iGP = 0;
    kPacket.m_nOK = -99;
    std::vector< char > vecChars;
    std::vector<char>::iterator vit;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PRC;
    }

    // 펫정보를 지운다.
    LIF( DeletePet( kPacket_.m_kPetDeleteInfo.m_dwUID ) );

    // 펫을 장착 하고 있는 캐릭터를 가져온다.
    GetPetOwner( kPacket_.m_kPetDeleteInfo.m_dwUID, vecChars );

    // 순회 하면서 캐릭터가 장착 하고 있는 펫을 제거 한다.
    for( vit = vecChars.begin() ; vit != vecChars.end() ; ++vit ) {

        KCharacterInfo* pkCharacter = NULL;
        pkCharacter = GetCharacterInfo( *vit );

        if( pkCharacter != NULL ) {
            pkCharacter->m_kEquipPetInfo.Init();
        }
    }

    // 펫 아이템 제거.
    m_kInventory.RemoveItem( kPacket_.m_kItemInfo.m_ItemID, kPacket_.m_kItemInfo.m_ItemUID );
    // 보상을 적용 한다.
    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
    if ( kPacket_.m_vecRewardItem.empty() && kPacket_.m_iGP > 0 ) { // 판매 후 보상이 GP 인 경우
        SetGP( kPacket_.m_iGP );
        SetInitGP( GetGP() );
    }

    // 전달 패킷을 만든다.
    kPacket.m_vecRewardItem = kPacket_.m_vecRewardItem;
    kPacket.m_iGP = GetGP();

END_PRC:
    switch( NetError::GetLastNetError() ) {

        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_PET_06,       kPacket.m_nOK, 1 ); // 펫 아이템(시민권)이 없음
        ERR_CASE( ERR_PET_07,       kPacket.m_nOK, 2 ); // 펫 정보가 존재하지 않음
        ERR_CASE( ERR_PET_03,       kPacket.m_nOK, 3 ); // 해당 유저가 존재하지 않음
        ERR_CASE( ERR_PET_04,       kPacket.m_nOK, 4 ); // 펫 아이템이 아님
        ERR_CASE( ERR_PET_08,       kPacket.m_nOK, 5 ); // 시민권을 팔다가 오류 발생
        ERR_CASE( ERR_DB_COMMON_03, kPacket.m_nOK, 6 ); // DB와 User Data 동기화 실패
        ERR_CASE( ERR_PET_37,       kPacket.m_nOK, 7 ); // PC 방 펫 제거 하려고 함.
        ERR_CASE( ERR_PET_39,       kPacket.m_nOK, 8 ); // 펫의 소유권을 가진 캐릭터가 아닌데 펫 삭제 시도했음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
        default:
            START_LOG( cerr, L"펫 제거 응답 중 알수 없는 오류. Name : " << m_strName )
                << BUILD_LOG( NetError::GetLastNetError() )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
            kPacket.m_nOK = 99;
    }

    // 펫 제거는 브로드할 필요 없음
    SEND_PACKET( EVENT_DELETE_PET_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"펫 제거 응답. Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_kItemInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItemInfo.m_ItemUID )
        << BUILD_LOG( kPacket.m_iGP )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_FEEDING_PET_REQ )
{
    KEVENT_FEEDING_PET_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    KPetInfo* pkPetInfo = NULL;
    KItemPtr pkFeedItem;
    int nFeedFactor = 0;

    // 펫을 보유하고 있는지 확인한다.
    if( !GetPetInfo( kPacket_.m_dwPetUID, &pkPetInfo ) )
    {
        SET_ERR_GOTO( ERR_PET_07, END_PROC );
    }
    // 펫 아이템이 있는지 확인한다.
    if ( m_kInventory.FindItemByItemUID( kPacket_.m_feedItemUID ) == NULL ) {
        SET_ERR_GOTO( ERR_PET_01, END_PROC );
    }

    // 먹이 아이템이 존재 하는가?
     pkFeedItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_feedItemUID );
     if( pkFeedItem == NULL ) {
         SET_ERR_GOTO( ERR_PET_09, END_PROC );
     }

     if ( pkFeedItem->m_nCount <= 0 ) {
         SET_ERR_GOTO( ERR_PET_09, END_PROC );
     }

    // 요청한 아이템이 먹이 아이템인지 확인 한다.
     if( !SiKPetHelper()->GetPetFeedFactor( (int)pkFeedItem->m_ItemID, nFeedFactor ) ) {
         SET_ERR_GOTO( ERR_PET_12, END_PROC );
     }

     // 먹이 주고, 아이템 수량 하나 깐다.
     const int nMaxFeed = SiKPetHelper()->GetMaxFeed();
     pkPetInfo->m_iSatiation = std::min<int>( pkPetInfo->m_iSatiation + nFeedFactor, nMaxFeed );
     --pkFeedItem->m_nCount;


     // 패킷에 데이터 복사.
     kPacket.m_iSatiation = pkPetInfo->m_iSatiation;
     kPacket.m_dwPetUID = kPacket_.m_dwPetUID;
     kPacket.m_kFeedItem = *pkFeedItem;

     SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK,     kPacket.m_nOK, 0 );
    ERR_CASE( ERR_PET_07, kPacket.m_nOK, 1 );// 해당 펫이 존재하지 않음
    ERR_CASE( ERR_PET_01, kPacket.m_nOK, 2 );// 해당 펫 아이템을 가지고 있지 않음
    ERR_CASE( ERR_PET_09, kPacket.m_nOK, 3 );// 펫 관련 아이템이 존재하지 않거나 부족함. ) //먹이.
    ERR_CASE( ERR_PET_12, kPacket.m_nOK, 5 );// 해당 아이템은 펫 먹이 아이템이 아님.
    default:
        START_LOG( cerr, L"정의되지 않은 Err값입니다." )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }
    //, m_iID, m_iUID, m_iDuration
    _LOG_SUCCESS( kPacket.m_nOK == 0, L"펫에 먹이 주기 Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwPetUID )
        << BUILD_LOG( nFeedFactor )
        << BUILD_LOG( kPacket.m_kFeedItem.m_ItemID )
        << BUILD_LOG( kPacket.m_iSatiation )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    SEND_PACKET( EVENT_FEEDING_PET_ACK );
}

IMPL_ON_FUNC( EVENT_CHANGE_PET_NAME_REQ )
{
    KItemPtr pkItem;

    KEVENT_CHANGE_PET_NAME_ACK kPacket;
    kPacket = -99;

    if ( kPacket_.m_dwPetUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
        SET_ERR_GOTO( ERR_PET_36, END_PROC );
    }

    if( CHANGE_PET_NICK_GOODS_ID != kPacket_.m_kNickNameItem.m_ItemID )
    {
        SET_ERR_GOTO( ERR_PET_23, END_PROC );
    }

    pkItem = m_kInventory.FindDurationItem( kPacket_.m_kNickNameItem.m_ItemID, kPacket_.m_kNickNameItem.m_ItemUID );

    // 몬찾았다
    if( pkItem == NULL )
    {
        SET_ERR_GOTO( ERR_PET_19, END_PROC );
    }

    // 찾았으면 적법한 닉네임인지 알아보자
    int nRet = SiKSlangFilter()->IsValidateNickNameString( kPacket_.m_strPetNickName, m_strName, SiKGSSimLayer()->m_uiPetNickNameAvailable );
    // 적법한 닉네임이 아니다
    if( nRet != 0 )
    {
        SET_ERR_GOTO( ERR_PET_20, END_PROC );
    }

    // 너...펫이 있냐?
    KPetInfo* pkPetInfo = NULL;
    if( !GetPetInfo( kPacket_.m_dwPetUID, &pkPetInfo ) )
    {
        SET_ERR_GOTO( ERR_PET_07, END_PROC );
    }

    if( pkPetInfo->m_strName.compare( kPacket_.m_strPetNickName ) == 0 )
    {
        SET_ERR_GOTO( ERR_PET_20, END_PROC );
    }

    if( kPacket_.m_dwPetID != pkPetInfo->m_dwID )
    {
        SET_ERR_GOTO( ERR_PET_21, END_PROC );
    }

    // 아이템도 있고~ 적법한 닉네임이고~ 펫도 있고~ ID도 맞고~ 착해라..
    QUEUEING_EVENT_TO_DB( EVENT_CHANGE_PET_NAME_REQ, kPacket_ );
    return;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( ERR_PET_19, kPacket, 1 );
        ERR_CASE( ERR_PET_20, kPacket, 2 );
        ERR_CASE( ERR_PET_07, kPacket, 3 );
        ERR_CASE( ERR_PET_21, kPacket, 4 );
        ERR_CASE( ERR_PET_23, kPacket, 6 ); // 5는 ACK에서 사용하므로.
        ERR_CASE( ERR_PET_36, kPacket, 7 );
    }

    // Err Send
    SEND_PACKET( EVENT_CHANGE_PET_NAME_ACK );
    START_LOG( cerr, L"펫 닉네임 바꾸기 실패" )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwPetID )
        << BUILD_LOG( kPacket_.m_dwPetUID )
        << BUILD_LOG( kPacket_.m_kNickNameItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kNickNameItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kNickNameItem.m_nCount )
        << BUILD_LOG( kPacket_.m_strPetNickName )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << END_LOG;
}

// 여기 까지 오면 닉네임은 이미 바뀐 상태이삼..
_IMPL_ON_FUNC( EVENT_CHANGE_PET_NAME_ACK, KEVENT_CHANGE_PET_NAME_REQ )
{
    // 에러값 5부터

    KEVENT_CHANGE_PET_NAME_ACK kPacket;
    kPacket = kPacket_.m_nOK;

    // 아이템을 지우자..
    KItemPtr pkItem;

    if( kPacket != 0 )
    {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindDurationItem( kPacket_.m_kNickNameItem.m_ItemID, kPacket_.m_kNickNameItem.m_ItemUID );

    if ( pkItem != NULL ) {
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount -1 );
    }
 
    if( kPacket_.m_nOK == 0 )
    {
        KPetInfo* pkPetInfo = NULL;
        if( GetPetInfo( kPacket_.m_dwPetUID, &pkPetInfo ) )
        {
            pkPetInfo->m_strName = kPacket_.m_strPetNickName;
        }
    }

END_PROC:

    SEND_PACKET( EVENT_CHANGE_PET_NAME_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwPetID )
        << BUILD_LOG( kPacket_.m_dwPetUID )
        << BUILD_LOG( kPacket_.m_kNickNameItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kNickNameItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kNickNameItem.m_nCount )
        << BUILD_LOG( kPacket_.m_strPetNickName )
        << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_LOUD_MSG, std::wstring )
{
    //KEventPtr spEvent( new KUserEvent );
    //
    //KEVENT_CHAT_NOT kPacket;
    //kPacket.m_cMsgType      = KEVENT_CHAT_NOT::ENUM_MESSAGE_TYPE::MSG_NOTIFY;
    //kPacket.m_strMessage    = kPacket_;

    //spEvent->m_usEventID = KUserEvent::EVENT_CHAT_NOT;

    //// make event object
    //KSerializer         ks;
    //// serialize - only data
    //ks.BeginWriting( &spEvent->m_kbuff );
    //ks.Put( kPacket );
    //ks.EndWriting();

    //spEvent->m_kbuff.Compress();   // 압축할 경우, KEvent의 buff만을 압축한다.

    //// serialize - full event object
    //KSerBuffer          kbuff;
    //ks.BeginWriting( &kbuff );
    //ks.Put( *spEvent );
    //ks.EndWriting();


    //SiKChannelManager()->BroadPacket( kbuff, L"EVENT_CHAT_NOT" );
}

_IMPL_ON_FUNC( EVENT_PRE_COMPLETE_REQ, int )
{
    int kPacket = -99;
    std::set<int> setForceMode;
    std::map< char, std::map<int,KDungeonUserInfo> >::iterator mitTotal;
    std::map<int,KDungeonUserInfo>::iterator mit;

    if( !SiKGSSimLayer()->IsEnableForceNewUser() )
    {
        kPacket = 1; // 시스템이 동작 중이지 않음.
        goto END_PROC;
    }

    SiKGSSimLayer()->GetForceMode( setForceMode );
    if( setForceMode.find( kPacket_ ) == setForceMode.end() )
    {
        kPacket = 2; // 모드 리스트에 없다.
        goto END_PROC;
    }

    mitTotal = m_mapDifficulty.find( m_cCurrentCharacter );
    if( mitTotal == m_mapDifficulty.end() ) {
        kPacket = 3; // 초기에 모드 셋팅하지 못했다.
        goto END_PROC;
    }

    mit = mitTotal->second.find( kPacket_ );
    if( mit == mitTotal->second.end() ) {
        kPacket = 3; // 초기에 모드 셋팅하지 못했다.
        goto END_PROC;
    }

    mit->second.m_kModeDifficulty.Set( 0 );
    kPacket = 0;

END_PROC:
    SEND_PACKET( EVENT_PRE_COMPLETE_REQ );

    _LOG_SUCCESS( kPacket == 0, L" 신규 유저 강제 선행 동작. Login : " << GetName() )
        << BUILD_LOG( kPacket )
        << BUILD_LOG( setForceMode.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_USE_INVENTORY_EXTEND_REQ, KExtendItem )
{
    KItemPtr pkItem;
    int nMaxInvenCapacity = SiKGCHelper()->GetMaxInvenCapacity();
    int nCharCurrentInvenCapacity = GetCharInvenCapacity();
    SET_ERROR( ERR_UNKNOWN );

    if( nCharCurrentInvenCapacity == 0 ) {
        SET_ERR_GOTO( ERR_INVEN_04, END_PROC );
    }

    if( nCharCurrentInvenCapacity >= nMaxInvenCapacity ) {
        // 더이상 인벤토리를 확장할수 없음
        SET_ERR_GOTO( ERR_INVEN_03, END_PROC );
    }

    // 인벤토리 아이템 ID로 확장 크기 구분
    // 내부에서 스크립트에 설정된 크기를 되돌린다. 쓰레기값이 나올 경우는 없겠다.
    if( SiKGCHelper()->IsInvenExtendItem( kPacket_.m_itemID, kPacket_.m_nInvenCapacity ) == false )
    {
        //인벤토리 아이템이 아님
        START_LOG( cerr, L"인벤토리 아이템이 아님. kPacket_.m_dwID : " << kPacket_.m_itemID ) << END_LOG;
        SET_ERR_GOTO( ERR_INVEN_00, END_PROC );
    }

    //인벤토리 초과 불가능일때, 검사.
    if( SiKGCHelper()->GetInvenOverflowEnable() == false &&
        nCharCurrentInvenCapacity + kPacket_.m_nInvenCapacity > nMaxInvenCapacity ) {
        // 현재 인벤 크기에 확장 요청 크기를 더해서 최대치를 넘으면 에러
        SET_ERR_GOTO( ERR_INVEN_03, END_PROC );
    }

    pkItem = m_kInventory.FindItem( kPacket_.m_itemID, kPacket_.m_itemUID );

    if( pkItem == NULL )
    {
        // 아이템을 보유하고 있지 않음
        SET_ERR_GOTO( ERR_INVEN_01, END_PROC );
    }

    if( pkItem->m_nCount <= 0 )
    {
        // 아이템 수량이 부족
        SET_ERR_GOTO( ERR_INVEN_02, END_PROC );
    }

    if ( pkItem ) {
        kPacket_.m_kItem = *pkItem;
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_USE_INVENTORY_EXTEND_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );       // 성공
    ERR_CASE( ERR_INVEN_00, kPacket_.m_nOK, 1 ); // 아이템 아이디가 다름
    ERR_CASE( ERR_INVEN_01, kPacket_.m_nOK, 2 ); // 아이템을 보유하고 있지 않음
    ERR_CASE( ERR_INVEN_02, kPacket_.m_nOK, 3 ); // 아이템 수량이 부족
    ERR_CASE( ERR_INVEN_03, kPacket_.m_nOK, 4 ); // 더이상 인벤토리를 확장할수 없음
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"인벤토리 확장.. 알수 없는 오류" )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( kPacket_.m_itemUID )
        << BUILD_LOG( kPacket_.m_nInvenCapacity )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( SiKGCHelper()->GetInvenOverflowEnable() )
        << BUILD_LOG( nMaxInvenCapacity ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_RECEIVED_PACKET( EVENT_USE_INVENTORY_EXTEND_ACK );
}

_IMPL_ON_FUNC( EVENT_USE_INVENTORY_EXTEND_ACK, KExtendItem )
{
    JIF( PacketHackCheck( nFrom_ ) );

    KItemPtr pkItem;
    int nTotalInvenCapacity;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( ( kPacket_.m_nOK != 0 && kPacket_.m_nOK != NetError::ERR_INVEN_13 ) ) {
        goto END_PROC;
    }

    UpdateCharInvenCapacity( kPacket_.m_nInvenCapacity, nTotalInvenCapacity );
    kPacket_.m_nInvenCapacity = nTotalInvenCapacity; // 전체 인벤사이즈 넣어주기.

    pkItem = m_kInventory.FindItem( kPacket_.m_itemID, kPacket_.m_itemUID );

    if( pkItem != NULL ) {
        pkItem->m_nCount = std::max<int>( 0, kPacket_.m_kItem.m_nCount );
        pkItem->m_nInitCount = pkItem->m_nCount;

        if ( pkItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( pkItem->m_ItemID, pkItem->m_ItemUID );
        }
    }

END_PROC:

    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 아이템 아이디가 다름
    ERR_CASE( ERR_INVEN_03, kPacket_.m_nOK, 4 ); // 더이상 인벤토리를 확장할수 없음
    ERR_CASE( ERR_INVEN_04, kPacket_.m_nOK, 5 ); // 해당 유저가 존재하지 않음
    ERR_CASE( ERR_INVEN_13, kPacket_.m_nOK, 6 ); // 가방 아이템 DB 동기화 실패
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"인벤토리 확장.. 알수 없는 오류" )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    }

    SEND_RECEIVED_PACKET( EVENT_USE_INVENTORY_EXTEND_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << L"인벤토리 확장 ." << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nInvenCapacity )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( SiKGCHelper()->GetInvenOverflowEnable() )
        << BUILD_LOG( SiKGCHelper()->GetMaxInvenCapacity() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_STAT_USER_HISTORY_NOT )
{
    QUEUEING_EVENT_TO_DB( EVENT_STAT_USER_HISTORY_NOT, kPacket_ );
}

//한시간 이벤트 (유저에게 NOT)
_IMPL_ON_FUNC( EVENT_REPETITION_PROVIDE_NOT, KEVENT_QUERY_INVENTORY_INFO_ACK )
{
    m_kInventory.AddItemList( kPacket_.m_vecInv );
    ChatNotice( kPacket_.m_strMsg );

    SEND_RECEIVED_PACKET( EVENT_REPETITION_PROVIDE_NOT );
}

_IMPL_ON_FUNC( EVENT_PROTECED_TABBLE_CHECKSUM_NOT, DWORD )
{
    SetTick( PROTECTED_TABLE_TICK );

    if( !SiKGSSimLayer()->CheckTableChecksum( kPacket_ ) )
    {
        //SiKFailRate()->IncreaseCErrCount( KFailRate::CE_HACK_TABLE );

        KEVENT_CLOSE_CONNECTION_NOT kPacket;
        kPacket.m_nCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_PT_CHECKSUM;
        kPacket.m_strMessage = SiKGameServer()->m_stringTable.GetValue(L"str_01");
        SEND_DATA( EVENT_CLOSE_CONNECTION_NOT, kPacket );
        int nType = KPreHackingCheckManager::HT_TABLE_CHECKSUM;
        QUEUEING_EVENT_TO_DB( EVENT_INSERT_HACK_INFO_NOT, nType );
        ReserveDestroy( DRW_PROTECTED_TABLE_VERIFY );

        START_LOG( cerr, L"뒈져라~~~~" )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( SiKGSSimLayer()->GetTableChecksum() ) << END_LOG;
    }
}

_IMPL_ON_FUNC( EVENT_FORCE_CHANGE_ROOMUSER_INFO_REQ, KForceChangeRoomUserInfo )
{
    // 상태가 올바르냐?
    _VERIFY_STATE(( 1, KUserFSM::STATE_ROOM ));
    // 룸 포인터가 유효하냐?
    JIF( m_pkRoom );
    // 방장이냐?
    if( this != m_pkRoom->GetHostPtr() && !IsAdmin() )
    {
        START_LOG( cwarn, L"방장 또는 운영자가 아닌데 방원들의 정보 변경 패킷을 보내려고 했다." )
            << BUILD_LOG( m_strName ) << END_LOG;
        return;
    }

    //액션을 취한다.
    m_pkRoom->ProcessForceEvent( kPacket_ );
    
    // 패킷을 보낸다.
    SEND_RECEIVED_PACKET( EVENT_FORCE_CHANGE_ROOMUSER_INFO_ACK );
    if( kPacket_.m_nOK == 0 )
    {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_FORCE_CHANGE_ROOMUSER_INFO_BROAD, kPacket_ );
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_ucChangeType )
        << BUILD_LOGc( kPacket_.m_bReady ) << END_LOG;
}

//유효한 진화가 가능한지 체크 한다.
IMPL_ON_FUNC( EVENT_CHECK_TRANSFORMATION_PET_REQ )
{
    // 상태가 올바른가.
    _VERIFY_STATE(( 3, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY ));

    KEVENT_CHECK_TRANSFORMATION_PET_ACK kPacket;
    KItemPtr pkItem;
    int nPromotionFactor = 0; // 진화 1단계 할지 톼화 1단계 할지 결정.
    int iThresholdLv = 0; // 진화 아이템에서 가져온 최소 진화 레벨 
    KPetInfo* pkPetInfo = NULL;

    SET_ERROR( ERR_UNKNOWN );

    if ( kPacket_.m_dwPetUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
        SET_ERR_GOTO( ERR_PET_38, END_PROC );
    }

    //해당 펫이 존재하는가?
    if( !GetPetInfo( kPacket_.m_dwPetUID, &pkPetInfo ) ) {
        // 펫이 없다.
        SET_ERR_GOTO( ERR_PET_07, END_PROC );
    }

    switch( kPacket_.m_ePTT )
    {
    case KUser::PTT_EVOLUTION:
        {
            if( SiKPetHelper()->GetEvolutionType( kPacket_.m_itemID )  != KUser::PTT_EVOLUTION ) {
                SET_ERR_GOTO( ERR_PET_30, END_PROC );
            }

            nPromotionFactor = 1;
        }
        break;
    case KUser::PTT_DEVOLUTION:
        {
            //펫의 전직 상태가 1 이상인가?
            if( pkPetInfo->m_cPromotion < 1 ) {
                SET_ERR_GOTO( ERR_PET_24, END_PROC );
            }

            if( SiKPetHelper()->GetEvolutionType( kPacket_.m_itemID )  != KUser::PTT_DEVOLUTION ) {
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

    //펫 진화/퇴화 카드가 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_itemID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_PET_25, END_PROC );
    }
    kPacket_.m_itemUID = pkItem->m_ItemUID;

    if( !SiKPetHelper()->GetTransformLv( kPacket_.m_itemID, iThresholdLv ) ) {
        SET_ERR_GOTO( ERR_PET_31, END_PROC );
    }

    pkPetInfo->m_iLevel = SiKPetHelper()->GetPetLevel( pkPetInfo->m_dwEXP );
    // 내 펫의 레벨이 진화 기준이 못미치는가?
    if( pkPetInfo->m_iLevel < iThresholdLv ) {
        START_LOG( cerr, L"펫 레벨이 부족하다.., Name : " << m_strName )
            << BUILD_LOG( pkPetInfo->m_iLevel )
            << BUILD_LOG( iThresholdLv ) << END_LOG;

        SET_ERR_GOTO( ERR_PET_24, END_PROC );
    }

    //진화되기 전의 기본 이름을 얻어 온다.
    if( !SiKLoudMessage()->GetItemName( static_cast<int>( pkPetInfo->m_dwID ), pkPetInfo->m_cPromotion, kPacket.m_strOriginName, GetLanguageCode() ) )
    {
        // 아이템 이름이 존재하지 않음.
        SET_ERR_GOTO( ERR_PET_26, END_PROC );
    }

    //진화된 펫의 이름을 얻어낸다.
    if( !SiKLoudMessage()->GetItemName( static_cast<int>( pkPetInfo->m_dwID ), pkPetInfo->m_cPromotion + nPromotionFactor, kPacket.m_strNewName, GetLanguageCode() ) )
    {
        // 아이템 이름이 존재하지 않음.
        SET_ERR_GOTO( ERR_PET_26, END_PROC );
    }

    kPacket.m_itemID = pkItem->m_ItemID;
    kPacket.m_itemUID = pkItem->m_ItemUID;
    kPacket.m_dwPetUID = kPacket_.m_dwPetUID;
    kPacket.m_ePTT     = kPacket_.m_ePTT;

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
    ERR_CASE( ERR_PET_07, kPacket.m_nOK, 1 ); // 펫 정보가 존재하지 않음
    ERR_CASE( ERR_PET_24, kPacket.m_nOK, 2 ); // 펫 이 진화하기에 레벨이 낮다.
    ERR_CASE( ERR_PET_25, kPacket.m_nOK, 3 ); // 펫 진화 카드를 가지고 있지 않거나 부족함.
    ERR_CASE( ERR_PET_26, kPacket.m_nOK, 4 ); // 이름 정보가 없습니다.
    ERR_CASE( ERR_PET_29, kPacket.m_nOK, 5 ); // 펫 전직 타입이 올바르지 않음.
    ERR_CASE( ERR_PET_30, kPacket.m_nOK, 6 ); // 잘못된 진화/퇴화 아이템 정보를 보냈음.
    ERR_CASE( ERR_PET_38, kPacket.m_nOK, 7 ); // PC 방 펫 진화 시도.

    default:
        kPacket.m_nOK = 99;
        SET_ERROR( ERR_UNKNOWN );
        break;
    }
    SEND_PACKET( EVENT_CHECK_TRANSFORMATION_PET_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L"진화 유효성 체크"
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ePTT )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( kPacket_.m_itemUID )
        << BUILD_LOG( kPacket_.m_dwPetUID )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_strNewName )
        << BUILD_LOG( kPacket.m_strOriginName ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_CYOU_GET_WEB_POINT_REQ )
{
    if ( SiKGSSimLayer()->GetBillingType() != KGSSimLayer::BT_CN_CYOU ) {
        return;
    }

    CYOUGetCurrentPoint( SiKGameServer()->GetServerID() );
}

_IMPL_ON_FUNC( EVENT_CYOU_GET_WEB_POINT_ACK, KCYOUCashPoint )
{ // 설명 : 중국 창유 포인트(웹 캐시) 는 소수점 첫째자리 까지 클라이언트에 표시.
    START_LOG( clog, L"중국 웹 캐시 잔액 조회 결과 , User : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwCYOUPoint )
        << END_LOG;

    m_bFirstGetCyouPoint = false;
    SEND_RECEIVED_PACKET( EVENT_CYOU_GET_WEB_POINT_ACK );
}

IMPL_ON_FUNC( EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT )
{
    /* DB 데이터에서 변경되는 정보를 따로 변수에 기록해두었다가 접종, 캐릭터 변경시 DB에 저장하는 용도로 사용하는 로직을 사용한다
       이 때문에 이 곳에 패킷이 전달되는 상황은 DB->S 상황만을 가정한다
       다른 경로에서 유저 스레드에 이 이벤트를 큐잉하는 경우 로직에 오류가 발생한다 */
    m_mapDungeonPersonalRecord[ kPacket_.m_cCharType ] = kPacket_.m_mapDungeonPersonalRecord;
    m_mapDungeonPersonalRecordDiff.clear();

    SEND_RECEIVED_PACKET( EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT );

    _LOG_SUCCESS( NetError::NET_OK == kPacket_.m_nOK, L"던전 개인 최고 기록 알림, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_mapDungeonPersonalRecord.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_DUNGEON_RANK_CURRENT_SEASON_RANK_PAGE_REQ )
{
    // kPacket_: < ModeID, PageNum > // 첫 페이지 번호를 0으로 한다

    KEVENT_DUNGEON_RANK_CURRENT_SEASON_RANK_PAGE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nModeID = kPacket_.first;
    kPacket.m_nPageNum = kPacket_.second;

    std::list< KDungeonRankInfo > lstCurrentDungeonRankList;
    std::list< KDungeonRankInfo >::iterator lit;
    int nRankEntryPerPage = SiKDungeonRankManager()->GetRankEntryPerPage();
    int nTotalPage = 0;
    int nLastPageEntries = nRankEntryPerPage;
    int nStartIndex = 0;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKDungeonRankManager()->IsRankingModes( kPacket.m_nModeID ) ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_01, END_PROC );
    }

    SiKDungeonRankManager()->GetCurrentDungeonRankList( kPacket.m_nModeID, lstCurrentDungeonRankList );

    if ( true == lstCurrentDungeonRankList.empty() ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_02, END_PROC );
    }

    nTotalPage = lstCurrentDungeonRankList.size() / nRankEntryPerPage;
    if ( nTotalPage < kPacket.m_nPageNum ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_02, END_PROC );
    }

    /* 요청마다 페이지를 매번 계산하는 구조는 좋지 않다
       매번 계산하지 않는 구조가 필요하며 기본적인 방법으로
       센터 또는 게임 서버에서 랭크 리스트가 갱신될 때마다 페이지를 재계산 하도록 수정한다
       특히나 이전 시즌 랭크 리스트는 자주 변하지 않기 때문에 미리 계산해두는 방법의 효율이 좋다 */
    nStartIndex = kPacket.m_nPageNum * nRankEntryPerPage;
    lit = lstCurrentDungeonRankList.begin();
    for ( int i = 0; i < nStartIndex; ++i ) {
        ++lit;
    }

    if ( nTotalPage == kPacket.m_nPageNum ) {
        nLastPageEntries = lstCurrentDungeonRankList.size() % nRankEntryPerPage;
    }

    for ( int i = 0; i < nLastPageEntries; ++i ) {
        if ( lstCurrentDungeonRankList.end() == lit ) {
            break;
        }

        kPacket.m_vecDungeonRankList.push_back( *lit );
        ++lit;
    }

    if ( true == kPacket.m_vecDungeonRankList.empty() ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_02, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_DUNGEON_RANK_01,              kPacket.m_nOK, NetError::ERR_DUNGEON_RANK_01 ); // 랭킹을 처리하는 던전이 아님.
        ERR_CASE( ERR_DUNGEON_RANK_02,              kPacket.m_nOK, NetError::ERR_DUNGEON_RANK_02 ); // 해당 페이지의 랭킹 정보가 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"던전 랭킹 현재 시즌 랭크 리스트 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK || NetError::ERR_DUNGEON_RANK_02 == kPacket.m_nOK, L"던전 랭킹 현재 시즌 랭크 리스트 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nPageNum )
        << BUILD_LOG( kPacket.m_nModeID )
        << BUILD_LOG( kPacket.m_vecDungeonRankList.size() )
        << END_LOG;

    SEND_PACKET( EVENT_DUNGEON_RANK_CURRENT_SEASON_RANK_PAGE_ACK );
}

IMPL_ON_FUNC( EVENT_DUNGEON_RANK_CURRENT_SEASON_USER_RANK_REQ )
{
    // kPacket_: ModeID

    KEVENT_DUNGEON_RANK_CURRENT_SEASON_USER_RANK_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cCharType = GetCurrentCharType();
    kPacket.m_nModeID = kPacket_;

    std::list< KDungeonRankInfo > lstCurrentDungeonRankList;
    std::list< KDungeonRankInfo >::iterator lit;

    SET_ERROR( ERR_UNKNOWN );

    SiKDungeonRankManager()->GetCurrentDungeonRankList( kPacket.m_nModeID, lstCurrentDungeonRankList );

    lit = std::find_if( lstCurrentDungeonRankList.begin(), lstCurrentDungeonRankList.end(),
        boost::bind( &KDungeonRankInfo::m_dwLoginUID, _1 ) == GetUID()
        && boost::bind( &KDungeonRankInfo::m_cCharType, _1 ) == kPacket.m_cCharType );

    if ( lstCurrentDungeonRankList.end() == lit ) {
        kPacket.m_kDungeonRankPersonal.m_nRank = -1;
    }
    else {
        kPacket.m_kDungeonRankPersonal = *lit;
    }

    SET_ERROR( NET_OK );

//END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, NetError::NET_OK ); // 성공.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"던전 랭킹 현재 시즌 유저 랭크 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK, L"던전 랭킹 현재 시즌 유저 랭크 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nModeID )
        << BUILD_LOGc( kPacket.m_cCharType )
        << BUILD_LOG( kPacket.m_kDungeonRankPersonal.m_nStageNum )
        << BUILD_LOG( kPacket.m_kDungeonRankPersonal.m_dwStageClearTime )
        << BUILD_LOG( kPacket.m_kDungeonRankPersonal.m_wstrNickName )
        << END_LOG;

    SEND_PACKET( EVENT_DUNGEON_RANK_CURRENT_SEASON_USER_RANK_ACK );
}

IMPL_ON_FUNC( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_RANK_PAGE_REQ )
{
    // kPacket_: < ModeID, PageNum > // 첫 페이지 번호를 0으로 한다

    KEVENT_DUNGEON_RANK_PREVIOUS_SEASON_RANK_PAGE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nModeID = kPacket_.first;
    kPacket.m_nPageNum = kPacket_.second;

    std::list< KDungeonRankInfo > lstPreviousDungeonRankList;
    std::list< KDungeonRankInfo >::iterator lit;
    int nRankEntryPerPage = SiKDungeonRankManager()->GetRankEntryPerPage();
    int nTotalPage = 0;
    int nLastPageEntries = nRankEntryPerPage;
    int nStartIndex = 0;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKDungeonRankManager()->IsRankingModes( kPacket.m_nModeID ) ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_01, END_PROC );
    }

    SiKDungeonRankManager()->GetPreviousDungeonRankList( kPacket.m_nModeID, lstPreviousDungeonRankList );

    if ( true == lstPreviousDungeonRankList.empty() ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_02, END_PROC );
    }

    nTotalPage = lstPreviousDungeonRankList.size() / nRankEntryPerPage;
    if ( nTotalPage < kPacket.m_nPageNum ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_02, END_PROC );
    }

    /* 요청마다 페이지를 매번 계산하는 구조는 좋지 않다
       매번 계산하지 않는 구조가 필요하며 기본적인 방법으로
       센터 또는 게임 서버에서 랭크 리스트가 갱신될 때마다 페이지를 재계산 하도록 수정한다
       특히나 이전 시즌 랭크 리스트는 자주 변하지 않기 때문에 미리 계산해두는 방법의 효율이 좋다 */
    nStartIndex = kPacket.m_nPageNum * nRankEntryPerPage;
    lit = lstPreviousDungeonRankList.begin();
    for ( int i = 0; i < nStartIndex; ++i ) {
        ++lit;
    }

    if ( nTotalPage == kPacket.m_nPageNum ) {
        nLastPageEntries = lstPreviousDungeonRankList.size() % nRankEntryPerPage;
    }

    for ( int i = 0; i < nLastPageEntries; ++i ) {
        if ( lstPreviousDungeonRankList.end() == lit ) {
            break;
        }

        kPacket.m_vecDungeonRankList.push_back( *lit );
        ++lit;
    }

    if ( true == kPacket.m_vecDungeonRankList.empty() ) {
        SET_ERR_GOTO( ERR_DUNGEON_RANK_02, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_DUNGEON_RANK_01,              kPacket.m_nOK, NetError::ERR_DUNGEON_RANK_01 ); // 랭킹을 처리하는 던전이 아님.
        ERR_CASE( ERR_DUNGEON_RANK_02,              kPacket.m_nOK, NetError::ERR_DUNGEON_RANK_02 ); // 해당 페이지의 랭킹 정보가 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"던전 랭킹 이전 시즌 랭크 리스트 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK || NetError::ERR_DUNGEON_RANK_02 == kPacket.m_nOK, L"던전 랭킹 이전 시즌 랭크 리스트 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nPageNum )
        << BUILD_LOG( kPacket.m_nModeID )
        << BUILD_LOG( kPacket.m_vecDungeonRankList.size() )
        << END_LOG;

    SEND_PACKET( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_RANK_PAGE_ACK );
}

IMPL_ON_FUNC( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_REQ )
{
    // kPacket_: ModeID

    KEVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cCharType = GetCurrentCharType();
    kPacket.m_nModeID = kPacket_;

    PAIR_CHAR_INT prKey( kPacket.m_cCharType, kPacket.m_nModeID );
    std::map< PAIR_CHAR_INT, KDungeonRankInfo >::iterator mit;

    SET_ERROR( ERR_UNKNOWN );

    mit = m_mapPreviousDungeonRankPersonal.find( prKey );
    if ( m_mapPreviousDungeonRankPersonal.end() == mit ) {
        kPacket.m_kDungeonRankPersonal.m_nRank = -1;
    }
    else {
        kPacket.m_kDungeonRankPersonal = mit->second;
    }

    SET_ERROR( NET_OK );

//END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, NetError::NET_OK ); // 성공.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"던전 랭킹 이전 시즌 유저 랭크 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK, L"던전 랭킹 이전 시즌 유저 랭크 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nModeID )
        << BUILD_LOGc( kPacket.m_cCharType )
        << BUILD_LOG( kPacket.m_kDungeonRankPersonal.m_nRank )
        << BUILD_LOG( kPacket.m_kDungeonRankPersonal.m_nStageNum )
        << BUILD_LOG( kPacket.m_kDungeonRankPersonal.m_dwStageClearTime )
        << BUILD_LOG( kPacket.m_kDungeonRankPersonal.m_wstrNickName )
        << END_LOG;

    SEND_PACKET( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_ACK );
}

IMPL_ON_FUNC( DB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_ACK )
{
    m_mapPreviousDungeonRankPersonal = kPacket_.m_mapPreviousDungeonRankPersonal;

    _LOG_SUCCESS( NetError::NET_OK == kPacket_.m_nOK, L"던전 랭킹 이전 시즌 유저 랭크 DB 조회 성공, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_mapPreviousDungeonRankPersonal.size() )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_UPDATED_NOT )
{
    QUEUEING_ID_TO_DB( DB_EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_REQ ); // 던전 랭킹 시스템 이전 시즌 유저 랭크 요청
}

IMPL_ON_FUNC( EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ )
{
    KEVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nSlotID = kPacket_.m_nSlotID;

    KDB_EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ kDBPacket;
    kDBPacket.m_nSlotID = kPacket_.m_nSlotID;
    kDBPacket.m_lstAttributeList.clear();

    KItemPtr pkTargetItem;
    KItemPtr pkUseItem;
    int nItemLevel = 0;
    std::list< std::pair< int, float > > lstResultAttributeList;
    std::vector< KAttributeInfo >::iterator vit;
    std::list< std::pair< int, float > >::iterator lit;

    SET_ERROR( ERR_UNKNOWN );

    // 대상 아이템과 주문서 아이템이 인벤에 있는지 확인
    pkTargetItem = m_kInventory.FindItemByItemUID( kPacket_.m_dwTargetItemUID );
    if ( NULL == pkTargetItem  ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_01, END_PROC );
    }

    if ( true == pkTargetItem->CheckItemType( KItem::TYPE_LOOK ) ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_04, END_PROC );
    }

    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwSingleRandomAttributeItemID );
    if ( NULL == pkUseItem ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_02, END_PROC );
    }

    kDBPacket.m_kTargetItem = *pkTargetItem;
    kDBPacket.m_kUseItem = *pkUseItem;

    // 해당 주문서를 대상 아이템에 사용할 수 있는지 확인(대상 아이템의 레벨 기준)
    nItemLevel = static_cast< int >( pkTargetItem->m_sEquipLevel );
    if ( 0 >= nItemLevel ) { // 혹시나 장착 레벨 정보가 아이템 컨테이너에 없는 경우를 대비해서 그럴경우 소트 정보를 이용하도록 보정을 한다
        DWORD dwItemLevel = 0;
        SiKItemManager()->GetItemEquipLevel( pkTargetItem->m_ItemID, dwItemLevel );
        nItemLevel = static_cast< int >( dwItemLevel );
    }

    if ( false == SiKItemManager()->m_kAttributeDecider.CheckSingleRandomAttributeItemLevel( static_cast< DWORD >( nItemLevel ), pkUseItem->m_ItemID ) ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_03, END_PROC );
    }

    // 대상 아이템에 랜덤돌릴 속성이 존재하는지 확인
    vit = std::find_if( pkTargetItem->m_vecAttribute.begin(), pkTargetItem->m_vecAttribute.end(),
        boost::bind( &KAttributeInfo::m_cSlotID, _1 ) == kPacket_.m_nSlotID );
    if ( pkTargetItem->m_vecAttribute.end() == vit ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_05, END_PROC );
    }

    /* 대상 아이템의 속성 중 변경되지 않을 속성 리스트를 모아두고
       이후 새 속성을 뽑을 때 이들을 제외시키는 용도로 사용한다
       추가로 DB 스레드에 최종 속성 리스트를 넘기고 다시 유저 스레드에서 사용할 수 있도록 활용한다 */
    for ( vit = pkTargetItem->m_vecAttribute.begin(); pkTargetItem->m_vecAttribute.end() != vit; ++vit ) {
        if ( vit->m_cSlotID != kPacket_.m_nSlotID ) {
            kDBPacket.m_lstAttributeList.push_back( std::make_pair( static_cast< int >( vit->m_cType ), vit->m_fValue ) );
        }
    }

    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkTargetItem->m_ItemID );
    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkTargetItem->m_ItemID );

    if ( true == SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_SELECTIVE );
    }

    // 이 로직은 한 개 속성의 랜덤 변경만을 고려하며 속성의 수를 확장하는 것을 고려하지 않았다
    if ( false == SiKItemManager()->m_kAttributeDecider.GetSelectiveAttributeExceptList( nAttributeTableID, 1, static_cast< int >( pkTargetItem->m_cGradeID ), kDBPacket.m_lstAttributeList, lstResultAttributeList ) ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_06, END_PROC );
    }

    kDBPacket.m_lstAttributeList.insert( kDBPacket.m_lstAttributeList.begin(), lstResultAttributeList.begin(), lstResultAttributeList.end() );

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, NetError::NET_OK );
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_01,   kPacket.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_01 ); // 속성을 변경할 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_02,   kPacket.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_02 ); // 개별 랜덤 속성 주문서 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_03,   kPacket.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_03 ); // 대상 아이템이 개별 랜덤 속성 주문서를 적용할 수 있는 레벨이 아님.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_04,   kPacket.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_04 ); // 대상 아이템이 개별 랜덤 속성 주문서를 사용할 수 없는 아이템 타입임.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_05,   kPacket.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_05 ); // 대상 아이템에 랜덤으로 변경할 선택된 속성이 없음.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_06,   kPacket.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_06 ); // 대상 아이템에 적용할 속성을 선택하지 못함.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"개별 랜덤 속성 주문서 사용 요청 처리 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::NET_OK != kPacket.m_nOK ) {
        SEND_PACKET( EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_ACK );
    }

    _LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK, L"개별 랜덤 속성 주문서 사용 요청 MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nSlotID )
        << BUILD_LOG( kPacket_.m_dwTargetItemUID )
        << BUILD_LOG( kPacket_.m_dwSingleRandomAttributeItemID )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_ACK )
{
    KItemPtr pkTargetItem;
    KItemPtr pkSingleRandomAttributeItem;

    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkTargetItem = m_kInventory.FindItemByItemUID( kPacket_.m_kTargetItem.m_ItemUID );
    if ( NULL == pkTargetItem ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_01, END_PROC );
    }

    // 속성 초기화한 아이템 리스트.
    pkTargetItem->m_vecAttribute = kPacket_.m_kTargetItem.m_vecAttribute;
    kPacket_.m_kTargetItem = *pkTargetItem;

    // 사용한 속성 초기화 아이템 갯수 차감.
    pkSingleRandomAttributeItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kSingleRandomAttributeItem.m_ItemUID );
    if ( NULL == pkSingleRandomAttributeItem ) {
        SET_ERR_GOTO( ERR_SINGLE_RANDOM_ATTRIBUTE_02, END_PROC );
    }

    pkSingleRandomAttributeItem->m_nCount = kPacket_.m_kSingleRandomAttributeItem.m_nCount;
    pkSingleRandomAttributeItem->m_nInitCount = kPacket_.m_kSingleRandomAttributeItem.m_nCount;

    if ( NULL != pkSingleRandomAttributeItem && 0 >= pkSingleRandomAttributeItem->m_nCount ) {
        m_kInventory.RemoveItem( kPacket_.m_kSingleRandomAttributeItem.m_ItemID, kPacket_.m_kSingleRandomAttributeItem.m_ItemUID );
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, NetError::NET_OK );
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_01,   kPacket_.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_01 ); // 속성을 변경할 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_02,   kPacket_.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_02 ); // 개별 랜덤 속성 주문서 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_07,   kPacket_.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_07 ); // DB에 기록 중 기록하려는 속성 정보가 없음.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_08,   kPacket_.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_08 ); // 대상 아이템의 속성정보 DB 기록 실패.
        ERR_CASE( ERR_SINGLE_RANDOM_ATTRIBUTE_09,   kPacket_.m_nOK, NetError::ERR_SINGLE_RANDOM_ATTRIBUTE_09 ); // 개별 랜덤 속성 아이템의 서버 DB 수량 동기화 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 );
    default:
        START_LOG( cerr, L"개별 랜덤 속성 주문서 사용 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_ACK );

    _LOG_SUCCESS( NetError::NET_OK == kPacket_.m_nOK, L"개별 랜덤 속성 주문서 사용 응답 처리 Msg:" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kTargetItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kTargetItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kTargetItem.m_vecAttribute.size() )
        << BUILD_LOG( kPacket_.m_kSingleRandomAttributeItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kSingleRandomAttributeItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kSingleRandomAttributeItem.m_nCount )
        << END_LOG;
}
