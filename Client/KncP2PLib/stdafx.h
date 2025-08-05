#pragma once


#undef _WIN32_IE
#undef WINVER
#undef _WIN32_WINDOWS
#undef _WIN32_WINNT
#define WINVER 0x0500
#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT 0x0500

#include <WinSock2.h>


#include "../stdafx_stl.h"
#include "../stdafx_boost.h"

#include "../GCEnum.h"

