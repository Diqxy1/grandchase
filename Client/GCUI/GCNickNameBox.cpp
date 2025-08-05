#include "stdafx.h"
#include "GCNickNameBox.h"


#include "SlangFilter.h"
IMPLEMENT_CLASSNAME( KGCNickNameBox );
IMPLEMENT_WND_FACTORY( KGCNickNameBox );
//IMPLEMENT_WND_FACTORY_NAME( KGCNickNameBox, "gc_nickname_box" );
IMPLEMENT_WND_FACTORY_NAME( KGCNickNameBox, "gc_recommend_box" );

KGCNickNameBox::KGCNickNameBox( void )
{
    m_dwUse         = 0;
    m_iItemID       = 0;
    m_dwItemUID     = 0;
    m_strNickName[0].erase();
    m_strNickName[1].erase();

    m_bNickRegComplete      = true;
    m_bRecommendRegComplete = true;
    m_bUseChangeNickName    = false;
    m_bIsNickName           = true;

#define WINDOW_NUM 2
    for( int i=0; i < 2; i++)
    {
        m_pkBackGround[i]   = NULL;
        m_pkBackFront[i]    = NULL;
        m_pkBackNickname[i] = NULL;
        m_pkEditNick[i]     = NULL;
        m_pkWndNickName[i]  = NULL;
        m_pkWndWelcome[i]   = NULL;
        m_pkBtnOk[i]        = NULL;
        m_pkStaticCheckbox[i]= NULL;
        m_pkCheckBox[i]     = NULL;
    }
    
    m_pkBackDesc            = NULL;
    m_pkBackBottom          = NULL;
    m_pkStaticDesc          = NULL;      
    m_pkStaticBottom        = NULL;    

    // 두개씩 밖에 없어.. 그냥 손으로 씀
    LINK_CONTROL( "back0",		            m_pkBackGround[0] );
    LINK_CONTROL( "back1",		            m_pkBackGround[1] );
    LINK_CONTROL( "backfront0",		        m_pkBackFront[0] );
    LINK_CONTROL( "backfront1",		        m_pkBackFront[1] );
    LINK_CONTROL( "back_nickname0",         m_pkBackNickname[0] );
    LINK_CONTROL( "back_nickname1",         m_pkBackNickname[1] );
    LINK_CONTROL( "win_nickname0",          m_pkWndNickName[0] );
    LINK_CONTROL( "win_nickname1",          m_pkWndNickName[1] );
    LINK_CONTROL( "btn_ok0",                m_pkBtnOk[0] );
    LINK_CONTROL( "btn_ok1",                m_pkBtnOk[1] );
    LINK_CONTROL( "edit_nickname0",         m_pkEditNick[0]);
    LINK_CONTROL( "edit_nickname1",         m_pkEditNick[1]);
    LINK_CONTROL( "win_welcome",            m_pkWndWelcome[0]);
    LINK_CONTROL( "win_with_friend",        m_pkWndWelcome[1]);

    LINK_CONTROL( "back_desc",              m_pkBackDesc);
    LINK_CONTROL( "back_bottom",            m_pkBackBottom);
    LINK_CONTROL( "static_desc",            m_pkStaticDesc);
    LINK_CONTROL( "static_bottom",          m_pkStaticBottom);
    LINK_CONTROL( "static_recommend0",      m_pkStaticCheckbox[0]);
    LINK_CONTROL( "static_recommend1",      m_pkStaticCheckbox[1]);
    LINK_CONTROL( "checkbox_recommend0",    m_pkCheckBox[0] );
    LINK_CONTROL( "checkbox_recommend1",    m_pkCheckBox[1] );
    LINK_CONTROL( "scroll",                 m_pkScroll);
}

KGCNickNameBox::~KGCNickNameBox( void )
{
    // empty
}

void KGCNickNameBox::OnCreate( void )
{

    for( int i=0; i< WINDOW_NUM; ++i)
    {
        m_strNickName[i].erase();
        m_pkBtnOk[i]->InitState( true, true, this );
        m_pkEditNick[i]->InitState( true, true,  this );
        m_pkCheckBox[i]->InitState( true, true, this );
        m_pkCheckBox[i]->SetCheck( false );
    }
	
	m_pkStaticDesc->InitState( false, true, this );
	m_pkStaticDesc->SetLineSpace( 1.2f );

    // 네이버 유저는 바이트 단위로 닉네임 길이를 제한한다.
    //if( 4 == g_kGlobalValue.m_dwChannelType )
    //{
    //    m_pkEditNick[0]->SetLimitTextByByte( GC_NICKNAME_SIZE );
    //}


    if( m_pkScroll )
    {
        m_pkScroll->SetSelfInputCheck(true);
        m_pkScroll->AddActionListener(this);
    }


    SetMsgBox();
}

void KGCNickNameBox::OnCreateComplete( void )
{
	m_pkScroll->SetScrollPageSize( m_pkStaticDesc->GetDisplayLimitLine() );
	m_pkScroll->SetScrollRangeMax( m_pkStaticDesc->GetCount() );
	m_pkScroll->SetScrollPos( m_pkStaticDesc->GetTopIndex() );

    m_strNickName[0].erase();
    m_strNickName[1].erase();
    SpeakToActionListener( KActionEvent( this, NBR_CREATED ) );
}

void KGCNickNameBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, NBR_DESTROYED ) );
}

void KGCNickNameBox::FrameMoveInEnabledState( void )
{
	// 키를 누르는 소리
	if( m_pkEditNick[0]->HasFocus() || m_pkEditNick[1]->HasFocus() )
	{
		for ( int i = 0; i < 256; i++ )
		{
			if ( g_pkInput->IsDown( i ) ) {
				KeydownEditNick();
				g_KDSound.Play( "71" );
			}
		}
	}

	if(m_bUseChangeNickName)
	{
		if ( g_pkInput->IsDown( DIK_ESCAPE ) )
		{
			Destroy();
            g_pkInput->IgnoreDown(DIK_ESCAPE,30);
		}
	}
}

void KGCNickNameBox::KeydownEditNick()
{
#if defined(NATION_CHINA)

	for ( int k = 0 ; k < 2 ; ++k ) {
		std::wstring strCheckNewNickname = m_pkEditNick[k]->GetText();  //글자를 얻는다.  

		if( strCheckNewNickname.empty() )
		{
			continue;
		}

		if(!SiKSlangFilter()->ValidCheckNickName( strCheckNewNickname)) 
		{
			static WCHAR strProhibition[] ={ L' ', 9, L'\'', L'\"', L',', L'　', 10,13  };
			std::wstring strTemp = L"";
			for ( int j = 0 ; j < (int)strCheckNewNickname.size() ; ++j) {
				bool bValid = true;

				for ( int i = 0 ; i < (sizeof(strProhibition) / sizeof(wchar_t)) ; ++i ){
					if ( strCheckNewNickname[j] == strProhibition[i] ) {
						bValid = false;
					}
				}
				if ( bValid )
					strTemp += strCheckNewNickname[j];
			}


			m_pkEditNick[k]->SetText(strTemp.c_str(),false);
			//m_pkEditNickname->GetCurCursorIndex()
			//strCheckNewNickname->
		}

	}


#endif


}

void KGCNickNameBox::ActionPerformed( const KActionEvent& event )
{
	//이벤트를 여러번 먹는것을 막기위한 코드임
	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
		return;

    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            break;
        }
        case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            break;
        }
    }

	if( event.m_pWnd == m_pkScroll )
	{
		if( m_pkStaticDesc->GetTopIndex() != m_pkScroll->GetScrollPos() )
		{
			m_pkStaticDesc->SetTopIndex( m_pkScroll->GetScrollPos() );
		}
	}

	GCWND_MSG_MAP( m_pkEditNick[0],            KD3DWnd::D3DWE_EDIT_RETURN,      OnConfirmNickName );
    GCWND_MSG_MAP( m_pkEditNick[0],            KD3DWnd::D3DWE_BUTTON_CLICK,     OnClickEditNick );
	GCWND_MSG_MAP( m_pkEditNick[1],       KD3DWnd::D3DWE_EDIT_RETURN,           OnConfirmRecommend );
    GCWND_MSG_MAP( m_pkEditNick[1],       KD3DWnd::D3DWE_BUTTON_CLICK,          OnClickEditRecommend );
    GCWND_MSG_MAP( m_pkBtnOk[0],               KD3DWnd::D3DWE_BUTTON_CLICK,     OnConfirmNickName );
    GCWND_MSG_MAP( m_pkBtnOk[1],           KD3DWnd::D3DWE_BUTTON_CLICK,         OnConfirmRecommend );
    GCWND_MSG_MAP( m_pkCheckBox[0], KD3DWnd::D3DWE_BUTTON_CLICK,                OnCheckNot );
    GCWND_MSG_MAP( m_pkCheckBox[1], KD3DWnd::D3DWE_BUTTON_CLICK,                OnCheckLater );
    GCWND_MSG_MAP( this, KD3DWnd::D3DWE_EDIT_ESCAPE,                            OnCancel);
}

void KGCNickNameBox::OnClickEditNick()
{
    if ( m_bNickRegComplete == false )
    {
        m_pkEditNick[0]->SetText(L"");
        m_pkEditNick[0]->SetFocus();
    }
}

void KGCNickNameBox::OnClickEditRecommend()
{
    m_pkEditNick[1]->SetText(L"");
	m_pkEditNick[1]->SetFocus();
    m_pkCheckBox[0]->SetCheck( false );
    m_pkCheckBox[1]->SetCheck( false );
}

void KGCNickNameBox::OnCheckNot()
{
    OnCheck(false);
}

void KGCNickNameBox::OnCheckLater()
{
    OnCheck(true);
}

void KGCNickNameBox::OnCheck( bool bLater)
{
    int i = ( bLater == true ) ? 1: 0;
    int noti = ( i == 0 )? 1: 0;

    // 체크 하려는데 저쪽것도 체크되있으면 혹시나.. 
    if( m_pkCheckBox[i]->GetCheck() == false && m_pkCheckBox[noti]->GetCheck() == true )
        m_pkCheckBox[noti]->SetCheck( false );

    m_pkCheckBox[i]->SetCheck( !m_pkCheckBox[i]->GetCheck() );

	//Destroy();
    if( bLater ){
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,g_pkStrLoader->GetString( STR_ID_CAN_RECOMMEND_IN_SQUARE2 ), L"", KGCUIScene::GC_MBOX_USE_RECOMMEND_NOT,0, 0, false, true);
    }
    else{
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,g_pkStrLoader->GetString( STR_ID_RECOMMEND_NOT ), L"", KGCUIScene::GC_MBOX_USE_RECOMMEND_NOT,1, 0, false, true);
    }
}

void KGCNickNameBox::OnCancel()
{
    switch( GetUse() ) 
    {
        case KGCUIScene::GC_MBOX_USE_NORMAL:
        {
#ifdef __PATH__
            ::PostMessage( g_hUIMainWnd, WM_CLOSE, 0, 0 );
#else
			Destroy();
#endif
            break;
        }
        case KGCUIScene::GC_MBOX_USE_CHANGE_NICK_NAME:
        {
            Destroy();
            break;
        }
        case KGCUIScene::GC_MBOX_USE_CHANGE_PET_NICK_NAME:
        {
            Destroy();
            break;
        }
    }
}

void KGCNickNameBox::OnConfirmNickName()
{
	::SetFocus(g_hUIMainWnd);
    m_strNickName[0] = m_pkEditNick[0]->GetText();

    //====================================================================================
    // 2008.02.16 Jaeho
    // 닉네임이 이미 등록되어 있는 상태와 추천인, 닉네임카드 두군대서 사용하게 고려하여 변경.
    if ( m_bNickRegComplete )
    {
        // 닉네임이 이미 등록된 상태에서는 STR_ID_NICK_NAME_REG_COMPLETE(닉네임 등록 완료) 스트링이 적용되어 있을 수 있기 때문.
        if( m_strNickName[0] != g_pkStrLoader->GetString( STR_ID_NICK_NAME_REG_COMPLETE ) )
        {
            if( false == CheckNickNameWidthMsgBox() )
                return;
        }
    }
    else
    {
        if( false == CheckNickNameWidthMsgBox() )
            return;
    }

    g_KDSound.Play( "31" );

    DWORD dwUse;
    switch( m_dwUse ) 
    {
    case KGCUIScene::GC_MBOX_USE_NORMAL:
        {
            dwUse = KGCUIScene::GC_MBOX_USE_NICK_NAME_AGREE;
            break;
        }
    case KGCUIScene::GC_MBOX_USE_CHANGE_NICK_NAME:
        {
            dwUse = KGCUIScene::GC_MBOX_USE_CHANGE_NICK_NAME;
            break;
        }
    case KGCUIScene::GC_MBOX_USE_CHANGE_PET_NICK_NAME:
        {
            dwUse = KGCUIScene::GC_MBOX_USE_CHANGE_PET_NICK_NAME;
            break;
        }
    default :
        dwUse = KGCUIScene::GC_MBOX_USE_NICK_NAME_AGREE;
        break;
    }

    // 등록 확인창 띄움.
    std::wostringstream strmText;

    if ( m_bUseChangeNickName == true )
    {
        if( dwUse == KGCUIScene::GC_MBOX_USE_CHANGE_NICK_NAME )
        {
            strmText << g_pkStrLoader->GetString( STR_ID_CHANGE_NICK_NAME_AGREE1 ) << L"_";
            strmText << g_pkStrLoader->GetReplacedString( STR_ID_CHANGE_NICK_NAME_AGREE2 , "L" , m_strNickName[0] );
        }
        else if( dwUse == KGCUIScene::GC_MBOX_USE_CHANGE_PET_NICK_NAME )
        {
            strmText << g_pkStrLoader->GetString( STR_ID_CHANGE_PET_NICK_NAME_AGREE1 ) << L"_";
            strmText << g_pkStrLoader->GetReplacedString( STR_ID_CHANGE_PET_NICK_NAME_AGREE2 , "L" , m_strNickName[0] );
        }

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,strmText.str(), L"",(KGCUIScene::EGCMsgBoxUse)dwUse,  
            GetlParam1() , GetlParam2(), false, true );
        //Destroy();
    }
    else
    {
        strmText << g_pkStrLoader->GetString( STR_ID_NICK_NAME_REG_AGREE1 ) << L"_";

#if defined( NATION_KOREA )
        strmText << g_pkStrLoader->GetReplacedString( STR_ID_NICK_NAME_REG_AGREE2 , "LL" , m_strNickName[0], std::wstring(L"") );
#endif       
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,strmText.str(), L"",(KGCUIScene::EGCMsgBoxUse)dwUse, GetlParam1() , GetlParam2(), true );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,strmText.str(), L"",(KGCUIScene::EGCMsgBoxUse)dwUse,  
            GetlParam1() , GetlParam2(), false, true );
    }


}

void KGCNickNameBox::OnConfirmRecommend()
{
	::SetFocus(g_hUIMainWnd);
    if( m_pkCheckBox[0]->GetCheck() == true )
    {
        KP2P::GetInstance()->Send_RecommendUserReq( L"", 1 ); //  bState 추천 할지 : 0, 추천 않함 : 1, 나중에 추천 할지 : 2.
        Destroy();
    }
    else if( m_pkCheckBox[1]->GetCheck() == true )
    {
        KP2P::GetInstance()->Send_RecommendUserReq( L"", 2 );
		g_pkUIScene->m_pkNickNameBox->SetRecommendRegisterComplete( false );
        Destroy();
    }
    else
    {
        m_strNickName[1] = m_pkEditNick[1]->GetText();
        if ( IsValidRecommendNickName() == false )
        {
            if( m_strNickName[1].empty() )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_EMPTY ), g_pkStrLoader->GetString( STR_ID_BLANK ), 
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            else
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_RECOMMEND_NICK_INVALID_LENGTH ), L"",
                KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            return;
        }
    
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString(STR_ID_WILL_YOU_RECOMMEND, "l", m_strNickName[1]), L"", KGCUIScene::GC_MBOX_USE_RECOMMEND_NICKNAME, 0, 0, false, true );

    }

    

}

bool KGCNickNameBox::CheckNickNameWidthMsgBox()
{
    if ( IsValidNickName() == false )
    {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME, L"", L"", (KGCUIScene::EGCMsgBoxUse)GetUse(), GetlParam1(), GetlParam2(), true );

        if( m_strNickName[0].empty() )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_EMPTY ), g_pkStrLoader->GetString( STR_ID_BLANK ), 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        else
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH ), g_pkStrLoader->GetString( STR_ID_BLANK ), 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        //Destroy();
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    // 2008.02.16 Jaeho
    // 닉네임에 욕설이 포함되어 있는지 체크.
    if( !SiKSlangFilter()->FilterCheck( m_strNickName[0], KSlangFilter::SLANG_FILTER_NICKNAME ) )
    {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NICK_NAME, L"", L"", (KGCUIScene::EGCMsgBoxUse)GetUse(), GetlParam1(), GetlParam2(), true );

        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_CHAR ),
            g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false , true );
        //Destroy();
        return false;
    }



    //////////////////////////////////////////////////////////////////////////
    // 2011.02.15 chanseon
    // 닉네임에 특수문자가 포함되어 있는지 체크.
    // 2012-08-20 닉네임 검사하는데 특수문자만 검사하는게 이상함.... 서버와 같이 조건 검사 하도록 수정 by Bishop
    if( !SiKSlangFilter()->ValidCheckNickName( m_strNickName[0] ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
            g_pkStrLoader->GetString( STR_ID_INVALID_NICKNAME ),
            g_pkStrLoader->GetString( STR_ID_NICK_NAME_ANOTHER_USE ), KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false , true );
        return false;
    }


    return true;
}

const std::wstring& KGCNickNameBox::GetNickName( void ) const
{
    return m_strNickName[0];
}

const std::wstring& KGCNickNameBox::GetRecommendNickName( void ) const
{
    return m_strNickName[1];
}

bool KGCNickNameBox::IsValidNickName()
{    
	if ( m_strNickName[0].find( L"#c" ) != std::string::npos || m_strNickName[0].find( L"#C" ) != std::string::npos )
		return false;

#if defined (NICKNAME_CHECK_WITH_WSTRING_REAL_LENGTH)
    if ( m_strNickName[0].empty() || g_pkFontMgr->wStringRealLength( m_strNickName[0].c_str() ) > GC_NICKNAME_SIZE )
        return false;
#else
    if ( m_strNickName[0].empty() || g_pkFontMgr->GetWidth( m_strNickName[0].c_str() ) > ( (12 / 2)*GC_NICKNAME_SIZE*m_fWindowScaleX ))
        return false;
#endif

    return true;
}

bool KGCNickNameBox::IsValidRecommendNickName()
{
#if defined (NICKNAME_CHECK_WITH_WSTRING_REAL_LENGTH)
    if ( m_strNickName[1].empty() || g_pkFontMgr->wStringRealLength( m_strNickName[1].c_str() ) > GC_NICKNAME_SIZE )
        return false;
#else
    if ( m_strNickName[1].empty() || g_pkFontMgr->GetWidth( m_strNickName[1].c_str() ) > ( (12 / 2)*GC_NICKNAME_SIZE*m_fWindowScaleX ))
        return false;
#endif

#if defined(NATION_KOREA)
    for( std::wstring::size_type i = 0 ; i < m_strNickName.size() ; ++i ) {
        if( iswspace( *m_strNickName[i].c_str() ) )
            return false;
    }
#endif 

    return true;
}

void KGCNickNameBox::SetNickBox(bool bTrue)
{
    m_pkWndWelcome[0]->ToggleRender( bTrue );
    m_pkEditNick[0]->ToggleRender( bTrue );
    m_pkBackGround[0]->ToggleRender( bTrue );
    m_pkBackFront[0]->ToggleRender( bTrue );
    m_pkBackNickname[0]->ToggleRender( bTrue );
    m_pkWndNickName[0]->ToggleRender( bTrue );
    m_pkBtnOk[0]->ToggleRender( bTrue );

    if( bTrue)
    {
        m_pkEditNick[0]->EnableSelfFocusCheck( bTrue );
        m_pkEditNick[0]->SetText( g_pkStrLoader->GetString( STR_ID_ENTER_YOUR_NICKNAME ).c_str() );
        m_pkEditNick[0]->SetFocus();

#ifdef NATION_KOREA
        m_pkEditNick[0]->SetLimitText( GC_NICKNAME_SIZE/2 );
#else
        m_pkEditNick[0]->SetLimitText( GC_NICKNAME_SIZE );
#endif 
    }
}

void KGCNickNameBox::SetRecommendBox( bool bTrue)
{
    m_pkWndWelcome[1]->ToggleRender( bTrue );
    m_pkEditNick[1]->ToggleRender( bTrue );
    m_pkBackGround[1]->ToggleRender( bTrue );
    m_pkBackFront[1]->ToggleRender( bTrue );
    m_pkBackNickname[1]->ToggleRender( bTrue );
    m_pkWndNickName[1]->ToggleRender( bTrue );
    m_pkBtnOk[1]->ToggleRender( bTrue );

    m_pkBackDesc->ToggleRender( bTrue );
    m_pkBackBottom->ToggleRender( bTrue );
    m_pkStaticDesc->ToggleRender( bTrue );   
    m_pkStaticBottom->ToggleRender( bTrue ); 
    m_pkStaticCheckbox[0]->ToggleRender( bTrue );
    m_pkStaticCheckbox[1]->ToggleRender( bTrue );
    m_pkCheckBox[0]->ToggleRender( bTrue );
    m_pkCheckBox[1]->ToggleRender( bTrue );
    m_pkScroll->ToggleRender( bTrue );

    if( bTrue)
    {
        m_pkEditNick[1]->EnableSelfFocusCheck( bTrue );
        m_pkEditNick[1]->SetText( g_pkStrLoader->GetString( STR_ID_ENTER_FRIENDS_NICKNAME ).c_str() );
        m_pkEditNick[1]->SetFocus();
        m_pkCheckBox[0]->SetCheck( false );
        m_pkCheckBox[1]->SetCheck( false );
    
		m_pkStaticDesc->Clear();
        m_pkStaticDesc->AddString( g_pkStrLoader->GetString( STR_ID_DESC_EVENT_FRIEND ));
        m_pkStaticDesc->SetTopIndex( 0 );

        m_pkStaticBottom->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_CAN_RECOMMEND_IN_SQUARE ));
        m_pkStaticCheckbox[0]->SetFontColor( 0xFF000000 );
        m_pkStaticCheckbox[1]->SetFontColor( 0xFF000000 );
        m_pkStaticCheckbox[0]->SetText( g_pkStrLoader->GetString( STR_ID_DO_NOT_RECOMMEND ));
        m_pkStaticCheckbox[1]->SetText( g_pkStrLoader->GetString( STR_ID_RECOMMEND_FRIEND_LATER ));

        //m_pkEditNick[1]->SetLimitText( GC_NICKNAME_SIZE, m_pkEditNick[1]->GetWidth(), 12 );


#ifdef NATION_KOREA
        m_pkEditNick[1]->SetLimitText( GC_NICKNAME_SIZE/2 );
#else
        if (m_bIsNickName)
        {
            m_pkEditNick[1]->SetLimitText( GC_NICKNAME_SIZE );
        }
        else
        {
            m_pkEditNick[1]->SetLimitText( PET_NICKNAME_SIZE );
        }
#endif 

    }
    else 
    {
        if ( m_bIsNickName )
        {
#ifdef NATION_KOREA
            m_pkEditNick[0]->SetLimitText( GC_NICKNAME_SIZE/2 );
#else
            m_pkEditNick[0]->SetLimitText( GC_NICKNAME_SIZE );
#endif 
        }
        else
        {
           m_pkEditNick[0]->SetLimitText( GC_NICKNAME_SIZE );
        }
    }
}

bool KGCNickNameBox::SetMsgBox()
{
    
    if ( m_bUseChangeNickName )  // 닉넴 체인지면 
    {
        SetNickBox( true );
        SetRecommendBox( false);
        m_pkWndWelcome[0]->ToggleRender(false);
        return true;
    }


    if( m_bNickRegComplete == true )    // 닉넴이 등록되있고
    {
        if( m_bRecommendRegComplete == false )  // 추천할 수 있으면
        {
            SetNickBox( false );
            SetRecommendBox( true );
            return true;
        }
        else
        {
            SetNickBox( false );
            SetRecommendBox( false);
            return false;
        }
    }
    else    // 닉넴이 등록 안되있으면 
    {
        SetNickBox( true );
        SetRecommendBox( false);
        return true;
    }
    
}
// 아 몰라..
void KGCNickNameBox::SetLock( int i )
{
    //m_pkBtnOk[0]->Lock(true);
    //m_pkEditNick[0]->EnableSelfFocusCheck(false);
    m_pkBtnOk[i]->Lock(true);
}

bool KGCNickNameBox::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    this->SetlParam1( static_cast<GCITEMID>( kInfo_.m_lParam ));
    this->SetlParam2( kInfo_.m_lParam2 );
    this->SetUse( (DWORD)kInfo_.m_nMsgBoxUse );
    bool bTrue = false;
    if ( kInfo_.m_nMsgBoxUse == KGCUIScene::GC_MBOX_USE_CHANGE_NICK_NAME )
    {
        m_bIsNickName = true;
        this->SetChangeNickName(true);
    }
    else if ( kInfo_.m_nMsgBoxUse == KGCUIScene::GC_MBOX_USE_CHANGE_PET_NICK_NAME )
    {
        m_bIsNickName = false;
        this->SetChangeNickName(true);
    }

    bTrue = this->SetMsgBox();

    return bTrue;
}