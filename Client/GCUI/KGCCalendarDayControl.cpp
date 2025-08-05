#include "stdafx.h"
#include ".\KGCCalendarDayControl.h"
#include "KGCAttendanceCalendar.h"
#include "KGCNumberControl.h"

#include "Procedure.h"
//
//
#include "MyD3D.h"
#include "KGCAlertWnd.h"

IMPLEMENT_CLASSNAME( KGCCalendarDayControl );
IMPLEMENT_WND_FACTORY( KGCCalendarDayControl );
IMPLEMENT_WND_FACTORY_NAME( KGCCalendarDayControl, "gc_calendar_day_control" );

KGCCalendarDayControl::KGCCalendarDayControl(void)
{
    m_pkDayBack					= NULL;
	m_pkDayNum				= NULL;

    m_pkStampCheck			= NULL;
    m_pkStampTriangle			= NULL;
    m_pkStampNoCheck       = NULL;
    m_pkStampLucky			= NULL;

    m_pkCheckButton			= NULL;
	m_bChecked					= false;
    m_bToday						= false;

    LINK_CONTROL( "day_back", m_pkDayBack );
    
	LINK_CONTROL( "day_num", m_pkDayNum );

    LINK_CONTROL( "stamp_check", m_pkStampCheck );
    LINK_CONTROL( "stamp_triangular", m_pkStampTriangle );
    LINK_CONTROL( "stamp_x", m_pkStampNoCheck );
    LINK_CONTROL( "stamp_lucky", m_pkStampLucky );
    LINK_CONTROL( "check_button", m_pkCheckButton );
}

KGCCalendarDayControl::~KGCCalendarDayControl(void)
{
}

void KGCCalendarDayControl::OnCreate( void )
{
	m_pkDayNum->InitState(true);
	m_pkDayNum->SetAlign(DT_CENTER);

    m_pkStampCheck->InitState( false );
    m_pkStampTriangle->InitState( false );
    m_pkStampNoCheck->InitState( false );
    m_pkStampLucky->InitState( false );

    m_pkStampCheck->SetWindowColor( 0xCCFFFFFF );
    m_pkStampTriangle->SetWindowColor( 0xCCFFFFFF );
    m_pkStampNoCheck->SetWindowColor( 0xCCFFFFFF );
    m_pkStampLucky->SetWindowColor( 0xCCFFFFFF );

    m_pkCheckButton->InitState( false, true, this );
}

void KGCCalendarDayControl::AllRenderOff()
{
	m_pkDayBack->ToggleRender( false );
    m_pkDayNum->ToggleRender( false );

    m_pkStampCheck->ToggleRender( false );
    m_pkStampTriangle->ToggleRender( false );
    m_pkStampNoCheck->ToggleRender( false );
    m_pkStampLucky->ToggleRender( false );

    m_pkCheckButton->ToggleRender( false );
}

void KGCCalendarDayControl::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER )
    g_KDSound.Play( "30" );

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkCheckButton )
        {
            g_KDSound.Play( "31" );

            KSimpleDate kDate = g_kGlobalValue.m_kToday;
            kDate.m_cDay = (char)m_iToday;

            Result_SeallingCalendar = INT_MAX;
            KP2P::GetInstance()->Send_SeallingCalendarReq( kDate );
            g_MyD3D->WaitForServerAck( Result_SeallingCalendar, INT_MAX , 7000, 9999 );

            // 0        : 성공
            // 1        : 누적시간 미달로 출첵불가
            // 2        : 달력 정보 로드 실패
            // 3        : 업데이트 할려는 날짜의 주차 정보를 얻어 낼수 없음
            // 4        : 업데이트 할려는 날짜의 정보를 얻어 낼수 없음
            // 5        : 이미 월개근해서 출첵불가(이런 경우는 없어야 하는데, 테스트 환경이 아니라면)
            // -1       : 포인트백 정보가 없음
            // -2       : 유저의 해당월 정보가 이미 존재함
            // -11이하  : 트랜잭션 에러
            // -99      : 알 수 없는 에러
            switch( Result_SeallingCalendar )
            {
                case 0:
                    // 출석 도장찍기 성공했으니 버튼은 꺼준다
                    m_pkCheckButton->ToggleRender( false );
                    g_pkUIScene->m_pkAttendanceCalendar->SetCalendar();
#ifdef CALENDAR_RECOMMAND
					g_pkUIScene->m_pkAttendanceInfoMsg->SetExtraValue2( 1 );
#endif
                    break;
                case 1:
                    // 출석 도장은 하루에 1시간 이상 플레이 해야_찍을 수 있습니다.
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_NOT_YET_TIME_STAMP, "i", g_kGlobalValue.m_iAttendTime ),
                                            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                    break;
                case 2: // 달력 정보 로드 실패
                    g_kGlobalValue.m_bExistCalendar = false;
                    g_pkUIScene->m_pkAttendanceCalendar->CloseCalendar();
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LOAD_CALENDAR_ERROR ) );
                    break;
                default:
                    // 출석도장 찍기에 실패했습니다._서버 이동을 하거나 게임을 다시_실행시켜 주세요.

                    std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                        KUserEvent::EVENT_SEALLING_CALENDAR_ACK, Result_SeallingCalendar );	
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_GET_STAMP_FAIL ),
                        strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                    break;
            }
        }
    }
}

void KGCCalendarDayControl::SetDay( int iNum, int iState, bool bNoCheckDay )
{
    AllRenderOff();

    m_iToday = iNum;

    // 오늘 표시
    
	m_pkDayBack->ToggleRender( true );
	
	switch(iState)
	{
	case DAY_TODAY:
		m_bToday = true;
		m_pkDayBack->SetWndMode(D3DWM_SELECT);
		break;
	case DAY_MONTH:
		m_pkDayBack->SetWndMode(D3DWM_HOVER);
		break;
	}

	//stamp
    m_pkStampNoCheck->ToggleRender( bNoCheckDay );

	//day
	m_pkDayNum->ToggleRender(true);
	m_pkDayNum->SetText(iNum);
}

void KGCCalendarDayControl::SetStampType( char cSealType )
{
    m_pkStampNoCheck->ToggleRender( false );

    //ST_TRI = 0x01, //'1일 1시간미만' (세모)
    //ST_SQU = 0x02, //'1일 1시간이상 출석체크안함' (네모)
    //ST_CIR = 0x04, //'1일 1시간이상 출석체크했음' (동그라미)

    //ST_STA = 0x08, //'1주 주간출석완료(토요일 결산)' (별)
    //ST_HEA = 0x10, //'1달 월간출석완료(매달말일 결산)' (하트)
    //ST_LUC = 0x20, // 럭키데이

    // 럭키데이
    if( cSealType & ST_LUC )
    {
        m_pkStampLucky->ToggleRender( true );
		m_bChecked = true;
    }
    else if( cSealType & ST_TRI ) //'1일 1시간미만' (세모)
    {
        m_pkStampTriangle->ToggleRender( true );
		m_bChecked = true;
    }
    else if( cSealType & ST_SQU ) //'1일 1시간이상 출석체크안함' (네모)
    {
        m_pkCheckButton->ToggleRender( true );
		m_bChecked = true;
    }
    else if( cSealType & ST_CIR ) //'1일 1시간이상 출석체크했음' (동그라미)
    {
        m_pkStampCheck->ToggleRender( true );
		m_bChecked = true;
    }
}
