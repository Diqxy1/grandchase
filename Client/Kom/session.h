#pragma once
#include <KCSLockType.h>

struct SProgressInfo:public boost::noncopyable
{
public:
	SProgressInfo()
		:totallength(0),totalreadlength(0),currentlength(0),currentreadlength(0),isstoped(false)
	{}	

	void Read(int size)
	{
		currentreadlength+=size;		
	}

	void TotalRead(int size)
	{		
		totalreadlength+=size;
	}

	void SetFile(std::string filename,int size)
	{
		currentfilename = filename;
		currentlength = size;
		currentreadlength = 0;
	}

    void GetProgressInfo( SProgressInfo& sProgressInfo )
    {   
        sProgressInfo.currentfilename = currentfilename;
        sProgressInfo.totallength = totallength;
        sProgressInfo.totalreadlength = totalreadlength;
        sProgressInfo.currentlength = currentlength;
        sProgressInfo.currentreadlength = currentreadlength;
    }

	UINT totallength;
	UINT totalreadlength;

	std::string currentfilename;
	
	int currentlength;	// 현재 파일 사이즈
	int currentreadlength;	// 현재 읽은 파일 사이즈

	bool isstoped;
};



#define NETSTATRT_CALLBACK boost::function0<void>
#define NETPROGRESS_CALLBACK boost::function1<void,SProgressInfo &>
#define NETERROR_CALLBACK boost::function2<void,std::string,int>
#define COMPLETE_CALLBACK boost::function2<void,bool,bool>
#define IS_START_CALLBACK boost::function0<bool>



#define NETGRIDDOWNLOADADD_CALLBACK		boost::function1<bool,SGridDownFileInfo &>
#define NETGRIDDOWNLOADSTART_CALLBACK	boost::function0<void>
#define NETGRIDDOWNLOADRUN_CALLBACK     boost::function0<void>


class KomSession
{
public:
	virtual bool WriteRemoteData(std::string url,std::ostream &stream,int off,int length,
		NETPROGRESS_CALLBACK progress,KCSLockType<SProgressInfo>& progressinfo ,int &error)=0;
	virtual bool WriteRemoteData(std::string url,std::ostream &stream,int off,int length)=0;

};
