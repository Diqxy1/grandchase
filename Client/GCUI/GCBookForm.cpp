#include "stdafx.h"
#include "GCBookForm.h"

IMPLEMENT_CLASSNAME( KGCBookForm );
IMPLEMENT_WND_FACTORY( KGCBookForm );
IMPLEMENT_WND_FACTORY_NAME( KGCBookForm, "gc_book_form" );

KGCBookForm::KGCBookForm( void )
: m_pkBtnPrev( NULL )
, m_pkBtnNext( NULL )
, m_pkFormAStatic( NULL )
, m_pkFormBStatic( NULL )
, m_pkFormBImageIcon( NULL )
, m_pkBtnClose( NULL )
, m_pkBackPage( NULL )
, m_pkStaticPage( NULL )
, m_bAllShowMode( false )
, m_bStartAnimMent( false )
, m_iCurrentPage( 0 )
{
    LINK_CONTROL( "back", m_pkBackStyle ); 

    LINK_CONTROL( "btn_prev",           m_pkBtnPrev );
    LINK_CONTROL( "btn_next",           m_pkBtnNext );
    LINK_CONTROL( "btn_close",          m_pkBtnClose );

    LINK_CONTROL( "form_a_static",      m_pkFormAStatic );
    LINK_CONTROL( "form_b_static",      m_pkFormBStatic );
    
    LINK_CONTROL( "form_b_imageicon",   m_pkFormBImageIcon );

    LINK_CONTROL( "back_page",   m_pkBackPage );
    LINK_CONTROL( "static_page",   m_pkStaticPage );
    
}

KGCBookForm::~KGCBookForm( void )
{
}

void KGCBookForm::OnCreate( void )
{
    m_pkBackStyle->InitState( true );

    m_pkBtnPrev->InitState( false, true, this );
    m_pkBtnNext->InitState( false, true, this );
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    
    m_pkFormAStatic->InitState( false );
    m_pkFormAStatic->SetFontColor( D3DCOLOR_ARGB( 255, 77, 44, 29) );
    m_pkFormAStatic->SetText( L"" );
    m_pkFormAStatic->SetLineSpace( 1.4f );

    m_pkFormBStatic->InitState( false );
    m_pkFormBStatic->SetFontColor( D3DCOLOR_ARGB( 255, 77, 44, 29) );
    m_pkFormBStatic->SetText( L"" );
    m_pkFormBStatic->SetLineSpace( 1.4f );

    m_pkStaticPage->InitState( false );
    m_pkStaticPage->SetFontColor( D3DCOLOR_ARGB( 255, 232, 212, 138) );
    m_pkStaticPage->SetAlign( DT_CENTER );
    m_pkStaticPage->SetText( L"" );

    m_pkBackPage->InitState( false );

    m_pkFormBImageIcon->InitState( false );

    m_bAllShowMode = false;
    m_iCurrentPage = 0;

    m_strMentAnim.clear();
}

void KGCBookForm::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnPrev,			KD3DWnd::D3DWE_BUTTON_CLICK, OnPrev );
    _GCWND_MSG_MAP( m_pkBtnNext,			KD3DWnd::D3DWE_BUTTON_CLICK, OnNext );
    _GCWND_MSG_MAP( m_pkBtnClose,			KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCBookForm::FrameMoveInEnabledState( void )
{
    RefreshMent();
}

void KGCBookForm::ShowBookPage( int strDesc_, int strAudio_, std::wstring& strFileName_ )
{
    bool bIsTypeA = false;
    if( strFileName_ == L"" ) {        
        bIsTypeA = true;
    } else {
        m_pkFormBImageIcon->SetTexNameUseMassFile( strFileName_ );
    }

    m_pkFormBStatic->ToggleRender( !bIsTypeA );
    m_pkFormBImageIcon->ToggleRender( !bIsTypeA );
    m_pkFormAStatic->ToggleRender( bIsTypeA );

    m_strMentAnim.clear();
    m_bStartAnimMent = true;
    m_strCurrentMent = g_pkStrLoader->GetString(strDesc_).c_str();

    std::stringstream strAudioID;
    strAudioID.str("");
    strAudioID << strAudio_;
    g_KDSound.Play( strAudioID.str().c_str() );

    if( m_bAllShowMode ) {
        std::wstringstream str;
        str << m_iCurrentPage + 1 << "/" << m_vecBookData.size();
        m_pkStaticPage->SetText( str.str() );
    }    

    m_pkBtnPrev->ToggleRender( m_bAllShowMode );
    m_pkBtnNext->ToggleRender( m_bAllShowMode );
    m_pkBackPage->ToggleRender( m_bAllShowMode );
    m_pkStaticPage->ToggleRender( m_bAllShowMode );
}

void KGCBookForm::ShowBookAllPage()
{
    m_bAllShowMode = true;
    m_iCurrentPage = 0;
    if( !m_vecBookData.empty() ) {
        ShowBookPage( m_vecBookData[m_iCurrentPage].m_strDesc, m_vecBookData[m_iCurrentPage].m_strAudio, m_vecBookData[m_iCurrentPage].m_strImgFileName );
    }
}

void KGCBookForm::SetBookFormData( const std::vector< KBookData >& vecBookData_ )
{
    m_vecBookData = vecBookData_;
}

void KGCBookForm::OnPrev()
{
    m_iCurrentPage--;

    if( m_iCurrentPage < 0 ) {
        m_iCurrentPage = 0;
    }

    if( !m_vecBookData.empty() ) {
        std::stringstream strAudioID;
        strAudioID.str("");
        strAudioID << m_vecBookData[m_iCurrentPage + 1].m_strAudio;
        g_KDSound.Stop(strAudioID.str().c_str());
        ShowBookPage( m_vecBookData[m_iCurrentPage].m_strDesc, m_vecBookData[m_iCurrentPage].m_strAudio, m_vecBookData[m_iCurrentPage].m_strImgFileName );
    }
}

void KGCBookForm::OnNext()
{
    m_iCurrentPage++;

    if( m_iCurrentPage >= static_cast<int>( m_vecBookData.size() ) ) {
        m_iCurrentPage = m_vecBookData.size() -1;
    }

    if( !m_vecBookData.empty() ) {
        std::stringstream strAudioID;
        if (m_iCurrentPage > 0)
        {
            strAudioID.str("");
            strAudioID << m_vecBookData[m_iCurrentPage - 1].m_strAudio;
            g_KDSound.Stop(strAudioID.str().c_str());
        }
        ShowBookPage( m_vecBookData[m_iCurrentPage].m_strDesc, m_vecBookData[m_iCurrentPage].m_strAudio, m_vecBookData[m_iCurrentPage].m_strImgFileName );
    }
}

void KGCBookForm::OnClose()
{
    std::stringstream strAudioID;
    strAudioID.str("");
    strAudioID << m_vecBookData[m_iCurrentPage].m_strAudio;
    g_KDSound.Stop(strAudioID.str().c_str());
    this->ToggleRender( false );
}

void KGCBookForm::RefreshMent()
{
    if( !m_bStartAnimMent )
        return;

    m_strMentAnim.append( m_strCurrentMent.substr( m_strMentAnim.size(), 1 ) );

    if( m_pkFormBStatic->IsRenderOn() ) {
        m_pkFormBStatic->SetTextAutoMultiline( m_strMentAnim );
    } else {
        m_pkFormAStatic->SetTextAutoMultiline( m_strMentAnim );
    }

    if( m_strCurrentMent.size() <= m_strMentAnim.size() )
        m_bStartAnimMent = false;
}

void KGCBookForm::SetBackStyle( int iBackStyle_ )
{
    if ( iBackStyle_ != -1 )
    {
        m_pkBackStyle->SetWndTex( D3DWM_DEFAULT,  iBackStyle_ );
    }    
}

void KGCBookForm::SetCurrentPage( int iCurrentPage_ )
{
    m_iCurrentPage = iCurrentPage_;

}