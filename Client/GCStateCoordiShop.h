#ifdef USE_COORDI_SHOP // Kawan>

#ifndef _GCSTATECOORDISHOP_H_
#define _GCSTATECOORDISHOP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateCoordiShop : public KGCState
{
public:
    KGCStateCoordiShop(void);
    virtual ~KGCStateCoordiShop(void);

public:
    virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove(float fElapsedTime);
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();
};

#endif // _GCSTATECOORDISHOP_H_

#endif