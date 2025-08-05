#include "CenterSimLayer.h"
#include "CnOlympicManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h"
#include "Log4.h"

ImplementSingleton( KCnOlympicManager );
ImplOstmOperatorW2A( KCnOlympicManager );
NiImplementRootRTTI( KCnOlympicManager );

KCnOlympicManager::KCnOlympicManager(void)
:m_tmDBUpdateTime(0)
,m_dwTickGap(1000*60)
,m_bChanged(false)
{
    m_dwLastTick = ::GetTickCount();
    m_tmBegin = time_t(NULL);
    m_tmEnd = time_t(NULL);
    m_vecRegDateRank.clear();
    m_vecTotalRank.clear();

    m_mapOlympicInfoData.clear();
}

KCnOlympicManager::~KCnOlympicManager(void)
{
}

ImplToStringW( KCnOlympicManager )
{
    KLocker lock( m_csCnOlympic );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tmDBUpdateTime) )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_vecRegDateRank.size() )
        << TOSTRINGW( m_vecTotalRank.size() )
        << TOSTRINGW( m_mapOlympicInfoData.size() );
}

bool KCnOlympicManager::LoadScript()
{
    KLuaManager kLuaMng;
    time_t      tmBegin;
    time_t      tmEnd;
    time_t      tmDBUpdateTime;
    DWORD       dwTickGap;
    _JIF( kLuaMng.DoFile( "InitCnOlympic.lua" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadUpdateTime( kLuaMng, std::string("DBUpdateTime"), tmDBUpdateTime ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );

    START_LOG( cerr, L"올림픽 이벤트 스크립트 읽기 완료. " ) << END_LOG;
    {
        KLocker lock( m_csCnOlympic );
        m_tmDBUpdateTime    = tmDBUpdateTime;
        m_dwTickGap         = dwTickGap;
        m_tmBegin           = tmBegin;
        m_tmEnd             = tmEnd;
    }
    return true;
}

bool KCnOlympicManager::LoadUpdateTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCnOlympicManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCnOlympicManager::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csCnOlympic );
    return ( m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent );
}

void KCnOlympicManager::Tick()
{
    if ( CheckTick() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_OLYMPIC_DATA_NOT, L"", 0 );
    }

    if( IsRun() ) {
        UpdateDBRankData();
    }

    // 올림픽 이벤트 기간중인가?
    if( ChkUpdateFromDB() && IsRun() ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_OLYMPIC_RANK_REQ, L"", 0 );
    }
}

bool KCnOlympicManager::CheckTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = ::GetTickCount();
    return true;
}

bool KCnOlympicManager::ChkUpdateFromDB()
{
    KLocker lock( m_csCnOlympic );
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

void KCnOlympicManager::UpdateRankDB( IN std::vector<KRegDateRankInfo>& vecRegDateRank_, IN std::vector<KTotalRankInfo>& vecTotalRank_, IN const bool& bInit_ )
{
    KLocker lock( m_csCnOlympic );
    m_vecRegDateRank = vecRegDateRank_;
    m_vecTotalRank = vecTotalRank_;

    if( !bInit_ ) { // 서버 처음 켜질때는 전달안함.
        KECN_OLYMPIC_RANK_DATA_NOT kPacket;
        kPacket.m_vecRegDateRank = m_vecRegDateRank;
        kPacket.m_vecTotalRank = m_vecTotalRank;
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_OLYMPIC_RANK_DATA_NOT, kPacket );
    }
}

void KCnOlympicManager::UpdateUserSoloData( IN const time_t& tmDate_, IN const int& nGameType_, IN const int& nTeamType_, IN const std::wstring& wstrNickName_, IN const int& nAccumulateData_ )
{
    std::map<time_t, KOlympicInfoData>::iterator mit;
    std::map<time_t, KOlympicInfoData>::iterator mitUpdate;

    KLocker lock( m_csCnOlympic );
    // 날짜가 지났는지 체크.
    mit = m_mapOlympicInfoData.find( tmDate_ );
    if( mit == m_mapOlympicInfoData.end() ) {
        KOlympicInfoData kOlympicInfoData;
        kOlympicInfoData.m_tmRegDate = tmDate_;
        kOlympicInfoData.m_nGameType = nGameType_;
        kOlympicInfoData.m_nTeamType = nTeamType_;
        kOlympicInfoData.m_biTotalData = nAccumulateData_;
        kOlympicInfoData.m_strNickName = wstrNickName_;
        m_mapOlympicInfoData[tmDate_] = kOlympicInfoData;
    } else {
        // 개인전 랭킹에 대하여 체크하고 갱신처리.
        if( nAccumulateData_ > mit->second.m_biTotalData ) {
            mit->second.m_strNickName = wstrNickName_;
            mit->second.m_biTotalData = nAccumulateData_;
        }
    }

    mitUpdate = m_mapOlympicInfoData.find( tmDate_ );
    if( mitUpdate == m_mapOlympicInfoData.end() ) {
        return;
    }

    KECN_OLYMPIC_SYNC_SOLO_DATA_NOT kPacket;
    kPacket.m_tmTodayDate = mitUpdate->second.m_tmRegDate;
    kPacket.m_nGameType = mitUpdate->second.m_nGameType;
    kPacket.m_nTeamType = mitUpdate->second.m_nTeamType;
    kPacket.m_nTopPlayerData = (int)mitUpdate->second.m_biTotalData;
    kPacket.m_strNickName = mitUpdate->second.m_strNickName;

    // 전체 게임서버에 CenterServer 개인전 Data 동기화.
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_OLYMPIC_SYNC_SOLO_DATA_NOT, kPacket );

    m_bChanged = true;
}

void KCnOlympicManager::UpdateUserTeamData( IN const time_t& tmDate_, IN const int& nGameType_, IN const int& nTeamType_, IN const int& nAccumulateData_ )
{
    std::map<time_t, KOlympicInfoData>::iterator mit;
    std::map<time_t, KOlympicInfoData>::iterator mitUpdate;

    KLocker lock( m_csCnOlympic );

    mit = m_mapOlympicInfoData.find( tmDate_ );
    if( mit == m_mapOlympicInfoData.end() ) {
        KOlympicInfoData kOlympicInfoData;
        kOlympicInfoData.m_tmRegDate = tmDate_;
        kOlympicInfoData.m_nGameType = nGameType_;
        kOlympicInfoData.m_nTeamType = nTeamType_;
        kOlympicInfoData.m_biTotalData += nAccumulateData_;
        m_mapOlympicInfoData[tmDate_] = kOlympicInfoData;
    } else {
        mit->second.m_biTotalData += nAccumulateData_;
    }

    mitUpdate = m_mapOlympicInfoData.find( tmDate_ );
    if( mitUpdate == m_mapOlympicInfoData.end() ) {
        return;
    }

    KECN_OLYMPIC_SYNC_TEAM_DATA_ACK kPacket;
    kPacket.m_tmTodayDate = mitUpdate->second.m_tmRegDate;
    kPacket.m_nGameType = mitUpdate->second.m_nGameType;
    kPacket.m_nTeamType = mitUpdate->second.m_nTeamType;
    kPacket.m_biTotalData = mitUpdate->second.m_biTotalData;

    // 전체 게임서버에 CenterServer 개인전 Data 동기화.
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_OLYMPIC_SYNC_TEAM_DATA_ACK, kPacket );

    m_bChanged = true;
}

void KCnOlympicManager::InitProcess()
{
    if( IsRun() ) {
        // 최초 호출시에는 UID에 1을 넣어준다.
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_OLYMPIC_RANK_REQ, L"", 1 );
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_OLYMPIC_DB_USER_DATA_REQ, L"", 0 );
    }
}

void KCnOlympicManager::GetOlympicUserData( OUT std::map<time_t, KOlympicInfoData>& mapOlympicInfoData_ )
{
    mapOlympicInfoData_.clear();

    KLocker lock( m_csCnOlympic );
    mapOlympicInfoData_ = m_mapOlympicInfoData;
}

void KCnOlympicManager::UpdateDBRankData()
{
    KLocker lock( m_csCnOlympic );
    // DB에 랭킹 데이터 갱신.
    if( m_bChanged ) {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_OLYMPIC_USER_DATA_UPDATE_NOT, L"", 0 );
        m_bChanged = false;
    }
}

void KCnOlympicManager::UpdateUserData( IN std::map<time_t, KOlympicInfoData>& mapOlympicInfoData_ )
{
    KLocker lock( m_csCnOlympic );
    m_mapOlympicInfoData.swap( mapOlympicInfoData_ );
}

void KCnOlympicManager::GetOlympicRankInfo( OUT std::vector<KRegDateRankInfo>& vecRegDateRank_, OUT std::vector<KTotalRankInfo>& vecTotalRank_ )
{
    vecRegDateRank_.clear();
    vecTotalRank_.clear();

    KLocker lock( m_csCnOlympic );
    vecRegDateRank_ = m_vecRegDateRank;
    vecTotalRank_ = m_vecTotalRank;
}

void KCnOlympicManager::GetOlympicEventData( OUT KECN_OLYMPIC_EVENT_DATA_NOT& kPacket_ )
{
    std::map<time_t, KOlympicInfoData>::reverse_iterator rmit;

    KLocker lock( m_csCnOlympic );
    if( !m_mapOlympicInfoData.empty() ) {
        rmit = m_mapOlympicInfoData.rbegin();
        kPacket_.m_tmRegDate = rmit->second.m_tmRegDate;
        kPacket_.m_nGameType = rmit->second.m_nGameType;
        kPacket_.m_nTeamType = rmit->second.m_nTeamType;
        kPacket_.m_strNickName = rmit->second.m_strNickName;
        kPacket_.m_biTotalData = rmit->second.m_biTotalData;
    }

    GetOlympicRankInfo( kPacket_.m_vecRegDateRank, kPacket_.m_vecTotalRank );
}