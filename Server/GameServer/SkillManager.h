#pragma once
#include "UserPacket.h"
#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <RTTI.H>

class KLuaManager;

struct SSkillElement
{
    int             m_nID;
    int             m_nGroupID;
    std::set<int>   m_setPreList;
    std::set<int>   m_setNextList;
    int             m_nCharLv;
    char            m_cCharType;
    char            m_cPromotion;
    bool            m_bEnable;
    int             m_nLearnSP;
    int             m_nLockGroupID;
    bool            m_bBase;
    bool            m_bOrCheckPreList;
};

struct SSkillConfig
{
    int                 m_nMaxSkillCount;
    std::set<DWORD>     m_setRemoveSkillItem;
    std::set<DWORD>     m_setResetSkillItem;
    GCITEMID               m_dwUnLockItemID;
    int                 m_nUnLockCount;

    SSkillConfig()
    {
        m_nMaxSkillCount = 2;
        m_dwUnLockItemID = 8880;
        m_nUnLockCount   = 1;

    }
    bool CheckValid()
    {
        if( m_nMaxSkillCount < 2 ) return false;
        if( m_dwUnLockItemID <= 0 ) return false;
        if( m_nUnLockCount < 1 ) return false;

        return true;
    }
};

class KSkillManager
{
    DeclareSingleton( KSkillManager );
    NiDeclareRootRTTI( KSkillManager );
    DeclToStringW;
public:
    KSkillManager(void);
    ~KSkillManager(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void PrintSkill( int nID );
    bool GetSkill( IN int nID, OUT SSkillElement& sSkill );

    // utill
    void InitSkillInfo( OUT KSkillInfo& kSkill, IN const char cCharType, IN const char cPromotion, IN const DWORD dwLevel ); // 스킬 정보 초기화.
    bool GetMaxSPX( IN const char cChar, IN const char cPromotion, OUT int& nMaxSkillPoint );
    int GetAccSPCount( char cCharType, int nLv );
    void UpdateSpInfo( IN OUT int& nSpLv, IN OUT DWORD& dwSPX, IN DWORD dwObtainSpx, IN const char cChar, IN const char cCharType );
    void InitSkillSetting( char cMaxPromotion, std::map<char, int>& m_mapEquipSkillSet ); // 장착 정보 초기화.
    int GetMaxSkillSetNum() { KLocker lock( m_csConfig); return m_sSkillConfig.m_nMaxSkillCount; };
    bool IsRemoveSkillItem( GCITEMID dwItemID );
    void InitSkillSets( IN const std::map<char, KCharacterInfo>& mapCharacterInfo,
                        OUT std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapSkillSets );
    int GetMaxTrainSP( char cChar, char cPromotion );
    int CalcSpendSP( std::vector<int>& vecTrainSkill );

    bool IsLockGroupID( int nLockGroupID, char& cCharType );
    GCITEMID GetUnLockItemID();
    int GetUnLockItemCount();

    // 현재 캐릭터의 최대 SPX MaxLevel이 있는지 체크.
    bool CheckMaxCharSPLevel( IN const char cChar, IN const char cPromotion, IN OUT int& nLevel, IN OUT DWORD& dwSpx );

    // 캐릭터 ExceptionID List에 대한 체크를 진행한다.
    bool CheckSkillExceptionID( IN const char cChar, IN const int nID );
    bool GetSkillPointInfo( IN const char cChar, IN const char cPromotion, IN const DWORD dwLevel, OUT int& nSkillPoint );
    bool SetNewCharSkillInfo( IN const char& cChar, IN const int& nLv, OUT int& nSkillPoint, OUT int& nMaxSkillPoint );
    bool GetCharStartSPX( IN const int nSPXType, OUT DWORD& dwStartSPX );
    bool CheckExceptionCharType( IN const char cChar );
    bool IsResetSkillItem( IN const GCITEMID& ItemID );
    void GetMaxCharSPLevel( OUT std::map< std::pair<char,char>, std::pair<int,int> >& mapData_ );
    bool CheckAPCharAddSlotOpen( IN const GCITEMID& ItemID, IN const int& nCharType );
    int GetMaxCharSlotNum() { KLocker lock( m_csAPCharAddSlotOpen ); return m_nMaxCharSlotNum; };

    bool CheckMissionRewardSkill( IN const int& nSkillID, OUT std::vector<DWORD>& vecMissionInfo );
    void GetSkillMissionInfo( OUT std::map<DWORD, int>& mapSkillMissionInfo );
    void GetDBUpdateSkillMissionInfo( OUT std::map<DWORD, std::pair<int,DWORD>>& mapDBUpdateSkillMissionInfo );
    bool GetSkillMissionID( IN const DWORD& dwMissionID, OUT int& nSkillID );
    bool CheckAbilitySkill( IN const char& cChar, IN const int& nSkillID );
    bool CheckAbilitySkillChar( IN const char& cChar );
    bool CheckExceptionSkill( IN const int& nSkillID );
    //void GetOldSkillTreeUseCharacter( OUT std::wstring& wstrChar_ );
    void GetOldSkillTreeUseCharacter( OUT std::set<char>& setChar_ );
    void GetSkillTreeOpenCharInfo( OUT std::set<std::pair<std::string, std::string>>& setSkillTreeOpenCharInfo );
    bool CheckRewardSkillMission( IN const DWORD& dwMissionID, OUT int& nSkillID );
    bool IsSkillOpenKeyUseSkill( IN const int& nSkillID );
    GCITEMID GetSkillOpenKeyInfo() { KLocker lock( m_csSkillTreeOpenChar ); return m_SkillOpenItemID; };
    bool IsOldSkillTreeUseCharacter( IN const char cCharacter_ );
    bool GetSkillScroolInfo( IN const GCITEMID& dwItemID, OUT int& nSkillID );

protected:
    bool LoadSkillTreeScript( OUT std::string& strScript );
    bool LoadSkillElement( IN KLuaManager& kLuaMng, OUT SSkillElement& sSkill );
    bool LoadList( IN KLuaManager& kLuaMng, OUT std::set<int>& setList );
    bool LoadSPXScript( KLuaManager& kLuaMng );
    bool LoadConfig( KLuaManager& kLuaMng );
    bool LoadMaxTrainPoint( KLuaManager& kLuaMng );
    bool IsInSkillLvRange( IN char cChar, IN int nLv );
    bool LoadMaxCharSPLevel( IN KLuaManager& kLuaMng );
    bool LoadSkillExceptionID( IN KLuaManager& kLuaMng );
    bool LoadExceptionID( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<int>& setExceptionID );
    bool LoadSPXTableInfo( IN KLuaManager& kLuaMng, OUT std::map<int,int>& mapSPXTableInfo );
    bool LoadCharSPXType( IN KLuaManager& kLuaMng );
    // 해당 캐릭터의 Type에 맞는 SPXTable Type을 찾는다.
    bool GetCharSPXTableType( IN const char cChar, OUT int& nCharSPXType );
    // 해당 캐릭터의 전체 SPXTable을 가져온다.
    bool GetCharSPXTable( IN int nSPXType, OUT std::map<int,int>& mapSPXTable );
    bool LoadAddCharSPX( IN KLuaManager& kLuaMng ); // 캐릭터 추가 SPX Table.
    bool LoadExceptionCharTypeList( IN KLuaManager& kLuaMng );
    bool LoadAPCharAddSlotOpenInfo( KLuaManager& kLuaMng );
    bool LoadSetInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<int>& setDungeonInfo );
    bool LoadMissionRewardSkillInfo( KLuaManager& kLuaMng );
    bool LoadAbilitySkillInfo( KLuaManager& kLuaMng );
    bool LoadOldSkillTreeUseCharacter( KLuaManager& kLuaMng_ );
    bool LoadSkillTreeOpenCharInfo( KLuaManager& kLuaMng );
    bool LoadSkillScroolInfo( KLuaManager& kLuaMng );

protected:
    mutable KncCriticalSection      m_csSkills;
    std::map<int,SSkillElement>     m_mapSkills;
    std::map< int, char>            m_mapLockGroups;

    mutable KncCriticalSection      m_csSPXTable;
    std::map< DWORD, std::map<int,int> > m_mapSPXTotalTable;

    mutable KncCriticalSection      m_csConfig;
    SSkillConfig                    m_sSkillConfig;

    mutable KncCriticalSection          m_csMaxTrainSP;
    std::map<std::pair<char,char>, int>  m_mapMaxTrainSP;

    mutable KncCriticalSection          m_csMaxCharSPLevel;
    std::map<std::pair<char,char>, std::pair<int,int>>  m_mapMaxCharSPLevel;

    mutable KncCriticalSection          m_csSkillAddPoint;
    std::map<char, int>                 m_mapSkillAddPoint;

    mutable KncCriticalSection          m_csSkillExceptionID;
    std::map<char, std::set<int>>       m_mapSkillExceptionID;

    mutable KncCriticalSection          m_csCharSPXType;
    std::map<char, int>                 m_mapCharSPXType;

    mutable KncCriticalSection           m_csAddCharSPX;
    std::map<std::pair<char,char>, int>  m_mapAddCharSPX; // 캐릭터 추가 SPX Table

    mutable KncCriticalSection           m_csExceptionCharType;
    std::set<char>                       m_setExceptionCharType;

    mutable KncCriticalSection           m_csAPCharAddSlotOpen;
    int                                  m_nMaxCharSlotNum;
    std::map<GCITEMID, std::set<int>>    m_mapAPCharAddSlotOpenInfo;

    mutable KncCriticalSection           m_csMissionRewardSkill;
    std::map<DWORD, int>                 m_mapSkillMissionInfo; // [MissionID, SkillID] 클라이언트 전달용.
    std::map<DWORD, std::pair<int,DWORD>> m_mapDBUpdateSkillMissionInfo; // [MissionID, SkillID] DB저장용.

    mutable KncCriticalSection           m_csAbilitySkill;
    std::map<char, std::set<int>>        m_mapAbilitySkillInfo; // [CharType, set::set<SkillID>]

    mutable KncCriticalSection           m_csOldSkillUseCharacter;
    std::set<char>                       m_setOldSkillUseCharacter;

    mutable KncCriticalSection           m_csSkillTreeOpenChar;
    std::set<std::pair<std::string, std::string>> m_setSkillTreeOpenCharInfo; // [CharType, CharPromotion]
    GCITEMID                             m_SkillOpenItemID; // 4단필 스킬 오픈 Key ItemID
    std::map<GCITEMID, int>              m_mapSkillScroolInfo; // 스킬 스크롤 정보.[ItemID, SkillID]
};

DefSingletonInline( KSkillManager );
DeclOstmOperatorA( KSkillManager );
