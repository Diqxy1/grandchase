#include "Sphinx.h"
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

ImplementSingleton( KSphinx );
ImplOstmOperatorW2A( KSphinx );
NiImplementRootRTTI( KSphinx );

KSphinx::KSphinx(void)
:m_tmBegin(1)
,m_tmEnd(1)
,m_dwSphinxCoinID(0)
,m_nMaxRigthAnswerCnt(0)
,m_nLoopRightAnswerCnt(1)
,m_nVersion(0)
{
    m_mapSphinxQuestionInfo.clear();
    m_mapCompressedSphinxQuestionInfo.clear();
    m_mapAccumulateReward.clear();
    m_mapKairoLevelReward.clear();
    m_vecQuestionIDs.clear();
    m_vecDefaultAccumulateReward.clear();
    m_mapSphinxTimeValue.clear();
    m_mapRewardList.clear();
    m_mapQuestionLevel.clear();
}

KSphinx::~KSphinx(void)
{
}

ImplToStringW( KSphinx )
{
    KLocker lock(m_csSphinx);
    return START_TOSTRINGW
        << TOSTRINGWt( CTime(m_tmBegin) )
        << TOSTRINGWt( CTime(m_tmEnd) )
        << TOSTRINGW( m_dwSphinxCoinID )
        << TOSTRINGW( m_mapSphinxQuestionInfo.size() )
        << TOSTRINGW( m_mapCompressedSphinxQuestionInfo.size() )
        << TOSTRINGW( m_mapAccumulateReward.size() )
        << TOSTRINGW( m_mapKairoLevelReward.size() )
        << TOSTRINGW( m_vecQuestionIDs.size() )
        << TOSTRINGW( m_vecDefaultAccumulateReward.size() )
        << TOSTRINGW( m_mapSphinxTimeValue.size() )
        << TOSTRINGW( m_mapRewardList.size() )
        << TOSTRINGW( m_mapQuestionLevel.size() );
}

bool KSphinx::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSphinx::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitSphinx.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    GCITEMID dwSphinxCoinID;
    int nMaxRigthAnswerCnt;
    int nLoopRightAnswerCnt;
    std::map<int, VEC_ITEMS > mapAccumulateReward;
    std::map< int, Kairo >    mapKairoLevelReward;
    VEC_ITEMS                 vecDefaultAccumulateReward;
    std::map<int,int>         mapSphinxTimeValue;
    std::map<int, std::set<int>> mapRewardList;

    _JIF( kLuaMng.GetValue( "SphinxCoinID", dwSphinxCoinID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxRigthAnswerCnt", nMaxRigthAnswerCnt ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "LoopRightAnswerCnt", nLoopRightAnswerCnt ) == S_OK, return false );

    _JIF( LoadItems( kLuaMng, std::string("DefaultAccumulateReward"), vecDefaultAccumulateReward ), return false );
    _JIF( LoadTimeInfo( kLuaMng, std::string("SphinxTimeValue"), mapSphinxTimeValue ), return false );
    _JIF( LoadAccumulateReward( kLuaMng, std::string("SphinxAccumulateReward"), mapAccumulateReward ), return false );
    _JIF( LoadLevelReward( kLuaMng, std::string("SphinxLevelReward"), mapKairoLevelReward ), return false );

    START_LOG( cerr, L"스핑크스 OX 게임 스크립트 로드 완료. " ) << END_LOG;

    {
        KLocker lock( m_csSphinx );
        m_dwSphinxCoinID = dwSphinxCoinID;
        m_nMaxRigthAnswerCnt = nMaxRigthAnswerCnt;
        m_nLoopRightAnswerCnt = nLoopRightAnswerCnt;

        m_mapAccumulateReward.swap( mapAccumulateReward );
        m_mapKairoLevelReward.swap( mapKairoLevelReward );
        m_vecDefaultAccumulateReward.swap( vecDefaultAccumulateReward );
        m_mapSphinxTimeValue.swap( mapSphinxTimeValue );
    }

    CreateRewardList( m_mapAccumulateReward, mapRewardList );
    m_mapRewardList.swap( mapRewardList );

    return true;
}

bool KSphinx::LoadTimeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, int>& mapLevelInfo_ )
{
    mapLevelInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        int nType = 0;
        int nSecond = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( 1, nType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nSecond ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapLevelInfo_[nType] = nSecond;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KSphinx::LoadAccumulateReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, VEC_ITEMS >& mapAccumulateReward_ )
{
    mapAccumulateReward_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        int nRightAnswerCnt = 0;
        VEC_ITEMS vecReward;

        _JIF( kLuaMng_.GetValue( "rightAnswerCount", nRightAnswerCnt ) == S_OK, return false );
        _JIF( LoadItems( kLuaMng_, std::string("Reward"), vecReward ), return false );

        // 서버 아이템 타입 리스트 Load.
        if( !mapAccumulateReward_.insert( std::make_pair( nRightAnswerCnt, vecReward ) ).second ) {
            START_LOG( cerr, L" 중복된 누적 Count정보가 있음 : RightAnswerCnt : " << nRightAnswerCnt ) << END_LOG;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSphinx::LoadItems( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT VEC_ITEMS& vecItems_ )
{
    vecItems_.clear();
    __JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while( true )
    {
        KDropItemInfo kData;
        if( kLuaMng_.GetValue( i++, kData.m_ItemID ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( i++, kData.m_nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( i++, kData.m_nPeriod ) == S_OK, return false );
        vecItems_.push_back( kData );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSphinx::LoadLevelReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, Kairo >& mapKairoLevelReward_ )
{
    mapKairoLevelReward_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1; ; ++i ) {
        int nLevelType;
        Kairo kKairo;

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        _JIF( kLuaMng_.GetValue( "LevelType", nLevelType ) == S_OK, return false );
        _JIF( LoadLevelRatio( kLuaMng_, std::string("RewardItemList"), kKairo ), return false );

        // 난위도별 보상 아이템 정보 Load.
        if( !mapKairoLevelReward_.insert( std::make_pair( nLevelType, kKairo ) ).second ) {
            START_LOG( cerr, L" 중복된 난위도정보가 있음. LevelType : " << nLevelType ) << END_LOG;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    return true;
}

bool KSphinx::LoadLevelRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        GCITEMID itemID = 0;
        int nDuration = 0;
        int nPeriod = 0;
        float fProb = 0.f;
        bool bDisplay = false;

        _JIF( kLuaMng_.GetValue( 1, itemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, fProb ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( kairo_.SetPostItem( itemID, nDuration, nPeriod, fProb ), return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KSphinx::UpdateSphinxData( KECN_SPHINX_DATA_NOT& kCenterPacket_ )
{
    {
        KLocker lock( m_csSphinx );
        m_nVersion = kCenterPacket_.m_nVersion;
        m_tmBegin = kCenterPacket_.m_tmBegin;
        m_tmEnd = kCenterPacket_.m_tmEnd;
        m_mapCompressedSphinxQuestionInfo.swap( kCenterPacket_.m_mapCompressedSphinxQuestionInfo );
        UnCompressSphinxInfo( m_mapCompressedSphinxQuestionInfo, m_mapSphinxQuestionInfo );

        m_vecQuestionIDs.clear();
        m_mapQuestionLevel.clear();
        std::map< int,KSphinxQuestionInfo >::iterator mit;
        std::map< int, std::vector<int> >::iterator mitID;
        for ( mit = m_mapSphinxQuestionInfo.begin() ; mit != m_mapSphinxQuestionInfo.end() ; ++mit ) {
            m_vecQuestionIDs.push_back( mit->first );
            // 아이템별 레벨 정보 Load.
            if( !m_mapQuestionLevel.insert( std::make_pair( mit->first, mit->second.m_nLevel ) ).second ) {
                START_LOG( cerr, L" 중복된 문제정보가. nUID : " << mit->first ) << END_LOG;
            }
        }
    }

    START_LOG( cerr, L"Sphinx Data Update..." ) << END_LOG;
}

void KSphinx::UnCompressSphinxInfo( IN std::map<int, KSerBuffer>& mapCompressedSphinxQuestionInfo_, OUT std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo_ )
{
    mapSphinxQuestionInfo_.clear();
    std::map<int, KSerBuffer>::iterator mit;
    for( mit = mapCompressedSphinxQuestionInfo_.begin() ; mit != mapCompressedSphinxQuestionInfo_.end() ; ++mit )
    {
        KSphinxQuestionInfo kQuestion;
        KSerBuffer kBuff = mit->second;
        kBuff.UnCompress();
        KSerializer ks;
        ks.BeginReading( &kBuff );
        if ( !ks.Get( kQuestion ) ) {
            continue;
        }
        ks.EndReading();
        mapSphinxQuestionInfo_.insert( std::make_pair( kQuestion.m_nID, kQuestion ) );
    }
}

void KSphinx::GetUnSolvedQuestion( IN const std::vector<int>& vecSolved_, OUT std::vector<int>& vecUnSolved_ )
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
        KLocker lock( m_csSphinx );
        if ( m_vecQuestionIDs.empty() )
            return;

        std::remove_copy_if( m_vecQuestionIDs.begin(), m_vecQuestionIDs.end(),
            std::back_inserter( vecUnSolved_ ), KRemover(vecSolved_) );
    }
}

bool KSphinx::GetRandomQuestionID( IN std::vector<int>& vecCorrectList_, OUT int& nQuestionID_ )
{
    std::vector<int> vecUnSolved;
    CheckMaxCorrectQuestion( vecCorrectList_ );
    GetUnSolvedQuestion( vecCorrectList_, vecUnSolved );
    if ( vecUnSolved.empty() ) {
        return false;
    }

    std::random_shuffle( vecUnSolved.begin(), vecUnSolved.end() );

    nQuestionID_ = vecUnSolved.front();
    return true;
}

bool KSphinx::GetRandomQuestion( IN std::vector<int>& vecCorrectList_, OUT KSerBuffer& kQuestion_, OUT int& nQuestionID_ )
{
    std::vector<int> vecUnSolved;
    CheckMaxCorrectQuestion( vecCorrectList_ );
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

bool KSphinx::GetCompressedQuestion( IN const int nQuestionID_, OUT KSerBuffer& kQuestionBuff_ )
{
    std::map<int,KSerBuffer>::const_iterator mit;

    KLocker lock( m_csSphinx );
    mit = m_mapCompressedSphinxQuestionInfo.find( nQuestionID_ );
    if ( mit == m_mapCompressedSphinxQuestionInfo.end() ) return false;

    kQuestionBuff_ = mit->second;
    return true;
}

bool KSphinx::GetAccumulateReward( IN const int nRightAnswerCount_, OUT VEC_ITEMS& vecReward_ )
{
    std::map<int, VEC_ITEMS >::iterator mit;

    KLocker lock( m_csSphinx );
    mit = m_mapAccumulateReward.find( nRightAnswerCount_ );
    if( mit == m_mapAccumulateReward.end() ) {
        return false;
    }
    vecReward_ = mit->second;
    return true;
}

bool KSphinx::GetKairoLevelReward( IN const int nLevelType_, OUT KDropItemInfo& kItem_ )
{
    std::map< int, Kairo >::iterator mit;
    mit = m_mapKairoLevelReward.find( nLevelType_ );
    if( mit == m_mapKairoLevelReward.end() ) {
        return false;
    }

    bool bSuccess;
    {
        KLocker lock( m_csSphinx );
        bSuccess = mit->second.Do( kItem_ );
    }
    if( bSuccess ) return true;

    if( kItem_.m_ItemID == KLottery::CASE_BLANK )
    {
        return false;
    }

    return false;
}

void KSphinx::GetDefaultAccumulateReward( IN const int nRigthAnswerCnt_, IN int nLoopRightAnswerCnt_, OUT VEC_ITEMS& vecReward_ )
{
    KLocker lock( m_csSphinx );
    nLoopRightAnswerCnt_ = std::max<int>( 10, nLoopRightAnswerCnt_ );
    int nCount = nRigthAnswerCnt_ % nLoopRightAnswerCnt_;
    if( nCount == 0 ) { // 반복횟수 달성시 아이템 지급.
        vecReward_ = m_vecDefaultAccumulateReward;
    }
}

bool KSphinx::GetSphinxTimeValue( IN const int nUID_, OUT int& nSecond_ )
{
    nSecond_ = 0;
    std::map<int,int>::iterator mit;
    std::map<int,int>::iterator mitValue;

    KLocker lock( m_csSphinx );
    mit = m_mapQuestionLevel.find( nUID_ );
    if( mit == m_mapQuestionLevel.end() ) {
        START_LOG( cerr, L" 문제의 레벨정보가 없음 UID : " << nUID_ ) << END_LOG;
        return false;
    }

    mitValue = m_mapSphinxTimeValue.find( mit->second );
    if( mitValue == m_mapSphinxTimeValue.end() ) {
        START_LOG( cerr, L" 레벨의 시간정보가 없음 TimeValue : " << mit->second ) << END_LOG;
        return false;
    }

    nSecond_ = mitValue->second;
    return true;
}

bool KSphinx::IsRun()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    KLocker lock( m_csSphinx );
    return (m_tmBegin <= tmCurrent && m_tmEnd >= tmCurrent);
}

void KSphinx::CreateRewardList( IN std::map<int, VEC_ITEMS >& mapAccumulateReward_, OUT std::map<int, std::set<int>>& mapRewardList_ )
{
    mapRewardList_.clear();
    std::map<int, VEC_ITEMS >::iterator mit;
    VEC_ITEMS::iterator vit;
    std::set<int> setItemList;

    KLocker lock( m_csSphinx );
    for( mit = mapAccumulateReward_.begin(); mit != mapAccumulateReward_.end(); ++mit ) {
        setItemList.clear();
        for( vit = mit->second.begin(); vit != mit->second.end(); ++vit ) {
            setItemList.insert( vit->m_ItemID );
        }
        if( mapRewardList_.insert( std::make_pair( mit->first, setItemList ) ).second == false ) {
            START_LOG( cerr, L"중복된 rightAnswerCount :" << mit->first ) << END_LOG;
        }
    }
}

void KSphinx::GetRewardList( OUT std::map<int, std::set<int>>& mapRewardList_ )
{
    KLocker lock( m_csSphinx );
    mapRewardList_ = m_mapRewardList;
}

bool KSphinx::GetQuestion( IN const int& nQuestionID_, OUT KSphinxQuestionInfo& kQuestion_ )
{
    std::map<int,KSphinxQuestionInfo>::iterator mit;
    mit = m_mapSphinxQuestionInfo.find( nQuestionID_ );
    if( mit == m_mapSphinxQuestionInfo.end() ) {
        return false;
    }

    kQuestion_ = mit->second;
    return true;
}

void KSphinx::CheckAccumulateReward( IN const int& nRightAnswerCnt_, OUT VEC_ITEMS& vecReward_ )
{
    int nTotalQuestionNum = GetTotalQuestionNum();
    int nLoopRightAnswerCnt = GetLoopRightAnswerCnt();
    if( nRightAnswerCnt_ <= nTotalQuestionNum ) { // 세팅된 누적확률 보상.
        GetAccumulateReward( nRightAnswerCnt_, vecReward_ );
    } else {
        GetDefaultAccumulateReward( nRightAnswerCnt_, nLoopRightAnswerCnt, vecReward_ );
    }
}

void KSphinx::CheckMaxCorrectQuestion( IN OUT std::vector<int>& vecCorrectList_ )
{
    KLocker lock( m_csSphinx );

    int nListSize = vecCorrectList_.size();
    if( nListSize >= m_nMaxRigthAnswerCnt ) {
        vecCorrectList_.clear();
    }
}

//=========================================================================================//
KSphinxUserData::KSphinxUserData()
:m_bLoaded(false)
,m_bComplete(false)
,m_nRigthAnswerCnt(0)
{
    m_vecCorrectQuestions.clear();
}

KSphinxUserData::~KSphinxUserData()
{

}

void KSphinxUserData::SetUserSphinxData( IN const int& nRigthAnswerCnt_, IN const std::vector<int>& vecCorrectQuestions_, IN const int nTotalQuestionNum_ )
{
    m_bLoaded = true;
    m_nRigthAnswerCnt = nRigthAnswerCnt_;
    m_vecCorrectQuestions = vecCorrectQuestions_;
    if( m_nRigthAnswerCnt >= nTotalQuestionNum_ ) {
        m_bComplete = true;
    }
}

void KSphinxUserData::AddCorrectQuestion( IN const int nQuestionID_, IN const int nTotalQuestionNum_ )
{
    if( !IsCompleted() ) {
        if( m_nRigthAnswerCnt >= nTotalQuestionNum_ ) {
            m_nRigthAnswerCnt += 1;
            m_bComplete = true;
            return;
        } else {
            m_vecCorrectQuestions.push_back( nQuestionID_ );
            m_nRigthAnswerCnt += 1;
        }
    } else {
        m_nRigthAnswerCnt += 1;
    }
}
