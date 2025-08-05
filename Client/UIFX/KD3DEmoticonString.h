#pragma once
//#include <string>
////#include <vector>
#include <utility>
//#include <sstream>
#include <KNCSingleton.h>

#define EMOTICON_SEPRATOR L'/'
#define EMOTICON_SPACE L' '
#define EMOTICON_TAB L'\t'
#define EMOTICON_NEWLINE L'\n'

const int EMOTICON_UI_SIZE = 36;

class KGCEmoticon;
class GCDeviceFont;

class KD3DEmoticonString
{
    DeclareSingleton( KD3DEmoticonString );

public:
    // < 이모티콘?, emoticonptr, string > 쌍
    typedef std::pair< bool, std::pair< KGCEmoticon*, std::wstring > > EmotStrPair;

    KD3DEmoticonString(void);
    virtual ~KD3DEmoticonString(void);

	std::vector<KGCEmoticon*> GetEmoticonDataVectorFromString( const std::wstring& strContent, int iIDGapSize , std::wstring& strNoEmoticon);
    void DivideStringPiece( const std::wstring& strContent, int iIDGapSize = 0);
    void Arrange( GCDeviceFont* pkFontMgr, const int iWidth, const std::wstring& strIDGap );
    void ArrangeForWordWrap( GCDeviceFont* pkFontMgr, const int iWidth, const std::wstring strIDGap );
	void ArrangeForThaiWordWrap( const std::wstring& strContent, GCDeviceFont* pkFontMgr, const int iWidth, const std::wstring strIDGap ,std::vector<KGCEmoticon*> vecEmoticons );

    void Dump( std::wstringstream& stm );
    void clear();

    const std::vector< std::vector< KD3DEmoticonString::EmotStrPair > >& GetPieces() { return m_vecStrPieces; }
    bool IsEmoticonHasMessage( const std::wstring& strContent );
    void GetDivideString( std::vector< KD3DEmoticonString::EmotStrPair >& vecStrDivide ) { vecStrDivide = m_vecStrDivide; }

    void CheckForSend( std::wstring& strContent );

private:
    std::vector< KD3DEmoticonString::EmotStrPair > m_vecStrDivide;
    std::vector< std::vector< KD3DEmoticonString::EmotStrPair > > m_vecStrPieces;
};

DefSingletonInline( KD3DEmoticonString );
