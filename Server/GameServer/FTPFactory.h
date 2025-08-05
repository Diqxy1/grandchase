#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "KncSmartPtr.h"
#include <map>

// 전방 선언
namespace nsFTP{ class CLogonInfo; }
SmartPtrWithNamespace( nsFTP, CFTPClient );

class KFTPFactory
{
    DeclareSingleton( KFTPFactory );
    NiDeclareRootRTTI( KFTPFactory );
    DeclToStringW;

public:
    KFTPFactory(void);
    ~KFTPFactory(void);
    bool IsInConnectionInfo( IN const std::wstring& strName );

    bool AddConnectionInfoA( IN const char* szName, IN const char* szHost, IN const unsigned short usPort,
                             IN const char* szUserName, IN const char* szPassword ); // for lua
    void DelConnectionInfoA( IN const char* szName ); // for lua
    void ModifyConnectionInfoA( IN const char* szName, IN const char* szHost, IN const unsigned short usPort,
                                IN const char* szUserName, IN const char* szPassword ); // for lua

    bool AddConnectionInfo( IN const std::wstring& strName, IN const std::wstring& strHost, IN const unsigned short usPort,
                            IN const std::wstring& strUserName, IN const std::wstring& strPassword );
    void DelConnectionInfo( IN const std::wstring& strName );
    void ModifyConnectionInfo( IN const std::wstring& strName, IN const std::wstring& strHost, IN const unsigned short usPort,
                               IN const std::wstring& strUserName, IN const std::wstring& strPassword );


    CFTPClientPtr GetConnection( IN const std::wstring& strName );

private:
    void DumpConnections( std::wostream& stm ) const;

private:
    mutable KncCriticalSection                      m_csConnectionInfo;
    std::map<std::wstring, nsFTP::CLogonInfo>       m_mapConnectionInfo;

};

DefSingletonInline( KFTPFactory );
DeclOstmOperatorA( KFTPFactory );