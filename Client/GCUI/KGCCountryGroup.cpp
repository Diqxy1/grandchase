#include "StdAfx.h"
#include "KGCCountryGroup.h"


IMPLEMENT_CLASSNAME( KGCCountryGroup );
IMPLEMENT_WND_FACTORY( KGCCountryGroup );
IMPLEMENT_WND_FACTORY_NAME( KGCCountryGroup, "gc_country_group" );

KGCCountryGroup::KGCCountryGroup(void)
{
    m_pkCountryGroupA = NULL;
    m_pkCountryGroupB = NULL;

    LINK_CONTROL( "country_group_a",	m_pkCountryGroupA );
    LINK_CONTROL( "country_group_b",	m_pkCountryGroupB );

}

KGCCountryGroup::~KGCCountryGroup(void)
{
}

void KGCCountryGroup::OnCreate( void )
{
    m_pkCountryGroupA->InitState(false);
    m_pkCountryGroupB->InitState(false);
}

void KGCCountryGroup::FrameMoveInEnabledState( void )
{
}

void KGCCountryGroup::AllRenderOnOff( bool bOnOff_ )
{
    m_pkCountryGroupA->ToggleRender(bOnOff_);
    m_pkCountryGroupB->ToggleRender(bOnOff_);
}

void KGCCountryGroup::AllRenderToolTipOnOff( bool bOnOff_)
{
    m_pkCountryGroupA->RenderToolTip(bOnOff_);
    m_pkCountryGroupB->RenderToolTip(bOnOff_);
}

void KGCCountryGroup::SetCountryGroup( std::wstring& strCountryName_, bool bUseToolTip_, int iStringID_, ED3DWndToolTipAlignType eAlignType_ )
{
#if !defined(USE_PLAY_COUNTRY_GROUP)
    return;
#endif

    AllRenderOnOff(false);

    if( strCountryName_ == L"PE" )
        m_pkCountryGroupA->ToggleRender(true);
    else
        m_pkCountryGroupB->ToggleRender(true);

    if( bUseToolTip_ )
    {
        AllRenderToolTipOnOff(true);
        m_pkCountryGroupA->SetToolTip( g_pkStrLoader->GetString( iStringID_ ), 0.f, 0.f, WNDUIAT_EXPANTION, eAlignType_ );
        m_pkCountryGroupB->SetToolTip( g_pkStrLoader->GetString( iStringID_ ), 0.f, 0.f, WNDUIAT_EXPANTION, eAlignType_ );
    }
    else
    {
        AllRenderToolTipOnOff(false);
    }
}