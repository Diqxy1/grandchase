#pragma once
#include "gcgameobject.h"

class CParticleEventSeqPTR;

class KGCMonsterBuff :
	public KGCGameObject
{
public:
	KGCMonsterBuff(void);
	virtual ~KGCMonsterBuff(void);

public:
	virtual void Create(void);
	virtual void Destroy(void);
	virtual void Enable(void);
	virtual void FrameMove(float fElapsedTime = GC_ELAPSED_TIME);
	virtual void Render(void);

	virtual GCCollisionRect<float> GetCollisionRect(void);

	GCDeviceTexture* m_pTexBase;
	int m_iTgID;                    // 어떤 트리거 인가?
	int m_iMonsterType;             // 어떤 몬스터와 연결되어있는 거니?
	int m_iInterval;                // 죽었을 때, 살아난다! 이만큼 후에!
	int m_iRemainTime;              // 실지 타이머
	std::vector<int> vecMonIndex;   // 내가 관리할 몬스터 인덱스~
	int m_iLife;                    // 몇대 ㅊ 맞으면 죽지?
	int DEFAULT_LIFE;

	std::vector< std::string > m_vecParticleStart;
	std::vector< CParticleEventSeqPTR > m_vecParticleActive;
	std::vector< CParticleEventSeqPTR > m_vecParticleCoolTime;

	virtual void Activate(void);
	virtual void ResetCoolTime();
	virtual void DoSomeThing() {};
	virtual void DoResetTime() {};

	virtual void EnableLive(bool bLive);

	virtual int GetLife() const { return m_iLife; }
	virtual void SetLife(int val) { m_iLife = val; }
	void SetMonsterType(int iMonType_) { m_iMonsterType = iMonType_; }
	void SetTgID(int iTgID_) { m_iTgID = iTgID_; }
};

class KGCMonsterDefence : public KGCMonsterBuff
{
public:
	KGCMonsterDefence(void);
	virtual ~KGCMonsterDefence(void);
	virtual void Create(void);
	virtual void Destroy(void);
	virtual void DoSomeThing();
	virtual void DoResetTime();
	virtual bool OnDamage(int iPlayerIndex, float fAttackPower, EGCAttackType eAttackType, EGCAttackDirection eAttackDirection, float fPushTargetX, float fPushTargetY, bool bSpecialAttack);
};

class KGCDemonCore : public KGCMonsterBuff
{
public:
	KGCDemonCore(void);
	virtual ~KGCDemonCore(void);
	virtual void Create(void);
	virtual void Destroy(void);
	virtual void DoSomeThing();
	virtual void DoResetTime();
	virtual void ResetCoolTime();
	virtual void FrameMove(float fElapsedTime = GC_ELAPSED_TIME);
	virtual bool OnDamage(int iPlayerIndex, float fAttackPower, EGCAttackType eAttackType, EGCAttackDirection eAttackDirection, float fPushTargetX, float fPushTargetY, bool bSpecialAttack);
	virtual bool IsCrashCheck() { return true; }

private:
	bool m_bStartTimer;
	bool m_bEnd;
};

class KGCDotaMonsterSummonGate : public KGCMonsterBuff
{
public:
	KGCDotaMonsterSummonGate(void);
	virtual ~KGCDotaMonsterSummonGate(void);

public:
	virtual void Create(void);
	virtual void Destroy(void);
	virtual void FrameMove(float fElapsedTime = GC_ELAPSED_TIME);
	virtual void Render(void);

public:
	int CheckSummon();

private:
	int m_iMapID;
	SDotaTemplate m_sTemplateInfo;
	SDotaGateInfo m_sGateInfo;
	D3DXVECTOR2 m_vSummonPos;
	std::vector< CParticleEventSeqPTR > m_vecParticleGate;

private:
	int m_iNowCoolTime;
};

class KGCDotaMonsterPortal : public KGCMonsterBuff
{
public:
	KGCDotaMonsterPortal(void);
	virtual ~KGCDotaMonsterPortal(void);

public:
	virtual void Create(void);
	virtual void Destroy(void);
	virtual void FrameMove(float fElapsedTime = GC_ELAPSED_TIME);
	virtual void Render(void);
	virtual GCCollisionRect<float> GetCollisionRect(void);

private:
	int m_iMapID;
	SDotaMonsterPortalInfo m_sMonsterPortalInfo;
};

class KGCDotaDPointShop : public KGCMonsterBuff
{
public:
	KGCDotaDPointShop(void);
	virtual ~KGCDotaDPointShop(void);

public:
	virtual void Create(void);
	virtual void Destroy(void);
	virtual void FrameMove(float fElapsedTime = GC_ELAPSED_TIME);
	virtual void Render(void);
	virtual GCCollisionRect<float> GetCollisionRect(void);
	void EnableParticle(bool bShow);

private:
	int m_iMapID;
	bool m_bCollision;
	SDotaTemplate   *m_pCurDotaInfo;
	SDota_D_PointShopInfo m_sDPointShopInfo;
	std::vector< CParticleEventSeqPTR > m_vecParticleShopOpen;
};
