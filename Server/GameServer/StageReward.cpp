#include "StageReward.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include <kncutil.h>
#include "Log4.h"

ImplementSingleton( KStageReward );
ImplOstmOperatorW2A( KStageReward );
NiImplementRootRTTI( KStageReward );

KStageReward::KStageReward(void)
{
    m_vecStageRewardInfoList.clear();
}

KStageReward::~KStageReward(void)
{
}

ImplToStringW( KStageReward )
{
    KLocker lock( m_csStageReward );
    return START_TOSTRINGW << TOSTRINGW( m_vecStageRewardInfoList.size() );
}

bool KStageReward::LoadScript()
{
    return _LoadScript(std::string());
}

bool KStageReward::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    KStageRewardInfo StageRewardInfo;
    std::vector<KStageRewardInfo> mapStageRewardInfo;
    const std::string strScriptName = "InitStageReward.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( std::string( "StageReward" ).c_str() ) == S_OK, return false );
    for( int i = 1; ;i++ )
    {
        if ( kLuaMng.BeginTable(i) != S_OK ) break;
        _JIF( LoadStageRewardInfo( kLuaMng, StageRewardInfo), break );
        mapStageRewardInfo.push_back( StageRewardInfo );
        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );
    START_LOG( clog, L"스테이지 보상 스크립트를 로드했습니다. " ) << BUILD_LOG( mapStageRewardInfo.size() ) << END_LOG;
    KLocker lock(m_csStageReward);
    m_vecStageRewardInfoList.swap( mapStageRewardInfo );
    return true;
}

bool KStageReward::LoadStageRewardInfo( IN KLuaManager& kLuaMng, OUT KStageRewardInfo& KStageRewardInfo_ )
{
    KStageRewardInfo StageRewardInfo;
    KStageRewardStage StageRewardStage;
    std::vector<KStageRewardStage> StageRewardStageVector;
    _JIF( kLuaMng.GetValue( "DungeonID", StageRewardInfo.m_nModeID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "Difficulty", StageRewardInfo.m_nDifficulty ) == S_OK, return false );
    for( int i = 1; ;i++ )
    {
        if ( kLuaMng.BeginTable(i) != S_OK ) break;
        _JIF( LoadStageRewardStage( kLuaMng, StageRewardStage ), break );
        StageRewardStageVector.push_back( StageRewardStage );
        _JIF( kLuaMng.EndTable() == S_OK, break );
    }
    StageRewardInfo.m_vecStageInfo.swap(StageRewardStageVector);
    KStageRewardInfo_ = StageRewardInfo;
    return true;
}

bool KStageReward::LoadStageRewardStage( IN KLuaManager& kLuaMng, OUT KStageRewardStage& KStageRewardStage_ )
{
    KStageRewardStage StageRewardStage;
    KDropItemInfo StageRewardItemList;
    std::vector<KDropItemInfo> vecStageRewardInfo;
    _JIF( kLuaMng.GetValue( "Stage", StageRewardStage.m_nStageNum ) == S_OK, return false );
    for( int i = 1; ;i++ )
    {
        if ( kLuaMng.BeginTable(i) != S_OK ) break;
        _JIF( LoadStageRewardStageItemList( kLuaMng, StageRewardItemList ), break);
        vecStageRewardInfo.push_back( StageRewardItemList );
        _JIF( kLuaMng.EndTable() == S_OK, break );
    }
    StageRewardStage.m_vecStageRewardInfo.swap( vecStageRewardInfo );
    KStageRewardStage_ = StageRewardStage;
    return true;
}

bool KStageReward::LoadStageRewardStageItemList( IN KLuaManager& kLuaMng, OUT KDropItemInfo& KStageRewardStageItemList_ )
{
    KDropItemInfo tempItemInfo;
    _JIF( kLuaMng.GetValue( 1, tempItemInfo.m_ItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 2, tempItemInfo.m_nDuration ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( 3, tempItemInfo.m_nPeriod ) == S_OK, return false );
    KStageRewardStageItemList_ = tempItemInfo;
    return true;
}

bool KStageReward::GetStageRewardInfo( IN const int& nGameMode, IN const int& nDifficulty, IN const int& nStage, OUT std::vector<KDropItemInfo>& kItemList )
{
    for( auto element : m_vecStageRewardInfoList )
    {
        if( element.m_nModeID == nGameMode && element.m_nDifficulty == nDifficulty )
        {
            for( auto inner_element : element.m_vecStageInfo )
            {
                if( inner_element.m_nStageNum == nStage )
                {
                    kItemList.swap( inner_element.m_vecStageRewardInfo );
                    return true;
                }
            }
        }
    }
    return false;
}