#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <set>
#include <vector>
#include "CommonPacket.h"

class KLuaManager;

class KCharDefault
{
    DeclareSingleton( KCharDefault );
    NiDeclareRootRTTI( KCharDefault );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo> VEC_REWARD;

    KCharDefault(void);
    ~KCharDefault(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool CheckCharDungeonInfo( IN const char cChar, OUT std::set<int>& setDungeonInfo );
    void GetDefaultItemInfo( IN const char& cCharType, OUT VEC_REWARD& vecItemInfo );
    bool CheckCharSkillInfo( IN const char cChar, OUT std::set<int>& setSkillInfo );
    void GetDefaultSlotItemInfo( IN const char& cCharType_, IN OUT VEC_REWARD& vecItemInfo_ );
    bool CheckCharCreateEnable( IN const char cChar, OUT DWORD& dwLevel );

protected:
    bool LoadDefaultDengeon( IN KLuaManager& kLuaMng, OUT std::map< char, std::set<int> >& mapCharDefaultDungeon );
    bool LoadSetInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<int>& setInfo );
    bool LoadDefaultItemInfo( IN KLuaManager& kLuaMng, OUT std::map< char, VEC_REWARD >& mapCharDefaultItemInfo );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadDefaultSkill( IN KLuaManager& kLuaMng, OUT std::map< char, std::set<int> >& mapCharDefaultSkill );
    bool LoadDefaultSlotItemInfo( IN KLuaManager& kLuaMng_, OUT std::map< char, VEC_REWARD >& mapCharSlotItemInfo_ );
    bool LoadCreateLimitLevel( IN KLuaManager& kLuaMng, OUT std::map< char, DWORD >& mapCharDefaultDungeon );

protected:
    mutable KncCriticalSection          m_csCharDefault;
    std::map< char, std::set<int> >     m_mapCharDefaultDungeon;
    std::map< char, VEC_REWARD > m_mapCharDefaultItemInfo;
    std::map< char, std::set<int> >     m_mapCharDefaultSkill;
    std::map< char, VEC_REWARD > m_mapCharDefaultSlotItemInfo;
    std::map< char, DWORD > m_mapCharCreateLimitLevel;
};

DefSingletonInline( KCharDefault );
DeclOstmOperatorA( KCharDefault );
