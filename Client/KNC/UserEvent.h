#pragma once

#include "Event.h"
#include "ObjectPool/BlockAllocator.h"

class KUserEvent : public KEvent, public BlockAllocator<KUserEvent, 1000>
{
    NiDeclareRTTI;
    DeclareEvent( KUserEvent );   // 1. 매크로 적는다.

public:
    // 2. ENUM_EVENT_ID의 이름으로 enum 선언
#   undef  _ENUM
#   define _ENUM( id ) id,
    enum ENUM_EVENT_ID { 
#       include "UserEvent_def.h" 
    };

    KUserEvent(void);
    virtual ~KUserEvent(void);
};