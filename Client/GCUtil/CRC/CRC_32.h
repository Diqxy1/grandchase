// NOTE : codeproject에 있는 CRC 코드를 변형함. (by parkch)

/****************************************************************************
CRC_32.h : file for the CRC_32 class
written by PJ Arends
pja@telus.net

based on the CRC-32 code found at
http://www.createwindow.com/programming/crc32/crcfile.htm

For updates check http://www3.telus.net/pja/crc32.htm

-----------------------------------------------------------------------------
This code is provided as is, with no warranty as to it's suitability or usefulness
in any application in which it may be used. This code has not been tested for
UNICODE builds, nor has it been tested on a network ( with UNC paths ).

This code may be used in any way you desire. This file may be redistributed by any
means as long as it is not sold for profit, and providing that this notice and the
authors name are included.

If any bugs are found and fixed, a note to the author explaining the problem and
fix would be nice.
-----------------------------------------------------------------------------
****************************************************************************/

#ifndef _CRC_32_H_
#define _CRC_32_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
//#include <string>

class CRC_32
{
public:
    static CRC_32& GetInstance( void );

    static std::string GetCRCFromBuffer( const void* pvBuf, UINT uiSize );
    static std::string GetCRCFromFile( const char* szFilePath );
    static std::string GetCRCFromString( const std::string& str );

private:
    /////////////////////////////////////////////////////////////////////////////
    //
    //  CRC_32 constructor  (private member function)
    //    Sets up the CRC-32 reference table
    //
    //  Parameters :
    //    None
    //
    //  Returns :
    //    Nothing
    //
    /////////////////////////////////////////////////////////////////////////////
    CRC_32( void )
    {
        // This is the official polynomial used by CRC-32 
        // in PKZip, WinZip and Ethernet. 
        ULONG ulPolynomial = 0x04C11DB7;

        // 256 values representing ASCII character codes.
        for (int i = 0; i <= 0xFF; i++)
        {
            Table[i] = Reflect(i, 8) << 24;
            for (int j = 0; j < 8; j++)
                Table[i] = (Table[i] << 1) ^ (Table[i] & (1 << 31) ? ulPolynomial : 0);
            Table[i] = Reflect(Table[i], 32);
        }
    }

public:
    /////////////////////////////////////////////////////////////////////////////
    //
    //  CRC_32::CalcCRC  (public member function)
    //    calculates the CRC-32 value for the given buffer
    //
    //  Parameters :
    //    buffer      [in] : a pointer to the data bytes
    //    size        [in] : the size of the buffer
    //
    //  Returns :
    //    returns the CRC-32 value of the buffer
    //    returns NULL if an error occurs
    //
    /////////////////////////////////////////////////////////////////////////////
    inline DWORD CalcCRC( const void* buffer, UINT size ) const
    {
        // check the validity of the data
        if (!buffer || !size)
            return 0;

        // calculate CRC directly
        DWORD CRC = 0xFFFFFFFF;
        Calculate(reinterpret_cast<const BYTE*>( buffer ), size, CRC);
        return CRC ^ 0xFFFFFFFF;
    }

private:
    /////////////////////////////////////////////////////////////////////////////
    //
    //  CRC_32::Calculate  (private member function)
    //    Calculates the CRC-32 value for the given buffer
    //
    //  Parameters :
    //    buffer [in] : pointer to the data bytes
    //    size   [in] : the size of the buffer
    //    CRC    [in] : the initial CRC-32 value
    //          [out] : the new CRC-32 value
    //
    //  Returns :
    //    Nothing
    //
    /////////////////////////////////////////////////////////////////////////////
    inline void Calculate( const BYTE* buffer, UINT size, ULONG& CRC ) const
    {
        // calculate the CRC
        const BYTE* pbyte = buffer;

        while (size--)
            CRC = (CRC >> 8) ^ Table[(CRC & 0xFF) ^ *pbyte++];
    }

    /////////////////////////////////////////////////////////////////////////////
    //
    //  CRC_32::Reflect  (private member function)
    //    used by the constructor to help set up the CRC-32 reference table
    //
    //  Parameters :
    //    ref [in] : the value to be reflected
    //    ch  [in] : the number of bits to move
    //
    //  Returns :
    //    the new value
    //
    /////////////////////////////////////////////////////////////////////////////
    inline ULONG Reflect( ULONG ref, char ch ) const
    {
        ULONG value = 0;

        // Swap bit 0 for bit 7
        // bit 1 for bit 6, etc.
        for(int i = 1; i < (ch + 1); i++)
        {
            if (ref & 1)
                value |= 1 << (ch - i);
            ref >>= 1;
        }
        return value;
    }

private:
    volatile ULONG Table[256];
};

#ifdef CRC
#undef CRC
#endif

#define CRC (CRC_32::GetInstance())

#endif // _CRC_32_H_