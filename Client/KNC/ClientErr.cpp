#include "stdafx.h"
#include ".\CLIENTERR.h"

//
//


namespace KClientErr  // 변수 선언은 namespace scope를 열고 해주어야 한다.
{

#undef _ENUM
#define _ENUM( id, comment ) L ## #comment,
    wchar_t* szErrorStr[] = {
#include "ClientErr_def.h"
    };

    // key : thread id. element : last error
    std::map<DWORD, int> mapLastError;

    std::map<DWORD, int>::iterator mit;     // 빠른 작업을 위해..

    KncCriticalSection  csKClientErr;
}

wchar_t* KClientErr::GetErrStr( int nErrorID )
{
    KLocker lock( csKClientErr );

    if( nErrorID >= CE_TOTAL || nErrorID < 0 ) return szErrorStr[ CE_UNKNOWN ];
    return szErrorStr[ nErrorID ];
}

void KClientErr::SetLastError( int nError_ )
{
    KLocker lock( csKClientErr );

    DWORD dwThreadID = ::GetCurrentThreadId();

    mit = mapLastError.find( dwThreadID );
    if( mit == mapLastError.end() ) // 새로운 스레드의 접근
    {
        mapLastError.insert( std::make_pair( dwThreadID, nError_ ) );
        return;
    }

    mit->second = nError_;  // 기존 스레드의 값 변경.
}

int KClientErr::GetLastError()
{
    KLocker lock( csKClientErr );

    mit = mapLastError.find( ::GetCurrentThreadId() );

    if( mit == mapLastError.end() ) // 새로운 스레드의 접근
        return 0;

    return mit->second;
}

wchar_t* KClientErr::GetLastErrMsg()
{
    KLocker lock( csKClientErr );

    return GetErrStr( GetLastError() ); 
}
