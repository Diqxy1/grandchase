#pragma once
#include <fstream>
#include <KncUtil.h>

class KGenericLogfile;

namespace GLOG
{
	KGenericLogfile& endl(KGenericLogfile& L);
	KGenericLogfile& tab(KGenericLogfile& L);
};

class KGenericLogfile
{
public:
	KGenericLogfile(void);
	~KGenericLogfile(void);

	void SetID(int nID) { m_nID = nID; };
	bool ChangeFile();
	bool GetWriteEnable() const { return m_bEnableWrite; };
	void ToggleWrite() { m_bEnableWrite = !m_bEnableWrite; };
	bool OpenFile(std::wstring& strFolderName);
	void CloseFile();
	bool MakeFolder(IN std::wstring& strFolderName, OUT std::wstring& strFullPath);
	void SetReserve() { m_bChangeReserved = true; }
	bool GetReserve() const { return m_bChangeReserved; }
	void Dump(std::wostream& stm) const;
	std::wstring GetFolderName() const { return m_strFolderName; }
	std::wstring GetFullPath() const { return m_strFullPath; }
	void SetHeader(const std::wstring& strHeader_);

	template<class T0> KGenericLogfile& operator << (T0 data)
	{
		if (m_bInitialized && m_bEnableWrite)
			m_wFile << data;

		return *this;
	}

	template<> KGenericLogfile& operator << (bool b)
	{
		if (m_bInitialized && m_bEnableWrite)
			m_wFile << ((b == true) ? L"true" : L"false");

		return *this;
	}

	template<> KGenericLogfile& operator << (KGenericLogfile& (*_manip)(KGenericLogfile&))
	{
		return _manip(*this);
	}

	friend KGenericLogfile& GLOG::endl(KGenericLogfile&);
	friend KGenericLogfile& GLOG::tab(KGenericLogfile&);

protected:
	int                 m_nID;
	bool                m_bEnableWrite;
	bool                m_bChangeReserved;
	bool                m_bInitialized;
	std::wofstream      m_wFile;
	std::wstring        m_strFolderName;
	std::wstring        m_strFullPath;
	std::wstring        m_strHeader;
};
