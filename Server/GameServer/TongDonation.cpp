#include "TongDonation.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include <boost/bind.hpp>
#include "EventType.h"

ImplementSingleton( KTongDonation );
ImplOstmOperatorW2A( KTongDonation );
NiImplementRootRTTI( KTongDonation );

ImplToStringW( KTongDonation )
{
    KLocker lock( m_csTongDonation );

    return START_TOSTRINGW
        << TOSTRINGWt( CTime( m_tmVersion ) )
        << TOSTRINGWt( CTime( m_tmBegin ) )
        << TOSTRINGWt( CTime( m_tmEnd ) )
        << TOSTRINGW( m_dwClientUIType )
        << TOSTRINGW( m_mapNPCs.size() )
        << TOSTRINGW( m_mapTotalAmount.size() );
}

KTongDonation::KTongDonation( void )
: m_tmVersion( 0 )
, m_tmBegin( 0 )
, m_tmEnd( 0 )
, m_dwClientUIType( 0 )
{
    m_mapNPCs.clear();
    m_mapTotalAmount.clear();
    m_mapEventType.clear();
    InitEventType();
}

KTongDonation::~KTongDonation( void )
{
}

bool KTongDonation::SetFromTongDonationInfoNot( IN const KECN_TONG_DONATION_INFO_NOT& kPacket_ )
{
    KLocker lock( m_csTongDonation );

    m_tmVersion         = kPacket_.m_tmVersion;
    m_tmBegin           = kPacket_.m_tmBegin;
    m_tmEnd             = kPacket_.m_tmEnd;
    m_dwClientUIType    = kPacket_.m_dwClientUIType;
    m_mapNPCs           = kPacket_.m_mapNPCs;

    return true;
}

DWORD KTongDonation::GetTotalAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_ )
{
    // 여러 스레드에서 동시에 접근할 가능성이 낮으나 안전을 위해 락을 둔다
    KLocker lock( m_csTongDonation );

    return m_mapTotalAmount[ tmVersion_ ][ dwNPCType_ ];
}

void KTongDonation::SetTotalAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_, IN const DWORD dwAmount_ )
{
    // 여러 스레드에서 동시에 접근할 가능성이 낮으나 안전을 위해 락을 둔다
    KLocker lock( m_csTongDonation );

    m_mapTotalAmount[ tmVersion_ ][ dwNPCType_ ] = dwAmount_;
}

bool KTongDonation::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csTongDonation );

    return ( m_tmBegin <= tmCurrent && tmCurrent <= m_tmEnd );
}

bool KTongDonation::GetTongDonationNPCInfo( IN const DWORD dwNPCType_, OUT KTongDonationNPCInfo& kTongDonationNPCInfo_ )
{
    KLocker lock( m_csTongDonation );

    std::map< DWORD, KTongDonationNPCInfo >::iterator mitNPC;
    mitNPC = m_mapNPCs.find( dwNPCType_ );
    if ( mitNPC == m_mapNPCs.end() ) {
        return false;
    }

    kTongDonationNPCInfo_ = mitNPC->second;

    return true;
}

DWORD KTongDonation::GetClientUIType()
{
    KLocker lock( m_csTongDonation );

    return m_dwClientUIType;
}

time_t KTongDonation::GetCurrentVersion()
{
    KLocker lock( m_csTongDonation );

    return m_tmVersion;
}

int KTongDonation::GetEventType( IN const DWORD dwNPCType_ )
{
    // 실시간으로 변경되지 않는 정보이므로 락은 걸지 않는다
    return m_mapEventType[ dwNPCType_ ];
}

void KTongDonation::InitEventType()
{
    // 실시간으로 변경할 정보가 아니므로 락은 걸지 않는다
    m_mapEventType.clear();

    m_mapEventType[ 1 ] = UET_TONG_DONATION_1;
    m_mapEventType[ 2 ] = UET_TONG_DONATION_2;
    m_mapEventType[ 3 ] = UET_TONG_DONATION_3;
    m_mapEventType[ 4 ] = UET_TONG_DONATION_4;
    m_mapEventType[ 5 ] = UET_TONG_DONATION_5;
    m_mapEventType[ 6 ] = UET_TONG_DONATION_6;
    m_mapEventType[ 7 ] = UET_TONG_DONATION_7;
    m_mapEventType[ 8 ] = UET_TONG_DONATION_8;
    m_mapEventType[ 9 ] = UET_TONG_DONATION_9;
    m_mapEventType[ 10 ] = UET_TONG_DONATION_10;
}

bool KTongDonation::GetNPCTypes( OUT std::vector< DWORD >& vecNPCType_ )
{
    vecNPCType_.clear();

    KLocker lock( m_csTongDonation );

    std::map< DWORD, KTongDonationNPCInfo >::iterator mitNPCs = m_mapNPCs.begin();
    for( ; mitNPCs != m_mapNPCs.end(); ++mitNPCs ) {
        vecNPCType_.push_back( mitNPCs->first );
    }

    if ( true == vecNPCType_.empty() ) {
        return false;
    }

    return true;
}
