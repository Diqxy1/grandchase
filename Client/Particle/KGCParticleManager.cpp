#include "stdafx.h"
// KGCParticleManager.cpp: implementation of the KGCParticleManager class.
//
//////////////////////////////////////////////////////////////////////
#include "KGCParticleManager.h"
#include "MyD3D.h"

KGCParticleManager* g_ParticleManager = NULL;
PARTICLE_SEQ CParticleEventSeqPTR::m_ParticleDummy;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KGCParticleManager::KGCParticleManager()
{
	g_ParticleManager = this;
	m_pmatCamera = NULL;

	lua_tinker::class_add<KGCParticleManager>(KGCLuabinder::getInstance().GetLuaState(), "KGCParticleManagerClass");
	lua_tinker::class_def<KGCParticleManager>(KGCLuabinder::getInstance().GetLuaState(), "CreateSequence", &KGCParticleManager::CreateSequence);;

	lua_tinker::decl(KGCLuabinder::getInstance().GetLuaState(), "KGCParticleManager", this);
}

void KGCParticleManager::InitDevice(LPDIRECT3DDEVICE9 pd3dDevice, D3DXMATRIX* pmatCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_pmatCamera = pmatCamera;

	m_ParticleSystem.InitDevice();
	m_ParticleSystem.SetZEnable(false);

	D3DXMatrixIdentity(&m_matIdentity);
}

KGCParticleManager::~KGCParticleManager()
{
	Clear();
}

void KGCParticleManager::DeleteAllInstance()
{
	m_ParticleSystem.DestroyAllInstance();
}

void KGCParticleManager::Clear()
{
	m_ParticleSystem.Clear();

	for (std::vector<ITEM_EFFECT>::iterator i = m_vecItemEff.begin(); i != m_vecItemEff.end(); ++i)
		(*i).itemName.clear();

	for (std::vector<ITEM_EFFECT>::iterator j = m_vecDamageEff.begin(); j != m_vecDamageEff.end(); ++j)
		(*j).itemName.clear();

	for (std::vector<ITEM_EFFECT>::iterator k = m_vecSkillEff.begin(); k != m_vecSkillEff.end(); ++k)
		(*k).itemName.clear();

	m_vecItemEff.clear();
	m_vecDamageEff.clear();
	m_vecSkillEff.clear();

	//  더미 객체 해제
	CParticleEventSeqPTR::ClearDummy();
}

void KGCParticleManager::Load(char* strParticle, char* strLua)
{
	CompileScript(strParticle);
	LoadFromLuaScript(strLua);
}

void KGCParticleManager::LoadFromLuaScript(char* filename)
{
	KLuaManager kLuaMgr;

	GCMemoryFile memfile_enum(g_pGCDeviceManager2->GetMassFileManager(), "Enum.stg");

	if (!memfile_enum.IsLoaded())
		return;

	if (FAILED(kLuaMgr.DoString(memfile_enum.GetDataPointer(), memfile_enum.GetFileSize())))
		return;

	GCMemoryFile memfile_etc(g_pGCDeviceManager2->GetMassFileManager(), "etc.stg");

	if (!memfile_etc.IsLoaded())
		return;

	if (FAILED(kLuaMgr.DoString(memfile_etc.GetDataPointer(), memfile_etc.GetFileSize())))
		return;

	GCMemoryFile memfile(g_pGCDeviceManager2->GetMassFileManager(), filename);

	if (!memfile.IsLoaded())
		return;

	if (FAILED(kLuaMgr.DoString(memfile.GetDataPointer(), memfile.GetFileSize())))
		return;

	LoadTable("ITEM_EFF", m_vecItemEff, kLuaMgr, EGCItem);
	LoadTable("DAMAGE_EFF", m_vecDamageEff, kLuaMgr, EGCDamage);
	LoadTable("SKILL_EFF", m_vecSkillEff, kLuaMgr, EGCSkill);
}

void KGCParticleManager::LoadTable(char* strTable, std::vector<ITEM_EFFECT>& vec, KLuaManager& kLuaMgr, EGCItemEffect egcItemEffect)
{
	int	i, j;

	std::string strItemName;

	kLuaMgr.BeginTable(strTable);
	{
		for (i = 1; ; ++i)
		{
			ITEM_EFFECT itemEff;
			if (kLuaMgr.BeginTable(i) == E_FAIL)
				break;
			std::string seqName;
			int charID;
			int enchantLv;
			int iTemp;

			if (kLuaMgr.BeginTable("SEQUENCE_NAME") == S_OK)
			{
				for (int i = 1; kLuaMgr.GetValue(i++, seqName) == S_OK; itemEff.sequenceName.push_back(seqName));
				kLuaMgr.EndTable();
			}
			else
			{
				if (kLuaMgr.GetValue("SEQUENCE_NAME", seqName) == S_OK)
					itemEff.sequenceName.push_back(seqName);
			}
			if (kLuaMgr.BeginTable("CHAR_IDENTY") == S_OK)
			{
				for (int i = 1; kLuaMgr.GetValue(i++, charID) == S_OK; itemEff.charID.push_back(charID));
				kLuaMgr.EndTable();
			}
			else
			{
				if (kLuaMgr.GetValue("CHAR_IDENTY", charID) == S_OK)
					itemEff.charID.push_back(charID);
			}

			if (kLuaMgr.BeginTable("ENCHANT_LEVEL") == S_OK)
			{
				for (int i = 1; kLuaMgr.GetValue(i++, enchantLv) == S_OK; itemEff.enchantLevel.push_back(enchantLv));
				kLuaMgr.EndTable();
			}
			else
			{
				if (kLuaMgr.GetValue("ENCHANT_LEVEL", iTemp) == S_OK)
					itemEff.enchantLevel.push_back(iTemp);
			}

			if (kLuaMgr.BeginTable("POSITION_TYPE") == S_OK)
			{
				for (int i = 1; kLuaMgr.GetValue(i++, iTemp) == S_OK; itemEff.emitPositionType.push_back((EMIT_POSITION_TYPE)iTemp));
				kLuaMgr.EndTable();
			}
			else
			{
				if (kLuaMgr.GetValue("POSITION_TYPE", iTemp) == S_OK)
					itemEff.emitPositionType.push_back((EMIT_POSITION_TYPE)iTemp);
			}

			if (itemEff.emitPositionType.empty())
			{
				itemEff.emitPositionType.push_back(EPT_BODY_CENTER);
			}

			if (FAILED(kLuaMgr.GetValue("TRACE", itemEff.m_bTrace)))
				itemEff.m_bTrace = true;
			if (FAILED(kLuaMgr.GetValue("PARTICLE_TRACE_TIME", itemEff.m_fParticleTraceTime)))
				itemEff.m_fParticleTraceTime = 0.3f;
			if (FAILED(kLuaMgr.GetValue("ISDIR_VELOCITY", itemEff.m_bDirVelocity)))
				itemEff.m_bDirVelocity = false;
			if (FAILED(kLuaMgr.GetValue("DIR_POS_X", itemEff.m_fDirPosX)))
				itemEff.m_fDirPosX = 0.0f;

			itemEff.m_EGCItemEffect = egcItemEffect;
			kLuaMgr.BeginTable("ITEM_NAME");
			{
				for (j = 1; ; ++j)
				{
					if (FAILED(kLuaMgr.GetValue(j, strItemName)))
						break;

#if defined(RESOURCE_GRAPH)
					char tmp[256];
					sprintf(tmp, "ITEM_%s_%d", strItemName.c_str(), itemEff.enchantLevel);
					g_pGCDeviceManager2->AddRelation("PARTICLE_" + itemEff.sequenceName, tmp);
#endif
					itemEff.itemName.push_back(strItemName);
				}
			}
			kLuaMgr.EndTable();

			if (FAILED(kLuaMgr.GetValue("X_OFFSET", itemEff.m_fOffsetX)))
				itemEff.m_fOffsetX = 0.0f;

			if (FAILED(kLuaMgr.GetValue("Y_OFFSET", itemEff.m_fOffsetY)))
				itemEff.m_fOffsetY = 0.0f;

			vec.push_back(itemEff);

			kLuaMgr.EndTable();
		}
	}
	kLuaMgr.EndTable();
}

CParticleEventSeqPTR KGCParticleManager::CreateSequence(const std::string& name, float x, float y, float z)
{
	CKTDGParticleSystem::CParticleEventSequence* seq = m_ParticleSystem.CreateSequence(name, x, y, z);

	if (!seq)
		return NULL;

	CParticleEventSeqPTR EventSeq = seq;

#if defined(RESOURCE_GRAPH)
	if (seq)
	{
		g_pGCDeviceManager2->AddRelation("PARTICLE_" + name, "STATE_" + g_MyD3D->m_pStateMachine->GetStateString());
	}
#endif

	return EventSeq;
}

CKTDGParticleSystem::CParticleEventSequence* KGCParticleManager::GetSequence(PARTICLE_UID _dwUID)
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = m_ParticleSystem.GetSequence(_dwUID);

	return pSeq;
}

void KGCParticleManager::CreateParticle(CParticleEventSeqPTR pSeq, int iCount, const D3DXVECTOR3& vPos)
{
	if (NULL == pSeq || false == IsLiveInstance(pSeq))
		return;

	for (int i = 0; i < iCount; ++i)
	{
		if (pSeq->GetNumFreeParticles() > 0)
		{
			try
			{
				pSeq->CreateNewParticle(vPos);
			}
			catch (...) {}
		}
	}
}

void KGCParticleManager::DeleteSequence(CParticleEventSeqPTR Seq)
{
	if (Seq.m_dwUID != UINT_MAX)
		m_ParticleSystem.DestroyInstance(Seq.m_dwUID);
}

void KGCParticleManager::DeleteSequence(std::vector<CParticleEventSeqPTR>& vecSeq)
{
	for (unsigned int i = 0; i < vecSeq.size(); i++)
	{
		DeleteSequence(vecSeq[i]);
	}
	vecSeq.clear();
}

bool KGCParticleManager::IsLiveInstance(CParticleEventSeqPTR Seq)
{
	return m_ParticleSystem.IsLiveInstance(Seq.m_dwUID);
}

void KGCParticleManager::FrameMove(float fTime, float elapsedTime)
{
	PROFILE_SET("KGCParticleManager::FrameMove");

	if (g_kGlobalValue.m_bFrameLock && o3 != 0)
		return;

	m_ParticleSystem.FrameMove(fTime, elapsedTime);
}

void KGCParticleManager::Render(int iLayer)
{
	// UI or SKY 인 경우 월드 행렬 변환을 따로 하지 않는다.
	if (GC_LAYER_UI == iLayer || GC_LAYER_SKY == iLayer || GC_LAYER_CUSTOM == iLayer || GC_LAYER_GACHA == iLayer || GC_LAYER_UNDER_UI == iLayer ||
		GC_LAYER_UPPER_SQUARE_UI_0 == iLayer || GC_LAYER_UPPER_SQUARE_UI_1 == iLayer || GC_LAYER_UPPER_SQUARE_UI_2 == iLayer ||
		GC_LAYER_UPPER_SQUARE_UI_3 == iLayer || GC_LAYER_PUZZLEBOX == iLayer || GC_LAYER_UI_SCENE == iLayer || GC_LAYER_AZIT_NPC == iLayer ||
		GC_LAYER_CUSTOM2 == iLayer ||
		GC_LAYER_CUSTOM_UI <= iLayer)
	{
		D3DXMATRIX matUIWorld;
		D3DXMatrixIdentity(&matUIWorld);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matUIWorld);
	}
	else
	{
		if (m_pmatCamera != NULL)
			m_pd3dDevice->SetTransform(D3DTS_WORLD, m_pmatCamera);
	}

	m_ParticleSystem.Render(iLayer);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matIdentity);
}

int KGCParticleManager::GetItemEffect(EGCItemEffect eEffect, const std::string& itemName, CHAR_IDENTY charID, int enchantLevel, std::vector<ITEM_EFFECT>& outvec)
{
	int iRet = 0;

	std::vector<ITEM_EFFECT>* vec;
	switch (eEffect)
	{
	case EGCItem: vec = &m_vecItemEff; break;
	case EGCDamage: vec = &m_vecDamageEff; break;
	case EGCSkill: vec = &m_vecSkillEff; break;
	default: return 0;
	}

	ITEM_EFFECT eff;

	for (std::vector<ITEM_EFFECT>::iterator i = vec->begin(); i != vec->end(); ++i)
	{
		eff = (*i);

		bool bEnable = false;

		// 아이템 이름 체크
		{
			std::vector<std::string>::iterator itor;
			for (itor = eff.itemName.begin(); itor != eff.itemName.end(); ++itor)
			{
				std::string strItemName = *itor;

				if (strItemName.compare(itemName) == 0)
				{
					bEnable = true;
					break;
				}
			}
		}
		if (!bEnable)
			continue;
		//강화 레벨 체크
		{
			std::vector<int>::iterator itor;
			for (itor = eff.enchantLevel.begin(); itor != eff.enchantLevel.end(); ++itor)
			{
				if (*itor == enchantLevel)
				{
					bEnable = true;
					break;
				}
				else
				{
					bEnable = false;
				}
			}
		}
		if (!bEnable)
			continue;
		//캐릭터 타입 체크
		{
			std::vector<int>::iterator itor;
			for (itor = eff.charID.begin(); itor != eff.charID.end(); ++itor)
			{
				if (*itor == charID ||
					*itor == CID_COMMON)
				{
					bEnable = true;
					break;
				}
				else
				{
					bEnable = false;
				}
			}
		}

		if (bEnable)
		{
			outvec.push_back(eff);
			iRet++;
		}
	}

	return iRet;
}

bool KGCParticleManager::SetLayer(CParticleEventSeqPTR pParticle, int iLayer)
{
	if (pParticle == NULL)
		return false;

	return m_ParticleSystem.SetLayer(pParticle, iLayer);
}

void KGCParticleManager::Z_Enable(bool bEnable)
{
	m_ParticleSystem.SetZEnable(bEnable);
}

void KGCParticleManager::ReleaseAllTexture()
{
	m_ParticleSystem.ReleaseAllTexture();
}

void KGCParticleManager::LevelUpEffect(const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence("Lv_Up_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_02", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_03", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_04", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_05", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_06", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_07", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_08", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_09", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_10", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_Font_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_Font_02", fx_, fy_, 0.5f), GC_LAYER_UI);

	g_KDSound.PlayNoOverlapping("LevelUpEffect", 0, 0);
}

void KGCParticleManager::SkillUpEffect(const float fx_, const float fy_)
{
#if defined(NATION_CHINA)
	return;
#endif
	this->SetLayer(this->CreateSequence("Skill_Up_Font_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Skill_Up_Font_02", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_02", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Skill_Up_03", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_04", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_08", fx_, fy_, 0.5f), GC_LAYER_UI);

	g_KDSound.PlayNoOverlapping("LevelUpEffect", 0, 0);
}

void KGCParticleManager::MissionComplateEffect(const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence("Mission_Com_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Mission_Com_02", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("MZP_Win_ef_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("MZP_Win_ef_02", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("MZP_Win_ef_05", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("MZP_Win_ef_06", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("MZP_Win_ef_07", fx_, fy_, 0.5f), GC_LAYER_UI);

	g_KDSound.PlayNoOverlapping("Mission_Complete", 0, 0);
}

void KGCParticleManager::MissionLevelUpEffect(const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence("Lv_Up_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_02", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_03", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_04", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_05", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_06", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_07", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_08", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Lv_Up_09", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Mission_Com_03", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Mission_Com_04", fx_, fy_, 0.5f), GC_LAYER_UI);

	g_KDSound.PlayNoOverlapping("LevelUpEffect", 0, 0);
}

void KGCParticleManager::MissionNewEffect(const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence("Special_Reward_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Special_Reward_03", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Mission_New_01", fx_ - 0.16f, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Mission_New_02", fx_ - 0.16f, fy_, 0.5f), GC_LAYER_UI);

	g_KDSound.PlayNoOverlapping("RainbowEventSound", 0, 0);
}

void KGCParticleManager::PetDunGaugeEffect(const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence("Petdun_UI_Gage_01", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Petdun_UI_Gage_02", fx_, fy_, 0.5f), GC_LAYER_UI);
	this->SetLayer(this->CreateSequence("Petdun_UI_Gage_03", fx_, fy_, 0.5f), GC_LAYER_UI);
}

void KGCParticleManager::MissionRewardEffect(const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence("Mission_Reward_Button", fx_, fy_, 0.5f), GC_LAYER_UI);
}

void KGCParticleManager::MissionCompleteEffect(const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence("Mission_Com_Button", fx_, fy_, 0.5f), GC_LAYER_UI);
}

void KGCParticleManager::MissionCompleteSubEffet(const std::string& strParticle_, const float fX_, const float fY_)
{
	this->SetLayer(this->CreateSequence(strParticle_, fX_, fY_, 0.5f), GC_LAYER_UI);
}

void KGCParticleManager::CompileScript(const char* strParticle)
{
	m_ParticleSystem.OpenScriptFile(strParticle);
}

CParticleEventSeqPTR KGCParticleManager::EnchantEffect(const std::string& strSquence_, const float fX_, const float fY_, int iLayer /*= 0*/)
{
	CParticleEventSeqPTR pParticle = this->CreateSequence(strSquence_, fX_, fY_, 0.5f);
	this->SetLayer(pParticle, GC_LAYER_CUSTOM_UI + iLayer);
	return pParticle;
}

int KGCParticleManager::GetMemorySize()
{
	return m_ParticleSystem.GetMemorySize();
}

void KGCParticleManager::GetLoadedTexture(int& nTotal, int& nLoad)
{
	return m_ParticleSystem.GetLoadedTexture(nTotal, nLoad);
}

void KGCParticleManager::LogUseParticleList()
{
	return m_ParticleSystem.LogUseParticleList();
}

void KGCParticleManager::CreateSequenceReceiveName(std::string strParticleName, const float fx_, const float fy_)
{
	this->SetLayer(this->CreateSequence(strParticleName.c_str(), fx_, fy_, 0.5f), GC_LAYER_CUSTOM);
}

CParticleEventSeqPTR::CParticleEventSeqPTR()
{
	ReSet();
}

CParticleEventSeqPTR::CParticleEventSeqPTR(PARTICLE_SEQ* seq)
{
	if (!seq)
	{
		ReSet();
	}
	else
	{
		m_dwUID = seq->GetUID();
		m_strParticleName = seq->GetName();
	}
}

CParticleEventSeqPTR::~CParticleEventSeqPTR()
{
	ClearDummy();
}

//  디버깅용 포인터 때문에 만들어야 한다.
CParticleEventSeqPTR::CParticleEventSeqPTR(const CParticleEventSeqPTR& EventSeq)
{
	m_dwUID = EventSeq.m_dwUID;
	m_strParticleName = EventSeq.m_strParticleName;
}

const CParticleEventSeqPTR& CParticleEventSeqPTR::operator=(const CParticleEventSeqPTR& EventSeq)
{
	m_dwUID = EventSeq.m_dwUID;
	m_strParticleName = EventSeq.m_strParticleName;

	return *this;
}

void CParticleEventSeqPTR::ReSet()
{
	m_dwUID = UINT_MAX;
	m_strParticleName = "";

	m_iBone = -1;
}

void CParticleEventSeqPTR::ClearDummy()
{
	m_ParticleDummy.DestoryParticleEventSeq();
}

bool CParticleEventSeqPTR::operator < (const CParticleEventSeqPTR& comp) const
{
	if (m_dwUID < comp.m_dwUID)
		return true;

	return false;
}

PARTICLE_SEQ* CParticleEventSeqPTR::operator -> ()
{
	if (g_ParticleManager)
	{
		PARTICLE_SEQ* pSeq = g_ParticleManager->GetSequence(m_dwUID);

		if (pSeq)
			return pSeq;
	}

	//  죽는걸 막기위해 더미를 생성하여 리턴한다.
	//  디버깅 코드이다. 제작일시 2011.09.26
	//  좀지나면 반드시 지우자.

	// 내부테스트에선 포인터가 없을때 메세지박스를 뛰운다.
#if !defined( __PATH__ )
	{
		std::string strTemp = boost::str(boost::format("Particle Point NULL %d %s") % m_dwUID%m_strParticleName);
		MessageBoxA(NULL, strTemp.c_str(), "", MB_OK);
		return NULL;
	}
#endif;

	if (!m_ParticleDummy.GetParticleEventSeq())
	{
		m_ParticleDummy.NewDummyParticleEventSeq();
	}

	return &m_ParticleDummy;
}

CParticleEventSeqPTR::operator PARTICLE_SEQ* ()
{
	if (!g_ParticleManager)
		return NULL;

	PARTICLE_SEQ* pSeq = g_ParticleManager->GetSequence(m_dwUID);

	return pSeq;
}

CParticleEventSeqPTR& CParticleEventSeqPTR::operator = (PARTICLE_SEQ* seq)
{
	if (seq == NULL)
	{
		ReSet();
	}
	else
	{
		m_dwUID = seq->GetUID();
		m_strParticleName = seq->GetName();
	}

	return *this;
}

bool CParticleEventSeqPTR::IsValid()
{
	PARTICLE_SEQ* pSeq = NULL;
	if (g_ParticleManager)
		pSeq = g_ParticleManager->GetSequence(m_dwUID);

	return (NULL == pSeq) ? false : true;
}