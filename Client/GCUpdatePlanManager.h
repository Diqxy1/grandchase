#pragma once
#include "KNCSingleton.h"
class GCUpdatePlanManager
{
    DeclareSingleton(GCUpdatePlanManager);



public:
    GCUpdatePlanManager(void);
    ~GCUpdatePlanManager(void);


    void Update_EVENT_CLIENT_CONTENTS_OPEN_INFO( IN KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kRecv_ );

    void Update_EVENT_CLIENT_CONTENTS_FIRST_INIT_INFO( IN KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT& kRecv_ );

    void Update_EVENT_ITEM_BUY_INFO_NOT( IN KItemBuyInfo& kRecv_ );

    void Update_EVENT_UPDATE_EVENT_SCRIPT_NOT( IN std::vector< int >& kRecv_ );

};
DefSingletonInline(GCUpdatePlanManager);
