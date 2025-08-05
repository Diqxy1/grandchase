#pragma once
#include "ActorProxy.h"
//#include <windows.h>
#include "Thread/Thread.h"
#include "KNCSingleton.h"
#include "MSEvent.h"
#include "MSPacket.h"

SmartPointer( KMSProxy ); // 060804. florist. unit test에서 사용된다.

class KMSEvent;
class KMSProxy : public KActorProxy
{
    typedef KMSEvent IDVenderType;
    NiDeclareRTTI;
    DeclareSingleton( KMSProxy );
    DeclToStringW;

public:
    KMSProxy(void);
    virtual ~KMSProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    virtual KEvent* CreateKEvent() { return new KMSEvent; }

    //////////////////////////////////////////////////////////////////////////
    // Interfaces for Client Programmer.

    //virtual bool Init( bool bUseIOCP );   // 로봇을 제작하는 경우 iocp를 사용하도록 셋팅 가능.
    //virtual bool Connect( const TCHAR* szIP, unsigned short usPort ); // 소켓 접속만 한다.

    //int ConnectNAuth( IN const KServerInfo& kServerInfo, IN const KEventPtr spEvent, IN DWORD dwTimeOut );

protected:
    virtual void OnSocketError( int nType );

    DECL_ON_FUNC( EMS_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_ACK );
   //_DECL_ON_FUNC( ECN_ADD_USER_ACK, KECN_REG_USER_DATA );
   // DECL_ON_FUNC( ECN_SERVER_MIGRATION_ACK );
   // DECL_ON_FUNC( ECN_KICK_USER_REQ );
   //_DECL_ON_FUNC( EVENT_NOT_MSG_NOT, std::wstring );

    // SocketObject에 설정해둘 수 있지만, 재접속하는 경우 CLOSE_SOCKET해도 주소가 기억될 필요가 있다.
    std::string      m_strIP;
    unsigned short   m_usPort;
};

DefSingletonInline( KMSProxy );
