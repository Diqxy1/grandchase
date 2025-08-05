#ifndef _KGCGUILDWAR_H_
#define _KGCGUILDWAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"

class KGCGuildWar : public KGCGameModeInterface
{
public:
    KGCGuildWar(void);
    virtual ~KGCGuildWar(void);
public:
	virtual HRESULT InitAtGame(){ return S_OK; }
	virtual HRESULT InitAtLoading();
    virtual HRESULT CustomFrameMove( float fElapsedTime );
    virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
	virtual void GameEnd();
};

#endif // _KGCGUILDWAR_H_
