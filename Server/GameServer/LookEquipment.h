#pragma once
#include "PrimitiveTypedef.h"
#include <set>
#include <vector>
#include <map>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef SWAP
#define SWAP
#endif

struct KCoordiInfo; // 전방 선언.
struct KCoordiDiffInfo;

class KLuaManager;
class KLookEquipChar
{
public:
    KLookEquipChar(void);
    ~KLookEquipChar(void);

    enum LOOK_COORDI{
        LC_COORDI_0,
        LC_COORDI_1,
        LC_COORDI_2,
        LC_COORDI_3,

        LC_COORDI_MAX,
    };

public:
    void Clear();
    bool EquipItem( IN const int nCoordiID_, IN const GCITEMUID ItemUID_ );
    bool EquipItem( IN const int nCoordiID_, IN const std::vector<GCITEMUID>& vecItemUID_ );
    bool EquipItem( IN const int nCoordiID_, IN const std::set<GCITEMUID>& setItemUID_ );
    bool UnEquipItem( IN const int nCoordiID_, IN const GCITEMUID ItemUID_ );
    bool UnEquipItem( IN const int nCoordiID_, IN const std::vector<GCITEMUID>& vecItemUID_ );
    bool UnEquipItem( IN const int nCoordiID_, IN const std::set<GCITEMUID>& setItemUID_ );
    bool UnAllEquipItem();
    
    bool ForceUnEquipItem( IN const GCITEMUID ItemUID_ );
    bool ForceUnEquipItem( IN const std::vector<GCITEMUID>& vecItemUID_ );
    void SetCharacter( IN const char cCharType_ );
    char GetCharacter();
    bool GetCoordiInfo( IN const int nCoordiID_, OUT KCoordiInfo& kCoordi_ );
    bool GetCurrentCoordiInfo( OUT KCoordiInfo& kCoordi_ );
    bool GetFullCoordiInfo( OUT std::map<int,KCoordiInfo>& mapFullCoordi_ );
    void SetFullCoordiInfo( IN const std::map<int,KCoordiInfo>& mapFullCoordiInfo_ );
    void SetCoordiInfo( IN const int nCoordiID_, IN const KCoordiInfo& kCoordiInfo_ );
    void GetDiffCoordiInfo( OUT std::map<int,KCoordiDiffInfo>& mapCoordiInfo_ );
    bool IsEquiped( IN const GCITEMUID itemUID_ );
    bool IsEquiped( IN const int nCoordiID_, IN const GCITEMUID itemUID_ );
    bool VerifySaveCoordiID( IN const int nCoordiID_ );
    bool SaveCoordi( IN const KCoordiInfo& kCoordiInfo_ );

private:
    bool VerifyCoordiID( IN const int nCoordiID_ );
    bool UnEquipItem( IN OUT std::set<GCITEMUID>& setEquip_, IN const GCITEMUID ItemUID_ );
    void GetDiffCoordiInfo( IN const KCoordiInfo& kInitCoordi_, IN const KCoordiInfo& kCurrentCoordi_, OUT KCoordiDiffInfo& kDiffInfo_  );
    void ExtractNotExistLeft( IN const std::set<GCITEMUID>& setLeft_, IN const std::set<GCITEMUID>& setRight, OUT std::set<GCITEMUID>& setExtract_ );
    bool IsEquiped( IN const std::set<GCITEMUID>& setEquips_, IN const GCITEMUID itemUID_ );

private:
    char m_cCharType;
    std::map<int,KCoordiInfo>  m_mapCoordiInfo;
    std::map<int,KCoordiInfo>  m_mapInitCoordiInfo;

};

#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include <Thread/Locker.h>

class KLookEquipment
{
    DeclareSingleton( KLookEquipment );
    NiDeclareRootRTTI( KLookEquipment );
    DeclToStringW;
public:
    KLookEquipment(void);
    ~KLookEquipment(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    GCITEMID GetChangeItemID();
    int GetCoordiNameLength();
    int GetMaxInvenSize();
    bool IsLookBanItem( IN const GCITEMID& ItemID );
    bool IsNormalBanItem( IN const GCITEMID& ItemID );
    bool IsLookItemID( IN const GCITEMID& ItemID );
    bool GetEnableAvatar();
    bool GetItemExtendSize( IN const GCITEMID& ItemID, OUT int& nExtendSize );
    GCITEMID GetCoordiComposeItemID() { KLocker lock( m_csLookEquip ); return m_CoordiComposeItemID; }
    std::map<GCITEMID, int> GetLookExtendInfo() { KLocker lock(m_csLookEquip); return m_mapLookExtendItemInfo; }

private:
    bool LoadItemIDList( IN KLuaManager& kLuaMng, IN const std::string& strTable, OUT std::set<GCITEMID>& setItemID );
    bool LoadLookExtendItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<GCITEMID, int>& mapLookExtendItemInfo );

private:
    mutable KncCriticalSection m_csLookEquip;
    GCITEMID m_lookChangeItemID;
    int m_nCoordiNameLength;
    int m_nMaxLookInvenSize;
    bool m_bUseAvatarSystem;
    GCITEMID m_CoordiComposeItemID; // 코디 합성 ItemID
    std::set<GCITEMID> m_setLookBanItemID; // 룩변환 금지 아이템
    std::set<GCITEMID> m_setNormalBanItemID; // 일반변환 금지 아이템
    std::set<GCITEMID> m_setLookItemList; // 룩아이템ItemID 리스트
    std::map<GCITEMID, int> m_mapLookExtendItemInfo; // 코디 인벤 늘리는 아이템 정보.
};
DefSingletonInline( KLookEquipment );
DeclOstmOperatorA( KLookEquipment );