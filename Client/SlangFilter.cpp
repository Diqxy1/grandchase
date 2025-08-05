#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////////
/// 070430. woosh.
///
/// 비속어 필터링
//////////////////////////////////////////////////////////////////////////////
//
//
//

//
#include "utf16_cvt.h"
//
#include "SlangFilter.h"


#ifndef SAFE_DELETE
    #define SAFE_DELETE(p)          if (p) { delete (p); (p) = NULL; }
#endif


namespace
{
    class KComp
    {
    public:
        bool operator() ( const std::pair<int,int> pr1, const std::pair<int,int> pr2 )
        {
            if( pr1.first < pr2.first )
                return true;
            return false;
        }

        bool operator() ( const int nVal, const std::pair<int,int> pr)
        {
            if( nVal < pr.first && nVal < pr.second )
                return true;
            return false;
        }

        bool operator() ( const std::pair<int,int> pr, const int nVal )
        {
            if( nVal > pr.first && nVal > pr.second )
                return true;
            return false;
        }
    };
};

ImplementSingleton( KSlangFilter );
//////////////////////////////////////////////////////////////////////////////
/// \struct SlangFilter::IMPL
/// \brief SlangFilter 클래스 내부 데이터 구조체
//////////////////////////////////////////////////////////////////////////////
struct KSlangFilter::IMPL
{
    KSlangNode* pRoot; ///< 최상위 노드

    static const std::wstring s_Punctuations; ///< 문장 부호들
    IMPL() : pRoot( new KSlangNode ) { } //AssertPtr(pRoot);
    ~IMPL() { SAFE_DELETE(pRoot); }
};

/// 문장 부호들
const std::wstring KSlangFilter::IMPL::s_Punctuations = L" `~!@#$%^&*()-_=+\\|[{]};:'\",<.>/?";

KSlangFilter::KSlangFilter() /// 생성자
: m_bEnableUnicode( false )
{
    m_vtImpl.clear();

    for(int i=0;i<NUM_OF_SLANG_FILETER;++i)
    {
        m_vtImpl.push_back( new IMPL );
    }
}
KSlangFilter::~KSlangFilter() 
{
    for(int i=0;i<(int)m_vtImpl.size();++i)
    {
        m_vtImpl[i]->pRoot->DeleteChild();
        SAFE_DELETE( m_vtImpl[i] );
    }
} /// 소멸자


void KSlangFilter::AddSlang( const std::wstring& slang_, IMPL_SLANG_FILTER enType /*= SLANG_FILTER_CHAT*/ )
{
	if( (int)m_vtImpl.size() <= (int)enType )
		return;

	KSlangNode* pCurrent = m_vtImpl[(int)enType]->pRoot; //최상위 노드를 정해줘야 한다

	for( unsigned int i = 0 ; i < slang_.length() ; ++i )
	{
		//if( pCurrent->isLeafNode() )
		//{
		//    return;
		//}
		pCurrent = pCurrent->AddChild( slang_[i] );
	}
	pCurrent->setLeafNode(true); 
}

size_t KSlangFilter::Match( const std::wstring& text_, const bool bIsNickName_, IMPL_SLANG_FILTER enType /*= SLANG_FILTER_CHAT*/ ) const
{
	if( (int)m_vtImpl.size() <= (int)enType )
	return 0;

	KSlangNode* pCurrent = m_vtImpl[(int)enType]->pRoot;
	size_t i = 0;
	size_t iPre = 0;

	bool bIsCurseWord = false;

    while( i < text_.size() )
    {
        if(	isPunctuation( text_[i] ) && !bIsNickName_ ) //
        {
            //이부분 null체크 혹시나 제거하면 큰일납니다!!
            if( pCurrent != NULL ){
                if ( NULL !=  pCurrent->FindChild( text_[i] ) )
                    pCurrent = pCurrent->FindChild( text_[i] );

                if( pCurrent == NULL ) {
                    return iPre;
                }

                if ( pCurrent->isLeafNode() ) {
                    iPre = i + 1;
                }
            }
            //목록에 있는 금지어라면 특수문자가 끼어있어도 넘기지 말고 다음노드로 이동한다.
            ++i;
            continue;
        }

        bIsCurseWord = true;

        //이부분 null체크 혹시나 제거하면 큰일납니다!!
        if( pCurrent == NULL ) return iPre;

        pCurrent = pCurrent->FindChild( text_[i] );

        if( pCurrent == NULL ) {
            return iPre;
        }

        if ( pCurrent->isLeafNode() ) {
            iPre = i + 1;
        }

        ++i;
    }

    //위 반복문을 무사히 마치고 여기까지 왔다면 일단 
    //끝 글자 앞 글자까지는 금지어임에 틀림없고, 끝 문자가 특수문자인 경우이다.
    //목록에서 다시한번 금지어 인지를 검사한다.
    if( i-1 == text_.size() -1 && bIsCurseWord )
    {
        if( pCurrent == NULL ) return iPre;

        if( pCurrent->isLeafNode() ) return i;
    }

    return iPre;
}

std::wstring KSlangFilter::Filter( const std::wstring& original_, IMPL_SLANG_FILTER enType /*= SLANG_FILTER_CHAT*/ ) const
{
    std::wstring text( original_ );
	std::wstring strTemp = original_;
	GCUTIL_STR::ToLower( strTemp );

    for( size_t i=0; i<original_.size(); )
    {
		size_t size= Match( strTemp.substr(i, strTemp.size() -i), false, enType );
        if( size > 0 )
        {
            text.replace( i, size, std::wstring(size, '*') ); //한글은 글자당 *를 하나만 할 수 있도록 수정하기(unicode, done)
            i += size;
        }
        else
        {
            i += 1; // for unicode , if not, use next  "i += original[i] & 0x80 ? 2 : 1;"
        }
    }
    return text;
}

bool KSlangFilter::FilterCheck( const std::wstring& original_, IMPL_SLANG_FILTER enType /*= SLANG_FILTER_CHAT*/ ) const
{
    if ( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) 
        return true;

    std::wstring text( original_ );
    std::wstring strTemp = original_;
    GCUTIL_STR::ToLower( strTemp );

    for( size_t i=0; i<original_.size(); )
    {
        size_t size= Match( strTemp.substr(i, strTemp.size() -i), false, enType );
        if( size > 0 )
        {
            return false;
        }
        else
        {
            i += 1; // for unicode , if not, use next  "i += original[i] & 0x80 ? 2 : 1;"
        }
    }
    return true;
}

bool KSlangFilter::HasSlang( const std::wstring& original_, const bool bIsNickName_, IMPL_SLANG_FILTER enType /*= SLANG_FILTER_CHAT*/ ) const// Does this Nickname include vulgarity word(s)?
{    
    for( size_t i=0 ; i < (int)original_.size() ; i++)
    {
        if ( Match(original_.substr(i, original_.size() - i), bIsNickName_, enType ) > 0 )
            return true;
    }
    return false;
}

bool KSlangFilter::isPunctuation( wchar_t c ) const
{
#ifdef NATION_CHINA
	if ( ( c < 0x2e80 || c > 0x2eff ) &&
		( c < 0x3130 || c > 0x318f ) &&
		( c < 0x31c0 || c > 0x31ef ) &&
		( c < 0x3200 || c > 0x32ff ) &&
		( c < 0x3300 || c > 0x33ff ) &&
		( c < 0x3400 || c > 0x34df ) &&
		( c < 0x4e00 || c > 0x9fbf ) &&
		( c < 0xa960 || c > 0xa97f ) &&
		( c < 0xac00 || c > 0xd7af ) &&
		( c < 0xd7b0 || c > 0xd7ff ) &&
		( c < 0xfe30 || c > 0xfe4f ) &&
		( c < 0x20000 || c > 0x2a6df ) &&
		( c < 0x2a700 || c > 0x2b73f ) &&
		( c < 0x2f800 || c > 0x2fa1f ) &&
		( c < 0x1100 || c > 0x11ff ) &&
		( c < 0x30 || c > 0x39 ) &&
		( c < 0x41 || c > 0x7a )
		)
		return true;

#endif

    if( IMPL::s_Punctuations.find(c) != std::wstring::npos )
        return true;
    return false;
}

bool KSlangFilter::LoadFilter( const char* szFileName, IMPL_SLANG_FILTER enType /*= SLANG_FILTER_CHAT*/ )
{
    std::wifstream  m_wFile;
    IMBUE_NULL_CODECVT( m_wFile );
    m_wFile.open( szFileName, std::ios::binary | std::ios::in );

    std::wstring strOut;

    if( !m_wFile.is_open() )
    {
        std::cout << "can't file open" << std::endl;
        return false;
    }
    wchar_t chBom[1];
    m_wFile.read( chBom, 1 );
    //std::cout <<"Bom: " << *chBom << std::endl; // 봄 내용 읽고, little endian? 아니면 return
    if( *chBom != 0xFEFF ) // FEFF = 65279 (little endian), FFFE = 65534 (Big endian)
    {
        return false;
    }

    while( std::getline( m_wFile, strOut, L'\n' ) ) 
    {
        boost::trim( strOut );
        if( strOut.empty() )
            continue;

        strOut = boost::to_lower_copy( strOut );
        if( strOut[0] != '/') // '/' 주석은 don't input in the map
        {
            AddSlang( strOut, enType );
        }
    }
    return true;
}

void KSlangFilter::LoadFilterFromKOM( const char* szFileName, IMPL_SLANG_FILTER enType /*= SLANG_FILTER_CHAT*/ )
{
    KLuaManager luaMgr;
    int i;

    if( GCFUNC::LoadLuaScript( luaMgr, szFileName) == false)
    {
        return;
    }	

    luaMgr.BeginTable( "SLANG_LIST" );
    {
        for(i=1 ; ; ++i )
        {		
            std::string strOut;
            LUA_GET_VALUE_NOASSERT( i, strOut, break );

			boost::trim( strOut );
            strOut = boost::to_lower_copy( strOut );
            if( strOut[0] != '/') // '/' 주석은 don't input in the map
            {
                AddSlang( GCUTIL_STR::GCStrCharToWide(strOut.c_str()), enType );
            }
        }			
    }
    luaMgr.EndTable();
}

void KSlangFilter::LoadFilterFromKOM( const char* szFileName, int iType ,IMPL_SLANG_FILTER enType )
{
	if ( iType == 0 )
	{
		LoadFilterFromKOM(szFileName,enType);
		return;
	}
	else {
		LoadFilterFromKOMBinary(szFileName,enType);
		return;
	}



}

void KSlangFilter::LoadFilterFromKOMBinary( std::string filename, IMPL_SLANG_FILTER enType )
{
	boost::shared_array<char> filedata;
	int length = 0;
	if(g_pGCDeviceManager2->GetMassFileManager()->LoadMemoryFile(filename, filedata,length) == false)
		return;

	WCHAR *pWCHAR = new WCHAR[length];
	memcpy(pWCHAR,filedata.get(),length);

	std::wstring str;
	int index = 0;
	int iline = 0;
	while(SiKGCMultipleLanguages()->GetLine( pWCHAR,index,L'\n',str ) && index < length)
	{
		std::wstring strDataString = str.substr( 0, str.find( L"\n" ) - 1 ); // \t 다음부터 \n 까지
		boost::trim( strDataString );
		GCUTIL_STR::ToLower( strDataString );
		boost::replace_all(strDataString,L" ",L"");
		if( strDataString.empty() ){
			iline++;
			continue;
		}
		if( strDataString[0] != '/') // '/' 주석은 don't input in the map
		{
			AddSlang( strDataString, enType );
		}
		iline++;
	}

	delete pWCHAR;

}



void KSlangFilter::AddLanguage( int iStart_, int iEnd_ ) // 071116. woosh. Add Unicode 
{
    m_vecAvailableUnicode.push_back( std::make_pair( iStart_, iEnd_ ) );

    //std::sort( m_vecAvailableUnicode.begin(), m_vecAvailableUnicode.end(), KComp() );
}

void KSlangFilter::AddNickNameLanguage( int iStart_, int iEnd_ ) // 071116. woosh. Add Unicode 
{
    m_vecNickAvailableUnicode.push_back( std::make_pair( iStart_, iEnd_ ) );

    //std::sort( m_vecNickAvailableUnicode.begin(), m_vecNickAvailableUnicode.end(), KComp() );
}

void KSlangFilter::SetEnableUnicode( const bool bEnableUnicode_ )
{
    // true 이면 벡터에 넣은 유니코드 값들을 허용, false이면 등록된 유니코드 필터
    m_bEnableUnicode = bEnableUnicode_;
}

bool KSlangFilter::CheckUnicode( const std::wstring& str ) // 071119. woosh.
{
    int iCount = 0;

    DWORD dwMask;
    if( m_bEnableUnicode )
        dwMask = 2; // 10 , 허용 유니코드
    else
        dwMask = 1; // 01 , 차단 유니코드
 
    for( unsigned int i = 0; i < str.size(); ++i )
    {
        DWORD dwValue = 1; // 01

        if( true == std::binary_search( m_vecAvailableUnicode.begin(), m_vecAvailableUnicode.end(), (int)str[i], KComp() ) ) // 080324 tslayer. 해당 문자가 설정한 유니코드 리스트 중에 있는가.
        {
            dwValue = 2; // 10
        }

        if( (dwValue & dwMask) <= 0 )
        {
            return true; // 걸렸네
        }
    }
    return false;
}

bool KSlangFilter::CheckNickUnicode( const std::wstring& str )
{
    return false;
}

//입력범위 안의 글자인지 체크합니다.
//bool KSlangFilter::ValidCharacterInputRange( IN const std::wstring& strNickName ) 
//{
//#if !defined(NATION_KOREA)
//    return true;
//#endif
//
//    if ( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) 
//        return true;
//    //          숫자	    영대	    영소	    한글
//    //아스키    0030~0039	0041~005A	0061~007A	
//    //유니코드  FF10~FF19	FF21~FF3A	FF41~FF5A   AC00~D7AF
//    //////////////////////////////////////////////////////////////////////////
//    //한글 이외의 유니코드 문자를 허용하면 db에서 검색할 수가 없다.
//    //한글도 일부분은 db에서 검색할 수 없지만 목록이 나오기 전까지 작업을 미룬다.
//
//    for ( unsigned int i = 0 ; i < strNickName.length() ; i++ ) {
//        if ( !(
//            (strNickName[i] >= 0x0030 && strNickName[i] <= 0x0039 ) ||  //숫자범위(아스키)
//            //(strNickName[i] >= 0xff10 && strNickName[i] <= 0xff19 ) ||  //숫자범위(유니코드)
//            (strNickName[i] >= 0x0041 && strNickName[i] <= 0x005a ) ||  //영대문자(아스키)
//            //(strNickName[i] >= 0xff21 && strNickName[i] <= 0xff3a ) ||  //영대문자(유니코드)
//            (strNickName[i] >= 0x0061 && strNickName[i] <= 0x007a ) ||  //영소문자(아스키)
//            //(strNickName[i] >= 0xff41 && strNickName[i] <= 0xff5a ) ||  //영소문자(유니코드)
//            (strNickName[i] >= 0xac00 && strNickName[i] <= 0xd7af )     //한글(유니코드)
//            ) )
//        {
//            //strNickName[i] 글자가 입력 범위를 넘어섰음
//            return false;
//        }
//    }
//    return true;
//}

bool KSlangFilter::IsInQueryInvalidChar( const std::wstring& strString_ )
{
    static wchar_t strProhibition[] ={ L' ', 9, L'\'', L'\"', L',', L'　', 10,13  };

    return ( std::find_first_of( strString_.begin(), strString_.end(),
        strProhibition, strProhibition + ( sizeof(strProhibition) / sizeof(wchar_t) ) ) != strString_.end() );
}


bool KSlangFilter::ValidCheckNickName( IN const std::wstring& strNickName )
{
    if ( IsInQueryInvalidChar( strNickName ) )
    {
        return false;
    }
    
    if ( CheckNickUnicode( strNickName ))
    {
        return false;
    }
    
    return true;
}
