#include "stdafx.h"
#include ".\gcdevicevertexbuffer.h"
#include "KJohnGen.h"

//
extern LPDIRECT3DDEVICE9        g_pd3dDevice;

GCDeviceVertexBuffer::GCDeviceVertexBuffer( std::string& strDeviceID_
										   , UINT uiLength_, DWORD dwUsage_, DWORD dwFVF_, D3DPOOL Pool_
										   , void* pData )
{
	m_strDeviceID      = strDeviceID_;
	m_uiLength      = uiLength_;
	m_dwUsage       = dwUsage_;
	m_dwFVF         = dwFVF_;
	m_Pool          = Pool_;
	m_eDeviceType	= GCDT_VERTEX_BUFFER;
	m_pBuffer       = NULL;
	m_pData			= NULL;
	g_pd3dDevice->CreateVertexBuffer( m_uiLength, m_dwUsage, m_dwFVF, m_Pool, &m_pBuffer, NULL );

	m_pData = new BYTE[uiLength_];

	if( pData != NULL )
	{
		memcpy(m_pData,pData,m_uiLength);
	}
	else
	{
		memset(m_pData,0,uiLength_);
	}
	SystemToVideo();
}

GCDeviceVertexBuffer::~GCDeviceVertexBuffer(void)
{
	UnLoad();
}

void GCDeviceVertexBuffer::SystemToVideo()
{
	BYTE* pTempData = NULL;
	m_pBuffer->Lock( 0, 0, (void**)&pTempData, 0 );
	memcpy( pTempData, m_pData, m_uiLength );
	m_pBuffer->Unlock();
}

void GCDeviceVertexBuffer::OnResetDevice()
{
	SAFE_RELEASE( m_pBuffer );
	g_pd3dDevice->CreateVertexBuffer( m_uiLength, m_dwUsage, m_dwFVF, m_Pool, &m_pBuffer, NULL );
	SystemToVideo();
}

void GCDeviceVertexBuffer::OnLostDevice()
{
	SAFE_RELEASE( m_pBuffer );
}

bool GCDeviceVertexBuffer::Load()
{
	return true;
}

void GCDeviceVertexBuffer::UnLoad()
{
	/************************************************************************/
	/*	System Memory & Video Memory Release
	/************************************************************************/
	SAFE_RELEASE( m_pBuffer );
	SAFE_DELETE_ARRAY( m_pData );
}

BYTE* GCDeviceVertexBuffer::Lock( BYTE* ppbData, UINT offsetToLock, UINT sizeToLock, DWORD flag )
{
	return m_pData;
}

void GCDeviceVertexBuffer::Unlock()
{
	/************************************************************************/
	/*	System Memory => Video Memory
	/************************************************************************/
	SystemToVideo();
}

HRESULT GCDeviceVertexBuffer::VideoLock( UINT offsetToLock, UINT sizeToLock, BYTE** ppbData, DWORD flag )
{
	HRESULT hr;
	if( FAILED( hr = m_pBuffer->Lock( offsetToLock, sizeToLock, (void**)ppbData, flag ) ) )
	{
		assert( !"Failed to Lock Vertex Buffer" );
	}
	return hr;
}

HRESULT GCDeviceVertexBuffer::VideoUnlock()
{
	HRESULT hr;
	if( FAILED( hr = m_pBuffer->Unlock() ) )
	{
		assert( !"Failed to Unlock Vertex Buffer" );
	}
	return hr;
}
