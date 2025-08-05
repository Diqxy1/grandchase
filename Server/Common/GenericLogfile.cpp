#include ".\genericlogfile.h"

#include <KncUtil.h>
#include "utf16_cvt.h"
#include <dbg/dbg.hpp>

KGenericLogfile::KGenericLogfile(void)
	:m_bInitialized(false),
	m_bChangeReserved(false),
	m_bEnableWrite(true),
	m_nID(-1)
{
}

void KGenericLogfile::Dump(std::wostream& stm_) const
{
}

KGenericLogfile::~KGenericLogfile(void)
{
	CloseFile();
}

bool KGenericLogfile::OpenFile(std::wstring& strFolderName_)
{
	return true;
}

void KGenericLogfile::CloseFile()
{
	m_bInitialized = false;
}

bool KGenericLogfile::ChangeFile()
{
	return m_bInitialized;
}

bool KGenericLogfile::MakeFolder(IN std::wstring& strFolderName_, OUT std::wstring& strFullPath_)
{
	return true;
}

void KGenericLogfile::SetHeader(const std::wstring& strHeader_)
{
	m_strHeader = strHeader_;
}

namespace GLOG
{
	KGenericLogfile& endl(KGenericLogfile& L)
	{
		return L;
	}

	KGenericLogfile& tab(KGenericLogfile& L)
	{
		return L;
	}
}