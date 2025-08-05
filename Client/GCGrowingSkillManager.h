#pragma once

#include "KNCSingleton.h"

class KLuaManager;
struct GCGrowingSkillInfo;

class GCGrowingSkillManager
{
	DeclareSingleton(GCGrowingSkillManager);

public:
	GCGrowingSkillManager(void);
	~GCGrowingSkillManager(void);

	void LoadFromLuaScript(char* strFilename_);
	void GetGrowingSkillInfo(int iCharType_, std::vector< GCGrowingSkillInfo>& vecGrowingSkillInfo_);

	float CountSkillUseMana(PLAYER* pPlayer_, float fMana_);
	float CountSkillCoolTime(PLAYER* pPlayer_, float fCoolTime_);
	float CountSkillUseHP(PLAYER* pPlayer_);
	float CountChangeDamageRate(PLAYER* pPlayer_, float fDamage_);
	float CountChangeDungeonDamageRate(PLAYER* pPlayer_, float fDamage_);
	float CountSkillCriticalRate(PLAYER* pPlayer_, float fRate_);
	float CountSkillCriticalDamage(PLAYER* pPlayer_, float fDamage_);

	void SetMagicEffectWhenCritical(PLAYER* pPlayer_);
	void SetMagicEffectWhenRage(PLAYER* pPlayer_);
	void SetMagicEffectWhenAttack(PLAYER* pPlayer_, int iCount_);

private:
	std::map< int, std::vector< GCGrowingSkillInfo > > m_mapGrowingSkillInfo;
};
DefSingletonInline(GCGrowingSkillManager);
