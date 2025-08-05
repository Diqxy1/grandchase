#pragma once

#include "CommonPacket.h"

#pragma pack( push, 1 )

DECL_PACKET(DB_ETR_TYPE_COUNT_STAT)
{
	enum {
		STAT_CURRENT_USER = 1,
		STAT_RECV_COUNT = 7,
		STAT_RELAY_COUNT = 8,
		STAT_PACKET_COUNT_OVER = 9, // 분당 패킷 카운트 초과
		STAT_SEND_PACKET_FAIL = 10, // 패킷 전송 실패.
	};

	int                 m_nSID; // TCP Server ID
	std::map<int, int>   m_mapTypeStat; // [Type, Count]
};

#pragma pack( pop )