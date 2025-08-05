#ifndef _KGCAGIT_H_
#define _KGCAGIT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"
#include "GCExtraMesh.h"

class KGCAgit : public KGCGameModeInterface
{
public:
	KGCAgit(void);
	virtual ~KGCAgit(void);
    float   m_fEventTime;
    DWORD   m_dwEndTimer;	

public:
	virtual HRESULT InitAtGame();
	virtual HRESULT InitAtLoading();
	virtual HRESULT CustomFrameMove( float fElapsedTime );
	virtual HRESULT CalcExpAndGP();
	virtual bool CheckGameEnd();
    virtual void GameEndProcess();
	virtual void GameEnd();

    bool Picking( int iMouseBtn );
    bool GetAgitMousePos( D3DXVECTOR2& dxvPos );

    static bool LoadScript();
    static bool LoadMotionScript(KLuaManager &luaMgr);
    static bool LoadExtraMeshScript(KLuaManager &luaMgr);
    static bool LoadAzitMotion();
};

#endif // _KGCAGIT_H_
