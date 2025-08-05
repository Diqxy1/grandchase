// LatencyTransfer.h: interface for the CLatencyTransfer class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_LATENCYTRANSFER_H__A4B10058_C600_4E97_AF0B_9FFE0C94DABA__INCLUDED_)
//#define AFX_LATENCYTRANSFER_H__A4B10058_C600_4E97_AF0B_9FFE0C94DABA__INCLUDED_

#ifndef _LATENCYTRANSFER_H_
#define _LATENCYTRANSFER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LTPacket.h"

class CLatencyTransfer  
{
	public:
		CLatencyTransfer();
		virtual ~CLatencyTransfer();

		void FrameMove( float fElapsedTime = 0.01818f );

		void PushPacket( LTP_BASIC* packet, int iSendType = 0 );		
		void PacketHandler( LTP_BASIC* packet );
		int ClearGamePacket( );
		int ClearPacket(LT_PACKET_LIST Type );
	public:
		void SendMagicEffectSynchPacket(int iMagicEffect, float fTime, bool bIsDamageUnitPlayer, int iDamageUnit, int iMagicLevel, DWORD dwAttackUnit = 0 );
	private:
		vector<LTP_BASIC*>		m_LatencyPacketList;
};

extern CLatencyTransfer g_LatencyTransfer;

#endif // _LATENCYTRANSFER_H_
