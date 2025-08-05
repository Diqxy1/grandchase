#pragma   warning( disable :4146 )
//{{ florist
#ifdef _STDAFX
#include "stdafx.h"
#endif
//}} florist

#include "ODBC.h"
#include "dbg/dbg.hpp"
#include <Thread/Locker.h>
#include <KncUtil.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

KODBC::KMaxProcessingInfo KODBC::ms_kMaxProcessingInfo = {
	KncCriticalSection(), L"초기상태", 0, CTime::GetCurrentTime()
};

KODBC::KODBC()
{
	m_hEnv = SQL_NULL_HENV;
	m_hDbc = SQL_NULL_HDBC;
	m_hStmt = SQL_NULL_HSTMT;

	m_usColumNum = 0;
	::memset(m_sqlState, 0, sizeof(m_sqlState));
	::memset(m_szDiagMsg, 0, SQL_MAX_MESSAGE_LENGTH);
	m_strLastQuery.clear();
	m_strBeforeLastQuery.clear();
	InitializeCriticalSection(&m_cs);

	m_sqlMsgLen = 0;
	m_sqlNativeErr = 0;
} // KODBC()

KODBC::~KODBC()
{
	Close();
	DeleteCriticalSection(&m_cs);
} // ~KODBC()

bool KODBC::DriverConnect(const wchar_t* szDsn_)
{
	std::wstring strDsn_ = szDsn_;

	wchar_t buff[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, buff);

	if (m_strDSN.size() <= 0)
	{
		//         m_strDSN = L"FILEDSN=";
		//         m_strDSN += buff;
		//         m_strDSN += L"\\";
		//         m_strDSN += strDsn_;

		m_strAbsolutePathDSN = L"";
		m_strAbsolutePathDSN += buff;
		m_strAbsolutePathDSN += L"\\";
		m_strAbsolutePathDSN += strDsn_;
	}

	SQLWCHAR InCon[255];
	SQLWCHAR OutCon[255];
	SQLSMALLINT cbOutCon;
	SQLRETURN Ret;

	// DSN 복호화 작업 추가
	FILE* pFile = NULL;

	pFile = fopen(KncUtil::toNarrowString(m_strAbsolutePathDSN).c_str(), "r");

	if (NULL == pFile)
	{
		START_LOG(cwarn, L"파일이 존재하지 않음")
			<< BUILD_LOG(m_strAbsolutePathDSN) << END_LOG;
		return false;
	}

	char bufDSNFileData[1024];
	char bufHexPWD[256];
	char *strTeaKey = "6664524F6B663350655153615C6A313A";
	char strHexToChar[512] = { 0, };
	char pwdDecryptedPWD[512] = { 0, };
	char *findStr = "PWD=";
	char* p = NULL;

	m_strDSN.clear();
	// DSN 파일의 내용을 읽는다
	while (NULL != fgets(bufDSNFileData, 1024 - 1, pFile)) {
		// 암호화 된 PWD부분만 별도의 변수에 저장
		p = strstr(bufDSNFileData, findStr);
		if (NULL != p) {
			strncpy(bufHexPWD, p + 4, 512);
			continue;
		}

		// 개행문자 찾아서 ;로 대체함 ( DRIVER=SQL Server;UID=gc; 이런식으로 됨)
		p = strchr(bufDSNFileData, '\n');
		if (NULL != p) {
			*p = ';';
		}

		m_strDSN += KncUtil::toWideString(bufDSNFileData);
		memset((void*)bufDSNFileData, 0, 1024);
	}
	// 16진수로 된 Key값 풀어서 저장
	unsigned int nStrLen = strlen(strTeaKey);
	m_tea.HexCharToChar(strTeaKey, nStrLen, strHexToChar, 512);
	m_tea.setKey(strHexToChar);
	memset(strHexToChar, 0, 512);

	// PWD 복호화
	/*nStrLen = strlen( bufHexPWD );
	m_tea.HexCharToChar( bufHexPWD, nStrLen, strHexToChar, 512 );
	nStrLen = strlen( strHexToChar ) + 1;
	m_tea.decrypt( strHexToChar, nStrLen );
	nStrLen = strlen( strHexToChar );
	m_tea.HexCharToChar( strHexToChar, nStrLen, pwdDecryptedPWD, 512 );
	*/

	m_strDSN += L"PWD=" + KncUtil::toWideString(bufHexPWD);

	fclose(pFile);
	pFile = NULL;

	////20080922. microcat. set connection pool in odbc
	//Ret = ::SQLSetEnvAttr( NULL,
	//    SQL_ATTR_CONNECTION_POOLING,
	//    ( SQLPOINTER )SQL_CP_ONE_PER_HENV,
	//    //( SQLPOINTER )SQL_CP_ONE_PER_DRIVER,
	//    SQL_IS_INTEGER );
	//if( !SQLOK( Ret ) )
	//{
	//    START_LOG( cerr, L"DriverConnect() Failed. Ret : " << Ret )
	//        << BUILD_LOG( m_strDSN )
	//        << END_LOG;
	//    return false;
	//}

	// allocate environment handle.
	Ret = ::SQLAllocHandle(SQL_HANDLE_ENV,
		SQL_NULL_HANDLE,
		&m_hEnv);

	if (!SQLOK(Ret))
	{
		START_LOG(cerr, L"DriverConnect() Failed. Ret : " << Ret)
			<< BUILD_LOG(m_strDSN)
			<< END_LOG;

		return false;
	}

	// set the ODBC version environment attribute.
	Ret = ::SQLSetEnvAttr(m_hEnv,
		SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3,
		SQL_IS_INTEGER);

	if (!SQLOK(Ret))
	{
		START_LOG(cerr, L"DriverConnect() Failed. Ret : " << Ret)
			<< BUILD_LOG(m_strDSN)
			<< END_LOG;

		return false;
	}

	//allocate connecton handle.
	Ret = ::SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);

	if (!SQLOK(Ret))
	{
		START_LOG(cerr, L"DriverConnect() Failed. Ret : " << Ret)
			<< BUILD_LOG(m_strDSN)
			<< END_LOG;

		return false;
	}

	//dsn파일을 이용해서 연결한다
	wsprintfW((wchar_t*)InCon, L"%s", m_strDSN.c_str());
	Ret = SQLDriverConnectW(m_hDbc, NULL, (SQLWCHAR*)InCon, sizeof(InCon), OutCon,
		sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);

	//에러검사
	if (!SQLOK(Ret))
	{
		START_LOG(cerr, L"DriverConnect() Failed. Ret : " << Ret)
			<< BUILD_LOG(m_strDSN)
			<< END_LOG;

		return false;
	}

	//명령핸들 할당
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);

	if (!SQLOK(Ret))
	{
		START_LOG(cerr, L"DriverConnect() Failed. Ret : " << Ret)
			<< BUILD_LOG(m_strDSN)
			<< END_LOG;

		return false;
	}

	START_LOG(clog, L"DriverConnect() success. m_strDSN : ") << BUILD_LOG(m_strDSN) << END_LOG;

	return true;
}

bool KODBC::ReConnect()
{
	Close();

	// DB연결을 시도를 해본다.  [7/26/2004]
	//if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
	// m_strDSN은 ReConnect가 불릴때, NULL일수 없다. 그래서 의미없이 parameter로 넘겨준다.
	if (!DriverConnect(m_strDSN.c_str()))
	{
		START_LOG(cerr, L"재접속 시도 실패") << END_LOG;
		return false;
	}
	else
	{
		START_LOG(clog, L"재접속 성공") << END_LOG;
		return true;
	}
}

void KODBC::Close()
{
	if (SQL_NULL_HSTMT != m_hStmt)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = SQL_NULL_HSTMT;
	} // if

	if (SQL_NULL_HDBC != m_hDbc)
	{
		SQLDisconnect(m_hDbc);
		::SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		m_hDbc = SQL_NULL_HDBC;
	} // if

	if (SQL_NULL_HENV != m_hEnv)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		m_hEnv = SQL_NULL_HENV;
	} // if

	m_usColumNum = 0;
} // Close()

bool KODBC::Query(const wchar_t* szFormat_, ...)
{
	wchar_t     szQuery[8192];
	va_list     vaList;
	SQLRETURN   ret;

	va_start(vaList, szFormat_);
	vswprintf(szQuery, szFormat_, vaList);
	va_end(vaList);

	m_strBeforeLastQuery = m_strLastQuery;
	m_strLastQuery = szQuery;

	DWORD dwElapTime = ::GetTickCount();  // 051001. florist. 가장 오래 걸린 쿼리 시간을 기억한다.

	// 해당 query에 대한 row가 없는 경우 ret는 SQL_NO_DATA가 return 된다.
	ret = ::SQLExecDirectW(m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS);

	//{{ Modified by ct 2003-04-30 오후 03:53:45
	SQLRETURN     ret2;
	ret2 = SQLGetDiagRecW(SQL_HANDLE_STMT,
		m_hStmt,
		1,
		m_sqlState,
		&m_sqlNativeErr,
		m_szDiagMsg,
		sizeof(m_szDiagMsg) / sizeof(SQLWCHAR),
		&m_sqlMsgLen);
	//}} Modified by ct 2003-04-30 오후 03:53:45

		// godsend odbc연결 실패시 다시 연결하고 쿼리문 다시 실행  [7/22/2004]
	if (ret == SQL_ERROR || ret == SQL_INVALID_HANDLE)
	{
		wchar_t* err1 = L"08S01";  // Communication link failure
		wchar_t* err2 = L"24000";  // Invalid cursor state

		// godsend 커서 오류와 DB연결 안된 오류 두가지 경우 다시 연결  [7/26/2004]
		if (wcscmp((const wchar_t*)m_sqlState, err1) == 0 || wcscmp((const wchar_t*)m_sqlState, err2) == 0)
		{
			START_LOG(cerr, L"Before : " << m_strBeforeLastQuery
				<< L"Current : " << m_strLastQuery
				<< L"(err:" << (const wchar_t*)m_sqlState << L")") << END_LOG;

			// godsend 연결을 끊는다.
			Close();

			// DB연결을 시도를 해본다.  [7/26/2004]
			//if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
			// godsend Connect -> DriverConnect로 변경 [9/6/2005]
			if (!DriverConnect(m_strDSN.c_str()))
			{
				START_LOG(cerr, L"재접속 시도 실패") << END_LOG;
				return false;
			}
			else
			{
				ret = ::SQLExecDirectW(m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS);
			}
		}
	}

	dwElapTime = ::GetTickCount() - dwElapTime;

	{   // lock이 걸리는 범위를 조절하는 scope
		KLocker lock(ms_kMaxProcessingInfo.m_cs);

		if (dwElapTime > ms_kMaxProcessingInfo.m_dwTerm)
		{
			ms_kMaxProcessingInfo.m_strQuery = szQuery;
			ms_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
			ms_kMaxProcessingInfo.m_dwTerm = dwElapTime;

			START_LOG(clog, L"쿼리 최대 소요시간 갱신.")
				<< BUILD_LOG(ms_kMaxProcessingInfo.m_strQuery)
				<< BUILD_LOG(ms_kMaxProcessingInfo.m_dwTerm) << END_LOG;
		}
	}   // lock이 걸리는 범위를 조절하는 scope

	return SQLOK(ret);
} // Query()

bool KODBC::QueryToWString(const std::wstring& wstrQuery_)
{
	SQLRETURN   ret;
	if (wstrQuery_.length() > QUERY_BUFFER_SIZE) {
		START_LOG(cerr, L"쿼리 스트링 길이가 너무 길다!")
			<< BUILD_LOG(wstrQuery_.length())
			<< BUILD_LOG(wstrQuery_)
			<< END_LOG;
	}

	m_strBeforeLastQuery = m_strLastQuery;
	m_strLastQuery = wstrQuery_;

	DWORD dwElapTime = ::GetTickCount();  // 051001. florist. 가장 오래 걸린 쿼리 시간을 기억한다.

	// 해당 query에 대한 row가 없는 경우 ret는 SQL_NO_DATA가 return 된다.
	ret = ::SQLExecDirectW(m_hStmt, (SQLWCHAR*)m_strLastQuery.c_str(), SQL_NTS);

	//{{ Modified by ct 2003-04-30 오후 03:53:45
	SQLRETURN     ret2;
	ret2 = SQLGetDiagRecW(SQL_HANDLE_STMT,
		m_hStmt,
		1,
		m_sqlState,
		&m_sqlNativeErr,
		m_szDiagMsg,
		sizeof(m_szDiagMsg) / sizeof(SQLWCHAR),
		&m_sqlMsgLen);
	//}} Modified by ct 2003-04-30 오후 03:53:45

	// godsend odbc연결 실패시 다시 연결하고 쿼리문 다시 실행  [7/22/2004]
	if (ret == SQL_ERROR || ret == SQL_INVALID_HANDLE)
	{
		wchar_t* err1 = L"08S01";  // Communication link failure
		wchar_t* err2 = L"24000";  // Invalid cursor state

		// godsend 커서 오류와 DB연결 안된 오류 두가지 경우 다시 연결  [7/26/2004]
		if (wcscmp((const wchar_t*)m_sqlState, err1) == 0 || wcscmp((const wchar_t*)m_sqlState, err2) == 0)
		{
			START_LOG(cerr, L"Before : " << m_strBeforeLastQuery
				<< L", Current : " << m_strLastQuery << L"(err:" << (const wchar_t*)m_sqlState << L")")
				<< END_LOG;

			// godsend 연결을 끊는다.
			Close();

			// DB연결을 시도를 해본다.  [7/26/2004]
			//if ( !Connect( m_strDSN.c_str(), m_strLogin.c_str(), m_strPasswd.c_str()) )
			// godsend Connect -> DriverConnect로 변경 [9/6/2005]
			if (!DriverConnect(m_strDSN.c_str()))
			{
				START_LOG(cerr, L"재접속 시도 실패") << END_LOG;
				return false;
			}
			else
			{
				ret = ::SQLExecDirectW(m_hStmt, (SQLWCHAR*)m_strLastQuery.c_str(), SQL_NTS);
			}
		}
	}

	dwElapTime = ::GetTickCount() - dwElapTime;

	{   // lock이 걸리는 범위를 조절하는 scope
		KLocker lock(ms_kMaxProcessingInfo.m_cs);

		if (dwElapTime > ms_kMaxProcessingInfo.m_dwTerm)
		{
			ms_kMaxProcessingInfo.m_strQuery = m_strLastQuery;
			ms_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
			ms_kMaxProcessingInfo.m_dwTerm = dwElapTime;

			START_LOG(clog, L"쿼리 최대 소요시간 갱신.")
				<< BUILD_LOG(ms_kMaxProcessingInfo.m_strQuery)
				<< BUILD_LOG(ms_kMaxProcessingInfo.m_dwTerm)
				<< END_LOG;
		}
	}   // lock이 걸리는 범위를 조절하는 scope

	return SQLOK(ret);
} // Query()

bool KODBC::QueryWithBlob(KSerBuffer& kbuff_, const wchar_t* szFormat_, ...)
{
	wchar_t     szQuery[4096];
	va_list     vaList;
	SQLRETURN   ret;

	va_start(vaList, szFormat_);
	vswprintf(szQuery, szFormat_, vaList);
	va_end(vaList);

	m_strBeforeLastQuery = m_strLastQuery;
	m_strLastQuery = szQuery;

	DWORD dwElapTime = ::GetTickCount();  // 051001. florist. 가장 오래 걸린 쿼리 시간을 기억한다.

	// binding parameter
	SQLINTEGER cbBlob = SQL_LEN_DATA_AT_EXEC(kbuff_.GetLength());
	::SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY,
		kbuff_.GetLength(), 0, (SQLPOINTER)BLOB_DATA_PARAM_MARKER, 0, &cbBlob);

	// 해당 query에 대한 row가 없는 경우 ret는 SQL_NO_DATA가 return 된다.
	ret = ::SQLExecDirectW(m_hStmt, (SQLWCHAR*)szQuery, SQL_NTS);

	if (ret == SQL_NEED_DATA)
	{
		SQLPOINTER pToken;
		ret = ::SQLParamData(m_hStmt, &pToken);
		while (ret == SQL_NEED_DATA) {
			if ((int)pToken == BLOB_DATA_PARAM_MARKER)
			{
				::SQLPutData(m_hStmt, (PTR)kbuff_.GetData(), kbuff_.GetLength());
			}
			ret = ::SQLParamData(m_hStmt, &pToken);
		}
	}

	SQLRETURN     ret2;
	ret2 = SQLGetDiagRecW(SQL_HANDLE_STMT,
		m_hStmt,
		1,
		m_sqlState,
		&m_sqlNativeErr,
		m_szDiagMsg,
		sizeof(m_szDiagMsg) / sizeof(SQLWCHAR),
		&m_sqlMsgLen);

	dwElapTime = ::GetTickCount() - dwElapTime;

	{   // lock이 걸리는 범위를 조절하는 scope
		KLocker lock(ms_kMaxProcessingInfo.m_cs);

		if (dwElapTime > ms_kMaxProcessingInfo.m_dwTerm)
		{
			ms_kMaxProcessingInfo.m_strQuery = szQuery;
			ms_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
			ms_kMaxProcessingInfo.m_dwTerm = dwElapTime;

			START_LOG(clog, L"쿼리 최대 소요시간 갱신.")
				<< BUILD_LOG(ms_kMaxProcessingInfo.m_strQuery)
				<< BUILD_LOG(ms_kMaxProcessingInfo.m_dwTerm) << END_LOG;
		}
	}   // lock이 걸리는 범위를 조절하는 scope

	return SQLOK(ret);
} // Query()

bool KODBC::BeginFetch()
{
	SQLRETURN   ret;
	m_usColumNum = 0;
	ret = ::SQLFetch(m_hStmt);

	if (!SQLOK(ret))
	{
		EndFetch();

		SQLRETURN     ret2;
		ret2 = SQLGetDiagRecW(SQL_HANDLE_STMT,
			m_hStmt,
			1,
			m_sqlState,
			&m_sqlNativeErr,
			m_szDiagMsg,
			sizeof(m_szDiagMsg) / sizeof(SQLWCHAR),
			&m_sqlMsgLen);

		return false;
	}

	return true;
}

SQLRETURN KODBC::Fetch()
{
	m_usColumNum = 0;

	SQLRETURN ret = ::SQLFetch(m_hStmt);

	if (!SQLOK(ret))
	{
		if (ret != SQL_NO_DATA)
			START_LOG(cerr, L"SQL Fetch Error! Ret : " << ret) << END_LOG;
		ret = SQL_NO_DATA;
		EndFetch();
	}

	return ret;
} // Fetch()

void KODBC::EndFetch()
{
	if (SQL_NULL_HSTMT != m_hStmt)
		::SQLCloseCursor(m_hStmt);
}

void KODBC::PrintLastQuery()
{
	START_LOG(cerr, m_szDiagMsg)
		<< L"   Sql State : " << m_sqlState
		<< L"   Last Query : " << m_strLastQuery
		<< L"   Before Last Query : " << m_strBeforeLastQuery << END_LOG;
}

KODBC& KODBC::operator >> (bool& val_)
{
	char        val;
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_STINYINT,
		&val,
		sizeof(bool),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	val_ = !(val == 0);
	return *this;
} // operator >>()

KODBC& KODBC::operator >> (char& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_STINYINT,
		&val_,
		sizeof(char),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (wchar_t& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_WCHAR,
		&val_,
		sizeof(wchar_t),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (short& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_SHORT,
		&val_,
		sizeof(short),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (int& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_SLONG,
		&val_,
		sizeof(int),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (__int64& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_SBIGINT,
		&val_,
		sizeof(__int64),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (UCHAR& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_UTINYINT,
		&val_,
		sizeof(UCHAR),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (USHORT& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_USHORT,
		&val_,
		sizeof(USHORT),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (DWORD& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_ULONG,
		&val_,
		sizeof(DWORD),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (std::string& val_)
{
	char acBuffer[1024] = { 0, };

	*this >> acBuffer;
	val_ = acBuffer;

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (char* val_)
{
	SQLRETURN   ret;
	int         nLen;
	SQLINTEGER  LenBin;

	m_usColumNum++;
	SQLColAttribute(m_hStmt,
		m_usColumNum,
		SQL_DESC_OCTET_LENGTH,
		0, 0, 0,
		&nLen);
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_CHAR,
		val_,
		1024,
		&LenBin);

	if (LenBin == SQL_NULL_DATA)   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
		return *this;

	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (std::wstring& val_)
{
	wchar_t acBuffer[1024] = { 0, };

	*this >> acBuffer;
	val_ = acBuffer;

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (wchar_t* val_)
{
	SQLRETURN   ret;
	int         nLen;
	SQLINTEGER  LenBin;

	m_usColumNum++;
	SQLColAttribute(m_hStmt,
		m_usColumNum,
		SQL_DESC_OCTET_LENGTH,
		0, 0, 0,
		&nLen);
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_WCHAR,
		val_,
		1024,
		&LenBin);

	if (LenBin == SQL_NULL_DATA)   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
		return *this;

	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (float& val_)
{
	SQLRETURN   ret;

	m_usColumNum++;
	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_FLOAT,
		&val_,
		sizeof(float),
		0);
	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
} // operator >>()

KODBC& KODBC::operator >> (KSerBuffer& val_)
{
	SQLRETURN   ret;
	char        cbuff[1024];
	SQLINTEGER  LenBin;

	m_usColumNum++;
	val_.Clear();

	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_BINARY,
		cbuff,
		sizeof(cbuff),
		&LenBin);

	while (ret != SQL_NO_DATA) {
		if (LenBin == SQL_NULL_DATA)   // SqlGetData는 읽을 필드가 NULL이더라도 SQL_SUCCESS를 리턴한다.
			return *this;

		val_.Write(cbuff, LenBin);

		ret = ::SQLGetData(m_hStmt,
			m_usColumNum,
			SQL_C_BINARY,
			cbuff,
			sizeof(cbuff),
			&LenBin);
	}

	if (ret != SQL_NO_DATA)
	{
		THROW_INVALID_QUERY();
	} // if

	return *this;
}

#include <time.h>

KODBC& KODBC::operator >> (time_t& val_)
{
	SQLRETURN   ret;
	TIMESTAMP_STRUCT sqlTime;

	m_usColumNum++;

	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_TYPE_TIMESTAMP,
		&sqlTime,
		sizeof(sqlTime),
		0);

	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	//TIMESTAMP_STRUCT -> struct tm -> time_t
	struct tm time = { 0 };
	time.tm_year = sqlTime.year - 1900;
	time.tm_mon = sqlTime.month - 1;
	time.tm_mday = sqlTime.day;
	time.tm_hour = sqlTime.hour;
	time.tm_min = sqlTime.minute;
	time.tm_sec = sqlTime.second;
	time.tm_isdst = -1; // 잊지 말자 섬머타임 ( 해당 flag를 -1로 세팅해 두면 summer time을 사용하지 않는다 )

	val_ = mktime(&time);
	if (val_ == (time_t)-1)
	{
		THROW_INVALID_QUERY();
	}

	return *this;
}

KODBC& KODBC::operator >> (SYSTEMTIME& val_)
{
	SQLRETURN   ret;
	TIMESTAMP_STRUCT sqlTime;

	m_usColumNum++;

	ret = ::SQLGetData(m_hStmt,
		m_usColumNum,
		SQL_C_TYPE_TIMESTAMP,
		&sqlTime,
		sizeof(sqlTime),
		0);

	if (!SQLOK(ret))
	{
		THROW_INVALID_QUERY();
	} // if

	val_.wYear = sqlTime.year;
	val_.wMonth = sqlTime.month;
	val_.wDay = sqlTime.day;
	val_.wHour = sqlTime.hour;
	val_.wMinute = sqlTime.minute;
	val_.wSecond = sqlTime.second;
	val_.wMilliseconds = 0;
	val_.wDayOfWeek = 0;

	return *this;
}

// 041115
ImplToStringW(KODBC)
{
	//SectionName을 얻어온다.
	wchar_t szSectionName[10] = { 0 };
	GetPrivateProfileSectionNamesW(szSectionName, 10, m_strAbsolutePathDSN.c_str());

	//Address Data를 얻어온다.
	wchar_t szAddress[100] = { 0 };
	GetPrivateProfileStringW(szSectionName, L"Address", 0, szAddress, 100, m_strAbsolutePathDSN.c_str());

	//출력한다.

	return stm_ << std::endl
		<< TOSTRINGW(szAddress)
		<< TOSTRINGW(m_strDSN);
}

void KODBC::DumpMaxTime(std::wostream& stm_)
{
	KLocker lock(ms_kMaxProcessingInfo.m_cs);

	stm_ << L" -- Max Processing query -- " << std::endl
		<< L"  query   : " << ms_kMaxProcessingInfo.m_strQuery.c_str() << std::endl
		<< L"  elapsed : " << ms_kMaxProcessingInfo.m_dwTerm << std::endl
		<< L"  execute : " << (const wchar_t*)ms_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void KODBC::GetDumpMaxTime(OUT std::wstring& query_, OUT DWORD& term_, OUT CTime& time_)
{
	KLocker lock(ms_kMaxProcessingInfo.m_cs);

	query_ = ms_kMaxProcessingInfo.m_strQuery;
	term_ = ms_kMaxProcessingInfo.m_dwTerm;
	time_ = ms_kMaxProcessingInfo.m_cTime;
}

void KODBC::ResetMaxTime()
{
	KLocker lock(ms_kMaxProcessingInfo.m_cs);

	std::string strQuery;

	ms_kMaxProcessingInfo.m_dwTerm = 0;
	ms_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
	strQuery = (CStringA)ms_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	strQuery.append(" 에 데이터 초기화 되었습니다.");
	ms_kMaxProcessingInfo.m_strQuery = KncUtil::toWideString(strQuery.c_str(), strQuery.length());
}

ImplOstmOperatorW2A(KODBC);
ImplOstmOperatorW(KODBC);