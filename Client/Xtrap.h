#pragma once


extern void WINAPI XTRAP_CALLBACK__XtrapCallbackProcedure (
	IN unsigned int Reserve1,
	IN unsigned int Reserve2,
	IN unsigned int Reserve3,
	IN unsigned int Reserve4,
	IN unsigned char *pXtrapCode	// [in, out] Pointer to a XTRAP_CODE data structure
	);

DWORD Convert_XTRAP_ERRORCODE( XTRAP_CODE& XtrapCode_ );

extern void ExitWithNPHSErrorMsg( LPCWSTR szTitle, long lErrorCode, LPCWSTR szMsg1, LPCWSTR szMsg2, BOOL bShowErrorCode );
