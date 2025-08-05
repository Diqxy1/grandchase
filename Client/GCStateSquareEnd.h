#pragma once

#include "KGCState.h"

class KGCStateSquareEnd : public KGCState
{
public:
    KGCStateSquareEnd(void);
    virtual ~KGCStateSquareEnd(void);

public:
    virtual bool OnInitBeforeStateChange( GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove( float fElapsedTime );
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();

private:
    GAME_STATE m_eBeforeState;
};
