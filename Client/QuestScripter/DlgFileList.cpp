// DlgFileList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgFileList.h"
#include ".\dlgfilelist.h"
#include "DlgDirectorySetting.h"
//#include "KLuaManager.h"
#include "FunctionToScriptFile.h"

// CDlgFileList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgFileList, CDialog)
CDlgFileList::CDlgFileList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileList::IDD, pParent)
{
}

CDlgFileList::~CDlgFileList()
{
}

void CDlgFileList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE_LIST, m_lcFileList);
	DDX_Control(pDX, IDC_BUTTON_FOLDER_ADD, m_btFolderAdd);
	DDX_Control(pDX, IDC_BUTTON_FILE_ADD, m_btFileAdd);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btDelete);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_LIST, m_btClearList);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_btLoad);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btSave);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btClose);
	DDX_Control(pDX, IDC_BUTTON_SAVE_TO_SERVER_SCRIPT, m_btServerScript);
}

BEGIN_MESSAGE_MAP(CDlgFileList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER_ADD, OnBnClickedButtonFolderAdd)
	ON_BN_CLICKED(IDC_BUTTON_FILE_ADD, OnBnClickedButtonFileAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_LIST, OnBnClickedButtonClearList)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedButtonClose)
	ON_WM_SIZE()
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE_LIST, OnNMDblclkListFileList)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_SERVER_SCRIPT, OnBnClickedButtonSaveToSscript)
END_MESSAGE_MAP()

// CDlgFileList 메시지 처리기입니다.

void CDlgFileList::OnBnClickedButtonFolderAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPITEMIDLIST pidlBrowse;
	TCHAR tmp[10240];

	BROWSEINFO BRinfo;
	BRinfo.hwndOwner = GetSafeHwnd();						// 부모 윈도우의 핸들
	BRinfo.pidlRoot = NULL;									// 브라우징 할 루트 디렉토리
	BRinfo.pszDisplayName = tmp;							// 선택할 디렉토리가 저장될 버퍼
	BRinfo.lpszTitle = L"디렉토리를 선택하세요";				// 출력 문자열
	BRinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;  // 디렉토리 선택 옵션
	BRinfo.lpfn = NULL;											// 이벤트에 대한 사용자 정의 처리 함수
	BRinfo.lParam = 0;											// 처리 함수에 넘겨질 인수

	pidlBrowse = SHBrowseForFolder(&BRinfo); // 다이얼로그를 띄운다.

	m_lcFileList.ShowWindow(SW_HIDE);
	if (pidlBrowse != NULL) {
		SHGetPathFromIDList(pidlBrowse, tmp); // 패스를 얻어온다.

		CString strPath;
		strPath.Format(L"%s\\*.*", tmp);

		AddFolderFileList(strPath);
	}
	m_lcFileList.ShowWindow(SW_SHOW);
}

void CDlgFileList::OnBnClickedButtonFileAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR Filter[] = L"모든파일(*.*)|*.*|";
	CFileDialog dlg(TRUE, L"*.*(*.*)", L"*.*", OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, Filter, NULL);

	CString strPath = SiCDlgDirectorySetting()->m_strQuestScriptPath;
	dlg.m_ofn.lpstrInitialDir = (LPTSTR)(strPath.GetBuffer());

	try
	{
		if (IDOK == dlg.DoModal())
		{
			AddList(dlg.GetFileName().GetBuffer());
		}
	}
	catch (...)
	{
		MessageBox(L"파일을 불러오던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요.", L"알림", MB_OK);
	}
}

void CDlgFileList::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos;
	while (pos = m_lcFileList.GetFirstSelectedItemPosition())
	{
		DeleteList(m_lcFileList.GetNextSelectedItem(pos));
	}
}

void CDlgFileList::OnBnClickedButtonClearList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_vtFileList.clear();
	RefreshFileList();
}

void CDlgFileList::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR Filter[] = L"모든파일(*.*)|*.*|";
	CFileDialog dlg(TRUE, L"*.*(*.*)", L"*.*", OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, Filter, NULL);

	CString strPath = SiCDlgDirectorySetting()->m_strQuestScriptPath;
	dlg.m_ofn.lpstrInitialDir = (LPTSTR)(strPath.GetBuffer());

	if (IDOK == dlg.DoModal())
	{
		KLuaManager luaMgr;

		std::string strTemp = GCUTIL_STR::GCStrWideToChar(dlg.GetPathName());
		if (SUCCEEDED(luaMgr.DoFile(strTemp.c_str())))
		{
			if (SUCCEEDED(luaMgr.BeginTable("EditFileList")))
			{
				m_lcFileList.ShowWindow(SW_HIDE);
				for (int i = 1;; ++i)
				{
					CStringA strFileName;
					strFileName.Format("FileName%d", i - 1);

					std::string strName;
					if (FAILED(luaMgr.GetValue(strFileName.GetBuffer(), strName))) break;

					CString strNameWide = GCUTIL_STR::GCStrCharToWide(strName.c_str());

					if (!AddList(strNameWide)) break;
				}
				m_lcFileList.ShowWindow(SW_SHOW);
				LUA_END_TABLE(return)
			}
		}
	}
}

void CDlgFileList::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog dlg(false, L"*.stg", NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, L"STG File(*.stg)|*.stg|", NULL);
	if (dlg.DoModal() == IDOK)
	{
		HANDLE hWrite = CreateFile(dlg.GetPathName(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE == hWrite)
		{
			MessageBox(L"저장 할 수 없습니다. 파일이름을 변경해보시거나 디스크 공간을 확인해 보세요.", L"알림", MB_OK);
			return;
		}

		int iTab = 0;
		CString strBraceTitle;
		WriteOpenBrace(hWrite, "EditFileList", iTab);
		for (int i = 0; i < (int)m_vtFileList.size(); ++i)
		{
			CStringA strFileName;
			strFileName.Format("FileName%d", i);
			WriteToFileScript(hWrite, strFileName, m_vtFileList[i].first, iTab + 2);
		}
		WriteCloseBrace(hWrite, iTab, 0);
		CloseHandle(hWrite);
	}
}

void CDlgFileList::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnCancel();
}

void CDlgFileList::OnSize(UINT nType, int cx, int cy)
{
	CRect rtWindow;
	GetWindowRect(rtWindow);
	if (rtWindow.Width() < FL_MIN_WINDOW_SIZE_X)
	{
		rtWindow.right = rtWindow.left + FL_MIN_WINDOW_SIZE_X + 5;
		MoveWindow(rtWindow, 1);
		return;
	}
	else if (rtWindow.Height() < FL_MIN_WINDOW_SIZE_Y)
	{
		rtWindow.bottom = rtWindow.top + FL_MIN_WINDOW_SIZE_Y + 5;
		MoveWindow(rtWindow, 1);
	}

	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_lcFileList.GetSafeHwnd())
	{
		CRect rtClient;
		GetClientRect(rtClient);
		rtClient.left += 10;
		rtClient.right -= 10;
		rtClient.top += 10;
		rtClient.bottom -= 10;

		CRect rtListControl = rtClient;
		rtListControl.bottom -= 30;
		m_lcFileList.MoveWindow(rtListControl, 1);

		CRect rtButton = rtClient;
		rtButton.top = rtButton.bottom - 20;
		rtButton.right = rtButton.left + 70;
		m_btFolderAdd.MoveWindow(rtButton, 1);

		rtButton.OffsetRect(80, 0);
		m_btFileAdd.MoveWindow(rtButton, 1);

		rtButton.OffsetRect(80, 0);
		m_btDelete.MoveWindow(rtButton, 1);

		rtButton.OffsetRect(80, 0);
		m_btClearList.MoveWindow(rtButton, 1);

		rtButton.right = rtClient.right;
		rtButton.left = rtButton.right - 40;
		rtButton.OffsetRect(-10, 0);
		m_btClose.MoveWindow(rtButton, 1);

		rtButton.OffsetRect(-50, 0);
		m_btSave.MoveWindow(rtButton, 1);

		rtButton.OffsetRect(-50, 0);
		m_btLoad.MoveWindow(rtButton, 1);

		rtButton.OffsetRect(-50, 0);
		rtButton.left = rtButton.right - 90;
		m_btServerScript.MoveWindow(rtButton, 1);
	}
}

bool CDlgFileList::AddList(CString strFileName)
{
	strFileName.Replace('\\', '/');
	m_vtFileList.push_back(std::make_pair(strFileName, COleDateTime::GetCurrentTime()));

	TCHAR szFileName[256], szEXT[256];
	_wsplitpath(m_vtFileList[m_vtFileList.size() - 1].first.GetBuffer(), 0, 0, szFileName, szEXT);
	CString strOnlyFileName;
	strOnlyFileName = szFileName;
	strOnlyFileName += szEXT;

	// 동일한 파일이 있나 검색해본다.
	LVFINDINFO fi;
	fi.flags = LVFI_STRING;
	fi.psz = strOnlyFileName.GetBuffer();
	fi.vkDirection = VK_DOWN;
	if (-1 != m_lcFileList.FindItem(&fi))
	{
		if (IDYES == MessageBox(L"동일한 파일이 존재합니다. 동일한 파일은 추가할 수 없습니다. 계속 추가를 진행하시 겠습니까?", L"알림", MB_YESNO))
			return true;

		return false;
	}

	// ListControl 추가.
	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = (int)m_vtFileList.size() - 1;
	lvitem.iSubItem = 0;
	lvitem.pszText = strOnlyFileName.GetBuffer();
	m_lcFileList.InsertItem(&lvitem);

	CString strTime;
	strTime.Format(L"%02d:%02d:%02d", m_vtFileList[m_vtFileList.size() - 1].second.GetHour()
		, m_vtFileList[m_vtFileList.size() - 1].second.GetMinute()
		, m_vtFileList[m_vtFileList.size() - 1].second.GetSecond());

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = (int)m_vtFileList.size() - 1;
	lvitem.iSubItem = 1;
	lvitem.pszText = strTime.GetBuffer();
	m_lcFileList.SetItem(&lvitem);

	return true;
}

void CDlgFileList::AddFolderFileList(CString strFolder)
{
	CFileFind finder;
	SHFILEINFO shInfo;

	if (finder.FindFile(strFolder) == TRUE)
	{
		BOOL bFlag = true;
		while (bFlag)
		{
			bFlag = finder.FindNextFile();
			SHGetFileInfo(finder.GetFilePath(), 0, &shInfo, sizeof(SHFILEINFO)
				, SHGFI_TYPENAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX);

			if (!finder.IsDots()                           //'.'과'..'은 뛰어 넘는다.
				&& !finder.IsHidden())
			{
				if (!(shInfo.dwAttributes & SFGAO_FOLDER))
				{
					if (!AddList(finder.GetFilePath())) return;    // false 이면 더이상 추가 하지 않고 리턴.
				}
			}
		}
	}
}

void CDlgFileList::InsertList(int iIndex, CString strFileName)
{
	m_vtFileList.insert(m_vtFileList.begin() + iIndex, std::make_pair(strFileName, COleDateTime::GetCurrentTime()));

	// ListControl 추가.
	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iIndex;
	lvitem.iSubItem = 0;
	lvitem.pszText = m_vtFileList[iIndex].first.GetBuffer();
	m_lcFileList.InsertItem(&lvitem);

	CString strTime;
	strTime.Format(L"%02d:%02d:%02d", m_vtFileList[iIndex].second.GetHour()
		, m_vtFileList[iIndex].second.GetMinute()
		, m_vtFileList[iIndex].second.GetSecond());

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iIndex;
	lvitem.iSubItem = 1;
	lvitem.pszText = strTime.GetBuffer();
	m_lcFileList.SetItem(&lvitem);
}

void CDlgFileList::DeleteList(int iIndex)
{
	if ((int)m_vtFileList.size() <= iIndex || iIndex < 0) return;

	m_vtFileList.erase(m_vtFileList.begin() + iIndex);
	m_lcFileList.DeleteItem(iIndex);
}

void CDlgFileList::ClearList(void)
{
	m_vtFileList.clear();
	m_lcFileList.DeleteAllItems();
}

void CDlgFileList::RefreshFileList(void)
{
	m_lcFileList.DeleteAllItems();

	m_lcFileList.ShowWindow(SW_HIDE);
	for (int i = 0; i < (int)m_vtFileList.size(); ++i)
	{
		LV_ITEM lvitem;
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		lvitem.pszText = m_vtFileList[i].first.GetBuffer();
		m_lcFileList.InsertItem(&lvitem);

		CString strTime;
		strTime.Format(L"%02d:%02d:%02d", m_vtFileList[i].second.GetHour()
			, m_vtFileList[i].second.GetMinute()
			, m_vtFileList[i].second.GetSecond());

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = i;
		lvitem.iSubItem = 1;
		lvitem.pszText = strTime.GetBuffer();
		m_lcFileList.SetItem(&lvitem);
	}
	m_lcFileList.ShowWindow(SW_SHOW);
}

void CDlgFileList::SwapFileList(int iFirst, int iSecond)
{
	if ((int)m_vtFileList.size() <= iFirst || (int)m_vtFileList.size() <= iSecond
		|| iFirst < 0 || iSecond < 0)
		return;

	swap(m_vtFileList[iFirst], m_vtFileList[iSecond]);
}

BOOL CDlgFileList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LVCOLUMN vColum[2];

	CRect rtClient;
	GetClientRect(rtClient);

	vColum[0].mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	vColum[0].fmt = LVCFMT_LEFT;
	vColum[0].pszText = L"FileName";
	vColum[0].iSubItem = 0;
	vColum[0].cx = (int)((rtClient.Width() / 100.0f) * 60);

	vColum[1].mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	vColum[1].fmt = LVCFMT_LEFT;
	vColum[1].pszText = L"LoadTime";
	vColum[1].iSubItem = 1;
	vColum[1].cx = (int)((rtClient.Width() / 100.0f) * 40);

	m_lcFileList.InsertColumn(0, &vColum[0]);
	m_lcFileList.InsertColumn(1, &vColum[1]);

	CRect rtWindow;
	GetWindowRect(rtWindow);
	rtWindow.right = rtWindow.left + FL_MIN_WINDOW_SIZE_X + 5;
	rtWindow.bottom = rtWindow.top + FL_MIN_WINDOW_SIZE_Y + 5;
	MoveWindow(rtWindow, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgFileList::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UINT  uNumFiles;
	TCHAR szNextFile[10240];

	// Get the # of files being dropped.
	uNumFiles = DragQueryFile(hDropInfo, -1, NULL, 0);

	m_lcFileList.ShowWindow(SW_HIDE);
	for (UINT uFile = 0; uFile < uNumFiles; uFile++)
	{
		// Get the next filename from the HDROP info.
		if (DragQueryFile(hDropInfo, uFile, szNextFile, 10240) > 0)
		{
			SHFILEINFO shInfo;
			SHGetFileInfo(szNextFile, 0, &shInfo, sizeof(SHFILEINFO)
				, SHGFI_TYPENAME | SHGFI_ATTRIBUTES | SHGFI_SYSICONINDEX);

			if (shInfo.dwAttributes & SFGAO_FOLDER)
			{
				CString strPath;
				strPath.Format(L"%s\\*.*", szNextFile);

				AddFolderFileList(strPath);
			}
			else
			{
				if (!AddList(szNextFile)) break;      // false 이면 더이상 추가하지 않고 리턴.
			}
		}
	}
	m_lcFileList.ShowWindow(SW_SHOW);

	// Free up memory.
	DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CDlgFileList::OnNMDblclkListFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (GetParent())
	{
		POSITION pos = m_lcFileList.GetFirstSelectedItemPosition();
		int iItem = m_lcFileList.GetNextSelectedItem(pos);
		GetParent()->PostMessage(WM_LOAD_FILE_FROM_FILE_LIST, iItem, 0);
	}

	*pResult = 0;
}

std::pair<CString, COleDateTime> CDlgFileList::GetItem(int iIndex)
{
	if ((int)m_vtFileList.size() > iIndex && iIndex >= 0)
	{
		return m_vtFileList[iIndex];
	}

	std::pair<CString, COleDateTime> blank;
	return blank;
}
void CDlgFileList::OnBnClickedButtonSaveToSscript()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (GetParent())
	{
		GetParent()->PostMessage(WM_SAVE_SERVER_SCRIPT_FROM_FILE_LIST, 0, 0);
	}
}