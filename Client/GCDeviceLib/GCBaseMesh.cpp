#include "stdafx.h"
#include ".\gcbasemesh.h"

GCBaseMesh::GCBaseMesh( LPDIRECT3DDEVICE9 d3ddevice )
{
	m_pd3dDevice = d3ddevice;
	m_pIndexBuffer	= NULL;
	m_pVertexBuffer = NULL;

	//   m_pPositionBone	= NULL;
	//m_pAngleBone		= NULL;
	m_dwNumVertex   = 0;
	m_dwNumFace		= 0;
	m_dwNumPositionBone = 0;
	m_dwNumAngleBone = 0;
	SetMeshComputePaletteInterface( &g_DefaultIGCComputePaletteP3M );
}
GCBaseMesh::~GCBaseMesh(void)
{
	ClearPositionBone();
	ClearAngleBon();
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	m_dwNumVertex = 0;
	m_dwNumFace = 0;
}

void GCBaseMesh::Render()
{
	if(m_pVertexBuffer == NULL) return;
	m_pd3dDevice->SetFVF( SKINVERTEX::FVF );
	m_pd3dDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(SKINVERTEX));
	m_pd3dDevice->SetIndices(m_pIndexBuffer);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwNumVertex, 0, m_dwNumFace );
}

GCBaseTexture::GCBaseTexture( LPDIRECT3DDEVICE9 d3ddevice )
{
	m_pd3dDevice = d3ddevice;
	m_pTexture   = NULL;
	m_dwHeight = m_dwWidth = 0;
}

GCBaseTexture::~GCBaseTexture( void )
{
	SAFE_RELEASE(m_pTexture);
	m_dwWidth = 0;
	m_dwHeight = 0;
}

void GCBaseTexture::SetDeviceTexture( int stage /*= 0 */ )
{
	m_pd3dDevice->SetTexture(stage, m_pTexture);
}
