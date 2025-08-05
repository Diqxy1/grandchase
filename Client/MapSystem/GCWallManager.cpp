/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

	★ 설계자 ☞ 이상호
	★ 설계일 ☞ 2013년 05월 21일
	★ E-Mail ☞ shmhlove@kog.co.kr
	★ 클래스 ☞ 벽 클래스( 스테이지 내에서 블럭을 구분짓기 위한 벽 )

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "stdafx.h"
#include "GCWallManager.h"


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★스크립트 파일모니터 등록★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#if !defined( __PATH__ )
#include "../GCUtil/GCFileMoniter.h"
static bool bReLoadWall = false;
void ReLoadWallAction()
{
    bReLoadWall	= true;
}
void KGCWallManager::CheckFileMonitor()
{
    if( bReLoadWall )
    {
        LoadScript();
        bReLoadWall = false;
    }
}
void KGCWallManager::RegisterFileMoniter( const char *szFileName )
{
    SiKGCFileMoniter()->RegisterFile( g_pGCDeviceManager2->GetMassFileManager()->GetResFullPath( szFileName ) , ReLoadWallAction );
}
#endif


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★★★★벽 매니져 클래스★★★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
ImplementSingleton( KGCWallManager )

KGCWallManager::KGCWallManager()
{
    m_mapWallInstance.clear();
    m_mapWallTemplate.clear();
}

KGCWallManager::~KGCWallManager()
{
    DestoryAllInstance();
}

void KGCWallManager::DestoryAllInstance()
{
    std::map<DWORD, KGCWall*>::iterator mapIter = m_mapWallInstance.begin();
    for(; mapIter != m_mapWallInstance.end(); ++mapIter)
    {
        SAFE_DELETE( mapIter->second );
    }
    m_mapWallInstance.clear();
}

void KGCWallManager::ClearInstance( DWORD dwUID )
{
    std::map<DWORD, KGCWall*>::iterator mapIter = m_mapWallInstance.find( dwUID );
    if( mapIter == m_mapWallInstance.end() )
        return;

    SAFE_DELETE( mapIter->second );
    m_mapWallInstance.erase( mapIter );
}

void KGCWallManager::FrameMove()
{
    std::vector<DWORD> vecDeleteList;
    std::map<DWORD, KGCWall*>::iterator mapIter = m_mapWallInstance.begin();
    for(; mapIter != m_mapWallInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->FrameMove();

        if( mapIter->second->m_bDestroyWall )
            vecDeleteList.push_back( mapIter->first );
    }

    std::vector<DWORD>::iterator vecIter = vecDeleteList.begin();
    for(; vecIter != vecDeleteList.end(); ++vecIter)
    {
        ClearInstance( (*vecIter) );
    }

#if !defined( __PATH__ )
    CheckFileMonitor();
#endif
}

void KGCWallManager::CheckInWallBound( float &fPos )
{
    std::map<DWORD, KGCWall*>::iterator mapIter = m_mapWallInstance.begin();
    for(; mapIter != m_mapWallInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->CheckInWallBound( fPos );
    }
}

void KGCWallManager::TextRender()
{
#if !defined(__PATH__)
    std::map<DWORD, KGCWall*>::iterator mapIter = m_mapWallInstance.begin();
    for(; mapIter != m_mapWallInstance.end(); ++mapIter)
    {
        if( NULL == mapIter->second )
            continue;

        mapIter->second->TextRender();
    }
#endif
}

const KGCWallTemplate* KGCWallManager::GetWallTemplate( DWORD dwType )
{
    KGCWallTemplate* pWallTemplate = NULL;
    std::map<DWORD, KGCWallTemplate>::iterator mapIter = m_mapWallTemplate.find( dwType );
    if( mapIter != m_mapWallTemplate.end() )
        pWallTemplate = &mapIter->second;
    return pWallTemplate;
}

KGCWall* KGCWallManager::GetWallInstance( DWORD dwUID )
{
    KGCWall* pWall = NULL;
    std::map<DWORD, KGCWall*>::iterator mapIter = m_mapWallInstance.find( dwUID );
    if( mapIter != m_mapWallInstance.end() )
        pWall = mapIter->second;
    return pWall;
}

KGCWall* KGCWallManager::CreateWall( DWORD dwType, DWORD dwSlot, float fPosX )
{
    const KGCWallTemplate* pWallTemplate = GetWallTemplate( dwType );
    if( NULL == pWallTemplate )
        return NULL;

    KGCWall* pWall = new KGCWall( pWallTemplate, fPosX );
    if( pWall )
    {
        if( NULL == pWall->m_pWallTemplate )
        {
            SAFE_DELETE( pWall );
            return NULL;
        }

        if( 0 == dwSlot )
            dwSlot = (m_mapWallInstance.size() + 1);

        pWall->m_dwUID = dwSlot;
        ClearInstance( dwSlot );
        m_mapWallInstance.insert( std::make_pair(dwSlot, pWall) );
    }

    return pWall;
}

void KGCWallManager::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

    lua_tinker::class_add<KGCWallManager>( L, "KGCWallManager" );
    lua_tinker::class_def<KGCWallManager>( L, "GetWallInstance",       &KGCWallManager::GetWallInstance );

    lua_tinker::decl( L, "KGCWallManager", this );
}

void KGCWallManager::LoadScript()
{
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return;

    LoadWallTemplate( luaMgr );
}

void KGCWallManager::LoadWallTemplate( KLuaManager &luaMgr )
{
    if( GCFUNC::LoadLuaScript( luaMgr, "WallTemplate.stg" ) == false )
        return;

    m_mapWallTemplate.clear();
    if( SUCCEEDED(luaMgr.BeginTable( "WallList" )) )
    {
        KGCWallTemplate kWallTemplate;
        for(int iLoop=1; SUCCEEDED(luaMgr.BeginTable( iLoop )); ++iLoop )
        {
            kWallTemplate.Init();
            LUA_GET_VALUE_DEF( "ID", kWallTemplate.m_dwID, kWallTemplate.m_dwID );
            LUA_GET_VALUE_DEF( "DIRECTION", kWallTemplate.m_iDirection, kWallTemplate.m_iDirection );
            LUA_GET_VALUE_DEF( "CHAR_INBOUND_SIZE", kWallTemplate.m_iInBoundSize, kWallTemplate.m_iInBoundSize );
            
            if( SUCCEEDED(luaMgr.BeginTable( "Creat_Particle" )) )
            {
                kWallTemplate.m_vecCreateParticleList.clear();
                for(int iParticleLoop=1; SUCCEEDED(luaMgr.BeginTable( iParticleLoop )); ++iParticleLoop )
                {
                    KWallParticle kWallParticle;
                    LUA_GET_VALUE( 1, kWallParticle.strSquence );
                    LUA_GET_VALUE( 2, kWallParticle.vOffset.x );
                    LUA_GET_VALUE( 3, kWallParticle.vOffset.y );

                    kWallTemplate.m_vecCreateParticleList.push_back( kWallParticle );
                    luaMgr.EndTable();
                }
                luaMgr.EndTable();
            }

            if( SUCCEEDED(luaMgr.BeginTable( "Broken_Particle" )) )
            {
                kWallTemplate.m_vecBrokenParticleList.clear();
                for(int iParticleLoop=1; SUCCEEDED(luaMgr.BeginTable( iParticleLoop )); ++iParticleLoop )
                {
                    KWallParticle kWallParticle;
                    LUA_GET_VALUE( 1, kWallParticle.strSquence );
                    LUA_GET_VALUE( 2, kWallParticle.vOffset.x );
                    LUA_GET_VALUE( 3, kWallParticle.vOffset.y );

                    kWallTemplate.m_vecBrokenParticleList.push_back( kWallParticle );
                    luaMgr.EndTable();
                }
                luaMgr.EndTable();
            }

            m_mapWallTemplate.insert( std::make_pair(kWallTemplate.m_dwID, kWallTemplate) );
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }

#if !defined( __PATH__ )
    std::vector<KWallReloadList> m_vecReLoadList;
    std::map<DWORD, KGCWall*>::iterator mapIter = m_mapWallInstance.begin();
    for(; mapIter != m_mapWallInstance.end(); ++mapIter)
    {
        KWallReloadList kWall;
        kWall.dwSlot = mapIter->first;
        kWall.dwType = mapIter->second->m_pWallTemplate->m_dwID;
        kWall.fPosX = mapIter->second->m_fPosX;
        m_vecReLoadList.push_back( kWall );
    }

    DestoryAllInstance();

    std::vector<KWallReloadList>::iterator vecIter = m_vecReLoadList.begin();
    for(; vecIter != m_vecReLoadList.end(); ++vecIter)
    {
        CreateWall( vecIter->dwType, vecIter->dwSlot, vecIter->fPosX );
    }
    RegisterFileMoniter( "WallTemplate.stg" );
#endif
}