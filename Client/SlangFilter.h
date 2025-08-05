// 070504. woosh.    source code from serious-code.net
#ifndef __SLANGFILTER_H__
#define __SLANGFILTER_H__

//////////////////////////////////////////////////////////////////////////////
/// \class SlangFilter
/// \brief 비속어 필터링 클래스
/// 
/// 이 클래스는 입력된 비속어를 트리로 만들어 가지고 있는다. 비속어 문자열의 
/// 한 바이트, 한 바이트가 노드를 이루게 된다. 예를 들어, "fuck"이란 단어와 
/// "fucking"이란 단어가 입력되었다면 대강 다음과 같은 모양을 가지게 된다.
///
/// <pre>
/// f(f) - u(f) - c(f) - k(t) - i(f) - n(f) - g(t)
/// </pre>
/// 
/// 옆의 f, t는 SlangNode 클래스의 bLeafNode 멤버 변수와 매치되는 값인데,
/// t일 때, 이 노드가 한 단어의 끝임을 말한다. 즉 fuck이란 단어도 있고,
/// "fucking"이란 단어도 있음을 말한다. 위의 트리에 "fox"란 단어도 비속어로
/// 입력되었다면 다음과 같은 모양이 될 것이다.
///
/// <pre>
/// f(f) + u(f) - c(f) - k(t) - i(f) - n(f) - g(t)
///      |
///      + o(f) - x(t)
/// </pre>
/// 
/// 이런 식으로 트리를 구성해두면, 비속어 데이터베이스가 아무리 커도
/// 비속어 포함 여부를 문자열 길이의 O(N)으로 검사할 수 있다.
//////////////////////////////////////////////////////////////////////////////
#include "KNCSingleton.h"
#include "slangnode.h"
//#include <vector>

class KSlangFilter
{
    DeclareSingleton( KSlangFilter );

public:
    KSlangFilter(); /// 생성자
    ~KSlangFilter(); /// 소멸자

    enum IMPL_SLANG_FILTER { SLANG_FILTER_CHAT = 0, SLANG_FILTER_NICKNAME, NUM_OF_SLANG_FILETER };    

    std::wstring Filter(const std::wstring& original_, IMPL_SLANG_FILTER enType = SLANG_FILTER_CHAT ) const;
    bool FilterCheck( const std::wstring& original_, IMPL_SLANG_FILTER enType = SLANG_FILTER_CHAT ) const;
    bool HasSlang( const std::wstring& original_, const bool bIsNickName_ = false, IMPL_SLANG_FILTER enType = SLANG_FILTER_CHAT ) const;
    void AddSlang( const std::wstring& slang, IMPL_SLANG_FILTER enType = SLANG_FILTER_CHAT );
    bool LoadFilter( const char* szFileName, IMPL_SLANG_FILTER enType = SLANG_FILTER_CHAT );
	void LoadFilterFromKOM( const char* szFileName, int iType ,IMPL_SLANG_FILTER enType );
	void LoadFilterFromKOM( const char* szFileName, IMPL_SLANG_FILTER enType );
	void LoadFilterFromKOMBinary( std::string filename, IMPL_SLANG_FILTER enType );
    void AddNickNameLanguage( int iStart, int iEnd ); // 071116. woosh. Add Unicode 
    void AddLanguage( int iStart, int iEnd ); // 071116. woosh. Add Unicode 
    bool CheckUnicode( const std::wstring& str );
    bool CheckNickUnicode( const std::wstring& str );
    void SetEnableUnicode( bool bEnableUnicode );
    bool ValidCheckNickName( IN const std::wstring& strNickName );
    bool IsInQueryInvalidChar( const std::wstring& strString_ );
    //bool ValidCharacterInputRange( IN const std::wstring& strNickName );      //입력번위 안의 글자인지 체크합니다. (쓰지 마시오)
private:
    size_t Match( const std::wstring& text, const bool bIsNickName_ = false, IMPL_SLANG_FILTER enType = SLANG_FILTER_CHAT ) const;
    bool isPunctuation( wchar_t c ) const;


	std::vector< std::pair< int, int > > m_vecAvailableUnicode;
	std::vector< std::pair< int, int > > m_vecNickAvailableUnicode;

    bool m_bEnableUnicode;
private:
    struct IMPL;
    //IMPL* m_pImpl; ///< 내부 데이터 PIMPL
    std::vector< IMPL* > m_vtImpl;

};
DefSingletonInline( KSlangFilter );
#endif //__SLANGFILTER_H__
