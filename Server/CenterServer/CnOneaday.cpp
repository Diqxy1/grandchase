#include "CnOneaday.h"
#include <dbg/dbg_log4.hpp >
#include "CenterDBThread.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnOneaday );
ImplOstmOperatorW2A( KCnOneaday );
NiImplementRootRTTI( KCnOneaday );

KCnOneaday::KCnOneaday(void)
:m_kTimer( 60 * 1000 )
{

}

KCnOneaday::~KCnOneaday(void)
{}

ImplToStringW( KCnOneaday )
{
    KLocker lock( m_csOneaday );

    std::vector<KOneadayItem>::const_iterator vit;
    for ( vit = m_vecOneadayItemList.begin(); vit != m_vecOneadayItemList.end(); ++vit ) {
        stm_ << L" StartDate : " << (LPCTSTR)CTime(vit->m_tStartDate).Format( KNC_TIME_FORMAT )
            << L" EndDate : " << (LPCTSTR)CTime(vit->m_tEndDate).Format( KNC_TIME_FORMAT )
            << L" ItemID : " << vit->m_ItemID
            << L" No : " << static_cast<int>(vit->m_OneadayNo)
            << L", Type : " << static_cast<int>(vit->m_SellType)
            << L", Current Count : " << vit->m_SellCurrentCount 
            << L", Sale % : " << vit->m_DiscountPercent
            << std::endl;
    }

    return stm_;
}

void KCnOneaday::Init()
{
    QueuingToDB( true );
}

void KCnOneaday::SetItemList( std::vector<KOneadayItem>& vecOneadayItemList_ )
{
    KLocker lock( m_csOneaday );
    m_vecOneadayItemList = vecOneadayItemList_;
}

void KCnOneaday::GetItemList( std::vector<KOneadayItem>& vecOneadayItemList_ )
{
    KLocker lock( m_csOneaday );
    vecOneadayItemList_ = m_vecOneadayItemList;
}


void KCnOneaday::Tick()
{
    if ( m_kTimer.CheckTime() ) {
        QueuingToDB( false );
    }
}

void KCnOneaday::QueuingToDB( IN const bool bFirst_ )
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_ONEADAY_SELECT_REQ, L"", ( bFirst_?1:0 ) );
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ECN_ONEADAY_UPDATE_REQ, L"", 0, m_vecOneadayItemList );
}

void KCnOneaday::SetKItems( std::vector<KItem>& vecOneadayItem_ )
{
    KLocker lock( m_csOneaday );
    m_vecOneadayKItem = vecOneadayItem_;
}

void KCnOneaday::UpdateItemCount( IN std::vector<KOneadayItem>& vecOneadayItem_ )
{
    std::vector<KOneadayItem>::const_iterator vit;

    for ( vit = vecOneadayItem_.begin(); vit != vecOneadayItem_.end(); ++vit ) {
        if ( vit->m_SellType == KOneadayItem::LIMITED_COUNT ) {
            std::vector<KOneadayItem>::iterator vitFindResult;
            
            KLocker lock( m_csOneaday );
            vitFindResult = std::find_if( m_vecOneadayItemList.begin(), m_vecOneadayItemList.end(), 
                boost::bind(&KOneadayItem::m_ItemID,_1) == vit->m_ItemID );

            if ( vitFindResult != m_vecOneadayItemList.end() ) {
                vitFindResult->m_SellCurrentCount -= ( vitFindResult->m_SellInitCount - vit->m_SellCurrentCount );
                vitFindResult->m_SellInitCount = vitFindResult->m_SellCurrentCount;
            }
        }
    }
}