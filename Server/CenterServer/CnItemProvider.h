#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>

// 서버 다운 없이 보상 아이템 실시간 지급 객체
// 현재는 공동구매를 위해서 사용하나,
// 추후 다른 데서도 사용할 수 있게 고려 하여 제작
// 1. 아이템 지급을 요청할 쪽에서 ItemProvider에서 처리 중인 데이터 없는지 확인 (생략해도 2번에서 걸러냄)
// 2. 정해진 데이터로 아이템 지급 함수를 호출
// 3. 아이템을 지급 하는 동안 해당 객체에 지급 요청 불가능
// 4. Tick에서 주기적으로 특정 인원수만큼 지급
// 5. DB Thread로 지급 요청을 하면 DB Thread에서 아이템 지급 후 요청자 별로 처리 추가 (요청자별 처리 필요없으면 생략)
// 6. 아이템 지급이 종료 되면 지급을 요청한 곳으로 알려주는 핸들러에 처리 추가 (필요없으면 생략)
// 7. Tick에서 지급정보가 비어있다면 지급 완료 절차 밟음
// 끝

// 아이템 지급 추가할 때 여기에 enum 추가
enum EProvideFromWhat {
    FROM_SC_GROUP_PURCHASE = 1,
};

class KCnItemProvider
{
    DeclareSingleton( KCnItemProvider );
    NiDeclareRootRTTI( KCnItemProvider );
    DeclToStringW;
public:
    KCnItemProvider(void);
    ~KCnItemProvider(void);

    bool LoadScript();
    void Tick();

    bool IsLock() { KLocker lock( m_csItemProvider ); return m_bLock; };
    bool ItemProvideReq( IN const VEC_FOR_REWARD_PROVIDE& vecProvideInfo, IN const int& nFromWhat );
    void ItemProvideCompleteHandler();
    void GetFromWhat( OUT int& nFrom ){ KLocker lock( m_csItemProvider ); nFrom = m_nFrom; };
    void ProvideCompleteProcedure( IN VEC_FOR_REWARD_PROVIDE& vecRemove );

protected:
    void SetLock(){ KLocker lock( m_csItemProvider ); m_bLock = true; };
    void ReleaseLock(){ KLocker lock( m_csItemProvider ); m_bLock = false; };
private:
    mutable KncCriticalSection  m_csItemProvider;

    KncTimer m_kProvideTimer;
    VEC_FOR_REWARD_PROVIDE m_vecProvideInfo;
    int      m_nOneTimeProvisionUserCount; // 한번에 지급 처리 할 유저수
    int      m_nFrom;                      // 어떤 객체가 아이템 지급을 요청했나
    bool     m_bLock;                      // 아이템 지급 중일 때는 요청 처리 무시
};

DefSingletonInline( KCnItemProvider );
DeclOstmOperatorA( KCnItemProvider );