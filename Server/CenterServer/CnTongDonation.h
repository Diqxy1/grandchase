#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "CenterPacket.h"
#include "UserPacket.h"

class KLuaManager;

class KCnTongDonation
{
    DeclareSingleton( KCnTongDonation );
    NiDeclareRootRTTI( KCnTongDonation );
    DeclToStringW;

public:
    KCnTongDonation( void );
    ~KCnTongDonation( void );

    bool LoadScript();
    bool GetTongDonationInfoNot( OUT KECN_TONG_DONATION_INFO_NOT& kPacket_ );

private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadNPCInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< DWORD, KTongDonationNPCInfo >& mapNPCs_ );
    bool LoadDonationReward( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KItem >& vecDonationRewards_ );
    bool LoadGoalInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KTongDonationGoalInfo >& vecGoalInfo_ );

private:
    mutable KncCriticalSection              m_csCnTongDonation;

    time_t                                  m_tmVersion;                // 버전으로 사용할 시간
    time_t                                  m_tmBegin;                  // 기부 시작 날짜
    time_t                                  m_tmEnd;                    // 기부 종료 날짜
    DWORD                                   m_dwClientUIType;           // 클라이언트 UI 타입
    std::map< DWORD, KTongDonationNPCInfo > m_mapNPCs;                  // 통합 기부 NPC 정보( key: NPCType )
};

DefSingletonInline( KCnTongDonation );
DeclOstmOperatorA( KCnTongDonation );
