#pragma once
#include <map>
#include <vector>
#include <set>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "Kairo.h"

class KLuaManager;
struct KEVENT_STRENGTH_MATERIAL_INFO;

// 해체 구조체.
struct KBreakUpInfo
{
    GCITEMID                    m_ItemID;          // 해체 ItemID.
    Kairo                       m_kBreakUpRatio;   // 해체 확률.
};

// 강화 속성 구조체.
struct KStrengthAttributeInfo
{
    std::pair<DWORD,DWORD>      m_prAttributeType; // <강화등급, 아이템 장착부위>
    DWORD                       m_dwRandowCount;   // 랜덤 선택 속성 갯수.
    Kairo                       m_kAttributeRatio; // 강화 속성 확률.
};

struct KStrengthUpgradeProtectionAssist
{
    GCITEMID                            m_ItemID;
    float                               m_fRatio;
    std::pair<int,int>                  m_prLevelRange;
};

class KStrengthManager
{
    DeclareSingleton( KStrengthManager );
    NiDeclareRootRTTI( KStrengthManager );
    DeclToStringW;
public:
    enum STRENGTH_RESULT
    {
        SR_SUCCESS      = 1, // 성공
        SR_WEAKNESS     = 2, // 하락
        SR_BREAK        = 3, // 깨짐
        SR_NONE         = 4, // 제자리
        SR_DROP         = 5, // 초기화
        SR_UPGRADE      = 6, // 에픽강화석 변화.
        SR_SPECIFIC     = 7, // 특정 레벨 변화
        SR_MAX,
    };

    enum STRENGTH_TYPE
    {
        ST_STRENGTH     = 0, // 강화.
        ST_BREAKUP      = 1, // 해체.
        ST_COMPOSE      = 2, // 합성.
        ST_MAX,
    };

    enum STRENGTH_GRADE // 아이템 정보랑 동일하게 일단 세팅.
    {
        GRADE_NORMAL   = 0,
        GRADE_RARE     = 1,
        GRADE_EPIC     = 2,
        GRADE_LEGEND = 3,
        GRADE_MYSTIC = 4,
        GRADE_NUM,
    };

    enum ASSIST_TYPE
    {
        AT_NOUSE    = 0,
        AT_UPGRADE  = 1, // 강화률 상승 아이템 사용.
        AT_NOBREAK  = 2, // 파괴 방지 아이템 사용.
        AT_SPECIFIC = 3, // 특정 레벨 강화 아이템 사용.
        AT_RANDOM   = 4, // 랜덤 레벨 강화 아이템 사용.
        AT_UPGRADE_NOBREAK   = 5, // ( 강화률 상승 + 파괴 방지 ) 복합 아이템 
        RT_MAX,
    };

    enum COMPOSE_RESULT
    {
        CR_TYPEA = 0, // 합성 결과물 Type A
        CR_TYPEB = 1, // 합성 결과물 Type B
        CR_TYPEC = 2, // 합성 결과물 Type C
        CR_TYPED = 3, // 합성 결과물 Type D
        CR_TYPEE = 4, // 합성 결과물 Type E
        CR_TYPEF = 5, // 합성 결과물 Type F
    };

    // 스킬 속성의 경우 클라이언트의 enum값을 그대로 사용.
    enum SKILL_ATTRIBUTE
    {
        ATTRTYPE_CRITICAL_DAMAGE_RESIST = 27, // 크리티컬데미지 저항력 - 상대에게서 크리티컬 데미지를 받을 때 효과만큼 데미지를 감소시킨다
        ATTRTYPE_CRITICAL_DAMAGE_UP = 28,     // 크리티컬데미지 증가 
        ATTRTYPE_CATCH_DAMAGE_RESIST =29,     // 잡기 데미지 저항력 
        ATTRTYPE_RECOVERY_UP_PET_MP = 30,     // 펫 MP회복속도 증가 
        ATTRTYPE_MOVEMENT_SPEED_UP = 31,      // 이동속도 증가( 달리기, 걷기 )
        ATTRTYPE_RECOVERY_UP_POTION = 32,     // 회복포션 효과 증가
        ATTRTYPE_FATAL_SUPER_ARMOR_BUFF = 33, // Fatal시 슈퍼아머발동 버프
        ATTRTYPE_FATAL_RECOVERY_MP_UP_BUFF = 34, // Fatat시 MP회복 속도 상승 버프
        ATTRTYPE_FATAL_ATTACK_UP_BUFF = 35,   // Fatat시 공격력 상승 버프
        ATTRTYPE_FATAL_SPEEP_UP_BUFF = 36,    // Fatat시 이동속도 상승 버프
        ATTRTYPE_FATAL_RECOVERY_POTION_UP = 37, // Fatat시 회복포션 효과 상승
        ATTRTYPE_FATAL_CRITICAL_DAMAGE_UP_BUFF = 38, // Fatat시 크리티컬 데미지 상승 버프
        ATTRTYPE_FATAL_ENCHANT_PROBABILITY_UP = 39,  // 강화확률 상승
        ATTRTYPE_FATAL_ENCHANT_DESTORY_RESIST = 40,  // 강화파괴 방지
    };

    enum STRENGTH_UPDATETYPE
    {
        SU_INIT    = 0,
        SU_UPDATE  = 1,
        SU_MAX,
    };

    // 강화하려는 아이템의 타입
    enum EITEM_STRENGTH_POSITION // InitStrength.lua 에 EISP라고 돼 있는게 뭘까 추론해서 적었음
    {
        EISP_HELMET = 1,
        EISP_JACKET = 2,
        EISP_PANTS  = 3,
        EISP_GLOVES = 4,
        EISP_SHOES	= 5,
        EISP_MANTLE = 6,
        EISP_WEAPON	= 7,
    };

    KStrengthManager(void);
    ~KStrengthManager(void);

    void Tick();
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetUpgradeAssistRatio( IN const GCITEMID dwItemID, OUT float& fAddRatio );
    bool StrengthAction( IN const DWORD dwStrengthType, IN const int nStrengthLevel, IN const float fAddRatio, IN const DWORD dwAssistType, OUT DWORD& dwResult );
    bool UpdateSpecificLevel( IN const GCITEMID dwItemID, OUT DWORD& dwLevel );
    bool GetStrengthSkillCount( IN const DWORD dwGrade, OUT int& nCount );
    bool CheckBreakUpMaterial( IN const GCITEMID ItemID );
    bool BreakUpAction( IN const GCITEMID ItemID, OUT DWORD& dwResult );
    bool GetComposeGradeType( IN DWORD dwFirstGrade, IN DWORD dwSecondGrade, OUT DWORD& dwComposeType );
    bool ComposeAction( IN const DWORD dwComposeType, IN const GCITEMID ItemID, OUT DWORD& dwResult );
    bool ComposeResult( IN const DWORD dwComposeResultType, IN int nFirstEnchantLevel, IN int nSecondEnchantLevel, OUT int& nResultLevel, OUT int& nResultGrade );
    bool CheckProtectionLevel( IN const GCITEMID ItemID, IN const int nItemLevel );
    void GetStrengthInfo( OUT KEVENT_STRENGTH_MATERIAL_INFO& kPacket );
    GCITEMID GetStrengthItemID(){ KLocker lock( m_csStrength ); return m_StrengthItemID; }
    bool GetStrengthAttributeInfo( IN const DWORD dwStrengthGrade, IN const DWORD dwEquipItemType, OUT std::list<std::pair<int,float>>& listAttributeList );
    void StrengthNotify( IN const DWORD dwStrengthLevel, IN const std::wstring wstrNickName, IN const int nLanguageType );
    bool GetStrengthNoticeEnable(){ KLocker lock( m_csStrength ); return m_bNoticeEnable; }
    DWORD GetStrengthNoticeMinLevel(){ KLocker lock( m_csStrength ); return m_dwNoticeMinLevel; }
    void SetNoticeInfo( IN std::vector<KSerBuffer> vecCompressedNotice );
    bool GetStrengthMigrationEnable(){ KLocker lock( m_csStrength ); return m_bMigrationEnable; }
    void GetNonStrengthItemList( OUT std::set<GCITEMID>& setNonStrengthItem );
    bool IsUseStrengthEquipItem( IN const GCITEMID ItemID );
    bool CheckUpgradeProtectionLevel( IN const GCITEMID ItemID, IN const int nItemLevel, OUT float& fRatio );
    bool GetStrengthGPCost( IN const DWORD& dwLevel, IN const int& nEquipType, OUT int& nGPCost );

protected:
    bool LoadStrengthGradeSkillInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD, int>& mapStrengthGradeSkill );
    bool LoadStrengthInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT GCITEMID& StrengthItemID_, OUT DWORD& dwMaxStrengthLevel, OUT std::map<std::pair<DWORD,DWORD>, Kairo>& mapStrengthInfo );
    bool LoadRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo );
    bool LoadLevelRange( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT int& nBegin, OUT int& nEnd );
    bool LoadUpgradeAssist( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, float>& mapUpgradeAssistInfo );
    bool LoadSpecificLevel( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, DWORD>& mapSpecificLevelInfo );
    bool LoadRandomLevelStrength( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, Kairo>& mapRandomLevelInfo );
    bool LoadBreakUpItem( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, KBreakUpInfo>& mapBreakUpInfo );
    bool LoadComposeType( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<std::pair<DWORD,DWORD>, DWORD>& mapComposeType );
    bool LoadItemType( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT DWORD& dwItemA, OUT DWORD& dwItemB );
    bool LoadComposeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<std::pair<DWORD,GCITEMID>, Kairo>& mapComposeItem );
    bool LoadComposeResultRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD, std::pair<int, float>>& mapComposeResultRatio );
    bool LoadResultRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT int& nGrade, OUT float& fRatio );
    void ModifyStrengthMaterial( OUT std::map< std::pair<DWORD, DWORD>, std::set< GCITEMID >>& mapStrengthMaterialInfo );
    bool LoadStrengthProtection( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, std::pair<int,int>>& mapStrengthProtectionInfo );
    bool LoadStrengthAttributeRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< std::pair<DWORD, DWORD>, KStrengthAttributeInfo >& mapStrengthAttributeInfo );
    bool LoadAttributeRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo );
    bool LoadShopItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<GCITEMID>& setStrengthShopItemList );
    bool LoadNoticeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT bool& bNoticeEnable, OUT DWORD& dwNoticeMinLevel );
    void SendNoticeInfoToCenter();
    void CompressNoticeInfo( IN const std::vector<std::wstring> vecNoticeInfo );
    void UnCompressNoticeInfo( IN std::vector<KSerBuffer> vecCompressedNotice, OUT std::vector<std::wstring>& vecNoticeInfo );
    bool LoadSetItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<GCITEMID>& setItemList );
    bool LoadUpgradeProtectionAssist( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, KStrengthUpgradeProtectionAssist>& mapUpgradeProtectionAssistInfo );
    bool LoadStrengthGPCost( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD, std::pair<int, int>>& mapStrengthGPCost );

protected:
    mutable KncCriticalSection                  m_csStrength;
    GCITEMID                                    m_StrengthItemID; // 강화석 ItemID
    DWORD                                       m_dwMaxStrengthLevel; // 강화 최고 레벨.
    bool                                        m_bNoticeEnable;          // 강화 공지 출력 여부.
    DWORD                                       m_dwNoticeMinLevel;       // 공지 최소 레벨.
    DWORD                                       m_dwTickGap;              // TickGap.
    DWORD                                       m_dwLastTick;             // LastTick.
    bool                                        m_bMigrationEnable;       // 강화 마이그레이션 여부.

    std::map<std::pair<DWORD,DWORD>, Kairo>     m_mapStrengthInfo; // 아이템 레벨별 강화 정보.
    std::map<DWORD, int>                        m_mapStrengthGradeSkill; // 강화석 등급에 따른 스킬 갯수.
    std::map<GCITEMID, float>                   m_mapUpgradeAssistInfo; // 강화 확률 상승 아이템 정보.
    std::map<GCITEMID, std::pair<int,int>>      m_mapStrengthProtectionInfo; // 강화 보호 아이템 정보.
    std::map<GCITEMID, DWORD>                   m_mapSpecificLevelInfo; // 특정 강화 레벨 변경 아이템 정보.
    std::map<GCITEMID, Kairo>                   m_mapRandomLevelInfo; // 랜덤 강화 레벨 아이템 정보.
    std::map<GCITEMID, KBreakUpInfo>            m_mapBreakUpInfo; // 아이템 해체 정보.
    std::map<std::pair<DWORD,DWORD>, DWORD>     m_mapComposeType; // 아이템 합성 Type 정보.
    std::map<std::pair<DWORD,GCITEMID>, Kairo>  m_mapComposeItem; // 아이템 합성 정보.
    std::map< DWORD, std::pair<int, float> >    m_mapComposeResultRatio; // 아이템 합성 배율.
    std::map< std::pair<DWORD, DWORD>, std::set< GCITEMID >> m_mapStrengthMaterialInfo;
    std::map< std::pair<DWORD, DWORD>, KStrengthAttributeInfo > m_mapStrengthAttributeInfo; // 강화 속성 정보.
    std::set<GCITEMID>                          m_setStrengthShopItemList; // 상점 판매 아이템 리스트.
    std::vector<std::wstring>                   m_vecNoticeInfo;          // 공지 정보.
    std::vector<KSerBuffer>                     m_vecCompressedNotice;
    std::set<GCITEMID>                          m_setNonStrengthItem; // 강화석 장착되지 않는 아이템 리스트.
    std::map< GCITEMID, KStrengthUpgradeProtectionAssist> m_mapUpgradeProtectionAssistInfo; // 강화 확률 상승 보호 복합 아이템 정보.
    std::map<GCITEMID, std::pair<int,int>>      m_mapProtectionInfoList; // 강화 보호 레벨 정보( 클라이언트 전달용 )
    std::map<DWORD, std::pair<int, int> >       m_mapStrengthGPCost; // 강화 시도 시 필요한 GP
};

DefSingletonInline( KStrengthManager );
DeclOstmOperatorA( KStrengthManager );