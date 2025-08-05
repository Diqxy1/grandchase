#include "stdafx.h"
#include ".\damage.h"


#include "Monster.h"
#include "Spark.h"



#include "GCCameraOrtho.h"
#include "DamageManager.h"
#include "LatencyTransfer.h"
#include "Controls.h"
//
#include "KGCRoomManager.h"
#include "KGC3DObject.h"
#include "KGC3DObjectMgr.h"
#include "Buff/KGCBuffManager.h"
#include "QuestGameManager.h"
#include "GCGrowingSkillManager.h"
#include "Stage.h"
#include "Message.h"

boost::pool<> CDamageInstance::bpool(sizeof(CDamageInstance));

CDamageInstance::~CDamageInstance()
{
    m_vecChildParticle.clear();
}

CDamage::CDamage(void) : m_Start_Life( 255 ), m_Life_Change( -1 ), m_Start_Scale_x( 1.0f ), m_Scale_Speed( 1.0f ),
                        m_Y_per_x( 1.0f ), m_Start_Locate_x( 0.0f ), m_Start_Locate_y( 0.0f ), m_fAnimStartPosX( 0.0f ), 
                        m_fAnimStartPosY( 0.0f ), m_x_Speed( 0.0f ), m_y_Speed( 0.0f ), m_Start_Angle( 0.0f ),
                        m_Angle_Speed( 0.0f ), m_Texture( GC_TEX_INVISIBLE ), m_bIsCorruptedDamage( false ), m_fDamageToPlayer( 0.0f ), m_fDamageToMonster( 0.0f ),
                        m_IsEnableDamage( false ), m_IsAngleMove( false ), m_IsRealTimeAngleMove( false ), m_IsAngleMoveWithOffset( false ), 
                        m_iDamageMotion( MID_COMMON_SMALLDAMAGE ), m_fDMXSpeed( -1.0f ),
                        m_fDMYSpeed( -1.0f ), m_dwDMAttribute( 0 ), m_iCrashSound( -1 ),
                        m_iMagicEffect( -1 ), m_fMagicEffTime( 0.0f ), m_iEarthQuake( 0 ), m_iCrashStart( 0 ), m_iCrashEnd( 0 ),
                        m_particleSeqName( "" ), m_EmitMin( 0.0f ), m_EmitMax( 0.0f ), m_bAddNoDirection( false ), 
                        m_AnimSeqName( "" ), m_bAnimLoop( false ), m_fAnim_Start_ScaleX( 1.0f ), m_fAnim_Start_ScaleY( 1.0f ), 
                        m_fAnimScaleX_Speed( 1.0f ), m_fAnimScaleY_Speed( 1.0f ), m_fAnim_Limit_ScaleX( 1.0f ), m_fAnim_Limit_ScaleY( 1.0f ),
                        m_fAnimAngle( 0.0f ), m_iParent( -1 ), m_bDirection( false ), m_SRCBlendMode( D3DBLEND_SRCALPHA ),
                        m_DESTBlendMode( D3DBLEND_INVSRCALPHA ), m_iJumpPossibleFrame( 0 ), m_fStoneMaxDmg( 0.0f ), m_fStoneMinDmg( 0.0f ),
                        m_fDecreasePerPush( 0.0f ),m_IsFixedAngleMove( false ), m_bParticleTrace( false ), m_bMotionTrace( false ),
                        m_iPlusDelay(0), m_iTracePosType(-1), m_iCannotCatchFrame(0), m_iNoCheckContact(-1), m_iNoCheckContactDamage(-1),
                        m_iAttackType( ATTACKTYPE_NORMAL ),m_iAttackTypeOnAerial(ATTACKTYPE_NORMAL), m_iAttackDirection( ATTACKDIR_MIDDLE ), m_fPushTargetX( 0.0f ), m_fPushTargetY( 0.0f ), 
						m_bSpecialAttack( false ), m_bSlotUniqueSkillDmg( false ), m_bSpecial4Damage( false ), m_bIsContinousDamage( false ), m_fContinousDamage( 0 ), m_bNoPassDamage( false ), 
						m_iDamageHP( 0 ), m_iHitPerFrame(0), m_bIsTotalRange(false), m_bIsGroundBottomBox(false), m_iSlowCount( 0 ), m_bMotionXTrace( true ), m_bMotionYTrace( true ),
                        m_bUserFrameDependant( false ),m_bUserMotionDependant( false ), m_fGravity( 0.0f ), m_bNoAlphaChange( false ),m_fLeechHP( 0.0f ),m_fLeechMP( 0.0f ),m_fAbsorbHP( 0.0f ),
                        m_iMagicLevel(1) , m_bAlwaysEnemy( false ), m_fMpChange( 0.0f ),m_bOwnerDirection( false ), m_iMaxInstanceNum( 0 ), m_bStopAtContact(false), m_bCheckBoundary(false),
                        m_iIndex(0), m_iLifeMax(1), m_fS(0), m_bOnUI(false),
                        m_bPlayerXTrace(true), m_bPlayerYTrace(true), m_iStopMotionTraceLife(-1),m_iMonsterMagicEff( -1 ),m_bApplyDamageSpeedByDamageDirection( false ),
                        m_iHaste(0), m_iFatalRatio(0), m_bToggleMesh(false), m_iToggleSummonMonsterID(0), m_bCreateCheckBoundary( false ), m_iDamageToTeleport( -1 ), m_bShieldBreak( false ),
                        m_fAbsorbHPToMaxHP( 0.0f ), m_iAddAbsorbHPPerHit( 0 ), m_x_CrashSpeed( 0.0f ), m_y_CrashSpeed( 0.0f ),  m_IsStaticDamageCleaner( false ), 
                        m_fDamageCrashSpeedX( 0.0f ), m_fDamageCrashSpeedY( 0.0f ), m_IsStaticDamage( false ), m_uiNotSpecialSuper( 0 ), m_iAPMagicEffect( -1 ), m_bGasDamage( false ), m_bEndAtContact( false ),
                        m_iUnMagicEffect( -1 ), m_iWhoStartMotion( -1 ), m_iNextAttackCount(0), m_bParticleSlow( false ), m_fLimite_Angle( 0.0f ), m_bNoLocalMotion( false ), m_bOwnerKillDamage(false),
                        m_fTraceCreateOffsetX( 0.0f ), m_fTraceCreateOffsetY( 0.0f ), m_Reverse_Life( 0 ),  m_iEarthQuakeY( 0 ), m_bIsCharLock( false ), m_bUniqueMgicEff( false ), m_iUniqueDamage( -1 ), m_bCheckNoPassFloor( false ),
                        m_bMonSuperDamage( false ), m_bMonTargettingRelease( false ), m_bNoCrashPlayer( false ), m_bNoCrashMonster( false ), m_fGround_Offset_X( 0.0f ), m_fGround_Offset_Y( 0.0f ), 
                        m_bOnlyOneUnit( false ), m_bTogetherDie( false ), m_iStopUnitFrame( 0 ), m_bNoPushSpeedToFatal( false ), m_nSpecialGrade( 0 ), m_bIgnoreDefDamage( false ), m_bTraceMonsterBoundBox( false ), m_iNoCrashUnMagicEffect( -1 ), m_fAbsorbHPToDamage( 0.0f ),
						m_fSameTeamDamageToMonster( 0.0f ), m_bFastHostMagicEff( false ), m_iEarthQuakeRunFrame( -1 ), m_TextureNoRenderCnt( 0 ), m_bParticleAngleMove( false ), m_bAutoDamageToGroundCrashOfNoDieCurDamage( false ), m_bAccGravity( false ), m_bEndAtLastBottomContact( false ),
                        m_bDeleteSync( false ), m_bAttackToSameTeam( false ), m_bIgnoreSuperDamage( false ), m_x_DecSpeed( 1.0f ), m_y_DecSpeed( 1.0f ), m_bNoCrashFatal( false ), m_bNoCrashDamage( false ), m_bNoCrashGameObject( false ), m_bNoCrash2DObject( false ), m_bStopAtLastBottomContact( false ),
                        m_iIgnoreGroundCheckHittedObject( -1 ), m_bIgnoreSpecialMonDamageToTeleport( false ), m_bIgnoreChampionProperty( false ), m_bNotComboBreak( false )
{
    m_vecChildParticle.clear();
    m_vecDamageHpParticle.clear();
    m_vecInToggleMeshParticle.clear();
    m_vecOutToggleMeshParticle.clear();
    m_vecInToggleSummonMonsterMeshParticle.clear();
    m_vecOutToggleSummonMonsterMeshParticle.clear();
    m_vecMPDamageParticle.clear();
    m_MeshInfo.Init();
    m_vecCrashParticle.clear();	
	m_vecAutoDamage.clear();
    m_vecTargetAutoDamage.clear();
	m_vecTimeSound.clear();
	m_vecParticle.clear();
    m_vecMileStone.clear();
    m_kRestrictHitCnt.Init();
    m_kVariousIncreaseRatio.Init();
    m_setDamageFlag.clear();
    m_vecRandMagicEffect.clear();	   
    m_vecPassBuff.clear();
    m_vecPassNoBuff.clear();
    m_pairSkipInfo = std::make_pair(0, 0);
    m_kMonReaction.Init();
    m_mapNoCreateExceptionGameMode.clear();
    m_vecPermissionDamageHP.clear();
    m_kDamageToDamage.Init();

    m_vecTogetherDieDamage.clear();
    m_vecCrashDamageToMonsterType.clear();
    m_vecTogetherDieBuff.clear();

    memset(m_Start_Locate_Random_x, 0, sizeof(float) * 2 );
    memset(m_Start_Locate_Random_y, 0, sizeof(float) * 2 );
    memset(m_Start_Locate_Frame_x, 0, sizeof(float) * 2 );
    memset(m_Start_Locate_Frame_y, 0, sizeof(float) * 2 );
}

CDamage::~CDamage(void)
{
    m_vecChildParticle.clear();
    m_vecDamageHpParticle.clear();
    m_vecInToggleMeshParticle.clear();
    m_vecOutToggleMeshParticle.clear();
    m_vecInToggleSummonMonsterMeshParticle.clear();
    m_vecOutToggleSummonMonsterMeshParticle.clear();
    m_vecMPDamageParticle.clear();
    m_MeshInfo.Init();
    m_vecCrashParticle.clear();
	m_vecAutoDamage.clear();
    m_vecTargetAutoDamage.clear();
	m_vecTimeSound.clear();
	m_vecParticle.clear();
}

void CDamage::Begin( CDamageInstance* pDamageInstance )
{
    MONSTER *pMonster = NULL;
    if ( pDamageInstance->m_IsMonsterDamage )
    {
		pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
		if(pMonster != NULL){
			//�������� ���� ��ġ �������� �޶����� ��������...
			if( m_IsAngleMove )
			{
				float Length = ASMsqrt( powf( m_Start_Locate_x, 2.0f ) + powf( m_Start_Locate_y, 2.0f ));
				float Angle = 0.0f;

				Angle = pMonster->Body_Angle * 0.8f + asinf( m_Start_Locate_y / Length );

				if( pDamageInstance->m_IsRight )
				{
					pDamageInstance->m_X = pMonster->m_afX[0] + cosf(Angle) * Length * 0.1f;
					pDamageInstance->m_Angle =  m_Start_Angle + pMonster->Body_Angle;
				}
				else
				{
					pDamageInstance->m_X = pMonster->m_afX[0] - cosf(Angle) * Length * 0.1f;
					pDamageInstance->m_Angle = -m_Start_Angle - pMonster->Body_Angle;
				}
				pDamageInstance->m_Y = pMonster->m_afY[0] + m_Start_Locate_y - 0.02f;
			} 
			else
			{
				float fStartLocateX = m_Start_Locate_x;
				float fStartLocateY = m_Start_Locate_y;
				if( 0.0f != m_Start_Locate_Random_x[0] || 0.0f != m_Start_Locate_Random_x[1] )
					fStartLocateX = RandomNumber(m_Start_Locate_Random_x[0], m_Start_Locate_Random_x[1]);
				if( 0.0f != m_Start_Locate_Random_y[0] || 0.0f != m_Start_Locate_Random_y[1] )
					fStartLocateY = RandomNumber(m_Start_Locate_Random_y[0], m_Start_Locate_Random_y[1]);

				//����� ���������� ����
				if( pDamageInstance->m_IsRight )
				{
					pDamageInstance->m_X = pMonster->m_afX[0] + fStartLocateX;
					pDamageInstance->m_Angle = m_Start_Angle;
				}
				else
				{
					pDamageInstance->m_X = pMonster->m_afX[0] - fStartLocateX;
					pDamageInstance->m_Angle = -m_Start_Angle;
				}
				pDamageInstance->m_Y = pMonster->m_afY[0] + fStartLocateY;
			}
		}
    }
    else
    {
        if( m_IsAngleMove )
        {
            float Length = ASMsqrt( powf( m_Start_Locate_x, 2.0f ) + powf( m_Start_Locate_y, 2.0f ));
            float Angle = 0.0f;
            Angle = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle * 0.8f + asinf( m_Start_Locate_y / Length );
    
            if( pDamageInstance->m_IsRight )
            {
                pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x + cosf(Angle) * Length * 0.1f;
                pDamageInstance->m_Angle =  m_Start_Angle + g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
            }
            else
            {
                pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x - cosf(Angle) * Length * 0.1f;
                pDamageInstance->m_Angle = -m_Start_Angle - g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
            }
            pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + m_Start_Locate_y - 0.02f;
        }
        else if( m_IsAngleMoveWithOffset )
        {
            float Length = ASMsqrt( powf( m_Start_Locate_x, 2.0f ) + powf( m_Start_Locate_y, 2.0f ));        
            float Angle = 0.0f;
            Angle = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle * 0.75f;// + asinf( m_Start_Locate_y / Length );

            if( pDamageInstance->m_IsRight )
            {
                pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x + cosf(Angle) * Length - Length + m_Start_Locate_x;
                pDamageInstance->m_Angle =  g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
            }
            else
            {
                pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x - cosf(Angle) * Length + Length - m_Start_Locate_x;
                pDamageInstance->m_Angle = -g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
            }

            if( Angle >= 0 ) {
                pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + m_Start_Locate_y + sinf(Angle) * Length - 0.02f;
            } else {
                pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + m_Start_Locate_y + sinf(Angle) * Length + 0.02f;
            }
        }
        else
        {
            float fStartLocateX = m_Start_Locate_x;
            float fStartLocateY = m_Start_Locate_y;
            if( 0.0f != m_Start_Locate_Random_x[0] || 0.0f != m_Start_Locate_Random_x[1] )
                fStartLocateX = RandomNumber(m_Start_Locate_Random_x[0], m_Start_Locate_Random_x[1]);
            if( 0.0f != m_Start_Locate_Random_y[0] || 0.0f != m_Start_Locate_Random_y[1] )
                fStartLocateY = RandomNumber(m_Start_Locate_Random_y[0], m_Start_Locate_Random_y[1]);
            if( 0.0f != m_Start_Locate_Frame_x[0] || 0.0f != m_Start_Locate_Frame_x[1] )
                fStartLocateX = (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - m_Start_Locate_Frame_x[0]) * m_Start_Locate_Frame_x[1];
            if( 0.0f != m_Start_Locate_Frame_y[0] || 0.0f != m_Start_Locate_Frame_y[1] )
                fStartLocateY = (g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame - m_Start_Locate_Frame_y[0]) * m_Start_Locate_Frame_y[1];

            //����� ���������� ����
            if( pDamageInstance->m_IsRight )
            {
                pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x + fStartLocateX;
                pDamageInstance->m_Angle = m_Start_Angle;            
            }
            else
            {
                pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x - fStartLocateX;
                pDamageInstance->m_Angle = -m_Start_Angle;
            }
            pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + fStartLocateY;
        }
    }

    // �������� �ۿ� �����Ǵ� ��������� �������� ������ �Ű��ִ� �ɼ�
    if( pDamageInstance->m_pOriginalDamage->m_bCreateCheckBoundary )
    {
        CreateCheckBoundary( pDamageInstance );
    }

    BeginAnim(pDamageInstance);
    BeginParticle(pDamageInstance);

    //  �������ڸ��� ���� ��� �ǰ� ����
    for( int i = 0 ; i<(int)pDamageInstance->m_pOriginalDamage->m_vecTimeSound.size(); ++i )
    {
        TIME_SOUND& timeSound = pDamageInstance->m_pOriginalDamage->m_vecTimeSound[i];
        
        if ( timeSound.m_iPlayTime == -1 ) 
        {
            char strTemp[15];
            sprintf( strTemp , "%d" , timeSound.m_iSoundNumber );
            g_KDSound.Play( strTemp, timeSound.m_bLoop );
        }
    }

    if( -1 != pDamageInstance->m_pOriginalDamage->m_GuidedMissile.m_iGMCondition )
        pDamageInstance->m_pOriginalDamage->CreateGuidedMissileTarget( pDamageInstance );

    pDamageInstance->m_vDamageStartPos = D3DXVECTOR2( pDamageInstance->m_X, pDamageInstance->m_Y );
    if( pMonster )
        pDamageInstance->m_vDamageWhoPos = D3DXVECTOR2( pMonster->GetX(), pMonster->GetY() );
    else
        pDamageInstance->m_vDamageWhoPos = D3DXVECTOR2( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetX(), g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetY() );
}

void CDamage::CreateCheckBoundary( CDamageInstance* pDamageInstance )
{
    float fMax_X = SiKGCRoomManager()->IsMonsterGameMode() ? SiKGCFantasticMap()->GetPixelWidth() : 1.5f * (float)g_MyD3D->MyStg->Num_Width;
    float fMax_Y = SiKGCRoomManager()->IsMonsterGameMode() ? SiKGCFantasticMap()->GetPixelHeight() : 1.5f * (float)g_MyD3D->MyStg->Num_Height;

    if ( pDamageInstance->m_X < 0.0f )
    {
        pDamageInstance->m_X = 0.0f + pDamageInstance->m_Scale_x;
    }
    else if ( pDamageInstance->m_X > fMax_X )
    {
        pDamageInstance->m_X = fMax_X - pDamageInstance->m_Scale_x;
    }

    if ( pDamageInstance->m_Y < 0.0f )
    {
        pDamageInstance->m_Y = 0.0f + (pDamageInstance->m_Scale_x * pDamageInstance->m_pOriginalDamage->m_Y_per_x);
    }
}

void CDamage::BeginWithAngle( CDamageInstance* pDamageInstance , float fStartAngle )
{
    float Length = ASMsqrt( powf( m_Start_Locate_x, 2.0f ) + powf( m_Start_Locate_y, 2.0f ));
    float Angle = 0.0f;
    Angle = fStartAngle * 0.8f + asinf( m_Start_Locate_y / Length );

    if (pDamageInstance->m_IsMonsterDamage)
	{		
		MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
		if(pMonster != NULL){

			if( pDamageInstance->m_IsRight )
			{
				pDamageInstance->m_X = pMonster->m_afX[0] + cosf(Angle) * Length * 0.1f;
				pDamageInstance->m_Angle =  m_Start_Angle + fStartAngle;            
			}
			else
			{
				pDamageInstance->m_X = pMonster->m_afX[0] - cosf(Angle) * Length * 0.1f;
				pDamageInstance->m_Angle = -m_Start_Angle - fStartAngle;                
			}
			pDamageInstance->m_Y = pMonster->m_afY[0] + m_Start_Locate_y - 0.02f;
		}
    }
    else
    {
        if( pDamageInstance->m_IsRight )
        {
            pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x + cosf(Angle) * Length * 0.1f;
            pDamageInstance->m_Angle =  m_Start_Angle + fStartAngle;            
        }
        else
        {
            pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x - cosf(Angle) * Length * 0.1f;
            pDamageInstance->m_Angle = -m_Start_Angle - fStartAngle;                
        }
        pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + m_Start_Locate_y - 0.02f;
    }

    BeginAnim(pDamageInstance);
    BeginParticle(pDamageInstance);

    if( -1 != pDamageInstance->m_pOriginalDamage->m_GuidedMissile.m_iGMCondition )
        pDamageInstance->m_pOriginalDamage->CreateGuidedMissileTarget( pDamageInstance );
}

void CDamage::BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY )
{
    float fStartLocateX = m_Start_Locate_x;
    float fStartLocateY = m_Start_Locate_y;
    if( 0.0f != m_Start_Locate_Random_x[0] || 0.0f != m_Start_Locate_Random_x[1] )
        fStartLocateX = RandomNumber(m_Start_Locate_Random_x[0], m_Start_Locate_Random_x[1]);
    if( 0.0f != m_Start_Locate_Random_y[0] || 0.0f != m_Start_Locate_Random_y[1] )
        fStartLocateY = RandomNumber(m_Start_Locate_Random_y[0], m_Start_Locate_Random_y[1]);

    if( pDamageInstance->m_IsRight )
    {
        pDamageInstance->m_X = fStartX + fStartLocateX;
        pDamageInstance->m_Angle = m_Start_Angle;            
    }
    else
    {
        pDamageInstance->m_X = fStartX - fStartLocateX;
        pDamageInstance->m_Angle = -m_Start_Angle;
    }
    pDamageInstance->m_Y = fStartY + fStartLocateY;

    BeginAnim(pDamageInstance);
    BeginParticle(pDamageInstance);

    if( -1 != pDamageInstance->m_pOriginalDamage->m_GuidedMissile.m_iGMCondition )
        pDamageInstance->m_pOriginalDamage->CreateGuidedMissileTarget( pDamageInstance );
}



void CDamage::BeginAnim( CDamageInstance* pDamageInstance )
{
    std::string tempAnim;

    if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
        tempAnim = m_AnimSeqName;
    else
        tempAnim = "";

    pDamageInstance->m_pAnim = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( tempAnim ) );

    if ( !pDamageInstance->m_pAnim )
    {
        pDamageInstance->m_pAnim = g_AnimManager.GetAnimSequence( (char*) tempAnim.c_str() );
    }

    if( pDamageInstance->m_pAnim != NULL )
    {
        if( m_bAnimLoop )
            pDamageInstance->m_pAnim->Start( CKTDGAnim::PT_LOOP );
        else
            pDamageInstance->m_pAnim->Start( CKTDGAnim::PT_ONE_PASS );

#if defined(RESOURCE_GRAPH)
		char tmp2[256];		
		sprintf(tmp2,"DAMAGE_%d",pDamageInstance->m_What);
		g_pGCDeviceManager2->AddRelation("ANIM_"+ tempAnim,tmp2);
#endif

    }
}

void CDamage::BeginParticle( CDamageInstance* pDamageInstance )
{
    if (m_dwDMAttribute & DA_TRACE_CREATOR || m_dwDMAttribute & DA_TRACE_HITTED )
    {
        SetTraceCreator( pDamageInstance );
    }

    std::string tempParticle;

    if( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
        tempParticle = g_pParticleChangeSys->GetParticleChageString( pDamageInstance->m_Who, m_particleSeqName );
    else
        tempParticle = m_particleSeqName;

    if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
        tempParticle = "";

    CHILD_PARTICLE stParticle;
    stParticle.m_particleSeqName = tempParticle.c_str();
    stParticle.m_EmitMin = m_EmitMin;
    stParticle.m_EmitMax = m_EmitMax;

    if( m_particleSeqName.empty() )
        return;

    pDamageInstance->m_pParticle = SetParticle( pDamageInstance, stParticle );
}


CParticleEventSeqPTR CDamage::SetParticle( CDamageInstance* pDamageInstance, CHILD_PARTICLE& stParticle )
{
    std::string strParticleName;

    float fXOffset, fYOffset;
    if( pDamageInstance->m_IsRight )
        fXOffset = stParticle.m_fXOffset;
    else
        fXOffset = -stParticle.m_fXOffset;
    fYOffset = stParticle.m_fYOffset;

    CParticleEventSeqPTR pResult;
	
	if( *( stParticle.m_particleSeqName.end() - 1 ) == '/' )
	{
		stParticle.m_particleSeqName.erase(stParticle.m_particleSeqName.end()-1);
		strParticleName = stParticle.m_particleSeqName;

        if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
            strParticleName = "";

		if ( !pDamageInstance->m_IsRight )
		{
			strParticleName += "_R";
			pResult = pDamageInstance->CreateSequence( strParticleName, pDamageInstance->m_X - 1.0f + fXOffset, pDamageInstance->m_Y - 0.5f + fYOffset, 0.5f );
		}
	}

	if ( !pResult )
	{
		strParticleName = stParticle.m_particleSeqName;

        if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
            strParticleName = "";

		pResult = pDamageInstance->CreateSequence(strParticleName, pDamageInstance->m_X - 1.0f + fXOffset, pDamageInstance->m_Y - 0.5f + fYOffset, 0.5f );
	}

    if( pResult )
    {
        // ���� ����Ʈ ���� - ���� ����Ʈ �̸鼭 �����ڰ� ������ �ƴϸ� �׸��� ����
        if( stParticle.m_bIsLocal && g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->IsLocalPlayer() == false ) {
            pResult->SetShow( false );
        }

		// ���⼺ ����
		DIRECTION_INFO stDirect;
		stDirect.m_fDirPosX = 0.0f;
		stDirect.m_bDirVelocity = m_bDirection;
		stDirect.m_bReverseTexture = stParticle.m_bReverseTexture;
        
        if( stParticle.m_bSyncCreatorDir ) {
            stDirect.m_pbIsRight = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->GetPlayerDir();
        } else {
		    if( stParticle.m_bDieTogether == true )
		    {
			    stDirect.m_pbIsRight = &pDamageInstance->m_IsRight;
		    }
		    else
		    {
			    stDirect.m_bIsRight = pDamageInstance->m_IsRight;
		    }
        }

		if( pDamageInstance->m_pOriginalDamage->m_bAddNoDirection == true )
			stDirect.m_bReverseTexture = false;
		else
			stDirect.m_bReverseTexture = true;

		pResult->SetDirectInfo( &stDirect );
        if( stParticle.m_EmitMin != 0 || stParticle.m_EmitMax != 0 )
        {
		    pResult->SetEmitRate( CMinMax<float>(stParticle.m_EmitMin, stParticle.m_EmitMax) );
        }

		if( pDamageInstance->m_pOriginalDamage->m_bParticleTrace == true /*&& stParticle.m_bDieTogether == true */)
		{
			TRACE_INFO stTrace;
			if ( true == stParticle.m_bWithAngle )
			{
				PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
				float fAngle = pPlayer->Body_Angle;

				D3DXVECTOR3 vOffsetPos( fXOffset, fYOffset, 0.5f );
				stTrace.m_fSpeedAngle = pDamageInstance->m_pOriginalDamage->m_Angle_Speed;

				if( !pPlayer->bIsRight )
				{
					fAngle = D3DX_PI - fAngle;
					vOffsetPos.x *= -1.0f;
					stTrace.m_fSpeedAngle *= -1.0f;
				}
				
				D3DXVECTOR3 vDestPos;
				
				D3DXMATRIX matRot;
				D3DXMatrixIdentity( &matRot );
				D3DXMatrixRotationZ( &matRot, fAngle );    
				D3DXVec3TransformCoord( &vDestPos, &vOffsetPos, &matRot );
				
				stTrace.m_pvPos = &pDamageInstance->m_pOriginalDamage->m_vPos;
				stTrace.m_fParticleTraceTime = pDamageInstance->m_Life * 0.018181818f;
				stTrace.m_fXOffset = vDestPos.x;
				stTrace.m_fYOffset = vDestPos.y;
				stTrace.m_fParticleTraceTime = stParticle.m_fParticleTraceTime;
			}
			else
			{
				stTrace.m_pvPos = &pDamageInstance->m_pOriginalDamage->m_vPos;
				stTrace.m_fParticleTraceTime = pDamageInstance->m_Life * 0.018181818f;
				stTrace.m_fXOffset = fXOffset;
				stTrace.m_fYOffset = fYOffset;
				stTrace.m_fParticleTraceTime = stParticle.m_fParticleTraceTime;
			}

			pDamageInstance->m_pOriginalDamage->m_vPos.x = pDamageInstance->m_X - 1.0f;
			pDamageInstance->m_pOriginalDamage->m_vPos.y = pDamageInstance->m_Y - 0.5f;
			pDamageInstance->m_pOriginalDamage->m_vPos.z = 0.5f;
			
			pResult->SetTrace( &stTrace );
		}

		if( pResult->IsBlackHoleEventHere() == true )
		{
			pResult->SetBlackHolePosition( &pDamageInstance->m_pOriginalDamage->m_vPos );
		}
    }   
    
    if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
        pResult->SetShow( true );
    else
        pResult->SetShow( false );

    return pResult;
}

void CDamage::End( CDamageInstance* pDamageInstance )
{
    if( pDamageInstance->m_pAnim != NULL )
    {
        g_MyD3D->m_pMapAnim->DeleteInst( pDamageInstance->m_pAnim );
        pDamageInstance->m_pAnim = NULL;
    }
    if( pDamageInstance->m_pParticle )
    {
        //��� �������� �� �׷��� ��۰Ÿ��� �����Դϴ�.
        g_ParticleManager->DeleteSequence(pDamageInstance->m_pParticle);
    }

    if( !pDamageInstance->m_vecChildParticle.empty() )
    {
		g_ParticleManager->DeleteSequence( pDamageInstance->m_vecChildParticle );
    }

    if( pDamageInstance->m_p3DObject != NULL )
    {
        SiKGC3DObjectMgr()->Remove3DObject( pDamageInstance->m_p3DObject );
    }

	for ( std::vector<CParticleEventSeqPTR>::iterator vit = m_vecParticle.begin(); vit != m_vecParticle.end(); vit++ )    
	{
		g_ParticleManager->DeleteSequence( *vit );
		(*vit) = NULL;
	}
	m_vecParticle.clear();
    pDamageInstance->m_mapHitCount.clear();

    //  ����ɶ� ���� ����
    for( int i = 0 ; i<(int)pDamageInstance->m_pOriginalDamage->m_vecTimeSound.size(); ++i )
    {
        TIME_SOUND& timeSound = pDamageInstance->m_pOriginalDamage->m_vecTimeSound[i];

        if ( timeSound.m_bStop ) 
        {
            char strTemp[15];
            sprintf( strTemp , "%d" , timeSound.m_iSoundNumber );
            g_KDSound.Stop( strTemp );
        }
    }
}

void CDamage::FrameMove( CDamageInstance* pDamageInstance )
{
    if( pDamageInstance->m_pOriginalDamage->m_kVariousIncreaseRatio.fScale_X_Ratio != 0.f ) {
        pDamageInstance->m_Scale_x += pDamageInstance->m_pOriginalDamage->m_kVariousIncreaseRatio.fScale_X_Ratio;
    }

    if( pDamageInstance->m_pOriginalDamage->m_kVariousIncreaseRatio.fY_Per_X_Ratio != 0.f ) {
        pDamageInstance->m_pOriginalDamage->m_Y_per_x += pDamageInstance->m_pOriginalDamage->m_kVariousIncreaseRatio.fY_Per_X_Ratio;
    }

    if( pDamageInstance->m_pOriginalDamage->m_kVariousIncreaseRatio.bNoFrameMove ) {
        return;
    }

    if ( m_vecMileStone.size() > 2 )
    {
        ValkyrieFrameMove( pDamageInstance );
        goto NoTraceNoGravity;
    }
    else
    {
        if( m_IsAngleMove || m_IsFixedAngleMove || m_IsRealTimeAngleMove || m_IsAngleMoveWithOffset )
        {
            AngleFrameMove( pDamageInstance );
        }
        else if( 0 < m_GuidedMissile.m_iGMCondition )
        {
            GuideMissileFrameMove( pDamageInstance );
        }
        else
        {
            NotAngleFrameMove( pDamageInstance );
        }
    }

    // �������� �������� �浹������ ��뵥������ �������� �ӵ��� ���Ϸ������Ƿ� �ʱ�ȭ
    pDamageInstance->SetDamageCrashSpeed( 0.0f, 0.0f );

    // ������ �ӵ��� ��ȭ��Ų��.
    pDamageInstance->m_pOriginalDamage->m_x_Speed *= pDamageInstance->m_pOriginalDamage->m_x_DecSpeed;
    pDamageInstance->m_pOriginalDamage->m_y_Speed *= pDamageInstance->m_pOriginalDamage->m_y_DecSpeed;

    //AngleMove�� ������ Gravity�� ������ڲٳ�.
    {
        float fAge = -1.0f;
        float fGravity = pDamageInstance->m_pOriginalDamage->m_fGravity;
        if( pDamageInstance->m_pOriginalDamage->m_bAccGravity )
        {
            pDamageInstance->m_fAccGravity += pDamageInstance->m_pOriginalDamage->m_fGravity;
            fGravity = pDamageInstance->m_fAccGravity;
        }
        else
        {
            int iChangeLife = pDamageInstance->m_pOriginalDamage->m_Life_Change;
            if( 0 == iChangeLife ) iChangeLife = -1;
            fAge =(float)( (pDamageInstance->m_pOriginalDamage->m_Start_Life - pDamageInstance->m_Life ) / iChangeLife );
        }

        pDamageInstance->m_Y += (fAge * fGravity);
    }
    
    pDamageInstance->m_Scale_x *= m_Scale_Speed;    

    //���� �ٳ�� �� �̹������� �׻� ����ٴϵ��� ��ǥ�� ���߾��ش�.
    if (m_dwDMAttribute & DA_TRACE_CREATOR || m_dwDMAttribute & DA_TRACE_HITTED )
    {
        SetTraceCreator( pDamageInstance );
    }

NoTraceNoGravity:
    if( pDamageInstance->m_p3DObject != NULL && pDamageInstance->m_p3DObject->GetIsLive() == true )
    {
        if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
        {
            pDamageInstance->m_p3DObject->SetRender( false );
        }

        D3DXVECTOR2& vPos = pDamageInstance->m_p3DObject->GetMotionPos();

        if (m_dwDMAttribute & DA_TRACE_CREATOR)
        {
            if( pDamageInstance->m_pOriginalDamage->m_bPlayerXTrace == false )
            {
                if( pDamageInstance->m_IsRight )
                    pDamageInstance->m_X += (vPos.x ) ;
                else
                    pDamageInstance->m_X -= (vPos.x ) ;
            }
            else
            {
                if( pDamageInstance->m_IsRight )
                    pDamageInstance->m_X += (vPos.x + m_vPrevMeshMotionPos.x) ;
                else
                    pDamageInstance->m_X -= (vPos.x + m_vPrevMeshMotionPos.x) ;
            }            

            if( pDamageInstance->m_pOriginalDamage->m_bPlayerYTrace == false )
                pDamageInstance->m_Y += (vPos.y - m_vPrevMeshMotionPos.y ) ;
            else
                pDamageInstance->m_Y += (vPos.y ) ;
        }
        else
        {
            if( pDamageInstance->m_IsRight )
                pDamageInstance->m_X += (vPos.x ) ;
            else
                pDamageInstance->m_X -= (vPos.x ) ;
            pDamageInstance->m_Y += (vPos.y - m_vPrevMeshMotionPos.y ) ;
        }		

		// Ư�������ӿ� ��ƼŬ�� �����Ѵ�.
		for( int i =0; i < (int)pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle.size(); ++i )
		{
            D3DXMATRIX matBone;
			MESH_CHILD_PARTICLE& meshChildParticle = pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i];


            if( meshChildParticle.m_iBoneIndex != -1 && (int)pDamageInstance->m_p3DObject->GetFrame() >= meshChildParticle.m_iCreateFrame )
            {
                pDamageInstance->m_p3DObject->GetGCObject()->SetMotionMatrix( true, NULL , false , true );
                pDamageInstance->m_p3DObject->GetGCObject()->GetAngleBone( meshChildParticle.m_iBoneIndex, &matBone );
                matBone *= *( pDamageInstance->m_p3DObject->GetGCObject()->GetWorldMat() );

                if ( pDamageInstance->m_IsRight )
                {
                    pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.x = matBone._41 - meshChildParticle.m_fOffSetX; 
                    pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.y = matBone._42 + meshChildParticle.m_fOffSetY; 
                    pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.z = matBone._43;
                }
                else
                {
                    pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.x = matBone._41 + meshChildParticle.m_fOffSetX;
                    pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.y = matBone._42 + meshChildParticle.m_fOffSetY;
                    pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.z = matBone._43;
                }
            }

            if( meshChildParticle.m_bDotParticle && (int)pDamageInstance->m_p3DObject->GetFrame() >= meshChildParticle.m_iCreateFrame )
            {
                if( pDamageInstance->m_IsRight )
                {
                    if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                        pDamageInstance->CreateSequence( meshChildParticle.m_strParticle.c_str(), 
                        pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.x + meshChildParticle.m_fOffSetX, 
                        pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.y + meshChildParticle.m_fOffSetY, 0.5f );                
                }
                else
                {
                    if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                        pDamageInstance->CreateSequence( meshChildParticle.m_strParticle.c_str(), 
                        pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.x - meshChildParticle.m_fOffSetX, 
                        pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos.y + meshChildParticle.m_fOffSetY, 0.5f );                
                }                
            }

			if( pDamageInstance->m_p3DObject->GetFrame() != meshChildParticle.m_iCreateFrame || 
				pDamageInstance->m_p3DObject->GetMotion() != meshChildParticle.m_iMontionIndex )
				continue;            

            if( meshChildParticle.m_bDotParticle )
                continue;

			if(pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_pParticle == NULL || 
               g_ParticleManager->IsLiveInstance( pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_pParticle ) == false)
            {
                float fParticleOffsetX = 0.0f;

                if( pDamageInstance->m_IsRight )
                    fParticleOffsetX = meshChildParticle.m_fOffSetX;
                else
                    fParticleOffsetX = -meshChildParticle.m_fOffSetX;

			    if( meshChildParticle.m_bDieTogether )
			    {
                    if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                    {
                        pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_pParticle = pDamageInstance->CreateSequence(meshChildParticle.m_strParticle.c_str(),
                            pDamageInstance->m_X - 1.0f + fParticleOffsetX, pDamageInstance->m_Y - 0.45f + meshChildParticle.m_fOffSetY, 0.5f);
                        m_vecParticle.push_back(pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_pParticle);
                    }
			    }
			    else
			    {
                    if (!g_kGlobalValue.IsSkillEffectDisabled(pDamageInstance->m_Who))
                    {
                        pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_pParticle = pDamageInstance->CreateSequence(meshChildParticle.m_strParticle.c_str(),
                            pDamageInstance->m_X - 1.0f + fParticleOffsetX, pDamageInstance->m_Y - 0.45f + meshChildParticle.m_fOffSetY, 0.5f);
                    }
			    }
                
                if( meshChildParticle.m_iBoneIndex != -1 )
                {
                    TRACE_INFO stTrace;
                    stTrace.m_pvPos = &pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_vBonePos;
                    stTrace.m_fParticleTraceTime = pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_fTraceTime;
                    if( pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_pParticle )
                        pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshChildParticle[i].m_pParticle->SetTrace( &stTrace );
                }            
            }
		}		

		//Ư�� �����ӿ� ���带 ����Ѵ�.
		for( int i =0; i < (int)pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshSoundList.size(); ++i )
		{
			MESH_SOUNDLIST& meshSoundList = pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshSoundList[i];

			if( pDamageInstance->m_p3DObject->GetFrame() != meshSoundList.m_iStartFrame )
				continue;

			g_KDSound.Play( meshSoundList.m_strSound.c_str() );
		}

		//Ư�� �����ӿ� �������� �����Ѵ�.
		for( int i=0; i< (int)pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshCreateDamage.size(); ++i )
		{
			MESH_CREATEDAMAGE& meshCreateDamage = pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecMeshCreateDamage[i];

			if( pDamageInstance->m_p3DObject->GetFrame() != meshCreateDamage.m_iCreateFrame ||
				pDamageInstance->m_p3DObject->GetMotion() != meshCreateDamage.m_iMontionIndex )
				continue;						
			bool bOldRight;
			MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);

			if( pDamageInstance->m_IsMonsterDamage )
			{
				if(pMonster != NULL){
					bOldRight = pMonster->GetIsRight();
					pMonster->SetIsRight( pDamageInstance->m_IsRight );
				}
			}
			else
			{
				bOldRight = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight();
				g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetIsRight( pDamageInstance->m_IsRight );
			}
			
			float fOffsetX = meshCreateDamage.m_fOffSetX;
            if( meshCreateDamage.m_bRightLeftOffset && g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight() == false )
                fOffsetX = -meshCreateDamage.m_fOffSetX;

			CDamageInstance* pMeshDamage = g_MyD3D->m_pDamageManager->AddWithLocate( meshCreateDamage.m_iDamageIndex, pDamageInstance->m_Who , 
                pDamageInstance->m_X + fOffsetX, pDamageInstance->m_Y + meshCreateDamage.m_fOffSetY, false, -1, NULL, pDamageInstance->m_HitWho);

            if( pDamageInstance->m_IsMonsterDamage )
            {
                if(pMonster != NULL){
                    pMonster->SetIsRight( bOldRight );
                }
            }
            else
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetIsRight( bOldRight );
            }

            // �θ𵥹������� �޾ƿ;ߵǴ� ������ ���⿡�� �߰��ؿ�.
            if( NULL != pMeshDamage )
            {
                pMeshDamage->m_bPetDamage = pDamageInstance->m_bPetDamage;
            }

        }

    }

    if( pDamageInstance->m_pAnim != NULL )
    {        
        float fEx = pDamageInstance->m_IsRight == true ? 1.0f : -1.0f;
        pDamageInstance->m_pAnim->GetMatrix()->Rotate( 0.0f, 0.0f, m_fAnimAngle*fEx );

        float fX = pDamageInstance->m_X + m_fAnimStartPosX - 1.0f;
        float fY = pDamageInstance->m_Y + m_fAnimStartPosY - 0.5f;

        pDamageInstance->m_pAnim->GetMatrix()->Move( fX, fY, 0.7f );

        if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
            pDamageInstance->m_pAnim->SetShow( pDamageInstance->m_IsShow );
        else
            pDamageInstance->m_pAnim->SetShow( false );

        //������ ����
        if (m_fAnimScaleX_Speed != 1.0f || m_fAnimScaleY_Speed != 1.0f)
        {                
            pDamageInstance->m_fAnimScaleX *= m_fAnimScaleX_Speed;
            pDamageInstance->m_fAnimScaleY *= m_fAnimScaleY_Speed;
            
            if(abs(m_fAnim_Limit_ScaleX) < abs(pDamageInstance->m_fAnimScaleX) )
            {
                pDamageInstance->m_fAnimScaleX = m_fAnim_Limit_ScaleX;
            }
            else if(abs(m_fAnim_Start_ScaleX) > abs(pDamageInstance->m_fAnimScaleX) )
            {
                pDamageInstance->m_fAnimScaleX = m_fAnim_Start_ScaleX;
            }
            
            if(abs(m_fAnim_Limit_ScaleY) < abs(pDamageInstance->m_fAnimScaleY) )
            {
                pDamageInstance->m_fAnimScaleY = m_fAnim_Limit_ScaleY;
            }
            else if(abs(m_fAnim_Start_ScaleY) > abs(pDamageInstance->m_fAnimScaleY) )
            {
                pDamageInstance->m_fAnimScaleY = m_fAnim_Start_ScaleY;
            }            
        }
    
        if(pDamageInstance->m_IsRight)
            pDamageInstance->m_pAnim->GetMatrix()->Scale(  pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );
        else
            pDamageInstance->m_pAnim->GetMatrix()->Scale( -pDamageInstance->m_fAnimScaleX, pDamageInstance->m_fAnimScaleY, 1.0f );                
    }

    if( !pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecAnimInfo.empty() && !pDamageInstance->m_p3DObject == NULL )
    {
        std::vector< std::pair< int, int > >& vecAnim = pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_vecAnimInfo;
        std::vector< std::pair< int, int > >::iterator vit;

        for( vit = vecAnim.begin(); vit != vecAnim.end(); ++vit )
        {
            std::pair< int, int >& pairAnim = (*vit);
            if( pDamageInstance->m_Life == pairAnim.first || 
                ( pairAnim.first == 0 )  )
            {
                if( !pDamageInstance->m_p3DObject->GetIsLive() )
                {
					m_vPrevMeshMotionPos.x += pDamageInstance->m_p3DObject->GetMotionPos().x ;
                    m_vPrevMeshMotionPos.y = pDamageInstance->m_p3DObject->GetMotionPos().y ;

					if( !pDamageInstance->m_pOriginalDamage->m_MeshInfo.m_bAlwaysRight ) 
						pDamageInstance->m_p3DObject->IsRight( pDamageInstance->m_IsRight );
                    if( pDamageInstance->m_IsRight )
                        pDamageInstance->m_X += (pDamageInstance->m_p3DObject->GetMotionPos().x );
                    else
                        pDamageInstance->m_X -= (pDamageInstance->m_p3DObject->GetMotionPos().x );
                    pDamageInstance->m_Y += pDamageInstance->m_p3DObject->GetMotionPos().y;
                    pDamageInstance->m_p3DObject->SetIsLive( true );
                }

                if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                    pDamageInstance->m_p3DObject->SetRender( false );
                else
                    pDamageInstance->m_p3DObject->SetRender( true );

                pDamageInstance->m_p3DObject->SetMotionFromIndex( pairAnim.second );
                vecAnim.erase( vit );
                break;
            }
        }
    }

    float fX = pDamageInstance->m_X - 1.0f;
    float fY = pDamageInstance->m_Y - 0.5f;

    if( pDamageInstance->m_pParticle != NULL )
    {
        if( pDamageInstance->m_pOriginalDamage->m_bParticleTrace == false )
        {
            pDamageInstance->m_pParticle->SetPosition( D3DXVECTOR3( fX, fY, 0.5f ) );

            for ( size_t i = 0; i < pDamageInstance->m_vecChildParticle.size(); ++i ) 
            {
                pDamageInstance->m_vecChildParticle[i]->SetPosition( D3DXVECTOR3( fX, fY, 0.5f ) );
            }
        }

        if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
            pDamageInstance->m_pParticle->SetShow( false );
	}

	pDamageInstance->m_pOriginalDamage->m_vPos.x = fX;
	pDamageInstance->m_pOriginalDamage->m_vPos.y = fY;
	pDamageInstance->m_pOriginalDamage->m_vPos.z = 0.5f;

    std::vector< CHILD_PARTICLE >::iterator vit;
    for( vit = pDamageInstance->m_pOriginalDamage->m_vecChildParticle.begin();
        vit != pDamageInstance->m_pOriginalDamage->m_vecChildParticle.end(); ++vit )
    {        
        if( vit->m_bDotParticle == false )
        {
            if( vit->m_iStartLife == pDamageInstance->m_Life )
            {
                if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                {
                    CParticleEventSeqPTR pChild;
                    pChild = SetParticle( pDamageInstance, (*vit) );
                    // [10/22/2007] breadceo.dietogether == false �̸� �׳� �� �ڸ��� ����
                    if( pChild && vit->m_bDieTogether == true )
                    {
                        // [12/7/2007] breadceo.Crash �ǽɺκ�
                        DIRECTION_INFO Info;
                        pChild->GetDirectInfo( Info );
                        if( vit->m_bSyncCreatorDir == false ) {
                            Info.m_pbIsRight = NULL;
                        }

                        if ( g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                            pChild->SetShow( false );

                        pDamageInstance->m_vecChildParticle.push_back( pChild );
                    }
                }
            }
        }
        else
        {
            if( vit->m_bDieTogether )
                vit->m_bDieTogether = false; //Ȥ�ó� ���� ���̵��� �����ؼ� ũ���� �� �� �־ false�� ������. �ٸ� ��ƼŬ�� ��� ���� �ִ´�.

            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                SetParticle( pDamageInstance, (*vit) );
        }
    }

    if( pDamageInstance->m_p3DObject != NULL )
    {
        // [12/6/2007] breadceo.��ġ ������Ʈ
        pDamageInstance->m_p3DObject->SetPos( D3DXVECTOR2( pDamageInstance->m_pOriginalDamage->m_vPos.x, 
            pDamageInstance->m_pOriginalDamage->m_vPos.y ) );
        m_vPrevMeshMotionPos = pDamageInstance->m_p3DObject->GetMotionPos();
    }
	
    if( pDamageInstance->m_pOriginalDamage->m_bUserMotionDependant )
    {
        if( pDamageInstance->m_IsMonsterDamage )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);
			if(pMonster != NULL){
				if( pMonster->m_bMotionChanged )
					pDamageInstance->m_Life = 1;
			}
        }
        else
        {
            if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_bMotionChanged )
                pDamageInstance->m_Life = 1;
        }
    }

    if( pDamageInstance->m_pOriginalDamage->m_bOwnerDirection )
    {
        bool oldbRight = pDamageInstance->m_IsRight;
        if ( pDamageInstance->m_IsMonsterDamage )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
			if(pMonster != NULL){
	            pDamageInstance->m_IsRight = pMonster->GetIsRight();
			}
        }
        else
        {
            pDamageInstance->m_IsRight = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetIsRight();
        }
        if( oldbRight != pDamageInstance->m_IsRight )
            pDamageInstance->m_Angle = -pDamageInstance->m_Angle;
    }

    CheckTrigger( pDamageInstance );

    if( pDamageInstance->m_pOriginalDamage->m_bParticleSlow ) {
        if( pDamageInstance->m_pParticle != NULL && pDamageInstance->m_IsMonsterDamage == false )
        {
            if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.bLive == true ) {
                if( pDamageInstance->m_ExtraMonsterNum == g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame ) {
                    pDamageInstance->m_Life = pDamageInstance->m_Life++;
                    if( pDamageInstance->m_pParticle != NULL )
                        pDamageInstance->m_pParticle->SlowCount( true );
                } else {
                    pDamageInstance->m_pParticle->SlowCount( false );
                }
                // ������ ���� ������
                pDamageInstance->m_ExtraMonsterNum = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame;
            } else {
                pDamageInstance->m_pParticle->SlowCount( false );
            }
        }
    }
	
    if( pDamageInstance->m_pOriginalDamage->m_bOwnerKillDamage )
    {
        if( pDamageInstance->m_IsMonsterDamage ) 
        {
            if ( g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) ) 
            {
                MONSTER* pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
                
                if ( pMonster->m_bDie || pMonster->m_fHP <= 0.0f )
                {
                    pDamageInstance->m_Life = 1;
                }
            }
        }
        else
        {
            if ( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) ) 
            {
                PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];

                if ( pPlayer->IsDead() )
                {
                    pDamageInstance->m_Life = 1;

                }
            }
        }
    }
    
}

void CDamage::Render( CDamageInstance* pDamageInstance )
{
}

bool CDamage::CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate )
{
    if( pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_CURE_FOR_MONSTER) ) {
        return false;
    }

    // ������ ���� ��Ʈ �� üũ
    if( pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.iHitCnt > 0 ) {
        if( pDamageInstance->m_mapHitCount.find( hitWho ) != pDamageInstance->m_mapHitCount.end() ) {
            if( pDamageInstance->m_mapHitCount[hitWho] < pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.iHitCnt ) {
                ++pDamageInstance->m_mapHitCount[hitWho];
                // ��Ÿ
                if( pDamageInstance->m_mapHitCount[hitWho] == pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.iHitCnt ) {
                    if( pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.iChangeMotion != -1 ) {
                        pDamageInstance->m_pOriginalDamage->m_iDamageMotion = pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.iChangeMotion;
                    }
                }
            } else {
                return false;
            }        
        } else {
            pDamageInstance->m_mapHitCount[hitWho] = 1;
        }
    }

    //ONEHIT��������� �ѹ��� �°� �Ѵ�.
	if ( pDamageInstance->InsertAlreadyDamaged(hitWho) == false )
		return false;

    // ���� ������ �ϰ�� �浶�� ����� �����ϵ���
    if( pDamageInstance->m_pOriginalDamage->m_bGasDamage &&
        ( g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[hitWho]->m_kUserInfo.GetCurrentChar(), GASMASK_ITEM_ID, true ) ||
          g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[hitWho]->m_kUserInfo.GetCurrentChar(), GASMASK_ITEM_ID_SEASON4, true )) )
    {
        return false;
    }

    // ���� �������� ó��
    if( false == g_MyD3D->MyPlayer[hitWho]->GetPermissionDamage().empty() )
    {
        std::vector<int> vecPermissionDamage = g_MyD3D->MyPlayer[hitWho]->GetPermissionDamage();
        std::vector<int>::iterator vecPermissionDamageIter = vecPermissionDamage.begin();
        for(; vecPermissionDamageIter != vecPermissionDamage.end(); ++vecPermissionDamageIter)
        {
            if( pDamageInstance->m_What == (*vecPermissionDamageIter) )
                break;
        }
        if( vecPermissionDamageIter == vecPermissionDamage.end() )
        {
            DWORD *pdwMissAttackTime = NULL;
            if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_ExtraMonsterNum) )
            {
                MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                if( pMonster )  pdwMissAttackTime = &pMonster->m_dwMissAttackTime;
            }
            else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
            {
                pdwMissAttackTime = &g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_dwMissAttackTime;
            }

            if( (NULL != pdwMissAttackTime) && (timeGetTime() - (*pdwMissAttackTime)) > MISS_ATTACK_TERM_TIME )
            {
                (*pdwMissAttackTime) = timeGetTime();
                D3DXVECTOR2 hitPos = D3DXVECTOR2( g_MyD3D->MyPlayer[hitWho]->vPos.x, g_MyD3D->MyPlayer[hitWho]->vPos.y + 0.05f );
                g_MyD3D->MyPlayer[hitWho]->ShowMissAttack( hitPos.x, hitPos.y );
            }
            return false;
        }
    }

	//====================================================================================
    // 2009.02.26 : Jemitgge
    // Comment : DA_TRACE_HITTED �� ���� ���̵���~ ���� ���� ������� �������̴� ����
	//			 �� ����� �������� ������� �� ���̵鸸 �°� �ϰ��ʹ�!!
	if ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_TRACE_HITTED && pDamageInstance->m_HitWho != hitWho )
	{
		return false;
	}

    if( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE || pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_CURE_PLUS )
    {
        // ��� ���ÿ� ī���� �ô� �� ���� ����.
        if( g_MyD3D->MyPlayer[hitWho]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[hitWho]->uiMotion== g_MyD3D->MyPlayer[hitWho]->GetOwnMotion(MID_COMMON_FRONTDOWN_AND_STANDUP) )
            return false;

        // ��� �ÿ��� �� ���� ����.
        if( g_MyD3D->MyPlayer[hitWho]->GetHP() <= 0.0f && g_MyD3D->MyPlayer[hitWho]->uiMotion== g_MyD3D->MyPlayer[hitWho]->GetOwnMotion(MID_COMMON_DOWN_AND_STANDUP) )
            return false;
    }
    else
    {
	    if ( !g_MyD3D->MyPlayer[hitWho]->OnDamagePreProcess( pDamageInstance->m_IsMonsterDamage ? pDamageInstance->m_ExtraMonsterNum : pDamageInstance->m_Who, false, pDamageInstance->m_IsMonsterDamage, 
            pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_What,  hitWho) )
		    return false;
    }


	// �Ф�
	if ( g_MyD3D->MyPlayer[hitWho]->Slow_Count > 0 && (pDamageInstance->m_What == DT_RONAN_MAGIC_SPECIAL2_FIRE || pDamageInstance->m_What == DT_LEY1_FLAME_ROLL_FIRE) )
		return false;

    //////////////////////////////////////////////////////////////////////////
    // ����2�� ����ŷ!! ������ �ڽ��� ����!
    //////////////////////////////////////////////////////////////////////////
    if( pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer() && hitWho == g_MyD3D->Get_MyPlayer() && 
        ( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_RONAN3 || g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurrentChar().iCharType == GC_CHAR_LIME ||
        ( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_SIEG && g_MyD3D->MyPlayer[hitWho]->CheckSkill(SID_SIEG1_FURY_TYPE_B_LV3))) && 
        g_MyD3D->MyPlayer[hitWho]->MetamorphosisForm == FORM_NORMAL /*&& g_MyD3D->MyPlayer[hitWho]->IsDamagedMotion() == false*/ )
    {
        int i = g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME;

        if( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_RONAN3 && pDamageInstance->m_What != DT_INFINITY_DUNGEON_HEAL_AREA ) {
            if ( pDamageInstance->m_pOriginalDamage->m_bSpecialAttack == false )
            {
                // ����2�� ����ŷ!!
                if ( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight == false )
                {
                    for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                    {
                        if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_RIGHT][i] ) break;
                    }
                }
                else
                {
                    for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                    {
                        if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_LEFT][i] ) break;
                    }
                }
            }
        } else {
            if( (( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_PHYSICAL ) ||
                ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_PHYSICAL_REACTION ) ||
                ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_SLOW_MOTION) ||
                ( pDamageInstance->m_pOriginalDamage->m_dwDMAttribute & DA_MISSILE ) ||
                ( pDamageInstance->m_What == DT_ARME3_ICEBALL ) ||
                ( pDamageInstance->m_What == DT_ARME4_FIREBALL )) &&
                ( pDamageInstance->m_pOriginalDamage->m_bSpecialAttack == false ) )
            {
                if ( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight == false )
                {
                    for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                    {
                        if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_RIGHT][i] ) break;
                    }
                }
                else
                {
                    for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                    {
                        if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_LEFT][i] ) break;
                    }
                }
            }
            else if ( pDamageInstance->m_What == DT_FIREBALL &&
                pDamageInstance->m_pOriginalDamage->m_bSpecialAttack == false )
            {
                // ����2�� ����ŷ!!
                if ( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight == false )
                {
                    for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                    {
                        if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_RIGHT][i] ) break;
                    }
                }
                else
                {
                    for ( i = 0; i < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++i )
                    {
                        if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_LEFT][i] ) break;
                    }
                }
            }
        }        

        if ( i != Controls::BLOCK_ENABLE_FRAME )
        {
            PLAYER* pMyPlayer = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()];
            if( pMyPlayer ) {
                if( g_MyD3D->MyPlayer[hitWho]->GetCurrentChar().iCharType == GC_CHAR_SIEG ) {
                    g_MyD3D->MyPlayer[hitWho]->SendBurnningPoint(hitWho);
                    if( g_MyD3D->MyPlayer[hitWho]->GetBurnningPoint() >= 0.5f ) {
                        if( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_SIEG4 ) {
                            pMyPlayer->Direct_Motion_Input( MID_SIEG4_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                        } else if( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_SIEG3 ) {
                            pMyPlayer->Direct_Motion_Input( MID_SIEG3_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                        } else if( g_MyD3D->MyPlayer[hitWho]->Extra_Char_Num == CID_SIEG2 ) {
                            pMyPlayer->Direct_Motion_Input( MID_SIEG2_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                        } else {
                            pMyPlayer->Direct_Motion_Input( MID_SIEG1_SKILL_GUARD, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
                        }
                    }
                } else if( pMyPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME ) {
                    if( pMyPlayer->CheckSkill( SID_LIME2_SELF_AURA_DEFFENSE_PLUS_LV3 ) && pMyPlayer->IsMagicEffect( EGC_EFFECT_LIME2_SELF_AURA_DEFFENCE ) ) {
                        if( pMyPlayer->GetMP() >= 0.3f ) {
                            pMyPlayer->Direct_Motion_Input( MID_LIME2_COUNTER_ATK );
                        }
                    }
                } 
				else
				{
					if (pMyPlayer->IsMagicEffect(EGC_EFFECT_RONAN3_BLOCK_COOL) == false)
					{
						g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Direct_Motion_Input(MID_RONAN3_BLOCKING, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
					}
				}
                pDamageInstance->m_Life = 0;
                return false;
            }            
        }                
    }

    if( pDamageInstance->m_Who != g_MyD3D->Get_MyPlayer() && hitWho == g_MyD3D->Get_MyPlayer() ) {
        int iEnemyAtkGuardMotion = g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetEnemyAtkGuardMotion( g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->GetCurrentMotion() );
        if( iEnemyAtkGuardMotion != -1 &&
            !( m_dwDMAttribute & DA_MISSILE ) &&
            !( m_dwDMAttribute & DA_CURE ) &&
            !( m_dwDMAttribute & DA_CURE_PLUS ) &&
            pDamageInstance->m_pOriginalDamage->m_bSpecialAttack == false ) {
                g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Direct_Motion_Input( iEnemyAtkGuardMotion );
                pDamageInstance->m_Life = 0;
                return false;
        }        
    }

    //////////////////////////////////////////////////////////////////////////

    // ���̾� ���� �̵��� ����

    if( pDamageInstance->m_IsMonsterDamage )
    {
		MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
		if(pMonster != NULL){
			GetDamageRate( fDamageRate, pMonster, g_MyD3D->MyPlayer[hitWho], pDamageInstance );
		}

    }

    bool bDefence = false;

    if ( m_dwDMAttribute & DA_DEFENCEABLE )
    {
        if( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ENABLE_ARROWDEF &&
            ( g_MyD3D->MyCtrl->k_Right || g_MyD3D->MyCtrl->k_Left || !g_MyD3D->MyPlayer[hitWho]->IsContact ) &&
            ( GC_GM_TUTORIAL != SiKGCRoomManager()->GetGameMode() ) ) // ���Ҵ�.
        {
            fDamageRate *= 0.2f;
            bDefence = true;
        }
    }

    // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
    g_MyD3D->MyPlayer[hitWho]->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    g_MyD3D->MyPlayer[hitWho]->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;

    // �������� Crash�� ������ ������ �����ڸ� �̵���Ų��.
    if( m_kMoveCreateToCrashPos.m_bIsMoving && false == pDamageInstance->m_bMoveCrashPos )
    {
        pDamageInstance->m_bMoveCrashPos = true;

        if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) )
        {
            float fX = pDamageInstance->m_X + m_kMoveCreateToCrashPos.m_fOffsetX;
            float fY = pDamageInstance->m_Y + m_kMoveCreateToCrashPos.m_fOffsetY;
			MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
			if(pMonster != NULL){
				pMonster->SetX( fX );
				pMonster->SetY( fY );
			}
        }
        else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
        {
            float fX = pDamageInstance->m_X + m_kMoveCreateToCrashPos.m_fOffsetX;
            float fY = pDamageInstance->m_Y + m_kMoveCreateToCrashPos.m_fOffsetY;
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDirectMove( fX, fY );
        }

        if( -1 != m_kMoveCreateToCrashPos.m_iDamageLife )
        {
            pDamageInstance->m_Life = m_kMoveCreateToCrashPos.m_iDamageLife;
        }
    }

    // �ǰ��� ��ġ�� ������ �����ڸ� �̵���Ų��.
    if( m_kMoveCreateToUnitPos.m_bIsMoving && false == pDamageInstance->m_bMoveUnitPos )
    {
        pDamageInstance->m_bMoveUnitPos = true;

        float fX = pDamageInstance->m_X + m_kMoveCreateToUnitPos.m_fOffsetX;
        float fY = pDamageInstance->m_Y + m_kMoveCreateToUnitPos.m_fOffsetY;
        if( OBJ_TYPE_PLAYER == pDamageInstance->m_emHitWhoType && g_MyD3D->IsPlayerIndex( hitWho ) )
        {
            PLAYER *pHitPlayer = g_MyD3D->MyPlayer[hitWho];
            if( pHitPlayer != NULL )
            {
                D3DXVECTOR3 vPos = pHitPlayer->GetPosition();
                fX = vPos.x + m_kMoveCreateToUnitPos.m_fOffsetX;
                fY = vPos.y + m_kMoveCreateToUnitPos.m_fOffsetY;
            }
        }

        if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) )
        {
            MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
            if(pMonster != NULL)
            {
                pMonster->SetX( fX );
                pMonster->SetY( fY );
            }
        }
        else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
        {
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x = fX;
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y = fY;
        }

        if( -1 != m_kMoveCreateToUnitPos.m_iDamageLife )
        {
            pDamageInstance->m_Life = m_kMoveCreateToUnitPos.m_iDamageLife;
        }
    }

    // ���� �������� �������� LastKillMePlayer�� �������ֵ��� ����!!
    //if( pDamageInstance->m_pOriginalDamage->m_IsEnableDamage != 0 )
    if( pDamageInstance->m_pOriginalDamage->m_fDamageToPlayer != 0 )
    {
        int iWhoIndex = pDamageInstance->m_Who;

        // Attacker����( ��ȯ�� ���Ͱ� Attacker��� �÷��̾� Index�� ����������. )
        if( g_kMonsterManager.IsSummonMonsterByPlayer(pDamageInstance->m_ExtraMonsterNum) )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
			if(pMonster){
				PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
				if( pPlayer )
					iWhoIndex = pPlayer->m_iPlayerIndex;
			}
        }

        g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer(static_cast<unsigned char>(iWhoIndex) );
    }
	else
	{
		g_MyD3D->MyPlayer[hitWho]->SetLastKillmePlayer(MAX_PLAYER_NUM);
	}

    if( !g_MyD3D->MyPlayer[hitWho]->IsInvinsible( pDamageInstance->m_pOriginalDamage->m_bSpecialAttack ) || pDamageInstance->m_pOriginalDamage->m_bIgnoreSuperDamage ) 
	{
        float fDamageToPlayer = m_fDamageToPlayer;
        if( ( pDamageInstance->m_pOriginalDamage->m_bSpecialAttack || pDamageInstance->m_pOriginalDamage->m_bSlotUniqueSkillDmg ) && pDamageInstance->m_pOriginalDamage->m_bSpecial4Damage == false ) 
		{
                fDamageToPlayer = SiGCGrowingSkillManager()->CountChangeDamageRate( g_MyD3D->MyPlayer[pDamageInstance->m_Who], m_fDamageToPlayer );
        }

        float fDamage = fDamageToPlayer * fDamageRate * m_kDivisionDamage.m_fRangeDamageRatio;

        if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] > 0.f && ( m_dwDMAttribute & DA_MISSILE ) ) 
		{
            fDamage -= ( fDamage * g_MyD3D->MyPlayer[hitWho]->m_fAbility[ ABILITY_MISSILE_DAMAGE_RESIST ] );
        }

        PLAYER::DamageInfo damage;
        damage.SetIgnoreDefence( pDamageInstance->m_pOriginalDamage->m_bIgnoreDefDamage );
        damage.SetDamage( fDamage );
        damage.SetDamageInstance( pDamageInstance );
        damage.SetSpecialAttack( pDamageInstance->m_pOriginalDamage->m_bSpecialAttack, pDamageInstance->m_pOriginalDamage->m_nSpecialGrade );
        damage.SetShieldCountEnable( true );
        damage.SetReflectionEnable( true );
        damage.SetNotComboBreak( pDamageInstance->m_pOriginalDamage->m_bNotComboBreak );

        if( pDamageInstance->m_pOriginalDamage->IsExistDamageFlag( DF_BUFF_CREATE_DAMAGE ) ) {
            damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_PET, pDamageInstance->m_Who );
        }

        float fDiff = g_MyD3D->MyPlayer[hitWho]->Change_HP( damage );

        // �� ������ �翡 ����ؼ� �������� HP�� �����Ѵ�.
        if( 0.0f != pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage )
        {
            if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) )
            {
                MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                if(pMonster != NULL)
                {
                    float fDamage = (fDiff * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage);
                    if( g_kGlobalValue.m_kUserInfo.bHost )
                        pMonster->SetHP( pMonster->GetHP() + fDamage );
                    else
                        pMonster->SendToHostOfMonsterHP( pMonster->GetHP() + fDamage );
                    if( 0.0f < fDamage )
                        pMonster->RenderHitNumber( fDamage, true, pMonster->GetX(), pMonster->GetY(), false );
                }
            }
            else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetHP( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() + (fDiff * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage) );
            }
        }
    }

	g_MyD3D->MyPlayer[hitWho]->DecreaseMP( m_fMpChange );

    float m_fCorruptedFinalDamage = m_fDamageToPlayer * fDamageRate * 100.0f;

    if ( m_bIsCorruptedDamage )
    {
        float m_iFinalManip = g_MyD3D->m_TempPlayer.GetCorruptedBuffNumber();
        m_fCorruptedFinalDamage *= ( m_iFinalManip * 0.01f );
    }

    if( 0.0f != m_fMpChange )
    {
        MPDamageParticle( pDamageInstance, D3DXVECTOR2(g_MyD3D->MyPlayer[hitWho]->vPos.x, g_MyD3D->MyPlayer[hitWho]->vPos.y) );
    }
	
    if( pDamageInstance->m_IsMonsterDamage )
    {
        g_MyD3D->MyPlayer[hitWho]->ShieldCountAttack( hitWho, pDamageInstance->m_ExtraMonsterNum, m_fCorruptedFinalDamage, false, true );
    }
    else
    {
        g_MyD3D->MyPlayer[hitWho]->ShieldCountAttack( hitWho, pDamageInstance->m_Who, m_fDamageToPlayer * fDamageRate, pDamageInstance->m_bPetDamage );
    }

    if( pDamageInstance->m_IsMonsterDamage )
        g_MyD3D->MyPlayer[hitWho]->ReflectionDamage( hitWho, pDamageInstance->m_ExtraMonsterNum, m_fCorruptedFinalDamage );
    else {
#if defined( LEVEL_DESIGN_STAT_CALC )
        g_MyD3D->MyPlayer[hitWho]->ReflectionDamage(hitWho, pDamageInstance->m_Who, m_fDamageToPlayer* fDamageRate * 0.03f, pDamageInstance->m_bPetDamage);
#else
        g_MyD3D->MyPlayer[hitWho]->ReflectionDamage(hitWho, pDamageInstance->m_Who, m_fDamageToPlayer* fDamageRate * 3.f, pDamageInstance->m_bPetDamage);
#endif
    }

    // ���� ���� �ɼ��� �ִٸ�
    if( pDamageInstance->m_pOriginalDamage->m_bShieldBreak )
        g_MyD3D->MyPlayer[hitWho]->Shield = 0;

    if( ( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() || GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() ) && !pDamageInstance->m_pOriginalDamage->m_bIgnoreSuperDamage )
    {
        if( g_MyD3D->MyPlayer[hitWho]->Shield == 0 )
        {
            // ž�¹��� ž���ϰ� ������ ������ȯ�ϸ� �ȵȴ�.
            if( UINT_MAX <= g_MyD3D->MyPlayer[hitWho]->GetEmbarkUID() )
            {
                if( !g_MyD3D->MyPlayer[hitWho]->m_bSuperArmor && !g_MyD3D->MyPlayer[hitWho]->m_bHyperArmor )
                {
                    if (m_dwDMAttribute & DA_EQUAL_DIRECTION)
                    {
                        g_MyD3D->MyPlayer[hitWho]->bIsRight = pDamageInstance->m_IsRight;
                    }

                    if (m_dwDMAttribute & DA_REVERSE_DIRECTION)
                    {
                        g_MyD3D->MyPlayer[hitWho]->bIsRight = !pDamageInstance->m_IsRight;
                    }

                    if ( m_dwDMAttribute & DA_DAMAGE_DIRECTION )
                    {
                        g_MyD3D->MyPlayer[hitWho]->bIsRight = g_MyD3D->MyPlayer[hitWho]->GetPosition().x < pDamageInstance->m_X;
                    }
                }
            }

            if ( ( g_MyD3D->MyPlayer[hitWho]->m_bHyperArmor ) && 
               (g_MyD3D->MyPlayer[hitWho]->GetCurrentMotion() == MID_COMMON_DOWN_AND_STANDUP  ||
                g_MyD3D->MyPlayer[hitWho]->GetCurrentMotion() == MID_COMMON_DAMAGED_BY_FLYATK ||
                g_MyD3D->MyPlayer[hitWho]->GetCurrentMotion() == MID_COMMON_DOWN_IN_SKY ) )
            {
                g_MyD3D->MyPlayer[hitWho]->Change_Motion(g_MyD3D->MyPlayer[hitWho]->GetWaitMotion());
            }

            if( g_MyD3D->MyPlayer[hitWho]->m_bSuperArmor && 
                ( pDamageInstance->m_pOriginalDamage->m_iDamageMotion == MID_COMMON_DOWN_AND_STANDUP ||
                pDamageInstance->m_pOriginalDamage->m_iDamageMotion == MID_COMMON_DAMAGED_BY_FLYATK ||
                pDamageInstance->m_pOriginalDamage->m_iDamageMotion == MID_COMMON_DOWN_IN_SKY ) ) {
                if (m_dwDMAttribute & DA_EQUAL_DIRECTION)
                {
                    g_MyD3D->MyPlayer[hitWho]->bIsRight = pDamageInstance->m_IsRight;
                }

                if (m_dwDMAttribute & DA_REVERSE_DIRECTION)
                {
                    g_MyD3D->MyPlayer[hitWho]->bIsRight = !pDamageInstance->m_IsRight;
                }

                if (m_dwDMAttribute & DA_DAMAGE_DIRECTION)
                {
                    g_MyD3D->MyPlayer[hitWho]->bIsRight = g_MyD3D->MyPlayer[hitWho]->GetPosition().x < pDamageInstance->m_X;
                }
            }

			if( g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_HOLD ) 
				&& g_MyD3D->MyPlayer[hitWho]->uiMotion != g_MyD3D->MyPlayer[hitWho]->GetOwnMotion( MID_COMMON_HOLD_NOEFFECT )  )
			{
				//����ߴ���. �˻�� �տ��� �̹� �ߴ�.
				if ( bDefence )
				{
					if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() )
					{
						g_MyD3D->MyCtrl->k_Right = false;
						g_MyD3D->MyCtrl->k_Left = false;

						if( g_MyD3D->MyPlayer[hitWho]->IsContact )
						{
							static bool bSwitch = false;
							if( bSwitch )
								g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_GROUND1, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
							else 
								g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_GROUND2, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );                    
							bSwitch = !bSwitch;
						}
						else 
						{                
							// ȭ�� �ݻ�� �ȵ�
							g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input( g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().ARROWDEF_SKY, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack );
						}
					}
				}
				//��� �ٲ��ֱ�. ��� �ߴٸ� ���������� ������ �ǰ� ������� ��ȯ ���� �ʴ´�.
				else if( m_iDamageMotion > -1 && 
                         m_iDamageMotion < g_pGraphicsHelper->GetPlayerMotionNum() && 
                         pDamageInstance->m_What != DT_ARME_SKILL_REVERSEGRAVITY_LV2_LEFT &&
						 pDamageInstance->m_What != DT_ARME_SKILL_REVERSEGRAVITY_LV2_RIGHT )
				{
                    // �����÷��̾�� ����� �������� ���Ŷ�!!
                    if( false == (g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() && m_bNoLocalMotion) )
                    {
					    g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(m_iDamageMotion, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
                    }

					if( m_dwDMAttribute & DA_PHYSICAL_REACTION )
					{
						if( g_MyD3D->MyPlayer[hitWho]->IsContact == false )
							g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
					}
				}

				//�ϴÿ� �� ���� �� ������ �������� �ڵ�
				if ((m_dwDMAttribute & DA_FELL_DOWN_AT_SKY) && !g_MyD3D->MyPlayer[hitWho]->IsContact && !bDefence)
				{
					g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
				} 

				if( m_dwDMAttribute & DA_PHYSICAL_REACTION )
				{
					float fIsRight;
					if( g_MyD3D->MyPlayer[hitWho]->bIsRight )
						fIsRight = -1.0f;
					else
						fIsRight = 1.0f;

					if( m_dwDMAttribute & DA_REVERSE_DIRECTION )
						g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.02f * fIsRight;
					else if( m_dwDMAttribute & DA_EQUAL_DIRECTION )
						g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.02f * fIsRight;
				}

				if ( m_dwDMAttribute & DA_PUSH )
                {
                    if( m_dwDMAttribute & DA_POSITION_CONTROL )
                    {						
                        if (pDamageInstance->m_IsRight)
                            g_MyD3D->MyPlayer[hitWho]->vPos.x += 0.01f;
                        else
                            g_MyD3D->MyPlayer[hitWho]->vPos.x -= 0.01f;
                    }
                    else
                    {
                        if (pDamageInstance->m_IsRight)
                            g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.01f;
                        else
                            g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.01f;
                    }
				}

				if ( m_dwDMAttribute & DA_PULL )
                {
                    if( m_dwDMAttribute & DA_POSITION_CONTROL )
                    {						
                        if (pDamageInstance->m_IsRight)
                            g_MyD3D->MyPlayer[hitWho]->vPos.x -= 0.01f;
                        else
                            g_MyD3D->MyPlayer[hitWho]->vPos.x += 0.01f;
                    }
                    else
                    {
                        if (pDamageInstance->m_IsRight)
                            g_MyD3D->MyPlayer[hitWho]->x_Speed = -0.01f;
                        else
                            g_MyD3D->MyPlayer[hitWho]->x_Speed = 0.01f;
                    }
				}

				if ( m_dwDMAttribute & DA_CANCEL_JUMP_STATE )
				{
					if( g_MyD3D->MyPlayer[hitWho]->y_Speed > 0 )        //���� ���� ����
						g_MyD3D->MyPlayer[hitWho]->y_Speed = 0;    
				}				

				//���� �� �ӵ� ��ȭ
                float fXSpeed = m_fDMXSpeed, fYSpeed = m_fDMYSpeed;
                if( pDamageInstance->m_pOriginalDamage->m_bNoPushSpeedToFatal )
                {
                    if( g_MyD3D->MyPlayer[hitWho]->IsFatal() )
                    {
                        fXSpeed = 0.0f;
                        fYSpeed = 0.0f;
                    }
                }

                // ž�¹��� ž���ϰ� ������ PushSpeed�� �������� �ȴ�.
                if( UINT_MAX > g_MyD3D->MyPlayer[hitWho]->GetEmbarkUID() )
                {
                    fXSpeed = 0.0f;
                    fYSpeed = 0.0f;
                }

				if (fXSpeed != -1.0f)
				{
					if ( m_bApplyDamageSpeedByDamageDirection )
					{
						fXSpeed = pDamageInstance->m_IsRight? fXSpeed : -fXSpeed;
					}
					else
					{
						fXSpeed = g_MyD3D->MyPlayer[hitWho]->bIsRight? fXSpeed : -fXSpeed;						
					}

					if( m_dwDMAttribute & DA_POSITION_CONTROL )
					{						
						g_MyD3D->MyPlayer[hitWho]->vPos.x += fXSpeed;
					}
					else
					{                        
					    g_MyD3D->MyPlayer[hitWho]->x_Speed = fXSpeed;                    
					}
				}
				if(fYSpeed != -1.0f)
				{                        
					g_MyD3D->MyPlayer[hitWho]->y_Speed = fYSpeed;
				}

                if( 0 < pDamageInstance->m_pOriginalDamage->m_iStopUnitFrame )
                {
                    g_MyD3D->MyPlayer[hitWho]->SetStopSpeedFrame( pDamageInstance->m_pOriginalDamage->m_iStopUnitFrame );
                }

                if( pDamageInstance->m_pOriginalDamage->m_kSyncDamagePosAtCrashPos.m_bIsSync ) {                    
                    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
                        g_MyD3D->MyPlayer[hitWho]->y_Speed = DEAULT_FALL_SPEED;
                    else
                        g_MyD3D->MyPlayer[hitWho]->m_fFall_Speed = 0.0f;
                }

				//���� ����� frame�� 0���� �ʱ�ȭ.
				if(m_dwDMAttribute & DA_INIT_FRAME)
				{
					if( g_MyD3D->MyPlayer[hitWho]->m_bSuperArmor ||
						g_MyD3D->MyPlayer[hitWho]->m_bHyperArmor )
					{

					}
					else
					{
						g_MyD3D->MyPlayer[hitWho]->cFrame = 0;
					}
					
				}
			}

			//fatal�� ��ġ�� �ʰ� �ٷ� ���̴� �ڵ�
			if ((m_dwDMAttribute & DA_KILLING_DAMAGE) && g_MyD3D->MyPlayer[hitWho]->GetHP() == 0)
			{
				g_MyD3D->MyPlayer[hitWho]->Direct_Motion_Input(MID_COMMON_DOWN_AND_STANDUP, pDamageInstance->m_pOriginalDamage->m_bSpecialAttack);
			}

			if( m_iCannotCatchFrame != 0 )
			{
				g_MyD3D->MyPlayer[hitWho]->SetNoCatchFrame( m_iCannotCatchFrame );
			}

            //ī�޶� �����ֱ�
            if( m_iEarthQuakeRunFrame == -1 || m_iEarthQuakeRunFrame < pDamageInstance->m_Life ) {
                if( m_iEarthQuake != 0 )
                    g_kCamera.SetEarthQuake( m_iEarthQuake );

                if( m_iEarthQuakeY != 0 )
                    g_kCamera.SetEarthQuakeY( m_iEarthQuakeY );
            }


            //��Ż ���ο� �ϵ� �ڵ�
            if(pDamageInstance->m_What == DT_MOTAL_BLOW)
            {
                g_MyD3D->MyPlayer[hitWho]->SetHP( 0.f );
            }

            // FatalRatioȮ���� ����Ż �����
            if( 0 < pDamageInstance->m_pOriginalDamage->m_iFatalRatio )
            {
                srand( time(NULL) );
                if( (rand()%101) <= pDamageInstance->m_pOriginalDamage->m_iFatalRatio )
                    g_MyD3D->MyPlayer[hitWho]->SetHP( 0.f );
            }

            //�������� ������
            if( 0 < pDamageInstance->m_pOriginalDamage->m_iHaste )
            {
                if( g_MyD3D->MyPlayer[hitWho]->m_fAbility[ABILITY_IGNORE_HASTE] <= 0.0f )
                    g_MyD3D->MyPlayer[hitWho]->Item_Remain_Time[GC_GAME_ITEM_HASTE] = pDamageInstance->m_pOriginalDamage->m_iHaste;
            }

            //�������� ������(��ȭ����)
            if( 0 < pDamageInstance->m_pOriginalDamage->m_pairSkipInfo.first && 0 < pDamageInstance->m_pOriginalDamage->m_pairSkipInfo.second )
            {
                g_MyD3D->MyPlayer[hitWho]->SetSkipFrame( pDamageInstance->m_pOriginalDamage->m_pairSkipInfo.second, pDamageInstance->m_pOriginalDamage->m_pairSkipInfo.first );
            }
        }

        // ���� ���� ��ε�� ����ó���� �Ѵ�.
        if( g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() ||
            GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
        {
            // ������ �����Ѵ�.
            if( m_iUnMagicEffect > -1 )
            {
                if( g_MyD3D->MyPlayer[hitWho]->IsMagicEffect( m_iUnMagicEffect ) )
                    g_MyD3D->MyPlayer[hitWho]->ClearMagicEffect( m_iUnMagicEffect );
            }

            // �Ϲ����� ������ �Ǵ�.
            if( (m_iMagicEffect > -1) )
            {
                if( false == (m_bUniqueMgicEff && g_MyD3D->MyPlayer[ hitWho ]->IsMagicEffect( m_iMagicEffect )) )
                {
                    DWORD dwAttackUnit = 0;
                    if( OBJ_TYPE_PLAYER == pDamageInstance->m_emWhoType && 
                        true == g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) && 
                        false == g_MyD3D->IsSameTeam( pDamageInstance->m_Who, hitWho ) )
                    {
                        dwAttackUnit = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->m_kUserInfo.dwUID;
                    }

                    g_LatencyTransfer.SendMagicEffectSynchPacket( m_iMagicEffect, m_fMagicEffTime , true, hitWho, m_iMagicLevel, dwAttackUnit );
                }
            }

            // APĳ���Ϳ��Ը� �ɾ�����ϴ� ������ �Ǵ�.
            if( m_iAPMagicEffect > -1 && g_MyD3D->MyPlayer[hitWho]->Is_SP_Character() )
            {
                if( false == (m_bUniqueMgicEff && g_MyD3D->MyPlayer[ hitWho ]->IsMagicEffect( m_iAPMagicEffect )) )
                {
                    DWORD dwAttackUnit = 0;
                    if( OBJ_TYPE_PLAYER == pDamageInstance->m_emWhoType && 
                        true == g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) && 
                        false == g_MyD3D->IsSameTeam( pDamageInstance->m_Who, hitWho ) )
                    {
                        dwAttackUnit = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->m_kUserInfo.dwUID;
                    }

                    g_LatencyTransfer.SendMagicEffectSynchPacket( m_iAPMagicEffect, m_fMagicEffTime, true, hitWho, m_iMagicLevel, dwAttackUnit );
                }
            }

            // ���� ������ �Ǵ�.
            if( 0 < int(m_vecRandMagicEffect.size()) )
            {
                int iBuffIndex = (rand()%int(m_vecRandMagicEffect.size()));

                DWORD dwAttackUnit = 0;
                if( OBJ_TYPE_PLAYER == pDamageInstance->m_emWhoType && 
                    true == g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) && 
                    false == g_MyD3D->IsSameTeam( pDamageInstance->m_Who, hitWho ) )
                {
                    dwAttackUnit = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->m_kUserInfo.dwUID;
                }

                g_LatencyTransfer.SendMagicEffectSynchPacket( m_vecRandMagicEffect[iBuffIndex].first, m_vecRandMagicEffect[iBuffIndex].second, true, hitWho, m_iMagicLevel, dwAttackUnit );
            }
        }
    }
    
    if ( (m_dwDMAttribute & DA_ONETIME_DAMAGE) && pDamageInstance->m_Life > 1)
    {
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
        pDamageInstance->m_Life = 1;
    }

    // 07/09/2007. daeuk. ������̰� DA_SHOW_DAMAGE_EFFECT �� ��Ÿ ����Ʈ�� �Ⱥٿ��� �ɵ� �ϴ�. �̹� ������ ����Ʈ ��ü�� ���ϴ�
    if( ( g_MyD3D->m_KGCOption.GetQualityCount() >= 2 || !( m_dwDMAttribute & DA_SHOW_DAMAGE_EFFECT ) ) && !( m_dwDMAttribute & DA_NOREACTION ) )
    {
        if ( m_dwDMAttribute & DA_HIT_LIGHT )
        {
            pDamageInstance->Add( HIT_LIGHT, (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who );
        }

        if ( m_dwDMAttribute & DA_PARTICLES )
        {
            if( m_vecSpark.size() == 0 )
            {
                g_MyD3D->MySparks->Particles( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 10 + pDamageInstance->m_Life / 20, 15, pDamageInstance->m_Who);
            }
            else
            {
                for( int i = 0 ; i < (int)m_vecSpark.size() ; ++i )
                {
                    switch( m_vecSpark[i].first )
                    {
                        case HE_RANDOM_DIRECTION_SPARK:
                            g_MyD3D->MySparks->Particles( (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 10 + pDamageInstance->m_Life / 20, 15, pDamageInstance->m_Who, m_vecSpark[i].second );
                            break;
                        case HE_HIT_EFFECT_SPARK:
                            pDamageInstance->Add( m_vecSpark[i].second, (pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who);
                            break;
                        case HE_HIT_EFFECT_BIM:
                            g_MyD3D->MySparks->Bim((pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y) / 3.0f, g_MyD3D->MyPlayer[hitWho]->vPos.y, 0, m_vecSpark[i].second);
                            break;
                    }
                }
            }
        }

        if ( m_dwDMAttribute & DA_FIRES )
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f );
        }
    }

    if ( m_dwDMAttribute & DA_SHOW_DAMAGE_EFFECT )
    {
        if( pDamageInstance->m_IsMonsterDamage ) { 
            MONSTER* pMonster = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
            if( pMonster ) { 
                pMonster->SetDamageEff( ( pDamageInstance->m_X - 1.0f + g_MyD3D->MyPlayer[hitWho]->MyBody.x + g_MyD3D->MyPlayer[hitWho]->MyBody.y ) / 3.0f, pDamageInstance->m_Y - 0.5f, 0.5f );
            }
        }
        else {
            D3DXVECTOR2 vRenderPos;
            GCCollisionRect<float> rtDiff;
            rtDiff.SetRect( 0.0f, 0.0f, 0.0f, 0.0f );
            if( rtDiff == pDamageInstance->m_rtCollision )
            {
                vRenderPos = D3DXVECTOR2(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f);
            }
            else
            {
                float fOffsetX = 0.0f;
                float fOffsetY = -0.25f;
                pDamageInstance->m_rtCollision.GetCenter(vRenderPos.x, vRenderPos.y);
                vRenderPos.x += fOffsetX;
                vRenderPos.y += fOffsetY;
            }
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDamageEff( vRenderPos.x, vRenderPos.y, 0.5f );
        }
    }

    if( m_dwDMAttribute & DA_SLOW_MOTION )
    {
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Slow_Count = 8;
    }

	if( pDamageInstance->m_pOriginalDamage->m_iSlowCount != 0 )
	{
		g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Slow_Count = static_cast<unsigned char>(pDamageInstance->m_pOriginalDamage->m_iSlowCount);
	}

    if( pDamageInstance->m_pOriginalDamage->m_iNextAttackCount != 0 ) 
	{
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count = pDamageInstance->m_pOriginalDamage->m_iNextAttackCount;
    }

    if( m_iPlusDelay != 0 && ( SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL || g_MyD3D->MyPlayer[hitWho]->IsLocalPlayer() == true ) )
    {
		if (m_iPlusDelay)
		{
			if (g_MyD3D->MyPlayer[hitWho]->Slow_Count < m_iPlusDelay)
			{
				g_MyD3D->MyPlayer[hitWho]->Slow_Count = m_iPlusDelay;
			}
		}
		else
		{
			if (g_MyD3D->MyPlayer[hitWho]->Slow_Count < g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().dwStopOnDamage)
			{
				g_MyD3D->MyPlayer[hitWho]->Slow_Count = (UCHAR)g_MyD3D->MyPlayer[hitWho]->GetCurFormTemplate().dwStopOnDamage;
			}
		}
    }

    if( m_iNoCheckContact != -1 ) 
	{
        g_MyD3D->MyPlayer[hitWho]->SetNoCheckContact( m_iNoCheckContact );
    } 
	else if( m_iIgnoreGroundCheckHittedObject != -1 ) 
	{
        g_MyD3D->MyPlayer[hitWho]->SetNoCheckContact( m_iIgnoreGroundCheckHittedObject );
    }

    if( m_dwDMAttribute & DA_PHYSICAL )
    {
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count = 21;
		TRACE( L"DAMAGE : Motion %d, Next_A_Count = %d\n", (int)g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion, (int)g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count );
    }

    if (m_iCrashSound > -1)
    {
        char strTemp[15];
        sprintf( strTemp , "%d" , m_iCrashSound);
        g_KDSound.Play( strTemp );
    }

    if( m_fLeechHP ) 
	{
        PLAYER::DamageInfo damage;
        damage.SetIgnoreDefence( pDamageInstance->m_pOriginalDamage->m_bIgnoreDefDamage );
        damage.SetDamage( -m_fDamageToPlayer * m_fLeechHP );
        damage.SetDamageInstanceAttacker( pDamageInstance );
        damage.SetAbilityEffect( true, false );
        damage.SetIgnoreDefence( true );
        damage.SetDamageFactor( false );
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Change_HP( damage );
    }

    if( m_fAbsorbHP ) 
	{
        SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
        if( -1 != sCharInfo.iCharType )
        {
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetHP( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() + (sCharInfo.iLevel * 0.1f * m_fAbsorbHP) );
        }
    }
	
    if( m_fAbsorbHPToMaxHP && m_iAddAbsorbHPPerHit == 0 )
    {
        if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_Who ) )
        {
			MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);
			if(pMonster){
				float fAbsorbHp = pMonster->GetHP() + (pMonster->GetMaxHP() * m_fAbsorbHPToMaxHP);
				pMonster->SetHP( fAbsorbHp );
			}

        }
        else if( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
        {
            SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
            if( -1 != sCharInfo.iCharType )
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetRecoveryHpByRadio( m_fAbsorbHPToMaxHP );
            }
        }
    }

    if( -1 < pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_iMonsterID )
    {
        if( -1 == pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_iSummonLifeTime )
        {
            pDamageInstance->m_pOriginalDamage->SummonMonster( pDamageInstance, hitWho );
            if( pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_bIsLock )
            {
                pDamageInstance->m_vecSummonMonHitWho.push_back( hitWho );
            }
        }
    }

    // ĳ���͸� Lock�Ѵ�.
    if( pDamageInstance->m_pOriginalDamage->m_bIsCharLock )
    {
        g_MyD3D->MyPlayer[hitWho]->SetLock( true );
        pDamageInstance->m_vecLockChar.push_back( g_MyD3D->MyPlayer[hitWho]->m_kUserInfo.dwUID );
    }

	g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP( m_fLeechMP );
	m_fLeechMP = 0.0f;

    return true;
}

void CDamage::CrashObject( CDamageInstance* pDamageInstance, int iObjIndex )
{
    if( !g_MyD3D->m_pObject[iObjIndex] ) return;
    //ONEHIT��������� �ѹ��� �°� �Ѵ�.

	if ( pDamageInstance->InsertAlreadyDamaged(iObjIndex + MAX_PLAYER_NUM) == false )
		return;

    bool UsePower = true;
#if defined(LEVEL_DESIGN_STAT_CALC) 
    UsePower = g_MyD3D->m_pObject[iObjIndex]->OnDamage(pDamageInstance->m_Who, m_fDamageToMonster * -1.0f,
        (EGCAttackType)m_iAttackType, (EGCAttackDirection)m_iAttackDirection,
        m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack);
#else
    UsePower = g_MyD3D->m_pObject[iObjIndex]->OnDamage(pDamageInstance->m_Who, m_fDamageToMonster * -1000.0f,
        (EGCAttackType)m_iAttackType, (EGCAttackDirection)m_iAttackDirection,
        m_fPushTargetX, m_fPushTargetY, m_bSpecialAttack);
#endif

    if ( ( m_dwDMAttribute & DA_ONETIME_DAMAGE ) && pDamageInstance->m_Life > 1)
    {
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
        pDamageInstance->m_Life = 1;
    }

	CrashEffect( pDamageInstance );


	g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP( m_fLeechMP );
	m_fLeechMP = 0.0f;
}

void CDamage::CrashMonster( CDamageInstance* pDamageInstance, int Monsteri )
{
#if !defined( __PATH__ )
    pDamageInstance->m_fRatio *= g_kGlobalValue.GetDamageRaito();
#endif

    bool bIsDarkLanceDmg = true;
    if ( pDamageInstance->m_pOriginalDamage->IsExistDamageFlag( DF_NO_HELL_SPEAR ) || pDamageInstance->m_bPetDamage ) {
        bIsDarkLanceDmg = false;
    }

    bool bIsNoCritDmg = false;
    if ( pDamageInstance->m_pOriginalDamage->IsExistDamageFlag( DF_NO_CRITICAL_DAMAGE_FOR_PLAYER ) ) {
        bIsNoCritDmg = true;
    }

    if( pDamageInstance->m_pOriginalDamage->m_bMonTargettingRelease )
    {
        MONSTER* pMon = g_kMonsterManager.GetMonster( Monsteri );
        pMon->ReleaseTarget( true );
        pMon->ReleaseTargetDamage();
        pMon->ReleaseTempTargetPos();
        pMon->ReleaseTargetSyncObject();
    }

    if( pDamageInstance->m_pOriginalDamage->IsExistDamageFlag(DF_CURE_FOR_MONSTER) ) {
        MONSTER* pMon = g_kMonsterManager.GetMonster( Monsteri );

		if( pMon == NULL )
			return;

        if( pMon->IsDie() ) { 
            return;
        }

        if( pMon->GetHP() > 0.0f ) { //&& pMon->m_iTeam == pDamageInstance->m_iTeam ) {
#if defined(LEVEL_DESIGN_STAT_CALC) 
            pMon->SetHP(pMon->GetHP() + m_fDamageToMonster * -1.0f);
#else
            pMon->SetHP(pMon->GetHP() + m_fDamageToMonster * -1000.0f);
#endif
        }

        if ( ( m_dwDMAttribute & DA_ONETIME_DAMAGE ) && pDamageInstance->m_Life > 1)
        {
            pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
            pDamageInstance->m_Life = 1;
        }

        if (m_iCrashSound > -1)
        {
            char strTemp[15];
            sprintf( strTemp , "%d" , m_iCrashSound);
            g_KDSound.Play( strTemp );
        }
        return;
    }    

    if( pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.iHitCnt > 0 &&
        pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.bOnlyPlayer == false ) {
        if( pDamageInstance->m_mapHitCount.find( Monsteri ) != pDamageInstance->m_mapHitCount.end() ) {

            if( pDamageInstance->m_mapHitCount[Monsteri] < pDamageInstance->m_pOriginalDamage->m_kRestrictHitCnt.iHitCnt ) {
                ++pDamageInstance->m_mapHitCount[Monsteri];
            } else {
                return;
            }        
        } else {
            pDamageInstance->m_mapHitCount[Monsteri] = 1;
        }    
    }

	g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);

	MONSTER *pMonster = g_kMonsterManager.GetMonster(Monsteri);
	if(pMonster == NULL) return;

	pMonster->SetAttackedMonster(true);

    if( pDamageInstance->m_ExtraMonsterNum == Monsteri && m_fSameTeamDamageToMonster == 0.0f )
        return;

	//ONEHIT��������� �ѹ��� �°� �Ѵ�.
	if ( pDamageInstance->InsertAlreadyDamaged(Monsteri + MAX_PLAYER_NUM) == false )
		return;

    const bool bSpecialPoint = ( DT_GAS != pDamageInstance->m_What );     // ����� ����Ʈ(�޺�, ��������<������>) ��� ����
                                                                          // ������ ��Ÿ�� ���Ƽ� �Ⱦ���.

    // �̽� ������ ó��
    if( false == pDamageInstance->m_IsMonsterDamage )
    {
        if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->IsAttackMiss() )
        {
            if( (timeGetTime() - g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_dwMissAttackTime) > MISS_ATTACK_TERM_TIME )
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_dwMissAttackTime = timeGetTime();
                D3DXVECTOR2 hitPos = pMonster->GetHitPos();
                pMonster->ShowMissAttack( hitPos.x, hitPos.y );
            }
            return;
        }
    }

    // ���� �������� ó��
    if( false == pMonster->GetPermissionDamage().empty() )
    {
        std::vector<int> vecPermissionDamage = pMonster->GetPermissionDamage();
        std::vector<int>::iterator vecPermissionDamageIter = vecPermissionDamage.begin();
        for(; vecPermissionDamageIter != vecPermissionDamage.end(); ++vecPermissionDamageIter)
        {
            if( pDamageInstance->m_What == (*vecPermissionDamageIter) )
                break;
        }
        if( vecPermissionDamageIter == vecPermissionDamage.end() )
        {
            DWORD *pdwMissAttackTime = NULL;
            if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_ExtraMonsterNum) )
            {
                MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                if( pMonster )  pdwMissAttackTime = &pMonster->m_dwMissAttackTime;
            }
            else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
            {
                pdwMissAttackTime = &g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_dwMissAttackTime;
            }

            if( (NULL != pdwMissAttackTime) && (timeGetTime() - (*pdwMissAttackTime)) > MISS_ATTACK_TERM_TIME )
            {
                (*pdwMissAttackTime) = timeGetTime();
                D3DXVECTOR2 hitPos = pMonster->GetHitPos();
                pMonster->ShowMissAttack( hitPos.x, hitPos.y );
            }
            return;
        }
    }

    // ������ ������ġ�� ������ ��������ġ�� �Ѱ��ش�.
    pMonster->m_vDamageStartPos = pDamageInstance->m_vDamageStartPos;
    pMonster->m_vDamageWhoPos = pDamageInstance->m_vDamageWhoPos;

    bool UsePower = true;
	if( !pDamageInstance->m_pOriginalDamage->m_bIsContinousDamage )
	{

        // ���� ������ ������ �������ֵ��� ����.
        if( g_kGlobalValue.m_kUserInfo.bHost )
        {
            // ������ �����Ѵ�.
            if( m_iUnMagicEffect > -1 )
            {
                if( pMonster->IsMagicEffect(Monsteri, m_iUnMagicEffect) )
                    pMonster->ClearMagicEffectNoLocal(Monsteri, m_iUnMagicEffect);
            }

            // �Ϲ����� ������ �Ǵ�.
            if( m_iMonsterMagicEff > -1 )
            {
                if( false == (m_bUniqueMgicEff && pMonster->IsMagicEffect(Monsteri, m_iMonsterMagicEff)) )
                {
                    int iWho = pDamageInstance->m_Who;
                    if( OBJ_TYPE_MONSTER == pDamageInstance->m_emWhoType )
                        iWho = pDamageInstance->m_ExtraMonsterNum + MAX_PLAYER_NUM;

                    if( m_bFastHostMagicEff )
                        pMonster->SetMagicEffect_NotMeBroad( Monsteri, m_iMonsterMagicEff, m_fMagicEffTime, m_iMagicLevel, iWho );
                    else
                        pMonster->SetMagicEffectNoLocal( Monsteri, m_iMonsterMagicEff, m_fMagicEffTime, m_iMagicLevel, iWho );
                }
            }

            // ���� ������ �Ǵ�.
            if( 0 < int(m_vecRandMagicEffect.size()) )
            {
                int iWho = pDamageInstance->m_Who;
                int iBuffIndex = (rand()%int(m_vecRandMagicEffect.size()));
                if( OBJ_TYPE_MONSTER == pDamageInstance->m_emWhoType )
                    iWho = pDamageInstance->m_ExtraMonsterNum + MAX_PLAYER_NUM;

                pMonster->SetMagicEffectNoLocal( Monsteri, m_vecRandMagicEffect[iBuffIndex].first, m_vecRandMagicEffect[iBuffIndex].second, m_iMagicLevel, iWho );
            }
        }

        if (m_fDamageToMonster != 0.0f || m_bMonSuperDamage)
        {
            if (pDamageInstance->IsHitPerFrame() && !pMonster->IsInvincible())
            {
                EGCAttackType atkType = (EGCAttackType)m_iAttackType;
                if (pMonster->CheckAerial())
                {
                    atkType = (EGCAttackType)m_iAttackTypeOnAerial;
                }

                float fDamage = m_fDamageToMonster;
                if ((pDamageInstance->m_pOriginalDamage->m_bSpecialAttack || pDamageInstance->m_pOriginalDamage->m_bSlotUniqueSkillDmg) &&
                    pDamageInstance->m_pOriginalDamage->m_bSpecial4Damage == false) {
                    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON) {
                        fDamage = SiGCGrowingSkillManager()->CountChangeDungeonDamageRate(g_MyD3D->MyPlayer[pDamageInstance->m_Who], m_fDamageToMonster);
                    }
                    else {
                        fDamage = SiGCGrowingSkillManager()->CountChangeDamageRate(g_MyD3D->MyPlayer[pDamageInstance->m_Who], m_fDamageToMonster);
                    }
                }
                // 같은 팀 몬스터에게 주는 데미지이다.
                if (0.0f != m_fSameTeamDamageToMonster) {
                    if (pDamageInstance->m_IsMonsterDamage) {
                        if (g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, pMonster->m_iMonsterIndex))
                            fDamage = m_fSameTeamDamageToMonster;
                    }
                    else {
                        if (g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, pMonster->m_iMonsterIndex))
                            fDamage = m_fSameTeamDamageToMonster;
                    }
                }

                if( 0.0f != fDamage )
                {
                    std::string strReaction;
                    if( 0 < static_cast<int>(strlen(m_kMonReaction.m_strReaction.c_str())) )
                    {
                        std::vector<std::string>::iterator vecPassIter = m_kMonReaction.m_vecPassState.begin();
                        for(; vecPassIter != m_kMonReaction.m_vecPassState.end(); ++vecPassIter)
                        {
                            if( pMonster->CheckState( const_cast<char*>(vecPassIter->c_str()) ) )
                                break;
                        }

                        bool bSetReaction = true;
                        if( m_kMonReaction.m_bPassBoss )
                        {
                            if( pMonster->m_iBossMonster == MONSTER::MON_BOSS || pMonster->m_iBossMonster == MONSTER::MON_MIDBOSS )
                                bSetReaction = false;
                        }

                        if( vecPassIter != m_kMonReaction.m_vecPassState.end() )
                            bSetReaction = false;

                        if( bSetReaction )
                            strReaction = m_kMonReaction.m_strReaction.c_str();
                    }

                    float fDiff = 0.0f;
                    bool bBuffCreateDamage = m_bSpecialAttack;
                    if( pDamageInstance->m_pOriginalDamage->IsExistDamageFlag( DF_BUFF_CREATE_DAMAGE ) ) {
                        bBuffCreateDamage = true;
                    }
#if defined(LEVEL_DESIGN_STAT_CALC) 
                    UsePower = pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
                        fDamage * -1.0f * pDamageInstance->m_fRatio * m_kDivisionDamage.m_fRangeDamageRatio,
                        atkType, (EGCAttackDirection)m_iAttackDirection,
                        m_fPushTargetX, m_fPushTargetY, bBuffCreateDamage, m_nSpecialGrade, bSpecialPoint, false, pDamageInstance->m_bPetDamage, m_bMonSuperDamage, &fDiff, (0 < static_cast<int>(strlen(strReaction.c_str()))), pDamageInstance->m_ExtraMonsterNum, bIsDarkLanceDmg, bIsNoCritDmg);

#else
                    UsePower = pMonster->OnDamage(pDamageInstance->m_Who, pDamageInstance->m_What,
                        fDamage * -1000.0f * pDamageInstance->m_fRatio * m_kDivisionDamage.m_fRangeDamageRatio,
                        atkType, (EGCAttackDirection)m_iAttackDirection,
                        m_fPushTargetX, m_fPushTargetY, bBuffCreateDamage, m_nSpecialGrade, bSpecialPoint, false, pDamageInstance->m_bPetDamage, m_bMonSuperDamage, &fDiff, (0 < static_cast<int>(strlen(strReaction.c_str()))), pDamageInstance->m_ExtraMonsterNum, bIsDarkLanceDmg, bIsNoCritDmg);
#endif
                    if( 0.0f < pMonster->GetHP() && 0 < static_cast<int>(strlen(strReaction.c_str())) )
                        pMonster->DirectMotionInput( strReaction.c_str() );

                    // �� ������ �翡 ����ؼ� �������� HP�� �����Ѵ�.
                    if( 0.0f != pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage )
                    {
                        if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) )
                        {
                            MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                            if(pMonster != NULL)
                            {
                                float fDamage = (fDiff * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage);
                                if( g_kGlobalValue.m_kUserInfo.bHost )
                                    pMonster->SetHP( pMonster->GetHP() + fDamage );
                                else
                                    pMonster->SendToHostOfMonsterHP( pMonster->GetHP() + fDamage );
                                if( 0.0f < fDamage )
                                    pMonster->RenderHitNumber( fDamage, true, pMonster->GetX(), pMonster->GetY(), false );   // ���ʹ� �Ķ����ڷ� HP�������� ��������.
                            }
                        }
                        else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
                        {
                            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetHP( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() + (fDiff * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage) );
                        }
                    }
                }
			}
		}

        if( 0 < pDamageInstance->m_pOriginalDamage->m_iStopUnitFrame )
        {
            pMonster->SetStopSpeedFrame( pDamageInstance->m_pOriginalDamage->m_iStopUnitFrame );
        }
	}
	else
	{
		if( !o10 )
		{
			if( m_fDamageToMonster != 0.0f )
			{
                bool bBuffCreateDamage = false;
                if( pDamageInstance->m_pOriginalDamage->IsExistDamageFlag( DF_BUFF_CREATE_DAMAGE ) || pDamageInstance->m_bPetDamage ) {
                    bBuffCreateDamage = true;
                }

                float fDamageToMonster = ASMfabs( m_fDamageToMonster );
#if defined(LEVEL_DESIGN_STAT_CALC) 
                float fDiff = pMonster->ChangeHP(NULL, fDamageToMonster * 0.06f * m_kDivisionDamage.m_fRangeDamageRatio, pDamageInstance->m_Who, bBuffCreateDamage, false, 0, pDamageInstance->m_ExtraMonsterNum, bIsDarkLanceDmg, bIsNoCritDmg);
#else
                float fDiff = pMonster->ChangeHP(NULL, fDamageToMonster * 60.0f * m_kDivisionDamage.m_fRangeDamageRatio, pDamageInstance->m_Who, bBuffCreateDamage, false, 0, bIsDarkLanceDmg, bIsNoCritDmg);
#endif
                pMonster->SetAttackedMonster(true);
                pMonster->SetHitPos( D3DXVECTOR2( pMonster->GetX(), pMonster->GetY() ) );

                // �� ������ �翡 ����ؼ� �������� HP�� �����Ѵ�.
                if( 0.0f != pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage )
                {
                    if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) )
                    {
                        MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
                        if(pMonster != NULL)
                        {
                            float fDamage = (fDiff * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage);
                            if( g_kGlobalValue.m_kUserInfo.bHost )
                                pMonster->SetHP( pMonster->GetHP() + fDamage );
                            else
                                pMonster->SendToHostOfMonsterHP( pMonster->GetHP() + fDamage );
                            if( 0.0f < fDamage )
                                pMonster->RenderHitNumber( fDamage, true, pMonster->GetX(), pMonster->GetY(), false );   // ���ʹ� �Ķ����ڷ� HP�������� ��������.
                        }
                    }
                    else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
                    {
                        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetHP( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() + (fDiff * pDamageInstance->m_pOriginalDamage->m_fAbsorbHPToDamage) );
                    }
                }
			}
		}
	}

    if ( ( m_dwDMAttribute & DA_ONETIME_DAMAGE ) && pDamageInstance->m_Life > 1)
    {
        pDamageInstance->m_PrevLife = pDamageInstance->m_Life;
        pDamageInstance->m_Life = 1;
    }

    // �������� Crash�� ������ ������ �����ڸ� �̵���Ų��.
    if( m_kMoveCreateToCrashPos.m_bIsMoving && false == pDamageInstance->m_bMoveCrashPos )
    {
        pDamageInstance->m_bMoveCrashPos = true;
        if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) )
        {
            float fX = pDamageInstance->m_X + m_kMoveCreateToCrashPos.m_fOffsetX;
            float fY = pDamageInstance->m_Y + m_kMoveCreateToCrashPos.m_fOffsetY;
			MONSTER *pMonsterPos = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

            pMonsterPos->SetX( fX );
            pMonsterPos->SetY( fY );
        }
        else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
        {
            float fX = pDamageInstance->m_X + m_kMoveCreateToCrashPos.m_fOffsetX;
            float fY = pDamageInstance->m_Y + m_kMoveCreateToCrashPos.m_fOffsetY;
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDirectMove( fX, fY );
        }

        if( -1 != m_kMoveCreateToCrashPos.m_iDamageLife )
        {
            pDamageInstance->m_Life = m_kMoveCreateToCrashPos.m_iDamageLife;
        }
    }

    // �ǰ��� ��ġ�� ������ �����ڸ� �̵���Ų��.
    if( m_kMoveCreateToUnitPos.m_bIsMoving && false == pDamageInstance->m_bMoveUnitPos )
    {
        pDamageInstance->m_bMoveUnitPos = true;

        float fX = pDamageInstance->m_X + m_kMoveCreateToUnitPos.m_fOffsetX;
        float fY = pDamageInstance->m_Y + m_kMoveCreateToUnitPos.m_fOffsetY;
        if( OBJ_TYPE_MONSTER == pDamageInstance->m_emHitWhoType && g_kMonsterManager.IsMonsterIndex( Monsteri ) )
        {
            MONSTER *pHitMonster = g_kMonsterManager.GetMonster(Monsteri);
            if( pHitMonster != NULL )
            {
                fX = pHitMonster->GetX() + m_kMoveCreateToUnitPos.m_fOffsetX;
                fY = pHitMonster->GetY() + m_kMoveCreateToUnitPos.m_fOffsetY;
            }
        }

        if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_ExtraMonsterNum ) )
        {
            MONSTER *pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
            if(pMonster != NULL)
            {
                pMonster->SetX( fX );
                pMonster->SetY( fY );
            }
        }
        else if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
        {
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x = fX;
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y = fY;
        }

        if( -1 != m_kMoveCreateToUnitPos.m_iDamageLife )
        {
            pDamageInstance->m_Life = m_kMoveCreateToUnitPos.m_iDamageLife;
        }
    }

    CrashEffect( pDamageInstance );

	if (m_iPlusDelay && g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost)
	{
		if (!g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].bNoPetPlusDelay)
		{
			pMonster->m_iSlowCount = m_iPlusDelay;
		}
	}

    if( m_fLeechHP ) {
        PLAYER::DamageInfo damage;
        damage.SetIgnoreDefence( pDamageInstance->m_pOriginalDamage->m_bIgnoreDefDamage );
        damage.SetDamage( -m_fDamageToPlayer* m_fLeechHP );
        damage.SetDamageInstanceAttacker( pDamageInstance );
        damage.SetAbilityEffect( true, false );
        damage.SetIgnoreDefence( true );
        damage.SetDamageFactor( false );
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Change_HP( damage );
    }

    if( m_fAbsorbHP ) {
        SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
        if( -1 != sCharInfo.iCharType )
        {
            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetHP( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() + (sCharInfo.iLevel * 0.1f * m_fAbsorbHP) );
        }
    }

    if( m_fAbsorbHPToMaxHP && m_iAddAbsorbHPPerHit == 0 )
    {
        if( pDamageInstance->m_IsMonsterDamage && g_kMonsterManager.IsMonsterIndex( pDamageInstance->m_Who ) )
        {
			MONSTER *pMonsterPos = g_kMonsterManager.GetMonster(pDamageInstance->m_Who);

            float fAbsorbHp = pMonsterPos->GetHP() + (pMonsterPos->GetMaxHP() * m_fAbsorbHPToMaxHP);
            pMonsterPos->SetHP( fAbsorbHp );
        }
        else if( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
        {
            SCharInfo& sCharInfo = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->m_kUserInfo.GetCurrentChar();
            if( -1 != sCharInfo.iCharType )
            {
                g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetRecoveryHpByRadio( m_fAbsorbHPToMaxHP );
            }
        }
    }

    // ���� ��ȯ
    if( -1 < pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_iMonsterID &&
        false == pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_bIsLock )
    {
        if( -1 == pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_iSummonLifeTime )
            pDamageInstance->m_pOriginalDamage->SummonMonster( pDamageInstance );
    }

	g_MyD3D->MyPlayer[pDamageInstance->m_Who]->DecreaseMP( m_fLeechMP );
	m_fLeechMP = 0.0f;
    
    if( g_MyD3D->GetMyPlayer() && pDamageInstance->m_Who == g_MyD3D->Get_MyPlayer() ) {
        // ���� ������ ����
        g_MyD3D->GetMyPlayer()->CheckChargeMarbleDamageAtk( pDamageInstance->m_What );
        // ���� ������ ����
        g_MyD3D->GetMyPlayer()->CheckChargeBurnDamageAtk( pDamageInstance->m_What );
		// �� ������ ����
		g_MyD3D->GetMyPlayer()->CheckChargeBloodDamageAtk( pDamageInstance->m_What );

        // ����� ���� ����
        if( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.empty() == false ) {
            std::map< int, std::map< int , std::pair< int , float > > >::iterator mit = g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.find( SPECIAL_BUFF_SUCCESS_ATK );
            if( mit != g_MyD3D->GetMyPlayer()->GetCurFormTemplate().mapSpecialBuff.end() ) {
                std::map< int , std::pair< int , float > >::iterator mitSB = mit->second.begin();
                PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
                for( mitSB; mitSB != mit->second.end(); ++mitSB ) {
                    if( g_MyD3D->GetMyPlayer()->CheckSkill( mitSB->first )  && 
                        g_MyD3D->GetMyPlayer()->SkillAloowMotion( pPlayer->GetCurrentChar().iCharType, pPlayer->uiMotion ) 
                        ) {
                        //g_pMagicEffect->SetMagicEffect( g_MyD3D->Get_MyPlayer(), mitSB->second.first, mitSB->second.second );
                        g_MyD3D->GetMyPlayer()->SetMagicEffect( mitSB->second.first, mitSB->second.second );
                    }
                }
            }
        }        
    }
}

void CDamage::CrashEffect( CDamageInstance* pDamageInstance )
{
    if( g_MyD3D->m_KGCOption.GetQualityCount() >= 2 || !(m_dwDMAttribute & DA_SHOW_DAMAGE_EFFECT) )
    {
        if ( m_dwDMAttribute & DA_HIT_LIGHT )
        {
            pDamageInstance->Add( HIT_LIGHT, pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who );
        }

        if ( m_dwDMAttribute & DA_PARTICLES )
        {
            if( m_vecSpark.size() == 0 )
            {
                g_MyD3D->MySparks->Particles( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 10 + pDamageInstance->m_Life / 40, 15, pDamageInstance->m_Who );
            }
            else
            {
                for( int i = 0 ; i < (int)m_vecSpark.size() ; ++i )
                {
                    switch( m_vecSpark[i].first )
                    {
                    case HE_RANDOM_DIRECTION_SPARK:
                        if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                            g_MyD3D->MySparks->Particles( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 10 + pDamageInstance->m_Life / 40, 15, pDamageInstance->m_Who, m_vecSpark[i].second );
                        break;
                    case HE_HIT_EFFECT_SPARK:
                        pDamageInstance->Add(  m_vecSpark[i].second , pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.7f, -100.0f, -100.0f, 0.0f, false, pDamageInstance->m_Who );
                        break;
                    case HE_HIT_EFFECT_BIM:
                        if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                            g_MyD3D->MySparks->Bim(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, m_vecSpark[i].second);
                        break;
                    }
                }
            }
        }

        if ( m_dwDMAttribute & DA_FIRES )
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                g_MyD3D->MySparks->Fires( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.2f, 0, 0.5f );
        }
    }

    if ( m_dwDMAttribute & DA_SHOW_DAMAGE_EFFECT )
    {
		if( pDamageInstance->m_IsMonsterDamage ){
			MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

			if(pMonster != NULL ) {
				pMonster->SetDamageEff( pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f, 0.5f );
			}
		}
        else
        {
            D3DXVECTOR2 vRenderPos;
            GCCollisionRect<float> rtDiff;
            rtDiff.SetRect( 0.0f, 0.0f, 0.0f, 0.0f );
            if( rtDiff == pDamageInstance->m_rtCollision )
            {
                vRenderPos = D3DXVECTOR2(pDamageInstance->m_X - 1.0f, pDamageInstance->m_Y - 0.5f);
            }
            else
            {
                float fOffsetX = 0.0f;
                float fOffsetY = -0.25f;
                pDamageInstance->m_rtCollision.GetCenter(vRenderPos.x, vRenderPos.y);
                vRenderPos.x += fOffsetX;
                vRenderPos.y += fOffsetY;
            }

            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->SetDamageEff( vRenderPos.x, vRenderPos.y, 0.5f );
        }
    }

    if( m_dwDMAttribute & DA_SLOW_MOTION )
    {
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Slow_Count = 8;
    }

	if( pDamageInstance->m_pOriginalDamage->m_iSlowCount != 0 )
	{
		g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Slow_Count = pDamageInstance->m_pOriginalDamage->m_iSlowCount;
	}

    if( pDamageInstance->m_pOriginalDamage->m_iNextAttackCount != 0 ) {
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count = pDamageInstance->m_pOriginalDamage->m_iNextAttackCount;
    }

    if( m_dwDMAttribute & DA_PHYSICAL )
    {
        g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Next_Attack_Count = 21;
    }


    int iTemp = m_iCrashSound;
    char strTemp[50];
    sprintf( strTemp , "%d" , iTemp);
    g_KDSound.Play( strTemp );
}

void CDamage::DamageHpEffect( CDamageInstance* pDamageInstance )
{
    if( pDamageInstance )
    {
        std::vector< DAMAGE_PARTICLE >::iterator vecIter = m_vecDamageHpParticle.begin();
        for(; vecIter != m_vecDamageHpParticle.end(); ++vecIter)
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                pDamageInstance->CreateSequence( vecIter->m_particleSeqName, m_vPos.x + vecIter->m_fXOffset, m_vPos.y + vecIter->m_fYOffset, vecIter->m_fTraceTime );
        }
    }
}

void CDamage::InToggleMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos )
{
    if( pDamageInstance )
    {
        std::vector< DAMAGE_PARTICLE >::iterator vecIter = m_vecInToggleMeshParticle.begin();
        for(; vecIter != m_vecInToggleMeshParticle.end(); ++vecIter)
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                pDamageInstance->CreateSequence( vecIter->m_particleSeqName, vPos.x + vecIter->m_fXOffset, vPos.y + vecIter->m_fYOffset, vecIter->m_fTraceTime );
        }
    }
}

void CDamage::OutToggleMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos )
{
    if( pDamageInstance )
    {
        std::vector< DAMAGE_PARTICLE >::iterator vecIter = m_vecOutToggleMeshParticle.begin();
        for(; vecIter != m_vecOutToggleMeshParticle.end(); ++vecIter)
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                pDamageInstance->CreateSequence( vecIter->m_particleSeqName, vPos.x + vecIter->m_fXOffset, vPos.y + vecIter->m_fYOffset, vecIter->m_fTraceTime );
        }
    }
}

void CDamage::InToggleSummonMonsterMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos )
{
    if( pDamageInstance )
    {
        std::vector< DAMAGE_PARTICLE >::iterator vecIter = m_vecInToggleSummonMonsterMeshParticle.begin();
        for(; vecIter != m_vecInToggleSummonMonsterMeshParticle.end(); ++vecIter)
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                pDamageInstance->CreateSequence( vecIter->m_particleSeqName, vPos.x + vecIter->m_fXOffset, vPos.y + vecIter->m_fYOffset, vecIter->m_fTraceTime );
        }
    }
}

void CDamage::OutToggleSummonMonsterMeshDamageEffect( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos )
{
    if( pDamageInstance )
    {
        std::vector< DAMAGE_PARTICLE >::iterator vecIter = m_vecOutToggleSummonMonsterMeshParticle.begin();
        for(; vecIter != m_vecOutToggleSummonMonsterMeshParticle.end(); ++vecIter)
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                pDamageInstance->CreateSequence( vecIter->m_particleSeqName, vPos.x + vecIter->m_fXOffset, vPos.y + vecIter->m_fYOffset, vecIter->m_fTraceTime );
        }
    }
}

void CDamage::MPDamageParticle( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos )
{
    if( pDamageInstance )
    {
        std::vector< DAMAGE_PARTICLE >::iterator vecIter = m_vecMPDamageParticle.begin();
        for(; vecIter != m_vecMPDamageParticle.end(); ++vecIter)
        {
            if ( !g_kGlobalValue.IsSkillEffectDisabled( pDamageInstance->m_Who ) )
                pDamageInstance->CreateSequence( vecIter->m_particleSeqName, vPos.x + vecIter->m_fXOffset - 1.0f, vPos.y + vecIter->m_fYOffset - 0.5f, vecIter->m_fTraceTime );
        }
    }
}

void CDamage::AngleFrameMove( CDamageInstance* pDamageInstance )
{

    if( m_IsRealTimeAngleMove )
    {
        float Length = ASMsqrt( powf( m_Start_Locate_x, 2.0f ) + powf( m_Start_Locate_y, 2.0f ));        
        float Angle = 0.0f;
        Angle = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle * 0.75f;

        if( pDamageInstance->m_IsRight )
        {
            pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x + cosf(Angle) * Length - Length + m_Start_Locate_x;
            pDamageInstance->m_Angle =  g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
        }
        else
        {
            pDamageInstance->m_X = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.x - cosf(Angle) * Length + Length - m_Start_Locate_x;
            pDamageInstance->m_Angle = -g_MyD3D->MyPlayer[pDamageInstance->m_Who]->Body_Angle;
        }

        if( Angle >= 0 ) {
            pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + m_Start_Locate_y + sinf(Angle) * Length - 0.02f;
        } else {
            pDamageInstance->m_Y = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->vPos.y + m_Start_Locate_y + sinf(Angle) * Length + 0.02f;
        }
    }

    if ( m_x_Speed == 0.0f && m_y_Speed == 0.0f )
        return;

    float fSpeedX = m_x_Speed;
    float fSpeedY = m_y_Speed;

    if( m_x_CrashSpeed && pDamageInstance->GetCrashCheck() )
        fSpeedX = m_x_CrashSpeed;
    if( m_y_CrashSpeed && pDamageInstance->GetCrashCheck() )
        fSpeedY = m_y_CrashSpeed;
    if( pDamageInstance->GetDamageCrashSpeedX() )
        fSpeedX += (fSpeedX * pDamageInstance->GetDamageCrashSpeedX());
    if( pDamageInstance->GetDamageCrashSpeedY() )
        fSpeedY += (fSpeedY * pDamageInstance->GetDamageCrashSpeedY());

    // �ӵ��� ���� ��ġ ����
    float Length = ASMsqrt( fSpeedX * fSpeedX + fSpeedY * fSpeedY );
    float Angle = acosf( fSpeedX / Length );

    //�ü� ��ȸ���� Ȱ���� �Ϸ� ������ ���ƾ� �Ѵ�.
    if( pDamageInstance->m_What != DT_ARROW 
        || pDamageInstance->m_Life > 100 )
    {
        if( pDamageInstance->m_IsRight )
        {
            if( m_fLimite_Angle && (pDamageInstance->m_Angle + Angle) < -m_fLimite_Angle )
            {
                pDamageInstance->m_X += cosf( -m_fLimite_Angle ) * Length;
                pDamageInstance->m_Y += sinf( -m_fLimite_Angle ) * Length;
            }
            else
            {
                pDamageInstance->m_X += cosf(pDamageInstance->m_Angle + Angle) * Length;
                pDamageInstance->m_Angle += m_Angle_Speed;
                pDamageInstance->m_Y += sinf(pDamageInstance->m_Angle + Angle) * Length;
            }
        }
        else
        {
            if( m_fLimite_Angle && (pDamageInstance->m_Angle + Angle) > m_fLimite_Angle )
            {
                pDamageInstance->m_X += cosf( m_fLimite_Angle ) * Length;
                pDamageInstance->m_Y += sinf( m_fLimite_Angle ) * Length;
            }
            else
            {
                pDamageInstance->m_X -= cosf(pDamageInstance->m_Angle + Angle) * Length;
                pDamageInstance->m_Angle -= m_Angle_Speed;
                pDamageInstance->m_Y -= sinf(pDamageInstance->m_Angle + Angle) * Length;
            }
        }
    }

    if( pDamageInstance->m_pOriginalDamage->m_bParticleAngleMove )
    {
        std::vector< CParticleEventSeqPTR >::iterator vecIter = pDamageInstance->m_vecChildParticle.begin();
        for(; vecIter != pDamageInstance->m_vecChildParticle.end(); ++vecIter)
        {
            if( false == vecIter->IsValid() )
                continue;

            D3DXVECTOR3 vOffset = (*vecIter)->GetRotateOffset();
            (*vecIter)->SetRotateOffset( vOffset.x, vOffset.y, ((pDamageInstance->m_Angle*180.0f)/3.14f) );
        }
    }
}

void CDamage::NotAngleFrameMove( CDamageInstance* pDamageInstance )
{
    float fSpeedX = m_x_Speed;
    float fSpeedY = m_y_Speed;

    // ĳ���ͳ� ���Ϳ� �浹������ �ӵ��� ����
    if( m_x_CrashSpeed && pDamageInstance->GetCrashCheck() )
        fSpeedX = m_x_CrashSpeed;
    if( m_y_CrashSpeed && pDamageInstance->GetCrashCheck() )
        fSpeedY = m_y_CrashSpeed;

    // �������� �������� �浹������ ��� �������� �������� %�� �ӵ� ����
    if( pDamageInstance->GetDamageCrashSpeedX() )
        fSpeedX += (fSpeedX * pDamageInstance->GetDamageCrashSpeedX());
    if( pDamageInstance->GetDamageCrashSpeedY() )
        fSpeedY += (fSpeedY * pDamageInstance->GetDamageCrashSpeedY());

    // �ӵ��� ���� ��ġ ����
    if( pDamageInstance->m_IsRight )
    {
        pDamageInstance->m_X += fSpeedX;
        pDamageInstance->m_Angle += m_Angle_Speed;
    }
    else
    {
        pDamageInstance->m_X -= fSpeedX;
        pDamageInstance->m_Angle -= m_Angle_Speed;
    }

    pDamageInstance->m_Y += fSpeedY;
}

// 2011.1.15, shmhlove : ����ź Create
void CDamage::CreateGuidedMissileTarget( CDamageInstance* pDamageInstance )
{
    // ����ó��
    if( NULL == pDamageInstance || NULL == pDamageInstance->m_pOriginalDamage )
        return;

    // ��� ���̱�
    GUIDEDMISSILE &GuidedMissile = pDamageInstance->m_pOriginalDamage->m_GuidedMissile;

    // ������ ������ ��ġ �� �߻� ���� ó��
    D3DXVECTOR2 vPos;
    if( pDamageInstance->m_IsMonsterDamage )
    {
        MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);

        if(pMonster != NULL ) {
            vPos = D3DXVECTOR2( pMonster->GetX(), pMonster->GetY() );
            if( !pMonster->bIsRight )
                GuidedMissile.m_vGMShottingDist.x = -GuidedMissile.m_vGMShottingDist.x;
        }
    }
    else
    {
        if( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) == false)
            return;

        vPos = D3DXVECTOR2( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().x, 
                            g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().y );

        if( !g_MyD3D->MyPlayer[pDamageInstance->m_Who]->bIsRight )
            GuidedMissile.m_vGMShottingDist.x = -GuidedMissile.m_vGMShottingDist.x;
    }

    // ���Ǻ� Ÿ�� ����
    bool bSuccessTarget = SetGuidedMissileTarget (pDamageInstance, GuidedMissile, vPos );

    // ���� ����ź�ϰ�� Ÿ�Ϲ��� ����
    if( GuidedMissile.m_bIsStraight )
    {
        D3DXVECTOR2 vTarget = GetGuidedTargetPos( pDamageInstance, GuidedMissile );
        D3DXVECTOR2 vDist;
        if( (0.0f == vTarget.x) && (0.0f == vTarget.y) )
            vDist = GuidedMissile.m_vGMShottingDist;
        else
            vDist =  vTarget - D3DXVECTOR2(pDamageInstance->m_X, pDamageInstance->m_Y);

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, vTarget) )
                vDist = GuidedMissile.m_vGMShottingDist;
        }
        
        D3DXVECTOR3 v3TargetDist(vDist.x, vDist.y, 0.0f);
        D3DXVec3Normalize( &v3TargetDist, &v3TargetDist);
        GuidedMissile.m_vGMShottingDist.x = v3TargetDist.x;
        GuidedMissile.m_vGMShottingDist.y = v3TargetDist.y;

        // ������ ����ź�� Ÿ�� ��ġ �����ص� 
        pDamageInstance->m_dxvTargetPos = vTarget;
    }

    // Ÿ���� ������� ������ ������ �����Ѵ�.
    if( (0 <= GuidedMissile.m_iNotTargetLife) && (false == bSuccessTarget) )
    {
        pDamageInstance->m_Life = GuidedMissile.m_iNotTargetLife;
    }
}

// 2011.1.15, shmhlove : ����ź FrameMove
#define RADIAN D3DX_PI/180.0f
void CDamage::GuideMissileFrameMove( CDamageInstance* pDamageInstance )
{
    // ����ó��
    if( NULL == pDamageInstance || NULL == pDamageInstance->m_pOriginalDamage )
        return;

    // ��� ���̱�
    GUIDEDMISSILE &GuidedMissile = pDamageInstance->m_pOriginalDamage->m_GuidedMissile;

    // ���� ����Ÿ�� ����
    if( 0 < GuidedMissile.m_iNotGMTime )
    {
        --GuidedMissile.m_iNotGMTime;
        float fTempX = m_x_Speed, fTempY = m_y_Speed;
        m_x_Speed = GuidedMissile.m_fNotGMTimeSpeedX;
        m_y_Speed = GuidedMissile.m_fNotGMTimeSpeedY;
        NotAngleFrameMove( pDamageInstance );
        m_x_Speed = fTempX;
        m_y_Speed = fTempY;
        return;
    }

    // ���� �� Ÿ�� ����
    if( pDamageInstance->m_Life < GuidedMissile.m_iEndGMTime )
    {
        pDamageInstance->m_X += (m_GuidedMissile.m_vGMShottingDist.x * m_x_Speed);
        pDamageInstance->m_Y += (m_GuidedMissile.m_vGMShottingDist.y * m_y_Speed);
        return;
    }

    // Ÿ�� ����
    if( GuidedMissile.m_bIsResetTarget )
    {
        D3DXVECTOR2 vPos;
        if( pDamageInstance->m_IsMonsterDamage )
        {
            MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
            if(pMonster != NULL )
                vPos = D3DXVECTOR2( pMonster->GetX(), pMonster->GetY() );
        }
        else
        {
            if( g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
            {
                vPos = D3DXVECTOR2( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().x, 
                    g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().y );
            }
        }

        SetGuidedMissileTarget( pDamageInstance, GuidedMissile, vPos );
    }

    // Ÿ�Ϲ������� ������ ���� ó��
    if( GuidedMissile.m_bUseTargetDirection )
    {
        bool bDamageIsRight = pDamageInstance->m_IsRight;
        if( GuidedMissile.m_bIsGMTargetMonster )
        {
            MONSTER *pMonster = g_kMonsterManager.GetMonster( GuidedMissile.m_iGMTargetIndex );
            if( pMonster )
                bDamageIsRight = ( pMonster->GetX() < pDamageInstance->m_X ) ? false : true;
        }
        else
        {
            if( g_MyD3D->IsPlayerIndex( GuidedMissile.m_iGMTargetIndex ) )
                bDamageIsRight = ( g_MyD3D->MyPlayer[GuidedMissile.m_iGMTargetIndex]->GetX() < pDamageInstance->m_X ) ? false : true;
        }
        pDamageInstance->m_IsRight = bDamageIsRight;
    }

    // ���� ����ź
    if( GuidedMissile.m_bIsStraight )
    {
        StraightGuideFrameMove( pDamageInstance, GuidedMissile );
        return;
    }

    // Ÿ�� ã��
    D3DXVECTOR2 vTargetPos = GetGuidedTargetPos( pDamageInstance, GuidedMissile );

    // �߻� ����
    D3DXVECTOR3 v3ShottingDist( m_GuidedMissile.m_vGMShottingDist.x, m_GuidedMissile.m_vGMShottingDist.y, 0.0f );
    D3DXVec3Normalize( &v3ShottingDist, &v3ShottingDist);

    // Ÿ�� ����
    D3DXVECTOR2 vDist = vTargetPos - D3DXVECTOR2(pDamageInstance->m_X, pDamageInstance->m_Y);
    D3DXVECTOR3 v3TargetDist(vDist.x, vDist.y, 0.0f);
    D3DXVec3Normalize( &v3TargetDist, &v3TargetDist);

    // �������;�� ( ������ ȸ�� ó�� )
    D3DXVECTOR3 v3Axis;
    D3DXVec3Cross( &v3Axis, &v3ShottingDist, &v3TargetDist );

    // ���������� HomingAngle��ŭ �߻���� ȸ��
    D3DXMATRIX matRotate;
    D3DXMatrixIdentity( &matRotate );
    D3DXMatrixRotationAxis( &matRotate, &v3Axis, (m_GuidedMissile.m_fHomingAngle * RADIAN) );
    D3DXVec3TransformNormal( &v3ShottingDist, &v3ShottingDist, &matRotate );

    // ȸ���� �������� �߻���� ����
    m_GuidedMissile.m_vGMShottingDist.x = v3ShottingDist.x;
    m_GuidedMissile.m_vGMShottingDist.y = v3ShottingDist.y;

    // �߻�������� �ӵ���ŭ ������ ��ġ �̵�
    pDamageInstance->m_fAccGuidSpeedX += m_GuidedMissile.m_vGMAccelerator.x;
    pDamageInstance->m_fAccGuidSpeedY += m_GuidedMissile.m_vGMAccelerator.y;
    pDamageInstance->m_X += (m_GuidedMissile.m_vGMShottingDist.x * (m_x_Speed+pDamageInstance->m_fAccGuidSpeedX));
    pDamageInstance->m_Y += (m_GuidedMissile.m_vGMShottingDist.y * (m_y_Speed+pDamageInstance->m_fAccGuidSpeedY));

    // ������ �޽ð� �ִ� ����ź�̶�� �޽� ���⵵ 
    if( pDamageInstance->m_p3DObject != NULL ) { 
        pDamageInstance->m_p3DObject->IsRight( pDamageInstance->m_IsRight );
    }
}

void CDamage::StraightGuideFrameMove( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile )
{
    // ����ó��
    if( NULL == pDamageInstance || NULL == pDamageInstance->m_pOriginalDamage )
        return;

    // �߻�������� �ӵ���ŭ ������ ��ġ �̵�
    pDamageInstance->m_fAccGuidSpeedX += m_GuidedMissile.m_vGMAccelerator.x;
    pDamageInstance->m_fAccGuidSpeedY += m_GuidedMissile.m_vGMAccelerator.y;
    pDamageInstance->m_X += (GuidedMissile.m_vGMShottingDist.x * (m_x_Speed+pDamageInstance->m_fAccGuidSpeedX));
    pDamageInstance->m_Y += (GuidedMissile.m_vGMShottingDist.y * (m_y_Speed+pDamageInstance->m_fAccGuidSpeedY));

    // Ÿ�� ��ġ���� �״°�?
    if( GuidedMissile.m_bDieAtTargetPos ) { 
        float fOffsetX = pDamageInstance->m_X - pDamageInstance->m_dxvTargetPos.x;
        float fOffsetY = pDamageInstance->m_Y - pDamageInstance->m_dxvTargetPos.y;

        if( fabs( fOffsetX ) <= GuidedMissile.m_dxvDiePosOffset.x || fabs( fOffsetY ) <= GuidedMissile.m_dxvDiePosOffset.y ) { 
            pDamageInstance->m_Life = 2;
        }
    }
}

D3DXVECTOR2 CDamage::GetGuidedTargetPos( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile )
{
    D3DXVECTOR2 vTargetPos(pDamageInstance->m_X, pDamageInstance->m_Y);
    int iIndex = GuidedMissile.m_iGMTargetIndex;
    if( -1 != GuidedMissile.m_iTargetDamageToEnum )
    {
        vTargetPos = g_MyD3D->m_pDamageManager->GetDamageInstancePos( GuidedMissile.m_iTargetDamageToEnum );

        if( -1 != GuidedMissile.m_iNotTargetLife )
        {
            if( 0.0f == vTargetPos.x && 0.0f == vTargetPos.y )
            {
                pDamageInstance->m_Life = GuidedMissile.m_iNotTargetLife;
            }
        }
    }
    else if( GuidedMissile.m_bIsGMTargetMonster )
    {
        MONSTER* pMonster = g_kMonsterManager.GetMonster(iIndex);

        if(pMonster != NULL ) {

            if( (pMonster->m_fHP > 0)   &&
                (pMonster->IsLive() )   )
            {
                float fX, fY;
                GCCollisionRect<float> fAttackRect = pMonster->GetAttackBoundRect( 0 );
                fAttackRect.GetCenter(fX, fY);
                vTargetPos.x = fX + 1.0f;
                vTargetPos.y = fY + 0.25f;
            }
        }
    }
    else
    {
        if( (0 <= iIndex && iIndex < MAX_PLAYER_NUM)    &&
            (g_MyD3D->MyPlayer[ iIndex ]->m_cLife > 0)  &&
            (g_MyD3D->MyPlayer[iIndex]->IsLive())       &&
            (g_MyD3D->MyPlayer[iIndex]->GetPlayerState() != PS_DEAD))
        {
            float fX, fY;
            GCCollisionRect<float> fRect = g_MyD3D->MyPlayer[ iIndex ]->GetCollisionRect();
            fRect.AdjustRect();
            fRect.GetCenter( fX, fY );
            vTargetPos.x = fX + 1.0f;
            vTargetPos.y = fY + 0.25f;
        }
    }

    vTargetPos.x += GuidedMissile.m_vTargetOffset.x;
    vTargetPos.y += GuidedMissile.m_vTargetOffset.y;

    return vTargetPos;
}

bool CDamage::SetGuidedMissileTarget( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile, D3DXVECTOR2 vPos )
{
    bool bSuccessTarget = false;
    GuidedMissile.m_iGMTargetIndex = -1;
    switch( (GuidedMissileCondition)GuidedMissile.m_iGMCondition )
    {
    case GM_TARGET_NEAR_DIST:
        bSuccessTarget = SetGuidedMissileTarget_Dist( pDamageInstance, vPos, GuidedMissile, true );
        break;
    case GM_TARGET_FAR_DIST:
        bSuccessTarget = SetGuidedMissileTarget_Dist( pDamageInstance, vPos, GuidedMissile, false );
        break;
    case GM_TARGET_SMALL_HP:
        bSuccessTarget = SetGuidedMissileTarget_Hp( pDamageInstance, vPos, GuidedMissile, true );
        break;
    case GM_TARGET_LARGE_HP:
        bSuccessTarget = SetGuidedMissileTarget_Hp( pDamageInstance, vPos, GuidedMissile, false );
        break;
    case GM_SAMETEAM_TARGET_NEAR_DIST:
        bSuccessTarget = SetGuidedMissileSameTeamTarget_Dist( pDamageInstance, vPos, GuidedMissile, true );
        break;
    case GM_SAMETEAM_TARGET_FAR_DIST:
        bSuccessTarget = SetGuidedMissileSameTeamTarget_Dist( pDamageInstance, vPos, GuidedMissile, false );
        break;
    case GM_SAMETEAM_TARGET_SMALL_HP:
        bSuccessTarget = SetGuidedMissileSameTeamTarget_Hp( pDamageInstance, vPos, GuidedMissile, true );
        break;
    case GM_SAMETEAM_TARGET_LARGE_HP:
        bSuccessTarget = SetGuidedMissileSameTeamTarget_Hp( pDamageInstance, vPos, GuidedMissile, false );
        break;
    case GM_TARGET_OWNER:
        bSuccessTarget = SetGuidedMissileOwnerTarget( pDamageInstance, vPos, GuidedMissile );
        break;
    case GM_TARGET_NEAR_DAMAGE:
        bSuccessTarget = true;
        break;
    case GM_TARGET_MONSTER_TYPE:
        bSuccessTarget = SetGuidedMissileMonsterTypeTarget( pDamageInstance, GuidedMissile );
        break;
    case GM_TARGET_PLAYER:
        bSuccessTarget = SetGuidedMissilePlayerTarget( pDamageInstance, GuidedMissile );
        break;
    case GM_TARGET_CHARACTER:
        bSuccessTarget = SetGuidedMissileCharTarget( pDamageInstance, vPos, GuidedMissile );
        break;
    }

    return bSuccessTarget;
}

// ����ź�� ������ ���������� �Ѿ�� ������ ���� Ư�������� ���� ���ָ� �����ǵ��� ������.
bool CDamage::CheckOwnderRectInPoint( CDamageInstance* pDamageInstance, GCCollisionRect<float> fRect, D3DXVECTOR2 vPos )
{
    // �������� �����ϵ��� ó��
    //if( GC_GM_DOTA != SiKGCRoomManager()->GetGameMode() )
    //    return true;

    D3DXVECTOR2 vDamageOwnerPos;
    if( pDamageInstance->m_IsMonsterDamage )
    {
        MONSTER *pDamageOwner = g_kMonsterManager.GetMonster( pDamageInstance->m_ExtraMonsterNum );
        if( NULL == pDamageOwner )
            return false;

        vDamageOwnerPos.x = pDamageOwner->GetX();
        vDamageOwnerPos.y = pDamageOwner->GetY();
    }
    else
    {
        PLAYER *pDamageOwner = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ];
        vDamageOwnerPos.x = pDamageOwner->GetPosition().x;
        vDamageOwnerPos.y = pDamageOwner->GetPosition().y;
    }

    fRect.SetRect( fRect.m_Left + vDamageOwnerPos.x, fRect.m_Top + vDamageOwnerPos.y, 
                   fRect.m_Right + vDamageOwnerPos.x, fRect.m_Bottom + vDamageOwnerPos.y );

    return fRect.PtInRect( vPos.x, vPos.y );
}

// 2011.1.15, shmhlove : ����ź TargetSetting(�Ÿ�����������)
#define DISTANCE( x1, y1, x2, y2 ) ASMsqrt( pow( ( x1 - x2 ), 2 ) + pow( ( y1 - y2 ), 2 ) )
bool CDamage::SetGuidedMissileTarget_Dist( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bNear )
{
    float fResult = bNear ? 9999.0f : 0.0f;
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pDamageInstance->m_IsMonsterDamage )
		{
			if( g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, mit->first) )
				continue;
		}
		else
		{
			if( g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first) )
				continue;
		}

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()) ) )
                continue;
        }

		if( pMonster->m_fHP <= 0 )
			continue;

		if( !pMonster->IsLive() )
			continue;

		if( pMonster->IsInvincible() )
			continue;

		const SMonsterTemplate &sMonsterTable = g_kMonsterManager.m_vecMonsterTable[ pMonster->m_iMonsterType ];
		if( sMonsterTable.bNotGuidedTarget )
			continue;

		float fThisDist = DISTANCE( vPos.x, vPos.y, 
			pMonster->GetX(), 
			pMonster->GetY() );

		if( bNear )
		{
			if( fResult >  fThisDist )
			{
				fResult = fThisDist;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}
		else
		{
			if( fResult <  fThisDist )
			{
				fResult = fThisDist;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}
	}

    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( pDamageInstance->m_IsMonsterDamage )
        {
            if( g_MyD3D->IsSameTeam_PlayerAndMonster(iLoop, pDamageInstance->m_ExtraMonsterNum) )
                continue;
        }
        else
        {
            if( g_MyD3D->IsSameTeam(pDamageInstance->m_Who, iLoop) )
                continue;
        }

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(g_MyD3D->MyPlayer[iLoop]->GetPosition().x, g_MyD3D->MyPlayer[iLoop]->GetPosition().y) ) )
                continue;
        }

        if( g_MyD3D->MyPlayer[iLoop]->m_cLife <= 0 )
            continue;

        if( !g_MyD3D->MyPlayer[iLoop]->IsLive() )
            continue;

        if( g_MyD3D->MyPlayer[iLoop]->GetPlayerState() == PS_DEAD )
            continue;

        float fThisDist = DISTANCE( vPos.x, vPos.y,
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().x, 
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().y );

        if( bNear )
        {
            if( fResult >  fThisDist )
            {
                fResult = fThisDist;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
        else
        {
            if( fResult <  fThisDist )
            {
                fResult = fThisDist;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
    }

    return (bNear ? 9999.0f : 0.0f) != fResult;
}

// 2011.1.15, shmhlove : ����ź TargetSetting(����HP������������)
bool CDamage::SetGuidedMissileTarget_Hp( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bSmallHp )
{
    float fResult = bSmallHp ? 99999.0f : 0.0f;
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pDamageInstance->m_IsMonsterDamage )
		{
			if( g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, mit->first) )
				continue;
		}
		else
		{
			if( g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first) )
				continue;
		}

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()) ) )
                continue;
        }

		if( pMonster->m_fHP <= 0 )
			continue;

		if( !pMonster->IsLive() )
			continue;

		if( pMonster->IsInvincible() )
			continue;

		const SMonsterTemplate &sMonsterTable = g_kMonsterManager.m_vecMonsterTable[ pMonster->m_iMonsterType ];
		if( sMonsterTable.bNotGuidedTarget )
			continue;

		if( bSmallHp )
		{
			float fNowHP = pMonster->GetHP();
			float fMaxHP = pMonster->GetMaxHP();
			float fHPRatio = fNowHP/fMaxHP;
			if( fResult > fHPRatio )
			{
				fResult = fHPRatio;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}
		else
		{
			float fNowHP = pMonster->GetHP();
			float fMaxHP = pMonster->GetMaxHP();
			float fHPRatio = fNowHP/fMaxHP;
			if( fResult < fHPRatio )
			{
				fResult = fHPRatio;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}	
	}

    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( pDamageInstance->m_IsMonsterDamage )
        {
            if( g_MyD3D->IsSameTeam_PlayerAndMonster(iLoop, pDamageInstance->m_ExtraMonsterNum) )
                continue;
        }
        else
        {
            if( g_MyD3D->IsSameTeam(pDamageInstance->m_Who, iLoop) )
                continue;
        }

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(g_MyD3D->MyPlayer[iLoop]->GetPosition().x, g_MyD3D->MyPlayer[iLoop]->GetPosition().y) ) )
                continue;
        }

        if( g_MyD3D->MyPlayer[iLoop]->m_cLife <= 0 )
            continue;

        if( !g_MyD3D->MyPlayer[iLoop]->IsLive() )
            continue;

        if( g_MyD3D->MyPlayer[iLoop]->GetPlayerState() == PS_DEAD )
            continue;

        g_MyD3D->MyPlayer[ iLoop ]->GetHP();
        float fThisDist = DISTANCE( vPos.x, vPos.y,
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().x, 
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().y );

        if( bSmallHp )
        {
            float fNowHP = g_MyD3D->MyPlayer[ iLoop ]->GetHP();
            float fMaxHP = g_MyD3D->MyPlayer[ iLoop ]->GetPlayerMaxHP();
            float fHPRatio = fNowHP/fMaxHP;
            if( fResult > fHPRatio )
            {
                fResult = fHPRatio;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
        else
        {
            float fNowHP = g_MyD3D->MyPlayer[ iLoop ]->GetHP();
            float fMaxHP = g_MyD3D->MyPlayer[ iLoop ]->GetPlayerMaxHP();
            float fHPRatio = fNowHP/fMaxHP;
            if( fResult < fHPRatio )
            {
                fResult = fHPRatio;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
    }

    return (bSmallHp ? 99999.0f : 0.0f) != fResult;
}

bool CDamage::SetGuidedMissileSameTeamTarget_Dist( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bNear )
{
    float fResult = bNear ? 9999.0f : 0.0f;
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;
		if( pDamageInstance->m_IsMonsterDamage )
		{
			if( false == g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, mit->first) )
				continue;
		}
		else
		{
			if( false == g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first) )
				continue;
		}

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()) ) )
                continue;
        }

		if( pMonster->m_fHP <= 0 )
			continue;

		if( !pMonster->IsLive() )
			continue;

		const SMonsterTemplate &sMonsterTable = g_kMonsterManager.m_vecMonsterTable[ pMonster->m_iMonsterType ];
		if( sMonsterTable.bNotGuidedTarget )
			continue;

		float fThisDist = DISTANCE( vPos.x, vPos.y, 
			pMonster->GetX(), 
			pMonster->GetY() );

		if( bNear )
		{
			if( fResult >  fThisDist )
			{
				fResult = fThisDist;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}
		else
		{
			if( fResult <  fThisDist )
			{
				fResult = fThisDist;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}
	}

    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( pDamageInstance->m_IsMonsterDamage )
        {
            if( false == g_MyD3D->IsSameTeam_PlayerAndMonster(iLoop, pDamageInstance->m_ExtraMonsterNum) )
                continue;
        }
        else
        {
            if( false == g_MyD3D->IsSameTeam(pDamageInstance->m_Who, iLoop) )
                continue;
        }

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(g_MyD3D->MyPlayer[iLoop]->GetPosition().x, g_MyD3D->MyPlayer[iLoop]->GetPosition().y) ) )
                continue;
        }

        if( g_MyD3D->MyPlayer[iLoop]->m_cLife <= 0 )
            continue;

        if( !g_MyD3D->MyPlayer[iLoop]->IsLive() )
            continue;

        if( g_MyD3D->MyPlayer[iLoop]->GetPlayerState() == PS_DEAD )
            continue;

        float fThisDist = DISTANCE( vPos.x, vPos.y,
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().x, 
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().y );

        if( bNear )
        {
            if( fResult >  fThisDist )
            {
                fResult = fThisDist;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
        else
        {
            if( fResult <  fThisDist )
            {
                fResult = fThisDist;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
    }

    return (bNear ? 9999.0f : 0.0f) != fResult;
}

// 2011.1.15, shmhlove : ����ź TargetSetting(����HP������������)
bool CDamage::SetGuidedMissileSameTeamTarget_Hp( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile, bool bSmallHp )
{
    float fResult = bSmallHp ? 99999.0f : 0.0f;
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER* pMonster = mit->second;

		if( pDamageInstance->m_IsMonsterDamage )
		{
			if( false == g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, mit->first) )
				continue;
		}
		else
		{
			if( false == g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first) )
				continue;
		}

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()) ) )
                continue;
        }

		if( pMonster->m_fHP <= 0 )
			continue;

		if( !pMonster->IsLive() )
			continue;

		const SMonsterTemplate &sMonsterTable = g_kMonsterManager.m_vecMonsterTable[ pMonster->m_iMonsterType ];
		if( sMonsterTable.bNotGuidedTarget )
			continue;

		if( bSmallHp )
		{
			float fNowHP = pMonster->GetHP();
			float fMaxHP = pMonster->GetMaxHP();
			float fHPRatio = fNowHP/fMaxHP;
			if( fResult > fHPRatio )
			{
				fResult = fHPRatio;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}
		else
		{
			float fNowHP = pMonster->GetHP();
			float fMaxHP = pMonster->GetMaxHP();
			float fHPRatio = fNowHP/fMaxHP;
			if( fResult < fHPRatio )
			{
				fResult = fHPRatio;
				GuidedMissile.m_bIsGMTargetMonster = true;
				GuidedMissile.m_iGMTargetIndex = mit->first;
			}
		}

	}

    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( pDamageInstance->m_IsMonsterDamage )
        {
            if( false == g_MyD3D->IsSameTeam_PlayerAndMonster(iLoop, pDamageInstance->m_ExtraMonsterNum) )
                continue;
        }
        else
        {
            if( false == g_MyD3D->IsSameTeam(pDamageInstance->m_Who, iLoop) )
                continue;
        }

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(g_MyD3D->MyPlayer[iLoop]->GetPosition().x, g_MyD3D->MyPlayer[iLoop]->GetPosition().y) ) )
                continue;
        }

        if( g_MyD3D->MyPlayer[iLoop]->m_cLife <= 0 )
            continue;

        if( !g_MyD3D->MyPlayer[iLoop]->IsLive() )
            continue;

        if( g_MyD3D->MyPlayer[iLoop]->GetPlayerState() == PS_DEAD )
            continue;

        g_MyD3D->MyPlayer[ iLoop ]->GetHP();
        float fThisDist = DISTANCE( vPos.x, vPos.y,
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().x, 
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().y );

        if( bSmallHp )
        {
            float fNowHP = g_MyD3D->MyPlayer[ iLoop ]->GetHP();
            float fMaxHP = g_MyD3D->MyPlayer[ iLoop ]->GetPlayerMaxHP();
            float fHPRatio = fNowHP/fMaxHP;
            if( fResult > fHPRatio )
            {
                fResult = fHPRatio;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
        else
        {
            float fNowHP = g_MyD3D->MyPlayer[ iLoop ]->GetHP();
            float fMaxHP = g_MyD3D->MyPlayer[ iLoop ]->GetPlayerMaxHP();
            float fHPRatio = fNowHP/fMaxHP;
            if( fResult < fHPRatio )
            {
                fResult = fHPRatio;
                GuidedMissile.m_bIsGMTargetMonster = false;
                GuidedMissile.m_iGMTargetIndex = iLoop;
            }
        }
    }

    return (bSmallHp ? 99999.0f : 0.0f) != fResult;
}

bool CDamage::SetGuidedMissileOwnerTarget( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile )
{
    bool bResult = false;
    if( pDamageInstance->m_IsMonsterDamage )
    {
        if( g_kMonsterManager.IsMonsterIndex(pDamageInstance->m_ExtraMonsterNum) )
        {
            bResult = true;
            GuidedMissile.m_bIsGMTargetMonster = true;
            GuidedMissile.m_iGMTargetIndex = pDamageInstance->m_ExtraMonsterNum;
        }
    }
    else
    {
        if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_Who) )
        {
            bResult = true;
            GuidedMissile.m_bIsGMTargetMonster = false;
            GuidedMissile.m_iGMTargetIndex = pDamageInstance->m_Who;
        }
    }

    return bResult;
}

bool CDamage::SetGuidedMissileMonsterTypeTarget( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile )
{
    bool bResult = false;

    if( false == g_kMonsterManager.IsMonsterType( GuidedMissile.m_bIsGMTargetMonster ) )
        return bResult;

    for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) 
    {
        MONSTER* pMonster = mit->second;

        if( GuidedMissile.m_iTargetMonsterToType != pMonster->m_iMonsterType )
            continue;

        if( false == pDamageInstance->m_pOriginalDamage->m_bAlwaysEnemy )
        {
            if( pDamageInstance->m_IsMonsterDamage )
            {
                if( g_MyD3D->IsSameTeam_MonsterAndMonster(pDamageInstance->m_ExtraMonsterNum, mit->first) )
                    continue;
            }
            else
            {
                if( g_MyD3D->IsSameTeam_PlayerAndMonster(pDamageInstance->m_Who, mit->first) )
                    continue;
            }
        }

        if( GuidedMissile.m_fCollisionRect.IsSetRect() )
        {
            if( false == CheckOwnderRectInPoint( pDamageInstance, GuidedMissile.m_fCollisionRect, D3DXVECTOR2(pMonster->GetX(), pMonster->GetY()) ) )
                continue;
        }

        if( pMonster->m_fHP <= 0 )
            continue;

        if( !pMonster->IsLive() )
            continue;

        if( pMonster->IsInvincible() )
            continue;

        const SMonsterTemplate &sMonsterTable = g_kMonsterManager.m_vecMonsterTable[ pMonster->m_iMonsterType ];
        if( sMonsterTable.bNotGuidedTarget )
            continue;

        GuidedMissile.m_bIsGMTargetMonster = true;
        GuidedMissile.m_iGMTargetIndex = mit->first;
        bResult = true;
        break;
    }

    return bResult;
}

bool CDamage::SetGuidedMissilePlayerTarget( CDamageInstance* pDamageInstance, GUIDEDMISSILE &GuidedMissile )
{
    if( -1 == GuidedMissile.m_iTargetPlayerToID )
        return false;

    if( false == g_MyD3D->IsPlayerIndex(GuidedMissile.m_iTargetPlayerToID) )
        return false;

    PLAYER *pPlayer = g_MyD3D->MyPlayer[ GuidedMissile.m_iTargetPlayerToID ];
    if( NULL == pPlayer )
        return false;

    if( false == pDamageInstance->m_pOriginalDamage->m_bAlwaysEnemy )
    {
        if( pDamageInstance->m_IsMonsterDamage )
        {
            if( g_MyD3D->IsSameTeam_PlayerAndMonster(pPlayer->m_iPlayerIndex, pDamageInstance->m_ExtraMonsterNum) )
                return false;
        }
        else
        {
            if( g_MyD3D->IsSameTeam(pDamageInstance->m_Who, pPlayer->m_iPlayerIndex) )
                return false;
        }
    }

    if( pPlayer->m_cLife <= 0 )
        return false;

    if( !pPlayer->IsLive() )
        return false;

    if( pPlayer->GetPlayerState() == PS_DEAD )
        return false;

    GuidedMissile.m_bIsGMTargetMonster = false;
    GuidedMissile.m_iGMTargetIndex = pPlayer->m_iPlayerIndex;

    return true;
}

bool CDamage::SetGuidedMissileCharTarget( CDamageInstance* pDamageInstance, D3DXVECTOR2 &vPos, GUIDEDMISSILE &GuidedMissile )
{
    if( -1 == GuidedMissile.m_iTargetCharToType )
        return false;

    float fResult = 9999.0f;
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( GuidedMissile.m_iTargetCharToType != g_MyD3D->MyPlayer[iLoop]->GetCurrentChar().iCharType )
            continue;

        if( false == pDamageInstance->m_pOriginalDamage->m_bAlwaysEnemy )
        {
            if( pDamageInstance->m_IsMonsterDamage )
            {
                if( g_MyD3D->IsSameTeam_PlayerAndMonster(iLoop, pDamageInstance->m_ExtraMonsterNum) )
                    continue;
            }
            else
            {
                if( g_MyD3D->IsSameTeam(pDamageInstance->m_Who, iLoop) )
                    continue;
            }
        }

        if( g_MyD3D->MyPlayer[iLoop]->m_cLife <= 0 )
            continue;

        if( !g_MyD3D->MyPlayer[iLoop]->IsLive() )
            continue;

        if( g_MyD3D->MyPlayer[iLoop]->GetPlayerState() == PS_DEAD )
            continue;

        float fThisDist = DISTANCE( vPos.x, vPos.y,
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().x, 
            g_MyD3D->MyPlayer[ iLoop ]->GetPosition().y );

        if( fResult >  fThisDist )
        {
            fResult = fThisDist;
            GuidedMissile.m_bIsGMTargetMonster = false;
            GuidedMissile.m_iGMTargetIndex = iLoop;
        }
    }

    return (9999.0f != fResult);
}

void CDamage::ValkyrieFrameMove( CDamageInstance* pDamageInstance )
{
    // D3DXVec3CatmullRom() �Լ��� �Ἥ ��� �����ϰڵ�.
    // D3DXVec3CatmullRom() �Լ����� pOut���� ������ ���� vecMileStone[1]�� vecMileStone[2] ������ �����̴�.
    // ���� �� ������ ���� ���ϴٰ� pOut�� ���� vecMileStone[2]�� ���� ���� �� ��, 
    // ���� ������ ���� ���ϱ� ���ؼ� ������ �ϳ��� �ű��� ���Ѵ�.
    // �ᱹ �������� �̵��ϴ� ������ vecMileStone[1] => vecMileStone[2] => vecMileStone[3]���� �̵��Ѵ�.
    // vecMileStone[0]�� vecMileStone[1]���� vecMileStone[2]�� �Ѿ�� ������ ���� ������ ���ϴµ� ����. �Ի簢 ������ �������� ��
    // vecMileStone[0] == vecMileStone[1] �� ���� [1]�� [2] ���� ������ ������ ���� ���̴�.

    //////////////////////////////////////////////////////////////////////////
    // �� ���̴� �����̷� ����....
    /*D3DXVECTOR2 vecPos = g_pkGameOverlayUI->GetHeadUp()->m_pkExpBar->m_pkExpPercent->GetFixedWindowPos();
    D3DXVECTOR3& CameraPos = D3DXVECTOR3( g_kCamera.m_fLookX + 1.0f, g_kCamera.m_fLookY + 0.75f, 0.0f );
    // ī�޶� ���� �ִ� ���� �˾Ƴ���. ���� ����Ǿ� �ִ�.
    float fWidth = g_kCamera.m_rtCamera.right - g_kCamera.m_rtCamera.left;//2.0f;//800.0f;
    float fHeight = g_kCamera.m_rtCamera.top - g_kCamera.m_rtCamera.bottom;//1.5f;//600.0f;
    // cw = ī�޶� ī�޶� �������� �󸶳� �������°�
    float cwx = CameraPos.x - fWidth * 0.5f;
    float cwy = CameraPos.y - fHeight * 0.5f;

    float fRealPosX = cwx + (vecPos.x / 400.0f) + 0.15f;
    float fRealPosY = cwy + (1.5f - vecPos.y / 400.0f) + 0.05f;*/
    //////////////////////////////////////////////////////////////////////////

    D3DXVECTOR3 pOut;
    // �츮�� ���� �� �ִ� ������ ���� ���� - 2 �����̴�.
    int iSection = m_vecMileStone.size() - 2;

    if ( m_Start_Life == pDamageInstance->m_Life )
    {
        m_iLifeMax = m_Start_Life;
        m_fS = 0.0f;
        m_iIndex = 0;

        // ��� ��ǥ�̸� ~ �������� ��ǥ �����ָ� ���� �� ;;
        if ( !m_bOnUI )
        {
            std::vector<D3DXVECTOR3>::iterator vit = m_vecMileStone.begin();
            while ( vit != m_vecMileStone.end() )
            {
                float fMasterPosX, fMasterPosY, fOffsetX, fOffsetY;
                fOffsetX = (*vit).x;
                fOffsetY = (*vit).y;

                if ( pDamageInstance->m_IsMonsterDamage )
                {
					MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
					if(pMonster != NULL ) {
						fMasterPosX = pMonster->GetX();
						fMasterPosY = pMonster->GetY();
					}
                }
                else
                {
                    fMasterPosX = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().x;
                    fMasterPosY = g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetPosition().y;
                }

                (*vit).x = fMasterPosX + ( fOffsetX * (pDamageInstance->m_IsRight ? 1.0f : -1.0f ) );
                (*vit).y = fMasterPosY + fOffsetY;
                ++vit;
            }
        }
    }

    // �ѱ��� ������? �� ���� ���� ������ �� 
    if ( m_fS >= 1.0f )
    {
        // �ε��� �ϳ� �÷��� �� 4���� �����ִ�?
        if ( m_iIndex + 1 + 3 < (int)m_vecMileStone.size() )
        {
            ++m_iIndex;
        }
        // �ȳ��� ������,, �׳� �ϳ��� ����;;
        else
        {
            m_vecMileStone[m_iIndex] = m_vecMileStone[m_iIndex + 1];
            m_vecMileStone[m_iIndex + 1] = m_vecMileStone[m_iIndex + 2];
            m_vecMileStone[m_iIndex + 2] = m_vecMileStone[m_iIndex + 3];
        }

		m_fS = (1.0f) / (float)(m_iLifeMax / iSection);
    }

    D3DXVec3CatmullRom( &pOut, &m_vecMileStone[m_iIndex], &m_vecMileStone[m_iIndex + 1], &m_vecMileStone[m_iIndex + 2], &m_vecMileStone[m_iIndex + 3], m_fS);
    // �ѱ����� ���� �ӵ��� �����ϴ�... �ӵ��� ������ �ϰ� �ʹٸ�,, ���� ���̸� �÷��� ���� �ƴϸ� �ڵ��� ������ ��
    m_fS += (1.0f) / (float)(m_iLifeMax / iSection);

    pDamageInstance->m_X = pOut.x;
    pDamageInstance->m_Y = pOut.y;
}

void CDamage::SetTraceCreator( CDamageInstance* pDamageInstance )
{
    // �ִϸ��̼��� �÷��̾� ��ġ�� �׻� ����ٳ�� �� ���
    // ��ġ�� ���� ����ش�
    float fX = m_Start_Locate_x + m_fAnimStartPosX;
    float fY = m_Start_Locate_y + m_fAnimStartPosY;

	KGCObj*		pTraceObj = NULL;
    bool bDestroyDamage = false;

	if ( m_dwDMAttribute & DA_TRACE_CREATOR )
	{
		//pTraceObj = g_MyD3D->MyObj[pDamageInstance->GetUnitedWho()];
		if( pDamageInstance->m_IsMonsterDamage == false ){
			pTraceObj = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ];
		}
		else{
			MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
			if(pMonster != NULL ) {
				pTraceObj = pMonster;
			}
		}
	}
	else
	{
        switch( pDamageInstance->m_emHitWhoType )
		{
        case OBJ_TYPE_PLAYER:
			if( g_MyD3D->IsPlayerIndex(pDamageInstance->m_HitWho) )
			{
				pTraceObj = g_MyD3D->MyPlayer[pDamageInstance->m_HitWho];
                if( (false == g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->IsLive()) || 
                    (false == g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->IsFatal() &&
                    (0 >= g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetHP())) )
			{
                    bDestroyDamage = true;
			}	
		}
            break;
        case OBJ_TYPE_MONSTER:
			MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_HitWho);
			if(pMonster != NULL ) {
				pTraceObj = pMonster;
                if( false == pMonster->IsLive() || 
                    0 >= pMonster->GetHP() )
                {
                    bDestroyDamage = true;
                }
			}
            break;
			}
		}

    if( bDestroyDamage )
        pDamageInstance->m_Life = 1;

	if ( !pTraceObj )
		return;

	D3DXVECTOR3 vTracePos;

    if ( pDamageInstance->m_IsMonsterDamage )
    {
        if( pDamageInstance->m_pOriginalDamage->m_iTracePosType == -1 )
        {
            vTracePos = pTraceObj->vRenderPos;
            if( pDamageInstance->m_pOriginalDamage->m_bMotionTrace )
            {
                vTracePos.y += pTraceObj->y_Ex;
            }
        }
        else
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
			if(pMonster != NULL ) {
				D3DXVECTOR3 pvPos = pMonster->GetBonePos( pDamageInstance->m_pOriginalDamage->m_iTracePosType );

				float xOffset = pDamageInstance->m_pOriginalDamage->m_Start_Locate_x;
				if ( false == pDamageInstance->m_IsRight )
				{
					xOffset *= -1.0f;
				}
				vTracePos = pvPos + D3DXVECTOR3( -1.0f, -0.5f, 0.0f ) + D3DXVECTOR3( xOffset, pDamageInstance->m_pOriginalDamage->m_Start_Locate_y, 0.0f);
			}
        }
    }
    else
    {
        if( pDamageInstance->m_pOriginalDamage->m_iTracePosType == -1 || pTraceObj->GetPosDesc() == NULL)
        {
            if( pDamageInstance->m_pOriginalDamage->m_bTraceMonsterBoundBox && pDamageInstance->m_emHitWhoType == OBJ_TYPE_MONSTER ) { 
                GCCollisionRect<float> boundBox = static_cast<MONSTER*>(pTraceObj)->GetAttackBoundRect(0);
                vTracePos.x = boundBox.m_Left + ( ( boundBox.m_Right - boundBox.m_Left )/2.0f );
                vTracePos.y = boundBox.m_Bottom;
                vTracePos.z = 0.5f;
            }
            else { 
                vTracePos = pTraceObj->vRenderPos;
            }
            if( pDamageInstance->m_pOriginalDamage->m_bMotionTrace )
            {
                vTracePos.y += pTraceObj->y_Ex;
            }
        }
        else
        {
            D3DXVECTOR3* pvPos = ComputePosDesc( *pTraceObj->GetPosDesc(), pDamageInstance->m_pOriginalDamage->m_iTracePosType );
            vTracePos = *pvPos;
        }
    }

    if( pDamageInstance->m_bPetTrace ) {
        KPet* pkPet = g_MyD3D->m_akPet[pDamageInstance->m_Who];
        D3DXVECTOR2* pvPos = pkPet->GetPos();
        pDamageInstance->m_X = pvPos->x;
        pDamageInstance->m_Y = pvPos->y + pTraceObj->y_Ex;;
        return;
    }
    if( pDamageInstance->m_bPlayerTrace ) {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[pDamageInstance->m_Who];
        D3DXVECTOR2 pvPos = pPlayer->GetXYPosition();
        pDamageInstance->m_X = pvPos.x;
        pDamageInstance->m_Y = pvPos.y + pTraceObj->y_Ex;;
        return;
    }

	if( pDamageInstance->m_pOriginalDamage->m_bMotionXTrace )
	{
		if( pTraceObj->bIsRight)
			vTracePos.x += fX;
		else
			vTracePos.x -= fX;
	}
	if( pDamageInstance->m_pOriginalDamage->m_bMotionYTrace )
		vTracePos.y += fY;

    if( pDamageInstance->m_pOriginalDamage->m_bTraceMonsterBoundBox && pDamageInstance->m_emHitWhoType == OBJ_TYPE_MONSTER ) { 
        vTracePos.x += 1.0f;
        vTracePos.y += 0.25f;
    }
    else { 
        vTracePos += (D3DXVECTOR3)pTraceObj->vParticleToDamageMagicNumber;
    }

    if ( m_dwDMAttribute & DA_TRACE_CREATOR )
    {
        vTracePos.x += pDamageInstance->m_IsRight ? pDamageInstance->m_pOriginalDamage->m_fTraceCreateOffsetX : -pDamageInstance->m_pOriginalDamage->m_fTraceCreateOffsetX;
        vTracePos.y += pDamageInstance->m_pOriginalDamage->m_fTraceCreateOffsetY;
    }

	if( pDamageInstance->m_pOriginalDamage->m_bPlayerXTrace ||
		pDamageInstance->m_pOriginalDamage->m_Start_Life == pDamageInstance->m_Life )
		pDamageInstance->m_X = vTracePos.x;
	
	if( pDamageInstance->m_pOriginalDamage->m_bPlayerYTrace || 
		pDamageInstance->m_pOriginalDamage->m_Start_Life == pDamageInstance->m_Life )
		pDamageInstance->m_Y = vTracePos.y;

}

CDamage& CDamage::operator = (const CDamage& damage)
{
    m_Start_Life            =    damage.m_Start_Life;        
    m_Life_Change            =    damage.m_Life_Change;       
    m_Reverse_Life          =   damage.m_Reverse_Life;

    m_Start_Scale_x            =    damage.m_Start_Scale_x;     
    m_Scale_Speed            =    damage.m_Scale_Speed;       
    m_Y_per_x                =    damage.m_Y_per_x;           

    m_Start_Locate_x        =    damage.m_Start_Locate_x;    
    m_Start_Locate_y        =    damage.m_Start_Locate_y;    
    m_Start_Locate_Random_x[0]=    damage.m_Start_Locate_Random_x[0];
    m_Start_Locate_Random_x[1]=    damage.m_Start_Locate_Random_x[1];
    m_Start_Locate_Random_y[0]=    damage.m_Start_Locate_Random_y[0];
    m_Start_Locate_Random_y[1]=    damage.m_Start_Locate_Random_y[1];

    m_Start_Locate_Frame_x[0]=    damage.m_Start_Locate_Frame_x[0];
    m_Start_Locate_Frame_x[1]=    damage.m_Start_Locate_Frame_x[1];
    m_Start_Locate_Frame_y[0]=    damage.m_Start_Locate_Frame_y[0];
    m_Start_Locate_Frame_y[1]=    damage.m_Start_Locate_Frame_y[1];

    m_fAnimStartPosX        =    damage.m_fAnimStartPosX;     
    m_fAnimStartPosY        =    damage.m_fAnimStartPosY;     

    m_x_Speed                =    damage.m_x_Speed;           
    m_y_Speed                =    damage.m_y_Speed;           
    m_x_CrashSpeed           =    damage.m_x_CrashSpeed;
    m_y_CrashSpeed           =    damage.m_y_CrashSpeed;
    m_fDamageCrashSpeedX     =    damage.m_fDamageCrashSpeedX;
    m_fDamageCrashSpeedY     =    damage.m_fDamageCrashSpeedY;
    m_x_DecSpeed             =    damage.m_x_DecSpeed;
    m_y_DecSpeed             =    damage.m_y_DecSpeed;

    m_Start_Angle            =    damage.m_Start_Angle;       
    m_Angle_Speed            =    damage.m_Angle_Speed;       
    m_fLimite_Angle          =    damage.m_fLimite_Angle;
    m_Texture                =    damage.m_Texture;           
    m_TextureNoRenderCnt     =    damage.m_TextureNoRenderCnt;

    m_bIsCorruptedDamage     =    damage.m_bIsCorruptedDamage;

    m_fDamageToPlayer        =    damage.m_fDamageToPlayer;   
    m_fDamageToMonster       =    damage.m_fDamageToMonster;
    m_bIgnoreDefDamage       =    damage.m_bIgnoreDefDamage;   
    m_bIgnoreSuperDamage     =    damage.m_bIgnoreSuperDamage;
	m_fMpChange				 =	  damage.m_fMpChange;
    m_bTraceMonsterBoundBox  =    damage.m_bTraceMonsterBoundBox;
    m_bDeleteSync            =    damage.m_bDeleteSync;
    m_bNoCrashFatal          =    damage.m_bNoCrashFatal;    
    m_iIgnoreGroundCheckHittedObject = damage.m_iIgnoreGroundCheckHittedObject; 

    m_IsEnableDamage        =    damage.m_IsEnableDamage;    
    m_IsStaticDamageCleaner  =   damage.m_IsStaticDamageCleaner;
    m_IsStaticDamage         =   damage.m_IsStaticDamage;
    m_IsAngleMove            =    damage.m_IsAngleMove;       
    m_IsRealTimeAngleMove    =    damage.m_IsRealTimeAngleMove;
    m_IsAngleMoveWithOffset  =    damage.m_IsAngleMoveWithOffset;
    
    m_IsFixedAngleMove      =   damage.m_IsFixedAngleMove;

    m_iDamageMotion            =    damage.m_iDamageMotion;
    m_fDMXSpeed                =    damage.m_fDMXSpeed;    
    m_fDMYSpeed                =    damage.m_fDMYSpeed;    
    m_bNoLocalMotion           =    damage.m_bNoLocalMotion;

    m_dwDMAttribute            =    damage.m_dwDMAttribute;
    m_iCrashSound            =    damage.m_iCrashSound;    

    m_iAPMagicEffect        =   damage.m_iAPMagicEffect;
    m_iMagicEffect            =    damage.m_iMagicEffect;    
    m_iUnMagicEffect        = damage.m_iUnMagicEffect;
    m_iNoCrashUnMagicEffect = damage.m_iNoCrashUnMagicEffect;
    m_fMagicEffTime            =    damage.m_fMagicEffTime;
    m_iMagicLevel           =   damage.m_iMagicLevel;
	m_iMonsterMagicEff        =   damage.m_iMonsterMagicEff;
    m_vecRandMagicEffect    =   damage.m_vecRandMagicEffect;
    m_bUniqueMgicEff        =   damage.m_bUniqueMgicEff;
    m_bFastHostMagicEff     =   damage.m_bFastHostMagicEff;
	
    m_iEarthQuake            =    damage.m_iEarthQuake;    
    m_iEarthQuakeY           =    damage.m_iEarthQuakeY;
    m_iEarthQuakeRunFrame    =    damage.m_iEarthQuakeRunFrame;

    m_iCrashStart        =    damage.m_iCrashStart;
    m_iCrashEnd            =    damage.m_iCrashEnd;

    m_GuidedMissile          = damage.m_GuidedMissile;

    m_particleSeqName        =    damage.m_particleSeqName;
    m_vecSpark                =   damage.m_vecSpark;
    m_EmitMin                =    damage.m_EmitMin;
    m_EmitMax                =    damage.m_EmitMax;
    m_bAddNoDirection        =    damage.m_bAddNoDirection;

    m_AnimSeqName            =    damage.m_AnimSeqName;
    m_bAnimLoop                =    damage.m_bAnimLoop;
    m_fAnim_Start_ScaleX    =    damage.m_fAnim_Start_ScaleX;
    m_fAnim_Start_ScaleY    =    damage.m_fAnim_Start_ScaleY;
    m_fAnimScaleX_Speed        =    damage.m_fAnimScaleX_Speed;
    m_fAnimScaleY_Speed        =    damage.m_fAnimScaleY_Speed;
    m_fAnim_Limit_ScaleX    =    damage.m_fAnim_Limit_ScaleX;
    m_fAnim_Limit_ScaleY    =    damage.m_fAnim_Limit_ScaleY;

    m_fAnimAngle            =    damage.m_fAnimAngle;

    m_iParent                =    damage.m_iParent;
    m_iReserveParent         =    damage.m_iReserveParent;
    m_bDirection            =    damage.m_bDirection;
    m_SRCBlendMode            =    damage.m_SRCBlendMode;
    m_DESTBlendMode            =    damage.m_DESTBlendMode;

    m_iJumpPossibleFrame    =   damage.m_iJumpPossibleFrame;

    m_fStoneMaxDmg = damage.m_fStoneMaxDmg;
    m_fStoneMinDmg = damage.m_fStoneMinDmg;
    m_fDecreasePerPush = damage.m_fDecreasePerPush;

    m_vPos           = damage.m_vPos;
    m_bParticleTrace = damage.m_bParticleTrace;
    m_bParticleAngleMove = damage.m_bParticleAngleMove;

    m_bMotionTrace   = damage.m_bMotionTrace;
	m_bMotionXTrace   = damage.m_bMotionXTrace;
	m_bMotionYTrace   = damage.m_bMotionYTrace;

    m_iStopMotionTraceLife   = damage.m_iStopMotionTraceLife;

    m_bPlayerXTrace = damage.m_bPlayerXTrace;
    m_bPlayerYTrace = damage.m_bPlayerYTrace;

    m_iAttackType           = damage.m_iAttackType;
	m_iAttackTypeOnAerial   = damage.m_iAttackTypeOnAerial;
    m_iAttackDirection      = damage.m_iAttackDirection;
    m_fPushTargetX          = damage.m_fPushTargetX;
    m_fPushTargetY          = damage.m_fPushTargetY;
    m_bMonSuperDamage       = damage.m_bMonSuperDamage;
    m_bSpecialAttack        = damage.m_bSpecialAttack;
    m_bSlotUniqueSkillDmg   = damage.m_bSlotUniqueSkillDmg;
    m_bSpecial4Damage       = damage.m_bSpecial4Damage;
    m_nSpecialGrade         = damage.m_nSpecialGrade;

    m_iPlusDelay = damage.m_iPlusDelay;

    m_iTracePosType = damage.m_iTracePosType;

    m_iCannotCatchFrame = damage.m_iCannotCatchFrame;

    m_vecChildParticle = damage.m_vecChildParticle;
    m_vecDamageHpParticle = damage.m_vecDamageHpParticle;
    m_vecInToggleMeshParticle = damage.m_vecInToggleMeshParticle;
    m_vecOutToggleMeshParticle = damage.m_vecOutToggleMeshParticle;
    m_vecInToggleSummonMonsterMeshParticle = damage.m_vecInToggleSummonMonsterMeshParticle;
    m_vecOutToggleSummonMonsterMeshParticle = damage.m_vecOutToggleSummonMonsterMeshParticle;
    m_vecMPDamageParticle = damage.m_vecMPDamageParticle;

    m_iNoCheckContact = damage.m_iNoCheckContact;

	m_iNoCheckContactDamage = damage.m_iNoCheckContactDamage;

    m_MeshInfo = damage.m_MeshInfo;

    m_vecCrashParticle = damage.m_vecCrashParticle;

	m_vecAutoDamage = damage.m_vecAutoDamage;
    m_vecTargetAutoDamage = damage.m_vecTargetAutoDamage;

	m_vecTimeSound = damage.m_vecTimeSound;

    m_vecTrigger = damage.m_vecTrigger;

	m_bIsContinousDamage = damage.m_bIsContinousDamage;
	m_fContinousDamage = damage.m_fContinousDamage;

	m_bNoPassDamage = damage.m_bNoPassDamage;
	m_iDamageHP = damage.m_iDamageHP;
    m_vecPermissionDamageHP = damage.m_vecPermissionDamageHP;
	m_iHitPerFrame = damage.m_iHitPerFrame;
	m_bIsTotalRange = damage.m_bIsTotalRange;
    m_bIsGroundBottomBox = damage.m_bIsGroundBottomBox;

	m_iSlowCount = damage.m_iSlowCount;

	m_bUserFrameDependant = damage.m_bUserFrameDependant;
	m_bUserMotionDependant = damage.m_bUserMotionDependant;

    m_fGravity = damage.m_fGravity;
    m_bAccGravity = damage.m_bAccGravity;
    m_bNoAlphaChange = damage.m_bNoAlphaChange;
    m_bStopAtContact = damage.m_bStopAtContact;
    m_bEndAtContact = damage.m_bEndAtContact;
    m_bAlwaysEnemy = damage.m_bAlwaysEnemy;
    m_bAttackToSameTeam = damage.m_bAttackToSameTeam;
    m_bCheckBoundary = damage.m_bCheckBoundary;
    m_bCreateCheckBoundary = damage.m_bCreateCheckBoundary;
    m_bEndAtLastBottomContact = damage.m_bEndAtLastBottomContact;
    m_bStopAtLastBottomContact = damage.m_bStopAtLastBottomContact;

    m_fLeechHP = damage.m_fLeechHP;
	m_fLeechMP = damage.m_fLeechMP;
	m_fAbsorbHP = damage.m_fAbsorbHP;
    m_fAbsorbHPToMaxHP = damage.m_fAbsorbHPToMaxHP;
    m_fAbsorbHPToDamage = damage.m_fAbsorbHPToDamage;
    m_iAddAbsorbHPPerHit = damage.m_iAddAbsorbHPPerHit;
    m_bOwnerDirection = damage.m_bOwnerDirection;

	m_iMaxInstanceNum = damage.m_iMaxInstanceNum;

    m_vecMileStone = damage.m_vecMileStone;
    m_iIndex = damage.m_iIndex;
    m_iLifeMax = damage.m_iLifeMax;
    m_fS = damage.m_fS;
    m_bOnUI = damage.m_bOnUI;
	m_bApplyDamageSpeedByDamageDirection = damage.m_bApplyDamageSpeedByDamageDirection;

    m_iHaste = damage.m_iHaste;

    m_iFatalRatio = damage.m_iFatalRatio;

    m_bToggleMesh = damage.m_bToggleMesh;
    m_iToggleSummonMonsterID = damage.m_iToggleSummonMonsterID;
    m_iDamageToTeleport = damage.m_iDamageToTeleport;
    m_bIgnoreSpecialMonDamageToTeleport = damage.m_bIgnoreSpecialMonDamageToTeleport;
    m_bIgnoreChampionProperty = damage.m_bIgnoreChampionProperty;
    m_bShieldBreak = damage.m_bShieldBreak;

    m_uiNotSpecialSuper = damage.m_uiNotSpecialSuper;

    m_iNextAttackCount = damage.m_iNextAttackCount;

    m_kOnlyMotionDamage = damage.m_kOnlyMotionDamage;

    m_kRestrictHitCnt = damage.m_kRestrictHitCnt;

    m_kVariousIncreaseRatio = damage.m_kVariousIncreaseRatio;

    m_setDamageFlag = damage.m_setDamageFlag;

    m_bOwnerKillDamage = damage.m_bOwnerKillDamage;
    
	
    m_bGasDamage = damage.m_bGasDamage;
    m_bIsCharLock = damage.m_bIsCharLock;

    m_kSummonMonInfo = damage.m_kSummonMonInfo;
    m_kMoveCreateToCrashPos = damage.m_kMoveCreateToCrashPos;
    m_kMoveCreateToUnitPos = damage.m_kMoveCreateToUnitPos;

    m_iWhoStartMotion = damage.m_iWhoStartMotion;	

    m_fTraceCreateOffsetX = damage.m_fTraceCreateOffsetX;
    m_fTraceCreateOffsetY = damage.m_fTraceCreateOffsetY;
   
    m_pairSkipInfo = damage.m_pairSkipInfo;
    m_kSyncDamagePosAtCrashPos = damage.m_kSyncDamagePosAtCrashPos;

    m_iUniqueDamage = damage.m_iUniqueDamage;

    m_bCheckNoPassFloor = damage.m_bCheckNoPassFloor;
    m_bMonTargettingRelease = damage.m_bMonTargettingRelease;

    m_vecPassBuff = damage.m_vecPassBuff;
    m_vecPassNoBuff = damage.m_vecPassNoBuff;

    m_bNoCrashPlayer = damage.m_bNoCrashPlayer;
    m_bNoCrashMonster = damage.m_bNoCrashMonster;
    m_bNoCrashDamage = damage.m_bNoCrashDamage;
    m_bNoCrashGameObject = damage.m_bNoCrashGameObject;
    m_bNoCrash2DObject = damage.m_bNoCrash2DObject;

    m_fGround_Offset_X = damage.m_fGround_Offset_X;
    m_fGround_Offset_Y = damage.m_fGround_Offset_Y;

    m_bOnlyOneUnit = damage.m_bOnlyOneUnit;
    m_iStopUnitFrame = damage.m_iStopUnitFrame;

    m_bTogetherDie = damage.m_bTogetherDie;
    m_bNoPushSpeedToFatal = damage.m_bNoPushSpeedToFatal;
    m_fSameTeamDamageToMonster = damage.m_fSameTeamDamageToMonster;

    m_kMonReaction = damage.m_kMonReaction;
    m_mapNoCreateExceptionGameMode = damage.m_mapNoCreateExceptionGameMode;
    m_kDamageToDamage = damage.m_kDamageToDamage;

    m_vecTogetherDieDamage = damage.m_vecTogetherDieDamage;

    m_bAutoDamageToGroundCrashOfNoDieCurDamage = damage.m_bAutoDamageToGroundCrashOfNoDieCurDamage;
    m_vecCrashDamageToMonsterType = damage.m_vecCrashDamageToMonsterType;

    m_kDivisionDamage = damage.m_kDivisionDamage;
    m_kBoundaryReflection = damage.m_kBoundaryReflection;

    m_vecTogetherDieBuff = damage.m_vecTogetherDieBuff;

    m_bNotComboBreak = damage.m_bNotComboBreak;
    return *this;
}

void CDamage::GetDamageRate( IN OUT float& fRate, MONSTER* pMonster, PLAYER* pPlayer, CDamageInstance* pDamage )
{
    // ���� ��������
    switch( pDamage->m_What )
    {
    case DT_PUMPKINHEAD_MISSILE:
        {
            if( true == pPlayer->m_bPumpkinDefence )
            {
                fRate *= 0.7f;
            }
        }
        break;
    case DT_MONSTER_ELENA01:
    case DT_ARME4_SPECIAL3_METEO_READY:
    case DT_ARME4_CRITICAL_DAMAGE:
    case DT_MONSTER_KAMIKI09:
    case DT_MONSTER_KAMIKI10:
    case DT_MONSTER_KAMIKI08:
    case DT_MONSTER_KAMIKI06:
    case DT_FIREBALL_DARK:
    case DT_MONSTER_KAMIKI01:
    case DT_MONSTER_KAMIKI03:
    case DT_MONSTER_KAMIKI05:
    case DT_ARME4_SPECIAL3_BLACK:
    case DT_ARME4_SPECIAL3_BLACK5:
    case DT_ARME4_SPECIAL3_BLACK_BOOM1:
    case DT_ARME4_SPECIAL3_BLACK_BOOM2:
    case DT_ARME4_SPECIAL3_BLACK_BOOM3:
    case DT_ARME4_SPECIAL3_BLACK_BOOM4:
    case DT_ARME4_SPECIAL3_BLACK_BOOM5:
    case DT_ARME4_SPECIAL3_BLACK_BOOM6:
    case DT_ARME4_SPECIAL3_BLACK_BOOM7:
    case DT_ARME4_SPECIAL3_BLACK_BOOM8:
    case DT_LIGHTNING_BOLT:
    case DT_LIGHTNING_GENERATE1:
	case DT_ARME_SKILL_SHOCKSTURN_LV2_ATK1:
    case DT_HERO_KAMIKI_ICEFIELD:
    case DT_EXTINCTION_MON_KAMIKI_PHASE1_ICEFIELD:
    case DT_EXTINCTION_MON_KAMIKI_PHASE2_ICEFIELD:
    case DT_ARME_SKILL_ICESTRIKE_ICEFIELD:
        {
            if( true == pPlayer->m_bKamikiDefence && 
                ( MON_ELENA == pMonster->m_iMonsterType || MON_KAMIKI == pMonster->m_iMonsterType ) )
            {
                fRate *= 0.5f;
            }
        }
        break;
    case DT_MONSTER_KAZEAZE2_02:
    case DT_MONSTER_KAZEAZE2_01:
    case DT_KAZEAZE_FIRE:
    case DT_MONSTER_KAZEAZE01_01:
    case DT_MONSTER_KAZEAZE01_02:
    case DT_MONSTER_KAZEAZE01_03:
    case DT_IMPACT_SMASH:
    case DT_MONSTER_KAZEAZE04_01:
    case DT_MONSTER_KAZEAZE04_02:
    case DT_MONSTER_KAZEAZE04_03:
    case DT_MONSTER_KAZEAZE05:
    case DT_MONSTER_KAZEAZE03:
    case DT_MONSTER_KAZEAZE02:
    case DT_KAZEAZE_DARKBALL:
    case DT_KAZEAZE_DARKBALL_CHILD:
        {
            if( true == pPlayer->m_bKazeazeDefence && 
                ( MON_KAZEAZE == pMonster->m_iMonsterType || MON_KAZEAZE2 == pMonster->m_iMonsterType ) )
            {
                fRate *= 0.5f;
            }
        }
        break;
    case DT_MONSTER_KAMIKI07:
        {
            if( true == pPlayer->m_bKamikiDefence && 
                ( MON_ELENA == pMonster->m_iMonsterType || MON_KAMIKI == pMonster->m_iMonsterType ) )
            {
                fRate *= 0.5f;
            }
            else if( true == pPlayer->m_bKazeazeDefence && 
                ( MON_KAZEAZE == pMonster->m_iMonsterType || MON_KAZEAZE2 == pMonster->m_iMonsterType ) )
            {
                fRate *= 0.5f;
            }
        }
        break;
    default:
        break;
    }
}

void CDamage::CheckTrigger( CDamageInstance* pDamageInstance )
{
    for( unsigned int i = 0; i < pDamageInstance->m_pOriginalDamage->m_vecTrigger.size(); ++i )
    {
        DAMAGE_TRIGGER& trigger = pDamageInstance->m_pOriginalDamage->m_vecTrigger[i];
        if( trigger.bActivated )
            continue;

        switch( trigger.TriggerType )
        {
        case DAMAGE_TRIGGER::TRIGGER_TIME:
            {
                if( trigger.val1 <= pDamageInstance->m_Life && pDamageInstance->m_Life <= trigger.val2 )
                    g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i);
            }
            break;
        case DAMAGE_TRIGGER::TRIGGER_OWNER_MOTION:
            {
                if( trigger.val1 == g_MyD3D->MyPlayer[pDamageInstance->m_Who]->uiMotion )
                {
                    if( trigger.val2 == g_MyD3D->MyPlayer[pDamageInstance->m_Who]->cFrame )
                    {
                        g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i);
                    }
                }
            }
        case DAMAGE_TRIGGER::TRIGGER_DAMAGE_MOTION:
            {
                if( !pDamageInstance->m_p3DObject )
                    continue;

                if( trigger.val1 == pDamageInstance->m_p3DObject->GetMotion() )
                {
                    if( trigger.val2 == pDamageInstance->m_p3DObject->GetFrame() )
                    {
                        g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i);
                    }
                }
            }
            break;
        case DAMAGE_TRIGGER::TRIGGER_OWNER_DIE:
            {

                if( pDamageInstance->m_IsMonsterDamage )
                {
					MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
					if(pMonster != NULL ) {
						if( pMonster->GetHP() <= 0.0f )
							pDamageInstance->m_Life = 1;
					}
                }
                else
                {
                    if( g_MyD3D->MyPlayer[pDamageInstance->m_Who]->GetHP() <= 0.0f )
                        pDamageInstance->m_Life = 1;
                }
            }
            break;
        case DAMAGE_TRIGGER::TRIGGER_CRASH:
            {
                if( pDamageInstance->GetCrashCheck() )
                {
                    g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i );
                }               
            }
            break;
        case DAMAGE_TRIGGER::TRIGGER_HITTED_DIE:
            {
                if( OBJ_TYPE_MONSTER == pDamageInstance->m_emHitWhoType)
                {
					MONSTER *pMonster =  g_kMonsterManager.GetMonster(pDamageInstance->m_HitWho);
                    if( pMonster != NULL && (pMonster->GetHP() <= 0.0f ||
                        pMonster->IsDie() ||
                        false == pMonster->IsLive()) )
                    {
                        pDamageInstance->m_Life = 1;
                        g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i);
                    }
                }
                if( OBJ_TYPE_PLAYER == pDamageInstance->m_emHitWhoType && g_MyD3D->IsPlayerIndex(pDamageInstance->m_HitWho) )
                {
                    if( g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->GetHP() <= 0.0f || 
                        g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->IsDie() ||
                        false == g_MyD3D->MyPlayer[pDamageInstance->m_HitWho]->IsLive() )
                    {
                        pDamageInstance->m_Life = 1;
                        g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i);
                    }
                }
            }
            break;
        case DAMAGE_TRIGGER::TRIGGER_MONSTER_MOTION:
            {
                if ( pDamageInstance->m_IsMonsterDamage )
                {
					MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
					if(pMonster != NULL ) {
						if( pMonster->CheckState(const_cast<char*>(trigger.strVal1.c_str())) )
						{
							int iIndex = LATENCY_INDEX;
							if( !g_kGlobalValue.m_kUserInfo.bHost )
								iIndex = 0;

							if ( trigger.val1 >= pMonster->m_aiFrame[iIndex] )
							{
								g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i);
							}
						}
					}
                }
            }
            break;
        }
    }
}

void CDamage::OnGroundCrash( CDamageInstance* pDamageInstance, bool bCrash_ )
{
	for( unsigned int i = 0; i < pDamageInstance->m_pOriginalDamage->m_vecTrigger.size(); ++i )
	{
		DAMAGE_TRIGGER& trigger = pDamageInstance->m_pOriginalDamage->m_vecTrigger[i];
		if( !trigger.bNoCheckActive && trigger.bActivated )
			continue;

        int iTriggerType = bCrash_ ? DAMAGE_TRIGGER::TRIGGER_GROUND_CRASH : DAMAGE_TRIGGER::TRIGGER_GROUND_CRASH_NOT;
        if ( trigger.TriggerType == iTriggerType )
        {
            g_MyD3D->m_pDamageManager->ActivateTrigger( pDamageInstance, i);
        }
	}
}

void CDamage::SetMileStonePos( float fX_, float fY_ )
{
    // ������ �߿��ϳ�... 
    // ù ���ؽ��� �Ի簢 ���� 
    // �ι�° ���ʹ� �������� ������ ���̿��� ����
    m_vecMileStone.push_back(D3DXVECTOR3(fX_, fY_, 1.0f));
}

void CDamage::SummonMonster( CDamageInstance* pDamageInstance, int iHitPlayerIdx )
{
    if( 0 > m_kSummonMonInfo.m_iMonsterID || m_kSummonMonInfo.m_iMonsterID >= MONSTER_MODEL_NUM )
        return;

    if( -1 != m_kSummonMonInfo.m_iLimiteLiveMonsterCnt )
    {
        if( m_kSummonMonInfo.m_iLimiteLiveMonsterCnt < g_kMonsterManager.GetLiveMonsterNum() )
            return;
    }

    // ���Ͱ� ������
    if( pDamageInstance->m_IsMonsterDamage )
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(pDamageInstance->m_ExtraMonsterNum);
		if(pMonster == NULL ) return;

        if( g_kGlobalValue.m_kUserInfo.bHost == false )
            return;

        KSummonInfo kSummonInfo;
        kSummonInfo.Init();
        kSummonInfo.SetState(m_kSummonMonInfo.m_szState);
        kSummonInfo.iMonType = m_kSummonMonInfo.m_iMonsterID;
        kSummonInfo.fX = pDamageInstance->m_X + m_kSummonMonInfo.m_fOffsetX;
        kSummonInfo.fY = pDamageInstance->m_Y + m_kSummonMonInfo.m_fOffsetY;
		if(pMonster != NULL ) {
			pMonster->SummonDamageMonster( &kSummonInfo, m_kSummonMonInfo.m_iLevel, iHitPlayerIdx, pDamageInstance->m_pOriginalDamage->m_kSummonMonInfo.m_bIsLock );
		}

    }

    // �÷��̾ ������(�÷��̾ ����, �÷��̾ �÷��̾ ������ �� )
    else
    {
        if( false == g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
            return;

        if( false == g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->IsLocalPlayer() )
            return;

        KSummonInfo kSummonInfo;
        kSummonInfo.Init();
        kSummonInfo.SetState(m_kSummonMonInfo.m_szState);
        kSummonInfo.iMonType = m_kSummonMonInfo.m_iMonsterID;
        kSummonInfo.fX = pDamageInstance->m_X + m_kSummonMonInfo.m_fOffsetX;
        kSummonInfo.fY = pDamageInstance->m_Y + m_kSummonMonInfo.m_fOffsetY;
        g_MyD3D->MyPlayer[ pDamageInstance->m_Who ]->SummonDamageMonster( &kSummonInfo, m_kSummonMonInfo.m_iLevel, 1000.0f, 15, iHitPlayerIdx );
    }
}

void CDamage::CheckOnlyMotionDamage( CDamageInstance* pDamageInstance )
{
    if( 0 >= int(m_kOnlyMotionDamage.m_vecCondition.size()) )
        return;

    if( false == g_MyD3D->IsPlayerIndex( pDamageInstance->m_Who ) )
        return;

    bool bResult = false;
    PLAYER *pPlayer = g_MyD3D->MyPlayer[ pDamageInstance->m_Who ];
    std::vector< std::pair<int, bool> >::iterator vecIter = m_kOnlyMotionDamage.m_vecCondition.begin();
    for(; vecIter != m_kOnlyMotionDamage.m_vecCondition.end(); ++vecIter)
    {
        if( (vecIter->first == pPlayer->uiMotion) == vecIter->second )
        {
            bResult = true;
            if( 1 == m_kOnlyMotionDamage.m_iOperation ) // OR���� : �ϳ��� TRUE�� TRUE
                break;
        }
        else if( 0 == m_kOnlyMotionDamage.m_iOperation )// AND���� : �ϳ��� FALSE�� FALSE
        {
            bResult = false;
            break;
        }
    }

    if( bResult )
    {
        if( m_kOnlyMotionDamage.m_iTrueToLife )
            pDamageInstance->m_Life = m_kOnlyMotionDamage.m_iTrueToLife;
    }
    else
        pDamageInstance->m_Life = m_kOnlyMotionDamage.m_iFalseToLife;
}

bool CDamage::IsExistDamageFlag( int iFlag_ )
{
    std::set<int>::iterator sit =  m_setDamageFlag.find(iFlag_);

    if( sit != m_setDamageFlag.end() ) {
        return true;
    } else {
        return false;
    }
}

CParticleEventSeqPTR CDamageInstance::CreateSequence( const std::string& name, float x, float y, float z )
{
    CParticleEventSeqPTR seq;
    seq = g_ParticleManager->CreateSequence( name, x, y, z );

#if defined(RESOURCE_GRAPH)
	if(seq)
	{
		char tmp[256];	
		sprintf( tmp,"DAMAGE_%d",m_What );	
		g_pGCDeviceManager2->AddRelation( "PARTICLE_" + name, tmp );
	}	
#endif
	return seq;
}

Effect* CDamageInstance::Add( int What, float x, float y, int Delay, float Scale /*= 1.0f*/, float Target_x /*= -100.0f*/, float Target_y /*= -100.0f*/, float Angle /*= 0.0f*/, bool bIsRight /*= false*/, int who /*= -1 */ )
{
#if defined(RESOURCE_GRAPH)
	char tmp1[256];	
	char tmp2[256];	
	sprintf(tmp1,"SPARK_%d",What);	
	sprintf(tmp2,"DAMAGE_%d",m_What);
	g_pGCDeviceManager2->AddRelation(tmp1,tmp2);
#endif

	return g_MyD3D->MySparks->Add(What,x,y,Delay,Scale,Target_x,Target_y,Angle,bIsRight,who);
}

int CDamageInstance::GetUnitedWho()
{
	if ( m_IsMonsterDamage || m_Who >= MAX_PLAYER_NUM )
	{
		return m_ExtraMonsterNum + MAX_PLAYER_NUM;
	}
	return m_Who;
}

void CDamageInstance::CountHitPerFrame()
{
    if( 0 >= (--m_iHitPerFrameCnt) )
    {
        m_iHitPerFrameCnt = m_pOriginalDamage->GetHitPerFrame();

        // HitPerFrame�� ���þȵȻ��¶�� Cnt����
        // Original�� �ٸ� ������ ���� ���־�� �Ѵ�.
        if( 0 == m_iHitPerFrameCnt )
            m_iHitPerFrameCnt = -1;
    }
}

bool CDamageInstance::IsHitPerFrame()
{
    if( 1 >= m_pOriginalDamage->GetHitPerFrame() ||
        m_pOriginalDamage->GetHitPerFrame() == m_iHitPerFrameCnt )
    {
        return true;
    }

    return false;
}

bool CDamageInstance::IsHitFrame()
{
    if( m_pOriginalDamage->GetHitPerFrame() == m_iHitPerFrameCnt )
    {
        return true;
    }

    return false;
}

void CDamageInstance::CheckSummonMonster()
{
    // ���� ��ȯ( LifeTime���� ��ȯ( �浹�ÿ��� Crash�Լ����� ��ȯ�� ) )
    if( -1 < m_pOriginalDamage->m_kSummonMonInfo.m_iMonsterID )
    {
        if( m_Life == m_pOriginalDamage->m_kSummonMonInfo.m_iSummonLifeTime )
            m_pOriginalDamage->SummonMonster( this );
    }

    // ��ȯ�� ���� ���°˻�
    std::vector<int>::iterator vecIter = m_vecSummonMonHitWho.begin();
    for(; vecIter != m_vecSummonMonHitWho.end(); )
    {
        if( g_MyD3D->IsPlayerIndex( (*vecIter) ) && m_pOriginalDamage->m_kSummonMonInfo.m_bIsLock )
        {
            PLAYER *pPlayer = g_MyD3D->MyPlayer[(*vecIter)];
            int iMonIdx = pPlayer->GetDamageSummonMonSlot();
			MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonIdx);
			if(pMonster == NULL ) return;

            // ��ȯ�� ���Ͱ� ��� ������
            if( pMonster->IsLive() && pMonster->GetHP() > 0.0f )
            {
                // ĳ���� ��ġ�� ��ȯ�� ���� ��ġ�� ���� �� ������ Lock
                pPlayer->vPos = D3DXVECTOR3( pMonster->GetX(), pMonster->GetY(), pPlayer->vPos.z );
                pPlayer->m_bFrameLock = true;
                pPlayer->m_bCrashCheck = false;
            }

            // ��ȯ�� ���Ͱ� �׾����� ó��
            if( g_kGlobalValue.m_kUserInfo.bHost )
            {
				if( pMonster->m_iSummoned != MONSTER::MEMS_NOT_SUMMON_MONSTER &&
                    (false == pMonster->IsLive() || 0.0f >= pMonster->GetHP()) )
                {
                    KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END kPacket;
                    kPacket.iHitPlayerIdx = (*vecIter);
                    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
                    KGCPC_MEMO( "KGC_PID_BROAD_SUMMON_DAMAGEMONSTER_DIE_END" );

                    vecIter = m_vecSummonMonHitWho.erase( vecIter );
                    continue;
                }
            }
            ++vecIter;
        }
    }
}

bool CDamageInstance::InsertAlreadyDamaged( int iIndex )
{
	if(m_setAlreadyDamage.insert(iIndex).second == false && m_pOriginalDamage->m_dwDMAttribute & DA_ONEHIT_DAMAGE )
		return false;
	return true;
}

bool CDamageInstance::CheckAlreadyDamaged( int iIndex )
{
	return m_setAlreadyDamage.find(iIndex) != m_setAlreadyDamage.end();
}

void CDamageInstance::EraseAlreadyDamaged( int iIndex )
{
	if ( iIndex == -1 ) {
		m_setAlreadyDamage.clear();
	}

	m_setAlreadyDamage.erase(iIndex);
}
