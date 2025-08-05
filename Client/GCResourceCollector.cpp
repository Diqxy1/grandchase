#include "StdAfx.h"
#include "GCResourceCollector.h"
#include "MyD3D.h"
#include "GCStateMachine.h"
#include "Pet.h"

ImplementSingleton( KGCResourceCollector )

KGCResourceCollector::KGCResourceCollector(void)
{
	m_bStarted = false;
	m_mapTableName[GCRC_PLAYER] = "CharacterFeedback";
	m_mapTableName[GCRC_MONSTER] = "MonsterFeedback";
	m_mapTableName[GCRC_DUNGEON] = "DungeonFeedback";
    m_mapTableName[GCRC_PET] = "PetFeedback";
    m_mapTableName[GCRC_NOTYPE] = "NotypeFeedback";

	m_mapKindMaxNum[GCRC_PLAYER]  = CID_MAX;
	m_mapKindMaxNum[GCRC_MONSTER] = MONSTER_MODEL_NUM;
	m_mapKindMaxNum[GCRC_DUNGEON] = NUM_GC_GAME_MODE;
    m_mapKindMaxNum[GCRC_PET] = EGCPETID::NUM;
    m_mapKindMaxNum[GCRC_NOTYPE] = NUM_GAME_STATE;
}

KGCResourceCollector::~KGCResourceCollector(void)
{
}


void KGCResourceCollector::StartCollection()
{
	m_bStarted = true;
}

void KGCResourceCollector::StopCollection()
{
	m_bStarted = false;
}

void KGCResourceCollector::AddNotLoadedParticleTexture( std::string strTextureName )
{
    if( m_bActivation == false )
        return;

	if( m_bStarted == false )
    {

#if !defined( __PATH__ ) && defined(LOAD_NECESSITY_TEXTURE)
        std::wstringstream stm;
        stm<<GCUTIL_STR::GCStrCharToWide( strTextureName.c_str() );
        stm<<L" 파일이 수집되지 않습니다.";
        g_pkChatManager->AddChatMsg( stm.str() );
        stm.str(L"");
#endif
        FeedbackToFile( GCRC_NOTYPE, g_MyD3D->m_pStateMachine->GetState(), strTextureName );
        AddToReportList( GCRC_NOTYPE, g_MyD3D->m_pStateMachine->GetState(), strTextureName );

		return;
    }

	FeedbackToFile( m_CurrentState.first, m_CurrentState.second, strTextureName );
    AddToReportList( m_CurrentState.first, m_CurrentState.second, strTextureName );
}

void KGCResourceCollector::SetResouceType( EGCResourceType eType, int iValue )
{
	m_CurrentState.first = eType;
	m_CurrentState.second = iValue;
}

bool KGCResourceCollector::FeedbackToFile( EGCResourceType eType, int kind, const std::string& strName, ResourceListAll& RemoveList )
{
	ResourceListAll mapResources;

	KLuaManager luaMgr;
	if( SUCCEEDED( luaMgr.DoFile( "ResFeedback.dat" ) ) )
	{
		for( int i = 0 ; i < GCRC_NUM ; ++i )
		{
			GetResourceList( luaMgr, static_cast<EGCResourceType>(i), mapResources[static_cast<EGCResourceType>(i)] );
		}
	}

	mapResources[eType][kind].insert( strName );

	////////////////////////////////////////////////////////////////////////
	std::fstream file;
//#if defined( __PATH__ )
	file.open( "ResFeedback.dat", std::ios_base::out );
//#else
//    file.open( "./Stage/script.kom/ResourceList.dat", std::ios_base::out );
//#endif

	if( file.is_open() == false )
		return false;

	for( int i = 0 ; i < GCRC_NUM ; ++i )
	{
		if( mapResources.find( static_cast<EGCResourceType>(i) ) == mapResources.end() )
			continue;

		WriteResourceList( file, static_cast<EGCResourceType>(i), mapResources[static_cast<EGCResourceType>(i)], RemoveList[static_cast<EGCResourceType>(i)] );
	}

	file.close();
	return true;
}

bool KGCResourceCollector::GetResourceList( KLuaManager& luaMgr, EGCResourceType eType, ResourceList& List )
{
	if( SUCCEEDED( luaMgr.BeginTable( m_mapTableName[eType].c_str() ) ) )
	{
		for( int i = 0 ; i < m_mapKindMaxNum[eType] ; ++i )
		{
			if( FAILED( luaMgr.BeginTable(i) ) )
				continue;

			for( int j = 1 ; ; ++j )
			{
				std::string strDeviceName;
				LUA_GET_VALUE_NOASSERT( j, strDeviceName, break );

				List[i].insert( strDeviceName );
			}

			luaMgr.EndTable();
		}
		luaMgr.EndTable();
	}
	else
	{
		return false;
	}
	return true;
}

bool KGCResourceCollector::WriteResourceList( std::fstream& file, EGCResourceType eType, ResourceList& List, ResourceList& RemoveList )
{
	file<<m_mapTableName[eType]<<" = {}"<<std::endl;
	ResourceList::iterator mit;
	for( mit = List.begin() ; mit != List.end() ; ++mit )
	{
		std::set<std::string>::iterator sit;
		file<<m_mapTableName[eType]<<"["<<mit->first<<"] = {";
		for( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit )
		{
			if( RemoveList[mit->first].find( *sit ) != RemoveList[mit->first].end() )
				continue;

			file<<"\""<<(*sit)<<"\",";
		}
		file<<"}"<<std::endl;
	}

	return true;
}

void KGCResourceCollector::LoadTextures( EGCResourceType eType, int kind )
{
#if !defined( LOAD_NECESSITY_TEXTURE )
	return;
#endif
	std::set<std::string> setResource;

	KLuaManager luaMgr;
	if( SUCCEEDED( luaMgr.DoFile( "ResFeedback.dat" ) ) )
	{
		GetLoadList( luaMgr, eType, kind, setResource );
	}

	KLuaManager luaMgr2;
	if( GCFUNC::LoadLuaScript( luaMgr2, "ResourceList.stg" ) )
	{
		GetLoadList( luaMgr2, eType, kind, setResource );
	}

	std::set<std::string>::iterator sit;
	for( sit = setResource.begin() ; sit != setResource.end() ; ++sit )
	{
		g_pGCDeviceManager->CreateTexture( *sit );
	}
}

void KGCResourceCollector::GetLoadList( KLuaManager& luaMgr, EGCResourceType eType, int kind, std::set<std::string>& List )
{
	ResourceList mapResources;
	GetResourceList( luaMgr, eType, mapResources );

	List.insert( mapResources[kind].begin(), mapResources[kind].end() );
}

void KGCResourceCollector::RefreshLocalScript()
{
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr, "ResourceList.stg" ) == false )
		return;

	std::map< EGCResourceType, ResourceList > mapResources;

	for( int i = 0 ; i < GCRC_NUM ; ++i )
	{
		GetResourceList( luaMgr, static_cast<EGCResourceType>(i), mapResources[static_cast<EGCResourceType>(i)] );
	}

	FeedbackToFile( m_CurrentState.first, m_CurrentState.second, std::string(), mapResources );
}

void KGCResourceCollector::AddToReportList( EGCResourceType eType, int kind, std::string strResourceName )
{
    m_ReportList[MAKELONG( kind, eType )].insert( strResourceName );
}

void KGCResourceCollector::ClearReportList()
{
    m_ReportList.clear();
}

void KGCResourceCollector::ReportToServer()
{
    if( m_ReportList.empty() )
        return;
#if defined(SEND_COLLECTED_SERVER)
    if( KP2P::GetInstance()->IsConnectedToGameServer() )
        KP2P::GetInstance()->Send_EffectTextureReport( m_ReportList );
#endif
    ClearReportList();
}