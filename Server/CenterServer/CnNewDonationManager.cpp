#include "CenterSimLayer.h" // winsock2
#include "CnNewDonationManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnNewDonationManager );
ImplOstmOperatorW2A( KCnNewDonationManager );
NiImplementRootRTTI( KCnNewDonationManager );

KCnNewDonationManager::KCnNewDonationManager(void)
:m_tmDBUpdateTime(0)
,m_dwTickGap(60*1000)
,m_tmBegin(1)
,m_tmEnd(1)
,m_dwMinTypeNum(0)
,m_dwMaxTypeNum(0)
{
    m_dwLastTick = ::GetTickCount();
    m_mapTotalDonation.clear();
}

KCnNewDonationManager::~KCnNewDonationManager(void)
{
}

ImplToStringW( KCnNewDonationManager )
{
    KLocker lock( m_csNewCnDonation );
    START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tmDBUpdateTime) )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwMinTypeNum )
        << TOSTRINGW( m_dwMaxTypeNum );
    DumpCurrentDonationState( stm_ );
    return stm_;
}

bool KCnNewDonationManager::LoadScript()
{
    KLuaManager kLuaMng;
    time_t      tmBegin;
    time_t      tmEnd;
    time_t      tmDBUpdateTime;
    DWORD       dwTickGap;
    DWORD       dwMinType;
    DWORD       dwMaxType;

    _JIF( kLuaMng.DoFile( "InitCnNewDonation.lua" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadUpdateTime( kLuaMng, std::string("DBUpdateTime"), tmDBUpdateTime ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MinTypeNum", dwMinType ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxTypeNum", dwMaxType ) == S_OK, return false );

    START_LOG( cerr, L"센터 신년 소원기부 스크립트 읽기 완료. " ) << END_LOG;
    KLocker lock( m_csNewCnDonation );
    m_tmDBUpdateTime    = tmDBUpdateTime;
    m_dwTickGap         = dwTickGap;
    m_tmBegin           = tmBegin;
    m_tmEnd             = tmEnd;
    m_dwMinTypeNum      = dwMinType;
    m_dwMaxTypeNum      = dwMaxType;

    return true;
}

bool KCnNewDonationManager::LoadUpdateTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
{
    int nHour, nMin;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, nHour ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, nMin ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    CTimeSpan ts(1,0,0,0); // 내일.

    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmUpdate( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), nHour, nMin, 0 );

    if( tmCurrent > tmUpdate )
        tmUpdate += ts;

    tmTime_ = KncUtil::TimeToInt( tmUpdate );
    return true;
}

bool KCnNewDonationManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

void KCnNewDonationManager::InitProcess()
{
    if( IsRun() )
    {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_GET_TOTAL_NEW_DONATION_REQ, L"", 0 );
    }
}

bool KCnNewDonationManager::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csNewCnDonation );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KCnNewDonationManager::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();
    if( dwCurrentTick - m_dwLastTick < m_dwTickGap ) return;

    m_dwLastTick = dwCurrentTick;

    if( ChkUpdateFromDB() && IsRun() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_GET_TOTAL_NEW_DONATION_REQ, L"", 0 );
    }
}

void KCnNewDonationManager::UpdateDeltaDonation( IN std::map<DWORD, int> mapDonationInfo_ )
{
    std::map<DWORD,int>::iterator mit;
    std::map<DWORD,int>::iterator mit_;
    KLocker lock(m_csNewCnDonation);
    if( !mapDonationInfo_.empty() ) {
        for( mit = m_mapTotalDonation.begin(); mit != m_mapTotalDonation.end(); ++mit ) {
            for( mit_ = mapDonationInfo_.begin(); mit_ != mapDonationInfo_.end(); ++mit_ ) {
                if( mit->first == mit_->first ) {
                    mit->second += mit_->second;
                }
            }
        }
    }
}

void KCnNewDonationManager::GetTotalDonation( OUT std::map<DWORD, int>& mapTotalDonation_ )
{
    KLocker lock(m_csNewCnDonation);
    mapTotalDonation_ = m_mapTotalDonation;
}

void KCnNewDonationManager::SetDonationFromDB( IN std::map< DWORD, int> mapTotalDonation_ )
{
    KLocker lock(m_csNewCnDonation);
    m_mapTotalDonation = mapTotalDonation_;
}

bool KCnNewDonationManager::ChkUpdateFromDB()
{
    KLocker lock( m_csNewCnDonation );
    bool bRet = false;
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmUpdate( m_tmDBUpdateTime );
    if( tmUpdate < tmCurrent )
    {
        CTimeSpan ts(1,0,0,0); // 내일.
        tmUpdate += ts;
        bRet = true;
        m_tmDBUpdateTime = KncUtil::TimeToInt( tmUpdate );
    }
    return bRet;
}

void KCnNewDonationManager::GetMinMaxType( OUT DWORD& dwMinType_, OUT DWORD& dwMaxType_ )
{
    KLocker lock( m_csNewCnDonation );
    dwMinType_ = m_dwMinTypeNum;
    dwMaxType_ = m_dwMaxTypeNum;
}

void KCnNewDonationManager::GetBeginTime( OUT time_t& tmDate_ )
{
    KLocker lock( m_csNewCnDonation );
    tmDate_ = m_tmBegin;
}

void KCnNewDonationManager::DumpCurrentDonationState( std::wostream& stm_ ) const 
{
    stm_ << L" --- NPC Donation Info ---- " << std::endl;
    std::map<DWORD, int>::const_iterator mit;
    for( mit = m_mapTotalDonation.begin(); mit != m_mapTotalDonation.end(); ++mit ) {
        stm_ << L"[" << mit->first
             << L"[" << mit->second
             << L"]" << std::endl;
    }
}