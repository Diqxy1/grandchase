#ifndef _KGC_BILLBOARD_CHAT_MANAGER_H_
#define _KGC_BILLBOARD_CHAT_MANAGER_H_
#pragma once


#include "KNCSingleton.h"
const DWORD MessageMaxTime = 60000; // 1 minute;

class KGCBillboardChatManager
{
	DeclareSingleton( KGCBillboardChatManager );
public:
	KGCBillboardChatManager();
	~KGCBillboardChatManager();

public:
	void PushMessage( const std::wstring strNickname, const std::wstring strMessage, D3DCOLOR dwColor );
	void PopMessage( std::wstring& strNickname, std::wstring& strMessage, D3DCOLOR& dwColor );

private:

	std::wstring m_strNickname;
	std::wstring m_strMessage;
	D3DCOLOR m_dwColor;
	DWORD m_dwLastTickCount;
};

DefSingletonInline( KGCBillboardChatManager );

#endif