// DlgDirectorySetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgDirectorySetting.h"
#include ".\dlgdirectorysetting.h"
#include "DlgDirectorySetting.h"
// CDlgDirectorySetting 대화 상자입니다.

TCHAR *strRegisterValueName[7][4] =
{
	{_T("Map_kr"),  _T("MapSubFile_kr"),	_T("Sound_kr"), _T("QuestScript_kr") },
	{_T("Map_tw"),	_T("MapSubFile_tw"),	_T("Sound_tw"),	_T("QuestScript_tw") },
	{_T("Map_hk"),	_T("MapSubFile_hk"),	_T("Sound_hk"),	_T("QuestScript_hk") },
	{_T("Map_br"),	_T("MapSubFile_br"),	_T("Sound_br"),	_T("QuestScript_br") },
	{_T("Map_us"),	_T("MapSubFile_us"),	_T("Sound_us"),	_T("QuestScript_us") },
	{_T("Map_ph"),	_T("MapSubFile_ph"),	_T("Sound_ph"),	_T("QuestScript_ph") },
	{_T("Map_th"),	_T("MapSubFile_th"),	_T("Sound_th"),	_T("QuestScript_th") },
};

ImplementSingleton(CDlgDirectorySetting);

IMPLEMENT_DYNAMIC(CDlgDirectorySetting, CDialog)
CDlgDirectorySetting::CDlgDirectorySetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDirectorySetting::IDD, pParent)
{
}

CDlgDirectorySetting::~CDlgDirectorySetting()
{
}

void CDlgDirectorySetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAP, m_ebMap);
	DDX_Control(pDX, IDC_EDIT_MAPSUBFILE, m_ebMapSubFile);
	DDX_Control(pDX, IDC_EDIT_SOUND, m_ebSound);
	DDX_Control(pDX, IDC_EDIT_QUESTSCRIPT, m_ebQuestScript);
	DDX_Control(pDX, IDC_COMBO_STATE_SORTING, m_cbState);
	m_ebMap.SetReadOnly(1);
	m_ebMap.SetBackColor(RGB(255, 255, 255));
	m_ebMap.SetTextColor(RGB(0, 0, 0));
	m_ebMapSubFile.SetReadOnly(1);
	m_ebMapSubFile.SetBackColor(RGB(255, 255, 255));
	m_ebMapSubFile.SetTextColor(RGB(0, 0, 0));
	m_ebSound.SetReadOnly(1);
	m_ebSound.SetBackColor(RGB(255, 255, 255));
	m_ebSound.SetTextColor(RGB(0, 0, 0));
	m_ebQuestScript.SetReadOnly(1);
	m_ebQuestScript.SetBackColor(RGB(255, 255, 255));
	m_ebQuestScript.SetTextColor(RGB(0, 0, 0));
}

BEGIN_MESSAGE_MAP(CDlgDirectorySetting, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MAP, OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BUTTON_SOUND, OnBnClickedButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_QUESTSCRIPT, OnBnClickedButtonQuestscript)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_MAPSUBFILE, OnBnClickedButtonMapsubfile)
	ON_CBN_SELENDOK(IDC_COMBO_STATE_SORTING, &CDlgDirectorySetting::OnCbnSelendokComboStateSorting)
END_MESSAGE_MAP()

// CDlgDirectorySetting 메시지 처리기입니다.

// this is how we set the initial directory
static int CALLBACK MyBrowseCallbackProc(
	HWND hwnd,
	UINT uMsg,
	LPARAM lParam,
	LPARAM lpData
)
{
	switch (uMsg) {
	case BFFM_INITIALIZED:
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

void CDlgDirectorySetting::OpenDirectoryDialog(CString *pstrResult, CString strTitle, CString strPath)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	LPITEMIDLIST pidl = NULL;
	TCHAR pszPathname[2048]; // 경로 저장을 위한 변수
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.lpszTitle = strTitle.GetBuffer();
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	BrInfo.lpfn = MyBrowseCallbackProc;
	BrInfo.pszDisplayName = strPath.GetBuffer();
	BrInfo.lParam = (LPARAM)strPath.GetBuffer();

	pidl = SHBrowseForFolder(&BrInfo);
	if (NULL != pidl)
	{
		SHGetPathFromIDList(pidl, pszPathname); // pszPathname : 디렉토리 위치가 저장이 됩니다.
		*pstrResult = pszPathname;
	}
}

void CDlgDirectorySetting::OnBnClickedButtonMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strResultPath;
	m_ebMap.GetWindowText(strResultPath);
	OpenDirectoryDialog(&strResultPath, L"맵_디렉토리를 선택하세요!", m_strMapPath);
	m_ebMap.SetWindowText(strResultPath);
}

void CDlgDirectorySetting::OnBnClickedButtonSound()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strResultPath;
	m_ebSound.GetWindowText(strResultPath);
	OpenDirectoryDialog(&strResultPath, L"사운드_디렉토리를 선택하세요!", m_strSoundPath);
	m_ebSound.SetWindowText(strResultPath);
}

void CDlgDirectorySetting::OnBnClickedButtonQuestscript()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strResultPath;
	m_ebQuestScript.GetWindowText(strResultPath);
	OpenDirectoryDialog(&strResultPath, L"퀘스트스크립트_디렉토리를 선택하세요!", m_strQuestScriptPath);
	m_ebQuestScript.SetWindowText(strResultPath);
}

void CDlgDirectorySetting::OnBnClickedButtonMapsubfile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strResultPath;
	m_ebMapSubFile.GetWindowText(strResultPath);
	OpenDirectoryDialog(&strResultPath, L"맵부가파일_디렉토리를 선택하세요!", m_strMapSubFilePath);
	m_ebMapSubFile.SetWindowText(strResultPath);
}

bool CDlgDirectorySetting::WriteToRegister(void)
{
	HKEY hkey;
	DWORD dwType = REG_DWORD;
	DWORD dwLength = sizeof(DWORD);

	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, L"KOG_QUESTSCRIPT_DIR",
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL))
	{
		// Write the window width/height.  This is just an example,
		// of how to use DXUtil_Write*() functions.

		CString strMap;
		CString strMapSubFile;
		CString strSound;
		CString strQuestScript;
		if (m_ebMap.GetSafeHwnd())             m_ebMap.GetWindowText(strMap);
		if (m_ebMapSubFile.GetSafeHwnd())      m_ebMapSubFile.GetWindowText(strMapSubFile);
		if (m_ebSound.GetSafeHwnd())           m_ebSound.GetWindowText(strSound);
		if (m_ebQuestScript.GetSafeHwnd())     m_ebQuestScript.GetWindowText(strQuestScript);

		if (!strMap.GetLength())
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\data\\map";
			strMap = strPath;
		}

		if (!strMapSubFile.GetLength())
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\data\\map";
			strMapSubFile = strPath;
		}

		if (!strSound.GetLength())
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\data\\Music";
			strSound = strPath;
		}

		if (!strQuestScript.GetLength())
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\data\\script";
			strQuestScript = strPath;
		}

		int selectState = m_cbState.GetCurSel();

		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strMap.GetString());
			std::string strRegValueName = GCUTIL_STR::GCStrWideToChar(strRegisterValueName[selectState][0]);
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}
		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strMapSubFile.GetString());
			std::string strRegValueName = GCUTIL_STR::GCStrWideToChar(strRegisterValueName[selectState][1]);
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}
		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strSound.GetString());
			std::string strRegValueName = GCUTIL_STR::GCStrWideToChar(strRegisterValueName[selectState][2]);
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}
		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strQuestScript.GetString());
			std::string strRegValueName = GCUTIL_STR::GCStrWideToChar(strRegisterValueName[selectState][3]);
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}
		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strMap.GetString());
			std::string strRegValueName = "Map_Directory";
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}
		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strMapSubFile.GetString());
			std::string strRegValueName = "MapSubFile_Directory";
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}
		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strSound.GetString());
			std::string strRegValueName = "Sound_Directory";
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}
		{
			std::string strRegValuePath = GCUTIL_STR::GCStrWideToChar(strQuestScript.GetString());
			std::string strRegValueName = "QuestScript_Directory";
			RegSetValueExA(hkey, strRegValueName.c_str(), 0, REG_SZ, (BYTE*)strRegValuePath.c_str(), strRegValuePath.length() + 1);
		}

		RegCloseKey(hkey);

		return true;
	}

	return false;
}

bool CDlgDirectorySetting::GetFromRegister(void)
{
	HKEY hkey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, L"KOG_QUESTSCRIPT_DIR",
		0, KEY_ALL_ACCESS, &hkey))
	{
		// Read the stored window width/height.  This is just an example,
		// of how to use DXUtil_Read*() functions.
		WCHAR szMap[2048];
		WCHAR szMapSubFile[2048];
		WCHAR szSound[2048];
		WCHAR szQuestScript[2048];
		DWORD data_size = 2048;
		int selectState = 0;
		if (m_cbState.m_hWnd != NULL)
			selectState = m_cbState.GetCurSel();

		if (ERROR_SUCCESS != RegQueryValueEx(hkey, strRegisterValueName[selectState][0], 0, 0, (BYTE*)szMap, &data_size))
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\data\\map";
			RegSetValueEx(hkey, L"Map_Directory", 0, REG_SZ, (BYTE*)strPath.GetBuffer(), strPath.GetLength());
			StringCchCopyN(szMap, sizeof(szMap), strPath.GetBuffer(), strPath.GetLength());
		}

		data_size = 2048;
		if (ERROR_SUCCESS != RegQueryValueEx(hkey, strRegisterValueName[selectState][1], 0, 0, (BYTE*)szMapSubFile, &data_size))
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\Data\\map";
			RegSetValueEx(hkey, L"MapSubFile_Directory", 0, REG_SZ, (BYTE*)strPath.GetBuffer(), strPath.GetLength());
			StringCchCopyN(szMapSubFile, sizeof(szMapSubFile), strPath.GetBuffer(), strPath.GetLength());
		}

		data_size = 2048;
		if (ERROR_SUCCESS != RegQueryValueEx(hkey, strRegisterValueName[selectState][2], 0, 0, (BYTE*)szSound, &data_size))
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\Data\\Music";
			RegSetValueEx(hkey, L"Sound_Directory", 0, REG_SZ, (BYTE*)strPath.GetBuffer(), strPath.GetLength());
			StringCchCopyN(szSound, sizeof(szSound), strPath.GetBuffer(), strPath.GetLength());
		}

		data_size = 2048;
		if (ERROR_SUCCESS != RegQueryValueEx(hkey, strRegisterValueName[selectState][3], 0, 0, (BYTE*)szQuestScript, &data_size))
		{
			CString strPath = GCUTIL_STR::GCStrCharToWide(g_Path);
			strPath += L"\\Data\\script";
			RegSetValueEx(hkey, L"QuestScript_Directory", 0, REG_SZ, (BYTE*)strPath.GetBuffer(), strPath.GetLength());
			StringCchCopyN(szQuestScript, sizeof(szQuestScript), strPath.GetBuffer(), strPath.GetLength());
		}

		if (m_ebMap.GetSafeHwnd())             m_ebMap.SetWindowText(szMap);
		if (m_ebMapSubFile.GetSafeHwnd())      m_ebMapSubFile.SetWindowText(szMapSubFile);
		if (m_ebSound.GetSafeHwnd())           m_ebSound.SetWindowText(szSound);
		if (m_ebQuestScript.GetSafeHwnd())     m_ebQuestScript.SetWindowText(szQuestScript);

		m_strMapPath = szMap;
		m_strMapSubFilePath = szMapSubFile;
		m_strSoundPath = szSound;
		m_strQuestScriptPath = szQuestScript;

		RegCloseKey(hkey);

		return true;
	}
	else
	{
		if (m_ebMap.GetSafeHwnd())             m_ebMap.SetWindowText(m_strMapPath);
		if (m_ebMapSubFile.GetSafeHwnd())      m_ebMapSubFile.SetWindowText(m_strMapSubFilePath);
		if (m_ebSound.GetSafeHwnd())           m_ebSound.SetWindowText(m_strSoundPath);
		if (m_ebQuestScript.GetSafeHwnd())     m_ebQuestScript.SetWindowText(m_strQuestScriptPath);
		WriteToRegister();
	}

	return false;
}

void CDlgDirectorySetting::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WriteToRegister();
	OnOK();
}

void CDlgDirectorySetting::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

BOOL CDlgDirectorySetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	TCHAR *strStateName[] =
	{
		_T("한국"),
		_T("대만"),
		_T("홍콩"),
		_T("브라질"),
		_T("북미"),
		_T("필리핀"),
		_T("태국"),
		_T("0")
	};
	m_cbState.ResetContent();
	for (int i = 0; strStateName[i][0] != L'0'; i++) {
		m_cbState.AddString(strStateName[i]);
	}

	m_cbState.SetCurSel(0);
	UpdateData(FALSE);

	GetFromRegister();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgDirectorySetting::OnCbnSelendokComboStateSorting()
{
	GetFromRegister();
}