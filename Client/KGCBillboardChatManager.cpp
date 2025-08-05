#include "stdafx.h"
#include "KGCBillboardChatManager.h"

ImplementSingleton( KGCBillboardChatManager );

KGCBillboardChatManager::KGCBillboardChatManager()
{
	m_dwLastTickCount = GetTickCount() + MessageMaxTime;
}

KGCBillboardChatManager::~KGCBillboardChatManager()
{

}

void KGCBillboardChatManager::PushMessage( const std::wstring strNickname, const std::wstring strMessage, D3DCOLOR dwColor )
{
	m_dwLastTickCount = GetTickCount();
	m_strMessage = strMessage;
	m_strNickname = strNickname;
	m_dwColor = dwColor;
}

void KGCBillboardChatManager::PopMessage( std::wstring& strNickname, std::wstring& strMessage, D3DCOLOR& dwColor )
{
	if( GetTickCount() - m_dwLastTickCount >= MessageMaxTime )
	{
		strNickname = L"";
		strMessage = L"";
		dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	else
	{
		strMessage = m_strMessage;
		strNickname = m_strNickname;
		dwColor = m_dwColor;
	}
}