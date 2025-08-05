#pragma once
#include "stdafx.h"
#include "FileDownloadManager.h"

#ifndef _BEGINTHREADEX 
typedef unsigned (__stdcall* PTHREAD_START)(void*);
#   define _BEGINTHREADEX( psa, \
	cbStack, \
	pfnStartAddr, \
	pvParam, \
	fdwCreate, \
	pdwThreadId) \
	((HANDLE)_beginthreadex( (void*) (psa), \
	(unsigned) (cbStack), \
	(PTHREAD_START) (pfnStartAddr), \
	(void*) (pvParam), \
	(unsigned) (fdwCreate), \
	(unsigned*) (pdwThreadId)))
#endif

class KFileDownThread
{
public:
	KFileDownThread(FileDownloadManager& kobject, void (FileDownloadManager::*pfnRun)(KHttpWrapper *), DWORD dwTimeout );
	~KFileDownThread();

	bool Begin();
	void End(DWORD dwTimeout);
	void End();

	inline void Run();

	DWORD GetTimeout() { return m_dwTimeout; }
	void SetTimeout(DWORD dwTimeout) { m_dwTimeout = dwTimeout; }
	DWORD GetThreadID() { return m_dwThreadID; }

protected:
	static unsigned int WINAPI ThreadProc(LPVOID pvParam)
	{
		KFileDownThread* thread = reinterpret_cast<KFileDownThread*>(pvParam);
		thread->Run();
		return true;
	}

protected:
	HANDLE			m_hKillEvent;
	HANDLE			m_hThread;
	KHttpWrapper    m_httpWrapper; ///< Internet handle.
	GCFTP		    m_kFTP; ///< Internet handle.
	DWORD			m_dwThreadID;

	FileDownloadManager&          m_kobject;
	void       (FileDownloadManager::*m_pfnRun)(KHttpWrapper *);
	DWORD       m_dwTimeout;
};
