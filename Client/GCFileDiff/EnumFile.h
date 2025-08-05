#ifndef ENUMFILE_H
#define ENUMFILE_H
#endif

#pragma warning(disable: 4786)

#include <map>
#include <vector>

#include "KAssert.h"
#include "tinyxml.h"
#include "GenerateMKom.h"
#include "LogManager.h"


class KStrLess
{
public:
    bool operator()(const char* pa, const char* pb) const
    {
        return (strcmp(pa, pb) < 0);
    }
};

class KDirInfo
{
public:
    char        m_szName[_MAX_PATH];
};
typedef std::map<const char*, KDirInfo*, KStrLess>  DirMap;


class KFileInfo
{
public:
    char        m_szName[_MAX_PATH];
    DWORD       m_dwSize;
    USHORT      m_usVersion;
    std::string m_strCheckSum;
    //{{ Modified by ct 2003-04-29 오후 04:53:14
    //FILETIME    m_ftLastWriteTime;
    //}} Modified by ct 2003-04-29 오후 04:53:14
};
typedef std::map<const char*, KFileInfo*, KStrLess> FileMap;


class KEnumFile
{
public:
    KEnumFile();
    ~KEnumFile();

    void SetMKomPath(std::string str);

    bool CreateFileList(const char* szCurDir, const char* szFileName);
    bool CreateFileList(const char* szCurDir, TiXmlDocument& xmlDoc);

    // 이 전의 serverfilelist.xml과 최근에 만들어진 파일을 비교 새로운 serverfilelist.xml을 생성
    bool CompareFileVersion(const TiXmlDocument& xmlOldDoc,
        const TiXmlDocument& xmlDoc,
        TiXmlDocument& xmlNewDoc);
    bool CompareFileVersion(const char* szOldPath, const TiXmlDocument& xmlOldDoc,
        const char* szNewPath, const TiXmlDocument& xmlDoc,
        TiXmlDocument& xmlNewDoc);

    // local과 server의 file list 비교 download 받아야 할 file list 생성
    bool CreateUpdateFileList(const char* szLocalFileName,
        const char* szServerFileName,
        TiXmlDocument& xmlUpdateDoc);

    int IsDiffFiles(const TCHAR* szSrcName, 
        const TCHAR* szDstName);

    static bool Signing(TiXmlDocument& xmlDoc);
    static bool Verify(const TiXmlDocument& xmlDoc);

    std::string GetOldMainCRC() const { return m_strOldMainCRC; }
    std::string GetNewMainCRC() const { return m_strNewMainCRC; }

    LogManager logmanager;

    static bool CreateCRCKOM(const char* szCurDir, const char* szFileName, const char* szOutFileName );


private:
    void EnumFile();

private:
    int             m_nDepth;
    char            m_szFileName[_MAX_FNAME]; // 필요없는 정보
    char            m_szCurPath[_MAX_PATH];
    char            m_szCurWorkingDir[_MAX_PATH];
    GenerateMKom    m_KomProc;

    // map
    DirMap*         m_pDirMap;
    FileMap*        m_pFileMap;

    // xml
    TiXmlElement*   m_pxmlSignElement;
    TiXmlElement*   m_pxmlDirElement;
    TiXmlElement*   m_pxmlFileElement;
    TiXmlDocument*  m_pxmlDoc;

    std::string         m_strOldMainCRC;
    std::string         m_strNewMainCRC;
    static TiXmlElement* GetSignElement(const TiXmlDocument& xmlDoc);
    static const std::string GetFileListStr(const TiXmlDocument& xmlDoc);    


};

