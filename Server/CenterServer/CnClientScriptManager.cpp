#include "CnClientScriptManager.h"
#include <dbg/dbg.hpp >
#include "Lua/KLuaManager.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnClientScriptManager );
ImplOstmOperatorW2A( KCnClientScriptManager );
NiImplementRootRTTI( KCnClientScriptManager );

KCnClientScriptManager::KCnClientScriptManager(void)
{
}

KCnClientScriptManager::~KCnClientScriptManager(void)
{
}

ImplToStringW( KCnClientScriptManager )
{
    KLocker lock1( m_csMonsterInfo );

    return START_TOSTRINGW
        << TOSTRINGW( m_mapMonsterInfo.size() );
}

bool KCnClientScriptManager::LoadScript()
{
    KLuaManager kLuaMng;
    _JIF( LoadMonsterInfo( kLuaMng ), return false );
    return true;
}

bool KCnClientScriptManager::LoadMonsterInfo( KLuaManager& kLuaMng_ )
{
    std::map< int, KSimpleMonsterInfo > mapMonsterInfo;
    std::string strFile( "monstertemplate.stg" );
    std::string strRequire( "Enum.stg" );

    _JIF( kLuaMng_.DoFile( strRequire.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.DoFile( strFile.c_str() ) == S_OK, return false );
    _JIF( kLuaMng_.BeginTable( "MonsterList" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        int nID = -1;
        KSimpleMonsterInfo kInfo;
        kInfo.Init();

        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng_.GetValue( "ID", nID ) == S_OK, return false );
        kLuaMng_.GetValue( "fAttackPoint", kInfo.m_fAttackPoint );
        kLuaMng_.GetValue( "fHPPoint", kInfo.m_fHPPoint );
        kLuaMng_.GetValue( "fIncAttackPoint", kInfo.m_fIncAttackPoint );
        kLuaMng_.GetValue( "fIncHPPoint", kInfo.m_fIncHPPoint );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapMonsterInfo[nID] = kInfo;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"몬스터 템플릿 정보. 스크립트 로드 완료. size : " << mapMonsterInfo.size() ) << END_LOG;

    KLocker lock( m_csMonsterInfo );
    m_mapMonsterInfo.swap( mapMonsterInfo );
    return true;
}

void KCnClientScriptManager::GetMonsterInfo( OUT std::map< int, KSimpleMonsterInfo >& mapInfo_ )
{
    KLocker lock( m_csMonsterInfo );
    mapInfo_ = m_mapMonsterInfo;
}

void KCnClientScriptManager::DumpMonsterInfo()
{
    std::map< int, KSimpleMonsterInfo >::const_iterator cmit;
    START_LOG( cerr, L"================== Dump Monster Info ==================" << dbg::endl
        << L" ID, AttackPoint, HPPoint, IncAttackPoint, IncHPPoint" ) << END_LOG;

    KLocker lock( m_csMonsterInfo );
    for( cmit = m_mapMonsterInfo.begin() ; cmit != m_mapMonsterInfo.end() ; ++cmit )
    {
        START_LOG( cerr, L" " << cmit->first
            << L", " << cmit->second.m_fAttackPoint
            << L", " << cmit->second.m_fHPPoint
            << L", " << cmit->second.m_fIncAttackPoint
            << L", " << cmit->second.m_fIncHPPoint ) << END_LOG;
    }
    START_LOG( cerr, L"====================== End Dump ======================="  ) << END_LOG;
}