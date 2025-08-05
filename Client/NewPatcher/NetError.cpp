#include "stdafx.h"
#include "NetError.h"

#include <map>
#include <Windows.h>    // DWORD, GetCurrentThreadID
#include "Thread/Locker.h"

namespace NetError  // 변수 선언은 namespace scope를 열고 해주어야 한다.
{

#undef NET_ERROR
#define NET_ERROR( id, comment ) L#comment,
    wchar_t* szErrorStr[] = {
#if defined(_TAIWAN) || defined( _TAIWAN_TEST )  //테스트 서버에서 언어 문제로
#include "NetError_def_tw.h"
#elif defined(_HONGKONG) || defined( _HONGKONG_TEST )  //테스트 서버에서 언어 때문에
#include "NetError_def_hk.h"
#elif defined(_LEVELUP_BRAZIL) || defined( _BRAZIL_TEST )   //테스트 서버에서 언어선택을 위해서
#include "NetError_def_br.h"
#elif defined( _USA ) || defined( _USA_TEST ) || defined( _PHILIPPINE ) || defined( _PHILIPPINE )
#include "NetError_def_en.h"
#elif defined(_THAILAND) || defined(_THAILAND_TEST)
#include "NetError_def_th.h"
#elif defined(_INDONESIA ) || defined(_INDONESIA_TEST)
#include "NetError_def_id.h"
#elif defined(_LATINAMERICA ) || defined(_LATINAMERICA_TEST)
#include "NetError_def_sa.h"
#elif defined(_EU ) || defined(_EU_TEST)
#include "NetError_def_eu.h"
#elif defined(_CHINA) || defined( _CHINA_TEST)
#include "NetError_def_cn.h"
#else
#include "NetError_def.h"
#endif
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

// int NetError::GetLastError()
// {
//     KLocker lock( csNetError );
// 
//     mit = mapLastError.find( ::GetCurrentThreadId() );
// 
//     if( mit == mapLastError.end() ) // 새로운 스레드의 접근
//         return 0;
// 
//     return mit->second;
// }
// 
// wchar_t* NetError::GetLastErrMsg()
// {
//     KLocker lock( csNetError );
// 
//     return GetErrStr( GetLastError() ); 
// }
