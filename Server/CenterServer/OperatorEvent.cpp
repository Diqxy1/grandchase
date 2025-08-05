#include "OperatorEvent.h"
#include "DefaultFSM.h"
NiImplementRTTI( KOperatorEvent, KEvent );
ImplementEvent( KOperatorEvent );   // 1. 매크로 적는다.

// 2. ms_szEventID의 이름으로 이벤트 ID 문자열 변수 초기화.
#undef _ENUM
#define _ENUM( id ) L ## #id,
const wchar_t*  KOperatorEvent::ms_szEventID[] = { 
#include "OperatorEvent_def.h" 
};

KOperatorEvent::KOperatorEvent(void)
{
}

KOperatorEvent::~KOperatorEvent(void)
{
}