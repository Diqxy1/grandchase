// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

//#define OLD_ELS_KOM


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          if (p) { delete (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    if (p) { delete[] (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE_ARRAY

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define _CRTDBG_MAP_ALLOC  

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <Windows.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <string>
#include <sstream>
#include <ios>
#include <fstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "zlib/zlib.h"
#include "./Tinyxm/tinyxml.h"
#include <iomanip>

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <Windows.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale>

#include "CRC32/CRC_32.h"