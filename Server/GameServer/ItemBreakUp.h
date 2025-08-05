#pragma once
#include "UserEvent.h"
#include "PrimitiveTypedef.h"
#include <Thread/Locker.h>
#include <vector>
#include <set>
#include <map>
#include "UserPacket.h"

struct KDropItemInfo;
struct KItem;
class KLuaManager;
class KItemBreakUp
{
public:
    enum {
        MATERIAL_MAX = 3,
    };

    KItemBreakUp(void);
    ~KItemBreakUp(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool CheckBreakUpEnable( IN const KItem& kItem );
    bool BreakUpItem( IN const KItem& kItem, OUT std::vector<KDropItemInfo>& vecDropMaterial );
    void SetExceptListFromDB( IN OUT std::set<GCITEMID>& setExceptList );
    float GetPriceRatio() { KLocker locm(m_csBreakUp); return m_fPriceRatio; }

    void DumpMaterialList();
    void DumpMaterialInfo( IN const int nItemLv, IN const int nItemGrade );

    void GetItemBreakupInfoAll( OUT KEVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK& kPacket_ );

private:
    bool LoadMaterialItemID( IN OUT KLuaManager& kLuaMng, OUT std::map<int,GCITEMID>& mapMaterialItemID );
    void LoadCountRatio( IN OUT KLuaManager& kLuaMng, OUT std::map<int,int>& mapCountRatio );
    bool LoadMaterialInfo( IN OUT KLuaManager& kLuaMng, OUT std::map<PAIR_INT, std::map<int,int> >& mapMaterial );

    bool IsInExceptList( IN const GCITEMID iteID );
    bool GetMaterialDrop( IN const int& nItemEquipLv, IN const int& nItemGrade, OUT std::vector<KDropItemInfo>& vecDropMaterial );
    GCITEMID GetMaterialItemID( IN const int& nIndex );
    float GetRandomNum( IN const float& fMin, IN const float& fMax );

    bool LoadSpecificBreakupInfo( IN KLuaManager& kLuaMng_, OUT std::map< PAIR_INT, std::vector< KDropItemInfo > >& mapSpecificBreakupInfo_ );
    bool GetSpecificDrop( IN const int& nItemID_, IN const int& nItemGrade_, OUT std::vector< KDropItemInfo >& vecDropMaterial_ );

private:
    KncCriticalSection      m_csBreakUp;
    float                   m_fPriceRatio; // 가격 배율
    float                   m_fMaterialRatioMin; // 재료아이템수 연산시 최소값
    float                   m_fMaterialRatioMax; // 재료아이템수 연산시 최대값
    std::set<GCITEMID>      m_setExceptList; // 아이템 해체시 제외 리스트(DB에서 받아온다)
    std::map<int, GCITEMID> m_mapMaterialItemID; // 재료아이템 [Index,ItemID]
    std::map< PAIR_INT, std::map<int,int> >  m_mapMaterialInfo; // [장착Lv,Grade], [재료Index,Count]

    std::map< PAIR_INT, std::vector< KDropItemInfo > >  m_mapSpecificBreakupInfo; // [입력 아이템ID, Grade], [결과 아이템들]
};
