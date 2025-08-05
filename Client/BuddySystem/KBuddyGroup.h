#pragma once

//#include <map>
#include <WTypes.h>
//#include <string>
#include "KBuddy.h"
#include "../gcui/GCBuddyTree.h"
#include "../gcui/GCFriendListTree.h"

struct KGroup;

class KBuddyGroup
{
public:
	KBuddyGroup(void);
	~KBuddyGroup(void);

	// typedefs
	typedef std::map< DWORD, KBuddy >::iterator iterBuddy;

	// getter/setter
	int GetGroupID() const { return m_nGroupKey; }
	void SetGroupID(int iID) { m_nGroupKey = iID; }
	bool GetBuddy(DWORD dwUID, KBuddy& kBuddy);
	bool GetBuddy(const std::wstring wstrNickName, KBuddy& kBuddy);
	void SetGroupName(const std::wstring& strGroupName)
	{
		m_strGroupName = strGroupName;
	}
	void GetFriendShipData(std::vector< FRIEND_UI_DATA >& vecRet);
	void GetFriendListData(std::vector< KFriendListData >& vecRet);
	bool GetOpenStatus() const { return m_bOpenStatus; }
	void SetOpenStatus(bool bOpen) { m_bOpenStatus = bOpen; }
	const std::wstring& GetGroupName() const { return m_strGroupName; }
	bool GetBuddyByNickName(const std::wstring& strNickName, KBuddy& kBuddy_);

	// packet processing
	void SetData(const KGroup& kPacket);
	bool AddBuddy(const KFriend& kInfo);
	void AddBuddy(const KBuddy& kInfo);
	bool DeleteBuddy(DWORD dwBuddyUID);
	void UpdateBuddy(const std::map<DWORD, KFriend>& kData);
	void UpdateBuddy(const KFriend& kData);

	// functions
	int GetOnlineBuddyCount();
	int GetTotalBuddyCount();
	bool FindBuddy(DWORD dwUID);

	void InitData(void);

	bool IsInNickName(const std::wstring& strNickName_);
private:
	int m_nGroupKey;
	std::map< DWORD, KBuddy > m_mapFriend;
	std::wstring m_strGroupName;
	bool m_bOpenStatus;
};
