#include "stdafx.h"
#include "WOGMapTOol.h"
#include ".\kgcmap.h"
#include "LayerManager.h"

extern GCDeviceShader*   g_pRenderState;

ImplementSingleton(KGCMap);

KGCMap::KGCMap(void)
	: KMap<CFootHold>()
	, CFHBlockInfo()
{
	RegisterLuabind();
}

KGCMap::~KGCMap(void)
{
	UnLoad();
}

void KGCMap::RegisterLuabind()
{
	lua_State* L = KGCLuabinder_Map::getInstance().GetLuaState();

	lua_tinker::class_add<KGCMap>(L, "KGCMap");
	lua_tinker::class_def<KGCMap>(L, "Load", &KGCMap::Load);
}

void KGCMap::Load()
{
	UnLoad();
	g_AppFormView->InitDungeon(m_iFHWidth, m_iFHHeight);
}

void KGCMap::UnLoad()
{
	g_AppFormView->DeleteDungeon();
}

void KGCMap::PreRender()
{
	SiCLayerManager()->PreRender();
}

void KGCMap::PostRender()
{
	SiCLayerManager()->PostRender();
}

void KGCMap::Render()
{
	SiCLayerManager()->PreRender();
	SiCLayerManager()->PostRender();
}

void KGCMap::FrameMove(float fElapsedTime)
{
	SiCLayerManager()->FrameMove(fElapsedTime);
}

std::string KGCMap::GetScriptHeader()
{
	std::stringstream str;

	str << "BASE_DATA = " << std::endl;
	str << "{\n";
	str << "\tWORLD_WIDTH = " << m_iFHWidth << ",\n";
	str << "\tWORLD_HEIGHT = " << m_iFHHeight << ",\n";
	return str.str();
}

std::string KGCMap::GetFootHoldEnum()
{
	std::stringstream str;
	str << "FHP_JUST_FOOTHOLD\t=0\t-- 그런거 없다. 올라가는 발판임\n"
		<< "FHP_NO_JUMP_DOWN\t=1\t-- ↓ 키를 눌러서 못내려간다.\n"
		<< "FHP_NO_JUMP_UP\t\t=2\t-- ↑ 키를 눌러서 못내려간다.\n"
		<< "FHP_NO_COME_RIGHT\t=4\t-- ← 키를 눌러서 못감.\n"
		<< "FHP_NO_COME_LEFT\t=8\t-- → 키를 눌러서 못감.\n"
		<< "FHP_DEAD_FOOTHOLD\t=16\t-- 충돌하면 뒤진다.\n"
		<< "\n\n";

	return str.str();
}

std::string KGCMap::GetFootHoldHeader()
{
	std::stringstream str;
	str << GetFootHoldEnum();
	str << "FOOTHOLD_DATA = \n";
	str << "{\n";

	return str.str();
}

void KGCMap::AddDungeonFootHold(KLuaManager& luaMgr)
{
	int PosX = 0;
	int PosY = 0;
	DWORD Prop = 0;
	std::string strValue;

	LUA_GET_VALUE_DEF(1, PosX, 0);
	LUA_GET_VALUE_DEF(2, PosY, 0);
	LUA_GET_VALUE_DEF(3, Prop, foothold_type::FHP_JUST_FOOTHOLD);
	LUA_GET_VALUE_DEF(4, strValue, "");

	CToolObject** ppObj = g_AppFormView->GetppObj();
	ppObj[PosX][PosY].SetPos(PosX, PosY, foothold_type::FHT_HEAD);
	ppObj[PosX][PosY].SetProp(Prop, "FOR");
	ppObj[PosX][PosY].SetValue(strValue);
}

void KGCMap::LoadBaseData(KLuaManager& luaMgr)
{
	LUA_BEGIN_TABLE("BASE_DATA", return)
	{
		int Width, Height;
		LUA_GET_VALUE("WORLD_WIDTH", Width, return);
		LUA_GET_VALUE("WORLD_HEIGHT", Height, return);

		SetWorld(Width, Height);

		int iLayerTot, iGamePlayLayer;
		LUA_GET_VALUE("LAYERTOTAL", iLayerTot, return);
		LUA_GET_VALUE("GAMEPLAYLAYER", iGamePlayLayer, return);

		SiCLayerManager()->SetMaxLayerNum(iLayerTot);
		SiCLayerManager()->SetGamePlayLayer(iGamePlayLayer);
		SiCLayerManager()->SetLayer();
	}
	LUA_END_TABLE(return);
}

void KGCMap::SetWorld(int iWidth, int iHeight)
{
	// Jaeho 2007.09.21
	// m_iFHWidth가 변경되면 UnLoad 함수 메모리 해지 할때 문제 생겨서 한번 해줬어요~
	UnLoad();

	m_iFHWidth = iWidth;
	m_iFHHeight = iHeight;

	m_fWidth = m_iFHWidth * FOOTHOLDSIZE;
	m_fHeight = m_iFHHeight * FOOTHOLDSIZE;

	Load();
}

void KGCMap::AddDungeonFootHoldByValue(float PosX, float PosY, DWORD Prop, void* pParent, std::string strValue)
{
}

void KGCMap::RemoveDungeonFootHold(float PosX, float PosY, void* pParent)
{
}