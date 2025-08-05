#pragma once

#include "KGCState.h"

class KGCStateTutorial : public KGCState
{
public:
	KGCStateTutorial(void);
	~KGCStateTutorial(void);

public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );		
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();

protected:
	int m_iCharacter;

public:
	void SetCharacter( int iChar_ ) { m_iCharacter = iChar_; }
	int GetCharacter() { return m_iCharacter; }
};
