#pragma once

//#include "stdafx.h"
//#include "LayerManager.h"
//#include "WOGMapTOol.h"
#include "ToolObject.h"
#include "KMap.h"
#include "FootHold.h"
#include "FHVector.h"
#include "CRenderEyeInfo.h"

#define FOOTHOLDSIZE (20.0f)
#define FOOTHOLDSIZE_INT (20)

class CToolObject;

class KGCMap : public KMap<CFootHold>
	, public CFHBlockInfo
{
	DeclareSingleton(KGCMap);
public:
	KGCMap(void);
	~KGCMap(void);

	virtual void                    RegisterLuabind();
	virtual void                    Render();
	virtual void                    FrameMove(float fElapsedTime);
	virtual void                    Load();
	virtual void                    UnLoad();
	virtual void                    AddDungeonFootHold(KLuaManager& luaMgr);
	virtual void                    AddDungeonFootHoldByValue(float PosX, float PosY, DWORD Prop, void* pParent, std::string strValue);
	virtual void                    RemoveDungeonFootHold(float PosX, float PosY, void* pParent);
	virtual void                    LoadBaseData(KLuaManager& luaMgr);
	virtual void                    SetBlockWidth(int Width) { m_iFHWidth = Width; m_fWidth = static_cast<float>(m_iFHWidth * FOOTHOLDSIZE); }
	virtual void                    SetBlockHeight(int Height) { m_iFHHeight = Height; m_fHeight = static_cast<float>(m_iFHHeight * FOOTHOLDSIZE); }

	std::string                     GetScriptHeader();
	std::string                     GetFootHoldHeader();
	std::string                     GetFootHoldEnum();

	virtual SHORT                   GetSBlockWidth() { return m_iFHWidth; }
	virtual SHORT                   GetSBlockHeight() { return m_iFHHeight; }

	void                            PreRender();
	void                            PostRender();

	virtual void                    SetWorld(int iWidth, int iHeight);
};

DefSingletonInline(KGCMap);
