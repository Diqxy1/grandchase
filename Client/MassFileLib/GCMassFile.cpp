#include "stdafx.h"
#include "Encryption/Sha1.h"
#include "Encryption/BlowFish.h"
#include "Encryption/adler32.h"
#include <GCUtil.h>
#include <stdafx_boost.h>
#include <iosfwd>
#include <sstream>
#include <memory>
#include <cctype>
#include "GCBufferManager.h"
#include "GCMassFile.h"
#include "Tinyxm/tinyxml.h"
#include "XorCRC.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

const unsigned char XOR_HEADER[100] = {
    0x80, 0x25, 0xB7, 0x42, 0xB8, 0x89, 0xE7, 0x4C,
    0x06, 0x2D, 0x55, 0x83, 0xAF, 0x44, 0x99, 0x95,
    0x57, 0x19, 0xC6, 0x2D, 0x56, 0x68, 0x3A, 0xCC,
    0x4A, 0x71, 0x80, 0x9D, 0x0B, 0x70, 0x70, 0x8A,
    0x0F, 0x54, 0x9C, 0x1B, 0x06, 0xC0, 0xEA, 0x3C,
    0x60, 0x3B, 0x0B, 0xB1, 0x90, 0x5C, 0x4A, 0xF8,
    0x80, 0x28, 0xB1, 0x45, 0xB6, 0x85, 0xE7, 0x4C,
    0x06, 0x2D, 0x55, 0x83, 0xAF, 0x44, 0x99, 0x95,
    0xD9, 0x98, 0xBF, 0xAE, 0x53, 0x43, 0x63, 0xC8,
    0x4A, 0x71, 0x80, 0x9D, 0x0B, 0x70, 0x70, 0x8A,
    0x0F, 0x54, 0x9C, 0x1B, 0x06, 0xC0, 0xEA, 0x3C,
    0xC0, 0x88, 0x71, 0x48, 0xB3, 0xB9, 0x45, 0x78,
    0x29, 0x33, 0x35, 0x40
};

GCBufferManager GCMassFile::m_BufferManager1;
GCBufferManager GCMassFile::m_BufferManager2;

class KSortFunctor
{
public:
    std::string GetFileName(const std::string& p)
    {
        std::string fileName = p;
        GCUTIL_STR::ToLower(fileName);

        fileName = fileName.substr(fileName.find_last_of('\\') + 1, fileName.length());
        return fileName;
    };

    bool operator()(const std::string& p, const std::string& q)
    {
        std::string first = GetFileName(p);
        std::string second = GetFileName(q);

        return 0 > strcmp(first.c_str(), second.c_str());
    }
};

GCMassFile::GCMassFile()
{
    m_fhdl = -1;
    memset(&m_kMassFileHeader, 0, sizeof(MASSFILE_HEADER));
    m_kMassFileHeader.bCompressed = true;
}

GCMassFile::~GCMassFile()
{
    if (m_fhdl != -1)
    {
        _close(m_fhdl);
        m_fhdl = -1;
    }

    if (m_pXMLHeader)
    {
        delete[] m_pXMLHeader;
    }
}

void GCMassFile::ClearMassFile()
{
    if (m_fhdl != -1)
    {
        _close(m_fhdl);
        m_fhdl = -1;
    }
    m_iFileVersion = 0;
    m_vecMemberFileHeader.clear();
    memset(m_strMassFileName, 0, sizeof(char) * MAX_PATH);
    memset(&m_kMassFileHeader, 0, sizeof(MASSFILE_HEADER));
    m_kMassFileHeader.bCompressed = true;
}

UINT GCMassFile::GetFileCount()
{
    return m_bIsNewVersion == false ? m_kMassFileHeader.uiTotalFileNum : m_kMassFileNewHeader.nFiles;
}

bool GCMassFile::IsCompressed()
{
    return m_bIsNewVersion == false ? m_kMassFileHeader.bCompressed : m_kMassFileNewHeader.bCompressed == 1;
}
void GCMassFile::GetFileName(char* pBuffer, size_t size, char* strFilePath)
{
    int iLen = static_cast<int>(strlen(strFilePath));
    for (int i = iLen - 1; i > 0; i--)
    {
        if (strFilePath[i] == '\\' || strFilePath[i] == '/')
        {
            strcpy_s(pBuffer, size, &strFilePath[i + 1]);
            return;
        }
    }
    strcpy_s(pBuffer, size, strFilePath);
    return;
}

void GCMassFile::DecryptBufferPlus(int key, char* data, int length)
{
    for (int i = 0; i < length; i++) {
        data[i] ^= XOR_HEADER[i % sizeof(XOR_HEADER)];
    }
    for (int i = 0; i < length; i++) {
        data[i] ^= key;
    }
}

void GCMassFile::EncryptBufferPlus(int key, char* data, int length)
{
    for (int i = 0; i < length; i++) {
        data[i] ^= key;
    }
    for (int i = 0; i < length; i++) {
        data[i] ^= XOR_HEADER[i % sizeof(XOR_HEADER)];
    }
}

struct MASSFILE_MEMBERFILEHEADER_
{
    WCHAR strFileName[MASSFILE_NAMESIZE];
    long RealSize;
    long CompressSize;
    long offSet;		//RealData�κп����� OffSet�Դϴ�

    MASSFILE_MEMBERFILEHEADER_()
    {
        ZeroMemory(strFileName, sizeof(char) * MASSFILE_NAMESIZE);
        RealSize = 0;
        CompressSize = 0;
        offSet = 0;
    }
};


bool GCMassFile::LoadMassFile(const char* strMassFile, std::string* strErrorLog /* = NULL */)
{
    if (m_fhdl != -1)
    {
        _close(m_fhdl);
        m_fhdl = -1;
    }

    errno_t erno = _sopen_s(&m_fhdl, strMassFile, _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD);
    if (m_fhdl == -1)
    {
        if (strErrorLog != NULL)
            *strErrorLog = "LoadMassfile Error = _sopen_s failure";
        return false;
    }

    strcpy(m_strMassFileName, strMassFile);
    m_vecMemberFileHeader.clear();

    int buffersize = 0;
    unsigned int key = 0;

    const std::string fileName = strMassFile;
    bool isOFC = fileName.find(".first") != std::string::npos;

    _read(m_fhdl, &buffersize, sizeof(buffersize));
    char* buffer = new char[buffersize];
    _read(m_fhdl, buffer, buffersize);
    _read(m_fhdl, &key, sizeof(key));

    this->DecryptBufferPlus(key, buffer, buffersize);

    memset(&m_kMassFileNewHeader, 0, sizeof(MASSFILE_NEWHEADER));
    memcpy(&m_kMassFileNewHeader, buffer, sizeof(MASSFILE_NEWHEADER));

    m_pXMLHeader = new unsigned char[m_kMassFileNewHeader.nHeaderXML + 1];
    memcpy(m_pXMLHeader, &buffer[sizeof(MASSFILE_NEWHEADER)], m_kMassFileNewHeader.nHeaderXML);
    m_pXMLHeader[m_kMassFileNewHeader.nHeaderXML] = '\0';
    delete[] buffer;
    m_iFileVersion = m_kMassFileNewHeader.wVersion;
    m_iBuffersize = buffersize;

    m_bIsNewVersion = false;

    switch (m_iFileVersion)
    {
    case 1:
    case 2:

        for (int i = 0; i < static_cast<int>(m_kMassFileHeader.uiTotalFileNum); i++)
        {
            MASSFILE_MEMBERFILEHEADER kMemberHeader;
            _read(m_fhdl, &kMemberHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
            std::string strName = kMemberHeader.strFileName;
            _strlwr((char*)strName.c_str());
            sprintf(kMemberHeader.strFileName, strName.c_str());
            m_vecMemberFileHeader.push_back(kMemberHeader);
            if (kMemberHeader.strFileName == "")
            {
                _close(m_fhdl);
                m_fhdl = -1;
                if (strErrorLog != NULL)
                    *strErrorLog = "LoadMassfile Error = Filename is empty";
                return false; //�����̸� ������ �� �ִ� ���..
            }
        }
        break;

    case 3:
    case 4:
    {
        DWORD filetime;
        DWORD adler32;
        DWORD headersize;
        DWORD dwBytesRead = 0;

        dwBytesRead = _read(m_fhdl, &filetime, sizeof(DWORD));
        if (dwBytesRead != sizeof(DWORD))
            return false;

        dwBytesRead = _read(m_fhdl, &adler32, sizeof(DWORD));
        if (dwBytesRead != sizeof(DWORD))
            return false;

        dwBytesRead = _read(m_fhdl, &headersize, sizeof(DWORD));
        if (dwBytesRead != sizeof(DWORD))
            return false;

        boost::scoped_array<char> header(new char[headersize]);

        dwBytesRead = _read(m_fhdl, header.get(), headersize);
        if (dwBytesRead != headersize)
            return false;

        xmlDoc* doc;
        doc = xmlReadMemory(header.get(), headersize, "Komfiles.xml", NULL, 0);

        xmlNode* files = xmlDocGetRootElement(doc);
        long offset = 72 + headersize - (sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_kMassFileHeader.uiTotalFileNum);

        if (files->type == XML_ELEMENT_NODE && strcmp((char*)files->name, "Files") == 0)
        {
            xmlNode* file = files->children;
            while (file)
            {
                if (file->type == XML_ELEMENT_NODE && strcmp((char*)file->name, "File") == 0)
                {
                    MASSFILE_MEMBERFILEHEADER MemberHeader;
                    xmlAttr* attribute = file->properties;
                    while (attribute)
                    {
                        if (strcmp((char*)attribute->name, "Name") == 0)
                        {
                            strcpy(MemberHeader.strFileName, (char*)attribute->children->content);
                            _strlwr(MemberHeader.strFileName);
                        }
                        else if (strcmp((char*)attribute->name, "CompressedSize") == 0)
                        {
                            sscanf((char*)attribute->children->content, "%d", &MemberHeader.ulCompressSize);
                        }
                        else if (strcmp((char*)attribute->name, "Size") == 0)
                        {
                            sscanf((char*)attribute->children->content, "%d", &MemberHeader.ulRealSize);
                        }
                        attribute = attribute->next;
                    }
                    MemberHeader.ulOffSet = offset;
                    offset += MemberHeader.ulCompressSize;
                    m_vecMemberFileHeader.push_back(MemberHeader);
                }
                file = file->next;
            }
        }
        xmlFreeDoc(doc);
    }
    break;
    case MAKEWORD(0, 11):
    {
        m_bIsNewVersion = true;
        xmlDoc* doc;
        doc = xmlReadMemory((char*)m_pXMLHeader, m_kMassFileNewHeader.nHeaderXML, "Komfiles.xml", NULL, 0);

        xmlNode* files = xmlDocGetRootElement(doc);

        long offset = m_iBuffersize + sizeof(buffersize);

        if (isOFC)
        {
            offset += 4;
        }

        if (files->type == XML_ELEMENT_NODE && strcmp((char*)files->name, "Files") == 0)
        {
            xmlNode* file = files->children;
            while (file)
            {
                if (file->type == XML_ELEMENT_NODE && strcmp((char*)file->name, "File") == 0)
                {
                    MASSFILE_MEMBERFILEHEADER MemberHeader;
                    xmlAttr* attribute = file->properties;
                    while (attribute)
                    {
                        if (strcmp((char*)attribute->name, "Name") == 0)
                        {
                            strcpy(MemberHeader.strFileName, (char*)attribute->children->content);
                            _strlwr(MemberHeader.strFileName);
                        }
                        else if (strcmp((char*)attribute->name, "Size") == 0)
                        {
                            sscanf((char*)attribute->children->content, "%d", &MemberHeader.ulRealSize);
                        }
                        else if (strcmp((char*)attribute->name, "CompressedSize") == 0)
                        {
                            sscanf((char*)attribute->children->content, "%d", &MemberHeader.ulCompressSize);
                        }

                        attribute = attribute->next;
                    }
                    MemberHeader.ulOffSet = offset;
                    offset += MemberHeader.ulCompressSize;
                    m_vecMemberFileHeader.push_back(MemberHeader);
                }
                file = file->next;
            }
        }
        xmlFreeDoc(doc);
    }
    break;
    default:
        return false;
    }

    if (strErrorLog != NULL)
        *strErrorLog = "LoadMassfile Error = None";

    delete[] m_pXMLHeader;
    m_pXMLHeader = nullptr;

    return true;
}

int GCMassFile::UpgradeMassFile(const char* strMassFile)
{
    std::vector<std::string> vecOldFileList;

    if (LoadMassFile(strMassFile) == false)
    {
        return -1;
    }

    UINT FileNo = GetFileCount();
    GCMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
    memset(&FileInfo, 0, sizeof(FileInfo));
    FileInfo.dwFlag = MFI_REALDATA;
    int bCRCFile = 0;

    vecOldFileList.clear();
    for (int i = 0; i < static_cast<int>(FileNo); i++)
    {
        GetMemberFile(i, &FileInfo);

        if (FileInfo.strFileName == "crc.xml")
        {
            bCRCFile++;
        }
        if (FileInfo.pRealData == NULL)
        {
            return -1;
        }
        vecOldFileList.push_back(FileInfo.strFileName);
    }

    SetFileAttributesA(strMassFile, FILE_ATTRIBUTE_NORMAL);
    ModifyMassFile(this, (char*)"./Temp.Newmassfile", &vecOldFileList, true);
    ReleaseMassFile();
    CopyFileA("./Temp.Newmassfile", strMassFile, FALSE);
    DeleteFileA("./Temp.Newmassfile");
    return 0;
}

bool GCMassFile::GetMemberFile(int i, char** data, int* length)
{
    MASSFILE_MEMBERFILEHEADER* header = &m_vecMemberFileHeader[i];
    *length = header->ulRealSize;

    long lPosRealDataPos = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_kMassFileHeader.uiTotalFileNum;

    (*data) = new char[header->ulRealSize + 1];

    if (m_kMassFileHeader.bCompressed)
    {
        char* pCompressedData = new char[header->ulCompressSize];
        _lseek(m_fhdl, lPosRealDataPos + header->ulOffSet, SEEK_SET);
        _read(m_fhdl, pCompressedData, sizeof(char) * header->ulCompressSize);
        unsigned long ulSize = header->ulRealSize;
        uncompress((BYTE*)(*data), &ulSize, (BYTE*)pCompressedData, header->ulCompressSize);
        delete[] pCompressedData;
    }
    else
    {
        _lseek(m_fhdl, lPosRealDataPos + header->ulOffSet, SEEK_SET);
        _read(m_fhdl, (*data), sizeof(char) * header->ulRealSize);
    }
    (*data)[header->ulRealSize] = 0;
    return true;
}

bool GCMassFile::GetMemberFile(IN MASSFILE_MEMBERFILEHEADER& kMemberFileHeader_, OUT MASSFILE_MEMBERFILEINFO* pFileInfo_, bool bDecompress_)
{
    if (kMemberFileHeader_.ulRealSize == 0)
        return false;
    pFileInfo_->ulSize = kMemberFileHeader_.ulRealSize;
    strcpy(pFileInfo_->strFileName, kMemberFileHeader_.strFileName);
    pFileInfo_->pRealData = NULL;
    if (pFileInfo_->dwFlag & MFI_REALDATA)
    {
        long lPosRealDataPos = 0;
        if (!m_bIsNewVersion)
        {
            lPosRealDataPos = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_kMassFileHeader.uiTotalFileNum;
        }

        if (m_kMassFileHeader.bCompressed)
        {
            char* pCompressedData = m_BufferManager2.GetBuffer(kMemberFileHeader_.ulCompressSize);
            _lseek(m_fhdl, lPosRealDataPos + kMemberFileHeader_.ulOffSet, SEEK_SET);
            _read(m_fhdl, pCompressedData, sizeof(char) * kMemberFileHeader_.ulCompressSize);

            if (bDecompress_ == false)
            {
                pFileInfo_->pRealData = pCompressedData;
                pFileInfo_->ulSize = kMemberFileHeader_.ulRealSize;
                pFileInfo_->ulCompressedSize = kMemberFileHeader_.ulCompressSize;
            }
            else
            {
                DecryptBufferPlus((int)0xC7, pCompressedData, kMemberFileHeader_.ulCompressSize);

                pFileInfo_->pRealData = (char*)m_BufferManager1.GetBuffer(kMemberFileHeader_.ulRealSize);
                unsigned long ulSize = kMemberFileHeader_.ulRealSize;
                uncompress((BYTE*)pFileInfo_->pRealData, &ulSize, (BYTE*)pCompressedData, kMemberFileHeader_.ulCompressSize);
            }
        }
        else
        {
            pFileInfo_->pRealData = (char*)m_BufferManager1.GetBuffer(kMemberFileHeader_.ulRealSize);
            _lseek(m_fhdl, lPosRealDataPos + kMemberFileHeader_.ulOffSet, SEEK_SET);
            _read(m_fhdl, pFileInfo_->pRealData, sizeof(char) * kMemberFileHeader_.ulRealSize);
        }
    }
    return true;
}
bool GCMassFile::GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut, bool bDecompress_)
{
    if (m_fhdl == -1)
        return false;

    MASSFILE_MEMBERFILEHEADER kMemberFileHeader;
    memset(&kMemberFileHeader, 0, sizeof(MASSFILE_MEMBERFILEHEADER));
    for (int i = 0; i < static_cast<int>(m_vecMemberFileHeader.size()); i++)
    {
        if (strcmp(_strlwr(m_vecMemberFileHeader[i].strFileName), _strlwr((char*)strRealFile)) == 0)
        {
            kMemberFileHeader = m_vecMemberFileHeader[i];
            break;
        }
    }
    return GetMemberFile(kMemberFileHeader, pOut, bDecompress_);
}

bool GCMassFile::GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut, bool bDecompress_)
{
    if (m_fhdl == -1)
        return false;

    if (iFileIndex < 0 || iFileIndex >= static_cast<int>(m_vecMemberFileHeader.size()))
        return false;

    MASSFILE_MEMBERFILEHEADER kMemberFileHeader = m_vecMemberFileHeader[iFileIndex];
    return GetMemberFile(kMemberFileHeader, pOut, bDecompress_);
}

bool GCMassFile::GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut, bool bDecompress_)
{
    if (LoadMassFile(strMassFile) == false)
        return false;
    bool bRet = GetMemberFile(strRealFile, pOut, bDecompress_);
    ClearMassFile();
    return bRet;
}

bool GCMassFile::GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut, bool bDecompress_)
{
    if (LoadMassFile(strMassFile) == false)
        return false;
    bool bRet = GetMemberFile(iFileIndex, pOut, bDecompress_);
    ClearMassFile();
    return bRet;
}



long GCMassFile::GetFileSize(const char* strFileName)
{
    int fhdl;
    errno_t erno = _sopen_s(&fhdl, strFileName, _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD);
    if (fhdl == -1)
        return -1;
    long lSize = _filelength(fhdl);
    _close(fhdl);
    return lSize;
}

bool GCMassFile::SaveMassFile(char* strFileName, std::vector<char*>* pVecFile, bool bZip)
{
    int fhdl;
    errno_t erno = _sopen_s(&fhdl, strFileName, _O_WRONLY | _O_CREAT | _O_BINARY, _SH_DENYNO, _S_IWRITE);
    if (fhdl == -1)
        return false;

    //MASSFILE_HEADER
    MASSFILE_HEADER Header;
    Header.uiTotalFileNum = static_cast<UINT>(pVecFile->size());
    Header.bCompressed = bZip;
    sprintf(Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.", MASSFILE_VERSION / 10, MASSFILE_VERSION % 10);
    _write(fhdl, &Header, sizeof(MASSFILE_HEADER));

    //MASSFILE_MEMBERFILEHEADER

    MASSFILE_MEMBERFILEHEADER MemberFileHeader;
    long TotalSize = 0;
    for (int i = 0; i < static_cast<int>(pVecFile->size()); i++)
    {
        _write(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
    }
    TotalSize = 0;

    //RealFile
    char* pFileData = 0;
    long filesize;

    for (int i = 0; i < static_cast<int>(pVecFile->size()); i++)
    {
        filesize = GetFileSize((*pVecFile)[i]);
        int mfhdl;
        errno_t erno = _sopen_s(&mfhdl, (*pVecFile)[i], _O_RDONLY | _O_BINARY, _SH_DENYNO, _S_IREAD);
        if (mfhdl == -1)
        {
            MessageBoxA(NULL, "Unknown Error", "Error", 0);
            return false;
        }

        unsigned long WriteByte = 0;

        if (bZip)
        {
            pFileData = (char*)m_BufferManager2.GetBuffer(filesize);

            char* pDestData = m_BufferManager1.GetBuffer(compressBound(filesize));
            WriteByte = m_BufferManager1.GetBufferSize();

            _read(mfhdl, pFileData, sizeof(char) * filesize);

            if (Z_OK != compress((BYTE*)pDestData, &WriteByte, (BYTE*)pFileData, filesize))
            {
                _close(fhdl);
                _close(mfhdl);
                return false;
            }
            _write(fhdl, pDestData, sizeof(char) * WriteByte);
        }
        else
        {
            pFileData = (char*)m_BufferManager1.GetBuffer(filesize);
            _read(mfhdl, pFileData, sizeof(char) * filesize);
            _write(fhdl, pFileData, sizeof(char) * filesize);
            WriteByte = filesize;
        }

        _close(mfhdl);

        //==============================
        //���� ���� Ŀ���� �޾ƿµ�..
        int cursor = _tell(fhdl);

        //Header�� �����ϰ� �ٽ� Ŀ�� ���ڸ���...
        _lseek(fhdl, sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * i, SEEK_SET);
        _read(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
        _lseek(fhdl, sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * i, SEEK_SET);

        memset(MemberFileHeader.strFileName, 0, sizeof(char) * MASSFILE_NAMESIZE);
        GetFileName(MemberFileHeader.strFileName, sizeof(MemberFileHeader.strFileName), (*pVecFile)[i]);
        if (strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
        {
            _close(fhdl);
            MessageBoxA(NULL, MemberFileHeader.strFileName, "File Name Too Long!!!", 0);
            return false;
        }

        MemberFileHeader.ulCompressSize = WriteByte;
        MemberFileHeader.ulOffSet = TotalSize;
        TotalSize += WriteByte;

        MemberFileHeader.ulRealSize = filesize;

        _write(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));

        _read(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));

        _lseek(fhdl, cursor, SEEK_SET);
        //==============================		
    }

    _close(fhdl);
    return true;
}


bool GCMassFile::SaveMassFile()
{
    if (m_strMassFileName[0] == 0)
        return false;

    int fhdl;

    std::string newFileName = m_strMassFileName;
    newFileName = newFileName.substr(0, newFileName.find_last_of("."));
    newFileName.append(".first");

    errno_t erno = _sopen_s(&fhdl, newFileName.data(), _O_RDWR | _O_CREAT | _O_BINARY, _SH_DENYNO, _S_IWRITE | _S_IREAD);
    if (fhdl == -1)
        return false;

    MASSFILE_NEWHEADER newHeader;
    ZeroMemory(&newHeader, sizeof(MASSFILE_NEWHEADER));
    newHeader.wVersion = MAKEWORD(0, 11);
    newHeader.nFiles = static_cast<UINT>(m_vecMemberFileHeader.size());
    newHeader.bCompressed = m_kMassFileHeader.bCompressed;

    std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
    stream << "<?xml version=\"1.0\"?><Files>";

    MASSFILE_MEMBERFILEINFO OutFile;
    memset(&OutFile, 0, sizeof(MASSFILE_MEMBERFILEINFO));
    OutFile.dwFlag = MFI_REALDATA | MFI_NODECOMPRESS;

    void* pBuffer = nullptr;
    UINT  uiSize = 0;
    UINT  uiRealSize = 0;
    long FileSize = 0;
    unsigned long WriteByte = 0;
    long TotalSize = 0;

    int iFileCount = 0;
    for (int i = 0; i < m_kMassFileHeader.uiTotalFileNum; i++)
    {
        GetMemberFile(i, &OutFile, true);
        FileSize = OutFile.ulSize;
        WriteByte = OutFile.ulCompressedSize;
        pBuffer = (void*)OutFile.pRealData;
        uiSize = static_cast<UINT>(OutFile.ulCompressedSize);
        uiRealSize = static_cast<UINT>(OutFile.ulSize);

        if (m_kMassFileHeader.bCompressed)
        {
            char* pDestData = new char[FileSize];
            WriteByte = FileSize;

            ::ZeroMemory(pDestData, sizeof(char) * WriteByte);

            if (FileSize < 8)
            {
                delete[] pDestData;
                continue;
            }

            if (Z_OK != compress((BYTE*)pDestData, &WriteByte, (BYTE*)pBuffer, FileSize))
            {
                delete[] pDestData;
                continue;
            }

            EncryptBufferPlus((int)0xC7, pDestData, WriteByte);

            pBuffer = (void*)pDestData;
            uiSize = static_cast<UINT>(WriteByte);
            uiRealSize = FileSize;
        }

        _write(fhdl, pBuffer, uiSize);

        char tmp[1024];
        unsigned int adler = 0x0001;
        adler = AdlerCheckSum::adler32(adler, (unsigned char*)pBuffer, uiSize);

        time_t t;
        time(&t);

        _snprintf_s(tmp, 1024, "<File Name=\"%s\" Size=\"%d\" CompressedSize=\"%d\" Checksum=\"%08x\" FileTime=\"%08x\" Algorithm=\"%d\" />",
            OutFile.strFileName, uiRealSize, uiSize, adler, t, 0);
        stream << tmp;

        iFileCount++;
    }

    stream << "</Files>";

    newHeader.nFiles = iFileCount;
    newHeader.nHeaderXML = stream.tellp();
    newHeader.uAdler32 = AdlerCheckSum::adler32(stream, 0, static_cast<int>(newHeader.nHeaderXML));
    int buffersize = sizeof(MASSFILE_NEWHEADER) + newHeader.nHeaderXML;
    buffersize = buffersize % 8 == 0 ? buffersize : buffersize + (8 - (buffersize % 8));

    unsigned char* encBuffer = new unsigned char[buffersize];
    memset(encBuffer, 0, buffersize);
    memcpy(encBuffer, &newHeader, sizeof(MASSFILE_NEWHEADER));
    memcpy(&encBuffer[sizeof(MASSFILE_NEWHEADER)], stream.str().c_str(), newHeader.nHeaderXML);

    unsigned int encKey = XOR_HEADER[rand() % 100];
    EncryptBufferPlus(encKey, (char*)encBuffer, buffersize);

    size_t length = _filelength(fhdl);
    unsigned char* filebuffer = new unsigned char[length];
    _lseek(fhdl, 0, SEEK_SET);
    _read(fhdl, filebuffer, length);
    _lseek(fhdl, 0, SEEK_SET);

    _write(fhdl, &buffersize, sizeof(buffersize));
    _write(fhdl, encBuffer, buffersize);
    _write(fhdl, &encKey, sizeof(encKey));
    _write(fhdl, filebuffer, length);
    _close(fhdl);
    _close(m_fhdl);
    m_fhdl = -1;

    delete[] filebuffer;
    delete[] encBuffer;
    ::SetFileAttributesA(newFileName.data(), FILE_ATTRIBUTE_NORMAL);

    return true;
}

int GCMassFile::ModifyMassFile(GCMassFile* pMassFile, char* strFileName, std::vector<std::string>* pVecFile)
{
    return ModifyMassFile(pMassFile, strFileName, pVecFile, true);
}

int GCMassFile::ModifyMassFile(GCMassFile* pMassFile, char* strFileName, std::vector<std::string>* pVecFile, bool bMakeCRC)
{
    // KomFileCheckSum.xml������ ������ �غ��۾�.
    std::string strChecksumFileName = "./crc.xml";

    //2007.8.8 knight26h CRC.XML������ ���� �ϴ� ��� ã�Ƽ� �����.
    for (std::vector<std::string>::iterator it = pVecFile->begin(); it != pVecFile->end(); )
    {
        //2007.8.23 knight26h ����� �ҹ��ڷ� ���� ����
        GCUTIL_STR::ToLower(*it);
        if (IsCRCFile(*it))
        {
            it = pVecFile->erase(it);
        }
        else
        {
            it++;
        }
    }

    //���� ��Ͽ� ������ ���� ��� ����
    if (pVecFile->size() <= 0) return 0;

    //2007.06.25 milennium9
    // FileList�� RealFile ���� Ǯ��ΰ� ����Ǿ� �����Ƿ� Sorting �� ���ϸ��� �����ؾ� �Ѵ�.
    // Functor�� ��������.
    std::sort(pVecFile->begin(), pVecFile->end(), KSortFunctor());

    int fhdl;
    errno_t erno = _sopen_s(&fhdl, "./Temp.massfile", _O_RDWR | _O_CREAT | _O_BINARY, _SH_DENYNO, _S_IWRITE | _S_IREAD);
    if (fhdl == -1)
        return -1;

    long FileSize = 0;
    unsigned long WriteByte = 0;
    long TotalSize = 0;

    //MASSFILE_HEADER
    MASSFILE_NEWHEADER Header;
    ZeroMemory(&Header, sizeof(MASSFILE_NEWHEADER));
    Header.wVersion = MAKEWORD(0, 11);
    Header.nFiles = static_cast<UINT>(pVecFile->size());
    Header.bCompressed = pMassFile->IsCompressed();

    std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
    stream << "<?xml version=\"1.0\"?><Files>";

    MASSFILE_MEMBERFILEINFO OutFile;
    memset(&OutFile, 0, sizeof(MASSFILE_MEMBERFILEINFO));
    OutFile.dwFlag = MFI_REALDATA | MFI_NODECOMPRESS;

    void* pBuffer = NULL;
    UINT  uiSize = 0;
    UINT  uiRealSize = 0;
    std::vector<std::string>::iterator it = pVecFile->begin();

    while (it != pVecFile->end())
    {
        if (!pMassFile->GetMemberFile(it->c_str(), &OutFile, false))
        {
            //���� ������ �ε��ؼ� ����
            //������ �ȿ� ����
            FileSize = GetFileSize(it->c_str());
            char* pFileData = (char*)m_BufferManager2.GetBuffer(FileSize);
            char* pDestData = m_BufferManager1.GetBuffer(compressBound(FileSize));
            WriteByte = m_BufferManager1.GetBufferSize();

            int mfhdl;
            _sopen_s(&mfhdl, it->c_str(), _O_RDONLY | _O_BINARY, _SH_DENYNO, _S_IREAD);

            if (mfhdl == -1) {
                return -1;
            }

            ::ZeroMemory(pFileData, sizeof(char) * FileSize);
            _read(mfhdl, pFileData, sizeof(char) * FileSize);
            ::ZeroMemory(pDestData, sizeof(char) * WriteByte);

            if (Z_OK != compress((BYTE*)pDestData, &WriteByte, (BYTE*)pFileData, FileSize))
            {
                _close(fhdl);
                _close(mfhdl);
                return -2;
            }

            std::string filenamex = it->substr(it->find_last_of("\\") + 1, it->length());
            std::transform(filenamex.begin(), filenamex.end(), filenamex.begin(), tolower);

            EncryptBufferPlus((int)0xC7, pDestData, WriteByte);

            _close(mfhdl);
            pBuffer = (void*)pDestData;
            uiSize = static_cast<UINT>(WriteByte);
            uiRealSize = FileSize;
            it->erase(0, it->find_last_of("\\") + 1);
        }
        else
        {
            //���⼭�� �ٷ� ������ �� �ִ�
            FileSize = OutFile.ulSize;
            WriteByte = OutFile.ulCompressedSize;
            pBuffer = (void*)OutFile.pRealData;
            uiSize = static_cast<UINT>(OutFile.ulCompressedSize);
            uiRealSize = static_cast<UINT>(OutFile.ulSize);
        }

        _write(fhdl, pBuffer, uiSize);

        char tmp[1024];
        unsigned int adler = 0x0001;
        adler = AdlerCheckSum::adler32(adler, (unsigned char*)pBuffer, uiSize);

        _snprintf_s(tmp, 1024, "<File Name=\"%s\" Size=\"%d\" CompressedSize=\"%d\" Checksum=\"%08x\" FileTime=\"%08x\" Algorithm=\"%d\" />",
            it->c_str(), uiRealSize, uiSize, adler, 0, 0);
        stream << tmp;

        it++;
    }

    stream << "</Files>";

    Header.nHeaderXML = stream.tellp();
    Header.uAdler32 = AdlerCheckSum::adler32(stream, 0, static_cast<int>(Header.nHeaderXML));
    int buffersize = sizeof(MASSFILE_NEWHEADER) + Header.nHeaderXML;
    buffersize = buffersize % 8 == 0 ? buffersize : buffersize + (8 - (buffersize % 8));

    unsigned char* encBuffer = new unsigned char[buffersize];
    memset(encBuffer, 0, buffersize);
    memcpy(encBuffer, &Header, sizeof(MASSFILE_NEWHEADER));
    memcpy(&encBuffer[sizeof(MASSFILE_NEWHEADER)], stream.str().c_str(), Header.nHeaderXML);

    std::string fileName = strFileName;

    unsigned int encKey = XOR_HEADER[rand() % 100];
    EncryptBufferPlus(encKey, (char*)encBuffer, buffersize);

    size_t length = _filelength(fhdl);
    unsigned char* filebuffer = new unsigned char[length];
    _lseek(fhdl, 0, SEEK_SET);
    _read(fhdl, filebuffer, length);
    _lseek(fhdl, 0, SEEK_SET);

    _write(fhdl, &buffersize, sizeof(buffersize));
    _write(fhdl, encBuffer, buffersize);
    _write(fhdl, &encKey, sizeof(encKey));
    _write(fhdl, filebuffer, length);
    _close(fhdl);

    if (m_fhdl > 0)
        _close(m_fhdl);
    m_fhdl = -1;

    delete[] filebuffer;
    delete[] encBuffer;

    // �ϴ� ���� ���� �����Ͽ� ���� ��Ų��.
    ::SetFileAttributesA(strFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/);

    std::string bkpFileName = strFileName;
    bkpFileName.append(".bkp");

    if (GetFileAttributesA(strFileName) != INVALID_FILE_ATTRIBUTES)
    {
        if (MoveFileA(strFileName, bkpFileName.data()) == false)
        {
            return 0;
        }
    }

    CopyFileA("./Temp.massfile", strFileName, FALSE);
    DeleteFileA("./Temp.massfile");
    DeleteFileA(strChecksumFileName.c_str());
    return 0;
}

bool GCMassFile::IsCRCFile(std::string strFileName)
{
    //���丮 ��ȣ ����
    std::string strSubStr;
    strFileName = ReplaceDirectoryMark(strFileName);
    int iPos = static_cast<int>(strFileName.find_last_of("\\"));

    if (iPos < 0)
    {
        strSubStr = strFileName;
    }
    else
    {
        strSubStr = strFileName.substr(iPos + 1, strFileName.length() - iPos);
    }

    std::transform(strSubStr.begin(), strSubStr.end(), strSubStr.begin(), tolower);

    if (strSubStr == "crc.xml")
    {
        return true;

    }
    return false;
}

std::string GCMassFile::ReplaceDirectoryMark(std::string strTemp)
{
    int nPoint = static_cast<int>(strTemp.find("/"));
    while (nPoint != -1)
    {
        strTemp.replace(nPoint, 1, "\\");
        nPoint = static_cast<int>(strTemp.find("/", nPoint + 1));
    }
    return strTemp;
}

int GCMassFile::ModifyMassFile(GCMassFile* kOriginal_, GCMassFile* kSource_, char* strFileName, std::vector<std::string>* pVecFile, bool bMakeCRC /*= false*/)
{
    // KomFileCheckSum.xml������ ������ �غ��۾�.
    std::string strChecksumFileName = "./crc.xml";

    //2007.8.8 knight26h CRC.XML������ ���� �ϴ� ��� ã�Ƽ� �����.
    for (std::vector<std::string>::iterator it = pVecFile->begin(); it != pVecFile->end(); )
    {
        //2007.8.23 knight26h ����� �ҹ��ڷ� ���� ����
        GCUTIL_STR::ToLower(*it);
        if (IsCRCFile(*it))
        {
            it = pVecFile->erase(it);
        }
        else
        {
            it++;
        }
    }

    //���� ��Ͽ� ������ ���� ��� ����
    if (pVecFile->size() <= 0) return 0;

    //2007.06.25 milennium9
    // FileList�� RealFile ���� Ǯ��ΰ� ����Ǿ� �����Ƿ� Sorting �� ���ϸ��� �����ؾ� �Ѵ�.
    // Functor�� ��������.
    std::sort(pVecFile->begin(), pVecFile->end(), KSortFunctor());

    if (bMakeCRC)
    {
        pVecFile->push_back(strChecksumFileName);
    }
    //==============================================================================
    //Xml File ������ ���� �ʱ�ȭ �۾�
    TiXmlDocument kXml(strChecksumFileName);
    TiXmlElement* pElementSet = NULL;
    InitMassFileXml(&kXml, &pElementSet);
    //==============================================================================

    int fhdl;
    errno_t erno = _sopen_s(&fhdl, "./Temp.massfile", _O_WRONLY | _O_CREAT | _O_BINARY, _SH_DENYNO, _S_IWRITE);
    if (fhdl == -1)
        return -1;

    long FileSize = 0;
    unsigned long WriteByte = 0;
    long TotalSize = 0;

    //MASSFILE_HEADER
    MASSFILE_HEADER Header;
    ZeroMemory(&Header, sizeof(MASSFILE_HEADER));
    sprintf(Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.", MASSFILE_VERSION / 10, MASSFILE_VERSION % 10);
    Header.uiTotalFileNum = static_cast<UINT>(pVecFile->size());
    Header.bCompressed = kOriginal_->IsCompressed();

    _write(fhdl, &Header, sizeof(MASSFILE_HEADER));

    MASSFILE_MEMBERFILEINFO OutFile;
    MASSFILE_MEMBERFILEHEADER MemberFileHeader;

    for (int i = 0; i < 60; ++i)
    {
        OutFile.strFileName[i] = 0;
        MemberFileHeader.strFileName[i] = 0;
    }
    OutFile.ulCompressedSize = 0;
    OutFile.dwFlag = 0;
    OutFile.pRealData = NULL;
    OutFile.ulSize = 0;

    MemberFileHeader.ulCompressSize = 0;
    MemberFileHeader.ulOffSet = 0;
    MemberFileHeader.ulRealSize = 0;


    for (int i = 0; i < static_cast<int>(pVecFile->size()); i++)
    {
        _write(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
    }
    OutFile.dwFlag = MFI_REALDATA | MFI_NODECOMPRESS;

    void* pBuffer = NULL;
    UINT  uiSize = 0;
    UINT  uiRealSize = 0;
    for (int i = 0; i < static_cast<int>(pVecFile->size()); i++)
    {
        if (kOriginal_->GetMemberFile((*pVecFile)[i].c_str(), &OutFile, false))
        {
            //���⼭�� �ٷ� ������ �� �ִ�
            _write(fhdl, OutFile.pRealData, sizeof(char) * OutFile.ulCompressedSize);
            FileSize = OutFile.ulSize;
            WriteByte = OutFile.ulCompressedSize;
            pBuffer = (void*)OutFile.pRealData;
            uiSize = static_cast<UINT>(OutFile.ulCompressedSize);
            uiRealSize = static_cast<UINT>(OutFile.ulSize);
        }
        else if (kSource_->GetMemberFile((*pVecFile)[i].c_str(), &OutFile, false))
        {
            //���⼭�� �ٷ� ������ �� �ִ�
            _write(fhdl, OutFile.pRealData, sizeof(char) * OutFile.ulCompressedSize);
            FileSize = OutFile.ulSize;
            WriteByte = OutFile.ulCompressedSize;
            pBuffer = (void*)OutFile.pRealData;
            uiSize = static_cast<UINT>(OutFile.ulCompressedSize);
            uiRealSize = static_cast<UINT>(OutFile.ulSize);
        }
        else
        {
            //���� ������ �ε��ؼ� ����
            //������ �ȿ� ����
            FileSize = GetFileSize((*pVecFile)[i].c_str());
            char* pFileData = (char*)m_BufferManager2.GetBuffer(FileSize);
            char* pDestData = m_BufferManager1.GetBuffer(compressBound(FileSize));
            WriteByte = m_BufferManager1.GetBufferSize();

            int mfhdl;
            _sopen_s(&mfhdl, (*pVecFile)[i].c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD);
            ::ZeroMemory(pFileData, sizeof(char) * FileSize);
            _read(mfhdl, pFileData, sizeof(char) * FileSize);
            ::ZeroMemory(pDestData, sizeof(char) * WriteByte);

            if (Z_OK != compress((BYTE*)pDestData, &WriteByte, (BYTE*)pFileData, FileSize))
            {
                _close(fhdl);
                _close(mfhdl);
                return false;
            }
            _write(fhdl, pDestData, sizeof(char) * WriteByte);
            _close(mfhdl);
            pBuffer = (void*)pDestData;
            uiSize = static_cast<UINT>(WriteByte);
            uiRealSize = FileSize;
        }

        // XML������ ��� ���� �ʴ´�.
        if ((i != pVecFile->size() - 1) && bMakeCRC)
        {
            std::string strFileName = (*pVecFile)[i];
            strFileName.erase(0, strFileName.find_last_of("\\") + 1);
            std::string strCheckSum;
            strCheckSum.append(CRC_32::GetCRCFromBuffer(pBuffer, uiSize));
            TiXmlElement kElement("Item");
            kElement.SetAttribute("Name", strFileName);
            kElement.SetAttribute("Size", uiRealSize);
            kElement.SetAttribute("Version", 0);
            kElement.SetAttribute("CheckSum", strCheckSum);
            pElementSet->InsertEndChild(kElement);
        }
        int cursor = _tell(fhdl);

        //Header�� �����ϰ� �ٽ� Ŀ�� ���ڸ���...
        _lseek(fhdl, sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * i, SEEK_SET);
        _read(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
        _lseek(fhdl, sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * i, SEEK_SET);

        memset(MemberFileHeader.strFileName, 0, sizeof(char) * MASSFILE_NAMESIZE);
        GetFileName(MemberFileHeader.strFileName, sizeof(MemberFileHeader.strFileName), (char*)(*pVecFile)[i].c_str());
        if (strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
        {
            _close(fhdl);
            MessageBoxA(NULL, MemberFileHeader.strFileName, "File Name Too Long!!!", 0);
            return false;
        }

        MemberFileHeader.ulCompressSize = WriteByte;
        MemberFileHeader.ulOffSet = TotalSize;
        TotalSize += WriteByte;
        MemberFileHeader.ulRealSize = FileSize;

        _write(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
        _read(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
        _lseek(fhdl, cursor, SEEK_SET);
        //====================================================================================
        // ��� ���ϵ��� �� ���� �� ������ XML File������ Kom�� ���� ��Ų��.

        if ((i == pVecFile->size() - 2) && bMakeCRC)
        {
            kXml.SaveFile();
        }
    }
    // �ϴ� ���� ���� �����Ͽ� ���� ��Ų��.
    _close(fhdl);
    ReleaseMassFile();
    ::SetFileAttributesA(strFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/);
    CopyFileA("./Temp.massfile", strFileName, FALSE);
    DeleteFileA("./Temp.massfile");
    DeleteFileA(strChecksumFileName.c_str());
    return 0;
}

int GCMassFile::ModifyMassFile(GCMassFile* kOriginal_, std::vector< GCMassFile* >& kSource_, char* strFileName, std::vector<std::string>* pVecFile, bool bMakeCRC /*= false*/, bool bOriginalFirst_)
{
    // KomFileCheckSum.xml������ ������ �غ��۾�.
    std::string strChecksumFileName = "./crc.xml";

    //2007.8.8 knight26h CRC.XML������ ���� �ϴ� ��� ã�Ƽ� �����.
    for (std::vector<std::string>::iterator it = pVecFile->begin(); it < pVecFile->end(); )
    {
        //2007.8.23 knight26h ����� �ҹ��ڷ� ���� ����
        GCUTIL_STR::ToLower(*it);
        if (IsCRCFile(*it))
        {
            it = pVecFile->erase(it);
        }
        else
        {
            it++;
        }
    }

    //���� ��Ͽ� ������ ���� ��� ����
    if (pVecFile->size() <= 0) return 0;

    //2007.06.25 milennium9
    // FileList�� RealFile ���� Ǯ��ΰ� ����Ǿ� �����Ƿ� Sorting �� ���ϸ��� �����ؾ� �Ѵ�.
    // Functor�� ��������.
    std::sort(pVecFile->begin(), pVecFile->end(), KSortFunctor());


    if (bMakeCRC)
    {
        pVecFile->push_back(strChecksumFileName);
    }
    //==============================================================================
    //Xml File ������ ���� �ʱ�ȭ �۾�
    TiXmlDocument kXml(strChecksumFileName);
    TiXmlElement* pElementSet = NULL;
    InitMassFileXml(&kXml, &pElementSet);
    //==============================================================================

    char strTempFileName[100];
    std::string strFullName = strFileName;
    int p = strFullName.find_last_of("\\");
    std::string strDeName = strFullName.substr(p + 1, strFullName.find_last_of(".") - p - 1);

    sprintf(strTempFileName, "./Temp.%s", strDeName.c_str());
    int fhdl;
    errno_t erno = _sopen_s(&fhdl, strTempFileName, _O_WRONLY | _O_CREAT | _O_BINARY, _SH_DENYNO, _S_IWRITE);
    if (fhdl == -1)
        return -1;

    long FileSize = 0;
    unsigned long WriteByte = 0;
    long TotalSize = 0;

    //MASSFILE_HEADER
    MASSFILE_HEADER Header;
    ZeroMemory(&Header, sizeof(MASSFILE_HEADER));
    sprintf(Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.", MASSFILE_VERSION / 10, MASSFILE_VERSION % 10);
    Header.uiTotalFileNum = static_cast<UINT>(pVecFile->size());
    Header.bCompressed = kOriginal_->IsCompressed();

    _write(fhdl, &Header, sizeof(MASSFILE_HEADER));

    MASSFILE_MEMBERFILEINFO OutFile;
    MASSFILE_MEMBERFILEHEADER MemberFileHeader;

    for (int i = 0; i < 60; ++i)
    {
        OutFile.strFileName[i] = 0;
        MemberFileHeader.strFileName[i] = 0;
    }
    OutFile.ulCompressedSize = 0;
    OutFile.dwFlag = 0;
    OutFile.pRealData = NULL;
    OutFile.ulSize = 0;

    MemberFileHeader.ulCompressSize = 0;
    MemberFileHeader.ulOffSet = 0;
    MemberFileHeader.ulRealSize = 0;


    for (int i = 0; i < static_cast<int>(pVecFile->size()); i++)
    {
        _write(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
    }
    OutFile.dwFlag = MFI_REALDATA | MFI_NODECOMPRESS;

    void* pBuffer = NULL;
    UINT  uiSize = 0;
    UINT  uiRealSize = 0;
    for (int i = 0; i < static_cast<int>(pVecFile->size()); i++)
    {
        bool bNotInList = true;
        for (int isrcLoop = 0; isrcLoop < static_cast<int>(kSource_.size()); ++isrcLoop)
        {
            if (kSource_[isrcLoop]->GetMemberFile((*pVecFile)[i].c_str(), &OutFile, false))
            {
                //���⼭�� �ٷ� ������ �� �ִ�
                _write(fhdl, OutFile.pRealData, sizeof(char) * OutFile.ulCompressedSize);
                FileSize = OutFile.ulSize;
                WriteByte = OutFile.ulCompressedSize;
                pBuffer = (void*)OutFile.pRealData;
                uiSize = static_cast<UINT>(OutFile.ulCompressedSize);
                uiRealSize = static_cast<UINT>(OutFile.ulSize);

                bNotInList = false;
                break;
            }
        }

        if (bNotInList)
        {
            if (kOriginal_->GetMemberFile((*pVecFile)[i].c_str(), &OutFile, false))
            {
                //���⼭�� �ٷ� ������ �� �ִ�
                _write(fhdl, OutFile.pRealData, sizeof(char) * OutFile.ulCompressedSize);
                FileSize = OutFile.ulSize;
                WriteByte = OutFile.ulCompressedSize;
                pBuffer = (void*)OutFile.pRealData;
                uiSize = static_cast<UINT>(OutFile.ulCompressedSize);
                uiRealSize = static_cast<UINT>(OutFile.ulSize);
            }
            else
            {
                //���� ������ �ε��ؼ� ����
                //������ �ȿ� ����
                FileSize = GetFileSize((*pVecFile)[i].c_str());
                // 
                if (FileSize <= 0)
                    continue;

                char* pFileData = (char*)m_BufferManager2.GetBuffer(FileSize);
                char* pDestData = m_BufferManager1.GetBuffer(compressBound(FileSize));
                WriteByte = m_BufferManager1.GetBufferSize();
                int mfhdl;
                _sopen_s(&mfhdl, (*pVecFile)[i].c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD);
                ::ZeroMemory(pFileData, sizeof(char) * FileSize);
                _read(mfhdl, pFileData, sizeof(char) * FileSize);
                ::ZeroMemory(pDestData, sizeof(char) * WriteByte);

                if (Z_OK != compress((BYTE*)pDestData, &WriteByte, (BYTE*)pFileData, FileSize))
                {
                    _close(fhdl);
                    _close(mfhdl);
                    return -1;
                }
                _write(fhdl, pDestData, sizeof(char) * WriteByte);
                _close(mfhdl);
                pBuffer = (void*)pDestData;
                uiSize = static_cast<UINT>(WriteByte);
                uiRealSize = FileSize;
            }
        }

        // XML������ ��� ���� �ʴ´�.
        if ((i != pVecFile->size() - 1) && bMakeCRC)
        {
            std::string strFileName = (*pVecFile)[i];
            strFileName.erase(0, strFileName.find_last_of("\\") + 1);
            std::string strCheckSum;
            strCheckSum.append(CRC_32::GetCRCFromBuffer(pBuffer, uiSize));
            TiXmlElement kElement("Item");
            kElement.SetAttribute("Name", strFileName);
            kElement.SetAttribute("Size", uiRealSize);
            kElement.SetAttribute("Version", 0);
            kElement.SetAttribute("CheckSum", strCheckSum);
            pElementSet->InsertEndChild(kElement);
        }
        int cursor = _tell(fhdl);

        //Header�� �����ϰ� �ٽ� Ŀ�� ���ڸ���...
        _lseek(fhdl, sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * i, SEEK_SET);
        _read(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
        _lseek(fhdl, sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * i, SEEK_SET);

        memset(MemberFileHeader.strFileName, 0, sizeof(char) * MASSFILE_NAMESIZE);
        GetFileName(MemberFileHeader.strFileName, sizeof(MemberFileHeader.strFileName), (char*)(*pVecFile)[i].c_str());
        if (strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
        {
            _close(fhdl);
            MessageBoxA(NULL, MemberFileHeader.strFileName, "File Name Too Long!!!", 0);
            return -1;
        }

        MemberFileHeader.ulCompressSize = WriteByte;
        MemberFileHeader.ulOffSet = TotalSize;
        TotalSize += WriteByte;
        MemberFileHeader.ulRealSize = FileSize;

        _write(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
        _read(fhdl, &MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER));
        _lseek(fhdl, cursor, SEEK_SET);
        //====================================================================================
        // ��� ���ϵ��� �� ���� �� ������ XML File������ Kom�� ���� ��Ų��.

        if ((i == pVecFile->size() - 2) && bMakeCRC)
        {
            kXml.SaveFile();
        }
    }
    // �ϴ� ���� ���� �����Ͽ� ���� ��Ų��.
    _close(fhdl);
    ReleaseMassFile();
    ::SetFileAttributesA(strFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/);
    CopyFileA(strTempFileName, strFileName, FALSE);
    DeleteFileA(strTempFileName);
    DeleteFileA(strChecksumFileName.c_str());
    return 0;
}

std::vector< std::string > GCMassFile::GetSortedFileNames()
{
    std::vector< std::string > vecstrFileNames;
    for (int i = 0; i < static_cast<int>(m_kMassFileHeader.uiTotalFileNum); i++)
    {
        std::string strFileName = m_vecMemberFileHeader[i].strFileName;
        vecstrFileNames.push_back(strFileName);
    }
    std::sort(vecstrFileNames.begin(), vecstrFileNames.end(), KSortFunctor());
    return vecstrFileNames;
}

bool GCMassFile::GenerateMassFileXml(std::string strMassFileName_, std::string strFilePath_)
{
    //==============================================================================
    //Xml File ������ ���� �ʱ�ȭ �۾�
    // KomFileCheckSum.xml������ ������ �غ��۾�.
    std::string strChecksumFileName = strFilePath_;
    TiXmlDocument kXml(strChecksumFileName);
    TiXmlElement* pFileElement = NULL;
    if (InitMassFileXml(&kXml, &pFileElement) == false)
    {
        return false;
    }
    if (LoadMassFile(strMassFileName_.c_str()) == false)
    {
        return false;
    }
    std::vector<std::string> vecstrFileNames = GetSortedFileNames();
    for (unsigned int i = 0; i < m_kMassFileHeader.uiTotalFileNum; i++)
    {
        UINT uiRealSize = 0;
        if (IsCRCFile(vecstrFileNames[i])) continue;
        std::string strCheckSum = GetMemberFileCheckSum(vecstrFileNames[i], uiRealSize);
        TiXmlElement kElement("Item");
        kElement.SetAttribute("Name", vecstrFileNames[i]);
        kElement.SetAttribute("Size", uiRealSize);
        kElement.SetAttribute("Version", 0);
        kElement.SetAttribute("CheckSum", strCheckSum);
        pFileElement->InsertEndChild(kElement);
    }
    ReleaseMassFile();
    kXml.SaveFile();
    return true;
}

std::string GCMassFile::GetMemberFileCheckSum(std::string& strMemberFileName_, UINT& uiRealSize_)
{
    std::string strCheckSum;
    UINT uiCompressedSize = 0;
    UINT uiRealSize = 0;
    char* pBuffer = GetCompressedFile(strMemberFileName_, uiCompressedSize, uiRealSize);

    strCheckSum.append(CRC_32::GetCRCFromBuffer(pBuffer, uiCompressedSize));
    uiRealSize_ = uiRealSize;
    return strCheckSum;
}

char* GCMassFile::GetCompressedFile(std::string& strMemberFileName_, UINT& uiCompressedSize_, UINT& uiRealSize_)
{
    int iIndex = 0;
    for (int i = 0; i < static_cast<int>(m_kMassFileHeader.uiTotalFileNum); i++)
    {
        if (strcmp(m_vecMemberFileHeader[i].strFileName, strMemberFileName_.c_str()) == 0)
        {
            iIndex = i;
            break;
        }
    }
    MASSFILE_MEMBERFILEHEADER kMemberFile = m_vecMemberFileHeader[iIndex];

    LONG lDataOffset = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_kMassFileHeader.uiTotalFileNum;

    char* pCompressedData = m_BufferManager2.GetBuffer(kMemberFile.ulCompressSize);
    _lseek(m_fhdl, lDataOffset + kMemberFile.ulOffSet, SEEK_SET);
    _read(m_fhdl, pCompressedData, sizeof(char) * kMemberFile.ulCompressSize);
    uiCompressedSize_ = static_cast<UINT>(kMemberFile.ulCompressSize);
    uiRealSize_ = static_cast<UINT>(kMemberFile.ulRealSize);
    return pCompressedData;
}

void GCMassFile::ReleaseMassFile()
{
    if (m_fhdl != -1)
    {
        _close(m_fhdl);
        m_fhdl = -1;
    }
}

bool GCMassFile::InitMassFileXml(TiXmlDocument* pkXml_, TiXmlElement** ppFileElement_)
{
    const char* szInitDoc = "<?xml version=\"1.0\"  standalone='no' >\n"
        "<FileInfo>"
        "<Version>"
        "</Version>"
        "<File>\n"
        "</File>"
        "</FileInfo>";
    pkXml_->Parse(szInitDoc);
    if (pkXml_->Error())
    {
        printf("Error in %s: %s\n", pkXml_->Value(), pkXml_->ErrorDesc());
        return false;
    }

    TiXmlNode* pNode = pkXml_->FirstChild("FileInfo");
    pNode = pNode->FirstChild("Version");
    TiXmlElement* pElementSet = pNode->ToElement();

    char strVersion[256] = { 0, };
    sprintf(strVersion, "V.%d.%d.", MASSFILE_VERSION / 10, MASSFILE_VERSION % 10);
    TiXmlElement kVerElement("Item");
    kVerElement.SetAttribute("Name", strVersion);
    pElementSet->InsertEndChild(kVerElement);

    pNode = pkXml_->FirstChild("FileInfo");
    pNode = pNode->FirstChild("File");
    *ppFileElement_ = pNode->ToElement();
    return true;
}

BOOL GCMassFile::GetCheckVersion()
{
    int iInfoSize = 0;
    iInfoSize = static_cast<int>(strlen(m_kMassFileHeader.strVersionInfo));
    if (iInfoSize > 0)
    {
        if (m_kMassFileHeader.strVersionInfo[iInfoSize - 2] != '1')
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
        return TRUE;
}

bool GCMassFile::IsLoaded()
{
    if (m_fhdl == -1)
        return false;

    return true;
}


int GCMassFile::ModifyMiniMassFile(GCMassFile* pMassFile, char* strFileName, std::vector<std::string>* pVecFile)
{
    std::vector<std::string> pVecMiniFileList;
    std::string strMFileName;
    for (std::vector<std::string>::iterator it = pVecFile->begin(); it < pVecFile->end(); it++)
    {
        GCMassFile* newMassFile;
        newMassFile = new GCMassFile();
        pVecMiniFileList.clear();
        pVecMiniFileList.push_back(*it);
        strMFileName = strFileName;
        strMFileName = strMFileName.substr(0, strMFileName.find_last_of("\\") + 1);
        strMFileName.append(*it);
        //���� ���ϸ��� Ȯ���ڸ� �ٸ� ���Ϲ����� ���Ͽ�, Ȯ���ڿ��ٰ� �߰��� .mkom�� ���̵��� ����
        //strMFileName=strMFileName.substr(0,strMFileName.find_last_of("."));
        strMFileName.append(FILE_SUFFIX_MKOM);
        GCUTIL_STR::ToLower(strMFileName);
        if (IsCRCFile(*it) == true)
            continue;
        if (newMassFile->ModifyMassFile(pMassFile, (char*)strMFileName.c_str(), &pVecMiniFileList, false) != 0)
        {
            return -1; //������ �ְ� �б������� ��� ������ ó���� ���� ����.
        }
        SAFE_DELETE(newMassFile);

    }
    return 0;
}

bool GCMassFile::IsKomFile(std::string strFileName)
{
    std::string strtolowerStr_KOM, strlowerStr_MKOM;
    strtolowerStr_KOM.resize(strFileName.length());
    std::transform(strFileName.begin(), strFileName.end(), strtolowerStr_KOM.begin(), tolower);
    strlowerStr_MKOM = strtolowerStr_KOM;
    strtolowerStr_KOM.erase(0, strtolowerStr_KOM.length() - 4);
    strlowerStr_MKOM.erase(0, strlowerStr_MKOM.length() - 5);

    //Ȯ���ڰ� �ٸ� ���
    if ((strtolowerStr_KOM.find(FILE_SUFFIX_KOM) != std::string::npos || strlowerStr_MKOM.find(FILE_SUFFIX_MKOM) != std::string::npos || strlowerStr_MKOM.find(FILE_SUFFIX_OGN) != std::string::npos) == false)
    {
        return false;
    }

    return true;
}
