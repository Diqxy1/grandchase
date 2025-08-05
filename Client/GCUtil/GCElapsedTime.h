#pragma once
#include <WTypes.h>
//#include <string>

class GCElapsedTime
{
public:
    GCElapsedTime(void);
    GCElapsedTime( const std::wstring& strLog );
    virtual ~GCElapsedTime(void);

private:
    DWORD m_dwElapsedTime;
    std::wstring m_strLog;
};
