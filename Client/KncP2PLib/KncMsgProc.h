#ifndef _KNCMSGPROC_H_
#define _KNCMSGPROC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <set>
#include "KncCommonHeader.h"
#include "KncPeerManager.h"
#include "KncPacket.h"
#include "./RakNetSrc/RakPeer.h"
#include "../GCUtil/EnumString.h"
//////////////////////////////////////////////////////////////////////////
//                           MsgHander Macro
//////////////////////////////////////////////////////////////////////////
#define DECLARE_SYSMSG_HANDLER(ID)          virtual bool On_##ID(Packet* pPacket)
#define IMPLEMENT_SYSMSG_HANDLER(ID)        bool CLASS_TYPE##::On_##ID(Packet* pPacket)
#define FUNC_SYSMSG_HANDLER(ID)             return On_##ID(pPacket)

#ifdef _DEBUG
#define         CASE_RAK_PACKET(ID)         case(ID):						            \
                                            START_LOG( clog2, L"PACKET ID : " << #ID);  \
	                                        FUNC_SYSMSG_HANDLER(ID)
#define         _CASE_RAK_PACKET(ID)        case(ID):						            \
	                                        FUNC_SYSMSG_HANDLER(ID)

#define         CASE_KNC_PACKET(ID)         case(ID):                                   \
                                            START_LOG( clog2, L"PACKET ID : " << #ID);  \
	                                        FUNC_SYSMSG_HANDLER(ID)
#define         _CASE_KNC_PACKET(ID)        case(ID):                                   \
	                                        FUNC_SYSMSG_HANDLER(ID)

#else
#define         CASE_RAK_PACKET(ID)         case(ID):                                   \
	                                        FUNC_SYSMSG_HANDLER(ID)
#define         _CASE_RAK_PACKET(ID)        case(ID):						            \
	                                        FUNC_SYSMSG_HANDLER(ID)
#define         CASE_KNC_PACKET(ID)         case(ID):                                   \
	                                        FUNC_SYSMSG_HANDLER(ID)
#define         _CASE_KNC_PACKET(ID)        case(ID):                                   \
	                                        FUNC_SYSMSG_HANDLER(ID)

#endif

#define         _MAKE_BITSTREAM_FROM_PACKET( PID ,kBitStream )                          \
	RakNet::BitStream kBitStream;                                                       \
	kBitStream.Reset();                                                                 \
	kBitStream.Write((unsigned char)PID_START);                                         \
	kBitStream.Write((unsigned char)(PID))
#define         MAKE_BITSTREAM_FROM_PACKET( PID , pData, size, kBitStream )             \
	_MAKE_BITSTREAM_FROM_PACKET(PID, kBitStream);                                       \
	kBitStream.Write((char*)pData, size )
#define         GET_BITSTREAM_FROM_PACKET( PID, kPacket, kBitStream )                   \
	RakNet::BitStream kBitStream;                                                       \
	K##PID kPacket;                                                                     \
	kBitStream.Reset();                                                                 \
	kBitStream.Write( (char*)pPacket->data, pPacket->length );                          \
	kBitStream.IgnoreBits( 8 + 8 );                                                     \
	kBitStream.Read( (char*)&kPacket, sizeof(K##PID) )
#define         GET_UID_FROM_PACKET( dwUID, kBitStream )                      \
	RakNet::BitStream kBitStream;                                                       \
	DWORD dwUID = 0;                                                                    \
	kBitStream.Reset();                                                                 \
	kBitStream.Write( (char*)pPacket->data, pPacket->length );                          \
	kBitStream.IgnoreBits(8);                                                           \
	kBitStream.Read( (unsigned long )dwUID);                                            \

#define         GET_IP_FROM_PACKET( dwIP, kBitStream)       GET_UID_FROM_PACKET( dwIP, kBitStream )

#define                     RAK_MSG_ID_LENGTH                       1
#define                     KNC_MSG_ID_LENGTH                       1

#ifndef  __NEVER_DEFINED_MACRO__
#define  __NEVER_DEFINED_MACRO__
#include "KncP2PStateDef.h"
#undef   __NEVER_DEFINED_MACRO__
#endif

class KncP2P_T_T;

class KncMsgProc
{
public:
	friend class KncP2P;
	friend class KncP2P_T_T;
public:
	// 함수
	KncMsgProc( RakPeer* pRakPeer, KncPeerManager* pPeerManager, KncMsgHandler_CS* pMsgHandler_ );
	virtual ~KncMsgProc(void);

	void SetRelayOnly( bool bRelay_ ) { m_bRelayOnly = bRelay_; }

	
protected:
    //====================================================================================
    // 2006.07.04 : Asirion
    RakPeer*                    m_pRakPeer;
    KncPeerManager*             m_pPeerManager;
    KncMsgHandler_CS*           m_pMsgHandler;
    std::vector<KConnectInfo>   m_vecConnectQ;
    //====================================================================================
    // 2006.11.18 : Asirion
    // UDP Protocol의 특성 때문에 나에게 오지 않아야할 패킷 또는 오래 전에 누군가 보낸
    // 패킷들이 처리되어져 Connect되는 경우가 있다. 이를 해결하기 위하여 내가 Connection을
    // 맺어야 하는 유저의 UID를 따로 저장해 놓는다.
    std::set<DWORD>				m_setUID;
	//====================================================================================
	// 2006.07.20 : Asirion
	//내가 Connect를 요청한 녀석들의 UID. Connect함수를 1번이상 부르지 않게 하기 위함
	std::set<DWORD>				m_setConnectUID;

    ECONNECT_STATE              m_eConnectState;
    bool                        m_bRegisterToRelayServer;

	bool						m_bRelayOnly;
	// 함수
    //====================================================================================
    // 2006.07.04 : Asirion

	virtual bool AddConnectQ( KConnectInfo& kInfo_ );
    virtual bool RemoveConnectQ( DWORD dwUID_ );
	virtual bool RemoveConnectUID( DWORD dwUID_ );
	virtual bool RemoveConnectReqUID( DWORD dwUID_ );


    virtual bool ResetConnectInfo( const DWORD dwUID_, const PlayerID& kRakPeerID_ );

    virtual void Send( RakNet::BitStream& kBitStream_, DWORD dwUID_, SendReliability optReliability_ );

	virtual bool RakMsgProc( Packet* pPacket );
	virtual bool KncP2PMsgProc( Packet* pPacket );
    //====================================================================================

	virtual bool CloseConnection( DWORD dwUID_ );
    virtual void SetState( ECONNECT_STATE eState_ );

	//====================================================================================
	// 2008.05.12 : milennium9
	// 과거에 보낸 패킷을 무시하기 위한 UID
	DWORD						m_dwConnectionUID;

	void SetConnectionUID( DWORD dwConnectionUID_ );

    virtual void ResetConnectQ();
	DECLARE_SYSMSG_HANDLER( ID_CONNECTION_REQUEST_ACCEPTED );
	DECLARE_SYSMSG_HANDLER( ID_NEW_INCOMING_CONNECTION );
	DECLARE_SYSMSG_HANDLER( ID_ADVERTISE_SYSTEM );
	DECLARE_SYSMSG_HANDLER( ID_DISCONNECTION_NOTIFICATION );
	DECLARE_SYSMSG_HANDLER( ID_CONNECTION_LOST );
	DECLARE_SYSMSG_HANDLER( ID_REMOTE_PORT_REFUSED );
	DECLARE_SYSMSG_HANDLER( ID_PONG );
	DECLARE_SYSMSG_HANDLER( ID_NO_FREE_INCOMING_CONNECTIONS );
	DECLARE_SYSMSG_HANDLER( ID_RECEIVED_STATIC_DATA );
    DECLARE_SYSMSG_HANDLER( ID_REPORT_PUBLIC_IP );
#ifdef KNC_PACKET_DEF
#undef KNC_PACKET_DEF
#endif
#define KNC_PACKET_DEF(PID, COMMENT) DECLARE_SYSMSG_HANDLER(PID);
#include "KncPacketDef.h"
};

#endif // _KNCMSGPROC_H_