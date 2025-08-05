#include ".\GCDeviceVirtualFont.h"
#include <algorithm>
#include "KJohnGen.h"
#include "KD3DDeviceState.h"

#define _16BIT_FONT
#ifdef _16BIT_FONT
//D3DCOLOR_A1R5G5B5
#define MYD3DCOLOR_ARGB(a,r,g,b)	(WORD)( ((a&0x1) << 15) | ((r&0x1F)<<10) | ((g&0x1F) << 5) | (b&0x1F) )
#define MYFONTCOLORFMT				(D3DFMT_A1R5G5B5)
#define MYFONTCOLOR_PTR				WORD*
#else
#define MYD3DCOLOR_ARGB(a,r,g,b)	D3DCOLOR_ARGB(a,r,g,b)
#define MYFONTCOLORFMT				(D3DFMT_A8R8G8B8)
#define MYFONTCOLOR_PTR				DWORD*
#endif

float SGCFontCache::PixelToTex = 1.0f / FONT_CACHE_SIZE;

void GCFontVertex::SetVertex(int x, int y, D3DCOLOR diff, float fTexU, float fTexV )
{
	//m_vPos = D3DXVECTOR4((float)x,(float)y,0.0f,1.0f);
	m_vPos.x = (float)x;
	m_vPos.y = (float)y;
	m_diff = diff;
	m_vTex.x = fTexU;
	m_vTex.y = fTexV;
}

class GCFontCacheScheduler
{
public: 
	bool operator()( const SGCFontCache* p, const SGCFontCache* q ) const
	{
		return p->dwLastUse < q->dwLastUse; 
	}
};


GCDeviceVirtualFont::GCDeviceVirtualFont(LPDIRECT3DDEVICE9 d3ddevice)
{
	m_pd3ddevice = d3ddevice;	
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		m_pTextureSys[i] = NULL;
		m_pTextureVid[i] = NULL;
		m_iCandidate[i] = 0;
	}	
}

GCDeviceVirtualFont::~GCDeviceVirtualFont(void)
{
	UnLoad();
}

void GCDeviceVirtualFont::OnResetDevice()
{	
	// Only : D3DPOOL_DEFAULT
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		SAFE_RELEASE(m_pTextureVid[i]);
		if( FAILED( D3DXCreateTexture( m_pd3ddevice, FONT_CACHE_SIZE, FONT_CACHE_SIZE, 1, NULL, MYFONTCOLORFMT, D3DPOOL_DEFAULT, &m_pTextureVid[i] ) ) )
		{
			//이것조차 실패하면 할말 없음
			return;
		}

		m_pTextureSys[i]->UnlockRect(0);
		m_pd3ddevice->UpdateTexture( m_pTextureSys[i], m_pTextureVid[i] );
		m_pTextureSys[i]->LockRect(0,&m_LockedRect[i], NULL, 0);
	}
}

void GCDeviceVirtualFont::OnLostDevice()
{
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		SAFE_RELEASE(m_pTextureVid[i]);
	}
}

bool GCDeviceVirtualFont::Load(const char* strFontName, int iFontSize)
{
	m_iFontSize = iFontSize;

	m_hFont = CreateFontA( iFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, strFontName);

	m_hDC = CreateCompatibleDC( NULL );
	m_hBitmap = CreateCompatibleBitmap( GetDC(GetDesktopWindow()), MAX_FONT_SIZE+1, MAX_FONT_SIZE+1 );	
	m_hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap );	
	m_hOldFont = (HFONT)SelectObject( m_hDC, m_hFont );
	SetTextColor( m_hDC, RGB(255,0,0));
	SetBkColor( m_hDC, RGB(0,0,0) );

	HRESULT hr;	

	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		if( FAILED(hr = D3DXCreateTexture( m_pd3ddevice, FONT_CACHE_SIZE, FONT_CACHE_SIZE, 1, NULL, MYFONTCOLORFMT, D3DPOOL_SYSTEMMEM, &m_pTextureSys[i] ) ) )
		{	
			//이것조차 실패하면 할말 없음
			return false;
		}	
		if( FAILED( D3DXCreateTexture( m_pd3ddevice, FONT_CACHE_SIZE, FONT_CACHE_SIZE, 1, NULL, MYFONTCOLORFMT, D3DPOOL_DEFAULT, &m_pTextureVid[i] ) ) )
		{
			//이것조차 실패하면 할말 없음
			return false;
		}

		hr = m_pTextureSys[i]->LockRect(0,&m_LockedRect[i], NULL, 0);	
	}

	InitCache();

	return true;
}

void GCDeviceVirtualFont::InitCache()
{
	m_iTotalCache = (FONT_CACHE_SIZE / m_iFontSize) * (FONT_CACHE_SIZE / m_iFontSize) * FONT_CACHE_PAGE;
	// m_iTotalCache = FONT_CACHE_PAGE * 10;		// TEST CODE
}

void GCDeviceVirtualFont::UnLoad()
{	
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		SAFE_RELEASE(m_pTextureSys[i]);
		SAFE_RELEASE(m_pTextureVid[i]);
	}
	
	SelectObject(m_hDC, m_hOldBitmap );
	SelectObject( m_hDC, m_hOldPen );
	SelectObject( m_hDC, m_hOldFont );

	DeleteObject( m_hBitmap );
	DeleteObject( m_hOldPen );
	DeleteObject( m_hOldFont );

	DeleteDC( m_hDC );

	std::map< WCHAR, SGCFontCache* >::iterator itor = m_mapChar.begin();
	while( itor != m_mapChar.end() )
	{
		SAFE_DELETE( itor->second );
		++itor;
	}
	m_mapChar.clear();
}

int GCDeviceVirtualFont::GetWidth( WCHAR wLetter ) const
{	
	SIZE size;
	GetTextExtentPoint32W( m_hDC, &wLetter, 1, &size);	
	return (int)size.cx;
}

int GCDeviceVirtualFont::GetHeight( WCHAR wLetter ) const
{
	SIZE size;
	GetTextExtentPoint32W( m_hDC, &wLetter, 1, &size);	
	return (int)size.cy;
}

int GCDeviceVirtualFont::GetWidth( const WCHAR* wszText ) const
{
	int iLen = (int)wcslen(wszText);
	WCHAR* strBuf = new WCHAR[iLen+1];
	memset( strBuf, 0, sizeof(WCHAR)*(iLen+1) );
	int iCur = 0;
	for(int i = 0; i < iLen; i++ )
	{
		if( wszText[i] == L'#' && i < iLen -1 && ( wszText[i+1] == L'c' || wszText[i+1] == L'C' ) )
		{
			if( wszText[i+2] == 'x' || wszText[i+2] == 'X' )
			{
				i += 2;
			}
			else
			{
				i += 7;
			}
			continue;
		}
		strBuf[iCur++] = wszText[i];
	}
	SIZE size;
	GetTextExtentPoint32W( m_hDC, strBuf, (int)wcslen(strBuf), &size);
	SAFE_DELETE_ARRAY( strBuf );
	return (int)size.cx;	
}

int GCDeviceVirtualFont::GetHeight( const WCHAR* wszText ) const
{
	SIZE size;
	GetTextExtentPoint32W( m_hDC, wszText, (int)wcslen(wszText), &size);	
	return (int)size.cy;
}

void GCDeviceVirtualFont::CharToSystemTexture( SGCFontCache* cache)
{	
	COLORREF black = RGB(0,0,0);	
	TextOutW( m_hDC, 0, 0, &cache->_Char, 1 );	
	COLORREF _col;

	MYFONTCOLOR_PTR pFirst = (MYFONTCOLOR_PTR)m_LockedRect[cache->iPage].pBits;

	for(int y = 0; y < cache->iHeight; y++)
	{
		pFirst = (MYFONTCOLOR_PTR)m_LockedRect[cache->iPage].pBits + (y + cache->iTop) * FONT_CACHE_SIZE;

		for(int x = 0; x < cache->iWidth; x++)
		{
			//투명한 부분임			
			_col = GetPixel( m_hDC, x, y );			
			if( _col == black )
			{
				*(pFirst + x + cache->iLeft ) = 0;//MYD3DCOLOR_ARGB(0,0,0,0);
			}
			else
			{
				//alpha 를 0~f로 표현하자
				int iCol = (int)((float)GetRValue(_col) * 0x1f / 255.0f);						
				*(pFirst + x + cache->iLeft ) = MYD3DCOLOR_ARGB(1,iCol,iCol,iCol);
			}			
		}
	}
}

SGCFontCache* GCDeviceVirtualFont::GetFontCache(WCHAR _Char)
{
	std::map< WCHAR, SGCFontCache* >::iterator itor = m_mapChar.find(_Char);

	if( itor != m_mapChar.end() )
	{
		itor->second->dwLastUse = m_dwCurrentTime;
		return itor->second;
	}

	//자리가 없으므로 자리를 마련함
	if( (int)m_mapChar.size() >= m_iTotalCache )
	{		
		std::vector<SGCFontCache*> vecCache;
		std::map< WCHAR, SGCFontCache* >::iterator i = m_mapChar.begin();

		while( i != m_mapChar.end() )
		{
			vecCache.push_back( i->second );
			i++;
		}

		std::sort( vecCache.begin(), vecCache.end(), GCFontCacheScheduler() );
		SGCFontCache *cache = vecCache[0];

		m_bUpdateTexture[cache->iPage] = true;
		m_mapChar.erase( m_mapChar.find(cache->_Char) );
		cache->_Char = _Char;
		cache->dwLastUse = m_dwCurrentTime;
		SIZE size;
		GetTextExtentPoint32W( m_hDC, &cache->_Char, 1, &size);	
		if( size.cx >= m_iFontSize )
			size.cx = m_iFontSize;
		if( size.cy >= m_iFontSize )
			size.cy = m_iFontSize;
		cache->_SetRect(size.cx,size.cy,m_iFontSize);
		CharToSystemTexture( cache );
		m_mapChar[_Char] = cache;
		return cache;
	}

	
	SGCFontCache *cache = new SGCFontCache();
	const int iCharPerPage = (FONT_CACHE_SIZE / m_iFontSize) * (FONT_CACHE_SIZE / m_iFontSize);
	//const int iCharPerPage = 10; // TEST CODE
	int iPage = (int)m_mapChar.size() / iCharPerPage;
	int iIndexInPage = (int)m_mapChar.size() % iCharPerPage;
	int y = iIndexInPage / (FONT_CACHE_SIZE / m_iFontSize);
	int x = iIndexInPage % (FONT_CACHE_SIZE / m_iFontSize);

	cache->iPage = iPage;
	m_bUpdateTexture[cache->iPage] = true;	
	cache->SetPos(x,y,m_iFontSize);
	cache->_Char = _Char;
	cache->dwLastUse = m_dwCurrentTime;
	SIZE size;
	GetTextExtentPoint32W( m_hDC, &cache->_Char, 1, &size);	
	if( size.cx >= m_iFontSize )
		size.cx = m_iFontSize;
	if( size.cy >= m_iFontSize )
		size.cy = m_iFontSize;
	cache->_SetRect(size.cx,size.cy,m_iFontSize);

	CharToSystemTexture( cache );
	m_mapChar[_Char] = cache;
	return cache;
}

void GCDeviceVirtualFont::Buffering(int iLeft, int iTop, const WCHAR* str, D3DCOLOR& color, D3DCOLOR DefColor,bool bNoColor)
{
	int isize = (int)wcslen(str);
	GCFontVertex* pVertex = NULL;
	
	for(int i = 0; i < isize; ++i )
	{
		if( str[i] == L'\n' )
			continue;
		if( str[i] == L'\r' )
			continue;

		// Color Change 		
		if( str[i] == L'#' && i != isize-1 && (str[i+1] == L'C' || str[i+1] == L'c' ) )
		{
			if( !bNoColor )
			{			
				if( str[i+2] == L'X' || str[i+2] == L'x' )
				{
					i += 2;
					color = DefColor;
					continue;
				}
				// #C R G B형식으로 들어있음
				// #CFF0000 -> Red
				int iCol;
				WCHAR strColor[7] = { 0, };
				wcsncpy(strColor,&str[i+2],6);
				swscanf(strColor,L"%x",&iCol);
#ifndef NO_TEXT_COLOR
				color = ( color & 0xFF000000 ) | iCol;
#endif
			}
			i += 7;
			continue;
		}
	
		SGCFontCache *cache = GetFontCache(str[i]);
		pVertex = &m_pVertex[cache->iPage][m_iCandidate[cache->iPage] * 6];			
		pVertex[0].SetVertex( iLeft, iTop,									color, cache->_texLeft, cache->_texTop );
		pVertex[1].SetVertex( iLeft + cache->iWidth, iTop,					color, cache->_texRight, cache->_texTop );
		pVertex[2].SetVertex( iLeft, iTop + cache->iHeight,					color, cache->_texLeft, cache->_texBottom );

		pVertex[3].SetVertex( iLeft + cache->iWidth, iTop,					color, cache->_texRight, cache->_texTop );
		pVertex[4].SetVertex( iLeft + cache->iWidth, iTop + cache->iHeight,	color, cache->_texRight, cache->_texBottom);
		pVertex[5].SetVertex( iLeft, iTop + cache->iHeight,					color, cache->_texLeft, cache->_texBottom );		
		
		iLeft += cache->iWidth;
		m_iCandidate[cache->iPage]++;
	}	
}

void GCDeviceVirtualFont::Flush()
{
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		if( m_iCandidate[i] == 0 )
			continue;

		if( m_bUpdateTexture[i] )
		{	
			m_pTextureSys[i]->UnlockRect(0);
			m_pd3ddevice->UpdateTexture( m_pTextureSys[i], m_pTextureVid[i] );
			m_pTextureSys[i]->LockRect(0,&m_LockedRect[i], NULL, 0);	
		}
		HRESULT hr;

		KD3DDeviceState::PushRenderStateSet();
		{	
			KD3DDeviceState::PushRenderState( D3DRS_LIGHTING, FALSE );
			KD3DDeviceState::PushRenderState( D3DRS_ZENABLE, FALSE );
			KD3DDeviceState::PushRenderState( D3DRS_ZWRITEENABLE, FALSE );
			KD3DDeviceState::PushRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			KD3DDeviceState::PushRenderState( D3DRS_ALPHATESTENABLE, TRUE );
			KD3DDeviceState::PushRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			KD3DDeviceState::PushRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			m_pd3ddevice->SetTexture(0, m_pTextureVid[i] );
			m_pd3ddevice->SetFVF( GCFontVertex::FVF );

			hr = m_pd3ddevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_pVertex[i], sizeof( GCFontVertex ) );
		}
		KD3DDeviceState::PopRenderStateSet();

		m_iCandidate[i] = 0;
		
	}
}

int GCDeviceVirtualFont::GetLeftPos( int iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag )
{	
	if( pRt == NULL || (dwFlag & DT_LEFT) )
		return iLeft;

	int iWidth = pRt->right - pRt->left;
	int iFontWidth = GetWidth(wszText);

	if( dwFlag & DT_CENTER )
		return iLeft + ( iWidth - iFontWidth ) / 2;
		//return pRt->left + ( iWidth - iFontWidth ) / 2;
	else if( dwFlag & DT_RIGHT )
		return (iLeft - pRt->left) + pRt->right - iFontWidth;
	return iLeft;
}

void GCDeviceVirtualFont::OutTextXY( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color, RECT* pRt, DWORD dwFlag )
{		
	iLeft = GetLeftPos( iLeft, wszText, pRt, dwFlag );

	m_dwCurrentTime = timeGetTime();
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		m_bUpdateTexture[i] = false;
	}
	D3DCOLOR curcol = color;
	Buffering(iLeft, iTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0 );
	Flush();		
}

void GCDeviceVirtualFont::OutTextMultiline( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color, float fLineSpace, RECT* pRt, DWORD dwFlag )
{
	m_dwCurrentTime = timeGetTime();
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		m_bUpdateTexture[i] = false;
	}

	static WCHAR			wstrBuf[VIRTUAL_FONT_LINE_MAX];
	wcscpy( wstrBuf, wszText );

	WCHAR*			pToken = wstrBuf;
	int iSize       = (int)wcslen(wstrBuf);
	bool bNewLine = false;

	D3DCOLOR curcol = color;

	for(int i = 0; i < iSize; i++)
	{
		if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' || i == iSize - 1 )
		{	
			bNewLine = false;
			if( wstrBuf[i] == L'\n' )
				bNewLine = true;
			if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' )
				wstrBuf[i] = 0;

			int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag );

			Buffering(iLineLeft, iTop, pToken, curcol, color, (dwFlag & DT_NO_COLOR) != 0 );
			//Display Here
			if( bNewLine )
			{
				iTop += (int)(m_iFontSize * fLineSpace);
			}			
			
			pToken = &wstrBuf[i+1];
		}

	}
/*
	pToken = wcstok( wstrBuf, L"\r\n" );

	while( pToken )
	{
		Buffering(iLeft, iTop, pToken, color );	
		iTop += (int)(m_iFontSize * fLineSpace);
		pToken = wcstok( NULL, L"\r\n" );
	}	
*/	
	Flush();
}
