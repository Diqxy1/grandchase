#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include <set>
#include "KncTimer.h"
#include "CnKairo.h"
#include "CnClientContentsInfo.h"
#include "CommonPacket.h"

class KLuaManager;

class KCnUpdatePlanManager
{
    DeclareSingleton( KCnUpdatePlanManager );
    NiDeclareRootRTTI( KCnUpdatePlanManager );
    DeclToStringW;

public:
    struct KICCUpdatePlan
    {
        time_t          m_tmCheckTime;
        std::string     m_strFileName;
    };

    struct KScriptUpdatePlan
    {
        time_t                         m_tmCheckTime;
        std::vector< PAIR_INT_STRING > m_vecScriptInfo;
        bool                           m_bUsed;
    };

    KCnUpdatePlanManager( void );
    ~KCnUpdatePlanManager( void );

    bool LoadScript();
    void Tick();
    bool GetUsingICC( OUT KCnClientContentsInfo& kCnClientContentsInfo_ );
    bool GetScriptUpdateInfo( OUT KECN_PLAN_SCRIPT_UPDATE_NOT& kPacket_ );

    bool SendScriptUpdateInfoNot( IN const KECN_PLAN_SCRIPT_UPDATE_NOT& kPacket_ );
    bool SendCurrentICCFullPacket();
    bool GetCurrentICCFullPacket( OUT KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kPacketCCOIN, OUT KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT& kPacketCCFIIN );
    void SendEventUpdateNot( IN int nEventType );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadICCUpdatePlan( IN KLuaManager& kLuaMng_ );
    bool LoadEventUpdatePlan( IN KLuaManager& kLuaMng_ );
    bool LoadScriptUpdatePlan( IN KLuaManager& kLuaMng_ );
    bool LoadScriptInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< PAIR_INT_STRING >& vecScriptInfo_ );

    void CheckICCUpdatePlan( IN const time_t tmCurrent_ );
    void CheckEventUpdatePlan( IN const time_t tmCurrent_ );
    void CheckScriptUpdatePlan( IN const time_t tmCurrent_ );

    bool SendClientContentsOpenInfoNot( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_ );
    bool CheckDifferenceOfKCCGameCategoryInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCGameCategoryInfo >& prGameCategoryInfo_ );
    bool CheckDifferenceOfKCCGameCharInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCGameCharInfo >& prGameCharInfo_ );
    bool CheckDifferenceOfKCCMonsterInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCMonsterInfo >& prMonsterInfo_ );
    bool CheckDifferenceOfKCCMiniEventnInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCMiniEventnInfo >& prMiniEventInfo_ );

    bool SendClientContentsFirstInitInfoNot( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_ );
    bool CheckDifferenceOfKCCUsingFilesInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCUsingFilesInfo >& prUsingFilesInfo_ );
    bool CheckDifferenceOfKCCHackCheckInfo( IN KCnClientContentsInfo& kUsingICC_, IN KCnClientContentsInfo& kNewICC_, OUT std::pair< std::vector< int >, KCCHackCheckInfo >& prHackCheckInfo_ );

protected:
    KncTimer                                                            m_kUpdateCheckTimer;                // 이벤트 변동이 있는지 확인할 타이머(1분)

    mutable KncCriticalSection                                          m_csICCUpdatePlan;
    std::vector< KICCUpdatePlan >                                       m_vecICCUpdatePlan;                 // 업데이트할 ICC 스크립트 리스트(설정 확인용. 실제 사용할 수 있는 데이터는 m_mapICCInfo에 있다)

    mutable KncCriticalSection                                          m_csICCInfo;
    std::map< int, std::pair< time_t, KCnClientContentsInfo > >         m_mapICCInfo;                       // 설정된 ICC 스크립트들의 정보. 처음에 한번 읽어두고 틱마다 참조한다
    KCnClientContentsInfo                                               m_kUsingICC;                        // 현재 사용중인 ICC 스크립트 정보. 패킷 전달을 위해 참조한다
    int                                                                 m_nLoadedIndex;                     // 현재 사용중인 ICC 스크립트 인덱스

    mutable KncCriticalSection                                          m_csEventUpdatePlan;
    std::vector< KEventUpdatePlan >                                     m_vecEventUpdatePlan;

    mutable KncCriticalSection                                          m_csScriptUpdatePlan;
    std::vector< KScriptUpdatePlan >                                    m_vecScriptUpdatePlan;
    std::map< int, std::string >                                        m_mapScriptInfoForGameServerSend;   // 게임서버 재연결 시 알려줘야 하기에 정보 보관 (중복 방지 위해 map으로)
};

DefSingletonInline( KCnUpdatePlanManager );
DeclOstmOperatorA( KCnUpdatePlanManager );
