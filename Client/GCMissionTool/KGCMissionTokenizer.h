#pragma once
//#include <string>
#include <fstream>
//#include <vector>
//#include <boost/tokenizer.hpp>
#include "utf16_cvt.h"
#include <atlstr.h>
//#include <iostream>

class KGCMissionTokenizer
{
public:
    KGCMissionTokenizer(void);
    ~KGCMissionTokenizer(void);
protected:
    std::vector< std::vector< std::wstring > > m_vecStringTok;

    int m_iIndex;
    int m_iCount;

public:
    void LoadFile( std::string strFileName );

    int GetInteger( int iIndex );
    std::wstring GetString( int iIndex );

    std::vector< std::wstring>* operator[] ( int iIndex )
    {
        if( iIndex < 0 || iIndex >= (int)m_vecStringTok.size() )
            return NULL;
        return &m_vecStringTok[iIndex];
    }
};
