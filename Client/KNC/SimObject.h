#pragma once

#include <RTTI.H>
#include <ToString.h>
////#include <vector>
//#include <string>
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/noncopyable.hpp>

#define IN
#define OUT

/// ToString, RTTI, RefCount, Name
class KSimObject : public boost::enable_shared_from_this<KSimObject>, public boost::noncopyable
{
    NiDeclareRootRTTI( KSimObject );
    DeclToStringW;

public:
    KSimObject(void);
    virtual ~KSimObject(void);

    const std::wstring& GetName() const         { return m_strName; }
    void SetName(const std::wstring& strName_)  { m_strName = strName_; }
    void SetName(const wchar_t* szName_)        { m_strName = szName_; }

    unsigned long GetUID() const                { return m_dwUID; }
    void SetUID( unsigned long dwUID )          { m_dwUID = dwUID; }
    
    int GetRefCount() const                     { return shared_from_this().use_count() - 1; }

protected:
    std::wstring         m_strName;
    unsigned long        m_dwUID;
};
