#include "stdafx.h"
#include "KGCAlertWnd.h"

//
//
#include "KGCTitleWindow.h"

#include "GCNateOn.h"
#include "GCSKT.h"
#include "GCCoupleSystem.h"
#include "KGCRecommendList.h"

IMPLEMENT_CLASSNAME( KGCAlertWnd );
IMPLEMENT_WND_FACTORY( KGCAlertWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCAlertWnd, "gc_alert_wnd" );


KGCAlertWnd::KGCAlertWnd(void)
{
    m_pkAlert = NULL;
    LINK_CONTROL( "alert", m_pkAlert );

    // 우선 전부 요걸로 초기화 시켜놓고..
    for( int i = 0; i < NUM_ALERT; ++i )
		AlertCheckFunc[ DEFAULT ]       = &KGCAlertWnd::AlertCheck_True;

    m_eAlertButtonType = DEFAULT;
    AlertCheckFunc[ DEFAULT ]           = &KGCAlertWnd::AlertCheck_True;
    AlertCheckFunc[ MISSION ]           = &KGCAlertWnd::AlertCheck_Mission;
    AlertCheckFunc[ MSN ]               = &KGCAlertWnd::AlertCheck_MSN;
    AlertCheckFunc[ CALENDAR ]          = &KGCAlertWnd::AlertCheck_Calendar;
	AlertCheckFunc[ MYINFOSCENE ]		= &KGCAlertWnd::AlertCheck_MyInfoScene;
	AlertCheckFunc[ SKILLTREE ]			= &KGCAlertWnd::AlertCheck_SkillTree;
	AlertCheckFunc[ COUPLE ]			= &KGCAlertWnd::AlertCheck_CoupleWnd;
    AlertCheckFunc[ RECOMMEND ]         = &KGCAlertWnd::AlertCheck_Recommend;

    m_iToggleFrame = ALERT_TOGGLE_FRAME;
}

KGCAlertWnd::~KGCAlertWnd(void)
{
}

void KGCAlertWnd::OnCreate()
{
    m_pkAlert->InitState( false );
    m_dwTime = 0;
}

void KGCAlertWnd::FrameMoveInEnabledState()
{
    if( g_kGlobalValue.ServerInfo.CheckServerType(ST_ENTER_LIMIT) || g_kGlobalValue.ServerInfo.CheckServerType(ST_CREATE_LIMIT) )
        this->Lock( true );
    else
        this->Lock( false );


    if( IsLocked() == false && (this->*AlertCheckFunc[m_eAlertButtonType])() )
    {
        int iOpacity = ALERT_TOGGLE_FRAME;
        switch( m_iToggleFrame / ALERT_TOGGLE_FRAME )
        {
            case 0: // 0 ~ ( ALERT_TOGGLE_FRAME - 1 )
                iOpacity = m_iToggleFrame;
                break;
            case 1: // ( ALERT_TOGGLE_FRAME ) ~ ( ALERT_TOGGLE_FRAME * 2 - 1 )
                iOpacity = ALERT_TOGGLE_FRAME;
                break;
            case 2: // ( ALERT_TOGGLE_FRAME * 2 ) ~ ( ALERT_TOGGLE_FRAME * 3 - 1 )
                iOpacity = ALERT_TOGGLE_FRAME * 3 - m_iToggleFrame;
                break;
        }

        BYTE alpha = static_cast<BYTE>( 255.0f * static_cast<float>( iOpacity ) / static_cast<float>( ALERT_TOGGLE_FRAME ) );

        m_pkAlert->ToggleRender( true );
        m_pkAlert->SetWindowColor( D3DCOLOR_ARGB( alpha, 0xFF, 0xFF, 0xFF ) );
    }
    else
    {
        m_pkAlert->ToggleRender( false );
    }

    m_iToggleFrame = ( m_iToggleFrame + 1 ) % ( ALERT_TOGGLE_FRAME * 3 );

    switch ( m_eAlertButtonType )
    {
    case CALENDAR:
        {
            //////////////////////////////////////////////////////////////////////////
#ifdef CALENDAR_RECOMMAND
            // 현재 플레이 타임!
            int iPlayTime = g_kGlobalValue.m_iPlayTime * 60 + ( timeGetTime() - g_kGlobalValue.m_dwAccTimeNot ) / 1000;
            iPlayTime = iPlayTime / 60;

            // 켈린더 일 때, 1분 마다 툴팁 시간 갱신;
            if( timeGetTime() - m_dwTime > 6000 ) 
            {
				if ( iPlayTime < 70 )
					this->SetToolTip( g_pkStrLoader->GetReplacedString( STR_ID_ATTENDANCE_CALENDAR2, "i", g_kGlobalValue.m_iAttendTime - iPlayTime ), 0.0, 5.0f );
				else
					this->SetToolTip( g_pkStrLoader->GetString( STR_ID_ATTENDANCE_CALENDAR ) );

                m_dwTime = timeGetTime();
            }

            // 20분 마다 한번씩 1분동안 출석부 가이드 표시~
            if ( iPlayTime != 0 && iPlayTime % 20 == 0 )
            {
                g_pkUIScene->m_pkAttendanceInfoMsg->ToggleRender( true );
                g_pkUIScene->m_pkAttendanceInfoMsg->SetExtraValue1( iPlayTime );
                g_pkUIScene->m_pkAttendanceInfoMsg->UpdateMsg();
            }
            else
                g_pkUIScene->m_pkAttendanceInfoMsg->ToggleRender( false );
#endif
            //////////////////////////////////////////////////////////////////////////
        }
        break;
    default:
        break;
    }
}

void KGCAlertWnd::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
        case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); return;
    }
}

bool KGCAlertWnd::AlertCheck_True()
{
    return true;
}

bool KGCAlertWnd::AlertCheck_Mission()
{
    return ( (g_kGlobalValue.m_kUserMission.GetCompleteMissionNum() > 0)
			 || g_kGlobalValue.m_kUserMission.IsNewMission() );
}

bool KGCAlertWnd::AlertCheck_MSN()
{
    return g_pkUIScene->m_pkNateOn->IsNew();
}

bool KGCAlertWnd::AlertCheck_Calendar()
{
    return ( g_kGlobalValue.CheckCalendarAlert() );
}

bool KGCAlertWnd::AlertCheck_MyInfoScene()
{	
	return g_pkUIScene->m_pkTitleCollection->IsCompleteCTMissionAndNotGet();
}

bool KGCAlertWnd::AlertCheck_SkillTree()
{
	return ( SiGCSKT()->GetNewSkillNum() > 0 );
}

bool KGCAlertWnd::AlertCheck_CoupleWnd()
{
	return SiKGCCoupleSystem()->GetPartnerState() == KGCCoupleSystem::GCCS_COUPLE_WND;
}

bool KGCAlertWnd::AlertCheck_Recommend()
{
    return g_pkUIScene->m_pkRecommendList->IsNew();
}