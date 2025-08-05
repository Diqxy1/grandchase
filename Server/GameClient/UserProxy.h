#pragma once

#include "ActorProxy.h"
#include "UserEvent.h"
#include <windows.h>
#include "UserPacket.h"
#include "KNCSingleton.h"
#include "Socket/NetCommon.h"

#include <queue>
#include <boost/shared_ptr.hpp>

SmartPointer( KUserProxy );

class KUserProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclareSingleton( KUserProxy );
    DeclToStringW;

public:
    KUserProxy(void);
    virtual ~KUserProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& );
    virtual KEvent* CreateKEvent();
        
    //////////////////////////////////////////////////////////////////////////
    // Interfaces for Client Programmer.

    static void InitForClient();    // init winsock, security
    static void ReleaseForClient();

    //virtual bool Init( bool bUseIOCP );   // 로봇을 제작하는 경우 iocp를 사용하도록 셋팅 가능.
    //virtual bool Connect( const TCHAR* szIP, unsigned short usPort ); // 소켓 접속만 한다.

    //int Connect( IN const KServerInfo& kServerInfo, IN const KEventPtr spEvent, IN DWORD dwTimeOut );

    // 060305. florist. 서버 이동시 cached data가 문제를 일으켜, 싱크기능이 추가됨.
    // 060412. florist. 서버 이동 절차 강화. 확실한 req/ack 처리를 진행한다.
    bool RequestServerMigration( IN const KServerInfo& kInfo, DWORD dwTimeOut = 3000 );

protected:  // Recv Msg Handler & Util Function
    virtual void OnSocketError();
    //DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_ACK );
   _DECL_ON_FUNC( EVENT_SERVER_MIGRATION_ACK, int );
    //DECL_ON_FUNC( EVENT_CREATE_ROOM_ACK );

public:
    USHORT m_usRoomID;

};

DefSingletonInline( KUserProxy )