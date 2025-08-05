#ifndef _KGCTAGMATCH_H_
#define _KGCTAGMATCH_H_

////#include <vector>
#include "KGCGameModeInterface.h"
//#include "GCEnum.h"

class KGCTagMatch : public KGCGameModeInterface
{
public:
    KGCTagMatch(void);
    virtual ~KGCTagMatch(void);

public:
    virtual HRESULT InitAtGame(){ return S_OK; }
    virtual HRESULT InitAtLoading();
    virtual HRESULT CustomFrameMove( float fElapsedTime );
    virtual HRESULT CalcExpAndGP();
    virtual bool CheckGameEnd();
    virtual void GameEnd();
};

#endif // _KGCTAGMATCH_H_
