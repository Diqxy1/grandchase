#ifndef __PLAY_AUTH_CONNECTORBASE_H__
#define __PLAY_AUTH_CONNECTORBASE_H__


//
// PlayAuth.Connector
//

#if defined(_WIN32) || defined(_WIN64)
#ifdef PLAY_AUTH_CONNECTOR_EXPORTS
#define PLAY_AUTH_CONNECTOR_API __declspec(dllexport)
#else
#define PLAY_AUTH_CONNECTOR_API __declspec(dllimport)

#ifndef _DEBUG
#pragma comment(lib, "PlayAuth.Connector.lib")
#else
#pragma comment(lib, "PlayAuth.ConnectorD.lib")
#endif
#endif
#else
#define PLAY_AUTH_CONNECTOR_API
#endif



#endif // __PLAY_AUTH_CONNECTORBASE_H__