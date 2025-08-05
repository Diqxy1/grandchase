#include "stdafx.h"
#include "GCAgitOption.h"

IMPLEMENT_CLASSNAME( KGCAgitOption );
IMPLEMENT_WND_FACTORY( KGCAgitOption );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitOption, "gc_agit_option" );

KGCAgitOption::KGCAgitOption( void )
{
    m_pkStaticAgitName = NULL;
    m_pkStaticAgitOpenType = NULL;
    m_pkStaticAgitPassword = NULL;
    m_pkStaticAgitNumPeople = NULL;
    m_pkStaticAgitMent = NULL;
    m_pkStaticLength = NULL;
    LINK_CONTROL( "staicAgitName", m_pkStaticAgitName );
    LINK_CONTROL( "staticOpenType", m_pkStaticAgitOpenType );
    LINK_CONTROL( "staticPassword", m_pkStaticAgitPassword );
    LINK_CONTROL( "staticNumPeople", m_pkStaticAgitNumPeople );
    LINK_CONTROL( "staticMent", m_pkStaticAgitMent );
    LINK_CONTROL( "staticLength", m_pkStaticLength );

    m_pkEditAgitName = NULL;
    m_pkEditAgitPassword = NULL;
    m_pkEditAgitMent = NULL;
    LINK_CONTROL( "editAgitName", m_pkEditAgitName );
    LINK_CONTROL( "editPassword", m_pkEditAgitPassword );
    LINK_CONTROL( "editMent", m_pkEditAgitMent );

    m_pkComboAgitOpenType = NULL;
    m_pkComboAgitNumPeople = NULL;
    m_pkScrollAgitMent = NULL;
    LINK_CONTROL( "openCombobox", m_pkComboAgitOpenType );
    LINK_CONTROL( "numCombobox", m_pkComboAgitNumPeople );
    LINK_CONTROL( "scroll", m_pkScrollAgitMent );

    m_pkBtnClose = NULL;
    m_pkBtnOK = NULL;
    m_pkBtnCancle = NULL;
    LINK_CONTROL( "btnClose", m_pkBtnClose );
    LINK_CONTROL( "btnOK", m_pkBtnOK );
    LINK_CONTROL( "btnCancle", m_pkBtnCancle );

    m_pkWndOptionTitle = NULL;
    m_pkWndRegisterTitle = NULL;
    LINK_CONTROL( "wndOptionTitle", m_pkWndOptionTitle );
    LINK_CONTROL( "wndRegisterTitle", m_pkWndRegisterTitle );

    m_iOptionType = TYPE_OPTION;
}

KGCAgitOption::~KGCAgitOption( void )
{
}

void KGCAgitOption::OnCreate( void )
{
    m_pkStaticAgitName->InitState( true, false, NULL );
    m_pkStaticAgitOpenType->InitState( true, false, NULL );
    m_pkStaticAgitPassword->InitState( true, false, NULL );
    m_pkStaticAgitNumPeople->InitState( true, false, NULL );
    m_pkStaticAgitMent->InitState( true, false, NULL );

    
    m_pkStaticAgitName->SetAlign( DT_LEFT );
    m_pkStaticAgitOpenType->SetAlign( DT_LEFT );
    m_pkStaticAgitPassword->SetAlign( DT_LEFT );
    m_pkStaticAgitNumPeople->SetAlign( DT_LEFT );
    m_pkStaticAgitMent->SetAlign( DT_LEFT );


    m_pkStaticAgitName->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticAgitOpenType->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticAgitPassword->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticAgitNumPeople->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticAgitMent->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
 

    m_pkEditAgitName->InitState( true, false, this );
    m_pkEditAgitPassword->InitState( true, false, this );
    m_pkEditAgitMent->InitState( true, false, this );
    
    m_pkEditAgitName->SetLimitText( MAX_AGIT_NAME );
    m_pkEditAgitName->SetLimitLine( 1 );
    m_pkEditAgitPassword->SetLimitText( MAX_AGIT_PASSWORD );
    m_pkEditAgitMent->SetLimitText( MAX_AGIT_MENT );
    
    m_pkEditAgitName->EnableSelfFocusCheck( true );
    m_pkEditAgitPassword->EnableSelfFocusCheck( true );
    m_pkEditAgitMent->EnableSelfFocusCheck( true );

    m_pkEditAgitPassword->SetWndMode( D3DWM_LOCK );
    m_pkEditAgitMent->SetMultiLine( true );
    m_pkEditAgitMent->SetNewLine( true );

    m_pkComboAgitOpenType->InitState( true, true, this );
    m_pkComboAgitNumPeople->InitState( true, true, this );

#if defined ( NATION_USA )
    m_pkScrollAgitMent->InitState( false, false, this );
#else
    m_pkScrollAgitMent->InitState( true, true, this );
#endif

    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );

    m_pkStaticAgitName->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_OPTION_NAME) );
    m_pkStaticAgitOpenType->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_OPTION_OPEN ) );
    m_pkStaticAgitPassword->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_OPTION_PASSWORD ) );
    m_pkStaticAgitNumPeople->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_OPTION_NUM_PEOPLE ) );
    m_pkStaticAgitMent->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_OPTION_MENT ) );
        
    if( m_pkStaticLength ) { 
        m_pkStaticLength->InitState( true, false, NULL );
        m_pkStaticLength->SetAlign( DT_LEFT );
        m_pkStaticLength->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
        WCHAR strTemp[200];
        std::wstring strCurNum = m_pkEditAgitMent->GetText();		
        swprintf( strTemp, 200, L"( %d / %d )", strCurNum.length(), 100 );		
        m_pkStaticLength->SetText(strTemp);
    }
    
    m_pkWndOptionTitle->InitState( true, false, NULL );
    m_pkWndRegisterTitle->InitState( true, false, NULL );
    m_iOptionType = TYPE_OPTION;

    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
}

void KGCAgitOption::ActionPerformed( const KActionEvent& event )
{
    if( ( event.m_pWnd == m_pkComboAgitOpenType || event.m_pWnd == m_pkComboAgitNumPeople ) && event.m_dwCode == KD3DWnd::D3DWE_COMBO_SELCHANGE )
        OnComboSelChange( event.m_pWnd );
    
    GCWND_MSG_MAP( m_pkBtnOK,           KD3DWnd::D3DWE_BUTTON_CLICK,    OnOK    );
    GCWND_MSG_MAP( m_pkBtnClose,        KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
    GCWND_MSG_MAP( m_pkBtnCancle,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );

    GCWND_MSG_MAP( m_pkEditAgitName,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditAgitName );
    GCWND_MSG_MAP( m_pkEditAgitPassword,KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditAgitPassword );
    GCWND_MSG_MAP( m_pkEditAgitMent,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditAgitMent );
}

void KGCAgitOption::FrameMoveInEnabledState( void )
{
    std::wstring strName = m_pkEditAgitName->GetText();
    std::wstring strMent = m_pkEditAgitMent->GetText();

    m_pkStaticAgitName->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_OPTION_NAME) );
    

    if( m_pkStaticLength ) { 
        WCHAR strTemp[200];
        std::wstring strCurNum = m_pkEditAgitMent->GetText();		
        swprintf( strTemp, 200, L"( %d / %d )", strCurNum.length(), 100 );		
        m_pkStaticLength->SetText(strTemp);
    }
}

void KGCAgitOption::InitAgitOption( KAgitInfo& kCurAgitInfo_, int iType_ )
{
    SetComboList();
    
    m_pkEditAgitName->SetText( kCurAgitInfo_.m_strName.c_str() );
    m_pkEditAgitPassword->SetText( kCurAgitInfo_.m_strPassword.c_str() );

#if defined( NATION_USA )
    m_pkEditAgitMent->SetText( L"" );
#else
    m_pkEditAgitMent->SetText( kCurAgitInfo_.m_kProfile.m_strProfile.c_str() );
#endif

    switch( kCurAgitInfo_.m_ucOpenType )
    {
    case KAgitInfo::OT_OPEN:
        m_iOpenType = 0;
        m_pkEditAgitPassword->InitState( false, false, NULL );
        break;

    case KAgitInfo::OT_FRIEND:
        m_iOpenType = 1;
        m_pkEditAgitPassword->InitState( false, false, NULL );
        break;

    case KAgitInfo::OT_CLOSE:
        m_iOpenType = 2;
        m_pkEditAgitPassword->InitState( true, true, this );
        break;
    }
    m_pkComboAgitOpenType->SetCurSel( m_iOpenType );

    m_iNumAgitPeople = static_cast<int>( kCurAgitInfo_.m_ucMaxUser );
    m_pkComboAgitNumPeople->SetCurSel( m_iNumAgitPeople - 1 );

    SetOptionboxType( iType_ );
}

void KGCAgitOption::SetComboList( void )
{
    m_pkComboAgitOpenType->DeleteAllItem();
    m_pkComboAgitNumPeople->DeleteAllItem();

    // 공개 설정
    m_pkComboAgitOpenType->SetComboStatic( g_pkStrLoader->GetString( STR_ID_AGIT_OPEN_ALL ) );
    m_pkComboAgitOpenType->AddString( g_pkStrLoader->GetString( STR_ID_AGIT_OPEN_ALL ) );
    m_pkComboAgitOpenType->AddString( g_pkStrLoader->GetString( STR_ID_AGIT_OPEN_FRIEND ) );
    m_pkComboAgitOpenType->AddString( g_pkStrLoader->GetString( STR_ID_AGIT_OPEN_HIDE ) );

    // 인원 설정
    std::wostringstream stm;
    stm << MAX_PLAYER_NUM;
    m_pkComboAgitNumPeople->SetComboStatic( stm.str() );
    for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
    {
        std::wostringstream stm;
        stm << ( i+1 );
        m_pkComboAgitNumPeople->AddString( stm.str() );
    }
}

void KGCAgitOption::OnComboSelChange( KD3DWnd* pCombo )
{
    if( NULL == pCombo )
        return;

    if( pCombo == m_pkComboAgitOpenType )
    {
        int iCurSel = m_pkComboAgitOpenType->GetCurSel();
        switch( iCurSel )
        {
        case 0:
            m_iOpenType = KAgitInfo::OT_OPEN;
            break;

        case 1:
            m_iOpenType = KAgitInfo::OT_FRIEND;
            break;

        case 2:
            m_iOpenType = KAgitInfo::OT_CLOSE;
            break;

        default:
            m_iOpenType = KAgitInfo::OT_OPEN;
            break;
        }

        if( KAgitInfo::OT_CLOSE == m_iOpenType )
            m_pkEditAgitPassword->InitState( true, true, this );
        else
            m_pkEditAgitPassword->InitState( false, false, NULL );
            
    }
    else if( pCombo == m_pkComboAgitNumPeople )
        m_iNumAgitPeople = m_pkComboAgitNumPeople->GetCurSel();
        ++m_iNumAgitPeople;
        if( m_iNumAgitPeople > MAX_PLAYER_NUM )
            m_iNumAgitPeople = MAX_PLAYER_NUM;
}

void KGCAgitOption::OnOK( void )
{
    // 아지트 이름 유효한지 검사 
    if( false == IsValidAgitName() )
        return;

    // 아지트 패스워드 유효한지 검사
    if( false == IsValidAgitPassword() )
        return;

    // 아지트 인사말 유효한지 검사 
    if( false == IsVAlidAgitMent() )
        return;

    // 아지트 정보 세팅 
    KAgitOption option;
    option.m_strName = m_pkEditAgitName->GetText();
    option.m_strPassword = m_pkEditAgitPassword->GetText();
    option.m_strProfile = m_pkEditAgitMent->GetText();
    option.m_ucInviteType = KAgitInfo::IT_ACCEPT;
    option.m_ucMaxUser = static_cast<UCHAR>( m_iNumAgitPeople );
    option.m_ucOpenType = static_cast<UCHAR>( m_iOpenType );
    boost::replace_all( option.m_strProfile, L"\n", L"" );
    SiKGCAgitGameManager()->SetMyAgitOption( option.m_strName, option.m_ucMaxUser, option.m_ucOpenType, option.m_strPassword, option.m_ucInviteType, option.m_strProfile );
    this->ToggleRender( false );

    if( TYPE_REGISTER == m_iOptionType )
    {
        SiKGCAgitGameManager()->RegisterAgitList( option );
    }
    
    SiKGCAgitGameManager()->SetAgitOption( option );
    g_pkAgitOverlayUI->UpdateAgitInfo();
}

void KGCAgitOption::OnClose( void )
{
    this->ToggleRender( false );   

    if( TYPE_REGISTER == m_iOptionType )
        g_pkAgitOverlayUI->ShowAgitInviteVisitUI( true );
}

bool KGCAgitOption::IsValidAgitName( void )
{
    std::wstring strAgitName = m_pkEditAgitName->GetText();
    if( false == IsValidText( strAgitName ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_CHAR_AGIT_NAME ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return false;
    }
    if( false == IsValidLength( strAgitName, MAX_AGIT_NAME ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_LENGTH_AGIT_NAME ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return false;
    }
    return true;
}

bool KGCAgitOption::IsValidAgitPassword( void )
{
    if( m_iOpenType != KAgitInfo::OT_CLOSE ) {
        m_pkEditAgitPassword->SetText( L"" );
        return true;
    }

    std::wstring strAgitPassword = m_pkEditAgitPassword->GetText();
    if( false == IsValidPassword( strAgitPassword ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_CHAR_AGIT_PASSWORD ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return false;
    }
    if( false == IsValidLength( strAgitPassword, MAX_AGIT_PASSWORD ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_LENGTH_AGIT_PASSWORD ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return false;
    }
    return true;
}

bool KGCAgitOption::IsVAlidAgitMent( void )
{
    std::wstring strAgitMent = m_pkEditAgitMent->GetText();
    if( false == IsValidText( strAgitMent ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_CHAR_AGIT_MENT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return false;
    }
    if( false == IsValidLength( strAgitMent, MAX_AGIT_MENT ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_LENGTH_AGIT_MENT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return false;
    }
    return true;
}

bool KGCAgitOption::IsValidText( std::wstring& strText_ )
{
    if( false == SiKSlangFilter()->FilterCheck( strText_ ) )
        return false;

    return true;
}

bool KGCAgitOption::IsValidPassword( std::wstring& strPassword_ )
{
    // 패스워드는 숫자로만 
    // 아스키 숫자 범위 : 0030 ~ 0039
    // 유니코드 숫자 범위 : ff10 ~ ff19
    for( unsigned int i=0 ; i<strPassword_.length() ; ++i )
    {
        if( 0x0030 > strPassword_[i] || strPassword_[i] > 0x0039 )
            return false;
    }

    return true;
}

bool KGCAgitOption::IsValidLength( std::wstring& strText_, unsigned int iMaxLength_ )
{
    if( strText_.length() > iMaxLength_ )
        return false;

    return true;
}

void KGCAgitOption::OnClickEditAgitName( void )
{
    m_pkEditAgitName->SetText( L"" );
    m_pkEditAgitName->SetFocus();
}

void KGCAgitOption::OnClickEditAgitPassword( void )
{
    m_pkEditAgitPassword->SetText( L"" );
    m_pkEditAgitPassword->SetFocus();
}

void KGCAgitOption::OnClickEditAgitMent( void )
{
    if( m_iOptionType == TYPE_REGISTER )
        return;

    m_pkEditAgitMent->SetText( L"" );
    m_pkEditAgitMent->SetFocus();
}

void KGCAgitOption::SetOptionboxType( int iType_ )
{
    if( iType_ < TYPE_OPTION || TYPE_REGISTER < iType_ )
        iType_ = TYPE_OPTION;

    // 옵션 저장으로 쓸때랑 채팅 아지트 등록으로 쓸 때랑 차이 
    // UI 상단 Title Texture / 아지트 인사말 설정 가능 여부 
    // 저장된 아지트 이름이 공백일 경우 기본 스트링 설정함
    std::wstring strAgitName = SiKGCAgitGameManager()->GetAgitName();
    if ( strAgitName.empty() ) {
        std::wstring strName = g_pkStrLoader->GetReplacedString( STR_ID_AGIT_NAME_DEFAULT, "l", SiKGCAgitGameManager()->GetOwnerName() );
        m_pkEditAgitName->SetText( strName.c_str(), false );
    }

    switch( iType_ )
    {
    case TYPE_OPTION:
        m_pkWndOptionTitle->ToggleRender( true );
        m_pkWndRegisterTitle->ToggleRender( false );
        m_pkEditAgitMent->InitState( true, true, this );
        m_pkEditAgitMent->SetMultiLine( true );
        //m_pkEditAgitMent->SetNewLine( true );
        m_pkEditAgitMent->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
        break;

    case TYPE_REGISTER:
        m_pkWndOptionTitle->ToggleRender( false );
        m_pkWndRegisterTitle->ToggleRender( true );
        m_pkEditAgitMent->InitState( true, false, NULL );
        m_pkEditAgitMent->SetMultiLine( true );
        //m_pkEditAgitMent->SetNewLine( true );
        m_pkEditAgitMent->SetWndMode( KD3DWnd::D3DWM_LOCK );
#if defined( NATION_USA )
        m_pkEditAgitMent->SetText( L"" );
#else
        m_pkEditAgitMent->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_OPTION_TYPE_REGISTER ).c_str() );
#endif
        break;
    }

    m_iOptionType = iType_;
}
