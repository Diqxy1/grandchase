#pragma once
#if defined(BUILD_BY_HUDSON)
#include "../hudsondef.h"
#endif

#include "stdafx.h"
#include "../kom/kom.h"



extern void LoadBitmapFromMassFile( Komfile * pMassFile,std::string strFileName_, CBitmap** ppBitmap_ );
extern void InstallCrashReport();
extern void BugTrap_InstallSehFilter();
bool CheckDXVersion();
std::string GetSystemFolderPath( int csidl );