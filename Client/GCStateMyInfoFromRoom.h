#pragma once

#include "KGCState.h"

class GCStateMyInfoFromRoom : public KGCState
{
public:
	GCStateMyInfoFromRoom(void);
	~GCStateMyInfoFromRoom(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
};
