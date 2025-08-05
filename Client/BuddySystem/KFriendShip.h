#pragma once

//#include <map>
////#include <vector>
#include <KNCSingleton.h>
#include <MSPacket.h>
#include "KBuddyGroup.h"
#include "../gcui/GCBuddyTree.h"
#include "../gcui/GCFriendListTree.h"
#include "../gcui/GCNateOn.h"

#define MAX_GROUP_NUM 10

class KFriendShip
{
	DeclareSingleton(KFriendShip);

public:
	KFriendShip(void);
	~KFriendShip(void);

	// typedefs
	typedef int FS_GROUPID;
	typedef std::map< FS_GROUPID, KBuddyGroup >::iterator iterBuddyGroup;
	const FS_GROUPID DEFGROUPID;
	typedef std::vector< KEMS_S2_ADD_FRIEND_NOT >::iterator iterFriendReq;
	typedef std::vector< FRIEND_UI_DATA > VEC_FRIEND_UI_DATA;

	// packet processing
	void SetData(const KEMS_S2_VERIFY_ACCOUNT_ACK& kPacket);
	void AddBuddy(const KEMS_S2_ADD_FRIEND_ACK& kPacket);
	void AddBuddy(const KFriend& kFriendInfo);
	void RegisterFriendRequest(const KEMS_S2_ADD_FRIEND_NOT& kPacket);
	void DeleteBuddy(const KEMS_S2_DELETE_FRIEND_ACK& kPacket);
	void RejectMyRequest(DWORD dwRejectUID);
	void UpdateBuddy(const KEMS_S2_UPDATE_FRIENDS_NOT& kPacket);
	void UpdateBuddy(const KFriend& kPacket);
	void BlockBuddy(const KEMS_S2_BLOCK_MOVE_FRIEND_ACK& kPacket);
	void BlockBuddy(const KEMS_S2_BLOCK_FRIEND_ACK& kPacket);
	bool UnBlockBuddy(const DWORD dwUID);
	void UnBlockBuddy(const KEMS_S2_UNBLOCK_MOVE_FRIEND_ACK& kPacket);
	void UnBlockBuddy(const KEMS_S2_UNBLOCK_FRIEND_ACK& kPacket);
	void MoveBuddy_BlockToFriend(const KEMS_S2_UNBLOCK_MOVE_FRIEND_ACK& kPacket);
	void AddBuddyGroup(const KEMS_S2_ADD_GROUP_ACK& kPacket);
	void DeleteBuddyGroup(const KEMS_S2_DELETE_GROUP_ACK& kPacket);
	void RenameBuddyGroup(const KEMS_S2_RENAME_GROUP_ACK& kPacket);
	void MoveBuddy(const KEMS_S2_MOVE_FRIEND_TO_GROUP_ACK& kPacket);
	void SetMyEmoticon(const KEMS_S2_SET_EMOTICON_ACK& kPacket);

	// functions
	void UpdateFriendShipData();
	void GetFriendShipData(GCNateOn::E_NATEON_MODE eMode, IN int iTopIdx, OUT std::vector< FRIEND_UI_DATA >& vecRet, const int iChildSize = MAX_CHILD_NUM, bool bExcludeHoney = true);
	void ToggleGroupOpen(FS_GROUPID iId);
	int GetDataUserCount(GCNateOn::E_NATEON_MODE eMode, bool bOnline = false);
	int GetDataCount(GCNateOn::E_NATEON_MODE eMode);
	void AddBuddyGroupReq(std::wstring strGroupName);
	int GetFriendGroupCount() { return (int)m_mapBuddyGroup.size(); }
	void GetFriendGroupVector(OUT std::vector< KBuddyGroup >& vecRet);
	void RenameGroupReq(int nGroupID, std::wstring strGroupName);
	void GetGroupName(IN int nGroupID, OUT std::wstring& strGroupName);
	void DeleteGroupReq(int nGroupID);
	void MoveBuddyReq(int nFrom, int nTo, UINT dwUID);
	void BlockMyFriendReq(int nGroupID, DWORD dwUID);
	void UnBlockMyFriendReq(DWORD dwUID);
	void DeleteBuddyReq(int nGroupID, DWORD dwUID);
	void BlockNoFriendReq(std::wstring strNickName);
	bool FindUserInfo(const DWORD dwUserUID, FRIEND_UI_DATA& FUData);
	bool GetFriendBuddyByNickName(const std::wstring strNickName, KBuddy& FData);
	bool GetFriendBuddyByUID(const DWORD dwUserUID, KBuddy& FData);
	bool GetBlockBuddy(const DWORD dwUserUID, KBuddy& FData);
	bool GetBlockBuddy(const std::wstring wstrNickName, KBuddy& FData);
	bool GetGuildBuddy(const DWORD dwUserUID, KBuddy& FData);
	int GetFriendBuddyGroupID(const DWORD dwUserUID);
	void GetFriendListData(OUT std::vector< KFriendListData >& vecRet);

	void CheckAlramAddFriend(void);
	KEMS_S2_ADD_FRIEND_NOT FirstPopFriendReq(void);

	// getter/setter
	DWORD GetEmoticon() const { return m_dwEmoticon; }
	bool SetEmoticon(DWORD dwEmoticon);

	void ClearFriendShipData();
	bool IsEmptyFriendShipData();

	// 090413. serastia 잠와..
	void SetRecommender(KRecommenderInfo kMyInfo, std::vector< KRecommenderInfo> vecRecommenders);
	void SetRecommender(std::vector< KRecommenderInfo> vecRecommenders);
	bool FindUIDInContactFriendList(DWORD dwUID_);
	void MakeContactFriendList();
	const std::vector<DWORD>& GetContactFriendList() { return m_vecContactFriendList; }
	int GetMyRecommendState();
	int GetAttendedRecommender() { return m_iAttendedRecommender; }

	void FindAndMakeFriend(VEC_FRIEND_UI_DATA& vecFriendData, VEC_FRIEND_UI_DATA& vecBlockData, KRecommenderInfo kFriendInfo);

	void GetOnlienFriendList(GCNateOn::E_NATEON_MODE _eMode, OUT std::vector< DWORD>& _vecOnlineFirendList);
	void GetOffLineFriendList(GCNateOn::E_NATEON_MODE _eMode, OUT std::vector< DWORD>& _vecFirendList);

	int GetOnlineGuildMember();
	int GetTotalGuildMember();

	void ClearGuildMemberData();
	void ModifyGuildMemberData(std::map<int, KGroup>& mapFriendGroup_, int iGroupType_, bool bAdd_);
	void SetAllGuildMemberData(const KGroup& kPacket);
	bool IsOpenAddFriendNot();
	bool IsBlockFriend(const std::wstring& strNickName_);

private:
	std::map< FS_GROUPID, KBuddyGroup > m_mapBuddyGroup;
	KBuddyGroup m_kBlockGroup;
	KBuddyGroup m_kGuildGroup;
	bool m_bNewPaperAlarm;
	DWORD m_dwEmoticon;
	std::vector< KEMS_S2_ADD_FRIEND_NOT > m_vecFriendReq;
	std::map< GCNateOn::E_NATEON_MODE, VEC_FRIEND_UI_DATA > m_mapFriendShipData;

	KRecommenderInfo m_myInfo;
	int m_iAttendedRecommender;
	//         1 ; 아직 추천인 없음
	//         2 ; 추천 하지 않음
	//         3 ; 추천 승인 대기중
	//         4 ; 추천 거절됨
	//         5 ; 추천 승인됨
	//         6 ; 첫출석 완료

		//귀찬타
public:
	std::vector< KRecommenderInfo > m_vecRecommenders;
	std::vector< KRecommenderInfo > m_vecWaitRecommenders;
	//         1 ; 추천 승인 대기중
	//         2 ; 추천 거절됨 - 이건 전송 하지 않음..
	//         3 ; 추천 승인됨
	//         4 ; 첫출석 완료 - 아이템을 받을 수 있는 상태
	//         5 ; 아이템을 받

	std::vector< DWORD> m_vecContactFriendList; //연계된 친구 리스트

	enum MY_RECOMMEND_STATE
	{
		MRS_UNKNOWN = 0, // 알 수 없는 상태
		MRS_NOT_YET = 1, // 아직 추천인 없음
		MRS_DONT_RECOM = 2, // 추천하지않음
		MRS_WAIT_ACCEPT = 3, // 추천 승인 대기중
		MRS_REFUSED = 4, // 추천 거절됨
		MRS_ACCEPTED = 5, // 추천 승인됨
		MRS_ATTENDANCE_FINISH = 6, // 첫출석 완료
	};

	enum OTHER_RECOMMENDED_STATE
	{
		ORS_WAIT_ACCEPT = 1, // 추천 승인 대기중
		ORS_REFUSED = 2, // 추천 거절됨 - 이건 전송 하지 않음.
		ORS_ACCEPTED = 3, // 추천 승인됨
		ORS_ATTENDANCE_FINISH = 4, // 첫출석 완료 - 아이템을 받을 수 있는 상태
		ORS_ITEM_RECEIVED = 5, //아이템을 받은 상태
	};
};

DefSingletonInline(KFriendShip);

class SortByContactFriend {
public:
	SortByContactFriend() {}
	~SortByContactFriend() {}

	bool operator() (const FRIEND_UI_DATA& left_, const FRIEND_UI_DATA& right_)
	{
		return left_.m_bOnline > right_.m_bOnline;
	}
};

class SortByAscending {
public:
	SortByAscending() {}
	~SortByAscending() {}

	bool operator() (const FRIEND_UI_DATA& left_, const FRIEND_UI_DATA& right_)
	{
		KBuddy kLeftBuddy, kRightBuddy;

		if (SiKFriendShip()->GetGuildBuddy(left_.m_dwUserUID, kLeftBuddy) &&
			SiKFriendShip()->GetGuildBuddy(right_.m_dwUserUID, kRightBuddy))
		{
			if (kLeftBuddy.GetData().m_nGuildUserLevel == kRightBuddy.GetData().m_nGuildUserLevel)
				return left_.m_dwUserUID < right_.m_dwUserUID;
			else
				return kLeftBuddy.GetData().m_nGuildUserLevel > kRightBuddy.GetData().m_nGuildUserLevel;
		}
		else
			return left_.m_dwUserUID < right_.m_dwUserUID;
	}
};

class SortByContactFriend_KFriendListData {
public:
	SortByContactFriend_KFriendListData() {}
	~SortByContactFriend_KFriendListData() {}

	bool operator() (const KFriendListData& left_, const KFriendListData& right_)
	{
		return left_.m_bOnline > right_.m_bOnline;
	}
};