#include "stdafx.h"
#include "GCSearchBar.h"
#include "GCCashShopScene.h"
#include "GCGPShopScene.h"
#include "GCDepotDlg.h"
#include "KGCNewForgeDlg.h"

IMPLEMENT_CLASSNAME( KGCSearchBar );
IMPLEMENT_WND_FACTORY( KGCSearchBar );
IMPLEMENT_WND_FACTORY_NAME( KGCSearchBar, "gc_search_bar" );

KGCSearchBar::KGCSearchBar( void )
{
    m_pkSearchButton = NULL;
    m_pkEditSearch = NULL;
    m_pkSearchDesc = NULL;

    LINK_CONTROL( "search_bar_button", m_pkSearchButton );
    LINK_CONTROL( "search_bar_edit", m_pkEditSearch );
    LINK_CONTROL( "search_desc", m_pkSearchDesc );
}

KGCSearchBar::~KGCSearchBar( void )
{
}

void KGCSearchBar::OnCreate( void )
{
    m_pkSearchButton->InitState( true, true, this );
    m_pkEditSearch->InitState( true, true, this );
    m_pkSearchDesc->InitState( true );

    m_pkSearchDesc->SetFontColor( 0xCCCCCCCC );
}

void KGCSearchBar::FrameMoveInEnabledState()
{
    std::wstring strKeyword = m_pkEditSearch->GetText();
    if( strKeyword.size() > 0 )
        m_pkSearchDesc->ToggleRender( false );
    else
        m_pkSearchDesc->ToggleRender( true );
}

void KGCSearchBar::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    {
        if( event.m_pWnd == m_pkSearchButton )
        {
            g_KDSound.Play( "30" );
            return;
        }
    }

    if ( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkSearchButton )
        {
            std::wstring strKeyword = m_pkEditSearch->GetText();
            if (g_pkUIScene->m_pkCoordiViewDlg->IsWndState(D3DWS_ENABLED))
            {
                g_pkUIScene->m_pkCoordiViewDlg->FindItemFromName(m_pkEditSearch->GetText());
            }
            else if (g_pkUIScene->m_pkDepot->isDepotSearchActive) {
                g_pkUIScene->m_pkDepot->UpdateItemSearch(strKeyword);
            }
            else if (g_pkUIScene->m_pkForgeDlg->IsWndState(D3DWS_ENABLED))
            {
                g_pkUIScene->m_pkForgeDlg->FindItemFromName(m_pkEditSearch->GetText());
            }
            switch (g_MyD3D->m_pStateMachine->GetState())
            {
                case GS_SHOP_CASH:
                    g_pkUIScene->m_pkCashShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#ifndef DISABLE_MAP_SHOP // Kawan>
                case GS_SHOP_GP:
                    g_pkUIScene->m_pkGPShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#endif
#if defined (USE_COORDI_SHOP)
                case GS_COORDI_SHOP:
                    g_pkUIScene->m_pkCoordiShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#endif
                case GS_MY_INFO:
                case GS_MY_INFO_FROM_ROOM:
                    g_pkUIScene->m_pkMyInfoScene->FindItemFromName(m_pkEditSearch->GetText()); break;

            }
            g_KDSound.Play("31");
            SetFocus(false);
        }
    }

    // 엔터를 눌렀을 때
    if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN )
    {
        std::wstring strKeyword = m_pkEditSearch->GetText();
        if (g_pkUIScene->m_pkCoordiViewDlg->IsWndState(D3DWS_ENABLED))
        {
            g_pkUIScene->m_pkCoordiViewDlg->FindItemFromName(m_pkEditSearch->GetText());
        }
        else if (g_pkUIScene->m_pkDepot->isDepotSearchActive) {
            g_pkUIScene->m_pkDepot->UpdateItemSearch(strKeyword);
        }
        else if (g_pkUIScene->m_pkForgeDlg->IsWndState(D3DWS_ENABLED))
        {
            g_pkUIScene->m_pkForgeDlg->FindItemFromName(m_pkEditSearch->GetText());
        }
        switch (g_MyD3D->m_pStateMachine->GetState())
        {
            case GS_SHOP_CASH:
                g_pkUIScene->m_pkCashShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#ifndef DISABLE_MAP_SHOP // Kawan>
            case GS_SHOP_GP:
                g_pkUIScene->m_pkGPShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#endif
#if defined (USE_COORDI_SHOP)
            case GS_COORDI_SHOP:
                g_pkUIScene->m_pkCoordiShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#endif
            case GS_MY_INFO:
            case GS_MY_INFO_FROM_ROOM:
                g_pkUIScene->m_pkMyInfoScene->FindItemFromName(m_pkEditSearch->GetText()); break;
        }
        g_KDSound.Play("31");
        SetFocus(false);
    }

    // ESC 눌렀을 때
    if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_ESCAPE )
    {
        std::wstring strKeyword = m_pkEditSearch->GetText();
        if (g_pkUIScene->m_pkCoordiViewDlg->IsWndState(D3DWS_ENABLED))
        {
            g_pkUIScene->m_pkCoordiViewDlg->FindItemFromName(m_pkEditSearch->GetText());
        }
        else if (g_pkUIScene->m_pkDepot->isDepotSearchActive) {
            g_pkUIScene->m_pkDepot->UpdateItemSearch(strKeyword);
        }
        else if (g_pkUIScene->m_pkForgeDlg->IsWndState(D3DWS_ENABLED))
        {
            g_pkUIScene->m_pkForgeDlg->FindItemFromName(m_pkEditSearch->GetText());
        }
        switch (g_MyD3D->m_pStateMachine->GetState())
        {
            case GS_SHOP_CASH:
                g_pkUIScene->m_pkCashShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#ifndef DISABLE_MAP_SHOP // Kawan>
            case GS_SHOP_GP:
                g_pkUIScene->m_pkGPShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#endif
#if defined (USE_COORDI_SHOP)
            case GS_COORDI_SHOP:
                g_pkUIScene->m_pkCoordiShopScene->FindItemFromName(m_pkEditSearch->GetText()); break;
#endif
            case GS_MY_INFO:
            case GS_MY_INFO_FROM_ROOM:
                g_pkUIScene->m_pkMyInfoScene->FindItemFromName(m_pkEditSearch->GetText()); break;
        }
        g_KDSound.Play("31");
        SetFocus(false);
    }
}

void KGCSearchBar::Init()
{
    m_pkEditSearch->SetFocus();
    m_pkEditSearch->SetText( L"" );
    m_pkSearchDesc->SetText( g_pkStrLoader->GetString( STR_ID_FIND_ITEM ).c_str() );
}

void KGCSearchBar::SetFocus( bool bFocus )
{
    if( bFocus )
        m_pkEditSearch->SetFocus();
    else
        m_pkEditSearch->Activate( bFocus );
}