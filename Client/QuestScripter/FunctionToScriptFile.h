#pragma once
#include "stdafx.h"
//#include <Windows.h>
//#include <string>

static void WriteToFileScript(HANDLE hWrite, CStringA strName, bool value, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s\t= %d", strName, value);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + ",\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript(HANDLE hWrite, CStringA strName, int value, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s\t= %d", strName, value);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + ",\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript(HANDLE hWrite, CStringA strName, float value, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s\t= %f", strName, value);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + ",\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript(HANDLE hWrite, CStringA strName, char *value, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s\t= \"%s\"", strName, value);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + ",\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript(HANDLE hWrite, CStringA strName, CStringA value, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s\t= \"%s\"", strName, value);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + ",\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript(HANDLE hWrite, CStringA strName, std::string value, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s\t= \"%s\"", strName, value.c_str());
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + ",\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript_NoEnter(HANDLE hWrite, CStringA strName, int iValue, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s = %d,", strName, iValue);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript_OnlyValue_String(HANDLE hWrite, std::string value, int iTab, bool IsEnter)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("\"%s\", ", value.c_str());
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	if (IsEnter)
		strResult = strResult + "\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript_OnlyValue_Number(HANDLE hWrite, float fValue, int iTab, bool IsEnter)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%f, ", fValue);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	if (IsEnter)
		strResult = strResult + "\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript_OnlyValue_Number(HANDLE hWrite, int iValue, int iTab, bool IsEnter)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%d, ", iValue);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	if (IsEnter)
		strResult = strResult + "\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteToFileScript_WithOut_QTMark(HANDLE hWrite, CStringA strName, CStringA value, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s\t= %s", strName, value);
	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + ",\r\n";
	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteOpenBrace(HANDLE hWrite, CStringA strName, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s =\r\n", strName);

	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult + "\t";
	strResult = strResult + "{\r\n";

	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteCloseBrace(HANDLE hWrite, int iTab, int cnt)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult = "}";

	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;

	if (0 == iTab)
		strResult = strResult + "\r\n";
	else
		strResult = strResult + ",\r\n";

	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteCloseBrace_NoEnter(HANDLE hWrite, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult = "}, ";

	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;

	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteOpenBrace_NoEnter(HANDLE hWrite, CStringA strName, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s = ", strName);

	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult + "\t";
	strResult = strResult + "{ ";

	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteOpenBrace_NoEnterAndNoBackTab(HANDLE hWrite, CStringA strName, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;
	strResult.Format("%s = ", strName);

	for (int i = 0; i < (int)iTab; i++)
		strResult = "\t" + strResult;
	strResult = strResult + "{ ";

	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

static void WriteOnlyOpenBrace_NoEnter(HANDLE hWrite, int iTab)
{
	if (!hWrite) return;

	DWORD dwSize;
	CStringA strResult;

	for (int i = 0; i < (int)iTab; i++)
		strResult = strResult + "\t";
	strResult = strResult + "{ ";

	WriteFile(hWrite, strResult.GetBuffer(), strResult.GetLength(), &dwSize, 0);
}

// static void WriteToFileScript(HANDLE hWrite, CStringW strName, bool value, int iTab)
// {
//     CStringA strNameA = GCUTIL_STR::GCStrWideToChar ( strName.GetBuffer() );
//
//     WriteToFileScript( hWrite, strNameA, value, iTab );
// }
//
// static void WriteToFileScript(HANDLE hWrite, CStringW strName, int value, int iTab)
// {
//     CStringA strNameA = GCUTIL_STR::GCStrWideToChar ( strName.GetBuffer() );
//
//     WriteToFileScript( hWrite, strNameA, value, iTab );
// }
//
// static void WriteToFileScript(HANDLE hWrite, CStringW strName, float value, int iTab)
// {
//     CStringA strNameA = GCUTIL_STR::GCStrWideToChar ( strName.GetBuffer() );
//
//     WriteToFileScript( hWrite, strNameA, value, iTab );
// }

static void WriteToFileScript(HANDLE hWrite, CStringA strName, CStringW value, int iTab)
{
	//    CStringA strNameA = GCUTIL_STR::GCStrWideToChar ( strName.GetBuffer() );
	std::string valueA = GCUTIL_STR::GCStrWideToChar(value.GetBuffer());

	WriteToFileScript(hWrite, strName, valueA, iTab);
}

static void WriteToFileScript(HANDLE hWrite, CStringA strName, std::wstring value, int iTab)
{
	//    CStringA strNameA = GCUTIL_STR::GCStrWideToChar ( strName.GetBuffer() );
	std::string valueA = GCUTIL_STR::GCStrWideToChar(value.c_str());

	WriteToFileScript(hWrite, strName, valueA, iTab);
}

static void WriteToFileScript_WithOut_QTMark(HANDLE hWrite, CStringA strName, CStringW value, int iTab)
{
	//    CStringA strNameA = GCUTIL_STR::GCStrWideToChar ( strName.GetBuffer() );
	CStringA valueA = GCUTIL_STR::GCStrWideToChar(value.GetBuffer());

	WriteToFileScript_WithOut_QTMark(hWrite, strName, valueA, iTab);
}

static void WriteOpenBrace(HANDLE hWrite, CString strName, int iTab)
{
	CStringA strNameA = GCUTIL_STR::GCStrWideToChar(strName.GetBuffer());
	WriteOpenBrace(hWrite, strNameA, iTab);
}

static void WriteOpenBrace_NoEnterAndNoBackTab(HANDLE hWrite, CString strName, int iTab)
{
	CStringA strNameA = GCUTIL_STR::GCStrWideToChar(strName.GetBuffer());
	WriteOpenBrace_NoEnterAndNoBackTab(hWrite, strNameA, iTab);
}

static void WriteOpenBrace_NoEnter(HANDLE hWrite, CString strName, int iTab)
{
	CStringA strNameA = GCUTIL_STR::GCStrWideToChar(strName.GetBuffer());
	WriteOpenBrace_NoEnter(hWrite, strNameA, iTab);
}

static CString MiddleBraceSidebyString(CString str)
{
	str = L"[" + str;
	str = str + L"]";

	return str;
}