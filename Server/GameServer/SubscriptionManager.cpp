#include "CnConnector.h"
#include "GSSimLayer.h"
#include "SubscriptionManager.h"
#include "UserEvent.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KSubscriptionManager );
ImplOstmOperatorW2A( KSubscriptionManager );
NiImplementRootRTTI( KSubscriptionManager );

ImplToStringW( KSubscriptionManager )
{
    KLocker lock( m_csSubscription );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapSubscriptionInfo.size() );
}

KSubscriptionManager::KSubscriptionManager(void)
{
}

KSubscriptionManager::~KSubscriptionManager(void)
{
}

void KSubscriptionManager::SetEventInfo( IN const std::map< int, KSubscription >& mapInfo_ )
{
    {
        KLocker lock( m_csSubscription );
        m_mapSubscriptionInfo = mapInfo_;
    }
    START_LOG( clog, L"응모시스템 정보갱신 완료. size : " << mapInfo_.size() ) << END_LOG;

    // 유저들에게 갱신된 정보 전달
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_SUBSCRIPTION_INFO_UPDATE_NOT, mapInfo_ );
}

void KSubscriptionManager::GetEventInfo( OUT std::map< int, KSubscription >&  mapInfo_ )
{
    KLocker lock( m_csSubscription );
    mapInfo_ = m_mapSubscriptionInfo;
}

bool KSubscriptionManager::SetCurrentGiftNum( IN const int nEventUID_, const int nNum_ )
{
    std::map< int, KSubscription >::iterator mit;
    {
        KLocker lock( m_csSubscription );
        mit = m_mapSubscriptionInfo.find( nEventUID_ );
        _JIF( mit != m_mapSubscriptionInfo.end(), return false );
        __JIF( ( mit->second.m_nCurrentGiftNum > nNum_ ), return false );

        mit->second.m_nCurrentGiftNum = nNum_;
    }
    START_LOG( clog, L"경품 수량 갱신됨. EventUID : " << nEventUID_ << L", CurrentNum : " << nNum_ ) << END_LOG;

    // 센터 서버로 전달
    {
        PAIR_INT kPacket;
        kPacket.first = nEventUID_;
        kPacket.second = nNum_;
        SiKCnConnector()->SendPacket( KCenterEvent::ECN_SUBSCRIPTION_CURRENT_GIFT_NUM_UPDATE_REQ, kPacket );
    }
    return true;
}

bool KSubscriptionManager::GetCurrentEvent( IN const int nEventUID_, OUT KSubscription& kEventInfo_ )
{
    std::map< int, KSubscription >::iterator mit;
    {
        KLocker lock( m_csSubscription );
        mit = m_mapSubscriptionInfo.find( nEventUID_ );
        _JIF( mit != m_mapSubscriptionInfo.end(), return false );
        kEventInfo_ = mit->second;
    }
    return true;
}

void KSubscriptionManager::GetCurrentGiftNum( OUT MAP_INT_INT& mapData_ )
{
    std::map< int, KSubscription >::const_iterator cmit;

    KLocker lock( m_csSubscription );
    for( cmit = m_mapSubscriptionInfo.begin() ; cmit != m_mapSubscriptionInfo.end() ; ++cmit )
    {
        mapData_[cmit->first] = cmit->second.m_nCurrentGiftNum;
    }
}

void KSubscriptionManager::DumpList()
{
    //std::map< int, KSubscription >::const_iterator cmit;
    //std::cout << L"================ START DUMP ================" << dbg::endl
    //    << L" m_mapSubscriptionInfo.size() : " << m_mapSubscriptionInfo.size() << dbg::endl;
    //{
    //    KLocker lock( m_csSubscription );
    //    for( cmit = m_mapSubscriptionInfo.begin() ; cmit != m_mapSubscriptionInfo.end() ; ++cmit )
    //    {
    //        std::cout << L" ------------------------------------"                 << dbg::endl
    //            << L" EventUID       : " << cmit->first                     << dbg::endl
    //            << L" TicketItemID   : " << cmit->second.m_dwTicketItemID   << dbg::endl
    //            << L" NeedTicketNum  : " << cmit->second.m_nNeedTicketNum   << dbg::endl
    //            << L" GiftItemID     : " << cmit->second.m_kRewardItem.m_ItemID     << dbg::endl
    //            << L" GiftCount      : " << cmit->second.m_kRewardItem.m_nDuration      << dbg::endl
    //            << L" GiftPeriod     : " << cmit->second.m_kRewardItem.m_nPeriod      << dbg::endl
    //            << L" MaxGiftNum     : " << cmit->second.m_dwMaxGiftNum     << dbg::endl
    //            << L" CurrentGiftNum : " << cmit->second.m_nCurrentGiftNum << dbg::endl;
    //    }
    //}
    //std::cout << L"================= END DUMP =================" << dbg::endl;
}