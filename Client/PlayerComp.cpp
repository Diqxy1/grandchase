#include "stdafx.h"
#include "PlayerComp.h"

#include "PetAI.h"

#include "./Square/GCSquarePeople.h"
#include "KGCPlayerCommon.h"
//

ItemEffectParam::ItemEffectParam(PLAYER* pPlayer, std::vector< int >& vecItemInfo_)
	: charID((CHAR_IDENTY)((char)pPlayer->Extra_Char_Num))
	, m_Elemental(pPlayer->m_Elemental)
	, vecItemInfo(vecItemInfo_)
	, m_StrongLevel(pPlayer->m_StrongLevel)
	, m_ItemAnim(pPlayer->m_ItemAnim)
	, m_DamageAnim(pPlayer->m_DamageAnim)
	, m_SkillAnim(pPlayer->m_SkillAnim)
	, m_vecItemEff(pPlayer->m_vecItemEff)
	, m_vecDamageEff(pPlayer->m_vecDamageEff)
	, m_vecSkillEff(pPlayer->m_vecSkillEff)
	, m_mapOverlappedPair(pPlayer->m_mapOverlappedPair)
	, m_iPlayerIndex(pPlayer->m_iPlayerIndex)
	, m_vecItemSlot(pPlayer->m_kUserInfo.kQuickSlot.m_vecItemSlot)
	, m_vecPetAiID(pPlayer->m_vecPetAiID)
	, m_bSquare(false)
{
}

ItemEffectParam::ItemEffectParam(PLAYER* pPlayer, TagCharacterInfo* pTagInfo, std::vector< int >& vecItemInfo_)
	: charID((CHAR_IDENTY)(pTagInfo->Extra_Char_Num))
	, m_Elemental(pTagInfo->m_Elemental)
	, vecItemInfo(vecItemInfo_)
	, m_StrongLevel(pTagInfo->m_StrongLevel)
	, m_ItemAnim(pTagInfo->m_ItemAnim)
	, m_DamageAnim(pTagInfo->m_DamageAnim)
	, m_SkillAnim(pTagInfo->m_SkillAnim)
	, m_vecItemEff(pTagInfo->m_vecItemEff)
	, m_vecDamageEff(pTagInfo->m_vecDamageEff)
	, m_vecSkillEff(pTagInfo->m_vecSkillEff)
	, m_mapOverlappedPair(pTagInfo->m_mapOverlappedPair)
	, m_iPlayerIndex(pPlayer->m_iPlayerIndex)
	, m_vecItemSlot(pPlayer->m_kUserInfo.kQuickSlot.m_vecItemSlot)
	, m_vecPetAiID(pTagInfo->m_vecPetAiID)
	, m_bSquare(false)
{
}

ItemEffectParam::ItemEffectParam(GCSquarePeople* pPeople, std::vector< int >& vecItemInfo_)
	: charID((CHAR_IDENTY)(pPeople->GetExtraCharNum()))
	, m_Elemental(pPeople->GetElemental())
	, vecItemInfo(vecItemInfo_)
	, m_StrongLevel(pPeople->GetStrongLevel())
	, m_ItemAnim(pPeople->GetItemAnim())
	, m_DamageAnim(pPeople->GetDamageAnim())
	, m_SkillAnim(pPeople->GetSkillAnim())
	, m_vecItemEff(pPeople->GetItemEff())
	, m_vecDamageEff(pPeople->GetDamageEff())
	, m_vecSkillEff(pPeople->GetSkillEff())
	, m_mapOverlappedPair(pPeople->GetOvelappedPair())
	, m_iPlayerIndex((int)pPeople->GetUID())
	, m_vecPetAiID(pPeople->GetPetAiID())
	, m_bSquare(true)
	, m_vecItemSlotSquare(pPeople->GetPeopleInfo().m_vecQuickSlot)
{
}

void UpdatePositionDesc(PositionDesc& posDesc, bool bIsLocalPlayer, int MetamorphosisForm)
{
	if (bIsLocalPlayer)
	{
		for (unsigned char i = 0; i < 4 + (LATENCY + SHOWLATENCY); i++)
		{
			posDesc.rightHandLatancy[i * 2] = posDesc.rightHandLatancy[(i + 1) * 2];
			posDesc.rightHandLatancy[i * 2 + 1] = posDesc.rightHandLatancy[(i + 1) * 2 + 1];

			posDesc.leftHandLatancy[i * 2] = posDesc.leftHandLatancy[(i + 1) * 2];
			posDesc.leftHandLatancy[i * 2 + 1] = posDesc.leftHandLatancy[(i + 1) * 2 + 1];

			posDesc.headLatancy[i * 2] = posDesc.headLatancy[(i + 1) * 2];
			posDesc.headLatancy[i * 2 + 1] = posDesc.headLatancy[(i + 1) * 2 + 1];

			posDesc.rightShoulderLatancy[i * 2] = posDesc.rightShoulderLatancy[(i + 1) * 2];
			posDesc.rightShoulderLatancy[i * 2 + 1] = posDesc.rightShoulderLatancy[(i + 1) * 2 + 1];

			posDesc.leftShoulderLatancy[i * 2] = posDesc.leftShoulderLatancy[(i + 1) * 2];
			posDesc.leftShoulderLatancy[i * 2 + 1] = posDesc.leftShoulderLatancy[(i + 1) * 2 + 1];
		}

		SETVECTOR3(posDesc.rightHandLatancy[8 + (LATENCY + SHOWLATENCY) * 2], posDesc.rightHandTip.x, posDesc.rightHandTip.y, posDesc.rightHandTip.z);
		SETVECTOR3(posDesc.rightHandLatancy[9 + (LATENCY + SHOWLATENCY) * 2], posDesc.rightHandBody.x, posDesc.rightHandBody.y, posDesc.rightHandBody.z);

		SETVECTOR3(posDesc.leftHandLatancy[8 + (LATENCY + SHOWLATENCY) * 2], posDesc.leftHandTip.x, posDesc.leftHandTip.y, posDesc.leftHandTip.z);
		SETVECTOR3(posDesc.leftHandLatancy[9 + (LATENCY + SHOWLATENCY) * 2], posDesc.leftHandBody.x, posDesc.leftHandBody.y, posDesc.leftHandBody.z);

		SETVECTOR3(posDesc.headLatancy[8 + (LATENCY + SHOWLATENCY) * 2], posDesc.HeadBombSpark.x, posDesc.HeadBombSpark.y, posDesc.HeadBombSpark.z);
		SETVECTOR3(posDesc.headLatancy[9 + (LATENCY + SHOWLATENCY) * 2], posDesc.HeadBombSpark.x, posDesc.HeadBombSpark.y, posDesc.HeadBombSpark.z);

		SETVECTOR3(posDesc.rightShoulderLatancy[8 + (LATENCY + SHOWLATENCY) * 2], posDesc.rightShoulder.x, posDesc.rightShoulder.y, posDesc.rightShoulder.z);
		SETVECTOR3(posDesc.rightShoulderLatancy[9 + (LATENCY + SHOWLATENCY) * 2], posDesc.rightShoulder.x, posDesc.rightShoulder.y, posDesc.rightShoulder.z);

		SETVECTOR3(posDesc.leftShoulderLatancy[8 + (LATENCY + SHOWLATENCY) * 2], posDesc.leftShoulder.x, posDesc.leftShoulder.y, posDesc.leftShoulder.z);
		SETVECTOR3(posDesc.leftShoulderLatancy[9 + (LATENCY + SHOWLATENCY) * 2], posDesc.leftShoulder.x, posDesc.leftShoulder.y, posDesc.leftShoulder.z);
	}
	else
	{
		// [2/5/2008] breadceo. 응?
		//if( CheckHiding() )
		//    return;
		for (unsigned char i = 0; i < 5; i++)
		{
			posDesc.rightHandLatancy[i * 2] = posDesc.rightHandLatancy[(i + 1) * 2];
			posDesc.rightHandLatancy[i * 2 + 1] = posDesc.rightHandLatancy[(i + 1) * 2 + 1];

			posDesc.leftHandLatancy[i * 2] = posDesc.leftHandLatancy[(i + 1) * 2];
			posDesc.leftHandLatancy[i * 2 + 1] = posDesc.leftHandLatancy[(i + 1) * 2 + 1];

			posDesc.headLatancy[i * 2] = posDesc.headLatancy[(i + 1) * 2];
			posDesc.headLatancy[i * 2 + 1] = posDesc.headLatancy[(i + 1) * 2 + 1];

			posDesc.rightShoulderLatancy[i * 2] = posDesc.rightShoulderLatancy[(i + 1) * 2];
			posDesc.rightShoulderLatancy[i * 2 + 1] = posDesc.rightShoulderLatancy[(i + 1) * 2 + 1];

			posDesc.leftShoulderLatancy[i * 2] = posDesc.leftShoulderLatancy[(i + 1) * 2];
			posDesc.leftShoulderLatancy[i * 2 + 1] = posDesc.leftShoulderLatancy[(i + 1) * 2 + 1];
		}

		SETVECTOR3(posDesc.rightHandLatancy[10], posDesc.rightHandTip.x, posDesc.rightHandTip.y, posDesc.rightHandTip.z);
		SETVECTOR3(posDesc.rightHandLatancy[11], posDesc.rightHandBody.x, posDesc.rightHandBody.y, posDesc.rightHandBody.z);

		SETVECTOR3(posDesc.leftHandLatancy[10], posDesc.leftHandTip.x, posDesc.leftHandTip.y, posDesc.leftHandTip.z);
		SETVECTOR3(posDesc.leftHandLatancy[11], posDesc.leftHandBody.x, posDesc.leftHandBody.y, posDesc.leftHandBody.z);

		SETVECTOR3(posDesc.headLatancy[10], posDesc.HeadBombSpark.x, posDesc.HeadBombSpark.y, posDesc.HeadBombSpark.z);
		SETVECTOR3(posDesc.headLatancy[11], posDesc.HeadBombSpark.x, posDesc.HeadBombSpark.y, posDesc.HeadBombSpark.z);

		SETVECTOR3(posDesc.rightShoulderLatancy[10], posDesc.rightShoulder.x, posDesc.rightShoulder.y, posDesc.rightShoulder.z);
		SETVECTOR3(posDesc.rightShoulderLatancy[11], posDesc.rightShoulder.x, posDesc.rightShoulder.y, posDesc.rightShoulder.z);

		SETVECTOR3(posDesc.leftShoulderLatancy[10], posDesc.leftShoulder.x, posDesc.leftShoulder.y, posDesc.leftShoulder.z);
		SETVECTOR3(posDesc.leftShoulderLatancy[11], posDesc.leftShoulder.x, posDesc.leftShoulder.y, posDesc.leftShoulder.z);
	}

	if (posDesc.charID == CID_LIRE1 || posDesc.charID == CID_LIRE2 || posDesc.charID == CID_LIRE3 || posDesc.charID == CID_LIRE4)
	{
		posDesc.bow = posDesc.leftHandLatancy[11];
		posDesc.bow.y -= 0.25f;
		posDesc.bowAngle = D3DXToRadian(90) + atan2f(posDesc.leftHandLatancy[10].y - posDesc.leftHandLatancy[11].y, posDesc.leftHandLatancy[10].x - posDesc.leftHandLatancy[11].x);

		if (posDesc.charID == CID_LIRE2)
		{
			posDesc.crossBowLeft = posDesc.bow;
			posDesc.crossBowLeftAngle = posDesc.bowAngle;

			posDesc.crossBowRight = posDesc.rightHandLatancy[11];
			posDesc.crossBowRight.y -= 0.25f;

			posDesc.crossBowRightAngle = D3DXToRadian(90) + atan2f(posDesc.rightHandLatancy[10].y - posDesc.rightHandLatancy[11].y, posDesc.rightHandLatancy[10].x - posDesc.rightHandLatancy[11].x);
		}
		else if (posDesc.charID == CID_LIRE4) // 노바..
		{
			posDesc.bow = posDesc.leftHandLatancy[10];
			posDesc.bow.y -= 0.24f;
			posDesc.bowAngle = D3DXToRadian(90) + atan2f(posDesc.leftHandLatancy[11].y - posDesc.leftHandLatancy[10].y, posDesc.leftHandLatancy[11].x - posDesc.leftHandLatancy[10].x);
		}
	}
	else if (posDesc.charID == CID_LAS1 || posDesc.charID == CID_LAS3)
	{
		const int iStart = 11;
		const int iEnd = 10;
		posDesc.LasKnifeOne = (posDesc.rightHandLatancy[iStart] + posDesc.rightHandLatancy[iEnd]) * 0.5f; //Center좌표 사용
		posDesc.LasKnifeOne.y -= 0.25f;

		posDesc.LasKnifeTwo = (posDesc.leftHandLatancy[iStart] + posDesc.leftHandLatancy[iEnd]) * 0.5f; //Center좌표 사용
		posDesc.LasKnifeTwo.y -= 0.25f;

		//여기서부터는 단검을 쥐고 있는 각도를 구한다. 손 끝 좌표와 칼 끝 좌표를 이용한 것이다.!
		posDesc.LasKnifeOneAngle = atan2f(posDesc.rightHandLatancy[iEnd].y - posDesc.rightHandLatancy[iStart].y, posDesc.rightHandLatancy[iEnd].x - posDesc.rightHandLatancy[iStart].x);
		posDesc.LasKnifeTwoAngle = atan2f(posDesc.leftHandLatancy[iEnd].y - posDesc.leftHandLatancy[iStart].y, posDesc.leftHandLatancy[iEnd].x - posDesc.leftHandLatancy[iStart].x);
	}
	// 라이언 도끼
	else if (posDesc.charID == CID_RYAN1)
	{
		posDesc.RyanAxe = posDesc.leftHandLatancy[10];
		posDesc.RyanAxe.y -= 0.26f;

		posDesc.RyanAxe.x -= ((posDesc.leftHandLatancy[10].x - posDesc.leftHandLatancy[11].x)) * 0.30f;
		posDesc.RyanAxe.y -= ((posDesc.leftHandLatancy[10].y - posDesc.leftHandLatancy[11].y)) * 0.30f;

		posDesc.RyanAxeAngle = D3DXToRadian(270)
			+ atan2f(posDesc.leftHandLatancy[10].y - posDesc.leftHandLatancy[11].y,
				posDesc.leftHandLatancy[10].x - posDesc.leftHandLatancy[11].x);
	}
	// 센티넬 크리센트
	else if (posDesc.charID == CID_RYAN2)
	{
		// 크리센트
		if (MetamorphosisForm == FORM_NORMAL)
		{
			posDesc.SentinelCricent = posDesc.rightHandLatancy[11];
			posDesc.SentinelCricent.y -= 0.245f;

			if (posDesc.right)
				posDesc.SentinelCricentAngle = atan2f(posDesc.rightHandLatancy[10].y - posDesc.leftHandLatancy[10].y, posDesc.rightHandLatancy[10].x - posDesc.leftHandLatancy[10].x);
			else
				posDesc.SentinelCricentAngle = atan2f(posDesc.leftHandLatancy[10].y - posDesc.rightHandLatancy[10].y, posDesc.leftHandLatancy[10].x - posDesc.rightHandLatancy[10].x);
		}
		else if (MetamorphosisForm == FORM_NEPHILIM)
		{
			posDesc.NephillimLeftHand = posDesc.leftHandLatancy[10];
			posDesc.NephillimRightHand = posDesc.rightHandLatancy[10];

			posDesc.NephillimLeftHand.y -= 0.25f;
			posDesc.NephillimRightHand.y -= 0.25f;
		}
	}
	else
	{
		posDesc.swordTip = posDesc.rightHandLatancy[10];
		posDesc.swordTip.y -= 0.25f;
		posDesc.swordBody = posDesc.rightHandLatancy[11];
		posDesc.swordBody.y -= 0.25f;

		posDesc.leftSwordTip = posDesc.leftHandLatancy[10];
		posDesc.leftSwordTip.y -= 0.25f;
		posDesc.leftSwordBody = posDesc.leftHandLatancy[11];
		posDesc.leftSwordBody.y -= 0.25f;

		posDesc.swordAngle = atan2f(posDesc.swordTip.y - posDesc.swordBody.y, posDesc.swordTip.x - posDesc.swordBody.x);
		posDesc.leftSwordAngle = atan2f(posDesc.leftSwordTip.y - posDesc.leftSwordBody.y, posDesc.leftSwordTip.x - posDesc.leftSwordBody.x);

		if (posDesc.charID == CID_ARME1 || posDesc.charID == CID_ARME2)
		{
			posDesc.staffTip = posDesc.swordTip;
			posDesc.staffBody = posDesc.swordBody;
			posDesc.staffAngle = posDesc.swordAngle;
		}

		posDesc.swordCenter.x = (posDesc.swordBody.x + posDesc.swordTip.x) / 2.0f;
		posDesc.swordCenter.y = (posDesc.swordBody.y + posDesc.swordTip.y) / 2.0f;

		posDesc.leftSwordCenter.x = (posDesc.leftSwordBody.x + posDesc.leftSwordTip.x) / 2.0f;
		posDesc.leftSwordCenter.y = (posDesc.leftSwordBody.y + posDesc.leftSwordTip.y) / 2.0f;

		if (posDesc.charID == CID_ARME3)
		{
			posDesc.swordCenter.y -= 0.05f;
		}

		if (posDesc.charID == CID_ELESIS3)
		{
			posDesc.swordCenter.y -= 0.02f;
			posDesc.leftSwordCenter.y -= 0.02f;
		}

		if (posDesc.charID == CID_ELESIS4)
		{
			posDesc.swordCenter.y -= 0.01f;
			posDesc.leftSwordCenter.y -= 0.01f;
		}

		if ((posDesc.charID == CID_RYAN3 && MetamorphosisForm == FORM_NEPHILIM) ||
			(posDesc.charID == CID_RYAN4 && MetamorphosisForm == FORM_NEPHILIM))
		{
			posDesc.NephillimLeftHand = posDesc.leftHandLatancy[10];
			posDesc.NephillimRightHand = posDesc.rightHandLatancy[10];

			posDesc.NephillimLeftHand.y -= 0.25f;
			posDesc.NephillimRightHand.y -= 0.25f;
		}
	}

	posDesc.effectrightShoulder = posDesc.rightShoulderLatancy[11];
	posDesc.effectrightShoulder.y -= 0.25f;
	posDesc.effectleftShoulder = posDesc.leftShoulderLatancy[11];
	posDesc.effectleftShoulder.y -= 0.25f;
}

void UpdateOverlappedPair(MAP_OVERLAPPED_PAIR& mapOverlappedPair, std::vector<KGCAnimManager::ItemAnim>& kItemAnim, std::vector<ITEM_EFFECT>& vecItemEff)
{
	const int iShowTime = 255 * 2;

	std::map< int, std::vector<std::pair< std::set<int>, std::set<int> > > >::iterator mit;
	for (mit = mapOverlappedPair.begin(); mapOverlappedPair.end() != mit; mit++)
	{
		if (mit->first == 0)
		{
			continue;
		}

		std::vector<std::pair< std::set<int>, std::set<int> > >::iterator vit;
		int iVitIndex = 0;
		for (vit = mit->second.begin(); mit->second.end() != vit; vit++)
		{
			if (mit->second.size() == 1)
			{
				continue;
			}

			if (iVitIndex == (StartGameTime / iShowTime) % mit->second.size())
			{
				BYTE alpha = (255 - abs((int)StartGameTime % iShowTime - 255));
				alpha = alpha > 85 ? 255 : alpha * 3;
				std::set<int>::iterator sit;
				for (sit = (*vit).second.begin(); sit != (*vit).second.end(); sit++)
				{
					kItemAnim[(*sit)].animInst->SetDiffuse(D3DCOLOR_ARGB(alpha, 255, 255, 255));
				}

				for (sit = (*vit).first.begin(); sit != (*vit).first.end(); sit++)
				{
					for (unsigned int k = 0; k < vecItemEff[(*sit)].pSeq.size(); k++)
					{
						vecItemEff[(*sit)].pSeq[k]->m_bRenderForImageTag = true;
					}
				}
			}
			else
			{
				std::set<int>::iterator sit;
				for (sit = (*vit).second.begin(); sit != (*vit).second.end(); sit++)
				{
					kItemAnim[(*sit)].animInst->SetDiffuse(0x00000000);
				}

				for (sit = (*vit).first.begin(); sit != (*vit).first.end(); sit++)
				{
					for (unsigned int k = 0; k < vecItemEff[(*sit)].pSeq.size(); k++)
					{
						vecItemEff[(*sit)].pSeq[k]->m_bRenderForImageTag = false;
					}
				}
			}
			iVitIndex++;
		}
	}
}

void SetItemEffect(ItemEffectParam& kParam)
{
	//#ifdef _DEBUG
	//    GCElapsedTime eTime( L"SetItemEffect" );
	//#endif

	bool bProtectRing = true;	// 수반 Effect 표시

	GCItem* itemData;
	ITEM_EFFECT stEffect;
	for (int j = 0; j < (int)kParam.vecItemInfo.size(); j++)
	{
		char str[15] = "";
		std::string strItemNID;
		int iItemNID = kParam.vecItemInfo[j];
		if (iItemNID == 5357)
			continue;
		strItemNID = itoa(iItemNID, str, 10);

		itemData = g_MyD3D->m_kItemMgr.GetItemData(iItemNID);
		CONTINUE_NIL(itemData);

		int enchantLevel;

		int iStrongLevel = itemData->iStrongLevel;
		EITEMATTRIBUTE eAttribute = itemData->eAttribute;

		if (iItemNID == 14915)
		{
			iStrongLevel = 9;
			if (g_kGlobalValue.m_vecNecklace.size() > 0) {
				std::random_shuffle(g_kGlobalValue.m_vecNecklace.begin(), g_kGlobalValue.m_vecNecklace.end());
				std::vector<int>::iterator mit = g_kGlobalValue.m_vecNecklace.begin();
				eAttribute = static_cast<EITEMATTRIBUTE>(*mit);
			}
			else
			{
				eAttribute = EIATTR_FIRE;
			}
		}

		if (eAttribute != EIATTR_NONE)
		{
			//  발찌면
			if (itemData->dwSlotPosition & ESP_A_SHOES)
			{
				if (kParam.m_Elemental == ITEM_ELEMENTAL_NONE)
					kParam.m_Elemental = g_MyD3D->m_kItemMgr.GetElementalType(eAttribute);
			}
			else
			{
				kParam.m_Elemental = g_MyD3D->m_kItemMgr.GetElementalType(eAttribute);
			}

			strItemNID = g_MyD3D->m_kItemMgr.EnumElementalTypetoStr(g_MyD3D->m_kItemMgr.GetElementalType(eAttribute));
		}

		//발찌는 자신의 강화레벨이 아니므로 - 목걸이만 강화레벨이다.
		if (eAttribute != EIATTR_NONE && kParam.m_StrongLevel < iStrongLevel)
		{
			//  발찌면
			if (!(itemData->dwSlotPosition & ESP_A_SHOES))
				kParam.m_StrongLevel = iStrongLevel;
		}

		if (iStrongLevel < 5)
		{
			enchantLevel = 0;
		}
		else if (iStrongLevel >= 5 && iStrongLevel < 8)
		{
			enchantLevel = 1;
		}
		else if (iStrongLevel == 8)
		{
			enchantLevel = 2;
		}
		else if (iStrongLevel == 9)
		{
			enchantLevel = 3;
		}

		int iVecSize = (int)(kParam.m_vecItemEff.size() + kParam.m_ItemAnim.size()); // 일단 여기서 이전 사이즈를 저장하고

		// 아이템 이펙트 정보를 얻어온다.
		int iAddedItemParticleNum = g_ParticleManager->GetItemEffect(EGCItem, strItemNID, kParam.charID, enchantLevel, kParam.m_vecItemEff);
		if (kParam.m_bSquare == false)
		{
			g_ParticleManager->GetItemEffect(EGCDamage, strItemNID, kParam.charID, enchantLevel, kParam.m_vecDamageEff);
			g_ParticleManager->GetItemEffect(EGCSkill, strItemNID, kParam.charID, enchantLevel, kParam.m_vecSkillEff);
			g_AnimManager.GetNewDamageAnim(strItemNID, kParam.charID, kParam.m_DamageAnim, enchantLevel);
			g_AnimManager.GetNewSkillAnim(strItemNID, kParam.charID, kParam.m_SkillAnim, enchantLevel);
		}

		int iAddedItemAnimNum = g_AnimManager.GetNewItemAnim(strItemNID, kParam.charID, kParam.m_ItemAnim, enchantLevel, kParam.m_bSquare);
		//새로 추가된 anim 중에 겹쳐서는 안되는 아이가 잇는지 찾는다.
		for (int i = 0; i < iAddedItemAnimNum; i++)
		{
			//겹쳐서는 안되는 아이가 잇따!!!
			if (kParam.m_ItemAnim[kParam.m_ItemAnim.size() - 1 - i].positionIndex)
			{
				std::set<int> setAddedItemParticle;
				std::set<int> setAddedItemAnim;

				for (int j = 0; j < iAddedItemAnimNum; j++)
				{
					setAddedItemAnim.insert(kParam.m_ItemAnim.size() - 1 - j);
				}

				for (int j = 0; j < iAddedItemParticleNum; j++)
				{
					setAddedItemParticle.insert(kParam.m_vecItemEff.size() - 1 - j);
				}

				if (kParam.m_mapOverlappedPair.find(kParam.m_ItemAnim[kParam.m_ItemAnim.size() - 1 - i].positionIndex) == kParam.m_mapOverlappedPair.end())
				{
					std::vector<std::pair< std::set<int>, std::set<int> > > vec;
					kParam.m_mapOverlappedPair[kParam.m_ItemAnim[kParam.m_ItemAnim.size() - 1 - i].positionIndex] = vec;
				}

				kParam.m_mapOverlappedPair[kParam.m_ItemAnim[kParam.m_ItemAnim.size() - 1 - i].positionIndex].push_back(std::make_pair(setAddedItemParticle, setAddedItemAnim));

				break;
			}
		}

		if (itemData->dwSlotPosition == ESP_A_FINGER &&
			iVecSize < (int)(kParam.m_vecItemEff.size() + kParam.m_ItemAnim.size())) // 현재 사이즈와 비교해서 성공 여부를 가늠하자 ㅠ
		{
			bProtectRing = false;
		}

		std::vector< int > vecRet = g_pkPetAI->CreatePetinItem(GCUTIL_STR::GCStrCharToWide((char*)strItemNID.c_str()), 0, kParam.m_iPlayerIndex, false, iItemNID);
		if (!vecRet.empty())
		{
			kParam.m_vecPetAiID.insert(kParam.m_vecPetAiID.end(), vecRet.begin(), vecRet.end());
		}
	}

	if (bProtectRing == true)
	{
		//if( kParam.m_bSquare )
		//{
		//    for( int i = 0; i < (int)kParam.m_vecItemSlotSquare.size(); ++i )
		//    {
		//        SetProtectRingEffect( kParam, kParam.m_vecItemSlotSquare[i] / 10 );
		//    }
		//}
		//else
		if (!kParam.m_bSquare)
		{
			for (std::vector<KSlotData>::iterator it = kParam.m_vecItemSlot.begin();
				it != kParam.m_vecItemSlot.end(); it++)
			{
				SetProtectRingEffect(kParam, ((*it).m_kItem.m_dwID) / 10);
			}
		}
	}
}

void SetItemEffect2(ItemEffectParam& kParam)
{
	//#ifdef _DEBUG
	//    GCElapsedTime eTime( L"SetItemEffect2" );
	//#endif

		// 암흑9단계는 펫(?)도 생성한다
	if (kParam.m_Elemental == ITEM_ELEMENTAL_DARK && kParam.m_StrongLevel == 9)
	{
		std::vector< int > vecRet = g_pkPetAI->CreatePetinItem(g_pkStrLoader->GetString(STR_ID_DARK_ENCHANT_LEVEL9).c_str(), 0, kParam.m_iPlayerIndex);
		if (!vecRet.empty())
			kParam.m_vecPetAiID.insert(kParam.m_vecPetAiID.end(), vecRet.begin(), vecRet.end());
	}

	char strTemp[30];
	sprintf(strTemp, "%d", g_MyD3D->m_kItemMgr.GetEquipSetItemName(kParam.vecItemInfo));
	g_ParticleManager->GetItemEffect(EGCItem, strTemp, kParam.charID, 0, kParam.m_vecItemEff);
	if (kParam.m_bSquare == false)
	{
		g_ParticleManager->GetItemEffect(EGCDamage, strTemp, kParam.charID, 0, kParam.m_vecDamageEff);
		g_ParticleManager->GetItemEffect(EGCSkill, strTemp, kParam.charID, 0, kParam.m_vecSkillEff);
		g_AnimManager.GetNewDamageAnim(strTemp, kParam.charID, kParam.m_DamageAnim, 0);
		g_AnimManager.GetNewSkillAnim(strTemp, kParam.charID, kParam.m_SkillAnim, 0);
	}
	g_AnimManager.GetNewItemAnim(strTemp, kParam.charID, kParam.m_ItemAnim, 0, kParam.m_bSquare);
	g_AnimManager.GetManager()->Sort();
}

void SetPlayerTemplate(std::vector< EGCSkillTree > setSkill, char CharNum, PLAYER_TEMPLATE& PlayerTemplate)
{
	int iChar = CharNum % GC_CHAR_NUM;
	int iJob = CharNum / GC_CHAR_NUM;

	char strFileName[MAX_PATH];
	sprintf(strFileName, "PlayerTemplate%d_%d.lua", iChar, iJob);

	if (!KGCPlayerCommon::LoadPlayerTemplate(setSkill, strFileName, PlayerTemplate, CharNum))
	{
		ASSERT("ㅆㅃ");
	}
}

void ProcessPositionDesc(ProcessPositionDescParam& kParam)
{
	D3DXVECTOR3 vecTemp;
	D3DXMATRIX matTemp;
	D3DXVECTOR3 posSword;
	D3DXMATRIX matTrans;

	// 변신 때문에 체크
	ASSERT(kParam.m_pObject != NULL);
	kParam.m_pObject->SetMotionMatrix(false, NULL, false);

	//손목이 아니라 주먹의 위치를 알아내기 위해서...
	// ----------------------------------------------------------------------------- //
	if (kParam.MetamorphosisForm == FORM_MDAN)
	{
		kParam.m_pObject->GetAngleBone(kParam.PlayerTemplate.iWeaponBoneIndex, &matTemp);
		D3DXMatrixTranslation(&matTrans, 0.05f * 2, 0.05f + 0.05f, 0.0f);
		D3DXMatrixMultiply(&matTemp, &matTrans, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
		D3DXMatrixTranslation(&matTrans, -0.05f, -0.05f, 0.0f);
		D3DXMatrixMultiply(&matTemp, &matTrans, &matTemp);
	}
	else
	{
		kParam.m_pObject->GetAngleBone(kParam.PlayerTemplate.iWeaponBoneIndex, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);

		if (kParam.Extra_Char_Num < CID_AMY2)
		{
			D3DXMatrixTranslation(&matTrans, -0.05f, -0.05f, 0.0f);
			D3DXMatrixMultiply(&matTemp, &matTrans, &matTemp);
		}
	}

	SETVECTOR3(kParam.m_posDesc.rightHandBody, matTemp._41, matTemp._42, matTemp._43);

	posSword = kParam.PlayerTemplate.vWeaponEnd - kParam.PlayerTemplate.vWeaponBase;
	D3DXVec3TransformCoord(&kParam.m_posDesc.rightHandTip, &posSword, &matTemp);

	if ((kParam.Extra_Char_Num == CID_RYAN2 ||
		(kParam.Extra_Char_Num == CID_RYAN4 && kParam.PlayerTemplate.WAIT != MID_RYAN4_WAIT))
		&& kParam.MetamorphosisForm != FORM_WOLF)
	{
		// 2007/2/7. iridology. 라이언 1차전직의 왼쪽 타격은 오른손에 위치.
		kParam.m_pObject->GetAngleBone(BID_RIGHT_WRIST, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);

		D3DXMatrixTranslation(&matTrans, 0.05f, -0.05f, 0.0f);
		D3DXMatrixMultiply(&matTemp, &matTrans, &matTemp);
		SETVECTOR3(kParam.m_posDesc.leftHandBody, matTemp._41, matTemp._42, matTemp._43);

		posSword = kParam.PlayerTemplate.vLeftWeaponEnd - kParam.PlayerTemplate.vLeftWeaponBase;
		D3DXVec3TransformCoord(&kParam.m_posDesc.leftHandTip, &posSword, &matTemp);
	}
	else
	{
		kParam.m_pObject->GetAngleBone(kParam.PlayerTemplate.iWeaponBoneIndexLeft, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);;
		//옛날 캐릭터들은 손목본 위치로 공격체크 하기 땜에 보간해줘야 한다.
		if (kParam.Extra_Char_Num < CID_AMY2)
		{
			D3DXMatrixTranslation(&matTrans, 0.05f, -0.05f, 0.0f);
			D3DXMatrixMultiply(&matTemp, &matTrans, &matTemp);
		}

		if (kParam.Extra_Char_Num == CID_RYAN3 && kParam.MetamorphosisForm != FORM_WOLF)
		{
			D3DXVec3TransformCoord(&kParam.m_posDesc.leftHandBody, &kParam.PlayerTemplate.vLeftWeaponBase, &matTemp);
			D3DXVec3TransformCoord(&kParam.m_posDesc.leftHandTip, &kParam.PlayerTemplate.vLeftWeaponEnd, &matTemp);
		}
		else
		{
			SETVECTOR3(kParam.m_posDesc.leftHandBody, matTemp._41, matTemp._42, matTemp._43);

			posSword = kParam.PlayerTemplate.vLeftWeaponEnd - kParam.PlayerTemplate.vLeftWeaponBase;
			posSword.x = -posSword.x;
			D3DXVec3TransformCoord(&kParam.m_posDesc.leftHandTip, &posSword, &matTemp);
		}
	}

	if (kParam.MetamorphosisForm == FORM_NEPHILIM)
	{
		// 2007/2/20. iridology. 네피림 오른 주먹
		kParam.m_pObject->GetAngleBone(23, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
		SETVECTOR3(kParam.m_posDesc.rightHandTip, matTemp._41, matTemp._42, matTemp._43);

		// 2007/2/20. iridology. 네피림 왼 주먹
		kParam.m_pObject->GetAngleBone(18, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
		SETVECTOR3(kParam.m_posDesc.leftHandTip, matTemp._41, matTemp._42, matTemp._43);
	}

	// ------------------------------------------------------------------------------ // 왼발 오른발
	kParam.m_pObject->GetAngleBone(BID_LEFT_KNEE, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);

	kParam.m_posDesc.m_BoneLegLeftTip.x = matTemp._41;
	kParam.m_posDesc.m_BoneLegLeftTip.y = matTemp._42;
	kParam.m_posDesc.m_BoneLegLeftTip.z = matTemp._43;

	kParam.m_pObject->GetAngleBone(BID_RIGHT_KNEE, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);

	kParam.m_posDesc.m_BoneLegRightTip.x = matTemp._41;
	kParam.m_posDesc.m_BoneLegRightTip.y = matTemp._42;
	kParam.m_posDesc.m_BoneLegRightTip.z = matTemp._43;

	if (CID_AMY1 == kParam.Extra_Char_Num || CID_AMY2 == kParam.Extra_Char_Num || CID_AMY3 == kParam.Extra_Char_Num || CID_AMY4 == kParam.Extra_Char_Num ||
		CID_JIN1 == kParam.Extra_Char_Num || CID_JIN2 == kParam.Extra_Char_Num || CID_JIN3 == kParam.Extra_Char_Num || CID_JIN4 == kParam.Extra_Char_Num)
	{
		D3DXVECTOR3 vMovePos = D3DXVECTOR3(0.0f, -0.1f, 0.0f);

		kParam.m_pObject->GetAngleBone(BID_LEFT_KNEE, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
		D3DXVec3TransformCoord(&kParam.m_posDesc.m_BoneLegLeftTip, &vMovePos, &matTemp);

		kParam.m_pObject->GetAngleBone(BID_RIGHT_KNEE, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
		D3DXVec3TransformCoord(&kParam.m_posDesc.m_BoneLegRightTip, &vMovePos, &matTemp);

		kParam.m_pObject->GetAngleBone(BID_NEW_CHAR_RIGHT_HAIR, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
		D3DXVec3TransformCoord(&kParam.m_posDesc.AmyHairLeft, &vMovePos, &matTemp);

		kParam.m_pObject->GetAngleBone(BID_NEW_CHAR_LEFT_HAIR, &matTemp);
		D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
		D3DXVec3TransformCoord(&kParam.m_posDesc.AmyHairRight, &vMovePos, &matTemp);
	}

	kParam.m_pObject->GetAngleBone(BID_BODY_CENTER, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);

	kParam.m_posDesc.m_BoneLegLeftBody.x = matTemp._41;
	kParam.m_posDesc.m_BoneLegLeftBody.y = matTemp._42;
	kParam.m_posDesc.m_BoneLegLeftBody.z = matTemp._43;

	kParam.m_pObject->GetAngleBone(BID_LEFT_FOOT, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);

	kParam.m_posDesc.m_BoneLegRightBody.x = matTemp._41;
	kParam.m_posDesc.m_BoneLegRightBody.y = matTemp._42;
	kParam.m_posDesc.m_BoneLegRightBody.z = matTemp._43;

	kParam.m_pObject->GetAngleBone(BID_LEFT_SHOULDER, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
	D3DXMatrixTranslation(&matTrans, 0.15f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&matTemp, &matTrans, &matTemp);

	kParam.m_posDesc.leftShoulder.x = matTemp._41;
	kParam.m_posDesc.leftShoulder.y = matTemp._42;
	kParam.m_posDesc.leftShoulder.z = matTemp._43;

	kParam.m_pObject->GetAngleBone(BID_RIGHT_SHOULDER, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
	D3DXMatrixTranslation(&matTrans, -0.15f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&matTemp, &matTrans, &matTemp);

	kParam.m_posDesc.rightShoulder.x = matTemp._41;
	kParam.m_posDesc.rightShoulder.y = matTemp._42;
	kParam.m_posDesc.rightShoulder.z = matTemp._43;

	kParam.m_pObject->GetAngleBone(BID_LEFT_FOOT, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
	kParam.m_posDesc.m_vLeftFoot.x = matTemp._41;
	kParam.m_posDesc.m_vLeftFoot.y = matTemp._42;
	kParam.m_posDesc.m_vLeftFoot.z = matTemp._43;

	kParam.m_pObject->GetAngleBone(BID_RIGHT_FOOT, &matTemp);
	D3DXMatrixMultiply(&matTemp, &matTemp, &kParam.matWorld);
	kParam.m_posDesc.m_vLeftFoot.x = matTemp._41;
	kParam.m_posDesc.m_vLeftFoot.y = matTemp._42;
	kParam.m_posDesc.m_vLeftFoot.z = matTemp._43;
}

ProcessPositionDescParam::ProcessPositionDescParam(PLAYER* pPlayer)
	: MetamorphosisForm(pPlayer->MetamorphosisForm)
	, Extra_Char_Num(pPlayer->Extra_Char_Num)
	, m_posDesc(pPlayer->m_posDesc)
	, PlayerTemplate(pPlayer->GetCurFormTemplate())
	, m_pObject(pPlayer->m_pObject)
{
}

ProcessPositionDescParam::ProcessPositionDescParam(GCSquarePeople* pPeople)
	: MetamorphosisForm(FORM_NORMAL)
	, Extra_Char_Num(pPeople->GetExtraCharNum())
	, m_posDesc(pPeople->GetposDesc())
	, PlayerTemplate(pPeople->GetPlayerTemplate())
	, m_pObject(pPeople->GetGCObject())
{
}
D3DXVECTOR3* ComputePosDesc(PositionDesc& posDesc, int ePosType)
{
	switch (ePosType)
	{
	case EPT_BODY_CENTER: return &posDesc.body;
	case EPT_SWORD_TIP: return &posDesc.swordTip;
	case EPT_SWORD_BODY: return &posDesc.swordBody;
	case EPT_SWORD_CENTER: return &posDesc.swordCenter;
	case EPT_BOW: return &posDesc.bow;
	case EPT_CROSSBOW_ONE: return &posDesc.crossBowLeft;
	case EPT_CROSSBOW_TWO: return &posDesc.crossBowRight;
	case EPT_LASKNIFE_ONE: return &posDesc.LasKnifeOne;
	case EPT_LASKNIFE_TWO: return &posDesc.LasKnifeTwo;
	case EPT_AXE: return &posDesc.RyanAxe;
	case EPT_CRICENT:           return &posDesc.SentinelCricent;
	case EPT_RHAND: return &posDesc.rightHandTip;
	case EPT_LHAND: return &posDesc.leftHandTip;
	case EPT_NEPHILLIM_RHAND: return &posDesc.NephillimRightHand;
	case EPT_NEPHILLIM_LHAND: return &posDesc.NephillimLeftHand;
	case EPT_LEFT_SWORD_TIP: return &posDesc.leftSwordTip;
	case EPT_LEFT_SWORD_BODY: return &posDesc.leftSwordBody;
	case EPT_LEFT_SWORD_CENTER: return &posDesc.leftSwordCenter;
	case EPT_LHAIR: return &posDesc.AmyHairLeft;
	case EPT_RHAIR: return &posDesc.AmyHairRight;
	case EPT_HEAD:	return &posDesc.HeadPos;
	case EPT_LFOOT: return &posDesc.m_vLeftFoot;
	case EPT_RFOOT: return &posDesc.m_vRightFoot;
	case EPT_RSHOULDER: return &posDesc.effectrightShoulder;
	case EPT_LSHOULDER: return &posDesc.effectleftShoulder;
	case EPT_EFFECT_RHAND:
		//posDesc.rightHandLatancy[11].y -= 0.23f;
		return &posDesc.rightHandLatancy[11];
	case EPT_EFFECT_LHAND:
		//posDesc.leftHandLatancy[11].y -= 0.46f;
		return &posDesc.leftHandLatancy[11];
	default: return NULL;
	}
}
void CreateItemEffect(CreateItemEffectParam& kParam)
{
	CParticleEventSeqPTR pTemp;

	for (unsigned int j = 0; j < kParam.stEffect.emitPositionType.size(); ++j)
	{
		for (unsigned int i = 0; i < kParam.stEffect.sequenceName.size(); ++i)
		{
			pTemp = g_ParticleManager->CreateSequence(kParam.stEffect.sequenceName[i], kParam.x, kParam.y, kParam.z);
			if (pTemp != NULL)
			{
				// Trace정보 세팅
				if (kParam.stEffect.m_bTrace == true)
				{
					TRACE_INFO stTrace;
					stTrace.m_pvPos = ComputePosDesc(kParam.posDesc, kParam.stEffect.emitPositionType[j]);
					stTrace.m_fParticleTraceTime = kParam.stEffect.m_fParticleTraceTime;
					stTrace.m_fXOffset = kParam.stEffect.m_fOffsetX;
					stTrace.m_fYOffset = kParam.stEffect.m_fOffsetY;
					pTemp->SetTrace(&stTrace);
				}

				// 방향성 세팅
				if (kParam.stEffect.m_bDirVelocity == true || kParam.stEffect.m_fDirPosX != 0.0f)
				{
					DIRECTION_INFO stDirect;
					stDirect.m_fDirPosX = kParam.stEffect.m_fDirPosX;
					stDirect.m_bDirVelocity = kParam.stEffect.m_bDirVelocity;
					stDirect.m_pbIsRight = &kParam.bIsRight;
					pTemp->SetDirectInfo(&stDirect);
				}

				pTemp->SetShow(&kParam.bShowEffect);
				kParam.stEffect.pSeq.push_back(pTemp);
			}
		}
	}
}

void SetProtectRingEffect(ItemEffectParam& kParam, DWORD dwItemID)
{
	if (g_MyD3D->IsProtectRing(dwItemID) == true)
	{
		char str[15] = "";
		std::string strItemNID;
		strItemNID = itoa(static_cast<int>(dwItemID), str, 10);

		g_ParticleManager->GetItemEffect(EGCItem, strItemNID, kParam.charID, 0, kParam.m_vecItemEff);
		g_AnimManager.GetNewItemAnim(strItemNID, kParam.charID, kParam.m_ItemAnim, 0);
	}
}

bool IsUseFaceLiftChar(char cExtraCharNum_, EGCFaceLiftType eFaceType_)
{
	std::vector< EGCSkillTree > setSkill;
	PLAYER_TEMPLATE kPlayerTemplate;
	SetPlayerTemplate(setSkill, cExtraCharNum_, kPlayerTemplate);
	std::map< int, FACE_LIFT_MESH >::iterator mit = kPlayerTemplate.mapFaceLiftMesh.find(eFaceType_);

	if (mit != kPlayerTemplate.mapFaceLiftMesh.end()) {
		return true;
	}

	return false;
}

void GetPlayerTemplate(char CharNum, PLAYER_TEMPLATE& PlayerTemplate)
{
	std::vector< EGCSkillTree > setSkill;
	SetPlayerTemplate(setSkill, CharNum, PlayerTemplate);
}