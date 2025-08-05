#pragma once
#include "../stdafx.h"
class FixWin10 {
public:
	LPDIRECT3D9 initFix();
	bool is8OrLater();
	bool IsRunningCompatMode();
};