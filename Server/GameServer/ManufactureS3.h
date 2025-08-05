#pragma once
#include <vector>
#include <map>
#include <set>
#include "PrimitiveTypedef.h"
#include <Thread/Locker.h>
#include <RTTI.H>
#include <ToString.h>
#include <Serializer/SerBuffer.h>

/*
광장 진입시 유저에게 아이템 목록(가격 포함) 전다.
해당 목록 선택시 재료 목록을 전달 해준다.

*/
struct KManufactureItem;
struct KDropItemInfo;
class KLuaManager;

class KManufactureS3
{
public:
    KManufactureS3(void);
    ~KManufactureS3(void);
    bool LoadScript( IN const std::string& strScript );
    bool GetCatalog( IN const PAIR_USHORT_DWORD& prKey, OUT KManufactureItem& kCatalogs );
    bool GetCatalog( IN const PAIR_USHORT_PAIR_DWORD_INT& prKey, OUT KManufactureItem& kCatalogs );
    bool GetCatalogs( OUT KSerBuffer& buffEvent );
    bool GetCatalogs( OUT std::map< PAIR_USHORT_DWORD, KManufactureItem >& mapList );
    bool GetCatalogs( OUT std::map<int, std::vector<KManufactureItem> >& mapList );
    bool GetCatalogByItemID( IN const GCITEMID& ItemID, OUT KManufactureItem& kCatalogs );
    bool GetMaterials( IN const PAIR_USHORT_DWORD& prKey, OUT std::vector<KDropItemInfo>& vecMaterials );
    bool GetMaterials( IN const PAIR_USHORT_PAIR_DWORD_INT& prKey, OUT std::vector<KDropItemInfo>& vecMaterials );
    void GetMaterialList( IN const std::set< PAIR_USHORT_DWORD >& setRequest, OUT std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterialList );
    void GetMaterialList( OUT std::map< PAIR_USHORT_DWORD,std::vector<KDropItemInfo> >& mapMaterialList );
    bool GetTabIDs( OUT std::vector<int>& vecList );

private:
    bool LoadCatalog( IN OUT KLuaManager& kLuaMng, OUT KManufactureItem& kCatalog );
    bool LoadMaterials( IN OUT KLuaManager& kLuaMng, OUT std::vector<KDropItemInfo>& vecMaterial );
    void LoadLookItem( IN OUT KLuaManager& kLuaMng, IN OUT KManufactureItem& kItem );
    void LoadCategory( IN OUT KLuaManager& kLuaMng, IN OUT KManufactureItem& kItem );
    int GetCostFromSocket( IN const GCITEMID ItemID );
    bool BuildPrice();
    void SetTabItemList( IN const KManufactureItem& kCatalog, IN OUT std::map<int, std::vector<KManufactureItem> >& mapList );
    DWORD GetContinentsFlag( IN const int nContinents_ );
    bool LoadContinents( IN OUT KLuaManager& kLuaMng_, OUT KManufactureItem& kItem_ );

private:
    KncCriticalSection                                        m_csManufacture;
    std::map< PAIR_USHORT_DWORD, KManufactureItem >           m_mapItemList; // map< key, ItemInfo >
    KSerBuffer                                                m_buffClientCatalog;
    std::map< PAIR_USHORT_DWORD, std::vector<KDropItemInfo> > m_mapMaterialList; // map< key, vector< Material > >
    std::map< int, std::vector<KManufactureItem> >            m_mapTabItemList; // map< TabID, ItemInfo >
};
