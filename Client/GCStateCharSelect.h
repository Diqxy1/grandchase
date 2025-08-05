#ifndef _GCSTATECHARSELECT_H_
#define _GCSTATECHARSELECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateCharSelcet : public KGCState
{
public:
	KGCStateCharSelcet(void);
	~KGCStateCharSelcet(void);
public:
	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
	void VerifyCharCardInfo( void );

private:
	bool m_bFistConnect;
};

#endif//_GCSTATECHARSELECT_H_
