#define EVENT_TYPE  KTREvent

#define QUEUEING_EVENT_TO_DB( id, data )    SiKTRDBLayer()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, data )
#define QUEUEING_ID_TO_DB( id )             SiKTRDBLayer()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID )

//Packet Lock