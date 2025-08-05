#include "CenterEvent.h"
#include "DefaultFSM.h"
#include "dbg/dbg.hpp"

NiImplementRTTI(KCenterEvent, KEvent);
ImplementEvent(KCenterEvent);   // 1. 매크로 적는다.

// 2. ms_szEventID의 이름으로 이벤트 ID 문자열 변수 초기화.
#undef _ENUM
#define _ENUM( id ) L ## #id,
const wchar_t*  KCenterEvent::ms_szEventID[] = {
	#include "CenterEvent_def.h"
};

KCenterEvent::KCenterEvent(void)
{
}

KCenterEvent::~KCenterEvent(void)
{
}