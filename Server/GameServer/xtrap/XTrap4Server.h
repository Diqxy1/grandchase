// ***************************************************************
//  X-Trap Interface Library For Server
//  -------------------------------------------------------------
//  Copyright (C)WiseLogic 2005 - All Rights Reserved
// ***************************************************************

#ifndef	__XTRAPAPI_SERVER_H
#define __XTRAPAPI_SERVER_H

// Buffer Size
#define XTRAP_CS4_COMSIZE_PACKDATA					128
#define XTRAP_CS4_BUFSIZE_SESSION					320
#define XTRAP_CS4_BUFSIZE_SESSIONADDON				80
#define	XTRAP_CS4_BUFSIZE_MAP						13000

// Timeout Limit
#define XTRAP_CS4_MAXTIMEOUT						(int)600
#define XTRAP_CS4_MINTIMEOUT						(int)100

// Return Value
#define XTRAP_API_RETURN_OK							0x00000000
#define XTRAP_API_RETURN_ERROR						0x00000001
#define XTRAP_API_RETURN_UNKNOWN					0x00000002
#define XTRAP_API_RETURN_INVALID_PARAMETER			0x00000003
#define XTRAP_API_RETURN_INVALID_CRC				0x00000004
#define XTRAP_API_RETURN_TIMEOUT					0x00000005
#define XTRAP_API_RETURN_DETECTTIMEOUT				0x00000006
#define XTRAP_API_RETURN_DETECTHACK					0x0000000F


// Protect Option
#define	XTRAP_PROTECT_PE							0x00000001
#define	XTRAP_PROTECT_TEXT							0x00000010
#define	XTRAP_PROTECT_RDATA							0x00000100
#define	XTRAP_PROTECT_EDATA							0x00001000
#define	XTRAP_PROTECT_RSRC							0x00010000
#define	XTRAP_PROTECT_RELOC							0x00100000
#define XTRAP_PROTECT_EXCEPT_VIRUS					0x00000002

// XTrap_Module Method												   Type								default
#define XTRAP_CS_MODULE_DM_TIMEOUT					0x00000001		// Detect Method	: Timeout		(on)
#define XTRAP_CS_MODULE_LOG_BUF						0x00000100		// Log Method		: Buffer		(on)
#define XTRAP_CS_MODULE_LOG_FILE					0x00000200		// Log Method		: File			(off)

// XTRAP OPTION FLAG
#define XTRAP_CS_OPTION_NULL						0x00000000
#define XTRAP_CS_OPTION_USETIME						0x00000001

// XTRAP ACTIVE CODE
#define XTRAP_ACTIVE_CODE_DEFAULT					0x0FFFFFFF
#define XTRAP_ACTIVE_CODE_THEMIDA					0x0FFFFFC3
#define XTRAP_ACTIVE_CODE_NOTUSE_MAPFILE			0x0FFFFF0C
#define XTRAP_ACTIVE_CODE_NOTUSE_RANDOMINIT			0x30000000

#define XTRAP_ACTIVE_CODE_LEVEL1					XTRAP_ACTIVE_CODE_DEFAULT			// 모든 기능 사용
#define XTRAP_ACTIVE_CODE_LEVEL2					XTRAP_ACTIVE_CODE_THEMIDA			// 패킹 지원
#define XTRAP_ACTIVE_CODE_LEVEL3					XTRAP_ACTIVE_CODE_NOTUSE_MAPFILE	// 맵 파일 사용안함.
#define XTRAP_ACTIVE_CODE_LEVEL_TEST				XTRAP_ACTIVE_CODE_NOTUSE_RANDOMINIT	// TEST 모드.


#if		defined(XTRAP4SERVER_EXPORTS)
#define XTRAPCC_API __declspec(dllexport)
#else
#define XTRAPCC_API
#endif

#if		defined(_WIN32)
#define XTRAPCC_CALLCONV __cdecl
#else
#define XTRAPCC_CALLCONV
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 리눅스 버전일때는 #if (!defined())로 진행을 해야 한다.
//#if		(!defined(XTRAP4SERVER_EXPORTS))
#if		(defined(XTRAP4SERVER_EXPORTS))



/////////////////////////////////////////////////
// X-Trap Server Interface
// Required
/////////////////////////////////////////////////

XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_Start(
	IN	unsigned long	ulTimeOut, 
	IN	unsigned long	ulMapCount,
	IN	const void*		pBufMap,
		void*			Reserved
	);


XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SessionInit(
	IN	unsigned long	ulTimeOut,
	IN	unsigned long	ulMapCount,
	IN	const void*		pBufMap,
	INOUT	void*		pBufSession 
	);

XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SetActiveCode(
	IN unsigned long	ulActiveCode
	);


XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_CS_Step1(
	INOUT	void*		pBufSession,
	OUT		void*		pBufPackData
	);



XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_CS_Step3(
	INOUT	void*		pBufSession,
	IN	const void*		pBufPackData
	);

XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_CS_Loading(
	IN	void*		pBufSession
	);

XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SendGamePacket(
	IN	void*		pBufSession
	);

XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_RecvGamePacket(
	IN	void*		pBufSession
	);
/////////////////////////////////////////////////
// X-Trap Server Interface
// Optional
/////////////////////////////////////////////////
XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SessionReset(
		unsigned long	ulTimeOut,
		unsigned long	ulMapCount,
		const void*		pBufMap,
		void*			pBufSession 
	);



XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SetModuleOn(
		unsigned long	ulMethod
	);



XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SetModuleOff(
		unsigned long	ulMethod
	);


XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SetOption(
		unsigned long	ulMethod,
		void*			pArray
	);



XTRAPCC_API unsigned long XTRAPCC_CALLCONV	XTrap_S_SaveLog(
		IN	const void*		pBufSession,
		IN	unsigned long	ulMethod,
		OUT	void*			pOut,
		IN	const char*		pUserID,
		IN	const char*		pServerName,
		IN	const char*		pUserIP,
		IN	const char*		Reserved,
		IN	long			lCharacterLevel
		);

#else

typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_Start)			( unsigned long ulTimeOut, unsigned long ulMapCount, const void* pBufMap, void* Reserved );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SessionInit)		( unsigned long ulTimeOut, unsigned long ulMapCount, const void* pBufMap, void* pBufSession );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SetActiveCode)	( unsigned long ulActiveCode );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_CS_Step1)			( void* pBufSession, void* pBufPackData );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_CS_Step3)			( void*	pBufSession, const void* pBufPackData );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SessionReset)	( unsigned long ulTimeOut, unsigned long ulMapCount, const void* pBufMap, void* pBufSession );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SetModuleOn)		( unsigned long	ulMethod );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SetModuleOff)	( unsigned long	ulMethod );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SetOption)		( unsigned long	ulMethod, void* pArray );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SaveLog)			( const void* pBufSession, unsigned long ulMethod, void* pOut, const char* pUserID, const char* pServerName, const char* pUserIP, const char* Reserved, long lCharacterLevel );

typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_CS_Loading)		( void* pBufSession );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_SendGamePacket)	( void* pBufSession );
typedef unsigned long	(XTRAPCC_CALLCONV *PFN_XTrap_S_RecvGamePacket)	( void* pBufSession );

extern PFN_XTrap_S_Start			XTrap_S_Start;
extern PFN_XTrap_S_SessionInit		XTrap_S_SessionInit;
extern PFN_XTrap_S_SetActiveCode	XTrap_S_SetActiveCode;
extern PFN_XTrap_CS_Step1			XTrap_CS_Step1;
extern PFN_XTrap_CS_Step3			XTrap_CS_Step3;
extern PFN_XTrap_S_SessionReset		XTrap_S_SessionReset;
extern PFN_XTrap_S_SetModuleOn		XTrap_S_SetModuleOn;
extern PFN_XTrap_S_SetModuleOff		XTrap_S_SetModuleOff;
extern PFN_XTrap_S_SetOption		XTrap_S_SetOption;
extern PFN_XTrap_S_SaveLog			XTrap_S_SaveLog;
extern PFN_XTrap_CS_Loading			XTrap_CS_Loading;
extern PFN_XTrap_S_SendGamePacket	XTrap_S_SendGamePacket;
extern PFN_XTrap_S_RecvGamePacket	XTrap_S_RecvGamePacket;



extern unsigned long	XTrap_S_LoadDll	();		// Dll 기본 로딩함수 예제
extern unsigned long	XTrap_S_FreeDll	();		// Dll 기본 해제함수 예제

#endif

#ifdef __cplusplus
}
#endif

#endif
