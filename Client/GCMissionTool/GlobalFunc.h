#pragma once

namespace GC
{
    std::wstring IntToWchar( int iInteger );
    void GetPairDataFromString( CString str , std::pair< int, int >& Pair );
    void GetIntegerFromString( CString str, int& arg );
    void GetIntListFromString( CString str, std::vector< DWORD >& arg, wchar_t* tok );
    void SetListColumn( CListCtrl& kList,std::vector< std::wstring > vecTitleList, int* aWidth, int iNum );
    bool GetProgramPath( std::string& strPath_ );
    void FillComboBoxFromINI( CComboBox& kCombo, char* strSection, char* strKeyFormat , int iStartIndex = 0 );
    void Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr );
    std::wstring ToWString( std::pair<int, int> value );
    template< class T>
        std::wstring ToWString( T value )
    {
        std::wstringstream strFloat;
        strFloat<<value;
        return strFloat.str();
    }
    float ToFloat( const wchar_t* strFloat );
}