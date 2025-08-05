#include "GCChallengeErrandManager.h"

ImplementSingleton( GCChallengeErrandManager )

#define RandErrand(a, b) (rand()%(b-a+1)) + a

GCChallengeErrandManager::GCChallengeErrandManager(void)
{
    m_nPlayTime = 0;
    m_nDelayTime = 0;
    m_nTimeIncreaseRate = 0;
    m_nMaxStage = 0;

    m_iSyncPlayTime = 0;
    m_iRemainTime = 0;
    m_bPlayable = false;

    m_bIsGetInfo = false;


    m_mapErrandStageInfo.clear();
    m_vecRewardItemForDisplay.clear();
    m_vecRewardItem.clear();
    

    InitStageInfo();

}

GCChallengeErrandManager::~GCChallengeErrandManager(void)
{
}

void GCChallengeErrandManager::InitStageInfo( void )
{
    m_nStageNum             = 0;
    m_nBuyListMinCount      = 0;
    m_nBuyListMaxCount      = 0;

    m_MerryEmergenceTime = 0;

    m_buyItemList.clear();
    m_vecBuyInfoList.clear();

}
void GCChallengeErrandManager::SetErrandInfo( KEVENT_ERRAND_INIT_INFO_ACK& kRecv )
{
    m_nPlayTime = kRecv.m_nPlayTime / 1000;
    m_nDelayTime = kRecv.m_nDelayTime;
    m_nTimeIncreaseRate = kRecv.m_nTimeIncreaseRate;
    m_nMaxStage = kRecv.m_nMaxStage;
    m_nMarryAppearInterval = kRecv.m_nMarryAppearInterval / 1000;
    m_nMarryKeepAliveTime = kRecv.m_nMarryKeepAliveTime / 1000;

    m_mapErrandStageInfo = kRecv.m_mapErrandStageInfo;
    m_vecRewardItemForDisplay = kRecv.m_vecRewardItemForDisplay;

    m_bIsGetInfo = true;
}

void GCChallengeErrandManager::SetRewardItem( KEVENT_ERRAND_REWARD_ACK& kRecv )
{
    m_vecRewardItem.clear();
    m_vecSpecialRewardItem.clear();

    if( !kRecv.m_vecRewardItem.empty() )
    {
        m_vecRewardItem = kRecv.m_vecRewardItem;
        std::vector< KItem >::iterator vit = kRecv.m_vecRewardItem.begin();
        while( vit != kRecv.m_vecRewardItem.end() )
        {
            GCItem* pItem = g_pItemMgr->GetItemData( vit->m_ItemID / 10 );
            if( NULL == pItem ) { 
                continue;
            }
            g_pItemMgr->m_kInventory.AddItem( *vit, true );
            ++vit;
        }

    }

    if( !kRecv.m_vecSpecialRewardItem.empty() )
    {
        m_vecSpecialRewardItem = kRecv.m_vecSpecialRewardItem;
        std::vector< KItem >::iterator itor = kRecv.m_vecSpecialRewardItem.begin();
        while( itor != kRecv.m_vecSpecialRewardItem.end() )
        {
            GCItem* pItem = g_pItemMgr->GetItemData( itor->m_ItemID / 10 );
            if( NULL == pItem ) { 
                continue;
            }
            g_pItemMgr->m_kInventory.AddItem( *itor, true );
            ++itor;
        }
    }
}

void GCChallengeErrandManager::SetEndGame( KEVENT_ERRAND_GAME_END_ACK& kRecv )
{
    int nInitRemainTime = m_nDelayTime;
    SetLastRewardTime(  nInitRemainTime );
    m_bPlayable = kRecv.m_bPlayAble;
}

void GCChallengeErrandManager::SetStageInfo( int iStage )
{
    std::map<int, KErrandStageInfo>::iterator pos = m_mapErrandStageInfo.find( iStage );

    if( pos == m_mapErrandStageInfo.end() )
        return;

    KErrandStageInfo& kStageInfo = pos->second;

    InitStageInfo();
    m_nStageNum             = kStageInfo.m_nStageNum;
    m_nBuyListMinCount      = kStageInfo.m_nBuyListMinCount;
    m_nBuyListMaxCount      = kStageInfo.m_nBuyListMaxCount;
    m_vecBuyInfoList        = kStageInfo.m_vecBuyInfoList;
    StageRandomInfo();

}

void GCChallengeErrandManager::StageRandomInfo()
{
    if( m_vecBuyInfoList.empty() )
        return;

    std::random_shuffle( m_vecBuyInfoList.begin(), m_vecBuyInfoList.end() );
    int iBuyListCnt = RandErrand( m_nBuyListMinCount, m_nBuyListMaxCount );

    for( int i=0; i < iBuyListCnt; i++ )
    {
        ErrandItemBuyList eBuyList;
        eBuyList.m_nType = m_vecBuyInfoList[i].m_nType;
        eBuyList.m_nItemNum = RandErrand( m_vecBuyInfoList[i].m_nMinCount, m_vecBuyInfoList[i].m_nMaxCount );

        m_buyItemList.push_back( eBuyList );
    }
}

void GCChallengeErrandManager::SetLastRewardTime( int iRemainTime )
{
    m_iRemainTime = iRemainTime / 1000;
    m_iSyncPlayTime = timeGetTime() / 1000;
}

int GCChallengeErrandManager::GetLastRewardTime( )
{

    int nCurrentTime = timeGetTime() / 1000;
    int nLastTime = nCurrentTime - m_iSyncPlayTime;
    if( m_iRemainTime > 0 )
        m_iRemainTime -= nLastTime;
    m_iSyncPlayTime = timeGetTime() / 1000;

    return m_iRemainTime;
}

void GCChallengeErrandManager::SetPlayable( bool bPlayable )
{
    m_bPlayable = bPlayable;
}