#include "stdafx.h"
#include "KGCWorldMapPanel.h"

#include "KGCWorldMapIcon.h"
#include "KGCWorldMapPath.h"
//
//
#include "GCChatBox.h"
#include "KGCWorldMapContainer.h"
#include "KGCRoomListManager.h"

#include "GCUI/GCNameTag.h"

IMPLEMENT_CLASSNAME(KGCWorldMapPanel);
IMPLEMENT_WND_FACTORY(KGCWorldMapPanel);
IMPLEMENT_WND_FACTORY_NAME(KGCWorldMapPanel, "gc_worldmap_panel");

KGCWorldMapPanel::KGCWorldMapPanel(void)
{


	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_ICON; ++i)
	{
		m_apkWorldMapIcon[i] = NULL;
	}
	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_PATH; ++i)
		m_apkWorldMapPath[i] = NULL;

	for (int i = 0; i < KGCWorldMapManager::NUM_UPPERWORLDMAP; ++i)
		m_pkUpperTitle[i] = NULL;

	char strWndName[MAX_PATH] = { 0, };
	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_ICON; ++i)
	{
		sprintf(strWndName, "map_icon%02d", i);
		LINK_CONTROL(strWndName, m_apkWorldMapIcon[i]);
	}

	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_PATH; ++i)
	{
		sprintf(strWndName, "map_path%02d", i);
		LINK_CONTROL(strWndName, m_apkWorldMapPath[i]);
	}

	for (int i = 0; i < KGCWorldMapManager::NUM_UPPERWORLDMAP; ++i)
	{
		sprintf(strWndName, "continent_base_img%d", i);
		LINK_CONTROL(strWndName, m_pkUpperTitle[i]);
	}

#ifdef DIMENSIONAL_BREAK
	m_pkIconErnas = NULL;
	LINK_CONTROL("base_icon_ernas", m_pkIconErnas);

	m_pkIconElyos = NULL;
	LINK_CONTROL("base_icon_elyos", m_pkIconElyos);
#endif

	m_pkGpShop = NULL;
	m_pkManufacture = NULL;
	m_pkReinforce = NULL;

	LINK_CONTROL("gpshop", m_pkGpShop);
	LINK_CONTROL("manufacture", m_pkManufacture);
	LINK_CONTROL("reinforce", m_pkReinforce);

	//m_pkNameTag = NULL;
	m_mapCharIcon.clear();

	m_pkFocusingIcon = NULL;
}

KGCWorldMapPanel::~KGCWorldMapPanel(void)
{
}

void KGCWorldMapPanel::OnCreate()
{
#ifdef DIMENSIONAL_BREAK
	if (m_pkIconErnas != NULL)
		m_pkIconErnas->InitState(true, true, this);

	if (m_pkIconElyos != NULL)
		m_pkIconElyos->InitState(true, true, this);
#endif

	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_ICON; ++i)
	{
		if (m_apkWorldMapIcon[i] == NULL)
			continue;

		m_apkWorldMapIcon[i]->InitState(true, true, this);
		m_apkWorldMapIcon[i]->SetIndex(i);
	}

	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_PATH; ++i)
	{
		if (m_apkWorldMapPath[i] == NULL)
			continue;

		m_apkWorldMapPath[i]->InitState(true);
	}

	for (int i = 0; i < KGCWorldMapManager::NUM_UPPERWORLDMAP; ++i)
	{
		if (m_pkUpperTitle[i] == NULL)
			continue;
		m_pkUpperTitle[i]->InitState(true, true, this);
	}
	if (m_pkGpShop)
	{
		m_pkGpShop->InitState(true, true, this);
		m_pkGpShop->SetContentsType(STATE_GP_SHOP);
	}
	if (m_pkManufacture)
	{
		m_pkManufacture->InitState(true, true, this);
		m_pkManufacture->SetContentsType(STATE_MANUFACTURE);
	}
	if (m_pkReinforce)
	{
		m_pkReinforce->InitState(true, true, this);
		m_pkReinforce->SetContentsType(STATE_ENCHANT);
	}

	InitCharIconTexInfo();
}

void KGCWorldMapPanel::InitCharIconTexInfo()
{
	int   const iMaxCharNumInOneLine = 8; //이름 짓기 힘들다..
	int   const iTextureNum = 407; //407번 worldmap_charicon.dds에서 필요한 텍스쳐 정보를 불러온다.

	float const fTexWidth = 58.f;
	float const fTexHeight = 91.f;


	for (int j = 0; j < (int)GC_CHAR_NUM; ++j)
	{
		sCharIconTexInfo schartexinfo;
		schartexinfo.clear();
		float fxOffset = 5.f;
		float fyOffset = 5.f;
		int iLine = j / iMaxCharNumInOneLine;

		if (j == 0) fxOffset = 0;
		if (iLine == 0) fyOffset = 0;


		schartexinfo.dxVecTexCoord = D3DXVECTOR4(
			3.f + (j % iMaxCharNumInOneLine * fTexWidth) + fxOffset * (j % iMaxCharNumInOneLine),
			3.f + (iLine * fTexHeight) + fyOffset * iLine,
			61.f + (j % iMaxCharNumInOneLine * fTexWidth) + fxOffset * (j % iMaxCharNumInOneLine),
			94.f + (iLine * fTexHeight) + fyOffset * iLine
		) / 512.f;

		schartexinfo.iTexNum = iTextureNum;
		m_mapCharIcon.insert(std::make_pair((EGCCharType)j, schartexinfo));

	}
}

void KGCWorldMapPanel::ActionPerformed(const KActionEvent& event)
{

	switch (event.m_dwCode)
	{
	case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play("30"); break;//return;
	case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play("31"); break;//return;
	}

	if (event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK)
		return;

#ifdef DIMENSIONAL_BREAK
	if (m_pkIconErnas != NULL)
		if (event.m_pWnd == m_pkIconErnas)
			g_pkUIScene->m_pkWorldMapContainer->GoTo(KGCWorldMapContainer::Mapas::Ernas);

	if (m_pkIconElyos != NULL)
		if (event.m_pWnd == m_pkIconElyos)
			g_pkUIScene->m_pkWorldMapContainer->GoTo(KGCWorldMapContainer::Mapas::Elyos);
#endif

	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_ICON; ++i)
	{
		if (m_apkWorldMapIcon[i] == NULL)
			continue;

		if (event.m_pWnd == m_apkWorldMapIcon[i])
		{
			// ¸Å´ÏÀúÇÑµ¥ ¹¹ ¼±ÅÃÇß´ÂÁö ¾Ë¸®ÀÚ..
			SiKGCWorldMapManager()->SelectIcon(i);
			return;
		}
	}
}
//현재 선택된 캐릭터가 갈 수 있는 대륙의 아이콘과 이름만 보여준다. 
void KGCWorldMapPanel::ShowAvailableContinentIcon()
{
	for (int i = 0; i < KGCWorldMapManager::NUM_UPPERWORLDMAP; ++i)
	{
#ifdef DIMENSIONAL_BREAK
		if (i == KGCWorldMapManager::UPPERWORLDMAP_NEWMAP) continue;

		if (m_pkUpperTitle[i] == NULL)
			continue;
#endif

		if (SiKGCWorldMapManager()->_CheckContinentEnable(SiKGCWorldMapManager()->GetWorldMapEnumFromWorldMapIcon(i)))
			m_pkUpperTitle[i]->ToggleRender(true);
		else
			m_pkUpperTitle[i]->ToggleRender(false);
	}
}
//대륙별로 수행 가능한 퀘스트가 있을때, 퀘스트 아이콘을 띄운다.
void KGCWorldMapPanel::ShowContinentQuestIcon()
{
	for (int i = 0; i < KGCWorldMapManager::NUM_UPPERWORLDMAP; ++i) {
#ifdef DIMENSIONAL_BREAK
		if (m_pkUpperTitle[i] == NULL)
			continue;
#endif
		m_pkUpperTitle[i]->SetRenderQuestIcon(false);
	}

	for (int i = 0; i < NUM_WORLDMAP; ++i)
	{
		if ((EGCWorldMap)i == WORLDMAP_UPPERMAP)
			continue;

		int iExitQuestContinent = -1;
		if (SiKGCWorldMapManager()->CheckQuestInContinent((EGCWorldMap)i))
		{
			//이 또한 월드맵 UI의 대륙 배치가 엉망인 관계로 따로 Enum 값을 따로 동기화 시키는 과정이 필요함
			switch ((EGCWorldMap)i)
			{
			case WORLDMAP_BERMESIA_SERDIN:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_BERMESIA_SERDIN;
				break;
			case WORLDMAP_BERMESIA_CANAVAN:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_BERMESIA_CANAVAN;
				break;
			case WORLDMAP_SILVERLAND:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_SILVERLAND;
				break;
			case WORLDMAP_ELIA:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ELIA;
				break;
			case WORLDMAP_XENEA:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_XENEA;
				break;
			case WORLDMAP_ATUM:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ATUM;
				break;
			case WORLDMAP_ACHAEMEDIA:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ACHAEMEDIA;
				break;
			case WORLDMAP_FLOSLAND:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_FLOSLAND;
				break;
			case WORLDMAP_DEVILDOM:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_DEVILDOM;
				break;
#ifdef ELYOS
			case WORLDMAP_ELYOS:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ELYOS;
				break;
#endif
#ifdef DIMENSIONAL_BREAK
			case WORLDMAP_NEWMAP:
				iExitQuestContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_NEWMAP;
				break;
#endif
			}
#ifdef DIMENSIONAL_BREAK
			if (m_pkUpperTitle[iExitQuestContinent] != NULL)
#endif
			m_pkUpperTitle[iExitQuestContinent]->SetRenderQuestIcon(true);
		}
	}
}

//상위 월드맵으로 나오기전에 있었던 대륙을 체크해서 해당 대륙 위에 현재 선택된 캐릭터 아이콘을 띄운다.
void KGCWorldMapPanel::ShowPreContinentCharIcon()
{
	std::map< EGCCharType, sCharIconTexInfo >::iterator itrPos = m_mapCharIcon.find((EGCCharType)g_kGlobalValue.m_kUserInfo.GetCurrentCharType());

	if (itrPos == m_mapCharIcon.end())
		return;

	EGCWorldMap ePreWorldMap = (EGCWorldMap)g_kGlobalValue.GetPreContinent();

	for (int i = 0; i < KGCWorldMapManager::NUM_UPPERWORLDMAP; ++i)
	{
#ifdef DIMENSIONAL_BREAK
		if (m_pkUpperTitle[i] == NULL)
			continue;
#endif
		m_pkUpperTitle[i]->SetRenderCharIcon(false);
		m_pkUpperTitle[i]->SetCharIconTexNum(itrPos->second.iTexNum);
		m_pkUpperTitle[i]->SetCharIconTexCoord(itrPos->second.dxVecTexCoord);
	}

	int iPreContinent = -1;
	switch (ePreWorldMap)
	{
	case WORLDMAP_BERMESIA_SERDIN:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_BERMESIA_SERDIN;
		break;
	case WORLDMAP_BERMESIA_CANAVAN:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_BERMESIA_CANAVAN;
		break;
	case WORLDMAP_SILVERLAND:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_SILVERLAND;
		break;
	case WORLDMAP_ELIA:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ELIA;
		break;
	case WORLDMAP_XENEA:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_XENEA;
		break;
	case WORLDMAP_ATUM:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ATUM;
		break;
	case WORLDMAP_ACHAEMEDIA:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ACHAEMEDIA;
		break;
	case WORLDMAP_FLOSLAND:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_FLOSLAND;
		break;
	case WORLDMAP_DEVILDOM:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_DEVILDOM;
		break;
#ifdef ELYOS
	case WORLDMAP_ELYOS:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_ELYOS;
		break;
#endif
#ifdef DIMENSIONAL_BREAK
	case WORLDMAP_NEWMAP:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_NEWMAP;
		break;
#endif
	default:
		iPreContinent = (int)KGCWorldMapManager::UPPERWORLDMAP_BERMESIA_SERDIN;
	}
#ifdef DIMENSIONAL_BREAK
	if (m_pkUpperTitle[iPreContinent] != NULL)
#endif
	m_pkUpperTitle[iPreContinent]->SetRenderCharIcon(true);

}

void KGCWorldMapPanel::FrameMoveInEnabledState(void)
{
	const KSafeArray<KGCWorldMapPanel*, NUM_WORLDMAP >& worldmappanel = g_pkUIScene->m_pkWorldMapContainer->GetWolrdMapPanel();
	if (worldmappanel[SiKGCWorldMapManager()->GetCurrentWorldMap()] == this)
	{
		for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_ICON; ++i)
		{
			if (m_apkWorldMapIcon[i] == NULL)
				continue;

			//if( SiKGCWorldMapManager()->IsHeroDungeon(SiKGCWorldMapManager()->GetGameMode(i)) && SiKGCRoomListManager()->IsHellMode() )
			//    m_apkWorldMapIcon[i]->ToggleRender(false);

			if (m_apkWorldMapIcon[i]->IsRenderOn() == false)
				continue;

			m_apkWorldMapIcon[i]->CheckImages();
			//카나반만 이펙트 띄우는 부분 (하드함... )
			if (SiKGCWorldMapManager()->GetCurrentWorldMap() == WORLDMAP_UPPERMAP && true == g_MyD3D->m_kGuideMgr.GetContinentEffect()) {
				if (m_apkWorldMapIcon[8])
					m_apkWorldMapIcon[8]->EffectContinentBlink(true);
			}
		}
	}
#if !defined (USE_NEWBIE_GUIDE)
	//m_pkNameTag->ToggleRender(false);
	if (SiKGCWorldMapManager()->GetCurrentWorldMap() == WORLDMAP_UPPERMAP)
		return;
	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_ICON; ++i)
	{
		if (m_apkWorldMapIcon[i] == NULL)
			continue;
		if (false == m_apkWorldMapIcon[i]->IsRenderOn())
			continue;

		if (m_apkWorldMapIcon[i]->CheckMousePosInWindowBound())
		{
			m_pkFocusingIcon = m_apkWorldMapIcon[i];
			SetNameTag(i);
			//m_pkNameTag->ToggleRender(true);
			break;
		}
	}
#endif

	if (g_MyD3D->m_pStateMachine->GetState() != GS_WORLDMAP) {
		if (m_pkReinforce)     m_pkReinforce->ToggleRender(false);
		if (m_pkGpShop)         m_pkGpShop->ToggleRender(false);
		if (m_pkManufacture)   m_pkManufacture->ToggleRender(false);

		return;
	}

	EGCWorldMap eCurrentMap = SiKGCWorldMapManager()->GetCurrentWorldMap();

	if (eCurrentMap != WORLDMAP_UPPERMAP)
	{
		//대륙별로 그리고 싶지 않은 것들을 설정한다. 툴에서 아이콘을 없애도 되지만, 다시 넣을지도 모르니.
		switch (eCurrentMap)
		{
		case WORLDMAP_BERMESIA_SERDIN:
			m_pkReinforce->ToggleRender(false);
			m_pkManufacture->ToggleRender(false);
			m_pkGpShop->ToggleRender(true);
			break;
		case WORLDMAP_BERMESIA_CANAVAN:
			m_pkReinforce->ToggleRender(false);
			m_pkManufacture->ToggleRender(false);
			m_pkGpShop->ToggleRender(true);
			break;
		case WORLDMAP_SILVERLAND:
			m_pkReinforce->ToggleRender(false);
			m_pkManufacture->ToggleRender(true);
			m_pkGpShop->ToggleRender(true);
			break;
		case WORLDMAP_ELIA:
		case WORLDMAP_XENEA:
		case WORLDMAP_ATUM:
		case WORLDMAP_ACHAEMEDIA:
			if (m_pkReinforce)     m_pkReinforce->ToggleRender(true);
			if (m_pkGpShop)         m_pkGpShop->ToggleRender(true);
			if (m_pkManufacture)   m_pkManufacture->ToggleRender(true);
			break;
		default:
			if (m_pkReinforce)     m_pkReinforce->ToggleRender(true);
			if (m_pkGpShop)         m_pkGpShop->ToggleRender(true);
			if (m_pkManufacture)   m_pkManufacture->ToggleRender(true);
			break;
		}
	}
}

void KGCWorldMapPanel::PreDraw(void)
{
	g_pGCDeviceManager2->PushState();
	g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

}

void KGCWorldMapPanel::PostChildDraw(void)
{
	g_pGCDeviceManager2->PopState();

}

KGCWorldMapIcon* KGCWorldMapPanel::GetWorldMapIcon(int iIndex_)
{
	if (iIndex_ < 0 || iIndex_ >= KGCWorldMapManager::MAX_WORLDMAP_ICON)
		return NULL;

#ifdef DIMENSIONAL_BREAK
	if (m_apkWorldMapIcon[iIndex_] == NULL)
		return NULL;
#endif

	return m_apkWorldMapIcon[iIndex_];
}

void KGCWorldMapPanel::SetNameTag(const int& iDun_)
{

	//EGCGameMode eMode = SiKGCWorldMapManager()->GetGameMode(iDun_);

	//m_pkNameTag->SetName( SiKGCWorldMapManager()->GetCurrentDungeonName(eMode) );

	//D3DXVECTOR2 vPos = m_pkFocusingIcon->GetFixedWindowLocalPos();

	//float fAddPosX = static_cast<float>(m_pkNameTag->GetWidth()) - static_cast<float>(m_pkFocusingIcon->GetWidth());
	//float fAddPosY = static_cast<float>(m_pkNameTag->GetHeight());

	//vPos.x -= fAddPosX/2;
	//vPos.y -= fAddPosY + 2;

	//m_pkNameTag->SetWindowPosDirect(vPos);

}

void KGCWorldMapPanel::SetDungeonName()
{
	for (int i = 0; i < KGCWorldMapManager::MAX_WORLDMAP_ICON; ++i)
	{
		if (m_apkWorldMapIcon[i] == NULL)
			continue;
		m_apkWorldMapIcon[i]->SetNameTag(i);
	}
}