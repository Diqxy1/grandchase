#pragma once

#include "KncBitStream.h"
#include "Serializer/SerPair.h"
#include "Serializer/SerTuple.h"
#include "Serializer/SerList.h"
#include "Serializer/SerVector.h"
#include "Serializer/SerDeque.h"
#include "Serializer/SerMap.h"
#include "Serializer/SerSet.h"
#include "Serializer/SerBuffer.h"

#include <boost/utility.hpp>    // nocopyable
#include <windows.h>            // HANDLE
#include <sstream>
#include "PrimitiveTypedef.h"

// under-score(_)로 시작하는 매크로로 정의된 구조체는 shallow copy를 허용한다.
// primitive로만 구성된 구조체가 assisn operator를 필요로 하는 경우에만 사용한다.
#define DECL_DATA(name)    struct name; \
    SERIALIZE_DEFINE_TAG(name, eTAG_USERCLASS); \
    SERIALIZE_DECLARE_PUTGET(name); \
struct name
#define DECL_PACKET(id)    DECL_DATA( K##id )

#define COPYCON_ASSIGNOP( type, t )     type(const type& t) { *this = t; } \
    type& operator=(const type& t)

#define CON_COPYCON_ASSIGNOP( type, t ) type() {} \
    COPYCON_ASSIGNOP( type, t )

#define PUT( x )            ks.Put( obj.x )
#define GET( x )            ks.Get( obj.x )
#define PUTARRAY( x, size ) ks.PutArray( obj.x, size )
#define GETARRAY( x, size ) ks.GetArray( obj.x, dwSize = size ) && (dwSize == size)

#pragma pack( push, 1 )

DECL_DATA(CYOUPacketHeader) // 실제 패킷에 보낼 데이터.
{
	USHORT m_usHeader;
	USHORT m_usPacketLength;
	DWORD m_dwPacketType;
	USHORT m_usPacketSquence;

	CYOUPacketHeader() :
		m_usHeader(0),
		m_usPacketLength(0),
		m_dwPacketType(0),
		m_usPacketSquence(0)
	{}
};

#define GAMESERVER_IP 16
DECL_DATA(CYOUPacketCommonBody) // 실제 패킷 데이터.
{
	UCHAR m_ucGameserverIPLength;
	char m_chServerIP[GAMESERVER_IP];

	CYOUPacketCommonBody() :
		m_ucGameserverIPLength(0)
	{
		for (int i = 0; i < GAMESERVER_IP; ++i) {
			m_chServerIP[i] = 0;
		}
	}
};

DECL_DATA(KCYOUServerInfo) // 서버에서 빌링 스레드로 큐잉할 때 전달할 데이터.
{
	USHORT m_usPacketSequence;
	USHORT m_usZoneID; // a.k.a ZoneID
	USHORT m_usGroupID;
	int m_nCCU;
	std::string m_strIP;

	KCYOUServerInfo() :
		m_usPacketSequence(0),
		m_usZoneID(0),
		m_usGroupID(0),
		m_nCCU(0),
		m_strIP("")
	{}
};

DECL_DATA(KCYOUAuthUserInfo) // 서버에서 빌링 스레드로 큐잉할 때 전달할 데이터.
{
	USHORT m_usPacketSequence;
	std::string m_strLogin;
	std::string m_strPassword; // MD5 password
	std::string m_strIP;
	std::string m_strMACAddress;
	std::string m_strPC; // Machinery feature code
	std::string m_strServerIP;
	DWORD m_dwUID;
	char m_cLogoutType;
	char m_Character;
	USHORT m_usCharacterLevel;
	std::string m_strCharNickname;

	KCYOUAuthUserInfo() :
		m_usPacketSequence(0),
		m_strLogin(""),
		m_strPassword(""),
		m_strIP(""),
		m_strMACAddress(""),
		m_strPC(""),
		m_strServerIP(""),
		m_dwUID(0),
		m_cLogoutType(0),
		m_Character(-1),
		m_usCharacterLevel(0),
		m_strCharNickname("")
	{}
};

DECL_DATA(KCYOUCashData)
{
	USHORT m_usPacketSequence;
	std::string m_strServerIP;
	std::string m_strLogin;
	std::string m_strUserIP;
	std::string m_strSerialNumber; // 21 자리의 숫자. server group number(1 digit number) + 14 digit GetCurrentTime + 6 digit random code
	std::string m_strItemInfo; // Item ID(999 is fixed), GC Cash points, CYOU points 그리고 마지막에 ; 붙이기 , 구분자는 쉼표이다.
	DWORD m_dwUserUID;
	int m_nCashPoint;
	USHORT m_usZoneID; // a.k.a ZoneID
	USHORT m_usGroupID;
	USHORT m_usServerID;
	std::string m_strCharNickname;
	USHORT m_usCharacterLevel;

	KCYOUCashData() :
		m_usPacketSequence(0)
		, m_strServerIP("")
		, m_strLogin("")
		, m_strUserIP("")
		, m_strSerialNumber("")
		, m_strItemInfo("")
		, m_dwUserUID(0)
		, m_nCashPoint(0)
		, m_usZoneID(0)
		, m_usGroupID(0)
		, m_usServerID(0)
		, m_strCharNickname("")
		, m_usCharacterLevel(0)
	{
	}
};

DECL_DATA(KCYOUChangeCashLogData)
{
	int                 m_nOK; // 포인트 전환 성공여부
	std::string         m_strLogin; // 빌링계정
	DWORD               m_dwUserUID;
	std::string         m_strCharNickname;
	DWORD               m_dwCashPoint; // 현재 캐쉬값
	DWORD               m_dwCYOUPoint; // 창유 포인트
	USHORT              m_usPacketSequence; // 패킷 시퀀스번호
	std::string         m_strServerIP; // 빌링서버 IP
	std::string         m_strUserIP; // 유저 IP
	std::string         m_strSerialNumber;
	std::string         m_strItemInfo;
};

#pragma pack( pop )
