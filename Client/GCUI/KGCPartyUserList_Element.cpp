#include "stdafx.h"
#include "KGCPartyUserList_Element.h"

IMPLEMENT_CLASSNAME( KGCPartyUserList_Element );
IMPLEMENT_WND_FACTORY( KGCPartyUserList_Element );
IMPLEMENT_WND_FACTORY_NAME( KGCPartyUserList_Element, "gc_party_user_list_element" );

KGCPartyUserList_Element::KGCPartyUserList_Element(void)
: m_pkUserName(NULL)
, m_pkNameBG(NULL)
{
    LINK_CONTROL("user_name" , m_pkUserName );
    LINK_CONTROL("back" ,  m_pkNameBG);
}

KGCPartyUserList_Element::~KGCPartyUserList_Element(void)
{
}

void KGCPartyUserList_Element::OnCreate( void )
{
    if (m_pkUserName )
        m_pkUserName->InitState( true );
    m_pkNameBG ->InitState(false);
}

void KGCPartyUserList_Element::ActionPerformed( const KActionEvent& event )
{

}
void KGCPartyUserList_Element::CursorEnter( bool bCursorEnter )
{
    m_pkNameBG->ToggleRender( bCursorEnter );
    m_pkNameBG->SetWndMode( D3DWM_HOVER, false );
}

void KGCPartyUserList_Element::initElement( void )
{
    m_pkUserName->SetText(L"");
    m_dwUID = 0;
}

void KGCPartyUserList_Element::SetPartyMember( const KPartyMember& memberInfo )
{
    m_pkUserName->SetText(memberInfo.m_strNickName);
    m_pkUserName->SetOffset( D3DXVECTOR2( 6.0f, 4.0f ) );
    m_pkUserName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
    m_dwUID = memberInfo.m_dwUID;
    //m_pkHostMark->ToggleRender(memberInfo.m_bHost);
}