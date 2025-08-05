#include "stdafx.h"
#include ".\KGCCalendarPerfectAttendanceControl.h"
#include "KGCAttendanceCalendar.h"
#include "KGCNumberControl.h"

#include "Procedure.h"
//
//
#include "MyD3D.h"

IMPLEMENT_CLASSNAME( KGCCalendarPerfectAttendanceControl );
IMPLEMENT_WND_FACTORY( KGCCalendarPerfectAttendanceControl );
IMPLEMENT_WND_FACTORY_NAME( KGCCalendarPerfectAttendanceControl, "gc_calendar_perfect_attendance_control" );

KGCCalendarPerfectAttendanceControl::KGCCalendarPerfectAttendanceControl(void)
{
	m_pkPerfectAttendanceBack				= NULL;
	m_pkPerfectAttendanceTitle				= NULL;

	m_pkPerfectAttendanceMark			= NULL;
	m_pkPerfectAttendanceCheck			= NULL;

	m_bChecked					= false;

	LINK_CONTROL( "perfect_attendance_back", m_pkPerfectAttendanceBack );
	LINK_CONTROL( "perfect_attendance_static", m_pkPerfectAttendanceTitle );
	LINK_CONTROL( "perfect_attendance_check_button", m_pkPerfectAttendanceCheck );
	LINK_CONTROL( "perfect_attendance_mark", m_pkPerfectAttendanceMark );
}

KGCCalendarPerfectAttendanceControl::~KGCCalendarPerfectAttendanceControl(void)
{
}

void KGCCalendarPerfectAttendanceControl::OnCreate( void )
{
	m_pkPerfectAttendanceTitle->InitState(true);
	m_pkPerfectAttendanceTitle->SetWindowColor( 0xFFFFFFFF );

	m_pkPerfectAttendanceMark->InitState( false );
	m_pkPerfectAttendanceCheck->InitState( false, true, this );
}

void KGCCalendarPerfectAttendanceControl::AllRenderOff()
{
	m_pkPerfectAttendanceBack->ToggleRender( false );
	m_pkPerfectAttendanceTitle->ToggleRender( false );

	m_pkPerfectAttendanceMark->ToggleRender( false );
	m_pkPerfectAttendanceCheck->ToggleRender( false );
}

void KGCCalendarPerfectAttendanceControl::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkPerfectAttendanceCheck && m_pkPerfectAttendanceCheck->IsRenderOn() == true)
		{
			g_KDSound.Play( "31" );

			if( !g_pItemMgr->GetPerfectWeekItemList().empty() )
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PERFECT_ATTENDANCE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_iWeek + 1, 0, false, true );
			}
			else
			{
				Result_WeeklyAttend = INT_MAX;
				KP2P::GetInstance()->Send_WeeklyAttendReq( m_iWeek+ 1 );
				g_MyD3D->WaitForServerAck( Result_WeeklyAttend, INT_MAX , 7000, 9999 );

				// 0 - OK
				// 1 - 달력에 해당 주 출석 정보가 없다
				// 2 - 해당 주의 출첵이 주개근 조건에 못미침
				// 3 - 이미 주개근 했음
				// 4 - 출석체크 할 수 있는 날 존재
				// 5 - 오늘 누적시간 한시간 미만
				// 6 - 달력 정보 로드되지 않음 
				switch( Result_WeeklyAttend )
				{
				case 0:
					m_bChecked = false;
					m_pkPerfectAttendanceCheck->ToggleRender(m_bChecked);
					m_pkPerfectAttendanceMark->ToggleRender(true);
					break;
				case 2:
				case 4:
					// 아직 출석체크를 하지 않은 날이 있습니다._출석체크를 먼저 해주세요.
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_NEED_ATTENDANCE ),
						L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
					break;
				case 3:
					// 이미 개근을 했습니다.
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ALREADY_PERFECT_WEEK ),
						L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
					break;
				case 5:
					// 오늘의 플레이 시간이 1시간 미만입니다.
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_NOT_YET_TIME, "i", g_kGlobalValue.m_iAttendTime ),
						L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
					break;
				case 6: // 달력 정보 로드되지 않음
					g_kGlobalValue.m_bExistCalendar = false;
					g_pkUIScene->m_pkAttendanceCalendar->CloseCalendar();
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LOAD_CALENDAR_ERROR ) );
					break;
				case 1:
				default:
					{
						std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KUserEvent::EVENT_WEEKLY_ATTEND_ACK, Result_WeeklyAttend );	
						// 개근 보너스를 받는데 실패했습니다._서버 이동을 하거나 게임을 다시_실행시켜 주세요.
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_PERFECT_WEEK_FAIL ),
							strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
					}
					break;
				}
			}
		}
	}
}

void KGCCalendarPerfectAttendanceControl::SetWeekDay( int iNum )
{
	m_iWeek = iNum;
	AllRenderOff();

	// 오늘 표시
	m_pkPerfectAttendanceBack->ToggleRender( true );

	//day
	m_pkPerfectAttendanceTitle->ToggleRender(true);
	m_pkPerfectAttendanceTitle->SetText(g_pkStrLoader->GetReplacedString(STR_ID_VIP_NTH_WEEK, "i",  m_iWeek + 1 ));
}

void KGCCalendarPerfectAttendanceControl::SetStamp()
{
	m_pkPerfectAttendanceMark->ToggleRender(true);
}

void KGCCalendarPerfectAttendanceControl::Check()
{
	m_bChecked = true;
	m_pkPerfectAttendanceCheck->ToggleRender(m_bChecked);
}