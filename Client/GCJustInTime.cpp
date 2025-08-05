#include "stdafx.h"
#include "GCJustInTime.h"

KGCJustInTime::KGCJustInTime( void )
{
    m_tmJITNextEventTime = 0;
    m_nState = JIT_NO_MORE_EVENT;
    m_strToolTip.clear();
	m_vecItemList.clear();
}

KGCJustInTime::~KGCJustInTime( void )
{
}

void KGCJustInTime::CreateJustInTimeToolTip( int nStringType /*= JIT_STRING_TYPE_NORMAL*/ )
{
    m_strToolTip.clear();

    if( nStringType == JIT_STRING_TYPE_NORMAL ) 
	{ 
        if ( m_nState == JIT_WAIT_NEXT_TIME ) 
		{
            CTime kTime = m_tmJITNextEventTime + g_kGlobalValue.m_tmServerSyncTime;
            int nHour = kTime.GetHour();

            m_strToolTip = boost::str( boost::wformat(L"%s - %02d:%02d")
                % g_pkStrLoader->GetString(STR_ID_EVENT_TIME)
                % nHour
                % kTime.GetMinute());
        } 
		else if ( m_nState == JIT_CLICK_ON_TO_GET_REWARD ) 
		{
            m_strToolTip = g_pkStrLoader->GetString( STR_ID_CLICK_ON_THE_CLOCK_TO_GET_THE_REWARD );
        } 
		else if ( m_nState == JIT_WAIT_REWARD )
		{
            m_strToolTip = g_pkStrLoader->GetString( STR_ID_JUST_IN_TIME_MESSAGE2 );
        } 
		else 
		{
            m_strToolTip = L"";
        }
    }
    else if( nStringType == JIT_STRING_TYPE_NOT_PCBANG_USER ) 
	{ 
        m_strToolTip = g_pkStrLoader->GetString( STR_ID_JIT_ONLY_PCBANG );
    }
    else if( nStringType == JIT_STRING_TYPE_NOT_NORMAL_USER ) 
	{ 
        m_strToolTip = g_pkStrLoader->GetString( STR_ID_JIT_ONLY_NORMAL );
    }
}

KGCJustInTime g_kJustInTime;