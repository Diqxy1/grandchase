#pragma once

#include "CommonPacket.h"
#include "GCEnum.h"

#pragma pack( push, 1 )

DECL_PACKET(EVENT_MATCH_VERIFY_ACCOUNT_ACK)
{
	int                 m_nOK;
	KSimpleServerInfo   m_kServerInfo;
	int                 m_nSID; // MatchServerID
	bool                m_bEnterance; // 입장가능한지
};

DECL_PACKET(EVENT_MATCH_ADD_USER_REQ)
{
	int                 m_nStartScope;
	KPartyData          m_kPartyData;

	KEVENT_MATCH_ADD_USER_REQ() :
		m_nStartScope(0)
	{}
};

DECL_PACKET(EVENT_MATCH_ADD_USER_ACK)
{
	int                 m_nOK;
	int                 m_nMatchListNum;
	KPartyData          m_kPartyData;
	int                 m_nWaitListCount;
};

DECL_PACKET(EVENT_MATCH_DEL_USER_ACK)
{
	int                 m_nOK;
	KPartyData          m_kPartyData;
	int                 m_nWaitListCount;
};

DECL_PACKET(EVENT_MATCH_FIND_MATCH_NOT)
{
	int                         m_nModeInfo;
	std::vector<KPartyData>     m_vecPartyData;
	int                         m_nLoopCount;
	int                         m_nWaitListCount;
};

#pragma pack( pop )