#include "stdafx.h"
#include ".\KGCBuffAnkOfResurrection.h"
#include "MyD3D.h"
#include "GCSKT.h"
#include "../GCUI/KGCInGameMessage.h"

KGCBuffAnkOfResurrection::KGCBuffAnkOfResurrection(void)
	:bLv2(false)
{
}

KGCBuffAnkOfResurrection::~KGCBuffAnkOfResurrection(void)
{
}

bool KGCBuffAnkOfResurrection::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

		if (pPlayer->GetHP() <= 0.0f && (pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) || (pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP))) && pPlayer->cFrame > 19)
		{
			if (pPlayer->IsMagicEffect(EGC_EFFECT_ANKH_OF_RESURRECTION) && bLv2) {
				pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.2f);
				pPlayer->SetSuper("resurrection", 20, 0);

				if (bLv2)
				{
					pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.5f);
				}
				pBuffInst_->m_fRemainTime = 0.1f;
			}
			else if (pPlayer->IsMagicEffect(EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP)) {
				pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.1f * (float)(pBuffInst_->m_iLevel + 1));
				pPlayer->SetSuper("resurrection", 20, 0);
				pBuffInst_->m_fRemainTime = 0.1f;
			}
			else {
				pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.2f);
				pPlayer->SetSuper("resurrection", 20, 0);
				pBuffInst_->m_fRemainTime = 0.1f;
			}

			pPlayer->ClearMagicEffect(EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP, false, true);
			pPlayer->ClearMagicEffect(EGC_EFFECT_ANKH_OF_RESURRECTION);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_ANKH_OF_RESURRECTION, false, true);
		}

		return true;
	}
	return false;
}

bool KGCBuffAnkOfResurrection::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		bLv2 = false;
		if (pBuffInst_->m_fRemainTime > 50.0f)
		{
			bLv2 = true;
		}

		return true;
	}

	return false;
}

KGCBuffDioLifeKeep::KGCBuffDioLifeKeep(void)
{
}

KGCBuffDioLifeKeep::~KGCBuffDioLifeKeep(void)
{
}

bool KGCBuffDioLifeKeep::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (iPlayerIndex_ >= MAX_PLAYER_NUM) {
			return false;
		}

		PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

		if (pPlayer->GetHP() <= 0.0f && (pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) || (pPlayer->uiMotion == pPlayer->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP))) && pPlayer->cFrame > 19)
		{
			pPlayer->SetHP(pPlayer->m_fAbility[ABILITY_HP] * 0.1f * (float)(pBuffInst_->m_iLevel + 1));
			pPlayer->SetSuper("resurrection", 20, 0);
			pBuffInst_->m_fRemainTime = 0.1f;
#if defined( MATCH_BALLENCE_121217 )
			if (pPlayer->GetCurrentChar().iCharType == GC_CHAR_DIO &&
				SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON &&
				SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DOTA) {
				pPlayer->SetMagicEffect(EGC_EFFECT_DIO_LIFEKEEP_LIMIT_BUFF, 999999.f);
			}

			g_pMagicEffect->ClearMagicEffect(EGC_EFFECT_ANKH_OF_RESURRECTION);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP, false, true);
			g_pMagicEffect->ClearMagicEffect(EGC_EFFECT_DIO_LIFEKEEP);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_DIO_LIFEKEEP, false, true);
			g_pMagicEffect->ClearMagicEffect(EGC_EFFECT_DIO4_LIFE_KEEP_MISSILE_DMG_RESIST);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_DIO4_LIFE_KEEP_MISSILE_DMG_RESIST, false, true);

			g_pMagicEffect->ClearMagicEffect(EGC_EFFECT_ANKH_OF_RESURRECTION);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_ANKH_OF_RESURRECTION, false, true);
#else
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP, false, true);
			pPlayer->ClearMagicEffect(EGC_EFFECT_DIO_LIFEKEEP);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_DIO_LIFEKEEP, false, true);
			g_pMagicEffect->ClearMagicEffect(EGC_EFFECT_DIO4_LIFE_KEEP_MISSILE_DMG_RESIST);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_DIO4_LIFE_KEEP_MISSILE_DMG_RESIST, false, true);

			pPlayer->ClearMagicEffect(EGC_EFFECT_ANKH_OF_RESURRECTION);
			g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_ANKH_OF_RESURRECTION, false, true);
#endif
		}

		return true;
	}
	return false;
}

bool KGCBuffZeroAmidSuper::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		{
			PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];

			if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK) {
				if (pPlayer->IsRemovePartsItem(HEAD))
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleBaseMeshStatic(true);
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleSecondBaseMeshAll(false);
				}
				else
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleBaseMeshStatic(false);
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleSecondBaseMesh("DOG", true);
					m_bFirstCheckFatalDOG = false;

					// 펫 변신중이면 다음에 걸어줘야 할 버프 있는지 확인
					std::pair< int, float > prNextTransformPet = g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTransformPetBuff();
					g_MyD3D->MyPlayer[iPlayerIndex_]->SetNextTransformPetBuff(prNextTransformPet.first, prNextTransformPet.second);
				}
			}
		}
		return true;
	}

	return false;
}

bool KGCBuffZeroAmidSuper::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_))
	{
		if (pBuffInst_->m_iBuffEnum != EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK) {
			if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_ZERO_UNIQUE_CARTOON) {
				if (g_MyD3D->IsPlayerIndex(iPlayerIndex_)) {
					g_MyD3D->MyPlayer[iPlayerIndex_]->SetTexture("zero_black.dds");
				}
			}
			else {
				if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->Super = 255;
				}
			}
		}
		else {
			PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];
			if (pPlayer->IsRemovePartsItem(HEAD))
			{
				return true;
			}
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsFatal() && g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer()) {
				if (g_MyD3D->MyPlayer[iPlayerIndex_]->CheckSkill(SID_ZERO4_DOMINATION_OF_GRANDARK_UPGRADE)) {
					if (m_bFirstCheckFatalDOG == false) {
						float fRemainTime = g_pMagicEffect->IsMagicEffectTimeRet(iPlayerIndex_, EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK);
						if (fRemainTime > 2.f) {
							m_bFirstCheckFatalDOG = true;
							g_MyD3D->MyPlayer[iPlayerIndex_]->SetCurMarbleNum(g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurMarbleNum() / 2.f);
							g_pMagicEffect->SetMagicEffectTimeRet(iPlayerIndex_, EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK, fRemainTime / 2.f);
						}
						else {
							g_MyD3D->MyPlayer[iPlayerIndex_]->ClearMagicEffect(EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK);
						}
					}
				}
				else {
					g_MyD3D->MyPlayer[iPlayerIndex_]->ClearMagicEffect(EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK);
					m_bFirstCheckFatalDOG = false;
				}
			}
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurrentChar().iPromotionLevel > 2) {
				g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("WEAPON", false);
				g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("MERGE_WEAPON", false);

				if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsToggleExtraMesh("GRANDARK_2ND") ||
					g_MyD3D->MyPlayer[iPlayerIndex_]->IsToggleExtraMesh("GRANDARK_2ND_1") ||
					g_MyD3D->MyPlayer[iPlayerIndex_]->IsToggleExtraMesh("GRANDARK_2ND_2") ||
					g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_ZERO3_OS_COMBO_UNIQUE_ATK) {
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("DOG_GRANDARK", false);
				}
				else {
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("DOG_GRANDARK", true);
				}

				if (g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_ZERO1_PRICKLEGUSH && g_MyD3D->MyPlayer[iPlayerIndex_]->GetFrameIndex() >= 45) {
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("DOG_GRANDARK", false);
				}

				g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_ZERO_DOG);
			}
		}
		return true;
	}
	return false;
}

bool KGCBuffZeroAmidSuper::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK) {
			if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetCurMarbleNum(0.f);

				if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsSpecialSkillMotion() == false) {
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleBaseMeshStatic(true);
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleSecondBaseMeshAll(false);
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("GRANDARK_2ND", false);
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("GRANDARK_2ND_1", false);
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("GRANDARK_2ND_2", false);
					g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("DOG_GRANDARK", false);

					if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsToggleExtraMesh("WEAPON")) {
						g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("WEAPON", true);
						g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("MERGE_WEAPON", false);
					}
					else {
						g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("WEAPON", false);
						g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh("MERGE_WEAPON", true);
					}
				}

				if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsMagicEffect(EGC_EFFECT_ZERO2_AMID_BUFF) == false) {
					g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_NORMAL);
				}

				m_bFirstCheckFatalDOG = false;
			}
		}
		else if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_ZERO_UNIQUE_CARTOON) {
			if (g_MyD3D->IsPlayerIndex(iPlayerIndex_)) {
				if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsMagicEffect(EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK)) {
					if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurrentChar().iPromotionLevel > 2) {
						g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_ZERO_DOG);
					}
					else {
						g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_INSECTIVOROUS);
					}
				}
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetTexture();
			}
		}
		else {
			if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->Super = 0;
				if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsMagicEffect(EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK)) {
					if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurrentChar().iPromotionLevel > 2) {
						g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_ZERO_DOG);
					}
					else {
						g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_INSECTIVOROUS);
					}
				}
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetTexture();
			}
		}
		return true;
	}
	return false;
}

bool KGCBuffZeroDSReflection::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID))
	{
		if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		{
			if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_ZERO_DS_REFLECTION_REINFORCEMENT) {
				g_MyD3D->MyPlayer[iPlayerIndex_]->AddDamage(DT_ZERO3_DEFENCE_REFLECTION, 0);
			}
		}
		return true;
	}

	return false;
}

bool KGCBuffZeroDSReflection::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_))
	{
		if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		{
			if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_ZERO_DS_REFLECTION_REINFORCEMENT) {
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetDamageLife(DT_ZERO3_DEFENCE_REFLECTION, 1);
			}
			else {
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetDamageLife(DT_ZERO2_DEFENCE_REFLECTION, 1);
			}
		}
		return true;
	}
	return false;
}

bool KGCBuffLeyHauntCurse::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ /* = -1 */, bool bIsCrashPlayer /* = true */, DWORD dwGivePlayerUID /* = 0 */)
{
	if (KGCBuff::Init(iPlayerIndex_, pBuffInst_, fBuffTime, iIndex_, bIsCrashPlayer, dwGivePlayerUID)) {
		if (g_MyD3D->IsPlayerIndex(iPlayerIndex_) && g_MyD3D->IsPlayerIndex(iIndex_)) {
			m_iWho = iIndex_;
			if (g_MyD3D->IsSameTeam(iPlayerIndex_, g_MyD3D->GetMyPlayer()->m_iPlayerIndex)) {
				KGCInGameMessage *pMessage = g_pkGameOverlayUI->GetInGameMessage();
				KInGameMessageAttribute kMessage;
				kMessage.iStringID = STR_ID_HAUNT_CURSE_EMERGENCY_MSG;
				kMessage.iShowFrame = static_cast<int>(fBuffTime) * GC_FPS_LIMIT;
				pMessage->SetMessage(&kMessage);
				g_pkGameOverlayUI->SetLocalTimer(fBuffTime);
			}
		}
		return true;
	}

	return false;
}

bool KGCBuffLeyHauntCurse::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (KGCBuff::FrameMove(iPlayerIndex_, pBuffInst_)) {
		return true;
	}
	return false;
}

bool KGCBuffLeyHauntCurse::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (KGCBuff::Destroy(iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_)) {
		if (g_MyD3D->IsPlayerIndex(iPlayerIndex_)) {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(g_kMonsterManager.GetSummonMonsterIndex(g_MyD3D->MyPlayer[m_iWho]->m_kUserInfo.dwUID, MON_SUMMON_HAUNT));
			if (pMonster != NULL) {
				if (pMonster->GetHP() > 0.0) {
					float fHP = 0.f;
					float fDecreaseHP = 0.f;
					if (pBuffInst_->m_iBuffEnum == EGC_EFFECT_LEY3_HAUNT_CURSE04_LV2) {
						fHP = g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() * 0.7f;
					}
					else {
						fHP = g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() * 0.8f;
					}
					fDecreaseHP = g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() - fHP;

					g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(fHP);
					g_MyD3D->MyHead->Add_HP_Changer(iPlayerIndex_, iPlayerIndex_, fDecreaseHP, false, "", 0);

					if (g_MyD3D->MyPlayer[iPlayerIndex_]->MetamorphosisForm == FORM_NORMAL) {
						g_MyD3D->MyPlayer[iPlayerIndex_]->Direct_Motion_Input(MID_COMMON_LEY3_HAUNT_CURSE4_DAMAGE);
					}
					else {
						g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticleNoDirectionPosWithTrace("Ley03_DeathScythe_01", 0, 0.0f, 0.0f, 5.0f);
						g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticleNoDirectionPosWithTrace("Ley03_DeathScythe_02", 0, 0.0f, 0.0f, 5.0f);
						g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticleNoDirectionPosWithTrace("Ley03_DeathScythe_03", 0, 0.0f, 0.0f, 5.0f);
						g_MyD3D->MyPlayer[iPlayerIndex_]->AddParticleNoDirectionPosWithTrace("Ley03_DeathScythe_04", 0, 0.0f, 0.0f, 5.0f);
					}
				}
			}
		}

		return true;
	}
	return false;
}