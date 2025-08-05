#pragma once
#include "SimLayer.h"
#include "MRConnector.h"

class KMSSimLayer : public KSimLayer
{
    DeclareSingleton( KMSSimLayer );
    NiDeclareRTTI;
    DeclToStringW;

public:

    KMSSimLayer(void);
    virtual ~KMSSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

    bool DestroyAllUsers();

    void GetLoginnedFriendInfo( KEMR_LOGINNED_FRIEND_INFO_REQ& kReq );
    void GetLoginnedFriendGroupInfo_S2( IN OUT KEMR_LOGINNED_FRIEND_INFO_REQ& kReq );
    void GetLoginnedFriendInfo_S2( IN DWORD dwLogonUID, IN std::set<DWORD>& mapFriendUID, OUT std::map<DWORD,KFriend>& mapMembers );

    void GetChatRoomUID( OUT std::pair<DWORD,DWORD>& prRoomKey );

public: // for lua..
    DWORD           m_dwShutdownTimeLimit;  // 060113. florist. 서버 종료시 DB처리를 위한 대기시간.
    DWORD           m_dwSendTimeLimit; // 070402. MSUser 에서 내 정보를 보고하는 시간 간격.
    //KMRConnector    m_kMRConnector;

    unsigned short  m_usMaxFriendLimit;     //최대 친구수
    unsigned short  m_usMaxBlockLimit;      //최대 블럭수
    unsigned short  m_usMaxGroupLimit;      //최대 그룹수
    unsigned short  m_usMaxGroupName;       //그룹명 길이
    unsigned short  m_usMaxChatRoom;        //최대 채팅방 생성수
    unsigned short  m_usMaxChatMember;      //최대 채팅방 맴버수

protected:
    mutable KncCriticalSection  m_csChatRoomUID;
    DWORD                       m_dwChatRoomUID; // 채팅룸 UID
};

DefSingletonInline( KMSSimLayer );