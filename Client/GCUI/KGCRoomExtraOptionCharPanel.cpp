#include "stdafx.h"
#include "GCCharIcon.h"
#include "KGCRoomExtraOptionCharPanel.h"
#include "KGCRoomListManager.h"


IMPLEMENT_CLASSNAME( KGCRoomExtraOptionCharPanel );
IMPLEMENT_WND_FACTORY( KGCRoomExtraOptionCharPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomExtraOptionCharPanel, "gc_roomextraoptioncharpanel" );

KGCRoomExtraOptionCharPanel::KGCRoomExtraOptionCharPanel( void ) 
: m_pkCloseBtn( NULL )
, m_pkResetBtn( NULL )
, m_pkOkBtn( NULL )
, m_bIsCreatingRoom( false )
{
	LINK_CONTROL( "close_btn", m_pkCloseBtn );
	LINK_CONTROL( "reset_btn", m_pkResetBtn );
	LINK_CONTROL( "ok_btn", m_pkOkBtn );
	
	for( int i = 0; i < GC_CHAR_NUM; i++ )
	{
		m_saCharactersIcon[i] = NULL;
		LINK_CONTROL_STM( "char" << i, m_saCharactersIcon[i] );
	}
}

KGCRoomExtraOptionCharPanel::~KGCRoomExtraOptionCharPanel( void )
{
}

void KGCRoomExtraOptionCharPanel::OnCreate( void )
{
	if( m_pkCloseBtn )
	{
		m_pkCloseBtn->InitState( true, true, this );
		m_pkCloseBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomExtraOptionCharPanel::OnClose);
		m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
	}

	if( m_pkResetBtn )
	{
		m_pkResetBtn->InitState( true, true, this );
		m_pkResetBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomExtraOptionCharPanel::OnReset);
	}

	if( m_pkOkBtn )
	{
		m_pkOkBtn->InitState( true, true, this );
		m_pkOkBtn->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCRoomExtraOptionCharPanel::OnOK);
		m_pkOkBtn->SetHotKey( DIK_RETURN );
	}

	for( int i = 0; i < GC_CHAR_NUM; i++ )
	{
		if( m_saCharactersIcon[i] )
		{
			m_saCharactersIcon[i]->InitState( true, true, this );
			m_saCharactersIcon[i]->ConnectEventProcedure<KGCRoomExtraOptionCharPanel, void>( D3DWE_BUTTON_CLICK, this, boost::bind( &KGCRoomExtraOptionCharPanel::OnClickIcon, _1, i ));
		}
	}
}

void KGCRoomExtraOptionCharPanel::OnCreateComplete()
{
	DWORD dwDisabledChars = 0;
	if( m_bIsCreatingRoom )
	{
		//printf("[KGCRoomExtraOptionCharPanel] Create\n");
		dwDisabledChars = SiKGCRoomListManager()->GetCreateRoomOptions().m_dwDisabledChars;
	}
	else
	{
		//printf("[KGCRoomExtraOptionCharPanel] filter\n");
		dwDisabledChars = SiKGCRoomListManager()->GetRoomListFilterOptions().m_dwDisabledChars;
	}

	//printf( "dwDisabledChars = 0x%X\n", dwDisabledChars);
	for( int i = 0; i < GC_CHAR_NUM; i++ )
	{
		if ( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType != i )
		{
			bool bDisable = (dwDisabledChars & (1 << i)) == (1 << i);
			//printf("[i = %d] bDisable = %s | %d - %d\n", i, bDisable ? "true" : "false", (1 << i), (dwDisabledChars & (1 << i)));
			m_saCharactersIcon[i]->SetSelIcon( bDisable );
		}
	}
}

void KGCRoomExtraOptionCharPanel::ActionPerformed( const KActionEvent& event )
{
	
}

void KGCRoomExtraOptionCharPanel::OnClose()
{
	g_KDSound.Play( "31" );
	ToggleRender( false );
}

void KGCRoomExtraOptionCharPanel::OnReset()
{
	for( int i = 0; i < GC_CHAR_NUM; i++ )
	{
		m_saCharactersIcon[i]->SetSelIcon( false );
	}
}

void KGCRoomExtraOptionCharPanel::OnOK()
{
	DWORD dwDisabledChars = 0;

	for( int i = 0; i < GC_CHAR_NUM; i++ )
	{
		if( m_saCharactersIcon[i]->IsSelected() )
		{
			dwDisabledChars += (1 << i);
		}
	}

	if( dwDisabledChars > ( ( 1 << GC_CHAR_NUM ) - 1 ) )
	{
		dwDisabledChars = 0;
	}

	KRoomOptions options;
	if( m_bIsCreatingRoom )
	{
		options = SiKGCRoomListManager()->GetCreateRoomOptions();
		options.m_dwDisabledChars = dwDisabledChars;
		SiKGCRoomListManager()->SetCreateRoomOptions(options);
	}
	else
	{
		options = SiKGCRoomListManager()->GetRoomListFilterOptions();
		options.m_dwDisabledChars = dwDisabledChars;
		SiKGCRoomListManager()->SetRoomListFilterOptions(options);
	}

	OnClose();
}

void KGCRoomExtraOptionCharPanel::OnClickIcon( int iIndex )
{
	if( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType != iIndex )
	{
		m_saCharactersIcon[iIndex]->SetSelIcon( !m_saCharactersIcon[iIndex]->IsSelected() );
	}
}