#include "stdafx.h"
#include "GCSquareUserList_Element.h"
//
//
#include "KGCGradeMark.h"

IMPLEMENT_CLASSNAME( KGCCheckuser_ListElement );
IMPLEMENT_WND_FACTORY( KGCCheckuser_ListElement );
IMPLEMENT_WND_FACTORY_NAME( KGCCheckuser_ListElement, "gc_checkuser_listitem" );

KGCCheckuser_ListElement::KGCCheckuser_ListElement(void)
: m_pkUserName(NULL)
, m_pkGradeMark(NULL)
, m_pkCheckBox(NULL)
, m_bIsInNameBG(false)
, m_pkNameBG(NULL)
{
    LINK_CONTROL( "static_name",m_pkUserName );
    LINK_CONTROL( "grade_mark", m_pkGradeMark );
    LINK_CONTROL( "checkbox_user", m_pkCheckBox );
    LINK_CONTROL( "back", m_pkNameBG );
}

KGCCheckuser_ListElement::~KGCCheckuser_ListElement(void)
{
}

void KGCCheckuser_ListElement::OnCreate( void )
{
    m_pkUserName->InitState(true);
    m_pkGradeMark->InitState(true);
    m_pkCheckBox->InitState(true, true, this);
    m_pkCheckBox->SetCheck(false);
    m_pkNameBG->InitState(true, true, this);
}

void KGCCheckuser_ListElement::SetUserInfo( std::wstring strUserName, int iGradeMark, DWORD dwUID )
{
    m_pkUserName->SetText( strUserName );
    m_pkUserName->SetOffset( D3DXVECTOR2( 6.0f, 4.0f ) );
    m_pkUserName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
    m_pkGradeMark->SetGrade( iGradeMark );
    m_dwUID = dwUID;
}

void KGCCheckuser_ListElement::ActionPerformed( const KActionEvent& event )
{
    if ( event.m_pWnd == m_pkNameBG )
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

        case D3DWE_BUTTON_CLICK:
            {
                if ( m_bIsInNameBG )
                {
                    m_pkNameBG->SetWndMode( D3DWM_SELECT, false );
                
                    m_pkCheckBox->SetCheck( !m_pkCheckBox->GetCheck() );
                    KActionEvent kevent(this,0, m_pkCheckBox->GetCheck() ? 100 : 200 );
                    m_pkNameBG->SetWndMode( D3DWM_HOVER, false );
                    SpeakToActionListener( kevent );
                }
            }
            break;
        }
    }
    else if( event.m_pWnd == m_pkCheckBox )
    {
        switch (event.m_dwCode )
        {
        case D3DWE_CURSOR_ENTER:
            m_pkNameBG->SetWndMode( D3DWM_HOVER, false );
            break;

        case D3DWE_BUTTON_CLICK:
            m_pkNameBG->SetWndMode( D3DWM_SELECT, false );

            m_pkCheckBox->SetCheck( !m_pkCheckBox->GetCheck() );
            KActionEvent kevent(this,0, m_pkCheckBox->GetCheck() ? 100 : 200 );
            m_pkNameBG->SetWndMode( D3DWM_HOVER, false );
            SpeakToActionListener( kevent );
            break;
        }
    }
}

void KGCCheckuser_ListElement::initElement( void )
{
    m_pkUserName->SetText();
    m_pkGradeMark->SetGrade();
}


bool KGCCheckuser_ListElement::IsEmpty( void )
{
    if ( L"" == m_pkUserName->GetText() )
        return true;
    else
        return false;
}

std::wstring KGCCheckuser_ListElement::GetUserName( void ) const
{
    return m_pkUserName->GetText();
}

void KGCCheckuser_ListElement::SetCheck( bool bCheck )
{
    m_pkCheckBox->SetCheck( bCheck );
}