#pragma once

#include "KGCState.h"

class KGCStateEndGame : public KGCState
{
public:
	KGCStateEndGame(void);
	~KGCStateEndGame(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void Process_Admin_Command( AdminCommandType _ACT );
	virtual void OnSetupFSM();
};
