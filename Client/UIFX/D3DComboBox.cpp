#include "stdafx.h"
//#include "D3DComboBox.h"
//
//
//

IMPLEMENT_CLASSNAME( KD3DComboBox );

KD3DComboBox::KD3DComboBox( void )
{
	m_pkComboMain	= NULL;
	m_pkComboPopup	= NULL;
	m_pkStaticCombo	= NULL;

	m_iCurSel		= -1;

	LINK_CONTROL( "combobox_main",  m_pkComboMain );
	LINK_CONTROL( "combobox_popup", m_pkComboPopup );
	LINK_CONTROL( "main_static",    m_pkStaticCombo );
    m_dwAlign = DT_LEFT;

	m_CurrentColor = D3DCOLOR_ARGB(255,255,255,0);
	m_NormalColor = D3DCOLOR_ARGB(255,255,255,255);

}

KD3DComboBox::~KD3DComboBox( void )
{
}

void KD3DComboBox::OnCreate( void )
{
	m_pkComboMain->AddActionListener(this);
	m_pkComboMain->ToggleRender(true);
	m_pkComboMain->SetSelfInputCheck(true);

	m_pkComboPopup->ToggleRender(false);
	m_pkComboPopup->SetSelfInputCheck(true);

	m_pkStaticCombo->ToggleRender(true);
}

void KD3DComboBox::ActionPerformed( const KActionEvent& event )
{
    if( this->IsLocked() )
        return;

	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkComboMain )
		{
			m_pkComboPopup->ToggleRender(!m_pkComboPopup->IsRenderOn());
			return;
		}

		for( int i = 9; i < (int)m_pkComboPopup->GetNumChild(); ++i )
		{	
			if( event.m_pWnd == m_pkComboPopup->GetChild(i) )
			{
				m_pkComboPopup->ToggleRender(false);

				if(m_iCurSel == i-9)				
					return;

				SetCurSel( i - 9 );
				return;
			}
		}
	}
}

void KD3DComboBox::SetComboBoxColor(D3DCOLOR currentColor, D3DCOLOR normalColor )
{
	m_CurrentColor = currentColor;
	m_NormalColor = normalColor;
}

void KD3DComboBox::FrameMoveInEnabledState( void )
{
	if(!m_pkComboPopup->IsRenderOn())
		return;

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );


	KD3DStatic* pChildStatic = NULL;
	for( int i = 9; i < (int)m_pkComboPopup->GetNumChild(); i++)
	{
		pChildStatic = (KD3DStatic*)m_pkComboPopup->GetChild(i);
		if( pChildStatic->CheckPosInWindowBound( vMousePos ) )
			pChildStatic->SetFontColor( m_CurrentColor );
		else 
			pChildStatic->SetFontColor( m_NormalColor );

	}

	if( !g_pkInput->BtnUp( KInput::MBLEFT ) )
		return;	

	if ( CheckPosInWindowBound( vMousePos ) )
		return;

	m_pkComboPopup->ToggleRender(false);
}

void KD3DComboBox::AddString( std::wstring cName, DWORD dwAlign_ )
{
	KD3DStatic* pStatic = new KD3DStatic;

	pStatic->SetSelfInputCheck( true );
	pStatic->AddActionListener( this );
	pStatic->ToggleRender( true );
    pStatic->SetAlign( dwAlign_ );
    pStatic->SetAlignType( m_pkComboPopup->GetAlignType() );

    std::stringstream stm;
    stm<<"static"<<m_pkComboPopup->GetNumChild();
    pStatic->SetWindowName( stm.str() );

	m_pkComboPopup->AddChildWithRegistToLua(pStatic);

	D3DXVECTOR2 vStaticOffSet( 0, (float)(m_pkComboPopup->GetNumChild() - 10) * 17 + 5);

	PopupHeightChange();
    if ( dwAlign_ == DT_LEFT)
	    pStatic->SetOffset(D3DXVECTOR2(5,0));
    else
        pStatic->SetOffset(D3DXVECTOR2(0,0));
	pStatic->SetWindowPos(vStaticOffSet);
	pStatic->SetWndState(D3DWS_ENABLED);
	pStatic->SetWidth(m_pkComboPopup->GetWidth());
	pStatic->SetHeight(15);
	pStatic->SetText( cName );	//텍스트를 마지막에 셋팅해 주어야 합니다. 그래야 Width 속성이 먹힘
}

void KD3DComboBox::PopupHeightChange()
{
	const int iFrameThick = 2;
	int iHeight = (m_pkComboPopup->GetNumChild() - 9) * 17;
	m_pkComboPopup->SetHeight( iHeight + iFrameThick * 2 );

	for(int i = 3; i < 9; i++)
	{
		if( i == 3 || i == 4 || i == 8 )
		{
			m_pkComboPopup->GetChild(i)->SetHeight(iHeight);
		}
		else
		{
			D3DXVECTOR2 wndpos = m_pkComboPopup->GetChild(i)->GetFixedWindowLocalPos();
			wndpos.y = (float)(iFrameThick + iHeight);
			m_pkComboPopup->GetChild(i)->SetFixedWindowLocalPos( wndpos );
		}
	}
}

void KD3DComboBox::SetComboStatic( std::wstring str, DWORD dwAlign_ )
{
	m_pkStaticCombo->SetText( str );
    m_pkStaticCombo->SetAlign( dwAlign_ );
}

void KD3DComboBox::DeleteAllItem()
{
	m_iCurSel = -1;

	while( (int)m_pkComboPopup->GetNumChild() >= 10 )
	{
		m_pkComboPopup->DeleteChild( 9 );
	}
}

void KD3DComboBox::ShowComboBoxList(bool bShow)
{
    m_pkComboPopup->ToggleRender(bShow);
}

std::wstring KD3DComboBox::GetCurStaticText(void)
{
    return m_pkStaticCombo->GetText();
}

void KD3DComboBox::OnDestroy( void )
{
	//DeleteAllItem();
}

void KD3DComboBox::SetCurSel( int iSel )
{
	if( m_pkComboPopup->GetNumChild() - 9 <= (UINT)iSel )
		return;

	if( iSel < 0 )
		return;

	m_iCurSel = iSel;
	SetComboStatic( ((KD3DStatic*)m_pkComboPopup->GetChild( iSel + 9 ))->GetText(), m_dwAlign );
	SpeakToActionListener( KActionEvent( this, D3DWE_COMBO_SELCHANGE ) );
}
