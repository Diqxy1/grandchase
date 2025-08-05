#include "stdafx.h"
#include ".\kbuddy.h"
//

KBuddy::KBuddy(void)
{
}

KBuddy::~KBuddy(void)
{
}

void KBuddy::SetData(const KFriend& kPacket)
{
	m_kData = kPacket;
}

void KBuddy::GetUIData(OUT FRIEND_UI_DATA& kData)
{
	kData.m_bUser = true;
	kData.m_bGroup = false;
	kData.m_bOnline = m_kData.m_bIsOnline;
	kData.m_dwEmoticon = m_kData.m_dwEmoticon;
	kData.m_dwUserUID = m_kData.m_dwUserUID;

	std::wstring strName;
	std::wstring strColor;
	std::wstring strStatus;

	if (m_kData.m_strNickColor == L"")
	{
		strColor = L"FFFFFF";
	}
	else
	{
		strColor = m_kData.m_strNickColor;
	}

	if (m_kData.m_strNickName == L"")
	{
		strName = g_pkStrLoader->GetString(STR_ID_NO_NICKNAME);
	}
	else
	{
		strName = m_kData.m_strNickName;
	}


	if (m_kData.m_bIsOnline)
	{
		strStatus = g_pkStrLoader->GetString(STR_ID_ONLINE);
	}
	else
	{
		strStatus = g_pkStrLoader->GetString(STR_ID_OFFLINE);
	}

	kData.m_strDisplay = L"#c" + strColor + strName + L"#cX - " + strStatus;
	kData.m_strNickname = m_kData.m_strNickName;
	kData.m_strNickColor = strColor;
}