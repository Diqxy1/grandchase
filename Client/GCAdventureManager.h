#pragma once

#define MAX_CONTINENT_REWARD_ITEM_NUM 9
#define WHOLE_CONTINENT 255

class GCAdventureManager
{
	DeclareSingleton(GCAdventureManager);

public:
	enum WORLDMAP_CONTINENT
	{
		CONTINENT_BERMESIAH = 0, // 배르메시아 대륙
		CONTINENT_ELIA,
		CONTINENT_SILVERLAND,
		CONTINENT_XENIA,
		CONTINENT_ATUM,
		CONTINENT_ACHAEMEDIA,
		CONTINENT_BERMESIA_CANAVAN,
#ifdef ELYOS
		CONTINENT_ELYOS,
#endif
		CONTINENT_MAX,
	};

public:

	GCAdventureManager(void);
	~GCAdventureManager(void);

public:
	void InitData();
	void OnEventAdventureDataAck(KEVENT_ADVENTURE_DATA_ACK& kRecv_);
	void ContinentRewardReq(int iContinentID);
	void WholeContinentFinalRewardReq();
	void SetCurrentContinent(int iCurrentContinent);
	void SetCollectData(std::map< int, std::map< DWORD, int > > mapCollectData);
	void SetDungeonCollectData(KEVENT_ADVENTURE_MISSION_MONSTER_DATA_NOT& kRecv);
	bool CanObtainContinentReward(int iContinentID);
	int GetCurrentContinent();
	bool GetContinentItemGoodsID(int iContinentID, DWORD dwCollectID, OUT DWORD& dwItemGoodsID);
	bool GetContinentItemInfo(int iContinentID, OUT std::map<DWORD, KCollectItem >& mapCollectItems_);
	bool GetContinentRewardInfo(int iContinentID, OUT std::vector< KDropItemInfo >& vecContinentRewards_);
	bool GetObtainCnt(int iContinentID, OUT std::map< DWORD, int >& mapCollectData_);
	bool IsValidTriggerID(int iTriigerID);
	bool isValidEventPeriod();
	void GetTriggerIDInfoAndAddCnt(int iTriggerID);
	bool GetItemMaxCount(int iContinentID, DWORD dwCollectID, OUT DWORD& dwMaxCnt);
	void SetShowItemInfo(bool bShowItem);
	bool GetShowItemInfo();
	bool CheckContinentReward(int iContinentID);
	bool CanObtainFinalReward();
	GCITEMID GetFinalRewardItemGoodsID();
	void CheckContinentRewardBtn(int iContinentID);
	void SetContinentReward(int iContinentID);
	void SetPrintObtainStr(bool bObtinStr_);
	bool GetPossibleObtainStr();
public:
	std::map< int, std::map< DWORD, int > >         m_mapCollectData;              // 전리품 획득 정보(<대륙 ID, < Collect ID, Gather Count >)
	std::map< int, bool >                           m_mapContinentData;            // 대륙 보상 정보(대륙ID, 지급했나)
	std::map< int, KContinentRewardInfo >           m_mapContinentRewardInfo;      // 대륙 보상 정보;  key: m_nContinentID
	KDropItemInfo                                   m_kFinalRewardInfo;           // 최종 보상 정보;
	std::map< int, KAdventureMonster >              m_mapDungeonCollectData;       // 던전 보상 정보 (어떤놈을 죽이면 떨구나)
	std::map < int, bool >                          m_mapContinentRewardCheck;

private:
	int m_iCurrentContinent;
	bool m_bEventPeriod;
	bool m_bShowItemInfo;
	bool m_bObtinStr;
};
DefSingletonInline(GCAdventureManager);
