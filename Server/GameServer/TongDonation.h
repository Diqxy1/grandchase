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

/*
게임 서버의 TongDonation 싱글톤에서 캐싱하는 전체 기부 누적량은 따로 초기화 하지 않는다
개인 누적 기부량은 유저 객체에 저장되므로 재접속을 하면 알아서 초기화된다
개인, 전체 누적 기부량은 둘 모두 버전별로 저장되도록 작성되어 있으며
버전이 변경되었다 하도라도 기존 버전의 캐싱 데이터를 삭제하지 않는다
각 기부 데이터는 기부하는 시점에 DB에 바로 갱신되며, 이때마다 갱신된 DB 데이터를 서버에 캐싱한다
센터 서버에서의 기능은 스크립트 정보를 읽어 게임 서버에 전달해주는 역할만 한다
*/
class KTongDonation
{
    DeclareSingleton( KTongDonation );
    NiDeclareRootRTTI( KTongDonation );
    DeclToStringW;

public:
    KTongDonation( void );
    ~KTongDonation( void );

    bool SetFromTongDonationInfoNot( IN const KECN_TONG_DONATION_INFO_NOT& kPacket_ );
    DWORD GetTotalAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_ );
    void SetTotalAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_, IN const DWORD dwAmount_ );
    bool IsEventTerm();
    bool GetTongDonationNPCInfo( IN const DWORD dwNPCType_, OUT KTongDonationNPCInfo& kTongDonationNPCInfo_ );
    DWORD GetClientUIType();
    time_t GetCurrentVersion();
    int GetEventType( IN const DWORD dwNPCType_ );
    bool GetNPCTypes( OUT std::vector< DWORD >& vecNPCType_ );

private:
    void InitEventType();

private:
    mutable KncCriticalSection              m_csTongDonation;

    time_t                                  m_tmVersion;                // 버전으로 사용할 시간
    time_t                                  m_tmBegin;                  // 기부 시작 날짜
    time_t                                  m_tmEnd;                    // 기부 종료 날짜
    DWORD                                   m_dwClientUIType;           // 클라이언트 UI 타입
    std::map< DWORD, KTongDonationNPCInfo > m_mapNPCs;                  // 통합 기부 NPC 정보( key: NPCType )

    std::map< time_t, std::map< DWORD, DWORD > > m_mapTotalAmount;      // NPC별 전체 기부 누적량( key: tmVersion, < NPCType, Amount > )

    std::map< DWORD, int >                  m_mapEventType;
};

DefSingletonInline( KTongDonation );
DeclOstmOperatorA( KTongDonation );
