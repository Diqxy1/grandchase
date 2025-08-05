//
// 1. Do not include this header file in another head file.
// 2. This header file should be included last.
//

#ifndef _GLOBALVARIABLES_H_
#define _GLOBALVARIABLES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 060206. jseop. nProtect game guard global variable.
#ifdef __NPGAMELIB_H__
#ifndef NO_GAMEGUARD
extern CNPGameLib g_CNPGameLib;
#endif NO_GAMEGUARD
#endif // #ifdef __NPGAMELIB_H__

// 060218. jseop nProtect game guard check timer.
#ifdef _NPROTECT_H_
extern float g_fNPCheckTime;
#endif // #ifdef _NPROTECT_H_

// 060331. jseop. client close reserving flag.
extern BOOL g_bReserveClose;

//==============================================================================
// 2009.04.21 : tcpl85
// Comment : XTrap Check Time
extern float g_fXTrapCheckTiem;

#endif // _GLOBALVARIABLES_H_
