#include "stdafx.h"
#include ".\gcdeviceindexbuffer.h"
#include "KJohnGen.h"

//
extern LPDIRECT3DDEVICE9        g_pd3dDevice;

// 기본 2byte짜리 인덱스로 Format을 잡는다.
GCDeviceIndexBuffer::GCDeviceIndexBuffer( std::string& strDeviceID_, UINT uiLength_, DWORD dwUsage_
										 , D3DFORMAT Format_, D3DPOOL Pool_, void* pData )
{
	m_strDeviceID      = strDeviceID_;
	m_uiLength      = uiLength_;
	m_dwUsage       = dwUsage_;
	m_Format        = Format_;
	m_Pool          = Pool_;
	m_eDeviceType	= GCDT_INDEX_BUFFER;
	m_pBuffer       = NULL;
	m_pData			= NULL;

	g_pd3dDevice->CreateIndexBuffer( m_uiLength, m_dwUsage, m_Format, m_Pool, &m_pBuffer, NULL );

	m_pData = new BYTE[uiLength_];

	if( pData != NULL )
	{
		memcpy(m_pData,pData,m_uiLength);
	}
	else
		memset(m_pData,0,uiLength_);

	SystemToVideo();
}

GCDeviceIndexBuffer::~GCDeviceIndexBuffer(void)
{
	UnLoad();
}

void GCDeviceIndexBuffer::SystemToVideo()
{
	BYTE* pTempData = NULL;
	m_pBuffer->Lock( 0, 0, (void**)&pTempData, 0 );
	memcpy( pTempData, m_pData, m_uiLength );
	m_pBuffer->Unlock();
}

void GCDeviceIndexBuffer::OnResetDevice()
{
	SAFE_RELEASE( m_pBuffer );
	g_pd3dDevice->CreateIndexBuffer( m_uiLength, m_dwUsage, m_Format, m_Pool, &m_pBuffer, NULL );
	SystemToVideo();
}

void GCDeviceIndexBuffer::OnLostDevice()
{
	SAFE_RELEASE( m_pBuffer );
}

bool GCDeviceIndexBuffer::Load()
{
	return true;
}

void GCDeviceIndexBuffer::UnLoad()
{
	/************************************************************************/
	/*	System & Video Memory Release
	/************************************************************************/
	SAFE_RELEASE( m_pBuffer );
	SAFE_DELETE_ARRAY( m_pData );
}

BYTE* GCDeviceIndexBuffer::Lock( BYTE* ppbData )
{
	return m_pData;
}

void GCDeviceIndexBuffer::Unlock()
{
	/************************************************************************/
	/*	System Memory => Video Memory
	/************************************************************************/
	SystemToVideo();
}

HRESULT GCDeviceIndexBuffer::VideoLock( UINT offsetToLock, UINT sizeToLock, BYTE** ppbData, DWORD flag )
{
	HRESULT hr;
	if( FAILED( hr = m_pBuffer->Lock( offsetToLock, sizeToLock, (void**)ppbData, flag ) ) )
	{
		assert( !"Failed to Lock Index Buffer" );
	}
	return hr;
}

HRESULT GCDeviceIndexBuffer::VideoUnlock()
{
	HRESULT hr;
	if( FAILED( hr = m_pBuffer->Unlock() ) )
	{
		assert( !"Failed to Unlock Index Buffer" );
	}
	return hr;
}
