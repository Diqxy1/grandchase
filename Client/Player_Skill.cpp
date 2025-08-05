#include "stdafx.h"

#include "GCSKT.h"
#include "MyD3D.h"
#include "GCStateMachine.h"

#include "Replay.h"

// PlayerTemplate들을 로딩하기m 위해서 해당 Extra_Char_Num에 해당하는 스킬을 세팅해두는 함수.
void PLAYER::PrepairSkill(char CharNum)
{
	GAME_STATE emGameState = g_MyD3D->m_pStateMachine->GetState();
	EGCGameModeCategory emGameMode = SiKGCRoomManager()->GetGameModeCategory();

	if (emGameState != GS_GAME_LOADING && emGameState != GS_GAME &&
		emGameState != GS_TUTORIAL && g_MyD3D->MyReplay->m_iCurrState != RP_PREPARE
		|| emGameMode == GC_GMC_MINIGAME)
		return;

	//#ifdef _DEBUG
	//    UnitTest::DebugOutputReporter reporter;
	//    UnitTest::RunAllTests( reporter, UnitTest::Test::GetTestList(), "SkillResourceChangeTemplateSetting", 0 );
	//#endif
	m_setSkill.clear();

	if (IsLocalPlayer() && g_MyD3D->MyReplay->m_iCurrState != RP_PREPARE)
	{
		if (g_kGlobalValue.IsQuickSkillPlayer(CharNum) ||
			g_kGlobalValue.IsHybridPlayer(CharNum))
		{
			SiGCSKT()->GetSkillSet(CharNum % GC_CHAR_NUM, 0, m_setSkill);
			SiGCSKT()->GetSkillSet(CharNum % GC_CHAR_NUM, 1, m_setSkill);
			SiGCSKT()->GetSkillSet(CharNum % GC_CHAR_NUM, 2, m_setSkill);
			SiGCSKT()->GetSkillSet(CharNum % GC_CHAR_NUM, 3, m_setSkill);
			SiGCSKT()->GetSkillSet(CharNum % GC_CHAR_NUM, -1, m_setSkill);
		}
		else
		{
			SiGCSKT()->GetSkillSet(CharNum % GC_CHAR_NUM, CharNum / GC_CHAR_NUM, m_setSkill);
			SiGCSKT()->GetSkillSet(CharNum % GC_CHAR_NUM, -1, m_setSkill);
		}
	}
	else
	{
		m_kUserInfo.GetSkillSet(CharNum, m_setSkill);
	}

	SkillSet skSet;
	for (std::vector< EGCSkillTree >::size_type i = 0; i < m_setSkill.size(); i++)
	{
		skSet.insert(m_setSkill[i]);
	}

	SiGCSKT()->InsertUserSkillSet(m_kUserInfo.dwUID, skSet);

	// 무기체인지의 스킬 리소스 미리 로딩
	// 스왑할 무기가 있는지 확인
	if (m_mapSwapWeapon.end() != m_mapSwapWeapon.find(GetCurrentChar().iCharType))
	{
		std::vector< EGCSkillTree > setSkill;
		GCItem* pGCItem = NULL;
		if ((pGCItem = g_pItemMgr->GetItemData(m_mapSwapWeapon[GetCurrentChar().iCharType].second.first / 10)))
		{
			m_kUserInfo.GetSkillEquipInfo(pGCItem->iNeedChangeJobLevelForEquippingItem * GC_CHAR_NUM + GetCurrentChar().iCharType, setSkill);

			SkillSet skSet;
			for (std::vector< EGCSkillTree >::size_type i = 0; i < setSkill.size(); i++)
			{
				skSet.insert(setSkill[i]);
			}

			SiGCSKT()->InsertUserSkillSet(m_kUserInfo.dwUID, skSet);
		}
	}
}

bool PLAYER::CheckSkill(int iSkillID)
{
	return g_kGlobalValue.CheckTrainedSkill(this, (EGCSkillTree)iSkillID);
}

bool PLAYER::IsLearnedSkill(int iSkillID)
{
	return SiGCSKT()->IsLearnedSkill((EGCSkillTree)iSkillID);
}

bool PLAYER::IsForceDoFrameInfoProcMotion(UINT uiMotion_, int iFrame_)
{
	switch (uiMotion_)
	{
	case MID_ELESIS123_BERSERK:
	case MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY:
	case MID_SIEG1_SKILL_BERSERK:
	case MID_DIO4_PASSIVE_LV3_BERSERK:
	{
		switch (iFrame_)
		{
		case 26:
			return true;
		default:
			return false;
		}
		break;
	}
	case MID_ELESIS_JUMPATK:
	{
		switch (iFrame_)
		{
		case 3:
		case 5:
			return true;
		default:
			return false;
		}
		break;
	}
	case MID_SKILL_ELESIS1_MAGIC_DEFENSE:
	{
		switch (iFrame_)
		{
		case 18:
			return true;
		default:
			return false;
		}
		break;
	}
	case MID_SKILL_ELESIS1_JUMP_ATK:
	{
		switch (iFrame_)
		{
		case 9:
			return true;
		default:
			return false;
		}
		break;
	}
	default:
		return false;
	}
	return false;
}

void PLAYER::InitSkillAnimation(const std::vector< std::string >& vecAnim)
{
	std::vector< std::string >::const_iterator vit = vecAnim.begin();
	while (vit != vecAnim.end())
	{
		std::map< std::string, std::vector<KGCAnimManager::ItemAnim> >::iterator mit = m_mapPlayAnimation.find((*vit));
		if (mit == m_mapPlayAnimation.end())
		{
			std::vector<KGCAnimManager::ItemAnim> vecInsert;
			g_AnimManager.GetNewItemAnim((*vit).c_str(), CID_COMMON, vecInsert, 0);
			g_AnimManager.GetNewItemAnim((*vit).c_str(), CID_COMMON, vecInsert, 0);
			for (int i = 0; i < (int)vecInsert.size(); ++i)
			{
				vecInsert[i].animInst->SetShow(false);
			}
			m_mapPlayAnimation.insert(std::map< std::string, std::vector<KGCAnimManager::ItemAnim> >::value_type((*vit), vecInsert));
		}
		++vit;
	}
}

void PLAYER::BeginAnim(char* strAnimName)
{
	std::map< std::string, std::vector<KGCAnimManager::ItemAnim> >::iterator mit = m_mapPlayAnimation.find(strAnimName);
	if (mit != m_mapPlayAnimation.end())
	{
		std::vector<KGCAnimManager::ItemAnim>& vecAnim = mit->second;
		for (int i = 0; i < (int)vecAnim.size(); ++i)
		{
			vecAnim[i].animInst->SetShow(true);
		}
	}
}

void PLAYER::EndAnim(char* strAnimName)
{
	std::map< std::string, std::vector<KGCAnimManager::ItemAnim> >::iterator mit = m_mapPlayAnimation.find(strAnimName);
	if (mit != m_mapPlayAnimation.end())
	{
		std::vector<KGCAnimManager::ItemAnim>& vecAnim = mit->second;
		for (int i = 0; i < (int)vecAnim.size(); ++i)
		{
			vecAnim[i].animInst->SetShow(false);
		}
	}
}

bool PLAYER::SkillAloowMotion(int iCharType, UINT uiMotion_)
{
	switch (iCharType)
	{
	case GC_CHAR_DIO:
	{
		switch (uiMotion_)
		{
		case MID_DIO1_DASH_ATK1:
		case MID_DIO1_JUMP_ATK:
		case MID_DIO1_ATK1:
		case MID_DIO1_ATK2:
		case MID_DIO1_ATK3:
		case MID_DIO1_ATK4:
		case MID_DIO1_ATK5:
		case MID_DIO1_CRITICAL_ATK:
		case MID_DIO1_DOUBLE_ATK:
		case MID_DIO1_STAND_UP_ATK:
		case MID_DIO1_DASH_ATK2:
		case MID_DIO2_ATK1:
		case MID_DIO2_ATK2:
		case MID_DIO2_ATK3:
		case MID_DIO2_ATK4:
		case MID_DIO2_COMBO_PLUS1:
		case MID_DIO2_COMBO_PLUS2:
		case MID_DIO3_COMBO_ATK1:
		case MID_DIO3_COMBO_ATK2:
		case MID_DIO3_COMBO_ATK3:
		case MID_DIO3_COMBO_ATK4:
		case MID_DIO3_DOUBLE_ATK:
		case MID_DIO3_DASH_ATK:
		case MID_DIO3_CRITICAL_ATK:
		case MID_DIO3_COMBO_ATK1_SPEED:
		case MID_DIO3_COMBO_ATK2_SPEED:
		case MID_DIO3_COMBO_ATK3_SPEED:
		case MID_DIO3_COMBO_ATK4_SPEED:
		case MID_DIO1_ATK1_SPEED:
		case MID_DIO1_ATK2_SPEED:
		case MID_DIO1_ATK3_SPEED:
		case MID_DIO1_ATK4_SPEED:
		case MID_DIO1_ATK5_SPEED:
		case MID_DIO2_ATK1_SPEED:
		case MID_DIO2_ATK2_SPEED:
		case MID_DIO2_ATK3_SPEED:
		case MID_DIO2_ATK4_SPEED:
			return true;
		}
	}
	break;
	default:
		return false;
	}
	return false;
}