#include "LunaNewYear.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "CommonPacket.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KLunaNewYear );
ImplOstmOperatorW2A( KLunaNewYear );
NiImplementRootRTTI( KLunaNewYear );

KLunaNewYear::KLunaNewYear(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_bActive(false)
,m_dwTickGap(1000 * 60)
,m_nRewardGP(0)
{
    SetLastTick();
}

KLunaNewYear::~KLunaNewYear(void)
{
}

ImplToStringW( KLunaNewYear )
{
    KLocker lock( m_csLuna );
    START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWb( m_bActive )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_nRewardGP )
        << TOSTRINGW( m_vecRewardItems.size() );

    DumpRewardItem( stm_, m_vecRewardItems, L"RewardItem" );
    return stm_;
}

void KLunaNewYear::DumpRewardItem( std::wostream& stm_, const std::vector<KDropItemInfo>& kReward_, wchar_t* szName_ ) const
{
    stm_ << L"    " << szName_;
    std::vector<KDropItemInfo>::const_iterator vit;
    for( vit = kReward_.begin() ; vit != kReward_.end() ; ++vit )
    {
        stm_ << L"(" << vit->m_ItemID
            << L"," << vit->m_nDuration
            << L"," << vit->m_nPeriod
            << L"), ";
    }
    stm_ << std::endl;
}

bool KLunaNewYear::LoadScript()
{
    return _LoadScript(std::string());
}

bool KLunaNewYear::_LoadScript( OUT std::string& strScript_ )
{
    time_t  tmBegin;
    time_t  tmEnd;
    DWORD   dwTickGap;
    int     nRewardGP;
    std::vector<KDropItemInfo> vecRewardItems;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitLunaNewYear.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "RewardGP", nRewardGP ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    _JIF( LoadRewardItem( kLuaMng, std::string("RewardItem"), vecRewardItems ), return false );

    START_LOG( cerr, L" 세뱃돈 이벤트 읽기 완료.. " ) << END_LOG;

    KLocker lock( m_csLuna );
    m_tmBegin           = tmBegin;;
    m_tmEnd             = tmEnd;;
    m_dwTickGap         = dwTickGap;;
    m_nRewardGP         = nRewardGP;;
    m_vecRewardItems.swap( vecRewardItems );

    UpdateState();
    return true;
}

bool KLunaNewYear::LoadRewardItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_,
                                  OUT std::vector<KDropItemInfo>& vecRewardItems_ )
{
    vecRewardItems_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

         KDropItemInfo Data;
        _JIF( kLuaMng_.GetValue( 1, Data.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, Data.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, Data.m_nPeriod ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecRewardItems_.push_back( Data );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KLunaNewYear::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

void KLunaNewYear::Tick()
{
    if( ::GetTickCount() - GetLastTick() < GetTickGap() ) return;

    UpdateState();

    SetLastTick();
}

bool KLunaNewYear::GetReward( OUT int& nGPReward_, OUT std::vector<KDropItemInfo>& vecRewardItems_ )
{
    nGPReward_ = 0;
    vecRewardItems_.clear();
    {
        KLocker lock( m_csLuna );
        nGPReward_      = m_nRewardGP;
        vecRewardItems_ = m_vecRewardItems;
    }

    return ( nGPReward_ != 0 || !vecRewardItems_.empty() );
}

void KLunaNewYear::UpdateState()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KLocker lock( m_csLuna );

    if( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent )
        m_bActive = true;
    else
        m_bActive = false;
}