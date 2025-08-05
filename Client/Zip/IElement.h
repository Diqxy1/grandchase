//#include <string>
#include <WTypes.h>

#pragma once

class IElement
{
public:
    enum ELEMENTTYPE
    {
        ELEMENTTYPE_NONE,
        ELEMENTTYPE_FILEINFO,
        ELEMENTTYPE_MAILINFO,
        ELEMENTTYPE_FTPINFO,
        ELEMENTTYPE_CRASH_SERVERINFO,
        ELEMENTTYPE_MAX,
    };//end ELEMENTTYPE

public:
    IElement(void);
    virtual ~IElement(void);
    ELEMENTTYPE GetType();

    
    bool IsValid();
    virtual void ValidChk() = 0;
protected:
    void _SetValid( bool bValid );
    void _SetType( ELEMENTTYPE eType );

private:
    bool m_bValid;
    ELEMENTTYPE m_eType;
};//class IElement

//------------------------------------------------------------------------------------------------//


class CElementFileInfo : public IElement
{
public:
    CElementFileInfo();
    ~CElementFileInfo();

    virtual void ValidChk();
    void SetFileName( std::wstring strFileName );
    std::wstring GetFileName();

    bool GetUserble(); // 파일을 사용해도 되느냐?
    bool ChkUserble();
private:
    bool m_bUsable;
    std::wstring m_strFileName;
    
};//class CElementFileInfo


//------------------------------------------------------------------------------------------------//

class CElementMailInfo : public IElement
{
public:
    CElementMailInfo();
    ~CElementMailInfo();
    
    std::wstring GetMailAddress();
    std::wstring GetSMTPAddress();

    void SetMailAddress( std::wstring strMailAddress );
    void SetSMTPAddress( std::wstring strSMTPAddress );
    
    virtual void ValidChk();
private:
    std::wstring m_strMailAddress;
    std::wstring m_strSTMPAddress;
};//class CElementMailInfo
//------------------------------------------------------------------------------------------------//


class CElementFTPInfo : public IElement
{
public:
    CElementFTPInfo();
    ~CElementFTPInfo();

    std::wstring GetFTPAddress();
    std::wstring GetFTPPort();
    std::wstring GetUserID();
    std::wstring GetUserPW();

    void SetFTPAddress( std::wstring strFTPAddress );
    void SetFTPPort( std::wstring strFTPPort );
    void SetUserID( std::wstring strUserID );
    void SetUserPW( std::wstring strUserPW );

    virtual void ValidChk();
private:
    std::wstring m_strFTPAddress;
    std::wstring m_strFTPPort;
    std::wstring m_strUserID;
    std::wstring m_strUserPW;

};//class CElementFTPInfo

//------------------------------------------------------------------------------------------------//

class CElementCrashInfo : public IElement
{
public:
    CElementCrashInfo();
    ~CElementCrashInfo();

    std::wstring GetCrashInfo();

    void SetCrashInfo( std::wstring strCrashServer );
    virtual void ValidChk();
private:
    std::wstring m_strCrashServer;
};



//------------------------------------------------------------------------------------------------//
namespace ElementFactory
{
    IElement* GetElement( IElement::ELEMENTTYPE eType );
};