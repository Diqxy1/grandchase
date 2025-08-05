#ifndef _GCSTATESERVER_H_
#define _GCSTATESERVER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateServer : public KGCState
{
public:
    KGCStateServer(void);
    virtual ~KGCStateServer(void);

public:
    virtual bool OnInitBeforeStateChange( GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove( float fElapsedTime );
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();
};

#endif //_GCSTATESERVER_H_
