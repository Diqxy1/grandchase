#include "VirtualCash.h"
#include <dbg/dbg.hpp>
#include <algorithm>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KVirtualCash );
ImplOstmOperatorW2A( KVirtualCash );
NiImplementRootRTTI( KVirtualCash );

ImplToStringW( KVirtualCash )
{
    return START_TOSTRINGW
        << TOSTRINGW( m_nVCMixRatio );
}
KVirtualCash::KVirtualCash(void)
:m_nVCMixRatio( 0 )
{

}

KVirtualCash::~KVirtualCash(void)
{}

void KVirtualCash::UpdateRestrictItemList( IN std::vector<GCITEMID>& vecItemList_ )
{
    KLocker lock( m_csVirtualCash );
    m_vecRestrictItemList = vecItemList_;

    std::sort( m_vecRestrictItemList.begin(), m_vecRestrictItemList.end() );

    START_LOG( clog, L"가상캐쉬 혼용 구매불가 아이템 리스트" )
        << BUILD_LOG( m_vecRestrictItemList.size() ) << END_LOG;
}

bool KVirtualCash::FindRestrictItem( IN GCITEMID dwItemID_ )
{
    std::vector< GCITEMID >::iterator vit;

    bool bResult = std::binary_search( m_vecRestrictItemList.begin(), m_vecRestrictItemList.end(), dwItemID_ );
    
    if( bResult == true )
    {
        START_LOG( cwarn, L"가상캐쉬 혼용시에 구매할 수 없는 아이템 입니다" )
            << BUILD_LOG( dwItemID_ )
            << END_LOG;
        return bResult; // 구매할 수 없는 아이템 목록에 있으면
    }

    return bResult;
}

bool KVirtualCash::LimitVirtualCashRatio( DWORD dwItemPrice_, DWORD dwDeductPrice_ )
{
    float fRatioResult;
    if( dwDeductPrice_ != 0 && dwItemPrice_ != 0 )
    {
        fRatioResult = ( (float)dwDeductPrice_ / (float)dwItemPrice_ ) * 100;
        if( fRatioResult > (float)m_nVCMixRatio )
        {
            START_LOG( cerr, L"가상캐쉬 비율제한 걸림" )
                << BUILD_LOG( dwDeductPrice_ )
                << BUILD_LOG( dwItemPrice_ )
                << BUILD_LOG( m_nVCMixRatio )
                << BUILD_LOG( fRatioResult ) << END_LOG;

            return false;
        }
        else
        {
            START_LOG( clog, L"가상캐쉬 비율제한 통과" )
                << BUILD_LOG( dwDeductPrice_ )
                << BUILD_LOG( dwItemPrice_ )
                << BUILD_LOG( m_nVCMixRatio )
                << BUILD_LOG( fRatioResult ) << END_LOG;

            return true;
        }
    }

    return false;
}

void KVirtualCash::SetLimitRatio( IN int nLimitRatio_)
{
    KLocker lock(m_csVirtualCash); 
    m_nVCMixRatio = nLimitRatio_;
}