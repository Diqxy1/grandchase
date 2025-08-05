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

        // 2011. 01. 21, shmhlove : ����ź ������ ó��
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

        // 2011. 06. 16, shmhlove : ���� ����ź ������ ó��
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

        // ���Ϳ� �Դ� �������� ������ �޴´�
        void GetDamageRate( IN OUT float& fRate, MONSTER* pMonster, PLAYER* pPlayer, CDamageInstance* pDamage );
        void SetMileStonePos( float fX_, float fY_ );
        int GetHitPerFrame() { return m_iHitPerFrame; }

        // ������ ��ǿ����� �����Ǵ� ������ üũ
        void CheckOnlyMotionDamage( CDamageInstance* pDamageInstance );

        bool IsExistDamageFlag( int iFlag_ );
		
        // ���� ��ȯ
        void SummonMonster( CDamageInstance* pDamageInstance, int iHitPlayerIdx = -1 );
        void SetPlayerLock( bool bLock );

        int     m_Start_Life;        ///< ������ ����Ʈ �ʱ� ����
        int     m_Reverse_Life;      ///< Trigger_Time�� �̿��Ͽ� Life�� �ݺ���ų �� �ִ�,,,
        int     m_Life_Change;       ///< ������ ����Ʈ ���� ��ȭ��
        
        float   m_Start_Scale_x;     ///< �ʱ� ���� ũ��
        float   m_Scale_Speed;       ///< �ӵ� ������
        float   m_Y_per_x;           ///< ���� ũ�⿡ ���� ���� ũ�� ����
        
        float   m_Start_Locate_x;    ///< �ʱ� x ��ġ locate y
        float   m_Start_Locate_y;    ///< �ʱ� y ��ġ
        float   m_Start_Locate_Random_x[2];    ///< �����ʱ� x ��ġ
        float   m_Start_Locate_Random_y[2];    ///< �����ʱ� y ��ġ
        float   m_Start_Locate_Frame_x[2];     ///< �÷��̾� �����ӿ� ���� �ʱ� x ��ġ
        float   m_Start_Locate_Frame_y[2];     ///< �÷��̾� �����ӿ� ���� �ʱ� y ��ġ

        float   m_fAnimStartPosX;    ///< �ʱ� Anim x ��ġ
        float   m_fAnimStartPosY;    ///< �ʱ� Anim y ��ġ

        float   m_x_Speed;           ///< x ���� �ӵ�
        float   m_y_Speed;           ///< y ���� �ӵ�
        float   m_x_CrashSpeed;      ///< x ũ���õǾ����� �ӵ�
        float   m_y_CrashSpeed;      ///< y ũ���õǾ����� �ӵ�
        float   m_fDamageCrashSpeedX;      /// �� �������� �ٸ� ������ �浹�� �ٸ� ������ �ӵ��� �����Ѵ�
        float   m_fDamageCrashSpeedY;      /// �� �������� �ٸ� ������ �浹�� �ٸ� ������ �ӵ��� �����Ѵ�
        float   m_x_DecSpeed;        ///< x ���� �ӵ� ������
        float   m_y_DecSpeed;        ///< y ���� �ӵ� ������

        float   m_Start_Angle;       ///< �ʱ� ����
        float   m_Angle_Speed;       ///< ȸ�� �ӵ�
        float   m_fLimite_Angle;     ///< ȸ�� ����
        int     m_Texture;           ///< ������ ����Ʈ �ؽ�ó ��ȣ        
        int     m_TextureNoRenderCnt;///< �ؽ��İ� ������ �����ӵ��� ������ �ʴ´�.

        bool    m_bIsCorruptedDamage;

        float   m_fDamageToPlayer;   //���ݷ�(���)
        float   m_fDamageToMonster;  //���ݷ�(����)
        bool    m_bIgnoreDefDamage;  //���� �����ϳ�???
        bool    m_bIgnoreSuperDamage;  //���� ���� - DA_IGNORE_INVINSIBLE �������� ��밡 ������ ���� �������� ���Ѵ�. ����Ŵ� ��Ǹ� ���� ����...

		float	m_fMpChange;		 //MP ��ȭ�� (�÷��̾)

        bool    m_bTraceMonsterBoundBox; // DA_TRACE_HITTED �Ӽ��� �������� ����. ������ Render ��ġ�� �ƴ� ������ �ٿ�� �ڽ� ��ġ�� ���� ���ΰ�?
        bool    m_bDeleteSync;           // �������� ���ŵɶ� ��Ŷ�� ������ ��ũ�� �����.(�ݵ��!!! AddDamage�� �ν��Ͻ��� ����ũ�� Identifie�� �������־���ϰ�, ���̺�ε�� ������ �ȴ�.)
        bool    m_bNoCrashFatal;         // ĳ���Ͱ� ����Ż�϶� �浹üũ�� ���� �ʴ´�.

        // �ǰݵ� ����� �׶��� üũ�� �����ϱ� ���ؼ� ���� �ִ� m_iCheckContact�� �ٸ����� ���� �־ �ش� ���� ��� ����.
        // ���� ��ü���� ó�� ���� �ȵǾ� �־� �ش� ������ ���� �ٴ� �����ϵ��� ������ ��� ������ ��Ƶ� �������� �߰� ���� �߻������� �־ ���� ���� ������.
        int     m_iIgnoreGroundCheckHittedObject;

        bool    m_IsEnableDamage;    ///< �������� �ִ°�?
        bool    m_IsAngleMove;       ///< ȸ���ϴ°�?
        bool    m_IsAngleMoveWithOffset;   ///< �������� ���� ��ǥ ���� ���� ( �÷��̾ ó���� )
        bool    m_IsRealTimeAngleMove;       ///< �ǽð� ȸ���ϴ°�?
        bool    m_IsStaticDamageCleaner;   /// �浹�� ������ �������� �����ϴ°�?
        bool    m_IsStaticDamage;    /// ������ ������ �̴�.( �̰� �����ÿ��� ������ �������� Cleaner�� �����ϱ� ���ؼ� ���� )
        
        bool    m_IsFixedAngleMove;  ///< ������ �. �� ���ΰ� Body�� Angle�� ������.

        int     m_iCrashStart;   //�̺��� �� life�� Ŀ�� �浹�� �Ͼ��.
        int     m_iCrashEnd;     //�̺��� �� life�� �۾ƾ� �浹�� �Ͼ��.

        // ����ź ����( GM : Guided Missile )
        GUIDEDMISSILE m_GuidedMissile;

        //�Ʒ� �� ������ ������׸� �ش�
        int     m_iDamageMotion;     //Damage�� �¾��� �� � ������� ���ϴ°�?
        float   m_fDMXSpeed;         //Damage�� �¾��� ���� x_speed
        float   m_fDMYSpeed;         //Damage�� �¾��� ���� y_speed
        bool    m_bNoLocalMotion;    //�����÷��̾�� ��Ǻ�ȭ�� ���� ���ϵ��� �Ѵ�.

        // ���� �Ӽ� �߿��� ���Ϳ��Ը� �ش�Ǵ� �͵�
        int     m_iAttackType;      // ���� Ÿ��
		int     m_iAttackTypeOnAerial;      // ���� Ÿ��
        int     m_iAttackDirection; // ����
        float   m_fPushTargetX;     // x������ �̴� ��
        float   m_fPushTargetY;     // y������ �̴� ��
        bool    m_bMonSuperDamage;  // ���� ������Ʈ�� ������� �����ǵ��� ������
        bool    m_bSpecialAttack;   // �ʻ���ΰ�??
        bool    m_bSlotUniqueSkillDmg; // ����Ư���� ������ �ΰ�?
        bool    m_bSpecial4Damage; // 4�� �ʻ�� ������ �ΰ�?
        int     m_nSpecialGrade;    // �ʻ�� ���

        DWORD   m_dwDMAttribute;     //Damage�� �Ӽ��� �پ��� �÷��׷� ���� �ִ� �����̴�.
        std::set<int> m_setDamageFlag; //���� �༮�̶� ����� ������ ������ �÷���(�Ӽ�) ���� ������ ����. 
        int     m_iCrashSound;       //�浹�� � �Ҹ��� ���°�?

        int     m_iAPMagicEffect;    //APĳ���Ϳ��Ը� �ɾ��ִ� �������
        int     m_iMagicEffect;      //SendMagicEffectSynchPacket�� ���� Magiceffect.
        int     m_iUnMagicEffect;    //�ɸ� ������ �������ִ�
        int     m_iNoCrashUnMagicEffect;    //�������� �浹���� �ʾ����� �ɸ� ������ �������ִ�
        bool    m_bUniqueMgicEff;    //m_iAPMagicEffect, m_iMagicEffect, m_iMonsterMagicEff�� �ɷ� ������ ��ø�ؼ� ���� �ʴ´�.
        bool    m_bFastHostMagicEff;    //������ ��� ������� ������ ��� �ɰ�, ��ε峴�̷� ��Ŷ�� �������� �Ѵ�.

        std::vector<std::pair<int,float>> m_vecRandMagicEffect;//���� ��������Ʈ( ����Enum, ����Time )
        float   m_fMagicEffTime;     //���� ����Ʈ Ÿ��.
        int     m_iMagicLevel;      //
		int 	m_iMonsterMagicEff;
        int     m_iEarthQuake;       //ī�޶� ���� ����.
        int     m_iEarthQuakeY;      //Y�ุ ī�޶� ���� ����.
        int     m_iEarthQuakeRunFrame; //ũ���� �� �� ���� ������ �����ӿ� �����ũ ȿ�� �ְ��� �� ��������.


        //��ƼŬ ����
        string  m_particleSeqName;
        float   m_EmitMin;
        float   m_EmitMax;
        bool    m_bAddNoDirection;  // Player ������ �����ϰ� �ֵ��� ���ΰ�~?
        std::vector< CHILD_PARTICLE > m_vecChildParticle;
        std::vector< DAMAGE_PARTICLE > m_vecDamageHpParticle;
        std::vector< DAMAGE_PARTICLE > m_vecInToggleMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecOutToggleMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecInToggleSummonMonsterMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecOutToggleSummonMonsterMeshParticle;
        std::vector< DAMAGE_PARTICLE > m_vecMPDamageParticle;
        std::vector< std::pair<int, int> > m_vecSpark;    //Ÿ�� ����Ʈ

        //��ƼŬ child

        //�ִϸ��̼� ����
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

        int m_iParent;              // �θ� ��ȣ
        int m_iReserveParent;       // �θ� ������ ������ �����ϰ� ���� �θ��ȣ�� ���� �����ϱ� ���ؼ�!! �����ſ� ����� ��������.
        bool m_bDirection;          // ���⼺�� �ִ°�?

        int     m_SRCBlendMode;     // �ҽ� ���� ���
        int     m_DESTBlendMode;    // ����Ʈ ���� ���
        int     m_iJumpPossibleFrame;

        // Stone Curse ����
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

        // ��� �ȵǴ� ������
        int m_iCannotCatchFrame;

        // ����üũ ���� ������
        int m_iNoCheckContact;

        // [12/6/2007] breadceo. 3d Mesh!!
        MESH_INFO   m_MeshInfo;
        D3DXVECTOR2 m_vPrevMeshMotionPos;

        std::vector< CRASH_PARTICLE > m_vecCrashParticle;

		// �� �������� ũ�����ǰų� ������Ÿ���� �ٵǰ� ���� �� �����Ǵ� ������
		std::vector< AUTO_DAMAGE > m_vecAutoDamage;
        std::vector< TARGET_AUTO_DAMAGE > m_vecTargetAutoDamage;

		std::vector< TIME_SOUND > m_vecTimeSound;

        std::vector< DAMAGE_TRIGGER > m_vecTrigger;

		std::vector<CParticleEventSeqPTR> m_vecParticle;

        OnlyMotionDamage m_kOnlyMotionDamage;

		// ����Ŀ��� ���� �������� ���� ���� ����
		bool m_bIsContinousDamage;
		float m_fContinousDamage;

		// �÷��̵��� ����� �� ���� ���������� ����
		bool m_bNoPassDamage;
		// Ÿ�� ������ �������� ����
		int m_iDamageHP;
        std::vector<int> m_vecPermissionDamageHP;

		int m_iHitPerFrame;

		// ������ �ڽ��� �浹üũ�� ������ ������ �ڽ� ��ü�� �Ұ�����
		bool m_bIsTotalRange;
        bool m_bIsGroundBottomBox;

		// ���ο�ī��Ʈ ���� �����ϱ� ����... ������ ���������� ������ó�� �ٷ��� �� �� �Ϲ����� �� 8�� �ָ� �ʹ� ����
		// ������ �� �� �����ϰ� ������ �� �ֵ��� ���ο�ī��Ʈ �� �����ϴ� ������ ���� ����
		int m_iSlowCount;
		
		//====================================================================================
		// 2009.02.10 : Jemitgge
		// Comment : ������ ���ǰ� �浹 üũ ���ϴ� ������ ����!
		//			 GROUND_CRASH_AUTO_DAMAGE �̰� ����, �� üũ ���ϴ� ������ �ְ� �ʹ�.
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

        // ��.. ��Ű��������Ʈ�� �� ����
        std::vector<D3DXVECTOR3> m_vecMileStone;
        int         m_iIndex;
        int         m_iLifeMax;
        float       m_fS;
        bool        m_bOnUI;

		bool m_bApplyDamageSpeedByDamageDirection;

        // ���ݼ��� ����ó�� �������� ������.
        int m_iHaste;

        int m_iFatalRatio;
        bool m_bToggleMesh;
        int m_iToggleSummonMonsterID;
        int m_iDamageToTeleport;
        bool m_bIgnoreSpecialMonDamageToTeleport; // �������� Ư���ϰ� ���� ���� ���͵��� ������ ������ �����ϰ� �Ѵ�. �⺻������ ���� ����.
        int m_bShieldBreak;

        // �ʻ�� ������ �� ����ó��
        UINT m_uiNotSpecialSuper;
        bool m_bGasDamage;
        bool m_bIsCharLock;

        // ���� ��ȯ
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

        int m_iWhoStartMotion; //������ ������Ų �÷��̾��� �ٷ� ���� ���
		
        int m_iNextAttackCount;

        bool m_bParticleSlow;
        bool m_bOwnerKillDamage;  //  ������ �����ڰ� ������ �������� ���δ�.
        int m_iUniqueDamage;      // ������ �������� �̹� �°� �ִٸ� �� �������� ���� �ʰ� �ϰڴ�.

        // DA_TRACE_CREATOR�Ӽ��϶� ��ġ Offset
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

        // ������ �����ϰ� �浹üũ�ϴ� ������
        std::vector<int> m_vecCrashDamageToMonsterType;

        // ������ ���� ���ҵ����� ����
        DIVISION_DAMAGE m_kDivisionDamage;

        // �������� ���� �ٵ������� �ݻ�����ִ� ���
        BoundaryReflection  m_kBoundaryReflection;

        std::vector< int > m_vecTogetherDieBuff;

        //���Ͱ� è�Ǿ� �Ӽ��� ������ �־, �� �÷��װ� true ��� è�Ǿ� �Ӽ� ���� �����Ѵ�. 
        bool m_bIgnoreChampionProperty;

        // �� �Ӽ��� �ִ� �������� �¾Ƶ� �޺��� ������ �ʴ´�. 
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

    int     m_What;              ///< ������ ����Ʈ�� ����
    int     m_Who;               ///< ������ ����Ʈ�� �߻���Ų ���
	int		m_HitWho;			
	
	//	�� �� ������ ���� �۾��ؾߵǿ�.
	//	�켱 �ӽ÷� �����ΰ� ���߿� Damage ���� �ڵ忡 �� �� Ÿ���� �Ἥ
	//	�ڵ��Ҽ��ֵ��� �����ؾ� �մϴ�...
	OBJ_TYPE	m_emWhoType;	 ///< ����Ʈ �߻���Ų Ÿ��
	OBJ_TYPE	m_emHitWhoType;	 ///< ����Ʈ �������� Ÿ��
    
    DWORD   m_dwIdentifie;      ///< ��ũ�� ���纸������ �ĺ���( AddDamage�� �˾Ƽ� ����ũ�ϰ� �����ؾ��� )

	int     m_Life;              ///< ������ ����Ʈ�� ����
    int     m_PrevLife;              ///< ������ ���� ������
    int     m_SlowCount;         ///< ������(������)

    float   m_X;                ///< x ��ġ
    float   m_Y;                ///< y ��ġ

    D3DXVECTOR2   m_vDamageStartPos; ///< ������ ������ġ
    D3DXVECTOR2   m_vDamageWhoPos;    ///< ������ ������ �������� ��ġ

    float   m_Scale_x;          ///< ���� ���� ũ��
    float   m_Angle;            ///< ����
	float	m_fRatio;			// �������� ����

    float   m_fAnimScaleX;      ///< Animation�� ���� ũ��
    float   m_fAnimScaleY;      ///< Animation�� ���� ũ��

    bool    m_IsRight;           ///< ������ �����ΰ�?
    bool    m_IsShow;            ///< ���̴°�?
    int     m_TextureNoRenderCnt;///< �ؽ��İ� ������ �����ӵ��� ������ �ʴ´�.

    int     m_iTeam;             ///< ���� ���ΰ�?  1�̸� �����, 0�̸� ����, -1�̸� ��� �����̰�, ���͵� ���� ���� �� �ִ�.
	int     m_iLoopNum;			 /// �������� ������ ���긦 ����̳� �� �����ǰ�!
    int     m_iHitPerFrameCnt;   /// HIT_PER_FRAME�� ī���� ����
    bool    m_bCrashDamage;      /// �÷��̾ ���Ϳ� ũ���� �Ǿ����� true

    float   m_fDamageCrashX;     /// �������� �������� �浹�Ǿ����� ��� �������� �� ���������� �������� �ӵ��� X
    float   m_fDamageCrashY;     /// �������� �������� �浹�Ǿ����� ��� �������� �� ���������� �������� �ӵ��� Y
    float   m_fAccGravity;       /// ���� �߷�

    float   m_fAccGuidSpeedX;     /// ����ź�� ���̴� ���ӵ��� �����Ǵ� ����
    float   m_fAccGuidSpeedY;     /// ����ź�� ���̴� ���ӵ��� �����Ǵ� ����

    bool    m_bMoveCrashPos;     /// �������� �浹�� ������ �����������ڰ� �̵��ߴ°�?
    bool    m_bMoveUnitPos;     /// �������� �浹�� ������ġ�� �����������ڰ� �̵��ߴ°�?
    bool    m_bGroundCrash;      /// �������� �ѹ��̶� ���ǰ� �浹�ߴ°�?
    std::vector<int>  m_vecSummonMonHitWho;  /// ��ȯ ���� �������� �����༮�� ID

    //KSafeArray<bool, MAX_PLAYER_NUM+MAX_MONSTER > m_bAlreadyDamaged;  ///< �÷��̾ ���� �� ���� ��ü�� �ش��ϴ� ������ true �� �ȴ�.
	std::set<int> m_setAlreadyDamage; //�÷��̾ ���� �� ���� �� �ִ�.

    CParticleEventSeqPTR                             m_pParticle;
    CKTDGAnim::CAnimInstance*                       m_pAnim;

    std::vector< CParticleEventSeqPTR > m_vecChildParticle;

    //ǥ�� ������
    CDamage* m_pOriginalDamage; 

    int     m_ExtraMonsterNum;
    bool    m_IsMonsterDamage;   ///< ������ �������ΰ�?

    //����Ʈ�� ��Ʈ �ϵ� �ڵ�
    KSafeArray<int,MAX_PLAYER_NUM + 1> m_ExceptPlayer;   ///< �浹 �˻翡�� ������ �����

    bool    m_bPetDamage;        ///< ���� ���°�??
    bool    m_bPetTrace;         // �꿡 �پ� �ٴϴ� ������?
    bool    m_bPlayerTrace;      // �÷��̾�� �پ�ٴϴ� ������?
    // [12/5/2007] breadceo.3D �� �׸��� �ʹٰ�!!
    KGC3DObject* m_p3DObject;

    std::map< int, int > m_mapHitCount;
    std::vector< DWORD > m_vecLockChar;   // Is_character_lock���� Lockó���� �÷��̾� UID ����

    GCCollisionRect<float> m_rtCollision;  // ũ���� �Ǿ����� �浹�� �ڽ��� ����.

    // ������ ��ġ ����ȭ�� ���Ӽ��� �� ������ ID�� ����ȴ�.
    std::vector<int> m_vecPlayerSyncDamagePosAtCrashPos;
    std::vector<int> m_vecMonsterSyncDamagePosAtCrashPos;

    D3DXVECTOR2 m_dxvTargetPos;         // ������ ���� ������ �� �� Ÿ�� ��ġ 
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
