#include ".\logmanager.h"
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

#   undef _ENUM
#   define _ENUM( id, comment , header) L## #comment,

const wchar_t* KLogManager::ms_szFolderName[] = {
#include "GenericLogfile_def.h"
};

#   undef _ENUM
#   define _ENUM( id, comment , header) L## #header,

const wchar_t* KLogManager::ms_szHeader[] = {
#include "GenericLogfile_def.h"
};

ImplementSingleton(KLogManager);
NiImplementRootRTTI(KLogManager);

KLogManager::KLogManager(void)
{
}

KLogManager::~KLogManager(void)
{
	CleanUp();
}

ImplToStringW(KLogManager)
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

void KLogManager::Init()
{
	for (int i = 0; i < LT_MAX; ++i)
	{
		KGenericLogfile* pkLog = new KGenericLogfile;
		JIF(pkLog);
		std::wstring strFolderName = KLogManager::ms_szFolderName[i];
		std::wstring strHeader = KLogManager::ms_szHeader[i];
		pkLog->SetID(i);
		pkLog->SetHeader(strHeader);
		pkLog->OpenFile(strFolderName);
		m_mapLogList.insert(std::make_pair(i, pkLog));
	}
}

KGenericLogfile& KLogManager::Get(int nKey_)
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

void KLogManager::ChangeFile(int nKey_)
{
	std::map<int, KGenericLogfile*>::iterator mit;
	mit = m_mapLogList.find(nKey_);

	if (mit != m_mapLogList.end())
		mit->second->SetReserve();
}

void KLogManager::CleanUp()
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

void KLogManager::ToggleWrite(int nID_)
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