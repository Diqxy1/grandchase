#include "stdafx.h"
#include ".\gcdevicebuildtexture.h"

void STextureSection::Set(int left,int top,int width,int height ,int tex_width,int tex_height)
{
	rect.left=left;
	rect.right=left+width;
	rect.top = top;
	rect.bottom=top+height;

	uv.x=(float)width / tex_width;
	uv.y=(float)height / tex_height;

	uv.z=(float)left / tex_width;
	uv.w=(float)top / tex_height;
}

GCTmpTexture::GCTmpTexture( LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName )
	:GCBaseTexture(d3ddevice)
{
	m_eDeviceType = GCDT_TEMP_TEXTURE;
	m_strFileName = strFileName;
	m_strDeviceID = strFileName+"tmp";
	m_dwFormat = D3DFMT_UNKNOWN;
}

bool GCTmpTexture::Load()
{
	if( m_pTexture != NULL )
		return false;
	assert(m_pTexture == NULL);

	GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(), m_strFileName );

	if( !memfile.IsLoaded() )
	{
		m_pTexture = NULL;

		//assert(m_pTexture != NULL);
		m_eDeviceState = GCDS_DISABLE;
		return false;
	}

	D3DXIMAGE_INFO		TexInfo;

	if( FAILED(D3DXCreateTextureFromFileInMemoryEx( m_pd3dDevice, (void*)memfile.GetDataPointer(), memfile.GetFileSize(),
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN,
		D3DPOOL_SYSTEMMEM, D3DX_FILTER_LINEAR ,D3DX_FILTER_LINEAR  , 0,
		&TexInfo,
		NULL, &m_pTexture ) ))
	{
		m_pTexture = NULL;
		assert(m_pTexture != NULL);
		return false;
	}
	m_dwWidth = TexInfo.Width;
	m_dwHeight = TexInfo.Height;
	m_eDeviceState = GCDS_LOADED;
	m_dwFormat = TexInfo.Format;

	return true;
}

void GCTmpTexture::UnLoad()
{
	if(m_eDeviceState != GCDS_LOADED)
		return;
	SAFE_RELEASE(m_pTexture);
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_eDeviceState = GCDS_NOT_LOADED;
}

void GCTmpTexture::SetWithLoad( int stage /*=0*/ )
{
	ASSERT(!"여기로 오면 안되!");
}

DWORD GCTmpTexture::GetDeviceSize()
{
	//32bit 이미지라고 생각함
	return 4 * m_dwWidth * m_dwHeight;
}

GCTmpTexture::~GCTmpTexture( void )
{
	UnLoad();
}

void GCTmpTexture::CopyTo( LPDIRECT3DSURFACE9 dest_surface, RECT *dest_rect , RECT *src_rect,D3DCOLOR col)
{
	LPDIRECT3DSURFACE9 src_surface;
	m_pTexture->GetSurfaceLevel(0,&src_surface);

	D3DXLoadSurfaceFromSurface(dest_surface,NULL,dest_rect,
		src_surface,NULL,src_rect,
		D3DX_FILTER_LINEAR,col);

	SAFE_RELEASE(src_surface);
}

GCBuildTexture::GCBuildTexture( LPDIRECT3DDEVICE9 d3ddevice, D3DFORMAT format,bool usetext,DWORD width, DWORD height)
	:GCBaseTexture(d3ddevice),m_section_height(PIXEL_TO_SEXEL(height)),m_section_width(PIXEL_TO_SEXEL(width))
{
	m_bBuildNickNameTex = true;

	m_dwWidth = width;
	m_dwHeight = height;

	m_bUseTexture = new bool[m_section_height * m_section_width];
	memset(m_bUseTexture,0,sizeof(bool)*m_section_height * m_section_width);

	if(usetext)
	{
		SetSection(0,0,PIXEL_TO_SEXEL(256) , PIXEL_TO_SEXEL(128),true);
	}

	if( FAILED(D3DXCreateTexture( m_pd3dDevice, width,height,1, 0, format,	D3DPOOL_MANAGED, &m_pTexture ) ))
	{
		m_pTexture = NULL;
		assert(m_pTexture != NULL);
	}
}

GCBuildTexture::~GCBuildTexture(void)
{
	SAFE_DELETE_ARRAY(m_bUseTexture);
}

bool GCBuildTexture::AddTexture( std::string texture,STextureSection* section,bool precalculated_section/*=false*/)
{
	if( NULL == section )
		return false;

	bool re=false;
	GCTmpTexture * src_Texture = g_pGCDeviceManager->CreateTmpTexture(texture);

	if( NULL == src_Texture )
		return false;

	if(src_Texture->IsLoaded())
	{
		if ( precalculated_section == false &&
			( src_Texture->GetFileFormat() == D3DFMT_DXT3 ||
			src_Texture->GetFileFormat() == D3DFMT_DXT5 ||
			src_Texture->GetFileFormat() == D3DFMT_A8R8G8B8 ) )
		{
			SAFE_RELEASE(src_Texture)
				return false;
		}

		LPDIRECT3DSURFACE9 dest_Surface;
		if (m_pTexture)
		{
			if(m_pTexture->GetSurfaceLevel(0,&dest_Surface)==D3D_OK)
			{
				if(precalculated_section)
				{
					src_Texture->CopyTo(dest_Surface,&section->rect);
					re=true;
				}
				else
				{
					if(GetFreeTextureSection(section,PIXEL_TO_SEXEL(src_Texture->GetWidth()),PIXEL_TO_SEXEL(src_Texture->GetHeight())))
					{
						src_Texture->CopyTo(dest_Surface,&section->rect);
						re= true;
					}
				}
				dest_Surface->Release();
			}
		}
	}
	SAFE_RELEASE(src_Texture);

	return re;
}

bool GCBuildTexture::AddTexture(GCTmpTexture* src_Texture,STextureSection* section,D3DXVECTOR4 * uv )
{
	bool re=false;
	if( NULL == section )
		return re;

	if( NULL == src_Texture )
		return re;

	if( NULL == uv )
		return re;

	if(src_Texture->IsLoaded())
	{
		RECT src_rect;
		src_rect.left = (long)(uv->x*src_Texture->GetWidth());
		src_rect.right = (long)(uv->z*src_Texture->GetWidth());
		src_rect.top = (long)(uv->y*src_Texture->GetHeight());
		src_rect.bottom = (long)(uv->w*src_Texture->GetHeight());

		LPDIRECT3DSURFACE9 dest_Surface;
		if( m_pTexture->GetSurfaceLevel(0,&dest_Surface)== D3D_OK)
		{
			if( GetFreeTextureSection(section,PIXEL_TO_SEXEL(src_rect.right-src_rect.left),PIXEL_TO_SEXEL( src_rect.bottom-src_rect.top ) ) )
			{
				src_Texture->CopyTo(dest_Surface,&section->rect,&src_rect);
				re= true;
			}

			SAFE_RELEASE(dest_Surface);
		}
	}

	return re;
}

// 국가 빌드 옵션 영향이 미치지 않는 Device쪽에 수정 작업이라. Define으로 처리가 안됨.
// 사실 태국 폰트 객체만 해도 정상적으로 동작한다. 하지만, 영향이 있을까봐
// 일단은 같은 함수 두개 둔다.

void GCBuildTexture::TextDraw( GCDeviceFontTH* font, int left,int top,std::wstring str,D3DCOLOR color /*= 0xffffffff*/, RECT* pRt /*= NULL*/, DWORD dwFlag /*= DT_LEFT|DT_TOP*/, D3DCOLOR colorOutLine /*= 0xff000000 */ )
{
	LPDIRECT3DTEXTURE9  src_Texture = NULL;

	if( FAILED(D3DXCreateTexture( m_pd3dDevice, 256,256,1, D3DUSAGE_DYNAMIC, D3DFMT_A8B8G8R8,	D3DPOOL_SYSTEMMEM, &src_Texture ) ))
	{
		src_Texture = NULL;
		assert(src_Texture != NULL);

		m_bBuildNickNameTex = false;
	}

	if(src_Texture)
	{
		font->OutTextInTexture(src_Texture,left,top,str.c_str(),color,pRt,dwFlag,colorOutLine);

		LPDIRECT3DSURFACE9 dest_Surface;
		LPDIRECT3DSURFACE9 src_Surface;
		m_pTexture->GetSurfaceLevel(0,&dest_Surface);
		src_Texture->GetSurfaceLevel(0,&src_Surface);

		D3DXLoadSurfaceFromSurface(dest_Surface,NULL,pRt,
			src_Surface,NULL,pRt,
			D3DX_DEFAULT,0);

		dest_Surface->Release();
		src_Surface->Release();
		src_Texture->Release();

		m_bBuildNickNameTex = true;
	}
}

void GCBuildTexture::TextDraw( GCDeviceFontBase* font, int left,int top,std::wstring str,D3DCOLOR color /*= 0xffffffff*/, RECT* pRt /*= NULL*/, DWORD dwFlag /*= DT_LEFT|DT_TOP*/, D3DCOLOR colorOutLine /*= 0xff000000 */ )
{
	LPDIRECT3DTEXTURE9  src_Texture = NULL;

	if( FAILED(D3DXCreateTexture( m_pd3dDevice, 256,256,1, D3DUSAGE_DYNAMIC, D3DFMT_A8B8G8R8,	D3DPOOL_SYSTEMMEM, &src_Texture ) ))
	{
		src_Texture = NULL;
		assert(src_Texture != NULL);

		m_bBuildNickNameTex = false;
	}

	if(src_Texture)
	{
		font->OutTextInTexture(src_Texture,left,top,str.c_str(),color,pRt,dwFlag,colorOutLine);

		LPDIRECT3DSURFACE9 dest_Surface;
		LPDIRECT3DSURFACE9 src_Surface;
		m_pTexture->GetSurfaceLevel(0,&dest_Surface);
		src_Texture->GetSurfaceLevel(0,&src_Surface);

		D3DXLoadSurfaceFromSurface(dest_Surface,NULL,pRt,
			src_Surface,NULL,pRt,
			D3DX_DEFAULT,0);

		dest_Surface->Release();
		src_Surface->Release();
		src_Texture->Release();

		m_bBuildNickNameTex = true;
	}
}

void GCBuildTexture::SetWithLoad( int stage )
{
	SetDeviceTexture(stage);
}

void GCBuildTexture::SetFreeTextureSection( STextureSection *section )
{
	SetSection(PIXEL_TO_SEXEL(section->rect.left),
		PIXEL_TO_SEXEL(section->rect.top),
		PIXEL_TO_SEXEL(section->rect.right-section->rect.left),
		PIXEL_TO_SEXEL(section->rect.bottom-section->rect.top),false);
}

bool GCBuildTexture::GetFreeTextureSection( STextureSection *section,DWORD width ,DWORD height )
{
	for(DWORD y=0;y<m_section_height-height+1;y++)
	{
		for(DWORD x=0;x<m_section_width-width+1;x++)
		{
			if(IsUseSection(x,y,width,height)==false)
			{
				SetSection(x,y,width,height,true);
				section->Set(SEXEL_TO_PIXEL(x),SEXEL_TO_PIXEL(y),SEXEL_TO_PIXEL(width),SEXEL_TO_PIXEL(height));
				return true;
			}
		}
	}
	return false;
}

bool GCBuildTexture::IsUseSection( int x,int y,int width,int height )
{
	for(int j=y;j<y+height;j++)
	{
		for(int i=x;i<x+width;i++)
		{
			if(m_bUseTexture[j*m_section_width+i])
				return true;
		}
	}
	return false;
}

void GCBuildTexture::SetSection( int x,int y,int width,int height,bool use )
{
	for(int j=y;j<y+height;j++)
	{
		for(int i=x;i<x+width;i++)
		{
			m_bUseTexture[j*m_section_width+i]=use;
		}
	}
}
