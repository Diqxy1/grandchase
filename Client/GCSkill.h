#pragma once

//#include <list>
//#include "GCEnum.h"
#include "GCSkillEnum.h"
//#include "KLuaManager.h"

class PLAYER;

struct GCSkillUIInfo
{
	float x;
	float y;	
	std::set<int> setLineType;
	GCSkillUIInfo()
	{
		x = 0.0f;
		y = 0.0f;		
	}
	~GCSkillUIInfo() {}
};

struct GCComboSkill
{
    int m_iSID;
    int m_iMotionChangeFrame;
    int m_iStartKeyCoolTime;
    int m_iEndKeyCoolTime;
    GCDeviceTexture* m_pTexture;

    GCComboSkill() : m_iSID( 0 ), m_iMotionChangeFrame( 0 ), m_iStartKeyCoolTime( 0 ), m_iEndKeyCoolTime( 0 ), m_pTexture( NULL ) {}
};

struct GCPossibleUseJump
{
    bool m_bPossible;
    int m_bPrecedingSID; // 이 놈 많아 지면 벡터로

    GCPossibleUseJump()
    {
        m_bPossible = false;
        m_bPrecedingSID = -1;
    }
    ~GCPossibleUseJump() {}
};

struct GCHotKeySkillInfo
{
	float m_fMana;
	int m_iMotion;
    int m_iMonsterModeMotion;
    int m_iAddMotion;
    int m_iMotionWhenCooltime;
    float m_fManaMotionWhenCooltime;
	float m_fCoolTime;
    float m_fMonsterModeMana;
    float m_fMonsterModeCoolTime;
    int m_iChargingSkillLV;    
    std::vector< GCComboSkill > m_vecComboSkill;
    bool m_bForceUseSkill;
    GCPossibleUseJump m_kPossibleUseJump;
    bool m_bSpecialSkill;
    float m_fAuraSizeWidth;
    float m_fAuraSizeHeight;
    std::vector< int > m_vecAuraBuff;
    std::vector< int > m_vecAuraDeBuff;
    int m_iBuffIndex;
    float m_fBuffTime;
    bool m_bNoEquipSlot;
    int m_iCheckCoolTimeBuff;
    bool m_bCheckSpecialMotion;
    bool m_bNoUseFatal;

	GCHotKeySkillInfo()
	{
		m_fMana = 0.0f;
		m_iMotion = -1;
        m_iMonsterModeMotion = -1;
        m_iAddMotion = -1;
        m_iMotionWhenCooltime = -1;
        m_fManaMotionWhenCooltime = 0.f;
		m_fCoolTime = 0.5f;
        m_fMonsterModeMana = 0.0f;
        m_fMonsterModeCoolTime = 0.0f;
        m_vecComboSkill.clear();
        m_bForceUseSkill = false;
        m_iChargingSkillLV = 0;
        m_bSpecialSkill = false;
        m_fAuraSizeWidth = 0;
        m_fAuraSizeHeight = 0;
        m_vecAuraBuff.clear();
        m_vecAuraDeBuff.clear();
        m_iBuffIndex = -1;
        m_fBuffTime = 0.f;
        m_bNoEquipSlot = false;
        m_iCheckCoolTimeBuff = -1;
        m_bCheckSpecialMotion = false;
        m_bNoUseFatal = false;
	}
	~GCHotKeySkillInfo() {}
};

struct GCGrowingSkillInfo
{
    int m_iSkillID;
    int m_iCondition;
    float m_fCoolTime;
    float m_fUniqueCoolTime;
    float m_fSpecialCoolTime;
    float m_fMP;
    float m_fHP;
    float m_fDamageRate;
    float m_fDungeonDamageRate;
    float m_fCriticalRate;
    float m_fCriticalDamage;
    int m_iBuffIndex;
    float m_fBuffTime;
    float m_fValue;

    GCGrowingSkillInfo()
    {
        Init();
    }
    ~GCGrowingSkillInfo() {}

    enum GrowingSkillCondition
    {
        GSC_NONE = 0,
        GSC_WHEN_CRITICAL = 1,
        GSC_RAGE_MP_RECOVERY = 2,
        GSC_WHEN_ATTACK = 3,
    };

    void Init()
    {
        m_iSkillID = -1;
        m_iCondition = GSC_NONE;
        m_fCoolTime = 0.f;
        m_fUniqueCoolTime = 0.f;
        m_fSpecialCoolTime = 0.f;
        m_fMP = 0.f;
        m_fHP = 0.f;
        m_fDamageRate = 0.f;
        m_fDungeonDamageRate = 0.f;
        m_fCriticalRate = 0.f;
        m_fCriticalDamage = 0.f;
        m_iBuffIndex = -1;
        m_fBuffTime = 0.f;
        m_fValue = 0.f;
    }
};

class GCSkill
{
public:
    GCSkill(void);
    virtual ~GCSkill(void);

    // functions
    bool Load( KLuaManager& luaMgr );
    void clear();

    // getter/setter
    EGCSkillTree GetID() const { return m_eID; }
    const SkillSet& GetPrevSet() const { return m_setPrevID; }
    void SetPrevList( const SkillPtrList& lPrev ) { m_lPrev = lPrev; }
    const SkillSet& GetNextSet() const { return m_setNextID; }
    void SetNextList( const SkillPtrList& lNext ) { m_lNext = lNext; }
    const SkillSet& GetLimitSet() const { return m_setLimitID; }
    //void SetLockList( const SkillPtrList& lLock ) { m_lNext = lLock; }
    bool IsEnableSkill() const { return m_bEnable; }
    int GetPromotion() const { return m_nPromotion; }
    EGCCharType GetCharType() const { return m_eCharType; }
    int GetLv() const { return m_nCharLv; }
    bool CheckItem( PLAYER* pPlayer );
    bool IsBase() const { return m_bBase; }
	bool IsCash() const { return m_bCash; }
	std::set< int > GetGoodsList() const	{ return m_setGoodsID; }
	EGCSkillGroup GetGroupID() const { return m_eGroup; }
	const std::string GetTextureFileName() const { return m_strTexName; }
    const std::string GetAddMotionTextureFileName() const { return m_strAddMotionTexName; }
	int GetLearnSP() const	{ return m_nLearnSP; }

	int	GetTitleID() const { return m_nTitleID; }
	int GetDescID() const { return m_nDescID; }
	int GetControlDescID() const { return m_nControlDescID; }

	const GCSkillUIInfo& GetUIinfo() const	{ return m_sUIinfo; }

    int GetBlockGroup() const { return m_nLockGroup; }

	bool isNew() const { return m_bNew; }
	void SetNew( bool bNew ) { m_bNew = bNew; }

#ifndef __PATH__
	bool LoadUI( KLuaManager& luaMgr );
#endif

private:
    EGCSkillTree m_eID;
    EGCSkillGroup m_eGroup;
    int m_nCharLv;
    EGCCharType m_eCharType;
    int m_nPromotion;
    bool m_bEnable;
    std::string m_strTexName;
    std::string m_strAddMotionTexName;
    int m_nTitleID;
    int m_nDescID;
	int m_nControlDescID;
	int m_nLearnSP;
    SkillPtrList m_lPrev;
    SkillPtrList m_lNext;
    SkillSet m_setPrevID;
    SkillSet m_setNextID;
    SkillSet m_setLimitID; // 같이 배울 수 없는 스킬 리스트를 담는다.
    std::set< int > m_setGoodsID;
    bool m_bBase;
	bool m_bCash;    
	bool m_bNew;

	//UI에 사용될 정보
	GCSkillUIInfo m_sUIinfo;
    int m_nLockGroup;

	
public:
	
	GCHotKeySkillInfo* m_pHotKeySkill;
	bool m_bAccSkill;
	bool m_bPassive;

    bool m_bSpecialTypeB; // 흑마법인지, 백마법인지 A의 기준은 디폴트로 설정되는 필살기 셋이라고 생각하면 된다.
    int m_iCurSkillLv;
    int m_iMaxSkillLv;

    int m_iAwakeningLv; // 각성차수
    bool m_bOrCheckPreList;

};