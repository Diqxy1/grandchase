#include "User.h"
#include "NpcGift.h"
#include "GSSimLayer.h"
#include "GSDBLayer.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KNpcGift );
ImplOstmOperatorW2A( KNpcGift );
NiImplementRootRTTI( KNpcGift );

ImplToStringW( KNpcGift )
{
    KLocker lock( m_csNpcGift );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapNpcGiftInfo.size() )
        << TOSTRINGW( m_setNpcGiftEvent.size() );
}

KNpcGift::KNpcGift(void)
{
}

KNpcGift::~KNpcGift(void)
{
}

void KNpcGift::UpdateEventInfo( IN const std::map<int, std::vector< KDropItemInfo > >& mapEventInfo_ )
{
    std::set<int> setEventList;
    std::map<int, std::vector< KDropItemInfo > >::const_iterator cmit;

    for( cmit = mapEventInfo_.begin() ; cmit != mapEventInfo_.end() ; ++cmit )
    {
        setEventList.insert( cmit->first );
    }

    // 정보갱신
    {
        KLocker lock( m_csNpcGift );
        m_setNpcGiftEvent = setEventList;
        m_mapNpcGiftInfo = mapEventInfo_;
    }

    START_LOG( clog, L"NPC 선물 이벤트, 갱신됨. size : " << setEventList.size() << L", " << mapEventInfo_.size() ) << END_LOG;

    // 갱신 정보 클라이언트에 알림.
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_NPC_GIFTS_NOT, setEventList );
}

bool KNpcGift::GetEventItemList( IN const int nEventID_, OUT std::vector< KDropItemInfo >& vecItemList_ )
{
    KLocker lock( m_csNpcGift );

    std::map<int, std::vector< KDropItemInfo > >::const_iterator cmit;
    cmit = m_mapNpcGiftInfo.find( nEventID_ );
    _JIF( cmit != m_mapNpcGiftInfo.end(), return false );

    vecItemList_ = cmit->second;
    return true;
}

void KNpcGift::GetEventList( OUT std::set< int >& setEventList_ )
{
    KLocker lock( m_csNpcGift );
    setEventList_ = m_setNpcGiftEvent;
}

void KNpcGift::DumpList()
{
    //std::wstringstream stm;
    //std::map<int, std::vector< KDropItemInfo > >::const_iterator cmit;
    //std::vector< KDropItemInfo >::const_iterator cvit;

    //KLocker lock( m_csNpcGift );

    //stm << L"-- NPC Gifts Event List --" << std::endl;
    //for( cmit = m_mapNpcGiftInfo.begin() ; cmit != m_mapNpcGiftInfo.end() ; ++cmit )
    //{
    //    stm << L"EventID : "<< cmit->first << std::endl;
    //    stm << L"ItemList <ItemID,Factor>" << std::endl;
    //    for( cvit = cmit->second.begin() ; cvit != cmit->second.end() ; ++cvit )
    //    {
    //        stm << L"  <" << cvit->m_ItemID
    //            << L"," << cvit->m_nDuration
    //            << L"," << cvit->m_nPeriod
    //            << L">" << std::endl;
    //    }
    //    stm << std::endl;
    //}
    //stm << L"-- NPC Gifts Event --" << std::endl;
    //std::cout2 << stm.str();
}