#include "User.h"
#include <dbg/dbg.hpp>
#include "UserEvent.h"
#include "UserFSM.h"
#include "GSSimLayer.h"
#include "Room.h"
#include "Channel.h"
#include "ChannelManager.h"
#include <algorithm>    // std::find
#include "GSDBLayer.h"
#include "IPAdvantage.h" // MININUM_PCBANG_UID
#include "LogManager.h"
#include "GCHelper.h"
#include "support_User.h"
#include "ResultManager.h"
#include <boost/bind.hpp>
#include "LoudMessage.h"
#include "SignBoard.h"
#include "CnConnector.h"
#include "EnchantStrong.h"
#include "StrengthManager.h"
#include "AgentConnector.h"
#include "AgitHelper.h"
#include "GameServer.h"
#include "GCPoint.h"
#include "HeroDungeonManager.h"
#include "CYOUBilling\CYOUPacket.h"
#include "InfinityDungeon.h"
#include "SkillManager.h"

#define CLASS_TYPE  KUser

IMPL_ON_FUNC( EVENT_CHANGE_HOST_ADDRESS_NOT )   
{
    VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING ));

    JIF( m_pkRoom );

    // 050114. 방장의 포인터를 유지하도록 수정하였다.
    m_pkRoom->ChangeHostAddress( this, kPacket_.m_vecHostIP, kPacket_.m_vecHostPort );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( m_pkRoom->GetName() )
        << BUILD_LOG( kPacket_.m_vecHostIP.size() )
        << BUILD_LOG( kPacket_.m_vecHostPort.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_WHERE_REQ, std::wstring )
{
    VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY,
        KUserFSM::STATE_CHANNEL,
        KUserFSM::STATE_ROOM,
        KUserFSM::STATE_PLAYING ));

    KEVENT_WHERE_ACK kPacket;
    kPacket.m_nOK           = NetError::ERR_UNKNOWN;

    KUserPtr spkUser;
    spkUser = SiKChannelManager()->FindUser( kPacket_ );

    if( spkUser != NULL && GetStateID() >= KUserFSM::STATE_CHANNELLOBBY )
    {
        kPacket.m_usRoomID      = -1;   // RoomId 초기화
        // 010521
        kPacket.m_strChannel    = GetChannel()->GetChannelName();
        kPacket.m_iState        = GetStateID();
        kPacket.m_nOK           = NetError::NET_OK;

        if( GetRoom() != NULL )
            kPacket.m_usRoomID  = GetRoom()->m_usRoomID;
    }

    SEND_PACKET( EVENT_WHERE_ACK );

    START_LOG( clog, L"" )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_WHISPER_REQ, KPacketNameMsg )
{
    VERIFY_STATE(( 6, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL,
                      KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT ));

    KUserPtr spkUser;
    KChatData kNot;
    kNot.m_cChatType        = ( IsAdmin() ? KChatData::MSG_WHISPER_EX : KChatData::MSG_WHISPER ); // 운영자는 차단되지 않는 귓속말로 설정
    kNot.m_dwSenderUID      = GetUID();
    kNot.m_strSenderNick    = m_strNickName;
    kNot.m_strChatMsg       = kPacket_.m_strMsg;
    kNot.m_strReceiverNick  = kPacket_.m_strName;

    KEVENT_WHISPER_ACK kPacket;
    kPacket.m_nOK           = -99;
    kPacket.m_kChatData     = kNot;

    DWORD dwReceiveUserUID = 0;

    SET_ERROR( ERR_UNKNOWN );

    if( !CheckChatEnable() ) { // 채팅 블럭 상태.
        SET_ERR_GOTO( ERR_CHAT_13, END_PROC );
    }

    // 귓속말 전송 주기가 짧다.
    if ( ::GetTickCount() - GetTick(WHISPER_TICK) < 1000 ) {
        SET_ERR_GOTO( ERR_CHAT_09, END_PROC );
    }

    // 귓속말 대상유저 이름이 비었거나 메세지가 없음.
    if ( kPacket_.m_strName.empty() || kPacket.m_kChatData.m_strChatMsg.empty() ) {
        SET_ERR_GOTO( ERR_CHAT_10, END_PROC );
    }

    spkUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( SiKGSSimLayer()->TranslateNickToLogin( kPacket_.m_strName ) );
    if ( spkUser != NULL ) {
        // 대상유저 정보설정
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_kChatData.m_dwReceiverUID    = spkUser->GetUID();
        kPacket.m_kChatData.m_strReceiverNick  = spkUser->GetNickName();
        QueueingEvent( KUserEvent::EVENT_WHISPER_ACK, kPacket );
    }
    else {
        SiKGSSimLayer()->RemoveNickToLogin( kPacket_.m_strName );
        QUEUEING_EVENT_TO_DB( EVENT_WHISPER_REQ, kNot );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,      kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_CHAT_09, kPacket.m_nOK, 1 ); // 귓속말 전송주기가 짧다.
        ERR_CASE( ERR_CHAT_10, kPacket.m_nOK, 2 ); // 귓속말 대상 유저 이름이 비었거나 메세지가 없음.
        ERR_CASE( ERR_CHAT_11, kPacket.m_nOK, 3 ); // 방송용 서버에서 운영자에게 귓속말을 할 수 없음.
        ERR_CASE( ERR_CHAT_12, kPacket.m_nOK, 4 ); // 대상 유저를 찾을 수 없음.
        ERR_CASE( ERR_CHAT_13, kPacket.m_nOK, 5 ); // 채팅 블럭 상태임.
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( !IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_WHISPER_ACK );
    }

    SendChatLogNot( L"Whisper", -1, kNot.m_strChatMsg );
    SetTick( WHISPER_TICK );

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"Result : " << kPacket.m_nOK )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOG( kPacket_.m_strMsg )
        << BUILD_LOGc( kNot.m_cChatType )
        << BUILD_LOG( kNot.m_dwSenderUID )
        << BUILD_LOG( kNot.m_strSenderNick )
        << BUILD_LOG( kNot.m_strReceiverNick )
        << BUILD_LOG( kNot.m_strChatMsg ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_WHISPER_ACK )
{
    KUserPtr spkUser;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 같은 서버에 있는지.
    spkUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_kChatData.m_dwReceiverUID );
    if ( !spkUser ) {
        CenterChat( kPacket_.m_kChatData );
        return;
    }

    // 방송용 서버
    if ( SiKGSSimLayer()->CheckTypeFlag(ST_BROADCASTING) ) {
        // 초고수나 운영자에게는 귓말을 전송하지 않는다.
        if ( spkUser->GetAuthLevel() == KUser::AL_HIGH_RANKER || spkUser->IsAdmin() ) {
            SET_ERR_GOTO( ERR_CHAT_11, END_PROC );
        }
    }

    spkUser->SendPacket( KUserEvent::EVENT_CHAT_NOT, kPacket_.m_kChatData );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,      kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_CHAT_09, kPacket_.m_nOK, 1 ); // 귓속말 전송주기가 짧다.
        ERR_CASE( ERR_CHAT_10, kPacket_.m_nOK, 2 ); // 귓속말 대상 유저 이름이 비었거나 메세지가 없음.
        ERR_CASE( ERR_CHAT_11, kPacket_.m_nOK, 3 ); // 방송용 서버에서 운영자에게 귓속말을 할 수 없음.
        ERR_CASE( ERR_CHAT_12, kPacket_.m_nOK, 4 ); // 대상 유저를 찾을 수 없음.
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_WHISPER_ACK );

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"Result : " << kPacket_.m_nOK )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( kPacket_.m_kChatData.m_cChatType )
        << BUILD_LOG( kPacket_.m_kChatData.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_kChatData.m_strSenderNick )
        << BUILD_LOG( kPacket_.m_kChatData.m_dwReceiverUID )
        << BUILD_LOG( kPacket_.m_kChatData.m_strReceiverNick )
        << BUILD_LOG( kPacket_.m_kChatData.m_strChatMsg ) << END_LOG;
}

// #EVENT_INVITE_REQ
_IMPL_ON_FUNC( EVENT_INVITE_REQ, std::vector<DWORD> )   
{
    VERIFY_STATE(( 1, KUserFSM::STATE_ROOM ));

    JIF( m_pkRoom );
    JIF( m_pkChannel );

    KInviteInfo kPacket;
    std::vector<std::wstring> vecInviteFail;
    vecInviteFail.reserve( kPacket_.size() );

    kPacket.m_dwChannelUID      = m_pkChannel->GetUID();
    kPacket.m_usRoomID          = m_pkRoom->m_usRoomID;
    kPacket.m_nGameMode         = m_pkRoom->m_iGameMode;
    kPacket.m_nDifficult        = m_pkRoom->m_nDifficulty;
    kPacket.m_dwHostUID         = GetUID();
    kPacket.m_strNickName       = m_strNickName;
    kPacket.m_strRoomName       = m_pkRoom->GetRoomName();
    kPacket.m_strRoomPasswd     = m_pkRoom->m_strPasswd;
    kPacket.m_bCenterRoom       = false;

    KSerBuffer kbuff;
    SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_INVITE_NOT, kPacket, kbuff );

    int nCount = 0;
    std::vector<DWORD>::iterator  vit;
    for( vit = kPacket_.begin() ; vit != kPacket_.end(); vit++ )
    {
        KUserPtr spkUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vit );
        if( !spkUser ) continue;

        if( spkUser->m_bInviteDeny ) continue; // 초대 메세지 거절.

        if( spkUser->GetStateID() != KUserFSM::STATE_CHANNEL &&
            spkUser->GetStateID() != KUserFSM::STATE_SQUARE ) continue;

        // 헬모드에서 초대 할때 체크..
        if( !spkUser->HellModeCheck( m_pkRoom->m_iGameMode, kPacket.m_nDifficult ) )
        {
            vecInviteFail.push_back( spkUser->m_strNickName );
            continue;
        }

        if ( !spkUser->HeroDungeonCheck( m_pkRoom->m_iGameMode, kPacket.m_nDifficult ) ) {
            vecInviteFail.push_back(spkUser->m_strNickName);
            continue;
        }

        if ( !spkUser->DungeonTicketCheck( m_pkRoom->m_iGameMode ) ) {
            vecInviteFail.push_back( spkUser->m_strNickName );
            continue;
        }

        spkUser->SendPacket( kbuff, L"KUserEvent::EVENT_INVITE_NOT" );
        ++nCount;
    }   

    if( !vecInviteFail.empty() )
    {
        SEND_DATA( EVENT_INVITE_FAIL_NOT, vecInviteFail );
    }

    START_LOG( clog, L"방에서 초대 메세지 보냄. Name : " << m_strName )
        << BUILD_LOG( kPacket_.size() )
        << BUILD_LOG( nCount )
        << BUILD_LOG( kPacket.m_dwChannelUID )
        << BUILD_LOG( kPacket.m_dwChannelUID )
        << BUILD_LOG( kPacket.m_usRoomID )
        << BUILD_LOG( kPacket.m_nGameMode )
        << BUILD_LOG( kPacket.m_dwHostUID )
        << BUILD_LOG( kPacket.m_strNickName )
        << BUILD_LOG( kPacket.m_strRoomName )
        << BUILD_LOG( kPacket.m_strRoomPasswd )
        << BUILD_LOG( vecInviteFail.size() ) << END_LOG;

}

//_IMPL_ON_FUNC( EVENT_OPEN_USER_INFO_REQ, std::wstring )
//{
//    KEVENT_OPEN_USER_INFO_ACK kPacket;    
//    kPacket.m_nOK           = NetError::ERR_UNKNOWN;
//    KUserPtr spkUser;
//    spkUser = GetChannel()->FindUser( kPacket_.m_strName );
//
//    if( spkUser == NULL )
//    {
//        SEND_PACKET( EVENT_OPEN_USER_INFO_ACK );
//        return;
//    }
//
//    //kPacket.m_iNumLooses    = m_iNumLooses;
//    //kPacket.m_iNumMatches   = m_iNumMatches;
//    //kPacket.m_iNumWins      = m_iNumWins;
//    
//    //< 유저의 방정보
//    if( GetRoom() != NULL )
//        kPacket.m_iRoomID   = GetRoom()->m_usRoomID;  
//
//    else kPacket.m_iRoomID       = -1;
//    //>
//
//    // 유저의 채널정보
//    kPacket.m_strChannel    = GetChannel()->GetChannelName();
//    kPacket.m_strLogin      = GetName();   
//    kPacket.m_nOK           = NetError::NET_OK;
//    SEND_PACKET( EVENT_OPEN_USER_INFO_ACK );
//
//    START_LOG( clog, L"" )
//        << BUILD_LOG( GetName() ) 
//        << BUILD_LOG( kPacket_.m_strName ) << END_LOG;
//}

//IMPL_ON_FUNC( EVENT_DELETE_ITEM_NOT )
//{       
//    std::vector<KItemInfo>::iterator vit;
//    for( vit = m_vecInv.begin(); vit != m_vecInv.end(); vit++ )
//    {
//        // 050219. 클라이언트가 UID를 보냈다!!
////        if( vit->m_iUID == kPacket_.m_nCardUID ) // 일치하는 아이템을 찾았음
////        {
////            if( vit->m_iAmount <= 0 ) // 근데 수량이 0...
////            {
////                START_LOG( cerr, L"사용한 카드의 amount가 0임. name:" << m_strName
////                    << ", wanted uid:" << kPacket_.m_nCardUID << ", id:" << vit->m_dwGoodsID );
////            }
////            else
////            {
////                vit->m_iAmount--;
////                vit->m_bDirty = true;
////            }
////
////            return;
////        }
//    }
//
//    // 삭제하고자 하는 카드 아이디가 없음
//    START_LOG( cerr, L"사용한 아이템을 아예 산 적도 없음. name:" << m_strName
//        << ", uid:" << kPacket_.m_nCardUID );
//}

IMPL_ON_FUNC_NOPARAM( EVENT_AGREEMENT_PRIVATEINFO_NOT )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNELLOBBY ));

    START_LOG( clog, L"" ) << END_LOG;

    QUEUEING_ID_TO_DB( EVENT_AGREEMENT_PRIVATEINFO_NOT );
}

IMPL_ON_FUNC( EVENT_USE_DURATION_ITEM_REQ )
{
    _VERIFY_STATE((4,KUserFSM::STATE_PLAYING,KUserFSM::STATE_ROOM,KUserFSM::STATE_AGIT_JOIN,KUserFSM::STATE_AGIT));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_vecDurationUIDs.size() ) << END_LOG;

    std::vector<KItem> kPacket;

    // 의미 없는 아이템.
    KItem kItemEmpty;
    kItemEmpty.m_ItemUID     = 0;
    kItemEmpty.m_nCount   = 0;
    kItemEmpty.m_nInitCount      = 0;
    kItemEmpty.m_ItemID      = 0xFFFFFFFF;   // 의미는 없지만 값을 정해준다.

    std::vector<GCITEMUID>::iterator vitUids;
    for( vitUids = kPacket_.m_vecDurationUIDs.begin(); vitUids != kPacket_.m_vecDurationUIDs.end(); ++vitUids )
    {
        // 해당 UID를 가진 수량아이템을 찾는다.
        KItemPtr pkItem = m_kInventory.FindItemByItemUID( *vitUids );

        if( pkItem == NULL )
        {
            // 060614. florist. 운영자는 아이템 치트를 사용할 수 있다.
            if( !IsAdmin() )
            {
                //SiKLogManager()->Get( KLogManager::LT_DURATION )
                //    << "Name : " << m_strName << " UID : " << *vitUids << "  Time : " << KNC_TIME_STRING << GLOG::endl;

                // 해당 UID를 가진 아이템 정보를 수량 0으로 돌려주어, 그 아이템이 지워지도록 한다.
                kItemEmpty.m_ItemUID = *vitUids;
                kPacket.push_back( kItemEmpty );
            }

            // 아이템이 존재하지 않으면 참조해서는 안된다.
            continue;
        }

        // 여기서 부터 아이템은 존재함..
        if( pkItem->m_nCount > 0 )
        {
            --pkItem->m_nCount;

            START_LOG( clog, L"아이템 수량 감소. ID:" << pkItem->m_ItemID
                << L", 남은 수량:" << pkItem->m_nCount << L"회" ) << END_LOG;

            // 060630. jseop. 사용한 수량 아이템 기록.
            m_vecUsedDurationItem.push_back( pkItem->m_ItemID );
        }
        else // 수량이 이미 0인 아이템.
        {
            //SiKLogManager()->Get( KLogManager::LT_DURATION )
            //    << "Name : " << m_strName << " UID : " << *vitUids << "  Time : " << KNC_TIME_STRING << GLOG::endl;

            //  아이템이 지워지도록 유도 한다.
            kItemEmpty.m_ItemUID = *vitUids;
            kPacket.push_back( kItemEmpty );
        }

        // 클라이언트가 보내줌.
        //// 완전히 소모된 아이템을 장착하고 있다면 제거해주자.
        //if( pkDurationItem->m_dwCurrentNum <= 0 )
        //{
        //    ReleaseEquipElement( pkDurationItem->m_dwGoodsUID, KItemUnit::IT_QUANTITY );
        //    ReleasePetEquipElement( pkDurationItem->m_dwGoodsUID, KItemUnit::IT_QUANTITY );
        //}

        kPacket.push_back( *pkItem );
    }
    // 방원들에게 알려줘야 할 듯한데.... 
    SEND_PACKET( EVENT_USE_DURATION_ITEM_ACK );
}

IMPL_ON_FUNC( EVENT_STRONG_LEVELUP_REQ )
{
    _VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KEVENT_STRONG_LEVELUP_ACK       kPacket;
    KDB_EVENT_STRONG_LEVELUP_REQ    kDBPacket;
    kDBPacket.m_cPCBangType = 0;
    kDBPacket.m_cUserBenfitType = 0;
    KItemPtr pkMaterialItem;
    std::map<char, KCharacterInfo>::const_iterator cmit;
    int nStrongMaterialIndex = -99;
    float fGuildBonus = 0.f;
    USHORT usGuildLv = 0;
    USHORT usUserContributeGuildLv = 0;
    KItemPtr pkStrongItem; // 강화할 아이템

    SET_ERROR( ERR_UNKNOWN );

    pkStrongItem = m_kInventory.FindItemByItemUID( kPacket_.m_dwGoodsUID );
    if( !pkStrongItem ) {
        SET_ERR_GOTO( ERR_STRONG_10, END_PROC );
    }

    // 하락방지 아이템 체크
    if ( kPacket_.m_dwWeakAssistItemID > 0 ) {
        // 하락방지 아이템인가?
        if ( SiKEnchantStrong()->IsStrongAssistWeak( kPacket_.m_dwWeakAssistItemID ) == false ) {
            SET_ERR_GOTO( ERR_STRONG_06, END_PROC );
        }

        // 보유하고 있는가?
        if ( m_kInventory.FindItemByItemID( kPacket_.m_dwWeakAssistItemID ) == NULL ) {
            SET_ERR_GOTO( ERR_STRONG_07, END_PROC );
        }
    }

    // 강화재료 체크(기본:Gem)
    if ( SiKEnchantStrong()->GetStrongMaterialIndex( kPacket_.m_dwMaterialItemID, nStrongMaterialIndex ) == false ) {
        START_LOG( cerr, L"등록된 강화재료(Gem)가 아님. ItemID : " << kPacket_.m_dwMaterialItemID ) << END_LOG;
        SET_ERR_GOTO( ERR_STRONG_04, END_PROC );
    }

    pkMaterialItem = m_kInventory.FindItemByItemID( kPacket_.m_dwMaterialItemID );
    if ( pkMaterialItem == NULL ) {
        SET_ERR_GOTO( ERR_STRONG_04, END_PROC );
    }

    // 길드 보너스
    if ( m_kGuildUserInfo.m_dwGuildUID > 0 ) {
        fGuildBonus = GetGuildBonus( KGuildConfig::GA_ACCESSORY_ENCHANT_BONUS );
    }

    // DB패킷 세팅
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {
        kDBPacket.m_setCharType.insert( cmit->first );
    }
    kDBPacket.m_kPacket = kPacket_;
    kDBPacket.m_kMaterialItem = *pkMaterialItem;
    kDBPacket.m_cPCBangType = GetPCBangType();
    kDBPacket.m_nMaterialIndex = nStrongMaterialIndex;
    kDBPacket.m_fGuildBonus = fGuildBonus;
    kDBPacket.m_cUserBenfitType = GetUserBenfitType();

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_STRONG_LEVELUP_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK,   0 );   // 성공
        ERR_CASE( ERR_STRONG_04,        kPacket.m_nOK,  -5 );   // 강화재료(젬)이 부족합니다.
        ERR_CASE( ERR_STRONG_06,        kPacket.m_nOK,  -7 );   // 강화 요청시 어시스트 ItemID틀림.
        ERR_CASE( ERR_STRONG_07,        kPacket.m_nOK,  -8 );   // 강화 어시스트 아이템이 없음
        ERR_CASE( ERR_STRONG_10,        kPacket.m_nOK,  -1 );   // 강화 어시스트 아이템이 없음
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 이미 요청중인 작업입니다.
        default:
            kPacket.m_nOK = -99;
            START_LOG( cerr, L"강화 알수 없는 오류 , Name : " << m_strName )
                << BUILD_LOG( NetError::GetLastNetError() )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_STRONG_LEVELUP_ACK );
    }

   _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, L"Result : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGoodsID )
        << BUILD_LOG( kPacket_.m_dwGoodsUID )
        << BUILD_LOG( kPacket_.m_dwMaterialItemID )
        << BUILD_LOG( kPacket_.m_dwWeakAssistItemID )
        << BUILD_LOGc( kDBPacket.m_cPCBangType )
        << BUILD_LOG( fGuildBonus ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_STRONG_LEVELUP_ACK, KDB_EVENT_STRONG_LEVELUP_ACK )
{
    KEVENT_STRONG_LEVELUP_ACK kPacket = kPacket_.m_kPacket;
    bool bMsgGet = false;
    std::wstring strLoudMsg;

    if ( kPacket.m_nOK < 0 ) {
        SEND_PACKET( EVENT_STRONG_LEVELUP_ACK );
        START_LOG( cerr, L"강화 실패, Name : " << m_strName << L",nOK : " << kPacket.m_nOK ) << END_LOG;
        return;
    }

    // 아이템이 변경된 경우 인벤토리에서 찾아 ID를 바꿔준다.
    if ( !kPacket.m_vecDelItem.empty() && !kPacket.m_vecUpdateItem.empty() ) {
        KItem& kItemBefore = *kPacket.m_vecDelItem.begin();
        KItem& kItemAfter = *kPacket.m_vecUpdateItem.begin();

        START_LOG( clog, L"uid : " << kItemBefore.m_ItemUID << L", id : " 
            << kItemBefore.m_ItemID << L" -> " << kItemAfter.m_ItemID ) << END_LOG;

        KItemPtr pkItem = m_kInventory.FindItem( kItemBefore.m_ItemID, kItemBefore.m_ItemUID );

        if ( pkItem != NULL ){
            pkItem->m_ItemID = kItemAfter.m_ItemID;
            kItemAfter = *pkItem;
        }
        else {
            START_LOG( cerr, L"강화 시 강화 전 아이템을 찾지 못함! Name : " << m_strName )
                << BUILD_LOG( kItemBefore.m_ItemID )
                << BUILD_LOG( kItemBefore.m_ItemUID ) << END_LOG;
        }

        bMsgGet = SiKLoudMessage()->GetStrongMsg( (int)kItemAfter.m_ItemID, m_strName, strLoudMsg, GetLanguageCode() );
    }

    KItemPtr pkItem = m_kInventory.FindItemByItemID( kPacket_.m_kMaterialItem.m_ItemID );
    if ( pkItem != NULL ) {
        // 뺀 결과가 0 보다 작아지지는 않는지 체크해야? 혹은 그 전에 처리하고 있다면 생략
        pkItem->m_nCount = std::max<int>(0,kPacket_.m_kMaterialItem.m_nCount);
        pkItem->m_nInitCount = std::max<int>(0,kPacket_.m_kMaterialItem.m_nInitCount);
        kPacket.m_vecUpdateItem.push_back( *pkItem );

        if ( pkItem->m_nCount <= 0 )
            m_kInventory.RemoveItem( pkItem->m_ItemID, pkItem->m_ItemUID );
    }
    else {
        START_LOG( cwarn, L"이미 강화는 진행 해버렸고 젬은 없구나.. Name : " << m_strName ) << END_LOG;
    }

    // 하락방지 아이템을 썼으면 감소시키자
    if ( kPacket_.m_dwWeakAssistItemID > 0 ) {
        KItemPtr pkAssistItem;
        pkAssistItem = m_kInventory.FindItemByItemID( kPacket_.m_dwWeakAssistItemID );

        if ( pkAssistItem != NULL && pkAssistItem->m_nCount > 0 ) {
            pkAssistItem->m_nCount -= 1;
            kPacket.m_vecUpdateItem.push_back( *pkAssistItem );
        }
        else {
            START_LOG( cwarn, L"이미 강화는 진행 해버렸고 보조 아이템은 없구나.. Name : " << m_strName ) << END_LOG;
        }
    }

    START_LOG( clog, L"아이템 강화 종료. Name : " << m_strName )
        << BUILD_LOGc( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_kMaterialItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_dwWeakAssistItemID )
        << BUILD_LOG( kPacket_.m_kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_kPacket.m_vecUpdateItem.size() )
        << BUILD_LOG( kPacket_.m_kPacket.m_vecDelItem.size() ) << END_LOG;

    SEND_PACKET( EVENT_STRONG_LEVELUP_ACK );

    if ( bMsgGet ) {
        KSignBoardData kData;
        kData.m_dwColor         = 0L;
        kData.m_ItemID          = 0L;
        kData.m_dwSenderUID     = 0L;
        kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN_EX_GAME;
        kData.m_strMsg.swap( strLoudMsg );
        kData.m_strSenderNick.clear();
        SiKSignBoard()->QueueingAdminData( kData );
    }
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GP_REQ )
{
    //_VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    KEVENT_BUY_FOR_GP_ACK kPacket;
    KDB_EVENT_BUY_FOR_GP_REQ kDBPacket;

    SET_ERROR( ERR_UNKNOWN );
    if ( GetGP() <= 0 ) {
        SET_ERR_GOTO( ERR_BUY_ITEM_00, END_PROC );
    }

    GetDBUpdateData( kDBPacket.m_kData );
    kDBPacket.m_kBuyInfo.m_ItemID = kPacket_.m_ItemID;
    kDBPacket.m_kBuyInfo.m_nCount = kPacket_.m_nDuration;
    kDBPacket.m_kBuyInfo.m_nPeriod = kPacket_.m_nPeriod;
    FillBuyerData( kDBPacket.m_kBuyInfo );

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_FOR_GP_REQ, kDBPacket );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        //ERR_CASE( ERR_BUY_ITEM_02, kPacket.m_nOK, 1 ); // 필수데이터 동기화 실패.
        ERR_CASE( ERR_BUY_ITEM_00, kPacket.m_nOK, 2 ); // 잔액이 부족함.
        ERR_CASE( ERR_BUY_ITEM_01, kPacket.m_nOK, 3 ); // 아이템 삽입 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"GP아이템 구매시 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || kPacket.m_nOK == -98 )
        << NetError::GetLastNetErrMsg() << L", Name : " << m_strName << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EVENT_BUY_FOR_GP_ACK );
}

IMPL_ON_FUNC( EVENT_BUY_FOR_GP_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if( kPacket_.m_nUseGP < 0 ) {
        START_LOG( cwarn, L"GP 사용량이 음수임. UseGP : " << kPacket_.m_nUseGP ) << END_LOG;
    }

    bool bDBUpdate = false; // DB에서 GP 갱신 처리 했음.
    DecreaseGP( KGpStatInfo::GDT_GP_BUY, bDBUpdate, kPacket_.m_nUseGP );
    kPacket_.m_nUseGP = GetGP();

    m_kInventory.AddItemList( kPacket_.m_vecItems );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_BUY_ITEM_02, kPacket_.m_nOK, 1 ); // 잔액이 부족함.
        ERR_CASE( ERR_BUY_ITEM_00, kPacket_.m_nOK, 2 ); // 잔액이 부족함.
        ERR_CASE( ERR_BUY_ITEM_01, kPacket_.m_nOK, 3 ); // 아이템 삽입 실패
        ERR_CASE( ERR_BUY_ITEM_07, kPacket_.m_nOK, 4 ); // 구매 제한 초과
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"GP아이템 구매시 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_GP_ACK );    // 성공 여부와 상관 없이 클라이언트에게 패킷 전송.

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"GP로 아이템 구매 완료 Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nUseGP )
        << BUILD_LOG( m_kGCPoint.GetGCPoint() )
        << BUILD_LOG( kPacket_.m_vecItems.size() ) << END_LOG;
}

_IMPL_ON_FUNC(EVENT_BUY_FOR_GEM_REQ, KEVENT_BUY_FOR_GEM_REQ)
{
    _VERIFY_STATE((2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL));

    KEVENT_BUY_FOR_GEM_ACK kPacket;
    KDB_EVENT_BUY_FOR_GEM_REQ kDBPacket;
    KItemPtr pkGemItem;

    SET_ERROR(ERR_UNKNOWN);

    kDBPacket.m_kPacket = kPacket_;

    // 크리스탈 아이템이 있는지 확인 한다.
    pkGemItem = m_kInventory.FindItemByItemID(VIPCOIN_ID);
    if (pkGemItem == NULL) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, END_PROC);
    }

    if (pkGemItem->m_nCount <= 0) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, END_PROC);
    }

    kPacket_.m_moneyItemUID = pkGemItem->m_ItemUID;

    GetDBUpdateData(kDBPacket.m_kData);
    kDBPacket.m_kPacket = kPacket_;
    kDBPacket.m_moneyItem = *pkGemItem;

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_BUY_FOR_GEM_REQ, kDBPacket);

    SET_ERROR(NET_OK);
END_PROC:
    switch (NetError::GetLastNetError()) {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_BUY_ITEM_05, kPacket.m_nOK, 1); // 아이템 구매 하기에 크리스탈이 부족함
        ERR_CASE(ERR_BUY_ITEM_02, kPacket.m_nOK, 2); // 필수데이터 동기화 실패
        ERR_CASE(ERR_BUY_ITEM_01, kPacket.m_nOK, 3); // 아이템 넣기 실패
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98);
    default:
        kPacket.m_nOK = -99;
        START_LOG(cerr, L"크리스탈 아이템 구매시 알수 없는 오류 , Name : " << m_strName)
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
    }

    LOG_SUCCESS(IS_CORRECT(NET_OK) || kPacket.m_nOK == -98)
        << NetError::GetLastNetErrMsg() << L", Name : " << m_strName << END_LOG;

    if (!IS_CORRECT(NET_OK))
        SEND_PACKET(EVENT_BUY_FOR_GEM_ACK);
}

IMPL_ON_FUNC(EVENT_BUY_FOR_GEM_ACK)
{
    JIF(PacketHackCheck(nFrom_));

    KItemPtr pkItem;
    NetError::SetLastNetError(kPacket_.m_nOK);
    if (!IS_CORRECT(NET_OK)) {
        goto END_PROC;
    }
    // 아이템 집어 넣고
    m_kInventory.AddItemList(kPacket_.m_vecItem);

    // 크리스탈 아이템 정보 갱신.
    pkItem = m_kInventory.FindItem(kPacket_.m_moneyItem.m_ItemID, kPacket_.m_moneyItem.m_ItemUID);
    if (pkItem) {
        pkItem->m_nCount = std::max<int>(pkItem->m_nCount - kPacket_.m_nPrice, 0);
        kPacket_.m_moneyItem = *pkItem;
    }
    else {
        START_LOG(cerr, L"크리스탈로 구매 완료 했지만 젬 차감시 젬이 없음. Name : " << m_strName)
            << BUILD_LOG(kPacket_.m_nPrice) << END_LOG;
    }

END_PROC:

    switch (NetError::GetLastNetError()) {
        ERR_CASE(NET_OK, kPacket_.m_nOK, 0);
        ERR_CASE(ERR_BUY_ITEM_05, kPacket_.m_nOK, 1); // 아이템 구매 하기에 크리스탈이 부족함
        ERR_CASE(ERR_BUY_ITEM_02, kPacket_.m_nOK, 2); // 필수데이터 동기화 실패
        ERR_CASE(ERR_BUY_ITEM_01, kPacket_.m_nOK, 3); // 아이템 넣기 실패

        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98);

    default:
        kPacket_.m_nOK = -99;
        START_LOG(cerr, L"크리스탈 아이템 구매시 알수 없는 오류 , Name : " << m_strName)
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
    }

    _LOG_SUCCESS(kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98, L"크리스탈 으로 아이템 구매 응답 : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(kPacket_.m_nOK)
        << BUILD_LOG(kPacket_.m_nPrice)
        << BUILD_LOG(kPacket_.m_vecItem.size()) << END_LOG;

    SEND_RECEIVED_PACKET(EVENT_BUY_FOR_GEM_ACK);
}

_IMPL_ON_FUNC(EVENT_BUY_FOR_CRYSTAL_REQ, KEVENT_BUY_FOR_GEM_REQ)
{
    KEVENT_BUY_FOR_CRYSTAL_ACK kPacket;
    KDB_EVENT_BUY_FOR_CRYSTAL_REQ kDBPacket;
    KItemPtr pkCrystalItem;

    SET_ERROR(ERR_UNKNOWN);

    kDBPacket.m_kPacket = kPacket_;
    kDBPacket.m_fDiscountRatio = 1.f;

    pkCrystalItem = m_kInventory.FindItemByItemID(CRYSTAL_GOODS_ID);
    if (pkCrystalItem == NULL) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, END_PROC);
    }
    if (pkCrystalItem->m_nCount <= 0) {
        SET_ERR_GOTO(ERR_BUY_ITEM_03, END_PROC);
    }

    kPacket_.m_moneyItemUID = pkCrystalItem->m_ItemUID;

    GetDBUpdateData(kDBPacket.m_kData);
    kDBPacket.m_kPacket = kPacket_;
    kDBPacket.m_moneyItem = *pkCrystalItem;

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_BUY_FOR_CRYSTAL_REQ, kDBPacket);

    SET_ERROR(NET_OK);
END_PROC:
    switch (NetError::GetLastNetError()) {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_BUY_ITEM_05, kPacket.m_nOK, 1); // ¾ÆÀÌÅÛ ±¸¸Å ÇÏ±â¿¡ Å©¸®½ºÅ»ÀÌ ºÎÁ·ÇÔ
        ERR_CASE(ERR_BUY_ITEM_02, kPacket.m_nOK, 2); // ÇÊ¼öµ¥ÀÌÅÍ µ¿±âÈ­ ½ÇÆÐ
        ERR_CASE(ERR_BUY_ITEM_01, kPacket.m_nOK, 3); // ¾ÆÀÌÅÛ ³Ö±â ½ÇÆÐ
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98);
    default:
        kPacket.m_nOK = -99;
        START_LOG(cerr, L"Error, Name : " << m_strName)
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
    }

    LOG_SUCCESS(IS_CORRECT(NET_OK) || kPacket.m_nOK == -98)
        << NetError::GetLastNetErrMsg() << L", Name : " << m_strName << END_LOG;

    if (!IS_CORRECT(NET_OK))
        SEND_PACKET(EVENT_BUY_FOR_CRYSTAL_ACK);
}

IMPL_ON_FUNC( EVENT_BUY_FOR_CRYSTAL_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    m_kInventory.AddItemList( kPacket_.m_vecItem );

    pkItem = m_kInventory.FindItem( kPacket_.m_moneyItem.m_ItemID, kPacket_.m_moneyItem.m_ItemUID );
    if ( pkItem ) {
        pkItem->m_nCount = std::max<int>( pkItem->m_nCount - kPacket_.m_nPrice, 0 );
        kPacket_.m_moneyItem = *pkItem;
    }
    else {
        START_LOG( cerr, L"크리스탈로 구매 완료 했지만 젬 차감시 젬이 없음. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_nPrice ) << END_LOG;
    }

END_PROC:

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_BUY_ITEM_05, kPacket_.m_nOK, 1 ); // 아이템 구매 하기에 크리스탈이 부족함
        ERR_CASE( ERR_BUY_ITEM_02, kPacket_.m_nOK, 2 ); // 필수데이터 동기화 실패
        ERR_CASE( ERR_BUY_ITEM_01, kPacket_.m_nOK, 3 ); // 아이템 넣기 실패

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );

    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"크리스탈 아이템 구매시 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    if ( kPacket_.m_nOK == 0 ) {
        SendVipEventUserRewardUpdateNot();
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98, L"크리스탈 으로 아이템 구매 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nPrice )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_CRYSTAL_ACK );
}

// Buy For GP 로 대체 가능.
//IMPL_ON_FUNC( EVENT_BUY_COUNT_FOR_GP_REQ )
//{
//    _VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
//
//    START_LOG( clog, L"GP로 수량지정 아이템을 구매 합니다." )
//        << BUILD_LOG( kPacket_.m_dwGoodsID )
//        << BUILD_LOG( kPacket_.m_dwGoodsUID )
//        << BUILD_LOG( kPacket_.m_iCount ) << END_LOG;
//
//    KDB_EVENT_BUY_COUNT_FOR_GP_REQ kPacket;
//    GetDBUpdateData( kPacket.m_kData );
//    kPacket.m_kPacket = kPacket_;
//
//    QUEUEING_EVENT_TO_DB( EVENT_BUY_COUNT_FOR_GP_REQ, kPacket );
//}
//
//IMPL_ON_FUNC( EVENT_BUY_COUNT_FOR_GP_ACK )
//{
//    if( kPacket_.m_cOK == 0 )
//    {
//        LIF( InsertIntoInventory( kPacket_.m_vecItem ) );
//        LIF( InsertIntoInventory( kPacket_.m_vecDurationItem ) );
//    }
//
//    m_iInitGP       = kPacket_.m_dwGP;
//    m_iGamePoint    = m_iInitGP;
//
//    SEND_RECEIVED_PACKET( EVENT_BUY_COUNT_FOR_GP_ACK );
//
//    LOG_SUCCESS( kPacket_.m_cOK == 0 || kPacket_.m_cOK == -5 ) // 잔액부족은 로그 남기지 않는다
//        << BUILD_LOG( m_strName )
//        << BUILD_LOGc( kPacket_.m_cOK )
//        << BUILD_LOG( kPacket_.m_dwGP )
//        << BUILD_LOG( kPacket_.m_vecItem.size() )
//        << BUILD_LOG( kPacket_.m_vecDurationItem.size() ) << END_LOG;
//}

IMPL_ON_FUNC_NOPARAM( EVENT_QUERY_INVENTORY_INFO_REQ )
{
    VERIFY_STATE((5,KUserFSM::STATE_CHANNELLOBBY,KUserFSM::STATE_CHANNEL,KUserFSM::STATE_ROOM,KUserFSM::STATE_PLAYING,KUserFSM::STATE_AGIT));

    if ( false == SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_NTREEV ) ) {
        JIF( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) );
    }

    QUEUEING_ID_TO_DB_CHECK( EVENT_QUERY_INVENTORY_INFO_REQ );

END_PROC:
    START_LOG( clog, L"유저 인벤토리 정보를 요청하였습니다.." ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_QUERY_INVENTORY_INFO_ACK )
{

    // TODO : 가차열쇠 아이템 구매 횟수를 더해주고 있었다. 
    // 현재의 수량과 새로 받은 수량의 차이가 있으면 이번 구매로 처리 하고 있었다.
    // TODO : 이부분은 구매 제한 루틴을 클라에서 호출 하는 형태로 변경 되어야 한다.
    // TODO : 피씨방 아이템이 있다면 복사 해둔다.
    // TODO : 수량 아이템 사용한 만큼 처리 한다.
    
    m_kInventory.AdjustQueryInventory( KUser::GT_INGAME, kPacket_.m_vecInv );

    m_kInventory.MakeCharacterInventory( m_cCurrentCharacter, kPacket_.m_vecInv );

    // 압축해서 보내자.
    SEND_COMPRESS_PACKET( EVENT_QUERY_INVENTORY_INFO_ACK, kPacket_ );

    START_LOG( clog, L"유저 인벤토리 정보 전송" )
        << BUILD_LOG( kPacket_.m_vecInv.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_EQUIP_ITEM_REQ, KEquipUser )
{
    _VERIFY_STATE(( 7, KUserFSM::STATE_CHANNELLOBBY, 
        KUserFSM::STATE_CHANNEL, 
        KUserFSM::STATE_ROOM,
        KUserFSM::STATE_GUILD_ROOM,
        KUserFSM::STATE_AGIT_JOIN,
        KUserFSM::STATE_AGIT,
        KUserFSM::STATE_PLAYING ));

    std::map<char,std::vector<KEquipItemInfo> > mapEquipCopy;
    KEquipUser              kBroadPacket; // 젠장 근본적으로 고쳐야 하는데.. 임시 방편이다.. ㅜㅜ..
    KEVENT_EQUIP_ITEM_ACK   kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    kPacket_.m_dwUID = GetUID();

    if( !IsCharExist( kPacket_.m_cCharType ) )
    {
        START_LOG( cerr, L"장착 정보 변경시 해당 캐릭이 없는데 요청. name : " << m_strName 
            << ", charType : " << static_cast<int>(kPacket_.m_cCharType) ) << END_LOG;
        SET_ERR_GOTO( ERR_EQUIP_00, END_PROC );
    }

    // 보내온 장착 아이템들이 내 인벤토리에 있는 것들인지 검증
    //캐릭터만 변경했을 때, 장착정보 변경 없음
    if( !kPacket_.m_mapEquipInfo.empty() ) {

        int nCount = 0;

        nCount = CheckValidationEquipReq( kPacket_.m_mapEquipInfo );
        if( nCount > 0 )
        {
            START_LOG( cwarn, L"?이 애정이 있어서 처리를 해준다. Name : " << m_strName )
                << BUILD_LOG( nCount )  << END_LOG;
        }

        // 릴리즈에서 이미 다 소모한 아이템이 있는 경우가 있다. 이부분 처리를 위해서 사본을 전달해준다.
        kBroadPacket = kPacket_;

        nCount = CheckValidationReleaseReq( kPacket_.m_mapEquipInfo );
        if( nCount > 0 )
        {
            START_LOG( cwarn, L"?이 애정이 있어서 릴리즈 처리를 해준다. Name : " << m_strName )
                << BUILD_LOG( nCount )  << END_LOG;
        }

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) { 
            START_LOG( cwarn,  L"--- Dump EquipItemReq ---" ) << END_LOG;
            START_LOG( cwarn,  L"장착정보 변경요청.. Login : " << kPacket_.m_strLogin << L",CharType : " << (int)kPacket_.m_cCharType << L",EquipInfo size : " << kPacket_.m_mapEquipInfo.size() ) << END_LOG;
            std::map<char, KChangeEquipment >::iterator mit;
            for ( mit = kPacket_.m_mapEquipInfo.begin() ; mit != kPacket_.m_mapEquipInfo.end() ; ++mit ) {
                START_LOG( cwarn,  L"char : " << (int)mit->first
                    << L",PetUID : " << mit->second.m_equipPetInfo.m_dwUID
                    << L",PetID : " << mit->second.m_equipPetInfo.m_dwID
                    << L",JobLv : " << (int)mit->second.m_cCurrentJobLV ) << END_LOG;

                START_LOG( cwarn,  L" EquipSize : " << mit->second.m_vecEquipItems.size() ) << END_LOG;
                std::vector<KEquipItemInfo>::iterator vitEquip;
                for ( vitEquip = mit->second.m_vecEquipItems.begin() ; vitEquip != mit->second.m_vecEquipItems.end() ; ++vitEquip ) {
                    START_LOG( cwarn,  L"  ItemID : " << vitEquip->m_dwID << L",ItemUID : " << vitEquip->m_dwUID << L",EquipLv : " << (int)vitEquip->m_nEquipLevel ) << END_LOG;
                }

                START_LOG( cwarn,  L" ReleaseSize : " << mit->second.m_vecReleaseItems.size() ) << END_LOG;
                std::vector<KEquipItemInfo>::iterator vitRelease;
                for ( vitRelease = mit->second.m_vecReleaseItems.begin() ; vitRelease != mit->second.m_vecReleaseItems.end() ; ++vitRelease ) {
                    START_LOG( cwarn,  L"  ItemID : " << vitRelease->m_dwID << L",ItemUID : " << vitRelease->m_dwUID << L",EquipLv : " << (int)vitRelease->m_nEquipLevel ) << END_LOG;
                }

                START_LOG( cwarn,  L" ChangeWeapon.. ItemID : " << mit->second.m_kChangeWeaponItem.m_dwID << L",ItemUID" << mit->second.m_kChangeWeaponItem.m_dwUID ) << END_LOG;
                START_LOG( cwarn,  L"------------------------------------------------" ) << END_LOG;
            }

            START_LOG( cwarn,  L"--- End Dump ---" ) << END_LOG;
        }

        if( !CopyEquipReq( kPacket_.m_mapEquipInfo, mapEquipCopy ) )
        {
            SET_ERR_GOTO( ERR_EQUIP_01, END_PROC );
        }

        if( !ApplyEquipItem( mapEquipCopy, kPacket_.m_mapEquipInfo ) )
        {
            SET_ERR_GOTO( ERR_EQUIP_02, END_PROC );
        }

        ApplyChangeWeapon( kPacket_.m_mapEquipInfo );
        // 장착 정보 변경된 캐릭터 정보 가지고 있기.
        UpdateCharEquipInfo( m_cCurrentCharacter );
    }

    if ( GetStateID() == KUserFSM::STATE_ROOM || GetStateID() == KUserFSM::STATE_PLAYING ) {
        if ( m_pkRoom != NULL ) {
            m_pkRoom->SyncCharInfo( m_strName, kPacket_.m_cCharType );
            m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_EQUIP_ITEM_BROAD, kBroadPacket, true );

            START_LOG( clog, L"룸에서 아이템을 장착하였습니다." )
                << BUILD_LOG( GetStateID() )
                << BUILD_LOG( GetStateIDString() ) << END_LOG;
        }
        else {
            START_LOG( cwarn, L"방안에 있는데 방정보가 없음. 일단 아이템은 장착함." )
                << BUILD_LOG( GetStateID() )
                << BUILD_LOG( GetStateIDString() ) << END_LOG;

            SEND_COMPRESS_PACKET( EVENT_EQUIP_ITEM_BROAD, kBroadPacket );
        }
    } else if ( GetStateID() == KUserFSM::STATE_AGIT ) {
        if ( m_dwStayAgitUID > 0 ) {
            // 아지트 데이터 동기화 패킷
            KEAG_AGIT_LOADING_COMPLETE_REQ KAgPacket;
            KAgPacket.m_dwAgitUID = m_dwStayAgitUID;
            if ( GetAgitUserInfo( KAgPacket.m_kUserInfo ) ) {
                int nSID = SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID );
                SiKAgentConnector()->SendPacket( nSID, KAgentEvent::EAG_UPDATE_AGIT_USER_INFO_NOT, KAgPacket );
            }

            // 아지트 유저들에게 동기화 패킷 전달.
            KSerBuffer kEventBuff;
            SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_EQUIP_ITEM_BROAD, kBroadPacket, kEventBuff );

            int nSID = SiKAgitHelper()->GetAgitSID( m_dwStayAgitUID );
            SiKAgentConnector()->SendAgitRelayPacket( nSID, KUserEvent::EVENT_EQUIP_ITEM_BROAD, kEventBuff, GetUID(), m_dwStayAgitUID );

            START_LOG( clog, L"아지트에서 아이템을 장착하였습니다." ) << END_LOG;
        }
        else {
            START_LOG( cwarn, L"아지트에 있는데, 아지트정보가 없음. 일단 아이템은 장착." ) << END_LOG;
            SEND_COMPRESS_PACKET( EVENT_EQUIP_ITEM_BROAD, kBroadPacket );
        }
    } else {
        START_LOG( clog, L"아이템 장착 성공. 현재 상태(StateID) : " << GetStateID() ) << END_LOG;
        SEND_COMPRESS_PACKET( EVENT_EQUIP_ITEM_BROAD, kBroadPacket );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK,       kPacket.m_nOK, 0 ); // 성공
    ERR_CASE( ERR_EQUIP_00, kPacket.m_nOK, 1 ); // 장착 정보 변경시 해당 캐릭이 없는데 요청.
    ERR_CASE( ERR_EQUIP_01, kPacket.m_nOK, 2 ); // 장착 해제 정보 복사본 처리에서 에러.
    ERR_CASE( ERR_EQUIP_02, kPacket.m_nOK, 3 ); // 장착 아이템 및 펫 장착 처리에서 에러.
    ERR_CASE( ERR_EQUIP_03, kPacket.m_nOK, 4 ); // 소유펫이 아닌데 장착할려고 했음.
    default:
        START_LOG( cerr, L"알수 없는 에러 발생 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 ) // 실패 했을때만 ACK를 보낸다.
    {
        //캐릭터 정보 다시 가져 와서 셋팅.
        GetInDoorCharInfo( kPacket.m_vecCharacterInfo );
        kPacket.m_cCharType = m_cCurrentCharacter;
        SEND_PACKET( EVENT_EQUIP_ITEM_ACK );
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << L"장착 정보 변경 Name : " << m_strName << dbg::endl
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_mapEquipInfo.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SELL_ITEM_REQ, KSimpleItem )
{
    _VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KEVENT_SELL_ITEM_ACK            kPacket;
    KDB_EVENT_SELL_ITEM_INFO        kDBPacket;
    KItemPtr pkItem;
    KItemPtr pkEquipItem;

    SET_ERROR( ERR_UNKNOWN );
    // 수량 영구 아이템과 기간 영구 아이템만 팔수 있다.
    // 기간은 판매 할 수 없고 삭제 한다.

    if( kPacket_.m_dwUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID() )
    {
        SET_ERR_GOTO( ERR_SELL_ITEM_01, END_PROC ); // PC방 아이템을 팔려고 시도함.
    }

    // Item을 보유 하고 있는가?
    pkItem = m_kInventory.FindItem( kPacket_.m_dwID, kPacket_.m_dwUID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_SELL_ITEM_00, END_PROC ); // 아이템을 보유하고 있지 않음.
    }

    // 수량이 0이면 팔수 없음.(음수이면 수량 아이템이 아님. )
    if ( pkItem->m_nCount == 0 ) {
        SET_ERR_GOTO( ERR_SELL_ITEM_03, END_PROC );
    }

    // 강화석이 박힌 아이템은 강화석까지 같이 팔아버림.
    if( pkItem->m_EnchantEquipItemUID > 0 && pkItem->m_ItemID != SiKStrengthManager()->GetStrengthItemID() ) {
        pkEquipItem = m_kInventory.FindDurationItemByItemUID( pkItem->m_EnchantEquipItemUID );
        if( pkEquipItem ) {
            kDBPacket.m_vecSellItem.push_back( *pkEquipItem );
        }
    }

    kDBPacket.m_vecSellItem.push_back( *pkItem );
    GetDBUpdateData( kDBPacket.m_kData );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SELL_ITEM_REQ, kDBPacket );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK,   0 ); // 성공
        ERR_CASE( ERR_SELL_ITEM_00,     kPacket.m_nOK,   1 ); // 아이템이 인벤토리에 존재하지 않음
        ERR_CASE( ERR_SELL_ITEM_01,     kPacket.m_nOK,   2 ); // PC방 아이템은 팔수 없음
        ERR_CASE( ERR_SELL_ITEM_02,     kPacket.m_nOK,   3 ); // 기간제 아이템은 팔수가 없음.
        ERR_CASE( ERR_SELL_ITEM_03,     kPacket.m_nOK,   4 ); // 현재 수량이 0입니다
        ERR_CASE( ERR_DB_COMMON_03,     kPacket.m_nOK,   5 ); // 필수 데이터 동기화 실패.
        ERR_CASE( ERR_SELL_ITEM_04,     kPacket.m_nOK,   6 ); // 아이템 수량 동기화 실패.
        ERR_CASE( ERR_SELL_ITEM_05,     kPacket.m_nOK,   7 ); // 아이템 수량 동기화 실패.
        ERR_CASE( ERR_SELL_ITEM_06,     kPacket.m_nOK,   8 ); // 강화석 장착된 아이템 팔수 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아이템 판매 DB 처리 오류.
    default:
        START_LOG( cerr, L"아이템 판매시 알수없는 오류. Name : " << GetName() )
            << L"Err Code : " << NetError::GetLastNetError() << dbg::endl
            << L"Err Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_SELL_ITEM_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || kPacket.m_nOK == -98 )
        << L" 아이템 판매 요청 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket_.m_cItemType )
        << BUILD_LOG( kPacket_.m_dwID )
        << BUILD_LOG( kPacket_.m_dwUID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_SELL_ITEM_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    KItemPtr pkItem;
    KItemPtr pkEquipItem;
    int nGPDiff = 0;
    std::vector<KItem>::iterator vit;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        goto END_PROC;
    }

    for( vit = kPacket_.m_vecSellItem.begin(); vit != kPacket_.m_vecSellItem.end(); ++vit ) {
        LIF( m_kInventory.RemoveItem( vit->m_ItemID, vit->m_ItemUID ) );

        // 2008-02-19 woosh. Room에서 GCClub 판매시 처리
        if ( m_pkRoom && IsPremiumItem( vit->m_ItemID ) == true ) {
            KPremiumInfo kInfo;
            CheckInventoryPremiumItem(kInfo);

            m_pkRoom->UpdatePremiumData( m_strName, kInfo ); // 아이템 돌면서 GCClub 정보 갱신

            NotifyPremiumToRoomUsers();
        }
    }

    if ( kPacket_.m_vecItem.empty() && kPacket_.m_nGP > 0 ) { // 판매 후 보상이 GP 인 경우
        SetGP( kPacket_.m_nGP );
        SetInitGP( GetGP() );
    }

    kPacket_.m_nGP   = GetGP();
    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK,   0 ); // 성공
        ERR_CASE( ERR_SELL_ITEM_00,     kPacket_.m_nOK,   1 ); // 아이템이 인벤토리에 존재하지 않음
        ERR_CASE( ERR_SELL_ITEM_01,     kPacket_.m_nOK,   2 ); // PC방 아이템은 팔수 없음
        ERR_CASE( ERR_SELL_ITEM_02,     kPacket_.m_nOK,   3 ); // 기간제 아이템은 팔수가 없음.
        ERR_CASE( ERR_SELL_ITEM_03,     kPacket_.m_nOK,   4 ); // 현재 수량이 0입니다
        ERR_CASE( ERR_DB_COMMON_03,     kPacket_.m_nOK,   5 ); // 필수 데이터 동기화 실패.
        ERR_CASE( ERR_SELL_ITEM_04,     kPacket_.m_nOK,   6 ); // 아이템 수량 동기화 실패.
        ERR_CASE( ERR_SELL_ITEM_05,     kPacket_.m_nOK,   7 ); // 아이템 수량 동기화 실패.
        ERR_CASE( ERR_SELL_ITEM_06,     kPacket_.m_nOK,   8 ); // 강화석 장착된 아이템 팔수 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아이템 판매 DB 처리 오류.
    default:
        START_LOG( cerr, L"아이템 판매 응답시 알수없는 오류. Name : " << GetName() )
            << L", Err Code : " << NetError::GetLastNetError() << dbg::endl
            << L", Err Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_SELL_ITEM_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L" 아이템 판매 응답 Err Msg : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP ) 
        << BUILD_LOG( kPacket_.m_vecSellItem.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;
}

//_IMPL_ON_FUNC( EVENT_CHAR_INVENTORY_REQ, char )
//{
//    VERIFY_STATE((5,KUserFSM::STATE_CHANNELLOBBY,KUserFSM::STATE_CHANNEL,KUserFSM::STATE_ROOM,KUserFSM::STATE_PLAYING,KUserFSM::STATE_AGIT));
//
//    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHAR_INVENTORY_REQ, kPacket_ );
//
//END_PROC:
//    ;
//}

_IMPL_ON_FUNC( EVENT_VERIFY_INVENTORY_NOT, KEVENT_QUERY_INVENTORY_INFO_ACK )
{
    m_kInventory.AdjustQueryInventory( KUser::GT_INGAME, kPacket_.m_vecInv );

    m_kInventory.MakeCharacterInventory( kPacket_.m_cCharacter, kPacket_.m_vecInv );

    SendInventoryInfo( kPacket_.m_vecInv );
}

IMPL_ON_FUNC(EVENT_CHANGE_CHARACTER_INFO_REQ)
{
    VERIFY_STATE((5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING, KUserFSM::STATE_AGIT));

    KDB_EVENT_CHANGE_CHARACTER_INFO_REQ kDBPacket;
    KDB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ kDBLogPacket;

    if (GetCurrentCharType() != kPacket_.m_cPrevChar) {
        START_LOG(cerr, L"����-Ŭ���̾�Ʈ�� ���� ĳ���Ͱ� �ٸ���, User: " << GetName())
            << END_LOG;
        // ���� �ȵǴ� ������� �ش� ��쿡 �������� �˰� �ִ� ������ �����Ѵ�.
        kPacket_.m_cPrevChar = m_cCurrentCharacter;
    }

    kDBPacket.m_cPrevChar = kPacket_.m_cPrevChar;
    kDBPacket.m_cNextChar = kPacket_.m_cNextChar;
    GetCharSlotNum(kDBPacket.m_dwSlotNum);

    if (SiKGCPoint()->IsGCPointSwitch() == true) {
        m_kCalendar.GetCalendarDBInfo(kDBPacket.m_vecCalendar);
    }

    kDBPacket.m_mapCharacterInfo = m_mapCharacterInfo;
    m_kInventory.GetDiffDurationItemAndUpdate(kDBPacket.m_vecCountItemInv);

    GetPetCacheData(kDBPacket.m_mapPetInfo);
    GetDepotPetCacheData(kDBPacket.m_mapDepotPetInfo);
    GetDBUpdateData(kDBPacket.m_kUserData);
    GetCharEquipInfo(kDBPacket.m_setChangeEquipInfo);
    kDBPacket.m_vecMissionSlot = m_vecMissionSlot;
    kDBPacket.m_vecCollectionSlot = m_vecCollectionSlot;
    kDBPacket.m_mapSkillSetDel = m_mapInitSkillSets;
    kDBPacket.m_mapSkillSetAdd = m_mapSkillSets;
    kDBPacket.m_setAddReserveSkill = m_setAddReserveSkill;
    kDBPacket.m_setDeleteReserveSkill = m_setDeleteReserveSkill;
    GetUserDifficultInfo(kDBPacket.m_mapDifficulty);
    kDBPacket.m_mapOldDifficulty = m_mapOldDifficulty;
    ExtractDiffLookEquip(kDBPacket.m_mapDiffLookEquip);
    kDBPacket.m_cPCBangType = GetPCBangType();
    GetConnectionGiftBoxUpdateData(kDBPacket.m_kConnectionGiftBoxUpdateData);
    kDBPacket.m_mapDungeonPersonalRecordDiff = m_mapDungeonPersonalRecordDiff[kPacket_.m_cPrevChar];
    kDBPacket.m_bInPvP = false;

    QUEUEING_EVENT_TO_DB_CHECK(DB_EVENT_CHANGE_CHARACTER_INFO_REQ, kDBPacket);

    // ����â�� ���� ������ �α� ����.
    GetInvenDepotMoveInfo(kDBLogPacket);
    //QUEUEING_EVENT_TO_DB( DB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ, kDBLogPacket );

    SetCurrentChar(kPacket_.m_cNextChar); // ���� Ŭ���̾�Ʈ�� ���� ĳ���ͷ� ����.

    // �Ϸ� �̼� ����Ʈ ���
    QUEUEING_ID_TO_DB(DB_EVENT_FINISHED_MISSION_LIST_REQ);

    {
        QUEUEING_ID_TO_DB(DB_EVENT_INIT_POST_LETTER_LIST_REQ);
    }

END_PROC:
    ;
}

IMPL_ON_FUNC(EVENT_CHANGE_CHARACTER_INFO_ACK)
{
    kPacket_.m_nOK = 0;
    std::wstring wstrCharNickName;

    m_kInventory.AdjustQueryInventory(KUser::GT_CHANGE_CHAR, kPacket_.m_vecInv);
    m_kInventory.MakeCharacterInventory(kPacket_.m_cCharacter, kPacket_.m_vecInv);

    std::map< char, KCharacterInfo >::iterator mitCharInfo;
    mitCharInfo = m_mapCharacterInfo.find(kPacket_.m_cCharacter);
    if (mitCharInfo != m_mapCharacterInfo.end()) {
        kPacket_.m_nInvenCapacity = mitCharInfo->second.m_nInvenCapacity;
        kPacket_.m_nLookInvenCapacity = mitCharInfo->second.m_nLookInvenCapacity;
        SetCharEquipItemDetail(kPacket_.m_vecInv, mitCharInfo->second.m_vecEquipItems);
        SetCharLookEquipDetail(kPacket_.m_vecInv, mitCharInfo->second.m_vecLookEquips);
        SetCharChangeWeaponEquipDetail(kPacket_.m_vecInv, mitCharInfo->second.m_kChangeWeaponItem);
        kPacket_.m_kChangeWeaponItem = mitCharInfo->second.m_kChangeWeaponItem;
        kPacket_.m_bChangeWeaponLock = mitCharInfo->second.m_bChangeWeaponLock;
        SetCharEquipPet(kPacket_.m_mapPetInfo, mitCharInfo->second.m_kEquipPetInfo);
        kPacket_.m_kEquipPetInfo = mitCharInfo->second.m_kEquipPetInfo;
        kPacket_.m_vecPetGlyphCharData = mitCharInfo->second.m_vecPetGlyphCharData;
        kPacket_.m_dwPetGlyphSize = mitCharInfo->second.m_dwPetGlyphSize;
    }

    m_mapPetInfo = kPacket_.m_mapPetInfo; // �� ���� ����.
    m_vecCollectionSlot = kPacket_.m_vecCollection; // Īȣ ���� ����.
    m_vecMissionSlot = kPacket_.m_vecMissionSlot; // �̼����� ����.
    kPacket_.m_vecFontVector = m_vecFontVector;
    kPacket_.m_iPvExp = m_iPvExp;

    // ��ų ���� ���� �ʱ�ȭ
    // DB ����ȭ�� �߱� ������ Init���� ���簪�� ����ȭ.
    m_setAddReserveSkill.clear();
    m_setDeleteReserveSkill.clear();
    m_mapInitSkillSets = m_mapSkillSets;

    m_mapOldDifficulty.clear();
    if (false == kPacket_.m_bReadOldModwLevel) {
        m_mapOldDifficulty.clear();
        std::map<int, KDungeonUserInfo>::iterator mit;
        for (mit = kPacket_.m_mapDifficulty.begin(); mit != kPacket_.m_mapDifficulty.end(); ++mit) {
            m_mapOldDifficulty[static_cast<char>(mit->first)] = mit->second.m_kModeDifficulty; // �ʱⰪ ����.
        }
    }

    // db���� ���� �������� Ŭ���� ���� �� �� �ʿ��� ����� ������ ����ִٸ� ä���־��ش�.
    for (int i = 0; i < NUM_GC_GAME_MODE; ++i) {
        const int& nModeID = i;

        if (false == KRoom::IsDifficultyMode(nModeID)) {
            continue;
        }

        if (kPacket_.m_mapDifficulty.find(nModeID) == kPacket_.m_mapDifficulty.end()) {
            KDungeonUserInfo kInfo;
            kPacket_.m_mapDifficulty.insert(std::make_pair(nModeID, kInfo));
        }

        // ���� �ִ� ���̵� Ŭ���� ���� (���� ����)
        int nLastDifficulty = SiKResultManager()->GetLastDifficulty(nModeID);
        if (nLastDifficulty > -1) {
            if (kPacket_.m_mapDifficulty[nModeID].m_kModeDifficulty.Get(nLastDifficulty)) {
                kPacket_.m_mapDifficulty[nModeID].m_kClearData.m_bClear = true;
            }
        }

        KHeroDungeon kInfo;
        bool bHeroDungeon = SiKHeroDungeonManager()->GetHeroDungeonInfo(nModeID, kInfo); // ���������̸� ���� ȹ��

        // �Ϲݴ����̰ų� ���̵� ���������ʴ� ������ ���.
        if (!bHeroDungeon || (bHeroDungeon && !kInfo.m_bResetDifficulty)) {
            // �÷��̰����� ���̵� Ŭ���� ����
            for (int i = GC_GMD_NORMAL_BEGIN; i < nLastDifficulty; ++i) {
                if (false == kPacket_.m_mapDifficulty[nModeID].m_kModeDifficulty.Get(i)) {
                    break;
                }

                kPacket_.m_mapDifficulty[nModeID].m_kClearData.m_shMaxDifficulty = i;
            }
        }
    }

    UpdateCharDifficultInfo(kPacket_.m_cCharacter, kPacket_.m_mapDifficulty);

    // LastPlayĳ���� ���� ������ ������ �����Ƿ� ĳ���� ���� �� �����ϵ��� ����
    CheckCharacterGraduate(KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_CONNECT);

    // ���� �������� �������� ����
    m_kConnectionGiftBoxUserData.SetData(static_cast<int>(kPacket_.m_cCharacter), kPacket_.m_kConnectionGiftBoxUpdateData);
    SendConnectionGiftBoxInfoNot(KConnectionGiftboxInfoNot::EST_ON_CONNECT);

    LIF(GetEquipItem(kPacket_.m_cCharacter, kPacket_.m_vecEquipItems));
    LIF(GetLookEquipItem(kPacket_.m_cCharacter, kPacket_.m_vecLookEquips));

    SendInventoryInfo(kPacket_.m_vecInv);
    kPacket_.m_vecInv.clear(); // ������ ������ �����ش��� ������ ���� clear��Ű��.
    SEND_COMPRESS_PACKET(EVENT_CHANGE_CHARACTER_INFO_ACK, kPacket_);
    SendUserGCPointInfo(); // GC ����Ʈ ���� ��������.

    // �������� Ŭ���� ���� �޾ƿ���
    QUEUEING_ID_TO_DB(EVENT_USER_HERO_DUNGEON_INFO_REQ);

    // ���� �������� ĳ������ ������ ������ �ִ��� üũ.
    std::map< char, KQuickSlot >::iterator mit;
    mit = m_mapQuickSlot.find(kPacket_.m_cCharacter);
    if (mit == m_mapQuickSlot.end()) {
        QUEUEING_ID_TO_DB(EVENT_SLOT_INFO_NOT); // ������ ���� ��������.
    }
    else {
        KQuickSlot kQuickSlot;
        kQuickSlot = mit->second;
        SEND_DATA(EVENT_SLOT_INFO_NOT, kQuickSlot);
    }

    CheckBonusRefill(); // ���ʽ� ������ ����
}

_IMPL_ON_FUNC( EVENT_CYOU_LOGIN_REQ, PAIR_CHAR_CHAR )
{
}

_IMPL_ON_FUNC( EVENT_CYOU_LOGIN_ACK, int )
{
}

IMPL_ON_FUNC( EVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ )
{
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );

    JIF( m_pkRoom );

    if ( true == kPacket_.m_vecSummonMonsters.empty() ) {
        return;
    }

    KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_ACK kPacketAck = 0;

    KGameInfo kGameInfo;
    m_pkRoom->GetGameInfo( kGameInfo );

    if ( false == SiKInfinityDungeon()->IsInfinityDungeon( kGameInfo.m_iGameMode ) ) {
        START_LOG( cerr, L"무한던전이 아닌데 무한던전 몬스터 소환 요청. ModeID: " << kGameInfo.m_iGameMode ) << END_LOG;

        kPacketAck = 1;
        SEND_DATA( EVENT_INFINITY_DUNGEON_MONSTER_SUMMON_ACK, kPacketAck );

        return;
    }

    // 무한던전 몬스터 소환 해킹 체크(소환 주기가 너무 빠른지, 한번에 너무 많이 소환하는지 확인)
    DWORD dwSummonGap = ::GetTickCount() - m_pkRoom->GetIDMonsterLastSummonTime();
    if ( KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_NORMAL == kPacket_.m_nSummonType ) { // 일반몹 소환에 대해서만 해킹체크를 한다
        if ( true == IsInfinityDungeonMonsterHack( dwSummonGap, kPacket_.m_vecSummonMonsters.size() ) ) {
            kPacketAck = 2;
            SEND_DATA( EVENT_INFINITY_DUNGEON_MONSTER_SUMMON_ACK, kPacketAck );

            return;
        }
    }

    // 룸에 무한던전 몬스터 마지막 소환시간을 기록
    m_pkRoom->SetIDMonsterLastSummonTime( ::GetTickCount() );

    // 몬스터UID 정보 생성
    std::list< KIDMonsterInfo > lstIDSummonedMonsters;
    DWORD dwIDSummonedMonsterLastUID = m_pkRoom->GetIDSummonedMonsterLastUID();
    DWORD dwIDSummonedMonsterLastUIDAfter = 0;
    MakeIDSummonedMonsterInfo( kPacket_, dwIDSummonedMonsterLastUID, lstIDSummonedMonsters, dwIDSummonedMonsterLastUIDAfter );

    // 아이템 드랍UID 정보 생성
    std::vector< KInfinityDungeon::KIDRewardInfo > vecIDRewardInfo;
    SiKInfinityDungeon()->GetIDRewardInfo( kGameInfo.m_iGameMode, vecIDRewardInfo );

    std::vector< DWORD > vecUserUIDs;
    m_pkRoom->GetUserUIDList( vecUserUIDs );
    std::list< KGameDropElement > lstIDReservedDrops;
    lstIDReservedDrops.clear();
    DWORD dwIDReservedDropLastUID = m_pkRoom->GetIDReservedDropLastUID();
    DWORD dwIDReservedDropLastUIDAfter = dwIDReservedDropLastUID;

    float fDropEventScale = GetDropEventScale( kGameInfo );
    SiKGameDrop()->MakeIDReservedDropInfoFromGameDrop( kGameInfo, fDropEventScale, lstIDSummonedMonsters, dwIDReservedDropLastUID, lstIDReservedDrops, dwIDReservedDropLastUIDAfter );
    MakeIDReservedDropInfo( lstIDSummonedMonsters, vecIDRewardInfo, vecUserUIDs.size(), dwIDReservedDropLastUIDAfter, lstIDReservedDrops, dwIDReservedDropLastUIDAfter );

    /* UserUID 바인딩 처리
       받을 유저가 지정되어 있는 아이템들은 RewardType을 RT_CHARDROP_ITEM으로 설정한다
       지정되지 않은 아이템들은 방원의 UserUID를 골고루 넣어준다 */
    BindIDReservedDropToUsers( lstIDReservedDrops, vecUserUIDs );

    // GP 드랍UID 정보 생성(아이템 드랍UID와 공용으로 사용함)
    std::list< KGameDropElement > lstIDReservedGPDrops;
    MakeIDReservedGPDropInfo( lstIDSummonedMonsters, dwIDReservedDropLastUIDAfter, lstIDReservedGPDrops, dwIDReservedDropLastUIDAfter );

    // 룸에 저장되어 있는 소환된 몬스터 리스트에 새로 소환된 몬스터들 합침
    std::list< KIDMonsterInfo > lstIDSummonedMonstersAfter;
    m_pkRoom->GetIDSummonedMonsters( lstIDSummonedMonstersAfter );
    AddIDSummonedMonsters( lstIDSummonedMonsters, lstIDSummonedMonstersAfter );

    // 룸에 저장되어 있는 드랍 예정 아이템 리스트에 새로 드랍될 아이템들 합침
    std::list< KGameDropElement > lstIDReservedDropsAfter;
    m_pkRoom->GetIDReservedDrops( lstIDReservedDropsAfter );
    AddIDReservedDrops( lstIDReservedDrops, lstIDReservedDropsAfter );
    
    // 룸에 저장되어 있는 드랍 예정 GP 리스트에 새로 드랍될 GP들 합침
    std::list< KGameDropElement > lstIDReservedGPDropsAfter;
    m_pkRoom->GetIDReservedGPDrops( lstIDReservedGPDropsAfter );
    AddIDReservedGPDrops( lstIDReservedGPDrops, lstIDReservedGPDropsAfter );

    // 그 다음에 룸에 저장
    m_pkRoom->SetIDSummonedMonsterLastUID( dwIDSummonedMonsterLastUIDAfter );
    m_pkRoom->SetIDReservedDropLastUID( dwIDReservedDropLastUIDAfter );
    m_pkRoom->SetIDSummonedMonsters( lstIDSummonedMonstersAfter );
    m_pkRoom->SetIDReservedDrops( lstIDReservedDropsAfter );
    m_pkRoom->SetIDReservedGPDrops( lstIDReservedGPDropsAfter );

    // 모든 방원들에게 몬스터 소환 알림
    KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_BROAD kPacket;
    SetInfinityDungeonMonsterSummonBroadPacket( lstIDSummonedMonsters, lstIDReservedDrops, lstIDReservedGPDrops, kPacket );
    kPacket.m_nCurrentTopRank = kPacket_.m_nCurrentTopRank;

    m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_INFINITY_DUNGEON_MONSTER_SUMMON_BROAD, kPacket, true );

    START_LOG( clog, L"무한던전 몬스터 소환 처리 완료." )
        << BUILD_LOG( kPacket_.m_nCurrentTopRank )
        << BUILD_LOG( kPacket_.m_dwRoundSequence )
        << BUILD_LOG( kPacket_.m_nSummonType )
        << BUILD_LOG( kPacket_.m_vecSummonMonsters.size() )
        << BUILD_LOG( kPacket.m_vecSummonedMonsters.size() )
        << BUILD_LOG( kPacket.m_vecMonsterDrop.size() )
        << BUILD_LOG( kPacket.m_vecGpDrop.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_EXP_REQ )
{
    // kPacket_: 잡은 몬스터UID
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );

    JIF( m_pkRoom );

    KGameInfo kGameInfo;
    m_pkRoom->GetGameInfo( kGameInfo );

    if ( false == SiKInfinityDungeon()->IsInfinityDungeon( kGameInfo.m_iGameMode ) ) {
        START_LOG( cerr, L"무한던전이 아닌데 무한던전 경험치 처리 요청. ModeID: " << kGameInfo.m_iGameMode ) << END_LOG;

        return;
    }

    std::vector< DWORD > vecUserUIDs;
    m_pkRoom->GetUserUIDList( vecUserUIDs );

    MAP_CHAR_JOB mapPromotionInfo;
    m_pkRoom->GetPromotionInfo( mapPromotionInfo );

    KEVENT_INFINITY_DUNGEON_REWARD_EXP_BROAD kPacket;
    GetCharExpReward( vecUserUIDs, mapPromotionInfo, kPacket );

    // 룸유저 누적 Exp값. [UserUID,CharType], Exp
    std::map< std::pair< DWORD, char >, __int64 > mapResultExp;
    m_pkRoom->GetResultExp( mapResultExp );

    // 유저의 레벨업 정보 [UserUID, <CharType, Level>]
    std::map< DWORD, std::pair< int, DWORD > > mapLevelUpUser;
    mapLevelUpUser.clear();
    m_pkRoom->GetLevelUpUser( mapLevelUpUser );

    // 잡은 몬스터가 소환된 몬스터인지 확인하고
    // 잡은 몬스터는 한번만 처리되도록 소환된 몬스터 리스트에서 제거함
    KIDMonsterInfo kIDMonsterInfo;
    std::list< KIDMonsterInfo > lstIDSummonedMonsters;
    m_pkRoom->GetIDSummonedMonsters( lstIDSummonedMonsters );
    if ( false == DoKillInfinityDungeonMonster( kPacket_, lstIDSummonedMonsters, kIDMonsterInfo ) ) {
        START_LOG( cerr, L"무한던전에서 소환되지 않은 몬스터를 잡으려고 시도함. ModeID: " << kGameInfo.m_iGameMode ) << END_LOG;

        return;
    }
    m_pkRoom->SetIDSummonedMonsters( lstIDSummonedMonsters );

    float fMonExp = static_cast< float >( SiKGameDrop()->GetMonExp( kIDMonsterInfo.m_nMonID, kIDMonsterInfo.m_nMonLv ) );
    if ( 0.f > fMonExp ) {
        START_LOG( clog, L"무한던전의 해당 몬스터 경험치가 0 미만. MonsterUID : " << kPacket_ )
            << BUILD_LOG( kGameInfo.m_iGameMode )
            << BUILD_LOG( kGameInfo.m_nDifficulty )
            << END_LOG;

        return;
    }

    std::vector< DWORD >::iterator vitUser;
    for ( vitUser = vecUserUIDs.begin() ; vitUser != vecUserUIDs.end() ; ++vitUser ) {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( *vitUser ) );
        if ( NULL == spUser ) {
            START_LOG( clog, L"서버에 유저 정보가 없음. UserUID: " << *vitUser ) << END_LOG;
            continue;
        }

        std::map< DWORD, std::vector< std::pair< char, char > > >::const_iterator cmitChar;
        cmitChar = mapPromotionInfo.find( *vitUser );
        if ( mapPromotionInfo.end() == cmitChar ) {
            START_LOG( clog, L"룸에 유저 정보가 없음. LoginID: " << spUser->GetName() ) << END_LOG;
            continue;
        }

        if ( true == cmitChar->second.empty() ) {
            START_LOG( clog, L"룸에 캐릭터 정보가 없음. LoginID: " << spUser->GetName() ) << END_LOG;
            continue;
        }

        char cCharType = cmitChar->second.begin()->first;
        KCharacterInfo* pkCharInfo = spUser->GetCharacterInfo( cCharType );
        if ( NULL == pkCharInfo ) {
            START_LOG( clog, L"유저 캐릭터 정보가 없음. LoginID : " << spUser->GetName() ) << END_LOG;
            continue;
        }

        DWORD dwOldLevel = pkCharInfo->m_dwLevel;
        float fCurrentMonExp = fMonExp;

        // 레벨차이에 따른 경험치 보정
        SiKResultManager()->GetApplyLvDiffRatio( pkCharInfo->m_dwLevel, kIDMonsterInfo.m_nMonLv, fCurrentMonExp );

        /* 과몰입과 활력 시스템에 대한 기획이 정해지면 아래 부분을 수정해야 한다
        // 중국. 2012903. woosh.
        // 게임 시작시 저장된 누적온라인 시간을 구해서 과몰입 시스템에 있는 적용 결과에 따라 경험치 조정.
        if ( true == SiKFatigueSystem()->IsRun() && false == spUser->IsAdult() ) {
            int nRatio = m_pkRoom->GetFatigueRatio( spUser->GetUID() );
            fCurrentMonExp = fCurrentMonExp * nRatio / 100.f;
        }

        // 활력이 0인 상태로 활력을 소모하는 던전을 플레이한 경우에 획득 경험치를 조정함
        int nRemainVitality = 0;
        if ( true == m_pkRoom->GetRemainVitalityOnStartGame( spUser->GetUID(), static_cast< int >( spUser->GetCurrentCharType() ), nRemainVitality ) ) {
            if ( nRemainVitality < nDeductVitality ) {
                int nExpGainRatioNoVitality = SiKVitalityManager()->GetExpGainRatioNoVitality( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty );
                fCurrentMonExp = fCurrentMonExp * nExpGainRatioNoVitality / 100.f;
            }
        }
        */

        fCurrentMonExp = std::max< float >( 0.f, fCurrentMonExp );

        // 무한던전 Exp 보너스 배율
        float fInfinityDungeonExpRewardRatio = std::max< float >( 0.f, SiKInfinityDungeon()->GetExpRewardRatio( kGameInfo.m_iGameMode ) );
        fCurrentMonExp *= fInfinityDungeonExpRewardRatio;

        pkCharInfo->m_biExp += static_cast< __int64 >( fCurrentMonExp );
        pkCharInfo->m_biTotalExp += static_cast< __int64 >( fCurrentMonExp );
        mapResultExp[ std::make_pair( *vitUser, cCharType ) ] += static_cast< __int64 >( fCurrentMonExp );

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
            START_LOG( clog , L"Exp Reward: " << fCurrentMonExp
                << L" ,IDExpRewardRatio: " << fInfinityDungeonExpRewardRatio
                << L" ,ModeID: " << kGameInfo.m_iGameMode
                << L" ,Difficulty: " << kGameInfo.m_nDifficulty
                << L" ,MonsterUID: " << kPacket_
                << L" ,MonsterID: " << kIDMonsterInfo.m_nMonID
                << L" ,MonsterLv: " << kIDMonsterInfo.m_nMonLv ) << END_LOG;
        }

        // 캐릭터에 적용.
        // EXP
        __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );
        pkCharInfo->m_biExp = std::min< __int64 >( pkCharInfo->m_biExp, biMaxExp );
        pkCharInfo->m_biExp = std::max< __int64 >( pkCharInfo->m_biExp, USER_MIN_EXP );
        // TotalExp( 누적 경험치 )
        pkCharInfo->m_biTotalExp = std::min< __int64 >( pkCharInfo->m_biTotalExp, _I64_MAX );
        pkCharInfo->m_biTotalExp = std::max< __int64 >( pkCharInfo->m_biTotalExp, USER_MIN_EXP );
        // 레벨 계산.
        pkCharInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

        // 캐릭터의 Levelup을 달성했을 경우에 체크하자.
        if ( dwOldLevel < pkCharInfo->m_dwLevel ) {
            // SPX 계산
            SiKSkillManager()->GetSkillPointInfo( cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );
            // 상하이 드랍 체크
            LevelUpDrop( spUser, cCharType, static_cast< char >( dwOldLevel ), static_cast< char >( pkCharInfo->m_dwLevel ) );
            // 유저 레벨업 정보 저장.
            SetUserLevelUpInfo( spUser->GetUID(), static_cast< int >( cCharType ), pkCharInfo->m_dwLevel, mapLevelUpUser );
            // 통계용
            spUser->UpdateCharLvPlayTime( static_cast< int >( cCharType ), dwOldLevel );

            if ( SiKGCHelper()->IsCharLevelEvent() ) {
                KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;
                if ( SiKGCHelper()->GetCharLevelRewardItem( pkCharInfo->m_cCharType, pkCharInfo->m_dwLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                    kDBPacket.m_setCharInfo.insert( pkCharInfo->m_cCharType );
                    _QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket, spUser );
                }
            }
        }

        // 클라이언트 전달용 패킷
        KCharExpReward kExpInfo;
        kExpInfo.m_cCharType = cCharType;
        kExpInfo.m_biExp = pkCharInfo->m_biExp;
        kExpInfo.m_biTotalExp = pkCharInfo->m_biTotalExp;
        kExpInfo.m_dwOldLevel = dwOldLevel;
        kExpInfo.m_dwLevel = pkCharInfo->m_dwLevel;
        kExpInfo.m_fRewardExp = fCurrentMonExp;
        kExpInfo.m_kSkillInfo = pkCharInfo->m_kSkillInfo;
        kPacket[ *vitUser ] = kExpInfo;
    }

    // Exp 처리결과 룸에 갱신
    m_pkRoom->SetResultExp( mapResultExp );

    // 유저의 레벨업 정보 갱신.
    m_pkRoom->SetLevelUpUser( mapLevelUpUser );

    // 모든 방원들에게 경험치 정보 알림
    m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_INFINITY_DUNGEON_REWARD_EXP_BROAD, kPacket, true );

    // 몬스터 킬 후 룸에 남은 몬스터 수 출력
    m_pkRoom->GetIDSummonedMonsters( lstIDSummonedMonsters );

    START_LOG( clog, L"무한던전 경험치 보상 처리 완료" )
        << BUILD_LOG( kPacket.size() )
        << BUILD_LOG( lstIDSummonedMonsters.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ )
{
    /* kPacket_.first: 드랍UID
       kPacket_.second: 유저UID */
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );

    JIF( m_pkRoom );

    KDB_EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ kDBPacket;
    KEVENT_INFINITY_DUNGEON_REWARD_ITEM_ACK kPacket;
    std::list< KGameDropElement > lstIDReservedDrops;
    std::vector< DWORD > vecUserUIDs;

    SET_ERROR( ERR_UNKNOWN );

    // 획득 요청한 아이템이 드랍 예정인 아이템인지 확인하고
    // 드랍한 아이템은 한번만 처리되도록 드랍 예정 아이템 리스트에서 제거함
    m_pkRoom->GetIDReservedDrops( lstIDReservedDrops );
    if ( false == DoGetInfinityDungeonItem( kPacket_.first, lstIDReservedDrops, kDBPacket.m_kDropItem, kDBPacket.m_dwRecvUserUID ) ) {
        SET_ERR_GOTO( ERR_INFINITY_DUNGEON_01, END_PROC );
    }
    m_pkRoom->SetIDReservedDrops( lstIDReservedDrops );

    // 누가 아이템을 받을지 결정한다
    m_pkRoom->GetUserUIDList( vecUserUIDs );
    if ( false == DecideWhoGetDropItem( kDBPacket.m_kDropItem.m_ItemID, vecUserUIDs, kDBPacket.m_dwRecvUserUID, kDBPacket.m_cRecvUserCharType, kDBPacket.m_mapUserDice ) ) {
        SET_ERR_GOTO( ERR_INFINITY_DUNGEON_02, END_PROC );
    }

    /* 짧은 시간에 동일한 내용의 패킷이 여러번 오더라도
       요청한 아이템은 첫 요청시 룸의 드랍 예정 아이템 리스트에서 제거되어 중복처리를 하지 않으므로
       DB큐잉시 QUEUEING_EVENT_TO_DB_CHECK 매크로를 사용하지 않는다
       현재 이 부분에 CHECK 매크로를 사용하는 경우 연속 아이템 획득시 처리되지 않는 경우가 발생할 수 있다
       패킷을 짧은 시간에 연속적으로 보내지 않고 묶어서 처리하는 구조로 변경하면 좋겠다 */
    QUEUEING_EVENT_TO_DB( DB_EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                  kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_INFINITY_DUNGEON_01, kPacket.m_nOK, NetError::ERR_INFINITY_DUNGEON_01 ); // 획득 요청한 아이템이 드랍 예정 아이템 리스트에 없음.
        ERR_CASE( ERR_INFINITY_DUNGEON_02, kPacket.m_nOK, NetError::ERR_INFINITY_DUNGEON_02 ); // 아이템을 획득할 유저를 결정할 수 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET,    kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L"무한던전 아이템 획득 처리 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    // 드랍 후 룸에 남은 드랍 예정 아이템 수 출력
    m_pkRoom->GetIDReservedDrops( lstIDReservedDrops );

    _LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK || NetError::ERR_INFINITY_DUNGEON_02 == kPacket.m_nOK, L"무한던전 아이템 획득 요청 처리 Msg:" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.first ) // 드랍UID
        << BUILD_LOG( kPacket_.second ) // 유저UID
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( lstIDReservedDrops.size() )
        << END_LOG;

    if ( NetError::NET_OK != kPacket.m_nOK ) {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_INFINITY_DUNGEON_REWARD_ITEM_BROAD, kPacket, true );
    }
}

IMPL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_ITEM_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 아이템 획득 결과 해당 유저의 인벤토리에 추가
    if ( false == kPacket_.m_vecDropItem.empty() ) {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( kPacket_.m_dwRecvUserUID ) );
        if ( NULL == spUser ) {
            START_LOG( cerr, L"보상을 받을 유저가 서버에 접속중이 아닙니다. UserUID : " << kPacket_.m_dwRecvUserUID ) << END_LOG;

            goto END_PROC;
        }

        spUser->m_kInventory.AddItemList( kPacket_.m_vecDropItem );
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                  kPacket_.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_INFINITY_DUNGEON_03, kPacket_.m_nOK, NetError::ERR_INFINITY_DUNGEON_03 ); // 무한던전 아이템 DB지급 실패.

        ERR_CASE( ERR_NOT_FINISHED_YET,    kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L"무한던전 아이템 획득 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( NetError::NET_OK == kPacket_.m_nOK, L"무한던전 아이템 획득 응답 처리 Msg:" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwRecvUserUID )
        << BUILD_LOG( kPacket_.m_vecDropItem.size() )
        << BUILD_LOG( kPacket_.m_mapUserDice.size() )
        << END_LOG;

    if ( m_pkRoom ) {
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_INFINITY_DUNGEON_REWARD_ITEM_BROAD, kPacket_, true );
    }
}

IMPL_ON_FUNC( EVENT_INFINITY_DUNGEON_REWARD_GP_NOT )
{
    // kPacket_: 획득한 GP의 UID리스트
    _VERIFY_STATE( ( 1, KUserFSM::STATE_PLAYING ) );

    JIF( m_pkRoom );

    KGameInfo kGameInfo;
    m_pkRoom->GetGameInfo( kGameInfo );

    if ( false == SiKInfinityDungeon()->IsInfinityDungeon( kGameInfo.m_iGameMode ) ) {
        START_LOG( cerr, L"무한던전이 아닌데 무한던전 GP 획득 처리 요청. ModeID: " << kGameInfo.m_iGameMode ) << END_LOG;

        return;
    }

    std::list< KGameDropElement > lstIDReservedGPDrops;
    m_pkRoom->GetIDReservedGPDrops( lstIDReservedGPDrops );

    // 획득할 GP 계산
    float fGPSum = 0.f;
    std::set< DWORD >::iterator sit;
    for ( sit = kPacket_.begin() ; sit != kPacket_.end() ; ++sit ) {
        std::list< KGameDropElement >::iterator vitGP;
        vitGP = std::find_if( lstIDReservedGPDrops.begin(), lstIDReservedGPDrops.end(),
            boost::bind( &KGameDropElement::m_dwUID, _1 ) == *sit );
        if ( vitGP == lstIDReservedGPDrops.end() ) {
            START_LOG( clog, L"무한던전 GP 드랍 예정 리스트에 없는 UID : " << *sit ) << END_LOG;
            continue;
        }

        // 획득GP = 몬스터 레벨에 따른 보상GP * 먹은 동전 개수 * 동전등급에 따른 지급비율(GP 드랍UID 생성시 미리 계산해둔 정보를 이용한다)
        float fMonGP = static_cast< float >( vitGP->m_nCount );
        float fCurrentMonGP = fMonGP * vitGP->m_fRatio;

        fCurrentMonGP = std::max< float >( 0.f, fCurrentMonGP );
        fGPSum += fCurrentMonGP;

        lstIDReservedGPDrops.erase( vitGP );
    }

    m_pkRoom->SetIDReservedGPDrops( lstIDReservedGPDrops );

    std::map< DWORD, int > mapResultGP;
    m_pkRoom->GetResultGP( mapResultGP ); // 룸에 누적하고 있는 GP 획득량(UserUID, GP)

    // 파티원 수로 나눈 비율로 GP 지급
    float fPartyRatio = static_cast< float >( 1.f / std::max< float >( 1.f, static_cast< float >( m_pkRoom->GetStartUserNum() ) ) );
    fGPSum *= fPartyRatio;

    // 무한던전 GP 보너스 배율
    float fInfinityDungeonGPRewardRatio = std::max< float >( 0.f, SiKInfinityDungeon()->GetGPRewardRatio( kGameInfo.m_iGameMode ) );
    fGPSum *= fInfinityDungeonGPRewardRatio;

    // 유저별로 획득한 GP 적용
    std::vector< DWORD > vecUserUIDs;
    m_pkRoom->GetUserUIDList( vecUserUIDs );
    std::vector< DWORD >::iterator vitUser;
    for ( vitUser = vecUserUIDs.begin() ; vitUser != vecUserUIDs.end() ; ++vitUser ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( *vitUser );
        if ( NULL == spUser ) {
            continue;
        }

        int nCurrentGP = static_cast< int >( fGPSum );
        spUser->IncreaseGP( KGpStatInfo::GIT_INFINITY_DUNGEON_REWARD_GP, false, nCurrentGP );
        mapResultGP[ *vitUser ] += nCurrentGP;

        START_LOG( clog, L"유저 획득 GP 적용. UserUID : " << *vitUser )
            << BUILD_LOG( spUser->GetGP() )
            << BUILD_LOG( nCurrentGP )
            << BUILD_LOG( fGPSum )
            << BUILD_LOG( fPartyRatio )
            << BUILD_LOG( m_pkRoom->GetStartUserNum() )
            << BUILD_LOG( fInfinityDungeonGPRewardRatio )
            << END_LOG;
    }

    // 총 획득한 GP량 갱신
    m_pkRoom->SetResultGP( mapResultGP );
}
