#pragma once

class GCItemCombinationManager
{
	DeclareSingleton(GCItemCombinationManager);

	enum {
		MAX_STUFF_WDITH = 4,
		MAX_STUFF_LIST_WIDTH = 5,
		MAX_REWARD_WIDTH = 6,
	};

public:
	typedef bool(*RecipeSortFunc)(const RecipeSimpleInfo& lhs, const RecipeSimpleInfo& rhs, bool bAscending_);
	typedef std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> > MAP_FM_MATERIAL_LIST;

	GCItemCombinationManager(void);
	~GCItemCombinationManager(void);

	int GetMaxStuffSlot() { return MAX_STUFF_WDITH; }
	int GetMaxStuffListSlot() { return MAX_STUFF_LIST_WIDTH; }
	int GetMaxRewardSlot() { return MAX_REWARD_WIDTH; }
	void OnItemComposeInfoNOT(KEVENT_ITEM_COMPOSE_INFO_NOT& kRecv);
	void InitData();
	void SetEventPeriod(bool isEventPeriod);
	bool GetEventPeriod() { return m_isEventPeriod; }
	bool MakeMaterialList(OUT std::vector<std::pair<GCITEMID, GCITEMUID>>& vecInvenList);
	bool GetRewardItemList(OUT std::vector<GCITEMID>& vecRewardList);
	DWORD GCItemCombinationManager::GetMaterialListNum() { return m_dwMaxMaterialListNum; }
	void Send_ItemComposeReq();
	void SetEndParticleTime(bool bEndParticle);
	bool GetEndParticleTime() { return m_bEndParticle; }
	void SetResultItemUID(IN std::vector<GCITEMUID>& vecCombinationList);
	void SetStuffSelect(bool bEnbaleStuffClick);
	bool GetStuffSelect() { return m_bEnbaleStuffClick; }
public:
	DWORD                               m_dwMaxMaterialListNum;
	std::vector<GCITEMID>               m_vecMaterialList; // 아이템 재료 정보.
	std::vector<GCITEMID>               m_vecRewardList;   // 보상 아이템 정보.
	std::vector<GCITEMUID>              m_vecCombinationList;
	bool m_isEventPeriod; //이벤트 기간이냐
	bool m_bEndParticle; //파티클 타임 체크
	bool m_bEnbaleStuffClick;  //파티클 시간중에 재료 선택 및 취소 선택을 하지 않게 하기 위한 변수
};
DefSingletonInline(GCItemCombinationManager);
