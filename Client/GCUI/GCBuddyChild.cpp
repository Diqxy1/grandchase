#include "stdafx.h"
#include ".\GCBuddyChild.h"
#include <dbg/dbg.hpp>
#include "../KGCEmoticon.h"
#include "../BuddySystem/KFriendShip.h"


IMPLEMENT_CLASSNAME( GCBuddyChild );
IMPLEMENT_WND_FACTORY( GCBuddyChild );
IMPLEMENT_WND_FACTORY_NAME( GCBuddyChild, "gc_buddy_child" );

GCBuddyChild::GCBuddyChild(void)
: m_pkICon(NULL)
, m_pkName(NULL)
, m_bFirst(false)
, m_pkNoEmoticon(NULL)
, m_pkLocation(NULL)
, m_pkGuildGradeMaster(NULL)
, m_pkGuildGradeGeneral(NULL)
{
    LINK_CONTROL( "icon_group", m_pkICon );
    LINK_CONTROL( "static_name", m_pkName );
    LINK_CONTROL( "no_emoticon", m_pkNoEmoticon );
	LINK_CONTROL( "static_location", m_pkLocation );	

	//길드메신저
	LINK_CONTROL( "guild_grade_master", m_pkGuildGradeMaster );	
	LINK_CONTROL( "guild_grade_general", m_pkGuildGradeGeneral );	
    m_vPos[0] = D3DXVECTOR2(0.0f, 0.0f);
    m_vPos[1] = D3DXVECTOR2(0.0f, 0.0f);
}

GCBuddyChild::~GCBuddyChild(void)
{
}

void GCBuddyChild::OnCreate()
{
    m_pkICon->InitState( m_pkICon->IsRenderOn() );
    m_pkName->InitState( true );
    m_pkName->SetWordWrap( true );
    m_pkNoEmoticon->InitState( m_pkNoEmoticon->IsRenderOn() );
	m_pkLocation->InitState( m_pkLocation->IsRenderOn(), true, NULL );	
	m_pkLocation->SetAlign( DT_LEFT );
	m_pkLocation->SetWordWrap( true );

	//길드메신저
	m_pkGuildGradeMaster->InitState( false );
	m_pkGuildGradeGeneral->InitState( false );
}

void GCBuddyChild::SetData( const FRIEND_UI_DATA& kData, bool bChangePosition /*= true*/ )
{
    //bool m_bOpen;
    //DWORD m_dwEmoticon;
    m_pkICon->ToggleRender( kData.m_bGroup );
    m_pkICon->SetWndMode( kData.m_bOpen ? D3DWM_DEFAULT : D3DWM_HOVER );
    m_pkName->SetForceWordWrapText( kData.m_strDisplay, true );
    m_pkName->SetAlign( DT_VCENTER );

#if defined( NATION_LATINAMERICA )
    m_pkName->SetAutoScroll( true );
#endif

	if( kData.m_bOnline )
	{
		KBuddy buddy;
		if( SiKFriendShip()->GetFriendBuddyByUID( kData.m_dwUserUID, buddy ) )
		{
			std::wstring strLocation = buddy.GetServerName();
			strLocation += L"/";
			strLocation += buddy.GetLocation();
			m_pkLocation->ToggleRender( true );
			m_pkLocation->SetForceWordWrapText( strLocation, true );
			m_pkLocation->SetToolTip( strLocation, 0.f, 0.f, WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_LEFT );
            //m_pkName->SetToolTip(kData.m_strDisplay, 0.f, 0.f, WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_LEFT );
		}
	}
	
	if( false == kData.m_bOnline || true == kData.m_bGroup )
	{
		m_pkLocation->ToggleRender( false );
	}

    D3DCOLOR kFontColor;
    if( kData.m_bOnline )
    {
        kFontColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
    }
    else
    {
        kFontColor = D3DCOLOR_ARGB( 255, 180, 180, 180 );
    }
    m_pkName->SetFontColor( kFontColor );

    m_kUIData = kData;

	m_vPos[0].y = GetFixedWindowLocalPos().y;
	m_vPos[1].y = m_vPos[0].y;
	//m_vPos[1].x = EMOTICON_UI_SIZE - 18.0f;

	if( bChangePosition )
	{
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
}

void GCBuddyChild::OnCreateComplete()
{
    if( false == m_bFirst )
    {
        m_vPos[0] = GetFixedWindowLocalPos();
        m_vPos[1] = m_vPos[0];
        m_vPos[1].x += EMOTICON_UI_SIZE - 18.0f;
        m_bFirst = true;
    }
}

void GCBuddyChild::PostChildDraw()
{
    m_pkNoEmoticon->ToggleRender( false );
	if( m_kUIData.m_dwEmoticon == KFriend::DEFAULT_EMOTICON_ID || m_kUIData.m_bGroup == true || 
        m_kUIData.m_bUser == false || m_kUIData.m_bOnline == false )
    {
        if( m_kUIData.m_bGroup == false && -1 != m_kUIData.m_nGroupID )
        {
            m_pkNoEmoticon->ToggleRender( true );
        }
        return;
    }

    KGCEmoticon* pEmoticon = SiKGCEmoticonManager()->GetEmoticonData( m_kUIData.m_dwEmoticon / 10 );
    JIF( pEmoticon != NULL );

    D3DXVECTOR2 vPos = GetRelocatedWindowPos();
    pEmoticon->RenderForChatbox( vPos.x, vPos.y, vPos.x + EMOTICON_UI_SIZE * m_fWindowScaleX, vPos.y + EMOTICON_UI_SIZE * m_fWindowScaleY);
}


void GCBuddyChild::SetGuildGradeMark( char cGrade_ )
{
	switch(cGrade_)
	{
	case KNGuildUserInfo::GL_MASTER:
		m_pkGuildGradeMaster->ToggleRender(true);
		m_pkGuildGradeGeneral->ToggleRender(false);
		break;
	case KNGuildUserInfo::GL_GENERAL:
		m_pkGuildGradeMaster->ToggleRender(false);
		m_pkGuildGradeGeneral->ToggleRender(true);
		break;
	case KNGuildUserInfo::GL_NORMAL:
	default:
		m_pkGuildGradeMaster->ToggleRender(false);
		m_pkGuildGradeGeneral->ToggleRender(false);
	}
}

void GCBuddyChild::SetTextColor( DWORD dwColor_ )
{
	m_pkName->SetFontColor( dwColor_ );
	m_pkLocation->SetFontColor( dwColor_ );
}