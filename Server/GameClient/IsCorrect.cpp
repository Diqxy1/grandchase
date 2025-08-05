#include "IsCorrect.h"

namespace {

    static const DWORD s_dwTable[] =
    {
        //    동        은        금
            203,      585,     2280,  // 떠돌이
           7609,    19580,   127530,  // 용병
         502710,  1492510,  3419710,  // 왕립 
        5717710,  8890510,            // 신
    };

    enum EXP_BOOSTING {
        EXP_BOOST_OPENTESOP     = 5,    // 오픈테섭
        EXP_BOOST_CRUSADER      = 12,   // 몬스터원정대(가도센 5마리)
        EXP_BOOST_PCBANG        = 2,    // 넷마블 pc방
        EXP_BOOST_GUILD         = 2,    // 길드전
        EXP_BOOST_TURTLESBACK   = 2,    // 거북이 등껍질
    };
}

/// Exp를 레벨로 환산한다.
unsigned char Exp_2_Level(DWORD Exp)
{
    // 각 레벨에 대한 경험치 경계값 테이블. 최초의 레벨0의 경험치는 100이다.
    static const DWORD s_dwExpTable[] =
    {   //    1         2         3         4         5         6         7         8         9        10
            101,      110,      125,      146,      173,      203,      236,      272,      311,      375,  // 10
            470,      585,      747,      964,     1279,     1708,     2280,     2966,     3776,     4838,  // 20
           6118,     7609,     9247,    11032,    12964,    15340,    19580,    26360,    41360,    60410,  // 30
          87210,   127530,   176810,   242410,   355510,   502710,   687010,   911410,  1178910,  1492510,  // 40
        1866910,  2309710,  2825710,  3419710,  4096510,  4860910,  5717710,  6671710,  7727710,  8890510,  // 50
       10164910, 11567110, 13103110, 14778910, 16600510, 18573910, 20729110, 23074110, 25616910, 28365510,  // 60
       31433710, 34757710, 38231710, 41855710, 45629710, 49553710, 53657710, 57941710, 62405710, 67049710,  // 70
       71873710, 76907710, 82151710, 87605710, 93269710,
    };

    // 경험치-레벨 테이블을 참조하여 현재 경험치에 대한 레벨을 얻어낸다.
    for ( int i = 0; i < sizeof( s_dwExpTable ) / sizeof( DWORD ); ++i )
    {
        if ( Exp < s_dwExpTable[i] )
        {
            return i;
        }
    }

    return i; // 75
}

bool IsCorrectGP(int iNowExp, int iChangeValue, bool bIsOpenTesop )
{
    static const int s_iGPGetTable[] =  // 각 레벨에서 얻을 수 있는 GP 테이블
    {   // 획득GP   // Level
        // 0      1      2      3      4      5      6      7      8      9
           5,     8,    10,    13,    16,    20,    22,    24,    26,    29,    // 0
          34,    40,    44,    48,    53,    60,    68,    72,    78,    85,    // 10
          93,    98,   104,   109,   114,   119,   125,   135,   148,   173,    // 20
         200,   202,   204,   206,   208,   210,   212,   214,   216,   218,    // 30
         220,   222,   224,   226,   228,   230,   232,   234,   236,   238,    // 40
         240,   242,   244,   246,   248,   250,   252,   254,   256,   258,    // 50
         260,   262,   264,   266,   268,   270,   272,   274,   276,   278,    // 60
         280,   282,   284,   286,   288,   290,
    };

    int iLevel  = Exp_2_Level( iNowExp );
    int iBound  = 0;

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > 75 ) iLevel = 75;

    // 바운드 계산
    float fBoostValue = EXP_BOOST_CRUSADER;
    if( bIsOpenTesop ) fBoostValue *= EXP_BOOST_OPENTESOP;

    // GP 아이템(게임 중 나오는 금화) 획득으로 얻을 수 있는 최대 보상은 30원( = 최대 3개 * 10원)이다.
    iBound = static_cast<int>( static_cast<float>( s_iGPGetTable[iLevel] + 30 ) * fBoostValue );

    // 감소 가능치는 획득 가능치의 절대값과 같은 음수이나, 상황에 따른 배수처리는 되지 않는다.
    if ( iChangeValue < -s_iGPGetTable[iLevel] || iChangeValue > iBound )
    {
        return false; // oh, no!
    }

    return true; // ok!
}

int GetMaxGP(int iNowExp, int iChangeValue, bool bIsOpenTesop )
{
    static const int s_iGPGetTable[] =  // 각 레벨에서 얻을 수 있는 GP 테이블
    {   // 획득GP   // Level
        // 0      1      2      3      4      5      6      7      8      9
        5,     8,    10,    13,    16,    20,    22,    24,    26,    29,    // 0
        34,    40,    44,    48,    53,    60,    68,    72,    78,    85,    // 10
        93,    98,   104,   109,   114,   119,   125,   135,   148,   173,    // 20
        200,   202,   204,   206,   208,   210,   212,   214,   216,   218,    // 30
        220,   222,   224,   226,   228,   230,   232,   234,   236,   238,    // 40
        240,   242,   244,   246,   248,   250,   252,   254,   256,   258,    // 50
        260,   262,   264,   266,   268,   270,   272,   274,   276,   278,    // 60
        280,   282,   284,   286,   288,   290,
    };

    int iLevel  = Exp_2_Level( iNowExp );
    int iBound  = 0;

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > 75 ) iLevel = 75;

    // 바운드 계산
    float fBoostValue = EXP_BOOST_CRUSADER;
    if( bIsOpenTesop ) fBoostValue *= EXP_BOOST_OPENTESOP;

    // GP 아이템(게임 중 나오는 금화) 획득으로 얻을 수 있는 최대 보상은 30원( = 최대 3개 * 10원)이다.
    iBound = static_cast<int>( static_cast<float>( s_iGPGetTable[iLevel] + 30 ) * fBoostValue );

    // 감소 가능치는 획득 가능치의 절대값과 같은 음수이나, 상황에 따른 배수처리는 되지 않는다.
    if( iChangeValue < 0 )
    {
        if( iChangeValue < -s_iGPGetTable[iLevel] )
            return s_iGPGetTable[iLevel];
        else
            return iChangeValue;
    }

    if( iChangeValue > iBound )
        return iBound;

    return iChangeValue;
}

int GetMaxExp(int iNowExp, int iChangeValue, bool bIsInPCBang, bool bIsOpenTesop )
{
    static const int s_iExpGetTable[] = // 각 레벨에서 얻을 수 있는 경험치 테이블.
    {   // 획득Exp  // Level
        // 0      1      2      3      4      5      6      7      8      9
        1,     3,     3,     3,     3,     3,     3,     3,     3,     4,    // 0
        5,     5,     6,     7,     9,    11,    13,    14,    15,    18,    // 10
        20,    21,    21,    21,    21,    24,    40,    60,   125,   150,    // 20
        200,   280,   320,   400,   650,   800,   950,  1100,  1250,  1400,    // 30
        1600,  1800,  2000,  2200,  2400,  2600,  2800,  3000,  3200,  3400,    // 40
        3600,  3800,  4000,  4200,  4400,  4600,  4800,  5000,  5200,  5400,    // 50
        5800,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,    // 60
        6000,  6000,  6000,  6000,  6000,  6000
    };

    int iLevel  = Exp_2_Level( iNowExp );
    int iBound  = 0;

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > 75 ) iLevel = 75;

    // 바운드 계산
    float fBoostValue = EXP_BOOST_TURTLESBACK   // 거북이 등껍질 보상
                        * EXP_BOOST_CRUSADER;     // 몬스터 원정대 최대 보상(가도센 5마리) (퀘스트의 최대 보상은 4.5)

    if( bIsInPCBang )   fBoostValue *= EXP_BOOST_PCBANG;        // 넷마블 PC방
    if( bIsOpenTesop )  fBoostValue *= EXP_BOOST_OPENTESOP;     // 오픈테섭

    iBound = static_cast<int>( static_cast<float>( s_iExpGetTable[iLevel] ) * fBoostValue );

    if( iChangeValue < 0 )
    {
        if( iChangeValue < -s_iExpGetTable[iLevel] )
            return -s_iExpGetTable[iLevel];
        else
            return iChangeValue;

    }

    if( iChangeValue > iBound )
    {
        return iBound; // oh, no!
    }

    return iChangeValue;

}

bool IsCorrectExp(int iNowExp, int iChangeValue, bool bIsInPCBang, bool bIsOpenTesop )
{
    static const int s_iExpGetTable[] = // 각 레벨에서 얻을 수 있는 경험치 테이블.
    {   // 획득Exp  // Level
        // 0      1      2      3      4      5      6      7      8      9
           1,     3,     3,     3,     3,     3,     3,     3,     3,     4,    // 0
           5,     5,     6,     7,     9,    11,    13,    14,    15,    18,    // 10
          20,    21,    21,    21,    21,    24,    40,    60,   125,   150,    // 20
         200,   280,   320,   400,   650,   800,   950,  1100,  1250,  1400,    // 30
        1600,  1800,  2000,  2200,  2400,  2600,  2800,  3000,  3200,  3400,    // 40
        3600,  3800,  4000,  4200,  4400,  4600,  4800,  5000,  5200,  5400,    // 50
        5800,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,    // 60
        6000,  6000,  6000,  6000,  6000,  6000
    };

    int iLevel  = Exp_2_Level( iNowExp );
    int iBound  = 0;

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > 75 ) iLevel = 75;

    // 바운드 계산
    float fBoostValue = EXP_BOOST_TURTLESBACK   // 거북이 등껍질 보상
                      * EXP_BOOST_CRUSADER;     // 몬스터 원정대 최대 보상(가도센 5마리) (퀘스트의 최대 보상은 4.5)

    if( bIsInPCBang )   fBoostValue *= EXP_BOOST_PCBANG;        // 넷마블 PC방
    if( bIsOpenTesop )  fBoostValue *= EXP_BOOST_OPENTESOP;     // 오픈테섭

    iBound = static_cast<int>( static_cast<float>( s_iExpGetTable[iLevel] ) * fBoostValue );

    // 감소 가능치는 획득 가능치의 절대값과 같은 음수이나, 상황에 따른 배수처리는 되지 않는다.
    if ( iChangeValue < -s_iExpGetTable[iLevel] || iChangeValue > iBound )
    {
        return false; // oh, no!
    }

    return true; // ok!
}

DWORD GetGradePoint( const std::vector<DWORD>& Exp )
{
    DWORD dwAccumExp    = 0;
    DWORD dwBestExp     = 0;

    // 모든 캐릭터의 경험치를 누적시키고 가장 높은 EXP를 찾는다.
    std::vector<DWORD>::const_iterator  vit;
    for(vit = Exp.begin(); vit != Exp.end(); vit++)
    {
        dwAccumExp += *vit;
        if ( dwBestExp < *vit ) dwBestExp = *vit;
    }

    // ( 모든 캐릭터 EXP 합 + 캐릭터 중 가장 높은 EXP * 2 ) / ( 주 캐릭터의 계산 횟수(3=1+2) )
    return ( dwAccumExp + dwBestExp * 2 ) / 3;
}

unsigned char GetGradeKor(const std::vector<DWORD>& Exp )
{          
    DWORD dwTotal = GetGradePoint( Exp );

    // 테이블을 참조하여 해당하는 계급을 찾아낸다.
    for ( int i = 0; i < sizeof( s_dwTable ) / sizeof( DWORD ); ++i )
    {
        if ( dwTotal < s_dwTable[i] )
        {
            return (i+1);
        }
    }

    return (i+1); // 11 (신 금)
}