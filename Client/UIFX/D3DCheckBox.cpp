#include "stdafx.h"
//#include "D3DCheckBox.h"

IMPLEMENT_CLASSNAME( KD3DCheckBox );

KD3DCheckBox::KD3DCheckBox( void )
: m_pWndCheck(NULL)
, m_pkStatic(NULL)
, m_pWndCheckBox(NULL)
{
	LINK_CONTROL("check",m_pWndCheck);
	LINK_CONTROL("static", m_pkStatic );
	LINK_CONTROL("check_back", m_pWndCheckBox );
	
}

KD3DCheckBox::~KD3DCheckBox( void )
{
}

void KD3DCheckBox::SetCheck( bool bCheck )
{
	m_pWndCheck->ToggleRender( bCheck );
}

bool KD3DCheckBox::GetCheck()
{
	return m_pWndCheck->IsRenderOn();
}

void KD3DCheckBox::SetStatic( const std::wstring& str)
{
    m_pkStatic->SetText( str );
}


void KD3DCheckBox::SetTwoStatic( const std::wstring& str, const std::wstring& str_2)
{
	m_pkStatic->SetText( str );
}

void KD3DCheckBox::SetStaticColor( D3DCOLOR color)
{
	m_pkStatic->SetFontColor(color);
}

void KD3DCheckBox::ToggleCheckBox( bool render)
{
	m_pWndCheck->ToggleRender(render && GetCheck());
	if(m_pWndCheckBox)
		m_pWndCheckBox->ToggleRender(render);

}

void KD3DCheckBox::CheckBoxLock( bool bLock )
{
    if(m_pWndCheckBox)
        m_pWndCheckBox->Lock(bLock);
}

void KD3DCheckBox::SetWordWrap( const std::wstring& str)
{
    m_pkStatic->SetForceWordWrapText( str, true);
    m_pkStatic->SetToolTip( str );
}
