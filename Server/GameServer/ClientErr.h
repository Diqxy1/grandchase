#pragma once

namespace KClientErr
{
#   undef _ENUM
#   define _ENUM( id, comment ) id,

    enum CLIENT_ERROR 
    {
#include "ClientErr_def.h"
    };

    
    wchar_t* GetErrStr( int nErrorID );

    void SetLastError( int nError );
    wchar_t* GetLastErrMsg();
    int GetLastError();
};
