#include "stdafx.h"
#include "GCMassFileManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline std::wstring toWideString(const char* pStr, int len = -1)
{
	std::wstring buf;
	if (pStr == NULL)
		return buf;
	if (len != -1 && len <= 0) // florist. 0이어도 통과시키지 말자.
		return buf;

	// figure out how many wide characters we are going to get 
	int nChars = MultiByteToWideChar(CP_ACP, 0, pStr, len, NULL, 0);
	if (len == -1)
		--nChars;
	if (nChars == 0)
		return L"";

	// convert the narrow string to a wide string 
	// nb: slightly naughty to write directly into the string like this
	buf.resize(nChars);
	MultiByteToWideChar(CP_ACP, 0, pStr, len,
		const_cast<wchar_t*>(buf.c_str()), nChars);

	return buf;
}

inline std::wstring toWideString(const std::string& str)
{
	return toWideString(str.c_str());
}

const WCHAR* GCStrCharToWide(const char* strSrc)
{
	static WCHAR strTemp[4096];
	memset(strTemp, 0, sizeof(WCHAR) * 4096);
	std::string strNarrow = strSrc;
	std::wstring strWide = toWideString(strNarrow);
	lstrcpyW(strTemp, strWide.c_str());

	return strTemp;
}

GCMassFileManager::GCMassFileManager()
{
	m_bRealFileFirst = false;		//압축파일 내부에서 우선 로딩하도록 합니다.
}

GCMassFileManager::~GCMassFileManager()
{
	for(int i = 0; i < (int)m_vecMassFile.size(); i++)
	{
		delete m_vecMassFile[i];
	}
	m_vecMassFile.clear();
}

int  GCMassFileManager::GetTotalFileCount()
{
	int iTotal = 0;
	for(int i = 0 ; i < (int)m_vecMassFile.size(); i++)
	{
		iTotal += m_vecMassFile[i]->GetFileCount();
	}
	return iTotal;
}

void GCMassFileManager::AddRealFile( const std::string& strFullPath_ )
{
	std::string strFileName = strFullPath_;
    std::transform( strFileName.begin(), strFileName.end(), strFileName.begin(), tolower );

	strFileName = strFileName.substr( strFileName.find_last_of( '\\' ) + 1 );

	m_mapRealFile.insert( std::unordered_map< std::string, std::string >::value_type( std::string(strFileName), strFullPath_ ) );
}

void GCMassFileManager::AddDirectory(const std::string& strDirectory)
{
#if defined( INCLUDE_KOM_FOLDER )
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

		std::string strPath = finder.GetFilePath().GetBuffer();
		AddRealFile( strPath );
	}

	finder.Close();
#else
	m_vecStrDir.push_back( strDirectory );
#endif

}

void GCMassFileManager::SetDataDirectory(const std::string& strDirectory)
{
	m_strDataDirectory = strDirectory;
}

GCMassFile* GCMassFileManager::AddMassFile(const std::string& strKomFileName)
{
#ifdef INCLUDE_KOM_FOLDER
	//적어도 5글자는 되어야 a.kom 이라는 형태가 나오니까...
	std::string strFileName = strKomFileName;
	boost::algorithm::trim( strFileName );
	if( strFileName.size() > 5 )
	{
		AddDirectory( strFileName.append("/") );
	}
#endif

    std::vector<GCMassFile*>::iterator vit;
    for(vit =	m_vecMassFile.begin();vit!= m_vecMassFile.end();vit++)
    {
        if((*vit)->GetMassFileName() == strKomFileName)
        {
#ifdef _DEBUG
            std::string strDuplicatedKOMFile = strKomFileName;
            strDuplicatedKOMFile += "파일의 중복호출이 감지되었습니다.";
            MessageBoxA( NULL, strDuplicatedKOMFile.c_str(), NULL, MB_OK );            
            //printf("중복파일 : %s(%s)\n",kMemberInfo.strFileName, ret.first->second.pMassFile->GetMassFileName().c_str() );
#endif
            //return (*vit);
        }
    }

    GCMassFile* pMassFile = new GCMassFile();
	if( pMassFile->LoadMassFile( strKomFileName.c_str() ) == false )
	{
		SAFE_DELETE( pMassFile );
		return NULL;
	}
	m_vecMassFile.push_back( pMassFile );

	GCMassFile::MASSFILE_MEMBERFILEINFO kMemberInfo;
	SMassFileIndex kFileIndex;
	ZeroMemory( &kMemberInfo,sizeof( GCMassFile::MASSFILE_MEMBERFILEINFO ) );

	for(int i = 0; i < (int)pMassFile->GetFileCount(); i++)
	{
		pMassFile->GetMemberFile(i,&kMemberInfo);
		kFileIndex.uiIndex = i;
		kFileIndex.pMassFile = pMassFile;
        //====================================================================================
        // 2006.12.28 : Asirion 이미 맵에 들어있을 경우를 처리해 줘야 한다.
		std::pair<std::unordered_map< std::string, SMassFileIndex >::iterator,bool> ret;
		ret = m_mapMassFile.insert(std::unordered_map< std::string, SMassFileIndex >::value_type( kMemberInfo.strFileName, kFileIndex ) );
		if(ret.second == false )
		{
            SDuplicateInfo sDuplicateInfo;
            sDuplicateInfo.strOriginKomFileName = ret.first->second.pMassFile->GetMassFileName();;
            sDuplicateInfo.strDuplicatedKomFileName = strKomFileName;
            sDuplicateInfo.strMemberFileName = kMemberInfo.strFileName;

            m_vecDuplicateInfo.push_back( sDuplicateInfo );
			
#ifdef _DEBUG
			printf("중복파일 : %s(%s)\n",kMemberInfo.strFileName, ret.first->second.pMassFile->GetMassFileName().c_str() );
#endif
		}	

	}

	return pMassFile;
}


std::vector<std::string> GCMassFileManager::FindFile( const std::string& strFileName )
{
    std::vector<std::string> vecFileNames;
    vecFileNames.clear();
    _strlwr_s( (char*)strFileName.c_str(), strFileName.size() );
	std::unordered_map< std::string, SMassFileIndex >::iterator itor = m_mapMassFile.begin();
    while( itor != m_mapMassFile.end() )
    {
        if(itor->first.find(strFileName)!=std::string::npos)
        {
            vecFileNames.push_back(itor->first);
        }
        ++itor;
    }
    return vecFileNames;
}

GCMassFile::MASSFILE_MEMBERFILEINFO* GCMassFileManager::LoadDataFile(const std::string& strFileName, bool bRealData, bool bGetFileName, char* strMassFileName)
{
	m_kMemberInfo.dwFlag = bRealData;
	_strlwr( (char*)strFileName.c_str() );

	if( !m_bRealFileFirst )
	{	
		if( m_mapMassFile.find( strFileName ) == m_mapMassFile.end() )
		{	
#if defined(_DEBUG) || defined( MAPTOOL )
			if( LoadRealFile( strFileName, &m_kMemberInfo ) == true )
				return &m_kMemberInfo;
			else
#endif            
				return NULL;
		}
		else
		{	
			SMassFileIndex *kMassFileIndex = &m_mapMassFile[strFileName];
			kMassFileIndex->pMassFile->GetMemberFile( kMassFileIndex->uiIndex, &m_kMemberInfo );
			if (bGetFileName==true)
			{
				if (strMassFileName !=NULL)
					sprintf(strMassFileName,"%s",kMassFileIndex->pMassFile->GetMassFileName().c_str());
			}
		}
	}
	else
	{	
		if( LoadRealFile( strFileName, &m_kMemberInfo ) == true )
			return &m_kMemberInfo;
		else if( m_mapMassFile.find( strFileName ) != m_mapMassFile.end() )
		{
			SMassFileIndex *kMassFileIndex = &m_mapMassFile[strFileName];
			kMassFileIndex->pMassFile->GetMemberFile( kMassFileIndex->uiIndex, &m_kMemberInfo );
			if (bGetFileName==true)
			{
				if (strMassFileName !=NULL)
					sprintf(strMassFileName,"%s",kMassFileIndex->pMassFile->GetMassFileName().c_str());
			}
		}
		else
			return NULL;
	}
	return &m_kMemberInfo;
}


bool GCMassFileManager::LoadMemoryFile( std::string filename,char ** data, int *length )
{	
	_strlwr( (char*)filename.c_str() );
	std::unordered_map<std::string,SMassFileIndex >::iterator iter = m_mapMassFile.find( filename );

	if( !m_bRealFileFirst )
	{	
		if( iter == m_mapMassFile.end() )
		{	
#if defined(_DEBUG) || defined( MAPTOOL )
			return LoadRealFile( filename, data,length );
#endif            
			return false;
		}
		else
		{	
			SMassFileIndex *kMassFileIndex = &iter->second;
			return kMassFileIndex->pMassFile->GetMemberFile(kMassFileIndex->uiIndex,data,length);			
		}
	}
	else
	{	
		if( LoadRealFile( filename, data,length ) == true )
			return true;
		else if( iter != m_mapMassFile.end() )
		{
			SMassFileIndex *kMassFileIndex = &iter->second;
			return kMassFileIndex->pMassFile->GetMemberFile(kMassFileIndex->uiIndex,data,length);			
		}
		else
			return false;
	}	
	return false;
}


bool GCMassFileManager::IsIn( const std::string& strFileName_ )
{
    _strlwr( (char*)strFileName_.c_str() );
    if( m_mapMassFile.find(strFileName_) == m_mapMassFile.end() )
    {
		int fhdl;
		errno_t erno;
        std::string strName;

		if( m_mapRealFile.find( strFileName_ ) == m_mapRealFile.end() )
			return false;

		erno = _sopen_s( &fhdl, m_mapRealFile[strFileName_].c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD );

        if ( fhdl != -1 )
        {
			_close( fhdl );
			return true;
        }
        else
		{
			return false;
        }
    }
    else
    {	
        return true;
    }
}

bool GCMassFileManager::LoadRealFile(const std::string& strFileName, GCMassFile::MASSFILE_MEMBERFILEINFO *pOut)
{
	int fhdl;
	errno_t erno;
	std::string strFullPath = "";

#if defined(INCLUDE_KOM_FOLDER)
	if( m_mapRealFile.find( strFileName ) == m_mapRealFile.end() )
		return false;

	strFullPath = m_mapRealFile[strFileName];
#else
	for( int i = 0; i < (int)m_vecStrDir.size(); i++ )
	{
		strFullPath = m_vecStrDir[i];
		strFullPath.append(strFileName);

		erno = _sopen_s( &fhdl, strFullPath.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD );
		if(fhdl == -1)
			break;
	}
	if( strFullPath == "" )
	{
		return false;
	}
#endif

	erno = _sopen_s( &fhdl, strFullPath.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD );

	if( fhdl == -1 )
	{
		return false;
	} 
	_lseek( fhdl, 0, SEEK_END );
	pOut->ulSize = _tell( fhdl );
	_lseek( fhdl, 0, SEEK_SET );

	if(pOut->dwFlag & MFI_REALDATA)
	{
		pOut->pRealData = (char*)m_kBufferManager.GetBuffer( pOut->ulSize );
		_read( fhdl, pOut->pRealData, pOut->ulSize );
	}

	strcpy( pOut->strFileName, strFileName.c_str() );

	_close( fhdl );
	return true;
}

bool GCMassFileManager::LoadRealFile(const std::string& strFileName,char**data,int * length)
{
	int fhdl;
	errno_t erno;
    std::string strFullPath = "";

#if defined(INCLUDE_KOM_FOLDER)
	if( m_mapRealFile.find( strFileName ) == m_mapRealFile.end() )
		return false;

	strFullPath = m_mapRealFile[strFileName];
#else
    for( int i = 0; i < (int)m_vecStrDir.size(); i++ )
    {
        strFullPath = m_vecStrDir[i];
        strFullPath.append(strFileName);

		erno = _sopen_s( &fhdl, strFullPath.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD );
        if(fhdl == -1)
            break;
    }
    if( strFullPath == "" )
    {
        return false;
    }
#endif

	erno = _sopen_s( &fhdl, strFullPath.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD );

    if( fhdl == -1 )
    {
        return false;
    } 
	_lseek( fhdl, 0, SEEK_END );
	(*length)= _tell( fhdl );
	_lseek( fhdl, 0, SEEK_SET );

	(*data) = new char[(*length)+1];	
	_read( fhdl,(*data), (*length));	
	_close( fhdl );
	(*data)[*length]=0;
	return true;
}

std::wstring GCMassFileManager::DumpMassFile()
{
    std::wstringstream stm;

	std::unordered_map< std::string, SMassFileIndex >::iterator mit;
    for( mit = m_mapMassFile.begin(); mit != m_mapMassFile.end(); ++mit )
    {
        std::wstring str( GCStrCharToWide( mit->first.c_str() ) );
        stm << L"FileName : " << str << L" Index : " << mit->second.uiIndex << std::endl;
    }

    return stm.str();
}

std::string GCMassFileManager::GetResFullPath( std::string strFileName )
{
    _strlwr( (char*)strFileName.c_str() );
    return m_mapRealFile[strFileName];
}
