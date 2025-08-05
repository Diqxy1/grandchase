#include "StdAfx.h"
#include "KGCHeroDungeonInfo.h"
#include "KGCHeroDungeonInfoList.h"
#include "KGCHeroDungeonOpentimeBox.h"

extern std::wstring GetEnableEnteranceTimeStr( int iStartHour_, int iStartMin_, int iEndHour_, int iEndMin_ );

IMPLEMENT_CLASSNAME( KGCHeroDungeonInfo );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonInfo, "gc_hero_dungeon_info" );

KGCHeroDungeonInfo::KGCHeroDungeonInfo(void)
: m_pkScrollBar( NULL )
, m_pkOpentimeBox( NULL )
{
    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < EMAX_DUNGEON_LIST_NUM; ++i )
    {
        m_pkDungeonInfoList[i] = NULL;
        sprintf_s( str, "dungeon_detail_info%d", i );
        LINK_CONTROL( str, m_pkDungeonInfoList[i] );
    }

    LINK_CONTROL( "scroll",                     m_pkScrollBar );
    LINK_CONTROL( "background",                 m_pkBackground );
    LINK_CONTROL( "hero_dungeon_opentime_box",  m_pkOpentimeBox );
}

KGCHeroDungeonInfo::~KGCHeroDungeonInfo(void)
{
}

void KGCHeroDungeonInfo::OnCreate( void )
{
    for( int i = 0; i < EMAX_DUNGEON_LIST_NUM; ++i )
        m_pkDungeonInfoList[i]->InitState(false, true, this);

    m_pkScrollBar->InitState( true, true, this );
    m_pkBackground->InitState( true );
    m_pkOpentimeBox->InitState( false, true, this);
    InitScroll();
}

void KGCHeroDungeonInfo::OnCreateComplete( void )
{
}

void KGCHeroDungeonInfo::ActionPerformed( const KActionEvent& event )
{
    if( m_pkOpentimeBox->IsRenderOn() )
        return;

    GCWND_MAP( m_pkScrollBar, OnScroll );
}

void KGCHeroDungeonInfo::OnDestroyComplete( void )
{
}

void KGCHeroDungeonInfo::FrameMoveInEnabledState( void )
{
    if( m_pkOpentimeBox->IsRenderOn() )
        return;

    if( m_pkScrollBar->IsLocked() )
        m_pkScrollBar->Lock(false);

    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

    if( m_pkBackground->CheckPosInWindowBound(vMousePos) )
    {
        if( g_pkInput->ISWheelUp() )
        {
            if( m_pkScrollBar->GetScrollPos() > 0 )
            {
                m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos());
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }
        else if( g_pkInput->ISWheelDown() )
        {
            if( m_pkScrollBar->GetScrollPos() < m_pkScrollBar->GetScrollRangeMax() )
            {
                m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos());
                OnScroll();
                g_KDSound.Play( "73" );
            }
        }	
    }

    if( m_kStopWatch.IsStop() && this->IsRenderOn() )
        OnScroll();    
}

void KGCHeroDungeonInfo::PostChildDraw( void )
{
}

void KGCHeroDungeonInfo::OnScroll()
{
    int iRealindex = m_pkScrollBar->GetScrollPos();
    int iMaxListView = GetListViewCount( iRealindex );

    for(int i = 0; i < iMaxListView; ++i )
    {
        int iIndex = i + iRealindex;

        if( 0 <= iIndex && iIndex < static_cast<int>(m_vecHeroDungeonInfo.size()) )
        {		
            m_pkDungeonInfoList[i]->ToggleRender( true );
            if( static_cast<int>(m_vecHeroDungeonInfo.size()) > iIndex)
            {
                m_pkDungeonInfoList[i]->SetDungeonDetailInfo(m_vecHeroDungeonInfo[iIndex]);
            }
            else
            {
                m_pkDungeonInfoList[i]->SetDungeonDetailInfo(KGCHeroDungeonInfoList::KDetailDungeonInfo());
            }
        }
    }

    for(int i = iMaxListView; i < EMAX_DUNGEON_LIST_NUM; ++i)
    {
        m_pkDungeonInfoList[i]->ToggleRender( false );
    }
}

int KGCHeroDungeonInfo::GetListViewCount(int iIndex_)
{
    int iListViewCount = static_cast<int>(m_vecHeroDungeonInfo.size() ) - iIndex_;

    if(iListViewCount < EMAX_DUNGEON_LIST_NUM)
        return iListViewCount;

    return EMAX_DUNGEON_LIST_NUM;
}

void KGCHeroDungeonInfo::InitScroll()
{
    m_kStopWatch.SetStopWatch(60000);
    m_pkScrollBar->SetScrollPos( 0 );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollPageSize( 1 );

    int iFileCount = static_cast<int>( m_vecHeroDungeonInfo.size() );    
    int iScrollMax = 0;
    int iCount =  iFileCount - EMAX_DUNGEON_LIST_NUM;

    if(iCount > 0)
    {
        iScrollMax = iCount;
    }

    m_pkScrollBar->SetScrollRangeMax( iScrollMax + 1 );

    OnScroll();
}

void KGCHeroDungeonInfo::SetHeroDungeonInfo( std::vector<KHeroDungeon> vecDungeonInfo_ )
{
    m_vecHeroDungeonInfo.clear();
    CTime curTime = CTime::GetCurrentTime();

    for( int i = 0; i < static_cast<int>(vecDungeonInfo_.size()); ++i )
    {
        KGCHeroDungeonInfoList::KDetailDungeonInfo kDungeonDetailInfo;        
        
        //던전ID
        kDungeonDetailInfo.iGameMode = vecDungeonInfo_[i].m_nModeID;
        //일일 최대클리어 수
        kDungeonDetailInfo.usMaxClearPerDay = vecDungeonInfo_[i].m_usMaxClearPerDay;
        //던전이름 셋팅
        kDungeonDetailInfo.wstrName = SiKGCWorldMapManager()->GetCurrentDungeonName( (EGCGameMode)vecDungeonInfo_[i].m_nModeID );
        //대륙이름 셋팅
        kDungeonDetailInfo.wstrLocate =SiKGCWorldMapManager()->GetContinentName( (EGCGameMode)vecDungeonInfo_[i].m_nModeID );
        
        //던전 오픈시간 셋팅
        std::map<USHORT,std::pair<time_t,time_t> >::iterator mit = vecDungeonInfo_[i].m_mapOpenTime.begin();
        for( ; mit != vecDungeonInfo_[i].m_mapOpenTime.end(); ++mit )
        {
            CTime startTime(mit->second.first + g_kGlobalValue.m_tmServerSyncTime);
            CTime endTime(mit->second.second + g_kGlobalValue.m_tmServerSyncTime);            

            kDungeonDetailInfo.vecAllOpenTime.push_back(GetEnableEnteranceTimeStr(startTime.GetHour(),startTime.GetMinute(),endTime.GetHour(),endTime.GetMinute()));
        }

        m_vecHeroDungeonInfo.push_back(kDungeonDetailInfo);
    }
}

void KGCHeroDungeonInfo::SetSelectDungeonAllOpentime( std::pair<std::wstring, std::vector<std::wstring>> prOpentime_ )
{
    m_pkOpentimeBox->SetAllOpentime(prOpentime_);
    m_pkScrollBar->Lock(true);
}

bool KGCHeroDungeonInfo::IsRenderOnOpentimeBox()
{
    return m_pkOpentimeBox->IsRenderOn();
}