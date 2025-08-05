#include "stdafx.h"
#include ".\gcdynamictexture.h"

GCDynamicTexture::GCDynamicTexture(void)
	:GCBaseTexture(NULL)
{
	m_d3dFormat = D3DFMT_A8R8G8B8;
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_bUpdateTexture = false;
	m_pTextureSys = NULL;
}

GCDynamicTexture::~GCDynamicTexture(void)
{
	SAFE_RELEASE(m_pTextureSys);
}

bool GCDynamicTexture::CreateTexture(LPDIRECT3DDEVICE9 pd3ddevice, DWORD dwWidth, DWORD dwHeight, D3DFORMAT d3dFormat)
{
	m_dwWidth  = dwWidth;
	m_dwHeight = dwHeight;
	m_d3dFormat = d3dFormat;
	m_pd3dDevice = pd3ddevice;
	HRESULT hr = S_OK;

	if( FAILED(hr = D3DXCreateTexture( m_pd3dDevice, m_dwWidth, m_dwHeight, 1, NULL, d3dFormat, D3DPOOL_SYSTEMMEM, &m_pTextureSys ) ) )
	{
		return false;
	}
	if( FAILED( D3DXCreateTexture( m_pd3dDevice, m_dwWidth, m_dwHeight, 1, NULL, d3dFormat, D3DPOOL_DEFAULT, &m_pTexture ) ) )
	{
		return false;
	}

	m_pTextureSys->LockRect(0,&m_LockedRect, NULL, 0);
	return true;
}

void GCDynamicTexture::ResetDevice()
{
	SAFE_RELEASE(m_pTexture);
	if( FAILED( D3DXCreateTexture( m_pd3dDevice, m_dwWidth, m_dwHeight, 1, NULL, m_d3dFormat, D3DPOOL_DEFAULT, &m_pTexture ) ) )
	{
		//이것조차 실패하면 할말 없음
		return;
	}

	m_pTextureSys->UnlockRect(0);
	m_pd3dDevice->UpdateTexture( m_pTextureSys, m_pTexture );
	m_pTextureSys->LockRect(0,&m_LockedRect, NULL, 0);
}

void GCDynamicTexture::LostDevice()
{
	SAFE_RELEASE(m_pTexture);
}

void GCDynamicTexture::SetTexture( int iStage )
{
	if( m_bUpdateTexture )
	{
		m_pTextureSys->UnlockRect(0);
		m_pd3dDevice->UpdateTexture( m_pTextureSys, m_pTexture );
		m_pTextureSys->LockRect(0,&m_LockedRect, NULL, 0);
		m_bUpdateTexture = false;
	}
	m_pd3dDevice->SetTexture(iStage, m_pTexture );
}

void GCDynamicTexture::SetWithLoad( int stage /*=0*/ )
{
	SetTexture( stage );
}

void GCDynamicTexture::Release()
{
	delete this;
}
