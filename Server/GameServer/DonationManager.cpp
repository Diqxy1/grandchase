#include "CnConnector.h" // winsock
#include "DonationManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "userevent.h"
#include <boost/bind.hpp>
#include "SignBoard.h"
#include "LoudMessage.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KDonationManager );
ImplOstmOperatorW2A( KDonationManager );
NiImplementRootRTTI( KDonationManager );

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

KDonationManager::KDonationManager(void)
:m_biTotalDonation(0)
,m_biDeltaDonation(0)
,m_nCurrentDSState(DS_NONE)
,m_tmBegin(0)
,m_tmEnd(0)
,m_tmDisplay(0)
,m_dwMaxRankSize(5)
,m_dwDonationUnit(0)
,m_nMinDonationPoint(100)
,m_bDonationMaxGradeCheck(true)
,m_dwMaxGrade(0)
{
    for( int i = 0 ; i < DT_MAX ; ++i )
    {
        SetTick( i );
        m_dwaTickGap[i] = 0xFFFFFFFF;
    }
}

KDonationManager::~KDonationManager(void)
{
}

ImplToStringW( KDonationManager )
{
    KLocker lock( m_csDonation );
    START_TOSTRINGW
        << TOSTRINGW( m_biTotalDonation )
        << TOSTRINGW( m_biDeltaDonation )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWt( CTime(m_tmDisplay) )
        << TOSTRINGW( m_nCurrentDSState )
        << TOSTRINGW( m_dwDonationUnit )
        << TOSTRINGW( m_nMinDonationPoint )
        << TOSTRINGW( m_dwaTickGap[DT_TICK] )
        << TOSTRINGW( m_dwaTickGap[DT_SYNC] )
        << TOSTRINGW( m_vecItemGradeInfo.size() )
        << TOSTRINGW( m_kCurrentDonation.m_dwGrade )
        << TOSTRINGW( m_kCurrentDonation.m_biGradeNum )
        << TOSTRINGW( m_kCurrentDonation.m_vecRewardList.size() )
        << TOSTRINGW( m_kCurrentDonation.m_dwGradeState );

    DumpDonationReward( stm_, m_userReward, L"User Acc Reward" );
    DumpDonationReward( stm_, m_donationReward, L"Donation Reward" );
    return stm_;

}

void KDonationManager::PrintRank()
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

bool KDonationManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KDonationManager::_LoadScript( OUT std::string& strScript_ )
{
    time_t  tmBegin;
    time_t  tmEnd;
    time_t  tmDisplay;
    DWORD   dwaTickGap[DT_MAX];
    int     nMinDonationPoint;
    DWORD   dwDonationUnit;
    sDonationReward userReward;
    sDonationReward donationReward;
    DWORD   dwMaxGrade;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitDonationInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );
    _JIF( LoadTime( kLuaMng, std::string("DisplayTime"), tmDisplay ), return false );

    _JIF( kLuaMng.GetValue( "TickGap", dwaTickGap[DT_TICK] ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "SyncDonationGap", dwaTickGap[DT_SYNC] ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MinPoint", nMinDonationPoint ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DonationPointType", dwDonationUnit ) == S_OK, return false );
    _JIF( LoadReward( kLuaMng, std::string("UserReward"), userReward ), return false );
    _JIF( LoadReward( kLuaMng, std::string("DonationReward"), donationReward ), return false );
    _JIF( kLuaMng.GetValue( "MaxGrade", dwMaxGrade ) == S_OK, return false );

    START_LOG( cerr, L"기부 시스템 로드 완료. " ) << END_LOG;

    KLocker lock( m_csDonation );
    m_tmBegin               = tmBegin;
    m_tmEnd                 = tmEnd;
    m_tmDisplay             = tmDisplay;
    m_nMinDonationPoint     = nMinDonationPoint;
    m_dwDonationUnit        = dwDonationUnit;
    m_userReward            = userReward;
    m_donationReward        = donationReward;
    m_dwMaxGrade            = dwMaxGrade;       // 아이템 기부 최대 Grade값 루아파일로 설정.
    ::memcpy( m_dwaTickGap, dwaTickGap, sizeof( dwaTickGap ) );
    UpdateState();
    return true;
}

bool KDonationManager::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KDonationManager::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT sDonationReward& rewardInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "Point", rewardInfo_.m_nDonationPoint ) == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "Reward" ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo Data;
        if( kLuaMng_.GetValue( i++, Data.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, Data.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, Data.m_nPeriod ) == S_OK, return false );
        rewardInfo_.m_vecReward.push_back( Data );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KDonationManager::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();
    if( dwCurrentTick - GetTick( DT_TICK ) < GetTickGap( DT_TICK ) ) return;

    START_LOG(clog, L" 기부 시스템 Tick... ^^" ) << END_LOG;
    // 이 때 할것이 현재 상태 업데이트
    UpdateState();

    // 센터 서버와의 동기화..
    if( dwCurrentTick - GetTick( DT_SYNC ) > GetTickGap( DT_SYNC ) )
    {
        SendDeltaDonationToCenter();
    }

    SetTick( DT_TICK );
}

void KDonationManager::UpdateState()
{
    CTime tmBegin( m_tmBegin );
    CTime tmEnd( m_tmEnd );
    CTime tmDisplay( m_tmDisplay );
    CTime tmCurrent = CTime::GetCurrentTime();

    if( tmBegin > tmCurrent || tmDisplay < tmCurrent )
    {
        KLocker lock( m_csDonation );
        m_nCurrentDSState = DS_NONE;
        return;
    }

    if( tmEnd < tmCurrent && tmDisplay >= tmCurrent )
    {
        KLocker lock( m_csDonation );
        m_nCurrentDSState = DS_DISPLAY;
        return;
    }

    if( tmBegin <= tmCurrent && tmEnd >= tmCurrent )
    {
        KLocker lock( m_csDonation );
        m_nCurrentDSState = DS_DONATION;
        return;
    }
}

void KDonationManager::UpdateTotalDonationFromCenter( __int64 biTotalDonation_ )
{
    KLocker lock( m_csDonation );
    m_biTotalDonation = biTotalDonation_;
}

bool KDonationManager::SendDeltaDonationToCenter()
{
    KDotantionSyncData kSyncData;
    {
        KLocker lock( m_csDonation );
        kSyncData.m_biTotalDonationPoint = m_biDeltaDonation;
        GetDonationRank( kSyncData.m_vecDonationRank );
        m_biTotalDonation += m_biDeltaDonation;
        m_biDeltaDonation = 0;
    }

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_DONATION_REQ, kSyncData ) );
    
    return false;
}

int KDonationManager::GetDonationState()
{
    KLocker lock( m_csDonation );
    return m_nCurrentDSState;
}

void  KDonationManager::GetDonationReword( int nCurrentPoint_, int nDeltaPoint_, OUT std::vector<KDropItemInfo>& vecItem_ )
{
    vecItem_.clear();

    KLocker lock( m_csDonation );
    if( m_donationReward.m_nDonationPoint <= nDeltaPoint_ )
    {
        AddReward( m_donationReward.m_vecReward, vecItem_ );
    }

    if( m_userReward.m_nDonationPoint > nCurrentPoint_ && 
        m_userReward.m_nDonationPoint <= nCurrentPoint_ + nDeltaPoint_ )
    {
        AddReward( m_userReward.m_vecReward, vecItem_ );
    }

}

__int64 KDonationManager::GetTotalDonation()
{
    KLocker lock( m_csDonation );
    return m_biTotalDonation + m_biDeltaDonation;
}

DWORD KDonationManager::GetTickGap( int nIndex_ )
{
    if( nIndex_ < 0 || nIndex_ >= DT_MAX ) return INT_MAX;

    return m_dwaTickGap[nIndex_];
}

DWORD KDonationManager::GetTick( int nIndex_ )
{
    if( nIndex_ < 0 || nIndex_ >= DT_MAX ) return INT_MAX;
    return m_dwaLastTick[nIndex_];
}

void KDonationManager::SetTick( int nIndex_ )
{
    if( nIndex_ < 0 || nIndex_ >= DT_MAX ) return;
    m_dwaLastTick[nIndex_] = ::GetTickCount();
}

void KDonationManager::UpdateDonationRankFromCenter( IN std::vector<KDotantionRank>& vecRank_ )
{
    KLocker lock( m_csDonation );
    m_vecDonationRank = vecRank_;
}

void KDonationManager::GetDonationRank( OUT std::vector<KDotantionRank>& vecRank )
{
    vecRank.clear();
    KLocker lock( m_csDonation );
    vecRank = m_vecDonationRank;
}

int KDonationManager::GetMinDonationPoint()
{
    KLocker lock( m_csDonation );
    return m_nMinDonationPoint;
}

void KDonationManager::InitProcess()
{
    if( GetDonationState() == DS_NONE ) return;
    SendDeltaDonationToCenter();
}

void KDonationManager::UpdateDonationRankPerUser( IN int nAccDonationPoint_, IN std::wstring strNick_ )
{
    if( GetLastAccDonationPoint() >= nAccDonationPoint_ ) return;

    KDotantionRank kRank;
    kRank.m_nAccDonationPoint   = nAccDonationPoint_;
    kRank.m_strNickName         = strNick_;

    KLocker lock( m_csDonation );

    std::vector<KDotantionRank>::iterator vit;
    vit = std::find_if( m_vecDonationRank.begin(), m_vecDonationRank.end(),
        boost::bind( &KDotantionRank::m_strNickName, _1) == strNick_ );

    if( vit != m_vecDonationRank.end() )
    {
        vit->m_nAccDonationPoint = std::max<int>( vit->m_nAccDonationPoint, nAccDonationPoint_ );
    }
    else
    {
        m_vecDonationRank.push_back( kRank );
    }

    SortAndNumbering( m_vecDonationRank );
}

void KDonationManager::SortAndNumbering( IN OUT std::vector<KDotantionRank>& vecRank_ )
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

int KDonationManager::GetLastAccDonationPoint()
{
    KLocker lock( m_csDonation );
    if( m_vecDonationRank.empty() ) return 0;
    if( m_vecDonationRank.size() < m_dwMaxRankSize ) return 0;
    return m_vecDonationRank.rbegin()->m_nAccDonationPoint;
}

void KDonationManager::AddDeltaDonationPoint( int nDeltaPoint_ )
{
    KLocker lock( m_csDonation );
    m_biDeltaDonation += nDeltaPoint_;

    // 현재 누적 금액에 대해서 기부 달성에 대한 부분을 체크하자.
    __int64 biTotalDonation = GetTotalDonation();

    if( biTotalDonation >= m_kCurrentDonation.m_biGradeNum && m_bDonationMaxGradeCheck )
    {
        // GameServer -> Client
        // 기부 Grade 변경에 대한 운영자 공지 전송.
        NotifyFromCenter( m_kCurrentDonation.m_dwGrade );

        // 최대 등급치가 되면 공지 출력 그만하고 끝낸다.
        if( m_kCurrentDonation.m_dwGrade == m_dwMaxGrade )
        {
            m_bDonationMaxGradeCheck = false;
            // 해당 등급의 Grade정보를 1로 바꾸는 함수 호출.
            SetUpdateGrade( m_kCurrentDonation.m_dwGrade );
        }
        else
        {
            // 해당 등급의 Grade정보를 1로 바꾸는 함수 호출.
            SetUpdateGrade( m_kCurrentDonation.m_dwGrade );
            // 현재 달성해야 하는 Grade정보를 수정하자.
            SetCurrentDonation();
        }
    }
}

void KDonationManager::DumpDonationReward( std::wostream& stm_, const sDonationReward& kReward_, wchar_t* szName_ ) const
{
    stm_ << L"    " << szName_ << L" : [" << kReward_.m_nDonationPoint << L"] = ";
    std::vector<KDropItemInfo>::const_iterator vit;
    for( vit = kReward_.m_vecReward.begin() ; vit != kReward_.m_vecReward.end() ; ++vit )
    {
        stm_ << L"(" << vit->m_ItemID
            << L"," << vit->m_nDuration
            << L"," << vit->m_nPeriod
            << L"), ";
    }
    stm_ << std::endl;
}

void KDonationManager::AddReward( IN const std::vector<KDropItemInfo>& vecReward_, IN OUT std::vector<KDropItemInfo>& vecItem_ )
{
    std::vector<KDropItemInfo>::const_iterator vit;
    for( vit = vecReward_.begin() ; vit != vecReward_.end() ; ++vit ) {
        vecItem_.push_back( *vit );
    }
}
void KDonationManager::Clear()
{
    KLocker lock( m_csDonation );
    m_vecDonationRank.clear();
    m_biTotalDonation = 0;
    m_biDeltaDonation = 0;
}

DWORD KDonationManager::GetDonationUnit()
{
    KLocker lock( m_csDonation );
    return m_dwDonationUnit;
}

// Center -> GameServer
// CenterServer로부터 받은 전체 GradeList를 갱신한다.
void KDonationManager::UpdateFromCenter( IN KECN_ITEM_DONATION_UPDATE_NOT& kPacket_ )
{
    KLocker lock( m_csDonation );
    m_vecItemGradeInfo = kPacket_.m_vecGradeInfo;

    SetCurrentDonation();
}

// 현재 기부가 진행중인 Grade를 저장하고 있자.
void KDonationManager::SetCurrentDonation()
{
    KLocker lock( m_csDonation );
    // 현재 기부중인 등급의 정보를 연산해서 저장해두자.
    std::vector<KItemDonationInfo>::iterator vit;
    for( vit = m_vecItemGradeInfo.begin(); vit != m_vecItemGradeInfo.end(); ++vit )
    {
        if( vit->m_dwGradeState == 0)
        {
            m_kCurrentDonation = *vit;
            return;
        }
    }

    if( m_vecItemGradeInfo.empty() ) return; // 컨테이너가 비어 있다면 아래쪽으로 내려가지 않도록.

    // 5단계까지 달성했으면 현재 진행중인 정보를 5단계로 세팅해 두자.
    // 마지막 데이터를 가져올 때 vit = -- m_vecItemGradeInfo.end()를 사용했는데( return iterator )
    // 이 부분을 m_vecItemGradeInfo.back()으로 수정.( return data )
    KItemDonationInfo& kItemDuInfo = m_vecItemGradeInfo.back();
    m_kCurrentDonation = kItemDuInfo;
    m_bDonationMaxGradeCheck = false;   // 공지출력할 필요도 없다.
}

// 클라이언트에게 아이템 지급을 위해서 해당 등급에 맞는 정보를 가져오자.
void KDonationManager::GetGradeInfo( IN const DWORD& dwGrade_, OUT KItemDonationInfo& kGradeInfo_ )
{
    KLocker lock( m_csDonation );
    std::vector<KItemDonationInfo>::iterator vit;
    for( vit = m_vecItemGradeInfo.begin(); vit != m_vecItemGradeInfo.end(); ++vit )
    {
        if( dwGrade_ == vit->m_dwGrade )
        {
            kGradeInfo_ = *vit;
        }
    }
}

// 클라이언트에게 보내주기 위한 전체 등급별 정보 가져오기.
void KDonationManager::GetTotalGradeInfo( OUT std::vector<KItemDonationInfo>& vecDoInfo_ )
{
    KLocker lock( m_csDonation );
    vecDoInfo_.clear();
    vecDoInfo_ = m_vecItemGradeInfo;
}

void KDonationManager::NotifyFromCenter( IN const DWORD& dwGrade_ )
{
    // 이 함수가 호출되었는 것은 공지를 출력해야 한다는 것이다.
    KLocker lock( m_csDonation );

    KSignBoardData kData;
    kData.m_dwColor         = 0L;
    kData.m_ItemID        = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
    kData.m_strSenderNick.clear();
    if( SiKLoudMessage()->GetItemDonationMsg( dwGrade_, kData.m_strMsg ) )
        SiKSignBoard()->QueueingAdminData( kData );
}

// 등급이 변경되면 해당함수 호출로 등급의 수정을 저장해 두자.
// 클라이언트에게 비활성, 활성을 서버가 계산해서 전달하기 위해서.
void KDonationManager::SetUpdateGrade( IN const DWORD& dwGrade )
{
    KLocker lock( m_csDonation );
    std::vector<KItemDonationInfo>::iterator vit;
    for( vit = m_vecItemGradeInfo.begin(); vit != m_vecItemGradeInfo.end(); ++vit )
    {
        if( dwGrade == vit->m_dwGrade )
        {
            vit->m_dwGradeState = 1;
        }
    }
}