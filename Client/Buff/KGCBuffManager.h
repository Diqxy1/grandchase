#pragma once
////#include <vector>
//#include "define.h"
#include "kgcbuff.h"

class KGCBuff;
class KGCBuffInstance;
class GCDeviceVertexBuffer;

class KGCBuffManager
{
public:
	KGCBuffManager(void);
	~KGCBuffManager(void);

	void Initialize();

	void LoadFromLuaScript(char* strFilename_);
	bool SetMagicEffect(int iPlayerIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_ = 1, int iIndex_ = -1, bool bIsCrashPlayer = true, bool bAccTime_ = false, DWORD dwAttackUnit = 0);
	int FrameMove();

	// iPlayerIndex_의 Player에게 걸린 Buff의 레벨을 돌려준다
	int IsMagicEffect(int iPlayerIndex_, int iBuffIndex_);
	//====================================================================================
	// 2009.03.22 : Jemitgge
	// Comment : 쿨타임 ㅠㅠ 아 젠장 이렇게 해야하나;?
	bool IsCoolTime(int iPlayerIndex_, int iBuffIndex_);
	void ClearMagicEffect(int iPlayerIndex_, int iBuffIndex_, bool bOnDie_ = false, bool bGoCoolTime = false, bool bOnNextStage_ = false);
	void ClearMagicEffect(int iPlayerIndex_, bool bOnDie_ = false, bool bOnNextStage_ = false);
	void ClearMagicEffect(bool bOnDie_ = false, bool bOnNextStage_ = false);
	void ClearBuffInstace(void);
	void ClearMonsterAllEffect(int iMonIndex);
	void ClearDeBuffEffect(int iPlayerIndex_, bool bIsCrashPlayer);

	void HiddenBuffParticle(int iBuffIndex_, bool bIsBufftimeStop_, bool bIsHiddenParticle_);

	void RenderHUD();

	void SetPlayerCartoon(int iPlayerIndex_, EGCCartoonTexture eCartoon);
	EGCCartoonTexture GetCartoon(int iPlayerIndex_);

	void SetPetCartoon(int iPlayerIndex_, EGCCartoonTexture eCartoon);
	EGCCartoonTexture GetPetCartoon(int iPlayerIndex_);

	void HitCreateBuffDamage(int iHit, int iHitted, D3DXVECTOR2* vPos = NULL, EGCCharAttackType eCharAtkType_ = CAHR_ATTACK_TYPE_PHYSIC);

	void DamageBuffDestroy(int iPlayerIndex_, int iDamage);
	int GetBuffLevel(int iPlayerIndex_, int iBuffIndex_);
	bool IsSpecialUsable(int iPlayerIndex_);
	// iPlayerIndex_의 Player에게 걸린 Buff의 remainTime을 돌려준다
	float IsMagicEffectTimeRet(int iPlayerIndex_, int iBuffIndex_);
	void SetMagicEffectTimeRet(int iPlayerIndex_, int iBuffIndex_, float fTime_);
	int GetNumTotalBuff(void) { return m_iEffectNum; }
private:

	KSafeArray<SLVERTEX, 4> V_Buff;
	KGCBuff**                    m_ppOriginalBuff;

	int m_iEffectNum;

	//KSafeArray<KGCBuffInstance*,MAX_PLAYER_NUM+MAX_MONSTER> m_mapPlayerBuff;
	std::map<int, std::map< int, KGCBuffInstance*> >   m_mapPlayerBuff;
	std::map<int, EGCCartoonTexture >   m_mapPlayerCartoon;
	std::map<int, EGCCartoonTexture >   m_mapPetCartoon;
public:
	int GetBuffInstanceCount(int iBuffEnum, bool bPlayer, int iMaxMonster = 0);
	int GetMonsterBuffInstanceCount(int iBuffEnum);
	int GetPlayerBuffInstanceCount(int iBuffEnum);

public:
	std::vector<std::wstring> m_strTempHudString;
	std::vector<std::wstring>& GetHudString(int iBuffEnum);
};

extern KGCBuffManager* g_pMagicEffect;
