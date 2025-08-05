#pragma once

#ifndef _WIN32_WINNT	             
#define _WIN32_WINNT 0x0501
#endif						

#define WIN32_LEAN_AND_MEAN

#if defined( INCLUDE_KOM_FOLDER )
 #include <afxwin.h>
 //#include <afxdisp.h>
#endif

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "zlib.h"
#include <stdafx_stl.h>
#include <stdafx_boost.h>
#include <locale>
#include "KncUtil.h"