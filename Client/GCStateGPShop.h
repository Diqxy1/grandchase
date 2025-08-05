#ifndef DISABLE_MAP_SHOP // Kawan>

#ifndef _GCSTATEGPSHOP_H_
#define _GCSTATEGPSHOP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateGPShop : public KGCState
{
public:
	KGCStateGPShop(void);
	virtual ~KGCStateGPShop(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
};

#endif // _GCSTATEGPSHOP_H_

#endif