#pragma once
#include "NetLayer.h"
#include "KNCSingleton.h"

class KTRNetLayer : public KNetLayer
{
    DeclareSingleton( KTRNetLayer );
    DeclToStringW;
    NiDeclareRTTI;

public:
    KTRNetLayer(void);
    virtual ~KTRNetLayer(void);

    // derived from KNetLayer
    void ResetMaxData();
    virtual bool Init();
    virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );


};

DefSingletonInline( KTRNetLayer );