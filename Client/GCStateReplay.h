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

	virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );			//������Ʈ�� �ٲ�� ������ �ʱ�ȭ ���ִ� �κ��Դϴ�. �����ϸ� ������Ʈ�� �ٲ��� �ʾƿ�.
	virtual bool OnDestroyBeforeStateChange();
	virtual bool FrameMove(float fElapsedTime);							//�����Ӹ��� ȣ�� �˴ϴ�.
	virtual bool Render();												//�׷��ݴϴ�.
	virtual bool OnInit();												//������Ʈ�� �ٲ�� �� �� �ʱ�ȭ ���ִ� �κ��Դϴ�.	
	virtual void OnDestroy();
	virtual void OnSetupFSM();

	void DeleteReplayData();
};
