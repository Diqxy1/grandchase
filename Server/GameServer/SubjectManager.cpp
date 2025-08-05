#include "SubjectManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KSubjectManager );
ImplOstmOperatorW2A( KSubjectManager );
NiImplementRootRTTI( KSubjectManager );

KSubjectManager::KSubjectManager(void)
{
    m_mapSubjectList.clear();
    m_mapSubjectInfo.clear();
}

KSubjectManager::~KSubjectManager(void)
{
}

ImplToStringW( KSubjectManager )
{
    KLocker lock( m_csSubject );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapSubjectList.size() )
        << TOSTRINGW( m_mapSubjectInfo.size() );
}

bool KSubjectManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSubjectManager::_LoadScript( OUT std::string& strScript_ )
{
    std::map< DWORD, KSubject > mapSubjectList;                                 // map< SubjectID, KSubject >
    std::map< PAIR_INT, std::map< DWORD, std::set< DWORD > > > mapSubjectInfo;  // map< ( Mode, Difficulty ), map< TriggerID, set< SubjectID > > >

    _JIF( LoadSubjectTemplate( mapSubjectList ), return false );
    _JIF( LoadSubjectInfo( mapSubjectInfo ), return false );

    {
        KLocker lock( m_csSubject );

        m_mapSubjectList.swap( mapSubjectList );
        m_mapSubjectInfo.swap( mapSubjectInfo );
    }

    START_LOG( cerr, L"Subject script loaded." ) << END_LOG;

    return true;
}

bool KSubjectManager::LoadSubjectTemplate( OUT std::map< DWORD, KSubject >& mapSubjectList_ )
{
    mapSubjectList_.clear();

    KLuaManager kLuaMng;

    _JIF( kLuaMng.DoFile( "Enum.stg" ) == S_OK, return false );
    _JIF( kLuaMng.DoFile( "SubjectTemplate.stg" ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "SubjectList" ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng.BeginTable( i ) != S_OK ) break;

        KSubject kSubject;

        _JIF( kLuaMng.GetValue( "ID", kSubject.m_dwSubjectID ) == S_OK, return false );

        // 클라이언트측의 요청으로 각 항목이 비어있는 경우도 정상처리 하도록 수정함
        kLuaMng.GetValue( "Exp", kSubject.m_nAddExp );
        kLuaMng.GetValue( "GP", kSubject.m_nAddGp );
        LoadDropList( kLuaMng, std::string( "Drop" ), kSubject.m_vecRewardItem );

        if ( false == mapSubjectList_.insert( std::make_pair( kSubject.m_dwSubjectID, kSubject ) ).second ) {
            START_LOG( cerr, L"SubjectTemplate.stg에 중복된 과제ID가 있습니다." ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // endTable SubjectList

    return true;
}

bool KSubjectManager::LoadDropList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewardItem_ )
{
    vecRewardItem_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        KDropItemInfo kDropItemInfo;
        int nGrade = 0;

        _JIF( kLuaMng_.GetValue( 1, kDropItemInfo.m_ItemID ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, nGrade ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 3, kDropItemInfo.m_nPeriod ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 4, kDropItemInfo.m_nDuration ) == S_OK, return false );

        kDropItemInfo.m_cGradeID = static_cast< char >( nGrade );

        vecRewardItem_.push_back( kDropItemInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KSubjectManager::LoadSubjectInfo( OUT std::map< PAIR_INT, std::map< DWORD, std::set< DWORD > > >& mapSubjectInfo_ )
{
    mapSubjectInfo_.clear();

    KLuaManager kLuaMng;

    _JIF( kLuaMng.DoFile( "InitSubject.lua" ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "Subject" ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng.BeginTable( i ) != S_OK ) break;

        PAIR_INT prModeAndDifficulty;
        std::map< DWORD, std::set< DWORD > > mapTriggerAndSubjectInfo;

        _JIF( kLuaMng.GetValue( "ModeID", prModeAndDifficulty.first ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( "Difficulty", prModeAndDifficulty.second ) == S_OK, return false );
        if ( false == LoadTriggerAndSubjectInfo( kLuaMng, std::string( "SubjectList" ), mapTriggerAndSubjectInfo ) ) {
            START_LOG( cerr, L"InitSubject.lua의 ModeID: " << prModeAndDifficulty.first << ", Difficulty: " << prModeAndDifficulty.second )
                << END_LOG;
            return false;
        }

        if ( false == mapSubjectInfo_.insert( std::make_pair( prModeAndDifficulty, mapTriggerAndSubjectInfo ) ).second ) {
            START_LOG( cerr, L"InitSubject.lua에 중복된 (모드ID, 난이도)가 있습니다. ModeID: "
                << prModeAndDifficulty.first << ", Difficulty: " << prModeAndDifficulty.second ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // endTable SubjectList
    return true;
}

bool KSubjectManager::LoadTriggerAndSubjectInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, std::set< DWORD > >& mapTriggerAndSubjectInfo_ )
{
    mapTriggerAndSubjectInfo_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        DWORD dwTriggerID = 0;
        std::set< DWORD > setSubjectID;

        _JIF( kLuaMng_.GetValue( "TriggerID", dwTriggerID ) == S_OK, return false );
        _JIF( LoadSubjectIDInfo( kLuaMng_, std::string( "SubjectID" ), setSubjectID ), return false );

        if ( false == mapTriggerAndSubjectInfo_.insert( std::make_pair( dwTriggerID, setSubjectID ) ).second ) {
            START_LOG( cerr, L"InitSubject.lua에 중복된 트리거ID가 있습니다. TriggerID: " << dwTriggerID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KSubjectManager::LoadSubjectIDInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< DWORD >& setSubjectID_ )
{
    setSubjectID_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while ( true ) {
        DWORD dwSubjectID = 0;
        if ( kLuaMng_.GetValue( i++, dwSubjectID ) != S_OK ) break;

        setSubjectID_.insert( dwSubjectID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KSubjectManager::GetSubject( IN const PAIR_INT& prModeAndDifficulty_, IN const DWORD dwTriggerID_, IN const DWORD dwSubjectID_, OUT KSubject& kSubject_ )
{
    KLocker lock( m_csSubject );

    std::map< PAIR_INT, std::map< DWORD, std::set< DWORD > > >::iterator mitMode;
    mitMode = m_mapSubjectInfo.find( prModeAndDifficulty_ );
    if ( mitMode == m_mapSubjectInfo.end() ) {
        START_LOG( cerr, L"과제정보가 설정되지 않은 모드에서 요청함. ModeID: " << prModeAndDifficulty_.first
            << ", Difficulty: " << prModeAndDifficulty_.second ) << END_LOG;
        return false;
    }

    std::map< DWORD, std::set< DWORD > >::iterator mitTrigger;
    mitTrigger = mitMode->second.find( dwTriggerID_ );
    if ( mitTrigger == mitMode->second.end() ) {
        START_LOG( cerr, L"해당 모드에 설정되지 않은 트리거를 요청함. TriggerID: " << dwTriggerID_ ) << END_LOG;
        return false;
    }

    std::set< DWORD >::iterator sitSubject;
    sitSubject = mitTrigger->second.find( dwSubjectID_ );
    if ( sitSubject == mitTrigger->second.end() ) {
        START_LOG( cerr, L"해당 트리거에 설정되지 않은 과제를 요청함. SubjectID: " << dwSubjectID_ ) << END_LOG;
        return false;
    }

    std::map< DWORD, KSubject >::iterator mitSubject;
    mitSubject = m_mapSubjectList.find( dwSubjectID_ );
    if ( mitSubject == m_mapSubjectList.end() ) {
        START_LOG( cerr, L"과제 보상 정보가 없음. SubjectID: " << dwSubjectID_ ) << END_LOG;
        return false;
    }

    kSubject_ = mitSubject->second;

    return true;
}

bool KSubjectManager::GetSubject( IN const DWORD dwSubjectID_, OUT KSubject& kSubject_ )
{
    KLocker lock( m_csSubject );

    std::map< DWORD, KSubject >::iterator mitSubject;
    mitSubject = m_mapSubjectList.find( dwSubjectID_ );
    if ( mitSubject == m_mapSubjectList.end() ) {
        START_LOG( cerr, L"과제 보상 정보가 없음. SubjectID: " << dwSubjectID_ ) << END_LOG;
        return false;
    }

    kSubject_ = mitSubject->second;

    return true;
}
