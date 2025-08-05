#pragma once

namespace NetError
{
#   undef NET_ERROR
#   define NET_ERROR( id, comment ) id,

	enum NETWORK_ERROR
	{
#include "NetError_def.h"
	};

	void InitializeCS();    //050613. microcat.
	void DeleteCS();        //050613. microcat.

	wchar_t* GetErrStr(int nErrorID);

	void SetLastNetError(int nError);
	wchar_t* GetLastNetErrMsg();
	int GetLastNetError();
}

#define SET_ERROR( id ) NetError::SetLastNetError( NetError::##id )

#define ERR_CASE( id, var, data )   case NetError::##id:  var = data; break
#define ERR_CASE_ACTION( id, var, data, action )   case NetError::##id:  var = data; action; break

#define SET_ERR_GOTO( id, where )   do{ SET_ERROR( id ); goto where; }while(false)
#define SET_ERR_VAL( id, var, data )   do{ SET_ERROR( id ); var = data; }while(false)
#define IS_CORRECT( id )    (NetError::GetLastNetError() == NetError::##id)
