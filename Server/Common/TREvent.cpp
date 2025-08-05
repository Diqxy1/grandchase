#if (_MSC_VER < 1310 )
#pragma warning( disable : 4786 )
#endif

#include "TREvent.h"
#include "dbg/dbg.hpp"

NiImplementRTTI(KTREvent, KEvent);
ImplementEvent(KTREvent);   // 1. 매크로 적는다.

// 2. ms_szEventID의 이름으로 이벤트 ID 문자열 변수 초기화.
#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t*  KTREvent::ms_szEventID[] = {
	#include "TREvent_def.h"
};

KTREvent::KTREvent(void)
{
}

KTREvent::~KTREvent(void)
{
}