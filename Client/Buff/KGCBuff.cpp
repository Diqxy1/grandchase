#include "stdafx.h"
#include ".\kgcbuff.h"
#include "Message.h"
#include "KGCBuffManager.h"
#include "KGCRoomManager.h"
#include "Monster.h"
#include "DamageManager.h"
#include "LatencyTransfer.h"
#include "GCCameraOrtho.h"
#include "../GCUI/GCGameOverlayUI.h"
#include "../GCUI/GCHeadupDisplay.h"
#include "../GCUI/GCMyHeadup.h"

#include "QuestGameManager.h"

KGCBuffInstance::KGCBuffInstance(void)
{
	m_fRemainTime = 0.0f;
	m_fReserve1 = 0.0f;
	m_iLevel = 0;
	m_iBuffEnum = -1;
	m_fPrevFullHPInstance = 0.0f;
	m_bPostCoolTime = false;
	m_iStartCoolTime = -1;
	m_vecLoopSound.clear();
}

KGCBuffInstance::~KGCBuffInstance(void)
{
}

KGCBuff::KGCBuff(void)
{
	m_eCartoonTexture = CARTOON_NORMAL;
	m_bAlwaysDescShow = false;

	m_bPetBuff = false;
	m_bClearOnDie = true;
	m_bClearOnNextStage = true;
	m_bGuildMemberDepend = false;
	m_bSuperArmor = false;
	m_bHyperArmor = false;
	m_bSpecialDisable = false;
	m_bIsKilling = false;

	m_bEnableContinousDamage = false;
	m_iHitPerFrame = 10;
	m_fDamageToPlayer = 0.0f;
	m_fDamageToPlayerMP = 0.0f;
	m_fDamageToMonster = 0.0f;
	m_fAddDamageToPlayer = 0.0f;
	m_fAddDamageToMonster = 0.0f;
	m_fCureDamage = 0.0f;

	m_bDamageByCharAbility = false;

	m_bEnableContinousHeal = false;
	m_fHealToPlayer = 0.0f;
	m_iHealPerFrame = 10;
	m_vecOnlyMotionID.clear();

	m_bIsBuffTimeStop = false;
	m_bIsHiddenParticle = false;
	m_bIsCrashPlayer = true;
	m_vecParticle.clear();
	m_vecAnim.clear();

	m_iCoolTime = 0;

	m_nArenaTime = false;
	m_bAntiPoison = false;
	m_nBerserkTime = false;

	m_iEdgeColor[0] = -1;
	m_nShaderType = -1;

	m_fReflectionDamageRatio = 0.0f;
	m_fDamageToMPAttack = 0.0f;
	m_fStaticToMPAttack = 0.0f;

	m_iMonsterNoTargetingCnt = 0;

	m_bSpeedPercent = false;
	m_dwGivePlayerUID = 0;
	m_bUseNotAP = false;
	m_bKeyLock = false;

	m_iWhenActivatedMotion = -1;

	m_bAllowToOnlyOneMotion = false;
	m_bCharLock = false;
	m_bCharMotionLock = false;
	m_bMonsterFrameLock = false;
	m_bBuffEndHeal = false;
	m_dwSoundIndex = 0;
	m_iStartCoolTime = 0;
	m_iMonsterStopSpeed = 0;

	m_bIsDeBuff = false;
	m_bClearDeBuff = false;

	m_fMonsterRunSpeed = 0.0f;
	m_fMonsterWalkSpeed = 0.0f;

	m_vecDeleteBuffList.clear();
	m_vecMonsterTraceIcon.clear();
	m_vecLoopSound.clear();
	m_vecNotBuffMonsterType.clear();

	m_fCatchResist = 0.f;
	m_fMissileDamageResist = 0.f;
	m_fOriMissileDamageResist = 0.f;

	m_bFatalSuperArmor = false;
	m_bIsAttackMiss = false;

	m_bIsSpecialAtk = false;

	m_fOriMPDecreaseRatio = 1.f;

	m_bDamageNoOnlyPhysicAtk = false;
	m_bDamageNoSpecialAtk = true;
	m_bDamageNoPetAtk = true;

	m_bNotDependenceFromModel = false;

	m_iSuper = 0;
	m_iInitSuper = 0;

	m_iPlayMotion = -1;

	m_bMPRecoveryFix = false;
	m_fMPRecoveryFix = 1.0f;

	m_iChangeTeam = -1;

	m_bNoCrashCheck = false;

	m_fChangeMpRatio = 0.f;
	m_fOriIncMpRatio = 0.f;
	m_fDecreaseBuffTime = 0.0f;

	m_bIsDramaSkip = false;
	m_bNoRenderPet = false;
	m_dwMeshState = 0;
}

KGCBuff::~KGCBuff(void)
{
	for (int i = 0; i < (int)m_vecpHudTex.size(); ++i)
	{
		SAFE_RELEASE(m_vecpHudTex[i]);
	}

	for (int i = 0; i < (int)m_vecpWaitHudTex.size(); ++i)
	{
		SAFE_RELEASE(m_vecpWaitHudTex[i]);
	}
}

void KGCBuff::Load(KLuaManager* pLuaMgr_)
{
	if (pLuaMgr_->BeginTable("StatAttribute") == S_OK)
	{
		for (int iIndex = 1; pLuaMgr_->BeginTable(iIndex) == S_OK; ++iIndex)
		{
			BuffBaseAttirbute att;
			pLuaMgr_->GetValue(1, att.m_fIncATK);
			pLuaMgr_->GetValue(2, att.m_fIncDEF);
			pLuaMgr_->GetValue(3, att.m_fIncHPRatio);
			pLuaMgr_->GetValue(4, att.m_fIncMPRatio);
			pLuaMgr_->GetValue(5, att.m_fIncHP);
			pLuaMgr_->GetValue(6, att.m_bUnFatal);
			pLuaMgr_->GetValue(7, att.m_fHPRecoveryRate);
			pLuaMgr_->GetValue(8, att.m_fCriticalRate);
			pLuaMgr_->GetValue(9, att.m_fCriticalRateMultiply);
			pLuaMgr_->GetValue(10, att.m_fMPRecoveryRate);
			pLuaMgr_->GetValue(11, att.m_fMPDecreaseRatio);
			pLuaMgr_->GetValue(12, att.m_fMinusAPCoolTime);
			pLuaMgr_->GetValue(13, att.m_fMPDecreaseRatioForSpecial);
			pLuaMgr_->GetValue(14, att.m_fCriticalDamageUp);

			if (att.m_fCriticalRateMultiply == 0)
				att.m_fCriticalRateMultiply = 1;

			if (att.m_fMPDecreaseRatio == 0.0f) {
				att.m_fMPDecreaseRatio = 1.0f;
			}

			if (att.m_fMPDecreaseRatioForSpecial == 0.f) {
				att.m_fMPDecreaseRatioForSpecial = 1.f;
			}

			m_vecAttribute.push_back(att);

			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	bool bIsAttackMiss;
	if (pLuaMgr_->GetValue("IsAttackMiss", bIsAttackMiss) == S_OK)
	{
		m_bIsAttackMiss = bIsAttackMiss;
	}

	if (pLuaMgr_->BeginTable("MonAttribute") == S_OK)
	{
		for (int iIndex = 1; pLuaMgr_->BeginTable(iIndex) == S_OK; ++iIndex)
		{
			BuffMonsterAttribute att;
			pLuaMgr_->GetValue(1, att.m_fMonsterAPRate);
			pLuaMgr_->GetValue(2, att.m_fMonsterDPRate);
			pLuaMgr_->GetValue(3, att.m_bMonsterAggro);
			pLuaMgr_->GetValue(4, att.m_bMonsterSDPRate);

			m_vecMonAttribute.push_back(att);

			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	if (pLuaMgr_->BeginTable("Anim") == S_OK)
	{
		for (int iIndex = 1; pLuaMgr_->BeginTable(iIndex) == S_OK; ++iIndex)
		{
			BuffAnim anim;

			pLuaMgr_->GetValue(1, anim.m_strAnim);
			pLuaMgr_->GetValue(2, anim.m_fAnimScaleX);
			pLuaMgr_->GetValue(3, anim.m_fAnimScaleY);
			pLuaMgr_->GetValue(4, anim.m_fAnimPosX);
			pLuaMgr_->GetValue(5, anim.m_fAnimPosY);

			m_vecAnim.push_back(anim);

			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	pLuaMgr_->GetValue("NotDependenceFromModel", m_bNotDependenceFromModel, false);
	if (pLuaMgr_->BeginTable("Particle") == S_OK)
	{
		for (int iIndex = 1; pLuaMgr_->BeginTable(iIndex) == S_OK; ++iIndex)
		{
			BuffParitcle particle;
			pLuaMgr_->GetValue(1, particle.m_strParticle);
			pLuaMgr_->GetValue(2, particle.m_fDirPosX);
			pLuaMgr_->GetValue(3, particle.m_iPosType);
			pLuaMgr_->GetValue(4, particle.m_fPosX);
			pLuaMgr_->GetValue(5, particle.m_fPosY);
			pLuaMgr_->GetValue(6, particle.m_bIsNoDirection);
			pLuaMgr_->GetValue(7, particle.m_iType);
			pLuaMgr_->GetValue(8, particle.m_fTraceTime, 0.0f);
			pLuaMgr_->GetValue(9, particle.m_bLocalEffect, false);

			m_vecParticle.push_back(particle);

			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	if (pLuaMgr_->BeginTable("HudTexture") == S_OK)
	{
		std::string strTextureName;
		for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, strTextureName) == S_OK; ++iIndex)
		{
			m_vecHudTexName.push_back(strTextureName);

			if (GC_GLOBAL_DEFINE::bThreadLoadBuffTex)
			{
				PushHudTexture(NULL);
			}
			else
			{
				PushHudTexture(g_pGCDeviceManager2->CreateTexture(strTextureName));
			}
		}

		pLuaMgr_->EndTable();
	}

	if (pLuaMgr_->BeginTable("HudString") == S_OK)
	{
		int iStrID;
		for (int iIndex2 = 1; pLuaMgr_->BeginTable(iIndex2) == S_OK; ++iIndex2)
		{
			std::wstring strDesc;
			for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, iStrID) == S_OK; ++iIndex)
			{
				strDesc = strDesc + g_pkStrLoader->GetString(iStrID);
			}
			PushHudString(strDesc);
			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	if (pLuaMgr_->BeginTable("WaitHudTexture") == S_OK)
	{
		std::string strTextureName;
		for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, strTextureName) == S_OK; ++iIndex)
		{
			m_vecWaitHudTexName.push_back(strTextureName);

			if (GC_GLOBAL_DEFINE::bThreadLoadBuffTex)
			{
				PushWaitHudTexture(NULL);
			}
			else
			{
				PushWaitHudTexture(g_pGCDeviceManager2->CreateTexture(strTextureName));
			}
		}

		pLuaMgr_->EndTable();
	}

	if (pLuaMgr_->BeginTable("WaitHudString") == S_OK)
	{
		int iStrID;
		for (int iIndex2 = 1; pLuaMgr_->BeginTable(iIndex2) == S_OK; ++iIndex2)
		{
			std::wstring strDesc;
			for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, iStrID) == S_OK; ++iIndex)
			{
				strDesc = strDesc + g_pkStrLoader->GetString(iStrID);
			}
			PushWaitHudString(strDesc);
			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	int iCartoon;
	if (pLuaMgr_->GetValue("Cartoon", iCartoon) == S_OK)
		m_eCartoonTexture = (EGCCartoonTexture)iCartoon;

	bool bClearOnDie;
	if (pLuaMgr_->GetValue("ClearOnDie", bClearOnDie) == S_OK)
	{
		m_bClearOnDie = bClearOnDie;
	}

	bool bClearOnNextStage;
	if (pLuaMgr_->GetValue("ClearOnNextStage", bClearOnNextStage) == S_OK)
	{
		m_bClearOnNextStage = bClearOnNextStage;
	}

	bool bAllowToOnlyOneMotion = false;
	if (pLuaMgr_->GetValue("AllowToOnlyOneMotion", bAllowToOnlyOneMotion) == S_OK)
	{
		m_bAllowToOnlyOneMotion = bAllowToOnlyOneMotion;
	}

	bool bGuildMemberDepend;
	if (pLuaMgr_->GetValue("GuildMemberDepend", bGuildMemberDepend) == S_OK)
	{
		m_bGuildMemberDepend = bGuildMemberDepend;
	}

	bool bIsKilling;
	if (pLuaMgr_->GetValue("IsKilling", bIsKilling) == S_OK)
	{
		m_bIsKilling = bIsKilling;
	}

	if (pLuaMgr_->BeginTable("SpeedAttribute") == S_OK)
	{
		for (int iIndex = 1; pLuaMgr_->BeginTable(iIndex) == S_OK; ++iIndex)
		{
			BuffSpeedAttribute att;
			pLuaMgr_->GetValue(1, att.m_fJumpXSpeed);
			pLuaMgr_->GetValue(2, att.m_fJumpYSpeed);
			pLuaMgr_->GetValue(3, att.m_fDoubleJumpXSpeed);
			pLuaMgr_->GetValue(4, att.m_fDoubleJumpYSpeed);
			pLuaMgr_->GetValue(5, att.m_fRunJumpXSpeed);
			pLuaMgr_->GetValue(6, att.m_fRunJumpYSpeed);
			pLuaMgr_->GetValue(7, att.m_fDashJumpXSpeed);
			pLuaMgr_->GetValue(8, att.m_fDashJumpYSpeed);
			pLuaMgr_->GetValue(9, att.m_fWalkSpeed);
			pLuaMgr_->GetValue(10, att.m_fRunSpeed);

			m_vecSpeedAttribute.push_back(att);

			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	bool bSuperArmor;
	if (pLuaMgr_->GetValue("SuperArmor", bSuperArmor) == S_OK)
	{
		m_bSuperArmor = bSuperArmor;
	}
	int nArenaState = 0;
	if (pLuaMgr_->GetValue("ArenaTime", nArenaState) == S_OK)
	{
		m_nArenaTime = nArenaState;
	}
	bool bAntiPoison = false;
	if (pLuaMgr_->GetValue("AntiPoison", bAntiPoison) == S_OK)
	{
		m_bAntiPoison = bAntiPoison;
	}
	int nBerserk = false;
	if (pLuaMgr_->GetValue("BerserkTime", nBerserk) == S_OK)
	{
		m_nBerserkTime = nBerserk;
	}

	int iColor = false;
	if (pLuaMgr_->GetValue("EdgeColorR", iColor) == S_OK)
	{
		m_iEdgeColor[0] = iColor;
	}
	if (pLuaMgr_->GetValue("EdgeColorG", iColor) == S_OK)
	{
		m_iEdgeColor[1] = iColor;
	}
	if (pLuaMgr_->GetValue("EdgeColorB", iColor) == S_OK)
	{
		m_iEdgeColor[2] = iColor;
	}

	int iShaderType = 0;
	if (pLuaMgr_->GetValue("ShaderType", iShaderType) == S_OK)
	{
		m_nShaderType = iShaderType;
	}

	pLuaMgr_->GetValue("HyperArmor", m_bHyperArmor);
	pLuaMgr_->GetValue("SpecialDIsable", m_bSpecialDisable);

	bool bEnableContinousDamage;
	if (pLuaMgr_->GetValue("EnableContinousDamage", bEnableContinousDamage) == S_OK)
	{
		m_bEnableContinousDamage = bEnableContinousDamage;
	}

	int iHitPerFrame;
	if (pLuaMgr_->GetValue("DamagePerFrame", iHitPerFrame) == S_OK)
	{
		m_iHitPerFrame = iHitPerFrame;
	}

	float fDamageToPlayer;
	if (pLuaMgr_->GetValue("DamageToPlayer", fDamageToPlayer) == S_OK)
	{
		m_fDamageToPlayer = fDamageToPlayer;
	}

	float fDamageToPlayerMP;
	if (pLuaMgr_->GetValue("DamageToPlayerMP", fDamageToPlayerMP) == S_OK)
	{
		m_fDamageToPlayerMP = fDamageToPlayerMP;
	}

	float fDamageToMonster;
	if (pLuaMgr_->GetValue("DamageToMonster", fDamageToMonster) == S_OK)
	{
		m_fDamageToMonster = fDamageToMonster;
	}

	float fAddDamageToPlayer;
	if (pLuaMgr_->GetValue("AddDamageToPlayer", fAddDamageToPlayer) == S_OK)
	{
#ifdef NATION_KOREA
		m_fAddDamageToPlayer = fAddDamageToPlayer;
#else
		m_fAddDamageToPlayer = (fAddDamageToPlayer * 0.01f);
#endif
	}

	float fAddDamageToMonster;
	if (pLuaMgr_->GetValue("AddDamageToMonster", fAddDamageToMonster) == S_OK)
	{
		m_fAddDamageToMonster = fAddDamageToMonster;
	}

	float fCureDamage;
	if (pLuaMgr_->GetValue("CureDamage", fCureDamage) == S_OK)
	{
		m_fCureDamage = fCureDamage;
	}

	bool bDamageByCharAbility;
	if (pLuaMgr_->GetValue("IsDamageByCharAbility", bDamageByCharAbility) == S_OK)
	{
		m_bDamageByCharAbility = bDamageByCharAbility;
	}

	bool bEnableContinousHeal;
	if (pLuaMgr_->GetValue("EnableContinousHeal", bEnableContinousHeal) == S_OK)
	{
		m_bEnableContinousHeal = bEnableContinousHeal;
	}

	float fHealToPlayer;
	if (pLuaMgr_->GetValue("HealToPlayer", fHealToPlayer) == S_OK)
	{
		m_fHealToPlayer = fHealToPlayer;
	}

	int iHealPerFrame;
	if (pLuaMgr_->GetValue("HealPerFrame", iHealPerFrame) == S_OK)
	{
		m_iHealPerFrame = iHealPerFrame;
	}

	if (pLuaMgr_->BeginTable("OnlyMotionID") == S_OK)
	{
		int iOnlyMotionID;
		m_vecOnlyMotionID.clear();
		for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, iOnlyMotionID) == S_OK; ++iIndex)
		{
			m_vecOnlyMotionID.push_back(iOnlyMotionID);
		}
		pLuaMgr_->EndTable();
	}

	int iCoolTime;
	if (pLuaMgr_->GetValue("CoolTime", iCoolTime) == S_OK)
	{
		m_iCoolTime = iCoolTime;
	}

	bool bPetBuff;
	if (pLuaMgr_->GetValue("PetBuff", bPetBuff) == S_OK)
	{
		m_bPetBuff = bPetBuff;
	}
	else
	{
		m_bPetBuff = false;
	}

	bool bUseNotAP = false;
	pLuaMgr_->GetValue("bUseNotAP", bUseNotAP);
	m_bUseNotAP = bUseNotAP;

	if (pLuaMgr_->BeginTable("Damage") == S_OK)
	{
		int iDamage;
		for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, iDamage) == S_OK; ++iIndex)
		{
			m_vecDamage.push_back(iDamage);
		}
		pLuaMgr_->EndTable();
	}
	pLuaMgr_->GetValue("Damage_NoOnlyPhysicAtk", m_bDamageNoOnlyPhysicAtk, false);
	pLuaMgr_->GetValue("Damage_NoSpecialAtk", m_bDamageNoSpecialAtk, true);
	pLuaMgr_->GetValue("Damage_NoPetAtk", m_bDamageNoPetAtk, true);

	m_vecPermissionDamage.clear();
	if (pLuaMgr_->BeginTable("PermissionDamage") == S_OK)
	{
		int iDamage;
		for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, iDamage) == S_OK; ++iIndex)
		{
			m_vecPermissionDamage.push_back(iDamage);
		}
		pLuaMgr_->EndTable();
	}

	if (pLuaMgr_->BeginTable("ToggleExtraMesh") == S_OK)
	{
		for (int iIndex = 1; pLuaMgr_->BeginTable(iIndex) == S_OK; ++iIndex)
		{
			BuffExtraMesh extraMesh;
			pLuaMgr_->GetValue(1, extraMesh.m_extraMesh);
			pLuaMgr_->GetValue(2, extraMesh.m_iMeshTech);

			m_vecExtraMesh.push_back(extraMesh);

			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	m_bHitOnce = false;
	pLuaMgr_->GetValue("ONE_HIT_BUFF", m_bHitOnce);
	m_fHpDrain = 0.0f;
	pLuaMgr_->GetValue("HP_DRAIN", m_fHpDrain, 0.0f);
	m_iItemEff = -1;
	pLuaMgr_->GetValue("ITEM_EFF", m_iItemEff);

	pLuaMgr_->GetValue("REFLECTION_DAMAGE_RATIO", m_fReflectionDamageRatio, 0.0f);
	pLuaMgr_->GetValue("DAMAGE_TO_MP_ATTACK", m_fDamageToMPAttack, 0.0f);
	pLuaMgr_->GetValue("STATIC_TO_MP_ATTACK", m_fStaticToMPAttack, 0.0f);
	pLuaMgr_->GetValue("MONSTER_NO_TARGETING_CNT", m_iMonsterNoTargetingCnt, 0);

	pLuaMgr_->GetValue("CALCULATE_PERCENT_SPEED", m_bSpeedPercent, false);
	pLuaMgr_->GetValue("IS_KEY_LOCK", m_bKeyLock, false);
	pLuaMgr_->GetValue("IS_CHAR_LOCK", m_bCharLock, false);
	pLuaMgr_->GetValue("IS_CHARMOTION_LOCK", m_bCharMotionLock, false);
	pLuaMgr_->GetValue("IS_MONSTER_LOCK", m_bMonsterFrameLock, false);
	pLuaMgr_->GetValue("MONSTER_STOP_SPEED", m_iMonsterStopSpeed, 0);
	pLuaMgr_->GetValue("DECREASE_BUFFTIME", m_fDecreaseBuffTime, 0.0f);

	m_bBuffEndHeal = false;
	pLuaMgr_->GetValue("BUFF_END_HEAL", m_bBuffEndHeal);

	m_dwSoundIndex = 0;
	pLuaMgr_->GetValue("SOUND_ID", m_dwSoundIndex);
	pLuaMgr_->GetValue("START_COOLTIME", m_iStartCoolTime);

	pLuaMgr_->GetValue("IS_DEBUFF", m_bIsDeBuff);
	pLuaMgr_->GetValue("CLEAR_DEBUFF", m_bClearDeBuff);

	pLuaMgr_->GetValue("MONSTER_RUNSPEED", m_fMonsterRunSpeed);
	pLuaMgr_->GetValue("MONSTER_WALKSPEED", m_fMonsterWalkSpeed);

	pLuaMgr_->GetValue("CATCH_RESIST", m_fCatchResist);
	pLuaMgr_->GetValue("MISSILE_DAMAGE_RESIST", m_fMissileDamageResist);

	pLuaMgr_->GetValue("FATAL_SUPER_ARMOR", m_bFatalSuperArmor);

	pLuaMgr_->GetValue("IS_SPECIAL_ATTACK", m_bIsSpecialAtk);

	m_vecDeleteBuffList.clear();
	if (pLuaMgr_->BeginTable("DeleteBuffList") == S_OK)
	{
		int iLoop = 1, iTemp;
		while (S_OK == pLuaMgr_->GetValue(iLoop++, iTemp))
		{
			m_vecDeleteBuffList.push_back(DWORD(iTemp));
		}

		pLuaMgr_->EndTable();
	}

	m_vecMonsterTraceIcon.clear();
	if (S_OK == pLuaMgr_->BeginTable("ChangeMonsterTraceIcon"))
	{
		bool bExistscreen, bInsert = false;
		char szIcon[1024], szArrow[1024];
		if (S_OK == pLuaMgr_->GetValue("ICON", szIcon, 1024))
		{
			bInsert = true;
		}
		if (S_OK == pLuaMgr_->GetValue("ARROW", szArrow, 1024))
		{
			bInsert = bInsert ? true : false;
		}
		pLuaMgr_->GetValue("EXISTSCREEN", bExistscreen);

		if (bInsert)
		{
			m_vecMonsterTraceIcon.insert(std::make_pair(szIcon, std::make_pair(0, bExistscreen)));
			m_vecMonsterTraceIcon.insert(std::make_pair(szArrow, std::make_pair(1, bExistscreen)));
		}

		pLuaMgr_->EndTable();
	}

	if (S_OK == pLuaMgr_->BeginTable("UI_EVENT"))
	{
		pLuaMgr_->GetValue("EVENTCODE", m_kBuffUIEvent.m_dwUIEventCode, 0);
		pLuaMgr_->GetValue("INIT_CALL", m_kBuffUIEvent.bInitCall, false);
		pLuaMgr_->GetValue("END_CALL", m_kBuffUIEvent.bEndCall, false);
		pLuaMgr_->EndTable();
	}

	if (S_OK == pLuaMgr_->BeginTable("SOUND_LOOP"))
	{
		int iLoop = 1;
		float fPlayTime;
		DWORD dwSoundID;
		m_vecLoopSound.clear();
		while (S_OK == pLuaMgr_->BeginTable(iLoop++))
		{
			pLuaMgr_->GetValue(1, dwSoundID, 0);
			pLuaMgr_->GetValue(2, fPlayTime, 0.0f);
			m_vecLoopSound.push_back(std::make_pair(dwSoundID, fPlayTime));
			pLuaMgr_->EndTable();
		}
		pLuaMgr_->EndTable();
	}

	if (S_OK == pLuaMgr_->BeginTable("NOTBUFF_MONSTERTYPE"))
	{
		int iLoop = 1;
		int iTemp;
		m_vecNotBuffMonsterType.clear();
		while (S_OK == pLuaMgr_->GetValue(iLoop++, iTemp))
		{
			m_vecNotBuffMonsterType.push_back(iTemp);
		}
		pLuaMgr_->EndTable();
	}

	if (pLuaMgr_->BeginTable("SummonMonsterAttribute") == S_OK)
	{
		m_kBuffSummonMonster.init();
		int iMonID = -1;
		pLuaMgr_->GetValue("ID", iMonID);
		m_kBuffSummonMonster.m_eMonID = static_cast<EGCMonster>(iMonID);
		pLuaMgr_->GetValue("OffsetX", m_kBuffSummonMonster.m_vOffset.x);
		pLuaMgr_->GetValue("OffsetY", m_kBuffSummonMonster.m_vOffset.y);
		pLuaMgr_->GetValue("LimitDistance", m_kBuffSummonMonster.m_fLimitDistance);
		pLuaMgr_->GetValue("LimitCount", m_kBuffSummonMonster.m_iLimitCount);
		pLuaMgr_->GetValue("MotherHpRatio", m_kBuffSummonMonster.m_fMotherHpRatio);
		pLuaMgr_->GetValue("BuffIcon", m_kBuffSummonMonster.m_iBuffIcon);
		pLuaMgr_->GetValue("NoPlayerPos", m_kBuffSummonMonster.m_bNoPlayerPos);
		pLuaMgr_->GetValue("MaxHP", m_kBuffSummonMonster.m_fMaxHP);
		pLuaMgr_->GetValue("StaticMaxHP", m_kBuffSummonMonster.m_fStaticMaxHP);
		pLuaMgr_->GetValue("IsUnlimitDef", m_kBuffSummonMonster.m_bUnlimitDef);
		pLuaMgr_->GetValue("OwnerAbilityByPercent", m_kBuffSummonMonster.m_iRatioOwnerAbility);

		pLuaMgr_->EndTable();
	}

	pLuaMgr_->GetValue("Super", m_iSuper);
	pLuaMgr_->GetValue("InitSuper", m_iInitSuper);

	pLuaMgr_->GetValue("PLAY_MOTION", m_iPlayMotion);

	pLuaMgr_->GetValue("IS_MP_RECOVERY_FIX", m_bMPRecoveryFix);
	pLuaMgr_->GetValue("MP_RECOVERY_FIX", m_fMPRecoveryFix);
	pLuaMgr_->GetValue("CHANGE_TEAM", m_iChangeTeam);
	pLuaMgr_->GetValue("NO_CRASH_CHECK", m_bNoCrashCheck);

	pLuaMgr_->GetValue("CHANGE_MP_INCREASE_RATIO", m_fChangeMpRatio);
	pLuaMgr_->GetValue("IsDramaSkip", m_bIsDramaSkip);

	m_dwMeshState = 0;
	pLuaMgr_->GetValue("SetMeshRenderState", m_dwMeshState);

	if (pLuaMgr_->BeginTable("SecondBaseMesh") == S_OK)
	{
		std::string strSecondBase;
		for (int iIndex = 1; pLuaMgr_->GetValue(iIndex, strSecondBase) == S_OK; ++iIndex)
		{
			m_vecSecondBaseMesh.push_back(strSecondBase);
		}
		pLuaMgr_->EndTable();
	}

	pLuaMgr_->GetValue("NoRenderPet", m_bNoRenderPet, false);
}

bool KGCBuff::IsBuffChampionIcon( EGCPlayerMagicEffect BuffNum )
{
	int championBuffNum = 40;
	EGCPlayerMagicEffect buffList[] =
	{
		EGC_EFFECT_CHAMPION_ATK,
		EGC_EFFECT_CHAMPION_DEF,
		EGC_EFFECT_CHAMPION_HPREC,
		EGC_EFFECT_CHAMPION_ARMOR,
		EGC_EFFECT_CHAMPION_FS,
		EGC_EFFECT_CHAMPION_CURSE,
		EGC_EFFECT_CHAMPION_STONE,
		EGC_EFFECT_CHAMPION_ICE,
		EGC_EFFECT_CHAMPION_AD,
		EGC_EFFECT_CHAMPION_SW,
		EGC_EFFECT_CHAMPION_ATK_2,
		EGC_EFFECT_CHAMPION_DEF_2,
		EGC_EFFECT_CHAMPION_HPREC_2,
		EGC_EFFECT_CHAMPION_ARMOR_2,
		EGC_EFFECT_CHAMPION_FS_2,
		EGC_EFFECT_CHAMPION_CURSE_2,
		EGC_EFFECT_CHAMPION_STONE_2,
		EGC_EFFECT_CHAMPION_ICE_2,
		EGC_EFFECT_CHAMPION_AD_2,
		EGC_EFFECT_CHAMPION_SW_2,
		EGC_EFFECT_CHAMPION_ATK_3,
		EGC_EFFECT_CHAMPION_DEF_3,
		EGC_EFFECT_CHAMPION_HPREC_3,
		EGC_EFFECT_CHAMPION_ARMOR_3,
		EGC_EFFECT_CHAMPION_FS_3,
		EGC_EFFECT_CHAMPION_CURSE_3,
		EGC_EFFECT_CHAMPION_STONE_3,
		EGC_EFFECT_CHAMPION_ICE_3,
		EGC_EFFECT_CHAMPION_AD_3,
		EGC_EFFECT_CHAMPION_SW_3,
		EGC_EFFECT_CHAMPION_ATK_4,
		EGC_EFFECT_CHAMPION_DEF_4,
		EGC_EFFECT_CHAMPION_HPREC_4,
		EGC_EFFECT_CHAMPION_ARMOR_4,
		EGC_EFFECT_CHAMPION_FS_4,
		EGC_EFFECT_CHAMPION_CURSE_4,
		EGC_EFFECT_CHAMPION_STONE_4,
		EGC_EFFECT_CHAMPION_ICE_4,
		EGC_EFFECT_CHAMPION_AD_4,
		EGC_EFFECT_CHAMPION_SW_4
	};
	for ( int i = 0; i < championBuffNum; ++i )
	{
		if( buffList[i] == BuffNum )
			return true;
	}
	return false;
}

bool KGCBuff::Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (true == bIsCrashPlayer && false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return false;
	else if (false == bIsCrashPlayer && false == g_kMonsterManager.IsMonsterIndex(iPlayerIndex_ - MAX_PLAYER_NUM))
		return false;

	// 특정 몬스터는 이 버프를 받지 않도록 한다.
	std::vector< int >::iterator vecNotBuffIter = m_vecNotBuffMonsterType.begin();
	for (; vecNotBuffIter != m_vecNotBuffMonsterType.end(); ++vecNotBuffIter)
	{
		if ((*vecNotBuffIter) == g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->GetMonsterType())
			return false;
	}

	//길드원 있을때만 적용되는 버프
	if (bIsCrashPlayer)
	{
		if (InitGuildBuff(iPlayerIndex_) == false)
			return false;
	}

	if (m_bPetBuff)
	{
		if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject == NULL)
		{
			return false;
		}
	}

	// 디버프를 지운다.
	if (m_bClearDeBuff)
		g_pMagicEffect->ClearDeBuffEffect(iPlayerIndex_, bIsCrashPlayer);

	//이미 적용된 버프다. 시간만 갱신
	if (pBuffInst_->m_fRemainTime > 0.0f)
	{
		// StartCoolTime으로 호출된 Init에서는 시간갱신 안해도 된다.
		if (-1 == pBuffInst_->m_iStartCoolTime)
		{
			if (pBuffInst_->m_fRemainTime < fBuffTime)
				pBuffInst_->m_fRemainTime = fBuffTime;
			return false;
		}
	}
	else
		pBuffInst_->m_fRemainTime = fBuffTime;

	m_iPlayerIndex = iIndex_;
	m_bIsCrashPlayer = bIsCrashPlayer;

	//StartTime이 있다면 Vector에 담아두고 FrameMove에서 Init처리하자.
	if (0 < m_iStartCoolTime && -1 == pBuffInst_->m_iStartCoolTime)
	{
		pBuffInst_->m_sStartTimeBuff.iPlayerIndex_ = iPlayerIndex_;
		pBuffInst_->m_sStartTimeBuff.pBuffInst_ = pBuffInst_;
		pBuffInst_->m_sStartTimeBuff.fBuffTime = fBuffTime;
		pBuffInst_->m_sStartTimeBuff.iIndex_ = iIndex_;
		pBuffInst_->m_sStartTimeBuff.bIsCrashPlayer = bIsCrashPlayer;
		pBuffInst_->m_sStartTimeBuff.dwGivePlayerUID = dwGivePlayerUID;
		pBuffInst_->m_iStartCoolTime = m_iStartCoolTime;

		return true;
	}

	// StartCoolTime초기화
	pBuffInst_->m_iStartCoolTime = -1;

	//파티클
	if (m_bIsCrashPlayer)
	{
		InitParticle(iPlayerIndex_, pBuffInst_, fBuffTime);
	}

	//능력치 설정하는 부분
	if (bIsCrashPlayer)
	{
		InitAbility(iPlayerIndex_, pBuffInst_);
	}

	pBuffInst_->m_vecAnim.clear();

	//에니메이션
	InitAnimation(pBuffInst_);

	//카툰
	if (m_eCartoonTexture != CARTOON_NORMAL && bIsCrashPlayer)
	{
		if (m_bPetBuff)
		{
			if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject != NULL)
			{
				if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->GetCartoon() == CARTOON_DEFAULT)
				{
					g_pMagicEffect->SetPetCartoon(iPlayerIndex_, m_eCartoonTexture);
				}
			}
		}
		else
		{
			//if ( g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->GetCartoon() == CARTOON_DEFAULT )
			{
				g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, m_eCartoonTexture);
			}
		}
	}

	//속도 능력치 설정하는 부분
	if (bIsCrashPlayer)
	{
		InitPlayerSpeed(iPlayerIndex_, pBuffInst_);
	}
	else
	{
		if (0.0f != m_fMonsterWalkSpeed)
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetAddWalkSpeed(m_fMonsterWalkSpeed);
		if (0.0f != m_fMonsterRunSpeed)
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetAddRunSpeed(m_fMonsterRunSpeed);
	}

	if (m_bSuperArmor)
	{
		if (bIsCrashPlayer)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->SuperArmor(m_bSuperArmor);
			g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_ATHENS);
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetOutlineColor(EDGE_COLOR_SUPERARMOR);
		}
		else
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetSuperArmor(m_bSuperArmor);
		}
	}

	if (m_nBerserkTime) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = GC_FPS_LIMIT * m_nBerserkTime;
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOn(MS_IMMORTAL);
		}
	}

	if (m_nArenaTime) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = GC_FPS_LIMIT * m_nArenaTime;
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOn(MS_IMMORTAL);
		}
	}

	if (m_dwMeshState > 0) {
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOn(m_dwMeshState);
		}
	}

	if (m_bAntiPoison) {
		ResistDebuff(iPlayerIndex_);
	}

	// 몬스터 스테이터스 설정 부분
	if (!m_bIsCrashPlayer)
	{
		InitMonsterStatus(iPlayerIndex_ - MAX_PLAYER_NUM, pBuffInst_, fBuffTime, iIndex_);
	}

	if (m_bIsCrashPlayer) {
		if (m_iItemEff != -1)
		{
			if (!g_MyD3D->MyPlayer[iPlayerIndex_]->IsUseItemEffect((EItemEffect)m_iItemEff))
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetUseItemEffect().insert((EItemEffect)m_iItemEff);
				pBuffInst_->m_fReserve1 = 1.0f;
			}
		}
		// 데미지 반사 설정
		if (0.0f != m_fReflectionDamageRatio)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIsCountAttackShield = true;
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fReflectionDamageRatio = m_fReflectionDamageRatio;
		}

		// MP공격 설정
		if ((0.0f != m_fDamageToMPAttack) || (0.0f != m_fStaticToMPAttack))
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fDamageToMPAttack = m_fDamageToMPAttack;
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fStaticToMPAttack = m_fStaticToMPAttack;
		}

		// AP를 사용할 수 없도록 한다.
		if (m_bUseNotAP)
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetUseNotAP(true);

		// KeyLock을 걸어준다.
		if (m_bKeyLock && g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
			g_MyD3D->SetEnableKeyInput(false);

		// Render를 Lock한다.
		if (m_bCharLock)
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetLock(true);

		// 캐릭터 모션을 Lock한다.
		if (m_bCharMotionLock)
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetMotionLock(true);

		// 버프를 준 플레이어의 UID
		m_dwGivePlayerUID = dwGivePlayerUID;

		if (bIsCrashPlayer) {
			m_iWhenActivatedMotion = g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion;
		}

		// 바꿔줄 모션이 있으면 바꿔주자
		if (m_iPlayMotion > -1) {
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer()) {
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetPlayerState(m_iPlayMotion);
			}
		}
	}

	// 버프 걸렸을때 사운드
	PlaySound(m_dwSoundIndex);

	// 이 유닛이 걸린 버프 중 리스팅된 버프가 걸려있으면 모두 지워라
	std::vector<DWORD>::iterator vecIter = m_vecDeleteBuffList.begin();
	for (; vecIter != m_vecDeleteBuffList.end(); ++vecIter)
	{
		if (bIsCrashPlayer)
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsMagicEffect(*vecIter, true))
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->ClearMagicEffect(*vecIter);
			}
		}
		else
		{
			if (g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->IsMagicEffect((iPlayerIndex_ - MAX_PLAYER_NUM), *vecIter))
			{
				g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ClearMagicEffect((iPlayerIndex_ - MAX_PLAYER_NUM), *vecIter);
			}
		}
	}

	if (false == bIsCrashPlayer)
	{
		std::map<std::string, std::pair<int, bool>>::iterator mapIter = m_vecMonsterTraceIcon.begin();
		for (; mapIter != m_vecMonsterTraceIcon.end(); ++mapIter)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ChangeTraceIcon(const_cast<char*>(mapIter->first.c_str()), mapIter->second.first);
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetTraceIconExistScreen(mapIter->second.second);
		}
	}

	if (int(m_vecLoopSound.size()))
	{
		pBuffInst_->m_vecLoopSound.clear();
		pBuffInst_->m_vecLoopSound = m_vecLoopSound;
	}

	if (bIsCrashPlayer)
	{
		if (m_bSpecialDisable && g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->ToggleBlackGauge(true);
		}

		if (m_iSuper > 0) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->Super += m_iSuper;
			m_iCountSuperFrame = m_iSuper;
		}

		if (m_iInitSuper > 0) {
			m_iOriginalSuperFrame = g_MyD3D->MyPlayer[iPlayerIndex_]->Super;
			g_MyD3D->MyPlayer[iPlayerIndex_]->Super = m_iInitSuper;
		}
	}

	if (bIsCrashPlayer && m_kBuffSummonMonster.m_eMonID != -1) {
		PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];
		if (pPlayer && pPlayer->IsLocalPlayer()) {
			float x, y;
			if (m_kBuffSummonMonster.m_bNoPlayerPos)
			{
				x = (m_kBuffSummonMonster.m_vOffset.x) / SiKGCFantasticMap()->GetPixelWidth();
				y = m_kBuffSummonMonster.m_vOffset.y;
			}
			// 플레이어의 상대적인 위치에 소환
			else
			{
				float fXDiff = pPlayer->GetIsRight() ? pPlayer->vPos.x + m_kBuffSummonMonster.m_vOffset.x : pPlayer->vPos.x - m_kBuffSummonMonster.m_vOffset.x;
				x = (fXDiff) / SiKGCFantasticMap()->GetPixelWidth();
				y = (pPlayer->vPos.y + m_kBuffSummonMonster.m_vOffset.y);
			}

			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bCheckDieSummons = false;
			SCharInfo sCharInfo = pPlayer->GetCurrentChar();
			if (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode())
			{
				// 소환 마릿수 제한
				pPlayer->SummonMonsterLimit(m_kBuffSummonMonster.m_iLimitCount, m_kBuffSummonMonster.m_eMonID);

				int iSlot = g_MyD3D->AddMonster(m_kBuffSummonMonster.m_eMonID, x, y, !pPlayer->GetIsRight(), false, 0, -1, 0, false, false);
				m_kBuffSummonMonster.m_iMonsterSlot = iSlot;

				MONSTER *pMonster = g_kMonsterManager.GetMonster(iSlot);

				pMonster->m_iTeam = pPlayer->m_kUserInfo.iTeam;
				pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_PLAYER;
				pMonster->m_dwOwnerUID = pPlayer->m_kUserInfo.dwUID;
				pMonster->m_bLive = true;
				pMonster->m_bRender = true;
				pMonster->ToggleMonsterInfo(true);

				// 플레이어가 소환한 몬스터는 플레이어 레벨에 따라 변한다.
				if (-1 != sCharInfo.iCharType)
				{
					pMonster->m_iLevel = sCharInfo.iLevel;
					if (m_kBuffSummonMonster.m_fStaticMaxHP != -1.f) {
						pMonster->m_fHPPoint = m_kBuffSummonMonster.m_fStaticMaxHP;
						pMonster->m_fHP = m_kBuffSummonMonster.m_fStaticMaxHP;
						pMonster->m_bUnlimitDef = m_kBuffSummonMonster.m_bUnlimitDef;
					}
					else {
						float fMotherMaxHP = g_MyD3D->MyPlayer[iPlayerIndex_]->GetPlayerMaxHP();
						pMonster->m_fHPPoint = fMotherMaxHP + (fMotherMaxHP * m_kBuffSummonMonster.m_fMotherHpRatio);
						pMonster->m_fHP = pMonster->m_fHPPoint;
					}
				}

				pPlayer->SetSummonMonsterLimitDist(m_kBuffSummonMonster.m_fLimitDistance);

				if (m_kBuffSummonMonster.m_iBuffIcon)
				{
					pMonster->m_iSummonBuffIcon = m_kBuffSummonMonster.m_iBuffIcon;
					pPlayer->SetMagicEffect(m_kBuffSummonMonster.m_iBuffIcon, 999999.0f);
				}
			}
			else {
				// 패킷 발사
				KGC_PID_BROAD_SUMMON_MONSTER kPacket;
				kPacket.dwUID = pPlayer->m_kUserInfo.dwUID;
				kPacket.IsRight = !pPlayer->GetIsRight();
				kPacket.iTeam = pPlayer->m_iOriginalTeam;
				kPacket.MonsterID = m_kBuffSummonMonster.m_eMonID;
				kPacket.fMotherHpRatio = m_kBuffSummonMonster.m_fMotherHpRatio;
				if (m_kBuffSummonMonster.m_iRatioOwnerAbility > 0 && -1 != sCharInfo.iCharType) {
					kPacket.fSummonsAtkRatio = ((sCharInfo.iLevel + 1) * 0.5f);
					kPacket.fOwnerAtkRatio = static_cast<float>(m_kBuffSummonMonster.m_iRatioOwnerAbility) / 100.f;
				}
				kPacket.fLimitDistance = m_kBuffSummonMonster.m_fLimitDistance;
				kPacket.iSummonLimit = m_kBuffSummonMonster.m_iLimitCount;
				kPacket.iBuffIcon = m_kBuffSummonMonster.m_iBuffIcon;
				kPacket.fMaxHP = m_kBuffSummonMonster.m_fMaxHP;
				kPacket.fStaticMaxHP = m_kBuffSummonMonster.m_fStaticMaxHP;
				kPacket.bUnlimitDef = m_kBuffSummonMonster.m_bUnlimitDef;
				kPacket.vecSummonPos = D3DXVECTOR2(x, y);
				kPacket.iSlotID = g_kMonsterManager.GetNextCreateMonstersNum(ECREATE_MONSTER_TYPE_SUMMON);
				m_kBuffSummonMonster.m_iMonsterSlot = kPacket.iSlotID;
				SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
				KGCPC_MEMO("KGC_PID_BROAD_SUMMON_MONSTER");
			}
		}
	}

	if (false == m_vecPermissionDamage.empty())
	{
		if (bIsCrashPlayer)
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetPermissionDamage(m_vecPermissionDamage);
		else
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetPermissionDamage(m_vecPermissionDamage);
	}

	if (bIsCrashPlayer && m_iChangeTeam != -1) {
		int iLiveCnt = 0;
		for (int i = 0; i < MAX_PLAYER_NUM; i++) {
			if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive) {
				iLiveCnt++;
			}
		}

		// 두 명 이상일 시만 적용하자.
		if (iLiveCnt > 2) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.SetTeam(m_iChangeTeam);
		}
	}

	if (bIsCrashPlayer && m_bNoCrashCheck) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bNoCrashCheckByBuff = m_bNoCrashCheck;
	}

	// AddMesh 처리 부분
	if (!m_vecSecondBaseMesh.empty() && IsSceondBaseMeshEnable(g_MyD3D->MyPlayer[iPlayerIndex_], false)) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleBaseMeshStatic(false);
		g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleRenderFace(true);

		for (std::vector< std::string >::const_iterator itSecondBase = m_vecSecondBaseMesh.begin(); itSecondBase != m_vecSecondBaseMesh.end(); ++itSecondBase) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleSecondBaseMesh(itSecondBase->c_str(), true);
			g_MyD3D->MyPlayer[iPlayerIndex_]->RenderWeapon(true);
		}
	}

	if (m_bNoRenderPet) {
		g_MyD3D->m_akPet[iPlayerIndex_]->TogglePetRender(!m_bNoRenderPet);
	}

	return true;
}

bool KGCBuff::InitAfter(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID)
{
	if (m_kBuffUIEvent.bInitCall)
	{
		g_pkUIMgr->SendEvent(KActionEvent(NULL, m_kBuffUIEvent.m_dwUIEventCode), false);
	}

	return true;
}

void KGCBuff::ResistDebuff(int iPlayerIndex_)
{
	// 석화 & 빙결 & 잡힌거 풀기
	if (g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_STONECURSE || g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_FREEZED_BY_PIXIE ||
		g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_FREEZED_BY_ENT || g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_FREEZED_BY_LICH ||
		g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_STONECURSE_BY_MEDUSA)
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->Direct_Motion_Input(g_MyD3D->MyPlayer[iPlayerIndex_]->GetCurFormTemplate().WAIT);
	}

	// 저주 풀기
	if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_DARKBALL))
	{
		g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_DARKBALL, true);
	}

	// 컨퓨전 풀기!
	if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION))
	{
		g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION, true);
	}
	if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION2))
	{
		g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION2, true);
	}
	if (g_pMagicEffect->IsMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION_REINFORCEMENT))
	{
		g_pMagicEffect->ClearMagicEffect(iPlayerIndex_, EGC_EFFECT_CONFUSION_REINFORCEMENT, true);
	}

	// 감전 풀기!
	std::map< int, std::vector<CDamageInstance*> >::iterator itor;
	for (itor = g_MyD3D->m_pDamageManager->GetDamageInstancePool().begin(); itor != g_MyD3D->m_pDamageManager->GetDamageInstancePool().end(); ++itor)
	{
		std::vector<CDamageInstance*>::iterator vit;
		for (vit = itor->second.begin(); vit != itor->second.end(); vit++)
		{
			CDamageInstance* pDamageOld = *vit;

			if (pDamageOld->m_HitWho == iPlayerIndex_)
			{
				if (pDamageOld->m_What == DT_HERO_KAMIKI_ATK3_2 ||
					pDamageOld->m_What == DT_SCORPIS_CRITICAL_INJECTION_STARTER ||
					pDamageOld->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE1_ATK3_2 ||
					pDamageOld->m_What == DT_EXTINCTION_MON_KAMIKI_PHASE2_ATK3_2
					)
				{
					pDamageOld->m_Life = 1;
				}
			}
		}
	}
}

bool KGCBuff::FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	if (m_bIsDramaSkip)
	{
		if (g_pkUIScene->m_pkGameOverlayUI->IsShowDialogue())
			return false;
	}

	if (pBuffInst_->m_fRemainTime <= 0.0f)
	{
		return false;
	}

	if (0 < pBuffInst_->m_iStartCoolTime)
	{
		--pBuffInst_->m_iStartCoolTime;
		if (0 == pBuffInst_->m_iStartCoolTime)
		{
			Init(pBuffInst_->m_sStartTimeBuff.iPlayerIndex_, pBuffInst_->m_sStartTimeBuff.pBuffInst_, pBuffInst_->m_sStartTimeBuff.fBuffTime,
				pBuffInst_->m_sStartTimeBuff.iIndex_, pBuffInst_->m_sStartTimeBuff.bIsCrashPlayer, pBuffInst_->m_sStartTimeBuff.dwGivePlayerUID);
		}

		return true;
	}

	if (!m_bIsBuffTimeStop)
	{
		pBuffInst_->m_fRemainTime -= GC_ELAPSED_TIME;

		// 2012.06.24, shmhlove : 좌우키 입력으로 버프시간을 감소시킨다.
		if (m_bIsCrashPlayer)
		{
			if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
			{
				PLAYER *pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];
				if (pPlayer &&
					(0.0f < m_fDecreaseBuffTime) &&
					pPlayer->IsLocalPlayer() &&
					(g_MyD3D->MyCtrl->k_Fresh_Left || g_MyD3D->MyCtrl->k_Fresh_Right))
				{
					pBuffInst_->m_fRemainTime -= m_fDecreaseBuffTime;
					if (0 >= pBuffInst_->m_fRemainTime)
					{
						pPlayer->ClearMagicEffect_NotMeBroad(pBuffInst_->m_iBuffEnum);
						return false;
					}
				}
			}
		}
	}

	// 2012.04.28, shmhlove : Loop사운드
	if (0 < int(pBuffInst_->m_vecLoopSound.size()))
	{
		std::vector< std::pair<DWORD, float> >::iterator vecIter = pBuffInst_->m_vecLoopSound.begin();
		for (int iLoop = 0; vecIter != pBuffInst_->m_vecLoopSound.end(); ++vecIter, ++iLoop)
		{
			if (iLoop >= int(m_vecLoopSound.size()))
				break;

			vecIter->second -= GC_ELAPSED_TIME;
			if (0.0f >= vecIter->second)
			{
				vecIter->second = m_vecLoopSound[iLoop].second;
				PlaySound(vecIter->first);
				printf("사운드재생 %d\n", vecIter->second);
			}
		}
	}

	if (pBuffInst_->m_fRemainTime <= 0.0f)
	{
		//====================================================================================
		// 2009.03.13 : Jemitgge
		// Comment : 버프 끝났는데 쿨타임이 있다! 그리고 쿨타임 한번도 안겪은 아이다
		//			 그럼다면 쿨타임을 적용시켜주쟈!
		if (m_iCoolTime > 0 && pBuffInst_->m_bPostCoolTime == false)
		{
			pBuffInst_->m_bPostCoolTime = true;
			pBuffInst_->m_fRemainTime = (float)m_iCoolTime;
			DestroyBuffEffect(iPlayerIndex_, pBuffInst_, false);
			return true;
		}
		// 쿨타임 따위 없다면 그냥 사라져!
		else
		{
			// 버프 죽으면서 힐 하는 아이면 힐 해 줍시다.
			if (m_bBuffEndHeal)
			{
				if (m_bIsCrashPlayer)
				{
					PLAYER* pHitPlayer = g_MyD3D->GetPlayerByUID(m_dwGivePlayerUID);
					if (pHitPlayer && pHitPlayer->IsDie() == false)
						pHitPlayer->SetHP(pHitPlayer->GetHP() + (pHitPlayer->GetPlayerMaxHP() * m_fHpDrain));
				}
				else
				{
					PLAYER* pPlayer = NULL;
					if (g_MyD3D->IsPlayerIndex(m_iPlayerIndex))
						pPlayer = g_MyD3D->MyPlayer[m_iPlayerIndex];
					else
						pPlayer = g_MyD3D->GetPlayerByUID(m_dwGivePlayerUID);

					if (pPlayer && pPlayer->IsDie() == false)
						pPlayer->SetHP(pPlayer->GetHP() + (pPlayer->GetPlayerMaxHP() * m_fHpDrain));
				}
			}
			Destroy(iPlayerIndex_, pBuffInst_, false, false);
			return false;
		}
	}

	if (m_iCountSuperFrame > 0) {
		m_iCountSuperFrame--;
	}

	if (m_iOriginalSuperFrame > 0) {
		m_iOriginalSuperFrame--;
	}

	if (m_bIsCrashPlayer
		&& (m_iOriginalSuperFrame > 0 || m_iCountSuperFrame > 0)
		&& (m_iSuper > 0 || m_iInitSuper > 0)
		&& g_MyD3D->IsPlayerIndex(iPlayerIndex_)) {

		if (m_iSuper > 0) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->Super += m_iSuper;
			m_iCountSuperFrame = m_iSuper;
		}

		if (m_iInitSuper > 0) {
			m_iOriginalSuperFrame = g_MyD3D->MyPlayer[iPlayerIndex_]->Super;
			g_MyD3D->MyPlayer[iPlayerIndex_]->Super = m_iInitSuper;
		}

		if (pBuffInst_->m_fRemainTime <= 0.0f)
		{
			if (m_iCoolTime > 0 && pBuffInst_->m_bPostCoolTime == false)
			{
				pBuffInst_->m_bPostCoolTime = true;
				pBuffInst_->m_fRemainTime = (float)m_iCoolTime;
				DestroyBuffEffect(iPlayerIndex_, pBuffInst_, false);
				return true;
			}
		}
	}

	if (pBuffInst_->m_bPostCoolTime)
		return true;

	if (m_bIsCrashPlayer)
	{
		// 버프가 걸린 시점의 모션에서만 허용하는 버프. 버프 걸린 시점의 모션이 아니면 버프는 사라진다.
		if (m_bAllowToOnlyOneMotion) {
			if (g_MyD3D->MyPlayer[iPlayerIndex_] &&
				m_iWhenActivatedMotion != g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion) {
				if (m_iCoolTime > 0 && pBuffInst_->m_bPostCoolTime == false) {
					pBuffInst_->m_bPostCoolTime = true;
					pBuffInst_->m_fRemainTime = (float)m_iCoolTime;
					DestroyBuffEffect(iPlayerIndex_, pBuffInst_, false);
					return false;
				}
				else {
					Destroy(iPlayerIndex_, pBuffInst_, false, false);
					return false;
				}
			}
		}

		FrameMovePlayerParticle(iPlayerIndex_, pBuffInst_);
		FrameMoveAnimation(iPlayerIndex_, pBuffInst_);

		if (m_bPetBuff)
		{
			if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject != NULL)
			{
				if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject->GetCartoon() == CARTOON_DEFAULT)
				{
					g_pMagicEffect->SetPetCartoon(iPlayerIndex_, m_eCartoonTexture);
				}
			}
		}

		if (m_nShaderType != -1) {
			g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, (EGCCartoonTexture)m_nShaderType);
		}

		if (m_bAntiPoison) {
			ResistDebuff(iPlayerIndex_);
		}

		if (m_bSuperArmor)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->SuperArmor(m_bSuperArmor);
			g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_ATHENS);
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetOutlineColor(EDGE_COLOR_SUPERARMOR);
		}

		if (m_iEdgeColor[0] != -1) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetOutlineColor(D3DXVECTOR4((float)(m_iEdgeColor[0] / 255), (float)(m_iEdgeColor[1] / 255), (float)(m_iEdgeColor[2] / 255), 1.0f));
		}

		if (m_bHyperArmor)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bHyperArmor = m_bHyperArmor;
		}

		if (m_bIsAttackMiss) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetAttackMiss(true);
		}

		if (m_fHpDrain != 0.0f)
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_bUsePower)
			{
				if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() != 0.0f) {
					PLAYER::DamageInfo damage;
					damage.SetDamage(g_MyD3D->MyPlayer[iPlayerIndex_]->m_fLastPower * m_fHpDrain);
					damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_PLAYER, iPlayerIndex_);
					g_MyD3D->MyPlayer[iPlayerIndex_]->Change_HP(damage);
				}
			}
		}

		if (!m_vecAttribute.empty())
		{
			if (g_MyD3D->Get_MyPlayer() == iPlayerIndex_)
			{
				if (m_vecAttribute[0].m_fIncMPRatio != 0.0f)
				{
					float fMPBuffRatio = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE];

					if (m_vecAttribute[0].m_fIncMPRatio == -1.f) {
						g_MyD3D->ChargeLocalPlayerMana(0.f, fMPBuffRatio);
					}
					else {
						g_MyD3D->ChargeLocalPlayerMana(m_vecAttribute[0].m_fIncMPRatio, fMPBuffRatio);
					}
				}
			}
		}

		for (unsigned int i = 0; i < m_vecExtraMesh.size(); ++i)
		{
			g_MyD3D->GetPlayer(iPlayerIndex_)->ToggleExtraMesh(m_vecExtraMesh[i].m_extraMesh.c_str(), true, m_vecExtraMesh[i].m_iMeshTech);
		}

		if (m_bMPRecoveryFix)
		{
			if (g_MyD3D->Get_MyPlayer() == iPlayerIndex_)
			{
				//  안걸려 있으면 무조껀 건다.
				if (!g_MyD3D->MyPlayer[iPlayerIndex_]->IsMPRecoveryFix())
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->SetMPRecoveryFix(true, m_fMPRecoveryFix);
				}
				else
				{
					//  효과가 더 좋을때만 새것으로 갱신한다.
					//  더 많이 깍기거나, 더 많이 올려주거나
					float fMPFix = g_MyD3D->MyPlayer[iPlayerIndex_]->GetMPRecoveryFixValue();

					if (fMPFix <= 0.0f && fMPFix > m_fMPRecoveryFix)
					{
						g_MyD3D->MyPlayer[iPlayerIndex_]->SetMPRecoveryFix(true, m_fMPRecoveryFix);
					}
					else if (fMPFix > 0.0f && fMPFix < m_fMPRecoveryFix)
					{
						g_MyD3D->MyPlayer[iPlayerIndex_]->SetMPRecoveryFix(true, m_fMPRecoveryFix);
					}
				}
			}
		}
	}
	else
	{
		if (m_bSuperArmor)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetSuperArmor(m_bSuperArmor);
		}

		if (CARTOON_NORMAL != m_eCartoonTexture)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetUserCartoon(m_eCartoonTexture);
		}

		if (m_iEdgeColor[0] != -1)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetUserEdgeColor((float)(m_iEdgeColor[0] / 255), (float)(m_iEdgeColor[1] / 255), (float)(m_iEdgeColor[2] / 255), 1.0f);
		}

		FrameMoveMonsterParticle(iPlayerIndex_ - MAX_PLAYER_NUM, pBuffInst_);

		for (unsigned int i = 0; i < m_vecExtraMesh.size(); ++i)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ToggleExtraMesh(m_vecExtraMesh[i].m_extraMesh.c_str(), true);
		}
	}

	FrameMoveContinousEffect(iPlayerIndex_);

	if (m_kBuffSummonMonster.m_eMonID != -1) {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(m_kBuffSummonMonster.m_iMonsterSlot);

		if (pMonster && g_MyD3D->MyPlayer[iPlayerIndex_]->m_bCheckDieSummons) {
			if (pMonster->GetHP() <= 0.f) {
				pBuffInst_->m_fRemainTime = 0.f;
			}
		}
	}

	if (m_bClearOnDie)
	{
		if (m_bIsCrashPlayer)
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsFatal() == false && 0.0f >= g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP())
				pBuffInst_->m_fRemainTime = GC_ELAPSED_TIME;
		}
		else
		{
			if (0.0f >= g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->GetHP())
				pBuffInst_->m_fRemainTime = GC_ELAPSED_TIME;
		}
	}

	return true;
}

bool KGCBuff::Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_)
{
	if (pBuffInst_->m_iLevel == 0)
	{
		return false;
	}

	// 스테이지를 넘어갈 때, 초기화 하지 않아도 되는 아이들을~ 넣어주쟈~
	if (m_bClearOnNextStage == false && bNextStage_ == true)
	{
		return false;
	}

	if (!m_bClearOnDie && bOnDie_)
	{
		return false;
	}

	// 예외처리( 여기에 걸리면 정상동작은 안하지만 크래시가 나지는 않습니다. )
	// 참 좆같은게 m_bIsCrashPlayer를 오리지날 버프객체가 들고 있기때문에
	// 같은버프ID를 캐릭터와 몬스터가 동시에 걸리면 크래시가 납니다.
	// 이거 수정하기 까다롭습니다. 많은 곳에 영향 있습니다.
	// 수정 한다면 버프로직 잘 파악해서 해요~ 저는 레이드 던전 작업때문에 지금 좀 많이 바쁨..
	if (m_bIsCrashPlayer)
	{
		if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
			return false;
	}
	else
	{
		if (false == g_kMonsterManager.IsMonsterIndex(iPlayerIndex_ - MAX_PLAYER_NUM))
			return false;
	}

	// StartCoolTime초기화
	pBuffInst_->m_iStartCoolTime = -1;

	// 버프가 제거될때 걸린 녀석을 죽인다.
	if (m_bIsKilling)
	{
		if (m_bIsCrashPlayer)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(0.0f);
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetLastKillmePlayer(MAX_PLAYER_NUM);
		}
		else
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetHP(0.0f);
		}
	}

	//====================================================================================
	// 2009.03.29 : Jemitgge
	// Comment : 쿨타임을 겪은 아이라면 이미 능력치는 원상 복귀 되어있다!! 건너뛰쟈 ㅠ
	if (pBuffInst_->m_bPostCoolTime)
		goto destroyBuff;

	if (g_MyD3D->IsPlayerIndex(iPlayerIndex_) == false)
		goto destroyBuff;

	//능력치 설정하는 부분
	if (m_bIsCrashPlayer)
	{
		if (m_bPetBuff)
		{
			FinalizePetAbility(iPlayerIndex_, pBuffInst_);
		}
		else
		{
			FinalizePlayerAbility(iPlayerIndex_, pBuffInst_);
		}
	}

	//속도 능력치 설정하는 부분
	if (m_bIsCrashPlayer)
	{
		FinalizePlayerSpeed(iPlayerIndex_, pBuffInst_);

		//카툰 벗겨!~
		if (m_eCartoonTexture != CARTOON_NORMAL && g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
		{
			if (g_pMagicEffect->GetCartoon(iPlayerIndex_) == m_eCartoonTexture)
			{
				g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_DEFAULT);
			}
		}

		if (m_nBerserkTime && pBuffInst_->m_fRemainTime <= 0) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(0.0f);
		}

		if (m_bAntiPoison) {
			g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_DEFAULT);
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetOutlineColor(EDGE_COLOR_FRIEND);
		}

		if (m_bSuperArmor)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->SuperArmor(false);

			if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo())
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo()->m_bSuperArmor = false;

			g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_DEFAULT);
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetOutlineColor(EDGE_COLOR_FRIEND);
		}

		if (m_iEdgeColor[0] != -1) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetOutlineColor(EDGE_COLOR_FRIEND);
		}

		if (m_bHyperArmor)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bHyperArmor = false;

			if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo())
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo()->m_bHyperArmor = false;
		}
	}
	else
	{
		if (0.0f != m_fMonsterWalkSpeed)
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetAddWalkSpeed(0.0f);
		if (0.0f != m_fMonsterRunSpeed)
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetAddRunSpeed(0.0f);

		if (m_bSuperArmor)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetSuperArmor(false);
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->m_pObject->SetOutlineColor(EDGE_COLOR_FRIEND);
		}
	}

	//====================================================================================
	// 2009.03.29 : Jemitgge
	// Comment : 있다면 삭제니까;; 또 들어와도 되겠지 머 ㅋㅋ
destroyBuff:
	//////////////////////////////////////////////////////////////////////////

	pBuffInst_->m_iLevel = 0;
	pBuffInst_->m_fRemainTime = 0.0f;

	// 애니메이션이 있다면 삭제
	FinalizeAnimation(pBuffInst_);

	//파티클이 있다면 삭제
	FinalizeParticle(iPlayerIndex_, pBuffInst_);

	//FinalizeMonsterStatus( iPlayerIndex_, pBuffInst_ );
	FinalizeMonsterStatus(iPlayerIndex_ - MAX_PLAYER_NUM, pBuffInst_);

	pBuffInst_->m_bPostCoolTime = false;

	if (g_MyD3D->IsPlayerIndex(iPlayerIndex_))
	{
		if (m_iItemEff != -1)
		{
			if (pBuffInst_->m_fReserve1 == 1.0f)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetUseItemEffect().erase((EItemEffect)m_iItemEff);
				pBuffInst_->m_fReserve1 = 0.0f;
			}
		}

		// 데미지 반사가 있었다면
		if (0.0f != m_fReflectionDamageRatio)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIsCountAttackShield = false;
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fReflectionDamageRatio = 0.0f;
		}

		// MP공격 설정
		if ((0.0f != m_fDamageToMPAttack) || (0.0f != m_fStaticToMPAttack))
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fDamageToMPAttack = 0.0f;
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fStaticToMPAttack = 0.0f;
		}
	}
	// AP 사용금지 명령을 풀어준다.
	if (m_bUseNotAP)
		g_MyD3D->MyPlayer[iPlayerIndex_]->SetUseNotAP(false);
	// KeyLock을 풀어준다.
	if (m_bKeyLock && g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		g_MyD3D->SetEnableKeyInput(true);

	// Render를 Lock을 푼다.
	if (m_bCharLock)
		g_MyD3D->MyPlayer[iPlayerIndex_]->SetLock(false);

	// 캐릭터 모션 Lock을 푼다.
	if (m_bCharMotionLock)
		g_MyD3D->MyPlayer[iPlayerIndex_]->SetMotionLock(false);

	if (false == m_bIsCrashPlayer)
	{
		if (0 < int(m_vecMonsterTraceIcon.size()))
		{
			int iMonsterType = g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->GetMonsterType();
			const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[iMonsterType];
			if ("" != kMonsterTemplate.sTraceIcon.strSqu_1)
				g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ChangeTraceIcon(const_cast<char*>(kMonsterTemplate.sTraceIcon.strSqu_1.c_str()), 0);
			else
				g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ChangeTraceIcon("M001", 0);

			if ("" != kMonsterTemplate.sTraceIcon.strSqu_2)
				g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ChangeTraceIcon(const_cast<char*>(kMonsterTemplate.sTraceIcon.strSqu_2.c_str()), 1);
			else
				g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ChangeTraceIcon("M002", 1);

			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetTraceIconExistScreen(kMonsterTemplate.sTraceIcon.bIsMyExistScreen);
		}
	}

	if (m_kBuffUIEvent.bEndCall)
	{
		g_pkUIMgr->SendEvent(KActionEvent(NULL, m_kBuffUIEvent.m_dwUIEventCode), false);
	}

	if (m_bIsCrashPlayer)
	{
		if (m_bSpecialDisable && g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			g_pkUIScene->m_pkGameOverlayUI->GetHeadUp()->m_pMyHeadup->ToggleBlackGauge(false);
		}

/*		if (m_iSuper > 0) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->Super -= m_iCountSuperFrame;
			if (0 > g_MyD3D->MyPlayer[iPlayerIndex_]->Super)
				g_MyD3D->MyPlayer[iPlayerIndex_]->Super = 0;
		}

		if (m_iInitSuper > 0) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->Super = m_iOriginalSuperFrame;
			if (0 > g_MyD3D->MyPlayer[iPlayerIndex_]->Super)
				g_MyD3D->MyPlayer[iPlayerIndex_]->Super = 0;
		}*/

		m_iCountSuperFrame = 0;
		m_iOriginalSuperFrame = 0;

		if (m_dwMeshState > 0) {
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject) {
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->MeshStateOff(m_dwMeshState);
			}
		}
	}

	if (m_kBuffSummonMonster.m_eMonID != -1 && g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer()) {
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.bHost == true) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetSummonMonsterHP(0.f, m_kBuffSummonMonster.m_iMonsterSlot, false, m_kBuffSummonMonster.m_eMonID);
		}
		else {
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetSummonMonsterHP_SendHost(0.f, m_kBuffSummonMonster.m_iMonsterSlot, false, static_cast<int>(m_kBuffSummonMonster.m_eMonID));
		}
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bCheckDieSummons = false;
		m_kBuffSummonMonster.m_iMonsterSlot = -1;
	}

	if (false == m_bIsCrashPlayer)
	{
		if (CARTOON_NORMAL != m_eCartoonTexture)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetUserCartoon(-1);
		}

		if (m_iEdgeColor[0] != -1)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetUserEdgeColor(-1.0, -1.0, -1.0, -1.0);
		}
	}

	if (false == m_vecPermissionDamage.empty())
	{
		if (m_bIsCrashPlayer)
			g_MyD3D->MyPlayer[iPlayerIndex_]->ClearPermissionDamage();
		else
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ClearPermissionDamage();
	}

	for (unsigned int i = 0; i < m_vecExtraMesh.size(); ++i)
	{
		if (m_bIsCrashPlayer)
			g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleExtraMesh(m_vecExtraMesh[i].m_extraMesh.c_str(), false, m_vecExtraMesh[i].m_iMeshTech);
		else
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->ToggleExtraMesh(m_vecExtraMesh[i].m_extraMesh.c_str(), false);
	}

	if (m_bIsCrashPlayer && m_iChangeTeam != -1) {
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_iOriginalTeam != g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.iTeam) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.SetTeam(g_MyD3D->MyPlayer[iPlayerIndex_]->m_iOriginalTeam);
		}
	}

	if (m_bIsCrashPlayer && m_bNoCrashCheck) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_bNoCrashCheckByBuff = false;
	}

	if (!m_vecSecondBaseMesh.empty() && IsSceondBaseMeshEnable(g_MyD3D->MyPlayer[iPlayerIndex_], true)) {
		if (!g_MyD3D->MyPlayer[iPlayerIndex_]->IsMagicEffect(EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK)) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleBaseMeshStatic(true);
		}

		for (std::vector< std::string >::const_iterator itSecondBase = m_vecSecondBaseMesh.begin(); itSecondBase != m_vecSecondBaseMesh.end(); ++itSecondBase) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->ToggleSecondBaseMesh(itSecondBase->c_str(), false);
		}
	}

	if (m_bNoRenderPet) {
		g_MyD3D->m_akPet[iPlayerIndex_]->TogglePetRender(m_bNoRenderPet);
	}

	return true;
}

GCDeviceTexture* KGCBuff::GetHudTexture(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_vecpHudTex.size() || nIndex >= (int)m_vecHudTexName.size())
		return  NULL;

	if (!m_vecpHudTex[nIndex])
	{
		m_vecpHudTex[nIndex] = g_pGCDeviceManager2->CreateTextureThread(m_vecHudTexName[nIndex], false, true);
	}

	return m_vecpHudTex[nIndex];
}

GCDeviceTexture* KGCBuff::GetWaitHudTexture(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_vecpWaitHudTex.size() || nIndex >= (int)m_vecWaitHudTexName.size())
		return  NULL;

	if (!m_vecpWaitHudTex[nIndex])
	{
		m_vecpWaitHudTex[nIndex] = g_pGCDeviceManager2->CreateTextureThread(m_vecWaitHudTexName[nIndex], false, true);
	}

	return m_vecpWaitHudTex[nIndex];
}

GCDeviceTexture* KGCBuff::GetHudTexture(KGCBuffInstance* pBuffInst_)
{
	//ZSTAIJI - 오후 3:08 2009-10-15 라스 분신술 사용시 공격한 상대방이 크래시 나는 문제 해결위해 이 함수 내용을 갈아엎음.

	if (pBuffInst_->m_fRemainTime <= 0.0f || pBuffInst_->m_iLevel < 1)
	{
		return NULL;
	}

	if (true == pBuffInst_->m_bPostCoolTime)
	{
		if (pBuffInst_->m_iLevel - 1 < (int)m_vecpWaitHudTex.size())
		{
			return GetWaitHudTexture(pBuffInst_->m_iLevel - 1);
		}
	}
	else
	{
		if (pBuffInst_->m_iLevel - 1 < (int)m_vecpHudTex.size())
		{
			return GetHudTexture(pBuffInst_->m_iLevel - 1);
		}
	}

	if (true == pBuffInst_->m_bPostCoolTime)
	{
		if (m_vecpWaitHudTex.empty() == false)
		{
			return GetWaitHudTexture(0);
		}
	}
	else
	{
		if (m_vecpHudTex.empty() == false)
		{
			return GetHudTexture(0);
		}
	}

	return NULL;
}

std::wstring KGCBuff::GetHudString(KGCBuffInstance* pBuffInst_, int iStringIndex_)
{
	//====================================================================================
	// 2009.03.13 : Jemitgge
	// Comment : 쿨타임이면 쿨타임꺼를~ 아니면 원래꺼를 ㅎㅎ 쿨타임 스트링 하나만 쓰려나?;;
	if (true == pBuffInst_->m_bPostCoolTime)
		return m_strWaitHudString[iStringIndex_];
	else
		return m_strHudString[iStringIndex_];
}

int KGCBuff::GetHudStringNum(KGCBuffInstance* pBuffInst_)
{
	if (true == pBuffInst_->m_bPostCoolTime)
		return m_strWaitHudString.size();
	else
		return m_strHudString.size();
}

void KGCBuff::PushHudTexture(GCDeviceTexture* pTexture)
{
	m_vecpHudTex.push_back(pTexture);
}

void KGCBuff::PushHudString(std::wstring strHudText)
{
	m_strHudString.push_back(strHudText);
}

void KGCBuff::PushWaitHudTexture(GCDeviceTexture* pTexture)
{
	m_vecpWaitHudTex.push_back(pTexture);
}

void KGCBuff::PushWaitHudString(std::wstring strHudText)
{
	m_strWaitHudString.push_back(strHudText);
}

void KGCBuff::HiddenParticle(bool bIsBuffTimeStop, bool bIsHiddenParticle)
{
	m_bIsBuffTimeStop = bIsBuffTimeStop;
	m_bIsHiddenParticle = bIsHiddenParticle;
}

void KGCBuff::InitPlayerAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (iLevelIndex >= m_vecAttribute.size())
	{
		iLevelIndex = m_vecAttribute.size() - 1;
	}

	if (m_fCatchResist > 0.f) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CATCH_DAMAGE_RESIST] += (m_fCatchResist);
	}

	if (m_bFatalSuperArmor) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SUPER_ARMOR_BUFF] += 1.f;
	}

	if (m_fMissileDamageResist > 0.f) {
		m_fOriMissileDamageResist = g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_MISSILE_DAMAGE_RESIST];
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_MISSILE_DAMAGE_RESIST] = m_fMissileDamageResist;
	}

	if (!m_vecAttribute.empty() && m_bIsCrashPlayer)
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fIncHPRatio += m_vecAttribute[iLevelIndex].m_fIncHPRatio;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_ATTACK_PERCENT] += m_vecAttribute[iLevelIndex].m_fIncATK;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEFENCE_PERCENT] += m_vecAttribute[iLevelIndex].m_fIncDEF;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] += m_vecAttribute[iLevelIndex].m_fCriticalRate;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] *= m_vecAttribute[iLevelIndex].m_fCriticalRateMultiply;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] += m_vecAttribute[iLevelIndex].m_fCriticalDamageUp;

		if (m_vecAttribute[iLevelIndex].m_bUnFatal)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIncHP = true;
		}

		if (0.0f < m_vecAttribute[iLevelIndex].m_fIncHP)
		{
			pBuffInst_->m_fPrevFullHPInstance = g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP];
		}
		float fBefoerHP = g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP];
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] += (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] * m_vecAttribute[iLevelIndex].m_fIncHP);

		if (false == g_MyD3D->MyPlayer[iPlayerIndex_]->IsWeaponChanging()) {
			// 더러운 무기채인지!!! 피 올려주지 마 !!
			if ((false == g_MyD3D->MyPlayer[iPlayerIndex_]->IsWeaponChanging() && fBefoerHP <= g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP()) &&
				// 더러운 피통증가ㅠㅠ 죄송
				(EGC_EFFECT_ZERO2_INCREASE_DS != pBuffInst_->m_iBuffEnum &&
					EGC_EFFECT_ZERO2_INCREASE_DS_LV1 != pBuffInst_->m_iBuffEnum &&
					EGC_EFFECT_ZERO2_INCREASE_DS_LV2 != pBuffInst_->m_iBuffEnum))
			{
				//생명최대치를 늘린만큼만 HP를 올려준다.
				if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] < g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] * m_vecAttribute[iLevelIndex].m_fIncHP))
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP]);
				}
				else
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(
						g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() +
						(g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] * m_vecAttribute[iLevelIndex].m_fIncHP)
					);
				}
			}
		}

		//if( SiKGCRoomManager()->IsMonsterGameMode())
		//	g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIncHP = true;
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			if (m_vecAttribute[iLevelIndex].m_fMPRecoveryRate != 0.0f)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE] += m_vecAttribute[iLevelIndex].m_fMPRecoveryRate;
			}

			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE] += m_vecAttribute[iLevelIndex].m_fIncMPRatio;

			m_fOriMPDecreaseRatio = g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatio;
			// 원래 값과 버프 값 중 작은 녀석을 선택 하겠다.
			if (m_fOriMPDecreaseRatio <= m_vecAttribute[iLevelIndex].m_fMPDecreaseRatio) {
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatio = m_fOriMPDecreaseRatio;
			}
			else {
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatio = m_vecAttribute[iLevelIndex].m_fMPDecreaseRatio;
			}

			if (m_vecAttribute[iLevelIndex].m_fMinusAPCoolTime != 0.0f)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMinusAPCoolTime += m_vecAttribute[iLevelIndex].m_fMinusAPCoolTime;
			}

			if (m_vecAttribute[iLevelIndex].m_fMPDecreaseRatioForSpecial != 1.0f)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatioForSpecial = m_vecAttribute[iLevelIndex].m_fMPDecreaseRatioForSpecial;
			}
		}

		float fRecoveryHP = g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] * (m_vecAttribute[iLevelIndex].m_fHPRecoveryRate / 100.0f);
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() < g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP])
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + fRecoveryHP);
	}

	if (m_bIsCrashPlayer)
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer()) {
			if (m_fChangeMpRatio != 0.f) {
				m_fOriIncMpRatio = g_MyD3D->m_fIncMPRatio * m_fChangeMpRatio;
				g_MyD3D->m_fIncMPRatio += g_MyD3D->m_fIncMPRatio * m_fChangeMpRatio;
			}
		}
	}
}

void KGCBuff::InitAnimation(KGCBuffInstance* pBuffInst_)
{
	for (unsigned int i = 0; i < m_vecAnim.size(); i++)
	{
		CKTDGAnim::CAnimInstance* pAnim;

		pAnim = g_MyD3D->m_pMapAnim->CreateInst(
			g_MyD3D->m_pMapAnim->GetSequence(m_vecAnim[i].m_strAnim));

		if (pAnim != NULL)
		{
			pAnim->GetMatrix()->Scale(m_vecAnim[i].m_fAnimScaleX, m_vecAnim[i].m_fAnimScaleY, 1.0f);
			pAnim->SetShow(true);
			pAnim->Start(CKTDGAnim::PT_LOOP);

			pBuffInst_->m_vecAnim.push_back(pAnim);
		}
	}
}

void KGCBuff::InitParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime)
{
	if (m_bPetBuff)
	{
		InitPetParticle(iPlayerIndex_, pBuffInst_, fBuffTime);
	}
	else
	{
		InitPlayerParticle(iPlayerIndex_, pBuffInst_, fBuffTime);
	}
}

void KGCBuff::InitPlayerSpeed(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (iLevelIndex >= (int)m_vecSpeedAttribute.size())
	{
		iLevelIndex = m_vecSpeedAttribute.size() - 1;
	}

	if (!m_vecSpeedAttribute.empty())
	{
		BuffSpeedAttribute& attr = m_vecSpeedAttribute[iLevelIndex];
		SetPlayerSpeed(pBuffInst_, iPlayerIndex_, pBuffInst_->m_iBuffEnum, attr, 1);
	}
}

void KGCBuff::InitMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iWho)
{
	//	if( SiKGCRoomManager()->IsMonsterGameMode() == false )
	//		return;

	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (!m_vecMonAttribute.empty())
	{
		g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fAtkRatio *= m_vecMonAttribute[iLevelIndex].m_fMonsterAPRate;
		g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fDefRatio *= m_vecMonAttribute[iLevelIndex].m_fMonsterDPRate;
		g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fSDefRatio *= m_vecMonAttribute[iLevelIndex].m_bMonsterSDPRate;

		if (m_vecMonAttribute[iLevelIndex].m_bMonsterAggro) {
			g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_bAggroState = true;
			g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_iTargetPlayerIndex = iWho;
		}
	}

	// 몬스터 NoTargetting 설정
	if (0 < m_iMonsterNoTargetingCnt)
	{
		g_kMonsterManager.m_mapMonster[iPlayerIndex_]->SetNoTargettingFrame(m_iMonsterNoTargetingCnt);
	}

	if (m_bMonsterFrameLock)
	{
		g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_bBuffFrameLock = true;
	}

	if (m_iMonsterStopSpeed)
	{
		g_kMonsterManager.m_mapMonster[iPlayerIndex_]->SetStopSpeedFrame(m_iMonsterStopSpeed);
	}

	for (unsigned int j = 0; j < m_vecParticle.size(); j++)
	{
		if (m_vecParticle[j].m_iType != -1 && m_vecParticle[j].m_iType != (int)g_kMonsterManager.m_mapMonster[iPlayerIndex_]->GetMonsterType())
			continue;

		float fHeight = 0.0f;

		// 몬스터의 머리위로 아이콘이 생성될 수 있도록 설정
		if (!g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_iBossMonster)
		{
			fHeight = m_vecParticle[j].m_fPosY + g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fAttackBoundBoxHeight * 0.5f;
		}
		else
		{
			// 보스몹은 아이콘 위치가 정말 애매해서 매직넘버가 곱해졌음 ㅜ
			fHeight = m_vecParticle[j].m_fPosY + g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fAttackBoundBoxHeight * 1.05f;
		}

		if (false == m_vecParticle[j].m_bLocalEffect)
		{
			if (m_bNotDependenceFromModel)
			{
				CParticleEventSeqPTR pParticle;
				pParticle = g_ParticleManager->CreateSequence(m_vecParticle[j].m_strParticle);
				if (pParticle != NULL)
				{
					TRACE_INFO stTrace;
					stTrace.m_pvPos = &g_kMonsterManager.m_mapMonster[iPlayerIndex_]->vRenderPos;
					stTrace.m_fXOffset = m_vecParticle[j].m_fPosX;
					stTrace.m_fYOffset = m_vecParticle[j].m_fPosY;

					if (0.0f < m_vecParticle[j].m_fTraceTime)
						stTrace.m_fParticleTraceTime = m_vecParticle[j].m_fTraceTime;
					else
						stTrace.m_fParticleTraceTime = fBuffTime;
					pParticle->SetTrace(&stTrace);

					if (!m_vecParticle[j].m_bIsNoDirection)
					{
						DIRECTION_INFO stDir;
						stDir.m_pbIsRight = &g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_abIsRight[g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0];
						stDir.m_fDirPosX = m_vecParticle[j].m_fDirPosX;
						pParticle->SetDirectInfo(&stDir);
					}

					pParticle->SetShow(true);
					pBuffInst_->m_vecParticle.push_back(pParticle);
				}
			}
			else
			{
				MSG_PARTICE_CREATE_INFO msg;
				msg.strSeqname = m_vecParticle[j].m_strParticle;
				msg.vPos.x = m_vecParticle[j].m_fPosX;
				msg.vPos.y = fHeight;

				if (0.0f < m_vecParticle[j].m_fTraceTime)
					msg.fTraceTime = m_vecParticle[j].m_fTraceTime;
				else
					msg.fTraceTime = fBuffTime;
				msg.bDirecitonal = false;
				msg.pTracePos = &g_kMonsterManager.m_mapMonster[iPlayerIndex_]->vRenderPos;

				pBuffInst_->m_vecParticle.push_back(g_kMonsterManager.m_mapMonster[iPlayerIndex_]->CreateParticle(msg));
			}
		}
	}
}

bool KGCBuff::InitGuildBuff(int iPlayerIndex_)
{
	if (!m_bGuildMemberDepend)
		return true;

	bool bGuildCheck = false;
	for (int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		if (i == iPlayerIndex_)
			continue;

		if (g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_PLAYING) ||
			g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState(GC_RUS_READY))
		{
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_kUserInfo.iGuildID == g_MyD3D->MyPlayer[i]->m_kUserInfo.iGuildID)
			{
				bGuildCheck = true;
			}
		}
	}

	if (!bGuildCheck)
	{
		return false;
	}

	return true;
}

void KGCBuff::FinalizePlayerAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (iLevelIndex >= m_vecAttribute.size())
	{
		iLevelIndex = m_vecAttribute.size() - 1;
	}

	if (m_bIsCrashPlayer) {
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			if (m_fChangeMpRatio != 0.f) {
				g_MyD3D->m_fIncMPRatio -= m_fOriIncMpRatio;
				m_fOriIncMpRatio = 0.f;
			}
		}
	}

	if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsWeaponChanging() || g_MyD3D->MyPlayer[iPlayerIndex_]->IsOnGameItemChange())
		return;

	if (m_fCatchResist > 0.f) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CATCH_DAMAGE_RESIST] -= (m_fCatchResist);

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CATCH_DAMAGE_RESIST] < 0.f) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CATCH_DAMAGE_RESIST] = 0.f;
		}
	}

	if (m_bFatalSuperArmor) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SUPER_ARMOR_BUFF] -= 1.f;

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SUPER_ARMOR_BUFF] < 0.f) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SUPER_ARMOR_BUFF] = 0.f;
		}
	}

	if (m_bIsAttackMiss) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->SetAttackMiss(false);
	}

	if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer() && m_bMPRecoveryFix) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->SetMPRecoveryFix(false, 0.0f);
	}

	if (m_fMissileDamageResist > 0.f) {
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_MISSILE_DAMAGE_RESIST] = m_fOriMissileDamageResist;
	}

	if (!m_vecAttribute.empty() && m_bIsCrashPlayer)
	{
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fIncHPRatio -= m_vecAttribute[iLevelIndex].m_fIncHPRatio;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_ATTACK_PERCENT] -= m_vecAttribute[iLevelIndex].m_fIncATK;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEFENCE_PERCENT] -= m_vecAttribute[iLevelIndex].m_fIncDEF;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] -= m_vecAttribute[iLevelIndex].m_fCriticalRate;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE] /= m_vecAttribute[iLevelIndex].m_fCriticalRateMultiply;
		g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] -= m_vecAttribute[iLevelIndex].m_fCriticalDamageUp;

		if (0.0f < pBuffInst_->m_fPrevFullHPInstance) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] = pBuffInst_->m_fPrevFullHPInstance;
		}

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsWeaponChanging() == false) {
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] < g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP())
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetHPFull();
		}

		if (m_vecAttribute[iLevelIndex].m_bUnFatal)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIncHP = true;
		}

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			if (m_vecAttribute[iLevelIndex].m_fMPRecoveryRate != 0.0f)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE] -= m_vecAttribute[iLevelIndex].m_fMPRecoveryRate;
			}

			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE] -= m_vecAttribute[iLevelIndex].m_fIncMPRatio;

			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatio = m_fOriMPDecreaseRatio;

			if (m_vecAttribute[iLevelIndex].m_fMinusAPCoolTime != 0.0f)
			{
				//깍아줄 쿨타임이 (유저기본값이) 0보다 큰 경우만 깎아 주자
				if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMinusAPCoolTime > 0)
					g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMinusAPCoolTime -= m_vecAttribute[iLevelIndex].m_fMinusAPCoolTime;
			}

			if (m_vecAttribute[iLevelIndex].m_fMPDecreaseRatioForSpecial != 1.0f)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatioForSpecial = 1.0f;
			}
		}
	}
}

void KGCBuff::FinalizePlayerSpeed(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (iLevelIndex >= (int)m_vecSpeedAttribute.size())
	{
		iLevelIndex = m_vecSpeedAttribute.size() - 1;
	}

	if (!m_vecSpeedAttribute.empty())
	{
		BuffSpeedAttribute& attr = m_vecSpeedAttribute[iLevelIndex];
		SetPlayerSpeed(pBuffInst_, iPlayerIndex_, pBuffInst_->m_iBuffEnum, attr, -1);
	}
}

void KGCBuff::FinalizeAnimation(KGCBuffInstance* pBuffInst_)
{
	for (std::vector<CKTDGAnim::CAnimInstance*>::iterator vit = pBuffInst_->m_vecAnim.begin(); vit != pBuffInst_->m_vecAnim.end(); vit++)
	{
		g_MyD3D->m_pMapAnim->DeleteInst(*vit);
		(*vit) = NULL;
	}
	pBuffInst_->m_vecAnim.clear();
}

void KGCBuff::FinalizeParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	for (std::vector<CParticleEventSeqPTR>::iterator vit = pBuffInst_->m_vecParticle.begin(); vit != pBuffInst_->m_vecParticle.end(); vit++)
	{
		g_ParticleManager->DeleteSequence(*vit);
		(*vit) = NULL;
	}
	pBuffInst_->m_vecParticle.clear();
}

void KGCBuff::FinalizeMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (iLevelIndex >= (int)m_vecMonAttribute.size())
	{
		iLevelIndex = m_vecMonAttribute.size() - 1;
	}

	if (m_bMonsterFrameLock)
	{
		g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_bBuffFrameLock = false;
	}

	if (!m_vecMonAttribute.empty())
	{
		BuffMonsterAttribute& attr = m_vecMonAttribute[iLevelIndex];

		//      if( SiKGCRoomManager()->IsMonsterGameMode() )
		if (!m_bIsCrashPlayer && g_kMonsterManager.IsMonsterIndex(iPlayerIndex_))
		{
			g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fAtkRatio /= attr.m_fMonsterAPRate;
			g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fDefRatio /= attr.m_fMonsterDPRate;
			g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_fSDefRatio /= attr.m_bMonsterSDPRate;
			if (attr.m_bMonsterAggro) {
				g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_bAggroState = false;
				g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_iTargetPlayerIndex = -1;
			}
		}
	}
}

void KGCBuff::FrameMoveAnimation(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	for (unsigned int i = 0; i < pBuffInst_->m_vecAnim.size(); i++)
	{
		// 위치 갱신
		float fX = 0.0f;
		float fY = 0.0f;

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			fX = g_kLocalPlayerLatency->kRenderData[0].vPos.x;
			fY = g_kLocalPlayerLatency->kRenderData[0].vPos.y;
		}
		else
		{
			fX = g_MyD3D->MyPlayer[iPlayerIndex_]->vPos.x;
			fY = g_MyD3D->MyPlayer[iPlayerIndex_]->vPos.y;
		}

		pBuffInst_->m_vecAnim[i]->GetMatrix()->Move(fX + m_vecAnim[i].m_fAnimPosX, fY + m_vecAnim[i].m_fAnimPosY, 1.0f);

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_AMY3_DANCE_CATCHED ||
			g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_AMY3_CATCHED ||
			g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_CATCH_BY_RYAN4 ||
			(g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject != NULL && g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->IsInvisible()))
		{
			pBuffInst_->m_vecAnim[i]->SetShow(false);
		}
		else
		{
			pBuffInst_->m_vecAnim[i]->SetShow(g_MyD3D->MyPlayer[iPlayerIndex_]->m_bRender);
		}
	}
}

void KGCBuff::FrameMovePlayerParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	for (unsigned int i = 0; i < pBuffInst_->m_vecParticle.size(); i++)
	{
		if (g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_AMY3_DANCE_CATCHED ||
			g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_AMY3_CATCHED ||
			g_MyD3D->MyPlayer[iPlayerIndex_]->uiMotion == MID_COMMON_CATCH_BY_RYAN4 ||
			(g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject != NULL && g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->IsInvisible()))
		{
			if (pBuffInst_->m_vecParticle[i])
			{
				pBuffInst_->m_vecParticle[i]->SetShow(false);
			}
		}
		else
		{
			if (pBuffInst_->m_vecParticle[i])
			{
				if (m_bIsHiddenParticle)
					pBuffInst_->m_vecParticle[i]->SetShow(false);
				else
					pBuffInst_->m_vecParticle[i]->SetShow(m_bNotDependenceFromModel || g_MyD3D->MyPlayer[iPlayerIndex_]->m_bRender);
			}
		}
	}
}

void KGCBuff::FrameMoveMonsterParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	for (unsigned int j = 0; j < pBuffInst_->m_vecParticle.size(); j++)
	{
		if (!pBuffInst_->m_vecParticle[j])
			continue;

		pBuffInst_->m_vecParticle[j]->SetShow(m_bNotDependenceFromModel || g_kMonsterManager.m_mapMonster[iPlayerIndex_]->m_bRender);

		for (unsigned int i = 0; i < m_vecParticle.size(); ++i)
		{
			if (m_vecParticle[i].m_strParticle == pBuffInst_->m_vecParticle[j]->GetName() && m_vecParticle[i].m_fPosX != 0.0f)
			{
				MONSTER* pMonster = g_kMonsterManager.m_mapMonster[iPlayerIndex_];
				float fDirection = pMonster->GetIsRight() ? 1.0f : -1.0f;
				TRACE_INFO* tmpTrace = pBuffInst_->m_vecParticle[j]->GetTrace();
				tmpTrace->m_fXOffset = m_vecParticle[i].m_fPosX * fDirection;
				tmpTrace->m_fYOffset = m_vecParticle[i].m_fPosY;
				pBuffInst_->m_vecParticle[j]->SetTrace(tmpTrace);
				//D3DXVECTOR3 vecPos = pBuffInst_->m_vecParticle[j]->GetPosition();
				//pBuffInst_->m_vecParticle[j]->SetPosition( (pMonster->GetX() - 1.0f) + (m_vecParticle[i].m_fPosX * fDirection), vecPos.y, vecPos.z );
			}
		}
	}
}

void KGCBuff::FrameMoveContinousEffect(int iPlayerIndex_)
{
	if (m_bIsCrashPlayer)
	{
		if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
			return;
	}
	else
	{
		if (false == g_kMonsterManager.IsMonsterIndex(iPlayerIndex_ - MAX_PLAYER_NUM))
			return;
	}

	static bool IsHit = false;
	if (m_bEnableContinousHeal)
	{
		IsHit = false;
		switch (m_iHealPerFrame)
		{
		case 3:
			if (!o3) IsHit = true;
			break;
		case 5:
			if (!o5) IsHit = true;
			break;
		case 10:
			if (!o10) IsHit = true;
			break;
		default:
			if (!o10) IsHit = true;
			break;
		}

		// 플레이어 페이탈 처리
		if (m_bIsCrashPlayer)
		{
			if (IsHit &&
				g_MyD3D->MyPlayer[iPlayerIndex_]->IsFatal() == false &&
				g_MyD3D->MyPlayer[iPlayerIndex_]->IsDie() == false)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(
					g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + m_fHealToPlayer);

				IsHit = false;
			}
		}
	}

	if (m_bEnableContinousDamage)
	{
		IsHit = false;
		if (SiKGCRoomManager()->IsMonsterGameMode())
		{
			switch (m_iHitPerFrame)
			{
			case 3:
				if (!o3) IsHit = true;
				break;
			case 5:
				if (!o5) IsHit = true;
				break;
			case 10:
				if (!o10) IsHit = true;
				break;
			case 30:
				if (!o30) IsHit = true;
				break;
			default:
				if (!o10) IsHit = true;
				break;
			}

			if (m_bIsCrashPlayer)  //  플레이어의 경우는 예외 처리를 해줘야 한다. 안그러면 버프 넣다가 크래쉬
			{
				if (IsHit)
				{
					if (0.0f != m_fDamageToPlayerMP)
						g_MyD3D->MyPlayer[iPlayerIndex_]->DecreaseMP(m_fDamageToPlayerMP);

					if (m_fDamageToPlayer)
					{
						if ((g_MyD3D->MyPlayer[iPlayerIndex_]->GetPlayerState() != PS_DEAD) && (g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() > 0.0f))
						{
#if defined( LEVEL_DESIGN_STAT_CALC ) 
							g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + m_fDamageToPlayer);
#else
							g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + m_fDamageToPlayer * 0.01f);
#endif
							g_MyD3D->MyHead->Add_HP_Changer(iPlayerIndex_, iPlayerIndex_, m_fDamageToPlayer*(-1.0f), false, "", 0);
						}
					}
					else if (m_fAddDamageToPlayer)
					{
						PLAYER *pPlayer = g_MyD3D->GetPlayerByUID(m_dwGivePlayerUID);
						if (pPlayer) {
							PLAYER::DamageInfo damage;
							damage.SetDamage(m_fAddDamageToPlayer);
							damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_PLAYER, pPlayer->m_iPlayerIndex);
							damage.SetSpecialAttack(m_bIsSpecialAtk, 0);
							g_MyD3D->MyPlayer[iPlayerIndex_]->Change_HP(damage);
						}
						else {
							if (m_iPlayerIndex != -1 && g_MyD3D->IsPlayerIndex(m_iPlayerIndex)) {
								PLAYER::DamageInfo damage;
								damage.SetDamage(m_fAddDamageToPlayer);
								damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_MONSTER, m_iPlayerIndex);
								damage.SetSpecialAttack(m_bIsSpecialAtk, 0);
								g_MyD3D->MyPlayer[iPlayerIndex_]->Change_HP(damage);
							}
						}
					}
					else if (m_fCureDamage)
					{
						float fCureDamage = ASMfabs(m_fCureDamage);
						g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + fCureDamage);
						g_MyD3D->MyHead->Add_HP_Changer(iPlayerIndex_, iPlayerIndex_, fCureDamage, false, "", 0, 0.0f, 0.0f, Headup::EDAMAGE_TEXT_BLUE);
					}
					IsHit = false;
				}
			}
			else
			{
				// 몹 피깍게 하는건 방장님만 해야지 -_-;;
				if (IsHit)
				{
					int iMonsterIndex = iPlayerIndex_ - MAX_PLAYER_NUM;
					if (m_fDamageToMonster)
					{
						if (g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost) {
							float fDamageToMonster = ASMfabs(m_fDamageToMonster);
							if (false == g_kMonsterManager.m_mapMonster[iMonsterIndex]->IsInvincible()) {
								g_kMonsterManager.m_mapMonster[iMonsterIndex]->ChangeHP(NULL, fDamageToMonster * 60.0f, m_iPlayerIndex, m_bIsSpecialAtk, true, 0);
								g_kMonsterManager.m_mapMonster[iMonsterIndex]->SetAttackedMonster(true);
								g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);  // 요거 안해주면 동우수옥탄에서 해킹체크가 걸려영..
							}
						}
					}
					else if (m_fAddDamageToMonster)
					{
						if (g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost) {
							float fDamageToMonster = ASMfabs(m_fAddDamageToMonster);
							if (g_MyD3D->IsPlayerIndex(m_iPlayerIndex))
							{
								PLAYER* pPlayer = g_MyD3D->MyPlayer[m_iPlayerIndex];
								if (pPlayer)
								{
									// 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
									g_kMonsterManager.m_mapMonster[iMonsterIndex]->m_vDamageStartPos = D3DXVECTOR2(pPlayer->GetX(), pPlayer->GetY());
									g_kMonsterManager.m_mapMonster[iMonsterIndex]->m_vDamageWhoPos = D3DXVECTOR2(pPlayer->GetX(), pPlayer->GetY());
								}
							}
							g_kMonsterManager.m_mapMonster[iMonsterIndex]->OnDamage(m_iPlayerIndex, -1, fDamageToMonster,
								ATTACKTYPE_BUFF_DAMAGE, static_cast<EGCAttackDirection>(-1), 0.0f, 0.0f, m_bIsSpecialAtk, false, false, false, true);
							g_kMonsterManager.m_mapMonster[iMonsterIndex]->SetAttackedMonster(true);
							g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);  // 요거 안해주면 동우수옥탄에서 해킹체크가 걸려영..
						}
					}
					else if (m_fCureDamage)
					{
						if (g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost) {
							float fCureDamage = ASMfabs(m_fCureDamage);
							g_kMonsterManager.m_mapMonster[iMonsterIndex]->SetHP(g_kMonsterManager.m_mapMonster[iMonsterIndex]->GetHP() + fCureDamage);
						}
					}
					g_kMonsterManager.m_mapMonster[iMonsterIndex]->SetHitPos(D3DXVECTOR2(g_kMonsterManager.m_mapMonster[iMonsterIndex]->GetX(), g_kMonsterManager.m_mapMonster[iMonsterIndex]->GetY()));
					IsHit = false;
				}
			}
		}
		else
		{
			// 지피는 지가 깍아야지 -_-
			switch (m_iHitPerFrame)
			{
			case 3:
				if (!o3) IsHit = true;
				break;
			case 5:
				if (!o5) IsHit = true;
				break;
			case 10:
				if (!o10) IsHit = true;
				break;
			default:
				if (!o10) IsHit = true;
				break;
			}

			if (m_bIsCrashPlayer)  //  플레이어의 경우는 예외 처리를 해줘야 한다. 안그러면 버프 넣다가 크래쉬
			{
				// 죽은 플레이어에게 데미지를 줄 필요가 없죠...
				if (IsHit && (g_MyD3D->MyPlayer[iPlayerIndex_]->GetPlayerState() != PS_DEAD) && (g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() > 0.0f))
				{
					if (m_fDamageToPlayer)
					{
#if defined( LEVEL_DESIGN_STAT_CALC )
						g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + m_fDamageToPlayer);
#else
						g_MyD3D->MyPlayer[iPlayerIndex_]->SetHP(g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP() + m_fDamageToPlayer * 0.01f);
#endif
						g_MyD3D->MyHead->Add_HP_Changer(iPlayerIndex_, iPlayerIndex_, m_fDamageToPlayer*(-1.0f), false, "", 0);
						//g_MyD3D->MyPlayer[iPlayerIndex_]->Slow_Count = 4;
					}
					else if (m_fAddDamageToPlayer)
					{
						PLAYER *pPlayer = g_MyD3D->GetPlayerByUID(m_dwGivePlayerUID);

						if (pPlayer) {
							PLAYER::DamageInfo damage;
							damage.SetDamage(m_fAddDamageToPlayer);
							damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_PLAYER, pPlayer->m_iPlayerIndex);
							damage.SetSpecialAttack(m_bIsSpecialAtk, 0);
							g_MyD3D->MyPlayer[iPlayerIndex_]->Change_HP(damage);
						}
						else {
							if (m_iPlayerIndex != -1 && g_MyD3D->IsPlayerIndex(m_iPlayerIndex)) {
								PLAYER::DamageInfo damage;
								damage.SetDamage(m_fAddDamageToPlayer);
								damage.SetAttacker(PLAYER::DamageInfo::DAMAGE_MONSTER, m_iPlayerIndex);
								damage.SetSpecialAttack(m_bIsSpecialAtk, 0);
								g_MyD3D->MyPlayer[iPlayerIndex_]->Change_HP(damage);
							}
						}
					}

					IsHit = false;
				}
			}
			else
			{
				// 몹 피깍게 하는건 방장님만 해야지 -_-;;
//				if( IsHit && g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
				if (IsHit)
				{
					int iMonsterIndex = iPlayerIndex_ - MAX_PLAYER_NUM;
					if (m_fDamageToMonster)
					{
						float fDamageToMonster = ASMfabs(m_fDamageToMonster);
						if (false == g_kMonsterManager.m_mapMonster[iMonsterIndex]->IsInvincible()) {
							g_kMonsterManager.m_mapMonster[iMonsterIndex]->ChangeHP(NULL, fDamageToMonster * 60.0f, m_iPlayerIndex, m_bIsSpecialAtk, true, 0);
							g_kMonsterManager.m_mapMonster[iMonsterIndex]->SetAttackedMonster(true);
						}
					}
					else if (m_fAddDamageToMonster)
					{
						float fDamageToMonster = ASMfabs(m_fAddDamageToMonster);
						if (g_MyD3D->IsPlayerIndex(m_iPlayerIndex))
						{
							PLAYER* pPlayer = g_MyD3D->MyPlayer[m_iPlayerIndex];
							if (pPlayer)
							{
								// 데미지 생성위치와 생성시 생성자위치를 넘겨준다.
								g_kMonsterManager.m_mapMonster[iMonsterIndex]->m_vDamageStartPos = D3DXVECTOR2(pPlayer->GetX(), pPlayer->GetY());
								g_kMonsterManager.m_mapMonster[iMonsterIndex]->m_vDamageWhoPos = D3DXVECTOR2(pPlayer->GetX(), pPlayer->GetY());
							}
						}
						g_kMonsterManager.m_mapMonster[iMonsterIndex]->OnDamage(m_iPlayerIndex, -1, fDamageToMonster,
							ATTACKTYPE_BUFF_DAMAGE, static_cast<EGCAttackDirection>(-1), 0.0f, 0.0f, m_bIsSpecialAtk, false, false, false, true);
						g_kMonsterManager.m_mapMonster[iMonsterIndex]->SetAttackedMonster(true);
					}
					g_kMonsterManager.m_mapMonster[iMonsterIndex]->SetHitPos(D3DXVECTOR2(g_kMonsterManager.m_mapMonster[iMonsterIndex]->GetX(), g_kMonsterManager.m_mapMonster[iMonsterIndex]->GetY()));
					IsHit = false;
				}
			}
		}
	}
}

void KGCBuff::InitPetParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime)
{
	if (g_MyD3D->m_akPet[iPlayerIndex_]->m_pObject == NULL)
		return;

	for (unsigned int i = 0; i < m_vecParticle.size(); i++)
	{
		if (false == m_vecParticle[i].m_bLocalEffect || g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			CParticleEventSeqPTR pParticle = g_MyD3D->m_akPet[iPlayerIndex_]->AddParticleWithTrace(m_vecParticle[i].m_strParticle.c_str(), 0.0f, 0);
			pParticle->SetShow(true);

			pBuffInst_->m_vecParticle.push_back(pParticle);
		}
	}
}

void KGCBuff::InitPlayerParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime)
{
	if (false == g_MyD3D->IsPlayerIndex(iPlayerIndex_))
		return;

	for (unsigned int i = 0; i < m_vecParticle.size(); i++)
	{
		if (false == m_vecParticle[i].m_bLocalEffect || g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
		{
			CParticleEventSeqPTR pParticle;
			pParticle = g_ParticleManager->CreateSequence(m_vecParticle[i].m_strParticle);

			if (pParticle != NULL)
			{
				TRACE_INFO stTrace;
				if (0 <= m_vecParticle[i].m_iPosType)
					stTrace.m_pvPos = ComputePosDesc(g_MyD3D->MyPlayer[iPlayerIndex_]->m_posDesc, m_vecParticle[i].m_iPosType);
				else
					stTrace.m_pvPos = g_kCamera.GetCameraCenterPos();

				stTrace.m_fXOffset = m_vecParticle[i].m_fPosX;
				stTrace.m_fYOffset = m_vecParticle[i].m_fPosY;

				if (0.0f < m_vecParticle[i].m_fTraceTime)
					stTrace.m_fParticleTraceTime = m_vecParticle[i].m_fTraceTime;
				else
					stTrace.m_fParticleTraceTime = fBuffTime;
				pParticle->SetTrace(&stTrace);

				if (!m_vecParticle[i].m_bIsNoDirection)
				{
					DIRECTION_INFO stDir;
					stDir.m_pbIsRight = &g_MyD3D->MyPlayer[iPlayerIndex_]->bIsRight;
					stDir.m_fDirPosX = m_vecParticle[i].m_fDirPosX;
					pParticle->SetDirectInfo(&stDir);
				}

				pParticle->SetShow(true);

				pBuffInst_->m_vecParticle.push_back(pParticle);
			}
		}
	}
}

void KGCBuff::InitAbility(int iPlayerIndex, KGCBuffInstance* pBuffInst_)
{
	if (m_bPetBuff)
	{
		InitPetAbility(iPlayerIndex, pBuffInst_);
	}
	else
	{
		InitPlayerAbility(iPlayerIndex, pBuffInst_);
	}
}

void KGCBuff::InitPetAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (iLevelIndex >= m_vecAttribute.size())
	{
		iLevelIndex = m_vecAttribute.size() - 1;
	}

	if (!m_vecAttribute.empty() && m_bIsCrashPlayer)
	{
		g_MyD3D->m_akPet[iPlayerIndex_]->SetAtkRatio(m_vecAttribute[iLevelIndex].m_fIncATK);
	}
}

void KGCBuff::DestroyBuffEffect(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_)
{
	if (pBuffInst_->m_iLevel == 0)
	{
		return;
	}

	if (!m_bClearOnDie && bOnDie_)
	{
		return;
	}

	//능력치 설정하는 부분
	if (m_bIsCrashPlayer)
	{
		unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

		if (iLevelIndex >= m_vecAttribute.size())
		{
			iLevelIndex = m_vecAttribute.size() - 1;
		}

		if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer() && m_bMPRecoveryFix) {
			g_MyD3D->MyPlayer[iPlayerIndex_]->SetMPRecoveryFix(false, 0.0f);
		}

		if (!m_vecAttribute.empty() && m_bIsCrashPlayer)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fIncHPRatio -= m_vecAttribute[iLevelIndex].m_fIncHPRatio;
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_ATTACK_PERCENT] -= m_vecAttribute[iLevelIndex].m_fIncATK;
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_DEFENCE_PERCENT] -= m_vecAttribute[iLevelIndex].m_fIncDEF;

			if (0.0f < pBuffInst_->m_fPrevFullHPInstance)
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] = pBuffInst_->m_fPrevFullHPInstance;
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_HP] < g_MyD3D->MyPlayer[iPlayerIndex_]->GetHP())
				g_MyD3D->MyPlayer[iPlayerIndex_]->SetHPFull();

			if (m_vecAttribute[iLevelIndex].m_bUnFatal)
			{
				g_MyD3D->MyPlayer[iPlayerIndex_]->m_bIncHP = true;
			}

			if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
			{
				if (m_vecAttribute[iLevelIndex].m_fMPRecoveryRate != 0.0f)
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE] -= m_vecAttribute[iLevelIndex].m_fMPRecoveryRate;
				}

				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE] -= m_vecAttribute[iLevelIndex].m_fIncMPRatio;

				g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatio = m_fOriMPDecreaseRatio;

				if (m_vecAttribute[iLevelIndex].m_fMinusAPCoolTime != 0.0f)
				{
					//깍아줄 쿨타임이 (유저기본값이) 0보다 큰 경우만 깎아 주자
					if (g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMinusAPCoolTime > 0)
						g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMinusAPCoolTime -= m_vecAttribute[iLevelIndex].m_fMinusAPCoolTime;
				}

				if (m_vecAttribute[iLevelIndex].m_fMPDecreaseRatioForSpecial != 1.0f)
				{
					g_MyD3D->MyPlayer[iPlayerIndex_]->m_fMPDecreaseRatioForSpecial = 1.0f;
				}
			}
		}

		if (m_bIsCrashPlayer) {
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer())
			{
				if (m_fChangeMpRatio != 0.f) {
					g_MyD3D->m_fIncMPRatio -= m_fOriIncMpRatio;
					m_fOriIncMpRatio = 0.f;
				}
			}
		}
	}

	//속도 능력치 설정하는 부분
	if (m_bIsCrashPlayer)
	{
		unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

		if (iLevelIndex >= (int)m_vecSpeedAttribute.size())
		{
			iLevelIndex = m_vecSpeedAttribute.size() - 1;
		}

		if (!m_vecSpeedAttribute.empty())
		{
			BuffSpeedAttribute& attr = m_vecSpeedAttribute[iLevelIndex];
			SetPlayerSpeed(pBuffInst_, iPlayerIndex_, pBuffInst_->m_iBuffEnum, attr, -1);
		}

		//카툰 벗겨!~
		if (m_eCartoonTexture != CARTOON_NORMAL && g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject)
		{
			if (g_pMagicEffect->GetCartoon(iPlayerIndex_) == m_eCartoonTexture)
			{
				g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_DEFAULT);
			}
		}

		if (m_bSuperArmor)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->SuperArmor(false);
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo())
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo()->m_bSuperArmor = false;

			g_pMagicEffect->SetPlayerCartoon(iPlayerIndex_, CARTOON_DEFAULT);
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_pObject->SetOutlineColor(EDGE_COLOR_FRIEND);
		}
		if (m_bHyperArmor)
		{
			g_MyD3D->MyPlayer[iPlayerIndex_]->m_bHyperArmor = false;
			if (g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo())
				g_MyD3D->MyPlayer[iPlayerIndex_]->GetNextTagCharInfo()->m_bHyperArmor = false;
		}
	}
	else
	{
		if (m_bSuperArmor)
		{
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->SetSuperArmor(false);
			g_kMonsterManager.m_mapMonster[(iPlayerIndex_ - MAX_PLAYER_NUM)]->m_pObject->SetOutlineColor(EDGE_COLOR_FRIEND);
		}
	}

	// 애니메이션이 있다면 삭제
	for (std::vector<CKTDGAnim::CAnimInstance*>::iterator vit = pBuffInst_->m_vecAnim.begin(); vit != pBuffInst_->m_vecAnim.end(); vit++)
	{
		g_MyD3D->m_pMapAnim->DeleteInst(*vit);
		(*vit) = NULL;
	}
	pBuffInst_->m_vecAnim.clear();

	//파티클이 있다면 삭제
	for (std::vector<CParticleEventSeqPTR>::iterator vit = pBuffInst_->m_vecParticle.begin(); vit != pBuffInst_->m_vecParticle.end(); vit++)
	{
		g_ParticleManager->DeleteSequence(*vit);
		(*vit) = NULL;
	}
	pBuffInst_->m_vecParticle.clear();

	//FinalizeMonsterStatus( iPlayerIndex_, pBuffInst_ );
	FinalizeMonsterStatus(iPlayerIndex_ - MAX_PLAYER_NUM, pBuffInst_);
}

void KGCBuff::FinalizePetAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_)
{
	unsigned int iLevelIndex = pBuffInst_->m_iLevel - 1;

	if (iLevelIndex >= m_vecAttribute.size())
	{
		iLevelIndex = m_vecAttribute.size() - 1;
	}

	if (!m_vecAttribute.empty() && m_bIsCrashPlayer)
	{
		g_MyD3D->m_akPet[iPlayerIndex_]->SetAtkRatio(1.0f);
	}
}

void KGCBuff::CreateDamage(int iHit, int iHitted, KGCBuffInstance* pBuffInst_, D3DXVECTOR2* vPos, EGCCharAttackType eCharAtkType_)
{
	if (pBuffInst_->m_fRemainTime <= 0.0f)
	{
		return;
	}

	if (pBuffInst_->m_bPostCoolTime)
		return;

	// 펫 데미지 허용 안한다면 리턴
	if (eCharAtkType_ == CAHR_ATTACK_TYPE_PET_DAMAGE && m_bDamageNoPetAtk == true) {
		return;
	}

	// 물리 데미지만 체크한다면 박스데미지 들어오면 리턴
	if (eCharAtkType_ == CAHR_ATTACK_TYPE_BOX_DAMAGE && m_bDamageNoOnlyPhysicAtk == false) {
		return;
	}

	// 필살기 데미지 허용 안하면 리턴
	if (eCharAtkType_ == CAHR_ATTACK_TYPE_SPECIAL_DAMAGE && m_bDamageNoSpecialAtk == true) {
		return;
	}

	for (int i = 0; i < (int)m_vecDamage.size(); i++)
	{
		if (vPos)
		{
			CDamageInstance* pDmg = g_MyD3D->m_pDamageManager->AddWithLocate(m_vecDamage[i], iHit, vPos->x, vPos->y);
			if (pDmg == NULL) {
				return;
			}

			pDmg->m_X = vPos->x;
			pDmg->m_Y = vPos->y;

			if (m_bHitOnce)
				pBuffInst_->m_fRemainTime = GC_ELAPSED_TIME;
		}
		else
		{
			LTP_DAMAGE packet;
			packet.bAbsoluePos = true;
			packet.iCharIndex = iHit;
			packet.iDamage = m_vecDamage[i];
			packet.fx = g_MyD3D->MyPlayer[iHitted]->vPos.x;
			packet.fy = g_MyD3D->MyPlayer[iHitted]->vPos.y;

			packet.bMonsterDamage = false;

			if (!g_MyD3D->MyPlayer[iHit]->bIsRight)
				packet.fx = -packet.fx;

			g_LatencyTransfer.PushPacket(&packet);
		}
	}
}

unsigned int KGCBuff::GetMaxLevel()
{
	unsigned int iRet;

	if (m_bIsCrashPlayer)
	{
		iRet = m_vecAttribute.size();
		if (iRet < m_vecSpeedAttribute.size())
		{
			iRet = m_vecSpeedAttribute.size();
		}
		if (iRet < m_vecpHudTex.size())
		{
			iRet = m_vecpHudTex.size();
		}
	}
	else
	{
		iRet = m_vecMonAttribute.size();
	}

	if (iRet == 0)
		iRet++;

	return iRet;
}

void KGCBuff::SetPlayerSpeed(KGCBuffInstance* pBuffInst_, int iPlayerIndex_, int nBuffEnum, BuffSpeedAttribute &attr, int sumValue)
{
	for (int i = 0; i < FORM_NUM; ++i)
	{
		SetPlayerSpeed(g_MyD3D->MyPlayer[iPlayerIndex_]->m_FormTemplate[i], pBuffInst_->m_kOriFormTemplate[i], nBuffEnum, attr, sumValue, iPlayerIndex_);
	}

	for (int i = 0; i < BTYPE_NUM; ++i)
	{
		SetPlayerSpeed(g_MyD3D->MyPlayer[iPlayerIndex_]->m_BurnningTemplate[i], pBuffInst_->m_kOriBurnningTemplate[i], nBuffEnum, attr, sumValue, iPlayerIndex_);
	}
	SetPlayerSpeed(g_MyD3D->MyPlayer[iPlayerIndex_]->m_SwapTemplate, pBuffInst_->m_kOriSwapTemplate, nBuffEnum, attr, sumValue, iPlayerIndex_);
}

void KGCBuff::SetPlayerSpeed(PLAYER_TEMPLATE &player_, BuffSpeedAttribute &kOriginalTemplate, int nBuffEnum, BuffSpeedAttribute &attr, int sumValue, int iPlayerIndex_)
{
	if (m_bSpeedPercent == false)
	{
		player_.fJumpXSpeed += (attr.m_fJumpXSpeed * sumValue);
		player_.fJumpYSpeed += (attr.m_fJumpYSpeed * sumValue);
		player_.fDoubleJumpXSpeed += (attr.m_fDashJumpXSpeed * sumValue);
		player_.fDoubleJumpYSpeed += (attr.m_fDashJumpYSpeed * sumValue);
		player_.fRunJumpXSpeed += (attr.m_fRunJumpXSpeed * sumValue);
		player_.fRunJumpYSpeed += (attr.m_fRunJumpYSpeed * sumValue);
		player_.fDashJumpXSpeed += (attr.m_fDashJumpXSpeed * sumValue);
		player_.fDashJumpYSpeed += (attr.m_fDashJumpYSpeed * sumValue);
		player_.fWalkSpeed += (attr.m_fWalkSpeed * sumValue);
		player_.fRunSpeed += (attr.m_fRunSpeed * sumValue);
	}
	else
	{
		if (sumValue != -1)
		{
			kOriginalTemplate.m_fJumpXSpeed = player_.fJumpXSpeed;
			kOriginalTemplate.m_fJumpYSpeed = player_.fJumpYSpeed;
			kOriginalTemplate.m_fDoubleJumpXSpeed = player_.fDoubleJumpXSpeed;
			kOriginalTemplate.m_fDoubleJumpYSpeed = player_.fDoubleJumpYSpeed;
			kOriginalTemplate.m_fRunJumpXSpeed = player_.fRunJumpXSpeed;
			kOriginalTemplate.m_fRunJumpYSpeed = player_.fRunJumpYSpeed;
			kOriginalTemplate.m_fDashJumpXSpeed = player_.fDashJumpXSpeed;
			kOriginalTemplate.m_fDashJumpYSpeed = player_.fDashJumpYSpeed;
			kOriginalTemplate.m_fWalkSpeed = player_.fWalkSpeed;
			kOriginalTemplate.m_fRunSpeed = player_.fRunSpeed;

			//  이동 속도 증가 버프 일때
			if (nBuffEnum == EGC_EFFECT_ATTRIBUTE_SPEED_UP)
			{
				if (iPlayerIndex_ != -1 && g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SPEEP_UP_BUFF] > 0.f)
				{
					float fPercentSpeed = g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SPEEP_UP_BUFF] / 100.f;
					player_.fWalkSpeed += player_.fWalkSpeed * fPercentSpeed;
					player_.fRunSpeed += player_.fRunSpeed * fPercentSpeed;
				}
			}
			else
			{
				player_.fJumpXSpeed += (player_.fJumpXSpeed * (attr.m_fJumpXSpeed / 100.f));
				player_.fJumpYSpeed += (player_.fJumpYSpeed * (attr.m_fJumpYSpeed / 100.f));
				player_.fDoubleJumpXSpeed += (player_.fDoubleJumpXSpeed * (attr.m_fDashJumpXSpeed / 100.f));
				player_.fDoubleJumpYSpeed += (player_.fDoubleJumpYSpeed * (attr.m_fDashJumpYSpeed / 100.f));
				player_.fRunJumpXSpeed += (player_.fRunJumpXSpeed * (attr.m_fRunJumpXSpeed / 100.f));
				player_.fRunJumpYSpeed += (player_.fRunJumpYSpeed * (attr.m_fRunJumpYSpeed / 100.f));
				player_.fDashJumpXSpeed += (player_.fDashJumpXSpeed * (attr.m_fDashJumpXSpeed / 100.f));
				player_.fDashJumpYSpeed += (player_.fDashJumpYSpeed * (attr.m_fDashJumpYSpeed / 100.f));
				player_.fWalkSpeed += (player_.fWalkSpeed * (attr.m_fWalkSpeed / 100.f));
				player_.fRunSpeed += (player_.fRunSpeed * (attr.m_fRunSpeed / 100.f));
			}
		}
		else
		{
			//  이동 속도 증가 버프 일때
			if (nBuffEnum == EGC_EFFECT_ATTRIBUTE_SPEED_UP)
			{
				if (iPlayerIndex_ != -1 && g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SPEEP_UP_BUFF] > 0.f)
				{
					float fPercentSpeed = g_MyD3D->MyPlayer[iPlayerIndex_]->m_fAbility[ABILITY_FATAL_SPEEP_UP_BUFF] / 100.f;
					player_.fWalkSpeed -= kOriginalTemplate.m_fWalkSpeed * fPercentSpeed;
					player_.fRunSpeed -= kOriginalTemplate.m_fRunSpeed * fPercentSpeed;
				}
			}
			else
			{
				player_.fJumpXSpeed -= (kOriginalTemplate.m_fJumpXSpeed * (attr.m_fJumpXSpeed / 100.f));
				player_.fJumpYSpeed -= (kOriginalTemplate.m_fJumpYSpeed * (attr.m_fJumpYSpeed / 100.f));
				player_.fDoubleJumpXSpeed -= (kOriginalTemplate.m_fDoubleJumpXSpeed * (attr.m_fDashJumpXSpeed / 100.f));
				player_.fDoubleJumpYSpeed -= (kOriginalTemplate.m_fDoubleJumpYSpeed * (attr.m_fDashJumpYSpeed / 100.f));
				player_.fRunJumpXSpeed -= (kOriginalTemplate.m_fRunJumpXSpeed * (attr.m_fRunJumpXSpeed / 100.f));
				player_.fRunJumpYSpeed -= (kOriginalTemplate.m_fRunJumpYSpeed * (attr.m_fRunJumpYSpeed / 100.f));
				player_.fDashJumpXSpeed -= (kOriginalTemplate.m_fDashJumpXSpeed * (attr.m_fDashJumpXSpeed / 100.f));
				player_.fDashJumpYSpeed -= (kOriginalTemplate.m_fDashJumpYSpeed * (attr.m_fDashJumpYSpeed / 100.f));
				player_.fWalkSpeed -= (kOriginalTemplate.m_fWalkSpeed * (attr.m_fWalkSpeed / 100.f));
				player_.fRunSpeed -= (kOriginalTemplate.m_fRunSpeed * (attr.m_fRunSpeed / 100.f));
			}
		}
	}
}

void KGCBuff::PlaySound(DWORD dwSoundID)
{
	if (dwSoundID != 0) {
		std::stringstream stream;
		stream << dwSoundID;
		g_KDSound.Stop(stream.str().c_str());
		g_KDSound.PlayNoOverlapping(stream.str().c_str(), 0, 0);
	}
}

bool KGCBuff::IsSceondBaseMeshEnable(PLAYER* pPlayer_, bool bDestroy_)
{
	if (NULL == pPlayer_) {
		return false;
	}

	// 제로 그란 지배일때 안됨
	if (false == bDestroy_ && pPlayer_->IsMagicEffect(EGC_EFFECT_ZERO_DOMINATION_OF_GRANDARK)) {
		return false;
	}

	// 변신 상태일때는 안됨
	if (false == bDestroy_ && pPlayer_->MetamorphosisForm != FORM_NORMAL) {
		return false;
	}

	return true;
}