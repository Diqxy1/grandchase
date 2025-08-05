#pragma once

#include "KGCState.h"
#include <WTypes.h>

class KGCStateSquareLoading : public KGCState
{
public:
    KGCStateSquareLoading(void);
    virtual ~KGCStateSquareLoading(void);

public:
    virtual bool OnInitBeforeStateChange( GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove( float fElapsedTime );
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();

private:
    DWORD m_dwLoadingTime;
    bool m_bSendLoadingComplete;
};
