#include "stdafx.h"
#include "CurseFilter.h"
//

//

//

class KSortStringLength
{
public: 
	bool operator()( const SFilter& p, const SFilter& q ) const
	{
		return p.src.GetLength() > q.src.GetLength(); 
	}
};

KCurseFilter g_kCurseFilter;

KCurseFilter::KCurseFilter()
{
}

KCurseFilter::~KCurseFilter()
{
}

void KCurseFilter::LoadCurses(const char *filename)
{
	m_vecFilterTable.clear();
	WCHAR strLine[MAX_PATH] = {0,};	
    WCHAR seps[] = L"\t\r\n";
    WCHAR* strTemp = NULL;
	FILE* fp = NULL;
   
	if( (fp = _wfopen( GCUTIL_STR::GCStrCharToWide(filename), L"rb") ) != NULL )
	{
        fseek(fp,2,SEEK_CUR);

		while( fgetws(strLine,MAX_PATH,fp) != NULL )
		{
			SFilter filter;	

			if( strLine[0] == L';' )		//주석임
				continue;			
            strTemp = wcstok( strLine, seps );
            if ( strTemp == NULL )
                continue;			

			filter.src = StrFull2Half(strTemp); //반각으로, 또 소문자로 입력받는다.

            strTemp = wcstok( NULL, seps );
            if ( strTemp == NULL )			
				filter.dest = "";                			
			else
				filter.dest = strTemp;			

			m_vecFilterTable.push_back(filter);			
		}
	}	

	//std::sort( m_vecFilterTable.begin(), m_vecFilterTable.end(), KSortStringLength() );
}

CString KCurseFilter::StrFull2Half(CString str)
{
	CString strResult = str;
	WCHAR wStrSrc[1] = {0,};
	WCHAR wStrDesc[1] = {0,};

	#ifdef NATION_CHINA
	{
		for (int i = 0 ; i < strResult.GetLength() ; i++ )
		{
			wStrSrc[0] = strResult.GetAt(i);
			wStrDesc[0] = wStrSrc[0];

			//전각을 반각으로 바꾸는 코드 : 이건 호원형이 가르쳐준 코드 
			LCMapStringW( LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH|LCMAP_LOWERCASE , wStrSrc , 1 , wStrDesc , 1 );

			if(wStrDesc[0] != wStrSrc[0])
			{
				strResult.SetAt(i,wStrDesc[0]);
			}
		}
	}
	#endif

	strResult.MakeLower(); //대문자를 소문자로 바꾼다.
	
	return strResult;
}

void KCurseFilter::ConvertString(WCHAR* str, UINT bufSize)
{			
	// 전각 -> 반각
	CString strResult = StrFull2Half(str);	
	bool bFilter = false;

    static std::wstringstream s_strWildCard;
	for(int i = 0; i < (int)m_vecFilterTable.size(); i++)
	{		
        s_strWildCard.str(L"");
        for( int j = 0; j < m_vecFilterTable[i].src.GetLength(); ++j )
            s_strWildCard << L"*";

		if( strResult.Replace(m_vecFilterTable[i].src, s_strWildCard.str().c_str()/*m_vecFilterTable[i].dest*/) != 0 ) // 필터 걸림
			bFilter = true;	
	}	
	if( bFilter )
		StringCchCopyW( str, bufSize, (LPTSTR)(LPCTSTR)strResult );
}
