#pragma once

////#include <vector>
#include <atltime.h>    // CString
#include <thread/Locker.h>  // KncCriticalSection
//#include <string>

#define _DeclareException  \
protected: \
static std::vector<std::wstring>    ms_vecExceptionInfo; \
static KncCriticalSection           ms_csExceptionTime; \
public: \
    static void DumpExceptionInfo( IN OUT std::wostream& stm )

#define _ImplementException( classname ) \
    std::vector<std::wstring>   classname##::ms_vecExceptionInfo; \
    KncCriticalSection          classname##::ms_csExceptionTime; \
    void classname##::DumpExceptionInfo( IN OUT std::wostream& stm_ ) \
    { \
        KLocker lock( ms_csExceptionTime ); \
        stm_ << L"    " L ## #classname L" #Exception : " << ms_vecExceptionInfo.size() << std::endl; \
        std::vector<std::wstring>::iterator vit; \
        int nCount = 1; \
        for( vit = ms_vecExceptionInfo.begin(); vit != ms_vecExceptionInfo.end(); vit++ ) \
        { \
            stm_ << L"    " << nCount << L" " << *vit << std::endl; \
            nCount++; \
        } \
    }

#define _BEGIN_TRY_BLOCK     try {
#define _END_TRY_BLOCK( data )       } catch( std::exception& ex ) \
{ \
    std::wstring strExceptionTime = __WFUNCTION__; \
    strExceptionTime += L"(), EXCEPTION. "; \
    strExceptionTime += (LPCTSTR)CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")); \
    strExceptionTime += L", "; \
    strExceptionTime += KncUtil::toWideString( ex.what() ).c_str(); \
    dbg::cout << strExceptionTime << data << END_LOG; \
    { \
        KLocker lock( ms_csExceptionTime ); \
        if( ms_vecExceptionInfo.size() < 100 ) \
            ms_vecExceptionInfo.push_back( strExceptionTime ); \
    } \
} \
catch( ... ) \
{ \
    std::wstring strExceptionTime = __WFUNCTION__;\
    strExceptionTime += L"(), EXCEPTION. "; \
    strExceptionTime += (LPCTSTR)CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")); \
    strExceptionTime += L", Unknown Exception Occured."; \
    dbg::cout << strExceptionTime << data << END_LOG; \
    { \
        KLocker lock( ms_csExceptionTime ); \
        if( ms_vecExceptionInfo.size() < 100 ) \
            ms_vecExceptionInfo.push_back( strExceptionTime ); \
    } \
}

#define _END_TRY_BLOCK_NOINFO( data )   } catch( std::exception& ex ) \
    { \
        EXCEPTION_LOG( ex.what() << data ); \
    } \
    catch( ... ) \
    { \
        EXCEPTION_LOG( "Unknown Exception Occured." << data ); \
    }

#define _END_TRY_BLOCK_RETHROW( data )   } catch( std::exception& ex ) \
    { \
        EXCEPTION_LOG( ex.what() << data ); \
        throw ex; \
    } \
    catch( ... ) \
    { \
        EXCEPTION_LOG( "#Unknown Exception Occured." << data ); \
        throw std::exception( __FUNCTION__ ); \
    }
// 
#ifdef _KNC_NO_CRASH_
#   define DeclareException     _DeclareException
#   define ImplementException   _ImplementException
#   define BEGIN_TRY_BLOCK      _BEGIN_TRY_BLOCK
#   define END_TRY_BLOCK        _END_TRY_BLOCK
#   define END_TRY_BLOCK_NOINFO _END_TRY_BLOCK_NOINFO
#   define END_TRY_BLOCK_RETHROW    _END_TRY_BLOCK_RETHROW
#else
#   define DeclareException
#   define ImplementException( x )
#   define BEGIN_TRY_BLOCK
#   define END_TRY_BLOCK( x )
#   define END_TRY_BLOCK_NOINFO( x )
#   define END_TRY_BLOCK_RETHROW( x )
#endif