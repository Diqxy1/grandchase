#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include "UserPacket.h"
#include "ManufactureS3.h"

class KAgitHelper
{
    DeclareSingleton( KAgitHelper );
    NiDeclareRootRTTI( KAgitHelper );
    DeclToStringW;

public:
    KAgitHelper(void);
    ~KAgitHelper(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void DumpInfo();

    void UpdateAgitConfig( IN const KAgitConfig& kInfo );
    void UpdateMapInfo( IN const KAgitMapInfo& kInfo );
    void UpdateFlowerPotConfig( IN const KFlowerPotConfig& kInfo );

    void GetMapCatalogue( OUT GCITEMID& PayItemID, OUT std::map<DWORD,KAgitMap>& mapList );
    bool GetCurrentMapInfo( IN const DWORD& dwMapID, OUT KAgitMap& kInfo );

    void UpdateAgitObjectItemID();
    void AddAgitObjectItemID( IN const GCITEMID& ItemID ) { KLocker lock(m_csConfig); m_setAgitObjectItemID.insert( ItemID ); }
    bool IsObjectItem( IN const GCITEMID& ItemID ) { KLocker lock(m_csConfig); return (m_setAgitObjectItemID.find( ItemID ) != m_setAgitObjectItemID.end()); }
    void GetAgitObjectItemIDList( OUT std::set<GCITEMID>& setList ) { KLocker lock( m_csConfig ); setList = m_setAgitObjectItemID; }

    void UpdateBrownieItemID();
    bool IsBrownieItem( IN const GCITEMID& ItemID ) { KLocker lock(m_csConfig); return (m_setBrownieItemID.find( ItemID ) != m_setBrownieItemID.end()); }

    USHORT GetMaxAgitNameLength() { KLocker lock( m_csConfig ); return m_kAgitConfig.m_usMaxAgitNameLength; }
    USHORT GetMaxProfileLength() { KLocker lock( m_csConfig ); return m_kAgitConfig.m_usMaxProfileLength; }
    USHORT GetMaxUserCount() { KLocker lock( m_csConfig ); return m_kAgitConfig.m_usMaxUserCount; }
    USHORT GetMaxPasswordLength() { KLocker lock( m_csConfig ); return m_kAgitConfig.m_usMaxPasswordLength; }
    int GetMaxInvenSize() { KLocker lock(m_csConfig); return m_kAgitConfig.m_nMaxInvenSize; }
    USHORT GetSellRewardFlowerCount() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usSellRewardFlowerCount; }
    bool IsBrownieID( IN const GCITEMID& ItemID ) { KLocker lock(m_csConfig); return (m_kAgitConfig.m_setBrownieID.find( ItemID ) != m_kAgitConfig.m_setBrownieID.end()); }
    void GetTutorialItem( OUT std::vector<KDropItemInfo>& vecList ) { KLocker lock(m_csConfig); vecList = m_kAgitConfig.m_vecTutorialItem; }
    bool IsTutorialItem( IN const GCITEMID dwItemID_ );
    bool IsAgitItem( IN const GCITEMID dwItemID_ );
    USHORT GetMaxGuestbookMsgLenth() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usMaxGuestbookMsgLenth; }
    DWORD GetGuestbookWriteTerm() { KLocker lock(m_csConfig); return m_kAgitConfig.m_dwGuestbookWriteTerm; }
    GCITEMID TrainingMaterialItemID() { KLocker lock(m_csConfig); return m_kAgitConfig.m_TrainingMaterialItemID; }
    GCITEMID TrainingItemID() { KLocker lock(m_csConfig); return m_kAgitConfig.m_TrainingItemID; }
    float GetTrainingExpRatio() { KLocker lock(m_csConfig); return m_kAgitConfig.m_fTrainingExpRatio; }
    USHORT GetCharacterOpenCost() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usCharacterOpenCost; }
    USHORT GetCharacterSpeechLength() { KLocker lock(m_csConfig); return m_kAgitConfig.m_usCharacterSpeechLength; }
    bool IsNotResellObjectItemID( IN const GCITEMID& ItemID ) { KLocker lock(m_csConfig); return (m_kAgitConfig.m_setNotResellObjectItemID.find( ItemID ) != m_kAgitConfig.m_setNotResellObjectItemID.end()); }
    void GetObjectItemID( OUT std::vector<GCITEMID>& vecList ) { KLocker lock(m_csConfig); vecList = m_kAgitConfig.m_vecObjectItemID; }
    void GetObjectItemInvenLimitList( OUT std::map<GCITEMID,int>& mapList ) { KLocker lock(m_csConfig); mapList = m_kAgitConfig.m_mapObjectItemInvenLimit; }
    bool GetObjectItemInvenLimitCount( IN const GCITEMID& , OUT int& nLimitCount );

    GCITEMID GetFlowerItemID() { KLocker lock( m_csFlowerPot ); return m_kFlowerPotConfig.m_FlowerItemID; }
    int GetInvenLimitFlowerCount() { KLocker lock( m_csFlowerPot ); return m_kFlowerPotConfig.m_nInvenLimitFlowerCount; }
    bool GetFlowerPotOption( IN const int& nSeedType, OUT KFlowerPotOption& kOption );

    GCITEMID GetSeedItemID() { KLocker lock(m_csFlowerPot); return m_kFlowerPotConfig.m_SeedItemID; }
    DWORD GetSeedDropTime() { KLocker lock( m_csFlowerPot ); return static_cast<DWORD>(m_kFlowerPotConfig.m_nSeedDropTime * 1000 * 60); }
    int GetSeedDropCount() { KLocker lock( m_csFlowerPot ); return m_kFlowerPotConfig.m_nSeedDropCount; }
    int GetInvenLimitSeedCount() { KLocker lock( m_csFlowerPot ); return m_kFlowerPotConfig.m_nInvenLimitSeedCount; }
    int GetDailyMaxSeedCount() { KLocker lock( m_csFlowerPot ); return m_kFlowerPotConfig.m_nDailyMaxSeedCount; }
    int GetDailyInitSeedCount() { KLocker lock( m_csFlowerPot ); return m_kFlowerPotConfig.m_nDailyInitSeedCount; }

    void UpdateFairyTreeConfig( IN const KFairyTreeConfig& kInfo ) { KLocker lock(m_csFairyTree); m_kFairyTreeConfig = kInfo; }
    void GetFairyTreeConfig( OUT KFairyTreeConfig& kInfo )  { KLocker lock(m_csFairyTree); kInfo = m_kFairyTreeConfig; }
    float GetFruitBuffRate( IN const int& nFairyTreeLv, IN const int& nFruitType );

    void GetNormalPotID( OUT std::set<GCITEMID>& setList ) { KLocker lock(m_csFlowerPot); setList = m_kFlowerPotConfig.m_setNormalPotItemID; }
    bool IsNormalPotItemID( IN const GCITEMID& PotItemID );
    bool IsCashPotItemID( IN const GCITEMID& PotItemID );

    void SetAgitSID( IN const DWORD& dwAgitUID, IN const int& nSID ) { KLocker lock(m_csAgitSID); m_mapAgitSID[dwAgitUID] = nSID; }
    int GetAgitSID( IN const DWORD& dwAgitUID );
    void ClearAgitSID() { KLocker lock(m_csAgitSID); m_mapAgitSID.clear(); }

public:
    KManufactureS3                  m_kAgitStoreItems; // 아지트 상점
    KManufactureS3                  m_kBrownieStoreItems; // 도우미 상점

protected:
    mutable KncCriticalSection      m_csConfig;
    KAgitConfig                     m_kAgitConfig; // 아지트 설정정보
    std::set<GCITEMID>              m_setAgitObjectItemID; // 아지트오브젝트ItemID
    std::set<GCITEMID>              m_setBrownieItemID; // 도우미ItemID
    std::set<GCITEMID>              m_setBrownieID; // 도우미ID

    mutable KncCriticalSection      m_csMap;
    std::map<DWORD,KAgitMap>        m_mapMapInfo; // map[MapID,MapInfo]

    mutable KncCriticalSection      m_csFlowerPot;
    KFlowerPotConfig                m_kFlowerPotConfig; // 별사탕화분 설정정보

    mutable KncCriticalSection      m_csAgitSID;
    std::map<DWORD,int>             m_mapAgitSID; // 아지트정보가 어느서버에 있는지. map[AgitUID,에이전트서버ID]

    mutable KncCriticalSection      m_csFairyTree;
    KFairyTreeConfig                m_kFairyTreeConfig;
};
DefSingletonInline( KAgitHelper );
DeclOstmOperatorA( KAgitHelper );