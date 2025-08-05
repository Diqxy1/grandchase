#pragma once

#include <Thread/Locker.h>
#include "KNCSingleton.h"

struct TexLoadData
{
	std::string strTexName;
	bool bLowQuality;
	D3DXIMAGE_INFO TexInfo;
	GCThreadMemoryFile* pMemFile;

	LPDIRECT3DTEXTURE9 m_pTexture;

	DWORD dwTickCount;

	TexLoadData()
		: pMemFile(NULL)
		, bLowQuality(false)
		, m_pTexture(NULL)
		, dwTickCount(0)
	{
	}
};

class KTextureLoadThread;

class KTextureLoadManager
{
	DeclareSingleton(KTextureLoadManager);

public:
	KTextureLoadManager();
	~KTextureLoadManager();

public:
	void Init();
	void InitLog();
	void BeginThread();
	virtual void EndThread();

	void AddTextureList(TexLoadData& sTexLoadData);
	bool GetTextureList(TexLoadData& sTexLoadData);

	void ClearLoadList();

	int GetTexLoadSize()
	{
		KLocker lock(m_csTexLoadList);
		return m_TexLoadList.size();
	}

	void AddCompTexture(const std::string& strTexName, TexLoadData& sTexLoadData);

	void ClearLoadTexture();
	void RemoveTexture(const std::string& strTexName);
	bool GetComplateTexture(TexLoadData& sTexLoadData, const std::string& strTexName);
	bool IsComplateTexture(const std::string& strTexName);

	void FrameMove();
	void DumpFileLog();

	DWORD GetSlowLoadCount();

	const std::string DumpMaxProcessInfo() const;
	const std::string DumpAvgProcessInfo() const;

	void ResetMaxProcessingInfo();
	void ResetAvgProcessingInfo();

	void CheckMaxProcessingInfo(DWORD dwAddElapTime, DWORD dwElapTime, const std::string strTexName);
	void CheckAvgProcessingInfo(DWORD dwAddElapTime, DWORD dwElapTime);

protected:
	KTextureLoadThread* CreateThread();

	std::queue<TexLoadData> m_TexLoadList; //  로드 대기중인 리스트
	std::map<std::string, TexLoadData> m_TexCompList; //  로드 완료된 리스트

	mutable CRITICAL_SECTION m_csTexLoadList;
	mutable CRITICAL_SECTION m_csTexCompList;
	KTextureLoadThread* m_pLoadThread;

	bool m_bTerminateReserved;
	bool m_bEndThread;

	DWORD m_dwTickCount;
	DWORD m_dwSlowLoadCount;
	bool m_bLogFile;

	struct KMaxTexLoadInfo
	{
		// 처리시간이 가장 오래 걸린 이벤트에 대한 정보를 기억.
		mutable KncCriticalSection m_cs;
		std::string m_strTexName;
		DWORD m_dwAddTerm; // 등록이후에 처리완료하기 까지 걸린시간.
		DWORD m_dwTerm; // 이벤트 하나를 처리완료하기까지 걸린 시간.
		CTime m_cTime;
	} m_kMaxTexLoadInfo;

	struct KAvgTexLoadInfo
	{
		mutable KncCriticalSection m_cs;
		int m_nLoadCount;
		DWORD m_dwAddTick;
		DWORD m_dwTick;
	} m_kAvgTexLoadInfo;
};

DefSingletonInline(KTextureLoadManager);
