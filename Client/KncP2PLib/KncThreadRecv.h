#ifndef _KNCTHREADRECV_H_
#define _KNCTHREADRECV_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KncCommonHeader.h"
#include <Thread/Thread.h>
#include "KncMsgProc.h"
#include "KncP2PError.h"
using namespace KncTimer;
class KncThreadRecv;
typedef void (KncThreadRecv::*KncTimerFunc)( WPARAM wParam_ , LPARAM lParam_ );
//UINT = ID, DWORD = LIMITTIME, DWORD = CurTime, KncTimerFunc = Func
typedef std::map< UINT, std::pair< DWORD, std::pair<DWORD, KncTimerFunc> > > KncTimerMsgMap;
#define KNC_TIMER_MSGMAP( ID, TIME, Func ) m_mapTimerFunc[ID] = std::make_pair(TIME, std::make_pair(0, Func));
#define _KNC_TIMER_MSGMAP( ID, TIME, INIT_TIME, Func ) m_mapTimerFunc[ID] = std::make_pair(TIME, std::make_pair(INIT_TIME, Func));

#define DECL_TIMER_HANDLER( ID )            virtual void ON_##ID( WPARAM  wParam_, LPARAM lParam_ )
#ifdef IMPL_TIMER_HANDLER
#undef IMPL_TIMER_HANDLER
#endif
#define IMPL_TIMER_HANDLER( ID )            void KncThreadRecv::ON_##ID( WPARAM  wParam_, LPARAM lParam_ )

typedef std::map< DWORD, KncTimerMsgMap > KncPartTimerMsgMap;
#define KNC_PART_TIMER_MSGMAP( UID, ID, TIME, Func ) m_mapPartTimerFunc[UID][ID] = std::make_pair(TIME, std::make_pair(0, Func));
#define _KNC_PART_TIMER_MSGMAP( UID, ID, TIME, INIT_TIME, Func ) m_mapPartTimerFunc[UID][ID] = std::make_pair(TIME, std::make_pair(INIT_TIME, Func));


class KncThreadRecv : public KncMsgProc
{
public:
    //====================================================================================
    // 2006.07.04 : Asirion
    KncTimerMsgMap          m_mapTimerFunc;
    std::vector< DWORD >    m_vecRemoveTimerID;
    //변수
    DWORD                   m_dwPastTime;
    DWORD                   m_dwCurTime;
    DWORD                   m_dwElapsedTime;

    bool                    m_bRecvEchoPort;
	bool					m_bServerDown;
    bool                    m_bInit;
    u_short                 m_usServerPort;
    DWORD                   m_dwServerIP;

    // 함수
	KncThreadRecv( RakPeer* pRakPeer, KncPeerManager* pPeerManager, KncMsgHandler_CS* pMsgHandler_ );
	virtual ~KncThreadRecv(void);
    virtual void SendServerEcho();
	virtual void SetEchoPort(  DWORD dwIP_, u_short usPort_ );
	virtual void SetServerDown( bool bDown ) { m_bServerDown = bDown; }
    virtual void OnTimerFrameMove();
	virtual void OnPartTimerFrameMove();
    virtual void Connect();
    virtual void Init( DWORD dwServerIP_, u_short usServerPort_ );
    virtual void CalcElapsedTime();
    virtual void FrameMove();
    virtual void RemoveTimer( DWORD dwID_ );
    virtual void SetupRelayServer();


	virtual bool CloseConnection( DWORD dwUID_  );

	/************************************************************************/
	/*  2006.11.22 : Daeuk
	For Individual Connection
	/************************************************************************/
	KncPartTimerMsgMap         m_mapPartTimerFunc;
	std::map< DWORD, std::vector< UINT > >    m_mapRemovePartTimerID;

	void Connect( DWORD dwUID_ );
	void SendConnectReq( KConnectInfo kConnectInfo_ );
	void RemovePartTimer( DWORD dwUID_, UINT ID_ );



    DECL_TIMER_HANDLER( T_HEARTBIT );
    DECL_TIMER_HANDLER( T_HEARTBIT_CHECK );
    DECL_TIMER_HANDLER( T_SERVER_ECHO );
    DECL_TIMER_HANDLER( T_CONNECT_P2P );
    DECL_TIMER_HANDLER( T_KEEP_UP_SERVER_ECHO_PORT );

    DECL_TIMER_HANDLER( T_REGISTER_TO_RELAYSERVER );
    DECL_TIMER_HANDLER( T_KEEP_UP_RELAYSERVER_CONNECTION );
    DECL_TIMER_HANDLER( T_FINISH_CONNECTION );

	DECLARE_SYSMSG_HANDLER( PID_CHECK_P2P_CONDITION_REQ );
	DECLARE_SYSMSG_HANDLER( PID_CHECK_P2P_CONDITION_ACK );

	DECL_TIMER_HANDLER( T_CONNECT_PART_P2P );
	DECL_TIMER_HANDLER( T_FINISH_PART_CONNECTION );
	DECL_TIMER_HANDLER( T_CONDITION_CHECK );
	DECL_TIMER_HANDLER( T_FINISH_CONDITION_CHECK );

};

#endif // _KNCTHREADRECV_H_
