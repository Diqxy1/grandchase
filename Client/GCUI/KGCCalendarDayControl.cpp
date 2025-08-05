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

            // 0        : ����
            // 1        : �����ð� �̴޷� ��ý�Ұ�
            // 2        : �޷� ���� �ε� ����
            // 3        : ������Ʈ �ҷ��� ��¥�� ���� ������ ��� ���� ����
            // 4        : ������Ʈ �ҷ��� ��¥�� ������ ��� ���� ����
            // 5        : �̹� �������ؼ� ��ý�Ұ�(�̷� ���� ����� �ϴµ�, �׽�Ʈ ȯ���� �ƴ϶��)
            // -1       : ����Ʈ�� ������ ����
            // -2       : ������ �ش�� ������ �̹� ������
            // -11����  : Ʈ����� ����
            // -99      : �� �� ���� ����
            switch( Result_SeallingCalendar )
            {
                case 0:
                    // �⼮ ������� ���������� ��ư�� ���ش�
                    m_pkCheckButton->ToggleRender( false );
                    g_pkUIScene->m_pkAttendanceCalendar->SetCalendar();
#ifdef CALENDAR_RECOMMAND
					g_pkUIScene->m_pkAttendanceInfoMsg->SetExtraValue2( 1 );
#endif
                    break;
                case 1:
                    // �⼮ ������ �Ϸ翡 1�ð� �̻� �÷��� �ؾ�_���� �� �ֽ��ϴ�.
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_NOT_YET_TIME_STAMP, "i", g_kGlobalValue.m_iAttendTime ),
                                            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                    break;
                case 2: // �޷� ���� �ε� ����
                    g_kGlobalValue.m_bExistCalendar = false;
                    g_pkUIScene->m_pkAttendanceCalendar->CloseCalendar();
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LOAD_CALENDAR_ERROR ) );
                    break;
                default:
                    // �⼮���� ��⿡ �����߽��ϴ�._���� �̵��� �ϰų� ������ �ٽ�_������� �ּ���.

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

    // ���� ǥ��
    
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

    //ST_TRI = 0x01, //'1�� 1�ð��̸�' (����)
    //ST_SQU = 0x02, //'1�� 1�ð��̻� �⼮üũ����' (�׸�)
    //ST_CIR = 0x04, //'1�� 1�ð��̻� �⼮üũ����' (���׶��)

    //ST_STA = 0x08, //'1�� �ְ��⼮�Ϸ�(����� ���)' (��)
    //ST_HEA = 0x10, //'1�� �����⼮�Ϸ�(�Ŵ޸��� ���)' (��Ʈ)
    //ST_LUC = 0x20, // ��Ű����

    // ��Ű����
    if( cSealType & ST_LUC )
    {
        m_pkStampLucky->ToggleRender( true );
		m_bChecked = true;
    }
    else if( cSealType & ST_TRI ) //'1�� 1�ð��̸�' (����)
    {
        m_pkStampTriangle->ToggleRender( true );
		m_bChecked = true;
    }
    else if( cSealType & ST_SQU ) //'1�� 1�ð��̻� �⼮üũ����' (�׸�)
    {
        m_pkCheckButton->ToggleRender( true );
		m_bChecked = true;
    }
    else if( cSealType & ST_CIR ) //'1�� 1�ð��̻� �⼮üũ����' (���׶��)
    {
        m_pkStampCheck->ToggleRender( true );
		m_bChecked = true;
    }
}
