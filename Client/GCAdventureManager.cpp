#include "GCAdventureManager.h"

ImplementSingleton(GCAdventureManager);

GCAdventureManager::GCAdventureManager(void)
	: m_iCurrentContinent(0)
{
	m_mapCollectData.clear();
	m_mapContinentData.clear();
	m_mapContinentRewardInfo.clear();
	m_mapDungeonCollectData.clear();
	m_mapContinentRewardCheck.clear();
	m_bEventPeriod = false;
	m_bShowItemInfo = true;
	m_bObtinStr = false;
}

GCAdventureManager::~GCAdventureManager(void)
{
}
//초기화
void GCAdventureManager::InitData()
{
	m_mapCollectData.clear();
	m_mapContinentData.clear();
	m_mapContinentRewardInfo.clear();
	m_mapDungeonCollectData.clear();
	m_bEventPeriod = true;
	m_bShowItemInfo = true;
	m_bObtinStr = false;

	for (int i = 0; i < CONTINENT_MAX; i++) {
		m_mapContinentRewardCheck[i] = false;
	}
}

//이벤트 기간내에 UI 클릭시, 클라 -> 서버 요청한 값을 컨테이너에 저장
void GCAdventureManager::OnEventAdventureDataAck(KEVENT_ADVENTURE_DATA_ACK& kRecv_)
{
	InitData();

	m_mapCollectData = kRecv_.m_mapCollectData;       // 전리품 획득 정보(<대륙 ID, Collect ID>, 얼마나 모았나)
	m_mapContinentData = kRecv_.m_mapContinentData;     // 대륙 보상 정보(대륙ID, 지급했나)

	m_mapContinentRewardInfo = kRecv_.m_mapContinentRewardInfo;      // 대륙 보상 정보;  key: m_nContinentID
	m_kFinalRewardInfo = kRecv_.m_kFinalRewardInfo;          // 최종 보상 정보;  key: m_nCharType
}
//대륙 보상 요청
void GCAdventureManager::ContinentRewardReq(int iContinentID)
{
	//대륙 보상 REQ 보내기
	KEVENT_ADVENTURE_CONTINENT_REWARD_REQ kPacket;

	kPacket.m_nContinentID = iContinentID;
	kPacket.m_vecDropItemInfo.clear();

	Result_ContinentRewardReq = INT_MAX;
	KP2P::GetInstance()->Send_GcAdventureContinentRewardReq(kPacket);
	g_MyD3D->WaitForServerAck(Result_ContinentRewardReq, INT_MAX, 3000, TIME_OUT_VALUE);

	if (Result_ContinentRewardReq == 0)
	{
		//대륙 보상을 받은 후, 전체 대륙 표시 및 보상받기 버튼을 잠궈 준다.
		CheckContinentRewardBtn(iContinentID);
		SetContinentReward(iContinentID);
	}
}
//전체 대륙 보상 받기
void GCAdventureManager::WholeContinentFinalRewardReq()
{
	//전체 대륙 보상 REQ 보내기
	Result_WholeContinentFinalRewardReq = INT_MAX;
	KP2P::GetInstance()->Send_GcAdventureWholeContinentFinalRewardReq();
	g_MyD3D->WaitForServerAck(Result_WholeContinentFinalRewardReq, INT_MAX, 3000, TIME_OUT_VALUE);
}
//현재 선택된 대륙 상태 저장
void GCAdventureManager::SetCurrentContinent(int iCurrentContinent)
{
	m_iCurrentContinent = 0;
	m_iCurrentContinent = iCurrentContinent;
}

int GCAdventureManager::GetCurrentContinent()
{
	return m_iCurrentContinent;
}

//해당 대륙의 전리품 아이템 정보
bool GCAdventureManager::GetContinentItemInfo(int iContinentID, OUT std::map<DWORD, KCollectItem >& mapCollectItems_)
{
	std::map< int, KContinentRewardInfo >::iterator mitContinentRewardInfo = m_mapContinentRewardInfo.find(iContinentID);

	if (mitContinentRewardInfo != m_mapContinentRewardInfo.end())
	{
		mapCollectItems_ = mitContinentRewardInfo->second.m_mapCollectItems;
		return true;
	}
	else
		return false;
}

//대륙 ID, CollectID 가지고 어떤 아이템 목록이 있는지 알아 낸다.
bool GCAdventureManager::GetContinentItemGoodsID(int iContinentID, DWORD dwCollectID, OUT DWORD& dwItemGoodsID)
{
	std::map< int, KContinentRewardInfo >::iterator mitContinentRewardInfo = m_mapContinentRewardInfo.find(iContinentID);

	if (mitContinentRewardInfo != m_mapContinentRewardInfo.end()) {
		dwItemGoodsID = mitContinentRewardInfo->second.m_mapCollectItems[dwCollectID].m_dwDisplayItemID;
		return true;
	}
	else {
		return false;
	}
}
//지정된 대륙의 CollectID로 모아야할 전리품 최대 갯수를 구한다.
bool GCAdventureManager::GetItemMaxCount(int iContinentID, DWORD dwCollectID, OUT DWORD& dwMaxCnt)
{
	std::map< int, KContinentRewardInfo >::iterator mitContinentRewardInfo = m_mapContinentRewardInfo.find(iContinentID);

	if (mitContinentRewardInfo != m_mapContinentRewardInfo.end())
	{
		dwMaxCnt = mitContinentRewardInfo->second.m_mapCollectItems[dwCollectID].m_dwMaxCount;
		return true;
	}
	else
	{
		return false;
	}
}

//해당 대륙의 보상 정보
bool GCAdventureManager::GetContinentRewardInfo(int iContinentID, OUT std::vector< KDropItemInfo >& vecContinentRewards_)
{
	std::map< int, KContinentRewardInfo >::iterator mitContinentRewardInfo = m_mapContinentRewardInfo.find(iContinentID);

	if (mitContinentRewardInfo != m_mapContinentRewardInfo.end())
	{
		vecContinentRewards_ = mitContinentRewardInfo->second.m_vecContinentRewards;
		return true;
	}
	else {
		return false;
	}
}

//해당 대륙에 내가 보상을 받을 수 있나?
bool GCAdventureManager::CanObtainContinentReward(int iContinentID)
{
	int iCount = 0;
	std::map< int, KContinentRewardInfo >::iterator mitContinentRewardInfo = m_mapContinentRewardInfo.find(iContinentID);
	int iSize = static_cast<int> (mitContinentRewardInfo->second.m_mapCollectItems.size());
	std::map <DWORD, int> mapCollectData;
	mapCollectData.clear();

	if (false == GetObtainCnt(iContinentID, mapCollectData))
		return false;

	if (m_mapContinentData[iContinentID] == false)
	{
		for (int i = 0; i < iSize; i++)
		{
			if (mapCollectData[i] >= static_cast<int> (mitContinentRewardInfo->second.m_mapCollectItems[i].m_dwMaxCount))
				iCount++;
		}
	}

	if (iCount == iSize && m_mapContinentRewardCheck[iContinentID] == false)
		return true;
	else if (m_mapContinentRewardCheck[iContinentID])
		return false;
	else
		return false;
}

//해당 대륙에 내가 모은 전리품 갯수 현황
bool GCAdventureManager::GetObtainCnt(int iContinentID, OUT std::map< DWORD, int >& mapCollectData_)
{
	std::map< int, std::map< DWORD, int > >::iterator mitCollectData = m_mapCollectData.find(iContinentID);

	if (mitCollectData != m_mapCollectData.end()) {
		mapCollectData_ = m_mapCollectData[iContinentID];
		return true;
	}
	else
		return false;
}

//대륙의 보상 물품들 얼마나 모았나 갱신 해주기 위한 Set 함수
void GCAdventureManager::SetCollectData(std::map< int, std::map< DWORD, int > > mapCollectData)
{
	m_mapCollectData.clear();
	m_mapCollectData = mapCollectData;
}

//KEVENT_ADVENTURE_MISSION_MONSTER_DATA_NOT( 던전내에 어떤 Trigger를 가진 몬스터가 어떤 아이템을 드랍하는지 정보들)을 이곳에서 받는다.
void GCAdventureManager::SetDungeonCollectData(KEVENT_ADVENTURE_MISSION_MONSTER_DATA_NOT& kRecv)
{
	m_mapDungeonCollectData.clear();
	m_mapDungeonCollectData = kRecv;
}

//트리거 아이디 검사
bool  GCAdventureManager::IsValidTriggerID(int iTriigerID)
{
	std::map< int, KAdventureMonster > ::iterator mitMonsterInfo = m_mapDungeonCollectData.find(iTriigerID);

	if (mitMonsterInfo != m_mapDungeonCollectData.end())
		return true;
	else
		return false;
}

//현재 이벤트 기간인가 체크
bool GCAdventureManager::isValidEventPeriod()
{
	return m_bEventPeriod;
}

//서버에서 트리거 ID (어떤 몬스터가 죽으면 카운팅하는지) 정보를 주면 이 정보를 가지고 메세지를 띄운다.
void GCAdventureManager::GetTriggerIDInfoAndAddCnt(int iTriggerID)
{
	int iContinentID = m_mapDungeonCollectData[iTriggerID].m_nContinentID;
	DWORD dwCollectID = m_mapDungeonCollectData[iTriggerID].m_dwCollectID;
	DWORD dwDropCount = m_mapDungeonCollectData[iTriggerID].m_dwDropCount;
	DWORD dwMaxCount = 0;
	DWORD dwGoodsId = 0;

	if (false == GetItemMaxCount(iContinentID, dwCollectID, dwMaxCount))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_GC_ADVENTURE_DATA_ERR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	}

	if (false == GetContinentItemGoodsID(iContinentID, dwCollectID, dwGoodsId))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_GC_ADVENTURE_DATA_ERR), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	}

	std::map< DWORD, int >::iterator mitCollectData = m_mapCollectData[iContinentID].begin();

	for (; mitCollectData != m_mapCollectData[iContinentID].end(); mitCollectData++)
	{
		if (mitCollectData == m_mapCollectData[iContinentID].end())
			continue;

		if (mitCollectData->first == dwCollectID && mitCollectData->second < static_cast<int>(dwMaxCount))
		{
			mitCollectData->second = mitCollectData->second + static_cast<int>(dwDropCount);

			SetPrintObtainStr(true);

			g_pkGameOverlayUI->PushMissionProcessStr(
				g_pkStrLoader->GetReplacedString(STR_ID_MISSION_MESSAGE1, "lii", g_pItemMgr->GetItemName(dwGoodsId / 10), mitCollectData->second, static_cast<int>(dwMaxCount))
			);
		}
	}
}

void GCAdventureManager::SetShowItemInfo(bool bShowItem)
{
	m_bShowItemInfo = bShowItem;
}

bool GCAdventureManager::GetShowItemInfo()
{
	return m_bShowItemInfo;
}

//대륙 보상을 받은 상태 인지 검사 한다.
bool GCAdventureManager::CheckContinentReward(int iContinentID)
{
	std::map< int, bool >::iterator mitCheckContinentReward = m_mapContinentData.find(iContinentID);

	return mitCheckContinentReward->second;
}

//전체 대륙 보상을 받을 상태 인지 검사(최종 보상 검사)
bool GCAdventureManager::CanObtainFinalReward()
{
	int iCount = 0;
	std::map< int, bool >::iterator mitCheckContinentReward = m_mapContinentData.begin();

	for (; mitCheckContinentReward != m_mapContinentData.end(); mitCheckContinentReward++)
	{
		if (mitCheckContinentReward == m_mapContinentData.end())
			continue;

		if (mitCheckContinentReward->second == true)
			iCount++;
	}

	if (iCount == static_cast<int> (m_mapCollectData.size()))
		return true;
	else
		return false;
}

GCITEMID GCAdventureManager::GetFinalRewardItemGoodsID()
{
	return m_kFinalRewardInfo.m_ItemID;
}

void GCAdventureManager::CheckContinentRewardBtn(int iContinentID)
{
	m_mapContinentRewardCheck[iContinentID] = true;
}

void GCAdventureManager::SetContinentReward(int iContinentID)
{
	std::map< int, bool >::iterator mitCheckContinentReward = m_mapContinentData.find(iContinentID);

	if (mitCheckContinentReward != m_mapContinentData.end())
		mitCheckContinentReward->second = true;
}

void GCAdventureManager::SetPrintObtainStr(bool bObtinStr_)
{
	m_bObtinStr = bObtinStr_;
}

bool GCAdventureManager::GetPossibleObtainStr()
{
	return m_bObtinStr;
}