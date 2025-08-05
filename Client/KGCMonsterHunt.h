#ifndef _KGCMONSTERHUNT_H_
#define _KGCMONSTERHUNT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"
class KGCMonsterHunt : public KGCGameModeInterface
{
public:
    KGCMonsterHunt(void);
    virtual ~KGCMonsterHunt(void);

public:
    virtual HRESULT InitAtGame();
    virtual HRESULT InitAtLoading(){ return S_OK; }
    virtual HRESULT CustomFrameMove( float fElapsedTime );
    virtual HRESULT CalcExpAndGP();
    virtual bool CheckGameEnd();
    virtual void GameEnd();

public:
    static void LoadScript();
};

#endif // _KGCMONSTERHUNT_H_
