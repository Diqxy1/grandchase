// Wave.cpp: implementation of the CWave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Wave.h"
#include "./mapsystem/GCFantasticMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWave::CWave()
{
	m_pWaterBack	= NULL;
	m_pWaterFront	= NULL;

	ZeroMemory( m_FrontWaveVertex, sizeof(SLVERTEX) * 4 );
	ZeroMemory( m_BackWaveVertex, sizeof(SLVERTEX) * 4 );
}

CWave::~CWave()
{
}

void CWave::Init( float height )
{
	height -= 0.55f;
	
	m_pWaterBack = g_pGCDeviceManager2->CreateTexture( "WaveSurface2.dds" );
	m_pWaterFront = g_pGCDeviceManager2->CreateTexture( "WaveSurface1.dds" );

	//0--1
	//2--3
	m_BackWaveVertex[0].Pos.x = -1.0f;
	m_BackWaveVertex[0].Pos.y = height + 1.0f;
	m_BackWaveVertex[0].Pos.z = 0.5f;
	m_BackWaveVertex[0].tu = 0;
	m_BackWaveVertex[0].tv = 0;
	m_BackWaveVertex[0].Diff = 0xffffffff;
	
	m_BackWaveVertex[1].Pos.x = SiKGCFantasticMap()->GetBlockWidth() * 1.5f;
	m_BackWaveVertex[1].Pos.y = height + 1.0f;
	m_BackWaveVertex[1].Pos.z = 0.5f;
	m_BackWaveVertex[1].tu = (float)(SiKGCFantasticMap()->GetBlockWidth() + 1);
	m_BackWaveVertex[1].tv = 0;
	m_BackWaveVertex[1].Diff = 0xffffffff;
	
	m_BackWaveVertex[2].Pos.x = -1.0f;
	m_BackWaveVertex[2].Pos.y = height - 1.0f;
	m_BackWaveVertex[2].Pos.z = 0.5f;
	m_BackWaveVertex[2].tu = 0;
	m_BackWaveVertex[2].tv = 1;
	m_BackWaveVertex[2].Diff = 0xffffffff;
	
	m_BackWaveVertex[3].Pos.x = SiKGCFantasticMap()->GetBlockWidth() * 1.5f;
	m_BackWaveVertex[3].Pos.y = height - 1.0f;
	m_BackWaveVertex[3].Pos.z = 0.5f;
	m_BackWaveVertex[3].tu = (float)(SiKGCFantasticMap()->GetBlockWidth() + 1);
	m_BackWaveVertex[3].tv = 1;
	m_BackWaveVertex[3].Diff = 0xffffffff;


	m_FrontWaveVertex[0].Pos.x = -1.0f;
	m_FrontWaveVertex[0].Pos.y = height + 1.0f;
	m_FrontWaveVertex[0].Pos.z = 0.5f;
	m_FrontWaveVertex[0].tu = 0;
	m_FrontWaveVertex[0].tv = 0;
	m_FrontWaveVertex[0].Diff = 0xffffffff;
	
	m_FrontWaveVertex[1].Pos.x = SiKGCFantasticMap()->GetBlockWidth() * 1.5f;
	m_FrontWaveVertex[1].Pos.y = height + 1.0f;
	m_FrontWaveVertex[1].Pos.z = 0.5f;
	m_FrontWaveVertex[1].tu = (float)(SiKGCFantasticMap()->GetBlockWidth() + 1);
	m_FrontWaveVertex[1].tv = 0;
	m_FrontWaveVertex[1].Diff = 0xffffffff;
	
	m_FrontWaveVertex[2].Pos.x = -1.0f;
	m_FrontWaveVertex[2].Pos.y = height - 1.0f;
	m_FrontWaveVertex[2].Pos.z = 0.5f;
	m_FrontWaveVertex[2].tu = 0;
	m_FrontWaveVertex[2].tv = 1;
	m_FrontWaveVertex[2].Diff = 0xffffffff;
	
	m_FrontWaveVertex[3].Pos.x = SiKGCFantasticMap()->GetBlockWidth() * 1.5f;
	m_FrontWaveVertex[3].Pos.y = height - 1.0f;
	m_FrontWaveVertex[3].Pos.z = 0.5f;
	m_FrontWaveVertex[3].tu = (float)(SiKGCFantasticMap()->GetBlockWidth() + 1);
	m_FrontWaveVertex[3].tv = 1;
	m_FrontWaveVertex[3].Diff = 0xffffffff;
}

void CWave::FrameMove()
{
	for( int i = 0; i < 4; i++ )
	{
		m_BackWaveVertex[i].tu	-= GC_ELAPSED_TIME / 3.0f;
		m_FrontWaveVertex[i].tu	+= GC_ELAPSED_TIME / 3.0f;
	}
}

void CWave::Render()
{
	D3DXMATRIX mat;
	g_pd3dDevice->GetTransform(D3DTS_WORLD,&mat);
	g_pGCDeviceManager2->DrawInWorld(m_pWaterBack,&mat,m_BackWaveVertex);
	g_pGCDeviceManager2->DrawInWorld(m_pWaterFront,&mat,m_BackWaveVertex);
}
