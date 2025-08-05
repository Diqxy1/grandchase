#include "stdafx.h"
#include "GCSmartPanel.h"

IMPLEMENT_CLASSNAME( KGCSmartPanel );
IMPLEMENT_WND_FACTORY( KGCSmartPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCSmartPanel, "gc_smartpanel" );

KGCSmartPanel::KGCSmartPanel( void ) : 
m_iShowLineIndex(0),
m_iSelectLineIndex(-1)
{
    m_strEditID.clear();
    m_strID.clear();
    m_strSelectedID.clear();

    m_vecAllMailList.clear();

    LINK_CONTROL( "smartpanel_popup",  m_pkSmartPanel_Popoup );
    LINK_CONTROL( "smartpanel_select",  m_pkSmartPanel_Select );

    for ( int i = 0; i < MAX_SHOW_LINE; ++i)
    {
        m_pkPopupTextList[i] = new KD3DStatic;

        m_pkPopupTextList[i]->SetSelfInputCheck( true );
        m_pkPopupTextList[i]->AddActionListener( this );
        m_pkPopupTextList[i]->ToggleRender( true );
        m_pkPopupTextList[i]->SetAlign( DT_LEFT );

        m_pkPopupTextList[i]->SetWndState(D3DWS_ENABLED);
        m_pkPopupTextList[i]->SetWidth(0);
        m_pkPopupTextList[i]->SetHeight( TEXT_LINE_HEIGHT );
        m_pkPopupTextList[i]->SetFontSize( 12 );
        m_pkPopupTextList[i]->SetText();

        m_vecChild.push_back(m_pkPopupTextList[i]);
    }
}

KGCSmartPanel::~KGCSmartPanel( void )
{    
}

void KGCSmartPanel::OnCreate( void )
{
    m_pkSmartPanel_Select->InitState( false, false, NULL );
    m_pkSmartPanel_Select->SetWidth(0);
    SetChildOrderBottom( m_pkSmartPanel_Select );

    m_pkSmartPanel_Popoup->InitState( false, false, NULL );
    m_pkSmartPanel_Popoup->SetWidth(0);
    SetChildOrderBottom( m_pkSmartPanel_Popoup );
}

void KGCSmartPanel::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        for ( int i = 0; i < MAX_SHOW_LINE; ++i)
        {
            if ( event.m_pWnd == m_pkPopupTextList[i] )
            {
                m_iSelectLineIndex = i;
                SpeakToActionListener( KActionEvent(this,D3DWE_BUTTON_CLICK) );
                return;
            }
        }
    }
}

void KGCSmartPanel::SetEditID( const std::wstring& strEditID )
{

    if ( 0 != m_strEditID.compare( strEditID ) )
    {
        m_strEditID.clear();
        m_strEditID = strEditID;
        
        if ( 0 == m_strEditID.find( L"@" ) || m_strEditID.empty() )
        {
            PopupListAllClear();
            PanelRender(false);
            return;
        }

        SetOnlyIDString(m_strEditID);
    }
}

void KGCSmartPanel::OnInitialize()
{
    if ( g_kGlobalValue.IsSmartPanelMailListEnable() )
    {
        m_vecAllMailList = g_kGlobalValue.GetSmartPanelMailList();
    }

    for ( int i = 0; i < MAX_SHOW_LINE; ++i)
    {
        D3DXVECTOR2 vStaticOffSet( 4, (float)(i * TEXT_LINE_HEIGHT) );

        D3DXVECTOR2 vPos = GetFixedWindowPos();

        vPos.x += vStaticOffSet.x;
        vPos.y += vStaticOffSet.y + 3.f;

        m_pkPopupTextList[i]->SetWindowPos(vPos);
    }
}

void KGCSmartPanel::PanelRender( bool render_ )
{
    if ( render_ == false )
    {
        SetSelectPanelPos( 0, false );
    }

    m_pkSmartPanel_Popoup->ToggleRender(render_);

    for ( int i = 0; i < MAX_SHOW_LINE; ++i)
    {
        m_pkPopupTextList[i]->ToggleRender(render_);
    }
}

void KGCSmartPanel::SetOnlyIDString( const std::wstring& editString_ )
{
    std::wstring strID;
    std::wstring strSearchKeyward;
    std::wstring::size_type idx;

    strID.clear();
    strID = editString_;

    strSearchKeyward.clear();

    if ( 0 == strID.compare( L"13" ) ||
        0 == strID.compare( L"15" ) ||
        0 == strID.compare( L"18" ) ||
        ( IsPhoneNumber(strID) && (int)strID.size() == PHONENUMBER_SIZE ) )
    {
        m_strID = strID;
        SearchMailList( strSearchKeyward, true );
        return;
    }

    idx = strID.find( L"@" );
    if ( idx == std::wstring::npos )
    {
        m_strID = strID;
        SearchMailList( strSearchKeyward );
        return;
    }
    else
    {
        strSearchKeyward = strID.substr( idx );
        m_strID = strID.substr( 0, idx );
        SearchMailList( strSearchKeyward );
    }

    return;
}

void KGCSmartPanel::SearchMailList( const std::wstring& searchKeyward_, bool bPhoneNumber_ /*= false */ )
{
    DWORD listWidth = 0;

    m_iShowLineIndex = 0;
    PopupListAllClear();

    if ( searchKeyward_.empty() )
    {
        if ( bPhoneNumber_ )
        {
            m_pkPopupTextList[m_iShowLineIndex]->SetText( m_strID );
            m_iShowLineIndex++;
        }

        std::vector< std::wstring >::iterator itor;
        for ( itor = m_vecAllMailList.begin(); itor != m_vecAllMailList.end(); ++itor )
        {
            if ( m_iShowLineIndex >= MAX_SHOW_LINE )
                break;

            m_pkPopupTextList[m_iShowLineIndex]->SetText( m_strID + (*itor) );
            DWORD width = (m_pkPopupTextList[m_iShowLineIndex]->GetStringWidthAndHeightInPixel()).x;
            listWidth = ( listWidth >= width ? listWidth : width );
            m_pkPopupTextList[m_iShowLineIndex]->SetWidth( listWidth );
            m_pkSmartPanel_Popoup->SetWidth( listWidth );
            m_pkSmartPanel_Select->SetWidth( listWidth );

            m_iShowLineIndex++;
        }
    }
    else
    {
        std::vector< std::wstring >::iterator itor;
        for ( itor = m_vecAllMailList.begin(); itor != m_vecAllMailList.end(); ++itor )
        {
            if ( m_iShowLineIndex >= MAX_SHOW_LINE )
                break;

            if ( itor->find( searchKeyward_.c_str() ) == std::wstring::npos )
                continue;

            m_pkPopupTextList[m_iShowLineIndex]->SetText( m_strID + (*itor) );
            DWORD width = (m_pkPopupTextList[m_iShowLineIndex]->GetStringWidthAndHeightInPixel()).x;
            listWidth = ( listWidth >= width ? listWidth : width );
            m_pkPopupTextList[m_iShowLineIndex]->SetWidth( listWidth );
            m_pkSmartPanel_Popoup->SetWidth( listWidth );
            m_pkSmartPanel_Select->SetWidth( listWidth );

            m_iShowLineIndex++;
        }
    }

    if ( m_pkPopupTextList[0]->IsEmpty() )
        PanelRender(false);
    else
        PanelRender(true);

    SetPopupHeight( m_iShowLineIndex * TEXT_LINE_HEIGHT );
}

void KGCSmartPanel::PopupListAllClear()
{
    for ( int i = 0; i < MAX_SHOW_LINE; ++i)
    {
        m_pkPopupTextList[i]->SetText();
    }
}

void KGCSmartPanel::SetPopupHeight( int height_ )
{
    m_pkSmartPanel_Popoup->SetHeight( height_ );
}

bool KGCSmartPanel::IsPhoneNumber( const std::wstring& strID_ )
{
    for ( int i = 0; i < (int)strID_.size(); ++i )
    {
        if ( !isdigit( strID_[i] ) )
            return false;
    }

    return true;
}

void KGCSmartPanel::FrameMoveInEnabledState( void )
{
    if ( !m_pkSmartPanel_Popoup->IsRenderOn() )
        return;

    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

    for ( int i = 0; i < MAX_SHOW_LINE; ++i)
    {
        if ( m_pkPopupTextList[i]->CheckPosInWindowBound(vMousePos) )
        {
            m_iSelectLineIndex = i;
        }
    }

    if ( g_pkInput->IsDown( DIK_DOWN ) )
    {
        if ( m_iSelectLineIndex < MAX_SHOW_LINE - 1 )
        {
            m_iSelectLineIndex++;
        }
    }
    else if ( g_pkInput->IsDown( DIK_UP ) )
    {
        if ( m_iSelectLineIndex > 0 )
        {
            m_iSelectLineIndex--;
        }
    }

    UpdateSelectPanel();
}

std::wstring KGCSmartPanel::GetSelectedID()
{
    if ( m_iSelectLineIndex < 0 )
        m_iSelectLineIndex = 0;

    m_strSelectedID = m_pkPopupTextList[m_iSelectLineIndex]->GetText();
    if ( m_strSelectedID.empty() )
        m_strSelectedID = m_strEditID;
    m_pkPopupTextList[m_iSelectLineIndex]->SetFontColor( D3DCOLOR_ARGB(255,255,255,255) );
    m_iSelectLineIndex = -1;
    PopupListAllClear();
    PanelRender(false);

    return m_strSelectedID;
}

void KGCSmartPanel::SetSelectPanelPos( int selectIndex_, bool render_ )
{
    D3DXVECTOR2 vPos = GetFixedWindowPos();

    vPos.x = 0.f;
    vPos.y = (float)selectIndex_ * TEXT_LINE_HEIGHT;

    m_pkSmartPanel_Select->SetWindowPos(vPos);
    m_pkSmartPanel_Select->ToggleRender(render_);
}

void KGCSmartPanel::UpdateSelectPanel()
{
    if ( m_iSelectLineIndex > m_iShowLineIndex - 1 )
        m_iSelectLineIndex = m_iShowLineIndex - 1;

    for ( int i = 0; i < MAX_SHOW_LINE; ++i)
    {
        if ( m_iSelectLineIndex == i )
            m_pkPopupTextList[i]->SetFontColor( D3DCOLOR_ARGB(255,255,255,0) );
        else
            m_pkPopupTextList[i]->SetFontColor( D3DCOLOR_ARGB(255,255,255,255) );
    }

    if ( m_iSelectLineIndex < 0 )
        SetSelectPanelPos( 0, false );
    else
        SetSelectPanelPos( m_iSelectLineIndex, true );
}

bool KGCSmartPanel::IsRenderPopup()
{
    return m_pkSmartPanel_Popoup->IsRenderOn();
}
