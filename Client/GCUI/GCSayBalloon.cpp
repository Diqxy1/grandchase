#include "stdafx.h"
#include ".\gcsayballoon.h"

IMPLEMENT_CLASSNAME( KGCSayBalloon );
IMPLEMENT_WND_FACTORY( KGCSayBalloon );
IMPLEMENT_WND_FACTORY_NAME( KGCSayBalloon, "gc_say_balloon" );

#define SAY_BOX_ALPHA 200

KGCSayBalloon::KGCSayBalloon(void)
: m_eSayWhoPos(GC_SP_MIDDLE)
{
    m_pkMessageBalloon = NULL;
    m_pkSayWho = NULL;

    LINK_CONTROL( "say_box",    m_pkMessageBalloon );
    LINK_CONTROL( "say_who",    m_pkSayWho );
}

KGCSayBalloon::~KGCSayBalloon(void)
{
}

void KGCSayBalloon::OnCreate()
{
    if( m_pkMessageBalloon == NULL ||
        m_pkSayWho == NULL )
    {
        ASSERT( !"UI Script Load Error" );
        return;
    }
    m_pkSayWho->InitState( true, true, this );
    m_pkMessageBalloon->InitState( true, true, this );

    SetAllWindowColor( D3DCOLOR_ARGB( SAY_BOX_ALPHA, 0xff, 0xff, 0xff ) );
}

void KGCSayBalloon::ActionPerformed(const KActionEvent& event )
{

}

void KGCSayBalloon::SetWidth( DWORD dwWidth )
{
    m_pkMessageBalloon->SetWidth( dwWidth );
    SetSayWhoPos( m_dwSayWhoPos );
}

void KGCSayBalloon::SetHeight( DWORD dwHeight )
{
    m_pkMessageBalloon->SetHeight( dwHeight );
    SetSayWhoPos( m_dwSayWhoPos );
}

void KGCSayBalloon::SetWidthHeight( DWORD dwWidth, DWORD dwHeight )
{
    m_pkMessageBalloon->SetWidth( dwWidth );
    m_pkMessageBalloon->SetHeight( dwHeight );
    SetSayWhoPos( m_pkMessageBalloon->GetCurrentWindowPos() );
}

void KGCSayBalloon::SetSayWhoPos( DWORD dwPos )
{
    if( m_pkMessageBalloon->GetWidth() < dwPos )
    {
        m_dwSayWhoPos = static_cast<DWORD>(m_pkMessageBalloon->GetWidth() * 0.5f);
    }
    else
    {
        m_dwSayWhoPos = dwPos;
    }

    D3DXVECTOR2 vPos = m_pkMessageBalloon->GetFixedWindowLocalPos();
    vPos.y += m_pkMessageBalloon->GetHeight() - 2;
    vPos.x = static_cast<FLOAT>(m_dwSayWhoPos);
}
#define GAP_INCURSIUON  2

void KGCSayBalloon::SetSayWhoPos( D3DXVECTOR2 vPos )
{
    vPos.y += m_pkMessageBalloon->GetHeight() - GAP_INCURSIUON;
	vPos.x += (m_pkMessageBalloon->GetWidth()-m_pkSayWho->GetWidth()) / 2 ;
	m_pkSayWho->SetWindowPosDirect( vPos );    
}
