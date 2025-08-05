#include "KGCGiftBoxManager.h"
#include "KGCWorldMapManager.h"

ImplementSingleton( KGCGiftBoxManager )

KGCGiftBoxManager::KGCGiftBoxManager(void)
: m_tmEventBegin(0)
, m_tmEventEnd(0)
, m_bNewUser(false)
, m_bEventUser(false)
, m_nNewUserCheckTime(0)
, m_tNewUserGoalTime(-1)
, m_nNewUserViewTime(-1)
, m_tEventGoalTime(-1)
, m_nEventViewTime(-1)
, m_bExistNewUserReward(true)
, m_bExistEventReward(true)
, m_iUserRewardItemID(0)
, m_bPossibleRewardNewUser(false)
, m_bPossibleRewardEvent(false)
{
    m_pairEventCheckTime = std::make_pair(0,0);
}

KGCGiftBoxManager::~KGCGiftBoxManager(void)
{
}

void KGCGiftBoxManager::Reset()
{
    m_tmEventBegin = 0;
    m_tmEventEnd = 0;
    m_bNewUser = false;
    m_bEventUser = false;
    m_nNewUserCheckTime = 0;
    m_tNewUserGoalTime = 0;
    m_nNewUserViewTime = -1;
    m_tEventGoalTime = -1;
    m_nEventViewTime = -1;
    m_bExistNewUserReward = false;
    m_bExistEventReward = false;
    m_iUserRewardItemID = 0;
    m_bPossibleRewardNewUser = false;
    m_bPossibleRewardEvent = false;
    m_pairEventCheckTime = std::make_pair(0,0);
    sNewUser.Init();
    sEventUser.Init();
}

bool KGCGiftBoxManager::IsEvent( int iEventType )
{
    if ( iEventType == 0 )
        return m_bNewUser;
    else if ( iEventType == 1)
        return m_bEventUser;

    return false;
}

void KGCGiftBoxManager::SetEvent( int iEventtype, bool bEvent )
{
    if ( iEventtype == 0 )
        m_bNewUser = bEvent;
    else if ( iEventtype == 1)
        m_bEventUser = bEvent;
}

void KGCGiftBoxManager::SetNewUserRemainTime( int nTime )
{
    time_t tmCurrentTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);
    m_tNewUserGoalTime = tmCurrentTime + ( (m_nNewUserCheckTime - nTime ) * 60);
}

int KGCGiftBoxManager::GetNewUserRemainTime()
{
    time_t tmCurrentTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);

    time_t tmTime = m_tNewUserGoalTime - tmCurrentTime;

    m_nNewUserViewTime = static_cast<int> ( tmTime - ( timeGetTime() - g_kGlobalValue.m_dwAccTimeNot ) / 1000) ;

    if ( m_nNewUserViewTime < 0 || m_tNewUserGoalTime <= tmCurrentTime)
        return 0;

    return m_nNewUserViewTime;
}

void KGCGiftBoxManager::SetEventRemainTime( int nTime )
{
    time_t tmCurrentTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);
    m_tEventGoalTime = tmCurrentTime + ( (m_pairEventCheckTime.first - nTime ) * 60);
}

int KGCGiftBoxManager::GetEventRemainTime()
{
    time_t tmCurrentTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);

    time_t tmTime = m_tEventGoalTime - tmCurrentTime;

    m_nEventViewTime = static_cast<int> ( tmTime - ( timeGetTime() - g_kGlobalValue.m_dwAccTimeNot ) / 1000) ;

    if ( m_nEventViewTime < 0 || m_tEventGoalTime <= tmCurrentTime)
        return 0;

    return m_nEventViewTime;
}

bool KGCGiftBoxManager::IsVeiwMode()
{
    GAME_STATE eGameState = g_MyD3D->m_pStateMachine->GetState();

    if( eGameState != GS_WORLDMAP &&
        eGameState != GS_MATCH &&
        eGameState != GS_SHOP_CASH &&
        eGameState != GS_SHOP_GP &&
#ifdef USE_COORDI_SHOP
        eGameState != GS_COORDI_SHOP &&
#endif
        eGameState != GS_MY_INFO ){
            return false;
    }
    return true;
}


void KGCGiftBoxManager::SetNewUserInfo( KEVENT_CONNECTION_GIFTBOX_NEWUSER_INFO_NOT& kNewUserInfo , bool bNewUser)
{
    sNewUser.m_vecGetRewards = kNewUserInfo.m_vecGetRewards;
    sNewUser.m_nAccPlayTime = kNewUserInfo.m_nAccPlayTime;

    if ( !kNewUserInfo.m_vecNextGift.empty() ) {
        sNewUser.m_vecNextGift = kNewUserInfo.m_vecNextGift;
        std::vector< std::pair< int, std::vector< KDropItemInfo > > >::iterator vit = kNewUserInfo.m_vecNextGift.begin();
        m_nNewUserCheckTime = vit->first;
    }

    SetNewUserRemainTime(sNewUser.m_nAccPlayTime);
    m_bNewUser = bNewUser;
}

void KGCGiftBoxManager::SetEventUserInfo( KEVENT_CONNECTION_GIFTBOX_EVENTUSER_INFO_NOT& kEventUserInfo , bool bEventUser)
{
    sEventUser.m_vecGetRewards = kEventUserInfo.m_vecGetRewards;
    sEventUser.m_nAccPlayTime = kEventUserInfo.m_nAccPlayTime;

    if ( !kEventUserInfo.m_vecNextGift.empty() ) {
        sEventUser.m_vecNextGift = kEventUserInfo.m_vecNextGift;
        std::vector< std::pair< int, std::vector< KDropItemInfo > > >::iterator vit = kEventUserInfo.m_vecNextGift.begin();
        
        int iCnt =0;
        for ( ; vit !=  kEventUserInfo.m_vecNextGift.end(); vit++)
        {
            if ( iCnt == 0 ) {
                m_pairEventCheckTime.first = vit->first;
                m_pairEventCheckTime.second = 0;
                iCnt++;
            }
            else if (iCnt == 1) {
                m_pairEventCheckTime.second = vit->first;
                iCnt++;
            }
        }
    }

    SetEventRemainTime(sEventUser.m_nAccPlayTime);
    m_bEventUser = bEventUser;
}

bool KGCGiftBoxManager::ExistNextGiftForEvent()
{
    if ( sEventUser.m_vecNextGift.size() < 2)
        return false;
    return true;
}

int KGCGiftBoxManager::CheckTime(int iEventType , bool bNextReward)
{
    if ( iEventType == 0 )
        return m_nNewUserCheckTime;
    else if ( iEventType == 1 && !bNextReward)
        return m_pairEventCheckTime.first;
    else if ( iEventType == 1 && bNextReward)
        return m_pairEventCheckTime.second;

    return -1;
}

void KGCGiftBoxManager::ExistNewUserReward( std::vector< std::pair< int, std::vector< KDropItemInfo > > >& vecNextReward , int iEventType)
{
    if (iEventType == 0) {
        m_bExistNewUserReward = vecNextReward.empty() ? false : true;
        SetEvent(iEventType, m_bExistNewUserReward);
    }
    else if (iEventType == 1) {
        m_bExistEventReward = vecNextReward.empty() ? false : true;
        SetEvent(iEventType, m_bExistEventReward);
    }
}

int KGCGiftBoxManager::GetNewUserRewardItemID( int iEventType , int iIndex)
{
    if ( iEventType == 0) {
        std::vector< std::pair< int, std::vector< KDropItemInfo > > >::iterator vit = sNewUser.m_vecNextGift.begin();
        int iSize = static_cast<int> ( sNewUser.m_vecNextGift.size() );
        if (vit != sNewUser.m_vecNextGift.end() && iSize > iIndex) {
            m_iUserRewardItemID = vit->second[0].m_ItemID;
        }
        else {
            m_iUserRewardItemID = 0;
        }
    }
    else if ( iEventType ==1 ) {
        std::vector< std::pair< int, std::vector< KDropItemInfo > > >::iterator vit = sEventUser.m_vecNextGift.begin();
        int iSize = static_cast<int> ( sEventUser.m_vecNextGift.size());
        if (vit != sEventUser.m_vecNextGift.end() && iSize > iIndex ) {
            m_iUserRewardItemID = vit->second[0].m_ItemID;
        }
        else {
            m_iUserRewardItemID = 0;
        }
    }

    return m_iUserRewardItemID;
}

void KGCGiftBoxManager::SetPossibleReward( int iEventType, bool bPossibleReward )
{
    if ( iEventType == 0 )
        m_bPossibleRewardNewUser = bPossibleReward;
    else if ( iEventType == 1)
        m_bPossibleRewardEvent = bPossibleReward;
}

bool KGCGiftBoxManager::GetPossibleReward( int iEventType)
{
    if ( iEventType == 0 )
        return m_bPossibleRewardNewUser;
    else if ( iEventType == 1)
        return m_bPossibleRewardEvent;

    return false;
}