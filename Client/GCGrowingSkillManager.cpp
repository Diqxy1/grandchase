#include "GCGrowingSkillManager.h"

ImplementSingleton(GCGrowingSkillManager)

GCGrowingSkillManager::GCGrowingSkillManager(void)
{
}

GCGrowingSkillManager::~GCGrowingSkillManager(void)
{
}

void GCGrowingSkillManager::LoadFromLuaScript(char* strFilename_)
{
	KLuaManager luaMgr;
	if (!GCFUNC::LoadLuaScript(luaMgr, "Enum.stg"))
	{
		return;
	}
	if (!GCFUNC::LoadLuaScript(luaMgr, "BuffEnum.stg"))
	{
		return;
	}
	if (!GCFUNC::LoadLuaScript(luaMgr, "MotionEnum.stg"))
	{
		return;
	}
	if (!GCFUNC::LoadLuaScript(luaMgr, strFilename_))
	{
		return;
	}

	m_mapGrowingSkillInfo.clear();

	for (int i = 0; i < GC_CHAR_NUM; i++)
	{
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		const char* strTableName;
		std::wstringstream wstrmTableName;
		wstrmTableName << L"Char" << i;
		strTableName = GCUTIL_STR::GCStrWideToChar(wstrmTableName.str().c_str());

		LUA_BEGIN_TABLE_RAII(strTableName)
		{
			for (int j = 1; luaMgr.BeginTable(j) == S_OK; ++j)
			{
				GCGrowingSkillInfo kGrowingSkillInfo;
				luaMgr.GetValue("SkillID", kGrowingSkillInfo.m_iSkillID);
				luaMgr.GetValue("Condition", kGrowingSkillInfo.m_iCondition);
				luaMgr.GetValue("CoolTime", kGrowingSkillInfo.m_fCoolTime);
				luaMgr.GetValue("UniqueCoolTime", kGrowingSkillInfo.m_fUniqueCoolTime);
				luaMgr.GetValue("SpecialCoolTime", kGrowingSkillInfo.m_fSpecialCoolTime);
				luaMgr.GetValue("MP", kGrowingSkillInfo.m_fMP);
				luaMgr.GetValue("HP", kGrowingSkillInfo.m_fHP);
				luaMgr.GetValue("DamageRate", kGrowingSkillInfo.m_fDamageRate);
				luaMgr.GetValue("DungeonDamageRate", kGrowingSkillInfo.m_fDungeonDamageRate);
				luaMgr.GetValue("CriticalRate", kGrowingSkillInfo.m_fCriticalRate);
				luaMgr.GetValue("CriticalDamage", kGrowingSkillInfo.m_fCriticalDamage);
				luaMgr.GetValue("BuffIndex", kGrowingSkillInfo.m_iBuffIndex);
				luaMgr.GetValue("BuffTime", kGrowingSkillInfo.m_fBuffTime);
				luaMgr.GetValue("Value", kGrowingSkillInfo.m_fValue);

				vecGrowingSkillInfo.push_back(kGrowingSkillInfo);

				luaMgr.EndTable();
			}
		}

		m_mapGrowingSkillInfo.insert(std::make_pair(i, vecGrowingSkillInfo));
	}
}

// 이 밑은 좀 하드한 함수들임... 성장개편에 사용할 조건들이 정해진 기준으로 사용할 수 없는 것들이 대부분이라서 해당 매니저 클래스에서 하드한 함수들 만들어 쓰는 방식을 택함...

void GCGrowingSkillManager::GetGrowingSkillInfo(int iCharType_, std::vector< GCGrowingSkillInfo>& vecGrowingSkillInfo_)
{
	std::map< int, std::vector< GCGrowingSkillInfo> >::iterator mit = m_mapGrowingSkillInfo.find(iCharType_);

	if (mit != m_mapGrowingSkillInfo.end()) {
		vecGrowingSkillInfo_ = mit->second;
	}
}

float GCGrowingSkillManager::CountSkillUseMana(PLAYER* pPlayer_, float fMana_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_fMP != 0.f) {
				if (pPlayer_->CheckSkill(vit->m_iSkillID)) {
					fMana_ += (fMana_ * vit->m_fMP);
				}
			}
		}
	}
	return fMana_;
}

float GCGrowingSkillManager::CountChangeDungeonDamageRate(PLAYER* pPlayer_, float fDamage_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_fDungeonDamageRate != 0.f) {
				if (pPlayer_->CheckSkill(vit->m_iSkillID)) {
					fDamage_ += (fDamage_ * vit->m_fDungeonDamageRate);
				}
			}
		}
	}
	return fDamage_;
}

float GCGrowingSkillManager::CountSkillCoolTime(PLAYER* pPlayer_, float fCoolTime_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_fCoolTime != 0.f) {
				if (pPlayer_->CheckSkill(vit->m_iSkillID)) {
					fCoolTime_ += (fCoolTime_ * vit->m_fCoolTime);
				}
			}
		}
	}
	return fCoolTime_;
}

float GCGrowingSkillManager::CountSkillUseHP(PLAYER* pPlayer_)
{
	float fChangeHP = 0.f;
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_fHP != 0.f) {
				if (pPlayer_->CheckSkill(vit->m_iSkillID)) {
					fChangeHP += (pPlayer_->GetHP() * vit->m_fHP);
				}
			}
		}

		if (fChangeHP != 0.f) {
			LTP_CHANGE_PLAYER_HP kPacket;
			kPacket.m_dwUserUID = pPlayer_->m_kUserInfo.dwUID;
			kPacket.m_fChangeHP = pPlayer_->GetHP() + fChangeHP;
			g_LatencyTransfer.PushPacket(&kPacket);
		}
	}
	return fChangeHP;
}

float GCGrowingSkillManager::CountChangeDamageRate(PLAYER* pPlayer_, float fDamage_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_fDamageRate != 0.f) {
				if (pPlayer_->CheckSkill(vit->m_iSkillID)) {
					fDamage_ += (fDamage_ * vit->m_fDamageRate);
				}
			}
		}
	}
	return fDamage_;
}

float GCGrowingSkillManager::CountSkillCriticalRate(PLAYER* pPlayer_, float fRate_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_fCriticalRate != 0.f) {
				if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM || g_MyD3D->m_pStateMachine->GetState() == GS_GAME) {
					if (pPlayer_->IsEquipSkill(static_cast<EGCSkillTree>(vit->m_iSkillID))) {
						fRate_ += vit->m_fCriticalRate;
					}
				}
				else {
					if (g_MyD3D->m_TempPlayer.IsEquipSkill(static_cast<EGCSkillTree>(vit->m_iSkillID))) {
						fRate_ += vit->m_fCriticalRate;
					}
				}
			}
		}
	}

	if (fRate_ < 0.f) {
		fRate_ = 0.f;
	}

	return fRate_;
}

float GCGrowingSkillManager::CountSkillCriticalDamage(PLAYER* pPlayer_, float fDamage_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_fCriticalDamage != 0.f) {
				if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM || g_MyD3D->m_pStateMachine->GetState() == GS_GAME) {
					if (pPlayer_->IsEquipSkill(static_cast<EGCSkillTree>(vit->m_iSkillID))) {
						fDamage_ += vit->m_fCriticalDamage;
					}
				}
				else {
					if (g_MyD3D->m_TempPlayer.IsEquipSkill(static_cast<EGCSkillTree>(vit->m_iSkillID))) {
						fDamage_ += vit->m_fCriticalDamage;
					}
				}
			}
		}
	}

	if (fDamage_ < 0.f) {
		fDamage_ = 0.f;
	}

	return fDamage_;
}

// 크리티컬이 발동 되었을 시 버프 걸어주는 함수.
void GCGrowingSkillManager::SetMagicEffectWhenCritical(PLAYER* pPlayer_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_iCondition == GCGrowingSkillInfo::GSC_WHEN_CRITICAL && vit->m_iBuffIndex != -1) {
				if (pPlayer_->IsEquipSkill(static_cast<EGCSkillTree>(vit->m_iSkillID)) && pPlayer_->IsMagicEffect(vit->m_iBuffIndex) == false) {
					pPlayer_->SetMagicEffect(vit->m_iBuffIndex, vit->m_fBuffTime);
				}
			}
		}
	}
}

void GCGrowingSkillManager::SetMagicEffectWhenRage(PLAYER* pPlayer_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_iCondition == GCGrowingSkillInfo::GSC_RAGE_MP_RECOVERY && vit->m_iBuffIndex != -1) {
				if (pPlayer_->CheckSkill(vit->m_iSkillID)) {
					if (pPlayer_->GetPlayerMaxHP() * vit->m_fValue >= pPlayer_->GetHP()) {
						if (pPlayer_->IsMagicEffect(vit->m_iBuffIndex) == false && (PS_DEAD != pPlayer_->GetPlayerState())) {
							pPlayer_->SetMagicEffect(vit->m_iBuffIndex, vit->m_fBuffTime);
						}
					}
					else {
						if (pPlayer_->IsMagicEffect(vit->m_iBuffIndex)) {
							pPlayer_->ClearMagicEffect(vit->m_iBuffIndex);
						}
					}
				}
			}
		}
	}
}

void GCGrowingSkillManager::SetMagicEffectWhenAttack(PLAYER* pPlayer_, int iCount_)
{
	if (pPlayer_) {
		std::vector< GCGrowingSkillInfo > vecGrowingSkillInfo;
		SiGCGrowingSkillManager()->GetGrowingSkillInfo(pPlayer_->GetCurrentChar().iCharType, vecGrowingSkillInfo);

		for (std::vector< GCGrowingSkillInfo >::iterator vit = vecGrowingSkillInfo.begin(); vit != vecGrowingSkillInfo.end(); ++vit) {
			if (vit->m_iCondition == GCGrowingSkillInfo::GSC_WHEN_ATTACK && vit->m_iBuffIndex != -1) {
				if (pPlayer_->CheckSkill(vit->m_iSkillID) && iCount_ >= vit->m_fValue) {
					pPlayer_->SetMagicEffect(vit->m_iBuffIndex, vit->m_fBuffTime);
				}
			}
		}
	}
}