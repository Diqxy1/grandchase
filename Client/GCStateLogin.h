#ifndef _GCSTATELOGIN_H_
#define _GCSTATELOGIN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateLogin : public KGCState
{
public:
    KGCStateLogin(void);
    ~KGCStateLogin(void);
public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove(float fElapsedTime);
    virtual bool Render();
    virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
};

#endif//_GCSTATELOGIN_H_
