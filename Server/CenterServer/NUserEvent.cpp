#if (_MSC_VER < 1310 )
#pragma warning( disable : 4786 )
#endif

#include ".\nuserevent.h"
#include "dbg/dbg.hpp"

NiImplementRTTI( KNUserEvent, KEvent );
ImplementEvent( KNUserEvent );   // 1. 매크로 적는다.

// 2. ms_szEventID의 이름으로 이벤트 ID 문자열 변수 초기화.
#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t*  KNUserEvent::ms_szEventID[] = { 
#include "NUserEvent_def.h" 
};

KNUserEvent::KNUserEvent(void)
{
}

KNUserEvent::~KNUserEvent(void)
{
}
