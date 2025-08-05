/** @file   KJohnGen.h
	@brief  interface of KJohnGen
	@author seojt@kogstudios.com
    @date   seojt on 2005-5-2, 11:42:04
            - UNICODE compatible
    @date   seojt on 2005-07-06
            - ARRAY_SIZE
    @date   seojt on 2005년 10월 27일 목요일
            - ASSERT_FLOAT()
*/

#pragma once
#pragma warning(disable:4503 4786 4788)
#define WIN32_LEAN_AND_MEAN

#include <crtdbg.h>
#include <d3dx9.h>
//#include <strsafe.h>


//------------------------------------------------------------------------------
// _DEBUG
//
#if defined(_DEBUG)

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp)         _ASSERTE(exp)
#endif // #if !defined(ASSERT)

// TRACE
//
//#if !defined(TRACE)
//    inline void _tracew(const TCHAR* _fmt, ...)
//    {
//        TCHAR   out[4096];
//        va_list argList;
//
//        va_start( argList, _fmt );
//        StringCchVPrintf( out, 4095, _fmt, argList );
//        va_end( argList );
//        OutputDebugString( out );
//    }//_tracew()
//
//    inline void _tracea(const char* _pszFormat, ...)
//    {
//        char    out[4096];
//        va_list argList;
//
//        va_start( argList, _pszFormat );
//        StringCchVPrintfA( out, 4095, _pszFormat, argList );
//        va_end( argList );
//        OutputDebugStringA( out );
//    }//_tracea()
//    #define TRACEA              _tracea
//    #define TRACEW              _tracew
//
//#ifdef UNICODE // 2005-5-2, 11:40:58
//#define TRACE               _tracew
//#else
//#define TRACE               _tracea
//#endif // #ifdef UNICODE // 2005-5-2, 11:40:58
//#endif // #if !defined(TRACE)

#define BREAK(cond)         if (cond) __asm int 3;
#define CASSERT(expn)       typedef char __C_ASSERT__[(expn)?1:-1]

#else // not defined _DEBUG ////////////////////////////////////////////////////

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp)
#endif // #if !defined(ASSERT)

// TRACE
//
#if !defined(TRACE)
    #define TRACE               1 ? (void)0 :
    #define TRACEA              1 ? (void)0 :
    #define TRACEW              1 ? (void)0 :
#endif // #if !defined(TRACE)

#define BREAK(cond)
#define CASSERT(expn)

#endif // defined(_DEBUG)
//
//------------------------------------------------------------------------------

// SAFE_DELETE
//
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p); (p)=NULL; } }
#endif

// SAFE_DELETE_ARRAY
//
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif

// SAFE_RELEASE
//
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

// SAFE_FILE_CLOSE
#ifndef SAFE_FILE_CLOSE
#define SAFE_FILE_CLOSE( p ) { if( p ) { fclose( p ); } }
#endif 

#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))

#define LINE1(x) #x
#define LINE(x) LINE1(x)
#define TODO(msg) message ( __FILE__ "(" LINE(__LINE__)  "): [TODO] " msg )
#define NOTE(msg) message ( __FILE__ "(" LINE(__LINE__)  "): [NOTE] " msg )

#ifdef _DEBUG
    #define ASSERT_FLOAT(f) if ( f >= 0 ){} else if ( f < 0 ){} else { assert( f && !"float error" ); }
#else // #ifdef _DEBUG
    #define ASSERT_FLOAT(f)
#endif // _DEBUG

//------------------------------------------------------------------------------
// CRT debug macros
//
#define INIT_CRTDEBUG()     _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF)
#define ASSERT_MEMORY()     ASSERT( _CrtCheckMemory() )

/** @example    ASSERT_MEMORY()

    #ifdef PROJECT_PCH
    #define _TOSTR1(x)    #x
    #define _TOSTR(x)     _TOSTR1(x)
    #include _TOSTR(PROJECT_PCH)
    #endif // PROJECT_PCH
    #include "KJohnGen.h"
    //#include <iostream>

    void main()
    {
        INIT_CRTDEBUG();

        int*    p = new int;
        delete p;

        *p = 3; // using after deletion
        ASSERT_MEMORY(); // check call stack window
        delete p;
    }//main()

*/

#define BREAK_ALLOC(line)   _CrtSetBreakAlloc( line )

/** @example    BREAK_ALLOC()

  //------------------------------------------------------------------------------
  //
  1) memory leak example:

    #ifdef PROJECT_PCH
    #define _TOSTR1(x)    #x
    #define _TOSTR(x)     _TOSTR1(x)
    #include _TOSTR(PROJECT_PCH)
    #endif // PROJECT_PCH
    #include "KJohnGen.h"
    #pragma warning(disable: 4995)
    //#include <iostream>

    void main()
    {
        INIT_CRTDEBUG();

        int*    p = new int;

        *p = 3;
    }//main()

    // debug message:
    Detected memory leaks!
    Dumping objects ->
    {44} normal block at 0x00372DA8, 4 bytes long.
    Data: <    > 03 00 00 00 
    Object dump complete.

    // @note    allocation number is {44}, so you can set break point at 44th alloc
    //          see below example


  //------------------------------------------------------------------------------
  //
  2) break on mal-alloc {44}:

    #ifdef PROJECT_PCH
    #define _TOSTR1(x)    #x
    #define _TOSTR(x)     _TOSTR1(x)
    #include _TOSTR(PROJECT_PCH)
    #endif // PROJECT_PCH
    #include "KJohnGen.h"
    #pragma warning(disable: 4995)
    //#include <iostream>

    void main()
    {
        BREAK_ALLOC( 44 ); // install break CRT point condition
        INIT_CRTDEBUG();

        int*    p = new int;

        *p = 3;
    }//main()

*/


