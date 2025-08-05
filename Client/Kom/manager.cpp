#include "stdafx.h"
#include "manager.h"
#include "..\MassFileLib\CRC32\CRC_32.h"
#include "XorCRC.h"
#include <unordered_set>

bool KomfileManager::IsIn(const std::string& filename)
{
	std::string tempString = filename;
	std::transform(tempString.begin(), tempString.end(), tempString.begin(), tolower);
	if (subfiles.find(tempString) != subfiles.end())
		return true;
	if (m_mapRealFile.find(tempString) != m_mapRealFile.end())
		return true;
	return false;
}

void KomfileManager::AddDirectory(const std::string strDirectory)
{
#if defined( MAPTOOL )
	WIN32_FIND_DATA FindData;
	HANDLE hFind;

	CString path;
	path = strDirectory.c_str();
	path += _T("*.*");

	hFind = FindFirstFile((LPCTSTR)path, &FindData);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		else
		{
			CString filePath;
			filePath = strDirectory.c_str();
			filePath += FindData.cFileName;

			std::string strPath = KncUtil::toNarrowString(filePath);
			AddRealFile(strPath);
		}
	} while (FindNextFile(hFind, &FindData));

	FindClose(hFind);
#elif defined( INCLUDE_KOM_FOLDER )
	CFileFind finder;

	// build a string with wildcards
	CString strWildcard;
	strWildcard = strDirectory.c_str();
	strWildcard += _T("*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
			continue;

		std::string strPath = KncUtil::toNarrowString(finder.GetFilePath().GetBuffer());
		AddRealFile(strPath);
	}

	finder.Close();
#else
	m_vecStrDir.push_back(strDirectory);
#endif
}

void KomfileManager::AddKeepFile(std::string strKeepFile)
{
	m_vecKeepFileList.push_back(strKeepFile);
}

void KomfileManager::AddRealFile(const std::string strFullPath_)
{
	std::string strFileName = strFullPath_;
	std::transform(strFileName.begin(), strFileName.end(), strFileName.begin(), tolower);
	strFileName = strFileName.substr(strFileName.find_last_of('\\') + 1);
	m_mapRealFile.insert(std::unordered_map< std::string, std::string >::value_type(std::string(strFileName), strFullPath_));
}

bool KomfileManager::HaveMassFile(const std::string& strKomFileName)
{
	for (auto vit : m_vecMassFile)
		if (vit->GetFilename() == strKomFileName && strKomFileName != "crc.xml")
			return true;
	return false;
}

Komfile* KomfileManager::AddMassFile(const std::string& strKomFileName)
{
	Komfile* pMassFile = new Komfile();
	if (!pMassFile->Open(strKomFileName))
	{
		delete pMassFile;
		return nullptr;
	}
	m_vecMassFile.push_back(pMassFile);
	if (std::find(m_vecKeepFileList.begin(), m_vecKeepFileList.end(), strKomFileName) != m_vecKeepFileList.end())
	{
		std::ifstream filestream(strKomFileName, std::ios_base::in | std::ios_base::binary);
		HANDLE handle = CreateFileA(strKomFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		m_vecfpStreamList.push_back(handle);
		pMassFile->Open(strKomFileName, filestream);
	}
	auto& iter_left = pMassFile->GetSubfiles();
	for (auto& subfile : iter_left)
	{
		std::string subfilename = subfile.first;
		std::transform(subfilename.begin(), subfilename.end(), subfilename.begin(), tolower);
		auto iter_right = subfiles.find(subfilename);
		if (iter_right == subfiles.end() || subfilename == "crc.xml")
		{
			subfiles.insert({ subfilename, subfile.second });
		}
		else
		{
			SDuplicateInfo sDuplicateInfo;
			sDuplicateInfo.strOriginKomFileName = iter_right->second.GetParent()->GetFilename();
			sDuplicateInfo.strDuplicatedKomFileName = pMassFile->GetFilename();
			sDuplicateInfo.strMemberFileName = iter_right->first;
			if (sDuplicateInfo.strMemberFileName.find(".stg") != std::string::npos || sDuplicateInfo.strMemberFileName.find(".lua") != std::string::npos)
			{
				std::string duplicatedInfo = "Arquivo duplicado: " + sDuplicateInfo.strMemberFileName;
				::MessageBoxA(nullptr, duplicatedInfo.c_str(), "", MB_OK);
				m_vecDuplicateInfo.push_back(sDuplicateInfo);
			}
		}
	}
	return pMassFile;
}

std::string KomfileManager::GetResFullPath(std::string filename)
{
	std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
	return m_mapRealFile[filename];
}

KomfileManager::~KomfileManager()
{
	std::vector<Komfile*>::iterator vit;
	for ( vit = m_vecMassFile.begin(); vit != m_vecMassFile.end(); vit++ )
		delete* vit;
	m_vecMassFile.clear();
	m_vecKeepFileList.clear();
	std::vector<HANDLE>::iterator vit2;
	for ( vit2 = m_vecfpStreamList.begin(); vit2 != m_vecfpStreamList.end(); vit2++ )
		::CloseHandle(*vit2);
	m_vecfpStreamList.clear();
}

bool KomfileManager::LoadGetFileData(std::string& filename, unsigned int& offset, unsigned int& compressedsize, unsigned int& size, bool& bRealFile)
{
	std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
	if (!m_bRealFileFirst)
	{
		auto iter = subfiles.find(filename);
		if (iter == subfiles.end())
			return false;
		bRealFile = false;
		return iter->second.GetFileData(filename, offset, compressedsize, size);
	}
	else
	{
		if (GetRealFileData(filename))
		{
			bRealFile = true;
			return true;
		}
		auto iter = subfiles.find(filename);
		if (iter != subfiles.end())
		{
			bRealFile = false;
			return iter->second.GetFileData(filename, offset, compressedsize, size);
		}
		return false;
	}
}

bool KomfileManager::LoadMemoryFile(const std::string& filename, boost::shared_array<char>& data, int& length)
{
	std::string tempFile = filename;
	std::transform(tempFile.begin(), tempFile.end(), tempFile.begin(), ::tolower);
	auto iter = subfiles.find(tempFile);
	if (!m_bRealFileFirst)
	{
		if (iter == subfiles.end())
			return false;
		length = iter->second.GetSize();
		data.reset(new char[length + 1]);
		data[length] = '\0';
		return iter->second.WriteDecompressed(data.get());
	}
	else
	{
		if (LoadRealFile(tempFile, data, length))
			return true;
		else if (iter != subfiles.end())
		{
			length = iter->second.GetSize();
			data.reset(new char[length + 1]);
			data[length] = '\0';
			return iter->second.WriteDecompressed(data.get());
		}
	}
	return false;
}

bool KomfileManager::GetRealFileData(std::string& filename)
{
	std::string strFullPath = "";
	int fhdl;
	errno_t erno;
	for (int i = 0; i < (int)m_vecStrDir.size(); i++)
	{
		strFullPath = m_vecStrDir[i];
		strFullPath.append(filename);
		erno = _sopen_s(&fhdl, strFullPath.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD);
		if (fhdl == -1)
			break;
	}
	if (strFullPath == "")
		return false;
	filename = strFullPath;
	return true;
}

bool KomfileManager::LoadRealFile(const std::string& strFileName, boost::shared_array<char>& data, int& length)
{
	int fhdl;
	std::string strFullPath = "";
	for (const auto& dir : m_vecStrDir)
	{
		strFullPath = dir + strFileName;
		if (_sopen_s(&fhdl, strFullPath.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD) == 0)
			break;
	}
	if (strFullPath.empty())
		return false;
	if (_sopen_s(&fhdl, strFullPath.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD) != 0)
		return false;
	_lseek(fhdl, 0, SEEK_END);
	length = _tell(fhdl);
	_lseek(fhdl, 0, SEEK_SET);
	data.reset(new char[length + 1]);
	_read(fhdl, data.get(), length);
	_close(fhdl);
	data[length] = 0;
	return true;
}

GCMemoryFile::GCMemoryFile(boost::shared_ptr<KomfileManager> manager, std::string filename)
	:m_pMFManager(manager), m_iCursorPos(0), m_pCursor(NULL), m_iSize(0)
{
	m_bLoaded = m_pMFManager->LoadMemoryFile(filename, m_pData, m_iSize);
	if (m_bLoaded)
		m_pCursor = m_pData.get();
}

GCMemoryFile::~GCMemoryFile()
{
}

int GCMemoryFile::ReadFile(void* pDest, int ReadBytes)
{
	if (m_iCursorPos + ReadBytes > m_iSize)
		return 0;
	memcpy(pDest, m_pCursor, ReadBytes);
	m_pCursor += ReadBytes;
	m_iCursorPos += ReadBytes;
	return ReadBytes;
}

int GCMemoryFile::FileSeek(int iPos, int Pivot)
{
	if (Pivot == FILESEEK_CUR)
		m_iCursorPos += iPos;
	else if (Pivot == FILESEEK_SET)
		m_iCursorPos = iPos;
	else if (Pivot == FILESEEK_END)
		m_iCursorPos = (m_iSize - 1) + iPos;
	m_pCursor = m_pData.get() + m_iCursorPos;
	return m_iCursorPos;
}

GCThreadMemoryFile::GCThreadMemoryFile(boost::shared_ptr<KomfileManager> manager, std::string filename)
	: m_iCursorPos(0)
	, m_pCursor(NULL)
	, m_iSize(0)
	, m_bFileValid(false)
	, m_bLoaded(false)
	, m_bRealFile(false)
{
	m_sFileData.strFileName = filename;
	m_bFileValid = manager->LoadGetFileData(m_sFileData.strFileName, m_sFileData.offset, m_sFileData.compressedsize, m_sFileData.size, m_bRealFile);
}

GCThreadMemoryFile::~GCThreadMemoryFile()
{
}

void GCThreadMemoryFile::LoadMemoryFile()
{
	m_bLoaded = false;
	if (m_bRealFile)
	{
		int fhdl = 0;
		int length = 0;
		if (_sopen_s(&fhdl, m_sFileData.strFileName.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD) != 0)
		{
			m_bLoaded = false;
			return;
		}
		length = _lseek(fhdl, 0, SEEK_END);
		_lseek(fhdl, 0, SEEK_SET);
		m_pData.reset(new char[length + 1]);
		if (_read(fhdl, m_pData.get(), length) != length)
		{
			_close(fhdl);
			m_bLoaded = false;
			return;
		}
		_close(fhdl);
		m_pData[length] = 0;
		m_iSize = length;
		m_pCursor = m_pData.get();
		m_bLoaded = true;
	}
	else
	{
		int length = m_sFileData.size;
		m_pData.reset(new char[length + 1]);
		m_pData[length] = 0;
		std::ifstream fileStream(m_sFileData.strFileName.c_str(), std::ios_base::in | std::ios_base::binary);
		fileStream.seekg(m_sFileData.offset, std::ios_base::beg);
		std::vector<char> compressedBuffer(m_sFileData.compressedsize);
		fileStream.read(compressedBuffer.data(), m_sFileData.compressedsize);
		unsigned long ulSize = length;
		DecryptBufferPlus((int)0xC7, compressedBuffer.data(), m_sFileData.compressedsize);
		uncompress((BYTE*)(m_pData.get()), &ulSize, (BYTE*)compressedBuffer.data(), m_sFileData.compressedsize);
		if (ulSize != length)
		{
			m_bLoaded = false;
			return;
		}
		m_iSize = length;
		m_pCursor = m_pData.get();
		m_bLoaded = true;
	}
}

int GCThreadMemoryFile::ReadFile(void* pDest, int ReadBytes)
{
	if (m_iCursorPos + ReadBytes > m_iSize)
		return 0;
	memcpy(pDest, m_pCursor, ReadBytes);
	m_pCursor += ReadBytes;
	m_iCursorPos += ReadBytes;
	return ReadBytes;
}

int GCThreadMemoryFile::FileSeek(int iPos, int Pivot)
{
	if (Pivot == FILESEEK_CUR)
		m_iCursorPos += iPos;
	else if (Pivot == FILESEEK_SET)
		m_iCursorPos = iPos;
	else if (Pivot == FILESEEK_END)
		m_iCursorPos = (m_iSize - 1) + iPos;
	m_pCursor = m_pData.get() + m_iCursorPos;
	return m_iCursorPos;
}