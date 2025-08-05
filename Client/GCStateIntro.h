#ifndef _GCSTATEINTRO_H_
#define _GCSTATEINTRO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#include "KGCState.h"

class KGCStateIntro : public KGCState
{
public:
	KGCStateIntro(void);
	virtual ~KGCStateIntro(void);

public:


	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);
	virtual bool Render();
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnSetupFSM();
private:
	void LoadScript();
	bool IsDrawingParticle();
	void OffParticle();		//파티클이 자동적으로 죽지 않았을때(Esc키를 이용하여 넘어 갔을시)

	DWORD		m_dwFrame;
	std::map<DWORD,std::vector<std::string>>	m_mapIntroInfo;

	std::vector< CParticleEventSeqPTR > m_vecIntroEffect;

};

#endif //_GCSTATEINTRO_H_
