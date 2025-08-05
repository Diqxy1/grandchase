#include "NetLayer.h"
#include "Socket/IOCP.h"
#include "Socket/IOThread.h"
#include "Socket/Session.h"

#include <dbg/dbg.hpp>
#include <KncSecurity/KncSecurity.h>
#include <boost/bind.hpp>

NiImplementRootRTTI(KNetLayer);

	
KNetLayer::KNetLayer(void)
{
	m_usPortMaster = 0;
	m_usPortSlave = 0;
	m_bCheckSequenceNum = false;
	m_uiPacketAuthLimitNum = 0;
	m_bHackCheckEnable = false;
	m_nPacketSendCount = 200;
	m_nLogPacketSendCount = 100;
	m_dwSendCountTickGap = 1000;

}

KNetLayer::~KNetLayer(void)
{
}

ImplToStringW(KNetLayer)
{
	START_TOSTRINGW
		<< TOSTRINGW(m_usPortMaster)
		<< TOSTRINGW(m_usPortSlave)
		<< TOSTRINGW(SiKIocp()->GetThreadNum());

	KIOThread::DumpExceptionInfo(stm_);

	SiKIocp()->DumpIOCPInfo(stm_);

	stm_ << std::endl;

	KSocketObject::DumpSendQueueLog(stm_);
	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
	KSession::DumpPacketAuthInfo(stm_);

	std::for_each(m_vecAccepter.begin(), m_vecAccepter.end(),
		boost::bind(&IAccepter::DumpMaxTimeInfo, _1, boost::ref(stm_)));

	return stm_;
}

bool KNetLayer::Init()
{
	// 050502. florist. 보안 코드 초기화 설정
	KncSecurity::InitSecurity();

	// init iocp
	SiKIocp()->Init(4);

	SiKIocp()->BeginThread();

	// 091113 tgkwon. SequenceNum 체크 추가.( 중복 패킷 방지 )
	// 정적접근을 통해서 선언을 해두고 추후에 NetLayer에서 Accept 함수 추가하는 걸로 수정필요.
	KSession::InitCheckSequenceNumInfo(GetCheckSequenceNum(), GetPacketAuthLimitNum()); // IOCP에 등록하기 전에 설정.
	KSession::InitPacketHackCheck(GetPacketHackCheckEnable(), GetPacketHackSendCount(), GetLogPacketHackSendCount(), GetPacketHackTickGap()); // IOCP에 등록하기 전에 설정.

	std::for_each(m_vecAccepter.begin(), m_vecAccepter.end(),
		boost::bind(&IAccepter::Begin, _1));

	return true;
}

void KNetLayer::ShutDown()
{
	std::for_each(m_vecAccepter.begin(), m_vecAccepter.end(),
		boost::bind(&IAccepter::End, _1, 3000));

	SiKIocp()->EndThread();
	Sleep(100);
	KIocp::ReleaseInstance();

	KncSecurity::ReleaseSecurity();
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KNetLayer::RegToLua(std::shared_ptr<lua_State> L)
{
	luabind::module(L.get())
		[
			luabind::class_<KNetLayer>("KNetLayer")
			.def_readwrite("m_usPortMaster", &KNetLayer::m_usPortMaster)
		.def_readwrite("m_usPortSlave", &KNetLayer::m_usPortSlave)
		.def("ResetMaxData", &KNetLayer::ResetMaxData)
		.def("SetCheckSequenceNum", &KNetLayer::SetCheckSequenceNum)
		.def("SetPacketLimitNum", &KNetLayer::SetPacketLimitNum)
		.def("SetUserPacketAttackInfo", &KNetLayer::SetUserPacketAttackInfo)
		.def("UpdateUserPacketAttackInfo", &KNetLayer::UpdateUserPacketAttackInfo)
		];
}

void KNetLayer::ResetMaxData()
{
	std::wostringstream strstm;

	std::for_each(m_vecAccepter.begin(), m_vecAccepter.end(),
		boost::bind(&IAccepter::DumpMaxTimeInfo, _1, boost::ref(strstm)));

	START_LOG(cerr, L"" << strstm.str()) << END_LOG;

	std::for_each(m_vecAccepter.begin(), m_vecAccepter.end(),
		boost::bind(&IAccepter::ResetMaxTimeInfo, _1));
}

void KNetLayer::SetUserPacketAttackInfo(IN bool bHackCheckEnable_, IN int nPacketSendCount_, IN int nLogPacketSendCount_, IN DWORD dwSendCountTickGap_)
{
	KLocker lock(m_csUserPacketAttack);
	m_bHackCheckEnable = bHackCheckEnable_;
	m_nPacketSendCount = nPacketSendCount_;
	m_nLogPacketSendCount = nLogPacketSendCount_;
	m_dwSendCountTickGap = dwSendCountTickGap_;
}

void KNetLayer::UpdateUserPacketAttackInfo()
{
	KSession::InitPacketHackCheck(GetPacketHackCheckEnable(), GetPacketHackSendCount(), GetLogPacketHackSendCount(), GetPacketHackTickGap());
}