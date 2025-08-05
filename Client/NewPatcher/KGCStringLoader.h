/// @file   KGCStringLoader.h
/// @author Chang-hyeon Park (pch413@kogstudios.com)
/// @since  2005-03-16 오전 10:42:07

#ifndef _KGCSTRINGLOADER_H_
#define _KGCSTRINGLOADER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "KAssert.h"
#include <string>
#include <vector>
#include "KAssert.h"

#define UNUSED_COPY_CONSTRUCTOR(c) \
        c##( const c##& ) \
        { ASSERT( false && "Do not use copy constructor!" ); }

#define UNUSED_ASSIGNMENT_OPERATOR(c) \
        const c##& operator=( const c##& ) \
        { ASSERT( false && "Do not use assignment operator!" ); return *this; }


class KGCStringLoader
{
public:	
    KGCStringLoader( void );
    ~KGCStringLoader( void );	

private:
    UNUSED_COPY_CONSTRUCTOR( KGCStringLoader );
    UNUSED_ASSIGNMENT_OPERATOR( KGCStringLoader );

public:
    bool Load( const std::string& strFileName );
    const std::wstring& GetString( int ID );
	std::wstring GetReplacedString(int ID, char* szParamList, ...);	

private:
    std::vector<std::wstring> m_vecString;
    std::wstring strNullStr;    
};


// -----------------------------------------------------------------------------
// NOTE
// -----------------------------------------------------------------------------
// If macro _JAPANESE is defined, load "Stage\\str_j.stg" file.
// If macro _CHINESE_SIMPLIFIED is defined, load "Stage\\str_cs.stg" file.
// If macro _CHINESE_TRADITIONAL is defined, load "Stage\\str_ct.stg" file.
// If nothing is defined, load "Stage\\str.stg" file.
// -----------------------------------------------------------------------------
const char*	GCStrWideToChar(const WCHAR* strSrc);
const WCHAR* GCStrCharToWide(const char* strSrc);

#endif // _KGCSTRINGLOADER_H_