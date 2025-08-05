#include "stdafx.h"
#include "D3DButtonCombo.h"
#include "D3DWnd.h"

IMPLEMENT_CLASSNAME( KD3DButtonCombo );
IMPLEMENT_WND_FACTORY( KD3DButtonCombo );
IMPLEMENT_WND_FACTORY_NAME( KD3DButtonCombo, "button_combo" );

KD3DButtonCombo::KD3DButtonCombo(void)
{
	m_vecComponent.resize( MAX_COMBOBUTTON_NUM, NULL );
	for(int i = 0; i < MAX_COMBOBUTTON_NUM; i++ )
	{
		char temp[MAX_PATH];
		sprintf_s( temp, MAX_PATH, "piece_%d", i + 1 );
		LINK_CONTROL(temp, m_vecComponent[i] );
	}
}

KD3DButtonCombo::~KD3DButtonCombo(void)
{
}

void KD3DButtonCombo::OnCreate( void )
{
	std::vector<KD3DWnd*>::iterator it;
	for ( it = m_vecComponent.begin(); it != m_vecComponent.end(); ++it )
	{
		if ( (*it) == NULL )
		{
			m_vecComponent.erase( it, m_vecComponent.end() );
			break;
		}		
	}

	for ( it = m_vecComponent.begin(); it != m_vecComponent.end(); ++it )
	{
		(*it)->InitState( (*it)->IsRenderOn(), true, this );
	}
}

void KD3DButtonCombo::ActionPerformed( const KActionEvent& event )
{
	for ( unsigned int i = 0; i < m_vecComponent.size(); i++ )
	{
		if( event.m_pWnd == m_vecComponent[i] )
		{
			KActionEvent event2 = event;
			event2.m_pWnd = this;
			event2.m_dwlParam = m_vecComponent[i]->m_dwOptionalData;
			event2.m_dwlParam2 = i;
			SpeakToActionListener( event2 );
			return;
		}
	}
}

void KD3DButtonCombo::ToggleRender( bool bRender , unsigned int iComponent )
{
	if( m_vecComponent.size() > iComponent )
		m_vecComponent[iComponent]->ToggleRender( bRender );
}

void KD3DButtonCombo::Lock( bool bLock, unsigned int iComponent )
{
	if( m_vecComponent.size() > iComponent )
		m_vecComponent[iComponent]->Lock( bLock );
}

void KD3DButtonCombo::SetData( DWORD dwData, unsigned int iComponent )
{
	if( m_vecComponent.size() > iComponent )
		m_vecComponent[iComponent]->m_dwOptionalData = dwData;
}

DWORD KD3DButtonCombo::GetData( unsigned int iComponent ) const
{
	if( m_vecComponent.size() > iComponent )
		return m_vecComponent[iComponent]->m_dwOptionalData;
	return 0;

}

D3DXVECTOR2 KD3DButtonCombo::GetCurrentBlockWindowPos( unsigned int iComponent ) const
{
	if( m_vecComponent.size() > iComponent )
		return m_vecComponent[iComponent]->GetCurrentWindowPos();
	return D3DXVECTOR2(0.0f,0.0f);

}

KD3DWnd::ED3DWndMode KD3DButtonCombo::GetWndMode( unsigned int iComponent ) const
{
	if( m_vecComponent.size() > iComponent )
		return m_vecComponent[iComponent]->GetWndMode();
	return NUM_WINDOW_MODE;
}

KD3DWnd* KD3DButtonCombo::GetChild( UINT index )
{
	if( m_vecComponent.size() > index )
		return m_vecComponent[index];
	return NULL;
}