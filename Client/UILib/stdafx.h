// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

//#include <vector>
//#include <string>
//#include <map>
//#include <queue>
//#include <sstream>
#include "../stdafx_stl.h"
#include "../GCUTIL/KJohnGen.h"

#include "../GCDeviceLib/GCUITexture.h"

// lua
#include "KLuabinder.h"
#include "KLuaManager.h"

#include <assert.h>

extern KLuabinder g_kLuabinder;

template <class TType>
void KStrToType(const char* pcString, TType& tVal)
{
	std::istringstream is;
	is.str(pcString);
	is >> tVal;
}

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
