#include "stdafx.h"
#include "GCGuildBenefitListBar.h"

IMPLEMENT_CLASSNAME( KGCGuildBenefitListBar );
IMPLEMENT_WND_FACTORY( KGCGuildBenefitListBar );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildBenefitListBar, "gc_guild_benefit_list_bar" );

KGCGuildBenefitListBar::KGCGuildBenefitListBar( void )
: m_pkGuildLevel( NULL )
, m_pkGuildBenefit( NULL )
, m_pkGuildPoint( NULL )
, m_pkGuildContribution( NULL )
{
    LINK_CONTROL( "guild_level", m_pkGuildLevel );
    LINK_CONTROL( "guild_benefit", m_pkGuildBenefit );
    LINK_CONTROL( "guild_point", m_pkGuildPoint );
    LINK_CONTROL( "guild_contribution", m_pkGuildContribution );
}

KGCGuildBenefitListBar::~KGCGuildBenefitListBar( void )
{
}

void KGCGuildBenefitListBar::OnCreateComplete( void )
{
 
}

void KGCGuildBenefitListBar::ClearText()
{
    if ( m_pkGuildLevel ) {
        m_pkGuildLevel->SetText( L"" );
        m_pkGuildLevel->SetAlign( DT_LEFT );
    }
    if ( m_pkGuildBenefit ) {
        m_pkGuildBenefit->SetText( L"" );
        m_pkGuildBenefit->SetAlign( DT_LEFT );
        m_pkGuildBenefit->SetWordWrap( true );
    }
    if ( m_pkGuildPoint ) {
        m_pkGuildPoint->SetText( L"" );
        m_pkGuildPoint->SetAlign( DT_CENTER );
    }
    if ( m_pkGuildContribution ) {
        m_pkGuildContribution->SetText( L"" );
        m_pkGuildContribution->SetAlign( DT_CENTER );
    }
}

void KGCGuildBenefitListBar::SetLevelText ( std::wstring strLevel_ )
{
    if( m_pkGuildLevel )
        m_pkGuildLevel->SetText( strLevel_ );
}

void KGCGuildBenefitListBar::SetStrNumberText ( int iNumber_ )
{
    if( m_pkGuildBenefit )
    {
        m_pkGuildBenefit->SetForceWordWrapText( g_pkStrLoader->GetString( iNumber_), true );
        m_pkGuildBenefit->SetToolTip( g_pkStrLoader->GetString( iNumber_) );
    }
}

void KGCGuildBenefitListBar::SetGpointText ( int iGpoint_ )
{
    if( m_pkGuildPoint )
        m_pkGuildPoint->SetText( iGpoint_ );
}

void KGCGuildBenefitListBar::SetContributionText (int iContribution )
{
    if( m_pkGuildContribution )
        m_pkGuildContribution->SetText( iContribution );

}

void KGCGuildBenefitListBar::SetListFontColor( bool bFontOn )
{
    DWORD fontColor;
    
    if ( bFontOn )
        fontColor = 0xffffffff;
    else
        fontColor = 0xff6f6f6f;

    if ( m_pkGuildLevel )
        m_pkGuildLevel->SetFontColor( fontColor );
    if ( m_pkGuildBenefit )
        m_pkGuildBenefit->SetFontColor( fontColor );
    if ( m_pkGuildPoint )
        m_pkGuildPoint->SetFontColor( fontColor );
    if ( m_pkGuildContribution )
        m_pkGuildContribution->SetFontColor( fontColor );
    
};