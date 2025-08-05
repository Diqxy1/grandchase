#pragma once
#include <map>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "PrimitiveTypedef.h"

class KLuaManager;
struct KDropItemInfo;

struct KItemTradeRewardInfo
{
    int                     m_nAFIndex;             // After 아이템 번호
    int                     m_nAFCount;             // After 지급 아이템 갯수
};

class KItemTradeManager
{
    DeclareSingleton( KItemTradeManager );
    NiDeclareRootRTTI( KItemTradeManager );
    DeclToStringW;
public:
    KItemTradeManager(void);
    ~KItemTradeManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetItemTradeEnable(){ KLocker lock(m_csItemTrade); return m_bItemTradeEnable; }
    void GetItemTradeListNotice( OUT std::map< std::pair<DWORD,int>, KDropItemInfo >& mapItemTradeInfo, OUT std::map<DWORD, KDropItemInfo>& mapItemAllTradeInfo );
    bool GetItemTradeInfo( IN const GCITEMID dwItemID, IN const int nItemCount, OUT KDropItemInfo& nRewardItem );  // 갯수 아이템 교환 정보 가져오기.
    bool GetItemAllTradeInfo( IN const GCITEMID dwItemID, OUT KDropItemInfo& dwRewardItemID ); // 전체 아이템 교환 정보 가져오기.
    void DumpCapsule(); // 전체 아이템 교환 정보 출력

private:
    bool LoadItemTradeData( IN KLuaManager& kLuaMng_, IN std::string& strTable, OUT std::map< std::pair<DWORD, int>, KDropItemInfo >& mapTradeInfo );
    bool LoadItemAllTradeData( IN KLuaManager& kLuaMng_, IN std::string& strTable, OUT std::map< DWORD, KDropItemInfo >& mapAllTradeInfo );

protected:
    mutable KncCriticalSection  m_csItemTrade;
    bool                        m_bItemTradeEnable;         // 아이템교환 사용여부.
    std::map< std::pair<DWORD, int>, KDropItemInfo > m_mapTradeInfo; // 교환 아이템 리스트 정보.
    std::map< DWORD, KDropItemInfo > m_mapAllTradeInfo;             // 전체 교환 아이템 리스트 정보.
};

DefSingletonInline( KItemTradeManager );
DeclOstmOperatorA( KItemTradeManager );