#include "stdafx.h"
// KTDXTimer.cpp: implementation of the CKTDXTimer class.
//
//////////////////////////////////////////////////////////////////////

//
//


#include "KTDXTimer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKTDXTimer::CKTDXTimer()
{
	m_AutoFlow = true;
}

CKTDXTimer::~CKTDXTimer()
{
	for( int i = 0; i < (int)m_TimerList.size(); i++ )
	{
		SAFE_DELETE( m_TimerList[i] );
	}
	m_TimerList.clear();

	m_DeleteTimerList.clear();
}

HRESULT CKTDXTimer::FrameMove( float fElapsedTime, void* pframeData )
{
	if( m_AutoFlow == false )
		return S_OK;

	TimeFlow( fElapsedTime );

	return S_OK;
}

void CKTDXTimer::SetTimer( int id, int time, CKTDXTimer::TIMER_TYPE type, int startDelay/* = 0*/, int EndDelay/* = 0*/ )
{
	str_TimerData* timer;

	timer				= new str_TimerData;
	timer->id			= id;
	timer->fElapsedTime = (float)(time/1000.0f);
    timer->fStartDelay  = (float)(startDelay/1000.0f);
    timer->fEndDelay    = (float)(EndDelay/1000.0f);
	timer->fAddTime		= 0.0f;
	timer->type			= type;
	timer->timeOut		= false;

	m_TimerList.push_back(timer);

}

bool CKTDXTimer::DestroyTimer( int id )
{
	for( int i = 0; i < (int)m_TimerList.size(); i++ )
	{
		if( m_TimerList[i]->id == id )
		{
			SAFE_DELETE(m_TimerList[i]);
			m_TimerList.erase( m_TimerList.begin() + i );

			for( i = 0; i < (int)m_DeleteTimerList.size(); i++ )
			{
				if( m_DeleteTimerList[i] == i )
					m_DeleteTimerList.erase( m_DeleteTimerList.begin() + i );
			}
			
			return true;
		}
	}
	return false;
}

bool CKTDXTimer::CheckTimer( int id )
{
	for( int i = 0; i < (int)m_TimerList.size(); i++ )
	{
		str_TimerData* t = m_TimerList[i];
		if( m_TimerList[i]->id == id )
		{
			return m_TimerList[i]->timeOut;
		}
	}
	return false;
}

void CKTDXTimer::TimeFlow( float fElapsedTime )
{
	for( int i=(int)m_DeleteTimerList.size()-1 ; i>=0 ; --i )
	{
		SAFE_DELETE(m_TimerList[m_DeleteTimerList[i]]);
		m_TimerList.erase( m_TimerList.begin() + m_DeleteTimerList[i] );
	}

	m_DeleteTimerList.clear();
	
	for( int i = 0; i < (int)m_TimerList.size(); i++ )
	{
        str_TimerData* sT = m_TimerList[i];

		sT->fAddTime += fElapsedTime;
		if( sT->fAddTime > sT->fElapsedTime + sT->fStartDelay )
		{
			sT->timeOut = true;
			if( sT->type == LOOP )
			{
                sT->fAddTime = 0.0f;
				sT->fAddTime -= sT->fEndDelay;
			}
			else
			{
				m_DeleteTimerList.push_back(i);
			}
		}
		else
		{
			sT->timeOut = false;
		}
	}
}