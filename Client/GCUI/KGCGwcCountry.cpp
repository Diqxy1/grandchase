#include "stdafx.h"
#include "GCUI/KGCGwcCountry.h"

IMPLEMENT_CLASSNAME( KGCGwcCountry );
IMPLEMENT_WND_FACTORY( KGCGwcCountry );
IMPLEMENT_WND_FACTORY_NAME( KGCGwcCountry, "gc_gwc_country" );

KGCGwcCountry::KGCGwcCountry( void )
: m_pkFlag ( NULL ), 
m_pkGuageBar(NULL) 
{

    for(int iLoop = 0; iLoop < MAX_GRADE; ++iLoop) {
        m_pkLevelWnd[iLoop] = NULL;
    }

    LINK_CONTROL( "flag", m_pkFlag );
    LINK_CONTROL( "guage_bar", m_pkGuageBar);
    char szTemp[256];
    for(int iLoop = 0; iLoop < MAX_GRADE; ++iLoop) {
        sprintf(szTemp, "level_%d", (iLoop+1));            LINK_CONTROL( szTemp, m_pkLevelWnd[iLoop]          );
    }

}

KGCGwcCountry::~KGCGwcCountry( void )
{
}

void KGCGwcCountry::OnCreate( )
{
    m_pkFlag->InitState(true, true ,this );
    m_pkGuageBar->InitState(false, true, this);
    m_pkGuageBar->SetWidth( 0 );
    
    for( int i = 0 ; i < MAX_GRADE ; ++i ) {
        m_pkLevelWnd[i]->InitState( false );
    }

}

void KGCGwcCountry::ActionPerformed( const KActionEvent& event )
{

}

void KGCGwcCountry::SetInformation( KGWCCountryInfo mapCountryInfo_ )
{
    KGWCCountryInfo kCountryInfo = mapCountryInfo_;

    float fFullGuageWidth		=	270.0f;
    DWORD m_dwGuageWidth =0;
    
   

    m_dwGuageWidth = static_cast<DWORD>( fFullGuageWidth *  0.067 * ( kCountryInfo.m_dwGrade ) );
    
   

    if (m_pkGuageBar) 
    {
        m_pkGuageBar->SetWidth( m_dwGuageWidth );
        m_pkGuageBar->ToggleRender( true );
    }
    
    for( int i = 0; i < MAX_GRADE; ++i )
    {
        bool bRender = ( i == (kCountryInfo.m_dwGrade -1 ) ) ? true : false;
        m_pkLevelWnd[i]->ToggleRender( bRender);
    }


}