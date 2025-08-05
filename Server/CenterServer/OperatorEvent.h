#pragma once
#include "Event.h"

SmartPointer(KOperatorEvent);

class KOperatorEvent : public KEvent
{
    NiDeclareRTTI;
    DeclareEvent( KOperatorEvent );     // 1. 매크로 적는다.

public:
    // 2. ENUM_EVENT_ID의 이름으로 enum 선언
#   undef  _ENUM
#   define _ENUM( id ) id,
    enum ENUM_EVENT_ID { 
#       include "OperatorEvent_def.h" 
    };

    KOperatorEvent(void);
    virtual ~KOperatorEvent(void);
};
