#include "stdafx.h"
#include "GCGuildMark.h"

KGCGuildMark::KGCGuildMark(void)
{
	m_vecTextrue.reserve(SIZE_GUILMARK_POOL);
	m_vecGuildCode.reserve(SIZE_GUILMARK_POOL);
	m_vecTextrue.clear();
	m_vecGuildCode.clear();
	for (int i = 0; i < SIZE_GUILMARK_POOL; i++)
	{
		m_vecTextrue.push_back(NULL);
		m_vecGuildCode.push_back(L"");
	}
}

KGCGuildMark::~KGCGuildMark(void)
{
	for (int i = 0; i < SIZE_GUILMARK_POOL; i++)
	{
		SAFE_DEVICE_RELEASE(m_vecTextrue[i]);
	}

	m_vecTextrue.clear();
	m_vecGuildCode.clear();
}

void KGCGuildMark::DeleteDevice()
{
}

HRESULT KGCGuildMark::InitDevice()
{
	for (int i = 0; i < SIZE_GUILMARK_POOL; i++)
	{
		m_vecTextrue[i] = g_pGCDeviceManager2->CreateTexture("defaultmark.dds"); //UI00.dds는 정체가 머임.
	}

	return S_OK;
}

void KGCGuildMark::Render(int Color, int Back, int Fore, float x, float y, float Size, int iIndex)
{
	static const float  SHIELD_LINE = 0.0f;
	static const float  SHIELD_WIDTH = 0.20117f;
	static const float  SHIELD_HEIGHT = 0.19727f;
	static const float  CREST_LINE = 2.0f * SHIELD_HEIGHT;
	static const float  CREST_WIDTH = 0.16406f;
	static const float  CREST_HEIGHT = 0.16016f;
	static const BYTE   MarkColor[GUILD_MARK_COLOR_NUM][3] =
	{
		231, 231, 231,
		201,  88, 194,
		228,  45,  45,
		228, 118,  45,
		228, 157,  45,
		250, 218,   6,
		162, 198,  36,
		46,  118,  41,
		22,   86, 119,
		83,   51, 141,
		50,   50,  50,
	};

	if (Color < 0 || Color >= GUILD_MARK_COLOR_NUM) return;
	if (Back < 0 || Back >= GUILD_MARK_SHIELD_NUM) return;
	if (Fore < 0 || Fore >= GUILD_MARK_CREST_NUM) return;

	D3DXMATRIX mat1;

	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	int Line = 0;
	//	m_pTexture->SetDeviceTexture();

	while (1) {
		if (Back > 4) {
			Line++;
			Back -= 5;
		}
		else break;
	}
	D3DXMatrixTranslation(&mat1, x, y, .8f);
	g_pGCDeviceManager2->DrawInWorld(m_vecTextrue[iIndex], &mat1,
		Size, Size*.9f, (float)Back / 5.0f, Line*SHIELD_HEIGHT, (float)Back / 5.0f + SHIELD_WIDTH, (Line + 1)*SHIELD_HEIGHT,
		0, 0, 0,
		D3DCOLOR_ARGB(255, MarkColor[Color][0], MarkColor[Color][1], MarkColor[Color][2]));
	//	g_pd3dDevice->SetTransform( D3DTS_WORLD, &mat1);
	//	g_MyD3D->Draw_Billboard(Size, Size*.9f, (float)Back/5.0f, Line*SHIELD_HEIGHT, (float)Back/5.0f+SHIELD_WIDTH, (Line+1)*SHIELD_HEIGHT, 255, MarkColor[Color][0], MarkColor[Color][1], MarkColor[Color][2]);

	Line = 0;
	while (1) {
		if (Fore > 5) {
			Line++;
			Fore -= 6;
		}
		else break;
	}

	D3DXMatrixTranslation(&mat1, x + .05f*Size, y + .03f*Size, .8f);
	g_pGCDeviceManager2->DrawInWorld(m_vecTextrue[iIndex], &mat1,
		Size*.8f, Size*.72f, (float)Fore / 6.0f, CREST_LINE + Line*CREST_HEIGHT, (float)Fore / 6.0f + CREST_WIDTH, CREST_LINE + (Line + 1)*CREST_HEIGHT);

	g_pGCDeviceManager2->PopState();
}