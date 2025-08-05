#include "stdafx.h"
#include "GCAgitUserElement.h"

IMPLEMENT_CLASSNAME( KGCAgitUserElement );
IMPLEMENT_WND_FACTORY( KGCAgitUserElement );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitUserElement, "gc_agit_user_element" );

KGCAgitUserElement::KGCAgitUserElement( void )
: m_pkStaticName(NULL)
, m_dwUID(0)
, m_bIsRBtnClick(false)
, m_bIsLBtnClick(false)
, m_bIsInNameBG(false)
{
    LINK_CONTROL( "name", m_pkStaticName );
}

KGCAgitUserElement::~KGCAgitUserElement( void )
{
}

void KGCAgitUserElement::OnCreate( void )
{
    m_pkStaticName->InitState( true, true, this );
    m_pkStaticName->SetAlign( DT_LEFT );
}

void KGCAgitUserElement::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_pWnd == m_pkStaticName)
	{
		switch ( event.m_dwCode )
		{
		case D3DWE_CURSOR_ENTER:
			m_bIsInNameBG = true;
			break;

		case D3DWE_CURSOR_LEAVE:
			m_bIsInNameBG = false;
			break;

		case D3DWE_RBUTTON_CLICK:
			if ( m_bIsInNameBG )
				m_bIsRBtnClick = true;
			break;

		case D3DWE_BUTTON_CLICK:
			if ( m_bIsInNameBG )
				m_bIsLBtnClick = true;
			break;
		}
	}
}

void KGCAgitUserElement::SetUserName( std::wstring& strName_, DWORD dwUID )
{
    m_pkStaticName->SetText( strName_ );
	m_dwUID = dwUID;
}

const std::wstring& KGCAgitUserElement::GetUserName( void )
{
    return m_pkStaticName->GetText();
}

void KGCAgitUserElement::Clear( void )
{
    m_pkStaticName->SetText( L"" );
}

void KGCAgitUserElement::SetRClick( bool IsClick )
{
	m_bIsRBtnClick = IsClick;
}

void KGCAgitUserElement::SetLClick( bool IsClick )
{
	m_bIsLBtnClick = IsClick;
}

bool KGCAgitUserElement::IsEmpty( void )
{
	if ( L"" == m_pkStaticName->GetText() )
		return true;
	else
		return false;
}