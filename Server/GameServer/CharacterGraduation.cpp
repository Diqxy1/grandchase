#include "CharacterGraduation.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
#include "UserEvent.h"

ImplementSingleton( KCharacterGraduation );
ImplOstmOperatorW2A( KCharacterGraduation );
NiImplementRootRTTI( KCharacterGraduation );

KCharacterGraduation::KCharacterGraduation(void)
{
}

KCharacterGraduation::~KCharacterGraduation(void)
{
}

ImplToStringW( KCharacterGraduation )
{
    KLocker lock( m_csCharacterGraduation );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapGraduation.size() );
}

bool KCharacterGraduation::LoadScript()
{
    return _LoadScript(std::string());
}

bool KCharacterGraduation::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMng;
    const std::string strScriptName = "InitCharacterGraduation.lua";
    strScript_ = strScriptName;

    std::map< int, KGraduation > mapGraduation; // < 캐릭터ID, 졸업정보 >
    mapGraduation.clear();

    _JIF( kLuaMng.DoFile( "Enum.stg" ) == S_OK, return false );
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "Graduation" ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng.BeginTable( i ) != S_OK ) break;

        KGraduation kGraduation;

        _JIF( kLuaMng.GetValue( "CharType", kGraduation.m_nCharType ) == S_OK, return false );
        if ( false == LoadCondition( kLuaMng, std::string( "Condition" ), kGraduation.m_mapConditionModes ) ) {
            START_LOG( cerr, L"InitCharacterGraduation.lua의 CharType: " << kGraduation.m_nCharType ) << END_LOG;
            return false;
        }
        _JIF( LoadClearModes( kLuaMng, std::string( "ClearModes" ), kGraduation.m_setClearModes ), return false );
        _JIF( LoadRewards( kLuaMng, std::string( "Rewards" ), kGraduation.m_vecRewards ), return false );

        if ( false == mapGraduation.insert( std::make_pair( kGraduation.m_nCharType, kGraduation ) ).second ) {
            START_LOG( cerr, L"InitCharacterGraduation.lua에 중복된 캐릭터ID가 있습니다. CharType: " << kGraduation.m_nCharType ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // endTable SubjectList

    {
        KLocker lock( m_csCharacterGraduation );

        m_mapGraduation.swap( mapGraduation );
    }
    START_LOG( cerr, L"Character Graduation script loaded." ) << END_LOG;

    return true;
}

bool KCharacterGraduation::LoadCondition( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::set< int > >& mapConditionModes_ )
{
    mapConditionModes_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );
    for ( int i = 1; ; ++i ) {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nModeID = 0;
        std::set< int > setDifficulty;

        _JIF( kLuaMng_.GetValue( "ModeID", nModeID ) == S_OK, return false );
        _JIF( LoadDifficulty( kLuaMng_, std::string( "Difficulty" ), setDifficulty ), return false );

        if ( false == mapConditionModes_.insert( std::make_pair( nModeID, setDifficulty ) ).second ) {
            START_LOG( cerr, L"중복된 ModeID가 있습니다. ModeID: " << nModeID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCharacterGraduation::LoadDifficulty( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< int >& setDifficulty_ )
{
    setDifficulty_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while ( true ) {
        int nModeID = 0;
        if ( kLuaMng_.GetValue( i++, nModeID ) != S_OK ) break;

        setDifficulty_.insert( nModeID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCharacterGraduation::LoadClearModes( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< int >& setClearModes_ )
{
    setClearModes_.clear();

    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    int i = 1;
    while ( true ) {
        int nModeID = 0;
        if ( kLuaMng_.GetValue( i++, nModeID ) != S_OK ) break;

        setClearModes_.insert( nModeID );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCharacterGraduation::LoadRewards( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewards_ )
{
    vecRewards_.clear();

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

        vecRewards_.push_back( kDropItemInfo );

        _JIF( kLuaMng_.EndTable() == S_OK, return false ); // // endTable i
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false ); // endTable c_str()

    return true;
}

bool KCharacterGraduation::GetGraduation( OUT std::map< int, KGraduation >& mapGraduation_ )
{
    KLocker lock( m_csCharacterGraduation );

    if ( true == m_mapGraduation.empty() ) {
        return false;
    }

    mapGraduation_ = m_mapGraduation;

    return true;
}
