#include "stdafx.h"
#include "GCRecipeInfoBar.h"

IMPLEMENT_CLASSNAME( KGCRecipeInfoBar );
IMPLEMENT_WND_FACTORY( KGCRecipeInfoBar );
IMPLEMENT_WND_FACTORY_NAME( KGCRecipeInfoBar, "gc_creation_recipe_bar" );

KGCRecipeInfoBar::KGCRecipeInfoBar( void )
: m_pkRecipeBar( NULL )
, m_pkStaticItemLevel( NULL )
, m_pkStaticItemName( NULL )
, m_pkStaticItemGrade( NULL )
, m_pkStaticPrice( NULL )
, m_bEnable( false )
{
    LINK_CONTROL( "recipe_bar",			m_pkRecipeBar );
    LINK_CONTROL( "static_item_level",	m_pkStaticItemLevel );
    LINK_CONTROL( "static_item_name",	m_pkStaticItemName );
	LINK_CONTROL( "static_grade",		m_pkStaticItemGrade );
    LINK_CONTROL( "static_price",		m_pkStaticPrice );
}

KGCRecipeInfoBar::~KGCRecipeInfoBar( void )
{

}

void KGCRecipeInfoBar::OnCreate( void )
{
    m_pkRecipeBar->InitState( true, true, this );
    m_pkStaticItemLevel->SetAlign( DT_CENTER );
    m_pkStaticItemName->SetAlign( DT_CENTER );
    m_pkStaticPrice->SetAlign( DT_CENTER );
	m_pkStaticItemGrade->SetAlign( DT_CENTER );
}

void KGCRecipeInfoBar::OnCreateComplete( void )
{
}

void KGCRecipeInfoBar::ActionPerformed( const KActionEvent& event_ )
{
    if( event_.m_pWnd == m_pkRecipeBar )
        SpeakToActionListener( KActionEvent( this, event_.m_dwCode) );
}

void KGCRecipeInfoBar::SetTextRecipeInfoBar( std::wstring strItemLevel_, std::wstring strItemName_, std::wstring strPrice_, std::wstring& strGrade_ )
{
    m_pkStaticItemLevel->SetText( strItemLevel_ );
    m_pkStaticItemName->SetForceWordWrapText( strItemName_ , true );
    m_pkStaticPrice->SetForceWordWrapText( strPrice_, true );
	m_pkStaticItemGrade->SetText( strGrade_ );
}

void KGCRecipeInfoBar::SetFontColorRecipeInfoBar( D3DCOLOR dwColor_ )
{
    m_dwColor = dwColor_;
	m_pkStaticItemGrade->SetFontColor( dwColor_ );

    if( m_dwColor != 0xff6f6f6f ) { 
        m_bEnable = true;
        m_pkStaticItemLevel->SetFontColor(0xffffffff);
        m_pkStaticItemName->SetFontColor(0xffffffff);
        m_pkStaticPrice->SetFontColor(0xffffffff);
    }
    else { 
        m_bEnable = false;
        m_pkStaticItemLevel->SetFontColor(0xff6f6f6f);
        m_pkStaticItemName->SetFontColor(0xff6f6f6f);
        m_pkStaticPrice->SetFontColor(0xff6f6f6f);
    }
}

void KGCRecipeInfoBar::SetHeightDirect( DWORD dwHeight )
{
    KD3DWnd::SetHeightDirect( dwHeight );
    m_pkRecipeBar->SetHeightDirect( dwHeight );
    float fHeight = static_cast< float>( dwHeight );
    float fStaticLocalPosY = fHeight * 0.3f;
    LONG iFontSize = static_cast<LONG>( fHeight * 0.48f );
    m_pkStaticItemLevel->SetWindowPosDirect( D3DXVECTOR2( m_pkStaticItemLevel->GetFixedWindowLocalPos().x, fStaticLocalPosY) );
    m_pkStaticItemName->SetWindowPosDirect( D3DXVECTOR2( m_pkStaticItemName->GetFixedWindowLocalPos().x, fStaticLocalPosY) );
    m_pkStaticPrice->SetWindowPosDirect( D3DXVECTOR2( m_pkStaticPrice->GetFixedWindowLocalPos().x, fStaticLocalPosY) );
	m_pkStaticItemGrade->SetWindowPosDirect( D3DXVECTOR2( m_pkStaticItemGrade->GetFixedWindowLocalPos().x, fStaticLocalPosY) );
    m_pkStaticItemLevel->SetFontSize( iFontSize );
    m_pkStaticItemName->SetFontSize( iFontSize );
    m_pkStaticPrice->SetFontSize( iFontSize );
	m_pkStaticItemGrade->SetFontSize( iFontSize );
}

void KGCRecipeInfoBar::FrameMoveInEnabledState()
{
	//POINT ptMouse = g_pkInput->GetMousePos();
	//D3DXVECTOR2 vMousePos( (float)ptMouse.x, (float)ptMouse.y );

	if( CheckMousePosInWindowBound() )
	{
		GCItem* pItem = g_pItemMgr->GetItemData( GetItemID() );
		if( NULL == pItem )
		{
			return;
		}

		g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, NULL, NULL, NULL, m_kRecipeInfo.cGrade, m_kRecipeInfo.iItemLevel );
		g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

		D3DXVECTOR2 vecPos( GetCurrentWindowPos() );
		vecPos.x += GetWidth();
		vecPos.x *= m_fWindowScaleX;
		vecPos.y *= m_fWindowScaleY;

		g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );            
		g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
		g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
	}
}