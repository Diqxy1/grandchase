#pragma once
#include ".\statistics.h"
#include "UserPacket.h"

struct KGpStatKey {
    int m_nModeID;  // ModeID
    int m_nCharType;// 캐릭터 타입
    int m_nLv;      // 캐릭터 레벨
    int m_nPromotion;//캐릭터 전직

    KGpStatKey()
    :m_nModeID(-1)
    ,m_nCharType(-1)
    ,m_nLv(-1)
    ,m_nPromotion(-1)
    {}

    bool operator == ( const KGpStatKey& right ) const
    {
        return ( m_nModeID    == right.m_nModeID &&
                 m_nCharType  == right.m_nCharType &&
                 m_nLv        == right.m_nLv &&
                 m_nPromotion == right.m_nPromotion );
    }

    bool operator < ( const KGpStatKey& right ) const
    {
        if( m_nModeID > right.m_nModeID )
            return false;
        else if( m_nModeID < right.m_nModeID)
            return true;

        if( m_nCharType > right.m_nCharType )
            return false;
        else if( m_nCharType < right.m_nCharType )
            return true;

        if( m_nLv > right.m_nLv )
            return false;
        else if( m_nLv < right.m_nLv )
            return true;

        if( m_nPromotion > right.m_nPromotion )
            return false;
        else if( m_nPromotion < right.m_nPromotion )
            return true;

        return false;
    }

    std::wstring Str() const
    {
        std::wstringstream stm;
        stm << m_nModeID << L" | "
            << m_nCharType << L" | "
            << m_nLv << L" | "
            << m_nPromotion;
        return stm.str();
    }
};

struct KGpStatVal {
    __int64 m_biGP;     // 획득/소비 GP
    int     m_nCount;   // 횟수

    void Init()
    {
        m_biGP = 0;
        m_nCount = 0;
    }

    void AddStat( IN const __int64& biGP )
    {
        m_biGP += biGP;
        ++m_nCount;
    }

    std::wstring Str() const
    {
        std::wstringstream stm;
        stm << m_biGP << L" | "
            << m_nCount;
        return stm.str();
    }
};

SmartPointer( KStatGP );
class KStatGP : public KStatistics
{
public:
    typedef std::map<int, std::map<KGpStatKey,KGpStatVal> > MAP_GP_STAT;

    KStatGP(void);
    ~KStatGP(void);

    virtual void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( IN const int& nStatType, IN const KGpStatInfo& kData );
    bool DayCheck();
    bool OneMinuteCheck();
    void SendLevelGPInfo();

protected:
    MAP_GP_STAT     m_mapGpInc; // GP 획득 통계 [IncType, [ModeID, [IncGP,count] ] ]
    MAP_GP_STAT     m_mapGpDec; // GP 소모 통계 [DecType, [ModeID, [DecGP,count] ] ]
    std::map<int, std::pair<__int64, __int64>>   m_mapLevelGPInfo; // [Level, [IncGC, DecGP]]
    KSimpleDate     m_kLastDayCheckDate;
    // Tick 관련 데이터
    DWORD           m_dwLastTick;
    DWORD           m_dwTickGap;
};

/*
    enum GP_INC_TYPE {
    GIT_UNKNOWN = -1,           // 알수없음.
    GIT_MINIGAME_CLEAR = 0,     // 미니게임 클리어
    GIT_COMPLETE_MISSION,       // 미션완료
    GIT_YAHOO_BENEFIT,          // 아후 이벤트
    GIT_GP_COUPON,              // GP 쿠폰
    GIT_LUNA_NEWYEAR,           // 신년 이벤트
    GIT_COMPLETE_ACHIEVEMENT,   // 칭호 획득
    GIT_SPECIAL_REWARD,         // 던전 - 스페셜 보상
    GIT_DUNGEON_REWARD_GP,      // 던전 - 몬스터 드랍
    GIT_PVP_REWARD_GP,          // 대전 - 킬 보상
    GIT_GAME_CLEAR_BONUS,       // 게임 클리어 보너스
    };

    -- GP 소모 경로
    enum GP_DEC_TYPE {
    GDT_UNKNOWN = -1,           // 알수없음.
    GDT_USE_SOCKET = 0,         // 소켓 사용
    GDT_GAMEBLE_BUY,            // GP겜블 구매
    GDT_MANUFACTURE,            // 아이템 제작
    GDT_ITEM_BREAKUP,           // 아이템 해체
    GDT_CREATE_GUILD,           // 길드 생성
    GDT_OPEN_CAPSULE,           // 캡슐 열기
    GDT_DONATION_POINT,         // GP 기부
    GDT_EMOTICON_MIX,           // 이모티콘 조합
    GDT_GP_BUY,                 // GP 구매
    };
*/
