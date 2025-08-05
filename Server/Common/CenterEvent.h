#pragma once
#include "Event.h"

class KCenterEvent : public KEvent
{
	NiDeclareRTTI;
	DeclareEvent(KCenterEvent);   // 1. 매크로 적는다.

public:
	// 2. ENUM_EVENT_ID의 이름으로 enum 선언
#   undef  _ENUM
#   define _ENUM( id ) id,
	enum ENUM_EVENT_ID {
#       include "CenterEvent_def.h"
	};

	KCenterEvent(void);
	virtual ~KCenterEvent(void);
};