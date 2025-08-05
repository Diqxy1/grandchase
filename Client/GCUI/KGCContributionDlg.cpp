#include "stdafx.h"
#include "KGCContributionDlg.h"
#include "KGCContributionManager.h"



// 기부이벤트 1, 2타입은 이 UI를 사용합니다.



IMPLEMENT_CLASSNAME( KGCContributionDlg );
IMPLEMENT_WND_FACTORY( KGCContributionDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCContributionDlg, "gc_contribution_dlg" );

KGCContributionDlg::KGCContributionDlg(void)
: m_pkContributionBtn( NULL )
, m_pkCloseBnt( NULL )
, m_pkEventBarBack( NULL )
{
	for(int i = 0 ; i < MAX_CONTRIBUTION_EVENT; ++i)
	{
        m_pkContributionEventBar[i] = NULL;
		m_pkContributionEventBarActive[i] = NULL;
		m_pkContributionEventName[i] = NULL;
        m_pkContributionEventStamp[i] = NULL;
        m_pkContributionEventStampActive[i] = NULL;

        char szTemp[MAX_PATH] = { 0, };
        sprintf(szTemp, "contribution_event_stamp_%d", i + 1);
        LINK_CONTROL( szTemp, m_pkContributionEventStamp[i]);

		char szTemp1[MAX_PATH] = { 0, };
		sprintf(szTemp1, "contribution_event_stamp_active_%d", i + 1);
		LINK_CONTROL( szTemp1, m_pkContributionEventStampActive[i]);

		char szTemp3[MAX_PATH] = { 0, };
		sprintf(szTemp3, "contribution_event_name_%d", i + 1);
		LINK_CONTROL( szTemp3, m_pkContributionEventName[i] );

		char szTemp4[MAX_PATH] = { 0, };
		sprintf(szTemp4, "contribution_event_bar_%d", i + 1);
		LINK_CONTROL( szTemp4, m_pkContributionEventBar[i]);

        char szTemp5[MAX_PATH] = { 0, };
        sprintf(szTemp5, "contribution_event_bar_active_%d", i + 1);
        LINK_CONTROL( szTemp5, m_pkContributionEventBarActive[i]);
	}

	LINK_CONTROL( "contribution_btn", m_pkContributionBtn );
	LINK_CONTROL( "dlg_close_button", m_pkCloseBnt );
	LINK_CONTROL( "contribution_event_bar_back", m_pkEventBarBack );
}

KGCContributionDlg::~KGCContributionDlg(void)
{

}

void KGCContributionDlg::OnCreate()
{
    m_pkContributionBtn->InitState( true, true, this);
    m_pkCloseBnt->InitState( true, true, this);

	for(int i = 0 ; i < MAX_CONTRIBUTION_EVENT; ++i)
	{
        m_pkContributionEventBar[i]->InitState( false );
        m_pkContributionEventBarActive[i]->InitState( false );
        m_pkContributionEventName[i]->InitState( false );
        m_pkContributionEventStamp[i]->InitState( false );
        m_pkContributionEventStampActive[i]->InitState( false );
	}

    m_pkEventBarBack->InitState( false );
}

void KGCContributionDlg::OnCreateComplete( void )
{
    Update();
}

void KGCContributionDlg::ActionPerformed(const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkCloseBnt,		KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
	GCWND_MSG_MAP( m_pkContributionBtn,		KD3DWnd::D3DWE_BUTTON_CLICK, OnClickDonationBtn );
}

void KGCContributionDlg::OnExit()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCContributionDlg::OnClickDonationBtn()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_CONTRIBUTION_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
}

void KGCContributionDlg::Update()
{
    std::vector< KTongDonationGoalInfo > vecGoalInfo = SiKGCContributionManager()->GetDonationGoalInfo();
    int nCurrentTotalCount = SiKGCContributionManager()->GetDonationTotalCount();
 
    if( !vecGoalInfo.empty() ) {
        std::vector< KTongDonationGoalInfo >::iterator it = vecGoalInfo.begin();

        for( int i = 0 ; it != vecGoalInfo.end(); ++it, ++i ){
            if( i >= MAX_CONTRIBUTION_EVENT ) break;

            m_pkContributionEventBar[i]->ToggleRender( true );
            m_pkContributionEventStamp[i]->ToggleRender( true );
            m_pkContributionEventName[i]->ToggleRender( true );
            m_pkContributionEventName[i]->SetText( g_pkStrLoader->GetReplacedString( STR_ID_DONATION_EVENT_DESC, "il", (*it).m_dwGoalCount, g_pkStrLoader->GetString( (*it).m_dwStrNumber ) ) );

            if( nCurrentTotalCount >= static_cast<int>( (*it).m_dwGoalCount ) ) { 
                m_pkContributionEventStampActive[i]->ToggleRender( true );
                m_pkContributionEventBarActive[i]->ToggleRender( true );
            }

            
        }

        m_pkEventBarBack->ToggleRender( true );
    }
}

void KGCContributionDlg::PostChildDraw()
{
//  #if defined( _DEBUG )
// 	KLuaManager luaMgr;
// 	GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
// 
// 	float x1 = 0.0f, y1 = 1.0f;
// 	LUA_GET_VALUE_DEF( "x1", x1, 1.0f );
// 	LUA_GET_VALUE_DEF( "y1", y1, 1.0f );
// 	float x2 = 0.0f, y2 = 1.0f;
// 	LUA_GET_VALUE_DEF( "x2", x2, 1.0f );
// 	LUA_GET_VALUE_DEF( "y2", y2, 1.0f );
// #endif


	D3DXVECTOR2 vPos = this->GetRelocatedWindowPos();

	D3DXVECTOR2 vPos1( 150.0f * m_fWindowScaleX, 45.0f * m_fWindowScaleY );
//	D3DXVECTOR2 vPos1( x1 * m_fWindowScaleX, y1 * m_fWindowScaleY );
    int nTotalCount = SiKGCContributionManager()->GetDonationTotalCount();
	g_pkUIMgr->RenderNumber( "blue_gradation_num", vPos + vPos1, SiKGCContributionManager()->ConvertNumToCommaUIString( nTotalCount ).c_str() );

	D3DXVECTOR2 vPos2( 150.0f * m_fWindowScaleX, 73.0f * m_fWindowScaleY );
//	D3DXVECTOR2 vPos2( x1 *m_fWindowScaleX, y1 * m_fWindowScaleY  );
    int nMyTotalCount = SiKGCContributionManager()->GetDonationMyTotalCount();
    g_pkUIMgr->RenderNumber( "blue_gradation_num", vPos + vPos1, SiKGCContributionManager()->ConvertNumToCommaUIString( nMyTotalCount ).c_str() );

	D3DXVECTOR2 vPos3( 150.0f * m_fWindowScaleX, 83.0f * m_fWindowScaleY );
	//D3DXVECTOR2 vPos3( x2*m_fWindowScaleX,  y2* m_fWindowScaleY );

	int nMyItemCount = SiKGCContributionManager()->GetDonationMyItemCount();
	g_pkUIMgr->RenderNumber( "blue_gradation_num", vPos + vPos3, SiKGCContributionManager()->ConvertNumToCommaUIString( nMyItemCount ).c_str() );

}
