#include "stdafx.h"
#include "KGCGuildCreateDlg.h"

IMPLEMENT_CLASSNAME( KGCGuildCreateDlg );
IMPLEMENT_WND_FACTORY( KGCGuildCreateDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildCreateDlg, "gc_guild_create_dlg" );

KGCGuildCreateDlg::KGCGuildCreateDlg(void)
: m_pkGuildCreateInfoDesc( NULL )
, m_pkGuildCreateInfoName( NULL )
, m_pkGuildCreateNameEdit( NULL )
, m_pkGuildCreateURLEdit( NULL )
, m_pkConfirmBtn( NULL )
, m_pkCancelBtn( NULL )
{
	char strCltrName[MAX_PATH];

	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
	{
		sprintf( strCltrName, "guild_create_dlg_info%d", i + 1 );
		LINK_CONTROL( strCltrName,		m_apkGuildCreateDlgInfo[i] );
		sprintf( strCltrName, "guild_create_dlg_scroll%d", i + 1 );
		LINK_CONTROL( strCltrName,		m_apkGuildCreateDlgScroll[i] );    
	}

	LINK_CONTROL( "guild_create_info_list",			m_pkGuildCreateInfoDesc );
	LINK_CONTROL( "guild_create_info_static2",		m_pkGuildCreateInfoName );
	
	LINK_CONTROL( "guild_create_name_edit",			m_pkGuildCreateNameEdit );
    LINK_CONTROL( "guild_create_URL_edit",			m_pkGuildCreateURLEdit );

	LINK_CONTROL( "confirm_btn",					m_pkConfirmBtn );
	LINK_CONTROL( "cancel_btn",						m_pkCancelBtn );
}

KGCGuildCreateDlg::~KGCGuildCreateDlg(void)
{
}

void KGCGuildCreateDlg::OnCreate()
{
	m_pkConfirmBtn->InitState( true, true, this );
	m_pkCancelBtn->InitState( true, true, this );

	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
	{
		m_apkGuildCreateDlgInfo[i]->InitState( true, true, this );
		m_apkGuildCreateDlgInfo[i]->SetLineSpace( 1.2f );
		m_apkGuildCreateDlgInfo[i]->Clear();
		m_apkGuildCreateDlgInfo[i]->SetTopIndex( 0 );
	}

	//	스트링을 채운다.
	//	길드 생성 조건
	std::wstring	strLinefeed(L"\r\n");
	m_pkGuildCreateInfoName->SetText( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_CONDITION_1 ) );
    m_pkGuildCreateInfoName->SetFontColor( D3DCOLOR_RGBA(184,134,11,255) );

	//	길드 생성 조건 상세 정보
	m_pkGuildCreateInfoDesc->Clear();
//	m_pkGuildCreateInfoDesc->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_CONDITION_2 ) );
	m_pkGuildCreateInfoDesc->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_CONDITION_3 ) );
	m_pkGuildCreateInfoDesc->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_CONDITION_4 ) );

	//	길드 이름 정보
	m_apkGuildCreateDlgInfo[0]->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_NAME_INFO_1 ) );
	m_apkGuildCreateDlgInfo[0]->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_NAME_INFO_2 ) );

	//	길드 URL 정보
	m_apkGuildCreateDlgInfo[1]->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_URL_INFO_1 ) );

	//	길드 만들기 상세 정보
	m_apkGuildCreateDlgInfo[2]->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_MORE_INFO_1 ) );
	m_apkGuildCreateDlgInfo[2]->AddString( strLinefeed );
	m_apkGuildCreateDlgInfo[2]->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_MORE_INFO_2 ) );
	m_apkGuildCreateDlgInfo[2]->AddString( strLinefeed );
	m_apkGuildCreateDlgInfo[2]->AddString( g_pkStrLoader->GetString( STR_ID_NEW_GUILD_CREATE_MORE_INFO_3 ) );

	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
		m_apkGuildCreateDlgScroll[i]->InitState( true, true, this );

	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
		m_apkGuildCreateDlgScroll[i]->SetScrollPageSize( m_apkGuildCreateDlgInfo[i]->GetDisplayLimitLine() );

	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
		m_apkGuildCreateDlgScroll[i]->SetScrollRangeMax( m_apkGuildCreateDlgInfo[i]->GetViewContentSize() );

	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
		m_apkGuildCreateDlgScroll[i]->SetScrollPos( 0 );
		//m_apkGuildCreateDlgScroll[i]->SetScrollPos( m_apkGuildCreateDlgInfo[i]->GetTopIndex() );

	m_pkGuildCreateInfoDesc->InitState( true, true, this );
	m_pkGuildCreateInfoName->InitState( true, true, this );
	m_pkGuildCreateNameEdit->InitState( true, true, this );
	m_pkGuildCreateNameEdit->SetText(L"");
    m_pkGuildCreateNameEdit->SetLimitText(MAX_GUILDNAME_SIZE);
	m_pkGuildCreateURLEdit->InitState( true, true, this );

#if defined (NATION_KOREA)
    m_pkGuildCreateURLEdit->SetLimitText(100);
#endif
	m_pkGuildCreateURLEdit->SetText(L"http://");
    m_pkGuildCreateURLEdit->SetWidth( 281 );
    m_pkGuildCreateURLEdit->SetWindowPosDirect( D3DXVECTOR2( 297.0f, m_pkGuildCreateURLEdit->GetFixedWindowLocalPos().y) );
    KD3DWnd* pChild = GetChildByName( "guild_create_frame10");
    if( pChild )
    {
        pChild->SetWidth( 281 );
        pChild->SetWindowPosDirect( D3DXVECTOR2( 297.0f, pChild->GetFixedWindowLocalPos().y) );
    }

	m_pkConfirmBtn->InitState( true, true, this );
	m_pkCancelBtn->InitState( true, true, this );

    m_pkGuildCreateInfoName->SetAlign( DT_CENTER );

	//	스크롤 바 이벤트
	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
	{
		if( m_apkGuildCreateDlgInfo[i]->GetTopIndex() != m_apkGuildCreateDlgScroll[i]->GetScrollPos() ){
			m_apkGuildCreateDlgInfo[i]->SetTopIndex( m_apkGuildCreateDlgScroll[i]->GetScrollPos() );		
		}																								
	}
}


void KGCGuildCreateDlg::ActionPerformed( const KActionEvent& event )
{
	//	버튼 클릭 이벤트
	if( event.m_pWnd ==	m_pkConfirmBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
		std::wstring strUrl = L"http://";
		std::wstring strGuildName = m_pkGuildCreateNameEdit->GetText();		

		if( strGuildName.length() == 0 )
		{
            SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_ENCHANT_FAIL5),
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_CREATE_NAME_INFO_1), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
		}		
		else if( IsExistBlank(strGuildName) )
		{
            SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, g_pkStrLoader->GetString(STR_ID_ENCHANT_FAIL5),
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_CREATE_EXIST_BLANK), KGCUIScene::GC_MBOX_USE_NORMAL, KGCGuildPublicMbox::MBR_CONFIRM_ONLY, 0, false, true );
        }
		else
		{			
			if( strUrl == m_pkGuildCreateURLEdit->GetText() )
			{
				//url이 없으면 공백으로 만들자( 기본이 "http://" 다보니...)
				m_pkGuildCreateURLEdit->SetText(L"");
			}
			KP2P::GetInstance()->Send_CreateGuildReq( m_pkGuildCreateNameEdit->GetText(), m_pkGuildCreateURLEdit->GetText() );
			m_pkGuildCreateNameEdit->SetText(L"");
			m_pkGuildCreateURLEdit->SetText(L"http://");
			SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
		}
	}

	if( event.m_pWnd ==	m_pkCancelBtn && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
        m_pkGuildCreateNameEdit->SetText(L"");
		m_pkGuildCreateURLEdit->SetText(L"http://");
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}

	//	스크롤 바 이벤트
	for( int i = 0; i < NUMOFSCROLLBAR; i++ )
	{
		if( event.m_pWnd == m_apkGuildCreateDlgInfo[i] || event.m_pWnd == m_apkGuildCreateDlgScroll[i] ){
		//if( event.m_pWnd == m_apkGuildCreateDlgScroll[i] ){
			if( m_apkGuildCreateDlgInfo[i]->GetTopIndex() != m_apkGuildCreateDlgScroll[i]->GetScrollPos() ){																								
				g_KDSound.Play( "73" );																			
				m_apkGuildCreateDlgInfo[i]->SetTopIndex( m_apkGuildCreateDlgScroll[i]->GetScrollPos() );		
			}																								
		}
	}
}

void KGCGuildCreateDlg::OnCreateComplete()
{
}


void KGCGuildCreateDlg::SetInfoText( std::wstring& rText, const int index, bool bClear )
{
	if( bClear ) m_apkGuildCreateDlgInfo[index]->Clear();
	m_apkGuildCreateDlgInfo[index]->AddString(rText);
}


//------------------------------------------------------------------------
//	NAME : FrameMoveInEnbledState()
//	DESC :
//------------------------------------------------------------------------
void KGCGuildCreateDlg::FrameMoveInEnabledState( void )
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	//#	윈도우 바운드 안에 마우스가 위치 했다면
	if( m_apkGuildCreateDlgInfo[LISTBOSGUILDINFO]->CheckPosInWindowBound( vMousePos ) )
	{
		if( g_pkInput->ISWheelUp() )
		{
			if( m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->GetScrollPos() > 0 )
			{
				m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->SetScrollPos(m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->GetScrollPos() - 1);
				g_KDSound.Play( "73" );
				m_apkGuildCreateDlgInfo[LISTBOSGUILDINFO]->SetTopIndex( m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->GetScrollPos() );
			}
		}
		else if( g_pkInput->ISWheelDown() )
		{
			if( m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->GetScrollPos() < m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->GetScrollRangeMax() )
			{
				m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->SetScrollPos(m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->GetScrollPos() + 1);
				g_KDSound.Play( "73" );
				m_apkGuildCreateDlgInfo[LISTBOSGUILDINFO]->SetTopIndex( m_apkGuildCreateDlgScroll[LISTBOSGUILDINFO]->GetScrollPos() );
			}
		}	
	}
}

bool KGCGuildCreateDlg::IsExistBlank(std::wstring& str_)
{
	// 공백을 검사해 주겠다!
	for( int i = 0; i < (int)str_.size(); i++ )
	{
		if( str_[i] == L' ' || str_[i] == 160 )
			return true;
	}
	return false;
}