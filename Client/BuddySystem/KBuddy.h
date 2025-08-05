#pragma once

//#include <string>
#include <MSPacket.h>
#include "../gcui/GCBuddyTree.h"

class KBuddy
{
public:
	KBuddy(void);
	~KBuddy(void);

	// getter/setter
	void GetData(KFriend& kData) const { kData = m_kData; }
	KFriend& GetData() { return m_kData; }
	void GetUIData(OUT FRIEND_UI_DATA& kData);
	const std::wstring& GetNickName() const { return m_kData.m_strNickName; }
	const std::wstring& GetNickColor() const { return m_kData.m_strNickColor; }
	const std::wstring& GetLocation() const { return m_kData.m_strLocation; }
	const std::wstring& GetServerName() const { return m_kData.m_strServerName; }
	DWORD GetUID() const { return m_kData.m_dwUserUID; }

	// packet processing
	void SetData(const KFriend& kPacket);

	// functions
	bool GetOnline() const { return m_kData.m_bIsOnline; }
	bool IsFriend() const { return m_kData.m_bIsFriend; }

private:
	KFriend m_kData;
};
