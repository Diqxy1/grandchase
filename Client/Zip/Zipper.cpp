#include "stdafx.h"

#include ".\zipper.h"

#include "IElement.h"
//



CZipper::CZipper(void)
{
    m_hz = NULL;
}

CZipper::~CZipper(void)
{
}


bool CZipper::CreateZipFile( std::wstring strFileName )
{
    m_strFileName = strFileName;
    if( m_strFileName.empty() )
        return false;

    m_hz = CreateZip( m_strFileName.c_str(), 0 );
    if( m_hz == NULL )
        return false;

    return true;
}

bool CZipper::AddFile( std::wstring strFileName )
{
    bool bRet;
    CElementFileInfo* kFileInfo = new CElementFileInfo();
    kFileInfo->SetFileName(strFileName);
    bRet = AddFile(kFileInfo);
    return bRet;
}

bool CZipper::AddFile( IElement* pElement )
{

    CElementFileInfo* pFileInfo = (CElementFileInfo*)pElement;
    std::wstring strDest = pFileInfo->GetFileName();
    
    strDest = strDest.substr( strDest.find_last_of(L"\\")+1 );
    ZRESULT zRes = ZipAdd(m_hz, strDest.c_str(), pFileInfo->GetFileName().c_str() );
    if( zRes != ZR_OK )
        return false;
    return true;
}

void CZipper::Close()
{
    CloseZip(m_hz);
}

std::wstring CZipper::GetZipFileName()
{
    if( m_strFileName.empty() )
        return NULL;
    return m_strFileName.c_str();
}