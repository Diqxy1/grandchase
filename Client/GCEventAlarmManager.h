#pragma once

struct SEventAlarm
{
	int m_nEventID;		//	EventID( InitClientContent.lua )
	
	std::string m_strParticleName;
	int	m_nTitleString;
	int m_nDescString;

	SEventAlarm()
	: m_nEventID ( -1 )
	, m_nTitleString( -1 )
	, m_nDescString( -1 )
	{
	}

	bool InVaild() { return ( m_nEventID != -1 ); }
};


class GCEventAlarmManager
{
public:
	GCEventAlarmManager();
	~GCEventAlarmManager();

	void LoadScript();
	bool GetEventAlarm( int nEventID, SEventAlarm& sEventAlarm );
	bool IsOpenEventAlarm() { return m_bOpenEventAlarm; }
	void SetEventAlarm( bool _bOpenEventAlarm ) { m_bOpenEventAlarm = _bOpenEventAlarm; }


public:

	std::map<int,SEventAlarm> m_mapEventAlarmList;
	
	bool	m_bOpenEventAlarm;	//	출력 여부
};


