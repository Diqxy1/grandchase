#include "stdafx.h"
#include "GCItem.h"
#include "Procedure.h"
#include "KSingleton.h"
#include "GCItemManager.h"
#include "GCGlobalFunction.h"
#include "GCParticleChangeSystem.h"

KGCParticleChangeSystem* g_pParticleChangeSys = NULL;

KGCParticleChangeSystem::KGCParticleChangeSystem()
{
    g_pParticleChangeSys = this;
    m_vecParticleChange.clear();
}

KGCParticleChangeSystem::~KGCParticleChangeSystem()
{
}

void KGCParticleChangeSystem::SetParticleChangeList()
{
    if ( !m_vecParticleChange.empty() )
		return;

	KLuaManager luaMgr;
	if ( GCFUNC::LoadLuaScript( luaMgr, "initparticlechange.lua" ) == false )
		return;

	luaMgr.BeginTable( "ParticleChange" );
	{
		for ( int i = 1; ; ++i )
		{
			if ( FAILED( luaMgr.BeginTable( i ) ) ) break;
			{
				GCParticleChangeDefinition TempParticleChange;
				LUA_GET_VALUE_DEF( "ItemID",	TempParticleChange.m_iItemID,	0 );
				if ( FAILED( luaMgr.BeginTable( "Particles" ) ) ) break;
				{
					for ( int j = 1; ; ++j )
					{
						if ( FAILED( luaMgr.BeginTable( j ) ) ) break;
						{
							GCParticleChangeString TempParticleString;
							LUA_GET_VALUE_DEF( "PCurrent",	TempParticleString.m_sCurrentParticle,	"" );
							LUA_GET_VALUE_DEF( "PChange",	TempParticleString.m_sCustomParticle,	"" );
							TempParticleChange.m_vecParticleVector.push_back( TempParticleString );
						}
						luaMgr.EndTable();
					}
				}
				luaMgr.EndTable();
				m_vecParticleChange.push_back( TempParticleChange );
			}
			luaMgr.EndTable();
		}
	}
	luaMgr.EndTable();
}

std::string KGCParticleChangeSystem::GetParticleChageString( const int iPlayerID, const std::string strParticleDefault )
{
	if ( m_vecParticleChange.empty() )
		return strParticleDefault;

	for ( auto& ParticleChange : m_vecParticleChange )
		if ( g_MyD3D->MyPlayer[ iPlayerID ]->CheckEquipItem( ParticleChange.m_iItemID / 10, true ) )
			for ( auto& ParticleChangeString : ParticleChange.m_vecParticleVector )
				if( ParticleChangeString.m_sCurrentParticle == strParticleDefault )
					return ParticleChangeString.m_sCustomParticle;

	return strParticleDefault;
}
