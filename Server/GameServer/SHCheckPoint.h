#pragma once

#include "KncBitStream.h"
#include "UserPacket.h"
#include <KNCSingleton.h>
#include <RTTI.h>
#include <ToString.h>
#include <Thread\Locker.h>

namespace luabind { class object; } // 전방선언

// 상하이 프로젝트에서 캐릭의 성장에 따라 도움이 되는 아이템을 드랍하는 구조..

struct KCheckCondition //primitive 타입만으로 만들어 져있기 때문에 복사 생성자만들지 않는다.
{
    enum 
    {
        EM_DROP_INVENTORY   = 0, // 인벤토리 지급
        EM_DROP_POST        = 1, // 우편 지급
    }; 
    
    char            m_cCharID;      // 대상 캐릭터 아이디(-1이면 베스트 캐릭터)
    char            m_cLevel;       // 드랍 조건 레벨
    KDropItemInfo   m_kItemInfo;
    bool            m_bRegister;      // 자동 등록 할것인가?
    int             m_nDropType;
};

class KLuaManager;

class KSHCheckPoint
{
    DeclareSingleton( KSHCheckPoint );
    NiDeclareRootRTTI( KSHCheckPoint );
    DeclToStringW;

public:
    enum EMissionControlType
    {
        EMCT_MUST_BE_DELETE = 0,
        EMCT_REGISTER_AGAIN,
    };

    typedef std::map<char,std::vector<KCheckCondition> >::iterator IT_CONDITION;
    struct SHChecData
    {
        char            m_cCurrentCharID;
        char            m_cOldLevel;
        char            m_cLevel;
        std::wstring    m_strLogin;
    };

    KSHCheckPoint(void);
    ~KSHCheckPoint(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool NewUserDropCheck( IN OUT std::vector<KDropItemInfo>& vecItem, IN OUT std::vector<KDropItemInfo>& vecPostItem, IN OUT std::vector<DWORD>& vecAutoMission, IN const std::wstring strLogin, IN const DWORD dwUserType );
    bool DropCheck( IN OUT std::vector<KDropItemInfo>& vecItem, IN OUT std::vector<KDropItemInfo>& vecPostItem, OUT std::vector<DWORD>& vecAutoMission, IN const SHChecData& shData, IN const DWORD dwUserType );
    bool NewCharacterDropCheck( IN OUT std::vector<KDropItemInfo>& vecItem, IN OUT std::vector<KDropItemInfo>& vecPostItem, IN OUT std::vector<DWORD>& vecAutoMission, IN const SHChecData& shData, IN const DWORD dwUserType );
    void CalcCharExclusiveDungeonMission( IN const std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > > mapCheckCondition, IN const std::map< char, DWORD > mapCharExclusiveDungeonMission, OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapCharMissionList );
    void GetCharMissionList( OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapCharMissionList );
    void GetRegistAginMissionList( OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapRegistMissionAfterResetPunishment );

protected:
    void DumpCheckPoint( OUT std::wostream& stm_ ) const;
    bool LoadCondition( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<std::pair<char,char>,std::vector<KCheckCondition> >& vecConditionList );
    bool LoadCharExclusiveDungeonMission( IN KLuaManager& kLuaMng, OUT std::map< char, DWORD >& mapCharExclusiveDungeonMission );
    bool LoadRegistMissionAfterResetPunishment( IN KLuaManager& kLuaMng, OUT std::map< std::pair<char,char>,std::vector<DWORD> >& mapRegistMissionAfterResetPunishment );
    bool LoadRegMission( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<DWORD>& vecRegMission );

    mutable KncCriticalSection      m_csCheckCondition;

    // map[ 채털링타입, map[ 대상 캐릭터 아이디(-1이면 베스트 캐릭터), vector[드랍조건정보] ] ]
    std::map< DWORD, std::map<std::pair<char,char>,std::vector<KCheckCondition> > > m_mapCheckCondition;
    std::map< char, DWORD > m_mapCharExclusiveDungeonMission;
    std::map< std::pair<char,char>,std::vector<DWORD> > m_mapCharMissionList;
    std::map< std::pair<char,char>,std::vector<DWORD> > m_mapRegistMissionAfterResetPunishment;
};
DefSingletonInline( KSHCheckPoint );
DeclOstmOperatorA( KSHCheckPoint );


//CheckPoint =
//{
//    ID = 0 -- 체크 포인트 아이디
//    char = 1 -- -1~3 , -1 = 베스트 캐릭터
//    level = 2 -- 레벨 2 가되는 순간
//    ItemID = 1230
//    factor = --아이템이 수량이면 수량, 일반 아이템의 경우 영구이면 -1, 기간이면 해당 일자
//
//}
//
//SHManager:AddCheckCondition( CheckPoint )