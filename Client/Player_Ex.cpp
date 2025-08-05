#include "stdafx.h"
//
//
#include "Message.h"

#include "QuestGameManager.h"
//
#include "KStats.h"
#include "KGCPlayerCommon.h"
#include "PrimitiveTypedef.h"

#include "DamageManager.h"
#include "Spark.h"

#include "GCStateMachine.h"
#include "Controls.h"
#include "Headup Display.h"
#include "Monster.h"
//
////
#include "Stage.h"
#include "./GCUI/GCHeadupDisplay.h"

#include "Buff/KGCBuffManager.h"
#include "GCCameraOrtho.h"
#include "KGCGameBoard.h"
#include "Replay.h"

//
#include "./gcui/GCGameOverlayUI.h"
#include "./gcui/GCAgitOverlayUI.h"
#include "./gcui/GCMyHeadup.h"
#include "PetAI.h"

#include "GCEventManager.h"
//

#include "gcui/KGCIndigoSelect.h"
#include "GCRenderManager.h"
#include "KGCChannelManager.h"
#include "./MapSystem/GCFantasticMap.h"
#include "KGCRoomManager.h"

#include "GCO2Manager.h"
#include "KGCPlayerAmy.h"
#include "KGCPlayerAmy_2.h"
#include "KGCPlayerAmy2.h"
#include "KGCPlayerAmy2_2.h"
#include "KGCPlayerAmy3.h"
#include "KGCPlayerAmy3_2.h"
#include "GCStateMachine.h"
#include "GCStateLoading.h"
#include "KSingleton.h"
#include "KncP2PLib/KncPeerManager.h"
#include "KncP2PLib/KncP2P.h"

#include "GCCoupleSystem.h"
#include "GCStateGame.h"
#include "KGCGuildMatch_CapTain.h"

#include "KGCWorldMapManager.h"

#include "GCIconImageManager.h"
#include "GCGraphicsHelper.h"

#include "KGCMultipleLanguages.h"
#include "GCEclipseEventManager.h"

#include "OpenProcess.h"

#include "KGCQALogManager.h"
#include "GCAutoMatchManager.h"
#include "KGCMatchManager.h"
#include "GCBuffDonationManager.h"
#include "GCUI/GCFatigueInfoBox.h"
// 3초i
const DWORD NextTagEnableTIme = GC_FPS_LIMIT * 3;
std::string TestString;

const float g_cfPvPCriticalDamageUp = 0.4753f;

#define __EQUAL_OPERATOR__(p) p = pPlayer->p

// 100을 곱한 값이 실제 공/방/생 입니다.
#if defined( NATION_KOREA )
#define BALANCE_PVP_ABILITY_ATK_VALUE 5000.0f;
#define BALANCE_PVP_ABILITY_DEF_VALUE 3800.f;
#define BALANCE_PVP_ABILITY_HP_VALUE  3000.0f;
#else
#define BALANCE_PVP_ABILITY_ATK_VALUE 50.0f;
#define BALANCE_PVP_ABILITY_DEF_VALUE 15.2f;
#define BALANCE_PVP_ABILITY_HP_VALUE 50.0f;
#endif

void TagCharacterInfo::InitTagCharData(PLAYER* pPlayer)
{
	__EQUAL_OPERATOR__(Extra_Char_Num);
	m_fHP = pPlayer->GetHP();
	__EQUAL_OPERATOR__(m_fAbility[ABILITY_HP]);
	__EQUAL_OPERATOR__(m_fAbility[ABILITY_ATK]);
	__EQUAL_OPERATOR__(m_fAbility[ABILITY_DEF]);
	__EQUAL_OPERATOR__(m_iSkillType);
	__EQUAL_OPERATOR__(m_iWeaponType);
	__EQUAL_OPERATOR__(MetamorphosisForm);
	__EQUAL_OPERATOR__(m_setExtra2);
	__EQUAL_OPERATOR__(m_vecSpecialAbility);
	__EQUAL_OPERATOR__(m_cLife);
	__EQUAL_OPERATOR__(m_pLire3Arrow);
	for (int i = 0; i < FORM_NUM; ++i)
		__EQUAL_OPERATOR__(m_pObjMetaForm[i]);

	for (int i = 0; i < ABILITY_MAX; ++i)
	{
		__EQUAL_OPERATOR__(m_fAbility[i]);
	}

	m_ePlayerState = pPlayer->GetPlayerState();
	m_iCharType = pPlayer->GetCurrentChar().iCharType;
	Extra_Char_Num = m_iCharType + GC_CHAR_NUM * pPlayer->GetPromotion(m_iCharType);
	m_fMana = g_MyD3D->MyHead->Level;
	//m_vecItemModel = pPlayer->GetItemModelVec();
	//m_pObjShadow = pPlayer->GetObjShadow();

	m_bSuperArmor = pPlayer->SuperArmor();
	m_bHyperArmor = pPlayer->m_bHyperArmor;
	m_bIsBlackMagic = g_MyD3D->MyHead->IsBlackMagic;

	m_mapwSkillItem = pPlayer->m_mapwSkillItem;

	for (int i = 0; i < FORM_NUM; ++i)
		__EQUAL_OPERATOR__(m_FormTemplate[i]);

	for (int i = 0; i < BTYPE_NUM; ++i)
		__EQUAL_OPERATOR__(m_BurnningTemplate[i]);

	__EQUAL_OPERATOR__(m_vecPetAiID);

	__EQUAL_OPERATOR__(m_vecItemEff);
	__EQUAL_OPERATOR__(m_vecDamageEff);
	__EQUAL_OPERATOR__(m_vecSkillEff);
	__EQUAL_OPERATOR__(Las4GreatSword);

	__EQUAL_OPERATOR__(m_ItemAnim);
	__EQUAL_OPERATOR__(m_DamageAnim);
	__EQUAL_OPERATOR__(m_SkillAnim);
	__EQUAL_OPERATOR__(m_pLasFireWingLeft);
	__EQUAL_OPERATOR__(m_pLasFireWingRight);
	__EQUAL_OPERATOR__(m_pDashEff);
	__EQUAL_OPERATOR__(m_pLasMotalBlowReady);
	__EQUAL_OPERATOR__(m_pLasMotalBlowReadyAnim);

	__EQUAL_OPERATOR__(m_Elemental);
	__EQUAL_OPERATOR__(m_StrongLevel);
	__EQUAL_OPERATOR__(m_SwapTemplate);

	__EQUAL_OPERATOR__(m_mapOverlappedPair);
	__EQUAL_OPERATOR__(m_pRonan3Force);
	__EQUAL_OPERATOR__(m_pRonan3Force1);
	__EQUAL_OPERATOR__(m_pRonan3Force2);
	__EQUAL_OPERATOR__(m_pRuneShield);
	__EQUAL_OPERATOR__(m_pRuneShield1);

	for (int j = 0; j < 4; j++)
		__EQUAL_OPERATOR__(m_pMonoVolt[j]);
	__EQUAL_OPERATOR__(m_pAbyssHandEff[0]);
	__EQUAL_OPERATOR__(m_pAbyssHandEff[1]);

	__EQUAL_OPERATOR__(m_vecHotKeySkill);
}
#undef __EQUAL_OPERATOR__

std::string TagCharacterInfo::GetPlayerStateStr()
{
	std::string str;
#define caseStateToString(state) case state: str = #state; break;
	switch (m_ePlayerState)
	{
		caseStateToString(PS_INVALID);
		caseStateToString(PS_ALIVE);
		caseStateToString(PS_FATAL);
		caseStateToString(PS_DEAD);
	default:
		str = "ErrPlayerState";
		break;
	}

	return str;
}

std::string TagCharacterInfo::GetCharString()
{
	switch (Extra_Char_Num)
	{
#define CHARTOSTRING(p) case p: return #p; break;
		CHARTOSTRING(CID_COMMON);
		CHARTOSTRING(CID_ELESIS1);
		CHARTOSTRING(CID_LIRE1);
		CHARTOSTRING(CID_ARME1);
		CHARTOSTRING(CID_LAS1);
		CHARTOSTRING(CID_RYAN1);
		CHARTOSTRING(CID_RONAN1);
		CHARTOSTRING(CID_AMY1); // 진 - ui작업할 때 주석들 풀어주자
		CHARTOSTRING(CID_JIN1);
		CHARTOSTRING(CID_ELESIS2);
		CHARTOSTRING(CID_LIRE2);
		CHARTOSTRING(CID_ARME2);
		CHARTOSTRING(CID_LAS2);
		CHARTOSTRING(CID_RYAN2);
		CHARTOSTRING(CID_RONAN2);
		CHARTOSTRING(CID_AMY2);
		CHARTOSTRING(CID_JIN2);
		CHARTOSTRING(CID_ELESIS3);
		CHARTOSTRING(CID_LIRE3);
		CHARTOSTRING(CID_ARME3);
		CHARTOSTRING(CID_LAS3);
		CHARTOSTRING(CID_RYAN3);
		CHARTOSTRING(CID_RONAN3);
		CHARTOSTRING(CID_AMY3);
		CHARTOSTRING(CID_JIN3);
		CHARTOSTRING(CID_ELESIS4);
		CHARTOSTRING(CID_LIRE4);
		CHARTOSTRING(CID_ARME4);
		CHARTOSTRING(CID_LAS4);
		CHARTOSTRING(CID_RYAN4);
		CHARTOSTRING(CID_RONAN4);
		CHARTOSTRING(CID_AMY4);
		CHARTOSTRING(CID_JIN4);
	}

	return "CID_MAX";
#undef CHARTOSTRING
}

std::string TagCharacterInfo::toStr()
{
	std::stringstream stm;

	stm << "NeedDelete : " << m_bNeedDelete << ", CharType : " << m_iCharType
		<< ", PlayerState : " << GetPlayerStateStr() << ", Extra_Char_Num : " << GetCharString() << ",\n"
		<< "\tStrongLevel : " << m_StrongLevel << ", Metamorphosis : " << MetamorphosisForm
		<< ", ItemModelSize : " /*<< m_vecItemModel.size() */ << ", SkillType : " << m_iSkillType << ",\n"
		<< "\tLife : " << static_cast<int>(m_cLife) << ", HP : " << static_cast<float>(m_fHP)
		<< ", Mana : " << static_cast<float>(m_fMana) << "\n";

	return stm.str();
}

void TagCharacterInfo::EffectClear(int iPlayerNumber)
{
	// 파티클 삭제
	for (int i = 0; i < (int)m_vecItemEff.size(); ++i)
		g_ParticleManager->DeleteSequence(m_vecItemEff[i].pSeq);
	for (int i = 0; i < (int)m_vecDamageEff.size(); ++i)
		g_ParticleManager->DeleteSequence(m_vecDamageEff[i].pSeq);
	for (int i = 0; i < (int)m_vecSkillEff.size(); ++i)
		g_ParticleManager->DeleteSequence(m_vecSkillEff[i].pSeq);

	m_vecItemEff.clear();
	m_vecDamageEff.clear();
	m_vecSkillEff.clear();

	g_ParticleManager->DeleteSequence(m_pRonan3Force);
	g_ParticleManager->DeleteSequence(m_pRonan3Force1);
	g_ParticleManager->DeleteSequence(m_pRonan3Force2);
	g_ParticleManager->DeleteSequence(m_pRuneShield);
	g_ParticleManager->DeleteSequence(m_pRuneShield1);
	g_ParticleManager->DeleteSequence(m_pLasFireWingLeft);
	g_ParticleManager->DeleteSequence(m_pLasFireWingRight);
	g_ParticleManager->DeleteSequence(m_pLasMotalBlowReady);
	g_ParticleManager->DeleteSequence(m_pDashEff);

	for (int i = 0; i < 4; i++)
		g_ParticleManager->DeleteSequence(m_pMonoVolt[i]);
	g_ParticleManager->DeleteSequence(m_pAbyssHandEff[0]);
	g_ParticleManager->DeleteSequence(m_pAbyssHandEff[1]);
	//g_ParticleManager->DeleteSequence( m_pLire3Wing );

	// 애니메이션 삭제
	for (int i = 0; i < (int)m_ItemAnim.size(); ++i)
		g_AnimManager.DeleteInst(m_ItemAnim[i].animInst);

	for (int i = 0; i < (int)Las4GreatSword.size(); ++i)
		g_AnimManager.DeleteInst(Las4GreatSword[i].animInst);

	for (int i = 0; i < (int)m_SkillAnim.size(); ++i)
		g_AnimManager.DeleteSkillInst(m_SkillAnim[i]);

	m_ItemAnim.clear();
	m_DamageAnim.clear();
	m_SkillAnim.clear();
	Las4GreatSword.clear();

	g_MyD3D->m_pMapAnim->DeleteInst(m_pLasMotalBlowReadyAnim);

	m_pRonan3Force = NULL;
	m_pRonan3Force1 = NULL;
	m_pRonan3Force2 = NULL;
	m_pRuneShield = NULL;
	m_pRuneShield1 = NULL;
	m_pLasFireWingLeft = NULL;
	m_pLasFireWingRight = NULL;
	m_pLasMotalBlowReady = NULL;
	m_pLasMotalBlowReadyAnim = NULL;
	for (int i = 0; i < 4; i++)
		m_pMonoVolt[i] = NULL;
	m_pAbyssHandEff[0] = NULL;
	m_pAbyssHandEff[1] = NULL;
	//m_pLire3Wing = NULL;
	m_pDashEff = NULL;
	m_Elemental = 0;
	m_StrongLevel = 0;

	m_mapOverlappedPair.clear();
}

void TagCharacterInfo::ShowEffect(bool bShow)
{
	for (std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
	{
		if (MetamorphosisForm == FORM_NORMAL)
			(*j).animInst->SetShow(bShow);
		else
			(*j).animInst->SetShow(false);
	}

	for (std::vector<ITEM_EFFECT>::iterator i = m_vecItemEff.begin(); i != m_vecItemEff.end(); i++)
	{
		for (unsigned int k = 0; k < (*i).pSeq.size(); k++)
		{
			if (MetamorphosisForm == FORM_NORMAL)
				(*i).pSeq[k]->SetShow(bShow);
			else
				(*i).pSeq[k]->SetShow(false);
		}
	}
}

void TagCharacterInfo::SetTagCharacterAbility(IN const EAbilityData& stAbilityData_)
{
	for (int i = 0; i < ABILITY_MAX; ++i)
	{
		m_fAbility[i] = stAbilityData_.fEquipItemAbility[i];
	}

	m_fHP = m_fAbility[ABILITY_HP];
}

//모든 데이터를 초기화합니다.
void PLAYER::ClearPlayerData(bool bInitUserInfo)
{
	//유저 정보 초기화
	ClearPlayerName();
	ReleasePlayerResource();
	//InitBurnningGauge();

	if (bInitUserInfo)
		m_kUserInfo.Init();

	m_kUserInfo.bLive = false;
	m_kUserInfo.nUserState = GC_RUS_NONE;
	m_kUserInfo.dwUID = 0;
	m_fGetExpGpRatio = 0.0f;

	m_bSuperMode = 0;
	g_kMonsterManager.CreateMonsterBoolState(m_mapS1AttackState);
	g_kMonsterManager.CreateMonsterBoolState(m_mapS2AttackState);
	g_kMonsterManager.CreateMonsterBoolState(m_mapS3AttackState);

	SiKGCWeatherSystem()->SetRender(-1, false);
}

int PLAYER::GetExtraCharNum(int iCharType)
{
	return iCharType + GetPromotion(iCharType) * GC_CHAR_NUM;
}

int PLAYER::AutoSetExtra()
{
	int iPromotionLevel = GetPromotion((int)m_kUserInfo.GetCurrentChar().iCharType);

#if defined( _DEBUG ) || !defined( __PATH__ )
	// Todo : 전직레벨을 강제로 수정해 주는 부분..
	// [임시] Lupus 무기 테스트용 전직( 건슬링거로 처리하고 있기 때문에 바꿈 )
	//if( 13 == m_kUserInfo.GetCurrentChar().iCharType )
	   // iPromotionLevel = 0;
#endif

	Extra_Char_Num = GetCurrentChar().iCharType + iPromotionLevel * GC_CHAR_NUM;
	return iPromotionLevel;
}

void PLAYER::AutoSetExtra(TagCharacterInfo& tagInfo)
{
	int iPromotionLevel = GetPromotion(tagInfo.m_iCharType);
	tagInfo.Extra_Char_Num = tagInfo.m_iCharType + iPromotionLevel * GC_CHAR_NUM;
}

DWORD PLAYER::GetCharTypeDword(int iCharType)
{
	DWORD dwCharType;
	switch (iCharType)
	{
	case GC_CHAR_ELESIS:
		dwCharType = ECT_ELESIS;
		break;
	case GC_CHAR_LIRE:
		dwCharType = ECT_LIRE;
		break;
	case GC_CHAR_ARME:
		dwCharType = ECT_ARME;
		break;
	case GC_CHAR_LAS:
		dwCharType = ECT_LAS;
		break;
	case GC_CHAR_RYAN:
		dwCharType = ECT_RYAN;
		break;
	case GC_CHAR_RONAN:
		dwCharType = ECT_RONAN;
		break;
	case GC_CHAR_AMY:
		dwCharType = ECT_AMY;
		break;
	case GC_CHAR_JIN:
		dwCharType = ECT_JIN;
		break;
	case GC_CHAR_SIEG:
		dwCharType = ECT_SIEG;
		break;
	case GC_CHAR_MARI:
		dwCharType = ECT_MARI;
		break;
	case GC_CHAR_DIO:
		dwCharType = ECT_DIO;
		break;
	case GC_CHAR_ZERO:
		dwCharType = ECT_ZERO;
		break;
	case GC_CHAR_LEY:
		dwCharType = ECT_LEY;
		break;
	case GC_CHAR_LUPUS:
		dwCharType = ECT_LUPUS;
		break;
	case GC_CHAR_RIN:
		dwCharType = ECT_RIN;
		break;
	case GC_CHAR_ASIN:
		dwCharType = ECT_ASIN;
		break;
	case GC_CHAR_LIME:
		dwCharType = ECT_LIME;
		break;
	case GC_CHAR_EDEL:
		dwCharType = ECT_EDEL;
		break;
	case GC_CHAR_BEIGAS:
		dwCharType = ECT_BEIGAS;
		break;
	case GC_CHAR_UNO:
		dwCharType = ECT_UNO;
		break;
#ifdef PERYTON // Kawan>
	case GC_CHAR_PERYTON:
		dwCharType = ECT_PERYTON;
		break;
#endif
#ifdef ALICE 
	case GC_CHAR_ALICE:
		dwCharType = ECT_ALICE;
		break;
#endif
#ifdef DECANEE 
	case GC_CHAR_DECANEE:
		dwCharType = ECT_DECANEE;
		break;
#endif
	default:
		ASSERT(!"Invalid CharType");
		dwCharType = ECT_ELESIS;
		break;
	}

	return dwCharType;
}

int PLAYER::GetNowCharPromotion()
{
	SCharInfo sChar = m_kUserInfo.GetCurrentChar();
	if (sChar.iCharType > -1)
		return GetPromotion(sChar.iCharType);
	return 0;
}

bool PLAYER::IsCharTypeMode()
{
	//  연습모드 진행중이라면
	if (g_MyD3D->m_pStateMachine->GetState() == GS_TUTORIAL)
		return false;

	if (g_MyD3D->m_pStateMachine->GetState() == GS_GAME && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
		return false;

	// 미니게임 진행중이라면
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME)
		return false;

	return true;
}

int PLAYER::GetPromotion(int iCharType)
{
	SCharInfo* pCharInfo;

	//  연습모드나 미니게임인지
	if (IsCharTypeMode()) // 타입
	{
		pCharInfo = &m_kUserInfo.GetCurrentChar(iCharType);
	}
	else {
		pCharInfo = &m_kUserInfo.GetCurrentChar();
	}

	if (pCharInfo == NULL)
		g_kGlobalValue.m_strNullFunc = __FUNCTION__;

	int jobLevel = 0;
	GCItem* kItem = NULL;

	if (g_pkUIScene->IsCurMsgBox_Type(KGCUIScene::GC_MBOX_MISSION_PREVIEW_S4_DLG) == false) {
		if (g_kGlobalValue.IsQuickSkillPlayer(pCharInfo->iCharType) ||
			g_kGlobalValue.IsHybridPlayer(pCharInfo->iCharType)) {
			return pCharInfo->iPromotionLevel;
		}
	}

	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); i++)
	{
		////무기 갈아낄 때만 캐릭터가 전직용으로 바뀌든지 한다!
		kItem = g_MyD3D->m_kItemMgr.GetItemData(pCharInfo->vecItemInfo[i].iItemID);
		CONTINUE_NIL(kItem);

		if (kItem->dwSlotPosition & ESP_WEAPON)
		{
			// 2007/2/15. iridology. 이 케릭터가 장착할 수 있는 장비만 검사합니다.
			//if( GetCharTypeDword( iCharType ) & kItem->dwCharType )
			//{
			if (jobLevel < kItem->iNeedChangeJobLevelForEquippingItem)
			{
				jobLevel = kItem->iNeedChangeJobLevelForEquippingItem;
				return jobLevel;
			}
			//}
		}
	}
	return 0;
}

void PLAYER::SetFormTemplate(char CharNum, KSafeArray<PLAYER_TEMPLATE, FORM_NUM>& FormTemplate)
{
	// 라이언4차 매그너스 단은 아이템이 아니라 무조건 사용할 수 있는 기술이라서 추가.
	if (CharNum == CID_RYAN4)
	{
		KGCPlayerCommon::LoadPlayerTemplate(m_setSkill, "formMDAN.lua", FormTemplate[FORM_MDAN]);
		//GetCurFormTemplate().sMetamorphosisTemplate = FormTemplate[FORM_MDAN].sMetamorphosisTemplate;
		//FormTemplate[FORM_MDAN].sMetamorphosisTemplate.init();
	}

	KGCPlayerCommon::LoadPlayerTemplate(m_setSkill, "form_rin_goddess.lua", FormTemplate[FORM_RIN_GODDESS]);
	KGCPlayerCommon::LoadPlayerTemplate(m_setSkill, "form_rin_evil.lua", FormTemplate[FORM_RIN_EVIL]);

	KUserItemVector vecItem;

	// 튜토리얼은 임의의 아이템을 사용하여 플레이 할 수 있게 하기 때문에... 따로 처리.
	if (GC_GMC_TUTORIAL == SiKGCRoomManager()->GetGameModeCategory())
		vecItem = GetCurrentChar().vecItemInfo;
	else
		vecItem = m_kUserInfo.GetCurrentChar(CharNum % GC_CHAR_NUM).vecItemInfo;

	KUserItemVector::iterator vitItem;
	for (vitItem = vecItem.begin(); vitItem != vecItem.end(); ++vitItem)
	{
		SUserItem& item = *vitItem;

		switch (item.iItemID)
		{
		case KGCItemManager::ITEM_FULL_MOON_RING:
		{
			std::vector< EGCSkillTree > setSkillTemp;
			KGCPlayerCommon::LoadPlayerTemplate(setSkillTemp, "FormWolf.lua", FormTemplate[FORM_WOLF], CID_COMMON);

			char CharIndex = CharNum % GC_CHAR_NUM;
			METAMORPHOSIS_TEMPLATE::S_MetamorphosisTexture sMetamorphosisTexture;
			sMetamorphosisTexture.strTextureFileName = "Wolf";
			sMetamorphosisTexture.iGoodsID = KGCItemManager::ITEM_FULL_MOON_RING;

			if (CharIndex == GC_CHAR_ELESIS)
				sMetamorphosisTexture.strTextureFileName += "Elesis.DDS";
			else if (CharIndex == GC_CHAR_LIRE)
				sMetamorphosisTexture.strTextureFileName += "Lire.DDS";
			else if (CharIndex == GC_CHAR_ARME)
				sMetamorphosisTexture.strTextureFileName += "Arme.DDS";
			else if (CharIndex == GC_CHAR_LAS)
				sMetamorphosisTexture.strTextureFileName += "Las.DDS";
			else if (CharIndex == GC_CHAR_RYAN)
				sMetamorphosisTexture.strTextureFileName += "Ryan.DDS";
			else if (CharIndex == GC_CHAR_RONAN)
				sMetamorphosisTexture.strTextureFileName += "Ronan.DDS";
			else if (CharIndex == GC_CHAR_AMY)
				sMetamorphosisTexture.strTextureFileName += "Amy.DDS";
			else if (CharIndex == GC_CHAR_JIN) {
				sMetamorphosisTexture.strTextureFileName += "Jin.DDS";
				InitBurnningGauge();
			}
			else if (CharIndex == GC_CHAR_SIEG)
				sMetamorphosisTexture.strTextureFileName += "Sieg.DDS";
			else if (CharIndex == GC_CHAR_MARI)
				sMetamorphosisTexture.strTextureFileName += "Mari.DDS";
			else if (CharIndex == GC_CHAR_RIN)
				sMetamorphosisTexture.strTextureFileName += "Rin.dds";
			else if (CharIndex == GC_CHAR_ASIN)
				sMetamorphosisTexture.strTextureFileName += "ASIN.dds";
			else if (CharIndex == GC_CHAR_LIME)
				sMetamorphosisTexture.strTextureFileName += "Lime.dds";

#if !defined(__PATH__)
			if (!g_pGCDeviceManager2->CreateTexture(sMetamorphosisTexture.strTextureFileName)) {
				MessageBoxA(NULL, "만월반지 캐릭터 텍스처 없음!", sMetamorphosisTexture.strTextureFileName.c_str(), MB_OK);
			}
#endif

			size_t vecSize = FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecMetamorphosisTexture.size();

			if (vecSize > 0)
			{
				FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecMetamorphosisTexture.push_back(FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecMetamorphosisTexture[vecSize - 1]);
				FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecMetamorphosisTexture[vecSize - 1] = sMetamorphosisTexture;
			}
			else {
				FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecMetamorphosisTexture.push_back(sMetamorphosisTexture);
			}

			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecStartEffect[0].strEffectName = "Change_wolf01";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecStartEffect[1].strEffectName = "Change_wolf02";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecStartEffect[2].strEffectName = "Change_wolf03";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecStartEffect[3].strEffectName = "Change_wolf04";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecStartEffect[4].strEffectName = "Change_wolf05";

			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecEndEffect[0].strEffectName = "Change_wolf01";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecEndEffect[1].strEffectName = "Change_wolf02";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecEndEffect[2].strEffectName = "Change_wolf03";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecEndEffect[3].strEffectName = "Change_wolf04";
			FormTemplate[FORM_WOLF].sMetamorphosisTemplate.vecEndEffect[4].strEffectName = "Change_wolf05";
		}
		break;
		case 3225:
		case 3227:
		case 3253:
		case 13713:
		case 13714:
			KGCPlayerCommon::LoadPlayerTemplate(m_setSkill, "FormWolf.lua", FormTemplate[FORM_WOLF], CID_COMMON);
			//GetCurFormTemplate().sMetamorphosisTemplate = FormTemplate[FORM_WOLF].sMetamorphosisTemplate;
			//FormTemplate[FORM_WOLF].sMetamorphosisTemplate.init();
			break;
		case 3953:
		case 3954:
		case 13715:
			KGCPlayerCommon::LoadPlayerTemplate(m_setSkill, "FormNephilim.lua", FormTemplate[FORM_NEPHILIM]);
			//GetCurFormTemplate().sMetamorphosisTemplate = FormTemplate[FORM_NEPHILIM].sMetamorphosisTemplate;
			//FormTemplate[FORM_NEPHILIM].sMetamorphosisTemplate.init();
			break;
		}
	}
}

void PLAYER::LoadPlayerTemplate(TagCharacterInfo* pTagInfo)
{
	for (int i = 0; i < FORM_NUM; ++i)
		pTagInfo->m_FormTemplate[i].Init();

	for (int i = 0; i < BTYPE_NUM; ++i)
		pTagInfo->m_BurnningTemplate[i].Init();

	pTagInfo->m_SwapTemplate.Init();

	PrepairSkill(pTagInfo->Extra_Char_Num);

	SetPlayerTemplate(m_setSkill, pTagInfo->Extra_Char_Num, pTagInfo->GetCurFormTemplate());
	SetFormTemplate(pTagInfo->Extra_Char_Num, pTagInfo->m_FormTemplate);
	SetSwapWeaponTemplate(pTagInfo->Extra_Char_Num, pTagInfo->m_SwapTemplate);
	SetBurnningTemplate(pTagInfo->Extra_Char_Num, pTagInfo->m_BurnningTemplate);

	if (m_fAbility[ABILITY_MOVEMENT_SPEED_UP] > 0.f)
	{
		float fSpeedUpRate = 1.f + (m_fAbility[ABILITY_MOVEMENT_SPEED_UP] / 100.f);

		GetCurFormTemplate().fRunSpeed *= fSpeedUpRate;
		GetCurFormTemplate().fWalkSpeed *= fSpeedUpRate;
		m_SwapTemplate.fRunSpeed *= fSpeedUpRate;
		m_SwapTemplate.fWalkSpeed *= fSpeedUpRate;

		for (int i = 0; i < BTYPE_NUM; ++i)
		{
			m_BurnningTemplate[i].fRunSpeed *= fSpeedUpRate;
			m_BurnningTemplate[i].fWalkSpeed *= fSpeedUpRate;
		}
	}

	m_bIsBurnningMode = false;

	//pTagInfo->m_vecHotKeySkill.clear();

	//for ( std::vector< EGCSkillTree >::iterator sit = m_setSkill.begin(); sit != m_setSkill.end(); sit++ )
	//{
 //       GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *sit )->m_pHotKeySkill;
	//	if( info )
	//	{
 //           //pTexture
	//		GCHotKeySkill skill;
	//		skill.m_eSkill = *sit;
	//		skill.pTexture = g_pGCDeviceManager2->CreateTexture( g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *sit )->GetTextureFileName() );
	//		pTagInfo->m_vecHotKeySkill.push_back( skill );
	//	}
	//}

	//if( IsLocalPlayer() == false ) {
	//    return;
	//}

	//if ( Extra_Char_Num_ == -1 || Extra_Char_Num_ != Extra_Char_Num )
	{
		int iChar = 0;
		int iJob = 0;

		if (pTagInfo->Extra_Char_Num == -1) {
			iChar = Extra_Char_Num % GC_CHAR_NUM;
			iJob = Extra_Char_Num / GC_CHAR_NUM;
		}
		else {
			iChar = pTagInfo->Extra_Char_Num % GC_CHAR_NUM;
			iJob = pTagInfo->Extra_Char_Num / GC_CHAR_NUM;
		}

		// 진짜 각성 스킬트리 개편 들어가면 들어내야 하는  부분임.
		if (!g_kGlobalValue.IsCharUseSpecial4(iChar)) {
			bool bCheckRelease = false;
			for (std::map< int, std::vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkill.begin(); mit != m_mapHotKeySkill.end(); ++mit) {
				for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); vit++) {
					vit->pTexture->Release();
					bCheckRelease = true;
				}
			}
			m_mapHotKeySkill.clear();
			m_mapPossibleChargingSkillInfo.clear();

			if (bCheckRelease = false) {
				for (std::vector< GCHotKeySkill >::iterator vit = pTagInfo->m_vecHotKeySkill.begin(); vit != pTagInfo->m_vecHotKeySkill.end(); vit++)
				{
					vit->pTexture->Release();
				}
			}
			pTagInfo->m_vecHotKeySkill.clear();

			m_vecPossibleChargingSkillInfo.clear();

			bool bDefaultSkillSetting = false;
			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL) {
				if (m_setSkill.empty() && (g_kGlobalValue.IsHybridPlayer(iChar) || g_kGlobalValue.IsQuickSkillPlayer(iChar))) {
					std::map< std::pair< int, int >, std::vector< EGCSkillTree > >::iterator mit = m_mapTutorialDefaultEquipSkillA.find(std::make_pair(iChar, iJob));
					if (mit != m_mapTutorialDefaultEquipSkillA.end()) {
						m_setSkill = mit->second;
						bDefaultSkillSetting = true;
					}
				}
			}

			for (std::vector< EGCSkillTree >::iterator sit = m_setSkill.begin(); sit != m_setSkill.end(); sit++)
			{
				const GCSkill* pSkill = SiGCSKT()->GetSkill(*sit);
				if (pSkill && pSkill->m_pHotKeySkill && pSkill->m_pHotKeySkill->m_bNoEquipSlot == false)
				{
					GCHotKeySkill skill;
					skill.m_eSkill = *sit;
					GCHotKeySkillInfo* info = SiGCSKT()->GetSkill(*sit)->m_pHotKeySkill;
					std::vector< GCComboSkill >::iterator vecIter = info->m_vecComboSkill.begin();
					for (; vecIter != info->m_vecComboSkill.end(); ++vecIter)
					{
						vecIter->m_pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(EGCSkillTree(vecIter->m_iSID))->GetTextureFileName());
					}
					skill.pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(*sit)->GetTextureFileName());
					skill.pAddMotionTexture = g_pGCDeviceManager2->CreateTexture(pSkill->GetAddMotionTextureFileName());
					pTagInfo->m_vecHotKeySkill.push_back(skill);
					m_mapHotKeySkill[SKILL_TYPE_NORMAL_A].push_back(skill);
					if (info->m_iChargingSkillLV > 0 && info->m_iChargingSkillLV < 4) {
						m_vecPossibleChargingSkillInfo.push_back(*info);
						m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_A].push_back(*info);
					}
				}
			}

			// 차징가능한 스킬 중 레벨이 높은 순으로 정렬 해주자. 이유는 마나 차징 수치에 따라서 해당 레벨에 맞는 필살기를 써주게 하기 위함이다.
			if (!m_vecPossibleChargingSkillInfo.empty()) {
				std::sort(m_vecPossibleChargingSkillInfo.begin(), m_vecPossibleChargingSkillInfo.end(), KChargingSkillLevelHigh());
			}

			for (std::map< int, std::vector< GCHotKeySkillInfo > >::iterator mit = m_mapPossibleChargingSkillInfo.begin(); mit != m_mapPossibleChargingSkillInfo.end(); ++mit) {
				std::sort(mit->second.begin(), mit->second.end(), KChargingSkillLevelHigh());
			}
		}
	}
}

void PLAYER::SetSkillType(int iSkillType)
{
	// 변신 케릭터는 갈아끼우면 안됨
	if (MetamorphosisForm != FORM_NORMAL)
		return;
	if (g_kGlobalValue.IsQuickSkillPlayer(GetCharType()) ||
		g_kGlobalValue.IsHybridPlayer(GetCharType()) ||
		g_kGlobalValue.IsCharUseSpecial4(GetCharType()))
	{
		return;
	}

	m_iSkillType = iSkillType;

	SetOriginaSpecial(GetCurFormTemplate());

	for (int i = 0; i < FORM_NUM; ++i)
	{
		SetOriginaSpecial(m_FormTemplate[i]);
	}

	SetOriginaSpecial(m_SwapTemplate);

	//GetCurFormTemplate().Init();
	//SetPlayerTemplate( Extra_Char_Num, GetCurFormTemplate() );

	//for( int i = 0; i < FORM_NUM; ++i )
	//    m_FormTemplate[i].Init();
	//SetFormTemplate( Extra_Char_Num, m_FormTemplate );

	m_mapwSkillItem.clear();

	if (m_iSkillType == 1)
	{
		// 라이언4의 2번째 타입의 2단 필살기를 매그너스 단으로 설정하는 부분입니다.
		//if( Extra_Char_Num == CID_RYAN4)
		//{
		//	if( MID_RYAN4_SPECIAL1 == GetCurFormTemplate().SPECIAL1_W )
		//		GetCurFormTemplate().SPECIAL2_W = MID_RYAN4_FORM_START;
		//	else
		//		GetCurFormTemplate().SPECIAL2_W = MID_RYAN4_ONE_HAND_FORM_START;
		//}
		SetPlayerSkill();

		SCharInfo* pCharInfo = &GetCurrentChar();

		if (pCharInfo == NULL)
			g_kGlobalValue.m_strNullFunc = __FUNCTION__;

		KUserItemVector::iterator vit;
		for (vit = pCharInfo->vecItemInfo.begin(); vit != pCharInfo->vecItemInfo.end(); ++vit)
		{
			GCItem* pkItem = NULL;
			if (g_pItemMgr->FindInventory(vit->dwItemUID, &pkItem) == false)
				continue;

			if (pkItem->dwSlotPosition & ESP_A_FINGER)
			{
				if (KGCItemManager::ITEM_FULL_MOON_RING == pkItem->dwGoodsID)
				{
					GetCurFormTemplate().SPECIAL2_W = MID_WOLF_FORM_START;
					GetCurFormTemplate().SPECIAL2_B = MID_WOLF_FORM_START;
				}
				break;
			}
		}
	}
}

//장착하고 있는 아이템이 무엇인가에 따라.. Template을 전환해 줍니다
void PLAYER::SetPlayerSkill()
{
	int iSkillPos = 0;

	if (!IsLocalPlayer())
		return;

	SCharInfo* pCharInfo = &GetCurrentChar();
	if (pCharInfo == NULL)
		g_kGlobalValue.m_strNullFunc = __FUNCTION__;

	for (int i = 0; i < (int)pCharInfo->vecItemInfo.size(); i++)
	{
		GCItem* _Item = NULL;
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
		{
			_Item = g_pItemMgr->GetItemData(pCharInfo->vecItemInfo[i].iItemID);
			CONTINUE_NIL(_Item);
		}
		else if (!g_pItemMgr->FindInventory(pCharInfo->vecItemInfo[i].dwItemUID, &_Item))
			continue;

		//기술장착임
		iSkillPos = -1; //m_iSkillItem에서의 Index를 나타냅니다.

		if (_Item->dwSkillPosition == ESP_SKC_INVALID)
			continue;

		if (Extra_Char_Num < GC_CHAR_NUM)//비전직 캐릭터일 경우....전직 무기를 장착하지 않음
		{
			if (_Item->dwSkillPosition & ESP_SKC_WHITE_ONE)
			{
				GetCurFormTemplate().SPECIAL1_W = _Item->dwTargetSkill;
				iSkillPos = 0;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_WHITE_TWO)
			{
				GetCurFormTemplate().SPECIAL2_W = _Item->dwTargetSkill;
				iSkillPos = 1;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_WHITE_THREE)
			{
				GetCurFormTemplate().SPECIAL3_W = _Item->dwTargetSkill;
				iSkillPos = 2;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_ONE)
			{
				GetCurFormTemplate().SPECIAL1_B = _Item->dwTargetSkill;
				iSkillPos = 3;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_TWO)
			{
				GetCurFormTemplate().SPECIAL2_B = _Item->dwTargetSkill;
				iSkillPos = 4;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_THREE)
			{
				GetCurFormTemplate().SPECIAL3_B = _Item->dwTargetSkill;
				iSkillPos = 5;
			}
		}
		else if (Extra_Char_Num < GC_CHAR_NUM * 2)// 1차 전직 캐릭터인 경우
		{
			if (_Item->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB_ONE)
			{
				GetCurFormTemplate().SPECIAL1_W = _Item->dwTargetSkill;
				iSkillPos = 0;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB_TWO)
			{
				GetCurFormTemplate().SPECIAL2_W = _Item->dwTargetSkill;
				iSkillPos = 1;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB_THREE)
			{
				GetCurFormTemplate().SPECIAL3_W = _Item->dwTargetSkill;
				iSkillPos = 2;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_CHANGEJOB_ONE)
			{
				GetCurFormTemplate().SPECIAL1_B = _Item->dwTargetSkill;
				iSkillPos = 3;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_CHANGEJOB_TWO)
			{
				GetCurFormTemplate().SPECIAL2_B = _Item->dwTargetSkill;
				iSkillPos = 4;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_CHANGEJOB_THREE)
			{
				GetCurFormTemplate().SPECIAL3_B = _Item->dwTargetSkill;
				iSkillPos = 5;
			}
		}
		else
		{
			if (_Item->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB2_ONE)
			{
				GetCurFormTemplate().SPECIAL1_W = _Item->dwTargetSkill;
				iSkillPos = 0;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB2_TWO)
			{
				GetCurFormTemplate().SPECIAL2_W = _Item->dwTargetSkill;
				iSkillPos = 1;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_WHITE_CHANGEJOB2_THREE)
			{
				GetCurFormTemplate().SPECIAL3_W = _Item->dwTargetSkill;
				iSkillPos = 2;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_CHANGEJOB2_ONE)
			{
				GetCurFormTemplate().SPECIAL1_B = _Item->dwTargetSkill;
				iSkillPos = 3;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_CHANGEJOB2_TWO)
			{
				GetCurFormTemplate().SPECIAL2_B = _Item->dwTargetSkill;
				iSkillPos = 4;
			}
			else if (_Item->dwSkillPosition & ESP_SKC_DARK_CHANGEJOB2_THREE)
			{
				GetCurFormTemplate().SPECIAL3_B = _Item->dwTargetSkill;
				iSkillPos = 5;
			}
		}

		if (iSkillPos != -1)
		{
			if (_Item->eItemType == GCIT_COUNT) //수량성이라면 수량을 깎을수 있도록 준비함
				m_mapwSkillItem[std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, iSkillPos)] = pCharInfo->vecItemInfo[i].dwItemUID;
		}
	}
	return;
}

// 슬로우 카운트에 상관없이 항상 측정하기 위해...
void PLAYER::Calculate_Body_Rect()
{
	if (!m_cLife || !m_kUserInfo.CheckState(GC_RUS_PLAYING))
		return;

	if (m_kUserInfo.cCharIndex == -1)
		return;

	RecordBonePosition();

	UINT i_Motion;
	if (IsLocalPlayer())
		i_Motion = g_kLocalPlayerLatency->kRenderData[0].usMotion;
	else
		i_Motion = uiMotion;

	// 모션별 궤적 그리기
	// 슬레쉬 버텍스 위치를 잡아준다.

	// 슬레쉬 버텍스 위치를 잡아준다.
	//v = NULL;
	//v = (SLVERTEX*)V_Slash_Left->Lock( (BYTE*)v );

	if (IsSkipFrameTime())
	{
		goto SkipSlashCalc;
	}

	if (g_kGlobalValue.m_bFrameLock && o3 != 0)
		goto SkipSlashCalc;

	if (IsLocalPlayer())
	{
		Slash[0].AddWithLatency(MyArmor_Ex, MyArmor, m_bDrawSlashRight);
		Slash[1].AddWithLatency(MyLeftArmor_Ex, MyLeftArmor, m_bDrawSlashLeft);
		Slash[2].AddWithLatency(m_posDesc.m_BoneLegLeftBody, m_posDesc.m_BoneLegLeftTip, m_bDrawSlashLegLeft);
		Slash[3].AddWithLatency(m_posDesc.m_BoneLegRightBody, m_posDesc.m_BoneLegRightTip, m_bDrawSlashLegRight);

		//if ( m_iPhysicAtkPower )
		{
			AttackSlash[PAT_SWORD].AddWithLatency(MyArmor_Ex, MyArmor, m_fPhysicAtkPower && m_ePhysicAtkType == PAT_SWORD);
			AttackSlash[PAT_LEG_LEFT].AddWithLatency(m_posDesc.m_BoneLegLeftBody, m_posDesc.m_BoneLegLeftTip, m_fPhysicAtkPower && m_ePhysicAtkType == PAT_LEG_LEFT);
			AttackSlash[PAT_LEG_RIGHT].AddWithLatency(m_posDesc.m_BoneLegRightBody, m_posDesc.m_BoneLegRightTip, m_fPhysicAtkPower && m_ePhysicAtkType == PAT_LEG_RIGHT);
			AttackSlash[PAT_SWORD_LEFT].AddWithLatency(MyLeftArmor_Ex, MyLeftArmor, m_fPhysicAtkPower && m_ePhysicAtkType == PAT_SWORD_LEFT);
			AttackSlash[PAT_FIST_RIGHT].AddWithLatency(m_posDesc.rightHandBody, m_posDesc.rightHandTip, m_fPhysicAtkPower && m_ePhysicAtkType == PAT_FIST_RIGHT);
			AttackSlash[PAT_FIST_LEFT].AddWithLatency(m_posDesc.leftHandBody, m_posDesc.leftHandTip, m_fPhysicAtkPower && m_ePhysicAtkType == PAT_FIST_LEFT);
		}
	}
	else
	{
		Slash[0].Add(MyArmor_Ex, MyArmor, m_bDrawSlashRight);
		Slash[1].Add(MyLeftArmor_Ex, MyLeftArmor, m_bDrawSlashLeft);
		Slash[2].Add(m_posDesc.m_BoneLegLeftBody, m_posDesc.m_BoneLegLeftTip, m_bDrawSlashLegLeft);
		Slash[3].Add(m_posDesc.m_BoneLegRightBody, m_posDesc.m_BoneLegRightTip, m_bDrawSlashLegRight);
	}
SkipSlashCalc:

	//Old_HP를 서서히 떨어뜨리는 부분
	Old_HP += (GetHP() - 0.01f - Old_HP) * 0.07f;

	if (!GetHP() &&
		m_iPlayerIndex == g_MyD3D->Get_MyPlayer() &&
		!o30 &&
		uiMotion != GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) &&
		uiMotion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) &&
		uiMotion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER))
	{
		if (IsFatalGameMode() == false)
		{
			g_KDSound.Play("77");
		}
	}
}
void PLAYER::ChangeSlashColor(int iIndex)
{
	if (0 <= iIndex)
	{
		SLVERTEX v[2];
		v[0].tu = 0;
		v[1].tu = 1.0f;
		v[0].tv = (float)4.0f * iIndex / 64.0f + .02f;
		v[1].tv = v[0].tv;
		Slash[0].SetFullTexture(false);
		Slash[1].SetFullTexture(false);
		Slash[2].SetFullTexture(false);
		Slash[3].SetFullTexture(false);
		Slash[0].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
		Slash[1].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
		Slash[2].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
		Slash[3].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
	}
	else
	{
		SetSlashV();
	}
}
void PLAYER::SetSlashV()
{
	SLVERTEX v[2];
	v[0].tu = 0;
	v[1].tu = 1.0f;

	//-1 이면 궤적으로 FullTexture 사용
	if (GetCurFormTemplate().sPlayerSlash.iIndex != -1)
	{
		switch (m_Elemental)
		{
		case ITEM_ELEMENTAL_FIRE:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[0] / 64.0f + .02f;
		case ITEM_ELEMENTAL_ICE:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[1] / 64.0f + .02f;
		case ITEM_ELEMENTAL_LIGHTNING:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[2] / 64.0f + .02f;
		case ITEM_ELEMENTAL_DARK:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[3] / 64.0f + .02f;
		case ITEM_ELEMENTAL_WIND:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[4] / 64.0f + .02f;
		case ITEM_ELEMENTAL_SHINING:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[5] / 64.0f + .02f;
		case ITEM_ELEMENTAL_LOVE:
		case ITEM_ELEMENTAL_ROSE:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[6] / 64.0f + .02f;
		case ITEM_ELEMENTAL_DEATH:
		case ITEM_ELEMENTAL_TIME:
		case ITEM_ELEMENTAL_LIFE:
		case ITEM_ELEMENTAL_DARK_DRAGON_RED:
		case ITEM_ELEMENTAL_DARK_DRAGON_BLUE:
		case ITEM_ELEMENTAL_FOOLS_DAY:
		case ITEM_ELEMENTAL_HALLOWEEN:
		case ITEM_ELEMENTAL_ELIA:
		case ITEM_ELEMENTAL_PEGASUS_NECK:
		case ITEM_ELEMENTAL_CELESTIAL_MASTER:
		case ITEM_ELEMENTAL_COLISEU:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.aiElemental[7] / 64.0f + .02f;
		default:
			v[0].tv = v[1].tv = (float)4.0f * GetCurFormTemplate().sPlayerSlash.iIndex / 64.0f + .02f;
			break;
		}
		Slash[0].SetFullTexture(false);
		Slash[1].SetFullTexture(false);
		Slash[2].SetFullTexture(false);
		Slash[3].SetFullTexture(false);
	}
	else
	{
		v[0].tu = 0.0f;
		v[0].tv = 0.0f;
		v[1].tu = 1.0f;
		v[1].tv = 1.0f;
		Slash[0].SetFullTexture(true);
		Slash[1].SetFullTexture(true);
		Slash[2].SetFullTexture(true);
		Slash[3].SetFullTexture(true);
	}

	// [3/26/2008] breadceo. .....
	if (Extra_Char_Num == CID_LAS4)
	{
		if (uiMotion == MID_LAS4_SPECIAL3 || uiMotion == MID_LAS4_SPECIAL3_FINISH || uiMotion == MID_LAS4_SPECIAL3_FINISH_LAST)
		{
			if (m_Elemental == ITEM_ELEMENTAL_FIRE || m_Elemental == ITEM_ELEMENTAL_SHINING)
				v[0].tv = v[1].tv = (float)4.0f * 10.0f / 64.0f + .02f;
			else
				v[0].tv = v[1].tv = (float)4.0f * 9.0f / 64.0f + .02f;
		}
	}

	Slash[0].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
	Slash[1].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
	Slash[2].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
	Slash[3].SetTextureUV(v[0].tu, v[0].tv, v[1].tu, v[1].tv);
}

void PLAYER::SetPetName(std::wstring str)
{
	if (m_pObject)
	{
		D3DCOLOR Color = g_MyD3D->IsMyTeam(m_iPlayerIndex) ? 0xFF00FF00 : 0xFFFF0000;
		TagCharacterInfo* pInfo = GetNextTagCharInfo();

		//m_pObject->SetText(3,str,Color);
		if (pInfo && pInfo->m_bNeedDelete == TRUE)
		{
			pInfo->m_pObjMetaForm[FORM_NORMAL]->SetText(3, str, Color);
		}

		for (int i = 0; i < FORM_NUM; ++i)
		{
			if (m_pObjMetaForm[i])
				m_pObjMetaForm[i]->SetText(3, str, Color);
		}

		m_pObject->SetText(3, str, Color);

		m_prPetNameWithColor.first = str;
		m_prPetNameWithColor.second = Color;
	}
}

void PLAYER::Draw_PetName(int x, int y)
{
	if (m_pObject)
	{
		if (m_pObject->IsUseBuildNickNameTex())
			m_pObject->DrawText(3, (float)(x - 128), (float)y);
		else
			g_pkFontMgrOutline->OutTextXY(x, y, m_prPetNameWithColor.first.c_str(), m_prPetNameWithColor.second, NULL, DT_CENTER | DT_TOP);
	}
}

void PLAYER::Draw_UserNickNameInPortal(int x, int y)
{
	if (m_pObject)
		m_pObject->DrawText(1, (float)(x - 128), (float)y);
}

void PLAYER::Draw_UserNickName()
{
	if ((g_pkGameOverlayUI->IsRenderOn() == false) &&
		(g_pkUIScene->m_pkAgitOverlayUI->IsRenderOn() == false))
		return;

	if (!m_cLife ||
		!m_bRenderQuestDrama ||
		!m_bRender ||
		!m_pObject ||
		m_bOffAllEffect)
		return;

	if (UINT_MAX > GetEmbarkUID())
		return;

	D3DXMATRIX mat1, mat2, matBasic;
	Get_BasicMatrix(&matBasic, true, true, true);

	// 한글 이름...
	if (!(Item_Remain_Time[GC_GAME_ITEM_HALLOW] || CheckHiding()) || (g_MyD3D->IsMyTeam(m_iPlayerIndex) && o5 < 3))
	{
		if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW || m_kUserInfo.CheckState(GC_RUS_PLAYING))
		{
			D3DXVECTOR3 vNickNamePos;
			D3DXMatrixTranslation(&mat2, -.15f, -.1f, 0);
			D3DXMatrixMultiply(&mat1, &matBasic, &mat2);

			// NOTE : 운영자가 아니고 x좌표가 0보다 커야지 아이디를 표시해라(궁수활쏘기 대회땜시)
			if (!IsHighRanker() && vPos.x > 0.0f)
			{
				::D3DXVec3TransformCoord(&vNickNamePos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mat1);
				// offset
				vNickNamePos.x += 0.15f;
				vNickNamePos.y += 0.055f; // 좀더 내립니다.
				//vNickNamePos.y += 0.085f;

				// world coordinates -> screen coordinates
				float fScreenX = (vNickNamePos.x + 1.0f) * (GC_SCREEN_FLOAT_WIDTH / 2.0f);
				float fScreenY = (-vNickNamePos.y + 0.75f) * (GC_SCREEN_FLOAT_WIDTH / 2.0f);

				fScreenX *= g_pGCDeviceManager->GetWindowScaleX();
				fScreenY *= g_pGCDeviceManager->GetWindowScaleY();
				if (m_pObject->IsUseBuildNickNameTex())
					m_pObject->DrawText(0, fScreenX - 128, fScreenY);
				else
					g_pkFontMgrOutline->OutTextXY((int)fScreenX, (int)fScreenY, m_prPlayerNameWithColor.first.c_str(), m_prPlayerNameWithColor.second, NULL, DT_CENTER | DT_TOP);

#if defined( _DEBUG ) || !defined( __PATH__ )
				{
					if (g_kGlobalValue.m_bRenderFootHold)
					{
						std::wstringstream strmAIHelper;

						int iIndexX = static_cast<int>(TO_INDEX(vPos.x));
						int iIndexY = static_cast<int>(TO_INDEX(vPos.y));

						strmAIHelper << L"X축 발판 인덱스 : " << iIndexX << std::endl;
						strmAIHelper << L"Y축 발판 인덱스 : " << iIndexY << std::endl;

						fScreenY -= 180.0f;

						g_pkFontMgr->OutTextMultiline(int(fScreenX + 1.0f), int(fScreenY + 1.0f), strmAIHelper.str().c_str(), 0xFFFFFFFF, 1.2f, NULL, DT_CENTER | DT_TOP);
						g_pkFontMgr->OutTextMultiline(int(fScreenX), int(fScreenY), strmAIHelper.str().c_str(), 0xFF00FF00, 1.2f, NULL, DT_CENTER | DT_TOP);
					}
				}
#endif
			}
		}
	}
}

void PLAYER::Draw_GuildMark()
{
	if (!g_pkGameOverlayUI->IsRenderOn() && !g_pkAgitOverlayUI->IsRenderOn())
		return;
	if (!m_cLife ||
		!m_bRenderQuestDrama ||
		!m_bRender ||
		!m_pObject ||
		m_bOffAllEffect)
		return;

	if (m_kUserInfo.iGuildID == 0 || g_kGlobalValue.m_kGuildInfo.m_dwUID == 0 ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME)
		return;

	if (UINT_MAX > GetEmbarkUID())
		return;

	D3DXMATRIX mat1, mat2, matBasic;
	Get_BasicMatrix(&matBasic, true, true, true);

	if (g_pkAgitOverlayUI->IsRenderOn())
	{
		m_kUserInfo.strGuildName = g_kGlobalValue.m_kGuildInfo.m_strName;
	}

	if (!(Item_Remain_Time[GC_GAME_ITEM_HALLOW] || CheckHiding()) || (g_MyD3D->IsMyTeam(m_iPlayerIndex) && o5 < 3))
	{
		if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW || m_kUserInfo.CheckState(GC_RUS_PLAYING))
		{
			D3DXVECTOR3 vNickNamePos;
			D3DXMatrixTranslation(&mat2, -.15f, -.1f, 0);
			D3DXMatrixMultiply(&mat1, &matBasic, &mat2);

			if (!IsHighRanker() && vPos.x > 0.0f)
			{
				::D3DXVec3TransformCoord(&vNickNamePos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mat1);
				// offset
				vNickNamePos.x += 0.15f;
				vNickNamePos.y += 0.055f;

				// world coordinates -> screen coordinates
				float fScreenX = int((vNickNamePos.x + 1.0f) * (GC_SCREEN_FLOAT_WIDTH / 2.0f)) + 0.5f;
				float fScreenY = int((-vNickNamePos.y + 0.75f) * (GC_SCREEN_FLOAT_WIDTH / 2.0f)) + 0.5f;

				fScreenX *= g_pGCDeviceManager->GetWindowScaleX();
				fScreenY *= g_pGCDeviceManager->GetWindowScaleY();
				g_pkFontMgrOutline->OutTextXY((int)fScreenX, (int)fScreenY + (int)(SiKGCMultipleLanguages()->GetDefualtFontSize() * 2.5f), m_kUserInfo.strGuildName.c_str(),
					m_prPlayerNameWithColor.second, NULL, DT_CENTER | DT_TOP);

				float fGuildNameWidth = (float)g_pkFontMgr->GetWidth(m_kUserInfo.strGuildName.c_str());

				g_pGCDeviceManager2->DrawInScreen(m_pGuildMarkTexture,
					fScreenX - (fGuildNameWidth + (SiKGCMultipleLanguages()->GetDefualtFontSize() * 2)), fScreenY + (SiKGCMultipleLanguages()->GetDefualtFontSize() * 2),
					fScreenX + 25.0f - (fGuildNameWidth + (SiKGCMultipleLanguages()->GetDefualtFontSize() * 2)), fScreenY + (SiKGCMultipleLanguages()->GetDefualtFontSize() * 2) + 25.0f,
					0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, 0xffffffff);
			}
		}
	}
}

void PLAYER::Draw_Slash()
{
	D3DXMATRIX mat1, mat2, mat3;

	if (!m_kUserInfo.bLive)
		return;

	if (!m_cLife)
		return;

	if (m_bRenderQuestDrama == false)
		return;

	// 이런게 하나 더 생기면 정리하길 바람 - jemitgge.
	if (m_pRuneShield)
	{
		m_pRuneShield->SetShow(m_bRender);
	}
	if (m_pRuneShield1)
	{
		m_pRuneShield1->SetShow(m_bRender);
	}

	if (m_bRender == false)
		return;

	UINT i_Motion;
	unsigned short i_Frame;
	if (IsLocalPlayer())
	{
		i_Frame = g_kLocalPlayerLatency->kRenderData[0].cFrame;
		i_Motion = g_kLocalPlayerLatency->kRenderData[0].usMotion;
	}
	else {
		i_Motion = uiMotion;
		i_Frame = cFrame;
	}

	D3DXVECTOR3 Vec;
	D3DXMATRIX matBasic;
	Get_BasicMatrix(&matBasic);
	D3DXVec3TransformCoord(&Vec, &D3DXVECTOR3(0, 0, 0), &matBasic);

	if (m_pTexSlash != NULL)
	{
		g_pGCDeviceManager2->PushState();
		{
			//g_pGCDeviceManager2->SetDeviceTextureWithLoad(m_pTexSlash);
			g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			g_pGCDeviceManager2->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			if (Extra_Char_Num == CID_JIN2)
				D3DXMatrixTranslation(&mat2, 0.0f, -.25f, 0.0f);
			else
				D3DXMatrixTranslation(&mat2, 0.0f, -.27f, 0.0f);

			D3DXMatrixMultiply(&mat1, &mat2, &g_kCamera.m_matCamera);

			g_pGCDeviceManager2->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			Slash[0].Render(m_pTexSlash, &mat1);
			Slash[1].Render(m_pTexSlash, &mat1);
			Slash[2].Render(m_pTexSlash, &mat1);
			Slash[3].Render(m_pTexSlash, &mat1);

			if (m_bAttackSwoosh)
			{
				for (int i = 0; i < PAT_NUM; i++)
					AttackSlash[i].Render(NULL, &mat1);
			}

			g_pGCDeviceManager2->FlushWorld();
		}
		g_pGCDeviceManager2->PopState();
	}

	if ((g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_ELESIS ||
		g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_RYAN ||
		(g_MyD3D->GetMyPlayer()->GetCurrentChar().iCharType == GC_CHAR_ARME &&
			(g_kGlobalValue.CheckTrainedSkill(g_MyD3D->GetMyPlayer(), SID_ARME1_SPIRIT_RECOVERY_LV1) ||
				g_kGlobalValue.CheckTrainedSkill(g_MyD3D->GetMyPlayer(), SID_ARME1_SPIRIT_RECOVERY_LV2) ||
				g_kGlobalValue.CheckTrainedSkill(g_MyD3D->GetMyPlayer(), SID_ARME1_SPIRIT_RECOVERY_LV3)))) &&
		IsLocalPlayer())
	{
		// 폭주 게이지를 보여준다.
		if (g_MyD3D->MyHead->SuperPoint > .3f)
		{
			g_pGCDeviceManager2->PushState();
			{
				float SuperPoint = g_MyD3D->MyHead->SuperPoint;
				if (SuperPoint > 1.0f) SuperPoint = 1.0f;

				g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				D3DXMatrixTranslation(&mat2, Vec.x - .083f, Vec.y - .066f + .15f * (SuperPoint - .5f), 1.0f);

				if (o3 < 2)
				{
					g_pGCDeviceManager2->DrawInWorld(NULL, &mat2,
						.012f, .15f * (SuperPoint - .5f) * 2.0f, 0, 0, 0, 0, 0, 0, 0,
						D3DCOLOR_ARGB(255, 255, 100, 0));
				}
				else
				{
					g_pGCDeviceManager2->DrawInWorld(NULL, &mat2,
						.012f, .15f * (SuperPoint - .5f) * 2.0f, 0, 0, 0, 0, 0, 0, 0,
						D3DCOLOR_ARGB(255, 0, 0, 0));
				}

				if (GetCurrentChar().iCharType == GC_CHAR_ELESIS)
				{
				}
				g_pGCDeviceManager2->PushState();
				{
					g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

					D3DXMatrixTranslation(&mat2, Vec.x - .084f, Vec.y - .069f, 1.0f);
					g_pGCDeviceManager2->DrawInWorld(m_pTexBerserk1, &mat2,
						.047f, .064f, .4f, 0, 1.0f, 1.0f);

					D3DXMatrixTranslation(&mat2, Vec.x - .083f, Vec.y + .024f, 1.0f);
					g_pGCDeviceManager2->DrawInWorld(m_pTexBerserk1, &mat2,
						.033f, .125f, 0, 0, .4f, 1.0f);
				}
				g_pGCDeviceManager2->PopState();
			}
			g_pGCDeviceManager2->PopState();
		}
	}

	// 폭주 중인지 검사한다.
	if (uiMotion == MID_ELESIS123_BERSERK && cFrame > 29 && cFrame < 44)
	{
		D3DXMatrixTranslation(&mat2, Vec.x + .05f, Vec.y + .326f, 1.0f);
		g_pGCDeviceManager2->DrawInWorld(m_pTexBerserk2, &mat2,
			.555f, .998f, 0, 0, 1.0f, 1.0f);
	}

	if (uiMotion == MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY && cFrame > 29 && cFrame < 44)
	{
		D3DXMatrixTranslation(&mat2, Vec.x, Vec.y + .326f, 1.0f);
		g_pGCDeviceManager2->DrawInWorld(m_pTexBerserk2, &mat2,
			.8f, .8f, 0, 0, 1.0f, 1.0f);
	}

	if (!(Item_Remain_Time[GC_GAME_ITEM_HALLOW] || CheckHiding()) || (g_MyD3D->IsMyTeam(m_iPlayerIndex) && o5 < 3))
	{
		if (Extra_Char_Num == CID_ARME1 && i_Frame < 50 &&
			(i_Motion == GetCurFormTemplate().SPECIAL1_W ||
				i_Motion == GetCurFormTemplate().SPECIAL2_W ||
				i_Motion == GetCurFormTemplate().SPECIAL3_W ||
				i_Motion == GetCurFormTemplate().SPECIAL1_B ||
				i_Motion == GetCurFormTemplate().SPECIAL2_B ||
				i_Motion == GetCurFormTemplate().SPECIAL3_B))
		{
			if (g_MyD3D->MySparks->m_pTexSpark[FLASH] != NULL)
			{
				//마법사 필살기 쓸때 지팡이에 붙는 Spark
				//SLVERTEX *v = NULL;

				g_pGCDeviceManager2->PushState();
				g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				float Big = (float)rand() / (float)RAND_MAX + .5f;
				D3DXMatrixScaling(&mat2, Big, Big, 1.0f);
				D3DXMatrixRotationZ(&mat3, (float)i_Frame * .01f);

				D3DXMatrixMultiply(&mat1, &mat2, &mat3);
				//v = (SLVERTEX*)V_Slash->Lock( (BYTE*)v );
				D3DXVECTOR3 vTipPos = m_posDesc.staffTip;//Slash[0].GetVertexPosByIndex(6);
				D3DXMatrixTranslation(&mat2, vTipPos.x, vTipPos.y, vTipPos.z);
				//V_Slash->Unlock();
				D3DXMatrixMultiply(&mat3, &mat1, &mat2);
				D3DXMatrixMultiply(&mat1, &mat3, &g_kCamera.m_matCamera);

				g_pGCDeviceManager2->DrawInWorld(g_MyD3D->MySparks->m_pTexSpark[FLASH], &mat1,
					.2f, .2f, 0, 0, 1.0f, 1.0f);

				g_pGCDeviceManager2->PopState();
			}
		}

		// 쉴드를 보여줌
		if (Shield && m_bSpecialShield == false)
		{
			int iTexNum = -1;

			switch (m_cUseShield)
			{
			case CID_COMMON:
			{
				if (m_pRuneShield == NULL || g_ParticleManager->IsLiveInstance(m_pRuneShield) == false)
				{
					std::string strParticleName = "Pet_Buzzy_Barrier_01";

					if (FORM_NORMAL != MetamorphosisForm)
						strParticleName = "Pet_Buzzy_Barrier_01_Big";

					m_pRuneShield = g_ParticleManager->CreateSequence(strParticleName, 0, 0, 0.5f);

					TRACE_INFO stTrace;
					if (m_pRuneShield) {
						stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
						stTrace.m_fParticleTraceTime = 5.0f;
						m_pRuneShield->SetTrace(&stTrace);
					}
				}
			}
			break;
			case CID_RONAN3:
			{
				if (m_pRuneShield == NULL || g_ParticleManager->IsLiveInstance(m_pRuneShield) == false)
				{
					m_pRuneShield = g_ParticleManager->CreateSequence("Ronan3_special2-2_Shield01", 0, 0, 0.5f);

					TRACE_INFO stTrace;
					if (m_pRuneShield) {
						stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
						stTrace.m_fParticleTraceTime = 5.0f;
						m_pRuneShield->SetTrace(&stTrace);
					}
				}
				if (m_pRuneShield1 == NULL || g_ParticleManager->IsLiveInstance(m_pRuneShield1) == false)
				{
					m_pRuneShield1 = g_ParticleManager->CreateSequence("Ronan3_special2-2_Shield02", 0, 0, 0.5f);

					TRACE_INFO stTrace;
					if (m_pRuneShield1) {
						stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
						stTrace.m_fParticleTraceTime = 5.0f;
						m_pRuneShield1->SetTrace(&stTrace);
					}
				}
			}
			break;
			case CID_RYAN1:
				break;
			case CID_MARI4:
				if (m_pRuneShield == NULL || g_ParticleManager->IsLiveInstance(m_pRuneShield) == false)
				{
					m_pRuneShield = g_ParticleManager->CreateSequence("Ronan3_special2-2_Shield01", 0, 0, 0.5f);

					TRACE_INFO stTrace;
					if (m_pRuneShield) {
						stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
						stTrace.m_fParticleTraceTime = 5.0f;
						m_pRuneShield->SetTrace(&stTrace);
					}
				}
				if (m_pRuneShield1 == NULL || g_ParticleManager->IsLiveInstance(m_pRuneShield1) == false)
				{
					m_pRuneShield1 = g_ParticleManager->CreateSequence("Ronan3_special2-2_Shield02", 0, 0, 0.5f);

					TRACE_INFO stTrace;
					if (m_pRuneShield1) {
						stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
						stTrace.m_fParticleTraceTime = 5.0f;
						m_pRuneShield1->SetTrace(&stTrace);
					}
				}
				break;
			case CID_ARME1:
			{
				if (!m_bIsCountAttackShield)
				{
					if (m_pRuneShield != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield) == true)
					{
						g_ParticleManager->DeleteSequence(m_pRuneShield);
						m_pRuneShield = NULL;
					}
					if (m_pRuneShield1 != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield1) == true)
					{
						g_ParticleManager->DeleteSequence(m_pRuneShield1);
						m_pRuneShield1 = NULL;
					}

					if (g_MyD3D->m_pDamageManager->m_apTexture[GC_TEX_WITCH_SHIELD] != NULL) {
#if defined (MATCH_BALLENCE_121217)
						if (m_iShieldTexNum == -1) {
							iTexNum = GC_TEX_ARME_MAGIC_SHILED_LV1;
						}
						else {
							iTexNum = m_iShieldTexNum;
						}
#else
						iTexNum = GC_TEX_WITCH_SHIELD;
#endif
					}
				}
				else
				{
					if (m_pRuneShield == NULL || g_ParticleManager->IsLiveInstance(m_pRuneShield) == false)
					{
						m_pRuneShield = g_ParticleManager->CreateSequence("Arme01_Protection_from_Fire_01", 0, 0, 0.5f);

						TRACE_INFO stTrace;
						stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
						stTrace.m_fParticleTraceTime = 5.0f;
						m_pRuneShield->SetTrace(&stTrace);
					}
					if (m_pRuneShield1 == NULL || g_ParticleManager->IsLiveInstance(m_pRuneShield1) == false)
					{
						m_pRuneShield1 = g_ParticleManager->CreateSequence("Arme01_Protection_from_Fire_02", 0, 0, 0.5f);

						TRACE_INFO stTrace;
						stTrace.m_pvPos = ComputePosDesc(m_posDesc, EPT_BODY_CENTER);
						stTrace.m_fParticleTraceTime = 5.0f;
						m_pRuneShield1->SetTrace(&stTrace);
					}
				}
			}
			break;
			case CID_ARME2:
				if (m_pRuneShield != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield) == false)
				{
					g_ParticleManager->DeleteSequence(m_pRuneShield);
					m_pRuneShield = NULL;
				}
				if (m_pRuneShield1 != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield1) == false)
				{
					g_ParticleManager->DeleteSequence(m_pRuneShield1);
					m_pRuneShield1 = NULL;
				}

				if (g_MyD3D->m_pDamageManager->m_apTexture[GC_TEX_WITCH_SHIELD] != NULL) {
					if (m_iShieldTexNum == -1) {
						iTexNum = GC_TEX_ARME_MAGIC_SHILED_LV1;
					}
					else {
						iTexNum = m_iShieldTexNum;
					}
				}
				break;
			case CID_MAX:
			{
				if (m_pRuneShield != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield) == true)
				{
					g_ParticleManager->DeleteSequence(m_pRuneShield);
					m_pRuneShield = NULL;
				}
				if (m_pRuneShield1 != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield1) == true)
				{
					g_ParticleManager->DeleteSequence(m_pRuneShield1);
					m_pRuneShield1 = NULL;
				}
			}
			break;
			default:
			{
				// 먼저 걸려있던 쉴드는 없애쟈!
				if (m_pRuneShield != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield) == true)
				{
					g_ParticleManager->DeleteSequence(m_pRuneShield);
					m_pRuneShield = NULL;
				}
				if (m_pRuneShield1 != NULL && g_ParticleManager->IsLiveInstance(m_pRuneShield1) == true)
				{
					g_ParticleManager->DeleteSequence(m_pRuneShield1);
					m_pRuneShield1 = NULL;
				}
			}
			break;
			}
			if (iTexNum != -1 /*&& g_MyD3D->m_pDamageManager->m_apTexture[GC_TEX_WITCH_SHIELD] != NULL*/)
			{
				D3DXMatrixTranslation(&mat2, 0, 0, -.5f);

				//D3DXMatrixMultiply( &mat1, &Get_BasicMatrix(), &mat2 );
				D3DXMatrixMultiply(&mat1, &matBasic, &mat2);

				//g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1);
				bool Right;
				if (IsLocalPlayer())
					Right = g_kLocalPlayerLatency->kRenderData[0].bIsRight;
				else Right = bIsRight;

				if (Right)
				{
					if (o10 < 10)
						g_pGCDeviceManager2->DrawInWorld(g_MyD3D->m_pDamageManager->m_apTexture[iTexNum], &mat1,
							1.2f + (float)o10 * .01f, 1.2f + (float)o10 * .01f, 0, 0, 1.0f, 1.0f);
					//		g_MyD3D->Draw_Billboard(1.2f+(float)o10*.01f,1.2f+(float)o10*.01f,0,0,1.0f,1.0f);
					else
						g_pGCDeviceManager2->DrawInWorld(g_MyD3D->m_pDamageManager->m_apTexture[iTexNum], &mat1,
							1.3f - (float)o10 * .01f, 1.3f - (float)o10 * .01f, 0, 0, 1.0f, 1.0f);
					//		g_MyD3D->Draw_Billboard(1.3f-(float)o10*.01f,1.3f-(float)o10*.01f,0,0,1.0f,1.0f);
				}
				else
				{
					if (o10 < 10)
						g_pGCDeviceManager2->DrawInWorld(g_MyD3D->m_pDamageManager->m_apTexture[iTexNum], &mat1,
							-1.2f - (float)o10 * .01f, 1.2f + (float)o10 * .01f, 0, 0, 1.0f, 1.0f);
					//		g_MyD3D->Draw_Billboard(-1.2f-(float)o10*.01f,1.2f+(float)o10*.01f,0,0,1.0f,1.0f);
					else
						g_pGCDeviceManager2->DrawInWorld(g_MyD3D->m_pDamageManager->m_apTexture[iTexNum], &mat1,
							-1.3f + (float)o10 * .01f, 1.3f - (float)o10 * .01f, 0, 0, 1.0f, 1.0f);
					//		g_MyD3D->Draw_Billboard(-1.3f+(float)o10*.01f,1.3f-(float)o10*.01f,0,0,1.0f,1.0f);
				}
			}
		}
		else
		{
			if (m_pRuneShield != NULL)
			{
				g_ParticleManager->DeleteSequence(m_pRuneShield);
				m_pRuneShield = NULL;
			}
			if (m_pRuneShield1 != NULL)
			{
				g_ParticleManager->DeleteSequence(m_pRuneShield1);
				m_pRuneShield1 = NULL;
			}
		}
	}

	if (IsLocalPlayer() && g_MyD3D->MyCtrl->Energy > 0.4f)
	{
		g_pGCDeviceManager2->PushState();
		g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		GCDeviceTexture* tex = NULL;
		if (o6 < 4)
		{
			if (g_MyD3D->MySparks->m_pTexSpark[ENERGY_FOCUS0] != NULL)
			{
				tex = g_MyD3D->MySparks->m_pTexSpark[ENERGY_FOCUS0];
			}
		}
		else if (o6 < 8)
		{
			if (g_MyD3D->MySparks->m_pTexSpark[ENERGY_FOCUS1] != NULL)
			{
				tex = g_MyD3D->MySparks->m_pTexSpark[ENERGY_FOCUS1];
			}
		}
		else
		{
			if (g_MyD3D->MySparks->m_pTexSpark[ENERGY_FOCUS2] != NULL)
			{
				tex = g_MyD3D->MySparks->m_pTexSpark[ENERGY_FOCUS2];
			}
		}

		if (bIsRight)
		{
			g_pGCDeviceManager2->DrawInWorld(tex, &matBasic,
				.8f, .8f, 0, 0, 1.0f, 1.0f);
		}
		else {
			D3DXMatrixRotationY(&mat1, 3.1416f);
			D3DXMatrixMultiply(&mat2, &mat1, &matBasic);

			g_pGCDeviceManager2->DrawInWorld(tex, &mat2,
				.8f, .8f, 0, 0, 1.0f, 1.0f);
		}
		g_pGCDeviceManager2->PopState();
	}

#ifndef MOVIE
	// 같은 팀원은 머리위에 세모를 그려준다. 팀마크
	//if(!g_MyD3D->Survival_Mode)
	if (m_bRender == true)
	{
		if (SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT)
		{
			if (!(Item_Remain_Time[GC_GAME_ITEM_HALLOW] || CheckHiding()))
			{
				D3DXMatrixTranslation(&mat2, .026f, .59f - (float)o10 * .003f, 0);
				D3DXMatrixMultiply(&mat1, &mat2, &matBasic);
				if (!bIsRight)
				{
					D3DXMatrixRotationY(&mat3, 3.1416f + (float)o10 * .0571f);
					D3DXMatrixMultiply(&mat2, &mat3, &mat1);
				}
				else
				{
					D3DXMatrixRotationY(&mat3, (float)o10 * .0571f);
					D3DXMatrixMultiply(&mat2, &mat3, &mat1);
				}

				if (IsLocalPlayer())
				{
					g_pGCDeviceManager2->DrawInWorld(g_pkUIMgr->GetDeviceTexture(3), &mat2,
						.158f, .163f, 0.0009765625f, 0.7626953125f, 0.0654296875f, 0.8583984375f, 0, 0, 0,
						D3DCOLOR_ARGB(255, 255, 0, 0));
				}
				else if (g_MyD3D->IsMyTeam(m_iPlayerIndex))
				{
					g_pGCDeviceManager2->DrawInWorld(g_pkUIMgr->GetDeviceTexture(3), &mat2,
						.158f, .163f, 0.0009765625f, 0.7626953125f, 0.0654296875f, 0.8583984375f, 0, 0, 0,
						D3DCOLOR_ARGB(255, 0, 255, 0));
				}

				// 대장전 ( 머리위에 화살표 ㅋㅋㅋ )
				if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN)
				{
					switch (((KGCGuildMatch_CapTain*)((KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr(GS_GAME))->GetGameModePtr(GC_GMC_CAPTAIN))->IsCaptainIndex(m_iPlayerIndex))
					{
					case TEAM_SERDIN:
						g_pGCDeviceManager2->DrawInWorld(m_pSerdinSign, &mat2,
							.5f, .6f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0);
						break;
					case TEAM_CANABAN:
						g_pGCDeviceManager2->DrawInWorld(m_pCanabanSign, &mat2,
							.5f, .6f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0);
						break;
					case -1:
						break;
					}
				}
			}
		}
		else
		{
			if (!(Item_Remain_Time[GC_GAME_ITEM_HALLOW] || CheckHiding()))
			{
				D3DXMatrixTranslation(&mat2, .026f, .59f - (float)o10 * .003f, 0);
				D3DXMatrixMultiply(&mat1, &mat2, &matBasic);
				if (!bIsRight)
				{
					D3DXMatrixRotationY(&mat3, 3.1416f + (float)o10 * .0571f);
					D3DXMatrixMultiply(&mat2, &mat3, &mat1);
				}
				else
				{
					D3DXMatrixRotationY(&mat3, (float)o10 * .0571f);
					D3DXMatrixMultiply(&mat2, &mat3, &mat1);
				}

				if (IsLocalPlayer())
				{
					g_pGCDeviceManager2->DrawInWorld(g_pkUIMgr->GetDeviceTexture(3), &mat2,
						.158f, .163f, 0.0009765625f, 0.7626953125f, 0.0654296875f, 0.8583984375f, 0, 0, 0,
						D3DCOLOR_ARGB(255, 255, 0, 0));
				}
			}
		}
	}

	// 한글 이름...
	if (!(Item_Remain_Time[GC_GAME_ITEM_HALLOW] || CheckHiding()) || (g_MyD3D->IsMyTeam(m_iPlayerIndex) && o5 < 3))
	{
		if (SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL || g_MyD3D->MyReplay->m_iCurrState == RP_VIEW || g_pGameBoard->m_pStartingMember[m_iPlayerIndex].dwUID == m_kUserInfo.dwUID)
		{
			// 길드 마크를 그린다.
			// NOTE : 리플레이와 튜토리얼에서는 길드 마크 표시를 하지 않는다.
			if (m_kUserInfo.iGuildID > -1 && g_MyD3D->m_KGCOption.Is3Back() &&
				g_MyD3D->MyReplay->m_iCurrState != RP_VIEW &&
				SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
			{
				if (g_kGlobalValue.m_kUserInfo.iGuildID > -1) {
					//g_MyD3D->m_KGCGuildMark.Render(
					//m_kUserInfo.aiGuildMark[0],
					//m_kUserInfo.aiGuildMark[1],
					//m_kUserInfo.aiGuildMark[2],
					//Vec.x-.143f, Vec.y-.129f, .093f);
				}
			}

			// Fatal 표시
			if (!GetHP() && !m_bOffAllEffect)
			{
				if (uiMotion != GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) && uiMotion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) && uiMotion != GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER))
				{
					D3DXMatrixTranslation(&mat1, Vec.x, Vec.y, .2f);

					if (o10 < 10)
					{
						g_pGCDeviceManager2->DrawInWorld(m_pTexFatal, &mat1,
							.2f + (float)o10 * .005f, .04f + (float)o10 * .001f, 0, .3f, 1.0f, .7f);
					}
					else
					{
						g_pGCDeviceManager2->DrawInWorld(m_pTexFatal, &mat1,
							.2f + (float)o10 * .005f, .04f + (float)o10 * .001f, 0, .3f, 1.0f, .7f);
					}
				}
			}
		}
	}
#endif

#if defined( _DEBUG ) || !defined( __PATH__ )

	if (g_kGlobalValue.m_bRenderBoundBox && UINT_MAX == GetEmbarkUID())
	{
		//몸통 바운드 그리기
		GCCollisionRect<float> fRect = GetCollisionRect();
		D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
		SETVECTOR3(lefttop, fRect.m_Left, fRect.m_Top - 0.2f, 0.4f);
		SETVECTOR3(righttop, fRect.m_Right, fRect.m_Top - 0.2f, 0.4f);
		SETVECTOR3(leftbottom, fRect.m_Left, fRect.m_Bottom - 0.2f, 0.4f);
		SETVECTOR3(rightbottom, fRect.m_Right, fRect.m_Bottom - 0.2f, 0.4f);

		g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
			lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
			D3DCOLOR_ARGB(180, 0, 0, 200));

		// 오른쪽 칼의 바운드 그리기
		fRect.SetRect(MyArmor.x, MyArmor.y, MyArmor_Ex.x, MyArmor_Ex.y);
		fRect.AdjustRect();
		SETVECTOR3(lefttop, fRect.m_Left, fRect.m_Top - 0.3f, 0.4f);
		SETVECTOR3(righttop, fRect.m_Right, fRect.m_Top - 0.3f, 0.4f);
		SETVECTOR3(leftbottom, fRect.m_Left, fRect.m_Bottom - 0.3f, 0.4f);
		SETVECTOR3(rightbottom, fRect.m_Right, fRect.m_Bottom - 0.3f, 0.4f);
		g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
			lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
			D3DCOLOR_ARGB(180, 200, 0, 0));

		// 왼쪽 칼의 바운드 그리기
		fRect.SetRect(MyLeftArmor.x, MyLeftArmor.y, MyLeftArmor_Ex.x, MyLeftArmor_Ex.y);
		fRect.AdjustRect();
		SETVECTOR3(lefttop, fRect.m_Left, fRect.m_Top - 0.3f, 0.4f);
		SETVECTOR3(righttop, fRect.m_Right, fRect.m_Top - 0.3f, 0.4f);
		SETVECTOR3(leftbottom, fRect.m_Left, fRect.m_Bottom - 0.3f, 0.4f);
		SETVECTOR3(rightbottom, fRect.m_Right, fRect.m_Bottom - 0.3f, 0.4f);
		g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
			lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
			D3DCOLOR_ARGB(180, 0, 200, 0));

		//칼의 위치 그리기
		D3DXMATRIX matCamera = g_kCamera.m_matCamera;
		D3DXMATRIX matWeapon;

		// Right
		D3DXMatrixTranslation(&matWeapon, MyArmor.x, MyArmor.y, 0.4f);
		matWeapon = matWeapon * matCamera;
		g_pGCDeviceManager2->DrawInWorld(NULL, &matWeapon,
			0.025f, 0.025f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0,
			D3DCOLOR_ARGB(100, 100, 0, 0));

		D3DXMatrixTranslation(&matWeapon, MyArmor_Ex.x, MyArmor_Ex.y, 0.4f);
		matWeapon = matWeapon * matCamera;
		g_pGCDeviceManager2->DrawInWorld(NULL, &matWeapon,
			0.025f, 0.025f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0,
			D3DCOLOR_ARGB(100, 100, 0, 0));

		// Left
		D3DXMatrixTranslation(&matWeapon, MyLeftArmor.x, MyLeftArmor.y, 0.4f);
		matWeapon = matWeapon * matCamera;
		g_pGCDeviceManager2->DrawInWorld(NULL, &matWeapon,
			0.025f, 0.025f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0,
			D3DCOLOR_ARGB(100, 100, 0, 0));

		D3DXMatrixTranslation(&matWeapon, MyLeftArmor_Ex.x, MyLeftArmor_Ex.y, 0.4f);
		matWeapon = matWeapon * matCamera;
		g_pGCDeviceManager2->DrawInWorld(NULL, &matWeapon,
			0.025f, 0.025f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 0,
			D3DCOLOR_ARGB(100, 100, 0, 0));
	}

#endif

	D3DXMatrixIdentity(&mat1);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mat1);
}

void PLAYER::Add_Face()
{
#if !defined( __PATH__ )
	if (g_MyD3D->IsGifRecordMode())
		return;
#endif

	if (!Silk && g_MyD3D->m_KGCOption.Is3Back())
	{
		bool Continue = false;
		if (g_kCamera.m_iTargetPlayer == m_iPlayerIndex)
			Continue = true;
		else
		{
			if (D3DXVec2Length(&D3DXVECTOR2(g_MyD3D->MyPlayer[g_kCamera.m_iTargetPlayer]->vPos.x - vPos.x,
				g_MyD3D->MyPlayer[g_kCamera.m_iTargetPlayer]->vPos.y - vPos.y)) < 1.5f)
				Continue = true;
		}
		if (Continue)
		{
			Silk = 1;
			m_bSilkRight = bIsRight;
		}
	}
}

void PLAYER::Process_Face()
{
	if (!Silk) return;

	//2007/07/16 milennium9 면상은 전부 스크립트로 빼 버렸어요.
	if (GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName.compare("NULL") != 0)
	{
		if (Silk == 1) {
			m_fEff_x = GetCurFormTemplate().sPlayerSpecialFace.avFacePosition[0].x;
			m_fEff_y = GetCurFormTemplate().sPlayerSpecialFace.avFacePosition[0].y;
		}
		else if (Silk < 45) {
			m_fEff_x += ((GetCurFormTemplate().sPlayerSpecialFace.avFacePosition[1].x - m_fEff_x) * .2f);
			m_fEff_y += ((GetCurFormTemplate().sPlayerSpecialFace.avFacePosition[1].y - m_fEff_y) * .2f);
		}
		else {
			m_fEff_x += ((GetCurFormTemplate().sPlayerSpecialFace.avFacePosition[2].x - m_fEff_x) * .2f);
			m_fEff_y += ((GetCurFormTemplate().sPlayerSpecialFace.avFacePosition[2].y - m_fEff_y) * .2f);
		}
	}
	else
	{
		if (Silk == 1)
		{
			m_fEff_x = 0.0f;
			m_fEff_y = 0.0f;
		}
		else
		{
			m_fEff_x *= 0.2f;
			m_fEff_y *= 0.2f;
		}
	}
	++Silk;
	if (Silk > 70) Silk = 0;
}

void PLAYER::Draw_Face()// 극대화된 캐릭터의 얼굴을 보여준다. (현재 Unable)
{
#if !defined( __PATH__ )
	if (g_MyD3D->IsGifRecordMode())
		return;
#endif

	if (!g_MyD3D->m_KGCOption.IsSkillEffect())
		return;

	if (!Silk) return;
	// 임시로 캐릭터 이펙트 보기

	int iEff_x = (int)m_fEff_x;
	int iEff_y = (int)m_fEff_y;

	GCDeviceTexture* texsp = NULL;

	std::string TempCutIn = g_MyD3D->m_kCutInSkinMgr.GetCutInSkinTexture(m_iPlayerIndex);
	if (!TempCutIn.empty())
		texsp = g_pGCDeviceManager2->CreateTexture(TempCutIn);

	if (Extra_Char_Num == CID_AMY4 && texsp == NULL)
	{
		if (uiMotion == MID_AMY4_SPECIAL1)
			texsp = m_pAmy4Action1;
		else if (uiMotion == MID_AMY4_SPECIAL2)
			texsp = m_pAmy4Action2;
		else if (uiMotion == MID_AMY4_SPECIAL3)
			texsp = m_pAmy4Action3;
	}

	// 누가 자동으로 계산좀 ㅡㅡ;;;;;;
#define PIXELTOTEXEL(p) ( (p)/512.0f )

	if (GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName.compare("NULL") != 0)
	{
		if (texsp == NULL)
			texsp = m_pPlayerSp;

		if (m_bSilkRight)
		{
			int iSize = (int)GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY.size();
			for (int i = 0; i < iSize; ++i)
			{
				g_pGCDeviceManager2->DrawInScreenResize(texsp,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vRScreenLeftTop.x + iEff_x) * GC_SCREEN_DIV_WIDTH,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vRScreenLeftTop.y + iEff_y) * GC_SCREEN_DIV_WIDTH,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vRScreenRightButtom.x + iEff_x) * GC_SCREEN_DIV_WIDTH,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vRScreenRightButtom.y + iEff_y) * GC_SCREEN_DIV_WIDTH,
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureLeftTop.x),
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureLeftTop.y),
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureRightButtom.x),
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureRightButtom.y),
					false, 10.0f, D3DCOLOR_ARGB(215, 215, 215, 215));
			}
		}
		else
		{
			int iSize = (int)GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY.size();
			for (int i = 0; i < iSize; ++i)
			{
				g_pGCDeviceManager2->DrawInScreenResize(texsp,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vLScreenLeftTop.x - iEff_x) * GC_SCREEN_DIV_WIDTH,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vLScreenLeftTop.y + iEff_y) * GC_SCREEN_DIV_WIDTH,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vLScreenRightButtom.x - iEff_x) * GC_SCREEN_DIV_WIDTH,
					(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vLScreenRightButtom.y + iEff_y) * GC_SCREEN_DIV_WIDTH,
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureRightButtom.x),
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureLeftTop.y),
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureLeftTop.x),
					PIXELTOTEXEL(GetCurFormTemplate().sPlayerSpecialFace.vecFaceUVXY[i].vTextureRightButtom.y),
					false, 10.0f, D3DCOLOR_ARGB(215, 215, 215, 215));
			}
		}
	}
#undef PIXELTOTEXEL
}

void PLAYER::Arena()
{
	int iChar = (int)m_kUserInfo.GetCurrentChar().iCharType;
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.find(iChar);
		if (mit == m_mapTagCharInfo.end())
		{
			ASSERT(!"바꾸려는 Character 없음");
		}
		TagCharacterInfo& tagInfo = mit->second;
		if (tagInfo.m_cLife == 0)
		{
			iChar = GetNextTagChar();

			// 없으면
			if (iChar == GC_CHAR_INVALID)
				return;
		}

		if (IsMagicEffect(EGC_SIEG1_SKILL_LIVING_FREE, true))
			ClearMagicEffect(EGC_SIEG1_SKILL_LIVING_FREE);
		// Life > 0  이면 그녀석이 나오면 됨
	}
	else
	{
		if (m_cLife == 0)
			return;
	}

	SetEPlayerState(PS_ALIVE);

	if (g_MyD3D->MyStg->GetStage() == GC_GS_TEMPLE_OF_FIRE)
	{
		g_MyD3D->MyStg->m_bDeathScrollY = false;
	}

	g_KDSound.Play("80");
	g_MyD3D->MyHead->Start_Text("ARENA");

	if (SiKGCRoomManager()->GetGameMode() == GC_GM_INDIGO_TEAM)
	{
		if (m_vecCharNum.size() > 0)
		{
			iChar = m_vecCharNum[0];
			m_vecCharNum.erase(m_vecCharNum.begin());
		}
	}

	Slow_Count = 0;

	// 탑승물 아레나 셋팅확인
	// 탑승물 종류
	// g_MyD3D->MyPlayer[i]->CreatePlayer( bLoad );
	Calculate_Arena_Pos();

	LTP_ARENA basicPacket;
	basicPacket.latencyTime = 0.0f;
	basicPacket.packetType = ELTP_GAME_ARENA;
	basicPacket.size = sizeof(LTP_ARENA);
	basicPacket.m_iChar = iChar;
	g_LatencyTransfer.PushPacket(&basicPacket);

	bool m_bZoom = true;
	if (m_kEmbarkArena.IsSetting() && false == m_kEmbarkArena.m_bUseArenaZoom)
		m_bZoom = false;

	if (m_bZoom)
		g_kCamera.SetZoom(2.0f, g_MyD3D->Get_MyPlayer());

#ifndef __PATH__
	static int iArenaTime = 0;
	{
		std::stringstream stm;
		FILE* fp;

		stm << "log" << iArenaTime << ".txt";
		fp = fopen(stm.str().c_str(), "w");
		fwrite(TestString.c_str(), 1, TestString.size(), fp);

		fclose(fp);

		TestString.clear();
	}

	iArenaTime++;

#endif
}

void PLAYER::ArenaToInitPlayer(int iArenaCharType)
{
	// 아레나 해야되는데 태그가 아니고 변신상태면 풀어주자
	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TAG_MATCH)
	{
		if (MetamorphosisForm != FORM_NORMAL)
			ProcessMetamorphosisNormalForm(true);
	}

	// 태그매치용 케릭터 얼굴 갱신
	if (IsLocalPlayer())
	{
		if (SiKGCRoomManager()->GetGameMode() == GC_GM_INDIGO_TEAM)
			g_pkHUD->m_pMyHeadup->CharFaceLock(m_kUserInfo.GetCurrentChar().iCharType);
		else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
		{
			if (m_cLife <= 0)
				g_pkHUD->m_pMyHeadup->CharFaceLock(m_kUserInfo.GetCurrentChar().iCharType);
		}
	}

	// 이전 케릭터와 다를때 새로 계산합니다.
	bool bLoad = false;
	if (m_kUserInfo.GetCurrentChar().iCharType != iArenaCharType)
	{
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
		{
			TagCharacter(true);
			KGC_PID_TAG_CHARACTER kTagInfo;
			kTagInfo.bArena = true;
			kTagInfo.cPlayerIndex = static_cast<char>(m_iPlayerIndex);
			g_MyD3D->MyReplay->Set_TagCharacter(&kTagInfo, kTagInfo.cPlayerIndex);
		}
		else
		{
			m_kUserInfo.SetCurrentChar(iArenaCharType);
			Calcualte_Point();
			LoadPlayerTemplate();
		}
		bLoad = true;
	}

	// 아레나 모션 세팅
	if (IsLocalPlayer())
	{
		if (g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON)
			SetPlayerState(GetCurFormTemplate().JUMP);
		else
			SetPlayerState(MID_COMMON_ARENA);
	}

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
		InitPlayerValueforTagArena(bLoad);
	else
		CreatePlayer(bLoad, true);

	// 탑승물 아레나가 셋팅되어 있으면...
	if (m_kEmbarkArena.IsSetting())
	{
		if (IsLocalPlayer() && false == m_kEmbarkArena.m_bSendPacket)
		{
			SiKGCEmbarkManager()->AddEmbark(m_kEmbarkArena.m_iEmbarkType, m_kEmbarkArena.m_fPosX, m_kEmbarkArena.m_fPosY, m_kEmbarkArena.m_bRight, true, m_kUserInfo.dwUID);
			m_kEmbarkArena.m_bSendPacket = true;
		}
	}

	SetSuper("", 0, 0);
}

void PLAYER::Heal_To_Player(int What, float fRate) //에이미 1차 버프
{
	switch (What)
	{
	case 0: // 다른 플레이어를 치료 큐어
	case 17:
	{
		for (int i = 0; i < MAX_PLAYER_NUM; i++)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			float fHealAmount = 0.015f;
#ifdef MATCH_BALLENCE_121217
			if (fRate > 0.f) {
				fHealAmount = fRate;
			}

			if (i == m_iPlayerIndex && What != 17)
#else
			if (i == m_iPlayerIndex)
#endif
			{
				fHealAmount /= 2.0f;
			}

			if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
			{
				if (g_MyD3D->MyPlayer[i]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
					continue;

				// 카운터 시 살려주지 말자.
				if (g_MyD3D->MyPlayer[i]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP))
					continue;

				float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
				float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));
				if (Length < 0.8f)
				{
					if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
					{
						DamageInfo damage;
						damage.SetDamage(fHealAmount);
						damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, m_iPlayerIndex);
						damage.SetAbilityEffect(true, false);
						g_MyD3D->MyPlayer[i]->Change_HP(damage);
					}
					for (int j = 0; j < 10; j++)
					{
						//g_MyD3D->MySparks->Water_Piece(g_MyD3D->MyPlayer[i]->x-1.15f+.3f*(float)rand()/RAND_MAX,
						//g_MyD3D->MyPlayer[i]->y+.3f*(float)rand()/RAND_MAX, 8);
						float x = g_MyD3D->MyPlayer[i]->vPos.x - 1.15f + .3f * (float)rand() / RAND_MAX;
						float y = g_MyD3D->MyPlayer[i]->vPos.y + .3f * (float)rand() / RAND_MAX;
						int Delay = 8;
						g_MyD3D->MySparks->Add(WATER_PIECE, x, y, Delay, 0.1f + 0.1f * randf(), x, y - 0.4f * randf(), 12.6f * randf());
					}
				}
			}
		}
	}
	break;
	case 1: // 다른 플레이어에게 매직쉴드
	{
		//             for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
		//             {
		//                 if( i == m_iPlayerIndex || g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1 )
		//                     continue;
		//
		//                 if( g_MyD3D->IsSameTeam(i, m_iPlayerIndex) )
		//                 {
		//                     if( g_MyD3D->MyPlayer[i]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[i]->uiMotion==g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) )
		//                         return;
		//
		//                     float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
		//                     float Length = D3DXVec2Length( &D3DXVECTOR2( fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y ) );
		//                     if( Length < 0.8f )
		//                     {
		//                         CDamageInstance* ptFire = g_MyD3D->m_pDamageManager->Add(DT_FLING_SHILED, m_iPlayerIndex);
		//                         if( ptFire != NULL )
		//                             ptFire->m_Who = i;
		//                     }
		//                     g_MyD3D->MyPlayer[i]->ConfirmUseSheild( CID_ARME2 );
		//                 }
		//             }
	}
	break;
	case 2: // 다른 플레이어의 마나 치료
	{
		int iShortestPlayerIndex = -1;
		float fShortestLength = 10000.0f;

		for (int i = 0; i < MAX_PLAYER_NUM; i++)
		{
			if (i == m_iPlayerIndex || g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
			{
				if (g_MyD3D->MyPlayer[i]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
					continue;

				float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
				float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));

				if (fShortestLength > Length)
				{
					iShortestPlayerIndex = i;
					fShortestLength = Length;
				}
			}
		}

		// 가장 가까운 한 녀석만
		if (fShortestLength < 1000.0f && iShortestPlayerIndex > -1)
		{
			if (g_MyD3D->MyPlayer[iShortestPlayerIndex]->IsLocalPlayer())
			{
				// 마나한칸을 올려줌
				if (g_kGlobalValue.CheckTrainedSkill(g_MyD3D->MyPlayer[m_iPlayerIndex], SID_ARME1_SPIRIT_RECOVERY_LV3))
					g_MyD3D->MyPlayer[iShortestPlayerIndex]->Change_MP(3.0f);
				else if (g_kGlobalValue.CheckTrainedSkill(g_MyD3D->MyPlayer[m_iPlayerIndex], SID_ARME1_SPIRIT_RECOVERY_LV2))
					g_MyD3D->MyPlayer[iShortestPlayerIndex]->Change_MP(2.0f);
				else if (g_kGlobalValue.CheckTrainedSkill(g_MyD3D->MyPlayer[m_iPlayerIndex], SID_ARME1_SPIRIT_RECOVERY_LV1))
					g_MyD3D->MyPlayer[iShortestPlayerIndex]->Change_MP(1.0f);
				else
					g_MyD3D->MyPlayer[iShortestPlayerIndex]->Change_MP(1.0f);


			}
			for (int j = 0; j < 10; j++)
			{
				float x = g_MyD3D->MyPlayer[iShortestPlayerIndex]->vPos.x - 1.15f + .3f * (float)rand() / RAND_MAX;
				float y = g_MyD3D->MyPlayer[iShortestPlayerIndex]->vPos.y + .3f * (float)rand() / RAND_MAX;
				int Delay = 8;
				g_MyD3D->MySparks->Add(WATER_PIECE, x, y, Delay, 0.1f + 0.1f * randf(), x, y - 0.4f * randf(), 12.6f * randf());
			}
		}
	}
	break;
	case 3: // Ankh of Resurrection
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
#ifndef _DEBUG
			if (i == m_iPlayerIndex || g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;
#else
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;
#endif

			if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
			{
				// 추가
				if (g_kGlobalValue.CheckTrainedSkill(g_MyD3D->MyPlayer[m_iPlayerIndex], SID_RYAN1_RESERECTION_LV2))
					g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_ANKH_OF_RESURRECTION, 60.0f);
				else
					g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_ANKH_OF_RESURRECTION, 30.0f);
			}
		}
	}
	break;
	case 4: // Aura of Wolf
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (i == m_iPlayerIndex)
			{
				// 나는 버프만 있는거를 세팅
				g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_METAMORPHOSIS_WOLF_AURA_SELF, 3.0f);

				// 버프 세팅
				//if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
				//    g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_METAMORPHOSIS_WOLF_AURA_SELF, 3.0f );
				continue;
			}

			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
			{
				float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
				float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));
				if (Length < 1.0f)
				{
					// 추가
					g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_METAMORPHOSIS_WOLF_AURA, 3.0f);

					// 버프 세팅
					//if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
					//    g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_METAMORPHOSIS_WOLF_AURA, 3.0f );
				}
			}
		}
	}
	break;
	case 5: // Armor of Nephillim
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			if (i == m_iPlayerIndex)
			{
				// 나는 더 쎈거를 셋팅
				g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_NEPHILLIM_ARMOR_ME, 35.0f);

				if (MetamorphosisForm == FORM_NORMAL)
				{
					continue;
				}

				//if( -1 != GetCurMetaFormBuffTimeIdx() )
				//{
				//    // 2007/2/20. iridology. 주먹불
				//    stEffect.m_fCurrTime = m_FormTemplate[MetamorphosisForm].sMetamorphosisTemplate.vecStartBuffEffect[GetCurMetaFormBuffTimeIdx()].fBuffTime;
				//    stEffect.m_eMagicEffect = EGC_EFFECT_NAPHILLIM_HAND_FIRE_LEFT;
				//    g_pMagicEffect->SetMagicEffect(i, stEffect);

				//    stEffect.m_fCurrTime = m_FormTemplate[MetamorphosisForm].sMetamorphosisTemplate.vecStartBuffEffect[GetCurMetaFormBuffTimeIdx()].fBuffTime;
				//    stEffect.m_eMagicEffect = EGC_EFFECT_NAPHILLIM_HAND_FIRE_RIGHT;
				//    g_pMagicEffect->SetMagicEffect(i, stEffect);
				//}

				// 버프 세팅
				//if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
				//    g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_NEPHILLIM_ARMOR_ME, 35.0f );

				continue;
			}

			if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
			{
				g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_NEPHILLIM_ARMOR_FRIEND, 25.0f);
			}
		}
	}
	break;
	case 6: // 로난 매직 스워드
	case 14:// 로난 매직 스워드 npc
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			if (i == m_iPlayerIndex)
			{
				g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_RONAN_MAGIC_SWORD, What == 6 ? 20.0f : 999999.0f);
				continue;
			}
		}
	}
	break;
	case 7: // 로난 힐
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			if (i == m_iPlayerIndex)
			{
				g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_RONAN_HEAL, 7.0f);
				continue;
			}
		}
	}
	break;
	case 8: // 에이미 강인한 아이돌
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;
			int iLv = 1;
			if (CheckSkill(SID_AMY1_MODECHANGE_LV2))
				iLv = 2;
			if (CheckSkill(SID_AMY1_MODECHANGE_LV3))
				iLv = 3;
			if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
			{
				g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_AMY1_DANCING_BUFF, fRate, iLv);
				continue;
			}
		}
	}
	break;
	case 9: // 에이미 피버 타임
	case 12: // 에이미 피버 타임 npc
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			int iLv = 1;
			fRate = 20;
			if (CheckSkill(SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV2))
				iLv = 2;
			if (CheckSkill(SID_AMY1_SPECIAL_DANCE2_FEVERTIME_LV3))
			{
				iLv = 2;
				fRate = 30;
			}

			float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
			float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));
			if (Length < 0.4f * iLv)
			{
				if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
				{
					g_pMagicEffect->ClearMagicEffect(i, EGC_EFFECT_AMY1_FEVER_TIME_BUFF);

					g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_AMY1_FEVER_TIME_BUFF, What == 9 ? fRate : 999999.0f, iLv);
					continue;
				}
			}
		}
	}
	break;
	case 10: // 에이미 러빙유
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			// 카운터 시 살려주지 말자.
			if (g_MyD3D->MyPlayer[i]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP))
				continue;

			fRate = 10;
			if (CheckSkill(SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV3))
				fRate = 20;
			else if (CheckSkill(SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV2))
				fRate = 15;

			float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
			float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));
			if (Length < 0.4f)
			{
				if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
				{
					if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
					{
						float fRecoveryHP = g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP] * (fRate / 100.0f);
						DamageInfo damage;
						damage.SetDamage(fRecoveryHP); //0.08f
						damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, m_iPlayerIndex);
						damage.SetAbilityEffect(true, false);
						g_MyD3D->MyPlayer[i]->Change_HP(damage);
					}

					//if (CheckSkill(SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV3))
					//{
					//	g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_AMY1_GODDESS_BUFF, fRate, 3);
					//}
					//else if (CheckSkill(SID_AMY1_SPECIAL_DANCE3_LOVINGYOU_LV2))
					//{
					//	g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_AMY1_GODDESS_BUFF, fRate, 2);
					//}
					continue;
				}
			}
		}
	}
	break;
	case 11: // 에이미 도발
	{
		//MAGIC_EFFECT stEffect;
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
			float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));
			if (Length < 0.4f)
			{
				if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i) && i != m_iPlayerIndex)
				{
					if (m_iPlayerIndex != i && Extra_Char_Num == CID_AMY1)
						g_MyD3D->MyPlayer[i]->AddParticleNoDirectionPosWithTrace("Amy01_Twinkling_buff01", 0, 0.0f, -0.04f);

					if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
					{
						if (m_iPlayerIndex != i)
							g_MyD3D->MyPlayer[i]->Change_MP(0.2f);
					}
				}
			}
		}
	}
	break;
	case 13: // 로난 다른 플레이어에게 디펜스 룬!
	{
		int nForceNum = g_MyD3D->MyPlayer[m_iPlayerIndex]->GetForceNum();
		for (int i = 0; i < MAX_PLAYER_NUM; i++)
		{
			if (!g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			if (g_MyD3D->IsSameTeam(i, m_iPlayerIndex))
			{
				if (g_MyD3D->MyPlayer[i]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
					return;

				float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
				float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));
				if (Length < 0.8f)
				{
					CDamageInstance* ptFire = g_MyD3D->m_pDamageManager->Add(DT_RONAN3_RUNE_SHEILD, m_iPlayerIndex);
					if (ptFire != NULL)
						ptFire->m_Who = i;

					float fForceRate;
					float fModeDecreaseRatio = 1.0f;

					switch (nForceNum)
					{
					case 1:
						fForceRate = 0.35f * fModeDecreaseRatio;
						break;
					case 2:
						fForceRate = 0.45f * fModeDecreaseRatio;
						break;
					case 3:
						fForceRate = 0.60f * fModeDecreaseRatio;
						break;
					default:
						fForceRate = 0.20f * fModeDecreaseRatio;
						break;
					}

#if defined( LEVEL_DESIGN_STAT_CALC)
					int ShieldHP = static_cast<int>(g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP] * 0.01f * fForceRate * 125.0f);
#else
					int ShieldHP = static_cast<int>(g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP] * fForceRate * 125.0f);
#endif

					g_MyD3D->MyPlayer[i]->SetShield(ShieldHP, 0.0f, 0, false);
					g_MyD3D->MyPlayer[i]->m_bIsCountAttackShield = false;
					g_MyD3D->MyPlayer[i]->ConfirmUseSheild(CID_RONAN3);
				}
			}
		}

		g_MyD3D->MyPlayer[m_iPlayerIndex]->InitForceNum();
	}
	break;
	case 15:
		if (g_MyD3D->MyPlayer[m_iPlayerIndex]->IsLocalPlayer())
		{
			if (GetHP() < m_fAbility[ABILITY_HP])
			{
				float fHP = 0.0f;
				fHP = GetHP() + m_fAbility[ABILITY_HP] * (fRate / 100);

				if (fHP >= m_fAbility[ABILITY_HP])
					fHP -= m_fAbility[ABILITY_HP];
				else
					fHP = m_fAbility[ABILITY_HP] * (fRate / 100);

				g_MyD3D->MyPlayer[m_iPlayerIndex]->SetHP(g_MyD3D->MyPlayer[m_iPlayerIndex]->GetHP() + fHP);
			}
		}
		break;
	case 16: // 에이미2차 아이돌스테이지
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING || g_MyD3D->MyPlayer[i]->m_cLife < 1)
				continue;

			// 카운터 시 살려주지 말자.
			if (g_MyD3D->MyPlayer[i]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[i]->uiMotion == g_MyD3D->MyPlayer[i]->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP))
				continue;

			fRate = 10;
			float fDstX = vPos.x - g_MyD3D->MyPlayer[i]->vPos.x;
			float Length = D3DXVec2Length(&D3DXVECTOR2(fDstX, vPos.y - g_MyD3D->MyPlayer[i]->vPos.y));
			if (Length < 0.4f)
			{
				if (g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
				{
					if (g_MyD3D->MyPlayer[i]->IsLocalPlayer())
					{
						float fRecoveryHP = g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP] * (fRate / 100.0f);
						DamageInfo damage;
						damage.SetDamage(fRecoveryHP);
						damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, m_iPlayerIndex);
						damage.SetAbilityEffect(true, false);
						g_MyD3D->MyPlayer[i]->Change_HP(damage);
					}

					g_pMagicEffect->SetMagicEffect(i, EGC_AMY3_DANCEMODE_SPECIAL3_BUFF, fRate);

				}
			}
		}
	}
	break;
	}
}

void PLAYER::Stop(float f_x, float f_y, bool IsAll)
{
	float Length;
	if (IsAll)
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_cLife > 0)
			{
				g_MyD3D->MyPlayer[i]->Slow_Count = 50;
			}
		}

		for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			MONSTER* pMonster = mit->second;

			if (pMonster->m_bLive && !pMonster->IsSuperArmor() && !pMonster->IsInvincible())
			{
				pMonster->m_iSlowCount = 50;
			}
		}
	}
	else if (m_iPlayerIndex == MAX_PLAYER_NUM)
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_cLife > 0)
			{
				if (IgnoreStop_MotionList(i))
					continue;

				if (g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->uiMotion == MID_ELESIS_ATK1_5)
				{
					g_MyD3D->MyPlayer[i]->Slow_Count = 40;
				}
				else
				{
					Length = D3DXVec2Length(&D3DXVECTOR2(f_x - g_MyD3D->MyPlayer[i]->vPos.x,
						f_y - g_MyD3D->MyPlayer[i]->vPos.y
					));
					if (Length < 1.0f)
						g_MyD3D->MyPlayer[i]->Slow_Count = 20;
					else if (Length > 1.5f)
						continue;
					else if (g_MyD3D->MyPlayer[i]->Slow_Count < 21 - (char)((Length - 1.0f) * 40.0f))
						g_MyD3D->MyPlayer[i]->Slow_Count = 21 - (char)((Length - 1.0f) * 40.0f);
				}
			}
		}
		for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			MONSTER* pMonster = mit->second;
			if (pMonster->m_bLive && !pMonster->IsSuperArmor() && !pMonster->IsInvincible())
			{
				Length = D3DXVec2Length(&D3DXVECTOR2(f_x - pMonster->m_afX[0],
					f_y - pMonster->m_afY[0]));
				if (Length < 1.0f)
					pMonster->m_iSlowCount = 20;
				else if (Length > 1.5f)
					continue;
				else if (pMonster->m_iSlowCount < 21 - (char)((Length - 1.0f) * 40.0f))
					pMonster->m_iSlowCount = 21 - (char)((Length - 1.0f) * 40.0f);
			}
		}
	}
	else
	{
		if (!SiKGCRoomManager()->IsMonsterGameMode())
		{
			for (int i = 0; i < MAX_PLAYER_NUM; ++i)
			{
				if (g_MyD3D->MyPlayer[i]->m_cLife > 0 && i != m_iPlayerIndex)
				{
					if (IgnoreStop_MotionList(i) || g_MyD3D->MyPlayer[i]->m_bIgnoreStopMotion)
						continue;

					Length = D3DXVec2Length(&D3DXVECTOR2(f_x - g_MyD3D->MyPlayer[i]->vPos.x,
						f_y - g_MyD3D->MyPlayer[i]->vPos.y));
					if (Length < 0.5f)
						g_MyD3D->MyPlayer[i]->Slow_Count = 80;
					else if (Length > 1.0f)
						continue;
					else if (g_MyD3D->MyPlayer[i]->Slow_Count < 80 - (char)((Length - .5f) * 160.0f))
						g_MyD3D->MyPlayer[i]->Slow_Count = 80 - (char)((Length - .5f) * 160.0f);
				}
			}
		}
		else
		{
			for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
				MONSTER* pMonster = mit->second;
				if (pMonster->m_bLive && !pMonster->IsSuperArmor() && !pMonster->IsInvincible())
				{
					Length = D3DXVec2Length(&D3DXVECTOR2(f_x - pMonster->m_afX[0],
						f_y - pMonster->m_afY[0]));
					if (Length < 0.5f)
						pMonster->m_iSlowCount = 80;
					else if (Length > 1.0f)
						continue;
					else if (pMonster->m_iSlowCount < 80 - (char)((Length - .5f) * 160.0f))
						pMonster->m_iSlowCount = 80 - (char)((Length - .5f) * 160.0f);
				}
			}
		}

		if (IsSlowAllowedMotion(uiMotion))
			Slow_Count = 50;
	}
}

void PLAYER::QuickStop(float f_x, float f_y, bool IsAll)
{
	float Length;
	if (IsAll)
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_cLife > 0)
			{
				g_MyD3D->MyPlayer[i]->Slow_Count = 25;
			}
		}

		for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			MONSTER* pMonster = mit->second;

			if (pMonster->m_bLive && !pMonster->IsSuperArmor() && !pMonster->IsInvincible())
			{
				pMonster->m_iSlowCount = 25;
			}
		}
	}
	else if (m_iPlayerIndex == MAX_PLAYER_NUM)
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_cLife > 0)
			{
				if (IgnoreStop_MotionList(i))
					continue;

				if (g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->uiMotion == MID_ELESIS_ATK1_5)
				{
					g_MyD3D->MyPlayer[i]->Slow_Count = 20;
				}
				else
				{
					Length = D3DXVec2Length(&D3DXVECTOR2(f_x - g_MyD3D->MyPlayer[i]->vPos.x,
						f_y - g_MyD3D->MyPlayer[i]->vPos.y
					));
					if (Length < 1.0f)
						g_MyD3D->MyPlayer[i]->Slow_Count = 10;
					else if (Length > 1.5f)
						continue;
					else if (g_MyD3D->MyPlayer[i]->Slow_Count < 21 - (char)((Length - 1.0f) * 40.0f))
						g_MyD3D->MyPlayer[i]->Slow_Count = 10.5 - (char)((Length - 1.0f) * 20.0f);
				}
			}
		}
		for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			MONSTER* pMonster = mit->second;
			if (pMonster->m_bLive && !pMonster->IsSuperArmor() && !pMonster->IsInvincible())
			{
				Length = D3DXVec2Length(&D3DXVECTOR2(f_x - pMonster->m_afX[0],
					f_y - pMonster->m_afY[0]));
				if (Length < 1.0f)
					pMonster->m_iSlowCount = 10;
				else if (Length > 1.5f)
					continue;
				else if (pMonster->m_iSlowCount < 21 - (char)((Length - 1.0f) * 40.0f))
					pMonster->m_iSlowCount = 10.5 - (char)((Length - 1.0f) * 20.0f);
			}
		}
	}
	else
	{
		if (!SiKGCRoomManager()->IsMonsterGameMode())
		{
			for (int i = 0; i < MAX_PLAYER_NUM; ++i)
			{
				if (g_MyD3D->MyPlayer[i]->m_cLife > 0 && i != m_iPlayerIndex)
				{
					if (IgnoreStop_MotionList(i) || g_MyD3D->MyPlayer[i]->m_bIgnoreStopMotion)
						continue;

					Length = D3DXVec2Length(&D3DXVECTOR2(f_x - g_MyD3D->MyPlayer[i]->vPos.x,
						f_y - g_MyD3D->MyPlayer[i]->vPos.y));
					if (Length < 0.5f)
						g_MyD3D->MyPlayer[i]->Slow_Count = 40;
					else if (Length > 1.0f)
						continue;
					else if (g_MyD3D->MyPlayer[i]->Slow_Count < 80 - (char)((Length - .5f) * 160.0f))
						g_MyD3D->MyPlayer[i]->Slow_Count = 40 - (char)((Length - .25f) * 80.0f);
				}
			}
		}
		else
		{
			for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
				MONSTER* pMonster = mit->second;
				if (pMonster->m_bLive && !pMonster->IsSuperArmor() && !pMonster->IsInvincible())
				{
					Length = D3DXVec2Length(&D3DXVECTOR2(f_x - pMonster->m_afX[0],
						f_y - pMonster->m_afY[0]));
					if (Length < 0.5f)
						pMonster->m_iSlowCount = 40;
					else if (Length > 1.0f)
						continue;
					else if (pMonster->m_iSlowCount < 80 - (char)((Length - .5f) * 160.0f))
						pMonster->m_iSlowCount = 40 - (char)((Length - .25f) * 80.0f);
				}
			}
		}

		if (IsSlowAllowedMotion(uiMotion))
			Slow_Count = 25;
	}
}

bool PLAYER::IsHighRanker()
{
	if (m_kUserInfo.cCharIndex == -1)
		return false;
	SCharInfo* pCharInfo = &GetCurrentChar();
	if (pCharInfo == NULL)
	{
		g_kGlobalValue.m_strNullFunc = __FUNCTION__;
		return false;
	}

	if (pCharInfo->vecItemInfo.size() == 0)
		return false;

	if (pCharInfo->vecItemInfo[0].iItemID > 80 && pCharInfo->vecItemInfo[0].iItemID < 87)
		return true;

	return false;
}

void PLAYER::Calcualte_Point(bool DecDuration, bool bStartGame, bool bHPFull)
{
	if (m_kUserInfo.cCharIndex == -1)
		return;

	m_fIncHPRatio = 1.0f;
	m_fMPDecreaseRatio = 1.0f;
	m_fMinusAPCoolTime = 0.0f;
	AutoSetExtra();
	m_setExtra2.clear();
	EAbilityData stCharacterAbility;
	GetLevelAbilityMode(stCharacterAbility);
	SetPlayerAbility(stCharacterAbility, bHPFull);

	if (SiKGCChannelManager()->IsPVPBallanceServer() || SiKGCRoomManager()->IsMatchBalance())
	{
		m_fAbility[ABILITY_ATK] = BALANCE_PVP_ABILITY_ATK_VALUE;
		m_fAbility[ABILITY_DEF] = BALANCE_PVP_ABILITY_DEF_VALUE;
		m_fAbility[ABILITY_HP] = BALANCE_PVP_ABILITY_HP_VALUE;
		m_fAbility[ABILITY_SPECIAL_ATK] = 0.0f;
		m_fAbility[ABILITY_SPECIAL_DEF] = 0.0f;
		m_fAbility[ABILITY_HP_DRAIN_RATE] = 0.0f;
		m_fAbility[ABILITY_MP_DRAIN_RATE] = 0.0f;
		m_fAbility[ABILITY_HP_RECOVERY_RATE] = 0.0f;
		m_fAbility[ABILITY_MP_RECOVERY_RATE] = 0.0f;
		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] = 0.0f;
		m_fAbility[ABILITY_COUNTER_DEF] = 0.0f;
		m_fAbility[ABILITY_ARENA_ABSENCE_TIME] = 0.0f;
		m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] = 0.0f;

		if (bHPFull)
			SetHPFull();
		return;
	}

#if defined(ANGELS_EGG_REFORM)
	if (SiKGCRoomManager()->IsAngelEggModeAbility())
	{
		m_fAbility[ABILITY_ATK] = 50.0f;
		m_fAbility[ABILITY_DEF] = 38.0f;
		m_fAbility[ABILITY_HP] = 30.0f;
		m_fAbility[ABILITY_SPECIAL_ATK] = 0.0f;
		m_fAbility[ABILITY_SPECIAL_DEF] = 0.0f;
		m_fAbility[ABILITY_HP_DRAIN_RATE] = 0.0f;
		m_fAbility[ABILITY_MP_DRAIN_RATE] = 0.0f;
		m_fAbility[ABILITY_HP_RECOVERY_RATE] = 0.0f;
		m_fAbility[ABILITY_MP_RECOVERY_RATE] = 0.0f;
		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] = 0.0f;
		m_fAbility[ABILITY_COUNTER_DEF] = 0.0f;
		m_fAbility[ABILITY_ARENA_ABSENCE_TIME] = 0.0f;
		m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] = 0.0f;

		if (bHPFull)
			SetHPFull();
		return;
	}
#endif

	if (SiKGCRoomManager()->IsDotaModeAbility())
	{
		SDotaTemplate* pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
		if (pDotaTemplate)
		{
			SCharInfo& sCharInfo = m_kUserInfo.GetCurrentChar();
			if ((-1 < sCharInfo.iCharType && sCharInfo.iCharType < GC_CHAR_NUM))
			{
				m_fAbility[ABILITY_ATK] = pDotaTemplate->vecsDotaAbility[sCharInfo.iCharType].fAbilityAtk;
				m_fAbility[ABILITY_DEF] = pDotaTemplate->vecsDotaAbility[sCharInfo.iCharType].fAbilityDef;
				m_fAbility[ABILITY_HP] = pDotaTemplate->vecsDotaAbility[sCharInfo.iCharType].fAbilityHp;
				m_fAbility[ABILITY_SPECIAL_ATK] = 0.0f;
				m_fAbility[ABILITY_SPECIAL_DEF] = 0.0f;
				m_fAbility[ABILITY_HP_DRAIN_RATE] = 0.0f;
				m_fAbility[ABILITY_MP_DRAIN_RATE] = 0.0f;
				m_fAbility[ABILITY_HP_RECOVERY_RATE] = 0.0f;
				m_fAbility[ABILITY_MP_RECOVERY_RATE] = 0.0f;
				m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] = 0.0f;
				m_fAbility[ABILITY_COUNTER_DEF] = 0.0f;
				m_fAbility[ABILITY_ARENA_ABSENCE_TIME] = 0.0f;
				m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] = 0.0f;

				if (bHPFull)
					SetHPFull();
			}
			return;
		}
	}

	if (SiGCAutoMatchManager()->IsAutoMatchBalance())
	{
		float fBaseAP = 1.0f;
		float fBaseDP = 1.0f;
		float fBaseHP = 1.0f;

		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_BASE_AP, fBaseAP);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_BASE_DP, fBaseDP);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_BASE_HP, fBaseHP);

		m_fAbility[ABILITY_ATK] *= fBaseAP;
		m_fAbility[ABILITY_DEF] *= fBaseDP;
		m_fAbility[ABILITY_HP] *= fBaseHP;
	}


	if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO) ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN)
	{
		return;
	}

	// Àåºñ´É·ÂÄ¡ ¼¼ÆÃ
	EAbilityData stEquipItemAbility;
	{
		bool bNoItemAbility = false;
		SAbilityRevision* pAbilityRevision = SiKGCWorldMapManager()->GetAbilityRevisionInfo(SiKGCRoomManager()->GetGameMode());
		if (pAbilityRevision && pAbilityRevision->bLoadTable)
			bNoItemAbility = pAbilityRevision->bNoItemAbility;

		if (false == bNoItemAbility)
			g_MyD3D->m_kItemMgr.GetItemRealAbility(this, GetCurrentChar().iCharType, DecDuration, stEquipItemAbility);
	}

	if (SiGCAutoMatchManager()->IsAutoMatchBalance())
	{
		float fEquipAP = 1.0f;
		float fEquipDP = 1.0f;
		float fEquipHP = 1.0f;
		float fEquipSpecialAP = 1.0f;
		float fEquipSpecialDP = 1.0f;
		float fEquipDarkSpear = 1.0f;
		float fEquipCriticalProbability = 1.0f;
		float fEquipCounterMP = 1.0f;

		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_EQUIPMENT_AP, fEquipAP);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_EQUIPMENT_DP, fEquipDP);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_EQUIPMENT_HP, fEquipHP);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_SPECIAL_AP, fEquipSpecialAP);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_SPECIAL_DP, fEquipSpecialDP);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_HELL_SPEAR, fEquipDarkSpear);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_CRITICAL_PROBABILITY, fEquipCriticalProbability);
		SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_COUNTER_MP, fEquipCounterMP);


		stEquipItemAbility.fEquipItemAbility[ABILITY_ATK] *= fEquipAP;
		stEquipItemAbility.fEquipItemAbility[ABILITY_DEF] *= fEquipDP;
		stEquipItemAbility.fEquipItemAbility[ABILITY_HP] *= fEquipHP;
		stEquipItemAbility.fEquipItemAbility[ABILITY_SPECIAL_ATK] *= fEquipSpecialAP;
		stEquipItemAbility.fEquipItemAbility[ABILITY_SPECIAL_DEF] *= fEquipSpecialDP;
		stEquipItemAbility.fEquipItemAbility[ABILITY_DARK_DAMAGE_ATK_POINT] *= fEquipDarkSpear;
		stEquipItemAbility.fEquipItemAbility[ABILITY_CRITICAL_ATTACK_RATE] *= fEquipCriticalProbability;
		stEquipItemAbility.fEquipItemAbility[ABILITY_COUNTER_DEF] *= fEquipCounterMP;
	}

	g_MyD3D->m_kItemMgr.SetPromotionStatus(stEquipItemAbility, m_kUserInfo.GetCurrentChar().iPromotionLevel, this);
	g_MyD3D->m_kItemMgr.MakeEquipAbilityUseful(stEquipItemAbility, bStartGame);

	stCharacterAbility += stEquipItemAbility;
  
	if (Extra_Char_Num % GC_CHAR_NUM == GC_CHAR_LIME) {
		bool bLearn = false;
		if (IsLocalPlayer()) {
			bLearn = IsLearnedSkill(SID_LIME_NORMAL_PARTY_BUFF);
		}
		else {
			bLearn = IsTrainedSkill(SID_LIME_NORMAL_PARTY_BUFF);
		}

		if (bLearn) {
			float fUpRate = (static_cast<float>(CountLiveTeamPlayer()) * 2.f) / 100.f;
			stCharacterAbility.fEquipItemAbility[ABILITY_HP] += (stCharacterAbility.fEquipItemAbility[ABILITY_HP] * fUpRate);
			stCharacterAbility.fEquipItemAbility[ABILITY_ATK] += (stCharacterAbility.fEquipItemAbility[ABILITY_ATK] * fUpRate);
			stCharacterAbility.fEquipItemAbility[ABILITY_DEF] += (stCharacterAbility.fEquipItemAbility[ABILITY_DEF] * fUpRate);
		}
	}

	if (Extra_Char_Num == CID_DIO4) {
		if (IsLearnedSkill(SID_DIO4_WEAPON_PASSIVE_LV2) || IsLearnedSkill(SID_DIO4_WEAPON_PASSIVE_LV3)) {
			float fUpRate = (static_cast<float>(CountLiveTeamPlayer()) * 2.f) / 100.f;
			stCharacterAbility.fEquipItemAbility[ABILITY_HP] += (stCharacterAbility.fEquipItemAbility[ABILITY_HP] * fUpRate);
		}
		else if (IsLearnedSkill(SID_DIO4_DEVIL_PASSIVE_LV2) || IsLearnedSkill(SID_DIO4_DEVIL_PASSIVE_LV3)) {
			m_fMPDecreaseRatio -= (static_cast<float>(CountLiveTeamPlayer()) * 5.f / 100.f);
		}
	}

	if (Extra_Char_Num % GC_CHAR_NUM == GC_CHAR_RIN) {
		if (IsLearnedSkill(SID_RIN2_SELECT_GODDESS)) {
			float fUpRate = (static_cast<float>(CountLiveTeamPlayer()) * 2.f) / 100.f;
			stCharacterAbility.fEquipItemAbility[ABILITY_HP] += (stCharacterAbility.fEquipItemAbility[ABILITY_HP] * fUpRate);
		}
	}

	SetSpecialAbility(stCharacterAbility);
	SetPlayerAbility(stCharacterAbility, bHPFull);


	if (SiGCAutoMatchManager()->IsAutoMatchBalance())
	{
		int iCurMode = SiGCAutoMatchManager()->GetCurrentBalanceMode();

		m_fAbility[ABILITY_ATK] = SiGCAutoMatchManager()->GetAutoMatchBallanacStat(iCurMode, ABILITY_ATK, m_fAbility[ABILITY_ATK]);
		m_fAbility[ABILITY_DEF] = SiGCAutoMatchManager()->GetAutoMatchBallanacStat(iCurMode, ABILITY_DEF, m_fAbility[ABILITY_DEF]);
		m_fAbility[ABILITY_HP] = SiGCAutoMatchManager()->GetAutoMatchBallanacStat(iCurMode, ABILITY_HP, m_fAbility[ABILITY_HP]);
		m_fAbility[ABILITY_SPECIAL_ATK] = SiGCAutoMatchManager()->GetAutoMatchBallanacStat(iCurMode, ABILITY_SPECIAL_ATK, m_fAbility[ABILITY_SPECIAL_ATK]);
		m_fAbility[ABILITY_SPECIAL_DEF] = SiGCAutoMatchManager()->GetAutoMatchBallanacStat(iCurMode, ABILITY_SPECIAL_DEF, m_fAbility[ABILITY_SPECIAL_DEF]);

		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] = SiGCAutoMatchManager()->GetAutoMatchBallanacStat(iCurMode, ABILITY_CRITICAL_ATTACK_RATE, m_fAbility[ABILITY_CRITICAL_ATTACK_RATE]);
		m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] = SiGCAutoMatchManager()->GetAutoMatchBallanacStat(iCurMode, ABILITY_CRITICAL_DAMAGE_UP, m_fAbility[ABILITY_CRITICAL_DAMAGE_UP]);

		m_fAbility[ABILITY_FATAL_SUPER_ARMOR_BUFF] = 0.0f;
		m_fAbility[ABILITY_FATAL_RECOVERY_MP_UP_BUFF] = 0.0f;
		m_fAbility[ABILITY_FATAL_ATTACK_UP_BUFF] = 0.0f;
		m_fAbility[ABILITY_FATAL_SPEEP_UP_BUFF] = 0.0f;
		m_fAbility[ABILITY_FATAL_RECOVERY_POTION_UP] = 0.0f;

		SetHPFull();
	}

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL)
	{
#if defined( NATION_KOREA )
		m_fAbility[ABILITY_ATK] = 1.82f;
		m_fAbility[ABILITY_DEF] = 1.47f;
		m_fAbility[ABILITY_HP] = 1000.f;

#else
		m_fAbility[ABILITY_ATK] = 0.0182f;
		m_fAbility[ABILITY_DEF] = 0.0147f;
		m_fAbility[ABILITY_HP] = 10.00f;
#endif
		SetHPFull();
	}

	if (g_MyD3D->m_pkQuestGameMgr->GetReviseCharLevel())
	{
		float fAP = m_fAbility[ABILITY_ATK];
		float fDP = m_fAbility[ABILITY_DEF];
		float fHP = m_fAbility[ABILITY_HP];
		m_fAbility[ABILITY_ATK] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "HeroDungeon_AP", fAP);
		m_fAbility[ABILITY_DEF] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "HeroDungeon_DP", fDP);
		m_fAbility[ABILITY_HP] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "HeroDungeon_HP", fHP);

		if (bHPFull)
			SetHPFull();
	}

	SAbilityRevision* pAbilityRevision = SiKGCWorldMapManager()->GetAbilityRevisionInfo(SiKGCRoomManager()->GetGameMode());
	if (pAbilityRevision && pAbilityRevision->bLoadTable)
	{
		float fAP = m_fAbility[ABILITY_ATK];
		float fDP = m_fAbility[ABILITY_DEF];
		float fHP = m_fAbility[ABILITY_HP];

		if (0 != pAbilityRevision->iStdLevel_AP)
			m_fAbility[ABILITY_ATK] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "DungeonRevision_AP", fAP, pAbilityRevision->iStdLevel_AP);
		if (0 != pAbilityRevision->iStdLevel_DP)
			m_fAbility[ABILITY_DEF] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "DungeonRevision_DP", fDP, pAbilityRevision->iStdLevel_DP);
		if (0 != pAbilityRevision->iStdLevel_HP)
			m_fAbility[ABILITY_HP] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "DungeonRevision_HP", fHP, pAbilityRevision->iStdLevel_HP);

		if (1.0f != pAbilityRevision->fSpecialAP)
			m_fAbility[ABILITY_SPECIAL_ATK] *= pAbilityRevision->fSpecialAP;
		if (1.0f != pAbilityRevision->fDrainHP)
			m_fAbility[ABILITY_HP_DRAIN_RATE] *= pAbilityRevision->fDrainHP;
		if (1.0f != pAbilityRevision->fDrainMP)
			m_fAbility[ABILITY_MP_DRAIN_RATE] *= pAbilityRevision->fDrainMP;
		if (1.0f != pAbilityRevision->fRecoveryHP)
			m_fAbility[ABILITY_HP_RECOVERY_RATE] *= pAbilityRevision->fRecoveryHP;
		if (1.0f != pAbilityRevision->fRecoveryMP)
			m_fAbility[ABILITY_MP_RECOVERY_RATE] *= pAbilityRevision->fRecoveryMP;
		if (1.0f != pAbilityRevision->fCriticalRate)
			m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] *= pAbilityRevision->fCriticalRate;
		if (1.0f != pAbilityRevision->fCounterDEF)
			m_fAbility[ABILITY_COUNTER_DEF] *= pAbilityRevision->fCounterDEF;
		if (1.0f != pAbilityRevision->fDarkATK)
			m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] *= pAbilityRevision->fDarkATK;
		if (1.0f != pAbilityRevision->fCriticalATK)
			m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] *= pAbilityRevision->fCriticalATK;
		if (1.0f != pAbilityRevision->fPetMPRecovery)
			m_fAbility[ABILITY_RECOVERY_UP_PET_MP] *= pAbilityRevision->fPetMPRecovery;
		if (1.0f != pAbilityRevision->fPortionUP)
			m_fAbility[ABILITY_RECOVERY_UP_POTION] *= pAbilityRevision->fPortionUP;

		if (bHPFull)
			SetHPFull();
	}
}

void PLAYER::Calcualte_Point(TagCharacterInfo& tagInfo, bool DecDuration)
{
	if (tagInfo.m_iCharType == -1)
		return;

	AutoSetExtra(tagInfo);
	tagInfo.m_setExtra2.clear();

	EAbilityData stTagCharacterAbility;
	GetLevelAbilityMode(m_kUserInfo.GetCurrentChar(tagInfo.m_iCharType).iLevel, tagInfo.m_iCharType, stTagCharacterAbility);
	tagInfo.SetTagCharacterAbility(stTagCharacterAbility);

	if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO) ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO ||
		SiKGCChannelManager()->IsPVPBallanceServer() ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN)
	{
		return;
	}

	EAbilityData stEquipItemAbility;
	g_MyD3D->m_kItemMgr.GetItemRealAbility(this, tagInfo.m_iCharType, DecDuration, stEquipItemAbility);
	g_MyD3D->m_kItemMgr.SetPromotionStatus(stEquipItemAbility, m_kUserInfo.GetCurrentChar(tagInfo.m_iCharType).iPromotionLevel, this);
	g_MyD3D->m_kItemMgr.MakeEquipAbilityUseful(stEquipItemAbility, false);

	stTagCharacterAbility += stEquipItemAbility;
	SetSpecialAbility(stTagCharacterAbility);
	tagInfo.SetTagCharacterAbility(stTagCharacterAbility);
}

float PLAYER::GetAttackPoint(const DamageInfo& damage_)
{
	if (damage_.IsAffectedByAtk() == false)
		return 1.0f;

	float f_fAtkPoint = 1.0f;

	// 몬스터 데미지
	if (damage_.IsMonsterDamage())
	{
		if (damage_.GetAttacker() == -1)
		{
			MONSTER* pMonster = g_kMonsterManager.GetMonster(0);
			if (pMonster != NULL) {
				f_fAtkPoint = pMonster->m_fAttackPoint * pMonster->m_fAtkRatio;
			}
		}

		else if (g_kMonsterManager.IsMonsterIndex(damage_.GetAttacker()))
		{
			MONSTER* pMonster = g_kMonsterManager.GetMonster(damage_.GetAttacker());

			if (pMonster->m_iSummoned == MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER && pMonster->m_fOwnerAtkRatio != -1.f) {
				PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(pMonster->m_dwOwnerUID);
				if (pPlayer) {
					f_fAtkPoint = pPlayer->m_fAbility[ABILITY_ATK] * pMonster->m_fOwnerAtkRatio;
				}
			}
			else {
				f_fAtkPoint = pMonster->m_fAttackPoint * pMonster->m_fAtkRatio;

				if (damage_.IsDamageFactor())
				{
					// 2007/1/29. iridology. Damage Factor 스크립트로..
					f_fAtkPoint *= GetFormTemplate(MetamorphosisForm).fDamagefactor;
				}
			}
		}
	}
	else if (damage_.IsPlayerDamage())
	{
		if (damage_.IsPetDamage() == false)
		{
			// 속성 능력치 적용!
			// 필살기 공격력~ 필살기 일때, 필살기 공격력을 더해서 피 깎는다..
			float fAttributeSpecialAtk = 0.0f;
			fAttributeSpecialAtk = g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_bSpecialAttack ? g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_SPECIAL_ATK] : 0.0f;
			f_fAtkPoint = g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_ATK] + fAttributeSpecialAtk;
		}
		else
		{
			KPetInfo kPetinfo = g_MyD3D->MyPlayer[damage_.GetAttacker()]->GetCurrentChar().kPetInfo;
#ifdef LEVEL_DESIGN_STAT_CALC
			f_fAtkPoint = 200.f + kPetinfo.m_iLevel * 2.f * static_cast<int>(pow(float(2), kPetinfo.m_cPromotion)) + 0.5f;
#else
			f_fAtkPoint = 0.2f + kPetinfo.m_iLevel * 0.002f * static_cast<int>(pow(float(2), kPetinfo.m_cPromotion)) + 0.0005f;
#endif
		}
	}

	return f_fAtkPoint;
}

// 바인딩용
void PLAYER::ChangeHP(float fDamage, bool bEffectAtk, bool bEffectDef, bool bCheckCritical)
{
	DamageInfo damage;
#ifdef LEVEL_DESIGN_STAT_CALC
	damage.SetDamage(fDamage * 100.f);
#else
	damage.SetDamage(fDamage);
#endif
	damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, GetLastKillmePlayer());
	damage.SetAbilityEffect(bEffectAtk, bEffectDef);
	damage.SetCriticalCheck(bCheckCritical);
	Change_HP(damage);
}

void PLAYER::SetCatchDamage(float fDamage)
{
	SCharInfo sCharInfo = m_kUserInfo.GetCurrentChar();

	DamageInfo damage;

	float fCatchResist = 1.f;
	if (m_fAbility[ABILITY_CATCH_DAMAGE_RESIST] > 0.f)
	{
		fCatchResist = (100.f - m_fAbility[ABILITY_CATCH_DAMAGE_RESIST]) / 100.f;
	}

#ifdef LEVEL_DESIGN_STAT_CALC
	damage.SetDamage(fDamage * 100.f * fCatchResist);
#else
	damage.SetDamage(fDamage * fCatchResist);
#endif
	damage.SetAttacker(DamageInfo::DAMAGE_PLAYER, GetLastKillmePlayer());
	damage.SetAbilityEffect(true, true);
	Change_HP(damage);
}

float PLAYER::CalculationRealDamage(bool bPlayerDamage, float c_HP, float fAttributeAtk, float fAttributeDef, float fAttributeSpecialDef, float f_fAtkPoint, float fIncDpRatio)
{
	float fDiff;
	const float fATKDEF_Rate = 2.085f;    //  pvp 공방 비율 상수값
#if defined( MATCH_BALANCE )
	if (bPlayerDamage)
	{
		fDiff = c_HP * ((fAttributeAtk / ((fAttributeDef + fAttributeSpecialDef) * fIncDpRatio) * 1.2f) * fATKDEF_Rate);
	}
	else
	{
		fDiff = c_HP * (fAttributeAtk / ((fAttributeDef + fAttributeSpecialDef) * fIncDpRatio) * 1.2f);
	}
#else
	if (bPlayerDamage)
	{
		fDiff = c_HP * (fAttributeAtk / ((fAttributeDef + fAttributeSpecialDef) * fIncDpRatio) * fATKDEF_Rate);
	}
	else
	{
		fDiff = c_HP * fAttributeAtk / ((fAttributeDef + fAttributeSpecialDef) * fIncDpRatio);
	}
#endif
	return fDiff;
}

float PLAYER::Change_HP(DamageInfo damage_)
{
	//시작모션일때 무적으로 super설정했는데도 자꾸 맞네요... 걍 여기서 예외 처리 합니다 ... 죄송요.
	if (uiMotion == this->GetCurFormTemplate().START)
		return 0.f;

	float c_HP = damage_.GetHPDelta();
	float fDiff = 0.f;

	//옛날 코드랑 같이 병행하면서 쓰기 위해서...
	if (damage_.GetDamageKind() != -1 && damage_.IsMonsterDamage() == false)
	{
		// 속성 능력치 적용!
		// 데미지의 의한 공격일때도, 스페셜 어택값을 설정해준다.
		g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_bSpecialAttack = damage_.IsSpecialAttack();
	}

	//아테네 소워드를 쓰는 동안에는 공격력이 15% 상승한다.
	bool bCritialAtk = false;
	if (damage_.IsPlayerDamage() || damage_.IsBuffDamage())
	{
		if (damage_.IsAffectedByAtk())
		{
			KHackCheckManager::GetInstance()->BeforeCheckApRatio(&g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_ATTACK_PERCENT], &c_HP);
			c_HP *= (g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_ATTACK_PERCENT] + 1.0f);
			KHackCheckManager::GetInstance()->AfterCheckApRatio(&c_HP);

			if (damage_.IsCheckCritical() && !g_kGlobalValue.m_bCriticalOffToggle && (g_kGlobalValue.m_bCriticalToggle || (randf() < g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE])))
			{
				bCritialAtk = true;

				float fCriticalDamageResist = 0.0f;
				if (m_fAbility[ABILITY_CRITICAL_DAMAGE_RESIST] > 0)
				{
					fCriticalDamageResist = (m_fAbility[ABILITY_CRITICAL_DAMAGE_RESIST]) / 100.f;
				}

				float fCriticalDamageUP = 0.0f;
				if (g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] > 0)
				{
					fCriticalDamageUP = (g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] / 100.f);
				}

				if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_ATTRIBUTE_CRITICAL_DAMAGE_UP))
				{
					fCriticalDamageUP = (g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_FATAL_CRITICAL_DAMAGE_UP_BUFF] / 100.f);
				}

				float fTempVal = 1.0f;
				if (SiGCAutoMatchManager()->IsAutoMatchBalance() || SiKGCRoomManager()->IsMatchBalance())
				{
					SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_CRITICAL_AP, fTempVal);
				}

				//대전일 경우 치명타 데미지 비율 조정
				if (damage_.IsPlayerDamage())
					fCriticalDamageUP *= g_cfPvPCriticalDamageUp;

				c_HP *= ((CRITICALDAMAGE_UP_RATE - fCriticalDamageResist + fCriticalDamageUP) * fTempVal); // 크리티컬 데미지 두배!! 두둥

				//////////////////// 시작 - 성장개편 스킬 버프 설정 /////////////////////////
				SiGCGrowingSkillManager()->SetMagicEffectWhenCritical(g_MyD3D->MyPlayer[damage_.GetAttacker()]); // 때린놈이 크리터진거니까 때린놈한테 버프 걸어주자.
				////////////////////  끝  - 성장개편 스킬 버프 설정 /////////////////////////
			}

			c_HP *= 2.0f; // 장비 공격력 2배가 사라져서 대전에서 섭섭하니 데미지 2배로 해달라고 하시넴
		}
	}

	// Attacker셋팅
	if (c_HP < 0)
	{
		if (damage_.IsMonsterDamage() == true)
		{
			// 소환된 몬스터가 Attacker라면 플레이어 Index를 셋팅해주자.
			int iAttacker = damage_.GetAttacker() + MAX_PLAYER_NUM;

			if (g_kMonsterManager.IsSummonMonsterByPlayer(damage_.GetAttacker()))
			{
				MONSTER* pMonster = g_kMonsterManager.GetMonster(damage_.GetAttacker());
				if (pMonster != NULL) {
					PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(pMonster->m_dwOwnerUID);
					if (pPlayer)
						iAttacker = pPlayer->m_iPlayerIndex;
				}
			}
			SetLastKillmePlayer(iAttacker);
		}
		else
			SetLastKillmePlayer(damage_.GetAttacker());
	}

	// 데미지 처리
	if (IsLocalPlayer() || GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
	{
		// 넘어졌다 일어설때 무적 5프레임 처리
		if (damage_.IsPlayerDamage()) {
			if (uiMotion == GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) && g_MyD3D->MyCtrl->Energy == 0 &&
				g_MyD3D->MyPlayer[damage_.GetAttacker()]->GetCurFormTemplate().JUMP_ATK == g_MyD3D->MyPlayer[damage_.GetAttacker()]->uiMotion)
			{
				Super = 5;
				TRACE(L"Player %d Counter\n", m_iPlayerIndex);
			}
		}

		// 어택포인트 계산
		float f_fAtkPoint = GetAttackPoint(damage_);

		// 내 에너지가 올라가는 것이라면 Defance의 영향을 받지 않는다.
		if (c_HP > 0)
		{
			// 피가 찰때,, c_HP 받아온 값만큼만 찬다. 나의 공격력과 방어력은 상관이 없다.
			if (damage_.IsIgnoreDef() == false) {
				if (damage_.IsAffectedByDef()) {
					fDiff = c_HP * f_fAtkPoint / (m_fAbility[ABILITY_DEF] * (1.0f + m_fAbility[ABILITY_DEFENCE_PERCENT]));
				}
				else {
					fDiff = c_HP * f_fAtkPoint;
				}
			}
			else
			{
				fDiff = c_HP;
			}
			SetHP(GetHP() + fDiff);

#ifndef __PATH__
			WCHAR strTemp[50];
			swprintf(strTemp, 50, L"HP change : %f", c_HP);
			g_kStatsManager.InsertDebugValue(strTemp);
#endif
		}
		//Damage를 입는것이라면
		else
		{
			// 콤보 브?!!!!!!!!
			if (false == damage_.IsNotComboBreak() && IsLocalPlayer())
				ComboBreak();

			if (damage_.GetDamageAtt() & DA_NOREACTION)
			{
				if (GC_GM_TUTORIAL != SiKGCRoomManager()->GetGameMode())
					g_kCamera.SetShakeCameraFrame(GetShakeValue(c_HP * f_fAtkPoint / (m_fAbility[ABILITY_DEF] * (1.0f + m_fAbility[ABILITY_DEFENCE_PERCENT])), m_fAbility[ABILITY_HP]));
				SetShakeEffectFrame(10);
				SetFlashEffectFrame(FLASH_EFFECT_FRAME);
			}

			if (Shield <= 0)

			{
				// 속성 능력치 적용!
				// 필살기 방어력~ 필살기 일때, 필살기 방어력을 더해서 피 깎는다..
				float fAttributeSpecialDef = 0.0f;
				float fAttributeDef = 1.0f;
				float fAttributeAtk = 1.0f;
				if (damage_.IsAffectedByDef())
				{
					if (damage_.IsPlayerDamage())
					{
						fAttributeSpecialDef = g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_bSpecialAttack ? m_fAbility[ABILITY_SPECIAL_DEF] : 0.0f;
					}
					fAttributeDef = m_fAbility[ABILITY_DEF];
				}
				if (damage_.IsAffectedByAtk())
				{
					fAttributeAtk = f_fAtkPoint;
				}

				float fSummonsDamage = c_HP;
				if (damage_.IsMonsterDamage())
				{
					MONSTER* pMonster = g_kMonsterManager.GetMonster(damage_.GetAttacker());
					if (pMonster != NULL) {
						if (pMonster->m_iSummoned == MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER && pMonster->m_fOwnerAtkRatio != -1.f) {
							if (pMonster->m_fSummonsAtkRatio != 0.f) {
								fSummonsDamage = c_HP / pMonster->m_fSummonsAtkRatio * 2.f; //대전에서는 데미지 두배하고 있어서...
							}
						}
					}
				}
				fDiff = CalculationRealDamage(damage_.IsPlayerDamage(), fSummonsDamage, fAttributeAtk, fAttributeDef, fAttributeSpecialDef, f_fAtkPoint, m_fAbility[ABILITY_DEFENCE_PERCENT] + 1.0f);
#ifdef LEVEL_DESIGN_STAT_CALC
				if (fDiff > -1.f && damage_.GetHPDelta() != 0.0f)
					fDiff = -1.f;
#else
				if (fDiff > -0.01f && damage_.GetHPDelta() != 0.0f)
					fDiff = -0.01f;
#endif
				bool bSpearAtk = false;
				if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON &&
#if defined( ANGELS_EGG_REFORM )
					SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_ANGELS_EGG &&
#endif
					damage_.IsMonsterDamage() == false)
				{
					if (g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] > 0.0f && c_HP != 0.0f)
					{
						// 암흑 검 소환
						if (damage_.IsSpecialAttack() == false && damage_.IsPetDamage() == false && randf() < 0.1f)
						{
							bSpearAtk = true;
							fDiff = CalculationRealDamage(damage_.IsPlayerDamage(), c_HP, fAttributeAtk, fAttributeDef, fAttributeSpecialDef, f_fAtkPoint, 1.0f);
#ifdef LEVEL_DESIGN_STAT_CALC
							fDiff -= g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] * 0.05f;
#else
							fDiff -= g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] * 0.0005f;
#endif
						}
					}
				}

				// 방어력 무시하고 넘어오는 데미지 그대로 입힌다
				if (damage_.IsIgnoreDef())
					fDiff = c_HP;

				//                 if( damage_.IsShieldCountEnable() ) {
				//                     ShieldCountAttack( m_iPlayerIndex, damage_.GetAttacker(), fDiff, damage_.IsPetDamage(), damage_.IsMonsterDamage() );
				//                 }
				//
				//                 if( damage_.IsReflectionEnable() ) {
				//                     ReflectionDamage( m_iPlayerIndex, damage_.GetAttacker(), fDiff, damage_.IsPetDamage(), damage_.IsMonsterDamage() );
				//                 }

				if (int iHolyDance = g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_AMY_HOLY_DANCE))
				{
					Change_MP((-fDiff * iHolyDance * 0.25f / m_fAbility[ABILITY_HP]) * g_kGlobalValue.GetMaxMP());
				}

				if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_MARI1_MANASHIELD) ||							/*KWAK1*/
					(g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_LIME2_MANASHIELD) && !g_pMagicEffect->IsCoolTime(m_iPlayerIndex, EGC_EFFECT_LIME2_MANASHIELD)))
				{
					// 피가 깎여야 하는 %를 구해서 같은 비율의 마나를 두배로 깎는다.
					g_MyD3D->MyHead->Level = g_MyD3D->MyHead->Level + ((fDiff / m_fAbility[ABILITY_HP]) * g_kGlobalValue.GetMaxMP()) * 2.0f;

					if (g_MyD3D->MyHead->Level < 0.0f)
					{
						fDiff = g_MyD3D->MyHead->Level / g_kGlobalValue.GetMaxMP() / 2.0f * m_fAbility[ABILITY_HP];
						g_MyD3D->MyHead->Level = 0.0f;
					}
					else
						fDiff = 0.0f;
				}

				int iAttacker = damage_.GetAttacker();
				if (damage_.IsPlayerDamage() &&
					((0.0f != g_MyD3D->MyPlayer[iAttacker]->m_fDamageToMPAttack) ||
						(0.0f != g_MyD3D->MyPlayer[iAttacker]->m_fStaticToMPAttack)))
				{
					float fChangeMP = ((fDiff / m_fAbility[ABILITY_HP] * 3.0f) * g_MyD3D->MyPlayer[iAttacker]->m_fDamageToMPAttack) +
						-g_MyD3D->MyPlayer[iAttacker]->m_fStaticToMPAttack;
					Change_MP(fChangeMP);
				}

				// 몬스터 데미지라면
				if (!(damage_.IsPlayerDamage() && m_iPlayerIndex == damage_.GetAttacker()))
				{
					LTP_CHANGE_HP_INFO kChangeHPInfo;
					kChangeHPInfo.Type = GC_PID_CHANGE_HP_INFO;

					if (damage_.IsPlayerDamage()) {
						kChangeHPInfo.iAttackerIndex = damage_.GetAttacker();
					}
					else {
						kChangeHPInfo.iAttackerIndex = damage_.GetAttacker() + MAX_PLAYER_NUM;
					}

					kChangeHPInfo.fDamage = fDiff;
					kChangeHPInfo.iTargetIndex = m_iPlayerIndex;

					char dwAttackType = PAA_NORMAL;
					if (g_MyD3D->IsPlayerIndex(damage_.GetAttacker()) && g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_bSpecialAttack)
						dwAttackType = PAA_SPECIAL;
					else if (damage_.IsPetDamage())
						dwAttackType = PAA_PET;
					if (bCritialAtk)
					{
						dwAttackType |= PAA_CRITICAL;
					}
					if (bSpearAtk) {
						dwAttackType |= g_kGlobalValue.GetSpearType(damage_.GetAttacker());
					}
					kChangeHPInfo.dwAttackType = dwAttackType;

					g_LatencyTransfer.PushPacket(&kChangeHPInfo);
				}

#if defined( USE_QA_LOG_SYSTEM )
				SiKGCQALogManager()->PlayerChangeHPLog(GetStrUserName(), damage_, iAttacker, GetHP(), fDiff, damage_.IsPlayerDamage() && g_MyD3D->MyPlayer[iAttacker]->m_bSpecialAttack, bCritialAtk, bSpearAtk);
#endif

				if (SiGCAutoMatchManager()->IsAutoMatchBalance() || SiKGCRoomManager()->IsMatchBalance()) {
					float fTempVal = 1.0f;
					SiGCAutoMatchManager()->GetPvPMatchingBalance(GCAutoMatchManager::PMB_DIRECT_HP_DAMAGE, fTempVal);
					fDiff *= fTempVal;
				}

				SetHP(GetHP() + fDiff);
#ifndef __PATH__
				WCHAR strTemp[50];
				swprintf(strTemp, 50, L"HP change : %f", fDiff);
				g_kStatsManager.InsertDebugValue(strTemp);
#endif

				Shield = 0;

				// 분노 게이지 채워 보자...
				{
					fDiff *= -1.0f;
					// 데미지 크기가 어느정도 되어야 분노 게이지를 채운다.
					if (fDiff > m_fAbility[ABILITY_HP] * 0.08f)
					{
						CountBP( GetCurFormTemplate().fBPPerAttackedMe * 3.0f, true );
					}
				}
			}
			else
			{
#if defined(LEVEL_DESIGN_STAT_CALC)
				float fPercent = 0.015f;
				if (damage_.IsWeakToShield())
				{
					fPercent = 0.01f;
				}
#else
				float fPercent = 150.0f;
				if (damage_.IsWeakToShield())
				{
					fPercent = 100.0f;
				}
#endif

				if (m_bSpecialShield)
					fPercent = 1.f;

				m_iPrevShield = Shield;

				if (m_bShieldTypeCounting) {
					Shield -= 1; // 카운팅 방식이면 한 타당 무조건 한개씩만 깎는다.
				}
				else {
					Shield += (int)(c_HP * fPercent * f_fAtkPoint);
					if (m_iPrevShield - 10 <= Shield)
						Shield = m_iPrevShield - 10;
				}

				if (Shield < 0)
					Shield = 0;
			}

			// 퀘스트 상태이면 특정 공격 할 시 공격한 플레이어의 포인트를 계산.
			if (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory())
			{
				// 귀신아귀의 검은 연기는 피격횟수를 계산하지 않는다.
				if (DT_DARK_POISON_GAS != damage_.GetDamageKind())
					g_pkQuestManager->IncreaseAttackedPoint(m_iPlayerIndex);
			}
		}
	}

	if (damage_.IsPlayerDamage())
	{
		g_MyD3D->MyPlayer[damage_.GetAttacker()]->ProcessBackStepBuff();

		if (g_MyD3D->MyPlayer[damage_.GetAttacker()]->IsMagicEffect(EGC_SKILL_HIDE))
		{
			g_MyD3D->MyPlayer[damage_.GetAttacker()]->Item_Remain_Time[GC_GAME_ITEM_HALLOW] = 0;

			g_pMagicEffect->ClearMagicEffect(g_MyD3D->MyPlayer[damage_.GetAttacker()]->m_iPlayerIndex, EGC_SKILL_HIDE, false, true);
		}
	}

	PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
	float fPlayerHP = pPlayer->m_fAbility[ABILITY_HP];
	float fMonScore = g_pkQuestManager->GetDungeonRankInfo(SiKGCRoomManager()->GetGameMode(), SiKGCRoomManager()->GetDungeonLevel(), 2);

	if (damage_.GetHPDelta() < 0.0f) {
		float fRankRatio = 0.f;
		g_pkQuestManager->GetRankGradeRatioValue(fRankRatio);
		g_pkQuestManager->SetTotalDamagePoint_S6(-(fDiff * fMonScore * fRankRatio), fPlayerHP, false);
		g_pkQuestManager->CalcDamageFromMonster(fDiff, fPlayerHP);
	}
	return fDiff;
}

void PLAYER::Kill(void)
{
	// 죽으면 버닝모드와 스왑웨폰을 원상태로!
	//SetNormalTpye();
	if (m_bIsBurnningMode)
		ChangeBurnningTemplate();

	m_bDirectKill = false;

	// 연습모드일때는 마릿수에 변화가 없음.
	if (Is_IfDie2End)
	{
		if (m_cLife > 0)
		{
			m_cLife--;
			UpdateTagCharInfo();
		}
	}
	else
	{
		if (m_cLife > 0)
		{
			m_cLife--;
			UpdateTagCharInfo();
		}

		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
		{
			if (GetNextTagChar() != GC_CHAR_INVALID && !IsDetermineWin)
			{
				Arena();
			}
		}
		else
		{
			if (m_cLife > 0 && !IsDetermineWin)
			{
				Arena();
			}
		}
	}
}

void PLAYER::SetSpecialAbility(OUT EAbilityData& stAbilityData_)
{
	// 불량 유저는 능력치를 낮춘다.
	if (m_kUserInfo.eGCUserLevel == USERLEVEL_2ND_ALERT)
	{
		stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= 0.5f;
		stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= 0.5f;
		stAbilityData_.fEquipItemAbility[ABILITY_HP] *= 0.5f;
	}
}

bool PLAYER::IsSlowAllowedMotion(UINT Motion)
{
	if (m_bUnstoppableMotion)
		return false;

	switch (Motion)
	{
	case MID_SIEG1_SKILL_BERSERK:
	case MID_SKILL_COMMON_ARME1_SPIRIT_RECOVERY:
	case MID_ELESIS123_BERSERK:
	case MID_LIRE_SPECIAL2:
	case MID_ARME_SPECIAL1_BLACK: //마법사 필살기는 슬로우 카운트 되면 안 된다.
	case MID_ARME_SPECIAL2_BLACK:
	case MID_ARME_SPECIAL3_BLACK:
	case MID_ARME_SPECIAL1:
	case MID_ARME_SPECIAL2:
	case MID_ARME_SPECIAL3:
	case MID_ARME2_SPECIAL1_BLACK: //연금술사 필살기는 슬로우 카운트 되면 안 된다.
	case MID_ARME2_SPECIAL2_BLACK:
	case MID_ARME2_SPECIAL3_BLACK:
	case MID_ARME2_SPECIAL1:
	case MID_ARME2_SPECIAL2:
		//case MID_ARME2_SPECIAL3:
	case MID_ELESIS2_SPECIAL1: //창병 필살기는 슬로우 카운트 되면 안 된다.
	case MID_ELESIS2_SPECIAL2:
	case MID_ELESIS2_SPECIAL3:
	case MID_ARME3_SPECIAL1:
	case MID_ARME3_SPECIAL2:
	case MID_ARME3_SPECIAL3:
	case MID_LAS3_SPECIAL2:
	case MID_LAS3_SPECIAL3:
	case MID_WOLF_FORM_END:
	case MID_LIRE4_SPECIAL2:
	case MID_LAS4_SPECIAL2_NANDO:
	case MID_LAS4_SPECIAL3:
	case MID_LAS4_SPECIAL3_FINISH_LAST:
	case MID_RONAN_SWORD_SPECIAL3_ATK:
	case MID_RYAN_NEW_SPECIAL3_ATK:
	case MID_RYAN4_ONE_HAND_SPECIAL1_2:
	case MID_RYAN4_ONE_HAND_SPECIAL2_2:
	case MID_RONAN3_SWORD_SPECIAL2_READY:
	case MID_RONAN3_SWORD_SPECIAL3_READY:
	case MID_RONAN3_MAGIC_SPECIAL2_READY:
	case MID_RONAN3_MAGIC_SPECIAL3_READY:
	case MID_RONAN3_SWORD_SPECIAL1_STOP:
	case MID_AMY2_PERFORMENCE_SPECIAL2_READY:
	case MID_RONAN4_MAGIC_SWORD_SPECIAL1_READY:
	case MID_RONAN4_WHITE_MAGIC_SPECIAL2_READY:
	case MID_SKILL_ARME1_WHITE2_ICECIRCULAR_LV1:
	case MID_SKILL_ARME1_WHITE2_ICECIRCULAR_LV2:
	case MID_SKILL_ARME1_WHITE2_ICESTRIKE:
	case MID_SKILL_ARME1_WHITE2_BASE_SPECIAL_LV2:
	case MID_SKILL_ARME1_W3_GORGOSFLAME_LV1:
	case MID_SKILL_ARME1_W3_GORGOSFLAME_LV2:
	case MID_SKILL_ARME1_B3_FINISHTOUCH_LV1:
	case MID_SKILL_ARME1_B2_SHOCKSTURN_LV2:
	case MID_SKILL_ARME1_W1_WALLOFICE:
	case MID_SKILL_ARME1_W1_BASE_MGSHIELD_LV2:
	case MID_SKILL_ARME1_W3_BASE_CURE_LV2:
	case MID_SKILL_ARME1_B2_DEATHCLOUD:
	case MID_SKILL_ARME1_W3_HOLYSTRIKE_LV2:
	case MID_SKILL_ARME1_W3_HOLYSTRIKE_LV1_READY:
	case MID_SKILL_ARME1_B1_FEAR_LV1:
	case MID_SKILL_ARME1_B1_FEAR_LV2:
	case MID_SKILL_ARME1_B1_HOLDBEAM_LV1:
	case MID_SKILL_ARME1_B1_HOLDBEAM_LV2:
	case MID_SKILL_ARME1_B3_SATELLITE_LV1_READY:
	case MID_SKILL_ARME1_B3_SATELLITE_LV2_READY:
	case MID_SKILL_ARME1_W1_PROTECTIONFIRE_LV1:
	case MID_SKILL_ARME1_W1_PROTECTIONFIRE_LV2:
	case MID_SKILL_ARME1_B3_SAINTBREEDING:
	case MID_SKILL_ARME1_B3_BIGBOOM_MOTION2:
	case MID_SKILL_ARME1_B1_BASE_STONE_LV2:
	case MID_AMY3_DANCE_SPECIAL2:
	case MID_AMY3_SPECIAL2:
	case MID_AMY3_SPECIAL3:
	case MID_AMY3_SPECIAL3_LEFT:
	case MID_JIN_SPECIAL3_FORCED:
	case MID_JIN_SPECIAL3_FORCED2:
	case MID_JIN1_SPECIAL2_FORCED:
	case MID_JIN1_SPECIAL1_FORCED:
	case MID_JIN1_SPECIAL3_READY:
	case MID_JIN1_SPECIAL3_FORCED_READY:
	case MID_SKILL_LIRE_METEO_STRIKE_LV1_JUMP:
	case MID_SKILL_LIRE_HOLYTRAP_START:
	case MID_SKILL_LIRE_FIRETRAP_START:
	case MID_SKILL_LIRE_THORNTRAP_START:
	case MID_SKILL_LIRE_DARKSHOT_START:
	case MID_SKILL_LIRE_SUMMON_ARCHER_LV1_START:
	case MID_SKILL_LIRE_SUMMON_ARCHER_LV1_ATK:
	case MID_SKILL_LIRE_SUMMON_ARCHER_LV2:
	case MID_SKILL_LIRE_DEMI_SHOCK_START:
	case MID_SKILL_LAS_IMPACTSLASH_LV2:
	case MID_SKILL_LAS_IMPACTSLASH_LV3:
	case MID_SKILL_LAS_BOMB_DART_START:
	case MID_SKILL_LAS_SPEEDY_MOVE_ATK_LV2_ATK:
	case MID_SKILL_LAS_VENOM_SLICE_START:
	case MID_SKILL_LAS_RAVEN_WALKER_START:
	case MID_SKILL_LAS_HONET_STING_START:
	case MID_SKILL_LAS_SUMMON_KAZEAZE_LV1:
	case MID_SKILL_LAS_SUMMON_KAZEAZE_LV2:
	case MID_SIEG1_SPECIAL2:
	case MID_SIEG1_SPECIAL2_F:
	case MID_JIN2_SPECIAL1_START:
	case MID_JIN2_SPECIAL1_FORCED_START:
	case MID_JIN2_SPECIAL2_START:
	case MID_JIN2_SPECIAL2_FORCED_START:
	case MID_JIN2_SPECIAL3_START:
	case MID_JIN2_SPECIAL3_START2:
	case MID_JIN2_SPECIAL3_FORCED_START:
	case MID_JIN2_SPECIAL3_FORCED_START2:
	case MID_SKILL_RYAN_EXT_SPECIAL1_1:
	case MID_SKILL_RYAN_SPECIAL2_LV3_2:
	case MID_SKILL_RYAN_GREAT_DEVIDE_START:
	case MID_SKILL_RYAN_GLORIOUS_START:
	case MID_SKILL_RYAN_WOLF_SPECIAL2_START:
	case MID_SKILL_RYAN_PHOENIX_ASSULT:
	case MID_SKILL_RYAN_HAMMERING:
		// m_bUnstoppableMotion 설정할 것
		return false;
	case MID_COMMON_DOWN_AND_STANDUP:
	case MID_COMMON_FRONTDOWN_AND_STANDUP:
	case MID_RYAN2_DOWN_AND_STAND_UP:
	case MID_COMMON_FRONTDOWN_AND_STANDUP_NO_COUNTER:
	{
		if (GetHP() == 0)
			return false;
		break;
	}
	}

	return true;
}

void PLAYER::SetSuper(char* SuperEffect, int iParticleNum, float fOffsetX, float fTime, bool bUsingIgnore, bool bUseImmortal /*= true*/)
{
	// 무적중이면 무시하는 옵션
	if (bUsingIgnore && g_MyD3D->MyPlayer[m_iPlayerIndex]->Item_Remain_Time[GC_GAME_ITEM_SUPER])
		return;

	float fArenaTime;
	if (0.0f < fTime)
	{
		fArenaTime = fTime;
	}
	else
	{
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH)
		{
			fArenaTime = (float)ARENA_SUPER_TIME_FATAL;
		}
		else
		{
			fArenaTime = (float)ARENA_SUPER_TIME;
		}
	}

	fArenaTime += fArenaTime * m_fAbility[ABILITY_ARENA_ABSENCE_TIME];

	g_MyD3D->MyPlayer[m_iPlayerIndex]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = (int)fArenaTime;
	if (g_MyD3D->MyPlayer[m_iPlayerIndex]->m_pObject && bUseImmortal)
		g_MyD3D->MyPlayer[m_iPlayerIndex]->m_pObject->MeshStateOn(MS_IMMORTAL);

	if (strcmp(SuperEffect, "") != 0)
		g_MyD3D->MyPlayer[m_iPlayerIndex]->AddParticle(SuperEffect, iParticleNum, fOffsetX);
}

void PLAYER::SetSuper_(int value)
{
	int SuperTime;
	SuperTime = value;
	g_MyD3D->MyPlayer[m_iPlayerIndex]->Super = SuperTime;
}

bool PLAYER::IsOldMotion(int iMotion)
{
	if (iMotion == Before_Motion)
		return true;

	return false;
}

void PLAYER::SetFlashEffectFrame(int iFlashEffectFrame)
{
	if (g_MyD3D->m_KGCOption.IsHiteffect())
	{
		m_iFlashEffectFrame = iFlashEffectFrame;
		if (g_MyD3D->m_KGCOption.IsCartoon())
			m_iFlashEffectFrame += CARTOON_FLASH_EFFECT_FRAME;
	}
}

void PLAYER::SetShakeEffectFrame(int iShakeEffectFrame)
{
	if (g_MyD3D->m_KGCOption.IsHiteffect())
	{
		m_iShakeEffectFrame = iShakeEffectFrame;
	}
}

void PLAYER::SetShowEquipEffect(EMIT_POSITION_TYPE ePos, bool bShow)
{
	for (std::vector<KGCAnimManager::ItemAnim>::iterator j = m_ItemAnim.begin(); j != m_ItemAnim.end(); j++)
	{
		if ((*j).positionType == ePos)
		{
			(*j).animInst->SetShow(bShow);
		}
	}

	for (std::vector<ITEM_EFFECT>::iterator i = m_vecItemEff.begin(); i != m_vecItemEff.end(); i++)
	{
		for (unsigned int j = 0; j < (*i).pSeq.size(); j++)
		{
			if ((*i).emitPositionType[j / (*i).sequenceName.size()] == ePos)
				(*i).pSeq[j]->SetShow(bShow);
		}
	}
}

void PLAYER::ToggleRenderPlayer(bool bShow)
{
	m_bRender = bShow;
	m_bRenderQuestDrama = bShow;
	ShowAllItemEffect(bShow);

	if (bShow == false)
	{
		if (m_pObject)
			m_pObject->MeshStateOff(MS_INVISIBLE_ENEMY);
		if (g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject)
			g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject->MeshStateOff(MS_INVISIBLE_ENEMY);
	}
	else
	{
		if (m_pObject)
			m_pObject->MeshStateOn(MS_INVISIBLE_ENEMY);
		if (g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject)
			g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject->MeshStateOn(MS_INVISIBLE_ENEMY);
	}
}

void PLAYER::SetEquipPetInfo(char cCharType, GCITEMUID dwUID)
{
	KPetInfo& kPet = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(cCharType).kPetInfo;

	if (dwUID == 0)
	{
		kPet.Init();
	}
	else
	{
		std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find(dwUID);
		if (mit != g_MyD3D->m_mapPetInfo.end())
		{
			kPet = mit->second;
		}
	}
}

void PLAYER::AutoEquipPetItem()
{
	SCharInfo& SChar = GetCurrentChar();
	KPetInfo& kPet = SChar.kPetInfo;

	// 스킬을 장착시키자!
	int iPetType = g_kGlobalValue.GetPetIDFromItemID(kPet.m_dwID);
	int iPetEvolution = static_cast<int>(kPet.m_cPromotion);
	DWORD dwEquipItem = 0;

	const SPetTemplate& sPetTemplate = g_kGlobalValue.GetPetTemplate(iPetType, iPetEvolution);

	dwEquipItem = 0;

	if (g_pItemMgr->FindInventoryForItemID(sPetTemplate.SkillGP))
		dwEquipItem = sPetTemplate.SkillGP;
	else if (g_pItemMgr->FindInventoryForItemID(sPetTemplate.SkillCash))
		dwEquipItem = sPetTemplate.SkillCash;
	else if (g_pItemMgr->FindInventoryForItemID(sPetTemplate.SkillAwakeningGP))
		dwEquipItem = sPetTemplate.SkillAwakeningGP;
	else if (g_pItemMgr->FindInventoryForItemID(sPetTemplate.SkillAwakeningCash))
		dwEquipItem = sPetTemplate.SkillAwakeningCash;

	// 아이템을 장착시키자
	if (dwEquipItem != 0)
	{
		EquipPetItem(dwEquipItem);
	}
}

void PLAYER::EquipPetItem(DWORD dwItemID)
{
	GCItem* itemInfo = NULL;
	KItem* kItemInfo = NULL;

	if (g_pItemMgr->FindInventoryForItemID(dwItemID, &kItemInfo, &itemInfo) == false)
		return;

	if (itemInfo == NULL || kItemInfo == NULL)
	{
		return;
	}

	SCharInfo& SChar = GetCurrentChar();
	KPetInfo& kPet = SChar.kPetInfo;

	if (kPet.m_dwUID == 0)
	{
		// 장착 중인 펫이 없습니다
		// 펫 장착 후 사용해 주세요
		std::wstring str1 = g_pkStrLoader->GetString(STR_ID_PET_EQUIP_ITEM_MESSAGE1);
		std::wstring str2 = g_pkStrLoader->GetString(STR_ID_PET_EQUIP_ITEM_MESSAGE2);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, str1, str2);
		return;
	}

	if (itemInfo->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_SKILL &&
		(false == IsUseEquipPetItem(itemInfo, kPet.m_dwID) ||
			itemInfo->shPetEquipItemPromotionLevel != kPet.m_cPromotion))
	{
		// @1 는
		// 장착한 펫이 사용할 수 없는 기술입니다
		std::wstring str1 = g_pkStrLoader->GetReplacedString(STR_ID_PET_EQUIP_ITEM_MESSAGE3, "l", itemInfo->strItemName);
		std::wstring str2 = g_pkStrLoader->GetReplacedString(STR_ID_PET_EQUIP_ITEM_MESSAGE4, "l", kPet.m_strName);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, str1, str2);
		return;
	}

	KSimpleItem kSimpleitem;
	kSimpleitem.m_dwID = kItemInfo->m_ItemID;
	kSimpleitem.m_dwUID = kItemInfo->m_ItemUID;

	for (int iPetEquipIndex = 0; iPetEquipIndex < (int)kPet.m_vecEquipItem.size(); iPetEquipIndex++)
	{
		GCItem* kPetItem = NULL;
		KItem* kItemInfo = NULL;

		g_pItemMgr->FindInventory(kPet.m_vecEquipItem[iPetEquipIndex].m_dwUID, &kItemInfo, &kPetItem);
		RETURN_NIL(kPetItem);

		// 펫에 장착된 아이템과 현재 장착하려는 아이템의 슬롯포지션이 같으면 이전에 있던 건 장착해제 시켜준다
		if (kPetItem->dwPetEquipItemSlot == itemInfo->dwPetEquipItemSlot)
		{
			kPet.m_vecEquipItem.erase(kPet.m_vecEquipItem.begin() + iPetEquipIndex);
			--iPetEquipIndex;
		}
	}
	kPet.m_vecEquipItem.push_back(kSimpleitem);
	g_MyD3D->m_mapPetInfo[kPet.m_dwUID].m_vecEquipItem = kPet.m_vecEquipItem;

	// 다른 캐릭터 중 같은 펫을 끼고 있는 녀석이 있으면 거기에도 장착정보를 바꿔준다
	for (int iCharIndex = 0; iCharIndex < (int)m_kUserInfo.vecCharInfo.size(); iCharIndex++)
	{
		SCharInfo& SChar_ = m_kUserInfo.vecCharInfo[iCharIndex];
		KPetInfo& kPet_ = SChar_.kPetInfo;

		if (SChar.iCharType == SChar_.iCharType)
		{
			continue;
		}

		//모든 캐릭터의 펫 스킬을 로드하도록 수정했습니다.Luma 가 제작했습니다.
		if (kPet.m_dwUID == kPet_.m_dwUID)
		{
			m_kUserInfo.vecCharInfo[iCharIndex].kPetInfo = kPet;
		}
	}
}

void PLAYER::EquipPetItemByItemUID(GCITEMUID dwItemUID_)
{
	GCItem* itemInfo = NULL;
	KItem* kItemInfo = NULL;

	if (g_pItemMgr->FindInventory(dwItemUID_, &kItemInfo, &itemInfo) == false)
		return;

	if (itemInfo == NULL || kItemInfo == NULL)
	{
		return;
	}

	SCharInfo& SChar = GetCurrentChar();
	KPetInfo& kPet = SChar.kPetInfo;

	if (kPet.m_dwUID == 0)
	{
		// 장착 중인 펫이 없습니다
		// 펫 장착 후 사용해 주세요
		std::wstring str1 = g_pkStrLoader->GetString(STR_ID_PET_EQUIP_ITEM_MESSAGE1);
		std::wstring str2 = g_pkStrLoader->GetString(STR_ID_PET_EQUIP_ITEM_MESSAGE2);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, str1, str2);
		return;
	}

	if (itemInfo->dwPetEquipItemSlot == EPET_EQUIP_ITEM_SLOT_SKILL &&
		(false == IsUseEquipPetItem(itemInfo, kPet.m_dwID) ||
			itemInfo->shPetEquipItemPromotionLevel != kPet.m_cPromotion))
	{
		// @1 는
		// 장착한 펫이 사용할 수 없는 기술입니다
		std::wstring str1 = g_pkStrLoader->GetReplacedString(STR_ID_PET_EQUIP_ITEM_MESSAGE3, "l", itemInfo->strItemName);
		std::wstring str2 = g_pkStrLoader->GetReplacedString(STR_ID_PET_EQUIP_ITEM_MESSAGE4, "l", kPet.m_strName);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, str1, str2);
		return;
	}

	KSimpleItem kSimpleitem;
	kSimpleitem.m_dwID = kItemInfo->m_ItemID;
	kSimpleitem.m_dwUID = kItemInfo->m_ItemUID;

	for (int iPetEquipIndex = 0; iPetEquipIndex < (int)kPet.m_vecEquipItem.size(); iPetEquipIndex++)
	{
		GCItem* kPetItem = NULL;
		KItem* pInven = NULL;

		g_pItemMgr->FindInventory(kPet.m_vecEquipItem[iPetEquipIndex].m_dwUID, &pInven, &kPetItem);
		RETURN_NIL(kPetItem);

		// 펫에 장착된 아이템과 현재 장착하려는 아이템의 슬롯포지션이 같으면 이전에 있던 건 장착해제 시켜준다
		if (kPetItem->dwPetEquipItemSlot == itemInfo->dwPetEquipItemSlot)
		{
			kPet.m_vecEquipItem.erase(kPet.m_vecEquipItem.begin() + iPetEquipIndex);
			--iPetEquipIndex;
		}
	}
	kPet.m_vecEquipItem.push_back(kSimpleitem);
	g_MyD3D->m_mapPetInfo[kPet.m_dwUID].m_vecEquipItem = kPet.m_vecEquipItem;

	// 다른 캐릭터 중 같은 펫을 끼고 있는 녀석이 있으면 거기에도 장착정보를 바꿔준다
	for (int iCharIndex = 0; iCharIndex < (int)m_kUserInfo.vecCharInfo.size(); iCharIndex++)
	{
		SCharInfo& SChar_ = m_kUserInfo.vecCharInfo[iCharIndex];
		KPetInfo& kPet_ = SChar_.kPetInfo;

		if (SChar.iCharType == SChar_.iCharType)
		{
			continue;
		}

		//모든 캐릭터의 펫 스킬을 로드하도록 수정했습니다.Luma 가 제작했습니다.
		if (kPet.m_dwUID == kPet_.m_dwUID)
		{
			m_kUserInfo.vecCharInfo[iCharIndex].kPetInfo = kPet;
		}
	}
}

bool PLAYER::IsUseEquipPetItem(GCItem* itemInfo, int iPetID)
{
	if (g_kGlobalValue.IsEquipSkill(itemInfo->dwGoodsID, iPetID)
		|| (itemInfo->dwPetGoodsIDOfOriginalPetEquipItemOfSkill == iPetID * 10))
	{
		return true;
	}

	return false;
}

void PLAYER::UnEquipPet(char cCharType)
{
	// 펫 : 펫장착 해제
	SCharInfo& SChar = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar(cCharType);
	KPetInfo& kPet = SChar.kPetInfo;

	GCITEMUID dwPetUID = kPet.m_dwUID;

	// 캐릭터의 펫정보를 지워버린다
	kPet.Init();

	// 다른 캐릭터 중 혹시 같은 펫을 장착하고 있는 사람이 있다면
	// Myd3d의 m_mapPetInfo 는 건들이지 않고 아무도 안끼고 있다면
	// m_mapPetInfo의 장착정보를 초기화 시킨다
	for (int iCharIndex = 0; iCharIndex < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); iCharIndex++)
	{
		SCharInfo SChar_ = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[iCharIndex];
		KPetInfo kPet_ = SChar_.kPetInfo;

		// 누군가 아직 해당 펫을 끼고 있다면 맵의 정보는 지우지 않는다
		if (dwPetUID == kPet_.m_dwUID)
			return;
	}

	// 아무도 현재 펫을 끼고 있지 않다면 여기서 장착정보를 지워준다
	std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find(dwPetUID);
	if (mit != g_MyD3D->m_mapPetInfo.end())
	{
		mit->second.m_vecEquipItem.clear();
	}
}

void PLAYER::UnEquipPetforUID(GCITEMUID dwPetUID)
{
	if (g_MyD3D->m_mapPetInfo.count(dwPetUID) == 0)
		return;

	// 펫의 장착정보를 지워준다
	g_MyD3D->m_mapPetInfo[dwPetUID].m_vecEquipItem.clear();

	// 파라메터로 받은 UID를 가진 펫을 끼고 있다면 모두 벗긴다
	for (int iCharIndex = 0; iCharIndex < (int)g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.size(); iCharIndex++)
	{
		SCharInfo& SChar = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo[iCharIndex];
		KPetInfo& kPetInfo = SChar.kPetInfo;

		if (dwPetUID == kPetInfo.m_dwUID)
		{
			kPetInfo.Init();
		}
	}
}

int PLAYER::GetShakeValue(float fDamage, float fHPpoint)
{
	int iShakeValue = 0;
	float fDamagePerHP = (fDamage / fHPpoint) * (-100);

	if (fDamagePerHP < 2.0f)
	{
		iShakeValue = 0;
	}
	else if (fDamagePerHP < 7.0f)
	{
		iShakeValue = 3;
	}
	else if (fDamagePerHP < 15.0f)
	{
		iShakeValue = 6;
	}
	else if (fDamagePerHP < 25.0f)
	{
		iShakeValue = 9;
	}
	else
	{
		iShakeValue = 20;
	}

	return iShakeValue;
}

void PLAYER::HandFireFrameMove()
{
	if (m_pFireLeftTip == NULL && m_pFireRightTip == NULL)
		return;

	if (bIsRight)
	{
		if (m_pFireRightTip != NULL)
			g_ParticleManager->SetLayer(m_pFireRightTip, GC_LAYER_BETWEEN_CHAR_FRONT);
		if (m_pFireLeftTip != NULL)
			g_ParticleManager->SetLayer(m_pFireLeftTip, GC_LAYER_BETWEEN_BLOCK_CHAR);
	}
	else
	{
		if (m_pFireRightTip != NULL)
			g_ParticleManager->SetLayer(m_pFireRightTip, GC_LAYER_BETWEEN_BLOCK_CHAR);
		if (m_pFireLeftTip != NULL)
			g_ParticleManager->SetLayer(m_pFireLeftTip, GC_LAYER_BETWEEN_CHAR_FRONT);
	}
}

void PLAYER::UpdateFormResource(int iFormID, OUT GCObject* objOut/* = NULL*/)
{
	if (m_kUserInfo.cCharIndex == -1)
	{
		return;
	}

	// 변신 모델 넣기
	GCObject* pObj = NULL;
	if (objOut == NULL)
	{
		SAFE_REMOVE_DELETE(m_pObjMetaForm[iFormID]);
		m_pObjMetaForm[iFormID] = new GCObject();
		pObj = m_pObjMetaForm[iFormID];
	}
	else
	{
		pObj = objOut;
	}

	bool IsItemExist = false;
	std::vector<int> vtVectorIDMesh;
	std::vector<int> vtVectorIDTexture;

	//모델을 불러옵니다.
	for (int i = 0; i < (int)m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel.size(); ++i)
	{
		if (m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel[i].iGoodsID == -1 && !IsItemExist)
		{
			vtVectorIDMesh.push_back(i);
		}
		if (g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel[i].iGoodsID))
		{
			IsItemExist = true;
			vtVectorIDMesh.push_back(i);
			break;
		}
	}

	//모델을 설정.
	if (0 == vtVectorIDMesh.size())
	{
		ASSERT(!"스크립트에 모델이 설정되어 있지 않음");
	}

	//텍스쳐를 불러옵니다.
	IsItemExist = false;
	for (int i = 0; i < (int)m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture.size(); ++i)
	{
		if (m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture[i].iGoodsID == -1 && !IsItemExist)
		{
			vtVectorIDTexture.push_back(i);
		}
		if (g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture[i].iGoodsID))
		{
			IsItemExist = true;
			vtVectorIDTexture.push_back(i);
			break;
		}
	}

	//텍스쳐를 설정.
	if (0 == vtVectorIDTexture.size())
	{
		ASSERT(!"스크립트에 텍스쳐가 설정되어 있지 않음");
	}

	for (int i = 0; i < (int)vtVectorIDMesh.size(); ++i)
	{
		std::string strMesh = "";
		std::string strTex = "";

		strMesh = m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel[vtVectorIDMesh[i]].strModelFileName;

		if ((int)vtVectorIDTexture.size() > i)
			strTex = m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture[vtVectorIDTexture[i]].strTextureFileName;

		//         pObj->AddMesh( g_pGCDeviceManager2->CreateMeshP3M( strMesh ),
		//             g_pGCDeviceManager2->CreateTexture(strTex) );
		pObj->AddStaticMesh(strMesh, strTex);
	}

	// ExtraMesh 있으면 붙여줍니다.
	if (!m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisExtraMesh.empty()) {
		std::vector< EXTRA_MESH >::const_iterator itExtra = m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisExtraMesh.begin();
		for (; itExtra != m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisExtraMesh.end(); ++itExtra) {
			GCObject* pExtraMesh = g_pGCDeviceManager2->CreateLiveModel(itExtra->strMeshName, itExtra->strTextureName, itExtra->strMotionName, itExtra->iBoneId, pObj, itExtra->vOptionalPos);
			if (pExtraMesh) {
				pExtraMesh->m_strName = itExtra->strID;
				pExtraMesh->m_bIsExtraObject = true;
				pExtraMesh->SetTechnique(itExtra->dwTechnique);
				pExtraMesh->SetRender(false);
			}
		}
	}

	std::wstringstream strmPlayerName;
	SAbilityRevision* pAbilityRevision = SiKGCWorldMapManager()->GetAbilityRevisionInfo(SiKGCRoomManager()->GetGameMode());
	bool bAbilityRevision = (pAbilityRevision && pAbilityRevision->bLoadTable);
	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL &&
		false == g_MyD3D->m_pkQuestGameMgr->GetReviseCharLevel() &&
		false == bAbilityRevision)
		strmPlayerName << g_pkStrLoader->GetString(STR_ID_LEVEL).c_str() << L" " << GetCurrentChar().iLevel << L". ";

	strmPlayerName << GetStrUserName();

	// 같은 팀이라면 녹색, 다른 팀이라면 적색으로 표시한다.
	D3DCOLOR Color;
	if (!g_MyD3D->IsMyTeam(m_iPlayerIndex)) {
		Color = 0xFFFF0000;
	}
	else {
		Color = SiGCTextColorManager()->StringToColor(GetStrUserColor());
	}

	pObj->SetText(0, strmPlayerName.str(), Color);

	m_prPlayerNameWithColor.first = strmPlayerName.str();
	m_prPlayerNameWithColor.second = Color;

	char tmp[128];
	sprintf(tmp, "character_%d.dds", GetCurrentChar().iCharType);
	pObj->SetText(1, strmPlayerName.str(), Color, 0xFF000000, tmp);
	pObj->Build();

	if (pObj->GetMeshObjectNum() == 0)
	{
#ifdef _DEBUG
		pObj->m_strName = "변신 모양, 메쉬가 없다!";
#endif
	}
}

// 2007/3/14. iridology. 아... 이딴식으로 짜다니 T^T 죄송
void PLAYER::UpdateFormResource(int iFormID, TagCharacterInfo* pTagInfo)
{
	if (pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.iFormID == FORM_NORMAL)
		return;

	// 변신 모델 넣기
	//GCObject* pObj = NULL;
	//pObj = new GCObject();
	SAFE_REMOVE_DELETE(pTagInfo->m_pObjMetaForm[iFormID]);
	pTagInfo->m_pObjMetaForm[iFormID] = new GCObject();

	bool IsItemExist = false;
	std::vector<int> vtVectorIDMesh;
	std::vector<int> vtVectorIDTexture;

	//모델을 불러옵니다.
	for (int i = 0; i < (int)pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel.size(); ++i)
	{
		if (pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel[i].iGoodsID == -1 && !IsItemExist)
		{
			vtVectorIDMesh.push_back(i);
		}
		if (g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel[i].iGoodsID))
		{
			IsItemExist = true;
			vtVectorIDMesh.push_back(i);
			break;
		}
	}

	//모델을 설정.
	if (0 == vtVectorIDMesh.size())
	{
		ASSERT(!"스크립트에 모델이 설정되어 있지 않음");
	}

	//텍스쳐를 불러옵니다.
	int iTextureVectorID = -1;
	IsItemExist = false;
	for (int i = 0; i < (int)pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture.size(); ++i)
	{
		if (pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture[i].iGoodsID == -1 && !IsItemExist)
		{
			vtVectorIDTexture.push_back(i);
		}
		if (g_pItemMgr->CheckEquipItemForItemID(&GetCurrentChar(), pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture[i].iGoodsID))
		{
			IsItemExist = true;
			vtVectorIDTexture.push_back(i);
			break;
		}
	}

	//텍스쳐를 설정.
	if (0 == vtVectorIDTexture.size())
	{
		ASSERT(!"스크립트에 텍스쳐가 설정되어 있지 않음");
	}

	for (int i = 0; i < (int)vtVectorIDMesh.size(); ++i)
	{
		std::string strMesh = "";
		std::string strTex = "";

		strMesh = pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisModel[vtVectorIDMesh[i]].strModelFileName;

		if ((int)vtVectorIDTexture.size() > i)
			strTex = pTagInfo->m_FormTemplate[iFormID].sMetamorphosisTemplate.vecMetamorphosisTexture[vtVectorIDTexture[i]].strTextureFileName;

		pTagInfo->m_pObjMetaForm[iFormID]->AddMesh(g_pGCDeviceManager2->CreateMeshP3M(strMesh), g_pGCDeviceManager2->CreateTexture(strTex));
	}

#ifdef _DEBUG
	if (pTagInfo->m_pObjMetaForm[iFormID]->GetMeshObjectNum() == 0)
	{
		pTagInfo->m_pObjMetaForm[iFormID]->m_strName = "변신 모양, 태그에서 생성, 메쉬가 없다!";
	}
#endif

	//if( pObj != NULL )
	//{
	//    pTagInfo->m_vecFormItemModel.push_back( pObj );
	//}
}

void PLAYER::ReleaseTagPlayerResource()
{
	std::map< int, TagCharacterInfo >::iterator mit;
	for (mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit)
	{
		// 이놈은 항상 만들기 때문에 지워줘야함
		mit->second.EffectClear(m_iPlayerIndex);

		// 마지막에 진행하던 케릭터는 빼자
		if (mit->second.m_bNeedDelete == FALSE)
			continue;

		//std::vector< GCObject* >& vecdel = mit->second.m_vecItemModel;
		//for( int i = 0; i < (int)vecdel.size(); ++i )
		//{
		//    SAFE_DELETE( vecdel[i] );
		//}

		mit->second.m_pLire3Arrow = NULL;

		for (int i = 0; i < FORM_NUM; ++i) {
			SAFE_DELETE(mit->second.m_pObjMetaForm[i]);
		}

		//SAFE_DELETE( mit->second.m_pFormShadow );

		mit->second.Init();
	}
	m_mapTagCharInfo.clear();
	m_vecPetAiID.clear();

	// 쉽게가려면 여기밖에 없다~!!!!!!
	if (g_MyD3D->m_akPet[m_iPlayerIndex])
		g_MyD3D->m_akPet[m_iPlayerIndex]->ReleaseTagPetResource();
}

bool PLAYER::VerifyMotion(int iMotion)
{
	return g_pGraphicsHelper->Verify((char)Extra_Char_Num, iMotion);
}

bool PLAYER::VerifyRealMotion(int iMotion)
{
	return g_pGraphicsHelper->Verify(iMotion);
}

bool PLAYER::IsTagAvailable()
{
	if (IsDetermineWin == true)
		return false;

	if (uiMotion == GetOwnMotion(MID_COMMON_ARENA) ||
		uiMotion == GetOwnMotion(MID_COMMON_DROP) ||
		uiMotion == GetOwnMotion(MID_COMMON_INTO_THE_TELEPORT) ||
		uiMotion == GetOwnMotion(MID_COMMON_TIGER_ROLLING_ACTION) ||
		IsCatchMotion(uiMotion) ||
		IsDamagedMotion() ||
		uiMotion == MID_WOLF_FORM_END ||
		uiMotion == MID_NEPHILLIM_FORM_END ||
		uiMotion == MID_WOLF_FORM_START ||
		uiMotion == MID_NEPHILLIM_FORM_START ||
		uiMotion == MID_RYAN4_MDAN_FORM_START ||
		uiMotion == MID_RYAN4_MDAN_FORM_END ||
		uiMotion == MID_RYAN4_ONE_HAND_FORM_START ||
		uiMotion == MID_RYAN4_ONE_HAND_FORM_END ||
		uiMotion == MID_RYAN4_FORM_START ||
		uiMotion == MID_RYAN4_FORM_END ||
		uiMotion == MID_SKILL_RYAN_NATURESHIELD ||
		uiMotion == MID_SKILL_RYAN_NATURESHIELD_STANDUP ||
		uiMotion == MID_SIEG1_SKILL_LIVING_FREE ||
		uiMotion == MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV1 ||
		uiMotion == MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV1_FURY ||
		uiMotion == MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV2 ||
		uiMotion == MID_SIEG1_SKILL_CASH_MATCH_SPECIAL3_LV2_FURY ||
		uiMotion == MID_RIN4_FORM_GODDESS_START ||
		uiMotion == MID_RIN4_FORM_GODDESS_END ||
		uiMotion == MID_RIN4_FORM_EVIL_START ||
		uiMotion == MID_RIN4_FORM_EVIL_END ||
		uiMotion == MID_RIN4_FORM_GODDESS_SPECIAL3_TO_END ||
		uiMotion == MID_RIN4_FORM_EVIL_SPECIAL3_TO_END ||
		uiMotion == MID_RIN4_FORM_OFF_END)
		return false;

	if (IsLocalPlayer() && m_dwPrevTagTime != 0)
		return false;

	if (IsMagicEffect(EGC_SIEG1_SKILL_CASH_SPECIAL3_BUFF, true))
		return false;

	if (IsMagicEffect(EGC_EFFECT_RIN4_FORM_GODDESS_DECO_PARTICLE, true) || IsMagicEffect(EGC_EFFECT_RIN4_FORM_EVIL_DECO_PARTICLE, true))
		return false;

	return true;
}

void PLAYER::SetTagMotion()
{
	if (m_vecTagCharNum.size() < 2 || IsTagAvailable() == false)
		return;

	if (uiMotion == GetCurFormTemplate().sTagTemplate.m_uiStartMotion)
		return;

	Direct_Motion_Input(GetCurFormTemplate().sTagTemplate.m_uiStartMotion);
	cFrame = 0;
	NoCheckContact = 20;

	if (IsMagicEffect(EGC_EFFECT_MARI4_REPAIR)) {
		ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR);
	}
	if (IsMagicEffect(EGC_EFFECT_MARI4_REPAIR_R)) {
		ClearMagicEffect(EGC_EFFECT_MARI4_REPAIR_R);
	}

	if (Extra_Char_Num == CID_MARI2)
		InitHittingGauge();
}

void PLAYER::TagCharacter(bool bArena)
{
	if (m_kUserInfo.bLive == false)
		return;

	if (IsMagicEffect(EGC_EFFECT_RIN_RAGE_MODE_BUFF)) {
		ClearMagicEffect(EGC_EFFECT_RIN_RAGE_MODE_BUFF);
	}

	// 다음에 나올 녀석 Push
	int iNextChar;
	std::vector< int >::iterator vit;
	vit = m_vecTagCharNum.begin();
	iNextChar = *vit;
	m_vecTagCharNum.erase(m_vecTagCharNum.begin());
	m_vecTagCharNum.push_back(iNextChar);

	// TagCharacterInfo Set
	std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.find(iNextChar);
	if (mit == m_mapTagCharInfo.end())
	{
		ASSERT(!"바꾸려는 Character 없음");
		return;
	}

	TagCharacterInfo& tagInfo = mit->second;

	// 현재꺼 저장 -> 바뀔놈꺼 Set
	std::map< int, TagCharacterInfo >::iterator mitCur = m_mapTagCharInfo.find(GetCurrentChar().iCharType);
	TagCharacterInfo& CurrentInfo = mitCur->second;

	CurrentInfo.InitTagCharData(this);
	CurrentInfo.ShowEffect(false);

	// Arena 된거면 현재꺼 죽은것임
	if (!IsLocalPlayer() && bArena)
	{
		m_cLife--;
		UpdateTagCharInfo();
	}

	// PlayerTemplate가 바뀌기 전에 Base Effect 처리
	BaseEffectClear();

	m_kUserInfo.SetCurrentChar(tagInfo.m_iCharType);
	SetEPlayerState(tagInfo.m_ePlayerState);

	// memcpy는 불안해..
	for (int i = 0; i < FORM_NUM; ++i)
		m_FormTemplate[i] = tagInfo.m_FormTemplate[i];

	for (int i = 0; i < BTYPE_NUM; ++i)
		m_BurnningTemplate[i] = tagInfo.m_BurnningTemplate[i];

	m_SwapTemplate = tagInfo.m_SwapTemplate;
	Extra_Char_Num = tagInfo.Extra_Char_Num;
	m_cLife = tagInfo.m_cLife;
	m_fAbility[ABILITY_ATK] = tagInfo.m_fAbility[ABILITY_ATK];
	m_fAbility[ABILITY_DEF] = tagInfo.m_fAbility[ABILITY_DEF];
	m_fAbility[ABILITY_HP] = tagInfo.m_fAbility[ABILITY_HP];
	m_iSkillType = tagInfo.m_iSkillType;
	m_iWeaponType = tagInfo.m_iWeaponType;
	MetamorphosisForm = tagInfo.MetamorphosisForm;
	SetHP(tagInfo.m_fHP);

	for (int i = 0; i < ABILITY_MAX; ++i)
	{
		m_fAbility[i] = tagInfo.m_fAbility[i];
	}

	for (int i = 0; i < FORM_NUM; ++i)
	{
		if (tagInfo.m_pObjMetaForm[i])
		{
			tagInfo.m_pObjMetaForm[i]->SetMeshState(m_pObject->GetMeshState());
			tagInfo.m_pObjMetaForm[i]->SetShellMatrixScale(m_pObject->GetShellMatrixScale());
		}
	}

	m_mapwSkillItem = tagInfo.m_mapwSkillItem;

	for (int i = 0; i < FORM_NUM; ++i)
		m_pObjMetaForm[i] = tagInfo.m_pObjMetaForm[i];

	m_pLire3Arrow = tagInfo.m_pLire3Arrow;
	m_bSuperArmor = tagInfo.m_bSuperArmor;

	m_bHyperArmor = tagInfo.m_bHyperArmor;

	g_RenderManager->RemoveObject(m_pObject);

	if (MetamorphosisForm == FORM_NORMAL)
	{
		SetSkillType(m_iSkillType);
		g_RenderManager->AddObject(GetMetaForm(FORM_NORMAL));
		m_pObject = GetMetaForm(FORM_NORMAL);
	}
	else
	{
		g_RenderManager->AddObject(GetMetaForm(MetamorphosisForm));
		m_pObject = GetMetaForm(MetamorphosisForm);
	}

	m_vecSpecialAbility = tagInfo.m_vecSpecialAbility;
	m_setExtra2 = tagInfo.m_setExtra2;

	if (IsLocalPlayer())
		g_MyD3D->MyHead->IsBlackMagic = tagInfo.m_bIsBlackMagic;

	m_vecItemEff = tagInfo.m_vecItemEff;
	m_vecDamageEff = tagInfo.m_vecDamageEff;
	m_vecSkillEff = tagInfo.m_vecSkillEff;

	m_ItemAnim = tagInfo.m_ItemAnim;
	m_DamageAnim = tagInfo.m_DamageAnim;
	m_SkillAnim = tagInfo.m_SkillAnim;

	m_pLasFireWingLeft = tagInfo.m_pLasFireWingLeft;
	m_pLasFireWingRight = tagInfo.m_pLasFireWingRight;
	m_pDashEff = tagInfo.m_pDashEff;
	m_pLasMotalBlowReady = tagInfo.m_pLasMotalBlowReady;
	m_pLasMotalBlowReadyAnim = tagInfo.m_pLasMotalBlowReadyAnim;

	m_Elemental = tagInfo.m_Elemental;
	m_StrongLevel = tagInfo.m_StrongLevel;

	Las4GreatSword = tagInfo.Las4GreatSword;

	m_mapOverlappedPair = tagInfo.m_mapOverlappedPair;

	m_vecHotKeySkill = tagInfo.m_vecHotKeySkill;
	// 후.. 일단은 이방법밖에
	HandFireShow(MetamorphosisForm == FORM_NEPHILIM);

	m_vecPetAiID = tagInfo.m_vecPetAiID;

	tagInfo.ShowEffect(true);

	char CharID = (char)Extra_Char_Num;
	CHAR_IDENTY cCharID = (CHAR_IDENTY)CharID;
	m_posDesc.charID = cCharID;

	if (IsLocalPlayer())
	{
		All_Latency_Equal();
		pLatency = LATENCY;

		if (!bArena)
		{
			g_kTagCharacter->cPlayerIndex = static_cast<char>(g_MyD3D->Get_MyPlayer());
			g_kTagCharacter->bArena = bArena;

			g_MyD3D->MyReplay->Set_TagCharacter(g_kTagCharacter, g_kTagCharacter->cPlayerIndex);
		}
		//         g_MyD3D->MyHead->Now_HP[m_iPlayerIndex]   = GetHP();
		g_MyD3D->MyHead->Level = tagInfo.m_fMana;
		m_dwPrevTagTime = GC_FPS_LIMIT * 3;

		// 소환 몬스터 모두 죽이기
		KGC_PID_BROAD_SUMMON_MONSTER_HP kPacket;
		kPacket.dwUID = m_kUserInfo.dwUID;
		kPacket.fHpRadio = 0.0f;
		SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
		KGCPC_MEMO("KGC_PID_BROAD_SUMMON_MONSTER_HP");

		// 떨어지는 속도처리( 레이 에테르, 플롯과 같은 기술로 인해.. )
		m_fFall_Speed = 0.002f;

		// 괜히한거 같군..
		//2013-08 매치 개선 UI 들어가면서 tagmatch 작업 고려 안함 (뺀 컨텐츠)
		//if( g_pkUIScene->m_pkGameOverlayUI->GetHeadUp() )
		//    g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->SetNextTagTime( tagInfo.m_iCharType, m_dwPrevTagTime );
	}
	else
	{
		for (int i = 0; i < LATENCY; i++)
		{
			L_Now_Motion[i] = GetCurFormTemplate().sTagTemplate.m_uiEndMotion;
			L_Frame[i] = 0;
		}
	}

	CurrentInfo.m_bNeedDelete = TRUE;
	tagInfo.m_bNeedDelete = FALSE;

	UpdatePlayerResourceOnGame();

	g_MyD3D->m_akPet[m_iPlayerIndex]->TagPet(CurrentInfo.m_iCharType, tagInfo.m_iCharType);

	LoadSlash();
	// AP캐릭터 콤보스킬 갱신
	if (IsLocalPlayer())
	{
		if (g_pkUIScene->m_pkGameOverlayUI)
			g_pkUIScene->m_pkGameOverlayUI->ResetComboSkill();
	}

	g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_SIEG_FURY);
	g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_JIN_BURNNING_MODE_EFFECT);

	if (g_kGlobalValue.IsCharUseSpecial4(GetCurrentChar().iCharType)) {
		UpdateChangedSkillSlot();
	}

	InitCurEquippedMarbleChargeMotion();
}

void PLAYER::UpdateTagCharInfo()
{
	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TAG_MATCH)
		return;

	int iChar = m_kUserInfo.GetCurrentChar().iCharType;
	std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.find(iChar);
	if (mit == m_mapTagCharInfo.end())
		return;

	TagCharacterInfo& tagInfo = mit->second;
	tagInfo.m_cLife = m_cLife;

	// m_vecTagCharNum에서 지운다
	if (m_cLife == 0)
		m_vecTagCharNum.erase(std::find(m_vecTagCharNum.begin(), m_vecTagCharNum.end(), iChar));
}

int PLAYER::GetNextTagChar()
{
	std::vector< int >::iterator vit;
	std::map< int, TagCharacterInfo >::iterator mit;
	for (vit = m_vecTagCharNum.begin(); vit != m_vecTagCharNum.end(); ++vit)
	{
		mit = m_mapTagCharInfo.find(*vit);
		if (mit == m_mapTagCharInfo.end())
			return GC_CHAR_INVALID;

		TagCharacterInfo& tagInfo = mit->second;
		int iLife = tagInfo.m_cLife;

		if (tagInfo.m_cLife > 0)
			return tagInfo.m_iCharType;
	}

	// 이까지 오면 없는거다
	return GC_CHAR_INVALID;
}

TagCharacterInfo* PLAYER::GetNextTagCharInfo()
{
	std::vector< int >::iterator vit;
	std::map< int, TagCharacterInfo >::iterator mit;
	for (vit = m_vecTagCharNum.begin(); vit != m_vecTagCharNum.end(); ++vit)
	{
		mit = m_mapTagCharInfo.find(*vit);
		if (mit == m_mapTagCharInfo.end())
			return NULL;

		TagCharacterInfo& tagInfo = mit->second;
		int iLife = tagInfo.m_cLife;

		if (tagInfo.m_cLife > 0)
			return &tagInfo;
	}

	// 이까지 오면 없는거다
	return NULL;
}

void PLAYER::SetTagSlotAlign()
{
	if (g_MyD3D->MyReplay->m_iCurrState == RP_VIEW)
		return;

	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TAG_MATCH)
		return;

	// 3명 들어가면 보장 못함
	if (m_kUserInfo.aiTagSlot[0] == -1 && -1 == m_kUserInfo.aiTagSlot[1])
		m_kUserInfo.aiTagSlot[0] = m_kUserInfo.aiTagSlot[1] = 0;
	else if (m_kUserInfo.aiTagSlot[0] == -1 || -1 == m_kUserInfo.aiTagSlot[1])
	{
		if (m_kUserInfo.aiTagSlot[0] == -1)
		{
			m_kUserInfo.aiTagSlot[0] = m_kUserInfo.aiTagSlot[1];
		}
		else if (m_kUserInfo.aiTagSlot[1] == -1)
		{
			m_kUserInfo.aiTagSlot[1] = m_kUserInfo.aiTagSlot[0];
		}
	}

	m_kUserInfo.aiTagSlot[2] = -1;

	if (IsLocalPlayer() == true && g_MyD3D->MyReplay->m_iCurrState != RP_PREPARE)
	{
		memcpy(g_kGlobalValue.m_kUserInfo.aiTagSlot, m_kUserInfo.aiTagSlot, sizeof(g_kGlobalValue.m_kUserInfo.aiTagSlot[0]) * 3);
		//g_pTagMatchSelect->UpdateSlot();
	}
}

void PLAYER::MakeTagMatchLifeMap(OUT std::map< int, int >& mapLife)
{
	char* pSlot;
	pSlot = m_kUserInfo.aiTagSlot;

	std::map<int, int>::iterator mit;
	for (int i = 0; i < TAG_CHAR_NUM; ++i)
	{
		if (pSlot[i] == -1)
			continue;

		mit = mapLife.find(pSlot[i]);
		if (mit == mapLife.end())
		{
			// 없으면 추가
			//if( g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH || g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS || g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH_REVERS )
			if (g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH || g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS)
				mapLife.insert(std::make_pair(pSlot[i], 2));
			else
				mapLife.insert(std::make_pair(pSlot[i], 1));
		}
		else
		{
			// 있으면 ++
			//if( g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH || g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS || g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH_REVERS )
			if (g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH || g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS)
				mit->second += 2;
			else
				mit->second++;
		}
	}
}

void PLAYER::TagMatchInit()
{
	m_dwPrevTagTime = GC_FPS_LIMIT * 3;
	// 괜히한거 같군..
	//2013-08 매치 개선 UI 들어가면서 tagmatch 작업 고려 안함 (뺀 컨텐츠)
	//if( g_pkUIScene->m_pkGameOverlayUI->GetHeadUp() )
	//    g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->SetNextTagTime( m_kUserInfo.GetCurrentChar().iCharType, m_dwPrevTagTime );

	// Ui에서 선택한 거 만큼 계산해줘야함.
	std::map< int, int > mapLife;
	MakeTagMatchLifeMap(mapLife);

	std::vector< int >::reverse_iterator vrit;
	for (vrit = m_vecTagCharNum.rbegin(); vrit != m_vecTagCharNum.rend(); ++vrit)
	{
		std::map< int, int >::iterator mit = mapLife.find((*vrit));
		if (mit == mapLife.end())
		{
			ASSERT(!"Tag Character Life를 구하지 못했음");
		}

		TagCharacterInfo tempTagChar;
		tempTagChar.m_cLife = mit->second;

		// 첫번째면( 역순이다 )
		if (vrit == m_vecTagCharNum.rbegin())
		{
			m_cLife = mit->second;
			tempTagChar.InitTagCharData(this);
			// 처음에는 로딩해 놓기 때문에 지울필요 없음
			tempTagChar.m_bNeedDelete = FALSE;
			g_MyD3D->m_akPet[m_iPlayerIndex]->InitTagPetData(m_iPlayerIndex, tempTagChar.m_iCharType);
			m_mapTagCharInfo.insert(std::make_pair((*vrit), tempTagChar));
		}
		else
		{
			tempTagChar.Init();
			// Init할때 Life 초기화 되버린다..
			tempTagChar.m_cLife = mit->second;
			tempTagChar.SetCharType((*vrit));
			g_MyD3D->m_akPet[m_iPlayerIndex]->AddTagPetInfo(m_iPlayerIndex, tempTagChar.m_iCharType);
			tempTagChar.Extra_Char_Num = tempTagChar.m_iCharType * GC_CHAR_NUM * m_kUserInfo.GetCurrentChar(tempTagChar.m_iCharType).iPromotionLevel;
			tempTagChar.m_ePlayerState = PS_ALIVE;
			Calcualte_Point(tempTagChar, true);
			LoadPlayerTemplate(&tempTagChar);
			EffectInit(tempTagChar);
			tempTagChar.ShowEffect(false);

			if (tempTagChar.m_iCharType == GC_CHAR_ARME)
				tempTagChar.m_fMana = 1.5f;
			else
				tempTagChar.m_fMana = 0.0f;

			UpdatePlayerResource(&tempTagChar);
			for (int k = 0; k < FORM_NUM; ++k)
				UpdateFormResource(k, &tempTagChar);
			//            CreateFormShadow( &tempTagChar );
			g_pGraphicsHelper->LoadPlayerActionMotionByPlayerIndex(m_iPlayerIndex, tempTagChar.Extra_Char_Num);
			m_mapTagCharInfo.insert(std::make_pair(tempTagChar.m_iCharType, tempTagChar));
		}
	}
}

std::map< char, GCEXPTYPE > PLAYER::GetCharacterExpMap()
{
	std::map< char, GCEXPTYPE > mapCharacterExp;

	KCharInfoVector::iterator vit;
	for (vit = m_kUserInfo.vecCharInfo.begin(); vit != m_kUserInfo.vecCharInfo.end(); ++vit)
	{
		SCharInfo& charInfo = (*vit);
		mapCharacterExp.insert(std::make_pair((char)charInfo.iCharType, charInfo.biExp));
	}

	return mapCharacterExp;
}

bool PLAYER::IsTagMotion()
{
	TagCharacterInfo* pTagInfo = GetNextTagCharInfo();

	if (pTagInfo == NULL)
		return false;

	if (uiMotion == GetCurFormTemplate().sTagTemplate.m_uiAtkMotion ||
		uiMotion == GetCurFormTemplate().sTagTemplate.m_uiEndMotion ||
		uiMotion == GetCurFormTemplate().sTagTemplate.m_uiStartMotion ||
		uiMotion == pTagInfo->GetCurFormTemplate().sTagTemplate.m_uiAtkMotion ||
		uiMotion == pTagInfo->GetCurFormTemplate().sTagTemplate.m_uiEndMotion ||
		uiMotion == pTagInfo->GetCurFormTemplate().sTagTemplate.m_uiStartMotion)
	{
		return true;
	}

	return false;
}

bool PLAYER::IsTagCharacterExist(int iCharNum_)
{
	std::map< int, TagCharacterInfo >::iterator mit;
	for (mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit)
	{
		TagCharacterInfo& tagInfo = mit->second;
		if (tagInfo.Extra_Char_Num == static_cast<char>(iCharNum_))
		{
			return true;
		}
	}

	return false;
}

std::string PLAYER::toTagStr()
{
	std::stringstream stm;

	std::map< int, TagCharacterInfo >::iterator mit;
	int i = 0;
	for (mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit, ++i)
	{
		stm << "[" << static_cast<int>(m_iPlayerIndex) << "TagCharInfo" << i << "] " << mit->second.toStr().c_str();
	}

	return stm.str();
}

void PLAYER::InitStarSignBoradBuff()
{
	std::map< int, std::set<int> > mapStars;
	KLuaManager luaMgr;

	if (GCFUNC::LoadLuaScript(luaMgr, "PairItem.stg") == false)
	{
		MessageBoxA(NULL, "PairItem Load Failed!", NULL, MB_OK);
		exit(0);
	}

	int PairItemFirst = 0;
	int PairItemSecond = 0;

	luaMgr.BeginTable("PAIRITEMLIST");
	{
		for (int i = 1; ; ++i)
		{
			if (FAILED(luaMgr.BeginTable(i)))
				break;
			luaMgr.GetValue(1, PairItemFirst);
			luaMgr.GetValue(2, PairItemSecond);
			mapStars[PairItemFirst].insert(PairItemSecond);
			mapStars[PairItemSecond].insert(PairItemFirst);

			luaMgr.EndTable();
		}
	}
	luaMgr.EndTable();

	std::set<int> setSerdinStar;
	std::set<int> setCarnavanStar;
	std::map<int, int> mapPlayerStar;
	for (int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		if (g_MyD3D->MyPlayer[i]->IsLive() == false)
			continue;

		std::set<int>& Stars = (g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() ? setSerdinStar : setCarnavanStar);

		for (int j = 0; j < 12; j++)
		{
			if (g_MyD3D->MyPlayer[i]->CheckEquipItem(KGCItemManager::ITEM_ARIES + j))
			{
				Stars.insert(KGCItemManager::ITEM_ARIES + j);
				mapPlayerStar[i] = KGCItemManager::ITEM_ARIES + j;
				break;
			}
		}
	}

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (g_MyD3D->MyPlayer[i]->IsLive() == false)
			continue;

		if (mapPlayerStar.find(i) == mapPlayerStar.end())
			continue;

		std::set<int>& Stars = (g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() ? setSerdinStar : setCarnavanStar);

		bool bExistPair = false;

		std::set<int>::iterator sit;
		for (sit = mapStars[mapPlayerStar[i]].begin(); sit != mapStars[mapPlayerStar[i]].end(); ++sit)
		{
			if (Stars.find((*sit)) == Stars.end())
				continue;

			bExistPair = true;
			break;
		}
		if (bExistPair == false)
			continue;

		if (g_pMagicEffect->IsMagicEffect(i, EGC_EFFECT_STARSIGNBOARD_EVENT))
			continue;

		g_pMagicEffect->SetMagicEffect(i, EGC_EFFECT_STARSIGNBOARD_EVENT, 999999.0f);
	}
}

void PLAYER::InitBonusBuff(bool bLoading)
{
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ||
		g_MyD3D->MyReplay->m_iCurrState == RP_VIEW ||
		g_MyD3D->IsLocalPlayerObserver())
		return;

	if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TAG_MATCH)
	{
		if (!g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_BALANCE) && g_MyD3D->MyPlayer[m_iPlayerIndex]->IsLive())
		{
			InitStarSignBoradBuff();
		}
		g_MyD3D->m_kItemMgr.GetGaChaSetItemAbility(this);

		// 방원이 가지고 있으면 모든 애들이 버프 적용받는 녀석
		bool bFoundJewel = false;
		bool bFoundFlowers = false;
		bool bFoundFriendShipRing = false;
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false)
				continue;

			if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), JEWEL_OF_QUEEN) == true
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), JEWEL_OF_QUEEN2) == true
				)
			{
				bFoundJewel = true;
			}

			if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), BRUNCH_OF_FLOWERS) == true)
			{
				bFoundFlowers = true;
			}

			if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING)
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ELESIS)
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LIR)
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ARME)
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LAS)
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RYAN)
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RONAN)
				|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_AMY)
				)
			{
				bFoundFriendShipRing = true;
			}

			if (bFoundFlowers && bFoundJewel && bFoundFriendShipRing)
				break;
		}

		if (IsLocalPlayer())
		{
			if (bFoundJewel == true)
			{
				//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_JEWEL_OF_QUEEN, -1 );
				g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_JEWEL_OF_QUEEN, 999999.0f);
			}

			if (bFoundFlowers == true)
			{
				//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_JEWEL_OF_QUEEN, -1 );
				g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_BRUNCH_OF_FLOWERS, 999999.0f);
			}

			if (bFoundFriendShipRing == true)
			{
				g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_FRIENDSHIP_RING, 999999.0f);
			}
		}
	}
	else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH)
	{
		if (!g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_BALANCE) && g_MyD3D->MyPlayer[m_iPlayerIndex]->IsLive())
		{
			InitStarSignBoradBuff();
		}
		bool bFoundJewel = false;
		bool bFoundFlowers = false;
		bool bFoundFriendShipRing = false;
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false)
				continue;

			std::map< int, TagCharacterInfo >& mapTagInfo = g_MyD3D->MyPlayer[i]->GetTagInfoMap();
			std::map< int, TagCharacterInfo >::iterator mit;
			for (mit = mapTagInfo.begin(); mit != mapTagInfo.end(); ++mit)
			{
				g_MyD3D->m_kItemMgr.GetGaChaSetItemAbility(this, &mit->second);

				if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), JEWEL_OF_QUEEN) == true
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), JEWEL_OF_QUEEN2) == true
					)
				{
					bFoundJewel = true;
				}

				if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), BRUNCH_OF_FLOWERS) == true)
				{
					bFoundFlowers = true;
				}

				if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING)
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING_ELESIS)
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING_LIR)
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING_ARME)
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING_LAS)
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING_RYAN)
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING_RONAN)
					|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(mit->first), FRIENDSHIP_RING_AMY)
					)
				{
					bFoundFriendShipRing = true;
				}

				if (bFoundFlowers && bFoundJewel && bFoundFriendShipRing)
					break;
			}

			if (IsLocalPlayer())
			{
				if (bFoundJewel == true)
				{
					//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_JEWEL_OF_QUEEN, -1 );
					g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_JEWEL_OF_QUEEN, 999999.0f);
				}

				if (bFoundFlowers == true)
				{
					//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_JEWEL_OF_QUEEN, -1 );
					g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_BRUNCH_OF_FLOWERS, 999999.0f);
				}

				if (bFoundFriendShipRing == true)
				{
					g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_FRIENDSHIP_RING, 999999.0f);
				}
			}
		}
	}

	//카네이션 아이템 체크
	if (g_MyD3D->m_kItemMgr.CheckEquipItemForItemID(&g_MyD3D->MyPlayer[m_iPlayerIndex]->m_kUserInfo.GetCurrentChar(), ITEM_ID_YELLOWCARNATION))
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_CARNATION, 999999.0f);
	}

	if (g_kGlobalValue.m_kChattingEvent.GetSuccessEvent())
	{
		if (!g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_CHATTING_BUFF))
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_CHATTING_BUFF, 999999.0f);
	}

#if defined( COUPLE_SYSTEM )
	if (IsLocalPlayer())
	{
		if (SiKGCCoupleSystem()->CheckCoupleWithMe(m_kUserInfo.dwUID, true) &&
			SiKGCCoupleSystem()->IsCoupleDay())
		{
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_COUPLE_DAY, 999999.0f);
		}
		else if (SiKGCCoupleSystem()->CheckCoupleWithMe(m_kUserInfo.dwUID, true))
		{
			GCItem* pItem = g_pItemMgr->GetItemData(SiKGCCoupleSystem()->GetRingItemID(true));

			if (EMT_GAMEPOINT == pItem->eMoneyType)
			{
#if !defined( NATION_HONGKONG) && !defined( NATION_TAIWAN )
				g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_COUPLE_BUFF_GP, 999999.0f);
#endif
			}
			else
			{
				g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_COUPLE_BUFF, 999999.0f);
			}
		}
	}
#endif
	//이벤트 보너스 포인트가 있는한 제한없이 보너스 포인트를 사용할수 있다. 그리고 던전이어야만 한다.
	if ((SiKGCEventManager()->IsActiveEvent(EVENT_ID_MUHAN_BONUS_POINT) ||
		(SiKGCEventManager()->IsActiveEvent(EVENT_ID_PLAYPARK_BONUS) && g_kGlobalValue.m_dwChannelType == 2) //멋쟁이 PP보너스 조건 쩔음..
		&& SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON))
	{
		g_kGlobalValue.m_bMUHANBonusPoint = true;
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_MUHAN_BONUSPOINT, 999999.0f);
	}
	else
	{
		g_kGlobalValue.m_bMUHANBonusPoint = false;
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_MUHAN_LIFE))
	{
		g_kGlobalValue.m_bMUHANLife = true;
	}
	else
	{
		g_kGlobalValue.m_bMUHANLife = false;
	}

#if !defined( NATION_KOREA )
	// 계급체널 보너스
	if (IsLocalPlayer() && SiKGCChannelManager()->IsChannelBonus())
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_GRADE_BONUS, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_GRADE_BONUS, 999999.0f);
	}
#endif

	//달이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_MOON_FESTIVAL))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_MOON_FESTIVAL, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_MOON_FESTIVAL, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_VALENTINE_COUPLE_BUF))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_MOON_FESTIVAL, -1 );
		if (SiKGCCoupleSystem()->GetRingID(m_kUserInfo.dwUID) == COUPLERING_VALENTINE)
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_VALENTINE_COUPLE_BUFF, 999999.0f);
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_PRE_MARI_1))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_MOON_FESTIVAL, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_MARI_1, 999999.0f);
	}

	//해 이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_SUN_FESTIVAL))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_SUN_FESTIVAL, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_SUN_FESTIVAL, 999999.0f);
	}

	//로이 끄라통 축제
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_LOI_FESTIVAL))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_SUN_FESTIVAL, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_LOI_FESTIVAL, 999999.0f);
	}

	//화이트데이 이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_WHITEDAY))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_WHITEDAY, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_WHITEDAY, 999999.0f);
	}

	//에이미 이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_AMY))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_AMY_EVENT, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_AMY_EVENT, 999999.0f);
	}

	//EVENT_ID_EVENT_FREE_2 는 버프없이 엠블램만 걸때 쓴다, 그래서 주석임
	//잉여 이벤트 발동 조건은 일단 던전, 대전을 가리지 않아야 한다.
	//2번 주석친거 풀지마세요. 다른 번호 쓰셈...
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EVENT_FREE_1))  //99
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_EVENT_BUF1, 999999.0f);
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EVENT_FREE_2))  //100
	{
		if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_EVENT_BUF2, 999999.0f);
		}
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EVENT_FREE_3))  //101
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_EVENT_BUF3, 999999.0f);
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EVENT_FREE_4))  //102
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_EVENT_BUF4, 999999.0f);
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EVENT_FREE_5))  //103
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_EVENT_BUF5, 999999.0f);
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_WEEKEND_BONUS)) //104
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_WEEKEND_BONUS, 999999.0f);
	}

	// 던전데이 이벤트
	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
	{
		if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_DUNGEON_DAY))
		{
			//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_DUNGEON_DAY, -1 );
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_DUNGEON_DAY, 999999.0f);
		}

		if ((m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCCLUB) > 0) {
			//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_GC_CLUB_BONUS, -1 );
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_GC_CLUB_BONUS, 999999.0f);
		}
		else if ((m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCBLOG) > 0) {
			//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_GC_CLUB_BONUS, -1 );
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_GC_BLOG_BONUS, 999999.0f);
		}

		if ((m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GCPREMIUM) > 0 && g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_PREMIUM_GC_CLUB_BONUS) == false)
		{
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_PREMIUM_GC_CLUB_BONUS, 999999.0f);
		}

		if (g_MyD3D->m_pkQuestGameMgr->GetIsCorrupted())
		{
			if (SiKGCRoomManager()->GetGameMode() >= GC_GM_QUEST99 && SiKGCRoomManager()->GetGameMode() <= GC_GM_QUEST102 && SiKGCRoomManager()->GetGameMode() != GC_GM_QUEST100)
				g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_ELYOS_DAMAGE, 999999.0f);
		}

		// PlayPark 유저일때만 이벤트 발동하쇼
		if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_PLAYPARK_BONUS) && g_kGlobalValue.m_dwChannelType == 2)
		{
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_PLAYPARK_BONUS_BUFF, 999999.0f);
		}

#if defined(NATION_THAILAND) || defined( NATION_BRAZIL )
		//  태국, 필리핀, 브라질은 피씨방 보너스 없음.
#else
		//IP Bonus는 던젼에서만 보여 줍니다.
		if (m_kUserInfo.cIsNetmarblePCRoom == 1)
		{
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_IP_BONUS, 999999.0f);
		}
#endif

		if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_SEIREN)) // 세이렌 이벤트이면
		{
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_SEIREN, 999999.0f);
		}

		if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_PRE_PRIME_KINGHT2))
		{
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_PRE_PRIME_KINGHT2, 999999.0f);
		}

		// 보유하고 있는 활력이 던전에 필요활력보다 많을시 버프
		int iGameMode = SiKGCRoomManager()->GetGameMode();
		int iDifficulty = SiKGCRoomManager()->GetDungeonLevel();
		if (!g_kGlobalValue.m_mapVitalityDungeonInfo.empty() &&
			SiKGCFatigueManager()->GetCurrentGameFatigueType() != KGCFatigueManager::RED_TIME)
		{
			std::map< PAIR_INT, KVitalityCheckDungeon >::iterator mit = g_kGlobalValue.m_mapVitalityDungeonInfo.find(PAIR_INT(iGameMode, iDifficulty));
			if (mit != g_kGlobalValue.m_mapVitalityDungeonInfo.end())
			{
				if (mit->second.m_nDeductVitality <= g_kGlobalValue.m_iPrevRemainVitality)
				{
					g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_VITALITY_BUFF, 999999.0f);
				}
			}
		}
	}

	// 인벤에 보유하고 있는 아이템 버프
	SetInvenBuffEffect();

	// 데스매치 이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_DEATHMATCH_BONUS) &&
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH)
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_DEATHMATCH_BONUS, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_DEATHMATCH_BONUS, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_FRIENDS_DAY))
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_FRIENDS_DAY, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_WELCOME_SEASON2))
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_WELCOME_SEASON2, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_AMY_TOGETHER))
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_AMY_TOGETHER, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EDEL_TOGETHER))
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && g_MyD3D->GetPlayerNum() > 1 && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && GC_CHAR_EDEL == g_MyD3D->MyPlayer[i]->GetCurrentChar().iCharType && g_MyD3D->IsMyTeam(i))
			{
				if (!g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_EDEL_TOGETHER))
					g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_EDEL_TOGETHER, 999999.0f);

				break;
			}
		}
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_TOGETHER))
	{
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
			{
				int nPlayerNum = g_MyD3D->GetPlayerNum();

				switch (nPlayerNum)
				{
				case 2:
				{
					if (!g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_TOGETHER_2))
						g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_TOGETHER_2, 999999.0f);
				}
				break;
				case 3:
				{
					if (!g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_TOGETHER_3))
						g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_TOGETHER_3, 999999.0f);
				}
				break;
				case 4:
				case 5:
				case 6:
				{
					if (!g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_TOGETHER_4))
						g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_TOGETHER_4, 999999.0f);
				}
				break;
				default:
					break;
				};

				break;
			}
		}
	}
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_RONAN_TOGETHER))
	{
#if !defined( NATION_USA )
		for (int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && g_MyD3D->GetPlayerNum() > 1 && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && GC_CHAR_RONAN == g_MyD3D->MyPlayer[i]->GetCurrentChar().iCharType && g_MyD3D->IsMyTeam(i))
			{
#endif
				if (!g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_TOGETHER))
					g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_TOGETHER, 999999.0f);
#if !defined( NATION_USA )
				break;
			}
		}
#endif
	}

	//달이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_MOON_FESTIVAL))
	{
		//g_MyD3D->MyBuff->SetBuff( EGC_EFFECT_MOON_FESTIVAL, -1 );
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_MOON_FESTIVAL, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_HONGKONG_EMBLEM))
	{
		if (m_kUserInfo.bLive && !g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, EGC_EFFECT_HONGKONG_CHARACTER_EMBLEM))
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_HONGKONG_CHARACTER_EMBLEM, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_SERVER))
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_SERVER_EXP_UP, 999999.0f);
	}

	// GWC 이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_GWC2009) && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_GWC, 999999.0f);
	}

	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_NEWYEAR_DAY) && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_NEW_YEAR, 999999.0f);
	}

	//쌍십절 이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_TEN_TEN_DAY))
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_TEN_TEN_DAY, 999999.0f);
	}

	if (g_kGlobalValue.m_bIsRainbowEvent && g_kGlobalValue.m_bJackPot)
	{
#if defined(FULLMOON_EVENT)
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_FULLMOON_EVENT_JACKPOT, 999999.0f);
#else
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_RAINBOW_EVENT_JACKPOT, 999999.0f);
#endif
	}
#ifdef NATION_KOREA
	//한국은 레인보우 기간이라도 버프가 터지지 않으면 아무 이펙트도 설정하지 않는다.
#else
	if (g_kGlobalValue.m_bIsRainbowEvent && !g_kGlobalValue.m_bJackPot)
	{
#if defined(FULLMOON_EVENT)
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_FULLMOON_EVENT, 999999.0f);
#else
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_RAINBOW_EVENT, 999999.0f);
#endif
	}
#endif

	// 이벤트 걸리면 적용되게 해주자
	//KLuaManager luaMgr;
	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
	//int red;
	//int green;
	//int blue;
	//int alpha;
	//LUA_GET_VALUE_DEF( "r", red, 0 );
	//LUA_GET_VALUE_DEF( "g", green, 0 );
	//LUA_GET_VALUE_DEF( "b", blue, 0 );
	//LUA_GET_VALUE_DEF( "a", alpha, 0 );
	//g_pkGameOverlayUI->SetEclipseTextureColor( D3DCOLOR_ARGB( alpha, red, green, blue ) );

	bool bHaveFriendRing1 = false;
	bool bHaveFriendRing2 = false;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (g_MyD3D->MyPlayer[i]->IsLive())
		{
			if (!bHaveFriendRing1 && g_MyD3D->MyPlayer[i]->CheckEquipItem(KGCItemManager::ITEM_RING_OF_BEST))
				bHaveFriendRing1 = true;

			if (!bHaveFriendRing2 && g_MyD3D->MyPlayer[i]->CheckEquipItem(KGCItemManager::ITEM_RING_OF_FRIEND))
				bHaveFriendRing2 = true;
		}

		if (bHaveFriendRing1 && bHaveFriendRing2)
		{
			for (int j = 0; j < MAX_PLAYER_NUM; ++j)
			{
				if (g_MyD3D->MyPlayer[j]->IsLive())
					g_pMagicEffect->SetMagicEffect(j, EGC_EFFECT_FRIEND_RING, 999999.0f);
			}
			break;
		}
	}

	//쌍십절 이벤트
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_EXP_BONUS)
#if defined(NATION_IDN)
		|| SiKGCEventManager()->IsActiveEvent(EVENT_ID_GP_EXP)
#endif
		)
	{
#if defined (NATION_TAIWAN)
		if (g_pItemMgr->m_kInventory.FindItemByItemID(10398))
#endif
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_EXP_UP, 999999.0f);
	}

	if (CheckSkill(SID_DIO2_CRITICAL_UP_LV3) || CheckSkill(SID_LEY01_CRITICAL_LV3))
	{
		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] += 0.015f;
	}
	else if (CheckSkill(SID_DIO2_CRITICAL_UP_LV4) || CheckSkill(SID_LEY01_CRITICAL_LV4))
	{
		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] += 0.02f;
	}
	else if (CheckSkill(SID_DIO2_CRITICAL_UP_LV5) || CheckSkill(SID_LEY01_CRITICAL_LV5))
	{
		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] += 0.025f;
	}
	else if (CheckSkill(SID_DIO1_CRITICAL_UP_LV1) || CheckSkill(SID_LEY01_CRITICAL_LV1))
	{
		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] += 0.005f;
	}
	else if (CheckSkill(SID_DIO1_CRITICAL_UP_LV2) || CheckSkill(SID_LEY01_CRITICAL_LV2))
	{
		m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] += 0.01f;
	}

	//유저 혜택 이벤트 (태국은 3BB 이벤트)
	if (g_kGlobalValue.m_kUserInfo.clsUserBenefitType == 3)
	{
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_USER_BENEFIT_BONUS, 999999.0f);
	}

	// 점령전 칭호 버프( 무기체인지시 초기화 되는 문제 처리 )
	// 점령전 칭호 버프가 걸려있는지 확인 후 처리
	if (SiKGCChannelManager()->IsPVPBallanceServer())
	{
		// 평준화 + 대전에는 칭호 버프 x
	}
	else
	{
		if (false == bLoading &&
			false == IsMagicEffect(EGC_EFFECT_ATHENA_SILVER) &&
			false == IsMagicEffect(EGC_EFFECT_ATHENA_GOLD) &&
			false == IsMagicEffect(EGC_EFFECT_ATHENA_MASTER) &&
			false == IsMagicEffect(EGC_EFFECT_ATHENA_GRANDMASTER))
		{
			g_MyD3D->MyHead->SetTitleEmblem();
		}
	}

	// 개기일식 밝기량 조절
	if (SiGCEclipseEventManager()->IsEventTime()) {
		if (SiGCEclipseEventManager()->IsActiveEvent() && SiGCEclipseEventManager()->IsValidDungeon(SiKGCRoomManager()->GetGameMode())) {
			g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_ECLIPSE_IMAGE_BUFF, 999999.0f);
		}
	}

#if defined( USE_AGIT_SYSTEM )
	if (!((SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AUTOMATCH) &&
		g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_NO_AGIT_BUFF))) {
		if (g_MyD3D->Get_MyPlayer() == m_iPlayerIndex) {
			SiKGCAgitGameManager()->SetFairyTreeBuff();    // 요정의 나무 열매 버프
		}
	}
#endif

	// 챔피언 무한 이벤트 일 ?
	if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_HELLMODE_FREE)) {
		g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, EGC_EFFECT_HELLMODE_FREE, 999999.0f);
	}
}

void PLAYER::LoadPet()
{
	if (g_MyD3D->m_akPet[m_iPlayerIndex]->IsPetWaitForTag() == true)
		g_MyD3D->m_akPet[m_iPlayerIndex]->EndPet();

	//if( !m_kUserInfo.bLive )
	//    return;

	if (m_kUserInfo.cCharIndex == -1)
		return;

	SCharInfo* pCharInfo = &GetCurrentChar();

	if (pCharInfo == NULL)
		g_kGlobalValue.m_strNullFunc = __FUNCTION__;

	// 090625 serastia:
	// 원래 -1 로 줬는데 왜 그랬는지는 모르겠다...일단 바꿨음..
	// 안바꾸면 펫쪽에서 크래시 나거나 강제로 0으로 바꾸니 주인이 달라져서..
	g_MyD3D->m_akPet[m_iPlayerIndex]->m_iPlayerNumber = m_iPlayerIndex;

	// 펫 : StartPet
	if (m_iPlayerIndex < MAX_PLAYER_NUM)
	{
		int iPetID = g_kGlobalValue.GetPetIDFromItemID(pCharInfo->kPetInfo.m_dwID);
		int iPetEvolutionLevel = (int)pCharInfo->kPetInfo.m_cPromotion;
		int iPetLevel = pCharInfo->kPetInfo.m_iLevel;
		// 자기 자신일 때는 자신의 정보를 가지고 펫을 생성한다
		bool bValidPet = false;

		if (iPetID != -1)
		{
			if ((IsLocalPlayer() && g_MyD3D->MyReplay->m_iCurrState != RP_VIEW))
			{
				if (pCharInfo->kPetInfo.m_dwUID != 0 && g_MyD3D->IsExistPetInfo(pCharInfo->kPetInfo.m_dwUID))
				{
					bValidPet = true;
				}
			}
			else if (m_kUserInfo.dwUID != 0 && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT) {
				bValidPet = true;
			}
			else
			{
				if (pCharInfo->kPetInfo.m_dwUID != 0)
				{
					bValidPet = true;
				}
			}
		}

		if (bValidPet)
		{
			g_MyD3D->m_akPet[m_iPlayerIndex]->StartPet((int)m_iPlayerIndex, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::GAME, pCharInfo->kPetInfo.m_strName, true, pCharInfo->kPetInfo.m_kCostume.m_dwID);
			g_MyD3D->m_akPet[m_iPlayerIndex]->CreateParticleEffect();
			g_MyD3D->m_akPet[m_iPlayerIndex]->InitBeforeGameStart();
		}
	}
}

void PLAYER::GetSoundLoadListfromExtraCharNum(IN char cCharNum, OUT std::set<int>& setLoadList)
{
	PLAYER_TEMPLATE tempTemplate;
	tempTemplate.Init();
	SetPlayerTemplate(m_setSkill, cCharNum, tempTemplate);

	KSafeArray<PLAYER_TEMPLATE, FORM_NUM> tempFormTemplate;
	for (int i = 0; i < FORM_NUM; ++i)
		tempFormTemplate[i].Init();
	SetFormTemplate(cCharNum, tempFormTemplate);

	PLAYER_TEMPLATE tempSwapTemplate;
	tempSwapTemplate.Init();
	SetSwapWeaponTemplate(cCharNum, tempSwapTemplate);

	if (tempTemplate.setCharSound.empty())
		return;

	//std::for_each(tempTemplate.setCharSound.begin(), tempTemplate.setCharSound.end(),
		//std::bind(&std::set<int>::insert, setLoadList, std::placeholders::_1));
		//boost::bind( &std::set<int>::insert, boost::ref(setLoadList), _1 ) );
	for (auto i = tempTemplate.setCharSound.begin(); i != tempTemplate.setCharSound.end(); ++i)
	{
		setLoadList.insert(*i);
	}

	for (int i = 0; i < FORM_NUM; ++i)
	{
		if (tempFormTemplate[i].setCharSound.empty())
			continue;

		for (auto j = tempFormTemplate[i].setCharSound.begin(); j != tempFormTemplate[i].setCharSound.end(); ++j)
		{
			setLoadList.insert(*j);
		}
	}

	if (tempSwapTemplate.setCharSound.empty())
		return;

	for (auto j = tempSwapTemplate.setCharSound.begin(); j != tempSwapTemplate.setCharSound.end(); ++j)
	{
		setLoadList.insert(*j);
	}
}

void PLAYER::MakeSoundLoadList()
{
	if (!m_kUserInfo.bLive)
		return;

	std::set<int> setLoadList;
	if (g_MyD3D->MyReplay->m_iCurrState == RP_PREPARE)
	{
		// [8/5/2007 breadceo] 이미 Replay의 Head를 읽은 상태이므로 무슨 케릭터가 나올지 안다.
		for (int i = 0; i < 3; ++i)
		{
			if (m_kUserInfo.aiTagSlot[i] != -1)
				GetSoundLoadListfromExtraCharNum(GetExtraCharNum(m_kUserInfo.aiTagSlot[i]), setLoadList);
		}

		if (m_kUserInfo.bLive == true)
		{
			GetSoundLoadListfromExtraCharNum(Extra_Char_Num, setLoadList);
		}

		for (std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit)
		{
			GetSoundLoadListfromExtraCharNum(GetExtraCharNum(mit->second.m_iCharType), setLoadList);
		}
	}
	// [8/5/2007 breadceo] 리플레이를 제외한 기타 모드들
	else
	{
		GetSoundLoadListfromExtraCharNum(Extra_Char_Num, setLoadList);

		for (std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit)
		{
			GetSoundLoadListfromExtraCharNum(GetExtraCharNum(mit->second.m_iCharType), setLoadList);
		}

		if (SiKGCRoomManager()->GetGameMode() == GC_GM_INDIGO_TEAM)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (m_kUserInfo.aiTagSlot[i] != -1)
					GetSoundLoadListfromExtraCharNum(GetExtraCharNum(m_kUserInfo.aiTagSlot[i]), setLoadList);
			}
		}
	}

	std::for_each(setLoadList.begin(), setLoadList.end(),
		boost::bind(&KDSound::PreparetoLoad, boost::ref(g_KDSound), _1));
}

void PLAYER::SetWhiteCount(int iCount)
{
	g_MyD3D->MyHead->White_Count = iCount;
}

DWORD PLAYER::GetElemental()
{
	if (m_StrongLevel == 9)
		return m_Elemental;
	else
		return 0;
};

void PLAYER::LoadPlayerTemplate(char Extra_Char_Num_ /*= -1*/)
{
	for (int i = 0; i < FORM_NUM; ++i)
		m_FormTemplate[i].Init();

	for (int i = 0; i < BTYPE_NUM; ++i)
		m_BurnningTemplate[i].Init();

	m_SwapTemplate.Init();

	PrepairSkill(Extra_Char_Num);

	SetSelectedPromotion(Extra_Char_Num);
	SetSelectedPromotion(Extra_Char_Num);
	SetPlayerTemplate(m_setSkill, Extra_Char_Num, GetCurFormTemplate());
	SetFormTemplate(Extra_Char_Num, m_FormTemplate);
	SetSwapWeaponTemplate(Extra_Char_Num, m_SwapTemplate);
	SetBurnningTemplate(Extra_Char_Num, m_BurnningTemplate);
	SetSkillType(0);

	if (m_fAbility[ABILITY_MOVEMENT_SPEED_UP] > 0.f)
	{
		float fSpeedUpRate = 1.f + (m_fAbility[ABILITY_MOVEMENT_SPEED_UP] / 100.f);

		GetCurFormTemplate().fRunSpeed *= fSpeedUpRate;
		GetCurFormTemplate().fWalkSpeed *= fSpeedUpRate;
		m_SwapTemplate.fRunSpeed *= fSpeedUpRate;
		m_SwapTemplate.fWalkSpeed *= fSpeedUpRate;

		for (int i = 0; i < BTYPE_NUM; ++i)
		{
			m_BurnningTemplate[i].fRunSpeed *= fSpeedUpRate;
			m_BurnningTemplate[i].fWalkSpeed *= fSpeedUpRate;
		}
	}

	m_bIsBurnningMode = false;

	m_mapStatsSkillMotionList.clear();
	for (std::set< int >::iterator sitor = GetCurFormTemplate().setStatsSkillMotionList.begin(); sitor != GetCurFormTemplate().setStatsSkillMotionList.end(); ++sitor) {
		m_mapStatsSkillMotionList.insert(std::make_pair(*sitor, 0));
	}

	// 밑에 설정은 로컬 자기자신만 필요한 셋팅들이다 불필요한 플레이어들 셋팅할 필요 없다.
	if (IsLocalPlayer() == false) {
		return;
	}

	SetSavingStateFunc();

	if (Extra_Char_Num_ == -1 || Extra_Char_Num_ != Extra_Char_Num)
	{
		int iChar = 0;
		int iJob = 0;

		if (Extra_Char_Num_ == -1) {
			iChar = Extra_Char_Num % GC_CHAR_NUM;
			iJob = Extra_Char_Num / GC_CHAR_NUM;
		}
		else {
			iChar = Extra_Char_Num_ % GC_CHAR_NUM;
			iJob = Extra_Char_Num_ / GC_CHAR_NUM;
		}

#if defined( USE_AP_SLOT_CHANGE )
		for (std::map< int, vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkillBySlotType.begin(); mit != m_mapHotKeySkillBySlotType.end(); ++mit) {
			for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
				if (vit->pTexture != NULL) {
					vit->pTexture->Release();
				}
			}
		}
		m_mapHotKeySkillBySlotType.clear();
		m_vecHotKeySkill.clear();

		std::vector< GCHotKeySkill > vecHotKeySkill;

		m_vecPossibleChargingSkillInfo.clear();

		for (int i = 0; i < KSkillSlot::INDEX_MAX; i++) {
			vecHotKeySkill.clear();

			std::vector< EGCSkillTree > vecSkillID = SiGCSKT()->GetHotKeySkillVectorByType(iChar, iJob, i);
			for (std::vector< EGCSkillTree >::iterator vit = vecSkillID.begin(); vit != vecSkillID.end(); vit++)
			{
				if (SiGCSKT()->GetSkill(*vit)->m_pHotKeySkill)
				{
					GCHotKeySkill skill;
					skill.m_eSkill = *vit;
					GCHotKeySkillInfo* info = SiGCSKT()->GetSkill(*vit)->m_pHotKeySkill;
					std::vector< GCComboSkill >::iterator vecIter = info->m_vecComboSkill.begin();
					for (; vecIter != info->m_vecComboSkill.end(); ++vecIter)
					{
						vecIter->m_pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(EGCSkillTree(vecIter->m_iSID))->GetTextureFileName());
					}
					skill.pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(*vit)->GetTextureFileName());
					//m_vecHotKeySkill.push_back( skill );
					vecHotKeySkill.push_back(skill);
					if (info->m_iChargingSkillLV > 0) {
						m_vecPossibleChargingSkillInfo.push_back(*info);
					}
				}
			}

			m_mapHotKeySkillBySlotType.insert(std::make_pair(i, vecHotKeySkill));
		}

		std::map< int, vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkillBySlotType.find(KSkillSlot::INDEX_A);
		if (mit != m_mapHotKeySkillBySlotType.end()) {
			m_vecHotKeySkill = mit->second;
		}

		// 차징가능한 스킬 중 레벨이 높은 순으로 정렬 해주자. 이유는 마나 차징 수치에 따라서 해당 레벨에 맞는 필살기를 써주게 하기 위함이다.
		if (!m_vecPossibleChargingSkillInfo.empty()) {
			std::sort(m_vecPossibleChargingSkillInfo.begin(), m_vecPossibleChargingSkillInfo.end(), KChargingSkillLevelHigh());
		}
#else

		// 진짜 각성 스킬트리 개편 들어가면 들어내야 하는  부분임.
		if (g_kGlobalValue.IsCharUseSpecial4(iChar)) {
			ReSettingSpecial4Skill(iChar, iJob);
		}
		else {
			bool bCheckRelease = false;
			for (std::map< int, std::vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkill.begin(); mit != m_mapHotKeySkill.end(); ++mit) {
				for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); vit++) {
					vit->pTexture->Release();
					bCheckRelease = true;
				}
			}
			m_mapHotKeySkill.clear();
			m_mapPossibleChargingSkillInfo.clear();

			if (bCheckRelease = false) {
				for (std::vector< GCHotKeySkill >::iterator vit = m_vecHotKeySkill.begin(); vit != m_vecHotKeySkill.end(); vit++)
				{
					vit->pTexture->Release();
				}
			}
			m_vecHotKeySkill.clear();

			m_vecPossibleChargingSkillInfo.clear();

			bool bDefaultSkillSetting = false;
			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL) {
				if (m_setSkill.empty() && (g_kGlobalValue.IsHybridPlayer(iChar) || g_kGlobalValue.IsQuickSkillPlayer(iChar))) {
					std::map< std::pair< int, int >, std::vector< EGCSkillTree > >::iterator mit = m_mapTutorialDefaultEquipSkillA.find(std::make_pair(iChar, iJob));
					if (mit != m_mapTutorialDefaultEquipSkillA.end()) {
						m_setSkill = mit->second;
						bDefaultSkillSetting = true;
					}
				}
			}

			for (std::vector< EGCSkillTree >::iterator sit = m_setSkill.begin(); sit != m_setSkill.end(); sit++)
			{
				const GCSkill* pSkill = SiGCSKT()->GetSkill(*sit);
				if (pSkill && pSkill->m_pHotKeySkill && pSkill->m_pHotKeySkill->m_bNoEquipSlot == false)
				{
					GCHotKeySkill skill;
					skill.m_eSkill = *sit;
					GCHotKeySkillInfo* info = SiGCSKT()->GetSkill(*sit)->m_pHotKeySkill;
					std::vector< GCComboSkill >::iterator vecIter = info->m_vecComboSkill.begin();
					for (; vecIter != info->m_vecComboSkill.end(); ++vecIter)
					{
						vecIter->m_pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(EGCSkillTree(vecIter->m_iSID))->GetTextureFileName());
					}
					skill.pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(*sit)->GetTextureFileName());
					skill.pAddMotionTexture = g_pGCDeviceManager2->CreateTexture(pSkill->GetAddMotionTextureFileName());
					m_vecHotKeySkill.push_back(skill);
					m_mapHotKeySkill[SKILL_TYPE_NORMAL_A].push_back(skill);
					if (info->m_iChargingSkillLV > 0 && info->m_iChargingSkillLV < 4) {
						m_vecPossibleChargingSkillInfo.push_back(*info);
						m_mapPossibleChargingSkillInfo[SKILL_TYPE_NORMAL_A].push_back(*info);
					}
				}
			}

			// 차징가능한 스킬 중 레벨이 높은 순으로 정렬 해주자. 이유는 마나 차징 수치에 따라서 해당 레벨에 맞는 필살기를 써주게 하기 위함이다.
			if (!m_vecPossibleChargingSkillInfo.empty()) {
				std::sort(m_vecPossibleChargingSkillInfo.begin(), m_vecPossibleChargingSkillInfo.end(), KChargingSkillLevelHigh());
			}

			for (std::map< int, std::vector< GCHotKeySkillInfo > >::iterator mit = m_mapPossibleChargingSkillInfo.begin(); mit != m_mapPossibleChargingSkillInfo.end(); ++mit) {
				std::sort(mit->second.begin(), mit->second.end(), KChargingSkillLevelHigh());
			}
		}
#endif
	}
}

//void PLAYER::LoadPlayerTemplate_NewSkill( char Extra_Char_Num_ /*= -1*/ )
//{
//    for( int i = 0; i < FORM_NUM; ++i )
//        m_FormTemplate[i].Init();
//
//	for( int i = 0; i < BTYPE_NUM; ++i )
//		m_BurnningTemplate[i].Init();
//
//    m_SwapTemplate.Init();
//
//    PrepairSkill( Extra_Char_Num );
//
//    SetPlayerTemplate( m_setSkill, Extra_Char_Num, GetCurFormTemplate() );
//    SetFormTemplate( Extra_Char_Num, m_FormTemplate );
//    SetSwapWeaponTemplate( Extra_Char_Num, m_SwapTemplate );
//	SetBurnningTemplate( Extra_Char_Num, m_BurnningTemplate );
//    SetSkillType( 0 );
//
//    if( m_fAbility[ABILITY_MOVEMENT_SPEED_UP] > 0.f )
//    {
//        float fSpeedUpRate = 1.f + ( m_fAbility[ABILITY_MOVEMENT_SPEED_UP] / 100.f );
//
//        GetCurFormTemplate().fRunSpeed *= fSpeedUpRate;
//        GetCurFormTemplate().fWalkSpeed *= fSpeedUpRate;
//        m_SwapTemplate.fRunSpeed *= fSpeedUpRate;
//        m_SwapTemplate.fWalkSpeed *= fSpeedUpRate;
//
//        for( int i = 0; i < BTYPE_NUM; ++i )
//        {
//            m_BurnningTemplate[i].fRunSpeed *= fSpeedUpRate;
//            m_BurnningTemplate[i].fWalkSpeed *= fSpeedUpRate;
//        }
//    }
//
//    m_bIsBurnningMode = false;
//
//    if( IsLocalPlayer() == false ) {
//        return;
//    }
//
//    if ( Extra_Char_Num_ == -1 || Extra_Char_Num_ != Extra_Char_Num )
//    {
//#if defined( USE_AP_SLOT_CHANGE )
//        for( std::map< int, vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkillBySlotType.begin(); mit != m_mapHotKeySkillBySlotType.end(); ++mit ) {
//            for( std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
//                if( vit->pTexture != NULL ) {
//                    vit->pTexture->Release();
//                }
//            }
//        }
//        m_mapHotKeySkillBySlotType.clear();
//        m_vecHotKeySkill.clear();
//#else
//        for ( std::vector< GCHotKeySkill >::iterator vit = m_vecHotKeySkill.begin(); vit != m_vecHotKeySkill.end(); vit++ )
//        {
//            vit->pTexture->Release();
//        }
//
//        m_vecHotKeySkill.clear();
//#endif
//
//        m_vecPossibleChargingSkillInfo.clear();
//
//        int iChar = 0;
//        int iJob = 0;
//
//        if( Extra_Char_Num_ == -1 ) {
//            iChar = Extra_Char_Num % GC_CHAR_NUM;
//            iJob = Extra_Char_Num / GC_CHAR_NUM;
//        } else {
//            iChar = Extra_Char_Num_ % GC_CHAR_NUM;
//            iJob = Extra_Char_Num_ / GC_CHAR_NUM;
//        }
//
//        bool bDefaultSkillSetting = false;
//        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ) {
//            if( m_setSkill.empty() &&
//                ( g_kGlobalValue.IsHybridPlayer( iChar ) || g_kGlobalValue.IsQuickSkillPlayer( iChar ) || g_kGlobalValue.IsNewSkillTreePlayer( iChar ) ) ) {
//                std::map< std::pair< int, int >, std::vector< EGCSkillTree > >::iterator mit = m_mapTutorialDefaultEquipSkill.find( std::make_pair( iChar, iJob ) );
//                if( mit != m_mapTutorialDefaultEquipSkill.end() ) {
//                    m_setSkill = mit->second;
//                    bDefaultSkillSetting = true;
//                }
//            }
//        }
//
//#if defined( USE_AP_SLOT_CHANGE )
//
//        std::vector< GCHotKeySkill > vecHotKeySkill;
//
//        m_vecPossibleChargingSkillInfo.clear();
//
//        for( int i = 0; i < KSkillSlot::INDEX_MAX; i ++ ) {
//            vecHotKeySkill.clear();
//
//            std::vector< EGCSkillTree > vecSkillID;
//            if( g_kGlobalValue.IsNewSkillTreePlayer( iChar ) ) {
//                iJob = GetCurrentChar().GetCurrentPromotion();
//                vecSkillID = SiKGCSkillTree()->GetHotKeySkillVectorByType( iChar, iJob, i );
//            } else {
//                vecSkillID = SiGCSKT()->GetHotKeySkillVectorByType( iChar, iJob, i );
//            }
//            for ( std::vector< EGCSkillTree >::iterator vit = vecSkillID.begin(); vit != vecSkillID.end(); vit++ )
//            {
//                if( g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *vit )->m_pHotKeySkill )
//                {
//                    GCHotKeySkill skill;
//                    skill.m_eSkill = *vit;
//                    GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *vit )->m_pHotKeySkill;
//                    std::vector< GCComboSkill >::iterator vecIter = info->m_vecComboSkill.begin();
//                    for(; vecIter != info->m_vecComboSkill.end(); ++vecIter )
//                    {
//                        vecIter->m_pTexture = g_pGCDeviceManager2->CreateTexture( g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, EGCSkillTree(vecIter->m_iSID) )->GetTextureFileName() );
//                    }
//                    skill.pTexture = g_pGCDeviceManager2->CreateTexture( g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *vit )->GetTextureFileName() );
//                    //m_vecHotKeySkill.push_back( skill );
//                    vecHotKeySkill.push_back( skill );
//                    if( info->m_iChargingSkillLV > 0 ) {
//                        m_vecPossibleChargingSkillInfo.push_back( *info );
//                    }
//                }
//            }
//
//            m_mapHotKeySkillBySlotType.insert( std::make_pair( i, vecHotKeySkill ) );
//        }
//
//        std::map< int, vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkillBySlotType.find( KSkillSlot::INDEX_A );
//        if( mit != m_mapHotKeySkillBySlotType.end() ) {
//            m_vecHotKeySkill = mit->second;
//        }
//#else
//        for ( std::vector< EGCSkillTree >::iterator sit = m_setSkill.begin(); sit != m_setSkill.end(); sit++ )
//        {
//            if( g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *sit )->m_pHotKeySkill )
//            {
//                GCHotKeySkill skill;
//                skill.m_eSkill = *sit;
//                GCHotKeySkillInfo* info = g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *sit )->m_pHotKeySkill;
//                std::vector< GCComboSkill >::iterator vecIter = info->m_vecComboSkill.begin();
//                for(; vecIter != info->m_vecComboSkill.end(); ++vecIter )
//                {
//                    vecIter->m_pTexture = g_pGCDeviceManager2->CreateTexture( g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, EGCSkillTree(vecIter->m_iSID) )->GetTextureFileName() );
//                }
//                skill.pTexture = g_pGCDeviceManager2->CreateTexture( g_kGlobalValue.GetSkill( GetCurrentChar().iCharType, *sit )->GetTextureFileName() );
//                m_vecHotKeySkill.push_back( skill );
//                if( info->m_iChargingSkillLV > 0 ) {
//                    m_vecPossibleChargingSkillInfo.push_back( *info );
//                }
//            }
//        }
//#endif
//
//        if( bDefaultSkillSetting ) {
//            if( !m_vecPossibleChargingSkillInfo.empty() ) {
//                std::sort( m_vecPossibleChargingSkillInfo.begin(), m_vecPossibleChargingSkillInfo.end(), KChargingSkillLevelHigh() );
//            }
//            return;
//        }
//
//        // 차징가능한 스킬 중 레벨이 높은 순으로 정렬 해주자. 이유는 마나 차징 수치에 따라서 해당 레벨에 맞는 필살기를 써주게 하기 위함이다.
//        if( !m_vecPossibleChargingSkillInfo.empty() ) {
//            std::sort( m_vecPossibleChargingSkillInfo.begin(), m_vecPossibleChargingSkillInfo.end(), KChargingSkillLevelHigh() );
//        }
//
//        //if( m_vecHotKeySkill.empty() && ( g_kGlobalValue.IsQuickSkillPlayer( iChar ) || g_kGlobalValue.IsHybridPlayer( iChar ) ) ) {
//        //
//        //}
//    }
//}

// 진짜 각성 들어가면 들어내야 하는 함수임. 이 안보고 나보고 머라 그라지 마셈!!!!!@#@$@#$!@#@!
void PLAYER::ReSettingSpecial4Skill(int iChar, int iJob)
{
	if (!IsLocalPlayer() || (g_MyD3D->m_pStateMachine->GetState() != GS_TUTORIAL && g_MyD3D->m_pStateMachine->GetState() != GS_GAME && g_MyD3D->m_pStateMachine->GetState() != GS_GAME_LOADING)) {
		return;
	}

	bool bCheckRelease = false;
	for (std::map< int, std::vector< GCHotKeySkill > >::iterator mit = m_mapHotKeySkill.begin(); mit != m_mapHotKeySkill.end(); ++mit) {
		for (std::vector< GCHotKeySkill >::iterator vit = mit->second.begin(); vit != mit->second.end(); vit++) {
			vit->pTexture->Release();
			bCheckRelease = true;
		}
	}
	m_mapHotKeySkill.clear();
	m_mapPossibleChargingSkillInfo.clear();

	for (int i = SKILL_TYPE_NORMAL_A; i < SKILL_TYPE_MAX; i++) {
		std::vector< GCHotKeySkill > vecTempHotKey;
		std::vector< GCHotKeySkillInfo > vecTempHotkeyInfo;
		m_mapHotKeySkill.insert(std::make_pair(i, vecTempHotKey));
		m_mapPossibleChargingSkillInfo.insert(std::make_pair(i, vecTempHotkeyInfo));
	}

	if (bCheckRelease == false) {
		for (std::vector< GCHotKeySkill >::iterator vit = m_vecHotKeySkill.begin(); vit != m_vecHotKeySkill.end(); vit++)
		{
			vit->pTexture->Release();
		}
	}
	m_vecHotKeySkill.clear();

	// 필살기를 착용하고 있어서 스왑이 일어났다면 여기서 스킬 교체를 해주도록 한다.
	// 흑/백마법 스킬들 따로 담도록 조절피요
	std::vector< EGCSkillTree > vecNormalSkill;
	std::map< int, std::vector< EGCSkillTree > > mapSpecialSkills;
	std::map< int, EGCSkillTree > mapSlotSkillA;
	std::map< int, EGCSkillTree > mapSlotSkillB;
	std::map< int, EGCSkillTree > mapCashSlotSkillA;
	std::map< int, EGCSkillTree > mapCashSlotSkillB;

	std::vector< EGCSkillTree >::iterator vit = m_setSkill.begin();
	for (vit; vit != m_setSkill.end(); ++vit) {
		const GCSkill* pSkill = SiGCSKT()->GetSkill(*vit);
		if (pSkill) {
			// 변신 스킬들은 제외시킴...
			if (pSkill->GetID() == SID_RYAN1_WOLF_SPECIAL1 ||
				pSkill->GetID() == SID_RYAN1_WOLF_SPECIAL2 ||
				pSkill->GetID() == SID_RYAN_NEPHILLIM_SPECIAL1 ||
				pSkill->GetID() == SID_RYAN_MEGNUS_DAN_SPECIAL1 ||
				pSkill->GetID() == SID_RYAN_MEGNUS_DAN_SPECIAL2 ||
				pSkill->GetID() == SID_RYAN_MEGNUS_DAN_SPECIAL3) {
				continue;
			}

			if (pSkill->m_pHotKeySkill && pSkill->m_pHotKeySkill->m_iChargingSkillLV > 0 && pSkill->m_pHotKeySkill->m_iChargingSkillLV <= 4) {
				// 4필은 다 넣어주자.
				if (pSkill->GetPromotion() == iJob && pSkill->GetCharType() == iChar) {
					if (pSkill->m_pHotKeySkill->m_iChargingSkillLV == 4) {
						mapSlotSkillA.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
						mapCashSlotSkillA.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
						if (m_mapTutorialDefaultEquipSkillB.empty() == false) {
							mapSlotSkillB.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
							mapCashSlotSkillB.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
						}
					}
					else {
						if (pSkill->m_bSpecialTypeB == false) {
							mapSlotSkillA.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
							mapCashSlotSkillA.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
						}
						else {
							mapSlotSkillB.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
							mapCashSlotSkillB.insert(std::make_pair(pSkill->m_pHotKeySkill->m_iChargingSkillLV, *vit));
						}
					}
				}
			}
			else {
				vecNormalSkill.push_back(*vit);
			}
		}
	}

	// 캐시 필살기 셋팅 해보자!!! 아오 이런식으로 만들기 싫은데@#@!@#!@!@!@#!!@#!
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCharInfo* pCharInfo = &GetCurrentChar();
	if (pCharInfo != NULL) {
		m_mapwSkillItem.clear();
		std::map< std::pair< int, int >, std::vector< SCashSkillInfo > >::iterator mitCashSkill;
		mitCashSkill = m_mapCashSkillInfo.find(std::make_pair(iChar, iJob));

		if (mitCashSkill != m_mapCashSkillInfo.end()) {
			for (int i = 0; i < static_cast<int>(pCharInfo->vecItemInfo.size()); i++) {
				GCItem* pItem = NULL;
				pItem = g_pItemMgr->GetItemData(pCharInfo->vecItemInfo[i].iItemID);
				if (pItem == NULL) {
					continue;
				}

				for (int j = 0; j < static_cast<int>(mitCashSkill->second.size()); j++) {
					for (int k = 0; k < static_cast<int>(mitCashSkill->second[j].m_vecGoodsID.size()); k++) {
						if (pCharInfo->vecItemInfo[i].iItemID == mitCashSkill->second[j].m_vecGoodsID[k]) {
							if (mitCashSkill->second[j].m_iSkillType == 0) {
								mapCashSlotSkillA[mitCashSkill->second[j].m_iSkillLv] = static_cast<EGCSkillTree>(mitCashSkill->second[j].m_iSkillID);
								if (pItem->eItemType == GCIT_COUNT) {//수량성이라면 수량을 깎을수 있도록 준비함
									m_mapwSkillItem[std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, mitCashSkill->second[j].m_iSkillLv)] = pCharInfo->vecItemInfo[i].dwItemUID;
								}

								// 캐쉬 스킬 잘못 등록된거 지우자
								std::map< int, EGCSkillTree >::iterator mit = mapSlotSkillA.find(mitCashSkill->second[j].m_iSkillLv);
								if (mit != mapSlotSkillA.end() && mit->second == mitCashSkill->second[j].m_iSkillID) {
									mapSlotSkillA.erase(mitCashSkill->second[j].m_iSkillLv);
								}
							}
							else {
								mapCashSlotSkillB[mitCashSkill->second[j].m_iSkillLv] = static_cast<EGCSkillTree>(mitCashSkill->second[j].m_iSkillID);
								if (pItem->eItemType == GCIT_COUNT) { //수량성이라면 수량을 깎을수 있도록 준비함
									m_mapwSkillItem[std::make_pair((EGCMETAMORPHOSIS)MetamorphosisForm, mitCashSkill->second[j].m_iSkillLv + 3)] = pCharInfo->vecItemInfo[i].dwItemUID;
								}

								std::map< int, EGCSkillTree >::iterator mit = mapSlotSkillB.find(mitCashSkill->second[j].m_iSkillLv);
								if (mit != mapSlotSkillB.end() && mit->second == mitCashSkill->second[j].m_iSkillID) {
									mapSlotSkillA.erase(mitCashSkill->second[j].m_iSkillLv);
								}
							}
						}
					}
				}
			}
		}

		if (Extra_Char_Num == CID_RYAN4) {
			std::map< int, EGCSkillTree >::iterator mit = mapCashSlotSkillA.find(2);
			if (mit == mapCashSlotSkillA.end()) {
				mapCashSlotSkillA[2] = SID_RYAN_MEGNUS_DAN_FORM;
			}
			mapCashSlotSkillB[2] = SID_RYAN_MEGNUS_DAN_FORM;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 1; i <= 4; i++) {
		// 기본 필살기 A 셋팅
		std::map< int, EGCSkillTree >::iterator mit = mapSlotSkillA.find(i);
		if (mit == mapSlotSkillA.end()) {
			std::map< std::pair< int, int >, std::vector< EGCSkillTree > >::iterator mitBaseSkill;
			mitBaseSkill = m_mapTutorialDefaultEquipSkillA.find(std::make_pair(iChar, iJob));
			if (mitBaseSkill != m_mapTutorialDefaultEquipSkillA.end() && static_cast<int>(mitBaseSkill->second.size()) > i - 1) {
				mapSpecialSkills[SKILL_TYPE_NORMAL_A].push_back(mitBaseSkill->second[i - 1]);
			}
		}
		else {
			mapSpecialSkills[SKILL_TYPE_NORMAL_A].push_back(mit->second);
		}

		// 기본 필살기 B 셋팅
		mit = mapSlotSkillB.find(i);
		if (mit == mapSlotSkillB.end()) {
			std::map< std::pair< int, int >, std::vector< EGCSkillTree > >::iterator mitBaseSkill;
			mitBaseSkill = m_mapTutorialDefaultEquipSkillB.find(std::make_pair(iChar, iJob));
			if (mitBaseSkill != m_mapTutorialDefaultEquipSkillB.end() && static_cast<int>(mitBaseSkill->second.size()) > i - 1) {
				mapSpecialSkills[SKILL_TYPE_NORMAL_B].push_back(mitBaseSkill->second[i - 1]);
			}
		}
		else {
			mapSpecialSkills[SKILL_TYPE_NORMAL_B].push_back(mit->second);
		}

		// 캐시 필살기 A 셋팅
		if (mapCashSlotSkillA.empty() == false) {
			mit = mapCashSlotSkillA.find(i);
			if (mit == mapCashSlotSkillA.end()) {
				std::map< std::pair< int, int >, std::vector< EGCSkillTree > >::iterator mitBaseSkill;
				mitBaseSkill = m_mapTutorialDefaultEquipSkillA.find(std::make_pair(iChar, iJob));
				if (mitBaseSkill != m_mapTutorialDefaultEquipSkillA.end() && static_cast<int>(mitBaseSkill->second.size()) > i - 1) {
					mapSpecialSkills[SKILL_TYPE_CASH_A].push_back(mitBaseSkill->second[i - 1]);
				}
			}
			else {
				mapSpecialSkills[SKILL_TYPE_CASH_A].push_back(mit->second);
			}
		}

		// 캐시 필살기 B 셋팅
		if (mapCashSlotSkillB.empty() == false) {
			mit = mapCashSlotSkillB.find(i);
			if (mit == mapCashSlotSkillB.end()) {
				std::map< std::pair< int, int >, std::vector< EGCSkillTree > >::iterator mitBaseSkill;
				mitBaseSkill = m_mapTutorialDefaultEquipSkillB.find(std::make_pair(iChar, iJob));
				if (mitBaseSkill != m_mapTutorialDefaultEquipSkillB.end() && static_cast<int>(mitBaseSkill->second.size()) > i - 1) {
					mapSpecialSkills[SKILL_TYPE_CASH_B].push_back(mitBaseSkill->second[i - 1]);
				}
			}
			else {
				mapSpecialSkills[SKILL_TYPE_CASH_B].push_back(mit->second);
			}
		}
	}

	for (std::map< int, std::vector< EGCSkillTree > >::iterator mit = mapSpecialSkills.begin(); mit != mapSpecialSkills.end(); ++mit) {
		for (int i = 0; i < static_cast<int>(mit->second.size()); i++) {
			const GCSkill* pSkill = SiGCSKT()->GetSkill(mit->second[i]);
			if (pSkill && pSkill->m_pHotKeySkill && iChar == pSkill->GetCharType())
			{
				GCHotKeySkill skill;
				skill.m_eSkill = mit->second[i];
				GCHotKeySkillInfo* info = SiGCSKT()->GetSkill(mit->second[i])->m_pHotKeySkill;
				skill.pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(mit->second[i])->GetTextureFileName());
				skill.pAddMotionTexture = g_pGCDeviceManager2->CreateTexture(pSkill->GetAddMotionTextureFileName());
				m_mapHotKeySkill[mit->first].push_back(skill);
				if (info->m_iChargingSkillLV > 0 && info->m_iChargingSkillLV < 4) {
					m_mapPossibleChargingSkillInfo[mit->first].push_back(*info);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////// 변신 스킬들 셋팅하는 슈퍼하드코딩 //////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if (GetCurrentChar().iCharType == GC_CHAR_RYAN) {
		std::vector< EGCSkillTree > vecSkillTemp;
		const GCSkill* pSkill = NULL;
		vecSkillTemp.push_back(SID_RYAN_MEGNUS_DAN_SPECIAL1);
		vecSkillTemp.push_back(SID_RYAN_MEGNUS_DAN_SPECIAL2);
		vecSkillTemp.push_back(SID_RYAN_MEGNUS_DAN_SPECIAL3);

		for (int i = 0; i < static_cast<int>(vecSkillTemp.size()); i++) {
			pSkill = SiGCSKT()->GetSkill(vecSkillTemp[i]);
			if (pSkill && pSkill->m_pHotKeySkill)
			{
				GCHotKeySkill skill;
				skill.m_eSkill = vecSkillTemp[i];
				GCHotKeySkillInfo* info = SiGCSKT()->GetSkill(vecSkillTemp[i])->m_pHotKeySkill;
				skill.pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(vecSkillTemp[i])->GetTextureFileName());
				m_mapHotKeySkill[FORM_MDAN].push_back(skill);
				if (info->m_iChargingSkillLV > 0) {
					m_mapPossibleChargingSkillInfo[FORM_MDAN].push_back(*info);
				}
			}
		}

		pSkill = SiGCSKT()->GetSkill(SID_RYAN_NEPHILLIM_SPECIAL1);
		if (pSkill && pSkill->m_pHotKeySkill)
		{
			GCHotKeySkill skill;
			skill.m_eSkill = SID_RYAN_NEPHILLIM_SPECIAL1;
			GCHotKeySkillInfo* info = SiGCSKT()->GetSkill(SID_RYAN_NEPHILLIM_SPECIAL1)->m_pHotKeySkill;
			skill.pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(SID_RYAN_NEPHILLIM_SPECIAL1)->GetTextureFileName());
			m_mapHotKeySkill[FORM_NEPHILIM].push_back(skill);
			if (info->m_iChargingSkillLV > 0) {
				m_mapPossibleChargingSkillInfo[FORM_NEPHILIM].push_back(*info);
			}
		}

		vecSkillTemp.clear();
		if (SiGCSKT()->IsEquipedSkill(SID_RYAN1_WOLF_SPECIAL1, 0)) {
			vecSkillTemp.push_back(SID_RYAN1_WOLF_SPECIAL1);
		}
		if (SiGCSKT()->IsEquipedSkill(SID_RYAN1_WOLF_SPECIAL2, 0)) {
			vecSkillTemp.push_back(SID_RYAN1_WOLF_SPECIAL2);
		}
		for (int i = 0; i < static_cast<int>(vecSkillTemp.size()); i++) {
			pSkill = SiGCSKT()->GetSkill(vecSkillTemp[i]);
			if (pSkill && pSkill->m_pHotKeySkill)
			{
				GCHotKeySkill skill;
				skill.m_eSkill = vecSkillTemp[i];
				GCHotKeySkillInfo* info = SiGCSKT()->GetSkill(vecSkillTemp[i])->m_pHotKeySkill;
				skill.pTexture = g_pGCDeviceManager2->CreateTexture(SiGCSKT()->GetSkill(vecSkillTemp[i])->GetTextureFileName());
				m_mapHotKeySkill[FORM_WOLF].push_back(skill);
				if (info->m_iChargingSkillLV > 0) {
					m_mapPossibleChargingSkillInfo[FORM_WOLF].push_back(*info);
				}
			}
		}

		if (vecSkillTemp.empty()) {
			std::vector< GCHotKeySkill > vecTempHotKey;
			std::vector< GCHotKeySkillInfo > vecTempHotkeyInfo;
			m_mapHotKeySkill.insert(std::make_pair(FORM_WOLF, vecTempHotKey));
			m_mapPossibleChargingSkillInfo.insert(std::make_pair(FORM_WOLF, vecTempHotkeyInfo));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	for (std::map< int, std::vector< GCHotKeySkillInfo > >::iterator mit = m_mapPossibleChargingSkillInfo.begin(); mit != m_mapPossibleChargingSkillInfo.end(); ++mit) {
		std::sort(mit->second.begin(), mit->second.end(), KChargingSkillLevelHigh());
	}

	std::map< int, std::vector< GCHotKeySkill > >::iterator mitSkill = m_mapHotKeySkill.find(SKILL_TYPE_NORMAL_A);
	if (mitSkill != m_mapHotKeySkill.end()) {
		m_vecHotKeySkill = mitSkill->second;
	}

	std::map< int, std::vector< GCHotKeySkillInfo > >::iterator mitChargingSkill = m_mapPossibleChargingSkillInfo.find(SKILL_TYPE_NORMAL_A);
	if (mitChargingSkill != m_mapPossibleChargingSkillInfo.end()) {
		m_vecPossibleChargingSkillInfo = mitChargingSkill->second;
	}
}

void PLAYER::ProcessFreezeMotion()
{
	PROFILE_SET("PLAYER::ProcessFreezeMotion");

	bool bFreezeMotion = false;

	if (GetOwnMotion(MID_COMMON_FREEZED_BY_PIXIE) == uiMotion)
	{
		// 2프레임마다 얼음 이펙트를 띄워준다
		// 다른 잡아두는 공격을 맞았다면 다른 이펙트를 띄워줘야 한다
		if ((m_iFreezeCount % 2) == 0)
		{
			if (cFrame < 33)
				AddDamage(DT_PIXIE_ICE1, 0);
			else if (cFrame < 66)
				AddDamage(DT_PIXIE_ICE2, 0);
			else if (cFrame < 99)
				AddDamage(DT_PIXIE_ICE3, 0);
			else
				AddDamage(DT_PIXIE_ICE4, 0);
		}
		bFreezeMotion = true;
	}
	else if (GetOwnMotion(MID_COMMON_FREEZED_BY_ENT) == uiMotion)
	{
		if ((m_iFreezeCount % 10) == 0)
		{
			if (cFrame < 33)
				AddDamage(DT_ENT_TRUNK1, 0);
			else if (cFrame < 66)
				AddDamage(DT_ENT_TRUNK2, 0);
			else if (cFrame < 99)
				AddDamage(DT_ENT_TRUNK3, 0);
			else
				AddDamage(DT_ENT_TRUNK4, 0);
		}
		bFreezeMotion = true;
	}
	else if (GetOwnMotion(MID_COMMON_FREEZED_BY_LICH) == uiMotion)
	{
		if ((m_iFreezeCount % 10) == 0)
		{
			switch ((m_iFreezeCount / 10) % 5)
			{
			case 0:
				AddParticle("tomb_hand_01_01", 0, 0.0f);
				break;
			case 1:
				AddParticle("tomb_hand_01_02", 0, 0.0f);
				break;
			case 2:
				AddParticle("tomb_hand_01_03", 0, 0.0f);
				break;
			case 3:
				AddParticle("tomb_hand_01_04", 0, 0.0f);
				break;
			case 4:
				AddParticle("tomb_hand_01_05", 0, 0.0f);
				break;
			}
			AddParticle("tomb_hand_02", 0, 0.0f);
		}
		bFreezeMotion = true;
	}
	else if (GetOwnMotion(MID_SKILL_ARME1_ICESTRIKE_FREEZING) == uiMotion)
	{
		if ((m_iFreezeCount % 2) == 0)
		{
			if (cFrame < 33)
				AddDamage(DT_PIXIE_ICE1, 0);
			else if (cFrame < 66)
				AddDamage(DT_PIXIE_ICE2, 0);
			else if (cFrame < 99)
				AddDamage(DT_PIXIE_ICE3, 0);
			else
			{
				AddDamage(DT_PIXIE_ICE4, 0);
			}
		}
	}

	// 얼려진 모션이라면..
	if (bFreezeMotion)
	{
		if ((m_iFreezeCount % 10) == 0) // 10프레임마다
		{
			// 조이스틱 좌우로 흔드는 표시 보여준다
			int iLeftRightEffect = DT_JOYSTICK_LEFT_HIGH + ((int)(m_iFreezeCount / 10) % 2);
			AddDamage(iLeftRightEffect, 0, true);

			// 몬스터에게 맞았다면 프레임 증가를 천천히 하게 해 준다
			// 10프레임당 1프레임씩 올라가서 모션의 10배 길이만큼 얼려져 있는다
			if (GetLastKillmePlayer() >= MAX_PLAYER_NUM)
			{
				if (m_iPlayerIndex == g_MyD3D->Get_MyPlayer())
				{
					++cFrame;
					if (cFrame >= (int)GetNowMotionFRM()->GetNum_Frame())
					{
						cFrame = (int)GetNowMotionFRM()->GetNum_Frame() - 1;
						m_bFrameLock = false;
					}

					// 빨리 안풀려나면 데미지를 지속적으로 들어가게 해 준다
					// 데미지 수치는 그다지 높지 않음
					DamageInfo damage;
					damage.SetDamage(m_fFreezeDamage);
					damage.SetAttacker(DamageInfo::DAMAGE_MONSTER, 0);
					Change_HP(damage);
				}
			}
		}
		++m_iFreezeCount;

		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_HOLY_LV3_SHIELD);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RENCIA_SHIELD);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_CHOUCRING_SHIELD);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_DOKKAEBIE_SHIELD);
	}
}

void  PLAYER::AddParticleXOffsetWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fTraceTime, int iTracePos)
{
	std::string tempParticle = strSeqeunce;

	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		tempParticle = "";

	CParticleEventSeqPTR pTemp;
	pTemp = g_ParticleManager->CreateSequence(tempParticle, vPos.x - 1.0f + fOffsetX,
		vPos.y - 0.48f + y_Ex,
		0.5f);
	if (pTemp != NULL)
	{
		// 방향성 세팅
		DIRECTION_INFO stDirect;
		stDirect.m_fDirPosX = 0.0f;
		stDirect.m_bDirVelocity = true;
		stDirect.m_pbIsRight = &bIsRight;
		pTemp->SetDirectInfo(&stDirect);
		pTemp->SetShow(true);

		TRACE_INFO stTrace;
		stTrace.m_pvPos = ComputePosDesc(m_posDesc, iTracePos);
		if (fTraceTime == 0.0f)
			stTrace.m_fParticleTraceTime = pTemp->GetKillTime();
		else
			stTrace.m_fParticleTraceTime = fTraceTime;
		stTrace.m_fXOffset = fOffsetX;
		pTemp->SetTrace(&stTrace);

		if (iParticleNum != 0)
		{
			CMinMax<float> emitRate(1000.0f, 1000.0f);
			pTemp->SetEmitRate(emitRate);
			pTemp->SetKillNum(iParticleNum);
		}
	}

	return;
}

void  PLAYER::AddParticleXOffset(char* strSeqeunce, int iParticleNum, float fOffsetX)
{
	std::string tempParticle = strSeqeunce;

	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		tempParticle = "";

	CParticleEventSeqPTR pTemp;
	pTemp = g_ParticleManager->CreateSequence(tempParticle, vPos.x - 1.0f + fOffsetX,
		vPos.y - 0.48f + y_Ex,
		0.5f);
	if (pTemp != NULL)
	{
		// 방향성 세팅
		DIRECTION_INFO stDirect;
		stDirect.m_fDirPosX = 0.0f;
		stDirect.m_bDirVelocity = true;
		stDirect.m_pbIsRight = &bIsRight;
		pTemp->SetDirectInfo(&stDirect);
		pTemp->SetShow(true);

		if (iParticleNum != 0)
		{
			CMinMax<float> emitRate(1000.0f, 1000.0f);
			pTemp->SetEmitRate(emitRate);
			pTemp->SetKillNum(iParticleNum);
		}
	}

	return;
}

void PLAYER::AddParticleXOffsetNoMotion(char* strSeqeunce, int iParticleNum, float fOffsetX)
{
	std::string tempParticle = strSeqeunce;

	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		tempParticle = "";

	CParticleEventSeqPTR pTemp;
	pTemp = g_ParticleManager->CreateSequence(tempParticle, vPos.x - 1.0f + fOffsetX,
		vPos.y - 0.48f,
		0.5f);
	if (pTemp != NULL)
	{
		// 방향성 세팅
		DIRECTION_INFO stDirect;
		stDirect.m_fDirPosX = 0.0f;
		stDirect.m_bDirVelocity = true;
		stDirect.m_pbIsRight = &bIsRight;
		pTemp->SetDirectInfo(&stDirect);
		pTemp->SetShow(true);

		if (iParticleNum != 0)
		{
			CMinMax<float> emitRate(1000.0f, 1000.0f);
			pTemp->SetEmitRate(emitRate);
			pTemp->SetKillNum(iParticleNum);
		}
	}

	return;
}

void PLAYER::AddParticleUINumber(int iNumber, D3DXVECTOR2 vOffsetPos, bool bPlusMark)
{
	// 번호 분해
	std::vector<int> vecNumber;
	while (iNumber)
	{
		vecNumber.push_back((iNumber % 10));
		iNumber = (iNumber / 10);
	}

	CParticleEventSeqPTR pTempParticle;

	if (SiKGCRoomManager()->GetGameMode() == GC_GM_DOTA)
	{
		// APoint 출력
		if (bPlusMark)
		{
			pTempParticle = g_ParticleManager->CreateSequence("D_Text_GD_Small_Apoint", vPos.x + vOffsetPos.x, vPos.y + vOffsetPos.y, 0.5f);
			if (pTempParticle)
				pTempParticle->SetShow(true);
			vOffsetPos.x += 0.075f;
		}
	}

	// 파티클 생성
	char strSeqeunce[256];
	for (int iLoop = (vecNumber.size() - 1); iLoop >= 0; --iLoop)
	{
		sprintf(strSeqeunce, "D_Text_GD_Small_0%d", vecNumber[iLoop]);
		pTempParticle = g_ParticleManager->CreateSequence(strSeqeunce, vPos.x + vOffsetPos.x, vPos.y + vOffsetPos.y, 0.5f);
		if (pTempParticle)
			pTempParticle->SetShow(true);

		vOffsetPos.x += 0.035f;
	}
}

bool PLAYER::IsCatchableMotion(UINT uiMotion)
{
	switch (uiMotion)
	{
	case MID_RYAN4_DASH_ATK2:
	case MID_RYAN4_DASH_ATK3:
	case MID_MARI1_WDW_DASH_ATK:
	case MID_MARI2_WDW_DASH_ATK:
	case MID_MARI3_JSJ_DASH_ATK:
		return true;
	default:
		return false;
	}
	return false;
}

void PLAYER::SetShield(int iShield, float fRegenRate, int iLimitTime, bool bSpecialShield, int iTexNum, bool bCountingShield_)
{
	m_fIncHPRatio -= m_fRegenRate;
	Shield = iShield;
	m_iPrevShield = iShield;
	m_iShieldTime = 0;
	m_iShieldLimitTime = iLimitTime * (int)GC_FPS_LIMIT;
	m_fRegenRate = fRegenRate;
	m_fIncHPRatio += m_fRegenRate;
	m_bSpecialShield = bSpecialShield;
	m_iShieldTexNum = iTexNum;
	m_bShieldTypeCounting = bCountingShield_;
}

void PLAYER::SetHostPlayer(bool bIsLeaveHost_ /*= true*/)
{
	m_kUserInfo.bHost = true;

	if (bIsLeaveHost_) {
		if (m_kUserInfo.nUserState == GC_RUS_READY)
			m_kUserInfo.nUserState = GC_RUS_WAIT;
	}

	//m_kUserInfo.bReady = false;
	if (IsLocalPlayer())
	{
		g_kGlobalValue.m_kUserInfo.bHost = true;

		if (bIsLeaveHost_) {
			if (g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY)
				g_kGlobalValue.m_kUserInfo.nUserState = GC_RUS_WAIT;

			if (g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM)
			{
				g_MyD3D->m_TempPlayer.m_kUserInfo.bHost = true;
				if (g_MyD3D->m_TempPlayer.m_kUserInfo.nUserState == GC_RUS_READY)
					g_MyD3D->m_TempPlayer.m_kUserInfo.nUserState = GC_RUS_WAIT;
			}
			SiKGCRoomManager()->GetRoomInfo().iRestBanCount = DEFAULT_BAN_COUNT;
			g_pkUIScene->EnableMessageBox(true);
		}
	}
}

std::string PLAYER::GetCharString()
{
	switch (Extra_Char_Num)
	{
#define CHARTOSTRING(p) case p: return #p; break;
		CHARTOSTRING(CID_COMMON);
		CHARTOSTRING(CID_ELESIS1);
		CHARTOSTRING(CID_LIRE1);
		CHARTOSTRING(CID_ARME1);
		CHARTOSTRING(CID_LAS1);
		CHARTOSTRING(CID_RYAN1);
		CHARTOSTRING(CID_RONAN1);
		CHARTOSTRING(CID_AMY1);
		CHARTOSTRING(CID_JIN1);
		CHARTOSTRING(CID_SIEG1);
		CHARTOSTRING(CID_ELESIS2);
		CHARTOSTRING(CID_LIRE2);
		CHARTOSTRING(CID_ARME2);
		CHARTOSTRING(CID_LAS2);
		CHARTOSTRING(CID_RYAN2);
		CHARTOSTRING(CID_RONAN2);
		CHARTOSTRING(CID_AMY2);
		CHARTOSTRING(CID_JIN2);
		CHARTOSTRING(CID_SIEG2);
		CHARTOSTRING(CID_ELESIS3);
		CHARTOSTRING(CID_LIRE3);
		CHARTOSTRING(CID_ARME3);
		CHARTOSTRING(CID_LAS3);
		CHARTOSTRING(CID_RYAN3);
		CHARTOSTRING(CID_RONAN3);
		CHARTOSTRING(CID_AMY3);
		CHARTOSTRING(CID_JIN3);
		CHARTOSTRING(CID_SIEG3);
		CHARTOSTRING(CID_ELESIS4);
		CHARTOSTRING(CID_LIRE4);
		CHARTOSTRING(CID_ARME4);
		CHARTOSTRING(CID_LAS4);
		CHARTOSTRING(CID_RYAN4);
		CHARTOSTRING(CID_RONAN4);
		CHARTOSTRING(CID_AMY4);
		CHARTOSTRING(CID_JIN4);
		CHARTOSTRING(CID_SIEG4);
		CHARTOSTRING(CID_MARI4);
	}

	return EnumToString(CID_MAX);
}

std::string PLAYER::GetFormString()
{
	switch (MetamorphosisForm)
	{
#define FORMTOSTRING(p) case p: return #p; break;
		FORMTOSTRING(FORM_NORMAL);
		FORMTOSTRING(FORM_WOLF);
		FORMTOSTRING(FORM_NEPHILIM);
		// For_Thanatos
		FORMTOSTRING(FORM_MDAN);
	}

	return EnumToString(FORM_NORMAL);
}

void PLAYER::AddBlurAlpha(float fAlpla)
{
	/*0.001f*/
	g_MyD3D->m_KGCBlurMgr.AddBlurAlpha(fAlpla);
}

void PLAYER::EndBlur()
{
	g_MyD3D->m_KGCBlurMgr.BlurEnd();
}

int PLAYER::GetOwnMotion(int Motion, bool bReverse)
{
	int iResult = Motion;

	if (!bReverse)
	{
		//공통 모션을 사용하지 않는 케릭터일 경우에 자신만의 모션을 찾아봅니다.
		std::map<int, std::pair<int, int> >::iterator IsOwnMotion = GetCurFormTemplate().mapCommonToOwnMotion.find(Motion);

		if (IsOwnMotion == GetCurFormTemplate().mapCommonToOwnMotion.end())
		{
			iResult = Motion;
		}
		else
		{
			iResult = GetCurFormTemplate().mapCommonToOwnMotion[Motion].second;
		}
	}
	else
	{
		//공통 모션을 사용하지 않는 케릭터일 경우에 모션은 다르더라도 공통모션의 FrameInfoProc을 사용합니다.
		std::map< int, std::pair<int, int> >::iterator IsOwnMotion;

		for (IsOwnMotion = GetCurFormTemplate().mapCommonToOwnMotion.begin(); IsOwnMotion != GetCurFormTemplate().mapCommonToOwnMotion.end(); IsOwnMotion++)
		{
			if (IsOwnMotion->second.second == Motion)
			{
				iResult = IsOwnMotion->second.first;
				break;
			}
			else
			{
				iResult = Motion;
			}
		}
	}

	return iResult;
}

void PLAYER::HitEffect(GCCollisionRect<float>& kWeapon_, GCCollisionRect<float>& kTarget)
{
	GCCollisionRect<float> kCollisionRect;

	kTarget.CheckCollision(kWeapon_, &kCollisionRect);

	if (GetCurFormTemplate().vecHitEffect.size() == 0)
	{
		g_MyD3D->MySparks->Particles(kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY(), 0, (int)m_fPhysicAtkPower, 15, m_iPlayerIndex);
		g_MyD3D->MySparks->Add(HIT_LIGHT, kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY(), 0, .7f, -100.0f, -100.0f, 0.0f, false, m_iPlayerIndex);
		SetDamageEff(kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY() - 0.3f, 0.4f);
	}
	else
	{
		for (int i = 0; i < (int)GetCurFormTemplate().vecHitEffect.size(); ++i)
		{
			HIT_EFFECT hit = GetCurFormTemplate().vecHitEffect[i];
			if (hit.first == HE_RANDOM_DIRECTION_SPARK)
			{
				g_MyD3D->MySparks->Particles(kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY(), 0, (int)m_fPhysicAtkPower, 15, m_iPlayerIndex, hit.second);
			}
			else if (hit.first == HE_HIT_EFFECT_SPARK)
			{
				g_MyD3D->MySparks->Add(hit.second, kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY(), 0, .7f, -100.0f, -100.0f, 0.0f, false, m_iPlayerIndex);
			}
			else if (hit.first == HE_HIT_EFFECT_BIM)
			{
				g_MyD3D->MySparks->Bim(kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY(), 0, hit.second);
			}
			else if (hit.first == HE_HIT_EFFECT_CUSTOM_PARTICLE)
			{
				CParticleEventSeqPTR pTemp;
				pTemp = g_ParticleManager->CreateSequence(hit.strParticleName, kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY() - 0.3f, 0.4f);
				if (pTemp != NULL)
				{
					// 방향성 세팅
					DIRECTION_INFO stDirect;
					stDirect.m_bIsRight = bIsRight;
					stDirect.m_bDirVelocity = true;
					stDirect.m_bReverseTexture = true;
					pTemp->SetDirectInfo(&stDirect);
				}
			}
		}
		SetDamageEff(kCollisionRect.GetCenterX(), kCollisionRect.GetCenterY() - 0.3f, 0.4f);
	}
}

//게임 로딩중에 한번 불러주자..
void PLAYER::UpdatePlayerResourceOnGame()//게임에서 필요한것들 까지 모두 로딩한다 -> 필살기 이미지 같은...
{
	ReleasePlayerResourceOnGame();

	int iCharType = m_kUserInfo.GetCurrentChar().iCharType;
	int iExtraCharNum = Extra_Char_Num;
	int iJobLevel = Extra_Char_Num / GC_CHAR_NUM;

	//공용 로딩하기
	m_pTexFatal = g_pGCDeviceManager2->CreateTexture("fatal.dds");

	if (iCharType == GC_CHAR_ELESIS)
	{
		m_pTexBerserk1 = g_pGCDeviceManager2->CreateTexture("berserk1.dds");
		m_pTexBerserk2 = g_pGCDeviceManager2->CreateTexture("berserk2.dds");
	}
	else if (iCharType == GC_CHAR_SIEG)
	{
		m_pTexBerserk1 = g_pGCDeviceManager2->CreateTexture("berserk1.dds");
		m_pTexBerserk2 = g_pGCDeviceManager2->CreateTexture("berserk2.dds");
	}
	else if (iCharType == GC_CHAR_ARME)
	{
		m_pTexBerserk1 = g_pGCDeviceManager2->CreateTexture("Mp_Regen_gauge.dds");
		m_pTexBerserk2 = g_pGCDeviceManager2->CreateTexture("Mp_Regen_01.dds");
	}
	else if (iCharType == GC_CHAR_RYAN || iCharType == GC_CHAR_MARI)
	{
		m_pTexBerserk1 = g_pGCDeviceManager2->CreateTexture("Ryan_Charging.dds");
	}
	else if (iCharType == GC_CHAR_RONAN)
	{
		m_pTexBerserk1 = g_pGCDeviceManager2->CreateTexture("RonanBerserkGauge.dds");
	}

	if (Extra_Char_Num == CID_AMY4)
	{
		m_pAmy4Action1 = g_pGCDeviceManager2->CreateTexture("amy4-1.dds");
		m_pAmy4Action2 = g_pGCDeviceManager2->CreateTexture("amy4-2.dds");
		m_pAmy4Action3 = g_pGCDeviceManager2->CreateTexture("amy4-3.dds");
	}

	if (GetCurFormTemplate().sPlayerSlash.strSlashName != "")
	{
		m_pTexSlash = g_pGCDeviceManager2->CreateTexture(GetCurFormTemplate().sPlayerSlash.strSlashName);
	}

	if (GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName.compare("NULL") != 0)
	{
		m_pPlayerSp = g_pGCDeviceManager2->CreateTexture(GetCurFormTemplate().sPlayerSpecialFace.strFaceTextureName);
	}

	LoadExtraMesh(GetCurFormTemplate().vecExtraMesh);
}

void PLAYER::ReleasePlayerResourceOnGame()
{
	SAFE_RELEASE(m_pTexSlash);
	SAFE_RELEASE(m_pTexBerserk1);
	SAFE_RELEASE(m_pTexBerserk2);
	SAFE_RELEASE(m_pTexFatal);

	SAFE_RELEASE(m_pPlayerSp);
	SAFE_RELEASE(m_pSerdinSign);
	SAFE_RELEASE(m_pCanabanSign);

	SAFE_RELEASE(m_pAmy4Action1);
	SAFE_RELEASE(m_pAmy4Action2);
	SAFE_RELEASE(m_pAmy4Action3);

	if (m_pObject)
	{
		for (std::vector< EXTRA_MESH >::iterator i = GetCurFormTemplate().vecExtraMesh.begin();
			i != GetCurFormTemplate().vecExtraMesh.end(); i++)
		{
			m_pObject->RemoveMesh((*i).strID);
		}
	}
}

GCDeviceMotionFRM* PLAYER::GetNowMotionFRM()
{
	return g_pGraphicsHelper->GetPlayerActionMotion(uiMotion);
}

void PLAYER::EffectDust(float fPosX, float fPosY)
{
	int iLatency = 0;

	if (!bIsRight)
	{
		fPosX = -fPosX;
	}

	D3DXVECTOR2 vDustPos(vPos.x - 1.0f + fPosX, vPos.y - 0.2f + fPosY);

	if (IsLocalPlayer())
		iLatency = LATENCY + SHOWLATENCY;

	if (SiKGCRoomManager()->IsMonsterGameMode())
	{
		//if( SiKGCFantasticMap()->IsWaterStage() && SiKGCFantasticMap()->GetO2Manager() )
		{
			if (SiKGCFantasticMap()->IsWaterStage() && SiKGCFantasticMap()->GetO2Manager()->IsInWater(m_iPlayerIndex))
			{
				g_MyD3D->MySparks->WaterSmogs(vDustPos.x, vDustPos.y, iLatency);
			}
			else if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST41)
			{
				g_MyD3D->MySparks->LeafSmogs(vDustPos.x, vDustPos.y, iLatency);
			}
			else if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST16 && g_pkQuestManager->m_iCurStage == 1)
			{
				g_ParticleManager->CreateSequence("Kaz_dusts", vPos.x - 1.0f, vPos.y - 0.68f + y_Ex, 0.5f);
			}
			else if (m_bIsEquipCoordiSetItem == true)
			{
				g_ParticleManager->CreateSequence(m_kCoordiSetTemplet.strSequence, vPos.x - 1.0f, vPos.y - 0.68f + y_Ex, 0.5f);
			}
			else
			{
				g_MyD3D->MySparks->Smogs(vDustPos.x, vDustPos.y, iLatency);
			}
		}
	}
	else
	{
		if (g_MyD3D->MyStg->GetStage() >= GC_GS_QUEST_ZERO_A && g_MyD3D->MyStg->GetStage() <= GC_GS_QUEST_ZERO_C)
		{
			g_MyD3D->MySparks->LeafSmogs(vDustPos.x, vDustPos.y, iLatency);
		}
		else if (g_MyD3D->MyStg->GetStage() >= GC_GS_CASTUL_TEMPLE_UNDER_A && g_MyD3D->MyStg->GetStage() <= GC_GS_CASTUL_TEMPLE_UNDER_L)
		{
			g_MyD3D->MySparks->UndeadSmogs(vDustPos.x, vDustPos.y, iLatency);
		}
		else
		{
			g_MyD3D->MySparks->Smogs(vDustPos.x, vDustPos.y, iLatency);
		}
	}
}

void PLAYER::OnStartGame()
{
	if (!IsLocalPlayer())
		return;

	SetStartMana();

	InitBurnningGauge();
}

bool PLAYER::Is_SP_Character()
{
	int iCharType = GetCurrentChar().iCharType;
	if (g_kGlobalValue.IsQuickSkillPlayer(iCharType) ||
		g_kGlobalValue.IsHybridPlayer(iCharType))
	{
		return true;
	}
	return false;
}

bool PLAYER::Is_Hybrid_Character()
{
	int iCharType = GetCurrentChar().iCharType;
	if (g_kGlobalValue.IsHybridPlayer(iCharType))
	{
		return true;
	}
	return false;
}

bool PLAYER::Is_AP_Character()
{
	int iCharType = GetCurrentChar().iCharType;
	if (g_kGlobalValue.IsQuickSkillPlayer(iCharType))
	{
		return true;
	}
	return false;
}

bool PLAYER::IsOneWeaponChar()
{
	int iCharType = GetCurrentChar().iCharType;
	if (g_kGlobalValue.IsQuickSkillPlayer(iCharType) ||
		g_kGlobalValue.IsHybridPlayer(iCharType))
	{
		return true;
	}
	return false;
}

int  PLAYER::GetSpecialGrade()
{
	if (uiMotion == GetCurFormTemplate().SPECIAL1_W || uiMotion == GetCurFormTemplate().SPECIAL1_B)
		return 1;
	else if (uiMotion == GetCurFormTemplate().SPECIAL2_W || uiMotion == GetCurFormTemplate().SPECIAL2_B)
		return 2;
	else if (uiMotion == GetCurFormTemplate().SPECIAL3_W || uiMotion == GetCurFormTemplate().SPECIAL3_B)
		return 3;
	//else if ( uiMotion == GetCurFormTemplate().SPECIAL4 )
	//    return 4;

	return 0;
}

bool PLAYER::IsSpecialSkillMotion(int iMotion_)
{
	if (iMotion_ == -1) {
		// 필살기가 쪼개져 있는것들은 따로 모션을 등록해야 되네요 ㅎ;;
		if (uiMotion == GetCurFormTemplate().SPECIAL1_W ||
			uiMotion == GetCurFormTemplate().SPECIAL2_W ||
			uiMotion == GetCurFormTemplate().SPECIAL3_W ||
			uiMotion == GetCurFormTemplate().SPECIAL1_B ||
			uiMotion == GetCurFormTemplate().SPECIAL2_B ||
			uiMotion == GetCurFormTemplate().SPECIAL3_B)
			return true;

		for (int i = 0; i < (int)GetCurFormTemplate().SPECIAL_ADD_MOTION.size(); i++)
		{
			if (uiMotion == GetCurFormTemplate().SPECIAL_ADD_MOTION[i])
				return true;
		}

		return m_bSpecialSkill;
	}
	else {
		for (int i = 0; i < (int)GetCurFormTemplate().SPECIAL_ADD_MOTION.size(); i++)
		{
			if (iMotion_ == GetCurFormTemplate().SPECIAL_ADD_MOTION[i])
				return true;
		}

		return false;
	}
}

bool PLAYER::IsSwapSpecialSkillMotion()
{
	if (uiMotion == m_SwapTemplate.SPECIAL1_W ||
		uiMotion == m_SwapTemplate.SPECIAL2_W ||
		uiMotion == m_SwapTemplate.SPECIAL3_W ||
		uiMotion == m_SwapTemplate.SPECIAL1_B ||
		uiMotion == m_SwapTemplate.SPECIAL2_B ||
		uiMotion == m_SwapTemplate.SPECIAL3_B)
		return true;
	return false;
}

CParticleEventSeqPTR PLAYER::AddTraceParticle(const std::string& strSequence, float fOffsetX, float fOffsetY, float fTraceTime, int iTracePos)
{
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSequence);

	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		tempParticle = "";

	CParticleEventSeqPTR pTemp;

	pTemp = g_ParticleManager->CreateSequence(tempParticle, vPos.x - 1.0f, vPos.y - 0.48f, 0.5f);
	if (pTemp != NULL)
	{
		// 방향성 세팅
		DIRECTION_INFO stDirect;
		stDirect.m_fDirPosX = -fOffsetX;
		stDirect.m_bDirVelocity = true;
		stDirect.m_pbIsRight = &bIsRight;
		pTemp->SetDirectInfo(&stDirect);
		pTemp->SetShow(true);

		TRACE_INFO stTrace;
		stTrace.m_pvPos = ComputePosDesc(m_posDesc, iTracePos);
		stTrace.m_fYOffset = fOffsetY;

		if (fTraceTime != FLT_MAX)
		{
			stTrace.m_fParticleTraceTime = fTraceTime;
		}
		else
		{
			stTrace.m_fParticleTraceTime = pTemp->GetKillTime();
		}
		pTemp->SetTrace(&stTrace);
	}
	return pTemp;
}

void PLAYER::AddParticle(char* strSeqeunce, int iParticleNum, float fOffsetX)
{
	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		return;
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSeqeunce);
	m_ParticleMsg.push_back(MSG_PARTICE_CREATE_INFO(tempParticle.c_str(), 0.0f, 0.0f, iParticleNum, fOffsetX));
}

void PLAYER::AddParticlePos(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY)
{
	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		return;
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSeqeunce);
	m_ParticleMsg.push_back(MSG_PARTICE_CREATE_INFO(tempParticle.c_str(), fOffsetX, fOffsetY, iParticleNum));
}

void PLAYER::AddParticleWithAngle(char* strSeqeunce, int iParticleNum, float fPosX, float fPosY, float fOffsetX)
{
	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		return;
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSeqeunce);
	m_ParticleMsg.push_back(MSG_PARTICE_CREATE_INFO(tempParticle.c_str(), fPosX, fPosY, iParticleNum, fOffsetX, true, -1.0f, NULL, true));
}

void PLAYER::AddParticleWithBlackhole(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY)
{
	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		return;
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSeqeunce);
	m_ParticleMsg.push_back(MSG_PARTICE_CREATE_INFO(tempParticle.c_str(), 0.0f, 0.0f, iParticleNum, fOffsetX, true, -1.0f, 0, false, true));
}

void PLAYER::AddParticleNoDirection(char* strSeqeunce, int iParticleNum, float fOffsetX)
{
	AddParticleNoDirectionPos(strSeqeunce, iParticleNum, fOffsetX, 0.0f);
}

void PLAYER::AddParticleNoDirectionPos(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY)
{
	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		return;
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSeqeunce);
	m_ParticleMsg.push_back(MSG_PARTICE_CREATE_INFO(tempParticle.c_str(), fOffsetX, fOffsetY, iParticleNum, 0.0f, false));
}

void PLAYER::AddParticleNoDirectionWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fTraceTime, int iTracePos)
{
	AddParticleNoDirectionPosWithTrace(strSeqeunce, iParticleNum, 0.0f, 0.0f, fTraceTime, iTracePos);
}

void PLAYER::AddParticleNoDirectionPosWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY, float fTraceTime, int iTracePos)
{
	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		return;
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSeqeunce);
	m_ParticleMsg.push_back(MSG_PARTICE_CREATE_INFO(tempParticle.c_str(), fOffsetX, fOffsetY, iParticleNum, 0.0f, false, fTraceTime, ComputePosDesc(m_posDesc, iTracePos)));
}

void PLAYER::AddParticleWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fTraceTime, int iTracePos)
{
	AddParticlePosWithTrace(strSeqeunce, iParticleNum, fOffsetX, 0.0f, 0.0f, fTraceTime, iTracePos);
}

void PLAYER::AddParticlePosWithTrace(char* strSeqeunce, int iParticleNum, float fXDir, float fOffsetX, float fOffsetY, float fTraceTime, int iTracePos)
{
	if (g_kGlobalValue.IsSkillEffectDisabled(m_iPlayerIndex))
		return;
	std::string tempParticle = g_pParticleChangeSys->GetParticleChageString(m_iPlayerIndex, strSeqeunce);
	m_ParticleMsg.push_back(MSG_PARTICE_CREATE_INFO(tempParticle.c_str(), fOffsetX, fOffsetY, iParticleNum, fXDir, true, fTraceTime, ComputePosDesc(m_posDesc, iTracePos)));
}

void PLAYER::ClearItemRemainTime()
{
	for (int i = 0; i < GC_GAME_ITEM_NUM; ++i)
		Item_Remain_Time[i] = 0;
}

void PLAYER::LoadResource()
{
	UpdatePlayerResource();
	UpdatePlayerResourceOnGame();
	LoadSlash();
	EffectClear();
	ClearMotionParticleList();
	EffectInit();

	DWORD dwMotionLoadFlag;
	switch (SiKGCRoomManager()->GetGameModeCategory())
	{
	case GC_GMC_AGIT:
		dwMotionLoadFlag = LOADMOTION_STANDARD;
		break;
	default:
		dwMotionLoadFlag = LOADMOTION_ALL;
		break;
	}
	g_pGraphicsHelper->LoadPlayerActionMotionByPlayerIndex(m_iPlayerIndex, (char)Extra_Char_Num, dwMotionLoadFlag);

#ifdef WEAPONCHANGE
	// 스왑할 무기가 있는지 확인
	if (m_mapSwapWeapon.end() != m_mapSwapWeapon.find(GetCurrentChar().iCharType))
	{
		KItem* pKItem = NULL;
		GCItem* pGCItem = NULL;
		if (g_MyD3D->Get_MyPlayer() == m_iPlayerIndex)
		{
			if ((pKItem = g_pItemMgr->m_kInventory.FindItemByItemUID(m_mapSwapWeapon[GetCurrentChar().iCharType].second.second)))
			{
				if ((pGCItem = g_pItemMgr->GetItemData(pKItem->m_ItemID)))
				{
					g_pGraphicsHelper->LoadPlayerActionMotionByPlayerIndex(m_iPlayerIndex, (char)GetCurrentChar().iCharType + (GC_CHAR_NUM * pGCItem->iNeedChangeJobLevelForEquippingItem), dwMotionLoadFlag);
				}
				else
					m_mapSwapWeapon.erase(GetCurrentChar().iCharType);
			}
			else
				m_mapSwapWeapon.erase(GetCurrentChar().iCharType);
		}
		else
		{
			pGCItem = g_pItemMgr->GetItemData(m_mapSwapWeapon[GetCurrentChar().iCharType].second.first / 10);
			if (pGCItem && 0 != pGCItem->dwGoodsID)
				g_pGraphicsHelper->LoadPlayerActionMotionByPlayerIndex(m_iPlayerIndex, (char)GetCurrentChar().iCharType + (GC_CHAR_NUM * pGCItem->iNeedChangeJobLevelForEquippingItem), dwMotionLoadFlag);
		}
	}
#endif

	// 변신 데이터를 미리 로딩
	for (int k = 0; k < FORM_NUM; ++k)
	{
		if (m_FormTemplate[k].sMetamorphosisTemplate.iFormID != FORM_NORMAL)
		{
			UpdateFormResource(k /*m_vecFormItemModel*/);
			//CreateFormShadow( GetCurFormTemplate().sMetamorphosisTemplate.iFormID );
		}
	}

	LoadPet();
	m_pObject = GetCurMetaForm();

	// 게임 로딩 타이밍에 리소스 로딩이 일어나므로 샵모션 같은거 바꾸려면 힘들듯..
	SiGCSKT()->LoadingResource(m_kUserInfo.dwUID);

	// 엑스트라 메시로드
	g_kGlobalValue.m_ExtraMesh.LoadExtraMesh(m_pObject, m_iPlayerIndex);
}

// 여기에 필요한거 넣으심 됩니다.
void PLAYER::InitPlayerValueforTagArena(bool bLoad)
{
	SetEPlayerState(PS_ALIVE); //태어난다.
	g_MyD3D->m_akPet[(int)m_iPlayerIndex]->SetPetMotion(EGCPETMOTION::INVALID);
	m_bThrowMatchGP = false;
	Is_Aleady_Scream = false;
	Last_Gravity = 0;
	m_bCrashCheck = true;
	Body_Angle = 0;
	pLatency = LATENCY;
	m_bRender = true;
	Silk = 0;
	Gravity = 100.0f;
	Shield = 0;
	cFrame = 0;
	Old_Frame = 0;
	Old_IsContact = false;
	IsContact = false;
	NoCheckContact = 20;
	iContact_to_Stone = -1;
	m_fO2 = 35.0f;
	m_bIsOnceHeal = false;
	Reserve_Motion = 0;
	ClearGameItem();
	ClearItemRemainTime();
	m_bIncHP = true;
	bIsRight = true;
	x_Speed = 0.0f;
	Slow_Count = 0;
	m_fGetExpGpRatio = 0.0f;
	m_iJumpActionLimitCount = 0;
	m_iSummoning = 0;
	m_bJumpZoomed = false;
	m_bJumpDashed = false;
	Next_Motion = -1;
	Next_Direction = -1;
	m_iJumpPossibleFrame = 0;
	m_iInvEffFrame = 0;
	m_nForceNum = 0;
	InitBurnningGauge();

	// 2008/04/09 Jaeho
	// 현재 이 함수는 태그매치에서만 사용합니다.
	// 태그매치는 아레나로 떨어질 경우에 변신체 상태일 경우가 있습니다. (바뀔 녀석이 변신체인 경우...)
	// 그래서 이부분은 주석처리를 합니다...
	//m_bSuperArmor = false;
	//for(int i=0;i<FORM_NUM;++i)
	//    GetMetaFormBuffTimeIdx(i) = -1;

	m_bNormalForm = false;

	// bLoad == false 이면 피를 초기화 해줘야한다.
	// ( Arena 시 케릭터가 같다 -> 실제로 목숨을 깐 아레나이기 때문
	if (!bLoad)
	{
		SetHPFull();

		if (IsLocalPlayer() == true)
		{
			SetStartMana();
		}
	}

	if (IsLocalPlayer())
	{
		for (int ii = 0; ii < LATENCY; ++ii)
		{
			L_HP[ii] = 0;
			L_Shield[ii] = 0;
			L_IsRight[ii] = bIsRight;
			L_x[ii] = vPos.x;
			L_y[ii] = vPos.y;
			L_Frame[ii] = cFrame;
			L_Now_Motion[ii] = uiMotion;
		}

		All_Latency_Equal();

		//         g_MyD3D->MyHead->Now_HP[m_iPlayerIndex] = GetHP();
	}
}

void PLAYER::CreatePlayer(bool bLoadResource, bool bArena)
{
	if (g_kGlobalValue.IsTutorialMode()) {
		m_iSkillType = 0;
	}

	m_bDirectKill = false;
	m_iShieldTexNum = -1;
	m_bShieldTypeCounting = false;
	m_mapPrevHotKeyCoolTime.clear();
	m_iDefaultFaceLiftType = FACE_LIFT_NORMAL;
	m_mapFaceLiftDelayInfo.clear();
	m_prReflectionDamage = std::make_pair(-1, 0.f);
	m_sRangeCheckInfo.Init();
	m_mapHeroItemCondition.clear();
	if (bLoadResource == true)
	{
		LoadResource();
	}

	if (IsLocalPlayer() == true)
	{
		SetStartMana();
	}

	SetEPlayerState(PS_ALIVE); //태어난다.
	m_pObject->ResetMeshState();
	m_pObject->SetRender(true);
	m_pObject->SetCartoon(CARTOON_DEFAULT);
	if (g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject)
	{
		g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject->ResetMeshState();
		g_MyD3D->m_akPet[m_iPlayerIndex]->m_pObject->SetRender(true);
	}

	g_MyD3D->m_akPet[(int)m_iPlayerIndex]->SetPetMotion(EGCPETMOTION::INVALID);
	m_bThrowMatchGP = false;
	Is_Aleady_Scream = false;
	Last_Gravity = 0;
	m_bCrashCheck = true;
	Body_Angle = 0;
	pLatency = LATENCY;
	m_bRender = true;
	//m_bIsRenderBaseMesh = true;
	//m_bIsRenderBaseMeshStatic = false;
	Silk = 0;
	Gravity = 100.0f;
	SetHPFull();
	Shield = 0;
	cFrame = 0;
	Old_Frame = 0;
	Old_IsContact = false;
	IsContact = false;
	NoCheckContact = 20;
	iContact_to_Stone = -1;
	m_fO2 = 35.0f;
	m_bIsOnceHeal = false;
	Reserve_Motion = 0;
	m_bResultTower = false;
	m_iComboSkillHotKeyIdx = -1;
	m_iComboSkillComboIdx = -1;
	m_iComboSkillNowComboIdx = -1;
	m_iDamageSummonMonSlot = -1;
	m_bNotCheckSideFH = false;
	m_bIsAttackMiss = false;

	m_iSkipFrame = 0;
	m_iResetSkipFrame = 0;
	m_iContinueSkipFrame = 0;
	m_vecPermissionDamage.clear();
	m_bBuffFrameLock = false;
	m_iEmbarkTypeOfInPortal = INT_MAX;
	m_dwEmbarkUIDOfInPortal = INT_MAX;
	m_vDamageStartPos = D3DXVECTOR2(0.0f, 0.0f);
	m_vDamageWhoPos = D3DXVECTOR2(0.0f, 0.0f);

	if ((true == bLoadResource) ||
		(GC_GMC_DOTA != SiKGCRoomManager()->GetGameModeCategory()))
	{
		ClearGameItem();
		ClearItemRemainTime();
	}

	m_bIncHP = true;
	bIsRight = true;
	x_Speed = 0.0f;
	Slow_Count = 0;
	m_fGetExpGpRatio = 0.0f;
	m_iJumpActionLimitCount = 0;
	m_iSummoning = 0;
	m_bJumpZoomed = false;
	m_bJumpDashed = false;
	Next_Motion = -1;
	Next_Direction = -1;
	m_iJumpPossibleFrame = 0;
	m_iInvEffFrame = 0;
	m_bSuperArmor = false;
	m_iWeaponType = 0;
	for (int i = 0; i < FORM_NUM; ++i)
		GetMetaFormBuffTimeIdx(i) = -1;

	m_bNormalForm = false;

	m_eAttackType = ATTACKTYPE_NORMAL;
	m_eAttackDirection = ATTACKDIR_MIDDLE;
	m_fPushTargetX = 0.0f;
	m_fPushTargetY = 0.0f;
	m_bSpecialAttack = false;
	m_bIsAgitBreaking = false;
	m_bUseNotAP = false;
	m_uiNotSpecialSuper = 0;
	m_dwMyEmbarkUID = UINT_MAX;
	m_iEmbarkAdaptBoneID = -1;
	m_dwEmbarkSendCool = 0;

	m_bMPRecoveryFix = false;
	m_fMPRecoveryFix = 0.0f;
	m_fMinusAPCoolTime = 0.0f;
	m_fMPDecreaseRatioForSpecial = 1.0f;

	m_iHittedPrevDamage = -1;
	m_iPrevDamageHitWho = -1;
	m_bNotTargeting = false;

	if (IsLocalPlayer())
	{
		for (int ii = 0; ii < LATENCY; ++ii)
		{
			L_HP[ii] = 0;
			L_Shield[ii] = 0;
			L_IsRight[ii] = bIsRight;
			L_x[ii] = vPos.x;
			L_y[ii] = vPos.y;
			L_Frame[ii] = cFrame;
			L_Now_Motion[ii] = uiMotion;
		}

		All_Latency_Equal();

		//         g_MyD3D->MyHead->Now_HP[m_iPlayerIndex] = GetHP();

		if ((SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH) && SiKGCEventManager()->IsActiveEvent(EVENT_ID_FATAL_MATCH))
		{
			// 			g_MyD3D->MyHead->Now_HP[m_iPlayerIndex] = 0;
			SetHP(0.f);
		}
	}

	g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, !bLoadResource);

	if (bLoadResource)
	{
		m_fIncHPRatio = 1.0f;
	}

	m_iCannotCatchFrame = 0;
	m_bDamageSetNoCheckContact = false;
	ContactBlock = 0;
	m_nForceNum = 0;
	m_cUseShield = CID_MAX;

	m_iArenaCoolTime = 0;

	if (m_bIsBurnningMode)
		ChangeBurnningTemplate();

	InitBurnningGauge();
	InitQuantityPoint();
	InitMarbleInfo();
	InitCurEquippedMarbleChargeMotion();
	InitCurEquippedBurnChargeMotion();
	InitCombinationSkill();
	InitCurEquippedBloodChargeMotion();

	//Guild Mark
	KInDoorUserInfo kIndoorUserInfo;
	m_kUserInfo.SetIndoorUserInfo(&kIndoorUserInfo);

	if (kIndoorUserInfo.m_dwGuildUID != 0)
	{
		m_pGuildMarkTexture = SiKGCIconImageManager()->GetTexture(kIndoorUserInfo.m_strMarkName);

		if (m_pGuildMarkTexture == NULL)
		{
			SiKGCIconImageManager()->LoadIconImageFromHTTP(kIndoorUserInfo.m_strMarkName);
			m_pGuildMarkTexture = SiKGCIconImageManager()->GetTexture(kIndoorUserInfo.m_strMarkName);
		}
	}
	else
	{
		m_strGuildMarkName = L"";
	}

	m_bHyperArmor = false;
	m_bStartShield = false;

	m_bIsEquipCoordiSetItem = g_pItemMgr->GetEquipCoordiSetItem(m_kCoordiSetTemplet, this);

	if (bLoadResource)
	{
		if (!SiKGCWorldMapManager()->IsSkipStartMotion(SiKGCRoomManager()->GetGameMode()))
		{
			SetPlayerState(GetCurFormTemplate().START);

			// 시작모션일때 무적으로 ㅡ ㅎ
			if (GC_GMC_TUTORIAL != SiKGCRoomManager()->GetGameModeCategory())
			{
				Super = 255;
			}
		}
	}

	m_bIsEquipCoordiSetItem = g_pItemMgr->GetEquipCoordiSetItem(m_kCoordiSetTemplet, this);

	if (GC_GMC_TAG_MATCH != SiKGCRoomManager()->GetGameModeCategory())
	{
		// AP캐릭터 콤보스킬 갱신
		if (IsLocalPlayer())
		{
			if (g_pkUIScene->m_pkGameOverlayUI)
				g_pkUIScene->m_pkGameOverlayUI->ResetComboSkill();
		}
	}

	m_iCurQuickSlotType = KSkillSlot::INDEX_A;

	m_iChangeFrame = 0;

	m_bNoSafePosition = false;

#if !defined(__PATH__)
	g_kGlobalValue.InitDamageCheckMotionInfo();
#endif

	KItemIDVector vecItemSorted = GetCurrentChar().GetEquipmentForLoadModel(m_iPlayerIndex);
	for (KItemIDVector::const_iterator vit = vecItemSorted.begin(); vit != vecItemSorted.end(); ++vit) {
		GCItem* pItem = g_pItemMgr->GetItemData(*vit);
		if (NULL == pItem) {
			continue;
		}

		// 빅헤드 관련 처리
		if ((pItem->dwSlotPosition & ESP_A_DOWN_HELMET) && (pItem->dwSlotPosition & ESP_A_UPPER_HELMET)) {
			m_iDefaultFaceLiftType = FACE_LIFT_BIG_HEAD;
			break;
		}
	}

	m_bNoCrashCheckByBuff = false;
	m_bRenderChildMeshParticle = true;
	m_bSavingMotion = false;
}

void PLAYER::Calculate_Arena_Pos()
{
	// 초기 위치를 정한다.
	//if ( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
	//Jaeho.Ready // Vec = g_MyD3D->m_pkQuestGameMgr->GetArenaPos();

	D3DXVECTOR2 Vec;
	if (SiKGCRoomManager()->IsMonsterGameMode() == false)
	{
		Vec = g_MyD3D->MyStg->Get_Random_XY();

		if (g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS)
		{
			if (vPos.x < 0.3f)
			{
				vPos.x = 0.3f; // 나무 피하기
			}
			vPos.y = g_MyD3D->MyStg->m_fScrollY - 0.75f;
			y_Speed = 0.07f;
			Super = 80;
		}
		else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG
			|| SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN)
		{
			vPos.x = g_MyD3D->MyStg->GetXLimit() / 2.0f;
			vPos.y = Vec.y + 0.7f;
			y_Speed = -.03f;
		}
		else
		{
			vPos.x = Vec.x;
			vPos.y = Vec.y + 1.0f;
			y_Speed = -.03f;
		}
	}
	else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MINIGAME)
	{
		// milennium9 test code
		vPos.x = 0.5;
		vPos.y = 0.5;
		y_Speed = -0.03f;
	}
	//dragon 몬스터 원정대에서 죽었을시 혹은 시작시에 엉뚱한 위치에서 나오는 문제.
	else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER)
	{
		vPos.x = 0.0;
		vPos.y = 0.0;
		y_Speed = -0.03f;
	}
	// 점령전 아레나
	else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA)
	{
		if (IsLocalPlayer())
		{
			vPos.x = m_vDotaArenaPos.x;
			vPos.y = m_vDotaArenaPos.y;
			y_Speed = -.03f;
			g_kCamera.SetDirectCameraPosition(vPos.x, vPos.y, KGCCameraOrtho::POS_MODE_DIRECT);

			SDotaTemplate* pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
			if (pDotaTemplate)
			{
				// 아레나 존위치는 세르딘이면 젤 첫번째 존이다 카나반이면 젤 마지막 존이다
				SetDotaStage((TEAM_SERDIN == m_kUserInfo.iTeam) ? 1 : pDotaTemplate->iNumZoneMax);
				g_pkUIScene->m_pkGameOverlayUI->UpdateDotaMiniMap();

				// 내 위치를 적에게 알리자!!(우리편도)
				KGC_PID_BROAD_DOTA_ZONE_POSITION kPacket(GC_PID_BROAD_DOTA_ZONE_POSITION,
					m_kUserInfo.dwUID, m_iPlayerIndex, GetDotaStage(), false, D3DXVECTOR2(vPos.x, vPos.y));
				SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE);
				KGCPC_MEMO("GC_PID_BROAD_DOTA_ZONE_POSITION");
			}
		}
	}
	// 아지트 아레나
	else if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT)
	{
		Vec = g_MyD3D->m_pkQuestGameMgr->GetArenaPos();
		vPos.x = Vec.x;
		vPos.y = Vec.y + 0.1f;
		y_Speed = -.03f;
	}
	// 파성
	else if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST46) {
		if (g_MyD3D->GetActiveStage() == 4) {
			vPos.x = 3.0f;
			vPos.y = 2.2f;
			y_Speed = -.03f;
		}
		else {
			D3DXVECTOR2 vSafePos = g_MyD3D->MyStg->Check_SafePostionX(vPos.x, vPos.y);
			SiKGCFantasticMap()->CheckPierceBlockXPos(vPos.x, vSafePos.x, vPos.y);
			for (int i = 0; i < LATENCY + SHOWLATENCY; ++i)
			{
				g_MyD3D->MyPlayer[m_iPlayerIndex]->L_x[i] = vSafePos.x;
				g_MyD3D->MyPlayer[m_iPlayerIndex]->L_y[i] = vSafePos.y;
			}
			vPos.x = vSafePos.x;
			vPos.y = vSafePos.y;
		}
	}
	else // 일반적인 게임 모드일때에는 랜덤을 쓰지 않고 시작위치에서 부활한다.
	{
		Vec = g_MyD3D->m_pkQuestGameMgr->GetArenaPos();
		vPos.x = Vec.x;
		vPos.y = Vec.y + 0.7f;
		y_Speed = -.03f;
	}
}

void PLAYER::SetDotaArenaPos(float fX, float fY)
{
	m_vDotaArenaPos.x = fX;
	m_vDotaArenaPos.y = fY;
}

bool PLAYER::FindSlot(int iChar)
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_kUserInfo.aiTagSlot[i] == iChar)
			return true;
	}

	return false;
}

void PLAYER::SetSlotAlign()
{
	// Slot 정렬
	std::vector< int > vecExist; //선택된 캐릭터
	std::vector< int > vecNotExist; //선택되지 않은 캐릭터

	for (int i = 0; i < (int)m_kUserInfo.vecCharInfo.size(); ++i)
	{
		if (FindSlot(i) == true)
			vecExist.push_back(i);
		else
			vecNotExist.push_back(i);
	}

	for (int i = 0; i < 3; ++i)
	{
		if (m_kUserInfo.aiTagSlot[i] == -1)
		{
			m_kUserInfo.aiTagSlot[i] = vecNotExist[0];
			vecNotExist.erase(vecNotExist.begin());
		}
	}

	//if( IsLocalPlayer() == true )
	//{
	//    memcpy( g_kGlobalValue.m_kUserInfo.aiTagSlot ,m_kUserInfo.aiTagSlot, sizeof(g_kGlobalValue.m_kUserInfo.aiTagSlot[0])*3 );
	//    g_pIndigoSelect->UpdateSlot();
	//}
}

bool PLAYER::IgnoreStop_MotionList(int iPlayerIndex)
{
	// Stop() 함수 안에서 Slow_Count를 적용시키는 부분이 있는데..
	// 여기서 멈추는걸 무시할 모션들을 넣어서 제어해 준다
	if (g_MyD3D->MyPlayer[iPlayerIndex]->uiMotion == g_MyD3D->MyPlayer[iPlayerIndex]->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP))
		return true;

	// 마검술 3단필은 중간에 스탑걸리지 않는다!
	switch (g_MyD3D->MyPlayer[iPlayerIndex]->uiMotion)
	{
	case MID_RONAN_SWORD_SPECIAL3_ATK:
	case MID_RONAN4_WHITE_MAGIC_SPECIAL3:
	case MID_SKILL_LIRE_SUMMON_ARCHER_LV1_START:
	case MID_SKILL_LIRE_SUMMON_ARCHER_LV2:
	case MID_SKILL_LAS_VENOM_SLICE_START:
	case MID_SKILL_LAS_VENOM_SLICE_ATK:
	case MID_SIEG1_SPECIAL2:
	case MID_SIEG1_SPECIAL2_F:
	case MID_SKILL_RYAN_EXT_SPECIAL1_1:
	case MID_SKILL_RYAN_SPECIAL2_LV3_2:
	case MID_SKILL_RYAN_PHOENIX_ASSULT:
	case MID_SKILL_RYAN_NATURESHIELD:
	case MID_SKILL_RYAN_NATURESHIELD_STANDUP:
	case MID_AMY4_SPECIAL2:
		return true;
	}

	return false;
}

void PLAYER::BackAttack(void)
{
	if (timeGetTime() - m_dwBackAttackTime > BACK_ATTACK_TERM_TIME)
	{
		m_dwBackAttackTime = timeGetTime();
		g_MyD3D->m_pDamageManager->Add(DT_BACKATTACK, m_iPlayerIndex);
		g_pkQuestManager->SetAttackTypeState(0);
	}
}

void PLAYER::AerialAttack(void)
{
	if (timeGetTime() - m_dwAerialAttackTime > AERIAL_TERM_TIME)
	{
		m_dwAerialAttackTime = timeGetTime();
		g_MyD3D->m_pDamageManager->Add(DT_AERIAL, m_iPlayerIndex);
		g_pkQuestManager->SetAttackTypeState(1);
	}
}

// std::for_each( tempTemplate.setMeshAnimation.begin(), tempTemplate.setMeshAnimation.end(),
//	boost::bind(&std::set<int>::insert, boost::ref(setLoadList), _1) );
#define KOGEACHINSERT(BEGIN,END,SET) for (auto jSET = BEGIN; jSET != END; ++jSET) { SET.insert(*jSET); }

void PLAYER::MakeFireInfo3DObjectLoadList()
{
	if (!m_kUserInfo.bLive)
		return;

	std::set<int> setLoadList;
	if (g_MyD3D->MyReplay->m_iCurrState == RP_PREPARE)
	{
		// [8/5/2007 breadceo] 이미 Replay의 Head를 읽은 상태이므로 무슨 케릭터가 나올지 안다.
		for (int i = 0; i < 3; ++i)
		{
			if (m_kUserInfo.aiTagSlot[i] != -1)
				GetFireInfo3DObjectLoadListfromExtraCharNum(GetExtraCharNum(m_kUserInfo.aiTagSlot[i]), setLoadList);
		}

		if (m_kUserInfo.bLive == true)
		{
			GetFireInfo3DObjectLoadListfromExtraCharNum(Extra_Char_Num, setLoadList);
		}

		for (std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit)
		{
			GetFireInfo3DObjectLoadListfromExtraCharNum(GetExtraCharNum(mit->second.Extra_Char_Num), setLoadList);
		}
	}
	// [8/5/2007 breadceo] 리플레이를 제외한 기타 모드들
	else
	{
		GetFireInfo3DObjectLoadListfromExtraCharNum(Extra_Char_Num, setLoadList);

		for (std::map< int, TagCharacterInfo >::iterator mit = m_mapTagCharInfo.begin(); mit != m_mapTagCharInfo.end(); ++mit)
		{
			GetFireInfo3DObjectLoadListfromExtraCharNum(GetExtraCharNum(mit->second.Extra_Char_Num), setLoadList);
		}

		if (SiKGCRoomManager()->GetGameMode() == GC_GM_INDIGO_TEAM)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (m_kUserInfo.aiTagSlot[i] != -1)
					GetFireInfo3DObjectLoadListfromExtraCharNum(GetExtraCharNum(m_kUserInfo.aiTagSlot[i]), setLoadList);
			}
		}
	}

	std::for_each(setLoadList.begin(), setLoadList.end(),
		boost::bind(&CDamageManager::CreateMeshAnimationForPrevLoad, boost::ref(g_MyD3D->m_pDamageManager), _1, m_iPlayerIndex));
}

void PLAYER::GetFireInfo3DObjectLoadListfromExtraCharNum(IN char cCharNum, OUT std::set<int>& setLoadList)
{
	PLAYER_TEMPLATE tempTemplate;
	tempTemplate.Init();
	SetPlayerTemplate(m_setSkill, cCharNum, tempTemplate);

	KSafeArray<PLAYER_TEMPLATE, FORM_NUM> tempFormTemplate;
	for (int i = 0; i < FORM_NUM; ++i)
		tempFormTemplate[i].Init();
	SetFormTemplate(cCharNum, tempFormTemplate);

	PLAYER_TEMPLATE tempSwapTemplate;
	SetSwapWeaponTemplate(cCharNum, tempSwapTemplate);

	if (tempTemplate.setMeshAnimation.empty())
		return;

	/*std::for_each( tempTemplate.setMeshAnimation.begin(), tempTemplate.setMeshAnimation.end(),
		boost::bind( &std::set<int>::insert, boost::ref(setLoadList), _1 ) );*/
	KOGEACHINSERT(tempTemplate.setMeshAnimation.begin(), tempTemplate.setMeshAnimation.end(), setLoadList);

	for (int i = 0; i < FORM_NUM; ++i)
	{
		if (tempFormTemplate[i].setMeshAnimation.empty())
			continue;

		/*std::for_each( tempFormTemplate[i].setMeshAnimation.begin(), tempFormTemplate[i].setMeshAnimation.end(),
			boost::bind( &std::set<int>::insert, boost::ref(setLoadList), _1 ) );*/
		KOGEACHINSERT(tempFormTemplate[i].setMeshAnimation.begin(), tempFormTemplate[i].setMeshAnimation.end(), setLoadList);
	}

	if (tempSwapTemplate.setMeshAnimation.empty())
		return;

	//std::for_each( tempSwapTemplate.setMeshAnimation.begin(), tempSwapTemplate.setMeshAnimation.end(),
		//boost::bind( &std::set<int>::insert, boost::ref(setLoadList), _1 ) );
	KOGEACHINSERT(tempSwapTemplate.setMeshAnimation.begin(), tempSwapTemplate.setMeshAnimation.end(), setLoadList);
}

bool PLAYER::IsPlusMotionX(UINT uiMotion)
{
	return false;
}

void PLAYER::ProcessSwapWeapon( bool bNotChangeMotion )
{
	//20091102 이동석
	if( uiMotion == MID_AMY1_DANCE_JOKE || uiMotion == MID_AMY1_JOKE ||
		 uiMotion == MID_AMY2_JOKE || uiMotion == MID_AMY2_PERFORMENCE_JOKE ||
		  uiMotion == MID_AMY3_JOKE || uiMotion == MID_AMY3_DANCE_JOKE ||
		   uiMotion == MID_AMY4_JOKE || uiMotion == MID_AMY4_MELO_JOKE 
		)
		return;
   if( IsDetermineWin || MetamorphosisForm == FORM_WOLF)
        return;
   //20091102 이동석
   g_KDSound.Play( "673" );

	// 변신하기전에 burnning template 정리해주쟈! 항상 제자리에있어야 한다!
	bool bBurning = false;
	if ( m_bIsBurnningMode )
	{
        swap( GetCurFormTemplate(),GetCurBurnningTemplate() );

		if( !m_bIsBurnningMode )
		{
			g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, EGC_SIEG_FURY );
			g_pMagicEffect->ClearMagicEffect( m_iPlayerIndex, EGC_JIN_BURNNING_MODE_EFFECT );
			g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_MAGICSWORD_MODE);
			g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN2_SPEAR_MODE);
			g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN3_SHIELD_MODE);
			g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN4_BURNING_MODE);
		}
		bBurning = true;
	}

    m_iWeaponType = !m_iWeaponType;

    ChangeWeaponTemplate();

	// 변신 후 버닝 모드이면, 다시 버닝 모드로 바꾸어 주쟈;; 
	if ( m_bIsBurnningMode )
	{
        m_bIsBurnningMode = true;		
        swap( GetCurFormTemplate(),GetCurBurnningTemplate() );

		LoadSlash();
	}

    bool bLatencyEqual = true;
    UINT uiChangeMotion = GetSwapWeaponMotion( Extra_Char_Num, &bLatencyEqual );
    ASSERT( uiChangeMotion != 0 );

    if( false == bNotChangeMotion )
    {
        SetPlayerState( uiChangeMotion );
    }
    SyncWeaponSwap( uiChangeMotion, bLatencyEqual, bNotChangeMotion );

    if( IsLocalPlayer() && ( m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_AMY || Extra_Char_Num == CID_RYAN4 ) ) {
        g_MyD3D->MyHead->IsBlackMagic = !g_MyD3D->MyHead->IsBlackMagic;
        SwapSlotSkillType( !g_MyD3D->MyHead->IsBlackMagic );
    }
}

UINT PLAYER::GetSwapWeaponMotion( UINT iType, bool *bLatencyEqual )
{
    UINT uiChangeMotion = 0;
    *bLatencyEqual = true;

    switch( iType )
    {
        // for_newchar_amy
    case CID_AMY1:
        // 필살기 중 혹은 조크 중 일때는 전환 모션을 하지 않는다.
        if ( IsSpecialSkillMotion() 
            || IsSwapSpecialSkillMotion()
            || uiMotion == MID_AMY1_DANCE_SPECIAL2_LEFT 
            || uiMotion == MID_AMY1_DANCE_SPECIAL3_LEFT 
            || uiMotion == MID_AMY1_JOKE
            || uiMotion == MID_AMY1_DANCE_JOKE 
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION_LEFT
            )
        {
            uiChangeMotion = uiMotion;
        }
        else
        {
            if(GetWaitMotion() == MID_AMY1_WAIT )
            {
                uiChangeMotion = MID_AMY1_SWAP_TO_FIGHT_MODE;
            }
            else if(GetWaitMotion() == MID_AMY1_DANCE_WAIT )
            {
                uiChangeMotion = MID_AMY1_SWAP_TO_DANCE_MODE;
            }
            cFrame = 0;
        }
        *bLatencyEqual = false;

        break;
        //dolki
    case CID_AMY2:
        if ( IsSpecialSkillMotion() 
            || IsSwapSpecialSkillMotion()            
            || uiMotion == MID_AMY2_PERFORMENCE_SPECIAL2_READY 
            || uiMotion == MID_AMY2_PERFORMENCE_SPECIAL3_READY 			
			|| uiMotion == MID_AMY2_PERFORMENCE_SPECIAL3_FORWORD 
			|| uiMotion == MID_AMY2_PERFORMENCE_SPECIAL3_BACK 
            || uiMotion == MID_AMY2_JOKE
            || uiMotion == MID_AMY2_PERFORMENCE_JOKE 
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION_LEFT
            )
        {
            uiChangeMotion = uiMotion;
        }
        else
        {
            if(GetWaitMotion() == MID_AMY2_WAIT )
            {
                uiChangeMotion = MID_AMY2_SWAP_TO_FIGHT_MODE;
            }
            else if(GetWaitMotion() == MID_AMY2_PERFORMENCE_WAIT )
            {
                uiChangeMotion = MID_AMY2_SWAP_TO_PERFORMENCE_MODE;
            }
            cFrame = 0;
        }
        *bLatencyEqual = false;
        break;
	case CID_AMY3: //에이미 2차전직
		if ( IsSpecialSkillMotion() 
			|| IsSwapSpecialSkillMotion()            
			|| uiMotion == MID_AMY3_SPECIAL3_LEFT
			|| uiMotion == MID_AMY3_DANCE_SPECIAL3_LEFT
			|| uiMotion == MID_AMY3_JOKE
			|| uiMotion == MID_AMY3_DANCE_JOKE 
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION_LEFT
			)
		{
			uiChangeMotion = uiMotion;
		}
		else
		{

			if(GetWaitMotion() ==  MID_AMY3_WAIT)
			{
				uiChangeMotion = MID_AMY3_SWAP_TO_FIGHT_MODE;
			}
			else if(GetWaitMotion() == MID_AMY3_DANCE_WAIT )
			{
				uiChangeMotion = MID_AMY3_SWAP_TO_DANCE_MODE;
			}
			cFrame = 0;
		}
		*bLatencyEqual = false;
		break;
    case CID_AMY4: //에이미 2차전직
        if ( IsSpecialSkillMotion() 
            || IsSwapSpecialSkillMotion()            
            || uiMotion == MID_AMY3_SPECIAL3_LEFT
            || uiMotion == MID_AMY3_DANCE_SPECIAL3_LEFT
            || uiMotion == MID_AMY3_JOKE
            || uiMotion == MID_AMY3_DANCE_JOKE 
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION
            || uiMotion == MID_AMY_WAIT_EXTRAMOTION_LEFT
#if !defined(NATION_KOREA) && !defined(NATION_IDN)
            || uiMotion == MID_AMY4_UNIQUE_SKILL1
#endif
            )
        {
            uiChangeMotion = uiMotion;
        }
        else
        {

            if(GetWaitMotion() == MID_AMY4_WAIT)
            {
                uiChangeMotion = MID_AMY4_SWAP_TO_FIGHT_MODE;
            }
            else if(GetWaitMotion() == MID_AMY4_MELO_WAIT )
            {
                uiChangeMotion = MID_AMY4_SWAP_TO_MELO_MODE;
            }
            cFrame = 0;
        }
        *bLatencyEqual = false;
        break;
    case CID_RYAN4:
        if( uiMotion == MID_RYAN4_SWAP_TO_TWO_HAND )
        {
            uiChangeMotion = MID_RYAN4_WAIT;
        }
        else if( uiMotion == MID_RYAN4_SWAP_TO_ONE_HAND )
        {
            uiChangeMotion = MID_RYAN4_ONE_HAND_WAIT;
        }
        break;

	case CID_JIN3:
		if( uiMotion == MID_JIN2_2_WAIT || uiMotion == MID_JIN2_2_WAIT_BURNNING_MODE )
		{
			uiChangeMotion = MID_JIN2_2_TO_JIN2_WAIT;
		}
		else if( uiMotion == MID_JIN2_THROWWEAPON )
		{
			if ( g_MyD3D->MyPlayer[m_iPlayerIndex]->m_iBranchSkill == SKILL_BRANCH_NUM )
			{
				uiChangeMotion = MID_JIN2_AFTERTHROW_ATK;
			}
			else
			{
				uiChangeMotion = MID_JIN2_2_WAIT;
			}

			cFrame = 0;
		}
		break;

    default:
        uiChangeMotion = 0;
        break;
    }


    return uiChangeMotion;
}

void PLAYER::SyncWeaponSwap( UINT uiChangeMotion_, bool bLatencyEqual/* = true*/, bool bNotChangeMotion/* = false */ )
{
    if( g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
        return;

    if( IsLocalPlayer() )
    {
        if( bLatencyEqual )
            All_Latency_Equal();
        pLatency = LATENCY;
        g_kSwapWeapon->ucPlayerIndex = (UCHAR)g_MyD3D->Get_MyPlayer();
        g_kSwapWeapon->ucWeaponType = m_iWeaponType;
        g_kSwapWeapon->bNotChangeMotion = bNotChangeMotion;
    }
    else
    {
        if( bLatencyEqual )
        {
            for ( int i = 0; i < LATENCY; i++ )
            {
                L_Now_Motion[i] = uiChangeMotion_;
                L_Frame[i]      = 0;
            }
        }
    }
}

void PLAYER::SetBurnningTemplate(IN char CharNum, OUT KSafeArray<PLAYER_TEMPLATE, BTYPE_NUM>& m_BurnningTemplate)
{
	int iChar = CharNum % GC_CHAR_NUM;
	int iJob = CharNum / GC_CHAR_NUM;

	for (int i = 0; i < BTYPE_NUM; i++)
	{
		char strFileName[MAX_PATH];
		sprintf(strFileName, "PlayerTemplate%d_%d_burning_%d.lua", iChar, iJob, i);

		KGCPlayerCommon::LoadPlayerTemplate(m_setSkill, strFileName, m_BurnningTemplate[i], CharNum);
	}
}

void PLAYER::SetSwapWeaponTemplate(IN char CharNum, OUT PLAYER_TEMPLATE& swapTemplate)
{
	int iChar = CharNum % GC_CHAR_NUM;
	int iJob = CharNum / GC_CHAR_NUM;

	char strFileName[MAX_PATH];
	sprintf(strFileName, "PlayerTemplate%d_%d_swap.lua", iChar, iJob);

	KGCPlayerCommon::LoadPlayerTemplate(m_setSkill, strFileName, swapTemplate, CharNum);
}

void PLAYER::ChangeBurnningTemplate()
{
	// 템플릿 바꾸기( 아... 반전... 안좋다... 이러면 싱크 안맞음!.)
	m_bIsBurnningMode = !m_bIsBurnningMode;

	swap(GetCurFormTemplate(), GetCurBurnningTemplate());

	if (!m_bIsBurnningMode)
	{
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_SIEG_FURY);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_JIN_BURNNING_MODE_EFFECT);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN_MAGICSWORD_MODE);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN2_SPEAR_MODE);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN3_SHIELD_MODE);
		g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, EGC_EFFECT_RONAN4_BURNING_MODE);
		InitBurnningGauge();
	}

	LoadSlash();

	if (IsLocalPlayer())
	{
		KGC_PID_BROAD_GAME_BURNNING kPacket;
		kPacket.dwUID = m_kUserInfo.dwUID;
		kPacket.bBurnning = m_bIsBurnningMode;
		SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE);
		KGCPC_MEMO("KGC_PID_BROAD_GAME_BURNNING");
	}
}

void PLAYER::ChangeEquipWeapon()
{
	// 현재 장착 중인 장비 찾기
	GCITEMID iWeaponID = 0;
	GCITEMUID dwWeaponUID = 0;
	std::vector<SUserItem>::iterator vecIter = GetCurrentChar().vecItemInfo.begin();
	for (; vecIter != GetCurrentChar().vecItemInfo.end(); ++vecIter)
	{
		GCItem* pItem = g_pItemMgr->GetItemData(vecIter->iItemID);
		if (pItem && (pItem->dwSlotPosition & ESP_WEAPON))
		{
			iWeaponID = vecIter->iItemID;
			dwWeaponUID = vecIter->dwItemUID;
			break;
		}
	}

	// 장비스왑 : 기본장비는 UID가 0이다.
	if (0 == m_mapSwapWeapon[GetCurrentChar().iCharType].second.second)
		g_pItemMgr->UnequipItemForItemUID(&GetCurrentChar(), dwWeaponUID);
	else
	{
		if (g_MyD3D->Get_MyPlayer() == m_iPlayerIndex)
			g_pItemMgr->EquipInventoryItem(&GetCurrentChar(), m_mapSwapWeapon[GetCurrentChar().iCharType].second.second);
		else
		{
			GCItem* pGCItem = g_MyD3D->m_kItemMgr.GetItemData(m_mapSwapWeapon[GetCurrentChar().iCharType].second.first / 10);
			KItem kChangeItem;
			GetChangeWeaponItem(kChangeItem);
			if (pGCItem) {
				g_pItemMgr->EquipChangeWeaponItem(&GetCurrentChar(), pGCItem, &kChangeItem);
			}
		}
	}

	// 끼고 왔던 놈이냐?
	bool bBaseEquip = m_mapSwapWeapon[GetCurrentChar().iCharType].first;
	m_mapSwapWeapon[GetCurrentChar().iCharType] = std::make_pair<bool, std::pair<GCITEMID, GCITEMUID>>(!bBaseEquip, std::make_pair/*<GCITEMID,GCITEMUID>*/((GCITEMID)(iWeaponID * 10), dwWeaponUID));
}

void PLAYER::ChangePromotionTemplate()
{
#if defined( WEAPONCHANGE )
	// 버닝모드면 없애버려!
	if (m_bIsBurnningMode)
		ChangeBurnningTemplate();

	m_bWeaponChanging = true;

	// 스왑웨폰 상태라면 원래대로.. ㅠ
	if (m_iWeaponType)
	{
		ChangeWeaponTemplate();
		m_iWeaponType = ~m_iWeaponType;
	}

	// 스왑할 장비가 있는지 확인
	if (m_mapSwapWeapon.end() == m_mapSwapWeapon.find(GetCurrentChar().iCharType))
		return;

	BaseEffectClear();
	ChangeEquipWeapon();
	ForcedClearBuffList();

	// 전직갱신 : 모션초기화, 리소스업데이트, 플레이어템플릿변경, 이펙트초기화, 스킬트리초기화, 펫초기화, 변신 모델 리로드
	if (g_kGlobalValue.IsHybridPlayer(GetCurrentChar().iCharType)) {
		UpdatePlayerResource();
		if (false == m_bDecChangeWeapon) {
			Calcualte_Point(!m_bDecChangeWeapon, true, false);
			m_bDecChangeWeapon = true;
		}
		else {
			Calcualte_Point(!m_bDecChangeWeapon, true, false);
		}

		//LoadPet(); 의 이유가 이름때문이냐?!
		KPetInfo kPet;
		kPet = m_kUserInfo.GetCurrentChar().kPetInfo;
		if (kPet.m_dwID != 0 && kPet.m_dwUID != 0)
		{
			std::wstringstream strTemp;
			strTemp << L"LV " << kPet.m_iLevel << L". " << kPet.m_strName;
			SetPetName(strTemp.str());
		}

		SetPlayerState(GetWaitMotion());
		SetFrameIndex(GetFrameNum());
	}
	else {
		ReloadResource();
	}

	if (g_kGlobalValue.IsCharUseSpecial4(GetCurrentChar().iCharType)) {
		UpdateChangedSkillSlot();
	}
	m_bWeaponChanging = false;
#endif
}

// 호위 이벤트용 몬스터 소환
void PLAYER::SummonEscortEventMonster()
{
	// 방장만
	if (g_kGlobalValue.IsHost() == false)
		return;

#if defined( USE_EVENT_MON_SUMMON_RING )
	if (g_kGlobalValue.IsHost()) {
		bool bEquipRing = false;
		for (int i = 0; i < MAX_PLAYER_NUM; ++i) {
			PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

			if (NULL == pPlayer) {
				continue;
			}

			if (pPlayer->IsLive()) {
				KUserItemVector::const_iterator it = pPlayer->GetCurrentChar().vecItemInfo.begin();
				for (; it != pPlayer->GetCurrentChar().vecItemInfo.end(); ++it) {
					if (it->iItemID == KGCItemManager::ITEM_SUMMON_ARME_RING) {
						bEquipRing = true;
						break;
					}
				}
			}
		}

		if (false == bEquipRing) {
			srand(time(NULL));
			int iRand = rand() % 100;
			if (8 < iRand) {
				return;
			}
		}
	}
#endif

	// 플레이어의 상대적인 위치에 소환
	float fXDiff = GetIsRight() ? vPos.x : vPos.x;
	float x = (fXDiff) / SiKGCFantasticMap()->GetPixelWidth();
	float y = (vPos.y);

	// 패킷 발사
	KGC_PID_BROAD_SUMMON_MONSTER kPacket;
	kPacket.dwUID = m_kUserInfo.dwUID;
	kPacket.IsRight = !GetIsRight();
	kPacket.iTeam = m_kUserInfo.iTeam;
	kPacket.MonsterID = static_cast<EGCMonster>(g_kGlobalValue.GetEscortMonsterID());
	kPacket.fMaxHP = (float)(g_kGlobalValue.GetEscortMonsterHP());
	kPacket.fMotherHpRatio = -1.f;
	kPacket.fLimitDistance = 0.5f;
	kPacket.iSummonLimit = 1;
	kPacket.iBuffIcon = 0;
	kPacket.fStaticMaxHP = kPacket.fMaxHP;
	kPacket.iSlotID = g_kMonsterManager.GetNextCreateMonstersNum(ECREATE_MONSTER_TYPE_SUMMON);
	kPacket.vecSummonPos = D3DXVECTOR2(x, y);
	SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
	KGCPC_MEMO("KGC_PID_BROAD_SUMMON_MONSTER");
}

void PLAYER::SummonDamageMonster(KSummonInfo* pSummonInfo, int iLevel, float fLimitDistance, int iLimitCount, int iHitPlayerIdx)
{
	if (false == IsLocalPlayer())
		return;

	// Offset위치에 소환
	float fX = pSummonInfo->fX / SiKGCFantasticMap()->GetPixelWidth();
	float fY = pSummonInfo->fY;

	// 렉 발생시 네트워크 싱크 문제가 발생하여 소환명령(아래 주석부분)을 Message로 옮깁니다.
	if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
	{
		// 소환 마릿수 제한
		SummonMonsterLimit(iLimitCount, static_cast<EGCMonster>(pSummonInfo->iMonType));

		// 몬스터 소환
			// 몬스터 소환
		int iSlot = g_MyD3D->AddMonster(pSummonInfo->iMonType, fX, fY, !GetIsRight(), false, 0, iLevel, 0, false, false);

		MONSTER* pMonster = g_kMonsterManager.GetMonster(iSlot);

		if (pMonster == NULL)
			return;

		pMonster->m_iTeam = m_kUserInfo.iTeam;
		pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER;
		pMonster->m_dwOwnerUID = m_kUserInfo.dwUID;
		pMonster->m_bLive = true;
		pMonster->m_bRender = true;
		pMonster->ToggleMonsterInfo(true);

		// 플레이어와 몬스터 간 제한거리 설정
		SetSummonMonsterLimitDist(fLimitDistance);

		// 몬스터 State셋팅
		pMonster->SetState(pSummonInfo->strState.c_str());

		if (g_MyD3D->IsPlayerIndex(iHitPlayerIdx))
		{
			PLAYER* pPlayer = g_MyD3D->MyPlayer[iHitPlayerIdx];
			pPlayer->SetDamageSummonMonSlot(iSlot);
			if (pPlayer->IsLocalPlayer())
			{
				g_kCamera.SetTargetMonster(iSlot);
			}
		}
	}
	else
	{
		// 패킷 발사
		KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER kPacket;
		kPacket.dwUID = m_kUserInfo.dwUID;
		kPacket.IsRight = !GetIsRight();
		kPacket.iTeam = m_iOriginalTeam;
		kPacket.iLevel = iLevel;
		kPacket.iSummonLimit = iLimitCount;
		kPacket.fLimitDistance = fLimitDistance;
		kPacket.iHitPlayerIdx = iHitPlayerIdx;
		kPacket.MonsterID = static_cast<EGCMonster>(pSummonInfo->iMonType);
		kPacket.vecSummonPos = D3DXVECTOR2(fX, fY);
		kPacket.iSlotID = g_kMonsterManager.GetNextCreateMonstersNum(ECREATE_MONSTER_TYPE_SUMMON);
		strcpy(kPacket.szState, pSummonInfo->strState.c_str());
		SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
		KGCPC_MEMO("KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_TO_PLAYER");
	}
}

void PLAYER::SummonMonsterUseBuff(int iBuffIndex_, float fBuffTime_)
{
	g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, iBuffIndex_, fBuffTime_);
}

void PLAYER::SummonMonster(EGCMonster monID, float fOffsetX, float fOffsetY, float fLimitDistance, int iLimitCount, float fHP, int iBuffIcon, bool bNoPlayerPos)
{
	if (false == IsLocalPlayer())
		return;

	// Offset위치에 소환
	float x, y;
	if (bNoPlayerPos)
	{
		x = (fOffsetX) / SiKGCFantasticMap()->GetPixelWidth();
		y = fOffsetY;
	}
	// 플레이어의 상대적인 위치에 소환
	else
	{
		float fXDiff = GetIsRight() ? vPos.x + fOffsetX : vPos.x - fOffsetX;
		x = (fXDiff) / SiKGCFantasticMap()->GetPixelWidth();
		y = (vPos.y + fOffsetY);
	}

	// 렉 발생시 네트워크 싱크 문제가 발생하여 소환명령(아래 주석부분)을 Message로 옮깁니다.
	if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
	{
		// 소환 마릿수 제한
		SummonMonsterLimit(iLimitCount, monID);

		int iSlot = g_MyD3D->AddMonster(monID, x, y, !GetIsRight(), false, 0, -1, 0, false, false);

		MONSTER* pMonster = g_kMonsterManager.GetMonster(iSlot);

		pMonster->m_iTeam = m_kUserInfo.iTeam;
		pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER;
		pMonster->m_dwOwnerUID = m_kUserInfo.dwUID;
		pMonster->m_bLive = true;
		pMonster->m_bRender = true;
		pMonster->ToggleMonsterInfo(true);

		// 플레이어가 소환한 몬스터는 플레이어 레벨에 따라 변한다.
		SCharInfo sCharInfo = GetCurrentChar();
		if (-1 != sCharInfo.iCharType)
		{
			// 레벨
			pMonster->m_iLevel = sCharInfo.iLevel;

			// 생명력
			pMonster->m_fHPPoint = ((sCharInfo.iLevel + 1) * fHP);
			pMonster->m_fHP = pMonster->m_fHPPoint;
		}

		// 플레이어와 몬스터 간 제한거리 설정
		SetSummonMonsterLimitDist(fLimitDistance);

		// 소환 버프 아이콘 출력
		if (iBuffIcon)
		{
			pMonster->m_iSummonBuffIcon = iBuffIcon;
			SetMagicEffect(iBuffIcon, 999999.0f);
		}
	}
	else
	{
		// 패킷 발사
		KGC_PID_BROAD_SUMMON_MONSTER kPacket;
		kPacket.dwUID = m_kUserInfo.dwUID;
		kPacket.IsRight = !GetIsRight();
		kPacket.iTeam = m_iOriginalTeam;
		kPacket.MonsterID = monID;
		kPacket.fMaxHP = fHP;
		kPacket.fMotherHpRatio = -1.f;
		kPacket.fOwnerAtkRatio = 1.f;
		kPacket.fSummonsAtkRatio = (m_kUserInfo.GetCurrentChar().iLevel + 1) * 0.5f;
		kPacket.fLimitDistance = fLimitDistance;
		kPacket.iSummonLimit = iLimitCount;
		kPacket.iBuffIcon = iBuffIcon;
		kPacket.vecSummonPos = D3DXVECTOR2(x, y);
		kPacket.iSlotID = g_kMonsterManager.GetNextCreateMonstersNum(ECREATE_MONSTER_TYPE_SUMMON);
		SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
		KGCPC_MEMO("KGC_PID_BROAD_SUMMON_MONSTER");
	}
}

// 2011, 3, 25, shmhlove : 소환 몬스터 마릿 수 제한
/*
	 전체 소환 마릿수는 2마리로 제한
	 동족에 한해서는 iLimitCount의 값으로 제한
*/
void PLAYER::SummonMonsterLimit(int iLimitCount, EGCMonster monID)
{
	if (iLimitCount < 1)
		return;

	// 소환된 몬스터 체크
	int iAllCnt = 0;
	int iKindCnt = 0;
	bool bKillMonster = false;

	for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		// 이벤트 소환 몬스터는 카운팅 하지 않는다.
		if (mit->second->GetMonsterKind() == MONSTER::EMON_ESCORT)
			continue;

		if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) == false)
			continue;

		if ((mit->second->GetHP() > 0) &&
			(mit->second->IsLive()) &&
			(mit->second->m_dwOwnerUID == m_kUserInfo.dwUID))
		{
			bKillMonster = false;

			// 소환된 몬스터 카운팅
			++iAllCnt;
			if (monID == mit->second->m_iMonsterType)
				++iKindCnt;

			// 동족 몬스터 마릿 수 제한
			if (iKindCnt >= iLimitCount)
			{
				--iKindCnt;
				--iAllCnt;
				bKillMonster = true;
				mit->second->m_iSummonBuffIcon = 0;
			}
			// 전체 몬스터 마릿 수 제한( 2마리로 무조건 제한 : 대전시 2마리 * 6 = 12마리,,, 15마리가 한계다 위험하다. )
			else if (iAllCnt >= 2)
			{
				--iAllCnt;
				if (monID == mit->second->m_iMonsterType)
					--iKindCnt;
				bKillMonster = true;
			}

			if (bKillMonster)
			{
				if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
				{
					mit->second->SetHP(0.0);
					mit->second->SetSummonMonsterDieType(true);
					mit->second->m_bLive = false;
				}
				else
					SetSummonMonsterHP_SendHost(0.0f, (char)mit->first, true);
			}
		}
	}
}

std::vector<MONSTER*> PLAYER::GetMySummonMonster(EGCMonster monID)
{
	std::vector<MONSTER*> vecMonster;
	for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) &&
			(mit->second->GetHP() > 0) &&
			(mit->second->IsLive()) &&
			(mit->second->m_iMonsterType == monID) &&
			(mit->second->m_dwOwnerUID == m_kUserInfo.dwUID)
			)
			vecMonster.push_back(mit->second);
	}
	return vecMonster;
}

int PLAYER::GetSummonMonsterNum()
{
	int iCnt = 0;
	for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) &&
			(mit->second->GetHP() > 0) &&
			(mit->second->IsLive()) &&
			(mit->second->m_dwOwnerUID == m_kUserInfo.dwUID)
			)
			++iCnt;
	}
	return iCnt;
}

void PLAYER::SetSummonMonsterHP(float fHPRadio, char cIndex, bool bDieType, EGCMonster eMonID_)
{
	if (-1 == cIndex)
	{
		for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) == false)
				continue;

			if (mit->second->GetMonsterKind() == MONSTER::EMON_ESCORT)
				continue;

			if (eMonID_ != MON_INVALID && eMonID_ != mit->second->m_iMonsterType) {
				continue;
			}

			if ((mit->second->GetHP() > 0) &&
				(mit->second->IsLive()) &&
				(mit->second->m_dwOwnerUID == m_kUserInfo.dwUID))
			{
				mit->second->SetHP((mit->second->GetHP() * fHPRadio));
				mit->second->SetSummonMonsterDieType(bDieType);
			}
		}
	}
	else if (g_kMonsterManager.IsSummonMonsterByPlayer(cIndex))
	{
		MONSTER* pMonster = g_kMonsterManager.GetMonster(cIndex);
		if (pMonster != NULL) {
			pMonster->SetHP((pMonster->GetHP() * fHPRadio));
			pMonster->SetSummonMonsterDieType(bDieType);
		}
	}
}

void PLAYER::SetSummonMonsterHP_SendHost(float fHPRadio, char cIndex, bool bDieType, int iMonID_)
{
	KGC_PID_BROAD_SUMMON_MONSTER_HP kPacket;
	kPacket.dwUID = m_kUserInfo.dwUID;
	kPacket.fHpRadio = fHPRadio;            // 변경할 몬스터의 HP비율( 현재HP * fHPRadio )
	kPacket.cIndex = cIndex;                // 몬스터의 슬롯인덱스, -1이면 모두 적용한다.
	kPacket.bDieType = bDieType;            // 몬스터가 다른몬스터 소환으로 죽는것인지 맞아서 죽는것인지 구분
	kPacket.iMonID = iMonID_;
	SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
	KGCPC_MEMO("KGC_PID_BROAD_SUMMON_MONSTER_HP");
}

// 소환된 몬스터에게 상태값 전달
void PLAYER::SendStateToMonster(EGCMonster monID, int iState)
{
	if (IsLocalPlayer())
	{
		for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) == false)
				continue;

			if ((mit->second->GetHP() > 0) &&
				(mit->second->IsLive()) &&
				(mit->second->m_dwOwnerUID == m_kUserInfo.dwUID) &&
				mit->second->m_iMonsterType == monID)
			{
				if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL)
				{
					KGC_PID_BROAD_SUMMON_MONSTER_STATE kPacket;
					kPacket.dwUID = m_kUserInfo.dwUID;
					kPacket.iMonsterSlotIndex = mit->first;
					kPacket.iMonsterState = iState;
					SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
					KGCPC_MEMO("KGC_PID_BROAD_SUMMON_MONSTER_STATE");
				}
				mit->second->m_iRecvStateFromPlayer = iState;
			}
		}
	}
}

int PLAYER::GetStateToMonster(EGCMonster monID)
{
	for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first) == false)
			continue;
		if ((mit->second->GetHP() > 0) &&
			(mit->second->IsLive()) &&
			(mit->second->m_dwOwnerUID == m_kUserInfo.dwUID) &&
			mit->second->m_iMonsterType == monID)
		{
			return mit->second->m_iRecvStateFromPlayer;
		}
	}
	return -1;
}

void PLAYER::GetChangeWeaponItem(KItem& kChangeItem, bool bBaseItem)
{
	//  bBaseItem이면 무조껀 기본무기로 변경한다.
	std::map<int, KEquipItemInfo>::iterator mIt = m_mapChangeWeaponItem.find(GetCurrentChar().iCharType);
	std::map<int, SUserItem>::iterator mIter = m_mapBaseChangeWeaponItem.find(GetCurrentChar().iCharType);

	//  서브 무기로 변경
	if ((m_mapSwapWeapon[GetCurrentChar().iCharType].first == false && bBaseItem == false))
	{
		if (m_mapChangeWeaponItem.end() == mIt)
		{
			kChangeItem.m_ItemID = -1;
			return;
		}

		mIt->second.Copy(kChangeItem);
		kChangeItem.m_ItemID = kChangeItem.m_ItemID / 10;
		kChangeItem.m_DesignCoordiID = kChangeItem.m_DesignCoordiID / 10;
	}
	//  기본 무기로 변경
	else
	{
		if (m_mapBaseChangeWeaponItem.end() == mIter)
		{
			kChangeItem.m_ItemID = -1;
			return;
		}

		mIter->second.Copy(kChangeItem);
		kChangeItem.m_ItemID = kChangeItem.m_ItemID / 10;
		kChangeItem.m_DesignCoordiID = kChangeItem.m_DesignCoordiID / 10;
	}
}

void PLAYER::InitPromotionTemplate()
{
#if defined( WEAPONCHANGE )
	// 스왑할 장비가 있는지 확인
	if (m_mapSwapWeapon.end() == m_mapSwapWeapon.find(GetCurrentChar().iCharType))
		return;

	if (false == m_mapSwapWeapon[GetCurrentChar().iCharType].first)
		return;

	// 현재 장착 중인 장비 찾기
	GCITEMID iWeaponID = 0;
	GCITEMUID dwWeaponUID = 0;
	std::vector<SUserItem>::iterator vecIter = GetCurrentChar().vecItemInfo.begin();
	for (; vecIter != GetCurrentChar().vecItemInfo.end(); )
	{
		GCItem* pItem = g_pItemMgr->GetItemData(vecIter->iItemID);
		if (pItem && (pItem->dwSlotPosition & ESP_WEAPON))
		{
			iWeaponID = vecIter->iItemID;
			dwWeaponUID = vecIter->dwItemUID;
			g_pItemMgr->UnequipItemForItemUID(&GetCurrentChar(), dwWeaponUID);
			break;
		}
		else
			++vecIter;
	}

	if (g_MyD3D->Get_MyPlayer() == m_iPlayerIndex)
		g_pItemMgr->EquipInventoryItem(&GetCurrentChar(), m_mapSwapWeapon[GetCurrentChar().iCharType].second.second);
	else
	{
		GCItem* pGCItem = g_MyD3D->m_kItemMgr.GetItemData(m_mapSwapWeapon[GetCurrentChar().iCharType].second.first / 10);
		if (pGCItem) {
			KItem kChangeItem;
			GetChangeWeaponItem(kChangeItem, true);
			g_pItemMgr->EquipChangeWeaponItem(&GetCurrentChar(), pGCItem, &kChangeItem);
		}
	}
	m_mapSwapWeapon[GetCurrentChar().iCharType] = std::make_pair<bool, std::pair<GCITEMID, GCITEMUID>>(false, std::make_pair/*<GCITEMID,GCITEMUID>*/((GCITEMID)(iWeaponID * 10), dwWeaponUID));

	// 전직갱신 : 모션초기화, 리소스업데이트, 플레이어템플릿변경, 이펙트초기화, 스킬트리초기화, 펫초기화
	UpdatePlayerResource();
#endif
}

void PLAYER::ChangeWeaponTemplate()
{
	swap(GetCurFormTemplate(), m_SwapTemplate);

	SetSkillType(m_iSkillType);

	RecordBonePosition();
	for (unsigned char i = 0; i < 5 + (LATENCY + SHOWLATENCY); i++)
	{
		m_posDesc.rightHandLatancy[i * 2] = m_posDesc.rightHandTip;
		m_posDesc.rightHandLatancy[i * 2 + 1] = m_posDesc.rightHandBody;

		m_posDesc.leftHandLatancy[i * 2] = m_posDesc.leftHandTip;
		m_posDesc.leftHandLatancy[i * 2 + 1] = m_posDesc.leftHandBody;
	}

	if (CID_AMY1 == Extra_Char_Num)
	{
		// ToWait 가 있는 모션들만 바꿔줍니다.
		if (MID_AMY1_WAIT == GetWaitMotion())
		{
			ms_mapStateFunction[MID_AMY1_DASH] = KGCPlayerAmy::OnDash;
			ms_mapStateFunction[MID_AMY1_DASH_ATK1] = KGCPlayerAmy::OnDashAtk;
			ms_mapStateFunction[MID_AMY1_JUMP_DASH] = KGCPlayerAmy::OnJumpDash;
			ms_mapStateFunction[MID_AMY1_DOUBLE_DASH] = KGCPlayerAmy::OnDoubleDash;
			ms_mapStateFunction[MID_AMY1_RUN] = KGCPlayerAmy::OnRun;
			ms_mapStateFunction[MID_AMY1_UNIQUE_ATK2] = KGCPlayerAmy::OnSpecial2Atk;
			ms_mapStateFunction[MID_AMY1_DASH_ATK2] = KGCPlayerAmy::OnDashAtk;
		}
		else
		{
			ms_mapStateFunction[MID_AMY1_DASH] = KGCPlayerAmy_2::OnDash;
			ms_mapStateFunction[MID_AMY1_DASH_ATK1] = KGCPlayerAmy_2::OnDashAtk;
			ms_mapStateFunction[MID_AMY1_JUMP_DASH] = KGCPlayerAmy_2::OnJumpDash;
			ms_mapStateFunction[MID_AMY1_DOUBLE_DASH] = KGCPlayerAmy_2::OnDoubleDash;
			ms_mapStateFunction[MID_AMY1_RUN] = KGCPlayerAmy_2::OnRun;
			ms_mapStateFunction[MID_AMY1_UNIQUE_ATK2] = KGCPlayerAmy_2::OnSpecial2Atk;
			ms_mapStateFunction[MID_AMY1_DASH_ATK2] = KGCPlayerAmy_2::OnDashAtk;
		}
	}
	if (CID_AMY2 == Extra_Char_Num)
	{
		// ToWait 가 있는 모션들만 바꿔줍니다.
		if (MID_AMY2_WAIT == GetWaitMotion())
		{
			ms_mapStateFunction[MID_AMY2_DASH] = KGCPlayerAmy2::OnDash;
			//ms_mapStateFunction[MID_AMY1_JUMP_DASH]                     = KGCPlayerAmy2::OnJumpDash;
			//ms_mapStateFunction[MID_AMY1_DOUBLE_DASH]                   = KGCPlayerAmy::OnDoubleDash;
			//ms_mapStateFunction[MID_AMY1_RUN]                           = KGCPlayerAmy2::OnRun;
			//ms_mapStateFunction[MID_AMY1_UNIQUE_ATK2]                   = KGCPlayerAmy::OnSpecial2Atk;
			//ms_mapStateFunction[MID_AMY1_DASH_ATK2]                     = KGCPlayerAmy::OnDashAtk;
		}
		else
		{
			ms_mapStateFunction[MID_AMY2_PERFORMENCE_DASH] = KGCPlayerAmy2_2::OnDash;
			//ms_mapStateFunction[MID_AMY1_JUMP_DASH]                     = KGCPlayerAmy2_2::OnJumpDash;
			//ms_mapStateFunction[MID_AMY1_DOUBLE_DASH]                   = KGCPlayerAmy_2::OnDoubleDash;
			//ms_mapStateFunction[MID_AMY1_RUN]                           = KGCPlayerAmy2_2::OnRun;
			//ms_mapStateFunction[MID_AMY1_UNIQUE_ATK2]                   = KGCPlayerAmy_2::OnSpecial2Atk;
			//ms_mapStateFunction[MID_AMY1_DASH_ATK2]                     = KGCPlayerAmy_2::OnDashAtk;
		}
	}
	if (CID_AMY3 == Extra_Char_Num) //에이미 2차전직
	{
		// ToWait 가 있는 모션들만 바꿔줍니다.
		if (MID_AMY3_WAIT == GetWaitMotion())
		{
			ms_mapStateFunction[MID_AMY3_DASH] = KGCPlayerAmy3::OnDash;
			ms_mapStateFunction[MID_AMY3_JUMP_DASH] = KGCPlayerAmy3::OnJumpDash;
			ms_mapStateFunction[MID_AMY3_RUN] = KGCPlayerAmy3::OnRun;
			ms_mapStateFunction[MID_AMY3_FIGHT_JUMP_UNIQUE_SKILL1] = KGCPlayerAmy3::OnJumpUniqueSkill;
			ms_mapStateFunction[MID_AMY3_DASH_ATK1] = KGCPlayerAmy3::OnDashAtk;
			ms_mapStateFunction[MID_AMY3_DASH_ATK2] = KGCPlayerAmy3::OnDashAtk;
		}
		else
		{
			ms_mapStateFunction[MID_AMY3_DANCE_DASH] = KGCPlayerAmy3_2::OnDash;
			ms_mapStateFunction[MID_AMY3_DANCE_JUMP_DASH] = KGCPlayerAmy3_2::OnJumpDash;
			ms_mapStateFunction[MID_AMY3_DANCE_RUN] = KGCPlayerAmy3_2::OnRun;
			ms_mapStateFunction[MID_AMY3_DANCE_JUMP_UNIQUE_SKILL1] = KGCPlayerAmy3::OnJumpUniqueSkill;
			ms_mapStateFunction[MID_AMY3_DANCE_DASH_ATK] = KGCPlayerAmy3_2::OnDashAtk;
		}
	}
	LoadSlash();
}

void PLAYER::ClearMotionParticleList(void)
{
	std::map< int, std::vector<CParticleEventSeqPTR> >::iterator itMP;
	for (itMP = m_mapMotionParticle.begin(); itMP != m_mapMotionParticle.end(); ++itMP)
	{
		for (int i = 0; i < (int)(*itMP).second.size(); ++i)
			g_ParticleManager->DeleteSequence((*itMP).second[i]);
	}

	m_mapMotionParticle.clear();
}

void PLAYER::CheckMotionParticleList(void)
{
	PROFILE_SET("PLAYER::CheckMotionParticleList");

	if (m_mapMotionParticle.count(cFrame))
	{
		for (int i = 0; i < (int)m_mapMotionParticle[cFrame].size(); ++i)
		{
			if (m_mapMotionParticle[cFrame][i])
				g_ParticleManager->DeleteSequence(m_mapMotionParticle[cFrame][i]);
		}

		m_mapMotionParticle.erase(cFrame);
	}
}

void PLAYER::AddMotionParticle(int iFrame, CParticleEventSeqPTR pMotionParticle)
{
	if (!pMotionParticle)
		return;

	if (m_mapMotionParticle.count(iFrame))
	{
		m_mapMotionParticle[iFrame].push_back(pMotionParticle);
	}
	else
	{
		std::vector< CParticleEventSeqPTR > vecParticle;
		vecParticle.push_back(pMotionParticle);
		m_mapMotionParticle.insert(std::make_pair(iFrame, vecParticle));
	}
}

bool PLAYER::CheckChangeSwap(void)
{
	if (!IsLocalPlayer())
		return false;

	if (m_kUserInfo.GetCurrentChar().iCharType != GC_CHAR_AMY)
		return false;

	if (!g_MyD3D->MyCtrl->k_Select_Item)
		return false;

	if (
		uiMotion != GetCurFormTemplate().RESULT_LOSE
		&& uiMotion != GetCurFormTemplate().SELECT
		&& uiMotion != GetCurFormTemplate().WIN
		&& uiMotion != GetCurFormTemplate().RESULT_WIN
		&& uiMotion != GetCurFormTemplate().START

		&& uiMotion != GetOwnMotion(MID_COMMON_ARENA)
		&& uiMotion != GetOwnMotion(MID_COMMON_DROP)
		&& uiMotion != GetOwnMotion(MID_COMMON_TIGER_ROLLING_ACTION)
		&& !IsCatchMotion(uiMotion)
		&& !IsTagMotion()
#if !defined(NATION_IDN)
		&& !IsSpecialSkillMotion()
#endif
		&& !IsDamagedMotion()
		&& uiMotion != GetOwnMotion(MID_COMMON_INTO_THE_TELEPORT)
		&& !CheckInPortal()
		&& uiMotion != GetOwnMotion(MID_COMMON_INTO_TELEPORT)

		&& uiMotion != MID_AMY1_SWAP_TO_DANCE_MODE
		&& uiMotion != MID_AMY1_SWAP_TO_FIGHT_MODE
		&& uiMotion != MID_AMY1_SWAP_TO_DANCEMODE_WAIT
		&& uiMotion != MID_AMY1_SWAP_TO_FIGHTMODE_WAIT
		&& uiMotion != MID_AMY1_SWAP_TO_JUMP

		&& uiMotion != MID_AMY2_SWAP_TO_PERFORMENCE_MODE
		&& uiMotion != MID_AMY2_SWAP_TO_FIGHT_MODE
		&& uiMotion != MID_AMY2_SWAP_TO_PERFORMENCEMODE_WAIT
		&& uiMotion != MID_AMY2_SWAP_TO_FIGHTMODE_WAIT
		&& uiMotion != MID_AMY2_SWAP_TO_FIGHTMODE_JUMP
		&& uiMotion != MID_AMY2_SWAP_TO_PERFORMENCE_JUMP

		&& uiMotion != MID_AMY2_PERFORMENCE_SPECIAL2_READY
		&& uiMotion != MID_AMY2_PERFORMENCE_SPECIAL2
		&& uiMotion != MID_AMY2_PERFORMENCE_SPECIAL3_READY
		&& uiMotion != MID_AMY2_PERFORMENCE_SPECIAL3
		&& uiMotion != MID_AMY2_PERFORMENCE_SPECIAL3_FORWORD
		&& uiMotion != MID_AMY2_PERFORMENCE_SPECIAL3_BACK

		&& uiMotion != MID_AMY3_SWAP_TO_DANCE_MODE
		&& uiMotion != MID_AMY3_SWAP_TO_FIGHT_MODE
		&& uiMotion != MID_AMY3_SWAP_TO_DANCEMODE_WAIT
		&& uiMotion != MID_AMY3_SWAP_TO_DANCEMODE_JUMP
		&& uiMotion != MID_AMY3_SWAP_TO_FIGHTMODE_WAIT
		&& uiMotion != MID_AMY3_SWAP_TO_FIGHTMODE_JUMP

		&& uiMotion != MID_AMY4_SWAP_TO_MELO_MODE
		&& uiMotion != MID_AMY4_SWAP_TO_FIGHT_MODE
		&& uiMotion != MID_AMY4_SWAP_TO_MELOMODE_WAIT
		&& uiMotion != MID_AMY4_SWAP_TO_MELOMODE_JUMP
		&& uiMotion != MID_AMY4_SWAP_TO_FIGHTMODE_WAIT
		&& uiMotion != MID_AMY4_SWAP_TO_FIGHTMODE_JUMP
		&& uiMotion != MID_COMMON_HOLD_UNTIL_ATTACKED
		&& uiMotion != MID_COMMON_DAMAGED_MID_SPECIAL
		&& uiMotion != MID_COMMON_DAMAGED_UPPER_SPECIAL
		&& uiMotion != MID_COMMON_FRONT_FALL_DOWN

		)
	{
		g_MyD3D->MyCtrl->k_Select_Item = false;
		return true;
	}

	return false;
}

UINT PLAYER::GetSwitchMotionByDirection(UINT uiMotion, bool bRight)
{
	if (false == bRight)
	{
		switch (uiMotion)
		{
		case MID_AMY1_DANCE_SPECIAL2:
			uiMotion = MID_AMY1_DANCE_SPECIAL2_LEFT;
			break;
		case MID_AMY1_DANCE_SPECIAL3:
			uiMotion = MID_AMY1_DANCE_SPECIAL3_LEFT;
			break;
		case MID_AMY3_SPECIAL3:
			uiMotion = MID_AMY3_SPECIAL3_LEFT;
			break;
		case MID_AMY3_DANCE_SPECIAL3:
			uiMotion = MID_AMY3_DANCE_SPECIAL3_LEFT;
			break;
		}
	}

	return uiMotion;
}

void PLAYER::LOADING_STATE::Init()
{
	m_iStep = -1;
	m_iDetailCursor = 0;
	m_vecItemEff.clear();
	m_mapOverlappedPair.clear();
	RefreshTime();
}

void PLAYER::LOADING_STATE::RefreshTime()
{
	m_dwElapsedTime = ::timeGetTime();
}

bool PLAYER::LOADING_STATE::IsTimeOver() const
{
	DWORD dwTime = ::timeGetTime() - m_dwElapsedTime;

#ifndef __PATH__
	if (dwTime > (DWORD)10)
	{
		std::stringstream stm;
		stm << "Step :  " << m_iStep << " DetailCursor : " << m_iDetailCursor << " Time : " << dwTime << std::endl;
		TestString += stm.str().c_str();
	}
#endif

	return dwTime > (DWORD)10;
}

bool PLAYER::NoForceEffect(EMIT_POSITION_TYPE positionType)
{
	if (Extra_Char_Num == CID_RONAN3 || Extra_Char_Num == CID_LAS4 || Extra_Char_Num == CID_SIEG4)
		return true;

	bool bWeaponEffect;

	//무기 랜더가 켜볐?꺼볐?보고 그려야 한다
	std::string weaponName = "WEAPON";

	if (Extra_Char_Num == CID_RYAN4)
	{
		if (m_SwapTemplate.WAIT == MID_RYAN4_WAIT)
		{
			weaponName = "MERGE_WEAPON";
		}
	}
	else if (Extra_Char_Num == CID_AMY3)
	{
		if (m_SwapTemplate.WAIT == MID_AMY3_DANCE_WAIT)
		{
			weaponName = "MERGE_WEAPON";
		}
	}
	else if (Extra_Char_Num == CID_SIEG3)
	{
		weaponName = "MERGE_WEAPON";
	}
	else if (Extra_Char_Num == CID_LUPUS1 || Extra_Char_Num == CID_LUPUS2 || Extra_Char_Num == CID_LUPUS3 || Extra_Char_Num == CID_LUPUS4)
	{
		weaponName = "WEAPON_0";
	}

	if (m_pObject != NULL)
		bWeaponEffect = m_pObject->GetMeshRender(weaponName);
	else
		bWeaponEffect = false;

	// 라임의 경우 스킬에 따라 MERGE_WEAPON 끄고 켜는 경우가 많아 따로 한번 더 검사합니다.
	if ((Extra_Char_Num == CID_LIME1 || Extra_Char_Num == CID_LIME2) && false == bWeaponEffect && m_pObject) {
		bWeaponEffect = m_pObject->GetMeshRender("MERGE_WEAPON");
	}

	switch (positionType)
	{
	case EPT_SWORD_TIP:
	case EPT_SWORD_BODY:
	case EPT_SWORD_CENTER:
	case EPT_LEFT_SWORD_TIP:
	case EPT_LEFT_SWORD_BODY:
	case EPT_LEFT_SWORD_CENTER:
		return bWeaponEffect;
	default:
		return true;
	}
}

bool PLAYER::IsReturnToWaitMotion()
{
	if (m_bCheckGround)
		return true;

	if (uiMotion == GetCurFormTemplate().JUMP ||
		uiMotion == GetCurFormTemplate().JUMP_ATK ||
		uiMotion == GetCurFormTemplate().DOUBLE_JUMP ||
		uiMotion == GetCurFormTemplate().sTagTemplate.m_uiEndMotion ||
		uiMotion == GetCurFormTemplate().sTagTemplate.m_uiAtkMotion ||
		(uiMotion >= (UINT)(MID_ELESIS4_JUMP_ATK1) && uiMotion <= (UINT)(MID_ELESIS4_JUMP_ATK6))
		)
	{
		return true;
	}

	switch (uiMotion)
	{
	case MID_LIRE_LEGATK:
	case MID_LIRE_DOUBLEJUMP:
	case MID_LAS_DOUBLEJUMP_ATK:
	case MID_ELESIS3_JUMPATK_UP:
	case MID_ELESIS3_JUMPATK_DOWN:
	case MID_LIRE3_JUMPATK2:
	case MID_ARME3_JUMPATK1:
	case MID_ARME3_JUMPATK2:
	case MID_RYAN2_JUMP_ATK1:
	case MID_ARME4_JUMP_ATK2:
	case MID_LIRE4_DOUBLEJUMP:
	case MID_LIRE4_DOUBLEJUMP_ATK:
	case MID_LIRE4_JUMP_ATK2:
	case MID_LAS4_DOUBLE_DASH_END_JUMP:
	case MID_LAS4_DASH_END_JUMP:
	case MID_RONAN_JUMP_DASH_ATK_END_JUMP:
	case MID_RONAN_DASH_ATK_END_JUMP:
	case MID_RYAN3_JUMP_DASH_ATK_END_JUMP:
	case MID_RYAN3_JUMP_UP_ATK:
	case MID_RYAN3_ARROWDEF_IN_SKY:
	case MID_RYAN3_DASH_ATK_JUMP:
	case MID_RYAN3_DASH_END_JUMP:
	case MID_RONAN3_DASH_ATK1_TO_JUMP:
	case MID_RONAN3_DASH_ATK2_TO_JUMP:
	case MID_AMY1_DOUBLE_JUMP:
	case MID_AMY1_RUN_TO_DOUBLE_JUMP:
	case MID_AMY1_UNIQUE_ATK1:
	case MID_AMY1_UNIQUE_ATK2_TO_JUMP:
	case MID_AMY1_JUMP_ATK:
	case MID_AMY1_DASH_ATK1_TO_JUMP:
	case MID_AMY1_DASH_ATK2_TO_JUMP:
	case MID_AMY1_RUN_TO_JUMP:
	case MID_AMY1_DOUBLE_DASH_TO_JUMP:
	case MID_AMY1_DASH_TO_JUMP:
	case MID_AMY2_DOUBLE_JUMP:
	case MID_AMY2_RUN_TO_DOUBLE_JUMP:
	case MID_AMY2_RUN_TO_JUMP:
	case MID_AMY2_DASH_TO_JUMP:
	case MID_AMY2_PERFORMENCE_SPECIAL_ATK:
	case MID_AMY2_PERFORMENCE_JUMP_ATK:
	case MID_AMY2_JUMP_ATK:
	case MID_AMY2_SPECIAL_ATK1_SKY_TO_JUMP:
	case MID_AMY2_PERFORMENCE_JUMPDOWN_ATK:
	case MID_SKILL_ELESIS1_SPECIAL_ATK2_END_WAIT:
	case MID_RONAN4_JUMP_ATK_A2:
	case MID_RONAN4_DASH_ATK_A1_TO_JUMP:
	case MID_RONAN4_DASH_ATK_A2_TO_JUMP:
	case MID_RONAN4_DASH_ATK_A3_TO_JUMP:
	case MID_RONAN4_DASH_ATK_B1_TO_JUMP:
	case MID_RONAN4_DASH_ATK_B2_TO_JUMP:
	case MID_RONAN4_JUMP_ATK_B:
	case MID_SKILL_ELESIS1_JUMP_ATK:
	case MID_AMY3_DOUBLE_JUMP:
	case MID_AMY3_DANCE_DOUBLE_JUMP:
	case MID_AMY3_RUN_TO_DOUBLE_JUMP:
	case MID_AMY3_DANCE_RUN_TO_DOUBLE_JUMP:
	case MID_AMY3_SWAP_TO_DANCEMODE_JUMP:
	case MID_AMY3_SWAP_TO_FIGHTMODE_JUMP:
	case MID_AMY3_DANCE_DASH_ATK_TO_JUMP:
	case MID_JIN1_TUMBLING_TO_JUMP:
	case MID_JIN1_AERIAL_STEP_ATTACKED:
	case MID_SKILL_LAS_JUMP_ATK_LV2:
	case MID_SKILL_LAS_DROPPING_TO_LAND:
		//case MID_JIN_DRAGON_KICK:
		//case MID_JIN_DRAGON_KICK_TO_JUMP:
	case MID_SIEG1_JUMP_ATK2:
	case MID_SIEG1_SKILL_CLASS0_DEFENCE_LV2:
	case MID_SIEG1_SKILL_JUMP_ATK_LV2_GET_DEFENCE_LV2:
		return true;
	}

	return false;
}

void PLAYER::InitPlayerAbility()
{
	SetPlayerAbility(EAbilityData());
}

void PLAYER::SetPlayerAbility(IN const EAbilityData& stAbilityData_, bool bHPFull /*= true*/)
{
	for (int i = 0; i < ABILITY_MAX; ++i)
	{
		m_fAbility[i] = stAbilityData_.fEquipItemAbility[i];
	}

	if (bHPFull)
		SetHPFull();
}

void PLAYER::GetLevelAbilityMode(OUT EAbilityData& stAbilityData_)
{
	GetLevelAbilityMode(GetCurrentChar().iLevel, m_kUserInfo.GetCurrentChar().iCharType, stAbilityData_);
}

void PLAYER::GetLevelAbilityMode(int iLevel, int iCharType, OUT EAbilityData& stAbilityData_)
{
#if defined(ANGELS_EGG_REFORM)
	if (SiKGCRoomManager()->IsAngelEggModeAbility())
	{
		stAbilityData_.fEquipItemAbility[ABILITY_ATK] = BALANCE_PVP_ABILITY_ATK_VALUE;
		stAbilityData_.fEquipItemAbility[ABILITY_DEF] = BALANCE_PVP_ABILITY_DEF_VALUE;
		stAbilityData_.fEquipItemAbility[ABILITY_HP] = BALANCE_PVP_ABILITY_HP_VALUE;
		return;
	}
#endif

	if (SiKGCRoomManager()->IsDotaModeAbility())
	{
		SDotaTemplate* pDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
		if (pDotaTemplate)
		{
			SCharInfo& sCharInfo = m_kUserInfo.GetCurrentChar();
			if ((-1 < sCharInfo.iCharType && sCharInfo.iCharType < GC_CHAR_NUM))
			{
				m_fAbility[ABILITY_ATK] = pDotaTemplate->vecsDotaAbility[sCharInfo.iCharType].fAbilityAtk;
				m_fAbility[ABILITY_DEF] = pDotaTemplate->vecsDotaAbility[sCharInfo.iCharType].fAbilityDef;
				m_fAbility[ABILITY_HP] = pDotaTemplate->vecsDotaAbility[sCharInfo.iCharType].fAbilityHp;
				return;
			}
		}
	}

	float fRate = 1.0f;
#if defined( LEVEL_DESIGN_STAT_CALC ) 
	fRate = 100.0f;
#endif 

	if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO ||
		(g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_BALANCE) && g_MyD3D->m_pStateMachine->GetState() == GS_MATCH) ||
		SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN ||
		g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO))
	{

#if defined(NATION_PHILIPPINE) || defined ( NATION_USA ) || defined(NATION_IDN) || defined(NATION_TAIWAN)
		stAbilityData_.fEquipItemAbility[ABILITY_ATK] = BALANCE_PVP_ABILITY_ATK_VALUE;
		stAbilityData_.fEquipItemAbility[ABILITY_DEF] = BALANCE_PVP_ABILITY_DEF_VALUE;
		stAbilityData_.fEquipItemAbility[ABILITY_HP] = BALANCE_PVP_ABILITY_HP_VALUE;
#else

		switch (iCharType)
		{
			case GC_CHAR_ELESIS:
			case GC_CHAR_DIO:
			case GC_CHAR_ZERO:
			case GC_CHAR_LUPUS:
			case GC_CHAR_LIME:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.38f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 3.27f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.19f * fRate;
				break;
			case GC_CHAR_LIRE:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.34f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 2.87f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.61f * fRate;
				break;
			case GC_CHAR_RIN:
			case GC_CHAR_ARME:
			case GC_CHAR_LEY:
			case GC_CHAR_BEIGAS:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.36f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 2.52f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 3.0f * fRate;
				break;
			case GC_CHAR_LAS:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.34f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 2.75f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.73f * fRate;
				break;
			case GC_CHAR_RYAN:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.30f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 3.30f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.27f * fRate;
				break;
			case GC_CHAR_RONAN:
			case GC_CHAR_EDEL:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.25f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 2.75f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.20f * fRate;
				break;
			case GC_CHAR_AMY:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.34f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 2.75f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.73f * fRate;
				break;
			case GC_CHAR_JIN:
			case GC_CHAR_ASIN:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.30f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 3.30f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.27f * fRate;
				break;
			case GC_CHAR_SIEG:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.38f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 3.27f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 2.19f * fRate;
				break;
			case GC_CHAR_MARI:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = 4.36f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = 3.52f * fRate;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = 3.0f * fRate;
				break;
			default:
				stAbilityData_.fEquipItemAbility[ABILITY_ATK] = (1.0f * fRate) + (float)iLevel * 0.02f;
				stAbilityData_.fEquipItemAbility[ABILITY_DEF] = (1.0f * fRate) + (float)iLevel * 0.01f;
				stAbilityData_.fEquipItemAbility[ABILITY_HP] = (1.0f * fRate) + (float)iLevel * 0.01f;
				break;
		}
		return;
#endif
	}

	stAbilityData_.fEquipItemAbility[ABILITY_ATK] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "PlayerStat_AP", iLevel);
	stAbilityData_.fEquipItemAbility[ABILITY_DEF] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "PlayerStat_DP", iLevel);
	stAbilityData_.fEquipItemAbility[ABILITY_HP] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "PlayerStat_HP", iLevel);
}

void PLAYER::GetCharBaseAbility(OUT EAbilityData& stAbilityData_)
{
	int iLevel = GetCurrentChar().iLevel;

	stAbilityData_.fEquipItemAbility[ABILITY_ATK] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "PlayerStat_AP", iLevel);
	stAbilityData_.fEquipItemAbility[ABILITY_DEF] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "PlayerStat_DP", iLevel);
	stAbilityData_.fEquipItemAbility[ABILITY_HP] = lua_tinker::call<float>(KGCLuabinder::getInstance().GetLuaState(), "PlayerStat_HP", iLevel);

	stAbilityData_.fEquipItemAbility[ABILITY_ATK] *= 100;
	stAbilityData_.fEquipItemAbility[ABILITY_DEF] *= 100;
	stAbilityData_.fEquipItemAbility[ABILITY_HP] *= 100;

	//Char & VIP Collection Increment
	//for ( auto element : g_kGlobalValue.SetVIPCollectionAttributeList( this ) )
	//	stAbilityData_.fEquipItemAbility[ (EAbilityType)element.m_iAttribute ] += element.m_fAttributeValue;
}

KInvenBuffItem* PLAYER::GetInvenBuffItem(GCITEMID _itemID)
{
	MAP_INVEN_BUFF_GROUP_ITER pos = g_pItemMgr->m_mapInvenBuffItem.begin();
	MAP_INVEN_BUFF_GROUP_ITER end = g_pItemMgr->m_mapInvenBuffItem.end();

	for (; pos != end; ++pos)
	{
		VEC_INVEN_BUFF* vecInven = &pos->second;

		VEC_INVEN_BUFF_ITER pos = std::find_if(vecInven->begin(), vecInven->end(), boost::bind(&KInvenBuffItem::m_ItemID, _1) == _itemID);
		if (pos != vecInven->end())
			return &(*pos);
	}

	return NULL;
}

// 인벤에 보유하고 있는 아이템 버프
void PLAYER::SetInvenBuffEffect()
{
	MAP_INVEN_BUFF_GROUP& mapInvenBuffItem = g_pItemMgr->m_mapInvenBuffItem;
	MAP_INVEN_BUFF mapInvenBuff;

	//	자기자신 버프 추가
	{
		std::set<GCITEMID>::iterator pos = m_kUserInfo.m_setInvenBuffItemID.begin();
		std::set<GCITEMID>::iterator end = m_kUserInfo.m_setInvenBuffItemID.end();

		for (; pos != end; ++pos)
		{
			GCITEMID itemID = *pos;

			KInvenBuffItem* pInvenBuffItem = GetInvenBuffItem(itemID);
			if (!pInvenBuffItem || pInvenBuffItem->m_ItemID == 0)
				continue;

			//	GameModeCheck()
			if (!pInvenBuffItem->CheckGameMode(SiKGCRoomManager()->GetGameModeCategory()))
				continue;

			mapInvenBuff.insert(std::make_pair(pInvenBuffItem->m_ItemID, *pInvenBuffItem));
		}
	}

	//	파티원이 가지고 있는 파티 버프 추가
	{
		for (int i = 0; i < MAX_PLAYER_NUM; i++)
		{
			if (m_iPlayerIndex == i)
				continue;

			if (!g_MyD3D->MyPlayer[i]->IsLive())
				continue;

			if (!g_MyD3D->IsSameTeam(m_iPlayerIndex, i))
				continue;

			std::set<GCITEMID>& setInvenBuffItemID = g_MyD3D->MyPlayer[i]->m_kUserInfo.m_setInvenBuffItemID;

			std::set<GCITEMID>::iterator pos = setInvenBuffItemID.begin();
			std::set<GCITEMID>::iterator end = setInvenBuffItemID.end();

			for (; pos != end; ++pos)
			{
				GCITEMID itemID = *pos;

				KInvenBuffItem* pInvenBuffItem = GetInvenBuffItem(itemID);
				if (!pInvenBuffItem || pInvenBuffItem->m_ItemID == 0)
					continue;

				//	GameModeCheck()
				if (!pInvenBuffItem->CheckGameMode(SiKGCRoomManager()->GetGameModeCategory()))
					continue;

				if (pInvenBuffItem->m_bAll)
				{
					mapInvenBuff.insert(std::make_pair(pInvenBuffItem->m_ItemID, *pInvenBuffItem));
				}
			}
		}
	}

	//	중복 체크하고 적용한다.
	{
		MAP_INVEN_BUFF_ITER pos = mapInvenBuff.begin();
		MAP_INVEN_BUFF_ITER end = mapInvenBuff.end();

		for (; pos != end; ++pos)
		{
			const KInvenBuffItem& kInvenBuff = pos->second;

			if (kInvenBuff.m_nGroupID == -1)
			{
				g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, kInvenBuff.m_nBuffID, 999999.0f);
			}
			else
			{
				// InvenBuff의 Group을 가져온다.
				MAP_INVEN_BUFF_GROUP_ITER pos_group = mapInvenBuffItem.find(kInvenBuff.m_nGroupID);

				if (pos_group == mapInvenBuffItem.end())
					continue;

				VEC_INVEN_BUFF& vecInven = pos_group->second;
				VEC_INVEN_BUFF_ITER pos2 = vecInven.begin();
				VEC_INVEN_BUFF_ITER end2 = std::find_if(vecInven.begin(), vecInven.end(),
					boost::bind(&KInvenBuffItem::m_ItemID, _1) == kInvenBuff.m_ItemID);

				if (end2 == vecInven.end())
					continue;

				bool bFind = false;

				for (; pos2 != end2; ++pos2)
				{
					const KInvenBuffItem& kInvenBuff2 = *pos2;

					//	더 좋은버프가 있는 상황
					if (mapInvenBuff.find(kInvenBuff2.m_ItemID) != mapInvenBuff.end())
					{
						bFind = true;
						break;
					}
				}

				if (!bFind)
				{
					g_pMagicEffect->SetMagicEffect(m_iPlayerIndex, kInvenBuff.m_nBuffID, 999999.0f);
				}
			}
		}
	}
}

void PLAYER::SetFatalAttriButeEffect()
{
	if (!IsLocalPlayer()) return;

	std::map<int, int> mapAbility;
	mapAbility.insert(std::make_pair(ABILITY_FATAL_SUPER_ARMOR_BUFF, EGC_EFFECT_ATTRIBUTE_SUPER_ARMOR));
	mapAbility.insert(std::make_pair(ABILITY_FATAL_ATTACK_UP_BUFF, EGC_EFFECT_ATTRIBUTE_ATTACK_UP));
	mapAbility.insert(std::make_pair(ABILITY_FATAL_SPEEP_UP_BUFF, EGC_EFFECT_ATTRIBUTE_SPEED_UP));
	mapAbility.insert(std::make_pair(ABILITY_FATAL_CRITICAL_DAMAGE_UP_BUFF, EGC_EFFECT_ATTRIBUTE_CRITICAL_DAMAGE_UP));
	mapAbility.insert(std::make_pair(ABILITY_FATAL_RECOVERY_POTION_UP, EGC_EFFECT_ATTRIBUTE_POTION_UP));

	std::map<int, int>::iterator pos = mapAbility.begin();
	std::map<int, int>::iterator end = mapAbility.end();

	for (; pos != end; ++pos)
	{
		EAbilityType eAbility = static_cast<EAbilityType> (pos->first);
		EGCPlayerMagicEffect eMagicEffect = static_cast<EGCPlayerMagicEffect> (pos->second);

		if (m_fAbility[eAbility] > 0.f)
		{
			SetMagicEffect(eMagicEffect, 99999.f);
		}
	}
}

void PLAYER::ReSetFatalAttriButeEffect()
{
	std::vector<int> vecBuffID;
	vecBuffID.push_back(EGC_EFFECT_ATTRIBUTE_SUPER_ARMOR);
	vecBuffID.push_back(EGC_EFFECT_ATTRIBUTE_RECORVERY_MP_UP);
	vecBuffID.push_back(EGC_EFFECT_ATTRIBUTE_ATTACK_UP);
	vecBuffID.push_back(EGC_EFFECT_ATTRIBUTE_SPEED_UP);
	vecBuffID.push_back(EGC_EFFECT_ATTRIBUTE_CRITICAL_DAMAGE_UP);
	vecBuffID.push_back(EGC_EFFECT_ATTRIBUTE_POTION_UP);

	int nSize = static_cast<int>(vecBuffID.size());

	for (int i = 0; i < nSize; ++i)
	{
		if (g_pMagicEffect->IsMagicEffect(m_iPlayerIndex, vecBuffID[i]))
			g_pMagicEffect->ClearMagicEffect(m_iPlayerIndex, vecBuffID[i]);
	}
}

void PLAYER::SetAgitUserInfo(const KAgitUserInfo& kAgitUserInfo)
{
	m_kUserInfo.strNickName = kAgitUserInfo.m_strNick;
	m_kUserInfo.cCharIndex = kAgitUserInfo.m_cCharType;
	m_kUserInfo.dwUID = kAgitUserInfo.m_dwUID;

	//	로긴정보가 아지트에 없기때문에 초기화 한다.
	m_kUserInfo.strLogin = L"";

	m_kUserInfo.vecIP.push_back(kAgitUserInfo.m_prUserIPPort.first);
	m_kUserInfo.vecPort.push_back(kAgitUserInfo.m_prUserIPPort.second);

	SCharInfo tempInfo;
	tempInfo.iCharType = kAgitUserInfo.m_cCharType;
	tempInfo.iPromotionLevel = kAgitUserInfo.m_cPromotion;
	tempInfo.setPromotion = kAgitUserInfo.m_setPromotion;
	tempInfo.iLevel = static_cast<UCHAR>(kAgitUserInfo.m_dwLevel);
	tempInfo.m_mapEquipSkill = kAgitUserInfo.m_mapSkills;

	m_kUserInfo.vecCharInfo.push_back(tempInfo);              // 새 캐릭 정보를 push_back()하고
	m_kUserInfo.SetCurrentChar(kAgitUserInfo.m_cCharType);    // 앞 인덱스 캐릭 정보를 SetCurrentChar() 하니 문제가 발생하는 듯

	SCharInfo& sCharInfo = m_kUserInfo.GetCurrentChar();

	sCharInfo.vecItemInfo.clear();
	for (int i = 0; i < (int)kAgitUserInfo.m_vecEquipItems.size(); i++)
	{
		SUserItem kUserItem;
		kUserItem = kAgitUserInfo.m_vecEquipItems[i];
		sCharInfo.vecItemInfo.push_back(kUserItem);
	}

	sCharInfo.vecLookInfo.clear();
	for (int i = 0; i < (int)kAgitUserInfo.m_vecLookEquips.size(); i++)
	{
		SUserItem kLookItem;
		kLookItem = kAgitUserInfo.m_vecLookEquips[i];
		sCharInfo.vecLookInfo.push_back(kLookItem);
	}

	sCharInfo.iLevel = static_cast<int>(kAgitUserInfo.m_dwLevel);

	sCharInfo.kPetInfo.m_dwID = kAgitUserInfo.m_kPetInfo.m_dwPetID;
	sCharInfo.kPetInfo.m_strName = kAgitUserInfo.m_kPetInfo.m_strPetName;
	sCharInfo.kPetInfo.m_cPromotion = kAgitUserInfo.m_kPetInfo.m_cPetPromotion;
	sCharInfo.kPetInfo.m_iLevel = kAgitUserInfo.m_kPetInfo.m_cPetLV;
	sCharInfo.kPetInfo.m_kCostume = kAgitUserInfo.m_kPetInfo.m_kCostume;
	sCharInfo.kPetInfo.m_dwID /= 10;
	sCharInfo.kPetInfo.m_kCostume.m_dwID /= 10;
}

void PLAYER::LoadAgitMotion()
{
	std::vector<int>::iterator vecIter = g_kGlobalValue.m_vecAzitMotion.begin();
	for (; vecIter != g_kGlobalValue.m_vecAzitMotion.end(); ++vecIter)
	{
		g_pGraphicsHelper->LoadPlayerActionMotion(Extra_Char_Num, (*vecIter));
	}
}

void PLAYER::SetSelectedPromotion(char Extra_Char_Num)
{
	//현재 선택된 전직 (최고 전직이 아닌...)
	//iIndexNum =  (  ( iCharType - ( (iFileNum -1 ) * 6) )  * NUM_JOB_LEVEL ) + iPromotion;
	if (Extra_Char_Num < GC_CHAR_NUM)
		m_cSelectedPromotion = 0;
	else
		m_cSelectedPromotion = Extra_Char_Num / GC_CHAR_NUM;
}

char PLAYER::GetSelectedPromotionValue()
{
	return m_cSelectedPromotion;
}