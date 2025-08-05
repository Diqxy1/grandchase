/// @file   KGCStringLoader.cpp
/// @author Chang-hyeon Park (pch413@kogstudios.com)
/// @since  2005-03-16 오전 10:42:07

#pragma warning(disable:4786)
#include "stdafx.h"
#include "KGCStringLoader.h"
#include "KStateInfo.h"
#include <sstream>
#include <fstream>
#include <atlstr.h>
#include <locale>
#include "gcutil.h"


KGCStringLoader::KGCStringLoader( void )
{    
    m_vecString.clear();
    strNullStr = L"NULL";
}

KGCStringLoader::~KGCStringLoader( void )
{
    m_vecString.clear();
}

const std::wstring& KGCStringLoader::GetString( int ID )
{
    if( (int)m_vecString.size() <= ID || ID < 0 )
    {
        if( m_vecString.size() <= STR_ID_BLANK )
            return strNullStr;
		return m_vecString[STR_ID_BLANK];
    }

    return m_vecString[ID];
}

//===============================================================================
// std::string KGCStringLoader::GetReplacedString(int ID, char* szParamList, ...)
// 의 szParamList가 나타내는것
//
//i			숫자로 바꿔줌
//I			ID의 문자열로 교체해줌
//s or S	char* 문자열로 교체해줌
//l or L	std::string 문자열로 교체해줌

//  1672 = "캐릭터 @1 획득에 실패하였습니다"
//  str1700 = "라스"

//  GetReplacedString( 1672 , "s" , "라스" ); -->   캐릭터 라스 획득에 실패하였습니다
//  GetReplacedString( 1672 , "I" , 1700 );   -->   캐릭터 라스 획득에 실패하였습니다

//  1641 = "@1차직업 @2단필살기"
//  GetReplacedString( 1641 , "ii" , 1, 1 );   -->  1차직업 1단필살기

//===============================================================================
std::wstring KGCStringLoader::GetReplacedString(int ID, char* szParamList, ...)
{
	if( (int)m_vecString.size() <= ID || ID < 0 )
		return m_vecString[STR_ID_BLANK];

	std::wstring original = m_vecString[ID];

	CString strTemp = original.c_str();
	CString strReplaceTarget;
	CString strReplace;

	int iParamSize = strlen(szParamList);

	va_list marker;
	va_start( marker, szParamList );

	for(int i = 0; i < iParamSize; i++)
	{
		strReplaceTarget.Format( L"@%d", i+1 );
		// @i를 교체합니다
		switch(szParamList[i])
		{
		case 'i':			//숫자로 바꿔줌
			{			
				int iParam = va_arg( marker, int);
				strReplace.Format( L"%d", iParam );
			}
			break;
		case 'I':			//ID의 문자열로 교체해줌
			{
				int iParam = va_arg( marker, int);
				std::wstring str = GetString( iParam );
				strReplace = str.c_str();
			}
			break;			
		case 's':			//char* 문자열로 교체해줌
		case 'S':
			{
				WCHAR* str = va_arg( marker, WCHAR*);
				strReplace = str;
			}
			break;
		case 'l':			//std::string 문자열로 교체해줌
		case 'L':
			{
				std::wstring str = va_arg( marker, std::wstring);
				strReplace = str.c_str();
			}
			break;
		}

		strTemp.Replace( strReplaceTarget, strReplace );
	}
	va_end( marker );

	std::wstring strret = strTemp;

	return strret;
}

bool KGCStringLoader::Load( const std::string& strFileName )
{
	m_vecString.reserve(MAX_NUM_STRING);

    const WCHAR* wszFileName = GCStrCharToWide( strFileName.c_str() );
    std::wstring wstrUprFileName;
    std::wstring wstrLwrFileName;
    wstrUprFileName.append( wszFileName );
    wstrLwrFileName.append( wszFileName );
    GCUTIL_STR::ToUpper(  wstrUprFileName );
    GCUTIL_STR::ToLower(  wstrLwrFileName );
	FILE* fp ;
	_wfopen_s(&fp, wszFileName , L"rb" );
    if( fp == NULL )
    {
         _wfopen_s(&fp, wstrUprFileName.c_str(), L"rb" );
        
        if( fp == NULL )
            _wfopen_s(&fp, wstrLwrFileName.c_str(), L"rb" );
    }
	fseek(fp, 2, SEEK_CUR );		//65279 라는 수를 띄어 넘자..

	WCHAR strTemp[2048];
	CString cStr;
	if( fp != NULL )
	{
		for( int i=0 ; i<MAX_NUM_STRING ; ++i )
		{
			memset( strTemp, 0, sizeof(strTemp) );
			fgetws( strTemp, 2048, fp );
			cStr = strTemp;
			cStr = &strTemp[cStr.Find(L'\t') + 1];

			WCHAR* tok;
			tok = wcstok( (LPTSTR)(LPCTSTR)cStr, L"\"\r" );

			cStr = tok;
			cStr.Replace( L"\\n", L"\r\n" );

			m_vecString.push_back((LPTSTR)(LPCTSTR)cStr);
		}
	}
	fclose( fp );

//#define MAKE_STR_FILE
#ifdef MAKE_STR_FILE
	FILE* fp = _wfopen_s( L"str.txt", L"w" );

	int j=0;
	std::vector<std::wstring>::iterator itor;
	for( itor = m_vecString.begin() ; itor != m_vecString.end() ; ++itor )
	{
		if( wcscmp( (*itor).c_str(), L"" ) )
			fprintf_s( fp, "%3d = %s\n", j++, GCStrWideToChar((*itor).c_str()));
	}

	fclose( fp );
#endif //MAKE_STR_FILE
    return true;
}
inline std::string toNarrowString( const wchar_t* pStr , int len = -1 )
{
    std::string buf ;
    if( pStr == NULL )
        return buf;
    if( len != -1 && len <= 0 ) // florist. 0이어도 통과시키지 말자.
        return buf;

    // figure out how many narrow characters we are going to get 
    int nChars = WideCharToMultiByte( CP_ACP , 0 , 
        pStr , len , NULL , 0 , NULL , NULL ) ; 
    if ( len == -1 )
        -- nChars ; 
    if ( nChars == 0 )
        return "" ;

    // convert the wide string to a narrow string
    // nb: slightly naughty to write directly into the string like this
    buf.resize( nChars ) ;
    WideCharToMultiByte( CP_ACP , 0 , pStr , len , 
        const_cast<char*>(buf.c_str()) , nChars , NULL , NULL ) ; 

    return buf ; 
}

inline std::string toNarrowString( const std::wstring& str )
{
    return toNarrowString( str.c_str() );
}

inline std::wstring toWideString( const char* pStr , int len = -1 )
{
    std::wstring buf ;
    if( pStr == NULL )
        return buf;
    if( len != -1 && len <= 0 ) // florist. 0이어도 통과시키지 말자.
        return buf;

    // figure out how many wide characters we are going to get 
    int nChars = MultiByteToWideChar( CP_ACP , 0 , pStr , len , NULL , 0 ) ; 
    if ( len == -1 )
        -- nChars ; 
    if ( nChars == 0 )
        return L"" ;

    // convert the narrow string to a wide string 
    // nb: slightly naughty to write directly into the string like this
    buf.resize( nChars ) ; 
    MultiByteToWideChar( CP_ACP , 0 , pStr , len , 
        const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

    return buf ;
}

inline std::wstring toWideString( const std::string& str )
{
    return toWideString( str.c_str() );
}

const char*	GCStrWideToChar(const WCHAR* strSrc)
{
    static char strTemp[4096];
    memset( strTemp, 0, sizeof(char)*4096 );
    std::wstring strWide = strSrc;
    std::string strNarrow = toNarrowString( strWide );
    lstrcpyA( strTemp, strNarrow.c_str() );

    return strTemp;
}

const WCHAR* GCStrCharToWide(const char* strSrc)
{
    static WCHAR strTemp[4096];
    memset( strTemp, 0, sizeof(WCHAR)*4096 );
    std::string strNarrow = strSrc;
    std::wstring strWide = toWideString( strNarrow );
    lstrcpyW( strTemp, strWide.c_str() );

    return strTemp;
}