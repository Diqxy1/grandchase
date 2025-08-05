#include "StdAfx.h"

#ifdef KTDGDEVICEFONT_SIZE_CACHE
bool    KExceptionSafeUtil::ESCopyMemory( void* destination, const void* source, size_t length ) throw()
{
	bool    bExceptionOccurred = false;

	try
	{
		CopyMemory( destination, source, length );
	}
	catch(...)
	{
		bExceptionOccurred = true;
	}

	return  bExceptionOccurred;
}//KExceptionSafeUtil::ESCopyMemory()

bool    KExceptionSafeUtil::ESCopyMemoryWithException( void* destination, const void* source, size_t length ) throw()
{
	bool    bExceptionOccurred = false;

	try
	{
		CopyMemory( destination, source, length );
		void* pData = NULL;
		ZeroMemory( pData, 1024 );
	}
	catch(...)
	{
		bExceptionOccurred = true;
	}

	return  bExceptionOccurred;
}//KExceptionSafeUtil::ESCopyMemory()
#endif
