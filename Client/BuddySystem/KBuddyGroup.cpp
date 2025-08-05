#include "stdafx.h"
#include "StdAfx.h"
#include ".\kbuddygroup.h"

#include "GCChatManager.h"
#include "KFriendShip.h"

#include "../GCUI/GCInviteRequestBox.h"
//
//

KBuddyGroup::KBuddyGroup(void)
	: m_nGroupKey(-1)
	, m_strGroupName(L"")
	, m_bOpenStatus(true)
{
	m_mapFriend.clear();
}

KBuddyGroup::~KBuddyGroup(void)
{
}

void KBuddyGroup::InitData(void)
{
	m_nGroupKey = -1;
	m_strGroupName = L"";
	m_bOpenStatus = true;
	m_mapFriend.clear();
}

void KBuddyGroup::SetData(const KGroup& kPacket)
{
	m_nGroupKey = kPacket.m_nGroupKey;
	m_strGroupName = kPacket.m_strGroupName;

	std::map<DWORD, KFriend>::const_iterator mit;
	for (mit = kPacket.m_mapMembers.begin(); mit != kPacket.m_mapMembers.end(); ++mit)
	{
		m_mapFriend[mit->first].SetData(mit->second);
	}
}

bool KBuddyGroup::AddBuddy(const KFriend& kInfo)
{
	iterBuddy mit = m_mapFriend.find(kInfo.m_dwUserUID);
	if (mit != m_mapFriend.end())
	{
		// 이미 친구인데? 업데이트는 해준다
		KBuddy& kBuddy = mit->second;
		kBuddy.SetData(kInfo);
		return false;
	}
	m_mapFriend[kInfo.m_dwUserUID].SetData(kInfo);
	return true;
}

void KBuddyGroup::AddBuddy(const KBuddy& kInfo)
{
	KFriend kData;
	kInfo.GetData(kData);
	AddBuddy(kData);
}

bool KBuddyGroup::DeleteBuddy(DWORD dwBuddyUID)
{
	iterBuddy mit = m_mapFriend.find(dwBuddyUID);
	if (mit == m_mapFriend.end())
	{
		// 없는 친구
		return false;
	}
	m_mapFriend.erase(mit);
	return true;
}

void KBuddyGroup::UpdateBuddy(const std::map<DWORD, KFriend>& kData)
{
	std::map<DWORD, KFriend>::const_iterator mit;
	for (mit = kData.begin(); mit != kData.end(); ++mit)
	{
		iterBuddy mitBuddy = m_mapFriend.find(mit->first);
		if (mitBuddy != m_mapFriend.end())
		{
			mitBuddy->second.SetData(mit->second);
		}
		else
		{
			// 없는 친구가 생겼다?
			assert(!"Cannot found friend new friend create?");
		}
	}
}

void KBuddyGroup::UpdateBuddy(const KFriend& kData)
{
	iterBuddy mit = m_mapFriend.find(kData.m_dwUserUID);
	if (mit == m_mapFriend.end())
	{
		return;
	}

	if (mit->second.GetOnline() != kData.m_bIsOnline)
	{
		std::wstring strMsg = L"";
		std::wstring strColor = L"FFFFFF";

		if (kData.m_strNickColor != strColor && !kData.m_strNickColor.size() < 6)
			strColor = kData.m_strNickColor;

		if (kData.m_bIsOnline)
			strMsg = g_pkStrLoader->GetReplacedString(STR_ID_MESSENGER_FRIEND_LOGIN, "l", L"#c" + strColor + kData.m_strNickName + L"#cX"); // [**]님이 로그인 하셨습니다.
		else
			strMsg = g_pkStrLoader->GetReplacedString(STR_ID_MESSENGER_FRIEND_LOGOUT, "l", L"#c" + strColor + kData.m_strNickName + L"#cX"); // [**]님이 로그아웃 하셨습니다.

		g_pkChatManager->AddChatMsg(strMsg, KGCChatManager::CHAT_TYPE_AUTO, 0xFFFFFFFF, false);
	}

	mit->second.SetData(kData);
}

bool KBuddyGroup::GetBuddy(DWORD dwUID, KBuddy& kBuddy)
{
	iterBuddy mit = m_mapFriend.find(dwUID);
	if (mit == m_mapFriend.end())
	{
		return false;
	}
	kBuddy = mit->second;
	return true;
}

bool KBuddyGroup::GetBuddy(const std::wstring wstrNickName, KBuddy& kBuddy)
{
	iterBuddy mit;
	for (mit = m_mapFriend.begin(); mit != m_mapFriend.end(); ++mit)
	{
		if (wstrNickName == mit->second.GetNickName())
		{
			kBuddy = mit->second;
			return true;
		}
	}

	return false;
}

void KBuddyGroup::GetFriendShipData(OUT std::vector< FRIEND_UI_DATA >& vecRet)
{
	if (m_nGroupKey == -1)
	{
		// 유효하지 않은 그룹
		return;
	}

	FRIEND_UI_DATA kData;
	kData.m_bGroup = true;
	kData.m_nGroupID = m_nGroupKey;
	kData.m_bOpen = m_bOpenStatus;
	kData.m_bUser = false;
	kData.m_bOnline = true;

	int iCount = 0;
	if (KGroup::GUILD_BASE == m_nGroupKey && NULL == m_strGroupName.size())
	{
		std::map< DWORD, KBuddy >::iterator mapIter = m_mapFriend.begin();
		for (; mapIter != m_mapFriend.end(); ++mapIter)
		{
			if (mapIter->second.GetData().m_nGuildUserLevel > 0)
			{
				++iCount;
			}
		}
	}
	else
	{
		iCount = m_mapFriend.size();
	}

	std::wstringstream stm;
	stm << m_strGroupName
		<< L" ("
		<< GetOnlineBuddyCount() << L"/" << iCount
		<< L") ";
	kData.m_strDisplay = stm.str();

	vecRet.push_back(kData);

	if (m_bOpenStatus == true)
	{
		if (m_mapFriend.empty())
		{
			FRIEND_UI_DATA kData;
			kData.m_bGroup = false;
			kData.m_nGroupID = -1;
			kData.m_bUser = false;
			kData.m_strDisplay = g_pkStrLoader->GetString(STR_ID_NO_FRIENDS_LONEY);
			kData.m_bOnline = false;
			vecRet.push_back(kData);
		}
		else
		{
			std::vector< FRIEND_UI_DATA > vecTemp;
			iterBuddy mit;
			for (mit = m_mapFriend.begin(); mit != m_mapFriend.end(); ++mit)
			{
				KBuddy& kBuddy = mit->second;
				FRIEND_UI_DATA kData;
				kBuddy.GetUIData(kData);
				KFriend& kFriend = kBuddy.GetData();
				kData.m_nGroupID = m_nGroupKey;

				std::map<DWORD, KNGuildUserInfo>::iterator mit = g_kGlobalValue.m_mapNGuildUserInfo.find(kData.m_dwUserUID);

				if ((m_nGroupKey == KGroup::GUILD_BASE && kData.m_dwUserUID == g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID) ||
					(m_nGroupKey == KGroup::GUILD_BASE && kFriend.m_nGuildUserLevel == KNGuildUserInfo::GL_JOINER)) //길드목록에서 자신은 제외시킨다.
					continue;

				vecTemp.push_back(kData);
			}

			//내림차순 정렬을 한다.
			std::sort(vecTemp.begin(), vecTemp.end(), SortByAscending());

			//온/오프라인을 따로 정렬시킨다.
			std::sort(vecTemp.begin(), vecTemp.end(), SortByContactFriend());

			std::copy(vecTemp.begin(), vecTemp.end(), back_inserter(vecRet));
		}
	}
}

void KBuddyGroup::GetFriendListData(std::vector< KFriendListData >& vecRet)//KWAK
{
	{ // GROUP TITLE
		KFriendListData kFriendTemp;
		kFriendTemp.m_bGroupTitle = true;
		kFriendTemp.m_bGroupOpen = m_bOpenStatus;
		kFriendTemp.m_iGroupID = m_nGroupKey;

		std::wstringstream stm;
		stm << L"    "
			<< m_strGroupName
			<< L" ("
			<< GetOnlineBuddyCount() << L"/" << GetTotalBuddyCount()
			<< L") ";
		kFriendTemp.m_strNickname = stm.str();

		vecRet.push_back(kFriendTemp);
	}

	if (m_bOpenStatus == false)
		return;

	// USER DATA
	std::vector< KFriendListData > vecTemp;
	vecTemp.clear();
	std::map< DWORD, KBuddy >::iterator mit = m_mapFriend.begin();
	for (; mit != m_mapFriend.end(); ++mit)
	{
		if (mit->second.GetUID() == g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID) //자신 포함 X
			continue;
		KFriendListData kFriendTemp;
		kFriendTemp.m_dwUserUID = mit->second.GetUID();
		kFriendTemp.m_bOnline = mit->second.GetOnline();
		kFriendTemp.m_strNickname = mit->second.GetNickName();
		kFriendTemp.m_strNickColor = mit->second.GetNickColor();
		vecTemp.push_back(kFriendTemp);
	}
	//온라인 상단 정렬
	std::sort(vecTemp.begin(), vecTemp.end(), SortByContactFriend_KFriendListData());
	std::copy(vecTemp.begin(), vecTemp.end(), back_inserter(vecRet));
}

int KBuddyGroup::GetOnlineBuddyCount()
{
	int iRet = 0;
	for (iterBuddy mit = m_mapFriend.begin(); mit != m_mapFriend.end(); ++mit)
	{
		FRIEND_UI_DATA kData;
		mit->second.GetUIData(kData);
		//접속자 수를 구할  자신은 포함시키지 않는다.
		if (true == mit->second.GetOnline() && kData.m_dwUserUID != g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID)
		{
			iRet++;
		}
	}
	return iRet;
}

bool KBuddyGroup::FindBuddy(DWORD dwUID)
{
	iterBuddy mit = m_mapFriend.find(dwUID);
	return mit != m_mapFriend.end();
}

bool KBuddyGroup::GetBuddyByNickName(const std::wstring& strNickName, KBuddy& kBuddy_)
{
	for (iterBuddy mit = m_mapFriend.begin(); mit != m_mapFriend.end(); ++mit)
	{
		KBuddy& kBuddy = mit->second;

		if (0 == wcsicmp(kBuddy.GetNickName().c_str(), strNickName.c_str()))
		{
			kBuddy_ = kBuddy;
			return true;
		}
	}
	return false;
}

int KBuddyGroup::GetTotalBuddyCount()
{
	if (m_nGroupKey == KGroup::GUILD_BASE) //그룹이 길드일 때는 길드가입대기자들은 총인원에서 제외되어야 한다.
	{
		int iCnt = 0;
		iterBuddy mit;
		for (mit = m_mapFriend.begin(); mit != m_mapFriend.end(); ++mit)
		{
			KBuddy& kBuddy = mit->second;
			KFriend& kFriend = kBuddy.GetData();

			if (kFriend.m_nGuildUserLevel == KNGuildUserInfo::GL_JOINER)
				continue;

			iCnt++;
		}
		return iCnt;
	}

	return m_mapFriend.size();
}

bool KBuddyGroup::IsInNickName(const std::wstring& strNickName_)
{
	iterBuddy mit;
	for (mit = m_mapFriend.begin(); mit != m_mapFriend.end(); ++mit)
	{
		if (strNickName_ == mit->second.GetNickName()) {
			return true;
		}
	}

	return false;
}