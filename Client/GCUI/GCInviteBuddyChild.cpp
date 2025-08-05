#include "stdafx.h"
#include ".\gcinvitebuddychild.h"
//
//
//

IMPLEMENT_CLASSNAME( GCInviteBuddyChild );
IMPLEMENT_WND_FACTORY( GCInviteBuddyChild );
IMPLEMENT_WND_FACTORY_NAME( GCInviteBuddyChild, "gc_invite_buddy_child" );

GCInviteBuddyChild::GCInviteBuddyChild(void)
: m_pkICon(NULL)
, m_pkName(NULL)
, m_bFirst(false)
, m_pkCheck(NULL)
{
	LINK_CONTROL( "icon_group", m_pkICon );
	LINK_CONTROL( "static_name", m_pkName );
	LINK_CONTROL( "check", m_pkCheck );

	m_vPos[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_vPos[1] = D3DXVECTOR2(0.0f, 0.0f);
}

GCInviteBuddyChild::~GCInviteBuddyChild(void)
{
}

void GCInviteBuddyChild::OnCreate()
{
	m_pkICon->InitState( true );
	m_pkName->InitState( true );
	m_pkCheck->InitState( true, true, this );
	m_pkCheck->SetCheck( false );
}

void GCInviteBuddyChild::ActionPerformed( const KActionEvent& event )
{
	if( D3DWE_BUTTON_CLICK == event.m_dwCode )
	{
		if( event.m_pWnd == m_pkCheck )
		{
			m_pkCheck->SetCheck( !m_pkCheck->GetCheck() );
			SpeakToActionListener( KActionEvent( this, IBC_BUDDY_CHILD_CHECK
									, m_kUIData.m_dwUserUID, m_pkCheck->GetCheck() ) );
		}
	}
}

void GCInviteBuddyChild::SetData( const FRIEND_UI_DATA& kData )
{
	//bool m_bOpen;
	//DWORD m_dwEmoticon;
	m_pkICon->ToggleRender( kData.m_bGroup );
	m_pkICon->SetWndMode( kData.m_bOpen ? D3DWM_DEFAULT : D3DWM_HOVER );
	m_pkName->SetForceWordWrapText( kData.m_strDisplay, true );
	m_pkName->SetAlign( DT_VCENTER );

	m_kUIData = kData;

	D3DCOLOR kFontColor;
	if( kData.m_bOnline )
	{
		kFontColor = SiGCTextColorManager()->StringToColor(m_kUIData.m_strNickColor);
	}
	else
	{
		kFontColor = D3DCOLOR_ARGB( 255, 180, 180, 180 );
	}
	m_pkName->SetFontColor( kFontColor );

	m_vPos[0].y = GetFixedWindowLocalPos().y;
	m_vPos[1].y = m_vPos[0].y;
	//m_vPos[1].x = EMOTICON_UI_SIZE - 18.0f;

	if( true == m_kUIData.m_bGroup )
	{
		//SetAllWindowPos( m_vPos[0] );
		SetWindowPosDirect( m_vPos[0] );
	}
	else
	{
		//SetAllWindowPos( m_vPos[1] );
		SetWindowPosDirect( m_vPos[1] );
	}
}

void GCInviteBuddyChild::OnCreateComplete()
{
	if( false == m_bFirst )
	{
		m_vPos[0] = GetFixedWindowLocalPos();
		m_vPos[1] = m_vPos[0];
		m_vPos[1].x += EMOTICON_UI_SIZE - 18.0f;
		m_bFirst = true;
	}
}

void GCInviteBuddyChild::PostChildDraw()
{
	if( m_kUIData.m_bGroup == true
		|| m_kUIData.m_bUser == false )
	{
		m_pkCheck->ToggleRender( false );
	}
	else
	{
		m_pkCheck->ToggleRender( true );
	}

	if( m_kUIData.m_bGroup == true
		|| m_kUIData.m_bUser == false
		|| m_kUIData.m_bOnline == false )
	{
		m_pkCheck->SetSelfInputCheck( false );
	}
	else
	{
		m_pkCheck->SetSelfInputCheck( true );
	}

	//KGCEmoticon* pEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_kUIData.m_dwEmoticon / 10 );
	//JIF( pEmoticon != NULL );

	//D3DXVECTOR2 vPos = GetFixedWindowPos();
	//pEmoticon->RenderForChatbox( vPos.x, vPos.y, vPos.x + EMOTICON_UI_SIZE, vPos.y + EMOTICON_UI_SIZE );
}
