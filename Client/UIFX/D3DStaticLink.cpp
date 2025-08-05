#include "stdafx.h"
#include ".\d3dstaticlink.h"
//

IMPLEMENT_CLASSNAME( KD3DStaticLink );

namespace{
	const int iStaticGap = 5;
}

KD3DStaticLink::KD3DStaticLink(void)
: m_dwDefColor(0xFFFFFFFF)
, m_dwHovColor(0xFFFFFF00)
, m_dwSelColor(0xFFFF0000)
, m_pCurSelect(NULL)
{
}

KD3DStaticLink::~KD3DStaticLink(void)
{
}

int KD3DStaticLink::AddLinkString( std::wstring strLink, bool bFixedSize, int iFixSize )
{
	KD3DStatic*	kLink = new KD3DStatic;

	kLink->SetSelfInputCheck( true );
	kLink->AddActionListener( this );
    kLink->ToggleRender( true );

    AddChildWithRegistToLua( kLink );

	kLink->SetWndState(D3DWS_ENABLED);

    if( false == bFixedSize )
    {
	kLink->SetText( strLink );
	kLink->SetWidthDirect( kLink->GetFontMgr()->GetWidth( strLink.c_str() ) );
	kLink->SetHeightDirect( kLink->GetFontMgr()->GetHeight( strLink.c_str() ) );
    }
    else
    {
        kLink->SetAlign( DT_CENTER );
        kLink->SetText( strLink );
        kLink->SetWidthDirect( iFixSize );
        kLink->SetHeightDirect( kLink->GetFontMgr()->GetHeight( strLink.c_str() ) );
    }

	if( m_listLinks.empty() )
	{
		kLink->SetWindowPosDirect( D3DXVECTOR2( 0, 0 ) );
	}
	else
	{
		KD3DStatic* kBack = m_listLinks.back();
		kLink->SetWindowPosDirect( kBack->GetFixedWindowLocalPos() + D3DXVECTOR2( (float)(kBack->GetWidth() + iStaticGap), .0f ) );
	}

    m_listLinks.push_back( kLink );

	return -1;
}

int KD3DStaticLink::RemoveLink( int iIndex )
{
	return -1;
}

void KD3DStaticLink::RefreshStringPos()
{

}

void KD3DStaticLink::ActionPerformed( const KActionEvent& event )
{
	std::list< KD3DStatic* >::iterator lit;
	for( lit = m_listLinks.begin(); lit != m_listLinks.end() ; ++lit )
	{
		if( event.m_pWnd == *lit )
			SpeakToActionListener( KActionEvent( this, event.m_dwCode, (DWORD)(*lit) , SL_ELEMENT_CLICK) );
	}
}

void KD3DStaticLink::OnCreate( void )
{

}

void KD3DStaticLink::FrameMoveInEnabledState( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	std::list< KD3DStatic* >::iterator lit;
	for( lit = m_listLinks.begin(); lit != m_listLinks.end() ; ++lit )
	{
		if( (*lit)->CheckPosInWindowBound( vMousePos ) )
			(*lit)->SetFontColor( m_dwHovColor );
		else 
			(*lit)->SetFontColor( m_dwDefColor );

        if( m_pCurSelect != NULL && m_pCurSelect == (*lit) )
        {
            (*lit)->SetFontColor( m_dwSelColor );
        }
	}
}

void KD3DStaticLink::Clear()
{
	std::list< KD3DStatic* >::iterator lit;
	for( lit = m_listLinks.begin(); lit != m_listLinks.end() ; ++lit )
	{
		SAFE_DELETE(*lit);
	}
	m_listLinks.clear();
	m_vecChild.clear();
}

void KD3DStaticLink::SelectLink( std::wstring strLink )
{
    std::list< KD3DStatic* >::iterator lit;
    for( lit = m_listLinks.begin(); lit != m_listLinks.end() ; ++lit )
    {
        if( (*lit)->GetText() == strLink )
        {
            m_pCurSelect = (*lit);
            break;
        }
    }
}
