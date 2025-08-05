#include "ServerBenefit.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"

ImplementSingleton( KServerBenefit );
ImplOstmOperatorW2A( KServerBenefit );
NiImplementRootRTTI( KServerBenefit );

KServerBenefit::KServerBenefit(void)
: m_prLvRange(-1,-1)
, m_fConditionBenefit(0.f)
, m_fDropBenefit(0.f)
{
    m_setConditionIDs.clear();
}

KServerBenefit::~KServerBenefit(void)
{
}

ImplToStringW( KServerBenefit )
{
    KLocker lock( m_csServerBenefit );

    return START_TOSTRINGW
        << TOSTRINGW( m_prLvRange.first )
        << TOSTRINGW( m_prLvRange.second )
        << TOSTRINGW( m_setConditionIDs.size() )
        << TOSTRINGW( m_fConditionBenefit )
        << TOSTRINGW( m_fDropBenefit );
}

bool KServerBenefit::LoadScript()
{
    return _LoadScript(std::string());
}

bool KServerBenefit::_LoadScript( OUT std::string& strScript_ )
{
    std::set<int> setConditionIDs;
    setConditionIDs.clear();
    float fConditionBenefit;
    float fDropBenefit;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitServerBenefit.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ConditionBenefit", fConditionBenefit ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "DungeonDropBenefit", fDropBenefit ) == S_OK, return false );

    _JIF( kLuaMng.BeginTable( "MissionCondisions" ) == S_OK, return false );
    int i = 0;
    while( true ) {
        int nConditionID = -1;
        if ( kLuaMng.GetValue( ++i, nConditionID ) != S_OK ) break;
        setConditionIDs.insert( nConditionID );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"레벨별 서버 혜택 스크립트 로드 완료." )
        << BUILD_LOG( setConditionIDs.size() )
        << BUILD_LOG( fConditionBenefit )
        << BUILD_LOG( fDropBenefit ) << END_LOG;

    {
        KLocker lock( m_csServerBenefit );
        m_setConditionIDs.swap( setConditionIDs );
        m_fConditionBenefit = fConditionBenefit;
        m_fDropBenefit = fDropBenefit;
    }

    return true;
}

void KServerBenefit::SetLevelRange( IN std::pair<int,int>& prLvRange_ )
{
    KLocker lock( m_csServerBenefit );
    m_prLvRange = prLvRange_;
}

bool KServerBenefit::IsCorrectLvRange( IN const int nCharLevel_ )
{
    KLocker lock( m_csServerBenefit );
    if ( m_prLvRange.first < 0 && m_prLvRange.second < 0 ) {
        return false;
    }
    return (m_prLvRange.first <= nCharLevel_ && m_prLvRange.second >= nCharLevel_);
}

float KServerBenefit::GetConditionBenefit()
{
    KLocker lock( m_csServerBenefit );
    return m_fConditionBenefit;
}

bool KServerBenefit::IsCorrectCondition( IN const int nConditionID_ )
{
    KLocker lock( m_csServerBenefit );
    return ( m_setConditionIDs.find( nConditionID_ ) != m_setConditionIDs.end() );
}

float KServerBenefit::GetDropBenefit()
{
    KLocker lock( m_csServerBenefit );
    return m_fDropBenefit;
}