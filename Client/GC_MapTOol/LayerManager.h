#pragma once

#include <KNCSingleton.h>
#include "Layer.h"
#include "LayerPart.h"
#include "KGCMap.h"
#include "KLayerManager.h"

struct KLayerFootHoldData
{
	SHORT m_sFHWidth;
	SHORT m_sFHHeight;
	std::vector< CToolObject > m_vecTO;

	KLayerFootHoldData()
	{
		m_sFHHeight = 0;
		m_sFHWidth = 0;
		m_vecTO.clear();
	}
};

class CLayerManager : public KLayerManager<CLayerPart, CLayer, KGCMap>
{
	DeclareSingleton(CLayerManager);

public:
	CLayerManager(void);
	~CLayerManager(void);

	virtual void                FrameMove(float fElapsedTime);
	virtual void                RegisterLuabind();
	virtual void                clear();
	virtual void                SetLayer();
	virtual void                SetLayerSize(KLuaManager& luaMgr);
	virtual void                AddLayerParts();
	virtual void                AddLayerPartsErr(std::string partName_);
	virtual bool                LoadFromLua(KLayerPartData& kData);
	virtual void                LoadParts(KLuaManager& luaMgr);
	virtual void                SetPartsCategoryList();

	void                        UpdateCategoryUse();

	void                        PreRender();
	void                        PostRender();

	std::vector< std::string >  GetCategoryList() { return m_vecPartCategoryList; }
	void                        SetCategoryList(std::vector< std::string >& CategoryList);
	void                        EraseUnUseCategory(const std::string strCategoryname);
	std::vector< std::string >& GetUsingCategoryList() { return m_vecUsingPartCategoryList; }
	void                        SetUsingCategoryList(std::vector< std::string > UsingList) { m_vecUsingPartCategoryList.clear(); m_vecUsingPartCategoryList = UsingList; }
	std::string                 GetCategoryListString();
	std::map< std::string, CLayerPart* >& GetPartPool() { return m_mapPartPool; }
	void                        AddCategoryList(std::string CategoryName) { m_vecPartCategoryList.push_back(CategoryName); }
	void                        AddLayerPart(CLayerPart* pPart)
	{
		if (pPart != NULL)
		{
			m_mapPartPool[pPart->GetPartName()] = pPart;
		}
	}
	void                        EraseLayerPart(CLayerPart* pPart);
	void                        DeleteCategory(std::string CategoryName);
	std::string                 GetScriptHeader();

	void CreateEmptyDefaultLayer(float fWidth, float fHeight);

	void                        FindPartsByName(std::string strName, std::vector< CLayerPart* >& vecParts, bool bSame = true);

	std::string                 GetLayerInfoString();
	std::string                 GetLoadPartsString();
	std::string                 GetCreateLayerHeader();
	std::string                 GetLayerSizeHeader();

	void                        FindLayerPartsByPoint(D3DVECTOR pt, std::vector< CLayerPart* >& vecOut);
	void                        GetLayerLocationByPtr(CLayerPart* pPart, int& iLayerNum, int& iIndex);

	bool                        LoadFootHoldData(KLayerFootHoldData& kLayerFHData);

	void                        RenderFH();
	void                        RenderHeadFH();
	virtual bool                LoadScript(std::string Script);

private:
	std::vector< std::string >      m_vecUsingPartCategoryList;
};
DefSingletonInline(CLayerManager);
