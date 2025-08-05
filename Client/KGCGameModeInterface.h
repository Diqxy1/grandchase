#ifndef _KGCGAMEMODEINTERFACE_H_
#define _KGCGAMEMODEINTERFACE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
//#include <string>
#include "KGCSantaEvent.h"
#include "ProtectedType.h"
//#include "GCGlobal.h"
#include "TypeDef.h"
#define LAST_REBIRTH_CHANCE_TERM (boost::mpl::multiplies<\
									boost::mpl::int_<55>,\
									boost::mpl::int_<5>\
								>::type::value)

class KGCGameModeInterface
{
protected:
//변수
	int             m_iOldNumLife;
	int             m_iNumLife;
	int             m_iNumNet;
    float           m_fAccumulateTime;

	int             m_MonsterNumToStart;
	KProtectedType<int>             m_NextMonsterStartTime;

	// 수호반지 관련
	//KProtectedType<int>             m_iRebirthCount;
	DWORD			m_dwLastRebirthTime;
    bool            m_bRequestRebirth;
public:
    KGCSantaEvent   m_kSanta;

//함수
public:
    KGCGameModeInterface(void);
    virtual ~KGCGameModeInterface(void);
    virtual HRESULT FrameMove(float fElapsedTime);
	virtual HRESULT Render();
	virtual HRESULT InitAtGame(){ return S_OK; }
	virtual HRESULT InitAtLoading(){ return S_OK; }
    virtual HRESULT CustomFrameMove( float fElapsedTime ) = 0;
    virtual HRESULT CalcExpAndGP() = 0;
	virtual bool CheckGameEnd() = 0;
	virtual void GameEnd() = 0;
    virtual void GameEndProcess();
    virtual void SetArenaCoolTime( int iTime ){};
    virtual int GetArenaCoolTime(){ return 0; };
    virtual void SetGameCoolTime( int iTime ){};
    virtual int GetGameCoolTime(){ return 0; };

    void ShowLevelUp();
	bool CheckExitGame();
	void CheckReplay();				//대전에서만 반응하지만 차후 어떻게 될지 몰라서
	void OnRecvGameEndPacket();
	bool VerifyRebirth();
	void Rebirth(DWORD dpnid);

    void SetPlayerGameResult( int iPlayerIndex_, char cCharType, float fExpWeight_, float fGpWeight_, bool bWin_, bool bLimit30Lv_ = false );
    void SetPlayerGameResult( int iPlayerIndex_, char cCharType, int iExp_, int iGP_, bool bWin_, bool bLimit30Lv_ = false );

	bool BeginScene();
	void EndScene();
    void ClearScene();

    void CheckStartPlayer();

    //void SendRisenEffectPacket( D3DXVECTOR2& vPos );
    //void RigenMonster( int iMonID_, int iMonLevel, bool bBoss_, const std::string& strAI_, int iMaxMon_, 
    //                    bool bRegenEffect_ = true, float x = -100.0f, float y = -100.0f, int iLevel = -1 );
    //void ResurrectMonster( int MonsterIndex_ );

    inline void SetLastRebirthTime( DWORD dwLastBirthTime ) { m_dwLastRebirthTime = dwLastBirthTime; }

    void ClearEventResource();
    virtual HRESULT ON_RENDER_GC_LAYER_SKY();
	virtual HRESULT ON_GC_LAYER_SCROLLED_LAYER1();
	virtual HRESULT ON_GC_LAYER_SCROLLED_LAYER2();
	virtual HRESULT ON_GC_LAYER_SCROLLED_LAYER3();
    virtual HRESULT ON_RENDER_GC_LAYER_BETWEEN_SKY_BLOCK();
    virtual HRESULT ON_RENDER_GC_LAYER_BLOCK();
    virtual HRESULT ON_RENDER_GC_LAYER_BETWEEN_BLOCK_CHAR();
    virtual HRESULT ON_RENDER_GC_LAYER_CHAR();
    virtual HRESULT ON_RENDER_GC_LAYER_BETWEEN_CHAR_FRONT();
    virtual HRESULT ON_RENDER_GC_LAYER_FRONT_MAP();
    virtual HRESULT ON_RENDER_GC_LAYER_TEXT();
    virtual HRESULT ON_RENDER_GC_LAYER_FACE();
    virtual HRESULT ON_RENDER_GC_LAYER_UNDER_UI();
    virtual HRESULT ON_RENDER_GC_LAYER_UI();
    virtual HRESULT ON_RENDER_GC_LAYER_ANNOUNCE();
    virtual HRESULT ON_RENDER_GC_LAYER_WEATHER_SYSTEM();
    virtual HRESULT ON_RENDER_GC_CHAR_OVER_UI_1();
    virtual HRESULT ON_RENDER_GC_CHAR_OVER_UI_2();

    bool GetRequestRebirth() const { return m_bRequestRebirth; }
    void SetRequestRebirth(bool val) { m_bRequestRebirth = val; }

protected:
	void CheckRebirth();

public:


    virtual void ProcessLastRebirthChance() {}
    virtual void InitRebirthTimer();

    float GetCharManaAutoRecoveryRatio();
    bool IsMPRecoveryMode();


    int     m_nRebirthTimer;
    bool    m_bRebirthProcessing;
};

#endif//_KGCGAMEMODEINTERFACE_H_
