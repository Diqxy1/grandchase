#pragma once

#include "KLayer.h"
#include "LayerPart.h"

class CLayerManager;

class CLayer : public KLayer<CLayerPart, CLayerManager>
{
public:
	CLayer(CLayerManager* pManager);
	virtual ~CLayer(void);

	virtual void    Process();

	// IGCRenderObj
	//virtual HRESULT Render();
	std::vector< CLayerPart* >& GetPartVector() { return m_vecPart; }

	virtual void AddParts(KLuaManager& luaMgr);
	virtual void    AddPartsErrFunc(std::string partName_);

	// 2007/5/20. daeuk. 맵툴은 리스트가 계속 바뀔 수 있기 때문에 이렇게 해주자.
	void            BuildRenderList();

	void            FindPartsByName(std::string strName, std::vector< CLayerPart* >& vecParts, bool bSame);
	void            FrameMove(float fElapsedTime);
	void            EraseLayerPart(CLayerPart* pPart);
	void            EraseLayerObject(CLayerPart* pPart);
	void            AddLayerPart(CLayerPart* pPart);
	void            AddLayerPartIndex(CLayerPart* pPart, int iIndex);

	void            SortByZOrder();
	void            ResetZOrder(int DeletedZOrder = -1);

	std::string     GetLayerHeader();
	std::string     GetLayerSizeData();

	void            FindLayerPartsByPoint(D3DVECTOR pt, std::vector< CLayerPart* >& vecOut);
	bool            GetLayerLocationByPtr(CLayerPart* pPart, int& iIndex);
	void            UpdateCategoryUse(std::map< std::string, int >& strCategoryUse);

	void            RenderFH();
	void            InitEyeBlockInfo(CFHBlockInfo* pBlockInfo, CRenderEyeInfo* pRenderEyeInfo);
	void            RenderHeadFH();

	virtual         void CreateBuffer();
};
