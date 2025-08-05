// MassFile.h: interface for the GCMassFile class.
// by �����
//////////////////////////////////////////////////////////////////////

#pragma once
//#include <Windows.h>
#include "GCBufferManager.h"
#include "GCMemoryFile.h"
//#include <vector>
//#include <string>
#include <map>

#define NEW_OGN

#define MASSFILE_VERSION		(4)
#define MASSFILE_NAMESIZE		(60)

#define FILE_SUFFIX_KOM                    ".kom"
#define FILE_SUFFIX_MKOM                   ".mkom"
#define FILE_SUFFIX_OGN					   ".first"

#define MFI_REALDATA			(1)
#define MFI_NODECOMPRESS		(2)
namespace MASSFILE_ERROR
{
    enum
    {
        READ_ONLY = 0,
        FAIL_MAKE_TEMPFILE,
        NUM,
    };
};
class TiXmlDocument;
class TiXmlElement;

class GCMassFile
{
private:
    struct MASSFILE_HEADER
    {
        char strVersionInfo[52];
        UINT uiTotalFileNum;
        bool bCompressed;
    };
    struct MASSFILE_MEMBERFILEHEADER
    {
        char strFileName[MASSFILE_NAMESIZE];
        ULONG   ulRealSize;
        ULONG   ulCompressSize;
        ULONG   ulOffSet;		//RealData�κп����� OffSet�Դϴ�
    };

#pragma pack(push, 1)
    struct MASSFILE_NEWHEADER
    {
        ULONG	ulFileTime;
        unsigned int nFiles;
        WORD	wVersion;
        UINT	bCompressed;
        UINT	uAdler32;
        UINT	nHeaderXML;
    };
#pragma pack(pop)
    void DecryptBufferPlus(int key, char* data, int length);
    void EncryptBufferPlus(int key, char* data, int length);
public:
    struct MASSFILE_MEMBERFILEINFO
    {
        char    strFileName[MASSFILE_NAMESIZE];
        ULONG   ulSize;
        ULONG   ulCompressedSize;
        char* pRealData;
        DWORD   dwFlag;
        MASSFILE_MEMBERFILEINFO()
        {
            ZeroMemory(strFileName, sizeof(char) * MASSFILE_NAMESIZE);
            ulSize = 0;
            pRealData = NULL;
            dwFlag = MFI_REALDATA;
        }
    };
public:
    GCMassFile();
    virtual ~GCMassFile();
    void ClearMassFile();
    static GCBufferManager m_BufferManager1;
    static GCBufferManager m_BufferManager2;
    UINT GetFileCount();		//MassFile�� ��� File�� ���ԵǾ��ִ��� �˾ƺ��ϴ�.

    bool LoadMassFile(const char* strMassFile, std::string* strErrorLog = NULL);
    int UpgradeMassFile(const char* strMassFile);

    //====================================================================================
    // 2006.12.28 : Asirion
    // MemberFileHeader�� ������ MemberFileInfo�κ��� ä���.
    bool GetMemberFile(IN MASSFILE_MEMBERFILEHEADER& kMemberFileHeader_, OUT MASSFILE_MEMBERFILEINFO* pOut_, bool bDecompress_);

    //====================================================================================
    // 2006.12.28 : Asirion
    // MemberFileHeader�� ã�� GetMemberFile( IN MASSFILE_MEMBERFILEHEADER& kMemberFileHeader_, OUT MASSFILE_MEMBERFILEINFO* pOut_ )�� ��
    bool GetMemberFile(IN const char* strRealFile, OUT MASSFILE_MEMBERFILEINFO* pOut, IN bool bDecompress_ = true);
    bool GetMemberFile(IN int iFileIndex, OUT MASSFILE_MEMBERFILEINFO* pOut, IN bool bDecompress_ = true);

    //�Ʒ� �� �Լ��� MassFile�� Load���ѵ��� �ʰ� �ϳ��� �о� ���̰��� �Ҷ�..
    //������ ����ҽ� �ӵ� ������ ������ ��
    bool GetMemberFile(IN const char* strMassFile, IN const char* strRealFile, OUT MASSFILE_MEMBERFILEINFO* pOut, IN bool bDecompress_ = true);
    bool GetMemberFile(IN const char* strMassFile, IN int iFileIndex, OUT MASSFILE_MEMBERFILEINFO* pOut, IN bool bDecompress_ = true);

    bool SaveMassFile(char* strFileName, std::vector<char*>* pVecFile, bool bZip = false);
    bool SaveMassFile();
    int  ModifyMassFile(GCMassFile* pMassFile, char* strFileName, std::vector<std::string>* pVecFile, bool bMakeCRC);
    int  ModifyMiniMassFile(GCMassFile* pMassFile, char* strFileName, std::vector<std::string>* pVecFile);
    bool WriteKomFileCheckSum(int iTotalFileCount_, FILE* pFile_);
    std::string GetMassFileName() const
    {
        return m_strMassFileName;
    }
    //FILE����...Help
    long GetFileSize(const char* strFileName);
    void GetFileName(char* Buffer, size_t size, char* strFilePath);
    BOOL GetCheckVersion();

    bool IsCompressed();

    //KOM�������� �����ϴ� �κ�.
    static bool IsKomFile(std::string strFileName);
    static bool IsCRCFile(std::string strFileName);
    static std::string ReplaceDirectoryMark(std::string strTemp);

    //====================================================================================
    // 2006.09.21 : Asirion
    // Mass File Xml�� �ʱ�ȭ �ϰ�, File�� ���� XmlElement�� return �Ѵ�.
    bool InitMassFileXml(TiXmlDocument* kXml_, TiXmlElement** ppFileElement_);

    //====================================================================================
    // 2006.09.21 : Asirion
    // CRC������ �����Ѵ�.
    bool GenerateMassFileXml(std::string strMassFileName_, std::string strFilePath_);

    //====================================================================================
    // 2006.09.21 : Asirion
    // ���� ������ üũ���� ����.
    // �ݵ�� LoadMassFile���Ŀ� ��� �ؾ� �Ѵ�.
    std::string GetMemberFileCheckSum(std::string& strMemberFileName_, UINT& uiRealSize_);

    //====================================================================================
    // 2006.09.21 : Asirion
    // ��� ���ϵ� �̸��� ���õ� ������ vector�� ����.
    // �ݵ�� LoadMassFile���Ŀ� ��� �ؾ� �Ѵ�.
    std::vector< std::string > GetSortedFileNames();

    //====================================================================================
    // 2006.09.21 : Asirion
    // Member������ ���� ����� �����Ϳ�, Size�� ����.
    char* GetCompressedFile(std::string& strMemberFileName_, UINT& uiCompressedSize_, UINT& uiRealSize_);

    //====================================================================================
    // 2007.06.29 : milennium9
    // �ٸ� Kom ���Ͽ� ����ִ� �����͸� �����ͼ� �߰��մϴ�.
    int ModifyMassFile(GCMassFile* kOriginal_, GCMassFile* kSource_, char* strFileName, std::vector<std::string>* pVecFile, bool bMakeCRC = false);
    int ModifyMassFile(GCMassFile* pMassFile, char* strFileName, std::vector<std::string>* pVecFile);
    int ModifyMassFile(GCMassFile* kOriginal_, std::vector< GCMassFile* >& kSource_, char* strFileName, std::vector<std::string>* pVecFile, bool bMakeCRC = false, bool bOriginalFirst_ = false);
    //====================================================================================
    bool IsLoaded();
    void ReleaseMassFile();        // ���� ������ ���� ���ؼ� LoadMassFile ���� ���� �ִ� ������ ���������� fclose���ִ� �Լ�

    bool GetMemberFile(int i, char** data, int* length);

private:

    std::vector< MASSFILE_MEMBERFILEHEADER > m_vecMemberFileHeader;
    MASSFILE_HEADER						   m_kMassFileHeader = { 0 };
    MASSFILE_NEWHEADER					   m_kMassFileNewHeader = { 0 };
    bool								   m_bIsNewVersion = false;
    int									   m_iBuffersize = -1;
    unsigned char* m_pXMLHeader = nullptr;
    char								   m_strMassFileName[MAX_PATH] = { 0 };
    int									   m_fhdl = -1;
    //FILE*								   m_pFile;
    int									   m_iFileVersion = -1;
};
