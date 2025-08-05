#ifndef _GCSTATEREGISTER_H_
#define _GCSTATEREGISTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateRegister : public KGCState
{
public:
    KGCStateRegister(void);
    ~KGCStateRegister(void);
public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove(float fElapsedTime);
    virtual bool Render();
    virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
};

#endif//_GCSTATEREGISTER_H_
