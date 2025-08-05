#include "NetCommon.h"
#include <string>
#include <KncUtil.h>
namespace NetCommon
{
	wchar_t WSAMsg[256] = { 0 };
}

const wchar_t* NetCommon::GetWsaMsg()
{
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ::WSAGetLastError(),
		GetSystemDefaultLangID()/*MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/,
		WSAMsg, MAX_PATH, NULL);
	WSAMsg[wcslen(WSAMsg) - 1] = L'\0';   // 문자열의 마지막에 있는 개행(\n)을 없앰.
	return WSAMsg;
}

const char* NetCommon::GetLocalIP()
{
	char szHostName[256];
	PHOSTENT pHostInfo;

	if (gethostname(szHostName, sizeof(szHostName)) != 0)
	{
		return false;
	}

	pHostInfo = gethostbyname(szHostName);

	if (pHostInfo == NULL)
	{
		return false;
	}

	return inet_ntoa(*(struct in_addr *)*pHostInfo->h_addr_list);
}

std::wstring NetCommon::GetLocalIPW()
{
	std::string strLocalIP = GetLocalIP();
	std::wstring wstrLocalIP = KncUtil::toWideString(strLocalIP.c_str(), strLocalIP.length());

	return wstrLocalIP;
}

bool NetCommon::InitWinSock()
{
	// socket init
	WORD        wVer = MAKEWORD(2, 2);
	WSADATA     wsaData;

	if (::WSAStartup(wVer, &wsaData) != 0)
	{
		::WSACleanup();
		return false;
	}

	if (wsaData.wVersion != MAKEWORD(2, 2))
	{
		::WSACleanup();
		return false;
	}

	return true;
}

const char* NetCommon::CalcIP(DWORD dwIP)
{
	in_addr IP;
	IP.s_addr = dwIP;
	return inet_ntoa(IP);
}