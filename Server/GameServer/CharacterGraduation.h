#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <set>
#include <vector>
#include "Lua/KLuaManager.h"
#include "CommonPacket.h"

class KCharacterGraduation
{
    DeclareSingleton( KCharacterGraduation );
    NiDeclareRootRTTI( KCharacterGraduation );
    DeclToStringW;

public:
    struct KGraduation {
        int m_nCharType; // 캐릭터 타입
        std::map< int, std::set< int > >    m_mapConditionModes; // 졸업조건 모드정보 map< ModeID, set< Difficulty > >
        std::set< int >                     m_setClearModes; // 졸업시 자동 클리어되는 모드들
        std::vector< KDropItemInfo >        m_vecRewards; // 졸업시 지급하는 보상 아이템들

        KGraduation() : m_nCharType( -1 ) {
            m_mapConditionModes.clear();
            m_setClearModes.clear();
            m_vecRewards.clear();
        }
    };

    KCharacterGraduation( void );
    ~KCharacterGraduation( void );

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetGraduation( OUT std::map< int, KGraduation >& mapGraduation_ );

private:
    bool LoadCondition( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< int, std::set< int > >& mapConditionModes_ );
    bool LoadDifficulty( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< int >& setDifficulty_ );
    bool LoadClearModes( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< int >& setClearModes_ );
    bool LoadRewards( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewards_ );
protected:
    mutable KncCriticalSection              m_csCharacterGraduation;

    std::map< int, KGraduation >            m_mapGraduation; // < 캐릭터ID, 졸업정보 >

};

DefSingletonInline( KCharacterGraduation );
DeclOstmOperatorA( KCharacterGraduation );
