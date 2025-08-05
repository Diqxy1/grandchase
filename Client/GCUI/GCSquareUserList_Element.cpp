#include "stdafx.h"
#include "GCSquareUserList_Element.h"
//
//
#include "KGCGradeMark.h"

IMPLEMENT_CLASSNAME( KGCSquareUserList_Element );
IMPLEMENT_WND_FACTORY( KGCSquareUserList_Element );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareUserList_Element, "gc_square_userlist_element" );

KGCSquareUserList_Element::KGCSquareUserList_Element(void)
: m_pkUserName(NULL)
, m_pkGradeMark(NULL)
, m_pkNameBG(NULL)
, m_bIsInNameBG(false)
, m_bIsRBtnClick(false)
, m_bIsLBtnClick(false)
{
    LINK_CONTROL( "name",       m_pkUserName );
    LINK_CONTROL( "grade_mark", m_pkGradeMark );
    LINK_CONTROL( "nameback", m_pkNameBG );
}

KGCSquareUserList_Element::~KGCSquareUserList_Element(void)
{
}

void KGCSquareUserList_Element::OnCreate( void )
{
    m_pkUserName->InitState(true);
    m_pkGradeMark->InitState(true);
    m_pkNameBG->InitState(true, true, this);
}

void KGCSquareUserList_Element::SetUserInfo( std::wstring strUserName, int iGradeMark, DWORD dwUID )
{
    m_pkUserName->SetText( strUserName );
    m_pkUserName->SetOffset( D3DXVECTOR2( 6.0f, 4.0f ) );
    m_pkUserName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
    m_pkGradeMark->SetGrade( iGradeMark );
    m_dwUID = dwUID;
}

void KGCSquareUserList_Element::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkNameBG)
    {
        switch ( event.m_dwCode )
        {
        case D3DWE_CURSOR_ENTER:
            m_pkNameBG->SetWndMode( D3DWM_HOVER, false );
            m_bIsInNameBG = true;
            break;

        case D3DWE_CURSOR_LEAVE:
            m_pkNameBG->SetWndMode( D3DWM_DEFAULT, false );
            m_bIsInNameBG = false;
            break;

        case D3DWE_RBUTTON_CLICK:
            if ( m_bIsInNameBG )
                m_bIsRBtnClick = true;
            break;

        case D3DWE_BUTTON_CLICK:
            if ( m_bIsInNameBG )
                m_bIsLBtnClick = true;
            break;
        }
    }
}

void KGCSquareUserList_Element::initElement( void )
{
    m_pkUserName->SetText();
    m_pkGradeMark->SetGrade();
}

void KGCSquareUserList_Element::SetRClick( bool IsClick )
{
    m_bIsRBtnClick = IsClick;
}

bool KGCSquareUserList_Element::IsEmpty( void )
{
    if ( L"" == m_pkUserName->GetText() )
        return true;
    else
        return false;
}

std::wstring KGCSquareUserList_Element::GetUserName( void ) const
{
    return m_pkUserName->GetText();
}

void KGCSquareUserList_Element::SetLClick( bool IsClick )
{
     m_bIsLBtnClick = IsClick;
}