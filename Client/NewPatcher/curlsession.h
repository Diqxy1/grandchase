#pragma once 
#include "stdafx.h"
#include "KCSLockType.h"
#include "../kom/session.h"


struct SRecieveInfo:public boost::noncopyable
{
public:
	SRecieveInfo(std::string u,std::ostream &strm,int off,int len,NETPROGRESS_CALLBACK prog, KCSLockType<SProgressInfo> &proginfo)
		:url(u),stream(strm),offset(off),length(len),readlength(0),progress(prog),progressinfo(proginfo)
	{
		std::transform(url.begin(),url.end(),url.begin(),tolower);
	}

	bool IsStop()
	{
		KCSLOCK_BLOCK(progressinfo)
		{
			return progressinfo.isstoped;
		}
	}

	void Read(int len)
	{
		readlength+= len;

		KCSLOCK_BLOCK(progressinfo)
		{
			progressinfo.Read(len);
			if(progress)
				progress(progressinfo);
		}
	}

	int GetFileSize()
	{
		KCSLOCK_BLOCK(progressinfo)
		{
			return progressinfo.currentlength;
		}
	}

	void SetFileSize( int _nSize )
	{
		KCSLOCK_BLOCK(progressinfo)
		{
			progressinfo.currentlength = _nSize;
		}
	}
	NETPROGRESS_CALLBACK progress;
	KCSLOCK_DECLARE( SProgressInfo&, progressinfo );  
	std::string url;
	std::ostream & stream;
	int	offset;
	int length;
	int readlength;	
};


class KCurlSession:public KomSession,public boost::noncopyable
{
public:
	
	static bool SplitURL(std::string url,std::string &server,std::string &path);

	
	KCurlSession();
	KCurlSession(std::string id,std::string pwd,bool rename);
	~KCurlSession();

	bool Report(std::string ur,std::string service,std::string version,char * data,int size);	
	bool ReportforGCLOG( std::string url,std::string service,std::string version, bool success,
		std::string lasterror, int lasterrorcode, bool ismannual,std::string patchserverip,DWORD patchtime,
		unsigned int local_info,unsigned int server_info, bool isEndReport, bool isPatch);
	bool ReportCN( std::string url,std::string strGameType, std::string strGameVersion, std::string strMac, int iState );
	bool SendFile(std::string url,std::string filename);

	bool WriteRemoteData(std::string url,std::ostream &stream,int off,int length,
		NETPROGRESS_CALLBACK progress,KCSLockType<SProgressInfo>& progressinfo ,int &error);
	bool DownloadFile(std::string url,std::string filename,int off ,int length,
		NETPROGRESS_CALLBACK progress,KCSLockType<SProgressInfo>& progressinfo,int &error);

	bool WriteRemoteData(std::string url,std::ostream &stream,int off=0,int length=-1);
	bool DownloadFile(std::string url,std::string filename,int off=0,int length=-1);

	void SetExeRename(bool b){exerename=b;}
	
	static void SetTotalWriteLength(int i){totalwrite = i;}
	static void AddTotalWriteLength(int i){totalwrite += i;}
	static int GetTotalWriteLength(){return totalwrite;}

private:	

	static int totalwrite;
	std::string RenameExe(std::string filename);

	CURL * m_ctx;
	std::string id;
	std::string pw;

    static std::string m_strLogFile;

	bool exerename;

};
