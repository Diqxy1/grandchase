#include "stdafx.h"

#include "GCOlympicScheduleSlot.h"
#include "GCOlympicRewardSlot.h"
#include "GCOlympicRankSlot.h"
#include "GCOlympicEventDlg.h"

IMPLEMENT_CLASSNAME( KGCOlympicEventDlg );
IMPLEMENT_WND_FACTORY( KGCOlympicEventDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicEventDlg, "gc_olympicevent_dlg" );

KGCOlympicEventDlg::KGCOlympicEventDlg( void )
: m_iScheduleSize( 0 )
, m_iScheduleScrollMax( 0 )
, m_iScheduleScrollPos( 0 )
//, m_tmServerTime( 0 )
{
    m_pkBtnStart = NULL;
    m_pkBtnClose = NULL;
    m_pkStaticGameTitle = NULL;
    m_pkStaticTimeAlram = NULL;
    LINK_CONTROL( "btn_start",           m_pkBtnStart );
    LINK_CONTROL( "btn_close",           m_pkBtnClose );
    LINK_CONTROL( "static_game_title",   m_pkStaticGameTitle );
    LINK_CONTROL( "static_time_alram",   m_pkStaticTimeAlram );

    m_pkStaticRank = NULL;
    m_pkStaticNation = NULL;
    m_pkStaticScore = NULL;
    m_pkStaticUpdateDesc = NULL;
    LINK_CONTROL( "static_rank",        m_pkStaticRank );
    LINK_CONTROL( "static_nation",      m_pkStaticNation );
    LINK_CONTROL( "static_score",       m_pkStaticScore );
    LINK_CONTROL( "static_update_desc", m_pkStaticUpdateDesc );

    m_pkScheduleScroll = NULL;
    LINK_CONTROL( "schedule_scroll", m_pkScheduleScroll );


    for( int i=0 ; i<NUM_SCHEDULE_SLOT ; ++i ) { 
        char szTemp[ 128 ];

        m_apkScheduleSlot[ i ] = NULL;
        sprintf_s( szTemp, 127, "schedule_slot%d", i );
        LINK_CONTROL( szTemp, m_apkScheduleSlot[ i ] );
    }

    for( int i=0 ; i<NUM_REWARD_SLOT ; ++i ) { 
        char szTemp[ 128 ];

        m_apkRewardSlot[ i ] = NULL;
        sprintf_s( szTemp, 127, "reward_slot%d", i );
        LINK_CONTROL( szTemp, m_apkRewardSlot[ i ] );
    }

    for( int i=0 ; i<NUM_RANK_SLOT ; ++i ) { 
        char szTemp[ 128 ];

        m_apkRankSlot[ i ] = NULL;
        sprintf_s( szTemp, 127, "rank_slot%d", i );
        LINK_CONTROL( szTemp, m_apkRankSlot[ i ] );
    }
}

KGCOlympicEventDlg::~KGCOlympicEventDlg( void )
{
}

void KGCOlympicEventDlg::OnCreate( void )
{
    m_pkBtnStart->InitState( true, true, this );
    m_pkBtnStart->SetWndMode( KD3DWnd::D3DWM_LOCK );
    
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->DisconnectAllEventProcedure();
    m_pkBtnClose->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCOlympicEventDlg::Onclose );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkStaticGameTitle->InitState( true );
    m_pkStaticGameTitle->SetAlign( DT_LEFT );
    m_pkStaticTimeAlram->InitState( true );
    m_pkStaticTimeAlram->SetAlign( DT_CENTER );

    m_pkStaticRank->InitState( true );
    m_pkStaticNation->InitState( true );
    m_pkStaticScore->InitState( true );
    m_pkStaticUpdateDesc->InitState( true );
    m_pkStaticRank->SetAlign( DT_CENTER );
    m_pkStaticNation->SetAlign( DT_CENTER );
    m_pkStaticScore->SetAlign( DT_CENTER );
    m_pkStaticUpdateDesc->SetAlign( DT_LEFT );
    m_pkStaticUpdateDesc->SetMultiLine( true );

    m_pkStaticRank->SetText( g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_RANKING ) );
    m_pkStaticNation->SetText( g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_NATION ) );
    m_pkStaticScore->SetText( g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_TOTAL_SCORE ) );
    m_pkStaticUpdateDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_UPDATE_DESC ) );


    m_pkScheduleScroll->InitState( true, true, this );


    for( int i=0 ; i<NUM_SCHEDULE_SLOT ; ++i ) { 
        m_apkScheduleSlot[ i ]->InitState( true );
    }

    for( int i=0 ; i<NUM_REWARD_SLOT ; ++i ) {
        m_apkRewardSlot[ i ]->InitState( true );
    }

    for( int i=0 ; i<NUM_RANK_SLOT ; ++i ) {
        m_apkRankSlot[ i ]->InitState( true );
    }
}

void KGCOlympicEventDlg::OnCreateComplete( void )
{
    // 올림픽 정보 갱신
    Result_OlympicEven_Data = INT_MAX;
    KP2P::GetInstance()->Send_OlympicInfoReq();
    g_MyD3D->WaitForServerAck( Result_OlympicEven_Data, INT_MAX, 3000, TIME_OUT_VALUE ); 

    // 오늘 경기 세팅 
    m_pkStaticGameTitle->ToggleRender( true );
    m_pkStaticGameTitle->SetText( SiGCOlympicManager()->GetTodayOlympicGameName() );

    // 정보 세팅하고 
    SetScheduleInfo();
    SetRewardInfo();
    SetRankInfo();

    // UI 세팅
    InitScheduleScroll();
    SetScheduleSlot();  
    SetRewardSlot();
    SetRankSlot();

    // 시작버튼 세팅 
    SetStartBtnState( SiGCOlympicManager()->IsPlayEnable() );
    SetStartBtnToolTip( -1 );
}

void KGCOlympicEventDlg::ActionPerformed( const KActionEvent& event )
{
    // 스크롤 처리 
    if( m_pkScheduleScroll && event.m_pWnd == m_pkScheduleScroll ) { 
        switch( event.m_dwCode ) { 
            case 0:
            case D3DWE_SCROLLBAR_DOWNBTN:
            case D3DWE_SCROLLBAR_UPBTN:
            case D3DWE_SCROLLBAR_DRAG:
                if( m_pkScheduleScroll->GetScrollPos() != m_iScheduleScrollPos ) { 
                    SetScheduleScrollPos( m_pkScheduleScroll->GetScrollPos() );
                }
            break;
        }
    }

    GCWND_MSG_MAP( m_pkBtnStart,                  KD3DWnd::D3DWE_BUTTON_CLICK, OnStartBtn )

}

void KGCOlympicEventDlg::FrameMoveInEnabledState( void )
{
    // 스크롤 처리 
    if( m_pkScheduleScroll ) { 
        if( g_pkInput->ISWheelUp() ) { 
            --m_iScheduleScrollPos;
            if( m_iScheduleScrollPos < 0 ) { 
                m_iScheduleScrollPos = 0;
            }

            SetScheduleSlot();
        }
        else if( g_pkInput->ISWheelDown() ) {
            ++m_iScheduleScrollPos;
            if( m_iScheduleScrollPos > m_iScheduleScrollMax ) { 
                m_iScheduleScrollPos = m_iScheduleScrollMax;
            }

            SetScheduleSlot();
        }
    }
}

void KGCOlympicEventDlg::SetScheduleSlot( void )
{
    if( m_vecScheduleInfo.empty() ) { 
        return;
    }

    for( int i=0 ; i<NUM_SCHEDULE_SLOT ; ++i ) { 
        m_apkScheduleSlot[ i ]->ClearScheduleSlot();
    }

    std::vector< std::pair< KOlympicSchedule, KRegDateRankInfo > >::const_iterator it = m_vecScheduleInfo.begin() + m_iScheduleScrollPos;
    for( int i=0 ; i<NUM_SCHEDULE_SLOT ; ++i ) { 
        if( it == m_vecScheduleInfo.end() ) {
            break;
        }

        m_apkScheduleSlot[ i ]->SetScheduleSlot( SiGCOlympicManager()->GetTimeToString( it->first.m_tmRegDate ),
                                                 SiGCOlympicManager()->GetOlympicGameName( it->first.m_nGameType, it->first.m_nTeamType ),
                                                 it->second.m_strNickName, it->second.m_nCountryCode, 
                                                 SiGCOlympicManager()->IsToday( it->first.m_tmRegDate) );
        ++it;
    }
}

void KGCOlympicEventDlg::Onclose( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCOlympicEventDlg::SetScheduleInfo( void )
{
    m_iScheduleSize = 0;
    m_vecScheduleInfo.clear();
    SiGCOlympicManager()->GetScheduleListWithDateRank( m_vecScheduleInfo );

    m_iScheduleSize = static_cast<int>( m_vecScheduleInfo.size() );
}

void KGCOlympicEventDlg::InitScheduleScroll( void )
{
    m_iScheduleScrollMax = m_iScheduleSize - NUM_SCHEDULE_SLOT;

    if( m_iScheduleScrollMax < 0 ) { 
        m_iScheduleScrollMax = 0;
    }
    m_iScheduleScrollPos = 0;

    m_pkScheduleScroll->InitScroll( m_iScheduleScrollPos, 1, 0, m_iScheduleScrollMax, 0, 0, 0, true );
}

void KGCOlympicEventDlg::SetScheduleScrollPos( const int iPos_ )
{
    m_iScheduleScrollPos = iPos_;
    if( m_iScheduleScrollPos < 0 ) { 
        m_iScheduleScrollPos = 0;
    }
    else if( m_iScheduleScrollPos > m_iScheduleScrollMax ) { 
        m_iScheduleScrollPos = m_iScheduleScrollMax;
    }

    SetScheduleSlot();
}

void KGCOlympicEventDlg::SetRankInfo( void )
{
    m_vecTotalRank.clear();
    SiGCOlympicManager()->GetTotalRankList( m_vecTotalRank );
}

void KGCOlympicEventDlg::SetRankSlot( void )
{
    for( int i=0 ; i<NUM_RANK_SLOT ; ++i ) { 
        m_apkRankSlot[ i ]->ClearRankSlot();
    }

    // Rank 정렬되있나??
    std::vector< KTotalRankInfo >::const_iterator it = m_vecTotalRank.begin();
    for( int i = 0 ; i<NUM_RANK_SLOT ; ++i ) { 
        if( it == m_vecTotalRank.end() ) { 
            break;
        }

        m_apkRankSlot[ i ]->SetRankSlot( it->m_nCountryCode, it->m_nGrade, it->m_nGradePoint );
        ++it;
    }
}

void KGCOlympicEventDlg::SetStartBtnState( const bool bStartEnable_ )
{
    if( false == bStartEnable_ ) {
        m_pkBtnStart->SetWndMode( KD3DWnd::D3DWM_LOCK );
    }
    else { 
        m_pkBtnStart->SetWndMode( KD3DWnd::D3DWM_DEFAULT );
    }
}

void KGCOlympicEventDlg::SetRewardInfo( void )
{
    m_mapGradeReward.clear();
    SiGCOlympicManager()->GetGradeRewardByScore( m_mapGradeReward );
}

void KGCOlympicEventDlg::SetRewardSlot( void )
{
    if( m_mapGradeReward.empty() ) { 
        return;
    }

    for( int i=0 ; i<NUM_REWARD_SLOT ; ++i ) { 
        m_apkRewardSlot[ i ]->ClearRewadSlot();
    }

    std::map< DWORD, std::vector< KDropItemInfo > >::const_iterator mit = m_mapGradeReward.begin();
    bool bActiveRewardBtn;

    for( int i=0 ; i<NUM_REWARD_SLOT ; ++i ) { 
        if( mit == m_mapGradeReward.end() ) { 
            break;
        }

        DWORD dwCanRecvGrade = SiGCOlympicManager()->GetCanRecvRewardGrade();            // 다음으로 받을 수 있는 보상 등급 
        DWORD dwMaxRewardGrade = -1;
        bool bAbleToRecieve = SiGCOlympicManager()->GetRewardGrade( static_cast<DWORD>(i), dwMaxRewardGrade );   // 현재 점수로 현재 등급의 보상이 가능한가. 

        bActiveRewardBtn = dwCanRecvGrade == dwMaxRewardGrade;

        if( bActiveRewardBtn )
            SiGCOlympicManager()->SetRewardNotice( bActiveRewardBtn );

        m_apkRewardSlot[ i ]->SetRewardSlot( mit->second, mit->first, bAbleToRecieve, bActiveRewardBtn );
        ++mit;
    }
}

void KGCOlympicEventDlg::OnStartBtn()
{
    // 올림픽 UI는 끄자
    Onclose();

    // 오늘의 경기 중 보스게이트면 패킷 보내지 말자.
    std::pair<int, int> prGameType = SiGCOlympicManager()->GetTodayOlympicGameType();

    if ( prGameType.first == KOlympicSchedule::OGM_BOSSGATE ){
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_BOSS_GATE ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        g_pkUIScene->m_pkOlympicDlg->SetStartBtnToolTip( STR_ID_OLYMPIC_GAME_BOSS_GATE );
        SetStartBtnState( false );
        return;
    }

    KP2P::GetInstance()->Send_OlympicJoinGameReq();
}


void KGCOlympicEventDlg::SetStartBtnToolTip( IN int nStr)
{
    if( nStr == -1 )
        m_pkBtnStart->ClearToolTip();
    else
        m_pkBtnStart->SetToolTip( g_pkStrLoader->GetString(nStr) );
}