#include "GCOlympicManager.h"
#include "kgcminigamemanager.h"

#include "GCUI/GCOlympicGawibawiboGame.h"
#include "GCUI/KGCOlympicDicePlay.h"
#include "GCUI/GCOlympicSongkranDlg.h"
#include "GCUI/GCOlympicEventDlg.h"
#include "GCUI/GCEventMinigameList.h"



ImplementSingleton( GCOlympicManager )

GCOlympicManager::GCOlympicManager()
: m_bSetInfo( false )
, m_bPlayEnable( false )
, m_bRewardNotice( false )
, m_dwRewardGrade( 0 )
, m_dwCountryID( 0 )
, m_pOlympicEvent(NULL)
, m_fX1Pos(1.0f)
, m_bIsRight(true)
, m_bIsOlympicEvent(false)
{
    m_vecSchedule.clear();
    m_vecDateRank.clear();
    m_vecTotalRank.clear();
    m_mapGradeDesc.clear();
    m_mapGradeRewardList.clear();
}

GCOlympicManager::~GCOlympicManager()
{

}

void GCOlympicManager::ClearOlympicInfo( void )
{
    m_bSetInfo = false;
    m_bPlayEnable = false;
    m_dwRewardGrade = 0;

    m_vecDateRank.clear();
    m_vecTotalRank.clear();
    m_mapGradeDesc.clear();
    m_mapGradeRewardList.clear();
}

void GCOlympicManager::ClearOlympicSchedule( void )
{
    m_bSetInfo = false;
    m_vecSchedule.clear();
}

void GCOlympicManager::SetScheduleList( IN std::vector< KOlympicSchedule >& vecList_ )
{
    m_vecSchedule.clear();
    for( std::vector< KOlympicSchedule >::iterator it = vecList_.begin() ; it != vecList_.end() ; ++it ) { 
        it->m_tmRegDate += g_kGlobalValue.m_tmServerSyncTime;
        m_vecSchedule.push_back( *it );
    }
}

void GCOlympicManager::SetDateRankList( IN std::vector< KRegDateRankInfo >& vecList_ )
{
    m_vecDateRank.clear();
    for( std::vector< KRegDateRankInfo >::iterator it = vecList_.begin() ; it != vecList_.end() ; ++it ) { 
        it->m_tmRegDate += g_kGlobalValue.m_tmServerSyncTime;
        m_vecDateRank.push_back( *it );
    }
}

void GCOlympicManager::SetTotalRankList( const IN std::vector< KTotalRankInfo >& vecList_ )
{
    m_vecTotalRank.clear();
    for( std::vector< KTotalRankInfo >::const_iterator it = vecList_.begin() ; it != vecList_.end() ; ++it ) { 
        m_vecTotalRank.push_back( *it );
    }
}


void GCOlympicManager::SetGradeDesc( const IN std::map< DWORD, std::pair< DWORD, DWORD > >& mapGrade_ )
{
    m_mapGradeDesc.clear();
    for( std::map< DWORD, std::pair< DWORD, DWORD > >::const_iterator it = mapGrade_.begin() ; it != mapGrade_.end() ; ++it ) { 
        m_mapGradeDesc.insert( std::make_pair( it->first, it->second ) );
    }
}

void GCOlympicManager::SetGradeReward( const IN std::map< DWORD, std::vector< KDropItemInfo > >& mapGradeReward_ )
{
    m_mapGradeRewardList.clear();
    for( std::map< DWORD, std::vector< KDropItemInfo > >::const_iterator it = mapGradeReward_.begin() ; it != mapGradeReward_.end() ; ++it ) { 
        m_mapGradeRewardList.insert( std::make_pair( it->first, it->second ) );
    }
}

void GCOlympicManager::SetOlympicInfo( IN KEVENT_OLYMPIC_INFO_ACK info_ )
{
    ClearOlympicInfo();

    SetRewardGrade( info_.m_dwGrade );
    SetPlayEnable( info_.m_bPlayEnable );
    SetDateRankList( info_.m_vecRegDateRank );
    SetTotalRankList( info_.m_vecTotalRank );
    SetGradeDesc( info_.m_mapGradeDesc );
    SetGradeReward( info_.m_mapGradeRewardList );
    m_dwCountryID = info_.m_dwCountryID - 1;

    // 시작버튼 세팅 
    g_pkUIScene->m_pkOlympicDlg->SetStartBtnState( m_bPlayEnable );

    // 보상공지 초기화
    m_bRewardNotice = false;
}

bool GCOlympicManager::GetScheduleList( OUT std::vector< KOlympicSchedule >& vecList_ )
{
    vecList_.clear();
    vecList_ = m_vecSchedule;
    return ( false == vecList_.empty() );
}

bool GCOlympicManager::GetDateRankList( OUT std::vector< KRegDateRankInfo >& vecList_ )
{
    vecList_.clear();
    vecList_ = m_vecDateRank;
    return( false == vecList_.empty() );
}

bool GCOlympicManager::GetTotalRankList( OUT std::vector< KTotalRankInfo >& vecList_ )
{
    vecList_.clear();
    vecList_ = m_vecTotalRank;
    return ( false == vecList_.empty() );
}

bool GCOlympicManager::GetGradeDesc( OUT std::map< DWORD, std::pair< DWORD, DWORD > >& mapGrade_ )
{
    mapGrade_.clear();
    mapGrade_ = m_mapGradeDesc;
    return ( mapGrade_.empty() );
}

bool GCOlympicManager::GetGradeReward( OUT std::map< DWORD, std::vector< KDropItemInfo > >& mapGradeReward_ )
{
    mapGradeReward_.clear();
    mapGradeReward_ = m_mapGradeRewardList;
    return ( mapGradeReward_.empty() );
}

bool GCOlympicManager::GetGradeRewardByScore( OUT std::map< DWORD, std::vector< KDropItemInfo > >& mapGradeReward_ )
{
    // 점수별 보상 아이템으로 변환해줍니다. 
    mapGradeReward_.clear();

    for( std::map< DWORD, std::pair< DWORD, DWORD > >::const_iterator it = m_mapGradeDesc.begin() ; it != m_mapGradeDesc.end() ; ++it ) { 
        std::map< DWORD, std::vector< KDropItemInfo > >::const_iterator itemIter = m_mapGradeRewardList.find( it->first );
        if( itemIter == m_mapGradeRewardList.end() ) { 
            continue;
        }

        mapGradeReward_.insert( std::make_pair( it->second.second, itemIter->second ) );    // 점수, 아이템
    }

    return ( false == mapGradeReward_.empty() );
}

bool GCOlympicManager::GetScheduleListWithDateRank( OUT std::vector< std::pair< KOlympicSchedule, KRegDateRankInfo > >& vecList_ )
{
    // 게임 스케쥴 + 각 날짜 랭킹 합쳐줍니다. 
    vecList_.clear();

    for( std::vector< KOlympicSchedule >::const_iterator scheduleIter = m_vecSchedule.begin() ; scheduleIter != m_vecSchedule.end() ; ++scheduleIter ) { 

        bool bCheckDateRank = false;
        for( std::vector< KRegDateRankInfo >::const_iterator dateIter = m_vecDateRank.begin() ; dateIter != m_vecDateRank.end() ; ++dateIter ) { 
            if( scheduleIter->m_tmRegDate == dateIter->m_tmRegDate && 
                scheduleIter->m_nGameType == dateIter->m_nGameType &&
                scheduleIter->m_nTeamType == dateIter->m_nTeamType ) { 
                    vecList_.push_back( std::make_pair( *scheduleIter, *dateIter ) );
                    bCheckDateRank = true;
                    break;
            }
        }

        if( false == bCheckDateRank ) { 
            KRegDateRankInfo tempRank;
            tempRank.m_nCountryCode = -1;
            vecList_.push_back( std::make_pair( *scheduleIter, tempRank ) );
        }
    }

    return ( false == vecList_.empty() );
}

std::wstring GCOlympicManager::GetTimeToString( const time_t tm_ )
{
    int iMonth = GCUTIL_TIME::GetMonth( tm_ );
    int iDay   = GCUTIL_TIME::GetDay( tm_ );

    return g_pkStrLoader->GetReplacedString( STR_ID_OLYMPIC_GAME_DATE, "ii", iMonth, iDay ) ;
}

bool GCOlympicManager::IsToday( const time_t tm_ )
{
    time_t tm_today = g_kGlobalValue.TimeToInt( g_kGlobalValue.m_tmServerTime );

    return ( GCUTIL_TIME::GetMonth( tm_ ) == GCUTIL_TIME::GetMonth( tm_today ) &&
             GCUTIL_TIME::GetDay( tm_ ) == GCUTIL_TIME::GetDay( tm_today ) );
}

std::wstring GCOlympicManager::GetOlympicGameName( const int eMode_, const int eTeam_ )
{
    std::wostringstream stm;

    switch( eMode_ ) {
        case KOlympicSchedule::OGM_TREEDROP:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_TREEDROP );
            break;

        case KOlympicSchedule::OGM_BALLOON:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_BALLOON );
            break;

        case KOlympicSchedule::OGM_WATERBOMB:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_WATERBOMB );
            break;

        case KOlympicSchedule::OGM_GAWIBAWIBO:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_GAWIBAWIBO );
            break;

        case KOlympicSchedule::OGM_DICEPLAY:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_DICEPLAY );
            break;

        case KOlympicSchedule::OGM_BOSSGATE:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_BOSSGATE );
            break;

    }

    switch( eTeam_ ) { 
        case KOlympicSchedule::OGT_SOLO:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_SOLO );
            break;

        case KOlympicSchedule::OGT_TEAM:
            stm << g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_TEAM );
            break;
    }

    return stm.str();
}

std::pair<int, int> GCOlympicManager::GetTodayOlympicGameType( void )
{
    if( m_vecSchedule.empty() ) { 
        return std::make_pair( -1, -1 );
    }

    time_t tm_today = g_kGlobalValue.TimeToInt( g_kGlobalValue.m_tmServerTime );
    int iTodayMonth = GCUTIL_TIME::GetMonth( tm_today );
    int iTodayDay = GCUTIL_TIME::GetDay( tm_today );

    std::vector< KOlympicSchedule >::const_iterator it = m_vecSchedule.begin();
    for( ; it != m_vecSchedule.end() ; ++it ) { 
        int iRegMonth = GCUTIL_TIME::GetMonth( it->m_tmRegDate );
        int iRegDay = GCUTIL_TIME::GetDay( it->m_tmRegDate );

        if( iTodayMonth == iRegMonth && iTodayDay == iRegDay ) { 
            return std::make_pair( it->m_nGameType, it->m_nTeamType );
        }
    }

    return std::make_pair( -1, -1 );
}

std::wstring GCOlympicManager::GetTodayOlympicGameName( void )
{
    std::pair< int, int > prGameType = GetTodayOlympicGameType();

    if( prGameType.first == -1 || prGameType.second == -1 ) { 
        return std::wstring();
    }

    return GetOlympicGameName( prGameType.first, prGameType.second );
}

bool GCOlympicManager::GetRewardGrade( IN const DWORD dwIndex, OUT DWORD& dwGrade_ )
{
    dwGrade_ = -1;

    std::vector< KTotalRankInfo >::iterator vit;
    vit = std::find_if( m_vecTotalRank.begin(), m_vecTotalRank.end(), boost::bind( &KTotalRankInfo::m_nCountryCode, _1 ) == static_cast<int>( m_dwCountryID + 1 ) );
    if ( vit == m_vecTotalRank.end() ) {
        return false;
    }

    // 내 국가의 점수를 가져오자
    DWORD dwNationPoint = static_cast<DWORD>( vit->m_nGradePoint );

    if( dwNationPoint >= m_mapGradeDesc[dwIndex+1].second ) { 
        dwGrade_ = dwIndex;
        return true;
    }

    return false;
}

bool GCOlympicManager::GetRewardGradeFromScore( const IN DWORD dwScore, OUT DWORD &dwGrade_ )
{
    std::map< DWORD, std::pair< DWORD, DWORD > >::const_iterator it = m_mapGradeDesc.begin();
    for( ; it != m_mapGradeDesc.end() ; ++it ) { 
        if( dwScore == it->second.second ) { 
            dwGrade_ = it->first;
            return true;
        }
    }

    dwGrade_ = 0;
    return false;
}


void GCOlympicManager::SendOlympicTreeDropResult( int iScore )
{
    KEVENT_OLYMPIC_TREEDROP_RESULT_REQ kPacket;

    kPacket.m_nIncreasedData = iScore;
    kPacket.m_nMyAccumulateData = 0; 
    kPacket.m_nGameType = m_kGameToday.m_nGameType;
    kPacket.m_nTeamType = m_kGameToday.m_nTeamType;
    kPacket.m_tmTodayDate = m_kGameToday.m_tmTodayDate;

    KP2P::GetInstance()->Send_OlympicTreeDropResultReq( kPacket );
}

void GCOlympicManager::SendOlympicBalloonResult( int iScore )
{
    KEVENT_OLYMPIC_BALLOON_RESULT_REQ kPacket;

    kPacket.m_nIncreasedData = iScore;
    kPacket.m_nMyAccumulateData = 0; 
    kPacket.m_nGameType = m_kGameToday.m_nGameType;
    kPacket.m_nTeamType = m_kGameToday.m_nTeamType;
    kPacket.m_tmTodayDate = m_kGameToday.m_tmTodayDate;

    KP2P::GetInstance()->Send_OlympicBalloonResultReq( kPacket );
}

void GCOlympicManager::OlympicStartGame( KEVENT_OLYMPIC_JOIN_GAME_ACK& kRecv_ )
{
    m_kGameToday = kRecv_;

    switch( m_kGameToday.m_nGameType ) {
        case KOlympicSchedule::OGM_TREEDROP :
            SiKGCMinigameManager()->SetAccumulatedScore( m_kGameToday.m_nMyAccumulateData );
            SiKGCMinigameManager()->StartGame( GC_GM_MINIGAME_TREEDROP );
            break;
        case KOlympicSchedule::OGM_BALLOON :
            SiKGCMinigameManager()->SetAccumulatedScore( m_kGameToday.m_nMyAccumulateData );
            SiKGCMinigameManager()->StartGame( GC_GM_MINIGAME_BALLOON );
            break;
        case KOlympicSchedule::OGM_WATERBOMB :
            g_pkUIScene->m_pkOlympicSongkranDlg->SetUserInfo( kRecv_ );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OLYMPIC_SONGKRAN_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        case KOlympicSchedule::OGM_GAWIBAWIBO :
            g_pkUIScene->m_pkOlympicGawibawiboDlg->SetGawibawiboGameInfo(kRecv_);
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OLYMPIC_GAWIBAWIBO_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        case KOlympicSchedule::OGM_DICEPLAY :
            g_pkUIScene->m_pkOlympicDicePlayDlg->SetMyBoardInfo( kRecv_ );
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OLYMPIC_DICE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            break;
        case KOlympicSchedule::OGM_BOSSGATE :
            break;
    }
}

void GCOlympicManager::OlympicQuitCurrentGame( void )
{
    switch( m_kGameToday.m_nGameType ) {
        case KOlympicSchedule::OGM_TREEDROP :
        case KOlympicSchedule::OGM_BALLOON :
            SiKGCMinigameManager()->QuitCurrentGame();
            break;
        case KOlympicSchedule::OGM_WATERBOMB :
            if( g_pkUIScene->m_pkOlympicSongkranDlg->IsRenderOn() )
                g_pkUIScene->m_pkOlympicSongkranDlg->Destroy();
            break;
        case KOlympicSchedule::OGM_GAWIBAWIBO :
            if( g_pkUIScene->m_pkOlympicGawibawiboDlg->IsRenderOn() )
                g_pkUIScene->m_pkOlympicGawibawiboDlg->Destroy();
            break;
        case KOlympicSchedule::OGM_DICEPLAY :
            if( g_pkUIScene->m_pkOlympicDicePlayDlg->IsRenderOn() )
                g_pkUIScene->m_pkOlympicDicePlayDlg->Destroy();
            break;
        case KOlympicSchedule::OGM_BOSSGATE :
            // 서버에서 처리
            break;
    }
}

// 미니게임 시작시 주는 보상을 끝나면 보여줍니다.
void GCOlympicManager::ShowMinigameReward()
{
    // 보상 박스 띄워주기 ( 미니게임 종료하면서 보여줌 )
    for( std::vector< KItem >::iterator vit = m_kGameToday.m_vecRewardItem.begin() ; vit != m_kGameToday.m_vecRewardItem.end() ; ++vit ) {
        GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
        if( NULL == pItem ) { 
            continue;
        }

        g_pItemMgr->m_kInventory.AddItem( *vit, true );
        std::wstring strMsg = g_pkStrLoader->GetReplacedString( STR_ID_ITEM_RECEIVE1, "l", pItem->strItemName );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX, strMsg, L"", KGCUIScene::GC_MBOX_USE_NORMAL, pItem->dwGoodsID * 10, vit->m_ItemUID, false, true );
    }
}

bool GCOlympicManager::IsOlympicNotice()
{
    // 플레이 가능한 게임이 있는 경우 알림
    // 받을 수 있는 보상이 있는 경우 알림
    if( m_bPlayEnable || m_bRewardNotice )
        return true;
    else
        return false;
}

void GCOlympicManager::Render()
{
   
    if(m_pOlympicEvent == NULL)
    {
        m_pOlympicEvent = g_pGCDeviceManager2->CreateTexture( "olympia.dds" );
    }

    if( m_bIsOlympicEvent == true )
    {
        if( g_MyD3D->m_pStateMachine->GetState() == GS_GUILD ||
            g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH  ||
            g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP ||
            g_MyD3D->m_pStateMachine->GetState() == GS_ROOM  ||
            g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO ||
            g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM ||
            g_MyD3D->m_pStateMachine->GetState() == GS_OPTION ||
            g_MyD3D->m_pStateMachine->GetState() == GS_SERVER || 
            g_MyD3D->m_pStateMachine->GetState() == GS_WORLDMAP ||
#if defined USE_COORDI_SHOP
            g_MyD3D->m_pStateMachine->GetState() == GS_COORDI_SHOP ||
#endif
            g_MyD3D->m_pStateMachine->GetState() == GS_MATCH  ) 

        {
            g_pGCDeviceManager2->DrawInScreen(m_pOlympicEvent, m_fX1Pos, -42.0f, m_fX1Pos + 128.0f, -42.0f + 128.0f + 10.0f, 0.0f, 0.0f, 1.0f, 1.0f);
            SetOlympicAlram();
        }
    }
    
}
void GCOlympicManager::SetOlympicAlram()
{
    if( m_fX1Pos >= 400.0f )
        m_bIsRight = false;
    else if( m_fX1Pos <= 1.0f )
        m_bIsRight = true;

    m_fX1Pos += ( (m_bIsRight) ? 0.1f: -0.1f);
}

void GCOlympicManager::SetOlympicEventAlram( bool bIsOlympicEvent )
{
    m_bIsOlympicEvent = bIsOlympicEvent;
}