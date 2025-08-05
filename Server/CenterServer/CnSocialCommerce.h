#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>

// Center에서는 DB 설정 정보만 얻는다
// 유저들의 구매 수량은 게임서버에서 각자 얻도록
// 구매 수량 게임서버에서 업데이트 후 센터로 보내서
// 전체 서버 동기화 해줌

class KCnSocialCommerce
{
    DeclareSingleton( KCnSocialCommerce );
    NiDeclareRootRTTI( KCnSocialCommerce );
    DeclToStringW;
public:
    KCnSocialCommerce(void);
    ~KCnSocialCommerce(void);

    bool LoadScript();
    void Tick();
    void CheckChangeDBSettingDataTick();
    void UpdateAllListTick();
    void GetRewardUserListTick();
    bool CheckChange();

    void GetSocialCommerceSettingData( OUT KECN_SOCIAL_COMMERCE_INIT_DATA_NOT& kPacket );
    void UpdateSocialCommerceSettingData( IN const std::map<int, KInformationDataOfSellType>& mapInfoOfSellType, IN bool& bChanged );
    void GetGroupPurchaseSuccessInfo( OUT DWORD& dwEventUID_, OUT int& nGetType );
    int GetRewardUserCount() { KLocker lock( m_csSocialCommerce ); return m_nGetUserCount; };
    bool GetRewardListByEventUID( IN const DWORD& dwEventUID, OUT VEC_REWARD& vecReward );
    void SetGroupPurchaseRewardInfo( IN VEC_GROUP_PURCHASE_REWARD& vecInfo );
    void GetGroupPurchaseRewardInfo( OUT VEC_GROUP_PURCHASE_REWARD& vecInfo );

private:
    mutable KncCriticalSection  m_csSocialCommerce;

    KncTimer                                  m_kUpdateCheckTimer;   // 이벤트 변동이 있는지 확인할 타이머(1분)
    KncTimer                                  m_kListUpdateTimer;    // 공동구매 달성 여부 판별용 전체 리스트 갱신 타이머
    KncTimer                                  m_kRewardListGetTimer; // 공동구매 달성 아이템에 대해 달성 유저 정보 갱신 타이머
    std::map<int, KInformationDataOfSellType> m_mapInfoOfSellType;   // 타입별 전체 판매 정보
    VEC_GROUP_PURCHASE_REWARD                 m_vecRewardInfo;       // 공동구매 보상 유저/아이템 정보
    int                                       m_nGetUserCount;       // 한번에 DB에서 얻어 올 공동구매 유저 수
    bool                                      m_bChanged;
};

// 데이터 전체 갱신 tick
// db에 요청 하는 tick
// 지급 완료가 아닐 때 계속 sp 호출하게 요청하는데,
// 무조건 얻어서 ItemProvider에 전달

DefSingletonInline( KCnSocialCommerce );
DeclOstmOperatorA( KCnSocialCommerce );