#ifndef _GCSTATEGUILD_H_
#define _GCSTATEGUILD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateGuild : public KGCState
{
public:
	KGCStateGuild(void);
	virtual ~KGCStateGuild(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
};

#endif //_GCSTATEGUILD_H_
