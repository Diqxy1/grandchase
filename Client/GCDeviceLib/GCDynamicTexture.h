#pragma once

#include "GCBaseMesh.h"

class GCDynamicTexture:public GCBaseTexture
{
public:
	GCDynamicTexture(void);
	virtual ~GCDynamicTexture(void);

	bool CreateTexture(LPDIRECT3DDEVICE9 pd3ddevice, DWORD dwWidth, DWORD dwHeight, D3DFORMAT d3dFormat);
	void SetChange(bool bChange) { m_bUpdateTexture = bChange; }
	void ResetDevice();
	void LostDevice();
	void SetTexture(int iStage = 0);

	virtual void Release();
	virtual void SetWithLoad(int stage =0);
public:
	D3DLOCKED_RECT		m_LockedRect;

protected:
	LPDIRECT3DTEXTURE9	m_pTextureSys;

	bool				m_bUpdateTexture;
	D3DFORMAT			m_d3dFormat;
};
