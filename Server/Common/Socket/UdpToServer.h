#pragma once

#pragma pack( push, 1 )

struct KUdpToServer
{
	DWORD               m_dwUserUID;  ///< 게임 아이디(Login)
	unsigned short      m_usPort;       ///< 클라이언트가 개방한 포트
};

#pragma pack( pop )
