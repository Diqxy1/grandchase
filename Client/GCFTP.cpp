#include "stdafx.h"
#include "GCFTP.h"

//생성자
GCFTP::GCFTP()
{
	//FTPIP,유저ID,암호를 설정
	m_FTPIP = "116.120.238.38";
	m_UserID = "grandchase";
	m_PassWord = "gpfrpdlxm310TT^^";
}


//FTP에 접속하는 함수
bool GCFTP::ConnectFTP()
{
	//인터넷을 연다
	m_HInternet = InternetOpen(L"FTPTEST", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	//유효한지 검사
	if(m_HInternet == NULL)
	{
		return false;
	}

	//FTP서버에 접속한다
	m_HFTP = InternetConnect(m_HInternet,m_FTPIP,INTERNET_DEFAULT_FTP_PORT,m_UserID,m_PassWord,INTERNET_SERVICE_FTP,0,0);

	if(m_HFTP == NULL)
	{
		InternetCloseHandle(m_HFTP);
		return false;
	}

	return true;
}


//인자로 주어진 값으로 디렉토리를 설정
bool GCFTP::SetDirectory(std::wstring CurrentDirectory, std::wstring RemoteFolderDirectory)
{	
	std::wstring strCurrentDir;
	strCurrentDir = RemoteFolderDirectory;

	BOOL bRet;
	static DWORD len = 255;

	//CurrentDirectory에 폴더가 존재하는지 확인
	bRet = FtpSetCurrentDirectoryW( m_HFTP, strCurrentDir.c_str());
	if( !bRet )
	{
		//존재하지 않을 경우 폴더을 생성하고
		bRet = FtpCreateDirectoryW( m_HFTP, strCurrentDir.c_str() );
		if( !bRet )
			return false;
		//생성한 폴더를 경로로 설정
		bRet = FtpSetCurrentDirectoryW( m_HFTP, strCurrentDir.c_str());
		if( !bRet )
			return false;
	}
	strCurrentDir = CurrentDirectory;

	//CurrentDirectory에 폴더가 존재하는지 확인
	bRet = FtpSetCurrentDirectoryW( m_HFTP, strCurrentDir.c_str());
	if( !bRet )
	{
		//존재하지 않을 경우 폴더을 생성하고
		bRet = FtpCreateDirectoryW( m_HFTP, strCurrentDir.c_str() );
		if( !bRet )
			return false;
		//생성한 폴더를 경로로 설정
		bRet = FtpSetCurrentDirectoryW( m_HFTP, strCurrentDir.c_str());
		if( !bRet )
			return false;
	}

	return true;
}

bool GCFTP::SendFile(std::wstring FileName)
{
	//인자로 주어진 파일명으로 파일을 전송
	if(!FtpPutFileW(m_HFTP,FileName.c_str(),FileName.c_str(),FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0))
		return false;

	return true;
}

void GCFTP::DisConnectFTP()
{
	//종료 시 FTP핸들들을 해제
	InternetCloseHandle(m_HInternet);
	InternetCloseHandle(m_HFTP);
	m_HInternet = NULL;
	m_HFTP = NULL;
}
