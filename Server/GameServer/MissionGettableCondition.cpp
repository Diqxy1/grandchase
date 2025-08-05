#include "MissionGettableCondition.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KMissionGettableCondition );
ImplOstmOperatorW2A( KMissionGettableCondition );
NiImplementRootRTTI( KMissionGettableCondition );

ImplToStringW( KMissionGettableCondition )
{
    KLocker lock( m_csMissionGettableCondition );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapGettableCondition.size() );
}

KMissionGettableCondition::KMissionGettableCondition(void)
{
    m_mapGettableCondition.clear();
}

KMissionGettableCondition::~KMissionGettableCondition(void)
{
}

bool KMissionGettableCondition::LoadScript()
{
    return _LoadScript(std::string());
}

bool KMissionGettableCondition::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitMissionGettableCondition.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScript_.c_str() ) == S_OK, return false );

    std::map< DWORD, DWORD > mapGettableCondition;
    mapGettableCondition.clear();

    _JIF( LoadGettableCondition( kLuaMng, std::string("GettableConditionInfo"), mapGettableCondition ), return false );

    {
        KLocker lock( m_csMissionGettableCondition );
        m_mapGettableCondition.swap( mapGettableCondition );
    }

    return true;
}

bool KMissionGettableCondition::LoadGettableCondition( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, DWORD >& mapCondition_ )
{
    mapCondition_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwMissionID = 0;
        DWORD dwPrevMissionID = 0;

        if( kLuaMng_.BeginTable( i ) != S_OK )
            break;

        _JIF( kLuaMng_.GetValue( 1, dwMissionID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, dwPrevMissionID ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        // insert
        if( !mapCondition_.insert( std::make_pair( dwMissionID, dwPrevMissionID ) ).second )
        {
            START_LOG( cerr, L"InitMissionGettableCondition 중복 된 미션 정보가 있음" )
                << BUILD_LOG( dwMissionID )
                << BUILD_LOG( dwPrevMissionID )
                << END_LOG;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"InitMissionGettableCondition 정보 로드 완료, Count : " << mapCondition_.size() ) << END_LOG;

    return true;
}

void KMissionGettableCondition::GetMissionGettableCondition( OUT std::map< DWORD, DWORD >& mapCondition_ )
{
    KLocker lock( m_csMissionGettableCondition );
    mapCondition_ = m_mapGettableCondition;
}

bool KMissionGettableCondition::MissionRegistEnableCheck( IN const DWORD dwMissionID_, IN std::vector< DWORD > vecFinishedMission_ )
{
    KLocker lock( m_csMissionGettableCondition );

    std::map< DWORD, DWORD >::iterator mitMissionRegInfo;
    std::vector< DWORD >::iterator vitFinishedMissionInfo;

    mitMissionRegInfo = m_mapGettableCondition.find( dwMissionID_ );
    if ( m_mapGettableCondition.end() == mitMissionRegInfo ) {
        START_LOG( cwarn, L"등록 하려는 미션이 스크립트에 설정 된 미션ID가 아님 MissionID : " << dwMissionID_ ) << END_LOG;
        return false;
    }

    // 조건이 0으로 등록 돼 있으면 그냥 패스 (시련의 숲같이 조건 필요 없는 미션들)
    if ( 0 != mitMissionRegInfo->second ) {
        // 조건으로 등록 된 미션을 클리어 한 상태인지 확인 (mitMissionRegInfo->second가 조건)
        vitFinishedMissionInfo = std::find( vecFinishedMission_.begin(), vecFinishedMission_.end(), mitMissionRegInfo->second );
        if ( vecFinishedMission_.end() == vitFinishedMissionInfo ) {
            return false; // 조건에 해당하는 미션을 클리어한 기록이 없다
        }
    }

    return true;
}