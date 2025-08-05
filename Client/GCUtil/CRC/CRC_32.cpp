#include "stdafx.h"
#include "CRC_32.h"

//

CRC_32& CRC_32::GetInstance( void )
{
    static CRC_32 obj;
    return obj;
}

std::string CRC_32::GetCRCFromBuffer( const void* pvBuf, UINT uiSize )
{
    DWORD dwCRCCode = CRC.CalcCRC( pvBuf, uiSize );

    if ( 0 == dwCRCCode )
    {
        return std::string(); // failed
    }

    std::ostringstream ostrStream;
    ostrStream << std::resetiosflags( std::ios::uppercase )
               << std::hex << std::setw( 8 ) << std::setfill( '0' )
               << dwCRCCode;

    return ostrStream.str(); // succeeded
}

std::string CRC_32::GetCRCFromFile( const char* szFilePath )
{
    HANDLE  hFile       = INVALID_HANDLE_VALUE;
    HANDLE  hFileMap    = NULL;
    void*   pvFile      = NULL;

    if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFileA( szFilePath,
                                                         GENERIC_READ,
                                                         FILE_SHARE_READ,
                                                         NULL,
                                                         OPEN_EXISTING,
                                                         FILE_ATTRIBUTE_READONLY,
                                                         NULL ) ) )
    {
        return std::string(); // failed
    }

    if ( NULL == ( hFileMap = ::CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL ) ) )
    {
        ::CloseHandle( hFile );

        return std::string(); // failed
    }

    if ( NULL == ( pvFile = ::MapViewOfFile( hFileMap, FILE_MAP_READ, 0, 0, 0 ) ) )
    {
        ::CloseHandle( hFileMap );
        ::CloseHandle( hFile );

        return std::string(); // failed
    }

    DWORD dwFileSize = ::GetFileSize( hFile, NULL );
    DWORD dwCRCCode = CRC.CalcCRC( pvFile, dwFileSize );
    std::ostringstream ostrStream;
    ostrStream << std::resetiosflags( std::ios::uppercase )
               << std::hex << std::setw( 8 ) << std::setfill( '0' )
               << dwCRCCode;

    ::UnmapViewOfFile( pvFile );
    ::CloseHandle( hFileMap );
    ::CloseHandle( hFile );

    return ostrStream.str(); // succeeded
}

std::string CRC_32::GetCRCFromString( const std::string& str )
{
    DWORD dwCRCCode = CRC.CalcCRC( str.c_str(), static_cast<UINT>(str.size()) );

    if ( 0 == dwCRCCode )
    {
        return std::string(); // failed
    }

    std::ostringstream ostrStream;
    ostrStream << std::resetiosflags( std::ios::uppercase )
        << std::hex << std::setw( 8 ) << std::setfill( '0' )
        << dwCRCCode;

    return ostrStream.str(); // succeeded
}