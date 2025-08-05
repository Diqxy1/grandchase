#ifndef _GCGLOBAL_H_
#define _GCGLOBAL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Packet.h"
#include "ServerType.h"
#include "GCLuaManager/lua_tinker.h"
#include "MapSystem/GCFootHold.h"
#include "GCUserMission.h"
#include "GCUserCollectionMission.h"
#include "NewPatcher/MailSMTPHeader.h"
#include "NewPatcher/MailMIMEHeader.h"
#include "gcranking.h"
#include <atltime.h>
#include "bandicap.h"
#include "PlayerComp.h"
#include "GCExtraMesh.h"
#include "GCHeroDungeonManager.h"
#include "GCEventAlarmManager.h"
#include "GCUI/GCShopAdvertising.h"
#include "GCGlobalFunction.h"

extern void CheckFileSizeAndSHA( KENU_VERIFY_ACCOUNT_REQ* kReq_, std::wstring strFileName_, std::wstring strFilePath_ );
// 던전 퍼포먼스 통계
struct kDungeonPerformanceState
{
    int iLoadingStarttingMember;
    int iGameStarttingMember;
    int iPlayMember;
    int iNormalityLaveCnt;

    int iLoadingBannedCnt;                               // 로딩스타팅 멤버기준   :   로딩튕김 카운트
    int iPlayBannedCnt;                                  // 게임스타팅 멤버기준   :   게임 중 튕김 카운트
    DWORD dwLoadingTime;                                 // 로딩스타팅 멤버기준   :   로딩시간
    std::map<int, std::pair<DWORD, double>>   mapAvgFPS; // 플레이 멤버기준       :   평균FPS

    kDungeonPerformanceState()
    {
        Init();
    }

    void Init()
    {
        iLoadingStarttingMember = 0;
        iGameStarttingMember = 0;
        iPlayMember = 0;
        iNormalityLaveCnt = 0;

        iLoadingBannedCnt = 0;
        iPlayBannedCnt = 0;
        dwLoadingTime = 0;
        mapAvgFPS.clear();
    }

    void AddFPSState( int iPlayMember, float fFPS )
    {
        if( 0.0f == fFPS )
        {
            return;
        }

        this->iPlayMember = iPlayMember;
        std::map<int, std::pair<DWORD, double>>::iterator mapIter = mapAvgFPS.find( iPlayMember );
        if( mapIter == mapAvgFPS.end() )
            mapAvgFPS.insert( std::make_pair(iPlayMember, std::make_pair(0, 0.0)) );

        if( INT_MAX == (mapAvgFPS[iPlayMember].first + 1) || INT_MAX <= (mapAvgFPS[iPlayMember].second + fFPS) )
        {
            float fAvg = static_cast<float>(mapAvgFPS[iPlayMember].second) / static_cast<float>(mapAvgFPS[iPlayMember].first);
            mapAvgFPS[iPlayMember].first = 1;
            mapAvgFPS[iPlayMember].second = fAvg;
        }

        ++mapAvgFPS[iPlayMember].first;
        mapAvgFPS[iPlayMember].second += fFPS;
    }

    int GetPlayBannedCnt( int iPlayMember )
    {
        // 게임을 같이 시작한 멤버에서 정상종료한 멤버를 빼고 현재 남은 멤버를 빼면 튕긴사람이 나오겠군.!
        int iBannedCnt = ((iGameStarttingMember - iNormalityLaveCnt) - iPlayMember);
        return ((iBannedCnt < 0) ? 0 : iBannedCnt);
    }

    std::map<int, float> GetAverageFPS()
    {
        std::map<int, float> mapFPS;
        std::map<int, std::pair<DWORD, double>>::iterator mapIter = mapAvgFPS.find( iPlayMember );
        for(; mapIter != mapAvgFPS.end(); ++mapIter)
        {
            mapFPS.insert( std::make_pair(mapIter->first, (static_cast<float>(mapIter->second.second)/static_cast<float>(mapIter->second.first))) );
        }
        return mapFPS;
    }
};

struct KMantisReportParm
{
    std::wstring wstrProject;
    std::wstring wstrCategory;
    std::wstring wstrServerityEng;
    std::wstring wstrSummary;
    std::wstring wstrDesc;
    std::wstring wstrID;
    std::wstring wstrPasswd;

    void Init() {
        wstrProject = L"";
        wstrCategory = L"";
        wstrServerityEng = L"";
        wstrSummary = L"";
        wstrDesc = L"";
        wstrID = L"";
        wstrPasswd = L"";
    }
};

struct KCoordiSetTemplet
{
    int                 iSetNum;
    std::vector<int>    vecParts;
    std::string         strSequence;
    bool                bAddEffect;
    int                 iMotionID;

    void Init()
    {
        iSetNum = -1;
        vecParts.clear();
        strSequence = "";
        bAddEffect = false;
        iMotionID = MID_COMMON_STONECURSE;
    }
};

struct MonsterFootHoldInfo
{
    MonsterFootHoldInfo()
    {
        Init();
    }
    int m_iBaseBone;
    int m_iEndBone;
    DWORD m_dwProp;

    float m_fRelativeX;
    float m_fRelativeY;

    void Init()
    {
        m_iBaseBone = 0;
        m_iEndBone = 0;
        m_dwProp = 0;
        m_fRelativeX = 0.0f;
        m_fRelativeY = 0.0f;
    }
};


// 몬스터 궤적 정보
struct SwooshInfo
{
    SwooshInfo()
    {
        Init();
    }

    int         iBaseBone;
    int         iEndBone;
    std::string strTex;
    float       fTu1, fTu2, fTv1, fTv2;
    void Init()
    {
        iBaseBone = -1;
        iEndBone = -1;
        strTex.clear();
        fTu1 = 0.0f;
        fTu2 = 1.0f;
        fTv1 = 0.0f;
        fTv2 = 1.0f;
    }
};


// 몬스터들이 사용하는 rect들
struct PushRectInfo
{
    PushRectInfo()
    {
        Init();
    }
    int iBoneId;
    float fWidth;
    float fHeight;

    float fOffsetX;
    float fOffsetY;

    void Init()
    {
        iBoneId = 0;
        fWidth = 0.0f;
        fHeight = 0.0f;

        fOffsetX = 0.0f;
        fOffsetY = 0.0f;
    }

};

struct AttackRectInfo
{
    AttackRectInfo()
    {
        Init();
    }
    int iBoneId;
    float fWidth;
    float fHeight;
    float fDelta;
	float OffsetX;
	float OffsetY;

    void Init()
    {
        iBoneId = 0;
        fWidth = 0.0f;
        fHeight = 0.0f;
        fDelta = 0.0f;
		OffsetX = 0.0f;
		OffsetY = 0.0f;
    }
};


class KGCContectInfo
{
private:
    KGCContectInfo()
    {
        m_x = 0.0f;
        m_y = 0.0f;
        m_xSpeed = 0.0f;
        m_ySpeed = 0.0f;
        m_cCheck = 0;
        m_bTagMotion = false;
        m_bPlayer = true;;
        m_HeadY = 0.0f;
        m_fHeight = 0.0f;
        m_iBlockX = 0;
        m_iBlockY = 0;
    }

public:
    float m_x;
    float m_y;
    float m_xSpeed;
    float m_ySpeed;
    unsigned char m_cCheck;
    bool m_bTagMotion;
    bool m_bPlayer;
    float m_HeadY;
    float m_fHeight;
    int m_iBlockX;
    int m_iBlockY;
    bool m_bNotCheckSideFH;

    KGCContectInfo( float x_, float y_, float xSpeed_, float ySpeed_, unsigned char cCheck, bool bTagMotion, bool bPlayer_ = true, float fHeadY = 0.0f, float fHeight = 0.0f )
        : m_x( x_ )
        , m_y( y_ )
        , m_xSpeed( xSpeed_ )
        , m_ySpeed( ySpeed_ )
        , m_cCheck( cCheck )
        , m_bTagMotion( bTagMotion )
        , m_bPlayer( bPlayer_ )
        , m_HeadY( fHeadY )
        , m_fHeight( fHeight )
        , m_bNotCheckSideFH( false )
    {
        m_iBlockX = 0;
        m_iBlockY = 0;
    }
    ~KGCContectInfo() {}
};

struct STraceIcon
{
    bool bIsMyExistScreen;      //내가 화면안에 없을때 표시할것이냐??(TRUE) 전체 몬스터 중 한마리도 없을때만 표시할것이냐??(FALSE)
    bool bIsNonCheckInScreen;   //이 몬스터는 InScreen체크를 하지마라.( 무조건 화면 밖에 있다고 판단함 )
    std::string strSqu_1;      //화살표 중앙에 들어갈 파티클
    std::string strSqu_2;      //화살표 파티클

    STraceIcon()
    {
        Init();
    };

    void Init()
    {
        bIsMyExistScreen = false;
        bIsNonCheckInScreen = false;
        strSqu_1.clear();   
        strSqu_2.clear();
    };
};

enum 
{
    EM_MONSTER_MIN_LV = 0,
    EM_MONSTER_MAX_LV = 200,
};

struct SMonsterTemplate
{
    SMonsterTemplate()
    {
        Init();
    }
    int             iMonsterID;
    std::string     strAIFileName;
    std::string     strInitFunc;
    int             iBaseLevel;
    float           fExp;
    float           fGP;
    float           fAttackPoint;
    float           fSummonsAtkRatio;
    float           fHPPoint;
    float           fSkExp;
    int             iNameID;
    int             iNameBone;
    float           fNameOffset_X;
    float           fNameOffset_Y;
    float           fScale;
    float           fWeight;
    float           fAttackBoundBoxWidth;
    float           fAttackBoundBoxHeight;
    float           fWidth;
    float           fHeight;
    float           fWalkSpeed;
    float           fRunSpeed;
    int             eNameSize;
    float           fShadowRatio;
    int             eNPC;
    float           fVolume;
    bool            bNoDamageReaction;
    float           fArrowDefence;
    int				iTechnique;
    int				iLayer;
    std::vector< EXTRA_MESH > vecMeshInfo;
    std::vector< EXTRA_MESH > vecMeshDeadInfo;
    std::vector< int > m_vecSounds;
    std::vector< SwooshInfo > vecSwooshInfo;
    std::vector< PushRectInfo > vecPushRectInfo;
    std::vector< AttackRectInfo > vecAttackRectInfo;
    std::vector< MonsterFootHoldInfo > vecFootHoldInfo;
    std::vector< int > vecBuffList;
    float           fZPos;
    bool			m_bAutoStat;
    float           fDefence;
    float           fSDefence;
    int             iCenterBone;
    int             iHitHP;
    bool            bNameRenderSkip;
    bool            bHPrenderSkip;
    bool            bMonsterTarget;
    bool            bNotMissionCount;
    bool            bNotBackAttack;
    bool            bNoPushCheck;
    bool            bNoPassMonster;
    bool            bNotGuidedTarget;
    bool            bNotCheckStageWall;
    bool            bNoCrashDamage;
    bool            bNoRealMiniMap;
    bool            bNoChampion;
    bool            bNoCheckStageRank;
    bool            bCrashParticlePosToAttackBox;
    bool            bNoCrashToIncMP;
    bool            bNoPushCheckToDamage;
    bool            bNoFlashEffect;
    bool			bNoPetPlusDelay;
    bool			bNoBuff;
    bool			bNoDamageBuff;
    int             iCharType;
    float			fHeadPosY;
    float			fHeadPosX;
    STraceIcon      sTraceIcon;
    void Init()
    {
        iMonsterID = 0;
        strAIFileName = "";
        strInitFunc = "";
        iBaseLevel = 0;
        fHeadPosY = -1.0f;
        fHeadPosX = 0.0f;
        fExp = 0.0f;
        fGP = 0.0f;
        fAttackPoint = 0.0f;
        fSummonsAtkRatio = 0.f;
        fHPPoint = 0;
        fSkExp = 0.0f;
        iNameID = 0;
        iNameBone = -1;
        fNameOffset_X = 0.0f;
        fNameOffset_Y = 0.0f;
        fScale = 1.0f;
        fWeight = 1.0f;
        fWalkSpeed = 0.01f;
        fRunSpeed = 0.01f;
        fAttackBoundBoxWidth = 0.5f;
        fAttackBoundBoxHeight = 0.5f;
        fWidth = 0.5f;
        fHeight = 0.5f;
        eNameSize = 0;
        fShadowRatio = 1.0f;
        eNPC = 0;
        fVolume = 0.5f;
        vecMeshInfo.clear();
        m_vecSounds.clear();
        vecMeshDeadInfo.clear();
        vecSwooshInfo.clear();
        vecPushRectInfo.clear();
        vecAttackRectInfo.clear();
        vecFootHoldInfo.clear();
        vecBuffList.clear();
        bNoDamageReaction = false;
        fArrowDefence = 0.f;
        fZPos = 2.5f;
        iTechnique = -1;
        iLayer = GC_LAYER_CHAR;
        m_bAutoStat = false;
        fDefence = 0.0f;
        fSDefence = 0.0f;
        iCenterBone = 0;
        iHitHP = 0;
        bNameRenderSkip = false;
        bHPrenderSkip = false;
        bMonsterTarget = true;
        bNotMissionCount = false;
        bNotBackAttack = false;
        bNoPushCheck = false;
        bNoPassMonster = false;
        bNotGuidedTarget = false;
        bNotCheckStageWall = false;
        bNoCrashDamage = false;
        bNoRealMiniMap = false;
        bNoCheckStageRank = false;
        bNoChampion = false;
        bCrashParticlePosToAttackBox = false;
        bNoCrashToIncMP = false;
        bNoPushCheckToDamage = false;
        bNoFlashEffect = false;
        sTraceIcon.Init();
        iCharType = GC_CHAR_INVALID;
    }
};

class LevelTable
{
public:
    float Exp;
    float SkExp;
    float GP;
    float Atk;
    float Vit;

    LevelTable()
    {
        Init();
    }

    void Init()
    {
        Exp = 0.0f;
        SkExp = 0.0f;
        GP = 0.0f;
        Atk = 0.0f;
        Vit = 0.0f;
    }

    void SetData( float exp_, float skExp_, float gp_, float atk_, float vit_ )
    {
        Exp = exp_;
        SkExp = skExp_;
        GP = gp_;
        Atk = atk_;
        Vit = vit_;
    }

    LevelTable& LevelTable::operator = ( const LevelTable& rhs )
    {
        Exp = rhs.Exp;
        SkExp = rhs.SkExp;
        GP = rhs.GP;
        Atk = rhs.Atk;
        Vit = rhs.Vit;

        return *this;
    }
};

class MonsterWeightTable
{
public:
    USHORT MonID;
    float Exp;
    float SkExp;
    float GP;
    float Atk;
    float Vit;
    float Def;
    float SDef;

    MonsterWeightTable()
    {
        Init();
    }

    void Init()
    {
        MonID = 0;
        Exp = 0.0f;
        SkExp = 0.0f;
        GP = 0.0f;
        Atk = 0.0f;
        Vit = 0.0f;
        Def = 0.0f;
        SDef = 0.0f;
    }

    void SetData( float exp_, float skExp_, float gp_, float atk_, float vit_, float def_, float sDef_ )
    {
        Exp = exp_;
        SkExp = skExp_;
        GP = gp_;
        Atk = atk_;
        Vit = vit_;
        Def = def_;
        SDef = sDef_;
    }

    MonsterWeightTable& MonsterWeightTable::operator = ( const MonsterWeightTable& rhs )
    {
        Exp = rhs.Exp;
        SkExp = rhs.SkExp;
        GP = rhs.GP;
        Atk = rhs.Atk;
        Vit = rhs.Vit;
        Def = rhs.Def;
        SDef = rhs.SDef;

        return *this;
    }
};

// 경험치 Table
struct sLevelReward
{
    int     m_nExp;
    int     m_nSP;
    int     m_nGP;
};

// 몬스터 별 경험치 가중치.
struct sMonAddRatio
{
    float m_fExpRatio;
    float m_fSPRatio;
    float m_fGPRatio;
};

struct PetMeshInfo
{
	PetMeshInfo()
	{
		Init();
	}

	std::string strMeshName;
	std::string strTextureName;
	std::string strShopTextureName;
	int         iCartoonTexture;
    int         iTechnique;

	void Init()
	{
		strMeshName     = "";
		strTextureName  = "";
		strShopTextureName = "";
		iCartoonTexture = -1;
        iTechnique = -1;
	}
};

struct SPetMent {
    int iTime;
    int iStringID;

    SPetMent( int iTime_, int iStringID_ ) {
        iTime = iTime_;
        iStringID = iStringID_;
    }
};

struct SPetEffectInfo {
    std::string     m_strEffectName;
    D3DXVECTOR3     m_vCreatePos;
    D3DXVECTOR2     m_vOffsetPos[2];

    SPetEffectInfo( const char* strEffectName_, float x_, float y_, float z_, float rx_, float ry_, float lx_, float ly_ ) {
        m_strEffectName = strEffectName_;
        m_vCreatePos = D3DXVECTOR3( x_, y_, z_ );
        m_vOffsetPos[1] = D3DXVECTOR2( rx_, ry_ );
        m_vOffsetPos[0] = D3DXVECTOR2( lx_, ly_ );
    }
};

struct SPetBoneEffectInfo {
    std::string     m_strEffectName;
    int             m_iBoneID;
    D3DXVECTOR2     m_vOffsetPos[2];
    float           m_fTraceTime;

    SPetBoneEffectInfo( const char* strEffectName_, int iBoneID_, float rx_, float ry_, float lx_, float ly_, float traceTime_ ) {
        m_strEffectName = strEffectName_;
        m_iBoneID = iBoneID_;
        m_vOffsetPos[1] = D3DXVECTOR2( rx_, ry_ );
        m_vOffsetPos[0] = D3DXVECTOR2( lx_, ly_ );
        m_fTraceTime = traceTime_;
    }
};

// 펫 정보
struct SPetTemplate
{
    int     iPetID;
    int     iPromotion;
    DWORD dwGoodsID;

    float fScale;
    float fDistX;
    float fDistY;

    float fNameHeight;

    int iNameID;
    std::map<int, int> mapDescID;

    float fResultScale;

	std::vector< PetMeshInfo > vecMeshInfo;
    std::vector< int > vecSound;
	std::vector< DWORD > vecCostumeList;
    std::map<int, std::vector<SPetMent> > mapMentList;
    std::vector< SPetEffectInfo > vecEffect;
    std::vector< SPetEffectInfo > vecActionEffect;
    std::vector< SPetEffectInfo > vecRenderOffEffect;
    std::vector< SPetEffectInfo > vecTransformEndEffect;
    std::vector< SPetBoneEffectInfo > vecBoneEffect;

    int iMaxEvolutionLevel;

    int iItemDescID;

	float fAtkConstant;
	float fDefConstant;
	float fHPConstant;

	float fEvolutionConst;

	float fAtkDefault;
	float fDefDefault;
	float fHPDefault;

    GCITEMID SkillGP;
    GCITEMID SkillCash;
	GCITEMID SkillAwakeningGP;
	GCITEMID SkillAwakeningCash;

	bool bExistMirrorMotion;
	int iTechnique;

    int iTransformEndSound;

	int GetAttack( int iLevel_, int iEvolutionLevel_ ) const;
	int GetDefence( int iLevel_, int iEvolutionLevel_ ) const;
	int GetHP( int iLevel_, int iEvolutionLevel_ ) const;
    int GetDescID( int iState_ ) const;
	float GetPetEvolutionConst( int iEvolutionLevel_ ) const	{ return fEvolutionConst;}
    void PushMeshInfo( const char* strModel_, const char* strTexture_, const char* strTextureHigh_, int iCartoon_, int iTechnique_ ) {
        PetMeshInfo sMesh;
        sMesh.strMeshName = strModel_;
        sMesh.strTextureName = strTexture_;
        sMesh.strShopTextureName = strTextureHigh_;
        sMesh.iCartoonTexture = iCartoon_;
        sMesh.iTechnique = iTechnique_;
        vecMeshInfo.push_back( sMesh );
    }
    void PushSoundInfo( int iSound ) {
        vecSound.push_back( iSound );
    }
    void PushCostume( GCITEMID ItemID_ ) {
        vecCostumeList.push_back( ItemID_ );
    }
    void PushEffect( const char* strEffectName_, float x_, float y_, float z_, float rx_, float ry_, float lx_, float ly_ ) {
        vecEffect.push_back( SPetEffectInfo( strEffectName_, x_, y_, z_, rx_, ry_, lx_, ly_ ) );
    }
    void PushActionEffect( const char* strEffectName_, float x_, float y_, float rx_, float ry_, float lx_, float ly_ ) {
        vecActionEffect.push_back( SPetEffectInfo( strEffectName_, x_, y_, 0.5f, rx_, ry_, lx_, ly_ ) );
    }
    void PushRenderOffEffect( const char* strEffectName_, float x_, float y_, float rx_, float ry_, float lx_, float ly_ ) {
        vecRenderOffEffect.push_back( SPetEffectInfo( strEffectName_, x_, y_, 0.5f, rx_, ry_, lx_, ly_ ) );
    }
    void PushTransformEndEffect( const char* strEffectName_, float x_, float y_, float rx_, float ry_, float lx_, float ly_ ) {
        vecTransformEndEffect.push_back( SPetEffectInfo( strEffectName_, x_, y_, 0.5f, rx_, ry_, lx_, ly_ ) );
    }
    void PushBoneEffect( const char* strEffectName_, int iBoneID_, float rx_, float ry_, float lx_, float ly_, float traceTime_ ) {
        vecBoneEffect.push_back( SPetBoneEffectInfo( strEffectName_, iBoneID_, rx_, ry_, lx_, ly_, traceTime_ ) );
    }
    bool IsAvailableCostume( GCITEMID ItemID_ );

    void AddDesc( int iState_, int iStringID_ );
    void AddMent( int iState_, int iTime_, int iStringID_ );
    void SetAxisCorrection( float x_, float y_, float z_ ) { vAxisCorrection = D3DXVECTOR3(x_,y_,z_); };
    void SetAxisCorrectionX( float x_ ) { vAxisCorrection.x = x_; }
    void SetAxisCorrectionY( float y_ ) { vAxisCorrection.y = y_; }
    void SetAxisCorrectionZ( float z_ ) { vAxisCorrection.z = z_; }

    static void BindToLua( lua_State* L );

    D3DXVECTOR3 vAxisCorrection;
	SPetTemplate()
	{
        iPetID = -1;
        iPromotion = -1;
		dwGoodsID = 0;
		fScale = 1.0f;
		fDistX = 0.0f;
		fDistY = 0.0f;
		fNameHeight = 0.0f;
		iNameID = 0;
		fResultScale = 1.0f;
		iMaxEvolutionLevel = 0;
		iItemDescID = 0;
		iTechnique = -1;
        iTransformEndSound = -1;

        SkillGP = -1;
        SkillCash = -1;
		SkillAwakeningGP = -1;
		SkillAwakeningCash = -1;

        fAtkConstant = 0.002f;
        fDefConstant = 0.001f;
        fHPConstant = 0.001f;
        fEvolutionConst = 1.f;

        fAtkDefault = 0.2f;
        fDefDefault = 0.2f;
        fHPDefault = 0.2f;

		bExistMirrorMotion = false;
	}

    void GetPetSkillInfo( std::vector<GCITEMID>& vecSkillInfo )
    {
        vecSkillInfo.clear();
        vecSkillInfo.push_back( SkillGP );
        vecSkillInfo.push_back( SkillCash );
        vecSkillInfo.push_back( SkillAwakeningGP );
        vecSkillInfo.push_back( SkillAwakeningCash );
    }
};

// 서버 정보 구조체
class SServerInfo : public lua_tinker::lua_value
{
public:
    KDeclareLuaInvoke
    DWORD           dwServerType;
    DWORD           dwServerIP;         //서버의 IP
    USHORT          usServerPort;       //서버의 Port
    USHORT          usUdpPort;          //서버의 UDP 포트번호
    DWORD           dwUserUID;          //서버에서 지정해 준 각 유저마다의 Unique ID.
    DWORD           dwPublicIP;         //서버에서 본 Client IP Address
    std::wstring    strServerName;
public:
    SServerInfo()
    {
        dwServerType            = 0;
        dwServerIP              = 0;
        usServerPort            = 0;
        usUdpPort               = 0;
        dwUserUID               = 0;
        dwPublicIP              = 0;
        strServerName           = L"";
    }
    virtual ~SServerInfo()
    {

    }
    bool CheckServerType( ServerType eServerType_ );
    void SetServerType( ServerType eServerType_ )
    {
        dwServerType |= eServerType_;
    }
    void EraseServerType( ServerType eServerType_ )
    {
        DWORD dwMax = ST_FORCE_DWORD;
        dwMax ^= eServerType_; 
        dwServerType &= dwMax;
    }
};

struct SLoginInfo
{
    std::string strID;
    std::string strPass;
	std::string strSpareParam;
    int         iSessionInfo;
    bool        bMale;
    bool        bExpAccount;    //체험 계정
	bool		bNewUser;
    bool        bGuideAzit;
	KProtectedType<int>      nFunBoxBonus;
    KProtectedType<int>      nLinBonus;
};

struct SClipBoardInfo
{
	std::string strID;
	std::string strPass;
	std::string strDataCookie;
	std::string strSpareParam;
};


struct MonsterMotionError
{
    bool bErrorOccured;
    int iMotion;
    int iFrame;

    MonsterMotionError()
    {
        bErrorOccured = false;
        iMotion = 0;
        iFrame = 0;
    }
};

struct MonsterFrmError
{
    bool bErrorOccured;
    std::string strWaitMotion;
    std::string strMotion;
    bool bIsDie;

    MonsterFrmError()
    {
        bErrorOccured = false;
        strWaitMotion = "";
        strMotion = "";
        bIsDie = false;
    }
};


// 채팅 이벤트관련 클래스 - 방안의 유져 모두가 일정 단어를 입력 하면 발동하는 이벤트
class KGCChattingEvent
{
private:
    int                         m_iChattingEventUID;
    std::vector<std::wstring>   m_vecChattiingEventMsg; // 이 벡터안의 단어가 입력되면 발동!!
    bool                        m_bSuccessChattingEvent;

public:
    KGCChattingEvent(){ ClearData(); }
    void SetData( int iEventUID, std::vector<std::wstring> vecEventMsg )
    {
        m_iChattingEventUID     = iEventUID;
        m_vecChattiingEventMsg  = vecEventMsg;
        m_bSuccessChattingEvent = false;
    }
    void ClearData()
    {
        m_iChattingEventUID     = -1;
        m_vecChattiingEventMsg.clear();
        m_bSuccessChattingEvent = false;
    }
    bool CheckChatting( std::wstring strText )
    {
        if( m_iChattingEventUID <= 0 )
            return false;

        int iVecSize = (int)m_vecChattiingEventMsg.size();
        for( int i = 0; i < iVecSize; ++i )
        {
            if( strText.find( m_vecChattiingEventMsg[i] ) != std::wstring::npos )
                return true;
        }
        return false;
    }
    int GetUID(){ return m_iChattingEventUID; }
    bool GetSuccessEvent(){ return m_bSuccessChattingEvent; }
    void SetSuccessEvent( bool bSuccess ){ m_bSuccessChattingEvent = bSuccess; }
};

struct SMonsterHuntMonsterInfo
{
    int             iMonsterType;
    int             iPoint;
    int             iRegenTime;
};

struct SMonsterHuntInfo
{
    SMonsterHuntInfo()
    {
        Init();
    }

    int             iLevel;
    std::string     strMap;
    int             iTotalTime;
    int             iClearPoint;
    int             iArenaPoint;

    std::vector< SMonsterHuntMonsterInfo > vecMonsterInfo;

    void Init()
    {
        iLevel          = 0;
        strMap          = "";
        iTotalTime      = 100;
        iClearPoint     = 100;
        iArenaPoint     = -5;

        vecMonsterInfo.clear();
    }
};

// 몬스터 원정대 정보를 담고 있는 struct
struct SMonsterCrusaderInfo
{
    SMonsterCrusaderInfo()
    {
        Init();
    }

    std::string     strMap;
    int             iMonsterID;
    int             iTextureIndex;
    float           fExpRatio;
    float           fGPRatio;

    void Init()
    {
        // 엔트로 초기화 해놓겠음..
        strMap          = "TrialForest_Stage_03.lua";
        iMonsterID      = MON_ENT;
        iTextureIndex   = 0;
        fExpRatio       = 0.05f;
        fGPRatio        = 0.1f;
    }
};

typedef         std::map<int,SPetTemplate>      KPetTemplateContainer;
typedef			std::map<int,std::vector<std::wstring>> KPVPLodingImageFileName;
// 전직별 능력치 보너스 
struct PromotionBonusStatus
{
    PromotionBonusStatus()
    {

        m_iPromotion1Status = 100;
        m_iPromotion2Status = 200;
        m_iPromotion3Status = 300;

        m_fPromotion2Critical = 0.01f;
        m_fPromotion3Critical = 0.02f;
    }

    int m_iPromotion1Status;
    int m_iPromotion2Status;
    int m_iPromotion3Status;

    float m_fPromotion2Critical;
    float m_fPromotion3Critical;
};

// 점령전 : 몬스터 포탈 정보를 담고 있는 struct
struct SDotaMonsterPortalInfo
{
    SDotaMonsterPortalInfo()
    {
        Init();
    }

    int         iPortalID;
    int         iTeam;
    D3DXVECTOR2 vSize;
    D3DXVECTOR2 vMovePos;

    void Init()
    {
        iPortalID = -1;
        iTeam = -1;
        vSize = D3DXVECTOR2(0.0f, 0.0f);
        vMovePos = D3DXVECTOR2(0.0f, 0.0f);
    }
};

// 점령전 : 게이트 정보를 담고 있는 struct
struct SDotaGateInfo
{
    SDotaGateInfo()
    {
        Init();
    }

    int              iGateID;
    int              iSummonLimit;
    int              iSummonCoolTime;
    int              iTeam;
    int              iHP;
    bool             bIsRight;
    std::vector<int> vecMonsterType;

    void Init()
    {
        iGateID             = -1;
        iSummonLimit        = -1;
        iSummonCoolTime     = 0;
        iTeam               = -1;
        iHP                 = 0;
        bIsRight            = false;
        vecMonsterType.clear();
    }
};

// 점령전 : 고정형 몬스터 정보를 담고 있는 struct
struct SDotaStaticMonsterInfo
{
    SDotaStaticMonsterInfo()
    {
        Init();
    }

    int iMonsterID;
    int iSlotID;
    int iTeam;
    int iHP;
    bool bIsRight;
    D3DXVECTOR2 vPos;

    void Init()
    {
        iMonsterID = -1;
        iSlotID = -1;
        iTeam = -1;
        iHP = 0,
        bIsRight = false;
        vPos = D3DXVECTOR2(0.0f, 0.0f);
    }
};

// 점령전 : 디폴트 D-Point 지급 정보를 담고 있는 struct
struct SDotaDefault_D_Point
{
    SDotaDefault_D_Point()
    {
        Init();
    }

    int iCoolTime;
    int iDPoint;

    void Init()
    {
        iCoolTime = 0;
        iDPoint = 0;
    }
};

// 점령전 : 코어 몬스터의 D-Point 정보를 담고 있는 struct
struct SDotaCore_D_Point
{
    SDotaCore_D_Point()
    {
        Init();
    }

    float fHpRadio;
    int iCoolTime;
    int iDPoint;

    void Init()
    {
        fHpRadio = 0.0f;
        iCoolTime = 0;
        iDPoint = 0;
    }
};

// 점령전 : D-Point 아이템들의 정보를 담고 있는 struct
struct SDotaCore_D_Point_Item
{
    SDotaCore_D_Point_Item()
    {
        Init();
    }

    float fAtk;         //
    float fHp;
    float fFullHp;      //
    float fCritical;    //
    float fNPCAtk;      //
    float fNPCDef;
    float fNPCHp;       //
    float fNPCSpeed;

    // 소환몬스터 능력치 누적변수
    float fAccSerdinNPCAtk;
    float fAccSerdinNPCHp;
    float fAccCanabanNPCAtk;
    float fAccCanabanNPCHp;

    // 아이템 사용횟수처리
    int iUseCntAtk;
    int iUseCntFullHp;
    int iUseCntCri;
    int iUseCntNPCAtk;
    int iUseCntNPCHp;

    void Init()
    {
        fAtk = 0.0f;
        fHp = 0.0f;
        fFullHp = 0.0f;
        fCritical = 0.0f;
        fNPCAtk = 0.0f;
        fNPCDef = 0.0f;
        fNPCHp = 0.0f;
        fNPCSpeed = 0.0f;

        InitCnt();
        InitNPCAcc();
    }

    void InitCnt()
    {
        iUseCntAtk = 0;
        iUseCntFullHp = 0;
        iUseCntCri = 0;
        iUseCntNPCAtk = 0;
        iUseCntNPCHp = 0;
    }

    void InitNPCAcc()
    {
        fAccSerdinNPCAtk = 0.0f;
        fAccSerdinNPCHp = 0.0f;
        fAccCanabanNPCAtk = 0.0f;
        fAccCanabanNPCHp = 0.0f;
    }
};

// 점령전 : D-Point Shop정보를 담고 있는 struct
struct SDota_D_PointShopInfo
{
    SDota_D_PointShopInfo()
    {
        Init();
    }

    // 샵에 대한 정보
    int iShopID;
    int iTeam;
    D3DXVECTOR2 vSize;

    void Init()
    {
        iShopID = -1;
        iTeam = -1;
        vSize = D3DXVECTOR2(0.0f, 0.0f);
    }
};

// 점령전 : D-Point Shop에서 판매하는 아이템 정보를 담고 있는 struct
struct SDota_D_PointShopItemInfo
{
    SDota_D_PointShopItemInfo()
    {
        Init();
    }

    // 판매할 아이템 정보(아이템ID, 아이템DPoint가격)
    std::vector<pair<int, int>> vecItemList;

    // 아이템 이름과 설명 StringID
    std::map<int, pair<int, int>> mapItemDesc;

    void Init()
    {
        vecItemList.clear();
        mapItemDesc.clear();
    }
};

// 점령전 : 게임진행 알림메시지 정보를 담고 있는 struct
struct SDotaPlayingDesc
{
    SDotaPlayingDesc()
    {
        Init();
    }

    void Init()
    {
        mapDotaMsg.clear();
    }

    enum
    {
        DOTA_MSG_STARTGAME,     // 아테나 모드는 적 진영의 코어(Core)를 먼저 파괴하면 승리 합니다.
        DOTA_MSG_STARTPLAYE,    // 잠시 후 아테나가 시작 됩니다.
        DOTA_MSG_PORTALOPEN,    // 코어 지역으로 이동 가능한 포탈이 활성화 되었습니다.
        DOTA_MSG_ARENA,         // 잠시 후 부활 합니다.\n준비하세요.
        DOTA_MSG_SHOP,          // Z 키를 누르면 A-Point상점을 이용할 수 있습니다.
        DOTA_MSG_INCOREZONE,    // 적이(@1) 코어 지역에 침입 하였습니다.
        DOTA_MSG_HPCORE,        // 코어의 생명력이 @1%이하가 되었습니다.\n지금 부터 A-Point가 자동 지급 됩니다.
        DOTA_MSG_HP_CORE_RADIO, // 코어 Hp가 10%씩 감소될때마다 출력할 알림 메시지
        DOTA_MSG_BREAK_CORE,    // 코어가 파괴되었을때 출력할 메시지
        DOTA_MSG_BREAK_GUARDIANTOWER, // 가디언타워가 파괴되었을때 출력할 메시지            
        DOTA_MSG_BREAK_GUARDTOWER,    // 가드타워가 파괴되었을때 출력할 메시지
        DOTA_MSG_PLAYER_DEATH,  // 플레이어가 플레이어에게 제압당했을때 출력할 메시지
        DOTA_MSG_EMPTY_TEAM,    // 상대 플레이어가 모두 나갔을때 출력할 메시지
        MAX_DOTA_MSG,
    };

    std::map<int, std::pair<int,int>> mapDotaMsg;
};

// 점령전 : 게임 가이드 메시지 정보를 담고 있는 struct
struct SDotaGameGuide
{
    SDotaGameGuide()
    {
        Init();
    }

    void Init()
    {
        mapDotaGameGuide.clear();
    }

    enum
    {
        DOTA_GUIDE_STARTGAME,       // 적의 제어장치를 파괴하면 승리 합니다.
        DOTA_GUIDE_MINIMAP,         // 적 제어장치는 미니 맵의 밝게 빛나는 부분에 있습니다.
        DOTA_GUIDE_FIRST_DEATH,     // 상점을 이용하면 지금 보다 더 강해질 수 있습니다.
        DOTA_GUIDE_FIRST_KILL,      // 적을 처치하면 A-POINT 를 얻을 수 있습니다.
        DOTA_GUIDE_APOINT,          // A-POINT 를 이용하여 아이템을 구입할 수 있습니다.
        DOTA_GUIDE_SHOP_CLOSE,      // 아이템 사용 : 1 ~ 3
        DOTA_GUIDE_ATTACK_CORE,     // 제어 장치가 공격 받고 있습니다.
        DOTA_GUIDE_BREAK_GUARDTOWER,// 가디언 타워를 파괴하세요.
        DOTA_GUIDE_IN_ZONE,        // 가드 타워를 파괴하세요.
        DOTA_GUIDE_BREAK_GUARDIANTOWER, // 제어장치를 파괴하면 승리 합니다.   
        MAX_DOTA_GUIDE,
    };

    std::map<int, int> mapDotaGameGuide;
};

// 점령전 : 캐릭터 능력치 평준화 정보를 담고 있는 struct
struct SDotaAbility
{
    SDotaAbility()
    {
        Init();
    }

    void Init()
    {
        fAbilityAtk = 0;
        fAbilityDef = 0;
        fAbilityHp = 0;
    }

    float                        fAbilityAtk;
    float                        fAbilityDef;
    float                        fAbilityHp;
};

// 점령전 : 점령전의 모든 정보를 담고있는 struct
struct SDotaTemplate
{
    SDotaTemplate()
    {
        Init();
    }

    int                          iDotaMapID;
    int                          iPlayer_D_Point;
    int                          iMonster_D_Point;
    int                          iKillerMonster_D_Point;
    int                          iCoreSerdin_MonSlotID;
    int                          iCoreCanaban_MonSlotID;
    int                          iSummonMonsterStartSlotIndex;

    int                          iGameStartCoolTime;
    int                          iArenaCoolTime;
    int                          iGameEndCoolTime;
    int                          iGameStartLimitUser;
    int                          iNumZoneMax;

    int                          iCoreZone_Serdin;
    int                          iCoreZone_Canaban;
    bool                         bUseAIFromScript;

    std::string                  strMap;
    SDotaDefault_D_Point         sDefault_D_Point;
    SDotaCore_D_Point            sCore_D_Point;
    SDotaCore_D_Point_Item       sDPointItemInfo;
    SDota_D_PointShopItemInfo    sDPointShopItemList;
    SDotaPlayingDesc             sDotaMsgInfo;
    SDotaGameGuide               sDotaGameGuide;
    std::vector<SDotaAbility>    vecsDotaAbility;
    std::vector<SDotaGateInfo>   vecsGateInfo;
    std::vector<SDotaStaticMonsterInfo> vecsStaticMonsterInfo;
    std::vector<SDotaMonsterPortalInfo> vecsMonsterPortalInfo;
    std::vector<SDota_D_PointShopInfo>  vecsDPointShopInfo;

    void Init()
    {
        iDotaMapID              = -1;
        iPlayer_D_Point         = 0;
        iMonster_D_Point        = 0;
        iKillerMonster_D_Point  = 0;
        iCoreSerdin_MonSlotID   = -1;
        iCoreCanaban_MonSlotID  = -1;
        iSummonMonsterStartSlotIndex = 0;

        iGameStartCoolTime      = 0;
        iArenaCoolTime          = 0;
        iGameEndCoolTime        = 0;
        iGameStartLimitUser     = 0;
        iNumZoneMax             = 0;

        iCoreZone_Serdin        = -1;
        iCoreZone_Canaban       = -1;
        bUseAIFromScript        = false;

        strMap                  = "dota_stage_1.lua";
        vecsGateInfo.clear();
        vecsStaticMonsterInfo.clear();
        vecsMonsterPortalInfo.clear();
        vecsDPointShopInfo.clear();
    }
};

// 패킷Send 딜레이 테스트용 구조체
struct KAddSendPacket
{
    BYTE *pData;
    UINT uiPacketSize;
    DWORD dwUID;
    UINT uiSendOption;
    KAddSendPacket()
    {
        pData = NULL;
        uiPacketSize = 0;
        dwUID = 0;
        uiSendOption = 0;
    };
    KAddSendPacket( const KAddSendPacket &Arg )
    {
        pData = new BYTE[Arg.uiPacketSize];
        memcpy(pData, Arg.pData, Arg.uiPacketSize);
        uiPacketSize = Arg.uiPacketSize;
        dwUID = Arg.dwUID;
        uiSendOption = Arg.uiSendOption;
    };
    ~KAddSendPacket()
    {
        SAFE_DELETE_ARRAY( pData );
    };
    const KAddSendPacket* operator = ( const KAddSendPacket &Arg )
    {
        SAFE_DELETE_ARRAY( pData );
        pData = new BYTE[Arg.uiPacketSize];
        memcpy(pData, Arg.pData, Arg.uiPacketSize);
        uiPacketSize = Arg.uiPacketSize;
        dwUID = Arg.dwUID;
        uiSendOption = Arg.uiSendOption;
        return this;
    };
    void AddPacketData(BYTE* pkPacket_, UINT uiPacketSize_, DWORD dwUID_, UINT uiSendOption_)
    {
        SAFE_DELETE_ARRAY( pData );
        pData = new BYTE[uiPacketSize_];
        memcpy(pData, pkPacket_, uiPacketSize_);
        uiPacketSize = uiPacketSize_;
        dwUID = dwUID_;
        uiSendOption = uiSendOption_;
    };
};

struct MoveModelMotion
{
    int m_iMotionID;						//0x00
    int m_iReplayCount;				//0x04
    int m_iMouseEventType;			//0x08
    std::string m_strMotionName;	//0x0C
};

struct MoveModelParticle
{
    std::string m_particleSeqName;
    float m_fXOffset;
    float m_fYOffset;
    float m_fTraceTime;
    int m_iMouseEventType;
};

struct MoveModelData
{
    float  m_fPosX;			// 0x00
    float  m_fPosY;			// 0x04
    float  m_fScale;			// 0x08
    float	m_fStartX;			// 0x0C
    float	m_fStartY;			// 0x10
    int		m_iWidth;			// 0x14
    int		m_iHeight;			// 0x18
    int		m_NPCID;			// 0x1C
    bool  m_bUseEvent;		// 0x20

    std::string m_strMeshName;
    std::string m_strTexName;

    std::vector< MoveModelMotion > m_vecMotionList;
    std::vector< MoveModelParticle > m_vecParticle;

    const MoveModelData* operator= (const MoveModelData& other)
    {
        m_NPCID = other.m_NPCID;
        m_fPosX = other.m_fPosX;
        m_fPosY = other.m_fPosY;
        m_fScale = other.m_fScale;
        m_bUseEvent = other.m_bUseEvent;
        m_fStartX = other.m_fStartX;
        m_fStartY = other.m_fStartY;
        m_iWidth = other.m_iWidth;
        m_iHeight = other.m_iHeight;
        m_strMeshName = other.m_strMeshName;
        m_strTexName = other.m_strTexName;
        m_vecMotionList.assign(other.m_vecMotionList.begin(), other.m_vecMotionList.end());
        m_vecParticle.assign(other.m_vecParticle.begin(), other.m_vecParticle.end());
        return this;
    }
};

// 전역 정보 구조체
class KGCGlobal : public lua_tinker::lua_value
{
public:
    KDeclareLuaInvoke
        KGCGlobal();
    ~KGCGlobal();

public:
	std::map< char, std::vector< std::pair< GCITEMID, int > > >           m_mapGachaSuccessList;
	std::map< char, std::vector< std::pair< GCITEMID, int > > >           m_mapGachaFailedList;

	std::map<int,GCITEMID> m_mapCharCashGoods;



    //int m_aiMonNameID[MONSTER_MODEL_NUM];
    //요기부터///////////////////////////////////////////////////////////
    // DATE: 07-03-14    milennium9@kogstudios.com
    /////////////////////////////////////////////////////////////////////
    //  신규 유저는 튜토리얼 꼭 거쳐야 되니까!!! 퀘스트도!!!
    /////////////////////////////////////////////////////////////////////
    bool m_bNewUser_Tutorial;
    bool m_bNewUser_Quest;
    /////////////////////////////////////////////////요기까지//milennium9

    //////////////////////////////////////////////////////////////////////////
    // 오전 10:46 2008-02-28 jemitgge - 차단 유저 닉넴리스트

private:
    std::vector<std::wstring> m_vecBlockUserName;
    // 대기방이나, 광장에 있는 유저들 이름~
    std::vector<std::wstring> m_vecCurrentStateUserInfo;
public:
    void OnQuantityDepotMove(GCITEMUID m_ItemUID, int DepotStatus);
    void AddCurrentStateUser( std::wstring strName );
    void DelCurrentStateUser( std::wstring strName );
    void ReleaseCurrentStateUser( void );
    void ADD_BLOCK_USER( std::wstring strName );
    void DEL_BLOCK_USER( std::wstring strName );
    bool IsBlockUser( std::wstring strName );
    int GetUserGP() { return m_kUserInfo.iGP; }
    int GetCurrentCharacterGP( void );
    bool IsHost();
    bool isCurrentShopVIP = false;
    bool isUndoCoordi = false;
    int m_iCurrentGachaType = 0;
    GCITEMID m_pkCurrentUseItemID = -1;
    GCITEMUID m_pkCurrentUseItemUID = 0;
    GCITEMID m_pkCurrentUseItemCurrency = -1;
    int m_iCurrentUseItemMultiplier = 1;
    int m_iCurrentUseItemQuantity = 1;
    int m_iCurrentUseItemPrice = 1;
    bool m_bIsCurrentUsechest = false;
    GCITEMID NickColorChangeItemID;
    GCITEMUID NickColorChangeItemUID;

    bool isNickColorChangeOn = false;
    int m_iOvejarCloneDeathCount = 0;
    float currentTotalForgeDiscount = 0.0f;

    std::vector<std::wstring>	m_vecstrNickColorList;
    std::vector<int> m_vecZeroMaskID = { 1595220, 1595230, 1595240, 1595250 };

    bool GetIsZeroMask(int iItemID);
    int GetCurrEquipedZeroMaskID(SCharInfo* pkCharInfo);
    int GetZeroMaskItemID(int iOption);

	std::wstring GetCharName(const int& iCharType_ );
	std::wstring GetCharDesc(const int& iCharType_ );
	std::wstring GetJopName(const int& iCharType_ , const int& iPromotion_ );
    std::wstring GetRankScoreName(const int& iRank_);

	int GetCharDifficuleLevel(const int& iCharType_ );
    
    std::vector<GCITEMID>                       m_vecCashQuantityList;
    void SetCashQuantityList(void);
    std::vector<GCITEMID>                       m_vecVIPItemList;
    void SetVIPItemList(void);
    bool IsItemInVIPList(GCITEMID iItemID);

	KProtectedType<BOOL>						m_bCheckChangedKOM;
	std::map< std::wstring, std::string>		m_mapModifiedFile;
	std::vector< std::string >					m_vecDeleteFileLIst;

	std::vector< std::wstring >					m_vecSHAFilePathList;
	std::vector< DWORD >						m_vecSHAFileSizeList;
	std::vector< std::wstring >					m_vecSHAFileSHA;

    std::wstring                                m_wstrFailString;
    // 챔피언 몬스터 정보
    std::vector< KChampionInfo >                m_vecChampionInfo;

    // 몬스터 대전 정보가 담긴 테이블
    std::vector< SMonsterHuntInfo >             m_mapMonsterHuntInfo;

    // 몬스터 원정대 정보가 담긴 테이블
    std::vector< SMonsterCrusaderInfo >         m_mapMonsterCrusaderInfo;

    // 길드레벨 테이블
    std::map<USHORT,int>                        m_mapGuildLevelTable;

    SUserInfo                                   m_kUserInfo;
    SServerInfo                                 ServerInfo;
    SLoginInfo                                  m_kLoginInfo;
	SClipBoardInfo								m_kClipBoardInfo;
    std::vector<KServerInfo>                    ServerList;
    std::vector<std::wstring>                   m_vecDelayMsg;
    KProtectedType<DWORD>                       m_dwCash;
    KProtectedType<DWORD>                       m_dwVirtualCash;
	KProtectedType<DWORD>                       m_dwGachaCoin;
    KCashbackRatioInfo                          m_kCashbackRatioInfo; // 캐시아이템 GC 전환율
    int                                         m_nCashbackRatioFirstBuying; // 첫구매 유저 전환율 (이벤트 적용시에만)
    KProtectedType<DWORD>                       m_dwChinaWebCash;
    // 펫 관련 정보
    int                                         m_iPetNum;
    int                                         m_iPetEvolutionlevel;
    int TreatCharTypeID(int ExtraCharNum);
    void SetCoordiViewItemInfo();
    bool IsSkillEffectDisabled(int iWho);
    std::vector< std::pair< int, int > >		                CoordiViewVector;
    std::map<int, std::vector< std::pair< int, int > > >		m_mapCoordiViewItemInfo;
private:
    std::map< int, KPetTemplateContainer >      m_mapPetTemplate;
    SPetTemplate& GetPetTemplate_(int iPetID_, int iPromotion_);

public:
    std::vector< std::pair< int, int > > GetCoordiViewChar(int m_iCharType) { return m_mapCoordiViewItemInfo[m_iCharType]; }
	const SPetTemplate& GetPetTemplateFromItemID( GCITEMID ItemID_, int iPromotion_ );
    const SPetTemplate& GetPetTemplate( int iPetID_, int iPromotion_ );
    void UpdatePetCostume( int iPetID_, int iPetPromotion_, const std::set<DWORD>& setCostumes );
    void AddPetTemplate( int iPetID_, int iPromotion_, const SPetTemplate& sPetTemplate_ );
    int GetPetIDFromItemID( GCITEMID ItemID_ );

	// 펫이 사용할 수 있는 스킬의 리스트
	std::map< int, std::vector<int> > m_mapPetSkillItemInfo;		// petSkill, vector<PetID>

    float                                       m_fTime;                // 게임 시작 후 누적 시간
    DWORD                                       m_dwLastMsgDisplayTime; //마지막으로 시간 경과 메세지를 보여준 시간
	DWORD                                       m_dwLastCommonMsgDisplayTime; //마지막으로 공지 메세지를 보여준 시간
	std::map< int, std::pair< DWORD, std::vector<std::wstring> > >	m_mapCommonMsg;			// 공지 메시지 map<공지시간간격, pair<남은시간간격, vector<공지메시지> > >
    int                                         m_iPlayBeginTime;       //게임 시작 시간. ms
    KProtectedType<int>                         m_MonsterMake;
    std::string                                 m_strCrashMsg;          //크래쉬 발생시 출력할 스트링
    std::wstring                                m_strDetectedHackTool;
    bool                                        m_bHighMem;

    int                                         m_iTestValue;       //ㅋㅋㅋㅋ
    bool                                        m_bRenderBoundBox;  //ㅋㅋㅋㅋ
    bool                                        m_bFrameLock;       //ㅋㅋㅋㅋ
    bool                                        m_bShowControls;    //ㅋㅋㅋㅋ
    bool                                        m_bRenderPosition;  //위치정보 출력
    PublisherAuthType                           m_dwAuthType;

    std::string                                 m_strNullFunc;
    bool                                        m_bCharInfoIsNull;
    bool                                        m_bSkillMotionIDErrorOccur;
    bool                                        m_bMonsterSuicide;
    MonsterMotionError                          m_errMonsterMotion;
    MonsterFrmError                             m_errFrmLoad;

public:
    KProtectedType<int>                         m_iInvenCapacity;
    KProtectedType<int>                         m_iCoordiCapacity;

    // 2007/3/23. iridology. 2번 보내기 막기 위함
    bool                                        m_bSendErrReport;
    int                                         m_iCloseType;
    // 2007/4/16. iridology. 무고한 해킹툴...젠장..
    std::wstringstream                          m_stmHackComment;

    // 2007.04.22 wony
    KProtectedType< int >                       m_iGCPoint;             // GC포인트
    bool                                        m_bExistCalendar;       // 달력 정보를 받았는가?
    KProtectedType< int >                       m_iPlayTime;            // 오늘의 플레이 타임
    KSimpleDate                                 m_kToday;               // 오늘의 날짜
    int                                         m_iAttendTime;          // 도장을 찍기 위해 플레이 해야하는 시간.. (분 단위)
    std::map<char, std::vector<KDailyInfo> >    m_mapCalendar;          // 이번달 달력정보 - 출석이 X인날은 제외하고 들어있음

    KSimpleDate                                 m_kLastDayOfLastMonth;  // 이전달의 마지막날
    KSimpleDate                                 m_kFirstDay;            // 이달의 첫째날
    KSimpleDate                                 m_kLastDay;             // 이달의 마지막날
    DWORD                                       m_dwAccTimeNot;         // 마지막으로 AccTimeNot를 받은 시간
    std::vector<KSimpleDate>                    m_vecReservedReward;    // 월 개근으로 받을 수 있는 아이템이 있는가?

    // 2007.8.20 milennium9 신고관련 변수들
    int                                         m_iReportAvailableCount;
    int                                         m_iReportedCount;
    bool                                        m_bReportInGame;

    std::vector< std::pair<int,int> >           m_vecHackCount;

    void SetCalendarInfo( KCalendarInfo kCalendarInfo );

    KGCChattingEvent                            m_kChattingEvent;
    bool m_bRisingSunOrMoonEvent;

	bool m_bLoginFirst;

    //현재 꺼지고 잇는 상태인가
    bool m_bIsOnGameClose;

	//현재 게임창이 활성화 상태인가
	bool										m_bActiveApp;
	KProtectedType<BOOL>						m_bUseCrack;

    bool                                        m_bAIEnable;
    bool                                        m_bRenderMonster;
    bool                                        m_bRenderFootHold; //맵에 발판을 표시해 준다
    bool                                        m_bRenderMonsterStatus; // 몬스터 정보를 표시한다
    bool                                        m_bRenderSquarePeopleData; // 광장 사람들 정보를 표시

    bool                                        m_bExpAccount; //체험계정인지를 가림 -- exp_id
    bool                                        m_bTutorialReselect;  //연습모드에서 1번키를 눌렀을때, 재선택모드일시

    bool                                        m_bAlreadyRegistered = false;

    // Player에서 크래쉬 날  상황을 알기 위해 추가된 녀석
    std::map< UINT, UINT >                      m_mapErrorMotion;
    static _Mail                                ms_kMail;
    bool MakeErrorMotionToFile( std::string strFileName, bool bSendMail = false );
    static void GlobalMailSendEnd( BOOL bSuccess_ );

    //파티목록 컨데이너
    std::vector<KPartyMember> m_vecPartyPeople;

    // 던전 게임 시작 플레이어수
    int m_nPlayerCount;

    //  셧다운제 셋팅 ( 서버에서 클라이언트 종료 명령을 내렸다. ) 
    bool m_bShutDown;


    // 2007.10.27 milennium9 유저의 로딩 시간을 알기위한 변수
private:
    bool    m_bLoadingTimeSend;
    int     m_iLoadingTime;
public:
    void SetLoadingTime( int iTime )   { m_iLoadingTime = iTime; }
    void SendLoadingTime();
    // 2007.10.27 milennium9 로그인 서버 접속 시간 지연 조사를 위한 변수
private:
    bool    m_bLoginServerConTimeSend;
    DWORD   m_dwElapsedSec;
public:
    void    SetLoginServerConTime( DWORD dwTime )  { m_dwElapsedSec = dwTime; }
    void    SendLoginServerConTime();

public:
    void InitPetTable();
    int GetMaxCharLevel();
    bool m_bIsObserverMode;

private:
	bool m_bObserverMode;
    bool m_bHellMode;
    bool m_bBreakMode;
    char m_cSavedCharType;
public:
	bool IsAdmin();
	bool IsObserverMode() { return m_bObserverMode; };
	void SetObserverMode( bool bObserver );

    bool IsHellMode() { return m_bHellMode; };
    void SetHellMode( bool bHellMode );

    bool IsBreakMode() { return m_bBreakMode; };
    void SetBreakMode(bool bBreakMode);

	bool IsEquipSkill(const int iSkill_ID, int iPetID);

    char GetSavedCharType() const { return m_cSavedCharType; }
    char GetSavedCharTypeToIndex();
    void SetSavedCharType(char val) { m_cSavedCharType = val; }
    void SendSavedCharType();

    //====================================================================================
    // 2006.11.10 : Asirion
    // 루아 관련 함수들.
    SServerInfo* GetServerInfo(){ return &ServerInfo; }

    bool CheckCalendarAlert();
    std::wstring GetGameVersionW();
    std::string GetGameVersionA();

    //미션 관련 변수
    KGCUserMission m_kUserMission;
    KGCUserCollectionMission m_kUserCollectionMission;

public:
    DWORD m_dwAuthTick;

#ifdef _DEBUG
    KSafeArray<float,10> m_fDevValue;
    void                LoadDevLua();
#endif

private:
    void				LoadUseHellModeBtn();
    std::set<int>     m_setFreeDungeonInHellMode;
    std::set<int>     m_setFreeWorldMapInHellMode;

public:
	int					m_iUseHellModeBtn;
	
    bool IsFreeDungeonInHellMode(EGCGameMode eGameMode_)
    {
        return (m_setFreeDungeonInHellMode.find( static_cast<int>(eGameMode_)) != m_setFreeDungeonInHellMode.end() );
    }

    bool IsFreeWorldMapInHellMode(int iWorldMap_)
    {
        return (m_setFreeWorldMapInHellMode.find( iWorldMap_) != m_setFreeWorldMapInHellMode.end() );
    }

    void AddFreeWorldMapInHellMode();
    void SetFreeDungeonInHellMode(std::set<int> setFreeDungeonInHellMode_)
    {
        m_setFreeDungeonInHellMode.swap(setFreeDungeonInHellMode_);
    }

private:
	//용섬 관련 변수
	std::vector<KIndigoRankInfo>	m_vecIndigoRank;
public:
	void SetIndigoRank( std::vector<KIndigoRankInfo>& vecRank );
	std::vector<KIndigoRankInfo> GetIndigoRank() { return m_vecIndigoRank; }

	//웹 캐시관련 함수
	void UpdateCashData();
	//////////////////////////////////////////////////////////////////////////
	// 반디 캡쳐!
	CBandiCaptureLibrary	 m_bandiCaptureLibrary;
	void ToggleVideoCapture();
	bool IsCapturing( void );
	void CapTureWork( LPDIRECT3DDEVICE9 pd3dDevice );
	//////////////////////////////////////////////////////////////////////////

public:
	KGCRanking	m_kRanking;

    bool        IsEditMode();
    void        SetEditMode( bool bEdit_ );

private:
    bool        m_bEditMode;

        

public:
	bool		m_bChatEventMessageTemp;
	int			m_iChatEventCount;
	CTime	m_tmServerTime;
	time_t	m_tmServerSyncTime;

	std::vector<std::string> m_vecProtectedTypeFail;

	inline time_t TimeToInt( CTime& cTime_ )
	{
		// CTime::GetTime()은 __time64_t를 리턴한다. 안전하게 struct tm을 얻어서 timt_t를 구하자.
		struct tm tmBuff;
		tmBuff.tm_isdst = -1;
		cTime_.GetLocalTm( &tmBuff );
		return ::mktime( &tmBuff );
	}

private:
	std::map< DWORD ,std::vector<PetMeshInfo>> m_mapPetCostumeInfo;   

    void CalcPushRect();
    void CalcAttackRect();

public:
	DWORD GetBasePetItemIDFromPetCostume( DWORD dwItemID );
	int GetPromotionFromPetCostume( DWORD dwItemID );
	const SPetTemplate& GetBasePetTemplateFromPetCostume( DWORD dwItemID );
	bool IsPetCostumeItem( DWORD dwItemID );
	void AddPetCostumeMeshInfo( DWORD dwItemID, PetMeshInfo sInfo );
	std::vector<PetMeshInfo> GetPetCostumeMeshList( DWORD dwItemID );

	std::vector<std::wstring> m_vecCrashStr;
	KServerInfo                         m_kMsgServerInfo;

	KEMS_S2_VERIFY_ACCOUNT_REQ		m_kVerifyInfo;

private:
	std::wstring m_strServerbarEventTitle;
	std::wstring m_strServerbarEventSubtitle;
	std::wstring m_strServerbarEventDesc;

public:
	void SetServerbarEventTitle( IN const std::wstring& str_ );
	void SetServerbarEventSubtitle( IN const std::wstring& str_ );
	void SetServerbarEventDesc( IN const std::wstring& str_ );
	std::wstring GetServerbarEventTitle();
	std::wstring GetServerbarEventSubtitle();
	std::wstring GetServerbarEventDesc();
	void ServerbarEventStringClear();
public:
    bool m_bIsRecommendEvent;

	bool m_bIsRainbowEvent;
	bool m_bJackPot;
	int m_iRainbowEventID;
	int m_ivurcashper; //가상캐쉬 혼용비율

	std::set<int> m_setGiftsEvent;

	bool m_bIncompletePatch;

	KEVENT_SUBSCRIPTION_INFO_NOT m_mapSubscriptionEvent;

    bool                                  m_bMUHANLife;// 목숨수 무제안 이벤트....
    bool                                  m_bMUHANBonusPoint;// 이벤트시 보너스 포인트가 있는 한  무한으로 사용 가능 한가??

public:
	bool CheckPremium( KPremiumInfo::PREMIUM_TYPE eType );

public:
	bool IsUnlock() { return m_bUnlock; };
	void SetUnlock( bool bUnlock ) { m_bUnlock = bUnlock; };
    
    bool IsDungeonFreePass() { return m_bDungeonFreePass; }
    void SetDungeonFreePass( bool bDungeonFreePass ) { m_bDungeonFreePass = bDungeonFreePass; };

private:
	bool m_bUnlock;             //  사내 치트키
    bool m_bDungeonFreePass;    //  던전 프리패스 입장권에 사용하는 변수 ( 전용던전때문에 나눕니다. ) 

	// 최대 로딩이미지 정보 기록
private:
    std::map< int, std::vector< std::wstring > >                            m_mapLoadingImageName;  // 로딩이미지 이름 ex) Load1_th_1.dds Load2_th_1.dds Load3_th_1.dds // LoadingImageFileName

	std::vector<int>	m_veciNewsFileNum;		// 뉴스 파일 이름
	std::vector<std::wstring>		m_strLoadingImageName; // 로딩이미지 이름1
	std::wstring		m_strLoadingImageName2; // 로딩이미지 이름2
	std::vector<int>	m_vecExceptionMotionID; // 예외 모션 ID
	std::set<DWORD>		m_setCharSkillEnable;   // 스킬트리 사용여부 On/Off 정보
    std::set<int>       m_setHideSceneMenuBtn;  // 메인메뉴 숨기기 여부


	KPVPLodingImageFileName	m_mapPvPLoadingImageName; //대전로딩 파일 이름.
	std::vector<std::pair<std::wstring, int>>	m_vecEventBannerInfo; //이벤트 벡터.

	std::wstring		m_strSquare;
	std::wstring		m_strSquareObject;
	std::wstring		m_strSquare3DObject;

public:
	std::set<DWORD> GetCharSkillEnable()	{ return m_setCharSkillEnable; }
	inline std::vector<int> GetNewFileNum()
	{
		return m_veciNewsFileNum;
	}

	inline bool IsExceptMotion(int iMotionID)
	{
		for (int i=0;i<(int)m_vecExceptionMotionID.size();i++)
		{
			if (m_vecExceptionMotionID[i]==iMotionID)
				return true;
		}
		return false;
	}

	inline bool IsSkillTreeOpenChar(DWORD iChar)
	{
#if !defined(SKILL_TREE_OPEN_BY_SERVERSCRIPT)
		return true;
#endif

		if (m_setCharSkillEnable.find(iChar)!=m_setCharSkillEnable.end())
		{
			return true;
		}
		return false;
	}


	inline std::vector<std::wstring> GetLoadingFileName1()
	{
		return m_strLoadingImageName;
	}

    std::vector<std::wstring> GetLoadingFileName();


	inline std::wstring GetLoadingFileName2()
	{
		return m_strLoadingImageName2;
	}

	inline std::wstring GetScriptNameSquare()
	{
		return m_strSquare;
	}

	inline std::wstring GetScriptNameSquareObject()
	{
		return m_strSquareObject;
	}

	inline std::wstring GetScriptNameSquare3DObject()
	{
		return m_strSquare3DObject;
	}

	inline void SetLoadingImageData(std::vector<std::wstring> strLoadingImageName1, std::vector<int> veciNewsFileNum)
	{
		m_veciNewsFileNum=veciNewsFileNum;
		m_strLoadingImageName=strLoadingImageName1;
	}

    inline void SetLoadingImage( std::map< int, std::vector< std::wstring > >& mapLoadingImageName_ )
    {
        m_mapLoadingImageName = mapLoadingImageName_;
    }
    inline void SetNewFileNum( std::vector<int> veciNewsFileNum_ )
    {
        m_veciNewsFileNum = veciNewsFileNum_;
    }

    inline void SetLoadingImageData(std::map< int, std::vector< std::wstring > >& mapLoadingImageName_ , std::vector<int> veciNewsFileNum_ )
    {
        if ( false == mapLoadingImageName_.empty() )
            m_mapLoadingImageName = mapLoadingImageName_;

        if ( false == veciNewsFileNum_.empty() )
            m_veciNewsFileNum = veciNewsFileNum_;
        
    }

	inline void SetPvPLoadingImageName(KPVPLodingImageFileName mapPvPLoadingImageName)
	{
        if ( false == mapPvPLoadingImageName.empty() )
		    m_mapPvPLoadingImageName = mapPvPLoadingImageName;
	}

	inline KPVPLodingImageFileName& GetPvPLoadingImageName()
	{
		return m_mapPvPLoadingImageName;
	}


	inline std::vector<std::pair<std::wstring, int>>& GetEventBannerInfo()
	{
		return m_vecEventBannerInfo;
	}

	void SetCharSkillEnable(std::set<DWORD> setCharSkillEnable)
	{
		m_setCharSkillEnable = setCharSkillEnable;
	}

	void SetvecExceptionMotionID(std::vector<int> vecExceptionMotionID)
	{
		m_vecExceptionMotionID=vecExceptionMotionID;
	}

	inline void SetEventBannerInfo(std::vector<std::pair<std::wstring, int>> vecEventBannerInfo)
	{
		m_vecEventBannerInfo = vecEventBannerInfo;
	}


	void SetSquareScriptName(std::wstring strSquare,std::wstring strSquareObject, std::wstring strSquare3DObject)
	{
		m_strSquare=strSquare;
		m_strSquareObject=strSquareObject;
		m_strSquare3DObject=strSquare3DObject;
	}

    void SetHideSceneMenuBtn( const std::set<int>& setHideSceneMenuBtn );
    bool IsHideSceneMenuBtn( int nHideType );

	// 최대 레벨 정보 기록
private:
	int				m_iMaxLevel;
public:
	inline int GetMaxLevel()
	{
		return m_iMaxLevel;
	}

	void SetMaxLevel(int iMaxLevel)
	{
		m_iMaxLevel=iMaxLevel;
	}

    std::vector< CollectionCharacter > m_vecCharacterCollectionList;
    std::vector< CollectionVIP > m_vecVIPCollectionList;
private:
    std::map<int, std::set < char > >       m_mapCharactes;    //< Char ID, Promotion >
	std::map<int,DWORD>		m_mapCharOpenType;	// <Char ID , OpenType >
    std::vector<int>      m_vecCharSP4OpenInfo;
    std::deque<int>       m_deqTRAvgPing;
public:
    void GetCharacterCollectionList();
    void GetVIPCollectionList();
    void SetOpenedCharacterInfo( const std::map<int, std::set< char > >& mapInfo ) { m_mapCharactes = mapInfo; }
	void SetCharOpenType( const std::map<int,DWORD>& mapInfo_ )		{ m_mapCharOpenType = mapInfo_; }
    void SetCharSp4OpenInfo( const std::vector<int>& vceCharSp4OpenInfo_ )		{ m_vecCharSP4OpenInfo = vceCharSp4OpenInfo_; }
    void SetDisableUIBtnList( const std::vector<int>& vecDisableMenuList_ ) { m_vecDisableUIBtnList = vecDisableMenuList_; }
    void SetSmartPanelMailList( const bool bSmartPanelMailListEnable, const std::vector<std::wstring> vecSmartPanelMailList );

    void SetOpenedEventDungeonInfo( std::vector<std::pair<int, std::vector<int> > > CategoryModes );
    std::vector<EGCGameMode> KGCGlobal::GetOpenedEventDungeonInfo();
	
    std::map<int,DWORD> GetCharOpenType()	{ return m_mapCharOpenType; }
    std::vector<int> GetCharSP4OpenInfo() { return m_vecCharSP4OpenInfo; }
	void ClearOpenedCharacterInfo() { m_mapCharactes.clear(); }
	int	 GetOpenedCharacterNum() {return m_mapCharactes.size() ;}
	bool IsCharacterOpenFromServer() { return !m_mapCharactes.empty(); }
	bool IsOpenedChar( int iCharID );
    bool IsOpenedPromotion( int iCharID, char iPromotion );
    bool IsOpenedPromotion( int iCharID, std::set< char > setPromotion );
	void GetOpenedPromotion( int iCharID, std::set< char >& setPromotion );
    int GetOpenedPromotionNum( int iCharID );
    std::map<int, std::set < char > > GetOpenedCharacterInfo() { return m_mapCharactes; }
	DWORD GetCharOpenType(const int& iCharType_ );
    std::vector<int> GetDisableMenuList() { return m_vecDisableUIBtnList; }
    bool IsSmartPanelMailListEnable() { return m_bSmartPanelMailListEnable; }
    std::vector<std::wstring> GetSmartPanelMailList() { return m_vecSmartPanelMailList; }

public:
    KNGuildUserInfo                		m_kGuildUserInfo;
    KNGuildInfo                    		m_kGuildInfo;
	std::map< DWORD, KNGuildUserInfo >	m_mapNGuildUserInfo;
	std::map<char, std::set<DWORD>>		m_mapNGuildChangeMemberGrade;
	std::set<DWORD>						m_setAddGuildMemberData;
	std::set<DWORD>						m_setDelGuildMemberData;
	std::vector< KGuildUserInfo > 		m_vecGuildUserList;
	std::vector< KGuildUserInfo > 		m_vecOnlineGuildUserList;
	std::wstring						m_strGuildMarkURL;
	std::wstring						m_strTempGuildName;
	DWORD								m_dwChannelType;
    bool                                m_bUseLoadCheckInServer;
    __int64                             m_biUniqueKey;
    int                                 m_nUniqueKey;
	int GetGuildMemberTotalCount();
    std::vector<KEventMonster>          m_vecEventMonster;
    bool                                m_bMonsterAction;
    int                                 m_iMonsterActionIndex;

    struct KEventDungeon 
    {
        int               m_nModeID;
        std::vector<int>  m_vecOpenDays;
    };

    struct KDetailDungeonInfo
    {
        int iGameMode;
        std::wstring wstrName;
        std::wstring wstrOpen;

        KDetailDungeonInfo()
        {
            iGameMode = -1;
            wstrName = L"";
            wstrOpen = L"";
        }
    };

    std::vector< KEventDungeon >       m_vecEventDungeonInfo;
    std::vector< KDetailDungeonInfo >  m_vecEventDungeonList;

public:
    void SetTRAvgPing( int iPing );
    int GetTRAvgPing();    
    void GetEventScheduleList();

    bool IsOpenDayEventDungeon( EGCGameMode _eGameMode );
    std::wstring GetEventDungeonOpenDayString( EGCGameMode _eGameMode );
    std::wstring GetEventDungeonOpenDayStringInfo(EGCGameMode _eGameMode);
    int GetEventDungeonListSize() { return m_vecEventDungeonList.size(); }
    void OtherWebOpen(const CHAR* strURL_);
    int GetGradeBonus(PLAYER* pPlayer);
    KGCHeroDungeonManager               m_kHeroDungeonInfo;
    DWORD                               m_dwJoinRoomLimitPing;
    float                               m_fFakeCCU;
    std::vector<EGCGameMode>            m_vecOpenedEventDungeonList;
    PromotionBonusStatus                m_kPromotionBonus;
    void SetBancapCfg(BCAP_CONFIG& pCfg);
    bool     m_bUseOldReplay;
    int      m_iAMPCharType;
    int      m_iAMPPromotion;
    GCITEMID m_iAMPItemID;
    bool     m_bShowItemInfoBox;
    bool     m_bBoxAlphaToggle;
    bool     m_bCriticalToggle;
    bool     m_bCriticalOffToggle;
    bool     m_bComboEffectShow;
    bool     m_bSpecialFaceShow;
    bool     m_bEventCreateFirst;
    bool     m_bEventCreateEnable;
    std::map <int, int> m_mapGpointDiff;
    KEVENT_ITEM_CHARPROMOTION_LEVEL_NOT  m_mapItemCharpromotionLevel;

    // 몬스터 레벨별 능력치 테이블
    std::map< int, LevelTable >                 m_mapMonLevelTable;
    // 몬스터 ID별 능력치 가중치 테이블
    std::map< int, MonsterWeightTable >         m_mapMonWeightTable;
    std::vector< float >                        m_vecBossHPRate;
    std::set< int >                             m_setBossHPDisable;

    // 일반 몬스터 HP 증가 비율 
    std::vector< float >                        m_vecNormalMonHPRate;
    std::set< int >                             m_setNormalMonHPDisable;

    void SetMonsterStat( int monId_, int level_, SMonsterTemplate& monTemplate_, bool bBoss = false );
    static void LoadMonsterStatInfo();

    void LoadAdjustLevelInfo();
    void AdjustExp( IN const int iPlayerLevel_, IN const int iMonsterLevel_, IN const int monId_, OUT float& fMonReward_ );

    int m_iAdjustMaxLevel;
    int m_iAdjustMinLevel;
    float m_fDefaultReward;
    std::map< int, float > m_mapAdjustRatio;

	bool IsQuickSkillPlayer( int iCharType_ );
    bool IsHybridPlayer( int iCharType_ );
    bool IsOneWeaponCharacter( const int iCharType_ );
    bool IsCharUseSpecial4( int iCharType_ );
    bool IsTempUseSpecial4Char( int CharType_ );
    std::map<char,std::pair<bool,GCITEMUID> > m_mapChangeWeapon; // map< 캐릭터타입, pair<잠김여부,ItemUID> >

public:
    // 서버와 접속 끊어질 때 타입들 
    enum GAME_EXIT_TYPE 
    {
        EXIT_GAME_STATE_DISCONNECT  = 0,
        EXIT_GAME_STATE_UPDATE      = 1,
        EXIT_GAME_STATE_SHUTDOWN    = 2,

        NUM_EXIT_GAME_STATE
    };

    // 길드 혜택 레벨
    enum GUILD_CONTRIBUTION_LEVEL
    {
        GUILDCL_LEVEL_NOTGUILD = -1,
        GUILDCL_LEVEL_NO_CONTIBUTION = 0,
        GUILDCL_LEVEL_EXP_5 = 1,
        GUILDCL_LEVEL_GUILD_STORE = 2,
        GUILDCL_LEVEL_RUNE_5 = 3,
        GUILDCL_LEVEL_EXP_10 = 4,
        GUILDCL_LEVEL_NECK_5 = 5,
        GUILDCL_LEVEL_GUILD_STORE_BUFF_POTION = 6,
        GUILDCL_LEVEL_POTION_SLOT_2 = 7,
		GUILDCL_LEVEL_RECOVERY_UP_POTION = 7,
        GUILDCL_LEVEL_RUNE_10 = 8,
        GUILDCL_LEVEL_NECK_10 = 9,
        GUILDCL_LEVEL_EXP_15 = 10,
    };

public:// 한국 새로운 폰트관련( 400종을 테스트 할 수 있게 해달라... )
    int m_iSelectFontIndex;
    bool m_bFontInstallSuccess;
    std::string m_strFontName;
    std::vector<std::string> m_vecFontList;
    void LoadFontList();
    void FindDirectoryAllFontFiles( char *strDirectory);

public:// 캐릭터 통계데이터
    struct CHARACTERSTATS
    {
        int iAccCount;
        int iGameMode;
        CHARACTERSTATS() : iGameMode(-1), iAccCount( 0 ) {}
    };
    std::map< std::pair<int, int>, CHARACTERSTATS > m_mapCharacterStats;
    std::set< int > m_setMIDList;
    void LoadStatsMIDList( char* strFilename_ );

    //시즌4카운터 변수
    DWORD m_dwConnetionTime;

    bool m_bServerReconnection;
    int GetGuildLevel();

public:
    bool m_bUIInfo;
    time_t          m_tFirstLoginTime;

private:
    std::map<int, float> m_mapChampionAbility;

public:
    float GetChampionAbility( int iAbilityType_ );
    void SetChampionAbility( std::map<int, float> mapAbility_ );
public:
    //Dll injecion  BlackList
    std::set<std::wstring>       m_setBlackDllList;
    std::vector<std::wstring>    m_vecExtendSHAList;
    bool                        m_bHackCheck;
    bool                        m_bFindMainFile;
    std::string                 m_strGCProcessName;
    std::string                 m_strCheckingBlackDll;

	unsigned long				m_ulGanadara;

    unsigned long               m_ulRandomCount;

	bool						m_bDamageHack;
	float						m_fBefore;
	float						m_fAter;

    KEquipItemInfo ConvertKItemtoKEquipItemInfo(KItem *pItem);

    void CheckUseTcpRelay( std::vector<int>& vecPing_ );
    void UseTCPRelay( bool bOnlyTR );
    void ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar = -1 );


    std::vector<KEventOpenInfo>     m_vecEventOpenInfo; // 이벤트 알리미 적용될 내용.
    std::set<GCITEMID>              m_setEventAlarmItemList; // 이벤트 알리미 알람 아이템 리스트.	

	GCEventAlarmManager				m_cEventAlarmManager;		

	

public:// 점령전 정보가 담겨있는 테이블
    int                                               m_iCurDotaMapID;       // 현재 선택된 맵ID
    std::vector< SDotaTemplate >                      m_vecDotaTemplate;     // 도타맵 정보
    inline bool IsDotaMapID( int iMapID )
    {
        std::vector< SDotaTemplate >::iterator vecIter = m_vecDotaTemplate.begin();
        for(; vecIter != m_vecDotaTemplate.end(); ++vecIter)
            if( vecIter->iDotaMapID == iMapID )
                return true;
        return false;
    }
    inline int GetDotaPlayer_D_Point()
    {
        std::vector< SDotaTemplate >::iterator vecIter = m_vecDotaTemplate.begin();
        for(; vecIter != m_vecDotaTemplate.end(); ++vecIter)
            if( vecIter->iDotaMapID == m_iCurDotaMapID )
                return vecIter->iPlayer_D_Point;
        return 0;
    }
    inline int GetDotaMonster_D_Point()
    {
        std::vector< SDotaTemplate >::iterator vecIter = m_vecDotaTemplate.begin();
        for(; vecIter != m_vecDotaTemplate.end(); ++vecIter)
            if( vecIter->iDotaMapID == m_iCurDotaMapID )
                return vecIter->iMonster_D_Point;
        return 0;
    }
    inline int GetDotaCore_D_Point()
    {
        std::vector< SDotaTemplate >::iterator vecIter = m_vecDotaTemplate.begin();
        for(; vecIter != m_vecDotaTemplate.end(); ++vecIter)
            if( vecIter->iDotaMapID == m_iCurDotaMapID )
                return vecIter->sCore_D_Point.iDPoint;
        return 0;
    }
    inline int GetDotaKillerMonster_D_Point()
    {
        std::vector< SDotaTemplate >::iterator vecIter = m_vecDotaTemplate.begin();
        for(; vecIter != m_vecDotaTemplate.end(); ++vecIter)
            if( vecIter->iDotaMapID == m_iCurDotaMapID )
                return vecIter->iKillerMonster_D_Point;
        return 0;
    }
    inline SDotaTemplate* GetCurrentDotaTemplate()
    {
        std::vector< SDotaTemplate >::iterator vecIter = m_vecDotaTemplate.begin();
        for(; vecIter != m_vecDotaTemplate.end(); ++vecIter)
            if( vecIter->iDotaMapID == m_iCurDotaMapID )
                break;
        if( vecIter == m_vecDotaTemplate.end() )
            return NULL;
        return &(*vecIter);
    }
    inline SDotaTemplate* GetDotaTemplate(int iMapID)
    {
        std::vector< SDotaTemplate >::iterator vecIter = m_vecDotaTemplate.begin();
        for(; vecIter != m_vecDotaTemplate.end(); ++vecIter)
            if( vecIter->iDotaMapID == iMapID )
                break;
        return &(*vecIter);
    }
public:
    std::vector<int>            m_vecAzitMotion;
    GCExtraMesh                 m_ExtraMesh;
    std::vector<int> m_vecNecklace;	

public:
    int GetCiphers( int iNum_ );    // 숫자가 몇자리 수인가??
    int IsSexCharacter( int iCharType );

public:// 던전 플레이 통계
#if defined( DUNGEONPLAYSTATE )
    enum
    {
        DUNPLAY_STATE_CLEARTIME,
        DUNPLAY_STATE_DEATH,
        DUNPLAY_STATE_LEAVE,
        DUNPLAY_STATE_SPECIAL,
        DUNPLAY_STATE_PET,
        DUNPLAY_STATE_GIVE_DAMAGE,
        DUNPLAY_STATE_RECEIVE_DAMAGE,
        NUM_DUNPLAY_STATE,
    };
    DWORD m_dwStageStartTime;
    std::map< KDungeonDetail, KDungeonPlayState > m_mapDungeonPlayState;
    void AddDungeonPlayState( int iState, int iValue = 0 );
#endif

public:
    void GetPromotionBonusStatus( IN const int& iPromotionLevel_, OUT int& iAtk_, OUT int& iDef_, OUT int& iHP_, OUT float& fCritical_, PLAYER* pPlayer_ = NULL );
    std::wstring GetStatusGrade( IN const int iTotalAtk_ );
    void applyPromotionBonuses(int statusBonus, float criticalBonus, int& iAtk_, int& iDef_, int& iHP_, float& fCritical_);

public:
    void LoadMantisProjectList();
    void GetMantisProjectList( std::map<std::wstring, std::vector<std::wstring>>& mapList ) { mapList = m_mapMantisProjectList; } 

    void SetEventMonsterList(std::map<int, KMonsterAbility> &mapList){m_mapEventMonsterAbility = mapList;}
    bool GetEventMonsterList( KMonsterAbility &kMonster, int iMonsterID  );
    bool GetMatchingKey(OUT int &iJoystickKey_, int ikeyboardKey_);
    void InitMatchingKey();
	void ReportforGCLOG(std::string strType );
	std::wstring ConvertAtoW( std::string str );
	std::string ConvertWtoA( std::wstring str );
private:
    std::map<std::wstring, std::vector<std::wstring>> m_mapMantisProjectList;	

    std::map<int, KMonsterAbility>                      m_mapEventMonsterAbility;
    std::map<int,int> m_mapMatchingKey;
	
	
private:
    // 서버로부터 받을 호위 이벤트 몬스터 정보 
    struct KGlobalEscortInfo
    {
        DWORD                 m_dwMonsterID;          // 호위할 몬스터 ID(-1: 호위몹 없음)
        int                   m_nMonsterHealth;       // 몬스터 체력
        int                   m_nTakeDamage;          // 받는 데미지(고정)
        int                   m_nInvincibleTime;      // 무적시간 (ms)
        GCITEMID              m_PotionID;             // 회복 아이템
        std::vector< int >    m_vecStrNum;            // 안내 텍스트 번호

        bool                  m_bIsActiveEscort;      // 게임시작시 이벤트 기간인지 서버로 받아서 확인

        bool                  m_bIsSummoned;          // 소환 했니?

        KGlobalEscortInfo()
        {
            m_dwMonsterID = -1;
            m_nMonsterHealth = 0;
            m_nTakeDamage = 0;
            m_nInvincibleTime = 0;
            m_PotionID = -1;
            m_vecStrNum.clear();
            m_bIsActiveEscort = false;
            m_bIsSummoned = false;
        }
    };

    //< MonsterID , MonsterInfo >
    std::map<DWORD,KGlobalEscortInfo>   m_mapEscortInfo;

    KGlobalEscortInfo m_kEscortInfo;

public:
    bool                 IsInEscortMonsterInfo( DWORD dwMonserID_ );
    void                 SetEscortInfo( KEVENT_ESCORT_INFO_NOT& kEscortInfo_ );
    DWORD                GetEscortMonsterID(){ return m_kEscortInfo.m_dwMonsterID; }
    int                  GetEscortMonsterHP(){ return m_kEscortInfo.m_nMonsterHealth; }
    int                  GetEscortMonsterFixedDamage(){ return m_kEscortInfo.m_nTakeDamage; }
    int                  GetEscortMonsterInvincibleTime(){ return m_kEscortInfo.m_nInvincibleTime; }
    GCITEMID             GetEscortPotionID(){ return m_kEscortInfo.m_PotionID; }
    std::vector< int >&  GetEscortString(){ return m_kEscortInfo.m_vecStrNum; }
    void                 SetActivateEscortEvent( bool bActive ){ m_kEscortInfo.m_bIsActiveEscort = bActive; }
    bool                 IsEscortEventActive(){ return m_kEscortInfo.m_bIsActiveEscort; }
    void                 SetEscortMonsterSummonState( bool bSummoned ){ m_kEscortInfo.m_bIsSummoned = bSummoned; }
    bool                 IsEscortMonsterSummoned(){ return m_kEscortInfo.m_bIsSummoned; }

public:
    void SetSSkillHotKeyUsedInfo( bool bUseHotKey_, char cSkillLevel_ );
    void CountSSkillHotKeyUsedInfo( std::map< char, std::map< char, int > >& mapInfo_, char cSkillLevel_ );
    void ClearSSkillHotKeyUsedInfo();
    bool IsEmptySSkillHotKeyUsedInfo();
    KEVENT_STAT_HOTKEY_USED_INFO_NOT GetSSkillHotKeyUsedInfo() { return m_sSSkillHotKeyStatistics; }
    void threadReportforGCLOG( );
public:
    bool                SearchProcess( const std::wstring& strURL );

// 데미지 테스트 관련 
#if !defined( __PATH__ )
private:
    float   m_fDamageRaito;

public:
    void    SetDamageRaito( float fRaito_ ) { m_fDamageRaito = fRaito_; }
    float   GetDamageRaito( void ) { return m_fDamageRaito; }
#endif

private:
    KEVENT_STAT_HOTKEY_USED_INFO_NOT m_sSSkillHotKeyStatistics;

	std::string m_strLogType;

private:
    // 강화 이펙트 정보 저장 구조체 
    struct EnchantEffectInfo
    {
        std::string strSquence;         // 이름 
        D3DXVECTOR2 dxvGamePos;         // 게임에서 출력 위치 
        D3DXVECTOR2 dxvRoomMyPos;       // 대기방 내 캐릭터 출력 위치 
        D3DXVECTOR2 dxvRoomOtherPos;    // 대기방 상대 캐릭터 출력  위치 
        int         iParticleNum;       // 파티클 수 
        float       fTraceTime;         // 따라다니는 시간 
        bool        bRenderGame;        // 게임 안에서 출력 할것인가?
        bool        bRenderRoomMy;      // 대기방 내 캐릭터에 출력 할 것인가?
        bool        bRenderRoomOther;   // 대기방 상대 캐릭터에 출력할 것인가?
    };

    // 강화 이펙트 정보 저장 < 이펙트 그룹 , < 이펙트 레벨, 이펙트 정보 > > 
    std::map< EGCEnchantColorGroup, std::map< EGCEnchantEffectLevel, std::vector< EnchantEffectInfo > > >   m_mapEnchantEffect;

public:
    bool LoadEnchantEffectInfo( const char* strFileName_ );


	std::map<int , std::vector<std::string>>	m_mapCharGainParticleInfo;
	void LoadCharGainParticleInfo ( const char* strFileName_ );
    void AddEnchantEffect( const EGCEnchantColorGroup eEffectGruop_, const EGCEnchantEffectLevel eEffectLevel_, PLAYER* pPlayer_, 
                           float fX_ = 0.0f, float fY_ = 0.0f, int iLayer = 0, std::vector<CParticleEventSeqPTR>* pVecParticle_ = NULL,
                           EGCEnchantEffectPos ePos = ENCHANT_EFFECT_ROOM_MY );

private:
    int     m_aiSelectMotionID[CID_MAX];

public:
    void SetSelectMotionID();
    int GetCharacterSelectMotionID( int iExtra_Char_Num_ );

    float GetMaxMP();

    bool IsExpantionManaCurChar(); //현재 캐릭터가 마나 확장 스킬을 익히고 있는 체크한다.

public:
	KSpecialMonster m_kSpecialMonster;    

public:
    int m_iPrevCheckMotion;
    int m_iCurReserveIndex;
    bool m_bStartCollectDamageList;
    bool m_bPrevIsRight;
    std::map< char, std::set< int > > m_mapPlayerDamageList;    
    std::vector< int > m_vecCurMotionList;

    bool RunPlayerAllMotion();
    void SavePlayerMotionDamageList( int iDamageID );
    void InitDamageCheckMotionInfo();

    void CheckParentDamage();

    bool m_bStartFrameCheck;
    std::wstringstream m_wstmFrame;

    void WriteCollectedFrameData();
    void StartCollectFrameData();
    void LoadNecklaceScript();	
    bool IsTutorialMode();
public:
    int m_iSendTimeForPacket;
    KncStopWatch m_kSendTimeForPacket;
    KAddSendPacket m_kSendPacket;
    std::vector<KAddSendPacket> m_vecSendPacket;

public:
    std::pair< int, int > m_prHybridSpecialUseCount; // 하이브리드 캐릭터 필살기 사용 유형 통계

    int m_iGetCharType; // 신 캐릭터 획득 띄워줄려면 캐릭터 타입 넣어주세영 

    // 신스킬트리 구 스킬트리 구분 함수
public:
    bool CheckTrainedSkill( PLAYER* pPlayer, EGCSkillTree eSkill );
    bool IsLearnedSkill( char cCharType, EGCSkillTree eSkill );    
    const GCSkill* GetSkill( char cCharType, EGCSkillTree eSkill );
    void SetPlayerSkillInfo( PLAYER* pPlayer, bool bUnlockAll, bool bSelectableChar, bool bSelectablePromotion );
    int GetOpenSkillSlotIndex( char cCharType );


public:
    bool m_bJPGScreenShot;  // 스샷 JPG/BMP 전환 

public:
    std::map< int, KSealedCharInfo >    m_mapSealedCharInfo;    // 캐릭터 봉인 정보
    std::vector<int> m_vecDisableUIBtnList;

public:
    int  m_iShutDownType;
    int  GetShutDownInfoType( const int iShutdownCheckResult_ );  
    void AddShutDownMsg( const KShutdownInfo::EShutdownInfo eType_, const unsigned int iTime_ );
    void SetDisableUIBtnList();
#if defined( DUNGEONPLAYSTATE )
    kDungeonPerformanceState    m_kDunPerforState;
#endif

private:
	int m_iInventoryPacketMaxCnt;
public:
	bool IsSuccessRecvInventoryPacket();
	bool IsSuccessRecvInventoryPacket( int iCurrentCnt );
	void SetInventoryPacketMaxCnt(int iCnt){m_iInventoryPacketMaxCnt = iCnt;}

private:
	std::map<int, KInDoorUserInfo> m_mapJoinRoomUserInfo;
    std::vector<KInDoorUserInfo> m_mapJoinRoomBroadUserInfo;
public:
	void AddJoinRoomUserInfo(int iIndex,KInDoorUserInfo &kInfo){m_mapJoinRoomUserInfo.insert(std::make_pair(iIndex, kInfo));}
    void AddJoinRoomBroadUserInfo( KInDoorUserInfo &kInfo ){m_mapJoinRoomBroadUserInfo.push_back( kInfo );}
	std::map<int, KInDoorUserInfo> GetJoinRoomUserInfo( ){return m_mapJoinRoomUserInfo;}
    std::vector<KInDoorUserInfo>  GetJoinRoomBroadUserInfo( ){return m_mapJoinRoomBroadUserInfo;}
	void ClearJoinRoomUserInfo( ){m_mapJoinRoomUserInfo.clear();m_mapJoinRoomBroadUserInfo.clear();}

public:
    bool bItemCrafting;

public:
    bool m_bNoCameraZoom;  // 카메라 줌기능 사용 금지처리
	int GetStatusGradeType( IN const int iAtk_, IN const int iDef_, IN const int iHP_ );
    bool RunLogProgram(std::string arg, std::string NPerr = "0");
    bool RunExecute( std::string filename, std::string param );

public:// 몬스터 상태 통계
    KMonsterTypeStat m_kMonsterTypeStat;

public:// HP,MP회복 치트키
    bool m_bToggleRecoverHP;
    bool m_bToggleRecoverMP;

// 슈퍼가챠 버전 정보 
public:
    std::vector< KGachaNewList >   m_vecGachaUseVersion;
    bool EnableGacha( void ) { return m_vecGachaUseVersion.empty() == false; }
    bool IsSingleGacha( void ) { return ( EnableGacha() && static_cast<int>(m_vecGachaUseVersion.size()) == 1 ); }
    int  GetSingleGachaVer( void );

public:
    bool GetPingData( std::string strIP, DWORD& dwTime ); //  접속하지 않은 서버의 Ping 정보 가져오기

public:
    PLAYER_ATTACK_ATTRIBUTE GetSpearType( const int iPlayerIndex_ );
    void SetSpearEffefct( LTP_BASIC* pInfo_ );

public:
    std::vector< GCITEMID >     m_vecVestedItemList;    // 귀속 아이템 리스트 
    bool IsVestedItem( const GCITEMID itemID_ );

	bool m_bSendServerInfoNot;


public:
    bool m_bIsGetWebCashOnStateMyInfoFirstInit; // 북미 전용, 장비창 첫 진입시 웹캐시 잔액정보 가져오도록 사용하기 위해 추가
    int m_iLocalPlayerIndex;

public:
    D3DXVECTOR2 vTempTexRenderPos;
    GCDeviceTexture* m_pTempTex;
    bool bRenderTempTexture;


// 속성 변경 이벤트 관련 값 
private:
    KEVENT_ATTRIBUTE_MIGRATION_INFO_ACK m_kAttributeMigrationInfo;

public:
    void SetAttributeMigrationInfo( const KEVENT_ATTRIBUTE_MIGRATION_INFO_ACK& Info_ );
    bool IsEnableAttributeMigration( void );
    int  GetEnableAttributeMigrationCount( void );
    bool GetMigrationChangeEnableAttributeList( OUT std::set< int >& List_ );
    bool GetMigrationItemCurrentAttribute( OUT std::vector< KAttributeInfo >& Info_ );
	
public:
    bool m_bPrintTG;
    std::string m_strBeforeTG;
	
    bool m_bPatcherSelectServer;
    std::string m_strPatcherSelIP;
    std::string m_strPatcherSelPort;
	std::string m_strUser;
	std::string m_strPass;

    bool m_bVersion;
    std::wstring m_wstrVersion;
#if defined( NATION_CHINA ) && !defined(__PATH__)
    std::wstring m_strRelayServer;
#endif

private:
    bool m_bSmartPanelMailListEnable;
    std::vector< std::wstring > m_vecSmartPanelMailList;
    std::string m_strChannerImgClickURL;

public:
    void SetChannerImgClickURL( IN std::wstring strChannelImageClickURL_ ) { m_strChannerImgClickURL = KncUtil::toNarrowString(strChannelImageClickURL_); }
    std::string GetChannerImgClickURL() { return m_strChannerImgClickURL; }

    // 활력 데이터
    KEVENT_VITALITY_INFO_NOT m_kVitalityInfo;
    int m_iPrevRemainVitality;
    std::map< PAIR_INT, KVitalityCheckDungeon > m_mapVitalityDungeonInfo;

    bool UseVitalitySystem();
    int GetVitality( IN int nCharType );
    int GetMaxVitality();
    int GetVitalityRemainRechargePoint();
    void SetUpdateVitalityInfo( IN KEVENT_VITALITY_INFO_NOT& kRedv_ );

    GCITEMID m_CharSoltAddItem;
	
private:
    DWORD   m_dwCurrentServerUID;

public:
    void    SetCurrentServerUID( const DWORD dwUID_ ) { m_dwCurrentServerUID = dwUID_; }
    DWORD   GetCurrentServerUID( void ) const { return m_dwCurrentServerUID; }	
		
private:
    std::map<int, MoveModelData> m_mapMoveModelData;
    int m_nMoveModelCount;
    int m_iStartModelIdServerState;

public:
    static void LoadMoveModelData();
    bool GetMoveModelData(IN int iID, OUT MoveModelData& ModelData);
    int GetMoveModelCount() { return m_nMoveModelCount; }
    int GetStartModelIdServerSate() { return m_iStartModelIdServerState; }

private:
    int m_iPreContinent;
public:
    void SetPreContinent(int econtinent){ m_iPreContinent = econtinent; }
    int GetPreContinent() { return m_iPreContinent; }

private:
    std::map<DWORD, KGachaPongMachineInfo> m_mapGachaPonMachineInfo;

public:
    struct GachaPonMachine
    {
        DWORD m_dwID;
        DWORD m_dwLeftCapsule;
        DWORD m_dwRewardCnt;
        GCITEMID m_dwRewardID;

        GachaPonMachine()
        {
            m_dwID = -1;
            m_dwLeftCapsule = 0;
            m_dwRewardCnt = 0;
            m_dwRewardID = 0;
        }
    };

    GachaPonMachine m_stGachaPonMachine;

    void SetGachaPonMachineInfo(std::map<DWORD, KGachaPongMachineInfo>& mapGachaPonMachineInfo);
    void UpdateGachaPonMachine(std::pair<DWORD, DWORD>& prMachineInfo, DWORD dwLeftCapsule, DWORD dwRewardCnt);
    bool GetGachaPonMachineInfo(DWORD dwMachineNumber, KGachaPongMachineInfo& machineInfo);
    int GetGachaPonMachineCnt() { return m_mapGachaPonMachineInfo.size(); }

public:
    struct ShortMenuCharInfo
    {
        std::string					m_strFootholdModel;
        std::string					m_strFootholdTexture;
        std::string					m_strFootholdMotion;
        std::string					m_strBackgroundTexture;
        std::vector<std::string>	m_vecMotion;
    };

    KSafeArray<ShortMenuCharInfo, GC_CHAR_NUM> m_saShortMenuInfo;

    static void LoadShortMenuInfo();
    ShortMenuCharInfo GetShortMenuInfoForCharType(int iCharType);
};

struct KAgitFootHold
{
    int     iPosX;
    int     iPosY;
    DWORD   dwProp;

    KAgitFootHold( void )
    {
        iPosX = 0;
        iPosY = 0;
        dwProp = 0;
    }
};

namespace GC_GLOBAL_DEFINE
{
    void InitGlobalDefine();
    void LoadGlobalDefine();

    extern KProtectedType<BOOL> bReformCoordi;      //  코디 개편 여부
    extern KProtectedType<BOOL> bWeaponCoordi;      //  무기 코디만 사용( 코디개편에서 분리 )	

    extern KProtectedType<BOOL> bCoordiCompose;     // 코디 합성 사용 여부   
    extern KProtectedType<BOOL> bCoordiUpgrade;     // 코디 등급 업그레이드 사용 여부

    extern KProtectedType<BOOL> bRelayHeroDungeon;      //  릴레이 영웅던전 여부

    extern std::wstring         strSearchURL;
    extern KProtectedType<BOOL> bJustInTimeSearch;

	extern KProtectedType<BOOL> bGuildLevel7;      //  길드레벨 7단계 보상 수정
	extern std::wstring         strBingoHintURL;	//	빙고 힌트 검색 URL
	extern KProtectedType<BOOL> bMonsterCardMix;   //  몬스터 카드 조합	

    extern KProtectedType<BOOL> bEventAlarm;  //  이벤트 알람		
	
    extern KProtectedType<int>  nShopBeginShowTab; // 샵에서 보여질 첫번째 탭	
	
	extern KProtectedType<BOOL> bLoadGameModeScript;	//  게임모드 스크립트에서 로드 ( 사내만 동작 )	
	
    extern KProtectedType<BOOL> bCheckStageClearTime;	

    extern KProtectedType<int>  iLevelItemAlram;        // 레벨 달성 아이템 지급 기준 레벨 
    
    extern KProtectedType<BOOL> bAutoMatch4Special;         // 대전매칭 4필 옵션
    extern KProtectedType<BOOL> bAutoMatchUsePetSpecial;    // 대전매칭 펫필 옵션
    extern KProtectedType<BOOL> bUseAutoMatchBalance;       // 대전매칭 밸런스 대전에서 사용여부

    extern KProtectedType<int>  nCoordiInven;         //  코디 인벤 사이즈
    extern int nDangerHPPercent; //HPPercentage For DangerAlarm

    extern KProtectedType<BOOL> bThreadLoadTexture;     // 쓰레드 텍스쳐로드 ( 전체 컨트롤 ) 
    extern KProtectedType<BOOL> bThreadLoadAnimTex;     // 쓰레드 Anim 로드	
    extern KProtectedType<BOOL> bThreadLoadBuffTex;     // 쓰레드 Buff 로드	
    extern KProtectedType<BOOL> bRealTimeLoadEtcTex;      // 기타 실시간 텍스쳐
    extern KProtectedType<BOOL> bRealTimeLoadMotion;      // 기타 실시간 모션 파일
    
    extern KProtectedType<BOOL> bAutoMatchRanking;      // 대전매칭 랭킹 시스템	
    extern KProtectedType<BOOL> bAutoMatch;             // 대전 매칭 시스템

    extern KProtectedType<BOOL> bMatchBalance2014;      // 2014년 대전밸런스
}

namespace GC_GLOBAL_CONST
{
    void InitGlobalConst();

    extern KProtectedType<float> fPetMPRecovery;
}

struct KBookData
{
    GCITEMID        m_dwItemID;
    int             m_strDesc;
    std::wstring    m_strImgFileName;
    int             m_strAudio;

    KBookData(void)
    {
        m_dwItemID = -1;
        m_strDesc = 0;
        m_strImgFileName = L"";
        m_strAudio = 0;
    }
};


void CashRechargeWebOpen();
void PopUpWebPage( const WCHAR* strURL_ );
//void SendGameBoardPacket();

//void  SetBit( BYTE* pByte_, int iIndex_, const bool& bBool_ );
extern KGCGlobal g_kGlobalValue;

#define CRASH_MSG( m ) g_kGlobalValue.m_strCrashMsg = m;

#endif //_GCGLOBAL_H_
