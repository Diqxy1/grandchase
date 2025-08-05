#pragma once 

#include "stdafx.h"
#include "threadpool.hpp"
#include "../kom/kom.h"
#include "curlsession.h"

#define PATCHINFOFILE "patchinfo.xml"
#define LANGINFOFILE "langinfo.xml"
#define LANGOPTFILE "lang.dat"

class KPatchFileInfo
{
public:
	KPatchFileInfo()
		:size(0),adler32(0),filetime(0)
	{}
	KPatchFileInfo(xmlNode * node);	
	KPatchFileInfo(std::string filename);
	KPatchFileInfo(const KPatchFileInfo&r)
		:size(r.size),adler32(r.adler32),filetime(r.filetime)
	{}

	~KPatchFileInfo(){}

	KPatchFileInfo & operator = (const KPatchFileInfo & r)
	{
		size = r.size;
		adler32 = r.adler32;
		filetime = r.filetime;		
		return *this;
	}

	bool operator == (const KPatchFileInfo & r)
	{
		return (size == r.size && adler32 == r.adler32 && filetime == r.filetime);
	}

	bool operator != (const KPatchFileInfo & r)
	{
		return (size != r.size || adler32 != r.adler32 || filetime != r.filetime);
	}

	unsigned int GetSize(){return size;}
	unsigned int GetAdler32(){return adler32;}
	unsigned int GetFileTime(){return filetime;}

	bool Verify(std::string filename);
	

private:

	unsigned int adler32;
	unsigned int size;
	unsigned int filetime;		
};




class KPatchInfo
{
public:
	
	bool Open(KCurlSession &session,std::string url);
	bool Open(std::string dir);
	bool Open(KPatchInfo & l,std::string dir);

	KPatchInfo()
		:renameexe(false)
	{Close();}
	KPatchInfo(const KPatchInfo & r)
		:fileinfos(r.fileinfos),renameexe(false)
	{}
	~KPatchInfo(){Close();}

	void Close(){fileinfos.clear();}	

	KPatchInfo & operator = (const KPatchInfo & r)
	{
		fileinfos = r.fileinfos;
		renameexe = r.renameexe;
		return *this;
	}	

	bool operator == (KPatchInfo & r);
	
	void Diff(KPatchInfo &l,KPatchInfo&r);
	UINT GetFileSize();

	void Insert(std::string key, std::string filename);
	void Erase(std::string filename);
	
	KPatchFileInfo Find(std::string);
	bool Exist(std::string);
	bool IsEmpty();

	std::map<std::string,KPatchFileInfo> & GetFileInfos(){return fileinfos;}

	std::set<std::string> & GetPatcherFiles(){return patcherfiles;}
	std::set<std::string> & GetVerifyFiles(){return verifyfiles;}

	int Size()
	{
		return fileinfos.size();
	}
	bool IsRenameExe(){return renameexe;}

	unsigned int GetCheckSum();
private:

	bool ParsePatchinfo(char* data,int len);

	std::map<std::string,KPatchFileInfo> fileinfos;

	std::set<std::string> patcherfiles;
	std::set<std::string> verifyfiles;	

	bool renameexe;
};

struct SReportInfo{
	int successtype;
	std::string url;
	std::string service;
	std::string version;
	SReportInfo(){
		successtype = 0;
		url = "";
		service ="";
		version = "";
	}

	SReportInfo(int successtype, std::string url,std::string service,std::string version ) 
	{
		successtype = successtype;
		url = url;
		service = service;
		version = version;
	}

};

class KReportInfo {
public:
	KReportInfo(){}
	std::vector<SReportInfo> m_vecReportInfo;
};

class KPatchLib
{
public:

	enum PatchError
	{
		OTHERERROR = 0,
		DIRCREATE = -1,
		FILEOPEN = -2,
		FILEDELETE = -3,
		WRITEKOM = -4,
		OPENKOM = -5,
		VERIFY = -6,
		MOVEFILE =-7,		
		SYSTEMFOLDER = -8,
	};

	KPatchLib();	
	KPatchLib(bool mannual,std::string mannualpatchpath);
	KPatchLib(std::string id,std::string pw);		
	~KPatchLib();

	bool GetBooleanFromDat(std::string url,bool &re);	

	bool GetPatchPath(std::string patchpathurl);	
    int GetPatchMode( std::string patchmodeurl);
	bool GetServerPatchInfo()
	{
		KCurlSession session(id,pw,renameexe);
		bool re = server_info.Open(session,patchpath+PATCHINFOFILE);
		renameexe = server_info.IsRenameExe();
		return re;
	}	
	bool GetMannualPatchInfo()
	{
		return server_info.Open(patchpath+PATCHINFOFILE);
	}
	bool GetLocalPatchInfo()
	{
		return local_info.Open(server_info,localpath);
	}

	bool GetLangInfo();		// 서버에서 설정된 랭귀지 정보를 가져온다.	
	bool ParseLanginfo( char* data ,int len);	//	xml 파싱
	bool LoadCurLang();	//	클라이언트내에 저장되어 있는 언어 정보
	bool SaveCurLang();	//	저장
	bool ConvertFileList();		//	언어별로 필요한 파일만 제외하고 모두 삭제한다.

	void Diff();
	void DownloadFiles(NETSTATRT_CALLBACK start,
        NETPROGRESS_CALLBACK fileprogress=NULL,
        NETPROGRESS_CALLBACK fileprogress2=NULL,
		NETERROR_CALLBACK error = NULL,
        COMPLETE_CALLBACK compete = NULL,
		NETGRIDDOWNLOADADD_CALLBACK  addfile = NULL,
		NETGRIDDOWNLOADSTART_CALLBACK startgriddown = NULL,
		IS_START_CALLBACK isStart = NULL
		);

	bool IsPatchComplete();
	bool CheckPatcherFiles();
	bool DownloadPatcherFiles();
	void ClearPatcherFiles();
	
	bool Run(std::string commandline);

	bool VerifyPatcherName(std::string patchername);
	bool VerifyPatcherPath(std::string path);
	
	void Start();
	void Stop();
	bool IsStoped();

	std::string GetLocalPath(){return localpath;}

	bool CreateMutex(std::wstring mutex);
	void ReleaseMutex();
	
	bool IsError(){return errorcount > MAX_RETRY;}
	void Report(int successtype,std::string url,std::string service,std::string version);

	bool IsPatched(){return patched;}

	void OnError(std::string url,int errorcode);	
private:

	void Init();		

	void DownloadFile(std::pair<std::string,KPatchFileInfo> info, NETPROGRESS_CALLBACK progress, NETPROGRESS_CALLBACK progress2,
                      NETERROR_CALLBACK error, NETGRIDDOWNLOADADD_CALLBACK  addfile = NULL);
	void DownloadMannualFile(std::pair<std::string,KPatchFileInfo> info,NETPROGRESS_CALLBACK progress,NETERROR_CALLBACK error);
    bool DownloadPatcherFile(std::string filename);	

    bool AddGridFile(std::string url, std::string localpath,  std::pair<std::string,KPatchFileInfo> info, NETGRIDDOWNLOADADD_CALLBACK addfile);

	std::string GetOSInfo();
	std::string patchserverip;
	std::string patchpath;
    std::string m_OriginalPatchpath;
	std::string localpath;

	std::string patcherdir;
	std::string patchername;

	KPatchInfo server_info;
	KPatchInfo local_info;
	boost::shared_ptr<Komfile> patchinfokom;

public:
	KCSLOCK_DECLARE( KPatchInfo , diff_info );
    KCSLOCK_DECLARE( SProgressInfo, progressinfo);

    int m_iNumPatchFile;
    std::map<std::string,KPatchFileInfo>* GetPatchFileInfo()
    {
        KCSLOCK_BLOCK( diff_info )
        {
            return &diff_info.GetFileInfos();
        }
    }
    void SetTotalSizeFromRecvSize(int iSize)
    {
        KCSLOCK_BLOCK( progressinfo )
        {
            progressinfo.TotalRead(iSize);
        }
    }
    void EraseDiffInfoFromRecvFile(const char *pszTemp)
    {
        KCSLOCK_BLOCK( diff_info )
        {
            diff_info.Erase( pszTemp );
        }
    }
    void CallPrograssBar( NETPROGRESS_CALLBACK prog )
    {
        KCSLOCK_BLOCK(progressinfo)
        {
            prog( progressinfo );	
        }
    }
    int GetCurrentFileNum()
    {
        KCSLOCK_BLOCK( diff_info )
        {
            return m_iNumPatchFile - diff_info.Size();
        }
    }
    int GetTotalFileNum()
    {
        return m_iNumPatchFile;
    }

    bool IsFTPDownSize( DWORD dwFileSize_ );
	int	GetMaxLang() { return vecLangInfo.size(); }
	std::string GetLangName( int nLang );
	void SetStrLastErr(std::string err){ lasterror = err; }
	void SetStrLastErr(int err){ lasterrorcode = err; }
	std::string GetStrLastErr(){ return lasterror; }

	std::string GetLastErrMsg( );
	bool RunExecute( std::string filename, std::string param );
	bool DownloadServerInfoFile( );
	bool DownloadVersionInfoFile( );

	KCSLOCK_DECLARE( KReportInfo, report_info );

	void SendReport(int successtype, std::string url,std::string service,std::string version );
private:
    bool isstoped;	
    HANDLE mutex;	
	std::vector<std::string> patcherfiles;
	std::vector<std::string> vecLangInfo;

	std::string pw;
	std::string id;

	bool ismannual;
	bool renameexe;		

	int			errorcount;
	std::string lasterror;
	int			lasterrorcode;
	DWORD		patchtime;
	bool		patched;
};