#include "StdAfx.h"
#include "GCAgreement.h"
#include "GCUIScene.h"
#include "KGCStringID.h"
#include "KDSound.h"

IMPLEMENT_CLASSNAME( KGCAgreement );
IMPLEMENT_WND_FACTORY( KGCAgreement );
IMPLEMENT_WND_FACTORY_NAME( KGCAgreement, "gc_kgcagreement" );

KGCAgreement::KGCAgreement(void)
: m_fAgreeBtnOriginalPosX(-1.f)
{
	m_pkScroll = NULL;
	m_pkMessage = NULL;
	m_pkDisagree = NULL;
	m_pkAgree = NULL;

#if defined(NATION_TAIWAN)
    LINK_CONTROL( "btn_changebeanfun", m_pkChangeBeanfun );
    LINK_CONTROL( "btn_beanfun_hk", m_pkBeanfun_hk );
    LINK_CONTROL( "btn_changebeanfun_hk", m_pkChangeBeanfun_hk );
#endif
	LINK_CONTROL( "btn_agree", m_pkAgree );
	LINK_CONTROL( "btn_disagree", m_pkDisagree );
	LINK_CONTROL( "static_message", m_pkMessage );
	LINK_CONTROL( "scroll_message", m_pkScroll );

    
}

KGCAgreement::~KGCAgreement(void)
{
}

void KGCAgreement::OnCreate( void )
{
	m_pkScroll->InitState( true, true, this );
	m_pkMessage->InitState( true );
	m_pkDisagree->InitState( true, true, this );
	m_pkAgree->InitState( true, true, this );

#if defined(NATION_TAIWAN)
    m_pkChangeBeanfun->InitState( true, true, this );
    m_pkBeanfun_hk->InitState( true, true, this );
    m_pkChangeBeanfun_hk->InitState( true, true, this );
#endif

	m_pkMessage->Clear();
	//m_pkMessage->AddString(g_pkStrLoader->GetString(STR_ID_USERAGREEMENT));

    if( m_fAgreeBtnOriginalPosX == -1.f )
        m_fAgreeBtnOriginalPosX = m_pkAgree->GetFixedWindowLocalPos().x;

#if !defined( NATION_TAIWAN )
    OnScroll();
#endif
}

void KGCAgreement::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkAgree, KD3DWnd::D3DWE_BUTTON_CLICK, OnAgree );
	GCWND_MSG_MAP( m_pkDisagree, KD3DWnd::D3DWE_BUTTON_CLICK, OnDisAgree );

#if defined(NATION_TAIWAN)
    GCWND_MSG_MAP( m_pkChangeBeanfun, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeBeanfun );
    GCWND_MSG_MAP( m_pkBeanfun_hk, KD3DWnd::D3DWE_BUTTON_CLICK, OnBeanfunHK );
    GCWND_MSG_MAP( m_pkChangeBeanfun_hk, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeBeanfunHK );
#endif

#if !defined( NATION_TAIWAN )
    if( event.m_pWnd == m_pkScroll )
	{
		OnScroll();
	}
#endif

}

void KGCAgreement::OnAgree()
{
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCAgreement::OnDisAgree()
{

#if defined( NATION_TAIWAN )	
    PopUpWebPage( L"http://tw.beanfun.com/game_zone/default.aspx?service_code=300036&service_region=TA" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
//    ::PostMessage( g_MyD3D->m_hWnd, WM_CLOSE, 0, 0 );
#else
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L"",
		KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, true );
	//::PostMessage( g_MyD3D->m_hWnd, WM_CLOSE, 0, 0 );
#endif
}

void KGCAgreement::OnScroll()
{
	int iTop = m_pkScroll->GetScrollPos();
	if ( m_pkMessage->GetTopIndex() != iTop )
	{
		g_KDSound.Play( "73" );
		m_pkMessage->SetTopIndex( iTop );
	}
}

void KGCAgreement::SetStringID( int iStringID )
{
    m_pkMessage->Clear();
	m_pkMessage->SetMaxLine( KD3DListBox::NUM_MAX_LINE_EX );
	if( iStringID == 0 )
	{

		m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT) );
		
#if defined ( NATION_KOREA )
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT2) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT3) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT4) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT5) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT6) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT7) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT8) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT9) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT10) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_11) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_12) );
#elif defined( NATION_PHILIPPINE )
		m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT2) );
		m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT3) );
		m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT4) );
		m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT5) );
		m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_SHOP) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT6) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT7) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT8) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT9) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT10) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_11) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_12) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_13) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_14) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_15) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_16) );
        m_pkMessage->AddString( g_pkStrLoader->GetString(STR_ID_USERAGREEMENT_17) );		
#endif 
				

		m_pkAgree->ToggleRender( true );
        m_pkAgree->SetFixedWindowLocalPos(D3DXVECTOR2(m_fAgreeBtnOriginalPosX, m_pkAgree->GetFixedWindowLocalPos().y));
		m_pkDisagree->ToggleRender( true );
	}
	else
	{
		m_pkMessage->AddString( g_pkStrLoader->GetString(iStringID) );
		m_pkAgree->ToggleRender( true );
        m_pkAgree->SetFixedWindowLocalPos(D3DXVECTOR2(m_fAgreeBtnOriginalPosX + 60, m_pkAgree->GetFixedWindowLocalPos().y));
		m_pkDisagree->ToggleRender( false );
	}

	m_pkScroll->SetScrollPageSize( m_pkMessage->GetDisplayLimitLine() );
	m_pkScroll->SetScrollRangeMax( m_pkMessage->GetViewContentSize() );
    m_pkMessage->SetTopIndex(0);
}

bool KGCAgreement::InitDialog( IN const KDialogInfo& kInfo_ )
{

#if !defined( NATION_TAIWAN )    
    this->SetStringID(static_cast<int>( kInfo_.m_lParam ));
#endif
    return true;
}

void KGCAgreement::OnChangeBeanfun()
{
    PopUpWebPage( L"https://tw.event.gamania.com/BeanFunEvent/E20100715/index.html" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCAgreement::OnBeanfunHK()
{
    PopUpWebPage( L"http://hk.beanfun.com/game_zone/?service_code=300036&service_region=TA" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCAgreement::OnChangeBeanfunHK()
{
    PopUpWebPage( L"https://www.gamania.com.hk/beanfun/GashBeanFun/beanfun.aspx");
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}


IMPLEMENT_CLASSNAME( KGCAgreement1301 );
IMPLEMENT_WND_FACTORY( KGCAgreement1301 );
IMPLEMENT_WND_FACTORY_NAME( KGCAgreement1301, "gc_kgcagreement_1301" );

KGCAgreement1301::KGCAgreement1301(void)
: m_pkScrollbar(NULL)
, m_pkBtnOK(NULL)
, m_pkBtnCancel(NULL)
, m_pkMessage(NULL)
, m_pkCheck(NULL)
{
    LINK_CONTROL("scroll",m_pkScrollbar);
    LINK_CONTROL("ok_btn",m_pkBtnOK);
    LINK_CONTROL("not_btn",m_pkBtnCancel);
    LINK_CONTROL("list",m_pkMessage);
    LINK_CONTROL("cb_agree",m_pkCheck);
}

KGCAgreement1301::~KGCAgreement1301(void)
{
}

void KGCAgreement1301::OnCreate( void )
{
    m_pkScrollbar->InitState(true,true,this);
    m_pkBtnOK->InitState(true,true,this);
    m_pkBtnOK->Lock(true);
    m_pkBtnCancel->InitState(true,true,this);
    m_pkMessage->InitState(true,true,this);
    m_pkCheck->InitState(true,true,this);
    m_pkCheck->SetCheck(false);

    LoadAgree();
}

void KGCAgreement1301::OnCreateComplete( void )
{
    m_pkCheck->SetText(g_pkStrLoader->GetString(STR_AGREEMENT_CHECK));

}

void KGCAgreement1301::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkScrollbar ) {
        OnScroll(event);
        return;
    }

    GCWND_MSG_MAP( m_pkCheck, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCheckBox );
    GCWND_MSG_MAP( m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOKBtn );
    GCWND_MSG_MAP( m_pkBtnCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickCancelBtn );


}

void KGCAgreement1301::OnScroll( const KActionEvent& event_ )
{
    UNREFERENCED_PARAMETER( event_ );

    int iTop = m_pkScrollbar->GetScrollPos();
    if ( m_pkMessage->GetTopIndex() != iTop )
    {
        g_KDSound.Play( "73" );
        m_pkMessage->SetTopIndex( iTop );
    }
}

void KGCAgreement1301::LoadAgree()
{
    m_pkMessage->Clear();
    m_pkMessage->SetMaxLine( KD3DListBox::NUM_MAX_LINE_EX );

    boost::shared_array<char> filedata;
    int length = 0;
    if(g_pGCDeviceManager2->GetMassFileManager()->LoadMemoryFile("agreement.stg", filedata,length) == false)
        return;

    int Wordlength = (length / 2) + 1;

    WCHAR *pWCHAR = new WCHAR[Wordlength];
    memcpy(pWCHAR,filedata.get(),length);

    std::wstring str;
    int index = 0;

    while(SiKGCMultipleLanguages()->GetLine( pWCHAR,index,L'\n',str ) && index < Wordlength)
    {
        int iIndex = _wtoi( str.substr(0, str.find( L"\t" ) ).c_str() );
        std::wstring strDataString = str.substr( 0, str.find( L"\n" ) - 1 ); // \t 다음부터 \n 까지

        m_pkMessage->AddString(strDataString);
    }

    m_pkScrollbar->SetScrollPageSize( m_pkMessage->GetDisplayLimitLine() );
    m_pkScrollbar->SetScrollRangeMax( m_pkMessage->GetViewContentSize() );
    m_pkMessage->SetTopIndex(0);

    delete pWCHAR;
}

void KGCAgreement1301::OnClickCheckBox()
{
    m_pkCheck->SetCheck(!m_pkCheck->GetCheck());
    m_pkBtnOK->Lock(!m_pkCheck->GetCheck());
}

void KGCAgreement1301::OnClickOKBtn()
{
    KP2P::GetInstance()->Send_CYOU_UserAgreement();
    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_AGREEMENT_OK ) );
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
    m_pkMessage->Clear();
}

void KGCAgreement1301::OnClickCancelBtn()
{
    g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_AGREEMENT_OK ) );
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
    m_pkMessage->Clear();
}