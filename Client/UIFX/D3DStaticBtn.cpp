#include "StdAfx.h"
#include "D3DStaticBtn.h"

IMPLEMENT_CLASSNAME( KD3DStaticBtn );

KD3DStaticBtn::KD3DStaticBtn(void)
{
	m_pkStatic = NULL;
	LINK_CONTROL( "static",		m_pkStatic );
}

KD3DStaticBtn::~KD3DStaticBtn(void)
{
}

void KD3DStaticBtn::OnCreate( void )
{
	m_pkStatic->InitState( true );
	m_pkStatic->SetAlign( DT_CENTER );
}

void KD3DStaticBtn::SetText( std::wstring strText )
{
	if( m_pkStatic == NULL )
		return;

	m_pkStatic->SetText( strText );
}

void KD3DStaticBtn::SetText( int iText )
{
	std::wstringstream str;
	str<<iText;
	SetText( str.str() );
}