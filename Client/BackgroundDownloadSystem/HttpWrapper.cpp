#include "stdafx.h"
#include "HttpWrapper.h"
#include <Thread/Locker.h>
#include <WinSock2.h>

bool MakeWritableFile( const WCHAR* szFileName )
{
	DWORD dwAtt;
	if ( 0xffffffff != ( dwAtt = ::GetFileAttributes( szFileName ) ) )
	{
		if( ::SetFileAttributes( szFileName, FILE_ATTRIBUTE_NORMAL/*dwAtt & (~FILE_ATTRIBUTE_NORMAL)*/ ) == FALSE)
		{
			return false;
		}
	}
	return true;
}

/// Default constructor.
KHttpWrapper::KHttpWrapper( void )
{
	m_hInternet = NULL;
}

/// Default destructor.
KHttpWrapper::~KHttpWrapper( void )
{
	Close();
}

/// HTTP 연결을 연다.
/// @param void
/// @return 성공하면 true, 실패하면 false
bool KHttpWrapper::Open( void )
{
	// HTTP Open
	KLocker lock(m_csLock);
	if ( NULL == ( m_hInternet = ::InternetOpen( L"HTTP_WRAPPER_KOG",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0 ) ) )
	{
		return false;
	}
	return true;
}

/// HTTP 연결을 닫는다.
/// @param void
/// @return void
void KHttpWrapper::Close( void )
{
	// HTTP Close
	KLocker lock(m_csLock);
	if ( NULL != m_hInternet )
	{
		::InternetCloseHandle( m_hInternet );
		m_hInternet = NULL;
	}
}

/// HTTP 연결을 리셋한다.
/// @param void
/// @return 성공하면 true, 실패하면 false
bool KHttpWrapper::Reset( void )
{
	Close();
	return Open();
}


#define STR_FILETEMP	L"_Temp"
#define STATUSCODE_SIZE  32

/// 주어진 파일을 다운로드한다.
/// @param szSrcPath_ 다운로드할 파일의 HTTP 경로
/// @param szFileName_ 파일 이름
/// @param szDestPath_ 다운로드받을 위치의 경로
/// @return 성공하면 0 , 404페이지 -1, 실패 -2
int KHttpWrapper::Get( const WCHAR* szSrcPath_,
					   const WCHAR* szFileName_,
					   const WCHAR* szDestPath_ ) const
{
	WCHAR szSrcPath[MAX_PATH]      = { 0, };
	WCHAR szSrcPathTemp[MAX_PATH]  = { 0, };
	WCHAR szDestPath[MAX_PATH]     = { 0, };
	WCHAR szDestPathTemp[MAX_PATH] = { 0, };

	if ( NULL == m_hInternet || NULL == szSrcPath_ ||
		NULL == szFileName_ || NULL == szDestPath_ )
	{
		return -2;
	}


	::wcscpy( szSrcPathTemp, szSrcPath_ );
	::wcscat( szSrcPathTemp, szFileName_ );

	::wcscpy( szDestPathTemp, szDestPath_ );
	::wcscat( szDestPathTemp, szFileName_ );

	
	BackSlash2Slash( szSrcPathTemp, szSrcPath );
	Slash2BackSlash( szDestPathTemp, szDestPath );


	// 파일이 있는 URL을 열어놓는다.
	HINTERNET hURLFile;
	if ( NULL == ( hURLFile = ::InternetOpenUrl( m_hInternet,
		szSrcPath,
		NULL,
		0,
		INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_NO_CACHE_WRITE,
		0 ) ) )
	{
		return -2;
	}

	// 기존 파일을 삭제한다.
	MakeWritableFile( szDestPath );
	::DeleteFile( szDestPath );
		

	// 연결정보 확인
	WCHAR szFileSize[12];
	unsigned int dwInfoFileSize;
	dwInfoFileSize = sizeof(szFileSize);
	BOOL bSizeIsKnown = HttpQueryInfo(hURLFile, HTTP_QUERY_CONTENT_LENGTH, szFileSize, (LPDWORD)&dwInfoFileSize, NULL);

	/*if(bSizeIsKnown) 
	{ 
		g_pPatcher->SetNewFileByte(atol(szFileSize)); 
	} */

	// 새 파일을 만든다.
	HANDLE hLocalFile;
	if ( INVALID_HANDLE_VALUE == ( hLocalFile = ::CreateFile( ((std::wstring)szDestPath+STR_FILETEMP).c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL ) ) )
	{
		if( GetLastError() == 3 )
			return true;
		return -2;
	}

	MakeWritableFile( szDestPath );

	BYTE  buf[100 * 1024] = { 0, };
	DWORD dwcbAvail    = 0;
	DWORD dwcbRead     = 0;
	DWORD dwcbWrite    = 0;

	// 다운로드받는다.
	do
	{
		// 전송이 준비된 데이터량을 확인한다.
		if ( FALSE == ::InternetQueryDataAvailable( hURLFile,
			&dwcbAvail,
			0, 0 ) )
		{
			::FlushFileBuffers( hLocalFile );
			::CloseHandle( hLocalFile );
			::InternetCloseHandle( hURLFile );
			return -2;
		}

		// 연결정보 확인
		DWORD dwStatusCode;
		unsigned int dwInfoSize;

		dwInfoSize = 4;

		HttpQueryInfo(hURLFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, (LPDWORD)&dwInfoSize, NULL);

		WCHAR szHostInfo[1024];
		DWORD size;
		size = sizeof(szHostInfo);
		BOOL ret = HttpQueryInfo(hURLFile, HTTP_QUERY_CONTENT_BASE | HTTP_QUERY_FLAG_REQUEST_HEADERS, szHostInfo, &size, NULL);

		if (dwStatusCode == HTTP_STATUS_OK)
		{ // 정상

		}
		else 
		{
			if (dwStatusCode == HTTP_STATUS_NOT_FOUND)
			{
			}
			else
			{
			}
			::FlushFileBuffers( hLocalFile );
			::CloseHandle( hLocalFile );
			::InternetCloseHandle( hURLFile );
			return -1;
		}


		// 파일을 읽어들인다.
		if ( FALSE == ::InternetReadFile( hURLFile,
			buf,
			( ( dwcbAvail > sizeof( buf ) ) ?
			sizeof( buf ) :
		dwcbAvail ),
			&dwcbRead ) )
		{
			::FlushFileBuffers( hLocalFile );
			::CloseHandle( hLocalFile );
			::InternetCloseHandle( hURLFile );
			return -2;
		}



		// 다운로드 받은 데이터를 파일에 기록한다.
		if ( FALSE == ::WriteFile( hLocalFile, &buf, dwcbRead, &dwcbWrite, NULL ) )
		{
			::FlushFileBuffers( hLocalFile );
			::CloseHandle( hLocalFile );
			::InternetCloseHandle( hURLFile );
			return -2;
		}
	}
	while ( 0 != dwcbRead );

	::FlushFileBuffers( hLocalFile );
	::CloseHandle( hLocalFile );
	::InternetCloseHandle( hURLFile );

	//원래 이름으로 파일이름을 바꾼다.
	if (MoveFileW(((std::wstring)szDestPath+STR_FILETEMP).c_str(),szDestPath)==FALSE)
		return -2;

	return 0;
}

/// 주어진 문자열에서 '\'를 '/'로 바꾼다.
/// @param szSrc original string
/// @param szDst output parameter
/// @return void
void KHttpWrapper::BackSlash2Slash( const WCHAR* szSrc, WCHAR* szDst ) const // by ctkim
{
	if ( NULL == szSrc ) return;

	::wcscpy( szDst, szSrc );
	WCHAR* pch = szDst;
	while ( NULL != ( pch = ::wcschr( pch, '\\' ) ) )
	{
		*pch = '/';
	}
}

/// 주어진 문자열에서 '/'를 '\'로 바꾼다.
/// @param szSrc original string
/// @param szDst output parameter
/// @return void
void KHttpWrapper::Slash2BackSlash( const WCHAR* szSrc, WCHAR* szDst ) const // by ctkim
{
	if ( NULL == szSrc ) return;

	::wcscpy( szDst, szSrc );
	WCHAR* pch = szDst;
	while ( NULL != ( pch = ::wcschr( pch, '/' ) ) )
	{
		*pch = '\\';
	}
}
