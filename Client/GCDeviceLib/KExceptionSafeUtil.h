#pragma once

#ifdef KTDGDEVICEFONT_SIZE_CACHE
namespace   KExceptionSafeUtil
{
	bool    ESCopyMemory( void* destination, const void* source, size_t length ) throw();

	bool    ESCopyMemoryWithException( void* destination, const void* source, size_t length ) throw();
}
#endif
