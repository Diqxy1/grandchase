#include "GWCSupport.h"
#include <dbg/dbg.hpp >
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG
ImplementSingleton( KGWCSupport );
ImplOstmOperatorW2A( KGWCSupport );
NiImplementRootRTTI( KGWCSupport );

KGWCSupport::KGWCSupport(void)
:m_bEventTerm(false)
,m_kTimer(60000)
{
}

KGWCSupport::~KGWCSupport(void)
{
}

ImplToStringW( KGWCSupport )
{
    DWORD dwInterval = m_kTimer.GetInterval();
    DWORD dwUserCount = GetUserCount();
    KLocker lock( m_csGWCData );
    return START_TOSTRINGW
        << TOSTRINGWb( m_bEventTerm )
        << TOSTRINGW( dwInterval )
        << TOSTRINGW( dwUserCount )
        << TOSTRINGW( m_kConfigData.m_dwItemID )
        << TOSTRINGW( m_kConfigData.m_nNationCode )
        << TOSTRINGWt( CTime(m_kConfigData.m_tmStart) )
        << TOSTRINGWt( CTime(m_kConfigData.m_tmFinish) )
        << TOSTRINGW( m_kConfigData.m_nDonationCount )
        << TOSTRINGW( m_kConfigData.m_vecRewardList.size() )
        << TOSTRINGW( m_vecGWCData.size() );
}

void KGWCSupport::Tick()
{
    if ( !m_kTimer.CheckTime() ) return;
    UpdateEventTerm();
}

bool KGWCSupport::IsEventTerm()
{
    KLocker lock( m_csGWCData );
    return m_bEventTerm;
}

void KGWCSupport::UpdateConfigData( IN const KGWCSupportConfigData& kConfigData_ )
{
    {
        KLocker lock( m_csGWCData );
        m_kConfigData = kConfigData_;
    }

    UpdateEventTerm();
}

void KGWCSupport::UpdateGWCData( IN std::vector<KGWCSupportData>& vecGWCData_ )
{
    KLocker lock( m_csGWCData );
    m_vecGWCData.swap( vecGWCData_ );
}

GCITEMID KGWCSupport::GetEventItem()
{
    KLocker lock( m_csGWCData );
    return m_kConfigData.m_dwItemID;
}

int KGWCSupport::GetNationCode()
{
    KLocker lock( m_csGWCData );
    return m_kConfigData.m_nNationCode;
}

bool KGWCSupport::AddUserData( IN const DWORD dwUID_, IN const int nAccCount_ )
{
    bool bRet = false;
    std::map<DWORD,int>::iterator mit;
    KLocker lock( m_csGWCUserData );
    mit = m_mapDonationData.find( dwUID_ );
    if ( mit != m_mapDonationData.end() )
    {
        mit->second = nAccCount_;
        bRet = false;
    }
    else {
        LIF( m_mapDonationData.insert( std::make_pair( dwUID_, nAccCount_ ) ).second );
        bRet = true;
    }

    return bRet;
}

void KGWCSupport::RemoveUserData( IN const DWORD dwUID_ )
{
    KLocker lock( m_csGWCUserData );
    m_mapDonationData.erase( dwUID_ );
}

int KGWCSupport::GetUserAccCount( IN const DWORD dwUID_ )
{
    std::map<DWORD,int>::iterator mit;
    KLocker lock( m_csGWCUserData );
    mit = m_mapDonationData.find( dwUID_ );

    if ( mit == m_mapDonationData.end() )
        return 0;

    return mit->second;
}

bool KGWCSupport::IncreaseUserAccCount( IN const DWORD dwUID_, IN const int nDiffCount_ )
{
    std::map<DWORD,int>::iterator mit;
    KLocker lock( m_csGWCUserData );
    mit = m_mapDonationData.find( dwUID_ );

    if ( mit == m_mapDonationData.end() )
        return false;

    mit->second += nDiffCount_;
    return true;
}

bool KGWCSupport::SetUserAccCount( IN const DWORD dwUID_, IN const int nAccCount_ )
{
    std::map<DWORD,int>::iterator mit;
    KLocker lock( m_csGWCUserData );
    mit = m_mapDonationData.find( dwUID_ );

    if ( mit == m_mapDonationData.end() )
        return false;

    mit->second = nAccCount_;
    return true;
}

void KGWCSupport::UpdateEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KLocker lock( m_csGWCData );
    m_bEventTerm = ( m_kConfigData.m_tmStart <= tmCurrent && m_kConfigData.m_tmFinish >= tmCurrent );
}

bool KGWCSupport::IsInUserData( IN const DWORD dwUID_ )
{
    KLocker lock( m_csGWCUserData );
    return ( m_mapDonationData.find( dwUID_ ) != m_mapDonationData.end() );
}

void KGWCSupport::GetGWCData( OUT std::vector<KGWCSupportData>& vecGWCData_ )
{
    KLocker lock( m_csGWCData );
    vecGWCData_ = m_vecGWCData;
}

void KGWCSupport::IncreaseNationAccCount( IN const int nCount_ )
{
    std::vector<KGWCSupportData>::iterator vit;
    KLocker lock( m_csGWCData );
    vit = std::find_if( m_vecGWCData.begin(), m_vecGWCData.end(),
        boost::bind( &KGWCSupportData::m_nNationCode, _1) == m_kConfigData.m_nNationCode );
    if ( vit != m_vecGWCData.end() ) {
        vit->m_nAccCount += nCount_;
    }
}

void KGWCSupport:: GetRewardItems( std::vector<KDropItemInfo>& vecRewards_ )
{
    KLocker lock(m_csGWCData);
    vecRewards_ = m_kConfigData.m_vecRewardList;
}