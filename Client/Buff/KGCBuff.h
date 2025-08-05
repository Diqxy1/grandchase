#pragma once

#include "animation/KTDGAnim.h"
#include "KTDGParticleSystem.h"
#include "../GCGraphicsHelper.h"
#include "Headup Display.h"

class KGCBuffInstance;
struct WAIT_BUFF
{
	int iPlayerIndex_;
	KGCBuffInstance* pBuffInst_;
	float fBuffTime;
	int iIndex_;
	bool bIsCrashPlayer;
	DWORD dwGivePlayerUID;

	WAIT_BUFF() : iPlayerIndex_(0), pBuffInst_(NULL), fBuffTime(0.0f), iIndex_(0), dwGivePlayerUID(0) {}
};

struct BuffSpeedAttribute
{
	float m_fJumpYSpeed;
	float m_fJumpXSpeed;
	float m_fDoubleJumpYSpeed;
	float m_fDoubleJumpXSpeed;
	float m_fRunJumpYSpeed;
	float m_fRunJumpXSpeed;
	float m_fDashJumpYSpeed;
	float m_fDashJumpXSpeed;
	float m_fWalkSpeed;
	float m_fRunSpeed;

	BuffSpeedAttribute()
	{
		m_fJumpYSpeed = 0.0f;
		m_fJumpXSpeed = 0.0f;
		m_fDoubleJumpYSpeed = 0.0f;
		m_fDoubleJumpXSpeed = 0.0f;
		m_fRunJumpYSpeed = 0.0f;
		m_fRunJumpXSpeed = 0.0f;
		m_fDashJumpYSpeed = 0.0f;
		m_fDashJumpXSpeed = 0.0f;
		m_fWalkSpeed = 0.0f;
		m_fRunSpeed = 0.0f;
	}
};

////#include <vector>
class Headup;
class KGCBuffInstance
{
public:
	KGCBuffInstance(void);
	~KGCBuffInstance(void);

	int m_iLevel;
	int m_iBuffEnum;

	float m_fRemainTime;
	float m_fReserve1;
	float m_fPrevFullHPInstance;
	bool m_bPostCoolTime;  // 쿨타임을 겪었는가? ㅎㅎㅎ
	int m_iStartCoolTime;
	std::vector< std::pair<DWORD, float> >  m_vecLoopSound;

	std::vector<CKTDGAnim::CAnimInstance*> m_vecAnim;
	std::vector<CParticleEventSeqPTR> m_vecParticle;

	WAIT_BUFF m_sStartTimeBuff;

	KSafeArray<BuffSpeedAttribute, FORM_NUM>   m_kOriFormTemplate;
	KSafeArray<BuffSpeedAttribute, BTYPE_NUM>  m_kOriBurnningTemplate;
	BuffSpeedAttribute                         m_kOriSwapTemplate;
};

struct BuffAnim
{
	BuffAnim()
	{
		m_strAnim = "";
		m_fAnimScaleX = 0.0f;
		m_fAnimScaleY = 0.0f;
		m_fAnimPosX = 0.0f;
		m_fAnimPosY = 0.0f;
	}

	//마법적용중 나타날 애니메이션
	std::string m_strAnim;

	float m_fAnimScaleX;
	float m_fAnimScaleY;
	float m_fAnimPosX;
	float m_fAnimPosY;
};

struct BuffExtraMesh
{
	BuffExtraMesh()
	{
		m_extraMesh = "";
		m_iMeshTech = -1;
	}

	std::string m_extraMesh;
	int m_iMeshTech;
};

struct BuffParitcle
{
	BuffParitcle()
	{
		m_strParticle = "";
		m_fDirPosX = 0.0f;
		m_iPosType = EPT_BODY_CENTER;
		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fTraceTime = 0.0f;
		m_bIsNoDirection = false;
		m_bLocalEffect = false;
		m_iType = -1;       // -1이면 다 같이.
	}

	//마법적용중 나타날 파티클
	std::string m_strParticle;

	float m_fDirPosX;
	// 2007/2/20. iridology. 파티클 붙일 위치
	int m_iPosType;

	float m_fPosX;
	float m_fPosY;
	float m_fTraceTime;
	bool m_bIsNoDirection;
	bool m_bLocalEffect;
	int m_iType;        // 타입! 이 타입에만 이 파티클을 쓴다!
};

struct BuffBaseAttirbute
{
	float m_fIncHP;
	float m_fIncATK;
	float m_fIncDEF;
	float m_fIncHPRatio;
	float m_fIncMPRatio;
	bool  m_bUnFatal;
	float m_fHPRecoveryRate;
	float m_fCriticalRate;
	float m_fCriticalRateMultiply;
	float m_fMPRecoveryRate;    //  MP 회복률
	float m_fMPDecreaseRatio;
	float m_fMinusAPCoolTime;
	float m_fMPDecreaseRatioForSpecial; //필살기 마나사용량만 줄여준다.
	float m_fCriticalDamageUp;

	BuffBaseAttirbute()
	{
		m_fIncHP = 0.0f;
		m_fIncATK = 0.0f;
		m_fIncDEF = 0.0f;
		m_fIncHPRatio = 0.0f;
		m_fIncMPRatio = 0.0f;
		m_bUnFatal = false;
		m_fHPRecoveryRate = 0.0f;
		m_fCriticalRate = 0.0f;
		m_fCriticalRateMultiply = 1.0f;
		m_fMPRecoveryRate = 0.0f;
		m_fMPDecreaseRatio = 1.0f;
		m_fMinusAPCoolTime = 0.0f;
		m_fMPDecreaseRatioForSpecial = 1.0f;
		m_fCriticalDamageUp = 0.0f;
	}
};

struct BuffMonsterAttribute
{
	float m_fMonsterAPRate;
	float m_fMonsterDPRate;
	float m_bMonsterAggro;
	float m_bMonsterSDPRate;

	BuffMonsterAttribute()
	{
		m_fMonsterAPRate = 1.0f;
		m_fMonsterDPRate = 1.0f;
		m_bMonsterAggro = false;
		m_bMonsterSDPRate = 1.0f;
	}
};

struct BuffUIEvent
{
	DWORD m_dwUIEventCode;
	bool bInitCall;
	bool bEndCall;

	BuffUIEvent()
	{
		m_dwUIEventCode = 0;
		bInitCall = false;
		bEndCall = false;
	}
};

struct BuffSummonMonsterInfo
{
	EGCMonster      m_eMonID;
	D3DXVECTOR2     m_vOffset;
	float           m_fLimitDistance;
	int             m_iLimitCount;
	float           m_fMotherHpRatio;
	int             m_iBuffIcon;
	bool            m_bNoPlayerPos;
	float           m_fMaxHP;
	float           m_fStaticMaxHP;
	bool            m_bUnlimitDef;
	int             m_iMonsterSlot;
	int             m_iRatioOwnerAbility;

	BuffSummonMonsterInfo()
	{
		init();
	}

	void init()
	{
		m_eMonID = MON_INVALID;
		m_vOffset = D3DXVECTOR2(0.f, 0.f);
		m_fLimitDistance = 0.f;
		m_iLimitCount = 0;
		m_fMotherHpRatio = -1.f;
		m_iBuffIcon = 0;
		m_bNoPlayerPos = false;
		m_fMaxHP = 0.0f;
		m_fStaticMaxHP = -1.f;
		m_bUnlimitDef = false;
		m_iMonsterSlot = -1;
		m_iRatioOwnerAbility = 0;
	}
};

class KLuaManager;

class KGCBuff
{
	friend class KGCBuffManager;
public:
	KGCBuff(void);
	virtual ~KGCBuff(void);

	void Load(KLuaManager* pLuaMgr_);

	bool IsBuffChampionIcon( EGCPlayerMagicEffect BuffNum );

	virtual bool Init(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ = -1, bool bIsCrashPlayer = true, DWORD dwGivePlayerUID = 0);
	virtual bool InitAfter(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_ = -1, bool bIsCrashPlayer = true, DWORD dwGivePlayerUID = 0);
	//매 프레임마다 해줘야 할 것들 // 자식 class에서도 함수초반에 KGCBuff::FrameMove를 불러줘야 할껄!!
	virtual bool FrameMove(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);

	//릴때마다 데미지 생성 시켜주는 버프 =ㅅ=
	virtual void CreateDamage(int iHit, int iHitted, KGCBuffInstance* pBuffInst_, D3DXVECTOR2* vPos = NULL, EGCCharAttackType eCharAtkType_ = CAHR_ATTACK_TYPE_PHYSIC);
	//설정 해제
	virtual bool Destroy(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_);

	//레벨에 따라 다른 텍스쳐를 쓸수 있을것 같기도 -ㅅ-
	virtual GCDeviceTexture* GetHudTexture(KGCBuffInstance* pBuffInst_);
	virtual std::wstring GetHudString(KGCBuffInstance* pBuffInst_, int iStringIndex_);
	virtual int GetHudStringNum(KGCBuffInstance* pBuffInst_);
	void PushHudTexture(GCDeviceTexture* pTexture);
	void PushHudString(std::wstring strHudText);

	void HiddenParticle(bool bIsBuffTimeStop, bool bIsHiddenParticle);

	//====================================================================================
	// 2009.03.13 : Jemitgge
	// Comment : 쿨타임용 함수들~
	virtual int GetWaitHudStringNum() { return m_strHudString.size(); }
	void PushWaitHudTexture(GCDeviceTexture* pTexture);
	void PushWaitHudString(std::wstring strHudText);
	virtual void DestroyBuffEffect(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_);

	unsigned int GetMaxLevel();
private:
	bool InitGuildBuff(int iPlayerIndex_);
	void InitAnimation(KGCBuffInstance* pBuffInst_);
	void InitParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime);
	void InitPetParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime);
	void InitPlayerParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime);
	void InitPlayerSpeed(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);

	void InitAbility(int iPlayerIndex, KGCBuffInstance* pBuffInst_);
	void InitPlayerAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void InitPetAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void FinalizePlayerAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void FinalizePetAbility(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void FinalizePlayerSpeed(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);

	void SetPlayerSpeed(PLAYER_TEMPLATE &player_, BuffSpeedAttribute &kOriginalTemplate, int nBuffEnum, BuffSpeedAttribute &attr, int sumValue = 1, int iPlayerIndex_ = -1);
	void SetPlayerSpeed(KGCBuffInstance* pBuffInst_, int iPlayerIndex_, int nBuffEnum, BuffSpeedAttribute &attr, int sumValue);

	void FinalizeAnimation(KGCBuffInstance* pBuffInst_);
	void FinalizeParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);

	void FrameMoveAnimation(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void FrameMovePlayerParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void FrameMoveMonsterParticle(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void FrameMoveContinousEffect(int iPlayerIndex_);
	void PlaySound(DWORD dwSoundID);

	GCDeviceTexture* GetHudTexture(int nIndex);
	GCDeviceTexture* GetWaitHudTexture(int nIndex);

	bool IsSceondBaseMeshEnable(PLAYER* pPlayer_, bool bDestroy_);

protected:
	virtual void InitMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iWho = -1);
	virtual void FinalizeMonsterStatus(int iPlayerIndex_, KGCBuffInstance* pBuffInst_);
	void ResistDebuff(int iPlayerIndex_);
	EGCCartoonTexture m_eCartoonTexture;

	std::vector<BuffAnim> m_vecAnim;
	std::vector<BuffParitcle> m_vecParticle;

	std::vector<GCDeviceTexture*>   m_vecpHudTex;
	std::vector<std::string>        m_vecHudTexName;
	std::vector<std::wstring>       m_strHudString;

	std::vector<GCDeviceTexture*>   m_vecpWaitHudTex;
	std::vector<std::string>        m_vecWaitHudTexName;
	std::vector<std::wstring>       m_strWaitHudString;

	int								m_iCoolTime;		// 쿨타임 몇초?
	std::vector<std::string>        m_extraMesh;

	std::vector<CParticleEventSeqPTR> m_vecMonsterParticle;

	bool    m_bClearOnDie;
	bool    m_bClearOnNextStage;
	bool    m_bGuildMemberDepend;
	bool    m_bSuperArmor;
	bool    m_bHyperArmor;
	bool	m_bPetBuff;
	bool	m_bEnableContinousDamage;
	bool	m_bSpecialDisable;
	bool    m_bAllowToOnlyOneMotion;
	bool    m_bIsKilling;

	int		m_iHitPerFrame;
	float	m_fDamageToPlayer;
	float	m_fDamageToPlayerMP;
	float   m_fDamageToMonster;
	float   m_fAddDamageToPlayer;
	bool    m_bDamageByCharAbility;
	float   m_fAddDamageToMonster;
	float   m_fCureDamage;

	DWORD   m_dwGivePlayerUID;
	std::vector<BuffMonsterAttribute>   m_vecMonAttribute;
	std::vector<DWORD>   m_vecDeleteBuffList;
	std::map<std::string, std::pair<int, bool>>   m_vecMonsterTraceIcon;  // 파티클시퀀스이름, <아이콘타입, Existscreen>

	bool	m_bEnableContinousHeal;
	float	m_fHealToPlayer;
	int		m_iHealPerFrame;
	std::vector<int> m_vecOnlyMotionID;
	bool	m_bKeyLock;
	bool    m_bCharLock;
	bool    m_bCharMotionLock;
	bool    m_bMonsterFrameLock;
	bool    m_bNotDependenceFromModel;
	bool    m_bBuffEndHeal;
	DWORD   m_dwSoundIndex;
	int     m_iStartCoolTime;

	int		m_iPlayerIndex;
	int     m_iMonsterStopSpeed;
	float   m_fDecreaseBuffTime;

	std::vector< std::string > m_vecSecondBaseMesh;  // 버프 걸리면서 바뀔 메시들
	bool    m_bNoRenderPet;    // 버프 걸린동안은 펫을 그리지 않는다.

public:
	bool    m_bIsCrashPlayer;
	bool m_bAlwaysDescShow;
	bool m_bIsBuffTimeStop;
	bool m_bIsHiddenParticle;
	bool m_bSpeedPercent;
	std::vector<BuffBaseAttirbute>  m_vecAttribute;
	std::vector<BuffSpeedAttribute> m_vecSpeedAttribute;
	std::vector<BuffExtraMesh> m_vecExtraMesh;

	// 버프 건 상태로 쳐때리면 생성되는 데미지
	std::vector<int> m_vecDamage;
	bool m_bDamageNoOnlyPhysicAtk;
	bool m_bDamageNoSpecialAtk;
	bool m_bDamageNoPetAtk;

	// 여기에 등록된 데미지만 처 맞도록 허용하는 속성
	std::vector<int> m_vecPermissionDamage;

	bool m_bHitOnce;
	float m_fHpDrain;
	int m_iItemEff;

	//이벤트 포션
	int     m_nArenaTime;   //아레나 상태
	bool    m_bAntiPoison;  //안티포션먹은 상태
	int     m_nBerserkTime; //광전사

	int     m_iEdgeColor[3];
	int     m_nShaderType;

	float   m_fOriMPDecreaseRatio;

public:
	float   m_fReflectionDamageRatio;
	float   m_fDamageToMPAttack;
	float   m_fStaticToMPAttack;
	bool    m_bUseNotAP;

public:
	int     m_iMonsterNoTargetingCnt;
	int     m_iWhenActivatedMotion;

public:
	BuffUIEvent m_kBuffUIEvent;

public:
	std::vector< std::pair<DWORD, float> >  m_vecLoopSound;
	std::vector< int >  m_vecNotBuffMonsterType;;

public:
	bool    m_bIsDeBuff;
	bool    m_bClearDeBuff;

public:
	float   m_fMonsterRunSpeed;
	float   m_fMonsterWalkSpeed;

public:
	float m_fCatchResist;
	float m_fOriMissileDamageResist;
	float m_fMissileDamageResist;

public:
	bool m_bFatalSuperArmor;

private:
	BuffSummonMonsterInfo m_kBuffSummonMonster;

private:
	bool m_bIsSpecialAtk;
	bool m_bIsAttackMiss;

private:
	int m_iSuper;
	int m_iOriginalSuperFrame; // 원래 플레이어가 가지고 있던 super 프레임
	int m_iCountSuperFrame; // 이전이 슈퍼 상태였는지를 담고 있어주자.
	int m_iInitSuper;       // 기존 Super무시하고 무조건 셋팅된 슈퍼값으로

private:
	int m_iPlayMotion;
	int m_iChangeTeam;

public:
	bool m_bMPRecoveryFix;    //  기본 MP 회복량을 특정 수치로 변경시킴.
	float m_fMPRecoveryFix;   //  기본 MP 회복량을 특정 수치로 변경시킴.

private:
	bool m_bNoCrashCheck;

	float m_fChangeMpRatio;
	float m_fOriIncMpRatio;

public:
	bool m_bIsDramaSkip;

	DWORD m_dwMeshState;
};
