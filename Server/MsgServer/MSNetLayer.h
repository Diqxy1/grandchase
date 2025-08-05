#pragma once
#include "NetLayer.h"
#include "KNCSingleton.h"

class KMSNetLayer : public KNetLayer
{
    DeclareSingleton( KMSNetLayer );
    DeclToStringW;
    NiDeclareRTTI;

public:
    KMSNetLayer(void);
    virtual ~KMSNetLayer(void);

    // derived from KNetLayer
    void ResetMaxData();
    virtual bool Init();
    //virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

public:
};

DefSingletonInline( KMSNetLayer );