#include "stdafx.h"
#include "KGCMissionUI_S4.h"

IMPLEMENT_CLASSNAME( KGCMissionUI_S4 );
IMPLEMENT_WND_FACTORY( KGCMissionUI_S4 );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionUI_S4, "gc_mission_ui_s4" );

KGCMissionUI_S4::KGCMissionUI_S4( void )
: m_pkBnt_Close( NULL )
, m_pkMissionMainDlg( NULL )
, m_pkMissionSubDlg( NULL )
, m_pkCharGetInfoDlg( NULL )
, m_bShowSub( false )
, m_bBefoeCharDlg( false )
, m_bNowCharDlg( false )
{
    LINK_CONTROL( "btn_close", m_pkBnt_Close );
    LINK_CONTROL( "main_mission", m_pkMissionMainDlg );
    LINK_CONTROL( "sub_mission", m_pkMissionSubDlg );
    LINK_CONTROL( "character_get_info", m_pkCharGetInfoDlg );

    m_vecOriPos = D3DXVECTOR2(753.0f * GC_SCREEN_DIV_WIDTH, 41.0f* GC_SCREEN_DIV_WIDTH);
}

KGCMissionUI_S4::~KGCMissionUI_S4( void ){}

bool KGCMissionUI_S4::InitDialog( IN const KDialogInfo& kInfo_ )
{
    m_bShowSub = false;
    if( kInfo_.m_lParam2 != 0 )
    {
        m_bShowSub = true;
        m_pkMissionMainDlg->ToggleRender( false );
        m_pkMissionSubDlg->ToggleRender( true );
        m_pkMissionSubDlg->RefreshSubMission( static_cast<DWORD>(kInfo_.m_lParam) );
    }
    return true;
}

void KGCMissionUI_S4::OnDestroy( void )
{
    m_pkCharGetInfoDlg->ToggleRender( false );
    m_pkCharGetInfoDlg->Destroy();

    while( g_pkUIScene->RemoveWaitMessageBox( KGCUIScene::GC_MBOX_MISSION_UI_S4_DLG ) );

#if defined( USE_SURVEY_SYSTEM )
    if ( g_pkUIScene->m_pkSurveyDlg->IsReserveQuestion() ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SURVEY_DLG );
    }
#endif
}

void KGCMissionUI_S4::OnDestroyComplete()
{
    if( KP2P::GetInstance()->IsConnectedToGameServer() == true )
    {
        if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        {
            KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_WAIT );
        }
    }

}

void KGCMissionUI_S4::OnCreateComplete( void )
{
    if( m_pkBnt_Close )
    {
        m_pkBnt_Close->SetHotKey(DIK_ESCAPE);
        m_pkBnt_Close->InitState( true, true, this);
        m_pkBnt_Close->DisconnectAllEventProcedure();
        m_pkBnt_Close->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMissionUI_S4::OnClickClsoe );
        m_pkBnt_Close->ConnectEventProcedure( KD3DWnd::D3DWE_CURSOR_ENTER, this, &KGCMissionUI_S4::OnEnterCursor );
    }

    if( m_pkCharGetInfoDlg )
    {
        m_pkCharGetInfoDlg->InitState( false, true, this );
        m_pkCharGetInfoDlg->Create();
    }

    if( !m_bShowSub )
    {
        m_pkBnt_Close->SetFixedWindowLocalPos( m_vecOriPos );
        if( m_pkMissionMainDlg )
            m_pkMissionMainDlg->InitState(true, true, this);
        if( m_pkMissionSubDlg )
            m_pkMissionSubDlg->InitState(true, true, this);
    }
    else
    {
        D3DXVECTOR2 vecPos;
        vecPos.x = (410 * GC_SCREEN_DIV_WIDTH- float(m_pkMissionSubDlg->GetWidth()/2.0f)) + (m_pkMissionSubDlg->GetWidth() - m_pkBnt_Close->GetWidth());
        vecPos.y = (300* GC_SCREEN_DIV_WIDTH - float(m_pkMissionSubDlg->GetHeight()/2.0f));
        m_pkBnt_Close->SetFixedWindowLocalPos( vecPos );
    }

    if( KP2P::GetInstance()->IsConnectedToGameServer() == true )
    {
        if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        {
            KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_MISSION );
        }
    }
}

void KGCMissionUI_S4::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkCharGetInfoDlg )
    {
        if( event.m_dwCode == EWNDMESSAGE_CLOSE )
        {
            m_pkCharGetInfoDlg->ToggleRender(false);
            m_pkCharGetInfoDlg->Destroy();
            return;
        }
    }
}

void KGCMissionUI_S4::FrameMoveInEnabledState( void )
{
    if( m_bShowSub && m_pkMissionSubDlg && m_pkMissionSubDlg->GetSubInfoInstance() )
    {
        D3DXVECTOR2 vecBtnPos = m_pkBnt_Close->GetFixedWindowLocalPos();
        D3DXVECTOR2 vecBoxPos = m_pkMissionSubDlg->GetFixedWindowLocalPos();
        vecBoxPos.y += m_pkMissionSubDlg->GetSubInfoInstance()->GetPosY();
        if( vecBoxPos.y != vecBtnPos.y )
        {
            vecBtnPos.y = vecBoxPos.y;
            m_pkBnt_Close->SetFixedWindowLocalPos( vecBtnPos );
        }
    }
}

void KGCMissionUI_S4::PostChildDraw( void )
{
//     g_ParticleManager->FrameMove( g_MyD3D->m_fTime, 0.0018180f );
//     g_MyD3D->RenderUI();
//     g_MyD3D->RenderFPS();
//     g_ParticleManager->Render( GC_LAYER_UI );
}

void KGCMissionUI_S4::OnClickClsoe()
{
    if( !m_pkCharGetInfoDlg->IsRenderOn() )
    {
#if defined(USE_GUIDE_SYSTEM)
        if ( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
        {
            // 가이드 상태조건 갱신
            g_MyD3D->m_kGuideMgr.CheckEnableGuide();
            g_pkUIScene->m_pkGCGuide->ToggleRender( false );
        }
#endif
        SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
        return;
    }
}

void KGCMissionUI_S4::OnEnterCursor()
{
    g_KDSound.Play( "30" );
}

void KGCMissionUI_S4::RefreshUI()
{
    if( m_pkMissionMainDlg )
        m_pkMissionMainDlg->RefreshUI();
}

void KGCMissionUI_S4::ShowMissionComplete( const SMissionReward& sReward )
{
    g_ParticleManager->MissionComplateEffect( 0.55f, -0.05f );

    if( sReward.m_bLevelUp )
    {
        g_ParticleManager->MissionLevelUpEffect( 0.55f, -0.1f );
        g_pkUIScene->OnLevelupInfo();
    }
}

void KGCMissionUI_S4::ShowGetCharacterUI( const SMissionReward& sReward )
{
    if( m_pkCharGetInfoDlg == NULL )
    {
        assert( !"이게 왜 널이야!!!" );
        return;
    }

    m_pkCharGetInfoDlg->Create();
    m_pkCharGetInfoDlg->ToggleRender( true );
    m_pkCharGetInfoDlg->SetShow( true );
    m_sReward = sReward;
}

void KGCMissionUI_S4::SetGetNewCharInfo( bool bNewChar, int iCharType, int iPromotion )
{
    m_pkCharGetInfoDlg->SetInfo(bNewChar, iCharType, iPromotion);

    // 캐릭터획득 및 전직시 후 바로 스킬을 익히면 생기는 버그때문에..ㅠ
    // 이딴짓을해버렸어요..
    // 서버랑 스킬정보가 달라서 그런문제가 생긴듯해서 동기화 한번 해줌..ㅠㅠ
    g_pkUIScene->m_pkSkillTree->OnClickClose();

   // if( bNewChar && ( iCharType == GC_CHAR_DIO || iCharType == GC_CHAR_ZERO || iCharType == GC_CHAR_LEY ) )
   // {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, KGCSkillTreeDlg::OPEN_NEW_CHAR, iCharType, true, false );
   //     g_kGlobalValue.m_kUserInfo.SetCurrentChar( iCharType );
   //     g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_SKILLTREE );
   // }
}