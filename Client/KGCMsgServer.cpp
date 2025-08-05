#include "stdafx.h"
#include ".\kgcmsgserver.h"
#include "MyD3D.h"
#include "GCStateMachine.h"

KGCMsgServer::KGCMsgServer( HWND hWnd_ )
{	
	m_hWnd = hWnd_;

	m_pkVerifyInfo = NULL;
	m_pkMsgServerInfo = NULL;
}

KGCMsgServer::~KGCMsgServer(void)
{
	if( !IsConnectedToMsgServer() )
		m_spMSProxy.reset();
}

void KGCMsgServer::DisconnectMsgServer()
{
	if( IsConnectedToMsgServer() )
	{
		m_spMSProxy.reset();
		DeleteConnectInfo();
	}
}

bool KGCMsgServer::Send_AddFriendReq( const std::wstring& strNickName, const std::wstring& strMsg )
{
	if( !IsConnectedToMsgServer() )
		return false;

	KEMS_ADD_FRIEND_REQ req;
	req.m_strNickName = strNickName;
	req.m_strMessage = strMsg;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_ADD_FRIEND_REQ, req );
}

bool KGCMsgServer::Send_DeleteFriendReq( DWORD dwUID, bool bBlock )
{
	if( !IsConnectedToMsgServer() )
		return false;

	KEMS_DELETE_FRIEND_REQ req;
	req.m_dwDeleteUID = dwUID;
	req.m_bBlock = bBlock;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_DELETE_FRIEND_REQ, req );
}

bool KGCMsgServer::Send_AcceptFriendReq( DWORD dwUID )
{
	if( !IsConnectedToMsgServer() )
		return false;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_ACCEPT_FRIEND_REQ, dwUID );
}

bool KGCMsgServer::Send_RejectFriendReq( DWORD dwUID )
{
	if( !IsConnectedToMsgServer() )
		return false;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_REJECT_FRIEND_REQ, dwUID );
}

bool KGCMsgServer::Send_UpdateMyDataNot( KFriendInfo& kInfo )
{
	if( !IsConnectedToMsgServer() )
		return false;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_UPDATE_MYDATA_NOT, kInfo );
}

bool KGCMsgServer::Send_MsnMessageReq( DWORD dwUID, const WCHAR* strMsg )
{
	if( !IsConnectedToMsgServer() )
		return false;

	KFriendMsgInfo kInfo;
	kInfo.m_dwReceiverUID = dwUID;
	kInfo.m_strMessage = strMsg;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_CHAT_REQ, kInfo );
}

bool KGCMsgServer::Send_BlockFriendReq( DWORD dwUID )
{
	if( !IsConnectedToMsgServer() )
		return false;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_BLOCK_FRIEND_REQ, dwUID );
}

bool KGCMsgServer::Send_UnBlockFriendReq( DWORD dwUID )
{
	if( !IsConnectedToMsgServer() )
		return false;

	return m_spMSProxy->SendPacket( KMSEvent::EMS_UNBLOCK_FRIEND_REQ, dwUID );
}

bool KGCMsgServer::Send_ChangeNickName( const std::wstring& strNickName_ )
{
	if( !IsConnectedToMsgServer() )
		return false;

    KFriend kInfo;
    kInfo.m_strNickName = strNickName_;
    kInfo.m_strServerName = g_kGlobalValue.ServerInfo.strServerName;
    kInfo.m_strCharNickName = g_kGlobalValue.m_kUserInfo.strNickName;

    return Send_UpdateMyDataNot_S2( kInfo );
}

bool KGCMsgServer::Send_ChangeNickColor( const std::wstring& strNickName_, const std::wstring& strNickColor_ )
{
    if (!IsConnectedToMsgServer())
        return false;

    KFriend kInfo;
    kInfo.m_strNickColor = strNickColor_;
    kInfo.m_strNickName = strNickName_;
    kInfo.m_strServerName = g_kGlobalValue.ServerInfo.strServerName;
    kInfo.m_strCharNickName = g_kGlobalValue.m_kUserInfo.strNickName;

    return Send_UpdateMyDataNot_S2(kInfo);
}

bool KGCMsgServer::Send_ChangeGameServer()
{
	if( !IsConnectedToMsgServer() )
		return false;

	// 유저 인증에서 셋팅된 정보가 없으면 접속 안함.
	if( m_pkMsgServerInfo == NULL || m_pkVerifyInfo == NULL )
	{
		DeleteConnectInfo();
		return false;
	}

    KFriend kInfo;
    kInfo.m_strNickName = m_pkVerifyInfo->m_strNickName;
    kInfo.m_strServerName = m_pkVerifyInfo->m_strServerName;
    kInfo.m_strLocation = g_MyD3D->m_pStateMachine->GetCurrentLocation();
    kInfo.m_iLocationNum = -1;
    kInfo.m_bIsGamming = false;
    kInfo.m_strCharNickName = g_kGlobalValue.m_kUserInfo.strNickName;
    bool bRet;
    bRet = Send_UpdateMyDataNot_S2( kInfo );
    DeleteConnectInfo();
    return bRet;
}

void KGCMsgServer::SetMsgServerInfo( IN KEVENT_VERIFY_ACCOUNT_ACK& kAck_ )
{
    m_pkMsgServerInfo = new KServerInfo();
    *m_pkMsgServerInfo = kAck_.m_kMsgServerInfo;

    m_pkVerifyInfo = new KEMS_S2_VERIFY_ACCOUNT_REQ();

	m_pkVerifyInfo->m_dwUserUID        = kAck_.m_dwUserUID;
	m_pkVerifyInfo->m_strLogin         = kAck_.m_strLogin;
	m_pkVerifyInfo->m_strNickName      = kAck_.m_strNickName;
    m_pkVerifyInfo->m_strNickColor     = kAck_.m_strNickColor;
	m_pkVerifyInfo->m_strServerName    = kAck_.m_strServerName;	
	m_pkVerifyInfo->m_strLocation	   = g_MyD3D->m_pStateMachine->GetCurrentLocation();
	m_pkVerifyInfo->m_iLocationNum	   = -1;
	m_pkVerifyInfo->m_bGamming		   = false;
    m_pkVerifyInfo->m_dwChannelType    = kAck_.m_dwChannelType;
    m_pkVerifyInfo->m_nLanguageCode    = SiKGCMultipleLanguages()->GetLanguageTypeNumber();
    m_pkVerifyInfo->m_strCharNickName  = kAck_.m_strCharNickName;
    // todo:woosh m_pkVerifyInfo->m_nGuildID         = kAck_.m_kGuildUserInfo.m_iGuildId;
}

void KGCMsgServer::SetMsgServerInfo( KServerInfo* pkServerInfo_, KEMS_S2_VERIFY_ACCOUNT_REQ* pkVerifyInfo_ )
{
	m_pkMsgServerInfo = new KServerInfo();
	*m_pkMsgServerInfo = *pkServerInfo_;

	m_pkVerifyInfo = new KEMS_S2_VERIFY_ACCOUNT_REQ();

	m_pkVerifyInfo->m_dwUserUID        = pkVerifyInfo_->m_dwUserUID;
	m_pkVerifyInfo->m_strLogin         = pkVerifyInfo_->m_strLogin;
	m_pkVerifyInfo->m_strNickName      = pkVerifyInfo_->m_strNickName;
    m_pkVerifyInfo->m_strNickColor      = pkVerifyInfo_->m_strNickColor;
	m_pkVerifyInfo->m_strServerName    = pkVerifyInfo_->m_strServerName;	
	m_pkVerifyInfo->m_strLocation		= g_MyD3D->m_pStateMachine->GetCurrentLocation();
	m_pkVerifyInfo->m_iLocationNum		= -1;
	m_pkVerifyInfo->m_bGamming			= false;
    m_pkVerifyInfo->m_nGuildID          = pkVerifyInfo_->m_nGuildID;
    m_pkVerifyInfo->m_dwChannelType     = pkVerifyInfo_->m_dwChannelType;
    m_pkVerifyInfo->m_nLanguageCode    = SiKGCMultipleLanguages()->GetLanguageTypeNumber();
    //m_pkVerifyInfo->m_strCharNickName  = g_kGlobalValue.m_kUserInfo.GetStrCharName();
}

void KGCMsgServer::DeleteConnectInfo()
{
	SAFE_DELETE( m_pkVerifyInfo );
	SAFE_DELETE( m_pkMsgServerInfo );
}

int KGCMsgServer::ConnectToMsgServer( IN DWORD dwTimeOut_ /* = 5000  */ )
{
	boost::shared_ptr<KMSProxy> spMSProxy( new KMSProxy );
	spMSProxy->SetClassID( MSPROXY_ID );
	spMSProxy->Init( false );
	spMSProxy->SetHwnd( m_hWnd );

	int nRet = -99;

	// 유저 인증에서 셋팅된 정보가 없으면 접속 안함.
	if( m_pkMsgServerInfo == NULL || m_pkVerifyInfo == NULL )
	{
		DeleteConnectInfo();
		return nRet;
	}

	// 메세지 서버의 IP가 유효하지 못하다면 접속 시도도 하지 않는다.
	if( m_pkMsgServerInfo->m_strIP.empty() )    
	{
		DeleteConnectInfo();
		return nRet;
	}

	//친구 리스트 초기화
	m_mapFriendLIst.clear();

    KEMS_S2_VERIFY_ACCOUNT_REQ kReq;

	kReq.m_dwUserUID        = m_pkVerifyInfo->m_dwUserUID;
	kReq.m_strLogin         = m_pkVerifyInfo->m_strLogin;
	kReq.m_strNickName      = m_pkVerifyInfo->m_strNickName;
    kReq.m_strNickColor     = m_pkVerifyInfo->m_strNickColor;
	kReq.m_strServerName    = m_pkVerifyInfo->m_strServerName;
	kReq.m_strLocation		= g_MyD3D->m_pStateMachine->GetCurrentLocation();
	kReq.m_iLocationNum		= -1;
	kReq.m_bGamming			= false;
    kReq.m_nGuildID         = g_kGlobalValue.m_kGuildInfo.m_dwUID;
    kReq.m_dwChannelType    = m_pkVerifyInfo->m_dwChannelType;
    kReq.m_nLanguageCode    = SiKGCMultipleLanguages()->GetLanguageTypeNumber();
    //kReq.m_strCharNickName  = g_kGlobalValue.m_kUserInfo.GetStrCharName();

	KEventPtr spEvent( new KMSEvent );

    spEvent->m_usEventID = KMSEvent::EMS_S2_VERIFY_ACCOUNT_REQ;

	spEvent->Serialize( kReq );

	nRet = spMSProxy->ConnectNAuth( *m_pkMsgServerInfo , spEvent, dwTimeOut_ );

	LOG_CONDITION( nRet < 0, cerr, cout )
		<< BUILD_LOG( nRet )
		<< BUILD_LOG( m_pkMsgServerInfo->m_strName )
		<< BUILD_LOG( m_pkMsgServerInfo->m_strIP )
		<< BUILD_LOG( m_pkMsgServerInfo->m_usPort );

	if( nRet == 0 )
	{
		m_spMSProxy.swap( spMSProxy );
	}

	DeleteConnectInfo();
	return nRet;
}

bool KGCMsgServer::FindFriend( DWORD dwUID, KFriendInfo& kInfo )
{
	if( IsFriend( dwUID ) )
	{
		kInfo = m_mapFriendLIst[dwUID];
		return true;
	}	

	return false;
}

void KGCMsgServer::AddFriend( const KFriendInfo& kInfo )
{
	KFriendInfo kTemp;
	if( FindFriend( kInfo.m_dwUserUID, kTemp ) == true )
		return;

	m_mapFriendLIst[kInfo.m_dwUserUID] = kInfo;		
}

void KGCMsgServer::UpdateFriend( KFriendInfo& kInfo )
{
	if( IsFriend( kInfo.m_dwUserUID ) )
	{
		m_mapFriendLIst[kInfo.m_dwUserUID] = kInfo;
	}
}

void KGCMsgServer::DeleteFriend( DWORD dwUID )
{
	if( IsFriend( dwUID ) )
	{
		m_mapFriendLIst.erase( dwUID );
	}

	//for( int i=0 ; i<(int)m_vecFriendList.size() ; ++i )
	//{
	//	if( m_vecFriendList[i].m_dwUserUID == dwUID )
	//	{
	//		m_vecFriendList.erase( m_vecFriendList.begin()+i );

	//		if( i <= m_iSelectFriend )
	//			--m_iSelectFriend;

	//		SetFriendList();

	//		break;
	//	}
	//}
}

bool KGCMsgServer::IsFriend( DWORD dwUID_ )
{
	if( m_mapFriendLIst.find( dwUID_ ) != m_mapFriendLIst.end() )
		return true;

	return false;
}

KFriendInfo* KGCMsgServer::GetFriend( DWORD dwUID_ )
{	
	if( IsFriend( dwUID_ ) )
	{
		return &m_mapFriendLIst[dwUID_];
	}

	return NULL;
}

void KGCMsgServer::GetFriendList( std::vector<DWORD>* vecList_ )
{
	vecList_->clear();
	for( std::map<DWORD, KFriendInfo>::iterator itor = m_mapFriendLIst.begin() ; itor != m_mapFriendLIst.end() ; itor++ )
	{
		vecList_->push_back( itor->second.m_dwUserUID );
	}
}

std::wstring KGCMsgServer::GetFriendName( DWORD dwUID_ )
{
	if( IsFriend( dwUID_ ) )
	{
		return m_mapFriendLIst[dwUID_].m_strNickName;
	}

	return L"";
}

bool KGCMsgServer::IsFriendOnline( DWORD dwUID_ )
{
	if( IsFriend( dwUID_ ) )
	{
		return m_mapFriendLIst[dwUID_].m_bOnline;
	}

	return false;
}

void KGCMsgServer::UpdateLocation()
{
    if( !IsConnectedToMsgServer() )
        return;

    KFriend kInfo;
    kInfo.m_strNickName = g_kGlobalValue.m_kUserInfo.strNickName;
    kInfo.m_strServerName = g_kGlobalValue.ServerInfo.strServerName;
    kInfo.m_strLocation = g_MyD3D->m_pStateMachine->GetCurrentLocation();
    kInfo.m_iLocationNum = -1;
    kInfo.m_bIsGamming = false;
    //kInfo.m_strCharNickName = g_kGlobalValue.m_kUserInfo.GetStrCharName();
    Send_UpdateMyDataNot_S2( kInfo );
}

void KGCMsgServer::FriendLogIn( std::wstring strNickName_ )
{	    
    g_pkChatManager->AddChatMsg( g_pkStrLoader->GetReplacedString( STR_ID_MESSENGER_FRIEND_LOGIN, "l", strNickName_ ),
        KGCChatManager::CHAT_TYPE_AUTO, 0xFFFFFFFF, false );// [**]님이 로그인 하셨습니다.
}

bool KGCMsgServer::IsBlockFriend( DWORD dwUID )
{
    std::set<DWORD>::iterator itor;
    itor = m_setBlockList.find( dwUID );

    if( itor == m_setBlockList.end() )
        return false;

    return true;
}

void KGCMsgServer::SetBlockFriend( DWORD dwUID )
{
    // 이미 차단되어있으면 리턴
    if( IsBlockFriend( dwUID ) == true )
        return;

    // 차단목록에 추가
    m_setBlockList.insert( dwUID );
}

void KGCMsgServer::SetUnBlockFriend( DWORD dwUID )
{
    // 차단목록에 없으면 리턴
    std::set<DWORD>::iterator itor;
    itor = m_setBlockList.find( dwUID );
    if( itor == m_setBlockList.end() )
        return;

    // 차단목록에서 해제
    m_setBlockList.erase( itor );
}



//-------------------------------------------------------------------------------------------------------//
// New Packet
//-------------------------------------------------------------------------------------------------------//

bool KGCMsgServer::Send_AddFriendReq_S2( IN const std::wstring& strNickName_, IN const std::wstring& strMsg_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_ADD_FRIEND_REQ req;
    req.m_bIsBlock = KEMS_S2_ADD_FRIEND_REQ::FRIEND_REQ;
    req.m_strNickName = strNickName_;
    req.m_strCharNickName = strNickName_;
    req.m_nCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
    req.m_strMessage = strMsg_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_ADD_FRIEND_REQ, req );
}

bool KGCMsgServer::Send_DeleteFriendReq_S2( IN DWORD dwUID_, IN int nGroupID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_DELETE_FRIEND_REQ req;
    req.m_dwFriendUID = dwUID_;
    req.m_nGroupID = nGroupID_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_DELETE_FRIEND_REQ, req );
}

bool KGCMsgServer::Send_AcceptFriendReq_S2( IN DWORD dwUID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_ACCEPT_FRIEND_REQ, dwUID_ );
}

bool KGCMsgServer::Send_RejectFriendReq_S2( IN DWORD dwUID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_REJECT_FRIEND_REQ, dwUID_ );
}

bool KGCMsgServer::Send_UpdateMyDataNot_S2( IN KFriend& kInfo_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_UPDATE_MYDATA_NOT, kInfo_ );
}

bool KGCMsgServer::Send_BlockFriendReq_S2( IN const std::wstring& strNickName_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_BLOCK_FRIEND_REQ req;
    req.m_bIsBlock = KEMS_S2_BLOCK_FRIEND_REQ::BLOCK_REQ;
    req.m_strNickName = strNickName_;
    req.m_strCharNickName = strNickName_;
    req.m_nCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_BLOCK_FRIEND_REQ, req );
}

bool KGCMsgServer::Send_UnBlockFriendReq_S2( IN DWORD dwUID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_UNBLOCK_FRIEND_REQ, dwUID_ );
}

bool KGCMsgServer::Send_BlockMoveFriendReq_S2( IN int nGroupID_, IN DWORD dwUID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_BLOCK_MOVE_FRIEND_REQ req;
    req.m_nGroupID = nGroupID_;
    req.m_dwFriendUID = dwUID_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_BLOCK_MOVE_FRIEND_REQ, req );
}

bool KGCMsgServer::Send_UnBlockMoveFriendReq_S2( IN DWORD dwUID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, dwUID_ );
}

// new msn2
bool KGCMsgServer::Send_AddGroupReq_S2( IN std::wstring& strGroupName_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_ADD_GROUP_REQ, strGroupName_ );
}

bool KGCMsgServer::Send_DeleteGroupReq_S2( IN int nGroupID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_DELETE_GROUP_REQ, nGroupID_ );
}

bool KGCMsgServer::Send_RenameGroupReq_S2( IN int nGroupID_, IN std::wstring& strGroupName_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_RENAME_GROUP_REQ req;
    req.m_nGroupID = nGroupID_;
    req.m_strNewGroupName = strGroupName_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_RENAME_GROUP_REQ, req );
}

bool KGCMsgServer::Send_MoveFriendGroupReq_S2( IN int nFromGroupID_, IN int nToGroupID_, IN DWORD dwUID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_MOVE_FRIEND_TO_GROUP_REQ req;
    req.m_nFromGroupID = nFromGroupID_;
    req.m_nToGroupID = nToGroupID_;
    req.m_dwFriendUID = dwUID_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_MOVE_FRIEND_TO_GROUP_REQ, req );
}

bool KGCMsgServer::Send_SetEmoticonReq_S2( IN DWORD dwItemID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_SET_EMOTICON_REQ, dwItemID_ );
}

bool KGCMsgServer::Send_StartChatReq_S2( IN std::set<DWORD>& setChatUIDs_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_START_CHAT_REQ, setChatUIDs_ );
}

bool KGCMsgServer::Send_LeaveChatMemberReq_S2( IN std::pair<DWORD,DWORD>& prRoomKey_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_LEAVE_CHAT_MEMBER_REQ, prRoomKey_ );
}

bool KGCMsgServer::Send_ChatReq_S2( IN std::pair<DWORD,DWORD>& prRoomKey_, IN std::set<DWORD>& setChatUIDs_, IN const std::wstring& strMsg_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    //EVENT_BAD_USER_INFO_NOT의 ENUM 값 UT_CHAT_BLOCK -> -4
    //채팅 금지 유저 일때는 return 하자.
    if ( g_pkChatManager->CheckChatBlockUser() )
        return false;

    KChatMsg req;
    req.m_prRoomKey = prRoomKey_;
    req.m_setReceiverUIDs = setChatUIDs_;//서버에서 세팅
    req.m_dwSenderUID = 0;//서버에서 세팅
    req.m_strMessage = strMsg_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_SEND_CHAT_REQ, req );
}

bool KGCMsgServer::Send_InviteChatMemberReq_S2( IN std::pair<DWORD,DWORD>& prRoomKey_, IN std::set<DWORD>& setInviteUIDs_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_INVITE_CHAT_MEMBER_REQ req;
    req.m_prRoomKey = prRoomKey_;           //채팅방key
    req.m_setInviteUIDs = setInviteUIDs_;   //내가 초대한 유저 UID 리스트

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_INVITE_CHAT_MEMBER_REQ, req );
}

bool KGCMsgServer::Send_SendPaperReq_S2( IN const std::wstring& strRecvNick_, IN std::set<DWORD>& setRecvUIDs_, IN std::wstring& strMessage_, IN DWORD dwSkinItemID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_SEND_PAPER_REQ req;
    req.m_strRecvNick = strRecvNick_;
    req.m_setRecvUIDs = setRecvUIDs_;
    req.m_strMessage = strMessage_;
    req.m_dwSkinItemID = dwSkinItemID_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_SEND_PAPER_REQ, req );
}

bool KGCMsgServer::Send_LoadSentPaperboxReq_S2()
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendID( KMSEvent::EMS_S2_LOAD_SENT_PAPERBOX_REQ );
}

bool KGCMsgServer::Send_LoadRecvPaperboxReq_S2()
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendID( KMSEvent::EMS_S2_LOAD_RECV_PAPERBOX_REQ );
}

bool KGCMsgServer::Send_ReadPaperReq_S2( IN std::set<DWORD>& setPaperUIDs_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_READ_PAPER_REQ, setPaperUIDs_ );
}

bool KGCMsgServer::Send_DelSentPaperReq_S2( IN std::set<DWORD>& setPaperUIDs_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_DEL_SENT_PAPER_REQ, setPaperUIDs_ );
}

bool KGCMsgServer::Send_DelRecvPaperReq_S2( IN std::set<DWORD>& setPaperUIDs_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_DEL_RECV_PAPER_REQ, setPaperUIDs_ );
}

bool KGCMsgServer::Send_SealPaperReq_S2( IN std::set<DWORD>& setPaperUIDs_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_SEAL_PAPER_REQ, setPaperUIDs_ );
}

bool KGCMsgServer::Send_ForceMakeFriend( IN DWORD dwRequesterUID_, IN std::wstring& strRequesterNick_, IN DWORD dwRequesteeUID_, IN std::wstring& strRequesteeNick_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_S2_ADD_FRIEND_FORCE_REQ kPacket;

    kPacket.m_dwRequesterUID    = dwRequesterUID_;
    kPacket.m_strRequesterNick  = strRequesterNick_;
    kPacket.m_dwRequesteeUID    = dwRequesteeUID_;
    kPacket.m_strRequesteeNick  = strRequesteeNick_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_S2_ADD_FRIEND_FORCE_REQ, kPacket );
}

bool KGCMsgServer::IsEmptyMsgServerInfo()
{
	if( m_pkMsgServerInfo == NULL || m_pkVerifyInfo == NULL )
	{
		DeleteConnectInfo();
		return true;
	}

	return false;
}

bool KGCMsgServer::ConnectToMsgServerAgain()
{
	// 만약 메시지 서버에 접속되어있지 않다면 메시지 서버에 접속시도
	if(IsConnectedToMsgServer() == false )
	{
		if(IsEmptyMsgServerInfo())
			SetMsgServerInfo( &g_kGlobalValue.m_kMsgServerInfo, &g_kGlobalValue.m_kVerifyInfo);

		ConnectToMsgServer();

		if( IsConnectedToMsgServer() == false )
		{
			return false;
		}

		Send_ChangeGameServer();
	}

	return true;
}

void KGCMsgServer::InitFriendList()
{
	m_mapFriendLIst.clear();
}

bool KGCMsgServer::Send_AddGuildMemberListReq( IN DWORD dwUID_, IN GCGuildUID GuildUID_, std::set< DWORD >& setMembers_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KUpdateGuildMemberReq kPacket;

    kPacket.m_dwUserUID = dwUID_;
    kPacket.m_GuildUID = GuildUID_;
    kPacket.m_setMemberUID = setMembers_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_ADD_GUILD_MEMBER_LIST_REQ, kPacket ); 
}

bool KGCMsgServer::Send_DelGuildMemberListReq( IN DWORD dwUID_, IN GCGuildUID GuildUID_, std::set< DWORD >& setMembers_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KUpdateGuildMemberReq kPacket;

    kPacket.m_dwUserUID = dwUID_;
    kPacket.m_GuildUID = GuildUID_;
    kPacket.m_setMemberUID = setMembers_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_DEL_GUILD_MEMBER_LIST_REQ, kPacket ); 
}

bool KGCMsgServer::Send_GuildAllMemberListReq( IN DWORD dwUID_, IN GCGuildUID GuildUID_ )
{
    if( !IsConnectedToMsgServer() )
        return false;

    KEMS_GUILD_ALL_MEMBER_LIST_REQ kPacket;

    kPacket.m_dwUserUID = dwUID_;
    kPacket.m_GuildUID = GuildUID_;

    return m_spMSProxy->SendPacket( KMSEvent::EMS_GUILD_ALL_MEMBER_LIST_REQ, kPacket ); 
}