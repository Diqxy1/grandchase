#pragma once
//#include <string>
//#include <map>

typedef std::map< DWORD, std::wstring > StringList;

class KGCMissionString
{
public:
    KGCMissionString(void);
    ~KGCMissionString(void);

    void Clear();
    bool LoadStringFile( std::string strFileName , FILE* pFile = NULL, bool bForTool = false );
    bool OutPutStringFile( std::string strFileName );
    int AddMissionString( std::wstring strString ); //이 스트링과 동일한 스트링이 있는지 찾아서 없으면 새로 Index를 만들고 있으면 해당 Index를 리턴.
    bool GetMissionString( int iIndex, std::wstring& strOut ); //해당 인덱스의 스트링을 반환.
    void Replacer( std::wstring& str_, wchar_t* OldStr, wchar_t* NewStr );
	const std::wstring& GetString( int iIndex );
    bool LoadStringFromKOM( std::string strFileName );

private:
    std::map< DWORD, std::wstring > m_mapMissionString;    //map< pair< MissionID, StringCount > 
    int m_iLastCount;
};