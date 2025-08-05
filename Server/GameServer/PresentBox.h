#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <set>
#include <vector>
#include <map>
#include "Kairo.h"

class KLuaManager;
struct KItemUnit;

class KPresentBox
{
    DeclareSingleton( KPresentBox );
    NiDeclareRootRTTI( KPresentBox );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo> VEC_REWARD;

    KPresentBox(void);
    ~KPresentBox(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    bool IsRun(); // 선물상자 이벤트 진행중인지 체크.
    bool GetTradeItem( IN const GCITEMID dwItemID, OUT VEC_REWARD& vecTradeInfo );
    void GetTradeItemList( OUT std::map<DWORD, VEC_REWARD>& mapTradeList );
    void GetPresentBoxItemList( OUT std::set<GCITEMID>& setItemList );
    void GetRewardItemList( OUT VEC_REWARD& vecRewardInfo );
    bool DoGamble( OUT KDropItemInfo& kItem );
    DWORD GetVersion(){ KLocker lock( m_csPresentBox ); return m_dwVersion; }; 

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadPresentBox( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<GCITEMID>& setPresentBoxInfo );
    bool LoadTradeItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD, VEC_REWARD>& mapTradeList );
    bool LoadRewardItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardList, OUT Kairo& kairo );

protected:
    mutable KncCriticalSection          m_csPresentBox;
    time_t                              m_tmBegin;           // 선물상자 이벤트 시작 날짜.
    time_t                              m_tmEnd;             // 선물상자 이벤트 종료 날짜.
    DWORD                               m_dwPresentBoxItem;  // 선물상자 이벤트 UI 아이템.
    DWORD                               m_dwVersion;         // 이벤트 버전 정보.
    std::set<GCITEMID>                  m_setPresentItemList;// 선물상자 조각 아이템 리스트.
    std::map< DWORD, VEC_REWARD >       m_mapTradeItemList;  // 조각 아이템 교환 리스트.
    VEC_REWARD                          m_vecRewardItemList; // 보상 아이템 리스트.
    Kairo                               m_kairoReward;       // 보상 아이템 리스트의 확률정보.
};

DefSingletonInline( KPresentBox );
DeclOstmOperatorA( KPresentBox );