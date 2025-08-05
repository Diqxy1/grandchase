#include "stdafx.h"
#include "GCMaterialItemInfo.h"

IMPLEMENT_CLASSNAME( KGCMaterialItemInfo );
IMPLEMENT_WND_FACTORY( KGCMaterialItemInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCMaterialItemInfo, "gc_material_item_info" );

KGCMaterialItemInfo::KGCMaterialItemInfo( void )
: m_pkStaticCount( NULL )
, m_pkItemBtn( NULL )
, m_pkBack( NULL )
, m_bEnable( false )
{
    LINK_CONTROL( "back", m_pkBack );
    LINK_CONTROL( "static_count", m_pkStaticCount );
    LINK_CONTROL( "item_select_btn", m_pkItemBtn );
}

KGCMaterialItemInfo::~KGCMaterialItemInfo( void )
{
}

void KGCMaterialItemInfo::OnCreate( void )
{
    m_pkBack->InitState(true, true, this);
    m_pkItemBtn->InitState(false, true, this);
    m_pkStaticCount->InitState( true, true, this );
	m_pkStaticCount->SetAlign( DT_CENTER );
}

void KGCMaterialItemInfo::OnCreateComplete( void )
{
}

void KGCMaterialItemInfo::ActionPerformed( const KActionEvent& event_ )
{
}


void KGCMaterialItemInfo::SetHeightDirect( DWORD dwHeight )
{
    KD3DWnd::SetHeightDirect( dwHeight );
    m_pkBack->SetHeightDirect( dwHeight );
    m_pkItemBtn->SetHeightDirect( dwHeight );
    float fHeight = static_cast< float>( dwHeight );
    float fStaticLocalPosY = fHeight * 0.3f;
    LONG iFontSize = static_cast<LONG>( fHeight * 0.48f );
    m_pkStaticCount->SetWindowPosDirect( D3DXVECTOR2( m_pkStaticCount->GetFixedWindowLocalPos().x, fStaticLocalPosY) );
    m_pkStaticCount->SetFontSize( iFontSize );
}
