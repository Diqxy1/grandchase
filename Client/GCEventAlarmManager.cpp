#include "stdafx.h"
#include "GCEventAlarmManager.h"


GCEventAlarmManager::GCEventAlarmManager()
: m_bOpenEventAlarm ( true )
{
	m_mapEventAlarmList.clear();
}

GCEventAlarmManager::~GCEventAlarmManager()
{
}

void GCEventAlarmManager::LoadScript()
{
    KLuaManager luaMgr;

	m_mapEventAlarmList.clear();

	if( GCFUNC::LoadLuaScript( luaMgr , "EventAlarm.lua" ) == false )
		return;


	LUA_BEGIN_TABLE("EventAlarmList",return);
	{
		for(int i = 1; ; i++)
		{
			SEventAlarm sEventAlarm;
			LUA_BEGIN_TABLE(i,break)
			{

				LUA_GET_VALUE("EventID",sEventAlarm.m_nEventID,return);
				LUA_GET_VALUE("ParticleName", sEventAlarm.m_strParticleName,return);
				LUA_GET_VALUE("EventTitle", sEventAlarm.m_nTitleString,return);
				LUA_GET_VALUE("EventDesc",sEventAlarm.m_nDescString,return);
			}


			m_mapEventAlarmList.insert(std::make_pair(sEventAlarm.m_nEventID, sEventAlarm));
			LUA_END_TABLE(return)
		}
	}
	LUA_END_TABLE(return)
}

bool GCEventAlarmManager::GetEventAlarm( int nEventID, SEventAlarm& sEventAlarm )
{
	if ( m_mapEventAlarmList.empty() )
		return false;


	std::map<int,SEventAlarm>::iterator pos = m_mapEventAlarmList.find( nEventID );
	if ( pos != m_mapEventAlarmList.end() )
	{
		sEventAlarm = pos->second;
		return true;
	}

	return false;
}


