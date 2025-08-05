#ifndef __SSERVERINFO__
#define __SSERVERINFO__

#include "stdafx.h"
struct SServerInfo
{
    wchar_t    m_strIP[24];
    DWORD      m_dwPort;
    int        m_iVersion;
    wchar_t    m_strName[20];
    
    SServerInfo( void )
    {
        ::ZeroMemory( this, sizeof( SServerInfo ) );
    }
    SServerInfo( const SServerInfo& src )
    {
        *this = src;
    }
    ~SServerInfo( void )
    {
        // empty
    }
    const SServerInfo& operator=( const SServerInfo& src )
    {
        wcscpy( m_strIP, src.m_strIP );
        wcscpy( m_strName, src.m_strName );
        m_dwPort      = src.m_dwPort;
        m_iVersion    = src.m_iVersion;
        return *this;
    }
};

#endif // __SSERVERINFO__