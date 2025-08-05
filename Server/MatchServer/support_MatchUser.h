#define EVENT_TYPE  KMatchEvent

#define QUEUEING_EVENT_TO_DB( id, data )    SiKMatchDBLayer()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, -1, data )
#define QUEUEING_ID_TO_DB( id )             SiKMatchDBLayer()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID )

//Packet Lock