#pragma once
#include <WinSock2.h>
//#include "Actor.h"
#include "Socket/Session.h"
#include "Thread/Thread.h"
#include "Socket/SocketObject.h"
#include "FSM/support_FSM.h"
#include <Serializer/Serializer.h>
#include <Serializer/SerBuffer.h>
#include <RTTI.h>
#include <KncSmartPtr.h>
#include <boost/enable_shared_from_this.hpp>

SmartPointer( KTRUser );

class KTRUser : public KSession // public KThread, public boost::enable_shared_from_this<KTRUser>
{
    DeclareFSM_MT;  // un-proxy side only.
    NiDeclareRootRTTI( KTRUser );   // Accepter에서 처리될 객체들은 Ni-RTTI가 선언되어 있어야 한다.

public:
    enum ENUM_TICKS // Tick 관련
    {
        CONNECT_TICK = 0,
        HB_TICK,            // heart_bit
        TICKS_NUM,
    };

    enum {
        EVENTID_RELAYED     = -2,
        EVENTID_HEART_BIT   = -1,
        EVENTID_REG_UID     = 0,
    };

    enum { 
        MINIMUM_PACKET_SIZE = 3, 
        MAXIMUM_PACKET_SIZE = 512 
    };

    KTRUser(void);                              // do not use.
    KTRUser(const KTRUser& right);              // do not use.
    KTRUser& operator=(const KTRUser& right);   // do not use.


    KTRUser( bool bIsProxy );
    virtual ~KTRUser(void);

    //static KSocketObject* Create();
    static KTRUserPtr Create();
    static void AddManager( KTRUserPtr spTRUser );

    virtual bool Begin();   //derived From KThread
    void Tick();

    virtual bool Init( bool bUseIocp );
    virtual bool Connect( const char* szIP, unsigned short usPort, DWORD dwUID ); // PROXY_ONLY

    KSocketObject* GetSockObj()         { return &m_kSockObj; }
    DWORD GetUID()const                 { return m_dwUID; }
    void SetHwnd( HWND hWnd )           { m_hWnd = hWnd; }

    bool Send( IN const std::vector<unsigned char>& buff );     // UNPROXY_ONLY
    bool Send( IN const std::vector<DWORD> vecUID, IN const void* pvData, IN const size_t dwDataSize ); // PROXY_ONLY

    void ReserveDestroy() { m_bDestroyReserved = true; }

    void SetClassID( int iClassID ) { m_iClassID = iClassID; }
    int GetClassID() { return m_iClassID; }

    KTRUserPtr GetThisPtr() { return boost::static_pointer_cast<KTRUser>( (*this).shared_from_this() );}

    //void ResetTRUser() { m_kSockObj.m_pkTRUser = NULL;  }
protected:
    virtual void Run();
    void OnSocketError();  

    void SetTick( IN ENUM_TICKS eIndex )    { m_adwTickCount[ eIndex ] = ::GetTickCount(); }
    DWORD GetTick( IN ENUM_TICKS eIndex)    { return m_adwTickCount[ eIndex ]; }

    void OnRecvCompleted( IN const char cRecvCnt, IN KSerBuffer& buff );
    void OnDestroy();

    void OnAcceptConnection();     // UNPROXY_ONLY
    void    SetKey( DWORD dwKey );//   { m_kSockObj.SetKey( dwKey ); }
    DWORD   GetKey() const          { return m_kSockObj.GetKey(); }

protected:
    class KSkTRUser : public KSocketObject {
    public:
    //    enum { MINIMUM_PACKET_SIZE = 3, MAXIMUM_PACKET_SIZE = 512 };
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError() { m_pkTRUser->OnSocketError(); }
        void OnAcceptConnection() { m_pkTRUser->OnAcceptConnection(); }
        KTRUser* m_pkTRUser;
    } m_kSockObj;

    void SetSendQueueSize( int nSize ) { m_kSockObj.SetSendQueueSize( nSize ); }

    bool                m_bUseIocp;
    bool				m_bIsProxy;
    bool				m_bDestroyReserved;
    DWORD               m_dwUID;
    DWORD               m_adwTickCount[TICKS_NUM];
    HWND                m_hWnd;
    int                 m_iClassID;
    int                 m_nPacketSendCount; // 패킷 보낸 Count.
    DWORD               m_dwConnectTime; // 접속 시간.

    // proxy-chile only.
    enum ENUM_EVENT_TYPE {    
        EVENT_DISCONNECTED,
        EVENT_RECV_COMPLETED,        
        EVENT_SEND_COMPLETED,        
        EVENT_MAX_VALUE
    };
    HANDLE              m_hEvents[EVENT_MAX_VALUE];
};
