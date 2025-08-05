#pragma once
#include "PacketReq.h"
#include "PacketRes.h"

#include <windows.h>
#include <string>
#include <map>
#include "CYOUPacket.h"

#pragma pack( push, 1 )

#define HEADER_SIZE_OF_CYOUBILLING ( ( int )sizeof( CYOUPacketHeader ) )
#define COMMONBODY_SIZE_OF_CYOUBILLING ( ( int )sizeof(CYOUPacketCommonBody) )
#define MAX_PACKET_SIZE_OF_CYOUBILLING 1024

#pragma pack( pop )

class CYOUBillingEvent {
public:
#   undef   _ENUM
#   define  _ENUM( name, id ) name = id,
	enum ENUM_EVENT_ID {
#       include "CYOUBillingEvent_def.h"
	};

	static std::map< int, std::wstring > ms_mapEventIDString;

public:
	CYOUBillingEvent() {};
	~CYOUBillingEvent() {};

	static void Init();
	void Reset();
	std::wstring GetEventIDString() const;
	DWORD GetEventID() const;
	USHORT GetPacketSize() const;

	void SetPacketHeader(BYTE* abyteBuffer);
	void SetPacketHeader(OutputStream& oStream_);

	void CopyToBuffer(BYTE* abyteBuffer, unsigned short usOffset, unsigned short usSize);
	void temp();
	bool SetPacketLoginServerConnection(IN const KCYOUServerInfo& kServerInfo, OUT unsigned char* buf_, OUT int& nSize);
	void SetPacketTail(OutputStream& oStream_);
	void SetInputStream(char* pBuff);
	void ReadPacketHeader();
	BYTE* GetBuffer();
	bool SetPacketServerKeepAlive(IN const KCYOUServerInfo& kServerInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketAuthUserRequest(IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketLoginRequest(IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketLogoutRequest(IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketUserHeartbeat(IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketAgreement(IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketServerRestart(IN const KCYOUServerInfo& kServerInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketCharacterSwitch(IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize);
	bool SetPacketChangeCash(IN const KCYOUCashData& kCashInfo_, OUT unsigned char* buf_, OUT int& nSize); // from web point card -> cash in game.  by NPC.
	bool SetPacketRemainPoint(IN const KCYOUCashData& kCashInfo_, OUT unsigned char* buf_, OUT int& nSize); // get web point info
	bool SetPacketBuyLog(IN const KCYOUCashData& kCashInfo_, OUT unsigned char* buf_, OUT int& nSize);
	void DumpBuffer(unsigned char* buffer_, int nSize_) const;

private:
	enum ITEMID {
		REFILL_ITEMID = 999,
	};
	CYOUPacketHeader m_kHeader;
	BYTE m_abyteBuffer[MAX_PACKET_SIZE_OF_CYOUBILLING];
};
