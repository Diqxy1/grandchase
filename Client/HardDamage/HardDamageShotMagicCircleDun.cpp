#include "stdafx.h"
#include "..\DamageManager.h"
#include ".\HardDamageShotMagicCircleDun.h"






CHardDamageShotMagicCircleDun::CHardDamageShotMagicCircleDun(void)
{
	m_iCrashCount = 99999;

}

CHardDamageShotMagicCircleDun::~CHardDamageShotMagicCircleDun(void)
{
}

//====================================================================================
// 2009.02.13 : Jemitgge
// Comment : 자신이 만든 산탄마법진으로 1발의 화살을 날리면 5발로 돌려준다 으흐흐흐~
//			 횟수와 시간 제한이 있어야 하고, 필살기 화살들은 생깐다.
//			 NO_PASS_DAMAG가 TRUE이면 pShieldInstance->m_pOriginalDamage->m_iDamageHP 이놈이 0이 되면 없어진다..
//			 여기선 NO_PASS_DAMAGE를 FALSE로 쓰고, 이 데미지에 몇 번 크래쉬 되었는지 체크하는데 썼다...
void CHardDamageShotMagicCircleDun::FrameMove(CDamageInstance* pDamageInstance)
{
	CDamage::FrameMove(pDamageInstance);

	//if (pDamageInstance->m_Life == 1450)
	//{
		//pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.0f;
		//pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.0f;
	//}

	///fafaf
	///afafsaffaf/as/fa/fa

	if (m_iCrashCount <= 0)
	{
		pDamageInstance->m_Life = 1;
		return;
	}

	std::map< int, std::vector<CDamageInstance*> >::iterator itor;

	for (itor = g_MyD3D->m_pDamageManager->GetDamageInstancePool().begin(); itor != g_MyD3D->m_pDamageManager->GetDamageInstancePool().end(); ++itor)
	{
		std::vector<CDamageInstance*>::iterator vit;
		for (vit = itor->second.begin(); vit != itor->second.end(); vit++)
		{
			CDamageInstance* pOwnerArrow = *vit;

			if (pOwnerArrow->m_Who != pDamageInstance->m_Who)
				continue;

			float fRangeX = 0.0f;
			float fRangeY = 0.0f;


			if (pOwnerArrow->m_pOriginalDamage->m_bIsTotalRange)
			{
				fRangeX = 1.0f;
				fRangeY = 1.0f;
			}
			else
			{
				fRangeX = 0.5f;
				fRangeY = 0.4f;
			}

			// Right to Left 비교
			if (pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f < pOwnerArrow->m_X - pOwnerArrow->m_Scale_x * fRangeX)
				continue;

			// Left to Right 비교
			if (pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f > pOwnerArrow->m_X + pOwnerArrow->m_Scale_x * fRangeX)
				continue;

			// 0.6 -> 0.8로 쫌 올려보자 --;

			// Top to Bottom 비교
			if (pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.8f
				< pOwnerArrow->m_Y - pOwnerArrow->m_Scale_x * pOwnerArrow->m_pOriginalDamage->m_Y_per_x * fRangeY)
				continue;

			// Bottom to Top 비교
			if (pDamageInstance->m_Y - pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.8f
				> pOwnerArrow->m_Y + pOwnerArrow->m_Scale_x * pOwnerArrow->m_pOriginalDamage->m_Y_per_x * fRangeY)
				continue;

			if (pOwnerArrow->m_pOriginalDamage->m_bSpecialAttack)
				continue;

			if (!(pOwnerArrow->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE))
				continue;


			float angle;
			if (pOwnerArrow->m_IsRight)
			{
				angle = pOwnerArrow->m_Angle;
			}
			else
			{
				angle = -pOwnerArrow->m_Angle;
			}

			bool PlayerRight = g_MyD3D->MyPlayer[pOwnerArrow->m_Who]->GetIsRight();
			g_MyD3D->MyPlayer[pOwnerArrow->m_Who]->SetIsRight(pOwnerArrow->m_IsRight);

			CDamageInstance* pkDamage = NULL;
			for (int i = -2; i < 3; i++)
			{
				pkDamage = g_MyD3D->m_pDamageManager->AddWithAngle(pOwnerArrow->m_What, pOwnerArrow->m_Who, angle - static_cast<float>(i) * 0.125f);
				pkDamage->m_X = pDamageInstance->m_X;
				pkDamage->m_Y = pDamageInstance->m_Y;
				pkDamage->m_pOriginalDamage->m_bSpecialAttack = true;
				pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;
			}

			g_MyD3D->MyPlayer[pOwnerArrow->m_Who]->SetIsRight(PlayerRight);

			pOwnerArrow->m_Life = 1;
			pOwnerArrow->m_pOriginalDamage->m_bSpecialAttack = true;
			pkDamage->m_pOriginalDamage->m_nSpecialGrade = pDamageInstance->m_pOriginalDamage->m_nSpecialGrade;

			m_iCrashCount--;

			return;
		}



	}


}

void CHardDamageShotMagicCircleDun::Begin(CDamageInstance* pDamageInstance)
{
	CDamage::Begin(pDamageInstance);

	switch (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion)
	{
	case MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_90:
		pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.015f;
		break;
	case MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_45:
		pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.015f;
		pDamageInstance->m_pOriginalDamage->m_y_Speed = 0.015f;
		break;
	case MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_0:
		pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.015f;
		break;
	case MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_315:
		pDamageInstance->m_pOriginalDamage->m_x_Speed = 0.015f;
		pDamageInstance->m_pOriginalDamage->m_y_Speed = -0.015f;
		break;
	case MID_SKILL_LIRE_SHOT_MAGIC_CIRCLE_270:
		pDamageInstance->m_pOriginalDamage->m_y_Speed = -0.015f;
		break;
	}
}

