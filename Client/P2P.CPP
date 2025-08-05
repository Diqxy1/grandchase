#include "stdafx.h"
#pragma comment( lib, "Ws2_32" )
//#include "P2P.h"




//
//

#include "ClientErr.h"

ImplementSingleton( KP2P );

#define NULL_CHECK(x) if( x == NULL || !x->IsConnected()) return false

// 패킷을 보내는 딜레이를 줄 수 있습니다
#define DELAY_CHECK( x, y ) \
    if( timeGetTime() - GetTimer(x) < 1000 * y/*초*/ ) \
        return false; \
    SetTimer(x)

void KP2P::Reset()
{
    m_hWnd = NULL;
    SAFE_DELETE(m_kMsgServer);
    SAFE_DELETE( m_pkUserProxy ); 

#if !defined _OPERATOR_TOOL_

    m_spTRUserProxy = boost::shared_ptr< KTRUser >( new KTRUser( true ) );
    m_spTRUserProxy->Init( false );

    m_spNUserProxy  = boost::shared_ptr< KNUserProxy >( new KNUserProxy );

    m_spNUserProxy->SetClassID( NUSERPROXY_ID );
    m_spTRUserProxy->SetClassID( TRUSERPROXY_ID );
#endif

    // 타이머 초기화 할 때
    DWORD dwTick = timeGetTime();
    for( int i = 0; i < ESPT_NUM; ++i )
        m_adwSendPacketTimer[i] = dwTick - 1000 * 3600;

    return;
}

bool KP2P::Init( HWND hWnd_ )
{
    m_hWnd = hWnd_;

#ifndef _OPERATOR_TOOL_
    m_spTRUserProxy->SetHwnd( hWnd_ );
    KUserProxy::InitForClient();

    m_spNUserProxy->Init( false );
    m_spNUserProxy->SetHwnd( m_hWnd );

    //m_spMSProxy->Init( false );
    //m_spMSProxy->SetHwnd( m_hWnd );

#endif
    m_kMsgServer = new KGCMsgServer( m_hWnd );

    return true;
}

bool KP2P::IsConnectedToGameServer()
{
    return m_pkUserProxy && m_pkUserProxy->IsConnected();
}

bool KP2P::Send_CreateRoomReq( const KRoomInfo& kRoomInfo_, const KInDoorUserInfo& kInDoorUserInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CREATE_ROOM_REQ  kPacket;
    kPacket.m_kRoomInfo = kRoomInfo_;
    kPacket.m_kInDoorUserInfo = kInDoorUserInfo_;

    //if( kPacket.m_kRoomInfo.m_iGameMode == GC_GM_QUEST49 )
    //{
    //    kPacket.m_kRoomInfo.m_iGameMode = GC_GM_QUEST48;
    //}

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CREATE_ROOM_REQ, kPacket, true, true );
} // Send_CreateRoomReq()

//{{ Appended by sman: 2004-05-17 오전 10:11:27
bool KP2P::Send_WhisperReq( const wchar_t* szPeerName_, const wchar_t* szToID, const wchar_t* szMessage_ )
{
    NULL_CHECK( m_pkUserProxy );
    KPacketNameMsg kPacket;
    kPacket.m_strName   = szToID;
    kPacket.m_strMsg    = szMessage_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_WHISPER_REQ, kPacket );
}
//}} Appended by sman: 2004-05-17 오전 10:11:27



bool KP2P::Send_JoinRoomReq( const unsigned int uiJoinType,
                            USHORT usRoomID,
                            const wchar_t* szRoomPasswd,
                            const KInDoorUserInfo& kInDoorUserInfo,
                            std::vector<USHORT> vecKickedRoomID,
                            char cQuickJoinCategory /*= GC_GMC_INVALID*/,
                            int iQuickJoinMode /*= GC_GM_INVALID*/,
                            int nDifficult /*= GC_SGM_QUEST_LEVEL1*/,
                            bool bGuild_,
                            std::map<DWORD,DWORD> mapJoinOption)
{
    //if( GC_GM_QUEST49 == iQuickJoinMode )
    //{
    //    iQuickJoinMode = GC_GM_QUEST48;
    //}

    NULL_CHECK( m_pkUserProxy );
    KJoinRoomReqInfo kPacket;
    kPacket.m_uiJoinType = uiJoinType;
    kPacket.m_cQuickJoinCategory = cQuickJoinCategory;
    kPacket.m_nQuickJoinModeID = iQuickJoinMode;
    kPacket.m_nDifficult = nDifficult;
    kPacket.m_vecKickedRoomID = vecKickedRoomID;
    kPacket.m_bGuild = bGuild_;

    switch( uiJoinType )
    {
        case KJoinRoomReqInfo::JOIN_NORMAL:
        {
            kPacket.m_usRoomID          = usRoomID;
            kPacket.m_strRoomPasswd     = szRoomPasswd;
            kPacket.m_kInDoorUserInfo   = kInDoorUserInfo;
            break;
        }
        case KJoinRoomReqInfo::JOIN_QUICK:
        {
            kPacket.m_kInDoorUserInfo   = kInDoorUserInfo;
            kPacket.m_cQuickJoinCategory = cQuickJoinCategory;
            kPacket.m_nQuickJoinModeID = iQuickJoinMode;
            kPacket.m_mapJoinOption = mapJoinOption;
            break;
        }

        case KJoinRoomReqInfo::JOIN_GUILD:
        {
            kPacket.m_usRoomID          = usRoomID;
            kPacket.m_strRoomPasswd     = szRoomPasswd;    // 방 비밀번호 란에 유저의 Login을 넣어서 보낸다.
            kPacket.m_kInDoorUserInfo   = kInDoorUserInfo;
            break;
        }

    default:
        return false;
    }

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JOIN_ROOM_REQ, kPacket, true, true );
}

//bool KP2P::Send_QuickJoinReq( const std::vector<DWORD>& vecIP_, const std::vector<USHORT>& vecPort_ )
//{
//    KJoinRoomReqInfo kPacket;
//    kPacket.m_vecIP     = vecIP_;
//    kPacket.m_vecPort   = vecPort_;
//
//    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUICK_JOIN_ROOM_REQ, kPacket );
//} 

bool KP2P::Send_BillboardChatRegistrationReq(const std::wstring& strMessage, D3DCOLOR color)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_BILLBOARD_REGISTRATION_REQ kPacket;
    kPacket.m_wstrNickName = L"#c" + g_MyD3D->m_TempPlayer.GetStrUserColor() + g_MyD3D->m_TempPlayer.GetStrUserName() + L"#cX";
    kPacket.m_dwColor = color;
    kPacket.m_wstrMessage = strMessage;
    kPacket.m_dwDword2 = 0;
    kPacket.m_dwDword3 = 0;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_BILLBOARD_REGISTRATION_REQ, kPacket);
}

bool KP2P::Send_ChatReq(const wchar_t* szPeerName_, 
                        const wchar_t* szMessage_,
                        KChatData::ENUM_CHAT_TYPE eChatType_,
                        const DWORD dwColor_)
{
    NULL_CHECK( m_pkUserProxy );
    //std::wstring kPacket( szMessage_ );    
    if ( g_pkChatManager->CheckChatBlockUser() )
        return false;

    KChatData kPacket;
    
    if( eChatType_ == KChatData::MSG_TEAM_CHAT 
        || eChatType_ == KChatData::MSG_TEAM_COLOR )
    {
        kPacket.m_cChatType = (char)eChatType_;
        kPacket.m_dwChatColor = dwColor_;
    }
    else if( eChatType_ == KChatData::MSG_GUILD )
    {
        kPacket.m_cChatType = (char)eChatType_;
        kPacket.m_dwChatColor = dwColor_;
    }
    else
    {
        if(USERLEVEL_ADMIN == g_kGlobalValue.m_kUserInfo.eGCUserLevel  )
        {
            kPacket.m_cChatType = KChatData::MSG_ADMIN_COMMON;
            kPacket.m_dwChatColor = dwColor_;
        }else
        {
            if( dwColor_ != 0xFFFFFFFF )
            {
                kPacket.m_cChatType = KChatData::MSG_COLOR_CHAT;
                kPacket.m_dwChatColor = dwColor_;
            }
            else
            {
                kPacket.m_cChatType = KChatData::MSG_COMMON;
                kPacket.m_dwChatColor = dwColor_;
            }
        }
    }
    kPacket.m_strSenderNick = szPeerName_;
    kPacket.m_strChatMsg = szMessage_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHAT_REQ, kPacket );
} 

bool KP2P::Send_LeaveRoomReq( const KEVENT_LEAVE_ROOM_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_LEAVE_ROOM_REQ, kPacket_ );
}

bool KP2P::Send_LeaveGameReq( const KGameResultIn& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_LEAVE_GAME_REQ, kPacket_ );
}

bool KP2P::Send_RoomListReq( const bool bSendDelay, const bool bWait_, const int nType_, const int nDifficult_, const int nPage_ )
{
    int nType = nType_;
    NULL_CHECK( m_pkUserProxy );

    if( bSendDelay ) // 룸 리스트 요청 딜레이를 적용할건가 안할건가..
    {
        DELAY_CHECK( ESPTRoomList, 3 );
    }
    else
    {
        SetTimer( ESPTRoomList );
    }

    //if( GC_GM_QUEST49 == nType )
    //{
    //    nType = GC_GM_QUEST48;
    //}

    if( SiKGCRoomManager()->IsChannelRoomList() )
    {
        KEVENT_CENTER_ROOM_LIST_REQ kPacket;
        kPacket.m_bWaitRoom         = bWait_;
        kPacket.m_nType             = nType;
        kPacket.m_nDifficult        = nDifficult_;
        kPacket.m_nPage             = nPage_;
        if( nType == GC_GMC_GUILD_BATTLE )
        {
            kPacket.m_iGuildID = g_kGlobalValue.m_kUserInfo.iGuildID;
        }
        else
        {
            kPacket.m_iGuildID = -1;
        }
        kPacket.m_nRoomNumPerPage   = 6;
        return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CENTER_ROOM_LIST_REQ, kPacket );

    }
    else
    {
        KEVENT_ROOM_LIST_REQ kPacket;
        kPacket.m_bWaitRoom     = bWait_;
        kPacket.m_nType         = nType;
        kPacket.m_nDifficult    = nDifficult_;
        kPacket.m_nPage         = nPage_;
        return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ROOM_LIST_REQ, kPacket );
    }
}

bool KP2P::Send_UserListReq(const int nPage_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USER_LIST_REQ, nPage_ );
}

bool KP2P::Send_FriendInviteListReq()
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_INVITE_FRIEND_LIST_REQ);
}


bool KP2P::Send_StartGameReq( const std::vector< std::pair<char,char> >& vecPromotionCount_, const int iGameMode_ /* = 0*/ , const char cSelChar_ /* = 0 */, int iMapID, int nStartStage /*= -1 */ )
{
    KStartGameReq kPacket;
    kPacket.m_bGuild = SiKGCRoomManager()->IsChannelRoomList();
    kPacket.m_vecPromotionCount = vecPromotionCount_;
    kPacket.m_nMapID = iMapID;
    kPacket.m_nStartStage = nStartStage;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_START_GAME_REQ, kPacket );
}


bool KP2P::Send_EndGameReq(const char* szPeerName_, 
                           KEndGameReq& kReq_)
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_END_GAME_REQ, kReq_ );
}


//050601. microcat. 패킷이 존재하지 않음.
bool KP2P::Send_ChangeHostNot( const std::vector<DWORD>& vecHostIP_, const std::vector<USHORT>& vecHostPort_ )
{
    KEVENT_CHANGE_HOST_ADDRESS_NOT kPacket;

    kPacket.m_vecHostIP     = vecHostIP_;
    kPacket.m_vecHostPort   = vecHostPort_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_HOST_ADDRESS_NOT, kPacket );
}

bool KP2P::Send_ChannelInfoListReq( const bool bDelayCheck )
{
    NULL_CHECK( m_pkUserProxy );
    if( bDelayCheck )
    {
        DELAY_CHECK( ESPTChannelList, 30 );
    }
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CHANNEL_LIST_REQ );
}

bool KP2P::Send_EnterChannelReq(const wchar_t* szPeerName,  DWORD dwChannelUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ENTER_CHANNEL_REQ, dwChannelUID_ );
}

bool KP2P::Send_LeaveChannelNot(const char* szPeerName_)
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_LEAVE_CHANNEL_NOT );
}

bool KP2P::Send_LoadCompleteNot( const int iP2PUser_, const int iRelayUser_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_LOAD_COMPLETE_NOT kPacket;
    kPacket.m_iP2PUser      = iP2PUser_;
    kPacket.m_iRelayUser    = iRelayUser_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_LOAD_COMPLETE_NOT, kPacket );
}

bool KP2P::Send_StageLoadCompleteNot()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_STAGE_LOAD_COMPLETE_NOT );
}

bool KP2P::Send_NotifyMessageNot(const wchar_t* szPeerName_, const std::wstring& strMessage_)
{
    NULL_CHECK( m_pkUserProxy );
    std::wstring kPacket( strMessage_ );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_NOTIFY_REQ, kPacket );
}

bool KP2P::Send_EquipItemReq( const KEquipUser& kReq_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EQUIP_ITEM_REQ, kReq_ );
}


bool KP2P::Send_BuyItemForGPReq( IN DWORD dwGoodsID_, IN int iDuration_, IN int iPeriod_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BUY_FOR_GP_REQ kPacket;

    kPacket.m_ItemID        = dwGoodsID_;
    kPacket.m_nDuration     = iDuration_;
    kPacket.m_nPeriod       = iPeriod_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_GP_REQ, kPacket );
}

// godsend GoodsID추가  [6/29/2004]
bool KP2P::Send_DropItemReq(const char* szPeerName_, GCITEMUID dwGoodsUID_, DWORD dwGoodsID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KSimpleItem kPacket;
    kPacket.m_dwID     = dwGoodsID_;
    kPacket.m_dwUID    =  dwGoodsUID_ ;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SELL_ITEM_REQ, kPacket );
}
// godsend GoodsID추가 [6/29/2004]

bool KP2P::Send_InviteReq( std::vector<DWORD>& kReq_)
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_INVITE_REQ, kReq_ );
}

bool KP2P::Send_ChangeRoomInfoReq( std::vector<std::pair<char,bool> >&  vecChangeSlot_ )
{
    NULL_CHECK( m_pkUserProxy );
    KChangeRoomInfo kInfo;
    kInfo.m_nOK = KChangeRoomInfo::CR_USER_INFO;
    kInfo.m_vecChangeSlot = vecChangeSlot_;
    kInfo.m_bGuild = SiKGCRoomManager()->IsChannelRoomList();
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_ROOM_INFO_REQ, kInfo );
}
bool KP2P::Send_ChangeRoomInfoReq( IN SRoomInfo *pkRoomInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    KChangeRoomInfo kInfo;
    pkRoomInfo_->SetChangeRoomInfo( &kInfo );
    kInfo.m_nOK = KChangeRoomInfo::CR_GAME_INFO;
    kInfo.m_bGuild = SiKGCRoomManager()->IsChannelRoomList();
    START_LOG( cout, "Send_ChangeRoomInfoReq" );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_ROOM_INFO_REQ, kInfo );
}

bool KP2P::Send_EnterGuildRoomReq(const char* szPeerName_)
{
    NULL_CHECK( m_pkUserProxy );
    //return m_pkUserProxy->SendID( KUserEvent::EVENT_ENTER_GUILD_ZONE_REQ );
    return false;
}


bool KP2P::Send_LeaveGuildRoomNot(const char* szPeerName_)
{
    NULL_CHECK( m_pkUserProxy );
    //return m_pkUserProxy->SendID( KUserEvent::EVENT_LEAVE_GUILD_ZONE_NOT );
    return false;
}


bool KP2P::Send_GuildChatReq(const wchar_t* szPeerName_, 
                             const std::wstring& strMessage_)
{
    NULL_CHECK( m_pkUserProxy );

    if ( g_pkChatManager->CheckChatBlockUser() )
        return false;

    std::wstring kPacket = strMessage_;
    if( kPacket.size() > MAX_PATH )
        kPacket.resize( MAX_PATH );

    //return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GUILD_CHAT_REQ, kPacket );
    return false;
}


bool KP2P::Send_GuildUserListReq(const char* szPeerName_)
{
    NULL_CHECK( m_pkUserProxy );
    //return m_pkUserProxy->SendID( KUserEvent::EVENT_GUILD_ZONE_USER_LIST_REQ );
    return false;
}


bool KP2P::Send_KickUserReq(const wchar_t* szPeerName_, 
                            const std::wstring& strLogin_)
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strLogin_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_KICK_USER_BY_ADMIN_REQ, kPacket );
}


bool KP2P::Send_ChangeUserDBInfoReq(const char* szPeerName_, KUserInfoForOperator& kUserDBInfo_)
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_USER_INFO_REQ, kUserDBInfo_ );
}


bool KP2P::Send_InventoryInfoReq(const char* szPeerName_)
{
    //전체 데이터를 디비로 받아오기 전에 창고 아이템을 먼저 디비에 쓰고 다시 받음.
    Result_DepotUpdateItemReq = INT_MAX;
    Send_DepotDBUpdateReq();
    g_MyD3D->WaitForServerAck( Result_DepotUpdateItemReq, INT_MAX, 5000, TIME_OUT_VALUE );


    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_QUERY_INVENTORY_INFO_REQ );
}


bool KP2P::Send_UserDBInfoReq(const wchar_t* szPeerName_, const std::wstring& strLogin_, bool bIsNickname)
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strLogin_;
    kPacket.m_bIsNickname   = bIsNickname;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_USER_INFO2_REQ, kPacket );
}


bool KP2P::Send_DropUserDBInfoReq(const wchar_t* szPeerName_, const std::wstring& strLogin_)
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strLogin_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REMOVE_USER_INFO_REQ, kPacket );
}

//050601. microcat. 패킷이 존재하지 않음
bool KP2P::Send_GuildNotifyMsgReq(const wchar_t* szPeerName_, const std::wstring& strMsg_)
{
    NULL_CHECK( m_pkUserProxy );
    std::wstring kPacket = strMsg_;
    //return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GUILD_NOTIFY_MSG_REQ, kPacket );
    return false;
}


bool KP2P::Send_GuildEditNoticeReq(const char* szPeerName_,
                                   KGuildNoticeInfo& kNotice_)
{
    NULL_CHECK( m_pkUserProxy );
    //todo:woosh 일단 빌드성공을 위해 주석처리 합니다.
    //KEVENT_GUILD_EDIT_NOTICE_REQ kPacket;
    //kPacket.m_kNotice       = kNotice_;

    //return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GUILD_EDIT_NOTICE_REQ, kPacket );
    return true;
}

// godsend add send_joinroomwithguilduseridreq [6/4/2004]
//bool KP2P::Send_JoinRoomWithGuildUserIDReq( const wchar_t* szMessage_, 
//                                            const std::vector<DWORD>& vecIP_,
//                                            const std::vector<USHORT>& vecPort_ )
//{
//    KJoinRoomReqInfo kPacket;
//    kPacket.m_strRoomPasswd = szMessage_;    // 방 비밀번호 란에 유저의 Login을 넣어서 보낸다.
//    kPacket.m_vecIP         = vecIP_;
//    kPacket.m_vecPort       = vecPort_;
//
//    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JOIN_ROOM_WITH_GUILD_USER_ID_REQ, kPacket );
//}

bool KP2P::Send_UseDurationItemReq( const std::vector<GCITEMUID>& vecDurationUids_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_USE_DURATION_ITEM_REQ kPacket;
    kPacket.m_vecDurationUIDs     = vecDurationUids_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_DURATION_ITEM_REQ, kPacket );
}

//050601. microcat. 패킷이 없음
bool KP2P::Send_UpdateTeamPointReq(const char* szPeerName, std::vector<std::string> vecUserName )
{
    return true;
}

//050601. microcat. 패킷이 없음
// godsend [9/9/2004]추석 이벤트 송편 5개를 모으면 한복을 주는 이벤트를 위한 패킷 
bool KP2P::Send_ChuseokEventItemReq( const char* szPeerName, DWORD dwGoodsID )
{
    return true;
}

// godsend 강화패킷요청 [10/1/2004]
bool KP2P::Send_StrongLevelUpReq( IN GCITEMID itemID_, IN GCITEMUID itemUID_, IN GCITEMID assistItemID_, IN GCITEMID materialItemID_/*=0*/ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_STRONG_LEVELUP_REQ kPacket;
    kPacket.m_dwGoodsID             = itemID_ * 10;
    kPacket.m_dwGoodsUID            = itemUID_;
    kPacket.m_dwWeakAssistItemID    = assistItemID_ * 10;
    kPacket.m_dwMaterialItemID      = (materialItemID_ > 0 ? materialItemID_ * 10 : KEVENT_STRONG_LEVELUP_REQ::MI_GEM); // 강화재료ItemID (기본은 Gem)

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_STRONG_LEVELUP_REQ, kPacket );
}

bool KP2P::Send_BuyItemForGemReq( IN GCITEMID itemID_, IN int iDuration_, IN int iPeriod_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BUY_FOR_GEM_REQ kPacket;
    kPacket.m_ItemID        = itemID_;
    kPacket.m_nDuration     = iDuration_;
    kPacket.m_nPeriod       = iPeriod_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_GEM_REQ, kPacket );
}

//050601. microcat. 패킷이 없음
bool KP2P::Send_RevivalBreakItemReq( const char* szPeerName_, DWORD dwGoodsID_, GCITEMUID dwGoodsUID_ )
{
    return true;
}

bool KP2P::Send_BuyItemForCrystalReq( GCITEMID itemID_, int iDuration_, int iPeriod_, int iMoneyType_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BUY_FOR_GEM_REQ kPacket;
    kPacket.m_ItemID = itemID_;
    kPacket.m_nDuration = iDuration_;
    kPacket.m_nPeriod = iPeriod_;
    kPacket.m_nMoneyType = iMoneyType_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_CRYSTAL_REQ, kPacket );
}

//godsend 운영자 툴 서버(DB)에서 일반 아이템 목록 다 가져오기 
bool KP2P::Send_SelectNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_NORMAL_ITEM_LIST_REQ, kPacket );
}

//수량성 
bool KP2P::Send_SelectDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_DURATION_ITEM_LIST_REQ, kPacket );
}

bool KP2P::Send_SelectDelNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, kPacket );
}

bool KP2P::Send_SelectDelDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_ )
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, kPacket );
}

bool KP2P::Send_InsertGPNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_NORMAL_ITEM_GP_REQ , kPacket );
}

bool KP2P::Send_InsertGPDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_DURATION_ITEM_GP_REQ, kPacket );
}

bool KP2P::Send_InsertCashNormalItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_NORMAL_ITEM_CASH_REQ, kPacket );
}

bool KP2P::Send_InsertCashDurationItemReq( const wchar_t* szPeerName_, std::wstring strName_, DWORD dwGoodsID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KAddItemInfo kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_dwGoodsID     = dwGoodsID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_DURATION_ITEM_CASH_REQ, kPacket );
}

bool KP2P::Send_DelItemReq( GCITEMUID dwGoodsUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_REMOVE_ITEM_REQ kPacket;

    kPacket.m_dwGoodsUID    = dwGoodsUID_;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_REMOVE_ITEM_REQ, kPacket);
}

bool KP2P::Send_SelectUserDataReq( const wchar_t* szPeerName_, std::wstring strName_ ) 
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_QUERY_USER_INFO_REQ, kPacket );
}

bool KP2P::Send_SelectChangeStrongItemListReq( const wchar_t* szPeerName_, std::wstring strName_ ) 
{
    NULL_CHECK( m_pkUserProxy );
    KPacket2Name kPacket;
    kPacket.m_strName       = strName_;
    kPacket.m_bIsNickname   = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_STRONG_ITEM_REQ, kPacket );
}

bool KP2P::Send_SelectAdminUseItemInfoDataReq( const char* szPeerName_ ) 
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_QUERY_ITEM_INFO_REQ );
}

//simon [2005-03-29] 정보 공개 허락한다는 패킷 보내는 함수.
bool KP2P::Send_AgreePrivateInfoNot(const char* szPeerName_)
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_AGREEMENT_PRIVATEINFO_NOT );
}

//050601. microcat. 패킷이 없음
//simon [2005-04-25] 어린이날 풍선 터뜨렸을 때 패킷 날리는 함수.
bool KP2P::Send_OpenKidDayGiftReq( const char* szPeerName_ )
{
    return true;
}

bool KP2P::Send_P2PSocketErrorNot( IN const std::wstring& strName_ )
{
    NULL_CHECK( m_pkUserProxy );
    std::wstring kPacket = strName_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_UDP_P2P_SOCKET_ERR_NOT, kPacket );
}

bool KP2P::Send_BanUserReq( IN const std::wstring& strTargetUserName_, IN int nReasonWhy_ )
{
    NULL_CHECK( m_pkUserProxy );
    KPacketNameOK kPacket;
    kPacket.m_strName   = strTargetUserName_;
    kPacket.m_nOK       = nReasonWhy_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BAN_USER_REQ, kPacket );
}

//bool KP2P::Send_JoinCompeleteReq() // 060711. kkurrung.
//{
//    return m_pkUserProxy->SendID( KUserEvent::EVENT_JOIN_COMPLETE_REQ );
//}

bool KP2P::Send_RegisterNickNameReq( const std::wstring& strNickName )
{
    NULL_CHECK( m_pkUserProxy );
    std::wstring kPacket = strNickName;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REGISTER_NICKNAME_REQ, kPacket );
}

bool KP2P::Send_ClientCrashedNot()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CLIENT_CRASHED_NOT );
}

bool KP2P::Send_DetailUserInfoReq( IN const std::wstring& strLogin_ )
{
    NULL_CHECK( m_pkUserProxy );
    std::wstring kPacket = strLogin_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DETAIL_USER_INFO_REQ, kPacket );
}

bool KP2P::Send_ClientError( IN const int iErrorType_, IN const int iErrorID_, IN const std::wstring strErrComment/* = L""*/ , IN DWORD dwReserve1 /*= 0*/, IN DWORD dwReserve2 /*= 0*/, IN DWORD dwReserve3 /*= 0*/ )
{
    NULL_CHECK( m_pkUserProxy );
    if( !IsConnectedToGameServer() )
    {
        return false;
    }

    KEventErr kPacket;
    kPacket.m_nErrType  = iErrorType_;
    kPacket.m_nErrID    = iErrorID_;
    kPacket.m_strComment = strErrComment;
    kPacket.m_dwReserve1 = dwReserve1;
    kPacket.m_dwReserve2 = dwReserve2;
    kPacket.m_dwReserve3 = dwReserve3;
    // 2007/5/11. daeuk. 압축
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CLIENT_ERR_REPORT_NOT, kPacket, true, true );
}

bool KP2P::Send_ClientErrorCount( const std::map< std::pair<int,int>, int >& mapErrorCount )
{
    NULL_CHECK( m_pkUserProxy );
    if( !IsConnectedToGameServer() )
    {
        return false;
    }

    KEventErrCount kPacket = mapErrorCount;


    // 2007/5/11. daeuk. 압축
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CLIENT_ERR_REPORT_COUNT_NOT, kPacket, true, true );
}

bool KP2P::Send_ChangeNickName( IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, IN const wchar_t* szNewNickName_, IN bool bUserItem_/*=true*/ , IN char cCharType_ /*= -1*/)
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_USE_CHANGE_NICKNAME_REQ req;
    req.m_ItemID = itemID_;
    req.m_ItemUID = itemUID_;
    req.m_strNewNickName = szNewNickName_;
    req.m_bUseItem = bUserItem_; // 닉네임변경카드를 사용하는지 여부 (false면 아이템없이 변경됨)

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_CHANGE_NICKNAME_REQ, req );
}

bool KP2P::Send_ChangeNickColor(IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, IN const wchar_t* szNewNickColor_, IN bool bUserItem_/*=true*/, IN char cCharType_ /*= -1*/)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_USE_CHANGE_NICKCOLOR_REQ req;
    req.m_ItemID = itemID_;
    req.m_ItemUID = itemUID_;
    req.m_strNewNickColor = szNewNickColor_;
    req.m_bUseItem = bUserItem_;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_USE_CHANGE_NICKCOLOR_REQ, req);
}

bool KP2P::Send_ChangePetNickName( IN const KItem& NickNameItem, IN const DWORD dwPetID, IN const GCITEMUID dwPetUID, IN const wchar_t* szNewNickName )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CHANGE_PET_NAME_REQ kReq;
    kReq.m_kNickNameItem = NickNameItem;
    kReq.m_kNickNameItem.m_ItemID   = NickNameItem.m_ItemID * 10;
    kReq.m_kNickNameItem.m_ItemUID  = NickNameItem.m_ItemUID ;

    kReq.m_dwPetID = dwPetID * 10;
    kReq.m_dwPetUID =  dwPetUID ;
    kReq.m_strPetNickName = szNewNickName;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_PET_NAME_REQ, kReq );
}

bool KP2P::Send_RegistMission( IN const GCITEMID itemID_, IN const GCITEMUID itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_REGIST_MISSION_REQ req;
    req.m_ItemID     = itemID_;
    req.m_ItemUID    = itemUID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REGIST_MISSION_REQ, req );
}

bool KP2P::Send_RemoveMission( IN const DWORD dwMissionID )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_REMOVE_MISSION req;
    req.m_dwMissionID = dwMissionID;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REMOVE_MISSION_REQ, req );
}
bool KP2P::Send_MissionComplete( IN const DWORD dwMissionID , IN const GCITEMID dwSelection )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_COMPLETE_MISSION_REQ req;
    req.m_dwMissionID = dwMissionID; 
    req.m_dwSelection = dwSelection;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_COMPLETE_MISSION_REQ, req );
}

bool KP2P::Send_ChangeItemReq( IN const GCITEMUID itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_KAIRO_REQ, itemUID_ );
}

bool KP2P::Send_ChangeItemOneShotReq(IN const int ItemCount_, IN const GCITEMUID itemUID_)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_KAIRO_ONESHOT_REQ req;
    req.m_iItemUID = itemUID_;
    req.m_iItemDuration = ItemCount_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_KAIRO_ONESHOT_REQ, req);
}

bool KP2P::Send_CurrentCashPoint()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CURRENT_CASH_POINT_REQ );
}

bool KP2P::Send_BuyForCashItem( IN const DWORD dwGoodsID, IN const int nCount_, int nPeriod_, DWORD dwVirtualCash, bool bPresent, 
                               std::wstring strOwnName, std::wstring strComment, IN const DWORD m_dwEventUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BUY_FOR_CASH_REQ req;
    req.m_kBuyInfo.m_ItemID = dwGoodsID;
    req.m_kBuyInfo.m_nCount = nCount_;
    req.m_kBuyInfo.m_nPeriod= nPeriod_;
    req.m_bPresent          = bPresent;

    req.m_strOwnerNickName  = strOwnName;
    req.m_strComment        = strComment;
    req.m_kVirtualCashInfo.m_nOK = 0;
    req.m_kVirtualCashInfo.m_dwCashPoint = g_kGlobalValue.m_dwVirtualCash;
    req.m_kVirtualCashInfo.m_dwDeductCashPoint = dwVirtualCash; // 사용할 가상 캐쉬 포인트를 넣어주세요

    // 소셜 커머스 구매 시 데이터 설정하고 아니면 Pass
    // req.m_dwEventUID = m_dwEventUID_;


//#if defined(ONE_A_DAY)
//	bool bOneADay   =   g_pkUIScene->m_pkOneADay->IsGoodsIDOfOneADayItem( dwGoodsID );
//#else
//    bool bOneADay   =   false;
//#endif
//    req.m_bOneaday	=	bOneADay;
    

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_CASH_REQ, req );
}

bool KP2P::Send_ChangeIndoorUserInfoReq( IN SUserInfo* pkUserInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    KInDoorUserInfo kUserInfo;
    pkUserInfo_->SetIndoorUserInfo( &kUserInfo );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_INDOOR_USERINFO_REQ, kUserInfo, true, true  );
}

bool KP2P::Send_InRoomChangeIndoorUserInfoReq( IN SUserInfo* pkUserInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    KInDoorUserInfo kUserInfo;
    pkUserInfo_->SetIndoorUserInfo( &kUserInfo );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_IN_ROOM_CHANGE_INDOOR_USERINFO_REQ, kUserInfo, true, true );
}

bool KP2P::Send_ChangeRoomUserInfoReq( IN SUserInfo& kUserInfo_, 
                                       IN const KChangeRoomUserInfo::ROOM_USER_INFO eRoomUserInfo_, int iPlayerIndex_ )
{
    NULL_CHECK( m_pkUserProxy );
    KChangeRoomUserInfo kRoomUserInfo;
    kRoomUserInfo.m_bGuild = SiKGCRoomManager()->IsChannelRoomList();
    kUserInfo_.SetChangeRoomUserInfo( &kRoomUserInfo, eRoomUserInfo_ );
    kRoomUserInfo.m_nUserSlot = iPlayerIndex_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_ROOMUSER_INFO_REQ, kRoomUserInfo );
}

bool KP2P::Send_ChangeRoomUserInfoReq( IN const KChangeRoomUserInfo& kUserInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    KChangeRoomUserInfo kPacket = kUserInfo_;
    kPacket.m_bGuild = SiKGCRoomManager()->IsChannelRoomList();
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_ROOMUSER_INFO_REQ, kPacket );
}

bool KP2P::Send_Into_Myinfo(IN DWORD dwUID, IN std::wstring strLogin_, IN int nState_)
{
    NULL_CHECK(m_pkUserProxy);
    KChangeRoomUserInfo kRoomUserInfo;
    kRoomUserInfo.m_dwUserUID = dwUID;
    kRoomUserInfo.m_ucChangeType = (UCHAR)KChangeRoomUserInfo::RUI_INFO_STATE;
    kRoomUserInfo.m_strLogin = strLogin_;
    kRoomUserInfo.m_nState = nState_;
    kRoomUserInfo.m_bGuild = SiKGCRoomManager()->IsChannelRoomList();

    if (nState_ == GC_RUS_MISSION || nState_ == GC_RUS_SKILL || nState_ == GC_RUS_MYINFO || nState_ == GC_RUS_COUPLEVIEWER || nState_ == GC_RUS_CHANGECHARACTER)
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()];
        pPlayer->m_kPressureState.m_dwElapsedTime = ::timeGetTime();

        Send_SetMyPressState(KPressureState::PS_GAUGE);
    }
    else
    {
        Send_SetMyPressState(KPressureState::PS_NORMAL);
    }

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_CHANGE_ROOMUSER_INFO_REQ, kRoomUserInfo);
}

bool KP2P::Send_RecommendUserReq( IN int iOK_, IN std::wstring strNickName_ )
{
    NULL_CHECK( m_pkUserProxy );
    KPacketNameOK kPacket;
    
    kPacket.m_nOK = iOK_;
    kPacket.m_strName = strNickName_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RECOMMENDER_REQ, kPacket );
}

bool KP2P::Send_EndResultNot()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_END_RESULT_NOT );
}

bool KP2P::Send_CheckRoomUserlistNot( IN std::vector<DWORD>& vecPeerUID_, IN std::vector<DWORD>& vecPlayerUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CHECK_ROOM_USERLIST_NOT kPacket;
    kPacket.m_vecP2PUser = vecPeerUID_;
    kPacket.m_vecSlotUser = vecPlayerUID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHECK_ROOM_USERLIST_NOT, kPacket );
}

//펫 관련
bool KP2P::Send_CreatePetReq( IN const char cCharType, IN const DWORD dwPetID, IN const GCITEMUID dwPetUID, IN const std::wstring strPetName )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CREATE_PET_REQ kPacket;

    kPacket.m_cCharType = cCharType;
    kPacket.m_dwPetID = dwPetID * 10;
    kPacket.m_dwPetUID =  dwPetUID ;
    kPacket.m_strPetName = strPetName;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CREATE_PET_REQ, kPacket );
}

bool KP2P::Send_SetCurrentCharacter( IN char cCharacter_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SET_CURRENT_CHARACTER_REQ, cCharacter_ );
}

bool KP2P::Send_DelPetReq( IN const GCITEMUID dwPetUID_, IN const char cCharType_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_DELETE_PET_REQ kPacket;
    kPacket.m_PetItemUID = dwPetUID_;
    kPacket.m_cCharType = cCharType_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DELETE_PET_REQ, kPacket );
}

//bool KP2P::Send_PetEquipReq( IN const DWORD dwPetID_, IN const DWORD dwPetUID_, IN const std::vector<KSimpleItem>& vecPetEquipItem_ )
//{
//    KEVENT_PET_EQUIP_REQ kPacket;
//    kPacket.m_dwPetID = dwPetID_;
//    kPacket.m_dwPetUID =  dwPetUID_ ;
//    kPacket.m_vecEquipItem = vecPetEquipItem_;
//    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PET_EQUIP_REQ, kPacket );
//}

bool KP2P::Send_FeedingPetReq( IN const GCITEMUID dwPetUID_, IN const GCITEMUID itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_FEEDING_PET_REQ kPacket;
    kPacket.m_dwPetUID      = dwPetUID_ ;
    kPacket.m_feedItemUID   = itemUID_ ;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_FEEDING_PET_REQ, kPacket );

}

int KP2P::ConnectToGameServer( IN const KServerInfo& kServerInfo_, 
                               IN KEVENT_VERIFY_ACCOUNT_REQ& kReq_,
                               IN DWORD dwTimeOut_ /* = 5000  */)
{
    int nRet = -99;

    // 서버 접속타입(초기접속/서버이동) 기본 값으로 최초 접속으로 셋팅한다.
    kReq_.m_nConnectType = KEVENT_VERIFY_ACCOUNT_REQ::ECT_FIRST_CONNECTION;

    if( m_pkUserProxy != NULL && m_pkUserProxy->IsConnected() )
    {
        // 서버 마이그레이션 완료 패킷을 받을때까지 대기 ( 최대 10초 ) 
        if ( !m_pkUserProxy->RequestServerMigration( kServerInfo_, 60000 ) ) 
        {
            return false;
        }

        //이전 서버가 존재 한다면 이동으로 셋팅한다.
        kReq_.m_nConnectType = KEVENT_VERIFY_ACCOUNT_REQ::ECT_SERVER_MIGRATION;
        
    }
#if defined( RETRY_LOGIN_ON_CLOSE_CONNECTION )
    if( g_kGlobalValue.m_bServerReconnection )
    {
        kReq_.m_nConnectType = KEVENT_VERIFY_ACCOUNT_REQ::ECT_SERVER_RECONNECT;        
    }
#endif

    kReq_.m_nLanguageCode = SiKGCMultipleLanguages()->GetLanguageTypeNumber();

    kReq_.m_dwChannellingType = g_kGlobalValue.m_dwChannelType;

#if defined(NATION_USA)
    kReq_.m_nUniqueKey = g_kGlobalValue.m_nUniqueKey; // 유저의 고유키값 넘기는 부분 추가.
#endif

#if defined(NATION_EU)
    kReq_.m_biUniqueKey = g_kGlobalValue.m_biUniqueKey; // 유저의 고유키값 넘기는 부분 추가.
#endif
    KUserProxy* pkUserProxy = new KUserProxy;

    pkUserProxy->SetClassID( USERPROXY_ID );
    pkUserProxy->Init( false );
    pkUserProxy->SetHwnd( m_hWnd );

    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = KUserEvent::EVENT_VERIFY_ACCOUNT_REQ;
    spEvent->Serialize( kReq_ );

    if( (nRet = pkUserProxy->ConnectNAuth( kServerInfo_, spEvent, dwTimeOut_ )) < 0 )
    {
        START_LOG( cerr, "connecting to game server failed." )
            << BUILD_LOG( kServerInfo_.m_strIP )
            << BUILD_LOG( kServerInfo_.m_usPort );
        SAFE_DELETE( pkUserProxy );
        return nRet;
    }

    std::swap( m_pkUserProxy, pkUserProxy );

    SAFE_DELETE( pkUserProxy );

    if( m_spNUserProxy != NULL && m_spNUserProxy->IsConnected() )
    {
        m_spNUserProxy.reset();
    }


    return nRet;
}

#if defined( _OPERATOR_TOOL_ )	// _OPERATOR_TOOL_

int KP2P::ConnectForGMTool( IN const KServerInfo& kServerInfo_, IN const KEVENT_GMTOOL_AUTHENTICATION_REQ& kReq_ )
{
    int nRet = NetError::ERR_UNKNOWN;

    KUserProxy* pkUserProxy = new KUserProxy;

    pkUserProxy->Init( false );
    pkUserProxy->SetHwnd( m_hWnd );

    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = KUserEvent::EVENT_GMTOOL_AUTHENTICATION_REQ;
    spEvent->Serialize( kReq_ );

    if( (nRet = pkUserProxy->ConnectNAuth( kServerInfo_, spEvent, 5000 )) < 0 )
    {
        START_LOG( cerr, "connecting to game server failed." )
            << BUILD_LOG( kServerInfo_.m_strIP )
            << BUILD_LOG( kServerInfo_.m_usPort );
        SAFE_DELETE( pkUserProxy );
        return nRet;
    }

    std::swap( m_pkUserProxy, pkUserProxy );

    SAFE_DELETE( pkUserProxy );

    return nRet;
}

#endif	// end _OPERATOR_TOOL_

// 070314. kkurrung. 유저 강제 선행 플레이(모드 ID를 넣으세요.)
bool KP2P::Send_PreCompleteReq( IN const int nModeID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PRE_COMPLETE_REQ, nModeID_ );
}

// 070314. kkurrung. 인벤 늘리는 아이템
bool KP2P::Send_InventoryExtendReq( IN const GCITEMID itemID_, IN const GCITEMUID itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KExtendItem kPacket;
    kPacket.m_itemID = itemID_ * 10;
    kPacket.m_itemUID =  itemUID_ ;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_INVENTORY_EXTEND_REQ, kPacket );
}

bool KP2P::Send_StatUserHistory( IN std::vector< std::pair<char,unsigned short> >& vecUserHistory_ )
{
    NULL_CHECK( m_pkUserProxy );
    if( m_pkUserProxy == NULL )
        return false;

    //if( !m_pkUserProxy->IsConnected() )
    //	return false;

    KEVENT_STAT_USER_HISTORY_NOT kPacket;
    kPacket.m_vecUserHistory.clear();
    kPacket.m_vecUserHistory.swap( vecUserHistory_ );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_STAT_USER_HISTORY_NOT, kPacket );
}

bool KP2P::Send_ProtectTableChkSum( IN DWORD dwCheckSum )
{
    NULL_CHECK( m_pkUserProxy );
    if( m_pkUserProxy == NULL ) return false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PROTECED_TABBLE_CHECKSUM_NOT, dwCheckSum );
}

bool KP2P::Send_ForceChangeRoomUserInfo()
{
    NULL_CHECK( m_pkUserProxy );
    KForceChangeRoomUserInfo kPacket;

    kPacket.m_ucChangeType = KForceChangeRoomUserInfo::FRUI_READY;
    kPacket.m_bReady = false;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_FORCE_CHANGE_ROOMUSER_INFO_REQ, kPacket );
}

// 2007.04.17 wony
// 진화 가능한 펫인지 펫의 UID를 보내서 체크를 한다
bool KP2P::Send_CheckTransformationPetReq( GCITEMUID dwPetUID, DWORD dwUseItemID, int iPetTransformationType )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CHECK_TRANSFORMATION_PET_REQ kPacket;

    kPacket.m_dwPetUID =  dwPetUID ;
    kPacket.m_itemID = dwUseItemID * 10;
    kPacket.m_ePTT = iPetTransformationType; // 진화(0) / 퇴화(1) 타입.

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHECK_TRANSFORMATION_PET_REQ, kPacket );
}

// 실제로 펫의 진화를 요청하는 패킷
bool KP2P::Send_TransformationPetReq( GCITEMUID dwPetUID, std::wstring strPetName, DWORD dwUseItemID, int iPetTransformationType )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_TRANSFORMATION_PET_REQ kPacket;

    kPacket.m_dwPetUID =  dwPetUID ;
    kPacket.m_strPetName = strPetName;
    kPacket.m_dwItemID = dwUseItemID * 10;
    kPacket.m_ePTT = iPetTransformationType; // 진화(0) / 퇴화(1) 타입.

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_TRANSFORMATION_PET_REQ, kPacket );
}

bool KP2P::Send_GetCalendarReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GET_CALENDAR_REQ );
}

bool KP2P::Send_MonthlyAttendRewardReq( IN const KSimpleDate& kReqData_, IN const KItem& kReqItem_ ) //월개근 보상 아이템 요청
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_MONTHLY_ATTEND_REWARD_REQ kPacket;
    kPacket.m_kReqDate = kReqData_;
    kPacket.m_kReqItem = kReqItem_;
    kPacket.m_kReqItem.m_ItemID *= 10;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MONTHLY_ATTEND_REWARD_REQ, kPacket );
}

bool KP2P::Send_GCPointRecommendReq( std::wstring strNickName_ ) // 친구 추천
{
    NULL_CHECK( m_pkUserProxy );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GCPOINT_RECOMMEND_REQ, strNickName_ );
}

bool KP2P::Send_WeeklyAttendReq( IN const char cWeekNum_, IN const GCITEMID nReqItemID_ /*= -1*/ ) // 주개근 요청
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_WEEKLY_ATTEND_REQ kPacket;
    kPacket.m_cWeekID = cWeekNum_;
    kPacket.m_kReqItem.m_ItemID = nReqItemID_; 

    //선택 보상이 없고 '기본 보상만 있는 경우'에 값을 -1로 설정해주세요.
    if( kPacket.m_kReqItem.m_ItemID  > 0 )
        kPacket.m_kReqItem.m_ItemID *= 10;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_WEEKLY_ATTEND_REQ, kPacket );
}


bool KP2P::Send_SeallingCalendarReq( KSimpleDate kReqData_ ) // 출석 도장 찍기 요청
{
    NULL_CHECK( m_pkUserProxy );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SEALLING_CALENDAR_REQ, kReqData_ );
}

bool KP2P::Send_GetCurrentPointReq() // GC 포인트 정보 요청 
{
    NULL_CHECK( m_pkUserProxy );

    return m_pkUserProxy->SendID( KUserEvent::EVENT_GET_CURRENT_POINT_REQ );
}

bool KP2P::Send_HatchingEggReq( const GCITEMUID dwEggPetUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    GCITEMUID dwEggPetUID =  dwEggPetUID_ ;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_HATCHING_EGG_REQ, dwEggPetUID );
}

bool KP2P::Send_BuyForGCPoint(IN const GCITEMID itemID_, IN int nCount /*= 1*/, IN int nMoneyType_/*=KBuySellItemReq::EMT_GCPOINT*/)
{
    /*�����ϴ� ������ Ÿ��
    KBuySellItemReq::EMT_GCPOINT : �⼮����Ʈ ������(�⺻��)
    KBuySellItemReq::EMT_CASH : ĳ�� ������ (ĳ���������� �� ��� ����)
    */
    NULL_CHECK(m_pkUserProxy);
    KBuySellItemReq kPacket;
    kPacket.m_ItemID = itemID_ * 10;
    kPacket.m_nMoneyType = nMoneyType_; // �����ϴ� ������ Ÿ��
    kPacket.m_nCount = nCount;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_BUY_GCPOINT_ITEM_REQ, kPacket);
}

bool KP2P::Send_SellForGCPoint( IN const GCITEMID itemID_, IN const GCITEMUID itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KBuySellItemReq kPacket;
    kPacket.m_ItemID = itemID_ * 10;
    kPacket.m_ItemUID = itemUID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SELL_GCPOINT_ITEM_REQ, kPacket );
}

bool KP2P::Send_FH_Check_Recommend()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_FH_CHECK_ACCOUNT_TYPE_REQ );
}

bool KP2P::Send_FH_Recommend_Info()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_FH_RECOMMENDEE_INFO_REQ );
}

bool KP2P::Send_FH_Recommend_ItemReq( const char chID )
{
    NULL_CHECK( m_pkUserProxy );
    char kPacket = chID;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_FH_REWARD_ITEM_REQ, kPacket );
}
// 070419. woosh. 강제로 바꾸고자 하는 상태를 넣으세요. (성공하면 넣은 상태로 바뀜)
// STATE_CHANNEL : 6, STATE_CHANNELLOBBY : 5
bool KP2P::Send_ForceTransitionState( IN int nDestState )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_FORCE_TRANSITION_STATE_REQ, nDestState );
}

bool KP2P::Send_Stat_FinishLodingTime( const DWORD dwElapsedTime_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_STAT_LOADING_TIME_NOT, dwElapsedTime_ );
}


bool KP2P::Send_Reset_Win_Lose_Data( IN const GCITEMID itemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KResetWinLoseData kPacket;
    kPacket.m_ItemID = itemID_ * 10;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RESET_WIN_LOSE_REQ, kPacket );
}

// 06/21/2007. daeuk. 방제 변경
bool KP2P::Send_Change_Room_Option_Req( const KRoomOption& kRoomOption_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_ROOM_OPTION_REQ, kRoomOption_ );
}

// 070817. woosh.  신고시스템
bool KP2P::Send_Report_User_Req( const int nReason, const std::vector<DWORD>& vecUser , const std::wstring& strNickName_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_REPORT_USER_REQ kPacket;
    kPacket.m_nReason = nReason;
    kPacket.m_vecUser = vecUser;
    kPacket.m_strUser = strNickName_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REPORT_USER_REQ, kPacket );
}

bool KP2P::Send_Chatting_Event_Correct_Not( const int nEventUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHATTING_EVENT_CORRECT_NOT, nEventUID_ );
}

bool KP2P::Send_ServerListReq()
{
    NULL_CHECK( m_pkUserProxy );
    DELAY_CHECK( ESPTServerList, 70 );

    return m_pkUserProxy->SendID( KUserEvent::EVENT_SERVER_LIST_REQ );
}
bool KP2P::Send_EnchantItemReq( IN const DWORD dwUserUID_, IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, IN const GCITEMID assistItemID_, IN const GCITEMUID assistItemUID_, 
                                IN const GCITEMID stuffItemID_, IN const GCITEMUID stuffItemUID_, IN const DWORD dwStuffItemCount_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_ENCHANT_REQ kPacket;
    kPacket.m_dwLoginUID = dwUserUID_;
    kPacket.m_ItemID = itemID_*10;
    kPacket.m_ItemUID = itemUID_;
    assistItemID_ != 0xFFFFFFFF ? kPacket.m_AssistItemID = assistItemID_*10 : kPacket.m_AssistItemID = 0xFFFFFFFF;
    kPacket.m_AssistItemUID = assistItemUID_;
    kPacket.m_EnchantStuffID = stuffItemID_*10;
    kPacket.m_EnchantStuffUID = stuffItemUID_;
    kPacket.m_uiEnchantStuffCount = dwStuffItemCount_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ENCHANT_REQ, kPacket );
}

bool KP2P::Send_ChangBonusCoinReq( DWORD dwItemId_, GCITEMUID dwItemUID_, int nCount_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CHANGE_BONUS_COIN_REQ kPacket;
    kPacket.m_kItem.m_dwID = dwItemId_ * 10;
    kPacket.m_kItem.m_dwUID =  dwItemUID_ ;
    kPacket.m_ucCount = nCount_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_BONUS_COIN_REQ, kPacket );
}

bool KP2P::Send_UseBonusPoint()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_USE_BONUS_POINT_REQ );
}

bool KP2P::Send_ChangeSlotEquipReq( KQuickSlot& kSlotInfo )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_SLOT_EQUIP_REQ, kSlotInfo );
}

bool KP2P::Send_UseQuickSlotReq( IN const int nSlotType_, IN const int SlotIndex_, IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, IN const int itemKind_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_USE_QUICK_SLOT_REQ kPacket;
    kPacket.m_nSlotType = nSlotType_;
    kPacket.m_kSlotData.m_nSlotIndex = SlotIndex_;
    kPacket.m_kSlotData.m_kItem.m_dwID = itemID_ * 10;
    kPacket.m_kSlotData.m_kItem.m_dwUID =  itemUID_ ;
    kPacket.m_nPositionType = itemKind_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_QUICK_SLOT_REQ, kPacket );
}

bool KP2P::Send_SquareListReq( IN DWORD dwGuildUID_/*=0*/ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SQUARE_LIST_REQ, dwGuildUID_ );
}

bool KP2P::Send_EnterSquareReq( const char cCharType_, const DWORD dwSquareID_, const int iSquareType_, const int iSquareIndex_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_ENTER_SQUARE_REQ kPacket;
    kPacket.m_cSelCharType = cCharType_;
    kPacket.m_dwSquareID   = dwSquareID_;
    kPacket.m_iSquareType = iSquareType_;
    kPacket.m_iSquareIndex = iSquareIndex_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ENTER_SQUARE_REQ, kPacket );
}

bool KP2P::Send_SquareLodingComplete()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SQUARE_LOADING_COMPLETE_REQ );
}

bool KP2P::Send_LeaveSquareReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_LEAVE_SQUARE_REQ );
}

bool KP2P::Send_UpdateSquareMyPos( DWORD dwSquareID_, float fXpos_, float fYpos_ )
{
    NULL_CHECK( m_pkUserProxy );

    KSquarePos kPacket;
    kPacket.m_dwSquareID = dwSquareID_;
    kPacket.m_fXpos = fXpos_;
    kPacket.m_fYpos = fYpos_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_UPDATE_MY_SQUARE_POS, kPacket );
}

bool KP2P::Send_SquareChat( char cChatType_, DWORD ReceiverUID_, std::wstring strMsg_, DWORD dwChatColor_ )
{
    NULL_CHECK( m_pkUserProxy );

    if ( g_pkChatManager->CheckChatBlockUser() )
        return false;

    KChatData kPacket;
    kPacket.m_cChatType = cChatType_;
    kPacket.m_dwReceiverUID = ReceiverUID_;
    kPacket.m_strChatMsg = strMsg_;
    kPacket.m_dwChatColor = dwChatColor_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SQUARE_CHAT_REQ, kPacket );

}

bool KP2P::Send_SyncNeighborhood()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SQUARE_USER_SYNC_REQ );
}

bool KP2P::Send_NewCharacterReq( IN const GCITEMID itemID_, IN const GCITEMUID itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_NEW_CHARACTER_REQ kPacket;
    kPacket.m_ItemID  = itemID_ * 10;
    kPacket.m_ItemUID = itemUID_ ;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_NEW_CHARACTER_REQ, kPacket );

}

bool KP2P::Send_UseEmoticon_Req( const DWORD dwEmoticonItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KUserEmoticon kPacket;
    kPacket.m_dwItemID  = dwEmoticonItemID_ * 10;
    kPacket.m_dwUserUID = 0L;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_EMOTICON_REQ, kPacket );
}

bool KP2P::Send_TWUserAgreementReq( const int nUserAgreement_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_TW_USER_AGREEMENT_REQ, nUserAgreement_ );
}

bool KP2P::Send_SetObserver( const bool bObserver_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_OBSERVER_REQ, bObserver_ );
}

bool KP2P::Send_EmoticonMixReq( const GCITEMID dwItemA_, const GCITEMID dwItemB_, const GCITEMID dwItemC_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_EMOTICON_MIX_REQ kPacket;

    kPacket.m_dwResultItem = 0L;
    kPacket.m_setMixMaterial.insert( dwItemA_ );
    kPacket.m_setMixMaterial.insert( dwItemB_ );
    kPacket.m_setMixMaterial.insert( dwItemC_ );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EMOTICON_MIX_REQ, kPacket );
}


bool KP2P::Send_InvitedRoomFromSquareReq( const DWORD dwChannelUID, const USHORT usRoomID,
                                const DWORD dwHostUID, const std::wstring& strRoomPW, const KInDoorUserInfo& kInfo )
{
    KEVENT_INVITED_ROOM_FROM_SQUARE_REQ kReq;
    kReq.m_dwChannelUID     = dwChannelUID;
    kReq.m_usRoomID         = usRoomID;
    kReq.m_dwHostUID        = dwHostUID;
    kReq.m_strRoomPasswd    = strRoomPW;
    kReq.m_kIndoorUserInfo  = kInfo;

    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_INVITED_ROOM_FROM_SQUARE_REQ, kReq, true, true );
}

bool KP2P::Send_SquareUserListReq( const DWORD dwSquareID, const int nPage )
{
    std::pair<DWORD,int> kPacket;
    kPacket.first = dwSquareID;
    kPacket.second = nPage;
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SQUARE_USER_LIST_REQ, kPacket );
}

bool KP2P::Send_Invite_Deny_Not( bool bInviteDiny_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_INVITE_DENY_NOT, bInviteDiny_ );
}

bool KP2P::Send_GetSignBoardRegNum()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SIGN_BOARD_NUM_REQ );
}

bool KP2P::Send_RegSignBoard( const KSignBoardData& kData_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SIGN_BOARD_REG_REQ, kData_ );
}

bool KP2P::Send_AdminSignBoardNot( const int nType_, const std::wstring& strMsg_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_SIGN_BOARD_NOTIFY kPacket;
    kPacket.m_eType = nType_;
    kPacket.m_strMsg = strMsg_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SIGN_BOARD_NOTIFY, kPacket );

}

bool KP2P::Send_RankPageReq( const char cRankType_, const char cCharType_, const char cPageNum_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_RANK_PAGE_REQ kPacket;
    kPacket.m_cRnkType  = cRankType_;
    kPacket.m_cCharType = cCharType_;
    kPacket.m_cPageNum  = cPageNum_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RANK_PAGE_REQ, kPacket );
}

bool KP2P::Send_RankSearchReq( const char cRankType_, const char cCharType_, const std::wstring& strNick_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_RANK_SEARCH_REQ kPacket;
    kPacket.m_cRankType     = cRankType_;
    kPacket.m_cCharType     = cCharType_;
    kPacket.m_strNickName   = strNick_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RANK_SEARCH_REQ, kPacket );
}

bool KP2P::Send_P2PConnectionCompleteNot()
{
#if defined( __PATH__ )
    return true;
#endif
    NULL_CHECK( m_pkUserProxy );
    //return m_pkUserProxy->SendID( KUserEvent::EVENT_P2P_CONNECTION_COMPLETE_NOT );
    return true;
}

bool KP2P::Send_P2PStandbyListAck( const std::set<DWORD>& setList_ ) // client to server
{
#if defined( __PATH__ )
    return true;
#endif
    NULL_CHECK( m_pkUserProxy );
    std::set<DWORD> kPacket;
    kPacket = setList_;
    //return m_pkUserProxy->SendPacket( KUserEvent::EVENT_P2P_STANDBY_LIST_ACK, kPacket );
    return true;
}

bool KP2P::Send_GetSellPrice( const DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    DWORD kPacket = dwItemID_* 10;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SELL_INFO_REQ, kPacket );
}

bool KP2P::Send_SellCountItemReq( IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, IN const int nSellCount_ )
{
    NULL_CHECK( m_pkUserProxy );
    KBuySellItemReq kPacket;
    kPacket.m_ItemID    = itemID_ * 10;
    kPacket.m_ItemUID   = itemUID_;
    kPacket.m_nCount    = nSellCount_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SELL_COUNT_ITEM_REQ, kPacket );
}

bool KP2P::Send_BuyItemForVirtualCashReq( IN const GCITEMID itemID_, IN const GCITEMUID itemUID_, IN const int nDuration_, IN const int nPeriod_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BUY_VIRTUAL_CASH_REQ kPacket;
    kPacket.m_bPresent = false;
    kPacket.m_strComment = L"";
    kPacket.m_strOwnerNickName = L"";

    KBuySellItemReq kBuySellItemReq;
    kBuySellItemReq.m_ItemID      = itemID_ * 10;
    kBuySellItemReq.m_ItemUID     = itemUID_;
    kBuySellItemReq.m_nCount      = nDuration_;
    kBuySellItemReq.m_nPeriod     = nPeriod_;
    kBuySellItemReq.m_nMoneyType  = KGCItemManager::EMTS_VIRTUALCASH;

    kPacket.m_kBuySellItemReq = kBuySellItemReq;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_VIRTUAL_CASH_REQ, kPacket );
}

bool KP2P::Send_CollectionComplete( const DWORD dwMissionID )
{
    NULL_CHECK( m_pkUserProxy );
    DWORD kPacket;
    kPacket = dwMissionID;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_COLLECTION_COMPLETE_REQ, kPacket );
}
bool KP2P::Send_JoinGameReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_JOIN_GAME_REQ );
}

bool KP2P::Send_P2PUniqueNumberReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_P2P_UNIQUE_NUM_REQ );
}

bool KP2P::Send_UpdateDeathScore( const int nCurrentKill_, const int nCurrentTime_ )
{
    KDeathMatchScore kPacket;
    kPacket.m_nCurrentKill = nCurrentKill_;
    kPacket.m_nCurrentTime = nCurrentTime_;

    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_UPDATE_DEATH_SCORE_NOT, kPacket );
}

bool KP2P::Send_RelayData( const std::vector<DWORD>& vecUsers_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RELAY_DATA, vecUsers_ );
}

bool KP2P::Send_StartMiniGameReq( const char cGameMode_ ) // 선택한 미니게임 모드
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_START_MINIGAME_REQ, cGameMode_ );
}

bool KP2P::Send_EndMiniGameReq( const KMiniGameRankInfo& kData_ ) // 미니게임이 끝났을 때 , 현재 점수를 넣어서 보내주세요
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_END_MINIGAME_REQ, kData_ );
}

bool KP2P::Send_ExitMiniGameReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_EXIT_MINIGAME_REQ );
}

bool KP2P::Send_RestartMiniGame( const char cGameMode_ ) // 미니게임 메뉴에서 미니게임 시작
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MINIGAME_RESTART_REQ, cGameMode_ );
}

bool KP2P::Send_MiniGameTopRank( const char cGameMode_ ) // 미니게임 종류별 top 랭크 요청
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MINIGAME_TOPRANK_REQ, cGameMode_ );
}

bool KP2P::Send_MiniGameMyRank() // 미니게임, 유저 랭킹 요청
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_MINIGAME_MY_RANK_INFO_REQ );
}

bool KP2P::Send_GetFullSkillInfo()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GET_FULL_SP_INFO_REQ );
}

bool KP2P::Send_SkillTrainingReq( const int nSkillID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SKILL_TRAINING_REQ, nSkillID_ );
}

bool KP2P::Send_SettingSkills( const KChangeSkillSet& kChangeSet )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SET_SKILL_REQ, kChangeSet, false, true ); // 사이즈가 크니까 압축
}

bool KP2P::Send_RemoveSkill( const int nSkillID_, const DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_REMOVE_SKILL_REQ kPacket;
    kPacket.m_dwItemID = dwItemID_ * 10;
    kPacket.m_nSkillID = nSkillID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REMOVE_SKILL_REQ, kPacket );
}

bool KP2P::Send_UnLockSkill( const int nLockGroupID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_UNLOCK_SKILL_REQ, nLockGroupID_ );
}

bool KP2P::Send_SkillOpenUseReq( const int nSkillID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SKILL_OPENKEY_USE_REQ, nSkillID_ );
}

bool KP2P::Send_StartJumpEvent()     // 운영자가 보내는 이벤트 시작 패킷
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_RESET_POSITION_REQ );

}

bool KP2P::Send_FinishJumpEvent()    // 운영자가 보내는 이벤트 종료 패킷
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_FINISH_EVENT_REQ );

}

bool KP2P::Send_ArriveGoal( const std::wstring strMsg_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_FINISH_GOAL_NOT, strMsg_ );

}

bool KP2P::Send_ChangeCouponReq( const DWORD dwItemID_, const GCITEMUID dwItemUID_, const char cItemType_, const int iAmmount_ )
{
    NULL_CHECK( m_pkUserProxy );

    KComplexItem kPacket;
    kPacket.m_dwID      = dwItemID_ * 10;
    kPacket.m_dwUID     = dwItemUID_;
    kPacket.m_cItemType = cItemType_;
    kPacket.m_iDuration = iAmmount_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_COUPON_REQ, kPacket );

}

bool KP2P::Send_ChangeVirtualCashCouponReq( const GCITEMID dwItemID_, const GCITEMUID dwItemUID_, const char cItemType_, const int iAmmount_ )
{
    NULL_CHECK( m_pkUserProxy );

    KComplexItem kPacket;
    kPacket.m_dwID      = dwItemID_ * 10;
    kPacket.m_dwUID     = dwItemUID_;
    kPacket.m_cItemType = cItemType_;
    kPacket.m_iDuration = iAmmount_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ, kPacket );

}

bool KP2P::Send_GachaRewardListReq(  IN const int iVer, IN const GCITEMID itemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GACHA_REWARD_LIST_REQ kPacket;
    kPacket.m_nVersion = iVer;
    kPacket.m_dwGachaItemID = itemID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_REWARD_LIST_REQ, kPacket );
}

bool KP2P::Send_GachaActionReq( IN const GCITEMID itemID_, IN const int iLevel_, IN const DWORD dwType_, IN const char cCharType_, IN const int iVer_ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_GACHA_ACTION_REQ kPacket;
    kPacket.m_nVersion = iVer_;
    kPacket.m_ItemID = itemID_ * 10;
    kPacket.m_nLevel = iLevel_;
    kPacket.m_dwType = dwType_;
    kPacket.m_cCharType = cCharType_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_ACTION_REQ, kPacket );
}

bool KP2P::Send_GachaLevelActionReq( IN const GCITEMID itemID_, IN const int iLevel_, IN const DWORD dwType_, IN const char cCharType_, IN const int iVer_ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_GACHA_ACTION_REQ kPacket;
    kPacket.m_nVersion = iVer_;
    kPacket.m_ItemID = itemID_ * 10;
    kPacket.m_nLevel = iLevel_;
    kPacket.m_dwType = dwType_;
    kPacket.m_cCharType = cCharType_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_LEVEL_ACTION_REQ, kPacket );
}

bool KP2P::Send_ObtainedGachaSetRewardList()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GACHA_OBTAINED_SET_REWARD_REQ );
}

bool KP2P::Send_ObtainedGachaLevelSetRewardList()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GACHA_LEVEL_OBTAINED_SET_REWARD_REQ );
}

bool KP2P::Send_GachaSetRewardListReq( IN const int iVer, IN const GCITEMID itemID ) // 구슬 아이디 너으면 됨.
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GACHA_SET_REWARD_LIST_REQ kPacket;
    kPacket.m_nVersion = iVer;
    kPacket.m_dwGachaItemID = itemID ;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_SET_REWARD_LIST_REQ, kPacket);
}

bool KP2P::Send_GachaSetRewardReq( DWORD dwGachaItemID_, DWORD dwType_, int iLevel_, DWORD dwRewardItemID_, char cCharType_, const int iVer_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GACHA_SET_REWARD_REQ kPacket;
    kPacket.m_nVersion = iVer_;
    kPacket.m_gachaItemID = dwGachaItemID_ * 10;
    kPacket.m_dwType = dwType_;
    kPacket.m_nLevel = iLevel_;
    kPacket.m_ItemID = dwRewardItemID_ * 10;
    kPacket.m_cCharType = cCharType_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_SET_REWARD_REQ, kPacket );
}

bool KP2P::Send_GachaLevelSetRewardReq( DWORD dwGachaItemID_, DWORD dwType_, int iLevel_, DWORD dwRewardItemID_, char cCharType_, const int iVer_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GACHA_SET_REWARD_REQ kPacket;
    kPacket.m_nVersion = iVer_;
    kPacket.m_gachaItemID = dwGachaItemID_ * 10;
    kPacket.m_dwType = dwType_;
    kPacket.m_nLevel = iLevel_;
    kPacket.m_ItemID = dwRewardItemID_ * 10;
    kPacket.m_cCharType = cCharType_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_LEVEL_SET_REWARD_REQ, kPacket );
}

bool KP2P::Send_ClientFailedGameStartNot( DWORD nFailedType_, int nSecond_, DWORD dwRoomTotalUser_, 
                                         DWORD dwMapID_,int nFailType_, DWORD dwFailReason_)
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CLIENT_FAILED_GAME_START_NOT	buf;
    buf.m_dwType = nFailedType_;
    buf.m_nSecond = nSecond_;
    buf.m_dwRoomTotalUser = dwRoomTotalUser_;
    buf.m_dwMapID = dwMapID_;
    buf.m_dwFailType = nFailType_;
    buf.m_dwFailReason = dwFailReason_; 

    // m_dwType에 대한 정의
    // Type : 1( 게임시작후 로딩 성공 )
    // Type : 2( 게임시작후 로딩 실패 )
    // Type : 3( Stage이동시 로딩 성공 )
    // Type : 4( Stage이동시 로딩 실패 )
    // Type : 5( 트리거 이상)

    // m_dwFailType
    // Type : 1( 현재 활성화 된 트리거에서 찾을 수 없음 )
    // Type : 2( 스크립트의 퀘스트 컨디션 ID의 갯수가 방장의 갯수랑 틀림)

    // m_dwFailReason
    // Type : 1( 어떤 트리거를 못찾는가??? )
    // Type : 2( 어떤 정보가 틀린가??)

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CLIENT_FAILED_GAME_START_NOT, buf );
}

//EVENT_MAKE_COUPLE_REQ
bool KP2P::Send_MakeCoupleReq( KEVENT_MAKE_COUPLE_REQ& kData_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MAKE_COUPLE_REQ, kData_ );
}

//EVENT_MAKE_COUPLE_ANS
bool KP2P::Send_MakeCoupleAns( bool bAccept_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MAKE_COUPLE_ANS, bAccept_ );
}

//EVENT_USE_COUPLE_ITEM_REQ
bool KP2P::Send_InsertCoupleInven( DWORD dwItemID_, GCITEMUID dwItemUID_, bool bDirectEquip_ )
{
    NULL_CHECK( m_pkUserProxy );
    KCoupleItemData kPacket;

    kPacket.m_kItem.m_dwID  = dwItemID_ * 10;
    kPacket.m_kItem.m_dwUID =  dwItemUID_ ;
    kPacket.m_bEquip = bDirectEquip_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_COUPLE_ITEM_REQ, kPacket );
}

//EVENT_EQUIP_COUPLE_ITEM_REQ
bool KP2P::Send_ChangeCoupleEquip( std::vector<DWORD>& vecEquip_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EQUIP_COUPLE_ITEM_REQ, vecEquip_ );
}

//EVENT_COUPLE_INFORMATION_REQ
bool KP2P::Send_CoupleInfoReq( DWORD dwUserUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_COUPLE_INFORMATION_REQ, dwUserUID_ );
}

//EVENT_SET_COUPLE_REQ_DENY
bool KP2P::Send_MakeCoupleReqDeny( DWORD dwUserUID_ ) // 0 이면 전체 거절
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SET_COUPLE_REQ_DENY, dwUserUID_ );
}

//EVENT_CHANGE_COUPLE_RING_REQ
bool KP2P::Send_ChangeCoupleRingReq( DWORD dwItemID_, GCITEMUID dwItemUID_, std::wstring& strMsg_, bool bInUserInven_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CHANGE_COUPLE_RING_REQ kPacket;

    kPacket.m_bInven = bInUserInven_;
    kPacket.m_kItem.m_dwID  = dwItemID_ * 10;
    kPacket.m_kItem.m_dwUID =  dwItemUID_ ;
    kPacket.m_strMsg = strMsg_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_COUPLE_RING_REQ, kPacket );
}

//EVENT_CHANGE_COUPLE_RING_ANS
bool KP2P::Send_ChangeCoupleRingAns( bool bAccept_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_COUPLE_RING_ANS, bAccept_ );
}

//EVENT_DIVORCE_COUPLE_REQ
bool KP2P::Send_DivorceCoupleReq( std::wstring& strMsg_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DIVORCE_COUPLE_REQ, strMsg_ );
}
//EVENT_CHANGE_COUPLE_CHAR_REQ
bool KP2P::Send_ChangeCoupleCharType( const char cCharType_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_COUPLE_CHAR_REQ, cCharType_ );
}

//EVENT_COUPLE_CHAT_REQ
bool KP2P::Send_CoupleChatReq( const std::wstring strMsg_, const DWORD dwColor_ )
{
    NULL_CHECK( m_pkUserProxy );

    if ( g_pkChatManager->CheckChatBlockUser() )
        return false;

    KChatData kPacket;

    kPacket.m_cChatType         = KChatData::MSG_COUPLE_CHAT;
    kPacket.m_dwSenderUID       = 0;
    kPacket.m_dwReceiverUID     = 0;
    kPacket.m_dwChatColor       = dwColor_;
    kPacket.m_strChatMsg        = strMsg_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_COUPLE_CHAT_REQ, kPacket );
}

//EVENT_COUPLE_WINDOW_STATE_REQ
bool KP2P::Send_CoupleWindowStateReq( const int nState_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_COUPLE_WINDOW_STATE_REQ, nState_ );
}

bool KP2P::Send_PetCostumeListReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_PET_COSTUM_LIST_REQ );
}

//EVENT_GET_USER_DONATION_INFO_REQ
bool KP2P::Send_MyDonationInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GET_USER_DONATION_INFO_REQ );

}

//EVENT_DONATION_INFO_REQ
bool KP2P::Send_DonationInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_DONATION_INFO_REQ );

}

//EVENT_DONATION_GP_REQ
bool KP2P::Send_DonationPointReq( const int nDonationPoint_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_DONATION_POINT_REQ kPacket;
    kPacket.m_nDonationPoint = nDonationPoint_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DONATION_POINT_REQ, kPacket );

}
//EVENT_BUY_FOR_VIP_REQ
bool KP2P::Send_BuyForVIPReq( const int nCharType_, const DWORD dwItemID_, const int nDuration_, const int nPeriod_, const int nVipCount_ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_BUY_FOR_VIP_REQ kPacket;
    kPacket.m_kBuyItem.m_ItemID = dwItemID_ * 10;
    kPacket.m_kBuyItem.m_nDuration = nDuration_;
    kPacket.m_kBuyItem.m_nPeriod = nPeriod_;
    kPacket.m_nItemPrice = nVipCount_;       // 해당Item의 가격(=필요한VIP뱃지수);
    kPacket.m_nCharType = nCharType_;       // ex) 공용:-1, 엘리시스:0, 리르:1,...에이미:6

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_VIP_REQ, kPacket );

}

bool KP2P::Send_AddExp( const char cCharType_, const __int64 biAddExp_ )
{
    NULL_CHECK( m_pkUserProxy );
    KAddExpInfo kPacket;
    kPacket.m_cCharType = cCharType_;
    kPacket.m_biExp = biAddExp_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_EXP_REQ, kPacket );
}

bool KP2P::Send_LunaNewYearReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_LUNA_NEWYEAR_REQ );
}

//bool KP2P::Send_CheckPollTerm()
//{
//    NULL_CHECK( m_pkUserProxy );
//    return m_pkUserProxy->SendID( KUserEvent::EVENT_CHAR_POLL_TERM_REQ );
//}

//bool KP2P::Send_CharPollReq( IN const char cCharType_, IN const GCITEMID itemID_, IN const int nUseCount_ )
//{
//    NULL_CHECK( m_pkUserProxy );
//    KEVENT_CHAR_POLL_REQ kPacket;
//    kPacket.m_cCharType = cCharType_;
//    kPacket.m_itemID    = itemID_ * 10;
//    kPacket.m_nUseCount = nUseCount_;
//    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHAR_POLL_REQ, kPacket );
//
//}

// 추천
bool KP2P::Send_CheckRecomEnableReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CHECK_RECOMMEND_ENABLE_REQ );
}

bool KP2P::Send_RecommendUserReq( const std::wstring& strNick_, const int nState_ ) //  bState 추천 할지 : 0, 추천 않함 : 1, 나중에 추천 할지 : 2.
{

    NULL_CHECK( m_pkUserProxy );
    KEVENT_RECOMMEND_USER_REQ kPacket;
    kPacket.m_nState = nState_;
    kPacket.m_strNick = strNick_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RECOMMEND_USER_REQ, kPacket );
}

bool KP2P::Send_RecommendFullInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_RECOMMEND_FULL_INFO_REQ);
}

bool KP2P::Send_AcceptRecommenderReq( const std::vector<DWORD>& vecRecommenders_ ) // 나를추천한 사람 중에 승인할 애들..
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RECOMMENDER_ACCEPT_REQ, vecRecommenders_ );
}

bool KP2P::Send_AttendBonusReq( const int nState_ ) // 3, 6, 9, 12
{

    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ATTENDANCE_BONUS_REQ, nState_ );
}

bool KP2P::Send_RecommenderDenyReq( const std::vector<DWORD>& vecRecommenders_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RECOMMENDER_DENY_REQ, vecRecommenders_ );
}

bool KP2P::Send_OpenCapsuleReq( const int nCharType_, const int nItemKind_, const int nIndex_, const int nPrice_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_OPEN_CAPSULE_REQ kPacket;
    kPacket.m_nCharType = nCharType_;
    kPacket.m_nItemKind = nItemKind_;
    kPacket.m_nCapsuleIndex = nIndex_;
    kPacket.m_nPrice = nPrice_;
#if defined(NATION_THAILAND)
    kPacket.m_nPriceType = 0;
#endif
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_OPEN_CAPSULE_REQ, kPacket );
}

bool KP2P::Send_OnlineFriendList(std::vector<DWORD> vecOnlineFriendList)
{
    NULL_CHECK(m_pkUserProxy);

    DELAY_CHECK(ESPTFriendList, 6);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_ONLINE_FRIENDS_NOT, vecOnlineFriendList);
}

//zstaiji - /setexp 숫자  입력하면 입력한 숫자대로 exp가 세팅 된다. 
bool KP2P::Send_SetExp( const char cCharType_, const __int64 biSetExp_ )
{
    NULL_CHECK( m_pkUserProxy );
    KAddExpInfo kPacket;
    kPacket.m_cCharType = cCharType_;   // 캐릭터 타입
    kPacket.m_biExp = biSetExp_;        // 설정할 경험치 수치
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SET_EXP_REQ, kPacket );
}

bool KP2P::Send_MissionPackage( const DWORD dwItemID_, const GCITEMUID dwItemUID_, const char cItemType_ )
{
    NULL_CHECK( m_pkUserProxy );

    KSimpleItem kPacket;
    kPacket.m_dwID      = dwItemID_ * 10;
    kPacket.m_dwUID     = dwItemUID_;
    kPacket.m_cItemType = cItemType_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MISSION_PACK_REGIST_REQ, kPacket );
}

bool KP2P::Send_StatPingInfo( const std::vector<KStatPingInfo>& vecStatPingInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    VECTOR_PING_INFO kPacket;
    kPacket = vecStatPingInfo_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USER_PING_INFO_NOT, kPacket );
}

bool KP2P::Send_GameCouponListReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GAME_COUPON_LIST_REQ );
}

bool KP2P::Send_UseGameCouponReq( const int nEventUID_, const std::wstring& strCouponID_, const std::wstring& strCouponPW )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_USE_GAME_COUPON_REQ kPacket;
    kPacket.m_nEventUID     = nEventUID_;       // 쿠폰이벤트UID
    kPacket.m_strCouponID   = strCouponID_;   // 게임쿠폰ID(strCouponID_ 스트링에 '홑따음표 입력 안되도록 체크해주세요)
    kPacket.m_strPassWD     = strCouponPW;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_GAME_COUPON_REQ, kPacket );
}

bool KP2P::Send_GameCouponFailNot()
{
    NULL_CHECK( m_pkUserProxy );
    // C->S 쿠폰사용 실패 알림(3회 연속 입력 실패시 보냄)
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GAME_COUPON_FAIL_NOT );
}

bool KP2P::Send_JackPotItemReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_RAINBOW_JACKPOT_ITEM_REQ );
}

bool KP2P::Send_DateChangeReq() 
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_MISSION_DATE_CHANGE_REQ );
}

bool KP2P::Send_ItemTradeReq(DWORD dwItemID_, int dwItemCount_, int iOption_ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_ITEM_TRADE_REQ kPacket;
    kPacket.m_nItemID		= dwItemID_;
    kPacket.m_nItemCount	= dwItemCount_;
    kPacket.m_nType			= iOption_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_TRADE_REQ, kPacket );
}

bool KP2P::Send_WebGachaCoinTrade(int dwItemID_, int dwItemCount_ )
{
    NULL_CHECK( m_pkUserProxy );
    
    KEVENT_WEB_GACHA_COIN_TRADE_REQ kPacket;
    kPacket.m_itemID = dwItemID_;
    kPacket.m_nItemCount = dwItemCount_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_WEB_GACHA_COIN_TRADE_REQ, kPacket );
}
bool KP2P::Send_CoOpItemBuyReq( const int nSlotID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KCoopEventItem kPacket;
    kPacket.m_nSlotID = nSlotID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CO_OP_ITEM_BUY_REQ, kPacket );

}

bool KP2P::Send_NpcGiftReq( const int nEventID_ )
{
    NULL_CHECK( m_pkUserProxy );
    // 광장 NPC클릭시 아이템받기 요청
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_NPC_GIFTS_REQ, nEventID_ );
}

bool KP2P::Send_CurrentSubscriptionGiftNumReq()
{
    NULL_CHECK( m_pkUserProxy );
    // 현재 응모이벤트 경품 남은 수량 확인
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CURRENT_SUBSCRIPTION_GIFT_NUM_REQ );
}

bool KP2P::Send_SubscriptReq( const int nEventUID, const DWORD dwTicketNum_ )
{
    NULL_CHECK( m_pkUserProxy );
    // 응모하기
    KEVENT_SUBSCRIPT_REQ kPacket;
    kPacket.m_nEventUID     = nEventUID;     // 응모 이벤트 UID
    kPacket.m_nTicketCount  = (int)dwTicketNum_;  // 응모하는 티켓 개수
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SUBSCRIPT_REQ, kPacket );
}
// C -> S
// 클라이언트가 등급 버튼을 클릭했을 경우 서버에게 보내는 REQ
// EVENT_DONATION_ITEM_GET_REQ
bool KP2P::Send_DonationGrade(const DWORD dwGrade_) 
{

    NULL_CHECK( m_pkUserProxy );
    KEVENT_DONATION_ITEM_GET_REQ kPacket;
    kPacket.m_dwGrade = dwGrade_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DONATION_ITEM_GET_REQ, kPacket);
}

bool KP2P::Send_AdvertisingExposureCountNot( const int nType_, int nCount/*=1*/ )
{
    NULL_CHECK( m_pkUserProxy );
    PAIR_INT prData( nType_, nCount );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADVERTISING_EXPOSURE_COUNT_NOT, prData );
}

bool KP2P::Send_CharacterStatInfoReq( const std::pair<char,char> prCharInfo_ )
{
    // prCharInfo_ : pair< 캐릭터ID, 직업 >
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHARACTER_STAT_INFO_REQ, prCharInfo_ );
}

bool KP2P::Send_ClientHackingUserNot( const int nHackType_ )
{
    // nHackType_ : 해킹타입 Enum 값
    // KEVENT_DUNGEON_HACKING_USER_NOT::MINIGAME_TIME_SCORE // 미니게임 시간대비 점수대 이상
    // KEVENT_DUNGEON_HACKING_USER_NOT::DUNGEON_STAGE_COUNT // 던전 최소 스테이지수보다 적음
    // KEVENT_DUNGEON_HACKING_USER_NOT::DUNGEON_CLEAR_TIME  // 던전 최단 클리어시간보다 ?음
    // KEVENT_DUNGEON_HACKING_USER_NOT::ATK_HACK            // 공격력 해킹
    // KEVENT_DUNGEON_HACKING_USER_NOT::INC_ATK_RATE_HACK   // 공격력증가율 해킹
    // KEVENT_DUNGEON_HACKING_USER_NOT::CLIENT_ALL_KILL     // 클라이언트 올킬(방장이 범인)
    // KEVENT_DUNGEON_HACKING_USER_NOT::CLIENT_MON_HP       // 클라이언트 몬스터HP해킹(방장이 범인)
    // KEVENT_DUNGEON_HACKING_USER_NOT::CLIENT_PARTY_USER   // 클라이언트 해킹 플레이랑 같이 플레이한 파티원
    // KEVENT_DUNGEON_HACKING_USER_NOT::MONSTER_TEMPLATE    // 몬스터템플릿 해킹
    // KEVENT_DUNGEON_HACKING_USER_NOT::CHARACTER_STAT      // 캐릭터 스텟 해킹
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CLIENT_HACKING_USER_NOT, nHackType_ );
}

// 보드게임 말판 기존 정보 요청.
// 보드게임판이 활성화 된 상태에서 1시간이 지난 상태에서 주사위 아이템 요청.
bool KP2P::Send_BoardGameInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_BOARD_GAME_INFO_REQ );
}

// 주사위 굴리기 버튼 클릭했을 경우.
bool KP2P::Send_DicePlayReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_DICE_PLAY_REQ );
}

// 주사위 아이템 교환을 할 경우.
bool KP2P::Send_DiceItemTradeReq( DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_DICE_ITEM_TRADE_REQ kPacket;
    kPacket.m_dwItemID = dwItemID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DICE_ITEM_TRADE_REQ, kPacket );
}

bool KP2P::Send_FrientInviteListReq()
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_INVITE_FRIEND_LIST_REQ);
}

bool KP2P::Send_JoinRoomFromChannelReq( const DWORD dwChannelUID_, const USHORT usRoomID_,
                                       const DWORD dwHostUID_, const std::wstring& strRoomPW_, const KInDoorUserInfo& kInfo_ )
{
    KEVENT_INVITED_ROOM_FROM_SQUARE_REQ kReq;
    kReq.m_dwChannelUID     = dwChannelUID_;
    kReq.m_usRoomID         = usRoomID_;
    kReq.m_dwHostUID        = dwHostUID_;
    kReq.m_strRoomPasswd    = strRoomPW_;
    kReq.m_kIndoorUserInfo  = kInfo_;

    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JOIN_ROOM_OTHER_CHANNEL_REQ, kReq, true, true );
}

// 주사위 아이템 구매가 가능한지에 대한 체크.
bool KP2P::Send_DiceItemBuyCheckReq( DWORD dwItemID_)
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_DICE_ITME_BUY_CHECK_REQ kPacket;
    kPacket.m_dwItemID = dwItemID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DICE_ITME_BUY_CHECK_REQ, kPacket );
}

//bool KP2P::Send_GWCEventInfoReq()
//{
//    NULL_CHECK( m_pkUserProxy );
//    return m_pkUserProxy->SendID( KUserEvent::EVENT_GWC_EVENT_INFO_REQ );
//}
//
//bool KP2P::Send_GWCDonationReq( const int nCount_ )
//{
//    NULL_CHECK( m_pkUserProxy );
//    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GWC_EVENT_DONATION_REQ, nCount_ );
//}
//
//bool KP2P::Send_GWCEventResultReq()
//{
//    NULL_CHECK( m_pkUserProxy );
//    return m_pkUserProxy->SendID( KUserEvent::EVENT_GWC_EVENT_RESULT_REQ );
//}

// New Guild - 길드목록창에서 최초 열 때랑 검색기능 정렬 기준 바뀔 때 마다
bool KP2P::Send_SearchGuildListReq( const UCHAR ucSortType_, const std::wstring strKeyword_, const UCHAR ucOrderBy_ )
{
    // 이 패킷을 부르는 경우는.
    // 1.길드목록창을 처음 열때,
    // 2.검색버튼을 누를때.

    // 정렬기준에 관해서.
    // GuildID는 생성시마다 1씩 값이 차례대로 증가함으로 창립일순과 동일하게 사용된다.
    // enum SORT_TYPE {
    //     ST_GUILD_ID_DATE,   // GuildID/창립일순 정렬(기본)
    //     ST_GUILD_NAME,      // 이름순
    //     ST_GUILD_GRADE,     // 등급순
    //     ST_GUILD_POINT,     // 길드포인트순
    //     ST_MAX,
    // };

    NULL_CHECK( m_pkUserProxy );
    KEVENT_SEARCH_GUILD_LIST_REQ kPacket;
    kPacket.m_ucSortType = ucSortType_; // 정렬기준
    kPacket.m_ucOrderBy = ucOrderBy_; // 오름차순 내림차순
    kPacket.m_strKeyword = strKeyword_; // 검색어
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SEARCH_GUILD_LIST_REQ, kPacket );
}

// 길드목록창에서 페이지이동
bool KP2P::Send_GuildListReq( const UCHAR ucSortType_, const std::wstring strKeyword_, const int iPageNum_, const UCHAR ucOrderBy_ )
{
    // 이 패킷은 페이지를 이동할때만 부르는 패킷이다.

    // 정렬기준(KEVENT_SEARCH_GUILD_LIST_REQ의 SORT_TYPE을 사용)
    // enum SORT_TYPE {
    //     ST_GUILD_ID_DATE,   // GuildID/창립일순 정렬(기본)
    //     ST_GUILD_NAME,      // 이름순
    //     ST_GUILD_GRADE,     // 등급순
    //     ST_GUILD_POINT,     // 길드포인트순
    //     ST_MAX,
    // };
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GUILD_LIST_REQ kPacket;
    kPacket.m_ucSortType = ucSortType_; // 정렬기준 (정렬된 상태에서 페이지 이동할 경우, 이 값을 꼭 채워줘야한다)
    kPacket.m_ucOrderBy = ucOrderBy_; // 오름차순 내림차순
    kPacket.m_strKeyword = strKeyword_; // 검색어 (검색한 상태에서 페이지 이동할 경우, 이 값을 꼭 채워줘야한다)
    kPacket.m_nPagenum = iPageNum_;     // 요청한 페이지 번호

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GUILD_LIST_REQ, kPacket );
}

// bool KP2P::Send_InviteGuildReq()
// {
// }

bool KP2P::Send_JoinGuildReq( const int iGuildID_, const std::wstring strComment_ )
{
    // 길드가입 신청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_JOIN_GUILD_REQ kPacket;
    kPacket.m_dwGuildUID = iGuildID_; // 가입할 길드의 ID
    kPacket.m_strMyComment = strComment_; // 자기소개
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JOIN_GUILD_REQ, kPacket );
}

bool KP2P::Send_CancelJoinGuildReq( const int iGuildID_ )
{
    // 길드가입 취소신청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CANCEL_JOIN_GUILD_REQ, iGuildID_ );
}

bool KP2P::Send_CreateGuildReq( const std::wstring strGuildName_, const std::wstring  strURL_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CREATE_GUILD_REQ kPacket;
    kPacket.m_strGuildName = strGuildName_; // 생성할 길드 이름
    kPacket.m_strGuildURL = strURL_; // 길드 URL
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CREATE_GUILD_REQ, kPacket );
}

bool KP2P::Send_EditGuildNoticeReq( const int iNoticeID_, const std::wstring strNotice_ )
{
    // 길드 소개/공지 수정시 불린다.

    // 공지타입에 맞춰 eunm값을 넣어주면 됨.
    // KNGuildNotice::GN_COMMENT  // 1.길드소개
    // KNGuildNotice::GN_NOTICE_1 // 2.길드공지1
    // KNGuildNotice::GN_NOTICE_2 // 3.길드공지2
    NULL_CHECK( m_pkUserProxy );
    KNGuildNotice kNotice;
    kNotice.m_nMsgID = iNoticeID_; // 공지타입
    kNotice.m_strMsg = strNotice_; // 공지내용

    KNGuildNoticeList kPacket;
    kPacket.m_vecNotice.push_back( kNotice );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EDIT_GUILD_NOTICE_REQ, kPacket );
}

bool KP2P::Send_EditGuildNameReq( const std::wstring strGuildName_, const DWORD dwGuildID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_EDIT_GUILD_NAME_REQ kPacket;
    kPacket.m_dwGuildUID = dwGuildID_;
    kPacket.m_strGuildName = strGuildName_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EDIT_GUILD_NAME_REQ, kPacket );
}

bool KP2P::Send_ChangeGuildJoinSettingReq( const UCHAR ucJoinSetting_ )
{
    NULL_CHECK( m_pkUserProxy );
    KNGuildJoinPolicy kPacket;
    kPacket.m_ucMethod = ucJoinSetting_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_GUILD_JOIN_SETTING_REQ, kPacket );
}

bool KP2P::Send_EditGuildUrlReq( const std::wstring strURL_, const DWORD dwGuildUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KNGuildURL kPacket;
    kPacket.m_dwUID = dwGuildUID_;
    kPacket.m_strURL = strURL_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EDIT_GUILD_URL_REQ, kPacket );
}

bool KP2P::Send_BreakupGuildReq( const int iGuildID_, const std::wstring strGuildName_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BREAKUP_GUILD_REQ kPacket;
    kPacket.m_dwGuildUID = iGuildID_; // 해체될 자기 길드 ID (체크용)
    //kPacket.m_strGuildName = strGuildName_; // 해체될 자기 길드 이름 (체크용)
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BREAKUP_GUILD_REQ, kPacket );
}

bool KP2P::Send_ChangeGuildMemberLevelReq( const std::map< char, std::set<DWORD> >&  mapChangeMemberLevel_ )
{
    // 길마가 길드원의 맴버레벨을 변경하거나,
    // 강제탈퇴 시키거나,
    // 길마 자리를 위임할때 불린다.
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CHANGE_GUILD_MEMBER_LEVEL_REQ kPacket;
    kPacket.m_mapChangeMemberLevel = mapChangeMemberLevel_; // map< 변경할 맴버레벨, set<UserUID> >

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_GUILD_MEMBER_LEVEL_REQ, kPacket );
}

bool KP2P::Send_SelfDrumoutGuildReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SELF_DRUMOUT_GUILD_REQ );
}

bool KP2P::Send_AcceptGuildJoinerReq( const SET_DWORD setAcceptUserUID_ )
{
    // 가입승인시 부른다.
    NULL_CHECK( m_pkUserProxy );
    KEVENT_ACCEPT_GUILD_JOINER_REQ kPacket;
    kPacket.m_setUserUID = setAcceptUserUID_; // 가입승인대상 유저UID 리스트

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ACCEPT_GUILD_JOINER_REQ, kPacket );
}

bool KP2P::Send_RejectGuildJoinerReq( const SET_DWORD setRejectUserUID_ )
{
    // 가입거절시 부른다.
    NULL_CHECK( m_pkUserProxy );
    KEVENT_REJECT_GUILD_JOINER_REQ kPacket;
    kPacket.m_setUserUID = setRejectUserUID_; // 가입승인대상 유저UID 리스트

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_REJECT_GUILD_JOINER_REQ, kPacket );
}

bool KP2P::Send_EditGuildMyCommentReq( const DWORD dwGuildUID_, const std::wstring& strComment_ )
{
    // 길드원이 자기소개 변경시 불린다.
    NULL_CHECK( m_pkUserProxy );
    KEVENT_EDIT_GUILD_MY_COMMENT_REQ kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_; // GuildUID
    kPacket.m_strMyComment = strComment_; // 바꿀 내 소갯말

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EDIT_GUILD_MY_COMMENT_REQ, kPacket );
}

bool KP2P::Send_GuildMarkUpload( IN const KSerBuffer& buffImage_, IN const std::wstring& strExt_, IN const GCITEMUID itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_MARK_UPLOAD_REQ kPacket;
    kPacket.m_buffImage = buffImage_;
    kPacket.m_strFileExt = strExt_;
    kPacket.m_kItem.m_ItemUID =  itemUID_ ;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MARK_UPLOAD_REQ, kPacket );
}

bool KP2P::Send_GuildRankReq( const int nType_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GUILD_RANK_REQ, nType_ );
}

bool KP2P::Send_GuildNoticeListReq()
{
    // 길드공지사항 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GUILD_NOTICE_LIST_REQ );
}

bool KP2P::Send_CreateRoomInSquareReq( DWORD dwChannelUID, const KRoomInfo& kRoomInfo, const KInDoorUserInfo& kInDoorUserInfo )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_CREATE_ROOM_FROM_SQUARE_REQ kPacket;

    kPacket.m_dwChannelUID = dwChannelUID;
    kPacket.m_kCreateRoom.m_kRoomInfo = kRoomInfo;
    kPacket.m_kCreateRoom.m_kInDoorUserInfo = kInDoorUserInfo;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CREATE_ROOM_FROM_SQUARE_REQ, kPacket, true, true );
}

bool KP2P::Send_JoinRoomFromSquareReq( const DWORD dwChannelUID, const unsigned int uiJoinType, USHORT usRoomID, const wchar_t* szRoomPasswd, const KInDoorUserInfo& kInDoorUserInfo, std::vector<USHORT> vecKickedRoomID, char cQuickJoinCategory /*= GC_GMC_INVALID*/, int iQuickJoinMode /*= GC_GM_INVALID*/, int nDifficult /*= GC_SGM_QUEST_LEVEL1*/, bool bGuild_ /*= false */ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_JOIN_ROOM_FROM_SQAURE_REQ kPacket;
    kPacket.m_dwChannelUID = dwChannelUID;

    KJoinRoomReqInfo& m_kJoinRoom = kPacket.m_kJoinRoom;
    m_kJoinRoom.m_uiJoinType = uiJoinType;
    m_kJoinRoom.m_cQuickJoinCategory = cQuickJoinCategory;
    m_kJoinRoom.m_nQuickJoinModeID = iQuickJoinMode;
    m_kJoinRoom.m_nDifficult = nDifficult;
    m_kJoinRoom.m_vecKickedRoomID = vecKickedRoomID;
    m_kJoinRoom.m_bGuild = bGuild_;

    switch( uiJoinType )
    {
    case KJoinRoomReqInfo::JOIN_NORMAL:
        {
            m_kJoinRoom.m_usRoomID          = usRoomID;
            m_kJoinRoom.m_strRoomPasswd     = szRoomPasswd;
            m_kJoinRoom.m_kInDoorUserInfo   = kInDoorUserInfo;
            break;
        }
    case KJoinRoomReqInfo::JOIN_QUICK:
        {
            m_kJoinRoom.m_kInDoorUserInfo   = kInDoorUserInfo;
            m_kJoinRoom.m_cQuickJoinCategory = cQuickJoinCategory;
            m_kJoinRoom.m_nQuickJoinModeID = iQuickJoinMode;
            break;
        }

    case KJoinRoomReqInfo::JOIN_GUILD:
        {
            m_kJoinRoom.m_usRoomID          = usRoomID;
            m_kJoinRoom.m_strRoomPasswd     = szRoomPasswd;    // 방 비밀번호 란에 유저의 Login을 넣어서 보낸다.
            m_kJoinRoom.m_kInDoorUserInfo   = kInDoorUserInfo;
            break;
        }

    default:
        return false;
    }

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JOIN_ROOM_FROM_SQAURE_REQ, kPacket, true, true );
}

bool KP2P::Send_InviteGuildUserReq( const std::vector<DWORD>& vecUsers )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JOIN_ROOM_FROM_SQAURE_REQ, vecUsers );
}

bool KP2P::Send_GuildPointReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GUILD_TOTAL_POINT_REQ );

}

bool KP2P::Send_GuildMemberListReq( const int nPageNum_/*=1*/ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GUILD_MEMBER_LIST_REQ kPacekt;
    kPacekt.m_nPageNum = nPageNum_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GUILD_MEMBER_LIST_REQ, kPacekt );
}

bool KP2P::Send_GetUserBingoDataReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_USER_BINGO_DATA_REQ );
}


bool KP2P::Send_ChangeBingoCoin()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CHANGE_BINGO_COIN_REQ );
}

bool KP2P::Send_GetBingoQuestion( const int nXPos_, const int nYPos_ )
{
    NULL_CHECK( m_pkUserProxy );
    std::pair<int,int> prPos(nXPos_,nYPos_);
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BINGO_QUESTION_REQ, prPos );
}

bool KP2P::Send_AnswerBingoQuestion( const int nQuestionID_, const int nAnswerNumver_,
                                     const std::pair<int,int>& prPos_, const bool bUseResurrection_ /*= false*/ )
{
  
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BINGO_ANSWER_REQ kPacket;
    kPacket.m_nQuestionID           = nQuestionID_;
    kPacket.m_nAnswerNum            = nAnswerNumver_;
    kPacket.m_prPos                 = prPos_;
    kPacket.m_bUseResurrection      = bUseResurrection_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BINGO_ANSWER_REQ, kPacket );
}

bool KP2P::Send_NewYearDonationInfoReq(DWORD dwType_)
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_NEW_YEAR_DONATION_INFO_REQ, dwType_);
}

bool KP2P::Send_NewYearDonationReq(DWORD dwType_,DWORD dwItemID_, int nItemCount_)
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_NEW_YEAR_DONATION_REQ kPacket;
    kPacket.m_dwType = dwType_;
    kPacket.m_dwItemID = dwItemID_;
    kPacket.m_nItemCount = nItemCount_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_NEW_YEAR_DONATION_REQ, kPacket );
}

bool KP2P::Send_ENU_OVERLAP_FILE_INFO()
{
    std::vector<KomfileManager::SDuplicateInfo> vecDuplicateFile = g_pGCDeviceManager2->GetMassFileManager()->GetDuplicatedFileList();
    
    KENU_OVERLAP_FILE_INFO kvecPacket;
    KOverlapFile kPacket;
    std::vector<KomfileManager::SDuplicateInfo>::iterator vit = vecDuplicateFile.begin();
#ifndef	__PATH__
    std::string strFileName = "DuplicatedKomFileList.txt";
    FILE* pFile = fopen( strFileName.c_str(), "wc" );
#endif
    for(;vit != vecDuplicateFile.end();vit++)
    {
        kPacket.m_strPreviousFile = vit->strOriginKomFileName;
        kPacket.m_strCurrentFile = vit->strDuplicatedKomFileName;
        kPacket.m_strOverlapFile = vit->strMemberFileName;
        kvecPacket.push_back(kPacket);
#ifndef	__PATH__
        if( pFile == NULL )
        {
            continue;
        }

        fprintf(pFile,"OriginKomFileName :\t%s\t",kPacket.m_strPreviousFile.c_str());
        fprintf(pFile,"DuplicatedKomFileName :\t%s\t",kPacket.m_strCurrentFile.c_str());
        fprintf(pFile,"MemberFileName :\t%s\r\n",kPacket.m_strOverlapFile.c_str());
#endif    
    }
#ifndef	__PATH__
    fclose(pFile);
#endif

    return  KP2P::GetInstance()->m_spNUserProxy->SendPacket( KNUserEvent::ENU_OVERLAP_FILE_INFO, kvecPacket );
}

bool KP2P::Send_AttributeSelectListReq( IN GCITEMUID itemUID_ )
{
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ATTRIBUTE_LIST_REQ, itemUID_ );
}

bool KP2P::Send_AttributeSelectReq( IN const GCITEMUID ItemUID_, IN const std::vector< KAttributeInfo >& vecSelectedAttributes_ )
{
    KEVENT_ATTRIBUTE_SELECT_REQ kPacket;
    kPacket.m_ItemUID = ItemUID_;
    kPacket.m_vecAttributeList = vecSelectedAttributes_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ATTRIBUTE_SELECT_REQ, kPacket );
}

bool KP2P::Send_OpenSocketReq( IN const GCITEMUID ItemUID_, IN const GCITEMID CashItemID_, IN const char SlotID_, IN const int ConsumeKind_ )
{
    KEVENT_SOCKET_OPEN_REQ kPacket;
    kPacket.m_ItemUID = ItemUID_;
    kPacket.m_CashItemID = CashItemID_ * 10;
    kPacket.m_kSocketInfo.m_cSlotID = SlotID_;
    kPacket.m_nConsumeKind = ConsumeKind_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SOCKET_OPEN_REQ, kPacket );

}

bool KP2P::Send_InsertItemSocketReq( IN const GCITEMUID ItemUID_, IN const GCITEMID CashItemID_, IN const char SlotID_, IN const int ConsumeKind_, IN const GCITEMID CardItemID_ )
{
    KEVENT_MONSTER_CARD_INSERT_REQ kPacket;
    kPacket.m_ItemUID = ItemUID_;
    kPacket.m_CardItemID = CardItemID_ * 10;
    kPacket.m_kSocketInfo.m_cSlotID = SlotID_;
    kPacket.m_CashItemID = CashItemID_ * 10;
    kPacket.m_nConsumeKind = ConsumeKind_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MONSTER_CARD_INSERT_REQ, kPacket );
}

bool KP2P::Send_RemoveItemSocketReq( IN const GCITEMUID ItemUID_, IN const GCITEMID CashItemID_, IN const char SlotID_, IN const int ConsumeKind_ )
{
    KEVENT_MONSTER_CARD_REMOVE_REQ kPacket;
    kPacket.m_ItemUID = ItemUID_;
    kPacket.m_CashItemID = CashItemID_ * 10;
    kPacket.m_kSocketInfo.m_cSlotID = SlotID_;
    kPacket.m_nConsumeKind = ConsumeKind_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MONSTER_CARD_REMOVE_REQ, kPacket );

}

bool KP2P::Send_GetWebCashItemReq( IN const int iType, IN const DWORD m_dwEventUID_)
{
	KEVENT_GET_WEB_CASH_ITEM_REQ kPacket;

    kPacket.m_nItemCharType = iType;
    kPacket.m_dwEventUID = m_dwEventUID_;

    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GET_WEB_CASH_ITEM_REQ, kPacket );
}

bool KP2P::Send_StatGameResolution( IN const int nWidth_, IN const int nHeight_, IN const int nMaxWidth_, IN const int nMaxHeight_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_STAT_RESOLUTION_NOT kPacket;
    kPacket.m_nWidth = nWidth_;
    kPacket.m_nHeight = nHeight_;
    kPacket.m_nMaxWidth = nMaxWidth_;
    kPacket.m_nMaxHeight = nMaxHeight_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_STAT_RESOLUTION_NOT, kPacket );
}
bool KP2P::Send_ItemBreakupReq( IN const GCITEMUID ItemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_BREAKUP_REQ, ItemUID_ );
}

bool KP2P::Send_GetItemCatalog()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_MANUFACTURES3_CATALOG_REQ );
}

bool KP2P::Send_GetMaterials( IN const std::set<PAIR_USHORT_DWORD>& setItems_ )
{
    NULL_CHECK( m_pkUserProxy );
    std::set<PAIR_USHORT_DWORD> kPacket;
    std::set<PAIR_USHORT_DWORD>::const_iterator sit;
    for ( sit = setItems_.begin() ; sit != setItems_.end() ; ++sit ) {
        kPacket.insert( std::make_pair( sit->first, (sit->second) * 10 ) );
    }
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MANUFACTURES3_MATERIAL_REQ, kPacket );
}

bool KP2P::Send_ItemCreationReq( IN const USHORT Index_, IN const GCITEMID ItemID_, IN const USHORT Factor_, IN const std::vector< GCITEMUID > vecUIDList_  )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_MANUFACTURES3_REQ kPacket;
    kPacket.m_kCatalog.m_Index = Index_;
    kPacket.m_kCatalog.m_ItemID = ItemID_ * 10;
    kPacket.m_usFactor = Factor_;
    kPacket.m_vecDelMaterials = vecUIDList_;
    kPacket.m_dDiscount = g_kGlobalValue.currentTotalForgeDiscount;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MANUFACTURES3_REQ, kPacket );
}

bool KP2P::Send_BreakUpPriceRatioReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ITEM_BREAKUP_PRICE_RATIO_REQ );
}

// 선물 상자 아이템을 클릭하고 정보요청.
bool KP2P::Send_PresentboxInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_PRESENTBOX_INFO_REQ );
}

// C->S 특정 조각 아이템 교환 요청
bool KP2P::Send_PresentboxTradeReq( DWORD dwItemID )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_PRESENTBOX_TRADE_REQ kPacket;
    kPacket.m_dwItemID = dwItemID;	
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PRESENTBOX_TRADE_REQ, kPacket );
}

// C->S 조각을 다 모아서 선물받기 버튼 클릭.
bool KP2P::Send_PresentboxActionReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_PRESENTBOX_ACTION_REQ );
}

//구매 제한 아이템 확인 여부
bool KP2P::Send_CanBuyCashItem( IN const GCITEMID& ItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_BUY_CHECK_REQ, ItemID_ );
}

bool KP2P::Send_DungeonStatusReq( IN const std::set<int>& setGameModeList_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_STATE_REQ, setGameModeList_ );
}

//bool KP2P::Send_TourmentDonationReq( IN const int nType_, IN const GCITEMID ItemID_)
//{
//    NULL_CHECK( m_pkUserProxy );
//
//    KEVENT_TOURNAMENT_DONATION_REQ kPacket;
//    kPacket.m_nType = nType_;
//    kPacket.m_itemID = ItemID_ * 10;
//    kPacket.m_nItemCount = 1; //1개씩 기부함
//    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_TOURNAMENT_DONATION_REQ, kPacket );
//}

bool KP2P::Send_EffectTextureReport( const KEVENT_EFFECT_TEX_REPORT& kData_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EFFECT_TEX_REPORT, kData_, true, true );
}

bool KP2P::Send_GuildeBookListReq()
{
    NULL_CHECK( m_spNUserProxy );
    return m_spNUserProxy->SendID( KNUserEvent::ENU_GUIDE_BOOK_LIST_REQ );
}

bool KP2P::Send_GuideBoolCheckPointNot( IN const short sKey_, IN const short sValue_ )
{
    NULL_CHECK( m_pkUserProxy );
    PAIR_SHORT kPacket( sKey_, sValue_);
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GUIDE_BOOL_CHECKPOINT_NOT, kPacket );
}

bool KP2P::Send_CheckDynamyLoadReq()
{
    NULL_CHECK( m_spNUserProxy );
    return m_spNUserProxy->SendID( KNUserEvent::ENU_TEXTURE_DYNAMIC_LOAD_REQ );
}

bool KP2P::Send_ExitNormalNot()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_NORMAL_EXIT_NOT );
}

bool KP2P::Send_PingInfoNot( IN const DWORD dwPingInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CLIENT_PING_REPORT_NOT, dwPingInfo_ );
}

bool KP2P::Send_PingConfigReq()
{
    NULL_CHECK( m_spNUserProxy );
    return m_spNUserProxy->SendID( KNUserEvent::ENU_CLIENT_PING_CONFIG_REQ );
}

bool KP2P::Send_GetRoomMenberPressStateReq()
{
    
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GET_ROOMUSER_PRESS_STATE_REQ );

}

bool KP2P::Send_SetMyPressState( IN const int nState_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SET_PRESS_STATE_REQ, nState_ );
}

bool KP2P::Send_PressMember( IN const DWORD dwUserUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PRESSURE_USER_REQ, dwUserUID_ );
}

bool KP2P::Send_GetRoomMenberIdleState()
{   
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GET_ROOMUSER_IDLE_STATE_REQ );
}

bool KP2P::Send_SetMyIdleState( IN const bool bIdle_ )
{
    int kPacket = (bIdle_ == true ? 1 : 0 );
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SET_IDLE_STATE_REQ, kPacket );
}

bool KP2P::Send_RoomMemberPingInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ROOM_MEMBER_PING_INFO_REQ );
}

bool KP2P::Send_RelayLoadingState( IN const std::pair<DWORD,int> kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RELAY_LOADING_STATE, kPacket_ );

}

bool KP2P::Send_NormalItemToLookItem( IN const std::set<GCITEMUID>& setItems_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_NORMAL_TO_LOOK_REQ, setItems_ );
}

bool KP2P::Send_LookItemToNormalItem( IN const std::set<GCITEMUID>& setItems_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_LOOK_TO_NORMAL_REQ, setItems_ );
}

bool KP2P::Send_SaveCoordiReq( IN const KSaveCoordiInfo& kCoordiInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SAVE_COORDI_REQ, kCoordiInfo_ );
}

bool KP2P::Send_ChangeLookEqipReq( IN const KEVENT_CHANGE_LOOK_EQUIP_REQ& kReq_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_LOOK_EQUIP_REQ, kReq_ );
}

bool KP2P::Send_BundleSellReq( IN const KEVENT_BUNDLE_SELL_ITEM_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUNDLE_SELL_ITEM_REQ, kPacket_ );
}

bool KP2P::Send_BuyForGambleReq( IN const std::vector<GCITEMID>& vecItemID_, const int nDuration_, const int m_nPeriod_ )
{
    // 겜블 구매 요청
    NULL_CHECK( m_pkUserProxy );

    KEVENT_BUY_FOR_GAMBLE_REQ kPacket;
    kPacket.m_vecItemID = vecItemID_;
    kPacket.m_nDuration = nDuration_;
    kPacket.m_nPeriod = m_nPeriod_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_GAMBLE_REQ, kPacket );
}

bool KP2P::Send_CostRateForGambleBuyReq()
{
    // 겜블 구매비용 비율 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_COST_RATE_FOR_GAMBLE_BUY_REQ );
}

bool KP2P::Send_BossDungeonStat( IN const KEVENT_STAT_END_GAME_INFO& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_STAT_END_GAME_INFO, kPacket_ );
}

bool KP2P::Send_HeroItemCatalogReq()
{
    NULL_CHECK( m_pkUserProxy );
    // 영웅아이템 리스트 요청
    return m_pkUserProxy->SendID( KUserEvent::EVENT_HERO_ITEM_CATALOG_REQ );
}
bool KP2P::Send_HeroItemMaterialReq( IN const std::set<PAIR_USHORT_DWORD>& setItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    // 영웅 아이템 구매조건 요청
    // setItemID_ : 영웅아이템 ItemID 리스트
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_HERO_ITEM_MATERIAL_REQ, setItemID_ );
}

bool KP2P::Send_BuyForHeroReq( IN const PAIR_USHORT_PAIR_DWORD_INT prKey_ )
{
    NULL_CHECK( m_pkUserProxy );
    // 영웅아이템 구매요청
    // prKey_.first : 구매할 영웅아이템 Index
    // prKey_.second : 구매할 영웅아이템 ItemID
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_HERO_REQ, prKey_ );

}

bool KP2P::Send_CharPromotionUpdateReq(  IN const GCITEMID ItemID_, IN const char cCharType, IN const char cPromotionLevel  )
{
    NULL_CHECK( m_pkUserProxy );
    // 캐릭터 전직 아이템 사용 요청
    // m_dwItemID;         // 전직 마법서 ItemID
    // m_cCharType;        // 전직할려는 CharType
    // m_cCharPromotion;   // 전직할려는 전직레벨
    KEVENT_CHAR_PROMOTION_UPDATE_REQ kPacket;
    kPacket.m_dwItemID = ItemID_;
    kPacket.m_cCharType = cCharType;
    kPacket.m_cCharPromotion = cPromotionLevel;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHAR_PROMOTION_UPDATE_REQ, kPacket );
}

bool KP2P::Send_SpecialRewardReq( IN const std::map< DWORD, std::set<USHORT> >& mapUserSelect_ )
{
    NULL_CHECK( m_pkUserProxy );
    // 유저들이 선택한 보상상자 아이템 요청
    // map< DWORD,set<USHORT> >     map[ UserUID, set[선택한보상상자Index] ]

    KRewardInfoList vecRewardInfo;
    vecRewardInfo.reserve( mapUserSelect_.size() );

    std::map<DWORD, std::set<USHORT> >::const_iterator cmit;
    for( cmit = mapUserSelect_.begin() ; cmit != mapUserSelect_.end() ; ++cmit ) {
        KRewardInfo kInfo;
        kInfo.m_dwUID = cmit->first;            // UserUID
        kInfo.m_setSelectIndex = cmit->second;  // 선택한보상상자Index
        vecRewardInfo.push_back( kInfo );
    }

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SPECIAL_REWARD_REQ, vecRewardInfo );
}

bool KP2P::Send_TRInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_TR_SERVER_INFO_REQ );

}

bool KP2P::Send_GetUserGwibawiboDataReq(int ver_)
{
    NULL_CHECK( m_pkUserProxy );
    int kPacket = ver_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GAWIBAWIBO_INFO_REQ, kPacket );
}

bool KP2P::Send_GetUserGwibawiboReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GAWIBAWIBO_REQ);
}

bool KP2P::Send_GetUserGwibawiboGiveUpReq(int iRetry)
{
    NULL_CHECK( m_pkUserProxy );
    int kPacket = iRetry;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GAWIBAWIBO_GIVE_UP_REQ, kPacket );
}

bool KP2P::Send_GetUserGwibawiboTryPointInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GAWIBAWIBO_TRYPOINT_INFO_REQ);
}

bool KP2P::Send_GawibawiboTradeReq( DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GAWIBAWIBO_ITEM_TRADE_REQ, dwItemID_ );
}

bool KP2P::Send_DungeonRewardGPNot( IN const std::set<DWORD>& setGpUID_ )
{
    // 던전 스테이지 넘어갈때마다 획득한 GP코인 정보를 방장이 서버로 알림.
    // setGpUID_ : 획득한 GP UID
    if ( setGpUID_.empty() ) return true;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_REWARD_GP_NOT, setGpUID_ );
}

bool KP2P::Send_DungeonRewardItemReq( IN const std::pair<DWORD,DWORD>& prDrop_, IN const std::pair<DWORD,std::vector<DWORD> >& prRouletteList_ )
{
    // 던전 아이템 획득시마다, 방장이 서버로 전달.
    // prDrop_ : 미션아이템 드랍. [DropUID, UserUID]
    // m_prRouletteList : 룰렛아이템 드랍 [DropUID, UserUID List]
    KEVENT_DUNGEON_REWARD_ITEM_REQ kPacket;
    kPacket.m_prDropList = prDrop_;
    kPacket.m_prRouletteList = prRouletteList_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_REWARD_ITEM_REQ, kPacket );
}

bool KP2P::Send_DungeonRewardExpReq( IN const int& nTriggerID_ )
{
    // 던전 몬스터 잡을때마다 방장이 서버로 전달
    // nTriggerID_ : 죽인 몬스터의 TriggerID
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_REWARD_EXP_REQ, nTriggerID_ );
}

bool KP2P::Send_PvpRewardExpGpReq( IN const std::pair<DWORD,char>& prKillUserInfo_, IN const std::pair<DWORD,char>& prDeathUserInfo_ )
{
    // 대전 상대 죽였을때 보상 방장이 서버로 요청.
    // prKillUserInfo_ : 죽인 캐릭터 정보 [UserUID,CharType]
    // prDeathUserInfo_ : 죽은 캐릭터 정보 [UserUID,CharType]
    KEVENT_PVP_REWARD_EXP_GP_REQ kPacket;
    kPacket.m_prKillUserInfo = prKillUserInfo_;
    kPacket.m_prDeathUserInfo = prDeathUserInfo_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PVP_REWARD_EXP_GP_REQ, kPacket );
}

bool KP2P::Send_PvpTagRewardExpGpReq( IN const DWORD& dwKillUserUID_, IN const std::vector< std::pair<DWORD,char> >& vecKillTeamInfo_, IN const std::pair<DWORD,char>& prDeathUserInfo_ )
{
    // 태그매치 실시간 Exp값 처리.
    // dwKillUserUID_ : 죽인 캐릭터 UserUID
    // vecKillTeamInfo_ : 죽인 캐릭터 팀의 정보 [UserUID, CharType]
    // prDeathUserInfo_ : 죽은 캐릭터 정보 [UserUID,CharType]
    KEVENT_PVP_TAG_REWARD_EXP_GP_REQ kPacket;
    kPacket.m_dwKillUserUID = dwKillUserUID_;
    kPacket.m_vecKillTeamInfo = vecKillTeamInfo_;
    kPacket.m_prDeathUserInfo = prDeathUserInfo_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PVP_TAG_REWARD_EXP_GP_REQ, kPacket );
}
bool KP2P::Send_InitItemAttributeReq( DWORD dwItemUID_, DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_INIT_ITEM_ATTRIBUTE_REQ kPacket;
    kPacket.m_ItemID = dwItemID_*10;
    kPacket.m_ItemUID = dwItemUID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_INIT_ITEM_ATTRIBUTE_REQ, kPacket );
}

bool KP2P::Send_InitGPItemAttributeReq( GCITEMUID dwItemUID_, DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_INIT_ITEM_ATTRIBUTE_REQ kPacket;
    kPacket.m_ItemID = dwItemID_*10;
    kPacket.m_ItemUID = dwItemUID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ, kPacket );
}


bool KP2P::Send_SocksHangupReq( IN const std::vector< GCITEMID >& vecSocks_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SOCKS_HANGUP_REQ, vecSocks_ );
}

bool KP2P::Send_SocksCollectReq( IN const std::vector< GCITEMID >& vecSocks_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SOCKS_COLLECT_REQ, vecSocks_ );    
}

bool KP2P::Send_SocksMaterialExchangeReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SOCKS_MATERIAL_EXCHANGE_REQ );
}

//bool KP2P::Send_OneadayInfoReq()
//{
//	NULL_CHECK( m_pkUserProxy );
//	return m_pkUserProxy->SendID( KUserEvent::EVENT_ONEADAY_INFO_REQ );
//}

bool KP2P::Send_UnlockChangeWeaponReq( IN const char& cCharacterType_ )
{
    // 무기체인지 언락할 캐릭터의 타입을 인자로 전달해주세요.
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_UNLOCK_CHANGE_WEAPON_REQ, cCharacterType_ );
}

bool KP2P::Send_ItemAttributeRandomSelectReq( IN GCITEMUID dwItemUID_, IN DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ kPacket;
    kPacket.m_ItemID = dwItemID_ * 10;
    kPacket.m_ItemUID = dwItemUID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_ATTRIBUTE_RANDOM_SELECT_REQ, kPacket );
}

bool KP2P::Send_GPItemAttributeRandomReq( IN GCITEMUID dwItemUID_, IN DWORD dwItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GP_ATTRIBUTE_RANDOM_REQ kPacket;
    kPacket.m_ItemID = dwItemID_ * 10;
    kPacket.m_ItemUID = dwItemUID_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GP_ATTRIBUTE_RANDOM_REQ, kPacket );
}

bool KP2P::Send_UserServerRoomReq( IN const DWORD& dwTargetUserUID_, IN std::wstring strTargetNick_/*=L""*/ )
{
    // 따라가기할 친구의 위치정보 문의.
    // dwTargetUserUID_ : 따라가기할 친구의 UserUID. UID를 알고있는 경우, 이값을 설정해서 보낼것.
    // strTargetNick_ : 따라가기할 친구의 닉네임. 채팅창에서 커맨드 입력으로 닉네임을 넣을 경우, 이값을 설정해서 보낼것.
    NULL_CHECK( m_pkUserProxy );
    PAIR_DWORD_WSTRING kPacket;
    kPacket.first = dwTargetUserUID_;
    kPacket.second = strTargetNick_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USER_SERVER_ROOM_REQ, kPacket );
}

bool KP2P::Send_DisableTodaysPopupNot()
{
    // 일일 팝업창 끄기 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_DISABLE_TODAYS_POPUP_NOT );
}


bool KP2P::Send_ResetSkillReq( IN const GCITEMID& ItemID_, IN const char& cCharType_ )
{
    // 스킬전체초기화 요청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_RESET_SKILL_REQ kPacket;
    kPacket.m_ItemID = ItemID_ * 10; // 사용할 스킬전체초기화 아이템ItemID
    kPacket.m_cCharType = cCharType_; // 초기화할 캐릭터타입

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RESET_SKILL_REQ, kPacket );
}

bool KP2P::Send_FashionCatalogReq()
{
    // 패션 카탈로그 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_FASHION_CATALOG_REQ );
}

bool KP2P::Send_UserRecomTypeReq()
{
    // 반갑다친구야 유저타입 문의
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_USER_RECOM_TYPE_REQ );
}


bool KP2P::Send_PlantTreeInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_PLANT_TREE_INFO_REQ );
}

bool KP2P::Send_PlantTreeActionReq(DWORD dwActionType,DWORD dwItemID)
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_PLANT_TREE_ACTION_REQ kPacket;
    kPacket.m_dwActionType = dwActionType;
    kPacket.m_dwItemID = dwItemID*10;
    kPacket.m_dwContinentType = 0;
    kPacket.m_dwCurrentPosition = 0;
    kPacket.m_dwTreeStatus = 0;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PLANT_TREE_ACTION_REQ, kPacket );
}

bool KP2P::Send_PlantTreeRewardReq(int iType)
{
    int kPacket = iType; // 임시로 처리해둠.( 1 : 열매보상, 2 : 대륙보상 )
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PLANT_TREE_REWARD_REQ, kPacket );
}

bool KP2P::Send_ChoiceBoxListReq()
{
    // 초이스박스 리스트 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CHOICE_BOX_LIST_REQ );
}

bool KP2P::Send_ChoiceBoxInfoReq( IN const GCITEMID& BoxItemID_ )
{
    // 초이스박스 정보요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHOICE_BOX_INFO_REQ, BoxItemID_ );
}

bool KP2P::Send_ChoiceBoxOpenReq( IN const GCITEMID& BoxItemID_, IN const std::set<USHORT>& setSelectRewardIndex_ )
{
    // 초이스박스 열기.
    NULL_CHECK( m_pkUserProxy );
    KEVENT_CHOICE_BOX_OPEN_REQ kPacket;
    kPacket.m_BoxItemID = BoxItemID_; // 박스의 ItemID
    kPacket.m_setSelectMaterialID = setSelectRewardIndex_; // 선택한 박스보상 Index
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHOICE_BOX_OPEN_REQ, kPacket );
}

bool KP2P::Send_GuildStoreCatalogReq()
{
    // 길드상점 아이템리스트 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GUILD_STORE_CATALOG_REQ );
}

bool KP2P::Send_BuyForGuildReq( IN const int& nTabID_, IN const USHORT& usIndex_, IN const GCITEMID& ItemID_ )
{
    // 길드상점 아이템구매 요청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_BUY_FOR_GUILD_REQ kPacket;
    kPacket.m_nTabID = nTabID_; // 구매할 아이템의 텝ID
    kPacket.m_usIndex = usIndex_; // 구매할 아이템의 Index값
    kPacket.m_ItemID = ItemID_ * 10; // 구매할 아이템의 ItemID
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_BUY_FOR_GUILD_REQ, kPacket );
}

bool KP2P::Send_UserAuthCheckReq( KEVENT_USER_AUTH_CHECK_REQ& kData_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_USER_AUTH_CHECK_REQ kPacket = kData_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USER_AUTH_CHECK_REQ, kPacket );
}

bool KP2P::Send_VipEventListReq()
{
    // VIP이벤트 리스트 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_VIPEVENT_LIST_REQ );
}

bool KP2P::Send_VipEventUserInfoReq( IN const USHORT& usVer_ )
{
    // VIP이벤트 유저정보 요청
    // usVer_ : Vip이벤트 version
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_VIPEVENT_USER_INFO_REQ, usVer_ );
}

bool KP2P::Send_MagicBoxListReq()
{
    // C->S. 두근두근마법상자 이벤트 아이템 리스트 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_MAGIC_BOX_LIST_REQ );
}

bool KP2P::Send_SetMagicBoxTargetReq( IN const GCITEMID& itemID_ )
{
    // C->S. 두근두근마법상자 잠금깎을 상자 설정 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SET_MAGIC_BOX_TARGET_REQ, itemID_ );
}

bool KP2P::Send_UseMagicBoxReq( IN const GCITEMID& targetItemID_, IN const GCITEMID& keyItemID_ )
{
    // C->S. 두근두근마법상자 상자 잠금깎기 요청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_USE_MAGIC_BOX_REQ kPacket;
    kPacket.m_TargetItemID = targetItemID_; // 키를 사용할 아이템ID
    kPacket.m_KeyItemID = keyItemID_; // 키 ItemID
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_MAGIC_BOX_REQ, kPacket );
}


bool KP2P::Send_ItemLevelListReq( IN const DWORD dwItemUID )
{
    NULL_CHECK( m_pkUserProxy );
    DWORD kPacket = dwItemUID;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_LEVEL_LIST_REQ, kPacket );
}

bool KP2P::Send_ItemLevelSelectReq( IN const DWORD dwItemUID, IN const int iLevel )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_ITEM_LEVEL_SELECT_REQ kPacket;
    kPacket.m_ItemUID = dwItemUID;
    kPacket.m_nLevel = iLevel;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_LEVEL_SELECT_REQ, kPacket );
}

bool KP2P::Send_PackageInfoReq()
{
    // 패키지 아이템 정보 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_PACKAGE_INFO_REQ );
}

bool KP2P::Send_PackageInfoDetailReq(IN const int& nMaxIndex, IN const int& nCurrentIndex )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_PACKAGE_INFO_DETAIL_REQ kPacket;
    kPacket.m_nMaxIndex = nMaxIndex;
    kPacket.m_nCurrentIndex = nCurrentIndex;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PACKAGE_INFO_DETAIL_REQ, kPacket );
}

bool KP2P::Send_ItemAttributeTableIDReq( IN const GCITEMUID& ItemUID_ )
{
    // 아이템 속성테이블 ID 요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_ATTRIBUTE_TABLE_ID_REQ, ItemUID_ );
}

bool KP2P::Send_RickoonActionReq( IN DWORD dwType, IN DWORD dwCharType, IN GCITEMID CoinItemID, IN GCITEMID LuckyCoinItemID )
{
    //
    NULL_CHECK( m_pkUserProxy );
    KEVENT_RKTORNADO_ACTION_REQ kPacket;
    kPacket.m_dwType = dwType;
    kPacket.m_dwCharType = dwCharType;
    kPacket.m_CoinItemID = CoinItemID;
    kPacket.m_LuckyCoinItemID = LuckyCoinItemID;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RKTORNADO_ACTION_REQ, kPacket);
}


bool KP2P::Send_OpenCalendarNot()
{
    // 출석부 열때마다 부르세요.
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_OPEN_CALENDAR_BONUS_POINT_REQ );
}
bool KP2P::Send_UserSphinxDataReq()
{
    // C->S 유저의 스핑크스 데이터 요청.
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_USER_SPHINX_DATA_REQ );
}

bool KP2P::Send_SphinxQuestionReq(IN const DWORD dwItemID)
{
    // C->S 유저의 스핑크스 질문 요청.
    NULL_CHECK( m_pkUserProxy );

    KEVENT_SPHINX_QUESTION_REQ kPacket;
    kPacket.m_nCoinItemID = dwItemID;
    kPacket.m_nQuestionID = 0;
    kPacket.m_nTimeValue = 0;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SPHINX_QUESTION_REQ, kPacket );
}

bool KP2P::Send_SphinxAnswerReq( IN const int nQuestionID_, IN const int nAnswerNum_ )
{
    // C->S 유저의 스핑크스 질문 요청.
    NULL_CHECK( m_pkUserProxy );
    KEVENT_SPHINX_ANSWER_REQ kPacket;
    kPacket.m_nQuestionID = nQuestionID_;
    // 0 == X
    // 1 == O
    // 2 == 시간초과
    kPacket.m_nAnswerNum = nAnswerNum_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SPHINX_ANSWER_REQ, kPacket );
}
bool KP2P::Send_CreatePartyReq(IN const DWORD dwUserUID, IN const DWORD dwPartyType)
{
    NULL_CHECK(m_pkUserProxy);

    KEVENT_CREATE_PARTY_REQ kPacket;
    kPacket.m_dwInviteUserUID = dwUserUID;
    kPacket.m_dwPartyType = dwPartyType;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_CREATE_PARTY_REQ, kPacket);
}

bool KP2P::Send_JoinPartyReq(IN const KPartyData kPartyData, bool bIsJoin)
{
    NULL_CHECK(m_pkUserProxy);
    KPartyData kPacket = kPartyData;

    kPacket.m_bAnswer = bIsJoin;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_CREATE_PARTY_ANSWER, kPacket);
}

bool KP2P::Send_InvitePartyRoomReq()
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_INVITE_PARTY_ROOM_REQ);
}

bool KP2P::Send_InvitePartyReq(IN const DWORD dwInviteeUID_, IN const DWORD dwPartyType)
{
    NULL_CHECK(m_pkUserProxy);

    KEVENT_INVITE_PARTY_REQ kPacket;
    kPacket.m_dwInviteUserUID = dwInviteeUID_;
    kPacket.m_dwPartyType = dwPartyType;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_INVITE_PARTY_REQ, kPacket);
}

bool KP2P::Send_InvitePartyAnswer(IN const KPartyData& kData_, bool bIsInvite)
{
    NULL_CHECK(m_pkUserProxy);
    KPartyData kPacket = kData_;
    kPacket.m_bAnswer = bIsInvite;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_INVITE_PARTY_ANSWER, kPacket);
}

bool KP2P::Send_ChangePartyHostReq(IN const DWORD dwNewHostUID_)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_CHANGE_PARTY_HOST_REQ, dwNewHostUID_);
}

bool KP2P::Send_LeavePartyUserReq(IN const DWORD dwUserUID_)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_LEAVE_PARTY_USER_REQ, dwUserUID_);
}

bool KP2P::Send_BanPartyUserReq(IN const DWORD dwUserUID_)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_BAN_PARTY_USER_REQ, dwUserUID_);
}

// ����Ʈ
bool KP2P::Send_EnterAgitReq(IN const DWORD& dwAgitUID_, IN const char& cCharType_, IN std::wstring strPassword_/*=L""*/)
{
    // ����Ʈ ����
    // dwAgitUID_ : ������ ����ƮUID
    // cCharType_ : ������ ĳ���� Ÿ��
    // strPassword_ : ������� ���, ��й�ȣ
    NULL_CHECK(m_pkUserProxy);
    KEnterAgitReq kPacket;
    kPacket.m_dwAgitUID = dwAgitUID_;
    kPacket.m_cCharType = cCharType_;
    kPacket.m_strPassword = strPassword_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_ENTER_AGIT_REQ, kPacket);
}

bool KP2P::Send_AgitLoadingCompleteReq()
{
    // ����Ʈ ����� �ε� �Ϸ��ϸ� ������
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_LOADING_COMPLETE_REQ);
}

bool KP2P::Send_AgitLoadingFailNot()
{
    // ����Ʈ ����� �ε� ���н� ������
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_LOADING_FAIL_NOT);
}

bool KP2P::Send_LeaveAgitReq()
{
    // ����Ʈ ������
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_LEAVE_AGIT_REQ);
}

bool KP2P::Send_AgitMapCatalogueReq()
{
    // ���Ű����� ����Ʈ �� īŻ�α� ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_MAP_CATALOGUE_REQ);
}

bool KP2P::Send_BuyAgitMapReq(IN const DWORD& dwMapUID_)
{
    // ����Ʈ �� ���ſ�û
    // dwMapUID_ : ������ ��UID
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_BUY_AGIT_MAP_REQ, dwMapUID_);
}

bool KP2P::Send_SetAgitOptionReq(std::wstring strName_, UCHAR ucMaxUser_, UCHAR ucOpenType_, std::wstring strPassword_, UCHAR ucInviteType_, std::wstring strProfile_)
{
    // ����Ʈ ���� ���� ��û
    // ������ ����Ǿ��� ��쿡�� �����ּ���.
    // ����Ǵ� �׸� �̿��� ������ ���� ������ �����ؼ� �����ּ���.
    NULL_CHECK(m_pkUserProxy);

    KAgitOption kPacket;
    kPacket.m_strName = strName_; // ����Ʈ��
    kPacket.m_ucMaxUser = ucMaxUser_; // ���������ο�
    kPacket.m_ucOpenType = ucOpenType_; // ���¼���(KAgitInfo::OPEN_TYPE)
    kPacket.m_strPassword = strPassword_; // ��ȣ
    kPacket.m_ucInviteType = ucInviteType_; // �ʴ뼳��(KAgitInfo::INVITE_TYPE)
    kPacket.m_strProfile = strProfile_; // ������

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SET_AGIT_OPTION_REQ, kPacket);
}

bool KP2P::Send_AgitStoreCatalogReq()
{
    // ����Ʈ���� ������ ����Ʈ ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_STORE_CATALOG_REQ);
}

bool KP2P::Send_AgitStoreMaterialReq(IN const std::set<PAIR_USHORT_DWORD>& setItemID_)
{
    // ����Ʈ���� ������ �������� ��û
    NULL_CHECK(m_pkUserProxy);
    // setItemID_ : set[ pair[Index,ItemID] ]
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_AGIT_STORE_MATERIAL_REQ, setItemID_);
}

bool KP2P::Send_BuyForAgitReq(IN const PAIR_USHORT_DWORD prKey_)
{
    // ����Ʈ������ ���ſ�û
    NULL_CHECK(m_pkUserProxy);
    // prKey_.first : ������ ������ Index
    // prKey_.second : ������ ������ ItemID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_BUY_FOR_AGIT_REQ, prKey_);
}

bool KP2P::Send_SellForAgitReq(IN const std::vector<GCITEMUID>& vecSellItemUID_)
{
    // ����Ʈ������ �Ǹſ�û
    NULL_CHECK(m_pkUserProxy);
    // vecSellItemUID_ : �Ǹ��� �����۵��� ItemUID��(m_ItemUID) ä���� �����ּ���.
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SELL_FOR_AGIT_REQ, vecSellItemUID_);
}

bool KP2P::Send_SeedFlowerPotReq(IN const GCITEMUID& PotItemUID_, IN const GCITEMUID& SeedITemUID_, IN const int& nSeedType_)
{
    // ȭ�п� ���ѽɱ� ��û
    /*
    KFlowerPot::SEED_TYPE
    ����� �� �ִ� ���ѽɱ� Ÿ��
    KFlowerPot::ST_FAST     : ���ڰ�
    KFlowerPot::ST_NORMAL   : ����������
    KFlowerPot::ST_BEST     : ������
    */
    NULL_CHECK(m_pkUserProxy);
    KSeedFlowerPotReq kPacket;
    kPacket.m_kPotItem.m_ItemUID = PotItemUID_; // ���� ���� ȭ�� ItemUID
    kPacket.m_kSeedItem.m_ItemUID = SeedITemUID_; // ����� ���� ItemUID
    kPacket.m_nSeedType = nSeedType_; // ���� �ɴ� Ÿ�� (KFlowerPot::SEED_TYPE)
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SEED_FLOWER_POT_REQ, kPacket);
}

bool KP2P::Send_HarvestFlowerPotReq(IN const GCITEMUID& PotItemUID_, IN const bool& bBonus_)
{
    // ȭ�п��� �� ��Ȯ�ϱ�
    NULL_CHECK(m_pkUserProxy);
    KHarvestFlowerPotReq kPacket;
    kPacket.m_kPotItem.m_ItemUID = PotItemUID_; // ��Ȯ�� ȭ��ItemUID
    kPacket.m_bBonusFlower = bBonus_; // ���ʽ� ȹ�濩��
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_HARVEST_FLOWER_POT_REQ, kPacket);
}

bool KP2P::Send_AgitChatReq(IN const char& cChatType_, IN const std::wstring& strMsg_, IN const DWORD& dwChatColor_)
{
    // ����Ʈ ä�� ��û
    /*
    ����� �� �ִ� ä�� Ÿ��
    KChatData::MSG_COMMON: �Ϲ� ä��
    KChatData::MSG_COLOR_CHAT: �÷� ä��
    KChatData::MSG_ADMIN_COMMON: ��� ä��
    */
    NULL_CHECK(m_pkUserProxy);

    if (g_pkChatManager->CheckChatBlockUser())
        return false;

    KChatData kPacket;
    kPacket.m_cChatType = cChatType_; // ä�� Ÿ��
    kPacket.m_strChatMsg = strMsg_; // �޼��� (��������� ����������)
    kPacket.m_dwChatColor = dwChatColor_; // �÷�ä�ý� ����� ����
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_AGIT_CHAT_REQ, kPacket);
}

bool KP2P::Send_AgitBanUserReq(IN const DWORD& dwUserUID_)
{
    // ����Ʈ���� ���� �߹� ��û
    NULL_CHECK(m_pkUserProxy);
    KAgitBanUserReq kPacket;
    kPacket.m_dwBanUserUID = dwUserUID_; // �߹��� ����UID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_AGIT_BAN_USER_REQ, kPacket);
}

bool KP2P::Send_SetAgitObjectPosReq(IN const std::vector<KAgitObjectPos>& vecList_)
{
    if (vecList_.empty())
        return true;

    // ����Ʈ ������Ʈ ��ġ ��û
    NULL_CHECK(m_pkUserProxy);
    KSetAgitObjectPosReq kPacket;
    kPacket.m_vecObject = vecList_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SET_AGIT_OBJECT_POS_REQ, kPacket);
}

bool KP2P::Send_AgitFriendListReq(IN const std::set<DWORD>& setFriendUserUID_)
{
    // ����Ʈ ������Ʈ ��ġ ��û
    NULL_CHECK(m_pkUserProxy);
    KAgitFriendListReq kPacket;
    kPacket.m_setFriendUID = setFriendUserUID_; // ģ�� UserUID (�޽������� ��������)
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_AGIT_FRIEND_LIST_REQ, kPacket);
}

bool KP2P::Send_AgitTopRankListReq()
{
    // �α����Ʈ ����Ʈ ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_TOP_RANK_LIST_REQ);
}

bool KP2P::Send_AgitFavoriteListReq()
{
    // ����Ʈ ���ã�� ����Ʈ ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_FAVORITE_LIST_REQ);
}

bool KP2P::Send_AgitAddFavoriteReq(IN const DWORD& dwAgitUID_)
{
    // ����Ʈ ���ã�� �߰�
    NULL_CHECK(m_pkUserProxy);
    KAgitUserParam kPacket;
    kPacket.m_dwAgitUID = dwAgitUID_; // ���ã�⿡ �߰��� ����ƮUID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_AGIT_ADD_FAVORITE_REQ, kPacket);
}

bool KP2P::Send_AgitRemoveFavoriteReq(IN const DWORD& dwAgitUID_)
{
    // ����Ʈ ���ã�� ����
    NULL_CHECK(m_pkUserProxy);
    KAgitUserParam kPacket;
    kPacket.m_dwAgitUID = dwAgitUID_; // ���ã�⿡ ������ ����ƮUID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_AGIT_REMOVE_FAVORITE_REQ, kPacket);
}

bool KP2P::Send_RegisterAgitPageReq(IN const std::wstring& strAgitName_, IN const UCHAR& ucMaxUser_, IN const UCHAR& ucOpenType_, IN const std::wstring& strPassword_)
{
    // ����Ʈ ����Ʈ�� ��Ͽ�û
    NULL_CHECK(m_pkUserProxy);
    KRegisterAgitPageReq kPacket;
    kPacket.m_strName = strAgitName_; // ����Ʈ��(������ ������ ������ �־��ּ���)
    kPacket.m_ucMaxUser = ucMaxUser_; // ���������ο�(������ ������ ������ �־��ּ���)
    kPacket.m_ucOpenType = ucOpenType_; // ���¼���(������ ������ ������ �־��ּ���)
    kPacket.m_strPassword = strPassword_; // ��ȣ(������ ������ ������ �־��ּ���)
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_REGISTER_AGIT_PAGE_REQ, kPacket);
}

bool KP2P::Send_UnregisterAgitPageReq()
{
    // ����Ʈ ����Ʈ�� ������� ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_UNREGISTER_AGIT_PAGE_REQ);
}

bool KP2P::Send_AgitPageListReq(IN int nChannel_/*=0*/, IN bool bOpen_/*=true*/, IN bool bNotFull_/*=true*/, IN USHORT usMinUser_/*=0*/, IN USHORT usPageNum_/*=1*/)
{
    // ����Ʈ ������ ��û
    NULL_CHECK(m_pkUserProxy);
    KAgitPageList kPacket;
    kPacket.m_nSID = nChannel_; // ����Ʈ ä�� ID
    kPacket.m_bOpen = bOpen_; // ������ ����(�⺻��true)
    kPacket.m_bNotFull = bNotFull_; // �����������͸� ����(�⺻��true)
    kPacket.m_usMinUser = usMinUser_; // n�� �̻� ����(�⺻��:0)
    kPacket.m_usPageNum = usPageNum_; // ���� ������(���ʿ�1��,�� ���Ŀ� ���ϴ� ������ ��ȣ��)
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_AGIT_PAGE_LIST_REQ, kPacket);
}

bool KP2P::Send_BrownieStoreCatalogReq()
{
    // ����� ���� �����۸���Ʈ ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_BROWNIE_STORE_CATALOG_REQ);
}

bool KP2P::Send_BrownieStoreMaterialReq(IN const std::set<PAIR_USHORT_DWORD>& setItemID_)
{
    // ����� ���� ������ �������� ��û
    // setItemID_ : set[ pair[Index,ItemID] ]
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_BROWNIE_STORE_MATERIAL_REQ, setItemID_);
}

bool KP2P::Send_BuyForBrownieReq(IN const PAIR_USHORT_DWORD prKey_)
{
    // ����� ���� ������ ����
    // prKey_.first : ������ ������ Index
    // prKey_.second : ������ ������ ItemID
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_BUY_FOR_BROWNIE_REQ, prKey_);
}

bool KP2P::Send_EquipBrownieReq(IN const DWORD& dwBrownieID_, IN const GCITEMID& ItemID_, IN const GCITEMUID& ItemUID_)
{
    // ����� ���������� ����
    NULL_CHECK(m_pkUserProxy);
    KEquipBrownieReq kPacket;
    kPacket.m_kBrownieInfo.m_dwUID = dwBrownieID_; // ������� �����ID
    kPacket.m_kEquip.m_ItemID = ItemID_; // ���� ������ID
    kPacket.m_kEquip.m_ItemUID = ItemUID_; // ���� ������UID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_EQUIP_BROWNIE_REQ, kPacket);
}

bool KP2P::Send_WateringFairyTreeReq()
{
    // �����ǳ��� ���ֱ�
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_WATERING_FAIRY_TREE_REQ);
}

bool KP2P::Send_UseFairyTreeFruitReq()
{
    // �����ǳ��� ���ϸԱ�
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_USE_FAIRY_TREE_FRUIT_REQ);
}

bool KP2P::Send_AgitTutorialReq()
{
    // Ʃ�丮�� ���� ������ ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_TUTORIAL_REQ);
}

bool KP2P::Send_AgitTutorialDoneReq()
{
    // Ʃ�丮�� ���࿩�� ����
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_AGIT_TUTORIAL_DONE_REQ);
}

bool KP2P::Send_InviteAgitReq(IN const std::vector<DWORD>& vecUserUID_)
{
    // ����Ʈ �ʴ�
    NULL_CHECK(m_pkUserProxy);
    // vecUserUID_; // �ʴ��� ��� ����UID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_INVITE_AGIT_REQ, vecUserUID_);
};

bool KP2P::Send_GuestbookPageReq()
{
    // ����Ʈ ������ ����������Ʈ ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_GUESTBOOK_PAGE_REQ);
}

bool KP2P::Send_GuestbookWriteReq(IN const std::wstring& strMsg_)
{
    // ����Ʈ ������ ���� ����
    NULL_CHECK(m_pkUserProxy);
    KGuestBookWriteReq kPacket;
    kPacket.m_strMsg = strMsg_; // ���� �� ����
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_GUESTBOOK_WRITE_REQ, kPacket);
};

bool KP2P::Send_GuestbookDeleteReq(IN const std::set<int>& setDeleteNo_)
{
    // ����Ʈ ������ �� ����
    NULL_CHECK(m_pkUserProxy);
    KGuestBookDeleteReq kPacket;
    kPacket.m_setDeleteNo = setDeleteNo_; // ������ �۹�ȣ ����Ʈ
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_GUESTBOOK_DELETE_REQ, kPacket);
};

bool KP2P::Send_UseTrainingObjReq(IN const int& nCharType_, IN const GCITEMUID& ItemUID_)
{
    // ȥ�ü� ��� 
    NULL_CHECK(m_pkUserProxy);
    KUseTrainingObjReq kPacket;
    kPacket.m_nCharType = nCharType_; // �Ʒý�ų ĳ���� Ÿ��
    kPacket.m_TrainingObjUID = ItemUID_; // �Ʒü�ItemUID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_USE_TRAINING_OBJ_REQ, kPacket);
};

bool KP2P::Send_StrengthEquipReq(GCITEMUID dwEnchantItemUID_, GCITEMUID dwEquipItemID_, DWORD dwEquipItemType_)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_STRENGTH_EQUIP_REQ kPacket;
    kPacket.m_StrengthItemUID = dwEnchantItemUID_;
    kPacket.m_EquipItemUID = dwEquipItemID_;
    kPacket.m_dwEquipItemType = dwEquipItemType_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_STRENGTH_EQUIP_REQ, kPacket);
}

bool KP2P::Send_StrengthActionReq(KEVENT_STRENGTH_ACTION_REQ& kPacket_)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_STRENGTH_ACTION_REQ, kPacket_);
}

bool KP2P::Send_StrengthBreakUpReq(GCITEMUID dwEnchantItemUID_, GCITEMUID dwMaterialItemUID_)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_STRENGTH_BREAKUP_REQ kPacket;
    kPacket.m_StrengthItemUID = dwEnchantItemUID_;
    kPacket.m_BreakUpMaterialUID = dwMaterialItemUID_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_STRENGTH_BREAKUP_REQ, kPacket);
}

bool KP2P::Send_StrengthComposeReq(std::pair<GCITEMUID, GCITEMUID> prItemInfo_, GCITEMUID dwAssistItemUID_)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_STRENGTH_COMPOSE_REQ kPacket;
    kPacket.m_prItemInfo = prItemInfo_;
    kPacket.m_dwComposeAssistUID = dwAssistItemUID_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_STRENGTH_COMPOSE_REQ, kPacket);
}

bool KP2P::Send_InvenBuffItemListReq()
{
    // �κ��丮 ���������� ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_INVEN_BUFF_ITEM_LIST_REQ);
}

bool KP2P::Send_OpenAgitCharReq(const int& nCharType_)
{
    // ��ġĳ���� ���� ��û
    // nCharType_ : ������ ĳ���� Ÿ��
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_OPEN_AGIT_CHARACTER_REQ, nCharType_);
}

bool KP2P::Send_SetAgitCharPosReq(const std::vector<KSimpleAgitCharPos>& vecCharPos_)
{
    // ��ġĳ���� ��ġ ����
    // ��ġ�� ȸ���Ҷ��� x,y ��ǥ�� -1,-1 �� ������ ��.
    NULL_CHECK(m_pkUserProxy);
    KSetAgitCharPosReq kPacket;
    kPacket.m_vecCharPos = vecCharPos_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SET_AGIT_CHARACTER_POS_REQ, kPacket);
}

bool KP2P::Send_SetAgitCharMotionReq(const int& nCharType_, const int& nMotionID_)
{
    // ��ġĳ���� ��� ����
    NULL_CHECK(m_pkUserProxy);
    KSetAgitCharMotionReq kPacket;
    kPacket.m_nCharType = nCharType_; // ��ġ�� ĳ���� Ÿ��
    kPacket.m_nMotionID = nMotionID_; // ���ID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SET_AGIT_CHARACTER_MOTION_REQ, kPacket);
}

bool KP2P::Send_SetAgitCharCoordiReq(const int& nCharType_, const std::map<DWORD, GCITEMUID>& mapSlotItemUID_)
{
    // ��ġĳ���� �ڵ� ����
    NULL_CHECK(m_pkUserProxy);
    KEVENT_SET_AGIT_CHARACTER_COORDI_REQ kPacket;
    kPacket.m_nCharType = nCharType_; // ��ġ�� ĳ���� Ÿ��
    kPacket.m_mapSlotItemUID = mapSlotItemUID_; // SlotID, �ڵ������UID
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SET_AGIT_CHARACTER_COORDI_REQ, kPacket);
}

bool KP2P::Send_SetAgitCharSpeechReq(const int& nCharType_, const std::map<USHORT, std::wstring>& mapSpeech_)
{
    // ��ġĳ���� ��� ����
    NULL_CHECK(m_pkUserProxy);
    KSetAgitCharSpeechReq kPacket;
    kPacket.m_nCharType = nCharType_; // ��ġ�� ĳ���� Ÿ��
    kPacket.m_mapSpeech = mapSpeech_; // map[ type, Speech ]
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SET_AGIT_CHARACTER_SPEECH_REQ, kPacket);
}

bool KP2P::Send_FairyTreeLvTableReq()
{
    // �����ǳ��� �������̺� ��û
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_FAIRY_TREE_LV_TABLE_REQ);
}

bool KP2P::Send_SurveyRewardReq( KEVENT_SURVEY_REWARD_REQ& kPacket_ )
{
    // 설문 답변 전달(설문 응답 보상 요청)
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SURVEY_REWARD_REQ, kPacket_ );
}

bool KP2P::Send_GachaNoticeReadCheckReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_UPDATE_GACHA_POPUP_INFO );
}

bool KP2P::Send_EventSHAEnableReq()
{
    //SHA ON/OFF 여부
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SHA_ENABLE_REQ );
}

bool KP2P::Send_GWCRakingRewardReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GWC_RANKING_REWARD_REQ );
}

bool KP2P::Send_GWCRakingListReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_GWC_RANKING_LIST_REQ );
}

bool KP2P::Send_EventJustInTimeRewardReq( KEVENT_JUST_IN_TIME_REWARD_REQ& kPacket_)
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JUST_IN_TIME_REWARD_REQ, kPacket_ );
}

bool KP2P::Send_CharismasEventInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CHARISMAS_EVENT_INFO_REQ );
}

bool KP2P::Send_CharismasEventConnectCheckReq( IN const std::wstring wstrNickName_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHARISMAS_EVENT_CONNECT_CHECK_REQ, wstrNickName_ );
}

bool KP2P::Send_CharismasEventPresentReq( KEVENT_CHARISMAS_EVENT_PRESENT_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHARISMAS_EVENT_PRESENT_REQ, kPacket_ );
}

bool KP2P::Send_CharismasEventDonationReq( KEVENT_CHARISMAS_EVENT_DONATION_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHARISMAS_EVENT_DONATION_REQ, kPacket_ );
}

bool KP2P::Send_CharismasEventGradeRewardReq( KEVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHARISMAS_EVENT_GRADE_REWARD_REQ, kPacket_ );
}

bool KP2P::Send_SockTreeReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SOCKS_INFO_REQ );

}

bool KP2P::Send_GetPostLetterListReq( IN const int& nPageNo_, IN char cLetterListType_/*=KEVENT_GET_POST_LETTER_LIST_REQ::LL_ALL*/ )
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_GET_POST_LETTER_LIST_REQ kPacket;
    kPacket.m_nPostPageNo = nPageNo_;
    kPacket.m_cLetterListType = cLetterListType_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_GET_POST_LETTER_LIST_REQ, kPacket);
}

bool KP2P::Send_SendLetterReq( IN std::wstring strToNickName_, IN std::wstring strTitle_, IN std::wstring strMessage_, IN std::vector<GCITEMUID> vecItemUID_, IN std::map<GCITEMUID,int> mapCountItemUID_, IN int nGamePoint_/*=0*/ )
{
    // 편지보내기
    NULL_CHECK( m_pkUserProxy );
    KEVENT_SEND_LETTER_REQ kPacket;
    kPacket.m_strToNickName = strToNickName_; // 받는 유저 NickName
    kPacket.m_strTitle = strTitle_; // 제목
    kPacket.m_strMessage = strMessage_; // 본문
    kPacket.m_vecItemUID = vecItemUID_; // 첨부 영구 ItemUID들
    kPacket.m_mapCountItemUID = mapCountItemUID_; // 첨부 수량ItemUID, 수량
    kPacket.m_nPostGP = nGamePoint_; // 첨부 GP
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SEND_LETTER_REQ, kPacket );
}

bool KP2P::Send_ReadLetterReq( IN const POSTUID& PostUID_ )
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_READ_LETTER_REQ, PostUID_);
}

bool KP2P::Send_GetItemFromLetterReq( IN const POSTUID& PostUID_ )
{
    // 편지 첨부물 받기 요청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_GET_ITEM_FROM_LETTER_REQ kPacket;
    kPacket.m_PostUID = PostUID_; // 첨부물 받을 편지 번호
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GET_ITEM_FROM_LETTER_REQ, kPacket );
}

bool KP2P::Send_DeleteLetterReq( IN const std::vector<POSTUID>& vecPostUID_ )
{
    // 편지 삭제 요청
    NULL_CHECK( m_pkUserProxy );
    // vecPostUID_ : 삭제할 편지번호 리스트
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DELETE_LETTER_REQ, vecPostUID_ );
}

bool KP2P::Send_RenewalLetterReq()
{
    // 편지함 갱신요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_RENEWAL_LETTER_REQ );
}

bool KP2P::Send_EclipsePlotEventInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ECLIPSE_PLOT_INFO_REQ );
}

bool KP2P::Send_EclipsePlotEventTimeRewardReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ECLIPSE_PLOT_TIME_REWARD_REQ );
}

bool KP2P::Send_EclipsePlotEventHuntRewardReq( KEVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ECLIPSE_PLOT_HUNT_REWARD_REQ, kPacket_ );
}

bool KP2P::Send_EclipsePlotEventFinalRewardReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ECLIPSE_PLOT_FINAL_REWARD_REQ);
}

bool KP2P::Send_EclipseCollectInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ECLIPSE_COLLECT_INFO_REQ );
}
bool KP2P::Send_EclipseCollectRewardReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ECLIPSE_COLLECT_REWARD_REQ );
}


bool KP2P::Send_SSkillHotKeyUsedInfo( KEVENT_STAT_HOTKEY_USED_INFO_NOT& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    g_kGlobalValue.ClearSSkillHotKeyUsedInfo();        
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_STAT_HOTKEY_USED_INFO_NOT, kPacket_ );
}

bool KP2P::Send_DungeonClearRewardReq( IN const int& nModeID_, IN const std::vector<INDEX_GCITEMID>& vecIndexItemID_ )
{
    // 던전 클리어보상 아이템받기 요청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_DUNGEON_CLEAR_REWARD_REQ kPacket;
    kPacket.m_nModeID = nModeID_; // 던전ID
    kPacket.m_vecIndexItemID = vecIndexItemID_; // 보상받을 아이템 Index + ItemID
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_CLEAR_REWARD_REQ, kPacket );
}

bool KP2P::Send_EquipLevelDownReq( IN const GCITEMUID& EquipItemUID_, IN const GCITEMUID& MaterialItemUID_ )
{
    // 아이템 장착레벨 낮추기 요청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_EQUIP_LEVEL_DOWN_REQ kPacket;
    kPacket.m_EquipItemUID = EquipItemUID_; // 장착레벨 낮출 아이템 ItemUID
    kPacket.m_MaterialItemUID = MaterialItemUID_; // 재료로 사용할 아이템 ItemUID
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_EQUIP_LEVEL_DOWN_REQ, kPacket );
}

bool KP2P::Send_MonsterCardMixReq( IN const std::map<GCITEMUID,int>& mapList_ )
{
    // 몬스터카드 조합요청
    NULL_CHECK( m_pkUserProxy );
    KEVENT_MONSTER_CARD_MIX_REQ kPacket;
    kPacket.m_mapMaterialItemCount = mapList_; // map[ 재료가될 몬스터카드 ItemUID, 사용할 재료 수량 ]
    kPacket.m_vecMaterialItem.clear(); // 서버에서 설정됨.
    kPacket.m_vecRewards.clear(); // 서버에서 설정됨.
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MONSTER_CARD_MIX_REQ, kPacket );
}

bool KP2P::Send_UseExpPotionReq( IN const DWORD dwItemID_, IN const GCITEMUID dwItemUID_, IN const char cCharType_ )
{
    // 경험치 포션 사용 요청
    NULL_CHECK( m_pkUserProxy );
    KExpPotionReq kPacket;
    kPacket.m_kItem.m_dwID      = dwItemID_ * 10;
    kPacket.m_kItem.m_dwUID     = dwItemUID_;
    kPacket.m_cCharType         = cCharType_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_EXP_POTION_REQ, kPacket );
}

bool KP2P::Send_ExpPotionListReq()
{
    // 편지함 갱신요청
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_EXP_POTION_LIST_REQ );
}

bool KP2P::Send_DepotInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_DEPOT_INFO_REQ );
}

bool KP2P::Send_DepotInfoByCharTabReq( KDepotInfo& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DEPOT_CHAR_TAB_INFO_REQ, kPacket_ );
}

bool KP2P::Send_DepotInsertItemReq( KDepotItem& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DEPOT_INSERT_ITEM_REQ, kPacket_ );

}

bool KP2P::Send_DepotDeleteItemReq( KDepotItem& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DEPOT_DELETE_ITEM_REQ, kPacket_ );

}

bool KP2P::Send_DepotMoveItem( KEVENT_DEPOT_MOVE_ITEM_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DEPOT_MOVE_ITEM_REQ, kPacket_ );
}

bool KP2P::Send_DepotExtendReq( KEVENT_DEPOT_EXTEND_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DEPOT_EXTEND_REQ, kPacket_ );
}

bool KP2P::Send_DepotDBUpdateReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_UPDATE_DEPOT_DATA_REQ );
}

bool KP2P::Send_AddSkillSlotOpenReq( IN const GCITEMID& ItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_ADD_SKILL_SLOT_OPEN_REQ kPacket;
    kPacket.m_ItmeID = ItemID_;
    kPacket.m_cCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_SKILL_SLOT_OPEN_REQ, kPacket );
}

bool KP2P::Send_MonsterCardMixInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_MONSTER_CARD_MIX_INFO_REQ );
}

// 송크란 미니게임 기본 설정 요청
bool KP2P::Send_SongkranEventScriptInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SONGKRAN_SCRIPT_INFO_REQ );
}

// 송크란 미니게임 기본 설정 요청
bool KP2P::Send_SongkranEventUserInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SONGKRAN_USER_INFO_REQ );
}
// 송크란 미니게임 점수 갱신 요청
bool KP2P::Send_SongkranEventWaterbombUseReq( DWORD dwScoreDiff_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SONGKRAN_WATERBOMB_USE_REQ, dwScoreDiff_ );
}
// 송크란 미니게임 보상 아이템 요청
bool KP2P::Send_SongkranEventGradeRewardReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SONGKRAN_GRADE_REWARD_REQ );
}

bool KP2P::Send_SongkranEventWaterBombExchangeReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SONGKRAN_WATERBOMB_EXCHANGEITEM_USE_REQ );
}

bool KP2P::Send_GcAdventureDateReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ADVENTURE_DATA_REQ );
}

bool KP2P::Send_GcAdventureContinentRewardReq( KEVENT_ADVENTURE_CONTINENT_REWARD_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADVENTURE_CONTINENT_REWARD_REQ , kPacket_ );
}

bool KP2P::Send_GcAdventureWholeContinentFinalRewardReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_ADVENTURE_FINAL_REWARD_REQ);
}

bool KP2P::Send_LevelResetItemReq( const char& cCharType, GCITEMID itemID )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_LEVEL_RESET_EVENT_ITEM_REQ kPacket;
    kPacket.m_cCharType = cCharType;
    kPacket.m_ItemID = itemID * 10;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_LEVEL_RESET_EVENT_ITEM_REQ, kPacket );
}

bool KP2P::Send_AddItem( const GCITEMID& ItemID, const int nPeriod, const int nDuraiton, const int nGrade, const int nItemLevel )
{
    NULL_CHECK( m_pkUserProxy );
    
    KEVENT_TEST_ADD_ITEM_REQ kPacket;
    kPacket.m_itemID = ItemID;
    kPacket.m_nPeriod = nPeriod;
    kPacket.m_nDuration = nDuraiton;
    kPacket.m_nGrade = nGrade;
    kPacket.m_nItemLevel = nItemLevel;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_TEST_ADD_ITEM_REQ, kPacket );
}

bool KP2P::Send_OlympicJoinGameReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_OLYMPIC_JOIN_GAME_REQ );
}

bool KP2P::Send_OlympicGawibawiboReq( const time_t tmTodayDate, const int nGameType, const int nTeamType )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ kPacket;   
    kPacket.m_tmTodayDate = tmTodayDate;
    kPacket.m_nGameType = nGameType;
    kPacket.m_nTeamType = nTeamType;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ, kPacket );
}

bool KP2P::Send_OlympicDicePlayReq( const time_t tmTodayDate, const int nGameType, const int nTeamType )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_OLYMPIC_DICEPLAY_RESULT_REQ kPacket;   
    kPacket.m_tmTodayDate = tmTodayDate;
    kPacket.m_nGameType = nGameType;
    kPacket.m_nTeamType = nTeamType;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_OLYMPIC_DICEPLAY_RESULT_REQ, kPacket );
}

bool KP2P::Send_OlympicWaterbombReq( const int nScore, const time_t tmTodayDate, const int nGameType, const int nTeamType )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_OLYMPIC_WATERBOMB_RESULT_REQ kPacket;
    kPacket.m_nIncreasedData = nScore;
    kPacket.m_tmTodayDate = tmTodayDate;
    kPacket.m_nGameType = nGameType;
    kPacket.m_nTeamType = nTeamType;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_OLYMPIC_WATERBOMB_RESULT_REQ, kPacket );
}

bool KP2P::Send_OlympicInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_OLYMPIC_INFO_REQ );
}

bool KP2P::Send_OlympicScheduleReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_OLYMPIC_SCHEDULE_REQ );
}

bool KP2P::Send_OlympicGradeRewardReq( KEVENT_OLYMPIC_REWARD_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_OLYMPIC_REWARD_REQ, kPacket_ );
}

bool KP2P::Send_OlympicTreeDropResultReq( KEVENT_OLYMPIC_TREEDROP_RESULT_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_OLYMPIC_TREEDROP_RESULT_REQ, kPacket_ );
}

bool KP2P::Send_OlympicBalloonResultReq( KEVENT_OLYMPIC_BALLOON_RESULT_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_OLYMPIC_BALLOON_RESULT_REQ, kPacket_ );
}

bool KP2P::Send_NewCharChoiceReq( IN const char& cCharType_, IN const std::wstring& strNickName )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_NEW_CHAR_CHOICE_REQ kPacket;
    kPacket.m_cCharType = cCharType_;
    kPacket.m_wstrNickName = strNickName;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_NEW_CHAR_CHOICE_REQ, kPacket );
}

bool KP2P::Send_ItemComposeReq( IN const std::vector<GCITEMUID>& vecInfo_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ITEM_COMPOSE_REQ, vecInfo_ );
}

bool KP2P::Send_GachaLotteryActionReq( IN const char& cCharType_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_GACHA_LOTTERY_ACTION_REQ, cCharType_ );
}

bool KP2P::Send_GetItemBreakupInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID( KUserEvent::EVENT_SPECIFIC_ITEM_BREAKUP_INFO_REQ );
}

bool KP2P::Send_MatchInviteFriendListReq()
{
    return m_pkUserProxy->SendID( KUserEvent::EVENT_MATCH_INVITE_FRIEND_LIST_REQ );
}

bool KP2P::Send_PartyUserReadyReq(DWORD dwState, char cCharType)
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_PARTY_USER_READY_REQ kPacket;
    kPacket.m_dwState = dwState;
    kPacket.m_cCharType = cCharType;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_PARTY_USER_READY_REQ, kPacket );
}

bool KP2P::Send_AddMatchReq(int nModeInfo, char cCharType, int nExceptModeID )
{
    NULL_CHECK( m_pkUserProxy );

    //int                                 m_nModeInfo; // 선택한 모드 정보.
    //char                                m_cCharType; // 선택한 캐릭터 정보.
    //int                                 m_nExceptModeID; // 제외할 게임모드 정보.
    KEVENT_ADD_MATCH_REQ kPacket;
    kPacket.m_nModeInfo = nModeInfo;
    kPacket.m_cCharType = cCharType;
    kPacket.m_nExceptModeID = nExceptModeID;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_MATCH_REQ, kPacket );
}

bool KP2P::Send_CurrentMatchModeCountReq( )
{
    return m_pkUserProxy->SendID( KUserEvent::EVENT_CURRENT_MATCH_MODE_COUNT_REQ );
}

bool KP2P::Send_DelMatchReq( )
{
    return m_pkUserProxy->SendID( KUserEvent::EVENT_DEL_MATCH_REQ );
}

bool KP2P::Send_AcceptMatchReq( bool bOk, KInDoorUserInfo userInfo )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_ACCEPT_MATCH_REQ kPacket;

    kPacket.m_bAccept = bOk;
    kPacket.m_kIndoorUserInfo = userInfo;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ACCEPT_MATCH_REQ, kPacket, true, true );
}

bool KP2P::Send_StartGamePossibleReq( )
{
    return m_pkUserProxy->SendID( KUserEvent::EVENT_START_GAME_POSSIBLE_REQ );
}

bool KP2P::Send_MatchModeStateReq( )
{
    return m_pkUserProxy->SendID( KUserEvent::EVENT_MATCH_MODE_STATE_REQ );
}

bool KP2P::Send_HeroItemUpgradeInfoReq( )
{
	return m_pkUserProxy->SendID( KUserEvent::EVENT_HERO_ITEM_UPGRADE_INFO_REQ );
}

bool KP2P::Send_HeroItemUpgradeReq( std::pair<DWORD, int>& prKey, std::vector<GCITEMUID>& vecMaterials )
{
    NULL_CHECK( m_pkUserProxy );

	KEVENT_HERO_ITEM_UPGRADE_REQ kPacket;

    kPacket.m_prKey = prKey;
    kPacket.m_vecMaterials = vecMaterials;

	return m_pkUserProxy->SendPacket( KUserEvent::EVENT_HERO_ITEM_UPGRADE_REQ, kPacket );
}

bool KP2P::Send_RitasChristmas_PlayInfo_Req()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent::EVENT_RITAS_CHRISTMAS_PLAY_INFO_REQ);
}

bool KP2P::Send_RitasChristmas_RankInfo_Req()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent::EVENT_RITAS_CHRISTMAS_RANK_INFO_REQ);
}

bool KP2P::Send_RitasChristmas_StageStart_Req( const KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RITAS_CHRISTMAS_STAGE_START_REQ, kPacket_ );
}

bool KP2P::Send_RitasChristmas_EmptyBasket_Req( const KEVENT_RITAS_CHRISTMAS_EMPTY_BASKET_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RITAS_CHRISTMAS_EMPTY_BASKET_REQ, kPacket_ );
}

bool KP2P::Send_RitasChristmas_StageEnd_Req( const KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RITAS_CHRISTMAS_STAGE_END_REQ, kPacket_ );
}

bool KP2P::Send_RitasChristmas_GetReward_Req( const KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ, kPacket_ );
}

bool KP2P::Send_RitasChristmas_ExchangeContinueCoin_Req()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent::EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ);
}

bool KP2P::Send_RitasChristmas_UserInfo_Req()
{
	NULL_CHECK( m_pkUserProxy );
	return m_pkUserProxy->SendID(KUserEvent::EVENT_RITAS_CHRISTMAS_USER_INFO_REQ);
}

bool KP2P::Send_RitasChristmas_RewardItemList_Req()
{
	NULL_CHECK( m_pkUserProxy );
	return m_pkUserProxy->SendID(KUserEvent:: EVENT_RITAS_CHRISTMAS_REWARD_VIEW_INFO_REQ);

}

bool KP2P::Send_Match_Rank_Page_Req( char cRankType, char cCharType, char cPageNum )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_MATCH_RANK_PAGE_REQ kPacket;

    kPacket.m_cRankType = cRankType;
    kPacket.m_cCharType = cCharType;
    kPacket.m_cPageNum = cPageNum;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MATCH_RANK_PAGE_REQ, kPacket );
}

bool KP2P::Send_Match_Rank_Search_Req( char cRankType, char cCharType, std::wstring strNickName )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_MATCH_RANK_SEARCH_REQ kPacket;

    kPacket.m_cRankType = cRankType;
    kPacket.m_cCharType = cCharType;
    kPacket.m_strNickName = strNickName;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MATCH_RANK_SEARCH_REQ, kPacket );
}

bool KP2P::Send_My_Match_Rank_Info_Req()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_MY_MATCH_RANK_INFO_REQ);
}

bool KP2P::SendClientHardwareInfo( const KEVENT_STAT_CLIENT_INFO& kStatClientInfo )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_STAT_CLIENT_INFO, kStatClientInfo );
}

bool KP2P::Send_BuffDonationReq( const GCITEMID dwDonationItemID_, int nDonationCount_ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_BUFF_DONATION_REQ kPacket;

    kPacket.m_dwDonationItemID = dwDonationItemID_;
    kPacket.m_nItemCount = nDonationCount_;

    return m_pkUserProxy->SendPacket(KUserEvent:: EVENT_BUFF_DONATION_REQ, kPacket);
}

bool KP2P::Send_AccumulateAttendanceInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_ACCUMULATE_ATTENDANCE_INFO_REQ);
}

bool KP2P::Send_AccumulateAttendanceRewardReq( const int& nRewardReqDayIndex_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket(KUserEvent:: EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ, nRewardReqDayIndex_);
}

bool KP2P::Send_CharSelectJoinReq( )
{
	NULL_CHECK( m_pkUserProxy );
	return m_pkUserProxy->SendID(KUserEvent:: EVENT_CHAR_SELECT_JOIN_REQ);

}

bool KP2P::Send_CashbackExtraRatioInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_CASHBACK_EXTRA_RATIO_INFO_REQ);
}

bool KP2P::Send_ErrandInitInfoReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_ERRAND_INIT_INFO_REQ);
}

bool KP2P::Send_ErrandRewardReq( const int& nSuccessCount_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket(KUserEvent:: EVENT_ERRAND_REWARD_REQ, nSuccessCount_);
}

bool KP2P::Send_ErrandGameEndReq( const int& nSuccessCount_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket(KUserEvent:: EVENT_ERRAND_GAME_END_REQ, nSuccessCount_);
}

bool KP2P::Send_ErrandUserDataReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent::EVENT_ERRAND_USER_INFO_REQ);
}

bool KP2P::Send_LookInventoryExtendReq( IN const GCITEMID& ItemID_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_LOOK_INVENTORY_EXTEND_REQ kPacket;
    kPacket.m_UseItemID = ItemID_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_LOOK_INVENTORY_EXTEND_REQ, kPacket);
}

bool KP2P::Send_SubjectRewardReq(const KEVENT_DUNGEON_SUBJECT_REWARD_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_SUBJECT_REWARD_REQ, kPacket_ );
}


bool KP2P::Send_SystemGuideItemReq( const int& nGuideType_, const int& nCharType_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_SYSTEM_GUIDE_ITEM_REQ kPacket;
    kPacket.m_nGuideType = nGuideType_;
    kPacket.m_nCharType = nCharType_;
    return m_pkUserProxy->SendPacket(KUserEvent:: EVENT_SYSTEM_GUIDE_ITEM_REQ, kPacket);
}

bool KP2P::Send_SystemGuideCompleteReq( const int& nGuideType_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket(KUserEvent:: EVENT_SYSTEM_GUIDE_COMPLETE_REQ, nGuideType_);
}

bool KP2P::Send_TutorialClearNOT()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_NEW_USER_TUTORIAL_CLEAR_NOT);
}

bool KP2P::Send_AttributeMigrationInfoReq( IN const GCITEMUID& itemUID_ )
{
    NULL_CHECK( m_pkUserProxy );
    GCITEMUID kPacket = itemUID_;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ATTRIBUTE_MIGRATION_INFO_REQ, kPacket );
}

bool KP2P::Send_AttributeMigrationSelectReq( IN const GCITEMUID& ItemUID_, IN const std::vector< KAttributeInfo >& vecAddAttributeList_, IN const std::vector< KAttributeInfo >& vecDelAttributeList_ )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_ATTRIBUTE_MIGRATION_SELECT_REQ kPacket;
    kPacket.m_ItemUID = ItemUID_;
    kPacket.m_vecAddAttributeList = vecAddAttributeList_;
    kPacket.m_vecDelAttributeList = vecDelAttributeList_;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ, kPacket);
}

bool KP2P::Send_CYOU_UserAgreement()
{
    NULL_CHECK( m_spNUserProxy );
    return m_spNUserProxy->SendID(KNUserEvent::ENU_USER_AGREEMENT_REQ );
}

bool KP2P::Send_CYOU_GetWebPoint_Req()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent::EVENT_CYOU_GET_WEB_POINT_REQ);

}

bool KP2P::Send_CYOU_CashChangeReq( DWORD dwPoint )
{
	NULL_CHECK( m_pkUserProxy );

	KEVENT_CYOU_CASH_CHANGE_REQ kPacket;

	kPacket.m_dwPoint = dwPoint;
	return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CYOU_CASH_CHANGE_REQ, kPacket );
}

bool KP2P::Send_AddSlotReq( IN const GCITEMID& ItemID_ )
{
    NULL_CHECK( m_pkUserProxy );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_ADD_SLOT_REQ, ItemID_ );
}

bool KP2P::Send_ChangeCharReq( IN const char cPrevChar_, IN const char cNextChar_ )
{
    NULL_CHECK( m_pkUserProxy );

    KEVENT_CHANGE_CHARACTER_INFO_REQ kPacket;
    kPacket.m_cNextChar = cNextChar_;
    kPacket.m_cPrevChar = cPrevChar_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CHANGE_CHARACTER_INFO_REQ, kPacket );
}

bool KP2P::Send_VitalitySystem_Recharge_Req( IN const int nCharType_ )
{
    NULL_CHECK( m_pkUserProxy );

    int kPacket = nCharType_;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_VITALITY_RECHARGE_REQ, kPacket );

    /*
    Result_ReceiveVitalitySystemRecharge = INT_MAX;
    KP2P::GetInstance()->Send_VitalitySystem_Recharge_Req();
    g_MyD3D->WaitForServerAck( Result_ReceiveVitalitySystemRecharge, INT_MAX, 3000, TIME_OUT_VALUE );
    */
}

bool KP2P::Send_CYOU_UserLogin_Req( IN const char cPrevCharType_, IN const char cNextCharType_ )
{ // 게임 서버 로그인 후 캐릭터 선택창에서 최초로 클라이언트 선택한 경우 1회만 이 패킷을 보낸다.
    NULL_CHECK( m_pkUserProxy );

    PAIR_CHAR_CHAR kPacket;
    kPacket = std::pair<char,char>( cPrevCharType_, cNextCharType_ );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_CYOU_LOGIN_REQ, kPacket );
}

bool KP2P::Send_Giftbox_NewUser_Reward_Req()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_GIFT_REQ);
}

bool KP2P::Send_Giftbox_EventUser_Reward_Req()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_GIFT_REQ);
}

bool KP2P::Send_JumpingCharReq( const char& cCharType )
{
    NULL_CHECK( m_pkUserProxy );
    char kPacket = cCharType;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_JUMPING_CHAR_REQ, kPacket );
}

bool KP2P::Send_JumpingCharRewardReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_JUMPING_CHAR_REWARD_REQ);
}
bool KP2P::Send_JoinRoomDivideReq( const int& nMaxIndex, const int& nCurrentIndex )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_JOIN_ROOM_INFO_DIVIDE_REQ kPacket;
    kPacket.m_nMaxIndex = nMaxIndex;
    kPacket.m_nCurrentIndex = nCurrentIndex;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_JOIN_ROOM_INFO_DIVIDE_REQ, kPacket);
}

bool KP2P::Send_UserRelaySendCountReq( const int nGameMode, const char cCharType, int nTRCount, int nURCount )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_USER_RELAY_SEND_COUNT_REQ kPacket;
    kPacket.m_nModeID = nGameMode;
    kPacket.m_cCharType = cCharType;
    
    kPacket.m_mapRelaySendCount.insert( std::pair<int,int>( KEVENT_USER_RELAY_SEND_COUNT_REQ::RT_TCP, nTRCount ) );
    kPacket.m_mapRelaySendCount.insert( std::pair<int,int>( KEVENT_USER_RELAY_SEND_COUNT_REQ::RT_UDP, nURCount ) );

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_USER_RELAY_SEND_COUNT_REQ, kPacket);
}

bool KP2P::Send_CoordiComposeReq( GCITEMUID _AbilityItemUID, GCITEMUID _DesignItemUID )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_COORDI_COMPOSE_REQ kPacket;
    kPacket.m_AbilityItemUID = _AbilityItemUID;
    kPacket.m_DesignItemUID = _DesignItemUID;  


    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_COORDI_COMPOSE_REQ, kPacket);
}

bool KP2P::Send_MoveItemToInventoryFromVirtualDepot( const KEVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_MOVE_ITEM_TO_INVENTORY_FROM_VIRTUAL_DEPOT_REQ, kPacket_ );
}

bool KP2P::Send_RelayServerStatusReq()
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_RELAY_SERVER_STATUS_REQ);
}

bool KP2P::Send_TongDonationInfoReq( const KEVENT_TONG_DONATION_INFO_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_TONG_DONATION_INFO_REQ, kPacket_ );
}

bool KP2P::Send_TongDonationDonateReq( const KEVENT_TONG_DONATION_DONATE_REQ& kPacket_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_TONG_DONATION_DONATE_REQ, kPacket_ );
}

bool KP2P::Send_SocialCommerceInfoReq() // 소셜커머스(보부상) 전체 정보 요청. UI열 때 마다 요청
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent::EVENT_SOSCIAL_COMMERCE_INFO_REQ);
}

bool KP2P::Send_CoordiGradeUpgradeInfoReq()
{   
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendID(KUserEvent:: EVENT_COORDI_GRADE_UPGRADE_INFO_REQ);
}

bool KP2P::Send_CoordiGradeUpgradeReq( const GCITEMUID& _itemUID, const GCITEMID& _CuponItemID, const char& _cGrade )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_COORDI_GRADE_UPGRADE_REQ kPacket;
    kPacket.m_ItemUID = _itemUID;
    kPacket.m_ItemID = _CuponItemID;  
    kPacket.m_cGrade = _cGrade;  

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_COORDI_GRADE_UPGRADE_REQ, kPacket);
}

bool KP2P::Send_SkillScrollTrainingReq( const int& _iSkillID, const GCITEMUID& m_ItemUID )
{
    KEVENT_SKILL_SCROOL_TRAINING_REQ kPacket;
    kPacket.m_nSkillID = _iSkillID;
    kPacket.m_ItemUID  = m_ItemUID;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_SKILL_SCROOL_TRAINING_REQ, kPacket);
}

bool KP2P::SendNickNameValidityCheckReq( IN const std::wstring& wstrNick_ )
{
    NULL_CHECK( m_pkUserProxy );
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_NICKNAME_VALIDITY_CHECK_REQ, wstrNick_ );
}

bool KP2P::Send_InfinityDungeonMonsterSummonReq( int _nCurrentTopRank, DWORD _dwRoundSeq, const std::vector<PAIR_INT>& _vecSummonMonsters, int _nSummonType )
{
    NULL_CHECK( m_pkUserProxy );
    KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ kPacket;
    kPacket.m_nCurrentTopRank = _nCurrentTopRank;
    kPacket.m_dwRoundSequence = _dwRoundSeq;
    kPacket.m_vecSummonMonsters = _vecSummonMonsters;
    kPacket.m_nSummonType = _nSummonType;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ, kPacket);
}

bool KP2P::Send_InfinityDungeonRewardExpReq( int _nMonUID )
{
    NULL_CHECK( m_pkUserProxy );
    int kPacket = _nMonUID;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_INFINITY_DUNGEON_REWARD_EXP_REQ, kPacket);
}

bool KP2P::Send_InfinityDungeonRewardItemReq( int _nDropUID, int _nUserUID )
{
    NULL_CHECK( m_pkUserProxy );
    std::pair<int,int> kPacket;
    kPacket.first = _nDropUID;
    kPacket.second = _nUserUID;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_INFINITY_DUNGEON_REWARD_ITEM_REQ, kPacket);
}

bool KP2P::Send_InfinityDungeonRewardGPNot( IN const std::set<DWORD>& setGpUID_ )
{
    // 던전 스테이지 넘어갈때마다 획득한 GP코인 정보를 방장이 서버로 알림.
    // setGpUID_ : 획득한 GP UID
    if ( setGpUID_.empty() ) return true;
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_INFINITY_DUNGEON_REWARD_GP_NOT, setGpUID_ );
}


bool KP2P::Send_DungeonMissionRegistReq( DWORD dwMissionID_ )
{
    NULL_CHECK( m_pkUserProxy );

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_DUNGEON_MISSION_REGIST_REQ, dwMissionID_ );
}

bool KP2P::Send_DungeonCurrentSeasonUserRankReq( int nModeID )
{
    NULL_CHECK( m_pkUserProxy );
    
    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_RANK_CURRENT_SEASON_USER_RANK_REQ, nModeID );
}

bool KP2P::Send_DungeonCurrentSeasonRankPageReq( int nModeID, int nPageNum )
{
    NULL_CHECK( m_pkUserProxy );

    std::pair<int,int> kPacket;
    kPacket.first = nModeID;
    kPacket.second = nPageNum;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_RANK_CURRENT_SEASON_RANK_PAGE_REQ, kPacket );
}

bool KP2P::Send_DungeonPreviousSeasonUserRankReq( int nModeID )
{
    NULL_CHECK( m_pkUserProxy );

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_RANK_PREVIOUS_SEASON_USER_RANK_REQ, nModeID );
}

bool KP2P::Send_DungeonPreviousSeasonRankPageReq( int nModeID, int nPageNum )
{
    std::pair<int,int> kPacket;
    kPacket.first = nModeID;
    kPacket.second = nPageNum;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_DUNGEON_RANK_PREVIOUS_SEASON_RANK_PAGE_REQ, kPacket );
}

bool KP2P::Send_SingleRandomAttributeRandomReq( int _nSlotID, GCITEMUID _itemUID, GCITEMID _UseItemID )
{
    NULL_CHECK( m_pkUserProxy );  

    KEVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ kPacket;
    kPacket.m_nSlotID = _nSlotID;
    kPacket.m_dwTargetItemUID = _itemUID;
    kPacket.m_dwSingleRandomAttributeItemID = _UseItemID;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_SINGLE_RANDOM_ATTRIBUTE_RANDOM_REQ, kPacket );
}

//GachaPon
bool KP2P::Send_GachaPongPlayInfoReq(DWORD dwMachineNumber)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_GACHA_PONG_PLAY_INFO_REQ, dwMachineNumber);
}

bool KP2P::Send_GachaPongPlayInfoUnsubscribeReq()
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendID(KUserEvent::EVENT_GACHA_PONG_PLAY_INFO_UNSUBSCRIBE_REQ);
}

bool KP2P::Send_GachaPongPlayInfoActionReq(DWORD dwMachineNumber, GCITEMID dwGoodsID)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_GACHA_PONG_ACTION_REQ, std::make_pair(dwMachineNumber, dwGoodsID));
}

bool KP2P::Send_PvpRoomListSearchReq(const KRoomOptions& options, char cRoomType)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_PVP_ROOM_LIST_SEARCH_REQ kPacket;

    kPacket.m_kRoomOptions = options;
    kPacket.m_cRoomType = cRoomType;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_PVP_ROOM_LIST_SEARCH_REQ, kPacket);
}

bool KP2P::Send_PvpRoomExtraOptionReq(const KRoomOptions& options)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_CHANGE_PVP_ROOM_EXTRA_OPTION_REQ, options);
}

bool KP2P::Send_CoordiViewSlotGetReq(IN const int CharType, IN const int LoginUID)
{
    NULL_CHECK(m_pkUserProxy);

    KEVENT_COORDIVIEWER_GETSLOT_REQ kPacket;
    kPacket.m_iLoginUID = LoginUID;
    kPacket.m_iCharType = CharType;
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_COORDIVIEWER_GETSLOT_REQ, kPacket);
}

bool KP2P::Send_CoordiViewSlotSetReq(int m_iCharType, int m_iSlotID, int m_iLoginUID, std::vector< GCITEMID > m_vecItemList)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_COORDIVIEWER_SETSLOT_REQ kPacket;
    kPacket.m_iCharType = m_iCharType;
    kPacket.m_iSlotID = m_iSlotID;
    kPacket.m_iLoginUID = m_iLoginUID;
    kPacket.m_vecItemList = m_vecItemList;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_COORDIVIEWER_SETSLOT_REQ, kPacket);
}

bool KP2P::Send_ChangeRoomHostReq(DWORD dwUserID_)
{
    NULL_CHECK(m_pkUserProxy);
    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_IN_ROOM_CHANGE_HOST_REQ, dwUserID_);
}

bool KP2P::Send_InRoomChangeCharReq(IN const char cPrevChar_, IN const char cNextChar_)
{
    NULL_CHECK(m_pkUserProxy);

    KEVENT_IN_ROOM_CHANGE_CHARACTER_INFO_REQ kPacket;
    kPacket.m_cNextChar = cNextChar_;
    kPacket.m_cPrevChar = cPrevChar_;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_IN_ROOM_CHANGE_CHARACTER_INFO_REQ, kPacket);
}

bool KP2P::Send_PetMagicStoneChangeEquipReq(GCITEMUID _petGlyphUID, int iCharType, bool bEquip)
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ kPacket;

    kPacket.m_dwPetGlyphUID = _petGlyphUID;
    kPacket.m_iCharType = iCharType;
    kPacket.m_bEquip = bEquip;

    return m_pkUserProxy->SendPacket(KUserEvent::EVENT_PET_MAGIC_STONE_CHANGE_EQUIP_REQ, kPacket);
}

bool KP2P::Send_UseHeroTicketReq( IN const int m_iDungeonID, IN const int m_iCharType, IN const DWORD m_dwItemID, IN const DWORD m_dwItemUID )
{
    NULL_CHECK(m_pkUserProxy);
    KEVENT_USE_HERO_TICKET_REQ kPacket;

    kPacket.m_iDungeonID = m_iDungeonID;
    kPacket.m_iCharType = m_iCharType;
    kPacket.m_dwItemID = m_dwItemID;
    kPacket.m_dwItemUID = m_dwItemUID;

    return m_pkUserProxy->SendPacket( KUserEvent::EVENT_USE_HERO_TICKET_REQ, kPacket );
}