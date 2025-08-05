#include "QuizBingo.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "UserPacket.h"
#include "CenterPacket.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "GSDBLayer.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KQuizBingo );
ImplOstmOperatorW2A( KQuizBingo );
NiImplementRootRTTI( KQuizBingo );

KQuizBingo::KQuizBingo(void)
:m_nEventID(0)
,m_tmBegin(1)
,m_tmEnd(1)
,m_bIsEventTerm(false)
,m_nBoardSize(10)
,m_dwBingoBoardID(1)
,m_dwResurrectionID(1) //
,m_dwBingoCoinID(1)    //
,m_nDailyMaxCoinCount(5)
,m_nCoinSupplyInterval(60)
,m_nCoinSupplyCount(1)
,m_dwTickGap(60000)
{
    m_dwLastTick = ::GetTickCount();
}

KQuizBingo::~KQuizBingo(void)
{
}

ImplToStringW( KQuizBingo )
{
    KLocker lock(m_csBingo);
    return START_TOSTRINGW
        << TOSTRINGW( m_nEventID )
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGWb( m_bIsEventTerm )
        << TOSTRINGW( m_nBoardSize )
        << TOSTRINGW( m_dwBingoBoardID )
        << TOSTRINGW( m_dwResurrectionID )
        << TOSTRINGW( m_dwBingoCoinID )
        << TOSTRINGW( m_nDailyMaxCoinCount )
        << TOSTRINGW( m_nCoinSupplyInterval )
        << TOSTRINGW( m_nCoinSupplyCount )
        << TOSTRINGW( m_mapBingoReward.size() )
        << TOSTRINGW( m_vecCorrectQuizReward.size() )
        << TOSTRINGW( m_mapQuestions.size() )
        << TOSTRINGW( m_mapCompressedQuestionList.size() )
        << TOSTRINGW( m_vecQuestionIDs.size() );
}

bool KQuizBingo::LoadScript()
{
    return _LoadScript(std::string());
}

bool KQuizBingo::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitQuizBingo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    int nBoardSize;
    GCITEMID dwBingoBoardID;
    GCITEMID dwResurrectionID;
    GCITEMID dwBingoCoinID;
    int nDailyMaxCoinCount;
    int nCoinSupplyInterval;
    int nCoinSupplyCount;
    std::map<int, VEC_ITEMS > mapBingoReward;
    VEC_ITEMS vecCorrectQuizReward;

    _JIF( kLuaMng.GetValue( "BoardSize", nBoardSize ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "BingoBoardItemID", dwBingoBoardID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ResurrectionItem", dwResurrectionID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "BingoCoinID", dwBingoCoinID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DailyMaxCoin", nDailyMaxCoinCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CoinSupplyInterval", nCoinSupplyInterval ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "CoinSupplyCount", nCoinSupplyCount ) == S_OK, return false );
    _JIF( LoadBingoReward( kLuaMng, std::string("BingoReward"), mapBingoReward ), return false );
    _JIF( LoadItems( kLuaMng, std::string("CorrectQuizReward"), vecCorrectQuizReward ), return false );
    START_LOG( cerr, L"퀴즈 빙고 스크립트 로드 완료. " ) << END_LOG;

    {
        KLocker lock( m_csBingo );
        m_nBoardSize = nBoardSize;
        m_dwBingoBoardID = dwBingoBoardID;
        m_dwResurrectionID = dwResurrectionID;
        m_dwBingoCoinID = dwBingoCoinID;
        m_nDailyMaxCoinCount = nDailyMaxCoinCount;
        m_nCoinSupplyInterval = nCoinSupplyInterval;
        m_nCoinSupplyCount = nCoinSupplyCount;
        m_mapBingoReward.swap( mapBingoReward );
        m_vecCorrectQuizReward.swap( vecCorrectQuizReward );
    }

    return true;
}

bool KQuizBingo::LoadBingoReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, VEC_ITEMS >& mapBingoReward_ )
{
    mapBingoReward_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    int i = 0;
    while( true )
    {
        if( kLuaMng_.BeginTable( ++i ) != S_OK ) break;

        int nCount = -1;
        VEC_ITEMS vecItems;
        _JIF( kLuaMng_.GetValue( "Count" , nCount ) == S_OK, return false );
        _JIF( LoadItems( kLuaMng_, std::string("Reward"), vecItems ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapBingoReward_.insert( std::make_pair( nCount, vecItems) ).second )
        {
            START_LOG( cerr, L"중복된 키가 있음.. Key : " << nCount ) << END_LOG;
            return false;
        }
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

//bool KQuizBingo::LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ )
//{
//    int nYear, nMonth, nDay, nHour;
//    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
//    _JIF( kLuaMng_.GetValue( 1, nYear ) == S_OK, return false );
//    _JIF( kLuaMng_.GetValue( 2, nMonth ) == S_OK, return false );
//    _JIF( kLuaMng_.GetValue( 3, nDay ) == S_OK, return false );
//    _JIF( kLuaMng_.GetValue( 4, nHour ) == S_OK, return false );
//    _JIF( kLuaMng_.EndTable() == S_OK, return false );
//
//    tmTime_ = KncUtil::TimeToInt( CTime ( nYear, nMonth, nDay, nHour, 0, 0 ) );
//    return true;
//}

bool KQuizBingo::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_ITEMS& vecItems_ )
{
    vecItems_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        vecItems_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KQuizBingo::UnCompressQuiz( IN std::map<int, KSerBuffer>& vecCompressedQuestionz_,
                                 OUT std::map<int,KBingoQuestionInfo>& mapQuestions_ )
{
    mapQuestions_.clear();
    std::map<int, KSerBuffer>::iterator mit;
    for( mit = vecCompressedQuestionz_.begin() ; mit != vecCompressedQuestionz_.end() ; ++mit )
    {
        KBingoQuestionInfo kQuestion;
        KSerBuffer kBuff = mit->second;
        kBuff.UnCompress();
        KSerializer ks;
        ks.BeginReading( &kBuff );
        if ( !ks.Get( kQuestion ) ) {
            continue;
        }
        ks.EndReading();
        mapQuestions_.insert( std::make_pair( kQuestion.m_nID, kQuestion ) );
    }
}

void KQuizBingo::UpdateBingoConfig( KECN_QUIZ_BINGO_DATA_NOT& kCenterPacket_ )
{
    {
        KLocker lock( m_csBingo );
        m_nEventID = kCenterPacket_.m_nEventID;
        m_tmBegin = kCenterPacket_.m_tmBegin;
        m_tmEnd = kCenterPacket_.m_tmEnd;
        m_mapCompressedQuestionList.swap( kCenterPacket_.m_mapCompressedQuestionList );
        UnCompressQuiz( m_mapCompressedQuestionList, m_mapQuestions );

        m_vecQuestionIDs.clear();
        std::map<int,KBingoQuestionInfo>::iterator mit;
        m_vecQuestionIDs.reserve( m_mapQuestions.size() );
        for ( mit = m_mapQuestions.begin() ; mit != m_mapQuestions.end() ; ++mit ) {
            m_vecQuestionIDs.push_back( mit->first );
        }
    }
    UpdateEventTerm();

    START_LOG( cerr, L"Quiz Bingo config Update..." ) << END_LOG;
}

bool KQuizBingo::CheckCoinRecvable( IN const int nAccPlayTime_ )
{
    return ( ( m_nDailyMaxCoinCount / std::max<int>(m_nCoinSupplyCount,1) ) * m_nCoinSupplyInterval > nAccPlayTime_);
}

void KQuizBingo::GetUnSolvedQuestion( IN const std::vector<int>& vecSolved_, OUT std::vector<int>& vecUnSolved_ )
{
    struct KRemover
    {
        std::vector<int> m_vecData;
        KRemover( const std::vector<int>& vecData):m_vecData(vecData){}
        bool operator() ( int nData )
        {
            return (std::find( m_vecData.begin(), m_vecData.end(), nData ) != m_vecData.end());
        }
    };

    vecUnSolved_.clear();
    { // Locking Scope
        KLocker lock( m_csBingo );
        if ( m_vecQuestionIDs.empty() )
            return;

        std::remove_copy_if( m_vecQuestionIDs.begin(), m_vecQuestionIDs.end(),
            std::back_inserter( vecUnSolved_ ), KRemover(vecSolved_) );
    }
}

bool KQuizBingo::GetRandomQuestionID( IN const std::vector<int>& vecCorrectList_, OUT int& nQuestionID_ )
{
    std::vector<int> vecUnSolved;
    GetUnSolvedQuestion( vecCorrectList_, vecUnSolved );
    if ( vecUnSolved.empty() ) {
        return false;
    }

    std::random_shuffle( vecUnSolved.begin(), vecUnSolved.end() );

    nQuestionID_ = vecUnSolved.front();
    return true;
}

bool KQuizBingo::GetRandomQuestion( IN const std::vector<int>& vecCorrectList_, OUT KSerBuffer& kQuestion_, OUT int& nQuestionID_ )
{
    std::vector<int> vecUnSolved;
    GetUnSolvedQuestion( vecCorrectList_, vecUnSolved );

    if ( vecUnSolved.empty() ) {
        return false;
    }

    std::random_shuffle( vecUnSolved.begin(), vecUnSolved.end() );

    std::vector<int>::const_iterator vit;
    for ( vit = vecUnSolved.begin() ; vit != vecUnSolved.end() ; ++vit ) {
        if ( GetCompressedQuestion( *vit, kQuestion_ ) ) {
            nQuestionID_ = *vit;
            return true;
        }
    }
    return false;
}

bool KQuizBingo::GetCompressedQuestion( IN const int nQuestionID_, OUT KSerBuffer& kQuestionBuff_ )
{
    std::map<int,KSerBuffer>::const_iterator mit;
    KLocker lock( m_csBingo );

    mit = m_mapCompressedQuestionList.find( nQuestionID_ );
    if ( mit == m_mapCompressedQuestionList.end() ) return false;

    kQuestionBuff_ = mit->second;
    return true;
}

bool KQuizBingo::GetQuestion( IN const int nQuestionID_, OUT KBingoQuestionInfo& kQuestion_ )
{
    std::map<int,KBingoQuestionInfo>::const_iterator mit;
    KLocker lock( m_csBingo );

    mit = m_mapQuestions.find( nQuestionID_ );
    if ( mit == m_mapQuestions.end() ) return false;

    kQuestion_ = mit->second;
    return true;
}

bool KQuizBingo::GetLineBingoReward( IN const int nBingoLineCount_, IN OUT VEC_ITEMS& vecRewards_ )
{
    std::map<int, VEC_ITEMS >::const_iterator mit;
    KLocker lock( m_csBingo );
    mit = m_mapBingoReward.find( nBingoLineCount_ );

    if ( mit == m_mapBingoReward.end() ) return false;

    vecRewards_.insert( vecRewards_.end(), mit->second.begin(), mit->second.end() );
    return true;
}

void KQuizBingo::GetLineBingReward( IN const std::vector<int>& vecLineBingo_, IN OUT VEC_ITEMS& vecRewards_ )
{
    if ( vecLineBingo_.empty() ) return;

    std::vector<int>::const_iterator vit;
    for ( vit = vecLineBingo_.begin() ; vit != vecLineBingo_.end() ; ++vit ) {
        if ( GetLineBingoReward( *vit, vecRewards_ ) ) {
            GetLineBingoReward( 0, vecRewards_ );
        }
    }
}

void KQuizBingo::GetCorrectQuizReward( IN OUT VEC_ITEMS& vecRewards_ )
{
    KLocker lock( m_csBingo );
    vecRewards_.insert( vecRewards_.end(), m_vecCorrectQuizReward.begin(), m_vecCorrectQuizReward.end() );
}

void KQuizBingo::UpdateEventTerm()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KLocker lock(m_csBingo);
    m_bIsEventTerm = (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

bool KQuizBingo::CheckUpdateTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) return false;
    m_dwLastTick = ::GetTickCount();
    return true;
}

void KQuizBingo::Tick()
{
    // 이벤트 기간인지 검증만 한다.
    if ( !CheckUpdateTick() ) return;
    UpdateEventTerm();
}

//=========================================================================================//
KBingoUserData::KBingoUserData()
:m_bLoaded(false)
,m_nAccPlayTime(0)
,m_bFinishToday(true)
,m_nUseCoinCount(0)
,m_nCurrentBingoLineCount(0)
,m_nTotalCoinCount(0)
{
    ResetLastFail();
    m_vecCorrectQuestions.clear();
    m_vecBingoData.clear();
}

KBingoUserData::~KBingoUserData()
{

}

void KBingoUserData::SetBingoData( IN const KEVENT_USER_BINGO_DATA_ACK& kData_ )
{
    m_bLoaded = true;
    m_nAccPlayTime = kData_.m_nAccPlayTime;
    m_nTotalCoinCount = kData_.m_nTotalBingoCoinCount;
    m_nUseCoinCount = kData_.m_nUseBingoCoinCount;
    m_vecCorrectQuestions = kData_.m_vecCorrectQuestions;
    m_vecBingoData = kData_.m_vecBingoData;
    UpdateFinishToday();
    m_nCurrentBingoLineCount = CalcTotalBingoLine();
}

int KBingoUserData::GetCurrentBingoCoinCount()
{
    return std::max<int>( m_nTotalCoinCount - m_nUseCoinCount, 0 );
}

bool KBingoUserData::IsInBingoData( IN const std::pair<int,int>& prPos_ )
{
    return ( std::find( m_vecBingoData.begin(), m_vecBingoData.end(), prPos_ ) != m_vecBingoData.end() );
}

void KBingoUserData::GetCalculatedBingoLine( IN const std::pair<int,int>& prPos_, OUT std::vector<int>& vecLineBingo_ )
{
    vecLineBingo_.clear();
    int nBingoCount = 0;
    if ( CheckBingoVirtical( prPos_.first ) ) {
        ++nBingoCount;
    }

    if ( CheckBingoHorizon( prPos_.second ) ) {
        ++nBingoCount;
    }

    nBingoCount += CheckBingoDiagonalLine( prPos_ );

    for ( int i = nBingoCount + m_nCurrentBingoLineCount ; i > m_nCurrentBingoLineCount ; --i ) {
        vecLineBingo_.push_back( i );
    }
}

int KBingoUserData::CalcTotalBingoLine()
{
    int nRet = 0;
    for ( int i = 0 ; i < SiKQuizBingo()->GetBoardSize() ; ++i )
    {
        if ( CheckBingoVirtical( i ) ) {
            ++nRet;
        }

        if ( CheckBingoHorizon( i ) ) {
            ++nRet;
        }
    }

    nRet += CheckBingoDiagonalLine( std::pair<int,int>(1,1) );
    nRet += CheckBingoDiagonalLine( std::pair<int,int>(0,SiKQuizBingo()->GetBoardSize() -1 ) );
    return nRet;
}

bool KBingoUserData::CheckBingoVirtical( IN const int nXPos_ )
{

    int nCount = std::count_if( m_vecBingoData.begin(), m_vecBingoData.end(),
                                boost::bind( &std::pair<int,int>::first, _1 ) == nXPos_ );

    return (nCount == SiKQuizBingo()->GetBoardSize());
}

bool KBingoUserData::CheckBingoHorizon( IN const int nYPos_ )
{
    int nCount = std::count_if( m_vecBingoData.begin(), m_vecBingoData.end(),
        boost::bind( &std::pair<int,int>::second, _1 ) == nYPos_ );

    return (nCount == SiKQuizBingo()->GetBoardSize());
}

int KBingoUserData::CheckBingoDiagonalLine( IN const std::pair<int,int>& prPos_ )
{
    int nRet = 0;
    if ( prPos_.first == prPos_.second ) {

        int nCount = std::count_if( m_vecBingoData.begin(), m_vecBingoData.end(),
            boost::bind( &std::pair<int,int>::first, _1 ) == boost::bind( &std::pair<int,int>::second, _1 ) );
        if ( nCount == SiKQuizBingo()->GetBoardSize() ) {
            ++nRet;
        }

    } else if ( (prPos_.first + prPos_.second) == (SiKQuizBingo()->GetBoardSize() -1) ) {

        struct KSumComp {
            int m_nData;
            KSumComp( int nData ):m_nData(nData) {}
            bool operator () ( const std::pair<int,int>& pr )
            {
                return (m_nData == pr.first + pr.second);
            }
        };

        int nCount = std::count_if( m_vecBingoData.begin(), m_vecBingoData.end(), KSumComp(SiKQuizBingo()->GetBoardSize() -1) );
        if ( nCount == SiKQuizBingo()->GetBoardSize() ) {
            ++nRet;
        }
    }
    return nRet;
}

void KBingoUserData::AddPos( IN const std::pair<int,int> prPos_ )
{
    m_vecBingoData.push_back( prPos_ );
}

void KBingoUserData::AddBingoLineCount( IN const int nCount_ )
{
    m_nCurrentBingoLineCount += nCount_;
}

void KBingoUserData::AddCorrectQuestion( IN const int nQuestionID_ )
{
    m_vecCorrectQuestions.push_back( nQuestionID_ );
}

void KBingoUserData::ResetLastFail()
{
    m_nLastFailQuistionID = -1;
    m_prLastFailPos = std::pair<int,int>(-1,-1);
}

void KBingoUserData::SetLastFail( IN const int nQuestionID_, IN const std::pair<int,int>& prLastFailPos_ )
{
    m_nLastFailQuistionID = nQuestionID_;
    m_prLastFailPos = prLastFailPos_;
}

bool KBingoUserData::GetLastFail( OUT int& nQuestionID_, OUT std::pair<int,int>& prLastFailPos_ )
{
    if ( m_nLastFailQuistionID < 0 || m_prLastFailPos.first < 0 || m_prLastFailPos.second < 0 ) return false;
    nQuestionID_ = m_nLastFailQuistionID;
    prLastFailPos_ = m_prLastFailPos;
    return true;
}

void KBingoUserData::IncreaseAccTime()
{
    if ( IsFinishedToday() ) return;
    ++m_nAccPlayTime;
}

bool KBingoUserData::CheckRecvBingoCoin()
{
    if ( IsFinishedToday() ) return false;

    if ( GetAccTime() > 0 && GetAccTime() % SiKQuizBingo()->GetCoinSupplyInterval() == 0 ) {
        return true;
    }
    return false;
}

void KBingoUserData::ResetAccTime()
{
    m_nAccPlayTime = 0;
    m_bFinishToday = false;
}

void KBingoUserData::UpdateFinishToday()
{
    m_bFinishToday = !SiKQuizBingo()->CheckCoinRecvable( GetAccTime() );
}

bool KBingoUserData::UpdateAccTime( IN const DWORD dwUID_, IN const std::wstring& strLogin_, IN const char cCharType_, IN const bool bChangeDate_, IN const KSimpleDate& kToday_, OUT KEVENT_BINGO_COIN_CHARGE_NOT& kUserPacket_ )
{
    KEVENT_DB_BINGO_ACC_TIME_UPDATE kDBPacket;
    kDBPacket.m_bIncreaseCoinCount  = false;
    kDBPacket.m_bChangeDate         = false;

    IncreaseAccTime();

    kDBPacket.m_nAccTime = GetAccTime(); // 위치 중요.
    // 코인을 받을수 있나?
    if ( CheckRecvBingoCoin() ) {
        m_nTotalCoinCount += SiKQuizBingo()->GetCoinSupplyCount();
        kDBPacket.m_bIncreaseCoinCount = true;
    }

    if ( bChangeDate_ ) {
        kDBPacket.m_bChangeDate = true;
        ResetAccTime();
        CTime tmYesterDay = CTime( kToday_.m_sYear, kToday_.m_cMonth, kToday_.m_cDay, 0, 0, 0 ) - CTimeSpan( 1, 0, 0, 0 );
        kDBPacket.m_dateYesterDay = KSimpleDate( tmYesterDay.GetYear(), tmYesterDay.GetMonth(), tmYesterDay.GetDay(), 0 );
    }

    UpdateFinishToday();

    if ( kDBPacket.m_bIncreaseCoinCount || kDBPacket.m_bChangeDate ) {
        kUserPacket_.m_bFinishToday = IsFinishedToday();
        kUserPacket_.m_nAccTime = GetCalcedAccTime();
        kUserPacket_.m_nCoinCount = GetCurrentBingoCoinCount();

        SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_DB_BINGO_ACC_TIME_UPDATE, strLogin_.c_str(), dwUID_, cCharType_, kDBPacket );
        return true;
    }

    return false;
}

int KBingoUserData::GetCalcedAccTime()
{
    int nAccTime = GetAccTime();
    return SiKQuizBingo()->GetCoinSupplyInterval() - (nAccTime % SiKQuizBingo()->GetCoinSupplyInterval() );
}