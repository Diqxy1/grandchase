#include "stdafx.h"
#include ".\GCBarControl.h"

IMPLEMENT_CLASSNAME( KGCBarControl );
IMPLEMENT_WND_FACTORY( KGCBarControl );
IMPLEMENT_WND_FACTORY_NAME( KGCBarControl, "gc_bar_control" );

KGCBarControl::KGCBarControl(void)
: m_pkLeft(NULL)
, m_pkCenter(NULL)
, m_pkRight(NULL)
, m_pkGage(NULL)
, m_pkGageEnd(NULL)
, m_dwGageWidth(0)
{
    LINK_CONTROL( "left",       m_pkLeft );
    LINK_CONTROL( "center",     m_pkCenter );
    LINK_CONTROL( "right",      m_pkRight );
    LINK_CONTROL( "gage",       m_pkGage );
    LINK_CONTROL( "gage_end",   m_pkGageEnd );
}

KGCBarControl::~KGCBarControl(void)
{

}

void KGCBarControl::OnCreate()
{
    m_pkLeft->InitState( true );
    m_pkCenter->InitState( true );
    m_pkRight->InitState( true );
    m_pkGage->InitState( true );
    m_pkGageEnd->InitState( true );
    // [2/25/2008] breadceo. 이 길이가 전체 길이가 됨
    m_dwGageWidth = m_pkCenter->GetWidth();
}

void KGCBarControl::OnCreateComplete()
{
    D3DXVECTOR2 vGagePos = m_pkLeft->GetFixedWindowLocalPos();
    vGagePos.x += static_cast<float>(m_pkLeft->GetWidth());
    m_pkGage->SetFixedWindowLocalPos( vGagePos );
    m_pkGageEnd->SetFixedWindowLocalPos( vGagePos );
}

void KGCBarControl::SetGage( float fPercent )
{
    D3DXVECTOR2 vGagePos = m_pkLeft->GetFixedWindowLocalPos();
    vGagePos.x += static_cast<float>(m_pkLeft->GetWidth());
    float fWidth = static_cast<float>(m_dwGageWidth) * fPercent;
    m_pkGage->SetWidthDirect( (DWORD)fWidth );
    vGagePos.x += static_cast<float>(m_pkGage->GetWidth());
    m_pkGageEnd->SetFixedWindowLocalPos( vGagePos );
}