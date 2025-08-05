#include "stdafx.h"
#include "GCAgitGuestbook.h"

IMPLEMENT_CLASSNAME( KGCAgitGuestbook );
IMPLEMENT_WND_FACTORY( KGCAgitGuestbook );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitGuestbook, "gc_agit_guestbook" );

KGCAgitGuestbook::KGCAgitGuestbook( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnInput = NULL;
    m_pkEditMent = NULL;
    m_pkStaticNumMent = NULL;
    m_pkStaticLink = NULL;
    LINK_CONTROL( "btnClose", m_pkBtnClose );
    LINK_CONTROL( "btnInput", m_pkBtnInput );
    LINK_CONTROL( "editMent", m_pkEditMent );
    LINK_CONTROL( "staticInputNum", m_pkStaticNumMent );
    LINK_CONTROL( "pagelink", m_pkStaticLink );

    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        m_apkSlot[i] = NULL;

        char szTemp[128] = {0, };
        sprintf_s( szTemp, 127, "guestbooSlot%d", i );
        LINK_CONTROL( szTemp, m_apkSlot[i] );
    }

}

KGCAgitGuestbook::~KGCAgitGuestbook( void )
{
}

void KGCAgitGuestbook::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnInput->InitState( true, true, this );
    m_pkStaticNumMent->InitState( true );
    m_pkStaticNumMent->SetAlign( DT_CENTER );

    m_pkEditMent->InitState( true, true, this );
    m_pkEditMent->SetLimitText( LENGTH_MAX_MENT );
    m_pkEditMent->EnableSelfFocusCheck( true );
    m_pkEditMent->SetMultiLine( true );
    m_pkEditMent->SetNewLine( true );
    m_pkEditMent->SetLimitLine( 3 );

    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->InitState( true );

    m_pkStaticLink->InitState( true, true, this );
    m_pkStaticLink->SetDefaultColor( 0xFFFFF7D3 );
    m_pkStaticLink->SetHoverColor( 0xFFFFA70F );
    m_pkStaticLink->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    SetLinkPage( 1 );

    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
}

void KGCAgitGuestbook::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkStaticLink && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK && event.m_dwlParam2 == KD3DStaticLink::SL_ELEMENT_CLICK )
    {
        std::wstring pageNum = ((KD3DStatic*)event.m_dwlParam)->GetText();
        if ( pageNum == L"▶" )
            SetPage( KGCAgitGuestbook::BTN_NEXT );
        else if ( pageNum == L"◀" )
            SetPage( KGCAgitGuestbook::BTN_PREV );
        else
        {
            int iPage = _wtoi( pageNum.c_str() );
            SetPage( iPage );
        }
    }

    GCWND_MSG_MAP( m_pkEditMent,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditMent );
    GCWND_MSG_MAP( m_pkBtnInput,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnInput );
    GCWND_MSG_MAP( m_pkBtnClose,    KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
}

void KGCAgitGuestbook::FrameMoveInEnabledState( void )
{
    SetEditNum();
}

void KGCAgitGuestbook::InitUI( void )
{
    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->Clear();

    m_pkEditMent->SetText( L"" );
    m_pkStaticNumMent->SetText( L"" );

    Result_AgitGuestbookPage = INT_MAX;
    KP2P::GetInstance()->Send_GuestbookPageReq();
    g_MyD3D->WaitForServerAck( Result_AgitGuestbookPage, INT_MAX, 3000, TIME_OUT_VALUE );
}

void KGCAgitGuestbook::OnInput( void )
{
    std::wstring strMent = m_pkEditMent->GetText();

    if( strMent.empty() )   // 비었음 
        return;

    if( false == IsValidLength( strMent ) )  // 제한길이 초과 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_TEXT_LENGTH ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return;
    }

    if( false == IsValidMent( strMent ) )   // 금지어 포함 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_INVALID_TEXT_INPUT ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        return;
    }

    KP2P::GetInstance()->Send_GuestbookWriteReq( strMent );
    m_pkEditMent->SetText( L"" );
}

void KGCAgitGuestbook::OnClose( void )
{
    this->ToggleRender( false );
}

void KGCAgitGuestbook::InitPage( void )
{
    m_iTotalMent = static_cast<int>( m_vecGuestBook.size() );
    
    m_iTotalPage = m_iTotalMent / NUM_SLOT;
    if( m_iTotalMent % NUM_SLOT )
        ++m_iTotalPage;

    m_iCurPage = 1;
    m_iPageOffset = 0;

    SetSlot( m_iCurPage );
    SetLinkPage( m_iCurPage );
}

void KGCAgitGuestbook::UpdatePage( void )
{
    m_iTotalMent = static_cast<int>( m_vecGuestBook.size() );

    m_iTotalPage = m_iTotalMent / NUM_SLOT;
    if( m_iTotalMent % NUM_SLOT )
        ++m_iTotalPage;
    
    if( m_iCurPage > m_iTotalPage )
        m_iCurPage = m_iTotalPage;

    if( m_iCurPage % 10 == 0 )
        --m_iPageOffset;
}

void KGCAgitGuestbook::SetSlot( int iPage_ )
{
    if( iPage_ == 0 )
        return;

    for( int i=0 ; i<NUM_SLOT ; ++i )
        m_apkSlot[i]->Clear();

    int iStartIndex = ( iPage_ - 1 ) * NUM_SLOT;

    for( int i=0 ; i<NUM_SLOT ; ++i )
    {
        if( iStartIndex + i >= m_iTotalMent )
            break;

        std::pair< int, KGuestMsg > tempInfo = m_vecGuestBook[ iStartIndex + i ];
        m_apkSlot[i]->SetSlot( tempInfo.first, tempInfo.second.m_dwUserUID, tempInfo.second.m_strNick, tempInfo.second.m_tmDate, tempInfo.second.m_strMsg );
    }
}

void KGCAgitGuestbook::SetEditNum( void )
{
    std::wstring strMent = m_pkEditMent->GetText();
    
    int iCurMentCount = static_cast<int>( strMent.size() );

    std::wostringstream stm;
    stm << iCurMentCount << L"/" << LENGTH_MAX_MENT ;
    m_pkStaticNumMent->SetText( stm.str() );
}

void KGCAgitGuestbook::SetGuestbookInfo( std::map<int, KGuestMsg>& info_ )
{
    m_vecGuestBook.clear();
    m_mapGuestBook.clear();

    for( std::map< int, KGuestMsg >::iterator mit = info_.begin() ; mit != info_.end() ; ++mit )
        m_vecGuestBook.insert( m_vecGuestBook.begin(), std::make_pair( mit->first, mit->second ) );

    m_mapGuestBook = info_;

    InitPage();
}

void KGCAgitGuestbook::SetLinkPage( int iStartPage )
{
    m_pkStaticLink->Clear();
    m_pkStaticLink->AddLinkString( L"◀", true );
    for( int i=0 ; i<NUM_LINK_PAGE ; ++i )
    {
        std::wostringstream strNum;
        strNum << iStartPage + i ;
        m_pkStaticLink->AddLinkString( strNum.str(), true );
    }
    m_pkStaticLink->AddLinkString( L"▶", true );
}

void KGCAgitGuestbook::SetPage( PAGE_BTN eBtn_ )
{
    switch( eBtn_ )
    {
    case BTN_PREV:
        if( m_iCurPage > 1 )
        {
            --m_iCurPage;

            if( m_iCurPage % NUM_LINK_PAGE == 0 )
            {
                SetLinkPage( m_iCurPage - NUM_LINK_PAGE + 1 );
                --m_iPageOffset;
            }

            SetSlot( m_iCurPage );
        }
        break;

    case BTN_NEXT:
        if( m_iCurPage < m_iTotalPage )
        {
            ++m_iCurPage;

            if( m_iCurPage % NUM_LINK_PAGE == 1 )
            {
                SetLinkPage( m_iCurPage );
                ++m_iPageOffset;
            }

            SetSlot( m_iCurPage );
        }
        break;
    }
}

void KGCAgitGuestbook::SetPage( int iPage_ )
{
    if( m_iTotalPage == 0 )
        return;

    int iResultPage = ( m_iPageOffset * NUM_LINK_PAGE ) + iPage_;

    if( iResultPage > m_iTotalPage )
        iResultPage = m_iTotalPage;

    m_iCurPage = iResultPage;
    SetSlot( m_iCurPage );
}

void KGCAgitGuestbook::OnClickEditMent( void )
{
    m_pkEditMent->SetText( L"" );
    m_pkEditMent->SetFocus();
}

bool KGCAgitGuestbook::IsValidMent( std::wstring& strMent_ )
{
    return SiKSlangFilter()->FilterCheck( strMent_ );
}

bool KGCAgitGuestbook::IsValidLength( std::wstring& strMent_ )
{
    if( strMent_.length() > LENGTH_MAX_MENT )
        return false;

    return true;
}

void KGCAgitGuestbook::AddNewMessage( int iMsgNum_, KGuestMsg& Msg_ )
{
    std::pair< std::map<int, KGuestMsg >::iterator, bool > prState;
    prState = m_mapGuestBook.insert( std::make_pair( iMsgNum_, Msg_ ) );

    if( prState.second )
    {
        m_vecGuestBook.insert( m_vecGuestBook.begin(), std::make_pair( iMsgNum_, Msg_ ) );
        UpdatePage();
        SetSlot( m_iCurPage );
    }
}

void KGCAgitGuestbook::DeleteMessage( std::set<int>& setDeleteID_ )
{   
    // 지우고 
    for( std::set<int>::iterator sit = setDeleteID_.begin() ; sit != setDeleteID_.end() ; ++sit )
    {
        std::map< int, KGuestMsg >::iterator mit = m_mapGuestBook.find( *sit );
        if( mit == m_mapGuestBook.end() )
            return;

        m_mapGuestBook.erase( mit );
    }

    // 벡터 다시 정렬 
    m_vecGuestBook.clear();
    for( std::map< int, KGuestMsg >::iterator mit = m_mapGuestBook.begin() ; mit != m_mapGuestBook.end() ; ++mit )
        m_vecGuestBook.insert( m_vecGuestBook.begin(), std::make_pair( mit->first, mit->second ) );

    UpdatePage();
    SetSlot( m_iCurPage );     
}

