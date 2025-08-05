#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>

struct KRainBowEvent; // CommonPacket.h
struct KRainBowData; // CommonPacket.h
struct KECN_RAINBOW_UPDATE_NOT; // CenterPacket.h
class KRainbow
{
    DeclareSingleton( KRainbow );
    NiDeclareRootRTTI( KRainbow );
    DeclToStringW;

public:

    enum RAIN_BOW_EVENT
    {
        RE_DUNGEON_EXP,         // ++던전 경험치 상승
        RE_GP_BONUS,            // ++GP 상승
        RE_DUNGEON_DROP,        // ++ 던전 드랍률 상승.
        RE_LUNE_STRONG,         // ++룬강화상승, 비쥬 소비량 감소
        RE_CHAMP_FREE,          // ++챔피언 자유 입장
        RE_PVP_EXP,             // ++PVP 경험치 상승
        RE_NECKLACE_STRONG,     // ++목걸이 강화율 상승/ 잼소비량 감소
    };

public:
    KRainbow(void);
    ~KRainbow(void);

    
    bool IsEventChanged();
    void SetEventChanged( bool bChange );
    bool IsRainbowEventTerm();
    bool IsJackPotTime();
    bool GetEventData( OUT KRainBowEvent& kEventData );
    bool GetEventByID( IN const int nEventID, OUT KRainBowData& kEvent );

    int GetRainbowID();
    int GetDailyEventID();
    int GetJackPotEventID();
    void UpdateFromCenter( IN KECN_RAINBOW_UPDATE_NOT& kPacket );
    void Tick();

    void AddItem( IN const GCITEMID dwItemID, IN const int nCount, IN const int nPeriod );
    bool GetJacPotItemList( OUT std::vector<KDropItemInfo>& vecItemList );

    void DumpJackPotItems( std::wostream& stm ) const;
    void DumpEvents( std::wostream& stm ) const;

    bool IsReceivedUser( IN DWORD dwUID );
    void InsertReceivedUser( IN DWORD dwUID );

protected:
    mutable KncCriticalSection  m_csRainbow;
    std::vector<KDropItemInfo>  m_vecJackPotItems;

    int     m_nRainbowID;
    int     m_nDailyEventID;
    int     m_nJackPotEventID;

    bool    m_bChanged;
    std::map<int, KRainBowData> m_mapEventList;
    std::set<DWORD> m_setReceivedUsers;
};

DefSingletonInline( KRainbow );
DeclOstmOperatorA( KRainbow );