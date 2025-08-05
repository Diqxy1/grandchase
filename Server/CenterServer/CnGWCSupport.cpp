#include "CenterSimLayer.h"
#include "CnGWCSupport.h"
#include <dbg/dbg.hpp >
#include "lua/KLuaManager.h"
#include "CenterDBThread.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG
ImplementSingleton( KCnGWCSupport );
ImplOstmOperatorW2A( KCnGWCSupport );
NiImplementRootRTTI( KCnGWCSupport );

KCnGWCSupport::KCnGWCSupport(void)
:m_kTimer(60000)
,m_bConfigChanged(false)
,m_bDataChanged(false)
{
}

KCnGWCSupport::~KCnGWCSupport(void)
{
}

ImplToStringW( KCnGWCSupport )
{
    DWORD dwInterval = m_kTimer.GetInterval();
    KLocker lock( m_csGWCData );
    return START_TOSTRINGW
        << TOSTRINGWb( m_bConfigChanged )
        << TOSTRINGWb( m_bDataChanged )
        << TOSTRINGW( dwInterval )
        << TOSTRINGW( m_kConfigData.m_dwItemID )
        << TOSTRINGW( m_kConfigData.m_nNationCode )
        << TOSTRINGWt( CTime(m_kConfigData.m_tmStart) )
        << TOSTRINGWt( CTime(m_kConfigData.m_tmFinish) )
        << TOSTRINGW( m_kConfigData.m_nDonationCount )
        << TOSTRINGW( m_kConfigData.m_vecRewardList.size() )
        << TOSTRINGW( m_vecGWCData.size() );
}

bool KCnGWCSupport::LoadScript()
{
    KGWCSupportConfigData kConfigData;
    DWORD dwTickGap = 60000;
    KLuaManager luaMng;
    _JIF( luaMng.DoFile( "InitGWCConfig.lua" ) == S_OK, return false );
    _JIF( LoadTime( luaMng, std::string("BeginTime"), kConfigData.m_tmStart ), return false );
    _JIF( LoadTime( luaMng, std::string("EndTime"), kConfigData.m_tmFinish ), return false );
    _JIF( luaMng.GetValue( "ItemID", kConfigData.m_dwItemID ) == S_OK, return false );
    _JIF( luaMng.GetValue( "NationCode", kConfigData.m_nNationCode ) == S_OK, return false );
    _JIF( luaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( luaMng.GetValue( "DonationCount", kConfigData.m_nDonationCount ) == S_OK, return false );
    _JIF( LoadRewardList( luaMng, kConfigData.m_vecRewardList ), return false );

    {
        KLocker lock( m_csGWCData );
        std::swap( m_kConfigData, kConfigData );
        m_kTimer.SetInterval( dwTickGap );
    }
    
    SetConfigChanged( true );
    return true;
}

void KCnGWCSupport::Tick()
{
    if ( IsConfigChanged() )
        SendConfigData();

    if ( IsDataChanged() )
        SendGWCData();

    IntervalTick();
}

void KCnGWCSupport::UpdateDataFromDB( IN std::vector<KGWCSupportData>& vecGWCData )
{
    bool bChanged = false;
    {
        KLocker lock( m_csGWCData );
        if ( vecGWCData != m_vecGWCData )
        {
            bChanged = true;
            m_vecGWCData.swap( vecGWCData );
        }
    }

    if ( bChanged )
        SetDataChanged( true );
}

void KCnGWCSupport::SendConfigData()
{
    KGWCSupportConfigData kPacket;
    GetConfigData( kPacket );
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GWC_CONFIG_DATA_NOT, kPacket, true );
}

void KCnGWCSupport::SendGWCData()
{
    std::vector<KGWCSupportData> kPacket;
    GetGWCData( kPacket );
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GWC_DATA_NOT, kPacket, true );
}

bool KCnGWCSupport::IsConfigChanged()
{
    KLocker lock( m_csGWCData );
    if ( m_bConfigChanged ) {
        m_bConfigChanged = false;
        return true;
    }
    return false;
}

void KCnGWCSupport::SetConfigChanged( IN const bool bChange )
{
    KLocker lock( m_csGWCData );
    m_bConfigChanged = bChange;
}

bool KCnGWCSupport::IsDataChanged()
{
    KLocker lock( m_csGWCData );
    if ( m_bDataChanged ) {
        m_bDataChanged = false;
        return true;
    }
    return false;
}

void KCnGWCSupport::SetDataChanged( IN const bool bChange )
{
    KLocker lock( m_csGWCData );
    m_bDataChanged = bChange;
}

bool KCnGWCSupport::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nYear, nMonth, nDay, nHour;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
    return true;
}

void KCnGWCSupport::GetConfigData( OUT KGWCSupportConfigData& kConfig_ )
{
    KLocker lock( m_csGWCData );
    kConfig_ = m_kConfigData;
}

void KCnGWCSupport::GetGWCData( OUT std::vector<KGWCSupportData>& vecGWCData_ )
{
    KLocker lock( m_csGWCData );
    vecGWCData_ = m_vecGWCData;
}

void KCnGWCSupport::QueueingToDB()
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_UPDATE_GWC_DATA, L"", 0 );
}

bool KCnGWCSupport::IsEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KLocker lock( m_csGWCData );

    return (m_kConfigData.m_tmStart <= tmCurrent && m_kConfigData.m_tmFinish >= tmCurrent);
}

void KCnGWCSupport::IntervalTick()
{
    if ( !m_kTimer.CheckTime() ) return;

    if ( !IsEventTerm() ) return;

    QueueingToDB();
}

bool KCnGWCSupport::LoadRewardList( IN KLuaManager& luaMng_, OUT std::vector< KDropItemInfo >& vecRewardList_ )
{
    vecRewardList_.clear();
    _JIF( luaMng_.BeginTable( "RewardItem" ) == S_OK, return false );
    int i = 0;
    while ( true ) {
        KDropItemInfo kData;
        if ( luaMng_.GetValue( ++i, kData.m_ItemID ) != S_OK )
            break;
        _JIF( luaMng_.GetValue( ++i, kData.m_nDuration ) == S_OK, return false );
        _JIF( luaMng_.GetValue( ++i, kData.m_nPeriod ) == S_OK, return false );
        vecRewardList_.push_back( kData );
    }
    _JIF( luaMng_.EndTable() == S_OK, return false );

    return true;
}