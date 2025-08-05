#include "MSUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"
#include "MsgServer.h"
#include "MSSimLayer.h"
#include "MSDBLayer.h"
#include "NetError.h"
#include "MRPacket.h"
#include "SlangFilter.h"
#include "support_MSUser.h"
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include "Log4.h"

NiImplementRTTI( KMSUser, KActor );
ImplementException( KMSUser );

KMSUserPtr KMSUser::Create()
{
    return SiKMSSimLayer()->m_kActorManager.CreateActor<KMSUser>();
}

void KMSUser::AddManager( KMSUserPtr spMSUser_ )
{
    JIF( spMSUser_ );
    JIF( SiKMSSimLayer()->m_kActorManager.Add( spMSUser_ ) );
}

KMSUser::KMSUser(void)
:m_iLocationNum(-1)
,m_bGamming(false)
,m_cChatRoomCount(0)
,m_bHaveNewSentPaper(true)
,m_bHaveNewRecvPaper(true)
,m_dwEmoticon(1)
,m_strCharNickName(L"")
{
    m_mapFriendGroup.clear();
    m_mapChatRoomInfo.clear();
    m_mapSentPaperBox.clear();
    m_mapRecvPaperBox.clear();

    //Tick 설정
    for( int i = 0 ; i < TI_MAX ; ++i )
    {
        SetTick( i );
    }

    SetTickGap( TI_SEND_USER_INFO, 10 * 1000 ); //10초마다 유저 정보 리모트 큐잉.
    SetTickGap( TI_LOAD_SENT_PAPERBOX, 3 * 1000 );
    SetTickGap( TI_LOAD_RECV_PAPERBOX, 3 * 1000 );
    m_tmLoginTime = CTime::GetCurrentTime();
}

KMSUser::~KMSUser(void)
{
}

ImplToStringW( KMSUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KMSUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
    CASE( EMS_S2_VERIFY_ACCOUNT_REQ );
    CASE( EMS_S2_VERIFY_ACCOUNT_ACK );
    CASE( EMS_S2_ADD_FRIEND_REQ );
    CASE( EMS_S2_ADD_FRIEND_ACK );
    CASE( EMS_S2_ADD_FRIEND_NOT );
   _CASE( EMS_S2_ACCEPT_FRIEND_NOT, KFriend );
    CASE( EMS_S2_DELETE_FRIEND_REQ );
    CASE( EMS_S2_DELETE_FRIEND_ACK );
   _CASE( EMS_S2_ACCEPT_FRIEND_REQ, DWORD );
    CASE( EMS_S2_ACCEPT_FRIEND_ACK );
    CASE( EMS_S2_UPDATE_FRIENDS_NOT );
   _CASE( EMS_S2_UPDATE_FRIEND_NOT, KFriend );
   _CASE( EMS_S2_REJECT_FRIEND_REQ, DWORD );
   _CASE( EMS_S2_REJECT_FRIEND_ACK, DWORD );
   _CASE( EMS_S2_REJECT_FRIEND_NOT, DWORD );
   _CASE( EMS_S2_UPDATE_MYDATA_NOT, KFriend );
    CASE( EMS_S2_LOGINNED_FRIEND_LIST_NOT );
    CASE( EMS_S2_BLOCK_FRIEND_REQ );
    CASE( EMS_S2_BLOCK_FRIEND_ACK );
   _CASE( EMS_S2_UNBLOCK_FRIEND_REQ, DWORD );
    CASE( EMS_S2_UNBLOCK_FRIEND_ACK );
    CASE( EMS_S2_BLOCK_MOVE_FRIEND_REQ );
    CASE( EMS_S2_BLOCK_MOVE_FRIEND_ACK );
   _CASE( EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, DWORD );
    CASE( EMS_S2_UNBLOCK_MOVE_FRIEND_ACK );

   _CASE( EMS_S2_LIVE_DATA_REQ, DWORD );
   _CASE( EMS_S2_LOGON_REPORT_REQ, int );
   _CASE( EMS_S2_FRIEND_NOT_EXIST_NOT, DWORD );
   // 친구그룹
   _CASE( EMS_S2_ADD_GROUP_REQ, std::wstring );
    CASE( EMS_S2_ADD_GROUP_ACK );
   _CASE( EMS_S2_DELETE_GROUP_REQ, int );
    CASE( EMS_S2_DELETE_GROUP_ACK );
    CASE( EMS_S2_RENAME_GROUP_REQ );
    CASE( EMS_S2_RENAME_GROUP_ACK );
    CASE( EMS_S2_MOVE_FRIEND_TO_GROUP_REQ );
    CASE( EMS_S2_MOVE_FRIEND_TO_GROUP_ACK );
   // 채팅하기
   _CASE( EMS_S2_START_CHAT_REQ, std::set<DWORD> );
    CASE( EMS_S2_START_CHAT_NOT );
    CASE( EMS_S2_INVITE_CHAT_MEMBER_REQ );
    CASE( EMS_S2_UPDATE_CHAT_MEMBER_NOT );
    CASE( EMS_S2_LEAVE_CHAT_MEMBER_REQ );
    CASE( EMS_S2_LEAVE_CHAT_MEMBER_NOT );
   _CASE( EMS_S2_SEND_CHAT_REQ, KChatMsg );
   _CASE( EMS_S2_SEND_CHAT_NOT, KChatMsg );
    CASE( EMS_S2_UPDATE_MY_CHAT_COUNT_NOT );
   // 이모티콘 설정
   _CASE( EMS_S2_SET_EMOTICON_REQ, DWORD );
    CASE( EMS_S2_SET_EMOTICON_ACK );
   //     // 길드원 목록 갱신
   //     CASE( EMS_S2_UPDATE_GUILD_MEMBER_REQ );
   //     CASE( EMS_S2_UPDATE_GUILD_MEMBER_NOT );
   //     // 길드미션완료 자동알림쪽지
   //     CASE( EMS_S2_AUTO_GUILD_MESSAGE_REQ );
   //     CASE( EMS_S2_AUTO_GUILD_MESSAGE_NOT );
   // 쪽지
    CASE( EMS_S2_SEND_PAPER_REQ );
    CASE( EMS_S2_SEND_PAPER_ACK );
    CASE( EMS_S2_NEW_PAPER_NOT );

    CASE_NOPARAM( EMS_S2_LOAD_SENT_PAPERBOX_REQ );
    CASE( EMS_S2_UPDATE_SENT_PAPERBOX_NOT );
    CASE_NOPARAM( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
    CASE( EMS_S2_UPDATE_RECV_PAPERBOX_NOT );

   _CASE( EMS_S2_READ_PAPER_REQ, std::set<DWORD> );
    CASE( EMS_S2_READ_PAPER_ACK );
   _CASE( EMS_S2_DEL_SENT_PAPER_REQ, std::set<DWORD> );
    CASE( EMS_S2_DEL_SENT_PAPER_ACK );
   _CASE( EMS_S2_DEL_RECV_PAPER_REQ, std::set<DWORD> );
    CASE( EMS_S2_DEL_RECV_PAPER_ACK );
   _CASE( EMS_S2_SEAL_PAPER_REQ, std::set<DWORD> );
    CASE( EMS_S2_SEAL_PAPER_ACK );

    CASE( EMS_S2_ADD_FRIEND_FORCE_REQ );
   _CASE( EMS_S2_ADD_FRIEND_FORCE_ACK, KEMS_S2_ADD_FRIEND_FORCE_REQ );
   _CASE( EMS_S2_ADD_FRIEND_FORCE_NOT, KFriend );

   _CASE( EMS_ADD_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq );
   _CASE( EMS_ADD_GUILD_MEMBER_LIST_ACK, KUpdateGuildMemberAck );
   _CASE( EMS_DEL_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq );

    CASE( EMS_GUILD_ALL_MEMBER_LIST_REQ );
    CASE( EMS_GUILD_ALL_MEMBER_LIST_ACK );

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetEventIDString() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime ) << END_LOG;
    }
}

void KMSUser::Tick()
{
    KActor::Tick();

    // SendToMyFriends 함수가 호출될 때,
    if( ::GetTickCount() - GetTick( TI_SEND_USER_INFO ) > GetTickGap( TI_SEND_USER_INFO ) )
    {
        //SendToMyFriends( m_friendInfo );    // 1. EMS_UPDATE_MYDATA_NOT 2. 여기(tick),예약되었을 때
                                            // 2의 경우는 1에서 바로 보내지 못했을 경우(10초미만) 에 해당

        SendToMyFriends_S2( m_friend );
        SetTick( TI_SEND_USER_INFO );
    }

    switch( GetStateID() ) {
    case KDefaultFSM::STATE_EXIT:
        SiKMSSimLayer()->m_kActorManager.ReserveDelete( m_strName );    // 자기 자신을 스스로 삭제할 수 없음.
    	break;
    }
}

void KMSUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() ) {
    case KDefaultFSM::STATE_CONNECTED:
    case KDefaultFSM::STATE_LOGINED:
        {
            // MsgRouter에 등록된 정보를 해제해 주어야 한다.
            SiKMRConnector()->SendPacket( m_dwUID, 
                KMREvent::EMR_CLIENT_DISCONNECT_REPORT_REQ, m_dwUID );

            // 로그인중인 친구들에게 나의 로그아웃을 알려야 한다.
            LeaveAllChatRoom();
            KFriend kMyInfo = GetMyFriendInfo_S2();
            kMyInfo.m_bIsOnline = false;
            SendToMyFriends_S2( kMyInfo, false );

            if( GetStateID() == KDefaultFSM::STATE_LOGINED && SiKMsgServer()->m_bWriteLogoutLog ) // 로그인이 성공해야만..
                LogoutLog();

            StateTransition( KDefaultFSM::INPUT_EXIT_GAME );    // OnDestroy()가 불린 현재 KSession::Tick 처리 중이며,
            // KMSUser::Tick 처리 단계에서 종료 대기명단에 등록된다.
        }
        break;
    }
}
//-----------------------------------------------------------------------------------------------------

IMPL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_CONNECTED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strLogin ) << END_LOG;

    SET_ERROR( ERR_UNKNOWN );

    // 동일 서버내 이중접속 여부 확인
    KMSUserPtr spUser( SiKMSSimLayer()->m_kActorManager.GetByUID<KMSUser>( kPacket_.m_dwUserUID ) );

    if( spUser != NULL )
    {
        int kPacket = 1; // 동일서버간 이중접속은 1, 다른 서버간 이중접속은 2
        SendPacket( KMSEvent::EMS_KICK_NOT, kPacket );
        spUser->SendPacket( KMSEvent::EMS_KICK_NOT, kPacket );

        spUser->ReserveDestroy();   // 이전 접속자를 종료하는 부분

        SET_ERR_GOTO( ERR_VERIFY_04, end_proc ); // 동일서버간 이중접속 시도
    }

    SetUID( kPacket_.m_dwUserUID );

    // 동일 이름이 없다면 이름 변경.
    LIF( SiKMSSimLayer()->m_kActorManager.Rename( GetName(), kPacket_.m_strLogin ) );
    LIF( SiKMSSimLayer()->m_kActorManager.RegByUID( *this ) );

    // db작업 요청 (이름 변경 후 해준다)
    QUEUEING_EVENT_TO_DB( EMS_S2_VERIFY_ACCOUNT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

    m_ReservedLoc_S2.m_dwUserUID        = kPacket_.m_dwUserUID;
    m_ReservedLoc_S2.m_strNickName      = m_strNickName         = kPacket_.m_strNickName;
    m_ReservedLoc_S2.m_strNickColor     = m_strNickColor        = kPacket_.m_strNickColor;
    m_ReservedLoc_S2.m_strCharNickName  = m_strCharNickName     = kPacket_.m_strCharNickName;
    m_ReservedLoc_S2.m_bIsGamming       = m_bGamming            = kPacket_.m_bGamming;
    m_ReservedLoc_S2.m_iLocationNum     = m_iLocationNum        = kPacket_.m_iLocationNum;
    m_ReservedLoc_S2.m_strLocation      = m_strLocation         = kPacket_.m_strLocation;
    m_ReservedLoc_S2.m_strServerName    = m_strServerName       = kPacket_.m_strServerName;

    // 유저 채널링 타입 저장.
    SetUserChannelType( kPacket_.m_dwChannelType );

end_proc:
    KEMS_S2_VERIFY_ACCOUNT_ACK kPacket;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_VERIFY_04, kPacket.m_nOK, 1 );
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"메신저 접속. 알 수 없는 오류 , Name : " << kPacket_.m_strLogin )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( m_strNickColor )
        << BUILD_LOG( m_strCharNickName )
        << BUILD_LOG( m_strServerName )
        << BUILD_LOG( m_strLocation )
        << BUILD_LOG( m_iLocationNum )
        << BUILD_LOG( m_bGamming )
        << BUILD_LOG( kPacket_.m_nGuildID )
        << BUILD_LOG( kPacket_.m_nLanguageCode )  << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        SEND_PACKET( EMS_S2_VERIFY_ACCOUNT_ACK );
        //접속종료.
        ReserveDestroy();
    }
}

IMPL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_CONNECTED ));

    LOG_CONDITION( kPacket_.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_mapFriendGroup.size() )
        << BUILD_LOG( kPacket_.m_kBlockGroup.m_mapMembers.size() )
        << BUILD_LOG( kPacket_.m_kGuildGroup.m_mapMembers.size() )
        << BUILD_LOG( kPacket_.m_bNewPaperAlarm )
        << BUILD_LOG( kPacket_.m_dwEmoticon ) << END_LOG;

    LIF( SendPacket( KMSEvent ::EMS_S2_VERIFY_ACCOUNT_ACK, kPacket_, true, true ) ); // 압축 하도록 수정

    if( kPacket_.m_nOK != 0 )
    {
        ReserveDestroy();
        return;
    }

    //인증 성공한 이후 진행
    StateTransition( KDefaultFSM::INPUT_VERIFICATION_OK );

    InitChatRoom();

    //{{그룹핑 확인용
    std::map<int,KGroup>::iterator mit;
    for( mit = kPacket_.m_mapFriendGroup.begin() ; mit != kPacket_.m_mapFriendGroup.end() ; ++mit )
    {
        START_LOG_WITH_NAME( clog )
            << BUILD_LOG( mit->second.m_nGroupKey )
            << BUILD_LOG( mit->second.m_strGroupName )
            << BUILD_LOG( mit->second.m_mapMembers.size() ) << END_LOG;
    }
    //}
    m_kBlockGroup    = kPacket_.m_kBlockGroup;
    m_mapFriendGroup = kPacket_.m_mapFriendGroup;
    m_kGuildGroup    = kPacket_.m_kGuildGroup;
    m_dwEmoticon     = kPacket_.m_dwEmoticon;
    m_ReservedLoc_S2.m_dwEmoticon = kPacket_.m_dwEmoticon;

    // 라우터에 등록 요청을 한다.
    int nInvalidID = -1;    // 초기접속의 경우 무효한 MR id를 넣어준다. (무조건 보고함)
    QueueingEvent( KMSEvent::EMS_S2_LOGON_REPORT_REQ, nInvalidID );

    // 로그인 성공 시간 기록.
    m_tmLoginTime = CTime::GetCurrentTime();

    START_LOG( clog, L"라우터에 등록요청 Nick : " << m_strNickName ) << END_LOG;
}

IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_bIsBlock )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket_.m_strMessage ) << END_LOG;

    wchar_t strProhibition[] ={ L'\'',};
    // 쿼리에 사용할수 없는 문자 포함(빈칸, 탭, \')
    if( std::find_first_of( kPacket_.m_strNickName.begin(), kPacket_.m_strNickName.end(),
        strProhibition, strProhibition + ( sizeof(strProhibition) / sizeof(wchar_t) ) ) != kPacket_.m_strNickName.end() )
    {
        SET_ERR_GOTO( ERR_MSN_25, err_proc );
    }

    // 자기 자신을 추가하려고 함
    if( m_strNickName == kPacket_.m_strNickName ) {
        SET_ERR_GOTO( ERR_MSN_06, err_proc );
    }

    // 친구 리스트 제한인원수 초과
    if( GetAllFriendCount() >= MAX_FRIEND_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_08, err_proc );
    }

    // 이미 내 친구이다.
    if( IsMyFriend( kPacket_.m_strNickName ) == true )
    {
        SET_ERR_GOTO( ERR_MSN_07, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_ADD_FRIEND_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_ADD_FRIEND_ACK kPacket;
    kPacket.m_nOK = -99;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_06, kPacket.m_nOK, 6 ); // 자기 자신을 추가하려고 함
        ERR_CASE( ERR_MSN_07, kPacket.m_nOK, 7 ); // 이미 내 친구이다.
        ERR_CASE( ERR_MSN_08, kPacket.m_nOK, 8 ); // 친구 리스트 제한인원수 초과
        ERR_CASE( ERR_MSN_25, kPacket.m_nOK, 25 );// 쿼리에 사용할수 없는 문자 포함(빈칸, 탭, \')
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"친구추가 요청. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( MAX_FRIEND_LIMIT )
        << BUILD_LOG( GetAllFriendCount() )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket_.m_nCharType )
        << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_ADD_FRIEND_ACK );
}

IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 10 || kPacket_.m_nOK == 100 )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_strNickName )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_strServerName )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_bIsOnline ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_S2_ADD_FRIEND_ACK );

    if ( kPacket_.m_nOK < 0 )
        return;

    //친구 등록 요청 성공시 진행
    std::map<int,KGroup>::iterator mit;
    mit = m_mapFriendGroup.find( KGroup::FRIEND_BASE );
    if( mit == m_mapFriendGroup.end() )
    {
        KGroup kGroup;
        kGroup.m_nGroupKey = KGroup::FRIEND_BASE;
        kGroup.m_strGroupName = L"BasicFriend";
        // 기본 그룹 없을시에 생성
        mit = m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
    }
    // 친구 추가는 우선적으로 기본 그룹에 넣는다.
    mit->second.m_mapMembers.insert( std::make_pair( kPacket_.m_kFriendInfo.m_dwUserUID, kPacket_.m_kFriendInfo ) );

    KEMS_S2_ADD_FRIEND_NOT kNot;
    kNot.m_dwSenderUID      = GetUID();
    kNot.m_strSenderNick    = m_strNickName;
    kNot.m_strMessage       = kPacket_.m_strMessage;

    // 친구 등록 대기상태가 됨.
    if( kPacket_.m_nOK == 0 )
    {
        REMOTE_QUEUEING( kPacket_.m_kFriendInfo.m_dwUserUID, EMS_S2_ADD_FRIEND_NOT, kNot );
        START_LOG(clog, L"현재 접속한 유저에게 친구 요청 메세지를 날리다." )
            << BUILD_LOG( m_strNickName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kNot.m_dwSenderUID )
            << BUILD_LOG( kNot.m_strSenderNick )
            << BUILD_LOG( kNot.m_strMessage ) << END_LOG;
    }
    else if( kPacket_.m_nOK == 10 )
    {
        // 다이렉트 친구가 됨.
        kPacket_.m_kFriendInfo.m_bIsFriend = true;
        // live inform.
        REMOTE_QUEUEING( kPacket_.m_kFriendInfo.m_dwUserUID, EMS_S2_UPDATE_FRIEND_NOT, GetMyFriendInfo_S2() );//내정보를 알리고,
        REMOTE_QUEUEING( kPacket_.m_kFriendInfo.m_dwUserUID, EMS_S2_LIVE_DATA_REQ, m_dwUID );//상대의 정보를 요청합자
        START_LOG(clog, L"다이렉트로 친구 맺어짐" )
            << BUILD_LOG( m_strNickName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kNot.m_dwSenderUID )
            << BUILD_LOG( kNot.m_strSenderNick )
            << BUILD_LOG( kNot.m_strMessage )
            << BUILD_LOG( kPacket_.m_kFriendInfo.m_strNickName )
            << BUILD_LOG( kPacket_.m_kFriendInfo.m_dwUserUID ) << END_LOG;
    }
    else if( kPacket_.m_nOK == 100 )
    {
        START_LOG( clog, L"이전에 내가 보낸 친구초청메세지가 있다." )
            << BUILD_LOG( m_strNickName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kNot.m_dwSenderUID )
            << BUILD_LOG( kNot.m_strSenderNick )
            << BUILD_LOG( kNot.m_strMessage )
            << BUILD_LOG( kPacket_.m_kFriendInfo.m_strNickName )
            << BUILD_LOG( kPacket_.m_kFriendInfo.m_dwUserUID ) << END_LOG;
    }
}

IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_strSenderNick )
        << BUILD_LOG( kPacket_.m_strMessage )
        << BUILD_LOG( IsBlockedFriend_S2( kPacket_.m_dwSenderUID ) ) << END_LOG;

    // block이 아니고 기존 내친구가 아니면 친구추가 알림 메세지를 보낸다.
    if( !IsBlockedFriend_S2( kPacket_.m_dwSenderUID ) && !IsMyFriend( kPacket_.m_dwSenderUID ) )
    {
        SEND_RECEIVED_PACKET( EMS_S2_ADD_FRIEND_NOT );
        START_LOG( clog, L"친구 추가 요청 알림 메세지 받아서 보냈음" ) << END_LOG;
    }
}

IMPL_ON_FUNC( EMS_S2_DELETE_FRIEND_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nGroupID )
        << BUILD_LOG( kPacket_.m_dwFriendUID ) << END_LOG;

    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    if( kPacket_.m_nGroupID == KGroup::NORMAL_BLOCK || kPacket_.m_nGroupID == KGroup::FRIEND_BLOCK )
    {
        mit2 = m_kBlockGroup.m_mapMembers.find( kPacket_.m_dwFriendUID );
        if( mit2 == m_kBlockGroup.m_mapMembers.end() )
        {
            //해당 그룹에 없는 유저.
            SET_ERR_GOTO( ERR_MSN_27, err_proc );
        }
    }
    else
    {
        mit = m_mapFriendGroup.find( kPacket_.m_nGroupID );
        if( mit == m_mapFriendGroup.end() )
        {
            //잘못된 그룹 아이디
            SET_ERR_GOTO( ERR_MSN_11, err_proc );
        }

        mit2 = mit->second.m_mapMembers.find( kPacket_.m_dwFriendUID );
        if( mit2 == mit->second.m_mapMembers.end() )
        {
            //해당 그룹에 없는 유저.
            SET_ERR_GOTO( ERR_MSN_27, err_proc );
        }
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_DELETE_FRIEND_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_DELETE_FRIEND_ACK kPacket;
    kPacket.m_nGroupID = kPacket_.m_nGroupID;
    kPacket.m_dwFriendUID = kPacket_.m_dwFriendUID;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_11, kPacket.m_nOK, 11 ); // 해당 그룹이 없음
        ERR_CASE( ERR_MSN_27, kPacket.m_nOK, 27 ); // 해당 그룹에 존재하지 않는 유저임
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"친구삭제 요청. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nGroupID )
        << BUILD_LOG( kPacket.m_dwFriendUID )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_DELETE_FRIEND_ACK );
}

IMPL_ON_FUNC( EMS_S2_DELETE_FRIEND_ACK )
{
    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nGroupID)
        << BUILD_LOG( kPacket_.m_dwFriendUID )
        << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        std::map<int,KGroup>::iterator mit;
        std::map<DWORD,KFriend>::iterator mit2;

        // 앞에서 이미 걸러낸 상황이지만 예외 처리는 해놓자.
        if( kPacket_.m_nGroupID == KGroup::NORMAL_BLOCK || kPacket_.m_nGroupID == KGroup::FRIEND_BLOCK )
        {
            mit2 = m_kBlockGroup.m_mapMembers.find( kPacket_.m_dwFriendUID );
            if( mit2 == m_kBlockGroup.m_mapMembers.end() )
            {
                START_LOG( cwarn, L"그룹안에 없는 유저입니다." ) << END_LOG;
            }
            else
            {
                // 자신의 리스트에 있는 데이터를 삭제한다.
                m_kBlockGroup.m_mapMembers.erase( mit2 );
            }
        }
        else
        {
            mit = m_mapFriendGroup.find( kPacket_.m_nGroupID );
            if( mit == m_mapFriendGroup.end() )
            {
                START_LOG( cwarn, L"해당 그룹이 없습니다." ) << END_LOG;
            }
            else
            {
                mit2 = mit->second.m_mapMembers.find( kPacket_.m_dwFriendUID );
                if( mit2 == mit->second.m_mapMembers.end() )
                {
                    START_LOG( cwarn, L"그룹안에 없는 유저입니다." ) << END_LOG;
                }
                else
                {
                    // 자신의 리스트에 있는 데이터를 삭제한다.
                    mit->second.m_mapMembers.erase( mit2 );
                }
            }
        }

        //상대에게 내가 오프라인으로 보이게.
        KFriend kMyInfo = GetMyFriendInfo_S2();
        kMyInfo.m_bIsOnline = false;

        REMOTE_QUEUEING( kPacket_.m_dwFriendUID, EMS_S2_UPDATE_FRIEND_NOT, kMyInfo );
    }

    SEND_RECEIVED_PACKET( EMS_S2_DELETE_FRIEND_ACK );
}

_IMPL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_REQ, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( GetAllFriendCount() ) << END_LOG;

    if( kPacket_ == GetUID() )
    {
        //자신을 추가하거나 삭제할 수 없음
        SET_ERR_GOTO( ERR_MSN_06, err_proc );
    }

    if( IsMyFriend( kPacket_ ) )
    {
        //이미 나의 친구이다.
        SET_ERR_GOTO( ERR_MSN_07, err_proc );
    }

    if( GetAllFriendCount() >= MAX_FRIEND_LIMIT )
    {
        //친구수 이미 Max다.
        SET_ERR_GOTO( ERR_MSN_08, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_ACCEPT_FRIEND_REQ, kPacket_ );
    SET_ERROR( NET_OK );
err_proc:
    KEMS_S2_ACCEPT_FRIEND_ACK kPacket;
    kPacket.m_nOK = -99;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_06, kPacket.m_nOK, 6 ); // 자신을 추가하거나 삭제할 수 없음
        ERR_CASE( ERR_MSN_07, kPacket.m_nOK, 7 ); // 이미 요청중이거나 친구이거나 블록상태임
        ERR_CASE( ERR_MSN_08, kPacket.m_nOK, 8 ); // 친구리스트 제한인원수 초과
        default: 
            kPacket.m_nOK = -99;
            START_LOG( cerr, L"친구삭제 요청. 알수 없는 오류 , Name : " << m_strName )
                << BUILD_LOG( NetError::GetLastNetError() )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if( kPacket.m_nOK == 8)
    {
        // DB처리부터는 친구 요청을 거절했을때와 동일한 작업을 수행한다. (reject ack는 유저에게 가지 않음)
        QUEUEING_EVENT_TO_DB( EMS_S2_REJECT_FRIEND_REQ, kPacket_ );
    }

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_ACCEPT_FRIEND_ACK );
}

IMPL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_strNickName )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_strServerName )
        << BUILD_LOG( kPacket_.m_kFriendInfo.m_bIsOnline ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_S2_ACCEPT_FRIEND_ACK );

    // OK값이 0이 아니라면 더이상 진행하지 않는다.
    if( kPacket_.m_nOK != 0 ) return;

    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    mit = m_mapFriendGroup.find( KGroup::FRIEND_BASE );
    if( mit == m_mapFriendGroup.end() )
    {
        KGroup kGroup;
        kGroup.m_nGroupKey = KGroup::FRIEND_BASE;
        mit = m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
    }

    if( mit->second.m_mapMembers.insert( std::make_pair( kPacket_.m_kFriendInfo.m_dwUserUID, kPacket_.m_kFriendInfo ) ).second == false )
    {
        START_LOG( cwarn, L"이미 등록된 친구입니다" ) << END_LOG;
    }

    // live inform.
    REMOTE_QUEUEING( kPacket_.m_kFriendInfo.m_dwUserUID, EMS_S2_ACCEPT_FRIEND_NOT, GetMyFriendInfo_S2() );
}

_IMPL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_NOT, KFriend )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket_.m_bIsOnline )
        << BUILD_LOG( kPacket_.m_bIsFriend )
        << BUILD_LOG( kPacket_.m_bIsGamming )
        << BUILD_LOG( kPacket_.m_iLocationNum )
        << BUILD_LOG( kPacket_.m_strServerName )
        << BUILD_LOG( kPacket_.m_dwEmoticon )
        << BUILD_LOG( kPacket_.m_cChatRoomCount ) << END_LOG;

    // 나의 친구요청을 수락한 유저의 정보 업데이트.
    LIF( UpdateFriendInfo_S2( kPacket_ ) );

    //블럭유저이면 오프라인으로 보이도록.
    KFriend kMyInfo = GetMyFriendInfo_S2();
    if( !IsMyFriend( kPacket_.m_dwUserUID ) || IsBlockedFriend_S2( kPacket_.m_dwUserUID ) )
    {
        kMyInfo.m_bIsOnline = false;
    }
    //상대에게 내 live 정보 보냄
    REMOTE_QUEUEING( kPacket_.m_dwUserUID, EMS_S2_UPDATE_FRIEND_NOT, kMyInfo );

    if( !IsBlockedFriend_S2( kPacket_.m_dwUserUID ) )
    {
        //상대에게 받은 정보
        SEND_RECEIVED_PACKET( EMS_S2_UPDATE_FRIEND_NOT );
    }
}

IMPL_ON_FUNC( EMS_S2_UPDATE_FRIENDS_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( GetAllFriendCount() ) << END_LOG;

    std::map< int, std::map<DWORD,KFriend> >::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        for( mit2 = mit->second.begin() ; mit2 != mit->second.end() ; ++mit2 )
        {
            UpdateFriendInfo_S2( mit2->second );
        }
    }

    SEND_RECEIVED_PACKET( EMS_S2_UPDATE_FRIENDS_NOT );
}

_IMPL_ON_FUNC( EMS_S2_UPDATE_FRIEND_NOT, KFriend )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket_.m_strNickColor )
        << BUILD_LOG( kPacket_.m_strServerName )
        << BUILD_LOG( kPacket_.m_strLocation)
        << BUILD_LOG( kPacket_.m_iLocationNum )
        << BUILD_LOG( kPacket_.m_bIsFriend )
        << BUILD_LOG( kPacket_.m_bIsOnline )
        << BUILD_LOG( kPacket_.m_bIsGamming )
        << BUILD_LOG( GetAllFriendCount() ) << END_LOG;

    if( UpdateFriendInfo_S2( kPacket_ ) == false )
    {
        //상대가 날 일방적으로 친구로 알고 있을 경우,
        START_LOG( clog, L"내 친구정보에 없는 아이다" ) << END_LOG;
        return;
    }

    SEND_RECEIVED_PACKET( EMS_S2_UPDATE_FRIEND_NOT );
}

_IMPL_ON_FUNC( EMS_S2_REJECT_FRIEND_REQ, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( GetAllFriendCount() ) << END_LOG;

    QUEUEING_EVENT_TO_DB( EMS_S2_REJECT_FRIEND_REQ, kPacket_ );
}
_IMPL_ON_FUNC( EMS_S2_REJECT_FRIEND_ACK, DWORD )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( m_dwUID ) << END_LOG;

    REMOTE_QUEUEING( kPacket_, EMS_S2_REJECT_FRIEND_NOT, m_dwUID );
}

_IMPL_ON_FUNC( EMS_S2_REJECT_FRIEND_NOT, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    std::map<int,KGroup>::iterator mit;
    mit = m_mapFriendGroup.find(KGroup::FRIEND_BASE);
    if( mit == m_mapFriendGroup.end() )
    {
        KGroup kGroup;
        kGroup.m_nGroupKey = KGroup::FRIEND_BASE;
        m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) );
        // 친구 기본 그룹이 생성되어 있지 않은 경우(있을수 없지만 예외처리)
        return;
    }

    if( mit->second.m_mapMembers.find( kPacket_ ) == mit->second.m_mapMembers.end() )
    {
        START_LOG( cwarn, L"친구요청 거절자 정보를 리스트에서 찾을 수 없음." )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( m_strNickName )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( GetAllFriendCount() )
            << BUILD_LOG( kPacket_ ) << END_LOG;
        return;
    }

    // 삭제 후 알림
    mit->second.m_mapMembers.erase( kPacket_ );
    SEND_RECEIVED_PACKET( EMS_S2_REJECT_FRIEND_NOT );
}

_IMPL_ON_FUNC( EMS_S2_UPDATE_MYDATA_NOT, KFriend )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strNickName )
        << BUILD_LOG( kPacket_.m_strServerName )
        << BUILD_LOG( kPacket_.m_bIsOnline )
        << BUILD_LOG( kPacket_.m_bIsFriend )
        << BUILD_LOG( kPacket_.m_strLocation )
        << BUILD_LOG( kPacket_.m_iLocationNum )
        << BUILD_LOG( kPacket_.m_bIsGamming ) << END_LOG;

    // 변경 가능한 정보만을 변경하도록 하자.
    m_strNickName       = kPacket_.m_strNickName;
    m_strServerName     = kPacket_.m_strServerName;
    m_strLocation       = kPacket_.m_strLocation;
    m_iLocationNum      = kPacket_.m_iLocationNum;
    m_bGamming          = kPacket_.m_bIsGamming;
    //m_dwEmoticon        = kPacket_.m_dwEmoticon;//이모티콘 정보는 서버에서 관리한다.

    SendToMyFriends_S2( GetMyFriendInfo_S2() );       // 변경된 정보로 갱신한 후 mr에 보고.
}

IMPL_ON_FUNC( EMS_S2_LOGINNED_FRIEND_LIST_NOT )
{
    KFriend kMyInfo = GetMyFriendInfo_S2();

    std::map< int, std::map<DWORD,KFriend> >::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        for( mit2 = mit->second.begin() ; mit2 != mit->second.end() ; ++mit2 )
        {
            // 상대방 정보를 받아 갱신하고, 내블럭리스트에 해당되지 않으면.
            if( UpdateFriendInfo_S2( mit2->second ) && !IsBlockedFriend_S2( mit2->first ) )
            {
                // 상대방에게 내 상태 전송
                REMOTE_QUEUEING( mit2->first, EMS_S2_UPDATE_FRIEND_NOT, kMyInfo );
            }
        }
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( GetAllFriendCount() ) << END_LOG;

    // 유저에게도 친구 정보 전송.
    SEND_RECEIVED_PACKET( EMS_S2_UPDATE_FRIENDS_NOT );
}

IMPL_ON_FUNC( EMS_S2_BLOCK_FRIEND_REQ )
{
    VERIFY_STATE( ( 1, KDefaultFSM::STATE_LOGINED ) );
    SET_ERROR( ERR_UNKNOWN );

    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    // 닉네임 검사 사용할 수 있는 스트링인지?
    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strNickName) )
    {
        SET_ERR_GOTO( ERR_MSN_25, err_proc );
    }

    if( m_kBlockGroup.m_mapMembers.size() >= MAX_BLOCK_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_08, err_proc ); // 제한 인원수 초과
    }

    if( IsBlockedFriend_S2( kPacket_.m_strNickName ) == true )
    {
        SET_ERR_GOTO( ERR_MSN_07, err_proc ); // 이미 블럭임
    }

    // 친구 목록에 있는 아이인지 검사.
    for( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit )
    {
        for( mit2 = mit->second.m_mapMembers.begin() ; mit2 != mit->second.m_mapMembers.end() ; ++mit2 )
        {
            if( mit2->second.m_strNickName == kPacket_.m_strNickName )
                break;
        }
    }

    // 내 친구 목록중에 있고,
    if( mit != m_mapFriendGroup.end() )
    {
        //얘가 내 친구이면.
        if( mit2->second.m_bIsFriend == true )
        {
            DWORD kDBPacket;
            kDBPacket = mit2->second.m_dwUserUID;

            QUEUEING_EVENT_TO_DB( EMS_S2_BLOCK_MOVE_FRIEND_REQ, kDBPacket );
        }
    }
    else
    {
        QUEUEING_EVENT_TO_DB( EMS_S2_BLOCK_FRIEND_REQ, kPacket_ );
    }

    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_BLOCK_FRIEND_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwBlockUID = 0;        // 접속중이 아니면 DB를 거쳐야 값이 나와요.
    kPacket.m_strBlockNick = kPacket_.m_strNickName;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_07, kPacket.m_nOK, 7 ); // 이미 블럭임
        ERR_CASE( ERR_MSN_08, kPacket.m_nOK, 8 ); // 제한 인원수 초과
        ERR_CASE( ERR_MSN_25, kPacket.m_nOK, 25 ); // 쿼리에 사용할수 없는 문자 포함(빈칸, 탭, \')
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"블럭 요청. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_strNickName ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_BLOCK_FRIEND_ACK );
}

IMPL_ON_FUNC( EMS_S2_BLOCK_FRIEND_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwBlockUID )
        << BUILD_LOG( kPacket_.m_strBlockNick ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        KFriend kBlockUser;
        kBlockUser.m_dwUserUID = kPacket_.m_dwBlockUID;
        kBlockUser.m_strNickName = kPacket_.m_strBlockNick;
        kBlockUser.m_bIsFriend = false; // 쌩블록

        // 블럭 리스트에 추가
        if( m_kBlockGroup.m_mapMembers.insert( std::make_pair( kBlockUser.m_dwUserUID, kBlockUser ) ).second == false )
        {
            START_LOG( cwarn, L"이미 블럭 리스트에 등록되어 있음." )
                << BUILD_LOG( kPacket_.m_dwBlockUID )
                << BUILD_LOG( kPacket_.m_strBlockNick ) << END_LOG;
        }

        // live inform. block당한 유저가 접속되어 있다면 나의 정보를 logout으로 변경하게 만든다.
        KFriend kMyInfo = GetMyFriendInfo_S2();
        kMyInfo.m_bIsOnline = false;

        REMOTE_QUEUEING( kPacket_.m_dwBlockUID, EMS_S2_UPDATE_FRIEND_NOT, kMyInfo );
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_dwBlockUID )
        << BUILD_LOG( kPacket_.m_strBlockNick ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_S2_BLOCK_FRIEND_ACK );
}

_IMPL_ON_FUNC( EMS_S2_UNBLOCK_FRIEND_REQ, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    std::map<DWORD,KFriend>::iterator mit;
    mit = m_kBlockGroup.m_mapMembers.find( kPacket_ );
    //이전에 친구였는지 검사해서..
    if( mit == m_kBlockGroup.m_mapMembers.end() )
    {
        START_LOG( cwarn, L"블럭 리스트에 없음." )
            << BUILD_LOG( kPacket_ ) << END_LOG;
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_UNBLOCK_FRIEND_REQ, kPacket_ );
}

IMPL_ON_FUNC( EMS_S2_UNBLOCK_FRIEND_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwBlockUID ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        if( m_kBlockGroup.m_mapMembers.find( kPacket_.m_dwBlockUID ) != m_kBlockGroup.m_mapMembers.end() )
        {
            // 쌩블럭 해제 = 삭제
            // 블럭 리스트에서 삭제
            m_kBlockGroup.m_mapMembers.erase( kPacket_.m_dwBlockUID );
        }
        else
        {
            START_LOG( cwarn, L"블럭 리스트에 없는 유저 : " << kPacket_.m_dwBlockUID ) << END_LOG;
        }
        //지우고 끝이다.
    }

    SEND_RECEIVED_PACKET( EMS_S2_UNBLOCK_FRIEND_ACK );
}

IMPL_ON_FUNC( EMS_S2_BLOCK_MOVE_FRIEND_REQ )
{
    VERIFY_STATE( ( 1, KDefaultFSM::STATE_LOGINED ) );
    SET_ERROR( ERR_UNKNOWN );

    std::map<int,KGroup>::iterator mit;
    mit = m_mapFriendGroup.find( kPacket_.m_nGroupID );

    if( mit == m_mapFriendGroup.end() )
    {
        SET_ERR_GOTO( ERR_MSN_11, err_proc ); // 해당하는 그룹이 없음
    }

    if( mit->second.m_mapMembers.find( kPacket_.m_dwFriendUID ) == mit->second.m_mapMembers.end() )
    {
        SET_ERR_GOTO( ERR_MSN_27, err_proc ); // 그룹내 해당유저를 찾을 수 없음
    }

    if( m_kBlockGroup.m_mapMembers.find( kPacket_.m_dwFriendUID ) != m_kBlockGroup.m_mapMembers.end() )
    {
        SET_ERR_GOTO( ERR_MSN_16, err_proc ); // 해당 그룹에 이미 등록된 유저임
    }

    if( m_kBlockGroup.m_mapMembers.size() >= MAX_BLOCK_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_08, err_proc ); // 제한 인원수 초과
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_BLOCK_MOVE_FRIEND_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_BLOCK_MOVE_FRIEND_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID = kPacket_.m_nGroupID;
    kPacket.m_dwFriendUID = kPacket_.m_dwFriendUID;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_08, kPacket.m_nOK, 8 );  // 제한 인원수 초과
        ERR_CASE( ERR_MSN_11, kPacket.m_nOK, 11 ); // 해당하는 그룹이 없음
        ERR_CASE( ERR_MSN_27, kPacket.m_nOK, 27 ); // 그룹내 해당유저를 찾을 수 없음
        ERR_CASE( ERR_MSN_16, kPacket.m_nOK, 16 ); // 해당 그룹에 이미 등록된 유저임
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"블럭 요청. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nGroupID )
        << BUILD_LOG( kPacket_.m_dwFriendUID ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_BLOCK_MOVE_FRIEND_ACK );
}

IMPL_ON_FUNC( EMS_S2_BLOCK_MOVE_FRIEND_ACK )
{
    VERIFY_STATE( ( 1, KDefaultFSM::STATE_LOGINED ) );

    if( kPacket_.m_nOK == 0 )
    {
        std::map<int,KGroup>::iterator mit;
        std::map<DWORD,KFriend>::iterator mit2;

        mit = m_mapFriendGroup.find( kPacket_.m_nGroupID );

        // req에서 이미 걸렀지만 일단 예외처리
        if( mit == m_mapFriendGroup.end() )
        {
            START_LOG( cwarn, L"해당하는 그룹이 없음" ) << END_LOG;
        }
        else
        {
            mit2 = mit->second.m_mapMembers.find( kPacket_.m_dwFriendUID );

            if( mit2 == mit->second.m_mapMembers.end() )
            {
                START_LOG( cwarn, L"그룹내 해당유저를 찾을 수 없음" ) << END_LOG;

                KFriend kFriend;
                kFriend.m_dwUserUID = kPacket_.m_dwFriendUID;
                //유저 UID로 유저 정보를 받아오는걸 찾아오는 함수가 없다면 만들어서 넣을까..
                m_kBlockGroup.m_mapMembers.insert( std::make_pair( kFriend.m_dwUserUID, kFriend ) );
            }
            else
            {
                // block list insert
                mit2->second.m_bIsFriend = true; //친구블록.
                m_kBlockGroup.m_mapMembers.insert( std::make_pair( mit2->second.m_dwUserUID, mit2->second ) );
                // 이전 친구 그룹에서 삭제
                mit->second.m_mapMembers.erase( kPacket_.m_dwFriendUID );
            }

            // live inform. block당한 유저가 접속되어 있다면 나의 정보를 logout으로 변경하게 만든다.
            KFriend kMyInfo = GetMyFriendInfo_S2();
            kMyInfo.m_bIsOnline = false;

            REMOTE_QUEUEING( kPacket_.m_dwFriendUID, EMS_S2_UPDATE_FRIEND_NOT, kMyInfo );
            START_LOG( clog, L"내쪽에서 친구를 절교했다." ) << END_LOG;
        }
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwFriendUID)
        << BUILD_LOG( kPacket_.m_nGroupID ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_S2_BLOCK_MOVE_FRIEND_ACK );
}

_IMPL_ON_FUNC( EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, DWORD )
{
    VERIFY_STATE( ( 1, KDefaultFSM::STATE_LOGINED ) );
    SET_ERROR( ERR_UNKNOWN );

    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::const_iterator cmit2;

    // 기본 그룹이 없으면 생성
    mit = m_mapFriendGroup.find( KGroup::FRIEND_BASE );
    if( mit == m_mapFriendGroup.end() )
    {
        KGroup kGroup;
        kGroup.m_nGroupKey = KGroup::FRIEND_BASE;
        mit = m_mapFriendGroup.insert( std::make_pair( KGroup::FRIEND_BASE, kGroup ) ).first;
    }

    if( mit->second.m_mapMembers.find( kPacket_ ) != mit->second.m_mapMembers.end() )
    {
        SET_ERR_GOTO( ERR_MSN_16, err_proc ); // 기본 그룹에 이미 등록되어 있음
    }

    cmit2 = m_kBlockGroup.m_mapMembers.find( kPacket_ );
    if( cmit2 == m_kBlockGroup.m_mapMembers.end() )
    {
        SET_ERR_GOTO( ERR_MSN_27, err_proc ); // 블럭 그룹에 존재하지 않는 유저임
    }

    if( GetAllFriendCount() >= MAX_FRIEND_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_08, err_proc ); // 친구 최대수를 초과합니다.
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_UNBLOCK_MOVE_FRIEND_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID = KGroup::FRIEND_BASE;
    kPacket.m_dwFriendUID = kPacket_;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_08, kPacket.m_nOK, 8 );  // 친구 최대수를 초과합니다.
        ERR_CASE( ERR_MSN_11, kPacket.m_nOK, 11 ); // 해당하는 그룹이 없음
        ERR_CASE( ERR_MSN_16, kPacket.m_nOK, 16 ); // 해당 그룹에 이미 등록된 유저임
        ERR_CASE( ERR_MSN_27, kPacket.m_nOK, 27 ); // 그룹내 해당유저를 찾을 수 없음
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"블럭 요청. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_UNBLOCK_MOVE_FRIEND_ACK );
}

IMPL_ON_FUNC( EMS_S2_UNBLOCK_MOVE_FRIEND_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwFriendUID ) << END_LOG;

    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    if( kPacket_.m_nOK == 0 )
    {
        std::map<DWORD,KFriend>::iterator mit;
        std::map<int,KGroup>::iterator mit2;

        // req에서 이미 걸렀지만 일단 예외처리
        mit = m_kBlockGroup.m_mapMembers.find( kPacket_.m_dwFriendUID );
        if( mit == m_kBlockGroup.m_mapMembers.end() )
        {
            START_LOG( cwarn, L"해당 그룹에 존재하지 않는 유저임" ) << END_LOG;
        }
        else
        {
            mit2 = m_mapFriendGroup.find( kPacket_.m_nGroupID );

            if( mit2 == m_mapFriendGroup.end() )
            {
                START_LOG( cwarn, L"해당하는 그룹이 없음" ) << END_LOG;

                KGroup kGroup;
                kGroup.m_nGroupKey = kPacket_.m_nGroupID;
                mit2 = m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
            }

            if( mit2->second.m_mapMembers.find( kPacket_.m_dwFriendUID ) != mit2->second.m_mapMembers.end() )
            {
                START_LOG( cwarn, L"해당 그룹에 이미 등록되어 있음" ) << END_LOG;
            }
            else
            {
                // 친구그룹으로 복귀
                mit2->second.m_mapMembers.insert( std::make_pair( mit->second.m_dwUserUID, mit->second ) );
                // delete block user
                m_kBlockGroup.m_mapMembers.erase( mit->second.m_dwUserUID );
            }
        }

        KFriend kMyInfo = GetMyFriendInfo_S2();

        REMOTE_QUEUEING( kPacket_.m_dwFriendUID, EMS_S2_UPDATE_FRIEND_NOT, kMyInfo );//상대에게 내 정보를 갱신시키고
        REMOTE_QUEUEING( kPacket_.m_dwFriendUID, EMS_S2_LIVE_DATA_REQ, m_dwUID );//상대의 정보를 요청한다.
    }

    SEND_RECEIVED_PACKET( EMS_S2_UNBLOCK_MOVE_FRIEND_ACK );
}

_IMPL_ON_FUNC( EMS_S2_LIVE_DATA_REQ, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( IsBlockedFriend_S2( kPacket_ ) ) << END_LOG;

    KFriend kMyInfo = GetMyFriendInfo_S2( kPacket_ );

    if( !IsMyFriend( kPacket_ ) )
    {
        START_LOG( cwarn, L"친구 리스트에 존재하지 않은 UID에게서 상태문의 요청을 받음." )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( GetAllFriendCount() ) << END_LOG;

        //내 친구가 아니니까 온라인 상태가 아니라고 알려주자.
        kMyInfo.m_bIsOnline = false;
    }

    //내정보를 원하는 친구에게 던져주자.
    REMOTE_QUEUEING( kPacket_, EMS_S2_UPDATE_FRIEND_NOT, kMyInfo );
}

// ms self : 유저 로그인 혹은 MR과 재접속된 경우 서버가 자기자신에게 신호.
_IMPL_ON_FUNC( EMS_S2_LOGON_REPORT_REQ, int )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    KEMR_CLIENT_CONNECT_REPORT_REQ kPacket;
    kPacket.m_dwLogonUID = GetUID();

    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    for( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit )
    {
        std::set<DWORD> setFriendUID;

        GetSendableFriends( mit->second.m_mapMembers, setFriendUID, true, false, false );

        kPacket.m_mapFriendGroup.insert( std::make_pair( mit->first, setFriendUID ) );
    }

    START_LOG( clog, L"등록된 라우터 수")
        << BUILD_LOG( SiKMRConnector()->GetRouterCount() ) << END_LOG;

    if( !SiKMRConnector()->IsUsingRouter() )  // 라우터를 사용하지 않는 경우.
    {
        SiKMSSimLayer()->GetLoginnedFriendGroupInfo_S2( kPacket );
        return;
    }

    if( kPacket_ == -1 ||   // 유저가 초기 접속한 경우. 특정 MR id가 없다면 무조건 보고한다.
        SiKMRConnector()->GetOwnerMRid( m_dwUID ) == kPacket_ )   // MR과의 연결 재접속.
    {
        START_LOG( clog, L"MR과의 연결 재접속 Nick : " << m_strNickName ) << END_LOG;
        LIF( SiKMRConnector()->SendPacket( m_dwUID, 
            KMREvent::EMR_CLIENT_CONNECT_REPORT_REQ, kPacket ) );
    }
}

_IMPL_ON_FUNC( EMS_S2_FRIEND_NOT_EXIST_NOT, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( cwarn )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( IsBlockedFriend_S2( kPacket_ ) ) << END_LOG;

    std::map<int,KGroup>::iterator mit;
    std::map<DWORD,KFriend>::iterator mit2;

    for( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit )
    {
        mit2 = mit->second.m_mapMembers.find( kPacket_ );

        if( mit2 != mit->second.m_mapMembers.end() )
        {
            break;
        }
    }

    if( mit == m_mapFriendGroup.end() )
    {
        START_LOG( cerr, L"친구목록에서 정보수정 대상자를 찾을 수 없음. UID : " << kPacket_ ) << END_LOG;
        return;
    }

    mit2->second.m_bIsOnline = false;

    LIF( SendPacket( KMSEvent::EMS_S2_UPDATE_FRIEND_NOT, mit2->second ) );

    //대화시 좀비 유저 처리.
    KMyChatRoom::const_iterator cmit;
    //채팅방 리스트에서 해당 유저를 찾아 떠났다고 대신 알려주자.
    for ( cmit = m_mapChatRoomInfo.begin() ; cmit != m_mapChatRoomInfo.end() ; ++cmit ) {
        //채팅방에 있는 유저인지?
        if ( cmit->second.find( kPacket_ ) == cmit->second.end() ) {
            continue;
        }

        std::vector<DWORD> vecSendUIDs;
        //패킷을 보낼 채팅방 UID 리스트를 뽑아내자
        std::map<DWORD,std::pair<std::wstring,std::wstring>>::const_iterator cmitUser;
        for ( cmitUser = cmit->second.begin() ; cmitUser != cmit->second.end() ; ++cmitUser ) {
            //떠날 아이의 UID는 제외
            if ( cmitUser->first != kPacket_ ) {
                vecSendUIDs.push_back( cmitUser->first );
            }
        }

        if ( !vecSendUIDs.empty() ) {
            KEMS_S2_LEAVE_CHAT_MEMBER_NOT kPacket;
            kPacket.m_prRoomKey = cmit->first;
            kPacket.m_dwLeaveUID = kPacket_;

            //그 채팅방을 떠났다고 통보
            REMOTE_QUEUEING_PACK( vecSendUIDs, EMS_S2_LEAVE_CHAT_MEMBER_NOT, kPacket );
            START_LOG( cwarn, L"대화시 실제로 로그아웃인 유저 배제함" )
                << BUILD_LOG( kPacket.m_prRoomKey.first )
                << BUILD_LOG( kPacket.m_prRoomKey.second )
                << BUILD_LOG( kPacket.m_dwLeaveUID ) << END_LOG;
        }
    }
}

// 친구그룹
_IMPL_ON_FUNC( EMS_S2_ADD_GROUP_REQ, std::wstring )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    std::map<int,KGroup>::const_iterator cmit;

    KEMS_S2_ADD_GROUP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID = -1;
    kPacket.m_strGroupName = kPacket_;

    //최소 기본 그룹 1개도 없는 경우.
    if( m_mapFriendGroup.empty() )
    {
        //DB에 유저 기본그룹이 등록 안되어 있거나, DB에서 제대로 데이터를 못 받았다.
        SET_ERR_GOTO( ERR_MSN_33, err_proc );
    }

    //기본 그룹이 없다.
    if( m_mapFriendGroup.size() == 1 && m_mapFriendGroup.find( KGroup::FRIEND_BASE ) == m_mapFriendGroup.end() )
    {
        SET_ERR_GOTO( ERR_MSN_33, err_proc );
    }

    // 그룹 최대수 초과
    if( m_mapFriendGroup.size() >= MAX_GROUP_LIMIT)
    {
        SET_ERR_GOTO( ERR_MSN_12, err_proc );
    }

    // 그룹명 길이 초과
    if( kPacket_.size() > MAX_GROUP_NAME_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_13, err_proc );
    }

    // 쿼리 사용불가 문구 거르기
    static wchar_t strSqlProhibition[] ={ L'\t', L'\'', L'\"', L',', L"!" };
    if( std::find_first_of( kPacket_.begin(), kPacket_.end(),
        strSqlProhibition, strSqlProhibition + ( sizeof(strSqlProhibition) / sizeof(wchar_t) ) ) != kPacket_.end() )
    {
        SET_ERR_GOTO( ERR_MSN_25, err_proc );
    }

    // 동일한 이름의 그룹이 있음
    for( cmit = m_mapFriendGroup.begin() ; cmit != m_mapFriendGroup.end() ; ++cmit )
    {
        if( cmit->second.m_strGroupName == kPacket_ )
        {
            SET_ERR_GOTO( ERR_MSN_10, err_proc );
            break;
        }
    }

    // 비어있는 그룹번호 구하기
    int nGroupNum = KGroup::FRIEND_BASE;
    for( cmit = m_mapFriendGroup.begin() ; cmit != m_mapFriendGroup.end() ; ++cmit )
    {
        // 유저생성 그룹이외에는 예외 처리
        if ( cmit->first < KGroup::FRIEND_BASE )
            continue;

        if ( cmit->first != nGroupNum ) {
            break;
        }

        ++nGroupNum;
    }

    //그룹 번호 초과(있을 수 없음)
    if( nGroupNum > KGroup::FRIEND_BASE + MAX_GROUP_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_12, err_proc );
    }
    //그룹 번호 부여.
    kPacket.m_nGroupID = nGroupNum;

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB( EMS_S2_ADD_GROUP_REQ, kPacket );

err_proc:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_10, kPacket.m_nOK, 10 ); // 해당 그룹이 존재함
        ERR_CASE( ERR_MSN_12, kPacket.m_nOK, 12 ); // 최대 그룹수 초과
        ERR_CASE( ERR_MSN_13, kPacket.m_nOK, 13 ); // 그룹명 문자열 길이 초과
        ERR_CASE( ERR_MSN_25, kPacket.m_nOK, 25 ); // 쿼리에 사용할수 없는 문자 포함(빈칸, 탭, \')
        ERR_CASE( ERR_MSN_33, kPacket.m_nOK, 33 ); // 친구 기본그룹이 없음.
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"메신저 친구그룹 추가. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_ADD_GROUP_ACK );
}

IMPL_ON_FUNC( EMS_S2_ADD_GROUP_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGroupID )
        << BUILD_LOG( kPacket_.m_strGroupName ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        KGroup kGroup;
        kGroup.m_nGroupKey = kPacket_.m_nGroupID;
        kGroup.m_strGroupName = kPacket_.m_strGroupName;
        // Create Group
        if( m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).second == false )
        {
            START_LOG( cwarn, L"이미 존재하는 그룹입니다 GroupID : " << kGroup.m_nGroupKey ) << END_LOG;
        }
    }

    SEND_RECEIVED_PACKET( EMS_S2_ADD_GROUP_ACK );
}

_IMPL_ON_FUNC( EMS_S2_DELETE_GROUP_REQ, int )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    if( kPacket_ == KGroup::FRIEND_BASE )
    {
        SET_ERR_GOTO( ERR_MSN_15, err_proc );
    }

    if( kPacket_ < KGroup::FRIEND_BASE ||
        kPacket_ > KGroup::FRIEND_BASE + MAX_GROUP_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_11, err_proc );
    }

    if( m_mapFriendGroup.find( kPacket_ ) == m_mapFriendGroup.end() )
    {
        SET_ERR_GOTO( ERR_MSN_11, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_DELETE_GROUP_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_DELETE_GROUP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID = kPacket_;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_15, kPacket.m_nOK, 15 ); // 기본그룹을 삭제할 수 없음
        ERR_CASE( ERR_MSN_11, kPacket.m_nOK, 11 ); // 해당그룹이 없음
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"메신저 친구그룹 삭제. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_DELETE_GROUP_ACK );
}

IMPL_ON_FUNC( EMS_S2_DELETE_GROUP_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGroupID )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        if( m_mapFriendGroup.find( kPacket_.m_nGroupID ) == m_mapFriendGroup.end() )
        {
            START_LOG( cerr, L"해당그룹이 없음 GroupID : " << kPacket_.m_nGroupID ) << END_LOG;
        }
        else
        {
            // delete group
            m_mapFriendGroup.erase( kPacket_.m_nGroupID );
            START_LOG( clog, L"해당그룹 삭제 GroupID : " << kPacket_.m_nGroupID ) << END_LOG;
        }
    }

    SEND_RECEIVED_PACKET( EMS_S2_DELETE_GROUP_ACK );
}

IMPL_ON_FUNC( EMS_S2_RENAME_GROUP_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );
    std::map<int,KGroup>::const_iterator cmit;

    if( kPacket_.m_nGroupID == KGroup::FRIEND_BASE )
    {
        SET_ERR_GOTO( ERR_MSN_15, err_proc );
    }

    cmit = m_mapFriendGroup.find( kPacket_.m_nGroupID );
    if( cmit == m_mapFriendGroup.end() )
    {
        SET_ERR_GOTO( ERR_MSN_11, err_proc );
    }

    if( kPacket_.m_strNewGroupName.size() > MAX_GROUP_NAME_LIMIT )
    {
        SET_ERR_GOTO( ERR_MSN_13, err_proc );
    }

    // 쿼리 사용불가 문구 거르기
    static wchar_t strSqlProhibition[] ={ L'\t', L'\'', L'\"', L',', L"!" };
    if( std::find_first_of( kPacket_.m_strNewGroupName.begin(), kPacket_.m_strNewGroupName.end(),
        strSqlProhibition, strSqlProhibition + ( sizeof(strSqlProhibition) / sizeof(wchar_t) ) ) != kPacket_.m_strNewGroupName.end() )
    {
        SET_ERR_GOTO( ERR_MSN_25, err_proc );
    }

    // 동일한 이름의 그룹이 있음
    for( cmit = m_mapFriendGroup.begin() ; cmit != m_mapFriendGroup.end() ; ++cmit )
    {
        if( cmit->second.m_strGroupName == kPacket_.m_strNewGroupName )
        {
            SET_ERR_GOTO( ERR_MSN_10, err_proc );
            break;
        }
    }

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB( EMS_S2_RENAME_GROUP_REQ, kPacket_ );

err_proc:
    KEMS_S2_RENAME_GROUP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGroupID = kPacket_.m_nGroupID;
    kPacket.m_strGroupName = kPacket_.m_strNewGroupName;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_10, kPacket.m_nOK, 10 ); // 해당 그룹이 존재함
        ERR_CASE( ERR_MSN_11, kPacket.m_nOK, 11 ); // 해당그룹이 없음
        ERR_CASE( ERR_MSN_13, kPacket.m_nOK, 13 ); // 그룹명 문자열 길이 초과
        ERR_CASE( ERR_MSN_15, kPacket.m_nOK, 15 ); // 기본그룹의 이름을 변경할 수 없음
        ERR_CASE( ERR_MSN_25, kPacket.m_nOK, 25 ); // 쿼리에 사용할수 없는 문자 포함
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"메신저 친구그룹명 변경. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGroupID )
        << BUILD_LOG( kPacket.m_strGroupName ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_RENAME_GROUP_ACK );
}

IMPL_ON_FUNC( EMS_S2_RENAME_GROUP_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGroupID )
        << BUILD_LOG( kPacket_.m_strGroupName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        std::map<int,KGroup>::iterator mit;

        mit = m_mapFriendGroup.find( kPacket_.m_nGroupID );
        if( mit == m_mapFriendGroup.end() )
        {
            START_LOG( cwarn, L"해당그룹이 없음 GroupID : " << kPacket_.m_nGroupID ) << END_LOG;
        }
        else
        {
            // rename group
            mit->second.m_strGroupName = kPacket_.m_strGroupName;
        }
    }

    SEND_RECEIVED_PACKET( EMS_S2_RENAME_GROUP_ACK );
}

IMPL_ON_FUNC( EMS_S2_MOVE_FRIEND_TO_GROUP_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    std::map<int,KGroup>::const_iterator cmit;
    cmit = m_mapFriendGroup.find( kPacket_.m_nFromGroupID );
    if( cmit == m_mapFriendGroup.end() )
    {
        SET_ERR_GOTO( ERR_MSN_11, err_proc );
    }

    if( cmit->second.m_mapMembers.find( kPacket_.m_dwFriendUID ) == cmit->second.m_mapMembers.end() )
    {
        SET_ERR_GOTO( ERR_MSN_27, err_proc );
    }

    cmit = m_mapFriendGroup.find( kPacket_.m_nToGroupID );
    if( cmit == m_mapFriendGroup.end() )
    {
        SET_ERR_GOTO( ERR_MSN_11, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_MOVE_FRIEND_TO_GROUP_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_MOVE_FRIEND_TO_GROUP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwFriendUID = kPacket_.m_dwFriendUID;
    kPacket.m_nFromGroupID = kPacket_.m_nFromGroupID;
    kPacket.m_nToGroupID = kPacket_.m_nToGroupID;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_11, kPacket.m_nOK, 11 ); // 해당그룹이 없음
        ERR_CASE( ERR_MSN_27, kPacket.m_nOK, 27 ); // 해당 그룹에 존재하지 않는 유저임
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"메신저 친구 그룹 이동. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nFromGroupID )
        << BUILD_LOG( kPacket.m_nToGroupID )
        << BUILD_LOG( kPacket.m_dwFriendUID ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_MOVE_FRIEND_TO_GROUP_ACK );
}

IMPL_ON_FUNC( EMS_S2_MOVE_FRIEND_TO_GROUP_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwFriendUID )
        << BUILD_LOG( kPacket_.m_nFromGroupID )
        << BUILD_LOG( kPacket_.m_nToGroupID )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        std::map<int,KGroup>::iterator mit;
        std::map<int,KGroup>::iterator mit2;
        std::map<DWORD,KFriend>::iterator mit_friend;

        mit = m_mapFriendGroup.find( kPacket_.m_nFromGroupID );
        if( mit == m_mapFriendGroup.end() )
        {
            START_LOG( cwarn, L"대상 그룹이 없습니다. m_nFromGroupID : " << kPacket_.m_nFromGroupID ) << END_LOG;
            return; // must fix
        }

        mit_friend = mit->second.m_mapMembers.find( kPacket_.m_dwFriendUID );
        if( mit_friend == mit->second.m_mapMembers.end() )
        {
            START_LOG( cwarn, L"해당 유저가 없습니다. m_dwFriendUID : " << kPacket_.m_dwFriendUID ) << END_LOG;
            return; // must fix
        }

        mit2 = m_mapFriendGroup.find( kPacket_.m_nToGroupID );
        if( mit2 == m_mapFriendGroup.end() )
        {
            START_LOG( cwarn, L"대상 그룹이 없습니다. m_nToGroupID : " << kPacket_.m_nToGroupID ) << END_LOG;
            return; // must fix
        }

        mit2->second.m_mapMembers.insert( std::make_pair( kPacket_.m_dwFriendUID, mit_friend->second ) );
        mit->second.m_mapMembers.erase( kPacket_.m_dwFriendUID );
    }

    SEND_RECEIVED_PACKET( EMS_S2_MOVE_FRIEND_TO_GROUP_ACK );
}

// 이모티콘 설정
_IMPL_ON_FUNC( EMS_S2_SET_EMOTICON_REQ, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName ) << END_LOG;

    QUEUEING_EVENT_TO_DB( EMS_S2_SET_EMOTICON_REQ, kPacket_ );
}

IMPL_ON_FUNC( EMS_S2_SET_EMOTICON_ACK )
{
    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwEmoticonID )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_strNickName ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        m_dwEmoticon = kPacket_.m_dwEmoticonID;

        //갱신된 이모티콘 정보를 친구들에게 알려주자.
        SendToMyFriends_S2( GetMyFriendInfo_S2() );
        START_LOG( clog, L"갱신된 이모티콘 정보를 친구들에게 전달" ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EMS_S2_SET_EMOTICON_ACK );
}

// 채팅하기
_IMPL_ON_FUNC( EMS_S2_START_CHAT_REQ, std::set<DWORD> )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    std::map<DWORD,std::pair<std::wstring, std::wstring>> mapMemberInfo;
    std::set<DWORD> setFailedUIDs;
    std::set<DWORD> setSuccessUIDs;
    std::vector<DWORD> vecRecvUIDs;

    KEMS_S2_START_CHAT_NOT kNotPacket;
    std::pair<DWORD,DWORD>  m_prRoomKey;
    m_prRoomKey = std::make_pair( 0, 0 );

    if( kPacket_.empty() )
    {
        //채팅상대가 비어있음.에러
        SET_ERR_GOTO( ERR_MSN_28, err_proc );
    }
    setSuccessUIDs = kPacket_;

    if( IsChatRoomEnable() == false )
    {
        //내 채팅방최대수를 넘지 않았는가?
        SET_ERR_GOTO( ERR_MSN_29, err_proc );
    }

    // 초대성공한유저,초대실패한유저,초대성공한유저정보(UID,NICK)
    ChkChatEnable( setSuccessUIDs, setFailedUIDs, mapMemberInfo, vecRecvUIDs );

    if( setSuccessUIDs.empty() || mapMemberInfo.empty())
    {
        //초대할 수 있는 상대가 없음
        SET_ERR_GOTO( ERR_MSN_28, err_proc );
    }

    //내정보도 넣어줘야해.
    setSuccessUIDs.insert( GetUID() );
    mapMemberInfo.insert( std::make_pair( GetUID(), std::make_pair(m_strNickName, m_strNickColor) ) );
    vecRecvUIDs.push_back( GetUID() );

    //채팅방 키생성
    SiKMSSimLayer()->GetChatRoomUID( m_prRoomKey );
    kNotPacket.m_prRoomKey = m_prRoomKey;
    kNotPacket.m_mapMemberInfo = mapMemberInfo; //채팅방 맴버정보 기록

    //채팅상대들에게 채팅시작 알림.
    REMOTE_QUEUEING_PACK( vecRecvUIDs, EMS_S2_START_CHAT_NOT, kNotPacket );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_START_CHAT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_prRoomKey = m_prRoomKey;
    kPacket.m_setSuccessUIDs = setSuccessUIDs;
    kPacket.m_setFailedUIDs = setFailedUIDs;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_28, kPacket.m_nOK, 28 ); // 초대가능한 유저 UID가 없음
        ERR_CASE( ERR_MSN_29, kPacket.m_nOK, 29 ); // 내 모든 채팅방을 사용중임
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"채팅방 최초 시작 요청. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    std::wstringstream stmSuccessUIDs;
    std::copy( kPacket.m_setSuccessUIDs.begin(), kPacket.m_setSuccessUIDs.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stmSuccessUIDs, L"," ) );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_prRoomKey.first )
        << BUILD_LOG( kPacket.m_prRoomKey.second )
        << BUILD_LOG( vecRecvUIDs.size() )
        << BUILD_LOG( kPacket.m_setSuccessUIDs.size() )
        << BUILD_LOG( stmSuccessUIDs.str() )
        << BUILD_LOG( setFailedUIDs.size() )
        << BUILD_LOG( kPacket.m_setFailedUIDs.size() )
        << BUILD_LOGc( m_cChatRoomCount ) << END_LOG;

    //성공,실패 유무를 알려주자.
    SEND_PACKET( EMS_S2_START_CHAT_ACK );
}

IMPL_ON_FUNC( EMS_S2_START_CHAT_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    if( kPacket_.m_mapMemberInfo.empty() )
    {
        // 있을 수 없는 경우지만 예외처리.
        START_LOG( cwarn, L"채팅 채팅상대가 비어있음" )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( m_strNickName )
            << BUILD_LOG( kPacket_.m_prRoomKey.first )
            << BUILD_LOG( kPacket_.m_prRoomKey.second )
            << BUILD_LOG( kPacket_.m_mapMemberInfo.size() ) << END_LOG;

        return;
    }

    if( m_mapChatRoomInfo.find( kPacket_.m_prRoomKey ) != m_mapChatRoomInfo.end() )
    {
        //나한테 이미 있는 채팅방이다. 중복으로 초대를 받는 경우
        START_LOG( cwarn, L"이미 존재하는 채팅방입니다." )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( m_strNickName )
            << BUILD_LOG( kPacket_.m_prRoomKey.first )
            << BUILD_LOG( kPacket_.m_prRoomKey.second )
            << BUILD_LOG( kPacket_.m_mapMemberInfo.size() ) << END_LOG;

        return;
    }

    //현재 사용할 수 있는 채팅방이 없다.
    if ( IsChatRoomEnable() == false ) {
        //패킷을 보낼 채팅방 UID 리스트를 뽑아내자
        std::vector<DWORD> vectorSendUIDs;

        std::map<DWORD,std::pair<std::wstring, std::wstring>>::iterator mit;
        for ( mit = kPacket_.m_mapMemberInfo.begin() ; mit != kPacket_.m_mapMemberInfo.end() ; ++mit ) {
            //나는 나가니까 내UID는 지워줘.
            if ( mit->first != GetUID() ) {
                vectorSendUIDs.push_back( mit->first );
            }
        }

        //상대가 요청한 사이에 꽉 차버리는 경우라고 생각됨.
        KEMS_S2_LEAVE_CHAT_MEMBER_NOT kNotPacket;
        kNotPacket.m_prRoomKey = kPacket_.m_prRoomKey;
        kNotPacket.m_dwLeaveUID = GetUID();

        //그 채팅방을 떠났다고 통보
        REMOTE_QUEUEING_PACK( vectorSendUIDs, EMS_S2_LEAVE_CHAT_MEMBER_NOT, kNotPacket );

        START_LOG( cwarn, L"내 채팅방이 가득 찼음" )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( m_strNickName ) << END_LOG;
        return;
    }

    //채팅방 만들고 채팅방 카운트 갱신
    MakeChatRoom( kPacket_.m_prRoomKey, kPacket_.m_mapMemberInfo );

    //내 채팅방 카운트정보 친구들에게 전달.
    SendUpdateMyChatCountNot();

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_prRoomKey.first )
        << BUILD_LOG( kPacket_.m_prRoomKey.second )
        << BUILD_LOG( kPacket_.m_mapMemberInfo.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_S2_START_CHAT_NOT );
}

//초대 요청(다중채팅)
IMPL_ON_FUNC( EMS_S2_INVITE_CHAT_MEMBER_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    KEMS_S2_UPDATE_CHAT_MEMBER_NOT kUpdatePacket;
    KEMS_S2_START_CHAT_NOT kStartPacket;

    KMyChatRoom::const_iterator cmit;
    std::map<DWORD, std::pair<std::wstring, std::wstring>>::const_iterator cmitUser;
    std::map<DWORD, std::pair<std::wstring, std::wstring>>::const_iterator cmitNewUser;

    std::map<DWORD, std::pair<std::wstring, std::wstring>> mapOldMemberInfo;
    std::map<DWORD, std::pair<std::wstring, std::wstring>> mapNewMemberInfo;
    std::map<DWORD, std::pair<std::wstring, std::wstring>> mapAllMemberInfo;
    std::set<DWORD> setSuccessUIDs;
    std::set<DWORD> setFailedUIDs;
    std::vector<DWORD> vecRecvUIDs;
    std::vector<DWORD> vecOldRecvUIDs;

    //채팅방에 초대 가능한 유저가 없다.
    if ( kPacket_.m_setInviteUIDs.empty() ) {
        SET_ERR_GOTO( ERR_MSN_28, err_proc );
    }

    //룸키에 맞는 내 채팅방이 없다=이미 지워져있는 경우.
    cmit = m_mapChatRoomInfo.find( kPacket_.m_prRoomKey );
    if ( cmit == m_mapChatRoomInfo.end() ) {
        SET_ERR_GOTO( ERR_MSN_30, err_proc );
    }

    //최대 대화맴버수를 초과하게 된다.
    if ( cmit->second.size() + kPacket_.m_setInviteUIDs.size() > MAX_CHAT_MEMBER ) {
        SET_ERR_GOTO( ERR_MSN_31, err_proc );
    }

    //기존채팅방 맴버정보.
    mapOldMemberInfo = cmit->second;

    //기본 채팅방맴버 UID들 뽑기.
    for ( cmitUser = mapOldMemberInfo.begin() ; cmitUser != mapOldMemberInfo.end() ; ++cmitUser ) {
        vecOldRecvUIDs.push_back( cmitUser->first );
    }

    //갱신될 채팅방 맴버정보
    mapAllMemberInfo = mapOldMemberInfo;

    //초대할 유저 거르기
    mapNewMemberInfo.clear();
    setSuccessUIDs = kPacket_.m_setInviteUIDs;
    //초대성공유저,초대실패유저,초대성공한유저정보(UID,NICK)
    ChkChatEnable( setSuccessUIDs, setFailedUIDs, mapNewMemberInfo, vecRecvUIDs );

    //초대할 유저 거르기2
    //해당 채팅맴버가 아닌 유저만 초대 가능
    for ( cmitNewUser = mapNewMemberInfo.begin() ; cmitNewUser != mapNewMemberInfo.end() ; ++cmitNewUser ) {

        //현재 채팅방에 있는 유저인가?
        if ( mapOldMemberInfo.find( cmitNewUser->first ) == mapOldMemberInfo.end() ) {
            //전체 맴버 정보 갱신(기존맴버에 초대할 맴버정보 추가)
            mapAllMemberInfo.insert( std::make_pair( cmitNewUser->first, std::make_pair(cmitNewUser->second.first, cmitNewUser->second.second) ) );
        }
        else {
            //채팅방에 있으면 제외시킴
            setSuccessUIDs.erase( cmitNewUser->first );
            //초대실패한유저
            setFailedUIDs.insert( cmitNewUser->first );
        }
    }

    //채팅방에 초대 가능한 유저가 없다.
    if ( setSuccessUIDs.empty() || mapNewMemberInfo.empty() ) {
        SET_ERR_GOTO( ERR_MSN_28, err_proc );
    }

    //내가 초대한 유저에게 초대패킷 날림
    //초대받은 유저들에겐 새로운 채팅방 시작알림
    kStartPacket.m_prRoomKey = kPacket_.m_prRoomKey;
    kStartPacket.m_mapMemberInfo = mapAllMemberInfo;
    REMOTE_QUEUEING_PACK( vecRecvUIDs, EMS_S2_START_CHAT_NOT, kStartPacket );

    //기존 유저들에게 맴버정보 갱신 패킷 날림
    kUpdatePacket.m_prRoomKey = kPacket_.m_prRoomKey;
    kUpdatePacket.m_mapMemberInfo = mapAllMemberInfo;
    REMOTE_QUEUEING_PACK( vecOldRecvUIDs, EMS_S2_UPDATE_CHAT_MEMBER_NOT, kUpdatePacket );

    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_INVITE_CHAT_MEMBER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_prRoomKey = kPacket_.m_prRoomKey;
    kPacket.m_setSuccessUIDs = setSuccessUIDs;
    kPacket.m_setFailedUIDs = setFailedUIDs;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_28, kPacket.m_nOK, 28 ); // 초대가능한 유저 UID가 없음
        ERR_CASE( ERR_MSN_30, kPacket.m_nOK, 30 ); // 해당하는 채팅방이 없다
        ERR_CASE( ERR_MSN_31, kPacket.m_nOK, 31 ); // 최대 대화맴버수가 초과됩니다.
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"채팅방에 유저 초대 요청. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    std::wstringstream stmSuccessUIDs;
    std::copy( kPacket.m_setSuccessUIDs.begin(), kPacket.m_setSuccessUIDs.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stmSuccessUIDs, L", " ) );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_prRoomKey.first )
        << BUILD_LOG( kPacket.m_prRoomKey.second )
        << BUILD_LOG( kPacket.m_setSuccessUIDs.size() )
        << BUILD_LOG( stmSuccessUIDs.str() )
        << BUILD_LOG( kPacket.m_setFailedUIDs.size() )
        << BUILD_LOG( mapAllMemberInfo.size() )
        << BUILD_LOGc( m_cChatRoomCount ) << END_LOG;

    //성공,실패 유무를 알려주자.
    SEND_PACKET( EMS_S2_INVITE_CHAT_MEMBER_ACK );
}

//기존 맴버에게 맴버 정보 갱신 알림(다중채팅)
IMPL_ON_FUNC( EMS_S2_UPDATE_CHAT_MEMBER_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_prRoomKey.first )
        << BUILD_LOG( kPacket_.m_prRoomKey.second )
        << BUILD_LOG( kPacket_.m_mapMemberInfo.size() ) << END_LOG;

    if ( kPacket_.m_mapMemberInfo.empty() ) {
        START_LOG( cwarn, L"채팅 맴버 정보가 비어있음" )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( m_strNickName ) << END_LOG;
        return;
    }

    //룸키에 맞는 내 채팅방이 있는지?
    KMyChatRoom::iterator mit;
    mit = m_mapChatRoomInfo.find( kPacket_.m_prRoomKey );

    //없다=이미 지워져있는 경우.
    if ( mit == m_mapChatRoomInfo.end() ) {
        std::vector<DWORD> vecSendUIDs;

        //패킷을 보낼 채팅방 UID 리스트를 뽑아내자
        std::map<DWORD,std::pair<std::wstring,std::wstring>>::const_iterator cmit;
        for ( cmit = kPacket_.m_mapMemberInfo.begin() ; cmit != kPacket_.m_mapMemberInfo.end() ; ++cmit ) {
            if ( cmit->first != GetUID() ) {
                vecSendUIDs.push_back( cmit->first );
            }
        }

        //상대가 요청한 사이에 꽉 차버리는 경우라고 생각됨.
        KEMS_S2_LEAVE_CHAT_MEMBER_NOT kNotPacket;
        kNotPacket.m_prRoomKey = kPacket_.m_prRoomKey;
        kNotPacket.m_dwLeaveUID = GetUID();

        //그 채팅방을 떠났다고 통보
        REMOTE_QUEUEING_PACK( vecSendUIDs, EMS_S2_LEAVE_CHAT_MEMBER_NOT, kNotPacket );

        //내 채팅방 카운트정보 친구들에게 전달.
        SendUpdateMyChatCountNot();

        START_LOG( cwarn, L"예전에 이미 해당 채팅방을 떠난다고 통보" )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( m_strNickName ) << END_LOG;
    }
    else {
        //기존 맴버 정보 갱신
        mit->second = kPacket_.m_mapMemberInfo;

        //클라이언트에 통보
        SEND_RECEIVED_PACKET( EMS_S2_UPDATE_CHAT_MEMBER_NOT );
    }
}

//채팅방 나감
IMPL_ON_FUNC( EMS_S2_LEAVE_CHAT_MEMBER_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    std::vector<DWORD> vecSendUIDs;
    KEMS_S2_LEAVE_CHAT_MEMBER_NOT kNotPacket;
    kNotPacket.m_prRoomKey = kPacket_;
    kNotPacket.m_dwLeaveUID = GetUID();

    //방키로 찾아보고 없으면 이미 나간거다.
    KMyChatRoom::iterator mit;
    mit = m_mapChatRoomInfo.find( kPacket_ );
    if ( mit != m_mapChatRoomInfo.end() ) {
        //채팅 전송할 UID 리스트 뽑고.
        std::map<DWORD,std::pair<std::wstring,std::wstring>>::iterator mitUser;
        for ( mitUser = mit->second.begin() ; mitUser != mit->second.end() ; ++mitUser ) {
            //내UID는 제외
            if ( mitUser->first != GetUID() ) {
                vecSendUIDs.push_back( mitUser->first );
            }
        }
    }

    //다른 애들한테 나갔다고 알림.
    REMOTE_QUEUEING_PACK( vecSendUIDs, EMS_S2_LEAVE_CHAT_MEMBER_NOT, kNotPacket );

    // 내 채팅방 제거
    CloseChatRoom( kPacket_ );

    //나에게 해당채팅방을 나갔음을 알림.
    SEND_RECEIVED_PACKET( EMS_S2_LEAVE_CHAT_MEMBER_ACK );
}

IMPL_ON_FUNC( EMS_S2_LEAVE_CHAT_MEMBER_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_prRoomKey.first )
        << BUILD_LOG( kPacket_.m_prRoomKey.second )
        << BUILD_LOG( kPacket_.m_dwLeaveUID ) << END_LOG;

    KMyChatRoom::iterator mit;
    std::map<DWORD,std::wstring>::iterator mit2;

    //해당하는 채팅방이 있는지
    mit = m_mapChatRoomInfo.find( kPacket_.m_prRoomKey );
    if( mit == m_mapChatRoomInfo.end() )
    {
        //없다=이미 내가 나간 채팅방이다.
        START_LOG( cwarn, L"없는 채팅방입니다." )
            << BUILD_LOG( m_dwUID )
            << BUILD_LOG( m_strNickName )
            << BUILD_LOG( kPacket_.m_prRoomKey.first )
            << BUILD_LOG( kPacket_.m_prRoomKey.second )
            << BUILD_LOG( kPacket_.m_dwLeaveUID ) << END_LOG;
    }
    else
    {
        //채팅방 맴버에서 제거
        mit->second.erase( kPacket_.m_dwLeaveUID );

        if( mit->second.size() <= 0 )
        {
            //아무도 없다=채팅방 종료
            CloseChatRoom( kPacket_.m_prRoomKey );
        }
    }

    //클라이언트에게 알림
    SEND_RECEIVED_PACKET( EMS_S2_LEAVE_CHAT_MEMBER_NOT );
}

_IMPL_ON_FUNC( EMS_S2_SEND_CHAT_REQ, KChatMsg )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    std::wstringstream stmChetMemberUIDs;
    std::copy( kPacket_.m_setReceiverUIDs.begin(), kPacket_.m_setReceiverUIDs.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stmChetMemberUIDs, L", " ) );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOGc( m_cChatRoomCount )
        << BUILD_LOG( kPacket_.m_prRoomKey.first )
        << BUILD_LOG( kPacket_.m_prRoomKey.second )
        << BUILD_LOG( kPacket_.m_strMessage )
        << BUILD_LOG( stmChetMemberUIDs.str() ) << END_LOG;

    if( kPacket_.m_strMessage.empty() )
    {
        START_LOG( cwarn, L"채팅 메세지가 없음" ) << END_LOG;
        return;
    }

    //발신자UID가 제대로 세팅안되어 있을 경우 설정해주자.
    if( kPacket_.m_dwSenderUID != GetUID() )
    {
        kPacket_.m_dwSenderUID = GetUID();
    }

    //룸키에 해당하는 채팅방을 찾아서
    KMyChatRoom::iterator mit;
    mit = m_mapChatRoomInfo.find( kPacket_.m_prRoomKey );
    if( mit == m_mapChatRoomInfo.end() )
    {
        START_LOG( cwarn, L"룸키와 일치하는 채팅방이 없음" ) << END_LOG;
        return;
    }

    //채팅방 맴버 채워주기
    kPacket_.m_setReceiverUIDs.clear();
    for( std::map<DWORD,std::pair<std::wstring,std::wstring>>::iterator mit2 = mit->second.begin() ;
        mit2 != mit->second.end() ; ++mit2 )
    {
        kPacket_.m_setReceiverUIDs.insert( mit2->first );
    }
    //자신 포함.
    kPacket_.m_setReceiverUIDs.insert( GetUID() );

    //채팅 메세지 전달
    std::vector<DWORD> vecRecvUIDs;
    SetRecvUIDs( kPacket_.m_setReceiverUIDs, vecRecvUIDs );
    REMOTE_QUEUEING_PACK_ACK( vecRecvUIDs, EMS_S2_SEND_CHAT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EMS_S2_SEND_CHAT_NOT, KChatMsg )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    std::wstringstream stmChetMemberUIDs;
    std::copy( kPacket_.m_setReceiverUIDs.begin(), kPacket_.m_setReceiverUIDs.end(), 
        std::ostream_iterator< std::set<DWORD>::value_type,wchar_t>( stmChetMemberUIDs, L"," ) );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOGc( m_cChatRoomCount )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_prRoomKey.first )
        << BUILD_LOG( kPacket_.m_prRoomKey.second )
        << BUILD_LOG( kPacket_.m_setReceiverUIDs.size() )
        << BUILD_LOG( stmChetMemberUIDs.str() )
        << BUILD_LOG( kPacket_.m_strMessage ) << END_LOG;

    if( kPacket_.m_strMessage.empty() )
    {
        START_LOG( cwarn, L"채팅 메세지가 없음" ) << END_LOG;
        return;
    }

    //클라이언트에게 메세지 내용 전달
    SEND_RECEIVED_PACKET( EMS_S2_SEND_CHAT_NOT );
}

IMPL_ON_FUNC( EMS_S2_UPDATE_MY_CHAT_COUNT_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOGc( kPacket_.m_dwSenderUID )
        << BUILD_LOGc( kPacket_.m_cChatRoomCount ) << END_LOG;

    //친구 UID 검색해서 있으면 갱신
    std::map<DWORD,KFriend>::iterator mit2;
    std::map<int,KGroup>::iterator mit;
    for( mit = m_mapFriendGroup.begin() ; mit != m_mapFriendGroup.end() ; ++mit )
    {
        mit2 = mit->second.m_mapMembers.find( kPacket_.m_dwSenderUID );
        if( mit2 != mit->second.m_mapMembers.end() )
        {
            //찾았으면 나가자.
            break;
        }
    }

    //서버만 알고 있으면 되는 정보다.
    if( mit != m_mapFriendGroup.end() && mit2 != mit->second.m_mapMembers.end() )
    {
        //친구의 채팅방 개수 기억
        mit2->second.m_cChatRoomCount = kPacket_.m_cChatRoomCount;
    }
}

//     // 길드원 목록 갱신
//     IMPL_ON_FUNC( EMS_S2_UPDATE_GUILD_MEMBER_REQ ){}
//     IMPL_ON_FUNC( EMS_S2_UPDATE_GUILD_MEMBER_NOT ){}
//     // 길드미션완료 자동알림쪽지
//     IMPL_ON_FUNC( EMS_S2_AUTO_GUILD_MESSAGE_REQ ){}
//     IMPL_ON_FUNC( EMS_S2_AUTO_GUILD_MESSAGE_NOT ){}

// 쪽지
IMPL_ON_FUNC( EMS_S2_SEND_PAPER_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    if( kPacket_.m_setRecvUIDs.empty() && kPacket_.m_strRecvNick.empty() )
    {
        //쪽지 받는 사람이 비어있음
        SET_ERR_GOTO( ERR_MSN_34, err_proc );
    }

    if( (!kPacket_.m_strRecvNick.empty() && kPacket_.m_setRecvUIDs.size() > KPaper::MAX_PAPER_SEND_LIMIT-1)
      || (kPacket_.m_strRecvNick.empty() && kPacket_.m_setRecvUIDs.size() > KPaper::MAX_PAPER_SEND_LIMIT) )
    {
        //한번에 보낼 수 있는 최대 쪽지수 초과됨.
        SET_ERR_GOTO( ERR_MSN_40, err_proc );
    }

    if( kPacket_.m_strMessage.empty() )
    {
        //메세지가 비어있음
        SET_ERR_GOTO( ERR_MSN_35, err_proc );
    }

    if( kPacket_.m_dwSkinItemID < 0 )
    {
        //잘못된 스킨 아이템 ID입니다.
        START_LOG( cwarn, L"잘못된 스킨 아이템 ID입니다. Item ID : " << kPacket_.m_dwSkinItemID ) << END_LOG;
        kPacket_.m_dwSkinItemID = 0;
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_SEND_PAPER_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_SEND_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_strRecvNick = kPacket_.m_strRecvNick;
    kPacket.m_setRecvUIDs = kPacket_.m_setRecvUIDs;
    kPacket.m_strMessage = kPacket_.m_strMessage;
    kPacket.m_dwSkinItemID = kPacket_.m_dwSkinItemID;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_34, kPacket.m_nOK, 34 ); // 쪽지 받는 사람이 비어있음
        ERR_CASE( ERR_MSN_35, kPacket.m_nOK, 35 ); // 메세지가 비어있음
        ERR_CASE( ERR_MSN_40, kPacket.m_nOK, 40 ); // 한번에 보낼 수 있는 최대 쪽지수 초과
    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"메신저 친구 그룹 이동. 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_strRecvNick )
        << BUILD_LOG( kPacket.m_setRecvUIDs.size() )
        << BUILD_LOG( kPacket.m_strMessage )
        << BUILD_LOG( kPacket.m_dwSkinItemID ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_SEND_PAPER_ACK );
}

IMPL_ON_FUNC( EMS_S2_SEND_PAPER_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    //DB에 기록 성공했으면,
    if( kPacket_.m_nOK == 0 )
    {
        KEMS_S2_NEW_PAPER_NOT kNotPacket;
        kNotPacket.m_dwSenderUID = GetUID();
        kNotPacket.m_strSenderNick = m_strNickName;

        //MR상에 대상유저들 찾아 NOT를 알려줘.
        std::vector<DWORD> vecRecvUIDs;
        SetRecvUIDs( kPacket_.m_setRecvUIDs, vecRecvUIDs );
        REMOTE_QUEUEING_PACK( vecRecvUIDs, EMS_S2_NEW_PAPER_NOT, kNotPacket );

        //새로 보낸 쪽지가 있음.
        m_bHaveNewSentPaper = true;
   }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strRecvNick )
        << BUILD_LOG( kPacket_.m_setRecvUIDs.size() )
        << BUILD_LOG( kPacket_.m_strMessage )
        << BUILD_LOG( kPacket_.m_dwSkinItemID ) << END_LOG;

   //본인에게 결과 알려주자
    SEND_RECEIVED_PACKET( EMS_S2_SEND_PAPER_ACK );
}

IMPL_ON_FUNC( EMS_S2_NEW_PAPER_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_strSenderNick ) << END_LOG;

    //새로 받은 쪽지가 있다.
    m_bHaveNewRecvPaper = true;

    SEND_RECEIVED_PACKET( EMS_S2_NEW_PAPER_NOT );
}

IMPL_ON_FUNC_NOPARAM( EMS_S2_LOAD_SENT_PAPERBOX_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName ) << END_LOG;

    // 3초단위로 쪽지함 읽도록.
    if( m_bHaveNewSentPaper == true && 
        ::GetTickCount() - GetTick( TI_LOAD_SENT_PAPERBOX ) > GetTickGap( TI_LOAD_SENT_PAPERBOX ) )
    {
        QUEUEING_ID_TO_DB( EMS_S2_LOAD_SENT_PAPERBOX_REQ );

        SetTick( TI_LOAD_SENT_PAPERBOX );
        return;
    }

    //쪽지함 바뀐 내역이 없음
    SEND_ID( EMS_S2_NOCHANGE_SENT_PAPERBOX_NOT );
}

//보낸 쪽지함 갱신
IMPL_ON_FUNC( EMS_S2_UPDATE_SENT_PAPERBOX_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    //보낸 쪽지함 확인함
    m_bHaveNewSentPaper = false;

    //DB 쪽지 데이터가 내 데이터랑 같으면..
    if( m_mapSentPaperBox == kPacket_ )
    {
        //쪽지함 바뀐 내역이 없음
        SEND_ID( EMS_S2_NOCHANGE_SENT_PAPERBOX_NOT );
        return;
    }

    //쪽지 데이터 갱신
    m_mapSentPaperBox = kPacket_;

    //보낸 쪽지는 최근에서부터 50개 끊어서 주자.
    if( m_mapSentPaperBox.size() > KPaper::MAX_SENT_PAPERBOX )
    {
        kPacket_.clear();
        int nCount = 1;

        for( KPAPERBOX::reverse_iterator mrit = m_mapSentPaperBox.rbegin();
            mrit != m_mapSentPaperBox.rend(); ++mrit )
        {
            if( nCount > KPaper::MAX_RECV_PAPERBOX )
                break;

            //쪽지 50개 끊어서, 패킷에 넣어주자.
            kPacket_.insert( std::make_pair( mrit->first, mrit->second ) );

            ++nCount;
        }
    }

    //KSerBuffer를 사용해야할까..패킷 무거울듯.
    SEND_RECEIVED_PACKET( EMS_S2_UPDATE_SENT_PAPERBOX_NOT );
}

IMPL_ON_FUNC_NOPARAM( EMS_S2_LOAD_RECV_PAPERBOX_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName ) << END_LOG;

    // 3초단위로 쪽지함 읽도록 && 새로 보낸 쪽지가 있으면 DB에 문의
    if( m_bHaveNewRecvPaper == true &&
        ::GetTickCount() - GetTick( TI_LOAD_RECV_PAPERBOX ) > GetTickGap( TI_LOAD_RECV_PAPERBOX ) )
    {
        QUEUEING_ID_TO_DB( EMS_S2_LOAD_RECV_PAPERBOX_REQ );

        SetTick( TI_LOAD_RECV_PAPERBOX );
        return;
    }

    //쪽지함 바뀐 내역이 없음
    SEND_ID( EMS_S2_NOCHANGE_RECV_PAPERBOX_NOT );
}

//받은 쪽지함 갱신
IMPL_ON_FUNC( EMS_S2_UPDATE_RECV_PAPERBOX_NOT )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

        VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    //받은 쪽지함 확인함
    m_bHaveNewRecvPaper = false;

    //받아온 쪽지 데이터가 내 데이터랑 같으면..
    if( m_mapSentPaperBox == kPacket_ )
    {
        //쪽지함 바뀐 내역이 없음
        SEND_ID( EMS_S2_NOCHANGE_RECV_PAPERBOX_NOT );
        return;
    }

    //쪽지 데이터 갱신
    m_mapRecvPaperBox = kPacket_;

    //쪽지가 50통이 넘으면
    if( m_mapRecvPaperBox.size() > KPaper::MAX_RECV_PAPERBOX )
    {
        kPacket_.clear();
        int nCount = 1;

        for( KPAPERBOX::iterator mit = m_mapRecvPaperBox.begin();
            mit != m_mapRecvPaperBox.end(); ++mit )
        {
            if( nCount > KPaper::MAX_RECV_PAPERBOX )
                break;

            //쪽지 50개 끊어서, 패킷에 넣어주자.
            kPacket_.insert( std::make_pair( mit->first, mit->second ) );

            ++nCount;
        }
    }

    //KSerBuffer를 사용해야할까..패킷 무거울듯.
    SEND_RECEIVED_PACKET( EMS_S2_UPDATE_RECV_PAPERBOX_NOT );

    //클라이언트에게는 받은쪽지함 가득 찼다고 알려주자.
    if( m_mapRecvPaperBox.size() >= KPaper::MAX_RECV_PAPERBOX )
        SEND_ID( EMS_S2_PAPERBOX_FULL_NOT );
}

_IMPL_ON_FUNC( EMS_S2_READ_PAPER_REQ, std::set<DWORD> )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    if( kPacket_.empty() )
    {
        //선택한 쪽지가 없음.
        SET_ERR_GOTO( ERR_MSN_41, err_proc );
    }

    //상태를 바꿀 쪽지UID가 있는지 받은쪽지함과 비교해보자.
    if( IsMyRecvPaper( kPacket_ ) == false )
    {
        SET_ERR_GOTO( ERR_MSN_42, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_LOAD_RECV_PAPERBOX_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_READ_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_41, kPacket.m_nOK, 41 ); //선택한 쪽지가 없음.
        ERR_CASE( ERR_MSN_42, kPacket.m_nOK, 42 ); //하나라도 없는 쪽지가 있음.

    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"쪽지 읽음(상태갱신). 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_setPaperUIDs.size() ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_READ_PAPER_ACK );
}

IMPL_ON_FUNC( EMS_S2_READ_PAPER_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    if( kPacket_.m_nOK == 0 )
    {
        //성공한 경우. 내 쪽지 정보를 변경시켜주자.
        if( SetMyRecvPaper( kPacket_.m_setPaperUIDs, KPaper::READ ) == false )
        {
            //변경 실패
            SET_ERR_VAL( ERR_MSN_42, kPacket_.m_nOK, 42 );
        }
    }
    
    if( kPacket_.m_nOK == 42 ) // DB,서버,클라 간의 기록이 다르다
    {
        //예외처리.DB와 받은 쪽지함 동기화를 시도하자.
        QUEUEING_ID_TO_DB( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_setPaperUIDs.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_S2_READ_PAPER_ACK );
}

_IMPL_ON_FUNC( EMS_S2_DEL_SENT_PAPER_REQ, std::set<DWORD> )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    if( kPacket_.empty() )
    {
        // 선택한 쪽지가 없음
        SET_ERR_GOTO( ERR_MSN_41, err_proc );
    }

    //상태를 바꿀 쪽지UID가 있는지 받은쪽지함과 비교해보자.
    if( IsMySentPaper( kPacket_ ) == false )
    {
        SET_ERR_GOTO( ERR_MSN_42, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_DEL_SENT_PAPER_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_DEL_SENT_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_41, kPacket.m_nOK, 41 ); //선택한 쪽지가 없음.
        ERR_CASE( ERR_MSN_42, kPacket.m_nOK, 42 ); //하나라도 없는 쪽지가 있음.

    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"보낸쪽지 삭제(상태갱신). 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_setPaperUIDs.size() ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_DEL_SENT_PAPER_ACK );
}

IMPL_ON_FUNC( EMS_S2_DEL_SENT_PAPER_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_setPaperUIDs.size() ) << END_LOG;

    if( kPacket_.m_nOK == 42 )
    {
        //예외처리.DB와 받은 쪽지함 동기화를 시도하자.
        QUEUEING_ID_TO_DB( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
    }

    //삭제 결과 알림.
    SEND_RECEIVED_PACKET( EMS_S2_DEL_SENT_PAPER_ACK );

    if( kPacket_.m_nOK != 0 )
        return;

    //삭제에 성공하면 쪽지함 갱신시켜주자.
    for( std::set<DWORD>::iterator sit = kPacket_.m_setPaperUIDs.begin() ;
        sit != kPacket_.m_setPaperUIDs.end() ; ++sit )
    {
        m_mapSentPaperBox.erase( *sit ); //해당 쪽지들을 찾아 지운다.
    }

    KEMS_S2_UPDATE_SENT_PAPERBOX_NOT kPacket;

    if( m_mapSentPaperBox.size() > KPaper::MAX_SENT_PAPERBOX )
    {
        int nCount = 1;
        for( KPAPERBOX::reverse_iterator mrit = m_mapSentPaperBox.rbegin();
            mrit != m_mapSentPaperBox.rend(); ++mrit )
        {
            if( nCount > KPaper::MAX_SENT_PAPERBOX )
                break;

            //쪽지 50개 끊어서, 패킷에 넣어주자.
            kPacket.insert( std::make_pair( mrit->first, mrit->second ) );

            ++nCount;
        }
    }
    else
    {
        //50개 이하면 그대로 넣어서 보내주자
        kPacket = m_mapSentPaperBox;
    }

    //성공했으면 보낸 쪽지함 갱신 알림
    SEND_PACKET( EMS_S2_UPDATE_SENT_PAPERBOX_NOT );
}

_IMPL_ON_FUNC( EMS_S2_DEL_RECV_PAPER_REQ, std::set<DWORD> )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    if( kPacket_.empty() )
    {
        // 선택한 쪽지가 없음
        SET_ERR_GOTO( ERR_MSN_41, err_proc );
    }

    //상태를 바꿀 쪽지UID가 있는지 받은쪽지함과 비교해보자.
    if( IsMyRecvPaper( kPacket_ ) == false )
    {
        SET_ERR_GOTO( ERR_MSN_42, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_DEL_RECV_PAPER_REQ, kPacket_ );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_DEL_RECV_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_41, kPacket.m_nOK, 41 ); //선택한 쪽지가 없음.
        ERR_CASE( ERR_MSN_42, kPacket.m_nOK, 42 ); //하나라도 없는 쪽지가 있음.

    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"받은쪽지 삭제(상태갱신). 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_setPaperUIDs.size() ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_DEL_RECV_PAPER_ACK );
}

IMPL_ON_FUNC( EMS_S2_DEL_RECV_PAPER_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_setPaperUIDs.size() ) << END_LOG;

    if( kPacket_.m_nOK == 42 ) // 없는 쪽지가 있다.
    {
        //예외처리.DB와 받은 쪽지함 동기화를 시도하자.
        QUEUEING_ID_TO_DB( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
    }

    //삭제 결과 알림.
    SEND_RECEIVED_PACKET( EMS_S2_DEL_RECV_PAPER_ACK );

    //삭제에 성공하면 쪽지함 갱신시켜주자.
    if( kPacket_.m_nOK != 0 )
        return;

    for( std::set<DWORD>::iterator sit = kPacket_.m_setPaperUIDs.begin() ;
        sit != kPacket_.m_setPaperUIDs.end() ; ++sit )
    {
        m_mapRecvPaperBox.erase( *sit ); //해당 쪽지들을 찾아 지운다.
    }

    KEMS_S2_UPDATE_RECV_PAPERBOX_NOT kPacket;

    if( m_mapRecvPaperBox.size() > KPaper::MAX_RECV_PAPERBOX )
    {
        int nCount = 1;
        for( KPAPERBOX::iterator mit = m_mapRecvPaperBox.begin();
            mit != m_mapRecvPaperBox.end(); ++mit )
        {
            if( nCount > KPaper::MAX_RECV_PAPERBOX )
                break;

            //쪽지 50개 끊어서, 패킷에 넣어주자.
            kPacket.insert( std::make_pair( mit->first, mit->second ) );

            ++nCount;
        }
    }
    else
    {
        //50개 이하면 그대로 넣어서 보내주자
        kPacket = m_mapRecvPaperBox;
    }

    //성공했으면 보낸 쪽지함 갱신 알림
    SEND_PACKET( EMS_S2_UPDATE_RECV_PAPERBOX_NOT );

    if( m_mapRecvPaperBox.size() >= KPaper::MAX_RECV_PAPERBOX )
        SEND_ID( EMS_S2_PAPERBOX_FULL_NOT );
}


_IMPL_ON_FUNC( EMS_S2_SEAL_PAPER_REQ, std::set<DWORD> )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );

    std::set<DWORD> kDBPacket;

    if( kPacket_.empty() )
    {
        // 선택한 쪽지가 없음
        SET_ERR_GOTO( ERR_MSN_41, err_proc );
    }

    //상태를 바꿀 쪽지UID가 있는지 받은쪽지함과 비교해보자.
    if( IsMyRecvPaper( kPacket_ ) == false )
    {
        //하나라도 없는 쪽지가 있음
        SET_ERR_GOTO( ERR_MSN_42, err_proc );
    }

    //이미 영구보관인 쪽지 제외
    for( std::set<DWORD>::iterator sit = kPacket_.begin() ;
        sit != kPacket_.end() ; ++sit )
    {
        if( IsSealedPaper( *sit) == false )
        {
            kDBPacket.insert( *sit );
        }
    }

    if( kDBPacket.empty() )
    {
        //이미 모두 영구보관 쪽지이다.
        SET_ERR_GOTO( ERR_MSN_44, err_proc );
    }

    //영구쪽지 최대 보관수를 넘지 않는지?(현재영구쪽지수+바꾸려는쪽지수)
    if( GetSealedPaperCount() + kDBPacket.size() > KPaper::MAX_SEAL_PAPER )
    {
        //영구쪽지 한도수 초과입니다.
        SET_ERR_GOTO( ERR_MSN_45, err_proc );
    }

    QUEUEING_EVENT_TO_DB( EMS_S2_SEAL_PAPER_REQ, kDBPacket );
    SET_ERROR( NET_OK );

err_proc:
    KEMS_S2_SEAL_PAPER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_setPaperUIDs = kPacket_;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_41, kPacket.m_nOK, 41 ); //선택한 쪽지가 없음.
        ERR_CASE( ERR_MSN_42, kPacket.m_nOK, 42 ); //하나라도 없는 쪽지가 있음.
        ERR_CASE( ERR_MSN_44, kPacket.m_nOK, 44 ); //이미 모두 영구보관 쪽지이다.
        ERR_CASE( ERR_MSN_45, kPacket.m_nOK, 45 ); //영구쪽지 한도수 초과입니다.

    default: 
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"영구쪽지 설정(상태갱신). 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_setPaperUIDs.size() )
        << BUILD_LOG( kDBPacket.size() ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EMS_S2_SEAL_PAPER_ACK );
}

IMPL_ON_FUNC( EMS_S2_SEAL_PAPER_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_dwUID )
        << BUILD_LOG( m_strNickName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_setPaperUIDs.size() ) << END_LOG;

    if( kPacket_.m_nOK == 0 )
    {
        //성공한 경우. 내 쪽지 정보를 변경시켜주자.
        if( SetMyRecvPaper( kPacket_.m_setPaperUIDs, KPaper::SEALED ) == false )
        {
            //정보변경 실패
            SET_ERR_VAL( ERR_MSN_42, kPacket_.m_nOK, 42 );
        }
    }
    
    if( kPacket_.m_nOK == 42 )
    {
        //예외처리.DB와 받은 쪽지함 동기화를 시도하자.
        QUEUEING_ID_TO_DB( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
    }

    SEND_RECEIVED_PACKET( EMS_S2_SEAL_PAPER_ACK );
}

IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    SET_ERROR( ERR_UNKNOWN );
    KEMS_S2_ADD_FRIEND_FORCE_ACK kPacket;
    kPacket.m_nOK = 99;

    // 자기 자신을 추가하려고 함
    if( GetUID()== kPacket_.m_dwRequesteeUID )
    {
        SET_ERR_GOTO( ERR_MSN_06, err_proc );
    }

    // 이미 내 친구이다.
    if( IsMyFriend( kPacket_.m_dwRequesteeUID ) )
    {
        SET_ERR_GOTO( ERR_MSN_07, err_proc );
    }
    QUEUEING_EVENT_TO_DB( EMS_S2_ADD_FRIEND_FORCE_REQ, kPacket_ );

    SET_ERROR( NET_OK );
err_proc:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_MSN_06, kPacket.m_nOK, 1 );
        ERR_CASE( ERR_MSN_07, kPacket.m_nOK, 2 );
    default:
        START_LOG( cerr, L"알수 없는 오류 발생. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EMS_S2_ADD_FRIEND_FORCE_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwRequesterUID )
        << BUILD_LOG( kPacket_.m_strRequesterNick )
        << BUILD_LOG( kPacket_.m_dwRequesteeUID )
        << BUILD_LOG( kPacket_.m_strRequesteeNick ) << END_LOG;
}

_IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_ACK, KEMS_S2_ADD_FRIEND_FORCE_REQ )
{
    KEMS_S2_ADD_FRIEND_FORCE_ACK kPacket;
    kPacket.m_nOK = kPacket_.m_nOK;

    kPacket.m_kFriendInfo.m_bIsOnline       = false; // 상대방이 알려준다.
    kPacket.m_kFriendInfo.m_bIsFriend       = true;
    kPacket.m_kFriendInfo.m_strServerName   = L"off";
    kPacket.m_kFriendInfo.m_dwUserUID       = kPacket_.m_dwRequesteeUID;
    kPacket.m_kFriendInfo.m_strNickName     = kPacket_.m_strRequesteeNick;

    // OK 가 0인 경우 
    if( kPacket.m_nOK != 0 )
    {
        SEND_PACKET( EMS_S2_ADD_FRIEND_FORCE_ACK );
        START_LOG( cerr, L"강제 친구 등록시 오류 발생... Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_nOK)
            << BUILD_LOG( kPacket_.m_dwRequesterUID )
            << BUILD_LOG( kPacket_.m_strRequesterNick )
            << BUILD_LOG( kPacket_.m_dwRequesteeUID )
            << BUILD_LOG( kPacket_.m_strRequesteeNick ) << END_LOG;
        return;
    }

    // 친구, 블럭 리스트 에서 먼저 제거..
    m_kBlockGroup.m_mapMembers.erase( kPacket_.m_dwRequesteeUID );

    // 친구로 등록.
    if( !IsMyFriend( kPacket_.m_dwRequesteeUID ) )
    {
        std::map<int,KGroup>::iterator mit;
        mit = m_mapFriendGroup.find( KGroup::FRIEND_BASE );
        if( mit == m_mapFriendGroup.end() )
        {
            KGroup kGroup;
            kGroup.m_nGroupKey = KGroup::FRIEND_BASE;
            mit = m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
        }

        LIF( mit->second.m_mapMembers.insert( std::make_pair( kPacket.m_kFriendInfo.m_dwUserUID, kPacket.m_kFriendInfo ) ).second );
    }
    
    // 전송, RemoteQueuing.
    SEND_PACKET( EMS_S2_ADD_FRIEND_FORCE_ACK );
    REMOTE_QUEUEING( kPacket.m_kFriendInfo.m_dwUserUID, EMS_S2_ADD_FRIEND_FORCE_NOT, GetMyFriendInfo_S2() );

}

_IMPL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_NOT, KFriend )
{
    // 친구, 블럭 리스트 에서 먼저 제거..
    m_kBlockGroup.m_mapMembers.erase( kPacket_.m_dwUserUID );

    // 친구로 등록.
    if( !IsMyFriend( kPacket_.m_dwUserUID ) )
    {
        std::map<int,KGroup>::iterator mit;
        mit = m_mapFriendGroup.find( KGroup::FRIEND_BASE );
        if( mit == m_mapFriendGroup.end() )
        {
            KGroup kGroup;
            kGroup.m_nGroupKey = KGroup::FRIEND_BASE;
            mit = m_mapFriendGroup.insert( std::make_pair( kGroup.m_nGroupKey, kGroup ) ).first;
        }

        LIF( mit->second.m_mapMembers.insert( std::make_pair( kPacket_.m_dwUserUID, kPacket_ ) ).second );
    }

    LIF( UpdateFriendInfo_S2( kPacket_ ) );
    REMOTE_QUEUEING( kPacket_.m_dwUserUID, EMS_S2_UPDATE_FRIEND_NOT, GetMyFriendInfo_S2() );

    if( !IsBlockedFriend_S2( kPacket_.m_dwUserUID ) )
    {
        //상대에게 받은 정보
        SEND_RECEIVED_PACKET( EMS_S2_ADD_FRIEND_FORCE_NOT );
    }
}

_IMPL_ON_FUNC( EMS_ADD_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq )
{
    if ( kPacket_.m_setMemberUID.empty() ) {
        START_LOG( cerr, L"추가할 길드원 데이터 없음, UserUID :" << kPacket_.m_dwUserUID ) << END_LOG;
        return;
    }


    START_LOG( clog, L"길드 멤버 리스트 추가 요청" )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_GuildUID )
        << BUILD_LOG( kPacket_.m_setMemberUID.size() ) << END_LOG;
        
    QUEUEING_EVENT_TO_DB( EMS_ADD_GUILD_MEMBER_LIST_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EMS_ADD_GUILD_MEMBER_LIST_ACK, KUpdateGuildMemberAck )
{
    kPacket_.m_nOK = NetError::NET_OK; // 아직 예외처리 없음

    std::map< DWORD, KFriend >& mapFriend = kPacket_.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers;

    std::map< DWORD, KFriend >::iterator mitMember;

    for ( mitMember = mapFriend.begin() ; mitMember != mapFriend.end() ; ++ mitMember ) {
        
        // 길드추가된 유저를 온라인 확인, 설정해주기
        KMSUserPtr spUser( SiKMSSimLayer()->m_kActorManager.GetByUID<KMSUser>( mitMember->first ) );

        if ( spUser ) {
            mitMember->second.m_bIsOnline = true;
        }
        
        m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.insert( std::make_pair( mitMember->first, mitMember->second ) );
    }

    START_LOG( clog, L"길드 멤버 리스트 추가 요청 응답" )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_GuildUID )
        << BUILD_LOG( kPacket_.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() )
        << BUILD_LOG( m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_ADD_GUILD_MEMBER_LIST_ACK );
}

_IMPL_ON_FUNC( EMS_DEL_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq )
{
    if ( kPacket_.m_setMemberUID.empty() ) {
        START_LOG( cerr, L"삭제할 길드원 데이터 없음, UserUID :" << kPacket_.m_dwUserUID ) << END_LOG;
        return;
    }
    
    KUpdateGuildMemberAck kPacket;

    std::map<DWORD,KFriend>::iterator mitMember;
    std::map<DWORD,KFriend>& mapMembers = m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers;

    std::set< DWORD >::iterator sit;
    for ( sit = kPacket_.m_setMemberUID.begin() ; sit != kPacket_.m_setMemberUID.end() ; ++sit ) {
        
        mitMember = mapMembers.find( *sit );
        
        if ( mitMember == mapMembers.end() ) {
            continue;
        }
        // 이건 꼭 안해줘도 상관없겠지만...(지운 멤버를 다시 응답으로 주는 것)
        kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.insert( std::make_pair( mitMember->first, mitMember->second ) );
        // 
        mapMembers.erase( mitMember );
    }

    kPacket.m_nOK = NetError::NET_OK;

    START_LOG( clog, L"길드 멤버 리스트 삭제 요청 응답" )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_GuildUID )
        << BUILD_LOG( kPacket.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() )
        << BUILD_LOG( m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() ) << END_LOG;

    SEND_PACKET( EMS_DEL_GUILD_MEMBER_LIST_ACK );
}

IMPL_ON_FUNC( EMS_GUILD_ALL_MEMBER_LIST_REQ )
{
    START_LOG( clog, L"길드 멤버 전체 리스트 요청" )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_GuildUID ) << END_LOG;

    QUEUEING_EVENT_TO_DB( EMS_GUILD_ALL_MEMBER_LIST_REQ, kPacket_ );
}

IMPL_ON_FUNC( EMS_GUILD_ALL_MEMBER_LIST_ACK )
{
    kPacket_.m_nOK = NetError::NET_OK;

    std::map< DWORD, KFriend >& mapFriend = kPacket_.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers;

    std::map< DWORD, KFriend >::iterator mitMember;

    for ( mitMember = mapFriend.begin() ; mitMember != mapFriend.end() ; ++ mitMember ) {

        // 길드추가된 유저를 온라인 확인, 설정해주기
        KMSUserPtr spUser( SiKMSSimLayer()->m_kActorManager.GetByUID<KMSUser>( mitMember->first ) );

        if ( spUser ) {
            mitMember->second.m_bIsOnline = true;
        }
    }

    START_LOG( clog, L"길드 멤버 리스트 업데이트 요청 응답" )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_mapFriendGroup[KGroup::GUILD_BASE].m_mapMembers.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EMS_GUILD_ALL_MEMBER_LIST_ACK );
}