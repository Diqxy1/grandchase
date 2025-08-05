#ifndef _KGCMULTIPLELANGUAGES_
#define _KGCMULTIPLELANGUAGES_

#include "KNCSingleton.h"

class KGCMultipleLanguages
{
    DeclareSingleton( KGCMultipleLanguages );


public:
    KGCMultipleLanguages(void);
    ~KGCMultipleLanguages(void);
public:
    void InitMultipleLanguages();
	void InitMultipleLanguages( int i );
	bool UseMultipleLanguages();
    std::string GetLanguageType();
    void CreateFont();

    int GetDefualtFontSize();
	int GetSmallFontSize();
	int GetMiddleFontSize();
    int GetLargeFontSize();
    int GetTitleFontSize();
    std::wstring GetLanguageTypeW();
    int GetLanguageTypeNumber();

    void SetDefaultFontSize(int iSize){m_iDefaultFontSize = iSize;}
	void SetSmallFontSize(int iSize){m_iSmallFontSize = iSize;}
	void SetMiddleFontSize(int iSize){m_iMiddleFontSize = iSize;}
    void SetLargeFontSize(int iSize){m_iLargeFontSize = iSize;}
    void SetTitleFontSize(int iSize){m_iTitleFontSize = iSize;}
    std::string ConvertNationStringFileNameA(const char *fileName , const char *fileType = ".stg");
    std::wstring ConvertNationStringFileNameW(const WCHAR *fileName, const WCHAR *fileType = L".stg");
    void LoadSortStringFile();
    void ClearSortString(){m_mapItemNameString.clear();m_mapItemDescString.clear();}
    bool LoadStringFile( std::string fileName, std::map<DWORD,std::wstring>& data);
    std::string GetLanguageTypeNUM();
	bool LoadStringFile_AutoNationFileName( std::string fileName, std::map<DWORD,std::wstring>& data);
	bool LoadSlangString( std::string filename, int enType );

    inline bool KGCMultipleLanguages::GetItemString( DWORD dwItemID,OUT std::wstring& strItemName,OUT std::wstring& strItemDesc )
    {
        std::map<DWORD,std::wstring>::iterator mItName = m_mapItemNameString.find(dwItemID*10);
        std::map<DWORD,std::wstring>::iterator mItDesc = m_mapItemDescString.find(dwItemID*10);

        if(mItName == m_mapItemNameString.end() || mItDesc == m_mapItemDescString.end()) return false;

        strItemName = mItName->second;
        strItemDesc = mItDesc->second;
        return true;
    }

    void SetBaseLanguagesNum();
	void Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr );
	bool GetLine( WCHAR *wstr , int &index, WCHAR endChar,OUT std::wstring &strData);
private:
    std::wstring ConvertAtoW(std::string str);
	bool m_bUseMultipleLanguages;
    std::string m_strLanguage;
    std::string m_strFontName;
    int m_iLanguageTypeNum;

    std::map<DWORD,std::wstring> m_mapItemNameString;
    std::map<DWORD,std::wstring> m_mapItemDescString;

    std::vector<std::string> m_vecStrLanguage;

    int m_iDefaultFontSize;
	int m_iSmallFontSize;
    int m_iMiddleFontSize;
    int m_iLargeFontSize;
    int m_iTitleFontSize;

    
};
DefSingletonInline( KGCMultipleLanguages );

#endif //_KGCMULTIPLELANGUAGES_