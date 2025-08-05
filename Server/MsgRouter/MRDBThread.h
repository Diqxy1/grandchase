#pragma once
#include "SubThread.h"
#include "IntEvent.h"
#include "MRPacket.h"
#include "odbc/Odbc.h"
#include "MREvent.h"

class KMRSimLayer;
class KMRDBThread : public KSubThread< KMREvent >
{
    typedef KMRSimLayer SLayerType;
    DeclareDBThread;
public:

    KMRDBThread();
    virtual ~KMRDBThread(void);

protected:
    // derived from KThreadManager
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );

protected:

    INT_DECL_ON_FUNC( DB_EVENT_SERVER_INFO_REQ );
};
