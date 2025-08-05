//////////////////////////////////////////////////////////////////////////
/// @file   OSInfo.h
///         interface for the COSInfo class of 'ERallyInstaller' project.
/// @author seojt@kogsoft.com
/// @since  28.08.2002
///

#ifndef _OSINFO_H_
#define _OSINFO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "KogGen.h"

//////////////////////////////////////////////////////////////////////
/// @class  COSInfo
///         OS detection class.
///
class COSInfo
{
public:
    OSVERSIONINFO   m_osvi; ///< OS version info. structure

public:
    /// constructor.
    COSInfo()
    {
        ZeroMemory( &m_osvi, sizeof(OSVERSIONINFO) );
        m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

        if( !GetVersionEx(&m_osvi) )
            ZeroMemory(&m_osvi, sizeof(OSVERSIONINFO));
    }//COSInfo()

    /// destructor.
    ~COSInfo(){}

    __forceinline BOOL IsNT() const
    {
        return m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }//IsNT()

    __forceinline BOOL IsWindows95() const
    {
        // Windows95 when
        // Major == 4 and Minor == 0 and PlatformId != NT
        return m_osvi.dwMajorVersion == 4
            && m_osvi.dwMinorVersion == 0
            && m_osvi.dwPlatformId != VER_PLATFORM_WIN32_NT;
    }//IsWindows95()

    __forceinline BOOL IsWindows98() const
    {
        // Windows98 when
        // Major >= 4 and Minor > 0 and PlatformId != NT
        // (except Major == 4 and Minor == 90 which is ME)
        return m_osvi.dwMajorVersion >= 4
            && m_osvi.dwMinorVersion > 0
            && m_osvi.dwPlatformId != VER_PLATFORM_WIN32_NT
            && !(m_osvi.dwMajorVersion == 4 &&    m_osvi.dwMinorVersion == 90);
    }//IsWindows98()

    __forceinline BOOL IsWindowsME() const
    {
        // WindowsME when
        // Major == 4 and Minor == 90 and PlatformId != NT
        return m_osvi.dwMajorVersion == 4
            && m_osvi.dwMinorVersion == 90
            && m_osvi.dwPlatformId != VER_PLATFORM_WIN32_NT;
    }//IsWindowsME()

    __forceinline BOOL IsWindowsNT() const
    {
        // WindowsNT4 when
        // Major == 4 and Minor == 0 and PlatformId == NT
        return m_osvi.dwMajorVersion == 4
            && m_osvi.dwMinorVersion == 0
            && m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }//IsWindowsNT()

    __forceinline BOOL IsWindows2K() const
    {
        // Windows2000 when
        // Major == 5 and Minor == 0 and PlatformId == NT
        return m_osvi.dwMajorVersion == 5
            && m_osvi.dwMinorVersion == 0
            && m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }//IsWindows2K()

    __forceinline BOOL IsWindowsXP() const
    {
        // WindowsXP when
        // Major == 5 and Minor == 1 and PlatformId == NT
        return m_osvi.dwMajorVersion == 5
            && m_osvi.dwMinorVersion == 1
            && m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }//IsWindowsXP()
};//class COSInfo

#endif // _OSINFO_H_
