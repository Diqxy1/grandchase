/* KGCMassFileManager.h: interface for the KGCMassFileManager class.

by KOG GC TEAM �����
===========================================================================================
GCMassFileManager
===========================================================================================
���� : 
- MassFile�� ����Ͽ�, Kom���� ���ο� �ִ� Ȥ�� Data���� �ȿ� �ִ��� ���ο� �������
�����͸� ������ �� �ֵ�����
- ���� ������ ���� �Ҽ� ��������.


===========================================================================================
��뿹��
===========================================================================================
KGCMassFileManager Manager;
Manager.SetDataDirectory("./Data");
if(!Manager.AddMassFile("./Data/Abta.kom")) return 0;
if(!Manager.AddMassFile("./Data/Texture.kom")) return 0;
if(!Manager.AddMassFile("./Data/Motion.kom")) return 0;

GCMassFile::MASSFILE_MEMBERFILEINFO Info;
for(int i = 0; i < Hits; i++)
{	
if( !Manager.LoadDataFile("ui_game.dds",&Info) )
return 0;

//Info�� ����Ͻÿ�
//Info.pRealData �������� ����...Manager�� ���� ������
}

===========================================================================================
�ӵ� ���� ��� ����Ʈ
===========================================================================================
ȯ��
OS - WindowXP
CPU - P4 3.0GHz
Mem - 512MB
HDD - 7200 RPM(Samsung)

3���� kom���Ͽ��� �˻�, �� 4457���� ������ ���ԵǾ� �־��� (10000Hits) - In Debug Mode


Mon_x07.frm : 206 ms
Abta000.dds : 217 ms
ui_game.dds : 253 ms

Average : 10000Hit Time - 225 ms

============================================================================================
FILE*���� fseek�� �ɸ��� �ð� ����(Debug Mode)
============================================================================================

100000ȸ �ݺ��� 76ms : ���� ���ٰ� ����ɵ�...
*/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include "GCMassFile.h"


class GCMassFileManager  
{
private:
    struct SMassFileIndex		// file�̸����� MassFile�� �����Ϳ� Index�� ������ �ϱ�����
    {
        GCMassFile* pMassFile;
        UINT uiIndex;
    };

public:
    struct SDuplicateInfo {
        std::string strOriginKomFileName;
        std::string strDuplicatedKomFileName;
        std::string strMemberFileName;

        SDuplicateInfo& operator = ( const SDuplicateInfo& arg )
        {
            strOriginKomFileName = arg.strOriginKomFileName;
            strDuplicatedKomFileName = arg.strDuplicatedKomFileName;
            strMemberFileName = arg.strMemberFileName;

            return *this;
        }

        bool operator == ( const SDuplicateInfo& arg )
        {
            return strOriginKomFileName.compare( arg.strOriginKomFileName ) == 0 &&
                strDuplicatedKomFileName.compare( arg.strDuplicatedKomFileName ) == 0 &&
                strMemberFileName.compare( arg.strMemberFileName ) == 0;
        }

        bool operator < ( const SDuplicateInfo& arg )
        {
            return strOriginKomFileName.compare( arg.strOriginKomFileName ) == -1 ||
                strDuplicatedKomFileName.compare( arg.strDuplicatedKomFileName ) == -1 ||
                strMemberFileName.compare( arg.strMemberFileName ) == -1;
        }
    };

public:
    GCMassFileManager();
    virtual ~GCMassFileManager();

    void AddDirectory(const std::string& strDirectory);
    void AddRealFile( const std::string& strFullPath_ );
    void SetDataDirectory(const std::string& strDirectory);
    GCMassFile* AddMassFile(const std::string& strKomFileName);

    void DeleteAllMassFile();                                           // ��� Kom File ��� ����
    GCMassFile::MASSFILE_MEMBERFILEINFO* LoadDataFile( const std::string& strFileName, bool bRealData = true,
        bool bGetFileName = false, char* strMassFileName = NULL);
    std::vector<std::string> FindFile( const std::string& strFileName );


    bool LoadMemoryFile( std::string filename,char ** data, int *length );

    int  GetTotalFileCount();

    void SetRealFileFirst( bool bRealFileFirst_ ) { m_bRealFileFirst = bRealFileFirst_; }
    bool IsIn( const std::string& strFileName_ );

    std::wstring DumpMassFile();		

    std::string GetResFullPath( std::string strFileName );

    bool IsDuplicateExist()   { return !m_vecDuplicateInfo.empty(); }
    std::vector<SDuplicateInfo> GetDuplicatedFileList() { return m_vecDuplicateInfo; }
    //	DWORD GetFileCheckSumFromMassFile( std::string strFileName_ , GCMassFileManager *manager );

private:
    //KOM������ �ƴ� Data�������� ���� ������ �о� ���̴°��..
    bool LoadRealFile(const std::string& strFileName, GCMassFile::MASSFILE_MEMBERFILEINFO *pOut);
    bool LoadRealFile(const std::string& strFileName,char**data,int * length);		
    bool m_bRealFileFirst;

    std::unordered_map< std::string, SMassFileIndex >	m_mapMassFile;
	std::unordered_map< std::string, std::string > m_mapRealFile;
    std::vector< GCMassFile* >				m_vecMassFile;
    std::vector< std::string >				m_vecStrDir;
    std::vector< std::string >				m_vecMassFileName;
    std::vector<SDuplicateInfo>                m_vecDuplicateInfo;
    
    std::string								m_strDataDirectory;

    GCMassFile::MASSFILE_MEMBERFILEINFO		m_kMemberInfo;
    GCBufferManager						    m_kBufferManager;
};

