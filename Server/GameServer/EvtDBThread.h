#pragma once
#include "SubThread.h"
#include "IntEvent.h"
#include "UserEvent.h"
#include "UserPacket.h"
#include "odbc/Odbc.h"

class KGSSimLayer;
class KEvtDBThread : public KSubThread< KUserEvent >
{
    typedef KGSSimLayer SLayerType;
    DeclareDBThread;
public:

    KEvtDBThread();
    virtual ~KEvtDBThread(void);

protected:
    // derived from KDBHandler
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );

protected:
  
    //INT_DECL_ON_FUNC( DB_EVENT_NM6TH_ANNIVERSARY );
};
