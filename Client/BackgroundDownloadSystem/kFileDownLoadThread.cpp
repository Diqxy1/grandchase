#include "KFileDownLoadThread.h"


KFileDownThread::KFileDownThread(FileDownloadManager& kobject_, void (FileDownloadManager::*pfnRun_)(KHttpWrapper *), DWORD dwTimeout_)
: m_kobject(kobject_), m_dwThreadID( 0 )
{
	m_dwTimeout     = dwTimeout_;
	m_pfnRun        = pfnRun_;
	m_hKillEvent    = NULL;
	m_hThread       = NULL;
}

KFileDownThread::~KFileDownThread()
{
	End();

	if( NULL != m_hKillEvent )
	{
		::CloseHandle( m_hKillEvent );
		m_hKillEvent = NULL;
	} // if
}

bool KFileDownThread::Begin()
{
	if( m_hThread != NULL )
		return false;

	m_hKillEvent = CreateEvent(NULL, false, false, NULL);
	if( NULL == m_hKillEvent )
		return false;

	m_hThread = _BEGINTHREADEX( NULL,
		0,
		ThreadProc,
		this,
		0,
		&m_dwThreadID );
	if( NULL == m_hThread )
	{
		::CloseHandle( m_hKillEvent);
		m_hKillEvent = NULL;
		return false;
	} // if

	if (m_httpWrapper.Open()==false)
	{
		::CloseHandle( m_hKillEvent);
		m_hKillEvent = NULL;
		return false;
	}

	return true;    
}

void KFileDownThread::End(DWORD dwTimeout_)
{
	if( NULL != m_hThread )
	{
		SetEvent( m_hKillEvent );
		::WaitForSingleObject( m_hThread, dwTimeout_ );
		::CloseHandle( m_hThread );
		m_hThread = NULL;
		m_httpWrapper.Close();
	} // if
}

void KFileDownThread::End()
{
	if( NULL != m_hThread )
	{
		SetEvent( m_hKillEvent );
		::WaitForSingleObject( m_hThread, INFINITE );
		::CloseHandle( m_hThread );
		m_hThread = NULL;
		m_httpWrapper.Close();
	} // if
}

void KFileDownThread::Run()
{
	DWORD   ret = NULL;
	bool    bEnd = false;

	while( !bEnd)
	{
		ret = ::WaitForSingleObject( m_hKillEvent, m_dwTimeout );
		switch( ret )
		{
		case WAIT_OBJECT_0:
			bEnd = true;
			break;
		case WAIT_TIMEOUT:
			(m_kobject.*m_pfnRun)(&m_httpWrapper);
			break;
		} // switch
	} // while
}

