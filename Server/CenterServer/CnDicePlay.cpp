#include "CnDicePlay.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h"
#include "CommonPacket.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnDicePlay );
ImplOstmOperatorW2A( KCnDicePlay );
NiImplementRootRTTI( KCnDicePlay );

// 랭킹 계산을 위한 비교 Class
// 랭킹 계산
// 전체 바퀴수는 같거나 크고.., 현재 판의 위치는 큰 값일때 변경되야 한다.
namespace
{
    class KComp
    {
    public:
        bool operator() ( const KDicePlayRank& a, const KDicePlayRank& b )
        {
            return ( a.m_dwTotalCount > b.m_dwTotalCount ||
                !(b.m_dwTotalCount > a.m_dwTotalCount) && a.m_dwCurrentPosition > b.m_dwCurrentPosition);
        }
    };
}

KCnDicePlay::KCnDicePlay(void)
:m_dwMaxRankSize(3)
,m_tmDBUpdateTime(0)
,m_dwLastTick(0)
,m_dwTickGap(0)
{
    m_dwLastTick = ::GetTickCount();
    m_tmBegin = time_t(NULL);
    m_tmEnd = time_t(NULL);
}

KCnDicePlay::~KCnDicePlay(void)
{
}

ImplToStringW( KCnDicePlay )
{
    KLocker lock( m_csDicePlay );
    return START_TOSTRINGW
        << TOSTRINGWb( IsRun() )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tmDBUpdateTime) )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_vecDicePlayRank.size() );
}

void KCnDicePlay::PrintRank()
{
    std::cout << "-- DicePlay Rank Dump --" << std::endl;
    std::vector<KDicePlayRank>::iterator vit;
    for( vit = m_vecDicePlayRank.begin() ; vit != m_vecDicePlayRank.end() ; ++vit )
    {
        std::cout << "(" << vit->m_nRank
            << ") [" << KncUtil::toNarrowString(vit->m_strNickName)
            << "] [" << vit->m_dwTotalCount
            << "] [" << vit->m_dwCurrentPosition
            << "]" << std::endl;
    }
    std::cout << "-- DicePlay Rank Dump END --" << std::endl;
}

// 주사위 이벤트 루아 스크립트 파일 Load
bool KCnDicePlay::LoadScript()
{
    KLuaManager kLuaMng;
    time_t      tmBegin;
    time_t      tmEnd;
    time_t      tmDBUpdateTime;
    DWORD       dwTickGap;
    _JIF( kLuaMng.DoFile( "InitCnDicePlay.lua" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadUpdateTime( kLuaMng, std::string("DBUpdateTime"), tmDBUpdateTime ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );

    START_LOG( cerr, L"주사위 이벤트 스크립트 읽기 완료. " ) << END_LOG;
    KLocker lock( m_csDicePlay );
    m_tmDBUpdateTime    = tmDBUpdateTime;
    m_dwTickGap         = dwTickGap;
    m_tmBegin           = tmBegin;
    m_tmEnd             = tmEnd;
    return true;
}

bool KCnDicePlay::LoadUpdateTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCnDicePlay::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

void KCnDicePlay::InitProcess()
{
    if( IsRun() )
    {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DICE_PLAY_TOTAL_RANK_REQ, L"", 0 );
    }
}

bool KCnDicePlay::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csDicePlay );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}


void KCnDicePlay::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();
    if( dwCurrentTick - m_dwLastTick < m_dwTickGap ) return;

    m_dwLastTick = dwCurrentTick;

    // 주사위 이벤트 기간중인가?
    if( ChkUpdateFromDB() && IsRun() )
    {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DICE_PLAY_TOTAL_RANK_REQ, L"", 0 );
    }
}

// DB Update를 위해서 하루가 지났는지를 체크.
bool KCnDicePlay::ChkUpdateFromDB()
{
    KLocker lock( m_csDicePlay );
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

// 랭킹 정보 가져오기.
void KCnDicePlay::GetDicePlayRank( OUT std::vector<KDicePlayRank>& vecRank_ )
{
    vecRank_.clear();
    KLocker lock( m_csDicePlay );
    vecRank_ = m_vecDicePlayRank;
}

// 랭킹 정보 정렬.
void KCnDicePlay::AddRankNoSort( IN KDicePlayRank& kRank_, DWORD dwTotalCount_, DWORD dwCurrentPosition_ )
{
    // 정렬이 필요없을 경우에는 그냥 return.
    if( dwTotalCount_ > kRank_.m_dwTotalCount || dwCurrentPosition_ > kRank_.m_dwCurrentPosition ) return;

    std::vector<KDicePlayRank>::iterator vit;
    vit = std::find_if( m_vecDicePlayRank.begin(), m_vecDicePlayRank.end(), 
        boost::bind( &KDicePlayRank::m_strNickName, _1 ) == kRank_.m_strNickName );

    if( vit != m_vecDicePlayRank.end() )
    {
        vit->m_dwTotalCount = std::max<DWORD>( vit->m_dwTotalCount, kRank_.m_dwTotalCount );
        vit->m_dwCurrentPosition = std::max<DWORD>( vit->m_dwCurrentPosition, kRank_.m_dwCurrentPosition );
    }
    else
    {
        m_vecDicePlayRank.push_back( kRank_ );
    }
}

// 랭킹정보 최대 숫자만큼 가지고 있기.
void KCnDicePlay::SortAddNumbering( IN OUT std::vector<KDicePlayRank>& vecRank_ )
{
    std::sort( vecRank_.begin(), vecRank_.end(), KComp() );

    if( vecRank_.size() > m_dwMaxRankSize )
        vecRank_.resize( m_dwMaxRankSize );

    std::vector<KDicePlayRank>::iterator vit;
    int nRank = 0;
    for( vit = vecRank_.begin(); vit != vecRank_.end(); ++vit )
    {
        vit->m_nRank = ++nRank;
    }
}

// DB에서 랭킹정보 가져오기.
void KCnDicePlay::UpdateDicePlayRankDB( IN std::vector<KDicePlayRank>& vecRank_ )
{
    SortAddNumbering( vecRank_ );

    KLocker lock( m_csDicePlay );
    m_vecDicePlayRank = vecRank_;
}

// GameServer -> CenterServer 랭킹 정보 전송.
void KCnDicePlay::UpdateDicePlayRankServer( IN std::vector<KDicePlayRank>& vecRank_ )
{
    DWORD dwTotalCount = 0;
    DWORD dwCurrentPosition = 0;

    std::vector<KDicePlayRank>::iterator vit;
    for( vit = vecRank_.begin(); vit != vecRank_.end(); ++vit )
    {
        AddRankNoSort( *vit, dwTotalCount, dwCurrentPosition );
    }

    SortAddNumbering( m_vecDicePlayRank );
}