#ifndef _MONSTER_H_
#define _MONSTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "GCEnum.h"
#include <GCDeviceLib/IGCComputePaletteP3M.h>
#include "GCCollisionRect.h"
//#include "GCGlobal.h"
#include "./animation/KGCAnimManager.h"
//#include "Player.h"
#include "MyD3D.h"
#include "KGCObj.h"
#include "HardAI/HardAI_Dota.h"
#include "./KncP2PLib/KncCommonDefine.h"
#include "gcui/GCRealMiniMap.h"
#include "Controls.h"
#include "GCUI\KGCPrograssBar.h"

class HardAI_Core;
class PLAYER;
class GCEffectsSword;
class KQuestGameManager;

//#include <boost/mpl/int.hpp>
//#include <boost/mpl/plus.hpp>
//#include <boost/mpl/minus.hpp>
//#include <boost/mpl/multiplies.hpp>


#define LATENCY_BUFFER_SIZE         (boost::mpl::plus<\
										boost::mpl::int_<LATENCY>,\
										boost::mpl::int_<SHOWLATENCY>\
									>::type::value)
#define LATENCY_INDEX               (boost::mpl::int_<LATENCY_BUFFER_SIZE>::prior::value)
#define MONSTER_FRICTIONAL_FORCE    ( 0.0005f )

#define KAMIKI_LIGHTNING_COUNT      ( 10 )

typedef bool (*DropItemObserver)(int, D3DXVECTOR2);



struct KMonsterFootHold
{

    int m_iBaseBone;
    int m_iEndBone;
    DWORD m_dwProp;

    float m_fRelativeX;
    float m_fRelativeY;

    std::vector< D3DXVECTOR2 > m_vecFootHoldPos;
};

template <class T>
class GCPushRect : public GCCollisionRect <T>
{
    public:
    GCPushRect()
    {
        iBoneId = 0;
        fWidth = 0.0f;
        fHeight = 0.0f;
        fOffsetX = 0.0f;
        fOffsetY = 0.0f;
        bActive = true;
    }

    ~GCPushRect()
    {
    }


    int iBoneId;
    float fWidth;
    float fHeight;

    float fOffsetX;
    float fOffsetY;

    bool bActive;

public:		// Operators
    GCPushRect& operator = ( const GCPushRect<T>& right )
    {
        m_Left = right.m_Left;
        m_Top  = right.m_Top;
        m_Right = right.m_Right;
        m_Bottom = right.m_Bottom;

        iBoneId = right.iBoneId;
        fWidth = right.fWidth;
        fHeight = right.fHeight;

        fOffsetX = right.fOffsetX;
        fOffsetY = right.fOffsetY;
        return *this;
    }
    bool operator == ( GCPushRect<T>& right )
    {
        if( m_Left		!= right.m_Left ) return false;
        if( m_Top		!= right.m_Top  ) return false;
        if( m_Right		!= right.m_Right  ) return false;
        if( m_Bottom	!= right.m_Bottom  ) return false;
        if( iBoneId	    != right.iBoneId  ) return false;
        if( fWidth	    != right.fWidth  ) return false;
        if( fHeight	    != right.fHeight  ) return false;
        if( fOffsetX	!= right.fOffsetX  ) return false;
        if( fOffsetY	!= right.fOffsetY  ) return false;
        
        return true;
    }
    bool operator > ( GCPushRect<T>& right )
    {
        if( *this == right )
            return false;

        if( m_Left > right.m_Left ) return false;
        if( m_Right < right.m_Right ) return false;
        if( m_Top < right.m_Top ) return false;
        if( m_Bottom > right.m_Bottom ) return false;

        return true;
    }
    bool operator < ( GCPushRect<T>& right )
    {
        return right > *this;
    }
};


template <class T>
class GCAttackRect : public GCCollisionRect <T>
{
    public:
    GCAttackRect()
    {
        iBoneId = 0;
        fWidth = 0.0f;
        fHeight = 0.0f;
        fDelta = 0.0f;
		OffsetX = 0.0f;
		OffsetY = 0.0f;
    }

    ~GCAttackRect()
    {
    }


    int iBoneId;
    float fWidth;
    float fHeight;
    float fDelta;
	float OffsetX;
	float OffsetY;
public:		// Operators
    GCAttackRect& operator = ( const GCAttackRect<T>& right )
    {
        m_Left = right.m_Left;
        m_Top  = right.m_Top;
        m_Right = right.m_Right;
        m_Bottom = right.m_Bottom;

        iBoneId = right.iBoneId;
        fWidth = right.fWidth;
        fHeight = right.fHeight;
        fDelta = right.fDelta;
		OffsetX = right.OffsetX;
		OffsetY = right.OffsetY;

        return *this;
    }
    bool operator == ( GCAttackRect<T>& right )
    {
        if( m_Left		!= right.m_Left ) return false;
        if( m_Top		!= right.m_Top  ) return false;
        if( m_Right		!= right.m_Right  ) return false;
        if( m_Bottom	!= right.m_Bottom  ) return false;
        if( iBoneId	    != right.iBoneId  ) return false;
        if( fWidth	    != right.fWidth  ) return false;
        if( fHeight	    != right.fHeight  ) return false;
        if( fDelta	    != right.fDelta ) return false;
		if( OffsetX	    != right.OffsetX  ) return false;
		if( OffsetY	    != right.OffsetY ) return false;

        return true;
    }
    bool operator > ( GCAttackRect<T>& right )
    {
        if( *this == right )
            return false;

        if( m_Left > right.m_Left ) return false;
        if( m_Right < right.m_Right ) return false;
        if( m_Top < right.m_Top ) return false;
        if( m_Bottom > right.m_Bottom ) return false;

        return true;
    }
    bool operator < ( GCAttackRect<T>& right )
    {
        return right > *this;
    }
};


struct KMonsterSwoosh
{
    int m_iBaseBone;
    int m_iEndBone;

    GCEffectsSword* m_pEffect;

    GCDeviceTexture* m_pTexture;

};

struct KMonsterPhysicAttack
{
    KMonsterPhysicAttack( int iBaseBone, int iEndBone, float fRange, int iDamage, int iDamageType, int iLifeTime )
        :m_iBaseBone(iBaseBone),
        m_iEndBone(iEndBone),
        m_fRange(fRange),
        m_iDamage(iDamage),
        m_iDamageType(iDamageType),
        m_iLifeTime(iLifeTime)
    {
        
    }
    int m_iBaseBone;
    int m_iEndBone;
    float m_fRange;
    int m_iDamage;
    int m_iDamageType;
    int m_iLifeTime;
    
};

struct KParticleArg
{
    std::string strParticleName;
    float fOffsetX;
    float fOffsetY;
    float fDirPosX;

    KParticleArg()
    {
        strParticleName = "";
        fOffsetX = 0;
        fOffsetY = 0;
        fDirPosX = 0;
    }

    void SetName( char* strName ) { strParticleName = strName; }
    void Init()
    {
        strParticleName = "";
        fOffsetX = 0;
        fOffsetY = 0;
        fDirPosX = 0;
    }
};

struct KSummonInfo 
{
    int iMonType;
    int iCountValue;
    float fX;
    float fY;
    bool bCheckSafePostion;
    std::string strState;

    KSummonInfo()
    {
        strState    = "";
        fX          = 0;
        fY          = 0;
        iMonType    = -1;
        iCountValue = 0;
        bCheckSafePostion = true;
    }

    void SetState( char* state ) { strState = state; }
    void Init()
    {
        strState    = "";
        fX          = 0;
        fY          = 0;
        iMonType    = -1;
        iCountValue = 0;
        bCheckSafePostion = true;
    }
};

struct KInfinitySummonInfo 
{
    int iMonType;
    int iCountValue;
    int iTGid;
    int iLevel;
    float fX;
    float fY;
    bool bCheckSafePostion;
    bool bBoss;
    std::string strState;

    KInfinitySummonInfo()
    {
        strState    = "";
        fX          = 0;
        fY          = 0;
        iMonType    = -1;
        iCountValue = 0;
        iTGid       = -1;
        iLevel      = 0;
        bCheckSafePostion = true;
        bBoss       = false;
    }

    void SetState( char* state ) { strState = state; }
    void Init()
    {
        strState    = "";
        fX          = 0;
        fY          = 0;
        iMonType    = -1;
        iCountValue = 0;
        iTGid       = -1;
        iLevel      = 0;
        bCheckSafePostion = true;
        bBoss       = false;
    }
};

struct KBoundBox
{
    float fTop;
    float fLeft;
    float fRight;
    float fBottom;

    KBoundBox()
    {
        fTop = 0.0f;
        fLeft = 0.0f;
        fRight = 0.0f;
        fBottom = 0.0f;
    }
};

struct sParticleModel
{
    sParticleModel()
    : pSequence( NULL )
    , m_iBornID( 0 )
    , m_fOffsetX( 0.0f )
    , m_fOffsetY( 0.0f )
    , m_fHpMinRadio( 0.0f )
    , m_fHpMaxRadio( 0.0f )
    , m_bNoDirection( false )
    {}

    int   m_iBornID;
    float m_fOffsetX;
    float m_fOffsetY;
    float m_fHpMinRadio;
    float m_fHpMaxRadio;
    bool m_bNoDirection;

    std::string strParticleName;
    CParticleEventSeqPTR pSequence;
};

enum EGCNAMESIZE
{
    SMALL_SIZE      = 0,
    NORMAL_SIZE     = 1,
    BIG_SIZE        = 2,
	SMALL_SIZE_CHAT = 3,
};

enum EGCMonNPCType
{
    // NPC는 인디케이터 발동 안시킴 ㅋㅋ 
    NPC_NONE        = 0,        // NPC 아님
    NPC_AGAINST     = 1,        // 우리편 이름 색깔
    NPC_OUR         = 2,        // 적편 이름 색깔
};

class GCObject;
class CDamageInstance;

class MONSTER : public KGCObj
{
public:
    void* operator new(size_t s){return bpool.malloc();}
    void operator delete(void *p){bpool.free(p);}
    static boost::pool<> bpool;

public:
	enum BOSSTYPE
	{
		MON_JAKO=0,
		MON_BOSS=1,
		MON_MIDBOSS=2,
	};

    enum DAMAGEDIRECTION
    {
        DR_NONE     = 0,
        DR_RIGHT    = 1,
        DR_LEFT     = 2,
    };

    enum ECHAMPIONABILITYRATE
    {
        ECHAMPION_ATTACK_RATE = 1,
        ECHAMPION_DEFENCE_RATE = 2,
        ECHAMPION_VITALITY_RATE = 3,
    };

    enum EMONSTERKIND {
        EMON_NORMAL = 0,
        EMON_AGIT_CHAR,
        EMON_ESCORT,
    };

	enum EMonsterRewardInfo
	{
		EMonsterExp     = 0,
		EDungeonExp     = 1,

		NUM_MONSTER_REWARD,
	};

	enum EMonsterSummonby {
		MEMS_NOT_SUMMON_MONSTER = 0,
		MEMS_SUMMON_MONSTER_BY_PLAYER = 1,
		MEMS_SUMMON_MONSTER_BY_MONSTER = 2,
        MEMS_SUMMON_MONSTER_BY_TRIGGER = 3,  // 전장 몬스터 소환 타입 
	};


public:
    MONSTER();
    ~MONSTER();
    static void RegisterLuaBind();
    bool StartMonster( EGCMonster eMonType, int iLevel = -1, DWORD dwChampionProperty = 0x00000000, int bBoss = MON_JAKO, bool bUseItemList = false  );
    void EndMonster( bool bBlink_ = true, bool bRenderOff = false, bool bDirectDie = false );
    void SetHPALLMonster( float fHP, int iTeam );
    void Init();
    void InitBuffer( float fX, float fY, bool bRight );
    bool Wait_Room( int iMonsterType = -1, float fX = -1.0f, float fY = -1.0f, bool bRight = false, bool bLoadModel_ = true,
                    int iLevel = -1, DWORD dwChampionProperty = 0x00000000, int iBoss = MON_JAKO, bool bUseItemList = false );
    void FrameMove();
    void CallLuaFucn_MoveFail();
    void CrashCheck(); // 몬스터 충돌체크
	void PushCheckWithMonster();
    bool CheckPierceBlockXPos(float fBeforeX, float fAfterX, float fY);
    bool CheckAttack();
    bool AttackPlayer( GCCollisionRect<float> rtWeapon, int iIndex, KMonsterPhysicAttack &kPhsAtk, bool &bSuccessAttack );
    bool AttackMonster( GCCollisionRect<float> rtWeapon, int iIndex, KMonsterPhysicAttack &kPhsAtk, bool &bSuccessAttack );
    void PlaySound( char* strSound, int loop = 0 );
    float FloatAbs( float fValue ) { return ASMfabs( fValue ); }
    float FloatRand(){ return randf(); }
    void StartText( char* strText );
    void EffectDust( float fX, float fY, int iLatency = 0 ); // 먼지 생성
    int GetMonsterKind() const { return m_iMonsterKind; }
    void SetMonsterKind(int val) { m_iMonsterKind = val; }
    void SetMissionMonster();
    float ChangeHP( CDamageInstance* pDamageInstance, float fDamage, int iPlayerIndex, bool bSpecialAtk, bool bPetDamage, int nSpecialGrade, int iMonsterIndex_ = -1, bool bIsDarkSpearDamage = true, bool bIsNoCritDmg = false );
    float GetDamageFromPlayerAttack( float fDamage, int iPlayerIndex, bool bSpecialAtk, float fSummonsOwnerAtkRatio_ = -1.f );

    D3DXMATRIX* ComputePositionBone(D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& vecPositionBone );
    virtual D3DXMATRIX* ComputeAngleBone( D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic );

    void ReadyToRender();
    void ShadowRender();    
	bool IsRenderAlpha();
	void HPGaugeRender();
	void HPFrontRender();
	void TextOutLineRender();
	void TextRender();
    void ToggleMonsterInfo( bool bToggle ) { m_bToggleInfo = bToggle; }
    bool IsToggleInfo() { return m_bToggleInfo; }
    void SetTargetingDelayTime(DWORD dwTargetingDelayTime) { m_dwTargetingDelayTime = dwTargetingDelayTime; }
    void SetMovement(bool bMovement) { m_bMovement = bMovement; }
    bool IsMovement(void) { return m_bMovement; }
    bool IsLastBottom();
    bool IsLive() {   return m_bLive;     }
    int GetOvejarCloneDeathCount() { return g_kGlobalValue.m_iOvejarCloneDeathCount; }
	void SetLive( bool bLive ) { m_bLive = bLive; }
    void SetState( const char* strState, bool bDirectMotion_ = false );
    std::string GetState();
    bool CheckState( char* strState );
    bool CheckPreState( char* strState );
    void DirectMotionInput( const char* strState );
    bool SetLatencyX( int iIndex, float fPos );
    bool SetLatencyY( int iIndex, float fPos );
    int GetFrame();
	int GetMonsterIndex()   {   return m_iMonsterIndex; }
    float GetHP(){ return (float)m_fHP; }
    void SetHP( float fHP_ );
    float GetMaxHP();
    void SetMaxHP(float fHP);
    float GetAttackPoint();
    void SetAttackPoint( float fAttackPoint );
    void ShowExp();
    void SetAttackBoundBox( int iWidth, int iHeight, int iIndex );
    void SetPushBoundBox( int iWidth, int iHeight, int iIndex );
    int GetMonsterNameID();
    const WCHAR* GetMonsterName();
    void StartAttack( int iAttackDamage, int iAttackType, float fRange, int iBaseBone_ = 0, int iEndBone_ = 0, int iLifeTime_ = 0 );
    void EndAttack();
    void RegistReaction( int iAttackType, int iAttackDir, char* strState );
    bool OnDamage( int iPlayerIndex, int iWhat, float fAttackPower, EGCAttackType eAttackType, EGCAttackDirection eAttackDirection,
        float fPushTargetX, float fPushTargetY, bool bSpecialAttack, int nSpecialGrade, bool bSpecialPoint = true, bool bArrowDamage = false, 
        bool bPetDamage = false, bool bSuperDamage_ = false, float *pDamage = NULL, bool bNoReAction = false, int iMonsterIndex_ = -1, bool bIsDarkSpearDamage = true, bool bIsNoCritDmg = false );
    
    void OnLand();
    void OnDie();
    void ActionCallBack( std::string strAction );
    //bool ActionCallBack( std::string strAction );
    bool IsBackAttack(){ return m_bIsBackAttack; }
    bool IsSpecialAttack(){ return m_bIsSpecialAttack; }
    void ResetDelay(){ m_bResult = true; }
    bool GetMovableMonster() const { return m_bMovableMonster; }
    void SetMovableMonster(bool val) { m_bMovableMonster = val; }
    void SetNoCheckContact( int iNoCheckContact ){ m_iNoCheckContact = iNoCheckContact; }
    void SetIgnoreFHCheck( bool bIgnore ) { m_bIgnoreFHCheck = bIgnore; }
	int GetLevel() const { return m_iLevel; }
    float GetX(){ return m_afX[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0 ]; }
    float GetY(){ return m_afY[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0 ]; }
    float GetRenderX(){ return m_afX[0]; }
    float GetRenderY(){ return m_afY[0]; }
    void SetX( float fX ){ SetLatencyX( g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0, fX ); }
    void SetY( float fY ){ SetLatencyY(  g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0 , fY ); }
    void SetSpeedX( float fSpeedX ){ m_fSpeedX = fSpeedX; }
    void SetSpeedY( float fSpeedY ){ m_fSpeedY = fSpeedY; }
    float GetSpeedX(){ return m_fSpeedX; }
    float GetSpeedY(){ return m_fSpeedY; }
    bool GetFrameLock(){ return m_bFrameLock; }
    void SetFrameLock( bool bFrameLock = true ){ m_bFrameLock = bFrameLock; }
    bool GetIsContact(){ return m_bIsContact; }
    void SetIsContact(bool bContact){ m_bIsContact = bContact; }
    bool GetIsContactByPos();
	float GetWalkSpeed(){ return m_abIsRight[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0  ] ? (m_fWalkSpeed+m_fAddWalkSpeed) : ( -1 ) * (m_fWalkSpeed+m_fAddWalkSpeed); }
	float GetRunSpeed(){ return m_abIsRight[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0  ] ? (m_fRunSpeed+m_fAddRunSpeed) : ( -1 ) * (m_fRunSpeed+m_fAddRunSpeed); }
    bool GetIsRight(){ return m_abIsRight[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0  ]; }
    void SetIsRight( bool bIsRight ){ m_abIsRight[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0  ] = bIsRight; }
    void Turn(){ m_abIsRight[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0  ] = !m_abIsRight[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0  ]; }
    int GetLastFrame();
    int GetLastFrame_( int iIndex );
    int GetPushRectNum() { return m_vecPushRect.size(); }
    int GetAttackBoundRectNum() { return m_vecAttackRect.size(); }
    GCCollisionRect<float>& GetAttackBoundRect( int iIndex );
    GCCollisionRect<float>& GetPushBoundRect( int iIndex, bool bIgnoreActive = false );
    void SetSuperArmor( bool bSuperArmor, bool bAttackSuper = false );
    void SetSuperArmorFrame( int iSuperArmor ){ m_iSuperArmor = iSuperArmor; }
    bool IsSuperArmor(){ return m_bIsSuperArmor || m_iSuperArmor > 0 || ( m_dwChampionProperty & CP_SUPER_ARMOR ); }
    bool IsAttackSuperArmor() { return m_bAttackSuperArmor; }
    void SetInvincible( bool bInvincible ){ m_bIsInvincible = bInvincible; }
    void SetInvincibleFrame( int iInvincible ){ m_iInvincible = iInvincible; }
	bool IsInvincible(){ return m_bIsInvincible || m_iInvincible > 0 || m_bIsInvincibleNoFrame; }
	bool IsInvincibleNoFrame(){ return m_bIsInvincibleNoFrame; }
	void SetInvincibleNoFrame( bool bInvincible ){ m_bIsInvincibleNoFrame = bInvincible; }
    void SetDownDelay( int iDownDelay ){ m_iDownDelay = iDownDelay; }
    int GetDownDelay(){ return m_iDownDelay; }
    void SetTargetPlayerIndex( int iIndex_ );
    bool IsTarget();// 타겟이 있는가?
    bool SetTarget( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool SetTargetNearPlayer();
    bool SetTargetFarPlayer();
    bool SetTargetToPlayer( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool SetTargetToMonster( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
	void SetStaticTarget( int iSlotIndex, bool bPlayer );   // 슬롯ID(배열번호)을 이용해서 고정적으로 타켓팅하도록 한다.
    void ReleaseTarget( bool bChangeWaitState = false );
    bool TurnToTarget( bool bReverse = false );
    float GetTargetX();
    float GetTargetY();
    bool GetTargetIsContact();
    bool GetTargetIsRight();
    bool IsTargetMonster() const;
    bool IsTargetPlayer() const;
    bool IsTargetMonsterType(int iMonID) const;
    void SetMonsterTargetManual( int iTargetMonsterIndex_ );
    int GetMonsterTarget() { return m_iTargetMonsterIndex; }
	int GetPlayerTarget() { return m_iTargetPlayerIndex; }
    void SetPlayerTargetManual( int iTargetPlayerIndex_ );
    // 유닛 체크함수들 모음( 접두접미 키워드 : 존재유무는 Check, 마릿수는 Get, 타켓팅유무는 Target, 팀구분은 Enemy, 범위는 Range )
    bool CheckTargetInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool CheckEnemyInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool CheckMonsterInRange( int iSlot, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool CheckPlayerInRange( int iSlot, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool CheckSameTeamInRange( int iType, int iNum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool CheckPosInRange( float fPosX, float fPosY, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    int GetEnemyCntInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0, bool bNoPassPlayer = false, bool bNoPassMonster = false);
    int GetUnitCntInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0, bool bNoPassPlayer = false, bool bNoPassMonster = false);
    int CountPlayerInRect( int iLeft, int iTop, int iRight, int iBottom );
    D3DXVECTOR2 GetEnemyDistAllRange( bool bPassPlayer, bool bPassMonster );
    D3DXVECTOR2 GetEnemyPlayerPos(int iPlayerIndex );
    D3DXVECTOR2 GetEnemyMonsterPos(int iMonsterIndex );

    bool IsTargetDamage() const;
    void TurnToDamageTarget();
    D3DXVECTOR2 GetTargetDamagePos();
    bool SetDamageTargetAllRange( int iDamageEnum );
    bool SetDamageTarget( int iDamageEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool CheckDamageTarget( bool bReset = false );
    bool CheckDamageTargetInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
    bool DamageTarget( int iDamageEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY, bool bCheckRange );
    void DeleteTargetDamage();
    int GetDamageInstanceCount( int iDamageEnum, int iLife = 0 );
    void ReleaseTargetDamage() { m_dwTargetDamage = 0; }
    int GetLiveEnemyCnt();
    D3DXVECTOR2 CheckNearFootHold( int iOffsetX, int iOffsetY );
    void SetTempTargetPos( D3DXVECTOR2 vPos ) { m_vTempTargetPos = vPos; }
    D3DXVECTOR2 GetTempTargetPos() { return m_vTempTargetPos; }
    bool IsTempTarget() { return (false == (0.0f == m_vTempTargetPos.x && 0.0f == m_vTempTargetPos.y)); }
    void ReleaseTempTargetPos() { m_vTempTargetPos.x = 0.0f; m_vTempTargetPos.y = 0.0f; }
    float GetVectorDistance( float fX, float fY );
    void TurnToTempTarget();
    void StartAfterImage();
    void EndAfterImage();
    bool SetTargetAllRange( bool bPassPlayer = false, bool bPassMonster = false );
    bool EffectiveTarget();
    bool EffectivePlayer( int iPlayerIndex );
    bool EffectiveMonster( int iMonsterIndex );
    int GetMaxPlayer() { return MAX_PLAYER_NUM; };
    int GetMaxMonster() { return MAX_MONSTER; };
    float GetDistance( MONSTER* pMonster_ );
    float GetDistanceWithPlayer( PLAYER* pPlayer_ );
    bool IsBoxIn( float fOffsetX, float fOffsetY, KBoundBox& kBoundBox_, MONSTER* pkMonster_) const;
    void SetFlashEffectFrame( int iFlashEffectFrame );
    void SetShakeEffectFrame( int iShakeEffectFrame );
    int GetCountValue(){ return m_iCountValue; }
    void SetCountValue( int iCountValue ){ m_iCountValue = iCountValue; }
    void AddCountValue( int iCountValue ){ m_iCountValue += iCountValue; }
    void Setv3TempValue( float fX, float fY, float fZ ) { m_vTempValue = D3DXVECTOR3(fX, fY, fZ);   }
    D3DXVECTOR3 Getv3TempValue() { return m_vTempValue;}
	int GetTimer(){ return m_iTimerFrame; }
	void InitTimer(){ m_iTimerFrame = 0; m_bProcessTimer = false; }
	void StartTimer(){ m_bProcessTimer = true; }
	void StopTimer(){ m_bProcessTimer = false; }
    void SetPushCheck( bool bCheck, int iIndex );
    bool GetPushCheck( int iIndex);
    void SetAttackCheck( bool bCheck ){ m_bAttackCheck = bCheck; }
    bool GetAttackCheck(){ return m_bAttackCheck; }
    bool IsLiveDamageIdentifie( DWORD dwIdentifie );
    CDamageInstance* AddDamage( int iDamage, float fStartAngle = 0.0f );
	CDamageInstance* AddDamageWithLocateDirect( int iDamage ,float fStartX, float fStartY, bool right);
    CDamageInstance* AddDamageLastPlayers( int iDamage );
    CDamageInstance* AddDamageWithAngle( int iDamage, float fStartAngle );
    CDamageInstance* AddDamageWithDirectionAngle( int iDamage, float fStartAngle, bool bRight );
    CDamageInstance* AddDamageWithLocate( int iDamage, float fStartX, float fStartY, bool bAbsolutePos = false, bool bDirection_ = true );
    CDamageInstance* AddDamageWithLocateAndIdentifie( DWORD dwIdentifie, int iDamage, float fStartX, float fStartY, bool bAbsolutePos = false, bool bDirection_ = true );
    CDamageInstance* AddDamageWithStatic( int iDamage, float fStartX, float fStartY, bool bNoDirection = false );
    CDamageInstance* AddDamageWithStatic2( int iDamage, float fStartX, float fStartY );
	CDamageInstance* AddDamageWithLocateAngle( int iDamage, float fStartX, float fStartY, bool bAbsolutePos = false, float fStartAngle = 0.0f, bool bMoveAngle = false );
    CDamageInstance* AddDamageWithTargetPos( int iDamage, float fStartX, float fStartY, float fTargetX, float fTargetY, char* strAction = "" );
    CDamageInstance* AddDamageWithSpeed( int iDamage, float fStartX, float fStartY, float fSpeedX, float fSpeedY );
    CDamageInstance* AddDamageWithStaticAndSpeed( int iDamage, float fStartX, float fStartY, float fSpeedX, float fSpeedY );
    void AddDamageToAlivePlayer( int iDamage, float fStartX, float fStartY, bool bUseMonPos_ = false );
	void AddDamageRelation(CDamageInstance* damage);
    void AddDamageWithPacket( int iDamageEnum, int iAddType, float fPosX, float fPosY, float fSpeedX, float fSpeedY, bool bSendLocal = false );
	void AddParticleRelation(CParticleEventSeqPTR particle);
    void AddParticle( char* strSeqeunce, float fOffsetX, float fOffsetY, bool bOnUI = false );
    void AddParticleArg( KParticleArg* pArg );
	void AddTraceParticle( char* strSeqeunce, float fTraceTime, float fOffsetX = 0.0f, float fOffsetY = 0.0f, bool bIsNoDirection = false );
    void AddTraceParticleToBone( char* strSeqeunce, float fTraceTime, int iBoneIndex_, int iLeftSideLayer_ = 0, int iRightSideLayer_ = 0,  float xOffset_ = 0.0f, float yOffset_ = 0.0f, bool bNoDirection = false );
	void AddTraceParticlePos( std::string strSeqeunce, float fTraceTime, int iBoneIndex_ = -1, float fPosX_ = 0.0f, float fPosY_ = 0.0f, bool bIsNoDirection = false );
    void AddParticleNoDirection( char* strSeqeunce, float fOffsetX, float fOffsetY );
    void AddOvejarCloneDeathCount() { g_kGlobalValue.m_iOvejarCloneDeathCount += 1; };
    void ResetOvejarCloneDeathCount() { g_kGlobalValue.m_iOvejarCloneDeathCount = 0; };
    void AddParticleNoDirectionToBone( char* strSeqeunce, float fOffsetX, float fOffsetY, int iBone );
    void AddTraceParticleNoDirection( char* strSeqeunce );
	void AddTraceParticleOffset( char* strSeqeunce, float fOffsetX, float fOffsetY, float fTraceTime);	
    CParticleEventSeqPTR AddTraceParticleToBoneEx( std::string strSeqeunce, float fTraceTime, int iBoneIndex_, int iLeftSideLayer_ = 0, int iRightSideLayer_ = 0,  float xOffset_ = 0.0f, float yOffset_ = 0.0f, bool bNoDirection = false );
    void AddTraceParticleDieToClear( char* strSeqeunce_, float fTraceTime_, int iBoneIndex_, int iLeftSideLayer_, int iRightSideLayer_, float xOffset_, float yOffset_, bool bNoDirection = false );
    void SetDamageEff( float fX, float fY, float fZ );
    void ToggleRender( bool bRender ){ m_bRender = bRender; }
    void SetFly( bool bFly ){ m_bIsFly = bFly; }
    bool GetFly(){ return m_bIsFly; }
    virtual bool IsDie() { return m_bDie; };
    void SetDie();
    void SkillReadyEffect( BYTE r, BYTE g, BYTE b, float fHeight );
    void SkillReadyEffectNoStop( BYTE r, BYTE g, BYTE b, float fHeight );
    void KamikiLightning( float fX, float fY );
    void KamikiLightningReady( float fX, float fY );
    void EarthQuake( int iShakeCamera, float fGravity, float fDamage );
    void EarthQuakeWithRange( int iShakeCamera, float fGravity, float fDamage, float fRange );
    void ReverseGravity( float fRange, float fDamage, bool bFlyCheck );
    void OnReverseGravity( float fRangeX, float fDamage, bool bFlyCheck, float fHeight = 0.0f, float fRangeY = 0.0f, float fOffSetX = 0.0f, float fOffSetY = 0.0f );
    void CameraVib( int iDirction, float fCameraVib, float fDecrease );
    void FlashScreen();
    int GetSummonedMonsterCount();
    int GetSummonedMonsterCountforType( int iType );
	bool CheckSummonSlot( );
	bool SummonMonster( KSummonInfo* pSummonInfo, bool bNoCheckType = false, bool bNoMonsterPos = false, int iSummonMonsterLevel = 0 );
    void SummonMonsterByLocal( KSummonInfo* pSummonInfo, bool bNoCheckType, bool bNoMonsterPos, int iSummonMonsterLevel );
    void SummonDamageMonster( KSummonInfo* pSummonInfo, int iSummonMonsterLevel, int iHitPlayerIdx, bool bIsCharLock = false );
    void RegisterSummonMonsterType( int iIndex );
    bool CheckSummonMotherDist();
    D3DXVECTOR2 TurnToMother(float fXLimit, float fYLimit);
    D3DXVECTOR2 SameTurnToMother(float fXLimit, float fYLimit);
    D3DXVECTOR2 GetMotherPos();
    DWORD GetSummonPlayerUID();
    int GetLastAttackedMe( void ) { return m_iLastAttackedMe; }
    void SetLastAttackedBoxIndex( unsigned int iIndex ) { m_iLastAttackedBoxIndex = iIndex; };
    void CustomMonsterSetting();
    void SetMeshRender( int iIndex_, bool bRender_ );
    bool GetMeshRender( int iIndex_ );
    int GetTraceParticleNum();
    void SetTexture( char *szTex );
    void SetTriggerID( int iTriggerID );
    int GetTriggerID() { return m_iTriggerID; }
    void SetDropObserver( DropItemObserver pFunc ) { m_pDropItemObserver = pFunc; }
    float ComputeAngleToTarget( float fPosX, float fPosY, float fMinAngle = 0.0f, float fMaxAngle = 0.0f );
    float GetAngleToTarget(){ return Body_Angle; }
    bool CheckBackAttack( int iPlayerIndex, bool bPhysicalDamage_ );
    bool CheckAerial();
    void FreezeAttack( float fDamage, bool bCatchEffefct );
    void ChampionMonsterAttack( int iIndex );
    DWORD GetChampionProperty(){ return m_dwChampionProperty; }
    void SetChampionProperty( DWORD dwChampionProperty){ m_dwChampionProperty = dwChampionProperty; }
    void AddBlackHoleEffect();
    void PullPlayer( float yOffset = 0.0f, float fPullRate = 1.0f, float xOffset = 0.0f );
    void KazeazeBlackBall();
    EGCMonster GetDefaultMonster( int iMonsterType );
    int GetMonsterType() { return m_iMonsterType; }
    void Fear( bool bConfusion );
    float GetDamageFactor( PLAYER* pPlayer ) const;
	void LuaActionCallBack( const char* strAction );
    void CalcPushRect();
    void CalcAttackRect();
    void CalcFootHold();
    void SetDrawSlash( unsigned int iIndex_, bool bRender );
        D3DXVECTOR3 GetBonePos( int iBoneID );
	void SetTitleText( const int iStringId_ );
    bool OnDamageProcess( int iPlayerIndex, int iWhat, EGCAttackType eAttackType, bool bSpecialAttack, bool bArrowDamage, float fAttackPower_, bool &bSkipDamageMotion_);
	float GetStageX( void );
	float GetStageY( void );
	void SetAttackedMonster( bool bIsAttacked_) { m_bIsAttacked = bIsAttacked_; }
	bool GetAttackedMonster() { return m_bIsAttacked; }
	void HackingCheck();
    void SetTraceIndex( int iIndex_, int iBone_ ) { m_iTraceindex = iIndex_; m_iTraceBone = iBone_; }
	void SetTrace( bool bTrace_ ) { m_bTrace = bTrace_; }
	void SetTraceGap( float fX_, float fY_ ) { m_fTraceGapX = fX_; m_fTraceGapY = fY_; }
    void SetTracePercent( float fPercent_ ) { m_fPercentLength = fPercent_; }
	void TraceMonster( void );
    void ClearTraceIcon();
    bool IsHideName( void );
    void SetHitPos( D3DXVECTOR2 vecPos_ );
    D3DXVECTOR2 GetHitPos( void ) { return m_vecHitPos; }
    void SetBodyAngle( float fBodyAngle_ );
    float GetBodyAngle();
    void SetUserBoneAngle( int iBoneIndex, float fAngleX, float fAngleY, float fAngleZ );
    D3DXVECTOR3 GetUserBoneAngle( int iBoneIndex );
    void ClearMagicPlayerEffect( int iPlayerIndex_, int iBuffIndex_ );
    void SetMagicPlayerEffect( int iPlayerIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_ );
    void SetMagicPlayerEffectInRange( int iBuffIndex_, float fBuffTime_, int iLeft, int iTop, int iRight, int iBottom );
    void SetMagicEffect( int iPlayerIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_ = 1, int iIndex_= -1, bool bIsCrashPlayer = true, bool bAccTime_ = false );
    void SetMagicMonsterALLEffect( int iBuffIndex_, float fBuffTime_, int iBuffLevel_ = 1, int iTeam = -1, int iIndex_= -1, bool bIsCrashPlayer = true, bool bAccTime_ = false );
    bool IsMagicEffect( int iPlayerIndex_, int iBuffIndex_ );
    int GetBuffLevel( int iPlayerIndex_, int iBuffIndex_ );
    void ClearMagicEffect( int iPlayerIndex_, int iBuffIndex_ );
    void ClearMagicPlayerAllEffect( int iBuffIndex_ );
    void ClearMagicMonsterALLEffect( int iBuffIndex_, int iTeam = -1 );
    void ClearALLMagicEffect();
    void SetMagicEffectNoLocal( int iMonsterIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_ = 1, int iWhoIndex_ = -1 );
    void ClearMagicEffectNoLocal( int iMonsterIndex_, int iBuffIndex_ );
	void SetMagicEffect_NotMeBroad( int iMonsterIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_, int iWho = 0 );
	void ClearMagicEffect_NotMeBroad( int iMonsterIndex_, int iBuffIndex_ );
    void ClearMagicEffect_AllMonster( int iBuffIndex_ );
    float GetMagicEffectTime( int iPlayerIndex_, int iBuffIndex_ );
    void ShowExpEffect( int iPlayerIndex_, float fExp_ );
	void TraceIconProcess();
	void UpdateHeadPos();
	bool IsInScreen();
    void SetChampionBuffIcon( int iMonsterIndex );
    void ShowAwakeMake( void );
    void ShowMissAttack(float fHitPosX, float fHitPosY );
    bool IsNoComboSkillCheckMonster();
    void SetMeshToggleState( bool bToggle )   {   m_bMeshToggleState = bToggle;   }
    bool GetMeshToggleState()                 {   return m_bMeshToggleState;      }
	virtual void ToggleExtraMesh( const char* strID, bool bToggle_ );
	virtual void ToggleExtraMeshAll( bool bToggle_ );
	virtual void ToggleBaseMesh( bool bToggle_ );
	virtual void SetChildMeshMotion( char* strChlid, char* strMotion, bool bReserveNext, bool bForce, bool bResetFrame = false, bool bNotOneTime = false, int iLoopMode = 0 );
    virtual void ClearChildMeshOneTimeMotion( char* strChlid );
    virtual CParticleEventSeqPTR AddParticleToChildMeshBone( char* strChlid, char* strSeqeunce, int iBoneIndex_, float fTraceTime = 0.5f, float offset_x = 0.0f, float offset_y = 0.0f, bool bNoDirection = false );
    void SetChildMeshMotion_Front( char* strChlid, char* strMotion );
    int GetGameMode( void );
	void EndBlur();
    void TurnOffBossBlur();
    void ChangeMonster( KGC_PID_CHANGE_MONSTER::KChangeMonInfo kChangeMon_ );
    void MonsterAction( int iMonsterActionIndex_ );
    int GetSpecialType( void ) { return m_nSpecial; }
    void SendBossObjectStat( OUT KBossDungeonStat& kDungeonStat );
    void SendDropInfo( void );
    void SendMonsterKill( void );
    float GetShield() const { return m_fShield; }
    void SetShield( float val ) { m_fShield = val; }
    void ShieldProcess( void );
    void SetShieldEffect( char* strName, float fOffsetX = 0.0f, float fOffsetY = 0.0f );
    void SetDamageDirection(DAMAGEDIRECTION eDamageDir_ ) { m_eDamageDir = eDamageDir_; }
    DAMAGEDIRECTION GetDamageDirection() { return m_eDamageDir; };
    void SetPlayerMonster();
    bool CheckMonsterBackAttack( int iMonsterIndex, bool bPhysicalDamage_ );
    bool OnMonsterDamageProcess( int iMonsterIndex, int iWhat, EGCAttackType eAttackType, bool bSpecialAttack, bool bArrowDamage, float fAttackPower_, bool &bSkipDamageMotion_ );
    void ChangeHPToMonster( CDamageInstance* pDamageInstance, float fDamage, int iMonsterIndex, bool bSpecialAtk );
    bool OnMonsterDamage( int iMonsterIndex, int iWhat,
        float fAttackPower,
        EGCAttackType eAttackType, EGCAttackDirection eAttackDirection,
        float fPushTargetX, float fPushTargetY, bool bSpecialAttack = false, bool bSpecialPoint = true, bool bArrowDamage = false );
    void SetNoTargettingFrame( int iFrame ){ m_iNoTargettingFrame = iFrame; }
    bool CheckStandBlock();
    bool CheckSideBlock();
    void SetSummonMonsterDieType( bool bType ) { m_bSummonMonsterDieType = bType;    }
    bool GetSummonMonsterDieType()             { return m_bSummonMonsterDieType;    }
    void SetDamageLife( int iDamageType, int iLife );
    BOOL IsDamageLife( DAMAGE_TYPE eDamageTypeiLife );
    bool IsMyDamageToType( DAMAGE_TYPE eDamageType );
    void DeleteAllMyDamageToType( DAMAGE_TYPE eDamageType, int iEraseLife = 0 );
    D3DXVECTOR2 GetPlayerPosByMinDistance(); 
    bool IsPlatformer();
    bool GetInitAI() { return m_bInitAI; }
    void SetInitAI(bool bState) { m_bInitAI = bState; }
    bool GetImitSummon() { return m_bImitSummon; }
    void SetImitSummon(bool bState) { m_bImitSummon = bState; }
    bool GetDotaGateIsRight() { return m_bDotaGateIsRight; }
    void SetDotaGateIsRight(bool bState) { m_bDotaGateIsRight = bState; }
    void AddParticleModel( char *strParticleName, float fX, float fY, int iBornID, float fHpMin, float fHpMax, bool bNoDirection );
    void FrameMove_ParticleModel();
    void SetItemList( const std::vector< GCITEMID >& vecItemList );
    void MonsterItemEquip( void );
    bool MakeItemListMeshInfo( OUT std::vector< EXTRA_MESH >& vecMeshInfo );
    int  GetCharType( void ) { return m_iCharType; }
    int  GetAgitMonCharacterUID( void ) { return m_iAgitMonCharUID; }
    void SetAgitMonCharUID( int iUID_ ) { m_iAgitMonCharUID = iUID_; }
    int  GetMentCount( void ) { return m_iMentCount; }
    void SetMentCount( void );
    void SetAgitCharCloseMent( void ); 
    bool CheckLiveMonster( int iMonsterType, int iMonCnt = 0 );
    bool CheckLiveMonsterFromAllSlot( int iTeam );
    int GetLiveMonsterCnt( int iMonsterType );
    bool IsHost();
    void ShowEscortMonsterDesc( float fDamage_ );    
    void ShowEscortMonsterHPBar( void );
    float GetDamageDiff() { return m_fDamageDiff; }
    void SetDamageDiff( float fDamageDiff ) { m_fDamageDiff = fDamageDiff; }
    int GetDamageWhat() { return m_iDamageWhat; }
    void SetDamageWhat( int iWhat ) { m_iDamageWhat = iWhat; }
    void SetInPlayerRangeCheck( bool bIn_ ) { m_bInPlayerRangeCheck = bIn_; }
    bool GetInPlayerRangeCheck() { return m_bInPlayerRangeCheck; }
	void Release();
	bool ChangeTraceIcon( char *szParitcleName, int iIconType = 0 );
    bool ResetTraceIcon( char *szParitcleName1, char *szParitcleName2 );
	void SetTraceIconExistScreen( bool bExistScreen ) { m_bIsMyExistScreen = bExistScreen; }
    void MultiplyBaseStats( float fMultiplier );
    bool IsMissionHellMode();
    bool IsMissionBreakMode();

public:
	int IsMagicEffectCount( int iMaxMon, int iBuffIndex_ );
	bool IsNoChangeState() { return m_bNoChangeState; }
	void SetNoChangeState(bool bValue ) { m_bNoChangeState = bValue; }
	bool SummonMonsterStaticSlot( KSummonInfo* pSummonInfo,  bool bNoCheckType = false, bool bNoMonsterPos = false, int iSummonMonsterLevel = 0, int iSlot = 0 );
	bool IsTargetSyncObject() const;
	void TurnToSyncObjectTarget();
	D3DXVECTOR2 GetTargetSyncObjectPos();
	bool SetSyncObjectTargetAllRange( int iObjectEnum );
	bool SetSyncObjectTarget( int iObjectEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
	bool SyncObjectTarget( int iObjectEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY, bool bCheckRange );
	bool CheckSyncObjectTarget( bool bReset = false );
	bool CheckSyncObjectTargetInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY = 0 );
	int GetSyncObjectInstanceCount( int iSyncObjectID );
	void ReleaseTargetSyncObject() { m_dwTargetSyncObject = 0; }
	void StartBlur( int iMonsterIndex, float fCameraZoom, DWORD dwBlurTime );
	void SelectEscortHPBarImg( int iImgIndex );
	void ShowEscortHPBar( bool bShow ) { m_bShowEscortHPBar = bShow; }
	void AddParticleBorn( char *strParticleName, float fOffsetX, float fOffsetY, int iBornID, bool bNoDirection);
	void SetAddWalkSpeed( float fSpeed ) { m_fAddWalkSpeed = fSpeed; }
	void SetAddRunSpeed( float fSpeed ) { m_fAddRunSpeed = fSpeed; }
	void SetNoCrashDamage( bool bToggle ) {  m_bNoCrashDamage = bToggle; }
	bool GetNoCrashDamage() {  return m_bNoCrashDamage; }
    bool IsEscortMonster() { return ( m_iMonsterKind == EMON_ESCORT ); }
	void SetSummonNumberAvailable(int iNum) {m_iSummonsNuberAvailable = m_iSummonsNuberAvailable;}
    D3DXVECTOR2 GetPlayerPos( int iIndex_ );
    bool GetPlayerIsRight( int iIndex_ );
    bool IsOldGraphicQuality();

public:
	int m_iRemainRender;
	int         m_iMonsterKind;
	KGC2DObject m_MissionIcon;
	bool             m_bToggleInfo;
	float fScreenX;
	float fScreenY;

	int m_iPreMotionFrame;
	int m_iPreIndex;
	std::map<int, int> m_mapMotionMaxFrameList;

	int m_iSlowCount;
	int m_iLatencySlowCount;

    KProtectedType<bool> m_bLive;                   // 몬스터가 로드되어 있나?

	int m_iTeam;
	bool        m_bMovement;               // AI를 OnOff 형식으로 끈다.
	DWORD       m_dwTargetingDelayTime;    // TargetOff
    DWORD       m_dwMissAttackTime;
	GCObject* m_pkMonsterModelDead;
	int m_iMonsterType;             // 몬스터 타입

	
	int m_iBossMonster;            // 보스몬스터인가?

	// 몬스터 스테이트 리스트
	std::vector< GCDeviceMotionFRM* > m_vecMotionList;
	int m_iPreMotion;
	int m_iMonsterIndex;
	KSafeArray<int, LATENCY_BUFFER_SIZE > m_aiMotion;
	KSafeArray<int, LATENCY_BUFFER_SIZE > m_aiFrame;
	KSafeArray<float, LATENCY_BUFFER_SIZE > m_afX;
	KSafeArray<float, LATENCY_BUFFER_SIZE > m_afY;
	KSafeArray<bool, LATENCY_BUFFER_SIZE > m_abIsRight;
	int m_iOldFrame;
	int m_iOldMotion;

	int m_iRemainFrame;

	// x,y 스피드 - 방장이 몬스터 움직임 계산할때만 쓰일듯? 
	float m_fSpeedX;
	float m_fSpeedY;

	float m_fShadowY; // 그림자 높이

	// 땅에 붙었는가..-ㅅ-
	bool m_bIsContact;
	bool m_bOldIsContact;

	// 공중에 뜨는 공격일 경우.. 어느시점에서 발판과 충돌체크를 할 것인지를 체크하기 위해 만든 변수
	float m_fContactHeight;

	// 넘어져 있는 모션인가? 넘어진 모션이면 바운딩 박스를 눞혀준다
	bool m_bIsFallDownMotion;

	bool m_bShowExp;
	static int m_aiMonsterRewardInfo[][NUM_MONSTER_REWARD];

	// 몬스터의 정보
	int m_iLevel;
	float m_fScale;
	float m_fWeight;
	float m_fWalkSpeed;
	float m_fRunSpeed;
	EGCNAMESIZE m_eNameSize;
	float m_fAttackBoundBoxWidth;
	float m_fAttackBoundBoxHeight;
	float m_fWidth;
	float m_fHeight;

	float m_fHeadPosY;
	float m_fHeadPosX;

	int m_iRecvStateFromPlayer;
	DWORD m_dwOwnerUID;
	int m_iSummonBuffIcon;
	EGCMonNPCType m_eNPC;
	float m_fShadowRatio;
	// 컬링하기 위한 값
	float m_fVolume;
	float m_fZPos;

	// 몬스터 체력
	KProtectedType<float> m_fOldHP;
	KProtectedType<float> m_fCumulativeDamage;
	KProtectedType<float> m_fHP;

	// 몬스터의 능력치
	float m_fAttackPoint;
    float m_fSummonsAtkRatio;
    float m_fOwnerAtkRatio;
	

	// 몬스터의 이름
	int m_iMonsterNameID;
	int m_iNameBone;
	float m_fNameOffset_X;
	float m_fNameOffset_Y;
	std::wstring m_strMonsterName;
	KProtectedType<float> m_fHPPoint;
	std::map< std::pair<EGCAttackType, EGCAttackDirection>, std::string > m_mapReaction;
	bool m_bIsBackAttack;
	bool m_bIsSpecialAttack;
	bool m_bResult;
	bool m_bMovableMonster;
	bool m_bFrameLock;      // 프레임을 멈춘다!
    bool m_bBuffFrameLock;  // 버프로 프레임을 멈춘다!
	int m_iNoCheckContact;  // 발판과의 충돌을 체크하는가? 0이면 체크함 값이 있으면 체크 안함
	bool m_bIgnoreFHCheck;  // 발판 속성 체크를 무시할 것인가? 보통은 false
	std::vector< GCPushRect<float> > m_vecPushRect;
	std::vector< GCAttackRect<float> > m_vecAttackRect;
	std::vector< KMonsterFootHold > m_vecFootHold;    
	// 슈퍼아머!!
	bool m_bIsSuperArmor;
    bool m_bAttackSuperArmor;   // 공격중에 걸리는 슈퍼아머 
	int m_iSuperArmor;

	// 무적!!
	bool m_bIsInvincible; //한프레임 무적판정용
	int m_iInvincible;
	bool m_bIsInvincibleNoFrame; //무적판정 끌때까지 무적
	// 넘어져 있는 딜레이
	int m_iDownDelay;
	// 몬스터가 타겟으로 잡은것
	int m_iTargetPlayerIndex;   // 타겟은 결국 플레이어겠지.. 플레이어의 인덱스를 저장한다
	int m_iTargetMonsterIndex;  // 몬스터도 타겟이 될 때가 잇따 가끔씩! 가끔씩!
	bool m_bMonsterTarget;      // 다른 몬스터가 이 몬스터를 타게팅 하는가???
    bool m_bNotMissionCount;    // 미션 카운팅을 하지 않는다.
	bool m_bNotBackAttack;		// 백어택 판정을 하지 않는다.
	float m_fTargetX;
	float m_fTargetY;
	// 타격이펙트 관련 변수, 함수
	int m_iFlashEffectFrame; // 타격받은 상대가 반짝 빛난다
	int m_iShakeEffectFrame; // 타격받은 상대가 흔들린다
	// AI 스크립트에서 여러 용도로 사용하는 값
	int m_iCountValue;
    D3DXVECTOR3 m_vTempValue;

	// AI 에서 타이머로 사용하쟈!
	int m_iTimerFrame;
	bool m_bProcessTimer;
	float m_fReserveValue;
	// 공격 판정 체크를 하겠는가?
	bool m_bAttackCheck;
	// 타격 이펙트
	std::vector< KGCAnimManager::DamageAnim > m_vecDamageAnim;
	// 중력의 영향을 받을것인가?
	bool m_bIsFly;

	bool m_bDie;
	// 카미키 라이트닝에 관련된 함수, 변수
	KSafeArray<CKTDGAnim::CAnimInstance*,KAMIKI_LIGHTNING_COUNT> m_apKamikiLightning;
	int m_iKamikiLightningIndex;
	std::vector< int > m_vecSummonMonsterType;
	// [10/5/2007] breadceo.for tank
	int m_iLastAttackedMe;
	//길이 계속 불러올 필요 없다. 
	float	m_fMonsterNameWidth;
	unsigned int m_iLastAttackedBoxIndex;
	int m_iTriggerID;
	DropItemObserver m_pDropItemObserver;
	// 챔피언 정보 0x00000000 은 챔피언 아닌녀석임..
	DWORD m_dwChampionProperty;
	static DWORD m_adwChampionPropertyList[];
	static int m_aiChampionPropertyString[];

	int m_iHPRecoveryCount;

	CParticleEventSeqPTR m_pkChampionEffect;
	CParticleEventSeqPTR m_pkFuryEffect;
    std::vector<CParticleEventSeqPTR> m_vecChildMeshParticle;
    std::vector<CParticleEventSeqPTR> m_vecDieToClearParticle;

	D3DXVECTOR3 m_vHeadPos;
	std::vector<KMonsterSwoosh> m_vecSwoosh;
	// 버프에서 몬스터 방어력 약화 수치를 저장한다. 방어력 약화는 이 수치를 캐릭터의 m_fIncApRatio에 더해줘서 데미지를 더 입도록한다.
	// 이쪽 관련 계산은 ChangeHP에... 
	int		m_iShockSturnCount;
	std::vector<KMonsterPhysicAttack> m_vecPhsAtk;

	int m_iProcessedFrame;

	bool m_bMotionChanged;

	float m_fAtkRatio;
	float m_fDefRatio;
    float m_fSDefRatio;
    bool m_bUnlimitDef;
	//몬스터해킹관련
	bool m_bIsAttacked;
	bool m_bTrace;
	int m_iTraceindex;
	int m_iTraceBone;
	float m_fTraceGapX;
	float m_fTraceGapY;
	float m_fPercentLength;
	float m_fHP_Recover;
	bool m_bBlur;
	DWORD m_dwBlurStartTime;
	D3DXVECTOR2 m_vecHitPos;
	std::vector<CParticleEventSeqPTR> TraceIcon;
	D3DXVECTOR3 TraceIconPos;
	bool m_bMeshToggleState;
	int m_iCurMotionIndex;
	int m_nSpecial;
	KGC_PID_CHANGE_MONSTER::KChangeMonInfo      m_kChangeMon;
	KProtectedType<float> m_fShield;
	std::vector<CParticleEventSeqPTR> m_vecShieldEffect;
	std::vector<KParticleArg> m_vecShieldEffectArg;
	int m_iSummoned;
	DAMAGEDIRECTION m_eDamageDir;
	int m_iNoTargettingFrame;
	bool m_bSummonMonsterDieType;   // false면 일반죽음, true면 소환으로 인한 죽음
	bool m_bInitAI;
	bool m_bImitSummon;
	// 현재 몬스터가 소환가능한 숫자.
	int m_iSummonsNuberAvailable;
	bool m_bDotaGateIsRight;
	int m_iCurParticleModel;
	std::vector<sParticleModel> m_vecParticleModel;
	HardAI_Core m_HardAI_Core;
	HardAI_Ancient m_HardAI_Ancient;
	HardAI_GuardianTower m_HardAI_GuardianTower;
	HardAI_GuardTower m_HardAI_GuardTower;
	int                      m_iCharType;
	int                      m_iAgitMonCharUID;
	int                      m_iMentCount;
	std::vector< GCITEMID >  m_vecItemIDList;
	KncStopWatch m_kEscortStopWatch;
	float m_fDamageDiff;
    int m_iDamageWhat;
	bool m_bInPlayerRangeCheck;
    bool m_bAggroState;

private:
	bool m_bIsMyExistScreen;
	D3DXVECTOR2 m_vTempTargetPos;
	DWORD m_dwTargetDamage;      // 타켓팅한 데미지 인스턴스의 주소정보
	DWORD m_dwTargetSyncObject;  // 타켓팅한 싱크오브젝트 인스턴스의 UID
	bool m_bNoChangeState;       // 몬스터 스테이트를 변경하지 못하게
	DWORD m_dwBlurEndTime;
	bool m_bShowEscortHPBar;
	float m_fAddWalkSpeed;
	float m_fAddRunSpeed;
	bool m_bNoCrashDamage;    
    bool m_bHPrenderSkip;

private:
    int m_iStopSpeedFrame;
public:
    void SetStopSpeedFrame( int iFrame ) { m_iStopSpeedFrame = iFrame; }

private:
        std::map<std::string, int>      m_mapTempValue;
public:
    int GetMapTempValue( char *szTemp );
    void SetMapTempValue( char *szTemp, int iValue);
    void Get_BasicMatrix( D3DXMATRIX* pMat, float fOffsetX, float fOffsetY, bool bCamera );

private:
    int                 m_iSelectTextrue;
    bool                m_bNonShakeEffect;
public:
    int GetSelectTexture()  {   return m_iSelectTextrue; }
    void SetNonShakeEffect( bool bShake )   { m_bNonShakeEffect = bShake; }

    
public:
    PLAYER* GetLocalPlayer();
    void RenderHitNumber( float fChange, bool IsMPDamage, float fHitPosX, float fHitPosY, bool bSend = false );
    void SetDiffMonsterTargeting( bool bToggle ) { m_bMonsterTarget = bToggle;  }
    bool GetDiffMonsterTargeting() { return m_bMonsterTarget;  }

public:
    int m_iBaseRender;
    float m_fRealMiniMapStaticScale;
    SMiniMapTexInfo m_kRealMiniMapAccTexInfo;
    ANIINFO m_kRealMiniMapScaleAniInfo;
    ANIINFO m_kRealMiniMapAlphaAniInfo;
    COLOR_ANIINFO m_kRealMiniMapColorAniInfo;
    void SetRealMiniMap_ScaleAni( int iAniTime, float fMinScale, float fMaxScale, float fSpeed, int iSoundID, bool bReturnAni );
    void SetRealMiniMap_AlphaAni( int iAniTime, float fMinAlpha, float fMaxAlpha, float fSpeed, int iSoundID, bool bReturnAni );
    void SetRealMiniMap_ColorAni( COLOR_ANIINFO *pColorAniInfo );
    void SetRealMiniMap_AddTexture( char *szFileName, float fX, float fY, float fZ, float fW, DWORD dwColor );
    void SetRealMiniMap_StaticScale( float fScale );
    void SetRealMiniMap_BaseRender( bool bRender );

public:
    int m_iUserCartoon;
    D3DXVECTOR4 m_vUserCartonEdgeColor;
    void SetUserCartoon( int iCartoonIdx );
    void SetUserEdgeColor( float fEdgeAlpha, float fEdgeR, float fEdgeG, float fEdgeB );

public:
    DWORD m_dwRequestUserUID;
    void RequestChangeStateToHost( char *szState );
    void SetRequestUserUID(DWORD dwRequestUserUID )    {   m_dwRequestUserUID = dwRequestUserUID;  };
    DWORD GetRequestUserUID()                          {   return m_dwRequestUserUID;  };
    bool IsAIConnectKey()                              {   return g_MyD3D->MyCtrl->k_keyAIConnection;   };

public:
    bool m_bNotCheckStateWall;
    void SetNotCheckStateWall( bool bCheck )           {   m_bNotCheckStateWall = bCheck;   }
    bool GetNotCheckStateWall()                        {   return m_bNotCheckStateWall;     }
    void SetDelay( int iDelay )                        {   m_iSlowCount = iDelay;   }
    void SetZPos( float fZPos )                        {   m_fZPos = fZPos;  }

public:
    float fBossDieCameraZoom;
    DWORD dwBossDieBlurTime;
    void SetBossDieCameraZoom( float fZoom )           {    fBossDieCameraZoom = fZoom; }
    void SetBossDieBlurTime( DWORD dwTime )            {    dwBossDieBlurTime = dwTime; }
    bool IsBlur()                                      {    return m_bBlur; }

public:
    void SetMonsterStateStat( char *szState );

public:
    bool    m_bCallDieFucn;

private:
    std::vector<int>    m_vecPermissionDamage;
public:
    std::vector<int> GetPermissionDamage()  {   return m_vecPermissionDamage;   };
    void ClearPermissionDamage()  {   m_vecPermissionDamage.clear();   };
    void SetPermissionDamage( const std::vector<int> &vecArg );
    void SendToHostOfMonsterHP( float fHP );

    void ShowWarninggMessage(int istring_ );

public: // 데미지가 이 플레이어에게 Crash되는 순간 
        // 데미지 생성위치와 생성시 생성자의 위치를 저장한다.
    D3DXVECTOR2 m_vDamageStartPos;
    D3DXVECTOR2 m_vDamageWhoPos;

public:
    bool m_bCollisionCheck;

public:
    KGCPrograssBar  m_kMonHPBar;
    KGCPrograssBar* GetHPGaugeBar() {   return &m_kMonHPBar; }

public:
    void SetMonsterTagetEffect();
    void SetMonsterAttackEffect();
    float GetShadowPos( float fX, float fY );

};

#endif // _MONSTER_H_
