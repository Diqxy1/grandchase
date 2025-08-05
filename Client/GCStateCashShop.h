#ifndef _GCSTATECASHSHOP_H_
#define _GCSTATECASHSHOP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateCashShop : public KGCState
{
public:
	KGCStateCashShop(void);
	virtual ~KGCStateCashShop(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
};

#endif // _GCSTATECASHSHOP_H_
