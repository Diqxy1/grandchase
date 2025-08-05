#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "PrimitiveTypedef.h"
#include "UserPacket.h"

// 보부상 이벤트
// 기존의 캐시 구매 로직을 사용
// 공동구매 보상 아이템의 경우 센터쪽에서 아이템 지급 싱글턴을 이용해서 순차적 지급
// 전체 데이터를 DB에 설정하고 센터에서 얻어 온다
// 구매 기간이 종료 되고 클라이언트에서 이벤트 정보 전달 닫는 것은 InitClientContents.lua에서 한다
// (기간 종료 되고 나서도 일정 기간 동안 공동구매 성공 여부를 UI에 노출 한다)
// 작업자 :
//    서버 - 손기운
//    클라 - 이재준
//    기획 - 구동현
//    Comment : 1. User5, user7에서 캐시 구매 성공 후 소셜커머스 아이템 구매 기록한다
//              2. 각 빌링 스레드에서 ACK패킷에 전달 할 EventUID설정 해줌
//              3. centerproxy에서 db설정 데이터를 얻고 나면 현재 구매 수량을 gsdbthread에서 받아서 갱신한다
//              4. 구매 수량, 이벤트 기간 종료는 게임서버에서 해결
//              5. 게임서버쪽에서 변경한 데이터를 center쪽에서는 일정 주기로 퍼온다
//              6. 구매 요청할 때 마다 DB에서 수량 새로 퍼옴
//              7. DB에서는 SellFlag, GroupPurchaseFlag 에 대한 고려 없이 하나의 flag로 구분 하기 때문에,
//                  게임서버에서 기간에 따라 변경 되는 SellFlag값을 하나 더 가진다
//              8. 테스트를 위해서 가상캐쉬 구매를 통해 소셜커머스 참여 가능하도록 작업 해 둠(EVENT_BUY_VIRTUAL_CASH_ACK)

class KLuaManager;
class KSocialCommerce
{
    DeclareSingleton( KSocialCommerce );
    NiDeclareRootRTTI( KSocialCommerce );
    DeclToStringW;
public:
    KSocialCommerce(void);
    ~KSocialCommerce(void);

    void Tick();
    bool IsRun() { KLocker lock( m_csSocialCommerce ); return m_bRun; }; // 하나라도 진행 중인 이벤트가 있는지
    bool GetSocialCommerceItemInfoFromCurrentBuyItem( IN const DWORD& dwEventUID, OUT KDB_EVENT_SOSCIAL_COMMERCE_BUY_REQ& kPacket );
    void UpdateAllSellInfo();
    bool GetSellInfoByEventUID( IN const DWORD dwEventUID, OUT std::map<DWORD, KSCSellInfo>::iterator& mitSell, OUT int& nType_ );
    void GetSocialCommerceData( OUT std::map<int, KInformationDataOfSellType>& mapInfoOfSellType );
    void UpdateSocialCommerceDataFromCenter( IN const std::map<int, KInformationDataOfSellType>& mapInfoOfSellType );
    void UpdateAllSellCountFromDB( IN std::vector<PAIR_DWORD_INT>& vecPrBuyCount_ );
    void UpdateSellCountDiff( IN const DWORD& dwEventUID, IN const int& nDiff );
    void SendSellCountUpdateReqToDB( IN const DWORD& dwUID_, IN const KDB_EVENT_SOSCIAL_COMMERCE_BUY_REQ& kPacket );
    void SendFlagUpdateReqToDB( IN const DWORD& dwEventUID, IN const int& nGroupPurchaseFlag );
    void UpdateSellCountAtBuyCashItem( IN const DWORD& dwEventUID, IN const int& nCurSellCount );
    bool CheckSellEnable( IN const DWORD& dwEventUID );

private:
    bool ChkOneMinTick();
    void UpdateFlagByPeriod( IN std::map<DWORD, KSCSellInfo>::iterator& mitSell, IN const int& nType );
    void UpdateFlagByDuration( IN std::map<DWORD, KSCSellInfo>::iterator& mitSell, IN const int& nType );

protected:
    mutable KncCriticalSection  m_csSocialCommerce;

    //     <판매Type, 타입별 판매 정보 컨테이너>
    std::map<int, KInformationDataOfSellType> m_mapInfoOfSellType; // 타입별 전체 판매 정보
    DWORD                                     m_dwLastTick;
    DWORD                                     m_dwTickGap;
    bool                                      m_bRun;              // 하나라도 진행 중인 판매가 있는지 체크 하는 변수
};

DefSingletonInline( KSocialCommerce );
DeclOstmOperatorA( KSocialCommerce );