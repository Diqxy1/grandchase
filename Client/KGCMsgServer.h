#ifndef _KGCMSGSERVER_H_
#define _KGCMSGSERVER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KNC/MSProxy.h"
#include "Packet.h"
//#include "define.h"

class KGCMsgServer
{
protected:	
	HWND m_hWnd;
	boost::shared_ptr< KMSProxy >       m_spMSProxy;	

    KEMS_S2_VERIFY_ACCOUNT_REQ*	m_pkVerifyInfo;

	KServerInfo*				m_pkMsgServerInfo;

	void DeleteConnectInfo();

    std::map<DWORD, KFriendInfo> m_mapFriendLIst;
    std::set<DWORD>              m_setBlockList;

	KGCMsgServer(void) {}
public:
	KGCMsgServer( HWND hWnd_ );
	~KGCMsgServer(void);

	enum
	{
        USERPROXY_ID    = 1,
		NUSERPROXY_ID   = 2,
		TRUSERPROXY_ID  = 3,
		MSPROXY_ID      = 4,
	};
	
	void SetMsgServerInfo( IN KEVENT_VERIFY_ACCOUNT_ACK& kAck_ );
	void SetMsgServerInfo( KServerInfo* pkServerInfo_, KEMS_S2_VERIFY_ACCOUNT_REQ* pkVerifyInfo_); 

	int ConnectToMsgServer( IN DWORD dwTimeOut = 5000 );
	bool IsConnectedToMsgServer()	{ return m_spMSProxy && m_spMSProxy->IsConnected(); }

	// old msn
	bool Send_AddFriendReq( const std::wstring& strNickName, const std::wstring& strMsg );
	bool Send_DeleteFriendReq( DWORD dwUID, bool bBlock );
	bool Send_AcceptFriendReq( DWORD dwUID );
	bool Send_RejectFriendReq( DWORD dwUID );
	bool Send_UpdateMyDataNot( KFriendInfo& kInfo );
	bool Send_MsnMessageReq( DWORD dwUID, const WCHAR* strMsg );
	bool Send_BlockFriendReq( DWORD dwUID );
	bool Send_UnBlockFriendReq( DWORD dwUID );	
	bool Send_ChangeNickName( const std::wstring& strNickName_ );
    bool Send_ChangeNickColor( const std::wstring& strNickName_, const std::wstring& strNickColor_ );
	bool Send_ChangeGameServer();

    // new msn
    bool Send_AddFriendReq_S2( IN const std::wstring& strNickName, IN const std::wstring& strMsg );
    bool Send_DeleteFriendReq_S2( IN DWORD dwUID, IN int nGroupID );
    bool Send_AcceptFriendReq_S2( IN DWORD dwUID );
    bool Send_RejectFriendReq_S2( IN DWORD dwUID );
    bool Send_UpdateMyDataNot_S2( IN KFriend& kInfo );
    bool Send_BlockFriendReq_S2( IN const std::wstring& strNickName_ );
    bool Send_UnBlockFriendReq_S2( IN DWORD dwUID );
    // new msn2
    bool Send_BlockMoveFriendReq_S2( IN int nGroupID, IN DWORD dwUID );
    bool Send_UnBlockMoveFriendReq_S2( IN DWORD dwUID );
    bool Send_AddGroupReq_S2( IN std::wstring& strGroupName );
    bool Send_DeleteGroupReq_S2( IN int nGroupID );
    bool Send_RenameGroupReq_S2( IN int nGroupID, IN std::wstring& strGroupName );
    bool Send_MoveFriendGroupReq_S2( IN int nFromGroupID, IN int nToGroupID, IN DWORD dwUID );
    bool Send_SetEmoticonReq_S2( IN DWORD dwItemID );
    // 대화하기
    bool Send_StartChatReq_S2( IN std::set<DWORD>& setChatUIDs );
    bool Send_InviteChatMemberReq_S2( IN std::pair<DWORD,DWORD>& prRoomKey, IN std::set<DWORD>& setInviteUIDs );
    bool Send_LeaveChatMemberReq_S2( IN std::pair<DWORD,DWORD>& prRoomKey );
    bool Send_ChatReq_S2( IN std::pair<DWORD,DWORD>& prRoomKey, IN std::set<DWORD>& setChatUIDs, IN const std::wstring& strMsg );
    // 쪽지
    bool Send_SendPaperReq_S2( IN const std::wstring& strRecvNick, IN std::set<DWORD>& setRecvUIDs, IN std::wstring& strMessage, IN DWORD dwSkinItemID );
    bool Send_LoadSentPaperboxReq_S2();
    bool Send_LoadRecvPaperboxReq_S2();
    bool Send_ReadPaperReq_S2( IN std::set<DWORD>& setPaperUIDs );
    bool Send_DelSentPaperReq_S2( IN std::set<DWORD>& setPaperUIDs );
    bool Send_DelRecvPaperReq_S2( IN std::set<DWORD>& setPaperUIDs );
    bool Send_SealPaperReq_S2( IN std::set<DWORD>& setPaperUIDs );
    bool Send_ForceMakeFriend( IN DWORD dwRequesterUID, IN std::wstring& strRequesterNick, IN DWORD dwRequesteeUID, IN std::wstring& strRequesteeNick );

    bool Send_AddGuildMemberListReq( IN DWORD dwUID, IN GCGuildUID GuildUID, std::set< DWORD >& setMembers );
    bool Send_DelGuildMemberListReq( IN DWORD dwUID, IN GCGuildUID GuildUID, std::set< DWORD >& setMembers );
    bool Send_GuildAllMemberListReq( IN DWORD dwUID, IN GCGuildUID GuildUID );

public:
    void    AddFriend( const KFriendInfo& kInfo );	
    void    DeleteFriend( DWORD dwUID );	
    void    UpdateFriend( KFriendInfo& kInfo );	
    bool    FindFriend( DWORD dwUID, KFriendInfo& kInfo );
    bool    IsFriend( DWORD dwUID_ );
    int     GetFriendsNum() { return (int)m_mapFriendLIst.size(); }
    void    UpdateLocation();
    void    FriendLogIn( std::wstring strNickName_ );

    void    SetBlockList( std::set<DWORD>& setBlockList ){ m_setBlockList = setBlockList; }
    void    SetBlockFriend( DWORD dwUID );
    void    SetUnBlockFriend( DWORD dwUID );
    bool    IsBlockFriend( DWORD dwUID );
    int     GetBlockFriendSize() { return (int)m_setBlockList.size(); }

public:
	//친구의 속성을 반환하는 부분입니다.
	//KFriendInfo 클래스를 다른 곳에서 include 하지 않고도 사용할 수 있도록 하기 위해서 만年求?
	//예전에는 KFriendInfo가 여기저기 흩어져서 저장되어 있었음. (불필요한 곳 까지... )


	void GetFriendList( std::vector<DWORD>* vecList_ );
	KFriendInfo* GetFriend( DWORD dwUID_ );
	std::wstring GetFriendName( DWORD dwUID_ );
	bool		IsFriendOnline( DWORD dwUID_ );
	bool		IsEmptyMsgServerInfo();
	bool		ConnectToMsgServerAgain();

	void InitFriendList();
	void DisconnectMsgServer();
};
#endif //_KGCMSGSERVER_H_
