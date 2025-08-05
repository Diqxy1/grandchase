#include "stdafx.h"
#include "GCSeedSlot.h"

IMPLEMENT_CLASSNAME( KGCSeedSlot );
IMPLEMENT_WND_FACTORY( KGCSeedSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCSeedSlot, "gc_seed_slot" );

KGCSeedSlot::KGCSeedSlot( void )
{
    m_pkStaticSeedDesc = NULL;
    m_pkStaticGrowthDesc = NULL;
    m_pkStaticStarDesc = NULL;
    m_pkStaticSeedNum = NULL;
    m_pkStaticGrowthNum = NULL;
    m_pkStaticStarNum = NULL;
    m_pkStaticSlotDesc = NULL;
    LINK_CONTROL( "staticSeedDesc",     m_pkStaticSeedDesc );
    LINK_CONTROL( "staticGrowthDesc",   m_pkStaticGrowthDesc );
    LINK_CONTROL( "staticStarDesc",     m_pkStaticStarDesc );
    LINK_CONTROL( "staticSeedNum",      m_pkStaticSeedNum );
    LINK_CONTROL( "staticGrowthNum",    m_pkStaticGrowthNum );
    LINK_CONTROL( "staticStarNum",      m_pkStaticStarNum );
    LINK_CONTROL( "staticSlotDesc",     m_pkStaticSlotDesc );

    m_pkBtnSelectDefault = NULL;
    m_pkBtnSelectActive = NULL;
    LINK_CONTROL( "btnSelectDefault", m_pkBtnSelectDefault );
    LINK_CONTROL( "btnSelectActive",  m_pkBtnSelectActive );
}

KGCSeedSlot::~KGCSeedSlot( void )
{
}

void KGCSeedSlot::OnCreate( void )
{
    m_pkStaticSeedDesc->InitState( true, false, NULL );
    m_pkStaticGrowthDesc->InitState( true, false, NULL );
    m_pkStaticStarDesc->InitState( true, false, NULL );
    m_pkStaticSeedNum->InitState( true, false, NULL );
    m_pkStaticGrowthNum->InitState( true, false, NULL );
    m_pkStaticStarNum->InitState( true, false, NULL );
    m_pkStaticSlotDesc->InitState( true, false, NULL );

    m_pkStaticSeedDesc->SetAlign( DT_CENTER );
    m_pkStaticGrowthDesc->SetAlign( DT_CENTER );
    m_pkStaticStarDesc->SetAlign( DT_CENTER );
    m_pkStaticSeedNum->SetAlign( DT_CENTER );
    m_pkStaticGrowthNum->SetAlign( DT_CENTER );
    m_pkStaticStarNum->SetAlign( DT_CENTER );
    m_pkStaticSlotDesc->SetAlign( DT_LEFT );

    m_pkStaticSlotDesc->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );

    // 설명은 초기에 스트링 세팅 해주셍
    m_pkStaticSeedDesc->SetText( g_pkStrLoader->GetString( STR_ID_SEED_NEED_DESC ) );
    m_pkStaticGrowthDesc->SetText( g_pkStrLoader->GetString( STR_ID_GROWTH_TIME_DESC ) );
    m_pkStaticStarDesc->SetText( g_pkStrLoader->GetString( STR_ID_GET_STAR_DESC ) );

    m_pkBtnSelectDefault->InitState( true, true, this );
    m_pkBtnSelectActive->InitState( false, true, this );
}

void KGCSeedSlot::ActionPerformed( const KActionEvent& event )
{
    if( ( event.m_pWnd == m_pkBtnSelectDefault || event.m_pWnd == m_pkBtnSelectActive ) &&
        event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        OnClickBtn( event.m_pWnd );
}

void KGCSeedSlot::InitSeedSlot( int iSeedLevel_, int iSeedNum_, int iGrowthNum_, int iStarNum_ )
{
    switch( iSeedLevel_ )
    {
    case 0:
        m_pkStaticSlotDesc->SetText( g_pkStrLoader->GetString( STR_ID_SEED_PLANT_DESC0 ) );
        break;

    case 1:
        m_pkStaticSlotDesc->SetText( g_pkStrLoader->GetString( STR_ID_SEED_PLANT_DESC1) );
        break;

    case 2:
        m_pkStaticSlotDesc->SetText( g_pkStrLoader->GetString( STR_ID_SEED_PLANT_DESC2 ) );
        break;

    default:
        m_pkStaticSlotDesc->SetText( g_pkStrLoader->GetString( STR_ID_SEED_PLANT_DESC0 ) );
        break;
    }

    m_pkStaticSeedNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SEED_NUM, "i", iSeedNum_ ) );
    m_pkStaticGrowthNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_GROWTH_TIME, "i", iGrowthNum_ ) );
    m_pkStaticStarNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_SEED_NUM, "i", iStarNum_ ) );
}

void KGCSeedSlot::Clear( void )
{
    m_pkStaticSlotDesc->SetText( L"" );
    m_pkStaticSeedNum->SetText( L"" );
    m_pkStaticGrowthNum->SetText( L"" );
    m_pkStaticStarNum->SetText( L"" );

    SetActive( false );
}

void KGCSeedSlot::SetActive( bool bSet_ )
{
    if( bSet_ )
    {
       m_pkBtnSelectDefault->ToggleRender( false );
       m_pkBtnSelectActive->ToggleRender( true );
    }
    else
    {
        m_pkBtnSelectDefault->ToggleRender( true );
        m_pkBtnSelectActive->ToggleRender( false );
    }
}

void KGCSeedSlot::OnClickBtn( KD3DWnd* pBtn_ )
{
    if( pBtn_ == m_pkBtnSelectDefault )
        SetActive( true );
    else
        SetActive( false );

    SpeakToActionListener( KActionEvent(this,KD3DWnd::D3DWE_BUTTON_CLICK) );
}

