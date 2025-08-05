#pragma once
#include <map>
#include <set>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "Kairo.h"

class KLuaManager;
struct KItemUnit;

struct KCharacterItemInfo
{
    GCITEMID                                        m_dwGachaItemID;
    GCITEMID                                        m_dwKeyItemID;
    std::map< int, int >                            m_mapNeedKeyInfo; // { ItemLv, NeedKeyCount }
    std::map< int, Kairo >                          m_mapKairoNormal;
    std::map< int, Kairo >                          m_mapKairoFail;
    std::map< int, std::vector< KDropItemInfo > >   m_mapGachSetReward; // < Type, ItemList >
    std::map< int, std::vector< GCITEMID > >        m_mapSetRewardList; // 클라이언트 GachaSetRewardList 전달을 위해서..,
    std::map< int, std::set< GCITEMID > >           m_mapCollectionList; // 클라이언트 GachaItemList 전달을 위해서..,
};

struct KGachaInfo
{
    int                                             m_nGachaVer;
    int                                             m_nGachaReelMaxPoint;
    bool                                            m_bGachaDecreaseEnable; // 봉인해제서 감소시킬지에 대한 여부.
    int                                             m_nGachaDecreaseCount;  // 감소시킬 봉인 해제서 갯수.
    bool                                            m_bGachaReelPointEnable; // 릴 사용여부
    bool                                            m_bEquipmentEnable; // 장비 오픈여부.
    bool                                            m_bAccessoriesEnable; // 악세사리 오픈여부.
    std::map< DWORD, std::pair< float, float > >    m_mapGachaItems; // 가챠아이템ID, 릴 설정값<GradePT,Div>

    std::map< GCITEMID, KCharacterItemInfo >        m_mapCharacterItemInfo;
};

class KGachaManager
{
    DeclareSingleton( KGachaManager );
    NiDeclareRootRTTI( KGachaManager );
    DeclToStringW;
public:
    enum GACHA_GRADE {
        GG_FREE     = 0, // 마력이고갈된
        GG_COMMON   = 1, // 낡은
        GG_UNCOMMON = 2, // 고급
        GG_RARE     = 3, // 최고급
    };

    enum GACHA_TYPE {
        GC_EQUIPMENT    = 0, // 장비 TYPE
        GC_ACCESSORISE  = 1, // 악세사리 TYPE
    };

    enum GACHA_REWARD_TYPE {
        GRT_EQUIP_REWARD = 0, // 장비 보상
        GRT_TOTAL_REWARD = 1, // 전체 보상
    };

    enum GACHA_REWARD_INFO {
        GRI_FAIL_REWARD     = 0,
        GRI_EQUIP_REWARD    = 1,
        GRI_WEAPON_REWARD   = 2,
        GRI_PET_REWARD      = 3,
    };

    KGachaManager(void);
    ~KGachaManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void GetRewardItemList( IN const int nVersion_, IN const GCITEMID dwItemID_, OUT std::map< int, std::set< GCITEMID > >& mapCollectionList_ );

    bool DoGamble( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const DWORD dwType_, IN const int nLevel_, OUT KDropItemInfo& kItem_, OUT bool& bRewardType_ );
    bool DoGambleFail( IN const int nVersion_, IN const GCITEMID dwItemID_, IN const int nLevel_, OUT KDropItemInfo& kItem_ );
    GCITEMID GetKeyItemID( IN const int nVersion_, IN const GCITEMID dwGachaItemID_ );
    int GetNeedKeyCount( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwLevel_, IN const bool bClear_ );
    bool IsGachaItem( IN const int nVersion_, IN const GCITEMID itemID_ );
    bool GetGachaSetReward( IN const int nVersion_, IN const GCITEMID gachaItemID_, OUT std::vector< KDropItemInfo >& vecRewardItems_, IN const DWORD dwType_, IN const GCITEMID ItemID_ );
    bool IsCollectionItem( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const GCITEMID nItemID_, IN const DWORD dwType_ );
    void GetGachaInfo( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, OUT float& fGrade_, OUT float& fDiv_ );
    bool GetGachaCollectItem( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const GCITEMID dwItemID_, OUT KDropItemInfo& kReward_, IN const DWORD dwType_ );
    bool IsReelPointEnable( IN const int nVersion_ );
    bool IsReelPointEnable();
    //int GetGachaVer() { KLocker lock(m_csGacha); return m_nGachaVer; }
    void GetGachaVersions( OUT std::vector< int >& vecVersions_ );
    void DumpNeedKey();
    void DumpGacha();
    void Print( IN const GCITEMID dwGachaItemID );
    bool IsUseReelPoint( IN const int nVersion_, IN const GCITEMID dwGachaItemID_ );
    bool GetAccessoriesEnable( IN const int nVersion_ );
    bool IsGachaTypeEnable( IN const int nVersion_, IN const DWORD dwType_ );
    bool GetGachaSetRewardList( IN const int nVersion_, IN const GCITEMID gachaItemID_, OUT std::map< int, std::vector< GCITEMID > >& mapGachSetReward_ );
    int GetGachaMaxReelPoint( IN const int nVersion_ );
    bool CheckGachaRewardLevel( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const DWORD dwLevel_ );
    bool GetGachaNeedKeyInfo( IN const int nVersion_, IN const GCITEMID dwGachaItemID_, IN const bool bClear_, OUT std::map< int,int >& mapNeedKeyInfo_ );
    bool GetGachaDecreaseEnable( IN const int nVersion_ );
    bool GetGachaUseVersions( OUT std::vector< KGachaNewList >& vecUseVersions_ );
    bool SendUserGachaDataGetFromDBNot( IN const std::string& strScript );
    bool SendGachaDataChangeNot( IN const std::string& strScript );

protected:
    bool LoadGachaTotalSetReward( IN KLuaManager& kLuaMng, OUT std::map< int, std::vector<KDropItemInfo> >& mapGachSetReward, OUT std::map< int, std::vector<GCITEMID> >& mapSetRewardList );
    bool LoadGachSetRewardList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector <KDropItemInfo>& vecGachSetReward );
    bool LoadCollectItemList( IN KLuaManager& kLuaMng, OUT std::map< int, Kairo >& mapKairoNormal, OUT std::map< int, std::set<GCITEMID> >& mapCollectionList );
    bool LoadGachaRatio( IN KLuaManager& kLuaMng, OUT std::map< int, Kairo >& mapKairoFail );
    bool LoadGachaRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo );
    bool LoadUserReelPoint( IN KLuaManager& kLuaMng, OUT std::map<USHORT,bool>& mapConfig );
    bool LoadGachaItemGrade( IN KLuaManager& kLuaMng_, OUT std::map< DWORD, std::pair< float, float > >& mapItems_ );
    bool LoadGachaItems( IN KLuaManager& kLuaMng_, IN OUT std::map< DWORD, std::pair< float, float > >& mapItems_ );
    bool LoadNeedKeyInfo( IN KLuaManager& kLuaMng, OUT std::map<int, int>& mapNeedKeyInfo );
    void DecreaseNeedKeyByClear( IN const bool bClear_, IN const bool bGachaDecreaseEnable_, IN const int nGachaDecreaseCount_, IN OUT std::map< int, int >& mapNeedKeyInfo_ );
    bool LoadUseVersions( IN KLuaManager& kLuaMng_, OUT std::vector< KGachaNewList >& vecUseVersions_ );
    bool LoadDefaultReelPointInfo( IN KLuaManager& kLuaMng_, OUT std::pair< float, float >& prDefaultReelPointInfo_ );

protected:
    mutable KncCriticalSection    m_csGacha;
    std::vector< KGachaNewList >  m_vecUseVersions; // 가차버전, 해당 버전 최종 보상 아이템 ID(펫)
    std::pair< float, float >     m_prDefaultReelPointInfo; // 기본 릴 포인트 정보. ( 등급 포인트, divider )
    std::map< int, KGachaInfo >   m_mapGachaInfos; // < 버전, < 가챠구슬ID, 가차정보 > >
};

DefSingletonInline( KGachaManager );
DeclOstmOperatorA( KGachaManager );