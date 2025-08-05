#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "CommonPacket.h"
#include "CenterPacket.h"

class KLuaManager;

class KUpdatePlanManager
{
    DeclareSingleton( KUpdatePlanManager );
    NiDeclareRootRTTI( KUpdatePlanManager );
    DeclToStringW;

public:
    KUpdatePlanManager( void );
    ~KUpdatePlanManager( void );

    bool UpdateClientContentsOpenInfoNotFromCenter( IN KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT& kPacket_ );
    bool UpdateClientContentsFirstInitInfoNotFromCenter( IN KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT& kPacket_ );

    bool UpdateItemBuyInfoNotFromCenter( IN KItemBuyInfo& kPacket_ );

    bool UpdateEventStateListFromCenter( IN KEVENT_UPDATE_EVENT_STATE_NOT& kPacket_ );

    bool UpdateScriptFilesFromCenter( IN KECN_PLAN_SCRIPT_UPDATE_NOT& kPacket_, OUT std::vector< int >& vecEventID_ );

protected:
    mutable KncCriticalSection                      m_csICCUpdatePlan;

    KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT            m_kEventClientContentsOpenInfoNot;
    KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT      m_kEventClientContentsFirstInitInfoNot;

    mutable KncCriticalSection                      m_csItemBuyInfo;
    KItemBuyInfo                                    m_kItemBuyInfo;

    mutable KncCriticalSection                      m_csEventUpdatePlan;
    mutable KncCriticalSection                      m_csScriptUpdatePlan;
    // 드랍스크립트 패킷도 멤버로...

    std::vector<KEventUpdatePlan>                   m_vecEventUpdatePlan;
};

DefSingletonInline( KUpdatePlanManager );
DeclOstmOperatorA( KUpdatePlanManager );
