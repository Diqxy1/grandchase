#pragma once
#include <WinSock2.h>
#include "Thread/Thread.h"
#include "Socket/SocketObject.h"
#include "FSM/support_FSM.h"
#include <Serializer/Serializer.h>
#include <Serializer/SerBuffer.h>
#include <RTTI.h>
#include <KncSmartPtr.h>
//#include <boost/enable_shared_from_this.hpp>

SmartPointer( KTRUser );

class KTRUser : public KThread, public boost::enable_shared_from_this<KTRUser>
{
    DeclareFSM_MT;  // un-proxy side only.
    NiDeclareRootRTTI( KTRUser );   // Accepter에서 처리될 객체들은 Ni-RTTI가 선언되어 있어야 한다.

public:
    enum ENUM_TICKS // Tick 관련
    {
        CONNECT_TICK = 0,
        HB_TICK,            // heart_bit
        PING_TICK,          // ping
        TICKS_NUM,
    };

    enum {
        EVENTID_RELAYED     = -2,
        EVENTID_HEART_BIT   = -1,
        EVENTID_REG_UID     = 0,
    };

    enum {
        SUBTYPE_NORMAL      = 0,
        SUBTYPE_PING,
    };

    KTRUser(void);                              // do not use.
    KTRUser(const KTRUser& right);              // do not use.
    KTRUser& operator=(const KTRUser& right);   // do not use.


    KTRUser( bool bIsProxy );
    virtual ~KTRUser(void);

    static KSocketObject* Create();
    
    virtual bool Begin();   //derived From KThread
    void Tick();

    virtual bool Init( bool bUseIocp );
    virtual bool Connect( const char* szIP, unsigned short usPort, DWORD dwUID ); // PROXY_ONLY

    KSocketObject* GetSockObj()         { return &m_kSockObj; }
    DWORD GetUID()const                 { return m_dwUID; }
    void SetHwnd( HWND hWnd )           { m_hWnd = hWnd; }

    bool Send( IN const std::vector<unsigned char>& buff, int& nErrorCode );     // UNPROXY_ONLY
    bool Send( IN const std::vector<DWORD>& vecUID, IN const void* pvData, IN const size_t dwDataSize, int& nErrorCode, DWORD dwSubType = SUBTYPE_NORMAL ); // PROXY_ONLY
    bool SendPing();

    void ReserveDestroy() { m_bDestroyReserved = true; }

    void SetClassID( int iClassID ) { m_iClassID = iClassID; }
    int GetClassID() { return m_iClassID; }

    bool IsConnected() const			{ return m_kSockObj.IsConnected(); }

    DWORD GetPing() const { return m_dwPing; }

    DWORD PingContinuousRTOCount() const { return m_dwPingContinuousRTOCount; }
    DWORD GetPingSendGap() const { return m_dwPingSendGap; }
    void SetPingSendGap(DWORD val) { m_dwPingSendGap = val; }
    DWORD GetPingTimeoutLimit() const { return m_dwPingTimeoutLimit; }
    void SetPingTimeoutLimit(DWORD val) { m_dwPingTimeoutLimit = val; }


    bool IsConnectCheck();
    bool IsReConnectCheck();

protected:
    virtual void Run();

    void Tick_Heartbit();
    void Tick_Ping();
    void OnSocketError( int nType );  

    void SetTick( IN ENUM_TICKS eIndex )    { m_adwTickCount[ eIndex ] = ::GetTickCount(); }
    DWORD GetTick( IN ENUM_TICKS eIndex)    { return m_adwTickCount[ eIndex ]; }
    void SetPing(DWORD val) { m_dwPing = val; }

    void ProcSubtype( IN KSerBuffer* pBuff );

    void OnRecvCompleted( IN const char cRecvCnt, IN KSerBuffer& buff );
    void OnDestroy();

    void OnAcceptConnection();     // UNPROXY_ONLY

protected:
    class KSkTRUser : public KSocketObject {
    public:
        enum { MINIMUM_PACKET_SIZE = 3, MAXIMUM_PACKET_SIZE = 512 };
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError( int nType ) 
        {
            if(m_pkTRUser)                                    
                m_pkTRUser->OnSocketError( nType );
        }
        void OnAcceptConnection() { if(m_pkTRUser)
                                    m_pkTRUser->OnAcceptConnection(); }
        KTRUser* m_pkTRUser;
    } m_kSockObj;

    bool                m_bUseIocp;
    bool				m_bIsProxy;
    bool				m_bDestroyReserved;
    DWORD               m_dwUID;
    DWORD               m_adwTickCount[TICKS_NUM];
    HWND                m_hWnd;
    int                 m_iClassID;

    DWORD               m_dwPing;
    DWORD               m_dwPingUID;
    DWORD               m_dwPingSendGap;
    DWORD               m_dwPingTimeoutLimit;
    DWORD               m_dwPingContinuousRTOCount;
    bool                m_bPingRecv;
    
    //  게임중 Connect 체크하자.
    bool                m_bConnectCheck;
    bool                m_bReConnect;
    DWORD               m_dwDisConnectTick;                

    // proxy-chile only.
    enum ENUM_EVENT_TYPE {    
        EVENT_DISCONNECTED,
        EVENT_RECV_COMPLETED,        
        EVENT_SEND_COMPLETED,        
        EVENT_MAX_VALUE
    };
    HANDLE              m_hEvents[EVENT_MAX_VALUE];
};
