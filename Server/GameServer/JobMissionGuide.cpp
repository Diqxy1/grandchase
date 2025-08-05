#include "JobMissionGuide.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KJobMissionGuide );
NiImplementRootRTTI( KJobMissionGuide );
ImplOstmOperatorW2A( KJobMissionGuide );

KJobMissionGuide::KJobMissionGuide(void)
: m_bUseJobMissionGuide( false )
{
    m_mapJobMissionInfo.clear();
}

KJobMissionGuide::~KJobMissionGuide(void)
{
}

ImplToStringW( KJobMissionGuide )
{
    KLocker lock( m_csJobMissionInfo );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapJobMissionInfo.size() );
}

bool KJobMissionGuide::LoadScript()
{
    return _LoadScript(std::string());
}

bool KJobMissionGuide::_LoadScript( OUT std::string& strScript )
{
    KLuaManager kLuaMng;
    std::map< PAIR_CHAR_INT, KJobMissionInfo > mapJobMissionInfo;
    mapJobMissionInfo.clear();
    int nUseJobMissionGuide = 0;

    _JIF( kLuaMng.DoFile( "InitJobMissionGuide.lua") == S_OK, return false );
    _JIF( kLuaMng.GetValue( "USE_JOB_MISSION_GUIDE", nUseJobMissionGuide ) == S_OK, return false );
    _JIF( LoadMissionGuideTable( kLuaMng, std::string( "MissionGuide" ), mapJobMissionInfo ), return false );

    {
        KLocker lock( m_csJobMissionInfo );

        m_bUseJobMissionGuide = ( nUseJobMissionGuide == 1 ) ? true : false;
        m_mapJobMissionInfo.swap( mapJobMissionInfo );
    }
    START_LOG( cerr, L"전직 미션 가이드 스크립트 로드 완료." ) << END_LOG;

    return true;
}

bool KJobMissionGuide::LoadMissionGuideTable( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_CHAR_INT, KJobMissionInfo >& mapJobMissionInfo_ )
{
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        //{ CharID, Level, GP Mission ID, Use GP Mission, Cash Mission ID, Use Cash Mission },
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        KJobMissionInfo kJobMissionInfo;
        int nCharType = -1;
        int nLevel = -1;
        int nUseGPMission = 0;
        int nUseCashMission = 0;

        _JIF( kLuaMng_.GetValue( 1, nCharType ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nLevel ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kJobMissionInfo.m_dwGPMissionID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, nUseGPMission ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 5, kJobMissionInfo.m_dwCashMissionID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 6, nUseCashMission ) == S_OK, return false );

        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( nCharType < 0 || nLevel < 0 )
        {
            START_LOG( cerr, L"캐릭터 타입 또는 레벨이 음수임.. CharType: " << nCharType << ", Level: " << nLevel ) << END_LOG;
            return false;
        }
        kJobMissionInfo.m_bUseGPMission = ( nUseGPMission == 1 ) ? true : false;
        kJobMissionInfo.m_bUseCashMission = ( nUseCashMission == 1 ) ? true : false;

        PAIR_CHAR_INT prKey =  std::make_pair( static_cast< char >( nCharType ), nLevel );
        mapJobMissionInfo_.insert( std::make_pair( prKey, kJobMissionInfo ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KJobMissionGuide::IsRun()
{
    return m_bUseJobMissionGuide;
}

bool KJobMissionGuide::GetJobMissionGuide( IN const PAIR_CHAR_INT& prKey_, OUT KEVENT_JOB_MISSION_GUIDE_NOT& kPacket_ )
{
    if ( !m_bUseJobMissionGuide ) {
        return false;
    }

    std::map< PAIR_CHAR_INT, KJobMissionInfo >::iterator mit;
    mit = m_mapJobMissionInfo.find( prKey_ );
    if ( mit == m_mapJobMissionInfo.end() ) {
        START_LOG(  clog, L"스크립트에 설정된 (캐릭터, 레벨) 정보가 없습니다: Char: " << prKey_.first << ", Level: " << prKey_.second ) << END_LOG;
        return false;
    }

    if ( mit->second.m_bUseGPMission ) {
        kPacket_.m_dwGPMissionID = mit->second.m_dwGPMissionID;
    }
    else {
        kPacket_.m_dwGPMissionID = -1;
    }
    if ( mit->second.m_bUseCashMission ) {
        kPacket_.m_dwCashMissionID = mit->second.m_dwCashMissionID;
    }
    else {
        kPacket_.m_dwCashMissionID = -1;
    }

    START_LOG( clog, L"패킷에 전직미션 가이드 정보 쓰기" )
        << BUILD_LOG( static_cast< int >( prKey_.first ) )
        << BUILD_LOG( prKey_.second )
        << BUILD_LOG( kPacket_.m_dwGPMissionID )
        << BUILD_LOG( kPacket_.m_dwCashMissionID )
        << END_LOG;

    return true;
}
