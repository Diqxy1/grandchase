#ifndef _GCSTATEGAMEEXIT_H_
#define _GCSTATEGAMEEXIT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"

class KGCStateExitGame : public KGCState
{
public:
    KGCStateExitGame(void);
    virtual ~KGCStateExitGame(void);

public:
    virtual bool OnInitBeforeStateChange(GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove(float fElapsedTime);
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();

public:
    HRESULT InitDevice( void );
    HRESULT LoadTexture( void );
    void	Render2D( LPDIRECT3DTEXTURE9 pTexture , RECT rtScreen ,D3DXVECTOR4 rtUv, float fWidthRatio = 1.0f, bool bRotate=false );
    void	DeleteDevice( void );

    void    SetExitType( KGCGlobal::GAME_EXIT_TYPE eExitType, int nSocketError );

private:
    LPDIRECT3DTEXTURE9  m_pImageTextureL;
	LPDIRECT3DTEXTURE9  m_pImageTextureR;
	LPDIRECT3DTEXTURE9  m_pImageTextureB;
    KD3DUIVertex        m_rect[4];

    int                 m_iFrameCount;
    KGCGlobal::GAME_EXIT_TYPE   m_eExitType;
    int                 m_nSocketError;
};

extern KGCStateExitGame*    g_pStateExitGame;

#endif//_GCSTATEGAMEEXIT_H_
