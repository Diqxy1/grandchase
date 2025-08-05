#pragma once
#include <RTTI.h>
#include <ToString.h>
#include "KncSmartPtr.h"
#include <KNCSingleton.h>
#include "AgentPacket.h"
#include "Agit.h"
#include "IntEvent.h"
#include <queue>
#include "KncTimer.h"
#include "AGSimLayer.h"
#include "Lottery.h"

SmartPointer(KAgit);

class KLuaManager;
class KAgitManager
{
	DeclareSingleton(KAgitManager);
	NiDeclareRootRTTI(KAgitManager);
	DeclToStringW;

public:
	KAgitManager(void);
	~KAgitManager(void);

	void UpdateAgitRankFromDB();

	bool LoadScript();
	bool LoadConfig();
	bool LoadMapInfo();
	bool LoadFlowerPot();
	bool LoadFairyTree();

	KAgitPtr GetAgit(IN const DWORD& dwAgitUID);
	void EraseAgit(IN const DWORD& dwAgitUID);
	void Tick();

	bool Add(IN const KAgitInfo& kInfo);

	bool GetCurrentMapInfo(IN const DWORD& dwMapID, OUT KAgitMap& kInfo);
	bool GetFlowerPotOption(IN const int& nSeedType, OUT KFlowerPotOption& kOption);
	void GetNormalPotItemID(OUT std::set<GCITEMID>& setList) { KLocker lock(m_csFlowerPot); setList = m_kFlowerPotConfig.m_setNormalPotItemID; }
	bool IsNormalPotItemID(IN const GCITEMID& PotItemID);
	bool IsCashPotItemID(IN const GCITEMID& PotItemID);
	USHORT GetMaxAgitCashPotCount() { KLocker lock(m_csFlowerPot); return m_kFlowerPotConfig.m_usMaxAgitCashPotCount; }
	GCITEMID GetFlowerItemID() { KLocker lock(m_csFlowerPot); return m_kFlowerPotConfig.m_FlowerItemID; }
	int GetInvenLimitFlowerCount() { KLocker lock(m_csFlowerPot); return m_kFlowerPotConfig.m_nInvenLimitFlowerCount; }

	int GetMaxCashingAgitInfo() { KLocker lock(m_csConfig); return m_kAgitConfig.m_nMaxCashingAgitInfo; }
	int GetMaxFavoriteCount() { KLocker lock(m_csConfig); return m_kAgitConfig.m_nMaxFavoriteCount; }
	USHORT GetReserveRoomID() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usReserveRoomID; }
	DWORD GetBuildPageTick() { KLocker lock(m_csConfig); return m_kAgitConfig.m_dwBuildPageTick; }
	USHORT GetAgitCountOnePage() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usAgitCountOnePage; }
	void GetAgitConfig(OUT KAgitConfig& kInfo) { KLocker lock(m_csConfig); kInfo = m_kAgitConfig; }
	USHORT GetGuestBookPageSize() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usGuestBookPageSize; }
	USHORT GetGuestbookWritePerDay() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usGuestbookWritePerDay; }
	GCITEMID TrainingItemID() { KLocker lock(m_csConfig); return m_kAgitConfig.m_TrainingItemID; }
	USHORT GetTrainingMaterialRegenTerm() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usTrainingMaterialRegenTerm; }
	USHORT GetTrainingMaterialLimit() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usTrainingMaterialLimit; }

	void GetMapInfo(OUT KAgitMapInfo& kInfo) { KLocker lock(m_csMap); kInfo = m_mapMapInfo; }
	void GetFlowerPotConfig(OUT KFlowerPotConfig& kInfo) { KLocker lock(m_csFlowerPot); kInfo = m_kFlowerPotConfig; }
	void GetFairyTreeConfig(OUT KFairyTreeConfig& kConfig) { KLocker lock(m_csFairyTree); kConfig = m_kFairyTreeConfig; }
	int GetFairyTreeLv(IN std::vector<PAIR_INT_DWORD>& vecLvExp, IN const DWORD& dwTotalExp);
	int GetTodayFruitType(IN const int& nFairyTreeLv);
	float GetFruitBuffRate(IN const int& nFairyTreeLv, IN const int& nFruitType_);
	bool FairyTreeGrowChange(IN const time_t& tmToday, IN OUT KFairyTree& kFairyTree);

	void SetPageChange(IN const bool& bValue) { KLocker lock(m_csPage); m_bPageChange = bValue; }
	bool IsPageChange() { KLocker lock(m_csPage); return m_bPageChange; }
	bool RegisterPage(IN const DWORD& dwAgitUID);
	bool UnregisterPage(IN const USHORT& usRoomID, IN const DWORD& dwAgitUID);
	void GetPageBuffer(IN OUT KAgitPageList& kPageList);
	void GetCustomPage(IN OUT KAgitPageList& kPageList);

	void SetRankAgit(IN const std::map<int, KSerBuffer>& mapPage, IN const std::map<int, std::vector<KRankAgit> >& mapRankList);
	void GetRankAgit(OUT std::map<int, KSerBuffer>& mapPage);
	bool IsRankAgit(IN const DWORD& dwAgitUID);
	USHORT GetRankDBUpdateHour() { KLocker lock(m_csRankAgit); return m_usDBUpdateHour; }

	void UpdateCashingData();
	USHORT GetCashingSize() { KLocker lock(m_csAgit); return m_mapAgit.size(); }

	void DumpAgit(IN const DWORD& dwAgitUID);
	void TestFunc_CreateAgit(IN const int& nCount);
	void TestFunc_BuildDummyPage(IN const int& nCount, OUT std::map<int, KSerBuffer>& mapPage);

	void BuildPageBufferPtr(IN std::vector<KAgitPtr> vecAgitPtr, OUT std::map<int, KSerBuffer>& mapPage);
	void BuildPageBuffer(IN std::map<DWORD, KAgitPageInfo> mapInfo, OUT std::map<int, KSerBuffer>& mapPage);
	void BuildOnePageBuffer(IN std::vector<KAgitPageInfo> vecInfo, IN OUT std::map<int, KSerBuffer>& mapPageBuff);

	bool IsAgitAvailable() { KLocker lock(m_csAgit); return m_bAgitAvailable; }
	void SetAgitAvailable(IN const bool& bVal) { KLocker lock(m_csAgit); m_bAgitAvailable = bVal; }

	void SetDestroyAgit(IN const bool& bVal) { KLocker lock(m_csAgit); m_bDestroyAgit = bVal; }
	bool IsDestroyAgit() { KLocker lock(m_csAgit); return m_bDestroyAgit; }

	void BuildGuestBookBuffer(IN const std::map<int, KGuestMsg>& mapList, OUT KSerBuffer& kBuff);
	bool IsWriteEnable(IN const DWORD& dwUserUID, IN const std::map<int, KGuestMsg>& mapList);

	template<typename T>
	void SendAgitPacket(unsigned short usEventID, T& Data);

	template<typename T>
	void QueueingEvent(unsigned short usEventID, DWORD dwUID, T& data);
	void QueueingEvent(KIntEventPtr spEvent) { KLocker lock(m_csEventQueue); m_queEvent.push_back(spEvent); }

private:
	void DistributeEvent();
	void EachTick();
	void IntervalTick();

	void BuildPage();
	void BuildPageByOption();
	bool CompressPage(IN std::vector<KAgitPageInfo> vecPage, OUT KSerBuffer& kBuff);
	bool GetCurrentPage(IN std::map<int, KSerBuffer>& mapList, IN OUT KAgitPageList& kPageList);
	void BuildOnePageBufferPtr(IN std::vector<KAgitPtr> vecAgitPtr, IN OUT KAgitPageList& kPageList);
	void BuildCutomPage(IN std::vector<KAgitPtr> vecAgitPtr, IN OUT KAgitPageList& kPageList);

	void InitRoomID();
	bool ReserveRoomID(IN const USHORT& usSize);
	size_t GetAgitRoomIdSize() { KLocker lock(m_csPage); return m_vecAgitRoomID.size(); }
	USHORT FindEmptyRoomID();

private:
	mutable KncCriticalSection      m_csAgit;
	std::map<DWORD, KAgitPtr>        m_mapAgit; // map[AgitUID,AgitPtr]
	DWORD                           m_dwShutdownTimeLimit; // 서버종료 대기시간
	bool                            m_bAgitAvailable; // 아지트정보 가용가능한지.
	bool                            m_bDestroyAgit; // 아지트정보 삭제할지 여부(AgentMaster에서 제어) True면 삭제, false면 삭제안함

	mutable KncCriticalSection      m_csEventQueue;
	std::deque<KIntEventPtr>        m_queEvent;

protected:
	KncTimer                        m_kPageTimer; // 페이지 빌드 타이머
	KncTimer                        m_kMinTimer;
	CTime                           m_tmToday;

	mutable KncCriticalSection      m_csConfig;
	KAgitConfig                     m_kAgitConfig;

	mutable KncCriticalSection      m_csMap;
	std::map<DWORD, KAgitMap>        m_mapMapInfo; // map[MapID,MapInfo]

	mutable KncCriticalSection      m_csFlowerPot;
	KFlowerPotConfig                m_kFlowerPotConfig; // 화분

	mutable KncCriticalSection      m_csFairyTree;
	KFairyTreeConfig                m_kFairyTreeConfig; // 요정의 나무
	std::map<int, KLottery>          m_mapFruitDecision; // 나무레벨별 열매등장확률 map[Lv,확률]

	mutable KncCriticalSection      m_csPage;
	bool                            m_bPageChange; // 아지트 페이지 구성 바뀌었는지 여부
	int                             m_nLastReservedRoomID; // 마지막으로 할당된 방번호
	std::vector<PAIR_INT_DWORD>     m_vecAgitRoomID; // 방번호별 아지트UID vector[ pair[RoomID,AgitUID] ]
	std::vector<KAgitPtr>           m_vecPageAll; // 아지트 페이지 전체 리스트
	std::vector<KAgitPtr>           m_vecPageOpen; // 오픈설정만
	std::vector<KAgitPtr>           m_vecPageNotFull; // 꽉차지않은방만
	std::vector<KAgitPtr>           m_vecPageOpenNotFull; // 오픈+꽉차지않은방

	mutable KncCriticalSection      m_csPageSort;
	std::map<int, KSerBuffer>        m_mapPageAll; // 아지트 전체 페이지 map[페이지번호, vector[KAgitPageInfo] ]
	std::map<int, KSerBuffer>        m_mapPageSortByOpen; // 아지트 전체공개 페이지 map[페이지번호, vector[KAgitPageInfo] ]
	std::map<int, KSerBuffer>        m_mapPageSortByNotFull; // 아지트 입장가능 페이지 map[페이지번호, vector[KAgitPageInfo] ]
	std::map<int, KSerBuffer>        m_mapPageSortByOpenNotFull; // 아지트 전체공개&입장가능 페이지 map[페이지번호, vector[KAgitPageInfo] ]

	mutable KncCriticalSection      m_csRankAgit;
	std::map<int, KSerBuffer>        m_mapRankPage; // 클라이언트 전달용 인기아지트 목록 map[페이지번호,vector[KAgitPageInfo]]
	std::map<int, std::vector<KRankAgit> > m_mapRankList; // 인기아지트 리스트 map[랭킹타입, vector[랭킹정보] ]
	USHORT                          m_usDBUpdateHour; // DB에서 정보 퍼오는 시간(시)
	CTime                           m_tmLastCheck; // 마지막으로 체크한 날짜.
};

DefSingletonInline(KAgitManager);
DeclOstmOperatorA(KAgitManager);

template<typename T>
void KAgitManager::SendAgitPacket(unsigned short usEventID_, T& Data_)
{
	SiKAGSimLayer()->m_kActorManager.SendToAll(usEventID_, Data_, true);
	START_LOG(clog, L"Send AgitPacket.. EventID : " << usEventID_) << END_LOG;
}

template< typename T >
void KAgitManager::QueueingEvent(unsigned short usEventID_, DWORD dwUID_, T& data_)
{
	KIntEventPtr spEvent(new KIntEvent);

	spEvent->m_usEventID = usEventID_;
	spEvent->m_dwSenderUID = dwUID_;

	KSerializer ks;
	ks.BeginWriting(&spEvent->m_kbuff);
	ks.Put(data_);
	ks.EndWriting();

	QueueingEvent(spEvent);
}