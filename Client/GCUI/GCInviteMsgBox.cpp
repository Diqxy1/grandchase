#include "stdafx.h"
//
#include ".\GCInviteMsgBox.h"
#include "GCInviteInfo.h"
#include "KGCInviteManager.h"
#include "GCCheckBox.h"
#include "GCBarControl.h"
//

#include "MyD3D.h"
#include "GCStateMachine.h"
#include "Procedure.h"
//

IMPLEMENT_CLASSNAME( KGCInviteMsgBox );
IMPLEMENT_WND_FACTORY( KGCInviteMsgBox );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteMsgBox, "gc_invite_msg_box" );

KGCInviteMsgBox::KGCInviteMsgBox(void)
: m_pkInviteInfo(NULL)
, m_pkTitle(NULL)
, m_pkDenyReserve(NULL)
, m_pkInvite(NULL)
, m_pkCheck(NULL)
, m_pkBar(NULL)
, m_pkYes(NULL)
, m_pkNo(NULL)
, m_bInviteAccept(false)
{
    LINK_CONTROL( "invite_info",            m_pkInviteInfo );
    LINK_CONTROL( "static_deny_reserve",    m_pkDenyReserve );
    LINK_CONTROL( "static_invite_message",  m_pkInvite );
    LINK_CONTROL( "deny_reserve_check",     m_pkCheck );
    LINK_CONTROL( "bar",                    m_pkBar );
    LINK_CONTROL( "btn_yes",                m_pkYes );
    LINK_CONTROL( "btn_no",                 m_pkNo );
}

KGCInviteMsgBox::~KGCInviteMsgBox(void)
{

}

void KGCInviteMsgBox::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCheck, D3DWE_BUTTON_CLICK, OnCheckBox );
    GCWND_MSG_MAP( m_pkYes, D3DWE_BUTTON_CLICK, OnClickYes );
    GCWND_MSG_MAP( m_pkNo, D3DWE_BUTTON_CLICK, OnClickNo );
}

void KGCInviteMsgBox::OnCreate()
{
    m_pkInviteInfo->InitState( true );
    KD3DWnd* pkTitleParent = GetChildByName( "invite_title" );
    ASSERT( pkTitleParent != NULL );
    pkTitleParent->InitState( true );
    m_pkTitle = static_cast<KD3DStatic*>( pkTitleParent->GetChildByName( "title" ) );
    ASSERT( m_pkTitle != NULL );
    m_pkTitle->InitState( true );
    m_pkDenyReserve->InitState( true );
    m_pkInvite->InitState( true );
    m_pkCheck->InitState( true, true, this );
    m_pkBar->InitState( true );
    m_pkYes->InitState( true, true, this );
    m_pkNo->InitState( true, true, this );
    m_pkNo->SetHotKey( DIK_ESCAPE );
    m_pkYes->SetHotKey( DIK_RETURN );
    m_pkYes->SetHotKey( DIK_NUMPADENTER );
}

void KGCInviteMsgBox::SetInviteInfo()
{
    const KInviteInfo& kInfo = SiKGCInviteManager()->GetInviteInfo();
    m_pkInviteInfo->SetInviteInfo( kInfo );
    std::wstring strInvite = g_pkStrLoader->GetReplacedString( STR_ID_INVITE_MESSAGE, "L", kInfo.m_strNickName );
    m_pkInvite->SetTextAutoMultiline( strInvite );
}

void KGCInviteMsgBox::OnCreateComplete()
{
    m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_INVITE_MESSAGE_TITLE ) );
    m_pkTitle->SetAlign( DT_CENTER );
    m_pkDenyReserve->SetText( g_pkStrLoader->GetString( STR_ID_INVITE_DENY_RESERVE ) );
    m_pkDenyReserve->SetFontColor( D3DCOLOR_ARGB( 255, 0, 0, 180) );

    m_pkCheck->SetCheck( SiKGCInviteManager()->IsDenyInviteMsg() );
    m_pkBar->SetGage( 0.0f );
}

void KGCInviteMsgBox::OnCheckBox()
{
    m_pkCheck->ToggleCheck();
}

void KGCInviteMsgBox::FrameMoveInEnabledState()
{
    // gage
    DWORD dwTimeDiff = ::timeGetTime() - SiKGCInviteManager()->GetOpenTime();
    float fPercent = static_cast<float>( dwTimeDiff ) / static_cast<float>( INVITE_BOX_WAIT_TIME );
    m_pkBar->SetGage( fPercent );

    if( dwTimeDiff >= INVITE_BOX_WAIT_TIME )
    {
        OnClickNo();
    }
}

void KGCInviteMsgBox::OnClickYes()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    m_bInviteAccept = true;
	SiKGCInviteManager()->m_bInviteAccept = true;
}

void KGCInviteMsgBox::OnClickNo()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
    m_bInviteAccept = false;
}

void KGCInviteMsgBox::OnDestroyComplete()
{
    SiKGCInviteManager()->SetDenyInviteMsg( m_pkCheck->GetCheck() );
    g_MyD3D->m_KGCOption.SetDenyInviteMsg( m_pkCheck->GetCheck() );

    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
    SiKGCInviteManager()->SetMessageBoxOpen( false );
    if( true == m_bInviteAccept )
    {
        SiKGCInviteManager()->JoinToInviteRoom();
    }

    SiKGCInviteManager()->PopInviteInfo();
    if( true == SiKGCInviteManager()->IsDenyInviteMsg() )
    {
        SiKGCInviteManager()->ClearInviteList();
    }
}

bool KGCInviteMsgBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    SetInviteInfo();
    return true;
}