#pragma once
#include <wininet.h>

class GCFTP
{
public:
	GCFTP();
	~GCFTP(){}
	bool ConnectFTP();
	bool SetDirectory(std::wstring StrFolderDirectory,std::wstring RemoteFolderDirectory);
	bool SendFile(std::wstring FileName);
	void DisConnectFTP();

private:
	//ftp접속 핸들
	HINTERNET m_HInternet;
	HINTERNET m_HFTP;
	//ftp접속 정보
	CString m_FTPIP;
	CString m_UserID;
	CString m_PassWord;
};

