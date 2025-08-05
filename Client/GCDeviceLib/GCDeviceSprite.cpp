#include "stdafx.h"
#include ".\gcdevicesprite.h"
#include "KJohnGen.h"

GCDeviceSprite::GCDeviceSprite(LPDIRECT3DDEVICE9 d3ddevice)
{
	m_pSprite = NULL;
	m_eDeviceType = GCDT_SPRITE;
	m_pd3ddevice = d3ddevice;
}

GCDeviceSprite::~GCDeviceSprite(void)
{
	UnLoad();
}

void GCDeviceSprite::OnResetDevice()
{
	m_pSprite->OnResetDevice();
}

void GCDeviceSprite::OnLostDevice()
{
	m_pSprite->OnLostDevice();
}

bool GCDeviceSprite::Load()
{
	SAFE_RELEASE( m_pSprite );

	HRESULT hr = D3DXCreateSprite( m_pd3ddevice, &m_pSprite );
	if ( FAILED( hr ) )
	{
		m_eDeviceState = GCDS_DISABLE;
		return false;
	}
	m_eDeviceState = GCDS_LOADED;
	return true;
}

void GCDeviceSprite::UnLoad()
{
	SAFE_RELEASE( m_pSprite );
	m_eDeviceState = GCDS_NOT_LOADED;
}
