#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "Kairo.h"
#include "UserPacket.h"

class KLuaManager;

class KItemCompose
{
    DeclareSingleton( KItemCompose );
    NiDeclareRootRTTI( KItemCompose );
    DeclToStringW;

public:
    KItemCompose(void);
    ~KItemCompose(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsEventTerm();
    DWORD GetMaxMaterialListNum(){ KLocker lock( m_csItemCompose ); return m_dwMaxMaterialListNum; }
    bool CheckMaterialInfo( IN const GCITEMID& dwItemID );
    bool DoGamble( OUT KDropItemInfo& kDropItemInfo );
    void GetItemComposeInfo( OUT KEVENT_ITEM_COMPOSE_INFO_NOT& kPacket );
    DWORD GetVersion(){ KLocker lock( m_csItemCompose ); return m_dwVersion; }

private:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadMaterialInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<GCITEMID>& vecInfo );
    bool LoadRewardRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo, OUT std::vector<GCITEMID>& vecInfo );

private:
    mutable KncCriticalSection  m_csItemCompose;
    DWORD                       m_dwVersion;       // 버전정보.
    DWORD                       m_dwMaxMaterialListNum;
    time_t                      m_tmBegin;         // 이벤트 시작 날짜.
    time_t                      m_tmEnd;           // 이벤트 종료 날짜.
    std::vector<GCITEMID>       m_vecMaterialList; // 아이템 재료 정보.
    std::vector<GCITEMID>       m_vecRewardList;   // 보상 아이템 정보.
    Kairo                       m_kairoInfo;       // 아이템 확률 정보.
};

DefSingletonInline( KItemCompose );
DeclOstmOperatorA( KItemCompose );
