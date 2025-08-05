#include "stdafx.h"
#include "kgcbuffmanager.h"
#include "KGCMultipleLanguages.h"
#include "KGCBuffArmeHolyStrike.h"
#include "MyD3D.h"

#include "KGCBuffAnkOfResurrection.h"
#include "KGCBuffBackStep.h"
#include "KGCBuffBloodBash.h"
#include "KGCBuffBombInfect.h"
#include "KGCBuffDarkBall.h"
#include "KGCBuffDarkSwarm.h"

#include "KGCBuffEagleSight.h"
#include "KGCBuffMetamorphosis.h"
#include "KGCBuffRonan3Force.h"
#include "KGCBuffSpit.h"
#include "KGCBuffStatBalance.h"
#include "KGCBuffCaptain.h"
#include "KGCBuffCaptainDisplay.h"
#include "KGCBuffLireMoveUp.h"
#include "KGCBuffLireMpRegenUp.h"
#include "KGCBuffLireShotSpeedUp.h"
#include "KGCBuffLirePoisonArrow.h"
#include "KGCBuffLireFireArrow.h"
#include "KGCBuffLasDropWeapon.h"
#include "KGCBuffLasHide.h"
#include "KGCBuffLasMoveUp.h"
//#include "KGCBuffArmeFrostringLv2.h"
#include "KGCBuffRyanLifeForce.h"
#include "KGCBuffRyanMoveUp.h"
#include "Controls.h"
#include "GCFileMoniter.h"
#include "KGCBuffWingShoes.h"
#include "KGCBuffHeroEarring01.h"
#include "KGCBuffMariEventEarring.h"
#include "KGCBuffrenciaShield.h"
#include "KGCBuffUnoEarring.h"
#include "KGCBuffArmeHolyStrike.h"

//

KGCBuffManager* g_pMagicEffect = NULL;

bool bBuffReload = false;
void InitBuffFileMoniter()
{
	bBuffReload = true;
}

KGCBuffManager::KGCBuffManager(void)
{
	m_iEffectNum = 0;

	//for( int i = 0 ; i < (int)m_aePlayerCartoon.size() ; ++i )
	//{
	//    m_aePlayerCartoon[i] = CARTOON_DEFAULT;
	//}
	//for( int i = 0 ; i < (int)m_aePetCartoon.size() ; ++i )
	//{
	//    m_aePetCartoon[i] = CARTOON_DEFAULT;
	//}

	m_ppOriginalBuff = NULL;

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mit = m_mapPlayerBuff.begin();
	for (; mit != m_mapPlayerBuff.end(); ++mit)
	{
		mit->second.clear();
	}

	m_mapPlayerBuff.clear();
}

KGCBuffManager::~KGCBuffManager(void)
{
	if (m_ppOriginalBuff)
	{
		for (int i = 0; i < m_iEffectNum; i++)
		{
			SAFE_DELETE(m_ppOriginalBuff[i]);
		}
		SAFE_DELETE_ARRAY(m_ppOriginalBuff);
	}

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mit = m_mapPlayerBuff.begin();
	for (; mit != m_mapPlayerBuff.end(); ++mit)
	{
		mit->second.clear();
	}

	m_mapPlayerBuff.clear();
}

void KGCBuffManager::Initialize()
{
	//스트링도 스크립트로 빼야 하는데 -ㅅ-;; 아놔 어떡하지

	m_ppOriginalBuff[EGC_EFFECT_JEWEL_OF_QUEEN]->PushHudString(g_pkStrLoader->GetString(STR_ID_BUFF_JEWEL_OF_QUEEN));
#if defined(NATION_KOREA)
	m_ppOriginalBuff[EGC_EFFECT_JEWEL_OF_QUEEN]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 100));
	m_ppOriginalBuff[EGC_EFFECT_JEWEL_OF_QUEEN]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 100));
#elif defined(NATION_THAILAND)
	m_ppOriginalBuff[EGC_EFFECT_JEWEL_OF_QUEEN]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 1));
	m_ppOriginalBuff[EGC_EFFECT_JEWEL_OF_QUEEN]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 1));
#else
	m_ppOriginalBuff[EGC_EFFECT_JEWEL_OF_QUEEN]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_EFFECT_JEWEL_OF_QUEEN]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));
#endif
	m_ppOriginalBuff[EGC_EFFECT_GRADE_BONUS]->PushHudString(g_pkStrLoader->GetString(STR_ID_BUFF_GRADE_BONUS));
	m_ppOriginalBuff[EGC_EFFECT_GRADE_BONUS]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GRADE_BUNUS_DESC, "i", 10));

	m_ppOriginalBuff[EGC_EFFECT_MOON_FESTIVAL]->PushHudString(g_pkStrLoader->GetString(STR_ID_BUFF_MOON_BONUS));
	m_ppOriginalBuff[EGC_EFFECT_MOON_FESTIVAL]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "s", L"1.5"));
	m_ppOriginalBuff[EGC_EFFECT_MOON_FESTIVAL]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "s", L"1.5"));

	m_ppOriginalBuff[EGC_EFFECT_SUN_FESTIVAL]->PushHudString(g_pkStrLoader->GetString(STR_ID_BUFF_SUN_BONUS));
	m_ppOriginalBuff[EGC_EFFECT_SUN_FESTIVAL]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_EFFECT_SUN_FESTIVAL]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_EFFECT_WHITEDAY]->PushHudString(g_pkStrLoader->GetString(STR_ID_WHITEDAY_EVENT));
	m_ppOriginalBuff[EGC_EFFECT_WHITEDAY]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_EFFECT_WHITEDAY]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_EFFECT_AMY_EVENT]->PushHudString(g_pkStrLoader->GetString(STR_ID_BUFF_AMY_BONUS));
	m_ppOriginalBuff[EGC_EFFECT_AMY_EVENT]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_EFFECT_AMY_EVENT]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_EFFECT_DEATHMATCH_BONUS]->PushHudString(g_pkStrLoader->GetString(STR_ID_DEATHMATCH_BONUS));
	m_ppOriginalBuff[EGC_EFFECT_DEATHMATCH_BONUS]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_EFFECT_DEATHMATCH_BONUS]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_EFFECT_BRUNCH_OF_FLOWERS]->PushHudString(g_pkStrLoader->GetString(STR_ID_BRUNCH_OF_FLOWERS));
	m_ppOriginalBuff[EGC_EFFECT_BRUNCH_OF_FLOWERS]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_EFFECT_BRUNCH_OF_FLOWERS]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_EFFECT_EMBLEM]->PushHudString(L"");
	m_ppOriginalBuff[EGC_EFFECT_EMBLEM]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "s", L"1.5"));
	m_ppOriginalBuff[EGC_EFFECT_EMBLEM]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "s", L"1.5"));

	m_ppOriginalBuff[EGC_FRIENDSHIP_RING]->PushHudString(g_pkStrLoader->GetString(STR_ID_FRIENDSHIP_RING));
	//m_ppOriginalBuff[ EGC_FRIENDSHIP_RING ]->PushHudString( g_pkStrLoader->GetReplacedString( STR_ID_BUFF_EXP_BONUS, "i", 2 ) );
	//m_ppOriginalBuff[ EGC_FRIENDSHIP_RING ]->PushHudString( g_pkStrLoader->GetReplacedString( STR_ID_BUFF_GP_BONUS, "i", 2 ) );

	m_ppOriginalBuff[EGC_ABYSS_KNIGHT]->PushHudString(L"");
	m_ppOriginalBuff[EGC_ABYSS_KNIGHT]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_ABYSS_KNIGHT]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_PV_DAY]->PushHudString(L"");
	m_ppOriginalBuff[EGC_PV_DAY]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "i", 2));
	m_ppOriginalBuff[EGC_PV_DAY]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_1010_DAY_GP100]->PushHudString(L"");
	m_ppOriginalBuff[EGC_1010_DAY_GP100]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "i", 2));

	m_ppOriginalBuff[EGC_EFFECT_AMY_TOGETHER]->PushHudString(g_pkStrLoader->GetString(STR_ID_BUFF_AMY_TOGETHER_BONUS));
	m_ppOriginalBuff[EGC_EFFECT_AMY_TOGETHER]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_EXP_BONUS, "s", L"2"));
	m_ppOriginalBuff[EGC_EFFECT_AMY_TOGETHER]->PushHudString(g_pkStrLoader->GetReplacedString(STR_ID_BUFF_GP_BONUS, "s", L"2"));
	// 마법의 숫자 32-0.5 =3=

	V_Buff[0].SetSLVER(-0.0015625f, 0.0984375f, 0, 0, 0);
	V_Buff[1].SetSLVER(0.0984375f, 0.0984375f, 0, 1.0f, 0);
	V_Buff[2].SetSLVER(-0.0015625f, -0.0015625f, 0, 0, 1.0f);
	V_Buff[3].SetSLVER(0.0984375f, -0.0015625f, 0, 1.0f, 1.0f);
}

void KGCBuffManager::LoadFromLuaScript(char* strFilename_)
{
	KLuaManager kLuaMgr;

	if (!GCFUNC::LoadLuaScript(kLuaMgr, "Enum.stg"))
		return;

	if (!GCFUNC::LoadLuaScript(kLuaMgr, "BuffEnum.stg"))
		return;

	if (!GCFUNC::LoadLuaScript(kLuaMgr, "DamageEnum.stg"))
		return;

	if (!GCFUNC::LoadLuaScript(kLuaMgr, "MotionEnum.stg"))
		return;

	kLuaMgr.GetValue("EGC_EFFECT_NUM", m_iEffectNum);

	if (!GCFUNC::LoadLuaScript(kLuaMgr, strFilename_))
	{
		MessageBoxA(NULL, "BuffInfo.stg Load Fail", NULL, MB_OK);
		return;
	}

	if (FAILED(kLuaMgr.BeginTable("BuffInfo", false)))
	{
		MessageBoxA(NULL, "BuffInfo.stg Load Fail", NULL, MB_OK);
		return;
	}

	SiKGCFileMoniter()->RegisterFile(g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath(strFilename_), InitBuffFileMoniter);

	m_ppOriginalBuff = new KGCBuff*[m_iEffectNum];

	for (int i = 0; i < m_iEffectNum; ++i)
	{
		switch (i)
		{
		case EGC_EFFECT_ARME_HOLYSTRIKE_LIFEKEEP:
			m_ppOriginalBuff[i] = new KGCBuffHolyStrike();
			break;
		case EGC_EFFECT_DARKBALL:
			m_ppOriginalBuff[i] = new KGCBuffDarkBall();
			break;
		case EGC_EFFECT_EAGLE_SITE:
			m_ppOriginalBuff[i] = new KGCBuffEagleSight();
			break;
		case EGC_EFFECT_DARK_SWORM:
		case EGC_EFFECT_MEDUSA_ATK:
			m_ppOriginalBuff[i] = new KGCBuffDarkSwarm();
			break;
		case EGC_EFFECT_BLOOD_BASH:
		case EGC_AMY_STIGMA:
		case EGC_AMY_ABRACADABRA_12:
			m_ppOriginalBuff[i] = new KGCBuffBloodBash();
			break;
		case EGC_EFFECT_BACKSTEP:
		case EGC_EFFECT_GHOST_HIDE:
			m_ppOriginalBuff[i] = new KGCBuffBackStep();
			break;
		case EGC_EFFECT_ANKH_OF_RESURRECTION:
			m_ppOriginalBuff[i] = new KGCBuffAnkOfResurrection();
			break;
		case EGC_EFFECT_SPIT:
			m_ppOriginalBuff[i] = new KGCBuffSpit();
			break;
		case EGC_EFFECT_METAMORPHOSIS_WOLF:
		case EGC_EFFECT_METAMORPHOSIS_WOLF_MOON_LIGHT:
		case EGC_EFFECT_METAMORPHOSIS_NEPHILLIM_FORM:
		case EGC_EFFECT_RIN4_FORM_GODDESS_BUFF:
		case EGC_EFFECT_RIN4_FORM_EVIL_BUFF:
			m_ppOriginalBuff[i] = new KGCBuffMetamorphosis();
			break;
		case EGC_EFFECT_STAT_BALANCE:
			m_ppOriginalBuff[i] = new KGCBuffStatBalance();
			break;
		case EGC_EFFECT_BOMB_INFECT:
			m_ppOriginalBuff[i] = new KGCBuffBombInfect();
			break;
		case EGC_EFFECT_RONAN3_FORCE_BUFF:
			m_ppOriginalBuff[i] = new KGCBuffRonan3Force();
			break;
		case EGC_EFFECT_CAPTAIN_DEBUFF:
			m_ppOriginalBuff[i] = new KGCBuffCaptain();
			break;
		case EGC_EFFECT_CAPTAIN_BUFF:
			m_ppOriginalBuff[i] = new KGCBuffCaptainDisplay();
			break;
		case EGC_EFFECT_DEVIL:
			m_ppOriginalBuff[i] = new KGCBuffDevilPet();
			break;
		case EGC_SKILLEFFECT_MOVE_SPEEDUP:
			m_ppOriginalBuff[i] = new KGCBuffLireMoveUp();
			break;
		case EGC_SKILLEFFECT_MP_REGEN_SPEED:
			m_ppOriginalBuff[i] = new KGCBuffLireMpRegenUp();
			break;
		case EGC_SKILLEFFECT_SHOT_SPEEDUP:
			m_ppOriginalBuff[i] = new KGCBuffLireShotSpeedUp();
			break;
		case EGC_SKILLEFFECT_POISONARROW_DOTDAMAGE:
			m_ppOriginalBuff[i] = new KGCBuffLirePoisonArrow();
			break;
		case EGC_SKILLEFFECT_FIREARROW_DOTDAMAGE:
		case EGC_EFFECT_NIGHTMARKET_FIREDOTDAMAGE:
		case EGC_EFFECT_GRANDROCK_TROKA_FIREDOTDAMAGE:
			m_ppOriginalBuff[i] = new KGCBuffLireFireArrow();
			break;
		case EGC_SKILL_DROP_WEAPON:
		case EGC_EFFECT_EDEL1_UNIQUE_SKILL1_STEAL_WEAPON:
			m_ppOriginalBuff[i] = new KGCBuffLasDropWeapon();
			break;
		case EGC_SKILL_HIDE:
			m_ppOriginalBuff[i] = new KGCBuffLasHide();
			break;
		case EGC_SKILL_LAS_MOVE_SPEEDUP:
			m_ppOriginalBuff[i] = new KGCBuffLasSpeedUp();
			break;
			//         case EGC_EFFECT_SKILL_ARME_FROSTRING_LV2:
			//             m_ppOriginalBuff[i] =  new KGCBuffArmeFrostringLv2();
			//             break;
		case EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV1:
		case EGC_EFFECT_RYAN_STRONGLY_LIFE_FORCE_LV2:
		case EGC_AMY_ABRACADABRA_DUN_6:
			m_ppOriginalBuff[i] = new KGCBuffLifeForce();
			break;
		case EGC_EFFECT_RYAN_MOVE_SPEEDUP:
			m_ppOriginalBuff[i] = new KGCBuffRyanMoveUp();
			break;
		case EGC_EFFECT_INVISIBLE:
			m_ppOriginalBuff[i] = new KGCBuffInvisible();
			break;
		case EGC_SKILLEFFECT_WING_SHOES:
			m_ppOriginalBuff[i] = new KGCBuffWingShoes();
			break;
		case EGC_AMY_ABRACADABRA_7:
			m_ppOriginalBuff[i] = new KGCBuffAmyAbracadabra7();
			break;
		case EGC_CANCELATION:
			m_ppOriginalBuff[i] = new KGCBuffCancelation();
			break;
		case EGC_MARI1_MANASHIELD:
		case EGC_EFFECT_LIME2_MANASHIELD:
			m_ppOriginalBuff[i] = new KGCBuffMari1MagicShield();
			break;
		case EGC_EFFECT_MON_NEMOPHILLA_MIND:
		case EGC_HELL_HOWLING_GHOST:
			m_ppOriginalBuff[i] = new KGCBuffNemoMindControl();
			break;
		case EGC_EFFECT_HERO_EARRING_01: //영웅던전 귀걸이 장신구 버프 등록하자
		case EGC_EFFECT_HERO_EARRING_02:
		case EGC_EFFECT_HERO_EARRING_03:
		case EGC_ETERNAL_SAGE_NOBLITAS_EFF:
			m_ppOriginalBuff[i] = new KGCBuffHeroEarring01();
			break;
		case EGC_EFFECT_DIO_LIFEKEEP:
		case EGC_EFFECT_LIME_SPECIAL4_LEFEKEEP:
			m_ppOriginalBuff[i] = new KGCBuffDioLifeKeep();
			break;
		case EGC_EFFECT_ZERO2_AMID_BUFF:
		case EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK:
		case EGC_EFFECT_ZERO_UNIQUE_CARTOON:
			m_ppOriginalBuff[i] = new KGCBuffZeroAmidSuper();
			break;
		case EGC_EFFECT_ZERO2_DS_REFLECTION:
		case EGC_EFFECT_ZERO_DS_REFLECTION_REINFORCEMENT:
			m_ppOriginalBuff[i] = new KGCBuffZeroDSReflection();
			break;
		case EGC_EFFECT_MARI_EVNET_EARRING:
			m_ppOriginalBuff[i] = new KGCBuffMariEventEarring();
			break;
		case EGC_EFFECT_PET_RENCIA:
		case EGC_EFFECT_PET_CHOUCRING:
		case EGC_EFFECT_PET_DOKKAEBIE:
			m_ppOriginalBuff[i] = new KGCBuffRenciaShield();
			break;
		case EGC_EFFECT_CONFUSION:
		case EGC_EFFECT_CONFUSION2:
		case EGC_EFFECT_CONFUSION_REINFORCEMENT:
			m_ppOriginalBuff[i] = new KGCBuffConfusion();
			break;
		case EGC_EFFECT_LEY3_HAUNT_CURSE04_LV1:
		case EGC_EFFECT_LEY3_HAUNT_CURSE04_LV2:
			m_ppOriginalBuff[i] = new KGCBuffLeyHauntCurse();
			break;
		default:
			m_ppOriginalBuff[i] = new KGCBuff();
			break;
		}

		if (kLuaMgr.BeginTable(i) == S_OK)
		{
			m_ppOriginalBuff[i]->Load(&kLuaMgr);
			kLuaMgr.EndTable();
		}
	}

	kLuaMgr.EndTable();

	Initialize();
}

bool KGCBuffManager::SetMagicEffect(int iPlayerIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_, int iIndex_, bool bIsCrashPlayer, bool bAccTime_, DWORD dwAttackUnit)
{
	if (bIsCrashPlayer && g_MyD3D->MyPlayer[iPlayerIndex_]->m_cLife == 0)
		return false;

	if (iBuffIndex_ >= m_iEffectNum || iBuffIndex_ < 0)
		return false;

	if (NULL == m_ppOriginalBuff)
		return false;

	int iRealOffset = bIsCrashPlayer ? iPlayerIndex_ : iPlayerIndex_ + MAX_PLAYER_NUM;

	// 정해진 버프만 걸려야하는 몬스터가 있따.
	if (false == bIsCrashPlayer)
	{
		if (g_kMonsterManager.IsMonsterIndex(iPlayerIndex_))
		{
			MONSTER *pMonster = g_kMonsterManager.GetMonster(iPlayerIndex_);
			if (pMonster && pMonster->IsLive())
			{
				const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType];
				if (false == kMonsterTemplate.vecBuffList.empty())
				{
					std::vector<int>::const_iterator vecIter = kMonsterTemplate.vecBuffList.begin();
					for (; vecIter != kMonsterTemplate.vecBuffList.end(); ++vecIter)
					{
						if ((*vecIter) == iBuffIndex_)
							break;
					}

					if (vecIter == kMonsterTemplate.vecBuffList.end())
						return false;
				}
			}
		}
	}

	// main.exe!public: int __thiscall KGCBuffManager::SetMagicEffect(int,int,float,int,int,bool) 크래시 관련.

	//====================================================================================
	// 2009.03.13 : Jemitgge
	// Comment : 쿨타임 상태이면 버프 걸지 않는다!
	bool bResult;

	// 버프레벨이 -1이면 중첩 시킨다.
	if (iBuffLevel_ == UINT_MAX)
	{
		iBuffLevel_ = GetBuffLevel(iPlayerIndex_, iBuffIndex_) + 1;
	}

	if (iBuffLevel_ > (int)m_ppOriginalBuff[iBuffIndex_]->GetMaxLevel())
		iBuffLevel_ = (int)m_ppOriginalBuff[iBuffIndex_]->GetMaxLevel();

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iRealOffset);
	if (mitlist == m_mapPlayerBuff.end()) {
		m_mapPlayerBuff[iRealOffset] = std::map< int, KGCBuffInstance*>();
		mitlist = m_mapPlayerBuff.find(iRealOffset);
	}

	std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(iBuffIndex_);
	if (mit == mitlist->second.end())
	{
		// 이미 걸린 버프 중 이 버프를 제거하는 옵션이 있는지 확인하고 걸어야 한다.
		bResult = true;
		std::map< int, KGCBuffInstance*>::iterator mapInstance = mitlist->second.begin();
		for (; mapInstance != mitlist->second.end(); ++mapInstance)
		{
			KGCBuffInstance* pInstance = mapInstance->second;
			if ((NULL == pInstance) || 0 > pInstance->m_iBuffEnum || pInstance->m_iBuffEnum >= m_iEffectNum)
				continue;

			std::vector<DWORD>::iterator vecDeleteList = m_ppOriginalBuff[pInstance->m_iBuffEnum]->m_vecDeleteBuffList.begin();
			for (; vecDeleteList != m_ppOriginalBuff[pInstance->m_iBuffEnum]->m_vecDeleteBuffList.end(); ++vecDeleteList)
			{
				if ((*vecDeleteList) == iBuffIndex_)
				{
					bResult = false;
					break;
				}
			}
			if (false == bResult) break;
		}

		if (bResult)
		{
			KGCBuffInstance* tmpBuff = new KGCBuffInstance();
			tmpBuff->m_iLevel = iBuffLevel_;
			tmpBuff->m_iBuffEnum = iBuffIndex_;
			bResult = m_ppOriginalBuff[iBuffIndex_]->Init(iRealOffset, tmpBuff, fBuffTime_, iIndex_, bIsCrashPlayer, dwAttackUnit);

			mitlist->second[iBuffIndex_] = tmpBuff;
			m_ppOriginalBuff[iBuffIndex_]->InitAfter(iRealOffset, tmpBuff, fBuffTime_, iIndex_, bIsCrashPlayer, dwAttackUnit);
		}
	}
	else
	{
		if (mit->second->m_bPostCoolTime == true)
			return false;

		if (mit->second->m_iLevel > iBuffLevel_)
			iBuffLevel_ = mit->second->m_iLevel;

		m_ppOriginalBuff[iBuffIndex_]->m_bIsCrashPlayer = bIsCrashPlayer;

		float fTime = mit->second->m_fRemainTime;
		ClearMagicEffect(iRealOffset, (int)iBuffIndex_);

		mit->second->m_iLevel = iBuffLevel_;
		mit->second->m_iBuffEnum = iBuffIndex_;
		bResult = m_ppOriginalBuff[iBuffIndex_]->Init(iRealOffset, mit->second, fBuffTime_, iIndex_, bIsCrashPlayer, dwAttackUnit);
		m_ppOriginalBuff[iBuffIndex_]->InitAfter(iRealOffset, mit->second, fBuffTime_, iIndex_, bIsCrashPlayer, dwAttackUnit);

		if (bAccTime_)
		{
			mit->second->m_fRemainTime = fBuffTime_ + fTime;
		}
	}

	return bResult;
}

int KGCBuffManager::FrameMove()
{
	PROFILE_SET("KGCBuffManager::FrameMove");

	if (bBuffReload)
	{
		delete g_pMagicEffect;
		g_pMagicEffect = new KGCBuffManager();
		g_pMagicEffect->LoadFromLuaScript("BuffInfo.stg");
		bBuffReload = false;
		return 0;
	}
	int iPlayerIdx = 0;
	for (std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.begin();
		mitlist != m_mapPlayerBuff.end(); ++mitlist)
	{
		iPlayerIdx = mitlist->first;
		for (std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.begin(); mit != mitlist->second.end(); mit++)
		{
			if (iPlayerIdx < MAX_PLAYER_NUM)
				m_ppOriginalBuff[mit->first]->m_bIsCrashPlayer = true;
			else
				m_ppOriginalBuff[mit->first]->m_bIsCrashPlayer = false;

			if (IsMagicEffect(iPlayerIdx, mit->first))
			{
				if (m_ppOriginalBuff[mit->first]->m_bIsCrashPlayer)
				{
					if (false == m_ppOriginalBuff[mit->first]->m_vecOnlyMotionID.empty() && g_MyD3D->MyPlayer[iPlayerIdx]->IsLocalPlayer())
					{
						std::vector<int>::iterator vecIter = m_ppOriginalBuff[mit->first]->m_vecOnlyMotionID.begin();
						for (; vecIter != m_ppOriginalBuff[mit->first]->m_vecOnlyMotionID.end(); ++vecIter)
						{
							if (*vecIter == g_MyD3D->MyPlayer[iPlayerIdx]->GetCurrentMotion())
								break;
						}

						if (vecIter == m_ppOriginalBuff[mit->first]->m_vecOnlyMotionID.end())
						{
							g_MyD3D->MyPlayer[iPlayerIdx]->ClearMagicEffect(mit->first);
							continue;
						}
					}
				}
			}

			m_ppOriginalBuff[mit->first]->FrameMove(iPlayerIdx, mit->second);
		}
	}

	return 0;
}

void KGCBuffManager::ClearMagicEffect(int iPlayerIndex_, int iBuffIndex_, bool bOnDie_, bool bGoCoolTime, bool bOnNextStage_ /*= false */)
{
	if (m_ppOriginalBuff && iBuffIndex_ < m_iEffectNum && iPlayerIndex_ > -1)
	{
		std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
		if (mitlist == m_mapPlayerBuff.end()) {
			return;
		}

		std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(iBuffIndex_);
		if (mit == mitlist->second.end())
			return;

		if (bGoCoolTime)
		{
			if (mit->second->m_bPostCoolTime == false)
				mit->second->m_fRemainTime = GC_ELAPSED_TIME;
		}
		else
		{
			if (m_ppOriginalBuff[iBuffIndex_])
				m_ppOriginalBuff[iBuffIndex_]->Destroy(iPlayerIndex_, (mit->second), bOnDie_, bOnNextStage_);
		}
	}
}

void KGCBuffManager::ClearMagicEffect(int iPlayerIndex_, bool bOnDie_, bool bOnNextStage_ /*= false */)
{
	for (int i = 0; i < m_iEffectNum; i++)
	{
		ClearMagicEffect(iPlayerIndex_, i, bOnDie_, false, bOnNextStage_);
	}
}

void KGCBuffManager::ClearMagicEffect(bool bOnDie_, bool bOnNextStage_ /*= false */)
{
	for (std::map<int, std::map< int, KGCBuffInstance*> >::iterator mit = m_mapPlayerBuff.begin(); mit != m_mapPlayerBuff.end(); ++mit) {
		ClearMagicEffect(mit->first, bOnDie_, bOnNextStage_);
	}
}

void KGCBuffManager::ClearMonsterAllEffect(int iMonIndex)
{
	if (NULL == m_ppOriginalBuff)
		return;

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iMonIndex + MAX_PLAYER_NUM);
	if (mitlist == m_mapPlayerBuff.end()) {
		return;
	}

	std::map< int, KGCBuffInstance*>::iterator mapIter = mitlist->second.begin();
	for (; mapIter != mitlist->second.end(); ++mapIter)
	{
		if (mapIter->second)
		{
			if (m_ppOriginalBuff[mapIter->first])
				m_ppOriginalBuff[mapIter->first]->Destroy((iMonIndex + MAX_PLAYER_NUM), (mapIter->second), false, false);

			SAFE_DELETE(mapIter->second);
		}
	}
	mitlist->second.clear();
}

void KGCBuffManager::ClearDeBuffEffect(int iPlayerIndex_, bool bIsCrashPlayer)
{
	if (NULL == m_ppOriginalBuff)
		return;

	int iIndex = bIsCrashPlayer ? iPlayerIndex_ : (iPlayerIndex_ + MAX_PLAYER_NUM);

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iIndex);
	if (mitlist == m_mapPlayerBuff.end()) {
		return;
	}

	std::map< int, KGCBuffInstance*>::iterator mapIter = mitlist->second.begin();
	for (; mapIter != mitlist->second.end(); ++mapIter)
	{
		if (mapIter->second && m_ppOriginalBuff[mapIter->first] && m_ppOriginalBuff[mapIter->first]->m_bIsDeBuff)
		{
			m_ppOriginalBuff[mapIter->first]->Destroy(iIndex, (mapIter->second), false, false);
		}
	}
}

void KGCBuffManager::RenderHUD()
{
	if (!m_ppOriginalBuff)
		return;

	int iMyPlayerIndex = g_MyD3D->Get_MyPlayer();

	GCDeviceTexture* pTex;

	D3DXMATRIX mat1;
	int DrawCount = 0;
	float m_fXTerm = 0.055f;
	float m_fCharSize = (float)SiKGCMultipleLanguages()->GetDefualtFontSize() / (float)(W_Width / 2);
	float m_fCharSizeH = (float)SiKGCMultipleLanguages()->GetDefualtFontSize() / (float)(W_Height / 2);

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iMyPlayerIndex);
	if (mitlist == m_mapPlayerBuff.end()) {
		return;
	}

	std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.begin();
	for (; mit != mitlist->second.end(); ++mit)
	{
		if (pTex = m_ppOriginalBuff[mit->first]->GetHudTexture(mit->second))
		{
			if (!pTex->IsLoaded())
			{
				pTex->GetThreadTexture();
				continue;
			}

			float TransX = .8f - (float)DrawCount*.1f;

			D3DXMatrixTranslation(&mat1, TransX, -.45f, .103f);

			g_pGCDeviceManager2->DrawInWorld(pTex, &mat1, V_Buff.as_array());

			WCHAR szText[20] = { 0, };
			wcscpy_s(szText, 20, g_pkStrLoader->GetString(STR_ID_SECOND).c_str());

			WCHAR wChar[20] = { 0, };
			_stprintf(wChar, _T("%d"), (int)mit->second->m_fRemainTime + 1);
			wcscat_s(wChar, 20, szText);

#ifdef NATION_BRAZIL
			std::wstring wstrTmp = wChar;
			boost::algorithm::erase_all(wstrTmp, L" ");
			float fXPos = (float)((float)g_pkFontMgr->GetWidth(wstrTmp.c_str()) / 12.0f * 0.5f * m_fCharSize);
#else
			float fXPos = (float)((float)g_pkFontMgr->GetWidth(wChar) / 12.0f * 0.5f * m_fCharSize);
#endif

			if (mit->second->m_fRemainTime < 10000.0f)
			{
				g_pGCDeviceManager2->Draw_Text(TransX + m_fXTerm - fXPos, -.455f, wChar, D3DCOLOR_ARGB(255, 255, 255, 255), SiKGCMultipleLanguages()->GetDefualtFontSize(), true);
			}

			if ((g_MyD3D->MyCtrl->m_x >= TransX && g_MyD3D->MyCtrl->m_x <= TransX + .1f && g_MyD3D->MyCtrl->m_y <= -.35f && g_MyD3D->MyCtrl->m_y >= -.45f) ||
				m_ppOriginalBuff[mit->first]->m_bAlwaysDescShow)
			{
				int DescSize = m_ppOriginalBuff[mit->first]->GetHudStringNum(mit->second);
				for (int j = 0; j < DescSize; ++j)
				{
					std::wstring strHud = m_ppOriginalBuff[mit->first]->GetHudString(mit->second, j);
					float fXPos = (float)((float)g_pkFontMgr->GetWidth(strHud.c_str()) / 12.0f * 0.5f * m_fCharSize);

					DWORD dwColor;
					if (j == 0)
						dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);
					else
						dwColor = D3DCOLOR_ARGB(255, 255, 255, 0);

					g_pGCDeviceManager2->Draw_Text(TransX + m_fXTerm - fXPos, -.3f + (DescSize - 1 - j) * m_fCharSizeH, strHud.c_str(), dwColor, SiKGCMultipleLanguages()->GetDefualtFontSize(), true);
				}
			}
			DrawCount++;
		}
	}
}

int KGCBuffManager::IsMagicEffect(int iPlayerIndex_, int iBuffIndex_)
{
	if (iBuffIndex_ < 0 || iBuffIndex_ > m_iEffectNum)
	{
		return 0;
	}

	if (m_mapPlayerBuff.empty())
	{
		return 0;
	}

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
	if (mitlist == m_mapPlayerBuff.end()) {
		return 0;
	}

	std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(iBuffIndex_);
	if (mit == mitlist->second.end())
		return 0;

	return mit->second->m_iLevel;
}

bool KGCBuffManager::IsCoolTime(int iPlayerIndex_, int iBuffIndex_)
{
	if (!g_MyD3D->IsPlayerIndex(iPlayerIndex_))
	{
		return false;
	}

	if (iBuffIndex_ < 0 || iBuffIndex_ >= m_iEffectNum)
	{
		return false;
	}

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
	if (mitlist == m_mapPlayerBuff.end()) {
		return false;
	}

	std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(iBuffIndex_);
	if (mit == mitlist->second.end())
		return false;

	//====================================================================================
	// 2009.03.20 : Jemitgge
	// Comment : 쿨타임 중이란 말이다.. 그럼 버프 걸린게 아니지 ㅋㅋ
	return mit->second->m_bPostCoolTime;
}

void KGCBuffManager::SetPlayerCartoon(int iPlayerIndex_, EGCCartoonTexture eCartoon)
{
	if (iPlayerIndex_ >= MAX_PLAYER_NUM || iPlayerIndex_ < 0)
		return;

	m_mapPlayerCartoon[iPlayerIndex_] = eCartoon;

	if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetCartoon(eCartoon);
}

EGCCartoonTexture KGCBuffManager::GetCartoon(int iPlayerIndex_)
{
	std::map<int, EGCCartoonTexture >::iterator mit = m_mapPlayerCartoon.find(iPlayerIndex_);

	if (mit == m_mapPlayerCartoon.end()) return CARTOON_DEFAULT;

	return mit->second;
}

void KGCBuffManager::HiddenBuffParticle(int iBuffIndex_, bool bIsBufftimeStop_, bool bIsHiddenParticle_)
{
	if (NULL == m_ppOriginalBuff)
		return;

	m_ppOriginalBuff[iBuffIndex_]->HiddenParticle(bIsBufftimeStop_, bIsHiddenParticle_);
}

void KGCBuffManager::SetPetCartoon(int iPlayerIndex_, EGCCartoonTexture eCartoon)
{
	if (iPlayerIndex_ >= MAX_PLAYER_NUM || iPlayerIndex_ < 0)
		return;

	if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject == NULL)
		return;

	m_mapPetCartoon[iPlayerIndex_] = eCartoon;
	g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->SetCartoon(eCartoon);
}

EGCCartoonTexture KGCBuffManager::GetPetCartoon(int iPlayerIndex_)
{
	std::map<int, EGCCartoonTexture >::iterator mit = m_mapPetCartoon.find(iPlayerIndex_);

	if (mit == m_mapPetCartoon.end()) return CARTOON_DEFAULT;

	return mit->second;
}

void KGCBuffManager::HitCreateBuffDamage(int iHit, int iHitted, D3DXVECTOR2* vPos, EGCCharAttackType eCharAtkType_)
{
	for (std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.begin();
		mitlist != m_mapPlayerBuff.end(); ++mitlist)
	{
		for (std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.begin(); mit != mitlist->second.end(); mit++)
		{
			if (mit->second->m_fRemainTime <= 0.0f)
				continue;

			if (mitlist->first == iHit)
			{
				m_ppOriginalBuff[mit->first]->CreateDamage(iHit, iHitted, (mit->second), vPos, eCharAtkType_);
			}
		}
	}
}

int KGCBuffManager::GetBuffLevel(int iPlayerIndex_, int iBuffIndex_)
{
	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
	if (mitlist == m_mapPlayerBuff.end()) {
		return 0;
	}

	std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(iBuffIndex_);
	if (mit == mitlist->second.end())
		return 0;

	return mit->second->m_iLevel;
}

void KGCBuffManager::DamageBuffDestroy(int iPlayerIndex_, int iDamage)
{
	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
	if (mitlist == m_mapPlayerBuff.end()) {
		return;
	}

	for (int j = 0; j < m_iEffectNum; j++)
	{
		if (m_ppOriginalBuff[j]->m_bHitOnce)
		{
			for (int i = 0; i < (int)m_ppOriginalBuff[j]->m_vecDamage.size(); i++)
			{
				if (iDamage == m_ppOriginalBuff[j]->m_vecDamage[i])
				{
					std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(j);
					if (mit == mitlist->second.end())
						continue;

					mit->second->m_fRemainTime = GC_ELAPSED_TIME;
				}
			}
		}
	}
}

void KGCBuffManager::ClearBuffInstace(void)
{
	if (m_mapPlayerBuff.empty())
		return;

	for (std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.begin();
		mitlist != m_mapPlayerBuff.end(); ++mitlist)
	{
		std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.begin();
		for (; mit != mitlist->second.end(); ++mit)
		{
			SAFE_DELETE(mit->second);
		}
		mitlist->second.clear();
	}
}

bool KGCBuffManager::IsSpecialUsable(int iPlayerIndex_)
{
	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
	if (mitlist == m_mapPlayerBuff.end()) {
		return true;
	}

	std::map< int, KGCBuffInstance*>& mapBuffInst = mitlist->second;
	std::map< int, KGCBuffInstance*>::iterator mit = mapBuffInst.begin();

	while (mit != mapBuffInst.end())
	{
		if (mit->second->m_fRemainTime > 0.0f)
		{
			if (m_ppOriginalBuff[mit->first]->m_bSpecialDisable)
			{
				return false;
			}
		}
		++mit;
	}
	return true;
}

float KGCBuffManager::IsMagicEffectTimeRet(int iPlayerIndex_, int iBuffIndex_)
{
	if (iBuffIndex_ < 0 || iBuffIndex_ > m_iEffectNum)
	{
		return 0.0f;
	}

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
	if (mitlist == m_mapPlayerBuff.end()) {
		return 0.0f;
	}

	std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(iBuffIndex_);
	if (mit == mitlist->second.end())
		return 0.0f;

	// 쿨타임 중이면 0으로 주자
	if (mit->second->m_bPostCoolTime == true) {
		return 0.0f;
	}

	return mit->second->m_fRemainTime;
}

void KGCBuffManager::SetMagicEffectTimeRet(int iPlayerIndex_, int iBuffIndex_, float fTime_)
{
	if (iBuffIndex_ < 0 || iBuffIndex_ > m_iEffectNum)
	{
		return;
	}

	std::map<int, std::map< int, KGCBuffInstance*> >::iterator mitlist = m_mapPlayerBuff.find(iPlayerIndex_);
	if (mitlist == m_mapPlayerBuff.end()) {
		return;
	}

	std::map< int, KGCBuffInstance*>::iterator mit = mitlist->second.find(iBuffIndex_);
	if (mit == mitlist->second.end())
		return;

	// 쿨타임 중이면 0으로 주자
	if (mit->second->m_bPostCoolTime == true) {
		return;
	}

	mit->second->m_fRemainTime = fTime_;
}

int KGCBuffManager::GetBuffInstanceCount(int iBuffEnum, bool bPlayer, int iMaxMonster)
{
	if (bPlayer)
		return GetPlayerBuffInstanceCount(iBuffEnum);
	else
		return GetMonsterBuffInstanceCount(iBuffEnum);
}

int KGCBuffManager::GetMonsterBuffInstanceCount(int iBuffEnum)
{
	int iInstanceCnt = 0;
	for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		if (mit->second == NULL)
			continue;

		if (mit->second->IsLive() == false)
			continue;

		if (0 != g_pMagicEffect->IsMagicEffect((mit->first + MAX_PLAYER_NUM), iBuffEnum))
			++iInstanceCnt;
	}

	return iInstanceCnt;
}

int KGCBuffManager::GetPlayerBuffInstanceCount(int iBuffEnum)
{
	int iInstanceCnt = 0;
	for (int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop)
	{
		if (false == g_MyD3D->MyPlayer[iLoop]->IsLive())
			continue;

		if ((0 != g_pMagicEffect->IsMagicEffect(iLoop, iBuffEnum)) && (false == IsCoolTime(iLoop, iBuffEnum)))
			++iInstanceCnt;
	}

	return iInstanceCnt;
}

std::vector<std::wstring>& KGCBuffManager::GetHudString(int iBuffEnum)
{
	if (0 > iBuffEnum || iBuffEnum > m_iEffectNum)
		return m_strTempHudString;

	KGCBuff *pBuff = m_ppOriginalBuff[iBuffEnum];
	if (pBuff)
		return pBuff->m_strHudString;
	return m_strTempHudString;
}