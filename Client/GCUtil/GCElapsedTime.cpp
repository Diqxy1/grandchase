#include "stdafx.h"
#include ".\gcelapsedtime.h"

#include <dbg/dbg.hpp>

#pragma comment(lib, "winmm.lib" )

GCElapsedTime::GCElapsedTime(void) : m_dwElapsedTime( ::timeGetTime() )
{
}

GCElapsedTime::GCElapsedTime( const std::wstring& strLog ) 
: m_dwElapsedTime( ::timeGetTime() ), m_strLog( strLog )
{

}

GCElapsedTime::~GCElapsedTime(void)
{
    m_dwElapsedTime = ::timeGetTime() - m_dwElapsedTime;
    START_LOG( clog, m_strLog )
        << BUILD_LOG( m_dwElapsedTime );
}
