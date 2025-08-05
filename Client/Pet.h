#ifndef _PET_H_
#define _PET_H_

#include "IGCComputePaletteP3M.h"
//#include "GCObject.h"
#include "./animation/KGCAnimManager.h"
//#include "UserPacket.h"
#include "Packet.h"
//#include "ProtectedType.h"

#if defined( NATION_EU )
#define PETNICKNAME 11
#elif defined(NATION_THAILAND) || defined(NATION_IDN) || defined(NATION_LATINAMERICA) || defined( NATION_USA ) || defined (NATION_BRAZIL)
#define PETNICKNAME 20
#elif defined(NATION_PHILIPPINE)
#define PETNICKNAME 21
#else
#define PETNICKNAME 10
#endif
#define MAXPETDELAY 12
enum DAMAGE_TYPE;

class GCObject;

namespace EGCPETID
{
    enum
    {
        INVALID = -1,
        LUNA = 0,
        BLUEFAIRY = 1,
        SEET = 2,
        PEPE = 3,
        PET_GON = 4,
        PET_NINKO = 5,
        PET_SLIM = 6,
        PET_EGG_SEET = 7,
        PET_EGG_PEPE = 8,
        PET_EGG_GON = 9,
        PET_EGG_SLIM = 10,
        PET_EGG_CUPID = 11,
        PET_MINGMING = 12,
        PET_CUPID = 13,
        PET_CUCA = 14,
        PET_PENGPENG = 15,
        PET_KELBY = 16,
        PET_DEVIL = 17,
        PET_LOVELY_BLUE = 18,
        PET_LOVELY_RED = 19,
        PET_TWINKY = 20,
        PET_CASANOVA = 23,
        PET_SAKU = 24,
        PET_MURING = 25,
        PET_PPIERO = 26,
        PET_GRIFFITH = 27,
        PET_BALCKANGEL = 28,
        PET_BUZZY = 29,
        PET_GAIKOZ = 30,
        PET_CRAKO = 31,
        PET_KAZEAZE = 32,
        PET_THANATOS = 33,
        PET_DK_MARK3 = 34,
        PET_KKAMBU = 35,
        PET_MASHIMARO = 36,
        PET_ELESIS = 37,
        PET_GWISHINEE = 38,
        PET_MERMAIRING = 39,
        PET_QUMING = 40,
        PET_COMMENDER = 41,
        PET_PIRO = 42,
        PET_FLAREBEAST = 43,
        PET_FRANCOISE = 44,
        PET_SEAMUS = 45,
        PET_KERORO = 46,
        PET_KITTY = 47,
        PET_EMILY = 48,
        PET_BURUBIS = 49,
        PET_ELENA = 50,
        PET_KUMI = 51,
        PET_BLACKBEAR = 52,
        PET_SARIMANOK = 53,
        PET_KUNGJI = 54,
        PET_ARIEL = 55,
        PET_RICKOON = 56,
        PET_SELION = 57,
        PET_HAETAE = 58,
        PET_SMALL_MAID = 59,
        PET_TOMO = 60,
        PET_KELLY = 61,
        PET_MONKEY = 62,
        PET_CATBOT = 63,
        PET_TLEK = 64,
        PET_HULAGIRL = 65,
        PET_NEWFAIRY = 66,
        PET_NEWNINKO = 67,
        PET_COLOR_LUNA = 68,
        PET_COLOR_BLUEFAIRY = 69,
        PET_NEW_LUNA = 70,
        PET_NEW_MINGMING = 71,
        PET_COLOR_PENGPENG = 72,
        PET_NIX = 73,
        PET_MARYJAIN = 74,
        PET_ALICE = 75,
        PET_TONNYDOG = 76,
        PET_HOUND = 77,
        PET_GHOST = 78,
        PET_MOONBUNNY = 79,
        PET_MEDUSA = 80,
        PET_DUEL = 81,
        PET_ZIDLER = 82,
        PET_LESLEY = 83,
        PET_CLARA = 84,
        PET_JRDARKPRIEST = 85,
        PET_BLACKDRAGON = 86,
        PET_JRJACK = 87,
        PET_ARON = 88,
        PET_KUNGJI_ELDER = 89,
        PET_VICTOR = 90,
        PET_DARKAMON = 91,
        PET_ASTAROT = 92,
        PET_LUCY = 93,
        PET_RENCIA = 94,
        PET_RICA = 95,
        PET_PERIOT = 96,
        PET_ZORO = 97,
        PET_CHUNRANG = 98,
        PET_RURU = 99,
        PET_CORDELIA = 100,
        PET_Blitzchen = 101,
        PET_BAKUNAWA = 102,
        PET_KETER = 103,
        PET_BLITCHEN_RED = 104,
        PET_BLITCHEN_GREEN = 105,
        PET_BLITCHEN_BLUE = 106,
        PET_BLITCHEN_YELLOW = 107,
        PET_BLITCHEN_BLACK = 108,
        PET_GWEN = 109,
        PET_ORDO = 110,
        PET_CHRISTMAS_PENGPENG = 111,
        PET_PLUBELL = 112,
        PET_CHOUCRING = 113,
        PET_MINI_RURU = 114,
        PET_MINI_RURU_PINK = 115,
        PET_MINI_RURU_GREEN = 116,
        PET_MINI_RURU_BLACK = 117,
        PET_MINI_RURU_RED = 118,
        PET_ELESIS_YELLOW = 119,
        PET_ELESIS_PURPLE = 120,
        PET_ELESIS_BLACK = 121,
        PET_ANYMOMO = 122,
        PET_KOG_MARK_1 = 123,
        PET_KOG_MARK_2 = 124,
        PET_KOG_MARK_3 = 125,
        PET_KOG_B = 126,
        PET_EDNA = 127,
        PET_DARKNIGHTMARE = 128,
        PET_DOKKAEBI = 129,
        PET_ICEDRAGON = 130,
        PET_TRICKY = 131,
        PET_LEONA = 132,
        PET_CHRISTMAS_MARYJAIN = 133,
        PET_CHRISTMAS_ALICE = 134,
        PET_CHRISTMAS_LESLEY = 135,
        PET_MIRACLE_MERMAIRING = 136,
        PET_FOX_NINE_TAILS = 137,
        PET_LITTLE_FOX_NINE_TAILS = 138,
        PET_CHUNRYEONGA = 139,
        PET_LUI = 140,
        PET_WORLDCUP_TOMO = 141,
        PET_DEVILION = 142,
        PET_BLACKSLIME = 143,
        PET_ASLAN = 144,
        PET_BERKAS = 145,
        PET_RAMIEL = 146,
        PET_DARKSPIRIT = 147,
        PET_LUKE = 148,
        PET_DEATH = 149,
        PET_NERAID = 150,
        PET_PENEK = 151,
        PET_DANA = 152,
        PET_DARKGRIFFIN = 153,
        NUM,
    };
};

namespace EGCPETMOTION
{
    enum
    {
        INVALID = -1,
        WAIT = 0,
        MOVE = 1,
        JUMP_READY = 2,
        JUMP_UP = 3,
        JUMP_DOWN = 4,
        START = 5,
        WIN = 6,
        LOSE = 7,
        ATTACK = 8,
        MAGIC = 9,
        TELEPORT_READY = 10,
        TELEPORT_ARRIVE = 11,
        ROOM = 12,
        SHOP = 13,
        JUMP_LAND = 14,
        GAME_LOADING = 15,
        SHOP_ACTION = 16,
        MOVE_END = 17,
        ATTACK_MIRROR = 18,
        JOKE_MIRROR = 19,
        DAMAGE_MIRROR = 20,
        BORING = 21,
        NUM,
    };
};

namespace EGCPETLOCATION
{
    enum
    {
        INVALID = -1,
        ROOM = 0,
        GAME = 1,
        SHOP = 2,
        GAME_LOADING = 3,
        NUM,
    };
};

struct SParticleInfo
{
    std::string strParticleName;	//파티클 이름

    float		fPosX;				//초기좌표
    float		fPosY;
    float		fPosZ;

    float		fCorrPosRX;			//펫이 오른쪽을 볼 때 보정 좌표
    float		fCorrPosRY;

    float		fCorrPosLX;			//펫이 왼쪽을 볼 때 보정 좌표
    float		fCorrPosLY;
};

class KPet;
struct TagPetInfo
{
    BOOL            m_bNeedDelete;
    int             m_iCharType;
    int             m_iPetID;
    int             m_iLevel;
    int             m_iEvolutionLevel;
    // 펫 모델
    //std::vector< GCObject* > m_vecPetModel;
    GCObject* m_pObject;
    bool            m_bEnableUseSkill;
    DWORD           m_dwSkillGoodsID;
    float           m_fCurrentMP;

    WCHAR           m_strPetName[PETNICKNAME];

    float           m_fScale;
    float           m_fDistX;
    float           m_fDistY;
    float           m_fNameHeight;

    float			m_fAtkRatio;

    KSimpleItem     m_kCostume;         // 펫 코스튬

    // 펫 모션
    std::map<int, GCDeviceMotionFRM*>                                       m_mapMotion;
    std::map<std::string, SParticleInfo >		                            m_mActionParticle;
    std::map<std::string, CParticleEventSeqPTR>                              m_pParticleEventSequence;
    std::map<std::string, CParticleEventSeqPTR>                              m_pParticleEventSequence2;
    std::map<std::string, std::pair<D3DXVECTOR2, D3DXVECTOR2> >		        m_mParticlePos;
    TagPetInfo()
    {
        m_pObject = NULL;
        m_fAtkRatio = 1.0f;
        m_iCharType = GC_CHAR_ELESIS;
        m_iPetID = 0;
        m_iLevel = 0;
        m_iEvolutionLevel = 0;
        m_bEnableUseSkill = false;
        m_fCurrentMP = 0.f;
        m_fScale = 1.f;
        m_fDistX = 0.f;
        m_fDistY = 0.f;
        m_fNameHeight = 0.f;
        m_kCostume.m_dwID = KItem::INVALID_ITEM;
        m_kCostume.m_dwUID = KItem::INVALID_ITEM;
        m_kCostume.m_cItemType = GCIT_PERIOD;
    }
    void Init();
    void InitTagPetData(KPet* pPet);
    void ShowEffect(bool bShow);
    std::string toStr();
    int GetPetMotionID();
};

class PLAYER;
class GCSquarePeople;
struct KPlayerRenderData;

struct KPetOwnerInfo
{
    KPlayerRenderData OwerPlayer;
    KRenderData* pOwnerRenderData;
    bool bIsContact;
    bool bRender;

    KPetOwnerInfo(PLAYER* pPlayer);
    KPetOwnerInfo(GCSquarePeople* pPeople);

    void Update(PLAYER* pPlayer);
    void Update(GCSquarePeople* pPeople);

    ~KPetOwnerInfo() {}
};

class KPet : public KRenderData
{
public:
    KPet();
    ~KPet();

    KPet& operator =(const KPet& right_);
    // 해킹 방지를 위해서 KProtectedType 으로 설정해 줘야 할 것들은 무엇인가..

    // 펫 주인의 인덱스
    int m_iPlayerNumber;
    // 자기가 어떤 펫인지..
    int m_iPetID;
    // 펫의 레벨
    int m_iLevel;
    // 진화레벨
    int m_iEvolutionLevel;
    // 현재 프레임
    //KProtectedType< int > m_iFrame;
    //int m_iFrame;
    int m_iResultFrame;
    // 현재 모션
    //KProtectedType< int > m_iMotion;
    //int m_iMotion;

    // 현재 펫의 위치와 정보들..
    //D3DXVECTOR2 m_vPos;
    D3DXVECTOR2 m_vPrePos;
    D3DXVECTOR2 m_vSlotPos;
    //bool m_bIsRight;
    bool m_bIsContact;

    float m_fMotionX;

    // 플레이어의 움직임을 담아놓고 펫은 그대로 따라가도록 한다
    KSafeArray<D3DXVECTOR2, MAXPETDELAY> m_avPos;
    KSafeArray<bool, MAXPETDELAY> m_abIsRight;
    KSafeArray<bool, MAXPETDELAY> m_abContact;

    // 점프상태인지
    bool m_bIsJump;
    // 텔레포트 하기전
    bool m_bIsTeleport;

    // 아래 변수가 true 이면 어떤동작 완료 후 wait 모션일 때 win 또는 lose 모션으로 만들어준다
    bool m_bIsWin;
    bool m_bIsLose;

    // 스타트 모션좀 보쟈 ㅠ 
    bool m_bStart;

    // 크기
    float m_fScale;
    // 플레이어로부터 떨어진 거리
    float m_fDistX;
    // 땅에서부터 떠있는 높이
    float m_fDistY;
    // 펫 이름의 높이
    float m_fNameHeight;

    int m_iRenderDelay;

    int m_bExistMirrorMotion;

    // 펫 모델
    //std::vector< GCObject* > m_vecPetModel;
    GCObject* m_pObject;
    // 펫 모션
    std::map<int, GCDeviceMotionFRM*> m_mapMotion;

    // 그림자의 y좌표
    float m_fShadowY;

    // bFrameMove 는 디폴트 false 이고.. 프레임을 1씩 증가시켜 줄것이면 true 로 하면 됩니다
    void SetMatrixPalette(int iMeshIndex, D3DXMATRIX& matBasic, bool bFrameMove = false, bool bEndLock = false);

    int GetMeshSize();

    static void BindLua();

    void StartPet(int iPlayerNumber, int iPetType, int iLevel, int iEvolutionLevel, int iLocation, std::wstring strPetName = L"", bool bAddToRenderMgr_ = false, DWORD dwCostumeID = 0, bool bUseShopTexture_ = false);
    void SetBasicInfo(int iPlayerNumber, int iPetType, int iLevel, int iEvolutionLevel, int iLocation, std::wstring strPetName = L"", bool bAddToRenderMgr_ = false);
    void UpdateResource();
    void InitBeforeGameStart();
    void EndPet();

    void CreateParticleEffect();
    void ClearParticleEffect();

    void FrameMove();

    void FrameMove_Motion();
    void FrameInfoProc_Common(UINT uiMotion);
    void FrameInfoProc(UINT uiMotion);
    void RenderShadow();
    void RenderPetName();

    bool IsHallow();

    void FrameMoveEffect();

    // PETDELAY 만큼의 프레임동안의 플레이어의 움직임의 정보를
    // 저장해서 이후에 펫을 움직이는 데 사용한다.
    void PlayerTrace();

    // 플레이어의 옆에 계속 붙어서 움직이기 때문에 발판끝부분에
    // 서 있는 경우 펫이 공중에 뜬것 처럼 보입니다
    // 이 경우 적절하게 보정해 줍니다.. 아래 함수에서..
    // 이름은 check지만 체크만 하지 않고 보정까지 한번에=ㅅ=
    void CheckPetContact(int index, bool bIsLocalPlayer);

    // 펫 모션 변경
    void SetPetMotion(int iMotion);

    // 스킬 사용 관련
    void UseSkill();
    void UseTransformSkill();
    bool m_bEnableUseSkill;
    DWORD m_dwSkillGoodsID;

    // TODO: MP는 프로텍티드 타입으로 해줘야 할것임
    //KProtectedType< float > m_fIncMP;
    float m_fIncMP;
    //KProtectedType< float > m_fCurrentMP;
    float m_fCurrentMP;

    // 공격 모션일 때
    void AddDamage(int eDamage, float fX, float fY, bool bPetTrace = false, bool bPlayerTrace = false);
    void AddDamageWithAngle(int eDamage, float fX, float fY, float fStartAngle, bool bMoveAngle = false);
    void AddActionParticle(char* strParticle_);
    void AddBuff(int iPlayerIndex_, int iBuffindex, float fbuffTime_);

    // 펫템플릿으로부터 정보 읽어오는 함수
    static void AddPetTemplate(int iPetID_, int iEvolutionLevel_);
    static void LoadPetTemplate();
    static void LoadPetCostume();
    static void LoadPetSkillInfo();

    std::map<std::string, CParticleEventSeqPTR> m_pParticleEventSequence;
    std::map<std::string, CParticleEventSeqPTR> m_pParticleEventSequence2;
    std::map<std::string, std::pair<D3DXVECTOR2, D3DXVECTOR2> >		m_mParticlePos;
    std::map<std::string, SParticleInfo >		m_mActionParticle;

    void LoadPetEffect();

    D3DXVECTOR3 m_vTracePos;
    D3DXVECTOR3 m_vTraceCenterPos;

    std::vector<D3DXVECTOR3> m_vBoneTracePos;

    CParticleEventSeqPTR AddParticle(char* strSeqeunce, float fDirPosX, int iParticleNum, bool bReverseTex = true, float fOffsetX = 0.f, float fOffsetY = 0.f);
    CParticleEventSeqPTR AddParticleWithTrace(const char* strSeqeunce, float fOffsetX, int iParticleNum, bool bReverseTex = true);
    CParticleEventSeqPTR AddParticleXOffset(char* strSeqeunce, int iParticleNum, float fOffsetX);
    CParticleEventSeqPTR AddParticleNoDirectionPosWithTrace(char* strSeqeunce, int iParticleNum, float fOffsetX = 0.f, float fOffsetY = 0.f, float fTraceTime = 0.f, bool bTracePet_ = false);

    WCHAR m_strPetName[PETNICKNAME];

    std::string GetMotionStr()
    {
        std::string str;
#define caseStateToString(state) case state: str = #state; break;
        switch (usMotion)
        {
            caseStateToString(EGCPETMOTION::INVALID);
            caseStateToString(EGCPETMOTION::WAIT);
            caseStateToString(EGCPETMOTION::MOVE);
            caseStateToString(EGCPETMOTION::JUMP_READY);
            caseStateToString(EGCPETMOTION::JUMP_UP);
            caseStateToString(EGCPETMOTION::JUMP_DOWN);
            caseStateToString(EGCPETMOTION::START);
            caseStateToString(EGCPETMOTION::WIN);
            caseStateToString(EGCPETMOTION::LOSE);
            caseStateToString(EGCPETMOTION::ATTACK);
            caseStateToString(EGCPETMOTION::MAGIC);
            caseStateToString(EGCPETMOTION::TELEPORT_READY);
            caseStateToString(EGCPETMOTION::TELEPORT_ARRIVE);
            caseStateToString(EGCPETMOTION::ROOM);
            caseStateToString(EGCPETMOTION::SHOP);
            caseStateToString(EGCPETMOTION::JUMP_LAND);
            caseStateToString(EGCPETMOTION::GAME_LOADING);
        default:
            str = "MotionErr";
            break;
        }
#undef caseStateToString
        return str;
    }

public:
    int GetPetMotionID();
    int GetPetNameID();
    int GetPetDesc(int iDescState_);
    float GetPetResultScale();
    D3DXVECTOR3 GetPetAxisCorrection();
    GCDeviceMotionFRM* GetPetMotionFrm(int iMotion_);
    bool IsItemEquip(int iItemKind_ = -1, int iGoodsID_ = -1);	//펫이 어떤 어떤 장비를 장착하고 있는지 확인할 때 사용합니다.

    //스크립트에서 사용되는 함수 입니다.
    D3DXVECTOR2* GetPos() {
        static D3DXVECTOR2 vRet;
        vRet = (D3DXVECTOR2)vPos;
        return &vRet;
    }
    D3DXVECTOR2* GetDelayPos() { return &m_avPos[0]; }
    D3DXVECTOR2* GetPrePos() { return &m_vPrePos; }
    int	GetNumFrame(int iMotion_);
    float FloatAbs(float fNum_) { return ASMfabs(fNum_); }

    void GetIncMPFromLua();
    void SetIncMP(float fIncMP_);

public:
    void PlaySound(int iSoundNum, unsigned char chDelay);

protected:
    bool IsEnableMotionToUseSkill();

private:
    // CharType, TagPetInfo
    std::map< int, TagPetInfo > m_mapTagPet;
    int m_iRemainSpecial;
    bool m_bPetWaitForTag;

public:
    bool IsPetWaitForTag() { return m_bPetWaitForTag; }

public:
    void clear();
    void InitTagPetData(IN int iPidx, IN int iCharType);
    void AddTagPetInfo(IN int iPidx, IN int iCharType);
    void StartPet(IN int iPidx, OUT TagPetInfo& tagPetInfo, int iLocation, bool bUSeShopTexture = false);

    void LoadPetActionMotion(int iPetID, int iPromotion, GCITEMID skillID = 0, std::map<int, GCDeviceMotionFRM*>* pmapMotion = NULL);
    void EndPet(OUT TagPetInfo& tagPetInfo);
    void CreateParticleEffect(OUT TagPetInfo& tagPetInfo);
    void ClearParticleEffect(OUT TagPetInfo& tagPetInfo);
    void LoadPetEffect(OUT TagPetInfo& tagPetInfo);
    void ReleaseTagPetResource();
    void TagPet(IN int iBeforeChar, IN int iCharType);
    inline int  GetRemainSpecial() { return m_iRemainSpecial; }
    inline std::wstring GetRemainSpecialStr()
    {
        std::wstringstream stm;
        stm << m_iRemainSpecial;
        return stm.str();
    }
    void CalcRemainSpecial(IN KPetInfo& kPet);
    void ShowEffect(IN bool bShow);
    bool IsPetAvailable(IN int iPidx, IN int iCharType);

    std::string toStr();
    std::string toTagStr();

    float GetPetScaleFromPetLevel(int iLevel);
    bool IsUseMotion(int iLocation, int iMotion);
    bool IsLocalPet();

    KPetOwnerInfo* m_pOwner;
    void UpdateMasterInfo();
    void CreateObject(bool bAddMgr);
    //void LoadPetModel( int iLocation, DWORD dwCostumeID, GCObject* pObject_ = NULL, int iPetID = -1, int iEvolutionLevel = -1 );
    void LoadPetModel(bool bUseShopTexture, DWORD dwCostumeID, GCObject* pObject_ = NULL, int iPetID = -1, int iEvolutionLevel = -1);
    void LoadPetMotion(int iLocation, int iPetID, int iEvolutionLevel, std::map<int, GCDeviceMotionFRM*>* pmapMotion = NULL);
    void LoadPetSingleMotion(int iMotionIndex, const char* strMotion, std::map<int, GCDeviceMotionFRM*>* pmapMotion = NULL);
    void SetOwnerInfo(int iPlayerNumber);
    void SetBaseInformation(int iPetType, int iLevel, int iEvolutionLevel, std::wstring strPetName);
    bool IsAvailableMotion(int iMotion_);
    void SetMotionFrm(int iMotion_, GCDeviceMotionFRM* pFrm_, std::map<int, GCDeviceMotionFRM*>* pmapMotion = NULL);

    static bool IsPetAvailableState();

private:
    void LoadPetSound(int iPetID_, int iPetEvolution_);

private:
    DWORD m_dwCostumeID;
public:
    void SetCostumeID(DWORD dwCostumeID) { m_dwCostumeID = dwCostumeID; };
    DWORD GetCostumeID() const { return m_dwCostumeID; };

private:
    float	m_fAtkRatio;
public:
    void	SetAtkRatio(float fAtkRatio_) { m_fAtkRatio = fAtkRatio_; }
    float	GetAtkRatio() { return m_fAtkRatio; }

    void StartAfterImage();
    void EndAfterImage();
    void OnReverseGravity(float fRangeX, float fRangeY, float fPosX, float fPosY, float fDamage, bool bFlyCheck, float fHeight);

    bool SetMp(float fMp);
    float GetMaxMP();

private:
    bool m_bRenderShadow;
    bool m_bRenderName;

public:
    void ToggleMesh(bool bToggle_);
    void TogglePetRender(bool bToggle_);
    bool IsRenderOnPet(void);
    void EnableGlyphEffect( bool bAttack = true );

};

#endif // _PET_H_
