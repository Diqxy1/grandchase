#include "GSSimLayer.h"
#include "Rainbow.h"
#include <dbg/dbg.hpp>
#include "CommonPacket.h"
#include "CenterPacket.h"
#include "UserEvent.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KRainbow );
ImplOstmOperatorW2A( KRainbow );
NiImplementRootRTTI( KRainbow );

KRainbow::KRainbow(void)
:m_nRainbowID(-1)
,m_nDailyEventID(-1)
,m_nJackPotEventID(-1)
,m_bChanged(false)
{
    m_mapEventList.clear();
}

KRainbow::~KRainbow(void)
{
}

ImplToStringW( KRainbow )
{
    KLocker lock( m_csRainbow );
    START_TOSTRINGW
        << TOSTRINGW( m_nRainbowID )
        << TOSTRINGW( m_nDailyEventID )
        << TOSTRINGW( m_nJackPotEventID )
        << TOSTRINGWb( m_bChanged );
    DumpJackPotItems( stm_ );
    DumpEvents( stm_ );
    return stm_;
}

void KRainbow::UpdateFromCenter( IN KECN_RAINBOW_UPDATE_NOT& kPacket_ )
{
    // 이 함수가 호출 되었다면 무조건 변화가 있는것이다..
    KLocker lock(m_csRainbow);
    m_nRainbowID        = kPacket_.m_nRainBowID;
    m_nDailyEventID     = kPacket_.m_nDailyEventID;
    m_nJackPotEventID   = kPacket_.m_nJackPotEventID;

    m_mapEventList.clear();
    std::vector<KRainBowData>::const_iterator vit;
    for( vit = kPacket_.m_vecEventList.begin() ; vit != kPacket_.m_vecEventList.end() ; ++vit )
    {
        m_mapEventList.insert(std::make_pair(vit->m_nEventID, *vit) );
    }

    SetEventChanged( true );

    m_setReceivedUsers.clear();
}

bool KRainbow::GetEventData( OUT KRainBowEvent& kEventData_ )
{
    
    // 비어 있으면 않됨..
    if( !IsRainbowEventTerm() ) return false;

    KLocker lock(m_csRainbow);
    kEventData_.m_bJackPot = IsJackPotTime();
    kEventData_.m_vecEventData.clear();
    std::map<int, KRainBowData>::const_iterator mit;
    for( mit = m_mapEventList.begin() ; mit != m_mapEventList.end() ; ++mit )
    {
        kEventData_.m_vecEventData.push_back( mit->second );
    }
    return true;
}

bool KRainbow::IsEventChanged()
{
    KLocker lock(m_csRainbow);
    return m_bChanged;
}

void KRainbow::SetEventChanged( bool bChange_ )
{
    KLocker lock(m_csRainbow);
    m_bChanged = bChange_;
}

bool KRainbow::IsRainbowEventTerm()
{
    KLocker lock( m_csRainbow );
    return ( !m_mapEventList.empty() );
}

bool KRainbow::IsJackPotTime()
{
    KLocker lock(m_csRainbow);
    return (m_nJackPotEventID < 0 ? false : true);
}

int KRainbow::GetRainbowID()
{
    KLocker lock( m_csRainbow );
    return m_nRainbowID;
}

int KRainbow::GetDailyEventID()
{
    KLocker lock( m_csRainbow );
    return m_nDailyEventID;
}

int KRainbow::GetJackPotEventID()
{
    KLocker lock(m_csRainbow);
    return m_nJackPotEventID;
}

bool KRainbow::GetEventByID( IN const int nEventID_, OUT KRainBowData& kEvent_ )
{
    KLocker lock(m_csRainbow);
    std::map<int, KRainBowData>::const_iterator mit;
    mit = m_mapEventList.find( nEventID_ );
    if( mit == m_mapEventList.end() ) return false;
    kEvent_ = mit->second;
    return true;
}

void KRainbow::Tick()
{
    if( !IsEventChanged() ) return;

    SetEventChanged( false );
    KRainBowEvent kPacket;
    GetEventData( kPacket );
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_RAINBOW_EVENT_NOT, kPacket );

}

void KRainbow::AddItem( IN const GCITEMID dwItemID_, IN const int nCount_, IN const int nPeriod_ )
{
    KLocker lock(m_csRainbow);
    KDropItemInfo kItem;
    kItem.m_ItemID = dwItemID_;
    kItem.m_nDuration = nCount_;
    kItem.m_nPeriod = nPeriod_;
    m_vecJackPotItems.push_back( kItem );
}

bool KRainbow::GetJacPotItemList( OUT std::vector<KDropItemInfo>& vecItemList_ )
{
    KLocker lock(m_csRainbow);
    vecItemList_.clear();
    if( m_vecJackPotItems.empty() ) return false;
    vecItemList_ = m_vecJackPotItems;
    return true;
}

void KRainbow::DumpJackPotItems( std::wostream& stm_ ) const
{
    stm_ << L"--- Jack Pot Item List " << std::endl;
    std::vector<KDropItemInfo>::const_iterator vit;
    for( vit = m_vecJackPotItems.begin() ; vit != m_vecJackPotItems.end() ; ++vit )
    {
        stm_ << L"[" << vit->m_ItemID
            << ", " << vit->m_nDuration
            << ", " << vit->m_nPeriod
            << L"], ";
    }
    stm_ << std::endl;
}

void KRainbow::DumpEvents( std::wostream& stm_ ) const
{
    stm_ << L" ---- EventList ---- " << std::endl;
    std::map<int, KRainBowData>::const_iterator mit;
    for( mit= m_mapEventList.begin() ; mit!= m_mapEventList.end() ; ++mit)
    {
        stm_ << L"[" << mit->second.m_nEventID << L", " << mit->second.m_fPrimaryRatio << L", " << mit->second.m_fSecondaryRatio << "]" << std::endl;
    }
}

bool KRainbow::IsReceivedUser( IN DWORD dwUID_ )
{
    KLocker lock(m_csRainbow);
    return( m_setReceivedUsers.end() != m_setReceivedUsers.find( dwUID_ ) );
}

void KRainbow::InsertReceivedUser( IN DWORD dwUID_ )
{
    KLocker lock(m_csRainbow);
    m_setReceivedUsers.insert( dwUID_ );
}