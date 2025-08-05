#pragma once
#include "NetLayer.h"
#include "KNCSingleton.h"

class KMatchNetLayer : public KNetLayer
{
    DeclareSingleton( KMatchNetLayer );
    DeclToStringW;
    NiDeclareRTTI;

public:
    KMatchNetLayer(void);
    virtual ~KMatchNetLayer(void);

    // derived from KNetLayer
    void ResetMaxData();
    virtual bool Init();
    virtual void ShutDown();
    virtual void RegToLua( boost::shared_ptr<lua_State> L );


};

DefSingletonInline( KMatchNetLayer );