#ifndef _PLAYER_COMP_H_
#define _PLAYER_COMP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
//#include "GCGlobal.h"
#include "GCObject.h"
#include "./Particle/KGCParticleManager.h"
#include "./animation/KGCAnimManager.h"
#include "GCItemEnum.h"
//#include <set>
#include "KGCObj.h"
#include "KncP2PLib/KncCommonDefine.h"

class PLAYER;
class CPetAI;
class GCSquarePeople;
typedef bool (PLAYER::*FuncIsCanUse)();
typedef void (PLAYER::*FuncUse)();
struct GCHotKeySkillInfo;

struct SCombinationSkillSetInfo
{
	std::vector< std::pair< int, bool > > vecSkillList;
	std::pair< int, float >  prAutoRunSkillInfo;

	SCombinationSkillSetInfo()
	{
		vecSkillList.clear();
		prAutoRunSkillInfo = std::make_pair(-1, 0.f);
	}
};

struct SRangeCheckInfo
{
	D3DXVECTOR2         m_vRangeCheckSize; // 체크범위의 사이즈를 저장한다.
	bool                m_bRangeInSameTeam;  // 같은 편이 체크범위 안에 있는지를 체크한다.
	bool                m_bRangeInEnemy;  // 적이 체크범위 안에 있는지를 체크한다.
	std::set< int >     m_setRangeBuff;    // 범위안에 들어갔을 시 아군에게 적용받게 되는 버프
	std::set< int >     m_setRangeDeBuff;    // 범위안에 들어갔을 시 아군에게 적용받게 되는 버프
	std::map< DWORD, std::set< int > >     m_mapAppliedBuff;    // 내가 적용한 버프 리스트
	std::map< DWORD, std::set< int > >     m_mapAppliedDeBuff;    // 내가 적용한 버프 리스트
	KncStopWatch        m_kStopWatch;  // 오오라 발동되는 시간 설정

	SRangeCheckInfo()
	{
		Init();
	}

	void Init()
	{
		m_vRangeCheckSize = D3DXVECTOR2(0.f, 0.f);
		m_bRangeInSameTeam = false;
		m_bRangeInEnemy = false;
		m_setRangeBuff.clear();
		m_setRangeDeBuff.clear();
		m_mapAppliedBuff.clear();
		m_mapAppliedDeBuff.clear();
		m_kStopWatch.Init();
		m_kStopWatch.SetStopCheck(true);
	}
};

struct SMarbleSytemInfo
{
	std::vector< int > m_vecMotionList;
	std::vector< int > m_vecDamageList;
	float m_fMarbleChargeNum;
	bool m_bCheckOnlyOneUnit;

	SMarbleSytemInfo()
	{
		m_vecDamageList.clear();
		m_vecMotionList.clear();
		m_fMarbleChargeNum = 0;
		m_bCheckOnlyOneUnit = false;
	}
};

struct SBurnSytemInfo
{
	std::vector< int > m_vecDamageList;
	float m_fBurnChargeNum;
	bool m_bCheckOnlyOneUnit;

	SBurnSytemInfo()
	{
		m_vecDamageList.clear();
		m_fBurnChargeNum = 0;
		m_bCheckOnlyOneUnit = false;
	}
};

struct SBloodSytemInfo
{
	std::vector< int > m_vecDamageList;
	float m_fBloodChargeNum;
	bool m_bCheckOnlyOneUnit;

	SBloodSytemInfo()
	{
		m_vecDamageList.clear();
		m_fBloodChargeNum = 0;
		m_bCheckOnlyOneUnit = false;
	}
};

struct GCHotKeySkill
{
	EGCSkillTree m_eSkill;
	float fCoolTime;
	int iStartKeyTime;
	int iEndKeyTime;
	GCDeviceTexture* pTexture;
	GCDeviceTexture* pAddMotionTexture;

	GCHotKeySkill()
	{
		m_eSkill = SID_MAX;
		fCoolTime = 0.0f;
		iStartKeyTime = 0;
		iEndKeyTime = 0;
		pTexture = NULL;
		pAddMotionTexture = NULL;
	}
	~GCHotKeySkill() {}

	GCHotKeySkill& operator = (const GCHotKeySkill& right)
	{
		m_eSkill = right.m_eSkill;
		fCoolTime = right.fCoolTime;
		iStartKeyTime = right.iStartKeyTime;
		iEndKeyTime = right.iEndKeyTime;
		pTexture = right.pTexture;
		pAddMotionTexture = right.pAddMotionTexture;
		return *this;
	}
};

struct KHotKeyCoolTimeInfo
{
	EGCSkillTree m_eSkill;
	float fCoolTime;
	int iStartKeyTime;
	int iEndKeyTime;

	KHotKeyCoolTimeInfo()
	{
		Init();
	}

	~KHotKeyCoolTimeInfo() {}

	KHotKeyCoolTimeInfo& operator = (const KHotKeyCoolTimeInfo& right)
	{
		m_eSkill = right.m_eSkill;
		fCoolTime = right.fCoolTime;
		iStartKeyTime = right.iStartKeyTime;
		iEndKeyTime = right.iEndKeyTime;
		return *this;
	}

	void Init()
	{
		m_eSkill = SID_MAX;
		fCoolTime = 0.0f;
		iStartKeyTime = 0;
		iEndKeyTime = 0;
	}
};

struct MotionHistory
{
	int iMotionNum;
	int iMotionFrame;
	float bodyAngle;
	D3DXMATRIX matrix;
};

struct SpecialAbilityItem
{
	std::string strItemName;
	FuncIsCanUse pfIsCanUse;
	FuncUse pfUse;
	SpecialAbilityItem() {}
	SpecialAbilityItem(std::string strItemName_, FuncIsCanUse pfIsCanUse_, FuncUse pfUse_)
	{
		strItemName = strItemName_;
		pfIsCanUse = pfIsCanUse_;
		pfUse = pfUse_;
	}
};

struct PositionDesc
{
	CHAR_IDENTY charID;

	D3DXVECTOR3 rightHandTip;
	D3DXVECTOR3 rightHandBody;

	D3DXVECTOR3 leftHandTip;
	D3DXVECTOR3 leftHandBody;

	D3DXVECTOR3 NephillimRightHand;
	D3DXVECTOR3 NephillimLeftHand;

	bool right;
	D3DXVECTOR3 body;

	D3DXVECTOR3 swordTip;
	D3DXVECTOR3 swordBody;

	D3DXVECTOR3 leftSwordTip;
	D3DXVECTOR3 leftSwordBody;

	D3DXVECTOR3 swordCenter;
	D3DXVECTOR3 leftSwordCenter;

	D3DXVECTOR3 HeadPos;

	float swordAngle;
	float leftSwordAngle;

	D3DXVECTOR3 bow;
	float bowAngle;

	D3DXVECTOR3 crossBowLeft;
	D3DXVECTOR3 crossBowRight;
	float crossBowLeftAngle;
	float crossBowRightAngle;

	//라스 단검 위치 좌표 추가(강화목걸이 위해)
	D3DXVECTOR3 LasKnifeOne;
	D3DXVECTOR3 LasKnifeTwo;
	float LasKnifeOneAngle;
	float LasKnifeTwoAngle;

	//라이언 도끼 위치 좌표
	D3DXVECTOR3 RyanAxe;
	float RyanAxeAngle;

	// 센티넬 크리센트 위치
	D3DXVECTOR3 SentinelCricent;
	float SentinelCricentAngle;

	D3DXVECTOR3 staffTip;
	D3DXVECTOR3 staffBody;
	float staffAngle;

	KSafeArray<D3DXVECTOR3, (5 + (LATENCY + SHOWLATENCY)) * 2> rightHandLatancy;
	KSafeArray<D3DXVECTOR3, (5 + (LATENCY + SHOWLATENCY)) * 2> leftHandLatancy;
	KSafeArray<D3DXVECTOR3, (5 + (LATENCY + SHOWLATENCY)) * 2> headLatancy;

	D3DXVECTOR3 HeadBombSpark;

	D3DXVECTOR3 m_BoneLegLeftTip;
	D3DXVECTOR3 m_BoneLegRightTip;
	D3DXVECTOR3 m_BoneLegLeftBody;
	D3DXVECTOR3 m_BoneLegRightBody;

	D3DXVECTOR3 AmyHairLeft;
	D3DXVECTOR3 AmyHairRight;

	// 충돌 체크를 위한 어깨위치
	D3DXVECTOR3	rightShoulder;
	D3DXVECTOR3	leftShoulder;
	// 레이턴시를 위한 어깨위치
	KSafeArray<D3DXVECTOR3, (5 + (LATENCY + SHOWLATENCY)) * 2> rightShoulderLatancy;
	KSafeArray<D3DXVECTOR3, (5 + (LATENCY + SHOWLATENCY)) * 2> leftShoulderLatancy;
	// 레이턴시가 적용되고 이펙트 나오기 위한 어깨위치
	D3DXVECTOR3	effectrightShoulder;
	D3DXVECTOR3	effectleftShoulder;

	D3DXVECTOR3 m_vRightFoot;
	D3DXVECTOR3 m_vLeftFoot;

	float bodyAngle;

	D3DXVECTOR3 m_vPlayerPos;
};

class PLAYER;

// 캐릭터마다 다르게 나와야 할 Sound 번호

#define __EQUAL_OPERATOR__(p) this->p = rval.p

struct PLAYER_TEMPLATE_SOUND
{
	KSafeArray<int, 4> AH;
	int AHH;
	int AHHH;
	KSafeArray<int, 2> STAND;
	KSafeArray<int, 5> ATK;

	PLAYER_TEMPLATE_SOUND()
	{
		AH[0] = -1;
		AH[1] = -1;
		AH[2] = -1;
		AH[3] = -1;
		AHH = -1;
		AHHH = -1;
		STAND[0] = -1;
		STAND[1] = -1;
		ATK[0] = -1;
		ATK[1] = -1;
		ATK[2] = -1;
		ATK[3] = -1;
		ATK[4] = -1;
	}

	PLAYER_TEMPLATE_SOUND& operator =(const PLAYER_TEMPLATE_SOUND& rval)
	{
		__EQUAL_OPERATOR__(AH[0]);
		__EQUAL_OPERATOR__(AH[1]);
		__EQUAL_OPERATOR__(AH[2]);
		__EQUAL_OPERATOR__(AH[3]);
		__EQUAL_OPERATOR__(AHH);
		__EQUAL_OPERATOR__(AHHH);
		__EQUAL_OPERATOR__(STAND[0]);
		__EQUAL_OPERATOR__(STAND[1]);
		__EQUAL_OPERATOR__(ATK[0]);
		__EQUAL_OPERATOR__(ATK[1]);
		__EQUAL_OPERATOR__(ATK[2]);
		__EQUAL_OPERATOR__(ATK[3]);
		__EQUAL_OPERATOR__(ATK[4]);
		return *this;
	}
};

struct METAMORPHOSIS_TEMPLATE
{
public:
	struct S_Effect
	{
		int iEffectKind;
		std::string strEffectName;
		int iParticleNum;
		float fOffset;
	};
	struct S_StartBuffEffect
	{
		int iEffectID;
		int iGoodsID;   //-1 이면 특정 아이템이 없어도 동작하는 이펙트, GoodsID가 등록되어 있으면 해당 아이템이 보여주는 이펙트로 전환
		float fBuffTime;
	};
	struct S_MetamorphosisModel
	{
		std::string strModelFileName;
		int iGoodsID;   //-1 이면 특정 아이템이 없어도 동작하는 이펙트, GoodsID가 등록되어 있으면 해당 아이템이 보여주는 이펙트로 전환
	};
	struct S_MetamorphosisShoadow
	{
		std::string strModelFileName;
		std::string strTextureFileName;
	};
	struct S_MetamorphosisTexture
	{
		std::string strTextureFileName;
		int iGoodsID;   //-1 이면 특정 아이템이 없어도 동작하는 이펙트, GoodsID가 등록되어 있으면 해당 아이템이 보여주는 이펙트로 전환
	};
	struct S_EndBuffEffect
	{
		int iEffectID;
		int iGoodsID;   //-1 이면 특정 아이템이 없어도 동작하는 이펙트, GoodsID가 등록되어 있으면 해당 아이템이 보여주는 이펙트로 전환
		float fBuffTime;
	};
public:
	int iFormID;
	int FormChangeCompleteMotion;

	//변신을 시작할 때의 능력치 변화량
	float StartIncApRatio;
	float StartIncDpRatio;
	float StartHPRatio;

	// 변신할 때의 패널티
	float fDamageRatio; // 체력의 몇%를 데미지로 준다.
	int iUseItem;       // 변신할 때 정해진 아이템의 효과를 적용시킴.

	//변신이 풀릴 때의 능력치 변화량
	float EndIncApRatio;
	float EndIncDpRatio;
	float EndHPRatio;

	std::vector<S_Effect> vecStartEffect;
	std::vector<S_StartBuffEffect> vecStartBuffEffect;
	std::vector<S_StartBuffEffect> vecFormBuffEffect;
	std::vector<S_MetamorphosisModel> vecMetamorphosisModel;
	std::vector<S_MetamorphosisTexture> vecMetamorphosisTexture;
	std::vector<S_Effect> vecEndEffect;
	std::vector<S_EndBuffEffect> vecEndBuffEffect;
	std::set<int> setEndClearBuffList;
	S_MetamorphosisShoadow sMetamorphosisShoadow;
	std::vector<EXTRA_MESH> vecMetamorphosisExtraMesh;

	void init()
	{
		iFormID = FORM_NORMAL;  //이 값이 그대로 이면 변신이라는게 없는 케릭터이다.
		FormChangeCompleteMotion = -1;

		//변신을 시작할 때의 능력치 변화량( 능력치가 0이 될까봐 1로 초기화 )
		StartIncApRatio = 1;
		StartIncDpRatio = 1;
		StartHPRatio = 1;

		// 변신할 때의 패널티
		fDamageRatio = 0.0f;    // 체력의 몇%를 데미지로 준다.
		iUseItem = -1;      // 변신할 때 정해진 아이템의 효과를 적용시킴.

		//변신이 풀릴 때의 능력치 변화량
		EndIncApRatio = 1;
		EndIncDpRatio = 1;
		EndHPRatio = 1;

		vecStartEffect.clear();
		vecStartBuffEffect.clear();
		vecFormBuffEffect.clear();
		vecMetamorphosisModel.clear();
		vecMetamorphosisTexture.clear();
		vecEndEffect.clear();
		vecEndBuffEffect.clear();
		vecMetamorphosisExtraMesh.clear();
	}
};

struct TAG_TEMPLATE
{
	UINT    m_uiStartMotion;
	UINT    m_uiEndMotion;
	UINT    m_uiAtkMotion;

	TAG_TEMPLATE()
	{
		m_uiStartMotion = 0;
		m_uiEndMotion = 0;
		m_uiAtkMotion = 0;
	}
};

//텍스쳐의 UV좌표와 화면에 그려질 XY 좌표;
struct SPECIAL_FACE_UVXY
{
	D3DXVECTOR2 vTextureLeftTop;
	D3DXVECTOR2 vTextureRightButtom;
	D3DXVECTOR2 vRScreenLeftTop;
	D3DXVECTOR2 vRScreenRightButtom;
	D3DXVECTOR2 vLScreenLeftTop;
	D3DXVECTOR2 vLScreenRightButtom;

	SPECIAL_FACE_UVXY()
	{
		vTextureLeftTop.x = -1.0;
		vTextureLeftTop.y = -1.0;
		vTextureRightButtom.x = -1.0;
		vTextureRightButtom.y = -1.0;
		vRScreenLeftTop.x = -1.0;
		vRScreenLeftTop.y = -1.0;
		vRScreenRightButtom.x = -1.0;
		vRScreenRightButtom.y = -1.0;
		vLScreenLeftTop.x = -1.0;
		vLScreenLeftTop.y = -1.0;
		vLScreenRightButtom.x = -1.0;
		vLScreenRightButtom.y = -1.0;
	}

	SPECIAL_FACE_UVXY& operator = (const SPECIAL_FACE_UVXY& right)
	{
		vTextureLeftTop = right.vTextureLeftTop;
		vTextureRightButtom = right.vTextureRightButtom;
		vRScreenLeftTop = right.vRScreenLeftTop;
		vRScreenRightButtom = right.vRScreenRightButtom;
		vLScreenLeftTop = right.vLScreenLeftTop;
		vLScreenRightButtom = right.vLScreenRightButtom;

		return *this;
	}
};

struct SPECIAL_FACE
{
	std::string strFaceTextureName; //텍스쳐 명
	KSafeArray<D3DXVECTOR2, 3> avFacePosition;  //이동 좌표 (0: 시작, 1:중간, 2:끝 )
	std::vector< SPECIAL_FACE_UVXY > vecFaceUVXY;   //텍스쳐의 UV좌표와 화면에 그려질 XY 좌표;

	void init()
	{
		strFaceTextureName = "NULL";
		for (int i = 0; i < 3; ++i)
		{
			avFacePosition[i].x = -1.0f;
			avFacePosition[i].y = -1.0f;
		}
		vecFaceUVXY.clear();
	}

	SPECIAL_FACE()
	{
		strFaceTextureName = "NULL";
		for (int i = 0; i < 3; ++i)
		{
			avFacePosition[i].x = -1.0f;
			avFacePosition[i].y = -1.0f;
		}
	}

	~SPECIAL_FACE()
	{
		strFaceTextureName.clear();
		vecFaceUVXY.clear();
	}

	SPECIAL_FACE& operator = (const SPECIAL_FACE& right)
	{
		strFaceTextureName = right.strFaceTextureName;
		for (int i = 0; i < 3; i++) {
			avFacePosition[i] = right.avFacePosition[i];
		}
		vecFaceUVXY = right.vecFaceUVXY;

		return *this;
	}
};

struct PLAYER_SLASH
{
	std::string strSlashName;
	int iIndex;
	KSafeArray<int, 8> aiElemental; // "gcitemenum.h EItemElemental 참조. 강화무기에 따른 궤적을 위해서. - 단 발찌는 제외

	PLAYER_SLASH()
		: strSlashName("")
		, iIndex(0)
	{
		for (int i = 0; i < (int)aiElemental.size(); ++i)
		{
			aiElemental[i] = 0;
		}
	}

	~PLAYER_SLASH()
	{
	}

	void init()
	{
		strSlashName = "";
		iIndex = 0;

		for (int i = 0; i < (int)aiElemental.size(); ++i)
		{
			aiElemental[i] = 0;
		}
	}
};

struct HIT_EFFECT
{
	int first;
	int second;
	std::string strParticleName;
};

struct CharBaseParticle
{
	// 파티클 기본정보
	int iPositionType;
	float fOffsetX;
	float fOffsetY;
	float fTraceTime;
	std::string strSequenceName;
	CParticleEventSeqPTR pParticle;

	// 파티클의 방향정보
	bool bIsRight;

	// 출력조건
	int iMinLevel;
	int iMaxLevel;

	CharBaseParticle()
		: fOffsetX(0.0f), fOffsetY(0.0f), fTraceTime(0.5f)
		, strSequenceName(""), iPositionType(0), pParticle(NULL)
		, bIsRight(false), iMinLevel(0), iMaxLevel(0) {}
};

struct CharBaseEffect
{
	int iCondition;
	std::vector< CharBaseParticle > vecParticleInfo;
	CharBaseEffect() : iCondition(-1) {}
	~CharBaseEffect() { vecParticleInfo.clear(); }
};

struct SSkillCommand
{
	int m_iSkillID;
	std::vector<UINT> m_vecMotionID;

	SSkillCommand()
	{
		m_iSkillID = -1;
		m_vecMotionID.clear();
	}
};

struct SSkillCommandList
{
	int m_iNameStringID;
	DWORD m_dwColor;
	std::vector<SSkillCommand> m_vecCommandList;

	SSkillCommandList()
	{
		m_iNameStringID = 0;
		m_dwColor = D3DCOLOR_ARGB(0, 0, 0, 0);
		m_vecCommandList.clear();
	}
};

struct PLAYER_TEMPLATE//공통적으로 사용하는 부분을 여기에 둡니다.
{
	void Init()
	{
		ATK1 = 0;
		ATKEND = 0;
		WAIT = 0;
		FATAL_WAIT = 0;
		WALK = 0;
		SPECIAL1_W = 0;
		SPECIAL2_W = 0;
		SPECIAL3_W = 0;
		SPECIAL1_B = 0;
		SPECIAL2_B = 0;
		SPECIAL3_B = 0;
		ORIGINAL1_W = 0;
		ORIGINAL2_W = 0;
		ORIGINAL3_W = 0;
		ORIGINAL1_B = 0;
		ORIGINAL2_B = 0;
		ORIGINAL3_B = 0;
		SPECIAL_ADD_MOTION.clear();
		DASH = 0;
		DASH_ATK = 0;
		WIN = 0;
		START = 0;
		SELECT = 0;
		RESULT_WIN = 0;
		RESULT_LOSE = 0;
		JOKE = 0;
		RUN = 0;
		JUMP_DASH_ATK = 0;

		ATK1_FASTINPUT = 0;
		iFastInputNeedCount = 0;

		JUMP = 0;
		JUMP_ATK = 0;
		JUMP_DASH = 0;
		JUMP_READY = 0;
		DOUBLE_JUMP = 0;
		DOUBLE_JUMP_ATK = 0;
		DOUBLE_JUMP_LAND = 0;
		iJumpAtkNoCheckContact = 0;

		CRITICAL_ATK = 0;
		DOUBLE_ATK = 0;

		ENABLE_ARROWDEF = false;
		ARROWDEF_GROUND1 = 0;
		ARROWDEF_GROUND2 = 0;
		ARROWDEF_SKY = 0;
		ARROW_REFLECTION = 0;
		iRefelectionRate = 0;

		GROUND_DOWN_DASH = 0;

		BURNNING_CHARGE = -1;
		BURNNING_ACTIVE = -1;

		DOWN_STAND_UP_EVASION = -1;
		mapEnemyAtkGuardMotion.clear();

		SID_EXPANSION_MANA = -1;
		SID_SPECIAL4 = -1;

		SAVING_STANCE = -1;
		SAVING_ATTACK = -1;
		SAVING_EVASION = -1;

		fJumpYSpeed = 0.0f;
		fJumpXSpeed = 0.0f;

		fDoubleJumpYSpeed = 0.0f;
		fDoubleJumpXSpeed = 0.0f;

		fRunJumpYSpeed = 0.0f;
		fRunJumpXSpeed = 0.0f;

		fDashJumpYSpeed = 0.0f;
		fDashJumpXSpeed = 0.0f;

		iNextAttackCount = 0;

		fWalkSpeed = 0.0f;
		fRunSpeed = 0.0f;

		fIncHP = 0.0f;

		CATCH = 0;
		WAKE = 0;
		dwStopOnDamage = (DWORD)0;
		fCharHeight = 0.0f;
		fPlayingScale = 0.f;

		fDamagefactor = 1.0f;

		fBPChargeTime = 0.0f;
		fBPStopDelayTime = 0.0f;
		fBPDecreaseTime = 1.0f;
		fBPHitIncrease = 0.0f;
		bUseBurnCharge = false;

		fHGPCumulativeNum = 0.0f;
		fHGPDecreaseTime = 0.0f;

		fBPPerAttackPlayer = 0.0f;
		fBPPerAttackMonster = 0.0f;
		fBPPerAttackedMe = 0.0f;

		int iWeaponBoneIndex = -1;
		int iWeaponBoneIndexLeft = -1;

		vecHitEffect.clear();
		mapCommonToOwnMotion.clear();
		sMetamorphosisTemplate.init();

		sPlayerSpecialFace.init();

		setCharSound.clear();
		setMeshAnimation.clear();
		vecExtraMesh.clear();
		mapFaceLiftMesh.clear();
		vecAddMesh.clear();
		vecItemMeshOption.clear();
		setStandardCharMotion.clear();
		setExtendCharMotion.clear();

		WeaponRenderBasically = true;
		LeftWeaponRenderBasically = false;
		BurningCostForSpecial = 1.0f;

		iSubAbilityType = SUB_ABILITY_NONE;
		iQPChargeFrame = 0;
		iMaxQP = 0;
		iRageModeHpPercent = -1;

		mapSpecialBuff.clear();
		mapPuzzleKeyPattern.clear();
		setPuzzleKeyList.clear();

		fMaxMarbleAmount = 0.f;
		vecMarbleSytemInfo.clear();
		mapCombinationSkillList.clear();
		setDamageList.clear();
		vecQuickSlotSkill.clear();

		fManaRecoveryRatioByAttackMon = -1.f;
		fManaAutoRecoveryRatio = -1.f;
		iOtherFaceHowToCheck = -1;
		iOtherFaceCheckValue = -1;
		strOtherFaceTextureName = "";

		setStatsSkillMotionList.clear();

		m_mapCommandList.clear();
		m_mapTutorialCommandList.clear();
	}

	PLAYER_TEMPLATE& operator =(const PLAYER_TEMPLATE& rval)
	{
		__EQUAL_OPERATOR__(ATK1);
		__EQUAL_OPERATOR__(ATKEND);
		__EQUAL_OPERATOR__(WAIT);
		__EQUAL_OPERATOR__(FATAL_WAIT);
		__EQUAL_OPERATOR__(WALK);
		__EQUAL_OPERATOR__(SPECIAL1_W);
		__EQUAL_OPERATOR__(SPECIAL2_W);
		__EQUAL_OPERATOR__(SPECIAL3_W);
		__EQUAL_OPERATOR__(SPECIAL1_B);
		__EQUAL_OPERATOR__(SPECIAL2_B);
		__EQUAL_OPERATOR__(SPECIAL3_B);
		__EQUAL_OPERATOR__(ORIGINAL1_W);
		__EQUAL_OPERATOR__(ORIGINAL2_W);
		__EQUAL_OPERATOR__(ORIGINAL3_W);
		__EQUAL_OPERATOR__(ORIGINAL1_B);
		__EQUAL_OPERATOR__(ORIGINAL2_B);
		__EQUAL_OPERATOR__(ORIGINAL3_B);
		__EQUAL_OPERATOR__(SPECIAL_ADD_MOTION);
		__EQUAL_OPERATOR__(DASH);
		__EQUAL_OPERATOR__(DASH_ATK);
		__EQUAL_OPERATOR__(WIN);
		__EQUAL_OPERATOR__(START);
		__EQUAL_OPERATOR__(SELECT);
		__EQUAL_OPERATOR__(RESULT_WIN);
		__EQUAL_OPERATOR__(RESULT_LOSE);
		__EQUAL_OPERATOR__(JOKE);
		__EQUAL_OPERATOR__(RUN);
		__EQUAL_OPERATOR__(JUMP_DASH_ATK);
		__EQUAL_OPERATOR__(ATK1_FASTINPUT);
		__EQUAL_OPERATOR__(iFastInputNeedCount);
		__EQUAL_OPERATOR__(JUMP);
		__EQUAL_OPERATOR__(JUMP_ATK);
		__EQUAL_OPERATOR__(JUMP_DASH);
		__EQUAL_OPERATOR__(JUMP_READY);
		__EQUAL_OPERATOR__(DOUBLE_JUMP);
		__EQUAL_OPERATOR__(DOUBLE_JUMP_ATK);
		__EQUAL_OPERATOR__(DOUBLE_JUMP_LAND);
		__EQUAL_OPERATOR__(iJumpAtkNoCheckContact);
		__EQUAL_OPERATOR__(CRITICAL_ATK);
		__EQUAL_OPERATOR__(DOUBLE_ATK);
		__EQUAL_OPERATOR__(ENABLE_ARROWDEF);
		__EQUAL_OPERATOR__(ARROWDEF_GROUND1);
		__EQUAL_OPERATOR__(ARROWDEF_GROUND2);
		__EQUAL_OPERATOR__(ARROWDEF_SKY);
		__EQUAL_OPERATOR__(ARROW_REFLECTION);
		__EQUAL_OPERATOR__(iRefelectionRate);
		__EQUAL_OPERATOR__(GROUND_DOWN_DASH);
		__EQUAL_OPERATOR__(fJumpYSpeed);
		__EQUAL_OPERATOR__(fJumpXSpeed);
		__EQUAL_OPERATOR__(fDoubleJumpYSpeed);
		__EQUAL_OPERATOR__(fDoubleJumpXSpeed);
		__EQUAL_OPERATOR__(fRunJumpYSpeed);
		__EQUAL_OPERATOR__(fRunJumpXSpeed);
		__EQUAL_OPERATOR__(fDashJumpYSpeed);
		__EQUAL_OPERATOR__(fDashJumpXSpeed);
		__EQUAL_OPERATOR__(iNextAttackCount);
		__EQUAL_OPERATOR__(fWalkSpeed);
		__EQUAL_OPERATOR__(fRunSpeed);
		__EQUAL_OPERATOR__(fIncHP);
		__EQUAL_OPERATOR__(CATCH);
		__EQUAL_OPERATOR__(WAKE);
		__EQUAL_OPERATOR__(dwStopOnDamage);
		__EQUAL_OPERATOR__(fCharHeight);
		__EQUAL_OPERATOR__(fPlayingScale);
		__EQUAL_OPERATOR__(vWeaponBase);
		__EQUAL_OPERATOR__(vWeaponEnd);
		__EQUAL_OPERATOR__(vLeftWeaponBase);
		__EQUAL_OPERATOR__(vLeftWeaponEnd);
		__EQUAL_OPERATOR__(sSound);
		__EQUAL_OPERATOR__(vecHitEffect);
		__EQUAL_OPERATOR__(mapCommonToOwnMotion);
		__EQUAL_OPERATOR__(fDamagefactor);
		__EQUAL_OPERATOR__(fBPChargeTime);
		__EQUAL_OPERATOR__(fBPHitIncrease);
		__EQUAL_OPERATOR__(bUseBurnCharge);
		__EQUAL_OPERATOR__(fBPPerAttackPlayer);
		__EQUAL_OPERATOR__(fBPPerAttackMonster);
		__EQUAL_OPERATOR__(fBPPerAttackedMe);
		__EQUAL_OPERATOR__(iWeaponBoneIndex);
		__EQUAL_OPERATOR__(iWeaponBoneIndexLeft);
		__EQUAL_OPERATOR__(fBPStopDelayTime);
		__EQUAL_OPERATOR__(fBPDecreaseTime);
		__EQUAL_OPERATOR__(BURNNING_CHARGE);
		__EQUAL_OPERATOR__(BURNNING_ACTIVE);
		__EQUAL_OPERATOR__(DOWN_STAND_UP_EVASION);
		__EQUAL_OPERATOR__(mapEnemyAtkGuardMotion);
		__EQUAL_OPERATOR__(SID_EXPANSION_MANA);
		__EQUAL_OPERATOR__(SID_SPECIAL4);
		__EQUAL_OPERATOR__(sMetamorphosisTemplate);
		__EQUAL_OPERATOR__(sTagTemplate);
		__EQUAL_OPERATOR__(sPlayerSpecialFace);
		__EQUAL_OPERATOR__(setCharSound);
		__EQUAL_OPERATOR__(setMeshAnimation);
		__EQUAL_OPERATOR__(sPlayerSlash);
		__EQUAL_OPERATOR__(vecExtraMesh);
		__EQUAL_OPERATOR__(mapFaceLiftMesh);
		__EQUAL_OPERATOR__(vecAddMesh);
		__EQUAL_OPERATOR__(vecItemMeshOption);
		__EQUAL_OPERATOR__(setStandardCharMotion);
		__EQUAL_OPERATOR__(setExtendCharMotion);
		__EQUAL_OPERATOR__(fHGPCumulativeNum);
		__EQUAL_OPERATOR__(fHGPDecreaseTime);

		__EQUAL_OPERATOR__(WeaponRenderBasically);
		__EQUAL_OPERATOR__(LeftWeaponRenderBasically);
		__EQUAL_OPERATOR__(BurningCostForSpecial);
		__EQUAL_OPERATOR__(BaseEffect);
		__EQUAL_OPERATOR__(vecSummonMonsterID);
		__EQUAL_OPERATOR__(iSubAbilityType);
		__EQUAL_OPERATOR__(iQPChargeFrame);
		__EQUAL_OPERATOR__(iMaxQP);
		__EQUAL_OPERATOR__(iRageModeHpPercent);
		__EQUAL_OPERATOR__(mapSpecialBuff);
		__EQUAL_OPERATOR__(mapPuzzleKeyPattern);
		__EQUAL_OPERATOR__(setPuzzleKeyList);
		__EQUAL_OPERATOR__(fMaxMarbleAmount);
		__EQUAL_OPERATOR__(vecBurnSytemInfo);
		__EQUAL_OPERATOR__(vecMarbleSytemInfo);
		__EQUAL_OPERATOR__(mapControlCountCoolTimeSkillList);
		__EQUAL_OPERATOR__(mapCombinationSkillList);
		__EQUAL_OPERATOR__(setDamageList);
		__EQUAL_OPERATOR__(fManaRecoveryRatioByAttackMon);
		__EQUAL_OPERATOR__(fManaAutoRecoveryRatio);
		__EQUAL_OPERATOR__(setStatsSkillMotionList);
		__EQUAL_OPERATOR__(m_mapCommandList);
		__EQUAL_OPERATOR__(m_mapTutorialCommandList);
		__EQUAL_OPERATOR__(SAVING_STANCE);
		__EQUAL_OPERATOR__(SAVING_ATTACK);
		__EQUAL_OPERATOR__(SAVING_EVASION);
		__EQUAL_OPERATOR__(SAVING_SKILL_ID);

#undef __EQUAL_OPERATOR__
		return *this;
	}

	enum {
		CMID_ATK1,
		CMID_ATKEND,
		CMID_WAIT,
		CMID_FATALWAIT,
		CMID_WALK,
		CMID_SPECIAL1_W,
		CMID_SPECIAL2_W,
		CMID_SPECIAL3_W,
		CMID_SPECIAL1_B,
		CMID_SPECIAL2_B,
		CMID_SPECIAL3_B,
		CMID_ORIGINAL1_W,
		CMID_ORIGINAL2_W,
		CMID_ORIGINAL3_W,
		CMID_ORIGINAL1_B,
		CMID_ORIGINAL2_B,
		CMID_ORIGINAL3_B,
		CMID_DASH,
		CMID_DASH_ATK,
		CMID_WIN,
		CMID_START,
		CMID_SELECT,
		CMID_RESULT_WIN,
		CMID_RESULT_LOSE,
		CMID_JOKE,
		CMID_RUN,
		CMID_JUMP_DASH_ATK,
		CMID_ATK1_FASTINPUT,
		CMID_JUMP,
		CMID_JUMP_ATK,
		CMID_JUMP_DASH,
		CMID_JUMP_READY,
		CMID_DOUBLE_JUMP,
		CMID_DOUBLE_JUMP_ATK,
		CMID_DOUBLE_JUMP_LAND,
		CMID_CRITICAL_ATK,
		CMID_DOUBLE_ATK,
		CMID_ARROWDEF_GROUND1,
		CMID_ARROWDEF_GROUND2,
		CMID_ARROWDEF_SKY,
		CMID_BURNNING_CHARGE,
		CMID_BURNNING_ACTIVE,
		CMID_DOWN_STAND_UP_EVASION,
		CMID_ENEMY_ATK_GUARD_MOTION,
		CMID_SID_EXPANSION_MANA,
		CMID_SID_SPECIAL4,
		CMID_SAVING_STANCE,
		CMID_SAVING_ATTACK,
		CMID_SAVING_EVASION,
		CMID_SAVING_SKILL_ID,
	};

	int GetMotionIDByEnum(int iEnum) {
		switch (iEnum) {
		case CMID_ATK1: return ATK1;
		case CMID_ATKEND: return ATKEND;
		case CMID_WAIT: return WAIT;
		case CMID_FATALWAIT: return FATAL_WAIT;
		case CMID_WALK: return WALK;
		case CMID_SPECIAL1_W: return SPECIAL1_W;
		case CMID_SPECIAL2_W: return SPECIAL2_W;
		case CMID_SPECIAL3_W: return SPECIAL3_W;
		case CMID_SPECIAL1_B: return SPECIAL1_B;
		case CMID_SPECIAL2_B: return SPECIAL2_B;
		case CMID_SPECIAL3_B: return SPECIAL3_B;
		case CMID_ORIGINAL1_W: return ORIGINAL1_W;
		case CMID_ORIGINAL2_W: return ORIGINAL2_W;
		case CMID_ORIGINAL3_W: return ORIGINAL3_W;
		case CMID_ORIGINAL1_B: return ORIGINAL1_B;
		case CMID_ORIGINAL2_B: return ORIGINAL2_B;
		case CMID_ORIGINAL3_B: return ORIGINAL3_B;
		case CMID_DASH: return DASH;
		case CMID_DASH_ATK: return DASH_ATK;
		case CMID_WIN: return WIN;
		case CMID_START: return START;
		case CMID_SELECT: return SELECT;
		case CMID_RESULT_WIN: return RESULT_WIN;
		case CMID_RESULT_LOSE: return RESULT_LOSE;
		case CMID_JOKE: return JOKE;
		case CMID_RUN: return RUN;
		case CMID_JUMP_DASH_ATK: return JUMP_DASH_ATK;
		case CMID_ATK1_FASTINPUT: return ATK1_FASTINPUT;
		case CMID_JUMP: return JUMP;
		case CMID_JUMP_ATK: return JUMP_ATK;
		case CMID_JUMP_DASH: return JUMP_DASH;
		case CMID_JUMP_READY: return JUMP_READY;
		case CMID_DOUBLE_JUMP: return DOUBLE_JUMP;
		case CMID_DOUBLE_JUMP_ATK: return DOUBLE_JUMP_ATK;
		case CMID_DOUBLE_JUMP_LAND: return DOUBLE_JUMP_LAND;
		case CMID_CRITICAL_ATK: return CRITICAL_ATK;
		case CMID_DOUBLE_ATK: return DOUBLE_ATK;
		case CMID_ARROWDEF_GROUND1: return ARROWDEF_GROUND1;
		case CMID_ARROWDEF_GROUND2: return ARROWDEF_GROUND2;
		case CMID_ARROWDEF_SKY: return ARROWDEF_SKY;
		case CMID_BURNNING_CHARGE: return BURNNING_CHARGE;
		case CMID_BURNNING_ACTIVE: return BURNNING_ACTIVE;
		case CMID_DOWN_STAND_UP_EVASION: return DOWN_STAND_UP_EVASION;
		case CMID_SID_EXPANSION_MANA: return SID_EXPANSION_MANA;
		case CMID_SID_SPECIAL4: return SID_SPECIAL4;
		case CMID_SAVING_STANCE: return SAVING_STANCE;
		case CMID_SAVING_ATTACK: return SAVING_ATTACK;
		case CMID_SAVING_EVASION: return SAVING_EVASION;
		case CMID_SAVING_SKILL_ID: return SAVING_SKILL_ID;
		};
		return WAIT;
	}

	KProtectedType<int> ATK1, ATKEND, WAIT, FATAL_WAIT, WALK;
	KProtectedType<int> SPECIAL1_W, SPECIAL2_W, SPECIAL3_W, SPECIAL1_B, SPECIAL2_B, SPECIAL3_B;
	std::vector<int> SPECIAL_ADD_MOTION;
	KProtectedType<int> ORIGINAL1_W, ORIGINAL2_W, ORIGINAL3_W, ORIGINAL1_B, ORIGINAL2_B, ORIGINAL3_B;
	KProtectedType<int> DASH, DASH_ATK, WIN, START, SELECT, RESULT_WIN, RESULT_LOSE, JOKE, RUN, JUMP_DASH_ATK;

	// Z키 연타시 나오는 기술
	KProtectedType<int> ATK1_FASTINPUT;
	KProtectedType<int> iFastInputNeedCount; // 필요연타수

	KProtectedType<int> JUMP, JUMP_ATK, JUMP_DASH, JUMP_READY, DOUBLE_JUMP, DOUBLE_JUMP_ATK, DOUBLE_JUMP_LAND;
	KProtectedType<int> iJumpAtkNoCheckContact;

	KProtectedType<int> CRITICAL_ATK, DOUBLE_ATK;

	//화살 막기 ENABLE_ARROWDEF : 1이면 뒤의 막기 기술들이 활성화 될 수 있으며, -1이 아닌 것들만 적용된다
	KProtectedType<BOOL> ENABLE_ARROWDEF;
	KProtectedType<int> ARROWDEF_GROUND1, ARROWDEF_GROUND2, ARROWDEF_SKY; //ENABLE_ARROWDEF이 1이면 반드시 이 세가지는 채워져야함
	KProtectedType<int> ARROW_REFLECTION; //-1이면 반사, 회피 못함
	KProtectedType<int> iRefelectionRate; //반사, 회피확률 0 - 100

	//땅에서 누워있는 상태로 Dash하면 발동 되는 기술..
	//Tiger Rolling Action 타이거셋을 모두 차고 있다면 쓸수 있다. -1이면 사용 못함
	KProtectedType<int> GROUND_DOWN_DASH;

	KProtectedType<int>		BURNNING_CHARGE;	// 진 버닝차지모션
	KProtectedType<int>		BURNNING_ACTIVE;	// 진 버닝모드 발동 모션

	KProtectedType<int>		DOWN_STAND_UP_EVASION;	// 다운 됐을 시 기상 회피 동작

	std::map< int, int >    mapEnemyAtkGuardMotion;

	KProtectedType<int>		SID_EXPANSION_MANA;	// 마나 확장 스킬아이디

	KProtectedType<int>     SID_SPECIAL4;
	KProtectedType<int>     SID_SPECIAL4_1;

	KProtectedType<int>		SAVING_STANCE;	    // 세이빙 자세
	KProtectedType<int>		SAVING_ATTACK;	    // 세이빙 공격
	KProtectedType<int>		SAVING_EVASION;	    // 세이빙 회피
	KProtectedType<int>		SAVING_SKILL_ID;	// 세이빙스킬ID

	//=============================점프능력==============================
	KProtectedType<float> fJumpYSpeed;
	KProtectedType<float> fJumpXSpeed;

	KProtectedType<float> fDoubleJumpYSpeed;
	KProtectedType<float> fDoubleJumpXSpeed;

	KProtectedType<float> fRunJumpYSpeed;
	KProtectedType<float> fRunJumpXSpeed;

	KProtectedType<float> fDashJumpYSpeed;
	KProtectedType<float> fDashJumpXSpeed;

	//다음 Z입력때 까지 남은 여유
	//Elesis의 경우 0으로 주면 적을 때렸을때 자동으로 15로 할당됨
	KProtectedType<int> iNextAttackCount;
	//=====================================================================

	//걷기와 달리기 속도
	KProtectedType<float> fWalkSpeed;
	KProtectedType<float> fRunSpeed;

	//회복속도
	KProtectedType<float> fIncHP; //HP자동 회복 속도

	KProtectedType<int> CATCH, WAKE;

	// 맞았을 때, 경직되는 정도
	KProtectedType<DWORD> dwStopOnDamage;

	// 캐릭터 키
	KProtectedType<float> fCharHeight;
	// 게임 플레이시 캐릭터 스케일
	KProtectedType<float> fPlayingScale;

	// 무기 위치
	D3DXVECTOR3 vWeaponBase;
	D3DXVECTOR3 vWeaponEnd;
	D3DXVECTOR3 vLeftWeaponBase;
	D3DXVECTOR3 vLeftWeaponEnd;

	//타격 이펙트 ( 기본 )
	std::vector< HIT_EFFECT > vecHitEffect;

	PLAYER_TEMPLATE_SOUND sSound;

	std::map< int, std::pair<int, int> >    mapCommonToOwnMotion;

	// 데미지 팩터 ( 몬스터에게 맞았을때 데미지 받는 양.. )
	KProtectedType<float> fDamagefactor;

	// 캐릭터가 가지는 서브능력 타입저장 변수
	KProtectedType<int> 	iSubAbilityType;

	std::map< int, std::map< int, std::pair< int, float > > > mapSpecialBuff;

	std::map< int, std::vector< int > > mapPuzzleKeyPattern;
	std::set< int > setPuzzleKeyList;

	// 진 - 버닝게이지 관련 기준 시간저장변수들
	KProtectedType<float>	fBPChargeTime;		// 게이지 풀이 될 때까지 걸리는 시간
	KProtectedType<float>	fBPStopDelayTime;	// 자동감소가 일어나기까지 걸리는 지연시간.
	KProtectedType<float>	fBPDecreaseTime;	// 자동감소되어 게이지가 0이 될 때까지 걸리는 시간.
	KProtectedType<float>	fBPHitIncrease;
	bool	bUseBurnCharge;
	KProtectedType<float>   fBPPerAttackPlayer; // 적 플레이어를 공격했을 때 얻는 버닝포인트
	KProtectedType<float>   fBPPerAttackMonster;// 몬스터를 공격했을 때 얻는 버닝포인트
	KProtectedType<float>   fBPPerAttackedMe;   // 자신이 공격을 받았을 때 얻는 버닝포인트

	// 마리 1차전직 히팅게이지용 변수
	KProtectedType<float>	fHGPCumulativeNum;	// 히팅게이지 1회 누적 수
	KProtectedType<float>	fHGPDecreaseTime;	// 히팅게이지 풀 상태에서 다 비워질 때까지의 소모시간

	// 서브 능력 수량포인트 관련 변수
	KProtectedType<int> 	iQPChargeFrame;	// 수량포인트 1개를 회복하기 위한 프레임
	KProtectedType<int> 	iMaxQP;	// 최대 수량포인트
	KProtectedType<int>     iRageModeHpPercent;

	KProtectedType<int> iMaxBP;//0x37C
	KProtectedType<float> fSpecial4HPReduceRation;//0x384

	KProtectedType<float>   fMaxMarbleAmount;
	std::vector< SMarbleSytemInfo > vecMarbleSytemInfo;
	std::vector< SBurnSytemInfo > vecBurnSytemInfo;
	std::vector< SBloodSytemInfo > vecBloodSytemInfo;

	// 몬스터 때릴 시 마나 회복시키는 비율 ( 기존 회복율의 비율임 )
	KProtectedType< float > fManaRecoveryRatioByAttackMon;
	// 자동으로 마나 회복시키는 비율 ( 기존 회복율의 비율임 )
	KProtectedType< float > fManaAutoRecoveryRatio;

	std::map< int, SCombinationSkillSetInfo > mapCombinationSkillList;

	std::map< int, float >  mapControlCountCoolTimeSkillList;

	int iWeaponBoneIndex;
	int iWeaponBoneIndexLeft;

	// 변신에 사용되는 정보들...
	METAMORPHOSIS_TEMPLATE sMetamorphosisTemplate;

	TAG_TEMPLATE sTagTemplate;

	//필살기 이펙트 케릭터 얼굴
	SPECIAL_FACE sPlayerSpecialFace;

	//다른 필살기 얼굴
	int iOtherFaceHowToCheck; // 해당 조건일 때 필살기 얼굴을 교체 시킨다.
	int iOtherFaceCheckValue; // 체크 값
	std::string strOtherFaceTextureName;

	// 이 케릭터가 사용하는 사운드 인덱스의 리스트
	std::set< int > setCharSound;

	// 파이어인포
	std::set<int> setMeshAnimation;

	// 검궤적
	PLAYER_SLASH sPlayerSlash;

	// 이 케릭터가 사용하는 모션 인덱스의 리스트
	std::set< int > setStandardCharMotion;
	std::set< int > setExtendCharMotion;

	std::set< int > setDamageList;

	// 퀵슬롯에 자동으로 장착 스킬 퀵슬롯 스킬 리스트
	std::vector< int > vecQuickSlotSkill;

	std::vector< EXTRA_MESH > vecExtraMesh;
	std::map< int, FACE_LIFT_MESH > mapFaceLiftMesh;
	std::vector< ADD_MESH > vecAddMesh;
	std::vector< ITEM_MESH_OPTION > vecItemMeshOption;

	bool WeaponRenderBasically;
	bool LeftWeaponRenderBasically;
	float BurningCostForSpecial;

	// 캐릭터에 기본으로 붙어 들어가는 파티클시스템
	CharBaseEffect BaseEffect;

	// 캐릭터가 소환할 몬스터 ID리스트( 게임 시작 전 미리 로딩을 해주기 위해.. )
	std::vector< int > vecSummonMonsterID;

	std::set< int > setStatsSkillMotionList;

	std::map<int, SSkillCommandList> m_mapCommandList;
	std::map<int, SSkillCommandList> m_mapTutorialCommandList;
};

typedef std::map< int, std::vector<std::pair< std::set<int>, std::set<int> > > > MAP_OVERLAPPED_PAIR;

//태그매치 뺀다매 안빼나
struct TagCharacterInfo
{
	BOOL                            m_bNeedDelete;
	int                             m_iCharType;
	EPlayerState                    m_ePlayerState;
	KSafeArray<PLAYER_TEMPLATE, FORM_NUM> m_FormTemplate;
	KSafeArray<PLAYER_TEMPLATE, BTYPE_NUM> m_BurnningTemplate;
	char                            Extra_Char_Num;

	char                            m_cLife;
	float                           m_fHP;
	float                           m_fAbility[ABILITY_MAX];
	float                           m_fMana;

	int                             m_iSkillType;
	int                             m_iWeaponType;

	int                             MetamorphosisForm;
	KSafeArray<GCObject*, FORM_NUM> m_pObjMetaForm;
	bool                            m_bSuperArmor;
	bool                            m_bHyperArmor;

	std::vector<SpecialAbilityItem> m_vecSpecialAbility;
	std::set<EItemEffect>           m_setExtra2;

	std::map< std::pair<EGCMETAMORPHOSIS, DWORD>, GCITEMUID > m_mapwSkillItem;          // std::map< <변신종류, 스킬슬롯>, ItemUID >
	bool                            m_bIsBlackMagic;
	//리르 화살 처리가 왜 안되어 있었을까 ㅡ
	GCMeshObject*                   m_pLire3Arrow;

	// Effect 들...
	std::vector< int >              m_vecPetAiID;
	std::vector<ITEM_EFFECT> m_vecItemEff;
	std::vector<ITEM_EFFECT> m_vecDamageEff;
	std::vector<ITEM_EFFECT> m_vecSkillEff;

	std::vector<KGCAnimManager::ItemAnim> m_ItemAnim;
	std::vector<KGCAnimManager::DamageAnim> m_DamageAnim;
	std::vector<KGCAnimManager::SkillAnim*> m_SkillAnim;
	std::vector<KGCAnimManager::ItemAnim> Las4GreatSword;

	CParticleEventSeqPTR m_pRonan3Force;
	CParticleEventSeqPTR m_pRonan3Force1;
	CParticleEventSeqPTR m_pRonan3Force2;
	CParticleEventSeqPTR m_pRuneShield;
	CParticleEventSeqPTR m_pRuneShield1;
	CParticleEventSeqPTR m_pLasFireWingLeft;
	CParticleEventSeqPTR m_pLasFireWingRight;
	CParticleEventSeqPTR m_pDashEff;
	CParticleEventSeqPTR m_pLasMotalBlowReady;
	KSafeArray<CParticleEventSeqPTR, 4> m_pMonoVolt;
	KSafeArray<CParticleEventSeqPTR, 2> m_pAbyssHandEff;
	CKTDGAnim::CAnimInstance* m_pLasMotalBlowReadyAnim;

	PLAYER_TEMPLATE                 m_SwapTemplate;
	DWORD m_Elemental;
	int m_StrongLevel;

	std::vector< GCHotKeySkill > m_vecHotKeySkill;

	// 2007/3/13. iridology. 이 구조체의 초기화는 BeforeStartGame에서
	void Init()
	{
		m_bNeedDelete = TRUE;
		m_iCharType = -1;
		m_ePlayerState = PS_INVALID;
		for (int i = 0; i < FORM_NUM; ++i)
			m_FormTemplate[i].Init();
		m_SwapTemplate.Init();
		Extra_Char_Num = -1;
		m_cLife = -1;
		m_fHP = -1.0f;
		m_fMana = -1.0f;
		m_iSkillType = 0;
		m_iWeaponType = 0;
		MetamorphosisForm = FORM_NORMAL;
		//m_pFormShadow = NULL;
		for (int i = 0; i < FORM_NUM; ++i)
			m_pObjMetaForm[i] = NULL;

		m_pLire3Arrow = NULL;
		m_bSuperArmor = false;
		m_bHyperArmor = false;
		m_setExtra2.clear();
		m_vecSpecialAbility.clear();
		m_mapwSkillItem.clear();
		m_bIsBlackMagic = true;

		m_vecPetAiID.clear();
		m_vecSkillEff.clear();
		m_vecDamageEff.clear();
		m_vecItemEff.clear();
		Las4GreatSword.clear();

		m_ItemAnim.clear();
		m_DamageAnim.clear();
		m_SkillAnim.clear();

		m_pRonan3Force = NULL;
		m_pRonan3Force1 = NULL;
		m_pRonan3Force2 = NULL;
		m_pRuneShield = NULL;
		m_pRuneShield1 = NULL;
		m_pLasFireWingLeft = NULL;
		m_pLasFireWingRight = NULL;
		m_pDashEff = NULL;
		m_pLasMotalBlowReady = NULL;
		m_pLasMotalBlowReadyAnim = NULL;

		m_Elemental = 0;
		m_StrongLevel = 0;

		m_mapOverlappedPair.clear();

		for (int i = 0; i < ABILITY_MAX; ++i)
		{
			m_fAbility[i] = 0.0f;
		}

		m_vecHotKeySkill.clear();
	}
	TagCharacterInfo()
	{
		Init();
	}
	void InitTagCharData(PLAYER* pPlayer);
	inline void SetCharType(int iCharType)
	{
		m_iCharType = iCharType;
	}
	void EffectClear(int MyNumber);
	void ShowEffect(bool bShow);
	std::string GetCharString();
	std::string toStr();
	std::string GetPlayerStateStr();

	GCObject* GetMetaForm(int iFormID) { return m_pObjMetaForm[iFormID]; }
	GCObject* GetMetaForm(void) { return m_pObjMetaForm[MetamorphosisForm]; }

	PLAYER_TEMPLATE &GetFormTemplate(int iFormID) { return m_FormTemplate[iFormID]; }
	PLAYER_TEMPLATE &GetCurFormTemplate(void) { return m_FormTemplate[MetamorphosisForm]; }

	MAP_OVERLAPPED_PAIR m_mapOverlappedPair;

	void SetTagCharacterAbility(IN const EAbilityData& stAbilityData_);
};

struct ItemEffectParam
{
	ItemEffectParam(PLAYER* pPlayer, std::vector< int >& vecItemInfo_);
	ItemEffectParam(GCSquarePeople* pPeople, std::vector< int >& vecItemInfo_);
	ItemEffectParam(PLAYER* pPlayer, TagCharacterInfo* pTagInfo, std::vector< int >& vecItemInfo_);
	~ItemEffectParam() {}

	CHAR_IDENTY charID;
	DWORD& m_Elemental;
	std::vector< int >& vecItemInfo;
	int& m_StrongLevel;

	std::vector< ITEM_EFFECT >& m_vecItemEff;
	std::vector< ITEM_EFFECT >& m_vecDamageEff;
	std::vector< ITEM_EFFECT >& m_vecSkillEff;
	std::vector< KGCAnimManager::ItemAnim >& m_ItemAnim;
	std::vector< KGCAnimManager::DamageAnim >& m_DamageAnim;
	std::vector< KGCAnimManager::SkillAnim* >& m_SkillAnim;

	MAP_OVERLAPPED_PAIR& m_mapOverlappedPair;
	int m_iPlayerIndex;

	bool m_bSquare;

	std::vector< KSlotData > m_vecItemSlot;
	std::vector< DWORD> m_vecItemSlotSquare;
	std::vector< int >& m_vecPetAiID;
};

struct ProcessPositionDescParam
{
	D3DXMATRIX matWorld;
	int MetamorphosisForm;
	char Extra_Char_Num;
	PositionDesc& m_posDesc;
	PLAYER_TEMPLATE& PlayerTemplate;
	bool bLeftAtktoRight;
	GCObject* m_pObject;

	ProcessPositionDescParam(PLAYER* pPlayer);
	ProcessPositionDescParam(GCSquarePeople* pPeople);
	~ProcessPositionDescParam() {}
};

void UpdatePositionDesc(PositionDesc& posDesc, bool bIsLocalPlayer, int MetamorphosisForm);
void UpdateOverlappedPair(MAP_OVERLAPPED_PAIR& mapOverlappedPair, std::vector<KGCAnimManager::ItemAnim>& kItemAnim, std::vector<ITEM_EFFECT>& vecItemEff);
void SetItemEffect(ItemEffectParam& kParam);
void SetItemEffect2(ItemEffectParam& kParam);
void SetPlayerTemplate(std::vector< EGCSkillTree > setSkill, char CharNum, PLAYER_TEMPLATE& PlayerTemplate);
void GetPlayerTemplate(char CharNum, PLAYER_TEMPLATE& PlayerTemplate);
void ProcessPositionDesc(ProcessPositionDescParam& kParam);
D3DXVECTOR3* ComputePosDesc(PositionDesc& posDesc, int ePosType);
void SetProtectRingEffect(ItemEffectParam& kParam, DWORD dwItemID);
bool IsUseFaceLiftChar(char cExtraCharNum_, EGCFaceLiftType eFaceType_ = FACE_LIFT_NORMAL);

struct CreateItemEffectParam
{
	bool& bShowEffect;
	bool& bIsRight;
	float x;
	float y;
	float z;
	ITEM_EFFECT& stEffect;
	PositionDesc& posDesc;

	CreateItemEffectParam(bool& bShowEff_, bool& bIsRight_, ITEM_EFFECT& stEffect_, PositionDesc& posDesc_)
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, bShowEffect(bShowEff_)
		, bIsRight(bIsRight_)
		, stEffect(stEffect_)
		, posDesc(posDesc_)
	{
	}
};

void CreateItemEffect(CreateItemEffectParam& kParam);

#endif // _PLAYER_COMP_H_