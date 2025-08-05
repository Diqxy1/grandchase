
#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL) VAL,
enum
{
#endif

_ENUM( EVENT_HEART_BIT = 0 )
_ENUM( EVENT_ACCEPT_CONNECTION_NOT = 1 )
//=================================================================================================

_ENUM( EMR_VERIFY_ACCOUNT_REQ )
_ENUM( EMR_VERIFY_ACCOUNT_ACK )
_ENUM( DB_EVENT_SERVER_INFO_REQ )
_ENUM( EMR_CLIENT_CONNECT_REPORT_REQ )      // ms -> (uniq)mr : 유저가 접속했음을 알린다
_ENUM( EMR_CLIENT_CONNECT_REPORT_ACK )      // 
_ENUM( EMR_LOGINNED_FRIEND_INFO_REQ )       // mr -> (all)ms : 접속중인 친구의 live data를 요청한다.
_ENUM( EMR_REMOTE_QUEUEING_REQ )
_ENUM( EMR_REMOTE_QUEUEING_ACK )
_ENUM( EMR_CLIENT_DISCONNECT_REPORT_REQ )   // ms -> (uniq)mr : 유저가 접속종료했음을 알린다 ( -> ms : EMR_CLIENT_LOGOFF_NOT )
_ENUM( EMR_CLIENT_DISCONNECT_REPORT_ACK )
_ENUM( EMR_KICK_CLIENT_NOT )                // mr -> (uniq)ms : 이중접속 등으로 라우터에서 유저 정보를 삭제했음을 알림.
_ENUM( EMR_REMOTE_QUEUEING_PACK_REQ )
_ENUM( EMR_REMOTE_QUEUEING_PACK_ACK )

//패킷의 끝
_ENUM( EVENT_EVENTID_SENTINEL ) //패킷의 끝

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif