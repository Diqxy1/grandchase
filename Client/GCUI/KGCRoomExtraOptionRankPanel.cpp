#include "stdafx.h"
#include "KGCRankIcon.h"
#include "KGCRoomExtraOptionRankPanel.h"
#include "KGCRoomListManager.h"


IMPLEMENT_CLASSNAME( KGCRoomExtraOptionRankPanel );
IMPLEMENT_WND_FACTORY( KGCRoomExtraOptionRankPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomExtraOptionRankPanel, "gc_roomextraoptionrankpanel" );

KGCRoomExtraOptionRankPanel::KGCRoomExtraOptionRankPanel( void ) 
: m_pkCloseBtn( NULL )
, m_pkResetBtn( NULL )
, m_pkOkBtn( NULL )
, m_bIsCreatingRoom( false )
{
	LINK_CONTROL( "close_btn", m_pkCloseBtn );
	LINK_CONTROL( "reset_btn", m_pkResetBtn );
	LINK_CONTROL( "ok_btn", m_pkOkBtn );
	
	for( int i = 0; i < GC_BG_GRADE_MAX; i++ )
	{
		m_saRankIcon[i] = NULL;
		LINK_CONTROL_STM( "rank_icon" << i, m_saRankIcon[i] );
	}
}

KGCRoomExtraOptionRankPanel::~KGCRoomExtraOptionRankPanel( void )
{
}

void KGCRoomExtraOptionRankPanel::OnCreate( void )
{
	if( m_pkCloseBtn )
	{
		m_pkCloseBtn->InitState( true, true, this );
		m_pkCloseBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomExtraOptionRankPanel::OnClose);
		m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
	}

	if( m_pkResetBtn )
	{
		m_pkResetBtn->InitState( true, true, this );
		m_pkResetBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomExtraOptionRankPanel::OnReset);
	}

	if( m_pkOkBtn )
	{
		m_pkOkBtn->InitState( true, true, this );
		m_pkOkBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomExtraOptionRankPanel::OnOK);
		m_pkOkBtn->SetHotKey( DIK_RETURN );
	}

	for( int i = 0; i < GC_BG_GRADE_MAX; i++ )
	{
		if( m_saRankIcon[i] )
		{
			m_saRankIcon[i]->InitState( true, true, this );
			m_saRankIcon[i]->ConnectEventProcedure<KGCRoomExtraOptionRankPanel, void>( D3DWE_BUTTON_CLICK, this, boost::bind( &KGCRoomExtraOptionRankPanel::OnClickIcon, _1, i ));
		}
	}
}

void KGCRoomExtraOptionRankPanel::OnCreateComplete()
{
	DWORD dwAllowedRanks = (1 << GC_BG_GRADE_MAX) - 1;
	if( m_bIsCreatingRoom )
	{
		dwAllowedRanks = SiKGCRoomListManager()->GetCreateRoomOptions().m_dwAllowedRankings;
	}
	else
	{
		dwAllowedRanks = SiKGCRoomListManager()->GetRoomListFilterOptions().m_dwAllowedRankings;
	}

	//printf( "dwAllowedRanks = 0x%X\n", dwAllowedRanks);
	for( int i = 0; i < GC_BG_GRADE_MAX; i++ )
	{
		if (g_kGlobalValue.m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOType != KELOUserData::MT_PLACEMENTTEST &&
			(g_MyD3D->m_TempPlayer.GetCurrentChar().kELOUserData.m_ucGrade - 1) != i )
		{
			bool bDisable = (dwAllowedRanks & (1 << i)) != (1 << i);
			//printf("[i = %d] bDisable = %s | %d - %d\n", i, bDisable ? "true" : "false", (1 << i), (dwAllowedRanks & (1 << i)));
			m_saRankIcon[i]->SetDisableIcon( bDisable );
		}
	}
}

void KGCRoomExtraOptionRankPanel::ActionPerformed( const KActionEvent& event )
{
	
}

void KGCRoomExtraOptionRankPanel::OnClose()
{
	g_KDSound.Play( "31" );
	ToggleRender( false );
}

void KGCRoomExtraOptionRankPanel::OnReset()
{
	for( int i = 0; i < GC_BG_GRADE_MAX; i++ )
	{
		m_saRankIcon[i]->SetDisableIcon( false );
	}
}

void KGCRoomExtraOptionRankPanel::OnOK()
{
	DWORD dwAllowedRanks = (1 << GC_BG_GRADE_MAX) - 1;

	for( int i = 0; i < GC_BG_GRADE_MAX; i++ )
	{
		if( m_saRankIcon[i]->IsDisabled() )
		{
			dwAllowedRanks -= (1 << i);
		}
	}

	if( dwAllowedRanks == 0 || dwAllowedRanks >= ( ( 1 << GC_BG_GRADE_MAX ) - 1 ) )
	{
		dwAllowedRanks |= 0xFFFFFFFF;
	}

	KRoomOptions options;
	if( m_bIsCreatingRoom )
	{
		options = SiKGCRoomListManager()->GetCreateRoomOptions();
		options.m_dwAllowedRankings = dwAllowedRanks;
		SiKGCRoomListManager()->SetCreateRoomOptions(options);
	}
	else
	{
		options = SiKGCRoomListManager()->GetRoomListFilterOptions();
		options.m_dwAllowedRankings = dwAllowedRanks;
		SiKGCRoomListManager()->SetRoomListFilterOptions(options);
	}

	OnClose();
}

void KGCRoomExtraOptionRankPanel::OnClickIcon( int iIndex )
{
	if( g_MyD3D->m_TempPlayer.GetCurrentChar().kELOUserData.m_ucGrade - 1 != iIndex )
	{
		m_saRankIcon[iIndex]->SetDisableIcon( !m_saRankIcon[iIndex]->IsDisabled() );
	}
}