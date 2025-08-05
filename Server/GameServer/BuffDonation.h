#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "PrimitiveTypedef.h"
#include "UserPacket.h"

class KLuaManager;

// 발렌타인 때 사용 된 기부 이벤트
// 기존의 기부와 다른 점은 기부 시에 보상 아이템을 지급 해주고,
// 전체 서버의 기부 수량이 일정 수치가 되면 버프를 걸어 준다 (클라에서)
// 버프는 스크립트 설정 된 시간만큼 유지 되며
// 버프 발동 시 전체 기부 수량을 깎는다
// 작업자 :
//    서버 - 손기운
//    클라 - 이재준
//    기획 - 구동현
//    Comment : 기획쪽에서 기부 대상과 기부하는 아이템은 무조건 하나라는 확답을 얻음
//              기부데이터 DB접근은 센터에서만 처리

class KBuffDonation
{
    DeclareSingleton( KBuffDonation );
    NiDeclareRootRTTI( KBuffDonation );
    DeclToStringW;
public:
    KBuffDonation(void);
    ~KBuffDonation(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsEventTerm();
    bool IsUseIndividualBuffSystem();
    void GetBeginTime( OUT time_t& tmDate );
    void GetDonationDataForFirstLogin( OUT KEVENT_BUFF_DONATION_LOGIN_INFO_NOT& kPacket );
    bool CheckDonationItemValidation( IN const GCITEMUID& dwItemID );
    void GetRewardItemList( OUT VEC_REWARD& vecReward );
    void GetDonationTotalData( OUT KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kPacket );
    void GetTotalDonationPoint( OUT __int64& biTotalPoint_ );
    void UpdateTotalDonationPointFromCenter( IN const KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kPacket );
    void NotifyDonationInfoToAllUser( IN const KEVENT_BUFF_DONATION_TOTAL_DATA_NOT& kPacket );
    void UpdateDonationPointDiffReqToCenter( IN const int& nDonationPointDiff );
    void GetType( OUT DWORD& dwType ) { KLocker lock( m_csBuffDonation ); dwType = m_dwType; };
    int GetBuffFlag() { KLocker lock( m_csBuffDonation ); return m_nBuffStateFlag; };

    void NotifyBuffStartMsgFromCenter();

private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadRewardItem( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    std::wstring GetBuffNotString( IN std::wstring strKey );

protected:
    mutable KncCriticalSection  m_csBuffDonation;
    time_t                      m_tmBegin;                  // 기부 시작 날짜
    time_t                      m_tmEnd;                    // 기부 종료 날짜

    GCITEMID                    m_dwDonationItem;           // 기부에 사용 되는 아이템
    VEC_REWARD                  m_vecRewardItem;            // 기부 하고 교환 받는 아이템 리스트
    __int64                     m_biActiveBuffPoint;        // 버프 걸어주는 기준치(클라 전달용으로 가지고 있음)
    int                         m_nRecoveryChance;          // 스킬 사용 시 HP/MP회복 확률 0~100 (클라 전달용 데이터)
    DWORD                       m_dwType;                   // 데이터 타입 (센터랑 동일한거)
    bool                        m_bUseIndividualBuffSystem; // 개별 버프 시스템 사용 여부 (기부자 한정 버프인지 전체인지)

    __int64                     m_biTotalDonationPoint;     // 센터에서 얻는 전체 기부 수량
    int                         m_nBuffStateFlag;           // 현재 버프 상태

    std::wstring                m_strBuffStartNotifyMsg;    // 버프 발동 시 공지 하는 문자열
};

DefSingletonInline( KBuffDonation );
DeclOstmOperatorA( KBuffDonation );