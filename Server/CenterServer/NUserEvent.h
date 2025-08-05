#pragma once

#include "Event.h"
#include "ObjectPool/BlockAllocator.h"

class KNUserEvent : public KEvent, public BlockAllocator<KNUserEvent, 1000>
{
    NiDeclareRTTI;
    DeclareEvent( KNUserEvent );
public:

#   undef  _ENUM
#   define _ENUM( id ) id,
    enum ENUM_EVENT_ID { 
#       include "NUserEvent_def.h" 
    };

    KNUserEvent(void);
    ~KNUserEvent(void);
};
