#pragma once
#include <KNCSingleton.h>

#include "DBLayer.h"
#include "ToString.h"
#include "MSPacket.h"

struct lua_State;
class KODBC;
class KMSDBLayer : public KDBLayer
{
    DeclareSingleton( KMSDBLayer );
    DeclToStringW;
    NiDeclareRTTI;

protected:
    KMSDBLayer(void);
public:
    virtual ~KMSDBLayer(void);

    virtual void RegToLua( std::shared_ptr<lua_State> sptlua );

protected:
    // util function
    virtual std::shared_ptr<KThread> CreateThread();  // derived from KThreadManager
};

DeclOstmOperatorA( KMSDBLayer );
DefSingletonInline( KMSDBLayer );