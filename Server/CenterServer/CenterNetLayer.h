#pragma once
#include "NetLayer.h"
#include "KNCSingleton.h"

class KCenterNetLayer : public KNetLayer
{
    DeclareSingleton( KCenterNetLayer );
    DeclToStringW;
    NiDeclareRTTI;

public:
    KCenterNetLayer(void);
    virtual ~KCenterNetLayer(void);

    void ResetMaxData();
    // derived from KNetLayer
    virtual bool Init();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

};

DefSingletonInline( KCenterNetLayer );
