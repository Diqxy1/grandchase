#include "MatchPacket.h"

SERIALIZE_DEFINE_PUT(KEVENT_MATCH_VERIFY_ACCOUNT_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kServerInfo) &&
		PUT(m_nSID) &&
		PUT(m_bEnterance);
};

SERIALIZE_DEFINE_GET(KEVENT_MATCH_VERIFY_ACCOUNT_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kServerInfo) &&
		GET(m_nSID) &&
		GET(m_bEnterance);
};

SERIALIZE_DEFINE_PUT(KEVENT_MATCH_ADD_USER_REQ, obj, ks)
{
	return  PUT(m_nStartScope) &&
		PUT(m_kPartyData);
};

SERIALIZE_DEFINE_GET(KEVENT_MATCH_ADD_USER_REQ, obj, ks)
{
	return  GET(m_nStartScope) &&
		GET(m_kPartyData);
};

SERIALIZE_DEFINE_PUT(KEVENT_MATCH_ADD_USER_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_nMatchListNum) &&
		PUT(m_kPartyData) &&
		PUT(m_nWaitListCount);
};

SERIALIZE_DEFINE_GET(KEVENT_MATCH_ADD_USER_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_nMatchListNum) &&
		GET(m_kPartyData) &&
		GET(m_nWaitListCount);
};

SERIALIZE_DEFINE_PUT(KEVENT_MATCH_DEL_USER_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kPartyData) &&
		PUT(m_nWaitListCount);
};

SERIALIZE_DEFINE_GET(KEVENT_MATCH_DEL_USER_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kPartyData) &&
		GET(m_nWaitListCount);
};

SERIALIZE_DEFINE_PUT(KEVENT_MATCH_FIND_MATCH_NOT, obj, ks)
{
	return  PUT(m_nModeInfo) &&
		PUT(m_vecPartyData) &&
		PUT(m_nLoopCount) &&
		PUT(m_nWaitListCount);
};

SERIALIZE_DEFINE_GET(KEVENT_MATCH_FIND_MATCH_NOT, obj, ks)
{
	return  GET(m_nModeInfo) &&
		GET(m_vecPartyData) &&
		GET(m_nLoopCount) &&
		GET(m_nWaitListCount);
};