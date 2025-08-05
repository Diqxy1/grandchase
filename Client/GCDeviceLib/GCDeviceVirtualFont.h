#pragma once

#define MAX_FONT_SIZE				(128)
#define FONT_CACHE_SIZE				(256)
#define FONT_CACHE_PAGE				(4)
#define VIRTUAL_FONT_LINE_MAX		(3000)
#define	MAX_CHAR_IN_PAGE			(((int)FONT_CACHE_SIZE/12)*((int)FONT_CACHE_SIZE/12))		//12 X 12를 최소로 생각함
#define MAX_VERTEX_IN_PAGE			(MAX_CHAR_IN_PAGE*6)

#define DT_NO_COLOR					(0xF0000000)

struct GCFontVertex
{
	enum _FVF
	{
		FVF = (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1),
	};

	GCFontVertex()
	{
		m_vPos.z = 0.0f;
		m_vPos.w = 1.0f;
	}
	D3DXVECTOR4 m_vPos; // x, y, z, 1/w
	D3DCOLOR    m_diff; // diffuse color
	D3DXVECTOR2 m_vTex; // tu, tv

	void SetVertex( int x, int y, D3DCOLOR diff, float fTexU, float iTexV );
};

struct SGCFontCache
{
	static float PixelToTex;

	int iLeft, iTop;	
	WCHAR _Char;
	DWORD dwLastUse;
	int iWidth,iHeight;
	int	iPage;

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

	void _SetRect(int width, int height,int iFontSize)
	{
		iWidth = width;
		iHeight = height;
		
		_texRight = PixelToTex * ((float)(iLeft + iWidth) + 0.5f);
		_texBottom = PixelToTex * ((float)(iTop + iHeight) + 0.5f);
	}
};

class GCDeviceVirtualFont
{
public:
	GCDeviceVirtualFont(LPDIRECT3DDEVICE9 d3ddevice);
	virtual ~GCDeviceVirtualFont(void);

	void OnResetDevice();
	void OnLostDevice();

	bool Load(const char* strFontName, int iFontSize);
	void UnLoad();

	int GetWidth( WCHAR wLetter ) const;
	int GetHeight( WCHAR wLetter ) const;
	int GetWidth( const WCHAR* wszText ) const;
	int GetHeight( const WCHAR* wszText ) const;	

	
	/// draw text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
	void OutTextXY( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color, RECT* pRt, DWORD dwFlag );

	/// draw multi-lined text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
	/// @param  fLineSpace: line space ratio
	///         - ex) fLineSpace == 2.f means double space
	/// @note   we use '\n' and "\\n" as line breaking special characters
	///         so "Hello\nWorld" and "Hello\\nWorld" will be rendered same format.
	void OutTextMultiline( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color, float fLineSpace, RECT* pRt, DWORD dwFlag);
private:
	int GetLeftPos( int iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag );
	void InitCache();
	SGCFontCache* GetFontCache(WCHAR _Char);
	void CharToSystemTexture( SGCFontCache* cache);
	void Buffering(int iLeft, int iTop, const WCHAR* str, D3DCOLOR& color, D3DCOLOR DefColor,bool bNoColor);
	void Flush();
private:	
	LPDIRECT3DDEVICE9   m_pd3ddevice;
	LPDIRECT3DTEXTURE9	m_pTextureSys[FONT_CACHE_PAGE];
	LPDIRECT3DTEXTURE9	m_pTextureVid[FONT_CACHE_PAGE];
	GCFontVertex		m_pVertex[FONT_CACHE_PAGE][MAX_VERTEX_IN_PAGE];
	int					m_iFontSize;
	int					m_iTotalCache;
	D3DLOCKED_RECT		m_LockedRect[FONT_CACHE_PAGE];
	bool				m_bUpdateTexture[FONT_CACHE_PAGE];
	int					m_iCandidate[FONT_CACHE_PAGE];

	DWORD				m_dwCurrentTime;

	std::map< WCHAR, SGCFontCache* > m_mapChar;
	

	// about DC
	HPEN				m_hPen, m_hOldPen;
	HBITMAP				m_hBitmap, m_hOldBitmap;
	HDC					m_hDC;
	HFONT				m_hFont;
	HFONT				m_hOldFont;
};
