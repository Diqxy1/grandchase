#ifndef _KGCMATCH_H_
#define _KGCMATCH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"

class KGCMatch : public KGCGameModeInterface
{
public:
	KGCMatch(void);
	virtual ~KGCMatch(void);

public:
	virtual HRESULT InitAtGame();
	virtual HRESULT InitAtLoading(){ return S_OK; }
	virtual HRESULT CustomFrameMove( float fElapsedTime );
	virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
	virtual void GameEnd();

	void CheckFastMode();
    void CheckFatalMode();
	void SetFastMode(bool bFastMode_ ) { m_bFastMode = bFastMode_; }
	void SetFatalMode(bool bFatalMode_ ) { m_bFatalMode = bFatalMode_; }

protected:
	bool    m_bFastMode;
	bool    m_bFatalMode;
    UINT m_uiFrameCount;
};

#endif // _KGCMATCH_H_
