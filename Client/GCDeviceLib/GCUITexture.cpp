#include "stdafx.h"
#include ".\gcuitexture.h"

GCUITexture::GCUITexture(std::string strFileName)
{
	m_pUITexture = g_pGCDeviceManager->CreateTexture(strFileName);

	SScreenRect rt;
	rt.m_rtTexture.left = (int)0;
	rt.m_rtTexture.top = (int)0;
	rt.m_rtTexture.right = (int)0;
	rt.m_rtTexture.bottom = (int)0;
	rt.m_vtScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecScreenRect.push_back(rt);
}

GCUITexture::~GCUITexture(void)
{
	SAFE_RELEASE(m_pUITexture);
	ClearScreenRect();
}

void GCUITexture::Render()
{
	// SpriteRendering을 모든 ScreenRect에 대해서 행한다.
	/*
	g_pGCDeviceManager->GetSprite()->Begin(0);

	for(int i = 0; i < (int)m_vecScreenRect.size(); i++)
	{
	g_pGCDeviceManager->GetSprite()->Draw( m_pUITexture->GetDeviceTexture(), &m_vecScreenRect[i].m_rtTexture, NULL, &m_vecScreenRect[i].m_vtScreenPos, 0xffffffff );
	}

	g_pGCDeviceManager->GetSprite()->End();
	*/
}

void GCUITexture::AddNewScreenRect(float fScreenY, float fScreenX, float fWidth, float fHeight, float fTexTop, float fTexLeft )
{
	SScreenRect rt;
	rt.m_rtTexture.left = (int)fTexLeft;
	rt.m_rtTexture.top = (int)fTexTop;
	rt.m_rtTexture.right = (int)fTexLeft + (int)fWidth;
	rt.m_rtTexture.bottom = (int)fTexTop + (int)fHeight;
	rt.m_vtScreenPos = D3DXVECTOR3(fScreenX, fScreenY, 0.0f);

	m_vecScreenRect.push_back(rt);
}

int GCUITexture::GetWidth()
{
	if(m_pUITexture->IsDisable())
		return 0;
	return m_pUITexture->GetWidth();
}

int GCUITexture::GetHeight()
{
	if(m_pUITexture->IsDisable())
		return 0;
	return m_pUITexture->GetHeight();
}

SScreenRect& GCUITexture::GetScreenRect(int iIndex)
{
	assert( iIndex > -1 && iIndex < (int)m_vecScreenRect.size() );
	return m_vecScreenRect[iIndex];
}
