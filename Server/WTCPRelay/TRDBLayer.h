#pragma once
#include <KNCSingleton.h>

#include "DBLayer.h"
#include "ToString.h"
#include "TRPacket.h"

struct lua_State;
class KODBC;
class KTRDBLayer : public KDBLayer
{
    DeclareSingleton( KTRDBLayer );
    DeclToStringW;
    NiDeclareRTTI;

protected:
    KTRDBLayer(void);
public:
    virtual ~KTRDBLayer(void);
    virtual void RegToLua( std::shared_ptr<lua_State> sptlua );
    void Init_ODBC( const char* szMainODBC, const char* szStatODBC );

protected:
    // util function
    virtual std::shared_ptr<KThread> CreateThread();  // derived from KThreadManager
};

DeclOstmOperatorA( KTRDBLayer );
DefSingletonInline( KTRDBLayer );