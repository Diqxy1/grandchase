#pragma once
#include <KNCSingleton.h>
#include <ToString.h>
#include <vector>
#include <map>
#include <atltime.h> // CTime
#include "UserPacket.h"
#include <Thread/Locker.h>

struct sSanta
{
    int             m_nEventUID;
    time_t          m_tmStartDateTime;
    time_t          m_tmEndDateTime;
    time_t          m_tmDormantDate;
    std::wstring    m_strEventName;
    int             m_nUserType;
    int             m_nCharType;
    int             m_nBeginLv;
    int             m_nEndLv;
    DROP_ITEMS      m_vecItem; // ItemID, factor
    int             m_nItemID;
    std::wstring    m_strComment;

};

class KSanta
{
    DeclareSingleton( KSanta );
    DeclToStringW;

public:
    KSanta(void);
    ~KSanta(void);

    enum ENUM_USERTYPE
    {
        RET_ERROR = -1,
        NEW_USER = 1,
        DORMANT_USER = 2,
        NORMAL_USER = 4,
        ALL_USER = 8,
    };

    void GetSanta(); // 산타 이벤트 목록 얻기. 필요한가...산타 클래스 안에서 처리하면 필요 없음....
    void UpdateSantaList( std::vector< sSanta >& vecSantaList ); // 주기적으로 호출하여 산타 리스트 변경
    int CheckUserType( sSanta& sData_, CTime& firstLogin_, CTime& lastLogin_ );// 휴면, 신규, 활성 유저 판단

    bool CharacterCheck( IN int nCharacter_, std::map<char, KCharacterInfo>& mapCharacterInfo_ ); // 유저의 캐릭터가 산타 해당되는지 판단
    bool LevelCompare( IN int nCharType_, IN int nBeginLevel_, IN int nEndLevel_, std::map<char, KCharacterInfo>& mapCharacterInfo_ ); // 유저 캐릭터의 레벨이 산타 해당되는지 판단
    void SortCategory( IN CTime& tmFirstLogin, IN CTime& tmLastLogin, std::map<char, KCharacterInfo>& mapCharacterInfo_ , OUT std::vector< sSanta >& dwSanta_ );

protected:
    mutable KncCriticalSection      m_csSantaList;
    std::vector< sSanta >           m_vecSantaList;


};

DefSingletonInline( KSanta );
DeclOstmOperatorA( KSanta );