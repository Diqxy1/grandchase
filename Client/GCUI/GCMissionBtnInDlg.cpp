
#include "stdafx.h"
//
#include ".\gcmissionbtnindlg.h"

IMPLEMENT_CLASSNAME( KGCMissionBtnInDlg );
IMPLEMENT_WND_FACTORY( KGCMissionBtnInDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCMissionBtnInDlg, "gc_mission_complete_btn" );

KGCMissionBtnInDlg::KGCMissionBtnInDlg(void)
: m_dwMissionID( 0 )
, m_dwSelectionID( 0 )
{
    m_pkComplete = NULL;
    m_pkFailed = NULL;
    m_pkDisComplete = NULL;
    m_bComplete = false;
    
    LINK_CONTROL( "btn_complete", m_pkComplete );
    LINK_CONTROL( "btn_failed", m_pkFailed );
    LINK_CONTROL( "btn_dis_complete", m_pkDisComplete );
}

KGCMissionBtnInDlg::~KGCMissionBtnInDlg(void)
{
}

void KGCMissionBtnInDlg::OnCreate()
{
    m_pkComplete->InitState( true, true, this );
    m_pkComplete->Activate( false );
    m_pkDisComplete->InitState( false, false, this );

    m_pkFailed->InitState( false, true, this );
    m_dwSelectionID = 0;
    m_bComplete = false;
}

void KGCMissionBtnInDlg::ActionPerformed(const KActionEvent& event )
{
    GCWND_CODE_MAP( m_pkComplete, OnComplete );
    GCWND_CODE_MAP( m_pkFailed, OnFailed );
}

void KGCMissionBtnInDlg::OnComplete( DWORD dwCode )
{
    if( m_bComplete ) return;

    switch( dwCode )
    {
	case KD3DWnd::D3DWE_BUTTON_UP:
    case KD3DWnd::D3DWE_BUTTON_CLICK:
        {
			 // 도저히 보유할 수 없을 정도로 크면 에러 처리
           if(g_pItemMgr->CheckItemCountLimit() == true)
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVENTORY_FULL_ERROR0), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false,true );
              m_bComplete = false;
			}
           else
			{
              m_bComplete = KP2P::GetInstance()->Send_MissionComplete( m_dwMissionID , m_dwSelectionID );
                if( m_bComplete )
                    g_KDSound.Play( "Mission_Complete" );
            }
        }
        break;
    default:
        break;
    }
}

void KGCMissionBtnInDlg::OnFailed( DWORD dwCode )
{
    switch( dwCode )
    {
    case KD3DWnd::D3DWE_BUTTON_CLICK:
        break;
    default:
        break;
    }
}

void KGCMissionBtnInDlg::SetComplete( bool bIsComplete /*= false*/ )
{
    m_pkComplete->ToggleRender( true );
    m_pkDisComplete->ToggleRender( false );
    m_pkFailed->ToggleRender( false );

    if( bIsComplete == true )
    {
        m_pkComplete->ToggleRender( true );
        m_pkDisComplete->ToggleRender( false );
    }
    else
    {
        m_pkComplete->ToggleRender( false );
        m_pkDisComplete->ToggleRender( true );
    }
}

void KGCMissionBtnInDlg::SetFailed( bool bIsFailed /*= true*/ )
{
    m_pkComplete->ToggleRender( false );
    m_pkDisComplete->ToggleRender( false );
    m_pkFailed->ToggleRender( true );
}

void KGCMissionBtnInDlg::SetCompleteToolTip( const std::wstring& strComment )
{
    if( strComment == L"")
        m_pkDisComplete->ClearToolTip();
    else
#if defined(NATION_TAIWAN)
        m_pkDisComplete->SetToolTip(strComment, -150.0f, 26.0f);
#else
        m_pkDisComplete->SetToolTip(strComment, -190.0f, 26.0f);
#endif
}
