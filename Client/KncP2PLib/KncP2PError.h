#ifndef _KNCP2PERROR_H_
#define _KNCP2PERROR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace KncP2PError
{

#   undef KNC_P2P_ERROR
#   define KNC_P2P_ERROR( id, comment ) id,

    enum KNC_P2P_ERROR_ENUM
    {
#include "KncP2PErrorDef.h"
    };

    char* GetErrStr( int nErrorID );

    void SetLastError( int nError );
    char* GetLastErrMsg();
    int GetLastError();
}

#ifdef SET_ERROR
#undef SET_ERROR
#endif
#define SET_ERROR( id ) KncP2PError::SetLastError( KncP2PError::##id )

#endif // _KNCP2PERROR_H_