#include "stdafx.h"
#include ".\layerpart.h"
//#include "../ParticleLib/GCEffectManager.h"
//#include "../GCDeviceLib/GCDeviceTexture.h"
//#include "../GCDeviceLib/GCDeviceManager.h"
//#include "WOGMapTOol.h"
#include "LayerManager.h"
#include <dbg/dbg.hpp>
#include "KCreateLayerPartDlg.h"

#define PIXELTOTEXEL_X(p) ( (float)((p) - 0.5f ) / (float) m_pTexture->GetWidth() )
#define PIXELTOTEXEL_Y(p) ( (float)((p) - 0.5f ) / (float) m_pTexture->GetHeight() )

CLayerPart::CLayerPart(void)
	: KLayerPart<CLayerPart>()
	, CFHBlockInfo()
	, m_bOutLineObject(false)
	, m_sFHWidth(0)
	, m_sFHHeight(0)
{
	vStart = D3DXVECTOR2(-1.0f, -1.0f);
	vEnd = D3DXVECTOR2(-1.0f, -1.0f);
	m_vecToolObject.clear();
	m_vecParticle.clear();
}

CLayerPart::~CLayerPart(void)
{
	m_vecToolObject.clear();
	m_vecParticle.clear();
}

void CLayerPart::Flush()
{
	if (m_bShow == false)
		return;

	SetVertex();

	if (m_pTexture != NULL)
	{
		for (int i = 0; i < 4; i++)
		{
			ScrolledVertex[i] = m_Vertex[i];
			ScrolledVertex[i].Pos.x += m_fScrollX;
			ScrolledVertex[i].Pos.y += m_fScrollY;

			if (m_bPlayerIsBack)
				ScrolledVertex[i].Diff = (ScrolledVertex[i].Diff & 0x00ffffff) | (0x80000000);
			if (m_bOutLineObject)
				ScrolledVertex[i].Diff = (ScrolledVertex[i].Diff & 0x00ffffff) | (0x20000000);
		}
	}
}

void CLayerPart::Render()
{
	if (m_bShow == false)
		return;

	Flush();

	if (m_pTexture != NULL)
	{
		m_pTexture->SetDeviceTexture();
		g_pd3dDevice->SetFVF(SLVERTEX::FVF);
		g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, ScrolledVertex, sizeof(SLVERTEX));
	}

	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::Render, _1));

	if ((vStart.x != vEnd.x) || (vStart.y != vEnd.y))
	{
		AdjustVertex(vStart.x, vStart.y, vEnd.x, vEnd.y);
		DrawRectangle(vStart.x, vStart.y, vEnd.x, vEnd.y);
	}
}

class KFuncFindType : public std::unary_function< CToolObject, void >
{
public:
	KFuncFindType(DWORD dwType) : m_dwType(dwType) {}
	virtual ~KFuncFindType() {}

	void operator()(CToolObject& _Left)
	{
		if (_Left.GetType() == m_dwType)
			_Left.Render();
	}

	DWORD m_dwType;
};

void CLayerPart::RenderFH()
{
	return;

	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		KFuncFindType(CFootHold::FHT_INIT));
}

//void CLayerPart::RenderEffect()
//{
//    if( m_pEffect != NULL )
//    {
//        g_pRenderState->EndPass();
//        SiKGCEffectManager()->Render( m_pEffect );
//        g_pRenderState->BeginPass(EGCRENDER_STATE_FAR_MAP_LINEAR);
//    }
//}

CLayerPart* CLayerPart::Clone()
{
	CLayerPart* pTemp = new CLayerPart();
	pTemp->CopyMember(this);
	return pTemp;
}

void CLayerPart::CopyMember(CLayerPart* pSrc)
{
	m_strPartName = pSrc->m_strPartName;
	m_strFileName = pSrc->m_strFileName;
	m_pTexture = pSrc->m_pTexture;
	if (m_pTexture)
		m_pTexture->AddRefCount();
	m_iTexX = pSrc->m_iTexX;
	m_iTexY = pSrc->m_iTexY;
	m_iTexWidth = pSrc->m_iTexWidth;
	m_iTexHeight = pSrc->m_iTexHeight;

	for (int i = 0; i < 4; i++)
		m_Vertex[i] = pSrc->m_Vertex[i];

	m_fLayerX = pSrc->m_fLayerX;
	m_fLayerY = pSrc->m_fLayerY;
	m_fWidth = pSrc->m_fWidth;
	m_fHeight = pSrc->m_fHeight;
	m_bMoveable = pSrc->m_bMoveable;
	m_fSpeedX = pSrc->m_fSpeedX;
	m_fSpeedY = pSrc->m_fSpeedY;
	m_iZOrder = pSrc->m_iZOrder;
	m_bPositionReset_X = pSrc->m_bPositionReset_X;
	m_bPositionReset_Y = pSrc->m_bPositionReset_Y;
	m_fScrollX = pSrc->m_fScrollX;
	m_fScrollY = pSrc->m_fScrollY;

	// Clone 한 Effect는 Effect가 있으면 따로 생성하므로 항상 NULL
	//m_strEffectName = "";
	//m_pEffect = NULL;
	//m_iXOffset = pSrc->m_iXOffset;
	//m_iYOffset = pSrc->m_iYOffset;

	m_strCategory = pSrc->m_strCategory;
	m_strUniqueName = pSrc->m_strUniqueName;

	m_iUserValue = pSrc->m_iUserValue;
	m_strUserString = pSrc->m_strUserString;

	m_sFHWidth = pSrc->m_sFHWidth;
	m_sFHHeight = pSrc->m_sFHHeight;
	m_vecToolObject = pSrc->m_vecToolObject;
	m_vecParticle = pSrc->m_vecParticle;

	vStart = pSrc->vStart;
	vEnd = pSrc->vEnd;

	SetTexel();
}

class CFunctorToString : std::unary_function<CToolObject, void>
{
public:
	CFunctorToString(std::stringstream& stm_) : stmOut(stm_) {}
	void operator()(CToolObject& _Left)
	{
		if (_Left.IsIndexValid() && _Left.GetType() == CFootHold::FHT_HEAD)
			stmOut << "\t" << _Left.ToString();
	}

	std::stringstream& stmOut;
};

std::string CLayerPart::ToString()
{
	std::stringstream toStr;
	toStr << "LayerManager:AddLayerParts{\n" << "\tCATEGORY = \"" << m_strCategory << "\",\n"
		<< "\tPARTNAME = \"" << m_strPartName << "\",\n" << "\tTEX_NAME = \"" << m_strFileName << "\",\n"
		<< "\tTEX_X = " << m_iTexX << ",\n" << "\tTEX_Y = " << m_iTexY << ",\n"
		<< "\tTEX_WIDTH = " << m_iTexWidth << ",\n" << "\tTEX_HEIGHT = " << m_iTexHeight << ", \n"
		<< "\tPARTS_WIDTH = " << m_fWidth * TO_GC_COORDINATE_RATE << ",\n" << "\tPARTS_HEIGHT = " << m_fHeight * TO_GC_COORDINATE_RATE << ",\n";

	if (m_vecParticle.size() != 0)
	{
		toStr << "\tPARTICLE = {\n";
		for (std::vector< KLayerParticleData >::iterator it = m_vecParticle.begin(); m_vecParticle.end() != it; ++it)
		{
			toStr << "\t\t{" << std::endl;
			toStr << "\t\t PARTICLE_NAME = \"" << it->strParticleName << "\"," << std::endl;
			toStr << "\t\t OFFSET_X = " << it->fOffsetX << "," << std::endl;
			toStr << "\t\t OFFSET_Y = " << it->fOffsetY << "," << std::endl;
			toStr << "\t\t}," << std::endl;
		}
		toStr << "\t}," << std::endl;;
	}

	if (GetHeadFootHoldSize() != 0)
	{
		toStr << "\tFOOTHOLD_WIDTH = " << static_cast<int>(m_sFHWidth) << ",\n";
		toStr << "\tFOOTHOLD_HEIGHT = " << static_cast<int>(m_sFHHeight) << ",\n";
		toStr << "\tFOOTHOLD_DATA = \n" << "\t{\n";

		std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
			CFunctorToString(toStr));

		toStr << "\t},\n";
	}
	toStr << "}\n";
	return toStr.str();
}

void CLayerPart::FrameMove(float fElapsedTime)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_bMoveable)
		{
			m_Vertex[i].Pos.x += m_fSpeedX;
			m_Vertex[i].Pos.y += m_fSpeedY;
		}
	}

	m_fLayerX = m_Vertex[0].Pos.x;
	m_fLayerY = m_Vertex[2].Pos.y;

	//if( m_bMoveable )
	//{
	//    if( m_pEffect != NULL )
	//        m_pEffect->SetPosition( m_Vertex[2].m_vPos.x, m_Vertex[2].m_vPos.y, true );
	//}

	if (m_bShow == false)
		return;

	Flush();
}

#define ENDMARK "\","
std::string CLayerPart::ToObjectString()
{
	std::stringstream str;

	str << "\t\t{";
	if (m_strUniqueName != m_strPartName)
	{
		str << " OBJECT_NAME = \"" << m_strUniqueName << ENDMARK;
		if (m_iUserValue != -1)
			str << " USER_VALUE = " << m_iUserValue << ",";
		if (m_strUserString != "")
			str << " USER_STRING = \"" << m_strUserString << ENDMARK;
	}

	str << " PARTNAME = \"" << m_strPartName << ENDMARK;
	CLayerPart* pPart = SiCLayerManager()->GetOriginalPart(m_strPartName);
	str << " LAYER_X = " << m_fLayerX * TO_GC_COORDINATE_RATE << ",";
	str << " LAYER_Y = " << m_fLayerY * TO_GC_COORDINATE_RATE << ",";

	if (m_bMoveable == true)
	{
		str << " MOVEABLE = " << m_bMoveable << ",";
		str << " MOVESPEED_X = " << m_fSpeedX * TO_GC_COORDINATE_RATE << ",";
		str << " MOVESPEED_Y = " << m_fSpeedY * TO_GC_COORDINATE_RATE << ",";
		str << " POSITION_RESET_X = " << m_bPositionReset_X << ",";
		str << " POSITION_RESET_Y = " << m_bPositionReset_Y << ",";
	}

	str << " SCROLL_X = " << GetScroll_X() << ",";
	str << " SCROLL_Y = " << GetScroll_Y() << ",";

	str << " Z_ORDER = " << m_iZOrder << ",";

	//if( m_bUseEffect )
	//{
	//    str << " PART_EFFECT = \"" << m_strEffectName << ENDMARK;
	//    str << " EFFECT_X = " << m_iXOffset << ",";
	//    str << " EFFECT_Y = " << m_iYOffset << ",";
	//}

	if (m_bShow == false)
		str << " SHOW = " << m_bShow << ",";

	str << " },\n";

	return str.str();
}

void CLayerPart::InitEyeBlockInfo(CFHBlockInfo* pBlockInfo, CRenderEyeInfo* pRenderEyeInfo)
{
	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::SetBlockInfo, _1, pBlockInfo));
	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::SetEyeInfo, _1, pRenderEyeInfo));
}

void CLayerPart::InitEyeBlockInfo(CRenderEyeInfo* pRenderEyeInfo)
{
	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::SetBlockInfo, _1, static_cast<CFHBlockInfo*>(this)));
	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::SetEyeInfo, _1, pRenderEyeInfo));
}

void CLayerPart::AddToolObject(int i_, int j_, DWORD dwProp, std::string strValue)
{
	JIF(j_ * m_sFHWidth + i_ < static_cast<int>(m_vecToolObject.size()));

	CToolObject& ToolObject = m_vecToolObject[j_ * m_sFHWidth + i_];
	ToolObject.SetEyeInfo(static_cast<CRenderEyeInfo*>(g_LayerToolDlg));
	ToolObject.SetBlockInfo(static_cast<CFHBlockInfo*>(this));
	ToolObject.SetPos(i_, j_, CFootHold::FHT_HEAD);
	ToolObject.SetProp(dwProp);
	ToolObject.SetValue(strValue);
}

void CLayerPart::InitTOVector()
{
	m_vecToolObject.clear();
	m_vecToolObject.resize(m_sFHHeight * m_sFHWidth);

	for (int i = 0; i < m_sFHHeight; ++i)
	{
		for (int j = 0; j < m_sFHWidth; ++j)
		{
			m_vecToolObject[i*m_sFHWidth + j].SetEyeInfo(static_cast<CRenderEyeInfo*>(g_LayerToolDlg));
			m_vecToolObject[i*m_sFHWidth + j].SetBlockInfo(static_cast<CFHBlockInfo*>(this));
			m_vecToolObject[i*m_sFHWidth + j].SetPos(j, i, CFootHold::FHT_INIT);
		}
	}
}

class CFunctorCount : std::unary_function<CToolObject, void>
{
public:
	void operator()(CToolObject& _Left)
	{
		if (_Left.GetType() == CFootHold::FHT_HEAD)
			s_iCount++;
	}

	static int s_iCount;
};

int CFunctorCount::s_iCount = 0;

int CLayerPart::GetHeadFootHoldSize()
{
	CFunctorCount::s_iCount = 0;

	return std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		CFunctorCount()).s_iCount;
}

void CLayerPart::MoveDown(int iTerm/* = 1*/)
{
	float term = FOOTHOLDSIZE;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		term = 1.0f;

	m_fLayerY -= iTerm * term;

	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::MoveDown, _1, true, term == 1.0f));
}

void CLayerPart::MoveUp(int iTerm/* = 1*/)
{
	float term = FOOTHOLDSIZE;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		term = 1.0f;

	m_fLayerY += iTerm * term;

	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::MoveUp, _1, true, term == 1.0f));
}

void CLayerPart::MoveLeft(int iTerm/* = 1*/)
{
	float term = FOOTHOLDSIZE;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		term = 1.0f;

	m_fLayerX -= iTerm * term;

	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::MoveLeft, _1, true, term == 1.0f));
}

void CLayerPart::MoveRight(int iTerm/* = 1*/)
{
	float term = FOOTHOLDSIZE;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		term = 1.0f;

	m_fLayerX += iTerm * term;

	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::MoveRight, _1, true, term == 1.0f));
}

void CLayerPart::SetLayerPos(const KLayerRect<float>& kRect)
{
	int iMoveTermX = static_cast<int>((kRect.m_X - m_fLayerX) / FOOTHOLDSIZE);
	int iMoveTermY = static_cast<int>((kRect.m_Y - m_fLayerY) / FOOTHOLDSIZE);
	int iMoveTermX_2 = static_cast<int>(static_cast<int>(kRect.m_X - m_fLayerX) % FOOTHOLDSIZE_INT);
	int iMoveTermY_2 = static_cast<int>(static_cast<int>(kRect.m_Y - m_fLayerY) % FOOTHOLDSIZE_INT);

	KLayerPart<CLayerPart>::SetLayerPos(kRect);

	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		boost::bind(&CToolObject::MoveGap, _1, iMoveTermX, iMoveTermY, iMoveTermX_2, iMoveTermY_2));
}

void CLayerPart::RenderHeadFH()
{
	std::for_each(m_vecToolObject.begin(), m_vecToolObject.end(),
		KFuncFindType(CFootHold::FHT_HEAD));
}

void CLayerPart::DrawRectangle(float tx, float ty, float bx, float by)
{
	TRIANGLEVERTEX RectVertex[8];

	RectVertex[0].x = RectVertex[2].x = RectVertex[4].x = RectVertex[5].x = tx;
	RectVertex[0].y = RectVertex[1].y = RectVertex[4].y = RectVertex[6].y = ty;

	RectVertex[1].x = RectVertex[3].x = RectVertex[6].x = RectVertex[7].x = bx;
	RectVertex[2].y = RectVertex[3].y = RectVertex[5].y = RectVertex[7].y = by;

	for (int i = 0; i < 8; i++)
	{
		RectVertex[i].z = 200.0f;
		RectVertex[i].color = D3DCOLOR_ARGB(255, 255, 0, 0);
	}

	g_pd3dDevice->SetTexture(0, NULL);
	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pd3dDevice->SetFVF(FVF_TRIANGLEVERTEX);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, RectVertex, sizeof(TRIANGLEVERTEX));
	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CLayerPart::AdjustVertex(float &tx, float &ty, float &bx, float &by)
{
	if (tx < 0)
		tx = 0;
	else if (tx > m_fLayerX + m_fWidth)
		tx = m_fLayerX + m_fWidth;
	if (ty < 0)
		ty = 0;
	else if (ty > m_fLayerY + m_fHeight)
		ty = m_fLayerY + m_fHeight;
	if (by > m_fLayerY + m_fHeight)
		by = m_fLayerY + m_fHeight;
	else if (by < 0)
		by = 0;
	if (bx > m_fLayerX + m_fWidth)
		bx = m_fLayerX + m_fWidth;
	else if (bx < 0)
		bx = 0;
}

void CLayerPart::SaveAndResetVertex(void)
{
	if ((vStart.x != vEnd.x) || (vStart.y != vEnd.y))
	{
		D3DXVECTOR2 temp = vEnd - vStart;
		float tempRatioX = m_fWidth / GetTexture()->GetWidth();
		float tempRatioY = m_fHeight / GetTexture()->GetHeight();

		// 텍스쳐 UV
		m_iTexX = vStart.x * GetTexture()->GetWidth() / m_fWidth;
		m_iTexY = GetTexture()->GetHeight() - vStart.y * GetTexture()->GetHeight() / m_fHeight;
		m_iTexWidth = fabs(temp.x * GetTexture()->GetWidth() / m_fWidth);
		m_iTexHeight = fabs(temp.y * GetTexture()->GetHeight() / m_fHeight);

		// 레이어 크기
		m_fWidth = m_iTexWidth * tempRatioX;
		m_fHeight = m_iTexHeight * tempRatioY;
	}

	vStart = vEnd;
}

void CLayerPart::SetRectangle(void)
{
	AdjustVertex(vStart.x, vStart.y, vEnd.x, vEnd.y);

	D3DLOCKED_RECT	d3drc;
	GetTexture()->GetDeviceTexture()->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);

	vEnd.x *= GetTexture()->GetWidth() / m_fWidth;
	vEnd.y = GetTexture()->GetHeight() - vEnd.y * GetTexture()->GetHeight() / m_fHeight;

	CRect gap(0, 0, 0, 0);
	//ClientToScreen(&gap);
	//m_ctlTexture.ScreenToClient(&gap);

	CPoint startPoint;
	startPoint.x = vEnd.x + gap.left;
	startPoint.y = vEnd.y + gap.top;

	// top
	bool bIsCheck = false;

	int itop = 0;
	int ibottom = 0;
	int ileft = 0;
	int iright = 0;

	bool bEnd = false;
	while (!bEnd)
	{
		bEnd = true;
		for (int i = ileft; i <= iright; ++i)
		{
			if (0x00 < (*((LPDWORD)d3drc.pBits + (i + startPoint.x) + (startPoint.y - 1 + itop) * (d3drc.Pitch / 4)) & 0xff000000) >> 24)
			{
				bIsCheck = true;
				break;
			}
		}

		if (bIsCheck)
		{
			if (startPoint.y - 1 + itop > 0)
			{
				--itop;
				bEnd &= false;
			}
			bIsCheck = false;
		}

		for (int i = ileft; i <= iright; ++i)
		{
			if (0x00 < (*((LPDWORD)d3drc.pBits + (i + startPoint.x) + (ibottom + 1 + startPoint.y) * (d3drc.Pitch / 4)) & 0xff000000) >> 24)
			{
				bIsCheck = true;
				break;
			}
		}

		if (bIsCheck)
		{
			if (ibottom + 1 + startPoint.y < 512)
			{
				++ibottom;
				bEnd &= false;
			}
			bIsCheck = false;
		}

		for (int i = itop; i <= ibottom; ++i)
		{
			if (0x00 < (*((LPDWORD)d3drc.pBits + (startPoint.x - 1 + ileft) + (i + startPoint.y) * (d3drc.Pitch / 4)) & 0xff000000) >> 24)
			{
				bIsCheck = true;
				break;
			}
		}

		if (bIsCheck)
		{
			if (startPoint.x - 1 + ileft > 0)
			{
				--ileft;
				bEnd &= false;
			}
			bIsCheck = false;
		}

		for (int i = itop; i <= ibottom; ++i)
		{
			if (0x00 < (*((LPDWORD)d3drc.pBits + (iright + 1 + startPoint.x) + (i + startPoint.y) * (d3drc.Pitch / 4)) & 0xff000000) >> 24)
			{
				bIsCheck = true;
				break;
			}
		}

		if (bIsCheck)
		{
			if (iright + 1 + startPoint.x < 512)
			{
				++iright;
				bEnd &= false;
			}
			bIsCheck = false;
		}
	}

	ileft *= m_fWidth / GetTexture()->GetWidth();
	itop *= m_fHeight / GetTexture()->GetHeight();
	iright *= m_fWidth / GetTexture()->GetWidth();
	ibottom *= m_fHeight / GetTexture()->GetHeight();

	startPoint.x *= m_fWidth / GetTexture()->GetWidth();
	startPoint.y *= m_fHeight / GetTexture()->GetHeight();

	vStart.x = startPoint.x + ileft - gap.left;
	vStart.y = m_fHeight - (startPoint.y + itop - gap.top);
	vEnd.x = startPoint.x + iright - gap.left + 1;
	vEnd.y = m_fHeight - (startPoint.y + ibottom - gap.top + 1);

	GetTexture()->GetDeviceTexture()->UnlockRect(0);
}