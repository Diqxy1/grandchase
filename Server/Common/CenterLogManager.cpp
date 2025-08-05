#include "CenterLogManager.h"

#include <dbg/dbg.hpp>

#include "log4.h"
//FILE_NAME_FOR_LOG

#   undef _ENUM
#   define _ENUM( id, comment ) L## #comment,

const wchar_t* KCenterLogManager::ms_szFolderName[] = {
#include "CenterLogfile_def.h"
};

ImplementSingleton(KCenterLogManager);
NiImplementRootRTTI(KCenterLogManager);

KCenterLogManager::KCenterLogManager(void)
{
}

KCenterLogManager::~KCenterLogManager(void)
{
	CleanUp();
}

ImplToStringW(KCenterLogManager)
{
	DWORD dwLogListSize = 0;

	{ KLocker lock(m_csMapLogList); dwLogListSize = m_mapLogList.size(); }

	START_TOSTRINGW
		<< L" Log List Size       : " << dwLogListSize << std::endl
		<< L" Log Mode List" << std::endl;

	{
		KLocker lock(m_csMapLogList);
		std::map< int, KGenericLogfile* >::const_iterator mit;
		mit = m_mapLogList.begin();
		for (; mit != m_mapLogList.end(); ++mit)
			mit->second->Dump(stm_);
	}
	return stm_;
}

void KCenterLogManager::Init()
{
	for (int i = 0; i < LT_MAX; ++i)
	{
		KGenericLogfile* pkLog = new KGenericLogfile;
		JIF(pkLog);
		std::wstring strFolderName = KCenterLogManager::ms_szFolderName[i];
		pkLog->SetID(i);
		pkLog->OpenFile(strFolderName);
		m_mapLogList.insert(std::make_pair(i, pkLog));
	}
}

KGenericLogfile& KCenterLogManager::Get(int nKey_)
{
	std::map<int, KGenericLogfile*>::iterator mit;
	mit = m_mapLogList.find(nKey_);

	if (mit != m_mapLogList.end())
	{
		if (mit->second->GetReserve() == true)
			mit->second->ChangeFile();
		return (*mit->second);
	}

	START_LOG(cerr, L" UnDefined Key : " << nKey_) << END_LOG;
	return m_kUnKnown;
}

void KCenterLogManager::ChangeFile(int nKey_)
{
	std::map<int, KGenericLogfile*>::iterator mit;
	mit = m_mapLogList.find(nKey_);

	if (mit != m_mapLogList.end())
		mit->second->SetReserve();
}

void KCenterLogManager::CleanUp()
{
	if (m_mapLogList.empty())
		return;
	std::map<int, KGenericLogfile*>::iterator mit;

	for (mit = m_mapLogList.begin(); mit != m_mapLogList.end(); ++mit)
	{
		delete mit->second;
	}

	m_mapLogList.clear();
}

void KCenterLogManager::ToggleWrite(int nID_)
{
	std::map<int, KGenericLogfile*>::iterator mit;
	mit = m_mapLogList.find(nID_);

	if (mit != m_mapLogList.end())
	{
		mit->second->ToggleWrite();
		std::wcout << L"Log File " << ms_szFolderName[nID_] << L" Toggle : " << (mit->second->GetWriteEnable() ? L"true" : L"false") << std::endl;
	}
	else
	{
		std::wcout << " Not Exist Log ID : " << nID_ << std::endl;
	}
}