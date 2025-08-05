#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "CommonPacket.h"
#include "Kairo.h"
#include <Thread/Locker.h>

struct KSpecialMonInfo
{
    int                         m_nGameModeID; // GameModeID.
    int                         m_nDifficult;  // 난이도.
    int                         m_nMonsterID; // 몬스터ID.
    int                         m_nMonLv; // 몬스터 레벨.
    bool                        m_bPropertyEnable; // 몬스터 속성 사용여부.
    Kairo                       m_kPropertyCount;
    std::vector< Kairo >        m_vecProperty;
};

class KLuaManager;
class KSpecialMonsterManager
{
    DeclareSingleton( KSpecialMonsterManager );
    NiDeclareRootRTTI( KSpecialMonsterManager );
    DeclToStringW;

public:
    KSpecialMonsterManager(void);
    ~KSpecialMonsterManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetSpecialMonsterInfo( IN int nModeID, IN int nDifficult, OUT KSpecialMonInfo& kSpecialMonInfo );
    bool IsActive(){ KLocker lock( m_csSpecialMonster ); return m_bActiveEvent; }

protected:
    bool LoadPropertyInfo( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT bool& bPropertyEnable, OUT Kairo& kPropertyCount, OUT std::vector< Kairo >& vecProperty );

protected:
    mutable KncCriticalSection          m_csSpecialMonster;
    bool                                m_bActiveEvent;
    std::map< std::pair<int,int>, KSpecialMonInfo >    m_mapSpecialMonsterInfo; // ModeID, SpecialMonsterInfo
};

DefSingletonInline( KSpecialMonsterManager );
DeclOstmOperatorA( KSpecialMonsterManager );