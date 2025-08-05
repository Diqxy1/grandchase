#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "Kairo.h"

class Kairo;
class KLuaManager;
struct KDropItemInfo;
struct KRewardInfo;
class KSpecialReward
{
    DeclareSingleton( KSpecialReward );
    NiDeclareRootRTTI( KSpecialReward );
    DeclToStringW;
    typedef std::pair<int,int> DUNGEON_KEY;
    typedef std::pair<USHORT,int> BOX_KEY;

public:
    KSpecialReward(void);
    ~KSpecialReward(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetBoxInfo( IN const int& nBoxID, OUT std::vector<USHORT>& vecBoxType );
    int GetDifficultBoxID( IN const int& nDifficult );
    bool GetBoxDropItem( IN const DUNGEON_KEY& prModeKey, IN const BOX_KEY& prBoxKey, OUT KDropItemInfo& kItem );
    float GetBoxGPRate( IN const USHORT& usBoxType );
    bool IsGPBox( IN const USHORT& usBoxType );
    bool IsItemBox( IN const USHORT& usBoxType );

    void DumpBoxInfo();
    void DumpDifficultBoxRate();
    bool DumpBoxItemList( IN const int& nModeID, IN const int& nDiff, IN const USHORT& usBoxType, IN const int& nCharType );

private:
    bool LoadKairo( IN OUT KLuaManager& kLuaMng_, IN const std::string& strTable_, IN DUNGEON_KEY& prDungeonKey_, IN BOX_KEY& prBoxKey_,OUT Kairo& kData_ );
    bool LoadBoxInfo( IN KLuaManager& kLuaMng, OUT std::map< int, std::vector<USHORT> >& mapBoxInfo );
    bool LoadDifficultBoxRate( IN KLuaManager& kLuaMng, OUT std::map< int, KLottery >& mapDifficultBoxRate );
    bool LoadDropBoxRate( IN KLuaManager& kLuaMng, IN OUT KLottery& kData );
    bool LoadDungeonBoxDropList( IN KLuaManager& kLuaMng, OUT std::map< DUNGEON_KEY, std::map<BOX_KEY,Kairo> >& mapDungeonBoxDropList );
    bool LoadBoxGPRate( IN KLuaManager& kLuaMng_, OUT std::map< USHORT, float >& mapBoxGPRate );

private:
    mutable KncCriticalSection                          m_csDrops;
    std::map< int, std::vector<USHORT> >                m_mapBoxInfo;           // map< BoxID, vector<BoxType> > 보상상자ID별 Type정보
    std::map< int, KLottery >                           m_mapDifficultBoxRate;  // map< Difficulty, BoxID-Rate > 던전 난이도별 보상상자 확률
    std::map< DUNGEON_KEY, std::map<BOX_KEY,Kairo> >    m_mapDungeonBoxDropList;  // map< [ModeID,Difficulty], map< [BoxType,CharType],DropList> >  던전난이도별 보상타입/캐릭터타입별 아이템드랍 리스트
    std::map< USHORT, float >                           m_mapBoxGPRate; // map<BoxType,GPrate> GP보상 퍼센테이지.
    std::set< USHORT >                                  m_setItemBox; // set< BoxType > 아이템보상 박스Type
};
DefSingletonInline( KSpecialReward );
DeclOstmOperatorA( KSpecialReward );
