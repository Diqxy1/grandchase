#pragma once

#include "HttpWrapper.h"
#define MAX_DOWNLOAD_DELAY 800 //다운로드 재시도 Interval
#define MAX_DOWNLOAD_RETRY 3   // 다운로드 재시도 횟수

//#define GUILD_MARK_FOLDER	L"GuildMark\\"
#define GUILD_MARK_FOLDER	L"GuildMark\\"
#define CHANNEL_IMG_FOLDER	L"Texture\\ChannelImg\\"

class FileDownloadManager
{
public:
	FileDownloadManager(void);
	~FileDownloadManager(void);
	KncCriticalSection m_csDownLoadListLock;
	KncCriticalSection m_csCompleteListLock;

    enum EDOWN_DATA_TYPE
    {
        EDOWN_GUILD_MARK = 0,
        EDOWN_CHANNEL_IMG = 1,
    };

	void MakeThread();
	void Start();
	void RunFunc(KHttpWrapper *kHttp);
    void InsertDownloadData(std::vector<std::wstring> vecFilNamestring, EDOWN_DATA_TYPE eType = EDOWN_GUILD_MARK);
    void InsertDownloadData(std::wstring strFilNamestring, EDOWN_DATA_TYPE eType = EDOWN_GUILD_MARK);
    void InsertCompleteData(IN EDOWN_DATA_TYPE type, IN std::wstring strFilNamestring);
    bool GetCompleteFileData( EDOWN_DATA_TYPE& type, std::wstring& strFilNamestring );
    void SetDownLoadURL(EDOWN_DATA_TYPE eType, std::wstring strURL);
    bool GetDownLoadFileData( IN EDOWN_DATA_TYPE& type, IN std::wstring& strFilNamestring );
private:
	bool DownLoadFromHttp(KHttpWrapper* pkHttp_,const WCHAR* pStrSrcPath_,const WCHAR* pStrFileName_, 
		const WCHAR* pStrDestPath_);

	//KFileDownThread *m_kDownloadThread;
	std::wstring m_strDefaultURL;
    std::map<EDOWN_DATA_TYPE, std::wstring> m_mapURLList;
    std::deque<std::pair<EDOWN_DATA_TYPE, std::wstring>> m_dquDownLoadFileList;
    std::deque<std::pair<EDOWN_DATA_TYPE, std::wstring>> m_vecCompleteFileList;
	
	void *m_kDownloadThread;
};
