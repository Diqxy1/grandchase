/// @file   KGCStringLoader.h
/// @author Chang-hyeon Park (pch413@kogstudios.com)
/// @since  2005-03-16 오전 10:42:07

#ifndef _KGCSTRINGLOADER_H_
#define _KGCSTRINGLOADER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable:4786)

//#include "define.h"
//#include "KAssert.h"
////#include <string>
////#include <vector>
////#include <map>
//#include "KLuabinder.h"

#include "KGCStringID.h"
#include "KGCStringID2.h"
#include "KGCStringID3.h"
#include "KGCStringID4.h"

class KGCStringLoader : public lua_tinker::lua_value
{
public:
	KDeclareLuaInvoke
    KGCStringLoader( bool bIsGlobal );
    ~KGCStringLoader( void );
	static void RegisterLuabind();

private:
    UNUSED_COPY_CONSTRUCTOR( KGCStringLoader );
	UNUSED_ASSIGNMENT_OPERATOR( KGCStringLoader );
	

public:
    bool Load( const std::string& strFileName );
    bool LoadFromKom( const std::string& strFileName );
    const std::wstring& GetString( int ID );
	std::wstring GetReplacedString(int ID, char* szParamList, ...);
	void Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr );

	int FindID( const std::wstring* strString );
    std::map<DWORD, std::wstring> m_mapString;
	
private:
    bool m_bIsGlobal;
};

extern KGCStringLoader* g_pkStrLoader;

// -----------------------------------------------------------------------------
// NOTE
// -----------------------------------------------------------------------------
// If macro _JAPANESE is defined, load "Stage\\str_j.stg" file.
// If macro _CHINESE_SIMPLIFIED is defined, load "Stage\\str_cs.stg" file.
// If macro _CHINESE_TRADITIONAL is defined, load "Stage\\str_ct.stg" file.
// If nothing is defined, load "Stage\\str.stg" file.
// -----------------------------------------------------------------------------

#endif // _KGCSTRINGLOADER_H_
