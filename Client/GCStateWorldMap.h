#ifndef _GCSTATEWORLDMAP_H_
#define _GCSTATEWORLDMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateWorldMap : public KGCState
{
public:
    KGCStateWorldMap(void);
    virtual ~KGCStateWorldMap(void);

public:
    virtual bool OnInitBeforeStateChange( GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove( float fElapsedTime );
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();
};

#endif //_GCSTATEWORLDMAP_H_