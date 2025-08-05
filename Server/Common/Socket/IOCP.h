#pragma once
#include <windows.h>    // HANDLE, ULONG_PTR
#include <KNCSingleton.h>
#include "ThreadManager.h"
#include "KncSmartPtr.h"

SmartPointer(KSession); // Actor에 대한 전방 선언 & Smart Ptr

class KIocp : public KThreadManager
{
	DeclareSingleton(KIocp);

protected:
	KIocp();
public:
	enum {
		SIGNAL_SHUTDOWN_SERVER = 0
	};

	virtual ~KIocp();

	virtual void EndThread();  // derived from KThreadManager

	bool AssociateSocket(KSessionPtr pkSockObj);
	bool AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey);
	bool PostStatus(ULONG_PTR CompKey,
		DWORD dwNumBytes = 0,
		OVERLAPPED* po = NULL);

	bool GetStatus(OUT ULONG_PTR* pCompKey,
		OUT PDWORD pdwNumBytes,
		OUT OVERLAPPED** ppo,
		IN DWORD dwMilliseconds = INFINITE);

	void DeleteCompKey(IN DWORD dwKey);
	void OnIOCompleted(IN DWORD dwKey, IN DWORD dwBytesTransfer, IN OVERLAPPED* povl);
	void OnClosedByLocal(IN DWORD dwKey);
	void DumpIOCPInfo(OUT std::wostream& stm);
	size_t GetSockObjNum();

private:
	KSessionPtr GetSockObject(IN DWORD dwKey);
	virtual std::shared_ptr<KThread> CreateThread();  // derived from KThreadManager
	inline DWORD GenNewCompKey() const;

	HANDLE                          m_hIOCP;
	mutable KncCriticalSection      m_csSessionPtr;
	std::map<DWORD, KSessionPtr>     m_mapSessionPtr;  // 070828. kkurrung. 원래는 SocketObj 였는데 Actor로 교체 ( 이유는 인생이 Safe하기 위해서)

	mutable KncCriticalSection  m_cs;               // Critical Section
	mutable DWORD               m_dwTerm;           // 키 생성하는 가장 긴 시간
	mutable CTime               m_cTime;            // 가장 긴 키생성시간이 발생한 시간
	mutable DWORD               m_dwSockObjNum;

	friend class KIOThread;
};

DefSingletonInline(KIocp);