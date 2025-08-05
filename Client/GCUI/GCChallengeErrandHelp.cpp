#include "stdafx.h"
#include "GCChallengeErrandHelp.h"

IMPLEMENT_CLASSNAME( KGCChallengeErrandHelp );
IMPLEMENT_WND_FACTORY( KGCChallengeErrandHelp );
IMPLEMENT_WND_FACTORY_NAME( KGCChallengeErrandHelp, "gc_errand_help" );

KGCChallengeErrandHelp::KGCChallengeErrandHelp( void ):
m_pkBtnClose(NULL)
{
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    char szName[MAX_PATH];

    for (int i=0 ; i<NUM_STATIC_HELP ; i++)
    {
        m_pkStaticHelp[i] = NULL;
        sprintf( szName, "static_help%d", i );
        LINK_CONTROL( szName, m_pkStaticHelp[i] );
    }

    m_pkStaticItemGuide = NULL;
    LINK_CONTROL( "static_item_guide", m_pkStaticItemGuide );

    for (int i=0 ; i<NUM_REWARD_VIEW_ITEM ; i++)
    {
        m_pkItemImg[i] = NULL;
        sprintf( szName, "item_back%d", i );
        LINK_CONTROL( szName, m_pkItemImg[i] );
        m_pTexItem[i] = NULL;
        m_GCitemID[i] = 0;
    }
}

KGCChallengeErrandHelp::~KGCChallengeErrandHelp( void )
{
    for (int i=0 ; i<NUM_REWARD_VIEW_ITEM ; i++)
        SAFE_RELEASE(m_pTexItem[i]);
}

void KGCChallengeErrandHelp::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCChallengeErrandHelp::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkStaticItemGuide->InitState(true);
    m_pkStaticItemGuide->SetAlign(DT_CENTER);

#if defined(NATION_IDN) || defined( NATION_LATINAMERICA )
    m_pkStaticItemGuide->SetTextAutoMultiline(g_pkStrLoader->GetString(STR_ID_ERRAND_GUIDE_4).c_str());
#else
    m_pkStaticItemGuide->SetText(g_pkStrLoader->GetString(STR_ID_ERRAND_GUIDE_4).c_str());

#endif
    for (int i=0 ; i<NUM_REWARD_VIEW_ITEM ; i++)
    {
        m_pkItemImg[i]->InitState(true);
    }
}

void KGCChallengeErrandHelp::OnClose()
{
    ToggleRender(false);

}

void KGCChallengeErrandHelp::OnCreateComplete()
{
    for (int i=0 ; i<NUM_STATIC_HELP ; i++)
        m_pkStaticHelp[i]->SetAlign(DT_CENTER);


    m_pkStaticHelp[0]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ERRAND_GUIDE_1 ).c_str() );
    m_pkStaticHelp[1]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ERRAND_GUIDE_2 ).c_str() );
    m_pkStaticHelp[2]->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_ERRAND_GUIDE_3 ).c_str() );

}

void KGCChallengeErrandHelp::PostChildDraw()
{
    for (int i=0 ; i<NUM_REWARD_VIEW_ITEM ; i++)
    {
        if (m_GCitemID[i] == 0)
            continue;

        if (m_pTexItem[i] == NULL)
            continue;

        D3DXVECTOR2 vPos = GetRelocatedWindowPos();
        vPos.x += m_pkItemImg[i]->GetFixedWindowLocalPos().x * m_fWindowScaleX;
        vPos.y += m_pkItemImg[i]->GetFixedWindowLocalPos().y * m_fWindowScaleY;

        g_pGCDeviceManager2->DrawInScreen( m_pTexItem[i], vPos.x, vPos.y, 
            vPos.x + m_pkItemImg[i]->GetWidth() * m_fWindowScaleX , vPos.y + m_pkItemImg[i]->GetHeight() * m_fWindowScaleY , 0.0f, 0.0f, 1.0f, 1.0f );

    }
}

void KGCChallengeErrandHelp::FrameMoveInEnabledState()
{
    for (int i=0 ; i<NUM_REWARD_VIEW_ITEM ; i++)
    {
        if ( m_pkItemImg[i] && m_pkItemImg[i]->CheckMousePosInWindowBound() )
        {
            if (m_GCitemID[i]== 0)
                continue;

            if ( NULL == m_pkItemImg[i] )
                continue;

            D3DXVECTOR2 vecPos = m_pkItemImg[i]->GetCurrentWindowPos();
            vecPos.x *= m_fWindowScaleX;
            vecPos.y *= m_fWindowScaleY;
            vecPos.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

            GCItem* pItem = g_pItemMgr->GetItemData(m_GCitemID[i]/10);
            if (pItem == NULL)
                break;
            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem );
            g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
            g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vecPos );
            g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
            break;
        }
    }

}

void KGCChallengeErrandHelp::SetRewardItemImg()
{
    std::vector<GCITEMID> vecItemIDContainer = SiGCChallengeErrandManager()->GetRewardDisplayItemID();

    if (vecItemIDContainer.empty())
        return;

    std::vector<GCITEMID>::iterator vit = vecItemIDContainer.begin();
    int i=0;
    while( i<NUM_REWARD_VIEW_ITEM && vit!=vecItemIDContainer.end() )
    {
        SAFE_RELEASE(m_pTexItem[i]);
        m_GCitemID[i] = (*vit);
        m_pTexItem[i] = g_pItemMgr->CreateShopItemTexture( m_GCitemID[i] / 10 );

        ++vit;
        i++;
    }

}

