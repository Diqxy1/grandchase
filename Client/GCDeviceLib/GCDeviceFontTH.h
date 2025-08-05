#pragma once

#include "Define.h"
#include "GCDeviceFont.h"

struct SGCFontCacheTH
{
	static float PixelToTex;

	int iLeft, iTop;
	std::wstring _Char;
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

//class GCDeviceFontTH : public GCDeviceFontBase
class GCDeviceFontTH : public GCDevice
{
public:
	enum
	{
#if defined(NATION_THAILAND)
		DEFAULT_FONT_SIZE   = 13,
		MIDDLE_FONT_SIZE    = 14,
		LARGE_FONT_SIZE     = 16,
		TITLE_FONT_SIZE     = 26,
#endif
	};

	enum
	{
		OUTLINE			=1,
		INLINE			=2,
	};

public:
	GCDeviceFontTH( LPDIRECT3DDEVICE9 pd3ddevice, std::string strFontName, int iFontSize, int iWeight = FW_NORMAL, int iOutLineSize = 0, bool bRHW = true ,float fFontScale = 1.0f);
	virtual ~GCDeviceFontTH(void);

	void OnResetDevice();
	void OnLostDevice();

	bool Load();
	void UnLoad();
	void InitCache();

	static std::string MakeFontID( std::string strFontName, int iFontSize, int iWeight = FW_NORMAL, int iOutLineSize = 0, bool bRHW = true, float fFontScale = 1.0f);

	int GetFontSize() { return m_iFontSize + m_iOutLineSize*2; }
	int GetWidth( WCHAR wchar );
	int GetWidthLetter( std::wstring wLetter );
	int GetHeight() const;
	int GetHeight( WCHAR wLetter ) const;
	int GetWidth( const WCHAR* wszText );
	int GetHeight( const WCHAR* wszText ) const;

	std::wstring ConvertToDot( std::wstring& str, int iWidth );

	void Buffering(int iLeft, int iTop, const WCHAR* str, D3DCOLOR& color, D3DCOLOR DefColor,bool bNoColor ,int iFontSpace = 0);
	void OutTextXY( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000 ,DWORD linemod=OUTLINE|INLINE, float fFontSpace =0 );
	void OutTextMultiline( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000,float fScale = 0, float fFontSpace = 0);
	void OutTextInTexture(LPDIRECT3DTEXTURE9 tex, int iLeft, int iTop, const WCHAR* wszText,
		D3DCOLOR color = 0xffffffff, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000 );
private:
	void CharToSystemTexture( SGCFontCacheTH* cache);
	SGCFontCacheTH* GetFontCacheTH(std::wstring _Char);
	void Flush(bool bOutLine);

	int GetLeftPos( int iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag );
	int GetTopPos( int iTop,const WCHAR* wszText,RECT* pRt, DWORD dwFlag );

	std::string     m_strFontName;
	int             m_iFontSize;
	int             m_iFontWeight;
	int             m_iOutLineSize;
	int             m_iFontCacheSize;       // m_iFontSize + m_iOutLineSize * 2

	bool            m_bRHW;

	GCDynamicTexture*   m_pTexture[FONT_CACHE_PAGE];
	GCDynamicTexture*   m_pTextureOutLine[FONT_CACHE_PAGE];

	KD3DUIVertex        m_pUIVertex[FONT_CACHE_PAGE][MAX_VERTEX_IN_PAGE];
	SLVERTEX            m_pVertex[FONT_CACHE_PAGE][MAX_VERTEX_IN_PAGE];
	int                 m_iTotalCache;
	int                 m_iCandidate[FONT_CACHE_PAGE];
	bool                m_bLoad;

	DWORD               m_dwCurrentTime;
	boost::unordered_map< std::wstring , SGCFontCacheTH* > m_mapTHChar;

	// about DC
	HPEN                m_hPen, m_hOldPen;
	HBITMAP             m_hBitmap, m_hOldBitmap;
	HDC                 m_hDC;
	HFONT               m_hFont;
	HFONT               m_hOldFont;
	LPDIRECT3DDEVICE9   m_pd3ddevice;
};
