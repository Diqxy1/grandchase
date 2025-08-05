#ifndef _KGCDOTA_H_
#define _KGCDOTA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCGameModeInterface.h"
#include "../gcui/KGCDotaOverLay.h"

class KGCDota : public KGCGameModeInterface
{
public:
    KGCDota(void);
    virtual ~KGCDota(void);

public:
    virtual HRESULT InitAtGame();
    virtual HRESULT InitAtLoading();
    virtual HRESULT CustomFrameMove( float fElapsedTime );
    virtual HRESULT CalcExpAndGP();
    virtual bool CheckGameEnd();
    virtual void GameEnd();
    virtual void ProcessLastRebirthChance(){};
    virtual HRESULT Render();
    virtual void SetArenaCoolTime( int iTime ) {  m_iArenaCoolTime = iTime;   };
    virtual int GetArenaCoolTime(){  return m_iArenaCoolTime;   };
    virtual void SetGameCoolTime( int iTime ) {  m_iPlayerGameStartCoolTime = iTime;   };
    virtual int GetGameCoolTime() {  return m_iPlayerGameStartCoolTime;   };

private:
    void FrameMove_Item();
    void CalcDefault_D_Point();
    void CalcCore_D_Point();
    void CalcCore_Message();
    bool SetCore_D_Point(int iMonsterID, int iPoint, int *piDPoint);
    void SendCore_D_Point(int *piDPoint);
    void CheckDotaEnd();
    void DestroyDialogUI();
    HRESULT CalcCoolTime();
    void ShowPortalEffect();

public:
    static void LoadScript();
    static void LoadAbility( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadDefault_D_Point( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadCore_D_Point( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadSummonGate( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadStaticMonster( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadMonsterPortal( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadDPointShop( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadDPointShopItem( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadDPointItem( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadDotaMsg( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void LoadDotaGameGuide( KLuaManager &luaMgr, SDotaTemplate &kDotaInfo );
    static void SendDotaItemInfo(int iIndex=-1, float fValue=0.0f);
	HRESULT InitAtLoadMonster();

private:
    SDotaTemplate   *m_pCurDotaInfo;
    int             m_iGameStartCoolTime;
    int             m_iPlayerGameStartCoolTime;
    int             m_iArenaCoolTime;
    int             m_iCanabanCoreDPointCool;
    int             m_iSerdinCoreDPointCool;
    int             m_iEndGameCoolTime;
    int             m_iDefaultDPointCool;
    int             m_iSerdinCoreHp;
    int             m_iCanabanCoreHp;

private:
    bool            m_bFirstMsg_Serdin;
    bool            m_bFirstMsg_Canaban;
    bool            m_bGameEnd;
};

#endif //_KGCDOTA_H_
