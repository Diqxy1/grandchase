#include "stdafx.h"
//
#include "../MyD3D.h"
#include "GCItemBar.h"
//


IMPLEMENT_CLASSNAME( KGCItemBar );
IMPLEMENT_WND_FACTORY( KGCItemBar );
IMPLEMENT_WND_FACTORY_NAME( KGCItemBar, "gc_item_bar" );

KGCItemBar::KGCItemBar( void )
{
    m_pkMoneyType   = NULL;
    m_pkVipType     = NULL;
    m_pkCert        = NULL;
    m_pkPointMark   = NULL;
    m_eMoneyType    = EMT_GAMEPOINT;

    LINK_CONTROL("money_type", m_pkMoneyType);
    LINK_CONTROL("vip_type", m_pkVipType);
    LINK_CONTROL( "quest_cert", m_pkCert );
    LINK_CONTROL( "point_mark", m_pkPointMark );
}

KGCItemBar::~KGCItemBar( void )
{
    // empty
}

void KGCItemBar::OnCreate( void )
{
    //m_pkMoneyType->InitState( true );
    //m_pkCert->InitState( true );
    //m_pkPointMark->InitState( true );
}

void KGCItemBar::SetCertType( ECertType eType )
{
    switch ( eType )
    {
        case CERT_TYPE_NONE:
        {
            SetMoneyType( m_eMoneyType );
            break;
        }
		default:
		{
            m_pkCert->ToggleRender( true );
            m_pkCert->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
            m_pkMoneyType->ToggleRender(false);
            m_pkVipType->ToggleRender(false);
            m_pkPointMark->ToggleRender( false );
            break;
		}
    }
}

void KGCItemBar::SetMoneyType( EMONEYTYPE eMoneyType )
{
    m_pkCert->ToggleRender( false );
    m_pkPointMark->ToggleRender( false );

    switch ( m_eMoneyType = eMoneyType )
    {
        case EMT_GAMEPOINT:    m_pkVipType->ToggleRender(false); m_pkMoneyType->ToggleRender(true); m_pkMoneyType->SetWndMode( KD3DWnd::D3DWM_DEFAULT ); break;
        case EMT_CASH:         m_pkVipType->ToggleRender(false); m_pkMoneyType->ToggleRender(true); m_pkMoneyType->SetWndMode( KD3DWnd::D3DWM_HOVER );   break;
        case EMT_GEM:          m_pkMoneyType->ToggleRender(false); m_pkVipType->ToggleRender(true);  break;
        case EMT_CRYSTAL:      m_pkVipType->ToggleRender(false); m_pkMoneyType->ToggleRender(true); m_pkMoneyType->SetWndMode( KD3DWnd::D3DWM_ACTIVE );  break;
    }
}

void KGCItemBar::SetPointType()
{
    m_pkPointMark->ToggleRender( true );
    m_pkMoneyType->ToggleRender(false);
    m_pkVipType->ToggleRender(false);
    m_pkCert->ToggleRender( false );
}

void KGCItemBar::SetItem( GCItem*& ItemInfo, bool bShop )
{

    SetMoneyType( ItemInfo->eMoneyType );

    if( g_MyD3D->m_kItemMgr.IsPointShopItem( ItemInfo->dwGoodsID ) )
    {
        SetPointType();
    }
}