#include "stdafx.h"
#include "KncP2PError.h"

namespace KncP2PError
{
#   undef KNC_P2P_ERROR
#   define KNC_P2P_ERROR( id, comment ) #comment,
    char* szErrorStr[] = {
#include "KncP2PErrorDef.h"
    };

    char* GetErrStr( int nErrorID )
    {
        if( nErrorID >= KNC_P2P_ERROR_END || nErrorID < 0 ) return szErrorStr[ KNC_P2P_ERROR_END ];
        return szErrorStr[ nErrorID ];
    }

    int nLastError = KNC_OK;

    void    SetLastError( int nError_ )     { nLastError = nError_; }
    int     GetLastError()                  { return nLastError; }
    char*   GetLastErrMsg()                 { return GetErrStr( nLastError ); }
}