#include "stdafx.h"
#include "GCShopTab.h"

IMPLEMENT_CLASSNAME( KGCShopTab );
IMPLEMENT_WND_FACTORY( KGCShopTab );
IMPLEMENT_WND_FACTORY_NAME( KGCShopTab, "gc_shop_tab" );

KGCShopTab::KGCShopTab( void )
{
    m_bToggleSize = false;

    m_pkTitleWnd = NULL;
	m_pkTitleWndToggle = NULL;

	LINK_CONTROL( "shop_tab_name", m_pkTitleWnd );
	LINK_CONTROL( "shop_tab_name2", m_pkTitleWndToggle );		//없을 수도 있음
}

KGCShopTab::~KGCShopTab( void )
{
    // empty
}

void KGCShopTab::OnCreate( void )
{
}

void KGCShopTab::OnActivate( void )
{
//    if ( D3DWS_ENABLED != m_eState ) return;

    if ( IsActive() )
    {
        if ( false == m_bToggleSize )
        {
            m_bToggleSize = true;
            SetHeight( static_cast<DWORD>( GetHeight() + 4.0f ) );
            SetWindowPos( GetFixedWindowLocalPos() - D3DXVECTOR2( 0.0f, 4.0f ) );
        }
    }
    else
    {
        if ( true == m_bToggleSize )
        {
            m_bToggleSize = false;
            SetHeight( static_cast<DWORD>( GetHeight() - 4.0f ) );
            SetWindowPos( GetFixedWindowLocalPos() + D3DXVECTOR2( 0.0f, 4.0f ) );
        }
    }
}

void KGCShopTab::FrameMoveInEnabledState( void )
{
    SetWndMode( IsActive() ? D3DWM_ACTIVE : D3DWM_DEFAULT );
    m_pkTitleWnd->SetWndMode( IsActive() ? D3DWM_ACTIVE : D3DWM_DEFAULT );
	if( m_pkTitleWndToggle != NULL )
		m_pkTitleWndToggle->SetWndMode( IsActive() ? D3DWM_ACTIVE : D3DWM_DEFAULT );
}

void KGCShopTab::ChangeTextType(int iTextType)
{
	if( iTextType == 1 )
	{
		if( m_pkTitleWndToggle != NULL )
		{
			m_pkTitleWndToggle->ToggleRender(true);
			m_pkTitleWnd->ToggleRender(false);
			return;
		}
	}
	m_pkTitleWndToggle->ToggleRender(false);
	m_pkTitleWnd->ToggleRender(true);
	
}