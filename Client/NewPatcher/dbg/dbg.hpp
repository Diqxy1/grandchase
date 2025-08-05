//////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Triple Buffer Software 2003 All rights reserved                                        //
//                                                                                                  //
// This is free sour code. You may check the terms and conditions for free source code usage at:    //
//                                                                                                  //
// http://triplebuffer.devmaster.net/license.php                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TB_DBG_HPP
#define TB_DBG_HPP


#include "logfile.hpp"
#include "assert.hpp"
#include "profiler.hpp"
//#include "mmgr.hpp"
#include <atltime.h>


#define KNC_TIME_FORMAT             _T("%Y-%m-%d %H:%M:%S")
#define LOG_HEADER_STR              __WFUNCTION__ L"(), " <<(LPCTSTR)CTime::GetCurrentTime().Format( KNC_TIME_FORMAT ) << L", "
#define START_LOG( type, data )     dbg::type << L#type L" : " LOG_HEADER_STR << data << dbg::endl
#define START_LOG_WITH_NAME( type ) START_LOG( type, L", Name : " << m_strName )
#define BUILD_LOG( data )           dbg::tab << L#data L" : " << data << dbg::endl
#define BUILD_LOGc( data )          dbg::tab << L#data L" : " << static_cast<int>(data) << dbg::endl
#define BUILD_LOGtm( data )         dbg::tab << L#data L" : " << (LPCWSTR)data.Format( KNC_TIME_FORMAT ) << dbg::endl
#define END_LOG                     dbg::tab << L"(" __WFILE__ L", " << __LINE__ << L")" << dbg::endl
#define LOG_CONDITION( cond, ctrue, cfalse )     \
    dbg::logfile& rlog = cond ? dbg::ctrue : dbg::cfalse; \
    wchar_t* szLogLevel = cond ? L#ctrue : L#cfalse; \
    rlog << szLogLevel << L" : " LOG_HEADER_STR << dbg::endl

#define LOG_SUCCESS( success ) LOG_CONDITION( success, clog, cerr )

// KncW 부터는 쓰지 않을 것들.
#define EXCEPTION_LOG( data )       dbg::cout << LOG_HEADER_STR << data << END_LOG
#define PRINT_LASTERROR             dbg::cwarn << NetError::GetLastErrMsg() << L"(" << NetError::GetLastError() << L")" << END_LOG


#ifdef CONVERT_JIF_LIF_TO_ASSERTION
#   define _JIF( x, action )  assert( x )
#   define  LIF(x)            assert( x )
#else
#   define _JIF( x, action )  do { if( !(x) ) { dbg::cerr << __WFUNCTION__ L"(), in " L#x << END_LOG; action; }} while(0)
#   define  LIF(x)            do { if( !(x) ) { dbg::cerr << __WFUNCTION__ L"(), in " L#x << END_LOG; }} while(0)
#endif // CONVERT_JIF_LIF_TO_ASSERTION

#define JIF(x) _JIF( x, return (void)0 )

#endif // TB_DBG_HPP
