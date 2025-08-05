
/***********************************************************************************************************/
/* [TAG] Xtrap_C_XtrapCode.h | 2009.06.27 | X-TRAP Interface Library For Client                            */
/*                                                                                                         */
/*  Copyright (C)WiseLogic 2005 - 2009 All Rights Reserved                                                 */
/***********************************************************************************************************/

#ifndef __Xtrap_C_XtrapCode_H
#define __Xtrap_C_XtrapCode_H

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Library Export Declaration Undocument (for CLIENT) typedef
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// XTRAP_CODE . MajorCode ///////////////////////////////////////////////////////////////////////////////////

// The values are defined in the special document file.

// XTRAP_CODE . MinorCode ///////////////////////////////////////////////////////////////////////////////////

// The values are defined in the special document file.

// XTRAP_CODE . Kinds ///////////////////////////////////////////////////////////////////////////////////////

#define XTRAPCODE_UNDEFINED						0x00000000
#define XTRAPCODE_KINDS_CRACK					0x00000001
#define XTRAPCODE_KINDS_CHEAT					0x00000002
#define XTRAPCODE_KINDS_ETC						0x00000003
#define XTRAPCODE_KINDS_BOT						0x00000004
#define XTRAPCODE_KINDS_SYSTEM					0x00000005
#define XTRAPCODE_KINDS_PATCH					0x00000006
#define XTRAPCODE_KINDS_ADMIN					0x00000007
#define XTRAPCODE_KINDS_KERNEL					0x00000008
#define XTRAPCODE_KINDS_ALERT					0x00000009
#define XTRAPCODE_KINDS_EXCEPT					0x00000010
#define XTRAPCODE_KINDS_DEBUG					0x00000011
#define XTRAPCODE_KINDS_MACRO					0x00000012
#define XTRAPCODE_KINDS_LAUNCHER				0x00000013

// XTRAP_CODE . Status ///////////////////////////////////////////////////////////////////////////////////////

#define XTRAPCODE_STATUS_UNDEFINED				0x00000000
#define XTRAPCODE_STATUS_DETECT					0x00000001
#define XTRAPCODE_STATUS_FAIL					0x00000002
#define XTRAPCODE_STATUS_REFERRENCE				0x00000003

// XTRAP_CODE . XTrap_C_Start ///////////////////////////////////////////////////////////////////////////////

#define XTRAPCODE_XTRAP_X_START_UNDEFINED		0x00000000
#define XTRAPCODE_XTRAP_X_START_SUCCESS			0x00000001
#define XTRAPCODE_XTRAP_X_START_FAIL			0x00000002

// XTRAP_CODE . ErrorCode ///////////////////////////////////////////////////////////////////////////////////

// The values are defined in the WinError.h header file.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XTRAP_CODE ///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _tagXTRAP_CODE
{
	unsigned int MajorCode;
	unsigned int MinorCode;

	unsigned int Kinds;
	unsigned int Status;
	
	unsigned int ErrorCode;
	unsigned int XTrap_C_Start;

} XTRAP_CODE, *PXTRAP_CODE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Library Export Declaration Undocument (for CLIENT) typedef
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (WINAPI *PFN_XTRAP_CALLBACK__XtrapCallbackProcedure) (
	IN unsigned int Reserve1,
	IN unsigned int Reserve2,
	IN unsigned int Reserve3,
	IN unsigned int Reserve4,
	IN unsigned char *pXtrapCode	// [in] Pointer to a XTRAP_CODE data structure
);

#endif
