#ifndef _GC_STRING_H_
#define _GC_STRING_H_

//#include <string>

#if defined(_UNICODE) || defined(UNICODE)
typedef std::wstring GCString;
#else
typedef std::string GCString;
#endif // _UNICODE || UNICODE

#endif // _GC_STRING_H_