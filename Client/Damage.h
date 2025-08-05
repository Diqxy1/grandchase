#ifndef _DAMAGE_H_
#define _DAMAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyD3D.h"
#include "DamageDefine.h"

class KGC3DObject;
struct Effect;
class CDamageInstance;
class CDamage
{
    public:
        CDamage();
        virtual ~CDamage();

		virtual void CustomizeBeforeCreate( CDamageInstance* pDamageInstance ){};
        virtual void Begin( CDamageInstance* pDamageInstance );
        virtual void BeginWithAngle( CDamageInstance* pDamageInstance , float fStartAngle );
        virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );

        virtual void BeginAnim( CDamageInstance* pDamageInstance );
        virtual void BeginParticle( CDamageInstance* pDamageInstance );

        virtual void End( CDamageInstance* pDamageInstance );

        virtual void FrameMove( CDamageInstance* pDamageInstance );
        virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
        virtual void NotAngleFrameMove( CDamageInstance* pDamageInstance );
        virtual void ValkyrieFrameMove( CDamageInstance* pDamageInstance );
        virtual void CheckTrigger( CDamageInstance* pDamageInstance );
		virtual void OnGroundCrash( CDamageInstance* pDamageInstance, bool bCrash_ );

        // 2011. 01. 21, shmhlove : 유도탄 데미지 처리
        void GuideMissileFrameMove( CDamageInstance* pDamageInstance );
        void CreateGuidedMissileTarget( CDamageInstance* pDamageInstance );
        bool SetGuidedMissileTarget( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile, D3DXVECTOR2 vPos );
        bool SetGuidedMissileTarget_Dist( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bNear );
        bool SetGuidedMissileTarget_Hp( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bSmallHp );
        bool SetGuidedMissileSameTeamTarget_Dist( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bNear );
        bool SetGuidedMissileSameTeamTarget_Hp( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bSmallHp );
        bool SetGuidedMissileOwnerTarget( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile );
        bool SetGuidedMissileMonsterTypeTarget( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile );
        bool SetGuidedMissilePlayerTarget( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile );
        bool SetGuidedMissileCharTarget( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile );
        
        bool CheckOwnderRectInPoint( CDamageInstance* pDamageInstance, GCCollisionRect<float> fRect, D3DXVECTOR2 vPos );
        D3DXVECTOR2 GetGuidedTargetPos( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile );

        // 2011. 06. 16, shmhlove : 직진 유도탄 데미지 처리
        void StraightGuideFrameMove( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile );

        void SetTraceCreator( CDamageInstance* pDamageInstance );

        void Render( CDamageInstance* pDamageInstance );
        virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
        virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
        virtual void CrashObject( CDamageInstance* pDamageInstance, int iObjIndex );
		virtual void CrashEffect( CDamageInstance* pDamageInstance );
        virtual void DamageHpEffect( CDamageInstance* pDamageInstance );
        virtual void InToggleMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos );
        virtual void OutToggleMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos );
        virtual void InToggleSummonMonsterMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos );
        virtual void OutToggleSummonMonsterMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos );
        virtual void MPDamageParticle( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos );

        virtual void UVOperation( CDamageInstance* pDamageInstance, int iVertexOffset, SLVERTEX*& v ) {}
        virtual void CreateCheckBoundary( CDamageInstance* pDamageInstance );

        CParticleEventSeqPTR SetParticle( CDamageInstance* pDamageInstance, CHILD_PARTICLE& stParticle );

        // 몬스터에 입는 데미지만 영향을 받는다
        void GetDamageRate( IN OUT float& fRate, MONSTER* pMonster, PLAYER* pPlayer, CDamageInstance* pDamage );
        void SetMileStonePos( float fX_, float fY_ );
        int GetHitPerFrame() { return m_iHitPerFrame; }

        // 지정한 모션에서만 유지되는 데미지 체크
        void CheckOnlyMotionDamage( CDamageInstance* pDamageInstance );

        bool IsExistDamageFlag( int iFlag_ );
		
        // 몬스터 소환
        void SummonMonster( CDamageInstance* pDamageInstance, int iHitPlayerIdx = -1 );
        void SetPlayerLock( bool bLock );

        int     m_Start_Life;        ///< 데미지 이펙트 초기 수명
        int     m_Reverse_Life;      ///< Trigger_Time을 이용하여 Life를 반복시킬 수 있는,,,
        int     m_Life_Change;       ///< 데미지 이펙트 수명 변화량
        
        float   m_Start_Scale_x;     ///< 초기 가로 크기
        float   m_Scale_Speed;       ///< 속도 조절량
        float   m_Y_per_x;           ///< 가로 크기에 대한 세로 크기 비율
        
        float   m_Start_Locate_x;    ///< 초기 x 위치 locate y
        float   m_Start_Locate_y;    ///< 초기 y 위치
        float   m_Start_Locate_Random_x[2];    ///< 랜덤초기 x 위치
        float   m_Start_Locate_Random_y[2];    ///< 랜덤초기 y 위치
        float   m_Start_Locate_Frame_x[2];     ///< 플레이어 프레임에 대한 초기 x 위치
        float   m_Start_Locate_Frame_y[2];     ///< 플레이어 프레임에 대한 초기 y 위치

        float   m_fAnimStartPosX;    ///< 초기 Anim x 위치
        float   m_fAnimStartPosY;    ///< 초기 Anim y 위치

        float   m_x_Speed;           ///< x 방향 속도
        float   m_y_Speed;           ///< y 방향 속도
        float   m_x_CrashSpeed;      ///< x 크래시되었을때 속도
        float   m_y_CrashSpeed;      ///< y 크래시되었을때 속도
        float   m_fDamageCrashSpeedX;      /// 이 데미지와 다른 데미지 충돌시 다른 데미지 속도를 제어한다
        float   m_fDamageCrashSpeedY;      /// 이 데미지와 다른 데미지 충돌시 다른 데미지 속도를 제어한다
        float   m_x_DecSpeed;        ///< x 방향 속도 감쇄율
        float   m_y_DecSpeed;        ///< y 방향 속도 감쇄율

        float   m_Start_Angle;       ///< 초기 각도
        float   m_Angle_Speed;       ///< 회전 속도
        float   m_fLimite_Angle;     ///< 회전 제한
        int     m_Texture;           ///< 데미지 이펙트 텍스처 번호        
        int     m_TextureNoRenderCnt;///< 텍스쳐가 지정한 프레임동안 보이지 않는다.

        bool    m_bIsCorruptedDamage;

        float   m_fDamageToPlayer;   //공격력(사람)
        float   m_fDamageToMonster;  //공격력(몬스터)
        bool    m_bIgnoreDefDamage;  //방어력 무시하냐???
        bool    m_bIgnoreSuperDamage;  //슈퍼 무시 - DA_IGNORE_INVINSIBLE 차이점은 상대가 슈퍼일 때는 데미지만 가한다. 현재거는 모션만 끊어 버림...

		float	m_fMpChange;		 //MP 변화량 (플레이어만)

        bool    m_bTraceMonsterBoundBox; // DA_TRACE_HITTED 속성이 있을때만 적용. 몬서터의 Render 위치가 아닌 몬스터의 바운드 박스 위치를 따라갈 것인가?
        bool    m_bDeleteSync;           // 데미지가 제거될때 패킷을 보내어 싱크를 맞춘다.(반드시!!! AddDamage시 인스턴스에 유니크한 Identifie를 설정해주어야하고, 낫미브로드로 보내게 된다.)
        bool    m_bNoCrashFatal;         // 캐릭터가 페이탈일때 충돌체크를 하지 않는다.

        // 피격된 대상의 그라운드 체크를 무시하기 위해서 원래 있던 m_iCheckContact는 다른데도 쓰고 있어서 해당 값을 사용 안함.
        // 몬스터 자체에는 처리 조차 안되어 있어 해당 값으로 몬스터 바닥 무시하도록 수정할 경우 기존에 잡아둔 판정에서 추가 문제 발생여지도 있어서 따로 변수 설정함.
        int     m_iIgnoreGroundCheckHittedObject;

        bool    m_IsEnableDamage;    ///< 데미지를 주는가?
        bool    m_IsAngleMove;       ///< 회전하는가?
        bool    m_IsAngleMoveWithOffset;   ///< 데미지의 시작 좌표 씹지 않음 ( 플레이어만 처리함 )
        bool    m_IsRealTimeAngleMove;       ///< 실시간 회전하는가?
        bool    m_IsStaticDamageCleaner;   /// 충돌시 고정형 데미지를 제거하는가?
        bool    m_IsStaticDamage;    /// 고정형 데미지 이다.( 이걸 만들당시에는 고정형 데미지를 Cleaner로 제거하기 위해서 만듬 )
        
        bool    m_IsFixedAngleMove;  ///< 포물선 운동. 단 주인공 Body의 Angle에 독립적.

        int     m_iCrashStart;   //이보다 더 life가 커야 충돌이 일어난다.
        int     m_iCrashEnd;     //이보다 더 life가 작아야 충돌이 일어난다.

        // 유도탄 관련( GM : Guided Missile )
        GUIDEDMISSILE m_GuidedMissile;

        //아래 세 변수는 사람한테만 해당
        int     m_iDamageMotion;     //Damage를 맞았을 때 어떤 모션으로 변하는가?
        float   m_fDMXSpeed;         //Damage를 맞았을 때의 x_speed
        float   m_fDMYSpeed;         //Damage를 맞았을 때의 y_speed
        bool    m_bNoLocalMotion;    //로컬플레이어는 모션변화를 하지 못하도록 한다.

        // 공격 속성 중에서 몬스터에게만 해당되는 것들
        int     m_iAttackType;      // 공격 타입
		int     m_iAttackTypeOnAerial;      // 공격 타입
        int     m_iAttackDirection; // 방향
        float   m_fPushTargetX;     // x축으로 미는 힘
        float   m_fPushTargetY;     // y축으로 미는 힘
        bool    m_bMonSuperDamage;  // 몬스터 스테이트에 상관없이 무조건들어가는 데미지
        bool    m_bSpecialAttack;   // 필살기인가??
        bool    m_bSlotUniqueSkillDmg; // 슬롯특수기 데미지 인가?
        bool    m_bSpecial4Damage; // 4단 필살기 데미지 인가?
        int     m_nSpecialGrade;    // 필살기 등급

        DWORD   m_dwDMAttribute;     //Damage의 속성을 다양한 플래그로 갖고 있는 변수이다.
        std::set<int> m_setDamageFlag; //위에 녀석이랑 비슷한 역할을 하지만 플래그(속성) 수에 제한은 없음. 
        int     m_iCrashSound;       //충돌시 어떤 소리를 내는가?

        int     m_iAPMagicEffect;    //AP캐릭터에게만 걸어주는 전용버프
        int     m_iMagicEffect;      //SendMagicEffectSynchPacket를 쓰는 Magiceffect.
        int     m_iUnMagicEffect;    //걸린 버프를 제거해주는
        int     m_iNoCrashUnMagicEffect;    //데미지와 충돌되지 않았을때 걸린 버프를 제거해주는
        bool    m_bUniqueMgicEff;    //m_iAPMagicEffect, m_iMagicEffect, m_iMonsterMagicEff이 걸려 있으면 중첩해서 걸지 않는다.
        bool    m_bFastHostMagicEff;    //몬스터일 경우 방장부터 버프를 즉시 걸고, 브로드낫미로 패킷을 보내도록 한다.

        std::vector<std::pair<int,float>> m_vecRandMagicEffect;//랜덤 매직이펙트( 버프Enum, 버프Time )
        float   m_fMagicEffTime;     //매직 이펙트 타임.
        int     m_iMagicLevel;      //
		int 	m_iMonsterMagicEff;
        int     m_iEarthQuake;       //카메라 흔드는 정도.
        int     m_iEarthQuakeY;      //Y축만 카메라 흔드는 정도.
        int     m_iEarthQuakeRunFrame; //크래시 될 때 말고도 정해진 프레임에 어스퀘이크 효과 주고할 때 설정하자.


        //파티클 관련
        string  m_particleSeqName;
        float   m_EmitMin;
        float   m_EmitMax;
        bool    m_bAddNoDirection;  // Player 방향을 무시하고 애드할 것인가~?
        std::vector< CHILD_PARTICLE > m_vecChildParticle;
        std::vector< DAMAGE_PARTICLE > m_vecDamageHpParticle;
        std::vector< DAMAGE_PARTICLE > m_vecInToggleMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecOutToggleMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecInToggleSummonMonsterMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecOutToggleSummonMonsterMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecMPDamageParticle;
        std::vector< std::pair<int, int> > m_vecSpark;    //타격 이펙트

        //파티클 child

        //애니매이션 관련
        string  m_AnimSeqName;
        bool    m_bAnimLoop;
        float   m_fAnim_Start_ScaleX;
        float   m_fAnim_Start_ScaleY;
        float   m_fAnimScaleX_Speed;
        float   m_fAnimScaleY_Speed;
        float   m_fAnim_Limit_ScaleX;
        float   m_fAnim_Limit_ScaleY;

        float   m_fAnimAngle;

        CDamage& operator = (const CDamage& damage);

        int m_iParent;              // 부모 번호
        int m_iReserveParent;       // 부모 정보로 데미지 생성하고 나서 부모번호는 따로 저장하기 위해서!! 위에거에 남기면 문제생김.
        bool m_bDirection;          // 방향성이 있는가?

        int     m_SRCBlendMode;     // 소스 블렌드 모드
        int     m_DESTBlendMode;    // 데스트 블렌드 모드
        int     m_iJumpPossibleFrame;

        // Stone Curse 전용
        float   m_fStoneMaxDmg;
        float   m_fStoneMinDmg;
        float   m_fDecreasePerPush;

        // Particle Trace
        bool        m_bParticleTrace;
        bool        m_bParticleAngleMove;
        D3DXVECTOR3 m_vPos;

        // Trace Motion
        bool        m_bMotionTrace;
		bool        m_bMotionXTrace;
		bool        m_bMotionYTrace;

        D3DXVECTOR2 m_vPrevDamagePos;

        bool        m_bPlayerXTrace;
        bool        m_bPlayerYTrace;

        int        m_iStopMotionTraceLife;

        // Plus Delay
        int m_iPlusDelay;

        // Trace Pos Type
        int m_iTracePosType;

        // 잡기 안되는 프레임
        int m_iCannotCatchFrame;

        // 발판체크 안할 프레임
        int m_iNoCheckContact;

        // [12/6/2007] breadceo. 3d Mesh!!
        MESH_INFO   m_MeshInfo;
        D3DXVECTOR2 m_vPrevMeshMotionPos;

        std::vector< CRASH_PARTICLE > m_vecCrashParticle;

		// 한 데미지가 크래쉬되거나 라이프타임이 다되고 났을 시 생성되는 데미지
		std::vector< AUTO_DAMAGE > m_vecAutoDamage;
        std::vector< TARGET_AUTO_DAMAGE > m_vecTargetAutoDamage;

		std::vector< TIME_SOUND > m_vecTimeSound;

        std::vector< DAMAGE_TRIGGER > m_vecTrigger;

		std::vector<CParticleEventSeqPTR> m_vecParticle;

        OnlyMotionDamage m_kOnlyMotionDamage;

		// 스톤커스등에 지속 데미지를 들어가게 할지 구분
		bool m_bIsContinousDamage;
		float m_fContinousDamage;

		// 플레이들이 통과할 수 없는 데미지인지 구분
		bool m_bNoPassDamage;
		// 타격 가능한 데미지의 수명
		int m_iDamageHP;
        std::vector<int> m_vecPermissionDamageHP;

		int m_iHitPerFrame;

		// 데미지 박스의 충돌체크의 범위를 데미지 박스 전체로 할것인지
		bool m_bIsTotalRange;
        bool m_bIsGroundBottomBox;

		// 슬로우카운트 수동 조절하기 위해... 데미지 물리데미지 맞은것처럼 줄려고 할 때 일반적인 값 8을 주면 너무 끊겨
		// 보여서 좀 더 유연하게 조절할 수 있도록 슬로우카운트 수 저장하는 변수를 따로 지정
		int m_iSlowCount;
		
		//====================================================================================
		// 2009.02.10 : Jemitgge
		// Comment : 데미지 발판과 충돌 체크 안하는 프레임 길이!
		//			 GROUND_CRASH_AUTO_DAMAGE 이걸 쓸때, 땅 체크 안하는 프레임 주고 싶다.
		int m_iNoCheckContactDamage;

        bool m_bUserFrameDependant;
        bool m_bUserMotionDependant;

        float m_fGravity;
        bool m_bAccGravity;

        bool m_bNoAlphaChange;

        float m_fLeechHP;
		float m_fLeechMP;
        float m_fAbsorbHP;
        float m_fAbsorbHPToMaxHP;
        float m_fAbsorbHPToDamage;

        int m_iAddAbsorbHPPerHit;

		bool m_bStopAtContact;
        bool m_bEndAtContact;
        bool m_bAlwaysEnemy;
        bool m_bAttackToSameTeam;
		bool m_bCheckBoundary;
        bool m_bCreateCheckBoundary;
        bool m_bStopAtLastBottomContact;
        bool m_bEndAtLastBottomContact;
        bool m_bOwnerDirection;
		int m_iMaxInstanceNum;

        // 흠.. 발키리프로젝트에 쓸 값들
        std::vector<D3DXVECTOR3> m_vecMileStone;
        int         m_iIndex;
        int         m_iLifeMax;
        float       m_fS;
        bool        m_bOnUI;

		bool m_bApplyDamageSpeedByDamageDirection;

        // 연금술사 가스처럼 느려지게 만들자.
        int m_iHaste;

        int m_iFatalRatio;
        bool m_bToggleMesh;
        int m_iToggleSummonMonsterID;
        int m_iDamageToTeleport;
        bool m_bIgnoreSpecialMonDamageToTeleport; // 던전에서 특별하게 사용된 사용된 몬스터들의 데미지 텔포를 무시하게 한다. 기본적으로 보스 포함.
        int m_bShieldBreak;

        // 필살기 데미지 외 무적처리
        UINT m_uiNotSpecialSuper;
        bool m_bGasDamage;
        bool m_bIsCharLock;

        // 몬스터 소환
        struct SUMMON_MONSTER_INFO
        {
            int m_iMonsterID;
            int m_iSummonLifeTime;
            int m_iLevel;
            int m_iLimiteLiveMonsterCnt;
            float m_fOffsetX;
            float m_fOffsetY;
            bool m_bIsLock;
            char m_szState[16];
            SUMMON_MONSTER_INFO() : m_iMonsterID( -1 ), m_iSummonLifeTime( 0 ), m_iLevel( 0 ), m_fOffsetX( 0.0f ), m_fOffsetY( 0.0f ), m_bIsLock( false ), m_iLimiteLiveMonsterCnt( -1 ) 
            {
                strcpy(m_szState, "SUMMON");
            }
        };
        SUMMON_MONSTER_INFO m_kSummonMonInfo;

        int m_iWhoStartMotion; //데미지 생성시킨 플레이어의 바로 이전 모션
		
        int m_iNextAttackCount;

        bool m_bParticleSlow;
        bool m_bOwnerKillDamage;  //  데미지 생성자가 죽으면 데미지도 죽인다.
        int m_iUniqueDamage;      // 설정된 데미지를 이미 맞고 있다면 이 데미지를 맞지 않게 하겠다.

        // DA_TRACE_CREATOR속성일때 위치 Offset
        float m_fTraceCreateOffsetX;
        float m_fTraceCreateOffsetY;

        KRestrictHitCount m_kRestrictHitCnt;

        KVariousIncreaseRatio m_kVariousIncreaseRatio;		

        MOVE_CREATER_TO_CRASHPOS m_kMoveCreateToCrashPos;
        MOVE_CREATER_TO_CRASHPOS m_kMoveCreateToUnitPos;

        SYNC_DAMAGE_POS_AT_CRASH_POS m_kSyncDamagePosAtCrashPos;

        std::pair<int, int> m_pairSkipInfo; // <ContinueFrame, SkipFrame>

        bool m_bCheckNoPassFloor;
        std::vector< int > m_vecPassBuff;
        std::vector< int > m_vecPassNoBuff;

        bool m_bMonTargettingRelease;

        bool m_bNoCrashPlayer;
        bool m_bNoCrashMonster;
        bool m_bNoCrashDamage;
        bool m_bNoCrashGameObject;
        bool m_bNoCrash2DObject;

        float m_fGround_Offset_X;
        float m_fGround_Offset_Y;

        bool m_bOnlyOneUnit;

        bool m_bTogetherDie;

        std::vector< int > m_vecTogetherDieDamage;

        int m_iStopUnitFrame;

        bool m_bNoPushSpeedToFatal;
        float m_fSameTeamDamageToMonster;

        MON_REACTION    m_kMonReaction;
        std::map<int,int> m_mapNoCreateExceptionGameMode;

        CRASH_DAMAGE_TO_DAMAGE m_kDamageToDamage;

        bool m_bAutoDamageToGroundCrashOfNoDieCurDamage;

        // 지정한 몬스터하고만 충돌체크하는 데미지
        std::vector<int> m_vecCrashDamageToMonsterType;

        // 영역에 따른 분할데미지 팩터
        DIVISION_DAMAGE m_kDivisionDamage;

        // 데미지가 벽에 붙딪혔을때 반사시켜주는 기능
        BoundaryReflection  m_kBoundaryReflection;

        std::vector< int > m_vecTogetherDieBuff;

        //몬스터가 챔피언 속성을 가지고 있어도, 이 플래그가 true 라면 챔피언 속성 없이 공격한다. 
        bool m_bIgnoreChampionProperty;

        // 이 속성이 있는 데미지는 맞아도 콤보가 끊기지 않는다. 
        bool m_bNotComboBreak;
};

enum OBJ_TYPE
{
	OBJ_TYPE_PLAYER = 0,
	OBJ_TYPE_MONSTER = 1,
	OBJ_TYPE_SIZE = 2,
};

class CDamageInstance
{
public:
    void* operator new(size_t s){return bpool.malloc();}
    void operator delete(void *p){bpool.free(p);}
    static boost::pool<> bpool;

    CDamageInstance() : m_bPetDamage( false )
    {
        m_What              = 0;
        m_Who               = 0;
		m_HitWho			= 0;
		m_emWhoType			= OBJ_TYPE_PLAYER;
		m_emHitWhoType		= OBJ_TYPE_PLAYER;
        m_Life              = 0;
        m_PrevLife          = 0;
        m_SlowCount         = 0;
        m_dwIdentifie       = 0;
        m_X                 = 0.0f;
        m_Y                 = 0.0f;
        m_Scale_x           = 0.0f;
        m_Angle             = 0.0f;
		m_fRatio			= 1.0f;
        m_fAnimScaleX       = 0.0f;
        m_fAnimScaleY       = 0.0f;
        m_IsRight           = false;
        m_IsShow            = false;    
        m_IsMonsterDamage   = false;
        m_iTeam             = ETBlue;
		m_iLoopNum          = 0;
        m_iHitPerFrameCnt   = 0;
        m_fDamageCrashX     = 0.0f;
        m_fDamageCrashY     = 0.0f;
        m_fAccGuidSpeedX    = 0.0f;
        m_fAccGuidSpeedY    = 0.0f;
        m_bMoveCrashPos     = false;
        m_bMoveUnitPos      = false;
        m_bGroundCrash      = false;
        m_fAccGravity       = 0.0f;

        m_pParticle         = NULL;
        m_pAnim             = NULL;
        m_pOriginalDamage   = NULL;
		
		m_setAlreadyDamage.clear();

        m_ExtraMonsterNum   = -1;

        for( int i = 0 ; i < (int)m_ExceptPlayer.size() ; ++i )
        {
            m_ExceptPlayer[i] = 0;
        }
           
        m_p3DObject = NULL;
        m_vecChildParticle.clear();
        m_mapHitCount.clear();
        m_vecLockChar.clear();

        m_bPetTrace = false;
        m_bPlayerTrace = false;
        m_bCrashDamage = false;
        m_rtCollision.SetRect( 0.0f, 0.0f, 0.0f, 0.0f );
        m_TextureNoRenderCnt = 0;
        m_vDamageStartPos = D3DXVECTOR2(0.0f, 0.0f);
        m_vDamageWhoPos = D3DXVECTOR2(0.0f, 0.0f);

        m_vecPlayerSyncDamagePosAtCrashPos.clear();
        m_vecMonsterSyncDamagePosAtCrashPos.clear();

        m_dxvTargetPos.x = 0.0f;
        m_dxvTargetPos.y = 0.0f;
    }

    ~CDamageInstance();

    int     m_What;              ///< 데미지 이펙트의 종류
    int     m_Who;               ///< 데미지 이펙트를 발생시킨 사람
	int		m_HitWho;			
	
	//	이 값 쓸려면 따로 작업해야되요.
	//	우선 임시로 만들어두고 나중에 Damage 관련 코드에 다 이 타입을 써서
	//	코딩할수있도록 수정해야 합니다...
	OBJ_TYPE	m_emWhoType;	 ///< 이펙트 발생시킨 타입
	OBJ_TYPE	m_emHitWhoType;	 ///< 이펙트 맞은넘의 타입
    
    DWORD   m_dwIdentifie;      ///< 싱크를 맞춰보기위한 식별자( AddDamage시 알아서 유니크하게 셋팅해야함 )

	int     m_Life;              ///< 데미지 이펙트의 수명
    int     m_PrevLife;              ///< 데미지 이전 라이프
    int     m_SlowCount;         ///< 딜레이(지연값)

    float   m_X;                ///< x 위치
    float   m_Y;                ///< y 위치

    D3DXVECTOR2   m_vDamageStartPos; ///< 데미지 생성위치
    D3DXVECTOR2   m_vDamageWhoPos;    ///< 데미지 생성시 생성자의 위치

    float   m_Scale_x;          ///< 가로 방향 크기
    float   m_Angle;            ///< 각도
	float	m_fRatio;			// 데미지의 강도

    float   m_fAnimScaleX;      ///< Animation의 가로 크기
    float   m_fAnimScaleY;      ///< Animation의 세로 크기

    bool    m_IsRight;           ///< 오른쪽 방향인가?
    bool    m_IsShow;            ///< 보이는가?
    int     m_TextureNoRenderCnt;///< 텍스쳐가 지정한 프레임동안 보이지 않는다.

    int     m_iTeam;             ///< 무슨 팀인가?  1이면 블루팀, 0이면 레드, -1이면 대계 몬스터이고, 몬스터도 팀이 있을 수 있다.
	int     m_iLoopNum;			 /// 데미지의 프레임 무브를 몇번이나 더 돌린건가!
    int     m_iHitPerFrameCnt;   /// HIT_PER_FRAME의 카운팅 변수
    bool    m_bCrashDamage;      /// 플레이어나 몬스터와 크래시 되었을때 true

    float   m_fDamageCrashX;     /// 데미지와 데미지가 충돌되었을때 상대 데미지가 내 데미지에게 셋팅해준 속도값 X
    float   m_fDamageCrashY;     /// 데미지와 데미지가 충돌되었을때 상대 데미지가 내 데미지에게 셋팅해준 속도값 Y
    float   m_fAccGravity;       /// 누적 중력

    float   m_fAccGuidSpeedX;     /// 유도탄에 쓰이는 가속도가 누적되는 변수
    float   m_fAccGuidSpeedY;     /// 유도탄에 쓰이는 가속도가 누적되는 변수

    bool    m_bMoveCrashPos;     /// 데미지가 충돌된 곳으로 데미지생성자가 이동했는가?
    bool    m_bMoveUnitPos;     /// 데미지가 충돌된 유닛위치로 데미지생성자가 이동했는가?
    bool    m_bGroundCrash;      /// 데미지가 한번이라도 발판과 충돌했는가?
    std::vector<int>  m_vecSummonMonHitWho;  /// 소환 몬스터 데미지를 맞은녀석의 ID

    //KSafeArray<bool, MAX_PLAYER_NUM+MAX_MONSTER > m_bAlreadyDamaged;  ///< 플레이어나 몬스터 중 맞은 객체에 해당하는 슬롯은 true 가 된다.
	std::set<int> m_setAlreadyDamage; //플레이어나 몬스터 중 맞은 놈만 있다.

    CParticleEventSeqPTR                             m_pParticle;
    CKTDGAnim::CAnimInstance*                       m_pAnim;

    std::vector< CParticleEventSeqPTR > m_vecChildParticle;

    //표준 정보들
    CDamage* m_pOriginalDamage; 

    int     m_ExtraMonsterNum;
    bool    m_IsMonsterDamage;   ///< 몬스터의 데미지인가?

    //라이트닝 볼트 하드 코딩
    KSafeArray<int,MAX_PLAYER_NUM + 1> m_ExceptPlayer;   ///< 충돌 검사에서 제외할 사람들

    bool    m_bPetDamage;        ///< 펫이 쓰는거??
    bool    m_bPetTrace;         // 펫에 붙어 다니는 데미지?
    bool    m_bPlayerTrace;      // 플레이어에게 붙어다니는 데미지?
    // [12/5/2007] breadceo.3D 모델 그리고 싶다고!!
    KGC3DObject* m_p3DObject;

    std::map< int, int > m_mapHitCount;
    std::vector< DWORD > m_vecLockChar;   // Is_character_lock으로 Lock처리된 플레이어 UID 정보

    GCCollisionRect<float> m_rtCollision;  // 크래시 되었을때 충돌된 박스가 들어간다.

    // 데미지 위치 동기화시 지속성을 줄 유닛의 ID가 저장된다.
    std::vector<int> m_vecPlayerSyncDamagePosAtCrashPos;
    std::vector<int> m_vecMonsterSyncDamagePosAtCrashPos;

    D3DXVECTOR2 m_dxvTargetPos;         // 직선형 유도 데미지 일 때 타겟 위치 
public:
	CParticleEventSeqPTR CreateSequence( const std::string& name, float x=0.0f, float y=0.0f, float z=0.0f );
	Effect* Add( int What, float x, float y, int Delay, float Scale = 1.0f,
		float Target_x = -100.0f, float Target_y = -100.0f,
		float Angle = 0.0f, bool bIsRight = false, int who = -1 );

	int GetUnitedWho();
    bool IsHitFrame();
    bool IsHitPerFrame();
    void CountHitPerFrame();
    void SetCrashCheck( bool bCheck ) { m_bCrashDamage = bCheck; }
    bool GetCrashCheck() { return m_bCrashDamage; }
    void SetDamageCrashSpeed( float fX, float fY ) { m_fDamageCrashX = fX; m_fDamageCrashY = fY; }
    float GetDamageCrashSpeedX() { return m_fDamageCrashX; }
    float GetDamageCrashSpeedY() { return m_fDamageCrashY; }

public:
    void CheckSummonMonster();
	bool InsertAlreadyDamaged(int iIndex);
	bool CheckAlreadyDamaged(int iIndex);
	void EraseAlreadyDamaged(int iIndex = -1);

};

#endif // _DAMAGE_H_
