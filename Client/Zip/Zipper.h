#pragma once
#include "zip.h"
//#include <string>

class IElement;

class CZipper
{
public:
    CZipper(void);
    ~CZipper(void);

    bool CreateZipFile(std::wstring strFileName);
    bool AddFile( IElement* pElement );
    bool AddFile( std::wstring strFileName ); // file name 
    void Close();
    std::wstring GetZipFileName();

private:
    HZIP m_hz;
    std::wstring m_strFileName;
};
