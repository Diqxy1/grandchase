#include "StdAfx.h"
#include "kFileDownLoadThread.h"
#include "FileDownloadManager.h"

#define GUILDMARK_URL L"http://116.120.238.38/cp/GrandChase/"


void FileDownloadManager::MakeThread()
{
	m_kDownloadThread = new KFileDownThread(*this, &FileDownloadManager::RunFunc, 500);
}

FileDownloadManager::FileDownloadManager(void)
{
	//m_kDownloadThread = NULL
	m_dquDownLoadFileList.clear();
	m_vecCompleteFileList.clear();
	m_strDefaultURL.clear();
    m_kDownloadThread = NULL;
    m_mapURLList.clear();
}

FileDownloadManager::~FileDownloadManager(void)
{
	if (m_kDownloadThread!=NULL)
	    ((KFileDownThread *)(m_kDownloadThread))->End();
	SAFE_DELETE(m_kDownloadThread);
}

void FileDownloadManager::RunFunc(KHttpWrapper *kHttp)
{

	std::wstring strData;
    std::wstring strURL;
    std::wstring strDownFolder;
    std::pair<EDOWN_DATA_TYPE, std::wstring> downLoadData;

	if ( GetDownLoadFileData(downLoadData.first, downLoadData.second) == false )
        return;

    strURL = m_mapURLList[downLoadData.first];
    strData = downLoadData.second;

    if ( downLoadData.first == EDOWN_GUILD_MARK )
        strDownFolder = GUILD_MARK_FOLDER;
    else if ( downLoadData.first == EDOWN_CHANNEL_IMG )
        strDownFolder = CHANNEL_IMG_FOLDER;

    //다운로드 시도..
    //Lock걸고 성공 목록에 넣어준다. 성공후 알려준다.
    if (DownLoadFromHttp(kHttp,strURL.c_str(),strData.c_str(), strDownFolder.c_str()) ==true)
    {
        InsertCompleteData(downLoadData.first, downLoadData.second);
    }
    else
    {
        //채널이미지의 경우 재시도 하지 않는다.
        if ( downLoadData.first == EDOWN_CHANNEL_IMG )
            return;
        InsertDownloadData(downLoadData.second, downLoadData.first);
    }
}

bool FileDownloadManager::DownLoadFromHttp(KHttpWrapper* pkHttp_,const WCHAR* pStrSrcPath_,const WCHAR* pStrFileName_, 
									  const WCHAR* pStrDestPath_)
{
	static DWORD s_dwRetryCount = 0;
	s_dwRetryCount = 0;
	bool bFTPDownload = true;
	std::wstring strFileName_=pStrFileName_;

	for ( int i = 0; i < MAX_DOWNLOAD_RETRY; i++ )
	{
		int ret;

		if (::access(GCUTIL_STR::GCStrWideToChar(pStrDestPath_),0)!=NULL)
        {
			CreateDirectoryW(pStrDestPath_,NULL);
		}

		ret = pkHttp_->Get( pStrSrcPath_, strFileName_.c_str(), pStrDestPath_);
		
		if ( ret == 0 )
		{
			return true;
		}
		else 
		{
			::Sleep( MAX_DOWNLOAD_DELAY );
			pkHttp_->Reset();
		}
	}
	return false;
}

bool FileDownloadManager::GetCompleteFileData( EDOWN_DATA_TYPE& type, std::wstring& strFilNamestring )
{
    std::pair<EDOWN_DATA_TYPE, std::wstring> tempData;

    KLocker lock(m_csCompleteListLock);
    if( m_vecCompleteFileList.empty() ) 
        return false;

    tempData = m_vecCompleteFileList[m_vecCompleteFileList.size()-1];
    m_vecCompleteFileList.pop_back();

    type = tempData.first;
    strFilNamestring = tempData.second;
    return true;
}


void FileDownloadManager::InsertDownloadData(std::vector<std::wstring> vecFilNamestring, EDOWN_DATA_TYPE eType)
{
    std::pair<EDOWN_DATA_TYPE, std::wstring> tempData;

    KLocker lock(m_csDownLoadListLock);
    if (vecFilNamestring.size() == 0 ) return;

    bool bChk;
    for (int i=0;i< static_cast<int>( vecFilNamestring.size() );i++)
    {
        bChk=true;
        for (int j=0;j < (int) m_dquDownLoadFileList.size();j++)
        {
            if (m_dquDownLoadFileList[j].first==eType &&
                m_dquDownLoadFileList[j].second==vecFilNamestring[i])
                bChk=false;
        }

        if (bChk==true)
        {
            tempData.first = eType;
            tempData.second = vecFilNamestring[i];

            m_dquDownLoadFileList.push_back(tempData);
        }	
    }
}

void FileDownloadManager::InsertDownloadData(std::wstring strFilNamestring, EDOWN_DATA_TYPE eType)
{
    std::pair<EDOWN_DATA_TYPE, std::wstring> tempData;

    KLocker lock(m_csDownLoadListLock);
    if (strFilNamestring.empty() == true ) return;

    for (int j=0;j < (int) m_dquDownLoadFileList.size();j++)
    {
        if (m_dquDownLoadFileList[j].first==eType &&
            m_dquDownLoadFileList[j].second==strFilNamestring)
            return;
    }

    tempData.first = eType;
    tempData.second = strFilNamestring;

    m_dquDownLoadFileList.push_back(tempData);
}

void FileDownloadManager::InsertCompleteData(EDOWN_DATA_TYPE type, std::wstring strFilNamestring)
{
    std::pair<EDOWN_DATA_TYPE, std::wstring> tempData;

    KLocker lock(m_csCompleteListLock);
    if (strFilNamestring.empty() == true ) return;

    tempData.first = type;
    tempData.second = strFilNamestring;

    m_vecCompleteFileList.push_back(tempData);
}

void FileDownloadManager::Start()
{
    ((KFileDownThread *)(m_kDownloadThread))->Begin();
}

void FileDownloadManager::SetDownLoadURL( EDOWN_DATA_TYPE eType, std::wstring strURL )
{
    KLocker lock(m_csDownLoadListLock);
    m_mapURLList.insert( std::make_pair( eType, strURL ) );
}

bool FileDownloadManager::GetDownLoadFileData( IN EDOWN_DATA_TYPE& type, IN std::wstring& strFilNamestring )
{
    std::pair<EDOWN_DATA_TYPE, std::wstring> downLoadData;

    //Lock걸어서 데이터 꺼낸다.
    KLocker lock(m_csDownLoadListLock);

    if( m_dquDownLoadFileList.empty() )
        return false;

    if (m_mapURLList.empty()) 
        return false;

    downLoadData = m_dquDownLoadFileList.front();
    m_dquDownLoadFileList.pop_front();

    type = downLoadData.first;
    strFilNamestring = downLoadData.second;

    return true;
}