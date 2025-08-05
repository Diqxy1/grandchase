#include "stdafx.h"
#include "NetError.h"

//
//


namespace NetError  // 변수 선언은 namespace scope를 열고 해주어야 한다.
{

#undef NET_ERROR
#define NET_ERROR( id, comment ) L#comment,
    wchar_t* szErrorStr[] = {
#include "NetError_def.h"
    };

    // key : thread id. element : last error
    std::map<DWORD, int> mapLastError;

    std::map<DWORD, int>::iterator mit;     // 빠른 작업을 위해..

    KncCriticalSection  csNetError;
}

wchar_t* NetError::GetErrStr( int nErrorID )
{
    KLocker lock( csNetError );

    if( nErrorID >= ERR_SENTINEL || nErrorID < 0 ) return szErrorStr[ ERR_SENTINEL ];
    return szErrorStr[ nErrorID ];
}

void NetError::SetLastError( int nError_ )
{
    KLocker lock( csNetError );

    DWORD dwThreadID = ::GetCurrentThreadId();

    mit = mapLastError.find( dwThreadID );
    if( mit == mapLastError.end() ) // 새로운 스레드의 접근
    {
        mapLastError.insert( std::make_pair( dwThreadID, nError_ ) );
        return;
    }

    mit->second = nError_;  // 기존 스레드의 값 변경.
}

int NetError::GetLastError()
{
    KLocker lock( csNetError );

    mit = mapLastError.find( ::GetCurrentThreadId() );

    if( mit == mapLastError.end() ) // 새로운 스레드의 접근
        return 0;

    return mit->second;
}

wchar_t* NetError::GetLastErrMsg()
{
    KLocker lock( csNetError );

    return GetErrStr( GetLastError() ); 
}
