#include "CenterSimLayer.h" // winsock2
#include "CnDonationManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "CenterDBThread.h"
#include "CommonPacket.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnDonationManager );
ImplOstmOperatorW2A( KCnDonationManager );
NiImplementRootRTTI( KCnDonationManager );

namespace
{
    class KComp
    {
    public:
        bool operator() ( const KDotantionRank& a, const KDotantionRank& b )
        {
            if( a.m_nAccDonationPoint > b.m_nAccDonationPoint ) return true;
            return false;
        }
    };
}

KCnDonationManager::KCnDonationManager(void)
:m_biTotalDonation(0)
,m_tmDBUpdateTime(0)
,m_dwLastTick(0)
,m_dwTickGap(1000*60)
,m_dwMaxRankSize(5)
,m_bDBUpdateCheck(false)
,m_bMaxGradeCheck(true)
,m_dwMaxGrade(0)
{
    m_dwLastTick = ::GetTickCount();
    m_tmBegin = time_t(NULL);
    m_tmEnd = time_t(NULL);
}

KCnDonationManager::~KCnDonationManager(void)
{
}

ImplToStringW( KCnDonationManager )
{
    KLocker lock( m_csDonation );
    return START_TOSTRINGW
        << TOSTRINGWb( IsRun() )
        << TOSTRINGW( m_biTotalDonation )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tmDBUpdateTime) )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_vecDonationRank.size() )
        << TOSTRINGW( m_vecTotalInfo.size() )
        << TOSTRINGW( m_kCurrentGrade.m_dwGrade )
        << TOSTRINGW( m_kCurrentGrade.m_biGradeNum )
        << TOSTRINGW( m_kCurrentGrade.m_vecRewardList.size() )
        << TOSTRINGW( m_kCurrentGrade.m_dwGradeState );
}

void KCnDonationManager::PrintRank()
{
    std::cout << "-- Rank Dump --" << std::endl;
    std::vector<KDotantionRank>::iterator vit;
    for( vit = m_vecDonationRank.begin() ; vit != m_vecDonationRank.end() ; ++vit )
    {
        std::cout << "(" << vit->m_nRank
            << ") [" << KncUtil::toNarrowString(vit->m_strNickName)
            << "] [" << vit->m_nAccDonationPoint
            << "]" << std::endl;
    }
    std::cout << "-- Rank Dump END --" << std::endl;
}

bool KCnDonationManager::LoadScript()
{
    KLuaManager kLuaMng;
    time_t      tmBegin;
    time_t      tmEnd;
    time_t      tmDBUpdateTime;
    DWORD       dwTickGap;
    _JIF( kLuaMng.DoFile( "InitCnDonation.lua" ) == S_OK, return false );
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadUpdateTime( kLuaMng, std::string("DBUpdateTime"), tmDBUpdateTime ), return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );

    START_LOG( cerr, L"센터 기부 스크립트 읽기 완료. " ) << END_LOG;
    KLocker lock( m_csDonation );
    m_tmDBUpdateTime    = tmDBUpdateTime;
    m_dwTickGap         = dwTickGap;
    m_tmBegin           = tmBegin;
    m_tmEnd             = tmEnd;
    return true;
}

void KCnDonationManager::InitProcess()
{
    if( IsRun() )
    {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_GET_TOTAL_DONATION_POINT_REQ, L"", 0 );
    }
}

void KCnDonationManager::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();
    if( dwCurrentTick - m_dwLastTick < m_dwTickGap ) return;

    m_dwLastTick = dwCurrentTick;

    // DB갱신 정보가 있으면 GameServer에게 정보 전송.
    if( m_bDBUpdateCheck && IsRun() )
    {
        SendToDonationInfo();
    }

    // 기부 시스템 동작 중인가?
    if( ChkUpdateFromDB() && IsRun() )
    {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_GET_TOTAL_DONATION_POINT_REQ, L"", 0 );
    }

    // DB에 기부 갱신된 정보가 있는지 체크하자.
    QueuingToDB( false );
}

__int64 KCnDonationManager::GetTotalDonation()
{
    KLocker lock( m_csDonation );
    return m_biTotalDonation;
}

void KCnDonationManager::UpdateDeltaDonation( __int64 biDeletaDonation_ )
{
    KLocker lock( m_csDonation );
    m_biTotalDonation += biDeletaDonation_;

    // 게임서버에서 정보를 보내줄 때 등급별 획득갯수 초과에 대한 체크를 하자.
    // GameServer들은 각자 계산해서 가지고 있다. Center에서도 Print 출력으로
    // 현재 진행 사항에 대해서 체크할 수 있도록 해당 처리를 진행한다.
    if( m_biTotalDonation >= m_kCurrentGrade.m_biGradeNum && m_bMaxGradeCheck )
    {
        std::vector<KItemDonationInfo>::iterator vit;
        for( vit = m_vecTotalInfo.begin(); vit != m_vecTotalInfo.end(); ++vit )
        {
            if( m_kCurrentGrade.m_dwGrade == vit->m_dwGrade )
            {
                vit->m_dwGradeState = 1; // 기부 등급달성 상태로 교체하자.
            }
        }
        // Max Grade에 도달했으면 이제 이처리를 해줄필요가 없다.
        if( m_kCurrentGrade.m_dwGrade == m_dwMaxGrade )
        {
            m_bMaxGradeCheck = false;
        }
        else
        {
            // 다시 현재의 달성해야 하는 등급으로 수정해 줘야 한다.
            SetCurrentDoInfo();
        }
        START_LOG( clog, L"센터 기부 Grade값 변경. " )
            << BUILD_LOG( m_kCurrentGrade.m_biGradeNum ) << END_LOG;
    }
}

void KCnDonationManager::SetDonationFromDB( __int64 biDeletaDonation_ )
{
    KLocker lock( m_csDonation );
    m_biTotalDonation = biDeletaDonation_;
}

void KCnDonationManager::UpdateDonationRankDB( IN std::vector<KDotantionRank>& vecRank_ )
{
    SortAndNumbering( vecRank_ );

    KLocker lock( m_csDonation );
    m_vecDonationRank = vecRank_;
}

void KCnDonationManager::UpdateDonationRankServer( IN std::vector<KDotantionRank>& vecRank_ )
{
    int LastPoint = 0;
    KLocker lock( m_csDonation );

    std::vector<KDotantionRank>::iterator vit;

    for( vit = vecRank_.begin() ; vit != vecRank_.end() ; ++vit )
    {
        AddRankNoSort( *vit, LastPoint );
    }

    SortAndNumbering( m_vecDonationRank );
}

void KCnDonationManager::AddRankNoSort( IN KDotantionRank& kRank_, int LastPoint_ )
{
    if( LastPoint_ >= kRank_.m_nAccDonationPoint ) return;

    std::vector<KDotantionRank>::iterator vit;
    vit = std::find_if( m_vecDonationRank.begin(), m_vecDonationRank.end(),
        boost::bind( &KDotantionRank::m_strNickName, _1) == kRank_.m_strNickName );

    if( vit != m_vecDonationRank.end() )
    {
        vit->m_nAccDonationPoint = std::max<int>( vit->m_nAccDonationPoint, kRank_.m_nAccDonationPoint );
    }
    else
    {
        m_vecDonationRank.push_back( kRank_ );
    }

}

void KCnDonationManager::SortAndNumbering( IN OUT std::vector<KDotantionRank>& vecRank_ )
{
    std::sort( vecRank_.begin(), vecRank_.end(), KComp() );

    if( vecRank_.size() > m_dwMaxRankSize )
        vecRank_.resize( m_dwMaxRankSize );

    std::vector<KDotantionRank>::iterator vit;
    int nRank = 0;
    for( vit = vecRank_.begin() ; vit != vecRank_.end() ; ++vit )
    {
        vit->m_nRank = ++nRank;
    }

}

void KCnDonationManager::GetDonationRank( OUT std::vector<KDotantionRank>& vecRank_ )
{
    vecRank_.clear();
    KLocker lock( m_csDonation );
    vecRank_ = m_vecDonationRank;
}

bool KCnDonationManager::ChkUpdateFromDB()
{
    KLocker lock( m_csDonation );
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

bool KCnDonationManager::LoadUpdateTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCnDonationManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KCnDonationManager::IsRun() const
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csDonation );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

// DB에서 가져온 기부 Grade List에 대해서 저장해 두고 해당 값들의 State값을 연산한다.
void KCnDonationManager::UpdateItemDonationInfo( IN std::vector<KItemDonationInfo>& vecDoInfo_ )
{
    KLocker lock( m_csDonation );
    m_vecTotalInfo = vecDoInfo_;

    // 송편 기부 정보를 가져와서 현재 state에 대해서 연산해서 저장해 두자.
    // state = 0 ( 미달성 ), state = 1 ( 달성 )
    std::vector<KItemDonationInfo>::iterator vit;
    for( vit = m_vecTotalInfo.begin(); vit != m_vecTotalInfo.end(); ++vit )
    {
        if( vit->m_biGradeNum < m_biTotalDonation )
        {
            vit->m_dwGradeState = 1;
        }
        // Grade의 최대값을 저장해 두자.
        m_dwMaxGrade = vit->m_dwGrade;
    }
    // DB에서 가져온 데이터를 가지고 Center Server에 현재 달성해야 하는 정보를 세팅.
    SetCurrentDoInfo();
    m_bDBUpdateCheck = true;    // 2번째 호출부터는 DB에 Update가 일어났기 때문에 해당 값 갱신.
}

// 현재 진행중인 Grade 설정.
void KCnDonationManager::SetCurrentDoInfo()
{
    std::vector<KItemDonationInfo>::const_iterator vit;
    for( vit=m_vecTotalInfo.begin(); vit != m_vecTotalInfo.end(); ++vit )
    {
        // GradeState로 연산해서 처음으로 0이 나온 Grade가 달성해야 하는 Grade 정보다.
        if( vit->m_dwGradeState == 0 )
        {
            m_kCurrentGrade = *vit;
            return;
        }
    }

    if( m_vecTotalInfo.empty() ) return; // 컨테이너가 비어 있다면 아래쪽으로 내려가지 않도록.

    // 5단계까지 달성했으면 현재 진행중인 정보를 5단계로 세팅해 두자.
    KItemDonationInfo& kItemDuInfo = m_vecTotalInfo.back();
    m_kCurrentGrade = kItemDuInfo;
}

// 현재 진행중인 Grade 정보 가져오기.
void KCnDonationManager::GetCurrentGradeInfo( OUT KItemDonationInfo& kCurrentGradeInfo_ )
{
    KLocker lock( m_csDonation );
    kCurrentGradeInfo_ = m_kCurrentGrade;
}

// 게임서버에 보내주기 위한 전체 등급별 정보 가져오기.
void KCnDonationManager::GetTotalGradeInfo( OUT KECN_ITEM_DONATION_UPDATE_NOT& kPacket_ ) const
{
    kPacket_.m_vecGradeInfo = m_vecTotalInfo;
}

// Center -> GameServer
// 전체 기부 Grade List 전달.
void KCnDonationManager::SendToDonationInfo()
{
    KECN_ITEM_DONATION_UPDATE_NOT kPacket;
    GetTotalGradeInfo( kPacket );
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_ITEM_DONATION_UPDATE_NOT, kPacket );
    m_bDBUpdateCheck = false;
}

// Tick에서 체크
// DB에 Grade List 값들의 변경이 있는지 체크하는 함수.
void KCnDonationManager::QueuingToDB( IN const bool bFirst_ )
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_ITEM_DONATION_UPDATE, L"", (bFirst_?1:0) );
}
