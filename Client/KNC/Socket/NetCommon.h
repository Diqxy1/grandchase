#pragma once
//#include <Windows.h>
//#include <string>
// SD_BOTH flag로 shutdown하는 경우, shutdown 없이 바로 closesocket 호출하는 것과 동일하다.
// 송/수신 시점에 있는 데이터를 손실할 가능성이 있다.
#define CLOSE_SOCKET( socket ) \
    if( socket != INVALID_SOCKET ) \
    { \
    ::shutdown( socket, SD_BOTH ); \
    ::closesocket( socket ); \
    socket = INVALID_SOCKET; \
    } 

#define GET_WSA_MSG NetCommon::GetWsaMsg()

namespace NetCommon
{
    const wchar_t* GetWsaMsg();
    const char* GetLocalIP();
	std::wstring GetLocalIPW();
    bool InitWinSock();
    const char* CalcIP( DWORD dwIP );

}