#include "StdAfx.h"
#include "KGCMultipleLanguages.h"

ImplementSingleton( KGCMultipleLanguages )

KGCMultipleLanguages::KGCMultipleLanguages(void)
: m_bUseMultipleLanguages(false)
, m_iDefaultFontSize(0)
, m_iMiddleFontSize(0)
, m_iLargeFontSize(0)
, m_iTitleFontSize(0)
, m_iSmallFontSize(0)
, m_iLanguageTypeNum(0)
{
    m_vecStrLanguage.push_back(std::string("ENG"));
    m_vecStrLanguage.push_back(std::string("GER"));
    m_vecStrLanguage.push_back(std::string("KOR"));
    m_vecStrLanguage.push_back(std::string("TW"));
    m_vecStrLanguage.push_back(std::string("PT"));
    m_vecStrLanguage.push_back(std::string("ENG"));
    m_vecStrLanguage.push_back(std::string("ENG"));
    m_vecStrLanguage.push_back(std::string("TH"));
    m_vecStrLanguage.push_back(std::string("ID"));
	m_vecStrLanguage.push_back(std::string("ES"));
	m_vecStrLanguage.push_back(std::string("CN"));
}

KGCMultipleLanguages::~KGCMultipleLanguages(void)
{
}

bool KGCMultipleLanguages::UseMultipleLanguages()
{
    return m_bUseMultipleLanguages;
}

void KGCMultipleLanguages::InitMultipleLanguages()
{
    for(int i = 1;__argv[i] != NULL;i++)
    {
        if( 0 == ::strcmp(__argv[i], "-ml") )
        {
            m_bUseMultipleLanguages = true;
            if(__argv[i+1] == NULL)
            {
                goto MULTIPLE_LANGUAGES_ERR;
            }

            for(int j = 0; j <static_cast<int>( m_vecStrLanguage.size());j++){
                if(m_vecStrLanguage[j].compare(__argv[i+1]) == 0){
                    m_iLanguageTypeNum = j;
                    m_strLanguage = m_vecStrLanguage[j];
                    m_bUseMultipleLanguages = true;
                    return;
                }
            }
            break;
        }
    }
MULTIPLE_LANGUAGES_ERR:
    SetBaseLanguagesNum();
    m_bUseMultipleLanguages = false;
}


void KGCMultipleLanguages::InitMultipleLanguages( int i )
{
	m_strLanguage = m_vecStrLanguage[i];
}

std::string KGCMultipleLanguages::GetLanguageType()
{
    return m_strLanguage;
}

std::string KGCMultipleLanguages::GetLanguageTypeNUM()
{
    char buf[20];
    memset(buf,'\0',20);
    itoa(m_iLanguageTypeNum,buf,10);
    return buf;
}



std::string KGCMultipleLanguages::ConvertNationStringFileNameA(const char *fileName, const char *fileType)
{
    std::string strFileName = fileName;
    strFileName += GetLanguageTypeNUM();
    strFileName += fileType;

    std::transform(strFileName.begin(),strFileName.end(),strFileName.begin(),tolower);

    return strFileName;
}

std::wstring KGCMultipleLanguages::ConvertNationStringFileNameW(const WCHAR *fileName, const WCHAR *fileType)
{
    std::wstring strSkillFileName = fileName;
    strSkillFileName += GetLanguageTypeW();
    strSkillFileName += fileType;
    return strSkillFileName;
}



std::wstring KGCMultipleLanguages::GetLanguageTypeW()
{
    USES_CONVERSION;
    return std::wstring(A2W(GetLanguageTypeNUM().c_str()));
}


void KGCMultipleLanguages::CreateFont()
{

#if defined(_THAI_LANG)
#define GC_CREATE_FONT g_pGCDeviceManager2->CreateFontTHA
#else
#define GC_CREATE_FONT g_pGCDeviceManager2->CreateFontA
#endif
    char temp[600];
    ZeroMemory(temp, 600);
    ::GetCurrentDirectoryA(600, temp);
    std::string strINIFileName = temp; 

    strINIFileName += "\\multiplelanguagesinfo.ini";



    std::string strLang = GetLanguageType();


    m_iDefaultFontSize = ::GetPrivateProfileIntA(strLang.c_str(), "DEFAULT_FONT_SIZE" ,14, strINIFileName.c_str());
	m_iSmallFontSize = ::GetPrivateProfileIntA(strLang.c_str(), "SMALL_FONT_SIZE" ,14, strINIFileName.c_str());

    m_iMiddleFontSize = ::GetPrivateProfileIntA(strLang.c_str(), "MIDDLE_FONT_SIZE" ,16, strINIFileName.c_str());
    m_iLargeFontSize = ::GetPrivateProfileIntA(strLang.c_str(), "LARGE_FONT_SIZE" ,20, strINIFileName.c_str());
    m_iTitleFontSize = ::GetPrivateProfileIntA(strLang.c_str(), "TITLE_FONT_SIZE" ,30, strINIFileName.c_str());

    char strFontNameTemp[30] = {0, };
    GetPrivateProfileStringA( strLang.c_str(), "FONT_NAME", "tahoma", strFontNameTemp, 30 - 1, strINIFileName.c_str() );

    std::string strFontName = strFontNameTemp ;

	// 윈도우에 폰트가 설치되었는지 확인하고 인스톨하도록...
#ifdef NATION_CHINA
	char szFontName[MAX_PATH] = {0};
	char szFontFileName[MAX_PATH] = {0};
	strcpy(szFontName, strFontNameTemp);

	std::string strFontFileName = szFontName;
	strFontFileName += ".ttf";
	strcpy(szFontFileName, strFontFileName.c_str());
	g_MyD3D->m_pkUIMgr->CheckInstallFont( szFontFileName, szFontName );
#endif

	g_pkFontMgr_original		= GC_CREATE_FONT( strFontName.c_str(), m_iDefaultFontSize);
	g_pkFontMgrSmall_original	= GC_CREATE_FONT( strFontName.c_str(), m_iSmallFontSize);
    g_pkFontMgrOutline_original = GC_CREATE_FONT( strFontName.c_str(), m_iDefaultFontSize, 1 );
	g_pkFontMgrOutlineSmall_original	= GC_CREATE_FONT( strFontName.c_str(), m_iSmallFontSize);
    g_pkFontMgr14_original		= GC_CREATE_FONT( strFontName.c_str(), m_iMiddleFontSize);
    g_pkFontMgr16_original		= GC_CREATE_FONT( strFontName.c_str(), m_iLargeFontSize);
    g_pkFontMgrTitle_original	= GC_CREATE_FONT( strFontName.c_str(), m_iTitleFontSize);
    g_pkFontMgrOutline14_original = GC_CREATE_FONT( strFontName.c_str(), m_iMiddleFontSize, 1 );
    g_pkFontMgrOutline16_original = GC_CREATE_FONT( strFontName.c_str(), m_iLargeFontSize, 1 );
    g_pkFontMgrOutlineTitle_original = GC_CREATE_FONT( strFontName.c_str(), m_iTitleFontSize, 2 );



    g_pkFontMgr = GC_CREATE_FONT( strFontName.c_str(), m_iDefaultFontSize ,0, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY());
	g_pkFontMgrSmall = GC_CREATE_FONT( strFontName.c_str(), m_iSmallFontSize ,0, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY());
	g_pkFontMgr14 = GC_CREATE_FONT( strFontName.c_str(), m_iMiddleFontSize ,0, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY());
    g_pkFontMgr16 = GC_CREATE_FONT( strFontName.c_str(), m_iLargeFontSize, 0 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY()  );
    g_pkFontMgrTitle = GC_CREATE_FONT( strFontName.c_str(), m_iTitleFontSize, 0 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY()  );
    g_pkFontMgrOutline = GC_CREATE_FONT( strFontName.c_str(), m_iDefaultFontSize, 1,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY()   );
	g_pkFontMgrOutlineSmall = GC_CREATE_FONT( strFontName.c_str(), m_iSmallFontSize ,1, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY());
    g_pkFontMgrOutline14 = GC_CREATE_FONT( strFontName.c_str(), m_iMiddleFontSize, 1,true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY() );
    g_pkFontMgrOutline16 = GC_CREATE_FONT( strFontName.c_str(), m_iLargeFontSize, 1 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY()  );
    g_pkFontMgrOutlineTitle = GC_CREATE_FONT( strFontName.c_str(), m_iTitleFontSize, 2 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY()  );

    g_pkFontMgr_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iDefaultFontSize ,0, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
	g_pkFontMgrSmall_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iSmallFontSize ,0, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
    g_pkFontMgrOutline_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iDefaultFontSize ,1, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
	g_pkFontMgrOutlineSmall_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iSmallFontSize ,1, true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
	g_pkFontMgr14_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iMiddleFontSize, 0 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
    g_pkFontMgr16_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iLargeFontSize, 0 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
    g_pkFontMgrTitle_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iTitleFontSize, 0,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
    g_pkFontMgrOutline14_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iMiddleFontSize, 1,true, FW_NORMAL, g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
    g_pkFontMgrOutline16_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iLargeFontSize, 1 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
    g_pkFontMgrOutlineTitle_mid_scale = GC_CREATE_FONT( strFontName.c_str(), m_iTitleFontSize, 2 ,true, FW_NORMAL,g_pGCDeviceManager2->GetWindowScaleY() / (1024.0f / 768.0f) );
}





int KGCMultipleLanguages::GetDefualtFontSize()
{
#if defined(NATION_IDN)
    return m_iMiddleFontSize;
#else
    return m_iDefaultFontSize;
#endif
}

int KGCMultipleLanguages::GetMiddleFontSize()
{
    return m_iMiddleFontSize;
}

int KGCMultipleLanguages::GetLargeFontSize()
{
    return m_iLargeFontSize;
}

int KGCMultipleLanguages::GetTitleFontSize()
{
    return m_iTitleFontSize;
}

bool KGCMultipleLanguages::LoadStringFile( std::string fileName, std::map<DWORD,std::wstring>& data)
{

    boost::shared_array<char> filedata;
    int nFileLength = 0;
    int nStringLength = 0;
    if(g_pGCDeviceManager2->GetMassFileManager()->LoadMemoryFile(fileName, filedata,nFileLength) == false)
        return false;

    nStringLength = nFileLength/2;

    WCHAR *pWCHAR = new WCHAR[nStringLength];
    wmemset(pWCHAR,0,nStringLength);
    memcpy(pWCHAR,filedata.get(),nFileLength);

    std::wstring str;
    int index = 0;

    while( index < nStringLength && GetLine( pWCHAR,index,L'\n',str ) )
    {
        int iIndex = _wtoi( str.substr(0, str.find( L"\t" ) ).c_str() );
        std::wstring strDataString = str.substr( str.find( L"\t" ) + 1, str.find( L"\n" ) - 2 ); // \t 다음부터 \n 까지
        while( true )
        {
            if( strDataString.find( L"\r" ) == strDataString.npos )
                break;
            strDataString.erase( strDataString.find( L"\r" ) );
        }
        Replacer( strDataString, L"\\n", L"\r\n" );
        data.insert(std::pair<DWORD,std::wstring>(static_cast<DWORD>(iIndex),strDataString));
    }

    SAFE_DELETE_ARRAY( pWCHAR );

    return true;

}

bool KGCMultipleLanguages::LoadStringFile_AutoNationFileName( std::string fileName, std::map<DWORD,std::wstring>& data)
{
	if(UseMultipleLanguages() == false){
		fileName += ".stg";
		return LoadStringFile(fileName,data);		
	}
	fileName += "_";
	std::string strFileName = ConvertNationStringFileNameA(fileName.c_str());
	return LoadStringFile(strFileName,data);		
}

void KGCMultipleLanguages::LoadSortStringFile()
{
    std::string strFileName = ConvertNationStringFileNameA("sort_itemname_");
    LoadStringFile(strFileName,m_mapItemNameString);

    strFileName = ConvertNationStringFileNameA("sort_itemdesc_");
    LoadStringFile(strFileName,m_mapItemDescString);
}


void KGCMultipleLanguages::Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr )
{
    size_t iFindPos = 0;
    while( (iFindPos = str_.find(OldStr)) != std::wstring::npos )
        str_.replace(iFindPos, wcslen(OldStr) , NewStr);
}



std::wstring KGCMultipleLanguages::ConvertAtoW( std::string str )
{
    USES_CONVERSION;
    return std::wstring(A2W(str.c_str()));
}

bool KGCMultipleLanguages::GetLine( WCHAR *wstr , int &index, WCHAR endChar,OUT std::wstring &strData)
{
    int i = index;
    for( ; wstr[i] != endChar && wstr[i] != L'\0' ; i++);

    int lenth = i - index+1;
    if(lenth <= 1) return false;

    WCHAR *pWCHAR = new WCHAR[lenth+1];

    wmemset(pWCHAR,0,lenth+1);
    wmemcpy(pWCHAR, &(wstr[index]),lenth);

    strData = pWCHAR;

    index = i;
    index++;
    SAFE_DELETE_ARRAY( pWCHAR );

    return true;
}

int KGCMultipleLanguages::GetLanguageTypeNumber()
{
    return m_iLanguageTypeNum;
}

void KGCMultipleLanguages::SetBaseLanguagesNum()
{
    //    LANGUAGE_CODE_EU_EN = 0,// -- 유럽 영어
    //    LANGUAGE_CODE_EU_GE = 1,// -- 독일어
    //    LANGUAGE_CODE_KR_KO = 2,// -- 한국어
    //    LANGUAGE_CODE_TW_TW = 3,// -- 대만어
    //    LANGUAGE_CODE_BR_PT = 4,// -- 포루트뇰
    //    LANGUAGE_CODE_US_EN = 5,// -- 미국 영어
    //    LANGUAGE_CODE_PH_EN = 6,// -- 필리핀 영어
    //    LANGUAGE_CODE_TH_TH = 7,// -- 태국어
    //    LANGUAGE_CODE_ID_ID = 8,// -- 인도네시아어
    //    LANGUAGE_CODE_LA_ES = 9,// -- 에스파뇰

#if defined(NATION_EU)
    m_iLanguageTypeNum = LANGUAGE_CODE_EU_EN;
#elif defined(NATION_KOREA)
    m_iLanguageTypeNum = LANGUAGE_CODE_KR_KO;
#elif defined(NATION_TAIWAN)
    m_iLanguageTypeNum = LANGUAGE_CODE_TW_TW;
#elif defined(NATION_BRAZIL)
    m_iLanguageTypeNum = LANGUAGE_CODE_BR_PT;
#elif defined(NATION_USA)
    m_iLanguageTypeNum = LANGUAGE_CODE_US_EN;
#elif defined(NATION_PHILIPPINE)
    m_iLanguageTypeNum = LANGUAGE_CODE_PH_EN;
#elif defined(NATION_THAILAND)
    m_iLanguageTypeNum = LANGUAGE_CODE_TH_TH;
#elif defined(NATION_IDN)
    m_iLanguageTypeNum = LANGUAGE_CODE_ID_ID;
#elif defined(NATION_LATINAMERICA)
    m_iLanguageTypeNum = LANGUAGE_CODE_LA_ES;
#elif defined(NATION_CHINA)
    m_iLanguageTypeNum = LANGUAGE_CODE_CN_CN;
#else
    m_iLanguageTypeNum = LANGUAGE_CODE_KR_KO;
#endif
}

int KGCMultipleLanguages::GetSmallFontSize()
{
	return m_iSmallFontSize;
}

