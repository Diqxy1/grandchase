#ifndef _KGCQUEST_H_
#define _KGCQUEST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"

class KGCQuest : public KGCGameModeInterface
{
public:
	KGCQuest(void);
	virtual ~KGCQuest(void);
    float   m_fEventTime;
    DWORD   m_dwEndTimer;	

public:
	virtual HRESULT InitAtGame();
	virtual HRESULT InitAtLoading();
	virtual HRESULT CustomFrameMove( float fElapsedTime );
	virtual HRESULT Render();
	virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
	virtual void GameEnd();
	void FrameMove_GameEnd(void);

	void DungeonTipMessage();
    virtual void ProcessLastRebirthChance();
};

#endif // _KGCQUEST_H_
