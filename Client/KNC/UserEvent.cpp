#include "stdafx.h"
#if (_MSC_VER < 1310 )
#pragma warning( disable : 4786 )
#endif

#include "UserEvent.h"


NiImplementRTTI( KUserEvent, KEvent );
ImplementEvent( KUserEvent );   // 1. 매크로 적는다.

// 2. ms_szEventID의 이름으로 이벤트 ID 문자열 변수 초기화.
#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t*  KUserEvent::ms_szEventID[] = { 
    #include "UserEvent_def.h" 
};

KUserEvent::KUserEvent(void)
{

}

KUserEvent::~KUserEvent(void)
{

}