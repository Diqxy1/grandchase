#ifndef __PLAY_AUTH_CONNECTOR_H__
#define __PLAY_AUTH_CONNECTOR_H__

#include "PlayAuth.ConnectorBase.h"
#include "PlayAuth.Handles.h"
#include "PlayAuth.Info.h"


namespace PlayAuth
{
	namespace Connector
	{
		/////////////////////////////////////
		// callbacks

		// connection
		typedef void (__stdcall	*OnConnectedCallback )					( void* context, const char* ip, unsigned short port);
		typedef void (__stdcall	*OnDisconnectedCallback )				( void* context, const char* ip, unsigned short port);

	
		//////////////////////////////////////////////////////////////////////////
		PLAY_AUTH_CONNECTOR_API unsigned int __stdcall GetLastError();

		/////////////////////////////////////
		// Handle
		PLAY_AUTH_CONNECTOR_API Handle __stdcall CreateHandle			();
		PLAY_AUTH_CONNECTOR_API void __stdcall DestroyHandle			(Handle handle);


		/////////////////////////////////////
		// start / stop
		PLAY_AUTH_CONNECTOR_API bool __stdcall Start(Handle handle);
		PLAY_AUTH_CONNECTOR_API void __stdcall Stop(Handle handle );

		// callback
		PLAY_AUTH_CONNECTOR_API bool __stdcall SetOnConnected			( Handle handle, OnConnectedCallback callback, void* context);
		PLAY_AUTH_CONNECTOR_API bool __stdcall SetOnDisconnected		( Handle handle, OnDisconnectedCallback callback, void* context);

		// CheckIn/Out API를 호출 callback
		typedef void (__stdcall	*OnCheckInCallback )	(void* context, int errorCode, const char* szUniqueID, unsigned long uUserIP, const char* szTicket, const UserCheckInInfo& rCheckInInfo);
		PLAY_AUTH_CONNECTOR_API bool __stdcall CheckIn	(Handle handle, const char* szUniqueID, unsigned long uUserIP, int nAge, OnCheckInCallback callback, void* context);

		typedef void (__stdcall	*OnCheckOutCallback )	(void* context, int errorCode, const char* szUniqueID, const char* szTicket);
		PLAY_AUTH_CONNECTOR_API bool __stdcall CheckOut	(Handle handle, const char* szUniqueID, const char* szTicket, OnCheckOutCallback callback, void* context);

		// 유저접속상태 체크
		// OnUserAliveCheckCallback 콜백이 호출시 게임서버는 해당유저가 게임서버내에 존재하는지여부를 확인하여서, bIsOnline변수에 true/false를 채워주어야한다
		typedef void (__stdcall	*OnUserAliveCheckCallback )	(void* context, const char* szUniqueID, const char* szTicket, bool& bIsOnline);
		PLAY_AUTH_CONNECTOR_API bool __stdcall SetOnUserAliveCheck	(Handle handle, OnUserAliveCheckCallback callback, void* context);

		// user state notification callback func
		// 유저의 셧다운제상태에 대해서 노티시 OnUserStateNotiCallback콜백이 호출됨
		typedef void (__stdcall	*OnUserStateNotiCallback )	(void* context, const int nNotiType, const UserNotiInfo& rUserNotiInfo);
		PLAY_AUTH_CONNECTOR_API bool __stdcall SetOnUserStateNoti	(Handle handle, OnUserStateNotiCallback callback, void* context);

		// 게임서버에서 특정유저의 상태가 궁금할시에 호출함
// 		typedef void (__stdcall	*OnUserStateCallback )	(void* context, int errorCode, const char* szUniqueID, const char* szTicket, unsigned long uUserIP, const UserStateInfo& rUserState);
// 		PLAY_AUTH_CONNECTOR_API bool __stdcall GetUserState(Handle handle, const char* szUniqueID, const char* szTicket, OnUserStateCallback, void* context);
	}
}

#endif // __PLAY_AUTH_CONNECTOR_H__
