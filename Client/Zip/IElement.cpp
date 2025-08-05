#include "stdafx.h"
#include ".\ielement.h"


IElement::IElement(void)
{
    _SetType( ELEMENTTYPE_NONE );
    m_bValid = false;
}

IElement::~IElement(void)
{
}

IElement::ELEMENTTYPE IElement::GetType()
{
    return m_eType;
}

void IElement::_SetValid( bool bValid )
{
    m_bValid = bValid;
}
bool IElement::IsValid()
{
    return m_bValid;
}

void IElement::_SetType( IElement::ELEMENTTYPE eType )
{
    m_eType = eType;
}


//------------------------------------------------------------------------------------------------//

CElementFileInfo::CElementFileInfo()
{
   _SetType( ELEMENTTYPE_FILEINFO );
   m_bUsable = false;
}

CElementFileInfo::~CElementFileInfo()
{
}

void CElementFileInfo::ValidChk()
{
    _SetValid( !m_strFileName.empty() );
}
void CElementFileInfo::SetFileName( std::wstring strFileName )
{
    m_strFileName = strFileName;
}
std::wstring CElementFileInfo::GetFileName()
{
    return m_strFileName.c_str();
}

bool CElementFileInfo::GetUserble()
{
    return m_bUsable;
}

bool CElementFileInfo::ChkUserble()
{
    FILE *file;
    bool bRet = false;
    file=_wfopen( m_strFileName.c_str(),L"r+");
    if( file !=NULL )
    {
        bRet = true;
        fclose(file);
    }//end if
    m_bUsable = bRet;
    return bRet;
}
//------------------------------------------------------------------------------------------------//

CElementMailInfo::CElementMailInfo()
{
    _SetType( ELEMENTTYPE_MAILINFO );
}

CElementMailInfo::~CElementMailInfo()
{
}

std::wstring CElementMailInfo::GetMailAddress()
{
    return m_strMailAddress;
}

std::wstring CElementMailInfo::GetSMTPAddress()
{
    return m_strSTMPAddress.c_str();
}

void CElementMailInfo::SetMailAddress( std::wstring strMailAddress )
{
    m_strMailAddress = strMailAddress;
}

void CElementMailInfo::SetSMTPAddress( std::wstring strSMTPAddress )
{
    m_strSTMPAddress = strSMTPAddress;
}

void CElementMailInfo::ValidChk()
{
    _SetValid( !m_strMailAddress.empty() &&  !m_strSTMPAddress.empty() );

}
//------------------------------------------------------------------------------------------------//

CElementFTPInfo::CElementFTPInfo()
{
    _SetType( ELEMENTTYPE_FTPINFO );
}
CElementFTPInfo::~CElementFTPInfo()
{
}

std::wstring CElementFTPInfo::GetFTPAddress()
{
    return m_strFTPAddress;
}
std::wstring CElementFTPInfo::GetFTPPort()
{

    return m_strFTPPort;
}
std::wstring CElementFTPInfo::GetUserID()
{
    return m_strUserID;
    
}
std::wstring CElementFTPInfo::GetUserPW()
{
    return m_strUserPW;
}

void CElementFTPInfo::SetFTPAddress( std::wstring strFTPAddress )
{
    m_strFTPAddress = strFTPAddress;
    

}
void CElementFTPInfo::SetFTPPort( std::wstring strFTPPort )
{
    m_strFTPPort    = strFTPPort;
    
}
void CElementFTPInfo::SetUserID( std::wstring strUserID )
{
    m_strUserID     = strUserID;

}
void CElementFTPInfo::SetUserPW( std::wstring strUserPW )
{
    m_strUserPW     = strUserPW;
}

void CElementFTPInfo::ValidChk()
{
    _SetValid( !m_strFTPAddress.empty() &&
               !m_strFTPPort.empty() &&
               !m_strUserID.empty() &&
               !m_strUserPW.empty() );
}

//------------------------------------------------------------------------------------------------//


CElementCrashInfo::CElementCrashInfo()
{
    _SetType( ELEMENTTYPE_CRASH_SERVERINFO );
}
CElementCrashInfo::~CElementCrashInfo()
{
}

std::wstring CElementCrashInfo::GetCrashInfo()
{
    return m_strCrashServer;
}

void CElementCrashInfo::SetCrashInfo( std::wstring strCrashServer )
{
    m_strCrashServer = strCrashServer;
}

void CElementCrashInfo::ValidChk()
{
    _SetValid( !m_strCrashServer.empty() );
}
//------------------------------------------------------------------------------------------------//

IElement* ElementFactory::GetElement( IElement::ELEMENTTYPE eType )
{
    IElement* pElement = NULL;
    switch( eType )
    {
    case IElement::ELEMENTTYPE_FILEINFO:
        {
            pElement = new CElementFileInfo;
        }
        break;
    case IElement::ELEMENTTYPE_MAILINFO:
        {
            pElement = new CElementMailInfo;
        }
        break;
    case IElement::ELEMENTTYPE_FTPINFO:
        {
            pElement = new CElementFTPInfo;
        }
        break;
    case IElement::ELEMENTTYPE_CRASH_SERVERINFO:
        {
            pElement = new CElementCrashInfo;
        }
        break;
    default:
        
        break;
    }//end switch

    //Log

    return pElement;
}
