#pragma once

namespace NetError
{

#   undef NET_ERROR
#   define NET_ERROR( id, comment ) id,

    enum PATCHER_MSG
    {
#if defined(_TAIWAN) || defined( _TAIWAN_TEST )  //테스트 서버에서 언어 문제로
#include "NetError_def_tw.h"
#elif defined(_HONGKONG) || defined( _HONGKONG_TEST )  //테스트 서버에서 언어 때문에
#include "NetError_def_hk.h"
#elif defined(_LEVELUP_BRAZIL) || defined( _BRAZIL_TEST )   //테스트 서버에서 언어선택을 위해서
#include "NetError_def_br.h"
#elif defined( _USA ) || defined( _USA_TEST )
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

    void InitializeCS();    //050613. microcat.
    void DeleteCS();        //050613. microcat.

    wchar_t* GetErrStr( int nErrorID );

    void SetLastError( int nError );
    wchar_t* GetLastErrMsg();
/*    int GetLastError();*/
}

#define SET_ERROR( id ) NetError::SetLastError( NetError::##id )