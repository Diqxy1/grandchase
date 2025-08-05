#include "stdafx.h"
#include "GCDungeonRankingPanel.h"

IMPLEMENT_CLASSNAME( KGCDungeonRankingPanel );
IMPLEMENT_WND_FACTORY( KGCDungeonRankingPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonRankingPanel, "gc_dungeonrankingpanel" );

const DWORD dwGET_DATA_TICK = 1000;

KGCDungeonRankingPanel::KGCDungeonRankingPanel( void )
{
    m_pkLeftBtn = NULL;
    m_pkRightBtn = NULL;
    m_pkRefrashBtn = NULL;

    m_pkNoRank = NULL;
    m_pkMyCurRank = NULL;
    m_pkMyOldRank = NULL;

    m_pkNumMyRank = NULL;

    m_nState = EM_CURRENT_RANK;
    m_nPage = 0;
    m_dwLastReqTick = 0;


    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < EM_SLOT_NUM; ++i ) 
    {
        m_pkRankSlot[i] = NULL;

        sprintf( str, "rank_slot%d", i );  
        LINK_CONTROL( str, m_pkRankSlot[i] );
    }

    LINK_CONTROL( "left_btn", m_pkLeftBtn );
    LINK_CONTROL( "right_btn", m_pkRightBtn );
    LINK_CONTROL( "refrash_btn", m_pkRefrashBtn );

    LINK_CONTROL( "no_rank", m_pkNoRank );
    LINK_CONTROL( "my_current_rank", m_pkMyCurRank );
    LINK_CONTROL( "my_old_rank", m_pkMyOldRank );

    LINK_CONTROL( "number_my_rank", m_pkNumMyRank );
}

KGCDungeonRankingPanel::~KGCDungeonRankingPanel( void )
{
}

void KGCDungeonRankingPanel::OnCreate( void )
{
    m_pkLeftBtn->InitState( true, true, this );
    m_pkRightBtn->InitState( true, true, this );
    m_pkRefrashBtn->InitState( true, true, this );

    m_pkNoRank->InitState( true );
    m_pkMyCurRank->InitState( false );
    m_pkMyOldRank->InitState( false );

    m_pkNumMyRank->InitState( false );
    m_pkNumMyRank->SetStringType( "dungeon_my_rank_big" );
    m_pkNumMyRank->SetWriteToRight( false );
    m_pkNumMyRank->SetWriteAlign( ALIGN_RIGHT_SIDE );
    m_pkNumMyRank->SetInterval(14.f);
    m_pkNumMyRank->SetShadow( false );

    for ( int i = 0; i < EM_SLOT_NUM; ++i ) 
    {
        m_pkRankSlot[i]->InitState( true );
    }

    m_nState = EM_CURRENT_RANK;
    m_nPage = 0;
}

void KGCDungeonRankingPanel::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkLeftBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnLeftBtn );
    GCWND_MSG_MAP( m_pkRightBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnRightBtn );
    GCWND_MSG_MAP( m_pkRefrashBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnRefrashBtn );
    
}

void KGCDungeonRankingPanel::OnLeftBtn()
{
    if ( m_nPage <= 0 )
        return;

    ChangeRankingPage( m_nPage - 1 );
}

void KGCDungeonRankingPanel::OnRightBtn()
{
    if ( m_nPage >= EM_MAX_PAGE - 1 )
        return;

    ChangeRankingPage( m_nPage + 1 );
}

void KGCDungeonRankingPanel::OnRefrashBtn()
{
    UpdateRankingData();
}

bool KGCDungeonRankingPanel::EnableGetData()
{
    if ( ( GetTickCount() - m_dwLastReqTick ) > dwGET_DATA_TICK )
        return true;

    return false;
}

void KGCDungeonRankingPanel::SetRankingState( int nState )
{
    m_nState = nState;
    m_nPage = 0;
}

void KGCDungeonRankingPanel::ReqRankingData()
{
    bool bCurrent = m_nState == EM_CURRENT_RANK;

    if ( bCurrent )
    {
        KP2P::GetInstance()->Send_DungeonCurrentSeasonUserRankReq( GC_GM_QUEST81 );
        KP2P::GetInstance()->Send_DungeonCurrentSeasonRankPageReq( GC_GM_QUEST81, m_nPage );
    }
    else
    {
        KP2P::GetInstance()->Send_DungeonPreviousSeasonUserRankReq( GC_GM_QUEST81 );
        KP2P::GetInstance()->Send_DungeonPreviousSeasonRankPageReq( GC_GM_QUEST81, m_nPage );
    }
}

void KGCDungeonRankingPanel::ChangeRankingPage( int nPage )
{    
    bool bCurrent = m_nState == EM_CURRENT_RANK;

    m_dwLastReqTick = GetTickCount();

    LockUI( true );
    SetPage( nPage );

    // 서버 요청 처리
    ReqRankingData();

    // 테스트 함수
//    TestFunction( bCurrent, nPage );
}

void KGCDungeonRankingPanel::UpdateRankingData()
{
    bool bCurrent = m_nState == EM_CURRENT_RANK;

    // 정보를 요청 가능한가?
    if ( EnableGetData() )
    {
        // 정보를 요청한다.
        // Packet받아서 갱신한다.
        m_dwLastReqTick = GetTickCount();        

        ReqRankingData();

        // 테스트 함수
//        TestFunction( bCurrent, m_nPage );
    }
    else
    {
        // 현재 있는 데이터로 갱신한다.
        UpdateUI();
    }
}

void KGCDungeonRankingPanel::UpdateUI()
{
    // 개인 랭킹
    bool bCurrent = m_nState == EM_CURRENT_RANK;
    int nMyRank = -1;

    KDungeonRankInfo kMyRankInfo;

    if ( g_kGlobalValue.m_kHeroDungeonInfo.GetMyDungeonRank( bCurrent, g_kGlobalValue.m_kUserInfo.GetCurrentCharType(), GC_GM_QUEST81, kMyRankInfo ) )
    {
        nMyRank = kMyRankInfo.m_nRank;
        m_pkNumMyRank->SetNumber( nMyRank );
        UpdateMyRankinUI( true );
    }
    else
    {
        UpdateMyRankinUI( false );
    }


    std::vector< KDungeonRankInfo > vecDungeonRank;

    if ( g_kGlobalValue.m_kHeroDungeonInfo.GetDungeonRank( bCurrent, GC_GM_QUEST81, m_nPage, vecDungeonRank ) )
    {
        for ( int i = 0; i < EM_SLOT_NUM; ++i ) 
        {
            if ( (int)vecDungeonRank.size() > i )
            {
                bool bMyRank = IsMyRank( vecDungeonRank[i] );
                m_pkRankSlot[i]->SetRankData( bMyRank, vecDungeonRank[i] );
            }
            else
            {
                int nRank = ( m_nPage * EM_SLOT_NUM ) + ( i + 1 );
                m_pkRankSlot[i]->SetRank( nRank );
            }            
        }
    }
    else
    {
        for ( int i = 0; i < EM_SLOT_NUM; ++i ) 
        {
            int nRank = ( m_nPage * EM_SLOT_NUM ) + ( i + 1 );
            m_pkRankSlot[i]->SetRank( nRank );
        }
    }
        
}

void KGCDungeonRankingPanel::UpdateMyRankinUI( bool bRank )
{
    bool bCurrent = m_nState == EM_CURRENT_RANK;
    
    m_pkNoRank->ToggleRender( !bRank );
    m_pkMyCurRank->ToggleRender( bRank && bCurrent );
    m_pkMyOldRank->ToggleRender( bRank && !bCurrent );
    m_pkNumMyRank->ToggleRender( bRank );
}

void KGCDungeonRankingPanel::LockUI( bool bLock )
{
    m_pkLeftBtn->Lock( bLock );
    m_pkRightBtn->Lock( bLock );
}

bool KGCDungeonRankingPanel::IsMyRank( const KDungeonRankInfo& kRank )
{
    if ( kRank.m_dwLoginUID != g_kGlobalValue.m_kUserInfo.dwUID )
        return false;

    if ( kRank.m_cCharType != g_kGlobalValue.m_kUserInfo.GetCurrentCharType() )
        return false;

    return true;
}

void KGCDungeonRankingPanel::TestFunction( bool bCurrent, int nPage )
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" ) == false )
        return;

    bool bMyRank;
    LUA_GET_VALUE( "MyRank", bMyRank, return );

    KDungeonRankInfo kMyRankInfo;

    if ( bMyRank )
    {
        if ( bCurrent )
        {
            LUA_GET_VALUE( "MyCurRankData", kMyRankInfo.m_nRank, return );
        }
        else
        {
            LUA_GET_VALUE( "MyOldRankData", kMyRankInfo.m_nRank, return );
        }

        g_kGlobalValue.m_kHeroDungeonInfo.SetMyDungeonRank( bCurrent, g_kGlobalValue.m_kUserInfo.GetCurrentCharType(), GC_GM_QUEST81, kMyRankInfo );
    }
    else
    {
        kMyRankInfo.m_nRank = -1;
        g_kGlobalValue.m_kHeroDungeonInfo.SetMyDungeonRank( bCurrent, g_kGlobalValue.m_kUserInfo.GetCurrentCharType(), GC_GM_QUEST81, kMyRankInfo );
    }

    std::string strTable = "CurrentRankingList";
    if ( !bCurrent )
        strTable = "OldRankingList";


    LUA_BEGIN_TABLE( strTable.c_str() ,return)
    {
        char str[MAX_PATH] = {0,};
        sprintf( str, "Page%d", nPage ); 

        if( !FAILED( luaMgr.BeginTable( str ) ) ) 
        {
            std::vector< KDungeonRankInfo > vecDungeonRank;

            for(int i = 1; ; i++)
            {
                LUA_BEGIN_TABLE( i, break )
                {
                    KDungeonRankInfo kRecord;

                    std::string strNick;
                    int nCharType;

                    LUA_GET_VALUE( "LoingUID", kRecord.m_dwLoginUID, return );
                    LUA_GET_VALUE( "Rank", kRecord.m_nRank, return );
                    LUA_GET_VALUE( "Char", nCharType, return );
                    LUA_GET_VALUE( "Name", strNick, return );                        
                    LUA_GET_VALUE( "Stage", kRecord.m_nStageNum, return );
                    LUA_GET_VALUE( "ClearTime", kRecord.m_dwStageClearTime, return );

                    kRecord.m_wstrNickName = KncUtil::toWideString( strNick );
                    kRecord.m_cCharType = (char)nCharType;

                    vecDungeonRank.push_back( kRecord );

                    
                }
                LUA_END_TABLE(return)
                
            }

            if ( !vecDungeonRank.empty() )
            {
                g_kGlobalValue.m_kHeroDungeonInfo.SetDungeonRank( bCurrent, GC_GM_QUEST81, nPage, vecDungeonRank );
            }

            LUA_END_TABLE(return)
        }
        

    }
    LUA_END_TABLE(return)

    g_pkUIScene->m_pkDungeonRankingDlg->LockUI( false );
    g_pkUIScene->m_pkDungeonRankingDlg->UpdateRankingUI();
}