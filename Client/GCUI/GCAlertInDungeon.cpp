#include "stdafx.h"
#include "GCAlertInDungeon.h"
#include "MyD3D.h"

#include <stdarg.h>

IMPLEMENT_CLASSNAME( KGCAlertInDungeon );
IMPLEMENT_WND_FACTORY( KGCAlertInDungeon );
IMPLEMENT_WND_FACTORY_NAME( KGCAlertInDungeon, "gc_alertindungeon" );

KGCAlertInDungeon::KGCAlertInDungeon( void )
{

	m_pkBack = NULL;
	m_pkStaticDesc = NULL;
    LINK_CONTROL( "back",   m_pkBack );
    LINK_CONTROL( "static_desc", m_pkStaticDesc );

	m_fTime = 0.0f;

}

KGCAlertInDungeon::~KGCAlertInDungeon( void )
{
}

void KGCAlertInDungeon::ActionPerformed( const KActionEvent& event )
{

}

void KGCAlertInDungeon::OnCreate( void )
{
	m_pkBack->InitState(true);
	m_pkStaticDesc->InitState(true);

#if defined(NATION_PHILIPPINE)
    m_pkStaticDesc->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
#endif 

#if !defined( NATION_USA )
	m_pkStaticDesc->SetFontSpace(2);
#endif

    m_pkStaticDesc->SetFontColor( D3DCOLOR_ARGB(255, 255, 249, 217) );
	m_pkStaticDesc->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
	m_pkStaticDesc->SetAlign(DT_CENTER);
    m_pkStaticDesc->SetMultiLine( true );
}

void KGCAlertInDungeon::SetStaticDesc( const int iStringNum_ )
{
	SetStaticDesc(g_pkStrLoader->GetString( iStringNum_ ));
}

void KGCAlertInDungeon::SetStaticDesc( const std::wstring& strTitleText_ )
{
	ToggleRender(true);
	m_pkStaticDesc->SetTextAutoMultiline(strTitleText_);
	m_fTime = 0.0f;

	//DWORD strWidth = g_pkFontMgr->GetWidth(strTitleText_.c_str());
	//m_pkStaticDesc->SetWindowPosX(0.5f * ( GC_SCREEN_WIDTH * m_fWindowScaleX - strWidth ));	
}

void KGCAlertInDungeon::ProcessTitle()
{
	if (m_fTime < 0.7f)
		m_fTime += 0.15f * GC_ELAPSED_TIME;
	else
		ToggleRender(false);

}
