#pragma once

#include "GCDynamicTexture.h"

#define MAX_FONT_SIZE               (128)
#define FONT_CACHE_SIZE             (256)
#define FONT_CACHE_PAGE             (4)
#define VIRTUAL_FONT_LINE_MAX       (3000)
#define MAX_CHAR_IN_PAGE            (((int)FONT_CACHE_SIZE/12)*((int)FONT_CACHE_SIZE/12)) //12 X 12를 최소로 생각함
#define MAX_VERTEX_IN_PAGE          (MAX_CHAR_IN_PAGE*4)

#define DT_NO_COLOR                 (0xF0000000)

#define _16BIT_FONT
#ifdef _16BIT_FONT

//D3DCOLOR_A1R5G5B5
#define MYD3DCOLOR_ARGB(a,r,g,b)    (WORD)( ((a&0x1) << 15) | ((r&0x1F)<<10) | ((g&0x1F) << 5) | (b&0x1F) )
#define MYFONTCOLORFMT              (D3DFMT_A1R5G5B5)
#define MYFONTCOLOR_PTR             WORD*
#define MYFONTCOLOR_ALPHA(col)      ( ( col & (0x1 << 15) ) >> 15 )
#else
#define MYD3DCOLOR_ARGB(a,r,g,b)    D3DCOLOR_ARGB(a,r,g,b)
#define MYFONTCOLORFMT              (D3DFMT_A8R8G8B8)
#define MYFONTCOLOR_PTR             DWORD*
#define MYFONTCOLOR_ALPHA(col)      ( ( col & 0xff000000 ) >> 24 )
#endif

class GCFontChunk {
public:
	GCFontChunk() {}
	~GCFontChunk() {}

	void SetString( ID3DXFont* pFont, std::wstring& str_, D3DCOLOR color ) {
		std::wstring strLength = str_;
		std::replace( strLength.begin(), strLength.end(), L' ', L'0' );
		RECT rc;
		pFont->DrawTextW( NULL, strLength.c_str(), -1, &rc, DT_CALCRECT, color );
		m_iWidth = rc.right - rc.left;
		m_str = str_;
		m_color = color;
	}
	void SetRect( ID3DXFont* pFont, int x, int y ) {
		RECT rc;
		::SetRect( &rc, x, y, 0, 0 );
		pFont->DrawTextW( NULL, m_str.c_str(), m_str.size(), &rc, DT_CALCRECT, 0xffffffff );
		m_rc = rc;
	}

	int m_iWidth;
	RECT m_rc;
	std::wstring m_str;
	D3DCOLOR m_color;
};

class GCStringChunk {
public:
	GCStringChunk() {
		m_colorOutline = 0xff000000;
		m_bOutLine = false;
	}
	~GCStringChunk() {}

	void SetString( ID3DXFont* pFont, const wchar_t* str_, int iLength_ = -1,
		D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutline = 0xff000000,
		RECT* pRt = NULL, DWORD dwFlag = 0, float fLineSpace = 0.f, bool bOutLine = false ) {
			if( iLength_ == -1 ) {
				iLength_ = (int)wcslen( str_ );
			}
			m_bOutLine = bOutLine;

			std::wstringstream stm;
			D3DCOLOR curColor = color;
			m_colorOutline = colorOutline;
			while( (*str_) != L'\0' ) {
				if( (*str_) == L'#' ) {
					PushChunk( pFont, stm, curColor );
					GetColorKey( str_, curColor, color );
				}
				else if( (*str_) == L'\r' ) {
				}
				else if( (*str_) == L'\n' ) {
					PushChunk( pFont, stm, curColor );
					PushNewline();
				}
				else {
					stm<<(*str_);
				}
				++str_;
			}
			if( stm.str().empty() == false ) {
				PushChunk( pFont, stm, curColor );
			}
			CalculatePosition( pFont, pRt, fLineSpace, dwFlag );
	}
	void CalculatePosition( ID3DXFont* pFont, RECT* pRt, float fLineSpace, DWORD dwFlag ) {
		if( pRt == NULL )
			return;

		int iTop = pRt->top;
		for( int iLine = 0; iLine < (int)m_vecLineChunk.size() ; ++iLine ) {
			std::vector<GCFontChunk>& vecChunk = m_vecLineChunk[iLine];
			int iWidth = GetLineWidth( iLine );
			int iRectWidth = pRt->right - pRt->left;
			int iLeft = pRt->left;

			if( dwFlag & DT_CENTER ) {
				iLeft = pRt->left + (iRectWidth/2) - (iWidth/2);
			}
			else if( dwFlag & DT_RIGHT ) {
				iLeft = pRt->left + iRectWidth - iWidth;
			}
			else{
				iLeft = pRt->left;
			}

			for( int i = 0 ; i < (int)vecChunk.size() ; ++i ) {
				vecChunk[i].SetRect( pFont, iLeft, iTop );
				iLeft += vecChunk[i].m_iWidth;
			}
			iTop += static_cast<int>(fLineSpace);
		}
	}
	int GetLineWidth( int iLine ) {
		if( m_vecLineChunk.empty() ) return 0;
		if( iLine < 0 ) iLine = 0;
		if( iLine >= static_cast<int>(m_vecLineChunk.size()) )
			iLine = static_cast<int>(m_vecLineChunk.size() - 1);

		int iAcc = 0;
		std::vector<GCFontChunk>::iterator vit;
		for( vit = m_vecLineChunk[iLine].begin() ; vit != m_vecLineChunk[iLine].end() ; ++vit ) {
			iAcc += (*vit).m_iWidth;
		}
		return iAcc;
	}
	int GetChunkSize() const{
		int iSize = 0;
		for( int i = 0 ; i < (int)m_vecLineChunk.size() ; ++i ) {
			iSize += (int)m_vecLineChunk[i].size();
		}
		return iSize;
	}
	const GCFontChunk* GetChunk( int iIndex ) const{
		for( int i = 0 ; i < (int)m_vecLineChunk.size() ; ++i ) {
			if( iIndex >= (int)m_vecLineChunk[i].size() ) {
				iIndex -= (int)m_vecLineChunk[i].size();
			}
			else {
				return &m_vecLineChunk[i][iIndex];
			}
		}
		return NULL;
	}
	void Clear() { m_vecLineChunk.clear(); }
	D3DCOLOR GetOutlineColor() { return m_colorOutline; }
	bool IsRenderOutLine() { return m_bOutLine; }
private:
	void PushChunk( ID3DXFont* pFont, std::wstringstream& stm_, D3DCOLOR color ) {
		if( stm_.str().empty() )
			return;

		GCFontChunk chunk;
		chunk.SetString( pFont, stm_.str(), color );
		stm_.str(L"");
		if( m_vecLineChunk.empty() ) {
			PushNewline();
		}
		m_vecLineChunk[m_vecLineChunk.size()-1].push_back( chunk );
	}
	void PushNewline() {
		m_vecLineChunk.push_back( std::vector<GCFontChunk>() );
	}
	void GetColorKey( const wchar_t*& str_, D3DCOLOR& DestColor, const D3DCOLOR& DefaultColor ) {
		if( *str_ == L'\0' )
			return;

		++str_;
		if( *str_ != L'c' && *str_ != L'C' )
			return;

		++str_;
		if( *str_ == L'x' || *str_ == L'X' ) {
			DestColor = DefaultColor;
			return;
		}

		int iCol;
		WCHAR strColor[7] = { 0, };
		for( int i = 0 ; i < 6 ; ++i ) {
			if( *str_ == L'\0' )
				return;

			strColor[i] = *str_;
			++str_;
		}
		swscanf(strColor,L"%x",&iCol);
		DestColor = ( DestColor & 0xFF000000 ) | iCol;

		// Color와 관련된 마지막 위치로 커서를 맞추기 위해서 한칸 뒤로 땡김.
		--str_;
	}
	std::vector<std::vector<GCFontChunk>> m_vecLineChunk;
	D3DCOLOR    m_colorOutline;
	bool        m_bOutLine;
};

struct SGCFontCache
{
	static float PixelToTex;

	int iLeft, iTop;
	WCHAR _Char;
	DWORD dwLastUse;
	int iWidth,iHeight;
	int iPage;

	float _texLeft;
	float _texRight;
	float _texTop;
	float _texBottom;

	void SetPos(int x, int y, int iFontSize)
	{
		iLeft = x * iFontSize;
		iTop  = y * iFontSize;

		_texLeft = PixelToTex * ((float)iLeft + 0.5f);
		_texTop = PixelToTex * ((float)iTop + 0.5f);
	}

	void _SetRect(int width, int height)
	{
		iWidth = width;
		iHeight = height;

		_texRight = PixelToTex * ((float)(iLeft + iWidth) + 0.5f);
		_texBottom = PixelToTex * ((float)(iTop + iHeight) + 0.5f);
	}
};

class GCDeviceFontBase : public GCDevice
{
public:
	enum
	{
		OUTLINE			=1,
		INLINE			=2,
	};

public:
	GCDeviceFontBase();
	GCDeviceFontBase( LPDIRECT3DDEVICE9 pd3ddevice, std::string strFontName, int iFontSize, int iWeight = FW_NORMAL, int iOutLineSize = 0, bool bRHW = true ,float fFontScale = 1.0f);
	virtual ~GCDeviceFontBase(void);

	void OnResetDevice();
	void OnLostDevice();

	bool Load();
	void UnLoad();

	static std::string MakeFontID( std::string strFontName, int iFontSize, int iWeight = FW_NORMAL, int iOutLineSize = 0, bool bRHW = true, float fFontScale = 1.0f);

	int GetFontSize() { return m_iFontSize + m_iOutLineSize*2; }
	int GetWidth( WCHAR wLetter );
	int GetHeight() const;
	int GetHeight( WCHAR wLetter ) const;
	int GetWidth( const WCHAR* wszText );
	int GetHeight( const WCHAR* wszText ) const;

	std::wstring ConvertToDot( std::wstring& str, int iWidth );

	/// draw text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
	void OutTextXY( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000 ,DWORD linemod=OUTLINE|INLINE, float fFontSpace = 0.0 );

	/// draw multi-lined text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
	/// @param  fLineSpace: line space ratio
	///         - ex) fLineSpace == 2.f means double space
	/// @note   we use '\n' and "\\n" as line breaking special characters
	///         so "Hello\nWorld" and "Hello\\nWorld" will be rendered same format.
	void OutTextMultiline( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000,float fScale = 0, float fFontSpace = 0.0 );

	void OutTextInTexture(LPDIRECT3DTEXTURE9 tex, int iLeft, int iTop, const WCHAR* wszText,
		D3DCOLOR color = 0xffffffff, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000 );

	int wStringRealLength(const std::wstring& strNickName );
protected:

	int GetLeftPos( int iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag );
	int GetTopPos( int iTop,const WCHAR* wszText,RECT* pRt, DWORD dwFlag );

	void InitCache();
	SGCFontCache* GetFontCache(WCHAR _Char);
	void CharToSystemTexture( SGCFontCache* cache);
	void Buffering(int iLeft, int iTop, const WCHAR* str, D3DCOLOR& color, D3DCOLOR DefColor,bool bNoColor, float fFontSpace = 0.0);
	void Flush(bool bOutLine);

protected:
	std::string     m_strFontName;
	int             m_iFontSize;
	int             m_iFontWeight;
	int             m_iOutLineSize;
	int             m_iFontCacheSize;       // m_iFontSize + m_iOutLineSize * 2

	bool            m_bRHW;

	KSafeArray<GCDynamicTexture*,FONT_CACHE_PAGE> m_pTexture;
	KSafeArray<GCDynamicTexture*,FONT_CACHE_PAGE> m_pTextureOutLine;

	KSafeArray<KSafeArray<KD3DUIVertex,MAX_VERTEX_IN_PAGE>,FONT_CACHE_PAGE> m_pUIVertex;
	KSafeArray<KSafeArray<SLVERTEX,MAX_VERTEX_IN_PAGE>,FONT_CACHE_PAGE> m_pVertex;
	int                 m_iTotalCache;
	KSafeArray<int,FONT_CACHE_PAGE> m_iCandidate;
	bool                m_bLoad;

	DWORD               m_dwCurrentTime;

	boost::unordered_map< WCHAR, SGCFontCache* > m_mapChar;

	// about DC
	HPEN                m_hPen, m_hOldPen;
	HBITMAP             m_hBitmap, m_hOldBitmap;
	HDC                 m_hDC;
	HFONT               m_hFont;
	HFONT               m_hOldFont;
	LPDIRECT3DDEVICE9   m_pd3ddevice;
};
