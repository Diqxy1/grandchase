#define EVENT_TYPE  KAgentEvent

#define QUEUEING_EVENT_TO_DB( id, data )    SiKAGDBLayer()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, -1, data )
#define QUEUEING_ID_TO_DB( id )             SiKAGDBLayer()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID )

//Packet Lock