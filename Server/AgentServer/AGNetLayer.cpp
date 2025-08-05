#include "Socket/Session.h"
#include "AGNetLayer.h"
#include "AGSimLayer.h"
#include <sstream>
#include "Agent.h"
#include "AGSlave.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton(KAGNetLayer);
NiImplementRTTI(KAGNetLayer, KNetLayer);

KAGNetLayer::KAGNetLayer(void)
{
}

KAGNetLayer::~KAGNetLayer(void)
{
}

ImplToStringW(KAGNetLayer)
{
	START_TOSTRING_PARENTW(KNetLayer) << std::endl;

	stm_ << L" -- Relay Server Address (size:" << m_vecTRServerAddr.size() << L") -- " << std::endl;

	int nIndex = 0;
	std::vector<SOCKADDR_IN>::const_iterator vit;
	for (vit = m_vecTRServerAddr.begin(); vit != m_vecTRServerAddr.end(); ++vit, ++nIndex)
	{
		stm_ << L"  Index:" << nIndex
			<< L", addr:" << inet_ntoa(vit->sin_addr) << L":" << ntohs(vit->sin_port) << std::endl;
	}
	return stm_;
}

ImplOstmOperatorW2A(KAGNetLayer);

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

bool KAGNetLayer::Init()
{
	m_vecAccepter.push_back(IAccepterPtr(new KAccepter<KAgent>(m_usPortMaster)));
	m_vecAccepter.push_back(IAccepterPtr(new KAccepter<KAGSlave>(m_usPortSlave)));

	_JIF(KNetLayer::Init(), false);

	return true;
}

void KAGNetLayer::ResetMaxData()
{
	std::wostringstream strstm;

	KSession::DumpMaxSendData(strstm);
	KNetLayer::ResetMaxData();
	std::wcout << strstm.str();
	KSession::ResetMaxSendData();
}

void KAGNetLayer::RegToLua(std::shared_ptr<lua_State> L)
{
	KNetLayer::RegToLua(L);

	luabind::module(L.get())
		[
			luabind::def("GetNetLayer", &KAGNetLayer::GetInstance),

			luabind::class_<KAGNetLayer, KNetLayer>("KAGNetLayer")
			.def(luabind::tostring(luabind::self))
		.def("AddTRAddress", &KAGNetLayer::AddTRServerAddress)
		.def("DeleteTRAddress", &KAGNetLayer::DeleteTRServerAddress)
		.def("AddURAddress", &KAGNetLayer::AddURServerAddress)
		.def("DeleteURAddress", &KAGNetLayer::DeleteURServerAddress)
		.def("ResetMaxData", &KAGNetLayer::ResetMaxData)
		];
}

void KAGNetLayer::ShutDown()
{
	KNetLayer::ShutDown();
}

void KAGNetLayer::AddTRServerAddress(const char* szIP_, USHORT usPort_)
{
	SOCKADDR_IN addrTRServer;
	addrTRServer.sin_addr.s_addr = inet_addr(szIP_);
	addrTRServer.sin_port = htons(usPort_);

	{
		KLocker lock(m_csRelayServer);
		m_vecTRServerAddr.push_back(addrTRServer);
	}
}

bool KAGNetLayer::GetTRServerAddress(IN int nKey_, OUT DWORD& dwIP_, OUT USHORT& usPort_)
{
	KLocker lock(m_csRelayServer);
	if (m_vecTRServerAddr.empty()) {
		START_LOG(cerr, L"릴레이 서버의 주소가 설정되어있지 않음.") << END_LOG;
		return false;
	}

	int nIndex = nKey_ % m_vecTRServerAddr.size();

	dwIP_ = m_vecTRServerAddr[nIndex].sin_addr.S_un.S_addr;
	usPort_ = ntohs(m_vecTRServerAddr[nIndex].sin_port);

	return true;
}

bool KAGNetLayer::DeleteTRServerAddress(IN unsigned int nIndex_)
{
	KLocker lock(m_csRelayServer);

	if (nIndex_ >= m_vecTRServerAddr.size()) {
		START_LOG(cwarn, L"올바르지 못한 인덱스. index:" << nIndex_)
			<< BUILD_LOG(m_vecTRServerAddr.size()) << END_LOG;
		return false;
	}

	m_vecTRServerAddr.erase(m_vecTRServerAddr.begin() + nIndex_);

	std::wcout << nIndex_ << L"번 릴레이 서버 삭제됨." << std::endl;
	return true;
}

void KAGNetLayer::AddURServerAddress(const char* szIP_, USHORT usPort_)
{
	SOCKADDR_IN addrURServer;
	addrURServer.sin_addr.s_addr = inet_addr(szIP_);
	addrURServer.sin_port = htons(usPort_);

	{
		KLocker lock(m_csRelayServer);
		m_vecURServerAddr.push_back(addrURServer);
	}
}

bool KAGNetLayer::GetURServerAddress(IN int nKey_, OUT DWORD& dwIP_, OUT USHORT& usPort_)
{
	KLocker lock(m_csRelayServer);
	if (m_vecURServerAddr.empty()) {
		START_LOG(cerr, L"릴레이 서버의 주소가 설정되어있지 않음.") << END_LOG;
		return false;
	}

	int nIndex = nKey_ % m_vecURServerAddr.size();

	dwIP_ = m_vecURServerAddr[nIndex].sin_addr.S_un.S_addr;
	usPort_ = ntohs(m_vecURServerAddr[nIndex].sin_port);

	return true;
}

bool KAGNetLayer::DeleteURServerAddress(IN unsigned int nIndex_)
{
	KLocker lock(m_csRelayServer);

	if (nIndex_ >= m_vecURServerAddr.size()) {
		START_LOG(cwarn, L"올바르지 못한 인덱스. index:" << nIndex_)
			<< BUILD_LOG(m_vecURServerAddr.size()) << END_LOG;
		return false;
	}

	m_vecURServerAddr.erase(m_vecURServerAddr.begin() + nIndex_);

	std::wcout << nIndex_ << L"번 릴레이 서버 삭제됨." << std::endl;
	return true;
}