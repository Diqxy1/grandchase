#include "stdafx.h"
#include "KGCRoomListFilterDlg.h"
#include "KGCRoomExtraOptionCharPanel.h"

IMPLEMENT_CLASSNAME( KGCRoomListFilterDlg );
IMPLEMENT_WND_FACTORY( KGCRoomListFilterDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomListFilterDlg, "gc_roomlistfilterdlg" );

KGCRoomListFilterDlg::KGCRoomListFilterDlg(void)
: m_pkUsePetCheck( NULL )
, m_pkUse4SPCheck( NULL )
, m_pkUseBallanceCheck( NULL )
, m_pkUseRankCheck( NULL )
, m_pkCharSetBtn( NULL )
, m_pkCharModifyBtn( NULL )
, m_pkCharPanel( NULL )
{
	LINK_CONTROL( "close_btn",			m_pkCloseBtn );
	LINK_CONTROL( "reset_btn",			m_pkResetBtn );
	LINK_CONTROL( "ok_btn",				m_pkApplyBtn );
	LINK_CONTROL( "check_use_pet",		m_pkUsePetCheck );
	LINK_CONTROL( "check_use_4sp",		m_pkUse4SPCheck );
	LINK_CONTROL( "check_use_ballance",	m_pkUseBallanceCheck );
	LINK_CONTROL( "check_use_rank",		m_pkUseRankCheck );

	LINK_CONTROL( "char_set_btn",		m_pkCharSetBtn );
	LINK_CONTROL( "char_modify_btn",	m_pkCharModifyBtn );

	for( int i = 0; i < MAX_ROOM_OPTION; i++ )
	{
		m_saExtraOptions[i] = NULL;
		LINK_CONTROL_STM( "static_extra_option" << i, m_saExtraOptions[i] );
	}

	LINK_CONTROL( "char_option_panel", m_pkCharPanel );
}

KGCRoomListFilterDlg::~KGCRoomListFilterDlg(void)
{
}

void KGCRoomListFilterDlg::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkUsePetCheck,			KD3DWnd::D3DWE_BUTTON_CLICK,	m_pkUsePetCheck->ToggleCheck );
	GCWND_MSG_MAP( m_pkUse4SPCheck,			KD3DWnd::D3DWE_BUTTON_CLICK,	m_pkUse4SPCheck->ToggleCheck );
	GCWND_MSG_MAP( m_pkUseBallanceCheck,	KD3DWnd::D3DWE_BUTTON_CLICK,	m_pkUseBallanceCheck->ToggleCheck );
	GCWND_MSG_MAP( m_pkUseRankCheck,		KD3DWnd::D3DWE_BUTTON_CLICK,	m_pkUseRankCheck->ToggleCheck );
}

void KGCRoomListFilterDlg::OnCreate()
{
	if( m_pkCloseBtn )
	{
		m_pkCloseBtn->InitState( true, true, this );
		m_pkCloseBtn->SetHotKey(DIK_ESCAPE);
		m_pkCloseBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListFilterDlg::OnClickCloseBtn);
	}
	
	if( m_pkResetBtn )
	{
		m_pkResetBtn->InitState( true, true, this );
		m_pkResetBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListFilterDlg::OnClickResetBtn);
	}

	if( m_pkApplyBtn )
	{
		m_pkApplyBtn->InitState( true, true, this );
		m_pkApplyBtn->SetHotKey(DIK_RETURN);
		m_pkApplyBtn->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCRoomListFilterDlg::OnClickApplyBtn);
	}

	if( m_pkUsePetCheck )
	{
		m_pkUsePetCheck->InitState( true, true, this );
		m_pkUsePetCheck->SetCheck( false );
	}

	if( m_pkUse4SPCheck )
	{
		m_pkUse4SPCheck->InitState( true, true, this );
		m_pkUse4SPCheck->SetCheck( false );
	}

	if( m_pkUseBallanceCheck )
	{
		m_pkUseBallanceCheck->InitState( true, true, this );
		m_pkUseBallanceCheck->SetCheck( false );
	}

	if( m_pkUseRankCheck )
	{
		m_pkUseRankCheck->InitState( true, true, this );
		m_pkUseRankCheck->SetCheck( false );
	}

	if( m_pkCharSetBtn )
	{
		m_pkCharSetBtn->InitState( false, true, this );
		m_pkCharSetBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomListFilterDlg::OnClickCharPanelSet );
	}

	if( m_pkCharModifyBtn )
	{
		m_pkCharModifyBtn->InitState( false, true, this );
		m_pkCharModifyBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomListFilterDlg::OnClickCharPanelSet );
	}

	for( int i = 0; i < MAX_ROOM_OPTION; i++ )
	{
		if( m_saExtraOptions[i] )
		{
			m_saExtraOptions[i]->InitState(true, true, this);
			m_saExtraOptions[i]->SetText( g_pkStrLoader->GetString(STR_ID_ROOM_EXTRA_OPTION_BLOCKED_PET + i));
		}
	}

	if ( m_pkCharPanel )
	{
		m_pkCharPanel->InitState( false, true, this );
	}
}

void KGCRoomListFilterDlg::OnCreateComplete()
{
	KRoomOptions options = SiKGCRoomListManager()->GetRoomListFilterOptions();

	if( options.m_dwDisabledChars != 0 )
	{
		m_pkCharModifyBtn->ToggleRender(true);
	}
	else
	{
		m_pkCharSetBtn->ToggleRender(true);
	}

	if( ( options.m_dwEnabledOptions & KRoomOptions::BLOCK_PET_OPTION ) == KRoomOptions::BLOCK_PET_OPTION )
	{
		m_pkUsePetCheck->SetCheck( true );
	}

	if( ( options.m_dwEnabledOptions & KRoomOptions::BLOCK_SUPREME_SKILL ) == KRoomOptions::BLOCK_SUPREME_SKILL )
	{
		m_pkUse4SPCheck->SetCheck( true );
	}

	if( ( options.m_dwEnabledOptions & KRoomOptions::ATTRIBUTE_CORRECTION ) == KRoomOptions::ATTRIBUTE_CORRECTION )
	{
		m_pkUseBallanceCheck->SetCheck( true );
	}

	if( options.m_dwAllowedRankings != 0xFFFFFFFF )
	{
		m_pkUseRankCheck->SetCheck( true );
	}
}

void KGCRoomListFilterDlg::OnClickCharPanelSet()
{
	g_KDSound.Play( "31" );
	m_pkCharPanel->ToggleRender( !m_pkCharPanel->IsRenderOn() );
}

void KGCRoomListFilterDlg::OnClickCloseBtn()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCRoomListFilterDlg::OnClickResetBtn()
{
	m_pkUsePetCheck->SetCheck(false);
	m_pkUse4SPCheck->SetCheck(false);
	m_pkUseBallanceCheck->SetCheck(false);
	m_pkUseRankCheck->SetCheck(false);
}

void KGCRoomListFilterDlg::OnClickApplyBtn()
{
	KRoomOptions options = SiKGCRoomListManager()->GetRoomListFilterOptions();
	options.m_dwEnabledOptions = 0;
	if( m_pkUsePetCheck->GetCheck() )
	{
		options.m_dwEnabledOptions |= KRoomOptions::BLOCK_PET_OPTION;
	}

	if( m_pkUse4SPCheck->GetCheck() )
	{
		options.m_dwEnabledOptions |= KRoomOptions::BLOCK_SUPREME_SKILL;
	}

	if( m_pkUseBallanceCheck->GetCheck() )
	{
		options.m_dwEnabledOptions |= KRoomOptions::ATTRIBUTE_CORRECTION;
	}

	if(g_kGlobalValue.m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOType != KELOUserData::MT_PLACEMENTTEST && m_pkUseRankCheck->GetCheck() )
	{
		int iGrade = (g_MyD3D->m_TempPlayer.GetCurrentChar().kELOUserData.m_ucGrade - 1);
		if( iGrade < 0 ) iGrade = 0;

		options.m_dwAllowedRankings = 1 << iGrade;
	}
	else
	{
		options.m_dwAllowedRankings = 0xFFFFFFFF;
	}

	SiKGCRoomListManager()->SetRoomListFilterOptions(options);
	SiKGCRoomListManager()->SaveRoomOptionsFile();
	KP2P::GetInstance()->Send_PvpRoomListSearchReq(options, SiKGCRoomListManager()->GetRoomFilter());
	OnClickCloseBtn();
}