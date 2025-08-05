#pragma once
#include "UserProxy.h"
#include "NUserProxy.h"
#include "UserFSM.h"
#include "FSM/support_FSM.h"
#include <ObjectPool/BlockAllocator.h>
#include "ActionDecider.h"
#include "SingleRobotPacketDef.h"
#include "KncP2P.h"
#include "MSProxy.h"
#include <fstream>
#include <iostream>
#include <atlbase.h>
#include <atlconv.h>

#define  IMPL_ON_KNC_FUNC(id, packet)   void CLASS_TYPE##::ON_##id( packet* pkPacket_ )

#define  DECL_ON_KNC_FUNC(id, packet)   void ON_##id( packet* pkPacket )

SmartPointer( KSingleRobot );

class KSingleRobot : public KUserProxy, public BlockAllocator<KSingleRobot, 2000>
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareFSM;
public:
#   undef KENUM
#   define KENUM( id ) id,
    enum eACTION
    {   
#   include "RobotAction_def.h"
    };

    KSingleRobot(void);
    virtual ~KSingleRobot(void);

    // derived from KSession
    virtual void OnDestroy();
    virtual void Tick();

    static void ReleaseRobot();

public: // for lua
    static void SetProtocolVer( int nProtocolVer )                  { ms_nProtocolVer = nProtocolVer; }
    static void SetAutoAction( bool bAutoAction )                   { ms_bAutoAction = bAutoAction; }
    static void SetRemoteIP( const char* szIP )                     { ms_strRemoteIP = szIP; } // 060102. kkurrung. Don't touch LUA.(robot)
    static void SetCenterRemoteIP( const char* szIP )               { ms_strCenterRemoteIP = szIP; } // 060102. kkurrung. Don't touch LUA.(robot)
    static void SetMsgRemoteIP( const char* szIP )                  { ms_strMsgRemoteIP = szIP; }
    static void SetMsgRemoteIP2nd( const char* szIP )                  { ms_strMsgRemoteIP2nd = szIP; }
    static void SetRemoteGameServerPort( unsigned short usPort )    { ms_usRemoteGameServerPort = usPort; }
    static void SetRemoteCenterServerPort( unsigned short usPort )  { ms_usRemoteCenterServerPort = usPort; }
    static void SetRemoteMsgServerPort( unsigned short usPort )     { ms_usRemoteMsgServerPort = usPort; }
    static void SetRemoteMsgServerPort2nd( unsigned short usPort )     { ms_usRemoteMsgServerPort2nd = usPort; }
    static KActionDecider* GetRobotAction()                         { return &ms_kActionDecider; }
    static void SetRobotTestMsgSize( int iSize_ )           
    {
        //ms_strRobotTestMsg.clear();
        //ms_strRobotTestMsg.erase( ms_strRobotTestMsg.begin(), ms_strRobotTestMsg.end() );
        //for ( int i = 0; i < iSize_; i++ )
        //{
        //    ms_strRobotTestMsg.append( L"." );
        //}
        //ms_kSerBuffer.Clear();
        //KSerializer kSerBuffer;
        //KEventPtr spEvent( new KUserEvent() );
        //spEvent->m_usEventID = KUserEvent::EVENT_BOT_TEST_NOT;

        //// make event object
        //KSerializer         ks;

        //// serialize - only data
        //ks.BeginWriting( &spEvent->m_kbuff );
        //ks.Put( ms_strRobotTestMsg );
        //ks.EndWriting();

        //// serialize - full event object
        //ks.BeginWriting( &ms_kSerBuffer );
        //ks.Put( *spEvent );
        //ks.EndWriting();    

    }
    static void SetRobotTestMsgTime( DWORD dwTime_ )    { ms_dwRobotTestMsgSendTime = dwTime_; }
    static void SetLoginOnly( bool bLoginOnly ) { ms_bLoginOnly = bLoginOnly; }
    bool GetAlive();
    int  GetCheckCount();
protected:

    inline int DecideAction();             // 확률에 근거해 실행해야 할 action을 정한다.
    inline void DoAction( int nAction );   // 입력받은 액션을 취하는 핸들러를 호출한다.
    inline void HandleRecvMsg();           // 내부 큐에 받아둔 recv 패킷들을 꺼내서 해당 핸들러를 호출한다.

    //{{ 050714. jseop. m_bOnWainting flag에 따른 처리를 위해 패킷 보내는 함수를 새로 정의하여 사용함.
    template < typename T >
    bool RobotSendPacket( unsigned short usEventID, T& tData, bool bSetWaiting, bool bCheckWaiting = true );
    bool RobotSendID( unsigned short usEventID, bool bSetWaiting, bool bCheckWaiting = true );
    //}} jseop.

protected:  // event handler function

    DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( EVENT_CHANNEL_LIST_ACK );
    _DECL_ON_FUNC( EVENT_REGISTER_NICKNAME_ACK,KPacketNameOK );
    //DECL_ON_FUNC( EVENT_CHANNEL_LIST_NOT );
    _DECL_ON_FUNC( EVENT_ENTER_CHANNEL_ACK, int );
   //_DECL_ON_FUNC( EVENT_CHAT_NOT, KChatData );
    DECL_ON_FUNC( EVENT_WHISPER_ACK );
    //DECL_ON_FUNC( EVENT_ROOM_LIST_NOT );
    //DECL_ON_FUNC( EVENT_USER_LIST_NOT );
    //DECL_ON_FUNC( EVENT_INVITE_NOT );
   _DECL_ON_FUNC( EVENT_JOIN_ROOM_ACK, KJoinRoomAckInfo );
   _DECL_ON_FUNC( EVENT_JOIN_ROOM_BROAD, KInDoorUserInfo );
   _DECL_ON_FUNC( EVENT_JOIN_COMPLETE_ACK, int );
    DECL_ON_FUNC( EVENT_INFORM_USER_LEAVE_ROOM_NOT  );
   _DECL_ON_FUNC( EVENT_HOST_MIGRATED_NOT, DWORD );
    DECL_ON_FUNC( EVENT_CREATE_ROOM_ACK );
    DECL_ON_FUNC( EVENT_START_GAME_BROAD );
    _DECL_ON_FUNC( EVENT_END_GAME_ACK, KEndGameAck );
    _DECL_ON_FUNC( EVENT_LEAVE_ROOM_ACK, int );

    DECL_ON_FUNC( EVENT_BUY_FOR_GP_ACK );
    DECL_ON_FUNC( EVENT_BUY_FOR_GEM_ACK );
    DECL_ON_FUNC( EVENT_BUY_FOR_CRYSTAL_ACK );
    //DECL_ON_FUNC( EVENT_BUY_COUNT_FOR_GP_ACK );
    DECL_ON_FUNC( EVENT_QUERY_INVENTORY_INFO_ACK );
    DECL_ON_FUNC( EVENT_SELL_ITEM_ACK );
    DECL_ON_FUNC( EVENT_STRONG_LEVELUP_ACK );
    DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_ACK );
    _DECL_ON_FUNC( ENU_WAIT_TIME_NOT, int );
    _DECL_ON_FUNC( ENU_SERVER_LIST_NOT, int );
    DECL_ON_FUNC( EMS_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( EMS_DELETE_FRIEND_REQ );

    //STATE pre-CHANNELLOBBY
    DECL_ON_FUNC_NOPARAM( ACTION_CONNECT );
    DECL_ON_FUNC_NOPARAM( ACTION_VERIFY );

    DECL_ON_FUNC_NOPARAM( ACTION_MSG_CONNECT );

    //STATE CHANNELLOBBY
    DECL_ON_FUNC_NOPARAM( ACTION_CH_LIST );
    DECL_ON_FUNC_NOPARAM( ACTION_NOTIFY );
    DECL_ON_FUNC_NOPARAM( ACTION_CHATTING );
    DECL_ON_FUNC_NOPARAM( ACTION_WHISPER );
    DECL_ON_FUNC_NOPARAM( ACTION_ENTER_CH );

    //STATE CHANNEL
    DECL_ON_FUNC_NOPARAM( ACTION_ROOM_LIST );
    DECL_ON_FUNC_NOPARAM( ACTION_USER_LIST );
    //DECL_ON_FUNC_NOPARAM( ACTION_WHERE_USER );
    DECL_ON_FUNC_NOPARAM( ACTION_LEAVE_CH );
    DECL_ON_FUNC_NOPARAM( ACTION_JOIN_ROOM );
    DECL_ON_FUNC_NOPARAM( ACTION_QUICK_JOIN_ROOM );
    DECL_ON_FUNC_NOPARAM( ACTION_CREATE_ROOM );

    //BUY Item
    DECL_ON_FUNC_NOPARAM( ACTION_BUY_CERT );
    DECL_ON_FUNC_NOPARAM( ACTION_BUY_FOR_GP );
    DECL_ON_FUNC_NOPARAM( ACTION_BUY_FOR_GEM );
    DECL_ON_FUNC_NOPARAM( ACTION_BUY_FOR_CRYSTAL );
    //DECL_ON_FUNC_NOPARAM( ACTION_BUY_COUNT_FOR_GP );
    DECL_ON_FUNC_NOPARAM( ACTION_QUERY_INVENTORY );
    //DECL_ON_FUNC_NOPARAM( ACTION_EQUIP_ITEM );
    DECL_ON_FUNC_NOPARAM( ACTION_SELL_ITEM );
    DECL_ON_FUNC_NOPARAM( ACTION_STRONG );
    
    //STATE ROOM
    DECL_ON_FUNC_NOPARAM( ACTION_INVITE );
    DECL_ON_FUNC_NOPARAM( ACTION_LEAVE_ROOM );
    DECL_ON_FUNC_NOPARAM( ACTION_START_GAME );

    //STATE GAME
    DECL_ON_FUNC_NOPARAM( ACTION_USE_ITEM );
    DECL_ON_FUNC_NOPARAM( ACTION_END_GAME );

    //FOR MSG TEST, MY DATA CHANGE AND UPDATE NOTIFY
    DECL_ON_FUNC_NOPARAM( ACTION_CHANGE_STATE );
    DECL_ON_FUNC_NOPARAM( ACTION_UPDATE_MYDATA );
    DECL_ON_FUNC_NOPARAM( ACTION_WAIT );

    DECL_ON_FUNC_NOPARAM( ACTION_DISCONNECT );

    //bool InsertIntoUserInventory( IN std::vector<KItemInfo>& kItemInfo, IN std::vector<KDurationItemInfo>& kDurationItemInfo );
    bool InsertIntoUserDurationInventory( IN std::vector<KDurationItemInfo>& kDurationItemInfo );

public: // for KncP2P
    void SetRobotID( int iSingleRobotID_ );
    //KNC P2P HANDLER 

    /*
    2013.   7.    29
    추가내용
    */
    bool SetClientCommandQue(std::vector<int> comque);
    void WriteFileClientEnterChannelList();
    bool checkChannelAck;
    void SetChannel( DWORD ChannelID);
    int GetComqueCount();
    int m_iRoomMode;
    void SetRoomMode(int RoomMode);
    bool m_bcheckDisconnect;
    
protected:
    //050518. microcat.
    KEVENT_VERIFY_ACCOUNT_ACK               m_kUserData;
    std::vector<KChannelInfo>               m_vecChannelInfo;
    std::vector<KSimpleRoomInfo>            m_vecRoomInfo;
    std::vector<KUserInfo>                  m_vecUserInfo;
    std::vector<std::wstring>               m_vecLocation; 
    //std::vector<KGuildChannelUserInfo>      m_vecGuildChannelUserInfo;
    unsigned short                          m_usWaitingID;
    int                                     m_iSingleRobotID;
    bool                                    m_bConnectToCenter;
    KChannelInfo                            m_kChannelInfo;
    KRoomInfo                               m_kRoomInfo;
    KncP2P*                                 m_pKncP2P;
    KInDoorUserInfo                         m_kUserInfo;
    KNUserProxy*                            m_pkNUserProxy;
    KMSProxy*                               m_pkMSProxy;
    //KUserProxy*                               m_pkUserProxy;

    //boost::shared_ptr< KMSProxy >           m_pkMSProxy;	
    
    KFriendInfo                             m_kMyInfo;

    DWORD                                   m_dwLastSendTime;
    DWORD                                   m_dwDisconnectedTick;               // 접속이 끊기고 나서는 일정 시간동안 재접속하지 않는다.
    DWORD                                   m_dwLatestActionTick;               // 가장 마지막으로 행한 액션의 tick.
    bool                                    m_bOnWaitingAck;                    ///< ACK를 받기 전까지는 아무 액션도 하지 않는다.
    static KActionDecider                   ms_kActionDecider;
    static const char*                      ms_szAction[];
    static bool                             ms_bAutoAction;                     // true일 경우 액션 테이블에 근거한 행동을 취한다.
    static int                              ms_nProtocolVer;                    // protocol version은 빈번히 변경될 수 있다.
    static std::string                      ms_strRemoteIP;                     // 접속할 서버 IP
    static std::string                      ms_strCenterRemoteIP;               // 접속할 서버 IP
    static unsigned short                   ms_usRemoteGameServerPort;          // 접속할 게임서버 Port.
    static unsigned short                   ms_usRemoteCenterServerPort;        // 접속할 센터서버 Port.
    static std::wstring                     ms_strRobotTestMsg;
    static KSerBuffer                       ms_kSerBuffer;
    static DWORD                            ms_dwRobotTestMsgSendTime;

    static std::string                      ms_strMsgRemoteIP;                  // 접속할 메신저 서버 IP
    static unsigned short                   ms_usRemoteMsgServerPort;           // 접속할 메신저 서버 Port.
    static std::string                      ms_strMsgRemoteIP2nd;               // 접속할 메신저 서버 IP , 2번째
    static unsigned short                   ms_usRemoteMsgServerPort2nd;        // 접속할 메신저 서버 Port, 2번째

    std::map<DWORD, KFriendInfo>            m_mapFriendList;                    // 친구 리스트, 메신저 서버 테스트할 때 필요
    friend class KMassiveRobot;

    bool                                    m_bFirstRun;
    static bool                             ms_bLoginOnly; // 로그인 전용 테스트. 중국에서 빌링(인증) 서버 부하 테스트를 위해서 로긴에만 접속하는 기능. 게임서버 봇은 따로 실행해야 한다.
    /*
    2013.   7.    29
    추가내용
    */
    std::vector<int>                        CommandQue;
    int                                     m_countCommand;
    bool                                    ClientLive;
    int                                     m_iCheckCountClinet;
    DWORD                                   m_iChannelID;
};
