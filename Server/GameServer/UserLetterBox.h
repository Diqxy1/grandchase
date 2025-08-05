#pragma once
#include "UserPacket.h"

class KUserLetterBox
{
public:
    enum LETTER_STATE
    {
        LS_NEW = 0, // 읽지않음
        LS_READ, // 읽음(첨부Item받지않음)
        LS_READ_GET_ITEM, // 읽음(첨부Item받음)
    };

public:
    KUserLetterBox(void);
    ~KUserLetterBox(void);

    void Clear();
    void Init( IN const std::vector<KPostItemInfo>& vecPostItems, 
        OUT int& nNewUserLetterCount, 
        OUT int& nNewSystemLetterCount, 
        OUT std::vector<KPostSmallTitleInfo>& vecNewLetterTitle );

    void GetRenewalLetter( OUT int& nNewUserLetterCount, 
        OUT int& nNewSystemLetterCount, 
        OUT std::vector<KPostSmallTitleInfo>& vecNewLetterTitle );

    void AddLetter( IN const KPostItemInfo& kPostItem );

    bool IsExist( IN const POSTUID& PostUID );
    bool IsExist( IN const std::vector<POSTUID>& vecPostNo );
    bool IsSystemLetter( IN const POSTUID& PostUID );

    bool GetLetter( IN const POSTUID& PostUID, OUT KPostItemInfo& kPostItem );
    bool ReadLetter( IN const POSTUID& PostUID, OUT KPostItemInfo& kPostItem, OUT bool& bReadUpdated );

    int GetTotalLetterCount( IN const char& cLetterListType );
    bool GetItemInfo( IN const POSTUID& PostUID, std::vector<KItem>& vecNewItemInfo );
    bool GetLetterList( IN const char& cLetterListType, IN const int& nPageNo, OUT std::vector<KPostItemTitleInfo>& vecLetterList );

    void DeleteLetter( IN const POSTUID& PostUID );
    void DeleteLetter( IN const std::vector<POSTUID>& vecPostNo );

    void SetReceivedFlag( IN const POSTUID& PostUID );

    // 스팸 편지 방지 블랙리스트 (차후구현)
    void InitBlackList( IN const std::set<DWORD>& setBlackList );
    void GetBlackList( OUT std::set<DWORD>& setBlackList );
    bool IsBlackList( IN const DWORD& dwUserUID );
    void AddBlackList( IN const DWORD& dwUserUID );
    void DelBlackList( IN const DWORD& dwUserUID );

private:
    std::map<POSTUID,KPostItemInfo>     m_mapLetters; // PostUID,Letter
    std::vector<KPostItemTitleInfo>     m_vecUserLetterList;
    std::vector<KPostItemTitleInfo>     m_vecSystemLetterList;

    std::set<DWORD>                     m_setBlackList; // UserUID (차후구현)
};