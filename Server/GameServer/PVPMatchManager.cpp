#include "GSSimLayer.h"
#include "PVPMatchManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include "UserPacket.h"
#include "UserEvent.h"

ImplementSingleton( KPVPMatchManager );
ImplOstmOperatorW2A( KPVPMatchManager );
NiImplementRootRTTI( KPVPMatchManager );

KPVPMatchManager::KPVPMatchManager(void)
:m_nPlacementTestPlayCount(0)
,m_nPlacementTestFirstConstantK(0)
,m_fPlacementTestRatio(0.f)
,m_nMaxConstantK(0)
,m_nMaxReceiveRP(0)
,m_nPlacementTestMaxReceiveRP(0)
,m_nMaxStartRPValue(0)
,m_fAddExpRatio(0.f)
,m_fAddGPRatio(0.f)
,m_nMatchPartyMaxNum(3)
,m_bLimitMatchEnable(false)
,m_nLimitMatchGroupNum(0)
,m_nSeasonYear(0)
,m_nSeasonVersion(0)
,m_dwMinDelMatchTime(5) // 5초.
,m_bPenaltyEnable(false)
,m_nPenaltyRefuseCount(0)
,m_dwPenaltyTime(1000*60*5) // 5분.
,m_dwMaxGrade(0)
,m_bOpenTimeEnable(false)
,m_dwTickGap(1000*60) // 1분.
{
    m_vecConstantKInfo.clear();
    m_vecPlacementTestRPTable.clear();
    m_vecRPDefineTable.clear();
    m_mapModeCount.clear();
    m_vecRPMarkGradeTable.clear();
    m_vecRPMarkDecreaseGradeTable.clear();
    m_mapOpenTimeTable.clear();
    m_mapModeMaxMinInfo.clear();
    m_mapModeOpenEnable.clear();
    m_vecOpenAlarm.clear();
    m_vecCloseAlarm.clear();
    // 서버 시작시간 저장.
    // 하루가 지났는지에 대한 여부를 확인을 위해서.
    CTime tmCurrent = CTime::GetCurrentTime();
    m_tmStartBegin = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), 
        tmCurrent.GetMonth(), 
        tmCurrent.GetDay(), 
        0, 0, 0 ) );
    m_dwLastTick = ::GetTickCount();
}

KPVPMatchManager::~KPVPMatchManager(void)
{
}

ImplToStringW( KPVPMatchManager )
{
    KLocker lock( m_csPVPMatchManager );

    PrintMatchModeInfo( stm_ );

    return START_TOSTRINGW
        << TOSTRINGW( m_nPlacementTestPlayCount )
        << TOSTRINGW( m_nPlacementTestFirstConstantK )
        << TOSTRINGW( m_fPlacementTestRatio )
        << TOSTRINGW( m_nMaxConstantK )
        << TOSTRINGW( m_nMaxReceiveRP )
        << TOSTRINGW( m_nPlacementTestMaxReceiveRP )
        << TOSTRINGW( m_nMaxStartRPValue )
        << TOSTRINGW( m_fAddExpRatio )
        << TOSTRINGW( m_fAddGPRatio )
        << TOSTRINGW( m_nMatchPartyMaxNum )
        << TOSTRINGW( m_bLimitMatchEnable )
        << TOSTRINGW( m_nLimitMatchGroupNum )
        << TOSTRINGW( m_nSeasonYear )
        << TOSTRINGW( m_nSeasonVersion )
        << TOSTRINGW( m_bPenaltyEnable )
        << TOSTRINGW( m_nPenaltyRefuseCount )
        << TOSTRINGW( m_dwPenaltyTime )
        << TOSTRINGW( m_vecConstantKInfo.size() )
        << TOSTRINGW( m_vecPlacementTestRPTable.size() )
        << TOSTRINGW( m_vecRPDefineTable.size() )
        << TOSTRINGW( m_vecRPMarkGradeTable.size() )
        << TOSTRINGW( m_vecRPMarkDecreaseGradeTable.size() )
        << TOSTRINGWb( m_bOpenTimeEnable )
        << TOSTRINGW( m_mapOpenTimeTable.size() )
        << TOSTRINGW( m_mapModeMaxMinInfo.size() )
        << TOSTRINGW( m_mapModeOpenEnable.size() )
        << TOSTRINGW( m_vecOpenAlarm.size() )
        << TOSTRINGW( m_vecCloseAlarm.size() );
}

bool KPVPMatchManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KPVPMatchManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    int                                     nMaxConstantK;
    std::vector<KConstantKInfo>             vecConstantKInfo;
    std::vector<KPlacementTestRPTable>      vecPlacementTestRPTable;
    int                                     nPlacementTestPlayCount;
    int                                     nPlacementTestFirstConstantK;
    float                                   fPlacementTestRatio;
    int                                     nMaxReceiveRP;
    int                                     nPlacementTestMaxReceiveRP;
    int                                     nMaxStartRPValue;
    float                                   fAddExpRatio;
    float                                   fAddGPRatio;
    int                                     nMatchPartyMaxNum;
    std::vector<KRPDefineTable>             vecRPDefineTable;
    bool                                    bLimitMatchEnable;
    int                                     nLimitMatchGroupNum;
    int                                     nSeasonYear;
    int                                     nSeasonVersion;
    bool                                    bPenaltyEnable;
    int                                     nPenaltyRefuseCount;
    DWORD                                   dwPenaltyTime;
    DWORD                                   dwMaxGrade;
    std::vector<KRPMarkGradeTable>          vecRPMarkGradeTable;
    std::vector<KRPMarkGradeTable>          vecRPMarkDecreaseGradeTable;
    bool                                    bOpenTimeEnable;
    std::map<int, KMatchOpenTime>           mapOpenTimeTable;
    std::map<int,std::pair<int,int>>        mapModeMaxMinInfo;
    std::map<int,bool>                      mapModeOpenEnable;
    std::vector<int>                        vecOpenAlarm;
    std::vector<int>                        vecCloseAlarm;

    const std::string strScriptName = "InitMatchHelper.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MatchPartyMaxNum", nMatchPartyMaxNum ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "bLimitMatchEnable", bLimitMatchEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "LimitMatchGroupNum", nLimitMatchGroupNum ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SeasonYear", nSeasonYear ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SeasonVersion", nSeasonVersion ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxConstantK", nMaxConstantK ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxReceiveRP", nMaxReceiveRP ) == S_OK, return false );
    _JIF( LoadConstantKTable( kLuaMng, vecConstantKInfo ), return false );
    _JIF( kLuaMng.GetValue( "ELOAddExpRatio", fAddExpRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ELOAddGpRatio", fAddGPRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxStartRPValue", nMaxStartRPValue ) == S_OK, return false );
    _JIF( LoadPlacementTestRPTable( kLuaMng, vecPlacementTestRPTable ), return false );
    _JIF( kLuaMng.GetValue( "PlacementTestPlayCount", nPlacementTestPlayCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PlacementTestFirstConstantK", nPlacementTestFirstConstantK ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PlacementTestRatio", fPlacementTestRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PlacementTestMaxReceiveRP", nPlacementTestMaxReceiveRP ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PenaltyEnable", bPenaltyEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PenaltyRefuseCount", nPenaltyRefuseCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "PenaltyTime", dwPenaltyTime ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxGrade", dwMaxGrade ) == S_OK, return false );
    _JIF( LoadRPMarkGradeTable( kLuaMng, std::string("RPMarkGradeTable"), vecRPMarkGradeTable ), return false );
    _JIF( LoadRPMarkGradeTable( kLuaMng, std::string("RPMarkDecreaseGradeTable"), vecRPMarkDecreaseGradeTable ), return false );
    _JIF( LoadRPDefineTable( kLuaMng, vecRPDefineTable ), return false );
    _JIF( LoadOpenTimeTable( kLuaMng, bOpenTimeEnable, mapOpenTimeTable ), return false );
    _JIF( LoadModeMaxMinInfo( kLuaMng, mapModeMaxMinInfo ), return false );
    _JIF( LoadModeOpenEnableInfo( kLuaMng, mapModeOpenEnable ), return false );
    _JIF( LoadAlarmTime( kLuaMng, "MatchOpenAlarm", vecOpenAlarm ), return false );
    _JIF( LoadAlarmTime( kLuaMng, "MatchCloseAlarm", vecCloseAlarm ), return false );

    {
        KLocker lock( m_csPVPMatchManager );
        m_nMatchPartyMaxNum = nMatchPartyMaxNum;
        m_nMaxConstantK = nMaxConstantK;
        m_vecConstantKInfo.swap( vecConstantKInfo );
        m_vecPlacementTestRPTable.swap( vecPlacementTestRPTable );
        m_nPlacementTestPlayCount = nPlacementTestPlayCount;
        m_nPlacementTestFirstConstantK = nPlacementTestFirstConstantK;
        m_fPlacementTestRatio = fPlacementTestRatio;
        m_nMaxReceiveRP = nMaxReceiveRP;
        m_nPlacementTestMaxReceiveRP = nPlacementTestMaxReceiveRP;
        m_nMaxStartRPValue = nMaxStartRPValue;
        m_fAddExpRatio = fAddExpRatio;
        m_fAddGPRatio = fAddGPRatio;
        m_bLimitMatchEnable = bLimitMatchEnable;
        m_nLimitMatchGroupNum = nLimitMatchGroupNum;
        m_vecRPDefineTable.swap( vecRPDefineTable );
        m_nSeasonYear = nSeasonYear;
        m_nSeasonVersion = nSeasonVersion;
        m_bPenaltyEnable = bPenaltyEnable;
        m_nPenaltyRefuseCount = nPenaltyRefuseCount;
        m_dwPenaltyTime = dwPenaltyTime;
        m_dwMaxGrade = dwMaxGrade;
        m_vecRPMarkGradeTable.swap( vecRPMarkGradeTable );
        m_vecRPMarkDecreaseGradeTable.swap( vecRPMarkDecreaseGradeTable );
        m_bOpenTimeEnable = bOpenTimeEnable;
        m_mapOpenTimeTable.swap( mapOpenTimeTable );
        m_mapModeMaxMinInfo.swap( mapModeMaxMinInfo );
        m_mapModeOpenEnable.swap( mapModeOpenEnable );
        m_vecOpenAlarm.swap( vecOpenAlarm );
        m_vecCloseAlarm.swap( vecCloseAlarm );
    }

    START_LOG( cerr, L"MatchInfo 정보 읽기 완료." )
        << BUILD_LOG( m_nMaxConstantK )
        << BUILD_LOG( m_vecConstantKInfo.size() )
        << BUILD_LOG( m_vecPlacementTestRPTable.size() )
        << BUILD_LOG( m_nPlacementTestPlayCount )
        << BUILD_LOG( m_nPlacementTestFirstConstantK )
        << BUILD_LOG( m_fPlacementTestRatio )
        << BUILD_LOG( m_nMaxReceiveRP )
        << BUILD_LOG( m_nPlacementTestMaxReceiveRP )
        << BUILD_LOG( m_nMaxStartRPValue )
        << BUILD_LOG( m_fAddExpRatio )
        << BUILD_LOG( m_fAddGPRatio )
        << BUILD_LOG( m_bLimitMatchEnable )
        << BUILD_LOG( m_nLimitMatchGroupNum )
        << BUILD_LOG( m_nSeasonYear )
        << BUILD_LOG( m_nSeasonVersion )
        << BUILD_LOG( m_bPenaltyEnable )
        << BUILD_LOG( m_nPenaltyRefuseCount )
        << BUILD_LOG( m_dwPenaltyTime )
        << BUILD_LOG( m_vecRPDefineTable.size() )
        << BUILD_LOG( m_vecRPMarkGradeTable.size() )
        << BUILD_LOG( m_vecRPMarkDecreaseGradeTable.size() )
        << BUILD_LOG( m_bOpenTimeEnable )
        << BUILD_LOG( m_mapOpenTimeTable.size() )
        << BUILD_LOG( m_mapModeMaxMinInfo.size() )
        << BUILD_LOG( m_mapModeOpenEnable.size() )
        << BUILD_LOG( m_vecOpenAlarm.size() )
        << BUILD_LOG( m_vecCloseAlarm.size() )
        << END_LOG;

    // 매칭 스크립트 Reload시 전체 유저에게 변경된 정보 전달.
    // 시간정보가 갱신되었을 수도 있기 때문에 전달 필요.
    SendMatchInfoAllUser();

    return true;
}

bool KPVPMatchManager::LoadConstantKTable( IN KLuaManager& kLuaMng_, OUT std::vector<KConstantKInfo>& vecConstantKInfo_ )
{
    vecConstantKInfo_.clear();
    _JIF( kLuaMng_.BeginTable( "ConstantKTable" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KConstantKInfo kConstantKInfo;
        _JIF( kLuaMng_.GetValue( "RaStartValue", kConstantKInfo.m_nRaStartValue ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "RaEndValue", kConstantKInfo.m_nRaEndValue ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "ConstantK", kConstantKInfo.m_nConstantK ) == S_OK, return false );

        vecConstantKInfo_.push_back( kConstantKInfo );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KPVPMatchManager::LoadPlacementTestRPTable( IN KLuaManager& kLuaMng_, OUT std::vector<KPlacementTestRPTable>& vecPlacementTestRPTable_ )
{
    vecPlacementTestRPTable_.clear();
    _JIF( kLuaMng_.BeginTable( "PlacementTestRPTable" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KPlacementTestRPTable kPlacementTestRPTable;
        _JIF( kLuaMng_.GetValue( "StartLevel", kPlacementTestRPTable.m_dwStartLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EndLevel", kPlacementTestRPTable.m_dwEndLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "StartRPValue", kPlacementTestRPTable.m_nStartRPValue ) == S_OK, return false );

        vecPlacementTestRPTable_.push_back( kPlacementTestRPTable );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

int KPVPMatchManager::GetConstantK( IN const int& nRP_ )
{
    std::vector<KConstantKInfo>::iterator vit;

    KLocker lock( m_csPVPMatchManager );
    for( vit = m_vecConstantKInfo.begin(); vit != m_vecConstantKInfo.end(); ++vit ) {
        if( vit->m_nRaStartValue <= nRP_ && vit->m_nRaEndValue >= nRP_ ) {
            return vit->m_nConstantK;
        }
    }
    return m_nMaxConstantK;
}

void KPVPMatchManager::GetPlacementTestRP( IN const DWORD& dwLevel_, OUT int& nRP_ )
{
    std::vector<KPlacementTestRPTable>::iterator vit;

    KLocker lock( m_csPVPMatchManager );
    for( vit = m_vecPlacementTestRPTable.begin(); vit != m_vecPlacementTestRPTable.end(); ++vit ) {
        if( vit->m_dwStartLevel <= dwLevel_ && vit->m_dwEndLevel >= dwLevel_ ) {
            nRP_ = vit->m_nStartRPValue;
            return;
        }
    }
    nRP_ = m_nMaxStartRPValue;
}

void KPVPMatchManager::CalcChangeRP( IN const int& nOurTeamRP_, IN const int& nOtherTeamRP_, IN const bool& bWin_, IN OUT int& nMyRP_ )
{
    LIF( nOurTeamRP_ >= 0 );
    LIF( nOtherTeamRP_ >= 0 );

    float fResultRP =0.f;
    int nResultRP = 0;
    int nRPGap = nOtherTeamRP_ - nOurTeamRP_; // RP 차이값 계산.
    int nConstantK = GetConstantK( nRPGap );  // 상수 K값 계산.

    float fWinRatio = 1.F / ( 1 + ::pow( 10.F, (nRPGap) / 400.F ) ); // 승률 계산.

    // ELO Rating Point 계산 공식
    // Result = Ra + K( Sa - Ea )
    // Ra : 유저의 현재 계산된 RP가 적용되기전 랭킹 점수
    // K  : 상수 K값
    // Sa : 게임에서 승리할 경우 1대입, 졌을 경우 0대입.
    if( bWin_ ) {
        fResultRP = ceil( nConstantK * ( 1.F - fWinRatio ) ); // 계산결과가 소수점 이하면 올림처리.
    } 
    else {
        fResultRP = ceil( -1 * nConstantK * fWinRatio );
    }
    nResultRP = static_cast<int>( fResultRP );
    int nMaxReceiveRP = GetMaxReceiveRP( KPVPMatchManager::CT_NORMAL ); // 최대 획득양.
    int nMinReceiveRP = -nMaxReceiveRP;            // 최소 획득양.

    if( nResultRP >= 0 ) {
        nResultRP = std::min<int>( nMaxReceiveRP, nResultRP );
    } else {
        nResultRP = std::max<int>( nMinReceiveRP, nResultRP );
    }

    nMyRP_ += nResultRP;
    nMyRP_ = std::max<int>( 0, nMyRP_ );

    START_LOG( clog, L"CalcChangeRP Result" )
        << BUILD_LOG( nOurTeamRP_ )
        << BUILD_LOG( nOtherTeamRP_ )
        << BUILD_LOG( bWin_ )
        << BUILD_LOG( nRPGap )
        << BUILD_LOG( nConstantK )
        << BUILD_LOG( nResultRP )
        << BUILD_LOG( nMyRP_ )
        << END_LOG;
}

void KPVPMatchManager::CalcPlacementTestChangeRP( IN const int& nOurTeamRP_, IN const int& nOtherTeamRP_, IN const bool& bWin_, IN OUT int& nConstantK_, IN const int& nMatchTotalCount_, IN OUT int& nLastWinLose_, IN OUT int& nMyRP_ )
{
    LIF( nOurTeamRP_ >= 0 );
    LIF( nOtherTeamRP_ >= 0 );

    int nWinLose = 0; // 현재 진행한 판의 승패 여부 계산.
    if( bWin_ ) {
        nWinLose = 1;
    }

    int nConstantK = nConstantK_;
    if( nMatchTotalCount_ == 0 ) { // 배치고사 첫판일 경우 상수K값 초기값 설정.
        nConstantK = m_nPlacementTestFirstConstantK;
    }
    else {
        if( nWinLose != nLastWinLose_ ) {
            // 반올림 처리를 위해서 0.5f 더해줘서 int casting.
            nConstantK = static_cast<int>( ( nConstantK * m_fPlacementTestRatio ) + 0.5f );
        }
    }

    if( bWin_ ) {
        nLastWinLose_ = 1;
    }
    else {
        nLastWinLose_ = 0;
    }

    float fResultRP =0.f;
    int nResultRP = 0;
    int nRPGap = nOtherTeamRP_ - nOurTeamRP_; // RP 차이값 계산.
    float fWinRatio = 1.F / ( 1 + ::pow( 10.F, (nRPGap) / 400.F ) ); // 승률 계산.

    // ELO Rating Point 계산 공식
    // Result = Ra + K( Sa - Ea )
    // Ra : 유저의 현재 계산된 RP가 적용되기전 랭킹 점수
    // K  : 상수 K값
    // Sa : 게임에서 승리할 경우 1대입, 졌을 경우 0대입.
    if( bWin_ ) {
        fResultRP = ceil( nConstantK * ( 1.F - fWinRatio ) ); // 계산결과가 소수점 이하면 올림처리.
    } 
    else {
        fResultRP = ceil( -1 * nConstantK * fWinRatio );
    }
    nResultRP = static_cast<int>( fResultRP );
    int nMaxReceiveRP = GetMaxReceiveRP( KPVPMatchManager::CT_PLACEMENTTEST ); // 최대 획득양.
    int nMinReceiveRP = -nMaxReceiveRP;            // 최소 획득양.

    if( nResultRP >= 0 ) {
        nResultRP = std::min<int>( nMaxReceiveRP, nResultRP );
    } else {
        nResultRP = std::max<int>( nMinReceiveRP, nResultRP );
    }

    nMyRP_ += nResultRP;
    nMyRP_ = std::max<int>( 0, nMyRP_ );
    nConstantK_ = nConstantK;

    START_LOG( clog, L"CalcChangeRP Result" )
        << BUILD_LOG( nOurTeamRP_ )
        << BUILD_LOG( nOtherTeamRP_ )
        << BUILD_LOG( bWin_ )
        << BUILD_LOG( nRPGap )
        << BUILD_LOG( nConstantK )
        << BUILD_LOG( nResultRP )
        << BUILD_LOG( nMyRP_ )
        << END_LOG;
}

int KPVPMatchManager::GetMaxReceiveRP( IN const int& nType_ )
{
    KLocker lock( m_csPVPMatchManager );
    if( nType_ == KPVPMatchManager::CT_PLACEMENTTEST ) {
        return m_nPlacementTestMaxReceiveRP;
    } else if( nType_ == KPVPMatchManager::CT_NORMAL ) {
        return m_nMaxReceiveRP;
    } else { // 잘못된 Type이 들어오면 기본 RP의 MAX값 리턴.
        START_LOG( cerr, L"설정되지 않은 ELO Type" )
            << BUILD_LOG( nType_ )
            << END_LOG;
        return m_nMaxReceiveRP;
    }
}

void KPVPMatchManager::CalcRatingPoint( IN std::pair<int, int>& prELOTeamRPAvg_, IN bool IsBlueTeam_, IN const bool& bWin_, IN OUT int& nMyRP_ )
{
    if( IsBlueTeam_ ) {
        CalcChangeRP( prELOTeamRPAvg_.first, prELOTeamRPAvg_.second, bWin_,nMyRP_ );
    } else {
        CalcChangeRP( prELOTeamRPAvg_.second, prELOTeamRPAvg_.first, bWin_,nMyRP_ );
    }
}

void KPVPMatchManager::CalcPlacementTestRatingPoint( IN std::pair<int, int>& prELOTeamRPAvg_, IN bool IsBlueTeam_, IN const bool& bWin_, IN OUT int& nConstantK_, IN const int& nMatchTotalCount_, IN OUT int& nLastWinLose_, IN OUT int& nMyRP_ )
{
    if( IsBlueTeam_ ) {
        CalcPlacementTestChangeRP( prELOTeamRPAvg_.first, prELOTeamRPAvg_.second, bWin_, nConstantK_, nMatchTotalCount_, nLastWinLose_, nMyRP_ );
    } else {
        CalcPlacementTestChangeRP( prELOTeamRPAvg_.second, prELOTeamRPAvg_.first, bWin_, nConstantK_, nMatchTotalCount_, nLastWinLose_, nMyRP_ );
    }
}

bool KPVPMatchManager::LoadRPDefineTable( IN KLuaManager& kLuaMng_, OUT std::vector<KRPDefineTable>& vecRPDefineTable_ )
{
    vecRPDefineTable_.clear();
    _JIF( kLuaMng_.BeginTable( "RPGradeTable" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KRPDefineTable kRPDefineTable;
        _JIF( kLuaMng_.GetValue( "StartRP", kRPDefineTable.m_dwStartRP ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EndRP", kRPDefineTable.m_dwEndRP ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "DefineRP", kRPDefineTable.m_nDefineRP ) == S_OK, return false );

        vecRPDefineTable_.push_back( kRPDefineTable );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

int KPVPMatchManager::GetDefineRP( IN const int& nRP_ )
{
    std::vector<KRPDefineTable>::iterator vit;
    KLocker lock( m_csPVPMatchManager );
    for( vit = m_vecRPDefineTable.begin(); vit != m_vecRPDefineTable.end(); ++vit ) {
        if( vit->m_dwStartRP <= (DWORD)nRP_ && vit->m_dwEndRP >= (DWORD)nRP_ ) {
            return vit->m_nDefineRP;
        }
    }
    // vector 컨테이너에 순서대로 들어가 있대로 생각하고 제일 최고값 리턴.
    if( !m_vecRPDefineTable.empty() ) {
        std::vector<KRPDefineTable>::reverse_iterator rvit = m_vecRPDefineTable.rbegin();
        return rvit->m_nDefineRP;
    } else {
        return 0;
    }
}

void KPVPMatchManager::InitModeCount()
{
    KLocker lock( m_csPVPMatchManager );
    m_mapModeCount.clear();
}

void KPVPMatchManager::SetModeCount( IN const int& nModeInfo_, IN const int& nWaitListCount_ )
{
    std::map<int,int>::iterator mit;
    KLocker lock( m_csPVPMatchManager );
    mit = m_mapModeCount.find( nModeInfo_ );
    if( mit == m_mapModeCount.end() ) {
        m_mapModeCount[nModeInfo_] = nWaitListCount_;
    } else {
        mit->second = nWaitListCount_;
    }
}

int KPVPMatchManager::GetModeCount( IN const int& nModeInfo_ )
{
    std::map<int,int>::iterator mit;
    KLocker lock( m_csPVPMatchManager );
    mit = m_mapModeCount.find( nModeInfo_ );
    if( mit == m_mapModeCount.end() ) {
        return 0;
    }
    return mit->second;
}

void KPVPMatchManager::CheckModeCount( IN const int& nModeInfo_, IN const int& nWaitListCount_ )
{
    std::map<int,int>::iterator mit;
    KLocker lock( m_csPVPMatchManager );
    mit = m_mapModeCount.find( nModeInfo_ );
    if( mit != m_mapModeCount.end() ) {
        if( mit->second != nWaitListCount_ ) {
            mit->second = nWaitListCount_;
        }
    }
}

bool KPVPMatchManager::CalcMatchTeam( IN const int& nModeInfo_, IN const std::vector<KPartyData>& vecPartyData_, OUT std::map<int, std::vector<KPartyMember> >& mapPartyMember_ )
{
    int nPartySize = vecPartyData_.size();
    if( !CheckCorrectPartyData( nModeInfo_, nPartySize ) ) { // 해당 모드의 파티사이즈가 정상인지 체크.
        START_LOG( cerr, L"잘못된 Party 사이즈 데이터" )
            << BUILD_LOG( nModeInfo_ )
            << BUILD_LOG( nPartySize )
            << END_LOG;
        return false;
    }

    std::map<int, std::vector<KPartyData>> mapTeamParty;
    int nStaticTeamSize = nModeInfo_ + 1; // 한팀 필요 인원수 세팅.
    if( nModeInfo_ < KPVPMatchManager::MI_PVP_1VS1 || nModeInfo_ > KPVPMatchManager::MI_PVP_3VS3 ) {
        START_LOG( cerr, L"잘못된 모드 Type" )
            << BUILD_LOG( nModeInfo_ )
            << END_LOG;
        return false;
    }

    if( !SetModeTeam( vecPartyData_, nStaticTeamSize, mapTeamParty ) ) {
        START_LOG( cerr, L"잘못된 팀 구성" )
            << BUILD_LOG( nModeInfo_ )
            << END_LOG;
        return false;
    }

    CalcPartyMemeber( mapTeamParty, mapPartyMember_ );

    return true;
}

void KPVPMatchManager::CalcPartyMemeber( IN std::map<int, std::vector<KPartyData>>& mapTeamParty_, OUT std::map<int, std::vector<KPartyMember> >& mapPartyMember_ )
{
    mapPartyMember_.clear();
    std::map<int, std::vector<KPartyData>>::iterator mit;
    std::vector<KPartyData>::iterator vitParty;
    std::vector<KPartyMember>::iterator vitMember;
    int nCount = 0;
    for( mit =  mapTeamParty_.begin(); mit != mapTeamParty_.end(); ++mit ) {
        for( vitParty = mit->second.begin(); vitParty != mit->second.end(); ++vitParty ) {
            for( vitMember = vitParty->m_vecPartyPeople.begin(); vitMember != vitParty->m_vecPartyPeople.end(); ++vitMember ) {
                if( nCount == 0 ) { // 룸에 방장으로 세팅
                    vitMember->m_IsHost = true; 
                } else {
                    vitMember->m_IsHost = false; 
                }
                vitMember->m_dwState = KPartyMember::US_NORMAL;
                vitMember->m_nExceptModeID = vitParty->m_nExceptModeID;
                mapPartyMember_[mit->first].push_back( *vitMember ); // 파티멤버 넣기.
                ++nCount;
            }
        }
    }
}

bool KPVPMatchManager::CheckCorrectPartyData( IN const int& nModeInfo_, IN const int& nPartySize_ )
{
    if( nModeInfo_ == KPVPMatchManager::MI_PVP_1VS1 ) {
        if( nPartySize_ != 2 ) {
            return false;
        }
    }
    else if( nModeInfo_ == KPVPMatchManager::MI_PVP_2VS2 ) {
        if( nPartySize_ < 2 || nPartySize_ > 4 ) {
            return false;
        }
    }
    else if( nModeInfo_ == KPVPMatchManager::MI_PVP_3VS3 ) {
        if( nPartySize_ < 2 || nPartySize_ > 6 ) {
            return false;
        }
    }
    return true;
}

void KPVPMatchManager::GetLimitMatchData( OUT bool& bLimitMatchEnable, OUT int& nLimitMatchGroupNum )
{
    KLocker lock( m_csPVPMatchManager );
    bLimitMatchEnable = m_bLimitMatchEnable;
    nLimitMatchGroupNum = m_nLimitMatchGroupNum;
}

bool KPVPMatchManager::SetModeTeam( IN const std::vector<KPartyData>& vecPartyData_, IN int nStaticTeamSize_, OUT std::map<int, std::vector<KPartyData>>& mapTeamParty_ )
{
    mapTeamParty_.clear();

    bool bResult = false;
    std::vector<KPartyData>::const_iterator cvitParty;
    int nRedTeam = 0;
    int nBludTeam = 0;

    for( cvitParty = vecPartyData_.begin(); cvitParty != vecPartyData_.end(); ++cvitParty ) {
        if ( nRedTeam + static_cast<int>(cvitParty->m_vecPartyPeople.size()) <= nStaticTeamSize_ ) {
            nRedTeam += static_cast<int>(cvitParty->m_vecPartyPeople.size());
            mapTeamParty_[KPVPMatchManager::TI_RED].push_back(*cvitParty);
        }
        else {
            nBludTeam += static_cast<int>(cvitParty->m_vecPartyPeople.size());
            mapTeamParty_[KPVPMatchManager::TI_BLUE].push_back(*cvitParty);
        }
    }

    if( nRedTeam == nBludTeam ) { // 3:3모드에서 파티멤버수가 2:2:2가 들어오는 case에 대해서는 return false처리를 하자.
        bResult = true;
    }
    return bResult;
}

void KPVPMatchManager::GetSeasonInfo( OUT int& nSeasonYear_, OUT int& nSeasonVersion_ )
{
    KLocker lock( m_csPVPMatchManager );
    nSeasonYear_ = m_nSeasonYear;
    nSeasonVersion_ = m_nSeasonVersion;
}

void KPVPMatchManager::CalcELOType( IN const int& nMatchTotalCount_, OUT int& nELOType_ )
{
    KLocker lock( m_csPVPMatchManager );
    if( nMatchTotalCount_ <= m_nPlacementTestPlayCount ) {
        nELOType_ = KELOUserData::MT_PLACEMENTTEST;
    } else {
        nELOType_ = KELOUserData::MT_NORMAL;
    }
}

void KPVPMatchManager::PrintMatchModeInfo( std::wostream& stm_ ) const
{
    std::map<int,int>::const_iterator cmit;

    stm_ << L" --- MatchModeInfo( size:" << m_mapModeCount.size() << L") --- " << std::endl;
    for( cmit = m_mapModeCount.begin(); cmit != m_mapModeCount.end(); ++cmit ) {
        stm_ << L" ModeType : " << cmit->first << L" Count : " << cmit->second << std::endl;
    }
    stm_ << L" --- MatchModeInfo Display End --- " << std::endl;
}

bool KPVPMatchManager::LoadRPMarkGradeTable( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::vector<KRPMarkGradeTable>& vecRPMarkGradeTable_ )
{
    vecRPMarkGradeTable_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KRPMarkGradeTable kRPMarkGradeTable;
        _JIF( kLuaMng_.GetValue( "StartRP", kRPMarkGradeTable.m_dwStartRP ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EndRP", kRPMarkGradeTable.m_dwEndRP ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Grade", kRPMarkGradeTable.m_dwGrade ) == S_OK, return false );

        vecRPMarkGradeTable_.push_back( kRPMarkGradeTable );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

DWORD KPVPMatchManager::GetMarkGrade( IN const int& nRP_ )
{
    std::vector<KRPMarkGradeTable>::iterator vit;

    KLocker lock( m_csPVPMatchManager );
    for( vit = m_vecRPMarkGradeTable.begin(); vit != m_vecRPMarkGradeTable.end(); ++vit ) {
        if( vit->m_dwStartRP <= (DWORD)nRP_ && vit->m_dwEndRP >= (DWORD)nRP_ ) {
            return vit->m_dwGrade;
        }
    }
    return m_dwMaxGrade;
}

void KPVPMatchManager::CheckDecreaseMarkGrade( IN const int& nRP_, IN OUT UCHAR& ucGrade_ )
{
    std::vector<KRPMarkGradeTable>::iterator vit;

    KLocker lock( m_csPVPMatchManager );
    for( vit = m_vecRPMarkDecreaseGradeTable.begin(); vit != m_vecRPMarkDecreaseGradeTable.end(); ++vit ) {
        if( vit->m_dwStartRP <= (DWORD)nRP_ && vit->m_dwEndRP >= (DWORD)nRP_ ) {
            if( vit->m_dwGrade < (DWORD)ucGrade_ ) {
                ucGrade_ = static_cast<UCHAR>( vit->m_dwGrade );
            }
        }
    }
}

void KPVPMatchManager::CalcCharELOGrade( IN const int& nRatingPoint_, IN OUT UCHAR& ucGrade_ )
{
    UCHAR ucGrade = static_cast<UCHAR>( GetMarkGrade( nRatingPoint_ ) );
    if( ucGrade_ < ucGrade ) {
        ucGrade_ = ucGrade;
        return;
    }
    CheckDecreaseMarkGrade( nRatingPoint_, ucGrade_ );
}

bool KPVPMatchManager::LoadOpenTimeTable( IN KLuaManager& kLuaMng_, OUT bool& bOpenTimeEnable_, OUT std::map<int, KMatchOpenTime>& mapOpenTimeTable_ )
{
    mapOpenTimeTable_.clear();

    _JIF( kLuaMng_.BeginTable( "ELOTimeTable"  ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "bEnable", bOpenTimeEnable_ ) == S_OK, return false );

    // 시간설정값.
    _JIF( kLuaMng_.BeginTable( "ELOSetOpenTime" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nIndex = 0;
        KMatchOpenTime kMatchOpenTime;
        _JIF( kLuaMng_.GetValue( "DayIndex", nIndex ) == S_OK, return false );
        _JIF( LoadTime( kLuaMng_, std::string("BeginTime"), kMatchOpenTime.m_prBeginTime ), return false );
        _JIF( LoadTime( kLuaMng_, std::string("EndTime"), kMatchOpenTime.m_prEndTime ), return false );

        if( mapOpenTimeTable_.insert( std::make_pair( nIndex, kMatchOpenTime ) ).second == false ) {
            START_LOG( cerr, L"중복된 요일 Index : " << nIndex )
                << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KPVPMatchManager::GetMatchTimeTable( OUT KEVENT_MATCH_TIME_TABLE_NOT& kPacket_ )
{
    KLocker lock( m_csPVPMatchManager );
    kPacket_.m_bOpenTimeEnable = m_bOpenTimeEnable;
    kPacket_.m_mapOpenTimeTable = m_mapOpenTimeTable;
    kPacket_.m_mapModeMaxMinInfo = m_mapModeMaxMinInfo;
}

bool KPVPMatchManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair<int,int>& prTime_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 1, prTime_.first ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, prTime_.second ) == S_OK, return false );;
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KPVPMatchManager::GetModeInfo( OUT std::map<int,int>& mapModeCount_ )
{
    mapModeCount_.clear();
    KLocker lock( m_csPVPMatchManager );
    mapModeCount_ = m_mapModeCount;
}

void KPVPMatchManager::Tick()
{
    // 시간체크
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return;
    }
    m_dwLastTick = ::GetTickCount();

    MatchAlarmNotice();
}

bool KPVPMatchManager::AddMatchEnable()
{
    CTime tmCurrentTime = CTime::GetCurrentTime();
    std::map<int, KMatchOpenTime>::iterator mit;
    int nDayOfWeek = tmCurrentTime.GetDayOfWeek() - 1; // 0 ~ 6사이 값을 생성.
    int nCurrentHour = tmCurrentTime.GetHour();
    int nCurrentMinute = tmCurrentTime.GetHour();

    KLocker lock( m_csPVPMatchManager );
    if( m_bOpenTimeEnable == false ) { // 시간설정을 체크하지 않을 경우..,
        return true;
    }

    mit = m_mapOpenTimeTable.find( nDayOfWeek );
    if( mit == m_mapOpenTimeTable.end() ) {
        START_LOG( cerr, L"설정된 날짜 Index값이 없음" )
            << BUILD_LOG( nDayOfWeek )
            << BUILD_LOG( m_mapOpenTimeTable.size() )
            << END_LOG;
        return false;
    }

    // 시간값이 맞지 않거나..,
    if( mit->second.m_prBeginTime.first > nCurrentHour || mit->second.m_prEndTime.first < nCurrentHour ) {
        return false;
    }

    // 시간이 같을 경우 현재 Minute값이 설정된 시간에 포함되지 않는다면..,
    if( ( mit->second.m_prBeginTime.first == nCurrentHour && mit->second.m_prBeginTime.second > nCurrentMinute ) ||
            ( mit->second.m_prEndTime.first == nCurrentHour && mit->second.m_prEndTime.second < nCurrentMinute ) ) {
            return false;
    }

    return true;
}

bool KPVPMatchManager::LoadModeMaxMinInfo( IN KLuaManager& kLuaMng_, OUT std::map<int,std::pair<int,int>>& mapModeMaxMinInfo_ )
{
    mapModeMaxMinInfo_.clear();

    // 시간설정값.
    _JIF( kLuaMng_.BeginTable( "ModeMaxMinInfo" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nModeInfo = 0;
        std::pair<int,int> prData;

        _JIF( kLuaMng_.GetValue( "ModeInfo", nModeInfo ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MinCount", prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MaxCount", prData.second ) == S_OK, return false );

        if( mapModeMaxMinInfo_.insert( std::make_pair( nModeInfo, prData ) ).second == false ) {
            START_LOG( cerr, L"중복된 모드정보 : " << nModeInfo )
                << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KPVPMatchManager::SendMatchInfoAllUser()
{
    KEVENT_MATCH_TIME_TABLE_NOT kPacket;
    SiKPVPMatchManager()->GetMatchTimeTable( kPacket );
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_MATCH_TIME_TABLE_NOT, kPacket );
}

bool KPVPMatchManager::LoadModeOpenEnableInfo( IN KLuaManager& kLuaMng_, OUT std::map<int,bool>& mapModeOpenEnable_ )
{
    mapModeOpenEnable_.clear();

    // 시간설정값.
    _JIF( kLuaMng_.BeginTable( "ModeOpenEnable" ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nModeInfo = 0;
        bool bOpenEnable = false;

        _JIF( kLuaMng_.GetValue( "ModeInfo", nModeInfo ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "OpenEnable", bOpenEnable ) == S_OK, return false );

        if( mapModeOpenEnable_.insert( std::make_pair( nModeInfo, bOpenEnable ) ).second == false ) {
            START_LOG( cerr, L"중복된 모드정보 : " << nModeInfo )
                << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KPVPMatchManager::CheckModeOpenEnable( IN const int& nModeInfo_ )
{
    std::map<int,bool>::const_iterator cmit;
    KLocker lock( m_csPVPMatchManager );
    cmit = m_mapModeOpenEnable.find( nModeInfo_ );
    if( cmit == m_mapModeOpenEnable.end() ) {
        return false;
    }
    if( cmit->second == false ) {
        return false;
    }
    return true;
}

void KPVPMatchManager::MatchAlarmNotice()
{
    CTime ctCurrent = CTime::GetCurrentTime();
    time_t tmCurrent = KncUtil::TimeToInt( ctCurrent );
    int nDayOfWeek = ctCurrent.GetDayOfWeek() - 1;
    std::map<int, KMatchOpenTime>::const_iterator cmit;

    KLocker lock( m_csPVPMatchManager );

    if( m_mapOpenTimeTable.empty() ) { // 시간정보 없을 경우..,
        return;
    }

    cmit = m_mapOpenTimeTable.find( nDayOfWeek );
    if( cmit == m_mapOpenTimeTable.end() ) { // 해당 요일의 시간정보 없을 경우..,
        return;
    }

    // 알람 보낼 시간인지 체크..,
    CheckAlarmNotice( tmCurrent, cmit->second.m_prBeginTime, cmit->second.m_prEndTime );
}

void KPVPMatchManager::CheckAlarmNotice( IN time_t& tmCurrent_, IN const std::pair<int,int>& prBeginTime_, IN const std::pair<int,int>& prEndTime_ )
{
    CTime ctCurrent( tmCurrent_ );
    CTime tmCheck;
    time_t tmOpen = KncUtil::TimeToInt( CTime( ctCurrent.GetYear(), ctCurrent.GetMonth(), ctCurrent.GetDay(), prBeginTime_.first, prBeginTime_.second, 0 ) );
    time_t tmClose = KncUtil::TimeToInt( CTime( ctCurrent.GetYear(), ctCurrent.GetMonth(), ctCurrent.GetDay(), prEndTime_.first, prEndTime_.second, 0 ) );

    std::vector<int>::iterator vitOpen;
    for( vitOpen = m_vecOpenAlarm.begin(); vitOpen != m_vecOpenAlarm.end(); ++vitOpen ) {
        tmCheck = CTime( tmOpen ) - CTimeSpan( 0, 0, *vitOpen, 0 );
        if( tmCheck.GetHour() == ctCurrent.GetHour() && tmCheck.GetMinute() == ctCurrent.GetMinute() ) {
            SendMatchAlarmNot( KEVENT_MATCH_ALARM_NOT::AT_ALARM_OPEN, prBeginTime_, prEndTime_, *vitOpen );
            return;
        }
    }

    std::vector<int>::iterator vitClose;
    for( vitClose = m_vecCloseAlarm.begin(); vitClose != m_vecCloseAlarm.end(); ++vitClose ) {
        tmCheck = CTime( tmClose ) - CTimeSpan( 0, 0, *vitClose, 0 );
        if( tmCheck.GetHour() == ctCurrent.GetHour() && tmCheck.GetMinute() == ctCurrent.GetMinute() ) {
            SendMatchAlarmNot( KEVENT_MATCH_ALARM_NOT::AT_ALARM_CLOSE, prBeginTime_, prEndTime_, *vitClose );
            return;
        }
    }
}

bool KPVPMatchManager::LoadAlarmTime( IN KLuaManager& kLuaMng_, IN const std::string& strTable_, OUT std::vector<int>& vecAlarm_ )
{
    vecAlarm_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        int nMin = 0;
        if ( kLuaMng_.GetValue( i, nMin ) != S_OK ) break;
        vecAlarm_.push_back( nMin );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KPVPMatchManager::SendMatchAlarmNot( IN const int nAlarmType_, IN const std::pair<int,int>& prBeginTime_, IN const std::pair<int,int>& prEndTime_, IN const int nMinute_ )
{
    KEVENT_MATCH_ALARM_NOT kPacket;
    kPacket.m_nAlarmType = nAlarmType_;
    kPacket.m_prOpenTime = prBeginTime_;
    kPacket.m_prCloseTime = prEndTime_;
    kPacket.m_nMinute = nMinute_;
    SiKGSSimLayer()->m_kActorManager.SendToAll( KUserEvent::EVENT_MATCH_ALARM_NOT, kPacket );
}