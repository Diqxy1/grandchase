#pragma once
#include "kom.h"
#include <unordered_map>

#ifndef FILESEEK_DEFINED
#define FILESEEK_DEFINED
enum
{
	FILESEEK_CUR = 0,
	FILESEEK_SET,
	FILESEEK_END,
};
#endif

class KomfileManager:protected Komfile
{
public:
	KomfileManager()
		: m_bRealFileFirst(false)
		{}
	virtual ~KomfileManager();

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

	Komfile*  AddMassFile(const std::string &strKomFileName);
    bool	   HaveMassFile( const std::string &strKomFileName );
	void AddDirectory(const std::string strDirectory);
	void AddRealFile( const std::string strFullPath_ );
	void AddKeepFile( std::string strKeepFile );
	void SetDataDirectory(const std::string strDirectory){m_strDataDirectory = strDirectory;}
    void SetRealFileFirst( bool bRealFileFirst_ ) { m_bRealFileFirst = bRealFileFirst_; }

	bool IsIn(const std::string &filename);
	std::string GetResFullPath( std::string strFileName );

	bool IsDuplicateExist()   { return !m_vecDuplicateInfo.empty(); }
	std::vector<SDuplicateInfo> GetDuplicatedFileList() { return m_vecDuplicateInfo; }	

	bool LoadMemoryFile(const std::string& filename,boost::shared_array<char>& data,int &length);
    bool LoadGetFileData(std::string& filename, unsigned int& offset, unsigned int& compressedsize, unsigned int& size, bool& bRealFile );

    std::vector<HANDLE> GetfpStreamList()   { return m_vecfpStreamList; }
    std::vector<std::string> GetKeepFileList()  { return m_vecKeepFileList; }
	std::map<std::string, KSubfile> GetSubFiles() { return GetSubfiles(); }

protected:

    bool GetRealFileData( std::string& filename );
	bool LoadRealFile(const std::string& strFileName,boost::shared_array<char>& data ,int &length);
	bool m_bRealFileFirst;
	std::string								m_strDataDirectory;
	std::vector< std::string >				m_vecStrDir;
	std::vector<SDuplicateInfo>                m_vecDuplicateInfo;
	std::unordered_map< std::string, std::string > m_mapRealFile;

	std::vector<Komfile*> m_vecMassFile;
	std::vector<std::string> m_vecKeepFileList;
	std::vector<HANDLE> m_vecfpStreamList;
};

class GCMemoryFile
{
public:
	GCMemoryFile(boost::shared_ptr<KomfileManager> manager,std::string filename);
	virtual ~GCMemoryFile();

	int		ReadFile(void* pDest, int ReadBytes);
	char*	GetDataPointer() { return m_pCursor; }
	int		FileSeek(int iPos, int Pivot);
	int		GetFileSize() { return m_iSize; }
	bool	IsLoaded(){return m_bLoaded;}
	
private:
	int   m_iSize;
	int   m_iCursorPos;	
	char* m_pCursor;	
	boost::shared_ptr<KomfileManager> m_pMFManager;
	boost::shared_array<char>		m_pData;
	bool  m_bLoaded;
};


class GCThreadMemoryFile
{
public:
    struct SFileLoadData
    {
        std::string strFileName;
        unsigned int offset;
        unsigned int compressedsize;
        unsigned int size;
        
        SFileLoadData()
            : offset( 0 )
            , compressedsize( 0 )
            , size( 0 )
        {

        }
    };

public:
    GCThreadMemoryFile(boost::shared_ptr<KomfileManager> manager,std::string filename);
    virtual ~GCThreadMemoryFile();

	void	LoadMemoryFile(void);
    int		ReadFile(void* pDest, int ReadBytes);
    char*	GetDataPointer() { return m_pCursor; }
    int		FileSeek(int iPos, int Pivot);
    int		GetFileSize() { return m_iSize; }
    bool	IsLoaded(){ return m_bLoaded; }
    bool    IsFileValid(){ return m_bFileValid; }

private:
    int   m_iSize;
    int   m_iCursorPos;	
    char* m_pCursor;	
    bool  m_bLoaded;
    
    bool  m_bRealFile;
    bool  m_bFileValid;

    SFileLoadData    m_sFileData;
    
    boost::shared_array<char>		m_pData;
};