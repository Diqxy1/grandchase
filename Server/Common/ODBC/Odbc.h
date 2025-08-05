#ifndef ODBC_H
#define ODBC_H "$Id: ODBC.H,v 1.1 2003/05/23 01:18:40 cvs Exp $"

#include <ostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "Serializer/SerBuffer.h"
#include <ToString.h>
#include <atltime.h>    // CTime
#include "KncException.h"
#include "KncSmartPtr.h"
#include "Tea.h"

SmartPointer(KODBC);

class KODBC
{
public:

#   define SQLOK(result) (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
#   define SQLNODATA(result) (result == SQL_NO_DATA)
#   define THROW_INVALID_QUERY() { PrintLastQuery(); throw std::exception("DB Query Exception"); }
#   define VERIFY_QUERY(query) \
            { \
                try \
                { \
                    query;\
                } \
                catch( std::exception& ex) \
                { \
                dbg::cout << LOG_HEADER_STR << KncUtil::toWideString( ex.what() ) << END_LOG; \
                } \
                catch( ... ) \
                { \
                dbg::cout << LOG_HEADER_STR << L"Unknown Exception Occured." << END_LOG; \
                } \
            }
	/*#   define VERIFY_QUERY( query ) \
				{ \
					try \
					{ \
						m_kODBC >> varlist;\
					} \
					catch( std::exception& ex) \
					{ \
					dbg::cout << LOG_HEADER_STR << KncUtil::toWideString( ex.what() ) << END_LOG; \
					} \
					catch( ... ) \
					{ \
					dbg::cout << LOG_HEADER_STR << L"Unknown Exception Occured." << END_LOG; \
					} \
				}*/

	enum ODBC_INFO
	{
		QUERY_BUFFER_SIZE = 8192,
	};

	DeclToStringW;

	KODBC();
	virtual ~KODBC();

	bool DriverConnect(const wchar_t* szDsn);
	bool ReConnect();
	void Close();

	bool Query(const wchar_t* szFormat, ...);
	bool QueryWithBlob(KSerBuffer& kbuff, const wchar_t* szFormat, ...);
	bool QueryToWString(const std::wstring& wstrQuery);

	bool BeginFetch();
	SQLRETURN Fetch();
	void EndFetch();

	void PrintLastQuery();

	KODBC& operator >> (bool& val);
	KODBC& operator >> (char& val);
	KODBC& operator >> (wchar_t& val);
	KODBC& operator >> (short& val);
	KODBC& operator >> (int& val);
	KODBC& operator >> (UCHAR& val);
	KODBC& operator >> (USHORT& val);
	KODBC& operator >> (DWORD& val);
	KODBC& operator >> (char* val);
	KODBC& operator >> (std::string& val);
	KODBC& operator >> (float& val);
	KODBC& operator >> (KSerBuffer& val);
	KODBC& operator >> (time_t& val);            // 040728
	KODBC& operator >> (wchar_t* val);           // 051229
	KODBC& operator >> (std::wstring& val);      // 051229
	KODBC& operator >> (__int64& val);           // 060921
	KODBC& operator >> (SYSTEMTIME& val);

	CRITICAL_SECTION& GetCS() { return m_cs; }

	static void DumpMaxTime(std::wostream& stm);
	static void ResetMaxTime();
	static void GetDumpMaxTime(OUT std::wstring& query_, OUT DWORD& term_, OUT CTime& time_);
	//protected:
public:
	enum {
		BLOB_DATA_PARAM_MARKER,     // 040622. blob 데이터를 전송하기 위해 사용할 아이디
	};
	SQLHENV             m_hEnv;
	SQLHDBC             m_hDbc;
	SQLHSTMT            m_hStmt;

	USHORT              m_usColumNum;
	CRITICAL_SECTION    m_cs;

public:
	// diagnostic message
	SQLWCHAR            m_szDiagMsg[SQL_MAX_MESSAGE_LENGTH];
	SQLSMALLINT         m_sqlMsgLen;
	SQLWCHAR            m_sqlState[6];
	SQLINTEGER          m_sqlNativeErr;

	std::wstring         m_strDSN;
	std::wstring         m_strAbsolutePathDSN;

	std::wstring         m_strLastQuery;
	std::wstring         m_strBeforeLastQuery;

	static struct KMaxProcessingInfo {
		KncCriticalSection  m_cs;
		std::wstring        m_strQuery;
		DWORD               m_dwTerm;       // 이벤트 하나를 처리완료하기까지 걸린 시간.
		CTime               m_cTime;
	} ms_kMaxProcessingInfo;

	minTea m_tea;
};

DeclOstmOperatorA(KODBC);
DeclOstmOperatorW(KODBC);

#ifndef _DBJIF
#define _DBJIF( x, action, odbc ) \
    if( !(x) ) \
{ \
    odbc->PrintLastQuery(); \
    action; \
}
#endif

#define DBJIF(x) _DBJIF( x, return, (&m_kODBC) )
#define DBJIF2(x) _DBJIF( x, return false, (&m_kODBC) )
#define DBJIF3(x, action ) _DBJIF( x, action, (&m_kODBC) )
#endif

#define DeclareDBThread \
    public: \
    static void InitFileDSN( const char* szFileDSN ); \
    protected: \
    static std::wstring ms_wstrFileDSN; \
    KODBC               m_kODBC

#define ImplementDBThread( classname ) \
    std::wstring classname##::ms_wstrFileDSN; \
    void classname##::InitFileDSN( const char* szFileDSN_ ) \
{ ms_wstrFileDSN = KncUtil::toWideString( szFileDSN_ ); }

#define ConnectDBThread()   m_kODBC.DriverConnect( ms_wstrFileDSN.c_str() )

#define DeclareDBThreadName( Name ) \
    public: \
    static void InitFileDSN##Name( const char* szFileDSN ); \
    protected: \
    static std::wstring ms_wstrFileDSN##Name; \
    KODBC               m_kODBC##Name

#define ImplementDBThreadName( classname, Name ) \
    std::wstring classname##::ms_wstrFileDSN##Name; \
    void classname##::InitFileDSN##Name( const char* szFileDSN_ ) \
{ ms_wstrFileDSN##Name = KncUtil::toWideString( szFileDSN_ ); }

#define ConnectDBThreadName( Name )     m_kODBC##Name.DriverConnect( ms_wstrFileDSN##Name.c_str() )

#define QUERY_WSTRING( query, argtype, arg ) \
{ \
    std::wstring wstrQuery; \
    try { \
    wstrQuery = boost::str(boost::wformat( query L" " argtype ) arg ); \
} catch (const boost::io::format_error& e) { \
    START_LOG( cerr, e.what() ) \
        << BUILD_LOG( query L" " argtype ) \
        << END_LOG; \
    goto END_PROC; \
} \
    if( !m_kODBC.QueryToWString( wstrQuery ) ) \
{ \
    START_LOG( cerr, wstrQuery ) \
        << BUILD_LOG( m_kODBC.m_strLastQuery ) \
        << BUILD_LOG( m_kODBC.m_szDiagMsg ) \
        << BUILD_LOG( (wchar_t*)m_kODBC.m_sqlState ) << END_LOG; \
    goto END_PROC; \
}
