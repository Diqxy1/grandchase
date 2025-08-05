#ifndef _KNCCOMMONDEFINE_H_
#define _KNCCOMMONDEFINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define                         KNC_BROADCAST                               0x00000000
#define                         KNC_BROADCAST_NOT_ME                        0xFFFFFFFF
#define                         KNC_RELAY_BROADCAST_NOT_ME                  0xFFFFFFFE
#define							KNC_RELAY_TO_ME								0xFFFFFFFD
#define							KNC_TCP_RELAY_TO_ME   						0xFFFFFFFC

#define                         DEFAULT_PORT                                32500
#define                         DEFAULT_PORT_RANGE                          100
//#define                         DEFAUlT_P2P_CONNECT_TIMEOUT                 15
#define                         DEFAUlT_REGISTER_TO_RELAYSERVER_COUNT       10
#define                         DEFAULT_MAX_PEER                            8
#define                         KNC_SYSTEM_PACKET_ORDER_CHANNEL             1
#define                         KNC_GAME_PACKET_ORDER_CHANNEL               2
#define							KNC_DEFAULT_MTU								1400
#define							KNC_MAX_PING_LOSS_COUNT_FOR_DISCONNECT		3
#define							KNC_PING_AVERAGE_COUNT						5
//////////////////////////////////////////////////////////////////////////
//                          ThreadRecv Timer
//////////////////////////////////////////////////////////////////////////
namespace KncTimer
{
#define                         TIME_HEARTBIT                               10000
#define                         TIME_HEARTBIT_CHECK                         100000
#define                         TIME_SERVER_ECHO                            300
#define                         TIME_KEEP_UP_SERVER_ECHO_PORT               30000
#define                         TIME_CONNECT_P2P                            200
#define                         TIME_REGISTER_TO_RELAYSERVER                200
#define                         TIME_KEEP_UP_RELAYSERVER_CONNECTION         20000
#define                         TIME_FINISH_CONNECTION                      7000
#define							TIME_CONDITION_CHECK						1000
#define							TIME_FINISH_CONDITION_CHECK					10000
    enum
    {
        T_HEARTBIT = 0,
        T_HEARTBIT_CHECK,
        T_SERVER_ECHO,
        T_KEEP_UP_SERVER_ECHO_PORT,
        T_CONNECT_P2P,
        T_REGISTER_TO_RELAYSERVER,
        T_KEEP_UP_RELAYSERVER_CONNECTION,
        T_FINISH_CONNECTION,
        T_CONNECT_PART_P2P,
        T_FINISH_PART_CONNECTION,
		T_CONDITION_CHECK,
		T_FINISH_CONDITION_CHECK,
        T_END,
    };
}
namespace KncUserCount
{
    enum EUserCount
    {
        ID_TOTAL = 0,
        ID_P2P,
        ID_RELAY,
    };
}


//////////////////////////////////////////////////////////////////////////
//                          Packet Reliability
//////////////////////////////////////////////////////////////////////////
enum SendReliability
{   
    _UNRELIABLE,
    _UNRELIABLE_SEQUENCED,
    _RELIABLE,              // 자주 보내는 패킷에 이거 넣으면 절대 안된다.
    _RELIABLE_ORDERED,      // 자주 보내는 패킷에 이거 넣으면 절대 안된다.
    _RELIABLE_SEQUENCED     // 자주 보내는 패킷에 이거 넣으면 절대 안된다.
};
//////////////////////////////////////////////////////////////////////////
//          DIRECT PLAY DEFINE과 STRUCT
//////////////////////////////////////////////////////////////////////////
namespace DPInterface
{

    typedef bool (WINAPI *KNCMSGHANDLER)(DWORD dwMessageType,PVOID pMessage);
    class KncMsgHandler_CS
    {
    public:
        CRITICAL_SECTION    m_csMsgHandler;
        KNCMSGHANDLER       m_pMsgHandler;
        KncMsgHandler_CS( KNCMSGHANDLER pMsgHandler_ )
        {
            m_pMsgHandler = pMsgHandler_;
            ::InitializeCriticalSection( &m_csMsgHandler );
        }
        virtual ~KncMsgHandler_CS()
        {
            ::DeleteCriticalSection( &m_csMsgHandler );
        }
        void MsgProc( DWORD dwMessageType_ ,PVOID pMessage_ )
        {
            KLocker CSMsgHandler( m_csMsgHandler );
            m_pMsgHandler( dwMessageType_, pMessage_ );
        }
    };

    typedef DWORD	KNCID,		*PKNCID;
    typedef	DWORD	DPNHANDLE,	*PDPNHANDLE;

    typedef struct	_BUFFERDESC
    {
        DWORD	dwBufferSize;		
        BYTE * 	pBufferData;		
    } BUFFERDESC, KNC_BUFFER_DESC, *PKNC_BUFFER_DESC;

    typedef BUFFERDESC	FAR * PBUFFERDESC;

    typedef enum 
    {
        KNC_MSGID_CONNECT_COMPLETE,
        KNC_MSGID_RECEIVE,  
        KNC_MSGID_PING,
    }KNC_MSGID;

    typedef struct _KNCMSG_CONNECT_COMPLETE                 // KNC_MSGID_CONNECT_COMPLETE
    {
        std::vector<DWORD> vecUID;
    } KNCMSG_CONNECT_COMPLETE, *PKNCMSG_CONNECT_COMPLETE;

    typedef struct                                          // KNC_MSGID_RECEIVE
    {
        DWORD       dwUID;
        DWORD       dwReceiveDataSize;
        PBYTE       pReceiveData;
    } KNCMSG_RECEIVE, *PKNCMSG_RECEIVE;
    typedef struct _KNCMSG_PING                             // KNC_MSGID_PING
    {
        DWORD       dwPing;
    } KNCMSG_PING, *PKNCMSG_PING;
}
//////////////////////////////////////////////////////////////////////////
//                  Connect에 필요한 구조체
//////////////////////////////////////////////////////////////////////////
struct KConnectInfo
{
    DWORD                   m_dwUID;
    std::vector<u_short>    m_vecPort;
    std::vector<DWORD>      m_vecIP;
    KConnectInfo()
    {
        m_dwUID = 0;
        m_vecIP.clear();
        m_vecPort.clear();
    }
    bool operator == ( DWORD dwUID )
    {
        return dwUID == m_dwUID;
    }
    bool operator != ( DWORD dwUID )
    {
        return dwUID != m_dwUID;
    }
};

//////////////////////////////////////////////////////////////////////////
//                               MACRO
//////////////////////////////////////////////////////////////////////////
#define					        INITCS(n)		                			::InitializeCriticalSection(&n)
#define                         DELETECS(n)                                 ::DeleteCriticalSection(&n)
#define                         LOCKET(n)                                   KLocker CS_##n( n );

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          if (p) { delete (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    if (p) { delete[] (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE_ARRAY

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         if (p) { (p)->Release(); (p) = NULL; }
#endif // #ifndef SAFE_RELEASE


//////////////////////////////////////////////////////////////////////////
//                              GAME MSG
//////////////////////////////////////////////////////////////////////////
#define         ADD_GAME_MSG_CONNECT_COMPLETE( vecUID )                                         \
do                                                                                              \
{                                                                                               \
    KNCMSG_CONNECT_COMPLETE connectComplete;                                                    \
    connectComplete.vecUID  = vecUID;                                                           \
    m_pMsgHandler->MsgProc( KNC_MSGID_CONNECT_COMPLETE, &connectComplete );                     \
} while(0)
#define         ADD_GAME_MSG_RECEIVE( dwUID_, iRecvDataSize, pData )                            \
do                                                                                              \
{                                                                                               \
    KNCMSG_RECEIVE msgRecv;                                                                     \
    msgRecv.dwReceiveDataSize = iRecvDataSize;                                                  \
    msgRecv.dwUID = dwUID_;                                                                     \
    msgRecv.pReceiveData = (PBYTE)pData;                                                        \
    m_pMsgHandler->MsgProc( KNC_MSGID_RECEIVE, &msgRecv );                                      \
} while(0)
#define         ADD_GAME_MSG_PING( dwPing_ )                                                    \
do                                                                                              \
{                                                                                               \
    KNCMSG_PING kncPing;                                                                        \
    kncPing.dwPing = dwPing_;                                                                   \
    m_pMsgHandler->MsgProc( KNC_MSGID_PING, &kncPing );                                         \
} while(0)


class KncStopWatch
{
public:
    KncStopWatch(){}
    KncStopWatch( DWORD dwEndTime_ )
    {
        m_dwPastTime = timeGetTime();
        m_dwCurTime = timeGetTime();
        m_dwAccumulateTime = 0;
        m_dwEndTime = dwEndTime_;
        m_bStopCheckStop = false;
    }
    virtual ~KncStopWatch(void){}
    DWORD m_dwPastTime;
    DWORD m_dwCurTime;
    DWORD m_dwAccumulateTime;
    DWORD m_dwEndTime;
    bool  m_bStopCheckStop;

    void Init()
    {
        m_dwPastTime = 0;
        m_dwCurTime = 0;
        m_dwAccumulateTime = 0;
        m_dwEndTime = 0;
        m_bStopCheckStop = false;
    }
    void SetStopWatch( DWORD dwEndTime_ )
    {
        Init();
        m_dwPastTime = timeGetTime();
        m_dwCurTime = timeGetTime();
        m_dwAccumulateTime = 0;
        m_dwEndTime = dwEndTime_;
    }
    bool IsStop()
    {
        if( m_bStopCheckStop )
            return false;

        m_dwCurTime = timeGetTime();
        m_dwAccumulateTime = m_dwAccumulateTime + ( m_dwCurTime - m_dwPastTime );
        m_dwPastTime = m_dwCurTime;
        if ( m_dwAccumulateTime >= m_dwEndTime )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void SetStopCheck( bool bStop_ ) { m_bStopCheckStop = bStop_; }
};


#endif // _KNCCOMMONDEFINE_H_


