#include "AgentPacket.h"

SERIALIZE_DEFINE_PUT(KEAG_VERIFY_ACCOUNT_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kServerInfo) &&
		PUT(m_nSID) &&
		PUT(m_bEnterance);
};

SERIALIZE_DEFINE_GET(KEAG_VERIFY_ACCOUNT_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kServerInfo) &&
		GET(m_nSID) &&
		GET(m_bEnterance);
};

SERIALIZE_DEFINE_PUT(KEAG_AGIT_LOADING_COMPLETE_REQ, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kUserInfo);
};

SERIALIZE_DEFINE_GET(KEAG_AGIT_LOADING_COMPLETE_REQ, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kUserInfo);
};

SERIALIZE_DEFINE_PUT(KEAG_ENTER_AGIT_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kUserInfo) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_ENTER_AGIT_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kUserInfo) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_LEAVE_AGIT_REQ, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_dwUserUID);
};

SERIALIZE_DEFINE_GET(KEAG_LEAVE_AGIT_REQ, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_dwUserUID);
};

SERIALIZE_DEFINE_PUT(KEAG_LEAVE_AGIT_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_dwUserUID) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_LEAVE_AGIT_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_dwUserUID) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_AGIT_MAP_CATALOGUE_ACK, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_PayItemID) &&
		PUT(m_mapMapList);
};

SERIALIZE_DEFINE_GET(KEAG_AGIT_MAP_CATALOGUE_ACK, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_PayItemID) &&
		GET(m_mapMapList);
};

SERIALIZE_DEFINE_PUT(KEAG_BUY_AGIT_MAP_REQ, obj, ks)
{
	return  PUT(m_dwUserUID) &&
		PUT(m_dwMapID) &&
		PUT(m_kPayItem) &&
		PUT(m_nPrice);
};

SERIALIZE_DEFINE_GET(KEAG_BUY_AGIT_MAP_REQ, obj, ks)
{
	return  GET(m_dwUserUID) &&
		GET(m_dwMapID) &&
		GET(m_kPayItem) &&
		GET(m_nPrice);
};

SERIALIZE_DEFINE_PUT(KEAG_CHANGE_AGIT_MAP_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_dwMapID) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_CHANGE_AGIT_MAP_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_dwMapID) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_CHANGE_FLOWER_POT_STATE_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kFlowerPotInfo) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_CHANGE_FLOWER_POT_STATE_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kFlowerPotInfo) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_AGIT_CHAT_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kChatData) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_AGIT_CHAT_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kChatData) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_CHANGE_AGIT_OBJECT_POS_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_vecObject) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_CHANGE_AGIT_OBJECT_POS_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_vecObject) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KDB_EAG_REGISTER_AGIT_PAGE_REQ, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_strNickName) &&
		PUT(m_kAgitOption);
};

SERIALIZE_DEFINE_GET(KDB_EAG_REGISTER_AGIT_PAGE_REQ, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_strNickName) &&
		GET(m_kAgitOption);
};

SERIALIZE_DEFINE_PUT(KEAG_CHANGE_BROWNIE_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kBrownieInfo) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_CHANGE_BROWNIE_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kBrownieInfo) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_CHANGE_FAIRY_TREE_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kFairyTree) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_CHANGE_FAIRY_TREE_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kFairyTree) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_UPDATE_FAIRY_TREE_REQ, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kFairyTree);
};

SERIALIZE_DEFINE_GET(KEAG_UPDATE_FAIRY_TREE_REQ, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kFairyTree);
};

SERIALIZE_DEFINE_PUT(KRankAgit, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_nType) &&
		PUT(m_nRank) &&
		PUT(m_dwVisitorCount);
};

SERIALIZE_DEFINE_GET(KRankAgit, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_nType) &&
		GET(m_nRank) &&
		GET(m_dwVisitorCount);
};

SERIALIZE_DEFINE_PUT(KEAG_UPDATE_AGIT_VISITER_COUNT_NOT, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_nTodayVisiterDiff);
};

SERIALIZE_DEFINE_GET(KEAG_UPDATE_AGIT_VISITER_COUNT_NOT, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_nTodayVisiterDiff);
};

SERIALIZE_DEFINE_PUT(KEAG_AGIT_VISITER_COUNT_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_nTodayVisiterCount) &&
		PUT(m_nTotalVisiterCount) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_AGIT_VISITER_COUNT_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_nTodayVisiterCount) &&
		GET(m_nTotalVisiterCount) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_WATERING_FAIRY_TREE_REQ, obj, ks)
{
	return  PUT(m_kPacket) &&
		PUT(m_bFruitChange);
};

SERIALIZE_DEFINE_GET(KEAG_WATERING_FAIRY_TREE_REQ, obj, ks)
{
	return  GET(m_kPacket) &&
		GET(m_bFruitChange);
};

SERIALIZE_DEFINE_PUT(KEAG_CHANGE_AGIT_OPTION_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kOption) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_CHANGE_AGIT_OPTION_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kOption) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_LOAD_AGIT_INFO_NOT, obj, ks)
{
	return  PUT(m_kInfo) &&
		PUT(m_strNick) &&
		PUT(m_mapFlowerPot) &&
		PUT(m_mapFavoritesInfo) &&
		PUT(m_mapBrownie) &&
		PUT(m_kTree) &&
		PUT(m_mapGuestbook) &&
		PUT(m_mapTrainingObj);
};

SERIALIZE_DEFINE_GET(KEAG_LOAD_AGIT_INFO_NOT, obj, ks)
{
	return  GET(m_kInfo) &&
		GET(m_strNick) &&
		GET(m_mapFlowerPot) &&
		GET(m_mapFavoritesInfo) &&
		GET(m_mapBrownie) &&
		GET(m_kTree) &&
		GET(m_mapGuestbook) &&
		GET(m_mapTrainingObj);
};

SERIALIZE_DEFINE_PUT(KEAG_ENTER_AGIT_ACK, obj, ks)
{
	return  PUT(m_kEnterAgitReq) &&
		PUT(m_kAgitLoadInfo);
};

SERIALIZE_DEFINE_GET(KEAG_ENTER_AGIT_ACK, obj, ks)
{
	return  GET(m_kEnterAgitReq) &&
		GET(m_kAgitLoadInfo);
};

SERIALIZE_DEFINE_PUT(KEAG_ENTER_AGIT_STAT_NOT, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_dwUserUID) &&
		PUT(m_dwPlayTime);
};

SERIALIZE_DEFINE_GET(KEAG_ENTER_AGIT_STAT_NOT, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_dwUserUID) &&
		GET(m_dwPlayTime);
};

SERIALIZE_DEFINE_PUT(KEAG_DELETE_FOR_AGIT_NOT, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_vecDeleteObject);
};

SERIALIZE_DEFINE_GET(KEAG_DELETE_FOR_AGIT_NOT, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_vecDeleteObject);
};

SERIALIZE_DEFINE_PUT(KEAG_GUESTBOOK_WRITE_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_nNo) &&
		PUT(m_kGuestMsg) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_GUESTBOOK_WRITE_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_nNo) &&
		GET(m_kGuestMsg) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_GUESTBOOK_DELETE_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_setDeleteNo) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_GUESTBOOK_DELETE_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_setDeleteNo) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_GUESTBOOK_WRITE_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kGuestbookWriteReq) &&
		PUT(m_nNo) &&
		PUT(m_tmDate);
};

SERIALIZE_DEFINE_GET(KEAG_GUESTBOOK_WRITE_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kGuestbookWriteReq) &&
		GET(m_nNo) &&
		GET(m_tmDate);
};

SERIALIZE_DEFINE_PUT(KEAG_GUESTBOOK_DELETE_ACK, obj, ks)
{
	return  PUT(m_nOK) &&
		PUT(m_kGuestBookDeleteReq);
};

SERIALIZE_DEFINE_GET(KEAG_GUESTBOOK_DELETE_ACK, obj, ks)
{
	return  GET(m_nOK) &&
		GET(m_kGuestBookDeleteReq);
};

SERIALIZE_DEFINE_PUT(KEAG_UPDATE_TRAINING_OBJ_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kTrainingObj) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_UPDATE_TRAINING_OBJ_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kTrainingObj) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_USE_TRAINING_OBJ_REQ, obj, ks)
{
	return  PUT(m_kUseTrainingObjReq) &&
		PUT(m_kTrainingObj);
};

SERIALIZE_DEFINE_GET(KEAG_USE_TRAINING_OBJ_REQ, obj, ks)
{
	return  GET(m_kUseTrainingObjReq) &&
		GET(m_kTrainingObj);
};

SERIALIZE_DEFINE_PUT(KEAG_UPDATE_TRAINING_OBJ_REQ, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_mapTrainingObj);
};

SERIALIZE_DEFINE_GET(KEAG_UPDATE_TRAINING_OBJ_REQ, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_mapTrainingObj);
};

SERIALIZE_DEFINE_PUT(KEAG_UPDATE_AGIT_CHARACTER_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kCharInfo) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_UPDATE_AGIT_CHARACTER_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kCharInfo) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KEAG_UPDATE_AGIT_CHARACTER_POS_BROAD, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_kCharPosInfo) &&
		PUT(m_mapServerUser);
};

SERIALIZE_DEFINE_GET(KEAG_UPDATE_AGIT_CHARACTER_POS_BROAD, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_kCharPosInfo) &&
		GET(m_mapServerUser);
};

SERIALIZE_DEFINE_PUT(KUserConnectStatus, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_mapUserConnect);
};

SERIALIZE_DEFINE_GET(KUserConnectStatus, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_mapUserConnect);
};

SERIALIZE_DEFINE_PUT(KEAG_USE_FAIRY_TREE_FRUIT_REQ, obj, ks)
{
	return  PUT(m_dwAgitUID) &&
		PUT(m_nFruitType);
};

SERIALIZE_DEFINE_GET(KEAG_USE_FAIRY_TREE_FRUIT_REQ, obj, ks)
{
	return  GET(m_dwAgitUID) &&
		GET(m_nFruitType);
};