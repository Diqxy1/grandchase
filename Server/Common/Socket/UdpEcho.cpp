#include <WinSock2.h>
#include "UdpEcho.h"
#include "NetCommon.h"  // CLOSE_SOCKET
#include <dbg/dbg.hpp>
#include "Socket/UdpToServer.h"
#include "../GameServer/User.h"
#include "../GameServer/GSSimLayer.h"

#define MAX_UDP_PACKET_SIZE 1024

KUdpEcho::KUdpEcho(void) : m_sock(INVALID_SOCKET)
{
}

KUdpEcho::~KUdpEcho(void)
{
}

bool KUdpEcho::Init(USHORT usPort_)
{
	// 윈속 초기화는 이미 진행 되었다고 가정하고 생략한다.

	m_sock = ::socket(AF_INET, SOCK_DGRAM, 0);    // 소켓 생성

	if (INVALID_SOCKET == m_sock)
	{
		START_LOG(cerr, NetCommon::GetWsaMsg()) << END_LOG;
		::WSACleanup();
		return false;
	}

	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(usPort_);

	// usPort_를 UDP 포트로 개방한다.
	if (::bind(m_sock, (SOCKADDR *)&local, sizeof(local)) == SOCKET_ERROR)
	{
		START_LOG(cerr, NetCommon::GetWsaMsg()) << END_LOG;
		::WSACleanup();
		return false;
	}

	return true;
}

void KUdpEcho::Run()
{
	START_LOG(clog, L"Enter Run()") << END_LOG;

	DWORD ret;

	while (true)
	{
		ret = ::WaitForSingleObject(m_hKillEvent, 0);     // sleep 타임을 주면 안된다.

		if (ret == WAIT_OBJECT_0) break;

		if (ret == WAIT_TIMEOUT) Loop();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;
	}// while
}

void KUdpEcho::Loop()
{
	int ret;
	char recvbuff[MAX_UDP_PACKET_SIZE] = { 0 };
	SOCKADDR_IN sender;
	int nSenderSize = sizeof(sender);

	ret = ::recvfrom(m_sock,
		recvbuff,
		MAX_UDP_PACKET_SIZE,
		0,
		(SOCKADDR*)&sender,
		&nSenderSize);

	if (ret == SOCKET_ERROR)
	{
		START_LOG(cerr, NetCommon::GetWsaMsg()) << END_LOG;
		return;
	}

	if (ret != sizeof(KUdpToServer)) // 항상 KUdpToServer 구조체만을 받도록 가정한다.
	{
		START_LOG(cerr, L"전달받은 데이터 크기 이상. wanted : " << sizeof(KUdpToServer))
			<< BUILD_LOG(ret)
			<< BUILD_LOG(inet_ntoa(sender.sin_addr))
			<< BUILD_LOG(ntohs(sender.sin_port))
			<< END_LOG;
		return;
	}

	KUdpToServer kData;
	::memmove(&kData, recvbuff, sizeof(kData));

	START_LOG(clog, L"UID : " << kData.m_dwUserUID
		<< L", sock addr : " << inet_ntoa(sender.sin_addr) << L":" << ntohs(sender.sin_port))
		<< BUILD_LOG(kData.m_usPort) << END_LOG;

	KUserPtr spUser(SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>(kData.m_dwUserUID));
	if (spUser)
	{
		spUser->SetUdpPort(sender, kData);
	}
	else
	{
		START_LOG(cwarn, L"유저 검색 불가. UID : " << kData.m_dwUserUID)
			<< BUILD_LOG(inet_ntoa(sender.sin_addr))
			<< BUILD_LOG(ntohs(sender.sin_port))
			<< END_LOG;
	}
}

void KUdpEcho::ShutDown()
{
	CLOSE_SOCKET(m_sock);     // blocked mode. 소켓을 먼저 닫아 스레드가 스스로 종료되도록 유도한다. (깔끔하지 못함.)

	End();

	::WSACleanup();
}

#undef MAX_UDP_PACKET_SIZE // 1024