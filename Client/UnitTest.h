extern int GetNowProcessMemory();

#ifdef _LOADING_TIMECHECK_

extern DWORD g_dwCurtime;
extern DWORD g_dwTime ;
extern char g_strTemp[512];
extern FILE* g_pFile ;
extern void CheckNowProcessMemory( FILE* fp );
#define MAKE_LOAD_PROFILE( comment )    g_dwCurtime = timeGetTime();\
	memset( g_strTemp, 0, sizeof(char)*256 );\
    sprintf( g_strTemp, "%s\t%u\t", comment, g_dwCurtime - g_dwTime );\
    if ( g_pFile ) \
	    fwrite( g_strTemp, sizeof(char), strlen( g_strTemp ), g_pFile );\
	g_dwTime = timeGetTime();\
	CheckNowProcessMemory( g_pFile );

#endif

