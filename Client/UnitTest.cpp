#include <stdafx.h>
#include "UnitTest.h"


DWORD g_dwCurtime = 0;
DWORD g_dwTime = 0;
char g_strTemp[512] = {0,};
FILE* g_pFile;

#include <Psapi.h>
#pragma comment( lib, "psapi.lib" )

int GetNowProcessMemory()
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, GetCurrentProcessId() );

    if (NULL == hProcess)
        return 0;

	GetProcessMemoryInfo( hProcess , &pmc, sizeof( pmc ) );

    return (int)pmc.WorkingSetSize;
}

#ifdef _LOADING_TIMECHECK_
void CheckNowProcessMemory( FILE* fp )
{
	int iUseMem = GetNowProcessMemory();
	iUseMem /= 1024;

    if ( fp )
	    fprintf( fp, "%d,%03d Kbyte\n\n", iUseMem/1000, iUseMem%1000 );
}
#endif

