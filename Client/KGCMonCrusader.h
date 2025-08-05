#ifndef _KGCMONCRUSADER_H_
#define _KGCMONCRUSADER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"
class KGCMonCrusader : public KGCGameModeInterface
{
public:
    KGCMonCrusader(void);
    virtual ~KGCMonCrusader(void);
    float   m_fEventTime;
public:
    virtual HRESULT InitAtGame();
    virtual HRESULT InitAtLoading(){ return S_OK; }
    virtual HRESULT CustomFrameMove( float fElapsedTime );
    virtual HRESULT CalcExpAndGP();
    virtual bool CheckGameEnd();
    virtual void GameEnd();

    virtual void ProcessLastRebirthChance();

    static void LoadScript();

private:
    int m_iRemainMonster;
    int m_iGameEndCheck;
};

#endif //_KGCMONCRUSADER_H_
