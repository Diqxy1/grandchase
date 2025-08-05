#include "stdafx.h"
#include "GCNameTag.h"

IMPLEMENT_CLASSNAME( KGCNameTag );
IMPLEMENT_WND_FACTORY( KGCNameTag );
IMPLEMENT_WND_FACTORY_NAME( KGCNameTag, "gc_name_tag" );

KGCNameTag::KGCNameTag( void )
{
	/*m_pkBG = NULL;
	LINK_CONTROL( "background", m_pkBG );*/
	/*m_pkName = NULL;
	LINK_CONTROL( "name", m_pkName );*/
}

KGCNameTag::~KGCNameTag( void )
{
}

void KGCNameTag::ActionPerformed( const KActionEvent& event )
{
    /*if( event.m_pWnd == m_pkName )
        m_pkName->SetWndMode(D3DWM_HOVER);
    else
        m_pkName->SetWndMode(D3DWM_DEFAULT);*/
}

void KGCNameTag::OnCreate( void )
{
    //if( m_pkName )
    //{
    //    m_pkName->InitState( true );
    //}
	/*if (m_pkBG)
		m_pkBG->InitState(true);*/
	/*if (m_pkName)
	{
		m_pkName->InitState(true);
		m_pkName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );
		m_pkName->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
        m_pkName->SetWordWrap(false);
	}*/
}

// void KGCNameTag::SetName( const std::wstring& strName_ )
// {
// 	m_pkName->SetAlign( DT_CENTER );
//     m_pkName->SetText(strName_);
// 	OptimaizeBG_();
// }
// 
// void KGCNameTag::SetName( const int& iStringID_ )
// {
// 	m_pkName->SetTextFromID(iStringID_);
// 	OptimaizeBG_();
// }
// 
// void KGCNameTag::OptimaizeBG_()
// {
// 	POINT ptWidthHeight = m_pkName->GetStringWidthAndHeightInPixel();
// 	
// 	DWORD dwWidth = ptWidthHeight.x + GAP_LEFT + GAP_RIGHT;
// 	DWORD dwHeight = ptWidthHeight.y + GAP_TOP + GAP_BOTTOM;
// 
// 	m_pkBG->SetSize(dwWidth,dwHeight);
// 
//     
// 
// 	D3DXVECTOR2 vPos = m_pkBG->GetFixedWindowLocalPos();
//     vPos.x += (m_pkBG->GetWidth() - ptWidthHeight.x)/2;
// 	vPos.y += GAP_TOP + (2.0f * g_pGCDeviceManager->GetWindowScaleY());
// 
// 	m_pkName->SetFixedWindowLocalPos(vPos);
// 
// 	SetWidthDirect(dwWidth);
// 	SetHeightDirect(dwHeight);
// }
