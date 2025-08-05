#pragma once

#include "KAssert.h"
#include <wininet.h>
#include <string>

#include <Thread/Locker.h>

/// HTTP 연결과 파일 다운로드를 담당하는 클래스.
class KHttpWrapper
{
public:
    /// Default constructor.
    KHttpWrapper( void );
    /// Default destructor.
    ~KHttpWrapper( void );
private:
    /// Copy constructor - disabled.
    KHttpWrapper( const KHttpWrapper& )
    { ASSERT( false && _T("Do not use copy constructor!") ); }
    /// Assignment operator - disabled.
    const KHttpWrapper& operator=( const KHttpWrapper& )
    { ASSERT( false && _T("Do not use assignment operator!") ); return *this; }
public:
    /// HTTP 연결을 연다.
    bool Open( void );
    /// HTTP 연결을 닫는다.
    void Close( void );
    /// HTTP 연결을 리셋한다.
    bool Reset( void );
    /// 주어진 파일을 다운로드한다.
    int Get( const WCHAR* szSrcPath_,
              const WCHAR* szFileName_,
              const WCHAR* szDestPath_ ) const;
private:
    /// 주어진 문자열에서 '\'를 '/'로 바꾼다.
    void BackSlash2Slash( const WCHAR* szSrc, WCHAR* szDst ) const;
    /// 주어진 문자열에서 '/'를 '\'로 바꾼다.
	void Slash2BackSlash( const WCHAR* szSrc, WCHAR* szDst ) const;
private:
    HINTERNET   m_hInternet; ///< Internet handle.


	KncCriticalSection m_csLock;
};

