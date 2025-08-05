#include "stdafx.h"
#include ".\GCInviteSquareInfo.h"
//
//

IMPLEMENT_CLASSNAME( KGCInviteSquareInfo );
IMPLEMENT_WND_FACTORY( KGCInviteSquareInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteSquareInfo, "gc_invite_square_info" );

KGCInviteSquareInfo::KGCInviteSquareInfo(void)
: m_bEmpty(true)
, m_pkBack(NULL)
, m_pkArrow(NULL)
{
    LINK_CONTROL( "square_info_back",   m_pkBack );
    LINK_CONTROL( "square_info_static", m_pkInfo );
    LINK_CONTROL( "arrow",              m_pkArrow );
}

KGCInviteSquareInfo::~KGCInviteSquareInfo(void)
{

}

void KGCInviteSquareInfo::OnCreate( void )
{
    m_pkBack->InitState( true );
    m_pkInfo->InitState( true );
    m_pkInfo->SetText( L"" );
    m_pkArrow->InitState( false );
}

void KGCInviteSquareInfo::InitSquareInfo()
{
    m_bEmpty = true;
    m_pkInfo->SetText( L"" );
    m_pkArrow->ToggleRender( false );
    SetBackMode( D3DWM_DEFAULT );
}

void KGCInviteSquareInfo::SetSquareInfo( const KSquareInfo& kInfo )
{
    m_bEmpty = false;

    std::wstringstream stm;
    stm << kInfo.m_strName << L" (" << kInfo.m_dwCurrentUserNum << L"/" << kInfo.m_dwMaxUserNum << L")";
    m_pkInfo->SetText( stm.str() );
    m_pkInfo->ToggleRender( true );

    m_kInfo = kInfo;
}

void KGCInviteSquareInfo::SetArrow( bool bArrow )
{
    m_pkArrow->ToggleRender( bArrow );
}

void KGCInviteSquareInfo::SetBackMode( ED3DWndMode eMode )
{
    m_pkBack->SetWndMode( eMode, false );
}

KD3DWnd::ED3DWndMode KGCInviteSquareInfo::GetBackMode() const
{
    return m_pkBack->GetWndMode();
}