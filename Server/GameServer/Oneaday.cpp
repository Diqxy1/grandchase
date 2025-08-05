#include "CnConnector.h"
#include "Oneaday.h"
#include <dbg/dbg_log4.hpp>
#include <boost/bind.hpp>
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KOneaday );
ImplOstmOperatorW2A( KOneaday );
NiImplementRootRTTI( KOneaday );

KOneaday::KOneaday(void)
:m_kTimer( 1000 * 60 )
{

}

KOneaday::~KOneaday(void)
{}

ImplToStringW( KOneaday )
{
    KLocker lock( m_csOneaday );

    std::vector<KOneadayItem>::const_iterator vit;
    for ( vit = m_vecOneadayItemList.begin(); vit != m_vecOneadayItemList.end(); ++vit ) {
        stm_ << L" ItemID : " << vit->m_ItemID
            << L", Type : " << static_cast<int>(vit->m_SellType)
            << L", Current Count : " << vit->m_SellCurrentCount << std::endl;
    }

    return stm_;
}

void KOneaday::UpdateRequest()
{
    std::vector<KOneadayItem> kPacket;
    KLocker lock( m_csOneaday );
    kPacket = m_vecOneadayItemList;

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_ONEADAY_UPDATE_REQ, kPacket ) );

}

void KOneaday::GetInfo( OUT std::vector<KOneadayItem>& vecOneaday_ )
{
    KLocker lock( m_csOneaday );
    vecOneaday_ = m_vecOneadayItemList;
}

void KOneaday::SetInfo( OUT std::vector<KOneadayItem>& vecOneaday_ )
{
    KLocker lock( m_csOneaday );
    m_vecOneadayItemList = vecOneaday_;
}

void KOneaday::Tick()
{
    if ( m_kTimer.CheckTime() ) {
        UpdateRequest();
    }
}

void KOneaday::UpdateLimitedCount( GCITEMID itemID_ )
{
    KLocker lock( m_csOneaday );

    std::vector<KOneadayItem>::iterator vit;
    //vit = std::find_if( m_vecOneadayItemList.begin(), m_vecOneadayItemList.end(),
        //boost::bind( &KOneadayItem::m_ItemID, _1 ) == itemID_ );
    
    //if ( vit == m_vecOneadayItemList.end() ) {
        //return;
    //}
    //
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    
    for ( vit = m_vecOneadayItemList.begin(); vit != m_vecOneadayItemList.end(); ++vit ) {
        if ( vit->m_tStartDate < tmCurrent && vit->m_tEndDate > tmCurrent ) {
            vit->m_SellCurrentCount -= 1;
        }
    }

    
    
    if ( vit->m_tStartDate > tmCurrent || vit->m_tEndDate < tmCurrent ) {
        START_LOG( Info, L"원어데이 한정판매 수량감소에서 날짜가 안맞음" )
            << BUILD_LOG( (LPCTSTR)CTime(vit->m_tStartDate).Format( KNC_TIME_FORMAT ) )
            << BUILD_LOG( (LPCTSTR)CTime(vit->m_tEndDate).Format( KNC_TIME_FORMAT ) )
            << BUILD_LOG( (LPCTSTR)CTime(tmCurrent).Format( KNC_TIME_FORMAT ) ) << END_LOG;
        return;
    }

    

    START_LOG( Info, L"원어데이 한정판매 수량 감소" )
        << BUILD_LOG( itemID_ )
        << BUILD_LOG( vit->m_SellInitCount )
        << BUILD_LOG( vit->m_SellCurrentCount ) << END_LOG;

}

bool KOneaday::CheckItemRemainCount( IN GCITEMID itemID_ )
{
    KLocker lock( m_csOneaday );

    std::vector<KOneadayItem>::iterator vit;
    vit = std::find_if( m_vecOneadayItemList.begin(), m_vecOneadayItemList.end(),
        boost::bind( &KOneadayItem::m_ItemID, _1 ) == itemID_ );

    if ( vit == m_vecOneadayItemList.end() ) {
        return false;
    }

    if ( vit->m_SellType == KOneadayItem::LIMITED_COUNT && vit->m_SellCurrentCount <= 0 ) { // 수량이 남아있으면 ok
        return false;
    }

    return true;
}