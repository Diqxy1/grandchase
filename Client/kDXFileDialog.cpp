#include "kDXFileDialog.h"
#include "shlobj.h"
#define BOOST_WINDOWS_PATH
#define FILEDLG_MAX_PATH 255
#define FILEDLG_MAX_BUFF 256
#define DEVICE_LIST_STRING	L"\\DeviceListAZDKSD"
#include <filesystem>

kFileDlgManager::kFileDlgManager(void)
{
	m_deqHistroy.clear();
    m_path = fs::initial_path<fs::wpath>(); //현재 폴더를 기본값으로.
    UpdateData();
}

kFileDlgManager::~kFileDlgManager(void)
{

}

std::wstring kFileDlgManager::string_replace_all(    
							const std::wstring& source,    
							const std::wstring& pattern,    
							const std::wstring& replace )   
{   
	std::wstring result = source;   
	std::wstring::size_type pos = 0;   
	std::wstring::size_type offset = 0;   
	std::wstring::size_type pattern_len = pattern.size();   
	std::wstring::size_type replace_len = replace.size();   

	while ( ( pos = result.find( pattern, offset ) ) != std::wstring::npos )   
	{   
		result.replace( result.begin() + pos,    
			result.begin() + pos + pattern_len,    
			replace );   
		offset = pos + replace_len;   
	}   
	return result;   
}  

void kFileDlgManager::SetSpecialfolder_MyDocumentDirectory(bool bHistory)
{
	wchar_t MyDocumentPath1[FILEDLG_MAX_PATH];
	wchar_t MyDocumentPath2[FILEDLG_MAX_PATH];

	//초기화
	m_nDirCount = 0;
	m_nFileCount = 0;
	m_vecFileList.clear();
	m_vecDirectoryList.clear();

	SHGetSpecialFolderPathW(NULL, MyDocumentPath1, CSIDL_COMMON_DOCUMENTS, FALSE);
	m_MyDocumentPath1 = m_path = fs::wpath(MyDocumentPath1);
	UpdateData(false,true);

	SHGetSpecialFolderPathW(NULL, MyDocumentPath2, CSIDL_PERSONAL, FALSE);
	m_MyDocumentPath2 = m_path = fs::wpath(MyDocumentPath2);
	UpdateData(bHistory,true);

	m_eDType = EDT_MYDOCUMENT;
	

}

void kFileDlgManager::SetSpecialfolder_DesktopDirectory(bool bHistory )
{
	wchar_t DeskTopPath1[FILEDLG_MAX_PATH];
	wchar_t DeskTopPath2[FILEDLG_MAX_PATH];

	//초기화
	m_nDirCount = 0;
	m_nFileCount = 0;
	m_vecFileList.clear();
	m_vecDirectoryList.clear();

	SHGetSpecialFolderPathW(NULL, DeskTopPath2, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
	m_DesktopPath2 = m_path = fs::wpath(DeskTopPath2);
	UpdateData(false,true);
	SHGetSpecialFolderPathW(NULL, DeskTopPath1, CSIDL_DESKTOPDIRECTORY, FALSE);
	m_DesktopPath1 = m_path = fs::wpath(DeskTopPath1);
	UpdateData(bHistory,true);
	m_eDType = EDT_DESKTOPFOLDER;
}
std::wstring kFileDlgManager::GetRootPath()
{
	return m_path.root_directory().wstring();
}
kFileDlgManager::eDirectory_TYPE kFileDlgManager::GetCurrentDirectoryType()
{
	return m_eDType;
}
std::wstring kFileDlgManager::GetCurrentDirectoryName()
{
	std::wstring strRet = L"";
	m_path = m_path.normalize();
	switch(m_eDType)
	{
		case EDT_NORMAL:
			strRet = m_path.wstring();
			break;
	}
	return strRet;
}

void kFileDlgManager::SetSpecialfolder_DeviceList(bool bHistory)
{
    m_nDirCount = 0;
    m_nFileCount = 0;
    m_vecFileList.clear();
    m_vecDirectoryList.clear();

    wchar_t szDriveBuf[FILEDLG_MAX_BUFF]={0x00,};
    UINT nCount=GetLogicalDriveStringsW(FILEDLG_MAX_BUFF,szDriveBuf);
    wchar_t *pBufIndex = szDriveBuf;
    bool bGetInfo = false;

    for(int i=0;i< (int)(nCount)/4;i++)
    {

        bGetInfo = GetDiskInfo(pBufIndex);

        if( bGetInfo )
        {
            m_vecDirectoryList.push_back(pBufIndex);
        }


        pBufIndex+= 4;
    }
    if (bHistory == true)
        m_deqHistroy.push_back(fs::wpath(DEVICE_LIST_STRING));
    m_eDType = EDT_DEVICELIST;
}

bool kFileDlgManager::IsRootDirectory()
{
	bool bRet = false;
	fs::wpath pathRoot = m_path.root_path();

	if (m_path == pathRoot)
		bRet = true;

	return bRet;
}

//상위 디렉토리로 이동 함수.
void kFileDlgManager::MoveToBrachDirectory()
{
	fs::wpath pathRoot;
	pathRoot = m_path.root_path();


	//내 문서에서 상위로 이동하면.. 바탕화면이다.
	if (m_eDType == EDT_MYDOCUMENT)
	{
		SetSpecialfolder_DesktopDirectory();
		return;
	}

	if (m_eDType == EDT_DEVICELIST) //내컴퓨터에서 상위 누르면 바탕화면으로
	{
		SetSpecialfolder_DesktopDirectory();
		return;
	}

	if (m_eDType == EDT_DESKTOPFOLDER) //바탕화면에서는 더 이상 못올라간다.
	{
		return;
	}

	if (m_path == pathRoot)	//루트 디렉토리인 경우 더이상 위로가지 않는다.
	{
		//루트인 경우 내컴퓨터 모드로 보내는 루틴 추가..
		SetSpecialfolder_DeviceList();
		return;
	}

	m_path = m_path.parent_path();
	UpdateData();
	CheckSpecialFolder();
}

//이상한 경로 입력시 필터링
std::wstring kFileDlgManager::CheckString(std::wstring strDirectory)
{
	std::wstring strNew=strDirectory;

	do
	{
		strDirectory = strNew;
		strNew = string_replace_all(strDirectory,L"\\\\",L"\\") ;
	}while(strNew!=strDirectory);

	do
	{
		strDirectory = strNew;
		strNew = string_replace_all(strDirectory,L"//",L"/") ;
	}while(strNew!=strDirectory);

	return strNew;
}

//경로 설정 함수
void kFileDlgManager::SetPath(std::wstring strDirectory)
{
	strDirectory=CheckString(strDirectory);
	m_path = fs::wpath(strDirectory);
	UpdateData();
}

// 특수 폴더인지 확인
bool kFileDlgManager::CheckSpecialFolder()
{
	if (m_path == m_DesktopPath1 || m_path == m_DesktopPath2)
	{
		SetSpecialfolder_DesktopDirectory(false);
		return true;
	}

	if (m_path == m_MyDocumentPath1 || m_path == m_MyDocumentPath2)
	{
		SetSpecialfolder_MyDocumentDirectory(false);
		return true;
	}

	if (m_path == fs::wpath(DEVICE_LIST_STRING))
	{
		SetSpecialfolder_DeviceList(false);
		return true;
	}
	
	return false;
}

//기존 경로에서 이동 하는 함수.
bool kFileDlgManager::MovePath(std::wstring strDirectory)
{
	strDirectory=CheckString(strDirectory);
	fs::wpath movePath;


	//내컴퓨터의 장치 목록인 경우 기존 경로를 사용하지 않는다.
	if (m_eDType == EDT_DEVICELIST)
	{
		movePath = fs::wpath(strDirectory);
	}
	else
	//내 문서에서 상위로 이동하면.. 바탕화면이다.
	if (m_eDType == EDT_MYDOCUMENT && strDirectory==L"..")
	{
		movePath = m_DesktopPath1;
	}
	else
	{
		movePath = m_path / fs::wpath(strDirectory);
	}
	
	bool bChk = false;

	if (!fs::exists(movePath) || !fs::is_directory( movePath) )
	{
		//이경로에는 없지만 바탕화면인 경우 2개의 경로를 사용하니까 그쪽 경로에 있는 파일인지 본다.
		if (m_path == m_DesktopPath1 || m_path == m_DesktopPath2)
		{
			if (m_path == m_DesktopPath1)
			{
				movePath = m_DesktopPath2 / fs::wpath(strDirectory);
			}

			if (m_path == m_DesktopPath2)
			{
				movePath = m_DesktopPath1 / fs::wpath(strDirectory);
			}
			if (!fs::exists(movePath) || !fs::is_directory( movePath) )
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	m_path = movePath;
	//m_path.normalize(); //상대 경로일 경우 사용시 날림.
	UpdateData();
	CheckSpecialFolder();
	return true;
}


//되돌리기 버튼 기능
void kFileDlgManager::HistoryBack()
{
	if (m_deqHistroy.size() <= 1) return;
	m_deqHistroy.pop_back();
	m_path = m_deqHistroy.back();
	if (CheckSpecialFolder() == false)
		UpdateData(false);
}

// Root까지의 목록을 모두 얻어오는 함수
std::vector<std::wstring> kFileDlgManager::GetVecAncestorList()
{
	fs::wpath pathRoot,brachPath;
	pathRoot = m_path.root_path();
	brachPath = m_path.parent_path();

	std::vector<std::wstring> vecResult;
	vecResult.clear();

	do 
	{
		vecResult.push_back(brachPath.wstring());
		brachPath = brachPath.parent_path();
	} while (brachPath == pathRoot);
	return vecResult;
}

void kFileDlgManager::UpdateData(bool bHistory, bool bAddData)
{
	if (bAddData==false)
	{
		m_nDirCount = 0;
		m_nFileCount = 0;
		m_vecFileList.clear();
		m_vecDirectoryList.clear();
	}
	
	//경로가 빈경우..
	if (m_path.empty()==true)
	{
		return;
	}

	//경로가 존재하지 않거나 디렉토리가 아닌경우.
	if (!fs::exists( m_path ) || !fs::is_directory( m_path ))
	{
		m_path = m_path.root_path();
		UpdateData();
		return;
	}

	//경로 히스토리에 저장.
	if (bHistory==true)
	{
		m_deqHistroy.push_back(m_path);
		if (m_deqHistroy.size()>NUM_HISTROY_LIMIT)
			m_deqHistroy.pop_front();
	}

    std::experimental::filesystem::directory_iterator end_iter;
    
	for ( std::experimental::filesystem::directory_iterator dir_itr( m_path.string() ); dir_itr != end_iter; ++dir_itr )
	{
		//std::string str = dir_itr->native_directory_string();
        /*fs::wpath curPath = (*dir_itr);*/
		auto curPath = (*dir_itr);
        std::wstring str = curPath.path().wstring();

        //if (fs::exists(curPath)==true)
        try
        {
            if( 0 == ::_waccess( str.c_str() , 0 ) )
            {
                if (std::experimental::filesystem::is_directory( *dir_itr ))
                {
                    try
                    {
                        std::experimental::filesystem::directory_iterator check_handling(  *dir_itr );
                    }
                    catch(std::runtime_error ex  )
                    {
                        continue;
                    }

                    ++m_nDirCount;			
                    m_vecDirectoryList.push_back(curPath.path().wstring());
                }
                else
                {
                    ++m_nFileCount;
                    m_vecFileList.push_back(curPath.path().wstring());
                }
            }		
        }
        catch (...)
        {
        }			
    }
	m_eDType = EDT_NORMAL;
}

bool kFileDlgManager::GetDiskInfo(wchar_t *pbuffer)
{
    DWORD dwSectorPerCluster = 0;
    DWORD dwBytesPerSector = 0;
    DWORD dwNumberOfFreeClusters = 0;
    DWORD dwTotalNumberOfClusters = 0;

    BOOL bSucces = FALSE;

    bSucces = GetDiskFreeSpace( pbuffer,
        &dwSectorPerCluster,
        &dwBytesPerSector,
        &dwNumberOfFreeClusters,
        &dwTotalNumberOfClusters);

    if( bSucces == FALSE || dwTotalNumberOfClusters <= 0)
    {
        return false;
    }

    return true;

}

//테스트 코드
/*
void kFileDlgManager::TestPrint()
{
	std::cout << "Directory Name : " << GetCuurentDirectoryName() << "\n";

	for (int i=0;i<m_vecDirectoryList.size();i++)
	{
		std::cout << "Directory : " << m_vecDirectoryList[i] << "\n";		
	}

	for (int i=0;i<m_vecFileList.size();i++)
	{
		std::cout << "File : " << m_vecFileList[i] << "\n";		
	}
}*/