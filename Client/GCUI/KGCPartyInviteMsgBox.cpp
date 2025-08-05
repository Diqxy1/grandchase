#include "stdafx.h"
//
#include "gcui/KGCPartyInviteMsgBox.h"
#include "GCBarControl.h"
//

#include "MyD3D.h"
#include "GCStateMachine.h"
//
#define BOX_END_TIME 4000

IMPLEMENT_CLASSNAME( KGCPartyInviteMsgBox );
IMPLEMENT_WND_FACTORY( KGCPartyInviteMsgBox );
IMPLEMENT_WND_FACTORY_NAME( KGCPartyInviteMsgBox, "gc_party_invite_msg_box" );

KGCPartyInviteMsgBox::KGCPartyInviteMsgBox(void)
: m_pkTitle(NULL)
, m_pkInvite(NULL)
, m_pkBar(NULL)
, m_pkYes(NULL)
, m_pkNo(NULL)
, m_bInviteAccept(false)
, m_dEndTime(0)
{
    LINK_CONTROL( "static_invite_message",  m_pkInvite );
    LINK_CONTROL( "bar",                    m_pkBar );
    LINK_CONTROL( "btn_yes",                m_pkYes );
    LINK_CONTROL( "btn_no",                 m_pkNo );
    LINK_CONTROL("invite_title",            m_pkTitle);
}

KGCPartyInviteMsgBox::~KGCPartyInviteMsgBox(void)
{

}

void KGCPartyInviteMsgBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkYes, D3DWE_BUTTON_CLICK, OnClickYes );
    GCWND_MSG_MAP( m_pkNo, D3DWE_BUTTON_CLICK, OnClickNo );
}

void KGCPartyInviteMsgBox::OnCreate()
{
    m_pkTitle->InitState( true );
    m_pkInvite->InitState( true );
    m_pkBar->InitState( true );
    m_pkYes->InitState( true, true, this );
    m_pkNo->InitState( true, true, this );
    m_pkNo->SetHotKey( DIK_ESCAPE );
    m_pkYes->SetHotKey( DIK_RETURN );
    m_pkYes->SetHotKey( DIK_NUMPADENTER );

	if ( SiKGCPartyManager()->IsParty() )
		OnClickNo();
}

void KGCPartyInviteMsgBox::OnCreateComplete()
{
    m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_INVITE_MESSAGE_TITLE ) );
    m_pkTitle->SetAlign( DT_CENTER );

    const KPartyData& kPartyInfo = SiKGCPartyManager()->GetPartyInfo();
    
    
    std::wstring strInvite = g_pkStrLoader->GetReplacedString( STR_ID_INVITE_MESSAGE, "L", kPartyInfo.m_vecPartyPeople.front().m_strNickName );
    m_pkInvite->SetTextAutoMultiline( strInvite );
    m_dEndTime = timeGetTime() + (BOX_END_TIME);
    m_pkBar->SetGage( 0.0f );
}

void KGCPartyInviteMsgBox::FrameMoveInEnabledState()
{
    // gage
    DWORD dTime = timeGetTime();
    if( m_dEndTime < dTime )
    {
        OnClickNo();
    }
    else
    {
        float fPercent = static_cast<float>( m_dEndTime - dTime ) / static_cast<float> (BOX_END_TIME);
        m_pkBar->SetGage( fPercent );
    }
}

void KGCPartyInviteMsgBox::OnClickYes()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    m_bInviteAccept = true;
}

void KGCPartyInviteMsgBox::OnClickNo()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    m_bInviteAccept = false;
}

void KGCPartyInviteMsgBox::OnDestroyComplete()
{
    SiKGCPartyManager()->SetInviteState( true ); 
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );

    if( true == m_bInviteAccept && SiKGCPartyManager()->GetInviteeUserInfo() )
    {
        SiKGCPartyManager()->JoinPartyToInvite( true );
    }
    else if( true == m_bInviteAccept && !SiKGCPartyManager()->GetInviteeUserInfo() )
    {
        SiKGCPartyManager()->JoinParty( true );
    }
    else if ( false == m_bInviteAccept && SiKGCPartyManager()->GetInviteeUserInfo() )
    {
        SiKGCPartyManager()->JoinPartyToInvite( false );
    }
    else if ( false == m_bInviteAccept )
    {
        SiKGCPartyManager()->JoinParty( false );
    }
    
}