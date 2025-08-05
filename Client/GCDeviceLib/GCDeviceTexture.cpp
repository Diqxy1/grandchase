#include "stdafx.h"
#include ".\gcdevicetexture.h"
#include "TextureLoadManager.h"

static FILE* pFile = NULL;

GCDeviceTexture::GCDeviceTexture(LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName, bool bLowQuality, bool bLoadFromRealpath, bool bLoadThread )
	:GCBaseTexture(d3ddevice)
{
	m_eDeviceType = GCDT_TEXTURE;
	m_strDeviceID   = strFileName;
	m_bLowQuality = bLowQuality;
	m_dwFormat = D3DFMT_UNKNOWN;
	m_dwImageFileFormat = D3DXIFF_FORCE_DWORD;
	m_bLoadThread = bLoadThread;
	m_bWaitLoad = false;
	m_bLoadFromRealpath = bLoadFromRealpath;
}

GCDeviceTexture::~GCDeviceTexture(void)
{
	UnLoad();
}

void GCDeviceTexture::GetThreadTexture()
{
	if ( !m_bLoadThread )
		return;

	if ( SiKTextureLoadManager()->IsComplateTexture( m_strDeviceID ) )
	{
		TexLoadData sTexLoadData;
		SiKTextureLoadManager()->GetComplateTexture( sTexLoadData, m_strDeviceID );

		if ( sTexLoadData.m_pTexture )
		{
			m_dwWidth = sTexLoadData.TexInfo.Width;
			m_dwHeight = sTexLoadData.TexInfo.Height;
			m_eDeviceState = GCDS_LOADED;
			m_dwFormat = sTexLoadData.TexInfo.Format;
			m_dwImageFileFormat = sTexLoadData.TexInfo.ImageFileFormat;
			m_pTexture = sTexLoadData.m_pTexture;
		}

		m_bWaitLoad = false;
	}
}

bool GCDeviceTexture::Load()
{
	if( m_pTexture != NULL )
		return false;
	assert(m_pTexture == NULL);

	if ( m_bLoadThread )
	{
		if ( m_bWaitLoad )
		{
			GetThreadTexture();
			return true;
		}

		GCThreadMemoryFile* pMemfile = new GCThreadMemoryFile(g_pGCDeviceManager->GetMassFileManager(), m_strDeviceID );
		if( !pMemfile->IsFileValid())
		{
			m_pTexture = NULL;

			//assert(m_pTexture != NULL);
			m_eDeviceState = GCDS_DISABLE;
			return false;
		}

		TexLoadData sTexLoadData;
		sTexLoadData.strTexName = m_strDeviceID;
		sTexLoadData.pMemFile = pMemfile;
		sTexLoadData.bLowQuality = m_bLowQuality;

		SiKTextureLoadManager()->AddTextureList( sTexLoadData );
		m_bWaitLoad = true;
	}
	else
	{
		D3DXIMAGE_INFO		TexInfo;

		if (m_bLoadFromRealpath == false)
		{
			GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(), m_strDeviceID );

			if( !memfile.IsLoaded())
			{
				m_pTexture = NULL;

				//assert(m_pTexture != NULL);
				m_eDeviceState = GCDS_DISABLE;
				return false;
			}

			if( FAILED(D3DXCreateTextureFromFileInMemoryEx( m_pd3dDevice, (void*)memfile.GetDataPointer(), memfile.GetFileSize(),
				D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, m_bLowQuality ? D3DFMT_DXT3 : D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
				&TexInfo,
				NULL, &m_pTexture ) ))
			{
				m_pTexture = NULL;
				assert(m_pTexture != NULL);
				return false;
			}
		}
		else //실제 경로에서 읽어들이는 경우...
		{
			D3DXCreateTextureFromFileEx( m_pd3dDevice, m_strDeviceID.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
				m_bLowQuality ? D3DFMT_DXT3 : D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
				&TexInfo,
				NULL, &m_pTexture );
		}

		m_dwWidth = TexInfo.Width;
		m_dwHeight = TexInfo.Height;
		m_eDeviceState = GCDS_LOADED;
		m_dwFormat = TexInfo.Format;
		m_dwImageFileFormat = TexInfo.ImageFileFormat;
	}

	return true;
}

void GCDeviceTexture::UnLoad()
{
	if(m_eDeviceState != GCDS_LOADED)
		return;

	SAFE_RELEASE(m_pTexture);
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_eDeviceState = GCDS_NOT_LOADED;
	m_bWaitLoad = false;
}

DWORD GCDeviceTexture::GetDeviceSize()
{
	//32bit 이미지라고 생각함
	return 4 * m_dwWidth * m_dwHeight;
}

void GCDeviceTexture::SetWithLoad( int stage /*=0*/ )
{
	if ( m_bLoadThread )
	{
		if( this->GetDeviceState() == GCDS_NOT_LOADED )
		{
			if ( m_bWaitLoad )
			{
				GetThreadTexture();
			}
			else
			{
				this->AddRefCount();
				ASSERT(""!=m_strDeviceID);
			}
		}

		GCBaseTexture::SetDeviceTexture(stage);
	}
	else
	{
		if( this->GetDeviceState() == GCDS_NOT_LOADED )
		{
			this->AddRefCount();
			ASSERT(""!=m_strDeviceID);
		}

		GCBaseTexture::SetDeviceTexture(stage);
	}
}

void GCDeviceTexture::SetDeviceTexture( int stage )
{
	SetWithLoad( stage );
}

GCImageIconTexture::GCImageIconTexture(LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName, bool bLowQuality, bool bLoadFromRealpath)
	:GCBaseTexture(d3ddevice)
{
	m_eDeviceType = GCDT_TEXTURE;
	m_strDeviceID   = strFileName;
	m_bLowQuality = bLowQuality;
	m_dwFormat = D3DFMT_UNKNOWN;
	m_dwImageFileFormat = D3DXIFF_FORCE_DWORD;
	m_bLoadFromRealpath = bLoadFromRealpath;
}

GCImageIconTexture::~GCImageIconTexture(void)
{
	UnLoad();
}

bool GCImageIconTexture::Load()
{
	if( m_pTexture != NULL )
		return false;
	assert(m_pTexture == NULL);

	D3DXIMAGE_INFO		TexInfo;

	if (m_bLoadFromRealpath == false)
	{
		GCMemoryFile memfile(g_pGCDeviceManager->GetMassFileManager(), m_strDeviceID );

		if( !memfile.IsLoaded())
		{
			m_pTexture = NULL;

			//assert(m_pTexture != NULL);
			m_eDeviceState = GCDS_DISABLE;
			return false;
		}

		if( FAILED(D3DXCreateTextureFromFileInMemoryEx( m_pd3dDevice, (void*)memfile.GetDataPointer(), memfile.GetFileSize(),
			SIZE_IMAGEICON, SIZE_IMAGEICON, 1, 0, m_bLowQuality ? D3DFMT_DXT3 : D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
			&TexInfo,
			NULL, &m_pTexture ) ))
		{
			m_pTexture = NULL;
			assert(m_pTexture != NULL);
			return false;
		}
	}
	else //실제 경로에서 읽어들이는 경우...
	{
		D3DXCreateTextureFromFileEx( m_pd3dDevice, m_strDeviceID.c_str(), SIZE_IMAGEICON, SIZE_IMAGEICON, 1, 0,
			m_bLowQuality ? D3DFMT_DXT3 : D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, 0,
			&TexInfo,
			NULL, &m_pTexture );
	}
	m_dwWidth = TexInfo.Width;
	m_dwHeight = TexInfo.Height;
	m_eDeviceState = GCDS_LOADED;
	m_dwFormat = TexInfo.Format;
	m_dwImageFileFormat = TexInfo.ImageFileFormat;
	return true;
}

void GCImageIconTexture::UnLoad()
{
	if(m_eDeviceState != GCDS_LOADED)
		return;

	SAFE_RELEASE(m_pTexture);
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_eDeviceState = GCDS_NOT_LOADED;
}

DWORD GCImageIconTexture::GetDeviceSize()
{
	//32bit 이미지라고 생각함
	return 4 * m_dwWidth * m_dwHeight;
}

void GCImageIconTexture::SetWithLoad( int stage /*=0*/ )
{
	if( this->GetDeviceState() == GCDS_NOT_LOADED )
	{
		this->AddRefCount();
		ASSERT(""!=m_strDeviceID);
	}
	this->SetDeviceTexture(stage);
}
