#pragma once
#include <Windows.h>    // DWORD..
#include <vector>       // std::vector..

/// Exp를 레벨로 환산한다.
unsigned char Exp_2_Level(DWORD Exp);
bool IsCorrectGP(int iNowExp, int iChangeValue, bool bIsOpenTesop );
bool IsCorrectExp(int iNowExp, int iChangeValue, bool bIsInPCBang, bool bIsOpenTesop );

int GetMaxGP(int iNowExp, int iChangeValue, bool bIsOpenTesop );
int GetMaxExp(int iNowExp, int iChangeValue, bool bIsInPCBang, bool bIsOpenTesop );
DWORD GetGradePoint( const std::vector<DWORD>& Exp );
unsigned char GetGradeKor( const std::vector<DWORD>& Exp );