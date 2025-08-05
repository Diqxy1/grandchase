#pragma once

#include "IGCComputePaletteP3M.h"
#include "../GCUTIL/KJohnGen.h"
#include "IGCBone.h"

class GCDeviceMotionFRM;
struct SFrame;

struct SKINVERTEX_SOURCE
{
	D3DXVECTOR3 Pos;
	float		w;
	DWORD		index; 	// matrix palette index, w1의 matrix palette index는 최하위바이트, w2는 그 다음 상위바이트
	D3DXVECTOR3 Nor;
	FLOAT       tu, tv;
};

struct SKINVERTEX
{
	D3DXVECTOR3 Pos;
	float		w[1];
	union
	{
		DWORD		index; 	// matrix palette index, w1의 matrix palette index는 최하위바이트, w2는 그 다음 상위바이트
		BYTE		indexByte[4];
	};
	D3DXVECTOR3 Nor;

	//DWORD       dwDiffuse;
	FLOAT       tu, tv;
	enum _FVF
	{
		FVF = (D3DFVF_XYZB2|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|/*D3DFVF_DIFFUSE|*/D3DFVF_TEX1),
	};
};

class GCBaseMesh: public IGCBone
{
public:
	GCBaseMesh(LPDIRECT3DDEVICE9 d3ddevice);
	virtual ~GCBaseMesh(void);

	virtual void Render();

	DWORD GetNumVertex() { return m_dwNumVertex; }
	DWORD GetNumFace()   { return m_dwNumFace; }

	void SetMeshComputePaletteInterface( IGCComputePaletteP3M* IMeshComputePalette )
	{
		if( IMeshComputePalette == NULL )
			m_pIMeshComputePalette = &g_DefaultIGCComputePaletteP3M;
		else
			m_pIMeshComputePalette = IMeshComputePalette;
	}
	virtual void Release()=0;
protected:

	LPDIRECT3DDEVICE9	m_pd3dDevice;
	IGCComputePaletteP3M* m_pIMeshComputePalette;

	//===========================================================================
	//Bone, Vertex, Index

	LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9		m_pIndexBuffer;
	DWORD						m_dwNumVertex, m_dwNumFace;	//파일에는 WORD형으로 저장되어 있음
};

class GCBaseTexture
{
public:
	GCBaseTexture(LPDIRECT3DDEVICE9 d3ddevice);
	virtual ~GCBaseTexture(void);

public:

	virtual void Release()=0;
	virtual void SetDeviceTexture( int stage = 0 );
	LPDIRECT3DTEXTURE9 GetDeviceTexture() { return m_pTexture; }

	DWORD GetWidth() { return m_dwWidth; }
	DWORD GetHeight() { return m_dwHeight; }
	void  SetWidth(DWORD dwWidth){ m_dwWidth = dwWidth; }
	void  SetHeight(DWORD dwHeight){ m_dwHeight = dwHeight; }

	virtual void SetWithLoad(int stage =0)=0;

protected:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPDIRECT3DTEXTURE9	m_pTexture;
	DWORD				m_dwWidth, m_dwHeight;
};
