#include "StdAfx.h"

#include "GCCoupleViewer.h"
#include "GCSizingThreeBox.h"
#include "GCCoupleWnd.h"
#include "GCCoupleInventory.h"
#include "GCCoupleSystem.h"
#include "MyD3D.h"
#include "GCStateMachine.h"

IMPLEMENT_CLASSNAME( KGCCoupleViewer );
IMPLEMENT_WND_FACTORY( KGCCoupleViewer );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleViewer, "gc_couple_viewer" );

KGCCoupleViewer::KGCCoupleViewer(void)
: m_iCurrentTab( CV_COUPLE_WINDOW )
, m_bFirst( true )
, m_bMyCouple( true )
, m_iCoupleDayFullLength( 100 )
{
	m_pkClose = NULL;
	LINK_CONTROL( "btn_close",	m_pkClose );

	m_pkActiveTab[CV_COUPLE_WINDOW] = NULL;
	m_pkActiveTab[CV_COUPLE_INVEN] = NULL;
	m_pkTab[CV_COUPLE_WINDOW] = NULL;
	m_pkTab[CV_COUPLE_INVEN] = NULL;
	m_pkTitle[CV_COUPLE_WINDOW] = NULL;
	m_pkTitle[CV_COUPLE_INVEN] = NULL;

	m_pkCoupleDayGuage = NULL;
	m_pkCoupleWnd = NULL;
	m_pkCoupleInven = NULL;
	m_pkCoupleDayGuageBg = NULL;

	LINK_CONTROL( "tab_couple_act",		m_pkActiveTab[CV_COUPLE_WINDOW] );
	LINK_CONTROL( "tab_inventory_act",	m_pkActiveTab[CV_COUPLE_INVEN] );
	LINK_CONTROL( "tab_couple",			m_pkTab[CV_COUPLE_WINDOW] );
	LINK_CONTROL( "tab_inventory",		m_pkTab[CV_COUPLE_INVEN] );
	LINK_CONTROL( "title_couple_wnd",	m_pkTitle[CV_COUPLE_WINDOW] );
	LINK_CONTROL( "title_inventory",	m_pkTitle[CV_COUPLE_INVEN] );

	LINK_CONTROL( "guage_heart",		m_pkHeart[CV_NORMAL_HEART] );
	LINK_CONTROL( "guage_heart_act",	m_pkHeart[CV_ACTIVE_HEART] );

	LINK_CONTROL( "bg_guage",			m_pkCoupleDayGuage );
	LINK_CONTROL( "bg_guage_forward",	m_pkCoupleDayGuageBg );
	LINK_CONTROL( "couple_view",		m_pkCoupleWnd );
	LINK_CONTROL( "couple_inven",		m_pkCoupleInven );

}

KGCCoupleViewer::~KGCCoupleViewer(void)
{
}

void KGCCoupleViewer::OnCreate( void )
{
	m_pkClose->InitState( true, true, this );


	m_pkHeart[CV_NORMAL_HEART]->InitState( true, true, this );
	m_pkHeart[CV_ACTIVE_HEART]->InitState( false, true, this );

	m_pkCoupleDayGuage->InitState( true, true, this );
	m_pkCoupleDayGuageBg->InitState( true );

	m_pkCoupleWnd->InitState( true, true, this );
	m_pkCoupleInven->InitState( false, true, this );

	if( m_bFirst )
	{
		m_iCoupleDayFullLength = m_pkCoupleDayGuage->GetWidth();
		m_bFirst = false;
	}
	Update();

	if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
	{
		KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_COUPLEVIEWER );
	}
	SiKGCCoupleSystem()->SetState( KGCCoupleSystem::GCCS_COUPLE_WND );
}

void KGCCoupleViewer::OnDestroy( void )
{
	if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
	{
		KP2P::GetInstance()->Send_Into_Myinfo( g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_WAIT );
	}
	SiKGCCoupleSystem()->SetState( KGCCoupleSystem::GCCS_COUPLE );
}

void KGCCoupleViewer::ActionPerformed( const KActionEvent& event )
{
	if( event.m_pWnd == m_pkClose && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}

	for( int i = 0 ; i < CV_COUPLE_TAB ; ++i )
	{
		if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
			continue;
		
		if( event.m_pWnd != m_pkTab[i] )
			continue;

		OnClickTab( i );
		break;
	}
}

void KGCCoupleViewer::OnClickTab( int iTabIndex )
{
	for( int i = 0 ; i < CV_COUPLE_TAB ; ++i )
	{
		m_pkActiveTab[i]->ToggleRender( iTabIndex == i );
		m_pkTab[i]->ToggleRender( iTabIndex != i );
		m_pkTitle[i]->ToggleRender( iTabIndex == i );
	}

	m_pkCoupleWnd->ToggleRender( CV_COUPLE_WINDOW == iTabIndex );
	m_pkCoupleInven->ToggleRender( CV_COUPLE_INVEN == iTabIndex );
}

void KGCCoupleViewer::SetCoupleUID( DWORD dwLeftUID, DWORD dwRightUID )
{
	m_dwLeftUID = dwLeftUID;
	m_dwRightUID = dwRightUID;
}

void KGCCoupleViewer::Update()
{
	m_pkCoupleWnd->Update();
	m_pkCoupleInven->Update();


	int iCoupleDay = ( (m_bMyCouple? SiKGCCoupleSystem()->GetMyCoupleDay() : SiKGCCoupleSystem()->GetYourCoupleDay() ) - 1 ) % 100 + 1;

	m_pkCoupleDayGuage->SetWidth( (DWORD)(m_iCoupleDayFullLength * ((float)iCoupleDay / 100.0f)) );
}

void KGCCoupleViewer::UseOtherCoupleView()
{
	m_bMyCouple = false;
	
	m_pkActiveTab[CV_COUPLE_WINDOW]->InitState( false );
	m_pkActiveTab[CV_COUPLE_INVEN]->InitState( false );
	m_pkTab[CV_COUPLE_WINDOW]->InitState( false );
	m_pkTab[CV_COUPLE_INVEN]->InitState( false );
	m_pkTitle[CV_COUPLE_WINDOW]->InitState( true, true, this );
	m_pkTitle[CV_COUPLE_INVEN]->InitState( false, true, this );

	m_pkCoupleDayGuageBg->SetToolTip( g_pkStrLoader->GetReplacedString( STR_ID_COUPLE_SYSTEM39, "i", SiKGCCoupleSystem()->GetYourCoupleDay() ));

	m_pkCoupleWnd->SetOtherCoupleWnd();
}

void KGCCoupleViewer::UseMyCoupleView()
{
	m_bMyCouple = true;

	m_pkActiveTab[CV_COUPLE_WINDOW]->InitState( true, true, this );
	m_pkActiveTab[CV_COUPLE_INVEN]->InitState( false, true, this );
	m_pkTab[CV_COUPLE_WINDOW]->InitState( false, true, this );
	m_pkTab[CV_COUPLE_INVEN]->InitState( true, true, this );
	m_pkTitle[CV_COUPLE_WINDOW]->InitState( true, true, this );
	m_pkTitle[CV_COUPLE_INVEN]->InitState( false, true, this );

	m_pkCoupleDayGuageBg->SetToolTip( g_pkStrLoader->GetReplacedString( STR_ID_COUPLE_SYSTEM39, "i", SiKGCCoupleSystem()->GetMyCoupleDay() ));

	OnClickTab( CV_COUPLE_WINDOW );
	m_pkCoupleWnd->SetMyCoupleWnd();
}

void KGCCoupleViewer::FrameMoveInEnabledState( void )
{
	if(g_pkInput->IsDown( DIK_ESCAPE )
#ifdef USE_JOYSTICK
        || g_MyD3D->GetJoystickKey() == EJK_KEY_SELECT
#endif
        )
	{
        SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
	}

	// 내가 커플창을 보고 있는데 솔로 체크가 되었다면 이미 상대방이 헤어지자고 한거임.. 창을 닫아주자!!
	if( SiKGCCoupleSystem()->Am_I_Solo() && m_bMyCouple == true )
	{
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}
}

bool KGCCoupleViewer::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    if( this->GetWndState() != KD3DWnd::D3DWS_ENABLED )
    {
        switch( kInfo_.m_nMsgBoxUse )
        {
        case KGCUIScene::GC_MBOX_USE_COUPLE_VIEWER_OTHERS:
            this->UseOtherCoupleView();
            break;
        default:
            this->UseMyCoupleView();
            break;
        }
    }
    return true;
}