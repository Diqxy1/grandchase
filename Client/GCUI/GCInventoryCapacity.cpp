#include "stdafx.h"
#include "GCItemManager.h"

#include "GCInventoryCapacity.h"
//
//
//

IMPLEMENT_CLASSNAME( KGCInventoryCapacity );
IMPLEMENT_WND_FACTORY( KGCInventoryCapacity );
IMPLEMENT_WND_FACTORY_NAME( KGCInventoryCapacity, "gc_inventory_capacity" );

KGCInventoryCapacity::KGCInventoryCapacity(void)
{
    m_pkInventoryCapacity = NULL;
    LINK_CONTROL( "inventory_capacity_static", m_pkInventoryCapacity );
}

KGCInventoryCapacity::~KGCInventoryCapacity(void)
{
}

void KGCInventoryCapacity::OnCreate( void )
{
    m_pkInventoryCapacity->InitState( true );
    m_pkInventoryCapacity->SetAlign( DT_CENTER );
    m_pkInventoryCapacity->SetFontColor( 0xffffffff );

    m_pkInventoryCapacity->SetText( L"" );
}

void KGCInventoryCapacity::ActionPerformed( const KActionEvent& event )
{
}

void KGCInventoryCapacity::UpdateCapacity( const bool bCoordi_ )
{
    if( false == bCoordi_ ) {
        int iInventoryCapacity = g_kGlobalValue.m_iInvenCapacity;
        int iUserItemSize = g_pItemMgr->GetTotalInventorySize();
        std::wstring strColor = GetColor( iUserItemSize, iInventoryCapacity );
        m_pkInventoryCapacity->SetText( g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_CAPACITY, "lii", strColor, iUserItemSize, iInventoryCapacity ) );
    }
    else { 
       int iCoordiInvenCapacity = g_kGlobalValue.m_iInvenCapacity;
       int iUserCoordiItemSize = g_pItemMgr->GetCoordiInventorySize();
       std::wstring strColor = GetColor( iUserCoordiItemSize, iCoordiInvenCapacity);
       m_pkInventoryCapacity->SetText( g_pkStrLoader->GetReplacedString( STR_ID_INVENTORY_CAPACITY, "lii", strColor, iUserCoordiItemSize, iCoordiInvenCapacity) );
    }
}

std::wstring KGCInventoryCapacity::GetColor( int iUserItemSize, int iInventoryCapacity )
{
    float fPercent = (float)iUserItemSize / (float)iInventoryCapacity;

    if( fPercent < 0.2f )
        return L"#cFFFFFF";
    else if( fPercent < 0.4f )
        return L"#cFFDDDD";
    else if( fPercent < 0.6f )
        return L"#cFFBBBB";
    else if( fPercent < 0.8f )
        return L"#cFF9999";
    else
        return L"#cFF7777";
}