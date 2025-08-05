#pragma once

#include "KNCSingleton.h"

struct SAutoMatchStat
{
    bool m_bValid;
    float m_fMaxStat;        //  게임내 지정된 최대 스탯값
    float m_fBasicStat;      //  게임내 지정된 기본값
    float m_fRangeStat;      //  기본 스탯 범위값 ( 최하 ~ 기본, 기본 ~ 최상 범위 )    

    SAutoMatchStat()
        : m_fMaxStat( 0 )
        , m_fBasicStat( 0 )
        , m_fRangeStat( 0 )
        , m_bValid( false )
    {
    }

    void Reset()
    {
        m_fMaxStat = 0;
        m_fBasicStat = 0;
        m_fRangeStat = 0;
        m_bValid = false;
    }

    bool IsValid() { return m_bValid; }
    void SetValid( bool bValid ) { m_bValid = bValid; }
};


class GCAutoMatchManager
{
public:
    enum
    {
        EM_AUTO_MATCH_MAX_MODE = 3,
    };

    enum EMAutoMatchBallanceStat
    {
        EM_AM_STAT_AP = 0,      //  공격력
        EM_AM_STAT_DP,          //  방어력
        EM_AM_STAT_HP,          //  생명력
        EM_AM_STAT_SPEC_AP,     //  필공
        EM_AM_STAT_SPEC_DP,     //  필방
        EM_AM_STAT_CRITICAL,    //  치명
        EM_AM_STAT_CRITICAL_DAMAGE,  //  치뎀
        EM_AM_STAT_SIZE,
    };

    enum PVPMATCHINGBALANCE
    {
        PMB_DIRECT_HP_DAMAGE = 0,   // -- 들어가는 데미지 자체를 밸런싱
        PMB_EQUIPMENT_AP,           // -- 착용 장비 공격력
        PMB_EQUIPMENT_DP,           // -- 착용 장비 방어력
        PMB_EQUIPMENT_HP,           // -- 착용 장비 생명력
        PMB_BASE_AP,                // -- 기본 공격력
        PMB_BASE_DP,                // -- 기본 방어력
        PMB_BASE_HP,                // -- 기본 생명령
        PMB_SPECIAL_AP,             // -- 착용 장비 필공
        PMB_SPECIAL_DP,             // -- 착용 장비 필방
        PMB_HP_RECORVERY,           // -- HP 회복력
        PMB_MP_RECORVERY,           // -- MP 회복력
        PMB_CRITICAL_AP,            // -- 크리티컬 데미지 자체 밸런싱	
        PMB_CRITICAL_PROBABILITY,   // -- 장비 크리티컬 확률
        PMB_COUNTER_MP,             // -- 카운터 MP
        PMB_COUNTER_DP,             // -- 미사용
        PMB_BUFF_AP,                // -- 미사용
        PMB_BUFF_DP,                // -- 미사용
        PMB_BUFF_HP,                // -- 미사용
        PMB_HELL_SPEAR,             // -- 헬스피어
        PMB_SIZE,
    };


    DeclareSingleton( GCAutoMatchManager );

public:
    GCAutoMatchManager(void);
    ~GCAutoMatchManager(void);

private:
    float GetBallanacStat( int iMode, EMAutoMatchBallanceStat eAbilityType, float fValue ); 

public:
    void LoadScript();
    bool LoadScript( std::string strFileName );

    void Reset();
    float GetAutoMatchBallanacStat( int iMode, EAbilityType eAbilityType, float fValue ); 
    void GetPvPMatchingBalance( int iType, float &tVal );

    int GetCurrentBalanceMode();
    
    bool IsAutoMatchBalance();  // 대전 매칭 밸런스 사용여부
    bool IsNotPetSpecial();
    bool IsNot4Special();

public:
    SAutoMatchStat  m_sBallaceStat[EM_AUTO_MATCH_MAX_MODE][EM_AM_STAT_SIZE];
    float           m_fPMBValue[PMB_SIZE];

};

DefSingletonInline( GCAutoMatchManager );