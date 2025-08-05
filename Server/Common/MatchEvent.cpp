#if (_MSC_VER < 1310 )
#pragma warning( disable : 4786 )
#endif

#include "MatchEvent.h"
#include "dbg/dbg.hpp"

NiImplementRTTI(KMatchEvent, KEvent);
ImplementEvent(KMatchEvent);   // 1. 매크로 적는다.

// 2. ms_szEventID의 이름으로 이벤트 ID 문자열 변수 초기화.
#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t*  KMatchEvent::ms_szEventID[] = {
	#include "MatchEvent_def.h"
};

KMatchEvent::KMatchEvent(void)
{
}

KMatchEvent::~KMatchEvent(void)
{
}