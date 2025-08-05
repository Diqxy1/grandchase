#include "stdafx.h"
#include "GCChallengeErrandModule.h"

IMPLEMENT_CLASSNAME( KGCChallengeErrandModule );
IMPLEMENT_WND_FACTORY( KGCChallengeErrandModule );
IMPLEMENT_WND_FACTORY_NAME( KGCChallengeErrandModule, "gc_errand_module" );

KGCChallengeErrandModule::KGCChallengeErrandModule( void ):
m_pkPlayerLocation( NULL ),
m_pkDefaultImg( NULL ),
m_pkHoverImg( NULL ),
m_pkSpace( NULL ),
m_pkRewardBack( NULL ),
m_pkItemGuide( NULL ),
m_pkImgItem( NULL ),
m_pkMerry( NULL ),
m_eModuleName( -1 ),
m_nIndex( 0 ),
m_iAniFrame( 0 ),
m_bIsMerry( false )
{
    LINK_CONTROL( "player_location", m_pkPlayerLocation );
    LINK_CONTROL( "default_image", m_pkDefaultImg );
    LINK_CONTROL( "hover_image", m_pkHoverImg );
    LINK_CONTROL( "buy_space", m_pkSpace );

    LINK_CONTROL( "reward_back", m_pkRewardBack );
    LINK_CONTROL( "static_guide", m_pkItemGuide );
    LINK_CONTROL( "item_wnd", m_pkImgItem );

    LINK_CONTROL( "errand_merry", m_pkMerry );

}

KGCChallengeErrandModule::~KGCChallengeErrandModule( void )
{
}

void KGCChallengeErrandModule::OnCreate( void )
{
    m_pkDefaultImg->InitState(true);
    m_pkHoverImg->InitState(false);
    m_pkPlayerLocation->InitState( false );
    m_pkSpace->InitState( false );

    m_pkRewardBack->InitState( false );
    m_pkItemGuide->InitState( false );
    m_pkItemGuide->SetAlign( DT_CENTER );

    m_pkImgItem->InitState( false );
    m_pkMerry->InitState( false );

}

void KGCChallengeErrandModule::ActionPerformed( const KActionEvent& event )
{
}


void KGCChallengeErrandModule::FrameMoveInEnabledState( void )
{

}

float KGCChallengeErrandModule::StayLocationX( void )
{
    float fStayPointX = m_pkPlayerLocation->GetFixedWindowLocalPos().x;
    float fLocalPointX = this->GetFixedWindowLocalPos().x;

    return fLocalPointX + fStayPointX;
}

float KGCChallengeErrandModule::StayLocationY( void )
{
    float fStayPointY = m_pkPlayerLocation->GetFixedWindowLocalPos().y;
    float fLocalPointY = this->GetFixedWindowLocalPos().y;

    return fStayPointY + fLocalPointY;
}

void KGCChallengeErrandModule::SetImgRenderOff( )
{
    if( m_pkHoverImg->IsRenderOn() )
    {
        m_pkHoverImg->InitState( false );
        m_pkSpace->InitState( false );
    }
}

void KGCChallengeErrandModule::SetImgRenderOn()
{
    if( !m_pkHoverImg->IsRenderOn() )
    {
        m_pkHoverImg->InitState( true );
        m_pkSpace->InitState( true );
    }
}

void KGCChallengeErrandModule::UpdateRewardRender( bool bRender )
{

    if( bRender )
    {
        m_pkRewardBack->InitState( true );
        m_pkImgItem->InitState( true );
        m_pkImgItem->ShowItem(true);
        m_pkItemGuide->InitState( true );
    }
    else
    {
        m_pkRewardBack->InitState( false );
        m_pkImgItem->InitState( false );
        m_pkImgItem->ShowItem(false);
        m_pkItemGuide->InitState( false );
    }


}

void KGCChallengeErrandModule::SetRewardItem( KItem kItem )
{
    GCItem* pItem = g_pItemMgr->GetItemData( kItem.m_ItemID / 10 );
    m_pkImgItem->SetItemInfo( kItem.m_ItemID / 10 );
    std::wstring strItemName = pItem->strItemName;
    m_pkItemGuide->SetText( strItemName );
    m_pkItemGuide->SetAutoScroll( true );
}


void KGCChallengeErrandModule::MoveMerryAmin()
{
    m_iAniFrame = 0;
    for( int i = 0; i < D3DWM_ACTIVE; i++ )
    {
        if (m_pkMerry)
            m_pkMerry->SetWndMode(static_cast<ED3DWndMode>(m_iAniFrame));
        m_iAniFrame++;
    }

}

bool KGCChallengeErrandModule::GetMerry( void )
{
     return m_bIsMerry;
}

void KGCChallengeErrandModule::MerryRender( bool bRender )
{

    float fValance = 10.0f; // X축 위치가 안맞아서 밸런스 값

    D3DXVECTOR2 vPosWindow = m_pkHoverImg->GetFixedWindowPos();
    vPosWindow.x += m_pkHoverImg->GetWidth() / 2 + fValance;
    vPosWindow.y += m_pkHoverImg->GetHeight() / 2;

    if( bRender )
    {
        m_pkMerry->InitState( true );
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence("UI_Merry_Alarm", PARTICLEPOS_X( vPosWindow.x ) , PARTICLEPOS_Y(vPosWindow.y), PARTICLEPOS_Z ), GC_LAYER_UI );
        if (m_pkMerry)
            m_pkMerry->SetWndMode(static_cast<ED3DWndMode>(m_iAniFrame));
    }
    else
    {
        m_pkMerry->InitState( false );
        m_iAniFrame = 0;
    }
}

void KGCChallengeErrandModule::SetMerry( bool bMerry )
{
    m_bIsMerry = bMerry;
}

void KGCChallengeErrandModule::ClearItem( void )
{
    m_pkImgItem->SetItemInfo( 0 );
    m_pkImgItem->ShowItem( false );
    m_pkItemGuide->SetText( L"" );
}

