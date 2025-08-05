#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include <set>
#include "PrimitiveTypedef.h"

class KLuaManager;
struct KItemUnit;
struct KDropItemInfo;

struct KCharPromotion
{
    GCITEMID               m_dwItemID;            // 전직 마법서 ItemID
    char                m_cEnablePromotion;    // 전직 가능한 전직 레벨.
    std::map< char, DWORD > m_mapCharMinLevel; // 전직 가능한 최소 레벨.
    std::map< char, DWORD > m_mapMissionIDList;    // 전직 마법서 사용시 등록해 줘야 하는 MissionID.
    std::map< char, std::vector<KDropItemInfo> > m_mapCharReward; // 전직시 보상 아이템 리스트.
};

class KCharPromotionSystem
{
    DeclareSingleton( KCharPromotionSystem );
    NiDeclareRootRTTI( KCharPromotionSystem );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

    KCharPromotionSystem(void);
    ~KCharPromotionSystem(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool CheckEnableItem( IN GCITEMID dwItemID, IN char cPromotion, OUT int& nOK ); // 사용가능한 아이템인지와 해당 아이템으로 해당 전직이 가능한지 체크.
    bool GetMissionID( IN GCITEMID dwItemID, IN char cCharType, OUT DWORD& dwMissionID ); // 미션 ID가져오기.
    bool GetPromotionReward( IN GCITEMID dwItemID, IN char cCharType, OUT std::vector<KDropItemInfo>& kReward ); // 해당 전직 아이템 보상 가져오기.
    bool CheckCharLevel( IN GCITEMID dwItemID, IN char cCharType, IN DWORD dwLevel ); // 캐릭터 최소 레벨 체크.
    void GetItemCharPromotionMinLevel( OUT std::map< DWORD, std::map<char, DWORD> >& mapItemPromotionLevel );

protected:
    bool LoadRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< char, VEC_ITEMS >& mapRewards );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_ITEMS& vecRewards );
    bool LoadMissionList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< char, DWORD >& mapMissionIDList );
    bool LoadMinLevelList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< char, DWORD >& mapCharMinLevel );

protected:
    mutable KncCriticalSection                      m_csCharPromotion;
    std::map< DWORD, KCharPromotion >               m_mapCharPromotionInfo;
};

DefSingletonInline( KCharPromotionSystem );
DeclOstmOperatorA( KCharPromotionSystem );
