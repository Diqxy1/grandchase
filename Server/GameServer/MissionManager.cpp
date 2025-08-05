#include "User.h"
#include "Room.h"
#include "MissionManager.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include <KncUtil.h>
#include "GSSimLayer.h"
#include <algorithm> // random_shuffle
#include "GameDrop.h" // sTriggerInfo
#include <boost/bind.hpp>
#include "ItemContainer.h"
#include "GameDrop.h"
#include "ServerBenefit.h"
#include "Log4.h"

namespace {
    struct KComp
    {
        bool operator() ( const KMissionPenaltyInfo& info1, const KMissionPenaltyInfo& info2 )
        {
            if( info1.m_prLvDiffRange.first < info2.m_prLvDiffRange.second ) return true;
            return false;
        }

        bool operator() ( const int nVal, const KMissionPenaltyInfo& info )
        {
            if( nVal < info.m_prLvDiffRange.first && nVal < info.m_prLvDiffRange.second )
                return true;
            return false;
        }

        bool operator() ( const KMissionPenaltyInfo& info, const int nVal )
        {
            if( nVal > info.m_prLvDiffRange.first && nVal > info.m_prLvDiffRange.second )
                return true;
            return false;
        }
    };
}

ImplementSingleton( KMissionManager );
NiImplementRootRTTI( KMissionManager );

KMissionManager::KMissionManager(void) 
:
    uint32( 1, UINT_MAX ),
    die(rng, uint32),
    m_fMissionBonusExp(1),
    m_fMissionBonusGp(1)
{
    rng.seed( ::GetTickCount() ); // Seed 값 설정.
}

KMissionManager::~KMissionManager(void)
{
}

bool KMissionManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KMissionManager::_LoadScript( OUT std::string& strScript_ )
{
    _JIF( LoadMissionInfo(strScript_), return false );
    _JIF( LoadMissionPenalty(strScript_), return false );

    return true;
}

bool KMissionManager::LoadMissionInfo( OUT std::string& strScript_ )
{
    std::string strFileName( "InitMission.lua" );
    KLuaManager kLuaMng;
    strScript_ = strFileName;
    _JIF( kLuaMng.DoFile( strFileName.c_str() ) == S_OK, return false );

    std::map< int, sCondition > mapConditions;
    _JIF( kLuaMng.BeginTable( "Condition" ) == S_OK, return false );
    _JIF( LoadCondition( kLuaMng, mapConditions ), return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    std::map< int, KSubMission > mapSubmission;
    _JIF( kLuaMng.BeginTable( "Submission" ) == S_OK, return false );
    _JIF( LoadSubMission( kLuaMng, mapSubmission ), return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    std::map< DWORD, KMissionInfo > mapMissionInfo;
    _JIF( kLuaMng.BeginTable( "Mission" ) == S_OK, return false );
    _JIF( LoadMission( kLuaMng, mapMissionInfo ), return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    KLocker lock( m_csMissions );
    m_mapConditions.swap( mapConditions );
    m_mapSubmission.swap( mapSubmission );
    m_mapMissionInfo.swap( mapMissionInfo );

    return true;
}

bool KMissionManager::LoadCondition( IN KLuaManager& kLuaMng_, OUT std::map< int, sCondition >& mapConditions_ )
{
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) == S_OK )
        {
            sCondition condition;

            _JIF( 0 == kLuaMng_.GetValue( "ID", condition.m_nID ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "Win", condition.m_bWin ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "Time", condition.m_nElapsedTime ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "MonsterKill", condition.m_nMonsterKill ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "Mode", condition.m_nModeID ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "Stage", condition.m_nStageID ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "Solo", condition.m_nSolo ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "MapID", condition.m_nMapID ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "Kill", condition.m_nKill ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "Die", condition.m_nDie ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "ExLife", condition.m_nExLife ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "ConditionActiveRatio", condition.m_fRatio ) , return false );

            if( kLuaMng_.BeginTable( "DifficultRange" ) == S_OK )
            {
                int nBegin  = -1;
                int nEnd    = -1;
                _JIF( 0 == kLuaMng_.GetValue( 1, nBegin ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, nEnd ) , return false );
                condition.m_prDifficultRage.first   = nBegin;
                condition.m_prDifficultRage.second  = nEnd;
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"DifficultRange를 읽지 못함." )
                    << BUILD_LOG( condition.m_nID ) << END_LOG;
            }

            if ( kLuaMng_.BeginTable( "MonLevelRange" ) == S_OK ) {
                _JIF( 0 == kLuaMng_.GetValue( 1, condition.m_prMonLevelRange.first ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, condition.m_prMonLevelRange.second ) , return false );

                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else {
                START_LOG( cerr, L"MonLevelRange를 읽지 못함." )
                    << BUILD_LOG( condition.m_nID ) << END_LOG;
            }

            _JIF( 0 == kLuaMng_.GetValue( "Type", condition.m_nType ) , return false );
            _JIF( 0 == kLuaMng_.GetValue( "TypeValue", condition.m_nValue ) , return false );
            if ( kLuaMng_.BeginTable( "TypeValueRange" ) == S_OK ) {
                _JIF( 0 == kLuaMng_.GetValue( 1, condition.m_prValue.first ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, condition.m_prValue.second ) , return false );

                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else {
                START_LOG( cerr, L"TypeValueRange를 읽지 못함." )
                    << BUILD_LOG( condition.m_nID ) << END_LOG;
            }

            _JIF( kLuaMng_.EndTable() == S_OK, return false );

            std::pair< std::map<int,sCondition>::iterator, bool > prRet;
            prRet = mapConditions_.insert( std::make_pair( condition.m_nID, condition) );

            if( !prRet.second )
            {
                START_LOG( cerr, L"중복된 컨디션 ID가 존재 합니다. ID : " << condition.m_nID ) << END_LOG;
                return false;
            }
        }
        else
            break;
    }

    START_LOG( cerr, L" 컨디션 로드 완료, 총 개수 : " << mapConditions_.size() ) << END_LOG;
    return true;
}

bool KMissionManager::LoadSubMission( IN KLuaManager& kLuaMng_, OUT std::map< int, KSubMission >& mapSubmission_ )
{
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) == S_OK )
        {
            KSubMission subMission;
            _JIF( 0 == kLuaMng_.GetValue( "SubmissionID", subMission.m_dwSubmissionID ) , return false );

            if( kLuaMng_.BeginTable( "Condition" ) == S_OK )
            {
                for( int j = 1 ; ; ++j )
                {
                    int nConditionID;
                    if( S_OK != kLuaMng_.GetValue( j, nConditionID ) ) break;
                    subMission.m_setConditions.insert( nConditionID );
                }
                _JIF( kLuaMng_.EndTable() == S_OK && L"Condition", return false );
            }
            else
            {
                START_LOG( cerr, L"Condition를 읽지 못함." )
                    << BUILD_LOG( subMission.m_dwSubmissionID ) << END_LOG;
                return false;
            }

            if( kLuaMng_.BeginTable( "Action" ) == S_OK )
            {
                _JIF( 0 == kLuaMng_.GetValue( "bMonDrop", subMission.m_sActionInfo.m_bMonDrop ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( "Item", subMission.m_sActionInfo.m_nItemID ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( "Count", subMission.m_sActionInfo.m_nCount ) , return false );

                _JIF( kLuaMng_.EndTable() == S_OK && L"Action", return false );
            }
            else
            {
                START_LOG( cerr, L"Action를 읽지 못함." )
                    << BUILD_LOG( subMission.m_dwSubmissionID ) << END_LOG;
                return false;
            }

            if( kLuaMng_.BeginTable( "Completion" ) == S_OK )
            {
                _JIF( 0 == kLuaMng_.GetValue( "Count", subMission.m_sCompletionInfo.m_nCount ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( "ItemID", subMission.m_sCompletionInfo.m_nItemID ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( "CharacterLv", subMission.m_sCompletionInfo.m_nCharLv ) , return false );

                _JIF( kLuaMng_.EndTable() == S_OK && L"Completion", return false );
            }
            else
            {
                START_LOG( cerr, L"Completion를 읽지 못함." )
                    << BUILD_LOG( subMission.m_dwSubmissionID ) << END_LOG;
                return false;
            }

            std::pair< std::map< int, KSubMission >::iterator, bool > prRet;
            prRet = mapSubmission_.insert( std::make_pair( (int)subMission.m_dwSubmissionID, subMission) );
            if( !prRet.second )
            {
                START_LOG( cerr, L"중복된 서브 미션이 있음. SubMissionID : " << subMission.m_dwSubmissionID ) << END_LOG;
                return false;
            }
            _JIF( kLuaMng_.EndTable() == S_OK, return false );
        }
        else
            break;

    }

    START_LOG( cerr, L" 서브 미션 로드 완료, 총 개수 : " << mapSubmission_.size() ) << END_LOG;
    return true;
}

bool KMissionManager::LoadMission( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, KMissionInfo >& mapMissionInfo_ )
{
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) == S_OK )
        {
            int nCharType;
            int nJobLv;
            int nNewChar;
            int nChangeJob;
            int nMissionTab;

            KMissionInfo missionInfo;
            _JIF( 0 == kLuaMng_.GetValue( "ID", missionInfo.m_dwMissionID ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "CharType", nCharType ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "Promotion", nJobLv ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "Unique", missionInfo.m_bUnique ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "Period", missionInfo.m_nPeriod ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "CorrectLevel", missionInfo.m_nCorrectLv ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "NewChar", nNewChar ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "ChangeJob", nChangeJob ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "GP", missionInfo.m_nGp), return false );
            _JIF( 0 == kLuaMng_.GetValue( "EXP", missionInfo.m_nExp ), return false );

            int nChainMissionID = -1;
            if( 0 != kLuaMng_.GetValue( "ChainMissionID", nChainMissionID ) ) // 칭호쪽 스크립트에는 없기 때문에.
                nChainMissionID = -1;

            missionInfo.m_dwChainMissionID = ( nChainMissionID > 0 ? (DWORD)(nChainMissionID) : 0 );
            
            _JIF( 0 == kLuaMng_.GetValue( "MissionTab", nMissionTab ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "EnableDelete", missionInfo.m_bEnableDelete), return false );
            _JIF( 0 == kLuaMng_.GetValue( "DungeonID", missionInfo.m_nDungeonID), return false );

            missionInfo.m_cMissionType  = (char)nMissionTab;
            missionInfo.m_cCharType     = (char)nCharType;
            missionInfo.m_cJobLv        = (char)nJobLv;
            missionInfo.m_cNewChar      = (char)nNewChar;
            missionInfo.m_cChangeJob    = (char)nChangeJob;

            //-------------
            if( kLuaMng_.BeginTable( "LevelRange" ) == S_OK )
            {
                _JIF( 0 == kLuaMng_.GetValue( 1, missionInfo.m_prLevelRage.first ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, missionInfo.m_prLevelRage.second ) , return false );
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"LevelRange를 읽지 못함." )
                    << BUILD_LOG( missionInfo.m_dwMissionID ) << END_LOG;
                return false;
            }

            if( kLuaMng_.BeginTable( "InvalidMission" ) == S_OK )
            {
                for( int j = 1 ; ; ++j )
                {
                    DWORD dwInvalid;
                    if( 0 != kLuaMng_.GetValue( j, dwInvalid ) ) break;

                    if( std::find( missionInfo.m_vecInvalidMission.begin(), missionInfo.m_vecInvalidMission.end(), dwInvalid ) !=
                        missionInfo.m_vecInvalidMission.end() )
                    {
                        START_LOG( cerr, L"중복된 InvalidMission이 존재 합니다. MissionID : " << missionInfo.m_dwMissionID )
                            << BUILD_LOG( dwInvalid ) << END_LOG;
                        return false;
                    }

                    missionInfo.m_vecInvalidMission.push_back( dwInvalid );
                }
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"InvalidMission를 읽지 못함." )
                    << BUILD_LOG( missionInfo.m_dwMissionID ) << END_LOG;
                return false;
            }

//---------------------------
            if( kLuaMng_.BeginTable( "InvalidCompleteMission" ) == S_OK )
            {
                for( int j = 1 ; ; ++j )
                {
                    DWORD dwInvalid;
                    if( 0 != kLuaMng_.GetValue( j, dwInvalid ) ) break;

                    if( std::find( missionInfo.m_vecInvalidComplete.begin(), missionInfo.m_vecInvalidComplete.end(), dwInvalid ) !=
                        missionInfo.m_vecInvalidComplete.end() )
                    {
                        START_LOG( cerr, L"중복된 InvalidCompleteMission이 존재 합니다. MissionID : " << missionInfo.m_dwMissionID )
                            << BUILD_LOG( dwInvalid ) << END_LOG;
                        return false;
                    }

                    missionInfo.m_vecInvalidComplete.push_back( dwInvalid );
                }
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"InvalidCompleteMission를 읽지 못함." )
                    << BUILD_LOG( missionInfo.m_dwMissionID ) << END_LOG;
                return false;
            }
            
//---------------------------
            if( kLuaMng_.BeginTable( "Submissions" ) == S_OK )
            {
                for( int j = 1 ; ; ++j )
                {
                    int nSubMission;
                    if( 0 != kLuaMng_.GetValue( j, nSubMission ) ) break;

                    if( nSubMission < 0 ) continue; // -1은 비우기 위한.. 

                    if( std::find( missionInfo.m_vecSubmission.begin(), missionInfo.m_vecSubmission.end(), nSubMission ) !=
                        missionInfo.m_vecSubmission.end() )
                    {
                        START_LOG( cerr, L"중복된 Submissions이 존재 합니다. MissionID : " << missionInfo.m_dwMissionID )
                            << BUILD_LOG( nSubMission ) << END_LOG;
                        return false;
                    }

                    missionInfo.m_vecSubmission.push_back( nSubMission );
                }
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"Submissions를 읽지 못함." )
                    << BUILD_LOG( missionInfo.m_dwMissionID ) << END_LOG;
                return false;
            }

//-----------------------------------------
            if( kLuaMng_.BeginTable( "RewardItem" ) == S_OK )
            {
                for( int j = 1 ; ; ++j )
                {
                    if( 0 != kLuaMng_.BeginTable( j ) == S_OK )
                    {
                        KMissionReward prReward;
                        prReward.m_nGradeID = -1;
                        prReward.m_bIsLook = false;
                        _JIF( 0 == kLuaMng_.GetValue( 1, prReward.m_ItemID )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 2, prReward.m_nCount )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 3, prReward.m_nPeriod ) , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 4, prReward.m_nGradeID ), return false );
                        _JIF( 0 == kLuaMng_.GetValue( 5, prReward.m_bIsLook ) , return false );

                        _JIF( kLuaMng_.EndTable() == S_OK, return false );
                        missionInfo.m_vecRewardItem.push_back( prReward );
                    }
                    else
                        break;

                }
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"RewardItem를 읽지 못함." )
                    << BUILD_LOG( missionInfo.m_dwMissionID ) << END_LOG;
                return false;
            }

//-----------------------------------------
            if( kLuaMng_.BeginTable( "SelectItem" ) == S_OK )
            {
                for( int j = 1 ; ; ++j )
                {
                    if( 0 != kLuaMng_.BeginTable( j ) == S_OK )
                    {
                        KMissionReward prReward;
                        prReward.m_nGradeID = -1;
                        prReward.m_bIsLook = false;
                        _JIF( 0 == kLuaMng_.GetValue( 1, prReward.m_ItemID )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 2, prReward.m_nCount )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 3, prReward.m_nPeriod ) , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 4, prReward.m_nGradeID ), return false );
                        _JIF( 0 == kLuaMng_.GetValue( 5, prReward.m_bIsLook ) , return false );
                        _JIF( kLuaMng_.EndTable() == S_OK, return false );

                        missionInfo.m_vecSelectItem.push_back( prReward );
                    }
                    else
                        break;
                }
                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"SelectItem를 읽지 못함." )
                    << BUILD_LOG( missionInfo.m_dwMissionID ) << END_LOG;
                return false;
            }

            std::pair< std::map< DWORD, KMissionInfo >::iterator, bool > prRet;
            prRet = mapMissionInfo_.insert( std::make_pair(missionInfo.m_dwMissionID, missionInfo) );
            if( !prRet.second )
            {
                START_LOG( cerr, L"중복된 미션이 존재함, Mission ID : " << missionInfo.m_dwMissionID ) << END_LOG;
                return false;
            }

            _JIF( kLuaMng_.EndTable() == S_OK, return false );
        }
        else
            break;
    }

    START_LOG( cerr, L"미션 로드 완료, 총 개수 : " << mapMissionInfo_.size() ) << END_LOG;
    return true;
}

template <class T>
std::wostream& operator<<( std::wostream& stm_, const std::pair<const DWORD,T>& data_ )
{
    return stm_ << data_.first;
}

template <class T>
std::wostream& operator<<( std::wostream& stm_, const std::pair<const int,T>& data_ )
{
    return stm_ << data_.first;
}

ImplOstmOperatorW2A( KMissionManager );
ImplToStringW( KMissionManager )
{
    DWORD dwMissionPenaltySize = 0;
    {
        KLocker lock1( m_csMissionPenalty );
        dwMissionPenaltySize = m_vecMissionPenalty.size();
    }

    KLocker lock( m_csMissions );
    START_TOSTRINGW
        << TOSTRINGW( m_mapConditions.size() )
        << TOSTRINGW( m_mapSubmission.size() )
        << TOSTRINGW( m_mapMissionInfo.size() )
        << TOSTRINGW( dwMissionPenaltySize );

    stm_ << std::endl << L" Mission : ";

    std::copy( m_mapMissionInfo.begin(), m_mapMissionInfo.end(), 
        std::ostream_iterator<std::map<DWORD, KMissionInfo>::value_type,wchar_t>( stm_, L", " ) );

    stm_ << std::endl << std::endl << L" SubMission : ";

    std::copy( m_mapSubmission.begin(), m_mapSubmission.end(), 
        std::ostream_iterator<std::map<int, KSubMission>::value_type,wchar_t>( stm_, L", " ) );

    stm_ << std::endl << std::endl << L" Condition : ";

    std::copy( m_mapConditions.begin(), m_mapConditions.end(), 
        std::ostream_iterator<std::map<int, sCondition>::value_type,wchar_t>( stm_, L", " ) );

    return stm_;
}

bool KMissionManager::GetMissionInfo( OUT KMissionInfo& kMissionInfo, IN const DWORD dwMissionID )
{
    KLocker lock( m_csMissions );

    std::map< DWORD, KMissionInfo >::iterator mit;
    mit = m_mapMissionInfo.find( dwMissionID );
    if( mit == m_mapMissionInfo.end() )
    {
        START_LOG( cerr, L"등록되지 않은 미션 목록 요청!!" )
            << BUILD_LOG( dwMissionID ) << END_LOG;
        return false;
    }
    kMissionInfo = mit->second;
    return true;
}

bool KMissionManager::GetSubMission( OUT KSubMission& kSubMission_, IN const int nSubMissionID_ )
{
    KLocker lock( m_csMissions );
    std::map< int, KSubMission >::iterator mit;
    mit = m_mapSubmission.find( nSubMissionID_ );
    if( mit == m_mapSubmission.end() )
    {
        START_LOG( cerr, L"등록되지 않은 서브미션 목록 요청!!" )
            << BUILD_LOG( nSubMissionID_ ) << END_LOG;
        return false;
    }
    kSubMission_ = mit->second;
    return true;
}

bool KMissionManager::GetMissionData( OUT KMissionData& kMissionData_, IN const DWORD dwMissionID_, IN bool bFillCount_/* = true*/ )
{
    KMissionInfo kMissionInfo;
    if( !GetMissionInfo( kMissionInfo, dwMissionID_ ) )
    {
        START_LOG( cerr, L"Mission Data 정보 생성중 해당 미션을 가져 오지 못함, ID : " << dwMissionID_ ) << END_LOG;
        return false;
    }

    kMissionData_.m_dwMissionID = kMissionInfo.m_dwMissionID;
    std::vector<int>::iterator vit; 
    for( vit = kMissionInfo.m_vecSubmission.begin() ; vit != kMissionInfo.m_vecSubmission.end() ; ++vit )
    {
        KSubMission kSubMission;
        if( !GetSubMission( kSubMission, *vit ) )
        {
            START_LOG( cerr, L"Mission Data 정보 생성중 해당 서브 미션을 가져 오지 못함" )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( *vit ) << END_LOG;
            return false;
        }

        if( kSubMission.m_sCompletionInfo.m_nItemID > 0 )
            continue;

        if( kSubMission.m_sCompletionInfo.m_nCount <= 0 )
        {
            START_LOG( cerr, L"카운트용 미션인데 카운트가 없다... SubMission ID : " << kMissionInfo.m_dwMissionID ) << END_LOG;
            continue;
        }

        std::pair<int,int> prCompletion;
        prCompletion = std::make_pair( (int)kSubMission.m_dwSubmissionID, kSubMission.m_sCompletionInfo.m_nCount );
        if( !bFillCount_ )
            prCompletion.second = 0;

        kMissionData_.m_mapCompletionInfo.insert( prCompletion );
    }
    
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmEnd = tmCurrent;
    tmEnd += CTimeSpan( kMissionInfo.m_nPeriod, 0, 0, 0 );

    kMissionData_.m_tmRegDate = KncUtil::TimeToInt( tmCurrent );
    kMissionData_.m_tmEndDate = KncUtil::TimeToInt( tmEnd );
    return true;
}

bool KMissionManager::GetCountSubMissionList( OUT std::vector<int>& vecSubmissions_, IN const DWORD dwMissionID_ )
{
    vecSubmissions_.clear();
    KMissionInfo kMissionInfo;
    if( !GetMissionInfo( kMissionInfo, dwMissionID_ ) )
    {
        START_LOG( cerr, L"Count SubMission 정보 생성중 해당 미션을 가져 오지 못함, ID : " << dwMissionID_ ) << END_LOG;
        return false;
    }

    std::vector<int>::iterator vit; 
    for( vit = kMissionInfo.m_vecSubmission.begin() ; vit != kMissionInfo.m_vecSubmission.end() ; ++vit )
    {
        KSubMission kSubMission;
        if( !GetSubMission( kSubMission, *vit ) )
        {
            START_LOG( cerr, L"Count SubMission 정보 생성중 해당 서브 미션을 가져 오지 못함" )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( *vit ) << END_LOG;
            return false;
        }

        if( kSubMission.m_sCompletionInfo.m_nItemID > 0 )
            continue;

        if( kSubMission.m_sCompletionInfo.m_nCount <= 0 )
        {
            START_LOG( cerr, L"카운트용 미션인데 카운트가 없다... SubMission ID : " << kMissionInfo.m_dwMissionID ) << END_LOG;
            continue;
        }

        vecSubmissions_.push_back( (int)kSubMission.m_dwSubmissionID );
    }
    return true;
}

void KMissionManager::GetGoalCollection( IN const std::vector<KMissionData>& vecMissionSlot_, OUT std::map<int,int>& mapGoalCollection_ )
{
    mapGoalCollection_.clear();
    if( vecMissionSlot_.empty() )
        return;
    std::vector<KMissionData>::const_iterator vit;
    for( vit = vecMissionSlot_.begin() ; vit != vecMissionSlot_.end() ; ++vit )
    {
        KMissionInfo kMissionInfo;
        if( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) )
            continue;

        std::vector<int>::iterator vit2;
        for( vit2 = kMissionInfo.m_vecSubmission.begin() ; vit2 != kMissionInfo.m_vecSubmission.end() ; ++vit2 )
        {
            KSubMission kSubMission;
            if( !GetSubMission( kSubMission, *vit2 ) )
                continue;

            if( kSubMission.m_sCompletionInfo.m_nItemID <= 0 || kSubMission.m_sCompletionInfo.m_nCount <= 0 )
                continue;

            mapGoalCollection_[kSubMission.m_sCompletionInfo.m_nItemID] += kSubMission.m_sCompletionInfo.m_nCount;            
        }
    }
}

bool KMissionManager::GetCondition( IN const int nConditionID_, OUT sCondition& condition_ )
{
    KLocker lock( m_csMissions );
    std::map< int, sCondition >::iterator mit;
    mit = m_mapConditions.find( nConditionID_ );
    if( mit == m_mapConditions.end() )
        return false;

    condition_ = mit->second;
    return true;
}

bool KMissionManager::CheckMissionComplete( IN KUser& kUser_,
                                            IN const KMissionData& kMissionData_,
                                            IN KItemContainer& kInventory_,
                                            OUT std::vector<KItem>& vecDelItem_ )
{
    // 미션 완료 조건 체크 하고.
    // 삭제 아이템 리스트를 전달해준다.

    // 해당 미션이 존재하는지 체크
    KMissionInfo kMissionInfo;
    if( !GetMissionInfo( kMissionInfo, kMissionData_.m_dwMissionID ) )
    {
        START_LOG( cerr, L"해당 미션이 존재하지 않음." )
            << BUILD_LOG( kMissionData_.m_dwMissionID ) << END_LOG;
        SET_ERROR( ERR_MISSION_17 );
        return false;
    }

    // 캐릭터 레벨은 캐릭터 타입이 정의된 경우에만 체크.
    int nCharLv = -1;
    if ( kMissionInfo.m_cCharType >= 0 ) {
        const KCharacterInfo* pkCharInfo = kUser_.GetCharacterInfo( kMissionInfo.m_cCharType );
        if ( pkCharInfo ) {
            nCharLv = pkCharInfo->m_dwLevel;
        }
    }

    // 해당 미션의 완료 조건 체크
    std::vector<int>::iterator vit;
    for( vit = kMissionInfo.m_vecSubmission.begin() ; vit != kMissionInfo.m_vecSubmission.end() ; ++vit )
    {
        // 서브 미션이 존재하가?
        KSubMission kSubMission;
        if( !GetSubMission( kSubMission, *vit ) )
        {
            START_LOG( cerr, L"해당 서브 미션이 존재하지 않음." )
                << BUILD_LOG( kMissionData_.m_dwMissionID )
                << BUILD_LOG( *vit ) << END_LOG;
            SET_ERROR( ERR_MISSION_28 );
            return false;
        }

        // 서브 미션 스크립트에서 기록된 필요 카운트가 올바른가?
        if( kSubMission.m_sCompletionInfo.m_nCount < 0 )
        {
            START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 수치가 음수이다." )
                << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                << L"SubMissionID : " << *vit << dbg::endl
                << L"Need Count : " << kSubMission.m_sCompletionInfo.m_nCount  << END_LOG;
            SET_ERROR( ERR_MISSION_29 );
            return false;
        }

        // 아이템을 모아야 한다면.
        if( kSubMission.m_sCompletionInfo.m_nItemID > 0 )
        {
            KItemPtr pkItem = kInventory_.FindItemByItemID( kSubMission.m_sCompletionInfo.m_nItemID );

            //인벤토리에 요구한 아이템이 있는가?
            if( pkItem == NULL )
            {
                START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 아이템이 없음." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kSubMission.m_dwSubmissionID << dbg::endl
                    << L"ItemID : " <<  kSubMission.m_sCompletionInfo.m_nItemID  << END_LOG;
                SET_ERROR( ERR_MISSION_30 );
                return false;
            }

            //인벤토리에 요구한 아이템의 개수가 충분한가?
            if( pkItem->m_nCount < kSubMission.m_sCompletionInfo.m_nCount )
            {
                START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 아이템의 수량이 부족." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kSubMission.m_dwSubmissionID << dbg::endl
                    << L"ItemID : " << kSubMission.m_sCompletionInfo.m_nItemID << dbg::endl
                    << L"Need Count : " << kSubMission.m_sCompletionInfo.m_nCount << dbg::endl
                    << L"Current Count : " << pkItem->m_nCount  << END_LOG;
                SET_ERROR( ERR_MISSION_31 );
                return false;
            }

            // 충분하다면 해당 아이템의 필요 수량을 밖으로 보내자...^^...
            KItem kDelItem;
            kDelItem.m_ItemID    = (DWORD)kSubMission.m_sCompletionInfo.m_nItemID;
            kDelItem.m_ItemUID   = pkItem->m_ItemUID;
            kDelItem.m_nCount = (DWORD)kSubMission.m_sCompletionInfo.m_nCount;
            vecDelItem_.push_back( kDelItem );
        }
        else if ( kSubMission.m_sCompletionInfo.m_nCharLv > 0 ) // 캐릭터 레벨달성이면..
        {
            // 목표레벨 체크
            if ( nCharLv < kSubMission.m_sCompletionInfo.m_nCharLv ) {
                START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 캐릭터 레벨이 안됨." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kSubMission.m_dwSubmissionID << dbg::endl
                    << L"CharType : " << (int)kMissionInfo.m_cCharType << dbg::endl
                    << L"Need CharLv : " << kSubMission.m_sCompletionInfo.m_nCharLv << dbg::endl
                    << L"Current CharLv : " << nCharLv << END_LOG;
                SET_ERROR( ERR_MISSION_32 );
                return false;
            }
        }
        else // 수치를 채워야 한다면
        {
            std::map<int,int>::const_iterator mit;
            mit = kMissionData_.m_mapCompletionInfo.find( kSubMission.m_dwSubmissionID );
            //유저 사이드의 정보에 해당 서브 미션이 있는가?
            if( mit == kMissionData_.m_mapCompletionInfo.end() )
            {
                START_LOG( cerr, L"서버 사이드의 미션에서 가지고 있는 서브 미션 완료 정보가 없다.." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kSubMission.m_dwSubmissionID  << END_LOG;
                SET_ERROR( ERR_MISSION_33 );
                return false;
            }

            // 해당 서브 미션의 수치가 충분한가?
            if( mit->second < kSubMission.m_sCompletionInfo.m_nCount )
            {
                START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 수치 부족." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kSubMission.m_dwSubmissionID << dbg::endl
                    << L"Need Count : " << kSubMission.m_sCompletionInfo.m_nCount << dbg::endl
                    << L"Current Count : " << mit->second  << END_LOG;
                SET_ERROR( ERR_MISSION_34 );
                return false;
            }
        }
    }

    return true;
}

//한 유저에 대한 처리.
void KMissionManager::DropSubMission( IN KUser& kUser_, // 용도 : 이름,  미션리스트, 캐릭터, 아이템 
                                     IN KRoom& kRoom_, // 방에 유저 체크, 시작 유저 수 , 게임 시간 ,  모드, 난이도, 맵
                                     IN const KGameResultIn& gameResult_, // 캐릭터, 승리, 랭크포인트, 죽은 수, 킬 수, 
                                     IN const KEndGameReq& kReq_, //m_setKillMonster,  end stage,  게임시간, 
                                     OUT std::vector<KDropItemInfo>& vecdrops_ ) // 아이템 주는 경우 여기에 담음.
{
    START_LOG( clog, L"미션 처리전 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();
    KInDoorUserInfo* pkStartingInfo = kRoom_.FindInDoorUserInfo( kUser_.GetName() );
    JIF( pkStartingInfo != NULL ); // 유저 유효성 체크?

    // 플레이한 캐릭터 정보를 따로 담아 둔다.
    std::vector< char > vecCharacter;
    std::wstringstream stmChar;
    {
        std::vector< KCharacterExpInfo >::const_iterator vitCh;
        for ( vitCh = gameResult_.m_vecCharExp.begin() ; vitCh != gameResult_.m_vecCharExp.end() ; ++vitCh ) {
            stmChar << static_cast<int>(vitCh->m_cCharType) << L"(" << vitCh->m_nBaseExp << L") ";
            vecCharacter.push_back( vitCh->m_cCharType );
        }
    }
    
    if ( kUser_.m_vecMissionSlot.empty() ) {
        return;
    }

    std::vector<KMissionData>::iterator vit;
    for ( vit = kUser_.m_vecMissionSlot.begin() ; vit != kUser_.m_vecMissionSlot.end() ; ++vit ) {
        KMissionInfo kMissionInfo;
        if ( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) ) {
            START_LOG( cerr, L" 등록된 미션이 미션 스크립트에 없음. Name : " << kUser_.GetName() )
                << BUILD_LOG( vit->m_dwMissionID ) << END_LOG;
            continue;
        }

        int nCharLevel = -1;

        // 캐릭터 확인
        if ( kMissionInfo.m_cCharType >= 0 ) {
            if ( std::find( vecCharacter.begin(), vecCharacter.end(), kMissionInfo.m_cCharType ) == vecCharacter.end() ) {
                continue;
            }

            const KCharacterInfo* pkCharInfo = kUser_.GetCharacterInfo( kMissionInfo.m_cCharType );
            JIF( pkCharInfo );
            nCharLevel = static_cast<int>( pkCharInfo->m_dwLevel );

            if ( kMissionInfo.m_prLevelRage.first >= 0 &&
                kMissionInfo.m_prLevelRage.first > nCharLevel ) {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( nCharLevel ) << END_LOG;
                continue;
            }

            if ( kMissionInfo.m_prLevelRage.second >= 0 &&
                kMissionInfo.m_prLevelRage.second < nCharLevel ) {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( nCharLevel ) << END_LOG;
                continue;
            }
        }

        std::vector<int>::iterator vit2;
        for ( vit2 = kMissionInfo.m_vecSubmission.begin() ; vit2 != kMissionInfo.m_vecSubmission.end() ; ++vit2 ) {

            KSubMission kSubMission;
            if ( !GetSubMission( kSubMission, *vit2 ) ) {
                START_LOG( cerr, L"등록된 미션의 서브미션을 찾는중 서브미션 리스트에 존재하지 않음. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( *vit2 ) << END_LOG;
                continue;
            }

            if ( kSubMission.m_sActionInfo.m_bMonDrop ) {
                continue;
            }

            // 충분한가?
            int nRemaindCount = kSubMission.m_sCompletionInfo.m_nCount;

            if ( kSubMission.m_sCompletionInfo.m_nItemID > 0 ) { // 아이템인 경우
                KItemPtr pkItem = kUser_.m_kInventory.FindItemByItemID( kSubMission.m_sCompletionInfo.m_nItemID );
                if ( pkItem ) {
                    nRemaindCount -= (int)pkItem->m_nCount;
                }
            }
            else { // 카운트인경우
                nRemaindCount -= vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID];
            }

            nRemaindCount = std::max<int>( nRemaindCount, 0 );

            if ( nRemaindCount <= 0 ) {
                continue;
            }

            // 캐릭터 레벨달성 체크
            if ( kSubMission.m_sCompletionInfo.m_nCharLv > 0 ) {
                if ( nCharLevel < kSubMission.m_sCompletionInfo.m_nCharLv ) {
                    START_LOG( clog, L"캐릭터 레벨이 달성목표 레벨보다 작다.. Name : " << kUser_.GetName() )
                        << BUILD_LOGc( kMissionInfo.m_cCharType )
                        << BUILD_LOG( nCharLevel )
                        << BUILD_LOG( kSubMission.m_sCompletionInfo.m_nCharLv ) << END_LOG;
                    continue;
                }

                int nAddCount = std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID] += nAddCount;
                nRemaindCount -= nAddCount;

                START_LOG( clog, L" 레벨 달성으로 미션 카운트 증가. Name : " << kUser_.GetName()
                    << L", Missin ID : " << kMissionInfo.m_dwMissionID
                    << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                    << L", Count : " << kSubMission.m_sActionInfo.m_nCount )  << END_LOG;
                continue;
            }

            std::set<int>::iterator sit;
            for ( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit ) {
                sCondition condition;
                std::map<int,int> mapPositionUseCount;
                if ( !GetCondition( *sit, condition ) ) {
                    START_LOG( cerr, L"해당 서브 미션의 컨디션을 가져 오지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( *sit ) << END_LOG;
                    continue;
                }

                //-----------------------
                
				//-----------------------
				// 0이면 무조껀 오케이
				// 1이면 솔로만 오케이
				// 2이상 이면 해당 유저 이상만 체크
				if ( ( condition.m_nSolo == 1 && kRoom_.m_nStartingUserNum != 1  ) ||
					( condition.m_nSolo > 1 && kRoom_.m_nStartingUserNum < condition.m_nSolo ) )
				{
                    START_LOG( clog, L"솔로로 플레이 해야 하는데.... Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nSolo )
                        << BUILD_LOG( kRoom_.m_nStartingUserNum ) << END_LOG;
                    continue;
                }

                // 아래처럼 바꿔서는 그리 좋지 못하다는 것이 결론
                if ( false == CheckCondition( condition, gameResult_.m_bWin, kMissionInfo.m_dwMissionID, kSubMission.m_dwSubmissionID, kUser_.GetName() ) ) {// 여기에 TRUE 되어 있으면 않됨.                    
                    continue;
                }

                // 몬스터 킬은 실시간으로 처리됨.
                if ( condition.m_nMonsterKill >= 0 ) {
                    continue;
                }

                if ( condition.m_nStageID >= 0 &&
                    condition.m_nStageID != kReq_.m_iQuestEndingStage ) {
                    START_LOG( clog, L"End Stage 번호가 틀림.. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( kReq_.m_iQuestEndingStage )
                        << BUILD_LOG( condition.m_nStageID ) << END_LOG;
                    continue;
                }

                if ( condition.m_nElapsedTime > 0 && (kReq_.m_dwElapsedSec / 60.f) > condition.m_nElapsedTime ) {
                    START_LOG( clog, L"너무 오래 플레이 했삼.. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nElapsedTime )
                        << BUILD_LOG( (kRoom_.GetLatestPlayTime()/(1000 * 60)) )
                        << BUILD_LOG( (kReq_.m_dwElapsedSec / 60) ) << END_LOG;
                    continue;
                }

                if ( condition.m_nExLife >= 0 ) {
                    START_LOG( clog, L"미구현 부분(정보 부족) ExLife가 있음." )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nExLife ) << END_LOG;
                    continue;
                }

                if ( condition.m_nDie >= 0 && gameResult_.m_iNumDies > condition.m_nDie ) {
                    START_LOG( clog, L"많이 죽었음.. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nDie )
                        << BUILD_LOG( gameResult_.m_iNumDies ) << END_LOG;
                    continue;
                }

                // PvP킬은 실시간으로 처리됨.
                if ( condition.m_nKill >= 0 ) {
                    continue;
                }

                if ( condition.m_nModeID >= 0 && kRoom_.m_iGameMode != condition.m_nModeID ) {
                    START_LOG( clog, L" Mode 가 맞지 않음 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOG( kRoom_.m_iGameMode ) << END_LOG;
                    continue;
                }

                if ( condition.m_prDifficultRage.first >= 0 &&
                    condition.m_prDifficultRage.first > kRoom_.m_nDifficulty ) {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_prDifficultRage.first )
                        << BUILD_LOG( kRoom_.m_nDifficulty ) << END_LOG;
                    continue;
                }

                if ( condition.m_prDifficultRage.second >= 0 &&
                    condition.m_prDifficultRage.second < kRoom_.m_nDifficulty ) {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_prDifficultRage.second )
                        << BUILD_LOG( kRoom_.m_nDifficulty ) << END_LOG;
                    continue;
                }

                if ( condition.m_nMapID >= 0 && kRoom_.m_iMapID != condition.m_nMapID ) {
                    START_LOG( clog, L"맵이 올바르지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nMapID )
                        << BUILD_LOG( kRoom_.m_iMapID ) << END_LOG;
                    continue;
                }

                if ( condition.m_fRatio < GetRatio() ) {
                    START_LOG( clog, L"확률이 낮음. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nMapID )
                        << BUILD_LOG( condition.m_fRatio ) << END_LOG;
                    continue;
                }

                // 몬스터 레벨 체크는 실시간으로 처리됨
                if ( condition.m_prMonLevelRange.first >= 0 || condition.m_prMonLevelRange.second >= 0 ) {
                    continue;
                }

                kRoom_.GetUserUsePosition( kUser_.GetUID(), mapPositionUseCount );
                // 미션 Type 조건에 따른 처리.
                // 해당 처리는 아직 특정 던전들에 국한적이다. 해당 부분이 던전 종속성이 없어지만
                // 위에 조건 처리 부분에 ModeID부분 체크를 넘어가야 한다.
                if( !CheckTypeCondition( kUser_.GetName(), kMissionInfo.m_dwMissionID, kSubMission.m_dwSubmissionID, condition, gameResult_.m_kUserControlInfoData, mapPositionUseCount ) ) {
                    continue;
                }

                // 아이템을 줘야 하는 경우..
                if( kSubMission.m_sActionInfo.m_nItemID > 0 ) {
                    KDropItemInfo kItemUnit;
                    kItemUnit.m_ItemID = (GCITEMID)kSubMission.m_sActionInfo.m_nItemID;
                    kItemUnit.m_nDuration = kSubMission.m_sActionInfo.m_nCount;
                    vecdrops_.push_back( kItemUnit );

                    START_LOG( clog, L" 미션으로 아이템 얻음. Name : " << kUser_.GetName()
                        << L", Missin ID : " << kMissionInfo.m_dwMissionID
                        << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                        << L", ItemID : " << kItemUnit.m_ItemID
                        << L", Count : " << kItemUnit.m_nDuration ) << END_LOG;
                }
                else { // 카운트를 올려야 하는경우.
                    std::map<int,int>::iterator mit;
                    mit = vit->m_mapCompletionInfo.find( (int)kSubMission.m_dwSubmissionID );
                    if( mit == vit->m_mapCompletionInfo.end() )
                    {
                        START_LOG( cerr, L"해당 유저가 서브 미션 완료 목록을 가지고 있지 않다. Name : " << kUser_.GetName() )
                            << BUILD_LOG( kMissionInfo.m_dwMissionID )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID ) << END_LOG;
                        continue;
                    }

                    mit->second += std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );
                    nRemaindCount -= std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                    START_LOG( clog, L" 미션으로 카운트 증가. Name : " << kUser_.GetName()
                        << L", Missin ID : " << kMissionInfo.m_dwMissionID
                        << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                        << L", Count : " << kSubMission.m_sActionInfo.m_nCount ) << END_LOG;
                }
                
            }
        }
    }

    START_LOG( clog, L"미션 처리 완료 후 미션 슬롯"  ) << END_LOG;

    stmChar.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stmChar.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    kUser_.DumpMissionSlot();
}

bool KMissionManager::CheckCondition( IN sCondition& condition, IN bool bResultWin_, DWORD dwMissionID, DWORD dwSubmissionID, const std::wstring& strName )
{
    // 승리조건 체크 안함
    if ( false == condition.m_bWin ) {
        return true;
    }

    if ( false == bResultWin_ ) {
        START_LOG( clog, L"승리조건인데, 이기지 못함. Name : " << strName )
            << BUILD_LOG( dwMissionID )
            << BUILD_LOG( dwSubmissionID )
            << BUILD_LOG( condition.m_nID ) << END_LOG;
        return false;
    }
    return true;
}

bool KMissionManager::IsDuplicateMission( IN const std::vector<DWORD>& vecMissions_, IN const DWORD dwMissionID_ )
{
    return std::find( vecMissions_.begin(), vecMissions_.end(), dwMissionID_ ) != vecMissions_.end();
}

bool KMissionManager::IsInInvalidMission( IN const std::vector<KMissionData>& vecMissionSlot_, IN const DWORD dwMissionID_ )
{
    KMissionInfo kMissionInfo;
    if( !GetMissionInfo( kMissionInfo, dwMissionID_ ) )
    {
        START_LOG( cerr, L"미션 스크립트에 없는 미션입니다" )
            << BUILD_LOG( dwMissionID_ )
            << END_LOG;
        return true;
    }

    std::vector<KMissionData>::const_iterator vit;
    for( vit = vecMissionSlot_.begin() ; vit != vecMissionSlot_.end() ; ++vit )
    {
        if( IsDuplicateMission( kMissionInfo.m_vecInvalidMission, vit->m_dwMissionID ) )
        {
            START_LOG( cerr, L"등록할 수 없는 InvalidMission " )
                << BUILD_LOG( vit->m_dwMissionID )
                << BUILD_LOG( dwMissionID_ )
                << END_LOG;
            return true;
        }
    }
    return false;
}

bool KMissionManager::IsInInvalidCompleteMission( IN const std::vector<DWORD>& vecMissionSlot_, IN const DWORD dwMissionID_ )
{
    std::vector<DWORD>::const_iterator vit;
    for( vit = vecMissionSlot_.begin() ; vit != vecMissionSlot_.end() ; ++vit )
    {
        KMissionInfo kMissionInfo;
        if( !GetMissionInfo( kMissionInfo, *vit ) )
        {
            START_LOG( cerr, L"존재하는 미션이 아님... ID : " << dwMissionID_ ) << END_LOG;
            return true;
        }

        if( IsDuplicateMission( kMissionInfo.m_vecInvalidMission, dwMissionID_ ) )
        {
            return true;
        }
    }
    return false;
}

int KMissionManager::GetCount( IN const std::vector<KMissionData>& vecMissions_, IN const char cCharacter_ )
{
    //공통캐릭과 현재 캐릭의 개수를 반환
    int nMissionCount = 0;
    std::vector<KMissionData>::const_iterator vit;
    for( vit = vecMissions_.begin() ; vit != vecMissions_.end() ; ++ vit )
    {
        std::map<DWORD, KMissionInfo>::iterator mit;
        KLocker lock( m_csMissions );
        if( (mit = m_mapMissionInfo.find( vit->m_dwMissionID )) != m_mapMissionInfo.end() )
        {
            if( mit->second.m_cCharType == cCharacter_ )
            {
                ++nMissionCount;
            }
        }
    }
    return nMissionCount;
}


void KMissionManager::PrintSubMission( int iIndex )
{
    std::cout << "Print SubMission : " << iIndex << std::endl;
    KSubMission kSubMission;
    if( !GetSubMission( kSubMission, iIndex ) )
    {
        std::cout << "Not Exits : " << iIndex << std::endl;
        return;
    }
    kSubMission.ToString( std::wcout );
}

void KMissionManager::PrintMission( int iIndex )
{
    std::cout << "Print Mission : " << iIndex << std::endl;
    KMissionInfo kMissionInfo;
    if( !GetMissionInfo( kMissionInfo, iIndex ) )
    {
        std::cout << "Not Exits : " << iIndex << std::endl;
        return;
    }
    kMissionInfo.ToString( std::wcout );
}

void KMissionManager::ClearAll()
{
    KLocker lock( m_csMissions );
    m_mapConditions.clear();
    m_mapSubmission.clear();
    m_mapMissionInfo.clear();
}

void KMissionManager::GetMissionDropList( IN const KGameInfo& kGameInfo_, IN std::vector<sTriggerInfo> vecTriggers_,
                                          OUT std::vector< KGameDropElement >& vecMissionDrop_ )
{

    std::vector<DWORD>::const_iterator vit;
    for( vit = kGameInfo_.m_vecUserUIDs.begin() ; vit != kGameInfo_.m_vecUserUIDs.end() ; ++vit )
    {
        
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vit );
        if( !spUser )
            continue;

        if( spUser->m_vecMissionSlot.empty() )
            continue;

        std::map<int,int> mapGoalCollection;
        GetGoalCollection( spUser->m_vecMissionSlot, mapGoalCollection );
        if( mapGoalCollection.empty() )
            continue;

        // 해당 유저의 미션을 이터레이션 한다.
        std::vector<KMissionData>::iterator vit2;
        for( vit2 = spUser->m_vecMissionSlot.begin() ; vit2 != spUser->m_vecMissionSlot.end() ; ++vit2 )
        {
            KMissionInfo kMissionInfo;
            if( !GetMissionInfo( kMissionInfo, vit2->m_dwMissionID ) )
            {
                continue;
            }

            // 해당 미션과 캐릭터 타입이 맞지 않으면 스킵..
            if( kMissionInfo.m_cCharType != -1 && kMissionInfo.m_cCharType != spUser->GetCurrentCharType() )
                continue;
            
            std::vector<int>::iterator vit3;
            for( vit3 = kMissionInfo.m_vecSubmission.begin() ; vit3 != kMissionInfo.m_vecSubmission.end() ; ++vit3 )
            {
                KSubMission kSubMission;
                if( !GetSubMission( kSubMission, *vit3 ) )
                {
                    START_LOG( cerr, L"해당 서브 미션을 가져 오지 못했음." )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( *vit3 ) << END_LOG;
                    continue;
                }

                // 몬스터 드랍이 아니면 쌩
                if( !kSubMission.m_sActionInfo.m_bMonDrop )
                    continue;

                // 콜랙션 정보를 가져 온다.
                std::map<int,int>::iterator mit;
                mit = mapGoalCollection.find( kSubMission.m_sActionInfo.m_nItemID );
                if( mit == mapGoalCollection.end() )
                {
                    START_LOG( cerr, L" 몬스터가 드랍한다고 해놓고 해당 서브 미션에 정보가 없음??" )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( kSubMission.m_sActionInfo.m_nItemID ) << END_LOG;
                }

                // 모을수 있는 수량..
                int nCurrentCount = (int)spUser->GetDurItemCount( kSubMission.m_sActionInfo.m_nItemID );
                int nRemaindCount = mit->second - nCurrentCount;

                // 이제 해당 서브 미션의 Condition을 보고 결정하자..
                std::set<int>::iterator sit;
                for( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit )
                {
                    sCondition condition;
                    if( !GetCondition( *sit, condition ) )
                    {
                        START_LOG( cerr, L" 서브 미션에 등록 되어 있는 컨디션이 실제 컨디션 목록에 없음.." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( *sit ) << END_LOG;
                        continue;
                    }

                    // 레벨별 서버 혜택.
                    float fServerBenefit = 1.f;
                    if ( SiKServerBenefit()->IsCorrectCondition( *sit ) ) {
                        const KCharacterInfo* pkChar = spUser->GetCurrentCharacter();
                        if ( pkChar != NULL && SiKServerBenefit()->IsCorrectLvRange( (const int)pkChar->m_dwLevel ) ) {
                            fServerBenefit += SiKServerBenefit()->GetConditionBenefit();
                        }
                    }

					//-----------------------
					// 0이면 무조껀 오케이
					// 1이면 솔로만 오케이
					// 2이상 이면 해당 유저 이상만 체크
					if ( ( condition.m_nSolo == 1 && kGameInfo_.m_nStartUserNum != 1  ) ||
						( condition.m_nSolo > 1 && kGameInfo_.m_nStartUserNum < condition.m_nSolo ) )
                    {
                        START_LOG( clog, L"솔로로 플레이 해야 함.." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nSolo )
                            << BUILD_LOG( kGameInfo_.m_nStartUserNum ) << END_LOG;
                        continue;
                    }


                    if( condition.m_bWin ) // 여기에 TRUE 되어 있으면 않됨.
                    {
                        START_LOG( cerr, L"서브 미션에 Montrop 이 True인데 win이 적용되어 있음" )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID ) << END_LOG;
                        continue;
                    }

                    if( condition.m_nStageID >= 0 )
                    {
                        START_LOG( cerr, L"서브 미션에 Montrop 이 True인데 Stage ID 가 있음." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nStageID ) << END_LOG;
                    }

                    if( condition.m_nElapsedTime >= 0 )
                    {
                        START_LOG( cerr, L"서브 미션에 Montrop 이 True인데 Elapsed Time이 있음." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nElapsedTime ) << END_LOG;
                    }

                    if( condition.m_nExLife >= 0 )
                    {
                        START_LOG( cerr, L"서브 미션에 MonDrop 이 True인데 ExLife가 있음." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nExLife ) << END_LOG;
                    }

                    if( condition.m_nDie >= 0 )
                    {
                        START_LOG( cerr, L"서브 미션에 Montrop 이 True인데 Die가 있음." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nDie ) << END_LOG;
                    }
                    
                    if( condition.m_nKill >= 0 )
                    {
                        START_LOG( cerr, L"서브 미션에 Montrop 이 True인데 Kill이 있음." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nKill ) << END_LOG;
                    }

                    if( condition.m_nModeID >= 0 && kGameInfo_.m_iGameMode != condition.m_nModeID )
                    {
                        START_LOG( clog, L" Mode 가 맞지 않음" )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nModeID )
                            << BUILD_LOG( kGameInfo_.m_iGameMode ) << END_LOG;
                        continue;
                    }

                    if( condition.m_prDifficultRage.first >= 0 &&
                        condition.m_prDifficultRage.first > kGameInfo_.m_nDifficulty )
                    {
                        START_LOG( clog, L"난이도 범위를 만족하지 못함" )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_prDifficultRage.first )
                            << BUILD_LOG( kGameInfo_.m_nDifficulty ) << END_LOG;
                        continue;
                    }

                    if( condition.m_prDifficultRage.second >= 0 &&
                        condition.m_prDifficultRage.second < kGameInfo_.m_nDifficulty )
                    {
                        START_LOG( clog, L"난이도 범위를 만족하지 못함" )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_prDifficultRage.second )
                            << BUILD_LOG( kGameInfo_.m_nDifficulty ) << END_LOG;
                        continue;
                    }

                    if( condition.m_nMapID >= 0 && kGameInfo_.m_iMapID != condition.m_nMapID )
                    {
                        START_LOG( clog, L"맵이 올바르지 못함." )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nID )
                            << BUILD_LOG( condition.m_nMapID )
                            << BUILD_LOG( kGameInfo_.m_iMapID ) << END_LOG;
                        continue;
                    }

                    // 몬스터들 모두 검사한다..
                    std::vector<sTriggerInfo>::const_iterator vit4;
                    for( vit4 = vecTriggers_.begin() ; vit4 != vecTriggers_.end() ; ++vit4 )
                    {
                        if( nRemaindCount <= 0 )
                            break;

                        if( vit4->m_nMonsterID == condition.m_nMonsterKill &&
                            (condition.m_fRatio * fServerBenefit) >= GetRatio() )
                        {
                            // 이제 드랍하자....
                            KGameDropElement kDropElement;
                            kDropElement.m_cRewardType  = KGameDropElement::RT_MISSION_ITEM;
                            kDropElement.m_dwItemID     = (DWORD)kSubMission.m_sActionInfo.m_nItemID;
                            kDropElement.m_nCount      = kSubMission.m_sActionInfo.m_nCount;
                            kDropElement.m_nPeriod      = -1;
                            kDropElement.m_nTriggerID   = vit4->m_nTriggerID;
                            kDropElement.m_nMonID       = vit4->m_nMonsterID;
                            kDropElement.m_dwUserUID    = spUser->GetUID();
                            kDropElement.m_nStageCountType = -1;

                            vecMissionDrop_.push_back( kDropElement );
                            --nRemaindCount;

                            START_LOG( clog, L"맞는 몬스터가 나옴... Name : " << spUser->GetName() )
                                << L"Mission ID : " << kMissionInfo.m_dwMissionID
                                << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                                << L", Condition ID : " << condition.m_nID
                                << L", MonID : " << vit4->m_nMonsterID
                                << L", Trigger ID : " << vit4->m_nTriggerID
                                << L", Itme ID : " << kSubMission.m_sActionInfo.m_nItemID
                                << L", Factor : " << kSubMission.m_sActionInfo.m_nCount  << END_LOG;
                        } // if
                    } // for
                } // for( sit = kSubMission 
            } // for( vit3
        } // for( vit2 =
    } // for( vit = kGameInfo_
}


float KMissionManager::GetRatio()
{
    return GetRandomNum()/(float)(UINT_MAX);
}

void KMissionManager::DumpUnique()
{
    std::wstringstream stm;
    std::map< DWORD, KMissionInfo >::iterator mit;
    KLocker lock( m_csMissions );
    for( mit = m_mapMissionInfo.begin() ; mit != m_mapMissionInfo.end() ; ++mit )
    {
        if( mit->second.m_bUnique )
            stm << mit->first << L", ";
    }

    //std::cout << L" Unique Mission : " << stm.str() << dbg::endl;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

bool KMissionManager::ProcessMiniGame( IN KMiniGameRankInfo& kGamResult_, IN OUT std::vector<KMissionData>& vecMissionSlot_, IN std::wstring& strLogin_ )
{
    bool bChange = false;
    if( vecMissionSlot_.empty() ) return bChange;


    std::vector<KMissionData>::iterator vit;
    for( vit = vecMissionSlot_.begin() ; vit != vecMissionSlot_.end() ; ++vit )
    {
        KMissionInfo kMissionInfo;
        if( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) )
        {
            continue;
        }

        if( kMissionInfo.m_cCharType != -1 && kMissionInfo.m_cCharType != kGamResult_.m_cCharType )
        {
            continue;
        }

        std::vector<int>::iterator vit1;
        for( vit1 = kMissionInfo.m_vecSubmission.begin() ; vit1 != kMissionInfo.m_vecSubmission.end() ; ++vit1 )
        {
            KSubMission kSubMission;
            if( !GetSubMission( kSubMission, *vit1 ) )
            {
                continue;
            }

            if( kSubMission.m_setConditions.empty() ||
                kSubMission.m_sActionInfo.m_nItemID > 0 ||
                kSubMission.m_sActionInfo.m_nCount <= 0 ||
                kSubMission.m_sActionInfo.m_bMonDrop == true ||
                kSubMission.m_sCompletionInfo.m_nItemID > 0 ||
                kSubMission.m_sCompletionInfo.m_nCount <= 0 ||
                kSubMission.m_sCompletionInfo.m_nCharLv > 0 )
            {
                START_LOG( cwarn, L"미니 게임의 기본이 안되어 있음.. Name : " << strLogin_ )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kSubMission.m_setConditions.empty() )
                    << BUILD_LOG( kSubMission.m_sActionInfo.m_nItemID )
                    << BUILD_LOG( kSubMission.m_sActionInfo.m_nCount )
                    << BUILD_LOG( kSubMission.m_sActionInfo.m_bMonDrop )
                    << BUILD_LOG( kSubMission.m_sCompletionInfo.m_nItemID )
                    << BUILD_LOG( kSubMission.m_sCompletionInfo.m_nCount )
                    << BUILD_LOG( kSubMission.m_sCompletionInfo.m_nCharLv ) << END_LOG;
                continue;
            }

            if( vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID] >= kSubMission.m_sCompletionInfo.m_nCount )
            {
                //이미 다 모았네..
                continue;
            }

            std::set<int>::iterator sit;
            for( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit )
            {
                sCondition condition;
                if( !GetCondition( *sit, condition ) )
                {
                    START_LOG( cwarn, L"미니 게임의 컨디션이 없음. Name : " << strLogin_ )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( *sit ) << END_LOG;
                    continue;
                }


                if( condition.m_nModeID != -1 && condition.m_nModeID != kGamResult_.m_cGameMode )
                {
                    START_LOG( cwarn, L"미니 게임 모드가 맞지 않음. Name : " << strLogin_ )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOGc( kGamResult_.m_cGameMode ) << END_LOG;
                    continue;
                }

                if( kSubMission.m_sActionInfo.m_nCount > 0 )
                {
                    vit->m_mapCompletionInfo[kSubMission.m_dwSubmissionID] = std::min<int>( vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID], kSubMission.m_sCompletionInfo.m_nCount );
                    bChange = true;
                    START_LOG( clog, L"미니 게임 적용. Name : " << strLogin_ )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOG( kGamResult_.m_dwScore )
                        << BUILD_LOG( vit->m_mapCompletionInfo[kSubMission.m_dwSubmissionID] ) << END_LOG;
                }
            }
        }
    }

    return bChange;
}

void KMissionManager::GetMissionBonusExp( int& Exp_ )
{
    KLocker lock(m_csMissionBonusExpGp);
    
    if( m_fMissionBonusExp > 0 )
    {
        Exp_ = (int)(Exp_ * m_fMissionBonusExp);
    }
    else
    {
        // nothing
    }
}

void KMissionManager::GetMissionBonusGp( int& Gp_ )
{
    KLocker lock(m_csMissionBonusExpGp);

    if( m_fMissionBonusGp > 0 )
    {
        Gp_ = (int)( Gp_ * m_fMissionBonusGp );
    }
    else
    {
        // nothing
    }
}


bool KMissionManager::LoadMissionPenalty( OUT std::string& strScript_ )
{
    std::vector< KMissionPenaltyInfo > vecMissionPenalty;
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitMissionPenalty.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "MissionPenalty" ) == S_OK, return false );
    for ( int i = 1 ; ; ++i ) {
        KMissionPenaltyInfo kInfo;
        if ( !LoadPenaltyInfo( kLuaMng, i, kInfo ) )
            break;

        vecMissionPenalty.push_back( kInfo );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"미션 패널티 읽기 완료. Size : " << vecMissionPenalty.size() ) << END_LOG;

    std::sort( vecMissionPenalty.begin(), vecMissionPenalty.end(), KComp() );

    KLocker lock( m_csMissionPenalty );
    m_vecMissionPenalty.swap( vecMissionPenalty );
    return true;
}

bool KMissionManager::LoadPenaltyInfo( IN KLuaManager& kLuaMng_, IN const int nIndex_, OUT KMissionPenaltyInfo& kInfo_ )
{
    if ( kLuaMng_.BeginTable( nIndex_ ) != S_OK ) return false;
    _JIF( kLuaMng_.GetValue( 1, kInfo_.m_prLvDiffRange.first ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 2, kInfo_.m_prLvDiffRange.second ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 3, kInfo_.m_fExpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( 4, kInfo_.m_fGpRatio ) == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KMissionManager::GetMissionPenaltyRatio( IN const int nMissionLv_, IN const int nCharLv_, OUT float& fExpRatio_, OUT float& fGPRatio_ )
{
    fExpRatio_ = 1.f;
    fGPRatio_ = 1.f;

    if ( nMissionLv_ < 0 )
        return;

    KMissionPenaltyInfo kInfo;
    JIF( GetPenaltyInfo( nCharLv_ - nMissionLv_, kInfo ) );

    fExpRatio_ = kInfo.m_fExpRatio;
    fGPRatio_ = kInfo.m_fGpRatio;

    return;
}

bool KMissionManager::GetPenaltyInfo( IN const int nLvDiff_, OUT KMissionPenaltyInfo& kInfo_ )
{
    typedef std::vector<KMissionPenaltyInfo>::iterator PenaltyIT;
    std::pair<PenaltyIT,PenaltyIT> prPenaltyIT;

    KLocker lock( m_csMissionPenalty );
    prPenaltyIT = std::equal_range( m_vecMissionPenalty.begin(), m_vecMissionPenalty.end(), nLvDiff_, KComp() );

    if ( prPenaltyIT.first == prPenaltyIT.second ) return false;

    kInfo_ = *prPenaltyIT.first;
    return true;
}

void KMissionManager::MonsterKillSubMission( IN KUser& kUser_, IN const char& cCharType_, IN const int& nStartNum_, IN const int& nModeID_, IN const int& nDifficulty_, IN const int& nMonID_, IN const int& nMonLv_ )
{
    START_LOG( clog, L"몬스터킬 미션처리전 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();

    if ( kUser_.m_vecMissionSlot.empty() ) {
        return;
    }

    std::vector<KMissionData>::iterator vit;
    for ( vit = kUser_.m_vecMissionSlot.begin() ; vit != kUser_.m_vecMissionSlot.end() ; ++vit ) {
        KMissionInfo kMissionInfo;
        if ( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) ) {
            START_LOG( cerr, L" 등록된 미션이 미션 스크립트에 없음. Name : " << kUser_.GetName() )
                << BUILD_LOG( vit->m_dwMissionID ) << END_LOG;
            continue;
        }

        // 캐릭터 확인
        int nCharLv = -1;
        KCharacterInfo* pkCharInfo = kUser_.GetCharacterInfo( cCharType_ );
        JIF( pkCharInfo );

        if ( kMissionInfo.m_cCharType >= 0 ) {

            if ( kMissionInfo.m_cCharType != cCharType_ ) {
                START_LOG( clog, L"플레이 캐릭터와 미션 캐릭터가 다르다. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOGc( kMissionInfo.m_cCharType )
                    << BUILD_LOGc( cCharType_ ) << END_LOG;
                continue;
            }

            nCharLv = pkCharInfo->m_dwLevel;
        }

        // 레벨 체크
        if ( kMissionInfo.m_prLevelRage.first >= 0 &&
            kMissionInfo.m_prLevelRage.first > (int)pkCharInfo->m_dwLevel ) {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( pkCharInfo->m_dwLevel ) << END_LOG;
                continue;
        }

        if ( kMissionInfo.m_prLevelRage.second >= 0 &&
            kMissionInfo.m_prLevelRage.second < (int)pkCharInfo->m_dwLevel ) {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( pkCharInfo->m_dwLevel ) << END_LOG;
                continue;
        }

        std::vector<int>::iterator vit2;
        for ( vit2 = kMissionInfo.m_vecSubmission.begin() ; vit2 != kMissionInfo.m_vecSubmission.end() ; ++vit2 ) {
            KSubMission kSubMission;
            if ( !GetSubMission( kSubMission, *vit2 ) ) {
                START_LOG( cerr, L"등록된 미션의 서브미션을 찾는중 서브미션 리스트에 존재하지 않음. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( *vit2 ) << END_LOG;
                continue;
            }

            // 몬스터 드랍은 다른 루트에서 처리됨.
            if ( kSubMission.m_sActionInfo.m_bMonDrop ) {
                continue;
            }

            // 아이템 드랍은 다른 루트에서 처리
            if ( kSubMission.m_sCompletionInfo.m_nItemID > 0 ) {
                continue;
            }

            // 몬스터 kill 카운트 조건 만족하는가?
            int nRemaindCount = kSubMission.m_sCompletionInfo.m_nCount;
            nRemaindCount -= vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID];
            nRemaindCount = std::max<int>( nRemaindCount, 0 );

            if ( nRemaindCount <= 0 ) {
                continue;
            }

            // 캐릭터 레벨조건체크
            if ( kSubMission.m_sCompletionInfo.m_nCharLv > 0 ) {

                if ( nCharLv < kSubMission.m_sCompletionInfo.m_nCharLv ) {
                    continue;
                }

                int nAddCount = std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID] += nAddCount;
                nRemaindCount -= nAddCount;

                START_LOG( clog, L" 레벨 달성으로 미션 카운트 증가. Name : " << kUser_.GetName()
                    << L", Missin ID : " << kMissionInfo.m_dwMissionID
                    << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                    << L", Count : " << kSubMission.m_sActionInfo.m_nCount ) << END_LOG;
                continue;
            }

            std::set<int>::iterator sit;
            for ( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit ) {

                sCondition condition;
                if ( !GetCondition( *sit, condition ) ) {
                    START_LOG( cerr, L"해당 서브 미션의 컨디션을 가져 오지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( *sit ) << END_LOG;
                    continue;
                }

				//-----------------------
				// 0이면 무조껀 오케이
				// 1이면 솔로만 오케이
				// 2이상 이면 해당 유저 이상만 체크
				if ( ( condition.m_nSolo == 1 && nStartNum_ != 1  ) ||
					( condition.m_nSolo > 1 && nStartNum_ < condition.m_nSolo ) )
				{
                    START_LOG( clog, L"솔로로 플레이 해야 하는데.... Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nSolo )
                        << BUILD_LOG( nStartNum_ ) << END_LOG;
                    continue;
                }

                // 아래처럼 바꿔서는 그리 좋지 못하다는 것이 결론
                if ( false == CheckCondition( condition, false, kMissionInfo.m_dwMissionID, kSubMission.m_dwSubmissionID, kUser_.GetName() ) ) {
                    continue;
                }

                // 몬스터 킬 체크인데, 몬스터 정보(MonID,Lv range)가 없음.
                if ( condition.m_nMonsterKill < 0 && condition.m_prMonLevelRange.first < 0 && condition.m_prMonLevelRange.second < 0 ) {
                    continue;
                }

                if ( condition.m_nMonsterKill >= 0 && condition.m_nMonsterKill != nMonID_ ) {
                    START_LOG( clog, L"필요한 몬스터를 못죽였네... Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nMonsterKill )
                        << BUILD_LOG( nMonID_ ) << END_LOG;
                    continue;
                }

                // 스테이지 체크 안함
                if ( condition.m_nStageID >= 0 ) {
                    continue;
                }
                // 클리어 시간 체크 안함
                if ( condition.m_nElapsedTime > 0 ) {
                    continue;
                }
                // 미구현
                if ( condition.m_nExLife >= 0 ) {
                    continue;
                }
                // 사용목숨수 체크 안함
                if ( condition.m_nDie >= 0 ) {
                    continue;
                }
                // 유저 킬 체크 안함
                if ( condition.m_nKill >= 0 ) {
                    continue;
                }

                if ( condition.m_nModeID >= 0 && nModeID_ != condition.m_nModeID ) {
                    START_LOG( clog, L" Mode 가 맞지 않음 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                if( condition.m_prDifficultRage.first >= 0 &&
                    condition.m_prDifficultRage.first > nDifficulty_ )
                {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_prDifficultRage.first )
                        << BUILD_LOG( nDifficulty_ ) << END_LOG;
                    continue;
                }

                if( condition.m_prDifficultRage.second >= 0 &&
                    condition.m_prDifficultRage.second < nDifficulty_ )
                {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_prDifficultRage.second )
                        << BUILD_LOG( nDifficulty_ ) << END_LOG;
                    continue;
                }

                // 던전에서 맵 체크 안함.
                if ( condition.m_nMapID >= 0 ) {
                    continue;
                }

                // 몬스터 레벨 체크
                if ( condition.m_prMonLevelRange.first >= 0 || condition.m_prMonLevelRange.second >= 0 ) {

                    KCharacterInfo* pkCharInfo = kUser_.GetCurrentCharacter();
                    JIF( pkCharInfo );

                    if ( nMonLv_ < static_cast<int>(pkCharInfo->m_dwLevel - condition.m_prMonLevelRange.first ) ) {
                        START_LOG( clog, L"몬스터 레벨이 캐릭터 조건 최소 레벨보다 작다. Name : " << kUser_.GetName() )
                            << BUILD_LOG( pkCharInfo->m_dwLevel )
                            << BUILD_LOG( nMonLv_ )
                            << BUILD_LOG( condition.m_prMonLevelRange.first ) << END_LOG;
                        continue;
                    }

                    if ( nMonLv_ > static_cast<int>(pkCharInfo->m_dwLevel + condition.m_prMonLevelRange.second ) ) {
                        START_LOG( clog, L"몬스터 레벨이 캐릭터 조건 최대 레벨보다 크다. Name : " << kUser_.GetName() )
                            << BUILD_LOG( pkCharInfo->m_dwLevel )
                            << BUILD_LOG( nMonLv_ )
                            << BUILD_LOG( condition.m_prMonLevelRange.second ) << END_LOG;
                        continue;
                    }
                }

                // 아이템을 줘야 하는 경우는 다른 루트에서 체크됨.
                if( kSubMission.m_sActionInfo.m_nItemID > 0 ) {
                    continue;
                }

                // 카운트를 올려야 하는경우.
                std::map<int,int>::iterator mit;
                mit = vit->m_mapCompletionInfo.find( (int)kSubMission.m_dwSubmissionID );
                if ( mit == vit->m_mapCompletionInfo.end() ) {
                    START_LOG( cerr, L"해당 유저가 서브 미션 완료 목록을 가지고 있지 않다. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID ) << END_LOG;
                    continue;
                }

                mit->second += std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );
                nRemaindCount -= std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                START_LOG( clog, L" 몬스터킬 미션 카운트 증가. Name : " << kUser_.GetName()
                    << L", Missin ID : " << kMissionInfo.m_dwMissionID
                    << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                    << L", Count : " << (kSubMission.m_sActionInfo.m_nCount ) ) << END_LOG;


            }// End Condition
        }// End Submission
    }// End MissionSlot

    START_LOG( clog, L"몬스터킬 미션처리 완료 후 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();
}

void KMissionManager::PvpKillSubmission( IN KUser& kUser_, IN const char& cCharType_, IN const int& nStartNum_, IN const int& nModeID_, IN const int& nMapID_ )
{
    START_LOG( clog, L"PvP킬 미션처리전 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();

    if ( kUser_.m_vecMissionSlot.empty() ) {
        return;
    }

    std::vector<KMissionData>::iterator vit;
    for ( vit = kUser_.m_vecMissionSlot.begin() ; vit != kUser_.m_vecMissionSlot.end() ; ++vit ) {
        KMissionInfo kMissionInfo;
        if ( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) ) {
            START_LOG( cerr, L" 등록된 미션이 미션 스크립트에 없음. Name : " << kUser_.GetName() )
                << BUILD_LOG( vit->m_dwMissionID ) << END_LOG;
            continue;
        }

        int nCharLv = -1;

        // 캐릭터 확인
        KCharacterInfo* pkCharInfo = kUser_.GetCharacterInfo( cCharType_ );
        JIF( pkCharInfo );

        if ( kMissionInfo.m_cCharType >= 0 ) {

            if ( kMissionInfo.m_cCharType != cCharType_ ) {
                START_LOG( clog, L"플레이 캐릭터와 미션 캐릭터가 다르다. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOGc( kMissionInfo.m_cCharType )
                    << BUILD_LOGc( cCharType_ ) << END_LOG;
                continue;
            }

            nCharLv = pkCharInfo->m_dwLevel;
        }

        if ( kMissionInfo.m_prLevelRage.first >= 0 &&
            kMissionInfo.m_prLevelRage.first > (int)pkCharInfo->m_dwLevel ) {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( pkCharInfo->m_dwLevel ) << END_LOG;
                continue;
        }

        if ( kMissionInfo.m_prLevelRage.second >= 0 &&
            kMissionInfo.m_prLevelRage.second < (int)pkCharInfo->m_dwLevel ) {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( pkCharInfo->m_dwLevel ) << END_LOG;
                continue;
        }

        std::vector<int>::iterator vit2;
        for ( vit2 = kMissionInfo.m_vecSubmission.begin() ; vit2 != kMissionInfo.m_vecSubmission.end() ; ++vit2 ) {
            KSubMission kSubMission;
            if ( !GetSubMission( kSubMission, *vit2 ) ) {
                START_LOG( cerr, L"등록된 미션의 서브미션을 찾는중 서브미션 리스트에 존재하지 않음. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( *vit2 ) << END_LOG;
                continue;
            }

            // 몬스터 드랍은 다른 루트에서 처리됨.
            if ( kSubMission.m_sActionInfo.m_bMonDrop ) {
                continue;
            }

            // 아이템 드랍은 다른 루트에서 처리
            if ( kSubMission.m_sCompletionInfo.m_nItemID > 0 ) {
                continue;
            }

            // kill 카운트 조건 만족하는가?
            int nRemaindCount = kSubMission.m_sCompletionInfo.m_nCount;
            nRemaindCount -= vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID];
            nRemaindCount = std::max<int>( nRemaindCount, 0 );

            if ( nRemaindCount <= 0 ) {
                continue;
            }

            // 캐릭터 레벨조건 체크
            if ( kSubMission.m_sCompletionInfo.m_nCharLv > 0 ) {

                if ( nCharLv < kSubMission.m_sCompletionInfo.m_nCharLv ) {
                    continue;
                }

                int nAddCount = std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID] += nAddCount;
                nRemaindCount -= nAddCount;

                START_LOG( clog, L" 레벨 달성으로 미션 카운트 증가. Name : " << kUser_.GetName()
                    << L", Missin ID : " << kMissionInfo.m_dwMissionID
                    << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                    << L", Count : " << kSubMission.m_sActionInfo.m_nCount ) << END_LOG;
                continue;
            }

            std::set<int>::iterator sit;
            for ( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit ) {

                sCondition condition;
                if ( !GetCondition( *sit, condition ) ) {
                    START_LOG( cerr, L"해당 서브 미션의 컨디션을 가져 오지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( *sit ) << END_LOG;
                    continue;
                }

				//-----------------------
				// 0이면 무조껀 오케이
				// 1이면 솔로만 오케이
				// 2이상 이면 해당 유저 이상만 체크
				if ( ( condition.m_nSolo == 1 && nStartNum_ != 1  ) ||
					( condition.m_nSolo > 1 && nStartNum_ < condition.m_nSolo ) )
				{
                    START_LOG( clog, L"솔로로 플레이 해야 하는데.... Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nSolo )
                        << BUILD_LOG( nStartNum_ ) << END_LOG;
                    continue;
                }

                // 아래처럼 바꿔서는 그리 좋지 못하다는 것이 결론
                if ( false == CheckCondition( condition, false, kMissionInfo.m_dwMissionID, kSubMission.m_dwSubmissionID, kUser_.GetName() ) ) {
                    continue;
                }

                // 몬스터킬 체크 안함
                if ( condition.m_nMonsterKill >= 0 ) {
                    continue;
                }

                // 스테이지 체크 안함
                if ( condition.m_nStageID >= 0 ) {
                    continue;
                }
                // 클리어 시간 체크 안함
                if ( condition.m_nElapsedTime > 0 ) {
                    continue;
                }
                // 미구현
                if ( condition.m_nExLife >= 0 ) {
                    continue;
                }
                // 사용목숨수 체크 안함
                if ( condition.m_nDie >= 0 ) {
                    continue;
                }
                // 유저 킬만 처리
                if ( condition.m_nKill < 0 ) {
                    continue;
                }

                if ( condition.m_nModeID >= 0 && nModeID_ != condition.m_nModeID ) {
                    START_LOG( clog, L" Mode 가 맞지 않음 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                // 난이도 범위 체크 안함
                if ( condition.m_prDifficultRage.first >= 0 || condition.m_prDifficultRage.second >= 0 ) {
                    continue;
                }

                // 맵 체크
                if ( condition.m_nMapID >= 0 && nMapID_ != condition.m_nMapID ) {
                    START_LOG( clog, L"맵이 올바르지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nMapID )
                        << BUILD_LOG( nMapID_ ) << END_LOG;
                    continue;
                }

                // 몬스터 레벨 체크 안함.
                if ( condition.m_prMonLevelRange.first >= 0 || condition.m_prMonLevelRange.second >= 0 ) {
                    continue;
                }

                // 아이템을 줘야 하는 경우는 다른 루트에서 체크됨.
                if( kSubMission.m_sActionInfo.m_nItemID > 0 ) {
                    continue;
                }

                // 카운트를 올려야 하는경우.
                std::map<int,int>::iterator mit;
                mit = vit->m_mapCompletionInfo.find( (int)kSubMission.m_dwSubmissionID );
                if ( mit == vit->m_mapCompletionInfo.end() ) {
                    START_LOG( cerr, L"해당 유저가 서브 미션 완료 목록을 가지고 있지 않다. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID ) << END_LOG;
                    continue;
                }

                mit->second += std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );
                nRemaindCount -= std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                START_LOG( clog, L" 유저킬 미션 카운트 증가. Name : " << kUser_.GetName()
                    << L", Missin ID : " << kMissionInfo.m_dwMissionID
                    << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                    << L", Count : " << (kSubMission.m_sActionInfo.m_nCount ) ) << END_LOG;

            }// End Condition
        }// End Submission
    }// End MissionSlot

    START_LOG( clog, L"PvP킬 미션처리 완료 후 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();
}

void KMissionManager::ModeSubmission( IN KUser& kUser_, IN const int& nModeID_, IN const int& nDifficulty_, IN const int& nMapID_ )
{
    START_LOG( clog, L"게임모드 미션처리전 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();

    if ( kUser_.m_vecMissionSlot.empty() ) {
        return;
    }

    std::vector<KMissionData>::iterator vit;
    for ( vit = kUser_.m_vecMissionSlot.begin() ; vit != kUser_.m_vecMissionSlot.end() ; ++vit ) {
        KMissionInfo kMissionInfo;
        if ( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) ) {
            START_LOG( cerr, L" 등록된 미션이 미션 스크립트에 없음. Name : " << kUser_.GetName() )
                << BUILD_LOG( vit->m_dwMissionID ) << END_LOG;
            continue;
        }

        std::vector<int>::iterator vit2;
        for ( vit2 = kMissionInfo.m_vecSubmission.begin() ; vit2 != kMissionInfo.m_vecSubmission.end() ; ++vit2 ) {
            KSubMission kSubMission;
            if ( !GetSubMission( kSubMission, *vit2 ) ) {
                START_LOG( cerr, L"등록된 미션의 서브미션을 찾는중 서브미션 리스트에 존재하지 않음. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( *vit2 ) << END_LOG;
                continue;
            }

            // 몬스터 드랍은 다른 루트에서 처리됨.
            if ( kSubMission.m_sActionInfo.m_bMonDrop ) {
                continue;
            }

            // 아이템 드랍은 다른 루트에서 처리
            if ( kSubMission.m_sCompletionInfo.m_nItemID > 0 ) {
                continue;
            }

            // kill 카운트 조건 만족하는가?
            int nRemaindCount = kSubMission.m_sCompletionInfo.m_nCount;
            nRemaindCount -= vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID];
            nRemaindCount = std::max<int>( nRemaindCount, 0 );

            if ( nRemaindCount <= 0 ) {
                continue;
            }

            if ( kSubMission.m_sCompletionInfo.m_nCharLv > 0 ) {
                continue;
            }

            std::set<int>::iterator sit;
            for ( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit ) {

                sCondition condition;
                if ( !GetCondition( *sit, condition ) ) {
                    START_LOG( cerr, L"해당 서브 미션의 컨디션을 가져 오지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( *sit ) << END_LOG;
                    continue;
                }

                // 몬스터킬 체크 안함
                if ( condition.m_nMonsterKill >= 0 ) {
                    continue;
                }
                // 스테이지 체크 안함
                if ( condition.m_nStageID >= 0 ) {
                    continue;
                }
                // 클리어 시간 체크 안함
                if ( condition.m_nElapsedTime > 0 ) {
                    continue;
                }
                // 미구현
                if ( condition.m_nExLife >= 0 ) {
                    continue;
                }
                // 사용목숨수 체크 안함
                if ( condition.m_nDie >= 0 ) {
                    continue;
                }
                // 유저 킬 체크 안함
                if ( condition.m_nKill >= 0 ) {
                    continue;
                }
                // 승리조건 체크안함
                if ( condition.m_bWin ) {
                    continue;
                }
                // 몬스터 레벨 체크 안함.
                if ( condition.m_prMonLevelRange.first >= 0 || condition.m_prMonLevelRange.second >= 0 ) {
                    continue;
                }

                // Mode 체크
                if ( condition.m_nModeID >= 0 && nModeID_ != condition.m_nModeID ) {
                    START_LOG( clog, L" Mode 가 맞지 않음 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                // 난이도 체크
                if ( condition.m_prDifficultRage.first >= 0 &&
                    condition.m_prDifficultRage.first > nDifficulty_ ) {
                        START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                            << BUILD_LOG( kMissionInfo.m_dwMissionID )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_prDifficultRage.first )
                            << BUILD_LOG( nDifficulty_ ) << END_LOG;
                        continue;
                }
                if ( condition.m_prDifficultRage.second >= 0 &&
                    condition.m_prDifficultRage.second < nDifficulty_ ) {
                        START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                            << BUILD_LOG( kMissionInfo.m_dwMissionID )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_prDifficultRage.second )
                            << BUILD_LOG( nDifficulty_ ) << END_LOG;
                        continue;
                }

                // 맵 체크
                if ( condition.m_nMapID >= 0 && nMapID_ != condition.m_nMapID ) {
                    START_LOG( clog, L"맵이 올바르지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nMapID )
                        << BUILD_LOG( nMapID_ ) << END_LOG;
                    continue;
                }

                // 아이템을 줘야 하는 경우는 다른 루트에서 체크됨.
                if( kSubMission.m_sActionInfo.m_nItemID > 0 ) {
                    continue;
                }

                // 카운트를 올려야 하는경우.
                std::map<int,int>::iterator mit;
                mit = vit->m_mapCompletionInfo.find( (int)kSubMission.m_dwSubmissionID );
                if ( mit == vit->m_mapCompletionInfo.end() ) {
                    START_LOG( cerr, L"해당 유저가 서브 미션 완료 목록을 가지고 있지 않다. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID ) << END_LOG;
                    continue;
                }

                mit->second += std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );
                nRemaindCount -= std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                START_LOG( clog, L"게임모드 미션 카운트 증가. Name : " << kUser_.GetName()
                    << L", Missin ID : " << kMissionInfo.m_dwMissionID
                    << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                    << L", Count : " << (kSubMission.m_sActionInfo.m_nCount ) ) << END_LOG;

            }// End Condition
        }// End Submission
    }// End MissionSlot

    START_LOG( clog, L"게임모드 미션처리 완료 후 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();
}


bool KMissionManager::IsEnableDelete( IN const DWORD dwMissionID )
{
    std::map< DWORD, KMissionInfo >::iterator mit;
    mit = m_mapMissionInfo.find( dwMissionID );
    if( mit == m_mapMissionInfo.end() )
    {
        START_LOG( cerr, L"등록되지 않은 미션 목록 요청!!" )
            << BUILD_LOG( dwMissionID ) << END_LOG;
        return false;
    }    
    
    return mit->second.m_bEnableDelete;
}

bool KMissionManager::CheckTypeCondition( IN const std::wstring& strName_, IN const DWORD& dwMissionID_, IN const DWORD dwSubMissionID_, IN const sCondition& condition_, IN const KUserControlInfo& kUserControlInfoData_, IN std::map<int,int>& mapPositionUseCount_ )
{
    if( condition_.m_nType < 0 ) {
        return true;
    }

    if( condition_.m_nType == sCondition::TYPE_RANK ) {
        if( condition_.m_nValue > kUserControlInfoData_.m_nRank ) {
            START_LOG( clog, L" 랭크 등급이 낮음. Name : " << strName_ )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( dwSubMissionID_ )
                << BUILD_LOG( condition_.m_nValue )
                << BUILD_LOG( kUserControlInfoData_.m_nRank )
                << END_LOG;
            return false;
        } else {
            return true;
        }
    }
    else if( condition_.m_nType == sCondition::TYPE_BEATTACK ) {
        if( condition_.m_nValue < kUserControlInfoData_.m_nBeAttackedCount ) {
            START_LOG( clog, L" 피격 횟수가 높음. Name : " << strName_ )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( dwSubMissionID_ )
                << BUILD_LOG( condition_.m_nValue )
                << BUILD_LOG( kUserControlInfoData_.m_nBeAttackedCount )
                << END_LOG;
            return false;
        } else {
            return true;
        }
    } else if( condition_.m_nType == sCondition::TYPE_COMBO ) {
        if( condition_.m_nValue > kUserControlInfoData_.m_nComboCount ) {
            START_LOG( clog, L" 콤보 횟수가 낮음. Name : " << strName_ )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( dwSubMissionID_ )
                << BUILD_LOG( condition_.m_nValue )
                << BUILD_LOG( kUserControlInfoData_.m_nComboCount )
                << END_LOG;
            return false;
        } else {
            return true;
        }
    } else if( condition_.m_nType == sCondition::TYPE_BACK_ATTACK ) {
        if( condition_.m_nValue > kUserControlInfoData_.m_nBackAttackCount ) {
            START_LOG( clog, L" 백어택 횟수가 낮음. Name : " << strName_ )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( dwSubMissionID_ )
                << BUILD_LOG( condition_.m_nValue )
                << BUILD_LOG( kUserControlInfoData_.m_nBackAttackCount )
                << END_LOG;
            return false;
        } else {
            return true;
        }
    } else if( condition_.m_nType == sCondition::TYPE_USE_POTION ) {
        std::map<int, int>::const_iterator cmit;
        cmit = kUserControlInfoData_.m_mapUsePositionCount.find( condition_.m_prValue.first );
        if( cmit == kUserControlInfoData_.m_mapUseSkillCount.end() ) {
            return false;
        }

        if( condition_.m_prValue.second < cmit->second ) {
            START_LOG( clog, L" 포션 사용횟수가 높음. Name : " << strName_ )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( dwSubMissionID_ )
                << BUILD_LOG( condition_.m_prValue.first )
                << BUILD_LOG( condition_.m_prValue.second )
                << BUILD_LOG( kUserControlInfoData_.m_mapUsePositionCount.size() )
                << END_LOG;
            return false;
        } else {
            return true;
        }
    } else if( condition_.m_nType == sCondition::TYPE_SKILL_COUNT ) {
        std::map<int, int>::const_iterator cmit;
        cmit = kUserControlInfoData_.m_mapUseSkillCount.find( condition_.m_prValue.first );
        if( cmit == kUserControlInfoData_.m_mapUseSkillCount.end() ) {
            return false;
        }

        if( condition_.m_prValue.second > cmit->second ) {
            START_LOG( clog, L" 스킬 사용횟수가 낮음. Name : " << strName_ )
                << BUILD_LOG( dwMissionID_ )
                << BUILD_LOG( dwSubMissionID_ )
                << BUILD_LOG( condition_.m_prValue.first )
                << BUILD_LOG( condition_.m_prValue.second )
                << BUILD_LOG( kUserControlInfoData_.m_mapUseSkillCount.size() )
                << END_LOG;
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

bool KMissionManager::CheckControlMission( IN OUT KUser& kUser_, IN const KGameResultIn& kGameResultIn_, IN const int& nModeID_, IN const int& nDifficulty_ )
{
    START_LOG( clog, L"게임중 나가기 처리전 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();
    // 플레이한 캐릭터 정보를 따로 담아 둔다.
    std::vector< char > vecCharacter;
    std::wstringstream stmChar;
    {
        std::vector< KCharacterExpInfo >::const_iterator vitCh;
        for ( vitCh = kGameResultIn_.m_vecCharExp.begin() ; vitCh != kGameResultIn_.m_vecCharExp.end() ; ++vitCh ) {
            stmChar << static_cast<int>(vitCh->m_cCharType) << L"(" << vitCh->m_nBaseExp << L") ";
            vecCharacter.push_back( vitCh->m_cCharType );
        }
    }

    if ( kUser_.m_vecMissionSlot.empty() ) {
        return false;
    }

    std::vector<KMissionData>::iterator vit;
    for ( vit = kUser_.m_vecMissionSlot.begin() ; vit != kUser_.m_vecMissionSlot.end() ; ++vit ) {
        KMissionInfo kMissionInfo;
        if ( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) ) {
            START_LOG( cerr, L" 등록된 미션이 미션 스크립트에 없음. Name : " << kUser_.GetName() )
                << BUILD_LOG( vit->m_dwMissionID ) << END_LOG;
            continue;
        }

        int nCharLevel = -1;

        // 캐릭터 확인
        if ( kMissionInfo.m_cCharType >= 0 ) {
            if ( std::find( vecCharacter.begin(), vecCharacter.end(), kMissionInfo.m_cCharType ) == vecCharacter.end() ) {
                continue;
            }

            const KCharacterInfo* pkCharInfo = kUser_.GetCharacterInfo( kMissionInfo.m_cCharType );
            if( !pkCharInfo ) {
                continue;
            }
            nCharLevel = static_cast<int>( pkCharInfo->m_dwLevel );

            if ( kMissionInfo.m_prLevelRage.first >= 0 &&
                kMissionInfo.m_prLevelRage.first > nCharLevel ) {
                    START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                        << BUILD_LOG( nCharLevel ) << END_LOG;
                    continue;
            }

            if ( kMissionInfo.m_prLevelRage.second >= 0 &&
                kMissionInfo.m_prLevelRage.second < nCharLevel ) {
                    START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                        << BUILD_LOG( nCharLevel ) << END_LOG;
                    continue;
            }
        }

        std::vector<int>::iterator vit2;
        for ( vit2 = kMissionInfo.m_vecSubmission.begin() ; vit2 != kMissionInfo.m_vecSubmission.end() ; ++vit2 ) {
            KSubMission kSubMission;
            if ( !GetSubMission( kSubMission, *vit2 ) ) {
                START_LOG( cerr, L"등록된 미션의 서브미션을 찾는중 서브미션 리스트에 존재하지 않음. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( *vit2 ) << END_LOG;
                continue;
            }

            // 몬스터 드랍은 다른 루트에서 처리됨.
            if ( kSubMission.m_sActionInfo.m_bMonDrop ) {
                continue;
            }

            // 아이템 드랍은 다른 루트에서 처리
            if ( kSubMission.m_sCompletionInfo.m_nItemID > 0 ) {
                continue;
            }

            // 몬스터 kill 카운트 조건 만족하는가?
            int nRemaindCount = kSubMission.m_sCompletionInfo.m_nCount;
            nRemaindCount -= vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID];
            nRemaindCount = std::max<int>( nRemaindCount, 0 );

            if ( nRemaindCount <= 0 ) {
                continue;
            }

            std::set<int>::iterator sit;
            for ( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit ) {

                sCondition condition;
                if ( !GetCondition( *sit, condition ) ) {
                    START_LOG( cerr, L"해당 서브 미션의 컨디션을 가져 오지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( *sit ) << END_LOG;
                    continue;
                }

                // 스테이지 체크 안함
                if ( condition.m_nStageID >= 0 ) {
                    continue;
                }
                // 클리어 시간 체크 안함
                if ( condition.m_nElapsedTime > 0 ) {
                    continue;
                }
                // 미구현
                if ( condition.m_nExLife >= 0 ) {
                    continue;
                }
                // 사용목숨수 체크 안함
                if ( condition.m_nDie >= 0 ) {
                    continue;
                }
                // 유저 킬 체크 안함
                if ( condition.m_nKill >= 0 ) {
                    continue;
                }

                if ( condition.m_nModeID >= 0 && nModeID_ != condition.m_nModeID ) {
                    START_LOG( clog, L" Mode 가 맞지 않음 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                if( condition.m_prDifficultRage.first >= 0 &&
                    condition.m_prDifficultRage.first > nDifficulty_ )
                {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_prDifficultRage.first )
                        << BUILD_LOG( nDifficulty_ ) << END_LOG;
                    continue;
                }

                if( condition.m_prDifficultRage.second >= 0 &&
                    condition.m_prDifficultRage.second < nDifficulty_ )
                {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_prDifficultRage.second )
                        << BUILD_LOG( nDifficulty_ ) << END_LOG;
                    continue;
                }

                // 던전에서 맵 체크 안함.
                if ( condition.m_nMapID >= 0 ) {
                    continue;
                }

                // 아이템을 줘야 하는 경우는 다른 루트에서 체크됨.
                if( kSubMission.m_sActionInfo.m_nItemID > 0 ) {
                    continue;
                }

                // 조건 Type이 설정되지 않았을 경우 넘어감.
                if( condition.m_nType < 0 || 
                    ( condition.m_nType != sCondition::TYPE_COMBO &&
                        condition.m_nType != sCondition::TYPE_BACK_ATTACK ) ) {
                    continue;
                }

                if( condition.m_nType == sCondition::TYPE_COMBO ) {
                    if( condition.m_nValue > kGameResultIn_.m_kUserControlInfoData.m_nComboCount ) {
                        START_LOG( clog, L" 콤보 횟수가 낮음. Name : " << kUser_.GetName() )
                            << BUILD_LOG( kMissionInfo.m_dwMissionID )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nValue )
                            << BUILD_LOG( kGameResultIn_.m_kUserControlInfoData.m_nComboCount )
                            << END_LOG;
                        continue;
                    }
                }

                if( condition.m_nType == sCondition::TYPE_BACK_ATTACK ) {
                    if( condition.m_nValue > kGameResultIn_.m_kUserControlInfoData.m_nBackAttackCount ) {
                        START_LOG( clog, L" 백어택 횟수가 낮음. Name : " << kUser_.GetName() )
                            << BUILD_LOG( kMissionInfo.m_dwMissionID )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID )
                            << BUILD_LOG( condition.m_nValue )
                            << BUILD_LOG( kGameResultIn_.m_kUserControlInfoData.m_nBackAttackCount )
                            << END_LOG;
                        continue;
                    }
                } 

                // 카운트를 올려야 하는경우.
                std::map<int,int>::iterator mit;
                mit = vit->m_mapCompletionInfo.find( (int)kSubMission.m_dwSubmissionID );
                if ( mit == vit->m_mapCompletionInfo.end() ) {
                    START_LOG( cerr, L"해당 유저가 서브 미션 완료 목록을 가지고 있지 않다. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID ) << END_LOG;
                    continue;
                }

                mit->second += std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );
                nRemaindCount -= std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );

                START_LOG( clog, L" 타입별 카운트 증가. Name : " << kUser_.GetName()
                    << L", Missin ID : " << kMissionInfo.m_dwMissionID
                    << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                    << L", Count : " << (kSubMission.m_sActionInfo.m_nCount ) ) << END_LOG;


            }// End Condition
        }// End Submission
    }// End MissionSlot

    START_LOG( clog, L"게임중 나갈경우 미션처리 완료 후 미션 슬롯"  ) << END_LOG;
    kUser_.DumpMissionSlot();

    return true;
}

bool KMissionManager::GetMissionInfoListByDungeonID( OUT std::vector< KMissionInfo >& vecMissionInfo_, IN const int& nDungeonID_ )
{
    KLocker lock( m_csMissions );

    std::map< DWORD, KMissionInfo >::iterator mit;
    KMissionInfo kMissionInfo;
    vecMissionInfo_.clear();

    // 던전 ID에 해당 하는 미션 리스트를 얻는다 (노멀, 에픽)
    for ( mit = m_mapMissionInfo.begin(); mit != m_mapMissionInfo.end(); ++mit ) {
        if( nDungeonID_ == mit->second.m_nDungeonID ) {
            kMissionInfo = mit->second;
            vecMissionInfo_.push_back( kMissionInfo );
        }
    }
    
    if ( true == vecMissionInfo_.empty() ) {
        START_LOG( cwarn, L"GetMissionInfoByDungeonID() 던전ID에 해당하는 미션 정보가 없음" ) << BUILD_LOG( nDungeonID_ ) << END_LOG;
        return false;
    }

    return true;
}