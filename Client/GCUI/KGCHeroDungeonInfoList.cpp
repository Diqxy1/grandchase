#include "StdAfx.h"
#include "KGCHeroDungeonInfoList.h"

extern std::wstring GetEnableEnteranceTimeStr( int iStartHour_, int iStartMin_, int iEndHour_, int iEndMin_ );

IMPLEMENT_CLASSNAME( KGCHeroDungeonInfoList );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonInfoList );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonInfoList, "gc_dungeon_detail_info" );

KGCHeroDungeonInfoList::KGCHeroDungeonInfoList(void)
: m_pkStaticName(NULL)
, m_pkStaticLocate(NULL)
, m_pkStaticOpenTime(NULL)
, m_pkStaticClear(NULL)
, m_pkStaticInitTime(NULL)
, m_pkCheckClearRed(NULL)
, m_pkCheckClearGreen(NULL)
, m_pkBtnDetailOpenTime(NULL)
{
    LINK_CONTROL("static_dungeon_name",			m_pkStaticName );
    LINK_CONTROL("static_locate",			    m_pkStaticLocate );
    LINK_CONTROL("static_open_time",			m_pkStaticOpenTime );
    LINK_CONTROL("static_clear",			    m_pkStaticClear );
    LINK_CONTROL("static_initialization",		m_pkStaticInitTime );

    LINK_CONTROL("check_clear_red",		        m_pkCheckClearRed );
    LINK_CONTROL("check_clear_green",		    m_pkCheckClearGreen );
    LINK_CONTROL("btn_open_time",		        m_pkBtnDetailOpenTime );
}

KGCHeroDungeonInfoList::~KGCHeroDungeonInfoList(void)
{
}

void KGCHeroDungeonInfoList::OnCreate( void )
{
    m_pkStaticName->InitState(true, true);
    m_pkStaticLocate->InitState(true, true);
    m_pkStaticOpenTime->InitState(true, true);
    m_pkStaticOpenTime->SetAlign(DT_CENTER);
    m_pkStaticClear->InitState(true, true);
    m_pkStaticClear->SetAlign(DT_CENTER);
    m_pkStaticInitTime->InitState(true, true);
    m_pkStaticInitTime->SetAlign(DT_CENTER);

    m_pkCheckClearGreen->InitState(true, true);
    m_pkCheckClearRed->InitState(false, true);

    m_pkBtnDetailOpenTime->InitState(true, true, this);

    InitDungeonInfo();
}

void KGCHeroDungeonInfoList::OnCreateComplete( void )
{
}

void KGCHeroDungeonInfoList::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnDetailOpenTime,			KD3DWnd::D3DWE_BUTTON_CLICK, OnOpenTime );
}

void KGCHeroDungeonInfoList::OnDestroyComplete( void )
{
}

void KGCHeroDungeonInfoList::FrameMoveInEnabledState( void )
{
}

void KGCHeroDungeonInfoList::PostChildDraw( void )
{
}

void KGCHeroDungeonInfoList::OnOpenTime()
{      
    
}

void KGCHeroDungeonInfoList::SetCheckClear( bool bClear_ )
{
    m_pkCheckClearRed->ToggleRender(bClear_);
    m_pkCheckClearGreen->ToggleRender(!bClear_);
}

void KGCHeroDungeonInfoList::InitDungeonInfo()
{
    m_pkStaticName->SetText(L"");
    m_pkStaticLocate->SetText(L"");
    m_pkStaticOpenTime->SetText(L"");
    m_pkStaticClear->SetText(L"");
    m_pkStaticInitTime->SetText(L"");

    SetCheckClear(false);
}

void KGCHeroDungeonInfoList::SetDungeonDetailInfo( KDetailDungeonInfo kInfo_ )
{
    InitDungeonInfo();

    m_prOpenTimeInfo = std::make_pair(kInfo_.wstrName, kInfo_.vecAllOpenTime);

    m_pkStaticName->SetText(kInfo_.wstrName);
    m_pkStaticLocate->SetText(kInfo_.wstrLocate);

    //CTime curTime = CTime::GetCurrentTime();
    CTime curTime = g_kGlobalValue.m_tmServerTime;
    int iCurChangeMinute = curTime.GetHour() * 60 + curTime.GetMinute();    
    if( curTime.GetHour() == 0 )
        iCurChangeMinute = 24 * 60 + curTime.GetMinute();

    std::wstringstream str;

    std::vector<KHeroDungeon>::iterator vit = std::find_if( g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.begin(), 
        g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end(),
        boost::bind(&KHeroDungeon::m_nModeID,_1) == kInfo_.iGameMode );
    if( vit != g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end() )
    {        
        bool bEnableTime = false;

        std::map<USHORT,std::pair<time_t,time_t> >::iterator mit = vit->m_mapOpenTime.begin();                
        for( ; mit != vit->m_mapOpenTime.end(); ++mit )
        {
            CTime startTime(mit->second.first+ g_kGlobalValue.m_tmServerSyncTime);
            CTime endTime(mit->second.second+ g_kGlobalValue.m_tmServerSyncTime);

            int iStartChangeMinute  = startTime.GetHour() * 60 + startTime.GetMinute();
            int iEndChangeMinute    = endTime.GetHour() * 60 + endTime.GetMinute();

            if( curTime.GetHour() == 0 && startTime.GetHour() == 0 )
                iCurChangeMinute = 24 * 60 + curTime.GetMinute();

            if( curTime.GetHour() == 0 && startTime.GetHour() == 1 )
                iCurChangeMinute = curTime.GetMinute();

            if( startTime.GetHour() == 0 )
                iStartChangeMinute  = 24 * 60 + startTime.GetMinute();

            if( endTime.GetHour() == 0 )
                iEndChangeMinute  = 24 * 60 + endTime.GetMinute();

            //현재입장가능시간을 구한다.
            if( iCurChangeMinute >= iStartChangeMinute && iCurChangeMinute <= iEndChangeMinute )
            {                                                
                m_pkStaticOpenTime->SetText(GetEnableEnteranceTimeStr(startTime.GetHour(), startTime.GetMinute(), endTime.GetHour(), endTime.GetMinute()));
                if( startTime.GetHour() != 0 || endTime.GetHour() != 0 ) 
                    break;
            }
            else if( iCurChangeMinute < iStartChangeMinute )
            {
                m_pkStaticOpenTime->SetText(GetEnableEnteranceTimeStr(startTime.GetHour(), startTime.GetMinute(), endTime.GetHour(), endTime.GetMinute()));
                if( startTime.GetHour() != 0 || endTime.GetHour() != 0 ) 
                    break;
            }
        }
    }

    CTime resetTime(g_kGlobalValue.m_kHeroDungeonInfo.m_tmResetTime+ g_kGlobalValue.m_tmServerSyncTime);
    int iResetChangeMinute = resetTime.GetHour() * 60 + resetTime.GetMinute();
    int iRemainTime;

    if( iResetChangeMinute == 0 )
        iRemainTime = 1440 - iCurChangeMinute;
    else if( iResetChangeMinute < iCurChangeMinute )
        iRemainTime = 1440 - iCurChangeMinute + iResetChangeMinute;
    else if( iResetChangeMinute > iCurChangeMinute )
        iRemainTime = iResetChangeMinute - iCurChangeMinute;
    else
        iRemainTime = 0;

    //초기화 시간 셋팅
    if( iRemainTime == 0 )
        m_pkStaticInitTime->SetText(g_pkStrLoader->GetReplacedString( STR_ID_HERO_DUNGEON_INFO_DLG_INIT_TIME, "ii", 0, 0 ));
    else
        m_pkStaticInitTime->SetText(g_pkStrLoader->GetReplacedString( STR_ID_HERO_DUNGEON_INFO_DLG_INIT_TIME, "ii", static_cast<int>(iRemainTime/60), iRemainTime%60 ));

    //클리어 횟수 셋팅
    int iClearCount = g_kGlobalValue.m_kHeroDungeonInfo.GetHeroDungeonPlayCount( kInfo_.iGameMode );
    if( iClearCount > static_cast<int>(kInfo_.usMaxClearPerDay) )
        iClearCount = kInfo_.usMaxClearPerDay;
    str << iClearCount << "/" << kInfo_.usMaxClearPerDay;
    if( iClearCount >= static_cast<int>(kInfo_.usMaxClearPerDay))
        SetCheckClear(true);
    else
        SetCheckClear(false);


    m_pkStaticClear->SetText(str.str());


}