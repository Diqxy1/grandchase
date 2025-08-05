#ifndef _PLAYER_H_
#define _PLAYER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Animation/KGCAnimManager.h"
#include "GCCollisionRect.h"
#include "PlayerComp.h"

#include "GCEffectsSword.h"
#include "KGCObjectEgo.h"
#include "TypeDef.h"
#include "KGCObj.h"
#include "GCSkill.h"
#include "KGCEmbarkManager.h"

#include "GCFaceOffManager.h"

struct SFrame;
class MONSTER;
class GCDeviceTexture;
class CDamageInstance;
class GCEffectsSword;
class KGCEmoticonInstance;
class CDamageInstance;
struct KCoordiSetTemplet;
struct KSummonInfo;
struct KInfinitySummonInfo;
//Function수가 많아 지는것을 우려한것임
//Key처리와 FrameMove를 동시에 처리하기 위해 만들어 졌습니다.
typedef int(*PLAYERSTATEFUNCTION)(PLAYER*, bool bKeyProc);

#define BACK_ATTACK_TERM_TIME       100
#define AERIAL_TERM_TIME            100
#define COUNTER_TERM_TIME           100
#define MISS_ATTACK_TERM_TIME       200
#define DEAULT_FALL_SPEED           0.0015f
#define MAX_TARGET_DAMAGE           10
#define MAX_MARBLE_BUFF_NUM         7

struct GCHotKeySkillInfo;
enum DAMAGE_TYPE;

// 엑스트라 메쉬 모션 루프 모드 : 등록된 모든 모션을 재생할 것인지 현재 모션만 재생할 것인지
enum
{
	EXTRA_MESH_LOOPMODE_ALL = 0,
	EXTRA_MESH_LOOPMODE_SINGLE = 1,
};

enum REMOVEPARTS // GC_GMC_*
{
	HEAD = 0,
	ARM = 1,

	MAX_PARTS, // 모드카테고리 개수
};

// 점령전 : 점령전의 통계 정보를 담고있는 struct
struct SDotaStats
{
	SDotaStats()
	{
		Init();
	}
	std::map<GCITEMID, int>                   m_mapItemUseCount;    // 아이템 사용 횟수.
	std::map<std::pair<DWORD, GCITEMID>, int> m_mapItemBuyCount;    // 캐릭터 레벨별 구입 아이템 횟수.
	std::map<DWORD, int>                       m_mapZoneDeathCount;  // Zone별 Death Count
	std::map<std::pair<DWORD, DWORD>, int >   m_mapDeathType;       //< <Type,Index>, Cnt > Type( Mon : 0, Char : 1 ),index( 고유값 )

	void Init()
	{
		m_mapItemUseCount.clear();
		m_mapItemBuyCount.clear();
		m_mapZoneDeathCount.clear();
		m_mapDeathType.clear();
	}
};

struct SFadeInOutInfo
{
	int m_iRed;
	int m_iGreen;
	int m_iBlue;
	int m_iAlpha;
	int m_iWeight;

	SFadeInOutInfo()
	{
		m_iRed = 0;
		m_iGreen = 0;
		m_iBlue = 0;
		m_iAlpha = 0;
		m_iWeight = 0;
	}
};

struct SCashSkillInfo
{
	int m_iSkillType;
	int m_iSkillLv;
	int m_iSkillID;
	std::vector< GCITEMID > m_vecGoodsID;

	SCashSkillInfo()
	{
		m_iSkillType = 0;
		m_iSkillLv = 0;
		m_iSkillID = 0;
		m_vecGoodsID.clear();
	}
};

class KChargingSkillLevelHigh
{
public:
	bool operator()(const GCHotKeySkillInfo p, const GCHotKeySkillInfo q)const
	{
		return p.m_iChargingSkillLV > q.m_iChargingSkillLV;
	}
};

struct kEmbarkArena
{
	int m_iEmbarkType;
	bool m_bRight;
	float m_fPosX;
	float m_fPosY;
	bool m_bUseArenaZoom;
	bool m_bSendPacket;

	kEmbarkArena() : m_iEmbarkType(-1), m_bRight(false), m_fPosX(0.0f), m_fPosY(0.0f), m_bUseArenaZoom(false), m_bSendPacket(false) {}
	bool IsSetting() { return (-1 < m_iEmbarkType); }
	void Init()
	{
		m_iEmbarkType = -1;
		m_bRight = false;
		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_bUseArenaZoom = false;
		m_bSendPacket = false;
	}
};

typedef std::map<int, GCITEMID>  MAP_HEROITEM;
typedef MAP_HEROITEM::iterator   MAP_HEROITEM_ITER;

class PLAYER
	: public KGCObj
{
public:
	//KDeclareLuaInvoke
	PLAYER(void);
	virtual ~PLAYER();
	PLAYER& operator =(const PLAYER& right_);

	struct DamageInfo {
	public:
		enum {
			DAMAGE_MONSTER = 0,
			DAMAGE_PLAYER,
			DAMAGE_PET,
			DAMAGE_BUFF,
		};

		DamageInfo() {
			Init();
		}

		void Init() {
			m_fHPDelta = 0.f;
			m_iAttacker = -1;
			m_bAffectedByAtk = true;
			m_bAffectedByDef = true;
			m_iAttackerType = DAMAGE_PLAYER;
			m_bIgnoreDef = false;
			m_bWeakToShield = false;
			m_iDamageKind = -1;
			m_dwDamageAtt = 0;
			m_bDamageFactor = true;
			m_bCheckCritical = true;
			m_bSpecialAttack = false;
			m_bReflectionEnable = false;
			m_bShieldCountEnable = false;
			m_nSpecialGrade = 0;
			m_bNotComboBreak = false;
		}
		void SetDamage(float fDamage_) {
			m_fHPDelta = fDamage_;
		}
		void SetAttacker(int iType_, int iIndex_) {
			m_iAttackerType = iType_;
			m_iAttacker = iIndex_;

			//             if( iType_ == DAMAGE_PLAYER ||  iType_ == DAMAGE_PET ) {
			//                 ASSERT( iIndex_ >= 0 && iIndex_ < MAX_PLAYER_NUM );
			//             }
			//             else if(  iType_ == DAMAGE_MONSTER ) {
			//                 ASSERT( iIndex_ >= 0 && iIndex_ < MAX_MONSTER );
			//             }
		}
		void SetDamageFactor(bool bEnable_) {
			m_bDamageFactor = bEnable_;
		}
		void SetAbilityEffect(bool bAttackPoint_, bool bDefencePoint_) {
			m_bAffectedByAtk = bAttackPoint_;
			m_bAffectedByDef = bDefencePoint_;
		}
		void SetIgnoreDefence(bool bIgnore_) {
			m_bIgnoreDef = bIgnore_;
		}
		void SetCriticalCheck(bool bEnable_) {
			m_bCheckCritical = bEnable_;
		}
		void SetDamageInstanceAttacker(const CDamageInstance* damage_);
		void SetDamageInstance(const CDamageInstance* damage_);
		void SetWeakToShield(bool bEnable_) {
			m_bWeakToShield = bEnable_;
		}
		void SetSpecialAttack(bool bSpecial_, int nSpecialGrade_) {
			m_bSpecialAttack = bSpecial_;
			m_nSpecialGrade = nSpecialGrade_;
		}

		float GetHPDelta() const { return m_fHPDelta; }
		int GetAttacker() const { return m_iAttacker; }
		int GetAttackerType() const { return m_iAttackerType; }
		bool IsAffectedByAtk() const { return m_bAffectedByAtk; }
		bool IsAffectedByDef() const { return m_bAffectedByDef; }
		bool IsIgnoreDef() const { return m_bIgnoreDef; }
		bool IsWeakToShield() const { return m_bWeakToShield; }
		bool IsSpecialAttack() const { return m_bSpecialAttack; }
		int GetSpecialGrade() const { return m_nSpecialGrade; }
		int GetDamageKind() const { return m_iDamageKind; }
		DWORD GetDamageAtt() const { return m_dwDamageAtt; }
		bool IsDamageFactor() const { return m_bDamageFactor; }
		bool IsCheckCritical() const { return m_bCheckCritical; }
		bool IsPlayerDamage() const;
		bool IsMonsterDamage() const;
		bool IsPetDamage() const { return m_iAttackerType == DAMAGE_PET; }
		bool IsBuffDamage() const { return m_iAttackerType == DAMAGE_BUFF; }
		bool IsReflectionEnable() const { return m_bReflectionEnable; }
		void SetReflectionEnable(bool val) { m_bReflectionEnable = val; }
		bool IsShieldCountEnable() const { return m_bShieldCountEnable; }
		void SetShieldCountEnable(bool val) { m_bShieldCountEnable = val; }
		void SetNotComboBreak(const bool bSet_) { m_bNotComboBreak = bSet_; }
		bool IsNotComboBreak() const { return m_bNotComboBreak; }
	private:
		float   m_fHPDelta;
		int     m_iAttacker;
		int     m_iAttackerType;
		bool    m_bAffectedByAtk;
		bool    m_bAffectedByDef;
		bool    m_bIgnoreDef;
		bool    m_bWeakToShield;
		bool    m_bSpecialAttack;
		int     m_nSpecialGrade;
		int     m_iDamageKind;
		DWORD   m_dwDamageAtt;
		bool    m_bDamageFactor;
		bool    m_bCheckCritical;
		bool    m_bReflectionEnable;
		bool    m_bShieldCountEnable;
		bool    m_bNotComboBreak;
	};

	KProtectedType<UINT>            uiMotion;
	KProtectedType<USHORT>           cFrame;
	KProtectedType<USHORT>           cBeforeFrame;

#if !defined(__PATH__)
	int m_iFrame;
#endif

	UINT GetCurrentMotion() { return uiMotion; }

	static void RegisterLuabind();

	bool VerifyMotion(int iMotion);
	bool VerifyRealMotion(int iMotion);
	float GetCorruptedBuffNumber();
	void SetRenderUnoFaceLift();
	SCharInfo& GetCurrentChar(void)
	{
		return m_kUserInfo.GetCurrentChar();
	}

	SCharInfo& GetCharInfo(int iChar_)
	{
		return m_kUserInfo.GetCurrentChar(iChar_);
	}

	int GetCurrentCharLevel(void)
	{
		return GetCurrentChar().iLevel;
	}

	D3DXVECTOR2 GetScreenPos();
	inline bool IsLocalPlayer() { return m_kUserInfo.dwUID == g_kGlobalValue.m_kUserInfo.dwUID; }
	void SetHostPlayer(bool bIsLeaveHost_ = true);
	static void SetStateFunctions();
	bool SetPlayerState(int iStateID);
	void FrameInfoProc(UINT uiMotion, unsigned char Info, unsigned short Reserved1, unsigned short Reserved2, int Frame);
	void SetPlayerSkill(); //아이템에 맞게 플레이어 스킬을 설정해줍니다.
	int GetSpecialMotionID(int iSpecial, int* outiDuration = NULL); //현재 캐릭터의 필살기번호를 돌려줍니다. iSpecial단계의...

	void MoveXInSky();
	bool CheckContact(); //return true이면 SlowCount를 적용시킵니다.
	bool ProcessSlowCount(); //return false이면 FrameMove를 멈춤니다.
	// Item & 특수기술의 사용
	void CommonKeyInput();
	bool CheckItemKey();
	void CheckSkillKey();
	void CheckPetKey();
	bool CheckTransformPetSkill();
	void CheckGameItemKey();
	void CheckJokeKey();
	void CheckShortCutItemKey();
	void CheckCameraZoomKey();
	void UseGameItem(bool bNoSound = false);

	void ArmeChageBlackNWhite();
	void EffectDust(float fPosX = 0.0f, float fPosY = 0.0f);
	bool CheckStandBlock(float fX, float fY);

public:
	enum
	{
		HEALING_DEFAULT,
		HEALING_FRESH_JOKE,
	};
	void SetIsRight(bool bIsRight_) {
		bIsRight = bIsRight_;
	}
	bool GetIsRight() {
		return bIsRight;
	}

	bool* GetPlayerDir() { return &bIsRight; }
	void SetEPlayerState(EPlayerState eState);
	EPlayerState GetPlayerState() { return m_ePlayerState; }
	void UpdateItemTime();
	void RecoveryHP_MP();
	void IncHP_inFrameMove(float RecoverValue = 0.0f);
	void IncMP_inFrameMove(float RecoverValue = 0.0f);
	bool IsSkipFrameTime();
	bool IsCanUseHealingBall(int iWhatWho = HEALING_DEFAULT);      // iWhatWho가 HEALING_FRESH_JOKE이면 도발키로 사용된 카제아제 힐링볼 임.
	void UseHealingBall();
	void GetProtectRingInfo(int& iItemID, int& iDuration);
	void GetHealingBallInfo(int& iItemID, int& iDuration);

	//Global에 있는 User정보를 PLAYER로 복사해온다..
	//현재 플레이어가 사용할 것임을 나타낸다.
	void SetCollectionData();
	void SetMyPlayer(int iSelChar = -1, bool bNoAllocation_ = false);
	//현재 객체의 정보를 전역 정보로 업데이트한다.
	void UpdateGlobalUserInfo();
	void UpdateTempPlayerUserInfo();
	void SetSpecialAbility(float& attP, float& defP, float& HPP, bool bForShopPlayer_ = false);
	void SetSpecialAbility(TagCharacterInfo* pTagInfo, float& attP, float& defP, float& HPP);
	void setTotalDamage(int totalDamage);
	void BeforeStartGame();
	void CreatePlayer(bool bLoadResource = true, bool bArena = false);
	void EndGame();
	void ClearItemRemainTime();

	void PresetTutorialChars(std::string strEquipScrip = "TutorialEquip.lua");
	void InitTutorialChar(int iChar_, int iExtraNum_ = -1);

	void LoadBaseSkillInfo();

	std::string GetFormString();
	std::string GetCharString();

	std::vector< CollectionAttribute > m_vecCharacterCollectionAttributeList;
	std::vector< VIPAttribute > m_vecVIPCollectionAttributeList;

	void SetCharacterCollectionAttributeList();

	void SetVIPCollectionAttributeList();
	std::wstring GetVIPCollectionExStringStatus();
	std::wstring GetVIPCollectionAllStringStatus();
	std::wstring GetVIPCollectionStringByFontID(int iFontID);

#ifdef DECANEE
	void UpdateSubjulgate();
#endif

	std::wstring GetCharCollectionAllStringStatus();
	std::wstring GetCharacterCollectionStringByPromotion(int iCharType, int iPromotion);

	int GetTotalVIPIndex();

public:
	// 게임 아이템 관련
	inline void ClearGameItem(void)
	{
		m_vecGameItem.clear();
	}
	inline void EraseGameItem(int iIndex)
	{
		if (iIndex < 0 || (int)m_vecGameItem.size() <= iIndex) return;
		m_vecGameItem.erase(m_vecGameItem.begin() + iIndex);
	}
	inline void SetNumGameItem(int iNumGameItem)
	{
		m_vecGameItem.clear();
		for (int i = 0; i < iNumGameItem; ++i) m_vecGameItem.push_back(-1);
	}
	inline int GetNumGameItem(void)const
	{
		return static_cast<int>(m_vecGameItem.size());
	}
	inline int GetGameItem(int iIndex)const
	{
		if (iIndex < 0 || (int)m_vecGameItem.size() <= iIndex) return -1;
		return m_vecGameItem[iIndex];
	}
	inline bool IsGameItem(int iItemIdx)const
	{
		std::vector<int>::const_iterator vecIter = m_vecGameItem.begin();
		for (; vecIter != m_vecGameItem.end(); ++vecIter)
			if ((*vecIter) == iItemIdx)
				return true;
		return false;
	}

	// 로그인 아이디 관련
	inline void SetPlayerLoginID(const WCHAR* strLogin_)
	{
		m_kUserInfo.strLogin = strLogin_;
	}
	inline const WCHAR* GetPlayerLoginID(void)const
	{
		return m_kUserInfo.strLogin.c_str();
	}
	inline size_t GetPlayerLoginIDSize(void)const
	{
		return m_kUserInfo.strLogin.length();
	}
	// 닉네임 관련
	void SetPlayerName(const WCHAR* strNickName_, wstring strNickColor_);

	std::wstring& GetStrUserName() { return m_kUserInfo.strNickName; }

	std::wstring& GetStrUserColor() { return m_kUserInfo.strNickColor; }

	inline const WCHAR* GetPlayerName(void)
	{
		return GetStrUserName().c_str();
	}
	inline wstring GetPlayerColor(void)
	{
		return GetStrUserColor();
	}
	inline size_t GetPlayerNameSize(void)
	{
		return GetStrUserName().length();
	}
	inline void ClearPlayerName(void)
	{
		m_kUserInfo.strNickName = L"";
	}

public:
	void SetShield(int iShield, float fRegenRate = 0.0f, int iLimitTime = 0, bool bSpecialShield = false, int iTexNum = -1, bool bCountingShield_ = false);
	bool CheckHiding();
	bool m_bOnceChargeBloodGuage;
	void ProcessBackStepBuff();
	bool Is_SP_Character();
	bool Is_Hybrid_Character();
	bool Is_AP_Character();
	bool IsOneWeaponChar();     // 전직 무기가 없는 캐릭터인가?

	bool IsSpecialSkillMotion(int iMotion_ = -1);
	int  GetSpecialGrade();

	D3DXMATRIX* ComputeAngleBone(D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic);
	D3DXMATRIX* ComputePositionBone(D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& aPositionBone);
	void Add_Face();
	void Process_Face();
	void Draw_Face();
	int totalDamage;

	bool IsHighRanker();
	int AutoSetExtra();
	int getTotalDamage();
	void AutoSetExtra(TagCharacterInfo& tagInfo);
	bool IsCharTypeMode();
	int GetPromotion(int iCharType);
	int GetNowCharPromotion();
	void ClearPlayerData(bool bInitUserInfo = true);
	void Arena();
	void Calculate_Arena_Pos();
	void ArenaToInitPlayer(int iArenaCharType);
	void OnStartGame();
	void Calcualte_Point(bool DecDuration = false, bool bStartGame = false, bool bHPFull = true);
	void Calcualte_Point(TagCharacterInfo& tagInfo, bool DecDuration = false);
	bool Attack_Player(GCCollisionRect<float>* rtWeapon);
	bool Attack_Monster(GCCollisionRect<float>* rtWeapon);
	bool Attack_Object(GCCollisionRect<float>* rtWeapon);
	bool Attack_MinigameObj(GCCollisionRect<float>* rtWeapon);
	bool Attack_Damage(GCCollisionRect<float>* rtWeapon);
	void Shadow_Render();
	void Stop(float, float, bool IsAll = false);
	void QuickStop(float, float, bool IsAll = false);
	float Change_HP(DamageInfo damage_);
	float CalculationRealDamage(bool bPlayerDamage, float c_HP, float fAttributeAtk, float fAttributeDef, float fAttributeSpecialDef, float f_fAtkPoint, float fIncDpRatio);

	void ChangeHP(float fDamage, bool bEffectAtk, bool bEffectDef, bool bCheckCritical); //바인딩용
	void SetCatchDamage(float fDamage); //바인딩용
	void ReadyToRender(KRenderData* pkRenderData_ = NULL);
	void SetSlashV();
	void ChangeSlashColor(int iIndex = -1);
	void Direct_Motion_Input(UINT Motion, bool bSpecialDamage = false);
	void Draw_Slash();
	void Draw_UserNickName();
	void Draw_GuildMark();
	void SetPetName(std::wstring str);
	void Draw_PetName(int x, int y);
	void Draw_UserNickNameInPortal(int x, int y);

	void ReverseGravity(int Attaker = 6, float fGravity = 0.06f, float fLastGravity = 0.06f, bool bCheckContact = true, bool bSpecialDamage = false);
	void ShowAllItemEffect(bool bShow);
	void ShowBaseEffect(bool bShow);

	void Frame_Move();
	bool hasEquipedItem(int itemID);
	void OnDamagedProcess(void);
	void My_Delayed_SlowCount(short);
	void Crash_Check();
	void PushCheckWithOtherPlayers();
	void PushCheckWithMonsters();
	void PushCheckWithObjects();
	void PushCehckWithMinigameObj();
	void PushCheckWithDamage();
	bool IsCatchMotion(UINT Motion, bool bOnlyCatch = false);
	bool IsSlowAllowedMotion(UINT Motion);
	void Heal_To_Player(int What, float fRate = 0.f);
	bool IsSpecialUsable(bool bForceUse_ = false, bool bNoCheckContact_ = false);
	void UseSpecial(int iUseShortCutLv = -1);
	bool IsUsableStateSpecial(int iUseShortCutLv, float fLimit_);
	bool AppointUseSkill(EGCSkillTree eSkillID, bool bCheckUsable = true);
	void UseSkill(GCHotKeySkill* skill, bool bCheckUsable = true, int iComboIdx = -1);
	bool UseSkillNotCheck();
	bool IsSpecialWhite(int iLevel_);
	int CheckComboSkill(EGCSkillTree eSkill);
	void CalcBloodPoint(int num);
	int GetBloodPoint() const { return m_nBloodPoint; };
	bool IsTutorialMode();
	void DamageToSameTeam(int iDamage);

	void ClearDeSableSkill();
	void RegDeSableSkill(int iMotionID);
	std::vector< int > m_vecDesableSkillMotion;

	//====================================================================================
	// 2009.04.03 : Jemitgge
	// Comment : 데미지를 맞기 전에 해야 할것들 ex) 로난의 가드라던가...
	//			 데미지 자체를 맞아도 안맞은 것 처럼 하고 싶다! ㅎ
	bool OnDamagePreProcess(int hitWho, bool bPhyAtk = false, bool bMonsterDamage = false, bool bSpecial = false, int iWhat = -1/*뭐가 때리는감?*/, int iHitWho = -1);

	/************************************************************************/
	/* OnResetDevice() -> InitDevice()로 변경
	/************************************************************************/
	//void OnResetDevice();
	void InitDevice(void);

	void RecordBonePosition();
	void UpdateData();
	void Calculate_Body_Rect();
	bool Take_Item(char What, bool bNoMessage = false);
	void Eff_Item(EFFITEM);
	void All_Latency_Equal();
	void Sound(int, unsigned char delay = 0, bool bNoOverlap = false);
	void SoundForCharIndex(const char* ID, unsigned char delay = 0, bool bNoOverlap = false);
	void Ah_Sound(char, char Special_Delay = 40);
	void Kill(void);
	void Get_BasicMatrix(D3DXMATRIX* pMat, bool bLatency = true, bool bCamera = true, bool bForNickName = false);

	GCDeviceMotionFRM* GetNowMotionFRM();

	void Rebirth(char cLife = 1);
	bool IsInvinsible(bool bSpecialDamage);
	bool IsSuperNotSpecial(bool bSpecialDamage);

	void LoadPlayerTemplate(char Extra_Char_Num_ = -1);

	void SetSkillType(int iSkillType);
	void LoadPlayerTemplate(TagCharacterInfo* pTagInfo);

	void EffectInit();
	void EffectFrameMove();
	void BaseEffectFrameMove();
	void CreateBaseEffect(CharBaseParticle& BaseParticle);
	void EffectClear();
	void EffectClear(bool bClearBuf);
	void BaseEffectClear();

	void SetDamageEff(float x, float y, float z);

	void LoadPet();

	void UpdatePlayerResource(bool bIsTempPlayer_ = false, bool bInitEgoItem = true); //필요없는 Resource를 모두 반환하고 필요한것들을 모두 로딩한다.
	void UpdatePlayerResourceClean(bool bIsTempPlayer_ = false, bool bInitEgoItem = true);
	void SortItemVector(KUserItemVector& vecItemSorted);
	void UpdatePlayerResource(TagCharacterInfo* pTagInfo);
	void ReleasePlayerResource();

	void UpdatePlayerResourceOnGame(); //게임에서 필요한것들 까지 모두 로딩한다 -> 필살기 이미지 같은...
	void ReleasePlayerResourceOnGame(); //필요없는 모든 리소스를 반환함
	GCMeshObject* AddMeshObject(std::string strMesh_, std::string strTexture_, EGCCartoonTexture eCartoon_ = CARTOON_NORMAL);
	void SetTexture(char* szTex = NULL);
	void RemoveMeshObject(std::string strMesh_);

	void AddChildLiveModel(char* strName, char* strMesh, char* stdTexture, char* strMotion, int iBone, float fZPos);
	void RemoveChildLiveModel(char* strName);

	GCCollisionRect<float>& GetCollisionRect();

	void SetSlotAlign();
	bool FindSlot(int iChar);
	CParticleEventSeqPTR AddTraceParticle(const std::string& strSequence, float fOffsetX, float fOffsetY, float fTraceTime = FLT_MAX, int iTracePos = (int)EPT_BODY_CENTER);
	void AddParticle(char* strSeqeunce, int iParticleNum, float fOffsetX);
	void AddParticleXOffset(char* strSeqeunce, int iParticleNum, float fOffsetX);
	void AddParticleXOffsetWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fTraceTime, int iTracePos);
	void AddParticleWithAngle(char* strSeqeunce, int iParticleNum, float fPosX, float fPosY, float fOffsetX);
	void AddParticleWithBlackhole(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY = 0.0f);
	void AddParticleWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fTraceTime = 0.0f, int iTracePos = (int)EPT_BODY_CENTER);
	void AddParticlePosWithTrace(char* strSeqeunce, int iParticleNum, float fXDir, float fOffsetX, float fOffsetY, float fTraceTime = 0.0f, int iTracePos = (int)EPT_BODY_CENTER);
	void AddParticleNoDirection(char* strSeqeunce, int iParticleNum, float fOffsetX);
	void AddParticleNoDirectionWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fTraceTime = 0.0f, int iTracePos = (int)EPT_BODY_CENTER);
	void AddParticleNoDirectionPosWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY, float fTraceTime = 0.0f, int iTracePos = (int)EPT_BODY_CENTER);
	void AddParticleXOffsetNoMotion(char* strSeqeunce, int iParticleNum, float fOffsetX);
	void AddParticlePos(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY);
	void AddParticleNoDirectionPos(char* strSeqeunce, int iParticleNum, float fOffsetX, float fOffsetY);
	void AddParticleUINumber(int iNumber, D3DXVECTOR2 vOffsetPos, bool bPlusMark = false);
	void OnReverseGravity(float fRangeX, float fDamage, bool bFlyCheck, float fHeight = 0.0f, float fRangeY = 0.0f, float fOffSetX = 0.0f, float fOffSetY = 0.0f, bool bSpecialDamage = false);
	void OnEarthQuake(float fRange, float fDamage, float fPosX, float fPosY, float fHeight = 0.0f, bool bSpecialDamage = false);
	void Jump();
	void JumpMotion(float fJumpSpeed, int iMotionIndex);
	int  m_nBloodPoint = 6;
	int  m_nPlayerPing = 0;
private:
	// 새롭게 추가된 루아 바인딩 함수

	void StartAttack(float iDamage, int iPhysicAtkType, int iDamageType);
	// 몬스터에게만 해당되는 속성들입니다
	void SetAttackProperty(int iAttackType = (int)ATTACKTYPE_NORMAL, int iAttackDirection = (int)ATTACKDIR_MIDDLE,
		float fPushTargetX = 0.0f, float fPushTargetY = 0.0f, bool bSpecialAttack = false,
		float fPushPlayerX = 0.0f, float fPushPlayerY = 0.0f);

	void SetAttackInfo(int iPlusDelay, int iCameraCrash, int iHitDelayPlus) { m_iPlusDelay = iPlusDelay; m_iCameraCrash = iCameraCrash; m_iHitDelay += iHitDelayPlus; }
	void EndAttack();
	void CheckDie();
	void Catched(float fOffsetX = 0.1f);
	void CheckDie2End();
	void ChangeMotion(int iMotion);
	void Change_MP(float fChange);
	void ChangeAllCharacterMP(float fChange);
	void AddItem();
	void Run();
	void StartSpecial(BYTE r, BYTE g, BYTE b, bool bNoSound = false, float fZoom = 2.0, bool bNoFace = false, bool bNoLightBim = false);
	void StartSpecialNoZoom(BYTE r, BYTE g, BYTE b, bool bNoSound = false);
	D3DXVECTOR3* GetPos() { return &(vPos); }
	void Scream();
	void OnLire2Atk();
	float GetAttackPoint(const DamageInfo& damage_);

	void CreateItemModel(GCObject* pObject_, const SCharInfo& sCharInfo_, char cExtraCharNum_, bool bTempPlayer_);
	void CreateItemModelClean(GCObject* pObject_, const SCharInfo& sCharInfo_, char cExtraCharNum_, bool bTempPlayer_);
	void LoadItemModel(GCObject* pObject_, PLAYER_TEMPLATE& kPlayerTemplate_, GCItem* pItem_, int iCharType_, int iPromotion_, bool bTempPlayer_);
	void LoadAddtionModel(GCObject* pObject, PLAYER_TEMPLATE& kPlayerTemplate, bool bTempPlayer_);
	void LoadETCModel(GCObject* pObject, char cExtraCharNum_, bool bTempPlayer_);
	void LoadFaceLift(GCObject* pObject);
	void LoadFaceTexture(GCObject* pObject, int iCharType, int iPromotion, int iItemID, DWORD dwSlotPosition);
	std::string GetDefaultMeshName(DWORD dwSlotPosition);

public:
	void LoadRoomAnimationModel(GCObject* pObject_, PLAYER_TEMPLATE& kPlayerTemplate_, std::string RoomAnimationModel, bool bTempPlayer_, int isWeapon);
	void CreateShowRoomETCMotionFile(int iCharMotionIndex);
	std::pair<std::wstring, D3DCOLOR> CreatePlayerName(GCObject* pObject_);
	float GetMP();
	void DecreaseMP(float fChange);
	float GetX() { return vPos.x; }
	float GetY() { return vPos.y; }

	D3DXVECTOR2 GetXYPosition() { D3DXVECTOR2 vPosxy; vPosxy.x = vPos.x; vPosxy.y = vPos.y;  return vPosxy; }
	D3DXVECTOR3 GetPosition() { return vPos; }

	CDamageInstance* AddDamage(unsigned short Reserved1, unsigned short Reserved2, bool bAlwayRight = false, bool bSpecialAttack = false);
	void AddDamageAngleAndPos(unsigned short Reserved1, unsigned short Reserved2, float fPosX, float fPosY);
	bool AddDamageTarget(unsigned short Reserved1, D3DXVECTOR2* vpTargetPos);
	void AddDamageNoLatency(int iWhat, bool bSpecialAttack = false);
	void AddDamageWithLocate(int iWhat, float fXLoc, float fYLoc, bool bSpecialAttack = false, bool bNoLatency = false);
	void AddDamageWithLocateAbsolutePos(int iWhat, float fXLoc, float fYLoc, bool bSpecialAttack = false);
	void AddDamageWithNoDirectionLocateAbsolutePos(int iWhat, float fXLoc, float fYLoc, bool bSpecialAttack = false);
	void AddDamageWithStaticLocate(int iWhat, float fStartX, float fStartY, bool bSpecialAttack = false, bool bNoLatency = false);
	void AddDamageOnP2P(int What);
	void AddDamageRelation(CDamageInstance* damage);
	void DeleteDamage(int iDamageEnum_, int iPlayerIndex_);

	void AddParticleRelation(CParticleEventSeqPTR particle);
	void SetShowBaseEffect(bool bShow);
	void SetRecoveryHpByRadio(float fRadio);

	//LPDIRECT3DDEVICE8 Device;
	//GCDeviceVertexBuffer* V_Slash;
	// 2007/2/7. iridology. 라이언 1차전직 전용!
	//GCDeviceVertexBuffer* V_Slash_Left;
	KSafeArray<GCEffectsSword, 4> Slash;
	KSafeArray<GCEffectsSword, PAT_NUM> AttackSlash;
	SUserInfo m_kUserInfo;
	std::wstring m_strNation;

	KPressureState		        m_kPressureState; //방안에 누를수 있는가 알아보는 정보.
	bool						m_bIdle;

	KProtectedType<EPlayerState>            m_ePlayerState;
	KSafeArray<PLAYER_TEMPLATE, FORM_NUM> m_FormTemplate;
	PLAYER_TEMPLATE& GetFormTemplate(int iFormID) { ASSERT(-1 != iFormID); return m_FormTemplate[iFormID]; }
	PLAYER_TEMPLATE& GetCurFormTemplate(void) { ASSERT(-1 != MetamorphosisForm); return m_FormTemplate[MetamorphosisForm]; }

	static std::map<MOTIONID, PLAYERSTATEFUNCTION> ms_mapStateFunction;
	PLAYERSTATEFUNCTION m_pCurStateFunction;
	bool m_bBuffFrameLock; // 버프로 제어되는 프레임 락( m_bFrameLock은 이곳저곳에서 많이 사용되어 버프로 처리하면 버그많이 생김 )
	bool m_bFrameLock; //프레임이 정지 되어야 한다면..Frame++이 되지 않습니다.
	char m_iPlayerIndex;
	bool m_bReserveWake;

	KProtectedType<char>                    Extra_Char_Num;
	int m_iInvEffFrame;

	std::vector< int > m_vecCharNum;

	KProtectedType< char > m_cLife;
	float GetHP() const;
	void SetHP(float fHP_);
	KProtectedType< float >* GetHPPtr() { return &m_fHP; }
	void SetHPFull();
	float GetAttack() { return m_fAbility[ABILITY_ATK]; }
	void SetAttack(float fAtk_);
	char GetExtraNum() { return Extra_Char_Num; }

	unsigned char GetLastKillmePlayer() const { return Last_Killme_Player; }
	unsigned char GetLastMademeGravityPlayer() const { return Last_MademeGravity_Player; }

	void SetLastKillmePlayer(unsigned char val);
	void SetLastMademeGravityPlayer(unsigned char val);

	KProtectedType< float > m_fAbility[ABILITY_MAX];
	PositionDesc m_posDesc;

	void FrameMoveItemEffect();

	//파티클 시스템
	bool m_bShowBaseEffect;
	bool m_bShowEffect;
	std::vector<ITEM_EFFECT> m_vecItemEff;
	std::vector<ITEM_EFFECT> m_vecDamageEff;
	std::vector<ITEM_EFFECT> m_vecSkillEff;

	//애니메이션 시스템
	std::vector<KGCAnimManager::ItemAnim> m_ItemAnim;
	std::vector<KGCAnimManager::DamageAnim> m_DamageAnim;
	std::vector<KGCAnimManager::SkillAnim*> m_SkillAnim;
	std::vector<KGCAnimManager::ItemAnim> Las4GreatSword;

	// 엑스트라 메시 토글처리
	bool m_bRenderWeapon;
	bool m_bRenderMergeWeapon;
	int m_iToggleExtraMesh;

	bool m_bKamikiDefence;
	bool m_bPumpkinDefence;
	bool m_bKazeazeDefence;

	CParticleEventSeqPTR m_pRonan3Force;
	CParticleEventSeqPTR m_pRonan3Force1;
	CParticleEventSeqPTR m_pRonan3Force2;
	CParticleEventSeqPTR m_pRuneShield;
	CParticleEventSeqPTR m_pRuneShield1;
	CParticleEventSeqPTR m_pLasFireWingLeft;
	CParticleEventSeqPTR m_pLasFireWingRight;
	CParticleEventSeqPTR m_pDashEff;
	CParticleEventSeqPTR m_pLasMotalBlowReady;
	KSafeArray<CParticleEventSeqPTR, 4> m_pHoldEffect;
	CKTDGAnim::CAnimInstance* m_pLasMotalBlowReadyAnim;

	DWORD m_Elemental;
	int m_StrongLevel;

	D3DXVECTOR4 MyBody;
	D3DXVECTOR3 MyArmor;
	D3DXVECTOR3 MyArmor_Ex;
	D3DXVECTOR3 MyLeftArmor;
	D3DXVECTOR3 MyLeftArmor_Ex;

	std::set<EItemEffect> m_setExtra2;    // 특수 기능 관련
	bool IsUseItemEffect(EItemEffect enItemEffect) { return (m_setExtra2.count(enItemEffect) > 0); }
	void ClearUseItemEffect(void) { m_setExtra2.clear(); }
	std::set<EItemEffect>& GetUseItemEffect() { return m_setExtra2; }

	//=============================================================================================
	std::vector<SpecialAbilityItem> m_vecSpecialAbility;
	//=============================================================================================

	KSafeArray<KProtectedType<int>, GC_GAME_ITEM_NUM> Item_Remain_Time;
	short Next_Attack_Count;

	bool m_bIsPushRightKey; //방향키→누른 상태 저장하는 변수
	bool m_bIsPushLeftKey;  //방향키←누른 상태 저장하는 변수

	bool Is_IfDie2End;
	bool Is_Aleady_Scream;
	bool IsContact;

	bool First_Receive;
	bool m_bDeathFromFall;

	unsigned char Super;

	char ContactBlock;
	char m_cTargetPortalBlock;

	unsigned char Fatal_Sound_Delay;
	int Old_Frame;
	KProtectedType< int >           L_Frame[LATENCY + SHOWLATENCY + 10];

	int Item_Score;
	std::vector< KKillCoinInfo >    m_vecKillCoinInfo;
	void GetCatchGPSum(int& nGPSum) const;
	bool m_bThrowMatchGP;

	int                             Target_Score;
	std::map< char, GCEXPTYPE >   m_mapEndExp;

	KSafeArray<bool, LATENCY + SHOWLATENCY + 10> L_IsRight;
	bool Reserve_IsRight;
	bool Old_IsContact;
	int iContact_to_Stone; //-1이면 예약이 안된상태 나머지는 LastKillMePlayer가 들어가 있음
	int m_iStoneCurseType; // 스톤커즈 ㅡ.ㅡ+

	KSafeArray<float, LATENCY + SHOWLATENCY + 10> L_x;
	float Old_x;
	//float y;
	KSafeArray<float, LATENCY + SHOWLATENCY + 10> L_y;

	float GetY_Ex();

	float Shadow_y;
	float x_Speed;
	float y_Speed;
	float m_fFall_Speed;
	float x_WindSpeed;
	float y_WindSpeed;
	float m_fJump_WindRadio;
	float Old_HP;
	KSafeArray<float, LATENCY + SHOWLATENCY + 10> L_HP;

	KProtectedType< int >        L_Shield[LATENCY + SHOWLATENCY + 10];
	int m_iCatchPlayerIndex;
	float Gravity;
	float Last_Gravity;

	// 슬롯에 나오는 캐릭터 간이 동영상을 위한 멤버변수
	unsigned char Slot_Frame;
	float Slot_x;
	float Slot_y;

	// ============================ 공격 정보 ============================== //
	KProtectedType<float> m_fPhysicAtkPower;
	ePhysicAtkType m_ePhysicAtkType;
	ePhysicAtkReaction m_ePhysicAtkDamageAction;
	int m_iCameraCrash;
	int m_iPlusDelay;
	int m_iHitDelay; // 친넘이 멈추는 값 ㅡ
	float m_fPlayerPhysicPushX; // 물리데미지로 플레이어 X 속도조절 값
	float m_fPlayerPhysicPushY; // 물리데미지로 플레이어 Y 속도조절 값
	// ============================ 공격 정보 ============================== // 끝

	KProtectedType<int> Shield;
	int m_iPrevShield;
	bool m_bExistShield;
	bool m_bStartShield;
	int m_iSkillType;
	std::map< std::pair<EGCMETAMORPHOSIS, DWORD>, GCITEMUID > m_mapwSkillItem;          // std::map< <변신종류, 스킬슬롯>, UID >
	unsigned char NoCheckContact;
	unsigned char Slow_Count;
	KProtectedType< UINT >          L_Now_Motion[LATENCY + SHOWLATENCY + 10];
	KProtectedType<UINT> Old_Motion;
	KProtectedType<UINT> Reserve_Motion;
	unsigned char pLatency;
	//퀘스트3에 새롭게 등장하는 산소 게이지
	float m_fO2;

	// 원화 이미지 관련
	int Silk;
	bool m_bSilkRight;
	float m_fEff_x;
	float m_fEff_y;

	bool m_bIsOnceHeal; //Healing pet이 한번만 살려주기위한 코드
	bool IsNoCrashCheckMotion(int iMotion);
	bool m_bCrashCheck;

	//=============================================================================================
	//MagicEffect관련
	float m_fIncHPRatio;
	bool m_bIncHP;

	bool m_bMPRecoveryFix;
	float m_fMPRecoveryFix;

	bool IsMPRecoveryFix() { return m_bMPRecoveryFix; }
	float GetMPRecoveryFixValue() { return m_fMPRecoveryFix; }
	void SetMPRecoveryFix(bool bMPRecoveryFix, float fMPRecoveryFix) { m_bMPRecoveryFix = bMPRecoveryFix; m_fMPRecoveryFix = fMPRecoveryFix; }

	// 충돌한 발판인덱스
	int m_iContactX;
	int m_iContactY;

	float m_fGetExpGpRatio;

	// 몬스터에게 스페셜 어택을 성공시켰나 체크하는 함수
	// 퀘스트에서 필살기를 맞췄나 체크하기 위해서 사용되는 플래그

	std::map<int, bool> m_mapS1AttackState;
	std::map<int, bool> m_mapS2AttackState;
	std::map<int, bool> m_mapS3AttackState;
private:
	unsigned char Last_Killme_Player;
	unsigned char Last_MademeGravity_Player;
	KProtectedType< float > m_fHP;
	std::vector< int > m_vecGameItem;

	DWORD m_dwLire3Sp2FrameSum;

	GCDeviceTexture* m_pTexBerserk1;
	GCDeviceTexture* m_pTexBerserk2;
	GCDeviceTexture* m_pTexFatal;
	GCDeviceTexture* m_pTexSlash;

	//필살기 면상을 전부 이걸로 씁니다.
	GCDeviceTexture* m_pPlayerSp;

	GCDeviceTexture* m_pAmy4Action1;
	GCDeviceTexture* m_pAmy4Action2;
	GCDeviceTexture* m_pAmy4Action3;

	// 대장전 ㅋㅋㅋㅋㅋㅋㅋㅋ
	GCDeviceTexture* m_pSerdinSign;
	GCDeviceTexture* m_pCanabanSign;

	GCCollisionRect<float> m_kCollisionRect;

	// 리르 2차전직 컨트롤 관련 변수들
public:
	bool m_bRenderQuestDrama; //드라마에서 쓰는 변수에요..하드하게 죄송합니다.ㅠ

	KSafeArray<GCObject*, FORM_NUM> m_pObjMetaForm;
	// 리르 2차전직 컨트롤 관련 변수들
	bool m_bRenderLire3Arrow; // Lire3의 화살이 너!무! 커서 그리지 말아야 하는 경우가 생긴다.
	GCMeshObject* m_pLire3Arrow;

	int m_iJumpActionLimitCount;

	// 아르메 2차전직 컨트롤 관련 변수들
public:
	int m_iSummoning;
	bool m_bJumpZoomed;

	// 라스 2차전직 컨트롤 관련 변수들
public:
	bool m_bJumpDashed;

	// 라이언 관련 변수들
public:
	int         MetamorphosisForm;
	//    GCObject*   m_pFormShadow;
	std::vector< GCObject* > m_vecFormItemModel;
	// 변신할 때 사용
	void        ProcessMetamorphosisFormChange(int iFormID);
	// 변신풀 때 사용
	void        ProcessMetamorphosisNormalForm(bool b3rdTransformClear = true, int iChangeMotion_ = -1);
	// 변신 지속시간이 끝나서 원래대로 돌아가야 하는가?
	bool        m_bNormalForm;
	void        UpdateFormResource(int iFormID, OUT GCObject* objOut = NULL);
	void        UpdateFormResource(int iFormID, TagCharacterInfo* pTagInfo);

	//====================================================================================
	// 2007.02.22 : Asirion
	void        SyncTransform(UINT uiChangeMotion_);
	int         Next_Motion;
	char        Next_Direction;

	bool        CheckEquipItem(int ItemID, bool bCoordi = false);
	bool        CheckSetEquipItem(int ItemID);
	void        SetSuper(char* SuperEffect, int iParticleNum, float fOffsetX, float fTime = -1.0f, bool bUsingIgnore = false, bool bUseImmortal = true);
	void        SetSuper_(int value);
	bool        IsOldMotion(int iMotion);
	int         Before_Motion;

	bool RenderWeapon() const;
	void RenderWeapon(bool val);

private:
	// 변신 하거나 다시 원래대로 돌아 올 때 강화목걸이를 착용하고 있을 경우
	// 무기 이펙트가 남아 있으면 이상하다. 변신 할 때 없에주고 변신이 풀릴 때
	// 다시 보여주는 함수

public:
	void ToggleRenderPlayer(bool bShow);
	void SetShowEquipEffect(EMIT_POSITION_TYPE ePos, bool bShow);

public:

	// 물줄기
	int m_iJumpPossibleFrame;

public:
	// 데미지에 따른 카메라 흔들림 정도를 구하는 함수
	int GetShakeValue(float fDamage, float fHPpoint);

public:
	int m_iFlashEffectFrame;
	int m_iShakeEffectFrame;
	void HitEffect(GCCollisionRect<float>& kWeapon_, GCCollisionRect<float>& kTarget);
	void SetFlashEffectFrame(int iFlashEffectFrame);
	void SetShakeEffectFrame(int iShakeEffectFrame);

	bool m_bUsePetSkill;
	void SetEquipPetInfo(char cCharType, GCITEMUID dwUID);
	void AutoEquipPetItem();
	void EquipPetItem(DWORD dwItemID);
	void EquipPetItemByItemUID(GCITEMUID dwItemUID_);
	bool IsUseEquipPetItem(GCItem* itemInfo, int iPetID);
	// 해당 캐릭터의 펫을 장착해제 한다
	void UnEquipPet(char cCharType);
	// UID가 같은 펫을 모두 장착해제 한다
	void UnEquipPetforUID(GCITEMUID dwPetUID);
private:
	int m_iDownCount;
	DWORD m_dwDownBugTime;

public:
	void setCount(int _count) { m_iDownCount = _count; }
	void resetDownBugTime() { m_dwDownBugTime = -1; }

public:
	int GetOwnMotion(int Motion, bool bReverse = false);

	void AddBlurAlpha(float fAlpla);
	void EndBlur();

	bool m_bSuperArmor;
	bool m_bHyperArmor;
public:
	bool SuperArmor() const { return m_bSuperArmor; }
	void SuperArmor(bool val) { m_bSuperArmor = val; }

private:
	CParticleEventSeqPTR m_pFireLeftTip;
	CParticleEventSeqPTR m_pFireRightTip;
	KSafeArray<int, FORM_NUM> m_iMetamorphosisBuffTimeIdx;        // 변신체에 붙는 이펙트 지속시간이 몇번째 인자인지 저장하는 변수

public:
	CParticleEventSeqPTR FireLeftTip() const { return m_pFireLeftTip; }
	void FireLeftTip(CParticleEventSeqPTR val) { m_pFireLeftTip = val; }
	CParticleEventSeqPTR FireRightTip() const { return m_pFireRightTip; }
	void FireRightTip(CParticleEventSeqPTR val) { m_pFireRightTip = val; }

	void HandFireFrameMove();
	inline void HandFireShow(bool bSHow)
	{
		if (m_pFireRightTip)
			m_pFireRightTip->SetShow(bSHow);
		if (m_pFireLeftTip)
			m_pFireLeftTip->SetShow(bSHow);
	}

	DWORD GetCharTypeDword(int iCharType);

	GCObject* GetMetaForm(int iFormID) { ASSERT(-1 != iFormID); return m_pObjMetaForm[iFormID]; }
	GCObject* GetCurMetaForm(void) { ASSERT(-1 != MetamorphosisForm); return m_pObjMetaForm[MetamorphosisForm]; }

	int& GetMetaFormBuffTimeIdx(int iFormID) { ASSERT(-1 != iFormID); return m_iMetamorphosisBuffTimeIdx[iFormID]; }
	int& GetCurMetaFormBuffTimeIdx(void) { ASSERT(-1 != MetamorphosisForm); return m_iMetamorphosisBuffTimeIdx[MetamorphosisForm]; }

	// 2007/3/13. iridology. TagMatch용 변수들
private:
	std::vector< int >                  m_vecTagCharNum;
	std::map< int, TagCharacterInfo >   m_mapTagCharInfo;
	KProtectedType<DWORD>               m_dwPrevTagTime;

public:
	void TagMatchInit();
	inline std::vector< int > VecTagCharNum() const { return m_vecTagCharNum; }
	inline void VecTagCharNum(IN std::vector< int > val) { m_vecTagCharNum = val; }
	void SetFormTemplate(IN char CharNum, OUT KSafeArray<PLAYER_TEMPLATE, FORM_NUM>& FormTemplate);

	void ReleaseTagPlayerResource();
	void TagCharacter(IN bool bArena = false);
	void SetTagMotion();
	void UpdateTagCharInfo();
	int GetNextTagChar();
	TagCharacterInfo* GetNextTagCharInfo();
	void LoadResource();
	void InitPlayerValueforTagArena(IN bool bLoad);
	void LoadSlash();
	void TransformStartParticle(int iFormID);
	void TransformStartBuff(int iFormID);
	void TransformEndParticle(int iFormID);
	void SetTagSlotAlign();
	void MakeTagMatchLifeMap(OUT std::map< int, int >& mapLife);
	bool IsTagAvailable();
	bool IsTagMotion();
	std::map< char, GCEXPTYPE > GetCharacterExpMap();
	void EffectInit(TagCharacterInfo& tagInfo);
	std::vector< int >                  m_vecPetAiID;
	bool IsTagCharacterExist(int iCharNum_);

	std::string toTagStr();

	void InitBonusBuff(bool bLoading = false);
	void InitStarSignBoradBuff();
	std::map< int, TagCharacterInfo >& GetTagInfoMap() { return m_mapTagCharInfo; }

	bool m_bSuperMode;

	int GetExtraCharNum(int iCharType);

	void AddMotionParticle(int iFrame, CParticleEventSeqPTR pMotionParticle);
	void ClearMotionParticleList(void);
	void CheckMotionParticleList(void);

	// 칼 궤적 관련
private:
	bool m_bDrawSlashRight;
	bool m_bDrawSlashLeft;
	bool m_bDrawSlashLegRight;
	bool m_bDrawSlashLegLeft;
public:
	bool m_bNoCrashCheck;

private:
	// 해당모션이 끝날때까지만 뿌려질 파티클 정보를 담는다.
	std::map< int, std::vector<CParticleEventSeqPTR> > m_mapMotionParticle;

public:
	// 공격 속성 중에서 몬스터에게만 해당되는 것들
	EGCAttackType       m_eAttackType;
	EGCAttackDirection  m_eAttackDirection;
	float               m_fPushTargetX;
	float               m_fPushTargetY;
	bool                m_bSpecialAttack;

	void ShowLas4GreatSword(bool bRender)
	{
		for (int i = 0; i < (int)Las4GreatSword.size(); ++i) {
			if (Las4GreatSword[i].animInst != NULL)    //SetShow( bRender );에서 크래시가 나서 널포인터 체크 구문 추가도었음
				Las4GreatSword[i].animInst->SetShow(bRender);
		}
	}
	void SetWhiteCount(int iCount);
	DWORD GetElemental();

public:
	void GetSoundLoadListfromExtraCharNum(IN char cCharNum, OUT std::set<int>& setLoadList);
	void MakeSoundLoadList();
	void GetFireInfo3DObjectLoadListfromExtraCharNum(IN char cCharNum, OUT std::set<int>& setLoadList);
	void MakeFireInfo3DObjectLoadList();
	void RenderMergeWeapon(bool bRenderMergeWeapon);
	bool RenderMergeWeapon() const;
	void DrawSlashRight(bool bDrawSlash) { m_bDrawSlashRight = bDrawSlash; }
	void DrawSlashLeft(bool bDrawSlash) { m_bDrawSlashLeft = bDrawSlash; }
	bool DrawSlashRight() const { return m_bDrawSlashRight; }
	bool DrawSlashLeft() const { return m_bDrawSlashLeft; }
	void DrawSlashLegRight(bool bDrawSlash) { m_bDrawSlashLegRight = bDrawSlash; }
	void DrawSlashLegLeft(bool bDrawSlash) { m_bDrawSlashLegLeft = bDrawSlash; }
	bool DrawSlashLegRight() const { return m_bDrawSlashLegRight; }
	bool DrawSlashLegLeft() const { return m_bDrawSlashLegLeft; }

	virtual bool IsDie() { return m_ePlayerState == PS_DEAD; }

#if !defined( __PATH__ )
public:
	D3DXVECTOR3 m_vPos_inQuest;
#endif

#if !defined( __PATH__ )
public:
	int m_iTutorialPlayerAction;
#endif

public:
	// 2007.10.10 wony
	int m_iFreezeCount;         // 얼려진 모션이 지속된지 얼마나 됐는지 세는 변수
	float m_fFreezeDamage;      // 지속적으로 받는 데미지
	int m_iPlusFreezeCount;     // 좌우 키를 누르면 올라가는 카운트

	void ProcessFreezeMotion();

	DWORD m_dwBackAttackTime;
	DWORD m_dwAerialAttackTime;
	DWORD m_dwMissAttackTime;
	void BackAttack(void);
	void AerialAttack(void);

	int         m_iCannotCatchFrame;
	int         GetNoCatchFrame() const { return m_iCannotCatchFrame; }
	void        SetNoCatchFrame(int iCannotCatchFrame) { m_iCannotCatchFrame = iCannotCatchFrame; }

	bool        IsCatchableMotion(UINT uiMotion);

	bool        m_bDamageSetNoCheckContact;
	void        SetNoCheckContact(int iNoCheckContact) { NoCheckContact = iNoCheckContact; m_bDamageSetNoCheckContact = true; }

	// 필살기 쓸 때 Stop() 함수 호출 시 멈추지 않는 모션들 여기 넣어주기
	bool IgnoreStop_MotionList(int iPlayerIndex);

	void        SetEmoticon(KGCEmoticonInstance* pEmoticon);
	void        RenderEmoticon(float left, float top, float right, float bottom);

	KGCEmoticonInstance* GetEmoticon() { return m_pEmoticon; }

	int         m_iArenaCoolTime;
	int         m_iWeaponType;

private:
	float		m_fHP_RecoverVal;
	float		m_fMP_RecoverVal;

	KGCEmoticonInstance* m_pEmoticon;

public:
	bool IsPlusMotionX(UINT uiMotion);
	bool IsEffectOffCatchMotion(UINT uiMotion);

	bool IsSwapSpecialSkillMotion();
	void ProcessSwapWeapon(bool bNotChangeMotion = false);
	UINT GetSwapWeaponMotion(UINT iType, bool* bLatencyEqual);
	void SyncWeaponSwap(UINT uiChangeMotion_, bool bLatencyEqual = true, bool bNotChangeMotion = false);
	void SetSwapWeaponTemplate(IN char CharNum, OUT PLAYER_TEMPLATE& swapTemplate);
	bool CheckChangeSwap(void);

	PLAYER_TEMPLATE m_SwapTemplate;
	void ChangeWeaponTemplate();
	void SetOriginaSpecial(PLAYER_TEMPLATE& PlayerTemplate);
	bool IsFatal();
	bool IsDead() { return m_ePlayerState == PS_DEAD; }

	// 보는 향에 따라 모션을 변경시켜주는 함수
	UINT GetSwitchMotionByDirection(UINT uiMotion, bool bRight);

	struct LOADING_STATE
	{
		int m_iStep;
		int m_iDetailCursor;
		DWORD m_dwElapsedTime;

		std::set<int> m_setLoadList;

		std::vector<ITEM_EFFECT> m_vecItemEff;
		std::map< int, std::vector<std::pair< std::set<int>, std::set<int> > > > m_mapOverlappedPair;

		DWORD m_dwStartTime;

		LOADING_STATE()
		{
			Init();
		}
		void Init();
		void RefreshTime();
		bool IsTimeOver() const;
	};
	LOADING_STATE m_LoadingState;

	float m_fAutoHpRegenLimit;

	MAP_OVERLAPPED_PAIR m_mapOverlappedPair;

	// 론한2촤 전용
	UINT m_nForceNum;
	char m_cUseShield;
	void ChangeForceNum(bool IsIncrease);
	void InitForceNum(void);
	UINT GetForceNum(void) { return m_nForceNum; }
	void ConfirmUseSheild(char cUseSheild);

	bool NoForceEffect(EMIT_POSITION_TYPE positionType);
	bool IsReturnToWaitMotion();
	void SetMagicEffect(int iID, float ftime, int iLv = 0, bool bNoLocal_ = false);
	void SetMagicEffect_NotMeBroad(int iBuffIndex_, float fBuffTime_, int iBuffLevel_);
	void ClearMagicEffect(int iID, bool bNoLocal_ = false);
	void ClearMagicEffect_NotMeBroad(int iBuffIndex_);
	bool IsMagicEffect(int iID, bool CooltimeSkip = false);
	void HiddenBuffParticle(int iBuffIndex, bool bIsBufftimeStop, bool bIsHiddenParticle);

	std::vector< EGCSkillTree > m_setSkill;
	void PrepairSkill(char CharNum);
	bool CheckSkill(int iSkillID);
	bool IsLearnedSkill(int iSkillID);
	bool IsForceDoFrameInfoProcMotion(UINT uiMotion_, int iFrame_);

	std::map< std::string, std::vector<KGCAnimManager::ItemAnim> > m_mapPlayAnimation;
	void InitSkillAnimation(const std::vector< std::string >& vecAnim);
	void BeginAnim(char* strAnimName);
	void EndAnim(char* strAnimName);
	// 로난 3차 관련
	bool m_bChargeMonoVolt;
	D3DXVECTOR3 m_vecMonoVoltPos;
	KSafeArray<CParticleEventSeqPTR, 4> m_pMonoVolt;
	char GetMyCharString() { return Extra_Char_Num; }

	void SetChargeMonoVolt(bool isChargeMonoVolt = false);
	void InitMonoVolt();

	int m_iAbyssHandEff;
	D3DXVECTOR3 m_vecAbyssHandEffPos;
	KSafeArray<CParticleEventSeqPTR, 2> m_pAbyssHandEff;
	void SetAbyssHandEffect(int iAbyssHandEff = 0, TagCharacterInfo* pTagInfo = NULL);
	void InitAbyssHandEffect(TagCharacterInfo* pTagInfo = NULL);
	void InitAbyssHandEffectToScript();
public:
	bool IsLive();
	int GetCharType(void);

	// 법사 스킬 관련
	bool	m_bIsContinousDamage;
	bool	m_bIsChargedShield;
	bool	m_bIsCountAttackShield;
	float	m_fContinousDamage;
	float	m_fEffectDamage;
	float	m_fRegenRate;
	int		m_iShieldTime;
	int		m_iShieldLimitTime;
	int		m_iShockSturnCount;

	void 	ChangeHpToMp(float fHpPercent, float fMpPercent);
	void 	Change_Motion(int iMotion);
	void	DeleteMagicShield();
	void	SetChargedShield(bool bIsChargedShield);
	void	ShieldCountAttack(int iPlayerIndex, int iAttacker, float fDamage, bool bPetDamage = false, bool bMonsterDamage = false);
	void	ReflectionDamage(int iPlayerIndex, int iAttacker, float fDamage, bool bPetDamage = false, bool bMonsterDamage = false);
	void	ShieldTime();
	float	GetPlayerMaxHP() { return m_fAbility[ABILITY_HP]; }

	std::pair< std::wstring, D3DCOLOR > m_prPetNameWithColor;
	std::pair< std::wstring, D3DCOLOR > m_prPlayerNameWithColor;
	// 진 관련
public:
	KProtectedType< float > m_fBurnningPoint;	//BP
	float	m_fBurnningElapsedTime; //상황에 따라 BP를 계산하기 위해 시간을 저장하는 변수
	bool	m_bIsBurnningMode; //버닝모드인지
	bool	m_bIsMovedAfterCharge; //버닝차지모션일때 어떠한 움직임이 있었다면 true, 아니면 false를 저장.
	int		m_iBranchSkill; // 공격등에서 키입력에 따라 스킬이 분기될 때의 상태를 저장하기 위한 변수
	bool	m_bBurnningSpecial;  // 버닝필살기를 썼는가 ㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠㅠ

#ifdef DECANEE
	int		m_iDecaneeChargeSpeed;
	int		m_iDecaneeConsumeSpeed;
	void	IncreaseMarble();
#endif

	void	InitBurnningGauge();
	void	UpdateBP();
	void    AutoChargeBurnningPoint();
	void	DrawBurnningModeEffect();
	void	CountBP(float fBP, bool bRatio, bool bForceDecrease = false); // 버닝포인트 계산함수( 특수기등을 쓸 때 버닝포인트를 감소시킬 때 사용 )

	KSafeArray<PLAYER_TEMPLATE, BTYPE_NUM> m_BurnningTemplate;
	void SetBurnningTemplate(IN char CharNum, OUT KSafeArray<PLAYER_TEMPLATE, BTYPE_NUM>& m_BurnningTemplate);
	PLAYER_TEMPLATE& GetCurBurnningTemplate(void) { return m_BurnningTemplate[m_iWeaponType ? BTYPE_B : BTYPE_A]; }
	void ChangeBurnningTemplate();
	void SetNormalTpye(void);
	void ChangePromotionTemplate();

	void ReloadResource();
	void GetChangeWeaponItem(KItem& kChangeItem, bool bBaseItem = false);

	void InitPromotionTemplate();
	KSafeArray<CParticleEventSeqPTR, 2> m_pBurnningCharge;

	// 몬스터 소환관련
public:
	enum
	{
		BUFF_SUMMON_GAROUILLE = 289,
		BUFF_SUMMON_BASTION = 290,
	};
	float   m_fLimitDistance;
	void SendStateToMonster(EGCMonster monID, int iState);                                        // 소환된 몬스터에게 상태값 전달
	int GetStateToMonster(EGCMonster monID);
	std::vector<MONSTER*> GetMySummonMonster(EGCMonster monID);
	int GetSummonMonsterNum();                                                                      // 소환한 몬스터 갯수
	void SummonMonsterLimit(int iLimitCount, EGCMonster monID);                                   // 소환 몬스터 마릿수 제한
	void SummonMonster(EGCMonster monID, float fOffsetX, float fOffsetY, float fLimitDistance, int iLimitCount, float fHP = 10.0f, int fBuffIcon = 0, bool bNoPlayerPos = false);   // 몬스터를 소환한다.
	void SummonMonsterUseBuff(int iBuffIndex_, float fBuffTime_);
	void SummonDamageMonster(KSummonInfo* pSummonInfo, int iLevel, float fLimitDistance, int iLimitCount, int iHitPlayerIdx);
	void SummonEscortEventMonster(); // 이벤트 몬스터 소환
	void SetSummonMonsterHP(float fHPRadio, char cIndex = -1, bool bDieType = false, EGCMonster eMonID_ = MON_INVALID);             // 소환한 몬스터의 HP를 바꾼다.
	void SetSummonMonsterHP_SendHost(float fHPRadio, char cIndex = -1, bool bDieType = false, int iMonID_ = -1);    // 소환한 몬스터의 HP를 바꾼다.( 스크립트 바인딩용 함수 )
	void SetSummonMonsterLimitDist(float fDist) { m_fLimitDistance = fDist; }                     // 캐릭터와 몬스터 사이 제한거리를 셋팅한다.
	float GetSummonMonsterLimitDist() { return m_fLimitDistance; }                                  // 캐릭터와 몬스터 사이 제한거리를 얻는다.

public:
	//====================================================================================
	// 2009.02.03 : Jemitgge
	// Comment : 모션으로 y값을 바꾸었다면, 다음 모션도 이전 모션의 y값에 따른다.
	void	SetBeforeMotionPosY(void);
	CParticleEventSeqPTR m_pHideParticle;

	int m_iProcessedFrame;
	bool m_bMotionChanged;

	bool m_bUnstoppableMotion;
	bool m_bIgnoreStopMotion;
	bool m_bOffAllEffect;
	bool m_bNoUseSpecial;
	bool m_bIsCatchableMotion;

	int GetFrameNum();
	void SetFrameIndex(int iFrame_);
	int GetFrameIndex();

	bool m_bCheckGround;

	bool m_bUsePower;
	float m_fLastPower;

	void StartAfterImage();
	void EndAfterImage();

	float GetBurnningPoint() { return m_fBurnningPoint; }
	void SetBurnningPoint(float fBurningPoint) { m_fBurnningPoint = fBurningPoint; }
	void SendBurnningPoint(int iPlayerIndex = -1);

	int GetOldMotion() { return Old_Motion; }

	void SetUISuperPoint(float fSuper);
private:
	// Guild mark texture Name
	std::wstring m_strGuildMarkName;

public:
	void InitPlayerAbility();

	void SetPlayerAbility(IN const EAbilityData& stAbilityData_, bool bHPFull = true);
	void SetSpecialAbility(OUT EAbilityData& stAbilityData_);

	// 캐릭터 레벨 기본 능력치( 공방생 )를 가져온다. ( 능력치 밸런스 작업함. )
	void GetLevelAbilityMode(OUT EAbilityData& stAbilityData_);
	void GetLevelAbilityMode(int iLevel, int iCharType, OUT EAbilityData& stAbilityData_);

	// 캐릭터 레벨 기본 능력치( 공방생 ) 및 전직 능력치를 가져온다. - UI에서 사용하는 용도이다.
	void GetCharBaseAbility(OUT EAbilityData& stAbilityData_);

	// 현재 캐릭터 능력치 ( 기본 능력치 + 아이템 능력치 ) - UI에서 표시되는 능력치와 동일
	void GetCharAbility(OUT EAbilityData& stAbilityData_);

	// 캐릭터 종합 공격력 수치를 계산한다.
	float GetCharTotalAtk(const EAbilityData& stAbilityData_);

private:
	bool m_bIsComboState;

public:
	void SetComboState(bool bIsComboState_) { m_bIsComboState = bIsComboState_; }
	bool GetcomboState() { return m_bIsComboState; }
	void ComboBreak();

	void DrainHPMP(const int iDrainType = 0);

private:
	float m_fHittingPoint;
	bool m_bIsFullHittingState;
	bool m_bHGDecreaseState; // 히팅게이지가 감소상태인가??
	bool m_bMariSummonState; // 마력구슬 소환상태인지??

public:
	void InitHittingGauge();
	void UpdateHittingPoint();
	void CountHittingPoint(float fHittingPoint);
	void SetHGDecreaseState(bool bState) { m_bHGDecreaseState = bState; }

	bool GetHGDecreaseState() { return m_bHGDecreaseState; }
	bool GetHittingState() { return m_bIsFullHittingState; }
	float GetHittingPoint() { return m_fHittingPoint; }
	float GetHGPCumulativeNum() { return GetCurFormTemplate().fHGPCumulativeNum; }

	void SetMariSummonState(bool bSummonState) { m_bMariSummonState = bSummonState; }
	bool GetMariSummonState() { return m_bMariSummonState; }

private:
	bool m_bMeshToggleState;

public:
	void SetMeshToggleState(bool bToggle) { m_bMeshToggleState = bToggle; }
	bool GetMeshToggleState() { return m_bMeshToggleState; }
	void ToggleBaseMeshStatic(bool bToggle_);
	void RenderOffExtraAllWeapon();
	void ToggleRenderFace(bool bToggle_) { m_bIsRenderFace = bToggle_; }
	virtual bool IsToggleExtraMesh(const char* strID);
	virtual void ToggleMeshAll(bool bToggle_);
	virtual void ToggleExtraMesh(const char* strID, bool bToggle_, int iShaderTech = -1);
	virtual void ToggleExtraMeshAll(bool bToggle_);
	virtual void ToggleBaseMesh(bool bToggle_);
	virtual void ToggleSecondBaseMesh(const char* strID, bool bToggle_);
	virtual void ToggleSecondBaseMeshAll(bool bToggle_);
	virtual bool IsToggleSecondBaseMesh(const char* strID_);
	virtual void SetChildMeshMotion(char* strChlid, char* strMotion, bool bReserveNext, bool bForce, bool bResetFrame = false, bool bNotOneTime = false, int iLoopMode = 0);
	virtual void SetChildMeshMotion_Front(char* strChlid, char* strMotion);
	virtual void SetChildMeshFrame(char* strChlid, char* strMotion, int iFrame);
	virtual void SetChildMeshCurrentFrame(char* strChlid, int iFrame);
	virtual int GetChildMeshCurrentFrame(char* strChlid);
	virtual const char* GetChildMeshCurrentMotionName(char* strChild);
	virtual void ClearChildMeshOneTimeMotion(char* strChlid);
	D3DXVECTOR3                     vPos;

	virtual PositionDesc* GetPosDesc() { return &m_posDesc; }

	void SetPlayerHP(float fHP);

	std::vector< GCHotKeySkill > m_vecHotKeySkill;
	std::map< int, std::vector< GCHotKeySkill > > m_mapHotKeySkill; // 하이브리드 캐릭터 스킬들 담아놓는 컨테이너
	std::map< int, std::vector< KHotKeyCoolTimeInfo > > m_mapPrevHotKeyCoolTime;
	std::map< int, vector< GCHotKeySkill > > m_mapHotKeySkillBySlotType;

	bool IsDamagedMotion(int iMotion_ = -1);
	bool IsNoAllowForceUseMotion(int iMotion_ = -1); // 단축키 스킬 강제사용을 허용하지 않는 모션인지

	void SetStartMana();

	bool m_bSpecialSkill;

	static bool m_bAttackSwoosh;
	bool m_bNotTargeting;

	bool IsLiveCharacter();
	void HideNDisAbleKey(bool bToggle_);

	bool m_bSpecialShield;
	int m_iShieldTexNum;
	bool m_bShieldTypeCounting; // 카운팅 방식 쉴드 인지??

	MAP_HEROITEM m_mapHeroItemCondition;//m_vecHeroItemCondition

	void SetIncMP(float fIncMP);

	bool IsWeaponChanging() { return m_bWeaponChanging; }

	bool IsOnGameItemChange(void) { return m_bOngameItemChange; }
	void SetOnGameItemChange(bool bSet_) { m_bOngameItemChange = bSet_; }

private:
	GCImageIconTexture* m_pGuildMarkTexture;
	bool                m_bWeaponChanging;
	bool                m_bOngameItemChange;
public:
	std::map<int, std::pair<bool, std::pair<GCITEMID, GCITEMUID>>> m_mapSwapWeapon;

	std::vector< KGCEgoItemInfo > m_pObjectEgo;

	void EgoItemInitialize();
	void EgoItemFrameMove(USHORT ucFrame);
	void SetEgoItemOrderPlayer(int iPlayerID, DWORD dwItemID, int iModelID, int iConditionID);
	void ShowExpEffect(int iGhostIndex, float fExp_);

public:
	int                 m_iSuperArmorFrame;
	void SetSuperArmorFrame(int iFrame) { m_iSuperArmorFrame = iFrame; }

public:
	int                 m_iSelectTextrue;

public:
	D3DXVECTOR2  m_vTargetPos[MAX_TARGET_DAMAGE];
	D3DXVECTOR2* GetTargetPos(int iTargetNum, float fLimitDist = 0.0f, bool bCheckBoss_ = false, bool bCheckRight_ = false, bool bCheckLeft_ = false, float fLimitRangeY_ = 0.f);
	float GetPosFromNearBlock();

public:
	// 모든 공격을 HP가 아닌 MP를 깍는 공격으로 바꾸자!!,
	float                m_fDamageToMPAttack;       // 데미지 비율에 따른 MP공격
	float                m_fStaticToMPAttack;       // 고정 MP공격

public:
	float                m_fReflectionDamageRatio;   // 반사시킬 데미지 비율

public:
	float                m_fMPDecreaseRatio;            // 마나 감소량 비율
	float                m_fMPDecreaseRatioForSpecial;  // 필살기 마나 감소량 비율

public:
	float                m_fMinusAPCoolTime;            // AP 쿨타임 까주는 변수

public:
	BOOL SetDamageLife(int iDamageType, int iLife);// 데미지 시간조절
	int GetDamageLife(int iDamageTypeiLife);       // 데미지 시간얻기
	BOOL IsDamageLife(DAMAGE_TYPE eDamageTypeiLife);// 데미지 생존 유무

public:
	void SetCameraZoom(float fZoom);
	bool IsMonsterGameMode();
	bool IsFatalGameMode();
	bool SetWeaponChangeItem(int iCharType, bool bChange, KEquipItemInfo itemInfo, bool bInGame = false);
	void ClearWeaponChengeItem();
	void RemovePlayerObject();
	void ChangeEquipWeapon();
private:
	std::map<int, KEquipItemInfo> m_mapChangeWeaponItem;
	std::map<int, SUserItem> m_mapBaseChangeWeaponItem;

	// ★★★★★★★★★★★점령전★★★★★★★★★★★ //
private:
	int                 m_iDotaPoint;               // D-Point
	int                 m_iMaxDotaPoint;            // 최대D-Point
	int                 m_iDotaStage;               // 도타 스테이지 번호( 미니맵 처리에 필요함 )
	D3DXVECTOR2         m_vDotaArenaPos;            // 도타 아레나 위치
	SDotaStats          m_sDotaStats;               // 도타 통계정보

public:
	bool SetDirectMove(float fX, float fY, bool bNoPassPlayer = false, bool bNoPassMonster = false, float fOffsetX_ = 0.f, float fOffsetY_ = 0.f, bool bDirReverse_ = false);         // 캐릭터 고정위치이동 처리
	bool CheckDirectMove(float fX, float fY, bool bNoPassPlayer = false, bool bNoPassMonster = false);       // 전방에 장애물이 있는지 확인
	void SetDotaArenaPos(float fX, float fY);       // 아레나위치
	int GetDotaStage() { return m_iDotaStage; }
	void SetDotaStage(int iState);
	int Get_D_Point() { return m_iDotaPoint; }
	void Set_D_Point(int iPoint) { m_iDotaPoint = (iPoint > m_iMaxDotaPoint) ? m_iMaxDotaPoint : iPoint; }
	int Get_MAX_D_Point() { return m_iMaxDotaPoint; }
	void Set_MAX_D_Point(int iPoint) { m_iMaxDotaPoint = iPoint; }
	SDotaStats& GetDotaStats() { return m_sDotaStats; }

public:
	int GetWaitMotion();
	void SetWaitMotion();
	bool IsWaitMotion();
	int GetFatalWaitMotion();
	bool IsFatalWaitMotion();
	int GetWalkMotion();
	void SetWalkMotion();
	bool IsWalkMotion();
	bool IsRunMotion();
	bool IsDashMotion();
	bool IsWinMotion();
	bool IsLoseMotion();
	bool IsStartMotion();
	int GetJokeMotion();
	bool IsJokeMotion();
	void SendAgitWateringFairyTree();

public:// SetPlayerState()함수정리
	void CheckIcePixie();
	bool CheckOption(int iStateID);
	void SetStatsMID(int iStateID);
	void ChangeWeaponFromMotion();
	bool CheckNotDrawWeapon();
public:
	bool m_bIsAgitBreaking;

public:
	void FrameMove_EnchantWeaponEffect();
	void RenderEnchantWeaponEffect(int iLevel_);
	EGCEnchantColorGroup GetEnchnatColorGroup(int iCharType, int iEnchantLavel);

public:
	void SetInvenBuffEffect();
	KInvenBuffItem* GetInvenBuffItem(GCITEMID _itemID);

private:
	bool m_bUseNotAP;
	UINT m_uiNotSpecialSuper;

public:
	void SetUseNotAP(bool bUse) { m_bUseNotAP = bUse; }
	void SetNotSpecialSuper(UINT uiSuper) { m_uiNotSpecialSuper = uiSuper; }
	UINT GetNotSpecialSuper() { return m_uiNotSpecialSuper; }

public:
	void SetFatalAttriButeEffect();
	void ReSetFatalAttriButeEffect();

private:
	bool m_bWeaponRenderState;
	bool m_bMergeWeaponRenderState;

public:
	void SetWeaponRenderState(bool bRender_) { m_bWeaponRenderState = bRender_; }
	void SetMergeWeaponRenderState(bool bRender_) { m_bMergeWeaponRenderState = bRender_; }
	bool GetWeaponRenderState(void) { return m_bWeaponRenderState; }
	bool GetMergeWeaponRenderState(void) { return m_bMergeWeaponRenderState; }

private:
	bool    m_bResultTower;
	bool    m_bSetSelectMotion;
	std::vector<int>  m_vecCommonMotionList;
	void SetCommonMontion();
public:
	void SetResultTower(bool bSet) { m_bResultTower = bSet; };
	void SetSelectMotion(bool bSet) { m_bSetSelectMotion = bSet; };
	void SetAgitUserInfo(const KAgitUserInfo& kAgitUserInfo);
	void LoadAgitMotion();

private:
	bool                                m_bIsEquipCoordiSetItem;
	KCoordiSetTemplet                   m_kCoordiSetTemplet;

public:
	bool IsEquipCoordiSetItem() { return m_bIsEquipCoordiSetItem; }

public:
	void SendDungeonPlayState(int iState);

private:
	bool    m_bDecChangeWeapon;

public:
	bool GetDecChangeWeaponState(void) { return m_bDecChangeWeapon; }
	void SetDecChangeWeaponState(bool bSet_) { m_bDecChangeWeapon = bSet_; }

public:
	GCITEMUID GetSwapWeaponUID(void);
	int GetWeaponJobLevel();
	int GetWeaponID(int);
	void UnequipLookWeapon();
public:
	void ReloadBuff(void);

public:
	int m_iComboSkillHotKeyIdx;
	int m_iComboSkillComboIdx;
	int m_iComboSkillNowComboIdx;
	bool IsSetComboSkill();
	int GetCurrentComboIdx() { return m_iComboSkillNowComboIdx; }

public:
	void RenderEclipseBack();

private:
	CParticleEventSeqPTR m_pEclipseBack;

	// 서브 능력 수량포인트를 가지는 캐릭터 관련 것들
public:
	void ChargeQuantityPoint(); // 수량포인트 충전
	void SetOPChargeWeight(int iWeight_);
	void InitQuantityPoint();
	void CountQuantityPoint(int iAmount_);

	int GetMaxQuantityPoint();
	int GetQuantityPoint();
	void SetQuantityPoint(int iPoint_) { m_iQuantityPoint = iPoint_; }
	int GetQPChargeFrame() { return m_iQPChargeFrame; }

	void SetSpecialBuff(int iIndex_, int iType_ = SPECIAL_BUFF_NORMAL);
	void ClearSpecialBuff(int iIndex_);

private:
	int m_iQuantityPoint; // 수량포인트
	int m_iQPChargeFrame; // 수량포인트 1개의 충전을 체크하기 위한 누적 프레임

public:
	void CreateQuestionPuzzleKey(float fSafePosX_ = 0.f, float fOffsetY_ = 0.f);
	void InputSpecialSkillPuzzleKey();
	void ClearPuzzleyKeyInfo();
	bool IsPushValidPuzzleKey(int iKeyValue_, int iOrder_);
	bool CheckValidPuzzleKey();

	void CheckRageMode();

private:
	std::vector< int > m_vecInputPuzzleKey;
	std::vector< int > m_vecQuestionPuzzleKey;

public:
	void SetFadeInOut(int iRed_, int iGreen_, int iBlue, int iAlpha_ = 255, int iWeight_ = 0);
	void RenderFadeInOut();

private:
	SFadeInOutInfo m_sFadeInOutInfo;

public:
	bool IsShowDialogue();

public:
	void SetWindSpeed(float fWeightX = 1.0f, float fWeightY = 1.0f);

private:
	int m_iDamageSummonMonSlot;
public:
	int GetDamageSummonMonSlot() { return m_iDamageSummonMonSlot; }
	void SetDamageSummonMonSlot(int iSlot) { m_iDamageSummonMonSlot = iSlot; }

public:
	void SetLock(bool bLock);
	void SetMotionLock(bool bLock);

public:
	bool IsFallArenaGameMode();
	bool NotUseShortCutItem( bool bCheckGameMode = false );
	bool IsNotRebirth();
private:
	int m_iCurQuickSlotType; // 게임 중 현재 퀵슬롯타입을 저장한다.

public:
	void ChangeQuickSlotSkill();

private:
	SRangeCheckInfo m_sRangeCheckInfo;

public:
	SRangeCheckInfo GetRangeCheckInfo() { return m_sRangeCheckInfo; }
	void SetRangeCheckSize(float fRangeX_, float fRangeY_, bool bInit_ = false, int iTime_ = 0);
	void SetRangeBuffEffect(int iBuffIndex_, bool bDeBuff_ = false, bool bClearPrevBuff = false); // 범위안에 있을 시 받게되는 버프를 셋팅하는 함수.
	void CheckTargetInRange(); // 타겟이 범위 안에 있는지 체크하는 함수
	void RenderRangeCheckBoundingBox(); // 범위 랜더링 용 함수
	void InitRangeInfo();

private:
	float m_fCurMarbleNum; // 현재 충전된 마블 수.
	bool m_bCheckRunMarble;
	std::map< int, float > m_mapCurEquippedMarbleChargeMotion;
	std::map< int, std::pair< bool, float > > m_mapCurEquippedMarbleChargeDamage;
	std::map< int, std::pair< bool, float > > m_mapCurEquippedBurnChargeDamage;
	std::map< int, std::pair< bool, float > > m_mapCurEquippedBloodChargeDamage;

	int m_iChangeFrame;
	std::pair< int, float > m_prSelectedMarbleBuff;

public:
	void InitMarbleInfo();
	void InitCurEquippedMarbleChargeMotion();
	void InitCurEquippedBurnChargeMotion();
	void InitCurEquippedBloodChargeMotion();
	void CheckChargeMarblePhysicAtk();
	void CheckChargeMarbleDamageAtk(int iDamageIndex = -1, bool bCheckOnlyOne = false);
	void CheckChargeBurnDamageAtk(int iDamageIndex = -1, bool bCheckOnlyOne = false);
	void CheckBloodAtk(int iDamageIndex = -1, bool bCheckOnlyOne = false);
	void CheckChargeBloodDamageAtk(int iDamageIndex, bool bCheckOnlyOne = false);
	void DecreaseMarble();

	float GetCurMarbleNum() { return m_fCurMarbleNum; }
	void SetCurMarbleNum(float fNum_) { m_fCurMarbleNum = fNum_; }

#ifdef DECANEE
	bool GetIsMarbleRun() { return m_bCheckRunMarble; }
	int GetMaxMarbleNum() { return GetCurFormTemplate().fMaxMarbleAmount; }
#endif

	bool CheckRunMarbleMode();
	bool IsUsableMarbleState();

public:
	bool m_bNoSafePosition;

#if defined( USE_CAMERA_ZOOM_STATE )
	std::map<float, std::pair<DWORD, DWORD> >    m_mapCameraZoomState;   // <줌단계, <시작시간, 누적시간>>
	void AddCameraZoomState(float fBefore, float fAfter);
#endif

public:// 캐릭터 능력치 통계
	std::vector< KDungeonCharInfo > m_vecCharAbilityState;
	void CalcAbilityState();

public:
	void InitCombinationSkill();
	bool RunCombinationSkill(int iCombinationIdex_);
	void CheckCombinationSkill(int iMotion_);

public:
	std::pair< int, float > m_prReflectionDamage;
	void CreateReflectionDamage();

private:
	std::vector< GCHotKeySkillInfo > m_vecPossibleChargingSkillInfo;
	std::map< int, std::vector< GCHotKeySkillInfo > > m_mapPossibleChargingSkillInfo;
	//std::map< int, std::vector< GCHotKeySkillInfo > > m_mapPossibleChargingFromSkill;

public:
	bool UseSpecialForHybridCharacter(int& iSkillIndex_, int iShortCutLv_ = -1);

	void SetAllObjectBuff(int iBuffIndex_, float fBuffTime_, bool bDeBuff, int iLv = 0); // 게임상의 전체 적군이나 아군에게 버프 적용시켜 주는 함수

	void SetRenderWeather(int iWeatherIndex_, bool bRender_, DWORD dwTime_);

	int GetEncoachSoundSwitch(int iOriginalSound);

	int GetEncoachFaceSwitch(int iOriginalFaceType);
private:
	bool m_bLoadDownHelmetNFL;
	int m_iDefaultFaceLiftType;
	std::map< int, GCMeshObject* > m_mapFaceLiftMeshObject;
	GCMeshObject* m_pNonChangeFaceLift;
public:
	int GetDefaultFaceLiftType() { return m_iDefaultFaceLiftType; };

public:
	bool m_bEquipBigHead;
	bool m_bEquipFaceOffHead;
	std::map< int, std::pair<int, int> >   m_mapFaceLiftDelayInfo;  // map< FaceLiftType, pair< DelayCount, DelayTime >>
	void SetRenderFaceLift(int iFaceLiftType_);

private:
	std::map< std::pair< int, int >, std::vector< EGCSkillTree > > m_mapTutorialDefaultEquipSkillA;
	std::map< std::pair< int, int >, std::vector< EGCSkillTree > > m_mapTutorialDefaultEquipSkillB;
	std::map< std::pair< int, int >, std::vector< SCashSkillInfo > > m_mapCashSkillInfo;

public:
	bool IsTrainedSkill(EGCSkillTree eSkillID_);
	bool IsEquipSkill(EGCSkillTree eSkillID_);
	void ReSettingSpecial4Skill(int iChar, int iJob);

private:
	int m_iStopSpeedFrame;
public:
	void AllUnEquipItem(char cCharType);
	void AllUnEquipCoordiItem(char cCharType);

	void SetStopSpeedFrame(int iFrame) { m_iStopSpeedFrame = iFrame; }

	void SwapSlotSkillType(bool bIsSkillTypeB = false);

	// 진짜 각성 들가면 지워주자 꼭!!!!!!
	void CountSkillCoolTime();
	void SetSkillCoolTime(GCHotKeySkill* skill);
	void ChangeSkillCoolTime(int iSkillID_, float fCoolTime_);
	void UpdateChangedSkillSlot();
	//	void LoadPlayerTemplate_NewSkill( char Extra_Char_Num_ /*= -1*/ );
		// 여기까지
private:
	bool m_bNotCheckSideFH;
public:
	D3DXVECTOR2 GetSafePosition(float fX, float fY);
	void SetNotCheckSideFH(bool bCheck) { m_bNotCheckSideFH = bCheck; }
	bool GetNotCheckSideFH() { return m_bNotCheckSideFH; }

	void SetQPChangeFrame(int iQP_);
	void SetRageModeHpPercent(int iPercent_);

private:
	float m_fManaShieldRatio;

public:
	void SetManaShieldRatio(float fRatio_ = 2.f) { m_fManaShieldRatio = fRatio_; }
public:
	void ResetFaceLiftDelay();
private:
	int m_iSkipFrame;
	int m_iResetSkipFrame;
	int m_iContinueSkipFrame;
public:
	void SetSkipFrame(int iSkipFrame, int iContinueFrame) { m_iResetSkipFrame = iSkipFrame; m_iContinueSkipFrame = iContinueFrame; }
private:
	bool m_bIsRenderBaseMesh;
	bool m_bIsRenderBaseMeshStatic;
	bool m_bIsRenderFace;

public:
	bool IsRenderBaseMeshStatic() { return m_bIsRenderBaseMeshStatic; }

public:
	void ForcedClearBuffList();

public:
	bool MoveToNearTargetPos(float fLimitDist_, float fPosX_ = 0.f, float fPosY_ = 0.f, bool bCheckBoss_ = false, bool bCheckRight_ = false, bool bCheckLeft_ = false, bool bDirReverse_ = false, float fLimitRangeY_ = 0.f);

public:
	void SetMissileDamageResist(float fRatio_) { m_fAbility[ABILITY_MISSILE_DAMAGE_RESIST] = fRatio_; }

	void ClearSkillCoolTime();
private:
	bool m_bIsAttackMiss;

public:
	bool IsAttackMiss() { return m_bIsAttackMiss; }
	void SetAttackMiss(bool bMiss) { m_bIsAttackMiss = bMiss; }

public:
	bool IsPossibleCounterMotion(UINT Motion);
	DWORD GetPlayerUID() { return m_kUserInfo.dwUID; }

public:
	void SetCharOutLineColor(float r_, float g_, float b_);

public:
	int CountLiveTeamPlayer();

public:
	void SetOtherSpecialFaceTexture();

public:
	void SetMagicEffectOnlyLocal(int iID, float ftime, int iLv = 1);
	void SetMagicEffectRandom(int iID, float ftime, bool bEnemyBuff = false, bool bOverlap_ = false);
	void ClearAllPlayerMagicEffect(int iBuffID_, bool bLocal_ = false);
	void ClearMagicEffectByLocal(int iBuffID_);

public:
	int m_iEmbarkAdaptBoneID;
	DWORD m_dwMyEmbarkUID;
	DWORD m_dwEmbarkSendCool;

	void FrameMoveEmbarkation();
	void SetEmbarkUID(DWORD dwUID) { m_dwMyEmbarkUID = dwUID; }
	DWORD GetEmbarkUID() { return m_dwMyEmbarkUID; }
	void SetEmbarkAdaptBoneID(int iBoneID) { m_iEmbarkAdaptBoneID = iBoneID; }
	int GetEmbarkAdaptBoneID() { return m_iEmbarkAdaptBoneID; }
	void SetEmbarkSendCool(DWORD dwCool) { m_dwEmbarkSendCool = dwCool; }
	DWORD GetEmbarkSendCool() { return m_dwEmbarkSendCool; }
	bool IsEmbarkationKey();
	KGCEmbarkObject* GetMyEmbarkInstance();

private:
	std::vector<int>    m_vecPermissionDamage;
public:
	std::vector<int> GetPermissionDamage() { return m_vecPermissionDamage; };
	void ClearPermissionDamage() { m_vecPermissionDamage.clear(); };
	void SetPermissionDamage(const std::vector<int>& vecArg);

public:
	void ShowMissAttack(float fHitPosX, float fHitPosY);

public:
	bool IsEndGamePlay();
	void DirectKillPlayer();
	bool m_bDirectKill;

public:
	void UseGameItem_Super(int iFrame);

public:
	void SetOverlayTimer(bool bEnable, float fLimitedTime = 0.0f, bool bLocal_ = false);
	void SetInGameMessage(int iStringID, int iTime_);

public:
	bool m_bCheckDieSummons;
	int m_iOriginalTeam;

public:// 데미지가 이 플레이어에게 Crash되는 순간
	   // 데미지 생성위치와 생성시 생성자의 위치를 저장한다.
	D3DXVECTOR2 m_vDamageStartPos;
	D3DXVECTOR2 m_vDamageWhoPos;

public:// 탑승물을 탑승한 채 스테이지 이동 포탈 안에 들어가거나 나올때 사용될 녀석들( 탑승물 Type이 저장됨 )
	bool CheckInPortal();
	int GetEmbarkTypeOfInPortal() { return m_iEmbarkTypeOfInPortal; }
	void SetEmbarkTypeOfInPortal(int iValue) { m_iEmbarkTypeOfInPortal = iValue; }
	DWORD GetEmbarkUIDOfInPortal() { return m_dwEmbarkUIDOfInPortal; }
	void SetEmbarkUIDOfInPortal(DWORD dwValue) { m_dwEmbarkUIDOfInPortal = dwValue; }
	bool IsRemovePartsItem(REMOVEPARTS removeParts);
	REMOVEPARTS DontAppearParts(char charType);
private:
	int m_iEmbarkTypeOfInPortal;
	DWORD m_dwEmbarkUIDOfInPortal;

public:
	// 스킬 모션 사용횟수 통계 저장 컨테이너
	std::map< int, int > m_mapStatsSkillMotionList;

public:// 무조건 지크하트 광분모드로...( 아... 안좋은데..ㅠ 이런거 만들어서 죄송합니다. )
	void SetUnConditionToSiegFury();
private: // 데미지가 때린 한 녀석의 인덱스 구해오기 위한 것들
	int m_iHittedPrevDamage;
	int m_iPrevDamageHitWho;

public:
	void SetHittedPrevDamageInfo(int iHittedPrevDamage_, int iPrevDamageHitWho_);
	int GetPrevDamageHitWho(int iDamage_);

public:
	bool m_bNoCrashCheckByBuff;
	char m_cSelectedPromotion;

public:
	int GetEnemyAtkGuardMotion(int iMotion_);
	void SetEnemyAtkGuardMotion(int iReadyMotion_, int iPlayMotion_);

	void StartText(char* strText);

public:
	kEmbarkArena    m_kEmbarkArena;
	void SetEmbarkArena(kEmbarkArena* pInfo = NULL);
	bool IsSetEmbarkArena();
	void AddLife();
	void SetSelectedPromotion(char Extra_Char_Num);
	char PLAYER::GetSelectedPromotionValue();

public:
	CParticleEventSeqPTR m_pSqueDanger;

	void InitDangerSque();
	void CreateDangerSque();
	void DangerEffect();

	bool IsInLength(int iPlayerIndex_, float fLength_);
	bool m_bNoRecoveryMpMotion; // true이면 마나 회복 못하는 모션으로 체크함.

public:
	void SetFatigueRatio(float fRatio) { m_fFatigueRatio = fRatio; }
	float GetFatigueRatio() { return m_fFatigueRatio; }
private:
	float m_fFatigueRatio;

private:
	int m_iRegenCount;
public:
	int GetPlayerRegeonCount() { return m_iRegenCount; }
	void ClearRegenCount() { m_iRegenCount = 0; }
private:
	std::vector< CParticleEventSeqPTR > m_vecChildMeshParticle;
	bool m_bRenderChildMeshParticle;

public:
	CParticleEventSeqPTR AddParticleToChildMeshBone(char* strChlid, char* strSeqeunce, int iBoneIndex_, float fTraceTime = 0.5f, float offset_x = 0.0f, float offset_y = 0.0f, bool bNoDirection = false);
	void RenderChildMeshAllParticle(bool bRender_);
	void RenderChildMeshParticle(const char* strID, bool bRender_);
	bool IsRenderChildMeshParticle(const char* strID);
	bool IsLiveChildMeshParticle(const char* strID);
	bool SkillAloowMotion(int iCharType, UINT uiMotion_);

	bool m_bNoUseSkill; // 단축키, z차징 등 스킬을 못 쓰게 막는 값

public:
	bool CheckInputSavingSystem();
	void SetSavingGaugeAmount(float fAmount_);
	void SetSavingStateFunc();
	bool m_bSavingMotion;
public:
	std::pair< int, float > GetNextTransformPetBuff(void);       // 펫 변신 버프 풀릴 때 다음에 걸어줘야 할 버프 있으면 받는다.
	void ClearTransformPetBuffFull(void);                        // 펫 변신 버프 제거
	void SetNextTransformPetBuff(int iBuff_, float fTime_);      // 펫 변신 버프 풀리고 다음에 걸어줘야 할 버프 걸어준다.
	bool CheckPetTransformMotion(void);

public:
	// 미션조건 저장용
	int m_iControlInfoComboCount;               //최대콤보 횟수
	int m_iControlInfoBackAttackCount;          //백어택 횟수
	int m_iControlInfoBeAttackedCount;          //피격 횟수
	int m_iControlInfoRank;                     //랭크
	std::map<int, int> m_mapControlInfoSkill;   //체크할 스킬ID
	std::map<int, int> m_mapControlInfoPotion;  //체크할 포션그룹ID
public:
	void ClearControlInfo();
	void AddControlInfoBackAttack() { m_iControlInfoBackAttackCount++; }
	void AddControlInfoComboCount(int count_) { m_iControlInfoComboCount = (m_iControlInfoComboCount < count_) ? count_ : m_iControlInfoComboCount; }
	void AddControlInfoBeAttackedCount() { m_iControlInfoBeAttackedCount++; }
	void AddControlInfoPotionCount(int iType_);

	int GetControlInfoComboCount() { return m_iControlInfoComboCount; }
	int GetControlInfoBackAttackCount() { return m_iControlInfoBackAttackCount; }
	int GetControlInfoBeAttackedCount() { return m_iControlInfoBeAttackedCount; }
	std::map<int, int> GetControlInfoSkill() { return m_mapControlInfoSkill; }
	std::map<int, int> GetControlInfoPotion() { return m_mapControlInfoPotion; }
	int GetControlInfoSkillCount(int iType_);
	int GetControlInfoPotionCount(int iType_);
	void SetControlInfo();

public:
	void SetCartoon(int iCartoon_ = -1);

public:
	void AddDiceParticle();
};

extern const DWORD NextTagEnableTIme;

#endif // _PLAYER_H_
