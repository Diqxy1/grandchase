#pragma once
#include "Actor.h"
#include "MSPacket.h"
#include <windows.h>    // ::GetTickCount()
#include "MSEvent.h"

typedef std::map<DWORD,KPaper> KPAPERBOX;
typedef std::map< std::pair<DWORD,DWORD>, std::map<DWORD,std::pair<std::wstring,std::wstring>> > KMyChatRoom;

SmartPointer(KMSUser);

class KMSUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareException;

    enum TICK_INFO
    {
        TI_SEND_USER_INFO = 0,
        TI_LOAD_SENT_PAPERBOX,
        TI_LOAD_RECV_PAPERBOX,
        TI_MAX,
    };

public:
    struct MSUserPoolTag{};
    KMSUser(void);
    virtual ~KMSUser(void);

    static KMSUserPtr Create();
    static void AddManager( KMSUserPtr spMSUser );

    // override
    virtual void Tick();
    virtual KEvent* CreateKEvent() { return new KMSEvent; }

    KFriend GetMyFriendInfo_S2( DWORD dwReceiverUID = 0 );

protected:
    //override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다 

    //util
    bool UpdateFriendInfo_S2( IN const KFriend& kInfo );
    void SendToMyFriends_S2( IN const KFriend& kInfo, IN bool bConditionCheck = true );
    void GetSendableFriends( IN const std::map<DWORD,KFriend>& mapFriends_, OUT std::set<DWORD>& setFriendUIDList_
        , IN bool bFriendCheck_ = true, IN bool bOnlineCheck_ = true, IN bool bBlockFriendCheck_ = true );
    void SendUpdateMyChatCountNot();
    int GetAllFriendCount();
    bool IsBlockedFriend_S2( IN const DWORD dwFriendUID );
    bool IsBlockedFriend_S2( IN const std::wstring& strNick );
    void InitChatRoom();        // 메신저 접속 성공하면 채팅방 설정 초기화
    bool IsChatRoomEnable();    // 비어있는 채팅방 있는지.
    void ChkChatEnable( IN OUT std::set<DWORD>& setInviteUIDs_, OUT std::set<DWORD>& setUnableUIDs_, OUT std::map<DWORD, std::pair<std::wstring, std::wstring>>& mapMemberInfo_, OUT std::vector<DWORD>& vecRecvUIDs_ );
    void MakeChatRoom( IN const std::pair<DWORD,DWORD>& prRoomKey_, IN const std::map<DWORD,std::pair<std::wstring,std::wstring>>& mapMemberInfo_ );
    void CloseChatRoom( IN const std::pair<DWORD,DWORD>& prRoomKey_ );
    char GetChatRoomCount() { return m_cChatRoomCount; }
    void LeaveAllChatRoom();
    bool IsMyFriend( IN const DWORD dwFriendUID );
    bool IsMyFriend( IN const std::wstring& strNick );
    bool _IsMyFriend( IN const std::map<DWORD,KFriend>& mapMembers_, IN const DWORD dwFriendUID_ );
    bool _IsMyFriend( IN const std::map<DWORD,KFriend>& mapMembers_, IN const std::wstring& strNick_ );
    bool IsSealedPaper( IN const DWORD dwPaperUID );
    bool IsMySentPaper( IN const std::set<DWORD>& setPaperUIDs );
    bool IsMyRecvPaper( IN const std::set<DWORD>& setPaperUIDs );
    bool SetMyRecvPaper( IN const std::set<DWORD>& setPaperUIDs, IN const int nState );
    int GetSealedPaperCount();

    //tick
    DWORD GetTick( IN const int nIndex_ )      { return m_dwTick[ nIndex_ ]; }
    void SetTick( IN const int nIndex_ )       { m_dwTick[ nIndex_ ] = ::GetTickCount(); }
    DWORD GetTickGap( IN const int nIndex_ )   { return m_dwTickGap[ nIndex_ ];}
    void SetTickGap( IN const int nIndex_, IN const DWORD dwGap_ )  { m_dwTickGap[ nIndex_ ] = dwGap_;}

    // netmarble Log
    void LogoutLog();
    void SetRecvUIDs( IN const std::set<DWORD> setUID_, OUT std::vector<DWORD>& vecUID_ );

    void SetUserChannelType( IN const DWORD dwChannelType ) { m_dwChannelType = dwChannelType; }
    DWORD GetUserChannelType() { return m_dwChannelType; }

protected:  // Packet Handling
    DECL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_REQ );
    DECL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( EMS_S2_ADD_FRIEND_REQ );
    DECL_ON_FUNC( EMS_S2_ADD_FRIEND_ACK );
    DECL_ON_FUNC( EMS_S2_ADD_FRIEND_NOT );
   _DECL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_NOT, KFriend );
    DECL_ON_FUNC( EMS_S2_DELETE_FRIEND_REQ );
    DECL_ON_FUNC( EMS_S2_DELETE_FRIEND_ACK );
   _DECL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_REQ, DWORD );
    DECL_ON_FUNC( EMS_S2_ACCEPT_FRIEND_ACK );
    DECL_ON_FUNC( EMS_S2_UPDATE_FRIENDS_NOT );
   _DECL_ON_FUNC( EMS_S2_UPDATE_FRIEND_NOT, KFriend );
   _DECL_ON_FUNC( EMS_S2_REJECT_FRIEND_REQ, DWORD );
   _DECL_ON_FUNC( EMS_S2_REJECT_FRIEND_ACK, DWORD );
   _DECL_ON_FUNC( EMS_S2_REJECT_FRIEND_NOT, DWORD );
   _DECL_ON_FUNC( EMS_S2_UPDATE_MYDATA_NOT, KFriend );
    DECL_ON_FUNC( EMS_S2_LOGINNED_FRIEND_LIST_NOT );
    DECL_ON_FUNC( EMS_S2_BLOCK_FRIEND_REQ );
    DECL_ON_FUNC( EMS_S2_BLOCK_FRIEND_ACK );
   _DECL_ON_FUNC( EMS_S2_UNBLOCK_FRIEND_REQ, DWORD );
    DECL_ON_FUNC( EMS_S2_UNBLOCK_FRIEND_ACK );

    DECL_ON_FUNC( EMS_S2_BLOCK_MOVE_FRIEND_REQ );
    DECL_ON_FUNC( EMS_S2_BLOCK_MOVE_FRIEND_ACK );
   _DECL_ON_FUNC( EMS_S2_UNBLOCK_MOVE_FRIEND_REQ, DWORD );
    DECL_ON_FUNC( EMS_S2_UNBLOCK_MOVE_FRIEND_ACK );

   _DECL_ON_FUNC( EMS_S2_LIVE_DATA_REQ, DWORD );
   _DECL_ON_FUNC( EMS_S2_LOGON_REPORT_REQ, int );
   _DECL_ON_FUNC( EMS_S2_FRIEND_NOT_EXIST_NOT, DWORD );
   // 친구그룹
   _DECL_ON_FUNC( EMS_S2_ADD_GROUP_REQ, std::wstring );
    DECL_ON_FUNC( EMS_S2_ADD_GROUP_ACK );
   _DECL_ON_FUNC( EMS_S2_DELETE_GROUP_REQ, int );
    DECL_ON_FUNC( EMS_S2_DELETE_GROUP_ACK );
    DECL_ON_FUNC( EMS_S2_RENAME_GROUP_REQ );
    DECL_ON_FUNC( EMS_S2_RENAME_GROUP_ACK );
    DECL_ON_FUNC( EMS_S2_MOVE_FRIEND_TO_GROUP_REQ );
    DECL_ON_FUNC( EMS_S2_MOVE_FRIEND_TO_GROUP_ACK );
   // 대화하기
   _DECL_ON_FUNC( EMS_S2_START_CHAT_REQ, std::set<DWORD> );
    DECL_ON_FUNC( EMS_S2_START_CHAT_NOT );
    DECL_ON_FUNC( EMS_S2_INVITE_CHAT_MEMBER_REQ );
    DECL_ON_FUNC( EMS_S2_UPDATE_CHAT_MEMBER_NOT );
    DECL_ON_FUNC( EMS_S2_LEAVE_CHAT_MEMBER_REQ );
    DECL_ON_FUNC( EMS_S2_LEAVE_CHAT_MEMBER_NOT );
   _DECL_ON_FUNC( EMS_S2_SEND_CHAT_REQ, KChatMsg );
   _DECL_ON_FUNC( EMS_S2_SEND_CHAT_NOT, KChatMsg );
    DECL_ON_FUNC( EMS_S2_UPDATE_MY_CHAT_COUNT_NOT );
   // 이모티콘 설정
   _DECL_ON_FUNC( EMS_S2_SET_EMOTICON_REQ, DWORD );
    DECL_ON_FUNC( EMS_S2_SET_EMOTICON_ACK );
//     // 길드원 목록 갱신
//     DECL_ON_FUNC( EMS_S2_UPDATE_GUILD_MEMBER_REQ );
//     DECL_ON_FUNC( EMS_S2_UPDATE_GUILD_MEMBER_NOT );
//     // 길드미션완료 자동알림쪽지
//     DECL_ON_FUNC( EMS_S2_AUTO_GUILD_MESSAGE_REQ );
//     DECL_ON_FUNC( EMS_S2_AUTO_GUILD_MESSAGE_NOT );
// 쪽지
    DECL_ON_FUNC( EMS_S2_SEND_PAPER_REQ );
    DECL_ON_FUNC( EMS_S2_SEND_PAPER_ACK );
    DECL_ON_FUNC( EMS_S2_NEW_PAPER_NOT );
    
    DECL_ON_FUNC_NOPARAM( EMS_S2_LOAD_SENT_PAPERBOX_REQ );
    DECL_ON_FUNC( EMS_S2_UPDATE_SENT_PAPERBOX_NOT );
    DECL_ON_FUNC_NOPARAM( EMS_S2_LOAD_RECV_PAPERBOX_REQ );
    DECL_ON_FUNC( EMS_S2_UPDATE_RECV_PAPERBOX_NOT );

   _DECL_ON_FUNC( EMS_S2_READ_PAPER_REQ, std::set<DWORD> );
    DECL_ON_FUNC( EMS_S2_READ_PAPER_ACK );
   _DECL_ON_FUNC( EMS_S2_DEL_SENT_PAPER_REQ, std::set<DWORD> );
    DECL_ON_FUNC( EMS_S2_DEL_SENT_PAPER_ACK );
   _DECL_ON_FUNC( EMS_S2_DEL_RECV_PAPER_REQ, std::set<DWORD> );
    DECL_ON_FUNC( EMS_S2_DEL_RECV_PAPER_ACK );
   _DECL_ON_FUNC( EMS_S2_SEAL_PAPER_REQ, std::set<DWORD> );
    DECL_ON_FUNC( EMS_S2_SEAL_PAPER_ACK );
    DECL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_REQ );
   _DECL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_ACK, KEMS_S2_ADD_FRIEND_FORCE_REQ );
   _DECL_ON_FUNC( EMS_S2_ADD_FRIEND_FORCE_NOT, KFriend );

   _DECL_ON_FUNC( EMS_ADD_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq );
   _DECL_ON_FUNC( EMS_ADD_GUILD_MEMBER_LIST_ACK, KUpdateGuildMemberAck );
   _DECL_ON_FUNC( EMS_DEL_GUILD_MEMBER_LIST_REQ, KUpdateGuildMemberReq );

    DECL_ON_FUNC( EMS_GUILD_ALL_MEMBER_LIST_REQ );
    DECL_ON_FUNC( EMS_GUILD_ALL_MEMBER_LIST_ACK );

protected:
    std::wstring                    m_strNickName; // 계정 닉네임( == 대표 닉네임 == 팀명 )
    std::wstring                    m_strNickColor; // 계정 닉네임( == 대표 닉네임 == 팀명 )
    std::wstring                    m_strCharNickName; // 현재 캐릭터의 닉네임
    std::wstring                    m_strServerName;    // 060801. florist. 게임 서버의 이름.
    std::wstring                    m_strLocation;   // 070208. ZzangDoL & woosh. 채널이름, 현재방정보, 게임중?
    int                             m_iLocationNum;      // -1이면 방에들어가 있지 않는 로비상태
    bool                            m_bGamming;

    std::map<int,KGroup>            m_mapFriendGroup;
    KGroup                          m_kBlockGroup;
    KGroup                          m_kGuildGroup;
    DWORD                           m_dwEmoticon;

    KFriend                         m_friend;      // 마지막으로 행동한 상태
    KFriend                         m_ReservedLoc_S2;// 행동을 하지않아도 10초후 처음 도는 틱에서 위치를 기억

    char                            m_cChatRoomCount; //채팅룸 개수
    KMyChatRoom                     m_mapChatRoomInfo; // 채팅룸 정보(키,맴버정보)

    bool                            m_bHaveNewSentPaper;//새로 보낸 쪽지가 있는지
    bool                            m_bHaveNewRecvPaper;//새로 받은 쪽지가 있는지.
    KPAPERBOX                       m_mapSentPaperBox;  //보낸 쪽지함
    KPAPERBOX                       m_mapRecvPaperBox;  //받은 쪽지함

    DWORD                           m_dwTick[TI_MAX];
    DWORD                           m_dwTickGap[TI_MAX];

    CTime                           m_tmLoginTime;
    GCGuildUID                      m_GuildUID; // 유저의 소속 길드 UID

    DWORD                           m_dwChannelType; // 해당 유저의 채널링 Type
};