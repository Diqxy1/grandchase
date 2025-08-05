#include "FTP/FTPClient.h"
#include "FTPFactory.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KFTPFactory );
ImplOstmOperatorW2A( KFTPFactory );
NiImplementRootRTTI( KFTPFactory );

KFTPFactory::KFTPFactory(void)
{
}

KFTPFactory::~KFTPFactory(void)
{
}

ImplToStringW( KFTPFactory )
{
    KLocker lock( m_csConnectionInfo );
    START_TOSTRINGW;
    DumpConnections( stm_ );
    return stm_;

}

bool KFTPFactory::IsInConnectionInfo( IN const std::wstring& strName_ )
{
    KLocker lock( m_csConnectionInfo );
    return ( m_mapConnectionInfo.find( strName_ ) != m_mapConnectionInfo.end() );
}

bool KFTPFactory::AddConnectionInfoA( IN const char* szName_, IN const char* szHost_, IN const unsigned short usPort_,
                                      IN const char* szUserName_, IN const char* szPassword_ )
{
    _JIF( szName_ != NULL && szHost_ != NULL && szUserName_ != NULL && szPassword_ != NULL && usPort_ > 0 , return false );

    return AddConnectionInfo( KncUtil::toWideString(szName_), KncUtil::toWideString(szHost_), usPort_,
                              KncUtil::toWideString(szUserName_), KncUtil::toWideString(szPassword_) );
}

void KFTPFactory::DelConnectionInfoA( IN const char* szName_ )
{
    JIF( szName_ != NULL );

}

void KFTPFactory::ModifyConnectionInfoA( IN const char* szName_, IN const char* szHost_, IN const unsigned short usPort_,
                                        IN const char* szUserName_, IN const char* szPassword_ )
{
    ModifyConnectionInfo( KncUtil::toWideString(szName_), KncUtil::toWideString(szHost_), usPort_,
                          KncUtil::toWideString(szUserName_), KncUtil::toWideString(szPassword_) );
}

bool KFTPFactory::AddConnectionInfo( IN const std::wstring& strName_, IN const std::wstring& strHost_, IN const unsigned short usPort_,
                                     IN const std::wstring& strUserName_, IN const std::wstring& strPassword_ )
{
    nsFTP::CLogonInfo loginInfo( strHost_, usPort_, strUserName_, strPassword_ );
    KLocker lock( m_csConnectionInfo );
    return m_mapConnectionInfo.insert( std::make_pair( strName_, loginInfo) ).second;
}

void KFTPFactory::DelConnectionInfo( IN const std::wstring& strName_ )
{
    KLocker lock( m_csConnectionInfo );
    m_mapConnectionInfo.erase( strName_ );
}

void KFTPFactory::ModifyConnectionInfo( IN const std::wstring& strName_, IN const std::wstring& strHost_, IN const unsigned short usPort_,
                                        IN const std::wstring& strUserName_, IN const std::wstring& strPassword_ )
{
    JIF( IsInConnectionInfo( strName_ ) );
    DelConnectionInfo( strName_ );
    JIF( AddConnectionInfo( strName_, strHost_, usPort_, strUserName_, strPassword_ ) );
}


CFTPClientPtr KFTPFactory::GetConnection( IN const std::wstring& strName_ )
{
    
    std::map<std::wstring, nsFTP::CLogonInfo>::const_iterator mit;
    mit = m_mapConnectionInfo.find( strName_ );
    if( mit == m_mapConnectionInfo.end() ) return CFTPClientPtr();

    CFTPClientPtr spConnection( new nsFTP::CFTPClient() );
    spConnection->Login( mit->second );
    if( !spConnection->IsConnected() ) return CFTPClientPtr();

    return spConnection;
}

void KFTPFactory::DumpConnections( std::wostream& stm ) const
{
    // 밖에서 락을 걸고 들어와야 합니다.
    stm << L"--- Connection Info ---- " << std::endl;
    std::map<std::wstring, nsFTP::CLogonInfo>::const_iterator mit;
    for( mit = m_mapConnectionInfo.begin() ; mit != m_mapConnectionInfo.end() ; ++mit )
    {
        stm << L"[" << mit->first << L"] : "
            << mit->second.Hostname() << L"/"
            << mit->second.Hostport() << L"/"
            << mit->second.Username() << L"/"
            << mit->second.Password() << std::endl;
    }
}