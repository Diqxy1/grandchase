#include "stdafx.h"
#include ".\KGCAttendanceCalendar.h"

//

//
#include "KGCNumberControl.h"
#include "KGCCalendarDayControl.h"
#include "KGCCalendarPerfectAttendanceControl.h"
//
//
#include "Procedure.h"
#include "MyD3D.h"
//
#include "KGCAlertWnd.h"
#include "gcui/KGCWorldMapContainer.h"
//

IMPLEMENT_CLASSNAME(KGCAttendanceCalendar);
IMPLEMENT_WND_FACTORY(KGCAttendanceCalendar);
IMPLEMENT_WND_FACTORY_NAME(KGCAttendanceCalendar, "gc_attendance_calendar");

KGCAttendanceCalendar::KGCAttendanceCalendar(void)
    : m_iGetGCPoint(0),
    m_iPlayTime(0)
{
    m_pkExit = nullptr;
    m_pkMonthCheckButton = nullptr;
    m_pkNumYear = nullptr;
    m_pkNumMonth = nullptr;
    m_pkMonthCheckMark = nullptr;

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            m_pkDay[i][j] = nullptr;
        }
    }

    for (int i = 0; i < 7; i++)
    {
        m_apkWeekText[i] = nullptr;
    }

    for (int i = 0; i < 6; i++)
    {
        m_apkPerfectAttendanceWeek[i] = nullptr;
    }

    m_pkPlayTimeNum = nullptr;
    m_pkPlayTimeText = nullptr;
    m_pkPoint = nullptr;

    LINK_CONTROL("exit_button", m_pkExit);
    LINK_CONTROL("month_all_check_button", m_pkMonthCheckButton);

    LINK_CONTROL("num_month", m_pkNumMonth);
    LINK_CONTROL("num_year", m_pkNumYear);

    LINK_CONTROL("month_check", m_pkMonthCheckMark);

    LINK_CONTROL("playtime_static", m_pkPlayTimeNum);
    LINK_CONTROL("playtime_text", m_pkPlayTimeText);
    LINK_CONTROL("perfect_attendance_title", m_pkPerfectAttendanceTitle);

    LINK_CONTROL("point_static", m_pkPoint);
    LINK_CONTROL("text_month", m_pkTextMonth);

    for (int i = 0; i < 12; i++)
    {
        LINK_CONTROL_STM("month_static" << (i + 1), m_pkMonthStatics[i]);
    }

    for (int i = 0; i < 7; i++)
    {
        LINK_CONTROL_STM("days_" << i, m_apkWeekText[i]);
    }
}

KGCAttendanceCalendar::~KGCAttendanceCalendar(void)
{
}

void KGCAttendanceCalendar::OnCreate(void)
{
    m_pkExit->InitState(true, true, this);

    m_pkMonthCheckButton->InitState(true, true, this);
    m_pkMonthCheckButton->Lock(true);

    m_pkNumYear->InitState(true);
    m_pkNumYear->SetStringType("Attendance_small");

    m_pkNumMonth->InitState(true);
    m_pkNumMonth->SetStringType("Attendance_big");

    for (int i = 0; i < 12; i++)
    {
        m_pkMonthStatics[i]->InitState(false);
    }

    m_pkTextMonth->InitState(true);
    m_pkMonthCheckMark->InitState(false);

    m_pkPlayTimeNum->InitState(true);
    m_pkPlayTimeText->InitState(true);
    m_pkPoint->InitState(true);

    m_pkPlayTimeNum->SetFontColor(0xFFFFFFFF);
    m_pkPlayTimeText->SetFontColor(0xFFFFFFFF);
    m_pkPerfectAttendanceTitle->SetFontColor(0xFFFFFF00);

    m_pkPoint->SetFontColor(0xFFFFFFFF);

    m_pkPerfectAttendanceTitle->SetAlign(DT_CENTER);
    m_pkPlayTimeNum->SetAlign(DT_CENTER);
    m_pkPlayTimeText->SetAlign(DT_CENTER);
    m_pkPoint->SetAlign(DT_RIGHT);

    m_pkExit->SetHotKey(DIK_ESCAPE);

    for (int i = 0; i < 7; i++)
    {
        m_apkWeekText[i]->InitState(true);
        m_apkWeekText[i]->ToggleRender(true);
        m_apkWeekText[i]->SetText(g_pkStrLoader->GetString(STR_ID_ATTENDANCE_CALENDAR_WEEK + i));
    }

    m_pkPlayTimeText->ToggleRender(true);
    m_pkPlayTimeText->SetText(g_pkStrLoader->GetString(STR_ID_ATTENDANCE_CALENDAR_TIME));

    m_pkPerfectAttendanceTitle->ToggleRender(true);
    m_pkPerfectAttendanceTitle->SetText(g_pkStrLoader->GetString(STR_ID_ATTENDANCE_CALENDAR_PRIZE));
}

#define TOGGLEFRAME 25

void KGCAttendanceCalendar::SetCalendar()
{
    // ��� ǥ��
    int iYear = g_kGlobalValue.m_kToday.m_sYear;
    int iMonth = g_kGlobalValue.m_kToday.m_cMonth;

    m_pkNumYear->SetNumber(iYear);
    m_pkNumYear->ToggleRender(iYear != 0);
    m_pkNumYear->SetWriteAlign(ALIGN_CENTER_SIDE);
    m_pkNumYear->SetScale(0.7f);
    m_pkNumYear->SetShadow(false);

    //�⼮�� ��¥ ������ ǥ��
    m_pkNumMonth->SetNumber(iMonth);
    m_pkNumMonth->ToggleRender(iMonth != 0);
    m_pkNumMonth->SetWriteAlign(ALIGN_RIGHT_SIDE);
    //m_pkNumMonth->SetInterval( 22.f );
    m_pkNumMonth->SetScale(1.3f);
    m_pkNumMonth->SetShadow(false);

    // GC����Ʈ ǥ��
    WCHAR strTemp[20];
    swprintf(strTemp, 20, L"%d", (int)g_kGlobalValue.m_iGCPoint);
    m_pkPoint->SetText(strTemp);

    // �÷���Ÿ�� ǥ��
    m_iPlayTime = g_kGlobalValue.m_iPlayTime * 60 * 1000;
    SetPlayTime();

    // ��¥�� ǥ��
    //SetDay();

    // ���� / �⼮��ư ǥ��
    SetStamp();

    // �� �������� ���� ���� �� �ִ� ���� �������� �ִ°�?
    bool bPerfectAttendanceReward = g_kGlobalValue.m_vecReservedReward.size() > 0;
    m_pkMonthCheckButton->Lock(!bPerfectAttendanceReward);

    m_iAlertToggle = TOGGLEFRAME;

    // UI�� ������Ʈ
    if (!g_pkUIScene->m_pkGCGuide->IsRenderOn()) {
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        g_pkUIScene->m_pkCashShopScene->UpdateUserInfo();
#ifndef DISABLE_MAP_SHOP // Kawan>
        g_pkUIScene->m_pkGPShopScene->UpdateUserInfo();
#endif
    }
}

void KGCAttendanceCalendar::ActionPerformed(const KActionEvent& event)
{
    if (event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER)
        g_KDSound.Play("30");

    if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        if (event.m_pWnd == m_pkExit)
        {
            g_KDSound.Play("31");
            SpeakToActionListener(KActionEvent(this, EXIT));
        }
        else if (event.m_pWnd == m_pkMonthCheckButton) // �� ���ٻ� ��ư
        {
            g_KDSound.Play("31");
            SpeakToActionListener(KActionEvent(this, PERFECT_ATTENDANCE));
        }
    }
}

void KGCAttendanceCalendar::FrameMoveInEnabledState()
{
    if (m_iGetGCPoint > 0)
    {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_FIRST_OPEN_CALENDAR_GET_POINT, "i", m_iGetGCPoint ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        m_iGetGCPoint = 0;
    }

    SetPlayTime();
}

void KGCAttendanceCalendar::OnDestroyComplete(void)
{
    SpeakToActionListener(KActionEvent(this, DESTROYED));
    if (g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP)
        g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap();
}

void KGCAttendanceCalendar::SetDay()
{
    int iDay = g_kGlobalValue.m_kFirstDay.m_cDay;
    int iWeekDay = g_kGlobalValue.m_kFirstDay.m_cWeekDay;
    int iWeekCount = 0;

    std::vector<KD3DWnd*> vecDays = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_attendance_calendar.stg", "", "attendance_calendar\\day_box", true, 42);

    for (unsigned int i = 0; i < vecDays.size(); i++)
    {
        if (m_pkDay[i / 7][i % 7])
        {
            m_pkDay[i / 7][i % 7]->ToggleRender(false);
        }

        vecDays[i]->InitState(false, false, this);
        float posX = (float)((i % 7) * vecDays[i]->GetWidth());
        float posY = (float)((i / 7) * vecDays[i]->GetHeight());

        if ((i % 7) != 0)
        {
            posX += (i % 7);
        }

        if (posY > 0.0f)
        {
            posY += (float)((int)(i / 7));
        }

        vecDays[i]->SetWindowPosX((float)(posX + 35));
        vecDays[i]->SetWindowPosY((float)(posY + 133));

        m_pkDay[i / 7][i % 7] = (KGCCalendarDayControl*)vecDays[i];
    }

    while (iDay <= g_kGlobalValue.m_kLastDay.m_cDay)
    {
        // �켱 �״��� ��¥�� �����Ѵ�
        m_pkDay[iWeekCount][iWeekDay]->ToggleRender(true);
        m_pkDay[iWeekCount][iWeekDay]->SetDay(iDay, iDay == g_kGlobalValue.m_kToday.m_cDay ? KGCCalendarDayControl::DAY_TODAY : KGCCalendarDayControl::DAY_MONTH, iDay < g_kGlobalValue.m_kToday.m_cDay);

        ++iWeekDay;
        ++iDay;
        if (iWeekDay == 7)
        {
            iWeekDay = 0;
            ++iWeekCount;
        }
    }

    // �̹����� ù��° ���� �Ͽ����� �ƴϸ� �������� ��ĥ�� ǥ��������Ѵ�
    // m_cWeekDay - �Ͽ��� 0, ������ 1, ...
    iDay = g_kGlobalValue.m_kLastDayOfLastMonth.m_cDay;
    if (g_kGlobalValue.m_kFirstDay.m_cWeekDay != 0)
    {
        for (int i = g_kGlobalValue.m_kLastDayOfLastMonth.m_cWeekDay; i >= 0; --i)
        {
            m_pkDay[0][i]->ToggleRender(true);
            m_pkDay[0][i]->SetDay(iDay, KGCCalendarDayControl::DAY_DEFAULT, true);
            --iDay;
        }
    }

    std::vector<KD3DWnd*> vecWeek = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_attendance_calendar.stg", "", "attendance_calendar\\perfect_attendance_box", true, 6);

    int index = 0;
    for (auto week : vecWeek)
    {
        week->InitState(true, false, this);
        week->SetWindowPosY((float)(index * week->GetHeight() + 133 + index));
        KGCCalendarPerfectAttendanceControl* control = (KGCCalendarPerfectAttendanceControl*)week;
        control->SetWeekDay(index);
        m_apkPerfectAttendanceWeek[index] = control;
        index++;
    }
}

void KGCAttendanceCalendar::SetStamp()
{
    m_pkMonthCheckMark->ToggleRender(false);

    // ���� ������ �����͸� ���� ���� ������ ǥ���� �ش�
    std::map<char, std::vector<KDailyInfo> >::iterator mit;

    // ���� ���� ���캻��
    for (mit = g_kGlobalValue.m_mapCalendar.begin(); mit != g_kGlobalValue.m_mapCalendar.end(); ++mit)
    {
        int iWeek = mit->first - 1;
        int iWeekStampCount = 0;

        // �ٿ�� üũ
        if (iWeek < 0)
            iWeek = 0;
        else if (iWeek > 5)
            iWeek = 5;

        std::vector<KDailyInfo> vecWeekInfo = mit->second;

        for (int i = 0; i < (int)vecWeekInfo.size(); ++i)
        {
            KDailyInfo kDaylyInfo = vecWeekInfo[i];

            if (!(kDaylyInfo.m_kDate == g_kGlobalValue.m_kToday && kDaylyInfo.m_cSealType == ST_TRI))
                m_pkDay[iWeek][kDaylyInfo.m_kDate.m_cWeekDay]->SetStampType(kDaylyInfo.m_cSealType);

            // �� �������� üũ
            if (kDaylyInfo.m_cSealType & ST_STA)
            {
                m_apkPerfectAttendanceWeek[iWeek]->SetStamp();
            }
            // �� �������� üũ
            else if (kDaylyInfo.m_cSealType & ST_HEA)
            {
                KSimpleDate kThisMonth;
                kThisMonth = g_kGlobalValue.m_kToday;
                kThisMonth.m_cDay = 1;

                // �̹����� ù��°������ �����̶�� ������ �����̹Ƿ� ������ üũ�� ���� �ʴ´�
                if (!(kDaylyInfo.m_kDate < kThisMonth))
                    m_pkMonthCheckMark->ToggleRender(true);
                m_pkMonthCheckMark->SetOrder_Top();
            }

            // ������ ������ ���� üũ�� ������ ī��Ʈ�� �ؼ� 7���̸�..
            // �� ������ ���� �� �ֵ��� �Ѵ�
            if ((!(kDaylyInfo.m_kDate == g_kGlobalValue.m_kToday) &&
                /*( ( kDaylyInfo.m_cSealType & ST_TRI ) && ( ( kDaylyInfo.m_cSealType & ST_STA ) == 0 ) ) ||*/
                ((kDaylyInfo.m_cSealType & ST_CIR) && ((kDaylyInfo.m_cSealType & ST_STA) == 0))))
            {
                ++iWeekStampCount;
            }
        }

        // ������ �������� �ϰ��� ��Ҵٸ�!!
        if (iWeekStampCount == 7)
        {
            if (m_apkPerfectAttendanceWeek[iWeek])
                m_apkPerfectAttendanceWeek[iWeek]->Check();
        }
    }
}

void KGCAttendanceCalendar::SetPlayTime()
{
    // ���ʸ��� �ð��� �������ش�
    // �̳��� �� ���� ��..
    int iPlayTime = g_kGlobalValue.m_iPlayTime * 60 + (timeGetTime() - g_kGlobalValue.m_dwAccTimeNot) / 1000;

    // ǥ�õǾ� �ִ� �÷��� Ÿ�Ӱ� ���� �÷��� Ÿ���� �ٸ��� ����
    if (m_iPlayTime != iPlayTime)
    {
        m_iPlayTime = iPlayTime;

        WCHAR strTemp[20];
        int hours = iPlayTime / 3600;
        iPlayTime = iPlayTime % 3600;
        int minutes = iPlayTime / 60;
        int seconds = iPlayTime % 60;

        swprintf(strTemp, 20, L"%02d : %02d : %02d", hours, minutes, seconds);
        m_pkPlayTimeNum->SetText(strTemp);
    }
}

void KGCAttendanceCalendar::CloseCalendar()
{
    SpeakToActionListener(KActionEvent(this, EXIT));
}

bool KGCAttendanceCalendar::InitDialog(IN const KDialogInfo& kInfo_)
{
    this->SetDay();
    this->SetCalendar();
    return true;
}

bool KGCAttendanceCalendar::CheckCondition(IN const KDialogInfo& kInfo_)
{
    if (SiKFriendShip()->GetMyRecommendState() == 1 || SiKFriendShip()->GetMyRecommendState() == 2)
    {
        Result_recommendFullINfo = INT_MAX;
        KP2P::GetInstance()->Send_RecommendFullInfoReq();
        g_MyD3D->WaitForServerAck(Result_recommendFullINfo, INT_MAX, 3000, 2);
    }

    return true;
}

void KGCAttendanceCalendar::OnCreateComplete()
{
    if (m_iGetGCPoint > 0)
    {
        //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetReplacedString( STR_ID_FIRST_OPEN_CALENDAR_GET_POINT, "i", m_iGetGCPoint ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        m_iGetGCPoint = 0;
    }
}

void KGCAttendanceCalendar::SetGCPoint(int _nPoint)
{
    m_iGetGCPoint = _nPoint;
}