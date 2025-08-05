#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "CnSocketConsumeItem.h"
#include "KncTimer.h"

// protected 함수들은 가능하면 호출하는데서 락을 걸어 주도록 한다.
class KCnItemManager
{
    DeclareSingleton( KCnItemManager );
    NiDeclareRootRTTI( KCnItemManager );
    DeclToStringW;
public:
    KCnItemManager(void);
    ~KCnItemManager(void);

    KCnSocketConsumeItem m_kSocketConsume;
public:
    void Tick();
    void UpdateBuyInfoFromDB( std::vector<std::pair<DWORD,bool> >& vecBuyInfo );
    bool GetBuyItemInfo( OUT bool& bBuyEnable, OUT std::vector< std::pair<DWORD,DWORD> >& vecBuyItemInfo );

    bool IsBuyItemInfoEnable();
    void SetBuyItemInfoEnable( bool bEnable );

    void UpdateItemRecommInfo( IN std::map<DWORD,unsigned char >& mapRecommInfo );
    void GetItemRecommInfo( OUT std::map<DWORD,unsigned char >& mapRecommInfo );

    bool LoadScript();

    void UpdateBuyLimitItem( IN const std::set<GCITEMID>& setList );
    void GetBuyLimitItem( OUT std::set<GCITEMID>& setList );
    void Send_BuyLimitItemListNot( IN const std::set<GCITEMID>& setList );
    void QueuingCheckDBUp();
    void DumpBuyLimitItem();
    void SendToItemUpdateNot( IN const bool& bUpdate );
    void SendItemBuyInfoNot();

    // 캐시백 정보 관련
    void UpdateCashbackDefaultRatio( IN const std::map< GCITEMID, int >& mapCashbackDefaultRatio_ );
    void UpdateCashbackEventRatio( IN const time_t tmStart_, IN const time_t tmEnd_, IN const std::map< GCITEMID, int >& mapCashbackEventRatio_ );
    void SendCashbackRatioInfo( IN const std::map< GCITEMID, int >& mapCashbackDefaultRatio_, IN const std::map< GCITEMID, int >& mapCashbackEventRatio_ );
    void GetCashbackRatioInfo( OUT KCashbackRatioInfo& kCashbackRatioInfo_ );

    // 게임 서버들에게 패키지 리스트 갱신 알림
    bool SendPackageListChangedNot();

protected:
    void SetBuyItemInfo( IN bool bBuyEnable, IN std::vector< std::pair<DWORD,DWORD> >& vecBuyItemInfo );
    bool IsCheckTime();
    void CheckCashbackEvent();
protected:
    mutable KncCriticalSection  m_csBuyItemInfo;
    bool    m_bBuyItemEnable; // 동작하게 할것인가?
    bool    m_bBuyEnable;
    time_t  m_tmLastUpdateTime;
    std::vector< std::pair<DWORD,DWORD> >   m_vecBuyItemInfo;
    std::map<DWORD,unsigned char >          m_mapRecommInfo;

    mutable KncCriticalSection  m_csTickInfo;
    DWORD   m_dwLastTick;
    DWORD   m_dwTickGap;

    mutable KncCriticalSection  m_csBuyLimitItem;
    std::set<GCITEMID>          m_setBuyLimitItem; // 아이템 구매 제한

    mutable KncCriticalSection  m_csCashbackRatio;
    std::map< GCITEMID, int >   m_mapCashbackDefaultRatio;
    std::map< GCITEMID, int >   m_mapCashbackEventRatio;
    time_t                      m_tmCashbackEventStart;
    time_t                      m_tmCashbackEventEnd;
    bool                        m_bCashbackEventStarted;
    bool                        m_bCashbackEventEnded;
    KncTimer                    m_kCashbackEventTimer;
};
DefSingletonInline( KCnItemManager );
DeclOstmOperatorA( KCnItemManager );
