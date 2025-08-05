#ifndef _KNCPACKET_H_
#define _KNCPACKET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KncPeer.h"
#include "./RakNetSrc/PacketEnumerations.h"

#pragma pack( push, 1 )

#undef  KNC_PACKET_DEF
#define KNC_PACKET_DEF( ID, COMMENT )           ID,

typedef enum
{
    PID_START = ID_END_RAKNETID + 2,
#include "KncPacketDef.h"
    PID_END
}KNC_PID;
#define				PID_RECV					PID_END + 1

#define             DECL_KNC_PACKET(ID)         struct K##ID

DECL_KNC_PACKET( PID_CONNECT_REQ )
{
    KPID_CONNECT_REQ(){}
    KPID_CONNECT_REQ( DWORD dwUID_ ){ dwUID = dwUID_; }
    DWORD dwUID;
};
//DECL_KNC_PACKET( PID_CONNECT_ACK )
//{
//    KPID_CONNECT_ACK(){}
//    KPID_CONNECT_ACK( DWORD dwUID_ ){ dwUID = dwUID_; }
//    DWORD dwUID;
//};
DECL_KNC_PACKET( PID_GAME_MSG )
{
    KPID_GAME_MSG(){}
    KPID_GAME_MSG( DWORD dwUID_, int iDataSize_, DWORD dwConnectionUID_ ){ dwUID = dwUID_; iDataSize = iDataSize_; dwConnectionUID = dwConnectionUID_; }
    DWORD dwUID;
    int   iDataSize;
	DWORD dwConnectionUID;
};

DECL_KNC_PACKET( PID_HEART_BIT_REQ )
{
    KPID_HEART_BIT_REQ(){}
    KPID_HEART_BIT_REQ( DWORD dwUID_, DWORD dwCreateTime_ ){ dwUID = dwUID_; dwCreateTime = dwCreateTime_; }
    DWORD dwUID;
    DWORD dwCreateTime;
};
DECL_KNC_PACKET( PID_HEART_BIT_ACK )
{
    KPID_HEART_BIT_ACK(){}
    KPID_HEART_BIT_ACK( DWORD dwUID_, DWORD dwCreateTime_ ){ dwUID = dwUID_; dwCreateTime = dwCreateTime_; }
    DWORD dwUID;
    DWORD dwCreateTime;
};

DECL_KNC_PACKET( PID_CHECK_P2P_CONDITION_REQ )
{
	KPID_CHECK_P2P_CONDITION_REQ(){}
	KPID_CHECK_P2P_CONDITION_REQ( DWORD dwUID_, DWORD dwCreateTime_ ){ dwUID = dwUID_; dwCreateTime = dwCreateTime_; }
	DWORD dwUID;
	DWORD dwCreateTime;
};
DECL_KNC_PACKET( PID_CHECK_P2P_CONDITION_ACK )
{
	KPID_CHECK_P2P_CONDITION_ACK(){}
	KPID_CHECK_P2P_CONDITION_ACK( DWORD dwUID_, DWORD dwCreateTime_ ){ dwUID = dwUID_; dwCreateTime = dwCreateTime_; }
	DWORD dwUID;
	DWORD dwCreateTime;
};
#pragma pack(pop)

#endif // _KNCPACKET_H_