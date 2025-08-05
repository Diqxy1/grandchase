#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

	_ENUM(EVENT_HEART_BIT = 0)
	_ENUM(EVENT_ACCEPT_CONNECTION_NOT = 1)
	//=================================================================================================

	// Event TRServer
	_ENUM(DB_EVENT_FIRST_SERVER_INFO_REQ)
	_ENUM(DB_ETR_TYPE_COUNT_STAT)

	_ENUM(EVENT_EVENTID_SENTINEL) //패킷의 끝

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
