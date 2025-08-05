#pragma once

#include "KGC2DObject.h"
#include "KGCGameModeInterface.h"

class KGCMiniGame : public KGCGameModeInterface
{
public:
	KGCMiniGame(void);
	~KGCMiniGame(void);

	KGC2DObject m_test;

public:
	virtual HRESULT CustomFrameMove( float fElapsedTime );
	virtual HRESULT InitAtGame();
	virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
	virtual void GameEnd();
    virtual void GameEndProcess() {};

    void SetCurrentGP( void );
    void SetScoreInfo( void );
    int  SetGradeInfo( void );
    void InitGameData( void );
    void ShowStartMsg( void );

private:
    bool bEndOneGame;       // 잠깐만.. 끝 낼 것인지 안 끝 낼것인지 정하는 중이다. 한판 죽고 말이지...
    bool m_bShowStartMsg;
};
