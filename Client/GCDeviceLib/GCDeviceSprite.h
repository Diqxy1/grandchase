#pragma once

class GCDeviceSprite : public GCDevice
{
public:
	GCDeviceSprite(LPDIRECT3DDEVICE9 d3ddevice);
	virtual ~GCDeviceSprite(void);

	void OnResetDevice();
	void OnLostDevice();

	bool Load();
	void UnLoad();

	LPD3DXSPRITE GetSprite() { return m_pSprite; }
private:
	LPDIRECT3DDEVICE9 m_pd3ddevice;
	LPD3DXSPRITE    m_pSprite;
};
