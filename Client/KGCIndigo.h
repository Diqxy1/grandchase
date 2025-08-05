#ifndef _KGCINDIGO_H_
#define _KGCINDIGO_H_

////#include <vector>
#include "KGCGameModeInterface.h"
//#include "GCEnum.h"

class KGCIndigo : public KGCGameModeInterface
{
public:
    KGCIndigo(void);
    virtual ~KGCIndigo(void);

public:
    virtual HRESULT InitAtGame(){ return S_OK; }
    virtual HRESULT InitAtLoading();
    virtual HRESULT CustomFrameMove( float fElapsedTime );
    virtual HRESULT CalcExpAndGP();
    virtual bool CheckGameEnd();
    virtual void GameEnd();
};

#endif // _KGCINDIGO_H_