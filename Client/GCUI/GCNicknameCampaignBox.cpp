#include "stdafx.h"
#include "GCNicknameCampaignBox.h"

IMPLEMENT_CLASSNAME( KGCNicknameCampaignBox );
IMPLEMENT_WND_FACTORY( KGCNicknameCampaignBox );
IMPLEMENT_WND_FACTORY_NAME( KGCNicknameCampaignBox, "gc_nickname_campaign_box" );

//닉네임 변경 캠페인
KGCNicknameCampaignBox::KGCNicknameCampaignBox( void )
: m_bValidNickname(false)
, m_bValidNicknameCheckComplete(false)
, m_bFirst(true)
, m_pkBtnClose(NULL)
, m_pkBtnConfirm(NULL)
, m_pkEditNick(NULL)
, m_pkBackground(NULL)
{
    LINK_CONTROL( "background",             m_pkBackground );
    LINK_CONTROL( "btnConfirm",             m_pkBtnConfirm );
    LINK_CONTROL( "btnClose",               m_pkBtnClose );
    LINK_CONTROL( "checkboxNoRecommend",    m_pkCheckNoRecommend );
    LINK_CONTROL( "staticNoRecommend",      m_pkStaticNoRecommend );    
    LINK_CONTROL( "staticNotice",           m_pkStaticNotice );
    LINK_CONTROL( "editNickname",           m_pkEditNick );
}

KGCNicknameCampaignBox::~KGCNicknameCampaignBox( void )
{

}

void KGCNicknameCampaignBox::OnCreate( void )
{
    m_pkEditNick->SetFocus();

    m_pkBtnClose->InitState( true, true );
    m_pkBtnConfirm->InitState( true, true );
    m_pkEditNick->InitState( true, true,  this );
    m_pkCheckNoRecommend->InitState( true, true, this );

    m_pkCheckNoRecommend->SetCheck( false );


    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNicknameCampaignBox::OnClose );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnConfirm->DisconnectAllEventProcedure();
    m_pkBtnConfirm->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCNicknameCampaignBox::OnConfirmNickName );
    m_pkBtnConfirm->SetHotKey( DIK_RETURN );

    static bool bInit = false;
    if ( !bInit ) {
        bInit = true;

        m_pkStaticNoRecommend->SetText( g_pkStrLoader->GetString(STR_ID_TO_DAY_NOT_SEE) );
        m_pkStaticNotice->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_CHATTING_WARRING_MSG) );
        m_pkStaticNotice->SetFontColor(D3DCOLOR_ARGB( 255, 111, 53, 15 ));
        m_pkStaticNotice->SetFontSize(SiKGCMultipleLanguages()->GetMiddleFontSize());
        //m_pkStaticNotice->SetFontSize(SiKGCMultipleLanguages()->GetLargeFontSize());

        m_pkStaticNotice->SetLineSpace( 1.0f );   //1.2
        m_pkStaticNotice->SetFontSpace( 1 );    //1
    }

    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CREATED) );

}


void KGCNicknameCampaignBox::ActionPerformed( const KActionEvent& event )
{
    //이벤트를 여러번 먹는것을 막기위한 코드임// <- 구실을 하는지 테스트 안되었음
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
    // 

    GCWND_MSG_MAP( m_pkBtnConfirm,       KD3DWnd::D3DWE_BUTTON_CLICK,   OnConfirmNickName   );
    GCWND_MSG_MAP( m_pkEditNick,         KD3DWnd::D3DWE_BUTTON_CLICK,   OnClickEditNick     );
    GCWND_MSG_MAP( m_pkCheckNoRecommend, KD3DWnd::D3DWE_BUTTON_CLICK,   OnCheckNoRecommend  );
    GCWND_MSG_MAP( m_pkBtnClose,         KD3DWnd::D3DWE_BUTTON_CLICK,   OnClose  );
}

void KGCNicknameCampaignBox::OnClickEditNick()
{
    m_pkEditNick->SetText(L"");
    m_pkEditNick->SetFocus();
}

void KGCNicknameCampaignBox::OnClose()
{
    m_bFirst = false;
    SetNicknameBox( false );

    if ( m_pkCheckNoRecommend->GetCheck() == true ) {
        KP2P::GetInstance()->Send_DisableTodaysPopupNot();
    }
    
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );   //이게 없으면 창 닫고난뒤에 밝아지지않으면서 진행이 안되었다.
}




bool KGCNicknameCampaignBox::CheckCondition( IN const KDialogInfo& kInfo_ )
{
    if( m_bFirst == false ) {
        return false;
    }

    if ( !IsValidNickname() ) {     //지금쓰고 있는 이름이 유효한지 검사한다.
        SetNicknameBox( true );
        return true;
    }
    return false;
}



void KGCNicknameCampaignBox::SetNicknameBox(bool bTrue)
{
    m_pkBackground->ToggleRender( bTrue );
    m_pkBtnConfirm->ToggleRender( bTrue );
    m_pkBtnClose->ToggleRender( bTrue );
    m_pkCheckNoRecommend->ToggleRender( bTrue );
    m_pkStaticNoRecommend->ToggleRender( bTrue );
    m_pkStaticNotice->ToggleRender( bTrue );
    m_pkEditNick->ToggleRender( bTrue );

    m_pkEditNick->EnableSelfFocusCheck( bTrue );
    m_pkEditNick->SetText( g_pkStrLoader->GetString( STR_ID_ENTER_YOUR_NICKNAME ).c_str() );
    if( bTrue ) {
        m_pkEditNick->SetFocus();    
    }

#if defined ( NATION_KOREA )
    m_pkEditNick->SetLimitText(GC_NICKNAME_SIZE/2 );
#else
    m_pkEditNick->SetLimitText(GC_NICKNAME_SIZE );
#endif 
}

void KGCNicknameCampaignBox::FrameMoveInEnabledState( void )
{
    // 키를 누르는 소리
    if( m_pkEditNick->HasFocus() )
    {
        for ( int i = 0; i < 256; i++ )
        {
            if ( g_pkInput->IsDown( i ) ) g_KDSound.Play( "71" );
        }
    }

}

void KGCNicknameCampaignBox::OnConfirmNickName()
{
    if( m_pkEditNick->HasFocus() == false )
        return;

    ::SetFocus(g_hUIMainWnd);

    g_KDSound.Play( "31" );

    m_strNickName = m_pkEditNick->GetText();

    if( false == CheckNickNameWidthMsgBox() ) {
        return;
    }

    //유효한 닉네임이라면 닉네임적용 메세지를 보내라. 적용시켜라.
    if ( IsValidNewNickname() ) { 

        std::wostringstream strmText;
        strmText << g_pkStrLoader->GetString( STR_ID_CHANGE_NICK_NAME_AGREE1 ) << L"_";
        strmText << g_pkStrLoader->GetReplacedString( STR_ID_CHANGE_NICK_NAME_AGREE2 , "L" , m_strNickName );        
        
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,strmText.str(), L"",(KGCUIScene::EGCMsgBoxUse)KGCUIScene::GC_MBOX_USE_CHANGE_NICK_NAME_FROM_CAMPAIGN,  
            0, 0, false, true );

    } 
    else {
        std::wostringstream strmText;
        strmText << g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_CHAR ) << L"_";
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK ,strmText.str(), L"",(KGCUIScene::EGCMsgBoxUse)KGCUIScene::GC_MBOX_NICKNAME_CAMPAIGN_BOX, 
            0 , 0, false, true ); //이메세지는 중복되어서 쌓이면 안된다.
    }
}



bool KGCNicknameCampaignBox::CheckNickNameWidthMsgBox()
{
    if ( !IsValidNicknameCondition() )
    {
        if( m_strNickName.empty() )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_EMPTY ), g_pkStrLoader->GetString( STR_ID_BLANK ), 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        else
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NICK_NAME_INVALID_LENGTH ), g_pkStrLoader->GetString( STR_ID_BLANK ), 
            KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return false;
    }
    return true;    
}


//Edit에 있는 값을 받아와서 비교한다. 
bool KGCNicknameCampaignBox::IsValidNewNickname()
{
    //입력가능한 문자인지 체크
    if ( !SiKSlangFilter()->ValidCheckNickName( m_strNickName ) )
        return false;

    //불량어를 쓰고 있는지 체크
    if ( !SiKSlangFilter()->FilterCheck( m_strNickName ) )
        return false;
    
    return true;    
}

//지금 쓰고 있는 닉네임이 유효한 닉네임인지 검사한다.
bool KGCNicknameCampaignBox::IsValidNickname()
{
    //입력가능한 문자인지 체크
    if ( !SiKSlangFilter()->ValidCheckNickName( g_kGlobalValue.m_kUserInfo.strNickName ) )
        return false;

    //불량어를 쓰고 있는지 체크
    if ( !SiKSlangFilter()->FilterCheck( g_kGlobalValue.m_kUserInfo.strNickName ) )
        return false;

    SetValidNickname(true);
    return true;    
}

bool KGCNicknameCampaignBox::IsValidNicknameCondition()
{    
    if ( m_strNickName.find( L"#c" ) != std::string::npos || m_strNickName.find( L"#C" ) != std::string::npos )
        return false;

#if defined (NICKNAME_CHECK_WITH_WSTRING_REAL_LENGTH)
    if ( m_strNickName.empty() || g_pkFontMgr->wStringRealLength( m_strNickName.c_str() ) > GC_NICKNAME_SIZE )
        return false;
#else
    if ( m_strNickName.empty() || g_pkFontMgr->GetWidth( m_strNickName.c_str() ) > ( (12 / 2)*GC_NICKNAME_SIZE*m_fWindowScaleX ))
        return false;
#endif
    return true;
}


void KGCNicknameCampaignBox::OnCheckNoRecommend()
{
    bool bCheckedNoRecommend = m_pkCheckNoRecommend->GetCheck();
    m_pkCheckNoRecommend->SetCheck( !bCheckedNoRecommend );
}


const std::wstring& KGCNicknameCampaignBox::GetNickName( void ) const
{
    return m_strNickName;
}
