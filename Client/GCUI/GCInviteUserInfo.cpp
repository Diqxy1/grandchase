#include "stdafx.h"
#include ".\GCInviteUserInfo.h"
//
//
#include "GCCheckBox.h"
#include "KGCGradeMark.h"
//

#include "MyD3D.h"
#include "KGCChannelManager.h"

IMPLEMENT_CLASSNAME( KGCInviteUserInfo );
IMPLEMENT_WND_FACTORY( KGCInviteUserInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteUserInfo, "gc_invite_user_info" );

KGCInviteUserInfo::KGCInviteUserInfo(void)
: m_pkBack(NULL)
, m_pkUserInfo(NULL)
, m_pkCheck(NULL)
, m_pkGrade(NULL)
, m_bEmpty(true)
, m_bCheck(false)
, m_bDiffGrade(false)
, m_pkLock(NULL)
{
    LINK_CONTROL( "user_info_back",     m_pkBack );
    LINK_CONTROL( "user_info_static",   m_pkUserInfo );
    LINK_CONTROL( "user_info_check",    m_pkCheck );
    LINK_CONTROL( "grade_mark",         m_pkGrade );
    LINK_CONTROL( "user_info_lock",     m_pkLock );
}

KGCInviteUserInfo::~KGCInviteUserInfo(void)
{

}

void KGCInviteUserInfo::OnCreate( void )
{
    m_pkBack->InitState( true );
    m_pkUserInfo->InitState( true );
    m_pkUserInfo->SetText( L"" );
    m_pkCheck->InitState( true );
    m_pkGrade->InitState( false );
    m_pkLock->InitState( false );
}

void KGCInviteUserInfo::SetFriendUserInfo( const KInviteFriendUserInfo& kInfo )
{
	m_pkUserInfo->SetText( kInfo.m_strNickName );
	m_pkUserInfo->ToggleRender( true );
	m_pkGrade->SetGrade( (int)kInfo.m_cGrade );
	m_pkGrade->ToggleRender( true );
	SetToolTip(kInfo.m_strLocation);

	m_dwUserUID = kInfo.m_dwUserUID;
	m_bEmpty = false;

	if( kInfo.m_bInviteOK == false )
	{
			Lock(true);
			m_pkLock->ToggleRender( true );
	}
	else
	{
		Lock(false);
		m_pkLock->ToggleRender( false );
	}
}

void KGCInviteUserInfo::SetUserInfo( const KInviteUserInfo& kInfo )
{
    m_pkUserInfo->SetText( kInfo.m_strNickName );
    m_pkUserInfo->ToggleRender( true );
    m_pkGrade->SetGrade( (int)kInfo.m_cGrade );
    m_pkGrade->ToggleRender( true );

    m_dwUserUID = kInfo.m_dwUserUID;
    m_bEmpty = false;

	Lock(false);
    if( SiKGCChannelManager()->GetCurrentChannelType() != CT_UNLIMITED && 
        SiKGCChannelManager()->GetCurrentChannelType() != CT_DUNGEON )
    {
        int iMyGrade = (int)g_kGlobalValue.m_kUserInfo.GetCharacterGrade();
        EChannelType eMyType = g_MyD3D->GradeToChannelType( iMyGrade );
        EChannelType eUserType = g_MyD3D->GradeToChannelType( (int)kInfo.m_cGrade );
        m_bDiffGrade = (eMyType != eUserType);

        if( m_bDiffGrade == true )
        {
            m_pkLock->ToggleRender( true );
        }
    }
    else
    {
        m_bDiffGrade = false;
        m_pkLock->ToggleRender( false );
    }
}

void KGCInviteUserInfo::InitUserInfo()
{
    m_bEmpty = true;
    ClearToolTip();
    m_pkUserInfo->SetText( L"" );
    m_pkGrade->ToggleRender( false );
    SetCheck( false );
    SetBackMode( D3DWM_DEFAULT );
    m_bDiffGrade = false;
    m_pkLock->ToggleRender( false );
}

void KGCInviteUserInfo::SetBackMode( ED3DWndMode eMode )
{
    m_pkBack->SetWndMode( eMode, false );
}

void KGCInviteUserInfo::SetCheck( bool bCheck )
{
    m_bCheck = bCheck;
    m_pkCheck->SetCheck( m_bCheck );
}

KD3DWnd::ED3DWndMode KGCInviteUserInfo::GetBackMode() const
{
     return m_pkBack->GetWndMode();
}