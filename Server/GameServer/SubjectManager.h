#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "Lua/KLuaManager.h"
#include "CommonPacket.h"

// 달성 과제 시스템
// 1. 던전 또는 대전에서 주어지는 과제를 달성할 경우 해당 과제에 대한 보상을 지급하는 시스템
// 2. 같은 던전이라도 각 유저별로 과제 완료 패킷을 보냄
// 3. 한 던전에서 같은 트리거ID로 두 번 이상 보상을 받을 수 없음
// 4. 운영자일 경우 치트키 사용시 예외로 위 조건을 무시함(중복완료체크, 트리거ID체크 없음)
// 5. KDungeonSubjectStat 구조체로 통계 남김
// 
// 작업자 :
//    클라 - 이상호
//    디비 - 정기섭
//    기획 - 구동현
//    서버 - 곽재영
//    Comment : 

class KSubjectManager
{
    DeclareSingleton( KSubjectManager );
    NiDeclareRootRTTI( KSubjectManager );
    DeclToStringW;

public:
    struct KSubject {
        DWORD                           m_dwSubjectID;
        int                             m_nAddExp;
        int                             m_nAddGp;
        std::vector< KDropItemInfo >    m_vecRewardItem;

        KSubject() : m_dwSubjectID( 0 ), m_nAddExp( 0 ), m_nAddGp( 0 ) {
            m_vecRewardItem.clear();
        }
    };

    KSubjectManager( void );
    ~KSubjectManager( void );

public:
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetSubject( IN const PAIR_INT& prModeAndDifficulty_, IN const DWORD dwTriggerID_, IN const DWORD dwSubjectID_, OUT KSubject& kSubject_ );
    bool GetSubject( IN const DWORD dwSubjectID_, OUT KSubject& kSubject_ );
private:
    bool LoadSubjectTemplate( OUT std::map< DWORD, KSubject >& mapSubjectList_ );
    bool LoadDropList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewardItem_ );
    bool LoadSubjectInfo( OUT std::map< PAIR_INT, std::map< DWORD, std::set< DWORD > > >& mapSubjectInfo_ );
    bool LoadTriggerAndSubjectInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, std::set< DWORD > >& mapTriggerAndSubjectInfo_ );
    bool LoadSubjectIDInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::set< DWORD >& setSubjectID_ );

protected:
    mutable KncCriticalSection                  m_csSubject;

private:
    std::map< DWORD, KSubject >                 m_mapSubjectList;                   // map< SubjectID, KSubject >
    std::map< PAIR_INT, std::map< DWORD, std::set< DWORD > > > m_mapSubjectInfo;    // map< ( Mode, Difficulty ), map< TriggerID, set< SubjectID > > >
};

DefSingletonInline( KSubjectManager );
DeclOstmOperatorA( KSubjectManager );
