#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include <set>
#include "Kairo.h"
#include "UserPacket.h"

struct KPlantTreeInfo;
class KLuaManager;

class KPlantTree
{
    DeclareSingleton( KPlantTree );
    NiDeclareRootRTTI( KPlantTree );
    DeclToStringW;
public:
    typedef std::vector<KDropItemInfo > VEC_REWARD;

public:
    KPlantTree(void);
    ~KPlantTree(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsEventTerm();
    void GetPlantTreeInfo( OUT std::map< DWORD, KPlantTreeInfo >& mapPlantTreeInfo );
    DWORD GetPlantTreeVersion() { KLocker lock(m_csPlantTree); return m_dwPlantTreeVersion; }
    bool DoGamble( OUT DWORD& dwType );
    bool IsGoldTreeItem( IN const GCITEMID& dwItemID );
    bool IsWaterItem( IN const GCITEMID& dwItemID );
    bool IsManureItem( IN const GCITEMID& dwItemID );
    DWORD GetMaxContinent() { KLocker lock(m_csPlantTree); return m_dwMaxContinent; }
    void CheckTreeStatus( IN OUT DWORD& dwTreeStatus );

private:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadPlantTreeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< DWORD, KPlantTreeInfo>& mapPlantTreeInfo );
    bool LoadRewardList( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT VEC_REWARD& vecReward );
    bool LoadRewardList( IN KLuaManager& kLuaMng, OUT VEC_REWARD& vecReward );
    bool LoadTypeReward(IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::map<DWORD,VEC_REWARD>& mapRewardList );
    bool LoadManureRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo );
    bool LoadTreeStatus( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<DWORD>& setTreeStatusInfo );

private:
    mutable KncCriticalSection  m_csPlantTree;
    time_t                      m_tmBegin;          // 이벤트 시작 날짜.
    time_t                      m_tmEnd;            // 이벤트 종료 날짜.

    DWORD                       m_dwPlantTreeVersion; // 나무나무 이벤트 버전.
    DWORD                       m_dwPlantTreeBoard;   // 나무나무 이벤트 말판 ItemID
    GCITEMID                       m_dwGoldTreeItemID;   // 황금나무 ItemID
    GCITEMID                       m_dwWaterItemID;      // 물주기 ItemID
    GCITEMID                       m_dwManureItemID;     // 거름주기 ItemID
    DWORD                       m_dwMaxContinent;     // Max 대륙Num.

    std::map< DWORD, KPlantTreeInfo > m_mapPlantTreeInfo;  // 대륙별 나무 설정 전체 정보.
    Kairo                       m_kairoInfo;       // 거름 설정시의 확률 설정.
    std::set< DWORD >           m_setTreeStatus;    // 나무 상태값 리스트.
};

DefSingletonInline( KPlantTree );
DeclOstmOperatorA( KPlantTree );

//====================================================================================//

struct KPlantTreeInfo;
struct KEVENT_PLANT_TREE_INFO_ACK;
class KPlantTreeUserData
{
public:
    enum ActionType 
    {
        AT_PLANT_TREE            = 1,    // 일반 나무 심기.
        AT_PLANT_GOLD_TREE       = 2,    // 황금 나무 심기.
        AT_WATER_THE_TREE        = 3,    // 나무에 물주기.
        AT_MANURE_THE_TREE       = 4,    // 나무에 거름주기.
        AT_MAX,
    };

    enum TreeStatus
    {
        TS_EMPTY                    = 0,       // 나무를 심을수 있는 상태.
        TS_NORMAL_INIT_TREE         = 10,      // 애기나무 상태.
        TS_WATER_NORMAL_TREE        = 11,      // 일반 나무 물 1회준 상태. 11 ~ 50
        TS_GOLD_INIT_TREE           = 50,      // 황금나무 상태.
        TS_WATER_GOLD_TREE          = 51,      // 황금 나무 물 1회준 상태. 51 ~ 99
        TS_NORMAL_FRUIT_WATER_TREE  = 100,     // 일반 나무 열매 맺은 상태.
        TS_GOLD_FRUIT_WATER_TREE    = 200,     // 황금 나무 열매 맺은 상태.
        TS_CONTINENT_REWARD_TREE    = 300,     // 대륙 보상을 받지 않은 나무 상태.
        TS_DEAD_TREE                = 400,     // 나무 죽은 상태.
        TS_COMPLETE_TREE            = 500,     // 전체 대륙 보상 획득 완료
        TS_MAX,
    };

    enum ErrorType
    {
        ET_SUCCESS                  = 0,        // 유저 행동 성공 상태.
        ET_ERROR_PLANT_TREE         = 3,        // 나무 심을수 없는 상태.
        ET_ERROR_ITEM               = 4,        // 잘못된 ItemID.
        ET_ERROR_CONTINENT_TYPE     = 5,        // 잘못된 대륙 타입.
        ET_ERROR_WATER_TREE         = 6,        // 물을 줄수 없는 상태.
        ET_ERROR_MANURE_TREE        = 7,        // 거름을 줄수 없는 상태.
        ET_ERROR_MANURE_TREE_KAIRO  = 8,        // 로터리에 등록되어 있지 않음.
        ET_UNDEFIND_ACTION_TYPE     = 9,        // 정의되지 않은 행동 타입.
        ET_MAX,
    };

    enum ManureType
    {
        MT_TREE_DIE                 = 0,        // 죽은 나무 상태.
        MT_TREE_NORMAL              = 1,        // 일반 나무 물준 상태.
        MT_TREE_GOLD                = 2,        // 황금 나무로 변경.
        MT_MAX,
    };

    enum RewardType
    {
        RT_NORMAL_FRUIT_TYPE = 0,               // 일반나무 열매 보상.
        RT_GOLD_FRUIT_TYPE = 1,                 // 황금나무 열매 보상.
        RT_CONTINENT_TYPE = 2,                  // 대륙 보상.
    };

    KPlantTreeUserData();
    ~KPlantTreeUserData();
    bool IsLoaded() { return m_bLoaded; }
    void SetPlantTreeData( IN KEVENT_PLANT_TREE_INFO_ACK& kData );
    bool ApplyUserAction( IN DWORD& dwActionType, IN GCITEMID dwItemID_, OUT int& nReturn );
    bool SetPlantTree( IN DWORD& dwTreeType, IN GCITEMID& dwItemID, OUT int& nReturn ); // 나무 심기
    bool SetWaterThePlant( IN GCITEMID& dwItemID, OUT int& nReturn ); // 나무 물주기.
    bool SetManureThePlant( IN GCITEMID& dwItemID, OUT int& nReturn ); // 나무 거름주기.
    void GetPlantTreeUserInfo( OUT DWORD& dwCurrentContinent, OUT DWORD& dwCurrentPosition, OUT DWORD& dwCurrentTreeStatus );
    bool CheckUserStatus( IN const DWORD& dwRewardType ); // 보상 받을수 있는지 체크.
    bool GetPlantTreeReward( IN const DWORD& dwRewardType, OUT std::vector<KDropItemInfo>& vecReward ); // 보상 아이템 가져오기.
    void ApplyRewardPosition( IN const DWORD& dwRewardType );

private:
    bool                            m_bLoaded;
    DWORD                           m_dwCurrentContinent; // 현재 나의 대륙위치.
    DWORD                           m_dwCurrentPosition;  // 현재 대륙에서의 나의 위치.
    DWORD                           m_dwCurrentTreeStatus; // 현재 나의 나무 상태.
};