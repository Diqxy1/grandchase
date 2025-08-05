#pragma once
#include "SubThread.h" // thread, Event Handle
#include "IntEvent.h"
#include "UserEvent.h"
#include "GenericLogfile.h"

class KGSSimLayer;
class KWorker : public KSubThread< KUserEvent >
{
    typedef KGSSimLayer SLayerType;
    DeclToStringW;

public:
    KWorker(void);
    ~KWorker(void);

protected:
    KGenericLogfile         m_logFile; // 제네릭 로그 객체

protected:
    // derived from KSubThread
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );

protected:
   _INT_DECL_ON_FUNC( EVENT_CHAT_LOG_NOT, KChatLog );
    INT_DECL_ON_FUNC_NOPARAM( EVENT_CHANGE_CHAT_LOG_NOT );
    //INT_DECL_ON_FUNC_NOPARAM( EVENT_STAT_AGENT );
   _INT_DECL_ON_FUNC( EVENT_LONG_PROCESS_LOG, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_TICK_MANAGER_LOG, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_DB_QUEUE_SIZE_LOG, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_TRAFFIC_ATTACK, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_CLIENT_ERR, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_CLIENT_ERR_COMMENT, std::wstring );
   //_INT_DECL_ON_FUNC( EVENT_LT_29_ERR_COMMENT, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_DUNGEON_HACK, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_SEND_BUFF_FULL, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_PACKET_HACK, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_LOW_PING, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_EXP_CALC_ERR, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_DEATH_KILL_HACK, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_P2P, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_WIM_WIN, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_ABUSE, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_RECV_PACKET, std::vector< std::wstring > );
   _INT_DECL_ON_FUNC( EVENT_TICK_MANAGER_QUEUE_SIZE, std::wstring );
    INT_DECL_ON_FUNC( EVENT_LT_PACKET_COUNT );
   _INT_DECL_ON_FUNC( EVENT_GP_STAT_LOG, KGpLog );
   _INT_DECL_ON_FUNC( EVENT_ALERT_EXCEED_DB_QUEUE, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_USER_EVENT_PROCESS_TIME, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_PVP_HACK, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_ST_MEMBER_NOT_ING_P2P, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_USER_START_FAIL_STATE, std::wstring );
   //_INT_DECL_ON_FUNC( EVENT_NEW_DELETE_USERCOUNT, std::wstring );
   _INT_DECL_ON_FUNC( EVENT_LT_USER_INVENTORY_ITEM_LIST, std::wstring );

};
