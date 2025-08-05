#pragma once
#include "SubThread.h"
#include "IntEvent.h"
#include "TRPacket.h"
#include "odbc/Odbc.h"
#include "TREvent.h"
#include "CommonPacket.h"

class KTRSimLayer;
class KTRDBThread : public KSubThread< KTREvent >
{
    typedef KTRSimLayer SLayerType;
    DeclareDBThread;
    DeclareDBThreadName( Stat );

public:

    KTRDBThread();
    virtual ~KTRDBThread(void);

protected:
    // derived from KThreadManager
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );

    bool UpdateStatType( IN const int nType, IN const int nServerID, IN const int nTypeCount );
    int GetServerUID( IN const std::wstring strIP, IN const USHORT usPort, IN const int nType );
    bool UpdateCCU( IN const int nUID, IN const int nStatus, IN const int nCnt );

protected:
    INT_DECL_ON_FUNC( DB_EVENT_FIRST_SERVER_INFO_REQ );
    INT_DECL_ON_FUNC( DB_ETR_TYPE_COUNT_STAT );
};
