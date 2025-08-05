#pragma once
#include "MatchSvrProxy.h"
#include "KNCSingleton.h"
#include <Thread/Locker.h>
#include <map>
#include <ToString.h>
#include "Socket/Session.h"
#include "MatchEvent.h"
#include "MatchPacket.h"

class KMatchSvrConnector : public KThread
{
    DeclToStringW;
    DeclareSingleton( KMatchSvrConnector );

public:
    KMatchSvrConnector(void);
    virtual ~KMatchSvrConnector(void);

    void Tick();

private:

protected:
    virtual void Run();

};
DefSingletonInline( KMatchSvrConnector );