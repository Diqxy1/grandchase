// KGCParticleManager.h: interface for the KGCParticleManager class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPARTICLEMANAGER_H__22C6D911_FDC6_4326_BACD_9290BFE6DCAD__INCLUDED_)
//#define AFX_KGCPARTICLEMANAGER_H__22C6D911_FDC6_4326_BACD_9290BFE6DCAD__INCLUDED_

#ifndef _KGCPARTICLEMANAGER_H_
#define _KGCPARTICLEMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#include "KTDGParticleSystem.h"
//#include "../GCLuaManager/KLuaManager.h"
//#include "../GCEnum.h"
#include <fstream>
//#include <vector>

typedef CKTDGParticleSystem::CParticleEventSequence PARTICLE_SEQ;

struct PositionDesc;

// 파티클 포인터로 가지고 접근을 하는것을 막기위해 만든 클래스 입니다.
// 파티클 생성시 UID를 설정하고 해당 UID로 파티클에 접근합니다.
// 기존 클래스의 인터페이스를 동일하게 하기 위해서 클래스로 래핑하였습니다.
class CParticleEventSeqPTR
{
public:
	CParticleEventSeqPTR();
	CParticleEventSeqPTR(PARTICLE_SEQ* seq);
	~CParticleEventSeqPTR();

	//  디버깅용 포인터 때문에 만들어야 한다.
	CParticleEventSeqPTR(const CParticleEventSeqPTR& EventSeq);
	const CParticleEventSeqPTR& operator=(const CParticleEventSeqPTR& EventSeq);

	bool operator < (const CParticleEventSeqPTR& comp) const;

	PARTICLE_SEQ* operator -> ();
	operator PARTICLE_SEQ* ();
	CParticleEventSeqPTR& operator = (PARTICLE_SEQ* seq);

public:
	bool IsValid();

private:
	bool IsEnable() { return m_dwUID != UINT_MAX; }
	void ReSet();

public:

	PARTICLE_UID    m_dwUID;

	//  디버깅용 코드 변수 나중에 지워주자. ( 2011.09.24 )
	std::string     m_strParticleName;
	int             m_iBone;

	static PARTICLE_SEQ   m_ParticleDummy;
	static void ClearDummy();
};

enum EGCItemEffect
{
	EGCUnknown = -1,
	EGCItem = 0,
	EGCDamage = 1,
	EGCSkill = 2,
};

struct ITEM_EFFECT
{
public:
	ITEM_EFFECT()
	{
		m_bTrace = false;
		m_fParticleTraceTime = 0.0f;

		m_bDirVelocity = true;
		m_fDirPosX = 0.0f;
		m_fOffsetX = 0.0f;
		m_fOffsetY = 0.0f;

		m_EGCItemEffect = EGCUnknown;
	}
	std::vector<std::string>  itemName;
	std::vector<std::string> sequenceName;

	std::vector<EMIT_POSITION_TYPE> emitPositionType;

	std::vector<int> charID;
	std::vector<int> enchantLevel;

	bool m_bTrace;
	float m_fParticleTraceTime;

	bool m_bDirVelocity;
	float m_fDirPosX;
	float m_fOffsetX;
	float m_fOffsetY;

	std::vector<CParticleEventSeqPTR> pSeq;
	EGCItemEffect m_EGCItemEffect;
};

class KGCParticleManager
{
public:
	KGCParticleManager();
	virtual ~KGCParticleManager();

	// Load
	void Load(char* strParticle, char* strLua);
	void CompileScript(const char* strParticle);

	void InitDevice(LPDIRECT3DDEVICE9 pd3dDevice, D3DXMATRIX* pmatCamera = NULL);

	void FrameMove(float fTime, float elapsedTime = 0.01818f);
	void Render(int iLayer);

	// 아이템 이펙트 관련
	int GetItemEffect(EGCItemEffect eEffect, const std::string& itemName, CHAR_IDENTY charID, int enchantLevel, std::vector<ITEM_EFFECT>& outvec);

	//  포인터 받아서 쓰지마세요.
	//  내부적으로 삭제 되므로 받아서 쓰면 안됩니다.
	CParticleEventSeqPTR CreateSequence(const std::string& name, float x = 0.0f, float y = 0.0f, float z = 0.0f);

	bool SetLayer(CParticleEventSeqPTR pParticle, int iLayer);
	void CreateParticle(CParticleEventSeqPTR pSeq, int iCount, const D3DXVECTOR3& vPos);

	CKTDGParticleSystem::CParticleEventSequence* GetSequence(PARTICLE_UID _dwUID);

	void DeleteSequence(std::vector<CParticleEventSeqPTR>& vecSeq);
	void DeleteSequence(CParticleEventSeqPTR pSeq);
	bool IsLiveInstance(CParticleEventSeqPTR pSeq);

	void ReleaseAllTexture();
	void DeleteAllInstance();
	void Clear();

	void Z_Enable(bool bEnable);

	//////////////////////////////////////////////////////////////////////////
	//  Hard Particle
	void LevelUpEffect(const float fx_, const float fy_);
	void SkillUpEffect(const float fx_, const float fy_);
	void MissionComplateEffect(const float fx_, const float fy_);
	void MissionLevelUpEffect(const float fx_, const float fy_);
	void MissionNewEffect(const float fx_, const float fy_);
	void MissionRewardEffect(const float fx_, const float fy_);
	void MissionCompleteEffect(const float fx_, const float fy_);
	void MissionCompleteSubEffet(const std::string& strParticle_, const float fX_, const float fY_);
	void PetDunGaugeEffect(const float fx_, const float fy_);
	CParticleEventSeqPTR EnchantEffect(const std::string& strSquence_, const float fX_, const float fY_, int iLayer = 0);

	// Log용
public:
	int GetMemorySize();
	void GetLoadedTexture(int& nTotal, int& nLoad);
	void LogUseParticleList();
	void CreateSequenceReceiveName(std::string strParticleName, const float fx_, const float fy_);

private:
	void LoadFromLuaScript(char* filename);
	D3DXVECTOR3* ComputePosDesc(int ePosType, PositionDesc* pPos);
	void LoadTable(char* strTable, std::vector<ITEM_EFFECT>& vec, KLuaManager& kLuaMgr, EGCItemEffect egcItemEffect);

private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	D3DXMATRIX* m_pmatCamera;
	D3DXMATRIX m_matIdentity;
	CKTDGParticleSystem m_ParticleSystem;

	// 특정 아이템과 이펙트를 연관짓는 구조체
	std::vector<ITEM_EFFECT>  m_vecItemEff;
	std::vector<ITEM_EFFECT>  m_vecDamageEff;
	std::vector<ITEM_EFFECT>  m_vecSkillEff;
};

extern KGCParticleManager* g_ParticleManager;

#endif // _KGCPARTICLEMANAGER_H_
