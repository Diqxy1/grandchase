#pragma once
#include "NetLayer.h"
#include "KNCSingleton.h"

class KMRNetLayer : public KNetLayer
{
    DeclareSingleton( KMRNetLayer );
    DeclToStringW;
    NiDeclareRTTI;

public:
    KMRNetLayer(void);
    virtual ~KMRNetLayer(void);

    // derived from KNetLayer
    void ResetMaxData();
    virtual bool Init();
    //virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

protected:

public: // lua access
};

DefSingletonInline( KMRNetLayer );