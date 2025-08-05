#if (_MSC_VER < 1310 )
#pragma warning( disable : 4786 )
#endif

#include "MREvent.h"
#include <dbg/dbg.hpp>

NiImplementRTTI( KMREvent, KEvent );
ImplementEvent( KMREvent );   // 1. 매크로 적는다.

// 2. ms_szEventID의 이름으로 이벤트 ID 문자열 변수 초기화.
#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t*  KMREvent::ms_szEventID[] = { 
    #include "MREvent_def.h" 
};

KMREvent::KMREvent(void)
{

}

KMREvent::~KMREvent(void)
{

}