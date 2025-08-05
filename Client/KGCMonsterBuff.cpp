#include "StdAfx.h"
#include "KGCMonsterBuff.h"

#include "MyD3D.h"
#include "LatencyTransfer.h"
#include "Player.h"
#include "GCCameraOrtho.h"
#include "Headup Display.h"
#include "Monster.h"
#include "Buff/KGCBuffManager.h"
#include "DamageManager.h"
#include "GCUI/KGCInGameMessage.h"
#ifndef __PATH__
#include "dbg/dbg.hpp"
#endif

KGCMonsterBuff::KGCMonsterBuff(void)
{
	m_iTgID = -1;
	m_pTexBase = NULL;
	m_iMonsterType = -1;
	m_iRemainTime = 0;
	m_iInterval = GC_FPS_LIMIT * 10;  // 10초
	DEFAULT_LIFE = 10;
	m_iLife = DEFAULT_LIFE;                                       // 10대
	m_bCrashCheck = true;
	vecMonIndex.clear();
}

KGCMonsterBuff::~KGCMonsterBuff(void)
{
	SAFE_RELEASE(m_pTexBase);
}

void KGCMonsterBuff::Create(void)
{
	// 타입이 맞는 몬스터!

	for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		MONSTER *pMonster = mit->second;
		if (m_iMonsterType == pMonster->m_iMonsterType)
		{
			vecMonIndex.push_back(mit->first);
		}
	}
}

void KGCMonsterBuff::Destroy(void)
{
	for (int i = 0; i < (int)m_vecParticleActive.size(); i++)
	{
		g_ParticleManager->DeleteSequence(m_vecParticleActive[i]);
	}

	for (int i = 0; i < (int)m_vecParticleCoolTime.size(); i++)
	{
		g_ParticleManager->DeleteSequence(m_vecParticleCoolTime[i]);
	}

	m_vecParticleActive.clear();
	m_vecParticleCoolTime.clear();
	m_vecParticleStart.clear();
}

void KGCMonsterBuff::Enable(void)
{
}

void KGCMonsterBuff::FrameMove(float fElapsedTime /*= GC_ELAPSED_TIME */)
{
	// 살아 있는 몬스터
	int iRet = 0;
	std::vector<int>::iterator vit = vecMonIndex.begin();
	for (; vit != vecMonIndex.end(); )
	{
		if (g_kMonsterManager.m_mapMonster[(*vit)]->m_fHP > 0)
		{
			iRet++;
			++vit;
		}
		else
			vit = vecMonIndex.erase(vit);
	}

	// 나와 바인딩 되어 있는 몬스터가 다 죽었군... 난 꺼질래 ㅋ
	if (iRet == 0)
	{
		EnableLive(false);
		return;
	}

	if (false == KGCGameObject::IsLive())
	{
		if (m_iRemainTime > 0)
		{
			m_iRemainTime--;
		}
		else
		{
			EnableLive(true);
			DoResetTime();
		}
	}

#if !defined( __PATH__ ) // 몬스터의 바운딩박스
	{
		if (g_kGlobalValue.m_bRenderBoundBox)
		{
			GCCollisionRect<float> fRect = GetCollisionRect();
			D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
			SETVECTOR3(lefttop, fRect.m_Left, fRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(righttop, fRect.m_Right, fRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(leftbottom, fRect.m_Left, fRect.m_Bottom - 0.25f, 0.4f);
			SETVECTOR3(rightbottom, fRect.m_Right, fRect.m_Bottom - 0.25f, 0.4f);

			g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
				lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
				D3DCOLOR_ARGB(60, 0, 128, 128));
		}
	}
#endif

	return;
}

void KGCMonsterBuff::Render(void)
{
	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pGCDeviceManager2->DrawInWorld(m_pTexBase, &g_kCamera.m_matCamera,
		D3DXVECTOR3(m_fPosX - 1.2f, m_fPosY - 0.56f, 3.0f),
		D3DXVECTOR3(m_fPosX - 1.2f, m_fPosY + m_fHeight - 0.56f, 3.0f),
		D3DXVECTOR3(m_fPosX + m_fWidth - 1.2f, m_fPosY - 0.56f, 3.0f),
		D3DXVECTOR3(m_fPosX + m_fWidth - 1.2f, m_fPosY + m_fHeight - 0.56f, 3.0f));

	g_pGCDeviceManager2->PopState();
}

GCCollisionRect<float> KGCMonsterBuff::GetCollisionRect(void)
{
	GCCollisionRect<float> rtRect;
	rtRect.m_Left = m_fPosX + (m_fWidth / 2) * 2 - 1.435f;
	rtRect.m_Right = m_fPosX + (m_fWidth / 2) * 3 - 1.335f;
	rtRect.m_Top = m_fPosY + m_fHeight - 0.25f;
	rtRect.m_Bottom = m_fPosY - 0.28f;

	return rtRect;
}

void KGCMonsterBuff::Activate(void)
{
	for (int i = 0; i < (int)m_vecParticleStart.size(); ++i)
	{
		g_ParticleManager->CreateSequence(m_vecParticleStart[i], m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f);
	}

	DoSomeThing();
	ResetCoolTime();
}

void KGCMonsterBuff::ResetCoolTime()
{
	m_iRemainTime = m_iInterval;
	m_iLife = DEFAULT_LIFE;
	EnableLive(false);
}

void KGCMonsterBuff::EnableLive(bool bLive)
{
	KGCGameObject::EnableLive(bLive);

	for (int i = 0; i < (int)m_vecParticleActive.size(); i++)
	{
		m_vecParticleActive[i]->SetShow(bLive);
	}

	for (int i = 0; i < (int)m_vecParticleCoolTime.size(); i++)
	{
		m_vecParticleCoolTime[i]->SetShow(bLive);
	}
}

KGCMonsterDefence::KGCMonsterDefence(void)
{
	m_iRemainTime = 0;
	m_iInterval = GC_FPS_LIMIT * 30;
}

KGCMonsterDefence::~KGCMonsterDefence(void)
{
}

void KGCMonsterDefence::Create(void)
{
	KGCMonsterBuff::Create();

	m_pTexBase = g_pGCDeviceManager2->CreateTexture("monbuff_base.dds");

	CParticleEventSeqPTR pParticle;

	char str[256] = "";

	for (int i = 0; i < 3; ++i)
	{
		switch (m_iMonsterType)
		{
		case MON_HERO_FIREGOLEM:
			sprintf(str, "FGolem_Gem_0%d", i + 1);
			break;
		case MON_HERO_GOLEM:
			sprintf(str, "Golem_Gem_0%d", i + 1);
			break;
		case MON_HERO_ICEGOLEM:
			sprintf(str, "IGolem_Gem_0%d", i + 1);
			break;
		}

		pParticle = g_ParticleManager->CreateSequence(str, m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f);
		m_vecParticleActive.push_back(pParticle);
	}

	for (int i = 0; i < 3; ++i)
	{
		switch (m_iMonsterType)
		{
		case MON_HERO_FIREGOLEM:
			sprintf(str, "FGolem_Gem_Run_0%d", i + 1);
			break;
		case MON_HERO_GOLEM:
			sprintf(str, "Golem_Gem_Run_0%d", i + 1);
			break;
		case MON_HERO_ICEGOLEM:
			sprintf(str, "IGolem_Gem_Run_0%d", i + 1);
			break;
		}
		m_vecParticleStart.push_back(str);
	}
}

void KGCMonsterDefence::Destroy(void)
{
	for (std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
		//		if ( g_kMonsterManager.IsMonsterIndex(0) == false ) break;

		g_kMonsterManager.m_mapMonster[0]->ClearMagicEffect(mit->first, EGC_EFFECT_MON_INFINITY_DEFENCE);
	}
}

void KGCMonsterDefence::DoSomeThing(void)
{
	int iCount = vecMonIndex.size();
	for (int i = 0; i < iCount; ++i)
	{
		//pMonster:SetMagicEffect( iMonsterIndex, EGC_EFFECT_SETESH_PROTECT, buffTime, 1, -1, false, false );
		if (g_kMonsterManager.IsMonsterIndex(vecMonIndex[i]) == false) continue;

		g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->ClearMagicEffect(vecMonIndex[i], EGC_EFFECT_MON_INFINITY_DEFENCE);
	}
}

bool KGCMonsterDefence::OnDamage(int iPlayerIndex, float fAttackPower, EGCAttackType eAttackType, EGCAttackDirection eAttackDirection, float fPushTargetX, float fPushTargetY, bool bSpecialAttack)
{
	if (false == IsLive() || fAttackPower <= 0.0f) return false;

	--m_iLife;
	if (m_iLife <= 0)
	{
		if (g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost)
		{
			LTP_CHANGE_OBJECT_HP_INFO kChangeObjectHPInfo;
			kChangeObjectHPInfo.iSlotIndex = this->GetIndex();

			g_LatencyTransfer.PushPacket(&kChangeObjectHPInfo);
		}
	}

	return true;
}

void KGCMonsterDefence::DoResetTime()
{
	int iCount = vecMonIndex.size();
	for (int i = 0; i < iCount; ++i)
	{
		if (g_kMonsterManager.IsMonsterIndex(vecMonIndex[i]) == false) continue;

		g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->SetMagicEffect(vecMonIndex[i], EGC_EFFECT_MON_INFINITY_DEFENCE, 999999.0f, 1, -1, false, false);
	}
}

//////////////////////////////////////////////////////////////////////////
KGCDemonCore::KGCDemonCore(void)
{
	m_iRemainTime = 0;
	m_iInterval = GC_FPS_LIMIT * 60;
	m_bStartTimer = false;
	m_bEnd = false;
}

KGCDemonCore::~KGCDemonCore(void)
{
}

void KGCDemonCore::Create(void)
{
	KGCMonsterBuff::Create();

	switch (m_iMonsterType)
	{
	case MON_HELL_VULCANUS:
		DEFAULT_LIFE = 30;
		m_iLife = DEFAULT_LIFE;
		m_iInterval = INT_MAX;
		m_pTexBase = g_pGCDeviceManager2->CreateTexture("demoncore_base_1.dds");
		break;
	case MON_HERO_NEMOPHILLA:
		m_pTexBase = g_pGCDeviceManager2->CreateTexture("demoncore_base.dds");
		break;
	}

	CParticleEventSeqPTR pParticle;

	char str[256] = "";

	std::string strParticle = "";
	for (int i = 0; i < 4; ++i)
	{
		switch (m_iMonsterType)
		{
		case MON_HERO_NEMOPHILLA:
			sprintf(str, "Demoncore_0%d", i + 1);
			break;
		case MON_HELL_VULCANUS:
			sprintf(str, "Demoncore2nd_0%d", i + 1);
			break;
		}

		pParticle = g_ParticleManager->CreateSequence(str, m_fPosX - 1.073f, m_fPosY - 0.4f, 0.5f);
		if (pParticle != NULL)
			m_vecParticleActive.push_back(pParticle);
	}

	for (int i = 0; i < 4; ++i)
	{
		switch (m_iMonsterType)
		{
		case MON_HERO_NEMOPHILLA:
			sprintf(str, "Demoncore_Boom_0%d", i + 1);
			break;
		case MON_HELL_VULCANUS:
			sprintf(str, "Demoncore2nd_Boom_0%d", i + 1);
			break;
		}

		m_vecParticleStart.push_back(str);
	}

	switch (m_iMonsterType)
	{
	case MON_HERO_NEMOPHILLA:
	{
		g_pkUIScene->m_pkGameOverlayUI->SetTitleText(g_pkStrLoader->GetString(6524));
		g_pkUIScene->m_pkGameOverlayUI->StartNumberCount(20);
		m_bStartTimer = true;
		int iCount = vecMonIndex.size();
		for (int i = 0; i < iCount; ++i)
		{
			if (g_kMonsterManager.IsMonsterIndex(vecMonIndex[i]) == false) continue;

			if (g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->m_fHP > 0.0f)
			{
				g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->SetMagicEffect(vecMonIndex[i], EGC_EFFECT_MON_INFINITY_DEFENCE, 20.0f, 1, -1, false, false);
				g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->SetState("ATTACK13");
			}
		}
	}
	break;
	case MON_HELL_VULCANUS:
		g_pkUIScene->m_pkGameOverlayUI->SetTitleText(g_pkStrLoader->GetString(8167));
		break;
	}
}

void KGCDemonCore::Destroy(void)
{
	//g_KDSound.Play("986032");
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (!g_MyD3D->m_pObject[i]) continue;
		KGCGameObject* pObject = (KGCGameObject*)g_MyD3D->m_pObject[i];

		if (!pObject) continue;

		pObject->EnableLive(false);
	}

	switch (m_iMonsterType)
	{
	case MON_HERO_NEMOPHILLA:
		for (int j = 0; j < MAX_PLAYER_NUM; ++j)
		{
			g_pkQuestManager->SetPlayerPosition(j, (float)(178.0f * 0.0025f), (float)(1818.5f * 0.0025));
		}
		break;
	}
}

void KGCDemonCore::DoSomeThing()
{
	switch (m_iMonsterType)
	{
	case MON_HERO_NEMOPHILLA:
		int iCount = vecMonIndex.size();
		for (int i = 0; i < iCount; ++i)
		{
			if (g_kMonsterManager.IsMonsterIndex(vecMonIndex[i]) == false) continue;

			g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->ClearMagicEffect(vecMonIndex[i], EGC_EFFECT_MON_INFINITY_DEFENCE);
			g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->SetState("COUNTER");
		}

		for (int j = 0; j < MAX_PLAYER_NUM; ++j)
		{
			g_pkQuestManager->SetPlayerPosition(j, (float)(178.0f * 0.0025f), (float)(1818.5f * 0.0025));
		}

		g_pkUIScene->m_pkGameOverlayUI->StartNumberCount(-1);
		break;
	}
}

void KGCDemonCore::DoResetTime()
{
	switch (m_iMonsterType)
	{
	case MON_HERO_NEMOPHILLA:
		g_pkUIScene->m_pkGameOverlayUI->SetTitleText(g_pkStrLoader->GetString(6524));
		g_pkUIScene->m_pkGameOverlayUI->StartNumberCount(20);
		m_bStartTimer = true;
		int iCount = vecMonIndex.size();
		for (int i = 0; i < iCount; ++i)
		{
			if (g_kMonsterManager.IsMonsterIndex(vecMonIndex[i]) == false) continue;

			if (g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->m_fHP > 0.0f)
			{
				g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->SetMagicEffect(vecMonIndex[i], EGC_EFFECT_MON_INFINITY_DEFENCE, 20.0f, 1, -1, false, false);
				g_kMonsterManager.m_mapMonster[vecMonIndex[i]]->SetState("ATTACK13");
			}
		}

		for (int i = 1; i < MAX_OBJECT; i++)
		{
			if (!g_MyD3D->m_pObject[i]) continue;
			KGCGameObject* pObject = (KGCGameObject*)g_MyD3D->m_pObject[i];

			if (!pObject) continue;
			pObject->EnableLive(true);
		}
		break;
	}
}

bool KGCDemonCore::OnDamage(int iPlayerIndex, float fAttackPower, EGCAttackType eAttackType, EGCAttackDirection eAttackDirection, float fPushTargetX, float fPushTargetY, bool bSpecialAttack)
{
	if (false == IsLive() || fAttackPower <= 0.0f) return false;

	--m_iLife;
	if (m_iLife <= 0)
	{
		if (g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost)
		{
			LTP_CHANGE_OBJECT_HP_INFO kChangeObjectHPInfo;
			kChangeObjectHPInfo.iSlotIndex = this->GetIndex();

			g_LatencyTransfer.PushPacket(&kChangeObjectHPInfo);
		}
	}

	return true;
}

void KGCDemonCore::FrameMove(float fElapsedTime /*= GC_ELAPSED_TIME */)
{
	if (m_bEnd) return;

	//if ( !o30 )
		//START_LOG( clog, L"******************" << m_iRemainTime << L"!!!!!!!!!!!!" );

	KGCMonsterBuff::FrameMove(fElapsedTime);
	// 살아 있는 몬스터
	int iRet = 0;
	std::vector<int>::iterator vit = vecMonIndex.begin();
	for (; vit != vecMonIndex.end(); )
	{
		if (g_kMonsterManager.IsMonsterIndex((*vit)) == false) continue;

		if (g_kMonsterManager.m_mapMonster[(*vit)]->m_fHP > 0)
		{
			iRet++;
			++vit;
		}
		else
			vit = vecMonIndex.erase(vit);
	}

	// 나와 바인딩 되어 있는 몬스터가 다 죽었군... 난 꺼질래 ㅋ
	if (iRet == 0)
	{
		m_bEnd = true;
		return;
	}

	if (KGCGameObject::IsLive())
	{
		switch (m_iMonsterType)
		{
		case MON_HERO_NEMOPHILLA:
			if (m_bStartTimer && g_pkUIScene->m_pkGameOverlayUI->IsNumberCountEnd())
			{
				for (int i = 0; i < MAX_PLAYER_NUM; i++)
				{
					if (g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true)
					{
						PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

						pPlayer->AddParticleNoDirectionPos("Kaz_After_05", 0, 0.0, 0.0);
						pPlayer->AddParticleNoDirectionPos("Kaz_After_07", 0, 0.0, 0.0);
						pPlayer->AddParticleNoDirectionPos("Arme01_Death_Cloud_Boom_01", 0, 0.0, 0.0);

						if (pPlayer->IsLocalPlayer())
						{
							pPlayer->SetHP(0.0f);
							pPlayer->Change_Motion(MID_COMMON_DOWN_AND_STANDUP);
						}
					}
				}

				m_bStartTimer = false;
				ResetCoolTime();
			}
			break;
		case MON_HELL_VULCANUS:
			int iRet = 0;
			std::vector<int>::iterator vit = vecMonIndex.begin();
			for (; vit != vecMonIndex.end(); ++vit)
			{
				if (g_kMonsterManager.IsMonsterIndex((*vit)) == false) continue;

				if (g_kMonsterManager.m_mapMonster[(*vit)]->m_fHP / g_kMonsterManager.m_mapMonster[(*vit)]->m_fHPPoint <= 0.3f)
				{
					g_kMonsterManager.m_mapMonster[(*vit)]->SetHP(g_kMonsterManager.m_mapMonster[(*vit)]->GetMaxHP() * 0.3f);
				}
			}
			break;
		}
	}
}

void KGCDemonCore::ResetCoolTime()
{
	KGCMonsterBuff::ResetCoolTime();

	switch (m_iMonsterType)
	{
	case MON_HERO_NEMOPHILLA:
		for (int i = 1; i < MAX_OBJECT; i++)
		{
			if (!g_MyD3D->m_pObject[i]) continue;
			KGCGameObject* pObject = (KGCGameObject*)g_MyD3D->m_pObject[i];

			if (!pObject) continue;
			pObject->EnableLive(false);
		}

		for (int j = 0; j < MAX_PLAYER_NUM; ++j)
		{
			g_pkQuestManager->SetPlayerPosition(j, (float)(178.0f * 0.0025f), (float)(1818.5f * 0.0025));
		}
		break;
	}
}

// 점령전 몬스터 소환 문
// 2011, 5, 26, shmhlove
KGCDotaMonsterSummonGate::KGCDotaMonsterSummonGate(void)
	: m_iNowCoolTime(0)
	, m_iMapID(0)
{
	m_vSummonPos = D3DXVECTOR2(0.0f, 0.0f);
}
KGCDotaMonsterSummonGate::~KGCDotaMonsterSummonGate(void) {}
void KGCDotaMonsterSummonGate::Create(void)
{
	KGCMonsterBuff::Create();

	// MapID가 1보다 작을일은 던전스크립트에서 Property값을 잘못줬을때이다.!!
	if (1 > m_iMonsterType)
		return;

	// Property속성으로 받은 값을 MapID로 처리
	m_iMapID = m_iMonsterType;

	// 도타템플릿 정보 얻어오기( 도타템플릿이 없다면 크래시 나게 놔둔다.. 스크립트 오류이니깐.. 크래시로 알려야함.. )
	std::vector<SDotaTemplate>::iterator vecTemplateInfoIter = g_kGlobalValue.m_vecDotaTemplate.begin();
	for (; vecTemplateInfoIter != g_kGlobalValue.m_vecDotaTemplate.end(); ++vecTemplateInfoIter)
	{
		if (m_iMapID == vecTemplateInfoIter->iDotaMapID)
			m_sTemplateInfo = (*vecTemplateInfoIter);
	}

	// 게이트 정보 얻어오기( 게이트ID가 없다면 크래시 나게 놔둔다.. 스크립트 오류이니깐.. 크래시로 알려야함.. )
	std::vector<SDotaGateInfo>::iterator vecGateInfoIter = m_sTemplateInfo.vecsGateInfo.begin();
	for (; vecGateInfoIter != m_sTemplateInfo.vecsGateInfo.end(); ++vecGateInfoIter)
	{
		if (m_iTgID == vecGateInfoIter->iGateID)
			m_sGateInfo = (*vecGateInfoIter);
	}

	// 소환문 텍스쳐 로드( 파티클로 대체함 )
	std::vector<std::string> strTemp;
	if (TEAM_SERDIN == m_sGateInfo.iTeam)
	{
		strTemp.push_back("Dota_PortalR_01");
		strTemp.push_back("Dota_PortalR_02");
	}
	else
	{
		strTemp.push_back("Dota_PortalB_01");
		strTemp.push_back("Dota_PortalB_02");
	}

	CParticleEventSeqPTR pParticle;
	pParticle = g_ParticleManager->CreateSequence(strTemp[0].c_str(), m_fPosX - 0.97f, m_fPosY - 0.17f, 1.0f);
	if (pParticle != NULL)
	{
		pParticle->SetShow(true);
		m_vecParticleGate.push_back(pParticle);
	}
	pParticle = g_ParticleManager->CreateSequence(strTemp[1].c_str(), m_fPosX - 0.97f, m_fPosY - 0.17f, 1.0f);
	if (pParticle != NULL)
	{
		pParticle->SetShow(true);
		m_vecParticleGate.push_back(pParticle);
	}
	//m_pTexBase = g_pGCDeviceManager2->CreateTexture( "o2_zone2.dds" );

	// 소환 위치 설정
	m_vSummonPos.x = (m_fPosX + 0.05f) / SiKGCFantasticMap()->GetPixelWidth();
	m_vSummonPos.y = m_fPosY + 0.05f;
}

void KGCDotaMonsterSummonGate::Destroy(void)
{
	std::vector< CParticleEventSeqPTR >::iterator vecIter = m_vecParticleGate.begin();
	for (; vecIter != m_vecParticleGate.end(); ++vecIter)
	{
		g_ParticleManager->DeleteSequence((*vecIter));
	}
	m_vecParticleGate.clear();
}

void KGCDotaMonsterSummonGate::FrameMove(float fElapsedTime)
{
	KGCMonsterBuff::FrameMove(fElapsedTime);

	if (g_kGlobalValue.m_kUserInfo.bHost)
	{
		if (m_iNowCoolTime > m_sGateInfo.iSummonCoolTime)
		{
			int iSlot = CheckSummon();

			// 쿨타임 초기화
			m_iNowCoolTime = 0;

			if (iSlot < 0)
				return;

			// 몬스터 소환
			KGC_PID_BROAD_DOTA_SUMMON_MONSTER kPacket;
			kPacket.bIsRight = m_sGateInfo.bIsRight;
			kPacket.iTeam = m_sGateInfo.iTeam;
			kPacket.MonsterID = (EGCMonster)m_sGateInfo.vecMonsterType[rand() % m_sGateInfo.vecMonsterType.size()];
			kPacket.vecSummonPos = D3DXVECTOR2(m_vSummonPos.x, m_vSummonPos.y);
			kPacket.iGateID = m_sGateInfo.iGateID;
			kPacket.iSlot = iSlot;
			kPacket.iHP = m_sGateInfo.iHP;
			SendP2PPacket(&kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE);
			KGCPC_MEMO("KGC_PID_BROAD_DOTA_SUMMON_MONSTER");
		}
		else
			++m_iNowCoolTime;
	}
}

void KGCDotaMonsterSummonGate::Render(void)
{
#if !defined( __PATH__ ) // 오브젝트의 바운딩박스
	{
		if (g_kGlobalValue.m_bRenderBoundBox)
		{
			g_pGCDeviceManager2->PushState();
			g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			GCCollisionRect<float> rtRect = GetCollisionRect();
			D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
			SETVECTOR3(lefttop, rtRect.m_Left, rtRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(righttop, rtRect.m_Right, rtRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(leftbottom, rtRect.m_Left, rtRect.m_Bottom - 0.25f, 0.4f);
			SETVECTOR3(rightbottom, rtRect.m_Right, rtRect.m_Bottom - 0.25f, 0.4f);

			g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
				lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
				D3DCOLOR_ARGB(180, 0, 0, 200));

			g_pGCDeviceManager2->PopState();
		}
	}
#endif
}

int KGCDotaMonsterSummonGate::CheckSummon()
{
	int iResult = -1;
	int iSummonCnt = 0;
	int iMaxSummonLimit = m_sGateInfo.iSummonLimit;

	// 코어가 파괴되었을땐 몬스터 소환을 하지 않는다.
	MONSTER *pMonsterSerdin = g_kMonsterManager.GetMonster(m_sTemplateInfo.iCoreSerdin_MonSlotID);
	MONSTER *pMonsterCanaban = g_kMonsterManager.GetMonster(m_sTemplateInfo.iCoreCanaban_MonSlotID);

	if (pMonsterSerdin == NULL || pMonsterCanaban == NULL)
		return iResult;

	if ((false == pMonsterSerdin->IsLive()) ||
		(false == pMonsterCanaban->IsLive()))
		return iResult;

	iResult = g_kMonsterManager.GetNextCreateMonstersNum();

	//for(int iLoop=m_sTemplateInfo.iSummonMonsterStartSlotIndex; iLoop<END_DOTA_MONSTER; ++iLoop)
	//{
	//    // 살아있는 몬스터가 이 게이트 몬스터인지 확인
	//    if( (g_kMonsterManager.m_mapMonster[ iLoop ]->IsLive() ) &&
	//        (g_kMonsterManager.m_mapMonster[ iLoop ]->m_dwOwnerUID == m_sGateInfo.iGateID) )
	//        ++iSummonCnt;

	//    // 소환할 수 있는 슬롯인지 확인
	//    else if( (-1 == iResult) &&
	//             (g_kMonsterManager.m_mapMonster[ iLoop ]->IsLive() == false ) &&
	//             g_kMonsterManager.m_mapMonster[ iLoop ]->GetImitSummon() == false )
	//    {
	//        // 임시 소환( 패킷받고 살리면 타이밍이 늦다. 슬롯 덮어쓴다. )
	//        g_kMonsterManager.m_mapMonster[ iLoop ]->SetImitSummon( true );
	//        iResult = iLoop;
	//    }

	//    // 소환 마릿수 제한
	//    if( iSummonCnt >= iMaxSummonLimit )
	//    {
	//        // 임시소환 취소
	//        if( g_kMonsterManager.IsMonsterIndex( iResult ) )
	//            g_kMonsterManager.m_mapMonster[ iResult ]->SetImitSummon( false );

	//        iResult = -1;
	//        break;
	//    }
	//}

	return iResult;

	//return iResult;
}

// 점령전 몬스터 포탈
// 2011, 6, 2, shmhlove
KGCDotaMonsterPortal::KGCDotaMonsterPortal(void)
	: m_iMapID(0)
{}
KGCDotaMonsterPortal::~KGCDotaMonsterPortal(void) {}
void KGCDotaMonsterPortal::Create(void)
{
	KGCMonsterBuff::Create();

	// MapID가 1보다 작을일은 던전스크립트에서 Property값을 잘못줬을때이다.!!
	if (1 > m_iMonsterType)
		return;

	// Property속성으로 받은 값을 MapID로 처리
	m_iMapID = m_iMonsterType;

	// 몬스터포탈 정보 얻어오기( 게이트ID가 없다면 크래시 나게 놔둔다.. 스크립트 오류이니깐.. 크래시로 알려야함.. )
	std::vector<SDotaMonsterPortalInfo>::iterator vecMonsterPortalInfoIter = g_kGlobalValue.m_vecDotaTemplate[(m_iMapID - 1)].vecsMonsterPortalInfo.begin();
	for (; vecMonsterPortalInfoIter != g_kGlobalValue.m_vecDotaTemplate[(m_iMapID - 1)].vecsMonsterPortalInfo.end(); ++vecMonsterPortalInfoIter)
	{
		if (m_iTgID == vecMonsterPortalInfoIter->iPortalID)
			m_sMonsterPortalInfo = (*vecMonsterPortalInfoIter);
	}

	// 포탈 크기 설정
	SetWidth(m_sMonsterPortalInfo.vSize.x);
	SetHeight(m_sMonsterPortalInfo.vSize.y);
}
void KGCDotaMonsterPortal::Destroy(void) {}
void KGCDotaMonsterPortal::FrameMove(float fElapsedTime)
{
	KGCMonsterBuff::FrameMove(fElapsedTime);

	if (g_kGlobalValue.m_kUserInfo.bHost)
	{
		GCCollisionRect<float> rtCollision;
		GCCollisionRect<float> rtPortalRect = GetCollisionRect();

		for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
			if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first))
				continue;
			MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

			if (pMonster == NULL) continue;

			if (false == pMonster->IsLive())
				continue;

			if (m_sMonsterPortalInfo.iTeam != pMonster->m_iTeam)
				continue;

			for (int iLoopRect = 0; iLoopRect < pMonster->GetAttackBoundRectNum(); ++iLoopRect)
			{
				GCCollisionRect<float> rtMonster = pMonster->GetAttackBoundRect(iLoopRect);

				if (rtMonster.CheckCollision(rtPortalRect, &rtCollision))
				{
					//rtCollision이걸 이용해서 파티클 작업가능 함.
					pMonster->SetX(m_sMonsterPortalInfo.vMovePos.x);
					pMonster->SetY(m_sMonsterPortalInfo.vMovePos.y);
				}
			}
		}
	}
}

void KGCDotaMonsterPortal::Render(void)
{
#if !defined( __PATH__ ) // 오브젝트의 바운딩박스
	{
		if (g_kGlobalValue.m_bRenderBoundBox)
		{
			KGCMonsterBuff::Render();
			g_pGCDeviceManager2->PushState();
			g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			GCCollisionRect<float> rtRect = GetCollisionRect();
			D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
			SETVECTOR3(lefttop, rtRect.m_Left, rtRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(righttop, rtRect.m_Right, rtRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(leftbottom, rtRect.m_Left, rtRect.m_Bottom - 0.25f, 0.4f);
			SETVECTOR3(rightbottom, rtRect.m_Right, rtRect.m_Bottom - 0.25f, 0.4f);

			g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
				lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
				D3DCOLOR_ARGB(180, 0, 0, 200));

			g_pGCDeviceManager2->PopState();
		}
	}
#endif
}

GCCollisionRect<float> KGCDotaMonsterPortal::GetCollisionRect(void)
{
	GCCollisionRect<float> rtRect;
	rtRect.m_Left = m_fPosX - 1.255f;
	rtRect.m_Right = m_fPosX + m_fWidth - 1.155f;
	rtRect.m_Top = m_fPosY + m_fHeight - 0.25f;
	rtRect.m_Bottom = m_fPosY - 0.28f;

	return rtRect;
}

// 점령전 DPoint샵
// 2011, 6, 10, shmhlove
KGCDotaDPointShop::KGCDotaDPointShop(void)
	: m_iMapID(0)
	, m_pCurDotaInfo(NULL)
{}
KGCDotaDPointShop::~KGCDotaDPointShop(void) {}
void KGCDotaDPointShop::Create(void)
{
	KGCMonsterBuff::Create();

	// MapID가 1보다 작을일은 던전스크립트에서 Property값을 잘못줬을때이다.!!
	if (1 > m_iMonsterType)
		return;

	// Property속성으로 받은 값을 MapID로 처리
	m_iMapID = m_iMonsterType;

	// DPointShop 정보 얻어오기( 게이트ID가 없다면 크래시 나게 놔둔다.. 스크립트 오류이니깐.. 크래시로 알려야함.. )
	std::vector<SDota_D_PointShopInfo>::iterator vecDPointShopInfoIter = g_kGlobalValue.m_vecDotaTemplate[(m_iMapID - 1)].vecsDPointShopInfo.begin();
	for (; vecDPointShopInfoIter != g_kGlobalValue.m_vecDotaTemplate[(m_iMapID - 1)].vecsDPointShopInfo.end(); ++vecDPointShopInfoIter)
	{
		if (m_iTgID == vecDPointShopInfoIter->iShopID)
			m_sDPointShopInfo = (*vecDPointShopInfoIter);
	}

	// 현재 도타템플릿 얻어두기
	m_pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();

	// ?표시용 2D이미지 로드
	if (TEAM_SERDIN == m_sDPointShopInfo.iTeam)
		m_pTexBase = g_pGCDeviceManager2->CreateTexture("dota_shop_serdin.dds");
	else
		m_pTexBase = g_pGCDeviceManager2->CreateTexture("dota_shop_canaban.dds");

	// Shop Open 이펙트
	CParticleEventSeqPTR pParticle;
	pParticle = g_ParticleManager->CreateSequence("Cursor_Space_01", m_fPosX - 0.95f, m_fPosY - 0.55f, 1.0f);
	if (pParticle != NULL)    m_vecParticleShopOpen.push_back(pParticle);
	pParticle = g_ParticleManager->CreateSequence("Cursor_Space_02", m_fPosX - 0.95f, m_fPosY - 0.55f, 1.0f);
	if (pParticle != NULL)    m_vecParticleShopOpen.push_back(pParticle);
	pParticle = g_ParticleManager->CreateSequence("Cursor_Space_03", m_fPosX - 0.95f, m_fPosY - 0.55f, 1.0f);
	if (pParticle != NULL)    m_vecParticleShopOpen.push_back(pParticle);

	// Shop 충돌 크기 설정
	SetWidth(m_sDPointShopInfo.vSize.x);
	SetHeight(m_sDPointShopInfo.vSize.y);
}

void KGCDotaDPointShop::Destroy(void)
{
	KGCMonsterBuff::Destroy();

	std::vector< CParticleEventSeqPTR >::iterator vecIter = m_vecParticleShopOpen.begin();
	for (; vecIter != m_vecParticleShopOpen.end(); ++vecIter)
	{
		g_ParticleManager->DeleteSequence((*vecIter));
	}
	m_vecParticleShopOpen.clear();
}

void KGCDotaDPointShop::FrameMove(float fElapsedTime)
{
	KGCMonsterBuff::FrameMove(fElapsedTime);

	// 事?열려져 있으면 들어가지 말아라
	m_bCollision = false;
	if (KD3DWnd::D3DWS_ENABLED != g_pkUIScene->GetStateUI(KGCUIScene::GC_MBOX_DOTA_SHOP) &&
		KD3DWnd::D3DWS_ENABLED != g_pkUIScene->GetStateUI(KGCUIScene::GC_MBOX_EXIT_GAME))
	{
		PLAYER *pPlayer = g_MyD3D->GetMyPlayer();
		if (pPlayer)
		{
			GCCollisionRect<float> rtCollision;
			GCCollisionRect<float> rtShopRect = GetCollisionRect();

			// 팀체크
			if (m_sDPointShopInfo.iTeam != pPlayer->m_kUserInfo.iTeam)
				return;

			// 충돌확인
			GCCollisionRect<float>& rtBody = pPlayer->GetCollisionRect();
			if (!(m_bCollision = rtBody.CheckCollision(rtShopRect, &rtCollision)))
				goto END_FUCN;

			// Shop오픈키 입력체크
			if (false == g_MyD3D->MyCtrl->k_DotaShop)
				goto END_FUCN;

			// Shop오픈
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_DOTA_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, false);
		}
	}

END_FUCN:
	// 충돌 시 Push이펙트를 띄우자!!
	EnableParticle(m_bCollision);

	// 충돌 시 DPointShop에 대한 설명을 출력하자!!
	KGCInGameMessage* pMessage = g_pkUIScene->m_pkGameOverlayUI->GetInGameMessage();
	if (m_bCollision && m_pCurDotaInfo)
	{
		if (-1 == pMessage->IsRenderToString(m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_SHOP].first))
		{
			KInGameMessageAttribute kMessage;
			kMessage.iStringID = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_SHOP].first;
			kMessage.iShowFrame = m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_SHOP].second;
			pMessage->SetMessage(&kMessage);
		}
	}
	else if (0 == pMessage->IsRenderToString(m_pCurDotaInfo->sDotaMsgInfo.mapDotaMsg[SDotaPlayingDesc::DOTA_MSG_SHOP].first))
	{
		pMessage->SetStringID(-1);
	}

	// ShopOpen키와 Collsion값을 초기화 하자!!
	g_MyD3D->MyCtrl->k_DotaShop = false;
	m_bCollision = false;
}

void KGCDotaDPointShop::Render(void)
{
	KGCMonsterBuff::Render();

#if !defined( __PATH__ ) // 오브젝트의 바운딩박스
	{
		if (g_kGlobalValue.m_bRenderBoundBox)
		{
			g_pGCDeviceManager2->PushState();
			g_pGCDeviceManager2->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			GCCollisionRect<float> rtRect = GetCollisionRect();
			D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
			SETVECTOR3(lefttop, rtRect.m_Left, rtRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(righttop, rtRect.m_Right, rtRect.m_Top - 0.25f, 0.4f);
			SETVECTOR3(leftbottom, rtRect.m_Left, rtRect.m_Bottom - 0.25f, 0.4f);
			SETVECTOR3(rightbottom, rtRect.m_Right, rtRect.m_Bottom - 0.25f, 0.4f);

			g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
				lefttop, righttop, leftbottom, rightbottom, 0, 0, 0,
				D3DCOLOR_ARGB(180, 0, 0, 200));

			g_pGCDeviceManager2->PopState();
		}
	}
#endif
}

GCCollisionRect<float> KGCDotaDPointShop::GetCollisionRect(void)
{
	GCCollisionRect<float> rtRect;
	rtRect.m_Left = m_fPosX - 1.255f;
	rtRect.m_Right = m_fPosX + m_fWidth - 1.155f;
	rtRect.m_Top = m_fPosY + m_fHeight - 0.25f;
	rtRect.m_Bottom = m_fPosY - 0.28f;

	return rtRect;
}

void KGCDotaDPointShop::EnableParticle(bool bShow)
{
	std::vector< CParticleEventSeqPTR >::iterator vecIter = m_vecParticleShopOpen.begin();
	for (; vecIter != m_vecParticleShopOpen.end(); ++vecIter)
	{
		(*vecIter)->SetShow(bShow);
	}
}