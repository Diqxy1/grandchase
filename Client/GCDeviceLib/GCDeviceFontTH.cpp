#include "StdAfx.h"
#include "GCDeviceFontTH.h"

float SGCFontCacheTH::PixelToTex = 1.0f / FONT_CACHE_SIZE;

class GCFontCacheSchedulerTH
{
public:
	bool operator()( const SGCFontCacheTH* p, const SGCFontCacheTH* q ) const
	{
		return p->dwLastUse < q->dwLastUse;
	}
};

//GCDeviceFontTH::GCDeviceFontTH(LPDIRECT3DDEVICE9 pd3ddevice, std::string strFontName, int iFontSize, int iWeight, int iOutLineSize, bool bRHW, float fFontScale)
//: GCDeviceFontBase(pd3ddevice,strFontName,iFontSize,iWeight,iOutLineSize,bRHW,fFontScale)
//{
//
//}

GCDeviceFontTH::GCDeviceFontTH(LPDIRECT3DDEVICE9 pd3ddevice, std::string strFontName, int iFontSize, int iWeight, int iOutLineSize, bool bRHW, float fFontScale)
{
	m_eDeviceType = GCDT_FONT;
	m_strDeviceID = MakeFontID(strFontName, iFontSize, iWeight, iOutLineSize, bRHW ,fFontScale);
	m_strFontName = strFontName;
	m_iFontSize = static_cast<int>( static_cast<float>(iFontSize) * fFontScale );
	m_iOutLineSize = iOutLineSize;
	m_iFontCacheSize = static_cast<int>( static_cast<float>(iFontSize * fFontScale) + iOutLineSize*2.f + fFontScale * 2.f ); //소수점 문제 처리해주기 위해서 +1 해준다.
	m_bRHW = bRHW;

	//m_iFontWeight = static_cast<int>( static_cast<float>(iWeight) * fFontScale );
	m_iFontWeight = iWeight; //폰트 두께를 늘리는 돼지 폰트가 된다.
	m_pd3ddevice = pd3ddevice;
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		m_pTexture[i]           = NULL;
		m_pTextureOutLine[i]    = NULL;
		m_iCandidate[i]         = 0;
	}

	m_bLoad = false;
}

GCDeviceFontTH::~GCDeviceFontTH(void)
{
	UnLoad();
}
int GCDeviceFontTH::GetHeight( ) const
{
	return m_iFontCacheSize;
}

void GCDeviceFontTH::OnResetDevice()
{
	if( !m_bLoad )
	{
		Load();
	}
	else
	{
		// Only : D3DPOOL_DEFAULT
		for(int i = 0; i < FONT_CACHE_PAGE; i++)
		{
			m_pTexture[i]->ResetDevice();
			if( m_pTextureOutLine[i] != NULL )
			{
				m_pTextureOutLine[i]->ResetDevice();
			}
		}
	}
}

void GCDeviceFontTH::OnLostDevice()
{
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		if( m_pTexture[i] != NULL )
		{
			m_pTexture[i]->LostDevice();
		}
		if( m_pTextureOutLine[i] != NULL )
		{
			m_pTextureOutLine[i]->LostDevice();
		}
	}
}

bool GCDeviceFontTH::Load()
{
	if( m_bLoad )
		return true;

	//m_hFont = CreateFontA( m_iFontSize, 0, 0, 0, m_iFontWeight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
	//    OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, m_strFontName.c_str());

	m_hFont = CreateFontA( m_iFontSize, 0, 0, 0, m_iFontWeight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, m_strFontName.c_str());

	m_hDC = CreateCompatibleDC( NULL );
	m_hBitmap = CreateCompatibleBitmap( GetDC(GetDesktopWindow()), MAX_FONT_SIZE+1, MAX_FONT_SIZE+1 );
	m_hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap );
	m_hOldFont = (HFONT)SelectObject( m_hDC, m_hFont );
	SetTextColor( m_hDC, RGB(255,0,0));
	SetBkColor( m_hDC, RGB(0,0,0) );

	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		m_pTexture[i] = new GCDynamicTexture();

		if( !m_pTexture[i]->CreateTexture(m_pd3ddevice, FONT_CACHE_SIZE, FONT_CACHE_SIZE, MYFONTCOLORFMT) )
		{
			m_eDeviceState = GCDS_DISABLE;
			return false;
		}

		if( m_iOutLineSize > 0 )
		{
			m_pTextureOutLine[i] = new GCDynamicTexture();

			if( !m_pTextureOutLine[i]->CreateTexture(m_pd3ddevice, FONT_CACHE_SIZE, FONT_CACHE_SIZE, MYFONTCOLORFMT) )
			{
				m_eDeviceState = GCDS_DISABLE;
				return false;
			}
		}
	}

	InitCache();
	m_bLoad = true;
	m_eDeviceState = GCDS_LOADED;
	return true;
}

void GCDeviceFontTH::InitCache()
{
	m_iTotalCache = (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize) * FONT_CACHE_PAGE;
}

void GCDeviceFontTH::UnLoad()
{
	if(m_eDeviceState!=GCDS_LOADED)
		return;

	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		SAFE_DELETE(m_pTexture[i]);
		SAFE_DELETE(m_pTextureOutLine[i]);
	}

	SelectObject(m_hDC, m_hOldBitmap );
	SelectObject( m_hDC, m_hOldPen );
	SelectObject( m_hDC, m_hOldFont );

	DeleteObject( m_hBitmap );
	DeleteObject( m_hOldPen );
	DeleteObject( m_hOldFont );

	DeleteDC( m_hDC );

	boost::unordered_map< std::wstring, SGCFontCacheTH* >::iterator itor = m_mapTHChar.begin();
	while( itor != m_mapTHChar.end() )
	{
		SAFE_DELETE( itor->second );
		++itor;
	}
	m_mapTHChar.clear();

	m_bLoad = false;
	m_eDeviceState = GCDS_NOT_LOADED;
}

std::string GCDeviceFontTH::MakeFontID( std::string strFontName, int iFontSize, int iWeight, int iOutLineSize, bool bRHW ,float fFontScale)
{
	static char strTemp[MAX_PATH];
	sprintf(strTemp,"%s%d_%d_%d_%d_%5.3f",strFontName.c_str(), iFontSize, iOutLineSize, bRHW, iWeight, fFontScale);
	strlwr( strTemp );
	return strTemp;
}

int GCDeviceFontTH::GetWidth( WCHAR wchar )
{
	std::wstring wstr;
	wstr.clear();
	wstr+=wchar;
	return GetWidthLetter(wstr);
}

int GCDeviceFontTH::GetWidthLetter( std::wstring wLetter )
{
	SGCFontCacheTH *cache = GetFontCacheTH(wLetter);
	if( cache == NULL )
		return 0;

	return cache->iWidth;// + m_iOutLineSize * 2;;
}

int GCDeviceFontTH::GetHeight( WCHAR wLetter ) const
{
	SIZE size;
	GetTextExtentPoint32W( m_hDC, &wLetter, 1, &size);
	return (int)size.cy;
}

int GCDeviceFontTH::GetWidth( const WCHAR* wszText )
{
	if( m_eDeviceState == GCDS_DISABLE )
		return 0;

	int iLen = (int)wcslen(wszText);
	if (iLen==0 ) return 0;
	int iCur = 0;
	int iWidth = 0;

	for(int i = 0; i < iLen; )
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
			i++;//For문에서 도는거 빠져서 넣어줌
			continue;
		}
		int charSize = (int)(GCUTIL_STR_THAI::CharNextTh(&wszText[i]) - &wszText[i]);

		std::wstring wstr;
		for (int charIndex=0;charIndex<charSize;charIndex++)
		{
			wstr+=wszText[i+charIndex];
		}
		iWidth += GetWidthLetter( wstr );
		i+=charSize;
	}

	return iWidth;
}

int GCDeviceFontTH::GetHeight( const WCHAR* wszText ) const
{
	if( m_eDeviceState == GCDS_DISABLE )
		return 0;

	SIZE size;
	GetTextExtentPoint32W( m_hDC, wszText, (int)wcslen(wszText), &size);
	return (int)size.cy;
}

int GCDeviceFontTH::GetLeftPos( int iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag )
{
	if( pRt == NULL )
	{
		if ( dwFlag & DT_CENTER )
			iLeft -= GetWidth(wszText) / 2;
		return iLeft;
	}
	if ( dwFlag & DT_LEFT )
	{
		return iLeft;
	}

	int iWidth = pRt->right - pRt->left;
	int iFontWidth = GetWidth(wszText);

	if( dwFlag & DT_CENTER )
		return pRt->left + ( iWidth - iFontWidth ) / 2;
	else if( dwFlag & DT_RIGHT )
		return pRt->right - iFontWidth;
	return iLeft;
}

int GCDeviceFontTH::GetTopPos( int iTop,const WCHAR* wszText,RECT* pRt, DWORD dwFlag )
{
	if( pRt == NULL )
	{
		if ( dwFlag & DT_VCENTER )
			iTop -= GetHeight(wszText) / 2;
		return iTop;
	}
	if ( dwFlag & DT_TOP )
	{
		return iTop;
	}

	int iHeight = pRt->bottom - pRt->top;
	int iFontHeight = GetHeight(wszText);

	if( dwFlag & DT_VCENTER )
		return pRt->top + ( iHeight - iFontHeight ) / 2;
	else if( dwFlag & DT_RIGHT )
		return pRt->bottom - iFontHeight;
	return iTop;
}

/// draw text.
/// @param  iLeft: left-position of first char of text string(screen coordinate)
/// @param  iTop: top-position of first char of text string
/// @param  pszText: [in] output text
void GCDeviceFontTH::OutTextXY( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color/* = 0xffffffff*/, RECT* pRt/* = NULL*/, DWORD dwFlag/* = DT_LEFT|DT_TOP*/, D3DCOLOR colorOutLine/* = 0xff000000 */,DWORD linemod, float fFontSpace)
{
	if( m_eDeviceState == GCDS_DISABLE )
		return;

	if( wcslen(wszText) == 0 )
		return;

	iLeft = GetLeftPos( iLeft, wszText, pRt, dwFlag );

	m_dwCurrentTime = timeGetTime();
	/*    for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
	m_pTexture[i]->SetChange(false);
	if( m_pTextureOutLine[i] )
	{
	m_pTextureOutLine[i]->SetChange(false);
	}
	}*/

	D3DCOLOR curcol;

	if( m_iOutLineSize && linemod & OUTLINE)
	{
		curcol = colorOutLine;
		Buffering(iLeft, iTop, (WCHAR*)wszText, curcol, colorOutLine, true , fFontSpace);
		Flush(true);
	}

	if(linemod & INLINE)
	{
		curcol = color;
		Buffering(iLeft, iTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0 , fFontSpace);
		Flush(false);
	}
}

void GCDeviceFontTH::Flush(bool bOutLine)
{
	//HRESULT hr;
	g_pGCDeviceManager->PushState();
	g_pGCDeviceManager->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
	g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
	g_pGCDeviceManager->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );

	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		if( m_iCandidate[i] == 0 )
			continue;

		/*if( bOutLine )
		m_pTextureOutLine[i]->SetTexture(0);
		else
		m_pTexture[i]->SetTexture(0);*/

		if( m_bRHW == true )
		{
			//m_pd3ddevice->SetFVF( KD3DUIVertex::FVF );
			//hr = m_pd3ddevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_pUIVertex[i], sizeof( KD3DUIVertex ) );
			if( bOutLine )
			{
				g_pGCDeviceManager->DrawInScreen(m_pTextureOutLine[i],m_pUIVertex[i],m_iCandidate[i]);
			}
			else
			{
				g_pGCDeviceManager->DrawInScreen(m_pTexture[i],m_pUIVertex[i],m_iCandidate[i]);
			}
		}
		else
		{
			//m_pd3ddevice->SetFVF( SLVERTEX::FVF );
			//hr = m_pd3ddevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_pVertex[i], sizeof( SLVERTEX ) );
			if( bOutLine )
			{
				g_pGCDeviceManager->DrawInWorld(m_pTextureOutLine[i],m_pVertex[i],m_iCandidate[i]);
			}
			else
			{
				g_pGCDeviceManager->DrawInWorld(m_pTexture[i],m_pVertex[i],m_iCandidate[i]);
			}
		}
		m_iCandidate[i] = 0;
	}
	g_pGCDeviceManager->PopState();
}

SGCFontCacheTH* GCDeviceFontTH::GetFontCacheTH(std::wstring _Char)
{
	boost::unordered_map< std::wstring, SGCFontCacheTH* >::iterator itor = m_mapTHChar.find(_Char);

	if( itor != m_mapTHChar.end() )
	{
		itor->second->dwLastUse = m_dwCurrentTime;
		return itor->second;
	}

	//자리가 없으므로 자리를 마련함
	if( (int)m_mapTHChar.size() >= m_iTotalCache )
	{
		std::vector<SGCFontCacheTH*> vecCache;
		boost::unordered_map< std::wstring, SGCFontCacheTH* >::iterator i = m_mapTHChar.begin();

		while( i != m_mapTHChar.end() )
		{
			vecCache.push_back( i->second );
			i++;
		}

		std::sort( vecCache.begin(), vecCache.end(), GCFontCacheSchedulerTH() );
		SGCFontCacheTH *cache = vecCache[0];

		m_pTexture[cache->iPage]->SetChange(true);

		if( m_pTextureOutLine[cache->iPage] )
			m_pTextureOutLine[cache->iPage]->SetChange(true);

		m_mapTHChar.erase( m_mapTHChar.find(cache->_Char) );
		cache->_Char = _Char;
		cache->dwLastUse = m_dwCurrentTime;
		SIZE size;
		GetTextExtentPoint32W( m_hDC, cache->_Char.c_str(), cache->_Char.size(), &size);
		if( size.cx > m_iFontCacheSize )
			size.cx = m_iFontCacheSize;
		if( size.cy > m_iFontCacheSize )
			size.cy = m_iFontCacheSize;
		cache->_SetRect(size.cx + m_iOutLineSize*2, size.cy + m_iOutLineSize*2 );
		CharToSystemTexture( cache );
		m_mapTHChar[_Char] = cache;
		return cache;
	}

	SGCFontCacheTH *cache = new SGCFontCacheTH();
	const int iCharPerPage = (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize);
	int iPage = (int)m_mapTHChar.size() / iCharPerPage;
	int iIndexInPage = (int)m_mapTHChar.size() % iCharPerPage;
	int y = iIndexInPage / (FONT_CACHE_SIZE / m_iFontCacheSize);
	int x = iIndexInPage % (FONT_CACHE_SIZE / m_iFontCacheSize);

	cache->iPage = iPage;
	m_pTexture[cache->iPage]->SetChange(true);
	if( m_pTextureOutLine[cache->iPage] )
		m_pTextureOutLine[cache->iPage]->SetChange(true);
	cache->SetPos(x,y,m_iFontCacheSize);
	cache->_Char = _Char;
	cache->dwLastUse = m_dwCurrentTime;
	SIZE size;
	GetTextExtentPoint32W( m_hDC, cache->_Char.c_str(), cache->_Char.size(), &size);
	if( size.cx > m_iFontCacheSize )
		size.cx = m_iFontCacheSize;
	if( size.cy > m_iFontCacheSize )
		size.cy = m_iFontCacheSize;
	cache->_SetRect( size.cx + m_iOutLineSize*2, size.cy + m_iOutLineSize*2 );

	CharToSystemTexture( cache );
	m_mapTHChar[_Char] = cache;
	return cache;
}

void GCDeviceFontTH::CharToSystemTexture( SGCFontCacheTH* cache)
{
	COLORREF black = RGB(0,0,0);
	TextOutW( m_hDC, 0, 0, cache->_Char.c_str(), cache->_Char.length() );
	COLORREF _col;

	MYFONTCOLOR_PTR pFirst = (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits;

	for(int y = 0; y < cache->iHeight; y++)
	{
		pFirst = (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits + (y + cache->iTop ) * FONT_CACHE_SIZE;

		for(int x = 0; x < cache->iWidth; x++)
		{
			if( y < m_iOutLineSize || y >= cache->iHeight - m_iOutLineSize ||
				x < m_iOutLineSize || x >= cache->iWidth -  m_iOutLineSize )
			{
				*(pFirst + x + cache->iLeft ) = 0;
			}
			else
			{
				//투명한 부분임
				_col = GetPixel( m_hDC, x-m_iOutLineSize, y-m_iOutLineSize );
				if( _col == black )
				{
					*(pFirst + x + cache->iLeft ) = MYD3DCOLOR_ARGB(0,0,0,0);
				}
				else
				{
					//alpha 를 0~f로 표현하자
					// ARGB : 1 5 5 5 mode에서 사용
					int iCol = (int)((float)GetRValue(_col) * 0x1f / 255.0f);
					*(pFirst + x + cache->iLeft ) = MYD3DCOLOR_ARGB(1,iCol,iCol,iCol);
					/*
					int iCol = (int)((float)GetRValue(_col));// * 255.0f / 255.0f);
					*(pFirst + x + cache->iLeft ) = MYD3DCOLOR_ARGB(iCol,iCol,iCol,iCol);
					*/
				}
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------
	// 아웃라인 중심이 벗어나면 안되니깐.. 직접 아래와 같이 코딩합니다
	if( m_iOutLineSize != 0 )
	{
		MYFONTCOLOR_PTR pFirstFont      = (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits;
		MYFONTCOLOR_PTR pFirstOutLine   = (MYFONTCOLOR_PTR)m_pTextureOutLine[cache->iPage]->m_LockedRect.pBits;
		MYFONTCOLOR_PTR pColor;
		bool            bFind;

		for(int y = 0; y < cache->iHeight; y++)
		{
			pFirstOutLine = (MYFONTCOLOR_PTR)m_pTextureOutLine[cache->iPage]->m_LockedRect.pBits + (y + cache->iTop ) * FONT_CACHE_SIZE;

			for(int x = 0; x < cache->iWidth; x++)
			{
				bFind = false;

				// Outline만큼... 체크해보자.
				for(int yy = max(0, y-m_iOutLineSize); yy <= min( cache->iHeight - 1, y + m_iOutLineSize ) && !bFind; yy++ )
				{
					pFirstFont = (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits + (yy + cache->iTop ) * FONT_CACHE_SIZE;

					// *(pFirstOutLine + x + cache->iLeft ) = *(pFirstFont + max(0, x-m_iOutLineSize) + cache->iLeft );
					// bFind = true;
					// break;

					for(int xx = max(0, x-m_iOutLineSize); xx <= min( cache->iWidth - 1, x + m_iOutLineSize ) && !bFind; xx++ )
					{
						pColor = (pFirstFont + xx + cache->iLeft );
						if( MYFONTCOLOR_ALPHA(*pColor) > 0 )
						{
							*(pFirstOutLine + x + cache->iLeft ) = *pColor;
							bFind = true;
						}
					}
				}

				if( bFind == false )
				{
					*(pFirstOutLine + x + cache->iLeft ) = 0;
				}
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------
}

/// draw multi-lined text.
/// @param  iLeft: left-position of first char of text string(screen coordinate)
/// @param  iTop: top-position of first char of text string
/// @param  pszText: [in] output text
/// @param  fLineSpace: line space ratio
///         - ex) fLineSpace == 2.f means double space
/// @note   we use '\n' and "\\n" as line breaking special characters
///         so "Hello\nWorld" and "Hello\\nWorld" will be rendered same format.
void GCDeviceFontTH::OutTextMultiline( int iLeft, int iTop, const WCHAR* wszText, D3DCOLOR color /*= 0xffffffff*/, float fLineSpace /*= 1.0f*/, RECT* pRt /*= NULL*/, DWORD dwFlag /*= DT_LEFT|DT_TOP*/, D3DCOLOR colorOutLine /*= 0xff000000*/, float fScale /*= 0*/, float fFontSpace /*= 0*/)
{
	if( m_eDeviceState == GCDS_DISABLE )
		return;

	if( wcslen(wszText) == 0 )
		return;

	if (fScale == 0)
	{
		//Font 는 Wide모니터는 있어도 세로로 긴 모니터는 말이 안되서 Y축 기준이다.
		fLineSpace=fLineSpace/g_pGCDeviceManager->GetWindowScaleY();
	}
	else
	{
		fLineSpace=fLineSpace/fScale;
	}

	m_dwCurrentTime = timeGetTime();

	static WCHAR			wstrBuf[VIRTUAL_FONT_LINE_MAX];
	wcscpy( wstrBuf, wszText );

	WCHAR*			pToken = wstrBuf;
	int iSize       = (int)wcslen(wstrBuf);
	bool bNewLine = false;
	int iTopLocal = iTop;

	D3DCOLOR curcol;

	if( m_iOutLineSize )
	{
		curcol = colorOutLine;

		for(int i = 0; i < iSize; i++)
		{
			if( wstrBuf[i] == L'\n' )
				bNewLine = true;

			if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' || i == iSize - 1 )
			{
				bNewLine = false;
				if( wstrBuf[i] == L'\n' )
					bNewLine = true;
				if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' )
					wstrBuf[i] = 0;

				int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag );

				Buffering(iLineLeft, iTopLocal, pToken, curcol, color, true , fFontSpace);

				if( bNewLine )
				{
					if( m_bRHW == true )
						iTopLocal += (int)(m_iFontCacheSize * fLineSpace);
					else
						iTopLocal -= (int)(m_iFontCacheSize * fLineSpace);
				}

				pToken = &wstrBuf[i+1];
			}
		}

		Flush(true);
	}

	wcscpy( wstrBuf, wszText );
	pToken = wstrBuf;
	curcol = color;
	iTopLocal = iTop;

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

			Buffering(iLineLeft, iTopLocal, pToken, curcol, color, (dwFlag & DT_NO_COLOR) != 0 , fFontSpace );

			//Display Here
			if( bNewLine )
			{
				if( m_bRHW == true )
					iTopLocal += (int)(m_iFontCacheSize * fLineSpace);
				else
					iTopLocal -= (int)(m_iFontCacheSize * fLineSpace);
			}

			pToken = &wstrBuf[i+1];
		}
	}

	Flush(false);
}

void GCDeviceFontTH::OutTextInTexture( LPDIRECT3DTEXTURE9 dest_Texture, int iLeft, int iTop, const WCHAR* str, D3DCOLOR DefColor /*= 0xffffffff*/, RECT* pRt /*= NULL*/, DWORD dwFlag /*= DT_LEFT|DT_TOP*/, D3DCOLOR colorOutLine /*= 0xff000000 */ )
{
	int isize = (int)wcslen(str);
	iLeft = GetLeftPos( iLeft, str, pRt, dwFlag );
	iTop = GetTopPos( iTop,str, pRt, dwFlag );
	SGCFontCacheTH *cache;

	D3DLOCKED_RECT rect;
	dest_Texture->LockRect(0,&rect, NULL, 0);
	DWORD* dest = (DWORD*)rect.pBits+FONT_CACHE_SIZE*iTop+iLeft;

	for(int i = 0; i < isize; ++i )
	{
		if( str[i] == L'\n' )
			continue;
		if( str[i] == L'\r' )
			continue;

		int test1 = (int)(GCUTIL_STR_THAI::CharNextTh(&str[i]));
		int test2 = (int)(&str[i]);
		int test3 = sizeof(WCHAR);
		int charSize = (int)(GCUTIL_STR_THAI::CharNextTh(&str[i]) - &str[i]);

		std::wstring wstr;
		for (int charIndex=0;charIndex<charSize;charIndex++)
		{
			wstr+=str[i+charIndex];
		}

		cache= GetFontCacheTH(wstr);
		DWORD* dest2 = dest;
		dest+=cache->iWidth;

		WORD* src_out = (WORD*)m_pTextureOutLine[cache->iPage]->m_LockedRect.pBits + cache->iTop * FONT_CACHE_SIZE + cache->iLeft;
		WORD* src = (WORD*)m_pTexture[cache->iPage]->m_LockedRect.pBits + cache->iTop * FONT_CACHE_SIZE + cache->iLeft;

		int height = cache->iHeight;
		for(int y=0;y<cache->iHeight;y++)
		{
			for(int x=0;x<cache->iWidth;x++)
			{
				if(src_out[x] && !src[x])
					dest2[x]=colorOutLine;
				else if(src[x])
					dest2[x]=DefColor;
			}
			src+=FONT_CACHE_SIZE;
			src_out+=FONT_CACHE_SIZE;
			dest2+=FONT_CACHE_SIZE;
		}
		i+=(charSize-1); //여러개 조합된 경우
	}

	dest_Texture->UnlockRect(0);
}

void GCDeviceFontTH::Buffering(int iLeft, int iTop, const WCHAR* str, D3DCOLOR& color, D3DCOLOR DefColor,bool bNoColor, int iFontSpace )
{
	int isize = (int)wcslen(str);

	for(int i = 0; i < isize; ++i )
	{
		if( str[i] == L'\n' )
			continue;
		if( str[i] == L'\r' )
			continue;

		// Color Change
		if( str[i] == L'#' && i != isize-1 && (str[i+1] == L'C' || str[i+1] == L'c' ) )
		{
			if( str[i+2] == L'X' || str[i+2] == L'x' )
			{
				i += 2;
				if( !bNoColor )
					color = DefColor;
				continue;
			}
			else
			{
				if( !bNoColor )
				{
					// #C R G B형식으로 들어있음
					// #CFF0000 -> Red
					int iCol;
					WCHAR strColor[7] = { 0, };
					wcsncpy(strColor,&str[i+2],6);
					swscanf(strColor,L"%x",&iCol);
					color = ( color & 0xFF000000 ) | iCol;
				}
			}
			i += 7;
			continue;
		}

		int charSize = (int)(GCUTIL_STR_THAI::CharNextTh(&str[i]) - &str[i]);

		std::wstring wstr;
		for (int charIndex=0;charIndex<charSize;charIndex++)
		{
			wstr+=str[i+charIndex];
		}

		SGCFontCacheTH *cache = GetFontCacheTH(wstr);

		if( m_bRHW == true )
		{
			KD3DUIVertex* pVertex = NULL;
			pVertex = &m_pUIVertex[cache->iPage][m_iCandidate[cache->iPage] * 4];
			pVertex[0].SetVertexDiff( (float)iLeft, (float)iTop,                                  color, cache->_texLeft, cache->_texTop );
			pVertex[1].SetVertexDiff( (float)(iLeft + cache->iWidth), (float)iTop,                  color, cache->_texRight, cache->_texTop );
			pVertex[2].SetVertexDiff( (float)iLeft, (float)(iTop + cache->iHeight),                 color, cache->_texLeft, cache->_texBottom );
			pVertex[3].SetVertexDiff( (float)(iLeft + cache->iWidth), (float)(iTop + cache->iHeight), color, cache->_texRight, cache->_texBottom);
		}
		else
		{
			SLVERTEX* pVertex = NULL;
			pVertex = &m_pVertex[cache->iPage][m_iCandidate[cache->iPage] * 4];
			pVertex[0].SetSLVERDiff( (float)iLeft, (float)iTop,                                  color, cache->_texLeft, cache->_texTop );
			pVertex[1].SetSLVERDiff( (float)(iLeft + cache->iWidth), (float)iTop,                  color, cache->_texRight, cache->_texTop );
			pVertex[2].SetSLVERDiff( (float)iLeft, (float)(iTop + cache->iHeight),                 color, cache->_texLeft, cache->_texBottom );
			pVertex[3].SetSLVERDiff( (float)(iLeft + cache->iWidth), (float)(iTop + cache->iHeight), color, cache->_texRight, cache->_texBottom);
		}

		iLeft += (cache->iWidth - iFontSpace) ;
		m_iCandidate[cache->iPage]++;
		i+=(charSize-1); //여러개 조합된 경우
	}
}
