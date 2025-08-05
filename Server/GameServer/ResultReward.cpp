#include "ResultReward.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KResultReward );
ImplOstmOperatorW2A( KResultReward );
NiImplementRootRTTI( KResultReward );


KResultReward::KResultReward(void)
{
    m_mapRewardInfo.clear();
    m_mapLevelRange.clear();
}

KResultReward::~KResultReward(void)
{
}

ImplToStringW( KResultReward )
{
    KLocker lock( m_csResultReward );
    return START_TOSTRINGW
        << TOSTRINGW( m_mapRewardInfo.size() )
        << TOSTRINGW( m_mapLevelRange.size() );
}

bool KResultReward::LoadScript()
{
    return _LoadScript(std::string());
}

bool KResultReward::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    KAddRewardInfo kAddRewardInfo;
    std::map<PAIR_INT, KResultRewardInfo> mapRewardInfo;
    std::map<int, std::vector<KLevelRangeInfo>> mapLevelRange;
    const std::string strScriptName = "InitResultReward.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( LoadAddRewardInfo( kLuaMng, std::string("AddRewardCondition"), kAddRewardInfo ), return false );
    _JIF( LoadLevelRangeInfo( kLuaMng, std::string("LevelRange"), mapLevelRange ), return false );
    // 던전별 캐릭터 보상정보 Load
    _JIF( LoadResultRewardInfo( kLuaMng, std::string("ResultReward"), mapRewardInfo ), return false );

    START_LOG( cerr, L"최종 보상 스크립트 로드 완료. " )
        << BUILD_LOG( mapRewardInfo.size() )
        << BUILD_LOG( mapLevelRange.size() )
        << END_LOG;

    {
        KLocker lock( m_csResultReward );
        m_kAddRewardInfo = kAddRewardInfo;
        m_mapRewardInfo.swap( mapRewardInfo );
        m_mapLevelRange.swap( mapLevelRange );

        return true;
    }
}

bool KResultReward::LoadAddRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KAddRewardInfo& kAddRewardInfo_ )
{
    KAddRewardInfo kAddRewardInfo;
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    _JIF( kLuaMng_.GetValue( "IPAdavntageCheck", kAddRewardInfo.m_bPCBangUser ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "LimitClearTime", kAddRewardInfo.m_dwLimitClearTime ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "MinPartyNum", kAddRewardInfo.m_nMinPartyNum ) == S_OK, return false );

    kAddRewardInfo_ = kAddRewardInfo;

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KResultReward::LoadLevelRangeInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int, std::vector<KLevelRangeInfo>>& mapLevelRange_ )
{
    mapLevelRange_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::vector<KLevelRangeInfo> vecRangeInfo;
        int nDungeonID;
        vecRangeInfo.clear();

        _JIF( kLuaMng_.GetValue( "DungeonID", nDungeonID ) == S_OK, return false );
        _JIF( LoadDungeonLevelList( kLuaMng_, std::string("DungeonLevel"), vecRangeInfo ), return false );
        
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if ( !mapLevelRange_.insert( std::make_pair( nDungeonID, vecRangeInfo ) ).second ) {
            START_LOG( cerr, L"해당 모드에 중복된 레벨 범위가 있음 : " << nDungeonID ) << END_LOG;
        }	
        }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KResultReward::LoadDungeonLevelList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<KLevelRangeInfo>& vecLevelRangeList_ )
{
    KLevelRangeInfo kRangeInfo;

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for ( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "Boundary", kRangeInfo.m_nBoundary ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MinLevel", kRangeInfo.m_nMinLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "MaxLevel", kRangeInfo.m_nMaxLevel ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecLevelRangeList_.push_back( kRangeInfo );
    }
    START_LOG( cerr, L"난이도 하나의 레벨 범위 리스트 로드 완료. " )
        << BUILD_LOG( vecLevelRangeList_.size() )
        << END_LOG;

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KResultReward::LoadResultRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<PAIR_INT, KResultRewardInfo>& mapRewardInfo_ )
{
    mapRewardInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KResultRewardInfo kRewardInfo;
        _JIF( kLuaMng_.GetValue( "DungeonID", kRewardInfo.m_nModeID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Difficulty", kRewardInfo.m_nDifficulty ) == S_OK, return false );
        _JIF( LoadRewardInfo( kLuaMng_, std::string("RewardInfo"), kRewardInfo ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        PAIR_INT prKey = std::make_pair( kRewardInfo.m_nModeID, kRewardInfo.m_nDifficulty );
        if ( !mapRewardInfo_.insert( std::make_pair( prKey, kRewardInfo ) ).second ) {
            START_LOG( cerr, L"중복된 키 값이 있음 : " ) 
                << BUILD_LOG( kRewardInfo.m_nModeID )
                << BUILD_LOG( kRewardInfo.m_nDifficulty )
                << END_LOG;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KResultReward::LoadRewardInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KResultRewardInfo& kResultRewardInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    std::map<PAIR_CHAR_INT, Kairo> mapDefaultRewardInfo; // 기본보상
    std::map<PAIR_CHAR_INT, Kairo> mapAddRewardInfo; // 추가보상
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nCharType;
        int nLevel;
        Kairo kDefaultKairo;
        Kairo kAddKairo;
        _JIF( kLuaMng_.GetValue( "CharType", nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Level", nLevel ) == S_OK, return false );
        _JIF( LoadRewardRatio( kLuaMng_, std::string("DefaultReward"), kDefaultKairo ), return false );
        _JIF( LoadRewardRatio( kLuaMng_, std::string("AddReward"), kAddKairo ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        PAIR_CHAR_INT prKey = std::make_pair( static_cast<char>( nCharType ), nLevel );
        if ( !mapDefaultRewardInfo.insert( std::make_pair( prKey, kDefaultKairo ) ).second ) {
            START_LOG( cerr, L"기본보상에 중복된 키 값이 있음 : " )
                << BUILD_LOG( nCharType )
                << BUILD_LOG( nLevel )
                << END_LOG;
        }

        if ( !mapAddRewardInfo.insert( std::make_pair( prKey, kAddKairo ) ).second ) {
            START_LOG( cerr, L"추가보상에 중복된 키 값이 있음 : " )
                << BUILD_LOG( nCharType )
                << BUILD_LOG( nLevel )
                << END_LOG;
        }
    }
    kResultRewardInfo_.m_mapDefaultRewardInfo.swap( mapDefaultRewardInfo );
    kResultRewardInfo_.m_mapAddRewardInfo.swap( mapAddRewardInfo );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KResultReward::LoadRewardRatio( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT Kairo& kKairo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        GCITEMID itemID = 0;
        int nDuration = -1;
        int nPeriod = -1;
        int nGrade = -1;
        int nLevel = 0;
        float fProb = 0.f;

        _JIF( kLuaMng_.GetValue( 1, itemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nDuration ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 5, nLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 6, fProb ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        _JIF( kKairo_.SetEquipLevelDropItem( itemID, nDuration, nPeriod, (char)nGrade, (char)nLevel, fProb ), return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KResultReward::GetDefaultRewardInfo( IN const int& nGameMode_, IN const int& nDifficulty_, IN const char& cCharType_, IN const char& cLevel_, OUT KDropItemInfo& kItem_ )
{
    std::map<PAIR_INT,KResultRewardInfo>::iterator mitResultReward;
    std::map<PAIR_CHAR_INT, Kairo>::iterator mitDefaultReward;

    PAIR_INT prKeyResultReward = std::make_pair( nGameMode_, nDifficulty_ );

    KLocker lock( m_csResultReward );
    mitResultReward = m_mapRewardInfo.find( prKeyResultReward );
    if( mitResultReward == m_mapRewardInfo.end() ) {
        START_LOG( cwarn, L"키 값에 해당 하는 RewardInfo 데이터 없음" )
            << BUILD_LOG( nGameMode_ )
            << BUILD_LOG( nDifficulty_ )
            << END_LOG;
        return false;
    }

    int nLevelBoundary;
    if ( false == GetLevelBoundaryInfoFromLevel( nGameMode_, static_cast<int>( cLevel_ ), nLevelBoundary ) ) {
        return false;
    }

    PAIR_CHAR_INT prKeyDefaultReward = std::make_pair( cCharType_, nLevelBoundary );
    mitDefaultReward = mitResultReward->second.m_mapDefaultRewardInfo.find( prKeyDefaultReward );
    if( mitDefaultReward == mitResultReward->second.m_mapDefaultRewardInfo.end() ) {
        START_LOG( cwarn, L"키 값에 해당 하는 DefaultReward 데이터 없음" )
            << BUILD_LOGc( cCharType_ )
            << BUILD_LOGc( cLevel_ )
            << BUILD_LOG( nLevelBoundary )
            << END_LOG;
        return false;
    }

    bool bSuccess = false;
    bSuccess = mitDefaultReward->second.Do( kItem_ );

    if( !bSuccess ) {
        return false;
    }

    if( kItem_.m_ItemID == 0 ) {
        return false;
    }

    return true;
}

bool KResultReward::GetAddRewardInfo( IN const int& nGameMode_, IN const int& nDifficulty_, IN const char& cCharType_, IN const char& cLevel_, IN const bool& bPCBang_, IN const DWORD& dwPlayTime_, IN const int& nPartyNum_, OUT KDropItemInfo& kItem_ )
{
    std::map<PAIR_INT,KResultRewardInfo>::iterator mitResultReward;
    std::map<PAIR_CHAR_INT, Kairo>::iterator mitAddReward;

    PAIR_INT prKeyResultReward = std::make_pair( nGameMode_, nDifficulty_ );

    KLocker lock( m_csResultReward );
    mitResultReward = m_mapRewardInfo.find( prKeyResultReward );
    if( mitResultReward == m_mapRewardInfo.end() ) {
        return false;
    }

    int nLevelBoundary;
    if ( false == GetLevelBoundaryInfoFromLevel( nGameMode_, static_cast<int>( cLevel_ ), nLevelBoundary ) ) {
        return false;
    }

    PAIR_CHAR_INT prKeyAddReward = std::make_pair( cCharType_, nLevelBoundary );

    if( bPCBang_ != true ) { // 유저 Type이 PC방이면 무조건 받음.
        if( m_kAddRewardInfo.m_dwLimitClearTime < dwPlayTime_ && m_kAddRewardInfo.m_nMinPartyNum > nPartyNum_ ) {
            return false;
        }
    }

    mitAddReward = mitResultReward->second.m_mapAddRewardInfo.find( prKeyAddReward );
    if( mitAddReward == mitResultReward->second.m_mapAddRewardInfo.end() ) {
        return false;
    }

    bool bSuccess = false;
    bSuccess = mitAddReward->second.Do( kItem_ );

    if( !bSuccess ) {
        return false;
    }

    if( kItem_.m_ItemID == 0 ) {
        return false;
    }

    return true;
}

bool KResultReward::GetLevelBoundaryInfoFromLevel( IN const int& nDungeonID_, IN const int& nLevel_, OUT int& nBoundaryInfo_ )
{
    std::map<int, std::vector<KLevelRangeInfo>>::iterator mit;
    std::vector<KLevelRangeInfo>::iterator vit;
    int nBoundaryInfo = -1;

    mit = m_mapLevelRange.find( nDungeonID_ );
    if ( m_mapLevelRange.end() == mit ) {
        START_LOG( cwarn, L"해당 던전이 ResultReward 레벨 범위에 설정 되어 있지 않음" )
            << BUILD_LOG( nDungeonID_ )
            << END_LOG;

        return false;
    }

    for( vit = mit->second.begin(); vit != mit->second.end() ;vit++ ) {
        // 매개변수로 받은 레벨 값이 MinLev~MaxLev 에 포함 된다면 해당 Boundary 값 전달
        if ( nLevel_ >= vit->m_nMinLevel && nLevel_ <= vit->m_nMaxLevel ) {
            nBoundaryInfo_ = vit->m_nBoundary;
            START_LOG( clog, L"보상 레벨 범위 얻음" )
                << BUILD_LOG( nLevel_ )
                << BUILD_LOG( vit->m_nMinLevel )
                << BUILD_LOG( vit->m_nMaxLevel )
                << BUILD_LOG( vit->m_nBoundary )
                << END_LOG;

            return true;
        }
    }

    START_LOG( cwarn, L"해당 던전의 보상 범위에 해당 하는 레벨이 아님" )
        << BUILD_LOG( nLevel_ )
        << BUILD_LOG( nBoundaryInfo_ )
        << END_LOG;

    return false;
}