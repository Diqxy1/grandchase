#pragma once

namespace KClientErr
{
#   undef _ENUM
#   define _ENUM( id, comment ) id,

    enum CLIENT_ERROR 
    {
#include "ClientErr_def.h"
    };

    
    enum EM_LOADING_ERROR
    {
        EM_LOADING_TIME_OUT1 = 0,
        EM_LOADING_TIME_OUT2,
        EM_LOADING_TIME_OUT3,
        EM_LOADING_P2P_CONNECT,
    };
    
    wchar_t* GetErrStr( int nErrorID );

    void SetLastError( int nError );
    wchar_t* GetLastErrMsg();
    int GetLastError();
};

namespace KFailRate
{
#ifndef __NEVER_DEFINED_MACRO__
#define __NEVER_DEFINED_MACRO__
#include "FailRate_def.h"
#undef  __NEVER_DEFINED_MACRO__
#else
#include "FailRate_def.h"
#endif
};

namespace KNetWorkErr
{
#ifndef __NEVER_DEFINED_MACRO__
#define __NEVER_DEFINED_MACRO__
#include "NetWorkErr_def.h"
#undef  __NEVER_DEFINED_MACRO__
#else
#include "NetWorkErr_def.h"
#endif
};
