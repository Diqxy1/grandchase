#ifndef _GCSTATEOPTION_H_
#define _GCSTATEOPTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateOption : public KGCState
{
public:
	KGCStateOption(void);
	virtual ~KGCStateOption(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
};

#endif _GCSTATEOPTION_H_
