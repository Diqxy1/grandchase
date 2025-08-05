#pragma once

#include "KGCState.h"

class KGCStateSquare : public KGCState
{
public:
    KGCStateSquare(void);
    virtual ~KGCStateSquare(void);

public:
    virtual bool OnInitBeforeStateChange( GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove( float fElapsedTime );
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();
};

