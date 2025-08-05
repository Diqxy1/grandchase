#include "StatGP.h"
#include "UserEvent.h"
#include "Worker.h"
#include "ThreadManager.h"
#include "GSDBLayer.h"
#include "Log4.h"

KStatGP::KStatGP(void)
:m_dwTickGap(1000*60)
{
    m_dwLastTick = ::GetTickCount();
    m_mapLevelGPInfo.clear();
}

KStatGP::~KStatGP(void)
{
}

void KStatGP::Init( int nID_, int nFlushGap_, bool bWriteToDB_, int nRowCount_ )
{
    KLocker lock( m_csStat );
    KStatistics::Init( nID_, nFlushGap_, bWriteToDB_, nRowCount_ );
}

void KStatGP::Tick()
{
    if( !OneMinuteCheck() ) {
        return;
    }

    SendLevelGPInfo();

    if ( !DayCheck() ) {
        return;
    }

    Flush();
    ResetTime();
}

void KStatGP::Dump( std::wostream& stm_ )
{
    KLocker lock( m_csStat );
    KStatistics::Dump( stm_ );
    stm_ << L" Data Size : " << m_mapGpInc.size() << L", " << m_mapGpDec.size() << std::endl;
}

void KStatGP::Flush()
{
    KGpLog kData;
    kData.m_strGpIncLog.clear();
    kData.m_strGpDecLog.clear();

    KLocker lock( m_csStat );

    if( !m_mapGpInc.empty() ) {
        std::wstringstream stm;
        stm << KNC_TIME_STRING << L"\n";

        MAP_GP_STAT::iterator mitType;
        for ( mitType = m_mapGpInc.begin() ; mitType != m_mapGpInc.end() ; ++mitType ) {

            std::map<KGpStatKey,KGpStatVal>::iterator mitMode;
            for ( mitMode = mitType->second.begin() ; mitMode != mitType->second.end() ; ++mitMode ) {
                stm << mitMode->first.Str() << L" | " << mitMode->second.Str() << L"\n";
            }
        }
        m_mapGpInc.clear();
        kData.m_strGpIncLog = stm.str();

        stm.clear();
        stm.str(L"");
    }

    if( !m_mapGpDec.empty() ) {
        std::wstringstream stm;
        stm << KNC_TIME_STRING << L"\n";

        MAP_GP_STAT::iterator mitType;
        for ( mitType = m_mapGpDec.begin() ; mitType != m_mapGpDec.end() ; ++mitType ) {

            std::map<KGpStatKey,KGpStatVal>::iterator mitMode;
            for ( mitMode = mitType->second.begin() ; mitMode != mitType->second.end() ; ++mitMode ) {
                stm << mitMode->first.Str() << L" | " << mitMode->second.Str() << L"\n";
            }
        }
        m_mapGpDec.clear();
        kData.m_strGpDecLog = stm.str();

        stm.clear();
        stm.str(L"");
    }

    KTThreadManager<KWorker>::GetInstance()->QueueingEvent( KUserEvent::EVENT_GP_STAT_LOG, L"", 0, 0, kData );
}

void KStatGP::AddStat( IN const int& nStatType_, IN const KGpStatInfo& kData_ )
{
    std::map<int, std::pair<__int64, __int64>>::iterator mit;
    KGpStatKey kKey;
    kKey.m_nModeID = kData_.m_nModeID;
    kKey.m_nCharType = kData_.m_nCharType;
    kKey.m_nLv = kData_.m_nLv;
    kKey.m_nPromotion = kData_.m_nPromotion;

    KLocker lock( m_csStat );

    switch ( nStatType_ ) {
    case KGpStatInfo::ST_GP_INC:
        m_mapGpInc[kData_.m_nType][kKey].AddStat( std::max<__int64>( kData_.m_biGP, 0 ) );
        break;
    case KGpStatInfo::ST_GP_DEC:
        m_mapGpDec[kData_.m_nType][kKey].AddStat( std::max<__int64>( kData_.m_biGP, 0 ) );
        break;
    default:
        START_LOG( cwarn, L"정의되지않은 통계타입 : " << nStatType_  ) << END_LOG;
        break;
    }

    mit = m_mapLevelGPInfo.find( kData_.m_nLv );
    std::pair<__int64,__int64> prData;
    prData.first = 0;
    prData.second = 0;
    if( mit == m_mapLevelGPInfo.end() ) {
        if( nStatType_ == KGpStatInfo::ST_GP_INC ) {
            prData.first = kData_.m_biGP;
        }
        else {
            prData.second = kData_.m_biGP;
        }
        m_mapLevelGPInfo[kData_.m_nLv] = prData;
    }
    else {
        if( nStatType_ == KGpStatInfo::ST_GP_INC ) {
            mit->second.first += kData_.m_biGP;
        }
        else {
            mit->second.second += kData_.m_biGP;
        }
    }
}

bool KStatGP::DayCheck()
{
    CTime tDate = CTime::GetCurrentTime();
    KSimpleDate kToday( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );

    KLocker lock( m_csStat );
    if ( m_kLastDayCheckDate == kToday ) {
        return false;
    }

    m_kLastDayCheckDate.SetDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
    START_LOG( clog, L"Set LastDay Date : " << m_kLastDayCheckDate.Str() ) << END_LOG;
    return true;
}

bool KStatGP::OneMinuteCheck()
{
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }
    m_dwLastTick = ::GetTickCount();
    return true;
}

void KStatGP::SendLevelGPInfo()
{
    std::map<int, std::pair<__int64, __int64>> mapLevelGPInfo;
    mapLevelGPInfo.clear();

    { // Lock 최소화.
        KLocker lock( m_csStat );
        if( m_mapLevelGPInfo.empty() ) {
            return;
        }
        mapLevelGPInfo.swap( m_mapLevelGPInfo );
    }

    SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_LEVEL_GP_INFO_NOT, L"StatGP", 1, 0, mapLevelGPInfo );
}