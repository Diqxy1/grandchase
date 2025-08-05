#include "GSSimLayer.h"
#include "CnConnector.h" // winsock
#include "DicePlay.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "UserPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include <boost/bind.hpp>
#include "Log4.h"

ImplementSingleton( KDicePlay );
ImplOstmOperatorW2A( KDicePlay );
NiImplementRootRTTI( KDicePlay );

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

KDicePlay::KDicePlay(void)
:m_dwToalMapCount(0)
,m_dwMaxGoal(0)
,m_dwDailyMax(0)
,m_dwDailyFree(0)
,m_dwTickGap(1000*60*60)
,m_dwSyncTickGap(1000*60*60)
,m_dwInitHour(0)
,m_tmBegin(1)
,m_tmEnd(1)
,m_dwMaxRankSize(3)
,m_dwDiceItemID(203210)
,m_dwBoardItemID(203200)
,m_dwVersion(0)
{
    m_mapGoalReward.clear();
    m_vecMissionList.clear();
    m_mapBoardReward.clear();
    m_mapBoardMapInfo.clear();
    m_vecDicePlayRank.clear();

    m_dwLastTick = ::GetTickCount();
}

KDicePlay::~KDicePlay(void)
{
}

ImplToStringW( KDicePlay )
{
    KLocker lock( m_csDicePlay );
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_dwSyncTickGap )
        << TOSTRINGW( m_dwToalMapCount )
        << TOSTRINGW( m_dwMaxGoal )
        << TOSTRINGW( m_dwDailyMax )
        << TOSTRINGW( m_dwDailyFree )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwInitHour )
        << TOSTRINGW( m_dwDiceItemID )
        << TOSTRINGW( m_dwBoardItemID )
        << TOSTRINGW( m_vecMissionList.size() )
        << TOSTRINGW( m_mapBoardReward.size() )
        << TOSTRINGW( m_mapBoardMapInfo.size() );
}

void KDicePlay::PrintRank()
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

bool KDicePlay::LoadScript()
{
    return _LoadScript(std::string());
}

bool KDicePlay::_LoadScript( OUT std::string& strScript_ )
{
    time_t      tmBegin;
    time_t      tmEnd;
    DWORD       dwSyncTickGap;
    DWORD       dwTotalMapCount;
    DWORD       dwMaxGoal;
    DWORD       dwDailyMax;
    DWORD       dwDailyFree;
    DWORD       dwTickGap;
    DWORD       dwInitHour;
    GCITEMID       dwDiceItemID;
    GCITEMID       dwBoardItemID;
    DWORD       dwVersion;
    VEC_REWARD vecItemOne;
    VEC_REWARD vecItemLast;
    std::vector<DWORD> vecMissionList;
    std::map<DWORD, VEC_REWARD> mapRewardInfo;
    std::map<DWORD,DWORD> mapBoardMapInfo;
    std::map<DWORD, VEC_REWARD> mapGoalReward;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitBoardInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 이벤트 기간 읽어오기.
    _JIF( LoadTime( kLuaMng, std::string("BeginTime"), tmBegin ), return false );
    _JIF( LoadTime( kLuaMng, std::string("EndTime"), tmEnd ), return false );

    // 이벤트 설정값 읽어오기.
    _JIF( kLuaMng.GetValue( "TICK_SYNC_GAP", dwSyncTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TOTAL_MAP_COUNT", dwTotalMapCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MAX_GOAX", dwMaxGoal ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DAILY_MAX", dwDailyMax ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DAILY_FREE", dwDailyFree ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "FREE_TICK_GAP", dwTickGap ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UPDATE_HOUR", dwInitHour ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DICE_ITEMID", dwDiceItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "BOARD_ITEMID", dwBoardItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "BOARD_VERSION", dwVersion ) == S_OK, return false );

    // 미션 보상 미션 정보 읽어오기.
    _JIF( LoadMission( kLuaMng, std::string("SpecialMission"), vecMissionList ), return false );

    // 말판 정보, 보상 정보 읽어오기.
    _JIF( LoadTypeReward( kLuaMng, std::string("InitPositionValue"), mapRewardInfo ), return false );
    _JIF( LoadMapInfo( kLuaMng, std::string("InitBoardGameInfo"), mapBoardMapInfo ), return false );
    _JIF( LoadGoalReward( kLuaMng, std::string("InitGoalReward"), mapGoalReward ), return false );

    START_LOG( cerr, L"주사위 정보 로드 완료. " ) << END_LOG;

    KLocker lock( m_csDicePlay );
    m_tmBegin            = tmBegin;
    m_tmEnd              = tmEnd;
    m_dwSyncTickGap      = dwSyncTickGap;
    m_dwToalMapCount     = dwTotalMapCount;
    m_dwMaxGoal          = dwMaxGoal;
    m_dwDailyMax         = dwDailyMax;
    m_dwDailyFree        = dwDailyFree;
    m_dwTickGap          = dwTickGap;
    m_dwInitHour         = dwInitHour;
    m_dwDiceItemID       = dwDiceItemID;
    m_dwBoardItemID      = dwBoardItemID;
    m_dwVersion          = dwVersion;
    m_vecMissionList     = vecMissionList;
    m_mapBoardReward     = mapRewardInfo;
    m_mapBoardMapInfo    = mapBoardMapInfo;
    m_mapGoalReward      = mapGoalReward;

    return true;
}

bool KDicePlay::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
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

bool KDicePlay::LoadReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_REWARD& vecRewardInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecRewardInfo_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KDicePlay::LoadMission( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<DWORD>& vecMissionList_ )
{
     _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

     int i = 1;
     while( true )
     {
         DWORD dwData;
         if( kLuaMng_.GetValue( i++, dwData ) != S_OK ) break;
         vecMissionList_.push_back( dwData );
     }

     _JIF( kLuaMng_.EndTable() == S_OK, return false );
     return true;
}

bool KDicePlay::LoadTypeReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,VEC_REWARD>& mapRewardInfo_ )
{
    VEC_REWARD vecItemInfo;
    vecItemInfo.clear();
    DWORD dwType;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "TYPE", dwType ) == S_OK, return false );
        _JIF( kLuaMng_.BeginTable( "ItemList" ) == S_OK, return false );
        _JIF( LoadRewardList( kLuaMng_, vecItemInfo ), return false );
         _JIF( kLuaMng_.EndTable() == S_OK, return false );
        if( !mapRewardInfo_.insert( std::make_pair( dwType, vecItemInfo ) ).second )
        {
            START_LOG( cerr, L" 중복된 보드 말판 보상 정보 있음. TYPE : " << dwType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vecItemInfo.clear();
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );           // InitPositionValue
    return true;
}

bool KDicePlay::LoadGoalReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,VEC_REWARD>& mapGoalReward_ )
{
    VEC_REWARD vecItemInfo;
    vecItemInfo.clear();
    DWORD dwType;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "GOAL", dwType ) == S_OK, return false );
        _JIF( kLuaMng_.BeginTable( "ItemList" ) == S_OK, return false );
        _JIF( LoadRewardList( kLuaMng_, vecItemInfo ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        if( !mapGoalReward_.insert( std::make_pair( dwType, vecItemInfo ) ).second )
        {
            START_LOG( cerr, L" 중복된 바퀴 보상정보 있음. GOAL : " << dwType ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        vecItemInfo.clear();
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );           // InitGoalReward
    return true;
}

// 보상 아이템 정보 읽어오기.
bool KDicePlay::LoadRewardList( IN KLuaManager& kLuaMng_, OUT VEC_REWARD& vecReward_ )
{
    vecReward_.clear();
    int nIndex = 0;
    while( true )
    {
        KDropItemInfo kReward;
        if( kLuaMng_.GetValue( ++nIndex, kReward.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( ++nIndex, kReward.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( ++nIndex, kReward.m_nPeriod ) == S_OK, return false );
        vecReward_.push_back( kReward );
    }
    return true;
}

bool KDicePlay::LoadMapInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<DWORD,DWORD>& mapBoardMapInfo_ )
{
    DWORD dwMapID;
    DWORD dwProperties;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "MapID", dwMapID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Properties", dwProperties ) == S_OK, return false );

        if( !mapBoardMapInfo_.insert( std::make_pair( dwMapID, dwProperties ) ).second )
        {
            START_LOG( cerr, L" 중복된 보드 말판 정보 있음. MapID : " << dwMapID ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );           // InitBoardGameInfo
    return true;
}

void KDicePlay::InitProcess()
{
    if( IsRun() )
    {
        SendDicePlayRankToCenter();
    }
}

bool KDicePlay::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csDicePlay );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KDicePlay::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();
    if( dwCurrentTick - m_dwLastTick < m_dwSyncTickGap ) return;
    m_dwLastTick = ::GetTickCount();

    // 이벤트 기간일때만 CenterServer에 정보 전송.
    if( IsRun() ) {
        // 센터 서버와의 동기화.
        SendDicePlayRankToCenter();
        START_LOG(clog, L" 주사위 이벤트 센터서버와의 동기화." ) << END_LOG;
        // 초기화 업데이트를 할지 체크.
        if( CheckUpdateFromDB() ) {
            SiKGSDBLayer()->QueueingID( KUserEvent::DB_EVENT_INIT_DICE_DAILY_FREE, L"", 0, 0 );
        }
    }
}

DWORD KDicePlay::GetTotalMapCount()
{
    KLocker lock( m_csDicePlay );
    return m_dwToalMapCount;
}

DWORD KDicePlay::GetMaxGoal()
{
    KLocker lock( m_csDicePlay );
    return m_dwMaxGoal;
}

DWORD KDicePlay::GetDailyMax()
{
    KLocker lock( m_csDicePlay );
    return m_dwDailyMax;
}

DWORD KDicePlay::GetDailyFree()
{
    KLocker lock( m_csDicePlay );
    return m_dwDailyFree;
}

DWORD KDicePlay::GetFreeTickGap()
{
    KLocker lock( m_csDicePlay );
    return m_dwTickGap;
}

DWORD KDicePlay::GetInitHour()
{
    KLocker lock( m_csDicePlay );
    return m_dwInitHour;
}

GCITEMID KDicePlay::GetDiceItemID()
{
    KLocker lock( m_csDicePlay );
    return m_dwDiceItemID;
}

GCITEMID KDicePlay::GetBoardItemID()
{
    KLocker lock( m_csDicePlay );
    return m_dwBoardItemID;
}

// 보드게임 전체 맵 정보 리턴해 주는 함수.
void KDicePlay::GetMapInfo( OUT BOARD_GAME_INFO& mapBoardMapInfo_ )
{
    KLocker lock( m_csDicePlay );
    mapBoardMapInfo_ = m_mapBoardMapInfo;
}

// 보드게임 칸의 Type을 가져오자.
bool KDicePlay::GetMapIDType( IN DWORD& dwType_, OUT DWORD& dwProperty_ )
{
    std::map<DWORD, DWORD>::iterator mit;
    KLocker lock( m_csDicePlay );
    mit = m_mapBoardMapInfo.find( dwType_ );
    _JIF( mit != m_mapBoardMapInfo.end(), return false );
    dwProperty_ = mit->second;
    return true;
}

// 전체 보상 아이템 리스트 읽어오기.
void KDicePlay::GetRewardTotalList( OUT std::map<DWORD, VEC_REWARD>& mapBoardReward_ )
{
    KLocker lock( m_csDicePlay );
    mapBoardReward_ = m_mapBoardReward;
}

// 보드게임 칸의 보상 아이템 리스트.가져오기.
bool KDicePlay::GetRewardInfo( IN DWORD& dwType_, OUT VEC_REWARD& vecRewardInfo_ )
{
    vecRewardInfo_.clear();
    std::map<DWORD, VEC_REWARD>::iterator mit;
    KLocker lock( m_csDicePlay );
    mit = m_mapBoardReward.find( dwType_ );
    _JIF( mit != m_mapBoardReward.end(), return false );
    vecRewardInfo_ = mit->second;
    return true;
}

// 바퀴 보상 아이템 리스트.
bool KDicePlay::GetGoalReward( IN DWORD& dwGoal_, OUT VEC_REWARD& vecGoalReward_ )
{
    vecGoalReward_.clear();
    std::map<DWORD, VEC_REWARD>::iterator mit;
    KLocker lock( m_csDicePlay );
    mit = m_mapGoalReward.find( dwGoal_ );
    _JIF( mit != m_mapGoalReward.end(), return false );
    vecGoalReward_ = mit->second;
    return true;
}

// 미션 속상칸의 미션 정보 읽어오기.
void KDicePlay::GetMissionList( OUT std::vector<DWORD>& vecMissionList_ )
{
    KLocker lock( m_csDicePlay );
    vecMissionList_ = m_vecMissionList;
}

// 주사위 굴리기.
void KDicePlay::GetDicePlayNum( OUT DWORD& dwFirstNum_, OUT DWORD& dwSecondNum_ )
{
    KLocker lock( m_csDicePlay );
    srand(::GetTickCount());
    dwFirstNum_  = (rand()%6)+1;
    dwSecondNum_ = (rand()%6)+1;
}

// 랭킹 정보 가져오기.
void KDicePlay::GetDicePlayRank( OUT std::vector<KDicePlayRank>& vecRank_ )
{
    vecRank_.clear();
    KLocker lock( m_csDicePlay );
    vecRank_ = m_vecDicePlayRank;
}

// GameServer -> Center 주사위 이벤트 랭킹 정보 보내기.
bool KDicePlay::SendDicePlayRankToCenter()
{
    KDicePlaySyncData kSyncData;
    {
        KLocker lock( m_csDicePlay );
        GetDicePlayRank( kSyncData.m_vecDicePlayRank ); // 주사위 이벤트 랭킹 정보 가져오기.
    }

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_DICE_PLAY_REQ, kSyncData ) );
    return false;
}

// Center -> GameServer 랭킹 정보 전송.
void KDicePlay::UpdateDicePlayRankFromCenter( IN std::vector<KDicePlayRank>& vecRank_ )
{
    KLocker lock( m_csDicePlay );
    m_vecDicePlayRank = vecRank_;
}

// 랭킹정보 최대 숫자만큼 가지고 있기.
void KDicePlay::SortAddNumbering( IN OUT std::vector<KDicePlayRank>& vecRank_ )
{
    std::sort( vecRank_.begin(), vecRank_.end(), KComp() );

    if( vecRank_.size() > m_dwMaxRankSize )
        vecRank_.resize( m_dwMaxRankSize );

    KLocker lock( m_csDicePlay );
    std::vector<KDicePlayRank>::iterator vit;
    int nRank = 0;
    for( vit = vecRank_.begin(); vit != vecRank_.end(); ++vit )
    {
        vit->m_nRank = ++nRank;
    }
}

// 주사위 굴릴때마다 랭킹 정보 갱신.
void KDicePlay::UpdateDicePlayRankPerUser( IN DWORD dwTotalCount_, IN DWORD dwCurrentPosition_, IN std::wstring strNick_ )
{
    KDicePlayRank kRank;
    kRank.m_dwTotalCount = dwTotalCount_;
    kRank.m_dwCurrentPosition = dwCurrentPosition_;
    kRank.m_strNickName = strNick_;

    KLocker lock( m_csDicePlay );

    std::vector<KDicePlayRank>::iterator vit;
    vit = std::find_if( m_vecDicePlayRank.begin(), m_vecDicePlayRank.end(), 
        boost::bind( &KDicePlayRank::m_strNickName, _1 ) == strNick_ );

    if( vit != m_vecDicePlayRank.end() )
    {
        vit->m_dwTotalCount = std::max<DWORD>( vit->m_dwTotalCount, dwTotalCount_ );
        vit->m_dwCurrentPosition = std::max<DWORD>( vit->m_dwCurrentPosition, dwCurrentPosition_ );
    }
    else
    {
        m_vecDicePlayRank.push_back( kRank );
    }

    SortAddNumbering( m_vecDicePlayRank );
}

bool KDicePlay::CheckUpdateFromDB()
{
    KLocker lock( m_csDicePlay );

    CTime tmCurrent = CTime::GetCurrentTime();
    // 초기화 시간에 한번만 DB에 데이터 초기화 갱신을 해준다.
    if( tmCurrent.GetHour() == m_dwInitHour && tmCurrent.GetMinute() < 1 ) {
        return true;
    }
    return false;
}

bool KDicePlay::Get_DicePlay_UserData_FromDB( IN const std::string& strScript_ )
{
    DWORD dwTemp = 0;

    SiKGSSimLayer()->m_kActorManager.QueueingToAll( KUserEvent::EVENT_UPDATEPLAN_BOARD_GAME_USER_INFO_NOT, dwTemp );

    return true;
}
