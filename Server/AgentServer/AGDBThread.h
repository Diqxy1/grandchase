#pragma once
#include "SubThread.h"
#include "IntEvent.h"
#include "odbc/Odbc.h"
#include "AgentEvent.h"
#include "AgentPacket.h"

class KAGSimLayer;
class KAGDBThread : public KSubThread< KAgentEvent >
{
	typedef KAGSimLayer SLayerType;
	DeclareDBThread;
	DeclareDBThreadName(Stat);

public:

	KAGDBThread();
	virtual ~KAGDBThread(void);

protected:
	// derived from KThreadManager
	virtual inline void ProcessIntEvent(const KIntEvent& kIntEvent);

	void SetServerListFromDB(IN const int& nProtocol);
	bool GetServerList(IN const int& nProtocol, OUT std::vector<KServerInfo>& vecServerList);
	bool UpdateCountItem(IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_);
	bool UseCountItem(IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_);
	bool AddCountItem(IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_);

	// 아지트
	bool AgitInfo_agittype_update(IN const DWORD& dwUserUID, IN const DWORD& dwMapID);
	bool AgitInfo_visitor_update(IN const DWORD& dwUserUID, IN const int& nDiffCount);
	bool AgitInfo_select(IN const DWORD& dwUserUID, IN OUT KAgitInfo& kInfo);
	bool AgiteObjPos_merge(IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID, IN const int& nPosX, IN const int& nPosY);
	bool AgiteObjPos_select(IN const DWORD& dwUserUID, OUT std::vector<KAgitObjectPos>& vecObjectPos);
	bool AgitInfo_lastcleaning_update(IN const DWORD& dwUserUID);
	bool AgitInfo_option_update(IN const DWORD& dwUserUID, IN const KAgitOption& kInfo);
	// 아지트-민들레
	bool AgiteFlowerpot_init(IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID);
	bool AgiteFlowerpot_seed(IN const DWORD& dwUserUID, IN const GCITEMUID& ItemUID, IN const int& nSeedType, OUT time_t& tmTime);
	bool AgiteFlowerpot_select(IN const DWORD& dwUserUID, OUT std::map<GCITEMUID, KFlowerPot>& mapList);
	// 아지트-월드맵
	bool AgitTownAuction_init(IN const DWORD& dwUserUID, IN const std::wstring& strLogin);
	bool AgitTown_select(OUT std::map<USHORT, std::map<USHORT, DWORD> >& mapList);
	bool AgitTown_update(IN const USHORT& usWorldID, IN const USHORT& usPosID, IN const DWORD& dwAgitUID);
	// 아지트-경매
	bool AgitTownAuction_select(OUT std::map< USHORT, std::map<USHORT, KAuctionInfo> >& mapList);
	bool AgitTownAuction_merge(IN const USHORT& usTownID, IN const USHORT& usPosID, IN const DWORD& dwUserUID, IN const int& nPayPoint);
	// 아지트 즐겨찾기
	bool AgitFavorites_insert(IN const DWORD& dwUserUID, IN const DWORD& dwAgitUID);
	bool AgitFavorites_delete(IN const DWORD& dwUserUID, IN const DWORD& dwAgitUID);
	bool AgitFavorites_select(IN const DWORD& dwUserUID, OUT std::map<DWORD, KFavoritesAgit>& mapList);
	bool AgitRank_select(OUT std::map<int, std::vector<KRankAgit> >& mapList);
	bool GetFavoritesInfoFromDB(IN const DWORD& dwUserUID, OUT std::map<DWORD, KAgitPageInfo>& mapPage);
	// 아지트 도우미
	bool AgitHelper_merge(IN const DWORD& dwUserUID, IN const KBrownieInfo& kInfo);
	bool AgitHelper_select(IN const DWORD& dwUserUID, OUT std::map<GCITEMUID, KBrownieInfo>& mapList);
	bool GetBrownieInfoFromDB(IN const DWORD& dwUserUID);
	// 요정의 나무
	bool AgitFairyTreeFruit_merge(IN const DWORD& dwUserUID, IN const int& nFruitType, IN const time_t& tmUpdate);
	bool AgitFairyTreeFruit_select(IN const DWORD& dwUserUID, OUT time_t& tmDate, OUT int& nFruitType);
	bool AgitFairyTree_merge(IN const DWORD& dwUserUID, IN const KFairyTree& kInfo);
	bool AgitFairyTree_select(IN const DWORD& dwUserUID, OUT KFairyTree& kInfo);
	bool GetFairyTreeInfoFromDB(IN const DWORD& dwUserUID, OUT KFairyTree& kInfo);
	int GetTodayUseFruit(IN const DWORD& dwUserUID);
	bool SetTodayUseFruit(IN const DWORD& dwUserUID);
	// 캐릭터 배치
	bool AgitCharInfo_insert(IN const DWORD& dwAgitUID, IN const int& nCharType);
	bool AgitCharInfo_pos_update(IN const DWORD& dwAgitUID, IN const int& nCharType, IN const int& nPosX, IN const int& nPosY);
	bool AgitCharInfo_dialogic_update(IN const DWORD& dwAgitUID, IN const int& nCharType, IN const std::wstring& strNormalDialogic, IN const std::wstring& strNearDialogic, IN const std::wstring& strClickDialogic);
	bool ASGAAgitCharInfo_motion_update(IN const DWORD& dwAgitUID, IN const int& nCharType, IN const int& nMotionID);
	bool AgiteCharInfo_select(IN const DWORD& dwAgitUID, OUT std::map<int, KAgitCharacterPos>& mapList);
	int AgiteCharCoordi_merge(IN const DWORD& dwAgitUID, IN const int& nCharType, IN const DWORD& dwSlotID, IN const GCITEMUID& ItemUID, IN const bool& bEquip);
	bool AgiteCharCoordi_select(IN const DWORD& dwAgitUID, IN OUT std::map<int, KAgitCharacterPos>& mapList);
	// 낙서장
	bool AgitGuestBook_insert(IN const DWORD& dwAgitUID, IN const DWORD& dwUserUID, IN const std::wstring& strMsg, OUT int& nNo);
	bool AgitGuestBook_delete(IN const DWORD& dwAgitUID, IN const DWORD& dwUserUID, IN const int& nNo);
	bool AgitGuestBook_select(IN const DWORD& dwAgitUID, OUT std::map<int, KGuestMsg>& mapList);
	bool AgitTrainingOBJ_merge(IN const DWORD& dwAgitUID, IN const GCITEMUID& ItemUID, IN const int& nDiff, IN const time_t& tmUpdate);
	bool AgitTrainingOBJ_select(IN const DWORD& dwAgitUID, OUT std::map<GCITEMUID, KTrainingObj>& mapList);

	// 기타
	bool GetAgitInfoFromDB(IN const DWORD& dwUserUID, OUT KAgitInfo& kInfo, OUT std::wstring& strNick);
	bool AgitSystemCollect_insert(IN const DWORD& dwAgitUID, IN const DWORD& dwEnterUserUID, IN const DWORD& dwTimeSec); // 통계
	bool LoginUIDtoNickname_select(IN const DWORD& dwUserUID_, OUT std::wstring& strNick);
	bool AgitCharDesignCoordi_select(IN const DWORD& dwUserUID, IN OUT std::map<int, KAgitCharacterPos>& mapCharacterPos);

protected:
	_INT_DECL_ON_FUNC(DB_EVENT_FIRST_INIT_REQ, int);
	_INT_DECL_ON_FUNC(DB_EVENT_SERVER_LIST_REQ, int);

	INT_DECL_ON_FUNC(EAG_BUY_AGIT_MAP_REQ);
	_INT_DECL_ON_FUNC(EAG_SET_AGIT_OPTION_REQ, KAgitOption);
	_INT_DECL_ON_FUNC(EAG_SEED_FLOWER_POT_REQ, KSeedFlowerPotReq);
	_INT_DECL_ON_FUNC(EAG_HARVEST_FLOWER_POT_REQ, KHarvestFlowerPotReq);
	_INT_DECL_ON_FUNC(EAG_SET_AGIT_OBJECT_POS_REQ, KSetAgitObjectPosReq);
	_INT_DECL_ON_FUNC(EAG_AGIT_FRIEND_LIST_REQ, KAgitFriendListReq);
	_INT_DECL_ON_FUNC(EAG_AGIT_ADD_FAVORITE_REQ, KAgitUserParam);
	_INT_DECL_ON_FUNC(EAG_AGIT_REMOVE_FAVORITE_REQ, KAgitUserParam);
	_INT_DECL_ON_FUNC(EAG_REGISTER_AGIT_PAGE_REQ, KDB_EAG_REGISTER_AGIT_PAGE_REQ);
	_INT_DECL_ON_FUNC(EAG_EQUIP_BROWNIE_REQ, KEquipBrownieReq);
	INT_DECL_ON_FUNC(EAG_WATERING_FAIRY_TREE_REQ);
	INT_DECL_ON_FUNC(EAG_USE_FAIRY_TREE_FRUIT_REQ);
	INT_DECL_ON_FUNC(EAG_UPDATE_FAIRY_TREE_REQ);
	INT_DECL_ON_FUNC_NOPARAM(EAG_AGIT_RANK_LIST_NOT);
	INT_DECL_ON_FUNC(EAG_UPDATE_AGIT_VISITER_COUNT_NOT);
	INT_DECL_ON_FUNC(EAG_ENTER_AGIT_STAT_NOT);
	_INT_DECL_ON_FUNC(EAG_LOAD_AGIT_INFO_NOT, DWORD);
	_INT_DECL_ON_FUNC(EAG_ENTER_AGIT_REQ, KEnterAgitReq);
	_INT_DECL_ON_FUNC(EAG_GUESTBOOK_WRITE_REQ, KGuestBookWriteReq);
	_INT_DECL_ON_FUNC(EAG_GUESTBOOK_DELETE_REQ, KGuestBookDeleteReq);
	INT_DECL_ON_FUNC(EAG_UPDATE_TRAINING_OBJ_REQ);
	_INT_DECL_ON_FUNC(EAG_OPEN_AGIT_CHARACTER_REQ, PAIR_DWORD_INT);
	_INT_DECL_ON_FUNC(EAG_SET_AGIT_CHARACTER_POS_REQ, KSetAgitCharPosReq);
	_INT_DECL_ON_FUNC(EAG_SET_AGIT_CHARACTER_MOTION_REQ, KSetAgitCharMotionReq);
	_INT_DECL_ON_FUNC(EAG_SET_AGIT_CHARACTER_COORDI_REQ, KSetAgitCharCoordiReq);
	_INT_DECL_ON_FUNC(EAG_SET_AGIT_CHARACTER_SPEECH_REQ, KSetAgitCharSpeechReq);
};
