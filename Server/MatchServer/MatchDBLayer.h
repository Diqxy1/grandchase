#pragma once
#include <KNCSingleton.h>

#include "DBLayer.h"
#include "ToString.h"
#include "MatchPacket.h"

struct lua_State;
class KODBC;
class KMatchDBLayer : public KDBLayer
{
    DeclareSingleton( KMatchDBLayer );
    DeclToStringW;
    NiDeclareRTTI;

protected:
    KMatchDBLayer(void);
public:
    virtual ~KMatchDBLayer(void);
    virtual void RegToLua( boost::shared_ptr<lua_State> sptlua );
    void Init_ODBC( const char* szMainODBC, const char* szStatODBC );

protected:
    // util function
    virtual boost::shared_ptr<KThread> CreateThread();  // derived from KThreadManager
};

DeclOstmOperatorA( KMatchDBLayer );
DefSingletonInline( KMatchDBLayer );