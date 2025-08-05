#include "StdAfx.h"
//#include <sstream>
#include ".\globalfunc.h"
#include "IniReader.h"
#include "define.h"

namespace GC
{
    void GetPairDataFromString( CString str , std::pair< int, int >& Pair )
    {
        str.Remove( L' ' );
        if( str.Find( L"~" ) == -1 )
        {
            Pair.first = _wtoi( str.GetBuffer() );
            return;
        }

        CString First = str.Left( str.Find( L"~" ) );
        CString Second = str.Right( str.GetLength() - str.Find( L"~" ) - 1);
        Pair.first = _wtoi( First.GetBuffer() );
        Pair.second = _wtoi( Second.GetBuffer() );
    }

    void GetIntegerFromString( CString str, int& arg )
    {
        str.Remove( L' ' );
        arg = _wtoi( str.GetBuffer() );
    }

    void GetIntListFromString( CString str, std::vector< DWORD >& arg, wchar_t* tok )
    {
        str.Remove( L' ' );
        if( str.Compare( L"" ) == 0 )
            return;

        if( str.Right( 1 ) != L',' )
            str += L',';
        while(true)
        {
            CString strDword;
            int iPos;

            iPos = str.Find( tok );
            if( iPos == -1 )
                break;
            strDword = str.Left( iPos );
            str = str.Right( str.GetLength() - iPos - 1 );
            int iValue = _wtoi( strDword.GetBuffer() );
            if( iValue != 0 )
                arg.push_back( iValue );
        }
    }

    std::wstring IntToWchar( int iInteger )
    {
        WCHAR strNumText[MAX_PATH];
        _itow( iInteger , strNumText, 10 );
        return strNumText;
    }

    std::wstring ToWString( std::pair<int, int> value )
    {
        WCHAR strText[MAX_PATH] = {0, };
        swprintf( strText, L"%d~%d", value.first, value.second );
        return strText;
    }

    float ToFloat( const wchar_t* strFloat )
    {
        std::wistringstream FloatStream( strFloat );
        float fRet = 0.0f;
        FloatStream>>fRet;
        return fRet;
    }

    void Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr )
    {
        CString strTrans = str_.c_str();
        strTrans.Replace( OldStr, NewStr );
        str_ = strTrans.GetBuffer();
    }

    void SetListColumn( CListCtrl& kList,std::vector< std::wstring > vecTitleList, int* aWidth, int iNum )
    {
        LV_COLUMNW lvcolumn;
        for( int i = 0 ; i < iNum ; ++i )
        {
            lvcolumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
            lvcolumn.fmt = LVCFMT_LEFT; 
            lvcolumn.pszText=(LPWSTR)(LPCWSTR)vecTitleList[i].c_str(); 
            lvcolumn.iSubItem=i; 
            lvcolumn.cx=aWidth[i]; 
            kList.InsertColumn(i,&lvcolumn); 
        }
    }

    bool GetProgramPath( std::string& strPath_ )
    {
        char strFullPath[MAX_PATH] = {0,};
        char szTempPath[MAX_PATH] = {0,};
        char szDirPath[MAX_PATH] = {0,};
        char szDrive[20] = {0,};

        ::GetModuleFileNameA( NULL, szTempPath, sizeof(szTempPath));
        _splitpath(szTempPath,szDrive,szDirPath,NULL,NULL);
        sprintf( strFullPath, "%s%s", szDrive, szDirPath );

        strPath_ = strFullPath;
        return true;
    }

    void FillComboBoxFromINI( CComboBox& kCombo, char* strSection, char* strKeyFormat , int iStartIndex )
    {
        char strOldCurrentDirectory[_MAX_PATH] = {0,};
        std::string strProgramPath;
        GC::GetProgramPath( strProgramPath );
        ::GetCurrentDirectoryA( _MAX_PATH, strOldCurrentDirectory );
        ::SetCurrentDirectoryA( strProgramPath.c_str() );

        strProgramPath.append( MISSION_INI_FILE );
        CIniReader kReader( (char*)strProgramPath.c_str() );

        int iMonsterCount = iStartIndex;
        kCombo.ResetContent();    
        while( true )
        {
            char strKey[MAX_PATH] = {0,};
            sprintf( strKey, strKeyFormat, iMonsterCount );

            std::string strAName;
            strAName.reserve( MAX_PATH );
            strAName = kReader.ReadString( strSection, strKey, "NULL" ); 

            wchar_t strMonsterName[MAX_PATH] = {0,};
            T2W2( strMonsterName, strAName.c_str() );
            if( strAName.compare( "NULL" ) == 0 )
            {   
                break;
            }

            int iIndex = 0;
            if( iMonsterCount == 0 )
            {
                iIndex = kCombo.InsertString( iMonsterCount, strMonsterName );            
            }
            else
            {
                iIndex = kCombo.AddString( strMonsterName );
            }

            ++iMonsterCount;
        }    

        ::SetCurrentDirectoryA( strOldCurrentDirectory );
    }
}