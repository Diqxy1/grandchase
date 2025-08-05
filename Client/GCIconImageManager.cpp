#include "StdAfx.h"
#include "GCIconImageManager.h"
#include "MyD3D.h"
#include "io.h"

KGCIconImageManager *g_IconImageManager;

ImplementSingleton( KGCIconImageManager )

KGCIconImageManager::KGCIconImageManager(void)
{
}

KGCIconImageManager::~KGCIconImageManager(void)
{
    for(std::map<std::wstring, SIconImageInfo>::iterator mIt = m_mapIconImageInfo.begin();mIt != m_mapIconImageInfo.end();mIt++)
    {
        SAFE_RELEASE(mIt->second.pTexture);
    }
    m_mapIconImageInfo.clear();
}

void KGCIconImageManager::InsertImageIcon( std::wstring strImageName, std::wstring strFileURL)
{
	time_t tCurTime,tOldestTime;
	time(&tCurTime);	    
	tOldestTime = tCurTime;

	int iIndex = 0;
	GCImageIconTexture* texture;

	//다운로드 URL이 없는 경우.. 내부 MassFile에서 읽어 온다.
	if (strFileURL==L"")
	{
		texture = g_pGCDeviceManager2->CreateImageIconTexture( GCUTIL_STR::GCStrWideToChar( strImageName.c_str() ));
	}
	else
	{
		texture = g_pGCDeviceManager2->CreateImageIconTexture( GCUTIL_STR::GCStrWideToChar( strFileURL.c_str() ) ,false , true);
		//texture = g_pGCDeviceManager2->CreateTexture( GCUTIL_STR::GCStrWideToChar( L"Cursor0.DDS" ) );	
	}

	if (texture==NULL)
		return;

    if(m_mapIconImageInfo.size() > LIMIT_NUM_LOADICON)
    {
        std::map<std::wstring, SIconImageInfo>::iterator delIt;
        for(std::map<std::wstring, SIconImageInfo>::iterator mIt = m_mapIconImageInfo.begin();mIt != m_mapIconImageInfo.end();mIt++)
        {
            if(mIt->second.tTime < tOldestTime)
            {
                tOldestTime = mIt->second.tTime;
                delIt = mIt;
            }
        }

        SAFE_RELEASE(delIt->second.pTexture);
        m_mapIconImageInfo.erase(delIt);
    }

    SIconImageInfo buf;
    buf.pTexture = texture;
    buf.tTime = tCurTime;

    m_mapIconImageInfo.insert(std::pair<std::wstring,SIconImageInfo>(strImageName,buf));

	
}

//아이콘 이미지를 임의의 경로에서 읽어 온다.
void KGCIconImageManager::LoadIconImageFromRealPath( std::wstring strImageName,bool bGuildMark)
{

    if(m_mapIconImageInfo.find(strImageName) != m_mapIconImageInfo.end())
        return;

	if (bGuildMark==true)
	{
		InsertImageIcon(strImageName,GUILD_MARK_FOLDER+strImageName);
	}
	else
	{
		InsertImageIcon(strImageName,strImageName);		
	}
}

//아이콘 이미지를 압축파일 안에서 읽어 온다.
void KGCIconImageManager::LoadIconImageFromMassFile( std::wstring strImageName)
{
    if(m_mapIconImageInfo.find(strImageName) != m_mapIconImageInfo.end())
        return;

	InsertImageIcon(strImageName,L"");
}

//아이콘 이미지를 다운로드 받는 기능.. 길드마크에서 사용하기 위하여 우선 제작
void KGCIconImageManager::LoadIconImageFromHTTP( std::wstring strImageName)
{
    if(m_mapIconImageInfo.find(strImageName) != m_mapIconImageInfo.end())
        return;

	//파일이 존재하지 않는 경우에만 다운로드 한다.
	if ( ::access(GCUTIL_STR::GCStrWideToChar((GUILD_MARK_FOLDER+strImageName).c_str()),0)!=NULL)
	{
		//파일 전송한다.
		g_MyD3D->m_fileDownloadManager.InsertDownloadData(strImageName);	
	}
	else
	{
		//파일이 있으면, 그냥 실경로로 부터 읽어라
		LoadIconImageFromRealPath(strImageName,true);
	}

	
}

GCImageIconTexture* KGCIconImageManager::GetTexture( std::wstring strImageName )
{
	int iIndex = -1;
	time_t tCurTime;
	time(&tCurTime);

    //찾아서 텍스쳐님을 보내 줍니다.
    std::map<std::wstring, SIconImageInfo>::iterator mIt = m_mapIconImageInfo.find(strImageName);

    if(mIt == m_mapIconImageInfo.end())//못 찾으면 널값 리턴하심	
        return NULL;

    mIt->second.tTime = tCurTime; //최근에 로드된 시간을 갱신함.
    return mIt->second.pTexture; 

}

void KGCIconImageManager::RemoveIconImage( std::wstring strImageName )
{
    std::map<std::wstring, SIconImageInfo>::iterator mIt = m_mapIconImageInfo.find(strImageName);
    if(mIt != m_mapIconImageInfo.end())
    {
        SAFE_RELEASE(mIt->second.pTexture);
        m_mapIconImageInfo.erase(mIt);
    }
}