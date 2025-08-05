#include "User.h"
#include "Room.h"
#include "CollectionManager.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include <KncUtil.h>
#include "GSSimLayer.h"
#include <algorithm> // random_shuffle
#include "GameDrop.h" // sTriggerInfo
#include <boost/bind.hpp>
#include "GameServer.h"
#include "Log4.h"

ImplementSingleton( KCollectionManager );
NiImplementRootRTTI( KCollectionManager );

KCollectionManager::KCollectionManager(void)
:
uint32( 1, UINT_MAX ),
die(rng, uint32),
m_bBroadcastCheck(false)
{
    rng.seed( ::GetTickCount() ); // Seed 값 설정.
}


KCollectionManager::~KCollectionManager(void)
{
}

bool KCollectionManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCollectionManager::_LoadScript( OUT std::string& strScript_ ) // 상속상태에서 overriding 
{
    std::string strFileName( "InitCTMission.lua" );
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

    bool bBroadcastCheck = false;
    std::vector< DWORD >     vecTitleList;
    std::map< GCITEMID, GCITEMID > mapTitleOrder;
    _JIF( kLuaMng.BeginTable( "TitleList") == S_OK, return false );
    _JIF( kLuaMng.GetValue( "BroadcastCheck", bBroadcastCheck ) == S_OK, return false );
    _JIF( LoadTitleList( kLuaMng, vecTitleList ), return false );
    _JIF( LoadTitleOrder( kLuaMng, mapTitleOrder ), return false );
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    KLocker lock( m_csMissions );
    m_mapConditions.swap( mapConditions );
    m_mapSubmission.swap( mapSubmission );
    m_mapMissionInfo.swap( mapMissionInfo );
    m_bBroadcastCheck = bBroadcastCheck;
    m_vecTitleCollection.swap( vecTitleList );
    m_mapTitleOrder.swap( mapTitleOrder );

    START_LOG( cerr, L" InitCTMission 로드 완료. Size : " ) << END_LOG;

    return true;
}

bool KCollectionManager::LoadCondition( IN KLuaManager& kLuaMng_, OUT std::map< int, sCondition >& mapConditions_ )
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

bool KCollectionManager::LoadSubMission( IN KLuaManager& kLuaMng_, OUT std::map< int, KSubMission >& mapSubmission_ )
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

bool KCollectionManager::LoadMission( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, KMissionInfo >& mapMissionInfo_ )
{
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) == S_OK )
        {
            int nCharType;
            int nJobLv;
            int nNewChar;
            int nChangeJob;
            KMissionInfo missionInfo;
            _JIF( 0 == kLuaMng_.GetValue( "ID", missionInfo.m_dwMissionID ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "CharType", nCharType ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "Promotion", nJobLv ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "Unique", missionInfo.m_bUnique ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "Period", missionInfo.m_nPeriod), return false );
            _JIF( 0 == kLuaMng_.GetValue( "NewChar", nNewChar ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "ChangeJob", nChangeJob ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "GP", missionInfo.m_nGp), return false );
            _JIF( 0 == kLuaMng_.GetValue( "EXP", missionInfo.m_nExp ), return false );

            int nChainMissionID = -1;
            if( 0 != kLuaMng_.GetValue( "ChainMissionID", nChainMissionID ) ) // 칭호쪽 스크립트에는 없기 때문에.
                nChainMissionID = -1;

            missionInfo.m_dwChainMissionID = ( nChainMissionID > 0 ? (DWORD)(nChainMissionID) : 0 );

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
                        _JIF( 0 == kLuaMng_.GetValue( 1, prReward.m_ItemID )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 2, prReward.m_nCount )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 3, prReward.m_nPeriod ) , return false );
//                        assert( 0 == kLuaMng_.GetValue( 3, prReward.m_nPeriod ) );

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
                        _JIF( 0 == kLuaMng_.GetValue( 1, prReward.m_ItemID )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 2, prReward.m_nCount )  , return false );
                        _JIF( 0 == kLuaMng_.GetValue( 3, prReward.m_nPeriod ) , return false );
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

#include "StmOperator.h"

ImplOstmOperatorW2A( KCollectionManager );
ImplToStringW( KCollectionManager )
{
    KLocker lock( m_csMissions );

    START_TOSTRINGW
        << TOSTRINGW( m_mapConditions.size() )
        << TOSTRINGW( m_mapSubmission.size() )
        << TOSTRINGW( m_mapMissionInfo.size() );

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

bool KCollectionManager::GetMissionInfo( OUT KMissionInfo& kMissionInfo, IN const DWORD dwMissionID )
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

void KCollectionManager::DropSubMission( IN KUser& kUser_,
                                     IN KRoom& kRoom_,
                                     IN const KGameResultIn& gameResult_,
                                     IN const KEndGameReq& kReq_,
                                     OUT  std::vector<KDropItemInfo>& vecdrops_ )
{
    //START_LOG( clog, L"미션 처리전 미션 슬롯"  ) << END_LOG;
    //kUser_.DumpCollectionSlot();
    const KInDoorUserInfo* pkStartingInfo = kRoom_.FindInDoorUserInfo( kUser_.GetName() );
    JIF( pkStartingInfo != NULL ); // 유저 유효성 체크?

    // 플레이한 캐릭터 정보를 따로 담아 둔다.
    std::vector< char > vecCharacter;    
    {
        std::vector< KCharacterExpInfo >::const_iterator vitCh;
        for( vitCh = gameResult_.m_vecCharExp.begin() ; vitCh != gameResult_.m_vecCharExp.end() ; ++vitCh )
        {
            vecCharacter.push_back( vitCh->m_cCharType );
        }
    }

    if( kUser_.m_vecCollectionSlot.empty() )
        return;

    std::vector<KCollectionData>::iterator vit;
    for( vit = kUser_.m_vecCollectionSlot.begin() ; vit != kUser_.m_vecCollectionSlot.end() ; ++vit )
    {
        KMissionInfo kMissionInfo;
        if( !GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) )
        {
            START_LOG( cerr, L" 등록된 미션이 미션 스크립트에 없음. Name : " << kUser_.GetName() )
                << BUILD_LOG( vit->m_dwMissionID ) << END_LOG;
            continue;
        }

        // 캐릭터 확인
        if( kMissionInfo.m_cCharType >= 0 )
        {
            if( std::find( vecCharacter.begin(), vecCharacter.end(), kMissionInfo.m_cCharType ) == vecCharacter.end() )
                continue;

            const KCharacterInfo* pkCharInfo = kUser_.GetCharacterInfo( kMissionInfo.m_cCharType );
            JIF( pkCharInfo );
            int nCharLevel = static_cast<int>(pkCharInfo->m_dwLevel);

            if( kMissionInfo.m_prLevelRage.first >= 0 &&
                kMissionInfo.m_prLevelRage.first > nCharLevel )
            {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( nCharLevel ) << END_LOG;
                continue;
            }

            if( kMissionInfo.m_prLevelRage.second >= 0 &&
                kMissionInfo.m_prLevelRage.second < nCharLevel )
            {
                START_LOG( clog, L"레벨 범위를 만족하지 못함. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( kMissionInfo.m_prLevelRage.first )
                    << BUILD_LOG( nCharLevel ) << END_LOG;
                continue;
            }
        }

        std::vector<int>::iterator vit2;
        for( vit2 = kMissionInfo.m_vecSubmission.begin() ; vit2 != kMissionInfo.m_vecSubmission.end() ; ++vit2 )
        {
            KSubMission kSubMission;
            if( !GetSubMission( kSubMission, *vit2 ) )
            {
                START_LOG( cerr, L"등록된 미션의 서브미션을 찾는중 서브미션 리스트에 존재하지 않음. Name : " << kUser_.GetName() )
                    << BUILD_LOG( kMissionInfo.m_dwMissionID )
                    << BUILD_LOG( *vit2 ) << END_LOG;
                continue;
            }

            if( kSubMission.m_sActionInfo.m_bMonDrop )
                continue;

            // 충분한가?
            int nRemaindCount = kSubMission.m_sCompletionInfo.m_nCount;
            if( kSubMission.m_sCompletionInfo.m_nItemID > 0 ) // 아이템인 경우
            {                
                KItemPtr pkItem = kUser_.m_kInventory.FindDurationItemByItemID( kSubMission.m_sCompletionInfo.m_nItemID );
                if( pkItem )
                    nRemaindCount -= pkItem->m_nCount;
            }
            else // 카운트인경우
            {
                nRemaindCount -= vit->m_mapCompletionInfo[(int)kSubMission.m_dwSubmissionID];
            }

            nRemaindCount = std::max<int>( nRemaindCount, 0 );

            if( nRemaindCount <= 0 )
                continue;

            std::set<int>::iterator sit;
            for( sit = kSubMission.m_setConditions.begin() ; sit != kSubMission.m_setConditions.end() ; ++sit )
            {
                sCondition condition;
                if( !GetCondition( *sit, condition ) )
                {
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

                if( condition.m_bWin && !gameResult_.m_bWin ) // 여기에 TRUE 되어 있으면 않됨.
                {
                    START_LOG( clog, L"이기지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID ) << END_LOG;
                    continue;
                }

                // 킬 체크는 실시간으로 처리됨.
                if( condition.m_nMonsterKill >= 0 )
                {
                    continue;
                }

                if( condition.m_nStageID >= 0 &&
                    condition.m_nStageID != kReq_.m_iQuestEndingStage )
                {
                    START_LOG( clog, L"End Stage 번호가 틀림.. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( kReq_.m_iQuestEndingStage )
                        << BUILD_LOG( condition.m_nStageID ) << END_LOG;
                    continue;
                }

                if( condition.m_nElapsedTime > 0 && (kReq_.m_dwElapsedSec / 60.f) > condition.m_nElapsedTime )
                {
                    START_LOG( clog, L"너무 오래 플레이 했삼.. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nElapsedTime )
                        << BUILD_LOG( (kRoom_.GetLatestPlayTime()/(1000 * 60)) )
                        << BUILD_LOG( (kReq_.m_dwElapsedSec / 60) ) << END_LOG;
                    continue;
                }

                if( condition.m_nExLife >= 0 )
                {
                    START_LOG( clog, L"미구현 부분(정보 부족) ExLife가 있음." )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nExLife ) << END_LOG;
                    continue;
                }

                if( condition.m_nDie >= 0 && gameResult_.m_iNumDies > condition.m_nDie )
                {
                    START_LOG( clog, L"많이 죽었음.. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nID )
                        << BUILD_LOG( condition.m_nDie )
                        << BUILD_LOG( gameResult_.m_iNumDies ) << END_LOG;
                    continue;
                }

                // PvP킬은 실시간으로 처리됨.
                if( condition.m_nKill >= 0 )
                {
                    continue;
                }

                if( condition.m_nModeID >= 0 && kRoom_.m_iGameMode != condition.m_nModeID )
                {
                    START_LOG( clog, L" Mode 가 맞지 않음 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nModeID )
                        << BUILD_LOG( kRoom_.m_iGameMode ) << END_LOG;
                    continue;
                }

                if( condition.m_prDifficultRage.first >= 0 &&
                    condition.m_prDifficultRage.first > kRoom_.m_nDifficulty )
                {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOGc( condition.m_prDifficultRage.first )
                        << BUILD_LOG( kRoom_.m_nDifficulty ) << END_LOG;
                    continue;
                }

                if( condition.m_prDifficultRage.second >= 0 &&
                    condition.m_prDifficultRage.second < kRoom_.m_nDifficulty )
                {
                    START_LOG( clog, L"난이도 범위를 만족하지 못함 Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOGc( condition.m_prDifficultRage.second )
                        << BUILD_LOG( kRoom_.m_nDifficulty ) << END_LOG;
                    continue;
                }

                if( condition.m_nMapID >= 0 && kRoom_.m_iMapID != condition.m_nMapID )
                {
                    START_LOG( clog, L"맵이 올바르지 못함. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nMapID )
                        << BUILD_LOG( kRoom_.m_iMapID ) << END_LOG;
                    continue;
                }

                if( condition.m_fRatio < GetRatio() )
                {
                    START_LOG( clog, L"확률이 낮음. Name : " << kUser_.GetName() )
                        << BUILD_LOG( kMissionInfo.m_dwMissionID )
                        << BUILD_LOG( kSubMission.m_dwSubmissionID )
                        << BUILD_LOG( condition.m_nMapID )
                        << BUILD_LOG( condition.m_fRatio ) << END_LOG;
                    continue;
                }


                // 아이템을 줘야 하는 경우..
                if( kSubMission.m_sActionInfo.m_nItemID > 0 )
                {
                    KDropItemInfo kItemUnit;
                    kItemUnit.m_ItemID = (GCITEMID)kSubMission.m_sActionInfo.m_nItemID;
                    kItemUnit.m_nDuration = kSubMission.m_sActionInfo.m_nCount;
                    kItemUnit.m_nPeriod = -1;
                    vecdrops_.push_back( kItemUnit );

                    START_LOG( clog, L" 미션으로 아이템 얻음. Name : " << kUser_.GetName()
                        << L", Missin ID : " << kMissionInfo.m_dwMissionID
                        << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                        << L", ItemID : " << kItemUnit.m_ItemID
                        << L", Count : " << kItemUnit.m_nDuration )  << END_LOG;
                }
                else // 카운트를 올려야 하는경우.
                {
                    std::map<int,int>::iterator mit;
                    mit = vit->m_mapCompletionInfo.find( (int)kSubMission.m_dwSubmissionID );
                    if( mit == vit->m_mapCompletionInfo.end() )
                    {
                        START_LOG( cerr, L"해당 유저가 서브 미션 완료 목록을 가지고 있지 않다. Name : " << kUser_.GetName() )
                            << BUILD_LOG( kMissionInfo.m_dwMissionID )
                            << BUILD_LOG( kSubMission.m_dwSubmissionID ) << END_LOG;
                        continue;
                    }

                    int nCount = std::min<int>( nRemaindCount, kSubMission.m_sActionInfo.m_nCount );
                    if( nCount < 0 )
                    {
                        START_LOG( cerr, L"미션 Count값이 음수다. Name : " << kUser_.GetName() << L", Count : " << nCount ) << END_LOG;
                        nCount = 0;
                    }
                    mit->second += nCount;
                    nRemaindCount -= nCount;

                    START_LOG( clog, L" 미션으로 카운트 증가. Name : " << kUser_.GetName()
                        << L", Missin ID : " << kMissionInfo.m_dwMissionID
                        << L", SubMission ID : " << kSubMission.m_dwSubmissionID
                        << L", Count : " << kSubMission.m_sActionInfo.m_nCount << L" / " << nCount )  << END_LOG;
                }

            }
        }
    }

    //START_LOG( clog, L"컬렉션 미션 처리 완료 후 미션 슬롯"  ) << END_LOG;
    //kUser_.DumpCollectionSlot();
}
bool KCollectionManager::GetSubMission( OUT KSubMission& kSubMission_, IN const int nSubMissionID_ )
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

bool KCollectionManager::GetMissionData( OUT KCollectionData& kMissionData_, IN const DWORD dwMissionID_, IN bool bFillCount_/* = true*/ )
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
    
    return true;
}

void KCollectionManager::GetMissionIDList( OUT std::vector<DWORD>& vecMissionIDList_ )
{
   vecMissionIDList_.clear();

   std::map< DWORD, KMissionInfo >::iterator mit;
   for( mit = m_mapMissionInfo.begin(); mit != m_mapMissionInfo.end(); ++mit )
   {
        vecMissionIDList_.push_back( mit->first );
   }

   START_LOG( cerr, L"컬렉션 미션 ID 리스트(스크립트)" )
       << BUILD_LOG( vecMissionIDList_.size() ) << END_LOG;
}

bool KCollectionManager::GetCondition( IN const int nConditionID_, OUT sCondition& condition_ )
{
    KLocker lock( m_csMissions );
    std::map< int, sCondition >::iterator mit;
    mit = m_mapConditions.find( nConditionID_ );
    if( mit == m_mapConditions.end() )
        return false;

    condition_ = mit->second;
    return true;
}


bool KCollectionManager::CheckMissionComplete( IN const KCollectionData& kMissionData_, IN KItemContainer& kInventory_ )
{
    // 미션 완료 조건 체크 하고.
    // 삭제 아이템 리스트를 전달해준다.

    // 해당 미션이 존재하는지 체크
    KMissionInfo kMissionInfo;
    if( !GetMissionInfo( kMissionInfo, kMissionData_.m_dwMissionID ) )
    {
        START_LOG( cerr, L"해당 컬렉션 미션이 존재하지 않음." )
            << BUILD_LOG( kMissionData_.m_dwMissionID ) << END_LOG;
        return false;
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
            return false;
        }

        // 서브 미션 스크립트에서 기록된 필요 카운트가 올바른가?
        if( kSubMission.m_sCompletionInfo.m_nCount < 0 )
        {
            START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 수치가 음수이다." )
                << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                << L"SubMissionID : " << *vit << dbg::endl
                << L"Need Count : " << kSubMission.m_sCompletionInfo.m_nCount  << END_LOG;
            return false;
        }

        // 아이템을 모아야 한다면.
        if( kSubMission.m_sCompletionInfo.m_nItemID > 0 )
        {
            const KItemPtr pkItem = kInventory_.FindItemByItemID( kSubMission.m_sCompletionInfo.m_nItemID );

            //인벤토리에 요구한 아이템이 있는가?
            if( pkItem == NULL )
            {
                START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 아이템이 없음." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kSubMission.m_dwSubmissionID << dbg::endl
                    << L"ItemID : " <<  kSubMission.m_sCompletionInfo.m_nItemID  << END_LOG;
                return false;
            }

            //인벤토리에 요구한 아이템의 개수가 충분한가?
            if( pkItem->m_nCount < kSubMission.m_sCompletionInfo.m_nCount )
            {
                START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 아이템의 수량이 부족." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"ItemID : " <<  kSubMission.m_sCompletionInfo.m_nItemID << dbg::endl
                    << L"Need Count : " << kSubMission.m_sCompletionInfo.m_nCount  << END_LOG;
                return false;
            }

            // 충분하다면 해당 아이템의 필요 수량을 밖으로 보내자...^^...
            //KDurationItemInfo kDelItem;
            //kDelItem.m_dwGoodsID    = (DWORD)kSubMission.m_sCompletionInfo.m_nItemID;
            //kDelItem.m_dwGoodsUID   = vitInv->m_dwGoodsUID;
            //kDelItem.m_dwCurrentNum = (DWORD)kSubMission.m_sCompletionInfo.m_nCount;
            //vecDelDurationItem_.push_back( kDelItem );
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
                return false;
            }

            // 해당 서브 미션의 수치가 충분한가?
            if( kSubMission.m_sCompletionInfo.m_nCount > mit->second )
            {
                START_LOG( cerr, L"해당 서브 미션을 완료하기 위한 수치 부족." )
                    << L"MissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"SubMissionID : " << kMissionData_.m_dwMissionID << dbg::endl
                    << L"Current Count : " <<  kSubMission.m_sCompletionInfo.m_nCount << dbg::endl
                    << L"Need Count : " << kSubMission.m_sCompletionInfo.m_nCount  << END_LOG;
                return false;
            }
        }
    }

    return true;
}

bool KCollectionManager::LoadTitleList( IN KLuaManager& kLuaMng_, OUT std::vector<DWORD>& vecTitleList_ )
{
    if( kLuaMng_.BeginTable( "TitleList" ) == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            DWORD dwTitleID;
            if( 0 != kLuaMng_.GetValue( i, dwTitleID ) ) break;

            vecTitleList_.push_back( dwTitleID );
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    else
    {
        START_LOG( cerr, L"칭호 아이템 리스트를 읽지 못함." ) << END_LOG;
        return false;
    }

    //std::vector<DWORD>::iterator vit;
    //std::cout << L"칭호 리스트 : " << dbg::endl;
    //for( vit = vecTitleList_.begin(); vit != vecTitleList_.end(); ++vit )
    //{
    //    std::cout << *vit << " , ";
    //}
    //std::cout << "" << dbg::endl;

    return true;
}

bool KCollectionManager::LoadTitleOrder( IN KLuaManager& kLuaMng_, OUT std::map< GCITEMID, GCITEMID >& mapTitleOrder_ )
{
    if( kLuaMng_.BeginTable( "TitleOrder" ) == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng_.BeginTable( i ) != S_OK ) break;

            std::pair<GCITEMID,GCITEMID> prTitleItemID;
            if( kLuaMng_.GetValue( 1, prTitleItemID.first ) != S_OK ) break;
            if( kLuaMng_.GetValue( 2, prTitleItemID.second ) != S_OK ) break;
            _JIF( kLuaMng_.EndTable() == S_OK, return false );

            if( mapTitleOrder_.insert( prTitleItemID ).second == false )
            {
                START_LOG( cerr, L"중복된 칭호 조건 정보." )
                    << BUILD_LOG( prTitleItemID.first )
                    << BUILD_LOG( prTitleItemID.second ) << END_LOG;
                return false;
            }
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }
    else
    {
        START_LOG( cerr, L"칭호획득 우선순위 리스트를 읽지 못함." ) << END_LOG;
        return false;
    }

//    std::map< GCITEMID, GCITEMID >::iterator mit;
//    std::cout << L"칭호획득 우선순위 리스트 : " << std::endl;
//    for( mit = mapTitleOrder_.begin(); mit != mapTitleOrder_.end(); ++mit )
//    {
//        std::cout << L"{" << mit->first << L"," << mit->second << L"},";
//    }
//    std::cout << std::endl;

    return true;
}

void KCollectionManager::GetTitleList( OUT std::vector<DWORD>& vecTitleList_ )
{

    vecTitleList_ = m_vecTitleCollection;
}

bool KCollectionManager::IsTitleItem( const GCITEMID ItemID_ )
{
    std::vector<DWORD>::iterator vit;

    vit = std::find( m_vecTitleCollection.begin(), m_vecTitleCollection.end(), ItemID_ );
    
    if( vit != m_vecTitleCollection.end() ) // 넘어온 아이템 아이디가 칭호 아이템의 그것인가?
    {
        START_LOG( clog, L"이 아이템은 칭호 아이템입니다")
            << BUILD_LOG( *vit )
            << BUILD_LOG( ItemID_ ) << END_LOG;
        return true; // 네
    }
    return false; // 아니오
}

std::wstring KCollectionManager::HowGetTitle( IN std::wstring strKey_ )
{
    wchar_t szHowGetIt[255] = {0};

    GetPrivateProfileStringW( L"Title", strKey_.c_str(), L"", szHowGetIt, 255, SiKGameServer()->m_stringTable.GetCurrentFile().c_str() );
    std::wstring strHowGetIt(szHowGetIt);

    START_LOG( clog, L"어떻게 얻은 칭호인가 ")
        << BUILD_LOG( strHowGetIt ) << END_LOG;
    return strHowGetIt;
}

void KCollectionManager::CheckGetPrevTitle( IN const GCITEMID dwItemID_, OUT std::vector<GCITEMID>& vecPrevCTList_ )
{
    KLocker lock( m_csMissions );
    vecPrevCTList_.clear();
    GCITEMID dwPrevItemID = 0;
    std::map< GCITEMID, GCITEMID >::const_iterator cmit;

    if( m_mapTitleOrder.empty() )
    {
        START_LOG( clog, L"칭호 선행조건 체크 안함." ) << END_LOG;
        return;
    }

    //획득할 칭호Item의 선행조건 찾기.
    cmit = m_mapTitleOrder.find( dwItemID_ );
    if( cmit != m_mapTitleOrder.end() )
    {
        dwPrevItemID = cmit->second;    //선행조건 칭호ItemID를 넣어주자.
    }
    else
    {
        START_LOG( cwarn, L"선행조건이 등록되어 있지 않은 칭호. ItemID : " << dwItemID_ ) << END_LOG;
        dwPrevItemID = 0;
    }

    for(;;)
    {
        if( dwPrevItemID == 0 ) //0은 선행조건 없음.
            break;

        //획득하려는 칭호의 선행조건ItmeID를 넣어준다.
        vecPrevCTList_.push_back( dwPrevItemID );

        //테이블에서 선행Item의 선행조건을 찾는다.
        cmit = m_mapTitleOrder.find( dwPrevItemID );
        if( cmit != m_mapTitleOrder.end() )
        {
            dwPrevItemID = cmit->second;    //선행조건 칭호ItemID를 넣어주자.
        }
        else
        {
            START_LOG( cwarn, L"선행조건이 등록되어 있지 않은 칭호. ItemID : " << dwPrevItemID ) << END_LOG;
            break;
        }
    }

    std::sort( vecPrevCTList_.begin(), vecPrevCTList_.end() ); // 정렬

    START_LOG( clog, L"획득 요청한 칭호 이전 조건의 칭호ItemID리스트 받아감.")
        << BUILD_LOG( vecPrevCTList_.size() )
        << BUILD_LOG( dwItemID_ ) << END_LOG;

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        //std::clog << L"선행조건 칭호ItemID 리스트 (획득할 칭호ItemID:" << dwItemID_ << L") : ";
        //std::vector<GCITEMID>::iterator vit;
        //for( vit = vecPrevCTList_.begin() ; vit != vecPrevCTList_.end() ; ++vit )
        //{
        //    std::clog << *vit << L",";
        //}
        //std::clog << dbg::endl;
    }
}

float KCollectionManager::GetRatio()
{
    return GetRandomNum()/(float)(UINT_MAX);
}