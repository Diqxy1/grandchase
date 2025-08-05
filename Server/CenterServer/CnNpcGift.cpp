#include "CenterSimLayer.h" // winsock2
#include "CnNpcGift.h"
#include "CenterDBThread.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnNpcGift );
ImplOstmOperatorW2A( KCnNpcGift );
NiImplementRootRTTI( KCnNpcGift );

ImplToStringW( KCnNpcGift )
{
    KLocker lock( m_csNpcGift );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapNpcGiftInfo.size() );
}

KCnNpcGift::KCnNpcGift(void)
:m_dwTickGap( 1000 * 60 * 5 )
{
    SetTick();
}

KCnNpcGift::~KCnNpcGift(void)
{
}

void KCnNpcGift::Tick()
{
    if( ::GetTickCount() - GetTick() > GetTickGap() )
    {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_NPC_GIFTS_UPDATE_REQ, L"", 0 );
        SetTick();
    }
}

void KCnNpcGift::UpdateEventInfo( IN const std::map<int, std::vector< KDropItemInfo > >& mapEventInfo_ )
{
    {
        KLocker lock( m_csNpcGift );
        if( m_mapNpcGiftInfo == mapEventInfo_ )
        {
            START_LOG( clog, L"NPC 선물 이벤트, 갱신된 사항 없음. size : " << mapEventInfo_ .size() ) << END_LOG;
            return;
        }

        // 정보갱신
        m_mapNpcGiftInfo = mapEventInfo_;
        START_LOG( clog, L"NPC 선물 이벤트, 갱신됨. size : " << mapEventInfo_.size() ) << END_LOG;
    }

    // 각 게임서버에 갱신 정보 알림.
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_NPC_GIFT_UPDATE_NOT, mapEventInfo_ );
}

void KCnNpcGift::GetEventInfo( OUT std::map<int, std::vector< KDropItemInfo > >& mapEventInfo_ )
{
    KLocker lock( m_csNpcGift );
    mapEventInfo_ = m_mapNpcGiftInfo;
}

void KCnNpcGift::DumpList()
{
    std::wstringstream stm;
    std::map<int, std::vector< KDropItemInfo > >::const_iterator cmit;
    std::vector< KDropItemInfo >::const_iterator cvit;

    KLocker lock( m_csNpcGift );

    stm << L"-- NPC Gifts Event List --" << std::endl;
    for( cmit = m_mapNpcGiftInfo.begin() ; cmit != m_mapNpcGiftInfo.end() ; ++cmit )
    {
        stm << L"EventID : "<< cmit->first << std::endl;
        stm << L"ItemList <ItemID,Count,Period>" << std::endl;
        for( cvit = cmit->second.begin() ; cvit != cmit->second.end() ; ++cvit )
        {
            stm << L"  <" << cvit->m_ItemID
                << L"," << cvit->m_nDuration
                << L"," << cvit->m_nPeriod
                << L">" << std::endl;
        }
        stm << std::endl;
    }
    stm << L"-- NPC Gifts Event --" << std::endl;
    START_LOG( cerr, stm.str() ) << END_LOG;
}