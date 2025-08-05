#include "CenterSimLayer.h" // winsock2
#include "CnSubscriptionManager.h"
#include "CenterDBThread.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnSubscriptionManager );
ImplOstmOperatorW2A( KCnSubscriptionManager );
NiImplementRootRTTI( KCnSubscriptionManager );

ImplToStringW( KCnSubscriptionManager )
{
    KLocker lock( m_csCnSubscription );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapCnSubscriptionInfo.size() );
}

KCnSubscriptionManager::KCnSubscriptionManager(void)
:m_dwTickGap( 1000 * 60 * 5 )
{
    SetTick();
}

KCnSubscriptionManager::~KCnSubscriptionManager(void)
{
}

void KCnSubscriptionManager::Tick()
{
    if( ::GetTickCount() - GetTick() > GetTickGap() )
    {
        SendDBSubscriptionInfoUpdateReq();
        SetTick();
    }
}

void KCnSubscriptionManager::SendDBSubscriptionInfoUpdateReq()
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_SUBSCRIPTION_INFO_UPDATE_REQ, L"", 0 );
}

void KCnSubscriptionManager::UpdateEventInfoFromDB( IN const std::map< int, KSubscription >& mapInfo_ )
{
    {
        KLocker lock( m_csCnSubscription );
        if( m_mapCnSubscriptionInfo == mapInfo_ )
        {
            START_LOG( clog, L"응모시스템 정보 변경사항 없음." ) << END_LOG;
            return;
        }
        m_mapCnSubscriptionInfo = mapInfo_;
    }
    START_LOG( clog, L"응모시스템 정보갱신 완료. size : " << mapInfo_.size() ) << END_LOG;

    // 각 게임서버에 갱신 정보 알림.
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_SUBSCRIPTION_INFO_UPDATE_NOT, mapInfo_, true );
}

void KCnSubscriptionManager::GetEventInfo( OUT std::map< int, KSubscription >& mapInfo_ )
{
    KLocker lock( m_csCnSubscription );
    mapInfo_ = m_mapCnSubscriptionInfo;
}

bool KCnSubscriptionManager::UpdateCurrentGiftNum( IN const int nEventUID_, const int nNum_ )
{
    std::map< int, KSubscription >::iterator mit;
    {
        KLocker lock( m_csCnSubscription );
        mit = m_mapCnSubscriptionInfo.find( nEventUID_ );
        _JIF( mit != m_mapCnSubscriptionInfo.end(), return false );
        __JIF( ( mit->second.m_nCurrentGiftNum > nNum_ ), return false );

        mit->second.m_nCurrentGiftNum = nNum_;
    }
    START_LOG( clog, L"경품 수량 갱신됨. EventUID : " << nEventUID_ << L", CurrentNum : " << nNum_ ) << END_LOG;

    // 각 게임서버에 갱신 정보 알림.
    PAIR_INT kData;
    kData.first = nEventUID_;
    kData.second = nNum_;
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_ACK, kData );
    return true;
}

void KCnSubscriptionManager::DumpList()
{
    std::map< int, KSubscription >::const_iterator cmit;
    START_LOG( cerr, L"================ START DUMP ================" << dbg::endl
              << L" m_mapCnSubscriptionInfo.size() : " << m_mapCnSubscriptionInfo.size() ) << END_LOG;
    {
        KLocker lock( m_csCnSubscription );
        for( cmit = m_mapCnSubscriptionInfo.begin() ; cmit != m_mapCnSubscriptionInfo.end() ; ++cmit )
        {
            START_LOG( cerr, L" ------------------------------------"  << dbg::endl
                      << L" EventUID       : " << cmit->first                     << dbg::endl
                      << L" TicketItemID   : " << cmit->second.m_dwTicketItemID   << dbg::endl
                      << L" NeedTicketNum  : " << cmit->second.m_nNeedTicketNum   << dbg::endl
                      << L" GiftItemID     : " << cmit->second.m_kRewardItem.m_ItemID     << dbg::endl
                      << L" GiftCount      : " << cmit->second.m_kRewardItem.m_nDuration     << dbg::endl
                      << L" GiftPeriod     : " << cmit->second.m_kRewardItem.m_nPeriod     << dbg::endl
                      << L" MaxGiftNum     : " << cmit->second.m_dwMaxGiftNum     << dbg::endl
                      << L" CurrentGiftNum : " << cmit->second.m_nCurrentGiftNum << dbg::endl
                      << L" NPCID          : " << cmit->second.m_dwNPCID          ) << END_LOG;
        }
    }
    START_LOG( cerr, L"================= END DUMP ================="  ) << END_LOG;
}
