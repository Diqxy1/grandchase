#pragma once

#include "GCDeviceTexture.h"

//#include <vector>

struct SScreenRect
{
	RECT m_rtTexture;
	D3DXVECTOR3 m_vtScreenPos;
};

class GCUITexture
{
public:
	GCUITexture(std::string strFileName);
	virtual ~GCUITexture(void);

	int GetWidth();
	int GetHeight();
	void Render();
	void ClearScreenRect() { m_vecScreenRect.clear(); }
	void AddNewScreenRect(float fScreenY, float fScreenX, float fWidth, float fHeight, float fTexTop, float fTexLeft );
	SScreenRect& GetScreenRect(int iIndex);

private:
	GCDeviceTexture* m_pUITexture;
	std::vector<SScreenRect>	m_vecScreenRect;
};
