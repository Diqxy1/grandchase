/**
 * @file	AloFileMonitor.cpp
 * @brief   CAloFileMonitor class 
 * @author	alones
 * @date initial version: 2007.08.22 
 * @brief http://alones.kr/wiki/wiki.php/c_cpp_windows_tip_alofilemonitor
 */
#include "stdafx.h"
#include "AloFileMonitor.h"
#include "io.h"


/** @brief set file monitor, file path and call-back funtion */
bool CAloFileMonitor::SetFileMonitor(std::string filePath, PFnFileChanged pFn)
{
	if( filePath.size() == 0 || pFn == NULL)
		return false;

	int nPos = (int)filePath.rfind("\\");
	if( nPos == -1 )
	{
		nPos = (int)filePath.rfind("/");
		if( nPos == -1)
			return false;
	}

	// set dir path
	m_dirPath = filePath.substr(0, nPos+1);

	// set file path
	m_filePath = filePath;

	// get last-write time of the file	
	GetFileAttributesExA( m_filePath.c_str(), GetFileExInfoStandard, (void*)&m_fileAttData);	
	

	// set function pointer
	m_pFnFileChanged = pFn;
	
	return true;
}

DWORD WINAPI thFnMonitor(LPVOID lpParam);
DWORD WINAPI thFnMonitor(LPVOID lpParam)
{
#if defined( BUG_TRAP )
	BT_SetTerminate();
#endif

	CAloFileMonitor* pMonitor = (CAloFileMonitor*)lpParam;
	if( pMonitor == NULL)
		return 0;

	DWORD dwWaitStatus; 	
	_WIN32_FILE_ATTRIBUTE_DATA fileAttData;
	
	// Watch the C:\WINDOWS directory for file creation and 
	// deletion. 
	
	pMonitor->dwChangeHandles[0] = FindFirstChangeNotificationA( 
		pMonitor->m_dirPath.c_str(),           // directory to watch 
		FALSE,                         // do not watch the subtree 
		FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file name changes 
	
	if (pMonitor->dwChangeHandles[0] == INVALID_HANDLE_VALUE)
	{
		DWORD dError = GetLastError();
		ExitProcess(GetLastError()); 
	}	

	pMonitor->dwChangeHandles[1] = CreateSemaphore( 
		NULL,   // no security attributes
		0,   // initial count
		1,   // maximum count
		NULL);  // unnamed semaphore

	if( pMonitor->dwChangeHandles[1] == NULL)
		return 0;
	
	// Change notification is set. Now wait on both notification 
	// handles and refresh accordingly. 	
	while (TRUE) 
	{ 
		// Wait for notification.
		dwWaitStatus = WaitForMultipleObjects(2, pMonitor->dwChangeHandles, FALSE, INFINITE); 
		
		switch (dwWaitStatus) 
		{ 
        case WAIT_OBJECT_0: 
			
			// check the file is changed
			// Retrieve the file times for the file.

			// if file cannot access, skip
			//if ( -1 != ::_access( pMonitor->m_filePath.c_str(), 0 ) ) break;

			GetFileAttributesExA( pMonitor->m_filePath.c_str(), GetFileExInfoStandard, (void*)&fileAttData);				

			if( CompareFileTime(&fileAttData.ftLastWriteTime, &pMonitor->m_fileAttData.ftLastWriteTime) != 0 )
			{
				(*pMonitor->m_pFnFileChanged)();
				pMonitor->m_fileAttData = fileAttData;
			}

			// A file was created or deleted in C:\WINDOWS. 
			// Refresh this directory and restart the 
			// change notification. RefreshDirectory is an 
			// application-defined function. 			
            if ( FindNextChangeNotification( pMonitor->dwChangeHandles[0]) == FALSE ) 
				return 0;
			break;
        case WAIT_OBJECT_0 + 1: 
            FindCloseChangeNotification(pMonitor->dwChangeHandles[0]);
            return 0;
        default: 
            Sleep( 100 );
            break;
		} 
	} 


	return 0;
}

/** @brief start monitor */
bool CAloFileMonitor::Start()
{
	DWORD dwThreadId;
	HANDLE hThread;
	
	hThread = CreateThread( 
        NULL,           // no security attributes 
        0,              // use default stack size  
        thFnMonitor,    // thread function 
        this,			// argument to thread function 
        0,              // use default creation flags 
        &dwThreadId);   // returns the thread identifier 

	return true;
}

/** @brief stop monitor */
bool CAloFileMonitor::Stop()
{

	if (!ReleaseSemaphore( 
        dwChangeHandles[1],  // handle to semaphore
        1,           // increase count by one
        NULL) )      // not interested in previous count
	{
		return false;
	}

	::Sleep(100);

	return true;
}