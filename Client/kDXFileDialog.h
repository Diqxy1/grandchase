#pragma once
//#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_WINDOWS_API
#define BOOST_WINDOWS_PATH
//#define BOOST_FILESYSTEM_NARROW_ONLY
#define NUM_HISTROY_LIMIT	7		//최대 돌아가기 수
#include "stdafx.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

namespace fs = boost::filesystem;

class kFileDlgManager
{
public:
	kFileDlgManager();
	~kFileDlgManager();
	void GetSpecialfolder();
	void MoveToBrachDirectory();
	void SetSpecialfolder_MyDocumentDirectory(bool bHistory = true);
	void SetSpecialfolder_DesktopDirectory(bool bHistory = true);
	void SetSpecialfolder_DeviceList(bool bHistory = true);
	std::wstring GetCurrentDirectoryName();
	bool IsRootDirectory();
	std::wstring GetRootPath();
	bool CheckSpecialFolder();
	void SetPath(std::wstring strDirectory);
	bool MovePath(std::wstring strDirectory);
	void HistoryBack();
	std::wstring kFileDlgManager::string_replace_all(    
		const std::wstring& source,    
		const std::wstring& pattern,    
		const std::wstring& replace );

	enum eDirectory_TYPE
	{
		EDT_NORMAL			= 0,
		EDT_DEVICELIST		= 1,
		EDT_DESKTOPFOLDER   = 2,
		EDT_MYDOCUMENT		= 3,
	};

	eDirectory_TYPE GetCurrentDirectoryType();

	//void TestPrint(); //테스트 코드
	std::vector<std::wstring> GetVecAncestorList();

	//파일 목록 보냅니다.
	inline std::vector<std::wstring> GetFileList()
	{
		return m_vecFileList;
	}

	//디렉토리 목록 보냅니다.
	inline std::vector<std::wstring> GetDirectoryList()
	{
		return m_vecDirectoryList;
	}

    bool GetDiskInfo(wchar_t *pbuffer);
private:
	//젠장 Boost::FileSystem에 Path쪽에는 왜 멀티바이트만 되어 있는지 모르겠다...
	std::string m_strDirectroy; 
	eDirectory_TYPE m_eDType;
	std::wstring CheckString(std::wstring strDirectory);

private:
	void UpdateData(bool bHistory = true, bool bAddData = false);
    fs::wpath				 m_path;
	int						 m_nDirCount;
	int						 m_nFileCount;
	std::vector<std::wstring> m_vecFileList;
	std::vector<std::wstring> m_vecDirectoryList;
	std::deque<fs::wpath>	 m_deqHistroy;
	fs::wpath 				 m_DesktopPath1;
	fs::wpath 				 m_DesktopPath2;
	fs::wpath				 m_MyDocumentPath1;
	fs::wpath				 m_MyDocumentPath2;
};