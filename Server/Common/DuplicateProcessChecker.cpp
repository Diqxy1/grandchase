#include "stdafx.h"
#include "DuplicateProcessChecker.h"
#include <TlHelp32.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
#include "shlwapi.h"

CDuplicateProcessChecker::CDuplicateProcessChecker()
{
	if (false == CheckDuplicateProcess()) {
		::MessageBoxW(NULL, L"프로세스 확인 중 오류가 발생하였습니다", L"Error", MB_OK);
		exit(0);
	}
}

CDuplicateProcessChecker::~CDuplicateProcessChecker()
{
}

bool CDuplicateProcessChecker::CheckDuplicateProcess()
{
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	HANDLE hProcess = NULL;
	PROCESSENTRY32W pe32;

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32W me32;

	WCHAR szModule[MAX_MODULE_NAME32 + 1] = { 0 };
	WCHAR szExePath[MAX_PATH + 1] = { 0 };
	int nModuleSize = MAX_MODULE_NAME32;
	int nExePathSize = MAX_PATH;

	// 현재 실행하려는 프로세스의 이름과 경로를 가져온다
	DWORD dwPID = GetCurrentProcessId();

	if ((hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID)) != NULL) {
		nModuleSize = GetModuleBaseNameW(hProcess, NULL, szModule, nModuleSize);
		nExePathSize = GetModuleFileNameExW(hProcess, NULL, szExePath, nExePathSize);
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	szModule[nModuleSize] = 0;
	szExePath[nExePathSize] = 0;

	// 현재 시스템에 실행중인 프로세스 리스트에서 경로 중복을 확인한다
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) {
		::MessageBoxW(NULL, L"CreateToolhelp32Snapshot (of processes)", L"Error", MB_OK);
		return false;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32FirstW(hProcessSnap, &pe32)) {
		::MessageBoxW(NULL, L"Process32FirstW", L"Error", MB_OK);
		return false;
	}

	do {
		if (dwPID == pe32.th32ProcessID) {
			continue;
		}

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);

		if (StrCmpNW(szModule, pe32.szExeFile, nModuleSize) == 0) {
			hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
			if (INVALID_HANDLE_VALUE == hModuleSnap) {
				::MessageBoxW(NULL, L"CreateToolhelp32Snapshot (of modules)", L"Error", MB_OK);
				return false;
			}

			me32.dwSize = sizeof(MODULEENTRY32W);

			if (!Module32FirstW(hModuleSnap, &me32)) {
				::MessageBoxW(NULL, L"Module32FirstW", L"Error", MB_OK);
				return false;
			}

			do {
				// 프로세스 실행 경로는 프로세스 엔트리에서 확인할 수 없고 모듈 엔트리에서 확인 가능하다
				if (StrCmpNW(szExePath, me32.szExePath, nExePathSize) == 0) {
					::MessageBoxW(NULL, L"같은 경로의 서버가 이미 실행 중 입니다", L"Error", MB_OK);
					return false;
				}
			} while (Module32NextW(hModuleSnap, &me32));

			CloseHandle(hModuleSnap);
		}
	} while (Process32NextW(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	CloseHandle(hProcess);

	return true;
}