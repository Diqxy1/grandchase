#pragma once

#include "KGCState.h"
#include <WTypes.h>

class KGCStateReplay : public KGCState
{
protected:
	bool	m_bFailed;
	DWORD	m_dwLoadCount;
public:
	KGCStateReplay(void);
	~KGCStateReplay(void);

	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );			//스테이트가 바뀌기 이전에 초기화 해주는 부분입니다. 실패하면 스테이트가 바뀌지 않아요.
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);							//프레임마다 호출 됩니다.
	virtual bool Render();												//그려줍니다.
	virtual bool OnInit();												//스테이트가 바뀌고 난 후 초기화 해주는 부분입니다.	
	virtual void OnDestroy();
	virtual void OnSetupFSM();

	void DeleteReplayData();
};
