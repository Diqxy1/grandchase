#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM(EVENT_HEART_BIT = 0)
	_ENUM(EVENT_ACCEPT_CONNECTION_NOT = 1)
	//=================================================================================================

	// Event MatchServer
	_ENUM(EVENT_MATCH_VERIFY_ACCOUNT_REQ)
	_ENUM(EVENT_MATCH_VERIFY_ACCOUNT_ACK)

	_ENUM(DB_EVENT_FIRST_INIT_REQ)

	_ENUM(EVENT_MATCH_ADD_USER_REQ) // GS->MS 매칭 요청.
	_ENUM(EVENT_MATCH_ADD_USER_ACK) // MS->GS 매칭 요청에 대한 응답.
	_ENUM(EVENT_MATCH_DEL_USER_REQ) // GS->MS 매칭 취소 요청.
	_ENUM(EVENT_MATCH_DEL_USER_ACK) // MS->GS 매칭 취소 요청에 대한 응답.

	_ENUM(EVENT_MATCH_FIND_MATCH_NOT) // MS->GS 매칭 찾기 성공 알림.
	_ENUM(EVENT_MATCH_DISCONNECT_GAMESERVER_NOT) // MS->GS 게임서버 연결끊김 알림.
	//패킷의 끝
	_ENUM(EVENT_EVENTID_SENTINEL) //패킷의 끝

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
