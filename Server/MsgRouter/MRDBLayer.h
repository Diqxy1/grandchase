#pragma once
#include <KNCSingleton.h>

#include "DBLayer.h"
#include "ToString.h"
#include "MRPacket.h"

struct lua_State;
class KODBC;
class KMRDBLayer : public KDBLayer
{
    DeclareSingleton( KMRDBLayer );
    DeclToStringW;
    NiDeclareRTTI;

protected:
    KMRDBLayer(void);
public:
    virtual ~KMRDBLayer(void);

    virtual void RegToLua( std::shared_ptr<lua_State> sptlua );

protected:
    // util function
    virtual std::shared_ptr<KThread> CreateThread();  // derived from KThreadManager
};

DeclOstmOperatorA( KMRDBLayer );
DefSingletonInline( KMRDBLayer );