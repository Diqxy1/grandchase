#include "stdafx.h"
//
//


// 060206. jseop. nProtect game guard global variable.
#ifndef NO_GAMEGUARD
#include "NPGameLib.h"
#if defined( NATION_KOREA )
CNPGameLib g_CNPGameLib( L"GrandChase" );
#elif defined( NATION_JAPAN )
CNPGameLib g_CNPGameLib( L"GrandChaseJP" );
#elif defined( NATION_TAIWAN ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseTW" );
#elif defined( NATION_TAIWAN ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseTWTest" );
#elif defined( NATION_HONGKONG ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseHK" );
#elif defined( NATION_HONGKONG ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseHKTest" );
#elif defined( NATION_USA ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseUS" );
#elif defined( NATION_USA ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseUSTest" );
#elif defined( NATION_BRAZIL ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseBR" );
#elif defined( NATION_BRAZIL ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseBRTest" );
#elif defined( NATION_PHILIPPINE ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChasePH" );
#elif defined( NATION_PHILIPPINE ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChasePHTest" );
#elif defined( NATION_IDN ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseID" );
#elif defined( NATION_IDN ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseIDTest" );
#elif defined( NATION_LATINAMERICA ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseSA" );
#elif defined( NATION_LATINAMERICA ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseSATest" );
#elif defined( NATION_EU ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseEU" );
#elif defined( NATION_EU ) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseEUTest" );
#elif defined( NATION_THAILAND ) && defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseTH" );
#elif defined( NATION_THAILAND) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseTHTest" );
#elif defined( NATION_CHINA) && !defined( __PATH__ )
CNPGameLib g_CNPGameLib( L"GrandChaseCNTest" );
#elif defined( NATION_CHINA) 
CNPGameLib g_CNPGameLib( L"GrandChaseCN" );
#endif 
#endif // NO_GAMEGUARD

// 060208. jseop. nProtect game guard check time.
float g_fNPCheckTime = 0.0f;

// 060331. jseop. client close reserving flag.
BOOL g_bReserveClose = FALSE;

//==============================================================================
// 2009.04.21 : tcpl85
// Comment : XTrap Check Time
float g_fXTrapCheckTiem = 0.0f;

