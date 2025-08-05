#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "PrimitiveTypedef.h"

class KLuaManager;

struct KGetLimitInfo
{
    GCITEMID            m_dwItemID;
    DWORD               m_dwLimit;
};

class KGetLimit
{
    DeclareSingleton( KGetLimit );
    NiDeclareRootRTTI( KGetLimit );
    DeclToStringW;

public:
    KGetLimit(void);
    ~KGetLimit(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsRun();
    bool GetGetLimitInfo( IN PAIR_INT& prKey_, OUT std::vector< KGetLimitInfo >& vecGetLimitInfo_ );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadGetLimit( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< PAIR_INT, std::vector< KGetLimitInfo > >& mapGetLimitInfo_ );
    bool LoadCheckItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KGetLimitInfo >& vecGetLimitInfo_ );

protected:
    mutable KncCriticalSection              m_csGetLimit;
    time_t                                  m_tmBegin;           // 이벤트 시작 날짜.
    time_t                                  m_tmEnd;             // 이벤트 종료 날짜.

    std::map< PAIR_INT, std::vector< KGetLimitInfo > >      m_mapGetLimitInfo;  // Key: < ModeID, Difficulty >
};

DefSingletonInline( KGetLimit );
DeclOstmOperatorA( KGetLimit );
