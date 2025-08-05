#include "Actor.h"
#include "IOCP.h"
#include "dbg/dbg.hpp"
#include "IOThread.h"
#include <boost/bind.hpp>
#include <boost/random.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton(KIocp);

KIocp::KIocp()
	:
	m_hIOCP(NULL),
	m_dwTerm(0L),
	m_dwSockObjNum(0L)

{
	m_cTime = CTime::GetCurrentTime();
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	LIF(m_hIOCP != NULL);
}

KIocp::~KIocp()
{
	if (m_hIOCP != NULL)
		CloseHandle(m_hIOCP);
}

bool KIocp::AssociateSocket(KSessionPtr spSessionPtr_)
{
	if (spSessionPtr_ == NULL) return false;

	KLocker lock(m_csSessionPtr);

	DWORD dwCompKey = GenNewCompKey();

	if (AssociateSocket(spSessionPtr_->GetSockObj()->m_sock, dwCompKey))
	{
		m_mapSessionPtr.insert(std::make_pair(dwCompKey, spSessionPtr_));
		spSessionPtr_->SetKey(dwCompKey);
		return true;
	}

	return false;
}

void KIocp::DeleteCompKey(IN DWORD dwKey_)
{
	KLocker lock(m_csSessionPtr);
	m_mapSessionPtr.erase(dwKey_);
	START_LOG(clog, L"Erase CompKey : " << dwKey_) << END_LOG;
}

DWORD KIocp::GenNewCompKey() const
{
	static boost::mt19937 rng;
	static boost::uniform_int<DWORD> uint32(1, UINT_MAX);
	static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

	KLocker lock(m_csSessionPtr);

	DWORD dwStart = GetTickCount();
	for (;;)
	{
		DWORD dwCompKey = die();

		if (m_mapSessionPtr.find(dwCompKey) == m_mapSessionPtr.end())
			return dwCompKey;
	}
	DWORD dwTerm = GetTickCount() - dwStart;

	if (dwTerm > m_dwTerm)
	{
		size_t sockObjNum;
		{
			KLocker lock(m_csSessionPtr);
			sockObjNum = m_mapSessionPtr.size();
		}

		KLocker lock(KIocp::m_cs);
		m_dwTerm = dwTerm;
		m_cTime = CTime::GetCurrentTime();
		m_dwSockObjNum = sockObjNum;
	}
}

bool KIocp::AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey)
{
	return (CreateIoCompletionPort((HANDLE)hSocket, m_hIOCP, CompKey, 0) != NULL);
}

bool KIocp::PostStatus(ULONG_PTR CompKey,
	DWORD dwNumBytes /* = 0 */,
	OVERLAPPED* po /* = NULL */)
{
	return (PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, CompKey, po) == TRUE);
}

bool KIocp::GetStatus(ULONG_PTR* pCompKey,
	PDWORD pdwNumBytes,
	OVERLAPPED** ppo,
	DWORD dwMilliseconds /* = INFINITE */)
{
	return (GetQueuedCompletionStatus(m_hIOCP,
		pdwNumBytes,
		pCompKey,
		ppo,
		dwMilliseconds) == TRUE);
}

void KIocp::EndThread()
{
	std::vector< std::shared_ptr<KThread> >::iterator vit;
	for (vit = m_vecThread.begin(); vit != m_vecThread.end(); vit++)
	{
		PostStatus(SIGNAL_SHUTDOWN_SERVER);
		//(*vit)->End();
	}
}

std::shared_ptr<KThread> KIocp::CreateThread()
{
	std::shared_ptr<KIOThread> spThread(new KIOThread);
	spThread->SetThreadManager(this);
	return spThread;
}

void KIocp::OnClosedByLocal(IN DWORD dwKey_)
{
	KSessionPtr spObj = GetSockObject(dwKey_);

	if (spObj)
	{
		spObj->GetSockObj()->OnSocketError();
	}
}

KSessionPtr KIocp::GetSockObject(DWORD dwKey_)
{
	KLocker lock(m_csSessionPtr);

	std::map<DWORD, KSessionPtr>::iterator mit;

	mit = m_mapSessionPtr.find(dwKey_);

	if (mit == m_mapSessionPtr.end())
	{
		START_LOG(clog, L"요청한 소켓의 포인터를 찾을 수 없음. Key : " << dwKey_)
			<< BUILD_LOG(m_mapSessionPtr.size())
			<< END_LOG;
		return KSessionPtr();
	}

	return mit->second;
}

void KIocp::OnIOCompleted(IN DWORD dwKey_, IN DWORD dwBytesTransfer_, IN OVERLAPPED* povl_)
{
	if (povl_ == NULL)
	{
		START_LOG(cerr, L"IOCP Error") << END_LOG;
		return;
	}

	KOVERLAPPED* pkOvl = static_cast<KOVERLAPPED*>(povl_);

	KSessionPtr spObj = GetSockObject(dwKey_);

	if (!spObj)
	{
		START_LOG(cerr, L"IO가 종료된 소켓의 포인터를 찾을 수 없음. Key : " << dwKey_)
			<< BUILD_LOG(dwBytesTransfer_)
			<< END_LOG;
		return;
	}

	switch (pkOvl->m_eIOMode) {
	case KOVERLAPPED::IO_RECV:
		if (&spObj->GetSockObj()->m_ovlRecv != pkOvl)
		{
			START_LOG(cerr, L"diffrent CompKey. (recv) key : " << dwKey_) << END_LOG;
		}
		spObj->GetSockObj()->OnRecvCompleted(dwBytesTransfer_);
		break;
	case KOVERLAPPED::IO_SEND:
		if (&spObj->GetSockObj()->m_ovlSend != pkOvl)
		{
			START_LOG(cerr, L"diffrent CompKey. (send) key : " << dwKey_) << END_LOG;
		}
		spObj->GetSockObj()->OnSendCompleted(dwBytesTransfer_);
		break;
	}
}

size_t KIocp::GetSockObjNum()
{
	KLocker lock(m_csSessionPtr);
	return m_mapSessionPtr.size();
}

void KIocp::DumpIOCPInfo(OUT std::wostream& stm)
{
	size_t sockObjNum = GetSockObjNum();

	KLocker lock(m_cs);
	stm << L" -- Max IOCP INFO --" << std::endl
		<< L"  Current SockOBJ NUM : " << sockObjNum << std::endl
		<< L"  Key Gen Term        : " << KIocp::m_dwTerm << std::endl
		<< L"  Occured Time        : " << (const wchar_t*)KIocp::m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl
		<< L"  SockOBJ NUM         : " << KIocp::m_dwSockObjNum << std::endl;
}