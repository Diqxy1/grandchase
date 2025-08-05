#include "stdafx.h"
#include "QuestGameManager.h"
#include "DamageManager.h"
#include "Spark.h"
#include "Message.h"
#include "Headup Display.h"
#include "Monster.h"
#include "Stage.h"
#include "Buff/KGCBuffManager.h"
#include "KGCGameBoard.h"
#include "GCCameraOrtho.h"
#include "GCO2Manager.h"
#include "MapSystem/GCFantasticMap.h"
#include "OpenProcess.h"
#include "GCDropItemManager.h"
#include "KGCRoomManager.h"
#include "KGCPlayerCommon.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCOnGameExpBar.h"
#include "GCUI/GCHeadupDisplay.h"
#include "GCEventManager.h"
#include "HardDamage/HardDamageArrow.h"
#include "HardDamage/HardDamageCriticalArrow.h"
#include "GCRenderManager.h"
#include "KGCBlurManager.h"
#include "HardDamage/HardDamagePeriot.h"
#include "KTDGFrustum.h"
#include "KGC3DObject.h"
#include "KGC3DObjectMgr.h"
#include "KGCQALogManager.h"
#include "GCUI/GCAlertInDungeon.h"
#include "GCUI/GCAddtionGuide.h"
#include "GCPetMagicStone.h"

// 두 점 사이의 거리를 구한다!
//
#define DISTANCE( x1, y1, x2, y2 ) ASMsqrt( pow( ( x1 - x2 ), 2 ) + pow( ( y1 - y2 ), 2 ) )\

#define DEFAULT_MONSTER_SCALE_SIZE (0.5f)
#define MAX_ATTACK_ADVANTAGE_RATIO (3.0f)

// 그랜드체이스의 중력임..
#define GRAVITY (0.0015f)

// 몬스터의 현재 상태를 보여준다
#if defined( _DEBUG ) || !defined( __PATH__ )
    #define AI_HELPER
#endif

// 자세한 에러 내용을 메세지 박스로 띄움
#if defined( _DEBUG ) || !defined( __PATH__ )
    #define ERROR_MESSAGE_BOX
#endif

boost::pool<> MONSTER::bpool(sizeof(MONSTER));

DWORD MONSTER::m_adwChampionPropertyList[] =
{
    CP_ATTACK,
    CP_DEFENCE,
    CP_HP_RECOVERY,
    CP_SUPER_ARMOR,
    CP_FAST_STAND,
    CP_ATTACK_CURES,
    CP_ATTACK_STONE,
    CP_ATTACK_ICE,
    CP_ARROW_DEFENCE,
    CP_SHOCKWAVE,	
};

int MONSTER::m_aiChampionPropertyString[] =
{
    STR_ID_CHAMPION_PROPERTY01,
    STR_ID_CHAMPION_PROPERTY02,
    STR_ID_CHAMPION_PROPERTY03,
    STR_ID_CHAMPION_PROPERTY04,
    STR_ID_CHAMPION_PROPERTY05,
    STR_ID_CHAMPION_PROPERTY06,
    STR_ID_CHAMPION_PROPERTY07,
    STR_ID_CHAMPION_PROPERTY08,
    STR_ID_CHAMPION_PROPERTY09,
    STR_ID_CHAMPION_PROPERTY10,
};

MONSTER::MONSTER()
{
	m_iMonsterIndex = 0;
    m_dwTargetingDelayTime     = 0;

    // 몬스터 모델
    m_pObject    = NULL;
    m_pkMonsterModelDead = NULL;

    // 미션 아이콘
    m_MissionIcon.RemoveAllTexture();

    // 카미키가 사용하는 라이트닝
    for( int i = 0; i < KAMIKI_LIGHTNING_COUNT; ++i )
        m_apKamikiLightning[i] = NULL;

    m_iTriggerID        = -1;

    m_pkChampionEffect  = NULL;
	m_pkFuryEffect = NULL;

    m_iShockSturnCount = 0;

    m_iLastAttackedBoxIndex = 0;
	vParticleToDamageMagicNumber = D3DXVECTOR2( 1.0f, 0.50f );

	m_vHeadPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    m_kChangeMon.Init();
    Init();
}

void MONSTER::Init()
{
	m_iPreMotionFrame = -1;
	m_iPreIndex =0;

    m_bMovableMonster       = true;
    m_bLive                 = false;
    m_bDie                  = true;
    m_iMonsterType          = 0; // 고르고스, 하피 등등..
    m_fSpeedX               = 0.0f;
    m_fSpeedY               = 0.0f;
    m_bIsContact            = true;
    m_bOldIsContact         = true;
    m_iFlashEffectFrame     = 0;
    m_iShakeEffectFrame     = 0;
    m_bFrameLock            = false;
    m_bBuffFrameLock        = false;
    m_iNoCheckContact       = 0;
    m_bResult               = false;
    m_iTargetPlayerIndex    = -1;
    m_dwTargetDamage        = 0;
    m_dwTargetSyncObject    = 0;
    m_iSuperArmor           = 0;
    m_bIsSuperArmor         = false;
    m_bAttackSuperArmor     = false;
    m_iInvincible           = 0;
    m_bIsInvincible         = false;
	m_bIsInvincibleNoFrame  = false;
    m_iDownDelay            = 0;
    m_fHP                   = 0.0f;
    m_fCumulativeDamage     = 0.0f;
    m_fOldHP                = 0.0f;
    m_eNameSize             = SMALL_SIZE;
    m_iCountValue           = 0;
    m_iTimerFrame			= 0;
    m_bProcessTimer			= false;
    m_bAttackCheck          = true;
    m_iRemainFrame          = 0;
    m_iRemainRender         = 0;
    m_bIsFly                = false;
    m_fContactHeight        = 0.0f;
    m_iMonsterNameID        = 0;
    m_iNameBone				= -1;
    m_fNameOffset_X         = 0.0f;
    m_fNameOffset_Y         = 0.0f;
    m_bIsFallDownMotion     = false;
    m_bIgnoreFHCheck        = false;
    m_iLastAttackedMe       = -1;
    m_iKamikiLightningIndex = 0;
    Body_Angle            = 0.0f;
    m_bMovement             = true;
    m_iLatencySlowCount     = 0;
    m_iSlowCount            = 0;
    m_iTeam                 = ETMonster;
    m_pDropItemObserver     = NULL;
    m_bRender               = true;
    m_dwChampionProperty    = 0x00000000;
    m_iHPRecoveryCount      = 0;
    m_iMonsterKind          = EMON_NORMAL;

    m_iTargetMonsterIndex   = -1;
    m_bMonsterTarget        = true;
    m_bNotMissionCount      = false;

    m_vecMotionList.clear();
    m_mapReaction.clear();
    m_strMonsterName.clear();
    m_vecSummonMonsterType.clear();
    m_vecDamageAnim.clear();

    m_fReserveValue = 0.0f;

    m_fAtkRatio = 1.0f;
    m_fDefRatio = 1.0f;
    m_fSDefRatio = 1.0f;
    m_bAggroState = false;
    m_bIsAttacked = false;

    m_bTrace = false;
    m_iTraceindex = -1;
    m_iTraceBone = -1;
    m_fTraceGapX = 0.0f;
    m_fTraceGapY = 0.0f;
    m_fPercentLength = 0.02f;

    m_bBlur = false;
    m_dwBlurStartTime = 0;
    m_vecHitPos = D3DXVECTOR2( 0.0f, 0.0f );
    m_eNPC      = NPC_NONE;
    m_MissionIcon.RemoveAllTexture();
    m_kChangeMon.Init();
    m_nSpecial = -1; // KEventMonster::OPT_NONE KEventMonster::OPT_BOSS 이런거

    m_vecShieldEffect.clear();
    m_vecShieldEffectArg.clear();

	m_fMonsterNameWidth = 0.0f;
    m_iSummoned = MEMS_NOT_SUMMON_MONSTER;
    m_dwOwnerUID = 10000;
    m_bToggleInfo = true;
    m_iNoTargettingFrame = 0;
    m_iSummonBuffIcon = 0;
    m_iRecvStateFromPlayer = 0;
    m_bSummonMonsterDieType = false;
    m_bMeshToggleState = false;
    m_bImitSummon = false;
    m_bDotaGateIsRight = false;

    m_iCharType = GC_CHAR_INVALID;
    m_iAgitMonCharUID = -1;
    m_iMentCount = -1;
    m_vecItemIDList.clear();
    m_kEscortStopWatch.Init();

    m_bIsSpecialAttack = false;
    m_fDamageDiff = 0.0f;

	m_fHeadPosY = -1.0f;
	m_fHeadPosX = 0.0f;	
	
    m_bInPlayerRangeCheck = false;
    m_bIsMyExistScreen = false;
    m_bShowEscortHPBar = false;
    m_bNoChangeState = false;

    m_vTempTargetPos = D3DXVECTOR2(0.0f, 0.0f);
    m_dwBlurEndTime = 0;

    m_fAddWalkSpeed = 0.0f;
    m_fAddRunSpeed = 0.0f;

    m_bNoCrashDamage = false;
    m_bHPrenderSkip = false;
	m_iSummonsNuberAvailable = 15;
    m_iStopSpeedFrame = 0;
    m_vTempValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_mapTempValue.clear();
    m_iSelectTextrue = -1;
    m_bNonShakeEffect = false;
    m_mapUserBoneAngle.clear();
    m_kRealMiniMapScaleAniInfo.Init();
    m_kRealMiniMapAlphaAniInfo.Init();
    m_kRealMiniMapColorAniInfo.Init();
    m_kRealMiniMapAccTexInfo.Init();
    m_fRealMiniMapStaticScale = 1.0f;

    m_iUserCartoon = -1;
    m_vUserCartonEdgeColor = D3DXVECTOR4(-1.0f, -1.0f, -1.0f, -1.0f);
    m_bNotCheckStateWall = false;

    fBossDieCameraZoom = 15.0f;
    dwBossDieBlurTime = 3000;
    m_bCallDieFucn = false;
	m_bNotBackAttack = false;
    m_iDamageWhat = -1;
    m_iBaseRender = -1;
    m_vecPermissionDamage.clear();
    m_dwMissAttackTime = 0;

    m_bUnlimitDef = false;

    m_vDamageStartPos = D3DXVECTOR2(0.0f, 0.0f);
    m_vDamageWhoPos = D3DXVECTOR2(0.0f, 0.0f);

    m_bCollisionCheck = false;

}

MONSTER::~MONSTER()
{
    Release();
}

//필요한 리소스를 로딩하고 능력치 셋팅!
bool MONSTER::StartMonster( EGCMonster eMonsterType, int iLevel /*= -1*/, DWORD dwChampionProperty /*= 0x00000000*/, int iBoss /*= false*/, bool bUseItemList /*= false*/ )
{
    if( g_kMonsterManager.m_vecMonsterTable.size() <= (UINT)eMonsterType )
    {
        #if defined( ERROR_MESSAGE_BOX )
        {
            std::wstringstream strText;
            std::wstringstream strCaption;
            strText << L"( " << eMonsterType << L" ) 잘못된 몬스터 타입입니다.";
            strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
            MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
        }
        #endif

        return false;
    }

    // StartMonster하면 반드시 게임 끝날때 Release를 해야되지만..
    // StartMonster만 연속으로 불릴 경우.. 릴리즈부터 하고.. 새로 만들자..
    Release();

    // 몬스터 타입 설정
    m_iMonsterType = (int)eMonsterType;

    // 챔피언 몬스터 정보
    m_dwChampionProperty = dwChampionProperty;

    // 보스 몬스터인가?
    m_iBossMonster = iBoss;

    // 몬스터에게 드랍 옵저버 셋팅
    SiKGCDropItemManager()->SetObserverToMonster( m_iTriggerID, m_iMonsterIndex, m_iMonsterType );

    // 우선 모델과 모션 리소스를 해제합니다.
    EndAfterImage();
    SAFE_REMOVE_DELETE( m_pObject );
    SAFE_REMOVE_DELETE( m_pkMonsterModelDead );

    // 파티클 모델 초기화
    m_bInitAI = true;
    m_iCurParticleModel = -1;
    m_vecParticleModel.clear();
    m_vecChildMeshParticle.clear();
    m_vecDieToClearParticle.clear();

    for( int i = 0; i < (int)m_vecMotionList.size(); ++i )
    {
        SAFE_RELEASE( m_vecMotionList[i] );
    }
    m_vecMotionList.clear();

    bool bBoss = ( m_iBossMonster == MON_BOSS );

    if ( iLevel != -1 ) 
        SiKGCWorldMapManager()->GetClearMonsterLevel( iLevel );

    // 몬스터 스탯 설정
    g_kGlobalValue.SetMonsterStat( m_iMonsterType, iLevel, g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ], bBoss );
    // 몬스터 템플릿의 정보를 가져옴
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];

    // 챔피언 속성을 무시해버린다.
    if( kMonsterTemplate.bNoChampion )
        m_dwChampionProperty = 0x00000000;

    m_pObject = g_RenderManager->CreateObject(kMonsterTemplate.iTechnique);
    m_pkMonsterModelDead = g_RenderManager->CreateObject(kMonsterTemplate.iTechnique);

    if( true == bUseItemList ) {
        std::vector< EXTRA_MESH > vecMeshInfo;
        MakeItemListMeshInfo( vecMeshInfo );
        LoadExtraMesh( vecMeshInfo );        
    }
    else {
	LoadExtraMesh( kMonsterTemplate.vecMeshInfo );
    }
	std::swap( m_pObject, m_pkMonsterModelDead );
	LoadExtraMesh( kMonsterTemplate.vecMeshDeadInfo );
	std::swap( m_pObject, m_pkMonsterModelDead );

    m_pObject->SetShellMatrixScale( DEFAULT_MONSTER_SCALE_SIZE );
    m_pkMonsterModelDead->SetShellMatrixScale( DEFAULT_MONSTER_SCALE_SIZE );

    // 사운드 로드
    for( int i = 0 ; i < (int)kMonsterTemplate.m_vecSounds.size() ; ++i )
    {
        g_KDSound.PreparetoLoad( kMonsterTemplate.m_vecSounds[i] );
        g_KDSound.LoadFromIndex( kMonsterTemplate.m_vecSounds[i] );
    }

    //m_pkMonsterModel->m_strDebugInfo = kMonsterTemplate.vecMeshInfo[i].strMeshName;
    m_pObject->AddTexture( g_pGCDeviceManager2->GetNullTexture());
    m_pObject->SetMeshComputePaletteInterface( (IGCComputePaletteP3M*)this );
    m_pObject->SetCenterBone( kMonsterTemplate.iCenterBone );

    m_pkMonsterModelDead->SetMeshComputePaletteInterface( NULL );

    m_pObject->SetVolume( kMonsterTemplate.fVolume );
    m_pObject->SetRender( false );

    m_pkMonsterModelDead->SetVolume( kMonsterTemplate.fVolume );
    m_pkMonsterModelDead->SetRender( false );

    // 쉴드 초기화~ 
    for ( unsigned int i = 0; i < m_vecShieldEffect.size(); ++i )
    {
        if ( m_vecShieldEffect[i] != NULL )
        {
            g_ParticleManager->DeleteSequence( m_vecShieldEffect[i] );
            m_vecShieldEffect[i] = NULL;
        }
    }
    
    m_vecShieldEffect.clear();
    m_vecShieldEffectArg.clear();

	g_kMonsterManager.LoadOneMonsterActionScript( m_iMonsterType );


    // 모션 로드
    std::map< int, std::string > mapMotionIndex = g_kMonsterManager.GetMonsterActionIndex( m_iMonsterType );
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    std::map< int, std::string >::iterator mapMotionIter = mapMotionIndex.begin();
    for(; mapMotionIter != mapMotionIndex.end(); ++mapMotionIter)
    {
        std::map< std::string, KGCMonsterAction >::iterator mapActionIter = mapAction.find( mapMotionIter->second.c_str() );
        KGCMonsterAction &kMonsterAction = mapActionIter->second;
        GCDeviceMotionFRM* pFrm = g_pGCDeviceManager2->CreateMotionFRM( kMonsterAction.m_strMotion );

        if( pFrm == NULL || pFrm->GetDeviceState() != GCDS_LOADED || pFrm->IsDisable() )
        {
#if defined( ERROR_MESSAGE_BOX )
            {
                std::wstringstream strText;
                std::wstringstream strCaption;
                strText << L"( " << m_strMonsterName << L" " << kMonsterAction.m_strMotion.c_str() << L" ) 파일이 존재하지 않음";
                strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
                MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
            }
#endif

#if !defined(__PATH__)
            {
                CreateDirectoryA( "./FileErrorLog", NULL );

                time_t curTime = GCUTIL_TIME::GetCurTime();                    

                // 한데 모은 로그파일
                std::wstringstream strText;
                strText << GCUTIL_TIME::GetYear(curTime) << "_" << GCUTIL_TIME::GetMonth(curTime) << "_" << GCUTIL_TIME::GetDay(curTime) << " | "
                    << GCUTIL_TIME::GetHour(curTime) << ":" << GCUTIL_TIME::GetMinute(curTime) << ":" << GCUTIL_TIME::GetSecond(curTime) << " | "
                    << L"에러난 함수 위치 : " << __FUNCTION__ << " | "
                    << L"( " << m_strMonsterName << L" " << kMonsterAction.m_strMotion.c_str() << L" ) 파일이 존재하지 않음" << "\n";

                FILE *fo = NULL;
                fo = fopen("FileErrorLog\\FileErrorLogList.txt","a");
                fwprintf(fo,L"%s",strText.str().c_str());
                fclose(fo);
            }
#endif
            return false;
        }

        m_vecMotionList.push_back( pFrm );
    }

    // 정보들을 저장한다
    if( iLevel == -1 ) // 스크립트의 레벨로 설정하자
        m_iLevel            = kMonsterTemplate.iBaseLevel;
    else if(iLevel!=-2) // 요때는 그냥 둔다 	
        m_iLevel            = iLevel; // 몬스터 레벨

    //m_vecPosDesc.clear();

    //for ( int i = 0; i < kMonsterTemplate.m_vecParticleInfo.size(); i++ )
    //{
    //    MonsterParticlePosDesc* pDesc = new MonsterParticlePosDesc();
    //    *pDesc = kMonsterTemplate.m_vecParticleInfo[i];
    //    m_vecPosDesc.push_back( pDesc );

    //}

    m_fScale                = kMonsterTemplate.fScale;
    m_fWeight               = kMonsterTemplate.fWeight;
    m_fWalkSpeed            = kMonsterTemplate.fWalkSpeed;
    m_fRunSpeed             = kMonsterTemplate.fRunSpeed;
    m_fAttackBoundBoxWidth  = kMonsterTemplate.fAttackBoundBoxWidth;
    m_fAttackBoundBoxHeight = kMonsterTemplate.fAttackBoundBoxHeight;
    m_fWidth                = kMonsterTemplate.fWidth;
    m_fHeight               = kMonsterTemplate.fHeight;
	m_fHeadPosY				= kMonsterTemplate.fHeadPosY;
	m_fHeadPosX				= kMonsterTemplate.fHeadPosX;
    m_eNPC                  = (EGCMonNPCType)kMonsterTemplate.eNPC;
    m_fShadowRatio          = kMonsterTemplate.fShadowRatio;
    m_eNameSize             = (EGCNAMESIZE)kMonsterTemplate.eNameSize;
    m_fAttackPoint          = kMonsterTemplate.fAttackPoint;
    m_fSummonsAtkRatio      = kMonsterTemplate.fSummonsAtkRatio;    
    m_fHPPoint              = kMonsterTemplate.fHPPoint;
    m_iMonsterNameID        = kMonsterTemplate.iNameID;
    m_iNameBone				= kMonsterTemplate.iNameBone;
    m_fNameOffset_X         = kMonsterTemplate.fNameOffset_X;
    m_fNameOffset_Y         = kMonsterTemplate.fNameOffset_Y;
    m_strMonsterName        = g_pkStrLoader->GetString( m_iMonsterNameID );
    m_fVolume               = kMonsterTemplate.fVolume;
    m_fZPos                 = kMonsterTemplate.fZPos;
    m_bMonsterTarget        = kMonsterTemplate.bMonsterTarget;
    m_bNotMissionCount      = kMonsterTemplate.bNotMissionCount;
	m_bNotBackAttack		= kMonsterTemplate.bNotBackAttack;
	
    m_bNoCrashDamage        = kMonsterTemplate.bNoCrashDamage;
    m_bNotCheckStateWall    = kMonsterTemplate.bNotCheckStageWall;
    m_bHPrenderSkip         = kMonsterTemplate.bHPrenderSkip;

    m_fOwnerAtkRatio        = -1.f;


    if ( m_fHPPoint < 0 )
    {
#if defined( ERROR_MESSAGE_BOX )
        {
            std::wstringstream strText;
            std::wstringstream strCaption;
            strText << L"( " << m_strMonsterName << L" HP : " << m_fHPPoint << L" ) HP가 0보다 작음 BaseLevel 의심!";
            strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
            MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
        }
#endif
    }

    // 챔피언 몬스터가 되면 생명력이 2배가 된다
    if( m_dwChampionProperty != 0x00000000 ) // 어떠한 속성이 있다면 챔피언 몬스터이다!
    {
        m_fHPPoint *= g_kGlobalValue.GetChampionAbility(static_cast<int>(ECHAMPION_VITALITY_RATE));

        if( m_dwChampionProperty & CP_ATTACK ) // 공격력 상승
            m_fAttackPoint *= g_kGlobalValue.GetChampionAbility(static_cast<int>(ECHAMPION_ATTACK_RATE));

        // 챔피언 몬스터 이펙트 파티클..
        m_pkChampionEffect = g_ParticleManager->CreateSequence( "Champion_monster", 0.0f, 0.0f, 0.0f );

        if (m_pkChampionEffect){
            m_pkChampionEffect->SetShow( &m_bRender );

            TRACE_INFO stTrace;
            stTrace.m_pvPos = &vRenderPos;
            stTrace.m_fParticleTraceTime = 1.0f;
            m_pkChampionEffect->SetTrace( &stTrace );

        }
        //Sistema de Icones do Heróico
        SetChampionBuffIcon( m_iMonsterIndex );
    }

    // 이런 일은 없어야되지만.. m_fAttackPoint와 m_fHPPoint의 최소값은 보장해준다..
    if( m_fAttackPoint < 0.0f )
        m_fAttackPoint = 0.1f;
    if( m_fHPPoint < 1.0f )
        m_fHPPoint = 1.0f;

	m_fHP = m_fOldHP = m_fHPPoint; // 최소값 보장해 준 후 넣자

    // [10/5/2007] breadceo.초기화 함수 부르기 전에
    m_vecSummonMonsterType.clear();

    // 별도의 세팅이 필요한 몬스터들은 여기에서 처리해줍니다
    CustomMonsterSetting();

    // 죽을 때 경험치를 보여줬는가?
    m_bShowExp = false;


	//보스인가 함 보자
	if(iBoss==MONSTER::MON_BOSS)
	{	
		if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_BOSS_FURY))
		{


			float fY = (m_fHeight * 400.0f * m_fScale)/17.0f;
			
			m_pkFuryEffect = g_ParticleManager->CreateSequence( "Fury_monster", 0.0f, 0.0f , 0.0f );

			if (m_pkFuryEffect){
				m_pkFuryEffect->SetShow( &m_bRender );

				TRACE_INFO stTrace;
				stTrace.m_pvPos = &m_vHeadPos;
				stTrace.m_fParticleTraceTime = 1.0f;
				m_pkFuryEffect->SetTrace( &stTrace );

			}

		}
		//g_pkGameOverlayUI->AddBossMonster(this);
	}

	if(SiKGCEventManager()->IsActiveEvent(EVENT_ID_WINTERWENDY) && (int)MON_WENDY == m_iMonsterType)
	{
		int iTotalLevel = 0;
		
		for(int i = 0; i < MAX_PLAYER_NUM; ++i)
		{
			if( g_MyD3D->MyPlayer[i]->m_kUserInfo.cCharIndex != -1 )
				iTotalLevel += g_MyD3D->MyPlayer[i]->GetCurrentChar().iLevel;
		}

		float fLevelRate = (float)iTotalLevel / (SiKGCRoomManager()->GetUserCount() * 8.0f);
		
		if(fLevelRate > 1.0f)
		{
			m_fAttackPoint *= fLevelRate;
			m_fHPPoint     *= fLevelRate;
			m_fHP = m_fOldHP = m_fHPPoint;
		}
	}
    
    for( unsigned int i = 0; i < (int)kMonsterTemplate.vecSwooshInfo.size(); i++ )
    {
        KMonsterSwoosh swoosh;
        swoosh.m_pEffect = new GCEffectsSword();
        swoosh.m_pEffect->Init();
        swoosh.m_pEffect->Show( false );

		bool bFullTexture = true;
		if ( kMonsterTemplate.vecSwooshInfo[i].fTu1 != 0.0f || kMonsterTemplate.vecSwooshInfo[i].fTu2 != 0.0f ||
			 kMonsterTemplate.vecSwooshInfo[i].fTv1 != 1.0f || kMonsterTemplate.vecSwooshInfo[i].fTv2 != 1.0f )
			bFullTexture = false;
        swoosh.m_pEffect->SetFullTexture( bFullTexture );
        swoosh.m_pEffect->SetTextureUV( kMonsterTemplate.vecSwooshInfo[i].fTu1,
                                        kMonsterTemplate.vecSwooshInfo[i].fTu2,
                                        kMonsterTemplate.vecSwooshInfo[i].fTv1,
                                        kMonsterTemplate.vecSwooshInfo[i].fTv2);
        swoosh.m_pTexture = g_pGCDeviceManager2->CreateTexture( kMonsterTemplate.vecSwooshInfo[i].strTex );
        swoosh.m_iBaseBone = kMonsterTemplate.vecSwooshInfo[i].iBaseBone;
        swoosh.m_iEndBone = kMonsterTemplate.vecSwooshInfo[i].iEndBone;
        
        m_vecSwoosh.push_back( swoosh );
    }

    for( unsigned int i = 0; i < (int)kMonsterTemplate.vecPushRectInfo.size(); i++ )
    {
        GCPushRect<float> Rect;
        Rect.iBoneId = kMonsterTemplate.vecPushRectInfo[i].iBoneId;
        Rect.fWidth = kMonsterTemplate.vecPushRectInfo[i].fWidth;
        Rect.fHeight = kMonsterTemplate.vecPushRectInfo[i].fHeight;
        Rect.fOffsetX = kMonsterTemplate.vecPushRectInfo[i].fOffsetX;
        Rect.fOffsetY = kMonsterTemplate.vecPushRectInfo[i].fOffsetY;

        m_vecPushRect.push_back( Rect );
    }

    for( unsigned int i = 0; i < (int)kMonsterTemplate.vecAttackRectInfo.size(); i++ )
    {
        GCAttackRect<float> Rect;
        Rect.iBoneId = kMonsterTemplate.vecAttackRectInfo[i].iBoneId;
        Rect.fWidth = kMonsterTemplate.vecAttackRectInfo[i].fWidth;
        Rect.fHeight = kMonsterTemplate.vecAttackRectInfo[i].fHeight;
        Rect.fDelta = kMonsterTemplate.vecAttackRectInfo[i].fDelta;
		Rect.OffsetX = kMonsterTemplate.vecAttackRectInfo[i].OffsetX;
		Rect.OffsetY = kMonsterTemplate.vecAttackRectInfo[i].OffsetY;

        m_vecAttackRect.push_back( Rect );
    }

    for( unsigned int i = 0; i < (int)kMonsterTemplate.vecFootHoldInfo.size(); i++ )
    {
        KMonsterFootHold FH;

        FH.m_iBaseBone  = kMonsterTemplate.vecFootHoldInfo[i].m_iBaseBone; 
        FH.m_iEndBone   = kMonsterTemplate.vecFootHoldInfo[i].m_iEndBone; 
        FH.m_dwProp     = kMonsterTemplate.vecFootHoldInfo[i].m_dwProp;    
        FH.m_fRelativeX = kMonsterTemplate.vecFootHoldInfo[i].m_fRelativeX;
        FH.m_fRelativeY = kMonsterTemplate.vecFootHoldInfo[i].m_fRelativeY;

        m_vecFootHold.push_back( FH );
    }


#if defined( ANGELS_EGG_REFORM )
    if( GC_GM_ANGELS_EGG != SiKGCRoomManager()->GetGameMode())
#endif
    {
        if( GC_GM_DOTA != SiKGCRoomManager()->GetGameMode() && g_kGlobalValue.IsTutorialMode() == false )
        {
            if( TraceIcon.empty() )
            {
                TRACE_INFO trace;		
                trace.m_fParticleTraceTime = 5.0f;
                trace.m_pvPos = &TraceIconPos;

                std::vector<std::string> vecSquString;
                const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];

                // 파티클 시퀀스
                if( "" == kMonsterTemplate.sTraceIcon.strSqu_1 )
                    vecSquString.push_back( "M001" );
                else
                    vecSquString.push_back( kMonsterTemplate.sTraceIcon.strSqu_1.c_str() );

                if( "" == kMonsterTemplate.sTraceIcon.strSqu_2 )
                    vecSquString.push_back( "M002" );
                else
                    vecSquString.push_back( kMonsterTemplate.sTraceIcon.strSqu_2.c_str() );

                // 출력 옵션
                m_bIsMyExistScreen = kMonsterTemplate.sTraceIcon.bIsMyExistScreen;

                // 파티클 로드
                std::vector<std::string>::iterator vecIter = vecSquString.begin();
                for(; vecIter != vecSquString.end(); ++vecIter)
                {
                    CParticleEventSeqPTR pSeq = g_ParticleManager->CreateSequence( vecIter->c_str(), 0.0f, 0.0f, 0.0f );
                    if (pSeq)
                    {
                        pSeq->SetTrace( &trace );
                        if( vecIter == vecSquString.begin() )
                            pSeq->SetBeginPos( TraceIconPos );

                        TraceIcon.push_back( pSeq );
                    }
                }
            }
        }
    }

    m_bBlur = false;

    m_vecHitPos = D3DXVECTOR2( 0.0f, 0.0f );

	m_pObject->SetLayer( kMonsterTemplate.iLayer );
	m_pkMonsterModelDead->SetLayer( kMonsterTemplate.iLayer );

    // 캐릭터형 몬스터라면 캐릭터 타입도 
    m_iCharType = kMonsterTemplate.iCharType;

    // 로드완료!
    m_bLive = true;
    m_bDie = false;
    m_bSummonMonsterDieType = false;	

    m_fHP_Recover = 0.02f;
    KGCPC_MEMO( "Monster Start" );

#if defined ( USE_MERRY_HP_BAR )
    // 호위몬스터의 경우 HP 게이지 세팅 
    ShowEscortMonsterHPBar();
#endif

    // AI 스크립트의 초기화 함수 불러준다
    // 여기서 플레이어의 공격 속성에 따른 몬스터의 피격모션을 연결 시킨다.
    char strFunc[MAX_PATH];
    sprintf( strFunc, "%s(%d)", kMonsterTemplate.strInitFunc.c_str(), m_iMonsterIndex );
    KGCLuabinder::getInstance().DoString( strFunc, strlen(strFunc) );
    sprintf( strFunc, "%s(%d)", "InitHPGauge", m_iMonsterIndex );
    KGCLuabinder::getInstance().DoString( strFunc, strlen(strFunc) );
    return true;
}

// 몬스터가 죽었을때 스크립트에서 불러준다
void MONSTER::EndMonster( bool bBlink_ /*= true*/, bool bRenderOff /*= false */, bool bDirectDie /*= false*/)
{
    m_bDie = true;                                                     // Ondie()는 안불릴 수도 있으므로 여기에 옮김.    
    m_iSummoned = MEMS_NOT_SUMMON_MONSTER;
	m_iSummonsNuberAvailable = 15;

#if defined ( USE_MERRY_HP_BAR )
    ShowEscortMonsterHPBar();
#endif

    // EndMonster에서는 몬스터를 릴리즈 시키지는 않는다 Release따로있음
    // 깜빡거리다가 사라질것임..
    if( m_iRemainRender == 0 )
    {
        if ( false == bBlink_ )
        {
            m_iRemainRender = (int)( 55 * 1.8f ); // 잠깐 깜빡이다가.. 사라지도록..

            if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER || m_iBossMonster==MONSTER::MON_BOSS || GetDefaultMonster( m_iMonsterType ) == MON_MOONEVENT )
                m_iRemainRender = (int)( 55 * 4.0f ); // 보스는 쫌더 깜빡거려 줘야지..-ㅅ -;

            // 펌프킨 헤드는 터지고 바로 없어져야 되기 때문에..
            // 여기서 렌더링을 바로 꺼버린다..
            if( GetDefaultMonster( m_iMonsterType ) == MON_PUMPKIN ||
                GetDefaultMonster( m_iMonsterType ) == MON_STATUE ||
                GetDefaultMonster( m_iMonsterType ) == MON_DKMARK_II ||
                GetDefaultMonster( m_iMonsterType ) == MON_DECOY ||
                GetDefaultMonster( m_iMonsterType ) == MON_MIRAGE_POT ||
                GetDefaultMonster( m_iMonsterType ) == MON_PATUPLAT_GOLEM ||
                GetDefaultMonster( m_iMonsterType ) == MON_HELL_HAMMERMON_TRANS ||
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER1 ||
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER2 ||
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER3 ||
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER4 ||  
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER5 ||
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER6 ||
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER7 ||
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER8 || 
                GetDefaultMonster( m_iMonsterType ) == MON_EVENT_CHARACTER9
                )
                m_bRender = false;

            // 깜빡거리게 해 준다 파투세이는 안깜빡..
            EGCMonster eMonster = GetDefaultMonster( m_iMonsterType );
            if( eMonster != MON_PATUSEI_HEAD && 
                eMonster != MON_PATUSEI_BODY &&
                eMonster != MON_PATUSEI_LEG &&
                eMonster != MON_GIANT &&
                m_pObject != NULL
                )
            {
                m_pObject->SetMeshState( MS_DEAD );
            }
        }

        if( bRenderOff )
            m_bRender = false;

        if( bDirectDie )
            m_bLive = false;

        m_bFrameLock = true;
        m_bBuffFrameLock = false;

        for( int i = 0 ; i < GetPushRectNum(); i++)
        {
            SetPushCheck( false , i );
        }

		//보스인가 함 보자
		if(this->m_iBossMonster==MONSTER::MON_BOSS)
		{
			switch(m_iMonsterType)
			{		
			case MON_PATUSEI_BODY:			
			case MON_PATUSEI_LEG:		
			case MON_VS_PATUSEI_BODY:			
			case MON_VS_PATUSEI_LEG:
				break;
			default:
				g_pkGameOverlayUI->EnableBossHPBar( false, m_iMonsterType );
				g_pkGameOverlayUI->RenderOffHPBar( false );
				break;
			}
		}

        // 소환 몬스터일때 버프 아이콘을 제거해주자!
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerUID );
        if( pPlayer && pPlayer->IsLocalPlayer() && m_iSummonBuffIcon )
        {
            if( pPlayer->IsMagicEffect( m_iSummonBuffIcon ) )
                pPlayer->ClearMagicEffect( m_iSummonBuffIcon );
        }

        if ( m_kChangeMon.usMonType != -1 && g_kGlobalValue.m_kUserInfo.bHost )
        {
            g_kChangeMonster->Type = GC_PID_CHANGE_MONSTER;
            g_kChangeMonster->usMonIndex = m_iMonsterIndex;
            g_kChangeMonster->kChangeMon = m_kChangeMon;

            SendNetworkData( GC_PID_CHANGE_MONSTER, 0 );
        }
    }
}

void MONSTER::SetHPALLMonster( float fHP, int iTeam )
{
    MAP_INT_PMONSTER::iterator mapIter = g_kMonsterManager.m_mapMonster.begin();
    for(; mapIter != g_kMonsterManager.m_mapMonster.end(); ++mapIter)
    {
        if( false == mapIter->second->IsLive() )
            continue;

        if( -2 == iTeam && iTeam != mapIter->second->m_iTeam )
            continue;

        mapIter->second->SetHP( fHP );
    }
}

//사용한 모든 리소스를 반환함!
void MONSTER::Release()
{
    // 몬스터 모델 리소스 해제
    EndAfterImage();
    SAFE_REMOVE_DELETE( m_pObject );
    SAFE_REMOVE_DELETE( m_pkMonsterModelDead );

    // 모션 리소스 해제
    for( int i = 0; i < (int)m_vecMotionList.size(); ++i )
    {
        SAFE_RELEASE( m_vecMotionList[i] );
    }
    m_vecMotionList.clear();

    m_kMonHPBar.ClearAllResource();

    for( int i = 0; i < KAMIKI_LIGHTNING_COUNT; ++i )
    {
        g_MyD3D->m_pMapAnim->DeleteInst( m_apKamikiLightning[i] );
        m_apKamikiLightning[i] = NULL;
    }

    // 변수 초기화
    Init();
    m_iTriggerID = -1;

    if( m_pkChampionEffect )
    {
        g_ParticleManager->DeleteSequence( m_pkChampionEffect );
        m_pkChampionEffect = NULL;
    }

	if( m_pkFuryEffect )
	{
		g_ParticleManager->DeleteSequence( m_pkFuryEffect );
		m_pkFuryEffect = NULL;
	}

    ClearTraceIcon();

    for ( unsigned int i = 0 ; i < m_vecShieldEffect.size(); ++i )
    {
        g_ParticleManager->DeleteSequence( m_vecShieldEffect[i] );
    }
    m_vecShieldEffect.clear();
    m_vecShieldEffectArg.clear();

    std::vector<CParticleEventSeqPTR>::iterator vecDieToClearIter = m_vecDieToClearParticle.begin();
    for(; vecDieToClearIter != m_vecDieToClearParticle.end(); ++vecDieToClearIter)
    {
        g_ParticleManager->DeleteSequence( *vecDieToClearIter );
    }
    m_vecDieToClearParticle.clear();

    for ( unsigned int i = 0 ; i < m_vecSwoosh.size(); ++i )
    {
        SAFE_DELETE( m_vecSwoosh[i].m_pEffect );
		SAFE_RELEASE(m_vecSwoosh[i].m_pTexture);
    }
    m_vecSwoosh.clear();

    m_vecPushRect.clear();
    m_vecAttackRect.clear();
    m_vecFootHold.clear();
    SetMovableMonster( true );

    m_vecItemIDList.clear();

    // 이 인덱스로 걸린 버프 모두 지우도록하자.
    g_pMagicEffect->ClearMonsterAllEffect( m_iMonsterIndex );

    KGCPC_MEMO( "Monster Released : " );
}

void MONSTER::InitBuffer( float fX, float fY, bool bRight )
{
    for( int i = 0; i < LATENCY_BUFFER_SIZE; ++i )
    {
        if( fX == -1.0f )
            SetLatencyX( i, SiKGCFantasticMap()->GetPixelWidth() * 0.5f );
        else
            SetLatencyX( i, fX );

        SetLatencyY( i, fY );

        m_abIsRight[i]  = bRight;
        m_aiFrame[i]    = 0;
        m_aiMotion[i]   = 0;
    }

    m_iOldFrame     = 0;
    m_iOldMotion    = 0;
}

bool MONSTER::Wait_Room( int iMonsterType, float fX, float fY, bool bRight, bool bLoadModel_,
                         int iLevel /*= -1*/, DWORD dwChampionProperty /*= 0x00000000*/, int iBoss /*= false*/, bool bUseItemList /*= false*/  )
{
    if( m_bLive )
        return false;

    int iMonster = 0;
    //if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
    // 던전이든 아니든.. 처리해줘야지..
    {
        if( iMonsterType >= (int)g_kMonsterManager.m_vecMonsterTable.size() )
            iMonster = (int)g_kMonsterManager.m_vecMonsterTable.size() - 1;
        else
            iMonster = iMonsterType;
    }

    InitBuffer( fX, fY, bRight );

    m_bLive = false;
    bool bSuccess = false;
    if( iMonster == -1 )
    {
        Release();
    }
    else
    {
        if ( bLoadModel_ == true )
		{
            bSuccess = StartMonster( (EGCMonster)iMonster, iLevel, dwChampionProperty, iBoss, bUseItemList );

#ifdef __PATH__
			if( !bSuccess )
			{
				// 몬스터 모션파일이 없다면 튕겨내자!!
				g_MyD3D->m_pStateMachine->ExitRoom( true );
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_DELETE_MONSTER_MOTION_ERROR ));
				Release();
				return false;
			}
#else
            if( !bSuccess )
            {
                // 몬스터 모션파일이 없다면 튕겨내자!!
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK
                    , g_pkStrLoader->GetReplacedString( STR_ID_MONSTER_ID_MOTION_ERROR, "i" , iMonster )
                    );
                Release();
                return false;
            }
#endif
		}

        g_pGameBoard->AddMonsterNum( iMonster, 1 );
	}
    return bSuccess;
}


float MONSTER::ChangeHP( CDamageInstance* pDamageInstance, float fDamage, int iPlayerIndex, bool bSpecialAtk, bool bPetDamage, int nSpecialGrade, int iMonsterIndex_, bool bIsDarkSpearDamage, bool bIsNoCritDmg )
{
    // 죽은 몬스터에겐 데미지를 줄 이유가 없죠..
    if( m_fHP <= 0.0f ) 
    {    return 0.0f;    }

    //플레이어 인덱스가 0보다 작을 순 없으므로 리턴, LatencyTransfer.cpp -> CLatencyTransfer::PacketHandler -> switch( packet->packetType ) -> case ELTP_MAGIC_DAMAGE_SYNCH: 에서 플레이어 인덱스없이 넘어오는게 있어(값 없으면 기본값 -1 ) 이런경우를 위한 크래시 체크
    if( iPlayerIndex < 0 )
        return 0.0f;

    //아테네 소워드를 쓰는 동안에는 공격력이 15% 상승한다.
    if( iPlayerIndex < MAX_PLAYER_NUM )
    {
		//핵 체크
		KHackCheckManager::GetInstance()->BeforeCheckApRatio(&g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATTACK_PERCENT] , &fDamage);
        fDamage = GetDamageFromPlayerAttack(fDamage, iPlayerIndex, bSpecialAtk);
		KHackCheckManager::GetInstance()->AfterCheckApRatio(&fDamage);


        // 속성 능력치 적용!
        // 필살기 공격력~ 필살기 일때, 필살기 공격력을 더해서 피 깎는다..
        float fAttributeSpecialAtk = 0.0f;
#if defined(LEVEL_DESIGN_STAT_CALC) 
        fAttributeSpecialAtk = bSpecialAtk ? g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_SPECIAL_ATK]*0.01f : 0.0f;
#else
        fAttributeSpecialAtk = bSpecialAtk ? g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_SPECIAL_ATK] : 0.0f;
#endif

#if defined(LEVEL_DESIGN_STAT_CALC) 
        fDamage *= ((g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATK]*0.01f + fAttributeSpecialAtk) * 1.4f);
#else
        fDamage *= ((g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATK] + fAttributeSpecialAtk) * 1.4f);
#endif

    }

    MONSTER* pMonster = g_kMonsterManager.GetMonster( iMonsterIndex_ );
    if( pMonster && pMonster->m_fSummonsAtkRatio != 0.f && pMonster->m_fOwnerAtkRatio != -1.f ) {
        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( pMonster->m_dwOwnerUID );
        if( pPlayer ) {
            fDamage /= pMonster->m_fSummonsAtkRatio;
            KHackCheckManager::GetInstance()->BeforeCheckApRatio(&pPlayer->m_fAbility[ABILITY_ATTACK_PERCENT] , &fDamage);
            fDamage = GetDamageFromPlayerAttack(fDamage, pPlayer->m_iPlayerIndex, false, pMonster->m_fOwnerAtkRatio );
#if defined(LEVEL_DESIGN_STAT_CALC) 
            fDamage *= ((pPlayer->m_fAbility[ABILITY_ATK]*0.01f * pMonster->m_fOwnerAtkRatio ) * 1.4f);
#else
            fDamage *= ((pPlayer->m_fAbility[ABILITY_ATK] * pMonster->m_fOwnerAtkRatio ) * 1.4f);
#endif
            

        }        
    }

    int CorruptedIndex = g_MyD3D->MyPlayer[iPlayerIndex]->GetCorruptedBuffNumber();
    if ( CorruptedIndex > 0 )
        fDamage -= fDamage * ( CorruptedIndex * 0.01f );

    bool bCritialAtk = false;

    if (!bIsNoCritDmg)
    {
        if (!g_kGlobalValue.m_bCriticalOffToggle && (g_kGlobalValue.m_bCriticalToggle || (randf() < g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_CRITICAL_ATTACK_RATE])))
        {
            bCritialAtk = true;

            float fCriticalDamageUP = 0.0f;
            if (g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] > 0)
            {
                fCriticalDamageUP = (g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_CRITICAL_DAMAGE_UP] / 100.f);
            }

            if (g_pMagicEffect->IsMagicEffect(iPlayerIndex, EGC_EFFECT_ATTRIBUTE_CRITICAL_DAMAGE_UP))
            {
                fCriticalDamageUP += (g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_FATAL_CRITICAL_DAMAGE_UP_BUFF] / 100.f);
            }

            if (g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->m_pObject != nullptr)
            {
                if (g_MyD3D->Get_MyPlayer() == iPlayerIndex)
                {
                    fCriticalDamageUP += SiGCPetMagicStone()->GetGlyphEffectAmmount(PET_MAGIC_STONE_EFFECT_TYPE::PMSET_INCREASE_CRITICAL);
                }
            }

            fDamage *= ((CRITICALDAMAGE_UP_RATE + fCriticalDamageUP) * 0.7f);

            SiGCGrowingSkillManager()->SetMagicEffectWhenCritical(g_MyD3D->MyPlayer[iPlayerIndex]);
        }
    }

    if( m_dwChampionProperty & CP_DEFENCE )
        fDamage /= 1.5f;

    bool bSpearAtk = false;

    if (bIsDarkSpearDamage)
    {
        if (g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] > 0.0f)
        {
            if (bPetDamage == false && randf() < 0.2f && fDamage != 0.0f)
            {
                bSpearAtk = true;

                if (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() || GC_GMC_MONSTER_CRUSADER == SiKGCRoomManager()->GetGameModeCategory())
                    fDamage += g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] * 100;
                else
                    fDamage += g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_DARK_DAMAGE_ATK_POINT] * 0.0005f;
            }
        }
    }

    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];

    if ( bSpecialAtk )
    {
        fDamage /= m_fSDefRatio;

        if ( kMonsterTemplate.fSDefence < 1.0f )
            fDamage *= ( 1.0f - kMonsterTemplate.fSDefence );
        else
            fDamage = 0.0f;
    }
    else
    {
        if ( kMonsterTemplate.fDefence < 1.0f )
            fDamage *= ( 1.0f - kMonsterTemplate.fDefence );
        else
            fDamage = 0.0f;
    }

    if ( m_fDefRatio < 9999.0f ) {
        if ( false == bSpearAtk )
            fDamage /= m_fDefRatio;
    }
    else
        fDamage = 0.0f;

    if ( kMonsterTemplate.iHitHP > 0 )
    {
        fDamage = m_fHPPoint / static_cast<float>(kMonsterTemplate.iHitHP);
    }

    if ( this->IsMagicEffect( m_iMonsterIndex, EGC_EFFECT_MON_INFINITY_DEFENCE) || m_bUnlimitDef )
    {
        fDamage = 1.0f;
    }

    if( m_iMonsterType == g_kGlobalValue.GetEscortMonsterID() )
    {
        if( m_kEscortStopWatch.IsStop() ) {
            fDamage = (float)( g_kGlobalValue.GetEscortMonsterFixedDamage() );
            m_kEscortStopWatch.SetStopWatch( g_kGlobalValue.GetEscortMonsterInvincibleTime() );
            m_fShield = 0.0f;
        }
        else {
            m_fShield = 99999.0f;
        }
        
    }

#if defined( USE_MERRY_HP_BAR )
    ShowEscortMonsterDesc( fDamage );
#endif

    SetDamageDiff( fDamage );
    ActionCallBack( "DamageBeforeChange" );
    if( GetDamageDiff() != fDamage )
        fDamage = GetDamageDiff();

    if ( bPetDamage )
    {
        fDamage*=0.71f;
    }
    float fRealMonsterHP = m_fHP;
    if ( m_fShield > 0.0f )
    {
        if( m_fShield - fDamage < 0.0f )
            m_fShield = 0.0f;
        else
            m_fShield -= fDamage;

        fDamage = 0.0f;
    }
    else
    {
        if( m_fHP - fDamage < 0.0f )
            m_fHP = 0.0f;
        else {
            m_fHP -= fDamage;

            if( m_fHP > m_fHPPoint )
            {
                fDamage -= (m_fHP - m_fHPPoint);
                m_fHP = m_fHPPoint;
            }
        }
    }

    if ( fDamage > 0.0f )
    {
        LTP_CHANGE_HP_INFO kChangeHPInfo;        
        kChangeHPInfo.iAttackerIndex = iPlayerIndex;
#ifdef LEVEL_DESIGN_STAT_CALC
        kChangeHPInfo.fDamage = fDamage * -1.f;
#else
        kChangeHPInfo.fDamage = -(fDamage / 100.0f);
#endif
		kChangeHPInfo.iTargetIndex = m_iMonsterIndex + MAX_PLAYER_NUM;
        kChangeHPInfo.fHitPosX = GetHitPos().x;
        kChangeHPInfo.fHitPosY = GetHitPos().y;

        char dwAttackType = PAA_NORMAL;
        if ( bSpecialAtk )
            dwAttackType = PAA_SPECIAL;
        else if ( bPetDamage )
            dwAttackType = PAA_PET;

        if ( bCritialAtk )
        {
            dwAttackType += PAA_CRITICAL;
        }
        if (bSpearAtk) { 
            dwAttackType += g_kGlobalValue.GetSpearType( iPlayerIndex );
        }

        kChangeHPInfo.dwAttackType = dwAttackType;
        kChangeHPInfo.fMonsterMaxHP = fRealMonsterHP;

        g_LatencyTransfer.PushPacket( &kChangeHPInfo );

#if defined( USE_QA_LOG_SYSTEM )
        SiKGCQALogManager()->MonsterChangeHPLog( g_MyD3D->MyPlayer[ iPlayerIndex ]->GetStrUserName(), GetMonsterName(), m_iMonsterIndex, 
                                                 m_fOldHP, m_fHP, fDamage, bPetDamage, bSpecialAtk, bCritialAtk, bSpearAtk );
#endif
    }

    if( IsNoComboSkillCheckMonster() == false && m_fHP > 0 )
    {
        float fTempDamage = fDamage;
        if( fDamage > m_fHP )
            fTempDamage = m_fHP;

        m_fCumulativeDamage += fTempDamage;
    }

    if( m_fHP == 0 )    
    {
        if( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
        {
            if( g_MyD3D->IsPlayerIndex( iPlayerIndex ) &&
                g_MyD3D->MyPlayer[ iPlayerIndex ]->Get_D_Point() < g_MyD3D->MyPlayer[ iPlayerIndex ]->Get_MAX_D_Point() )
            {
                if( g_kGlobalValue.m_kUserInfo.bHost )
                {
                    g_MyD3D->MyPlayer[ iPlayerIndex ]->Set_D_Point( g_MyD3D->MyPlayer[ iPlayerIndex ]->Get_D_Point() + g_kGlobalValue.GetDotaMonster_D_Point() );

                    int iDPoint[MAX_PLAYER_NUM]={0,};
                    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
                    {
                        if( g_MyD3D->MyPlayer[ iLoop ]->IsLive() )
                            iDPoint[iLoop] = g_MyD3D->MyPlayer[ iLoop ]->Get_D_Point();
                    }
                    KGC_PID_BROAD_DOTA_POINT kCustom( GC_PID_BROAD_DOTA_POINT, g_MyD3D->MyPlayer[ iPlayerIndex ]->m_kUserInfo.dwUID, iDPoint );
                    SendP2PPacket( &kCustom, sizeof(kCustom), KNC_BROADCAST_NOT_ME, _RELIABLE );
                    KGCPC_MEMO( "GC_PID_BROAD_DOTA_POINT" );
                }

                int iMyPlayerID = g_MyD3D->Get_MyPlayer();
                if( g_MyD3D->IsPlayerIndex(iMyPlayerID) && g_MyD3D->IsSameTeam(iMyPlayerID, iPlayerIndex) )
                {
                    g_MyD3D->MyPlayer[ iPlayerIndex ]->AddParticleUINumber( g_kGlobalValue.GetDotaMonster_D_Point(), D3DXVECTOR2(-1.05f, -0.05f), true );
                }
            }

            SDotaTemplate* pSDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
            if( pSDotaTemplate && g_MyD3D->IsPlayerIndex(iPlayerIndex) && g_kGlobalValue.m_kUserInfo.bHost )
            {
                int iMsgID = -1;
                switch(m_iMonsterType)
                {
                case MON_DOTA_GUARDIAN_TOWER:
                    iMsgID = SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDIANTOWER;
                    break;
                case MON_DOTA_GUARD_TOWER:
                    iMsgID = SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDTOWER;
                default:
                    iMsgID = SDotaPlayingDesc::MAX_DOTA_MSG;
                    break;
                }
                if( -1 != iMsgID )
                {
                    int iTeamString;
                    if( TEAM_SERDIN == m_iTeam )    iTeamString = STR_TEAM_SERDIN;
                    else                            iTeamString = STR_TEAM_CANABAN;

                    KGC_PID_BROAD_DOTA_MONSTERKILL_MSG kPacket;
                    kPacket.bKillerMonster       = false;
                    kPacket.iMsgID = iMsgID;
                    kPacket.iKillerIdx = iPlayerIndex;
                    kPacket.iTeamStringID = iTeamString;
                    kPacket.iMonsterIdx = m_iMonsterIndex;
                    kPacket.vDeathPos.x = GetX();
                    kPacket.vDeathPos.y = GetY();
                    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
                    KGCPC_MEMO( "KGC_PID_BROAD_DOTA_MONSTERKILL_MSG" );
                }
            }
        }
    }

    {
        g_MyD3D->MyPlayer[iPlayerIndex]->ProcessBackStepBuff();
    }

    if ( g_MyD3D->MyPlayer[iPlayerIndex]->IsMagicEffect( EGC_SKILL_HIDE ) )
    {
        g_MyD3D->MyPlayer[iPlayerIndex]->Item_Remain_Time[GC_GAME_ITEM_HALLOW] = 0;

        g_pMagicEffect->ClearMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_SKILL_HIDE, false, true );
    }

    m_iLastAttackedMe = iPlayerIndex;

    if( GetHP() <= 0.f ) {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[ iPlayerIndex ];
        if( pPlayer ) {
            if( pPlayer->m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_LIME ) {
                if( pPlayer->IsLocalPlayer() ) {
                    if( pPlayer->CheckSkill( SID_LIME_NORMAL_KILL_BUFF ) ) {
                        pPlayer->SetHP( pPlayer->GetHP() + pPlayer->GetPlayerMaxHP() * 0.05f );
                    }
                } else {
                    if( pPlayer->IsTrainedSkill( SID_LIME_NORMAL_KILL_BUFF ) ) {
                        LTP_CHANGE_PLAYER_HP kPacket;
                        kPacket.m_dwUserUID = pPlayer->m_kUserInfo.dwUID;
                        kPacket.m_fChangeHP = pPlayer->GetHP() + pPlayer->GetPlayerMaxHP() * 0.05f;
                        g_LatencyTransfer.PushPacket( &kPacket );
                    }
                }
            } else if( pPlayer->m_kUserInfo.GetCurrentChar().iCharType == GC_CHAR_RIN && pPlayer->MetamorphosisForm != FORM_NORMAL ) {
                if( pPlayer->IsLocalPlayer() ) {
                    if( pPlayer->CheckSkill( SID_RIN4_FORM_PASSIVE_SKILL_1_TYPE_B ) ) {
                        pPlayer->SetHP( pPlayer->GetHP() + pPlayer->GetPlayerMaxHP() * 0.05f );
                    }
                } else {
                    if( pPlayer->IsTrainedSkill( SID_RIN4_FORM_PASSIVE_SKILL_1_TYPE_B ) ) {
                        LTP_CHANGE_PLAYER_HP kPacket;
                        kPacket.m_dwUserUID = pPlayer->m_kUserInfo.dwUID;
                        kPacket.m_fChangeHP = pPlayer->GetHP() + pPlayer->GetPlayerMaxHP() * 0.05f;
                        kPacket.m_iSkillID = static_cast< int >( SID_RIN4_FORM_PASSIVE_SKILL_1_TYPE_B );
                        g_LatencyTransfer.PushPacket( &kPacket );
                    }
                }
            }
        }
    }    
    
    return fDamage;
}

void MONSTER::ChangeHPToMonster( CDamageInstance* pDamageInstance, float fDamage, int iMonsterIndex, bool bSpecialAtk )
{
    // Á×Àº ¸ó½ºÅÍ¿¡°Õ µ¥¹ÌÁö¸¦ ÁÙ ÀÌÀ¯°¡ ¾øÁÒ..
    if( m_fHP <= 0.0f ) 
    {    return;    }

    // Ã¨ÇÇ¾ð Áß ¹æ¾î·Â »ó½Â
    if( m_dwChampionProperty & CP_DEFENCE ) // ¹æ¾î·Â »ó½Â
        fDamage /= g_kGlobalValue.GetChampionAbility(static_cast<int>(ECHAMPION_DEFENCE_RATE)); // ¹æ¾î·Â 1.5¹è Áõ°¡


    bool bSpearAtk = false;
	MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
	if(pMonster != NULL) {
		if (pMonster->m_fAttackPoint > 0.0f)
		{
			if(bSpecialAtk ==false && randf()< 0.1f && fDamage != 0.0f)
			{
				bSpearAtk= true;

				if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
					fDamage += pMonster->m_fAttackPoint; 
				else
					fDamage += pMonster->m_fAttackPoint * 0.0005f; 
			}
		}
	}


    // ¼Ó¼º ´É·ÂÄ¡ Àû¿ë!
    // ÀÌ°Ç.. ¸ó½ºÅÍ ÅÛÇÃ¸´¿¡¼­ »ç¿ëÇÏ´Â µðÆæ½º ·¹ÀÌÆ®
    // reserved fDefence, fSDefence
    // fDefence, fSDefence ´Â 0.0ÀÌ¸é µé¾î¿À´Â µ¥¹ÌÁö ±×´ë·Î ¹Þ°í, 1.0ÀÌ¸é ¹«ÇÑ ¹æ¾îÀÌ´Ù.
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];

    if ( bSpecialAtk )
    {
        // ÇÊ»ì±â ·¹ÀÌÆ®( ¹öÇÁ¿¡¼­ »ç¿ëµÊ )
        fDamage /= m_fSDefRatio;

        // ÇÊ»ì±â °ø°ÝÀÌ¸é~ 
        if ( kMonsterTemplate.fSDefence < 1.0f )
            fDamage *= ( 1.0f - kMonsterTemplate.fSDefence );
        else
            fDamage = 0.0f;
    }
    else
    {
        // ÀÏ¹Ý °ø°ÝÀÌ¸é~ (ÇÊ¿äÇÑ°¡?;;)
        if ( kMonsterTemplate.fDefence < 1.0f )
            fDamage *= ( 1.0f - kMonsterTemplate.fDefence );
        else
            fDamage = 0.0f;
    }



    //////////////////////////////////////////////////////////////////////////
    // ÀÌ°Ç.. aiÆÄÀÏ¿¡¼­ ÇÑ¹ø¾¿ »ç¿ëÇÏ´Â... µðÆæ½º ·¹ÀÌÆ®
    // infinity defence
    if ( m_fDefRatio < 9999.0f )
        fDamage /= m_fDefRatio;
    else
        fDamage = 0.0f;
    //////////////////////////////////////////////////////////////////////////

    if ( kMonsterTemplate.iHitHP > 0 )
    {
        fDamage = m_fHPPoint / static_cast<float>(kMonsterTemplate.iHitHP);
    }

    // ¾ÕÀ¸·Î ÀÌ ¹öÇÁ´Â ¸ó½ºÅÍ°¡ 1¸¸ µû¸£°Ô ÇÒ¶§!! ¤»
    if ( this->IsMagicEffect( m_iMonsterIndex, EGC_EFFECT_MON_INFINITY_DEFENCE) || m_bUnlimitDef )
    {
        fDamage = 1.0f;
    }

#if defined( USE_MERRY_HP_BAR )
    // È£À§Äù ¸ó½ºÅÍ¸é ÀÏÁ¤ Ã¼·Â¸¶´Ù °æ°í ¸Þ½ÃÁö 
    ShowEscortMonsterDesc( fDamage );
#endif

    // µ¥¹ÌÁö ÄÝ¹é
    SetDamageDiff( fDamage );
    ActionCallBack( "DamageBeforeChange" );
    if( GetDamageDiff() != fDamage )
        fDamage = GetDamageDiff();

    
    // ½ÇÁ¦ ÇÇ µû´Â ºÎºÐ  ///
    if ( m_fShield > 0.0f )
    {
        if( m_fShield - fDamage < 0.0f )
            m_fShield = 0.0f;
        else
            m_fShield -= fDamage;

        // ÆÐÅ¶ ¾Èº¸³½´Ù.. 
        fDamage = 0.0f;
    }
    else
    {
        if( m_fHP - fDamage < 0.0f )
            m_fHP = 0.0f;
        else
            m_fHP -= fDamage;
    }

    //////////////////////////////////////////////////////////////////////////
    if( m_fHP == 0 )    
    {
        if( GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
        {
            // °¡µåÅ¸¿ö³ª °¡µð¾ðÅ¸¿ö°¡ ±úÁ³´Ù¸é ¸Þ½ÃÁö¸¦ Ãâ·ÂÇØÁÖÀÚ.
            SDotaTemplate* pSDotaTemplate = g_kGlobalValue.GetCurrentDotaTemplate();
            if( pSDotaTemplate && g_kMonsterManager.IsMonsterIndex(iMonsterIndex) )
            {
                int iMsgID = -1;
                switch(m_iMonsterType)
                {
                case MON_DOTA_GUARDIAN_TOWER:
                    iMsgID = SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDIANTOWER;
                    break;
                case MON_DOTA_GUARD_TOWER:
                    iMsgID = SDotaPlayingDesc::DOTA_MSG_BREAK_GUARDTOWER;
                    break;
    }
                if( -1 != iMsgID )
                {
                    int iTeamString;
                    if( TEAM_SERDIN == m_iTeam )    iTeamString = STR_TEAM_SERDIN;
                    else                            iTeamString = STR_TEAM_CANABAN;

                    KGC_PID_BROAD_DOTA_MONSTERKILL_MSG kPacket;
                    kPacket.bKillerMonster       = true;
                    kPacket.iMsgID = iMsgID;
                    kPacket.iKillerIdx = iMonsterIndex;
                    kPacket.iTeamStringID = iTeamString;
                    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
                    KGCPC_MEMO( "KGC_PID_BROAD_DOTA_MONSTERKILL_MSG" );
                }
            }
        }
    }

    m_iLastAttackedMe = iMonsterIndex;
}

void MONSTER::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KParticleArg>( L, "KParticleArg" );
    lua_tinker::class_con<KParticleArg>( L, lua_tinker::constructor<>() );
        lua_tinker::class_mem<KParticleArg>( L, "fOffsetX",               &KParticleArg::fOffsetX  );
        lua_tinker::class_mem<KParticleArg>( L, "fOffsetY",               &KParticleArg::fOffsetY  );
        lua_tinker::class_mem<KParticleArg>( L, "fDirPosX",               &KParticleArg::fDirPosX  );
        lua_tinker::class_def<KParticleArg>( L,  "Init",                            &KParticleArg::Init  );
        lua_tinker::class_def<KParticleArg>( L,  "SetName",                         &KParticleArg::SetName  );
        ;

        lua_tinker::class_add<KBoundBox>( L, "KBoundBox" );
        lua_tinker::class_con<KBoundBox>( L, lua_tinker::constructor<>() );
        lua_tinker::class_mem<KBoundBox>( L, "fTOP",                   &KBoundBox::fTop  );
        lua_tinker::class_mem<KBoundBox>( L, "fLEFT",                  &KBoundBox::fLeft  );        
        lua_tinker::class_mem<KBoundBox>( L, "fRIGHT",                 &KBoundBox::fRight  );
        lua_tinker::class_mem<KBoundBox>( L, "fBOTTOM",                &KBoundBox::fBottom  );
        ;

        lua_tinker::class_add<KSummonInfo>( L, "KSummonInfo" );
        lua_tinker::class_con<KSummonInfo>( L, lua_tinker::constructor<>() );
        lua_tinker::class_mem<KSummonInfo>( L, "iMonType",               &KSummonInfo::iMonType  );
        lua_tinker::class_mem<KSummonInfo>( L, "iCountValue",            &KSummonInfo::iCountValue  );
        lua_tinker::class_mem<KSummonInfo>( L, "fX",                     &KSummonInfo::fX  );
        lua_tinker::class_mem<KSummonInfo>( L, "fY",                     &KSummonInfo::fY  );
        lua_tinker::class_mem<KSummonInfo>( L, "bCheckSafePostion",      &KSummonInfo::bCheckSafePostion  );
        lua_tinker::class_def<KSummonInfo>( L,  "Init",                  &KSummonInfo::Init  );
        lua_tinker::class_def<KSummonInfo>( L,  "SetState",              &KSummonInfo::SetState  );
        ;

        lua_tinker::class_add<COLOR_ANIINFO>( L, "KRealMiniMapColorAniInfo" );
        lua_tinker::class_con<COLOR_ANIINFO>( L, lua_tinker::constructor<>() );
        lua_tinker::class_def<COLOR_ANIINFO>( L,  "SetMinLimit",           &COLOR_ANIINFO::SetMinLimit  );
        lua_tinker::class_def<COLOR_ANIINFO>( L,  "SetMaxLimit",           &COLOR_ANIINFO::SetMaxLimit  );
        lua_tinker::class_def<COLOR_ANIINFO>( L,  "SetAniSpeed",           &COLOR_ANIINFO::SetAniSpeed  );
        lua_tinker::class_def<COLOR_ANIINFO>( L,  "SetAniInfo",            &COLOR_ANIINFO::SetAniInfo  );
        ;


    lua_tinker::class_add<MONSTER>( L, "MONSTER" );
	lua_tinker::class_inh<MONSTER, KGCObj>( L );	
        lua_tinker::class_mem<MONSTER>( L,  "fTargetX",              &MONSTER::m_fTargetX  ); 
        lua_tinker::class_mem<MONSTER>( L,  "fTargetY",              &MONSTER::m_fTargetY  );
        lua_tinker::class_mem<MONSTER>( L,  "fReserveValue",         &MONSTER::m_fReserveValue  );
		lua_tinker::class_mem<MONSTER>( L,  "Team", 					&MONSTER::m_iTeam  );
        lua_tinker::class_mem<MONSTER>( L,  "iFlashEffectFrame", 	&MONSTER::m_iFlashEffectFrame  );
        lua_tinker::class_mem<MONSTER>( L,  "DefRatio", 	            &MONSTER::m_fDefRatio  );
        lua_tinker::class_mem<MONSTER>( L,  "m_bRender", 	            &MONSTER::m_bRender  );
        lua_tinker::class_mem<MONSTER>( L,  "m_iRecvStateFromPlayer", 	&MONSTER::m_iRecvStateFromPlayer  );
        lua_tinker::class_mem<MONSTER>( L,  "m_bCollisionCheck", 	&MONSTER::m_bCollisionCheck  );
        lua_tinker::class_mem<MONSTER>( L,  "m_iBossMonster", 	    &MONSTER::m_iBossMonster  );
        

        lua_tinker::class_def<MONSTER>( L,  "PlaySound",                       &MONSTER::PlaySound  );
        lua_tinker::class_def<MONSTER>( L,  "GetLastFrame",                    &MONSTER::GetLastFrame  );
        lua_tinker::class_def<MONSTER>( L,  "SetState",                        &MONSTER::SetState  );
        lua_tinker::class_def<MONSTER>( L,  "CheckState",                      &MONSTER::CheckState  );
        lua_tinker::class_def<MONSTER>( L,  "StartAttack",                     &MONSTER::StartAttack  );
        lua_tinker::class_def<MONSTER>( L,  "EndAttack",                       &MONSTER::EndAttack  );

        lua_tinker::class_def<MONSTER>( L,  "GetFrameLock",                    &MONSTER::GetFrameLock  );
        lua_tinker::class_def<MONSTER>( L,  "SetFrameLock",                    &MONSTER::SetFrameLock  );
        lua_tinker::class_def<MONSTER>( L,  "GetIsContact",                    &MONSTER::GetIsContact  );
        lua_tinker::class_def<MONSTER>( L,  "SetIsContact",                    &MONSTER::SetIsContact  );
        lua_tinker::class_def<MONSTER>( L,  "ResetDelay",                      &MONSTER::ResetDelay  );
        lua_tinker::class_def<MONSTER>( L,  "SetSuperArmor",                   &MONSTER::SetSuperArmor  );
        lua_tinker::class_def<MONSTER>( L,  "SetSuperArmorFrame",              &MONSTER::SetSuperArmorFrame  );
        lua_tinker::class_def<MONSTER>( L,  "IsSuperArmor",                    &MONSTER::IsSuperArmor  );
        lua_tinker::class_def<MONSTER>( L,  "SetInvincible",                   &MONSTER::SetInvincible  );
        lua_tinker::class_def<MONSTER>( L,  "SetInvincibleFrame",              &MONSTER::SetInvincibleFrame  );
        lua_tinker::class_def<MONSTER>( L,  "IsInvincible",                    &MONSTER::IsInvincible  );
        lua_tinker::class_def<MONSTER>( L,  "SetNoCheckContact",               &MONSTER::SetNoCheckContact  );
        lua_tinker::class_def<MONSTER>( L,  "SetDownDelay",                    &MONSTER::SetDownDelay  );
        lua_tinker::class_def<MONSTER>( L,  "GetDownDelay",                    &MONSTER::GetDownDelay  );

        lua_tinker::class_def<MONSTER>( L,  "SetTarget",                       &MONSTER::SetTarget  );
        lua_tinker::class_def<MONSTER>( L,  "SetTargetToPlayer",               &MONSTER::SetTargetToPlayer  );
        lua_tinker::class_def<MONSTER>( L,  "SetTargetToMonster",              &MONSTER::SetTargetToMonster  );
        lua_tinker::class_def<MONSTER>( L,  "SetStaticTarget",                 &MONSTER::SetStaticTarget );
        lua_tinker::class_def<MONSTER>( L,  "IsTarget",                        &MONSTER::IsTarget  );
        lua_tinker::class_def<MONSTER>( L,  "IsTargetMonster",                 &MONSTER::IsTargetMonster  );
        lua_tinker::class_def<MONSTER>( L,  "IsTargetPlayer",                  &MONSTER::IsTargetPlayer  );
            
        lua_tinker::class_def<MONSTER>( L,  "TurnToTarget",                    &MONSTER::TurnToTarget  );
        lua_tinker::class_def<MONSTER>( L,  "GetTargetX",                      &MONSTER::GetTargetX  );
        lua_tinker::class_def<MONSTER>( L,  "GetTargetY",                      &MONSTER::GetTargetY  );	
        lua_tinker::class_def<MONSTER>( L,  "GetTargetIsContact",              &MONSTER::GetTargetIsContact  );
        lua_tinker::class_def<MONSTER>( L,  "GetTargetIsRight",                &MONSTER::GetTargetIsRight  );
        lua_tinker::class_def<MONSTER>( L,  "CheckTargetInRange",              &MONSTER::CheckTargetInRange  );

        lua_tinker::class_def<MONSTER>( L,  "GetMonsterTarget",                &MONSTER::GetMonsterTarget  );
		lua_tinker::class_def<MONSTER>( L,  "GetPlayerTarget",                 &MONSTER::GetPlayerTarget  );
        lua_tinker::class_def<MONSTER>( L,  "SetMonsterTargetManual",          &MONSTER::SetMonsterTargetManual  );
        lua_tinker::class_def<MONSTER>( L,  "SetPlayerTargetManual",           &MONSTER::SetPlayerTargetManual  );


        lua_tinker::class_def<MONSTER>( L,  "IsBoxIn",                         &MONSTER::IsBoxIn  );                
        
        lua_tinker::class_def<MONSTER>( L,  "GetX",                            &MONSTER::GetX  );
        lua_tinker::class_def<MONSTER>( L,  "GetY",                            &MONSTER::GetY  );
        lua_tinker::class_def<MONSTER>( L,  "SetX",                            &MONSTER::SetX  );
        lua_tinker::class_def<MONSTER>( L,  "SetY",                            &MONSTER::SetY  );
        lua_tinker::class_def<MONSTER>( L,  "SetSpeedX",                       &MONSTER::SetSpeedX  );
        lua_tinker::class_def<MONSTER>( L,  "SetSpeedY",                       &MONSTER::SetSpeedY  );
        lua_tinker::class_def<MONSTER>( L,  "GetSpeedX",                       &MONSTER::GetSpeedX  );
        lua_tinker::class_def<MONSTER>( L,  "GetSpeedY",                       &MONSTER::GetSpeedY  );
        lua_tinker::class_def<MONSTER>( L,  "GetWalkSpeed",                    &MONSTER::GetWalkSpeed  );
        lua_tinker::class_def<MONSTER>( L,  "GetRunSpeed",                     &MONSTER::GetRunSpeed  );
        lua_tinker::class_def<MONSTER>( L,  "SetIsRight",                      &MONSTER::SetIsRight  );
        lua_tinker::class_def<MONSTER>( L,  "GetIsRight",                      &MONSTER::GetIsRight  );
		lua_tinker::class_def<MONSTER>( L,  "GetLevel", 						&MONSTER::GetLevel  );

        lua_tinker::class_def<MONSTER>( L,  "FloatAbs",                        &MONSTER::FloatAbs  );
        lua_tinker::class_def<MONSTER>( L,  "FloatRand",                       &MONSTER::FloatRand  );
        lua_tinker::class_def<MONSTER>( L,  "StartText",                       &MONSTER::StartText  );
        lua_tinker::class_def<MONSTER>( L,  "EarthQuake",                      &MONSTER::EarthQuake  );
        lua_tinker::class_def<MONSTER>( L,  "EarthQuakeWithRange",             &MONSTER::EarthQuakeWithRange  );

        lua_tinker::class_def<MONSTER>( L,  "SetAttackBoundBox",               &MONSTER::SetAttackBoundBox  );
        lua_tinker::class_def<MONSTER>( L,  "SetPushBoundBox",                 &MONSTER::SetPushBoundBox  );
        
		lua_tinker::class_def<MONSTER>( L,  "StartMonster",                    &MONSTER::StartMonster  );
        lua_tinker::class_def<MONSTER>( L,  "EndMonster",                      &MONSTER::EndMonster  );
        lua_tinker::class_def<MONSTER>( L,  "SetHPALLMonster",                 &MONSTER::SetHPALLMonster  );
        lua_tinker::class_def<MONSTER>( L,  "Turn",                            &MONSTER::Turn  );
        lua_tinker::class_def<MONSTER>( L,  "RegistReaction",                  &MONSTER::RegistReaction  );
        lua_tinker::class_def<MONSTER>( L,  "IsBackAttack",                    &MONSTER::IsBackAttack  );

        lua_tinker::class_def<MONSTER>( L,  "GetCountValue",                   &MONSTER::GetCountValue  );
        lua_tinker::class_def<MONSTER>( L,  "SetCountValue",                   &MONSTER::SetCountValue  );
        lua_tinker::class_def<MONSTER>( L,  "AddCountValue",                   &MONSTER::AddCountValue  );

        lua_tinker::class_def<MONSTER>( L,  "SetPushCheck",                    &MONSTER::SetPushCheck  );
		lua_tinker::class_def<MONSTER>( L,  "GetPushCheck",                    &MONSTER::GetPushCheck  );
		
        lua_tinker::class_def<MONSTER>( L,  "SetAttackCheck",                  &MONSTER::SetAttackCheck  );
		lua_tinker::class_def<MONSTER>( L,  "GetAttackCheck",                  &MONSTER::GetAttackCheck  );
		
        lua_tinker::class_def<MONSTER>( L,  "ReleaseTarget",                   &MONSTER::ReleaseTarget  );
        lua_tinker::class_def<MONSTER>( L,  "GetHP",                           &MONSTER::GetHP  );
        lua_tinker::class_def<MONSTER>( L,  "SetHP",                           &MONSTER::SetHP  );
        lua_tinker::class_def<MONSTER>( L,  "GetMaxHP",                        &MONSTER::GetMaxHP  );

        lua_tinker::class_def<MONSTER>( L,  "AddDamage",                       &MONSTER::AddDamage  );		
        lua_tinker::class_def<MONSTER>( L,  "AddDamageLastPlayers",            &MONSTER::AddDamageLastPlayers  );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithAngle",              &MONSTER::AddDamageWithAngle  );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithDirectionAngle",     &MONSTER::AddDamageWithDirectionAngle  );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithLocate",             &MONSTER::AddDamageWithLocate  );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithStatic",             &MONSTER::AddDamageWithStatic  );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithStatic2",            &MONSTER::AddDamageWithStatic2  );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithLocateAngle",        &MONSTER::AddDamageWithLocateAngle  );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithLocateDirect",       &MONSTER::AddDamageWithLocateDirect );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageToAlivePlayer",          &MONSTER::AddDamageToAlivePlayer );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithTargetPos",          &MONSTER::AddDamageWithTargetPos );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithSpeed",              &MONSTER::AddDamageWithSpeed );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithStaticAndSpeed",     &MONSTER::AddDamageWithStaticAndSpeed );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithLocateAndIdentifie", &MONSTER::AddDamageWithLocateAndIdentifie );
        lua_tinker::class_def<MONSTER>( L,  "AddDamageWithPacket",             &MONSTER::AddDamageWithPacket );
        lua_tinker::class_def<MONSTER>( L,  "IsLiveDamageIdentifie",           &MONSTER::IsLiveDamageIdentifie );

        lua_tinker::class_def<MONSTER>( L,  "ToggleRender",                    &MONSTER::ToggleRender  );
        lua_tinker::class_def<MONSTER>( L,  "CheckPreState",                   &MONSTER::CheckPreState  );
        lua_tinker::class_def<MONSTER>( L,  "DirectMotionInput",               &MONSTER::DirectMotionInput  );
        lua_tinker::class_def<MONSTER>( L,  "CountPlayerInRect",               &MONSTER::CountPlayerInRect  );

        lua_tinker::class_def<MONSTER>( L,  "SetFly",                          &MONSTER::SetFly  );
        lua_tinker::class_def<MONSTER>( L,  "GetFly",                          &MONSTER::GetFly  );

        lua_tinker::class_def<MONSTER>( L,  "AddParticle",                     &MONSTER::AddParticle  );
        lua_tinker::class_def<MONSTER>( L,  "AddParticleArg",                  &MONSTER::AddParticleArg  );
        lua_tinker::class_def<MONSTER>( L,  "AddTraceParticle",                &MONSTER::AddTraceParticle  );
        lua_tinker::class_def<MONSTER>( L,  "AddTraceParticleToBone",          &MONSTER::AddTraceParticleToBone  );
		lua_tinker::class_def<MONSTER>( L,  "AddTraceParticleOffset", 		   &MONSTER::AddTraceParticleOffset  );
		lua_tinker::class_def<MONSTER>( L,  "AddTraceParticlePos", 		       &MONSTER::AddTraceParticlePos  );
        lua_tinker::class_def<MONSTER>( L,  "AddTraceParticleDieToClear",      &MONSTER::AddTraceParticleDieToClear );
        
        lua_tinker::class_def<MONSTER>( L,  "AddParticleNoDirection",          &MONSTER::AddParticleNoDirection  );
        lua_tinker::class_def<MONSTER>( L,  "AddTraceParticleNoDirection",     &MONSTER::AddTraceParticleNoDirection  );
        
        lua_tinker::class_def<MONSTER>( L,  "AddOvejarCloneDeathCount",        &MONSTER::AddOvejarCloneDeathCount );
        lua_tinker::class_def<MONSTER>( L,  "ResetOvejarCloneDeathCount",      &MONSTER::ResetOvejarCloneDeathCount );

        lua_tinker::class_def<MONSTER>( L,  "ReverseGravity",                  &MONSTER::ReverseGravity  );
        lua_tinker::class_def<MONSTER>( L,  "SetIgnoreFHCheck",                &MONSTER::SetIgnoreFHCheck  );
        lua_tinker::class_def<MONSTER>( L,  "KamikiLightning",                 &MONSTER::KamikiLightning  );
        lua_tinker::class_def<MONSTER>( L,  "KamikiLightningReady",            &MONSTER::KamikiLightningReady  );

        lua_tinker::class_def<MONSTER>( L,  "SkillReadyEffect",                &MONSTER::SkillReadyEffect  );
        lua_tinker::class_def<MONSTER>( L,  "GetSummonedMonsterCount",         &MONSTER::GetSummonedMonsterCount  );
        lua_tinker::class_def<MONSTER>( L,  "GetSummonedMonsterCountforType",  &MONSTER::GetSummonedMonsterCountforType  );
        lua_tinker::class_def<MONSTER>( L,  "SummonMonster",                   &MONSTER::SummonMonster  );
        lua_tinker::class_def<MONSTER>( L,  "SummonMonsterByLocal",            &MONSTER::SummonMonsterByLocal  );
        lua_tinker::class_def<MONSTER>( L,  "RegisterSummonMonsterType",       &MONSTER::RegisterSummonMonsterType  );

        lua_tinker::class_def<MONSTER>( L,  "GetAngleToTarget",                &MONSTER::GetAngleToTarget  );
        lua_tinker::class_def<MONSTER>( L,  "ComputeAngleToTarget",            &MONSTER::ComputeAngleToTarget  );

        lua_tinker::class_def<MONSTER>( L,  "CameraVib",                       &MONSTER::CameraVib  );
        lua_tinker::class_def<MONSTER>( L,  "FreezeAttack",                    &MONSTER::FreezeAttack  );
        lua_tinker::class_def<MONSTER>( L,  "SkillReadyEffectNoStop",          &MONSTER::SkillReadyEffectNoStop  );
        lua_tinker::class_def<MONSTER>( L,  "AddBlackHoleEffect",              &MONSTER::AddBlackHoleEffect  );
        lua_tinker::class_def<MONSTER>( L,  "PullPlayer",                      &MONSTER::PullPlayer  );
        lua_tinker::class_def<MONSTER>( L,  "KazeazeBlackBall",                &MONSTER::KazeazeBlackBall  );
        lua_tinker::class_def<MONSTER>( L,  "Fear",                            &MONSTER::Fear  );
        lua_tinker::class_def<MONSTER>( L,  "IsLive",                          &MONSTER::IsLive  );
        lua_tinker::class_def<MONSTER>( L,  "GetOvejarCloneDeathCount",        &MONSTER::GetOvejarCloneDeathCount );
		lua_tinker::class_def<MONSTER>( L,  "SetLive", 						&MONSTER::SetLive  );
        lua_tinker::class_def<MONSTER>( L,  "GetMonsterType",                  &MONSTER::GetMonsterType  );
        lua_tinker::class_def<MONSTER>( L,  "SetMeshRender",                   &MONSTER::SetMeshRender  );
        lua_tinker::class_def<MONSTER>( L,  "GetMeshRender",                   &MONSTER::GetMeshRender  );
        
        
        lua_tinker::class_def<MONSTER>( L,  "IsLastBottom",                    &MONSTER::IsLastBottom  );
        lua_tinker::class_def<MONSTER>( L,  "FlashScreen",                     &MONSTER::FlashScreen );
		lua_tinker::class_def<MONSTER>( L,  "SetChampionProperty", 			&MONSTER::SetChampionProperty  );
		lua_tinker::class_def<MONSTER>( L,  "GetChanpionProperty", 			&MONSTER::GetChampionProperty  );
		lua_tinker::class_def<MONSTER>( L,  "SetMovement", 					&MONSTER::SetMovement  );
		lua_tinker::class_def<MONSTER>( L,  "ActionCallBack", 					&MONSTER::LuaActionCallBack  );

        lua_tinker::class_def<MONSTER>( L,  "SetDrawSlash", 					&MONSTER::SetDrawSlash  );        
        lua_tinker::class_def<MONSTER>( L,  "GetIsContactByPos", 				&MONSTER::GetIsContactByPos  );        
        lua_tinker::class_def<MONSTER>( L,  "GetTraceParticleNum", 			&MONSTER::GetTraceParticleNum  );        

        lua_tinker::class_def<MONSTER>( L,  "GetBonePos", 			            &MONSTER::GetBonePos  );
        lua_tinker::class_def<MONSTER>( L,  "SetTitleText", 			        &MONSTER::SetTitleText  );
        lua_tinker::class_def<MONSTER>( L,  "GetStageX", 						&MONSTER::GetStageX  );
        lua_tinker::class_def<MONSTER>( L,  "GetStageY", 						&MONSTER::GetStageY  );
        lua_tinker::class_def<MONSTER>( L,  "GetTimer", 						&MONSTER::GetTimer  );
        lua_tinker::class_def<MONSTER>( L,  "InitTimer", 						&MONSTER::InitTimer  );
        lua_tinker::class_def<MONSTER>( L,  "StartTimer", 						&MONSTER::StartTimer  );
        lua_tinker::class_def<MONSTER>( L,  "StopTimer", 						&MONSTER::StopTimer  );
        lua_tinker::class_def<MONSTER>( L,  "SetTraceIndex", 					&MONSTER::SetTraceIndex  );
        lua_tinker::class_def<MONSTER>( L,  "SetTrace", 						&MONSTER::SetTrace  );
        lua_tinker::class_def<MONSTER>( L,  "SetTraceGap", 					    &MONSTER::SetTraceGap  );
        lua_tinker::class_def<MONSTER>( L,  "SetTracePercent", 				    &MONSTER::SetTracePercent  );
        lua_tinker::class_def<MONSTER>( L,  "SetBodyAngle", 				    &MONSTER::SetBodyAngle  );
        lua_tinker::class_def<MONSTER>( L,  "GetBodyAngle", 				    &MONSTER::GetBodyAngle  );
        lua_tinker::class_def<MONSTER>( L,  "SetUserBoneAngle", 				&MONSTER::SetUserBoneAngle  );
        lua_tinker::class_def<MONSTER>( L,  "GetUserBoneAngle", 				&MONSTER::GetUserBoneAngle  );
        lua_tinker::class_def<MONSTER>( L,  "SetMagicEffect", 				    &MONSTER::SetMagicEffect  );
        lua_tinker::class_def<MONSTER>( L,  "SetMagicMonsterALLEffect", 		&MONSTER::SetMagicMonsterALLEffect  );
        lua_tinker::class_def<MONSTER>( L,  "GetBuffLevel", 				    &MONSTER::GetBuffLevel  );
        lua_tinker::class_def<MONSTER>( L,  "IsMagicEffect", 				    &MONSTER::IsMagicEffect  );
        lua_tinker::class_def<MONSTER>( L,  "ClearMagicEffect", 				&MONSTER::ClearMagicEffect  );
        lua_tinker::class_def<MONSTER>( L,  "ClearMagicPlayerAllEffect", 		&MONSTER::ClearMagicPlayerAllEffect  );
        lua_tinker::class_def<MONSTER>( L,  "ClearMagicMonsterALLEffect", 		&MONSTER::ClearMagicMonsterALLEffect  );
        lua_tinker::class_def<MONSTER>( L,  "GetMagicEffectTime", 		        &MONSTER::GetMagicEffectTime  );



		lua_tinker::class_def<MONSTER>( L, "ToggleExtraMesh", 					&MONSTER::ToggleExtraMesh  );
		lua_tinker::class_def<MONSTER>( L, "ToggleExtraMeshAll", 				&MONSTER::ToggleExtraMeshAll  );
		lua_tinker::class_def<MONSTER>( L, "ToggleBaseMesh", 					&MONSTER::ToggleBaseMesh  );		
		lua_tinker::class_def<MONSTER>( L, "SetChildMeshMotion",				&MONSTER::SetChildMeshMotion );
        lua_tinker::class_def<MONSTER>( L, "SetChildMeshMotion_Front",			&MONSTER::SetChildMeshMotion_Front );
        lua_tinker::class_def<MONSTER>( L, "ClearChildMeshOneTimeMotion",	    &MONSTER::ClearChildMeshOneTimeMotion );
        lua_tinker::class_def<MONSTER>( L, "AddParticleToChildMeshBone",	    &MONSTER::AddParticleToChildMeshBone );

        lua_tinker::class_def<MONSTER>( L, "GetGameMode",	                    &MONSTER::GetGameMode );
        lua_tinker::class_def<MONSTER>( L, "SetTargetingDelayTime",	            &MONSTER::SetTargetingDelayTime );
        lua_tinker::class_def<MONSTER>( L, "GetSpecialType",	                &MONSTER::GetSpecialType );
        lua_tinker::class_def<MONSTER>( L, "SendDropInfo",	                    &MONSTER::SendDropInfo );
        lua_tinker::class_def<MONSTER>( L, "SendMonsterKill",	                &MONSTER::SendMonsterKill );
        lua_tinker::class_def<MONSTER>( L, "SetShield",	                        &MONSTER::SetShield );
        lua_tinker::class_def<MONSTER>( L, "GetShield",	                        &MONSTER::GetShield );
        lua_tinker::class_def<MONSTER>( L, "SetShieldEffect",	                &MONSTER::SetShieldEffect );

        lua_tinker::class_def<MONSTER>( L, "GetLastAttackedMe",	                &MONSTER::GetLastAttackedMe );
		lua_tinker::class_def<MONSTER>( L, "GetHitPos",	                &MONSTER::GetHitPos );
        lua_tinker::class_def<MONSTER>( L, "EffectDust",	                    &MONSTER::EffectDust );
        lua_tinker::class_def<MONSTER>( L, "CheckSummonMotherDist",             &MONSTER::CheckSummonMotherDist  );
        lua_tinker::class_def<MONSTER>( L, "TurnToMother",                      &MONSTER::TurnToMother  );
        lua_tinker::class_def<MONSTER>( L, "SameTurnToMother",                  &MONSTER::SameTurnToMother  );
        lua_tinker::class_def<MONSTER>( L, "GetSummonPlayerUID",                &MONSTER::GetSummonPlayerUID  );
        lua_tinker::class_def<MONSTER>( L, "ToggleMonsterInfo",                &MONSTER::ToggleMonsterInfo  );
        lua_tinker::class_def<MONSTER>( L, "SetPlayerMonster",                &MONSTER::SetPlayerMonster  );
        lua_tinker::class_def<MONSTER>( L, "CheckStandBlock",                 &MONSTER::CheckStandBlock  );
        lua_tinker::class_def<MONSTER>( L, "CheckSideBlock",                 &MONSTER::CheckSideBlock  );
        lua_tinker::class_def<MONSTER>( L, "GetMotherPos",                    &MONSTER::GetMotherPos  );
        lua_tinker::class_def<MONSTER>( L, "GetSummonMonsterDieType",         &MONSTER::GetSummonMonsterDieType );
        lua_tinker::class_def<MONSTER>( L, "SetDamageLife",                    &MONSTER::SetDamageLife );
        lua_tinker::class_def<MONSTER>( L, "IsDamageLife",                    &MONSTER::IsDamageLife );
        lua_tinker::class_def<MONSTER>( L, "GetDotaGateIsRight",              &MONSTER::GetDotaGateIsRight );
        lua_tinker::class_def<MONSTER>( L, "AddParticleModel",              &MONSTER::AddParticleModel );
        lua_tinker::class_def<MONSTER>( L, "GetInitAI",                     &MONSTER::GetInitAI );
        lua_tinker::class_def<MONSTER>( L, "SetInitAI",                     &MONSTER::SetInitAI );
        lua_tinker::class_def<MONSTER>( L, "IsTargetMonsterType",           &MONSTER::IsTargetMonsterType );
        lua_tinker::class_def<MONSTER>( L, "GetPlayerPosByMinDistance",     &MONSTER::GetPlayerPosByMinDistance );
        lua_tinker::class_def<MONSTER>( L, "IsPlatformer",           &MONSTER::IsPlatformer );

        lua_tinker::class_def<MONSTER>( L, "SetAgitCharCloseMent",           &MONSTER::SetAgitCharCloseMent );
        lua_tinker::class_def<MONSTER>( L, "CheckSummonSlot",               &MONSTER::CheckSummonSlot );
        lua_tinker::class_def<MONSTER>( L, "CheckLiveMonster",               &MONSTER::CheckLiveMonster );
        lua_tinker::class_def<MONSTER>( L, "IsHost",                        &MONSTER::IsHost );
        lua_tinker::class_def<MONSTER>( L, "SetMagicPlayerEffect",          &MONSTER::SetMagicPlayerEffect );
        lua_tinker::class_def<MONSTER>( L, "SetMagicPlayerEffectInRange",   &MONSTER::SetMagicPlayerEffectInRange );
        lua_tinker::class_def<MONSTER>( L, "ClearMagicPlayerEffect",        &MONSTER::ClearMagicPlayerEffect );

        lua_tinker::class_def<MONSTER>( L, "SetMaxHP",                      &MONSTER::SetMaxHP );
        lua_tinker::class_def<MONSTER>( L, "GetAttackPoint",                &MONSTER::GetAttackPoint );
        lua_tinker::class_def<MONSTER>( L, "SetAttackPoint",                &MONSTER::SetAttackPoint );
        lua_tinker::class_def<MONSTER>( L, "GetDamageDirection",            &MONSTER::GetDamageDirection );
        lua_tinker::class_def<MONSTER>( L, "IsSpecialAttack",               &MONSTER::IsSpecialAttack );
        lua_tinker::class_def<MONSTER>( L, "DeleteAllMyDamageToType",       &MONSTER::DeleteAllMyDamageToType );
        lua_tinker::class_def<MONSTER>( L, "IsMyDamageToType",              &MONSTER::IsMyDamageToType );
        lua_tinker::class_def<MONSTER>( L, "GetFrame",                      &MONSTER::GetFrame );
        lua_tinker::class_def<MONSTER>( L, "GetDamageDiff",                 &MONSTER::GetDamageDiff );
        lua_tinker::class_def<MONSTER>( L, "SetDamageDiff",                 &MONSTER::SetDamageDiff );
        lua_tinker::class_def<MONSTER>( L, "CheckPierceBlockXPos",          &MONSTER::CheckPierceBlockXPos );
        lua_tinker::class_def<MONSTER>( L, "SetDie",                        &MONSTER::SetDie );
        lua_tinker::class_def<MONSTER>( L, "ChangeTraceIcon",               &MONSTER::ChangeTraceIcon );
        lua_tinker::class_def<MONSTER>( L, "ResetTraceIcon",                &MONSTER::ResetTraceIcon );
        lua_tinker::class_def<MONSTER>( L, "SetTraceIconExistScreen",       &MONSTER::SetTraceIconExistScreen );
        lua_tinker::class_def<MONSTER>( L, "IsTargetDamage",                &MONSTER::IsTargetDamage );
        lua_tinker::class_def<MONSTER>( L, "TurnToDamageTarget",            &MONSTER::TurnToDamageTarget );
        lua_tinker::class_def<MONSTER>( L, "GetTargetDamagePos",            &MONSTER::GetTargetDamagePos );
        lua_tinker::class_def<MONSTER>( L, "SetDamageTarget",               &MONSTER::SetDamageTarget );
        lua_tinker::class_def<MONSTER>( L, "SetDamageTargetAllRange",       &MONSTER::SetDamageTargetAllRange );
        lua_tinker::class_def<MONSTER>( L, "CheckDamageTarget",             &MONSTER::CheckDamageTarget );
        lua_tinker::class_def<MONSTER>( L, "CheckDamageTargetInRange",      &MONSTER::CheckDamageTargetInRange );
        lua_tinker::class_def<MONSTER>( L, "DeleteTargetDamage",            &MONSTER::DeleteTargetDamage );
        lua_tinker::class_def<MONSTER>( L, "GetDamageInstanceCount",        &MONSTER::GetDamageInstanceCount );
        lua_tinker::class_def<MONSTER>( L, "ReleaseTargetDamage",           &MONSTER::ReleaseTargetDamage );
        lua_tinker::class_def<MONSTER>( L, "IsTargetSyncObject",            &MONSTER::IsTargetSyncObject );
        lua_tinker::class_def<MONSTER>( L, "TurnToSyncObjectTarget",        &MONSTER::TurnToSyncObjectTarget );
        lua_tinker::class_def<MONSTER>( L, "GetTargetSyncObjectPos",        &MONSTER::GetTargetSyncObjectPos );
        lua_tinker::class_def<MONSTER>( L, "SetSyncObjectTarget",           &MONSTER::SetSyncObjectTarget );
        lua_tinker::class_def<MONSTER>( L, "SetSyncObjectTargetAllRange",   &MONSTER::SetSyncObjectTargetAllRange );
        lua_tinker::class_def<MONSTER>( L, "CheckSyncObjectTarget",         &MONSTER::CheckSyncObjectTarget );
        lua_tinker::class_def<MONSTER>( L, "CheckSyncObjectTargetInRange",  &MONSTER::CheckSyncObjectTargetInRange );
        lua_tinker::class_def<MONSTER>( L, "GetSyncObjectInstanceCount",    &MONSTER::GetSyncObjectInstanceCount );
        lua_tinker::class_def<MONSTER>( L, "ReleaseTargetSyncObject",       &MONSTER::ReleaseTargetSyncObject );
        lua_tinker::class_def<MONSTER>( L, "ShowEscortHPBar",               &MONSTER::ShowEscortHPBar );
        lua_tinker::class_def<MONSTER>( L, "SelectEscortHPBarImg",          &MONSTER::SelectEscortHPBarImg );
        lua_tinker::class_def<MONSTER>( L, "CheckEnemyInRange",             &MONSTER::CheckEnemyInRange );
        lua_tinker::class_def<MONSTER>( L, "CheckMonsterInRange",           &MONSTER::CheckMonsterInRange );
        lua_tinker::class_def<MONSTER>( L, "CheckPlayerInRange",            &MONSTER::CheckPlayerInRange );
        lua_tinker::class_def<MONSTER>( L, "CheckNearFootHold",             &MONSTER::CheckNearFootHold );
        lua_tinker::class_def<MONSTER>( L, "CheckPosInRange",               &MONSTER::CheckPosInRange );
        lua_tinker::class_def<MONSTER>( L, "SetTempTargetPos",              &MONSTER::SetTempTargetPos );
        lua_tinker::class_def<MONSTER>( L, "GetTempTargetPos",              &MONSTER::GetTempTargetPos );
        lua_tinker::class_def<MONSTER>( L, "IsTempTarget",                  &MONSTER::IsTempTarget );
        lua_tinker::class_def<MONSTER>( L, "ReleaseTempTargetPos",          &MONSTER::ReleaseTempTargetPos );

        lua_tinker::class_def<MONSTER>( L, "Setv3TempValue",                &MONSTER::Setv3TempValue );
        lua_tinker::class_def<MONSTER>( L, "Getv3TempValue",                &MONSTER::Getv3TempValue);
        
        lua_tinker::class_def<MONSTER>( L, "GetVectorDistance",             &MONSTER::GetVectorDistance );
        lua_tinker::class_def<MONSTER>( L, "TurnToTempTarget",              &MONSTER::TurnToTempTarget );
        lua_tinker::class_def<MONSTER>( L, "AddParticleBorn",               &MONSTER::AddParticleBorn );
        lua_tinker::class_def<MONSTER>( L, "StartAfterImage",               &MONSTER::StartAfterImage );
        lua_tinker::class_def<MONSTER>( L, "EndAfterImage",                 &MONSTER::EndAfterImage );
        lua_tinker::class_def<MONSTER>( L, "SetTargetAllRange",             &MONSTER::SetTargetAllRange );
        lua_tinker::class_def<MONSTER>( L, "GetEnemyDistAllRange",          &MONSTER::GetEnemyDistAllRange );
        lua_tinker::class_def<MONSTER>( L, "EffectiveTarget",               &MONSTER::EffectiveTarget );
        lua_tinker::class_def<MONSTER>( L, "GetMaxPlayer",                  &MONSTER::GetMaxPlayer );
        lua_tinker::class_def<MONSTER>( L, "GetMaxMonster",                 &MONSTER::GetMaxMonster );
        lua_tinker::class_def<MONSTER>( L, "EffectivePlayer",               &MONSTER::EffectivePlayer );
        lua_tinker::class_def<MONSTER>( L, "EffectiveMonster",              &MONSTER::EffectiveMonster );
        lua_tinker::class_def<MONSTER>( L, "GetEnemyPlayerPos",             &MONSTER::GetEnemyPlayerPos );
        lua_tinker::class_def<MONSTER>( L, "GetEnemyMonsterPos",            &MONSTER::GetEnemyMonsterPos );
        lua_tinker::class_def<MONSTER>( L, "IsMagicEffectCount",            &MONSTER::IsMagicEffectCount );
        lua_tinker::class_def<MONSTER>( L, "ClearTraceIcon",                &MONSTER::ClearTraceIcon );
        lua_tinker::class_def<MONSTER>( L, "IsNoChangeState",               &MONSTER::IsNoChangeState );
        lua_tinker::class_def<MONSTER>( L, "SetNoChangeState",              &MONSTER::SetNoChangeState );
        lua_tinker::class_def<MONSTER>( L, "SummonMonsterStaticSlot",       &MONSTER::SummonMonsterStaticSlot );
        lua_tinker::class_def<MONSTER>( L, "StartBlur",                     &MONSTER::StartBlur );
        lua_tinker::class_def<MONSTER>( L, "EndBlur",                       &MONSTER::EndBlur );
        lua_tinker::class_def<MONSTER>( L, "IsDie",                         &MONSTER::IsDie );
        lua_tinker::class_def<MONSTER>( L, "SetNoCrashDamage",              &MONSTER::SetNoCrashDamage );
        lua_tinker::class_def<MONSTER>( L, "GetNoCrashDamage",              &MONSTER::GetNoCrashDamage );
        lua_tinker::class_def<MONSTER>( L, "SetMagicEffect_NotMeBroad",     &MONSTER::SetMagicEffect_NotMeBroad );
        lua_tinker::class_def<MONSTER>( L, "ClearMagicEffect_NotMeBroad",   &MONSTER::ClearMagicEffect_NotMeBroad );
		lua_tinker::class_def<MONSTER>( L, "GetMonsterIndex",               &MONSTER::GetMonsterIndex );
		lua_tinker::class_def<MONSTER>( L, "SetSummonNumberAvailable",      &MONSTER::SetSummonNumberAvailable );
        lua_tinker::class_def<MONSTER>( L, "OnReverseGravity",              &MONSTER::OnReverseGravity );
		
        lua_tinker::class_def<MONSTER>( L, "GetMapTempValue",               &MONSTER::GetMapTempValue );
        lua_tinker::class_def<MONSTER>( L, "SetMapTempValue",               &MONSTER::SetMapTempValue );
        lua_tinker::class_def<MONSTER>( L, "GetPlayerPos",                  &MONSTER::GetPlayerPos );
        lua_tinker::class_def<MONSTER>( L, "GetPlayerIsRight",              &MONSTER::GetPlayerIsRight );
        lua_tinker::class_def<MONSTER>( L, "CheckSameTeamInRange",          &MONSTER::CheckSameTeamInRange );
        lua_tinker::class_def<MONSTER>( L, "ClearALLMagicEffect",           &MONSTER::ClearALLMagicEffect );
        lua_tinker::class_def<MONSTER>( L, "ClearMagicEffect_AllMonster",   &MONSTER::ClearMagicEffect_AllMonster );
        lua_tinker::class_def<MONSTER>( L, "SetTexture",                    &MONSTER::SetTexture );
        lua_tinker::class_def<MONSTER>( L, "GetSelectTexture",              &MONSTER::GetSelectTexture );
        lua_tinker::class_def<MONSTER>( L, "SetNonShakeEffect",             &MONSTER::SetNonShakeEffect );
        lua_tinker::class_def<MONSTER>( L, "IsOldGraphicQuality",           &MONSTER::IsOldGraphicQuality );
        lua_tinker::class_def<MONSTER>( L, "GetEnemyCntInRange",            &MONSTER::GetEnemyCntInRange );
        lua_tinker::class_def<MONSTER>( L, "GetUnitCntInRange",             &MONSTER::GetUnitCntInRange );
        lua_tinker::class_def<MONSTER>( L, "GetLiveEnemyCnt",               &MONSTER::GetLiveEnemyCnt );
        lua_tinker::class_def<MONSTER>( L, "GetLiveMonsterCnt",             &MONSTER::GetLiveMonsterCnt );
        lua_tinker::class_def<MONSTER>( L, "CheckLiveMonsterFromAllSlot",   &MONSTER::CheckLiveMonsterFromAllSlot );
        lua_tinker::class_def<MONSTER>( L, "SetTargetNearPlayer",           &MONSTER::SetTargetNearPlayer );
        lua_tinker::class_def<MONSTER>( L, "SetTargetFarPlayer",            &MONSTER::SetTargetFarPlayer );
        lua_tinker::class_def<MONSTER>( L, "GetLocalPlayer",                &MONSTER::GetLocalPlayer );
        lua_tinker::class_def<MONSTER>( L, "RenderHitNumber",               &MONSTER::RenderHitNumber );
        lua_tinker::class_def<MONSTER>( L, "SetDiffMonsterTargeting",       &MONSTER::SetDiffMonsterTargeting );
        lua_tinker::class_def<MONSTER>( L, "GetDiffMonsterTargeting",       &MONSTER::GetDiffMonsterTargeting );

        lua_tinker::class_def<MONSTER>( L, "SetRealMiniMap_ScaleAni",       &MONSTER::SetRealMiniMap_ScaleAni );
        lua_tinker::class_def<MONSTER>( L, "SetRealMiniMap_AlphaAni",       &MONSTER::SetRealMiniMap_AlphaAni );
        lua_tinker::class_def<MONSTER>( L, "SetRealMiniMap_ColorAni",       &MONSTER::SetRealMiniMap_ColorAni );
        lua_tinker::class_def<MONSTER>( L, "SetRealMiniMap_StaticScale",    &MONSTER::SetRealMiniMap_StaticScale );
        lua_tinker::class_def<MONSTER>( L, "SetRealMiniMap_BaseRender",     &MONSTER::SetRealMiniMap_BaseRender );
        lua_tinker::class_def<MONSTER>( L, "SetRealMiniMap_AddTexture",     &MONSTER::SetRealMiniMap_AddTexture );
        lua_tinker::class_def<MONSTER>( L, "SetUserCartoon",                &MONSTER::SetUserCartoon );
        lua_tinker::class_def<MONSTER>( L, "RequestChangeStateToHost",      &MONSTER::RequestChangeStateToHost );
        lua_tinker::class_def<MONSTER>( L, "GetRequestUserUID",             &MONSTER::GetRequestUserUID );
        lua_tinker::class_def<MONSTER>( L, "IsAIConnectKey",                &MONSTER::IsAIConnectKey );
        lua_tinker::class_def<MONSTER>( L, "SetDelay",                      &MONSTER::SetDelay );
        lua_tinker::class_def<MONSTER>( L, "SetNotCheckStateWall",          &MONSTER::SetNotCheckStateWall );
        lua_tinker::class_def<MONSTER>( L, "GetNotCheckStateWall",          &MONSTER::GetNotCheckStateWall );
        lua_tinker::class_def<MONSTER>( L, "SetZPos",                       &MONSTER::SetZPos );
        lua_tinker::class_def<MONSTER>( L, "SetBossDieCameraZoom",          &MONSTER::SetBossDieCameraZoom );
        lua_tinker::class_def<MONSTER>( L, "SetBossDieBlurTime",            &MONSTER::SetBossDieBlurTime );
        lua_tinker::class_def<MONSTER>( L, "IsBlur",                        &MONSTER::IsBlur );
        lua_tinker::class_def<MONSTER>( L, "SetMonsterStateStat",           &MONSTER::SetMonsterStateStat );
        lua_tinker::class_def<MONSTER>( L, "SetDamageWhat",                 &MONSTER::SetDamageWhat );
        lua_tinker::class_def<MONSTER>( L, "GetDamageWhat",                 &MONSTER::GetDamageWhat );
        lua_tinker::class_def<MONSTER>( L, "ShowMissAttack",                &MONSTER::ShowMissAttack );
        lua_tinker::class_def<MONSTER>( L, "ShowWarninggMessage",           &MONSTER::ShowWarninggMessage );
        lua_tinker::class_def<MONSTER>( L, "IsInScreen",                    &MONSTER::IsInScreen );
        lua_tinker::class_def<MONSTER>( L, "IsInScreen",                    &MONSTER::IsInScreen );
        lua_tinker::class_def<MONSTER>( L, "SetFlashEffectFrame",           &MONSTER::SetFlashEffectFrame );
        lua_tinker::class_def<MONSTER>( L, "SetShakeEffectFrame",           &MONSTER::SetShakeEffectFrame );
        lua_tinker::class_def<MONSTER>( L, "GetHPGaugeBar",                 &MONSTER::GetHPGaugeBar  ); 
        lua_tinker::class_def<MONSTER>( L, "SetMonsterTagetEffect",         &MONSTER::SetMonsterTagetEffect  ); 
        lua_tinker::class_def<MONSTER>( L, "SetMonsterAttackEffect",        &MONSTER::SetMonsterAttackEffect  ); 
        lua_tinker::class_def<MONSTER>( L, "GetShadowPos",                  &MONSTER::GetShadowPos  ); 
        lua_tinker::class_mem<MONSTER>( L, "m_vDamageStartPos",             &MONSTER::m_vDamageStartPos ); 
        lua_tinker::class_mem<MONSTER>( L, "m_vDamageWhoPos",               &MONSTER::m_vDamageWhoPos ); 
        lua_tinker::class_mem<MONSTER>( L, "m_bHPrenderSkip",               &MONSTER::m_bHPrenderSkip ); 
        lua_tinker::class_def<MONSTER>( L, "MultiplyBaseStats",             &MONSTER::MultiplyBaseStats );
        lua_tinker::class_def<MONSTER>( L, "IsMissionHellMode",             &MONSTER::IsMissionHellMode );
        lua_tinker::class_def<MONSTER>( L, "IsMissionBreakMode",            &MONSTER::IsMissionBreakMode );

#if !defined(__PATH__)
        lua_tinker::class_def<MONSTER>( L, "RenderBox",                     &MONSTER::RenderBox );
#endif
}

void MONSTER::PlaySound( char* strSound, int loop /*= 0*/ )
{
    g_KDSound.Play( strSound, loop, LATENCY_BUFFER_SIZE );
}

int MONSTER::GetLastFrame()
{
    std::map< std::string, KGCMonsterAction > mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mapIter = mapAction.begin();

    if( mapAction.size() > 0 )
    {
        std::string strMotion = g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_aiMotion[ LATENCY_INDEX] );
        std::map< std::string, KGCMonsterAction >::iterator mit = mapAction.find( strMotion );

        if( mit == mapAction.end() )
        {
            #if defined( ERROR_MESSAGE_BOX )
            {
                std::wstringstream strText;
                std::wstringstream strCaption;
                strText << L"( " << m_strMonsterName << L" " << strMotion.c_str() << L" ) 모션이 존재하지 않음";
                strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
                MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
            }
            #endif

            return 0;
        }

        const KGCMonsterAction &kMonsterAction = mit->second;

        if( (int)m_vecMotionList.size() <= kMonsterAction.m_iMotionIndex )
            return 0;

        return m_vecMotionList[ kMonsterAction.m_iMotionIndex ]->GetNum_Frame() - 1;
    }
    else
        return 0;
}

void MONSTER::SetState( const char* strState, bool bDirectMotion_ )
{
    if( strState == "" )
        return;

    if( m_bNoChangeState )
        return;

    if( m_bDie )
        return;

    std::string strMotion = strState;

    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mit = mapAction.find( strMotion );

    // ±×·± ½ºÅ×ÀÌÆ®°¡ ¾ø´Ù!! ½ºÅ©¸³Æ® Àß¸øÂ®À½..=¤µ =
    if( mit == mapAction.end() )
    {
        //#if defined( ERROR_MESSAGE_BOX )
//         {
//             std::wstringstream strText;
//             std::wstringstream strCaption;
//             strText << L"( " << m_strMonsterName << L" " << strState << L" ) ÇØ´ç ½ºÅ×ÀÌÆ®°¡ Á¸ÀçÇÏÁö ¾Ê½À´Ï´Ù";
//             strCaption << L"¿¡·¯³­ ÇÔ¼ö À§Ä¡ : " << __FUNCTION__;
//             MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
//         }
        //#endif

        return;
    }

    const KGCMonsterAction &kMonsterAction = mit->second;

    // ÀÌ°Å ¸ð¼Ç ÆÄÀÏ ¾øÀ» È®·üÀÌ ³ô´Ù..
    // ¸ð¼ÇÀÌ ¿Ã¹Ù¸£Áö ¾ÊÀºµ¥ ±¦È÷ ½ºÅ×ÀÌÆ® ÀÌµ¿ÇÏÁö¸»°í..
    // ÀÌ °æ¿ì¿¡ ±×³É return ÇØ¹ö·Á¼­ crash ³ª¼­ Á×Áö ¾Ê°Ô ÇØ ÁÖÀÚ..
	// m_iMotionIndex ¸ð¼Ç ¸®½ºÆ® »çÀÌÁîº¸´Ù Å©¸é NULL Ã¼Å©·Î ¾ÈÀâÈù´Ù.. - breadceo
    if( (int)m_vecMotionList.size() <= kMonsterAction.m_iMotionIndex || 
		kMonsterAction.m_iMotionIndex < 0 ||
		m_vecMotionList[ kMonsterAction.m_iMotionIndex ] == NULL ||
		m_vecMotionList[ kMonsterAction.m_iMotionIndex ]->IsDisable() )
    {
        //#if defined( ERROR_MESSAGE_BOX )
        //{
        //    std::wstringstream strText;
        //    std::wstringstream strCaption;
        //    strText << L"( " << m_strMonsterName << L" " << strState << L" ) 스테이트에 해당하는 파일이 없을 가능성이 많음..";
        //    strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
        //    //MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );

        //    g_pkChatManager->AddChatMsg( strText.str(), strCaption.str() );
        //}
        //#endif

        return;
    }

    // °ø°ÝÇÒ ¶§, ¶ß´Â ÀÌÆåÆ®! Áö±Ý ³Ê °ø°ÝÇÏ´Ï±î Àß ¾Ë¾Æ¸Ô¾î!! Á» Ä£ÀýÇÑµ¥ ¤»¤»
    // ·¾ÀÌ 34º¸´Ù ³·À¸¸é ¶ç¿öÁØ´Ù!
    if ( this->m_iLevel <= 34 && strMotion.find( "ATTACK" ) != string::npos && m_eNPC == NPC_NONE )
    {
        AddTraceParticleToBone( "Goblin_Shock_01", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, -(m_fWidth / 2.0f), m_fHeight + 0.05f, true );
        AddTraceParticleToBone( "Ppazik", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, -(m_fWidth / 2.0f), m_fHeight + 0.05f, true );
    }

    if( bDirectMotion_ )
    {

        int iFrame = 0;
        for( int i = SHOWLATENCY; i < LATENCY + SHOWLATENCY; ++i )
        {
            m_aiMotion[i] = kMonsterAction.m_iMotionIndex;
            m_aiFrame[i] = iFrame;

            if ( m_iSlowCount > 0 )
            {
                --m_iSlowCount;
                continue;
            }
            else if( iFrame < GetLastFrame_( i ) )
            {
                ++iFrame;
            }
        }

    }
    else
    {

        m_aiMotion[ LATENCY_INDEX ] = kMonsterAction.m_iMotionIndex;
        m_aiFrame[ LATENCY_INDEX ]  = 0;
    }

    m_bFrameLock                = false;
    m_bIsSuperArmor             = false;
    m_bAttackSuperArmor         = false;
    m_bIsInvincible             = false;



	std::vector<KMonsterSwoosh>::iterator vit;
    for( vit = m_vecSwoosh.begin() ; vit != m_vecSwoosh.end() ; vit++)
        vit->m_pEffect->Show( false );
    EndAttack();

    //printf( "MonsterState : %d %s\n", m_iMonsterType, GetState().c_str() );
}

std::string MONSTER::GetState()
{
    return g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_aiMotion[ LATENCY_INDEX ] );
}

bool MONSTER::IsMissionHellMode()
{
    return SiKGCRoomListManager()->IsHellMode();
}

bool MONSTER::IsMissionBreakMode()
{
    return SiKGCRoomListManager()->IsBreakMode();
}

void MONSTER::MultiplyBaseStats( float fMultiplier )
{
    m_fHPPoint   *=      fMultiplier;
    m_fHP         =       m_fHPPoint;
    m_fAtkRatio  += fMultiplier / 10;
    m_fDefRatio  += fMultiplier / 10;
    m_fSDefRatio += fMultiplier / 10;
}

void MONSTER::SetFlashEffectFrame( int iFlashEffectFrame )
{
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    if( kMonsterTemplate.bNoFlashEffect )
        return;

    if( g_MyD3D->m_KGCOption.IsHiteffect() )
    {
        m_iFlashEffectFrame = iFlashEffectFrame;
        if( g_MyD3D->m_KGCOption.IsCartoon() )
            m_iFlashEffectFrame += CARTOON_FLASH_EFFECT_FRAME;
    }
}

void MONSTER::SetShakeEffectFrame( int iShakeEffectFrame )
{
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    if( kMonsterTemplate.bNoFlashEffect )
        return;

    //if( g_MyD3D->m_KGCOption.IsHiteffect() )
    EGCMonster eMonster = GetDefaultMonster( m_iMonsterType );
    if ( eMonster != MON_BOSS_OBJECT )
    {
        m_iShakeEffectFrame = iShakeEffectFrame;
    }
}

void MONSTER::EffectDust( float fX, float fY, int iLatency/* = 0*/ )
{
    EGCGameMode eGameMode = SiKGCRoomManager()->GetGameMode();
   
    if( SiKGCFantasticMap()->IsWaterStage() && SiKGCFantasticMap()->GetO2Manager() )
    {
        if( SiKGCFantasticMap()->GetO2Manager()->IsInWaterMonster( m_iMonsterIndex ) ) // ¹°¾ÈÀÏ¶§..
            g_MyD3D->MySparks->WaterSmogs( fX, fX, iLatency );
    }
    else if( eGameMode == GC_GM_QUEST0 )    // ½Ã·ÃÀÇ ½£
    {
        g_MyD3D->MySparks->LeafSmogs( fX, fX, iLatency );
    }
    else if( eGameMode == GC_GM_QUEST9 ||   // ¸ÁÀÚÀÇ ¹¦Áö
        eGameMode == GC_GM_QUEST17 )        // Ä«½ºÅø ÇÏÃþ
    {
        g_MyD3D->MySparks->UndeadSmogs( fX, fX, iLatency );
    }
    else
    {
        g_MyD3D->MySparks->Smogs( fX, fX, iLatency );
    }
}

void MONSTER::SetStaticTarget( int iSlotIndex, bool bPlayer )
{
    if( bPlayer )
    {
        SetTargetPlayerIndex( iSlotIndex );
        m_iTargetMonsterIndex = -1;
    }
    else
    {
        m_iTargetMonsterIndex = iSlotIndex;
        m_iTargetPlayerIndex = -1;
    }
}

// ¿©±â¼­ ¹Þ´Â °ªµéÀº ÇÈ¼¿ ´ÜÀ§ÀÌ´Ù
bool MONSTER::SetTarget( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    int iNearPlayer = -1;
    int iNearMonster = -1;
    float fMinDistance = 9999.0f;


    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( false == EffectivePlayer(i) )
            continue;

        // ¹üÀ§¾È¿¡ ÇÃ·¹ÀÌ¾î°¡ ÀÖ´ÂÁö »ìÇÉ´Ù
        if( pPlayer->vPos.x > iX1 && 
            pPlayer->vPos.x < iX2 &&
            pPlayer->vPos.y > iY2 && 
            pPlayer->vPos.y < iY1 )
        {
            float fDist = DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
            if( fMinDistance > fDist )
            {
                fMinDistance = fDist;
                iNearPlayer = i;
            }
        }
    }

    // ¸ó½ºÅÍµµ °°Àº ÆÀÀÌ ¾Æ´Ñ ¸ó½ºÅÍ¸¦ °ø°ÝÇØ¾ßÇÑ´Ù.
    GCAttackRect<float> rect;
    GCAttackRect<float> CollisionRect;
    rect.SetRect( iX1-1.0f, iY1-0.25f, iX2-1.0f, iY2-0.25f);
    fMinDistance = -9999.0f;

	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if( !mit->second->IsLive() || mit->second->GetHP() <= 0.f )
			continue;

		if( g_MyD3D->IsSameTeam_MonsterAndMonster(mit->first, m_iMonsterIndex) )
			continue;

		if( NPC_NONE != mit->second->m_eNPC )
			continue;

		if( mit->second->m_bMonsterTarget == false )
			continue;

		// 2011.2.8, shmhlove : ¹üÀ§¾È¿¡ Å¸°ÙÀÌ ÀÖ´ÂÁö »ìÇÉ´Ù
		std::vector< GCAttackRect<float> >::iterator vecIter = mit->second->m_vecAttackRect.begin();
		for(; vecIter != mit->second->m_vecAttackRect.end(); ++vecIter)
		{
			if( rect.CheckCollision( *vecIter, &CollisionRect ) )
			{
				float fHeight = CollisionRect.m_Right - CollisionRect.m_Left;
				float fUnder = CollisionRect.m_Top - CollisionRect.m_Bottom;
				float fDist = fHeight * fUnder;
				if( fMinDistance < fDist )
				{
					fMinDistance = fDist;
					iNearMonster = mit->first;
				}
			}
		}

	}

    bool bResult = false;
    if( 0 <= iNearPlayer || m_bAggroState )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_iTargetPlayerIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }

        SetTargetPlayerIndex( iNearPlayer );
        bResult = true;
    }
    else if( 0 <= iNearMonster )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_iTargetMonsterIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }
        m_iTargetMonsterIndex = iNearMonster;
        bResult = true;
    }

    return bResult;
}

bool MONSTER::SetTargetToPlayer( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    int iNearPlayer = -1;
    float fMinDistance = 9999.0f;
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( false == EffectivePlayer(i) )
            continue;

        // ¹üÀ§¾È¿¡ ÇÃ·¹ÀÌ¾î°¡ ÀÖ´ÂÁö »ìÇÉ´Ù
        if( pPlayer->vPos.x > iX1 && 
            pPlayer->vPos.x < iX2 &&
            pPlayer->vPos.y > iY2 && 
            pPlayer->vPos.y < iY1 )
        {
            float fDist = DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
            if( fMinDistance > fDist )
            {
                fMinDistance = fDist;
                iNearPlayer = i;
            }
        }
    }

    bool bResult = false;
    if( 0 <= iNearPlayer || m_bAggroState )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_iTargetPlayerIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }

        SetTargetPlayerIndex( iNearPlayer );
        bResult = true;
    }

    return bResult;
}

bool MONSTER::SetTargetToMonster( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    int iNearMonster = -1;
    float fMinDistance = 9999.0f;

    GCAttackRect<float> rect;
    GCAttackRect<float> CollisionRect;
    rect.SetRect( iX1-1.0f, iY1-0.25f, iX2-1.0f, iY2-0.25f);
    fMinDistance = -9999.0f;

    for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
        if( !mit->second->IsLive() || mit->second->GetHP() <= 0.f )
            continue;

        if( g_MyD3D->IsSameTeam_MonsterAndMonster(mit->first, m_iMonsterIndex) )
            continue;

        if( NPC_NONE != mit->second->m_eNPC )
            continue;

        if( mit->second->m_bMonsterTarget == false )
            continue;

        // 2011.2.8, shmhlove : ¹üÀ§¾È¿¡ Å¸°ÙÀÌ ÀÖ´ÂÁö »ìÇÉ´Ù
        std::vector< GCAttackRect<float> >::iterator vecIter = mit->second->m_vecAttackRect.begin();
        for(; vecIter != mit->second->m_vecAttackRect.end(); ++vecIter)
        {
            if( rect.CheckCollision( *vecIter, &CollisionRect ) )
            {
                float fHeight = CollisionRect.m_Right - CollisionRect.m_Left;
                float fUnder = CollisionRect.m_Top - CollisionRect.m_Bottom;
                float fDist = fHeight * fUnder;
                if( fMinDistance < fDist )
                {
                    fMinDistance = fDist;
                    iNearMonster = mit->first;
                }
            }
        }

    }

    bool bResult = false;
    if( 0 <= iNearMonster )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_iTargetMonsterIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }
        m_iTargetMonsterIndex = iNearMonster;
        bResult = true;
    }

    return bResult;
}

void MONSTER::ReleaseTarget( bool bNotChangeWaitState )
{
    SetTargetPlayerIndex( -1 );
    m_iTargetMonsterIndex = -1;

    if( false == bNotChangeWaitState )
        SetState( "WAIT" );
}

bool MONSTER::TurnToTarget( bool bReverse )
{
    if( !IsTarget() )
        return false;

    // Å¸°ÙÀÌ ÀÚ½ÅÀÇ ¿À¸¥ÂÊ¿¡ ÀÖ´ÂÁö ¿ÞÂÊ¿¡ ÀÖ´ÂÁö ÆÇ´ÜÇØ¼­ µ¹¾Æ¼±´Ù
    bool bPreIsRight = m_abIsRight[ LATENCY_INDEX ];
    float fTargetX = GetTargetX();
    bool bRight = false;

    if( m_afX[ LATENCY_INDEX ] > fTargetX )
        bRight = false;
    else 
        bRight = true;

    if ( bReverse ) 
        bRight = !bRight;

    m_abIsRight[ LATENCY_INDEX ] = bRight; 

    return ( bPreIsRight != m_abIsRight[ LATENCY_INDEX ] );
}

float MONSTER::GetTargetX()
{
    if( IsTarget() )
    {
        if( IsTargetMonster() && g_kMonsterManager.IsMonsterIndex(m_iTargetMonsterIndex) )
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonsterIndex);
			MONSTER* pMonsterTarget = g_kMonsterManager.GetMonster(m_iTargetMonsterIndex);
			if(pMonster != NULL && pMonsterTarget != NULL) {

				// GC_TEMP_CODE_START À§Ä¡ Â÷ÀÌ°¡ 0ÀÌµÇ¸é ¾ÈµÇ±â¿¡ ÀÓ½Ã·Î ÀÌ·¸°Ô ÇØµÓ´Ï´Ù. ³ªÁß¿¡ ¼öÁ¤!!!
				float fMyX = pMonster->m_afX[ LATENCY_INDEX ];
				float fTargetX = pMonsterTarget->m_afX[ LATENCY_INDEX ];

				if( fMyX == fTargetX )
					fTargetX += 0.0001f;
            // GC_TEMP_CODE_END
				return fTargetX;
			}
        }
        else if( g_MyD3D->IsPlayerIndex(m_iTargetPlayerIndex) )
        {
            if( m_iTargetPlayerIndex == g_MyD3D->Get_MyPlayer() ) // Å¸°ÙÀÌ ÀÚ±â ÀÚ½ÅÀÌ¸é..
            {
                return g_kLocalPlayerLatency->kRenderData[0].vPos.x;
            }
            else
            {
                return g_MyD3D->MyPlayer[ m_iTargetPlayerIndex ]->vPos.x;
            }
        }        
        else if( IsTargetDamage() )
        {
            return GetTargetDamagePos().x;
        }
    }

    return 0.0f;
}

float MONSTER::GetTargetY()
{
    if( IsTarget() )
    {
        if( IsTargetMonster() && g_kMonsterManager.IsMonsterIndex(m_iTargetMonsterIndex) )
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iMonsterIndex);
			MONSTER* pMonsterTarget = g_kMonsterManager.GetMonster(m_iTargetMonsterIndex);
			if(pMonster != NULL && pMonsterTarget != NULL) {
				// GC_TEMP_CODE_START À§Ä¡ Â÷ÀÌ°¡ 0ÀÌµÇ¸é ¾ÈµÇ±â¿¡ ÀÓ½Ã·Î ÀÌ·¸°Ô ÇØµÓ´Ï´Ù. ³ªÁß¿¡ ¼öÁ¤!!!
				float fMyY = pMonster->m_afY[ LATENCY_INDEX ];
				float fTargetY = pMonsterTarget->m_afY[ LATENCY_INDEX ];

				if( fMyY == fTargetY )
					fTargetY += 0.0001f;
				// GC_TEMP_CODE_END

				return fTargetY;
			}
        }
        else if( g_MyD3D->IsPlayerIndex(m_iTargetPlayerIndex) )
        {
            if( m_iTargetPlayerIndex == g_MyD3D->Get_MyPlayer() ) // Å¸°ÙÀÌ ÀÚ±â ÀÚ½ÅÀÌ¸é..
            {
                return g_kLocalPlayerLatency->kRenderData[0].vPos.y;
            }
            else
            {
                return g_MyD3D->MyPlayer[ m_iTargetPlayerIndex ]->vPos.y;
            }
        }
        else if( IsTargetDamage() )
        {
            return GetTargetDamagePos().y;
        }
    }

    return 0.0f;
}

bool MONSTER::GetTargetIsContact()
{
    if( IsTarget() )
    {
        if ( IsTargetMonster() )
        {
			MONSTER* pMonsterTarget = g_kMonsterManager.GetMonster(m_iTargetMonsterIndex);
			if(pMonsterTarget)
				return pMonsterTarget->m_bIsContact;
        }
        return g_MyD3D->MyPlayer[ m_iTargetPlayerIndex ]->IsContact;
    }
    
    return false;
}

bool MONSTER::GetTargetIsRight()
{
    if( IsTarget() )
    {
        if ( IsTargetMonster() )
        {
			MONSTER* pMonsterTarget = g_kMonsterManager.GetMonster(m_iTargetMonsterIndex);
			if(pMonsterTarget)
	            return pMonsterTarget->m_abIsRight[ LATENCY_INDEX ];
        }
        else
        {
            if( m_iTargetPlayerIndex == g_MyD3D->Get_MyPlayer() ) // Å¸°ÙÀÌ ÀÚ±â ÀÚ½ÅÀÌ¸é..
            {
                return g_kLocalPlayerLatency->kRenderData[0].bIsRight;
            }
            else
            {
                return g_MyD3D->MyPlayer[ m_iTargetPlayerIndex ]->GetIsRight();
            }
        }
    }

    return false;
}

void MONSTER::StartAttack( int iAttackDamage, int iAttackType, float fRange, int iBaseBone_, int iEndBone_, int iLifeTime_ )
{
    KMonsterPhysicAttack kAtt( iBaseBone_,iEndBone_,fRange,iAttackDamage,iAttackType,iLifeTime_? iLifeTime_ : INT_MAX );
    m_vecPhsAtk.push_back( kAtt );
}

void MONSTER::EndAttack()
{
    m_vecPhsAtk.clear();

}

void MONSTER::FrameMove( void )
{
    PROFILE_SET( "MONSTER::FrameMove" );

	OnFrameMove();

    ReadyToRender();
    TraceMonster();

    CalcPushRect();
    CalcAttackRect();
    CalcFootHold();

	m_QueueParticleList.push( m_ParticleMsg );
	m_ParticleMsg.clear();

	if( m_QueueParticleList.size() )
	{
		ParticleQueueProcess(m_QueueParticleList.front());
		m_QueueParticleList.pop();
	}

    m_kMonHPBar.FrameMove();

    if( !m_bLive )
        return;

    FrameMove_ParticleModel();

#if defined(ENABLE_DUGEON_HACK_CHECKING)
	// ÀÌÁß Ã¼Å·À» ÇÏµµ·Ï ÇÏ¿´´Ù. °ÔÀÓÃ¢ÀÌ ºñÈ°¼ºÈ­ »óÅÂ¿¡¼­ ÇØÅ·»ç¿ëÀ» 1Â÷ÀûÀ¸·Î Ã¼Å· ÈÄ
	// ÇØÅ·»ç¿ë¿©ºÎ°¡ È®ÀÎµÇ¸é, 2Â÷ÀûÀ¸·Î ´Ù½Ã ÇØÅ·Ã¼Å©¸¦ ¼öÇàÇÑ´Ù.
	if( !g_kGlobalValue.m_bActiveApp )
	{
		HackingCheck();
	}
	else
	{
		if( g_kGlobalValue.m_bUseCrack )
			HackingCheck();
	}
#endif

    // µå¶ó¸¶ Áß¿¡ ¼ÒÈ¯µÈ º¸½º¸ó½ºÅÍÀÏ °æ¿ì¿¡´Â 
    // µå¶ó¸¶ Á¾·á ÆÐÅ¶ µµÂø Å¸ÀÌ¹Ö¿¡ µû¶ó HP°ÔÀÌÁö°¡ ¾Èº¸ÀÏ ¼öµµ ÀÖ¾î Ãß°¡ÇÑ ÄÚµåÀÔ´Ï´Ù.
    if( (MON_BOSS == m_iBossMonster) && (false == g_pkGameOverlayUI->IsShowDialogue()) )
    {
        if( (false == m_bDie) && m_bLive && m_fHP > 0.0f )
        {
            // º¸½º°¡ Ãß°¡¾ÈµÇ¾î ÀÖÀ¸¸é Ãß°¡
            if( false == g_pkGameOverlayUI->IsAddBossMonster( this ) )
                g_pkGameOverlayUI->AddBossMonster( this );
            // °ÔÀÌÁö¹Ù°¡ Ãâ·Â¾ÈµÇÀÖÀ¸¸é Ãâ·Â
            if( false == g_pkGameOverlayUI->IsRenderBossHPBar(this) )
                g_pkGameOverlayUI->UpdateBossHPBar( m_iMonsterType );
        }
    }
#if defined (USE_NEWBIE_GUIDE)
    if (SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST0)
    {
        g_pkGameOverlayUI->m_pkAddtionGuide->CheckSpecialMonster();
    }
#endif

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        // ¸ó½ºÅÍ ¸ð¼Ç ÇÏ³ª¾¿ µ¹·Áº¸Àð ¤¾ 
        if ( g_kGlobalValue.m_bMonsterAction && g_MyD3D->MyCtrl->k_MonsterAction != 0 && m_bDie == false && m_fHP > 0.0f && m_bLive == true )
        {
            MonsterAction( g_kGlobalValue.m_iMonsterActionIndex );
        }

		// Å¸ÀÌ¸Ó µ¹¸®Àð~ ÇÑÇÁ·¹ÀÓ¿¡ 1¾¿ ¿Ã¸°´Ù~ ¤¾¤¾ 
		if ( m_bProcessTimer )
			++m_iTimerFrame;

        // ÃÖÃÊ·Î HP°¡ 0µÇ´Â ¼ø°£ÀÌ³ª!!! HP°¡ 0ÀÌ µÇ¾ú´Âµ¥ È¤½Ã³ª OnDie°¡ È£Ãâ¾ÈµÇ¾ú´Ù¸é OnDie¸¦ È£Ãâ½ÃÅ°ÀÚ!!!
        // HP°¡ 0ÀÎµ¥ ¸ó½ºÅÍ°¡ ¾È»ç¶óÁö´Â ¹®Á¦°¡ ÀÖ´Ù°í ÇØ¼­ ÀçÇöÀº ¾ÈµÇ°í,,, ÀÏ´Ü ¿¹¿ÜÄÚµå ³Ö¾îµÒ.( OnDieÈ£ÃâÈ®ÀÎÄÚµå )
        if( (m_fHP <= 0.0f && m_fOldHP > 0.0f) || (m_fHP <= 0.0f && false == m_bCallDieFucn) )
        {
            m_fOldHP = m_fHP;
            OnDie();

            if( g_MyD3D->m_pkQuestGameMgr->m_iCurMonsterNum > 0 )
            {
                if( g_MyD3D->m_pkQuestGameMgr->m_iCurMonsterNum == 1 )
                {
                    g_MyD3D->m_pkQuestGameMgr->SetCurIncAtkRatio();					
                    g_MyD3D->m_pkQuestGameMgr->m_iCurMonsterNum--;
                }
                else
                {
                    if( m_iBossMonster==MONSTER::MON_BOSS )
                    {
                        g_MyD3D->m_pkQuestGameMgr->SetCurIncAtkRatio();
                    }
                    g_MyD3D->m_pkQuestGameMgr->m_iCurMonsterNum--;
                }
            }
            return;
        }

        m_fOldHP = m_fHP;

    }

    // ¹æÀåÀÌµç ¹æÀåÀÌ ¾Æ´Ïµç ÃÖÃÊ·Î HP°¡ 0ÀÌ µÇ´Â ¼ø°£..
    if( m_fHP <= 0.0f && m_bShowExp == false )
    {
        // ¼ÒÈ¯µÈ ¸ó½ºÅÍ°¡ Á×À» ¶§´Â °æÇèÄ¡ ¾øÀ½.
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerUID );
        if( NULL == pPlayer )
            ShowExp();

        if ( m_iSummoned == MEMS_NOT_SUMMON_MONSTER )
        {
            g_kGlobalValue.m_kUserMission.VirtualKillMonster( m_iMonsterType, m_iLevel, int(m_eNPC) );
        }
        SiKGCAchieveManager()->OccurAction( SAchieve::KILL_MONSTER, g_MyD3D->GetMyPlayer()->GetCharType(), m_iMonsterType, m_iBossMonster );
        
        if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) 
        { 
            g_pkQuestManager->SetComboKillPoint( false );

            // º¸½º¸¸ µû·Î Áý°è
            if( m_iBossMonster == MONSTER::MON_BOSS )
            {
                g_pkQuestManager->SetComboBossKillPoint();
            }
        }
        else if ( g_MyD3D->GetMyPlayer()->GetcomboState() ) {
            g_pkQuestManager->SetComboKillPoint( false );
        }

        if( m_dwChampionProperty != 0x00000000 ) {
            SiKGCAchieveManager()->OccurAction( SAchieve::KILL_CHAMPION, g_MyD3D->GetMyPlayer()->GetCharType(), m_iMonsterType, m_iBossMonster );
        }

        if( -1 != m_iTriggerID )
        {
            g_pGameBoard->AddMonKillNum( m_iMonsterType, 1, m_iTriggerID );     // Á×Àº ¸ó½ºÅÍÀÇ Æ®¸®°Å ¹øÈ£¸¦ ±â·Ï.
            m_iTriggerID = -1;
        }

        m_bShowExp = true;
        EGCMonster eMonster = GetDefaultMonster( m_iMonsterType );
        if( m_iBossMonster == MONSTER::MON_BOSS &&
            eMonster != MON_PATUSEI_BODY &&
            eMonster != MON_PATUSEI_LEG &&
            eMonster != MON_CARNIVAL_RENEW_H_ZIDLER  )
        {
            StartBlur( m_iMonsterIndex, fBossDieCameraZoom, dwBossDieBlurTime );
        }
    }

    if ( m_bBlur )
    {
        if ( m_dwBlurStartTime + m_dwBlurEndTime >= ::timeGetTime() )
        {
            g_MyD3D->m_KGCBlurMgr.SetBlurAlpha( 0.75f );
            g_MyD3D->m_KGCBlurMgr.Enable( true );
        }
        else
        {
            EndBlur();
        }
    }

    // Æ¯Á¤ ¸ó½ºÅÍ´Â ÇÃ·¹ÀÌ¾î°¡ ¸øÁö³ª°¡µµ·Ï ¸·´Â´Ù..-¤µ -;
    EGCMonster eMonster = GetDefaultMonster( m_iMonsterType );
    if( eMonster == MON_BASILISK ||
        eMonster == MON_PATUSEI_HEAD ||
        eMonster == MON_PATUSEI_BODY ||
        eMonster == MON_PATUSEI_LEG ||
        eMonster == MON_HERO_FLAME_BASILISK ||
        eMonster == MON_HERO_FROST_BASILISK )
    {
        if( m_fHP > 0.0f )
        {
            float fBlockPosX = 9999.0f;

            switch( eMonster )
            {
                case MON_BASILISK:
                    fBlockPosX = 1.9f;
                    break;
                case MON_PATUSEI_HEAD:
                case MON_PATUSEI_BODY:
                case MON_PATUSEI_LEG:
                    fBlockPosX = 2.05f;
                    break;
                case MON_HERO_FLAME_BASILISK:
                    fBlockPosX = 5.10f;
                    break;
                case MON_HERO_FROST_BASILISK:
                    fBlockPosX = -0.89f;
                    break;
            }

            PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
            if( pPlayer )
            {
                if ( fBlockPosX < 0.0f )
                {
                    if( pPlayer->vPos.x < -fBlockPosX )
                    {
                        pPlayer->vPos.x = -fBlockPosX;
                    }
                }
                else
                {
                    if( pPlayer->vPos.x > fBlockPosX )
                    {
                        pPlayer->vPos.x = fBlockPosX;
                    }
                }
            }
        }
    }

    if( m_iSlowCount )
    {
        if ( !m_iLatencySlowCount )
        {
            m_iLatencySlowCount = LATENCY_BUFFER_SIZE;
            if( !g_kGlobalValue.m_kUserInfo.bHost )
                m_iLatencySlowCount = 1;
        }
        else if ( m_iLatencySlowCount == 1 )
        {
            --m_iSlowCount;
            return;
        }
        else
        {
            --m_iLatencySlowCount;
        }
    }
    else
    {
        m_iLatencySlowCount = 0;
    }

    // ¸ó½ºÅÍ°¡ Á×¾úÀ» ¶§ m_iRemainRender ¸¸Å­ ·»´õ¸µ µÇ´Ù°¡ º¸ÀÌÁö ¾Ê°ÔµÈ´Ù
    if( m_iRemainRender > 0 )
    {
        --m_iRemainRender;

        if( m_iRemainRender == 0 )
        {
            m_bLive = false;

            EGCMonster eMonsterType = GetDefaultMonster( m_iMonsterType );
            if( eMonsterType != MON_PATUSEI_HEAD )
            {
                m_pObject->SetRender( false );
                m_bRender = false;
                m_pkMonsterModelDead->SetRender( true );
            }

            // º¸½º ºí·¯¿Í ÇÁ·¹ÀÓ¶ôÀÌ ¾È³¡³µÀ»±îºÁ;;;
            if( m_bBlur || (m_iBossMonster == MONSTER::MON_BOSS) )
            {
                EndBlur();
            }
            //////////////////////////////////////////////////////////////////////////

            if( m_iBossMonster==MONSTER::MON_BOSS && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
            {
                g_MyD3D->MyHead->Start_Text( "DEFEAT" );
                g_KDSound.Play( "88" );
            }

            m_pObject->ResetMeshState();

			ActionCallBack("End");
           
            return; 
        }
    }

    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    for( int i = 0; i < LATENCY_INDEX; ++i )
    {
        m_aiMotion[i]   = m_aiMotion[i + 1];
        m_aiFrame[i]    = m_aiFrame[i + 1];
        SetLatencyX( i, m_afX[i + 1] );
        SetLatencyY( i, m_afY[i + 1] );
        m_abIsRight[i]  = m_abIsRight[i + 1];
    }

    // ÇÁ·¹ÀÓ¹«ºê µ¹¸±¶§ ¾²´Â ÀÎµ¦½º..
    // ¹æÀåÀº LATENCY_INDEX, ¹æÀå¾Æ´Ï¸é 0
    int iIndex = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iIndex = 0;

    std::string strMotion = g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_aiMotion[ iIndex ] );
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mit = mapAction.find( strMotion );
    if( mit == mapAction.end() )
        return;

    KGCMonsterAction &kMonsterAction = mit->second;

    if( m_iNoCheckContact > 0 )
        --m_iNoCheckContact;

    if( m_iNoCheckContact <= 0 )
        m_bIgnoreFHCheck = false;

    if( m_iSuperArmor > 0 )
        --m_iSuperArmor;

    if( m_iInvincible > 0 )
        --m_iInvincible;

    if( m_iDownDelay > 0 && m_bIsContact )
    {
        --m_iDownDelay;

        // °¡¼Ó±â»ó.. ¶¥¿¡ ´êÀÚ¸¶ÀÚ ÀÏ¾î³­´Ù!!
        if( m_dwChampionProperty & CP_FAST_STAND )
            m_iDownDelay = 0;
	}

	// ±â»ó½Ã ¼îÅ© ¿þÀÌºê °ø°ÝÇÏ´Â³ð 
	if( m_dwChampionProperty & CP_SHOCKWAVE  && strMotion=="STANDUP" && m_aiFrame[ iIndex ]==1)
	{	
		AddDamage(874);
		AddDamage(875);
        g_KDSound.Play( "43" );
		AddParticleNoDirection("Ronan2_SPECIAL1-2-01",0,0.15f);
	}			
	

    if( m_dwChampionProperty & CP_HP_RECOVERY ) // HPÈ¸º¹
    {
        // ¸Å ÃÊ Ã¼·ÂÀÇ 2%È¸º¹
        if( m_iHPRecoveryCount == 0 && m_fHP < m_fHPPoint && m_fHP != 0 )
        {
            if( m_fHP + m_fHPPoint * m_fHP_Recover / GC_FPS_LIMIT < m_fHPPoint )
                m_fHP += ( m_fHPPoint * m_fHP_Recover / GC_FPS_LIMIT );
            else
                m_fHP = m_fHPPoint;
        }

        if( m_iHPRecoveryCount > 0 )
            --m_iHPRecoveryCount;
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
		//TraceMonster();

        if( 0 < m_iStopSpeedFrame )
        {
            m_fSpeedX = 0.0f;
            m_fSpeedY = 0.0f;
            --m_iStopSpeedFrame;
        }

        if( !m_bIsFly ) // ³¯¾Æ´Ù´Ï¸é ÄÁÅÃ Ã³¸® ÇÒ ÇÊ¿ä ¾øÀ½
        {
            D3DXVECTOR3 vPosContact;
            m_bOldIsContact = m_bIsContact;
            m_bIsContact = false;

            bool bNoCheckContact = ( m_iNoCheckContact > 0 );

            if( m_fContactHeight > 0.0f )
                bNoCheckContact = bNoCheckContact || ( m_fContactHeight <= m_afY[LATENCY_INDEX] + m_fSpeedY );

            KGCContectInfo kInfo( m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ], m_fSpeedX, m_fSpeedY, bNoCheckContact, m_bIgnoreFHCheck, false );
            if( true == g_MyD3D->MyStg->Check_Contact( kInfo ) )
            {
                if( kInfo.m_ySpeed <= 0.0f )
                {
                    // ¶¥¿¡ ºÙ¾úÀ½
                    m_bIsContact = true;
                    SetLatencyY(  LATENCY_INDEX , kInfo.m_y );
                    kInfo.m_ySpeed = 0.0f;
                    m_fContactHeight = 0.0f;
                }
            }

            SetLatencyX( LATENCY_INDEX, kInfo.m_x );
            m_fSpeedX = kInfo.m_xSpeed;
            m_fSpeedY = kInfo.m_ySpeed;

            if( !m_bIsContact ) // °øÁß¿¡ ¶° ÀÖ´Â µ¿¾È Áß·ÂÀ» Àû¿ë½ÃÅ°ÀÚ..
            {
                // °øÁß¿¡ ¶° ÀÖ´Â µ¿¾ÈÀº x½ºÇÇµåÀÇ º¯È­´Â ¾øÀ½..
                if( m_fSpeedY > -0.03f )
                {
                    if( m_fSpeedY - GRAVITY > -0.03f )
                        m_fSpeedY -= GRAVITY;
                    else
                        m_fSpeedY = -0.03f;
                }

                // ¼ÒÈ¯µÈ ¸ó½ºÅÍ°¡ ³«»çÀÎÁö Ã¼Å©ÇØº¸ÀÚ..
                PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerUID );
                if( pPlayer && m_afY[ LATENCY_INDEX ] < -1.0f )
                {
                    pPlayer->SetSummonMonsterHP( 0.0f );
                }
            }
            else // ¶¥¿¡ ºÙ¾úÀ» ¶§..
            {
                bool bMoveFail = false;

                if( kMonsterAction.m_bEnableMoveCheck )
                {
                    KNextBlockCheck kNextCheck;
                    kNextCheck.bIsRight = m_abIsRight[ LATENCY_INDEX ];
                    kNextCheck.fX = m_afX[ LATENCY_INDEX ] + m_fSpeedX;
                    kNextCheck.fY = m_afY[ LATENCY_INDEX ] + m_fSpeedY;
                    kNextCheck.fWidth = m_fWidth;
                    if( !g_MyD3D->MyStg->IsExistNextBlock( kNextCheck ) )
                    {
                        bMoveFail = true;
                    }
                }

                // ÇöÀç ¶¥À» °È´Â ÁßÀÌ´Ù!!
                if( kMonsterAction.m_bEnableMoveCheck )
                {
                    if( bMoveFail )
                    {
                        CallLuaFucn_MoveFail();
                        return;
                    }
                }

                // ¶¥¿¡ ÀÖÀ» ¶§´Â ¸¶Âû·ÂÀ» Àû¿ë½ÃÄÑ ÁØ´Ù!
                if( m_fSpeedX < -MONSTER_FRICTIONAL_FORCE )
                    m_fSpeedX += MONSTER_FRICTIONAL_FORCE;
                else if( m_fSpeedX > MONSTER_FRICTIONAL_FORCE )
                    m_fSpeedX -= MONSTER_FRICTIONAL_FORCE;
                else
                    m_fSpeedX = 0.0f;
            }
        }

        m_bIsFallDownMotion = kMonsterAction.m_bIsFallDownMotion;

        if( m_bOldIsContact == false && m_bIsContact == true )
        {
            OnLand();
            return;
        }

        if( 0.0f != m_fSpeedX && false == m_bNotCheckStateWall )
        {
            float fBeforeX = m_afX[ LATENCY_INDEX ];
            float fAfterX = m_afX[ LATENCY_INDEX ] + m_fSpeedX;

            if( fBeforeX < fAfterX )
            {
                fBeforeX -= FOOTHOLDSIZE;
                fAfterX += FOOTHOLDSIZE;
            }
            else
            {
                fBeforeX += FOOTHOLDSIZE;
                fAfterX -= FOOTHOLDSIZE;
            }

            if( CheckPierceBlockXPos(fBeforeX, fAfterX, m_afY[ LATENCY_INDEX ]+m_fSpeedY) )
            {
                m_fSpeedX = 0.0f;
            }
        }

        m_afX[ LATENCY_INDEX ] += m_fSpeedX;
        m_afY[ LATENCY_INDEX ] += m_fSpeedY;

        #if defined( ERROR_MESSAGE_BOX )
        {
            if( !m_vecMotionList.empty() )
            {
                if( m_vecMotionList[ m_aiMotion[ LATENCY_INDEX ] ] == NULL || m_vecMotionList[ m_aiMotion[ LATENCY_INDEX ] ]->IsDisable() )
                {
                    std::wstringstream strText;
                    std::wstringstream strCaption;
                    std::string strState = g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_aiMotion[ LATENCY_INDEX ] );
                    strText << L"( " << m_strMonsterName << L" " << strState.c_str() << L" ) 모션 파일에 문제 있을듯";
                    strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
                    MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
                }
            }            
        }
        #endif

        // ¸ð¼ÇÀÇ XÃàÀÌ ÀÌµ¿ÇßÀ» °æ¿ì
        if( !m_vecMotionList.empty() )
        {
            float fMotionX = m_vecMotionList[ m_aiMotion[ LATENCY_INDEX ] ]->GetFrameData( m_aiFrame[ LATENCY_INDEX ] ).Plus_x * m_fScale;
            if( 0.0f != fMotionX )
            {
                float fBeforeX = m_afX[ LATENCY_INDEX ];
                float fAfterX = m_afX[ LATENCY_INDEX ];

                if( m_abIsRight[ LATENCY_INDEX ] )
                    fAfterX += fMotionX;
                else
                    fAfterX -= fMotionX;

                // ¿©ºÐÀÇ ¿ÀÇÁ¼Â Ã³¸®
                if( fBeforeX < fAfterX )
                {
                    fBeforeX -= FOOTHOLDSIZE;
                    fAfterX += FOOTHOLDSIZE;
                }
                else
                {
                    fBeforeX += FOOTHOLDSIZE;
                    fAfterX -= FOOTHOLDSIZE;
                }

                if( m_bNotCheckStateWall ) { 
                    if( m_abIsRight[ LATENCY_INDEX ] )
                        m_afX[ LATENCY_INDEX ] += m_vecMotionList[ m_aiMotion[ LATENCY_INDEX ] ]->GetFrameData( m_aiFrame[ LATENCY_INDEX ] ).Plus_x * m_fScale;
                    else
                        m_afX[ LATENCY_INDEX ] -= m_vecMotionList[ m_aiMotion[ LATENCY_INDEX ] ]->GetFrameData( m_aiFrame[ LATENCY_INDEX ] ).Plus_x * m_fScale;
                }
                else if( false == CheckPierceBlockXPos(fBeforeX, fAfterX, m_afY[ LATENCY_INDEX ]) ) {
                    if( fBeforeX < fAfterX )
                        m_afX[ LATENCY_INDEX ] = fAfterX - FOOTHOLDSIZE;
                    else
                        m_afX[ LATENCY_INDEX ] = fAfterX + FOOTHOLDSIZE;
                }
            }
        }

        // ÇÁ·¹ÀÓ Áõ°¡!
        if( !m_bFrameLock && !m_bBuffFrameLock )
        {
            int iLastFrame = GetLastFrame_( LATENCY_INDEX );
            if( m_aiFrame[ LATENCY_INDEX ] < iLastFrame )
            {
                ++m_aiFrame[ LATENCY_INDEX ];
            }
            else
            {
                m_aiFrame[ LATENCY_INDEX ] = iLastFrame;
                m_bFrameLock = true;
            }
        }
    }

    if( !g_kGlobalValue.m_kUserInfo.bHost )
    {
        if( m_iRemainFrame <= 0 )
        {
            ++m_aiFrame[ LATENCY_INDEX ];

            int iLastFrame = GetLastFrame_( LATENCY_INDEX );
            if( m_aiFrame[ LATENCY_INDEX ] >= iLastFrame )
                m_aiFrame[ LATENCY_INDEX ] = iLastFrame;
        }
        else
        {
            --m_iRemainFrame;
        }
    }

    CrashCheck(); // Ãæµ¹Ã³¸®

    if( (0 >= m_iNoTargettingFrame) || (0 < m_iBossMonster) )
    {
        // ¸»ÇÏ´Â ÁßÀÌ¸é ai¾Èµ¹¸°´Ù.
        if( g_kGlobalValue.m_kUserInfo.bHost && m_bLive && g_kGlobalValue.m_bAIEnable && m_bMovement && !g_kGlobalValue.m_bMonsterAction )
        {
            bool bGameEnd = false;
            KGCGameModeInterface* pQuest = g_pStateGame->GetCurGameModePtr();
            switch( SiKGCRoomManager()->GetGameModeCategory() )
            {
            case GC_GMC_DUNGEON:
                bGameEnd = g_pkQuestManager->m_bEndGame;
                break;
            case GC_GMC_MONSTER_CRUSADER:
                bGameEnd = pQuest->CheckGameEnd();
                break;
            }

            if ( g_pkGameOverlayUI->IsShowDialogue() || bGameEnd )
                m_bMovement = false;
            else
                m_bMovement = true;
        }

        // ¹æÀåÀÌ¸é ¸ó½ºÅÍÀÇ ¿òÁ÷ÀÓÀ» Á¦¾îÇÏÀÚ
        if( g_kGlobalValue.m_kUserInfo.bHost && m_bLive && g_kGlobalValue.m_bAIEnable && m_bMovement && !g_kGlobalValue.m_bMonsterAction )
        {
            for( int i = 0; i < (int)kMonsterAction.m_vecActionTrigger.size(); ++i )
            {
                // ¸¸¾à Æ®¸®°Å¿¡¼­ setstate¸¦ ÇØ¼­ ´Ù¸¥ state·Î ÀÌµ¿Çß´Ù¸é
                // ÀÌÀü½ºÅ×ÀÌÆ®ÀÇ Æ®¸®°Å¸¦ ´õ ÀÌ»ó µ¹¸®¸é ¾ÈµÈ´Ù ¸ð¼Ç ¹Ù²ï µÚ ºüÁ®³ª°¨
                if( m_iOldMotion != m_aiMotion[ LATENCY_INDEX ] )
                    break;

                KGCActionTrigger &kActionTrigger = kMonsterAction.m_vecActionTrigger[i];

                // µô·¹ÀÌ°¡ ³¡³µÀ¸¸é..
                if( kActionTrigger.iRemainDelayFrame <= 0 )
                {
                    if( kActionTrigger.iRemainAICheckFrame <= 0 )
                    {

                        m_bResult = false;

                        bool bUseAIScript = false;
                        SDotaTemplate* pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();
                        if( pCurDotaInfo &&
                            false == pCurDotaInfo->bUseAIFromScript &&
                            GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
                        {//Æ®¸®°ÅÇÔ¼ö
                            switch( m_iMonsterType )
                            {
                            case MON_DOTA_ANCIENT_SERDIN:
                            case MON_DOTA_ANCIENT_CANABAN:
                                if( strstr(kActionTrigger.strAIFunc.c_str(), "Ancient_Check_Targeting" ) )
                                    m_HardAI_Ancient.Trigger_Check_Targeting( i,  m_iMonsterIndex);
                                else if( strstr(kActionTrigger.strAIFunc.c_str(), "Ancient_Check_Attack01" ) )
                                    m_HardAI_Ancient.Trigger_Check_Attack01( i,  m_iMonsterIndex);
                                else if( strstr(kActionTrigger.strAIFunc.c_str(), "Ancient_Check_Attack02" ) )
                                    m_HardAI_Ancient.Trigger_Check_Attack02( i,  m_iMonsterIndex);
                                else if( strstr(kActionTrigger.strAIFunc.c_str(), "Ancient_Check_Attack03" ) )
                                    m_HardAI_Ancient.Trigger_Check_Attack03( i,  m_iMonsterIndex);
                                break;
                            case MON_DOTA_GUARDIAN_TOWER:
                                if( strstr(kActionTrigger.strAIFunc.c_str(), "DotaGuardianTower_Check_Targeting" ) )
                                    m_HardAI_GuardianTower.Trigger_Check_Targeting( i,  m_iMonsterIndex);
                                else if( strstr(kActionTrigger.strAIFunc.c_str(), "DotaGuardianTower_Check_Attack01" ) )
                                    m_HardAI_GuardianTower.Trigger_Check_Attack( i,  m_iMonsterIndex);
                                break;
                            case MON_DOTA_GUARD_TOWER:
                                if( strstr(kActionTrigger.strAIFunc.c_str(), "DotaGuardTower_Check_Targeting" ) )
                                    m_HardAI_GuardTower.Trigger_Check_Targeting( i,  m_iMonsterIndex);
                                else if( strstr(kActionTrigger.strAIFunc.c_str(), "DotaGuardTower_Check_Attack01" ) )
                                    m_HardAI_GuardTower.Trigger_Check_Attack( i,  m_iMonsterIndex);
                                break;
                            case MON_DOTA_CORE:
								break;
							
                            default:
                                bUseAIScript = true;
                            }
                        }
                        else
                        {
                            bUseAIScript = true;
                        }
                        
                        if( bUseAIScript )
                        {
                        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
                        CHECK_STACK(L)

                            lua_checkstack( L, 10 );					// ½ºÅÃ »çÀÌÁî¸¦ Ã¼Å© ÇÕ´Ï´Ù. ( 20º¸´Ù ÀÛÀ¸¸é ¿©À¯°ø°£À» È®º¸ ÇÕ´Ï´Ù. )
                        lua_pushstring( L, kActionTrigger.strAIFunc.c_str() );		// Player_Action Å×ÀÌºíÀ» ºÒ·¯¿À±â À§ÇØ ÀÌ¸§À» Push ÇÕ´Ï´Ù.
                        lua_rawget( L, LUA_GLOBALSINDEX );			// ±Û·Î¹ú Å×ÀÌºí¿¡¼­ Player_Action Å°¸¦ Ã£½À´Ï´Ù.
                        if( lua_isfunction( L, -1 ) )
                        {
                            lua_pushinteger( L, m_iMonsterIndex );
                            lua_pushinteger( L, m_aiFrame[ LATENCY_INDEX] );
                            lua_pcall( L, 2, 0, 0 );
                            if( m_bResult )
                            {
                                // µô·¹ÀÌ °»½Å
                                kActionTrigger.iRemainDelayFrame = kActionTrigger.iDelayFrame;
                            }
                        }
                        else
                        {
                            lua_pop( L, -1 );
                        }
                        }
                        // ´ÙÀ½ Ã¼Å©ÇÏ´Â ÇÁ·¹ÀÓ °»½Å
                        kActionTrigger.iRemainAICheckFrame = kActionTrigger.iAICheckFrame;
                    }
                    else
                    {
                        --kActionTrigger.iRemainAICheckFrame;
                    }
                }
                else
                {
                    --kActionTrigger.iRemainDelayFrame;
                }
            }
        }
    }

    int iStartFrame = 0;
    int iEndFrame = 0;

    // ÀÌÀü Motion°ú ÇöÀç MotionÀÌ °°´Ù¸é...
    if( m_iOldMotion == m_aiMotion[ iIndex ] )
    {
        m_bMotionChanged = false;
        if( m_iOldFrame < m_aiFrame[ iIndex ] )
        {
            iStartFrame = m_iOldFrame + 1;
            iEndFrame = m_aiFrame[ iIndex ] + 1;
        }
        else if( (m_bFrameLock || m_bBuffFrameLock) && ( m_iOldFrame == m_aiFrame[ iIndex ] ) ) // FrameLock°É·Áµµ °è¼Ó ±× ÇÁ·¹ÀÓ¿¡ µé¾î°¡µµ·Ï..
        {
            iStartFrame = m_aiFrame[ iIndex ];
            iEndFrame = m_aiFrame[ iIndex ] + 1;
        }
    }
    else
    {
        m_bMotionChanged = true;
        iStartFrame = 0;
        iEndFrame = m_aiFrame[ iIndex ] + 1;

        if( !g_kGlobalValue.m_kUserInfo.bHost )
        {
            m_bFrameLock                = false;
            m_bIsSuperArmor             = false;
            m_bAttackSuperArmor         = false;
            m_bIsInvincible             = false;
        }

        // ¿þÀÌÆ® ¸ð¼ÇÀ¸·Î °¥¶§ PushÃ¼Å©¸¦ Ç®¾îÁÖ´Â °í´ëÄÚµå°¡ ÀÖ´Ù.
        // ¾ÈÁÁÀº ÄÚµåÀÌ±ä ÇÏÁö¸¸ °ü·Ã ¹®Á¦¸¦ ´Ù Ã£¾Æ³¾ ¼ö ¾ø¾î ±×³É ³öµÎ±â·Î Çß´Ù.
        const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
        if( false == kMonsterTemplate.bNoPushCheck )
        {
            std::string strState = g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_aiMotion[ iIndex ] );
            if( strState == "WAIT" )
            {
                for( int i = 0 ; i < GetPushRectNum(); i++ )
                {
                    SetPushCheck( true , i );
                }
            }
        }
    }

    m_iOldFrame = m_aiFrame[ iIndex ];
    m_iOldMotion = m_aiMotion[ iIndex ];

    m_iProcessedFrame = 0;
    // ¾×¼Ç ½ºÅ©¸³Æ®!!
    for( int i = iStartFrame; i < iEndFrame; ++i )
    {
        //char strFunc[MAX_PATH];
        //sprintf( strFunc, "%s(%d,%d)", kMonsterAction.m_strFunc.c_str(), m_iMonsterIndex, i );

        // ¾×¼Ç ½ºÅ©¸³Æ®¸¦ µ¹¸°´Ù!
        //KGCLuabinder::getInstance().DoString( strFunc );

        bool bUseAIScript =false;
        SDotaTemplate* pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();
        if( pCurDotaInfo &&
            false == pCurDotaInfo->bUseAIFromScript &&
            GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
        {// ¾×¼Ç ÇÔ¼ö
            switch( m_iMonsterType )
            {
            case MON_DOTA_CORE:
                if( strstr( kMonsterAction.m_strFunc.c_str(), "DotaCore_WaitFunc" ) )
                    m_HardAI_Core.Action_Wait(i, m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "DotaCore_DieFunc" ) )
                    m_HardAI_Core.Action_Die(i, m_iMonsterIndex);
                break;
            case MON_DOTA_ANCIENT_SERDIN:
            case MON_DOTA_ANCIENT_CANABAN:
                if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_WaitFunc" ) )
                    m_HardAI_Ancient.Action_WaitFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_WalkFunc" ) )
                    m_HardAI_Ancient.Action_WalkFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_Attack01Func" ) )
                    m_HardAI_Ancient.Ancient_Attack01Func( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_Attack02Func" ) )
                    m_HardAI_Ancient.Ancient_Attack02Func( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_JumpDownFunc" ) )
                    m_HardAI_Ancient.Ancient_JumpDownFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_JumpToWait" ) )
                    m_HardAI_Ancient.Ancient_JumpToWait( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_DieFunc" ) )
                    m_HardAI_Ancient.Ancient_DieFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_StoneCurseFunc" ) )
                    m_HardAI_Ancient.Ancient_StoneCurseFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_IceCurseFunc" ) )
                    m_HardAI_Ancient.Ancient_IceCurseFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "Ancient_HoldFunc" ) )
                    m_HardAI_Ancient.Ancient_HoldFunc( i,  m_iMonsterIndex);
                break;
            case MON_DOTA_GUARDIAN_TOWER:
                if( strstr( kMonsterAction.m_strFunc.c_str(), "DoTaGuardianTower_WaitFunc" ) )
                    m_HardAI_GuardianTower.Action_WaitFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "DoTaGuardianTower_ATK" ) )
                    m_HardAI_GuardianTower.Action_AttackFunc( i,  m_iMonsterIndex);
                else if( strstr( kMonsterAction.m_strFunc.c_str(), "DoTaGuardianTower_DieFunc" ) )
                    m_HardAI_GuardianTower.Action_DieFunc( i,  m_iMonsterIndex);
                break;
            case MON_DOTA_GUARD_TOWER:
                if( strstr( kMonsterAction.m_strFunc.c_str(), "DoTaGuardTower_WaitFunc" ) )
                    m_HardAI_GuardTower.Action_WaitFunc( i,  m_iMonsterIndex);
                if( strstr( kMonsterAction.m_strFunc.c_str(), "DoTaGuardTower_ATK" ) )
                    m_HardAI_GuardTower.Action_AttackFunc( i,  m_iMonsterIndex);
                if( strstr( kMonsterAction.m_strFunc.c_str(), "DoTaGuardTower_DieFunc" ) )
                    m_HardAI_GuardTower.Action_DieFunc( i,  m_iMonsterIndex);
                break;
            default:
                bUseAIScript = true;
            }
        }
        else
            bUseAIScript = true;

        if( bUseAIScript )
        {
        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
        CHECK_STACK(L)

		lua_checkstack( L, 10 );					// ½ºÅÃ »çÀÌÁî¸¦ Ã¼Å© ÇÕ´Ï´Ù. ( 20º¸´Ù ÀÛÀ¸¸é ¿©À¯°ø°£À» È®º¸ ÇÕ´Ï´Ù. )
		lua_pushstring( L, kMonsterAction.m_strFunc.c_str() );		// Action Å×ÀÌºíÀ» ºÒ·¯¿À±â À§ÇØ ÀÌ¸§À» Push ÇÕ´Ï´Ù.
		lua_rawget( L, LUA_GLOBALSINDEX );			// ±Û·Î¹ú Å×ÀÌºí¿¡¼­ Action Å°¸¦ Ã£½À´Ï´Ù.
		if( lua_isfunction( L, -1 ) )
		{
			lua_pushinteger( L, m_iMonsterIndex );
			lua_pushinteger( L, i );
            lua_pcall( L, 2, 0, 0 );
		}
		else
		{
			lua_pop( L, -1 );
		}
        }
        m_iProcessedFrame++;

        if( m_iOldMotion != m_aiMotion[ iIndex ] )
            return;
    }

    // AI °¡ µ¿ÀÛÇÑ µÚ TargetDelayTimeÀ» °¨¼Ò ½ÃÅ²´Ù.
    if( m_bMovement )
    {
        if( m_dwTargetingDelayTime > 0 ) 
            --m_dwTargetingDelayTime;
    }

    // ¸ó½ºÅÍ°¡ Å¸ÄÏÆÃÀ» ¸øÇÒ ÇÁ·¹ÀÓÀ» Á¦¾îÇÑ´Ù.
    if( 0 < m_iNoTargettingFrame )
        --m_iNoTargettingFrame;

    // ¾ÆÁöÆ® ¹èÄ¡ Ä³¸¯ÅÍÀÏ °æ¿ì ´ë»ç ³¯¸± ½Ã°£ µ¹¸°´Ù. 
    if( m_iCharType != GC_CHAR_INVALID ) { 
        --m_iMentCount;
        SetMentCount();
    }

    return;
}

void MONSTER::CallLuaFucn_MoveFail()
{
    int iIndex = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iIndex = 0;

    std::string strMotion = g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_aiMotion[ iIndex ] );
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mit = mapAction.find( strMotion );
    if( mit == mapAction.end() )
        return;

    KGCMonsterAction &kMonsterAction = mit->second;

    bool bUseAIScript = false;
    SDotaTemplate* pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();
    if( pCurDotaInfo &&
        false == pCurDotaInfo->bUseAIFromScript &&
        GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() )
    {
        switch( m_iMonsterType )
        {
        case MON_DOTA_ANCIENT_SERDIN:
        case MON_DOTA_ANCIENT_CANABAN:
            if( strstr(kMonsterAction.m_strMoveFailFunc.c_str(), "Ancient_MoveFail") )
                m_HardAI_Ancient.Action_WalkFailFunc( m_iMonsterIndex );
            break;
        case MON_DOTA_CORE:
        case MON_DOTA_GUARDIAN_TOWER:
        case MON_DOTA_GUARD_TOWER:
            break;
        default:
            bUseAIScript = true;
        }
    }
    else
    {
        bUseAIScript = true;
    }

    if( bUseAIScript )
    {
        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
        CHECK_STACK(L)
            lua_checkstack( L, 10 );
        lua_pushstring( L, kMonsterAction.m_strMoveFailFunc.c_str() );
        lua_rawget( L, LUA_GLOBALSINDEX );
        if( lua_type( L, -1 ) == LUA_TFUNCTION )
        {
            lua_pushinteger( L, m_iMonsterIndex );
            lua_pcall( L, 1, -1, 0 );
        }
        else
        {
            lua_pop( L, -1 );
        }

    }
}

void MONSTER::CrashCheck()
{
    int iIndex = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iIndex = 0;

    // ÁÂ¿ì¿¡ º®¿¡ ´ê¾ÒÀ»¶§.. ³Ñ¾î°¡Áö ¾Ê°Ô ÇØ ÁØ´Ù ( ¹Ù¿îµù ¹Ú½ºÀÇ ÁÂ¿ì 55%ÀÇ Å©±â°¡ º®¹ÛÀ¸·Î ³Ñ¾î°¡Áö ¾Ê°Ô ÇÑ´Ù )

    float fPositionLimit = 0.55f;

    if( GetDefaultMonster( m_iMonsterType ) == MON_KING_CRAB )
    {
        fPositionLimit = 0.3f;
        if( GetState() == "ATTACK01" || 
            GetState() == "ATTACK04" )
            goto monsterMoveEnd;
    }

	if( GetDefaultMonster( m_iMonsterType ) == MON_DRAKE_WORM ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_TREE ||
        GetDefaultMonster( m_iMonsterType ) == MON_ANGRYBOSS_DIVINE_TREE ||
        GetDefaultMonster( m_iMonsterType ) == MON_SUPERMON_DIVINE_TREE ||        
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH1 ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH2 ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH3 ||
        GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH4 ||
        GetDefaultMonster( m_iMonsterType ) == MON_TALIN_DSR ||
        GetDefaultMonster( m_iMonsterType ) == MON_TALIN_TANK_DSR ||
        GetDefaultMonster( m_iMonsterType ) == MON_HELL_DEATH_WORM
		)
	{
		goto monsterMoveEnd;
	}

    if( false == m_bNotCheckStateWall )
    {
        if( m_afX[ iIndex ] > SiKGCFantasticMap()->GetPixelWidth() - ( m_fWidth * fPositionLimit ) )
        {
            SetLatencyX( iIndex, SiKGCFantasticMap()->GetPixelWidth() - ( m_fWidth * fPositionLimit ) );
            m_fSpeedX = 0.0f;
        }
        else if( m_afX[ iIndex ] < ( m_fWidth * fPositionLimit ) )
        {
            SetLatencyX( iIndex, ( m_fWidth * fPositionLimit ) );
            m_fSpeedX = 0.0f;
        }
    }

monsterMoveEnd:
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    if( false == kMonsterTemplate.bNoPushCheckToDamage )
	    PushCheckWithMonster();

    // °ø°Ý µ¥¹ÌÁö°¡ ÀÖÀ» °æ¿ì..
    
    if( !m_vecPhsAtk.empty() && !m_pObject->IsCullObject( true ) ) 
    {
        CheckAttack();
    }
}

void MONSTER::PushCheckWithMonster() 
{
	g_MyD3D->m_pDamageManager->PushCheckWithMonster( m_iMonsterIndex );
}

bool MONSTER::CheckPierceBlockXPos(float fBeforeX, float fAfterX, float fY)
{
    if( SiKGCFantasticMap()->CheckPierceBlockXPos(fBeforeX, fAfterX, fY) )
    {
        return true;
    }
    else
    {
        // ¸ó½ºÅÍ¿Í º®»çÀÌ¿¡ Ä³¸¯ÅÍ°¡ ÀÖ´ÂÁö È®ÀÎ
        for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

            if( pPlayer->m_kUserInfo.nUserState != GC_RUS_PLAYING )
                continue;

            if( g_MyD3D->IsSameTeam_PlayerAndMonster(iLoop, m_iMonsterIndex) )
                continue;

            float fAfterPlayerWidth;
            GCCollisionRect<float>& rtBody = pPlayer->GetCollisionRect();
            D3DXVECTOR3 vPlayerPos = pPlayer->GetPosition();

            // ÇÃ·¹ÀÌ¾î¿Í ¸ó½ºÅÍ°¡ Ãæµ¹µÇ¾úÀ» ¶§¸¸
            for( int iRect=0; iRect<GetPushRectNum(); ++iRect)
            {
                GCCollisionRect<float> kRect = GetPushBoundRect(iRect);
                GCCollisionRect<float> kColl;
                if( rtBody.CheckCollision( kRect, &kColl ) )
                {
                    if(fBeforeX < vPlayerPos.x )
                        fAfterPlayerWidth = fAfterX + rtBody.GetWidth();
                    else
                        fAfterPlayerWidth = fAfterX - rtBody.GetWidth();

                    if( SiKGCFantasticMap()->CheckPierceBlockXPos(fBeforeX, fAfterPlayerWidth, fY) )
                    {
                        return true;
                    }
                    break;
                }
            }
        }
    }
    return false;
}

bool MONSTER::CheckAttack()
{
    int iIndex = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iIndex = 0;

    if( IsDetermineWin )
        return false;

    bool bSuccessAttack = false;

    for( std::vector<KMonsterPhysicAttack>::iterator it = m_vecPhsAtk.begin();
         it != m_vecPhsAtk.end(); it++ )
    {
        bSuccessAttack = false;
         (*it).m_iLifeTime--;

        GCCollisionRect<float> rtWeapon;

        //rect ¼³Á¤
        {
            D3DXVECTOR3 vecBase, vecEnd;
            D3DXMATRIX matBone;
            const D3DXMATRIX* pWorld;
            pWorld = m_pObject->GetWorldMat();

            if( (*it).m_iBaseBone == 0 && (*it).m_iEndBone == 0 )
            {
                m_pObject->GetAngleBone( 0, &matBone );
                matBone = matBone * *pWorld;

                float fRangeX, fRangeY;

                fRangeX = ( m_fAttackBoundBoxWidth ) / 2.0f;
                fRangeY = ( m_fAttackBoundBoxHeight ) / 2.0f;
                      

                if( m_abIsRight[ iIndex ] )
                    rtWeapon.SetRect( matBone._41, matBone._42 - fRangeY, matBone._41 + fRangeX ,matBone._42 + fRangeY );
                else
                    rtWeapon.SetRect( matBone._41, matBone._42 - fRangeY, matBone._41 - fRangeX ,matBone._42 + fRangeY );
            }
            else
            {
                m_pObject->GetAngleBone( (*it).m_iBaseBone, &matBone );
                matBone = matBone * *pWorld;
                SETVECTOR3( vecBase , matBone._41, matBone._42, 1.0f );

                m_pObject->GetAngleBone( (*it).m_iEndBone, &matBone );
                matBone = matBone * *pWorld;
                SETVECTOR3( vecEnd , matBone._41, matBone._42, 1.0f );

                rtWeapon.SetRect( vecBase.x, vecBase.y, vecEnd.x, vecEnd.y);
            }
        }

        rtWeapon.AdjustRect();

        rtWeapon.m_Left     -= (*it).m_fRange / 2.0f;
        rtWeapon.m_Top      += (*it).m_fRange / 2.0f;
        rtWeapon.m_Right    += (*it).m_fRange / 2.0f;
        rtWeapon.m_Bottom   -= (*it).m_fRange / 2.0f;

        rtWeapon.AdjustRect();

#if !defined( __PATH__ ) // ¸ó½ºÅÍÀÇ ¹Ù¿îµù¹Ú½º
        {
            if( g_kGlobalValue.m_bRenderBoundBox )
            {
                D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
                SETVECTOR3( lefttop, rtWeapon.m_Left, rtWeapon.m_Top , 0.4f );
                SETVECTOR3( righttop, rtWeapon.m_Right, rtWeapon.m_Top , 0.4f );
                SETVECTOR3( leftbottom, rtWeapon.m_Left, rtWeapon.m_Bottom , 0.4f );
                SETVECTOR3( rightbottom, rtWeapon.m_Right, rtWeapon.m_Bottom , 0.4f );

                g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
                    lefttop, righttop, leftbottom, rightbottom,0,0,0,
                    D3DCOLOR_ARGB(180, 200, 0, 0));
            }
        }
#endif

        AttackPlayer( rtWeapon, iIndex, (*it), bSuccessAttack );
        AttackMonster( rtWeapon, iIndex, (*it), bSuccessAttack );
        // ¹°¸® °ø°ÝÀ¸·Î µ¥¹ÌÁö¿¡ Å¸°ÝÀ» Ã¼Å©ÇÑ´Ù.
        g_MyD3D->m_pDamageManager->CrashWithMonsterPhsAtk( m_iMonsterIndex, &rtWeapon );

        if( (*it).m_iLifeTime <= 0 || bSuccessAttack )
        {
            it = m_vecPhsAtk.erase(it);
            if( m_vecPhsAtk.empty() )
                break;
            else
                --it;
        }
    }

    
    return bSuccessAttack;
}

// ¸ó½ºÅÍ°¡ ÇÃ·¹ÀÌ¾î¸¦ ¶§¸± ¶§
bool MONSTER::AttackPlayer( GCCollisionRect<float> rtWeapon, int iIndex, KMonsterPhysicAttack &kPhsAtk, bool &bSuccessAttack ) 
{
    bool bResulte = true;
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( pPlayer->m_kUserInfo.bLive == false )
            continue;
        if( pPlayer->m_cLife == 0 )
            continue;
        if( pPlayer->Slow_Count )
            continue;
        if( pPlayer->IsInvinsible( false ) == true )
            continue;
        if( g_MyD3D->IsSameTeam_PlayerAndMonster(i, m_iMonsterIndex) )
            continue;
        
        GCCollisionRect<float>& rtBody = pPlayer->GetCollisionRect();
        rtBody.m_Bottom -= 0.2f;
        rtBody.m_Top -= 0.2f;

        KGCEmbarkObject* pEmbark = SiKGCEmbarkManager()->GetEmbarkInstance( pPlayer->GetEmbarkUID() );
        if( pEmbark )
        {
            rtBody = pEmbark->GetAttackRect();
            rtBody.m_Bottom += 0.25f;
            rtBody.m_Top += 0.25f;
        }
        
        GCCollisionRect<float> rtColl;
        
        // °ø°Ý ¹üÀ§¾È¿¡ µé¾î°¡´ÂÁö Ã¼Å©ÇÑ´Ù.. °ø°Ý¹üÀ§´Â ÁÂ¿ì ¸ó½ºÅÍ ¹Ù¿îµù¹Ú½º Å©±âÀÇ ¹Ý 
        if( rtBody.CheckCollision( rtWeapon, &rtColl ) )
        {
            if( false == pPlayer->GetPermissionDamage().empty() )
            {
                if( (timeGetTime() - m_dwMissAttackTime) > MISS_ATTACK_TERM_TIME )
                {
                    m_dwMissAttackTime = timeGetTime();
                    D3DXVECTOR2 hitPos = D3DXVECTOR2( rtColl.GetCenterX() + 1.0f, rtColl.GetCenterY() + 0.2f );
                    pPlayer->ShowMissAttack( hitPos.x, hitPos.y );
                }
            }
            m_iSlowCount = 8;

            if( pEmbark )
            {
                pEmbark->SetWhatDamage( 0 );
                pEmbark->CallLuaFunc_Callback( KGCEmbarkTemplate::CALLBACK_CRASH_DAMAGE );
            }

            // °ø°ÝÀ» ¼º°øÇß´Ù!
            bSuccessAttack = true;
            rtColl.m_Top += 0.2f;
            rtColl.m_Bottom += 0.2f;

            // ³»°¡ Å¸°Ý ´çÇßÀ» ¶§..
            if( pPlayer->IsLocalPlayer() ) 
            {
                if ( !pPlayer->OnDamagePreProcess( m_iMonsterIndex, false, true ) )
                {
                    bResulte = false;
                    break;
                }

                // ·Î³­ 2Â÷ ºí·ÎÅ·!!
                if( pPlayer->MetamorphosisForm == FORM_NORMAL && pPlayer->m_bSpecialAttack == false &&
                    ( pPlayer->Extra_Char_Num == CID_RONAN3 || pPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME ||
                    ( pPlayer->GetCurrentChar().iCharType == GC_CHAR_SIEG && pPlayer->CheckSkill(SID_SIEG1_FURY_TYPE_B_LV3))) &&
                    pPlayer->IsDamagedMotion() == false )
                {
                    static int b = g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME;
                    if ( pPlayer->bIsRight == false )
                    {
                        for ( b = 0; b < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++b )
                        {
                            if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_RIGHT][b] ) break;
                        }
                    }	
                    else
                    {
                        for ( b = 0; b < g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME; ++b )
                        {
                            if ( g_MyD3D->MyCtrl->m_bUserInputGuard[g_MyD3D->MyCtrl->GUARD_LEFT][b] ) break;
                        }
                    }

                    if ( b != g_MyD3D->MyCtrl->BLOCK_ENABLE_FRAME )
                    {
                        if( pPlayer->GetCurrentChar().iCharType == GC_CHAR_SIEG ) {
                            pPlayer->SendBurnningPoint(pPlayer->m_iPlayerIndex);
                            if( pPlayer->GetBurnningPoint() >= 0.5f ) {
                                if( pPlayer->Extra_Char_Num == CID_SIEG4 ) {
                                    pPlayer->Direct_Motion_Input( MID_SIEG4_SKILL_GUARD );
                                } else if( pPlayer->Extra_Char_Num == CID_SIEG3 ) {
                                    pPlayer->Direct_Motion_Input( MID_SIEG3_SKILL_GUARD );
                                } else if( pPlayer->Extra_Char_Num == CID_SIEG2 ) {
                                    pPlayer->Direct_Motion_Input( MID_SIEG2_SKILL_GUARD );
                                } else {
                                    pPlayer->Direct_Motion_Input( MID_SIEG1_SKILL_GUARD );
                                }
                            }
                        } else if( pPlayer->GetCurrentChar().iCharType == GC_CHAR_LIME ) {
                            if( pPlayer->IsMagicEffect( EGC_EFFECT_LIME2_SELF_AURA_DEFFENCE ) && pPlayer->CheckSkill( SID_LIME2_SELF_AURA_DEFFENSE_PLUS_LV3 ) ) {
                                if( pPlayer->GetMP() >= 0.3f ) {
                                    pPlayer->Direct_Motion_Input( MID_LIME2_COUNTER_ATK );
                                }
                            }
                        }else{
							if (pPlayer->IsMagicEffect(EGC_EFFECT_RONAN3_BLOCK_COOL) == false)
							{
								pPlayer->Direct_Motion_Input(MID_RONAN3_BLOCKING);
							}
						}

                        bResulte = false;
                        break;
                    }
                }

                pPlayer->Slow_Count = 8;

                // Attacker¼ÂÆÃ( ¼ÒÈ¯µÈ ¸ó½ºÅÍ°¡ Attacker¶ó¸é ÇÃ·¹ÀÌ¾î Index¸¦ ¼ÂÆÃÇØÁÖÀÚ. )
                PLAYER *pSummonPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerUID );
                if( pSummonPlayer )
                {
                    pPlayer->SetLastKillmePlayer( pSummonPlayer->m_iPlayerIndex );
                }
                else
                    pPlayer->SetLastKillmePlayer( m_iMonsterIndex + MAX_PLAYER_NUM );

                // ³ª¸¦ ¶§¸° ¸ó½ºÅÍ¸¦ º¸°Ô ÇÑ´Ù
                if( m_abIsRight[ iIndex ] )     pPlayer->bIsRight = false;
                else                            pPlayer->bIsRight = true;

                // ¸®¾×¼Ç Å¸ÀÔ¿¡ µû¶ó¼­ º¯ÇÏ´Â ¸ð¼ÇÀ¸·Î ¹Ù²ãÁØ´Ù!
                if( kPhsAtk.m_iDamageType == PAR_SMALL_DAMAGE_NODOWN ) {
                    pPlayer->Direct_Motion_Input( MID_COMMON_SMALLDAMAGE );
                }
                else if( kPhsAtk.m_iDamageType == PAR_BIG_DAMAGE_NODOWN ) {
                    pPlayer->Direct_Motion_Input( MID_COMMON_BIGDAMAGE );
                }
                else if( kPhsAtk.m_iDamageType == PAR_SMALL_DAMAGE ) {
                    pPlayer->Direct_Motion_Input( MID_COMMON_SMALLDAMAGE );
                }
                else {
                    pPlayer->Direct_Motion_Input( MID_COMMON_BIGDAMAGE );
                }

                // ¸Â¾ÒÀ» ¶§ ³¯¾Æ°¡´Â°Å..                
                if( kPhsAtk.m_iDamageType == PAR_DOWN || !pPlayer->IsContact )
                {
                    if( kPhsAtk.m_iDamageType != PAR_SMALL_DAMAGE_NODOWN && kPhsAtk.m_iDamageType != PAR_BIG_DAMAGE_NODOWN ) {
                        if( pPlayer->bIsRight )
                            pPlayer->x_Speed = -0.02f * ( 1.0f + (float)kPhsAtk.m_iDamage / 10.0f );
                        else
                            pPlayer->x_Speed = 0.02f * ( 1.0f + (float)kPhsAtk.m_iDamage / 10.0f );

                        pPlayer->Direct_Motion_Input( MID_COMMON_DOWN_AND_STANDUP );
                    }                    
                }
                else if( kPhsAtk.m_iDamageType == PAR_REVERSE_DOWN || !pPlayer->IsContact )
                {
                    if( kPhsAtk.m_iDamageType != PAR_SMALL_DAMAGE_NODOWN && kPhsAtk.m_iDamageType != PAR_BIG_DAMAGE_NODOWN ) {
                        if( pPlayer->bIsRight )
                            pPlayer->x_Speed = -0.02f * ( 1.0f + (float)kPhsAtk.m_iDamage / 10.0f );
                        else
                            pPlayer->x_Speed = 0.02f * ( 1.0f + (float)kPhsAtk.m_iDamage / 10.0f );

                        pPlayer->bIsRight = !pPlayer->bIsRight;

                        pPlayer->Direct_Motion_Input( MID_COMMON_DOWN_AND_STANDUP );
                    }
                }
                else if( kPhsAtk.m_iDamageType >= PAR_FRONTDOWN_0 &&
                         kPhsAtk.m_iDamageType <= PAR_FRONTDOWN_DROPDAMAGE_LAST ) // ¶ç¿ì±â °ø°Ý
                {
                    //¶ß±â¸¸ ÇÏ°í µ¥¹ÌÁö ¾øÀ½
                    int iGravity = 0;
                    int iDropDamage = 0;
                    if( kPhsAtk.m_iDamageType <= PAR_FRONTDOWN_LAST )
                    {
                        iGravity = kPhsAtk.m_iDamageType - PAR_FRONTDOWN_0;
                    }
                    else
                    {
                        iGravity = kPhsAtk.m_iDamageType - PAR_FRONTDOWN_DROPDAMAGE_0;
                        iDropDamage = iGravity / 2;
                    }

                    // »ó´ëÆí ¶ç¿ì±â °ø°Ý
                    pPlayer->Gravity = pPlayer->vPos.y;
                    pPlayer->y_Speed = (float)( iGravity ) * 0.005f + 0.025f;
                    pPlayer->Last_Gravity = 0.01f * iDropDamage;                        
                    pPlayer->Direct_Motion_Input( MID_COMMON_DAMAGED_BY_FLYATK );
                }

                if( pPlayer->y_Speed > 0 && pPlayer->uiMotion != pPlayer->GetOwnMotion( MID_COMMON_DAMAGED_BY_FLYATK ) )
                    pPlayer->y_Speed = 0;

                ChampionMonsterAttack( i );
            }

            // m_fHP¸¦ ±ï´Â´Ù!
            PLAYER::DamageInfo damage;
            damage.SetDamage( (float)kPhsAtk.m_iDamage * GetDamageFactor( pPlayer ) );
            damage.SetAttacker( PLAYER::DamageInfo::DAMAGE_MONSTER, m_iMonsterIndex );

            pPlayer->Change_HP( damage );

            pPlayer->ShieldCountAttack( pPlayer->m_iPlayerIndex, m_iMonsterIndex, (float)kPhsAtk.m_iDamage * 0.03f );
            pPlayer->ReflectionDamage( pPlayer->m_iPlayerIndex, m_iMonsterIndex, (float)kPhsAtk.m_iDamage * 0.03f );

			
            // »ç¿îµå, Å¸°Ý ÆÄÆ¼Å¬
            g_KDSound.Play( "35" );
            g_MyD3D->MySparks->Particles( rtColl.GetCenterX(), rtColl.GetCenterY(), ( LATENCY + SHOWLATENCY ), 15, 15 );
            g_MyD3D->MySparks->Add( HIT_LIGHT, rtColl.GetCenterX(), rtColl.GetCenterY(), ( LATENCY + SHOWLATENCY ), 0.7f );
        }
    }

    return bResulte;
}

// ÀÌ ¸ó½ºÅÍ°¡ ´Ù¸¥ ¸ó½ºÅÍ¸¦ ¶§¸± ¶§
bool MONSTER::AttackMonster( GCCollisionRect<float> rtWeapon, int iIndex, KMonsterPhysicAttack &kPhsAtk, bool &bSuccessAttack )
{
    bool UsePower = false;
    GCCollisionRect<float> rtMonster;
    GCCollisionRect<float> rtCollision;

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if( !pMonster->IsLive() )
			continue;

		if( g_MyD3D->IsSameTeam_MonsterAndMonster(mit->first, m_iMonsterIndex) )
			continue;

		for(int j = 0 ; j < pMonster->GetAttackBoundRectNum(); j++ )
		{
			rtMonster = pMonster->GetAttackBoundRect(j);

			if( pMonster->IsInvincible() || !rtMonster.CheckCollision( rtWeapon, &rtCollision ) )
				continue;

            if( false == pMonster->GetPermissionDamage().empty() )
            {
                if( ( timeGetTime() - m_dwMissAttackTime ) > MISS_ATTACK_TERM_TIME )
                {
                    m_dwMissAttackTime = timeGetTime();
                    D3DXVECTOR2 hitPos = D3DXVECTOR2( rtCollision.GetCenterX() + 1.0f, rtCollision.GetCenterY() + 0.2f );
                    pMonster->ShowMissAttack( hitPos.x, hitPos.y );
                }
            }

			pMonster->SetLastAttackedBoxIndex( j );
			bool bCheckHit = pMonster->OnMonsterDamage( m_iMonsterIndex, -1, (float)kPhsAtk.m_iDamage, (EGCAttackType)kPhsAtk.m_iDamageType, (EGCAttackDirection)3, 0.0f, 0.0f, false );
			if( bCheckHit )
			{
				D3DXVECTOR2 hitPos = D3DXVECTOR2( rtCollision.GetCenterX() + 1.0f  ,rtCollision.GetCenterY() + 0.2f );
				pMonster->SetHitPos( hitPos );
				bSuccessAttack = true;
				break;
			}
		}

		pMonster->SetAttackedMonster(true);

	}

    return bSuccessAttack;
}

GCCollisionRect<float>& MONSTER::GetAttackBoundRect( int iIndex )
{
    return m_vecAttackRect[iIndex];
}

GCCollisionRect<float>& MONSTER::GetPushBoundRect( int iIndex, bool bIgnoreActive /*=false*/ )
{
    static GCCollisionRect<float> rect;

    if ( (int)m_vecPushRect.size() <= iIndex )
        return rect;

    if( bIgnoreActive )
        return m_vecPushRect[ iIndex ];

    if( m_vecPushRect[iIndex].bActive )
        return m_vecPushRect[iIndex];    

    return rect;

}

void MONSTER::SetSuperArmor( bool bSuperArmor, bool bAttackSuper /* = false */ )
{
    m_bIsSuperArmor = bSuperArmor;
    m_bAttackSuperArmor = bAttackSuper;
}

void MONSTER::EarthQuake( int iShakeCamera, float fGravity, float fDamage )
{
    g_KDSound.Play( "47" );

    // Ä«¸Þ¶ó Èçµé¸²
    g_kCamera.SetEarthQuake( iShakeCamera );

    if( fGravity > 0.0f )
    {
        if( !IsDetermineWin )
        {
            for( char i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
                    continue;

                float fFactor = 1.0f;
                if( m_iMonsterType == MON_KAZEAZE2 && true == g_MyD3D->MyPlayer[i]->m_bKazeazeDefence )
                {
                    fFactor = 0.5f;
                }
                g_MyD3D->MyPlayer[i]->ReverseGravity( MAX_PLAYER_NUM, fGravity, fDamage * fFactor );
            }
        }
    }
}

void MONSTER::EarthQuakeWithRange( int iShakeCamera, float fGravity, float fDamage, float fRange )
{
    g_KDSound.Play( "47" );

    // Ä«¸Þ¶ó Èçµé¸²
    g_kCamera.SetEarthQuake( iShakeCamera );

    if( fGravity > 0.0f )
    {
        if( !IsDetermineWin )
        {
            for( char i = 0; i < MAX_PLAYER_NUM; ++i )
            {
                PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
                if( pPlayer->m_kUserInfo.bLive == false )
                    continue;

                float fDist = DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );

                // ¹üÀ§ ¾ÈÀÇ ³à¼®¸¸ Àû¿ëµÈ´Ù!
                if( fDist > fRange )
                    continue;

                float fFactor = 1.0f;
                if( m_iMonsterType == MON_KAZEAZE2 && true == pPlayer->m_bKazeazeDefence )
                {
                    fFactor = 0.5f;
                }
                pPlayer->ReverseGravity( MAX_PLAYER_NUM, fGravity, fDamage * fFactor );
            }
        }
    }
}

void MONSTER::ShadowRender()
{
    if( g_MyD3D->m_KGCOption.GetQualityCount() <= 3)
        return;

    if( !m_bLive )
        return;

    if( !m_bRender || !g_kGlobalValue.m_bRenderMonster )
        return;

    int iIndex = 0;
    //int iIndex = LATENCY_INDEX;
    //if( !g_kGlobalValue.m_kUserInfo.bHost )
    //    iIndex = 0;


    m_fShadowY = g_MyD3D->MyStg->Get_LowHeight( m_afX[iIndex], m_afY[0] );

	if( (int)m_vecMotionList.size() <= m_aiMotion[ iIndex ] || 0 > m_aiMotion[ iIndex ] )
		return;

	if( m_vecMotionList[ m_aiMotion[ iIndex ] ]->GetNum_Frame() <= m_aiFrame[ iIndex ] )
		return;

    float fMotionY = m_vecMotionList[ m_aiMotion[ iIndex ] ]->GetFrameData( m_aiFrame[ iIndex ] ).Pos_y;
    float fDistance = m_afY[iIndex] - m_fShadowY + ( fMotionY + m_fHeight / 2.0f );

    if( fDistance < -1.0f )
        fDistance = -1.0f;

    D3DXMATRIX mat1, mat2, mat3;
    float fZ = 4.0f;
    D3DXMatrixScaling( &mat1, m_fScale, m_fScale, .0001f );
    D3DXMatrixTranslation( &mat2, m_afX[iIndex] - 1.0f, m_fShadowY - 0.52f, 2.0f );
    D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
    D3DXMatrixMultiply( &mat1, &mat3, &g_kCamera.m_matCamera );

    D3DXVECTOR4 vResult;
    D3DXVec3Transform( &vResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f ), &mat1 );

    g_pGraphicsHelper->SimpleShadowRender( vResult.x, vResult.y, fDistance, ( m_fHeight * m_fScale * m_fShadowRatio ), fZ );
}

bool MONSTER::OnDamageProcess( int iPlayerIndex, int iWhat, EGCAttackType eAttackType, bool bSpecialAttack, bool bArrowDamage, float fAttackPower_, bool &bSkipDamageMotion_ )
{
	if ( eAttackType == -1 || eAttackType == ATTACKTYPE_BUFF_DAMAGE )
		bSkipDamageMotion_ = true;

    if ( !g_MyD3D->IsPlayerIndex( iPlayerIndex ) )
        return false;

	PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex];

	if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() && pPlayer->IsLocalPlayer() )
	{
		// ÀúÁÖÀÇÁ¶À² ¿ÞÂÊ! º¸½º Å¸°Ý ½Ã 1hit´ç 2%ÀÇ ÇÇ¸¦ Ã¤¿ò!
		if ( m_iMonsterType == MON_JURIOR && g_pMagicEffect->IsMagicEffect( iPlayerIndex, EGC_EFFECT_JURIOR_CURSE_L ) )
		{
			pPlayer->SetHP( pPlayer->GetHP() + (pPlayer->m_fAbility[ABILITY_HP] * 0.02f) );
            return true;
        }
    }

    bool bPhysicalDamage = false;

    if( iWhat == -1 )
        bPhysicalDamage = true;

    if ( ( m_iMonsterType == MON_SETESH || m_iMonsterType == MON_ANGRYBOSS_SETESH || m_iMonsterType == MON_SUPERMON_SETESH ) &&
           ( IsMagicEffect( m_iMonsterIndex, EGC_EFFECT_SETESH_PROTECT ) || IsMagicEffect( m_iMonsterIndex, EGC_EFFECT_ANGRY_SETESH_PROTECT ) ) &&
                false == CheckBackAttack( iPlayerIndex, bPhysicalDamage ) )
    {
        float fPosx = 0.0f;
        float fPosy = 0.45f;
        if ( this->GetIsRight() )
        {
            AddParticleNoDirection("Setesh_Field_01_R", fPosx, fPosy);
        }
        else
        {
            AddParticleNoDirection("Setesh_Field_01", fPosx, fPosy);
        }
        AddParticle("Setesh_Field_02", fPosx, fPosy);
        AddParticle("Setesh_Field_03", fPosx, fPosy);
        AddParticle("Setesh_Field_04", fPosx, fPosy);

#ifdef LEVEL_DESIGN_STAT_CALC
        float fChargeHP = fAttackPower_ * 2.0f * g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATK]*0.01f * 1.4f;
#else
        float fChargeHP = fAttackPower_ * 2.0f * g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATK] * 1.4f;
#endif

        // µ¥¹ÌÁö ÄÝ¹é
        SetDamageDiff( fChargeHP );
        ActionCallBack( "DamageBeforeChange" );
        if( GetDamageDiff() != fChargeHP )
            fChargeHP = GetDamageDiff();

        SetHP( m_fHP + fChargeHP );

        return false;
    }

    if ( (m_iMonsterType == MON_THUNDER_HAMMER_DEFENDER || m_iMonsterType == MON_THUNDER_HAMMER_ROYALGUARD) && false == CheckBackAttack( iPlayerIndex, bPhysicalDamage ) )
    {
        if ( eAttackType == ATTACKTYPE_NORMAL )
        {
            bSkipDamageMotion_ = true;
        }
        else if ( eAttackType == ATTACKTYPE_DOWN )
        {
            // ´Ù¿î °ø°Ý¿¡ ¸é¿ª!
            m_fDefRatio = 10.0f;
        }

        return true;
    }

    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() && pPlayer->IsLocalPlayer() )
    {
        // ¹è¹ÝÀÇ ÀúÁÖ °É·ÈÀ» ¶§´Â... ³ªÀÇ °ø°Ý·ÂÀÇ 50%ÀÇ µ¥¹ÌÁö¸¦ ³»°¡ ¹Þ´Â´Ù!
        if ( g_pMagicEffect->IsMagicEffect( iPlayerIndex, EGC_EFFECT_MON_NEMOPHILLA_FALL ) )
        {
            float fChargeHP = bSkipDamageMotion_ * 2.0f * g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATK] * 1.4f;

            // µ¥¹ÌÁö ÄÝ¹é
            SetDamageDiff( fChargeHP );
            ActionCallBack( "DamageBeforeChange" );
            if( GetDamageDiff() != fChargeHP )
                fChargeHP = GetDamageDiff();

            pPlayer->SetHP( pPlayer->GetHP() - (fAttackPower_) );

            return true;
        }
    }

    if (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() && pPlayer->IsLocalPlayer())
    {
        // ¹è¹ÝÀÇ ÀúÁÖ °É·ÈÀ» ¶§´Â... ³ªÀÇ °ø°Ý·ÂÀÇ 50%ÀÇ µ¥¹ÌÁö¸¦ ³»°¡ ¹Þ´Â´Ù!
        if (g_pMagicEffect->IsMagicEffect(iPlayerIndex, EGC_EFFECT_GCORIGIN_BOF_PHASE1_CURSE))
        {
            float fChargeHP = bSkipDamageMotion_ * 2.0f * g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATK] * 1.4f;

            // µ¥¹ÌÁö ÄÝ¹é
            SetDamageDiff(fChargeHP);
            ActionCallBack("DamageBeforeChange");
            if (GetDamageDiff() != fChargeHP)
                fChargeHP = GetDamageDiff();

            pPlayer->SetHP(pPlayer->GetHP() - (fAttackPower_));

            return true;
        }
    }

    if (GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() && pPlayer->IsLocalPlayer())
    {
        // ¹è¹ÝÀÇ ÀúÁÖ °É·ÈÀ» ¶§´Â... ³ªÀÇ °ø°Ý·ÂÀÇ 50%ÀÇ µ¥¹ÌÁö¸¦ ³»°¡ ¹Þ´Â´Ù!
        if (g_pMagicEffect->IsMagicEffect(iPlayerIndex, EGC_OVEJAR_40FORM_DEBUFF_ON_CHAR))
        {
            float fChargeHP = bSkipDamageMotion_ * 2.0f * g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATK] * 1.4f;

            // µ¥¹ÌÁö ÄÝ¹é
            SetDamageDiff(fChargeHP);
            ActionCallBack("DamageBeforeChange");
            if (GetDamageDiff() != fChargeHP)
                fChargeHP = GetDamageDiff();

            pPlayer->SetHP(pPlayer->GetHP() - (fAttackPower_));

            return true;
        }
    }

    if ( m_iMonsterType == MON_HERO_NEMOPHILLA && GetShield() > 0.0f )
    {
        float fPosx = 0.0f;
        float fPosy = 0.5f;
        if ( this->GetIsRight() )
        {
            if ( CheckBackAttack( iPlayerIndex, bPhysicalDamage ) )
            {
                AddParticleNoDirection("Nemo_Field_01", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02", fPosx, fPosy);
            }
            else
            {
                AddParticleNoDirection("Nemo_Field_01_R", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02_R", fPosx, fPosy);
            }
        }
        else
        {
            if ( CheckBackAttack( iPlayerIndex, bPhysicalDamage ) )
            {
                AddParticleNoDirection("Nemo_Field_01_R", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02_R", fPosx, fPosy);
            }
            else
            {
                AddParticleNoDirection("Nemo_Field_01", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02", fPosx, fPosy);
            }
        }
    }

    return true;
}

bool MONSTER::OnMonsterDamageProcess( int iMonsterIndex, int iWhat, EGCAttackType eAttackType, bool bSpecialAttack, bool bArrowDamage, float fAttackPower_, bool &bSkipDamageMotion_ )
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
	if( pMonster == NULL )
		return false;

    bool bPhysicalDamage = false;

    if( iWhat == -1 )
        bPhysicalDamage = true;

    if ( (m_iMonsterType == MON_SETESH || m_iMonsterType == MON_ANGRYBOSS_SETESH || m_iMonsterType == MON_SUPERMON_SETESH ) && 
        ( IsMagicEffect( m_iMonsterIndex, EGC_EFFECT_SETESH_PROTECT ) || IsMagicEffect( m_iMonsterIndex, EGC_EFFECT_ANGRY_SETESH_PROTECT ) ) &&
            false == CheckMonsterBackAttack( iMonsterIndex, bPhysicalDamage ) )
    {
        float fPosx = 0.0f;
        float fPosy = 0.45f;
        if ( this->GetIsRight() )
        {
            AddParticleNoDirection("Setesh_Field_01_R", fPosx, fPosy);
        }
        else
        {
            AddParticleNoDirection("Setesh_Field_01", fPosx, fPosy);
        }
        AddParticle("Setesh_Field_02", fPosx, fPosy);
        AddParticle("Setesh_Field_03", fPosx, fPosy);
        AddParticle("Setesh_Field_04", fPosx, fPosy);

        float fChargeHP = fAttackPower_ * 2.0f * 1.4f;

        // µ¥¹ÌÁö ÄÝ¹é
        SetDamageDiff( fChargeHP );
        ActionCallBack( "DamageBeforeChange" );
        if( GetDamageDiff() != fChargeHP )
            fChargeHP = GetDamageDiff();

        SetHP( m_fHP + fChargeHP );

        return false;
    }

    if ( (m_iMonsterType == MON_THUNDER_HAMMER_DEFENDER || m_iMonsterType == MON_THUNDER_HAMMER_ROYALGUARD) && false == CheckMonsterBackAttack( iMonsterIndex, bPhysicalDamage ) )
    {
        if ( eAttackType == ATTACKTYPE_NORMAL )
        {
            bSkipDamageMotion_ = true;
        }
        else if ( eAttackType == ATTACKTYPE_DOWN )
        {
            // ´Ù¿î °ø°Ý¿¡ ¸é¿ª!
            m_fDefRatio = 10.0f;
        }

        return true;
    }

    if ( m_iMonsterType == MON_HERO_NEMOPHILLA && GetShield() > 0.0f )
    {
        float fPosx = 0.0f;
        float fPosy = 0.5f;
        if ( this->GetIsRight() )
        {
            if ( CheckMonsterBackAttack( iMonsterIndex, bPhysicalDamage ) )
            {
                AddParticleNoDirection("Nemo_Field_01", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02", fPosx, fPosy);
            }
            else
            {
                AddParticleNoDirection("Nemo_Field_01_R", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02_R", fPosx, fPosy);
            }
        }
        else
        {
            if ( CheckMonsterBackAttack( iMonsterIndex, bPhysicalDamage ) )
            {
                AddParticleNoDirection("Nemo_Field_01_R", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02_R", fPosx, fPosy);
            }
            else
            {
                AddParticleNoDirection("Nemo_Field_01", fPosx, fPosy);
                AddParticleNoDirection("Nemo_Field_02", fPosx, fPosy);
            }
        }
    }

    return true;
}


bool MONSTER::OnDamage( int iPlayerIndex, int iWhat, float fAttackPower, EGCAttackType eAttackType, 
                        EGCAttackDirection eAttackDirection, float fPushTargetX, float fPushTargetY, 
                        bool bSpecialAttack, int nSpecialGrade, bool bSpecialPoint /*= true*/,bool bArrowDamage, bool bPetDamage,
                        bool bSuperDamage_, float *pDamage/* = NULL*/, bool bNoReAction/* = false*/, int iMonsterIndex_, bool bIsDarkSpearDamage, bool bIsNoCritDmg)
{
    // ÀüÀå °ÔÀÌÆ® ¸ó½ºÅÍ´Â ÇÊ»ì±â³ª ÆêÇÊ  ¹«½ÃÇÕ´Ï´Ù. 
    if( m_iMonsterType == MON_INFINITY_DUNGEON_GATE && ( bSpecialAttack || bPetDamage ) ) { 
        return false;
    }

    // µ¥¹ÌÁö´Â µé¾î¿ÀÁö¸¸ ¸ð¼ÇÀº ¹Ù²Ù±â ½È´Ù.. ½´ÆÛ¾Æ¸Ó¶û ¶È°°ÀÝ¾Æ Á¨Àå ¤Ð
    bool bSkipMotion = false;	

    bool bPhysicalDamage = false;

    if( iWhat == -1 )
        bPhysicalDamage = true;

	g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);
	SetAttackedMonster(true);

    m_bIsSpecialAttack = bSpecialAttack;
    SetDamageWhat( iWhat );

    //µ¥¹ÌÁö ¸Â¾ÒÀ» ¶§, ¹º°¡ ÇØ¾ßÇÏ¸é.. ¿©±â´Ù ÇÏÀð ¤Ð ´õ·¯¿î ÄÚµåµéÀÇ ÁýÇÕÃ¼ ¤»
    if ( -1 == iMonsterIndex_ && false == OnDamageProcess(iPlayerIndex, iWhat, eAttackType, bSpecialAttack, bArrowDamage, fAttackPower, bSkipMotion) )
        return false;

	float fDelta = m_vecAttackRect[m_iLastAttackedBoxIndex].fDelta;	

    // Äù½ºÆ® »óÅÂÀÌ¸é Æ¯Á¤ °ø°Ý ¹ÞÀ» ½Ã °ø°ÝÇÑ ÇÃ·¹ÀÌ¾îÀÇ Æ÷ÀÎÆ®¸¦ °è»ê.
    if( bSpecialPoint && GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
    {
        g_pkQuestManager->IncreaseAttackPoint(iPlayerIndex);

        if( bSpecialAttack )
        {
            g_MyD3D->CalcSpecialAttackPoint(iPlayerIndex, m_iMonsterIndex);
        }
    }

    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    if( false == kMonsterTemplate.bNoCrashToIncMP )
    {
        // ¸ó½ºÅÍ¸¦ ¶§¸®¸é ¸¶³ª°¡ Âù´Ù..=¤µ = ( ¼ÒÈ¯µÈ ¸ó½ºÅÍ´Â Á¦¿Ü )
        // ¹æÀåÀ» Æ÷ÇÔÇØ¼­ ¸ðµç ¹æ¿øµéÀÌ Ã³¸®ÇØÁà¾ß ÇÑ´Ù
		if (eAttackType != ATTACKTYPE_BUFF_DAMAGE && (!bSpecialAttack || bPetDamage) && iPlayerIndex == g_MyD3D->Get_MyPlayer() && m_eNPC != NPC_OUR && NULL == g_MyD3D->GetPlayerByUID(m_dwOwnerUID))
		{
			//0.0125*0.9167 = 0.011458   
			//float fIncMP = fAttackPower * 0.011458f * fDelta;
            float fIncMP = 0.15f;

			// ´Á´ë¸é MP Áõ°¡·® 1/2
			// ³×ÇÇ¸²ÀÌ¸é MP Áõ°¡·® 1/3
			PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex];
			switch (pPlayer->MetamorphosisForm)
			{
			    case FORM_WOLF:
			    case FORM_NEPHILIM:
				    fIncMP /= 2.0f;
				    break;
			}

            float mpRecFinal = pPlayer->m_fAbility[ABILITY_MP_RECOVERY_RATE] / 2;

			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA)
				fIncMP = fIncMP * (1.0f + 0.5f);
			else
				fIncMP = fIncMP * (1.0f + mpRecFinal);

			if ((g_kGlobalValue.IsHybridPlayer(g_MyD3D->GetMyPlayer()->GetCharType()) ||
				g_kGlobalValue.IsCharUseSpecial4(g_MyD3D->GetMyPlayer()->GetCharType())) &&
				g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fManaRecoveryRatioByAttackMon != -1) {
				fIncMP *= g_MyD3D->GetMyPlayer()->GetCurFormTemplate().fManaRecoveryRatioByAttackMon;
			}

			bool bIsQuickPlayer = g_kGlobalValue.IsQuickSkillPlayer(g_MyD3D->GetMyPlayer()->GetCharType());

			//  Æê½ºÅ³ »ç¿ë½Ã MP È¸º¹·® Á¶Á¤
			if ( bPetDamage )
			{
				if ( iPlayerIndex == g_MyD3D->Get_MyPlayer() )
				{
					auto fAPIncrease = SiGCPetMagicStone()->GetGlyphEffectAmmount( PET_MAGIC_STONE_EFFECT_TYPE::PMSET_INCREASE_AP );

					if ( bIsQuickPlayer )
					{
						if ( fAPIncrease > 0.0f )
						{
							fIncMP = ( fAPIncrease * GC_GLOBAL_CONST::fPetMPRecovery ) * fIncMP;
							g_MyD3D->ChargeLocalPlayerMana( fIncMP, mpRecFinal );
						}
					}

					if ( g_kGlobalValue.IsHybridPlayer( g_MyD3D->GetMyPlayer()->GetCharType() ) )
					{
						if ( fAPIncrease > 0.0f )
						{
							fIncMP += ( fAPIncrease * GC_GLOBAL_CONST::fPetMPRecovery ) * fIncMP;
						}
					}
				}

				if (!bIsQuickPlayer)
				{
					fIncMP *= GC_GLOBAL_CONST::fPetMPRecovery;
					if ( g_MyD3D->Get_MyPlayer() == iPlayerIndex )
					{
						fIncMP += SiGCPetMagicStone()->GetGlyphEffectAmmount( PET_MAGIC_STONE_EFFECT_TYPE::PMSET_INCREASE_MP );
					}
				}
			}

			if ( !bIsQuickPlayer )
			{
				g_MyD3D->ChargeLocalPlayerMana( fIncMP, mpRecFinal );
			}
		}
    }

    // °ø°Ý ´çÇÑ µÚ 1ÃÊµÚºÎÅÍ HPÈ¸º¹
    m_iHPRecoveryCount = 55;

    if( bSpecialPoint && iPlayerIndex == g_MyD3D->Get_MyPlayer() ) // °ø°ÝÇÑ »ç¶÷ÀÌ ÀÚ±â ÀÚ½ÅÀÌ¸é..
    {
        if( CheckBackAttack( iPlayerIndex, bPhysicalDamage ) && fDelta >= 1.0f )
        {
            g_MyD3D->GetMyPlayer()->BackAttack();
            SiKGCAchieveManager()->OccurAction( SAchieve::BACK_ATTACK, g_MyD3D->GetMyPlayer()->GetCharType() );
        }
		else if( m_dwChampionProperty & CP_ARROW_DEFENCE)
		{
			// ¸ó½ºÅÍ Æ¯¼ö ¼Ó¼º Ã³¸®
			// ¹æÀåÀÌ°Ç ¹¹°Ç ´ÙÇÏÀÚ 
			// µ¥¹ÌÁö¸¸ Ãß°¡ÇØ ÁÖÀå 
			if( CHardDamageArrow::IsArrow( iWhat )  || CHardDamageCriticalArrow::IsArrow(iWhat) || 
                CHardDamageArrow::IsBolt( iWhat )|| CHardDamageCriticalArrow::IsBolt(iWhat)	||
                CHardDamageArrow::IsHugeArrow( iWhat ) || CHardDamageCriticalArrow::IsHugeArrow(iWhat) ||
                CHardDamageArrow::IsNovaBolt_Arrow( iWhat ) ||	CHardDamageCriticalArrow::IsNovaBolt_Arrow(iWhat)||
				iWhat == DT_NOVA_SPECIAL1_BOLT || iWhat == DT_NOVA_SPECIAL1_BOLT_LIGHTNING || 
                iWhat == DT_NOVA_SPECIAL1_BOLT_SHINING ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_DARK || iWhat == DT_NOVA_SPECIAL1_BOLT_LOVE ||
			    iWhat == DT_SKULL_ARROW || iWhat == DT_NOVA_SPECIAL1_BOLT_DEATH ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_TIME ||
				iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED ||
				iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE || 
                iWhat == DT_NOVA_SPECIAL1_BOLT_HALLOWEEN ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_ELIA ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_PEGASUS ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_COLISEU ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_C_MASTER)
			{		
				AddDamage(DT_DEFENCE);
                PlaySound("12");
			}		
		}

        if( CheckAerial() )
        {
            g_MyD3D->GetMyPlayer()->AerialAttack();
            AddParticle("Aerial_Hit01", 0, 0);
        }

        if( fAttackPower >= 0.0f && fDelta >= 1.0f )
        {
            if( (iWhat == -1 || !(g_MyD3D->m_pDamageManager->GetOriginalDamage(iWhat)->m_dwDMAttribute & DA_NOT_COMBO_COUNT )) &&  eAttackType != -1)
            {
                SetFlashEffectFrame( FLASH_EFFECT_FRAME );
                SetShakeEffectFrame( 10 );
            }
        }

        g_MyD3D->MyHead->Event_Combo(CheckAerial(), (float)fAttackPower*.03f);
        g_pGameBoard->AddAttackCount();
    }

#if defined( USE_MERRY_HP_BAR )
    // È£À§¸÷ÀÌ¸é HPÇ¥½ÃÇØÁÝ½Ã´Ù 
    ShowEscortMonsterHPBar();
#endif

    // ¹é¾îÅÃ È®ÀÎ
    if( CheckBackAttack( iPlayerIndex, bPhysicalDamage ) )
        m_bIsBackAttack = true;
    else
        m_bIsBackAttack = false;

    // ³»°¡ °ø°ÝÇÑ °ø°ÝÀÏ¶§¸¸ ´Þ¼º°úÁ¦ ÄÝ¹éÇÔ¼ö¸¦ È£ÃâÇØÁØ´Ù.
    if( iPlayerIndex == g_MyD3D->Get_MyPlayer() )
    {
        const std::map<DWORD, KGCSubject*>& mapSubject = SiKGCSubjectManager()->GetAllSubjectInstance();
        std::map<DWORD, KGCSubject*>::const_iterator mapSubjectIter = mapSubject.begin();
        for(; mapSubjectIter != mapSubject.end(); ++mapSubjectIter)
        {
            KGCSubject *pSubject = mapSubjectIter->second;
            if( NULL == pSubject )
                continue;

            pSubject->CallLuaFunc_Callback_To_HitMonDamage( iPlayerIndex, m_iMonsterIndex, iWhat );
        }
    }

    // ¹æ¿øµµ µ¥¹ÌÁö ¹Þ¾ÒÀ»¶§ ¾Ë·ÁÁà¾ßÇÒ ³»¿ëÀÌ ÀÖ´Ù.
    ActionCallBack( "SyncDamage" );

    // ÀÌ ¾Æ·¡ÂÊÀº ¹æÀå¸¸ Ã³¸®ÇÏ¸é µÈ´Ù
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        return true;

    
    if( m_fHP <= 0.0f ) // Á×¾úÀ¸¸é ¸®ÅÏ
        return true;

    if( !m_bAttackCheck )
        return false;

    if( !IsTarget() )
    {
        // Å¸°ÙÀÌ ¾ø´Â »óÅÂ¿¡¼­ °ø°Ý ¹ÞÀ¸¸é ±×³à¼®À» Å¸°ÙÀ¸·Î Àâ´Â´Ù
        SetTargetPlayerIndex( iPlayerIndex );
    }

    if( !IsInvincible() ) // ¹«Àû »óÅÂ°¡ ¾Æ´Ï°Å³ª ¹«Á¶°Ç ¸Â´Â µ¥¹ÌÁö¶ó¸é
    {
        float fRatio = 1.0f;

        // ¹é¾îÅÃ ¿©ºÎ¸¦ ÆÇ´ÜÇØ¼­ µ¥¹ÌÁö¸¦ 2.0¹è·Î ÇØ ÁØ´Ù
        if( CheckBackAttack( iPlayerIndex, bPhysicalDamage ) )
        {
            m_bIsBackAttack = true;
            fRatio *= 2.0f;         // ¹é¾îÅÃÀÇ °æ¿ì µ¥¹ÌÁö 1.5¹è!
        }
        else
        {
            m_bIsBackAttack = false;
        }

        if( ( !IsSuperArmor() && !bSkipMotion && m_fShield == 0.0f ) || bSuperDamage_ )
        {
            float fAttackPlayerX;

            if( iPlayerIndex == g_MyD3D->Get_MyPlayer() )
                fAttackPlayerX = g_kLocalPlayerLatency->kRenderData[0].vPos.x;
            else
                fAttackPlayerX = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.x;

            
			if( !kMonsterTemplate.bNoDamageReaction || eAttackType == -1 )
            {
                // ´Ù¿îµÇ´Â °ø°ÝÀÌ¸é ³ª¸¦ °ø°ÝÇÑ ÇÃ·¹ÀÌ¾î¸¦ ¹Ù¶óº»´Ù
                if( eAttackType == ATTACKTYPE_DOWN )
                {
                    if( m_afX[ LATENCY_INDEX ] > fAttackPlayerX )
                        m_abIsRight[ LATENCY_INDEX ] = false;
                    else 
                        m_abIsRight[ LATENCY_INDEX ] = true;

                    m_iSlowCount = 0;
                }
                else if( fDelta >= 1.0f )
                {
                    m_iSlowCount = 8;
                }

                // °ø°ÝÀ» ¸Â¾ÒÀ» ¶§ ³¯¾Æ°¡´Â ÈûÀ» Àû¿ë½ÃÄÑ ÁØ´Ù
                if( m_afX[ LATENCY_INDEX ] > fAttackPlayerX )
                    m_fSpeedX = fPushTargetX;
                else
                    m_fSpeedX = fPushTargetX * ( -1 );

                m_fSpeedY = fPushTargetY;

                if( m_fSpeedY > 0.0f )
                {
                    if( m_fContactHeight == 0.0f )
                        m_fContactHeight = m_afY[ LATENCY_INDEX ];
                    m_bIsContact = false;
                }
            
            }
            
            m_iPreMotion = m_aiMotion[ LATENCY_INDEX ];

            bool bAerial = false;

            // °øÁßÄÞº¸ ¿©ºÎ¸¦ ÆÇ´ÜÇØ¼­ µ¥¹ÌÁö¸¦ 2.0¹è·Î ÇØ ÁØ´Ù
            if( CheckAerial() )
            {
                bAerial = true;
                fRatio *= 1.5f;         // °øÁßÄÞº¸ °æ¿ì µ¥¹ÌÁö 1.5¹è!
            }
            
            // µ¥¹ÌÁö ÄÝ¹é
            // È­»ì ¹æ¾î ¸ó½ºÅÍ°¡ È­»ì ¸ÂÀ¸¸é µ¥¹ÌÁö 0.2¹è -¤µ-
            if( bArrowDamage && kMonsterTemplate.fArrowDefence != 0.f && !m_bIsBackAttack && !bAerial )
            {
                
                fRatio *= kMonsterTemplate.fArrowDefence;
                ActionCallBack( "DefenceArrow" );
                
            }
            else if(false == m_bIsBackAttack && m_dwChampionProperty & CP_ARROW_DEFENCE)
			{
				if( CHardDamageArrow::IsArrow( iWhat )  || 
					CHardDamageCriticalArrow::IsArrow(iWhat))	
				{					
					fRatio*=0.15f;					
				}
				else if( CHardDamageArrow::IsBolt( iWhat )||
					CHardDamageCriticalArrow::IsBolt(iWhat))			
				{
					fRatio*=0.2f;				
				}
				else if( CHardDamageArrow::IsHugeArrow( iWhat ) ||
					CHardDamageCriticalArrow::IsHugeArrow(iWhat))			
				{
					fRatio*= 0.2f;				
				}
				else if( CHardDamageArrow::IsNovaBolt_Arrow( iWhat ) ||	
					CHardDamageCriticalArrow::IsNovaBolt_Arrow(iWhat)||
					iWhat == DT_NOVA_SPECIAL1_BOLT ||
					iWhat == DT_NOVA_SPECIAL1_BOLT_LIGHTNING ||
					iWhat == DT_NOVA_SPECIAL1_BOLT_SHINING ||
					iWhat == DT_NOVA_SPECIAL1_BOLT_DARK ||
                    iWhat == DT_NOVA_SPECIAL1_BOLT_DEATH ||
					iWhat == DT_NOVA_SPECIAL1_BOLT_LOVE ||
                    iWhat == DT_NOVA_SPECIAL1_BOLT_TIME ||
					iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED ||
					iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE ||
                    iWhat == DT_NOVA_SPECIAL1_BOLT_HALLOWEEN ||
                    iWhat == DT_NOVA_SPECIAL1_BOLT_ELIA ||
                    iWhat == DT_NOVA_SPECIAL1_BOLT_PEGASUS ||
                    iWhat == DT_NOVA_SPECIAL1_BOLT_COLISEU ||
                    iWhat == DT_NOVA_SPECIAL1_BOLT_C_MASTER )

				{
					fRatio*=0.15f;				
				}
				else if( eAttackType == DT_SKULL_ARROW )
				{
					fRatio*= 0.0f;					
				}
				else
				{
                    if( false == bNoReAction )
                    {
					    // °ø°Ý Å¸ÀÔ¿¡ µû¶ó¼­.. µî·ÏµÇ¾î ÀÖ´Â ¸®¾×¼ÇÀ» ÃëÇÏµµ·Ï ÇÑ´Ù!
					    std::pair<EGCAttackType, EGCAttackDirection> key = std::make_pair( eAttackType, eAttackDirection );
					    std::map< std::pair<EGCAttackType, EGCAttackDirection>, std::string >::iterator mit = m_mapReaction.find( key );

					    if( mit != m_mapReaction.end() )
					    {
					    	std::string strReactionState = mit->second;
					    	//SetState( strReactionState.c_str() );
					    	DirectMotionInput( strReactionState.c_str() );
					    }
                    }
				}
			}
			else
			{
                if( false == bNoReAction )
                {
				    // °ø°Ý Å¸ÀÔ¿¡ µû¶ó¼­.. µî·ÏµÇ¾î ÀÖ´Â ¸®¾×¼ÇÀ» ÃëÇÏµµ·Ï ÇÑ´Ù!
				    std::pair<EGCAttackType, EGCAttackDirection> key = std::make_pair( eAttackType, eAttackDirection );
				    std::map< std::pair<EGCAttackType, EGCAttackDirection>, std::string >::iterator mit = m_mapReaction.find( key );

				    if( mit != m_mapReaction.end() )
				    {
				    	std::string strReactionState = mit->second;
				    	//SetState( strReactionState.c_str() );
				    	DirectMotionInput( strReactionState.c_str() );
				    }
                }
			}        

            // µ¥¹ÌÁö ÄÝ¹é
            ActionCallBack( "Damage" );
        }
		else if( m_fShield )
		{
			ActionCallBack( "ShieldDamage" );
        }


        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
        {
            float fLevelAdvantageRatio = 1.0f;
            // ¶§¸° »ç¶÷ÀÇ ·¹º§
            int iHitPlayerLevel = m_iLevel; 
            // ¸ó½ºÅÍ°¡ ¸ó½ºÅÍ¸¦ ¶§¸®´Â °æ¿ìµµ ÀÖ¾î¾ß ÇÑ´Ù -¤µ-; 
            if ( iPlayerIndex != MAX_PLAYER_NUM )
                iHitPlayerLevel = g_MyD3D->MyPlayer[iPlayerIndex]->GetCurrentChar().iLevel;


            int iDiffLevel = iHitPlayerLevel - m_iLevel;
            // ·¹º§ 6ÀÌ»ó Â÷ÀÌ°¡ ³ª¸é ·¹º§¸¶´Ù xÆÛ¼¾Æ®¾¿ °ø°Ý·Â »ó½Â Àû¿ë..
            const float fDamageAdvantage = 0.02f;
            if( iDiffLevel > 5 )
                fLevelAdvantageRatio *= ( (float)( iDiffLevel - 5 ) * fDamageAdvantage + 1.0f );

            if( fLevelAdvantageRatio > MAX_ATTACK_ADVANTAGE_RATIO ) // Max°ªÀº ÀÖ´Ù´Â°Å.. (ÇöÀç3¹è)
                fLevelAdvantageRatio = MAX_ATTACK_ADVANTAGE_RATIO;

            fRatio *= fLevelAdvantageRatio;
        }


        // ¸ó½ºÅÍ m_fHP °¨¼Ò
        float fDamage = ChangeHP( NULL, fAttackPower * fRatio * fDelta , iPlayerIndex, bSpecialAttack, bPetDamage, nSpecialGrade, iMonsterIndex_, bIsDarkSpearDamage, bIsNoCritDmg );
        if( NULL != pDamage )
            *pDamage = fDamage;

        // 2008.01.15 wony
        // ¾Æ·¡ ÄÚµå´Â ÆÄÅõ¼¼ÀÌÀÇ ¸öÅë°ú ´Ù¸®°¡ »ì¾ÆÀÖÀ¸¸é ¸Ó¸®°¡ Á×Áö ¾Êµµ·Ï Ã³¸®ÇÏ´Â ºÎºÐÀÌ´Ù..
        // ¸Ó¸®¸¦ ¸¶Áö¸·¿¡ ±ú°íÀÚ ÇÏ´Â ÀÇµµ ¶§¹®¿¡..-¤µ -;;

        // ÆÄÅõ¼¼ÀÌ ¸Ó¸®´Â ÆÄÅõ¼¼ÀÌ ¸öÅë°ú ´Ù¸®°¡ »ì¾ÆÀÖÀ¸¸é Á×Áö ¾Ê¾Æ!!!
        if( ( GetDefaultMonster( m_iMonsterType ) == MON_PATUSEI_HEAD ) && m_fHP <= 0.0f )
        {
            bool bNeverDie = false;
			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

				// ¾ÆÁ÷ ´Ù¸®³ª ¸öÅëÀÌ »ì¾Æ ÀÖ´Ù¸é
				if( ( GetDefaultMonster( pMonster->m_iMonsterType ) == MON_PATUSEI_BODY ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_PATUSEI_LEG ) &&
					pMonster->m_fHP > 0.0f )
				{
					bNeverDie = true;
					break;
				}
			}

            // Ã¼·ÂÀ» ´«²Å¸¸Å­ Ã¤¿ö¼­ Á×Áö ¾Ê°Ô Ã³¸®ÇÑ´Ù..-¤µ - ¾îÂ¿¼ö ¾øÀ½..
            if( bNeverDie )
                m_fHP = 0.1f;
        }

		// ½Å¸ñÀº °¡ÁöµéÀÌ ÇÏ³ª Á×À»¶§ ¸¶´Ù 25% ¸¸Å­ ±ïÀ» ¼ö ÀÖ´Ù!
		if( GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_TREE ||
            GetDefaultMonster( m_iMonsterType ) == MON_ANGRYBOSS_DIVINE_TREE ||
            GetDefaultMonster( m_iMonsterType ) == MON_SUPERMON_DIVINE_TREE )
		{
			int iLeftBranchCount = 0;
			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

				if( ( GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH1 ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH2 ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH3 ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH4 )
					&& pMonster->m_fHP > 0.0f )
				{
					iLeftBranchCount += 1;
				}
			}


			if ( iLeftBranchCount != 0 && m_fHP < ( m_fHPPoint / 4 ) * iLeftBranchCount )
			{
				m_fHP = ( m_fHPPoint / 4 ) * iLeftBranchCount;
			}
		}

        if( m_iMonsterType == MON_NEXTSTAGE_OBJECT && m_fHP <= 1.0f )
        {
            bool bNeverDie = false;
			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

				// ´Ù¸¥ ¹®ÀÌ Á×¾ú´Ù¸é! ³­ Á×Áö ¾Ê¾Æ!
				if( pMonster->m_iMonsterType == MON_NEXTSTAGE_OBJECT && pMonster->m_fHP <= 1.0f )
				{
					bNeverDie = true;
					break;
				}
			}


            // Ã¼·ÂÀ» ´«²Å¸¸Å­ Ã¤¿ö¼­ Á×Áö ¾Ê°Ô Ã³¸®ÇÑ´Ù..-¤µ - ¾îÂ¿¼ö ¾øÀ½..
            if( bNeverDie )
                m_fHP = 1.5f;
        }
    }    
    return true;
}

bool MONSTER::OnMonsterDamage( int iMonsterIndex, int iWhat,
                       float fAttackPower,
                       EGCAttackType eAttackType, EGCAttackDirection eAttackDirection,
                       float fPushTargetX, float fPushTargetY, bool bSpecialAttack, bool bSpecialPoint /*= true*/,bool bArrowDamage )
{
    // µ¥¹ÌÁö´Â µé¾î¿ÀÁö¸¸ ¸ð¼ÇÀº ¹Ù²Ù±â ½È´Ù.. ½´ÆÛ¾Æ¸Ó¶û ¶È°°ÀÝ¾Æ Á¨Àå ¤Ð
    bool bSkipMotion = false;

    bool bPhysicalDamage = false;

    if( iWhat == -1 )
        bPhysicalDamage = true;

    g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);
    SetAttackedMonster(true);
    SetDamageWhat( iWhat );

    //µ¥¹ÌÁö ¸Â¾ÒÀ» ¶§, ¹º°¡ ÇØ¾ßÇÏ¸é.. ¿©±â´Ù ÇÏÀð ¤Ð ´õ·¯¿î ÄÚµåµéÀÇ ÁýÇÕÃ¼ ¤»
    if ( false == OnMonsterDamageProcess(iMonsterIndex, iWhat, eAttackType, bSpecialAttack, bArrowDamage, fAttackPower, bSkipMotion) )
        return false;

    float fDelta = m_vecAttackRect[m_iLastAttackedBoxIndex].fDelta;	

    // °ø°Ý ´çÇÑ µÚ 1ÃÊµÚºÎÅÍ HPÈ¸º¹
    m_iHPRecoveryCount = 55;

    if( bSpecialPoint )
    {
        if( m_dwChampionProperty & CP_ARROW_DEFENCE)
        {
            // ¸ó½ºÅÍ Æ¯¼ö ¼Ó¼º Ã³¸®
            // ¹æÀåÀÌ°Ç ¹¹°Ç ´ÙÇÏÀÚ 
            // µ¥¹ÌÁö¸¸ Ãß°¡ÇØ ÁÖÀå 
            if( CHardDamageArrow::IsArrow( iWhat )  || CHardDamageCriticalArrow::IsArrow(iWhat) || 
                CHardDamageArrow::IsBolt( iWhat )|| CHardDamageCriticalArrow::IsBolt(iWhat)	||
                CHardDamageArrow::IsHugeArrow( iWhat ) || CHardDamageCriticalArrow::IsHugeArrow(iWhat) ||
                CHardDamageArrow::IsNovaBolt_Arrow( iWhat ) ||	CHardDamageCriticalArrow::IsNovaBolt_Arrow(iWhat)||
                iWhat == DT_NOVA_SPECIAL1_BOLT || iWhat == DT_NOVA_SPECIAL1_BOLT_LIGHTNING ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_SHINING || iWhat == DT_NOVA_SPECIAL1_BOLT_DARK ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_LOVE ||
                iWhat == DT_SKULL_ARROW || iWhat == DT_NOVA_SPECIAL1_BOLT_DEATH ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_TIME ||
				iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED ||
				iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE || 
                iWhat == DT_NOVA_SPECIAL1_BOLT_HALLOWEEN ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_ELIA ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_PEGASUS ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_COLISEU ||
                iWhat == DT_NOVA_SPECIAL1_BOLT_C_MASTER)
                
            {		
                AddDamage(DT_DEFENCE);
                PlaySound("12");
            }		
        }

        if( fAttackPower >= 0.0f && fDelta >= 1.0f )
        {
            if( iWhat == -1 || !(g_MyD3D->m_pDamageManager->GetOriginalDamage(iWhat)->m_dwDMAttribute & DA_NOT_COMBO_COUNT ) )
            {
                SetFlashEffectFrame( FLASH_EFFECT_FRAME );
                SetShakeEffectFrame( 10 );
            }
        }
    }


    // ÀÌ ¾Æ·¡ÂÊÀº ¹æÀå¸¸ Ã³¸®ÇÏ¸é µÈ´Ù
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        return true;


    if( m_fHP <= 0.0f ) // Á×¾úÀ¸¸é ¸®ÅÏ
        return true;

    if( !m_bAttackCheck )
        return false;

    if( !IsTarget() )
    {
        // Å¸°ÙÀÌ ¾ø´Â »óÅÂ¿¡¼­ °ø°Ý ¹ÞÀ¸¸é ±×³à¼®À» Å¸°ÙÀ¸·Î Àâ´Â´Ù
        m_iTargetMonsterIndex = iMonsterIndex;
    }

    if( !IsInvincible() ) // ¹«Àû »óÅÂ°¡ ¾Æ´Ï¸é..
    {
        const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
        float fRatio = 1.0f;

        // ¹é¾îÅÃ ¿©ºÎ¸¦ ÆÇ´ÜÇØ¼­ µ¥¹ÌÁö¸¦ 2.0¹è·Î ÇØ ÁØ´Ù
        bool m_bIsBackAttack;
        if( CheckMonsterBackAttack( iMonsterIndex, bPhysicalDamage ) )
        {
            m_bIsBackAttack = true;
            fRatio *= 1.5f;         // ¹é¾îÅÃÀÇ °æ¿ì µ¥¹ÌÁö 1.5¹è!
        }
        else
        {
            m_bIsBackAttack = false;
        }

        if( !IsSuperArmor() && !bSkipMotion && m_fShield == 0.0f )
        {
			MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
			if( pMonster != NULL){
				float fAttackPlayerX = pMonster->m_afX[ LATENCY_INDEX ];

				if( !kMonsterTemplate.bNoDamageReaction )
				{
					// ´Ù¿îµÇ´Â °ø°ÝÀÌ¸é ³ª¸¦ °ø°ÝÇÑ ÇÃ·¹ÀÌ¾î¸¦ ¹Ù¶óº»´Ù
					if( eAttackType == ATTACKTYPE_DOWN )
					{
						if( m_afX[ LATENCY_INDEX ] > fAttackPlayerX )
							m_abIsRight[ LATENCY_INDEX ] = false;
						else 
							m_abIsRight[ LATENCY_INDEX ] = true;

						m_iSlowCount = 0;
					}
					else if( fDelta >= 1.0f )
					{
						m_iSlowCount = 8;
					}

					// °ø°ÝÀ» ¸Â¾ÒÀ» ¶§ ³¯¾Æ°¡´Â ÈûÀ» Àû¿ë½ÃÄÑ ÁØ´Ù
					if( m_afX[ LATENCY_INDEX ] > fAttackPlayerX )
						m_fSpeedX = fPushTargetX;
					else
						m_fSpeedX = fPushTargetX * ( -1 );

					m_fSpeedY = fPushTargetY;

					if( m_fSpeedY > 0.0f )
					{
						if( m_fContactHeight == 0.0f )
							m_fContactHeight = m_afY[ LATENCY_INDEX ];
						m_bIsContact = false;
					}

				}

				m_iPreMotion = m_aiMotion[ LATENCY_INDEX ];

				// µ¥¹ÌÁö ÄÝ¹é
				// È­»ì ¹æ¾î ¸ó½ºÅÍ°¡ È­»ì ¸ÂÀ¸¸é µ¥¹ÌÁö 0.2¹è -¤µ-
				if( bArrowDamage && kMonsterTemplate.fArrowDefence != 0.f && !m_bIsBackAttack )
				{
					fRatio *= kMonsterTemplate.fArrowDefence;
					ActionCallBack( "DefenceArrow" );

				}
				else if(false == m_bIsBackAttack && m_dwChampionProperty & CP_ARROW_DEFENCE)
				{
					if( CHardDamageArrow::IsArrow( iWhat )  || 
						CHardDamageCriticalArrow::IsArrow(iWhat))	
					{					
						fRatio*=0.15f;					
					}
					else if( CHardDamageArrow::IsBolt( iWhat )||
						CHardDamageCriticalArrow::IsBolt(iWhat))			
					{
						fRatio*=0.2f;				
					}
					else if( CHardDamageArrow::IsHugeArrow( iWhat ) ||
						CHardDamageCriticalArrow::IsHugeArrow(iWhat))			
					{
						fRatio*= 0.2f;				
					}
					else if( CHardDamageArrow::IsNovaBolt_Arrow( iWhat ) ||	
						CHardDamageCriticalArrow::IsNovaBolt_Arrow(iWhat)||
						iWhat == DT_NOVA_SPECIAL1_BOLT ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_LIGHTNING ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_SHINING ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_DARK ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_DEATH ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_LOVE ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_TIME ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_RED ||
						iWhat == DT_NOVA_SPECIAL1_BOLT_DARK_DRAGON_BLUE ||
                        iWhat == DT_NOVA_SPECIAL1_BOLT_HALLOWEEN ||
                        iWhat == DT_NOVA_SPECIAL1_BOLT_ELIA ||
                        iWhat == DT_NOVA_SPECIAL1_BOLT_PEGASUS ||
                        iWhat == DT_NOVA_SPECIAL1_BOLT_COLISEU ||
                        iWhat == DT_NOVA_SPECIAL1_BOLT_C_MASTER)
					{
						fRatio*=0.15f;				
					}
					else if( eAttackType == DT_SKULL_ARROW )
					{
						fRatio*= 0.0f;					
					}
					else
					{
						// °ø°Ý Å¸ÀÔ¿¡ µû¶ó¼­.. µî·ÏµÇ¾î ÀÖ´Â ¸®¾×¼ÇÀ» ÃëÇÏµµ·Ï ÇÑ´Ù!
						std::pair<EGCAttackType, EGCAttackDirection> key = std::make_pair( eAttackType, eAttackDirection );
						std::map< std::pair<EGCAttackType, EGCAttackDirection>, std::string >::iterator mit = m_mapReaction.find( key );

						if( mit != m_mapReaction.end() )
						{
							std::string strReactionState = mit->second;
							//SetState( strReactionState.c_str() );
							DirectMotionInput( strReactionState.c_str() );
						}
					}
				}
				else
				{
					// °ø°Ý Å¸ÀÔ¿¡ µû¶ó¼­.. µî·ÏµÇ¾î ÀÖ´Â ¸®¾×¼ÇÀ» ÃëÇÏµµ·Ï ÇÑ´Ù!
					std::pair<EGCAttackType, EGCAttackDirection> key = std::make_pair( eAttackType, eAttackDirection );
					std::map< std::pair<EGCAttackType, EGCAttackDirection>, std::string >::iterator mit = m_mapReaction.find( key );

					if( mit != m_mapReaction.end() )
					{
						std::string strReactionState = mit->second;
						//SetState( strReactionState.c_str() );
						DirectMotionInput( strReactionState.c_str() );
					}
				}        

				// µ¥¹ÌÁö ÄÝ¹é
				ActionCallBack( "Damage" );

			}
        }
        else if( m_fShield )
        {
            ActionCallBack( "ShieldDamage" );
        }

        // ¿ä°Å ´øÀü¿¡¼­¸¸ Àû¿ëÀÓ
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON )
        {
            float fLevelAdvantageRatio = 1.0f;
            // ¶§¸° »ç¶÷ÀÇ ·¹º§
            int iHitPlayerLevel = m_iLevel; 
            // ¸ó½ºÅÍ°¡ ¸ó½ºÅÍ¸¦ ¶§¸®´Â °æ¿ìµµ ÀÖ¾î¾ß ÇÑ´Ù -¤µ-; 
			MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
			if( pMonster != NULL){

				iHitPlayerLevel = pMonster->m_iLevel;
			}

            int iDiffLevel = iHitPlayerLevel - m_iLevel;
            // ·¹º§ 6ÀÌ»ó Â÷ÀÌ°¡ ³ª¸é ·¹º§¸¶´Ù xÆÛ¼¾Æ®¾¿ °ø°Ý·Â »ó½Â Àû¿ë..
            const float fDamageAdvantage = 0.02f;
            if( iDiffLevel > 5 )
                fLevelAdvantageRatio *= ( (float)( iDiffLevel - 5 ) * fDamageAdvantage + 1.0f );

            if( fLevelAdvantageRatio > MAX_ATTACK_ADVANTAGE_RATIO ) // Max°ªÀº ÀÖ´Ù´Â°Å.. (ÇöÀç3¹è)
                fLevelAdvantageRatio = MAX_ATTACK_ADVANTAGE_RATIO;

            fRatio *= fLevelAdvantageRatio;
        }

        // ¸ó½ºÅÍ m_fHP °¨¼Ò
        ChangeHPToMonster( NULL, fAttackPower * fRatio * fDelta , iMonsterIndex, bSpecialAttack );

        // 2008.01.15 wony
        // ¾Æ·¡ ÄÚµå´Â ÆÄÅõ¼¼ÀÌÀÇ ¸öÅë°ú ´Ù¸®°¡ »ì¾ÆÀÖÀ¸¸é ¸Ó¸®°¡ Á×Áö ¾Êµµ·Ï Ã³¸®ÇÏ´Â ºÎºÐÀÌ´Ù..
        // ¸Ó¸®¸¦ ¸¶Áö¸·¿¡ ±ú°íÀÚ ÇÏ´Â ÀÇµµ ¶§¹®¿¡..-¤µ -;;

        // ÆÄÅõ¼¼ÀÌ ¸Ó¸®´Â ÆÄÅõ¼¼ÀÌ ¸öÅë°ú ´Ù¸®°¡ »ì¾ÆÀÖÀ¸¸é Á×Áö ¾Ê¾Æ!!!
        if( ( GetDefaultMonster( m_iMonsterType ) == MON_PATUSEI_HEAD ) && m_fHP <= 0.0f )
        {
            bool bNeverDie = false;

			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

				// ¾ÆÁ÷ ´Ù¸®³ª ¸öÅëÀÌ »ì¾Æ ÀÖ´Ù¸é
				if( ( GetDefaultMonster( pMonster->m_iMonsterType ) == MON_PATUSEI_BODY ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_PATUSEI_LEG ) &&
					pMonster->m_fHP > 0.0f )
				{
					bNeverDie = true;
					break;
				}
			}
 
            // Ã¼·ÂÀ» ´«²Å¸¸Å­ Ã¤¿ö¼­ Á×Áö ¾Ê°Ô Ã³¸®ÇÑ´Ù..-¤µ - ¾îÂ¿¼ö ¾øÀ½..
            if( bNeverDie )
                m_fHP = 0.1f;
        }

        // ½Å¸ñÀº °¡ÁöµéÀÌ ÇÏ³ª Á×À»¶§ ¸¶´Ù 25% ¸¸Å­ ±ïÀ» ¼ö ÀÖ´Ù!
        if( GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_TREE ||
            GetDefaultMonster( m_iMonsterType ) == MON_ANGRYBOSS_DIVINE_TREE ||
            GetDefaultMonster( m_iMonsterType ) == MON_SUPERMON_DIVINE_TREE )
        {
            int iLeftBranchCount = 0;

			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

				if( ( GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH1 ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH2 ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH3 ||
					GetDefaultMonster( pMonster->m_iMonsterType ) == MON_DIVINE_BRANCH4 )
					&& pMonster->m_fHP > 0.0f )
				{
					iLeftBranchCount += 1;
				}
			}


            if ( iLeftBranchCount != 0 && m_fHP < ( m_fHPPoint / 4 ) * iLeftBranchCount )
            {
                m_fHP = ( m_fHPPoint / 4 ) * iLeftBranchCount;
            }
        }

        if( m_iMonsterType == MON_NEXTSTAGE_OBJECT && m_fHP <= 1.0f )
        {
            bool bNeverDie = false;
			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

				// ´Ù¸¥ ¹®ÀÌ Á×¾ú´Ù¸é! ³­ Á×Áö ¾Ê¾Æ!
				if( pMonster->m_iMonsterType == MON_NEXTSTAGE_OBJECT && pMonster->m_fHP <= 1.0f )
				{
					bNeverDie = true;
					break;
				}			
			}

            // Ã¼·ÂÀ» ´«²Å¸¸Å­ Ã¤¿ö¼­ Á×Áö ¾Ê°Ô Ã³¸®ÇÑ´Ù..-¤µ - ¾îÂ¿¼ö ¾øÀ½..
            if( bNeverDie )
                m_fHP = 1.5f;
        }
    }

    return true;
}

void MONSTER::OnLand()
{
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        return;

    ActionCallBack( "Land" );
}

void MONSTER::SendDropInfo( void )
{
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        return;

    //¾ÆÀÌÅÛÀ» µå¶øÇØ¾ßÇÏ´Â ¸ó½ºÅÍ·Î µî·ÏµÇ¾î ÀÖÀ¸¸é ¾ÆÀÌÅÛµå¶ø¿¡°Ô Á¤º¸¸¦ º¸³À´Ï´Ù.
    if( m_pDropItemObserver != NULL )
    {
        float fOffsetX = 0.0f;
        float fOffsetY = 0.0f;
        switch( GetDefaultMonster( m_iMonsterType ) )
        {
        case MON_GIANT:
            fOffsetY = 5.3f;
            break;
        case MON_DIVINE_BRANCH1:
        case MON_DIVINE_BRANCH2:
            fOffsetX = 0.5f;
            break;
        case MON_DIVINE_BRANCH3:
        case MON_DIVINE_BRANCH4:
            fOffsetX = -0.5f;
            break;
        case MON_DIVINE_TREE:
        case MON_ANGRYBOSS_DIVINE_TREE:
        case MON_SUPERMON_DIVINE_TREE:
            fOffsetY = 2.0f;
            break;
        case MON_HELL_DEATH_WORM:
            fOffsetX = -1.35f;
            fOffsetY = 0.25f;
        }
        (*m_pDropItemObserver)( m_iTriggerID, D3DXVECTOR2( GetX() + fOffsetX, GetY() + fOffsetY ));
        m_pDropItemObserver = NULL;
    }
}

void MONSTER::OnDie()
{
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        return;

    ActionCallBack( "Die" );
    KGCPC_MEMO( "Monster Die" );
    //m_bCallDieFucn = true;
    m_bCallDieFucn = true;
    
    SendDropInfo();

    //Result_MonsterDefeat_Battlepass = INT_MAX;
    //SiKP2P()->Send_MonsterDefeat_Battlepass( g_kGlobalValue.m_kUserInfo.dwUID, m_iMonsterType );
}

void MONSTER::ActionCallBack( std::string strAction )
{
    
    //char strFunc[MAX_PATH];
    //sprintf( strFunc, "%s(%d)", g_kMonsterManager.GetMonsterCallbackToStringKey( strAction ).c_str(), m_iMonsterIndex );
    //KGCLuabinder::getInstance().DoString( strFunc );

    bool bUseAIScript =false;
    SDotaTemplate* pCurDotaInfo = g_kGlobalValue.GetCurrentDotaTemplate();
    if( pCurDotaInfo &&
        false == pCurDotaInfo->bUseAIFromScript &&
        GC_GMC_DOTA == SiKGCRoomManager()->GetGameModeCategory() ){
        switch( m_iMonsterType )
        {
        case MON_DOTA_CORE:
            if( strstr(strAction.c_str(), "Die" ) )
                m_HardAI_Core.CallBack_Die( m_iMonsterIndex );
            else if( strstr(strAction.c_str(), "Damage" ) )
                m_HardAI_Core.CallBack_Damage( m_iMonsterIndex );
            break;
        case MON_DOTA_ANCIENT_SERDIN:
        case MON_DOTA_ANCIENT_CANABAN:
            if( strstr(strAction.c_str(), "Land" ) )
                m_HardAI_Ancient.CallBack_Land( m_iMonsterIndex );
            else if( strstr(strAction.c_str(), "Die" ) )
                m_HardAI_Ancient.CallBack_Die( m_iMonsterIndex );
            break;
        case MON_DOTA_GUARDIAN_TOWER:
            if( strstr(strAction.c_str(), "Die" ) )
                m_HardAI_GuardianTower.CallBack_Die( m_iMonsterIndex );
            else if( strstr(strAction.c_str(), "Damage" ) )
                m_HardAI_GuardianTower.CallBack_Damage( m_iMonsterIndex );
            break;
        case MON_DOTA_GUARD_TOWER:
            if( strstr(strAction.c_str(), "Die" ) )
                m_HardAI_GuardTower.CallBack_Die( m_iMonsterIndex);
            else if( strstr(strAction.c_str(), "Damage" ) )
                m_HardAI_GuardTower.CallBack_Damage( m_iMonsterIndex);
            break;
        default:
            bUseAIScript = true;
        }
    }
    else
    {
        bUseAIScript = true;
    }

    if( bUseAIScript )
    {
        lua_State* L = KGCLuabinder::getInstance().GetLuaState();
        CHECK_STACK(L)

            lua_checkstack( L, 10 );					// ½ºÅÃ »çÀÌÁî¸¦ Ã¼Å© ÇÕ´Ï´Ù. ( 20º¸´Ù ÀÛÀ¸¸é ¿©À¯°ø°£À» È®º¸ ÇÕ´Ï´Ù. )
        lua_pushstring( L, g_kMonsterManager.GetMonsterCallbackToStringKey( m_iMonsterType, strAction ).c_str() );		// Player_Action Å×ÀÌºíÀ» ºÒ·¯¿À±â À§ÇØ ÀÌ¸§À» Push ÇÕ´Ï´Ù.
        lua_rawget( L, LUA_GLOBALSINDEX );			// ±Û·Î¹ú Å×ÀÌºí¿¡¼­ Player_Action Å°¸¦ Ã£½À´Ï´Ù.
        if( lua_isfunction( L, -1 ) )
        {
            lua_pushinteger( L, m_iMonsterIndex );
            lua_pcall( L, 1, 0, 0 );
        }
        else
        {
            lua_pop( L, 1 );
        }
    }
}

bool MONSTER::CheckState( char* strState )
{
    std::string strCurrentState = GetState();
    return ( strCurrentState.compare( strState ) == 0 );
}

void MONSTER::StartText( char* strText )
{
    g_MyD3D->MyHead->Start_Text( strText );
}

void MONSTER::ReadyToRender()
{
    if( GC_GM_DOTA != SiKGCRoomManager()->GetGameMode() ) {
        TraceIconProcess();

		//º¸½º ¸ó½ºÅÍ ¸Ó¸® À§¿¡ ¾ÆÀÌÄÜ ¶Ù¿ì±â À§ÇÑ ¸ð¸® Æ÷Áö¼Ç ¾÷µ¥ÀÌÆ®
		UpdateHeadPos();
    }

    ShieldProcess();
    int iIndex = 0;
    //int iIndex = LATENCY_INDEX;
    //if( !g_kGlobalValue.m_kUserInfo.bHost )
    //    iIndex = 0;

    GCObject* pkModel = NULL;

    if( m_bLive )
    {
        pkModel = m_pObject;

        if (m_pkMonsterModelDead)
        {
            m_pkMonsterModelDead->SetRender( false );
        }
        // ¸ó½ºÅÍ°¡ »ì¾ÆÀÖÀ»¶§¸¸ ¤Ñ ·»´õ¸¦ ²¯´Ù°¡ Ä×´Ù°¡ ÇØÁÖÀÚ¤Ñ Á×Àº ¾ÆÀÌ´Â ¿Ø¸¸ÇØ¼­´Â ¾È²ô´Â°Í °°À½ ¤Ñ

		if( pkModel )
        {
            if( m_bMeshToggleState )
                pkModel->SetRender( false );
            else
    			pkModel->SetRender( g_kGlobalValue.m_bRenderMonster && m_bRender );
        }
    }
    else
    {
        pkModel = m_pkMonsterModelDead;
        
        if (m_pObject)
        {
            EGCMonster eMonsterType = GetDefaultMonster( m_iMonsterType );
            if( eMonsterType != MON_PATUSEI_HEAD )
                m_pObject->SetRender( false );
        }
    }

    std::vector<CParticleEventSeqPTR>::iterator vecIter = m_vecChildMeshParticle.begin();
    for(; vecIter != m_vecChildMeshParticle.end(); )
    {
        if( false == g_ParticleManager->IsLiveInstance( *vecIter ) )
        {
            vecIter = m_vecChildMeshParticle.erase( vecIter );
            continue;
        }

        (*vecIter)->SetShow( g_kGlobalValue.m_bRenderMonster && m_bRender );
        ++vecIter;
    }

    if ( pkModel == NULL )
        return;

    D3DXMATRIX matScale, matRot, matTrans, matWorld;

	if( m_fVolume >= 10000.0f / 400.0f )
		D3DXMatrixScaling( &matScale, m_fScale, m_fScale, 0.1f * m_fScale );
	else
		D3DXMatrixScaling( &matScale, m_fScale, m_fScale, m_fScale );

    if( m_abIsRight[iIndex] )
    {
        D3DXMatrixRotationY( &matRot, 0.0f );
    }
    else
    {
        D3DXMatrixRotationY( &matRot, 3.1415f );
    }

    float fShakeX = 0.0f;
    if( m_iShakeEffectFrame > 0 )
    {
        fShakeX = (float)( rand() % 2 ? 0.02f : -0.02f );
        --m_iShakeEffectFrame;
    }

    if( m_bNonShakeEffect )
    {
        fShakeX = 0.0f;
    }

	if( (int)m_vecMotionList.size() <= m_aiMotion[iIndex] || m_aiMotion[iIndex] < 0 )
		return;

    if( GetLastFrame_( iIndex ) < m_aiFrame[ iIndex ] )
    {
    }
    else
    {
		if ( MON_GIANT == GetDefaultMonster( m_iMonsterType ) && !m_bLive )
		{
			D3DXMatrixTranslation( &matTrans,
				m_afX[iIndex] - 1.0f + fShakeX,
				m_afY[iIndex] - 0.5f ,
				m_fZPos );
		}
		else
		{
			D3DXMatrixTranslation( &matTrans,
								m_afX[iIndex] - 1.0f + fShakeX,
								m_afY[iIndex] + m_vecMotionList[ m_aiMotion[iIndex] ]->GetFrameData( m_aiFrame[iIndex] ).Pos_y * m_fScale - 0.5f ,
								m_fZPos );
		}

		matWorld = matScale * matRot * matTrans;

		pkModel->SetWorldMat(&matWorld);
		pkModel->SetFrame(&m_vecMotionList[m_aiMotion[iIndex]]->GetFrameData(m_aiFrame[iIndex]));
    }

    if( m_iFlashEffectFrame > 0 )
    {
        if ( g_MyD3D->m_KGCOption.IsCartoon() )
        {
            if( m_abIsRight[iIndex] )
                pkModel->SetCartoon( CARTOON_DAMAGE_REVERSE );
            else
                pkModel->SetCartoon( CARTOON_DAMAGE );
        }
        --m_iFlashEffectFrame;
    }
    else
    {
        // ½´ÆÛ¾Æ¸Ó~
        if( IsSuperArmor() && IsAttackSuperArmor() )
        {
            pkModel->SetCartoon( CARTTON_MONSTER_SUPER );
            pkModel->SetOutlineColor( EDGE_COLOR_SUPERARMOR );
        }
        else if( IsSuperArmor() && !IsAttackSuperArmor() ) { 
            pkModel->SetCartoon( CARTOON_ATHENS );
            pkModel->SetOutlineColor( EDGE_COLOR_SUPERARMOR );
        }
        else
        {
            pkModel->SetCartoon( CARTOON_DEFAULT );
            pkModel->SetOutlineColor( EDGE_COLOR_FRIEND );
        }

        pkModel->SetTexture( m_iSelectTextrue );
    }

    // À½... Ä«Å÷ ÅØ½ºÃÄ¸¦ °­Á¦·Î ¹Ù²Û´Ù¸é..
    if( -1 != m_iUserCartoon )
    {
        pkModel->SetCartoon( static_cast<EGCCartoonTexture>(m_iUserCartoon) );
    }
    if( D3DXVECTOR4(-1.0f, -1.0f, -1.0f, -1.0f) != m_vUserCartonEdgeColor )
    {
        pkModel->SetOutlineColor( m_vUserCartonEdgeColor );
    }

	bIsRight = m_abIsRight[iIndex];
    vRenderPos.x = m_afX[iIndex] - vParticleToDamageMagicNumber.x;
    vRenderPos.y = m_afY[iIndex] - vParticleToDamageMagicNumber.y;
    vRenderPos.z = 0.5f;

    
    pkModel->AdjustParticlePos();
    pkModel->ChildFrameMove();

    for( std::vector<KMonsterSwoosh>::iterator it = m_vecSwoosh.begin(); it != m_vecSwoosh.end(); it++ )
    {
        if ( pkModel->GetNumAngleBone() <= (DWORD)(*it).m_iBaseBone )
        {
            continue;
        }

        if ( pkModel->GetNumAngleBone() <= (DWORD)(*it).m_iEndBone )
        {
            continue;
        }


        D3DXVECTOR3 vecBase, vecEnd;
        D3DXMATRIX matBone;

        pkModel->GetAngleBone( (*it).m_iBaseBone, &matBone );
        matBone = matBone * matWorld;
        SETVECTOR3( vecBase , matBone._41, matBone._42, 1.0f );

        pkModel->GetAngleBone( (*it).m_iEndBone, &matBone );
        matBone = matBone * matWorld;
        SETVECTOR3( vecEnd , matBone._41, matBone._42, 1.0f );
        
        (*it).m_pEffect->Add( vecBase, vecEnd );

    }
}

D3DXMATRIX* MONSTER::ComputePositionBone(D3DXMATRIX* matOut, int Bone_Num, const D3DXMATRIX& matBasic, std::vector<KPositionBone*>& vecPositionBone )
{
    return IGCComputePaletteP3M::ComputePositionBone( matOut, Bone_Num, matBasic, vecPositionBone);
}

D3DXMATRIX* MONSTER::ComputeAngleBone( D3DXMATRIX* matOut, SFrame* pFrame, int Bone_Num, const D3DXMATRIX& matBasic )
{
    if( pFrame == NULL || pFrame->A_Bone == NULL )
        return matOut;

    if ( pFrame->dwNumAngleBone <= (DWORD)Bone_Num  )
    {
        *matOut = matBasic;
        return matOut;
    }

    std::map<int, D3DXVECTOR3>::iterator mapIter = m_mapUserBoneAngle.find( Bone_Num );
    if( mapIter != m_mapUserBoneAngle.end() )
    {
        D3DXMATRIX matRotX, matRotY, matRotZ, matRotation;

        D3DXMatrixIdentity( &matRotX );
        D3DXMatrixIdentity( &matRotY );
        D3DXMatrixIdentity( &matRotZ );
        D3DXMatrixIdentity( &matRotation );

        if( 0.0f != mapIter->second.x )
        {
            D3DXMatrixRotationX( &matRotX, mapIter->second.x );
        }
        if( 0.0f != mapIter->second.y )
        {
            D3DXMatrixRotationY( &matRotY, mapIter->second.y );
        }
        if( 0.0f != mapIter->second.z )
        {
            D3DXMatrixRotationZ( &matRotZ, mapIter->second.z );
        }

        matRotation = matRotX * matRotY * matRotZ * matBasic;
        D3DXMatrixMultiply(matOut, &pFrame->A_Bone[Bone_Num], &matRotation);

        return matOut;
    }

    return IGCComputePaletteP3M::ComputeAngleBone(matOut, pFrame, Bone_Num, matBasic );
}

void MONSTER::SetAttackBoundBox( int iWidth, int iHeight, int iIndex )
{
    m_vecAttackRect[iIndex].fWidth = (float)iWidth / 400.0f;
    m_vecAttackRect[iIndex].fHeight = (float)iHeight / 400.0f;    
}

void MONSTER::SetPushBoundBox( int iWidth, int iHeight, int iIndex )
{
    m_fWidth =  m_vecPushRect[iIndex].fWidth = (float)iWidth / 400.0f;
    m_fHeight = m_vecPushRect[iIndex].fHeight = (float)iHeight / 400.0f;    
}

bool MONSTER::CheckTargetInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY/* = 0*/ )
{
    if( !IsTarget() )
        return false;
    
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

	iY1 += (float)iOffsetY / 400.0f;
	iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    if ( IsTargetMonster() )
    {
		MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iTargetMonsterIndex);
		if( pMonster == NULL){
			return false;
		}

        // Å¸ÄÏÀÇ À¯È¿¼º °Ë»ç
        if( false == EffectiveTarget() )
            return false;

        // 2011.2.8, shmhlove : ¹üÀ§¾È¿¡ Å¸°ÙÀÌ ÀÖ´ÂÁö »ìÇÉ´Ù
        GCAttackRect<float> rect;
        rect.SetRect( iX1 - 1.0f, iY1 - 0.25f, iX2 - 1.0f, iY2 - 0.25f );
        std::vector< GCAttackRect<float> >::iterator vecIter = pMonster->m_vecAttackRect.begin();
        for(; vecIter != pMonster->m_vecAttackRect.end(); ++vecIter)
        {
            if( rect.CheckCollision( *vecIter, NULL ) )
                return true;
        }

        // ¿¹Àü ¹üÀ§Ã¼Å© ¹æ½Ä
        //if( pMonster->m_afX[LATENCY_INDEX] > iX1 && 
        //    pMonster->m_afX[LATENCY_INDEX] < iX2 &&
        //    pMonster->m_afY[LATENCY_INDEX] > iY2 && 
        //    pMonster->m_afY[LATENCY_INDEX] < iY1 )
        //{
        //    return true;
        //}

        return false;
    }
    else
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[m_iTargetPlayerIndex];

        // Å¸ÄÏÀÇ À¯È¿¼º °Ë»ç
        if( false == EffectiveTarget() )
            return false;

        // ¾ö¸¶ÇÑÅ× °¡ÀÚ~
        if( pPlayer->m_kUserInfo.dwUID == m_dwOwnerUID )
        {
            // °Å¸® È®ÀÎ
            if( pPlayer->GetSummonMonsterLimitDist() > DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] ) )
                ReleaseTarget();
            return false;
        }


        // ¹üÀ§¾È¿¡ Å¸°ÙÀÌ ÀÖ´ÂÁö »ìÇÉ´Ù            
        if( pPlayer->vPos.x > iX1 && 
            pPlayer->vPos.x < iX2 &&
            pPlayer->vPos.y > iY2 && 
            pPlayer->vPos.y < iY1 )
        {  
            return true;
        }

        return false;
    }
}

int MONSTER::GetEnemyCntInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/, bool bNoPassPlayer /*= false*/, bool bNoPassMonster /*= false*/)
{
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    int iCount = 0;

    if( false == bNoPassPlayer )
    {
        for( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

            if( false == EffectivePlayer(i) )
                continue;

            if( pPlayer->vPos.x > iX1 && 
                pPlayer->vPos.x < iX2 &&
                pPlayer->vPos.y > iY2 && 
                pPlayer->vPos.y < iY1 )
            {
                ++iCount;
            }
        }
    }

    if( false == bNoPassMonster )
    {
        GCAttackRect<float> rect;
        GCAttackRect<float> CollisionRect;
        rect.SetRect( iX1- 1.0f, iY1- 0.25f, iX2- 1.0f, iY2- 0.25f);
        for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ;  ++ mit ) {
            if( false == EffectiveMonster( mit->first ) )
                continue;

            if ( mit->second == NULL )
                continue;

            std::vector< GCAttackRect<float> >::iterator vecIter = mit->second->m_vecAttackRect.begin();
            for(; vecIter != mit->second->m_vecAttackRect.end(); ++vecIter)
            {
                if( rect.CheckCollision( *vecIter, &CollisionRect ) )
                {
                    ++iCount;
                    break;
                }
            }
        }
    }

    return iCount;
}

int MONSTER::GetUnitCntInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/, bool bNoPassPlayer /*= false*/, bool bNoPassMonster /*= false*/)
{
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    int iCount = 0;

    if( false == bNoPassPlayer )
    {
        for( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

            if( pPlayer->m_kUserInfo.bLive == false )
                continue;
            if( pPlayer->m_cLife == 0 )
                continue;
            if( pPlayer->CheckHiding() )
                continue;
            if( pPlayer->m_ePlayerState == PS_DEAD )
                continue;
            if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().START )
                continue;
            if ( pPlayer->m_bNotTargeting == true)
                continue;

            if( pPlayer->vPos.x > iX1 && 
                pPlayer->vPos.x < iX2 &&
                pPlayer->vPos.y > iY2 && 
                pPlayer->vPos.y < iY1 )
            {
                ++iCount;
            }
        }
    }

    if( false == bNoPassMonster )
    {
        GCAttackRect<float> rect;
        GCAttackRect<float> CollisionRect;
        rect.SetRect( iX1- 1.0f, iY1- 0.25f, iX2- 1.0f, iY2- 0.25f);
        for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ;  ++ mit ) 
        {
            MONSTER *pMonster = mit->second;
            if( pMonster == NULL || !pMonster->IsLive() )
                continue;

            if( NPC_NONE != pMonster->m_eNPC )
                continue;

            if( pMonster->m_bMonsterTarget == false )
                continue;

            std::vector< GCAttackRect<float> >::iterator vecIter = pMonster->m_vecAttackRect.begin();
            for(; vecIter != pMonster->m_vecAttackRect.end(); ++vecIter)
            {
                if( rect.CheckCollision( *vecIter, &CollisionRect ) )
                {
                    ++iCount;
                    break;
                }
            }
        }
    }

    return iCount;
}

void MONSTER::RegistReaction( int iAttackType, int iAttackDir, char* strState )
{
    std::string strReaction = strState;
    std::pair<EGCAttackType, EGCAttackDirection> key = std::make_pair( (EGCAttackType)iAttackType, (EGCAttackDirection)iAttackDir );
    m_mapReaction.insert( std::make_pair( key, strReaction ) );
}

CDamageInstance* MONSTER::AddDamage( int iDamage, float fStartAngle )
{
    CDamageInstance* damage = g_MyD3D->m_pDamageManager->MonsterAdd( iDamage , m_iMonsterIndex );	
    if( damage )
        damage->m_Angle = fStartAngle;
	return damage;
}


CDamageInstance* MONSTER::AddDamageWithLocateDirect( int iDamage ,float fStartX, float fStartY,  bool right)
{
	CDamageInstance* damage = g_MyD3D->m_pDamageManager->MonsterAdd( iDamage , m_iMonsterIndex );	
	if(damage)
	{
		damage->m_IsRight = right;
		damage->m_X+=fStartX;
		damage->m_Y+=fStartY;
		
	}
	return damage;
}


CDamageInstance* MONSTER::AddDamageLastPlayers( int iDamage )
{
    if ( false == g_MyD3D->IsPlayerIndex( m_iLastAttackedMe ) )
        return NULL;

    CDamageInstance* damage =  g_MyD3D->m_pDamageManager->MonsterAddLastPlayers( iDamage, m_iLastAttackedMe, m_iMonsterIndex );	
	return damage;
}

CDamageInstance* MONSTER::AddDamageWithAngle( int iDamage, float fStartAngle )
{
    CDamageInstance* damage =  g_MyD3D->m_pDamageManager->MonsterAddWithAngle( iDamage , m_iMonsterIndex, fStartAngle );
	
	return damage;
}

CDamageInstance* MONSTER::AddDamageWithDirectionAngle( int iDamage, float fStartAngle, bool bRight )
{
    CDamageInstance* damage =  g_MyD3D->m_pDamageManager->MonsterAddWithAngle( iDamage , m_iMonsterIndex, fStartAngle );

    if( damage )
    {
        damage->m_IsRight = bRight;
    }

    return damage;
}

CDamageInstance* MONSTER::AddDamageWithLocate( int iDamage, float fStartX, float fStartY, bool bAbsolutePos, bool bDirection_ )
{
	CDamageInstance* damage = NULL;
    if( !bAbsolutePos )
    {
        float fDirection = m_abIsRight[ LATENCY_INDEX ] ? 1.0f : -1.0f;
        float fPosX = fStartX * fDirection + m_afX[ LATENCY_INDEX ];
        float fPosY = fStartY + m_afY[ LATENCY_INDEX ];

        damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iDamage , m_iMonsterIndex, fPosX, fPosY );
    }
    else
    {
        damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iDamage , m_iMonsterIndex, fStartX, fStartY );
        if ( damage && bDirection_ == false )
        {
            damage->m_IsRight = !damage->m_IsRight;
        }
    }

	
	return damage;
}

CDamageInstance* MONSTER::AddDamageWithLocateAndIdentifie( DWORD dwIdentifie, int iDamage, float fStartX, float fStartY, bool bAbsolutePos, bool bDirection_ )
{
    CDamageInstance* damage = NULL;
    if( !bAbsolutePos )
    {
        float fDirection = m_abIsRight[ LATENCY_INDEX ] ? 1.0f : -1.0f;
        float fPosX = fStartX * fDirection + m_afX[ LATENCY_INDEX ];
        float fPosY = fStartY + m_afY[ LATENCY_INDEX ];

        damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iDamage , m_iMonsterIndex, fPosX, fPosY );
    }
    else
    {
        damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iDamage , m_iMonsterIndex, fStartX, fStartY );
        if ( damage && bDirection_ == false )
        {
            damage->m_IsRight = !damage->m_IsRight;
        }
    }

    if( damage )
    {
        damage->m_dwIdentifie = dwIdentifie;
    }

    return damage;
}

bool MONSTER::IsLiveDamageIdentifie( DWORD dwIdentifie )
{
    std::map< int, std::vector<CDamageInstance*> >& mapDamageInstance = g_MyD3D->m_pDamageManager->GetDamageInstancePool();
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = mapDamageInstance.begin();
    for(; mapIter != mapDamageInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter)
        {
            if( (*vecIter)->m_dwIdentifie == dwIdentifie )
                return true;
        }
    }

    return false;
}

CDamageInstance* MONSTER::AddDamageWithStatic( int iDamage, float fStartX, float fStartY, bool bNoDirection )
{
    CDamageInstance* damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iDamage , m_iMonsterIndex, fStartX, fStartY );
    if ( damage )
    {
        if( false == bNoDirection )
            damage->m_IsRight = m_abIsRight[LATENCY_INDEX];
        else
            damage->m_IsRight = true;
    }

    return damage;
}

CDamageInstance* MONSTER::AddDamageWithStatic2( int iDamage, float fStartX, float fStartY )
{
    CDamageInstance* damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iDamage , m_iMonsterIndex, fStartX, fStartY );
    if ( damage )
    {
        damage->m_X = fStartX;
        damage->m_Y = fStartY;
    }

    return damage;
}

CDamageInstance* MONSTER::AddDamageWithLocateAngle( int iDamage, float fStartX, float fStartY, bool bAbsolutePos, float fStartAngle, bool bMoveAngle )
{
    CDamageInstance* damage = AddDamageWithLocate( iDamage, fStartX, fStartY, bAbsolutePos );
    if( damage )
    {
        damage->m_Angle = fStartAngle;

        if( bMoveAngle )
        {
            float sx = damage->m_pOriginalDamage->m_x_Speed;
            float sy = damage->m_pOriginalDamage->m_y_Speed;
            sy = sinf(damage->m_Angle) * sx;
            sx = cosf(damage->m_Angle) * sx;
            damage->m_pOriginalDamage->m_x_Speed = sx;
            damage->m_pOriginalDamage->m_y_Speed = sy;
        }
    }
    
	return damage;
}

CDamageInstance* MONSTER::AddDamageWithTargetPos( int iDamage, float fStartX, float fStartY, float fTargetX, float fTargetY, char* strAction /*= "" */ )
{
    CDamageInstance* damage = NULL;

    float fDirection = m_abIsRight[ LATENCY_INDEX ] ? 1.0f : -1.0f;
    float fPosX = fStartX * fDirection + m_afX[ LATENCY_INDEX ];
    float fPosY = fStartY + m_afY[ LATENCY_INDEX ];
    damage = g_MyD3D->m_pDamageManager->MonsterAddWithTargetPos( iDamage, m_iMonsterIndex, fStartX, fStartY, fTargetX, fTargetY, strAction );

    return damage;
}

CDamageInstance* MONSTER::AddDamageWithSpeed( int iDamage, float fStartX, float fStartY, float fSpeedX, float fSpeedY )
{
    CDamageInstance* damage = g_MyD3D->m_pDamageManager->MonsterAdd( iDamage , m_iMonsterIndex );	
    if(damage)
    {
        damage->m_IsRight = m_abIsRight[ LATENCY_INDEX ];
        damage->m_X+=fStartX;
        damage->m_Y+=fStartY;
        damage->m_pOriginalDamage->m_x_Speed = fSpeedX;
        damage->m_pOriginalDamage->m_y_Speed = fSpeedY;
    }
    return damage;
}

CDamageInstance* MONSTER::AddDamageWithStaticAndSpeed( int iDamage, float fStartX, float fStartY, float fSpeedX, float fSpeedY )
{
    CDamageInstance* damage = g_MyD3D->m_pDamageManager->MonsterAdd( iDamage , m_iMonsterIndex );	
    if(damage)
    {
        damage->m_IsRight = m_abIsRight[ LATENCY_INDEX ];
        damage->m_X = fStartX;
        damage->m_Y = fStartY;
        damage->m_pOriginalDamage->m_x_Speed = fSpeedX;
        damage->m_pOriginalDamage->m_y_Speed = fSpeedY;
    }
    return damage;
}

void MONSTER::AddDamageWithPacket( int iDamageEnum, int iAddType, float fPosX, float fPosY, float fSpeedX, float fSpeedY, bool bSendLocal )
{
    if( false == bSendLocal )
    {
        if( g_kGlobalValue.m_kUserInfo.bHost == false )
            return;
    }

    KGC_PID_BROAD_ADD_DAMAGE kDamageInfo;
    kDamageInfo.iPlayerIndex = m_iMonsterIndex;
    kDamageInfo.iDamageEnum = iDamageEnum;
    kDamageInfo.iAddDamageType = iAddType;
    kDamageInfo.bMonster = true;
    kDamageInfo.vPos = D3DXVECTOR2(fPosX, fPosY);
    kDamageInfo.vSpeed = D3DXVECTOR2(fSpeedX, fSpeedY);
    SendP2PPacket( &kDamageInfo, sizeof(kDamageInfo), KNC_BROADCAST, _RELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_ADD_DAMAGE" );
}

void MONSTER::DirectMotionInput( const char* strState )
{
    SetState( strState , true );
}

int MONSTER::GetLastFrame_( int iIndex )
{
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
   if( mapAction.size() > 0 )
    {
        std::string strMotion = g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_aiMotion[iIndex] );
        std::map< std::string, KGCMonsterAction >::iterator mit = mapAction.find( strMotion );
        // ±×·± ¸ð¼ÇÀÌ ¾ø´Ù?
        if( mit == mapAction.end() )
            return 0;

        if( (int)m_vecMotionList.size() <= mit->second.m_iMotionIndex )
            return 0;
		
		return m_vecMotionList[ mit->second.m_iMotionIndex ]->GetNum_Frame() - 1;
    }
    else
        return 0;
}

int MONSTER::CountPlayerInRect( int iLeft, int iTop, int iRight, int iBottom )
{
    int iCount = 0;

    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( pPlayer->m_kUserInfo.bLive == false )
            continue;
        if( pPlayer->m_cLife == 0 )
            continue;
        if( pPlayer->CheckHiding() )
            return false;
        if( pPlayer->m_ePlayerState == PS_DEAD )
            return false;
        if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().START )
            return false;
        if ( pPlayer->m_bNotTargeting == true)
            return false;
        
        if( pPlayer->vPos.x > m_afX[ LATENCY_INDEX ] + iX1 &&
            pPlayer->vPos.x < m_afX[ LATENCY_INDEX ] + iX2 &&
            pPlayer->vPos.y > m_afY[ LATENCY_INDEX ] + iY2 &&
            pPlayer->vPos.y < m_afY[ LATENCY_INDEX ] + iY1 )
        {
            ++iCount;
        }
    }

    return iCount;
}

bool MONSTER::CheckPreState( char* strState )
{
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    if( mapAction.size() > 0 )
    {
        std::string strPreMotion = g_kMonsterManager.GetMonsterStateToIndex( m_iMonsterType, m_iPreMotion );
        return( strPreMotion.compare( strState ) == 0 );
    }
    return false;
}

int MONSTER::GetMonsterNameID()
{
    return m_iMonsterNameID;
}

const WCHAR* MONSTER::GetMonsterName()
{ 
    return m_strMonsterName.c_str();
}

void MONSTER::AddParticle( char* strSeqeunce, float fOffsetX, float fOffsetY, bool bOnUI )
{
	MSG_PARTICE_CREATE_INFO msg;
	msg.strSeqname = strSeqeunce;
	msg.vPos.x = fOffsetX;
	msg.vPos.y = fOffsetY;

	if ( false == bOnUI )
	{
		float fDir = m_abIsRight[ LATENCY_INDEX ] ? 1.0f : -1.0f;
		msg.vPos.x = fOffsetX * fDir;		
		msg.bDirecitonal = true;
	}
	else
	{
		msg.pTracePos = &g_MyD3D->MyHead->m_vCameraLookPos;
		msg.fTraceTime = 5.0f;
	}

	m_ParticleMsg.push_back( msg );
}

void MONSTER::AddParticleNoDirection( char* strSeqeunce, float fOffsetX, float fOffsetY )
{
	MSG_PARTICE_CREATE_INFO msg;
	msg.strSeqname = strSeqeunce;
	msg.vPos.x = fOffsetX;
	msg.vPos.y = fOffsetY;
	msg.bDirecitonal = false;

    m_ParticleMsg.push_back( msg );
}

CParticleEventSeqPTR MONSTER::AddTraceParticleToBoneEx( std::string strSeqeunce, float fTraceTime, int iBoneIndex_, int iLeftSideLayer_, int iRightSideLayer_,  float xOffset_, float yOffset_, bool bNoDirection )
{
    if ( !m_pObject )
        return NULL;

    float fY_Ex = 0.0f;
    fY_Ex = y_Ex;

    D3DXVECTOR2 vPos;

    if ( bNoDirection ) 
    {
        vPos.x = xOffset_;
    }
    vPos.y = yOffset_;


    CParticleEventSeqPTR pSequence = NULL;	
    if( *(strSeqeunce.end() - 1) == '/' )
    {
        strSeqeunce.erase(strSeqeunce.end()-1);
        std::string strParticleName = strSeqeunce;
        if ( !bIsRight )
        {			
            strParticleName += "_R";
            pSequence = g_ParticleManager->CreateSequence( strParticleName, vRenderPos.x + vPos.x, vRenderPos.y + fY_Ex + vPos.y, 0.5f );
        }
    }
    if ( !pSequence )
    {
        pSequence = g_ParticleManager->CreateSequence( strSeqeunce, vRenderPos.x + vPos.x, vRenderPos.y + fY_Ex + vPos.y, 0.5f );
    }


    if( pSequence )
    {
        if( !bNoDirection )
        {
            DIRECTION_INFO stDirect;
            stDirect.m_bDirVelocity = true;
            stDirect.m_fDirPosX = -xOffset_;
            stDirect.m_pbIsRight = &bIsRight;

            pSequence->SetDirectInfo( &stDirect );
            pSequence->SetShow( true );
        }

        {
            TRACE_INFO stTrace;

            if( fTraceTime == 0.0f )
                stTrace.m_fParticleTraceTime = pSequence->GetKillTime();
            else
                stTrace.m_fParticleTraceTime = fTraceTime;

            stTrace.m_fXOffset = vPos.x;
            stTrace.m_fYOffset = vPos.y;
            if ( iBoneIndex_ == -1 )
            {			
                stTrace.m_pvPos = &vRenderPos;
                pSequence->SetShow( &m_bRender );

            }
            else if( iBoneIndex_ != -1 )
            {
                ParticlePosDesc* pDesc = new ParticlePosDesc();			
                if( pSequence->GetKillNum() == 0 && pSequence->GetKillTime() == 0 ) 
                {
                    pDesc->pParticle = pSequence;
                }                
                stTrace.m_pvPos =  &pDesc->m_vPos;			
                pDesc->m_iBoneIndex = iBoneIndex_;
                pDesc->m_iLeftSideLayer = iLeftSideLayer_;
                pDesc->m_iRightSideLayer = iRightSideLayer_;

                pSequence->SetShow( &m_bRender );
                m_pObject->m_vecPosDesc.push_back( pDesc );

                if( stTrace.m_fParticleTraceTime < 0.0f )
                {
                    D3DXMATRIX matPartcle;
                    const D3DXMATRIX *matWorld = m_pObject->GetWorldMat();

                    m_pObject->GetAngleBone( iBoneIndex_, &matPartcle );        
                    D3DXMatrixMultiply( &matPartcle, &matPartcle, matWorld );

                    pSequence->SetPosition( D3DXVECTOR3(matPartcle._41,matPartcle._42,matPartcle._43)  );
                }
            }
            pSequence->SetTrace( &stTrace );
        }
    }

    return pSequence;

}
void MONSTER::AddParticleNoDirectionToBone( char* strSeqeunce, float fOffsetX, float fOffsetY, int iBone )
{
    MSG_PARTICE_CREATE_INFO msg;
    msg.strSeqname = strSeqeunce;
    msg.vPos.x = fOffsetX;
    msg.vPos.y = fOffsetY;
    msg.bDirecitonal = false;
    msg.iBoneID = iBone;

    m_ParticleMsg.push_back( msg );
}

void MONSTER::AddTraceParticleNoDirection( char* strSeqeunce )
{
	AddTraceParticlePos( strSeqeunce, 0.0f, -1, 0.0f, 0.0f, true );
}

void MONSTER::AddParticleArg( KParticleArg* pArg )
{
	{
		float fDir = m_abIsRight[ LATENCY_INDEX ] ? 1.0f : -1.0f;
		pArg->fOffsetX = pArg->fOffsetX * fDir; 
	}

	MSG_PARTICE_CREATE_INFO msg;
	msg.strSeqname = pArg->strParticleName;
	msg.vPos.x = pArg->fOffsetX;
	msg.vPos.y = pArg->fOffsetY;
	msg.bDirecitonal = true;
	msg.fOffsetX = pArg->fDirPosX;

	m_ParticleMsg.push_back( msg );
}

void MONSTER::AddTraceParticleToBone( char* strSeqeunce, float fTraceTime, int iBoneIndex_, int iLeftSideLayer_, int iRightSideLayer_, float xOffset_, float yOffset_, bool bNoDirection_ )
{
	MSG_PARTICE_CREATE_INFO msg;
	msg.strSeqname = strSeqeunce;
    
    if ( bNoDirection_ == false )
    {
        msg.fOffsetX = xOffset_;
    }
    else
    {
	    msg.vPos.x = xOffset_;
    }

	msg.vPos.y = yOffset_;

	msg.fTraceTime = fTraceTime;
	msg.bDirecitonal = !bNoDirection_;
	msg.iBoneID = iBoneIndex_;
	msg.iLeftSideLayer = iLeftSideLayer_;
	msg.iRightSideLayer = iRightSideLayer_;

	if (iBoneIndex_ == -1)
	{
		msg.pTracePos = &vRenderPos;
	}

	m_ParticleMsg.push_back( msg );
}

void MONSTER::AddTraceParticleOffset( char* strSeqeunce, float fOffsetX, float fOffsetY, float fTraceTime)
{
	MSG_PARTICE_CREATE_INFO msg;
	msg.strSeqname = strSeqeunce;
	msg.vPos.x = fOffsetX;
	msg.vPos.y = fOffsetY;
	msg.pTracePos = &vRenderPos;
	msg.fTraceTime = fTraceTime;
	
	m_ParticleMsg.push_back( msg );

}

void MONSTER::AddTraceParticlePos( std::string strSeqeunce, float fTraceTime, int iBoneIndex_, float fPosX_, float fPosY_, bool bIsNoDirection )
{
	MSG_PARTICE_CREATE_INFO msg;
	msg.strSeqname = strSeqeunce;
	msg.vPos.x = fPosX_;
	msg.vPos.y = fPosY_;

	msg.fTraceTime = fTraceTime;
	msg.bDirecitonal = !bIsNoDirection;
	msg.iBoneID = iBoneIndex_;
	
	if ( iBoneIndex_ == -1 )
		msg.pTracePos = &vRenderPos;

	m_ParticleMsg.push_back( msg );
}

void MONSTER::AddTraceParticle( char* strSeqeunce, float fTraceTime, float fOffsetX, float fOffsetY, bool bIsNoDirection )
{
	AddTraceParticlePos( strSeqeunce, fTraceTime, -1, fOffsetX, fOffsetY, bIsNoDirection );
}

void MONSTER::AddTraceParticleDieToClear( char* strSeqeunce_, float fTraceTime_, int iBoneIndex_, int iLeftSideLayer_, int iRightSideLayer_, float xOffset_, float yOffset_, bool bNoDirection )
{
    if ( !m_pObject )
        return;

    float fY_Ex = 0.0f;
    fY_Ex = y_Ex;

    D3DXVECTOR2 vPos;

    if ( bNoDirection ) 
    {
        vPos.x = xOffset_;
    }
    vPos.y = yOffset_;

    std::string strSeqeunce = strSeqeunce_;
    CParticleEventSeqPTR pSequence = NULL;	
    if( *(strSeqeunce.end() - 1) == '/' )
    {
        strSeqeunce.erase(strSeqeunce.end()-1);
        std::string strParticleName = strSeqeunce;
        if ( !bIsRight )
        {			
            strParticleName += "_R";
            pSequence = g_ParticleManager->CreateSequence( strParticleName, vRenderPos.x + vPos.x, vRenderPos.y + fY_Ex + vPos.y, 0.5f );
        }
    }
    if ( !pSequence )
    {
        pSequence = g_ParticleManager->CreateSequence( strSeqeunce, vRenderPos.x + vPos.x, vRenderPos.y + fY_Ex + vPos.y, 0.5f );
    }


    if( pSequence )
    {
        if( !bNoDirection )
        {
            DIRECTION_INFO stDirect;
            stDirect.m_bDirVelocity = true;
            stDirect.m_fDirPosX = -xOffset_;
            stDirect.m_pbIsRight = &bIsRight;

            pSequence->SetDirectInfo( &stDirect );
            pSequence->SetShow( true );
        }

        {
            TRACE_INFO stTrace;

            if( fTraceTime_ == 0.0f )
                stTrace.m_fParticleTraceTime = pSequence->GetKillTime();
            else
                stTrace.m_fParticleTraceTime = fTraceTime_;

            stTrace.m_fXOffset = vPos.x;
            stTrace.m_fYOffset = vPos.y;
            if ( iBoneIndex_ == -1 )
            {			
                stTrace.m_pvPos = &vRenderPos;
                pSequence->SetShow( &m_bRender );

            }
            else if( iBoneIndex_ != -1 )
            {
                ParticlePosDesc* pDesc = new ParticlePosDesc();			
                if( pSequence->GetKillNum() == 0 && pSequence->GetKillTime() == 0 ) 
                {
                    pDesc->pParticle = pSequence;
                }                
                stTrace.m_pvPos =  &pDesc->m_vPos;			
                pDesc->m_iBoneIndex = iBoneIndex_;
                pDesc->m_iLeftSideLayer = iLeftSideLayer_;
                pDesc->m_iRightSideLayer = iRightSideLayer_;

                pSequence->SetShow( &m_bRender );
                m_pObject->m_vecPosDesc.push_back( pDesc );

                if( stTrace.m_fParticleTraceTime < 0.0f )
                {
                    D3DXMATRIX matPartcle;
                    const D3DXMATRIX *matWorld = m_pObject->GetWorldMat();

                    m_pObject->GetAngleBone( iBoneIndex_, &matPartcle );        
                    D3DXMatrixMultiply( &matPartcle, &matPartcle, matWorld );

                    pSequence->SetPosition( D3DXVECTOR3(matPartcle._41,matPartcle._42,matPartcle._43)  );
                }
            }
            pSequence->SetTrace( &stTrace );
        }
    }

    m_vecDieToClearParticle.push_back( pSequence );
}

void MONSTER::ReverseGravity( float fRange, float fDamage, bool bFlyCheck )
{
    for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->vPos.x > m_afX[ LATENCY_INDEX ] - fRange && g_MyD3D->MyPlayer[i]->vPos.x < m_afX[ LATENCY_INDEX ] + fRange )
        {
            g_MyD3D->MyPlayer[i]->ReverseGravity( MAX_PLAYER_NUM, 0.06f, fDamage, !bFlyCheck );
        }
    }
}

void MONSTER::KamikiLightningReady( float fX, float fY )
{
    float fLastBottomPos = SiKGCFantasticMap()->GetLastBottomPosition( fX, fY );
    fY = fY > fLastBottomPos ? fY : fLastBottomPos;

    fY = SiKGCFantasticMap()->FindShadowPos( fX, fY );

    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x = fX;
    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y = fY;
    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight = true;

    if( m_apKamikiLightning[m_iKamikiLightningIndex] == NULL )
    {
        m_apKamikiLightning[m_iKamikiLightningIndex] = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "Lightning" ) );
        m_apKamikiLightning[m_iKamikiLightningIndex]->SetShow( false );
        m_apKamikiLightning[m_iKamikiLightningIndex]->Stop();
    }

    m_apKamikiLightning[m_iKamikiLightningIndex]->GetMatrix()->Scale( 0.0002f, 0.015f, 1.0f );
    m_apKamikiLightning[m_iKamikiLightningIndex]->Stop();
    m_apKamikiLightning[m_iKamikiLightningIndex]->Start( CKTDGAnim::PT_ONE_PASS );
    m_apKamikiLightning[m_iKamikiLightningIndex]->SetShow( true );
    m_apKamikiLightning[m_iKamikiLightningIndex]->GetMatrix()->Move( fX - 1.0f, fY + 1.35f, 0.5f );

    m_iKamikiLightningIndex = ( m_iKamikiLightningIndex + 1 ) % KAMIKI_LIGHTNING_COUNT;
}

void MONSTER::KamikiLightning( float fX, float fY )
{
    float fLastBottomPos = SiKGCFantasticMap()->GetLastBottomPosition( fX, fY );
    fY = fY > fLastBottomPos ? fY : fLastBottomPos;

    fY = SiKGCFantasticMap()->FindShadowPos( fX, fY );

    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x = fX;
    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y = fY;
    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight = true;

    g_KDSound.Play( "43" );

    if( m_apKamikiLightning[m_iKamikiLightningIndex] == NULL )
    {
        m_apKamikiLightning[m_iKamikiLightningIndex] = g_MyD3D->m_pMapAnim->CreateInst( g_MyD3D->m_pMapAnim->GetSequence( "Lightning" ) );
        m_apKamikiLightning[m_iKamikiLightningIndex]->SetShow( false );
        m_apKamikiLightning[m_iKamikiLightningIndex]->Stop();
    }

    m_apKamikiLightning[m_iKamikiLightningIndex]->GetMatrix()->Scale( 0.002f, 0.015f, 1.0f );
    m_apKamikiLightning[m_iKamikiLightningIndex]->Stop();
    m_apKamikiLightning[m_iKamikiLightningIndex]->Start( CKTDGAnim::PT_ONE_PASS );
    m_apKamikiLightning[m_iKamikiLightningIndex]->SetShow( true );
    m_apKamikiLightning[m_iKamikiLightningIndex]->GetMatrix()->Move( fX - 1.0f, fY + 1.35f, 0.5f );

    AddDamageWithLocate( DT_MONSTER_KAMIKI07, fX, fY + 1.4f, true );

    m_iKamikiLightningIndex = ( m_iKamikiLightningIndex + 1 ) % KAMIKI_LIGHTNING_COUNT;

    g_MyD3D->MyStg->Color_r = 60;
    g_MyD3D->MyStg->Color_g = 60;
    g_MyD3D->MyStg->Color_b = 255;

    g_MyD3D->MySparks->IceFires( fX - 1.0f, fY - 0.1f, 0, 0.25f );

    //g_MyD3D->m_pDamageManager->Add( DT_KAMIKI_LIGHTNING_WAVE, MAX_PLAYER_NUM, false, m_iMonsterIndex );
}


void MONSTER::SkillReadyEffect( BYTE r, BYTE g, BYTE b, float fHeight )
{
    SkillReadyEffectNoStop( r, g, b, fHeight );
    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->Stop( m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX], false );
}

int MONSTER::GetSummonedMonsterCountforType( int iType )
{
    if( m_vecSummonMonsterType.empty() )
        return 0;

    // »ì¾Æ ÀÖ´Â ¸ó½ºÅÍ
    int iRet = 0;
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if( iType != pMonster->m_iMonsterType )
			continue;

		if( (m_vecSummonMonsterType.end() !=
			std::find( m_vecSummonMonsterType.begin(), m_vecSummonMonsterType.end(), pMonster->m_iMonsterType )) &&
			(pMonster->m_fHP > 0) )
		{
			iRet++;
		}
	}
 
    return iRet;
}

int MONSTER::GetSummonedMonsterCount()
{
    if( m_vecSummonMonsterType.empty() )
        return 0;

    // »ì¾Æ ÀÖ´Â ¸ó½ºÅÍ
    int iRet = 0;
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		MONSTER *pMonster = mit->second;
		if( (m_vecSummonMonsterType.end() != 
			std::find( m_vecSummonMonsterType.begin(), m_vecSummonMonsterType.end(), pMonster->m_iMonsterType )) &&
			(pMonster->m_bDie == false) )
		{
			iRet++;
		}
	}

    return iRet;
}

bool MONSTER::CheckSummonSlot( )
{
    int iSlot = g_MyD3D->CheckDunMonsterSummonSlot( );
    if( -1 == iSlot )
        return false;
    return true;
}

void MONSTER::SummonDamageMonster( KSummonInfo* pSummonInfo, int iSummonMonsterLevel, int iHitPlayerIdx, bool bIsCharLock )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return;

    float fX = pSummonInfo->fX / SiKGCFantasticMap()->GetPixelWidth();
    float fY = pSummonInfo->fY;

    int iSlot = g_kMonsterManager.GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE_SUMMON );

    if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
    {
        iSlot = g_MyD3D->AddMonster( pSummonInfo->iMonType, fX, fY, false, false, 0, iSummonMonsterLevel, 0, true, false );

		MONSTER *pMonster = g_kMonsterManager.GetMonster(iSlot);

        pMonster->m_bLive = true;
        pMonster->m_pObject->SetRender( true );
        pMonster->SetState( pSummonInfo->strState.c_str() );
		pMonster->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;

        if( bIsCharLock && g_MyD3D->IsPlayerIndex( iHitPlayerIdx ) )
        {
            // Ä«¸Þ¶ó ¼ÂÆÃ
            PLAYER *pPlayer = g_MyD3D->MyPlayer[iHitPlayerIdx];
            pPlayer->SetDamageSummonMonSlot( iSlot );
            if( pPlayer->IsLocalPlayer() )
            {
                g_kCamera.SetTargetMonster( iSlot );
            }

            // Ä³¸¯ÅÍ LockÃ³¸®
            g_MyD3D->MyPlayer[iHitPlayerIdx]->SetLock( true );
			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;

				if( mit->second->m_bLive && 
					mit->second->m_fHP > 0.0f && 
					mit->second->GetPlayerTarget() == iHitPlayerIdx )
					mit->second->ReleaseTarget();
			}
        }
    }
    else
    {
        // ÆÐÅ¶ ¹ß»ç
        KGC_PID_BROAD_SUMMON_DAMAGEMONSTER kPacket;
        kPacket.m_iSlot = iSlot;
        kPacket.m_usMonType = USHORT( pSummonInfo->iMonType );
        kPacket.m_usEntranceX = GCUTIL_MATH::floatToHalf( fX );
        kPacket.m_usEntranceY = GCUTIL_MATH::floatToHalf( fY );
        kPacket.m_iLevel = iSummonMonsterLevel;
        kPacket.m_iHitPlayerIdx = iHitPlayerIdx;
        kPacket.m_bCharLock = bIsCharLock;
        strcpy(kPacket.m_szState, pSummonInfo->strState.c_str());
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_SUMMON_DAMAGEMONSTER" );
    }
}

bool MONSTER::SummonMonster( KSummonInfo* pSummonInfo, bool bNoCheckType, bool bNoMonsterPos, int iSummonMonsterLevel )
{
    if( m_vecSummonMonsterType.empty() )
        return false;

    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return false;

    if( m_vecSummonMonsterType.end() == 
        std::find( m_vecSummonMonsterType.begin(), m_vecSummonMonsterType.end(), pSummonInfo->iMonType ) )
        return false;


	int iSlot = g_kMonsterManager.GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE_SUMMON );

	// 30 ¸¶¸® Á¦ÇÑÇÔ.
	if ( GetSummonedMonsterCountforType( pSummonInfo->iMonType) >= m_iSummonsNuberAvailable )
		return false;


    float x, y;
    if( bNoMonsterPos )
    {
        x = pSummonInfo->fX / SiKGCFantasticMap()->GetPixelWidth();
        y = pSummonInfo->fY;
    }
    else
    {
        float fXDiff = m_abIsRight[0] ? pSummonInfo->fX*0.0025f : pSummonInfo->fX*(-0.0025f);
        x = ( m_afX[0] + fXDiff ) / SiKGCFantasticMap()->GetPixelWidth();
        y = m_afY[0] + pSummonInfo->fY*0.0025f;
        SiKGCFantasticMap()->GetSafePosition( x, y, false );
    }

    int iLevel = g_kMonsterManager.GetBaseMonsterLevel(pSummonInfo->iMonType);

    if( 0 < iSummonMonsterLevel )
        iLevel = iSummonMonsterLevel;

    // ÆÐÅ¶ º¸³»°í break
    g_kSummonMonster->Init();
    strncpy( g_kSummonMonster->m_szState, pSummonInfo->strState.c_str(), pSummonInfo->strState.size() );
    g_kSummonMonster->m_usMonIndex = (USHORT)(iSlot);
    g_kSummonMonster->m_usMonType = (USHORT)( pSummonInfo->iMonType);
    g_kSummonMonster->m_usEntranceX = GCUTIL_MATH::floatToHalf( x );
    g_kSummonMonster->m_usEntranceY = GCUTIL_MATH::floatToHalf( y );
    g_kSummonMonster->m_iLevel = iLevel;
    g_kSummonMonster->m_bIsRight = m_abIsRight[0];
    g_kSummonMonster->m_iCountValue = pSummonInfo->iCountValue;
    g_kSummonMonster->m_bCheckSafePostion = pSummonInfo->bCheckSafePostion;
    SendNetworkData( GC_PID_SUMMON_MONSTER, KNC_BROADCAST );
    
    return false;
}

void MONSTER::SummonMonsterByLocal( KSummonInfo* pSummonInfo, bool bNoCheckType, bool bNoMonsterPos, int iSummonMonsterLevel )
{
    int iSlot = g_kMonsterManager.GetNextCreateMonstersNum(ECREATE_MONSTER_TYPE_SUMMON);

    // 30 ¸¶¸® Á¦ÇÑÇÔ.
    if ( GetSummonedMonsterCountforType( pSummonInfo->iMonType) >= m_iSummonsNuberAvailable )
        return;

    float x, y;
    if( bNoMonsterPos )
    {
        x = pSummonInfo->fX / SiKGCFantasticMap()->GetPixelWidth();
        y = pSummonInfo->fY;
    }
    else
    {
        float fXDiff = m_abIsRight[0] ? pSummonInfo->fX*0.0025f : pSummonInfo->fX*(-0.0025f);
        x = ( m_afX[0] + fXDiff ) / SiKGCFantasticMap()->GetPixelWidth();
        y = m_afY[0] + pSummonInfo->fY*0.0025f;
        SiKGCFantasticMap()->GetSafePosition( x, y, false );
    }

    int iLevel = g_kMonsterManager.GetBaseMonsterLevel(pSummonInfo->iMonType);

    if( 0 < iSummonMonsterLevel )
        iLevel = iSummonMonsterLevel;

    if( g_kMonsterManager.IsSummonMonsterByPlayer(iSlot) )
    {
        ASSERT( !"Custom->m_ucMonIndex >= SUMMON_MONSTER_INDEX" );
    }

    x = GCUTIL_MATH::halfToFloat( GCUTIL_MATH::floatToHalf( x ) );
    y = GCUTIL_MATH::halfToFloat( GCUTIL_MATH::floatToHalf( y ) );
    
    g_MyD3D->AddMonster( pSummonInfo->iMonType, x, y, m_abIsRight[0], 0, 0, iLevel, 0, pSummonInfo->bCheckSafePostion, false, iSlot );
    
    MONSTER* pMon = g_kMonsterManager.GetMonster( iSlot );

    if ( pMon == NULL ) return;

    pMon->m_bLive = true;
    pMon->m_pObject->SetRender( true );
    pMon->SetCountValue( pSummonInfo->iCountValue );
    pMon->SetState( pSummonInfo->strState.c_str() );
    pMon->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
}

void MONSTER::RegisterSummonMonsterType( int iIndex )
{
    if( m_vecSummonMonsterType.end() == std::find( m_vecSummonMonsterType.begin(), m_vecSummonMonsterType.end(), iIndex ) )
    {
        m_vecSummonMonsterType.push_back( iIndex );
    }
}

void MONSTER::CustomMonsterSetting()
{
    switch( GetDefaultMonster( m_iMonsterType ) )
    {
        case MON_KAMIKI:
        case MON_KAZEAZE2:
            g_AnimManager.GetNewDamageAnim( "ITEM_ELEMENTAL_DARK", CID_COMMON, m_vecDamageAnim, 3 );
            break;
        case MON_HERO_GOLEM:
        case MON_HERO_ICEGOLEM:
        case MON_HERO_FIREGOLEM:
            this->SetMagicEffect( m_iMonsterIndex, EGC_EFFECT_MON_INFINITY_DEFENCE, 999999.0f, 1, -1, false, false );
            break;
    }
}

void MONSTER::SetDamageEff( float fX, float fY, float fZ )
{
    int iIndex = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iIndex = 0;

    for( std::vector<KGCAnimManager::DamageAnim>::iterator j = m_vecDamageAnim.begin(); j != m_vecDamageAnim.end(); j++)
    {
        g_AnimManager.FrameUpdateDamageAnim( &(*j), D3DXVECTOR3( fX, fY, fZ), m_abIsRight[iIndex] );
    }
}

float MONSTER::ComputeAngleToTarget( float fPosX, float fPosY, float fMinAngle, float fMaxAngle )
{
    if( !IsTarget() )
    {
        Body_Angle = 0.0f;
        return Body_Angle;
    }

    if ( false == GetIsRight() )
    {
        fPosX *= -1.0f;
    }

    float fX = GetTargetX() - ( GetX() + fPosX );
    float fY = GetTargetY() - ( GetY() + fPosY );

    float fLength = ASMsqrt( powf( fX, 2.0f ) + powf( fY, 2.0f ) );
    float fAngle = asinf( fY / fLength );
    
    float fAdjustMinAngle = fMinAngle * (D3DX_PI / 180.0f);
    float fAdjustMaxAngle = fMaxAngle * (D3DX_PI / 180.0f);

    if ( fMinAngle != 0.0f && fAngle < fAdjustMinAngle )
    {
        fAngle = fAdjustMinAngle;
    }

    if ( fMaxAngle != 0.0f && fAngle > fAdjustMaxAngle )
    {
        fAngle = fAdjustMaxAngle;
    }

    Body_Angle = fAngle;

    return Body_Angle;
}

bool MONSTER::CheckBackAttack( int iPlayerIndex, bool bPhysicalDamage_ )
{

    if ( GetDefaultMonster( m_iMonsterType ) == MON_HALLOWEEN_PUMPKIN 
#if defined(ANGELS_EGG_REFORM)
        || GetDefaultMonster( m_iMonsterType ) == MON_ANGELSEGG
#endif
        )
        return false;


    bool bBackAttack = false;
	if( m_bNotBackAttack )
		goto END_CHECK_BACKATTACK;

    float fX = 0.0f;
    if( iPlayerIndex == g_MyD3D->Get_MyPlayer() ) // Å¸°ÙÀÌ ÀÚ±â ÀÚ½ÅÀÌ¸é..
    {
        fX = g_kLocalPlayerLatency->kRenderData[0].vPos.x;
    }
    else
    {
        fX = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.x;
    }

    // 2012. 06. 26. shmhlove, : ¾Æ·¡ µ¥¹ÌÁö ¹æÇâ¿¡ µû¸¥ ¹é¾îÅÃ ÆÇÁ¤À» Á¦°ÅÇÕ´Ï´Ù.
    /*
        1. ºÐ¸í µÚ¿¡¼­ ¶§·ÈÀ¸³ª µ¥¹ÌÁö ¹Ú½º°¡ À¯´Ö ¾Õ¿¡¼­ »ý¼ºµÇ¾î ¹é¾îÅÃ ÆÇÁ¤ÀÌ ¾Æ´Ñ °æ¿ì°¡ ¹ß»ý
        2. ±×·¸´Ù°í ÆÇÁ¤Á¶°Ç¿¡ µ¥¹ÌÁö ¹æÇâÀ» Á¦°ÅÇØ¹ö¸®¸é 
           À¯´Ö ¾Õ¿¡¼­ Áö¼Ó½Ã°£ÀÌ ²ÒÀÖ´Â ´Ù´ÜÈ÷Æ® µ¥¹ÌÁö¸¦ »ý¼º½ÃÄÑµÎ°í, À¯´Ö µÚ·Î ÀÌµ¿ÇÏ¿© ¾Õ¿¡¼­ ¶§·ÈÀ¸³ª ¹é¾îÅÃ ÆÇÁ¤À» ¸¸µé¾î ³¾ ¼ö ÀÖÀ½.
        => °á·Ð
           µÚ¿¡¼­ ¶§·ÈÀ¸³ª ¹é¾îÅÃ ÆÇÁ¤ÀÌ ¾È³ª´Â°Í º¸´Ù 
           ¾Õ¿¡¼­ ¶§·Á³õ°í µÚ·Î ÀÌµ¿ÇØ¼­ ¹é¾îÅÃ ÆÇÁ¤ÀÌ µÇ´Â°ÍÀÌ 
           "½ºÆ®·¹½º(ºÒ¸¸)°¡ ¾ÈµÇ¹Ç·Î ³Ð°Ô º¸¸é Â¥ÁõÀ¸·Î ÀÎÇÑ À¯ÀúÀÌÅ»À» ¸·´Âµ¥ ¿µÇâÀ» ÁÜ."
           ´Ü, ¹é¾îÅÃ ÆÇÁ¤ÀÌ ¸¹¾ÆÁúÅ×´Ï Å©Áö´Â ¾ÊÁö¸¸ º§·±½º¿¡ ¹®Á¦°¡ µÉ ¼ö ÀÖÀ½.

           ¿©Æ° ¹é¾îÅÃÆÇÁ¤¿¡¼­ µ¥¹ÌÁö¹æÇâÀ» Á¦°ÅÇÑ´Ù!!!
    */
    if( GetIsRight() ) // ¿À¸¥ÂÊÀ» ¹Ù¶óº¸°í ÀÖ´Âµ¥
    {
        if( GetX() > fX && bPhysicalDamage_ )
            bBackAttack = true;
        else if( GetX() > fX /*&& m_eDamageDir == DR_LEFT*/ )
            bBackAttack = true;
    }
    else // ¿ÞÂÊÀ» ¹Ù¶óº¸°í ÀÖ´Âµ¥
    {
        if( GetX() < fX && bPhysicalDamage_ )
            bBackAttack = true;
        else if( GetX() < fX /*&& m_eDamageDir == DR_RIGHT*/ )
            bBackAttack = true;
    }

END_CHECK_BACKATTACK:

    if( bBackAttack )
    {
        SiKGCSubjectManager()->AddBackAttack();
    }

    return bBackAttack;
}

bool MONSTER::CheckMonsterBackAttack( int iMonsterIndex, bool bPhysicalDamage_ )
{
    if( GetDefaultMonster( m_iMonsterType ) == MON_BASILISK ||
		GetDefaultMonster( m_iMonsterType ) == MON_GIANT ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_TREE ||
        GetDefaultMonster( m_iMonsterType ) == MON_ANGRYBOSS_DIVINE_TREE ||
        GetDefaultMonster( m_iMonsterType ) == MON_SUPERMON_DIVINE_TREE ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH1 ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH2 ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH3 ||
		GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH4 ||
        GetDefaultMonster( m_iMonsterType ) == MON_MOONEVENT ||
        GetDefaultMonster( m_iMonsterType ) == MON_HATSHEPSUT_COCOON ||
        GetDefaultMonster( m_iMonsterType ) == MON_HERO_FROST_BASILISK ||
        GetDefaultMonster( m_iMonsterType ) == MON_HERO_FLAME_BASILISK ||
        GetDefaultMonster( m_iMonsterType ) == MON_PATUPLAT_GOLEM ||
        GetDefaultMonster( m_iMonsterType ) == MON_COWNAT_CUBE || 
        GetDefaultMonster( m_iMonsterType ) == MON_WHALE_BOX ||
        GetDefaultMonster( m_iMonsterType ) == MON_WORMHOLE_UP ||
        GetDefaultMonster( m_iMonsterType ) == MON_WORMHOLE_DOWN ||
        GetDefaultMonster( m_iMonsterType ) == MON_TRIAL_MUSHGOLEM1 ||
        GetDefaultMonster( m_iMonsterType ) == MON_TRIAL_MUSHGOLEM2 ||
        GetDefaultMonster( m_iMonsterType ) == MON_ANGRYBOSS_HATSHEPSUT_COCOON
		) // ¹Ù½ÇÀº ¹é¾îÅÃ¾øÀ½..
        return false;

	MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonsterIndex);
	if( pMonster == NULL){
		return false;
	}

    float fX = 0.0f;
    fX = pMonster->m_afX[ LATENCY_INDEX ];

    if( GetIsRight() ) // ¿À¸¥ÂÊÀ» ¹Ù¶óº¸°í ÀÖ´Âµ¥
    {        
        if( GetX() > fX && bPhysicalDamage_ )
            return true;
        else if( GetX() > fX && m_eDamageDir == DR_LEFT )
            return true;
    }
    else // ¿ÞÂÊÀ» ¹Ù¶óº¸°í ÀÖ´Âµ¥
    {
        // ³¯ ¶§¸°³ðÀÌ ¿À¸¥ÂÊ¿¡ ÀÖÀ¸¸é ¹é¾îÅÃ
        if( GetX() < fX && bPhysicalDamage_ )
            return true;
        else if( GetX() < fX && m_eDamageDir == DR_RIGHT )
            return true;
    }
    return false;
}

bool MONSTER::CheckAerial()
{
    if( CheckState("DOWN_UP01") || CheckState("DOWN_UP02") )
    {
        return true;
    }

    return false;
}

void MONSTER::SetTriggerID( int iTriggerID )
{
    m_iTriggerID = iTriggerID;
}

void MONSTER::CameraVib( int iDirction, float fCameraVib, float fDecrease )
{
    // ¹æÇâÀÌ 0ÀÌ¸é ÁÂ¿ì Áøµ¿ 1ÀÌ¸é »óÇÏ Áøµ¿
    g_kCamera.CameraVib( iDirction, fCameraVib, fDecrease );
}

void MONSTER::FreezeAttack( float fDamage , bool bCatchEffefct )
{
    int iTargetMotion = MID_COMMON_FREEZED_BY_ENT;
    switch( GetDefaultMonster( m_iMonsterType ) )
    {
        case MON_ENT:
            iTargetMotion = MID_COMMON_FREEZED_BY_ENT;
            break;
        case MON_LICH_BOSS:
            iTargetMotion = MID_COMMON_FREEZED_BY_LICH;
            break;
    }

    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( pPlayer->m_kUserInfo.bLive == false )
            continue;
        if( pPlayer->m_cLife == 0 )
            continue;

        if( bCatchEffefct )
        {
            switch( GetDefaultMonster( m_iMonsterType ) )
            {
                case MON_ENT:
                {
                    // ÀâÀ¸·Á´Â ³ª¹«ÁÙ±â ¿¬Ãâ..
                    D3DXVECTOR2 vPos = pPlayer->vPos;
                    vPos.y = SiKGCFantasticMap()->FindShadowPos( vPos.x, vPos.y );

                    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x = vPos.x;
                    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y = vPos.y;
                    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight = true;
                    AddDamageWithLocate( DT_ENT_TRUNK_START, vPos.x, vPos.y, true );

                    break;
                }
                case MON_LICH_BOSS:
                {
                    D3DXVECTOR2 vPos = pPlayer->vPos;
                    vPos.y = SiKGCFantasticMap()->FindShadowPos( vPos.x, vPos.y );

                    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.x = vPos.x;
                    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->vPos.y = vPos.y;
                    g_MyD3D->MyPlayer[MAX_PLAYER_NUM]->bIsRight = true;
                    AddDamageWithLocate( DT_LICH_FREEZE_START, vPos.x, vPos.y, true );

                    break;
                }
            }
        }

        // ÀÌ°Ç ÀÚ±âÀÚ½ÅÀÏ¶§¸¸ Àû¿ëµÇ´Â »çÇ×..
        if( g_MyD3D->Get_MyPlayer() == i )
        {
            // ¹«ÀûÀÎ°¡?
            if( pPlayer->IsInvinsible( false ) == true )
                continue;

            // ÀÌ¹Ì °É·ÁÀÖ´Â »óÅÂ¶ó¸é ¶Ç °É¸®Áø ¾Ê´Â´Ù
            if( pPlayer->GetOwnMotion( iTargetMotion ) == pPlayer->uiMotion )
                continue;

            if( pPlayer->IsContact )
            {
                pPlayer->Direct_Motion_Input( iTargetMotion );                

                pPlayer->m_iPlusFreezeCount = 4;
                pPlayer->m_fFreezeDamage = fDamage;
            }
        }
    }
}

void MONSTER::SkillReadyEffectNoStop( BYTE r, BYTE g, BYTE b, float fHeight )
{
    g_KDSound.Play( "5" );
    if( r + g + b > 0 )
    {
        g_MyD3D->MyStg->Color_Stop_Count = 50 + ( LATENCY + SHOWLATENCY );
        g_MyD3D->MyStg->Color_r = r;
        g_MyD3D->MyStg->Color_g = g;
        g_MyD3D->MyStg->Color_b = b;
    }
    m_iSlowCount = 20;

    if( fHeight == 0.0f )
        fHeight = m_fAttackBoundBoxHeight / 2.0f;

    AddDamageWithLocate( DT_SKILL_READY_CIRCLE, 0, fHeight );
    g_MyD3D->MySparks->LightBim( m_afX[LATENCY_INDEX] - 1.0f, m_afY[LATENCY_INDEX] - 0.1f, LATENCY + SHOWLATENCY );
}

void MONSTER::ChampionMonsterAttack( int iIndex )
{
    if( m_dwChampionProperty == 0x00000000 || !g_MyD3D->IsPlayerIndex( iIndex ) )
        return;

    PLAYER* pPlayer = g_MyD3D->MyPlayer[iIndex];

    // °ø°Ý ½Ã ÀúÁÖ
    if( m_dwChampionProperty & CP_ATTACK_CURES )
    {
        float fCurrTime = 10.0f;
        fCurrTime -= fCurrTime * pPlayer->m_fAbility[ ABILITY_ABNORMAL_CONDITION_RESISTANCE];

        g_pMagicEffect->SetMagicEffect( iIndex, EGC_EFFECT_DARKBALL, fCurrTime );
    }

    //°ø°Ý ½Ã ¼®È­
    if( m_dwChampionProperty & CP_ATTACK_STONE )
    {
        if( pPlayer->IsContact )
        {
            g_Push_Count = 0;
            pPlayer->m_iStoneCurseType = DT_STONE_CURSE;
            pPlayer->Direct_Motion_Input( MID_COMMON_STONECURSE );            
            pPlayer->bIsRight  = true;
            pPlayer->x_Speed   = 0;
        }
        else
        {
            pPlayer->iContact_to_Stone = pPlayer->GetLastKillmePlayer();
        }
    }

    // °ø°Ý½Ã ºù°á
    if( m_dwChampionProperty & CP_ATTACK_ICE )
    {
        pPlayer->Direct_Motion_Input( MID_COMMON_FREEZED_BY_PIXIE );        

#ifdef LEVEL_DESIGN_STAT_CALC
        pPlayer->m_fFreezeDamage = -1.f;
#else
        pPlayer->m_fFreezeDamage = -0.01f;
#endif
		
        pPlayer->m_iPlusFreezeCount = 8;

        pPlayer->m_fFreezeDamage  -= pPlayer->m_fFreezeDamage * pPlayer->m_fAbility[ ABILITY_ABNORMAL_CONDITION_RESISTANCE];
    }
}

void MONSTER::AddBlackHoleEffect()
{
    Effect* pEffect = g_MyD3D->MySparks->Add( INTO_THE_DARKNESS, m_afX[LATENCY_INDEX], m_afX[LATENCY_INDEX], 0,
                                              1.5f, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX], 0.0f, m_abIsRight[LATENCY_INDEX] );

    if ( pEffect )
        pEffect->SetTrace( 0.05f, 0.0f, m_iMonsterIndex, true );
}

void MONSTER::PullPlayer( float yOffset /*= 0.0f*/, float fPullRate /*= 1.0f*/, float xOffset /*= 0.0f*/ )
{
    float fXTerm = m_abIsRight[LATENCY_INDEX] ? 0.18f : -0.18f;
    float xOffsetAD = m_abIsRight[LATENCY_INDEX] ? xOffset : -xOffset;

    if( m_aiFrame[LATENCY_INDEX] % 6 == 0 )
        g_kCamera.SetEarthQuake(8);

    int iMyIndex = g_MyD3D->Get_MyPlayer();
	if(iMyIndex>= MAX_PLAYER_NUM)
	{
		//³ªÁß¿¡ ·Î±× ³ÖÀÚ 
		return;
	}

    if( g_MyD3D->MyPlayer[iMyIndex]->m_kUserInfo.bLive )
    {
        if( g_MyD3D->MyPlayer[iMyIndex]->vPos.x > m_afX[LATENCY_INDEX] + fXTerm + xOffsetAD )
        {
            g_MyD3D->MyPlayer[iMyIndex]->vPos.x -= 0.018f * fPullRate;
        }
        else
        {
            g_MyD3D->MyPlayer[iMyIndex]->vPos.x += 0.018f * fPullRate;
        }

        if( g_MyD3D->MyPlayer[iMyIndex]->vPos.y > m_afY[LATENCY_INDEX] + yOffset )
        {
            g_MyD3D->MyPlayer[iMyIndex]->vPos.y -= 0.006f * fPullRate;
        }
        else
        {
            g_MyD3D->MyPlayer[iMyIndex]->vPos.y += 0.006f * fPullRate;
        }

        // MP °¨¼Ò½ÃÅ°Áö ¾Ê°ÚÀ½.. MP´Â ±ÍÇÑ°ÍÀÌ¿©..-¤µ -
        //if( g_MyD3D->MyHead->Level > 0 )
        //{
        //    g_MyD3D->MyHead->Level -= 0.01f;
        //    if( g_MyD3D->MyHead->Level < 0 )
        //        g_MyD3D->MyHead->Level = 0;

        //    g_MyD3D->MyHead->bMP_Shine = true;
        //}
    }
}



int MONSTER::m_aiMonsterRewardInfo[][NUM_MONSTER_REWARD] =
{
    // @1 : ¸ó½ºÅÍ »ç³É °æÇèÄ¡
    // @2 : ´øÀü Å¬¸®¾î °æÇèÄ¡

    // @1       @2
    { 4     ,   6       },  // 0
    { 4     ,   27      },  // 1
    { 4     ,   39      },  // 2
    { 4     ,   41      },  // 3
    { 4     ,   43      },  // 4
    { 5     ,   45      },  // 5
    { 5     ,   47      },  // 6
    { 5     ,   50      },  // 7
    { 5     ,   53      },  // 8
    { 5     ,   55      },  // 9
    { 6     ,   63      },  // 10
    { 7     ,   69      },  // 11
    { 8     ,   89      },  // 12
    { 9     ,   101     },  // 13
    { 10    ,   135     },  // 14
    { 11    ,   162     },  // 15
    { 13    ,   191     },  // 16
    { 14    ,   206     },  // 17
    { 15    ,   222     },  // 18
    { 18    ,   266     },  // 19
    { 20    ,   296     },  // 20
    { 20    ,   305     },  // 21
    { 20    ,   308     },  // 22
    { 21    ,   315     },  // 23
    { 22    ,   326     },  // 24
    { 24    ,   357     },  // 25
    { 39    ,   592     },  // 26
    { 58    ,   885     },  // 27
    { 122   ,   1875    },  // 28
    { 146   ,   2242    },  // 29
    { 193   ,   2978    },  // 30
    { 271   ,   4172    },  // 31
    { 310   ,   4770    },  // 32
    { 387   ,   5964    },  // 33
    { 629   ,   9695    },  // 34
    { 774   ,   11936   },  // 35
    { 919   ,   14178   },  // 36
    { 1065  ,   16420   },  // 37
    { 1210  ,   18663   },  // 38
    { 1356  ,   20907   },  // 39
    { 1549  ,   23898   },  // 40
    { 1740  ,   26837   },  // 41
    { 1930  ,   29770   },  // 42
    { 2139  ,   33000   },  // 43
    { 2330  ,   35937   },  // 44
    { 2520  ,   38868   },  // 45
    { 2709  ,   41796   },  // 46
    { 2899  ,   44719   },  // 47
    { 3112  ,   48000   },  // 48
    { 3301  ,   50926   },  // 49
    { 3491  ,   53848   },  // 50
    { 3685  ,   56847   },  // 51
    { 3879  ,   59845   },  // 52
    { 4074  ,   62843   },  // 53
    { 4268  ,   65841   },  // 54
    { 4462  ,   68840   },  // 55
    { 4657  ,   71841   },  // 56
    { 4851  ,   74841   },  // 57
    { 5046  ,   77841   },  // 58
    { 5240  ,   80842   },  // 59
    { 5629  ,   86836   },  // 60
    { 5823  ,   89838   },  // 61
    { 5824  ,   89845   },  // 62
    { 5824  ,   89852   },  // 63
    { 5825  ,   89858   },  // 64
    { 5825  ,   89863   },  // 65
    { 5825  ,   89869   },  // 66
    { 5826  ,   89875   },  // 67
    { 5826  ,   89880   },  // 68
    { 5826  ,   89884   },  // 69
    { 5827  ,   89889   },  // 70
    { 5827  ,   89893   },  // 71
    { 5827  ,   89898   },  // 72
    { 5827  ,   89902   },  // 73
    { 5828  ,   89905   },  // 74
    { 6677  ,   103015  },  // 75
};

// ¸ó½ºÅÍ°¡ ¸· Á×¾úÀ» ¶§ ÇÃ·¹ÀÌ¾î ¸Ó¸® À§¿¡ EXP ¾ó¸¶ ¹ÞÀºÁö ¶ç¿öÁØ´Ù

// ¸ó½ºÅÍ »ç³É °æÇèÄ¡ = ( ¸ó½ºÅÍ °æÇèÄ¡ * ÆÄÆ¼ º¸³Ê½º / ÆÄÆ¼¿ø ¼ö )
//                      ( ¸ó½ºÅÍ °æÇèÄ¡ÀÇ ÇÑ°è°ª = ÀÚ½ÅÀÇ ·¹º§º¸´Ù 5·¹º§ »óÀ§ÀÇ ¸ó½ºÅÍ °æÇèÄ¡ )

// ´øÁ¯ ¿Ï·á °æÇèÄ¡ = ( ¿Ï·á °æÇèÄ¡ )
//                    ( ¿Ï·á °æÇèÄ¡ÀÇ ÇÑ°è°ª = ÀÚ½ÅÀÇ ·¹º§º¸´Ù 5·¹º§ »óÀ§ÀÇ ´øÀü ¿Ï·á °æÇèÄ¡ )

void MONSTER::ShowExp()
{
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON )
        return;

    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];

    int iStartingMemberCount = 0;

    for( int i = 0; i < MAX_DUNGEON_PLAYER_NUM; ++i )
    {
        // °ÔÀÓ ½ÃÀÛ ½Ã Âü¿©ÇÑ ÇÃ·¹ÀÌ¾î¶ó¸é?
        if( g_pGameBoard->m_pStartingMember[i].IsLive )
        {
            // ½ÃÀÛ ÇÒ ‹š ÀÖ¾ú´ø »ç¶÷ ¼ö ¼¼±â
            ++iStartingMemberCount;
        }
    }

    // ¹Ù¿îµå Ã¼Å©
    if( iStartingMemberCount < 1 ) // 1¸íº¸´Ù ÀÛÀ»¼ö ¾øÁö-¤µ-
        iStartingMemberCount = 1;
    else if( iStartingMemberCount > MAX_DUNGEON_PLAYER_NUM ) // 4¸íº¸´Ù ¸¹À»¼ö ¾øÀ½..
        iStartingMemberCount = MAX_DUNGEON_PLAYER_NUM;

    // ¸ó½ºÅÍ »ç³É ½Ã È¹µæ °æÇèÄ¡
    float fExp = kMonsterTemplate.fExp;
    if( fExp > 0.0f ) // ¿©±â¼­ ±¸ÇÑ exp°¡ 0ÀÌ¶ó¸é ¾ê´Â °æÇèÄ¡¸¦ ¾Æ¿¹ ¾ÈÁÖ´Â ³à¼®ÀÌ¶ó¼­ ¶ç¿öÁÖÁö ¾Ê°Ú´Ù
    {
        int iIndex = g_MyD3D->Get_MyPlayer();
        PLAYER* pPlayer = g_MyD3D->MyPlayer[iIndex];

        if( pPlayer->m_kUserInfo.bLive && pPlayer->m_cLife > 0 )
        {

            if ( false == SiKGCWorldMapManager()->IsHeroDungeon() )

			{
                g_kGlobalValue.AdjustExp( pPlayer->m_kUserInfo.GetCurrentChar().iLevel, m_iLevel, m_iMonsterType, fExp );
            }
			
			if ( SiKGCFatigueManager()->IsFatigueSystemApplyUser() == true ) {
				float fRatio = static_cast<float>(SiKGCFatigueManager()->GetGameRatio()) / 100.f;
				fExp *= fRatio;
			}

            ShowExpEffect( iIndex, static_cast<float>(fExp) );
        }
    }
}

void MONSTER::KazeazeBlackBall()
{
    int iAngle = 45;
    for( int i = 0 ; i < 4 ; ++i, iAngle += 90 )
    {
        CDamageInstance* fire = NULL;
        fire = AddDamageWithLocate( DT_KAZEAZE_DARKBALL, 0.0f, 0.8f );
        if( fire != NULL )
        {
            fire->m_Angle = D3DXToRadian( iAngle );
        }
    }
}

// ¶È°°Àº ¸ó½ºÅÍ°¡ ¿©·µÀÖ°í..
// ÇØ´ç ¸ó½ºÅÍ¿¡ Æ¯º°ÇÑ Ã³¸®¸¦ ÇØÁà¾ßµÇ¸é ¿ä ÇÔ¼ö ÇÑ¹ø °ÅÃÄ¼­ °Ë»çÇÏ¸é µÉµí..
EGCMonster MONSTER::GetDefaultMonster( int iMonsterType )
{
    switch( (EGCMonster)iMonsterType )
    {
        case MON_ENT:
        case MON_VS_ENT:
            return MON_ENT;

        case MON_KAMIKI:
        case MON_VS_KAMIKI:
            return MON_KAMIKI;

        case MON_PUMPKIN:
            return MON_PUMPKIN;

        case MON_LICH_BOSS:
        case MON_LICH:
        case MON_VS_LICH_BOSS:
            return MON_LICH_BOSS;

        case MON_PARADOM_BOSS:
        case MON_PARADOM:
        case MON_VS_PARADOM_BOSS:
            return MON_PARADOM_BOSS;

        case MON_BASILISK:
        case MON_VS_BASILISK:
            return MON_BASILISK;

        case MON_PATUSEI_HEAD:
        case MON_VS_PATUSEI_HEAD:
            return MON_PATUSEI_HEAD;

        case MON_PATUSEI_BODY:
        case MON_VS_PATUSEI_BODY:
            return MON_PATUSEI_BODY;

        case MON_PATUSEI_LEG:
        case MON_VS_PATUSEI_LEG:
            return MON_PATUSEI_LEG;
    }

    return (EGCMonster)iMonsterType;
}

void MONSTER::Fear( bool bConfusion )
{
    if( m_aiFrame[LATENCY_INDEX] % 5 == 0 )
        g_kCamera.SetEarthQuake(8);

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if( pPlayer->m_kUserInfo.bLive )
    {
        if( pPlayer->vPos.x > m_afX[LATENCY_INDEX] )
        {
            pPlayer->vPos.x += 0.022f;
        }
        else
        {
            pPlayer->vPos.x -= 0.022f;
        }

        if( bConfusion )
        {
            g_pMagicEffect->SetMagicEffect( g_MyD3D->Get_MyPlayer(), EGC_EFFECT_CONFUSION, 8.0f );
        }
    }
}

float MONSTER::GetDamageFactor( PLAYER* pPlayer ) const
{
    switch( m_iMonsterType )
    {
    case MON_ELENA:
    case MON_KAMIKI:
        {
            if( true == pPlayer->m_bKamikiDefence )
            {
#ifdef LEVEL_DESIGN_STAT_CALC
                return -1.5f;
#else
                return -0.015f;
#endif
				
            }
        }
        break;
    case MON_KAZEAZE:
    case MON_KAZEAZE2:
        {
            if( true == pPlayer->m_bKazeazeDefence )
            {
#ifdef LEVEL_DESIGN_STAT_CALC
                return -1.5f;
#else
                return -0.015f;
#endif
            }
        }
        break;
    }

#ifdef LEVEL_DESIGN_STAT_CALC
    return -3.f;
#else
    return -0.03f;
#endif
}


void MONSTER::SetHP( float fHP_ )
{
    if ( g_MyD3D != NULL && g_MyD3D->m_pStateMachine != NULL)
        if ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME )
            return;
	g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(true);
	SetAttackedMonster(true);

    if ( fHP_ > m_fHPPoint )
    {
        fHP_ = m_fHPPoint;
    }
    m_fHP = fHP_;

#if defined( USE_MERRY_HP_BAR )
    ShowEscortMonsterHPBar();
#endif

}

float MONSTER::GetMaxHP()
{
    return m_fHPPoint;
}

void MONSTER::SetMaxHP(float fHP)
{
    m_fHPPoint = fHP;
}

float MONSTER::GetAttackPoint()
{
    return m_fAttackPoint;
}

void MONSTER::SetAttackPoint( float fAttackPoint )
{
    m_fAttackPoint = fAttackPoint;
}

float MONSTER::GetDistance( MONSTER* pMonster_ )
{
    return DISTANCE( GetX(), GetY(), pMonster_->GetX(), pMonster_->GetY() );
}

float MONSTER::GetDistanceWithPlayer( PLAYER* pPlayer_ )
{
    return DISTANCE( GetX(), GetY(), pPlayer_->vPos.x, pPlayer_->vPos.y );
}



bool MONSTER::IsTarget()
{ 
    //¸ó½ºÅÍ¿Í ÇÃ·¹ÀÌ¾î µÑ´Ù Å¸ÄÏÀÌ µÇ¾î¼­´Â ¾ÈµÈ´Ù. ´Ù½Ã ¸»ÇØ µÑÁß ÇÏ³ª´Â -1ÀÌ¾î¾ß ÇÑ´Ù.
    //ASSERT( m_iTargetPlayerIndex == -1 || m_iTargetMonsterIndex == -1 );

    return ( m_iTargetPlayerIndex != -1 || m_iTargetMonsterIndex != -1 ); 
} 

bool MONSTER::IsTargetMonster() const
{
    //¸ó½ºÅÍ¿Í ÇÃ·¹ÀÌ¾î µÑ´Ù Å¸ÄÏÀÌ µÇ¾î¼­´Â ¾ÈµÈ´Ù. ´Ù½Ã ¸»ÇØ µÑÁß ÇÏ³ª´Â -1ÀÌ¾î¾ß ÇÑ´Ù.
    //ASSERT( m_iTargetPlayerIndex == -1 || m_iTargetMonsterIndex == -1 );

    return m_iTargetMonsterIndex != -1;
}

bool MONSTER::IsTargetPlayer() const
{
    //¸ó½ºÅÍ¿Í ÇÃ·¹ÀÌ¾î µÑ´Ù Å¸ÄÏÀÌ µÇ¾î¼­´Â ¾ÈµÈ´Ù. ´Ù½Ã ¸»ÇØ µÑÁß ÇÏ³ª´Â -1ÀÌ¾î¾ß ÇÑ´Ù.
    //ASSERT( m_iTargetPlayerIndex == -1 || m_iTargetMonsterIndex == -1 );

    return m_iTargetPlayerIndex != -1;
}

bool MONSTER::IsTargetMonsterType(int iMonID) const
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iTargetMonsterIndex);
	if( pMonster == NULL){
		return false;
	}
    return (iMonID == pMonster->m_iMonsterType);
}

void MONSTER::SetMonsterTargetManual( int iTargetMonsterIndex_ )
{
    if ( g_kMonsterManager.IsMonsterIndex(iTargetMonsterIndex_) == false )
    {
        return;
    }
    if( m_iMonsterIndex != iTargetMonsterIndex_ )
    {
        m_iTargetMonsterIndex = iTargetMonsterIndex_;
        m_iTargetPlayerIndex = -1;
    }
}

void MONSTER::SetPlayerTargetManual( int iTargetPlayerIndex_ )
{
    //ASSERT( !(iTargetPlayerIndex_ < 0 || iTargetPlayerIndex_ > SiKGCRoomManager()->GetRoomInfo().usMaxUsers ) );
    if (iTargetPlayerIndex_ < 0 || iTargetPlayerIndex_ > SiKGCRoomManager()->GetRoomInfo().usMaxUsers )
    {
        return;
    }
    SetTargetPlayerIndex( iTargetPlayerIndex_ );
    m_iTargetMonsterIndex = -1;
}

bool MONSTER::IsBoxIn( float fOffsetX, float fOffsetY, KBoundBox& kBoundBox_, MONSTER* pkMonster_ ) const
{
    float iX1 = kBoundBox_.fLeft;
    float iY1 = kBoundBox_.fTop + fOffsetY;
    float iX2 = kBoundBox_.fRight;
    float iY2 = kBoundBox_.fBottom + fOffsetY;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += fOffsetX;
        iX2 += fOffsetX;
    }
    else
    {
        iX1 -= fOffsetX;
        iX2 -= fOffsetX;
    }

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    iX1 /= 400.0f;
    iY1 /= 400.0f;
    iX2 /= 400.0f;
    iY2 /= 400.0f;

    if( pkMonster_->m_afX[LATENCY_INDEX] > m_afX[ LATENCY_INDEX ] + iX1 && 
        pkMonster_->m_afX[LATENCY_INDEX] < m_afX[ LATENCY_INDEX ] + iX2 &&
        pkMonster_->m_afY[LATENCY_INDEX] > m_afY[ LATENCY_INDEX ] + iY2 && 
        pkMonster_->m_afY[LATENCY_INDEX] < m_afY[ LATENCY_INDEX ] + iY1 )
    {
        return true;
    }

    return false;
}

bool MONSTER::CheckStandBlock()
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
        return !SiKGCFantasticMap()->IsAvailableGoDown(m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ]);
    return ( !g_MyD3D->MyStg->IsEnableNoCheck(m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ]) || m_afY[ LATENCY_INDEX ] <= -0.5f );
}

bool MONSTER::CheckSideBlock()
{
    if( SiKGCRoomManager()->IsMonsterGameMode() )
        return !SiKGCFantasticMap()->IsAvailableGoSide(m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ]);
    return false;
}

void MONSTER::SetMeshRender( int iIndex_, bool bRender_ )
{
    if ( m_pObject->GetMeshObjectNum() <= iIndex_ )
    {
        ASSERT_MBOX( "SetMeshRender 실패" );
        return;
    }
    GCMeshObject* pObj = m_pObject->GetMeshObject( iIndex_ );
    pObj->m_bRender = bRender_;    
}

void MONSTER::SetTexture( char *szTex )
{
    m_iSelectTextrue = -1;
    if( 0 < strlen(szTex) )
    {
        if( !(m_iSelectTextrue = m_pObject->IsLoadTexture( szTex )) )
            m_iSelectTextrue = m_pObject->AddTexture( g_pGCDeviceManager2->CreateTexture( szTex ) );
    }
    m_pObject->SetTexture( m_iSelectTextrue );
}

bool MONSTER::GetMeshRender( int iIndex_ )
{
    if ( m_pObject->GetMeshObjectNum() <= iIndex_ )
    {
        ASSERT_MBOX( "GetMeshRender 실패" );
        return false;
    }
    GCMeshObject* pObj = m_pObject->GetMeshObject( iIndex_ );
    return pObj->m_bRender;    
}

bool MONSTER::IsLastBottom()
{
    if ( SiKGCFantasticMap()->IsLastBottomPosition( m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ] ) )
        {
            return true;
        }        
    return false;
}
    
void MONSTER::FlashScreen()
{
    g_MyD3D->MyHead->White_Count = 2 + SHOWLATENCY;
}

void MONSTER::HPGaugeRender()
{
    if( !m_bLive || !m_bRender || !g_kGlobalValue.m_bRenderMonster || m_bHPrenderSkip )
        return;

    if ( IsHideName() )
        return;

    if( false == m_bToggleInfo )
        return;

    int iIndex = 0;
    //int iIndex = LATENCY_INDEX;
    //if( !g_kGlobalValue.m_kUserInfo.bHost )
    //    iIndex = 0;


	D3DXVECTOR3 pos1;
	D3DXVECTOR3 pos2;
	D3DXVECTOR3 pos3;
	D3DXVECTOR3 pos4;

#if !defined( __PATH__ ) // ¸ó½ºÅÍÀÇ ¹Ù¿îµù¹Ú½º
    {
		if( g_kGlobalValue.m_bRenderBoundBox )
        {
			/*static GCDeviceTexture* pNullTexture = g_pGCDeviceManager2->CreateTexture("ÀÓ½Ã·Î »ç¿ëÇÏ´Â ³Î ÅØ½ºÃÄ");

			g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_kCamera.m_matCamera );
			pNullTexture->SetDeviceTexture();*/

			// °ø°Ý ¹Ù¿îµù ¹Ú½º ±×¸®±â
            for( int i = 0 ; i < GetAttackBoundRectNum(); i++ )
            {
			    GCCollisionRect<float> fRect = GetAttackBoundRect(i);
			    D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
			    SETVECTOR3( lefttop, fRect.m_Left, fRect.m_Top - 0.25f, 0.4f );
			    SETVECTOR3( righttop, fRect.m_Right, fRect.m_Top - 0.25f, 0.4f );
			    SETVECTOR3( leftbottom, fRect.m_Left, fRect.m_Bottom - 0.25f, 0.4f );
			    SETVECTOR3( rightbottom, fRect.m_Right, fRect.m_Bottom - 0.25f, 0.4f );
			    //          g_MyD3D->Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 180, 0, 0, 200 );

			    g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
				    lefttop,  righttop, leftbottom, rightbottom,0,0,0,
				    D3DCOLOR_ARGB( 180, 0, 0, 200));
            }

			// ¹Ì´Â ¹Ù¿îµù ¹Ú½º ±×¸®±â
            for( int i = 0 ; i < GetPushRectNum(); i++ )
            {
                GCCollisionRect<float> fRect = GetPushBoundRect(i);
                D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
                SETVECTOR3( lefttop, fRect.m_Left, fRect.m_Top - 0.25f, 0.4f );
                SETVECTOR3( righttop, fRect.m_Right, fRect.m_Top - 0.25f, 0.4f );
                SETVECTOR3( leftbottom, fRect.m_Left, fRect.m_Bottom - 0.25f, 0.4f );
                SETVECTOR3( rightbottom, fRect.m_Right, fRect.m_Bottom - 0.25f, 0.4f );

                g_pGCDeviceManager2->DrawInWorld(NULL, &g_kCamera.m_matCamera,
                    lefttop,  righttop, leftbottom, rightbottom,0,0,0,
                    D3DCOLOR_ARGB( 60, 0, 0, 0));
                //            g_MyD3D->Draw_Billboard3( lefttop,  righttop, leftbottom, rightbottom, 60, 0, 0, 0 );
            }
			
        }        
    }
#endif

    if ( m_fHP > 0.0f )
    {
        float fX = m_afX[iIndex] - 1.0f;
        float fY = m_afY[iIndex] - 0.51f;

		float fWidth = m_fAttackBoundBoxWidth;
		float fHeight = 0.15f;

        if ( m_iNameBone != -1 )
        {
            D3DXMATRIX matName;
            const D3DXMATRIX *matWorld = m_pObject->GetWorldMat();
            m_pObject->GetAngleBone( m_iNameBone, &matName );
            D3DXMatrixMultiply( &matName, &matName, matWorld );
            fX = matName._41 + m_fNameOffset_X;
            fY = matName._42 - 0.05f + m_fNameOffset_Y;
        }
        else
        {
            if( GetDefaultMonster( m_iMonsterType ) == MON_BASILISK ||
                m_iMonsterType == MON_HERO_FLAME_BASILISK ||
                m_iMonsterType == MON_HERO_FROST_BASILISK )
            {
                fWidth = 1.35f;
                fX -= 0.16f;
                fY -= 0.6f;
            }
            else if( GetDefaultMonster( m_iMonsterType ) == MON_GIANT_WORM )
            {
                fX -= m_abIsRight[iIndex] ? -0.2f : 0.2f;
                fY -= 0.06f;
            }
            else if( GetDefaultMonster( m_iMonsterType ) == MON_DRAKE_WORM )
            {
                fX -= m_abIsRight[iIndex] ? -1.05f : 1.05f;
                fY -= 0.15f;
            }
            else if( GetDefaultMonster( m_iMonsterType ) == MON_GIANT )
            {
                fY -= -6.3f;
            }
			else if( GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH1
				 ||  GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH2
				 ||  GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH3
				 ||  GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH4 )
			{
				fX -= m_abIsRight[iIndex] ? -0.6f : 0.5f;
				fY -= 0.14f;
			}
        }

		pos1.x = fX - fWidth * 0.5f;
		pos1.y = fY - fHeight * 0.5f;
		pos1.z = 0.4f;

		pos2.x = fX - fWidth * 0.5f;
		pos2.y = fY + fHeight * 0.5f; 
		pos2.z = 0.4f;

		pos3.x = fX + fWidth * 0.5f;
		pos3.y = fY - fHeight * 0.5f;
		pos3.z = 0.4f;

		pos4.x = fX + fWidth * 0.5f;
		pos4.y = fY + fHeight * 0.5f;
		pos4.z = 0.4f;		

		fY -= 0.005f;

        float fHP = m_fHP;
        float fHPPoint = m_fHPPoint;

        // ÆÄÅõ¼¼ÀÌ ´Ù¸®¸é.. ÆÄÅõ¼¼ÀÌ ¸öÀÌ¶û ¸Ó¸®ÀÇ HPµµ ¸ðµÎ ÀÚ±â°¡ º¸¿©ÁØ´Ù-¤µ -;
        switch( GetDefaultMonster( m_iMonsterType ) )
        {
        case MON_PATUSEI_LEG:
            {
                for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
                    if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
                        continue;
                    MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

                    if(pMonster == NULL) continue;

                    if( GetDefaultMonster( pMonster->m_iMonsterType ) == MON_PATUSEI_HEAD ||
                        GetDefaultMonster( pMonster->m_iMonsterType ) == MON_PATUSEI_BODY )
                    {
                        fHP += pMonster->m_fHP;
                        fHPPoint += pMonster->m_fHPPoint;
                    }
                }
            }
        }

        if( m_fHP <= m_fHPPoint )
        {
            m_kMonHPBar.SetNowValue( static_cast<int>(m_fHP) );
            m_kMonHPBar.SetMaxValue( static_cast<int>(m_fHPPoint) );
        }
        m_kMonHPBar.SetRenderPos( fX - (fWidth * 0.5f), fY - (fHeight * 0.5f) );
        m_kMonHPBar.SetWidth( fWidth );
        m_kMonHPBar.SetHeight( fHeight );
        m_kMonHPBar.DrawPrograssBar( GC_LAYER_FACE );
	}
}

void MONSTER::TextOutLineRender()
{
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    if( !m_bLive || !m_bRender || !g_kGlobalValue.m_bRenderMonster || m_iMonsterNameID == 0 || kMonsterTemplate.bNameRenderSkip )
        return;

    if ( IsHideName() )
        return;

    if( false == m_bToggleInfo )
        return;

    int iIndex = 0;

    D3DXMATRIX mat1,mat2,mat3;
    if( m_abIsRight[iIndex] )
    {
        D3DXMatrixScaling( &mat3, 0.5f, 0.5f, 1.0f );
    }
    else
    {
        D3DXMatrixScaling( &mat1, 0.5f, 1.5f, 1.0f );
        D3DXMatrixRotationY( &mat2, 3.1415f );
        D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
    }

    if ( m_iNameBone != -1 )
    {
        D3DXMATRIX matName;
        const D3DXMATRIX *matWorld = m_pObject->GetWorldMat();
        m_pObject->GetAngleBone( m_iNameBone, &matName );
        D3DXMatrixMultiply( &matName, &matName, matWorld );
        matName._41 += m_fNameOffset_X;
        matName._42 += m_fNameOffset_Y;
        D3DXMatrixMultiply( &matName, &matName, &g_kCamera.m_matCamera );

        fScreenX = int( ( matName._41 + 1.0f ) * (400.f * GC_SCREEN_DIV_WIDTH ) ) + 0.5f * GC_SCREEN_DIV_WIDTH;
        fScreenY = int( ( -matName._42 + 0.75f ) * ( 400.f * GC_SCREEN_DIV_HEIGHT) ) + 0.5f  * GC_SCREEN_DIV_HEIGHT;
    }
    else
    {
        D3DXMatrixTranslation( &mat1, m_afX[iIndex] - 1.0f, m_afY[iIndex] - 0.48f, 0.5f );
        D3DXMatrixMultiply( &mat2, &mat3, &mat1 );
        D3DXMatrixMultiply( &mat1, &mat2, &g_kCamera.m_matCamera );

        D3DXVECTOR3 vPos;
        float fTransX = -0.15f;
        float fTransY = -0.1f;
        if( GetDefaultMonster( m_iMonsterType ) == MON_GODOFCIRCULATION )
        {
            fTransY = -1.0f;
        }
        else if( GetDefaultMonster( m_iMonsterType ) == MON_GIANT )
        {
            fTransY = 3.0f;
        }
        else if( GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_TREE || 
                 GetDefaultMonster( m_iMonsterType ) == MON_ANGRYBOSS_DIVINE_TREE ||
                 GetDefaultMonster( m_iMonsterType ) == MON_SUPERMON_DIVINE_TREE )
        {
            fTransY = 0.5f;
        }
		else if( GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH1 ||
			     GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH2 )
		{
			fTransX = 0.32f;
			fTransY = -0.15f;
		}
		else if( GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH3 ||
				 GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH4 )
		{
			fTransX = -0.52f;
			fTransY = -0.15f;
		}

        ::D3DXMatrixTranslation( &mat2, fTransX, fTransY, 0.0f );
        ::D3DXMatrixMultiply( &mat3, &mat1, &mat2 );
        ::D3DXVec3TransformCoord( &vPos, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &mat3 );

        vPos.x += 0.15f;
        vPos.y -= 0.04f;
        if( GetDefaultMonster( m_iMonsterType ) == MON_BASILISK ||
            m_iMonsterType == MON_HERO_FLAME_BASILISK ||
            m_iMonsterType == MON_HERO_FROST_BASILISK )
        {
            vPos.y -= 0.55f;
            vPos.x -= 0.1f;
        }
        else if( GetDefaultMonster( m_iMonsterType ) == MON_DRAKE_WORM )
        {
            vPos.y -= 0.12f;
            vPos.x -= m_abIsRight[iIndex] ? -1.05f : 1.05f;
        }
        else if( GetDefaultMonster( m_iMonsterType ) == MON_GIANT_WORM )
        {
            vPos.y -= 0.05f;
            vPos.x -= m_abIsRight[iIndex] ? -0.2f : 0.2f;
        }
        fScreenX = int( ( vPos.x + 1.0f ) * (400.f * GC_SCREEN_DIV_WIDTH ) ) + 0.5f * GC_SCREEN_DIV_WIDTH;
        fScreenY = int( ( -vPos.y + 0.75f ) * ( 400.f * GC_SCREEN_DIV_HEIGHT) ) + 0.5f  * GC_SCREEN_DIV_HEIGHT;
        fScreenY -= 52.0f * GC_SCREEN_DIV_HEIGHT;
    }

    WCHAR strMonsterName[MAX_PATH];
    SAbilityRevision* pAbilityRevision = SiKGCWorldMapManager()->GetAbilityRevisionInfo( SiKGCRoomManager()->GetGameMode() );
    bool bAbilityRevision = ( pAbilityRevision && pAbilityRevision->bLoadTable );
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER ||         // ¸ó½ºÅÍ ¿øÁ¤´ë
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_HUNT ||             // ¸ó½ºÅÍ ÇåÅÍ
        g_MyD3D->m_pkQuestGameMgr->GetReviseCharLevel() ||                              // Ä³¸¯ÅÍ ·¹º§ÀÌ ¿µ¿õ´øÀü °ø½ÄÀ¸·Î º¸Á¤µÈ ´øÀü
        bAbilityRevision ||                                                             // ´É·ÂÄ¡ º¸Á¤ÀÌµÈ ´øÀü
        SiKGCWorldMapManager()->IsHeroDungeon(SiKGCRoomManager()->GetGameMode()) &&     // ¿µ¿õ´øÀü( ¿µ¿õ´øÀüÀº Ä³¸¯ÅÍ ·¹º§ÀÌ º¸Á¤µÈ ´øÀü¿¡µµ Æ÷ÇÔµÊ )
        (g_kMonsterManager.IsSummonMonsterByPlayer(m_iMonsterIndex) == false) )
    {
        wsprintf( strMonsterName, L"%s", g_pkStrLoader->GetString( m_iMonsterNameID ).c_str() );
    }
    else if (g_kMonsterManager.IsSummonMonsterByPlayer(m_iMonsterIndex))
    {
        PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(m_dwOwnerUID);
        if (pPlayer)
        {
            if (m_iMonsterKind == EMON_ESCORT)
                wsprintf(strMonsterName, L"%s", g_pkStrLoader->GetString(m_iMonsterNameID).c_str());
            else if (m_dwOwnerUID == g_kGlobalValue.m_kUserInfo.dwUID)
                wsprintf(strMonsterName, L"★ %s(%s)", g_pkStrLoader->GetString(m_iMonsterNameID).c_str(), g_MyD3D->MyPlayer[pPlayer->m_iPlayerIndex]->GetStrUserName().c_str());
            else
                wsprintf(strMonsterName, L"%s(%s)", g_pkStrLoader->GetString(m_iMonsterNameID).c_str(), g_MyD3D->MyPlayer[pPlayer->m_iPlayerIndex]->GetStrUserName().c_str());
        }
    }
    else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
    {
        wsprintf( strMonsterName, L"%s", g_pkStrLoader->GetString( m_iMonsterNameID ).c_str() );
    }
    else
    {
        wsprintf( strMonsterName, L"%s %d. %s", g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str(), m_iLevel, g_pkStrLoader->GetString( m_iMonsterNameID ).c_str() );
    }

    GCDeviceFont* pFont = NULL;
    switch( m_eNameSize )
    {
    case SMALL_SIZE:
        pFont = g_pkFontMgrOutline;
        fScreenY -= SiKGCMultipleLanguages()->GetDefualtFontSize() * 0.5f; // À§Ä¡ ¸ÂÃß±â!
        break;

    case NORMAL_SIZE:
        pFont = g_pkFontMgrOutline16;
        fScreenY -= SiKGCMultipleLanguages()->GetLargeFontSize() * 0.5f; // À§Ä¡ ¸ÂÃß±â!
        break;

    case BIG_SIZE:
        pFont = g_pkFontMgrOutlineTitle;
        fScreenY -= SiKGCMultipleLanguages()->GetTitleFontSize() * 0.5f; // À§Ä¡ ¸ÂÃß±â!

	case SMALL_SIZE_CHAT:
		pFont = g_pkFontMgrOutlineSmall;
		fScreenY -= SiKGCMultipleLanguages()->GetSmallFontSize() * 0.5f; // À§Ä¡ ¸ÂÃß±â!
        break;
    }

	fScreenX *= g_pGCDeviceManager->GetWindowScaleX();
	fScreenY *= g_pGCDeviceManager->GetWindowScaleY();

    D3DCOLOR Color = ( m_dwChampionProperty != 0x00000000 ) ? 0xFFFFE080 : 0xFFFF0000;

    int iMyPlayer = g_MyD3D->Get_MyPlayer();

    if( g_MyD3D->IsPlayerIndex( iMyPlayer ) && g_MyD3D->IsSameTeam_PlayerAndMonster( g_MyD3D->MyPlayer[iMyPlayer]->m_iPlayerIndex, m_iMonsterIndex) )
        Color = 0xFF00FF00;

    if( g_kMonsterManager.IsSummonMonsterByPlayer(m_iMonsterIndex) )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerUID );
    if( (m_eNPC == NPC_OUR) ||
        (pPlayer && g_MyD3D->IsSameTeam( pPlayer->m_iPlayerIndex, g_MyD3D->MyPlayer[ iMyPlayer ]->m_iPlayerIndex)) )
            Color = 0xFF00FF00;
    }

#if defined( ANGELS_EGG_REFORM )
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG &&
        g_MyD3D->IsSameTeam_PlayerAndMonster( g_MyD3D->MyPlayer[iMyPlayer]->m_iPlayerIndex, m_iMonsterIndex ) )
        Color = 0xFF00FF00;
#endif

    if( pFont != NULL )
        pFont->OutTextXY((int)fScreenX, (int)fScreenY, strMonsterName, Color, NULL, DT_CENTER | DT_TOP ,0xff000000,GCDeviceFont::OUTLINE);
}

void MONSTER::TextRender()
{
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    if( !m_bLive ||!m_bRender || !g_kGlobalValue.m_bRenderMonster || kMonsterTemplate.bNameRenderSkip )
        return;
    if( IsHideName() )
        return;
    if( false == m_bToggleInfo )
        return;

	GCDeviceFont* pFont = NULL;

	WCHAR strMonsterName[MAX_PATH];
    SAbilityRevision* pAbilityRevision = SiKGCWorldMapManager()->GetAbilityRevisionInfo( SiKGCRoomManager()->GetGameMode() );
    bool bAbilityRevision = ( pAbilityRevision && pAbilityRevision->bLoadTable );
	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER ||         // ¸ó½ºÅÍ ¿øÁ¤´ë
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_HUNT ||             // ¸ó½ºÅÍ ÇåÅÍ
        g_MyD3D->m_pkQuestGameMgr->GetReviseCharLevel() ||                              // Ä³¸¯ÅÍ ·¹º§ÀÌ º¸Á¤µÈ ´øÀü
        bAbilityRevision ||                                                             // ´É·ÂÄ¡ º¸Á¤ÀÌµÈ ´øÀü
        SiKGCWorldMapManager()->IsHeroDungeon(SiKGCRoomManager()->GetGameMode()) &&     // ¿µ¿õ´øÀü( ¿µ¿õ´øÀüÀº Ä³¸¯ÅÍ ·¹º§ÀÌ º¸Á¤µÈ ´øÀü¿¡µµ Æ÷ÇÔµÊ )
        (g_kMonsterManager.IsSummonMonsterByPlayer(m_iMonsterIndex) == false) )
    {
		wsprintf( strMonsterName, L"%s", g_pkStrLoader->GetString( m_iMonsterNameID ).c_str() );
    }
    else if( g_kMonsterManager.IsSummonMonsterByPlayer(m_iMonsterIndex) )
    {
        PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerUID );
                            
        if( pPlayer )
        {
            if( m_iMonsterKind == EMON_ESCORT )
                wsprintf( strMonsterName, L"%s", g_pkStrLoader->GetString( m_iMonsterNameID ).c_str() );
            else if( m_dwOwnerUID == g_kGlobalValue.m_kUserInfo.dwUID )
                wsprintf( strMonsterName, L"¡Ú %s(%s)", g_pkStrLoader->GetString( m_iMonsterNameID ).c_str(), g_MyD3D->MyPlayer[ pPlayer->m_iPlayerIndex ]->GetStrUserName().c_str() );
            else
                wsprintf( strMonsterName, L"%s(%s)", g_pkStrLoader->GetString( m_iMonsterNameID ).c_str(), g_MyD3D->MyPlayer[ pPlayer->m_iPlayerIndex ]->GetStrUserName().c_str() );
        }
    }
    else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
    {
        wsprintf( strMonsterName, L"%s", g_pkStrLoader->GetString( m_iMonsterNameID ).c_str() );
    }
	else
    {
		wsprintf( strMonsterName, L"%s %d. %s", g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str(), m_iLevel, g_pkStrLoader->GetString( m_iMonsterNameID ).c_str() );
    }

    switch( m_eNameSize )
    {
    case SMALL_SIZE:		
        pFont = g_pkFontMgrOutline;		
        break;
    case NORMAL_SIZE:
        pFont = g_pkFontMgrOutline16;		
        break;
    case BIG_SIZE:		
        pFont = g_pkFontMgrOutlineTitle;	
	case SMALL_SIZE_CHAT:		
		pFont = g_pkFontMgrOutlineSmall;		
		break;

        break;
    }

    {
        D3DCOLOR Color = ( m_dwChampionProperty != 0x00000000 ) ? 0xFFFFE080 : 0xFFFF0000;

        int iMyPlayer = g_MyD3D->Get_MyPlayer();

        if( g_MyD3D->IsPlayerIndex( iMyPlayer ) && g_MyD3D->IsSameTeam_PlayerAndMonster( g_MyD3D->MyPlayer[iMyPlayer]->m_iPlayerIndex, m_iMonsterIndex) )
            Color = 0xFF00FF00;

        if (g_kMonsterManager.IsSummonMonsterByPlayer(m_iMonsterIndex))
        {
            PLAYER* pPlayer = g_MyD3D->GetPlayerByUID(m_dwOwnerUID);
            if ((m_eNPC == NPC_OUR) ||
                (pPlayer && g_MyD3D->IsSameTeam(pPlayer->m_iPlayerIndex, g_MyD3D->MyPlayer[iMyPlayer]->m_iPlayerIndex))) {
                if (m_iMonsterType == MON_ESCORT_MERRY)
                    Color = 0xFF663399;
                else
                    Color = 0xFF00FF00;
            }
        }

#if defined( ANGELS_EGG_REFORM )
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG &&
            g_MyD3D->IsSameTeam_PlayerAndMonster( g_MyD3D->MyPlayer[iMyPlayer]->m_iPlayerIndex, m_iMonsterIndex ) )
            Color = 0xFF00FF00;
#endif

        if( pFont != NULL )
            pFont->OutTextXY((int)fScreenX, (int)fScreenY, strMonsterName, Color, NULL, DT_CENTER | DT_TOP ,0xff000000,GCDeviceFont::INLINE);
    }

	if(m_fMonsterNameWidth == 0.0f){
		m_MissionIcon.SetScale( 0.12f, 0.12f );
		m_fMonsterNameWidth = static_cast<float>(pFont->GetWidth( strMonsterName ));
	}

	if(m_MissionIcon.IsSetTexture()){
		m_MissionIcon.SetPosition( (GetX() - m_fMonsterNameWidth/GC_SCREEN_WIDTH) - 0.12f, GetY() - 0.025f );
		m_MissionIcon.Render();
	}

    if( GetDefaultMonster( m_iMonsterType ) == MON_GIANT )
    {
        fScreenY += ( 2200.f * g_pGCDeviceManager->GetWindowScaleY() );
    }

    fScreenY -= (m_fAttackBoundBoxHeight * ( GC_SCREEN_FLOAT_WIDTH / 2.0f )) * g_pGCDeviceManager->GetWindowScaleY(); 

    /*
    std::wstringstream strChampionProperty;
    int iSize = sizeof( m_adwChampionPropertyList ) / sizeof( m_adwChampionPropertyList[0] );
    for( int i = 0; i < iSize; ++i )
    {
        if( m_dwChampionProperty & m_adwChampionPropertyList[i] )
        {
            strChampionProperty << g_pkStrLoader->GetString( m_aiChampionPropertyString[i] ).c_str() << std::endl;
        }
    }
	g_pkFontMgr->OutTextMultiline( (int)(fScreenX + 1), (int)(fScreenY + 1), strChampionProperty.str().c_str(), 0xFFFFFFFF, 1.2f, NULL, DT_CENTER | DT_TOP | DT_NO_COLOR );
	g_pkFontMgr->OutTextMultiline( (int)fScreenX, (int)fScreenY, strChampionProperty.str().c_str(), -1, 1.2f, NULL, DT_CENTER | DT_TOP );
	strChampionProperty.clear();
    */

#if defined( AI_HELPER )
	{
		if( g_kGlobalValue.m_bRenderMonsterStatus )
		{
			int iHPPercent = (int)( m_fHP / m_fHPPoint * 100.0f * 100.0f );

			// ¸ó½ºÅÍÀÇ ÇöÀç »óÅÂÇ¥½Ã
			std::wstringstream strmAIHelper;

            strmAIHelper << L"[ " << KncUtil::toWideString( GetState() ).c_str() << L" ] : /MA : " << (g_kGlobalValue.m_bMonsterAction?L"TRUE":L"FALSE") << std::endl;
            strmAIHelper << L"HP : " << (int)m_fHP << L" / " << (int)m_fHPPoint << L" (" << (float)iHPPercent / 100.0f << "%)" << std::endl;
            strmAIHelper << L"SD : " << (int)m_fShield << std::endl;
            strmAIHelper << L"AP : " << (int)( m_fAttackPoint ) << std::endl;
            strmAIHelper << L"Level : " << (int)( m_iLevel ) << std::endl;
            strmAIHelper << L"TYPE/SLOT : " << (int)( m_iMonsterType ) << L" / " << (int)( m_iMonsterIndex ) << std::endl;
            strmAIHelper << L"X / Y Pos : " << ( m_afX[ LATENCY_INDEX ] ) << L" / " << ( m_afY[ LATENCY_INDEX ] ) << std::endl;
            strmAIHelper << L"X / Y Block : " << (int)( TO_INDEX( m_afX[ LATENCY_INDEX ] ) ) << L" / " << (int)( TO_INDEX( m_afY[ LATENCY_INDEX ] ) ) << std::endl;

			fScreenY -= 60;

			g_pkFontMgr->OutTextMultiline( (int)(fScreenX + 1), (int)(fScreenY + 1), strmAIHelper.str().c_str(), 0xFFFFFFFF, 1.2f, NULL, DT_CENTER|DT_TOP );
			g_pkFontMgr->OutTextMultiline( (int)fScreenX, (int)fScreenY, strmAIHelper.str().c_str(), 0xFF00FF00, 1.2f, NULL, DT_CENTER|DT_TOP );
		}
	}
#endif

    g_pGCDeviceManager2->PushState();
    g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
    g_pGCDeviceManager2->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


    g_pGCDeviceManager2->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    for( std::vector<KMonsterSwoosh>::iterator it = m_vecSwoosh.begin(); it != m_vecSwoosh.end(); it++ )
    {
        (*it).m_pEffect->Render( (*it).m_pTexture,  &g_kCamera.m_matCamera );
    }
    g_pGCDeviceManager2->PopState();
}

void MONSTER::LuaActionCallBack( const char* strAction )
{
	ActionCallBack( strAction );
}

void MONSTER::AddDamageRelation( CDamageInstance* damage )
{
#if defined(RESOURCE_GRAPH)
	if(damage)
	{
		char tmp1[256];
		char tmp2[256];

		sprintf(tmp1,"DAMAGE_%d",damage->m_What);	
		sprintf(tmp2,"MONSTER_%d",m_iMonsterType);
		g_pGCDeviceManager2->AddRelation(tmp1,tmp2);		
	}
#endif
}

void MONSTER::AddParticleRelation( CParticleEventSeqPTR particle )
{
#if defined(RESOURCE_GRAPH)
	if(particle)
	{
		char tmp1[256];
		char tmp2[256];
		
		sprintf(tmp2,"MONSTER_%d",m_iMonsterType);
		g_pGCDeviceManager2->AddRelation("PARTICLE_"+particle->GetName(),tmp2);		
	}
#endif
}

void MONSTER::SetDrawSlash( unsigned int iIndex_, bool bRender )
{
    if( m_vecSwoosh.size() <= iIndex_ )
        return;

    m_vecSwoosh[iIndex_].m_pEffect->Show( bRender );
}

bool MONSTER::GetIsContactByPos()
{
    bool bNoCheckContact = false;
    if( m_fContactHeight > 0.0f )
        bNoCheckContact = bNoCheckContact || ( m_fContactHeight <= m_afY[LATENCY_INDEX] + m_fSpeedY );

    KGCContectInfo kInfo( m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ], m_fSpeedX, m_fSpeedY, bNoCheckContact, false );
    bool bRet = g_MyD3D->MyStg->Check_Contact( kInfo );    

    if( bRet && kInfo.m_ySpeed <= 0.0f )
    {
        // ¶¥¿¡ ºÙ¾úÀ½
        SetLatencyY(  LATENCY_INDEX , kInfo.m_y );        
        m_fContactHeight = 0.0f;
    }
    return bRet;

}

void MONSTER::CalcPushRect()
{
    int iLatency = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iLatency = 0;

    for( unsigned int i = 0; i < (int)m_vecPushRect.size(); i++ )
    {
        float fHeight;
        float fWidth;
        if( m_bIsFallDownMotion )
        {
            fHeight = m_fWidth;
            fWidth = m_fHeight;
        }
        else
        {
            fHeight = m_fHeight;
            fWidth = m_fWidth;
        }


        if( m_vecPushRect[i].iBoneId == -1 )
        {
            if( GetDefaultMonster( m_iMonsterType ) == MON_KING_CRAB )
            {

                D3DXVECTOR3 vecBase;
                D3DXMATRIX matBone;                
                D3DXMatrixIdentity( &matBone );
                
                const D3DXMATRIX* pWorld;
                pWorld = m_pObject->GetWorldMat();

                if ( NULL != m_pObject->GetAngleBone( 12, &matBone ) ) 
                {
                    matBone = matBone * *pWorld;
                    SETVECTOR3( vecBase , matBone._41, matBone._42, 1.0f );

                    m_vecPushRect[i].SetRect( vecBase.x - ( fWidth * 0.5f ), vecBase.y + ( fHeight * 0.5f )
                        , vecBase.x + ( fWidth * 0.5f ) ,vecBase.y - ( fHeight * 0.5f ));
                }
                else
                {
                    m_vecPushRect[i].m_Top = m_afY[iLatency] - 0.25f + fHeight;
                    m_vecPushRect[i].m_Bottom = m_afY[iLatency] - 0.25f;

                    m_vecPushRect[i].m_Left = m_afX[iLatency] - 1.0f - ( fWidth * 0.5f );
                    m_vecPushRect[i].m_Right = m_afX[iLatency] - 1.0f + ( fWidth * 0.5f );
                }

            }
            else
            {
                m_vecPushRect[i].m_Top = m_afY[iLatency] - 0.25f + fHeight;
                m_vecPushRect[i].m_Bottom = m_afY[iLatency] - 0.25f;

                m_vecPushRect[i].m_Left = m_afX[iLatency] - 1.0f - ( fWidth * 0.5f );
                m_vecPushRect[i].m_Right = m_afX[iLatency] - 1.0f + ( fWidth * 0.5f );
                
            }
        }
        else
        {
            D3DXVECTOR3 vecBase;
            D3DXMATRIX matBone;
            D3DXMatrixIdentity( &matBone );
            
            const D3DXMATRIX* pWorld;
            pWorld = m_pObject->GetWorldMat();

            if ( NULL != m_pObject->GetAngleBone( m_vecPushRect[i].iBoneId, &matBone ) ) 
            {
                matBone = matBone * *pWorld;

                SETVECTOR3( vecBase , matBone._41 + m_vecPushRect[i].fOffsetX, 
                                      matBone._42 + m_vecPushRect[i].fOffsetY + 0.25f, 1.0f );

                m_vecPushRect[i].SetRect( vecBase.x - ( m_vecPushRect[i].fWidth * 0.5f ), vecBase.y + ( m_vecPushRect[i].fHeight * 0.5f )
                    , vecBase.x + ( m_vecPushRect[i].fWidth * 0.5f ) ,vecBase.y - ( m_vecPushRect[i].fHeight * 0.5f ));
            }
            else
            {
                m_vecPushRect[i].m_Top = m_afY[iLatency] - 0.25f + fHeight;
                m_vecPushRect[i].m_Bottom = m_afY[iLatency] - 0.25f;

                m_vecPushRect[i].m_Left = m_afX[iLatency] - 1.0f - ( fWidth * 0.5f );
                m_vecPushRect[i].m_Right = m_afX[iLatency] - 1.0f + ( fWidth * 0.5f );
            }
        }
    }
    
}

void MONSTER::CalcAttackRect()
{

    int iLatency = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iLatency = 0;

    for( unsigned int i = 0; i < (int)m_vecAttackRect.size(); i++ )
    {
        float fHeight;
        float fWidth;
        if( m_bIsFallDownMotion )
        {
            fHeight = m_fAttackBoundBoxWidth;
            fWidth = m_fAttackBoundBoxHeight;
        }
        else
        {
            fHeight = m_fAttackBoundBoxHeight;
            fWidth = m_fAttackBoundBoxWidth;
        }

        if( m_vecAttackRect[i].iBoneId == -1 )
        {
            if( GetDefaultMonster( m_iMonsterType ) == MON_KING_CRAB )
            {

                D3DXVECTOR3 vecBase;
                D3DXMATRIX matBone;
                D3DXMatrixIdentity( &matBone );
                
                const D3DXMATRIX* pWorld;
                pWorld = m_pObject->GetWorldMat();

                if ( NULL != m_pObject->GetAngleBone( 12, &matBone ) ) 
                {
                    matBone = matBone * *pWorld;
                    SETVECTOR3( vecBase , matBone._41, matBone._42, 1.0f );

                    m_vecAttackRect[i].SetRect( vecBase.x - ( fWidth * 0.5f ), vecBase.y + ( fHeight * 0.5f )
                        , vecBase.x + ( fWidth * 0.5f ) ,vecBase.y - ( fHeight * 0.5f ));
                }
                else
                {
                    m_vecAttackRect[i].m_Top = m_afY[iLatency] - 0.25f + fHeight;
                    m_vecAttackRect[i].m_Bottom = m_afY[iLatency] - 0.25f;

                    m_vecAttackRect[i].m_Left = m_afX[iLatency] - 1.0f - ( fWidth * 0.5f );
                    m_vecAttackRect[i].m_Right = m_afX[iLatency] - 1.0f + ( fWidth * 0.5f );
                }

            }
            else if( GetDefaultMonster( m_iMonsterType ) == MON_BASILISK )
            {
                m_vecAttackRect[i].m_Top -= 0.18f;
                m_vecAttackRect[i].m_Bottom -= 0.18f;

                m_vecAttackRect[i].m_Left -= 0.5f;
                m_vecAttackRect[i].m_Right -= 0.5f;

                // ¹Ù½Ç ¸Â¾Æ¼­ ¾²·¯Áø ¸ð¼Ç
                std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
                std::map< std::string, KGCMonsterAction >::iterator mit = mapAction.find( "DOWN" );
                if( mit != mapAction.end() )
                {
                    const KGCMonsterAction &kMonsterAction = mit->second;
                    if( kMonsterAction.m_iMotionIndex == m_aiMotion[iLatency] )
                    {
                        m_vecAttackRect[i].m_Top -= 0.5f;
                        m_vecAttackRect[i].m_Bottom -= 0.5f;
                    }
                }

                // ¹Ù½Ç Àâ¾Æ¸Ô±â °ø°Ý
                mit = mapAction.find( "ATTACK04" );
                if( mit != mapAction.end() )
                {
                    const KGCMonsterAction &kMonsterAction = mit->second;
                    if( kMonsterAction.m_iMotionIndex == m_aiMotion[iLatency] )
                    {
                        m_vecAttackRect[i].m_Top -= 0.3f;
                        m_vecAttackRect[i].m_Bottom -= 0.3f;

                        m_vecAttackRect[i].m_Left += 0.12f;
                        m_vecAttackRect[i].m_Right += 0.12f;
                    }
                }
            }
            else
            {
                m_vecAttackRect[i].m_Top = m_afY[iLatency] - 0.25f + fHeight;
                m_vecAttackRect[i].m_Bottom = m_afY[iLatency] - 0.25f;

                m_vecAttackRect[i].m_Left = m_afX[iLatency] - 1.0f - ( fWidth * 0.5f );
                m_vecAttackRect[i].m_Right = m_afX[iLatency] - 1.0f + ( fWidth * 0.5f );
            }
        }
        else
        {
            D3DXVECTOR3 vecBase;
            D3DXMATRIX matBone;
            D3DXMatrixIdentity( &matBone );

            const D3DXMATRIX* pWorld;
            pWorld = m_pObject->GetWorldMat();

            if ( NULL != m_pObject->GetAngleBone( m_vecAttackRect[i].iBoneId, &matBone ) ) 
            {
                matBone = matBone * *pWorld;
                SETVECTOR3( vecBase , matBone._41, matBone._42 + 0.25f, 1.0f );

                m_vecAttackRect[i].SetRect( vecBase.x - ( m_vecAttackRect[i].fWidth * 0.5f ) + m_vecAttackRect[i].OffsetX, vecBase.y + ( m_vecAttackRect[i].fHeight * 0.5f ) + m_vecAttackRect[i].OffsetY
                    , vecBase.x + ( m_vecAttackRect[i].fWidth * 0.5f ) + m_vecAttackRect[i].OffsetX ,vecBase.y - ( m_vecAttackRect[i].fHeight * 0.5f ) + m_vecAttackRect[i].OffsetY );
            }
            else
            {
                m_vecAttackRect[i].m_Top = m_afY[iLatency] - 0.25f + fHeight;
                m_vecAttackRect[i].m_Bottom = m_afY[iLatency] - 0.25f;

                m_vecAttackRect[i].m_Left = m_afX[iLatency] - 1.0f - ( fWidth * 0.5f );
                m_vecAttackRect[i].m_Right = m_afX[iLatency] - 1.0f + ( fWidth * 0.5f );
            }
        }
    }

}

void MONSTER::CalcFootHold()
{
	if( m_pObject == NULL )
		return;

    for( unsigned int i = 0 ; i < (int)m_vecFootHold.size(); i++ )
    {
        // ÀÌÀü²« Áö¿ì°í
        for( unsigned int j = 0; j < (int)m_vecFootHold[i].m_vecFootHoldPos.size() ; j++ )
        {
            SiKGCFantasticMap()->RemoveDungeonFootHold( m_vecFootHold[i].m_vecFootHoldPos[j].x, 
                                                        m_vecFootHold[i].m_vecFootHoldPos[j].y, (void*) this );            
        }
        m_vecFootHold[i].m_vecFootHoldPos.clear();

        if( m_bDie )
            continue;

        D3DXVECTOR2 vecBase, vecEnd;

        //µÎ Á¡À» Ã£ÀÚ
        {
			m_pObject->SetMotionMatrix( true, NULL , false , true );
            D3DXMATRIX matBone;
            D3DXMATRIX matTrans;

            const D3DXMATRIX* pWorld;
            pWorld = m_pObject->GetWorldMat();

            D3DXMatrixTranslation( &matTrans, m_vecFootHold[i].m_fRelativeX, m_vecFootHold[i].m_fRelativeY, 0.0f );        

            matTrans = *pWorld * matTrans;

            m_pObject->GetAngleBone( m_vecFootHold[i].m_iBaseBone, &matBone );
            matBone = matBone * matTrans;

            SETVECTOR2( vecBase, matBone._41, matBone._42 );

            m_pObject->GetAngleBone( m_vecFootHold[i].m_iEndBone, &matBone );
            matBone = matBone * matTrans;

            SETVECTOR2( vecEnd, matBone._41, matBone._42 );


        }

        //¶óÀÎÀ» ±ßÀÚ. vecBase
        {
            float fDistanceX,fDistanceY;
            float fStepX, fStepY;
            
            fStepX = fStepY = 0.05f;

            int iLocateAdjustVal = (int)(vecBase.x / fStepX);
            vecBase.x = iLocateAdjustVal * fStepX;
            iLocateAdjustVal = (int)(vecBase.y / fStepY);
            vecBase.y = iLocateAdjustVal * fStepY;

            fDistanceX = vecEnd.x - vecBase.x;
            fDistanceY = vecEnd.y - vecBase.y;

            if( fDistanceX == 0.0f && fDistanceY == 0.0f )
                continue;

            if( fDistanceX < 0.0f )
            {
                fDistanceX = -fDistanceX;
                fStepX = -fStepX;
            }

            if( fDistanceY < 0.0f )
            {
                fDistanceY = -fDistanceY;
                fStepY = -fStepY;
            }

            SiKGCFantasticMap()->AddDungeonFootHoldByValue( vecBase.x, vecBase.y, m_vecFootHold[i].m_dwProp, (void*) this,"" );         
            m_vecFootHold[i].m_vecFootHoldPos.push_back( vecBase );

            if( fDistanceX > fDistanceY )  
            {  
              

                bool bEndChecker = vecBase.x < vecEnd.x;
                while( bEndChecker == ( vecBase.x < vecEnd.x ) )  
                {  
                    vecBase.x += fStepX;  
                    vecBase.y += ( fDistanceY / fDistanceX ) * fStepY;
                       
                    SiKGCFantasticMap()->AddDungeonFootHoldByValue( vecBase.x, vecBase.y, m_vecFootHold[i].m_dwProp, (void*) this,"" );         
                    m_vecFootHold[i].m_vecFootHoldPos.push_back( vecBase );
                }  
            }  
            else  
            {  
                bool bEndChecker = vecBase.y < vecEnd.y;
                while( bEndChecker == ( vecBase.y < vecEnd.y ) )  
                {                 
                    vecBase.y += fStepY;              
                    vecBase.x += ( fDistanceX / fDistanceY ) * fStepX;
    
                    SiKGCFantasticMap()->AddDungeonFootHoldByValue( vecBase.x, vecBase.y, m_vecFootHold[i].m_dwProp, (void*) this,"" );         
                    m_vecFootHold[i].m_vecFootHoldPos.push_back( vecBase );
                }  
            }     
        }
    }
}

D3DXVECTOR3 MONSTER::GetBonePos( int iBoneID )
{
    D3DXMATRIX matBone;
    const D3DXMATRIX* pWorld;
    D3DXVECTOR3 vecBase;

    pWorld = m_pObject->GetWorldMat();

    m_pObject->GetAngleBone( iBoneID, &matBone );
    matBone = matBone * *pWorld;

    SETVECTOR3( vecBase, matBone._41 + 1.0f, matBone._42 + 0.5f, 0 );
    return vecBase;
}

void MONSTER::SetPushCheck( bool bCheck, int iIndex )
{
    m_vecPushRect[iIndex].bActive = bCheck;
}

bool MONSTER::GetPushCheck( int iIndex )
{
    return m_vecPushRect[iIndex].bActive;
}

void MONSTER::SetTitleText( const int iStringId_ )
{
	g_pkUIScene->m_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( iStringId_ ) );
}

float MONSTER::GetStageX( void )
{
	if( SiKGCRoomManager()->IsMonsterGameMode() )
	{
		return SiKGCFantasticMap()->GetPixelWidth();
	}
	else
	{
		return 1.5f * (float)g_MyD3D->MyStg->Num_Width;
	}
}

float MONSTER::GetStageY( void )
{
	if( SiKGCRoomManager()->IsMonsterGameMode() )
	{
		return SiKGCFantasticMap()->GetPixelHeight();
	}
	else
	{
		return 1.5f * (float)g_MyD3D->MyStg->Num_Height;
	}
}

void MONSTER::HackingCheck()
{
	//¸ó½ºÅÍÀÇ hp º¯È­´Â È£½ºÆ®¸¸ ¾Ë¸éµÈ´Ù
	if(!g_kGlobalValue.m_kUserInfo.bHost)
		return;

	int iHP = (int)m_fHP;
	int iOldHP = (int)m_fOldHP;

	//g_kGlobalValue.m_bActiveApp Ã¼Å©ÀÌÀ¯´Â °ÔÀÓÀ©µµ¿ì°¡ È°¼ºÈ­ µÇÀÖÀ» ¶§¸¸ Ã¼Å©ÇÏ±â À§ÇØ¼­ÀÓ.
	//ÀÌ ÇØÅ·À» °¨ÁöÇÏ´Â »óÈ²Àº ÇØÅ·Åø·Î ¸ó½ºÅÍÀÇ HP¸¦ Á¶ÀÛÇÏ´Â ¼ø°£ÀÌ±â ¶§¹®¿¡ °ÔÀÓÀ©µµ¿ì´Â ºñÈ°¼ºÈ­ µÉ ¼ö ¹Û¿¡ ¾ø´Ù.
	if( g_MyD3D->m_pkQuestGameMgr->IsBossStage() )
	{
		//º¸½º½ºÅ×ÀÌÁö¿¡¼­´Â º¸½º¸¸ Ã¼Å©ÇÏ°Ú´Ù. º¸½º°¡ Á×À¸¸é ´Ù¸¥ ¸÷µéÀº µ¥¹ÌÁö ¾ø¾îµµ ±×³É Á×±â ¶§¹®ÀÌ´Ù.
		if( !m_bIsAttacked && (iHP != iOldHP) && (iHP < iOldHP) && m_iBossMonster == MONSTER::MON_BOSS )
		{
			g_MyD3D->m_pkQuestGameMgr->SetMonsterHPHack(true);			

			if( !g_kGlobalValue.m_bActiveApp )
				g_kGlobalValue.m_bUseCrack = TRUE;
#if !defined(ONLY_COLLECT_HACKING_DATA)
			g_pStateGame->OnExitGame();			
#endif
		}
	}
	else
	{
		if( !m_bIsAttacked && (iHP != iOldHP) && (iHP < iOldHP) )
		{
			g_MyD3D->m_pkQuestGameMgr->SetMonsterHPHack(true);			

			if( !g_kGlobalValue.m_bActiveApp )
				g_kGlobalValue.m_bUseCrack = TRUE;
#if !defined(ONLY_COLLECT_HACKING_DATA)
			g_pStateGame->OnExitGame();			
#endif
		}
	}			

	m_bIsAttacked = false;
}

void MONSTER::TraceMonster( void )
{
	if ( m_bTrace )
	{
		MONSTER* pMonster = g_kMonsterManager.GetMonster(m_iTraceindex);
		if( pMonster != NULL){
            int iIndex = 0;
            if ( g_kGlobalValue.m_kUserInfo.bHost )
                iIndex = LATENCY_INDEX;

			D3DXVECTOR2 vTargetPos;
			D3DXVECTOR2 vTemp;
			D3DXVECTOR2 vMyPos;

            if ( m_iTraceBone != -1 )
            {
                // ±× º»ÀÇ ·ÎÅ×ÀÌ¼Ç °ª±îÁö °¡Áö°í¿Í¼­ ÀÌµ¿!!
                D3DXMATRIX matTraceBone, matTrans;
                // µû¶ó ´Ù´Ò ¸ó½ºÅÍÀÇ ¿ùµå ¸ÞÆ®¸¯½º
                const D3DXMATRIX *matMasterWorld = pMonster->m_pObject->GetWorldMat();
                // µû¶ó ´Ù´Ò º»ÀÇ ¸ÞÆ®¸¯½º
                pMonster->m_pObject->GetAngleBone( m_iTraceBone, &matTraceBone );
                D3DXMatrixMultiply( &matTraceBone, &matTraceBone, matMasterWorld );
                union SHORT_TO_FLOAT
                {
                    short short_[2];
                    float float_;
                };
                SHORT_TO_FLOAT shot;
                shot.short_[0] = m_vecMotionList[ m_aiMotion[ iIndex ] ]->GetFrameData( m_aiFrame[ iIndex ] ).Reserved1;
                shot.short_[1] = m_vecMotionList[ m_aiMotion[ iIndex ] ]->GetFrameData( m_aiFrame[ iIndex ] ).Reserved2;

                float fTemp = 1.0f;
                if ( false == pMonster->m_abIsRight[iIndex] )
                {
                    fTemp *= -1.0f;
                }
                D3DXMatrixTranslation( &matTrans, m_fTraceGapX, m_fTraceGapY * fTemp, shot.float_ * fTemp );
                D3DXMatrixMultiply( &matTraceBone, &matTraceBone, &matTrans );
                m_pObject->SetWorldMat( &matTraceBone );

                vTargetPos.x = matTraceBone._41 + m_fTraceGapX + 1.0f;
                vTargetPos.y = matTraceBone._42 + 0.51f;
            }
            else
            {
                if( pMonster->GetIsRight() )
                    vTargetPos.x = pMonster->m_afX[iIndex] - m_fTraceGapX;
                else
                    vTargetPos.x = pMonster->m_afX[iIndex] + m_fTraceGapX;
                vTargetPos.y = pMonster->m_afY[iIndex] + m_fTraceGapY;
            }

			vMyPos.x = this->m_afX[iIndex];
			vMyPos.y = this->m_afY[iIndex];

			vTemp = vTargetPos - vMyPos;
			float fLength = D3DXVec2Length( &vTemp );

            vMyPos = vTargetPos * m_fPercentLength + vMyPos * (1.0f - m_fPercentLength);

			SetLatencyX( iIndex, vMyPos.x );
			SetLatencyY( iIndex, vMyPos.y );
			m_abIsRight[iIndex] = pMonster->m_abIsRight[iIndex];
            m_aiFrame[iIndex] = pMonster->m_aiFrame[iIndex];
		}
	}
}

void MONSTER::SetMissionMonster()
{
    m_MissionIcon.AddTexture( g_pGCDeviceManager->CreateTexture( "missionicon.dds" ) );
}

bool MONSTER::IsHideName( void )
{
    switch ( GetDefaultMonster( m_iMonsterType ) )
    {
    case MON_AMY:
    case MON_MOONEVENT:
    case MON_GIANT_BACKGROUND:
    case MON_PATUSEI_HEAD:
    case MON_PATUSEI_BODY:
    case MON_IBLIS_KNIGHT:
    case MON_HATSHEPSUT_COCOON:
    case MON_PRISON_FRONT:
    case MON_PRISON_BACK:
    case MON_TALIN_POOLGUARD:
    case MON_TALIN_DSR:
    case MON_MIRAGE_POT:
    case MON_PATUPLAT_GOLEM:
	case MON_JIN:
	case MON_RONAN:
	case MON_RYAN:
	case MON_MARI:
    case MON_BOSS_OBJECT:
    case MON_EVENT_MIRROR1:
    case MON_EVENT_MIRROR2:
    case MON_EVENT_MIRROR3:
    case MON_EVENT_MIRROR4:
    case MON_ANGRYBOSS_HATSHEPSUT_COCOON:
        return true;
    }
	if(g_MyD3D->m_KGCOption.GetQualityCount() <= 2 ) return true;
    return false;
}

void MONSTER::SetHitPos( D3DXVECTOR2 vecPos_ )
{
    m_vecHitPos = vecPos_;
}

void MONSTER::SetBodyAngle( float fBodyAngle_ )
{
    Body_Angle = fBodyAngle_;
}

float MONSTER::GetBodyAngle()
{
    return Body_Angle;
}

void MONSTER::SetUserBoneAngle( int iBoneIndex, float fAngleX, float fAngleY, float fAngleZ )
{
    m_mapUserBoneAngle[iBoneIndex] = D3DXVECTOR3( fAngleX, fAngleY, fAngleZ );
}

D3DXVECTOR3 MONSTER::GetUserBoneAngle( int iBoneIndex )
{
    std::map<int, D3DXVECTOR3>::iterator mapIter = m_mapUserBoneAngle.find( iBoneIndex );
    if( mapIter != m_mapUserBoneAngle.end() )
        return mapIter->second;
    return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// ÇÃ·¹ÀÌ¾îÀÇ ¹öÇÁ¸¦ Áö¿î´Ù.
void MONSTER::ClearMagicPlayerEffect( int iPlayerIndex_, int iBuffIndex_ )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return;

    if( false == g_MyD3D->IsPlayerIndex( iPlayerIndex_ ) )
    {
        for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        {
            if( false == g_MyD3D->MyPlayer[iLoop]->IsLive() )
                continue;

            if( g_MyD3D->MyPlayer[iLoop]->IsMagicEffect( iBuffIndex_ ) )
            {
                LTP_BUFF buff;
                buff.iBuffIndex = iBuffIndex_;
                buff.iTargetIndex = iLoop;
                buff.bClearBuff = true;
                g_LatencyTransfer.PushPacket( &buff, 1 );
            }
        }
    }
    else
    {
        if( g_MyD3D->MyPlayer[iPlayerIndex_]->IsMagicEffect( iBuffIndex_ ) )
        {
            LTP_BUFF buff;
            buff.iBuffIndex = iBuffIndex_;
            buff.iTargetIndex = iPlayerIndex_;
            buff.bClearBuff = true;
            g_LatencyTransfer.PushPacket( &buff, 1 );
        }
    }
}

// ÇÃ·¹ÀÌ¾î¿¡°Ô ¹öÇÁ¸¦ °Ç´Ù.
void MONSTER::SetMagicPlayerEffect( int iPlayerIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_ )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return;

    if( false == g_MyD3D->IsPlayerIndex( iPlayerIndex_ ) )
    {
        for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        {
            if( false == g_MyD3D->MyPlayer[iLoop]->IsLive() )
                continue;

            LTP_BUFF buff;
            buff.iBuffIndex = iBuffIndex_;
            buff.iTargetIndex = iLoop;
            buff.fBuffTime = fBuffTime_;
            buff.iBuffLv = iBuffLevel_ + 1;
            buff.bClearBuff = false;
            buff.iWho = m_iMonsterIndex + MAX_PLAYER_NUM;
            g_LatencyTransfer.PushPacket( &buff, 1 );
        }
    }
    else
    {
        LTP_BUFF buff;
        buff.iBuffIndex = iBuffIndex_;
        buff.iTargetIndex = iPlayerIndex_;
        buff.fBuffTime = fBuffTime_;
        buff.iBuffLv = iBuffLevel_ + 1;
        buff.bClearBuff = false;
        buff.iWho = m_iMonsterIndex + MAX_PLAYER_NUM;
        g_LatencyTransfer.PushPacket( &buff, 1 );
    }
}

// ¹üÀ§¾È¿¡ ÀÖ´Â ÇÃ·¹ÀÌ¾î¿¡°Ô ¹öÇÁ¸¦ °Ç´Ù.
void MONSTER::SetMagicPlayerEffectInRange( int iBuffIndex_, float fBuffTime_, int iLeft, int iTop, int iRight, int iBottom )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return;

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    for( int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

        if( false == g_MyD3D->MyPlayer[iLoop]->IsLive() )
            continue;

        if( pPlayer->IsMagicEffect( iBuffIndex_ ) )
            continue;

        if( pPlayer->vPos.x > iX1 && 
            pPlayer->vPos.x < iX2 &&
            pPlayer->vPos.y > iY2 && 
            pPlayer->vPos.y < iY1 )
        {
            LTP_BUFF buff;
            buff.iBuffIndex = iBuffIndex_;
            buff.iTargetIndex = iLoop;
            buff.fBuffTime = fBuffTime_;
            buff.iBuffLv = 1;
            buff.bClearBuff = false;
            buff.iWho = m_iMonsterIndex + MAX_PLAYER_NUM;
            g_LatencyTransfer.PushPacket( &buff, 1 );
        }
    }
}

void MONSTER::SetMagicEffect( int iPlayerIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_, int iIndex_, bool bIsCrashPlayer, bool bAccTime_ )
{
    if( -1 == iIndex_ )
        iIndex_ = m_iMonsterIndex + MAX_PLAYER_NUM;
    g_pMagicEffect->SetMagicEffect( iPlayerIndex_, iBuffIndex_, fBuffTime_, iBuffLevel_, iIndex_, bIsCrashPlayer, bAccTime_ );
}

void MONSTER::SetMagicMonsterALLEffect( int iBuffIndex_, float fBuffTime_, int iBuffLevel_, int iTeam, int iIndex_, bool bIsCrashPlayer, bool bAccTime_ )
{
    if( -1 == iIndex_ )
        iIndex_ = m_iMonsterIndex + MAX_PLAYER_NUM;

    MAP_INT_PMONSTER::iterator mapIter = g_kMonsterManager.m_mapMonster.begin();
    for(; mapIter != g_kMonsterManager.m_mapMonster.end(); ++mapIter)
    {
        if( false == mapIter->second->IsLive() )
            continue;
        
        if( iTeam != mapIter->second->m_iTeam )
            continue;

        if( mapIter->second->GetMonsterType() == MON_GCORIGIN_BERD_ORBE )
            continue;

        g_pMagicEffect->SetMagicEffect( mapIter->first, iBuffIndex_, fBuffTime_, iBuffLevel_, iIndex_, bIsCrashPlayer, bAccTime_ );
    }
}

void MONSTER::ClearMagicMonsterALLEffect( int iBuffIndex_, int iTeam )
{
    MAP_INT_PMONSTER::iterator mapIter = g_kMonsterManager.m_mapMonster.begin();
    for(; mapIter != g_kMonsterManager.m_mapMonster.end(); ++mapIter)
    {
        if( false == mapIter->second->IsLive() )
            continue;

        if( iTeam != mapIter->second->m_iTeam )
            continue;

        if( IsMagicEffect( mapIter->first, iBuffIndex_ ) )
            g_pMagicEffect->ClearMagicEffect( mapIter->first + MAX_PLAYER_NUM, iBuffIndex_, false, false, false );
    }
}

int MONSTER::GetBuffLevel( int iPlayerIndex_, int iBuffIndex_ )
{
    return g_pMagicEffect->GetBuffLevel( iPlayerIndex_ + MAX_PLAYER_NUM, iBuffIndex_ );
}

bool MONSTER::IsMagicEffect( int iPlayerIndex_, int iBuffIndex_ )
{
    return g_pMagicEffect->IsMagicEffect( iPlayerIndex_ + MAX_PLAYER_NUM, iBuffIndex_ ) != 0;
}

void MONSTER::ClearMagicEffect( int iPlayerIndex_, int iBuffIndex_ )
{
    if( IsMagicEffect( iPlayerIndex_, iBuffIndex_ ) )
    {
        g_pMagicEffect->ClearMagicEffect( iPlayerIndex_ + MAX_PLAYER_NUM, iBuffIndex_, false, false, false );
    }
}
void MONSTER::ClearMagicPlayerAllEffect( int iBuffIndex_ )
{
    for( int i = 0; i < MAX_PLAYER_NUM; i++ ) {
        ClearMagicEffect( i, iBuffIndex_ );
    }
}


void MONSTER::ClearMagicEffect_AllMonster( int iBuffIndex_ )
{
    std::map<int, MONSTER*>::iterator mapIter = g_kMonsterManager.m_mapMonster.begin();
    for(; mapIter != g_kMonsterManager.m_mapMonster.end() ; ++mapIter ) 
    {
        g_pMagicEffect->ClearMagicEffect( mapIter->first + MAX_PLAYER_NUM, iBuffIndex_, false, false, false );
    }
}

void MONSTER::ClearALLMagicEffect()
{
    g_pMagicEffect->ClearMonsterAllEffect( m_iMonsterIndex );
}

void MONSTER::SetMagicEffectNoLocal( int iMonsterIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_, int iWhoIndex_ )
{
    LTP_BUFF buff;
    buff.iBuffIndex = iBuffIndex_;
    buff.iTargetIndex = iMonsterIndex_ + MAX_PLAYER_NUM;
    buff.fBuffTime = fBuffTime_;
    buff.iBuffLv = iBuffLevel_+ 1;
    buff.bClearBuff = false;
    buff.iWho = iWhoIndex_;
    g_LatencyTransfer.PushPacket( &buff, 1 );
}

void MONSTER::ClearMagicEffectNoLocal( int iMonsterIndex_, int iBuffIndex_ )
{
    LTP_BUFF buff;
    buff.iBuffIndex = iBuffIndex_;
    buff.iTargetIndex = iMonsterIndex_ + MAX_PLAYER_NUM;
    buff.bClearBuff = true;
    g_LatencyTransfer.PushPacket( &buff, 1 );
}


void MONSTER::SetMagicEffect_NotMeBroad( int iMonsterIndex_, int iBuffIndex_, float fBuffTime_, int iBuffLevel_, int iWho )
{
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        LTP_BUFF buff;
        buff.iBuffIndex = iBuffIndex_;
        buff.iTargetIndex = iMonsterIndex_ + MAX_PLAYER_NUM;
        buff.fBuffTime = fBuffTime_;
        buff.iBuffLv = iBuffLevel_+ 1;
        buff.bClearBuff = false;
        buff.iWho = iWho;
        g_LatencyTransfer.PushPacket( &buff, 0 );

        SetMagicEffect( iMonsterIndex_, iBuffIndex_, fBuffTime_, iBuffLevel_, iWho, false );
    }
}

void MONSTER::ClearMagicEffect_NotMeBroad( int iMonsterIndex_, int iBuffIndex_ )
{
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        LTP_BUFF buff;
        buff.iBuffIndex = iBuffIndex_;
        buff.iTargetIndex = iMonsterIndex_ + MAX_PLAYER_NUM;
        buff.bClearBuff = true;
        g_LatencyTransfer.PushPacket( &buff, 0 );

        ClearMagicEffect( iMonsterIndex_, iBuffIndex_ );
    }
}

float MONSTER::GetMagicEffectTime( int iPlayerIndex_, int iBuffIndex_ )
{
    float fBuffTime = g_pMagicEffect->IsMagicEffectTimeRet( iPlayerIndex_ + MAX_PLAYER_NUM, iBuffIndex_ );
    return fBuffTime;
}

void MONSTER::AddDamageToAlivePlayer( int iDamage, float fStartX, float fStartY, bool bUseMonPos_ /*= false*/ )
{
    // »ì¾Æ ÀÖ´Â ¸ðµç ÇÃ·¹ÀÌ¾î¿¡°Ô µ¥¹ÌÁö ¹ß»ç!!!
    if ( SiKGCRoomManager()->IsMonsterGameMode() )
    {
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_cLife > 0 )
            {
                CDamageInstance* damage = NULL;

                D3DXVECTOR3 vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                if ( !bUseMonPos_ )
                {
                    if( i == g_MyD3D->Get_MyPlayer() ) // Å¸°ÙÀÌ ÀÚ±â ÀÚ½ÅÀÌ¸é..
                        vecPos = g_kLocalPlayerLatency->kRenderData[0].vPos;
                    else
                        vecPos = g_MyD3D->MyPlayer[i]->vPos;
                }
                // ³ªÀÇ ÁÂÇ¥¿¡¼­ ¹ß»çÇÒ ¼ö µµ ÀÖÁö ¸Ó.. À¯µµÅºÀÌ ±×·¸´Ù ¤Ð 
                else
                {
                    vecPos = D3DXVECTOR3( m_afX[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0 ], m_afY[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0 ], 0.5f );
                }

                float fDirection = m_abIsRight[ LATENCY_INDEX ] ? 1.0f : -1.0f;
                float fPosX = fStartX * fDirection + vecPos.x;
                float fPosY = fStartY + vecPos.y;

                damage = g_MyD3D->m_pDamageManager->MonsterAddWithLocate( iDamage , m_iMonsterIndex, fPosX, fPosY );
                damage->m_HitWho = i;
            }
        }
    }
}

void MONSTER::ShowExpEffect( int iPlayerIndex_, float fExp_ )
{
    PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex_];
    CDamageInstance* pkDamage = g_MyD3D->m_pDamageManager->Add(DT_EXP_EFFECT, iPlayerIndex_, false , -1 , NULL , true);

    if( pkDamage == NULL )
        return;

    float fOffset_X = 0.0f;
    float fOffset_Y = 0.0f;
    float fPlayerPos_X = g_kLocalPlayerLatency->kRenderData[0].vPos.x + 0.15f;
    float fPlayerPos_Y = g_kLocalPlayerLatency->kRenderData[0].vPos.y;
    float fMonsterPos_X = m_vecHitPos.x == 0.0f ? m_afX[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0 ] : m_vecHitPos.x;
    float fMonsterPos_Y = m_vecHitPos.x == 0.0f ? m_afY[ g_kGlobalValue.m_kUserInfo.bHost ? LATENCY_INDEX : 0 ] : m_vecHitPos.y;

    D3DXVECTOR2 vecPos = g_pkGameOverlayUI->GetHeadUp()->m_pkExpBar->m_pkExpPercent->GetFixedWindowPos();
    D3DXVECTOR3& CameraPos = D3DXVECTOR3( g_kCamera.m_fLookX + 1.0f, g_kCamera.m_fLookY + 0.75f, 0.0f );

    float fWidth = g_kCamera.m_rtCamera.right - g_kCamera.m_rtCamera.left;
    float fHeight = g_kCamera.m_rtCamera.top - g_kCamera.m_rtCamera.bottom;
    float cwx = CameraPos.x - fWidth * 0.5f;
    float cwy = CameraPos.y - fHeight * 0.5f;

    float fRealPosX = cwx + (vecPos.x / 400.0f);
    float fRealPosY = cwy + (1.5f - vecPos.y / 400.0f);

    //m_iLastAttackedMe
    pkDamage->m_pOriginalDamage->SetMileStonePos( fMonsterPos_X, fMonsterPos_Y );
    pkDamage->m_pOriginalDamage->SetMileStonePos( fMonsterPos_X - FloatRand() / 15.0f, fMonsterPos_Y + FloatRand() / 15.0f );
    pkDamage->m_pOriginalDamage->SetMileStonePos( fMonsterPos_X , fMonsterPos_Y - FloatRand() / 15.0f );
    pkDamage->m_pOriginalDamage->SetMileStonePos( fRealPosX, fRealPosY );
    pkDamage->m_pOriginalDamage->SetMileStonePos( fRealPosX, fRealPosY );

    pkDamage->m_pOriginalDamage->m_fDamageToPlayer = fExp_;
}

void MONSTER::UpdateHeadPos()
{
	//º¸½º ¸ó½ºÅÍ°¡ ¾Æ´Ï¸é ±»ÀÌ ¿¬»êÇÒ ÇÊ¿ä°¡ ¾øÀ½.
	if (m_iBossMonster != MONSTER::MON_BOSS)
		return;

	//¸ó½ºÅÍ ÅÛÇÃ¸´¿¡¼­ÀÇ ³ôÀÌ °ªÀÌ ¿Ø¸¸ÇÏ¸é ¸Ó¸® À§Ä¡¿¡ ÀûÁßÇÏÁö¸¸ ´ëÇü ¸ó½ºÅÍÀÇ °æ¿ì ¸ÂÁö¸¦ ¾Ê´Â´Ù.
	// ±×·²°æ¿ì ¿¡ m_fHeadPosY °ªÀ» È°¿ëÇÑ´Ù. (m_fHeadPosY °ªÀº µðÆúÆ® °ªÀÌ -1)
	
	m_vHeadPos = vRenderPos;
	if (m_fHeadPosY < 0.0f)
		m_vHeadPos.y += m_fHeight*m_fScale;
	else
		m_vHeadPos.y += m_fHeadPosY;

	if (m_fHeadPosX != 0.0f)
		m_vHeadPos.x += m_fHeadPosX;
}

void MONSTER::TraceIconProcess()
{
    // ºñ¾úÀ¸¸é µ¹¸±ÇÊ¿ä ¾ø´Ù.
    if( TraceIcon.empty() ) {
        return;
    }

#if defined( ANGELS_EGG_REFORM )
    if( GC_GM_ANGELS_EGG != SiKGCRoomManager()->GetGameMode())
#endif
    {
        TraceIconPos = vRenderPos;
        FRect rt = g_kCamera.m_rtCamera;	
        rt.bottom += g_kCamera.m_fLookTermY;
        rt.top = rt.bottom + ( rt.top - rt.bottom ) * 0.8f;
        rt.AdjustPtInRect( TraceIconPos.x, TraceIconPos.y, 0.09f );	 

        D3DXVECTOR2 vDIrection = TraceIconPos - vRenderPos;

        D3DXVec2Normalize( &vDIrection, &vDIrection );	

        float fAngle = acos( vDIrection.x ) ;

        if( vDIrection.y < 0.0f )
        {
            fAngle = ( D3DX_PI - fAngle ) + D3DX_PI;
            fAngle += D3DX_PI / 2.0f;
        }
        else
        {
            fAngle += D3DX_PI / 2.0f;
        }

        bool bShow = true;


        // Ä«¸Þ¶ó ¾Þ±Û¾È¿¡ ¸ó½ºÅÍ°¡ ÇÑ¸¶¸®¶óµµ ÀÖÀ¸¸é ¾ÆÀÌÄÜ ¼ûÄÑ¶ó!!
        if( g_pkQuestManager->m_bExistMonsterInScreen )
            bShow = false;

        // ³»°¡ ¾Èº¸ÀÌ¸é ¹«Á¶°Ç ¾ÆÀÌÄÜ ¶ç¾ÆÁÖ¶ó!!
        if( m_bIsMyExistScreen )
        {
            if( IsInScreen() )
                bShow = false;
            else
                bShow = true;
        }

        // NPC¸ó½ºÅÍ¸é ¾ÆÀÌÄÜ ¾ø¾Ö¶ó!!
        if( m_eNPC > NPC_NONE )
            bShow = false;

        // Á×Àº³à¼®µµ ¾ø¾Ö¶ó!!
        if( GetHP() <= 0.0f || false == m_bLive )
            bShow = false;

        // InCheck¾ÈÇÑ ¸ó½ºÅÍµµ ¾ø¾Ö¶ó!!
        const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
        if( kMonsterTemplate.sTraceIcon.bIsNonCheckInScreen )
            bShow = false;

        for ( unsigned int i = 0 ; i < TraceIcon.size(); i++ )
        {
            if( TraceIcon[i] ) 
            { 
                TraceIcon[i]->SetShow( bShow );
                TraceIcon[i]->SetBeginPos( TraceIconPos );
                if( i == 1 )
                    TraceIcon[i]->SetAdditinalZRot( fAngle );
            }
        }
    }
}

bool MONSTER::IsInScreen()
{
	if ( !m_bLive )
	{
		return false;
	}

    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    if( kMonsterTemplate.sTraceIcon.bIsNonCheckInScreen )
        return false;

	FRect rt = g_kCamera.m_rtCamera;
	rt.top += g_kCamera.m_fLookTermY ;
	rt.bottom += g_kCamera.m_fLookTermY ;

	for (unsigned int i = 0; i < m_vecAttackRect.size(); ++i )
	{
		GCAttackRect<float> rect = m_vecAttackRect[i];
		
		if( rect.CheckCollision(rt) )
		{
			return true;
		}
	}
	return false;
}

void MONSTER::ShowAwakeMake(void)
{
    AddTraceParticleToBone("Goblin_Shock_01", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, -(m_fWidth / 2.0f), m_fHeight + 0.05f, true);
    AddTraceParticleToBone("Goblin_Shock_02", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, -(m_fWidth / 2.0f), m_fHeight + 0.05f, true);
}

//Sistema de Icones do Heróico
void MONSTER::SetChampionBuffIcon( int iMonsterIndex )
{
    EGCPlayerMagicEffect ProprietyName[] =
    {
        EGC_EFFECT_CHAMPION_ATK,
        EGC_EFFECT_CHAMPION_DEF,
        EGC_EFFECT_CHAMPION_HPREC,
        EGC_EFFECT_CHAMPION_ARMOR,
        EGC_EFFECT_CHAMPION_FS,
        EGC_EFFECT_CHAMPION_CURSE,
        EGC_EFFECT_CHAMPION_STONE,
        EGC_EFFECT_CHAMPION_ICE,
        EGC_EFFECT_CHAMPION_AD,
        EGC_EFFECT_CHAMPION_SW,
    };
    int bufferSize = 0;
    int iSize = sizeof( m_adwChampionPropertyList ) / sizeof( m_adwChampionPropertyList[0] );
    for ( int i = 0; i < iSize; ++i )
    {
        if ( m_dwChampionProperty & m_adwChampionPropertyList[i] )
        {
            if( !this->IsMagicEffect( iMonsterIndex, ProprietyName[i] + ( bufferSize * 10 ) ) )
            {
                this->SetMagicEffect( iMonsterIndex, ProprietyName[i] + ( bufferSize * 10 ), 999999, 1, -1, false );
                bufferSize++;
            }
        }
    }
}

void MONSTER::ShowMissAttack(float fHitPosX, float fHitPosY )
{
    float fOffsetX = fHitPosX - m_afX[LATENCY_INDEX];
    float fOffsetY = fHitPosY - m_afY[LATENCY_INDEX];
    AddTraceParticleToBone( "Miss_01", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, fOffsetX, fOffsetY, true );
    AddTraceParticleToBone( "Miss_02", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, fOffsetX, fOffsetY, true );
}

bool MONSTER::IsNoComboSkillCheckMonster()
{
    if( GetDefaultMonster( m_iMonsterType ) == MON_PUMPKIN ||
        GetDefaultMonster( m_iMonsterType ) == MON_STATUE ||
        GetDefaultMonster( m_iMonsterType ) == MON_DKMARK_II ||
        GetDefaultMonster( m_iMonsterType ) == MON_DECOY ||        
        GetDefaultMonster( m_iMonsterType ) == MON_STATUE ||        
        GetDefaultMonster( m_iMonsterType ) == MON_MAGMA_PARADOM_ARCADE ||        
        GetDefaultMonster( m_iMonsterType ) == MON_GIANT_BACKGROUND ||        
        GetDefaultMonster( m_iMonsterType ) == MON_POISONTHORN_FOR_BOSS ||        
        GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH1 ||        
        GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH2 ||        
        GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH3 ||        
        GetDefaultMonster( m_iMonsterType ) == MON_DIVINE_BRANCH4 ||        
        GetDefaultMonster( m_iMonsterType ) == MON_GARGOYLE_MEC ||
        GetDefaultMonster( m_iMonsterType ) == MON_IBLIS_KNIGHT )
        return true;
    else
        return false;
}


void MONSTER::ToggleExtraMesh( const char* strID, bool bToggle_ )
{
	return KGCObj::ToggleExtraMesh( strID, bToggle_ );
}

void MONSTER::ToggleExtraMeshAll( bool bToggle_ )
{
	return KGCObj::ToggleExtraMeshAll( bToggle_ );
}

void MONSTER::ToggleBaseMesh( bool bToggle_ )
{
	return KGCObj::ToggleBaseMesh( bToggle_ );
}

void MONSTER::SetChildMeshMotion( char* strChlid, char* strMotion, bool bReserveNext, bool bForce, bool bResetFrame/* = false*/, bool bNotOneTime/* = false*/, int iLoopMode/* = 0*/)
{
	return KGCObj::SetChildMeshMotion( strChlid, strMotion, bReserveNext, bForce, bResetFrame, bNotOneTime, iLoopMode );
}

void MONSTER::SetChildMeshMotion_Front( char* strChlid, char* strMotion )
{
    return KGCObj::SetChildMeshMotion_Front( strChlid, strMotion );
}

void MONSTER::ClearChildMeshOneTimeMotion( char* strChlid )
{
	return KGCObj::ClearChildMeshOneTimeMotion( strChlid );
}

CParticleEventSeqPTR MONSTER::AddParticleToChildMeshBone( char* strChlid, char* strSeqeunce, int iBoneIndex_, float fTraceTime /*= 0.5f*/, float offset_x /*= 0.0f*/, float offset_y /*= 0.0f*/, bool bNoDirection /*= false */ )
{
    CParticleEventSeqPTR pParticle = KGCObj::AddParticleToChildMeshBone( strChlid, strSeqeunce, iBoneIndex_, fTraceTime, offset_x, offset_y, bNoDirection );
    if( pParticle )
    {
        m_vecChildMeshParticle.push_back( pParticle );
    }

    return pParticle;
}

void MONSTER::AddParticleModel( char *strParticleName, float fX, float fY, int iBornID, float fHpMin, float fHpMax, bool bNoDirection)
{
    sParticleModel particleModel;
    particleModel.m_iBornID = iBornID;
    particleModel.m_fOffsetX = fX;
    particleModel.m_fOffsetY = fY;
    particleModel.m_fHpMinRadio = fHpMin;
    particleModel.m_fHpMaxRadio = fHpMax;
    particleModel.m_bNoDirection = bNoDirection;
    particleModel.strParticleName = strParticleName;
    m_vecParticleModel.push_back( particleModel );
}

void MONSTER::AddParticleBorn( char *strParticleName, float fOffsetX, float fOffsetY, int iBornID, bool bNoDirection)
{
    MSG_PARTICE_CREATE_INFO msg;
    msg.strSeqname = strParticleName;
    if ( bNoDirection == false )
        msg.fOffsetX = fOffsetX;
    else
        msg.vPos.x = fOffsetX;
    msg.vPos.y = fOffsetY;

    msg.fTraceTime = 0.5f;
    msg.bDirecitonal = !bNoDirection;
    msg.iBoneID = iBornID;
    msg.iLeftSideLayer = GC_LAYER_BLOCK;
    msg.iRightSideLayer = GC_LAYER_BLOCK;

    if ( msg.iBoneID == -1 )
        msg.pTracePos = &vRenderPos;

    CParticleEventSeqPTR pSequence = CreateParticle( msg );
    if( pSequence )
        pSequence->SetShow( true );
}

int MONSTER::GetTraceParticleNum()
{
    if ( m_pObject != NULL )
    {
        return m_pObject->m_vecPosDesc.size();
    }

    return 0;
}

int MONSTER::GetGameMode( void )
{
    return SiKGCRoomManager()->GetGameMode();
}

void MONSTER::ChangeMonster( KGC_PID_CHANGE_MONSTER::KChangeMonInfo kChangeMon_ )
{
    KGC_PID_CHANGE_MONSTER::KChangeMonInfo tmp = kChangeMon_;
    if ( kChangeMon_.usMonType == -1 )
    {
#if defined( ERROR_MESSAGE_BOX )
        {
            std::wstringstream strText;
            std::wstringstream strCaption;
            strText << L"보스게이트에서 잘못되었다.";
            strCaption << L"에러난 함수 위치 : " << __FUNCTION__;
            MessageBoxW( NULL, strText.str().c_str(), strCaption.str().c_str(), MB_OK );
        }
#endif
        return;
    }
    // ÇØÅ·¶§¹®¿¡ Æ¨±ä´Ù ¤Ð 
    this->SetAttackedMonster( true );
    this->SetLive( false );
    this->Wait_Room( tmp.usMonType, GetX(), GetY(), GetIsRight(), true, tmp.ucLv, tmp.dwChampionProperty, false );
    this->m_fAttackPoint = tmp.fAttackPoint;
    this->m_fHPPoint = tmp.fHPPoint;
    this->m_fHP = this->m_fOldHP = this->m_fHPPoint;
    this->m_iLevel = tmp.ucLv;
    this->m_nSpecial = tmp.usSpecial;
    this->SetTriggerID( tmp.usTriggerID );
    this->SetLive( true );
    this->ToggleRender( true );
    this->SetMovement( true );
}

void MONSTER::MonsterAction( int iMonsterActionIndex_ )
{
    if ( g_kGlobalValue.m_bMonsterAction && g_kGlobalValue.m_kUserInfo.bHost )
    {
        int iMaxIndex = (int)m_vecMotionList.size();
        int iIndex = iMonsterActionIndex_ % iMaxIndex;

        if ( iIndex < 0 )
            iIndex = iMaxIndex;

        int iFrame = 0;
        for( int i = SHOWLATENCY; i < LATENCY + SHOWLATENCY; ++i )
        {
            m_aiMotion[i] = iIndex;
            m_aiFrame[i] = iFrame;

            if (m_iSlowCount > 0)
            {
                --m_iSlowCount;
                continue;
            }
            else if (iFrame < GetLastFrame_(i))
            {
                ++iFrame;
            }
        }

        m_bFrameLock                = false;
        m_bIsSuperArmor             = false;
        m_bAttackSuperArmor         = false;
        m_bIsInvincible             = false;
    }
}

// º¸½º ¿ÀºêÁ§Æ®¿¡ ´ëÇÑ Åë°èÀÚ·á!
void MONSTER::SendBossObjectStat( OUT KBossDungeonStat& kDungeonStat )
{
    // ¹æÀå¸¸ ÇÏ¸é µÇÁö ¸Ó.. 
    if ( !g_kGlobalValue.m_kUserInfo.bHost )
        return;

    kDungeonStat.m_dwDungeonID = SiKGCRoomManager()->GetGameMode();
    kDungeonStat.m_dwDifficult = SiKGCRoomManager()->GetDungeonLevel();

    // º¸½º °ÔÀÌÆ® ±ú°í ¸÷ÀÌ ³ª¿Ô³×~ 
    if ( m_nSpecial == KEventMonster::OPT_GATE )
    {
        if ( m_bDie == true && m_fHP <= 0.0f )
        {
            kDungeonStat.m_dwClearType = 1;
        }
        else
        {
            kDungeonStat.m_dwClearType = 2;
        }
    }
    // º¸½º °ÔÀÌÆ® ÀÎÃ¼·Î ´ÙÀ½ ÆÇÀ¸·Î ³Ñ¾î°£´Ù¸é... ¾È±ü°Å´Ù..
    else if ( m_iMonsterType == MON_BOSS_OBJECT )
    {
        kDungeonStat.m_dwClearType = 0;
    }
}

void MONSTER::SendMonsterKill( void )
{
    if ( m_iSummoned != MEMS_NOT_SUMMON_MONSTER )
    {
        g_kGlobalValue.m_kUserMission.VirtualKillMonster( m_iMonsterType, m_iLevel, int(m_eNPC) );
    }
    if( -1 != m_iTriggerID )
    {
        g_pGameBoard->AddMonKillNum( m_iMonsterType, 1, m_iTriggerID );     // Á×Àº ¸ó½ºÅÍÀÇ Æ®¸®°Å ¹øÈ£¸¦ ±â·Ï.
        m_iTriggerID = -1;
    }
}


void MONSTER::ShieldProcess( void )
{
    if ( m_fShield > 0.0f )
    {
        for ( unsigned int i = 0; i < m_vecShieldEffectArg.size(); ++i )
        {
            if ( m_vecShieldEffectArg.size() > m_vecShieldEffect.size() )
            {
                CParticleEventSeqPTR tmp;
                tmp = g_ParticleManager->CreateSequence( m_vecShieldEffectArg[i].strParticleName, 0,0, 0.0f );

                if ( tmp == NULL )
                    continue;

                TRACE_INFO stTrace;
                stTrace.m_pvPos = &vRenderPos;
                stTrace.m_fXOffset = m_vecShieldEffectArg[i].fOffsetX;
                stTrace.m_fYOffset = m_vecShieldEffectArg[i].fOffsetY;
                stTrace.m_fParticleTraceTime = 1.0f;
                tmp->SetTrace( &stTrace );

                m_vecShieldEffect.push_back(tmp);
            }
        }
    }
    else
    {
        for ( unsigned int i = 0; i < m_vecShieldEffect.size(); ++i )
        {
            if ( m_vecShieldEffect[i] != NULL )
            {
                g_ParticleManager->DeleteSequence( m_vecShieldEffect[i] );
                m_vecShieldEffect[i] = NULL;
            }
        }

        m_vecShieldEffect.clear();
    }
}

void MONSTER::SetShieldEffect( char* strName, float fOffsetX /*= 0.0f*/, float fOffsetY /*= 0.0f */ )
{
    KParticleArg arg;

    arg.SetName( strName );
    arg.fOffsetX = fOffsetX;
    arg.fOffsetY = fOffsetY;

    std::vector< KParticleArg >::iterator vit = std::find_if( m_vecShieldEffectArg.begin(), m_vecShieldEffectArg.end(), boost::bind( &KParticleArg::strParticleName, _1 ) == strName );

    if( m_vecShieldEffectArg.end() == vit )
        m_vecShieldEffectArg.push_back( arg );
}

bool MONSTER::CheckSummonMotherDist()
{
    PLAYER *pPlayer;
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        pPlayer = g_MyD3D->MyPlayer[iLoop];
        if( pPlayer->IsLive() && (m_dwOwnerUID == pPlayer->m_kUserInfo.dwUID) )
        {
            if( pPlayer->GetSummonMonsterLimitDist() < DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] ) )
            {
                return true;
            }
        }
    }
    return false;
}

D3DXVECTOR2 MONSTER::TurnToMother(float fXLimit, float fYLimit)
{
    PLAYER *pPlayer;
    bool IsRight = false;
    D3DXVECTOR2 vecPos(0.0f, 0.0f);
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        pPlayer = g_MyD3D->MyPlayer[iLoop];
        if( pPlayer && (m_dwOwnerUID == pPlayer->m_kUserInfo.dwUID) )
        {
            // °Å¸®°¡ °¡±î¿ï¶§´Â ¹ÌÄ£µíÀÌ ¶°´Â ¹®Á¦°¡ »ý±ä´Ù.
            if( fXLimit < ASMfabs( pPlayer->vPos.x - m_afX[LATENCY_INDEX] ) ) 
            {
                m_abIsRight[ LATENCY_INDEX ] = (m_afX[ LATENCY_INDEX ] < pPlayer->vPos.x);
                vecPos.x = (m_afX[ LATENCY_INDEX ] > pPlayer->vPos.x) ? -1.0f : (m_afX[ LATENCY_INDEX ] < pPlayer->vPos.x) ? 1.0f : 0.0f;
            }
            if( fYLimit < ASMfabs( pPlayer->vPos.y - m_afY[LATENCY_INDEX] ) ) 
            {
                vecPos.y = (m_afY[ LATENCY_INDEX ] > pPlayer->vPos.y) ? -1.0f : (m_afY[ LATENCY_INDEX ] < pPlayer->vPos.y) ? 1.0f : 0.0f;
            }
            break;
        }
    }

    return vecPos;
}

D3DXVECTOR2 MONSTER::SameTurnToMother(float fXLimit, float fYLimit)
{
    PLAYER *pPlayer;
    bool IsRight = false;
    D3DXVECTOR2 vecPos(0.0f, 0.0f);
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        pPlayer = g_MyD3D->MyPlayer[iLoop];
        if( pPlayer && (m_dwOwnerUID == pPlayer->m_kUserInfo.dwUID) )
        {
            for(int iLatency = 0; iLatency<LATENCY_INDEX; ++iLatency)
            {
                m_abIsRight[ iLatency ] = pPlayer->GetIsRight();
            }

            if( fXLimit < ASMfabs( pPlayer->vPos.x - m_afX[LATENCY_INDEX] ) ) 
            {
                vecPos.x = (m_afX[ LATENCY_INDEX ] > pPlayer->vPos.x) ? -1.0f : (m_afX[ LATENCY_INDEX ] < pPlayer->vPos.x) ? 1.0f : 0.0f;
            }
            if( fYLimit < ASMfabs( pPlayer->vPos.y - m_afY[LATENCY_INDEX] ) ) 
            {
                vecPos.y = (m_afY[ LATENCY_INDEX ] > pPlayer->vPos.y) ? -1.0f : (m_afY[ LATENCY_INDEX ] < pPlayer->vPos.y) ? 1.0f : 0.0f;
            }
            break;
        }
    }

    return vecPos;
}

D3DXVECTOR2 MONSTER::GetMotherPos()
{
    D3DXVECTOR2 vReturnPos(m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ]);

    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if( g_MyD3D->MyPlayer[iLoop] && (m_dwOwnerUID == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.dwUID) )
        {
            vReturnPos.x = g_MyD3D->MyPlayer[iLoop]->vPos.x;
            vReturnPos.y = g_MyD3D->MyPlayer[iLoop]->vPos.y;
            break;
        }
    }

    return vReturnPos;
}

DWORD MONSTER::GetSummonPlayerUID()  
{
    return m_dwOwnerUID;  
}

void MONSTER::SetPlayerMonster()  
{
    // ¾Æ·¡ MAX_PLAYER ·çÇÁ¸¦ ¾Èµ¹°Ô ÇÏ·Á°í Ãß°¡ÇßÀ½.
    int iMyPlayer = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex(iMyPlayer) )
    {
        m_iTeam = g_MyD3D->MyPlayer[iMyPlayer]->m_kUserInfo.iTeam;
    }

    //for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    //{
    //    if( g_MyD3D->MyPlayer[iLoop]->IsLocalPlayer() )
    //    {
    //        m_iTeam = g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.iTeam;
    //        //m_dwOwnerUID = g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.dwUID;
    //        break;
    //    }
    //}
}

void MONSTER::SetDamageLife( int iDamageType, int iLife )
{
    g_MyD3D->m_pDamageManager->SetDamageLife( iDamageType, iLife );
}

BOOL MONSTER::IsDamageLife( DAMAGE_TYPE eDamageTypeiLife )
{
    return -1 < g_MyD3D->m_pDamageManager->GetDamageLife( eDamageTypeiLife );
}

bool MONSTER::IsMyDamageToType( DAMAGE_TYPE eDamageType )
{
    return g_MyD3D->m_pDamageManager->IsMonsterDamageToType( m_iMonsterIndex, eDamageType );
}

void MONSTER::DeleteAllMyDamageToType( DAMAGE_TYPE eDamageType, int iEraseLife )
{
    g_MyD3D->m_pDamageManager->DeleteMonsterDamageToType( m_iMonsterIndex, eDamageType, iEraseLife );
}

void MONSTER::FrameMove_ParticleModel()
{
    if( 0 == int(m_vecParticleModel.size()) )
        return;

    // HP ±¸°£¿¡ ¸Â´Â ÀÌÆåÆ®¸¦ Ã£ÀÚ!!
    int iLoop;
    bool bFinded = false;
    float fRadio = GetHP() / GetMaxHP();
    std::vector<sParticleModel>::iterator vecIter = m_vecParticleModel.begin();
    for( iLoop=0; vecIter != m_vecParticleModel.end(); ++vecIter, ++iLoop )
    {
        if( vecIter->m_fHpMinRadio < fRadio && fRadio <= vecIter->m_fHpMaxRadio )
        {
            bFinded = true;
            break;
        }
    }

    // HP±¸°£¿¡ ¸Â´Â ÀÌÆåÆ®°¡ ¾ø´Ù¸é ¾Æ¹«º¯È­¾øµµ·Ï ÇÏÀÚ
    if( (false == bFinded) && (iLoop == int(m_vecParticleModel.size())) )
        iLoop = m_iCurParticleModel;
    
    // »õ·Î¿î ·¹º§±¸°£ÀÌ ¼±ÅÃµÇ¾ú´Ù¸é ÀÌÆåÆ®¸¦ ¹ß¶óÁÖÀÚ!!
    if( iLoop != m_iCurParticleModel )
    {
        // ÇöÀç »Ñ·ÁÁö°íÀÖ´Â ÆÄÆ¼Å¬ÀÌ ÀÖÀ¸¸é Áö¿ìÀÚ!!
        if( 0 <= m_iCurParticleModel && m_iCurParticleModel < int(m_vecParticleModel.size()) )
        {
            g_ParticleManager->DeleteSequence( m_vecParticleModel[m_iCurParticleModel].pSequence );
            m_vecParticleModel[m_iCurParticleModel].pSequence = NULL;
        }

         //»õ·Î¿î ÆÄÆ¼Å¬À» »ý¼ºÇÏÀÚ!!
        m_iCurParticleModel = iLoop;
        if( m_vecParticleModel[iLoop].pSequence )
            g_ParticleManager->DeleteSequence( m_vecParticleModel[iLoop].pSequence );

        MSG_PARTICE_CREATE_INFO msg;
        msg.strSeqname = m_vecParticleModel[iLoop].strParticleName;
        if ( m_vecParticleModel[iLoop].m_bNoDirection == false )
            msg.fOffsetX = m_vecParticleModel[iLoop].m_fOffsetX;
        else
            msg.vPos.x = m_vecParticleModel[iLoop].m_fOffsetX;
        msg.vPos.y = m_vecParticleModel[iLoop].m_fOffsetY;

        msg.fTraceTime = 0.5f;
        msg.bDirecitonal = !m_vecParticleModel[iLoop].m_bNoDirection;
        msg.iBoneID = m_vecParticleModel[iLoop].m_iBornID;
        msg.iLeftSideLayer = GC_LAYER_BLOCK;
        msg.iRightSideLayer = GC_LAYER_BLOCK;

        if ( msg.iBoneID == -1 )
            msg.pTracePos = &vRenderPos;

        m_vecParticleModel[iLoop].pSequence = CreateParticle( msg );
    }

    if( (0 <= m_iCurParticleModel && m_iCurParticleModel < int(m_vecParticleModel.size())) &&
         m_vecParticleModel[m_iCurParticleModel].pSequence )
    {
        m_vecParticleModel[m_iCurParticleModel].pSequence->SetShow( true );
}
}

D3DXVECTOR2 MONSTER::GetPlayerPosByMinDistance()
{
    D3DXVECTOR2 vReturnPos(m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ]);

    float fTemp = 0.0;
    float fMinDistance = 5.0f;   // µðÆúÆ®´Â 5·Î Àâ°í 

    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        if ( g_MyD3D->MyPlayer[iLoop] && g_MyD3D->MyPlayer[iLoop]->IsLiveCharacter() ) { // ÇöÀç ÀÖ´Â ÇÃ·¹ÀÌ¾î Áß »ì¾ÆÀÖ´Â ÇÃ·¹ÀÌ¾î¸¸ °Ë»ç
            fTemp = DISTANCE( GetX(), GetY(), g_MyD3D->MyPlayer[iLoop]->vPos.x, g_MyD3D->MyPlayer[iLoop]->vPos.y );

            if ( fTemp < fMinDistance ) { // °¡Àå °¡±î¿î »ç¶÷À» ÁÖÀÎÀ¸·Î ÇÑ´Ù.
                fMinDistance = fTemp;

                vReturnPos.x = g_MyD3D->MyPlayer[iLoop]->vPos.x;
                vReturnPos.y = g_MyD3D->MyPlayer[iLoop]->vPos.y;
            }
        }
    }

    return vReturnPos;
}

bool MONSTER::SetLatencyX( int iIndex, float fPos )
{
    if( iIndex < 0 || iIndex >= LATENCY_BUFFER_SIZE ) {
        return false;
    }

    if( GetMovableMonster() == false )
        return false;

    m_afX[iIndex] = fPos;
    return true;
}

bool MONSTER::SetLatencyY( int iIndex, float fPos )
{
    if( iIndex < 0 || iIndex >= LATENCY_BUFFER_SIZE ) {
        return false;
    }

    if( GetMovableMonster() == false )
        return false;

    m_afY[iIndex] = fPos;
    return true;
}

void MONSTER::SetItemList( const std::vector< GCITEMID >& vecItemList )
{
    if( false == m_vecItemIDList.empty() )
        m_vecItemIDList.clear();

    m_vecItemIDList = vecItemList;
    MonsterItemEquip();
}

void MONSTER::MonsterItemEquip( void )
{
    EndAfterImage();
    SAFE_REMOVE_DELETE( m_pObject );
    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];
    m_pObject = g_RenderManager->CreateObject(kMonsterTemplate.iTechnique);

    std::vector< EXTRA_MESH > vecMeshInfo;
    MakeItemListMeshInfo( vecMeshInfo );
    LoadExtraMesh( vecMeshInfo );
    m_pObject->SetShellMatrixScale( DEFAULT_MONSTER_SCALE_SIZE );
}

bool MONSTER::MakeItemListMeshInfo( OUT std::vector< EXTRA_MESH >& vecMeshInfo )
{
    bool bShadowEquip = false;
    bool bRemovePartsItem = false;
    DWORD dwFlag = 0;
    vecMeshInfo.clear();

    bool bEquipFaceOffHead = false;
    // Ç¥Á¤½Ã½ºÅÛÀÌ Àû¿ëµÇ´Â Åõ±¸¸¦ »ç¿ëÇÏ´ÂÁö ÆÇº°ÇÏÀÚ
    for( std::vector< GCITEMID >::iterator it = m_vecItemIDList.begin() ; it != m_vecItemIDList.end() ; ++it ) {
        GCItem* pItem = g_pItemMgr->GetItemData( *it );
        if( NULL == pItem ) {
            continue;
        }

        // Ç¥Á¤½Ã½ºÅÛ Åõ±¸ÀÎÁö °Ë»ç
        if( SiGCFaceOffManager()->IsFaceOffItem( (*pItem).dwGoodsID ) ){
            bEquipFaceOffHead = true;
            break;
        }
    }
    
    std::string strFaceTexture;
    for( std::vector< GCITEMID >::iterator it = m_vecItemIDList.begin() ; it != m_vecItemIDList.end() ; ++it ) {
        GCItem* pItem = g_pItemMgr->GetItemData( *it );
        if( NULL == pItem ) {
            continue;
        }

        DWORD dwItemGoodsID = g_pItemMgr->GetSameImageIndex(pItem->dwGoodsID);

        DWORD dwBigHead = ESP_A_UPPER_HELMET + ESP_A_DOWN_HELMET;
        bool bHeadItem = false;
        if( pItem->dwSlotPosition & dwBigHead ||
            pItem->dwSlotPosition & ESP_HELMET ) { 
                bHeadItem = true;
        }

        if ( bHeadItem && g_pItemMgr->IsRemovePartsItem( (*pItem).dwGoodsID , HEAD) ){
            bRemovePartsItem = true;
        }
		
        if( bHeadItem ) { 
            const char* strAbtaName = g_pGCDeviceManager2->GetAbtaName( dwItemGoodsID );
            strFaceTexture = g_pGCDeviceManager2->GetResourceFileName( strAbtaName, m_iCharType, 0, "_NFL.dds" );
        }
        
        EXTRA_MESH mesh;
        mesh.strMeshName = boost::str( boost::format("abta%03d.p3m")%dwItemGoodsID );
        mesh.strTextureName = boost::str( boost::format("abta%03d.dds")%dwItemGoodsID );
        mesh.dwCartoon = 0;
        mesh.dwTechnique = -1;
        mesh.strID =  "";
        mesh.iBoneId =  -1;
        mesh.strMotionName = "";
        mesh.vOptionalPos.x = 0.0f;
        mesh.vOptionalPos.y = 0.0f;
        mesh.vOptionalPos.z = 0.0f;

        vecMeshInfo.push_back( mesh );

        dwFlag |= pItem->dwSlotPosition;

        if( pItem->dwGoodsID == ITEMID_ASIN_SHAADOW_SET || pItem->dwGoodsID == ITEMID_UNO_SHADOW_SET) {
            bShadowEquip = true;
        }
    }

    // ±âº» ¸ðµ¨ÀÌ ¾øÀ¸¸é Ã¤¿ö ÁÝ´Ï´Ù. ¹«±â Á¦¿Ü 
    DWORD aSlotPos[3] = { ESP_HELMET, ESP_JACKET, ESP_PANTS };
    for( int i=0 ; i<3 ; ++i ) {
        if( !(dwFlag & aSlotPos[i]) ) {

            GCITEMID baseItemID = g_pItemMgr->GetBaseItemID( m_iCharType, static_cast<ESLOTPOSITION>(aSlotPos[i]) );
            if( UINT_MAX == baseItemID ) { 
                continue;
            }

            EXTRA_MESH mesh;
            mesh.strMeshName = boost::str( boost::format("abta%03d.p3m")%baseItemID );
            mesh.strTextureName = boost::str( boost::format("abta%03d.dds")%baseItemID );
            mesh.dwCartoon = 0;
            mesh.dwTechnique = -1;
            mesh.strID =  "";
            mesh.iBoneId =  -1;
            mesh.strMotionName = "";
            mesh.vOptionalPos.x = 0.0f;
            mesh.vOptionalPos.y = 0.0f;
            mesh.vOptionalPos.z = 0.0f;

            vecMeshInfo.push_back( mesh );
        }
    }

    // µð¿À ÆÈ
    if( GC_CHAR_DIO == m_iCharType ) {
        EXTRA_MESH mesh;
        mesh.strMeshName = "Dio1_R_Hand.p3m";
        mesh.strTextureName = "Dio1_R_Hand.dds";
        mesh.dwCartoon = 0;
        mesh.dwTechnique = -1;
        mesh.strID =  "";
        mesh.iBoneId =  -1;
        mesh.strMotionName = "";
        mesh.vOptionalPos.x = 0.0f;
        mesh.vOptionalPos.y = 0.0f;
        mesh.vOptionalPos.z = 0.0f;

        vecMeshInfo.push_back( mesh );
    }

    // Á¦·Î °¡¸é 
    if( GC_CHAR_ZERO == m_iCharType ) {
        EXTRA_MESH mesh;
        mesh.strMeshName = "";
        mesh.strTextureName = "";
        mesh.dwCartoon = 0;
        mesh.dwTechnique = -1;
        mesh.strID =  "";
        mesh.iBoneId =  -1;
        mesh.strMotionName = "";
        mesh.vOptionalPos.x = 0.0f;
        mesh.vOptionalPos.y = 0.0f;
        mesh.vOptionalPos.z = 0.0f;

        vecMeshInfo.push_back( mesh );
    }

    std::vector< EGCSkillTree > setSkill;
    PLAYER_TEMPLATE kPlayerTemplate;
    SetPlayerTemplate(setSkill, m_iCharType, kPlayerTemplate );
    if( kPlayerTemplate.mapFaceLiftMesh.empty() == false ) {
        bool bLoad = false;

        if( bEquipFaceOffHead == true )
            bLoad = true;

        if( bEquipFaceOffHead == false ) {
            if( !SiGCFaceOffManager()->IsUseThisScript() || SiGCFaceOffManager()->IsFaceOffChar( m_iCharType ) )
                bLoad = true;
        }

        if ( bRemovePartsItem == true ) {
            bLoad = false;
        }

        if(bLoad) {
            int iFaceLift = FACE_LIFT_NORMAL;
            if( bShadowEquip ) {
                iFaceLift = FACE_LIFT_SHADOW;
            }

            std::map< int, FACE_LIFT_MESH >::iterator mit = kPlayerTemplate.mapFaceLiftMesh.find( iFaceLift );
            if( mit != kPlayerTemplate.mapFaceLiftMesh.end() ) {
                EXTRA_MESH mesh;
                mesh.strMeshName = mit->second.strMeshName;

	            if( "" == strFaceTexture ) { 
	                mesh.strTextureName = mit->second.strTextureName;
	            }
	            else {
	                mesh.strTextureName = strFaceTexture;
	            }
				
                mesh.dwCartoon = 0;
                mesh.dwTechnique = -1;
                mesh.strID =  "";
                mesh.iBoneId =  -1;
                mesh.strMotionName = "";
                mesh.vOptionalPos.x = 0.0f;
                mesh.vOptionalPos.y = 0.0f;
                mesh.vOptionalPos.z = 0.0f;

                vecMeshInfo.push_back( mesh );
            }
        }
    }   

    if( GC_CHAR_ASIN == m_iCharType ) { 
        // ¾Æ½Å ÆÈ º¸È£±¸
        EXTRA_MESH mesh;        
        mesh.strMeshName = "Asin_HandProtect.p3m";
        if( bShadowEquip ) {
            mesh.strTextureName = "Asin_HandProtect_Shadow.dds";
        } else {
            mesh.strTextureName = "Asin_HandProtect.dds";
        }        
        mesh.dwCartoon = 0;
        mesh.dwTechnique = -1;
        mesh.strID =  "";
        mesh.iBoneId =  -1;
        mesh.strMotionName = "";
        mesh.vOptionalPos.x = 0.0f;
        mesh.vOptionalPos.y = 0.0f;
        mesh.vOptionalPos.z = 0.0f;

        vecMeshInfo.push_back( mesh );
    }

    // Ã»¼Ò, Á¹±â °ü·Ã ¸ð¼ÇÀÌ¸é ºøÀÚ·ç ºÙÀÔ´Ï´Ù.
    std::string strCurState = GetState();
    if( strCurState == "DROWSE" || strCurState == "DROWSE_ACTION" ) {
        int iCharSex = g_kGlobalValue.IsSexCharacter( m_iCharType );
        EXTRA_MESH mesh;
        mesh.strMeshName = "broom.p3m";
        mesh.strTextureName = "broom.dds";
        mesh.dwCartoon = 0;
        mesh.dwTechnique = -1;
        mesh.strID =  "BROOM";
        mesh.iBoneId =  14;
        mesh.strMotionName = iCharSex == SEX_MAN ? "NPC_broom_ZZZ_man.frm" : "NPC_broom_ZZZ_girl.frm";
        mesh.vOptionalPos.x = 0.0f;
        mesh.vOptionalPos.y = 0.0f;
        mesh.vOptionalPos.z = 0.0f;

        vecMeshInfo.push_back( mesh );
    }
    else if ( strCurState == "CLEANING_ACTION" || strCurState == "CLEANING" ) { 
        EXTRA_MESH mesh;
        mesh.strMeshName = "broom.p3m";
        mesh.strTextureName = "broom.dds";
        mesh.dwCartoon = 0;
        mesh.dwTechnique = -1;
        mesh.strID =  "BROOM";
        mesh.iBoneId =  14;
        mesh.strMotionName = "NPC_broom.frm";
        mesh.vOptionalPos.x = 0.0f;
        mesh.vOptionalPos.y = 0.0f;
        mesh.vOptionalPos.z = 0.0f;

        vecMeshInfo.push_back( mesh );
    }
    return true;
}

void MONSTER::SetMentCount( void )
{
    if( m_iMentCount < 0 ) {
        srand( time(NULL) );
        int iTime = rand() % 21;
        iTime += 10;
        m_iMentCount = iTime * 55;

        // ½Ã°£ »õ·Î »õÆÃ µÉ ¶§ ´ë»ç »Ñ¸°´Ù. 
        SiKGCAgitGameManager()->SetAgitCharacterSayBaloon( m_iCharType, m_iAgitMonCharUID, KAgitCharacterPos::SPT_DEFAULT );       
    }
}

void MONSTER::SetAgitCharCloseMent( void )
{
    SiKGCAgitGameManager()->SetAgitCharacterSayBaloon( m_iCharType, m_iAgitMonCharUID, KAgitCharacterPos::SPT_CLOSE );     
}

bool MONSTER::CheckLiveMonster(int iMonsterType, int iMonCnt)
{
    if( 0 > iMonsterType || iMonsterType > MONSTER_MODEL_NUM )
        return false;

    for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first))
            continue;
        MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

        if (pMonster == NULL) continue;

        if (false == pMonster->IsLive())
            continue;

        if (0.0f >= pMonster->GetHP())
            continue;

        if (iMonsterType == pMonster->m_iMonsterType)
        {
            if (0 == iMonCnt)
                return true;
            else
                --iMonCnt;
        }
    }

    return false;
}

bool MONSTER::CheckLiveMonsterFromAllSlot(int iTeam)
{
    MAP_INT_PMONSTER::iterator mapIter = g_kMonsterManager.m_mapMonster.begin();
    for (; mapIter != g_kMonsterManager.m_mapMonster.end(); ++mapIter)
    {
        if (NULL == mapIter->second || mapIter->first == m_iMonsterIndex || !mapIter->second->IsLive() || 0.0f >= mapIter->second->GetHP())
            continue;

        if (iTeam != mapIter->second->m_iTeam)
            continue;

        return true;
    }
    return false;
}

int MONSTER::GetLiveMonsterCnt(int iMonsterType)
{

    if( 0 > iMonsterType || iMonsterType > MONSTER_MODEL_NUM )
        return false;

    int iCount = 0;
    for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        if (g_kMonsterManager.IsSummonMonsterByPlayer(mit->first))
            continue;
        MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

        if (pMonster == NULL) continue;

        if (false == pMonster->IsLive())
            continue;

        if (0.0f >= pMonster->GetHP())
            continue;

        if (iMonsterType == pMonster->m_iMonsterType)
        {
            ++iCount;
        }
    }

    return iCount;
}

bool MONSTER::IsHost()
{
    return g_kGlobalValue.m_kUserInfo.bHost;
}

bool MONSTER::IsPlatformer()
{ 
    return g_pkQuestManager->IsPlatformer();
}


void MONSTER::ClearTraceIcon()
{
    for ( unsigned int i = 0 ; i < TraceIcon.size(); ++i )
    {
        g_ParticleManager->DeleteSequence( TraceIcon[i] );
    }
    TraceIcon.clear();
}

float MONSTER::GetDamageFromPlayerAttack( float fDamage, int iPlayerIndex, bool bSpecialAtk, float fSummonsOwnerAtkRatio_ )
{
    float fResultDamage = fDamage;

    fDamage *= ( 1.0f + g_MyD3D->MyPlayer[iPlayerIndex]->m_fAbility[ABILITY_ATTACK_PERCENT] );


    fResultDamage = fDamage;

    return fResultDamage;
}

void MONSTER::ShowEscortMonsterDesc( float fDamage_ )
{
    if( m_iMonsterType != g_kGlobalValue.GetEscortMonsterID() ) {
        return;
    }

    if ( m_fShield > 0.0f ) { 
        return;
    }

    float fBeforeHP = m_fHP;
    float fAfterHP = m_fHP - fDamage_;

    float fHP75 = m_fHPPoint * ( 75.0f / 100.0f );
    float fHP50 = m_fHPPoint * ( 50.0f / 100.0f );
    float fHP25 = m_fHPPoint * ( 25.0f / 100.0f );

    int iStringID = -1;
    if( fBeforeHP > fHP75 && fAfterHP <= fHP75 ) { 
        iStringID = STR_ID_ESCORT_MERRY_HP_75;
    }
    else if ( fBeforeHP < fHP75 && fBeforeHP > fHP50 && fAfterHP <= fHP50 ) { 
        iStringID = STR_ID_ESCORT_MERRY_HP_50;
    }
    else if( fBeforeHP < fHP50 && fBeforeHP > fHP25 && fAfterHP <= fHP25 ) { 
        iStringID = STR_ID_ESCORT_MERRY_HP_25;
    }

    if( iStringID != -1 ) { 
        KGC_PID_BROAD_ESCORT_MONSTER_DESC kPacket;
        kPacket.Type = GC_PID_BROAD_ESCORT_MONSTER_DESC;
        kPacket.iStringID = iStringID;
        kPacket.iShowFrame = 275;
        kPacket.bWarning = true;

        SendP2PPacket( &kPacket, sizeof( kPacket ), KNC_BROADCAST, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_ESCORT_MONSTER_DESC" );
    }
}

void MONSTER::ShowEscortMonsterHPBar( void )
{
    if( false == m_bShowEscortHPBar )
    {
        if( m_iMonsterType != g_kGlobalValue.GetEscortMonsterID() ) { 
            return;
        }

        if( -1 == g_kGlobalValue.GetEscortMonsterID() ) { 
            return;
        }
    }

    g_pkGameOverlayUI->SetEscortMonHPBar( m_fHPPoint, m_fHP );
}

int MONSTER::GetFrame()
{
    return m_aiFrame[ LATENCY_INDEX ];
}

void MONSTER::SetDie()
{
    m_bDie = true;
    m_iSummoned = MEMS_NOT_SUMMON_MONSTER;
	m_iSummonsNuberAvailable = 15;

#if defined ( USE_MERRY_HP_BAR )
	ShowEscortMonsterHPBar();
#endif

    // ¼ÒÈ¯ ¸ó½ºÅÍÀÏ¶§ ¹öÇÁ ¾ÆÀÌÄÜÀ» Á¦°ÅÇØÁÖÀÚ!
    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( m_dwOwnerUID );
    if( pPlayer && pPlayer->IsLocalPlayer() && m_iSummonBuffIcon )
    {
        if( pPlayer->IsMagicEffect( m_iSummonBuffIcon ) )
            pPlayer->ClearMagicEffect( m_iSummonBuffIcon );
    }

    if ( m_kChangeMon.usMonType != -1 && g_kGlobalValue.m_kUserInfo.bHost )
    {
        g_kChangeMonster->Type = GC_PID_CHANGE_MONSTER;
        g_kChangeMonster->usMonIndex = m_iMonsterIndex;
        g_kChangeMonster->kChangeMon = m_kChangeMon;

        SendNetworkData( GC_PID_CHANGE_MONSTER, 0 );
    }
}

bool MONSTER::ChangeTraceIcon( char *szParitcleName, int iIconType /* = 0*/ )
{
    if( 0 > iIconType || iIconType >= int( TraceIcon.size() ) )
        return false;

    TRACE_INFO trace;		
    trace.m_fParticleTraceTime = 5.0f;
    trace.m_pvPos = &TraceIconPos;

    CParticleEventSeqPTR pSeq = g_ParticleManager->CreateSequence( szParitcleName, 0.0f, 0.0f, 0.0f );
    if ( pSeq )
    {
        pSeq->SetTrace( &trace );

        if( 1 != iIconType )
            pSeq->SetBeginPos( TraceIconPos );
        
        g_ParticleManager->DeleteSequence( TraceIcon[ iIconType ] );
        std::vector<CParticleEventSeqPTR>::iterator vecIter = TraceIcon.erase( (TraceIcon.begin() + iIconType) );

        TraceIcon.insert( vecIter, pSeq );
    }

    return (NULL != pSeq);
}

bool MONSTER::ResetTraceIcon( char *szParitcleName1, char *szParitcleName2 )
{
    if( NULL == szParitcleName1 || NULL == szParitcleName2 )
        return false;

    CParticleEventSeqPTR pSeq1 = g_ParticleManager->CreateSequence( szParitcleName1, 0.0f, 0.0f, 0.0f );
    CParticleEventSeqPTR pSeq2 = g_ParticleManager->CreateSequence( szParitcleName2, 0.0f, 0.0f, 0.0f );
    if( NULL == pSeq1 || NULL == pSeq2 )
        return false;

    TRACE_INFO trace;		
    trace.m_fParticleTraceTime = 5.0f;
    trace.m_pvPos = &TraceIconPos;

    pSeq1->SetTrace( &trace );
    pSeq2->SetTrace( &trace );
    pSeq2->SetBeginPos( TraceIconPos );

    ClearTraceIcon();
    TraceIcon.push_back( pSeq1 );
    TraceIcon.push_back( pSeq2 );

    return true;
}

bool MONSTER::IsTargetDamage() const
{
    return m_dwTargetDamage != 0;
}

void MONSTER::TurnToDamageTarget()
{
    if( false == IsTargetDamage() )
        return;

    D3DXVECTOR2 vPos = GetTargetDamagePos();
    if( m_afX[ LATENCY_INDEX ] < vPos.x )
        SetIsRight( true );
    else
        SetIsRight( false );
}

D3DXVECTOR2 MONSTER::GetTargetDamagePos()
{
    D3DXVECTOR2 vPos(0.0f, 0.0f);
    std::map< int, std::vector<CDamageInstance*> >& mapInstance = g_MyD3D->m_pDamageManager->GetDamageInstancePool();
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = mapInstance.begin();
    for(; mapIter != mapInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter )
        {
            if( (DWORD)*vecIter == m_dwTargetDamage )
            {
                vPos.x = (*vecIter)->m_X;
                vPos.y = (*vecIter)->m_Y;
                break;
            }
        }

        if( 0.0f != vPos.x || 0.0f != vPos.y )
            break;
    }

    if( mapIter == mapInstance.end() )
        m_dwTargetDamage = 0;

    return vPos;
}

bool MONSTER::SetDamageTargetAllRange( int iDamageEnum )
{
    return DamageTarget( iDamageEnum, 0, 0, 0, 0, 0, 0, false );
}

bool MONSTER::SetDamageTarget( int iDamageEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    return DamageTarget( iDamageEnum, iOffsetX, iLeft, iTop, iRight, iBottom, iOffsetY, true );
}

bool MONSTER::DamageTarget( int iDamageEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY, bool bCheckRange )
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( bCheckRange )
    {
        if( m_abIsRight[ LATENCY_INDEX ] )
        {
            iX1 += (float)iOffsetX / 400.0f;
            iX2 += (float)iOffsetX / 400.0f;
        }
        else
        {
            iX1 -= (float)iOffsetX / 400.0f;
            iX2 -= (float)iOffsetX / 400.0f;
        }

        iY1 += (float)iOffsetY / 400.0f;
        iY2 += (float)iOffsetY / 400.0f;
    }

    DWORD dwNearDamage = 0;
    float fMinDistance = 9999.0f;
    std::map< int, std::vector<CDamageInstance*> >& mapInstance = g_MyD3D->m_pDamageManager->GetDamageInstancePool();
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = mapInstance.begin();
    for(; mapIter != mapInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter )
        {
            if( (*vecIter)->m_What == iDamageEnum )
            {
                if( bCheckRange )
                {
                    // ¹üÀ§¾È¿¡ µ¥¹ÌÁö°¡ ÀÖ´ÂÁö »ìÇÉ´Ù
                    if( false == ((*vecIter)->m_X > m_afX[ LATENCY_INDEX ] + iX1 && 
                                  (*vecIter)->m_X < m_afX[ LATENCY_INDEX ] + iX2 &&
                                  (*vecIter)->m_Y > m_afY[ LATENCY_INDEX ] + iY2 && 
                                  (*vecIter)->m_Y < m_afY[ LATENCY_INDEX ] + iY1) )
                        continue;
                }

                float fDist = DISTANCE( (*vecIter)->m_X, (*vecIter)->m_Y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
                if( fMinDistance > fDist )
                {
                    fMinDistance = fDist;
                    dwNearDamage = (DWORD)*vecIter;
                }
            }
        }
    }

    bool bResult = false;
    if( 0 < dwNearDamage )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_dwTargetDamage != dwNearDamage )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }
        m_dwTargetDamage = dwNearDamage;
        bResult = true;
    }

    return bResult;
}

bool MONSTER::CheckDamageTargetInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY/* = 0*/ )
{
    if( !IsTargetDamage() )
        return false;

    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

#if !defined( __PATH__ ) // ¸ó½ºÅÍÀÇ ¹Ù¿îµù¹Ú½º
    {
        if( g_kGlobalValue.m_bRenderBoundBox )
        {
            D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
            SETVECTOR3( lefttop, iX1- 1.0f, iY1- 0.25f , 0.4f );
            SETVECTOR3( righttop, iX2- 1.0f, iY1- 0.25f , 0.4f );
            SETVECTOR3( leftbottom, iX1- 1.0f, iY2- 0.25f , 0.4f );
            SETVECTOR3( rightbottom, iX2- 1.0f, iY2- 0.25f , 0.4f );

            g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
                lefttop, righttop, leftbottom, rightbottom,0,0,0,
                D3DCOLOR_ARGB(90, 0, 200, 0));
        }
    }
#endif

    std::map< int, std::vector<CDamageInstance*> >& mapInstance = g_MyD3D->m_pDamageManager->GetDamageInstancePool();
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = mapInstance.begin();
    for(; mapIter != mapInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter )
        {
            if( (DWORD)*vecIter == m_dwTargetDamage )
            {
                // ¹üÀ§¾È¿¡ µ¥¹ÌÁö°¡ ÀÖ´ÂÁö »ìÇÉ´Ù
                if( (*vecIter)->m_X > m_afX[ LATENCY_INDEX ] + iX1 && 
                    (*vecIter)->m_X < m_afX[ LATENCY_INDEX ] + iX2 &&
                    (*vecIter)->m_Y > m_afY[ LATENCY_INDEX ] + iY2 && 
                    (*vecIter)->m_Y < m_afY[ LATENCY_INDEX ] + iY1 )
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool MONSTER::CheckDamageTarget( bool bReset /* = false */)
{
    if( false == IsTargetDamage() )
        return false;

    std::map< int, std::vector<CDamageInstance*> >& mapInstance = g_MyD3D->m_pDamageManager->GetDamageInstancePool();
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = mapInstance.begin();
    for(; mapIter != mapInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter )
        {
            if( (DWORD)*vecIter == m_dwTargetDamage )
            {
                return true;
            }
        }
    }

    if( bReset )
        m_dwTargetDamage = 0;

    return false;
}

void MONSTER::DeleteTargetDamage()
{
    std::map< int, std::vector<CDamageInstance*> >& mapInstance = g_MyD3D->m_pDamageManager->GetDamageInstancePool();
    std::map< int, std::vector<CDamageInstance*> >::iterator mapIter = mapInstance.begin();
    for(; mapIter != mapInstance.end(); ++mapIter)
    {
        std::vector<CDamageInstance*>::iterator vecIter = mapIter->second.begin();
        for(; vecIter != mapIter->second.end(); ++vecIter )
        {
            if( (DWORD)*vecIter == m_dwTargetDamage )
            {
                (*vecIter)->m_Life = 1;
                return;
            }
        }
    }
}

int MONSTER::GetDamageInstanceCount( int iDamageEnum, int iLife )
{
    return g_MyD3D->m_pDamageManager->GetDamageInstanceCount( iDamageEnum, iLife );
}

void MONSTER::SelectEscortHPBarImg( int iImgIndex )
{
    g_pkGameOverlayUI->SelectEscortMonHPBarImg( iImgIndex );
}

bool MONSTER::CheckEnemyInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( false == EffectivePlayer(i) )
            continue;

        if( pPlayer->vPos.x > iX1 && 
            pPlayer->vPos.x < iX2 &&
            pPlayer->vPos.y > iY2 && 
            pPlayer->vPos.y < iY1 )
        {
            return true;
        }
    }

    GCAttackRect<float> rect;
    GCAttackRect<float> CollisionRect;
    rect.SetRect( iX1- 1.0f, iY1- 0.25f, iX2- 1.0f, iY2- 0.25f);
	


    for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ;  ++ mit ) {
        if( false == EffectiveMonster( mit->first ) )
            continue;

        if ( mit->second == NULL )
            continue;

        std::vector< GCAttackRect<float> >::iterator vecIter = mit->second->m_vecAttackRect.begin();
        for(; vecIter != mit->second->m_vecAttackRect.end(); ++vecIter)
        {
            if( rect.CheckCollision( *vecIter, &CollisionRect ) )
            {
                return true;
            }
        }
    }
    return false;
}

int MONSTER::GetLiveEnemyCnt()
{
    int iCount = 0;
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( false == EffectivePlayer(i) )
            continue;

        ++iCount;
    }

    for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ;  ++ mit ) {
        if( false == EffectiveMonster( mit->first ) )
            continue;

        if ( mit->second == NULL )
            continue;

        ++iCount;
    }

    return iCount;
}

bool MONSTER::CheckMonsterInRange( int iSlot, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    GCAttackRect<float> rect;
    GCAttackRect<float> CollisionRect;
    rect.SetRect( iX1- 1.0f, iY1- 0.25f, iX2- 1.0f, iY2- 0.25f);

    MONSTER *pMonster = g_kMonsterManager.GetMonster( iSlot );
    if( pMonster )
    {
        std::vector< GCAttackRect<float> >::iterator vecIter = pMonster->m_vecAttackRect.begin();
        for(; vecIter != pMonster->m_vecAttackRect.end(); ++vecIter)
        {
            if( rect.CheckCollision( *vecIter, &CollisionRect ) )
            {
                return true;
            }
        }
    }

    return false;
}

bool MONSTER::CheckPlayerInRange( int iSlot, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    if( g_MyD3D->IsPlayerIndex( iSlot ) )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[iSlot];
        if( pPlayer )
        {
            if( pPlayer->vPos.x > iX1 && 
                pPlayer->vPos.x < iX2 &&
                pPlayer->vPos.y > iY2 && 
                pPlayer->vPos.y < iY1 )
            {
                return true;
            }
        }
    }

    return false;
}

D3DXVECTOR2 MONSTER::CheckNearFootHold( int iOffsetX, int iOffsetY )
{
    D3DXVECTOR2 vBlock_1 = SiKGCFantasticMap()->CheckNearFootHold( 0, m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ], iOffsetX, iOffsetY, true );
    D3DXVECTOR2 vBlock_2 = SiKGCFantasticMap()->CheckNearFootHold( 0, m_afX[ LATENCY_INDEX ], m_afY[ LATENCY_INDEX ], iOffsetX, iOffsetY, false );
    
    if( 0.0f == vBlock_1.x && 0.0f == vBlock_1.y )
        vBlock_1 = vBlock_2;
    if( 0.0f == vBlock_2.x && 0.0f == vBlock_2.y )
        vBlock_2 = vBlock_1;

    D3DXVECTOR2 v1, v2;
    v1.x = vBlock_1.x > m_afX[ LATENCY_INDEX ] ? (vBlock_1.x - m_afX[ LATENCY_INDEX ]) : (m_afX[ LATENCY_INDEX ] - vBlock_1.x);
    v1.y = vBlock_1.y > m_afY[ LATENCY_INDEX ] ? (vBlock_1.y - m_afY[ LATENCY_INDEX ]) : (m_afY[ LATENCY_INDEX ] - vBlock_1.y);
    v2.x = vBlock_2.x > m_afX[ LATENCY_INDEX ] ? (vBlock_2.x - m_afX[ LATENCY_INDEX ]) : (m_afX[ LATENCY_INDEX ] - vBlock_2.x);
    v2.y = vBlock_2.y > m_afY[ LATENCY_INDEX ] ? (vBlock_2.y - m_afY[ LATENCY_INDEX ]) : (m_afY[ LATENCY_INDEX ] - vBlock_2.y);

    return ( D3DXVec2Length( &v1 ) < D3DXVec2Length( &v2 ) ) ? vBlock_1 : vBlock_2;
}

float MONSTER::GetVectorDistance( float fX, float fY )
{
    D3DXVECTOR2 vDist;

    if( fX < m_afX[ LATENCY_INDEX ] )
        vDist.x = m_afX[ LATENCY_INDEX ] - fX;
    else
        vDist.x = fX - m_afX[ LATENCY_INDEX ];

    if( fY < m_afY[ LATENCY_INDEX ] )
        vDist.y = m_afY[ LATENCY_INDEX ] - fY;
    else
        vDist.y = fY - m_afY[ LATENCY_INDEX ];
    
    float fResult = D3DXVec2Length( &vDist );

    return fResult;
}

void MONSTER::TurnToTempTarget()
{
    if( false == IsTempTarget() )
        return;

    D3DXVECTOR2 vPos = GetTempTargetPos();
    if( m_afX[ LATENCY_INDEX ] < vPos.x )
        SetIsRight( true );
    else
        SetIsRight( false );
}

void MONSTER::StartAfterImage()
{
    if ( m_pObject ) 
    {
        g_MyD3D->m_kAfterImageRenderer.RegisterObject( m_pObject );
    }
}

void MONSTER::EndAfterImage()
{
    if ( m_pObject ) 
    {
        if( g_MyD3D->m_kAfterImageRenderer.IsRegisterObject( m_pObject ) )
            g_MyD3D->m_kAfterImageRenderer.UnRegisterObject( m_pObject );
    }
}

bool MONSTER::SetTargetAllRange( bool bPassPlayer, bool bPassMonster )
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    int iNearPlayer = -1;
    int iNearMonster = -1;
    float fMinDistance = 9999.0f;
    if( false == bPassPlayer )
    {
        for( int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

            if( false == EffectivePlayer(iLoop) )
                continue;

            float fDist = DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
            if( fMinDistance > fDist )
            {
                fMinDistance = fDist;
                iNearPlayer = iLoop;
            }
        }
    }

    // ¸ó½ºÅÍµµ °°Àº ÆÀÀÌ ¾Æ´Ñ ¸ó½ºÅÍ¸¦ °ø°ÝÇØ¾ßÇÑ´Ù.
    fMinDistance = -9999.0f;

    if( false == bPassMonster )
    {
        for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
            if( !mit->second->IsLive() || mit->second->GetHP() <= 0.f )
                continue;

            if ( false == EffectiveMonster( mit->first ) )
                continue;

            if ( mit->second == NULL )
                continue;

            float fDist = DISTANCE( mit->second->m_afX[ LATENCY_INDEX ], mit->second->m_afY[ LATENCY_INDEX ], m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
            if( fMinDistance > fDist )
            {
                fMinDistance = fDist;
                iNearMonster = mit->first;
            }
        }
    }

    bool bResult = false;
    if( 0 <= iNearPlayer || m_bAggroState )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_iTargetPlayerIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }
        SetTargetPlayerIndex( iNearPlayer );
        m_iTargetMonsterIndex = -1;
        bResult = true;
    }
    else if( 0 <= iNearMonster )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_iTargetMonsterIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }
        m_iTargetMonsterIndex = iNearMonster;
        m_iTargetPlayerIndex = -1;
        bResult = true;
    }

    return bResult;
}

D3DXVECTOR2 MONSTER::GetEnemyDistAllRange( bool bPassPlayer, bool bPassMonster )
{
    int iNearPlayer = -1;
    int iNearMonster = -1;
    float fMinDistance = 9999.0f;

    if( false == bPassPlayer )
    {
        for( int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

            if( false == EffectivePlayer(iLoop) )
                continue;

            float fDist = DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
            if( fMinDistance > fDist )
            {
                fMinDistance = fDist;
                iNearPlayer = iLoop;
            }
        }
    }

    if( false == bPassMonster )
    {
        fMinDistance = -9999.0f;

        for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
            if ( false == EffectiveMonster( mit->first ) )
                continue;

            if ( mit->second == NULL )
                continue;

            float fDist = DISTANCE( mit->second->m_afX[ LATENCY_INDEX ], mit->second->m_afY[ LATENCY_INDEX ], m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
            if( fMinDistance > fDist )
            {
                fMinDistance = fDist;
                iNearMonster = mit->first;
            }
        }
    }

    D3DXVECTOR2 vResult(0.0f, 0.0f);
    if( 0 <= iNearPlayer )
    {
        vResult.x = g_MyD3D->MyPlayer[iNearPlayer]->vPos.x;
        vResult.y = g_MyD3D->MyPlayer[iNearPlayer]->vPos.y;
    }
    else if( 0 <= iNearMonster )
    {
		MONSTER *pMonster = g_kMonsterManager.GetMonster(iNearMonster);
		
		if ( pMonster != NULL ) {
			vResult.x = pMonster->GetX();
			vResult.y = pMonster->GetY();
		}
    }

    return vResult;
}

bool MONSTER::EffectiveTarget()
{
	MONSTER *pMonster = g_kMonsterManager.GetMonster(m_iTargetMonsterIndex);

    if ( IsTargetMonster() && pMonster != NULL )
    {
        if( pMonster->IsLive() == false ||
            pMonster->IsDie() )
        {
            ReleaseTarget();
            return false;
        }
    }
    else if( g_MyD3D->IsPlayerIndex( m_iTargetPlayerIndex ) )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[m_iTargetPlayerIndex];
        if( pPlayer->Item_Remain_Time[GC_GAME_ITEM_HALLOW] ||
            pPlayer->CheckHiding() ||
            pPlayer->m_ePlayerState == PS_DEAD ||
            pPlayer->m_kUserInfo.bLive == false ||
            pPlayer->m_cLife == 0 ||
            pPlayer->uiMotion == pPlayer->GetCurFormTemplate().START )
        {
            ReleaseTarget();
            return false;
        }    
    }
    return true;
}

bool MONSTER::EffectivePlayer( int iPlayerIndex )
{
    if( false == g_MyD3D->IsPlayerIndex( iPlayerIndex ) )
        return false;

    PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIndex];
    if( pPlayer->m_kUserInfo.bLive == false )
        return false;
    if( pPlayer->m_cLife == 0 )
        return false;
    if( pPlayer->Item_Remain_Time[GC_GAME_ITEM_HALLOW] )
        return false;
    if( pPlayer->CheckHiding() )
        return false;
    if( pPlayer->m_ePlayerState == PS_DEAD )
        return false;
    if( pPlayer->uiMotion == pPlayer->GetCurFormTemplate().START )
        return false;
    if ( pPlayer->m_bNotTargeting == true)
        return false;
    if( g_MyD3D->IsSameTeam_PlayerAndMonster(iPlayerIndex, m_iMonsterIndex) )
        return false;

    return true;

}

bool MONSTER::EffectiveMonster( int iMonsterIndex )
{
    MONSTER* pMonster = g_kMonsterManager.GetMonster( iMonsterIndex );

    if( pMonster == NULL || !pMonster->IsLive() )
        return false;

    if( g_MyD3D->IsSameTeam_MonsterAndMonster(iMonsterIndex, m_iMonsterIndex) )
        return false;

    if( NPC_NONE != pMonster->m_eNPC )
        return false;

    if( pMonster->m_bMonsterTarget == false )
        return false;

    return true;
}

D3DXVECTOR2 MONSTER::GetEnemyPlayerPos(int iPlayerIndex )
{
    D3DXVECTOR2 vResult(0.0f, 0.0f);
    if( EffectivePlayer(iPlayerIndex) )
    {
        vResult.x = g_MyD3D->MyPlayer[ iPlayerIndex ]->vPos.x;
        vResult.y = g_MyD3D->MyPlayer[ iPlayerIndex ]->vPos.y;
    }
    return vResult;
}

D3DXVECTOR2 MONSTER::GetEnemyMonsterPos(int iMonsterIndex )
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster( iMonsterIndex );
	D3DXVECTOR2 vResult(0.0f, 0.0f);

	if( pMonster == NULL || !pMonster->IsLive() )
		return vResult;

    if( EffectiveMonster(iMonsterIndex) )
    {
        vResult.x = pMonster->GetX();
        vResult.y = pMonster->GetY();
    }
    return vResult;
}

int MONSTER::IsMagicEffectCount( int iMaxMon, int iBuffIndex_ )
{
    return g_pMagicEffect->GetBuffInstanceCount( iBuffIndex_, false );
}

bool MONSTER::SummonMonsterStaticSlot( KSummonInfo* pSummonInfo, bool bNoCheckType, bool bNoMonsterPos, int iSummonMonsterLevel, int iSlot )
{
    if( g_kGlobalValue.m_kUserInfo.bHost == false )
        return false;

    MONSTER* pMon = g_kMonsterManager.GetMonster(iSlot);

	if ( pMon == NULL || pMon->m_bLive ) {
		iSlot = g_kMonsterManager.GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE_SUMMON );
	}
	
    float x, y;
    if( bNoMonsterPos )
    {
        x = pSummonInfo->fX / SiKGCFantasticMap()->GetPixelWidth();
        y = pSummonInfo->fY;
    }
    else
    {
        float fXDiff = m_abIsRight[0] ? pSummonInfo->fX*0.0025f : pSummonInfo->fX*(-0.0025f);
        x = ( m_afX[0] + fXDiff ) / SiKGCFantasticMap()->GetPixelWidth();
        y = m_afY[0] + pSummonInfo->fY*0.0025f;
        SiKGCFantasticMap()->GetSafePosition( x, y, false );
    }

    int iLevel = iSummonMonsterLevel;

    g_kSummonMonster->Init();
    strncpy( g_kSummonMonster->m_szState, pSummonInfo->strState.c_str(), pSummonInfo->strState.size() );
    g_kSummonMonster->m_usMonIndex = static_cast<USHORT>( iSlot ) ;
    g_kSummonMonster->m_usMonType = (USHORT)(pSummonInfo->iMonType);
    g_kSummonMonster->m_iCountValue = pSummonInfo->iCountValue;
    g_kSummonMonster->m_usEntranceX = GCUTIL_MATH::floatToHalf( x );
    g_kSummonMonster->m_usEntranceY = GCUTIL_MATH::floatToHalf( y );
    g_kSummonMonster->m_iLevel = iSummonMonsterLevel;
    g_kSummonMonster->m_bIsRight = m_abIsRight[0];
    g_kSummonMonster->m_bCheckSafePostion = pSummonInfo->bCheckSafePostion;

    if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
    {
        if( g_kMonsterManager.IsSummonMonsterByPlayer( g_kSummonMonster->m_usMonIndex ) )
        {
            ASSERT( !"g_kSummonMonster->m_usMonIndex >= SUMMON_MONSTER_INDEX" );
        }

        float fX = GCUTIL_MATH::halfToFloat( g_kSummonMonster->m_usEntranceX );
        float fY = GCUTIL_MATH::halfToFloat( g_kSummonMonster->m_usEntranceY );

        g_MyD3D->AddMonster( g_kSummonMonster->m_usMonType, fX, fY, g_kSummonMonster->m_bIsRight, 0, 0, g_kSummonMonster->m_iLevel, 0, true, false, g_kSummonMonster->m_usMonIndex );

        MONSTER* pMon = g_kMonsterManager.GetMonster( g_kSummonMonster->m_usMonIndex );

        if ( pMon == NULL ) return false;

        pMon->m_bLive = true;
        pMon->m_pObject->SetRender( true );
        pMon->SetCountValue( pSummonInfo->iCountValue );
        pMon->SetState( g_kSummonMonster->m_szState );
        pMon->m_iSummoned = MONSTER::MEMS_SUMMON_MONSTER_BY_MONSTER;
    }
    else
    {
        SendNetworkData( GC_PID_SUMMON_MONSTER, KNC_BROADCAST );
    }

    return true;
}

bool MONSTER::IsTargetSyncObject() const
{
    return m_dwTargetSyncObject != 0;
}

void MONSTER::TurnToSyncObjectTarget()
{
    if( false == IsTargetSyncObject() )
        return;

    D3DXVECTOR2 vPos = GetTargetSyncObjectPos();
    if( m_afX[ LATENCY_INDEX ] < vPos.x )
        SetIsRight( true );
    else
        SetIsRight( false );
}

D3DXVECTOR2 MONSTER::GetTargetSyncObjectPos()
{
    D3DXVECTOR2 vTargetPos = SiKGCSyncObjectManager()->GetInstancePos( m_dwTargetSyncObject );

    if( 0.0f == vTargetPos.x && 0.0f == vTargetPos.y )
        m_dwTargetSyncObject = 0;

    return vTargetPos;
}

bool MONSTER::SetSyncObjectTargetAllRange( int iObjectEnum )
{
    return SyncObjectTarget( iObjectEnum, 0, 0, 0, 0, 0, 0, false );
}

bool MONSTER::SetSyncObjectTarget( int iObjectEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    return SyncObjectTarget( iObjectEnum, iOffsetX, iLeft, iTop, iRight, iBottom, iOffsetY, true );
}

bool MONSTER::SyncObjectTarget( int iObjectEnum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY, bool bCheckRange )
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( bCheckRange )
    {
        if( m_abIsRight[ LATENCY_INDEX ] )
        {
            iX1 += (float)iOffsetX / 400.0f;
            iX2 += (float)iOffsetX / 400.0f;
        }
        else
        {
            iX1 -= (float)iOffsetX / 400.0f;
            iX2 -= (float)iOffsetX / 400.0f;
        }

        iY1 += (float)iOffsetY / 400.0f;
        iY2 += (float)iOffsetY / 400.0f;
    }

    const std::map<DWORD, SyncObjectInstance*>* pInstance = SiKGCSyncObjectManager()->GetObjectInstancePool();
    if(NULL == pInstance )
        return false;

    DWORD dwNearObject = 0;
    float fMinDistance = 9999.0f;
    std::map<DWORD, SyncObjectInstance*>::const_iterator mapIter = pInstance->begin();
    for(; mapIter != pInstance->end(); ++mapIter)
    {
        if( mapIter->second && mapIter->second->m_iObjectID == iObjectEnum )
        {
            if( bCheckRange )
            {
                if( false == (  mapIter->second->m_vPos.x > m_afX[ LATENCY_INDEX ] + iX1 && 
                                mapIter->second->m_vPos.x < m_afX[ LATENCY_INDEX ] + iX2 &&
                                mapIter->second->m_vPos.y > m_afY[ LATENCY_INDEX ] + iY2 && 
                                mapIter->second->m_vPos.y < m_afY[ LATENCY_INDEX ] + iY1) )
                    continue;
            }

            float fDist = DISTANCE( mapIter->second->m_vPos.x, mapIter->second->m_vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
            if( fMinDistance > fDist )
            {
                fMinDistance = fDist;
                dwNearObject = mapIter->first;
            }
        }
    }

    bool bResult = false;
    if( 0 < dwNearObject )
    {
        // »õ·Î Å¸°ÙÆÃ ÇßÀ»¶§ ¸Ó¸®À§¿¡ ´À³¦Ç¥ µûÀ§ »Ñ·ÁÁÖÀð ¤¾
        if ( m_dwTargetSyncObject != dwNearObject )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }
        m_dwTargetSyncObject = dwNearObject;
        bResult = true;
    }

    return bResult;
}

bool MONSTER::CheckSyncObjectTargetInRange( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY/* = 0*/ )
{
    if( !IsTargetSyncObject() )
        return false;

    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

#if !defined( __PATH__ ) // ¸ó½ºÅÍÀÇ ¹Ù¿îµù¹Ú½º
    {
        if( g_kGlobalValue.m_bRenderBoundBox )
        {
            D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
            SETVECTOR3( lefttop, iX1- 1.0f, iY1- 0.25f , 0.4f );
            SETVECTOR3( righttop, iX2- 1.0f, iY1- 0.25f , 0.4f );
            SETVECTOR3( leftbottom, iX1- 1.0f, iY2- 0.25f , 0.4f );
            SETVECTOR3( rightbottom, iX2- 1.0f, iY2- 0.25f , 0.4f );

            g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
                lefttop, righttop, leftbottom, rightbottom,0,0,0,
                D3DCOLOR_ARGB(90, 0, 200, 0));
        }
    }
#endif

    const std::map<DWORD, SyncObjectInstance*>* pInstance = SiKGCSyncObjectManager()->GetObjectInstancePool();
    if(NULL == pInstance )
        return false;

    std::map<DWORD, SyncObjectInstance*>::const_iterator mapIter = pInstance->begin();
    for(; mapIter != pInstance->end(); ++mapIter)
    {
        if( mapIter->second && mapIter->first == m_dwTargetSyncObject )
        {
            if( mapIter->second->m_vPos.x > iX1 && 
                mapIter->second->m_vPos.x < iX2 &&
                mapIter->second->m_vPos.y > iY2 && 
                mapIter->second->m_vPos.y < iY1 )
            {
                return true;
            }
        }
    }

    return false;
}

bool MONSTER::CheckSyncObjectTarget( bool bReset /* = false */)
{
    if( false == IsTargetSyncObject() )
        return false;

    const std::map<DWORD, SyncObjectInstance*>* pInstance = SiKGCSyncObjectManager()->GetObjectInstancePool();
    if(NULL == pInstance )
        return false;

    std::map<DWORD, SyncObjectInstance*>::const_iterator mapIter = pInstance->find( m_dwTargetSyncObject );
    if( mapIter != pInstance->end() && mapIter->second )
        return true;

    if( bReset )
        m_dwTargetSyncObject = 0;

    return false;
}

int MONSTER::GetSyncObjectInstanceCount( int iSyncObjectID )
{
    return SiKGCSyncObjectManager()->GetInstanceCountToID( iSyncObjectID );
}

void MONSTER::StartBlur( int iMonsterIndex, float fCameraZoom, DWORD dwBlurTime )
{
    SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
    if( pCameraZoom && pCameraZoom->m_bNotBossDieEffect ) 
        return;

    if( m_bBlur ) 
        return;

    m_bBlur = true;
    g_kGlobalValue.m_bFrameLock = true;
    m_dwBlurEndTime = dwBlurTime;
    m_dwBlurStartTime = ::timeGetTime();
    g_kCamera.m_fWideMode = fCameraZoom;
    g_kCamera.SetTargetMonster( m_iMonsterIndex );
}

void MONSTER::EndBlur()
{
    SCameraZoom* pCameraZoom = SiKGCWorldMapManager()->GetCameraZoomInfo( SiKGCRoomManager()->GetGameMode() );
    if( pCameraZoom && pCameraZoom->m_bNotBossDieEffect ) 
        return;

    if( !m_bBlur ) 
        return;

    m_bBlur = false;
    g_kGlobalValue.m_bFrameLock = false;
    m_dwBlurEndTime = 0;
    g_MyD3D->m_KGCBlurMgr.BlurEnd();
    g_kCamera.InitCamera();

    //float fZoomValue = g_pkGameOverlayUI->GetZoomValue();
    //if( 0.0f < fZoomValue )
    //    g_kCamera.m_fWideMode = fZoomValue;
}

void MONSTER::SetTargetPlayerIndex( int iIndex_ )
{
    if( m_bAggroState == false ) {
        m_iTargetPlayerIndex = iIndex_;
    }
}

void MONSTER::OnReverseGravity( float fRangeX, float fDamage, bool bFlyCheck, float fHeight /*= 0.0f*/, float fRangeY /*= 0.0f*/, float fOffSetX /*= 0.0f*/, float fOffSetY /*= 0.0f */ )
{
    if( fHeight == 0.0f )
        fHeight = 0.06f;

    D3DXVECTOR2 vecPos(GetX()+fOffSetX, GetY()+fOffSetY);
    for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
    {
        if( g_MyD3D->IsSameTeam_PlayerAndMonster( i, m_iMonsterIndex ) == false ) 
        {
            ReverseGravity( fRangeX, fDamage, bFlyCheck );
        }
    }

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        fDamage *= 500;
        float fRangeToMonsterX = fRangeX * 1.0f;
        float fRangeToMonsterY = fRangeY * 1.0f;
        float fDamageToLarge = fDamage;

        // ¸ó½ºÅÍµµ ¸®¹ö½º±×¶óºñÆ¼!!
        for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
            MONSTER *pMonster = mit->second;
            if( g_MyD3D->IsSameTeam_MonsterAndMonster( m_iMonsterIndex, mit->first ) ) {
                continue;
            }

            if( 0.0f < fRangeY )
            {
                if( (pMonster->GetX() > vecPos.x - fRangeToMonsterX && pMonster->GetX() < vecPos.x + fRangeToMonsterX) &&
                    (pMonster->GetY() > vecPos.y - fRangeToMonsterY && pMonster->GetY() < vecPos.y + fRangeToMonsterY) && 
                    pMonster->IsLive() )
                {				
                    if( bFlyCheck || ( !bFlyCheck && pMonster->GetIsContact() ) )
                    {   
                        float fRealDamage = fDamage;
                        if( g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].bNoDamageReaction )
                        {
                            fRealDamage = fDamageToLarge;
                        }
                        pMonster->OnMonsterDamage( m_iMonsterIndex, DT_REVERSE_GRAVITY, fRealDamage, ATTACKTYPE_DOWN, ATTACKDIR_UP, 0.0f, fHeight, true, 0, true );
                    }
                }
            }
            else
            {
                if( pMonster->GetX() > vecPos.x - fRangeToMonsterX && pMonster->GetX() < vecPos.x + fRangeToMonsterX && 
                    pMonster->IsLive() )
                {				
                    if( bFlyCheck || ( !bFlyCheck && pMonster->GetIsContact() ) )
                    {   
                        float fRealDamage = fDamage;
                        if( g_kMonsterManager.m_vecMonsterTable[pMonster->m_iMonsterType].bNoDamageReaction )
                        {
                            fRealDamage = fDamageToLarge;
                        }
                        pMonster->OnMonsterDamage( m_iMonsterIndex, DT_REVERSE_GRAVITY, fRealDamage, ATTACKTYPE_DOWN, ATTACKDIR_UP, 0.0f, fHeight, true, 0, true );
                    }
                }
            }
        }
    }
}

D3DXVECTOR2 MONSTER::GetPlayerPos( int iIndex_ )
{
    D3DXVECTOR2 vPos(0.0f, 0.0f);
    if( false == EffectivePlayer( iIndex_ ) )
        return vPos;

    vPos.x = g_MyD3D->MyPlayer[iIndex_]->vPos.x;
    vPos.y = g_MyD3D->MyPlayer[iIndex_]->vPos.y;
    return vPos;
}

bool MONSTER::GetPlayerIsRight( int iIndex_ )
{
    if( false == g_MyD3D->IsPlayerIndex(iIndex_) )
        return false;

    return g_MyD3D->MyPlayer[iIndex_]->GetIsRight();
}

int MONSTER::GetMapTempValue( char *szTemp )
{
    if( NULL == szTemp )
        return 0;

    std::map<std::string, int>::iterator mapIter = m_mapTempValue.find( szTemp );
    if( mapIter != m_mapTempValue.end() )
        return mapIter->second;
    return 0;
}

void MONSTER::SetMapTempValue( char *szTemp, int iValue)
{
    if( NULL == szTemp )
        return;

    m_mapTempValue[ szTemp ] = iValue;
}

#if !defined(__PATH__)
void MONSTER::RenderBox( int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY /*= 0*/ )
{
    // 게임안에서 사용하는 수치로 바꾼다..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    D3DXVECTOR3 lefttop, righttop, leftbottom, rightbottom;
    SETVECTOR3( lefttop, iX1- 1.0f, iY1- 0.25f , 0.4f );
    SETVECTOR3( righttop, iX2- 1.0f, iY1- 0.25f , 0.4f );
    SETVECTOR3( leftbottom, iX1- 1.0f, iY2- 0.25f , 0.4f );
    SETVECTOR3( rightbottom, iX2- 1.0f, iY2- 0.25f , 0.4f );

    g_pGCDeviceManager2->DrawInWorld(NULL,&g_kCamera.m_matCamera,
        lefttop, righttop, leftbottom, rightbottom,0,0,0,
        D3DCOLOR_ARGB(90, 0, 200, 0));
}
#endif

bool MONSTER::CheckSameTeamInRange( int iType, int iNum, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY/* = 0*/ )
{
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    int iCount = 0;
    if ( 0 == iType )
    {
        std::map<int, MONSTER*>::iterator mapIter = g_kMonsterManager.m_mapMonster.begin();
        for(; mapIter != g_kMonsterManager.m_mapMonster.end() ; ++mapIter ) 
        {
            if( NULL == mapIter->second )
                continue;

            if( false == mapIter->second->IsLive() )
                continue;

            if( mapIter->second->IsDie() )
                continue;
            
            if( false == g_MyD3D->IsSameTeam_MonsterAndMonster(m_iMonsterIndex, mapIter->second->m_iMonsterIndex) )
                continue;

            GCAttackRect<float> rect;
            rect.SetRect( iX1 - 1.0f, iY1 - 0.25f, iX2 - 1.0f, iY2 - 0.25f );
            std::vector< GCAttackRect<float> >::iterator vecIter = mapIter->second->m_vecAttackRect.begin();
            for(; vecIter != mapIter->second->m_vecAttackRect.end(); ++vecIter)
            {
                if( rect.CheckCollision( *vecIter, NULL ) )
                    break;
            }

            if( vecIter != mapIter->second->m_vecAttackRect.end() )
                ++iCount;
        }
    }
    else
    {
        for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[iLoop];

            if( pPlayer->m_kUserInfo.bLive == false )
                continue;

            if( pPlayer->m_ePlayerState == PS_DEAD )
                continue;

            if( false == g_MyD3D->IsSameTeam_PlayerAndMonster(iLoop, m_iMonsterIndex) )
                continue;

            if( pPlayer->vPos.x > iX1 && 
                pPlayer->vPos.x < iX2 &&
                pPlayer->vPos.y > iY2 && 
                pPlayer->vPos.y < iY1 )
            {  
                ++iCount;
            }
        }
    }

    if( iNum <= iCount )
        return true;
    return false;
}

bool MONSTER::CheckPosInRange( float fPosX, float fPosY, int iOffsetX, int iLeft, int iTop, int iRight, int iBottom, int iOffsetY )
{
    // °ÔÀÓ¾È¿¡¼­ »ç¿ëÇÏ´Â ¼öÄ¡·Î ¹Ù²Û´Ù..
    float iX1 = (float)iLeft / 400.0f;
    float iY1 = (float)iTop / 400.0f;
    float iX2 = (float)iRight / 400.0f;
    float iY2 = (float)iBottom / 400.0f;

    if( m_abIsRight[ LATENCY_INDEX ] )
    {
        iX1 += (float)iOffsetX / 400.0f;
        iX2 += (float)iOffsetX / 400.0f;
    }
    else
    {
        iX1 -= (float)iOffsetX / 400.0f;
        iX2 -= (float)iOffsetX / 400.0f;
    }

    iY1 += (float)iOffsetY / 400.0f;
    iY2 += (float)iOffsetY / 400.0f;

    iX1 += m_afX[ LATENCY_INDEX ];
    iX2 += m_afX[ LATENCY_INDEX ];
    iY2 += m_afY[ LATENCY_INDEX ];
    iY1 += m_afY[ LATENCY_INDEX ];

    if( fPosX > iX1 && fPosX < iX2 &&
        fPosY > iY2 && fPosY < iY1 )
    {  
        return true;
    }

    return false;
}

bool MONSTER::IsOldGraphicQuality()
{
    return !g_RenderManager->GetUsingVS();
}

PLAYER* MONSTER::GetLocalPlayer()
{
    int iMyIndex = g_MyD3D->Get_MyPlayer();
    if( g_MyD3D->IsPlayerIndex( iMyIndex ) )
        return g_MyD3D->MyPlayer[ iMyIndex ];
    return NULL;
}

void MONSTER::RenderHitNumber( float fChange, bool IsMPDamage, float fHitPosX, float fHitPosY, bool bSend )
{
    g_MyD3D->MyHead->Add_HP_Changer( -1, (m_iMonsterIndex + MAX_PLAYER_NUM), fChange, IsMPDamage, CString(), 1, fHitPosX, fHitPosY );

    if( bSend )
    {
        KGC_PID_BROAD_RENDER_HIT_NUMBER kPacket;
        kPacket.iUnitID = (m_iMonsterIndex + MAX_PLAYER_NUM);
        kPacket.fNumber = fChange;
        kPacket.bMPDamage = IsMPDamage;
        kPacket.fHitPosX = fHitPosX;
        kPacket.fHitPosY = fHitPosY;
        SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
        KGCPC_MEMO( "KGC_PID_BROAD_RENDER_HIT_NUMBER" );
    }
}

bool MONSTER::SetTargetNearPlayer()
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    int iNearPlayer = -1;
    float fMinDistance = 9999.0f;
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( false == EffectivePlayer(i) )
            continue;

        float fDist = DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
        if( fMinDistance > fDist )
        {
            fMinDistance = fDist;
            iNearPlayer = i;
        }
    }

    bool bResult = false;
    if( 0 <= iNearPlayer )
    {
        if ( m_iTargetPlayerIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget( m_iMonsterIndex );

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }

        SetTargetPlayerIndex( iNearPlayer );
        bResult = true;
    }

    return bResult;
}

bool MONSTER::SetTargetFarPlayer()
{
    if( m_dwTargetingDelayTime > 0 )
        return false;

    int iFarPlayer = -1;
    float fMaxDistance = 0.0f;
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        PLAYER* pPlayer = g_MyD3D->MyPlayer[i];

        if( false == EffectivePlayer(i) )
            continue;

        float fDist = DISTANCE( pPlayer->vPos.x, pPlayer->vPos.y, m_afX[LATENCY_INDEX], m_afY[LATENCY_INDEX] );
        if( fMaxDistance < fDist )
        {
            fMaxDistance = fDist;
            iFarPlayer = i;
        }
    }

    bool bResult = false;
    if( 0 <= iFarPlayer )
    {
        if ( m_iTargetPlayerIndex == -1 )
        {
            g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget( m_iMonsterIndex );

            if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
                ShowAwakeMake();
        }

        SetTargetPlayerIndex( iFarPlayer );
        bResult = true;
    }

    return bResult;
}

void MONSTER::Get_BasicMatrix( D3DXMATRIX* pMat, float fOffsetX, float fOffsetY, bool bCamera )
{
    D3DXMATRIX mat2, mat3;
    int iIndex = LATENCY_INDEX;
    if( !g_kGlobalValue.m_kUserInfo.bHost )
        iIndex = 0;

    const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ m_iMonsterType ];

    if( GetIsRight() )
    {
        D3DXMatrixScaling ( pMat, m_fScale, m_fScale, m_fScale );
    }
    else
    {
        D3DXMatrixScaling ( &mat2, m_fScale, m_fScale, m_fScale );
        D3DXMatrixRotationY( &mat3, 3.1415f);
        D3DXMatrixMultiply( pMat, &mat2, &mat3 );
    }

    if( bCamera )
        D3DXMatrixTranslation( &mat2, m_afX[iIndex] - 1.0f + fOffsetX, m_afY[iIndex] - 0.48f + fOffsetY, 1.0f);
    else
        D3DXMatrixTranslation( &mat2, m_afX[iIndex] - 1.0f + fOffsetX, m_afY[iIndex] - 0.23f + fOffsetY, 1.0f);

    D3DXMatrixMultiply( &mat3, pMat, &mat2 );

    if( bCamera )
    {
        D3DXMatrixMultiply( pMat, &mat3, &g_kCamera.m_matCamera );
    }
    else
    {
        *pMat = mat3;
    }
}

void MONSTER::SetRealMiniMap_ScaleAni( int iAniTime, float fMinScale, float fMaxScale, float fSpeed, int iSoundID, bool bReturnAni )
{
    m_kRealMiniMapScaleAniInfo.fMinLimit = fMinScale;
    m_kRealMiniMapScaleAniInfo.fMaxLimit = fMaxScale;
    m_kRealMiniMapScaleAniInfo.fAniSpeed = fSpeed;
    m_kRealMiniMapScaleAniInfo.iKeepFrame = iAniTime;
    m_kRealMiniMapScaleAniInfo.iSoundID = iSoundID;
    m_kRealMiniMapScaleAniInfo.bReturnAni = bReturnAni;    
}

void MONSTER::SetRealMiniMap_AlphaAni( int iAniTime, float fMinAlpha, float fMaxAlpha, float fSpeed, int iSoundID, bool bReturnAni )
{
    m_kRealMiniMapAlphaAniInfo.fMinLimit = fMinAlpha;
    m_kRealMiniMapAlphaAniInfo.fMaxLimit = fMaxAlpha;
    m_kRealMiniMapAlphaAniInfo.fAniSpeed = fSpeed;
    m_kRealMiniMapAlphaAniInfo.iKeepFrame = iAniTime;
    m_kRealMiniMapAlphaAniInfo.iSoundID = iSoundID;
    m_kRealMiniMapAlphaAniInfo.bReturnAni = bReturnAni;
}

void MONSTER::SetRealMiniMap_ColorAni( COLOR_ANIINFO *pColorAniInfo )
{
    m_kRealMiniMapColorAniInfo = (*pColorAniInfo);
}

void MONSTER::SetRealMiniMap_StaticScale( float fScale )
{
    m_fRealMiniMapStaticScale = fScale;
}

void MONSTER::SetRealMiniMap_BaseRender( bool bRender )
{
    if( bRender )
        m_iBaseRender = 1;
    else
        m_iBaseRender = 0;
}

void MONSTER::SetRealMiniMap_AddTexture( char *szFileName, float fX, float fY, float fZ, float fW, DWORD dwColor )
{
    m_kRealMiniMapAccTexInfo.m_strFileName = szFileName;
    m_kRealMiniMapAccTexInfo.m_vUV.x = fX;
    m_kRealMiniMapAccTexInfo.m_vUV.y = fY;
    m_kRealMiniMapAccTexInfo.m_vUV.z = fZ;
    m_kRealMiniMapAccTexInfo.m_vUV.w = fW;
    m_kRealMiniMapAccTexInfo.m_dwColor = dwColor;
}

void MONSTER::SetUserCartoon( int iCartoonIdx )
{
    m_iUserCartoon = iCartoonIdx;
}

void MONSTER::SetUserEdgeColor( float fEdgeAlpha, float fEdgeR, float fEdgeG, float fEdgeB )
{
    m_vUserCartonEdgeColor = D3DXVECTOR4(fEdgeAlpha, fEdgeR, fEdgeG, fEdgeB );
}

void MONSTER::RequestChangeStateToHost( char *szState )
{
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mapIter = mapAction.find( szState );
    if( mapIter == mapAction.end() )
        return;

    const KGCMonsterAction &kMonsterAction = mapIter->second;
    if( static_cast<int>(m_vecMotionList.size()) <= kMonsterAction.m_iMotionIndex || 
        kMonsterAction.m_iMotionIndex < 0 ||
        m_vecMotionList[ kMonsterAction.m_iMotionIndex ] == NULL ||
        m_vecMotionList[ kMonsterAction.m_iMotionIndex ]->IsDisable() )
        return;

    KGC_PID_BROAD_CHANGE_MONSTER_STATE kPacket;
    kPacket.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;
    kPacket.iMonsterID = m_iMonsterType;
    kPacket.iSlotID = m_iMonsterIndex;
    kPacket.iStateID = kMonsterAction.m_iMotionIndex;
    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST, _RELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_CHANGE_MONSTER_STATE" );
}

void MONSTER::SetMonsterStateStat( char *szState )
{
    if( szState == "" )
        return;

    if( m_bDie )
        return;

    std::string strMotion = szState;
    std::map< std::string, KGCMonsterAction >& mapAction = g_kMonsterManager.GetMonsterAction( m_iMonsterType );
    std::map< std::string, KGCMonsterAction >::iterator mit = mapAction.find( strMotion );
    if( mit == mapAction.end() )
        return;

    const KGCMonsterAction &kMonsterAction = mit->second;
    if( (int)m_vecMotionList.size() <= kMonsterAction.m_iMotionIndex || 
        kMonsterAction.m_iMotionIndex < 0 ||
        m_vecMotionList[ kMonsterAction.m_iMotionIndex ] == NULL ||
        m_vecMotionList[ kMonsterAction.m_iMotionIndex ]->IsDisable() )
        return;

    g_kGlobalValue.m_kMonsterTypeStat.m_nModeID = SiKGCRoomManager()->GetGameMode();
    g_kGlobalValue.m_kMonsterTypeStat.m_nDifficult = SiKGCRoomManager()->GetDungeonLevel();

    std::map< std::pair<int,int>,int >::iterator mapStateIter = g_kGlobalValue.m_kMonsterTypeStat.m_mapMonsterTypeCount.find( std::make_pair(m_iMonsterType, kMonsterAction.m_iMotionIndex) );
    if( mapStateIter != g_kGlobalValue.m_kMonsterTypeStat.m_mapMonsterTypeCount.end() )
        ++mapStateIter->second;
    else
        g_kGlobalValue.m_kMonsterTypeStat.m_mapMonsterTypeCount.insert( std::make_pair(std::make_pair(m_iMonsterType, kMonsterAction.m_iMotionIndex), 1) );
}

void MONSTER::SetPermissionDamage( const std::vector<int> &vecArg )
{
    m_vecPermissionDamage.clear();
    m_vecPermissionDamage = vecArg;
}

void MONSTER::SendToHostOfMonsterHP( float fHP )
{
    KGC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST kPacket;
    kPacket.fHP = fHP;
    kPacket.iSlotID = m_iMonsterIndex;
    SendP2PPacket( &kPacket, sizeof(kPacket), KNC_BROADCAST_NOT_ME, _RELIABLE );
    KGCPC_MEMO( "KGC_PID_BROAD_CHANGE_MONSTER_HP_TO_HOST" );
}

void MONSTER::ShowWarninggMessage( int istring_ )
{
    if (g_pkGameOverlayUI->m_pkAlerInDungeon)
        g_pkGameOverlayUI->m_pkAlerInDungeon->SetStaticDesc(istring_);
    else
        g_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( istring_ ) );
}

void MONSTER::SetMonsterTagetEffect()
{
    g_MyD3D->m_pkQuestGameMgr->SetMonsterRefreshTaget(m_iMonsterIndex);

    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
        ShowAwakeMake();
}

void MONSTER::SetMonsterAttackEffect()
{
    AddTraceParticleToBone( "Goblin_Shock_01", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, -(m_fWidth / 2.0f), m_fHeight + 0.05f, true );
    AddTraceParticleToBone( "Ppazik", 0.5f, -1, GC_LAYER_CHAR, GC_LAYER_CHAR, -(m_fWidth / 2.0f), m_fHeight + 0.05f, true );
}

float MONSTER::GetShadowPos( float fX, float fY )
{
    return SiKGCFantasticMap()->FindShadowPos( fX, fY );
}