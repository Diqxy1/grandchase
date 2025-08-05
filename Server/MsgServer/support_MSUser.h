
#define EVENT_TYPE  KMSEvent
#define CLASS_TYPE  KMSUser

#define MAX_FRIEND_LIMIT        SiKMSSimLayer()->m_usMaxFriendLimit// 최대 친구수
#define MAX_BLOCK_LIMIT         SiKMSSimLayer()->m_usMaxBlockLimit // 최대 블럭수
#define MAX_GROUP_LIMIT         SiKMSSimLayer()->m_usMaxGroupLimit // 최대 그룹 생성수
#define MAX_GROUP_NAME_LIMIT    SiKMSSimLayer()->m_usMaxGroupName  // 그룹명 길이 제한
#define MAX_CHAT_ROOM_LIMIT     SiKMSSimLayer()->m_usMaxChatRoom   // 최대 대화방수
#define MAX_CHAT_MEMBER         SiKMSSimLayer()->m_usMaxChatMember // 최대 대화맴버수

#define REMOTE_QUEUEING( uuid, id, data )           SiKMRConnector()->QueueingPacket( m_dwUID, uuid, EVENT_TYPE::##id, data, false, false )
#define REMOTE_SEND( uuid, id, data )               SiKMRConnector()->QueueingPacket( m_dwUID, uuid, EVENT_TYPE::##id, data, false, true )
#define REMOTE_QUEUEING_ACK( uuid, id, data )       SiKMRConnector()->QueueingPacket( m_dwUID, uuid, EVENT_TYPE::##id, data, true, false )
#define REMOTE_SEND_ACK( uuid, id, data )           SiKMRConnector()->QueueingPacket( m_dwUID, uuid, EVENT_TYPE::##id, data, true, true )
#define REMOTE_QUEUEING_PACK( uuid, id, data )      SiKMRConnector()->QueueingPacketPack( m_dwUID, uuid, EVENT_TYPE::##id, data, false, false )
#define REMOTE_QUEUEING_PACK_ACK( uuid, id, data )  SiKMRConnector()->QueueingPacketPack( m_dwUID, uuid, EVENT_TYPE::##id, data, true, false )

#define QUEUEING_EVENT_TO_DB( id, data )    SiKMSDBLayer()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, -1, data )
#define QUEUEING_ID_TO_DB( id )             SiKMSDBLayer()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID )

//Packet Lock