#ifndef INIREADER_H
#define INIREADER_H
#include <string>
class CIniReader
{
public:
    CIniReader(wchar_t* szFileName); 
    int ReadInteger(wchar_t* szSection, wchar_t* szKey, int iDefaultValue);
    float ReadFloat(wchar_t* szSection, wchar_t* szKey, float fltDefaultValue);
    bool ReadBoolean(wchar_t* szSection, wchar_t* szKey, bool bolDefaultValue);
    std::wstring ReadString(wchar_t* szSection, wchar_t* szKey, const wchar_t* szDefaultValue);
private:
    std::wstring m_szFileName;
};
#endif//INIREADER_H

