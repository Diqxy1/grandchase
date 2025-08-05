#include "stdafx.h"
#include ".\layermanager.h"
#include "../GCDeviceLib/GCDeviceShader.h"
#include "WOGMapTOol.h"
#include "KCreateLayerPartDlg.h"
//#include <boost/algorithm/string.hpp>
#include <dbg/dbg.hpp>

extern GCDeviceShader*   g_pRenderState;

ImplementSingleton(CLayerManager);

CLayerManager::CLayerManager(void) : KLayerManager<CLayerPart, CLayer, KGCMap>(SiKGCMap())
{
}

CLayerManager::~CLayerManager(void)
{
	clear();
}

void CLayerManager::clear()
{
	KLayerManager<CLayerPart, CLayer, KGCMap>::clear();
	m_vecUsingPartCategoryList.clear();
}

void CLayerManager::UpdateCategoryUse()
{
	std::map< std::string, int > mapCategoryUse;
	mapCategoryUse.clear();

	for (int i = 0; i < m_iMaxLayerNum; ++i)
	{
		m_vecLayer[i]->UpdateCategoryUse(mapCategoryUse);
	}

	m_vecUsingPartCategoryList.clear();
	std::map< std::string, int >::iterator mit;
	for (mit = mapCategoryUse.begin(); mit != mapCategoryUse.end(); ++mit)
	{
		m_vecUsingPartCategoryList.push_back(mit->first);
	}
}

void CLayerManager::RegisterLuabind()
{
	clear();

	lua_State* L = KGCLuabinder_Map::getInstance().GetLuaState();

	lua_tinker::class_add<CLayerManager>(L, "CLayerManager");
	lua_tinker::class_def<CLayerManager>(L, "SetPartsCategoryList", &CLayerManager::SetPartsCategoryList);
	lua_tinker::class_def<CLayerManager>(L, "AddLayerParts", &CLayerManager::AddLayerParts);

	lua_tinker::class_add<CLayer>(L, "KGCLayer");

	lua_tinker::decl(L, "LayerManager", this);

	std::string strPath = g_Path;
	strPath += "/Stage/PartsCategoryList.lua";
	if (KGCLuabinder_Map::getInstance().DoFile(strPath.c_str()) == E_FAIL)
	{
		strPath = g_Path;
		strPath += "/Stage/Fan_Map/PartsCategoryList.lua";
		if (KGCLuabinder_Map::getInstance().DoFile(strPath.c_str()) == E_FAIL)
			ASSERT(!"PartCategoryList Load Failed!" && strPath.c_str());
	}

	for (int i = 0; i < (int)m_vecPartCategoryList.size(); ++i)
	{
		std::string CategoryPath = g_Path;
		CategoryPath += "/Stage/";
		CategoryPath += m_vecPartCategoryList[i];
		if (KGCLuabinder_Map::getInstance().DoFile(CategoryPath.c_str()) == E_FAIL)
		{
			CategoryPath = g_Path;
			CategoryPath += "/Stage/Fan_Map/";
			CategoryPath += m_vecPartCategoryList[i];
			if (KGCLuabinder_Map::getInstance().DoFile(CategoryPath.c_str()) == E_FAIL)
				ASSERT(!"Category Load Failed!" && CategoryPath.c_str());
		}
	}
}

void CLayerManager::SetLayer()
{
	UnLoad();

	for (int i = 0; i < GC_ZORDER_NUM; ++i)
	{
		m_vecLayer.push_back(new CLayer(this));
	}
	for (int i = 0; i < (int)m_vecLayer.size(); i++)
	{
		m_vecLayer[i]->SetLayerNum(i);
	}
}

void CLayerManager::PreRender()
{
	if (m_vecLayer.empty())
		return;

	// No Alpha
	g_pRenderState->BeginPass(EGCRENDER_STATE_FAR_MAP_LINEAR);
	for (int i = (int)m_vecLayer.size() - 1; i >= m_iPlayingLayer + 1; --i)
		//for( int i = m_iPlayingLayer+1; i < (int)m_vecLayer.size(); ++i )
	{
		m_vecLayer[i]->Render();
	}
	//m_vecLayer[19]->Render();
	//for( int i = m_iPlayingLayer+1; i < (int)m_vecLayer.size() - 1; ++i )
	//{
	//    m_vecLayer[i]->Render();
	//}
	g_pRenderState->EndPass();
}

void CLayerManager::PostRender()
{
	if (m_vecLayer.empty())
		return;

	// Alpha
	g_pRenderState->BeginPass(EGCRENDER_STATE_NEAR_MAP_LINEAR);
	for (int i = m_iPlayingLayer; i >= 0; --i)
	{
		m_vecLayer[i]->Render();
	}
	g_pRenderState->EndPass();
}

void CLayerManager::CreateEmptyDefaultLayer(float fWidth, float fHeight)
{
	UnLoad();

	lua_State* L = KGCLuabinder_Map::getInstance().GetLuaState();

	m_iPlayingLayer = 5;
	m_iMaxLayerNum = GC_ZORDER_NUM;

	for (int i = 0; i < GC_ZORDER_NUM; ++i)
		m_vecLayer.push_back(new CLayer(this));

	for (int i = 0; i < (int)m_vecLayer.size(); i++)
	{
		m_vecLayer[i]->SetLayerNum(i);
		m_vecLayer[i]->SetLayerXSize(fWidth);
		m_vecLayer[i]->SetLayerYSize(fHeight);
	}

	std::stringstream stm;
	// 루아 바인딩용
	for (int i = 0; i < (int)m_vecLayer.size(); i++)
	{
		stm.str("");
		stm << "Layer" << i;
		lua_tinker::decl(L, stm.str().c_str(), &m_vecLayer[i]);
	}

	for (int i = 0; i < (int)m_vecLayer.size(); ++i)
	{
		m_vecLayer[i]->CreateBuffer();
	}
}

void CLayerManager::EraseLayerPart(CLayerPart* pPart)
{
	std::map< std::string, CLayerPart* >::iterator iter = m_mapPartPool.find(pPart->GetPartName());
	SAFE_DELETE(iter->second);
	m_mapPartPool.erase(iter);

	if (!m_vecLayer.empty())
	{
		for (int i = 0; i < (int)m_vecLayer.size(); ++i)
		{
			m_vecLayer[i]->EraseLayerPart(pPart);
		}
	}
}

void CLayerManager::SetCategoryList(std::vector< std::string >& CategoryList)
{
	std::vector< std::string >::iterator vit;
	std::vector< std::string >::iterator vitFind;
	for (vit = m_vecPartCategoryList.begin(); vit != m_vecPartCategoryList.end(); ++vit)
	{
		vitFind = std::find(CategoryList.begin(), CategoryList.end(), (*vit));
		if (vitFind == CategoryList.end())
			EraseUnUseCategory((*vit));
	}

	m_vecPartCategoryList = CategoryList;
}

void CLayerManager::EraseUnUseCategory(const std::string strCategoryname)
{
	std::map< std::string, CLayerPart* >::iterator iter;
	for (iter = m_mapPartPool.begin(); iter != m_mapPartPool.end(); )
	{
		if (iter->second->GetPartCategory() == strCategoryname)
		{
			SAFE_DELETE(iter->second);
			iter = m_mapPartPool.erase(iter);
		}
		else ++iter;
	}
}

void CLayerManager::DeleteCategory(std::string CategoryName)
{
	for (int i = 0; i < (int)m_vecPartCategoryList.size(); )
	{
		if (m_vecPartCategoryList[i] == CategoryName)
			m_vecPartCategoryList.erase(m_vecPartCategoryList.begin() + i);
		else ++i;
	}
}

std::string CLayerManager::GetCategoryListString()
{
	std::string toStr;

	toStr += "LayerManager:SetPartsCategoryList{\n";
	for (int i = 0; i < (int)m_vecPartCategoryList.size(); ++i)
	{
		toStr += "\t\"";
		toStr += m_vecPartCategoryList[i];
		toStr += "\",\n";
	}
	toStr += "}";

	return toStr;
}

std::string CLayerManager::GetScriptHeader()
{
	std::string LuaHeader;
	LuaHeader = "D3DBLEND_ZERO = 1\nD3DBLEND_ONE = 2\nD3DBLEND_SRCALPHA = 5\nD3DBLEND_INVSRCALPHA = 6\nD3DBLEND_DESTALPHA = 7\nD3DBLEND_INVDESTALPHA = 8\n";
	return LuaHeader;
}

void CLayerManager::FrameMove(float fElapsedTime)
{
	if (m_vecLayer.empty())
		return;

	for (int i = 0; i < (int)m_vecLayer.size(); ++i)
	{
		m_vecLayer[i]->FrameMove(fElapsedTime);
	}
}

std::string CLayerManager::GetLayerInfoString()
{
	std::stringstream str;

	str << "\tLAYERTOTAL = " << GC_ZORDER_NUM << ",\n" << "\tGAMEPLAYLAYER = " << m_iPlayingLayer << ",\n";
	str << "}\n";

	return str.str();
}

std::string CLayerManager::GetLoadPartsString()
{
	std::stringstream str;
	str << "PARTS_LIST = \n";
	str << "{\n";
	for (int i = 0; i < (int)m_vecUsingPartCategoryList.size(); ++i)
	{
		str << "\t\"" << m_vecUsingPartCategoryList[i] << "\",\n";
	}
	str << "}" << std::endl;;

	return str.str();
}

std::string CLayerManager::GetCreateLayerHeader()
{
	std::stringstream str;
	str << "LAYER_DATA = \n";
	str << "{\n";

	return str.str();
}

std::string CLayerManager::GetLayerSizeHeader()
{
	std::stringstream str;
	str << "LAYER_SIZE_DATA = \n";
	str << "{\n";

	return str.str();
}

void CLayerManager::FindPartsByName(std::string strName, std::vector< CLayerPart* >& vecParts, bool bSame)
{
	for (int i = 0; i < (int)m_vecLayer.size(); i++)
	{
		m_vecLayer[i]->FindPartsByName(strName, vecParts, bSame);
	}
}

void CLayerManager::FindLayerPartsByPoint(D3DVECTOR pt, std::vector< CLayerPart* >& vecOut)
{
	for (int i = 0; i < (int)m_vecLayer.size(); ++i)
	{
		m_vecLayer[i]->FindLayerPartsByPoint(pt, vecOut);
	}
}

void CLayerManager::GetLayerLocationByPtr(CLayerPart* pPart, int& iLayerNum, int& iIndex)
{
	iLayerNum = -1;

	for (int i = 0; i < (int)m_vecLayer.size(); ++i)
	{
		if (m_vecLayer[i]->GetLayerLocationByPtr(pPart, iIndex) == true)
		{
			iLayerNum = i;
			break;
		}
	}
}

void CLayerManager::AddLayerPartsErr(std::string partName_)
{
	std::string str = "( ";
	str += partName_;
	str += " )는 이미 존재합니다";
	AfxMessageBox(str.c_str());
}

bool CLayerManager::LoadFromLua(KLayerPartData& kData)
{
	KLuaManager luaMgr(KGCLuabinder_Map::getInstance().GetLuaState());

	LUA_GET_VALUE_DEF("CATEGORY", kData.PartCategory, "");
	LUA_GET_VALUE_DEF("PARTNAME", kData.PartName, "");
	LUA_GET_VALUE_DEF("TEX_NAME", kData.TexName, "");
	LUA_GET_VALUE_DEF("TEX_X", kData.kTexRect.m_X, 0);
	LUA_GET_VALUE_DEF("TEX_Y", kData.kTexRect.m_Y, 0);
	LUA_GET_VALUE_DEF("TEX_WIDTH", kData.kTexRect.m_Width, 0);
	LUA_GET_VALUE_DEF("TEX_HEIGHT", kData.kTexRect.m_Height, 0);
	LUA_GET_VALUE_DEF("PARTS_WIDTH", kData.kLayerRect.m_Width, static_cast<float>(kData.kTexRect.m_Width));
	LUA_GET_VALUE_DEF("PARTS_HEIGHT", kData.kLayerRect.m_Height, static_cast<float>(kData.kTexRect.m_Height));

	LUA_BEGIN_TABLE("PARTICLE", goto PARTICLE_LOAD_END)
	{
		for (int i = 1; ; ++i)
		{
			LUA_BEGIN_TABLE(i, break)
			{
				KLayerParticleData kParticle;
				LUA_GET_VALUE_DEF("PARTICLE_NAME", kParticle.strParticleName, "");
				LUA_GET_VALUE_DEF("OFFSET_X", kParticle.fOffsetY, 0.0f);
				LUA_GET_VALUE_DEF("OFFSET_Y", kParticle.fOffsetY, 0.0f);
				kData.vecParticle.push_back(kParticle);
			}
			LUA_END_TABLE(break)
		}
	}
	LUA_END_TABLE(; )

		PARTICLE_LOAD_END:

	kData.kLayerRect.m_Width *= TO_TOOL_COORDINATE_RATE;
	kData.kLayerRect.m_Height *= TO_TOOL_COORDINATE_RATE;

	//std::string PartChanged = boost::replace_all_copy( kData.PartCategory, " ", "" );
	//if( kData.PartCategory != PartChanged )
	//{
	//    //START_LOG( clog, dbg::endl << kData.PartCategory << L" Changes " << PartChanged );
	//    kData.PartCategory = PartChanged;
	//}

	//PartChanged = boost::replace_all_copy( kData.PartName, " ", "" );
	//if( kData.PartName != PartChanged )
	//{
	//    //START_LOG( clog, dbg::endl << kData.PartName << L" Changes " << PartChanged );
	//    kData.PartName = PartChanged;
	//}

	std::map< std::string, CLayerPart* >::iterator mit = m_mapPartPool.find(kData.PartName);
	if (mit != m_mapPartPool.end())
	{
		AddLayerPartsErr(kData.PartName);
		return false;
	}

	return true;
}

void CLayerManager::AddLayerParts()
{
	KLayerPartData kData;
	if (LoadFromLua(kData) == false)
		return;

	KLayerFootHoldData kLayerFHData;
	if (LoadFootHoldData(kLayerFHData) == false)
		return;

	CLayerPart* pPart = new CLayerPart();
	pPart->SetPartCategory(kData.PartCategory);
	pPart->SetPartName(kData.PartName);
	pPart->SetTexturePart(kData.TexName, kData.kTexRect);
	pPart->SetLayerPos(kData.kLayerRect);
	pPart->SetParticle(kData.vecParticle);
	pPart->SetFHWidth(kLayerFHData.m_sFHWidth);
	pPart->SetFHHeight(kLayerFHData.m_sFHHeight);
	pPart->SetFootHoldVector(kLayerFHData.m_vecTO);

	m_mapPartPool[pPart->GetPartName()] = pPart;

	if (g_LayerToolDlg != NULL)
		pPart->InitEyeBlockInfo(static_cast<CRenderEyeInfo*>(g_LayerToolDlg));
}

bool CLayerManager::LoadFootHoldData(KLayerFootHoldData& kLayerFHData)
{
	KLuaManager luaMgr(KGCLuabinder_Map::getInstance().GetLuaState());

	LUA_GET_VALUE_DEF("FOOTHOLD_WIDTH", kLayerFHData.m_sFHWidth, 0);
	LUA_GET_VALUE_DEF("FOOTHOLD_HEIGHT", kLayerFHData.m_sFHHeight, 0);

	kLayerFHData.m_vecTO.clear();
	if (kLayerFHData.m_sFHWidth * kLayerFHData.m_sFHHeight == 0)
		return true;

	kLayerFHData.m_vecTO.resize(kLayerFHData.m_sFHWidth * kLayerFHData.m_sFHHeight);
	for (int i = 0; i < kLayerFHData.m_sFHHeight; ++i)
	{
		for (int j = 0; j < kLayerFHData.m_sFHWidth; ++j)
		{
			kLayerFHData.m_vecTO[i*kLayerFHData.m_sFHWidth + j].SetPos(j, i, CFootHold::FHT_INIT);
		}
	}
	LUA_BEGIN_TABLE("FOOTHOLD_DATA", return false);
	{
		for (int i = 1; ; ++i)
		{
			LUA_BEGIN_TABLE(i, break);
			{
				int PosX = 0;
				int PosY = 0;
				DWORD Prop = 0;
				std::string strValue;

				LUA_GET_VALUE_DEF(1, PosX, 0);
				LUA_GET_VALUE_DEF(2, PosY, 0);
				LUA_GET_VALUE_DEF(3, Prop, CFootHold::FHP_JUST_FOOTHOLD);
				LUA_GET_VALUE_DEF(4, strValue, "");

				CToolObject& TObj = kLayerFHData.m_vecTO[kLayerFHData.m_sFHWidth*PosY + PosX];
				TObj.SetPos(PosX, PosY, CFootHold::FHT_HEAD);
				TObj.SetProp(Prop);
				TObj.SetValue(strValue);
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return false);
	return true;
}

void CLayerManager::RenderFH()
{
	if (m_vecLayer.empty())
		return;

	std::for_each(m_vecLayer.begin(), m_vecLayer.end(),
		boost::bind(&CLayer::RenderFH, _1));
}

bool CLayerManager::LoadScript(std::string Script)
{
	bool bRet = true;
	bRet &= KLayerManager<CLayerPart, CLayer, KGCMap>::LoadScript(Script);
	for (int i = 0; i < m_iMaxLayerNum; ++i)
		m_vecLayer[i]->InitEyeBlockInfo(static_cast<CFHBlockInfo*>(SiKGCMap()), static_cast<CRenderEyeInfo*>(g_AppFormView->GetRenderView()));

	return bRet;
}

void CLayerManager::RenderHeadFH()
{
	if (m_vecLayer.empty())
		return;

	std::for_each(m_vecLayer.begin(), m_vecLayer.end(),
		boost::bind(&CLayer::RenderHeadFH, _1));
}

void CLayerManager::LoadParts(KLuaManager& luaMgr)
{
	LUA_BEGIN_TABLE("PARTS_LIST", return)
	{
		for (int i = 1; ; ++i)
		{
			std::string strScriptName;
			LUA_GET_VALUE_NOASSERT(i, strScriptName, break);
			strScriptName = "./Stage/" + strScriptName;
			if (KGCLuabinder_Map::getInstance().DoFile(strScriptName.c_str()) == E_FAIL)
				ASSERT(!"LoadParts DoFile Failed!!");
		}
	}
	LUA_END_TABLE(return);
}

void CLayerManager::SetPartsCategoryList()
{
	KLuaManager luaMgr(KGCLuabinder_Map::getInstance().GetLuaState());

	for (int i = 1; ; ++i)
	{
		std::string strScriptName;
		LUA_GET_VALUE_NOASSERT(i, strScriptName, break);

		//std::string strChanged = boost::replace_all_copy( strScriptName, " ", "" );

		//if( strScriptName != strChanged )
		//{
		//    START_LOG( clog, dbg::endl << strScriptName << L" Changes " << strChanged );
		//    //CString str = "./Stage/";
		//    //str += strScriptName.c_str();
		//    //CString strCh = "./Stage/";
		//    //strCh += strChanged.c_str();
		//    //CFile::Rename( str, strCh );
		//    strScriptName = strChanged;
		//}
		m_vecPartCategoryList.push_back(strScriptName);
	}
}

void CLayerManager::SetLayerSize(KLuaManager& luaMgr)
{
	LUA_BEGIN_TABLE("LAYER_SIZE_DATA", return)
	{
		for (int i = 1; ; ++i)
		{
			LUA_BEGIN_TABLE(i, break)
			{
				int iLayerNum;
				float fSizeX, fSizeY;
				LUA_GET_VALUE(1, iLayerNum, break);
				LUA_GET_VALUE(2, fSizeX, break);
				LUA_GET_VALUE(3, fSizeY, break);

				m_vecLayer[iLayerNum]->SetLayerNum(iLayerNum);
				m_vecLayer[iLayerNum]->SetLayerXSize(fSizeX * TO_TOOL_COORDINATE_RATE);
				m_vecLayer[iLayerNum]->SetLayerYSize(fSizeY * TO_TOOL_COORDINATE_RATE);
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return);
}