#pragma once
#include "stdafx.h"
#include <dbg/dbg.hpp>
#include <Shlwapi.h>

extern bool CreateDirectoryA( const char* szPath_ );
extern void AppendBackSlash( std::string& str );
extern void AppendSlash( std::string& str );
extern bool DeleteFileAbsolutely( const char* szFileName );
extern bool DeleteDirectoryAbsolutely( const char* strDirectory_ );
extern bool MakeWritableFile( const char* szFileName );
extern void GetCurrentPath( std::string& strPath_ );
extern void* GetLibraryFunction( std::string strFileName_ , std::string strFunc_ , HINSTANCE hInst );
extern int DeleteDirectory(const char* sPath, bool bFlag );
extern int GetFileSize( const char* strFileName_ );
extern bool IsFileExists( std::string filename );

int GetFileSize( const char* strFileName_ )
{
    HANDLE hFile;
    START_LOG( clog, strFileName_ );
    hFile = CreateFileA(strFileName_, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//    START_LOG( cerr, "LastError : " << GetLastError() );

    if( ::GetLastError() == 32 )
    {
        hFile = CreateFileA(strFileName_, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        START_LOG( cerr, "LastError : " << GetLastError() );
    }

    if( hFile == INVALID_HANDLE_VALUE )
    {
        return -1;
    }


    DWORD dwFileSize;
    DWORD dwSizeLow;
    DWORD dwSizeHigh;
    dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
    dwFileSize = dwSizeHigh << 16 | dwSizeLow;
    ::CloseHandle( hFile );

    START_LOG( clog, strFileName_ << " ( Size : " << dwFileSize << " ) " );

    return dwFileSize;
}

void* GetLibraryFunction( std::string strFileName_ , std::string strFunc_ , HINSTANCE hInst )
{
    hInst = NULL;
    void* pFunc = NULL;
    std::string strCurrentPath;
    GetCurrentPath( strCurrentPath );
    strCurrentPath.append( strFileName_ );
    if( IsFileExists( strCurrentPath) == false)
    {
        return NULL;
    }

    hInst = ::LoadLibraryA( strCurrentPath.c_str() );
    DWORD dwErr = ::GetLastError();

    if( hInst == NULL )
    {
        if( (hInst = ::LoadLibraryA( strFileName_.c_str() ) ) == NULL )
        {
            dwErr = ::GetLastError();
            return NULL;
        }
    }
    

    if( (pFunc = GetProcAddress( hInst, strFunc_.c_str() )) == NULL )
    {
        //함수가 없네??? 젝일슨 뭐야??
        pFunc = NULL;
    }    
    
    return pFunc;
}

void GetCurrentPath( std::string& strPath_ )
{
    char strFullPath[MAX_PATH] = {0,};
    char szTempPath[MAX_PATH] = {0,};
    char szDirPath[MAX_PATH] = {0,};
    char szDrive[20] = {0,};

    ::GetModuleFileNameA( NULL, szTempPath, sizeof(szTempPath));
    _splitpath(szTempPath,szDrive,szDirPath,NULL,NULL);
    sprintf( strFullPath, "%s%s", szDrive, szDirPath );

    strPath_ = strFullPath;
}


bool MakeWritableFile( const char* szFileName )
{
    DWORD dwAtt;
    if ( 0xffffffff != ( dwAtt = ::GetFileAttributesA( szFileName ) ) )
    {
        if( ::SetFileAttributesA( szFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/ ) == FALSE)
        {
            START_LOG( cerr, L"File Attribute change Error : " << szFileName );
            return false;
        }
    }
    return true;
}

/// 파일의 지웁니다. (읽기 전용 무시 )
bool DeleteFileAbsolutely( const char* szFileName )
{
    DWORD dwError;

    MakeWritableFile( szFileName );
    if( !::DeleteFileA( szFileName ) )
    {
        dwError = ::GetLastError();
    }

    return true;
}

bool DeleteDirectoryAbsolutely( const char* strDirectory_ )
{
    HANDLE          hFile;
    WIN32_FIND_DATAA	wfd;
    char strOldCurrentDirectory[_MAX_PATH] = {0,};
    char strNowCurrentDirectory[_MAX_PATH] = {0,};

    strcpy(strNowCurrentDirectory ,strDirectory_);
    ::GetCurrentDirectoryA( _MAX_PATH, strOldCurrentDirectory );
	if (::SetCurrentDirectoryA( strDirectory_ )==FALSE) return false;
    hFile = FindFirstFileA( "*.*", &wfd );

    int nTempLen = 0;
    do
    {
        std::string strDeleteFile;
        strDeleteFile = strDirectory_;
        AppendBackSlash( strDeleteFile );
        strDeleteFile.append( wfd.cFileName );
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(wfd.cFileName,".") && strcmp(wfd.cFileName,".."))
			{		

				DeleteDirectoryAbsolutely(strDeleteFile.c_str());
			}
		}
		else
		{
			DeleteFileAbsolutely( strDeleteFile.c_str() ); 
		}
        
    }while( FindNextFileA(hFile, &wfd) != NULL );
    FindClose( hFile );

    ::SetCurrentDirectoryA( strOldCurrentDirectory );

    if( !::RemoveDirectoryA(strNowCurrentDirectory) )
    {
        DWORD err = ::GetLastError();
        if( err != ERROR_DIR_NOT_EMPTY )
        {

        }
        return false;
    }

    return true;
}



/// 문자열 끝에 '\\'가 있는지 확인하여 없으면 추가한다.
/// @param str output parameter
/// @return void
void AppendBackSlash( std::string& str )
{
    if ( '\\' != str[str.size() - 1] )
    {
        str.append( "\\" );
    }
}

/// 문자열 끝에 '/'가 있는지 확인하여 없으면 추가한다.
/// @param str output parameter
/// @return void
void AppendSlash( std::string& str )
{
    if ( '/' != str[str.size() - 1] )
    {
        str.append( "/" );
    }
}


/*------------------------------------------------------------------------*/
/* directory를 삭제한다.
bFlag == true : 하위 디렉토리 및 모든 파일을 삭제 한다.
bFlag == false : 파일이 없을 경우에만 디렉토리를 삭제한다.
ex) cs_file_DirDelete("c:\\aaaa\\aaaa",true);
*/
int DeleteDirectory(const char* sPath, bool bFlag )
{
    TCHAR szFullPathFileName[MAX_PATH];
    TCHAR szFilename[MAX_PATH];

    if (!::RemoveDirectoryA(sPath))
    {
        DWORD err = GetLastError();
        if (err != ERROR_DIR_NOT_EMPTY)
            return false;
    }

    if(!bFlag) return 0;
    // remove sub folders and files.

    WIN32_FIND_DATA FileData = {0};
    BOOL bFinished = false; 
    DWORD dwSize = 0;

    swprintf(szFullPathFileName, TEXT("%s\\*.*"), sPath);
    HANDLE hSearch = FindFirstFile(szFullPathFileName, &FileData); 
    if (hSearch == INVALID_HANDLE_VALUE) 
        return 0;

    while (!bFinished)
    {
        swprintf(szFilename, TEXT("%s\\%s"),sPath,FileData.cFileName);
        if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (StrCmpW(FileData.cFileName,TEXT(".")) && 
                StrCmpW(FileData.cFileName,TEXT("..")))
            {
                DeleteDirectory( (LPCSTR)(LPCTSTR)szFilename, true);
                RemoveDirectoryW(szFilename);
            }
        }
        else
        {
            if (FileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                SetFileAttributes(szFilename, FILE_ATTRIBUTE_NORMAL);

            if (!::DeleteFile(szFilename))
            {
                FindClose(hSearch);
                return false;
            }
        }
        if (!FindNextFile(hSearch, &FileData)) 
        {
            if (GetLastError() == ERROR_NO_MORE_FILES) 
                bFinished = TRUE;
        } 
    }
    FindClose(hSearch);

    // Here the directory is empty.
    ::RemoveDirectoryA(sPath);
    return TRUE;

    return 0;
}

/// Create directory.
/// @param szPath_ 생성할 디렉토리 경로
/// @return 성공하면 true, 실패하면 false
bool CreateDirectoryA( const char* szPath_ )// by ctkim
{
    char* szToken   = NULL;
    char* szToken2  = NULL;
    char szDirName[20][64];

    int len = strlen(szPath_);
    int ct = 0;
    int numDir = -1;
    while( ct < len )
    {
        if( szPath_[ct] == '\\' )
        {
            // 가장 처음에 발견되면 무시
            if( numDir == -1 )
            {
                numDir++;
            }
            else
            {
                memcpy( szDirName[numDir], szPath_, (ct+1) );
                szDirName[numDir][ct+1] = '\0';
                numDir++;
            } // else
        } // if
        ct++;
    } // while

    // Create a new directory.
    ct = 0;
    while( ct < numDir )
    {
        ::CreateDirectoryA( szDirName[ct], NULL );
        ct++;
    } // while
    return true;
}

