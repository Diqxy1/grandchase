
#include "stdafx.h"
#include "KGCNewTutorialOption.h"
#include "../KGCModeTutorial.h"
#include "../GCStateGame.h"
#include "../GCStateMachine.h"
#include "GCCameraOrtho.h"
#include "Headup Display.h"
#include "Stage.h"
#include "KGCGameBoard.h"
#include "GCGameOverlayUI.h"
#include "KGCPlayerCommon.h"
#include "KGCRoomManager.h"
#include "GCSkillTreeDlg.h"
#include "./MapSystem/KGCLayerManager.h"
#include "GCSkillSlotBox.h"

IMPLEMENT_CLASSNAME( KGCNewTutorialOption );
IMPLEMENT_WND_FACTORY( KGCNewTutorialOption );
IMPLEMENT_WND_FACTORY_NAME( KGCNewTutorialOption, "gc_tutorialoptoin_select_char_new" );

KGCNewTutorialOption::KGCNewTutorialOption( void ) 
: m_bFirstTutorial( true )
, m_bSkillTreeCancle( false )
, m_RenderLoading( 0 )
, m_pTutorialLoadingBG( NULL )
, m_pTutorialLoadingDiscBG( NULL )
, m_pTutorialLoadingChar( NULL )
, m_pTutorialLoadingDisc( NULL )
, m_pkWhiteBackground( NULL )
, m_bSkillTreeClose( true )
, m_iBackgroundAlphaCounter( 0 )
, m_pkSubBtnSkillA( NULL )
, m_pkSubBtnSkillB( NULL )
, m_bIsSelectedSkillB( false )
{
    LINK_CONTROL( "background", m_pkWndBackGround );
    LINK_CONTROL( "button_ok", m_pkButtonOk );
    LINK_CONTROL( "button_cancel", m_pkButtonCancel );
    LINK_CONTROL( "button_skill", m_pkButtonSkill );
    LINK_CONTROL( "char_select_box", m_pkChar );
    LINK_CONTROL( "map_select_box", m_pkMap );
    LINK_CONTROL( "background_white", m_pkWhiteBackground );
    LINK_CONTROL( "sub_btn_skill_a", m_pkSubBtnSkillA );
    LINK_CONTROL( "sub_btn_skill_b", m_pkSubBtnSkillB );
    
    g_kGlobalValue.m_bTutorialReselect = false;
}

KGCNewTutorialOption::~KGCNewTutorialOption( void ){}

void KGCNewTutorialOption::OnCreateComplete( void )
{
    m_pkWndBackGround->InitState( true, false, this );
    m_pkButtonOk->InitState( true, true, this );
    m_pkButtonSkill->InitState( true, true, this );
    m_pkSubBtnSkillA->InitState( false, true, this );
    m_pkSubBtnSkillB->InitState( false, true, this );

    m_pkButtonCancel->InitState( !(g_kGlobalValue.m_kLoginInfo.bNewUser && m_bFirstTutorial ), true, this );

    m_pkChar->InitState(true, true, this );
    m_pkMap->InitState(true, true, this );

    m_pkWhiteBackground->InitState( false, false, this );
   
    m_pkButtonOk->SetHotKey( DIK_RETURN );
    m_pkButtonOk->SetHotKey( DIK_NUMPADENTER );
    m_pkButtonCancel->SetHotKey( DIK_ESCAPE );

#if !defined(SKILL_TREE) || defined(__PANHO__)	
    m_pkButtonSkill->ToggleRender(false);
#else
    if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER ) {
        SiGCSKT()->_D_TutorialAllTrainSkill();
        SiGCSKT()->_D_UnTrainSkill( GC_CHAR_RIN, GC_CHAR_INVALID, SID_RIN2_SELECT_EVIL );
    }
#endif

	if( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER )
		g_MyD3D->MyPlayer[0]->PresetTutorialChars();    

    m_RenderLoading = 0;

    if( m_pTutorialLoadingBG )
        SAFE_RELEASE( m_pTutorialLoadingBG );
    if( m_pTutorialLoadingDiscBG )
        SAFE_RELEASE( m_pTutorialLoadingDiscBG );
    if( m_pTutorialLoadingChar )
        SAFE_RELEASE( m_pTutorialLoadingChar );
    if( m_pTutorialLoadingDisc )
        SAFE_RELEASE( m_pTutorialLoadingDisc );

    RenderSubSkillBtn( false );    
}

void KGCNewTutorialOption::FrameMoveInEnabledState( void )
{
    // R버튼 클릭처리
    if( g_pkInput->BtnDown( KInput::MBRIGHT ) )
    {
        m_pkChar->SetPlayerTap( !m_pkChar->GetPlayerTab() );
        g_KDSound.Play( "31" );
    }
    if( g_pkInput->IsDown( DIK_ESCAPE ) )
    {
        if( m_bSkillTreeCancle ) m_bSkillTreeCancle = false;
        else OnClickClose();

        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
    }

    if( m_bSkillTreeClose == false )
    {
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
        m_iBackgroundAlphaCounter--;

        BYTE alphaBlend = static_cast<BYTE>( static_cast<float>( 0xb0 ) * static_cast<float>( m_iBackgroundAlphaCounter ) / static_cast<float>( 25 ) );
        m_pkWhiteBackground->ForceSetWindowColor( D3DCOLOR_ARGB( alphaBlend, 0x00, 0x00, 0x00 ) );
        
        if( m_iBackgroundAlphaCounter < 0 )
        {
            m_pkWhiteBackground->ToggleRender( false );
            m_bSkillTreeClose = true;
        }
    }
}

void KGCNewTutorialOption::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == this )
    {  return; }

    // 롤오버소리
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {  g_KDSound.Play( "30" ); }

    // 클릭소리
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN ||
        event.m_dwCode == KD3DWnd::D3DWE_RBUTTON_DOWN )
    {  g_KDSound.Play( "31" ); }

    // 캐릭터 UI이벤트 처리
#if defined(SKILL_TREE_OPEN_BY_SERVERSCRIPT)
	int nChar = m_pkChar->GetMyChar() + ( m_pkChar->GetMyJobLevel() * GC_CHAR_NUM );
	std::set<DWORD> setSkill = g_kGlobalValue.GetCharSkillEnable();
#endif

    if( event.m_pWnd == m_pkChar )
    {
        switch( event.m_dwCode )
        {
        case ON_SKILL:
#if defined(SKILL_TREE_OPEN_BY_SERVERSCRIPT)

			if (setSkill.find(nChar) != setSkill.end())
			{
            m_pkButtonSkill->SetWndMode( D3DWM_DEFAULT );
            m_pkButtonSkill->SetSelfInputCheck(true);
			}
			else
			{
				m_pkButtonSkill->SetWndMode( D3DWM_LOCK );
				m_pkButtonSkill->SetSelfInputCheck(false);
			}
#else
			m_pkButtonSkill->SetWndMode( D3DWM_DEFAULT );
			m_pkButtonSkill->SetSelfInputCheck(true);
#endif
            break;
        case OFF_SKILL:
#if defined(SKILL_TREE_OPEN_BY_SERVERSCRIPT)
            if( setSkill.find( nChar ) != setSkill.end() && m_pkChar->GetPlayerTab() == true )
            {
                m_pkButtonSkill->SetWndMode( D3DWM_DEFAULT );
                m_pkButtonSkill->SetSelfInputCheck(false);
            }
            else
            {
                m_pkButtonSkill->SetWndMode( D3DWM_LOCK );
                m_pkButtonSkill->SetSelfInputCheck(false);
            }
#else
            m_pkButtonSkill->SetWndMode( D3DWM_LOCK );
            m_pkButtonSkill->SetSelfInputCheck(false);
#endif
            break;

        }
    }

    // 버튼 클릭함수 연결
    GCWND_MSG_MAP( m_pkButtonOk,     KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOk );
    GCWND_MSG_MAP( m_pkButtonCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
    GCWND_MSG_MAP( m_pkButtonSkill,  KD3DWnd::D3DWE_BUTTON_CLICK, OnClickSkill );
    GCWND_MSG_MAP( m_pkSubBtnSkillA, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickSubBtnSkillA );
    GCWND_MSG_MAP( m_pkSubBtnSkillB, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickSubBtnSkillB );
}

// 연습모드 UI가 닫힐때
void KGCNewTutorialOption::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, CANCLE ) );
}

void KGCNewTutorialOption::PostChildDraw( void )
{
    if( 0 < m_RenderLoading )
    {
        if( m_pTutorialLoadingBG )
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTutorialLoadingBG,
                0, 0, GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX, GC_SCREEN_FLOAT_HEIGHT*m_fWindowScaleY, 0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);
        }

        if( m_pTutorialLoadingChar )
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTutorialLoadingChar,
                0, 0, GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX, GC_SCREEN_FLOAT_HEIGHT*m_fWindowScaleY, 0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);
        }

        if( m_pTutorialLoadingDiscBG )
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTutorialLoadingDiscBG,
                0, 0, GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX, GC_SCREEN_FLOAT_HEIGHT*m_fWindowScaleY, 0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);
        }

        if( m_pTutorialLoadingDisc )
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTutorialLoadingDisc,
                0, 0, GC_SCREEN_FLOAT_WIDTH*m_fWindowScaleX, GC_SCREEN_FLOAT_HEIGHT*m_fWindowScaleY, 0.0f, 0.0f, 1.0f, 1.0f,false,10.0f,0xffffffff);
        }

        if( 0 == --m_RenderLoading )
            LoadingTutorial();
    }
}

// 확인버튼
void KGCNewTutorialOption::OnClickOk(void)
{
    if( 0 == m_RenderLoading )
    {
#if defined( USE_TUTORIAL_LOADING )
        // 로딩 텍스쳐 로드
        char strTextureName[MAX_PATH];

        SAFE_RELEASE( m_pTutorialLoadingDiscBG );
        sprintf( strTextureName, "LT_Disc_BG.dds" );
        m_pTutorialLoadingDiscBG = g_pGCDeviceManager2->CreateTexture( strTextureName );

        SAFE_RELEASE( m_pTutorialLoadingBG );
        sprintf( strTextureName, "LT_BG_%d.dds", m_pkChar->GetMyChar() );
        m_pTutorialLoadingBG = g_pGCDeviceManager2->CreateTexture( strTextureName );

        // 해외팀을 위해 백그라운드를 한장만 사용한다.
        if( NULL == m_pTutorialLoadingBG )
        {
            sprintf( strTextureName, "LT_BG_%d.dds", 0 );
            m_pTutorialLoadingBG = g_pGCDeviceManager2->CreateTexture( strTextureName );
        }

        SAFE_RELEASE( m_pTutorialLoadingChar );
        sprintf( strTextureName, "LT_Char_%d_%d.dds", m_pkChar->GetMyChar(), m_pkChar->GetMyJobLevel() );
        m_pTutorialLoadingChar = g_pGCDeviceManager2->CreateTexture( strTextureName );

        SAFE_RELEASE( m_pTutorialLoadingDisc );
        sprintf( strTextureName, "LT_Disc_%d_%d.dds", m_pkChar->GetMyChar(), m_pkChar->GetMyJobLevel() );
        m_pTutorialLoadingDisc = g_pGCDeviceManager2->CreateTexture( strTextureName );

        m_pkButtonOk->InitState( false, false, NULL );
        m_pkButtonCancel->InitState( false, false, NULL );
        m_pkButtonSkill->InitState( false, false, NULL );
        m_pkMap->InitState( false, false, NULL );
        m_pkChar->InitState( false, false, NULL );
#endif
        m_RenderLoading = 2;
    }
}

void KGCNewTutorialOption::LoadingTutorial(void)
{
    // 게임 실행후 처음으로 시작하는 튜토리얼임을 알 수 있는 변수
    m_bFirstTutorial = false;

    //현재 선택된 캐릭터와 전직 상태를 받아온다.
    m_iMySelCharIndex = m_pkChar->GetMyChar();
    m_iJobState =  m_pkChar->GetMyJobLevel();

    //상대방 (COM)의 캐릭터와 전직 상태를 받아온다.
    m_iExcharMySelCharIndex = m_pkChar->GetExtChar();
    m_iExcharJobState =  m_pkChar->GetExtJobLevel();

    // 채팅 에디트박스에 포커스가 가지 않도록 설정
    ::SetFocus( g_hUIMainWnd );
    g_pkGameOverlayUI->ShowChatToggleMsg(true);
    KGCPlayerCommon::MakeSoundLoadList();

    //////////////////////////////////////////////////////////////////////////
    // 튜토리얼 중인 상태가 아니라면(로비에서 튜토리얼 옵션창을 클릭 한 상태라면)
    // 튜토리얼 모드를 실행하고 캐릭터의 위치를 잡아준다.
    if( GS_GAME != g_MyD3D->m_pStateMachine->GetState() )
    {
        g_MyD3D->m_pStateMachine->GoTutorial();

        if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
        {
            // 기술표를 업데이트 한다.
            KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
            if( pMode )
            {
                pMode->m_emGameStage = m_pkMap->GetMapStage();

                g_MyD3D->MyPlayer[0]->vPos.x = pMode->GetCurStageMyCharPos( pMode->m_emGameStage ).x;
                g_MyD3D->MyPlayer[0]->vPos.y = pMode->GetCurStageMyCharPos( pMode->m_emGameStage ).y;
                g_MyD3D->MyPlayer[1]->vPos.x = pMode->GetCurStageTutorialCharPos( pMode->m_emGameStage ).x;
                g_MyD3D->MyPlayer[1]->vPos.y = pMode->GetCurStageTutorialCharPos( pMode->m_emGameStage ).y;

                pMode->CheckTutCharDirection();
            }

            if(g_kGlobalValue.m_kLoginInfo.bNewUser )
            {
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TUTORIAL_GUIDE_STRING1 ), KGCChatManager::CHAT_TYPE_ADMIN );
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TUTORIAL_GUIDE_STRING2 ), KGCChatManager::CHAT_TYPE_ADMIN );
                g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString( STR_ID_TUTORIAL_TOGGLE ), KGCChatManager::CHAT_TYPE_ADMIN);
            }
        }

        // 로딩씬 제거
        SAFE_RELEASE( m_pTutorialLoadingBG );
        SAFE_RELEASE( m_pTutorialLoadingDiscBG );
        SAFE_RELEASE( m_pTutorialLoadingChar );
        SAFE_RELEASE( m_pTutorialLoadingDisc );

        SpeakToActionListener( KActionEvent( this, STARTTUTORIAL ) );
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // 튜토리얼이 이미 실행중이라면 튜토리얼을 다시 로드한다.
    LoadTutorial();

    // 기술표를 업데이트 한다.
    KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
    if( pMode )
    {  pMode->InitAtGame(); }

    // 로딩씬 제거
    SAFE_RELEASE( m_pTutorialLoadingBG );
    SAFE_RELEASE( m_pTutorialLoadingDiscBG );
    SAFE_RELEASE( m_pTutorialLoadingChar );
    SAFE_RELEASE( m_pTutorialLoadingDisc );

    SpeakToActionListener( KActionEvent( this, STARTTUTORIAL ) );
}

// 스킬
void KGCNewTutorialOption::OnClickSkill(void)
{    
    if( m_pkChar->GetMyChar() == GC_CHAR_RIN ) {
        RenderSubSkillBtn( true );
    } else {
        //g_pkUIScene->m_pkSkillTree->UpdatePreviewSkill( true );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true ); 
        g_pkUIScene->m_pkSkillTree->SetSelectedChar( m_pkChar->GetMyChar(), m_pkChar->GetMyJobLevel(), true );
        g_pkUIScene->Stop( true );

        if( GS_GAME == g_MyD3D->m_pStateMachine->GetState() )
            g_pkUIScene->DeleteWaitMsgBox( KGCUIScene::GC_MBOX_TUTORIAL_OPTION_NEW );

        ToggleSkillTreeBackGround( true );
    }        
}

// 취소
void KGCNewTutorialOption::OnClickClose(void)
{
    SpeakToActionListener( KActionEvent( this, CANCLE ) );
    
    //g_pkUIScene->m_pkGameOverlayUI->Initialize();

    if ( g_kGlobalValue.m_bTutorialReselect ==  true && (m_iMySelCharIndex !=m_pkChar->GetMyChar() || m_pkChar->GetMyJobLevel() != m_iJobState ))
        InitTutorialChar();

    if ( g_MyD3D->m_pStateMachine->GetState() == GS_SERVER) {
        g_kGlobalValue.SetPlayerSkillInfo( &g_MyD3D->m_TempPlayer, false, true, true );
	}
}

// 튜토리얼 모드 실행
void KGCNewTutorialOption::LoadTutorial(void)
{
    // 이전 스테이지 초기화
    g_pStateGame->ClearStageResource();    

    // 스테이지 로드
    const bool bSameStage = (g_MyD3D->MyStg->GetStage() == m_pkMap->GetMapStage());
    g_MyD3D->MyStg->SetStage( m_pkMap->GetMapStage() ); // Stage 초기화
    g_MyD3D->MyStg->Load_Stage();

    // 캐릭터들 초기화
    g_MyD3D->MyHead->InitDataForTutorial();

    // 같은 스테이지라면 이전 위치를 그대로 사용한다.
    const D3DXVECTOR3 vMyBeforePos = g_MyD3D->MyPlayer[0]->vPos;
    const D3DXVECTOR3 vTutorialBeforePos = g_MyD3D->MyPlayer[1]->vPos;
   
    ReleaseCharResource();

    InitTutorialChar();
    if( bSameStage )
    {
        g_MyD3D->MyPlayer[0]->vPos = vMyBeforePos;
        g_MyD3D->MyPlayer[0]->vPos.y += 0.3f;
        g_MyD3D->MyPlayer[1]->vPos = vTutorialBeforePos;
        g_MyD3D->MyPlayer[1]->vPos.y += 0.3f;
    }

    // 캐릭터를 서로 마주보게 설정한다.
    if( g_MyD3D->MyPlayer[0]->vPos.x <= g_MyD3D->MyPlayer[1]->vPos.x )
    {
        g_MyD3D->MyPlayer[0]->bIsRight = true;
        g_MyD3D->MyPlayer[0]->x_Speed = 0.001f;
        g_MyD3D->MyPlayer[1]->bIsRight = false;
        g_MyD3D->MyPlayer[1]->x_Speed = -0.001f;
    }
    else
    {
        g_MyD3D->MyPlayer[0]->bIsRight = false;
        g_MyD3D->MyPlayer[0]->x_Speed = -0.001f;
        g_MyD3D->MyPlayer[1]->bIsRight = true;
        g_MyD3D->MyPlayer[1]->x_Speed = 0.001f;
    }

    g_KDSound.FullLoadfromLoadList();
}

void KGCNewTutorialOption::InitTutorialChar(void)
{
    // 캐릭터 생성
    SetTutorialCharacter();
    
        g_pGameBoard->m_pStartingMember[0].dwUID = g_MyD3D->MyPlayer[0]->m_kUserInfo.dwUID;
        g_pGameBoard->m_pStartingMember[0].IsLive = true;
        g_pGameBoard->m_pStartingMember[1].dwUID = g_MyD3D->MyPlayer[1]->m_kUserInfo.dwUID;  
        g_pGameBoard->m_pStartingMember[1].IsLive = true;

        for(int i=0;i<LATENCY*2;i++)
        {
            g_kLocalPlayerLatency[0].kRenderData[i].usMotion = g_MyD3D->MyPlayer[0]->GetWaitMotion();
            g_kLocalPlayerLatency[0].kRenderData[i].cFrame = i;
        }

        // 카메라 타겟 설정
        g_kCamera.SetTargetPlayer(0);

        // 현재의 게임 모드가 튜토리얼 모드인지 확인하고 GameModeInterface의 ptr을 받아서 쓴다.
        if( GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode() )
        {
            KGCModeTutorial* pMode = static_cast<KGCModeTutorial*>(g_pStateGame->GetCurGameModePtr());
            if( pMode )
            {
                pMode->m_emGameStage = m_pkMap->GetMapStage();

                // 같은 스테이지면 이전위치를 그대로 넣어준다.
                g_MyD3D->MyPlayer[0]->vPos.x = pMode->GetCurStageMyCharPos(pMode->m_emGameStage).x;
                g_MyD3D->MyPlayer[0]->vPos.y = pMode->GetCurStageMyCharPos(pMode->m_emGameStage).y;

                g_MyD3D->MyPlayer[1]->vPos.x = pMode->GetCurStageTutorialCharPos(pMode->m_emGameStage).x;
                g_MyD3D->MyPlayer[1]->vPos.y = pMode->GetCurStageTutorialCharPos(pMode->m_emGameStage).y;

                // 캐릭터 인덱스 설정변경 및 캐릭터 기술표 변경
                pMode->SetCharType(g_MyD3D->MyPlayer[0]->m_kUserInfo.cCharIndex);
                pMode->CheckTutCharDirection();
            }
        }
        g_MyD3D->MyPlayer[0]->UpdatePlayerResource();
        g_MyD3D->MyPlayer[1]->UpdatePlayerResource();
        SetTutorialCharSound(); 
}

bool KGCNewTutorialOption::InitDialog( IN const KDialogInfo& kInfo_ )
{
	return true;
}

void KGCNewTutorialOption::SetTutorialCharSound()
{
    g_MyD3D->MyPlayer[0]->MakeSoundLoadList();
    g_MyD3D->MyPlayer[1]->MakeSoundLoadList();
}

void KGCNewTutorialOption::ReleaseCharResource()
{
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        g_MyD3D->MyPlayer[i]->ReleasePlayerResource();
        g_MyD3D->MyPlayer[i]->MetamorphosisForm = FORM_NORMAL;
        g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive = false;
    }

    // 모션과 모델을 모두 지운다. 아니면 메모리가 캐릭터 수만큼(모델 및 모션만큼) 늘어난다.
    // 메모리가 허용범위가 높다면 궂이 안해도 된다.
    g_pGraphicsHelper->UnLoadPlayerActionMotion();
    g_pGraphicsHelper->UnLoadPlayerModel();
}

void KGCNewTutorialOption::SetTutorialCharacter()
{
    int iMyChar= m_iMySelCharIndex * NUM_JOB_LEVEL + m_iJobState;    
    int iExtChar= m_iExcharMySelCharIndex * NUM_JOB_LEVEL + m_iExcharJobState;

    g_MyD3D->MyPlayer[0]->PresetTutorialChars();
    g_MyD3D->MyPlayer[0]->InitTutorialChar( iMyChar, m_iMySelCharIndex + GC_CHAR_NUM * m_iJobState );
    g_MyD3D->MyPlayer[0]->SetPlayerName(GCFUNC::GetCharName( m_iMySelCharIndex ).c_str(), L"00FF00");
    g_MyD3D->MyPlayer[0]->m_cLife = 1;
    g_MyD3D->MyPlayer[0]->m_kUserInfo.iTeam = 0;
    g_MyD3D->MyPlayer[0]->m_iOriginalTeam = 0;
    g_MyD3D->MyPlayer[0]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 100;
    g_MyD3D->MyPlayer[0]->m_pObject->SetOutlineColor( EDGE_COLOR_FRIEND );

    for(int i=0;i<FORM_NUM;++i)
    {
        if( g_MyD3D->MyPlayer[0]->m_pObjMetaForm[i] )
            g_MyD3D->MyPlayer[0]->m_pObjMetaForm[i]->SetOutlineColor( EDGE_COLOR_FRIEND );
    }

    g_MyD3D->MyPlayer[1]->PresetTutorialChars();
    g_MyD3D->MyPlayer[1]->InitTutorialChar( iExtChar, m_iExcharMySelCharIndex + GC_CHAR_NUM *  m_iExcharJobState );        
#ifdef NATION_KOREA
    g_MyD3D->MyPlayer[1]->SetPlayerName( L"연습용 캐릭터");
#else
    g_MyD3D->MyPlayer[1]->SetPlayerName( L"First Chase", L"FF0000");
#endif

    g_MyD3D->MyPlayer[1]->m_kUserInfo.iTeam = 1;
    g_MyD3D->MyPlayer[1]->m_iOriginalTeam = 1;
    g_MyD3D->MyPlayer[1]->m_kUserInfo.dwUID = g_MyD3D->MyPlayer[0]->m_kUserInfo.dwUID+1;
    g_MyD3D->MyPlayer[1]->m_cLife = 1;
    g_MyD3D->MyPlayer[1]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 100;
    g_MyD3D->MyPlayer[1]->m_pObject->SetOutlineColor( EDGE_COLOR_ENEMY );
    g_MyD3D->MyPlayer[1]->m_iOriginalTeam = g_MyD3D->MyPlayer[1]->m_kUserInfo.iTeam;
    for(int i=0;i<FORM_NUM;++i)
    {
        if( g_MyD3D->MyPlayer[1]->m_pObjMetaForm[i] )
            g_MyD3D->MyPlayer[1]->m_pObjMetaForm[i]->SetOutlineColor( EDGE_COLOR_FRIEND );
    }
}

void KGCNewTutorialOption::ToggleSkillTreeBackGround( bool bRender )
{
    if( bRender )
    {
        m_pkWhiteBackground->ToggleRender( true );
        m_pkWhiteBackground->ForceSetWindowColor( D3DCOLOR_ARGB( 0xb0, 0x00, 0x00, 0x00 ) );
        m_iBackgroundAlphaCounter = 25;
    }

}
void KGCNewTutorialOption::RenderSubSkillBtn( bool bRender_ )
{
    if( m_pkSubBtnSkillA != NULL && m_pkSubBtnSkillB != NULL ) {
        m_pkSubBtnSkillA->ToggleRender( bRender_ );
        m_pkSubBtnSkillB->ToggleRender( bRender_ );
    }    
}

void KGCNewTutorialOption::OnClickSubBtnSkillA()
{
    // 이전 선택된 녀석 체크해서 스킬 초기화 해준다.
    if( m_bIsSelectedSkillB == true ) {
        g_kGlobalValue.SetPlayerSkillInfo( g_MyD3D->MyPlayer[0], true, false, true );
        SiGCSKT()->_D_TutorialAllTrainSkill();
    }

    g_pkUIScene->m_pkSkillTree->m_bIsSelectedSkillB = false;
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true ); 
    g_pkUIScene->m_pkSkillTree->SetSelectedChar( m_pkChar->GetMyChar(), m_pkChar->GetMyJobLevel(), true );

    if ( SiGCSKT()->IsUnlockAll() ) {
        SiGCSKT()->_D_TrainSkill( static_cast< EGCCharType >( m_pkChar->GetMyChar() ), GC_CHAR_INVALID, SID_RIN2_SELECT_GODDESS );
        SiGCSKT()->_D_UnTrainSkill( static_cast< EGCCharType >( m_pkChar->GetMyChar() ), GC_CHAR_INVALID, SID_RIN2_SELECT_EVIL );
        g_pkUIScene->m_pkSkillTree->ThreadSafeUpdate();
    }

    g_pkUIScene->Stop( true );

    if( GS_GAME == g_MyD3D->m_pStateMachine->GetState() )
        g_pkUIScene->DeleteWaitMsgBox( KGCUIScene::GC_MBOX_TUTORIAL_OPTION_NEW );    

    m_bIsSelectedSkillB = false;
    RenderSubSkillBtn( false );
    ToggleSkillTreeBackGround( true );
}

void KGCNewTutorialOption::OnClickSubBtnSkillB()
{
    // 이전 선택된 녀석 체크해서 스킬 초기화 해준다.
    if( m_bIsSelectedSkillB == false ) {
        g_kGlobalValue.SetPlayerSkillInfo( g_MyD3D->MyPlayer[0], true, false, true );
        SiGCSKT()->_D_TutorialAllTrainSkill();
    }

    g_pkUIScene->m_pkSkillTree->m_bIsSelectedSkillB = true;
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL,0,0, true, true ); 
    g_pkUIScene->m_pkSkillTree->SetSelectedChar( m_pkChar->GetMyChar(), m_pkChar->GetMyJobLevel(), true );

    if ( SiGCSKT()->IsUnlockAll() ) {
        SiGCSKT()->_D_TrainSkill( static_cast< EGCCharType >( m_pkChar->GetMyChar() ), GC_CHAR_INVALID, SID_RIN2_SELECT_EVIL );
        SiGCSKT()->_D_UnTrainSkill( static_cast< EGCCharType >( m_pkChar->GetMyChar() ), GC_CHAR_INVALID, SID_RIN2_SELECT_GODDESS );
        g_pkUIScene->m_pkSkillTree->ThreadSafeUpdate();
    }

    g_pkUIScene->Stop( true );

    if( GS_GAME == g_MyD3D->m_pStateMachine->GetState() )
        g_pkUIScene->DeleteWaitMsgBox( KGCUIScene::GC_MBOX_TUTORIAL_OPTION_NEW );

    m_bIsSelectedSkillB = true;
    RenderSubSkillBtn( false );
    ToggleSkillTreeBackGround( true );
}