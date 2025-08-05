#include "StdAfx.h"
#include ".\gcskillboxgroup.h"

IMPLEMENT_CLASSNAME( KGCSkillBoxGroup );
IMPLEMENT_WND_FACTORY( KGCSkillBoxGroup );
IMPLEMENT_WND_FACTORY_NAME( KGCSkillBoxGroup, "gc_skillbox_group" );

KGCSkillBoxGroup::KGCSkillBoxGroup(void)
: m_pkTitle( NULL )
, m_pkBackground( NULL )
, m_pkTitleBack( NULL )
{
	LINK_CONTROL( "static_title", m_pkTitle );
	LINK_CONTROL( "sizing_background", m_pkBackground );
	LINK_CONTROL( "sizing_title", m_pkTitleBack );
}

KGCSkillBoxGroup::~KGCSkillBoxGroup(void)
{
}

void KGCSkillBoxGroup::SetTitle( std::wstring strTitle )
{
	m_pkTitle->SetText( strTitle );
	m_pkTitle->SetWidth( g_pkFontMgr->GetWidth( strTitle.c_str() ) + 6 );
	m_pkTitleBack->SetWidth( g_pkFontMgr->GetWidth( strTitle.c_str() ) + 6 );
}

void KGCSkillBoxGroup::SetSize( DWORD dwWidth, DWORD dwHeight )
{
	this->SetWidthDirect( dwWidth );
	this->SetHeightDirect( dwHeight );
	m_pkBackground->SetSize( dwWidth, dwHeight );
}