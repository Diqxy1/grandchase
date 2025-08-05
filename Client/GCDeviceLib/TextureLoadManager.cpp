#include "stdafx.h"
#include "TextureLoadManager.h"
#include "TextureLoadThread.h"

const int nLogTickCount = 1000 * 60 * 5; // 5분마다 측정
const int nSlowTickCount = 500; //  0.5초

ImplementSingleton(KTextureLoadManager)

	KTextureLoadManager::KTextureLoadManager()
	: m_bTerminateReserved(false)
	, m_bEndThread(false)
	, m_dwTickCount(0)
	, m_bLogFile(false)
{
	InitializeCriticalSection(&m_csTexLoadList);
	InitializeCriticalSection(&m_csTexCompList);

	m_TexCompList.clear();

	m_kMaxTexLoadInfo.m_strTexName = "Initial state.";
	m_kMaxTexLoadInfo.m_dwAddTerm = 0;
	m_kMaxTexLoadInfo.m_dwTerm = 0;
	m_kMaxTexLoadInfo.m_cTime = CTime::GetCurrentTime();

	m_kAvgTexLoadInfo.m_nLoadCount = 0;
	m_kAvgTexLoadInfo.m_dwTick = 0;
	m_kAvgTexLoadInfo.m_dwAddTick = 0;

	m_dwSlowLoadCount = 0;
}

void KTextureLoadManager::InitLog()
{
	m_bLogFile = true;

	FILE* pFile = NULL;
	pFile = fopen("TexureLoadLog.txt", "wb");
	if (pFile)
	{
		fclose(pFile);
	}
}

KTextureLoadManager::~KTextureLoadManager()
{
	DeleteCriticalSection(&m_csTexLoadList);
	DeleteCriticalSection(&m_csTexCompList);
}

void KTextureLoadManager::FrameMove()
{
	DWORD dwCurTickCount = GetTickCount();

	if (dwCurTickCount - m_dwTickCount >= nLogTickCount)
	{
		DumpFileLog();
		m_dwTickCount = dwCurTickCount;
	}
}

void KTextureLoadManager::DumpFileLog()
{
	if (!m_bLogFile)
		return;

	FILE* pFile = NULL;

	pFile = fopen("TexureLoadLog.txt", "ab");
	if (pFile)
	{
		std::string str = DumpMaxProcessInfo();
		fprintf(pFile, "%s", str.c_str());

		str = DumpAvgProcessInfo();
		fprintf(pFile, "%s", str.c_str());

		fclose(pFile);

		ResetAvgProcessingInfo();
		ResetMaxProcessingInfo();
	}
}

const std::string KTextureLoadManager::DumpMaxProcessInfo() const
{
	std::string strText;

	KLocker lock(m_kMaxTexLoadInfo.m_cs);

	std::ostringstream stm;
	stm << "Max Load time  "
		<< (const char*)m_kMaxTexLoadInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << " "
		<< m_kMaxTexLoadInfo.m_strTexName << " "
		<< m_kMaxTexLoadInfo.m_dwTerm << " "
		<< m_kMaxTexLoadInfo.m_dwAddTerm
		<< std::endl;

	strText = stm.str();

	return strText;
}

void KTextureLoadManager::ResetMaxProcessingInfo()
{
	KLocker lock(m_kMaxTexLoadInfo.m_cs);

	m_kMaxTexLoadInfo.m_dwTerm = 0;
	m_kMaxTexLoadInfo.m_dwAddTerm = 0;
	m_kMaxTexLoadInfo.m_cTime = CTime::GetCurrentTime();
	m_kMaxTexLoadInfo.m_strTexName = "";
}

void KTextureLoadManager::CheckMaxProcessingInfo(DWORD dwAddElapTime, DWORD dwElapTime, const std::string strTexName)
{
	KLocker lock(m_kMaxTexLoadInfo.m_cs);

	if (dwAddElapTime > m_kMaxTexLoadInfo.m_dwAddTerm)
	{
		m_kMaxTexLoadInfo.m_strTexName = strTexName;
		m_kMaxTexLoadInfo.m_cTime = CTime::GetCurrentTime();
		m_kMaxTexLoadInfo.m_dwTerm = dwElapTime;
		m_kMaxTexLoadInfo.m_dwAddTerm = dwAddElapTime;
	}
}

const std::string KTextureLoadManager::DumpAvgProcessInfo() const
{
	std::string strText;

	KLocker lock(m_kAvgTexLoadInfo.m_cs);

	int nLoadTime = 0;
	int nAddLoadTime = 0;
	if (m_kAvgTexLoadInfo.m_nLoadCount > 0)
	{
		nAddLoadTime = (m_kAvgTexLoadInfo.m_dwAddTick / m_kAvgTexLoadInfo.m_nLoadCount);
		nLoadTime = (m_kAvgTexLoadInfo.m_dwTick / m_kAvgTexLoadInfo.m_nLoadCount);
	}

	std::ostringstream stm;
	stm << "Avg Load time  "
		<< (const char*)CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")) << " "
		<< m_kAvgTexLoadInfo.m_nLoadCount << " "
		<< m_kAvgTexLoadInfo.m_dwTick << " "
		<< m_kAvgTexLoadInfo.m_dwAddTick << " "
		<< nLoadTime << " "
		<< nAddLoadTime
		<< std::endl;

	strText = stm.str();

	return strText;
}

void KTextureLoadManager::ResetAvgProcessingInfo()
{
	KLocker lock(m_kAvgTexLoadInfo.m_cs);

	m_kAvgTexLoadInfo.m_nLoadCount = 0;
	m_kAvgTexLoadInfo.m_dwTick = 0;
	m_kAvgTexLoadInfo.m_dwAddTick = 0;
}

void KTextureLoadManager::CheckAvgProcessingInfo(DWORD dwAddElapTime, DWORD dwElapTime)
{
	KLocker lock(m_kAvgTexLoadInfo.m_cs);

	if (dwAddElapTime > nSlowTickCount)
	{
		m_dwSlowLoadCount++;
	}

	m_kAvgTexLoadInfo.m_nLoadCount++;
	m_kAvgTexLoadInfo.m_dwTick += dwElapTime;
	m_kAvgTexLoadInfo.m_dwAddTick += dwAddElapTime;
}

DWORD KTextureLoadManager::GetSlowLoadCount()
{
	KLocker lock(m_kAvgTexLoadInfo.m_cs);

	return m_dwSlowLoadCount;
}

void KTextureLoadManager::AddTextureList(TexLoadData& sTexLoadData)
{
	KLocker lock(m_csTexLoadList);

	if (SiKTextureLoadManager()->IsComplateTexture(sTexLoadData.strTexName))
		return;

	sTexLoadData.dwTickCount = GetTickCount();

	m_TexLoadList.push(sTexLoadData);
}

bool KTextureLoadManager::GetTextureList(TexLoadData& sTexLoadData)
{
	{
		// locking scope
		KLocker lock(m_csTexLoadList);

		if (!m_TexLoadList.empty())
		{
			sTexLoadData = m_TexLoadList.front();
			m_TexLoadList.pop();
			return true;
		}
		else if (m_bTerminateReserved)
		{
			sTexLoadData.strTexName = "";
			m_bEndThread = true;
			return true;
		}
	} // locking scope

	return false;
}

void KTextureLoadManager::ClearLoadList()
{
	KLocker lock(m_csTexLoadList);

	while (!m_TexLoadList.empty())
	{
		m_TexLoadList.pop();
	}
}

void KTextureLoadManager::Init()
{
	m_pLoadThread = CreateThread();
}

void KTextureLoadManager::BeginThread()
{
	if (m_pLoadThread)
		m_pLoadThread->Begin();
}

void KTextureLoadManager::EndThread()
{
	m_bTerminateReserved = true;

	while (!m_bEndThread) //&& ::GetTickCount() - dwWaitTick < 10000 )
	{
		::Sleep(20);
	}

	SAFE_DELETE(m_pLoadThread);

	ClearLoadList();
	ClearLoadTexture();
}

KTextureLoadThread* KTextureLoadManager::CreateThread()
{
	KTextureLoadThread* pLoadThread = new KTextureLoadThread;
	return pLoadThread;
}

void KTextureLoadManager::AddCompTexture(const std::string& strTexName, TexLoadData& sTexLoadData)
{
	KLocker lock(m_csTexCompList);

	//  이미 로드 되어 있다.
	if (m_TexCompList.find(strTexName) != m_TexCompList.end())
	{
		SAFE_RELEASE(sTexLoadData.m_pTexture);
	}

	m_TexCompList.insert(std::make_pair(strTexName, sTexLoadData));
}

bool KTextureLoadManager::IsComplateTexture(const std::string& strTexName)
{
	KLocker lock(m_csTexCompList);

	if (m_TexCompList.find(strTexName) == m_TexCompList.end())
		return false;

	return true;
}

bool KTextureLoadManager::GetComplateTexture(TexLoadData& sTexLoadData, const std::string& strTexName)
{
	KLocker lock(m_csTexCompList);

	std::map<std::string, TexLoadData>::iterator pos = m_TexCompList.find(strTexName);
	if (pos != m_TexCompList.end())
	{
		sTexLoadData = pos->second;
		m_TexCompList.erase(pos);
		return true;
	}

	return false;
}

void KTextureLoadManager::ClearLoadTexture()
{
	KLocker lock(m_csTexCompList);

	std::map<std::string, TexLoadData>::iterator pos = m_TexCompList.begin();
	for (; pos != m_TexCompList.end(); ++pos)
	{
		SAFE_RELEASE(pos->second.m_pTexture);
	}

	m_TexCompList.clear();
}

void KTextureLoadManager::RemoveTexture(const std::string& strTexName)
{
	KLocker lock(m_csTexCompList);

	std::map<std::string, TexLoadData>::iterator pos = m_TexCompList.find(strTexName);
	if (pos != m_TexCompList.end())
	{
		m_TexCompList.erase(pos);
	}

	return;
}
