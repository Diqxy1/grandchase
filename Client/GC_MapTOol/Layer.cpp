#include "stdafx.h"
#include "Layer.h"
#include "LayerManager.h"
#include "WOGMapTOol.h"
#include "KGCMap.h"

extern GCDeviceShader*   g_pRenderState;

class LayerSort_ZOrder
{
public:
	bool operator() (const CLayerPart* p, const CLayerPart* q) const
	{
		int ip = p->GetZOrder();
		int iq = q->GetZOrder();
		return ip < iq;
	}
};

CLayer::CLayer(CLayerManager* pManager)
	: KLayer<CLayerPart, CLayerManager>(pManager)
{
}

CLayer::~CLayer(void)
{
}

void CLayer::AddParts(KLuaManager& luaMgr)
{
	std::string     PartName;
	LUA_GET_VALUE_DEF("PARTNAME", PartName, "");
	CLayerPart* TempPart = m_pLayerManager->CreateClonePart(PartName);

	if (TempPart == NULL)
	{
		AddPartsErrFunc(PartName);
		return;
	}

	KLayerRect<int> kTexRect;
	TempPart->GetTexRect(kTexRect);
	LUA_GET_VALUE_DEF("TEX_X", kTexRect.m_X, kTexRect.m_X);
	LUA_GET_VALUE_DEF("TEX_Y", kTexRect.m_Y, kTexRect.m_Y);
	LUA_GET_VALUE_DEF("TEX_WIDTH", kTexRect.m_Width, kTexRect.m_Width);
	LUA_GET_VALUE_DEF("TEX_HEIGHT", kTexRect.m_Height, kTexRect.m_Height);
	TempPart->SetTextureRedirect(kTexRect);

	KLayerRect<float> kLayerRect;
	TempPart->GetLayerRect(kLayerRect);
	LUA_GET_VALUE_DEF("LAYER_X", kLayerRect.m_X, kLayerRect.m_X);
	LUA_GET_VALUE_DEF("LAYER_Y", kLayerRect.m_Y, kLayerRect.m_Y);
	kLayerRect.m_X *= TO_TOOL_COORDINATE_RATE;
	kLayerRect.m_Y *= TO_TOOL_COORDINATE_RATE;

	TempPart->SetLayerPos(kLayerRect);
	TempPart->SetLayer(m_iLayerNum);
	TempPart->SetVertex();

	bool Moveable;
	float  MoveSpeed_X;
	float  MoveSpeed_Y;
	LUA_GET_VALUE_DEF("MOVEABLE", Moveable, false);
	LUA_GET_VALUE_DEF("MOVESPEED_X", MoveSpeed_X, 0);
	LUA_GET_VALUE_DEF("MOVESPEED_Y", MoveSpeed_Y, 0);
	MoveSpeed_X *= TO_TOOL_COORDINATE_RATE;
	MoveSpeed_Y *= TO_TOOL_COORDINATE_RATE;
	TempPart->SetMoveProp(Moveable, MoveSpeed_X, MoveSpeed_Y);

	int   Z_Order;
	LUA_GET_VALUE_DEF("Z_ORDER", Z_Order, 0);
	TempPart->SetZOrder(Z_Order);

	bool PositionReset;
	LUA_GET_VALUE_DEF("POSITION_RESET_X", PositionReset, false);
	TempPart->SetRepeat_X(PositionReset);
	LUA_GET_VALUE_DEF("POSITION_RESET_Y", PositionReset, false);
	TempPart->SetRepeat_Y(PositionReset);

	//std::string     EffectName;
	//int             Effect_X;
	//int             Effect_Y;
	//LUA_GET_VALUE_DEF( "PART_EFFECT", EffectName, "" );
	//LUA_GET_VALUE_DEF( "EFFECT_X", Effect_X, 0 );
	//LUA_GET_VALUE_DEF( "EFFECT_Y", Effect_Y, 0 );
	//TempPart->CreateChildEffect( EffectName, Effect_X, Effect_Y );

	std::string     UniqueName;
	LUA_GET_VALUE_DEF("OBJECT_NAME", UniqueName, TempPart->GetPartName());
	TempPart->SetObjectName(UniqueName);

	DWORD           dwUserValue;
	LUA_GET_VALUE_DEF("USER_VALUE", dwUserValue, TempPart->GetUserValue());
	TempPart->SetUserValue(dwUserValue);

	std::string userString;
	LUA_GET_VALUE_DEF("USER_STRING", userString, TempPart->GetUserString());
	TempPart->SetUserString(userString);

	bool bShow;
	LUA_GET_VALUE_DEF("SHOW", bShow, true);
	TempPart->SetShow(bShow);

	m_vecPart.push_back(TempPart);

	std::vector< CRenderPartPair<CLayerPart> >::iterator vit = std::find_if(m_vecRenderPart.begin(), m_vecRenderPart.end(),
		KFindFileName<CLayerPart>(TempPart->GetFileName()));

	if (vit == m_vecRenderPart.end())
	{
		CRenderPartPair<CLayerPart> partPair;
		partPair.first = TempPart->GetFileName();
		partPair.push_back(TempPart);
		m_vecRenderPart.push_back(partPair);
	}
	else
	{
		CRenderPartPair<CLayerPart>& renderPartPair = (*vit);
		renderPartPair.push_back(TempPart);
	}
}

void CLayer::AddPartsErrFunc(std::string partName_)
{
	std::string str = "( ";
	str += partName_;
	str += " )는 없는 파트입니다. 삭제해주세요.";
	AfxMessageBox(str.c_str());
}

void CLayer::Process()
{
	if (m_vecPart.empty())
		return;

	float PlayingWidth = static_cast<float>(m_pLayerManager->GetMapPtr()->GetPixelWidth());
	float PlayingHeight = static_cast<float>(m_pLayerManager->GetMapPtr()->GetPixelHeight());

	float cwx = g_AppFormView->GetRenderView()->GetEyeX() - WOG_WIDTH * 0.5f;
	float cwy = g_AppFormView->GetRenderView()->GetEyeY() - WOG_HEIGHT * 0.5f;

	float ScrollX = 0.0f;
	float ScrollY = 0.0f;
	ScrollX = (((float)m_fLayerSizeX - WOG_WIDTH) / (PlayingWidth - WOG_WIDTH)) * (cwx);
	ScrollY = (((float)m_fLayerSizeY - WOG_HEIGHT) / (PlayingHeight - WOG_HEIGHT)) * (cwy);

	if (m_fLayerSizeX == 800)
		ScrollX = 0;
	if (m_fLayerSizeY == 600)
		ScrollY = 0;

	for (int i = 0; i < (int)m_vecPart.size(); ++i)
	{
		int bScrollX = m_vecPart[i]->GetScroll_X() ? 1.0f : 0.0f;
		int bScrollY = m_vecPart[i]->GetScroll_Y() ? 1.0f : 0.0f;

		if (m_vecPart[i]->GetLayer() == LAST_LAYER - 1)
		{
			m_vecPart[i]->SetScrollPos(-ScrollX * 0.1f * bScrollX + cwx, -ScrollY * 0.3f * bScrollY + cwy);
		}
		else if (m_vecPart[i]->GetLayer() == LAST_LAYER - 2)
		{
			m_vecPart[i]->SetScrollPos(-ScrollX * 0.3f * bScrollX + cwx, -ScrollY * 0.45f * bScrollY + cwy);
		}
		else if (m_vecPart[i]->GetLayer() == LAST_LAYER - 3)
		{
			m_vecPart[i]->SetScrollPos(-ScrollX * 0.65f * bScrollX + cwx, -ScrollY * 0.8f * bScrollY + cwy);
		}
		else
		{
			m_vecPart[i]->SetScrollPos(-ScrollX + cwx, -ScrollY + cwy);
		}

		m_vecPart[i]->ProcessRepeat(m_fLayerSizeX, m_fLayerSizeY);
	}
}

void CLayer::FrameMove(float fElapsedTime)
{
	if (m_vecPart.empty())
		return;

	Process();
	SetZoffset();
	for (int i = 0; i < (int)m_vecPart.size(); ++i)
	{
		m_vecPart[i]->FrameMove(fElapsedTime);
	}
	BuildRenderList();
	SetBufferType1();
}

//HRESULT CLayer::Render()
//{
//    KLayer<CLayerPart, CLayerManager>::Render();
//
//    for(int i = 0; i < (int)m_vecPart.size(); ++i )
//    {
//        m_vecPart[i]->RenderEffect();
//    }
//    return S_OK;
//}

void CLayer::EraseLayerPart(CLayerPart* pPart)
{
	for (int i = 0; i < (int)m_vecPart.size(); )
	{
		if (m_vecPart[i]->GetPartName() == pPart->GetPartName())
		{
			SAFE_DELETE(m_vecPart[i]);
			m_vecPart.erase(m_vecPart.begin() + i);
		}
		else
			++i;
	}
}

void CLayer::AddLayerPart(CLayerPart* pPart)
{
	m_vecPart.push_back(pPart);
}

void CLayer::AddLayerPartIndex(CLayerPart* pPart, int iIndex)
{
	std::vector< CLayerPart* >::iterator vit;
	for (vit = m_vecPart.begin(); vit != m_vecPart.end(); ++vit)
	{
		if ((*vit)->GetZOrder() == iIndex)
		{
			m_vecPart.insert(vit, pPart);
			break;
		}
	}

	if (vit == m_vecPart.end())
		m_vecPart.push_back(pPart);
	ResetZOrder();
}

void CLayer::SortByZOrder()
{
	std::sort(m_vecPart.begin(), m_vecPart.end(), LayerSort_ZOrder());
}

void CLayer::EraseLayerObject(CLayerPart* pPart)
{
	for (int i = 0; i < (int)m_vecPart.size(); )
	{
		if (m_vecPart[i] == pPart)
		{
			// [8/20/2007 breadceo] 실체는 가지고 있는다
			m_vecPart.erase(m_vecPart.begin() + i);
		}
		else
			++i;
	}
}

void CLayer::ResetZOrder(int DeletedZOrder/* = -1*/)
{
	for (int i = 0; i < (int)m_vecPart.size(); ++i)
	{
		if (DeletedZOrder == -1)
		{
			m_vecPart[i]->SetZOrder(i);
		}
		else
		{
			int ZOrder = m_vecPart[i]->GetZOrder();
			if (ZOrder > DeletedZOrder)
			{
				ZOrder--;
				m_vecPart[i]->SetZOrder(ZOrder);
			}
		}
	}
}

std::string CLayer::GetLayerHeader()
{
	std::stringstream str;
	str << "\tLAYER" << m_iLayerNum << " = {\n";

	return str.str();
}

void CLayer::FindPartsByName(std::string strName, std::vector< CLayerPart* >& vecParts, bool bSame)
{
	std::string strPartsName;

	strupr((char*)strName.c_str());

	for (int i = 0; i < (int)m_vecPart.size(); i++)
	{
		strPartsName = m_vecPart[i]->GetObjectName();
		strupr((char*)strPartsName.c_str());

		if (bSame)
		{
			if (strPartsName == strName)
			{
				vecParts.push_back(m_vecPart[i]);
			}
		}
		else
		{
			if (strncmp(strPartsName.c_str(), strName.c_str(), strName.size()) == 0)
			{
				vecParts.push_back(m_vecPart[i]);
			}
		}
	}
}

std::string CLayer::GetLayerSizeData()
{
	std::stringstream str;
	str << "\t{ " << m_iLayerNum
		<< ", " << m_fLayerSizeX * TO_GC_COORDINATE_RATE
		<< ", " << m_fLayerSizeY * TO_GC_COORDINATE_RATE << ", "
		<< "},\n";

	return str.str();
}

void CLayer::FindLayerPartsByPoint(D3DVECTOR pt, std::vector< CLayerPart* >& vecOut)
{
	for (int i = 0; i < (int)m_vecPart.size(); ++i)
	{
		if (m_vecPart[i]->IsMouseIn(pt))
			vecOut.push_back(m_vecPart[i]);
	}
}

bool CLayer::GetLayerLocationByPtr(CLayerPart* pPart, int& iIndex)
{
	iIndex = -1;

	for (int i = 0; i < (int)m_vecPart.size(); ++i)
	{
		if (m_vecPart[i] == pPart)
		{
			iIndex = i;
			return true;
		}
	}

	return false;
}

// 카테고리 사용 횟수 업데이트
void CLayer::UpdateCategoryUse(std::map< std::string, int >& strCategoryUse)
{
	std::map< std::string, int >::iterator mit;

	for (int i = 0; i < (int)m_vecPart.size(); ++i)
	{
		mit = strCategoryUse.find(m_vecPart[i]->GetPartCategory());

		if (mit == strCategoryUse.end())
			strCategoryUse[m_vecPart[i]->GetPartCategory()] = 1;
		else
			mit->second++;
	}
}

void CLayer::BuildRenderList()
{
	m_vecRenderPart.clear();

	std::vector< CLayerPart* >::iterator vit;

	for (vit = m_vecPart.begin(); vit != m_vecPart.end(); ++vit)
	{
		if ((*vit)->GetShow() == false)
			continue;

		std::vector< CRenderPartPair<CLayerPart> >::iterator vitList = std::find_if(m_vecRenderPart.begin(), m_vecRenderPart.end(),
			KFindFileName<CLayerPart>((*vit)->GetFileName()));

		if (vitList == m_vecRenderPart.end())
		{
			CRenderPartPair<CLayerPart> partPair;
			partPair.first = (*vit)->GetFileName();
			partPair.push_back((*vit));
			m_vecRenderPart.push_back(partPair);
		}
		else
		{
			CRenderPartPair<CLayerPart>& renderPartPair = (*vitList);
			renderPartPair.push_back((*vit));
		}
	}
}

void CLayer::RenderFH()
{
	std::for_each(m_vecPart.begin(), m_vecPart.end(),
		boost::bind(&CLayerPart::RenderFH, _1));
}

void CLayer::InitEyeBlockInfo(CFHBlockInfo* pBlockInfo, CRenderEyeInfo* pRenderEyeInfo)
{
	std::for_each(m_vecPart.begin(), m_vecPart.end(),
		boost::bind(&CLayerPart::InitEyeBlockInfo, _1, pBlockInfo, pRenderEyeInfo));
}

void CLayer::RenderHeadFH()
{
	std::for_each(m_vecPart.begin(), m_vecPart.end(),
		boost::bind(&CLayerPart::RenderHeadFH, _1));
}

void CLayer::CreateBuffer()
{
	char szTemp[256] = { 0, };

	sprintf(szTemp, "KGCLayer_m_pVBPartsRender%d", m_iLayerNum);
	m_pVBPartsRender = g_pGCDeviceManager->CreateVertexBuffer(szTemp, MAX_PART_VERTEX * sizeof(SLVERTEX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, SLVERTEX::FVF, D3DPOOL_DEFAULT, NULL);
	sprintf(szTemp, "KGCLayer_m_pIBPartsRender%d", m_iLayerNum);
	m_pIBPartsRender = g_pGCDeviceManager->CreateIndexBuffer(szTemp, MAX_PART_INDEX * sizeof(WORD),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, NULL);
}