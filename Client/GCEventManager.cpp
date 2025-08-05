#include "stdafx.h"
#include ".\gceventmanager.h"
#include "MyD3D.h"

//
//
#include "KGCRoomManager.h"

ImplementSingleton( KGCEventManager )

KGCEventManager::KGCEventManager(void)
{
    //포인터 100개 정도 잡아준다.
    m_vecParticles.reserve( 100 );
}

KGCEventManager::~KGCEventManager(void)
{
}

bool KGCEventManager::ParticleEffect( std::string strName_, float fx_ /*= -1 */, float fy_ /*= -1*/, float fz_ /*= -1 */ )
{
    m_vecParticles.push_back( g_ParticleManager->CreateSequence( strName_, fx_, fy_, fz_ ) );
    return true;
}

KGCEventManager* GetEventManager()
{
    return SiKGCEventManager();
}

bool KGCEventManager::RegisterLuabind()
{
    lua_tinker::def( KGCLuabinder::getInstance().GetLuaState(), "GetEventManager", GetEventManager );

    lua_tinker::class_add<PLAYER>( KGCLuabinder::getInstance().GetLuaState(), "KGCEventManager" );
        lua_tinker::class_def<PLAYER>( KGCLuabinder::getInstance().GetLuaState(), "AddParticle",    &KGCEventManager::ParticleEffect  );;

    if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "LowEvent.lua" ) == false )
    {
        OKBOXA( "LowEvent.lua load Failed" , "error");
        return false;
    }

    if( GCFUNC::LoadLuaScript( KGCLuabinder::getInstance(), "EventScript.stg" ) == false )
    {
        OKBOXA( "EventScript.stg load Failed!!" , "error");
        return false;
    }

    return true;
}

void KGCEventManager::ClearResource()
{
	// 이전대로면 dangling pointer를 유발
	// 이렇게 수정하면 댕글거리지 않음 
	for ( std::vector<CParticleEventSeqPTR>::iterator vit = m_vecParticles.begin(); vit != m_vecParticles.end(); vit++ )    
	{
		g_ParticleManager->DeleteSequence( *vit );
		(*vit) = NULL;
	}
	m_vecParticles.clear();
}

void KGCEventManager::ClearEvents()
{
    m_mapEventPool.clear();
}

bool KGCEventManager::IsActiveEvent( int iEventID )
{
    if( m_setEvents.count( iEventID ) != 0 )
        return true;
    return false;
}

void KGCEventManager::SetEvents( std::set<int>& setEvents )
{
    m_setEvents.swap( setEvents );
}

void KGCEventManager::SetEvents( std::map< DWORD, std::set<int> >& mapEvents )
{
	std::map< DWORD, std::set<int> >::iterator mit = mapEvents.find( g_kGlobalValue.m_kUserInfo.dwUID );

	if( mapEvents.end() == mit )
	{
		m_setEvents.clear();
		return;
	}
	SetEvents( mit->second );
}

bool KGCEventManager::EventActivation( DWORD dwEventID_, int iPosition )
{
    DWORD dwGameMode = SiKGCRoomManager()->GetGameMode();
    std::stringstream strLuaFunc;
    strLuaFunc<<"EventActivation("<<dwEventID_<<","<<dwGameMode<<","<<iPosition<<")";

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
	lua_checkstack( L, 20 );
	lua_pushstring( L, "EventActivation" );
	lua_rawget( L, LUA_GLOBALSINDEX );
	if( lua_type( L, -1 ) == LUA_TFUNCTION )
	{
		lua_pushinteger( L, dwEventID_ );
		lua_pushinteger( L, dwGameMode );
		lua_pushinteger( L, iPosition );
		if( lua_pcall( L, 3, -1, 0 ) != 0 )
		{
			ASSERT_MBOX( "Event Activation Error : "<< dwEventID_ );
			return false;
		}
	}
	else
	{
		lua_pop( L, -1 );
		ASSERT_MBOX( "Event Activation Error : "<< dwEventID_ );
		return false;
	}
    return true;
}
