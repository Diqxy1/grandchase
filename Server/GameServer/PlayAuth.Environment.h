#ifndef __PLAY_AUTH_ENVIRONMENT_H__
#define __PLAY_AUTH_ENVIRONMENT_H__


#include "PlayAuth.ConnectorBase.h"
#include "PlayAuth.Handles.h"

using namespace std;

namespace PlayAuth
{
	namespace Environment
	{
		PLAY_AUTH_CONNECTOR_API Handle	__stdcall CreateHandle(unsigned int nThreadPoolSize);
		PLAY_AUTH_CONNECTOR_API void	__stdcall DestroyHandle(Handle handle);
		PLAY_AUTH_CONNECTOR_API bool	__stdcall SetServerAddress(Handle handle, const char* ip, unsigned short port);
	}
}

#endif // __PLAY_AUTH_ENVIRONMENT_H__