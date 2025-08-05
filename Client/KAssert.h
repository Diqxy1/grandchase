/**
    @file   KAssert.h
    @since  2003-01-14 am 10:51:55
    @author seojt@kogsoft.com
*/

#ifndef _KASSERT_H_
#define _KASSERT_H_

#define WIN32_LEAN_AND_MEAN

#ifndef _MSC_VER
    #error "KAssert.h needs Visual C++ 6 or higher"
#endif // #ifndef _MSC_VER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstdio>
#include <cassert>
#include <windows.h>
#include <crtdbg.h>
#include <tchar.h>


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          if (p) { delete (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    if (p) { delete[] (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE_ARRAY

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         if (p) { (p)->Release(); (p) = NULL; }
#endif // #ifndef SAFE_RELEASE

// undefine previous definition if exist
#ifdef BREAK
#undef BREAK
#endif // #ifdef BREAK
#ifdef ASSERT
#undef ASSERT
#endif // #ifdef ASSERT
#ifdef VERIFY
#undef VERIFY
#endif // #ifdef VERIFY
#ifdef TRACE
#undef TRACE
#endif // #ifdef TRACE
#ifdef TODO
#undef TODO
#endif // #ifdef TODO


#if defined(_DEBUG) || defined(_USE_ASSERT_IN_RELEASE_MODE)
    #define BREAK(cond)                     if (cond) __asm int 3;
    #define ASSERT(exp)                     assert(exp)
    #define VERIFY(exp)                     assert(exp)
#if defined( UNICODE )
    inline void _trace(WCHAR *fmt, ...)
    {
        WCHAR   out[1024];
	    va_list body;

	    va_start(body, fmt);
	    vswprintf(out, fmt, body);
        ASSERT( _tcslen(out) < 1024 && L"TRACE() buffer is overrun" );
	    va_end(body);

        ::OutputDebugString(out);
    }//_trace()
#else
    inline void _trace(CHAR *fmt, ...)
    {
        CHAR   out[1024];
        va_list body;

        va_start(body, fmt);
        vsprintf(out, fmt, body);
        ASSERT( _tcslen(out) < 1024 && "TRACE() buffer is overrun" );
        va_end(body);

        ::OutputDebugString(out);
    }//_trace()
#endif
    #define TRACE                           _trace
    #define GETOBJECTTYPE(h)                ::GetObjectType(h)
    #define ISWINDOW(hWnd)                  assert( TRUE == ::IsWindow(hWnd) )
    #define ASSERT_CODEPTR(lpfn)            assert( FALSE == ::IsBadCodePtr(lpfn) )
    #define ASSERT_READPTR(lp, ucb)         assert( FALSE == ::IsBadReadPtr(lp, ucb) )
    #define ASSERT_WRITEPTR(lp, ucb)        assert( FALSE == ::IsBadWritePtr(lp, ucb) )
    #define ASSERT_STRINGPTR(lpsz, ucchMax) assert( FALSE == ::IsBadStringPtr(lpsz, ucchMax) )
#else // not defined _DEBUG
    #define BREAK(cond)
    #define ASSERT(exp)
    #define VERIFY(exp)                     exp
    #define TRACE                           1 ? (void)0 :
    #define GETOBJECTTYPE(h)
    #define ISWINDOW(hWnd)
    #define ASSERT_CODEPTR(lpfn)
    #define ASSERT_READPTR(lp, ucb)
    #define ASSERT_WRITEPTR(lp, ucb)
    #define ASSERT_STRINGPTR(lpsz, ucchMax)
#endif // defined(_DEBUG)


#define LINE1(x)    #x
#define LINE(x)     LINE1(x)
#define TODO(msg)   message ( __FILE__ "(" LINE(__LINE__)  "): [TODO] " msg )

#endif // _KASSERT_H_


/** @example    KAssert.h

    #include "KAssert.h"

    void main()
    {
        ///////////////////////////////////////////////////////////////
        // 1. usage of ASSERT(), TRACE(), BREAK() and SAFE_DELETE()
        int*    p = NULL;

        ASSERT( p == NULL );
        p = new int[10];
        ASSERT( p != NULL );
        TRACE( "[info] p allocated at line %i\n", __LINE__ );
        // see Debug Window:
        // [info] p allocated at line 13
        p[9] = 3;
        BREAK( p[9] == 3 ); // Start debug if condition satisfied when 'Debug Run'
        SAFE_DELETE_ARRAY( p );

        ///////////////////////////////////////////////////////////////
        // 2. usage of ASSERT_WRITEPTR()
        int     p2[8];

        ASSERT_WRITEPTR( p, sizeof(int)*9 ); // run-time error occurs
        p2[9] = 1;

        ///////////////////////////////////////////////////////////////
        // 3. usage TODO()
    #pragma TODO( "This function is not tested\n" )
        // see Build Window:
        // e:\[my projects]\console\main.cpp(23): [TODO] This function is not tested

        ///////////////////////////////////////////////////////////////
        // 4. usage CRT debug assert
        //    you must include "crtdbg.h"
        _ASSERTE( p != NULL );
        _ASSERT( p != NULL );
    }//main()

*/
