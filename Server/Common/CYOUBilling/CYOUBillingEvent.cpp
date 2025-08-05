#include "CYOUBillingEvent.h"
#include <dbg/dbg.hpp>
#include "Log4.h"

/*std::map< int, std::wstring > CYOUBillingEvent::ms_mapEventIDString;

CYOUBillingEvent::CYOUBillingEvent(void)
{
	::ZeroMemory( m_abyteBuffer, MAX_PACKET_SIZE_OF_CYOUBILLING );
}

CYOUBillingEvent::~CYOUBillingEvent(void)
{
}

void CYOUBillingEvent::Init()
{
#undef _ENUM
#define _ENUM( name, id ) CYOUBillingEvent::ms_mapEventIDString[id] = L#name;
#include "CYOUBillingEvent_def.h"
}

void CYOUBillingEvent::Reset()
{
	//::ZeroMemory( &m_kHeader, HEADER_SIZE_OF_CYOUBILLING );
	::ZeroMemory( m_abyteBuffer, MAX_PACKET_SIZE_OF_CYOUBILLING );
}

std::wstring CYOUBillingEvent::GetEventIDString() const // usually for log
{
	std::wstring wstrRet;
	if ( ms_mapEventIDString.find( GetEventID() ) != ms_mapEventIDString.end() ) {
		wstrRet = ms_mapEventIDString[GetEventID()];
	}
	else {
		wstrRet = L"";
	}

	return wstrRet;
}

DWORD CYOUBillingEvent::GetEventID() const
{
	return m_kHeader.m_dwPacketType;
}

USHORT CYOUBillingEvent::GetPacketSize() const
{
	return m_kHeader.m_usPacketLength+6;
}

void CYOUBillingEvent::SetPacketHeader( BYTE* abyteBuffer_ )
{
	CopyToBuffer( abyteBuffer_, 0, HEADER_SIZE_OF_CYOUBILLING );
	// 헤더이기 때문에 오프셋 0
	ReadPacketHeader();
}

void CYOUBillingEvent::SetPacketHeader( OutputStream& oStream_ )
{
	oStream_.Write((unsigned char)0xaa); // 패킷의 시작.
	oStream_.Write((unsigned char)0x55);
}

void CYOUBillingEvent::SetPacketTail( OutputStream& oStream_ )
{
	oStream_.Write((unsigned char)0x55); // 패킷의 끝.
	oStream_.Write((unsigned char)0xaa); // 패킷의 끝.
}

void CYOUBillingEvent::temp()
{
	Packet *pPacket = NULL;
	LBBillEnd packet;
	pPacket = &packet; // 부모 포인터에 자식을 넣는 일반적인 사용법?
	packet.SetPacketIndex(65444); // 패킷의 시퀀스 번호, 누가 증가시키지?
	packet.SetServerIP("127.0.0.1");
	packet.SetCn("abcdefghij"); // character name 을 줄인 것으로 예상. 캐릭터명
	packet.SetRoleLevel(21); // 캐릭터 레벨
	packet.SetType(0); // 로그아웃 타입
	// packet 에 Set 함수들을 사용해서 패킷의 본문(바디)을 만든다.
	unsigned char buf[1024];
	memset(buf, 0, sizeof(buf)); // buf 을 0 으로 초기화
	unsigned char *pbuf = buf;
	OutputStream oStream = OutputStream((char*)pbuf);
	oStream.Write((unsigned char)0xaa);
	oStream.Write((unsigned char)0x55);
	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if(PCI_TRUE == pPacket->Write(oStream))
	{
		oStream.Write((unsigned char)0x55); // 패킷의 끝.
		oStream.Write((unsigned char)0xaa); // 패킷의 끝.
	}
}

// 3. Login Server Connection Protocol ( Protocol A0 ), 서버가 빌링서버에 로그인하는 프로토콜
bool CYOUBillingEvent::SetPacketLoginServerConnection( IN const KCYOUServerInfo& kServerInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBConnect packet;
	pPacket = &packet;

	packet.SetGroup( kServerInfo_.m_usGroupID );
	packet.SetZone( kServerInfo_.m_usZoneID );
	//packet.SetZone( 6 ); // 6 * 1000 + 8 , 분리해서 6과 8을 세팅.
	//packet.SetGroup( 8 ); // gc의 group id와 zone id 는 무엇인가? 6008 이다.

	packet.SetPacketIndex(kServerInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kServerInfo_.m_strIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf); // 패킷의 길이를 set 하는 부분이 이 안에 있다.
		nSize_ +=6;

		return true;
	}
	else {
		return false;
	}
};

bool CYOUBillingEvent::SetPacketServerKeepAlive( IN const KCYOUServerInfo& kServerInfo_, OUT unsigned char* buf_, OUT int& nSize_  )
{
	Packet* pPacket = NULL;
	LBKeepAlive packet;
	pPacket = &packet;
	packet.SetGroup( kServerInfo_.m_usGroupID ); // gc의 group id와 zone id 는 무엇인가?
	packet.SetZone( kServerInfo_.m_usZoneID );
	packet.SetPacketIndex(kServerInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kServerInfo_.m_strIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.
	packet.SetUsers( kServerInfo_.m_nCCU ); // 서버의 동접 정보 기록.

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf); // 패킷의 길이를 set 하는 부분이 이 안에 있다.
		nSize_ += 6;

		return true;
	}
	else {
		return false;
	}
};

bool CYOUBillingEvent::SetPacketAuthUserRequest( IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBAuthUser packet;
	pPacket = &packet;

	packet.SetPacketIndex(kUserInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kUserInfo_.m_strServerIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	// 계정 세팅
	char szEN[PCI_MAX_CN_LEN];
	strncpy_s( szEN, sizeof( szEN ), kUserInfo_.m_strLogin.c_str(), _TRUNCATE );
	packet.SetEN( szEN );
	// 계정 세팅 끝.
	// 패스워드 세팅
	PCI_CHAR szPassword[PCI_MAX_PASSWD_LEN];
	strncpy_s( szPassword, sizeof( szPassword ), kUserInfo_.m_strPassword.c_str(), _TRUNCATE );
	packet.SetPassword( szPassword );
	// 패스워드 세팅 끝.

	// 유저의 아이피 세팅.
	PCI_CHAR szUserIP[PCI_MAX_IP_LEN];
	strncpy_s( szUserIP, sizeof( szUserIP ), kUserInfo_.m_strIP.c_str(), _TRUNCATE );
	packet.SetUserIP( szUserIP );

	// 클라이언트 맥어드레스 MD5
	PCI_CHAR szMACMD5Value[PCI_MAX_PASSWD_LEN];
	strncpy_s( szMACMD5Value, sizeof( szMACMD5Value ), kUserInfo_.m_strMACAddress.c_str(), _TRUNCATE );
	packet.SetMACMD5Value( szMACMD5Value );

	// 클라이언트 머신 코드
	PCI_CHAR szPCValue[PCI_PC_VALUE_LEN];
	strncpy_s( szPCValue, sizeof( szPCValue ), kUserInfo_.m_strPC.c_str(), _TRUNCATE );
	packet.SetPCValue( szPCValue );

	// 로그인 타입
	PCI_UCHAR ucType = 0;
	packet.SetEType( ucType );

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf) + 6; // 패킷의 길이를 set 하는 부분이 이 안에 있다.

		return true;
	}
	else {
		return false;
	}
};

bool CYOUBillingEvent::SetPacketLoginRequest( IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBBillStart packet;
	pPacket = &packet;

	packet.SetPacketIndex(kUserInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kUserInfo_.m_strServerIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	// 계정 세팅
	char szCN[PCI_MAX_CN_LEN];
	strncpy_s( szCN, sizeof( szCN ), kUserInfo_.m_strLogin.c_str(), _TRUNCATE );
	packet.SetCn( szCN );
	// 계정 세팅 끝.

	packet.SetGuid( kUserInfo_.m_dwUID ); // User의 UID 설정.

	PCI_CHAR szRoleName[PCI_MAX_ROLE_NAME_LEN];
	strncpy_s( szRoleName, sizeof( szRoleName ), kUserInfo_.m_strCharNickname.c_str(), _TRUNCATE );
	packet.SetRoleName( szRoleName );
	//packet.SetRoleName( kUserInfo_.m_strCharNickname.c_str() );
	packet.SetRoleLevel( kUserInfo_.m_usCharacterLevel );

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf) + 6; // 패킷의 길이를 set 하는 부분이 이 안에 있다.

		return true;
	}
	else {
		return false;
	}
}

bool CYOUBillingEvent::SetPacketLogoutRequest( IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBBillEnd packet;
	pPacket = &packet;

	packet.SetPacketIndex(kUserInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kUserInfo_.m_strServerIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	// 계정 세팅
	char szCN[PCI_MAX_CN_LEN];
	strncpy_s( szCN, sizeof( szCN ), kUserInfo_.m_strLogin.c_str(), _TRUNCATE );
	packet.SetCn( szCN );
	// 계정 세팅 끝.

	packet.SetRoleLevel( kUserInfo_.m_usCharacterLevel );
	packet.SetType( kUserInfo_.m_cLogoutType );
	//packet.SetType( 0x01 );

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf); // 패킷의 길이를 set 하는 부분이 이 안에 있다.
		nSize_ += 6;

		return true;
	}
	else {
		return false;
	}
}

bool CYOUBillingEvent::SetPacketUserHeartbeat( IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBAutoBill packet;
	pPacket = &packet;

	packet.SetPacketIndex(kUserInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kUserInfo_.m_strServerIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	// 계정 세팅
	char szCN[PCI_MAX_CN_LEN];
	strncpy_s( szCN, sizeof( szCN ), kUserInfo_.m_strLogin.c_str(), _TRUNCATE );
	packet.SetUserName( szCN );
	// 계정 세팅 끝.

	packet.SetRoleLevel( kUserInfo_.m_usCharacterLevel );

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf); // 패킷의 길이를 set 하는 부분이 이 안에 있다.
		nSize_ += 6;

		return true;
	}
	else {
		return false;
	}
}

bool CYOUBillingEvent::SetPacketAgreement( IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBConfirmProtocol packet;
	pPacket = &packet;

	packet.SetPacketIndex(kUserInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kUserInfo_.m_strServerIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	PCI_CHAR szUserIP[PCI_MAX_IP_LEN];
	strncpy_s( szUserIP, sizeof( szUserIP ), kUserInfo_.m_strIP.c_str(), _TRUNCATE );
	packet.SetUserIP( szUserIP );

	// 계정 세팅
	char szCN[PCI_MAX_CN_LEN];
	strncpy_s( szCN, sizeof( szCN ), kUserInfo_.m_strLogin.c_str(), _TRUNCATE );
	packet.SetUserName( szCN );
	// 계정 세팅 끝.

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf) + 6; // 패킷의 길이를 set 하는 부분이 이 안에 있다.

		return true;
	}
	else {
		return false;
	}
}

bool CYOUBillingEvent::SetPacketServerRestart( IN const KCYOUServerInfo& kServerInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBServRestart packet;
	pPacket = &packet;

	packet.SetGroup( kServerInfo_.m_usGroupID );
	packet.SetZone( kServerInfo_.m_usZoneID );

	packet.SetPacketIndex(kServerInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kServerInfo_.m_strIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf); // 패킷의 길이를 set 하는 부분이 이 안에 있다.
		nSize_ +=6;

		return true;
	}
	else {
		return false;
	}
};

bool CYOUBillingEvent::SetPacketCharacterSwitch( IN const KCYOUAuthUserInfo& kUserInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBSwitchRole packet;
	pPacket = &packet;

	packet.SetPacketIndex(kUserInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kUserInfo_.m_strIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf) + 6; // 패킷의 길이를 set 하는 부분이 이 안에 있다.

		return true;
	}
	else {
		return false;
	}
}

bool CYOUBillingEvent::SetPacketChangeCash( IN const KCYOUCashData& kCashInfo_, OUT unsigned char* buf_, OUT int& nSize_ ) // from web point card -> cash in game.  by NPC.
{
	//2012.11.08. woosh
	//게임내 화폐 전환 프로토콜에서 ItemID 는 999 설정한다. (중국 빌링 담당자 회의)
	//ItemID, amount, price 구분자는 comma(,) 이다.

	Packet* pPacket = NULL;
	LBExchangeYB packet;
	pPacket = &packet;

	packet.SetPacketIndex(kCashInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kCashInfo_.m_strServerIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.
	// packet.SetGoods( REFILL_ITEMID );

	// 계정 세팅
	//char szCN[PCI_MAX_CN_LEN];
	//strncpy_s( szCN, sizeof( szCN ), kCashInfo_.m_strLogin.c_str(), _TRUNCATE );
	//packet.SetCN( szCN );

	packet.SetCN( kCashInfo_.m_strLogin.c_str() ); // 중국 라이브러리의 일관성 부족.
	// 계정 세팅 끝.

	packet.SetUserIP( kCashInfo_.m_strUserIP.c_str() );

	packet.SetRoleName( kCashInfo_.m_strCharNickname.c_str() );

	packet.SetSerialNo( kCashInfo_.m_strSerialNumber.c_str() );

	// SetGoods 에 들어가는 consumed points 의 한계는 9999 이다. 천룡팔부에서는 5000 이 최대이다.

	char szGoods[PCI_MAX_GOODS_LEN];
	strncpy_s( szGoods, sizeof( szGoods ), kCashInfo_.m_strItemInfo.c_str(), _TRUNCATE );
	packet.SetGoods( szGoods );

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf) + 6; // 패킷의 길이를 set 하는 부분이 이 안에 있다.

		return true;
	}
	else {
		return false;
	}
}

bool CYOUBillingEvent::SetPacketRemainPoint( IN const KCYOUCashData& kCashInfo_, OUT unsigned char* buf_, OUT int& nSize_ ) // get web point info
{
	Packet* pPacket = NULL;
	LBCnPoint packet;
	pPacket = &packet;

	packet.SetPacketIndex(kCashInfo_.m_usPacketSequence); // 패킷의 시퀀스 번호, 누가 증가시키지? 게임서버가 증가시킨다. static 변수를 써야 할 것 같다.
	packet.SetServerIP( kCashInfo_.m_strServerIP.c_str() ); // 서버 아이피, 파라미터로 받게 수정해야 함.

	packet.SetCN( kCashInfo_.m_strLogin.c_str() ); // 중국 라이브러리의 일관성 부족.

	packet.SetUserIP( kCashInfo_.m_strUserIP.c_str() );

	packet.SetRoleName( kCashInfo_.m_strCharNickname.c_str() );

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf) + 6; // 패킷의 길이를 set 하는 부분이 이 안에 있다.

		return true;
	}
	else {
		return false;
	}
}

bool CYOUBillingEvent::SetPacketBuyLog(  IN const KCYOUCashData& kCashInfo_, OUT unsigned char* buf_, OUT int& nSize_ )
{
	Packet* pPacket = NULL;
	LBServBuyLog packet;
	pPacket = &packet;

	packet.SetPacketIndex(kCashInfo_.m_usPacketSequence);
	packet.SetServerIP( kCashInfo_.m_strServerIP.c_str() ); // 서버 아이피

	packet.SetGroup( kCashInfo_.m_usGroupID );
	packet.SetZone( kCashInfo_.m_usZoneID );

	packet.SetSceneID( 0 );

	char szCN[PCI_MAX_CN_LEN];
	strncpy_s( szCN, sizeof( szCN ), kCashInfo_.m_strLogin.c_str(), _TRUNCATE );
	packet.SetCN( szCN );
	// 계정 세팅 끝.

	// 유저의 아이피 세팅.
	PCI_CHAR szUserIP[PCI_MAX_IP_LEN];
	strncpy_s( szUserIP, sizeof( szUserIP ), kCashInfo_.m_strUserIP.c_str(), _TRUNCATE );
	packet.SetUserIP( szUserIP );

	PCI_CHAR szRoleName[PCI_MAX_ROLE_NAME_LEN];
	strncpy_s( szRoleName, sizeof( szRoleName ), kCashInfo_.m_strCharNickname.c_str(), _TRUNCATE );
	packet.SetRoleName( szRoleName );

	packet.SetUserLevel( kCashInfo_.m_usCharacterLevel );

	PCI_CHAR szSerialNum[PCI_MAX_SERIAL_NO_LEN];
	strncpy_s( szSerialNum, sizeof( szSerialNum ), kCashInfo_.m_strSerialNumber.c_str(), _TRUNCATE );
	packet.SetSN( szSerialNum );

	packet.SetGUID( kCashInfo_.m_dwUserUID );
	time_t seconds;
	time(&seconds);
	UINT nTime = seconds;
	packet.SetTransTime( nTime );
	packet.SetJewelNum( kCashInfo_.m_nCashPoint );

	PCI_CHAR szRemark[PCI_MAX_REMARK_LEN];
	strncpy_s( szRemark, sizeof( szRemark ), kCashInfo_.m_strItemInfo.c_str(), _TRUNCATE );
	packet.SetRemark( szRemark );

	unsigned char *pbuf = buf_;
	OutputStream oStream = OutputStream((char*)pbuf);

	SetPacketHeader( oStream ); // 패킷 헤더 조립 함수를 따로 만듦.

	unsigned short pksize = 0; // 패킷의 길이 ( 헤더, 테일은 제외 ), ushort 만큼 자리만 잡아놓네 (실제 구하는 곳은 새로 만들어야 되나?)
	oStream.Write(pksize);
	unsigned int pkid = pPacket->GetPacketID(); // 패킷의 타입, 패킷 아이디 , 4바이트
	oStream.Write(pkid);
	unsigned short pkidx = pPacket->GetPacketIndex(); // 패킷의 시퀀스 번호, req에서 1 이면 ack 도 1 로 온다.
	oStream.Write(pkidx);
	char svrip[16];
	memset(svrip, 0, sizeof(svrip));
	strncpy(svrip, pPacket->GetServerIP(), sizeof(svrip)-1);
	unsigned char svrip_len = strlen(svrip);
	oStream.Write((unsigned char)svrip_len); // 서버 아이피의 길이
	oStream.Write(svrip, svrip_len); // 서버 아이피

	if (PCI_TRUE == pPacket->Write(oStream)) {
		SetPacketTail( oStream );
		nSize_ = oStream.WritePacketSize(pbuf) + 6; // 패킷의 길이를 set 하는 부분이 이 안에 있다.

		return true;
	}
	else {
		return false;
	}
}

BYTE* CYOUBillingEvent::GetBuffer()
{
	return m_abyteBuffer+HEADER_SIZE_OF_CYOUBILLING;
}

void CYOUBillingEvent::CopyToBuffer( BYTE* abyteBuffer_, unsigned short usOffset_, unsigned short usSize_ )
{
	if( usOffset_ + usSize_ > MAX_PACKET_SIZE_OF_CYOUBILLING )
	{
		START_LOG( cerr, L"카피 범위 초과." )
			<< BUILD_LOG( usOffset_ )
			<< BUILD_LOG( usSize_ )
			<< BUILD_LOG( MAX_PACKET_SIZE_OF_CYOUBILLING )
			<< END_LOG;

		return;
	}

	::memcpy( m_abyteBuffer + usOffset_, abyteBuffer_, ( size_t )usSize_ );
}

void CYOUBillingEvent::SetInputStream( char* pBuff )
{
	InputStream iStream = InputStream( pBuff );
}

void CYOUBillingEvent::ReadPacketHeader()
{
	::memcpy( &m_kHeader, m_abyteBuffer, HEADER_SIZE_OF_CYOUBILLING );
	m_kHeader.m_usHeader = ::ntohs( m_kHeader.m_usHeader );
	m_kHeader.m_usPacketLength = ::ntohs( m_kHeader.m_usPacketLength );
	m_kHeader.m_dwPacketType = ::ntohl( m_kHeader.m_dwPacketType );
	m_kHeader.m_usPacketSquence = ::ntohs( m_kHeader.m_usPacketSquence );
}

void CYOUBillingEvent::DumpBuffer( unsigned char* buffer_, int nSize_ ) const
{
	char szNBuffer[1024 * 3 + 1]; // ?
	char szByte[4]; // ?

	for( int i = 0; i < nSize_; i++ )
	{
		if( (int)buffer_[i] >= 16 ) // 16보다 크면
		{
			::itoa( ( int )buffer_[i], szByte, 16 ); // 숫자를 문자로 변환, 16진수
			szNBuffer[i * 3] = szByte[0];
			szNBuffer[i * 3 + 1] = szByte[1];
			szNBuffer[i * 3 + 2] = ' ';
		}
		else
		{
			::itoa( ( int )buffer_[i], szByte, 16 );
			szNBuffer[i * 3] = '0';
			szNBuffer[i * 3 + 1] = szByte[0];
			szNBuffer[i * 3 + 2] = ' ';
		}

		if( i % 8 == 7 )
		{
			szNBuffer[i * 3 + 2] = '\n'; // 8개 출력 후 다음줄
		}
	}

	std::string strNBuffer = szNBuffer;

	START_LOG( clog, L"BUFFER : ")
		<< BUILD_LOG( KncUtil::toWideString( strNBuffer ) )
		//<< BUILD_LOG( KncUtil::toWideString( strTmp ) )
		<< BUILD_LOG( sizeof(buffer_) ) << END_LOG;
}
*/