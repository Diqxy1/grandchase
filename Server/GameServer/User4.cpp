#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "GSSimLayer.h"
#include "Guild.h"
#include "GSDBThread.h"
#include "Channel.h"
#include "ChannelManager.h"
#include "GameServer.h"
#include "FailRate.h"
#include "MissionManager.h"
#include "SHCheckPoint.h"
#include "LogManager.h"
#include "ResultManager.h"
#include "SquareManager.h"
#include "StatisticsManager.h"
#include "StatItemGet.h"
#include "StatDeathMatch.h"
#include "support_User.h"
#include "CnConnector.h"
#include "CollectionManager.h"
#include "LoudMessage.h"
#include "StatP2PCritical.h"
#include "GCHelper.h"
#include "PreHackingCheckManager.h"
#include "IPAdvantage.h"
#include "CharDefault.h"
#include "SkillManager.h"
#include "Worker.h"
#include <boost/lexical_cast.hpp>
#include "LevelResetEvent.h"
#include "RecomHelper.h"
#include "SlangFilter.h"
#include "HeroDungeonManager.h"
#include "StatNetwork.h"
#include "MissionGettableCondition.h"

#define CLASS_TYPE  KUser

_IMPL_ON_FUNC( DB_EVENT_INSERT_DROP_ITEM_ACK, KEndGameAck )
{
    int nGameMode = -99;
    int nDifficulty = -99;
    if ( m_pkRoom ) {
        nGameMode = m_pkRoom->m_iGameMode;
        nDifficulty = m_pkRoom->m_nDifficulty;
    }

    std::vector<KGameResultOut>::iterator vit;
    for ( vit = kPacket_.m_vecGameResult.begin() ; vit != kPacket_.m_vecGameResult.end() ; ++vit ) {
        // Insert Inventory 하기 전에 처리해야 함.
        KStatItemGetPtr spStat = GET_STAT( KStatItemGet, SI_STAT_ITEM_GET );
        if ( spStat ) {
            spStat->AddStat( nGameMode, vit->m_vecInv );
        }

        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( !spUser ) {
            START_LOG( cwarn, L"게임 종료 처리중 해당 유저가 게임서버에 존재하지 않음.. Name : " << vit->m_strLogin << L", UID : " << vit->m_dwUID ) << END_LOG;
            continue;
        }
        spUser->m_kInventory.AddItemList( vit->m_vecInv );

        std::vector<KItem>::iterator vitInv;
        // vit->m_vecInv  드랍받는 아이템중에 칭호 아이템이 있으면  전체 채팅 메세지로 보내기
        for ( vitInv = vit->m_vecInv.begin(); vitInv != vit->m_vecInv.end() ; ++vitInv ) {   // 칭호 아이템은 기간제라고 가정하고 있음
            if ( SiKCollectionManager()->IsTitleItem( vitInv->m_ItemID ) ) { // 칭호 중의 하나이고, 메세지 전송 설정이면.
                if ( SiKCollectionManager()->GetBroadcastCheck() == false ) {
                    continue;
                }

                // 닉네임  님께서   '던전이름' 의 '칭호 이름' 칭호를 획득하셨습니다. ['획득 방법']
                std::wstring strMsg;
                std::wstring strDungeon;// = L"어떤던전"; KAIRO , 펫 획득 메세지처럼 DB에 있음
                std::wstring strMethod;// = L"던전클리어보상"; config_server.ini에 설정

                strMethod = SiKCollectionManager()->HowGetTitle( L"str_01" ); // 어떻게 얻은 칭호인지, 이닛드랍
                SiKLoudMessage()->GetItemName( vitInv->m_ItemID, 1, strDungeon, GetLanguageCode() );
                if ( SiKLoudMessage()->GetLoudMsg( vitInv->m_ItemID, 0,  spUser->m_strNickName, strMsg, strDungeon, strMethod, GetLanguageCode() ) ) {
                    if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) ) {
                        std::wstring strLanguageCode = boost::lexical_cast<std::wstring>( GetLanguageCode() );
                        strMsg = strLanguageCode + L":" + strMsg;
                    }
                    ChatNotice( strMsg );
                    START_LOG( clog, L"Collection get msg : " << strMsg ) << END_LOG;
                }
            }
        }
    }
    
    if ( m_pkRoom ) {
        m_pkRoom->RollbackMigrationFromPlaying( kPacket_.m_dwHostUID );
    }

    if ( SiKGSSimLayer()->CheckTypeFlag( ST_CREATE_LIMIT ) ) { // 대회서버는 옵저버에게 종료 패킷을 보내야 함.
        if ( m_pkRoom ) {
            std::vector<DWORD> vecRoomUserUIDs;
            m_pkRoom->GetTotalUserUIDList( vecRoomUserUIDs ); // 대회서버의 경우 옵저버 포함 유저 전체 정보를 가져와야 한다.

            for ( std::vector<DWORD>::iterator vitUserUID = vecRoomUserUIDs.begin() ; vitUserUID != vecRoomUserUIDs.end() ; ++vitUserUID ) {
                // 해당 유저를 찾을 수 없음.
                KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vitUserUID ) );
                if( !spUser ) continue;

                spUser->QueueingEvent( KUserEvent::EVENT_END_GAME_BROAD, kPacket_ );
            }

            //m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_END_GAME_BROAD, kPacket_, true );
        }
    }
    else {
        std::vector<KGameResultOut>::iterator vitBroad;
        for ( vitBroad = kPacket_.m_vecGameResult.begin() ; vitBroad != kPacket_.m_vecGameResult.end() ; ++vitBroad ) {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vitBroad->m_strLogin );
            if ( !spUser ) {
                START_LOG( cwarn, L"게임 종료 처리중 해당 유저가 게임서버에 존재하지 않음.. Name : " << vitBroad->m_strLogin ) << END_LOG;
                continue;
            }
            spUser->QueueingEvent( KUserEvent::EVENT_END_GAME_BROAD, kPacket_ );

            spUser->CheckCharacterGraduate( KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_GRADUATE );
        }
    }

    // 최초 클리어 던전의 경우 Prev값 갱신 (컨트롤 피드백 시스템)
    // 패킷 보낼 때는 Prev값이 0이어야 하고 다음 클리어에는 지금 최고 기록이 Prev여야 하기 때문에 추가 함
    for ( vit = kPacket_.m_vecGameResult.begin() ; vit != kPacket_.m_vecGameResult.end() ; ++vit ) {
        if ( true == vit->m_bWin ) {
            // 해당 유저를 찾을 수 없음.
            KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID ) );
            if( !spUser ) continue;
            spUser->ProcessPrevRecordDataWhenFirstClear( *spUser, nGameMode, nDifficulty );
        }
    }
}

//-----------------------------------------------------------------------------
//050530. microcat
_IMPL_ON_FUNC( EVENT_QUERY_NORMAL_ITEM_LIST_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_QUERY_NORMAL_ITEM_LIST_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_QUERY_NORMAL_ITEM_LIST_ACK, KItemInv )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_QUERY_NORMAL_ITEM_LIST_ACK );
}

_IMPL_ON_FUNC( EVENT_QUERY_DURATION_ITEM_LIST_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_QUERY_DURATION_ITEM_LIST_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_QUERY_DURATION_ITEM_LIST_ACK, KDurationInv )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_QUERY_DURATION_ITEM_LIST_ACK );
}

_IMPL_ON_FUNC( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_ACK, KItemInv )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_ACK );
}

_IMPL_ON_FUNC( EVENT_CHAT_REQ, KChatData )
{
    if ( kPacket_.m_cChatType == KChatData::MSG_GUILD || kPacket_.m_cChatType == KChatData::MSG_ADMIN_COMMON ) {
        VERIFY_STATE(( 6, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE, KUserFSM::STATE_PLAYING, KUserFSM::STATE_AGIT ));
    }
    else {
        VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE ));
    }

    kPacket_.m_strSenderNick = m_strNickName;
    kPacket_.m_strSenderNickColor = m_strNickColor;
    kPacket_.m_dwSenderUID = GetUID();
    int nTmpRoomID = ( m_pkRoom != NULL ? static_cast<int>( m_pkRoom->GetRoomID() ) : -1 );
    std::wstring strTmpChannelName = ( m_pkChannel != NULL ? m_pkChannel->GetChannelName() : L"" );

    SET_ERROR( ERR_UNKNOWN );

    if( !CheckChatEnable() ) { // 채팅 블럭 상태.
        SET_ERR_GOTO( ERR_CHAT_13, END_PROC );
    }

    // 채팅 간격 체크(방송서버 3초,일반서버 1초)
    if( SiKGSSimLayer()->CheckTypeFlag( ST_BROADCASTING ) ) {
        if( ::GetTickCount() - GetTick( CHAT_TICK ) < 3000 ) {
            SET_ERR_GOTO( ERR_CHAT_03, END_PROC );
        }
    }
    else {
        if( ::GetTickCount() - GetTick( CHAT_TICK ) < 1000 ) {
            SET_ERR_GOTO( ERR_CHAT_03, END_PROC );
        }
    }

    // 유효한 채팅 타입인가?
    if ( kPacket_.m_cChatType != KChatData::MSG_COLOR_CHAT &&
        kPacket_.m_cChatType != KChatData::MSG_COMMON &&
        kPacket_.m_cChatType != KChatData::MSG_ADMIN_COMMON &&
        kPacket_.m_cChatType != KChatData::MSG_TEAM_CHAT &&
        kPacket_.m_cChatType != KChatData::MSG_TEAM_COLOR &&
        kPacket_.m_cChatType != KChatData::MSG_GUILD ) {
        SET_ERR_GOTO( ERR_CHAT_01, END_PROC );
    }

    if( kPacket_.m_cChatType == KChatData::MSG_GUILD ) {
        if ( m_kGuildUserInfo.m_dwGuildUID == 0 ) {
            SET_ERR_GOTO( ERR_CHAT_08, END_PROC );
        }

        if ( m_kGuildUserInfo.m_cMemberLevel <= KNGuildUserInfo::GL_JOINER ) {
            SET_ERR_GOTO( ERR_CHAT_08, END_PROC );
        }

        // 길드 채팅
        kPacket_.m_iParam = m_kGuildUserInfo.m_dwGuildUID;
        CenterChat( kPacket_ );
        SET_ERR_GOTO( NET_OK, END_PROC );
    }

    if( !IsAdmin() ) {
        if( GetStateID() == KUserFSM::STATE_PLAYING ) {
            SET_ERR_GOTO( ERR_CHAT_00, END_PROC );
        }

        // 운영자 채팅 체크
        if ( kPacket_.m_cChatType == KChatData::MSG_ADMIN_COMMON ) {
            SET_ERR_GOTO( ERR_CHAT_02, END_PROC );
        }

        // 컬러 채팅은 GC클럽 보유자나 PC방 유저만 가능.
        if( kPacket_.m_cChatType == KChatData::MSG_COLOR_CHAT ||
            kPacket_.m_cChatType == KChatData::MSG_TEAM_COLOR ) {
            if( IsPremiumUser() == false && m_cPCBangType == KIPAdvantage::PCBANG_NONE ) {
                SET_ERR_GOTO( ERR_CHAT_04, END_PROC );
            }
        }
    }

    if ( GetStateID() == KUserFSM::STATE_CHANNEL ) {
        if ( m_pkChannel == NULL ) {
            SET_ERR_GOTO( ERR_CHAT_05, END_PROC );
        }

        // ST_CREATE_LIMIT일때는 운영자만 채팅 가능
        if ( SiKGSSimLayer()->CheckTypeFlag(ST_CREATE_LIMIT) ) {
            if( !IsObserver() ) {
                SET_ERR_GOTO( ERR_CHAT_00, END_PROC );
            }
        }

        // 채널 채팅
        if ( m_pkChannel->Send_ChatMsg( kPacket_ ) ) {
            SET_ERR_GOTO( NET_OK, END_PROC );
        }
    }
    else if ( GetStateID() == KUserFSM::STATE_ROOM ) {
        if( m_pkRoom == NULL ) {
            SET_ERR_GOTO( ERR_CHAT_05, END_PROC );
        }

        // 팀채팅
        if( kPacket_.m_cChatType == KChatData::MSG_TEAM_CHAT ||
            kPacket_.m_cChatType == KChatData::MSG_TEAM_COLOR )
        {
            const KInDoorUserInfo* pkInDoorUserInfo = m_pkRoom->FindInDoorUserInfo( m_strName );
            if ( pkInDoorUserInfo == NULL ) {
                SET_ERR_GOTO( ERR_CHAT_07, END_PROC );
            }

            if( m_pkRoom->Send_TeamChatMsg( kPacket_, pkInDoorUserInfo->m_iTeam ) ) {
                SET_ERR_GOTO( NET_OK, END_PROC );
            }
        }

        // 룸채팅
        if( m_pkRoom->Send_ChatMsg( kPacket_ ) ) {
            SET_ERR_GOTO( NET_OK, END_PROC );
        }
    }

     // 여기 까지 온다면. 채팅 전송실패다.
    SET_ERROR( ERR_CHAT_06 );

END_PROC:
    // 채팅 성공시 처리..
    if ( IS_CORRECT( NET_OK ) ) {
        if( IsAdmin() ) {
            SiKLogManager()->Get( KLogManager::LT_ADMIN_NOTICE )
                << L"ID : " << m_strName
                << L" , " << kPacket_.m_strChatMsg
                << L", Time : " << KNC_TIME_STRING
                << GLOG::endl;
        }

        // 채팅기록 남기기(내부에서 옵션 체크)
        SetTick( CHAT_TICK );
        SendChatLogNot( strTmpChannelName, nTmpRoomID, kPacket_.m_strChatMsg );
    }

    _LOG_CONDITION( IS_CORRECT( NET_OK ), clog, cwarn, L"채팅 완료.. : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( strTmpChannelName )
        << BUILD_LOG( nTmpRoomID )
        << BUILD_LOGc( kPacket_.m_cChatType )
        << BUILD_LOG( kPacket_.m_strChatMsg )
        << BUILD_LOG( kPacket_.m_iParam )
        << BUILD_LOG( m_kGuildUserInfo.m_dwGuildUID )
        << BUILD_LOGc( m_kGuildUserInfo.m_cMemberLevel ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_NOTIFY_REQ, std::wstring )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( IsAdmin() ) << END_LOG;

    if( !IsAdmin() ) return;

    SiKLogManager()->Get( KLogManager::LT_ADMIN_NOTICE )
        << L"ID : " << m_strName
        << L", Notice : " << kPacket_
        << L", Time : " << KNC_TIME_STRING
        << GLOG::endl;

    // 공지 정보
    SiKChannelManager()->Send_NotMsg( kPacket_.c_str() );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CHANNEL_LIST_REQ )
{
    _VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
    SET_ERROR( ERR_UNKNOWN );


    KEVENT_CHANNEL_LIST_ACK kPacket;
    if( ::GetTickCount() - GetTick( SEND_CHANNEL_LIST ) < KChannelManager::ms_uiSendChannelListGap )
    {
        SET_ERR_GOTO( ERR_LIST_03, END_PROC );
    }

    kPacket.m_buffCompChannelList = SiKChannelManager()->GetChannelListData();
    SetTick( KUser::SEND_CHANNEL_LIST );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
    ERR_CASE( ERR_LIST_03, kPacket.m_nOK, 1 );
    default:
        START_LOG( cerr, L"채널 리스트 전송중 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_CHANNEL_LIST_ACK );

    LOG_CONDITION( NetError::GetLastNetError() == NetError::NET_OK, clog, cwarn )
        << BUILD_LOG( m_strName )
        << L" 오류 메세지 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_ROOM_LIST_REQ )
{
    _VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY ));

    JIF( m_pkChannel );
    SET_ERROR( ERR_UNKNOWN );
    KPageInfo kPacket;
    kPacket.m_buffCompList.Clear();

    // 내부에서 에러 셋팅 되어서 옴..
    if ( SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) {
        m_pkChannel->GetCorrectGuildRoomList( kPacket_.m_bWaitRoom, kPacket_.m_nType, kPacket_.m_nDifficult, kPacket_.m_nPage, kPacket, m_kGuildUserInfo.m_dwGuildUID );
    }
    else {
        m_pkChannel->GetCorrectRoomList( kPacket_.m_bWaitRoom, kPacket_.m_nType, kPacket_.m_nDifficult, kPacket_.m_nPage, kPacket );
    }
    SetTick( SEND_ROOM_LIST );

//END_PROC:
    switch( NetError::GetLastNetError() )
    {
    ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
    ERR_CASE( ERR_LIST_00, kPacket.m_nOK, 1 ); // 목록이 비어 있습니다.
    ERR_CASE( ERR_LIST_01, kPacket.m_nOK, 2 ); // 해당 카테고리 모드 목록이 없습니다.
    ERR_CASE( ERR_LIST_02, kPacket.m_nOK, 3 ); // 해당 페이지가 존재하지 않습니다.
    ERR_CASE( ERR_LIST_03, kPacket.m_nOK, 4 ); // 목록 전송 주기가 짧습니다.
    }

    SEND_PACKET( EVENT_ROOM_LIST_ACK );

    SetTick( SEND_ROOM_LIST );

    //LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == 1, clog, cwarn ) // 주석처리한 이유, 사내 테스트에서 로그가 많이 남아서.불편..
    //    << BUILD_LOG( kPacket.m_nOK )
    //    << BUILD_LOG( NetError::GetLastNetErrMsg() )
    //    << BUILD_LOG( kPacket_.m_nPage )
    //    << BUILD_LOG( kPacket_.m_bWaitRoom )
    //    << BUILD_LOG( kPacket_.m_nType )
    //    << BUILD_LOG( kPacket_.m_nDifficult ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_USER_LIST_REQ, int )
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

    // 내부에서 에러 셋팅 되어서 옴..
    m_pkChannel->GetUserList( kPacket_, kPacket );
    SetTick( SEND_USER_LIST );

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_LIST_00, kPacket.m_nOK, 1 ); // 목록이 비어 있습니다.
        ERR_CASE( ERR_LIST_03, kPacket.m_nOK, 4 ); // 목록 전송 주기가 짧습니다.
    default:
        START_LOG( cerr, L" 알수 없는 Err , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_USER_LIST_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nCurrentPage )
        << BUILD_LOG( kPacket.m_nMaxPage ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ITEM_EXPIRED_NOT, std::vector<KItem> )
{
    std::vector<KItem>::iterator vit;
    for( vit = kPacket_.begin(); vit != kPacket_.end(); vit++ )
    {
        // 실패든 아니든, 넘어온 아이템은 무조건 지운다.
        m_kInventory.RemoveItem( vit->m_ItemUID );

        // 클라이언트에서 보내줌.
        //ReleaseEquipElement( vit->m_dwUID, KItemUnit::IT_PERIOD );
        //ReleasePetEquipElement( vit->m_dwUID, KItemUnit::IT_PERIOD );
        ReleaseQuickSlot( vit->m_ItemUID );
        ReleasePetCostume( vit->m_ItemUID );
        if ( vit->CheckItemType(KItem::TYPE_LOOK) ) {
            ReleaseLookEquip( vit->m_ItemUID );
        }

        if( vit->m_nPeriod == KGSDBThread::UNKNOWN_ERROR )
        {
            START_LOG( cerr, L"기간제 아이템 기간 만료 DB처리 실패. Name : " << m_strName )
                << BUILD_LOG( vit->m_ItemUID )
                << BUILD_LOG( vit->m_ItemID )
                << END_LOG;
        }
    }

    KEVENT_ITEM_EXPIRED_NOT kPacket;
    kPacket.m_strLogin = m_strName;
    kPacket.m_vecItem = kPacket_;
    // 방이라면 룸에 관련된 정보 설정 후에 방원들에게 지운 아이템 전달
    if( m_pkRoom != NULL && GetStateID() == KUserFSM::STATE_ROOM )
    {
        m_pkRoom->SyncCharInfo( m_strName );
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_ITEM_EXPIRED_NOT, kPacket );
    }

    if( IsCouple() )
    {
        DumpCoupleInven();
        ChkCoupleItemExpired( kPacket_ );
        DumpCoupleInven();
    }

    SEND_PACKET( EVENT_ITEM_EXPIRED_NOT );
}

_IMPL_ON_FUNC( EVENT_DETAIL_USER_INFO_REQ, std::wstring )
{
    _VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY, 
        KUserFSM::STATE_CHANNEL,
        //KUserFSM::STATE_JOIN,
        KUserFSM::STATE_ROOM,
        KUserFSM::STATE_PLAYING));

    KEVENT_DETAIL_USER_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    if( kPacket_.empty() )
    {
        START_LOG( cwarn, L"정보를 요청한 유저의 이름이 비었음. 요청자:" << m_strName )
            << BUILD_LOG( GetStateIDString() )
            << END_LOG;

        kPacket.m_nOK = -1;
        SEND_PACKET( EVENT_DETAIL_USER_INFO_ACK );
        return;
    }

    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( kPacket_ ) );

    if( spUser == NULL )
    {
        START_LOG( cwarn, L"정보를 요청한 유저를 서버에서 찾을 수 없음. 요청자 : " << m_strName )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( GetStateIDString() )
            << END_LOG;

        kPacket.m_nOK = -2;
        SEND_PACKET( EVENT_DETAIL_USER_INFO_ACK );
        return;
    }

    kPacket.m_nOK = NetError::NET_OK;
    spUser->GetInfo( kPacket.m_kData );

    SEND_PACKET( EVENT_DETAIL_USER_INFO_ACK );

}

_IMPL_ON_FUNC( EVENT_P2P_RELAY_REQ, KP2PData )
{

    VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING ));

    JIF( m_pkRoom );

    // 로그를 찍고 싶지만 전송 주기가 매우 짧을 것이므로 생략한다.

    //m_pkRoom->RelayP2PData( *this, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_QUERY_SALED_DURATION_ITEM_LIST_ACK, KDurationInv )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_QUERY_SALED_DURATION_ITEM_LIST_ACK );
}

_IMPL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_GP_REQ, KAddItemInfo )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_ADD_NORMAL_ITEM_GP_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_GP_ACK, KPacketAck )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_ADD_NORMAL_ITEM_GP_ACK );
}

_IMPL_ON_FUNC( EVENT_ADD_DURATION_ITEM_GP_REQ, KAddItemInfo )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_ADD_DURATION_ITEM_GP_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_ADD_DURATION_ITEM_GP_ACK, KPacketAck )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_ADD_DURATION_ITEM_GP_ACK );
}

_IMPL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_CASH_REQ, KAddItemInfo )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_ADD_NORMAL_ITEM_CASH_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_CASH_ACK, KPacketAck )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_ADD_NORMAL_ITEM_CASH_ACK );
}

_IMPL_ON_FUNC( EVENT_ADD_DURATION_ITEM_CASH_REQ, KAddItemInfo )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_ADD_DURATION_ITEM_CASH_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_ADD_DURATION_ITEM_CASH_ACK, KPacketAck )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_ADD_DURATION_ITEM_CASH_ACK );
}

IMPL_ON_FUNC( EVENT_REMOVE_ITEM_REQ )
{

    QUEUEING_EVENT_TO_DB( EVENT_REMOVE_ITEM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_REMOVE_ITEM_ACK, KPacketAck )
{
    SEND_RECEIVED_PACKET( EVENT_REMOVE_ITEM_ACK );
}

_IMPL_ON_FUNC( EVENT_QUERY_USER_INFO_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_QUERY_USER_INFO_REQ, kPacket_ );
}

IMPL_ON_FUNC( EVENT_QUERY_USER_INFO_ACK )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_QUERY_USER_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_CHANGE_STRONG_ITEM_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_CHANGE_STRONG_ITEM_REQ, kPacket_ );
}

IMPL_ON_FUNC( EVENT_CHANGE_STRONG_ITEM_ACK )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_STRONG_ITEM_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_QUERY_ITEM_INFO_REQ )
{
    if( !IsAdmin() )    return;

    QUEUEING_ID_TO_DB( EVENT_QUERY_ITEM_INFO_REQ );
}

IMPL_ON_FUNC( EVENT_QUERY_ITEM_INFO_ACK )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_QUERY_ITEM_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_KICK_USER_BY_ADMIN_REQ, DWORD )
{
    int kPacket = -99;
    if( !IsAdmin() ) return;

    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_ ) ); //UID로 정보를 찾자.

    if( !spUser )
    {
        kPacket = -1; //유저없음
    }
    else
    {
        spUser->ReserveDestroy( DRW_KICK_BY_ADMIN );
        kPacket = 0; //존재함 튕겨냈음
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket ) << END_LOG;

    {
        std::wstringstream stm;
        stm << KNC_TIME_STRING << L"|" << m_strName << L"|" << KncUtil::toWideString( GetIPStr() ) << L"|" << kPacket_;
        SiKLogManager()->Get( KLogManager::LT_KICK_USER ) << stm.str() << GLOG::endl;

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }

    SEND_PACKET( EVENT_KICK_USER_BY_ADMIN_ACK );
}

//_IMPL_ON_FUNC( EVENT_KICK_USER_BY_ADMIN_ACK, KPacketAck )
//{
//    if( !IsAdmin() )    return;
//
//    SEND_RECEIVED_PACKET( EVENT_KICK_USER_BY_ADMIN_ACK );
//}

//IMPL_ON_FUNC( EVENT_CHANGE_USER_INFO_REQ ) // 08-04-21 tslayer. 운영자툴 기능 제한
//{
//    if( !IsAdmin() )    return;
//
//    QUEUEING_EVENT_TO_DB( EVENT_CHANGE_USER_INFO_REQ, kPacket_ );
//}

//_IMPL_ON_FUNC( EVENT_CHANGE_USER_INFO_ACK, KPacketAck )
//{
//    if( !IsAdmin() )    return;
//
//    SEND_RECEIVED_PACKET( EVENT_CHANGE_USER_INFO_ACK );
//}

_IMPL_ON_FUNC( EVENT_QUERY_USER_INFO2_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_QUERY_USER_INFO2_REQ, kPacket_ );
}

IMPL_ON_FUNC( EVENT_QUERY_USER_INFO2_ACK )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_QUERY_USER_INFO2_ACK );
}

_IMPL_ON_FUNC( EVENT_REMOVE_USER_INFO_REQ, KPacket2Name )
{
    if( !IsAdmin() )    return;

    QUEUEING_EVENT_TO_DB( EVENT_REMOVE_USER_INFO_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EVENT_REMOVE_USER_INFO_ACK, KPacketAck )
{
    if( !IsAdmin() )    return;

    SEND_RECEIVED_PACKET( EVENT_REMOVE_USER_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_UDP_P2P_SOCKET_ERR_NOT, std::wstring )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING ));
    JIF( m_pkRoom );
    
    bool bIsBanHost = false;
    if( !m_pkRoom->BanishOther( *this, kPacket_, KRoom::LEAVE_CLIENT_FIND_ERR, bIsBanHost ) )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_pkRoom->GetName() )
            << BUILD_LOG( m_pkRoom->GetStateIDString() )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( kPacket_ )
            << END_LOG;
        return;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_pkRoom->GetName() )
        << BUILD_LOG( m_pkRoom->GetStateIDString() )
        << BUILD_LOG( GetStateIDString() )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_BAN_USER_REQ, KPacketNameOK )
{
    _VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING ));
    JIF( m_pkRoom );

    bool bIsBanHost = false;
    if( !m_pkRoom->BanishOther( *this, kPacket_.m_strName, kPacket_.m_nOK, bIsBanHost ) )
    {
        if( bIsBanHost == true )
        {
            START_LOG( cwarn, L"방장을 강퇴함, 1 " ) << END_LOG;
            return;
        }
        JIF( m_pkRoom->m_spHostUser.get() );

        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_pkRoom->GetName() )
            << BUILD_LOG( m_pkRoom->m_usRoomID )
            << BUILD_LOG( m_pkRoom->GetStateIDString() )
            << BUILD_LOG( m_pkRoom->GetCount() )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( kPacket_.m_strName )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( m_pkRoom->m_spHostUser->GetName() )
            << END_LOG;
        return;
    }
    if( bIsBanHost == true )
    {
        START_LOG( cwarn, L"방장을 강퇴함, 2 " ) << END_LOG;
        return;
    }

    JIF( m_pkRoom->m_spHostUser.get() );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_pkRoom->GetName() )
        << BUILD_LOG( m_pkRoom->m_usRoomID )
        << BUILD_LOG( m_pkRoom->GetStateIDString() )
        << BUILD_LOG( m_pkRoom->GetCount() )
        << BUILD_LOG( GetStateIDString() )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_pkRoom->m_spHostUser->GetName() ) << END_LOG;
}

//IMPL_ON_FUNC_NOPARAM( EVENT_JOIN_COMPLETE_REQ )


IMPL_ON_FUNC_NOPARAM( EVENT_CLIENT_CRASHED_NOT )
{
    // 클라이언트가 Crash를 일으키고 비정상적으로 종료된 경우,
    // 서버에서의 disconnect 감지 시점을 앞당기기위해 이 메시지를 보낸다.
    ReserveDestroy( DRW_BY_CRASH );
}

_IMPL_ON_FUNC( EVENT_SERVER_MIGRATION_REQ, std::wstring )
{
    LIF( ForceTranslateState( KUserFSM::STATE_CHANNELLOBBY ) );

    SendExpUpdateToDBNotice();
    UpdateGachaReelPointToDB();

    KDB_EVENT_EXIT_GAME_NOT kNot2;
    GetDBUpdateData( kNot2 );
    if( kPacket_.empty() )
    {
        kPacket_ = L"NoNameServer";
    }
    kNot2.m_strWantedServerName = kPacket_;

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    // if( Is Server Migration )
    // User -> DB t. -> center proxy -> {} ->Center Proxy -> User (Net marble, Nexon, Connected CenterProxy )
    // User -> DB t. -> gsGash -> {} -> gsGash -> Center Proxy -> {} -> Center Proxy -> User ( Gash, Connected Center Proxy )

    // User -> DB t. -> User (Net marble, Nexon, DisConnected CenterProxy )
    // User -> DB t. -> gsGash -> {} -> gsGash -> User ( Gash, DisConnected Center Proxy )

    QUEUEING_EVENT_TO_DB( DB_EVENT_EXIT_GAME_NOT, kNot2 );
}

_IMPL_ON_FUNC( EVENT_SERVER_MIGRATION_ACK, int )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    if( kPacket_  == 0 )
    {
        m_bServerMigrationReserved = true; //정상적으로 DBup이 된경우 true
        SetTick( MIGRATION_TICK );
    }
    
    SetDisconnReason( DRW_SERVER_MIGRATION );
    SEND_RECEIVED_PACKET( EVENT_SERVER_MIGRATION_ACK );
}

_IMPL_ON_FUNC( EVENT_CLIENT_ERR_REPORT_COUNT_NOT, KEventErrCount )
{
    KEventErrCount::iterator mit = kPacket_.begin();
    KEventErrCount::iterator mit_end = kPacket_.end();

    for ( ; mit != mit_end; ++mit ) 
    {
        std::pair<int,int>  pairKey = mit->first;

        //  카운팅 패킷 처리
        switch( pairKey.first )
        {
        case KEventErr::ERR_P2PFAIL:
            SiKFailRate()->IncreaseCount( pairKey.second, mit->second );
            break;
        case KEventErr::ERR_CLIENTRPT:
        case KEventErr::ERR_CUSTOM:
            SiKFailRate()->IncreaseCErrCount( pairKey.second, mit->second );
            break;
        case KEventErr::ERR_NETWORK:
            KStatNetworkPtr spStat = GET_STAT( KStatNetwork, SI_STAT_NETWORK );
            if ( NULL != spStat ) {
                spStat->AddStat( pairKey.second, mit->second );
            }
            break;
        }
    }
}

_IMPL_ON_FUNC( EVENT_CLIENT_ERR_REPORT_NOT, KEventErr )
{
    //  데이터 실어 보내는 아이들에 대한 처리
    switch ( kPacket_.m_nErrType )
    {
    case KEventErr::ERR_P2PFAIL:
    case KEventErr::ERR_CLIENTRPT:
    case KEventErr::ERR_CUSTOM:
        {
            switch( kPacket_.m_nErrID )
            {    
            case KFailRate::CE_EXITWITHHACKALERT:
                {
                    std::wstringstream stm;

                    stm << SiKFailRate()->CErrIDString( kPacket_.m_nErrID )
                        << L", Time :  " << KNC_TIME_STRING
                        << L", LoginID : " << m_strName
                        << L", IP : " << KncUtil::toWideString( GetIPStr() )
                        << L", Comment : " << kPacket_.m_strComment;
                        
                    TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_LT_CLIENT_ERR_COMMENT, GetName(), GetUID(), 0, stm.str() );

                    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
                    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

                    //클라이언트에서 해킹이 감지되어 보고 되었다.
                    ++m_nHackReport;
                }
                break;
            case KFailRate::CE_TEAM_SCORE:
                {
                    KStatDeathMatchPtr spStat = GET_STAT( KStatDeathMatch, SI_STAT_DEATH );
                    JIF( spStat );
                    spStat->AddStatGap( kPacket_.m_dwReserve1, kPacket_.m_dwReserve2 );
                }
                break;
            case KFailRate::CE_NOT_EQUAL_QUEST_INFO:
                {
                    if( !kPacket_.m_strComment.empty() ) {

                        std::wstringstream stm;
                        stm << SiKFailRate()->CErrIDString( kPacket_.m_nErrID )
                            << L", Time :  " << KNC_TIME_STRING
                            << L", LoginID : " << m_strName
                            << L", IP : " << KncUtil::toWideString( GetIPStr() )
                            << L", Comment : " << kPacket_.m_strComment;

                        //TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_LT_29_ERR_COMMENT, GetName(), GetUID(), stm.str() );

                        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
                        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
                    }
                }
                break;
            case KFailRate::CE_ST_MEMBER_NOT_ING_P2P:
                {
                    std::wstringstream stm;
                    stm << SiKFailRate()->CErrIDString( kPacket_.m_nErrID )
                        << L", Time :  " << KNC_TIME_STRING
                        << L", LoginID : " << m_strName
                        << L", IP : " << KncUtil::toWideString( GetIPStr() )
                        << L", Comment : " << kPacket_.m_strComment;

                    TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_ST_MEMBER_NOT_ING_P2P, GetName(), GetUID(), 0, stm.str() );

                    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
                    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
                }
                break;
            case KFailRate::CE_USER_START_FAIL_STATE:
                {
                    std::wstringstream stm;
                    stm << SiKFailRate()->CErrIDString( kPacket_.m_nErrID )
                        << L", Time :  " << KNC_TIME_STRING
                        << L", LoginID : " << m_strName
                        << L", IP : " << KncUtil::toWideString( GetIPStr() )
                        << L", Comment : " << kPacket_.m_strComment;

                    TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_USER_START_FAIL_STATE, GetName(), GetUID(), 0, stm.str() );

                    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
                    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
                }
                break;
            default:
                break;
            }
        }        
        break;
    default:
        START_LOG( cerr, L"잘못된 오류보고 타입 Type : " << kPacket_.m_nErrType << L", Value : " << kPacket_.m_nErrID ) << END_LOG;
        break;
    }
}

IMPL_ON_FUNC( EVENT_USE_CHANGE_NICKNAME_REQ )
{
    //이것은 채널과 채널 로비 상태에서만 가능하다. 룸에서 하면 처리에 이상이 생길수 있다.
    _VERIFY_STATE( (2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL) );// 너무 위험한가?

    KEVENT_USE_CHANGE_NICKNAME_ACK kPacket;
    kPacket.m_nOK = -99;
    int nRet = -99;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 닉네임변경카드가 필요한 경우.
    if ( kPacket_.m_bUseItem ) {
        if ( kPacket_.m_ItemID != CHANGE_NICK_GOODS_ID ) {
            SET_ERR_GOTO( ERR_NICK_00, END_PROC );
        }

        pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
        if ( pkItem == NULL ) {
            SET_ERR_GOTO( ERR_NICK_00, END_PROC );
        }

        if ( pkItem->m_ItemID != CHANGE_NICK_GOODS_ID ) {
            SET_ERR_GOTO( ERR_NICK_00, END_PROC );
        }
    }

    // 운영자 예외처리
    if ( !IsAdmin() ) {
        // 허용 유니코드, 문자열 체크
        nRet = SiKSlangFilter()->IsValidateNickNameString( kPacket_.m_strNewNickName, m_strName, SiKGSSimLayer()->m_uiNickNameAvailable );
        if ( nRet != 0 ) {
            // 내부에서 결정된 에러처리
            if ( nRet == -5 ) {
                SET_ERR_GOTO( ERR_NICK_01, END_PROC );
            }
            if ( nRet == -4 ) {
                SET_ERR_GOTO( ERR_NICK_02, END_PROC );
            }

            SET_ERR_GOTO( ERR_NICK_03, END_PROC );
        }
    }

    kPacket_.m_strOldNickName = m_strNickName;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_USE_CHANGE_NICKNAME_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,      kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_NICK_00, kPacket.m_nOK, 1 ); // 유효한 닉네임 변경카드 아이템이 없습니다.
        ERR_CASE( ERR_NICK_01, kPacket.m_nOK, 2 ); // 닉네임 제한크기 초과 또는 크기가 0입니다.
        ERR_CASE( ERR_NICK_02, kPacket.m_nOK, 3 ); // 닉네임 중에 부적절한 것이 있습니다.
        ERR_CASE( ERR_NICK_03, kPacket.m_nOK, 4 ); // 닉네임 처리중 알 수 없는 에러.
        ERR_CASE( ERR_NICK_04, kPacket.m_nOK, 5 ); // 아이디와 기존 닉네임으로 해당 유저를 찾을 수 없음.
        ERR_CASE( ERR_NICK_05, kPacket.m_nOK, 6 ); // 이미 사용중인 닉네임 입니다.
        ERR_CASE( ERR_NICK_06, kPacket.m_nOK, 7 ); // 닉네임변경 아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음.
        ERR_CASE( ERR_NICK_07, kPacket.m_nOK, 8 ); // DB처리 실패
        ERR_CASE( ERR_NICK_08, kPacket.m_nOK, 9 ); // 서버 클라이언트 알고 있는 캐릭터 정보가 틀리다.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업

    default:
        START_LOG( cerr, L"닉네임 변경중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_USE_CHANGE_NICKNAME_ACK );
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( SiKGSSimLayer()->m_uiNickNameAvailable )
        << BUILD_LOG( CHANGE_NICK_GOODS_ID )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_strNewNickName )
        << BUILD_LOG( GetNickName() )
        << BUILD_LOG( kPacket_.m_bUseItem ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_USE_CHANGE_NICKNAME_ACK, KEVENT_USE_CHANGE_NICKNAME_REQ )
{
    KEVENT_USE_CHANGE_NICKNAME_ACK kPacket;
    kPacket.m_nOK = -99;
    std::wstring strOldNickName;

    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( kPacket_.m_bUseItem ) {
        LIF( m_kInventory.RemoveItem( kPacket_.m_ItemID, kPacket_.m_ItemUID ) );
    }

    kPacket.m_strNewNickName = kPacket_.m_strNewNickName;
    kPacket.m_UseItemUID = kPacket_.m_ItemUID;

    SiKGSSimLayer()->RemoveNickToLogin( m_strNickName );
    SiKGSSimLayer()->AddNickToLogin( m_strName, kPacket.m_strNewNickName );

    strOldNickName = m_strNickName;
    m_strNickName = kPacket.m_strNewNickName;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,      kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_NICK_00, kPacket.m_nOK, 1 ); // 유효한 닉네임 변경카드 아이템이 없습니다.
        ERR_CASE( ERR_NICK_01, kPacket.m_nOK, 2 ); // 닉네임 제한크기 초과 또는 크기가 0입니다.
        ERR_CASE( ERR_NICK_02, kPacket.m_nOK, 3 ); // 닉네임 중에 부적절한 것이 있습니다.
        ERR_CASE( ERR_NICK_03, kPacket.m_nOK, 4 ); // 닉네임 처리중 알 수 없는 에러.
        ERR_CASE( ERR_NICK_04, kPacket.m_nOK, 5 ); // 아이디와 기존 닉네임으로 해당 유저를 찾을 수 없음.
        ERR_CASE( ERR_NICK_05, kPacket.m_nOK, 6 ); // 이미 사용중인 닉네임 입니다.
        ERR_CASE( ERR_NICK_06, kPacket.m_nOK, 7 ); // 닉네임변경 아이템의 UID 또는 ID값이 다르거나 아이템이 존재하지 않음.
        ERR_CASE( ERR_NICK_07, kPacket.m_nOK, 8 ); // DB처리 실패
        ERR_CASE( ERR_NICK_08, kPacket.m_nOK, 9 ); // 서버 클라이언트 알고 있는 캐릭터 정보가 틀리다.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업

    default:
        START_LOG( cerr, L"닉네임 변경중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_USE_CHANGE_NICKNAME_ACK );
    SendUpdateGuildMemberNickNotice();

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( strOldNickName )
        << BUILD_LOG( GetNickName() )
        << BUILD_LOG( kPacket_.m_bUseItem ) << END_LOG;
}

IMPL_ON_FUNC(EVENT_USE_CHANGE_NICKCOLOR_REQ)
{
    //이것은 채널과 채널 로비 상태에서만 가능하다. 룸에서 하면 처리에 이상이 생길수 있다.
    _VERIFY_STATE((2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL));// 너무 위험한가?

    KEVENT_USE_CHANGE_NICKCOLOR_ACK kPacket;
    kPacket.m_nOK = -99;
    int nRet = -99;
    KItemPtr pkItem;

    SET_ERROR(ERR_UNKNOWN);

    // 닉네임변경카드가 필요한 경우.
    if (kPacket_.m_bUseItem) {
        if (kPacket_.m_ItemID != CHANGE_NICK_COLOR_GOODS_ID) {
            SET_ERR_GOTO(ERR_NICK_00, END_PROC);
        }

        pkItem = m_kInventory.FindItemByItemUID(kPacket_.m_ItemUID);
        if (pkItem == NULL) {
            SET_ERR_GOTO(ERR_NICK_00, END_PROC);
        }

        if (pkItem->m_ItemID != CHANGE_NICK_COLOR_GOODS_ID) {
            SET_ERR_GOTO(ERR_NICK_00, END_PROC);
        }
    }

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_USE_CHANGE_NICKCOLOR_REQ, kPacket_);
    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98); // 아직 처리중인 작업

    default:
        START_LOG(cerr, L"닉네임 변경중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg())
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket.m_nOK = -99;
    }

    if (kPacket.m_nOK != 0) {
        SEND_PACKET(EVENT_USE_CHANGE_NICKCOLOR_ACK);
    }

    _LOG_CONDITION(kPacket.m_nOK == 0, clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(m_strName)
        << BUILD_LOG(CHANGE_NICK_COLOR_GOODS_ID)
        << BUILD_LOG(kPacket_.m_ItemID)
        << BUILD_LOG(kPacket_.m_ItemUID)
        << BUILD_LOG(kPacket_.m_strNewNickColor)
        << BUILD_LOG(kPacket_.m_bUseItem) << END_LOG;
}

_IMPL_ON_FUNC(EVENT_USE_CHANGE_NICKCOLOR_ACK, KEVENT_USE_CHANGE_NICKCOLOR_REQ)
{
    KEVENT_USE_CHANGE_NICKCOLOR_ACK kPacket;
    kPacket.m_nOK = -99;

    NetError::SetLastNetError(kPacket_.m_nOK);

    if (!IS_CORRECT(NET_OK)) {
        goto END_PROC;
    }

    if (kPacket_.m_bUseItem) {
        LIF(m_kInventory.RemoveItem(kPacket_.m_ItemID, kPacket_.m_ItemUID));
    }

    kPacket.m_strNewNickColor = kPacket_.m_strNewNickColor;
    kPacket.m_UseItemUID = kPacket_.m_ItemUID;

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0); // 성공
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98); // 아직 처리중인 작업

    default:
        START_LOG(cerr, L"닉네임 변경중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg())
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET(EVENT_USE_CHANGE_NICKCOLOR_ACK);

    _LOG_CONDITION(kPacket.m_nOK == 0, clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg())
        << BUILD_LOG(GetName())
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kPacket_.m_strNewNickColor)
        << BUILD_LOG(GetNickName())
        << BUILD_LOG(kPacket_.m_bUseItem) << END_LOG;
}

IMPL_ON_FUNC( EVENT_REGIST_MISSION_REQ )
{
    //상태 확인.
    _VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));

    KEVENT_REGIST_MISSION_ACK kPacket;
    kPacket.m_nOK = -99;

    std::vector<DWORD>::iterator vit;
    KItemPtr pkItem;
    KDB_EVENT_REGIST_MISSION_REQ kDBPacket;
    KMissionInfo kMissionInfo;
    int nSkillID = 0;
    SSkillElement kSkill;

    SET_ERROR( ERR_UNKNOWN );

    // 이런 미션이 존재 하는가?
    if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, kPacket_.m_ItemID ) )
    {
        SET_ERR_GOTO( ERR_MISSION_10, END_PROC );
    }

    //현재 캐릭 + 공통미션 < 5
    //if( SiKMissionManager()->GetCount( m_vecMissionSlot, kMissionInfo.m_cCharType ) >= 5 )
    //{
    //    SET_ERR_GOTO( ERR_MISSION_08, END_PROC );
    //}

    //이미 동일한 미션을 수행중인가?
    if( IsRegistedMission( kPacket_.m_ItemID ) )
    {
        SET_ERR_GOTO( ERR_MISSION_01, END_PROC );
    }

    if( kMissionInfo.m_cCharType >= 0 )
    {
        // 캐릭터 정보 확인
        const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( kMissionInfo.m_cCharType );
        if( !pkCharacterInfo )
        {
            SET_ERR_GOTO( ERR_MISSION_12, END_PROC );
        }

        //해당 캐릭의 전직 상태
        if( kMissionInfo.m_cJobLv != -1 && pkCharacterInfo->m_cPromotion < kMissionInfo.m_cJobLv )
        {
            SET_ERR_GOTO( ERR_MISSION_13, END_PROC );
        }

        // 제한 레벨 이상인지 확인
        if( kMissionInfo.m_prLevelRage.first >= 0 &&
            (int)pkCharacterInfo->m_dwLevel < kMissionInfo.m_prLevelRage.first )
        {
            SET_ERR_GOTO( ERR_MISSION_14, END_PROC );
        }

        if( kMissionInfo.m_prLevelRage.second >= 0 &&
            (int)pkCharacterInfo->m_dwLevel > kMissionInfo.m_prLevelRage.second )
        {
            SET_ERR_GOTO( ERR_MISSION_14, END_PROC );
        }

        // 전직이면.
        if( kMissionInfo.m_cChangeJob > 0 && pkCharacterInfo->m_cPromotion >= kMissionInfo.m_cChangeJob )
        {
            SET_ERR_GOTO( ERR_MISSION_26, END_PROC );
        }
    }

    // 캐릭터 획득 이면..
    if( kMissionInfo.m_cNewChar > 0 && IsCharExist( kMissionInfo.m_cNewChar ) )
    {
        SET_ERR_GOTO( ERR_MISSION_25, END_PROC );
    }

    for( vit = kMissionInfo.m_vecInvalidMission.begin() ; vit != kMissionInfo.m_vecInvalidMission.end() ; ++vit )
    {
        if( GetMissionDataFromSlot( *vit ) != NULL )
        {
            SET_ERR_GOTO( ERR_MISSION_09, END_PROC );
        }
    }

    //아이템 존재 확인
    if( m_kInventory.FindItem( kPacket_.m_ItemID, kPacket_.m_ItemUID ) == NULL )
    {
        SET_ERR_GOTO( ERR_INVEN_01, END_PROC );
    }

    // 해당 미션이 스킬보상이 있는 스킬이면 해당 스킬 배운지에 대한 체크 필요.
    if( SiKSkillManager()->CheckRewardSkillMission( kPacket_.m_ItemID, nSkillID ) ) {
        // 이 스킬이 존재 하는가?
        if( !SiKSkillManager()->GetSkill( nSkillID, kSkill ) ) {
            SET_ERR_GOTO( ERR_MISSION_17, END_PROC );
        }

        // 이미 획득한 스킬인가?
        if( IsTrainedSkill( kSkill ) ) {
            SET_ERR_GOTO( ERR_MISSION_02, END_PROC );
        }
    }

    //DB큐잉
    kDBPacket.m_kPacket = kPacket_;
    GetDBUpdateData( kDBPacket.m_kUserData );
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_REGIST_MISSION_REQ, kDBPacket );


    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_MISSION_00,   kPacket.m_nOK, 1 );     // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_01,   kPacket.m_nOK, 2 );     // 현재 등록된 미션임
        ERR_CASE( ERR_MISSION_02,   kPacket.m_nOK, 3 );     // 이미 완료한 미션임
        ERR_CASE( ERR_MISSION_08,   kPacket.m_nOK, 4 );     // 미션 슬롯이 꽉찼음
        ERR_CASE( ERR_MISSION_09,   kPacket.m_nOK, 5 );     // 등록된 미션과 연관된 미션이므로 등록불가.
        ERR_CASE( ERR_MISSION_10,   kPacket.m_nOK, 6 );     // 미션 아이템이 아닌데 미션으로 등록하려고 했음
        ERR_CASE( ERR_MISSION_11,   kPacket.m_nOK, 7 );     // 미션 정보 가져오기 실패.
        ERR_CASE( ERR_MISSION_12,   kPacket.m_nOK, 8 );     // 캐릭터가 존재하지 않음
        ERR_CASE( ERR_MISSION_13,   kPacket.m_nOK, 9 );     // 전직 상태가 맞지 않음.
        ERR_CASE( ERR_MISSION_14,   kPacket.m_nOK, 10 );    // 캐릭터 레벨 제한을 만족하지 못함
        ERR_CASE( ERR_MISSION_17,   kPacket.m_nOK, 11 );    // 존재하는 미션이 아님.
        ERR_CASE( ERR_MISSION_20,   kPacket.m_nOK, 12 );    // Count SubMission 목록을 가져 오는중 실패.
        ERR_CASE( ERR_MISSION_21,   kPacket.m_nOK, 13 );    // 미션 처리시 DB 오류.
        ERR_CASE( ERR_INVEN_01,     kPacket.m_nOK, 14 );    // 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_MISSION_25,   kPacket.m_nOK, 15 );    // 이미 보유하고 있는 캐릭터 획득 미션을 등록하려 함.)
        ERR_CASE( ERR_MISSION_26,   kPacket.m_nOK, 16 );    // 이미 전직된 상태임.)
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"미션 등록중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_REGIST_MISSION_ACK );

    LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 4 || kPacket.m_nOK == 5 || kPacket.m_nOK == 15 || kPacket.m_nOK == 16 || kPacket.m_nOK == -98 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kMissionInfo.m_dwMissionID )
        << BUILD_LOGc( kMissionInfo.m_cCharType )
        << BUILD_LOGc( kMissionInfo.m_cJobLv )
        << BUILD_LOG( kMissionInfo.m_nPeriod )
        << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
        << BUILD_LOG( kMissionInfo.m_prLevelRage.second )
        << BUILD_LOG( kMissionInfo.m_vecInvalidMission.size() )
        << BUILD_LOG( kMissionInfo.m_vecInvalidComplete.size() )
        << BUILD_LOG( kMissionInfo.m_bUnique )
        << BUILD_LOG( kMissionInfo.m_vecSubmission.size() )
        << BUILD_LOGc( kMissionInfo.m_cNewChar )
        << BUILD_LOGc( kMissionInfo.m_cChangeJob )
        << BUILD_LOG( kMissionInfo.m_nGp )
        << BUILD_LOG( kMissionInfo.m_nExp )
        << BUILD_LOG( kMissionInfo.m_vecRewardItem.size() )
        << BUILD_LOG( kMissionInfo.m_vecSelectItem.size() ) << END_LOG;
}


IMPL_ON_FUNC( EVENT_REGIST_MISSION_ACK )
{
    //성공적이면 나의 슬롯에 추가하고, 미션 카드 제거.
    KItemPtr pkItem;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }

    m_vecMissionSlot.push_back( kPacket_.m_kMissiondata );

    pkItem = m_kInventory.FindItem( kPacket_.m_ItemID, kPacket_.m_ItemUID );
    if( pkItem == NULL )
    {
        START_LOG( cerr, L"낙장 불입__미션 등록 후 아이템을 제거하기 위해 찾지 못함. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_ItemID )
            << BUILD_LOG( kPacket_.m_ItemUID ) << END_LOG;
        goto END_PROC;
    }

    if( pkItem->m_nCount <= 0 )
    {
        START_LOG( cerr, L"낙장 불입__미션 등록 후 아이템을 수량을 깍지 못했음. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_ItemID )
            << BUILD_LOG( kPacket_.m_ItemUID ) << END_LOG;
        goto END_PROC;
    }
    pkItem->m_nCount -= 1;

    START_LOG( clog, L"새로운 미션 추가" )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_vecMissionSlot.size() )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_MISSION_00,   kPacket_.m_nOK, 1 );    // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_01,   kPacket_.m_nOK, 2 );    // 현재 등록된 미션임
        ERR_CASE( ERR_MISSION_02,   kPacket_.m_nOK, 3 );    // 이미 완료한 미션임
        ERR_CASE( ERR_MISSION_08,   kPacket_.m_nOK, 4 );    // 미션 슬롯이 꽉찼음
        ERR_CASE( ERR_MISSION_09,   kPacket_.m_nOK, 5 );    // 등록된 미션과 연관된 미션이므로 등록불가.
        ERR_CASE( ERR_MISSION_10,   kPacket_.m_nOK, 6 );    // 미션 아이템이 아닌데 미션으로 등록하려고 했음
        ERR_CASE( ERR_MISSION_11,   kPacket_.m_nOK, 7 );    // 미션 정보 가져오기 실패.
        ERR_CASE( ERR_MISSION_12,   kPacket_.m_nOK, 8 );    // 캐릭터가 존재하지 않음
        ERR_CASE( ERR_MISSION_13,   kPacket_.m_nOK, 9 );    // 전직 상태가 맞지 않음.
        ERR_CASE( ERR_MISSION_14,   kPacket_.m_nOK, 10 );   // 캐릭터 레벨 제한을 만족하지 못함
        ERR_CASE( ERR_MISSION_17,   kPacket_.m_nOK, 11 );   // 존재하는 미션이 아님.
        ERR_CASE( ERR_MISSION_20,   kPacket_.m_nOK, 12 );   // Count SubMission 목록을 가져 오는중 실패.
        ERR_CASE( ERR_MISSION_21,   kPacket_.m_nOK, 13 );   // 미션 처리시 DB 오류.
        ERR_CASE( ERR_INVEN_01,     kPacket_.m_nOK, 14 );   // 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_MISSION_25,   kPacket_.m_nOK, 15 );   // 이미 보유하고 있는 캐릭터 획득 미션을 등록하려 함.)
        ERR_CASE( ERR_MISSION_26,   kPacket_.m_nOK, 16 );   // 이미 전직된 상태임.)
    default:
        START_LOG( cerr, L"미션 등록중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        SET_ERROR( ERR_UNKNOWN );
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_REGIST_MISSION_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_MISSION_02 )
        << L" 미션 등록 로그 Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kMissiondata.m_dwMissionID )
        << BUILD_LOGtm( CTime(kPacket_.m_kMissiondata.m_tmRegDate) )
        << BUILD_LOGtm( CTime(kPacket_.m_kMissiondata.m_tmEndDate) )
        << BUILD_LOG( kPacket_.m_kMissiondata.m_mapCompletionInfo.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_REMOVE_MISSION_REQ, KEVENT_REMOVE_MISSION )
{
    //허용상태
    _VERIFY_STATE(( 5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE, KUserFSM::STATE_SQUARE_JOIN ));
    SET_ERROR( ERR_UNKNOWN );

    if( !SiKMissionManager()->IsEnableDelete( kPacket_.m_dwMissionID ) )
    {
        SET_ERR_GOTO( ERR_MISSION_29, END_PROC );
    }

    //지우려고하는 미션이 존재하는가
    if( !IsRegistedMission( kPacket_.m_dwMissionID ) )
    {
        SET_ERR_GOTO( ERR_MISSION_22, END_PROC );
    }

    //DB큐잉
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_REMOVE_MISSION_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성콩
        ERR_CASE( ERR_MISSION_00, kPacket_.m_nOK, 1 ); // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_22, kPacket_.m_nOK, 2 ); // 제거하려는 미션이 등록되어 있지 않음.
        ERR_CASE( ERR_MISSION_21, kPacket_.m_nOK, 3 ); // 미션 처리시 DB 오류.
        ERR_CASE( ERR_MISSION_29, kPacket_.m_nOK, 4 ); // 삭제할 수 없는 미션
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"미션 제거시 알수 없는 오류, Name : " << m_strName )
            << L"ErrMsg : " << NetError::GetLastNetErrMsg() << dbg::endl
            << L"ErrCode : " << NetError::GetLastNetError() << dbg::endl
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kPacket_.m_dwMissionID ) << END_LOG;
    }

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_RECEIVED_PACKET( EVENT_REMOVE_MISSION_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || kPacket_.m_nOK == -98 )
        << L" 미션 제거 로그, Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwMissionID ) << END_LOG;   
}

_IMPL_ON_FUNC( EVENT_REMOVE_MISSION_ACK, KEVENT_REMOVE_MISSION )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    if( !EraseMissionDataFromSlot( kPacket_.m_dwMissionID ) )
    {
        START_LOG( cerr, L"실제 미션 제거 시점에서 해당 미션을 찾을수 없음.. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_dwMissionID ) << END_LOG;
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MISSION_00, kPacket_.m_nOK, 1 ); // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_22, kPacket_.m_nOK, 2 ); // 제거하려는 미션이 등록되어 있지 않음.
        ERR_CASE( ERR_MISSION_21, kPacket_.m_nOK, 3 ); // 미션 처리시 DB 오류.
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"미션 제거시 알수 없는 오류, Name : " << m_strName )
            << L"ErrMsg : " << NetError::GetLastNetErrMsg() << dbg::endl
            << L"ErrCode : " << NetError::GetLastNetError() << dbg::endl
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kPacket_.m_dwMissionID ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_REMOVE_MISSION_ACK );
}

IMPL_ON_FUNC( EVENT_COMPLETE_MISSION_REQ )
{
    //허용상태
    _VERIFY_STATE(( 5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL,
        KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE, KUserFSM::STATE_SQUARE_JOIN ));

    KEVENT_COMPLETE_MISSION_ACK kPacket;
    KMissionData* pkMissionData = NULL;
    KMissionInfo kMissionInfo;

    SET_ERROR( ERR_UNKNOWN );

    int nRatio = CheckFatigueTime();
    if ( nRatio != -1 && nRatio != 100 ) {
        SET_ERR_GOTO( ERR_MISSION_27, END_PROC );
    }
    
    // 해당 미션이 존재하는가?
    if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, kPacket_.m_dwMissionID ) )
    {
        SET_ERR_GOTO( ERR_MISSION_17, END_PROC ); // 존재하는 미션이 아님.
    }

    if( !kMissionInfo.m_vecSelectItem.empty() &&
        !kMissionInfo.IsInSelection( kPacket_.m_dwSelection ) )
    {
        SET_ERR_GOTO( ERR_MISSION_24, END_PROC ); // 선택 보상을 선택하지 않았음
    }

    //해당 미션이 나의 미션슬롯에 있는가?
    pkMissionData = GetMissionDataFromSlot( kPacket_.m_dwMissionID );
    if( pkMissionData == NULL )
    {
        SET_ERR_GOTO( ERR_MISSION_18, END_PROC ); // 해당 미션이 슬롯에 존재하지 않음
    }

    //해당 미션의 완료 조건이 되는가?
    if( !SiKMissionManager()->CheckMissionComplete( *this, *pkMissionData, m_kInventory, kPacket_.vecDelItemInfo ) ) {
        goto END_PROC;
    }

    // 캐릭터 획득 미션일 경우 기본 복장 지급이 있는가?
    if( kMissionInfo.m_cNewChar > -1 ) {
        SiKCharDefault()->GetDefaultItemInfo( kMissionInfo.m_cNewChar, kPacket_.m_vecDefaultReward );
    }

    // 가이드에 해당하는 미션인지 확인
    if ( true == SiKSystemGuide()->CheckGuideNeedMission( kPacket_.m_dwMissionID, kPacket_.m_nGuideType ) ) {
        // 이미 완료 한 가이드인지 확인
        if ( true == m_kSystemGuideUserData.CheckAlreadyCompleteGuide( kPacket_.m_nGuideType ) ) {
            // 이미 완료 한 가이드이므로 패스하자
            kPacket_.m_nGuideType = -1;
        }
    }

    KCharacterInfo* pkCharacterInfo = GetCharacterInfo( GetCurrentCharType() );
    if( NULL != pkCharacterInfo ) {
        kPacket_.m_cCharPromotion = pkCharacterInfo->m_cPromotion;
    } else {
        kPacket_.m_cCharPromotion = 0;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwMissionID )
        << BUILD_LOG( kPacket_.vecDelItemInfo.size() )
        << BUILD_LOG( kPacket_.m_vecDefaultReward.size() )
        << END_LOG;

    //해당미션의 보상을 기입하여 DB로 보낸다.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_COMPLETE_MISSION_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MISSION_00, kPacket.m_nOK, 1 ); // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_11, kPacket.m_nOK, 2 ); // 미션 정보 가져오기 실패.
        ERR_CASE( ERR_MISSION_17, kPacket.m_nOK, 3 ); // 존재하는 미션이 아님.
        ERR_CASE( ERR_MISSION_18, kPacket.m_nOK, 4 ); // 해당 미션이 슬롯에 존재하지 않음.
        ERR_CASE( ERR_MISSION_19, kPacket.m_nOK, 5 ); // 미션 완료 조건을 만족하지 못함.
        ERR_CASE( ERR_MISSION_21, kPacket.m_nOK, 6 ); // 미션 처리시 DB 오류.
        ERR_CASE( ERR_MISSION_22, kPacket.m_nOK, 7 ); // 미션이 등록되어 있지 않음.
        ERR_CASE( ERR_MISSION_23, kPacket.m_nOK, 8 ); // 미션 보상 아이템이 DB에 존재하지 않음.
        ERR_CASE( ERR_MISSION_24, kPacket.m_nOK, 9 ); // 선택 보상을 선택하지 않았음.
        ERR_CASE( ERR_MISSION_28, kPacket.m_nOK, 10 ); // 서브 미션이 존재하지 않음.
        ERR_CASE( ERR_MISSION_29, kPacket.m_nOK, 12 ); // 서브 미션을 완료하기 위한 수치가 음수임.
        ERR_CASE( ERR_MISSION_30, kPacket.m_nOK, 13 ); // 서브 미션을 완료하기 위한 아이템이 없음
        ERR_CASE( ERR_MISSION_31, kPacket.m_nOK, 14 ); // 서브 미션을 완료하기 위한 보유 아이템 수량 부족
        ERR_CASE( ERR_MISSION_32, kPacket.m_nOK, 15 ); // 서브 미션을 완료하기 위한 캐릭터 레벨이 부족함
        ERR_CASE( ERR_MISSION_33, kPacket.m_nOK, 16 ); // 서버에 서브 미션 완료 정보가 없음
        ERR_CASE( ERR_MISSION_34, kPacket.m_nOK, 17 ); // 서브 미션을 완료하기 위한 수치가 부족함
        ERR_CASE( ERR_MISSION_35, kPacket.m_nOK, NetError::ERR_MISSION_35 ); // 피로도 상태라 미션 완료 불가.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"미션 완료요청시 알 수 없는 오류, Name : " << m_strName << L" Err Str : " 
            << NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || 
        NetError::GetLastNetError() == NetError::ERR_MISSION_24 || 
        NetError::GetLastNetError() == NetError::ERR_NOT_FINISHED_YET ||
        NetError::GetLastNetError() == NetError::ERR_MISSION_18 )
        << NetError::GetLastNetErrMsg() << L", Name : " << m_strName <<  dbg::endl
        << BUILD_LOG( kPacket_.m_dwMissionID )
        << BUILD_LOG( kPacket_.vecDelItemInfo.size() ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
        SEND_PACKET( EVENT_COMPLETE_MISSION_ACK ); // 실패시에만 ACK를 보내자.
}

IMPL_ON_FUNC( EVENT_COMPLETE_MISSION_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    KMissionInfo kMissionInfo;
    std::vector<KItem>::iterator vit2;
    std::set<int> setDungeonInfo;
    int nSkillID = -1;
    std::map<int,KDungeonUserInfo> mapDifficulty;
    GetUserDifficultInfo( mapDifficulty );

    __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }

    // 일반/에픽 미션 완료 기록 삽입
    m_vecFinishedMission.push_back( kPacket_.m_dwMissionID );
    // 클라 전달 데이터 세팅
    kPacket_.m_vecFinishedMission.clear();
    kPacket_.m_vecFinishedMission = m_vecFinishedMission;

    // Insert Inventory 하기 전에 처리해야 함.
    {
        KStatItemGetPtr spStat = GET_STAT( KStatItemGet, SI_STAT_ITEM_GET );
        if( spStat )
        {
            spStat->AddStat( -1, kPacket_.m_vecInv );
        }
    }

    m_kInventory.AddItemList( kPacket_.m_vecInv );

    //경험치등의 정보를 저장한다.
    if( SiKMissionManager()->GetMissionInfo( kMissionInfo, kPacket_.m_dwMissionID ) )
    {
        KCharacterInfo* pkCharacterInfo = GetCharacterInfo( kMissionInfo.m_cCharType );
        if( pkCharacterInfo )
        {
            float fExpRatio = 1.f;
            float fGpRatio = 1.f;
            char cOldLevel = static_cast<char>( pkCharacterInfo->m_dwLevel );
            SiKMissionManager()->GetMissionPenaltyRatio( kMissionInfo.m_nCorrectLv, (int)pkCharacterInfo->m_dwLevel, fExpRatio, fGpRatio );
            kPacket_.m_iExp = (int)(kPacket_.m_iExp * fExpRatio);
            kPacket_.m_iGP = (int)(kPacket_.m_iGP * fGpRatio);

            if( 0 > kPacket_.m_iExp ) {
                START_LOG( cerr, L"MissionExp Value is Minus" )
                    << BUILD_LOG( kPacket_.m_dwMissionID )
                    << BUILD_LOG( kPacket_.m_iExp ) << END_LOG;
            }

            pkCharacterInfo->m_biTotalExp += kPacket_.m_iExp; // 누적경험치 저장.

            if( (DWORD)SiKResultManager()->Exp_2_Level( pkCharacterInfo->m_biExp + kPacket_.m_iExp ) >= SiKResultManager()->GetMaxLevel() )
            {
                __int64 ExpTemp = pkCharacterInfo->m_biExp;

                ExpTemp = std::max<__int64>( 0, ExpTemp + kPacket_.m_iExp );
                ExpTemp = std::min<__int64>( SiKResultManager()->GetMaxExp(), ExpTemp );

                kPacket_.m_iExp = (int)( ExpTemp - pkCharacterInfo->m_biExp );
            }

            // 추가 경험치가 있느냐?
            if ( CheckExpCalcErr( ECC_EVENT_COMPLETE_MISSION_ACK_DEFAULT, 0, kPacket_.m_dwMissionID, kMissionInfo.m_cCharType, kPacket_.m_iExp, pkCharacterInfo->m_biExp ) ) {
                __int64 biCurruntExp = pkCharacterInfo->m_biExp;
                biCurruntExp += kPacket_.m_iExp;
                biCurruntExp = std::min<__int64>( biCurruntExp, biMaxExp ); // 최대 Exp 제한
                biCurruntExp = std::max<__int64>( biCurruntExp, USER_MIN_EXP ); // 최소 Exp 보장

                LIF( CheckExpCalcErr( ECC_EVENT_COMPLETE_MISSION_ACK_CALC, 0, kPacket_.m_dwMissionID, kMissionInfo.m_cCharType, kPacket_.m_iExp, pkCharacterInfo->m_biExp ) );

                // 경험치 적용
                pkCharacterInfo->m_biExp = biCurruntExp;
                UpdateGrade();
            }

            pkCharacterInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharacterInfo->m_biExp );

            if( kPacket_.m_cPromotionLevel > 0 )    // 전직 여부를 별도의 변수로 확인하지 않고, 값으로 체크한다.
            {
                pkCharacterInfo->m_cPromotion = kPacket_.m_cPromotionLevel;

                START_LOG( clog, L"character promotion. Login : " << m_strName 
                    << L", CharType : " << static_cast<int>(kMissionInfo.m_cCharType)
                    << L", Promotion : " << static_cast<int>(kPacket_.m_cPromotionLevel) ) << END_LOG;
                pkCharacterInfo->m_mapEquipSkillSet.insert( std::make_pair( pkCharacterInfo->m_cPromotion, 0 ) );
                AddNewSkillSet( pkCharacterInfo->m_cCharType, pkCharacterInfo->m_cPromotion );
            }

            // 전직할때의 SPPoint 체크 & 레벨업시의 SPPoint 체크.
            char cLevel         = SiKResultManager()->Exp_2_Level( pkCharacterInfo->m_biExp );
            if( (cOldLevel < cLevel) || kPacket_.m_cPromotionLevel > 0 ) {
                SiKSkillManager()->GetSkillPointInfo( pkCharacterInfo->m_cCharType, pkCharacterInfo->m_cPromotion, 
                    pkCharacterInfo->m_dwLevel, pkCharacterInfo->m_kSkillInfo.m_nLvSPPoint );
            }

            kPacket_.m_kSkillInfo = pkCharacterInfo->m_kSkillInfo;

            // 060728. kkurrung.
            if( kPacket_.m_iExp > 0 )
            {
                KSHCheckPoint::SHChecData shData;
                KEVENT_SHANGHAI_DROP_REQ shReq;

                shData.m_cCurrentCharID = pkCharacterInfo->m_cCharType;
                shData.m_cOldLevel      = cOldLevel;
                shData.m_cLevel         = static_cast<char>( pkCharacterInfo->m_dwLevel );
                shData.m_strLogin       = GetName();

                START_LOG( clog, L"SHANGHAI_DROPCHECK"  ) << END_LOG;

                if( m_tmFirstLoginTime > SiKGSSimLayer()->GetSHAdhustTime() &&
                    SiKSHCheckPoint()->DropCheck( shReq.m_vecDropItems, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, shData, KUser::USER_CT_TOTAL ) )
                {
                    START_LOG( clog, L"SHANGHAI_DROP size : ")
                        << BUILD_LOG( shReq.m_vecDropItems.size() )
                        << BUILD_LOG( shReq.m_vecDropPostItems.size() )
                        << BUILD_LOG( shReq.m_vecAutoMission.size() ) << END_LOG;
                }
                // PC방 유저가 레벨업 달성하면..,
                // 따로 패킷을 추가하지 않고 SHANGHAI Packet에 함께 넣어서 전송.
                if( IsPCBangUser() ) {
                    int nCount = SiKIPAdvantage()->AddLevelUpReward( shData.m_cOldLevel, shData.m_cLevel, shReq.m_vecDropItems );
                    START_LOG( clog, L"PC방 유저가 레벨업 해서 아이템 받음.. Name : " << GetName() <<  L", Count : " << nCount )
                        << BUILD_LOGc( shData.m_cOldLevel )
                        << BUILD_LOGc( shData.m_cLevel ) << END_LOG;
                }
                // 반갑다 친구야 레벨업시 아이템 지급 여부 체크.
                if( ( IsNewRecomUser() || IsReturnRecomUser() ) && m_MyRecomInfo.m_nState >= KRecomHelper::RBS_ACCEPT_RECOM ) {
                    std::pair<int,int> prRecomInfo = GetNewRecomUserInfo();
                    int nCount = RecomLevelUpCheck( shData.m_cOldLevel, shData.m_cLevel, prRecomInfo.second, shReq.m_vecDropItems );
                    START_LOG( clog, L"반갑다 친구야 신규/복귀 유저가 레벨업 해서 아이템 받음.. Name : " << GetName() <<  L", Count : " << nCount )
                        << BUILD_LOGc( shData.m_cOldLevel )
                        << BUILD_LOGc( shData.m_cLevel )
                        << BUILD_LOG( prRecomInfo.first )
                        << BUILD_LOG( prRecomInfo.second ) << END_LOG;
                }
                DWORD dwChannelType = 0;
                dwChannelType = GetUserChannelType();
                // 유저 채널 타입에 따른 레벨업 보상 이벤트 있는지 체크.
                if( SiKSHCheckPoint()->DropCheck( shReq.m_vecDropItems, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, shData, dwChannelType ) )  {
                    START_LOG( clog, L" 채널링 유저 레벨업 해서 아이템 지급 성공." )
                        << BUILD_LOGc( shData.m_cOldLevel )
                        << BUILD_LOGc( shData.m_cLevel ) << END_LOG;
                }

                if ( shData.m_cOldLevel < shData.m_cLevel ) {
                    SendGuildMemberLevelupNotice( pkCharacterInfo->m_cCharType, shData.m_cLevel );
                    shReq.m_cCharType = pkCharacterInfo->m_cCharType;
                    CheckLevelUpDrop( shReq );

                    // 레벨업시 설문정보 주는 부분 있는지 확인.
                    if ( SiKSurvey()->IsRun() ) {   // 이벤트 기간일때만 설문 확인 및 전달
                        CheckLevelUpSurvey( (int)pkCharacterInfo->m_cCharType, (int)shData.m_cLevel );
                    }
                    // 전직 미션 가이드 정보 체크
                    CheckJobMissionGuide( GetUID(), pkCharacterInfo->m_cCharType, static_cast< int >( shData.m_cLevel ) );

                    // 봉인 캐릭터 정보 갱신
                    UpdateUserSealedCharInfo( GetThisPtr(), pkCharacterInfo->m_cCharType, static_cast< int >( shData.m_cLevel ) );
                }
                
                //  레벨 초기화 이벤트 달성 여부 체크
                if ( SiKLevelResetEvent()->IsRun() && shData.m_cOldLevel < shData.m_cLevel )
                {
                    
                    std::vector<KDropItemInfo> vecReward;
                    bool bCashType = false;

                    bool bReward = CheckRewardLevel( pkCharacterInfo->m_cCharType, shData.m_cLevel, bCashType, vecReward );

                    if( bReward ) 
                    {
                        KDB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ kDBPacket;
                        kDBPacket.m_dwVersion = SiKLevelResetEvent()->GetVersion();
                        kDBPacket.m_cCharType = pkCharacterInfo->m_cCharType;
                        kDBPacket.m_bCashType = bCashType;
                        kDBPacket.m_vecItemInfo.swap( vecReward );
                        
                        //  보상 지급 요청 및 데이터 셋팅    
                        QUEUEING_EVENT_TO_DB( DB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ, kDBPacket );
                    }   
				} 
			}                    

            if( ( cOldLevel < cLevel ) && SiKGCHelper()->IsCharLevelEvent() ) { // 레벨업을 했을 경우에..,
                KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;
                if( SiKGCHelper()->GetCharLevelRewardItem( pkCharacterInfo->m_cCharType, cLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                    kDBPacket.m_setCharInfo.insert( pkCharacterInfo->m_cCharType );
                    QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket );
                }
            }
        }
    }

    // 060524. florist. 추가된 캐릭터가 있다면 캐릭터 정보를 얻어둔다.
    if( kPacket_.m_kCharacterInfo.m_cCharType > 0 )
    {
        LIF( m_mapCharacterInfo.insert( std::make_pair( kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo ) ).second );
        LIF( AddStatInitExpInfo( kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo.m_biInitExp, kPacket_.m_kCharacterInfo.m_biInitTotalExp ) );
        // 신규 캐릭터 획득시 룩 정보 추가.
        KLookEquipChar kLookEquip;
        kLookEquip.SetCharacter( kPacket_.m_kCharacterInfo.m_cCharType );
        m_mapLookEquip.insert( std::map<char,KLookEquipChar>::value_type( kLookEquip.GetCharacter(), kLookEquip ) );

        // 신규 캐릭터 무기교체 잠금
        bool bWeaponLock = false;
        m_mapChangeWeapon[kPacket_.m_kCharacterInfo.m_cCharType] = std::make_pair( bWeaponLock, 0 );
        kPacket_.m_kCharacterInfo.m_bChangeWeaponLock = bWeaponLock;

        // 신규유저 상하이 지급
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

    IncreaseGP( KGpStatInfo::GIT_COMPLETE_MISSION, false, kPacket_.m_iGP );

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
            const int& nModeID = *sit;

            std::map<int,KDungeonUserInfo>::iterator mit;
            mit  = mapDifficulty.find( nModeID );
            if ( mit == mapDifficulty.end() ) { // 현재 해당 던전이 오픈되지 않았을 경우에만 오픈해 주자.
                mapDifficulty[nModeID].m_kModeDifficulty.Set(0);
            }
            else {
                mit->second.m_kModeDifficulty.Set(0);
            }
        }
    }
    UpdateCharDifficultInfo( kPacket_.m_kCharacterInfo.m_cCharType, mapDifficulty );
    kPacket_.m_mapDifficulty = mapDifficulty;

    //수량
    for( vit2 = kPacket_.vecDelItemInfo.begin(); vit2 != kPacket_.vecDelItemInfo.end(); vit2++ )
    {
        KItemPtr pkItem = m_kInventory.FindItemByItemUID( vit2->m_ItemUID );
        if( pkItem != NULL )
        {
            pkItem->m_nCount -= vit2->m_nCount;
            *vit2 = *pkItem; //결과를 클라이언트 에게 전달한다.
        }

    }

    //슬롯에서 제거한다.
    EraseMissionDataFromSlot( kPacket_.m_dwMissionID );
    UpdateCompleteMission( kPacket_.m_dwMissionID );
    if( !kPacket_.m_vecMissiondata.empty() )
    {
        std::vector<KMissionData>::iterator vitChain;
        for( vitChain = kPacket_.m_vecMissiondata.begin() ; vitChain != kPacket_.m_vecMissiondata.end() ; ++vitChain )
            m_vecMissionSlot.push_back( *vitChain );
    }

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MISSION_00, kPacket_.m_nOK, 1 ); // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_11, kPacket_.m_nOK, 2 ); // 미션 정보 가져오기 실패.
        ERR_CASE( ERR_MISSION_17, kPacket_.m_nOK, 3 ); // 존재하는 미션이 아님.
        ERR_CASE( ERR_MISSION_18, kPacket_.m_nOK, 4 ); // 해당 미션이 슬롯에 존재하지 않음.
        ERR_CASE( ERR_MISSION_19, kPacket_.m_nOK, 5 ); // 미션 완료 조건을 만족하지 못함.
        ERR_CASE( ERR_MISSION_21, kPacket_.m_nOK, 6 ); // 미션 처리시 DB 오류.
        ERR_CASE( ERR_MISSION_22, kPacket_.m_nOK, 7 ); // 미션이 등록되어 있지 않음.
        ERR_CASE( ERR_MISSION_23, kPacket_.m_nOK, 8 ); // 미션 보상 아이템이 DB에 존재하지 않음.
        ERR_CASE( ERR_MISSION_24, kPacket_.m_nOK, 9 ); // 선택 보상을 선택하지 않았음.
    default:
        START_LOG( cerr, L"미션 완료요청시 알수 없는 오류, Name : " << m_strName )
            << L" Err Str : " << NetError::GetLastNetErrMsg() << END_LOG;
        kPacket_.m_nOK = -99;
    }
    // 받은것을 보낸다.
    SEND_RECEIVED_PACKET( EVENT_COMPLETE_MISSION_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L"미션 완료, Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << L" Err Code : " << NetError::GetLastNetError() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwMissionID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cPromotionLevel )
        << BUILD_LOG( kPacket_.m_iExp )
        << BUILD_LOG( kPacket_.m_iGP )
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( kPacket_.vecDelItemInfo.size() )
        << BUILD_LOG( kPacket_.m_vecMissiondata.size() )
        << BUILD_LOG( kPacket_.m_mapDifficulty.size() )
        << BUILD_LOGc( kPacket_.m_kCharacterInfo.m_cCharType )
        << BUILD_LOG( kPacket_.m_vecFinishedMission.size() )
        << BUILD_LOG( setDungeonInfo.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_DUNGEON_MISSION_REGIST_REQ, DWORD )
{
    //상태 확인.
    _VERIFY_STATE(( 1, KUserFSM::STATE_ROOM ));
    SET_ERROR( ERR_UNKNOWN );

    KDB_EVENT_DUNGEON_MISSION_REGIST_REQ kDBPacket;
    KEVENT_DUNGEON_MISSION_REGIST_ACK kPacket;
    kPacket.m_nOK = -99;
    KMissionInfo kMissionInfo;
    std::vector<DWORD>::iterator vit;

    // 이런 미션이 존재 하는가?
    if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_MISSION_10, END_PROC );
    }

    //이미 동일한 미션을 수행중인가?
    if( IsRegistedMission( kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_MISSION_01, END_PROC );
    }

    for( vit = kMissionInfo.m_vecInvalidMission.begin() ; vit != kMissionInfo.m_vecInvalidMission.end() ; ++vit )
    {
        if( GetMissionDataFromSlot( *vit ) != NULL )
        {
            SET_ERR_GOTO( ERR_MISSION_09, END_PROC );
        }
    }

    int nGameMode = -99;
    if ( m_pkRoom ) {
        nGameMode = m_pkRoom->m_iGameMode;
    }

    // 이미 클리어 한 미션인지 확인
    if ( true == IsAlreadyFinishedMission( kMissionInfo.m_dwMissionID ) ) {
        SET_ERR_GOTO( ERR_MISSION_02, END_PROC );
    }

    // 140116. nodefeat. 따라가기를 통해서 높은 레벨던전의 미션 클리어 하는 문제 수정으로 추가
    // 요청한 미션의 타입이 에픽 미션이면 미션등록에 필요한 조건에 해당하는 미션을 클리어했는지 확인
    if ( KMissionInfo::MISSION_TYPE_EPIC == kMissionInfo.m_cMissionType ) {
        if ( false == SiKMissionGettableCondition()->MissionRegistEnableCheck( kMissionInfo.m_dwMissionID, m_vecFinishedMission ) ) {
            SET_ERR_GOTO( ERR_MISSION_40, END_PROC );
        }
    }

    // 요청한 미션의 타입이 일반 미션이면 해당 던전의 에픽 미션을 클리어 했는지 확인
    if ( KMissionInfo::MISSION_TYPE_NORMAL == kMissionInfo.m_cMissionType ) {
        if ( false == CheckNormalMissionRegistEnable( nGameMode ) ) {
            SET_ERR_GOTO( ERR_MISSION_38, END_PROC );
        }
    }

    //DB큐잉
    kDBPacket.m_dwMissionID = kPacket_;
    GetDBUpdateData( kDBPacket.m_kUserData );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_DUNGEON_MISSION_REGIST_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, NetError::NET_OK );               // 성공
        ERR_CASE( ERR_MISSION_00,       kPacket.m_nOK, NetError::ERR_MISSION_00 );       // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_02,       kPacket.m_nOK, NetError::ERR_MISSION_02 );       // 이미 완료한 미션임
        ERR_CASE( ERR_MISSION_13,       kPacket.m_nOK, NetError::ERR_MISSION_13 );       // 전직 상태가 맞지 않음.
        ERR_CASE( ERR_MISSION_14,       kPacket.m_nOK, NetError::ERR_MISSION_14 );       // 캐릭터 레벨 제한을 만족하지 못함
        ERR_CASE( ERR_MISSION_38,       kPacket.m_nOK, NetError::ERR_MISSION_38 );       // 에픽미션을 클리어 하지 않아서 일반 미션 등록 불가능
        ERR_CASE( ERR_MISSION_40,       kPacket.m_nOK, NetError::ERR_MISSION_40 );       // 미션 등록에 필요한 조건에 해당하는 미션을 클리어하지 않았음
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, NetError::ERR_NOT_FINISHED_YET );

    default:
        START_LOG( cerr, L"던전 일반/에픽 미션 등록중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_DUNGEON_MISSION_REGIST_ACK );
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 4 || kPacket.m_nOK == 5 || kPacket.m_nOK == 15 || kPacket.m_nOK == 16 || kPacket.m_nOK == -98 )
        << L"던전 일반/에픽 미션 등록 요청 결과 : "
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kMissionInfo.m_dwMissionID )
        << BUILD_LOGc( kMissionInfo.m_cCharType )
        << BUILD_LOG( kMissionInfo.m_nPeriod )
        << BUILD_LOG( kMissionInfo.m_vecInvalidMission.size() )
        << BUILD_LOG( kMissionInfo.m_vecInvalidComplete.size() )
        << BUILD_LOG( kMissionInfo.m_bUnique )
        << BUILD_LOG( kMissionInfo.m_vecSubmission.size() )
        << BUILD_LOG( kMissionInfo.m_nGp )
        << BUILD_LOG( kMissionInfo.m_nExp )
        << BUILD_LOG( kMissionInfo.m_vecRewardItem.size() )
        << BUILD_LOG( kMissionInfo.m_vecSelectItem.size() )
        << BUILD_LOG( kMissionInfo.m_nDungeonID )
        << BUILD_LOGc( kMissionInfo.m_cMissionType )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_DUNGEON_MISSION_REGIST_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }

    m_vecMissionSlot.push_back( kPacket_.m_kMissiondata );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_MISSION_00,   kPacket_.m_nOK, 1 );    // 유저가 존재하지 않음
        ERR_CASE( ERR_MISSION_01,   kPacket_.m_nOK, 2 );    // 현재 등록된 미션임
        ERR_CASE( ERR_MISSION_02,   kPacket_.m_nOK, 3 );    // 이미 완료한 미션임
        ERR_CASE( ERR_MISSION_08,   kPacket_.m_nOK, 4 );    // 미션 슬롯이 꽉찼음
        ERR_CASE( ERR_MISSION_09,   kPacket_.m_nOK, 5 );    // 등록된 미션과 연관된 미션이므로 등록불가.
        ERR_CASE( ERR_MISSION_11,   kPacket_.m_nOK, 7 );    // 미션 정보 가져오기 실패.
        ERR_CASE( ERR_MISSION_12,   kPacket_.m_nOK, 8 );    // 캐릭터가 존재하지 않음
        ERR_CASE( ERR_MISSION_13,   kPacket_.m_nOK, 9 );    // 전직 상태가 맞지 않음.
        ERR_CASE( ERR_MISSION_17,   kPacket_.m_nOK, 11 );   // 존재하는 미션이 아님.
        ERR_CASE( ERR_MISSION_20,   kPacket_.m_nOK, 12 );   // Count SubMission 목록을 가져 오는중 실패.
        ERR_CASE( ERR_MISSION_21,   kPacket_.m_nOK, 13 );   // 미션 처리시 DB 오류.
        ERR_CASE( ERR_MISSION_25,   kPacket_.m_nOK, 15 );   // 이미 보유하고 있는 캐릭터 획득 미션을 등록하려 함.)
        ERR_CASE( ERR_MISSION_26,   kPacket_.m_nOK, 16 );   // 이미 전직된 상태임.)
    default:
        START_LOG( cerr, L"미션 등록중 알수 없는 오류.. MSG : " << NetError::GetLastNetErrMsg() )
            << L" Err Code : " << NetError::GetLastNetError() << END_LOG;
        SET_ERROR( ERR_UNKNOWN );
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_DUNGEON_MISSION_REGIST_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_MISSION_02 )
        << L"던전 일반/에픽 미션 등록 응답 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kMissiondata.m_dwMissionID )
        << BUILD_LOGtm( CTime(kPacket_.m_kMissiondata.m_tmRegDate) )
        << BUILD_LOGtm( CTime(kPacket_.m_kMissiondata.m_tmEndDate) )
        << BUILD_LOG( kPacket_.m_kMissiondata.m_mapCompletionInfo.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_FINISHED_MISSION_LIST_NOT )
{
    if ( false == kPacket_.empty() ) {
        m_vecFinishedMission.clear();
        m_vecFinishedMission = kPacket_;
    }

    SEND_RECEIVED_PACKET( EVENT_FINISHED_MISSION_LIST_NOT );
}

IMPL_ON_FUNC(EVENT_PVP_ROOM_LIST_SEARCH_REQ)
{
    _VERIFY_STATE((1, KUserFSM::STATE_CHANNEL));

    JIF(m_pkChannel);
    SET_ERROR(ERR_UNKNOWN);
    KPagePvPInfo kPacket;
    kPacket.m_buffCompList.Clear();

    m_pkChannel->GetFilteredPvPRoomList(kPacket_.m_cRoomType, kPacket_.m_kRoomOptions, kPacket);
    SetTick(SEND_ROOM_LIST);

    //END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_LIST_00, kPacket.m_nOK, 1); // ¸ñ·ÏÀÌ ºñ¾î ÀÖ½À´Ï´Ù.
        ERR_CASE(ERR_LIST_01, kPacket.m_nOK, 2); // ÇØ´ç Ä«Å×°í¸® ¸ðµå ¸ñ·ÏÀÌ ¾ø½À´Ï´Ù.
    }

    START_LOG(cerr, "Result")
        << BUILD_LOG(kPacket.m_nOK)
        << END_LOG;

    SEND_PACKET(EVENT_PVP_ROOM_LIST_SEARCH_ACK);

    SetTick(SEND_ROOM_LIST);
}

_IMPL_ON_FUNC(EVENT_CHANGE_PVP_ROOM_EXTRA_OPTION_REQ, KRoomOptions)
{
    SET_ERROR(ERR_UNKNOWN);

    KEVENT_CHANGE_PVP_ROOM_EXTRA_OPTION_ACK kPacket;
    kPacket.m_nOK = 0;
    kPacket.m_kRoomOptions = KRoomOptions();

    if (GetStateID() != KUserFSM::STATE_ROOM || m_pkRoom == NULL || m_pkChannel == NULL) {

        SET_ERROR(ERR_USER_00);

        kPacket.m_nOK = 1;

        SEND_COMPRESS_PACKET(EVENT_CHANGE_PVP_ROOM_EXTRA_OPTION_ACK, kPacket);
        START_LOG(cerr, NetError::GetLastNetErrMsg())
            << BUILD_LOG((GetStateID() != KUserFSM::STATE_ROOM))
            << BUILD_LOG((m_pkRoom == NULL)) << END_LOG;
        return;
    }

    m_pkRoom->ChangeRoomExtraOptions(this, kPacket_); //ÀÌ ¾È¿¡¼­ ¹æÀåÀÎÁö ¾îµå¹ÎÀÎÁö È®ÀÎÇÑ´Ù.

    int nOK = 1;
    //END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_ROOM_39, kPacket.m_nOK, -1); // ¹æÁ¤º¸ º¯°æ ½ÇÆÐ. ¹æÀå ºÎÀç
        ERR_CASE(ERR_ROOM_40, kPacket.m_nOK, -2); // ¹æÁ¤º¸ º¯°æ ½ÇÆÐ. ¹æÀå ¶Ç´Â ¿î¿µÀÚ°¡ ¾Æ´Ô
    default:
        START_LOG(cerr, L"Error")
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg())
            << BUILD_LOG(GetName()) << END_LOG;
    }

    if (kPacket.m_nOK == 0)
    {
        m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_CHANGE_PVP_ROOM_EXTRA_OPTION_BROAD, kPacket_);
    }

    LOG_SUCCESS(nOK == 0 || nOK == -1 || nOK == -2)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_.m_dwEnabledOptions)
        << BUILD_LOGc(kPacket_.m_dwDisabledChars)
        << BUILD_LOG(kPacket_.m_dwAllowedRankings)
        << END_LOG;
}

IMPL_ON_FUNC(EVENT_IN_ROOM_CHANGE_CHARACTER_INFO_REQ)
{
    VERIFY_STATE((5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING, KUserFSM::STATE_AGIT));

    KDB_EVENT_CHANGE_CHARACTER_INFO_REQ kDBPacket;
    KDB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ kDBLogPacket;

    if (GetCurrentCharType() != kPacket_.m_cPrevChar) {
        START_LOG(cerr, L"¼­¹ö-Å¬¶óÀÌ¾ðÆ®ÀÇ ÇöÀç Ä³¸¯ÅÍ°¡ ´Ù¸£´Ù, User: " << GetName())
            << END_LOG;
        // ¸»ÀÌ ¾ÈµÇ´Â °æ¿ìÁö¸¸ ÇØ´ç °æ¿ì¿¡ ¼­¹ö¿¡¼­ ¾Ë°í ÀÖ´Â Á¤º¸·Î °»½ÅÇÑ´Ù.
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
    kDBPacket.m_bInPvP = true;

    QUEUEING_EVENT_TO_DB_CHECK(DB_EVENT_CHANGE_CHARACTER_INFO_REQ, kDBPacket);

    // °ø¿ëÃ¢°í ¼ö·® ¾ÆÀÌÅÛ ·Î±× Àü´Þ.
    GetInvenDepotMoveInfo(kDBLogPacket);
    //QUEUEING_EVENT_TO_DB( DB_EVENT_INVEN_DEPOT_MOVE_LOG_REQ, kDBLogPacket );

    SetCurrentChar(kPacket_.m_cNextChar); // ÇöÀç Å¬¶óÀÌ¾ðÆ®ÀÇ º¯°æ Ä³¸¯ÅÍ·Î º¯°æ.

    // ¿Ï·á ¹Ì¼Ç ¸®½ºÆ® ¾ò±â
    QUEUEING_ID_TO_DB(DB_EVENT_FINISHED_MISSION_LIST_REQ);

END_PROC:
    ;
}

_IMPL_ON_FUNC(EVENT_IN_ROOM_CHANGE_CHARACTER_INFO_ACK, KEVENT_CHANGE_CHARACTER_INFO_ACK)
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

    m_mapPetInfo = kPacket_.m_mapPetInfo; // Æê Á¤º¸ °»½Å.
    m_vecCollectionSlot = kPacket_.m_vecCollection; // ÄªÈ£ Á¤º¸ °»½Å.
    m_vecMissionSlot = kPacket_.m_vecMissionSlot; // ¹Ì¼ÇÁ¤º¸ °»½Å.
    kPacket_.m_vecFontVector = m_vecFontVector;
    kPacket_.m_iPvExp = m_iPvExp;
    // ½ºÅ³ º¯°æ Á¤º¸ ÃÊ±âÈ­
    // DB µ¿±âÈ­¸¦ Çß±â ¶§¹®¿¡ Init°ªÀ» ÇöÀç°ª°ú µ¿±âÈ­.
    m_setAddReserveSkill.clear();
    m_setDeleteReserveSkill.clear();
    m_mapInitSkillSets = m_mapSkillSets;

    m_mapOldDifficulty.clear();
    if (false == kPacket_.m_bReadOldModwLevel) {
        m_mapOldDifficulty.clear();
        std::map<int, KDungeonUserInfo>::iterator mit;
        for (mit = kPacket_.m_mapDifficulty.begin(); mit != kPacket_.m_mapDifficulty.end(); ++mit) {
            m_mapOldDifficulty[static_cast<char>(mit->first)] = mit->second.m_kModeDifficulty; // ÃÊ±â°ª ÀúÀå.
        }
    }

    // db¿¡¼­ ¾ò¾î¿Â ½ºÅ×ÀÌÁö Å¬¸®¾î Á¤º¸ Áß ²À ÇÊ¿äÇÑ ¸ðµåÀÇ Á¤º¸°¡ ºñ¾îÀÖ´Ù¸é Ã¤¿ö³Ö¾îÁØ´Ù.
    for (int i = 0; i < NUM_GC_GAME_MODE; ++i) {
        const int& nModeID = i;

        if (false == KRoom::IsDifficultyMode(nModeID)) {
            continue;
        }

        if (kPacket_.m_mapDifficulty.find(nModeID) == kPacket_.m_mapDifficulty.end()) {
            KDungeonUserInfo kInfo;
            kPacket_.m_mapDifficulty.insert(std::make_pair(nModeID, kInfo));
        }

        // ´øÀü ÃÖ´ë ³­ÀÌµµ Å¬¸®¾î ¼³Á¤ (Çï¸ðµå Á¦¿Ü)
        int nLastDifficulty = SiKResultManager()->GetLastDifficulty(nModeID);
        if (nLastDifficulty > -1) {
            if (kPacket_.m_mapDifficulty[nModeID].m_kModeDifficulty.Get(nLastDifficulty)) {
                kPacket_.m_mapDifficulty[nModeID].m_kClearData.m_bClear = true;
            }
        }

        KHeroDungeon kInfo;
        bool bHeroDungeon = SiKHeroDungeonManager()->GetHeroDungeonInfo(nModeID, kInfo); // ¿µ¿õ´øÀüÀÌ¸é Á¤º¸ È¹µæ

        // ÀÏ¹Ý´øÀüÀÌ°Å³ª ³­ÀÌµµ ¸®¼ÂÇÏÁö¾Ê´Â ¿µ´øÀÏ °æ¿ì.
        if (!bHeroDungeon || (bHeroDungeon && !kInfo.m_bResetDifficulty)) {
            // ÇÃ·¹ÀÌ°¡´ÉÇÑ ³­ÀÌµµ Å¬¸®¾î ¼³Á¤
            for (int i = GC_GMD_NORMAL_BEGIN; i < nLastDifficulty; ++i) {
                if (false == kPacket_.m_mapDifficulty[nModeID].m_kModeDifficulty.Get(i)) {
                    break;
                }

                kPacket_.m_mapDifficulty[nModeID].m_kClearData.m_shMaxDifficulty = i;
            }
        }
    }

    UpdateCharDifficultInfo(kPacket_.m_cCharacter, kPacket_.m_mapDifficulty);

    // LastPlayÄ³¸¯ÅÍ Á¹¾÷ Á¤º¸¸¸ °¡Áö°í ÀÖÀ¸¹Ç·Î Ä³¸¯ÅÍ º¯°æ ½Ã Àü´ÞÇÏµµ·Ï ¼öÁ¤
    CheckCharacterGraduate(KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_CONNECT);

    // Á¢¼Ó ¼±¹°»óÀÚ À¯ÀúÁ¤º¸ Àü´Þ
    m_kConnectionGiftBoxUserData.SetData(static_cast<int>(kPacket_.m_cCharacter), kPacket_.m_kConnectionGiftBoxUpdateData);
    SendConnectionGiftBoxInfoNot(KConnectionGiftboxInfoNot::EST_ON_CONNECT);

    LIF(GetEquipItem(kPacket_.m_cCharacter, kPacket_.m_vecEquipItems));
    LIF(GetLookEquipItem(kPacket_.m_cCharacter, kPacket_.m_vecLookEquips));

    SendInventoryInfo(kPacket_.m_vecInv);
    kPacket_.m_vecInv.clear(); // ¾ÆÀÌÅÛ Á¤º¸¸¦ º¸³»ÁØ´ÙÀ½ ¾ÆÀÌÅÛ Á¤º¸ clear½ÃÅ°´Ù.
    SEND_COMPRESS_PACKET(EVENT_IN_ROOM_CHANGE_CHARACTER_INFO_ACK, kPacket_);
    SendUserGCPointInfo(); // GC Æ÷ÀÎÆ® Á¤º¸ °¡Á®¿À±â.

    // ¿µ¿õ´øÀü Å¬¸®¾î Á¤º¸ ¹Þ¾Æ¿À±â
    QUEUEING_ID_TO_DB(EVENT_USER_HERO_DUNGEON_INFO_REQ);

    // ÇöÀç ¼­¹ö¿¡¼­ Ä³¸¯ÅÍÀÇ Äü½½·Ô °¡Áö°í ÀÖ´ÂÁö Ã¼Å©.
    std::map< char, KQuickSlot >::iterator mit;
    mit = m_mapQuickSlot.find(kPacket_.m_cCharacter);
    if (mit == m_mapQuickSlot.end()) {
        QUEUEING_ID_TO_DB(EVENT_SLOT_INFO_NOT); // Äü½½·Ô Á¤º¸ °¡Á®¿À±â.
    }
    else {
        KQuickSlot kQuickSlot;
        kQuickSlot = mit->second;
        SEND_DATA(EVENT_SLOT_INFO_NOT, kQuickSlot);
    }

    CheckBonusRefill(); // º¸³Ê½º ¶óÀÌÇÁ ÃæÀü
}

_IMPL_ON_FUNC(EVENT_IN_ROOM_CHANGE_INDOOR_USERINFO_REQ, KInDoorUserInfo)
{
    VERIFY_STATE((5, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM, KUserFSM::STATE_PLAYING, KUserFSM::STATE_AGIT));
    JIF(m_pkRoom);

    const KInDoorUserInfo* pKInDoorUserInfo = m_pkRoom->FindInDoorUserInfo(m_strName);
    JIF(pKInDoorUserInfo);

    kPacket_.m_bIsObserver = pKInDoorUserInfo->m_bIsObserver;
    kPacket_.m_iPlayerIndex = pKInDoorUserInfo->m_iPlayerIndex;
    kPacket_.m_nState = pKInDoorUserInfo->m_nState;

    DumpIndoorUserInfo(kPacket_);
    JIF(m_pkRoom->AssignInDoorUserInfo(kPacket_));
    m_pkRoom->BroadPacket(KRoom::BROAD_TO_ALL, m_strName, KUserEvent::EVENT_IN_ROOM_CHANGE_INDOOR_USERINFO_BROAD, kPacket_, true);
}