#pragma once
#include "SubThread.h"
#include "IntEvent.h"
#include "MatchPacket.h"
#include "odbc/Odbc.h"
#include "MatchEvent.h"

class KMatchSimLayer;
class KMatchDBThread : public KSubThread< KMatchEvent >
{
    typedef KMatchSimLayer SLayerType;
    DeclareDBThread;
    DeclareDBThreadName( Stat );

public:

    KMatchDBThread();
    virtual ~KMatchDBThread(void);

protected:
    // derived from KThreadManager
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );

    void SetServerListFromDB( IN const int& nProtocol );
    bool GetServerList( IN const int& nProtocol, OUT std::vector<KServerInfo>& vecServerList );

protected:
   _INT_DECL_ON_FUNC( DB_EVENT_FIRST_INIT_REQ, int );
};
