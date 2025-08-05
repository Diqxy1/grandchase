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
// Comment : �ڽ��� ���� ��ź���������� 1���� ȭ���� ������ 5�߷� �����ش� ��������~
//			 Ƚ���� �ð� ������ �־�� �ϰ�, �ʻ�� ȭ����� �����.
//			 NO_PASS_DAMAG�� TRUE�̸� pShieldInstance->m_pOriginalDamage->m_iDamageHP �̳��� 0�� �Ǹ� ��������..
//			 ���⼱ NO_PASS_DAMAGE�� FALSE�� ����, �� �������� �� �� ũ���� �Ǿ����� üũ�ϴµ� ���...
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

			// Right to Left ��
			if (pDamageInstance->m_X + pDamageInstance->m_Scale_x * 0.5f < pOwnerArrow->m_X - pOwnerArrow->m_Scale_x * fRangeX)
				continue;

			// Left to Right ��
			if (pDamageInstance->m_X - pDamageInstance->m_Scale_x * 0.5f > pOwnerArrow->m_X + pOwnerArrow->m_Scale_x * fRangeX)
				continue;

			// 0.6 -> 0.8�� �� �÷����� --;

			// Top to Bottom ��
			if (pDamageInstance->m_Y + pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x * 0.8f
				< pOwnerArrow->m_Y - pOwnerArrow->m_Scale_x * pOwnerArrow->m_pOriginalDamage->m_Y_per_x * fRangeY)
				continue;

			// Bottom to Top ��
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

