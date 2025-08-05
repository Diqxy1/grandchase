#include "stdafx.h"
/// @file   KGCStringLoader.cpp
/// @author Chang-hyeon Park (pch413@kogstudios.com)
/// @since  2005-03-16 오전 10:42:07

#pragma warning(disable:4786)

//
//#include "KGCStringLoader.h"
//
//
//
#include "KGCMultipleLanguages.h"



#include "utf16_cvt.h"

#ifndef __PATH__
#include "GCUtil/GCFileMoniter.h"

void StringFileMoniter()
{
	return;
    if(SiKGCMultipleLanguages()->UseMultipleLanguages())
    {
        return;
    }

#if defined(_JAPANESE) // 일본어 (일본)
	g_pkStrLoader->Load( ".\\Stage\\str_j.stg" );
#elif defined(NATION_CHINA) // 중국어 간체 (중국)
	g_pkStrLoader->Load( ".\\Stage\\str_cn.stg" );
#elif defined(_CHINESE_TRADITIONAL) // 중국어 번체 (대만)
# ifdef NATION_HONGKONG
	g_pkStrLoader->Load( ".\\Stage\\str_hk.stg" ); // 홍콩 (클로즈드베타)
# else // !NATION_HONGKONG
	g_pkStrLoader->Load( ".\\Stage\\str_ct.stg" ); // 대만 (오픈베타/정식)
# endif // NATION_TAIWAN
# ifdef NATION_USA
	g_pkStrLoader->Load( ".\\Stage\\str_en.stg" );
# endif
#elif  defined(_THAI_LANG) // 태국어
	g_pkStrLoader->Load( ".\\Stage\\str_th.stg" );
#elif  defined(_KOREAN) // 한국어 (한국)
	g_pkStrLoader->Load( ".\\Stage\\str.stg" );
#else
    std::wstring strName = L".\\Stage\\str_";
    strName += NATION_CODE;
    strName += L".stg";
    g_pkStrLoader->Load( KncUtil::toNarrowString(strName) );

#endif
}
#endif

KGCStringLoader* g_pkStrLoader = NULL;

void KGCStringLoader::RegisterLuabind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCStringLoader>( L, "KGCStringLoader" );
		lua_tinker::class_def<KGCStringLoader>( L, "GetString",  &KGCStringLoader::GetString );;

	lua_tinker::decl( L,"g_pkStrLoader", g_pkStrLoader);
}

KGCStringLoader::KGCStringLoader( bool bIsGlobal )
: m_bIsGlobal( false )
{
    m_bIsGlobal = bIsGlobal;

    if( bIsGlobal )
        g_pkStrLoader = this;
    m_mapString.clear();
	RegisterLuabind();
#ifndef __PATH__
#if defined(_JAPANESE) // 일본어 (일본)
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_j.stg" , StringFileMoniter );
#elif defined(NATION_CHINA) // 중국어 간체 (중국)
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_cn.stg" , StringFileMoniter );
#elif defined(_CHINESE_TRADITIONAL) // 중국어 번체 (대만)
# ifdef NATION_HONGKONG
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_hk.stg" , StringFileMoniter ); // 홍콩 (클로즈드베타)
# else // !NATION_HONGKONG
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_ct.stg" , StringFileMoniter ); // 대만 (오픈베타/정식)
# endif // NATION_TAIWAN
#elif defined(_PORTUGUESE) // 포르투갈어 (포르투갈,브라질)
#ifdef NATION_BRAZIL
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_br.stg" , StringFileMoniter );
#else
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_p.stg" , StringFileMoniter );
#endif
#elif defined(_ENGLISH) // 영어 (미국,영국,필리핀)
# if defined( NATION_PHILIPPINE )
    SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_ph.stg" , StringFileMoniter );
# elif defined( NATION_EUROPE )
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_eu.stg" , StringFileMoniter );
# elif defined( NATION_THAILAND )
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_th.stg" , StringFileMoniter );
# else
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str_en.stg" , StringFileMoniter );
# endif
#elif defined(_THAI_LANG) // 태국어
	//SiKGCFileMoniter()->RegisterFile( "./Stage/str_th.stg" , StringFileMoniter );
#elif defined(_SPANISH) //스페인어
    SiKGCFileMoniter()->RegisterFile( "./Stage/str_la.stg" , StringFileMoniter );
#else // defined(_KOREAN) // 한국어 (한국)
	SiKGCFileMoniter()->RegisterFile( ".\\Stage\\str.stg" , StringFileMoniter );
#endif
#endif
}

KGCStringLoader::~KGCStringLoader( void )
{
    m_mapString.clear();

    if( m_bIsGlobal )
        g_pkStrLoader = NULL;
}

const std::wstring& KGCStringLoader::GetString( int ID )
{
    std::map<DWORD, std::wstring>::iterator mit = m_mapString.find( ID );
	if( mit == m_mapString.end() )
		return m_mapString[STR_ID_BLANK];

	return m_mapString[ID];
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
//	if( (int)m_mapString.size() <= ID || ID < 0 )
	if( m_mapString.count( ID ) == 0 )
		return m_mapString[STR_ID_BLANK];

	std::wstring original = m_mapString[ID];

	wstring strTemp = original;
	int iParamSize = strlen(szParamList);

	va_list marker;
	va_start( marker, szParamList );

	for(int i = 0; i < iParamSize; i++)
	{
		WCHAR strReplaceTarget[256] = L"";
		WCHAR strReplaceTemp[256] = L"";
		wstring strReplace = L"";

		wsprintf(strReplaceTarget, L"@%d", i + 1);
		//strReplaceTarget.Format( L"@%d", i+1 );
		// @i를 교체합니다
		switch(szParamList[i])
		{
		case 'i':			//숫자로 바꿔줌
			{			
				int iParam = va_arg( marker, int);
				strReplace = _itow(iParam, strReplaceTemp, 10);
				//strReplace.Format( L"%d", iParam );
			}
			break;
		case 'I':			//ID의 문자열로 교체해줌
			{
				int iParam = va_arg( marker, int);
				strReplace = GetString( iParam ); 
			}
			break;			
		case 's':			//char* 문자열로 교체해줌
		case 'S':
			{
				strReplace = va_arg( marker, WCHAR*);
			}
			break;
		case 'l':			//std::string 문자열로 교체해줌
		case 'L':
			{
				strReplace = va_arg( marker, std::wstring);
			}
			break;
		case 'M':
		case 'm':
			{
				//영어권 국가에서 달력의 표현이 필요하다.
				int iParam = va_arg( marker, int);
				//달력은 1~12월
				if( iParam < 1 )
					iParam = 1;
				if( iParam > 12 )
					iParam = 12;
				--iParam; //1월을 기준으로 더해줄꺼라서 1빼준다.
				strReplace = GetString( STR_ID_CARENDAL_MONTH1 + iParam );
			}
			break;
		case 'o':
		case 'O':
			{
				//영어권 국가에서 서수의 표현이 필요하다.
				int iParam = va_arg( marker, int);

				//서수의 표현은 10까지만
				if( iParam < 1 ) iParam = 1;
				if( iParam > 10 ) iParam = 10;

				--iParam;	// First부터 시작할 것이라서 1 빼준다.
				strReplace = GetString( STR_ID_ORDINARY_NUMBER1 + iParam );
			}
			break;
		case 'n':
		case 'N':
			{
				//영어권 국가에서 서수의 표현이 필요하다.
				int iParam = va_arg( marker, int);
				//서수의 표현은 1부터
				if( iParam < 1 ) iParam = 1;

				std::wstringstream stm;
				if( iParam < 4 )
					stm<<iParam<<GetString( STR_ID_ORDINARY_NUMBER_POST1 + iParam - 1 );
				else
					stm<<iParam<<GetString( STR_ID_ORDINARY_NUMBER_POST1 + 3 );

				strReplace = stm.str();
			}
			break;
        case 'd':
            {
                double fParam = va_arg( marker, double );
                swprintf_s( strReplaceTemp, 256, L"%.2f", fParam);
                strReplace = strReplaceTemp;
            }
            break;
		}

		size_t iFindPos = 0;
        while( (iFindPos = strTemp.find(strReplaceTarget)) != std::wstring::npos ){
            if(strReplace.find(strReplaceTarget) != std::wstring::npos ) break;
			strTemp.replace(iFindPos, wcslen(strReplaceTarget) , strReplace);
        }
	}
	va_end( marker );

	return strTemp;
}

bool KGCStringLoader::Load( const std::string& strFileName )
{
	std::wifstream file;
	file.open( strFileName.c_str() , std::ios::binary | std::ios::in );

	IMBUE_NULL_CODECVT( file );
	if( file.is_open() == false )
		return false;

	wchar_t GarvStr[2048];
	file.read( GarvStr, 1 );

	std::wstring str;
	while( std::getline( file, str, L'\n' ) ) 
	{
		int iIndex = _wtoi( str.substr(0, str.find( L"\t" ) ).c_str() );
		std::wstring strMissionString = str.substr( str.find( L"\t" ) + 1, str.find( L"\n" ) - 2 ); // \t 다음부터 \n 까지
		while( true )
		{
			if( strMissionString.find( L"\r" ) == strMissionString.npos )
				break;
			strMissionString.erase( strMissionString.find( L"\r" ) );
		}
		Replacer( strMissionString, L"\\n", L"\r\n" );
		m_mapString[static_cast<DWORD>(iIndex)] = strMissionString;
	}

	file.close();
	return true;
/*
	m_vecString.clear();
	m_vecString.reserve(MAX_NUM_STRING);

	FILE* fp = _wfopen( GCUTIL_STR::GCStrCharToWide( strFileName.c_str() ), L"rb" );
	fseek(fp, 2, SEEK_CUR );		//65279 라는 수를 띄어 넘자..

	WCHAR strTemp[4096];
	CString cStr;
	if( fp != NULL )
	{
		for( int i=0 ; i<MAX_NUM_STRING ; ++i )
		{
			memset( strTemp, 0, sizeof(strTemp) );
			fgetws( strTemp, 4096, fp );
			cStr = strTemp;
			cStr = &strTemp[cStr.Find(L'\t') + 1];

			WCHAR* tok;
			tok = wcstok( (LPTSTR)(LPCTSTR)cStr, L"\"\r" );

			cStr = tok;
			cStr.Replace( L"\\n", L"\r\n" );

			m_vecString.push_back((LPTSTR)(LPCTSTR)cStr);
		}
	}
	
//#define MAKE_STR_FILE
#ifdef MAKE_STR_FILE
	FILE* fp = _wfopen( L"str.txt", L"w" );

	int j=0;
	std::vector<std::wstring>::iterator itor;
	for( itor = m_vecString.begin() ; itor != m_vecString.end() ; ++itor )
	{
		if( wcscmp( (*itor).c_str(), L"" ) )
			fprintf( fp, "%3d = %s\n", j++, GCUTIL_STR::GCStrWideToChar((*itor).c_str()));
	}	
#endif //MAKE_STR_FILE

	fclose( fp );

    return true;
	*/
}

void KGCStringLoader::Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr )
{
	size_t iFindPos = 0;
	while( (iFindPos = str_.find(OldStr)) != std::wstring::npos )
		str_.replace(iFindPos, wcslen(OldStr) , NewStr);
}

//주어진 스트링 매개변수로 str.stg 에 있는 인덱스코드를 찾는다.
//시프트 키를 눌리면 스트링 대신 인덱스코드를 가져오게 하기 위함.
int KGCStringLoader::FindID( const std::wstring* strString )
{
	if(*strString == L" " || (*strString).empty() )
	{
		return -1;
	}
	std::map<DWORD, std::wstring>::iterator mit = m_mapString.begin();
	for( ; mit != m_mapString.end() ; ++mit )
	{
		std::wstring& str = mit->second;
		if( str == (*strString) )
		{
			return mit->first;
		}
	}

	return -1;
}

bool KGCStringLoader::LoadFromKom( const std::string& strFileName )
{
	return SiKGCMultipleLanguages()->LoadStringFile(strFileName, m_mapString);
}