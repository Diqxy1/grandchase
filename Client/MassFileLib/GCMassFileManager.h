/* KGCMassFileManager.h: interface for the KGCMassFileManager class.

by KOG GC TEAM 류상욱
===========================================================================================
GCMassFileManager
===========================================================================================
목적 : 
- MassFile을 등록하여, Kom파일 내부에 있던 혹은 Data폴더 안에 있는지 여부에 관계없이
데이터를 관리할 수 있도록함
- 차후 압축을 관리 할수 있을것임.


===========================================================================================
사용예제
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

//Info를 사용하시오
//Info.pRealData 삭제하지 말것...Manager가 직접 관리함
}

===========================================================================================
속도 측정 결과 리포트
===========================================================================================
환경
OS - WindowXP
CPU - P4 3.0GHz
Mem - 512MB
HDD - 7200 RPM(Samsung)

3개의 kom파일에서 검색, 총 4457개의 파일이 포함되어 있었음 (10000Hits) - In Debug Mode


Mon_x07.frm : 206 ms
Abta000.dds : 217 ms
ui_game.dds : 253 ms

Average : 10000Hit Time - 225 ms

============================================================================================
FILE*에서 fseek에 걸리는 시간 측정(Debug Mode)
============================================================================================

100000회 반복시 76ms : 거의 없다고 보면될듯...
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
    struct SMassFileIndex		// file이름으로 MassFile의 포인터와 Index를 역참조 하기위함
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

    void DeleteAllMassFile();                                           // 모든 Kom File 등록 해제
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
    //KOM파일이 아닌 Data폴더에서 실제 파일을 읽어 들이는경우..
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

