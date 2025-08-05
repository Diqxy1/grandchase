#ifndef _KGCDeathMatch_H_
#define _KGCDeathMatch_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"

class KGCDeathMatch : public KGCGameModeInterface
{
public:
	KGCDeathMatch(void);
	virtual ~KGCDeathMatch(void);

public:
	virtual HRESULT InitAtGame();
	virtual HRESULT InitAtLoading(){ return S_OK; }
	virtual HRESULT CustomFrameMove( float fElapsedTime );

    HRESULT ON_RENDER_GC_LAYER_TEXT();

	virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
	virtual void GameEnd();

    void UpdateStatBalace();
    
    KProtectedType<int>       m_iGoal;
    KProtectedType<float>     m_fTime;
    
    int       m_iLastJoinSendTime;
    bool      m_bTimeOverChecked;

};

#endif // _KGCDeathMatch_H_
