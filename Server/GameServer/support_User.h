#pragma once
#include "UserEventChecker.h"
#include "AgitHelper.h"
#include "AgentConnector.h"
#include "PostConfig.h"
#include "EventDivideDispatcher.h"
#include "EquipLvDown.h"

#define EVENT_TYPE KUserEvent

#define QUEUEING_EVENT_TO_DB( id, data )       SiKGSDBLayer()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, data )
#define QUEUEING_ID_TO_DB( id )                SiKGSDBLayer()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, m_cCurrentCharacter )
#define _QUEUEING_EVENT_TO_DB( id, data, ptr ) SiKGSDBLayer()->QueueingEvent( EVENT_TYPE::##id, ptr->m_strName.c_str(), ptr->m_dwUID, ptr->m_cCurrentCharacter, data )

#define QUEUEING_EVENT_TO_DB_CHECK( id, data ) \
do{ _EVENT_CHECK_IN(id); SiKGSDBLayer()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, m_cCurrentCharacter, data ); }while(0)
#define QUEUEING_ID_TO_DB_CHECK( id ) \
do{ _EVENT_CHECK_IN(id); SiKGSDBLayer()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, m_cCurrentCharacter ); }while(0)
#define _QUEUEING_EVENT_TO_DB_CHECK( id, data, ptr ) \
do{ _EVENT_CHECK_IN(id); SiKGSDBLayer()->QueueingEvent( EVENT_TYPE::##id, ptr->m_strName.c_str(), ptr->m_dwUID, ptr->m_cCurrentCharacter, data ); }

#define EVENT_CHECK_IN( id, where ) \
    do{ if(!m_kEventCheck.EventIn(id)) { \
            START_LOG( cwarn, L"이미 작업중인 EventID입니다. " << id << L", " << m_strName ) << END_LOG; \
            SET_ERR_GOTO( ERR_NOT_FINISHED_YET, where ); \
        } \
    }while(0)

#define _EVENT_CHECK_IN( id )               EVENT_CHECK_IN( EVENT_TYPE::##id, END_PROC )
#define EVENT_CHECK_OUT( id )               _EVENT_CHECK_OUT( id, SiKGSSimLayer()->GetCheckInEnable() == true )
#define _EVENT_CHECK_OUT( id, IsEnable )    KEventCheck CheckOn( EVENT_TYPE::##id, dwUID_, IsEnable )

#define CHECK_AGIT_ENABLE           __JIF( SiKGSSimLayer()->IsAgitEnable(), return )
#define CHECK_POST_ENABLE           __JIF( SiKPostConfig()->IsPostEnable(), return )
#define CHECK_EQUIP_LV_DOWN_ENABLE  __JIF( SiKEquipLvDown()->IsEquipLvDownEnable(), return )

// doubt. 코드 정리할때 매크로로 바꾸자.
// 에이전트 서버로 패킷 보낼 경우.
#define SEND_DATA_TO_AGENT_MASTER( id, data )       SiKAgentConnector()->SendPacket( SiKAgentConnector()->GetAgentMasterSID(), KAgentEvent::##id, data );
#define SEND_DATA_TO_AGENT( sid, id, data )         SiKAgentConnector()->SendPacket( sid, KAgentEvent::##id, data );
#define SEND_PACKET_TO_AGENT( sid, id )             SiKAgentConnector()->SendPacket( sid, KAgentEvent::##id, kPacket );
#define SEND_RECEIVED_PACKET_TO_AGENT( sid, id )    SiKAgentConnector()->SendPacket( sid, KAgentEvent::##id, kPacket_ );

// 특정 아지트에 패킷 보낼 경우.
#define SEND_DATA_TO_AGIT( uid, id, data )          SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( uid ), KAgentEvent::##id, data );
#define SEND_PACKET_TO_AGIT( uid, id )              SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( uid ), KAgentEvent::##id, kPacket );
#define SEND_RECEIVED_PACKET_TO_AGIT( uid, id )     SiKAgentConnector()->SendPacket( SiKAgitHelper()->GetAgitSID( uid ), KAgentEvent::##id, kPacket_ );

#define QUEUEING_EVENT_TO_DB_USING_DISPATCHER( id, data )                   SiKEventDivideDispatcher()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, data )
#define QUEUEING_EVENT_TO_DB_USING_DISPATCHER_CHECK_IN( id, data ) \
do{ _EVENT_CHECK_IN(id); SiKEventDivideDispatcher()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, data ); }while(0)

#define SEND_DATA_TO_MATCHSERVER( sid, id, data )         SiKMatchConnector()->SendPacket( sid, KMatchEvent::##id, data );
#define SEND_PACKET_TO_MATCHSERVER( sid, id )             SiKMatchConnector()->SendPacket( sid, KMatchEvent::##id, kPacket );
#define SEND_RECEIVED_PACKET_TO_MATCHSERVER( sid, id )    SiKMatchConnector()->SendPacket( sid, KMatchEvent::##id, kPacket_ );