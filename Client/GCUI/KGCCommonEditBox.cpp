#include "StdAfx.h"
#include "KGCCommonEditBox.h"

IMPLEMENT_CLASSNAME( KGCCommonEditBox );
IMPLEMENT_WND_FACTORY( KGCCommonEditBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCommonEditBox, "gc_common_editbox" );

KGCCommonEditBox::KGCCommonEditBox(void)
: m_pkScrollBar(NULL)
, m_pkEditBtn(NULL)
, m_pkOKBtn(NULL)
, m_pkLength(NULL)
, m_pkContents(NULL)
, m_pkContentsEdit(NULL)
, m_pkStaticTitle(NULL)
, m_dwColor(D3DCOLOR_RGBA(0,0,0,255))
, m_iLimitText(150)
, m_bUseScrollBar(true)
{
	LINK_CONTROL( "scroll", m_pkScrollBar );
	LINK_CONTROL( "edit_btn", m_pkEditBtn );
	LINK_CONTROL( "ok_btn", m_pkOKBtn );
	LINK_CONTROL( "length", m_pkLength );
	LINK_CONTROL( "contents", m_pkContents );
	LINK_CONTROL( "contents_edit", m_pkContentsEdit );
	LINK_CONTROL( "static_title", m_pkStaticTitle );

	LINK_CONTROL( "btn_close", m_pkBtnClose);
	LINK_CONTROL( "btn_cancel", m_pkBtnCancel );
}

KGCCommonEditBox::~KGCCommonEditBox(void)
{
}


void KGCCommonEditBox::OnCreate( void )
{
	m_pkEditBtn->InitState( true, true, this );
	m_pkOKBtn->InitState( false, true, this );

	m_pkLength->InitState( true, true, this );
	m_pkLength->SetAlign(DT_CENTER);

	m_pkContents->InitState( true, true, this );
#if defined(NATION_IDN)
    m_pkContents->SetLineSpace( 0.9f );
#else
	m_pkContents->SetLineSpace( 1.2f );
#endif

	m_pkScrollBar->InitState( true, true, this );

	m_pkContentsEdit->InitState( false, true, this );
	m_pkContentsEdit->SetMultiLine( true );
	m_pkContentsEdit->SetNewLine( true );

	m_pkStaticTitle->InitState(true);

	m_pkBtnCancel->InitState(false, true, this);
	m_pkBtnClose->InitState(true, true, this);

}

void KGCCommonEditBox::OnCreateComplete( void )
{
	m_pkScrollBar->SetScrollPageSize( m_pkContents->GetDisplayLimitLine() );
    m_pkScrollBar->SetScrollRangeMax( m_pkContents->GetViewContentSize() );
	m_pkScrollBar->SetScrollPos( m_pkContents->GetTopIndex() );
}

void KGCCommonEditBox::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkContentsEdit,	KD3DWnd::D3DWE_EDIT_RETURN, OnEditPushEnter );
	GCWND_MSG_MAP( m_pkEditBtn,			KD3DWnd::D3DWE_BUTTON_CLICK, OnEdit );
	GCWND_MSG_MAP( m_pkOKBtn,			KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
	GCWND_MSG_MAP( m_pkBtnCancel,		KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel );
	GCWND_MSG_MAP( m_pkBtnClose,		KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );

	if( event.m_pWnd == m_pkScrollBar )
	{
		OnScroll();
	}
}

void KGCCommonEditBox::OnDestroyComplete( void )
{
}

void KGCCommonEditBox::FrameMoveInEnabledState( void )
{
	if ( m_pkContentsEdit->HasFocus() && g_pkInput->IsDown( DIK_ESCAPE ) )
	{
		SetEditMode( false );
        g_pkInput->IgnoreDown(DIK_ESCAPE,30);
	}

	//확인버튼이 랜더되어진 상태라면 에디트모드라 본다.
	if( m_pkOKBtn->IsRenderOn() )
	{
		WCHAR strTemp[20];
		std::wstring strCurNum = m_pkContentsEdit->GetText();		
		swprintf( strTemp, 20, L"%d / %d", strCurNum.length(), m_iLimitText );		
		m_pkLength->SetText(strTemp);
	}

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	if( m_pkContents->CheckPosInWindowBound(vMousePos) )
	{
		if( g_pkInput->ISWheelUp() )
		{
			if( m_pkScrollBar->GetScrollPos() > 0 )
			{
				m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos() - 1);
				g_KDSound.Play( "73" );
				m_pkContents->SetTopIndex( m_pkScrollBar->GetScrollPos() );
			}
		}
		else if( g_pkInput->ISWheelDown() )
		{
			if( m_pkScrollBar->GetScrollPos() < m_pkScrollBar->GetScrollRangeMax() )
			{
				m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos() + 1);
				g_KDSound.Play( "73" );
				m_pkContents->SetTopIndex( m_pkScrollBar->GetScrollPos() );
			}
		}	
	}
}

void KGCCommonEditBox::PostChildDraw( void )
{
}

void KGCCommonEditBox::OnOK( void )
{
	if( !IsRenderOn() )
		return;

	std::wstring strContents = m_pkContentsEdit->GetText();
	if((int)strContents.length() > m_iLimitText)
	{
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_MAXIMUM_LENGTH_REACHED), L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
	}	

	SendPacket(m_iEditType);

	m_pkContents->Clear();
	m_pkContents->AddString( m_strPreData, m_dwColor );
	m_pkScrollBar->SetScrollPageSize( m_pkContents->GetDisplayLimitLine() );
	m_pkScrollBar->SetScrollRangeMax( m_pkContents->GetViewContentSize() );
	m_pkScrollBar->SetScrollPos( 0 );

	m_pkContents->SetTopIndex( 0 );

	WCHAR strTemp[20];
	swprintf( strTemp, 20, L"%d / %d", strContents.length(), m_iLimitText );		
	m_pkLength->SetText(strTemp);

	SetEditMode( false );
	g_KDSound.Play( "31" );
}

void KGCCommonEditBox::OnEdit( void )
{
	m_strPreData = m_pkContentsEdit->GetText();
	SetEditMode( true );
	m_pkContentsEdit->SetFocus();
	g_KDSound.Play( "31" );
}

void KGCCommonEditBox::SetEditMode( bool bEdit_ )
{
	m_pkEditBtn->ToggleRender( !bEdit_ );
	m_pkBtnCancel->ToggleRender( bEdit_ );
	m_pkOKBtn->ToggleRender( bEdit_ );
	m_pkContents->ToggleRender( !bEdit_ );
	m_pkContentsEdit->ToggleRender( bEdit_ );
	m_pkContentsEdit->SetText( m_pkContents->GetAllMsg().c_str() );
}

void KGCCommonEditBox::OnScroll( void )
{
	if( !m_bUseScrollBar )
		return;

	if( m_pkContents->GetTopIndex() != m_pkScrollBar->GetScrollPos() )
	{
		g_KDSound.Play( "73" );
		m_pkContents->SetTopIndex( m_pkScrollBar->GetScrollPos() );
	}
}

void KGCCommonEditBox::OnEditPushEnter( void )
{
	//     std::wstring strEdit = m_pkContentsEdit->GetText();
	//     strEdit += L"\n";
	//     m_pkContentsEdit->SetText( strEdit.c_str(), false );
	m_pkContentsEdit->SetFocus();
}

void KGCCommonEditBox::SetEditBoxLimitLine( int iLimitLine_ )
{
	m_pkContentsEdit->SetLimitLine(iLimitLine_);
}

void KGCCommonEditBox::SetTextColor( D3DCOLOR dwColor_ )
{
	m_dwColor = dwColor_;
	m_pkContentsEdit->SetFontColor(dwColor_);
}


void KGCCommonEditBox::SetDefaultText( std::wstring str_ )
{
	m_pkContents->Clear();
	m_pkContents->AddString(str_, m_dwColor);

	WCHAR strTemp[20];
	swprintf( strTemp, 20, L"%d / %d", str_.length(), m_iLimitText );		
	m_pkLength->SetText(strTemp);
	m_pkScrollBar->SetScrollPageSize( m_pkContents->GetDisplayLimitLine() );
	m_pkScrollBar->SetScrollRangeMax( m_pkContents->GetViewContentSize() );
	m_pkScrollBar->SetScrollPos( 0 );
	m_pkContents->SetTopIndex(0);
}

void KGCCommonEditBox::SetEditBoxLimitText( int iLimitText_)
{	
	m_iLimitText = iLimitText_;
	m_pkContentsEdit->SetLimitText(iLimitText_ );
}

void KGCCommonEditBox::SetTitleText( std::wstring str_, DWORD dwAlign_, D3DCOLOR dwCololr_)
{
	m_pkStaticTitle->SetAlign(dwAlign_);
	m_pkStaticTitle->SetFontColor(dwCololr_);
	m_pkStaticTitle->SetText(str_);
}

void KGCCommonEditBox::OnClose()
{
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE) );
	g_KDSound.Play( "31" );
}

void KGCCommonEditBox::OnCancel()
{
	SetEditMode(false);
	g_KDSound.Play( "31" );
}

// 공용으로 쓰려고 만들긴 했지만, 이 클래스안에서 패킷을 보내야 하기 때문에 이딴 함수를 만들 수 밖에 없네요...
void KGCCommonEditBox::SendPacket( int iPacketType_ )
{
	std::wstring buf =  m_pkContentsEdit->GetText();
	if(buf == m_strPreData)
		return;
	m_strPreData = buf;
	switch( iPacketType_ )
	{
	case ECEB_COMMENT:
	case ECEB_NOTICE1:
	case ECEB_NOTICE2:
		KP2P::GetInstance()->Send_EditGuildNoticeReq( m_iEditType, m_pkContentsEdit->GetText());
		break;
	case ECEB_MY_INTRODUCE:
		KP2P::GetInstance()->Send_EditGuildMyCommentReq( g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID, m_pkContentsEdit->GetText());
		break;
	}
}

void KGCCommonEditBox::RenderEditBtn( bool bRender_ )
{
	m_pkEditBtn->ToggleRender(bRender_);
}