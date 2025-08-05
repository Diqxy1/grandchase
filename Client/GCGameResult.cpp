#include "stdafx.h"
#include "ProtectedType.h"
#include "QuestGameManager.h"
#include "GCGameResult.h"
#include "Monster.h"
#include "KGCGameBoard.h"
#include "GCDropItemManager.h"
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "Message.h"
#include "KGCSyncObject.h"
#include "KGCSubjectManager.h"


KProtectedType<int>     g_aiArrowScore[MAX_PLAYER_NUM][3] = { 0, };

bool    IsBlueWin       = false;
bool    IsDetermineWin  = false;
bool    IsResultComplete= false;

const float g_fExpTenBonus = 1.1f;
const float g_fGpTenBonus = 1.1f;
const float g_fExpFiveBonus = 1.05f;
const float g_fGpFiveBonus = 1.05f;

static std::map<DWORD, GCEXPTYPE>   mapExpTable;


// 각 레벨에 대한 경험치 경계값 테이블. 최초의 레벨0의 경험치는 0이다.

//  서버에서 받아오는걸로 수정했습니다. ( 2011/10/10 )
// 시즌2에 적용될 거..
//static KProtectedType< GCEXPTYPE > s_biExpTable[] =
//{
//    // 레벨 디자인 경험치 조정 ( 2011_09_21 ) 
////  0           1           2           3           4           5           6           7           8           9           // 레벨
//    100,        140,        422,        863,        1314,       1995,       2625,       3424,       4202,       5156,       // 0
//    6090,       7930,       9491,       11824,      14630,      16487,      19613,      21740,      24444,      28655,      // 10
//    33733,      38598,      46099,      53437,      60997,      69229,      78301,      88297,      99301,      112405,     // 20
//    127987,     146257,     167761,     192870,     222102,     254232,     301440,     353352,     410472,     473304,     // 30
//    542352,     618456,     702120,     794184,     895320,     1006704,    1159794,    1328214,    1513434,    1757874,    // 40
//    2026758,    2322606,    2702160,    3119640,    3752250,    4448445,    5214300,    6056700,    6983340,    8104218,    // 50
//    9337362,    10693909,   12185888,   13976312,   15946070,   18293207,   20875163,   23933561,   27297572,   32388632,   // 60
//    40789232,   51568637,   65120357,   81891137,   102389537,  145317174,  216861549,  321794049,  466075299,  679380699,  // 70
//    0xFFFFFFFF,


//    // 시즌4 경험치 테이블 수정.
// //  0           1           2           3           4           5           6           7           8           9           // 레벨
//    100,	    240,	    548,	    884,	    1430,	    2060,	    2732,	    3488,	    4286,	    5168,         // 0
//    6680,	    8318,	    10145,	    12098,	    14303,	    16697,	    19343,	    22241,	    25454,	    28982,        // 10
//    32825,	    37046,	    43262,	    50066,	    57626,	    65858,	    74930,	    84926,	    95930,	    114074,       // 20
//    134108,	    156032,	    180224,	    206810,	    236042,	    268172,	    315380,	    367292,	    424412,	    487244,       // 30
//    556292,	    632396,	    716060,	    808124,	    909260,	    1020644,	1173734,	1342154,	1527374,	1771814,      // 40
//    2040698,	2336546,	2716100,	3133580,	3766190,	4462385,	5228240,	6070640,	6997280,	8118158,      // 50
//    9351302,	10707849,	12199828,	13990252,	15960010,	18307147,	20889103,	23947501,	27311512,	32402572,     // 60
//    40803172,	51582577,	65134297,	81905077,	102403477,	145331114,  216875489,  321807989,  466089239,  679394639,    // 70
//    0xFFFFFFFF,                                                   // 70
// 
// 
// 
// //  0           1           2           3           4           5           6           7           8           9           // 레벨
//    2020,       2200,       2500,       2920,       3460,       4060,       4720,       5440,       6220,       7500,       // 0
//    9400,       11700,      14940,      19280,      25580,      34160,      45600,      59320,      75520,      96760,      // 10
//    122360,     152180,     184940,     220640,     259280,     306800,     391600,     527200,     827200,     1208200,    // 20
//    1744200,    2550600,    3536200,    4848200,    7110200,    10054200,   13740200,   18228200,   23578200,   29850200,   // 30
//    37338200,   46194200,   56514200,   68394200,   81930200,   97218200,   114354200,  133434200,  154554200,  177810200,  // 40
//    203298200,  231342200,  262062200,  295578200,  332010200,  371478200,  414582200,  461482200,  512338200,  567310200,  // 50
//    628674200,  695154200,  764634200,  837114200,  912594200,  991074200,  1073154200, 1158834200, 1248114200, 1340994200, // 60
//    1437474200, 1538154200, 1643034200, 1752114200, 1865394200, 0xFFFFFFFF,                                                 // 70
// 
// };

// 각 레벨에서 얻을 수 있는 경험치 테이블.
static KProtectedType< GCEXPTYPE > s_biExpGetTable[] =
{
//  0       1       2       3       4       5       6       7       8       9       // 레벨
    20,     60,     60,     60,     60,     60,     60,     60,     60,     80,     // 0
    100,    100,    120,    140,    180,    220,    260,    280,    300,    360,    // 10
    400,    420,    420,    420,    420,    480,    800,    1200,   2500,   3000,   // 20
    4000,   5600,   6400,   8000,   13000,  16000,  19000,  22000,  25000,  28000,  // 30
    32000,  36000,  40000,  44000,  48000,  52000,  56000,  60000,  64000,  68000,  // 40
    72000,  76000,  80000,  84000,  88000,  92000,  96000,  100000, 104000, 108000, // 50
    116000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, // 60
    120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, // 70
    120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, 120000, // 80
};

// 각 레벨에서 얻을 수 있는 GP 테이블
static KProtectedType< int > s_iGPGetTable[] =
{   // 획득GP   // Level
    5,          // 0
    8,          // 1
    10,         // 2
    13,         // 3
    16,         // 4
    20,         // 5
    22,         // 6
    24,         // 7
    26,         // 8
    29,         // 9
    34,         // 10
    40,         // 11
    44,         // 12
    48,         // 13
    53,         // 14
    60,         // 15
    68,         // 16
    72,         // 17
    78,         // 18
    85,         // 19
    93,         // 20
    98,         // 21
    104,        // 22
    109,        // 23
    114,        // 24
    119,        // 25
    125,        // 26
    135,        // 27
    148,        // 28
    173,        // 29
    200,        // 30
    202,        // 31
    204,        // 32
    206,        // 33
    208,        // 34
    210,        // 35
    212,        // 36
    214,        // 37
    216,        // 38
    218,        // 39
    220,        // 40
    222,        // 41
    224,        // 42
    226,        // 43
    228,        // 44
    230,        // 45
    232,        // 46
    234,        // 47
    236,        // 48
    238,        // 49
    240,        // 50
    242,        // 51
    244,        // 52
    246,        // 53
    248,        // 54
    250,        // 55
    252,        // 56
    254,        // 57
    256,        // 58
    258,        // 59
    260,        // 60
    262,        // 61
    264,        // 62
    266,        // 63
    268,        // 64
    270,        // 65
    272,        // 66
    274,        // 67
    276,        // 68
    278,        // 69
    280,        // 70
    282,        // 71
    284,        // 72
    286,        // 73
    288,        // 74
    290,        // 75
    290,        // 76
    290,        // 77
    290,        // 78
    290,        // 79
    290,        // 80
    290,        // 81
    290,        // 82
    290,        // 83
    290,        // 84
    290,        // 85
    290,        // 86
    290,        // 87
    290,        // 88
    290,        // 89
    290,        // 90
};

// 각 레벨에 대한 경험치 경계값 테이블. 최초의 레벨0의 경험치는 100이다.
static KProtectedType< DWORD > s_dwPetExpTable[] =
{
    //100,        // 0
    104,        // 1
    119,        // 2
    152,        // 3
    232,        // 4
    358,        // 5
    566,        // 6
    845,        // 7
    1241,        // 8
    1733,        // 9
    2377,        // 10
    3142,        // 11
    4094,        // 12
    5192,        // 13
    6512,        // 14
    8003,        // 15
    9751,        // 16
    11695,        // 17
    13931,        // 18
    16388,        // 19
    19172,        // 20
    22202,        // 21
    25594,        // 22
    29257,        // 23
    33317,        // 24
    37673,        // 25
    42461,        // 26
    47570,        // 27
    53146,        // 28
    59068,        // 29
    65492,        // 30
    72287,        // 31
    86795,        // 32
    102251,        // 33
    118685,        // 34
    136127,        // 35
    154607,        // 36
    174155,        // 37
    194801,        // 38
    216575,        // 39
    239507,        // 40
    993269710,    // 41
};

// 이겼을 때 얻을 수 있는 Pet의 경험치의 레벨당 수치입니다.
static KProtectedType< DWORD > s_dwPetWinExpGetTable[] =
{
    4,          // 1
    3,          // 2
    6,          // 3
    9,          // 4
    12,         // 5
    15,         // 6
    18,         // 7
    21,         // 8
    24,         // 9
    27,         // 10
    30,         // 11
    33,         // 12
    36,         // 13
    39,         // 14
    42,         // 15
    45,         // 16
    48,         // 17
    51,         // 18
    54,         // 19
    57,         // 20
    60,         // 21
    63,         // 22
    66,         // 23
    69,         // 24
    72,         // 25
    75,         // 26
    78,         // 27
    81,         // 28
    84,         // 29
    87,         // 30
    90,         // 31
    93,         // 32
    96,         // 33
    99,         // 34
    102,        // 35
    105,        // 36
    108,        // 37
    111,        // 38
    114,        // 39
    117,        // 40
    120,        // 41
};

// 졌을 때 얻을 수 있는 Pet의 경험치의 레벨당 수치입니다.
static KProtectedType< DWORD > s_dwPetLoseExpGetTable[] =
{
    2,          // 1
    2,          // 2
    2,          // 3
    3,          // 4
    3,          // 5
    4,          // 6
    5,          // 7
    5,          // 8
    6,          // 9
    6,          // 10
    7,          // 11
    8,          // 12
    8,          // 13
    9,          // 14
    9,          // 15
    10,         // 16
    11,         // 17
    11,         // 18
    12,         // 19
    12,         // 20
    13,         // 21
    14,         // 22
    14,         // 23
    15,         // 24
    15,         // 25
    16,         // 26
    17,         // 27
    17,         // 28
    18,         // 29
    18,         // 30
    19,         // 31
    20,         // 32
    20,         // 33
    21,         // 34
    21,         // 35
    22,         // 36
    23,         // 37
    23,         // 38
    24,         // 39
    24,         // 40
    25,         // 41
};

// 펫 Exp를 레벨로 환산한다.
unsigned char GetPetLevel( DWORD dwExp )
{
    // 경험치-레벨 테이블을 참조하여 현재 경험치에 대한 레벨을 얻어낸다.
	int i;
    for( i = 0; i < sizeof( s_dwPetExpTable ) / sizeof( s_dwPetExpTable[0] ); i++ )
    {
        if( i == 41 )
            return 40;

        if ( dwExp < s_dwPetExpTable[i] )
        {
            return i;
        }
    }
    return i;
}

float GetPetExpPercent( DWORD dwExp )
{
    int iLevel = GetPetLevel( dwExp );

    if( iLevel >= 40 )
        return 100.0f;

    DWORD dwTargetExp = s_dwPetExpTable[ iLevel ];
    DWORD dwPrevExp = 100;

    if( iLevel != 0 )
        dwPrevExp = s_dwPetExpTable[ iLevel - 1 ];

    return (float)( dwExp - dwPrevExp ) * 100.0f / (float)( dwTargetExp - dwPrevExp );    
}

DWORD Pet_Exp_For_Next( DWORD Exp )
{
    for ( int i = 0; i < sizeof( s_dwPetExpTable ) / sizeof( s_dwPetExpTable[0] ); ++i )
    {
        if( i == 40 )
            return 0;

        if ( Exp < s_dwPetExpTable[i] )
        {
            return s_dwPetExpTable[i] - Exp;
        }
    }
    return 0;
}

// Exp를 레벨로 환산한다.
unsigned char Exp_2_Level( GCEXPTYPE biExp_ )
{
    // 경험치-레벨 테이블을 참조하여 현재 경험치에 대한 레벨을 얻어낸다.
    int i;

    for ( i = 0; i < (int)mapExpTable.size(); ++i )
    {
        if ( biExp_ < mapExpTable[i] )
        {
            if( i > g_kGlobalValue.GetMaxLevel() )
                i = g_kGlobalValue.GetMaxLevel();
            return i;
        }
    }
    if( i > g_kGlobalValue.GetMaxLevel() )
        i = g_kGlobalValue.GetMaxLevel();


    return i; // 75
}

GCEXPTYPE Level_2_Exp( int iLevel_ )
{
    if( iLevel_ > 0 && g_kGlobalValue.GetMaxLevel() > iLevel_ )
        return mapExpTable[iLevel_];
    else
    {
        if( iLevel_ <= 0 )
            return mapExpTable[0];
        else
            return mapExpTable[g_kGlobalValue.GetMaxLevel()-1];
    }
}

// 클라이언트에서는 사용되지 않지만 서버에서 GP 변화의 유효성을 체크할 때 참조한다.
static bool IsCorrectGP( GCEXPTYPE biNowExp, int iNowGP, int iChangeValue )
{
    int iResult = iNowGP + iChangeValue;
    int iLevel  = Exp_2_Level( biNowExp );
    int iBound  = 0;

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > g_kGlobalValue.GetMaxLevel() )
        iLevel = g_kGlobalValue.GetMaxLevel();

    // 바운드 계산
    // 오픈테섭의 보상은 5배이다.
    // 몬스터 원정대 가도센5마리의 최대 획득 경험치는 12배임
    // GP 아이템(게임 중 나오는 금화) 획득으로 얻을 수 있는 최대 보상은 30원( = 최대 3개 * 10원)이다.

#   ifdef _OPEN_TEST_       // 넷마블 테스트 서버인 경우
    {
        iBound = static_cast<int>( static_cast<float>( s_iGPGetTable[iLevel] + 30 ) * 5.0f * 12.0f );
    }
#   else // !_OPEN_TEST_    // 일반 서버인 경우 또는 KOG 사내 테스트인 경우
    {
        iBound = static_cast<int>( static_cast<float>( s_iGPGetTable[iLevel] + 30 ) * 12.0f );
    }
#   endif // _OPEN_TEST_


    // 결과가 현재보다 작아지거나 변화치가 최대 변화치 바운드를 벗어나면 잘못된 경우이다.
    if ( iResult < iNowGP || iBound < iChangeValue )
    {
        return false; // oh, no!
    }

    return true; // ok!
}

// 클라이언트에서는 사용되지 않지만 서버에서 EXP 변화의 유효성을 체크할 때 참조한다.
static bool IsCorrectExp( GCEXPTYPE biNowExp_, int iChangeValue, int iPlayerIndex )
{
    GCEXPTYPE iResult = biNowExp_ + iChangeValue;
    int iLevel  = Exp_2_Level( biNowExp_ );
    int iBound  = 0;

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > g_kGlobalValue.GetMaxLevel() )
        iLevel = g_kGlobalValue.GetMaxLevel();

    // 바운드 계산
    // 오픈테섭의 보상은 5배이다.
    // 몬스터 원정대 최대 보상(가도센 5마리)은 12배이다. (퀘스트의 최대 보상은 4.5)
    // 넷마블 PC방 보상은 2배이다.
    // 길드전의 최대 보상은 2배이다. (줄여가고 있다. 현재 1.7)
    // 거북이 등껍질의 보상은 2배이다.
    // 즉, 최대는 5.0 * 5.0f * 2.0 * 2.0 * 2.0

    // NOTE : 넷마블 PC방일 경우 경험치를 2배로 준다.
    if ( g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.cIsNetmarblePCRoom == 1 ) // 해당 유저가 넷마블 PC방일 경우 경험치 2배
    {
#       ifdef _OPEN_TEST_       // 넷마블 테스트 서버인 경우
        {
            iBound = static_cast<int>( static_cast<float>( s_biExpGetTable[iLevel] ) * 12.0f * 2.0f * 2.0f * 2.0f * 5.0f );
        }
#       else // !_OPEN_TEST_    // 일반 서버인 경우 또는 KOG 사내 테스트인 경우
        {
            iBound = static_cast<int>( static_cast<float>( s_biExpGetTable[iLevel] ) * 12.0f * 2.0f * 2.0f * 2.0f );
        }
#       endif // _OPEN_TEST_
    }    
    else // 넷마블 PC방이 아닌 경우
    {
#       ifdef _OPEN_TEST_       // 넷마블 테스트 서버인 경우
        {
            iBound = static_cast<int>( static_cast<float>( s_biExpGetTable[iLevel] ) * 12.0f * 2.0f * 2.0f * 5.0f );
        }
#       else // !_OPEN_TEST_    // 일반 서버인 경우 또는 KOG 사내 테스트인 경우
        {
            iBound = static_cast<int>( static_cast<float>( s_biExpGetTable[iLevel] ) * 12.0f * 2.0f * 2.0f );
        }
#       endif // _OPEN_TEST_
    }

    // 결과가 현재보다 작아지거나 변화치가 최대 변화치 바운드를 벗어나면 잘못된 경우이다.
    if ( iResult < biNowExp_ || iBound < iChangeValue )
    {
        return false; // oh, no!
    }

    return true; // ok!
}

// 내 EXP에 맞는 EXP 상승치를 알려준다.
float Get_MyGetExp( int iPlayerIndex, char cCharType, bool bLimit30Lv, float fRatio_ )
{
    //대회용 서버에서는 변화하지 않음
#if !defined( NATION_KOREA )
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) || g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ))
#else 
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
#endif
        return 0;

    // 자신의 레벨을 확인한다.
    int iLevel = Exp_2_Level( GetCharacterExp( g_pGameBoard->m_pStartingMember[iPlayerIndex].m_mapStartExp, cCharType ) );

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > g_kGlobalValue.GetMaxLevel() )
        iLevel = g_kGlobalValue.GetMaxLevel();

    // 30레벨 이상 보상을 일정하게 하는 옵션이 켜져 있다면
    if ( bLimit30Lv )
    {
        if ( iLevel > 30 )
            iLevel = 30;
    }
    // 패널티를 적용 시키지 않아야 하는 레벨이면 패널티를 적용 시키지 않는다.
    if ( iLevel < LIMIT_PENALTY_LEVEL && fRatio_ < 0.0f )
        return 0.0f;

    float fPercent = 1.0f;
    float fOutRatioGp = 1.0f; // 사용하지 않는다.
    GetPlayerExpGpRatio( iPlayerIndex, fPercent, fOutRatioGp );

    // 경험치가 깍일 경우에는 이벤트등으로 인한 뻥튀기의 적용을 받지 않게 한다.
    if ( fRatio_ < 0.0f )
        fPercent = 1.0f;

    return (s_biExpGetTable[iLevel] * fPercent * fRatio_ );
}

// 내 EXP에 맞는 GP 상승치를 알려준다.
float Get_MyGetGP( int iPlayerIndex, bool bLimit30Lv, float fRatio_ )
{
    //대회용 서버에서는 변화하지 않음
#if !defined( NATION_KOREA )
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) || g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE) )
#else
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
#endif
        return 0;

    // 자신의 레벨을 확인한다.
    int iLevel = Exp_2_Level( GetCharacterExp( g_pGameBoard->m_pStartingMember[iPlayerIndex].m_mapStartExp, g_MyD3D->MyPlayer[iPlayerIndex]->GetCurrentChar().iCharType ) );

    // 레벨 최대치를 벗어나지 않도록 한다.
    if ( iLevel > g_kGlobalValue.GetMaxLevel() )
        iLevel = g_kGlobalValue.GetMaxLevel();

    // 30레벨 이상 보상을 일정하게 하는 옵션이 켜져 있다면
    if ( bLimit30Lv )
    {
        if ( iLevel > 30 )
            iLevel = 30;
    }

    // 게임 중 획득한 GP 아이템이 3개가 넘지 않도록 한다.
    if ( g_MyD3D->MyPlayer[iPlayerIndex]->Item_Score > 3 )
    {
        g_MyD3D->MyPlayer[iPlayerIndex]->Item_Score = 3;
    }

    // 패널티를 적용 시키지 않아야 하는 레벨이면 패널티를 적용 시키지 않는다.
    if ( iLevel < LIMIT_PENALTY_LEVEL && fRatio_ < 0.0f )
        return 0.0f;

    float fPercent = 1.0f;
    float fOutRatioExp = 1.0f; // 사용하지 않는다.
    GetPlayerExpGpRatio( iPlayerIndex, fOutRatioExp, fPercent );
    // GP가 깍일 경우에는 이벤트등으로 인한 뻥튀기의 적용을 받지 않게 한다.
    if ( fRatio_ < 0.0f )
        fPercent = 1.0f;
#   ifdef _OPEN_TEST_
    {
        return (float)( s_iGPGetTable[iLevel]*fPercent*fRatio_ + g_MyD3D->MyPlayer[iPlayerIndex]->Item_Score * 10 * 5.0f );
    }
#   else // !_OPEN_TEST_
    {
        return (float)( s_iGPGetTable[iLevel]*fPercent*fRatio_ + g_MyD3D->MyPlayer[iPlayerIndex]->Item_Score * 10 );
    }
#   endif // _OPEN_TEST_
}

GCEXPTYPE GetCharacterExp( std::map< char, GCEXPTYPE > mapExp, char cCharType )
{
    std::map< char, GCEXPTYPE >::iterator mit = mapExp.find( cCharType );
    if( mit == mapExp.end() )
        return CHAR_EXP_INVALID;

    return mit->second;
}

void GetPlayerExpGp( int iPlayerIdx, char cCharType, int& dwOutExp, int& dwOutGp )
{
    // 자신의 레벨을 확인한다.
    int iLevel = Exp_2_Level( GetCharacterExp( g_pGameBoard->m_pStartingMember[iPlayerIdx].m_mapStartExp, cCharType ) );
    dwOutGp = (DWORD)s_iGPGetTable[iLevel];
    dwOutExp = (DWORD)s_biExpGetTable[iLevel];
}

void GetPlayerExpGpRatio( int iPlayerIdx, float& fOutRatioExp, float& fOutRatioGp )
{
    PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerIdx];
    fOutRatioExp = 1.0f;
    fOutRatioGp  = 1.0f;

    //====================================================================================
    // 2007.02.22 : Asirion
    // 슬라임 큐큐투구 때문에 죽더라 일단 안죽게 수정
    if ( pPlayer->m_kUserInfo.bLive == false )
        return;

    // NOTE : 넷마블 PC방일 경우 경험치를 2배로 준다.
    if ( pPlayer->m_kUserInfo.cIsNetmarblePCRoom == 1 )
    {
        fOutRatioExp *= 2.0f;
    }

//     if ( SiKGCChannelManager()->CheckChannel( CT_GUILD ) && GC_GMC_GUILD_BATTLE == SiKGCRoomManager()->GetGameModeCategory() )
//     {
//         //창호형의 2005년 6월 8일자 요청에 따라 서서히 줄여갑니다.
//         //fPercent *= 2.0f;
//         fOutRatioExp *= 1.7f;
//     }

    //==============================================================================================
#   ifdef _OPEN_TEST_       // 넷마블 테스트 서버인 경우    
    fOutRatioExp *= 5.0f;
    fOutRatioGp *= 5.0f;
#    endif

    // 방원이 가지고 있으면 모든 애들이 보너스 적용받는 녀석들

    bool bFoundJewel = false;
	bool bFoundFlowers = false;
	bool bFoundFriendShipRing = false;
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
            continue;

        if( g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), JEWEL_OF_QUEEN ) == true 
            || g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), JEWEL_OF_QUEEN2 ) == true
            )
        {
            bFoundJewel = true;
		}

		if( g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), BRUNCH_OF_FLOWERS ) == true )
		{
			bFoundFlowers = true;
		}

		if( g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING )
			|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ELESIS )
			|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LIR )
			|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_ARME )
			|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_LAS )
			|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RYAN )
			|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_RONAN )
			|| g_MyD3D->m_kItemMgr.CheckEquipItemForItemID( &g_MyD3D->MyPlayer[i]->m_kUserInfo.GetCurrentChar(), FRIENDSHIP_RING_AMY )
			)
		{
			bFoundFriendShipRing = true;
		}

		if( bFoundFlowers && bFoundJewel && bFoundFriendShipRing )
            break;
        }

    if( bFoundJewel == true )
    {
        fOutRatioExp *= 2.0f;
        fOutRatioGp *= 2.0f;
    }

	if( bFoundFlowers == true )
    {
        fOutRatioExp *= 2.0f;
        fOutRatioGp *= 2.0f;
    }

	if( bFoundFriendShipRing == true )
	{
		fOutRatioExp *= 2.0f;
		fOutRatioGp *= 2.0f;
	}

    if( SiKGCChannelManager()->IsChannelBonus() ) // 계급 채널에서 플레이 했다면 경치 10% 추가
    {
        fOutRatioExp *= g_fExpTenBonus;
    }
}

void GetPlayerExpGpExtra( int iPlayerIdx, DWORD& dwOutExp, DWORD& dwOutGp )
{
    dwOutExp = 0;

    if( g_MyD3D->MyPlayer[iPlayerIdx]->Item_Score > 3 )
        g_MyD3D->MyPlayer[iPlayerIdx]->Item_Score = 3;

    dwOutGp = g_MyD3D->MyPlayer[iPlayerIdx]->Item_Score * 10;    

}

void Auto_BoundCheckGPandEXP( int iPlayerIndex )
{
    int iResultGP = g_MyD3D->MyPlayer[iPlayerIndex]->Target_Score;

    // GP의 바운드는 최저 0, 최고 MAX_GP_VALUE이다.
    if      ( iResultGP < 0 )            iResultGP = 0;
    else if ( iResultGP > MAX_GP_VALUE ) iResultGP = MAX_GP_VALUE;

    // 조절된 GP로 다시 셋팅한다.
    g_MyD3D->MyPlayer[iPlayerIndex]->Target_Score = iResultGP;
}

GCEXPTYPE Exp_For_Next( GCEXPTYPE biExp )
{
    for ( int i = 0; i < (int)mapExpTable.size(); ++i )
    {
        if( i == g_kGlobalValue.GetMaxLevel() )
            return 0;

        if ( biExp < mapExpTable[i] )
        {
            return mapExpTable[i] - biExp;
        }
    }
    return 0;
}

float Exp_For_Percent( GCEXPTYPE biExp )
{
    for ( int i = 0; i < (int)mapExpTable.size(); ++i )
    {
        if( i == g_kGlobalValue.GetMaxLevel() )
            return 0;

        if ( biExp < mapExpTable[i] )
        {
            GCEXPTYPE biTankExp, biMyExp;
            if( 0 == i )
            {
                biMyExp = biExp - 0;
                biTankExp = mapExpTable[i] - 0;
            }
            else
            {
                biMyExp = biExp - mapExpTable[i-1];
                biTankExp = mapExpTable[i] - mapExpTable[i-1];
            }
            return gcround( (float(biMyExp)/float(biTankExp)*100.0f), 2 );
        }
    }
    return 0;
}

bool MakeGameResult( KGameResultIn& kgr, int iPlayer, bool bExitGame )
{
    if( !g_MyD3D->MyPlayer[iPlayer]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) && !g_MyD3D->MyPlayer[iPlayer]->m_kUserInfo.CheckState( GC_RUS_READY ) )
        return false;

    // 패킷 초기화
    kgr.m_strLogin = g_MyD3D->MyPlayer[iPlayer]->GetPlayerLoginID();
    kgr.m_dwUID = g_MyD3D->MyPlayer[iPlayer]->m_kUserInfo.dwUID;
    kgr.m_nBaseGP = 0;
    kgr.m_nTotalRecvGP = 0;
    kgr.m_iNumKills = 0;
    kgr.m_iNumDies = 0;
    kgr.m_bWin = false;
    kgr.m_nScore = 0;
    kgr.m_iContPoint = 0;
    kgr.m_vecKillCoin.clear();
	kgr.m_kHactkData.m_vecAtkData.clear();
	kgr.m_kHactkData.m_vecIncAPRatio.clear();
	kgr.m_kHactkData.m_bUseAllKill = false;
	kgr.m_kHactkData.m_bMonsterHPHack = false;
    kgr.m_nRegenCount = 0;

    // 유저의 스킬(스킬모션) 사용 횟수와 현재 장착한 슬롯에 스킬 정보를 기록한다.
    if( g_MyD3D->MyPlayer[iPlayer]->IsLocalPlayer() ) {
        kgr.m_kUserSkillStat.m_mapSkillUseCount = g_MyD3D->MyPlayer[iPlayer]->m_mapStatsSkillMotionList;

        for( int i = 0; i < static_cast< int >( g_MyD3D->MyPlayer[iPlayer]->m_vecHotKeySkill.size() ); i++ ) {
            if( i == MAX_SLOT_COUNT ) {
                break;
            }

            GCHotKeySkill kSlotSkill = g_MyD3D->MyPlayer[iPlayer]->m_vecHotKeySkill[ i ];
            if( kSlotSkill.m_eSkill != SID_DIO1_EMPTY_SLOT ) {                
                kgr.m_kUserSkillStat.m_mapSkillSlotInfo.insert( std::make_pair( i, static_cast< int >( kSlotSkill.m_eSkill ) ) );
            }            
        }        
    }    

    kgr.m_prUseSkillCount = g_kGlobalValue.m_prHybridSpecialUseCount;
    g_kGlobalValue.m_prHybridSpecialUseCount = std::make_pair( 0, 0 );

    // 달성과제 정보를 기록한다.
    if( g_MyD3D->MyPlayer[iPlayer]->IsLocalPlayer() ) 
    {
        int iCharType = g_MyD3D->MyPlayer[iPlayer]->m_kUserInfo.GetCurrentCharType();
        int iCharLevel = g_MyD3D->MyPlayer[iPlayer]->GetCurrentCharLevel();
        int iCharPromotion = g_MyD3D->MyPlayer[iPlayer]->GetPromotion( iCharType );
        kgr.m_kDungeonSubjectStat.m_nCharType = iCharType;
        kgr.m_kDungeonSubjectStat.m_nCharLevel = iCharLevel;
        kgr.m_kDungeonSubjectStat.m_nCharPromotion = iCharPromotion;
        kgr.m_kDungeonSubjectStat.m_mapSubjectClearInfo = SiKGCSubjectManager()->m_kSubjectStats.m_mapSubjectClearInfo;
        SiKGCSubjectManager()->InitStats();
    }

    //유저별 랭크 방장이 보낸다.
    kgr.m_kUserControlInfoData.m_nRank = g_pkQuestManager->GetPlayerRank( iPlayer );

    // 미션조건체크용 데이터
    kgr.m_kUserControlInfoData.m_nBeAttackedCount = g_MyD3D->MyPlayer[iPlayer]->GetControlInfoBeAttackedCount();
    kgr.m_kUserControlInfoData.m_nComboCount = g_MyD3D->MyPlayer[iPlayer]->GetControlInfoComboCount();
    kgr.m_kUserControlInfoData.m_nBackAttackCount = g_MyD3D->MyPlayer[iPlayer]->GetControlInfoBackAttackCount();
    kgr.m_kUserControlInfoData.m_mapUseSkillCount = g_MyD3D->MyPlayer[iPlayer]->GetControlInfoSkill();
    kgr.m_kUserControlInfoData.m_mapUsePositionCount = g_MyD3D->MyPlayer[iPlayer]->GetControlInfoPotion();
    g_MyD3D->MyPlayer[iPlayer]->ClearControlInfo();

	//던전해킹체크 - 스테이지별로 로딩된 플레이어들의 공격력을 패킷에 담는다.
	if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
	{
		if( g_MyD3D->m_pkQuestGameMgr->m_vecCurAtk.size() == MAX_PLAYER_NUM )
			kgr.m_kHactkData.m_vecAtkData = g_MyD3D->m_pkQuestGameMgr->m_vecCurAtk[iPlayer];

		if( g_MyD3D->m_pkQuestGameMgr->m_vecIncAtkRatio.size() == MAX_PLAYER_NUM )
			kgr.m_kHactkData.m_vecIncAPRatio = g_MyD3D->m_pkQuestGameMgr->m_vecIncAtkRatio[iPlayer];

		//해킹감지된 부분 패킷의 값에 대입
		kgr.m_kHactkData.m_bUseAllKill = g_MyD3D->m_pkQuestGameMgr->GetHackedDungeon();
		kgr.m_kHactkData.m_bMonsterHPHack = g_MyD3D->m_pkQuestGameMgr->GetMonsterHPHack();

        // 던전 플레이 통계
#if defined( DUNGEONPLAYSTATE )
        // 보스 스테이지에서는 NextStage가 없기때문에 클리어타임을 남길 수가 없다.
        // 그래서 여기에서 보스스테이지에 대한 클리어타임을 남긴다.
        if ( false == bExitGame )
            g_kGlobalValue.AddDungeonPlayState( KGCGlobal::DUNPLAY_STATE_CLEARTIME );

        //부활한 횟수
        kgr.m_nRegenCount = g_MyD3D->MyPlayer[iPlayer]->GetPlayerRegeonCount();
        g_MyD3D->MyPlayer[iPlayer]->ClearRegenCount();

        if ( g_MyD3D->MyPlayer[iPlayer]->m_kUserInfo.bHost )
        {
            bool bSendState = true;
            if ( bExitGame )
            {
                // 강제 종료시 마지막 플레이어가 아니면 SendState에 통계기록을 담지말거라
                if ( 1 != SiKGCRoomManager()->GetRoomInfo().usUsers )
                    bSendState = false;
            }

            if( bSendState )
            {
                // 던전 플레이 통계
                kgr.m_mapDungeonPlayState = g_kGlobalValue.m_mapDungeonPlayState;

#if defined( CHAR_ABILITY_STATE )
                // 캐릭터 능력치 통계
                kgr.m_vecDungeonCharStat = g_MyD3D->MyPlayer[iPlayer]->m_vecCharAbilityState;
#endif

                // 던전 퍼포먼스 통계
                {
                    int iPlayMember = 0;
                    for ( int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop )
                    {
                        if( GC_RUS_PLAYING == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.nUserState )
                            iPlayMember++;
                    }

                    kgr.m_kUserLoadingStat.m_nGameMode = static_cast<int>(SiKGCRoomManager()->GetGameMode());
                    kgr.m_kUserLoadingStat.m_nPartyUserNum = g_kGlobalValue.m_kDunPerforState.iLoadingStarttingMember;
                    kgr.m_kUserLoadingStat.m_nLoadingOutUserCnt = g_kGlobalValue.m_kDunPerforState.iLoadingBannedCnt;
                    kgr.m_kUserLoadingStat.m_nAvgLoadingComplete = g_kGlobalValue.m_kDunPerforState.dwLoadingTime;
                    kgr.m_kUserLoadingStat.m_nPlayingOutUserCnt = g_kGlobalValue.m_kDunPerforState.GetPlayBannedCnt( iPlayMember );

                    // FPS는 평균내서 한껀으로 보내자.
                    int iCount = 0;
                    float fAvgFPS = 0.0f;
                    std::map<int, std::pair<DWORD, double>>::iterator mapIter = g_kGlobalValue.m_kDunPerforState.mapAvgFPS.begin();
                    for(; mapIter != g_kGlobalValue.m_kDunPerforState.mapAvgFPS.end(); ++mapIter)
                    {
                        fAvgFPS += static_cast<float>(mapIter->second.second) / static_cast<float>(mapIter->second.first);
                        ++iCount;
                    }

                    if( 0 < iCount )
                    {
                        fAvgFPS /= iCount;
                    }

                    kgr.m_kUserLoadingStat.m_nAvgFPS = static_cast<int>(fAvgFPS);

                    //FILE *pFile = fopen("AvgFPSInfo.txt", "at");
                    //fprintf(pFile, "GameMode : %d,\tAvgFPS : %f\n", SiKGCRoomManager()->GetGameMode(), fAvgFPS);
                    //fclose(pFile);

                    g_kGlobalValue.m_kDunPerforState.Init();
                }

                // 몬스터 상태 정보
                {
                    int iPlayerDeathCnt = 0;
                    std::map< KDungeonDetail, KDungeonPlayState >::iterator mapDeathIter = g_kGlobalValue.m_mapDungeonPlayState.begin();
                    for(; mapDeathIter != g_kGlobalValue.m_mapDungeonPlayState.end(); ++mapDeathIter)
                    {
                        iPlayerDeathCnt += mapDeathIter->second.m_usDeath_Cnt;
                    }

                    g_kGlobalValue.m_kMonsterTypeStat.m_nTotalDeathCount = iPlayerDeathCnt;
                    kgr.m_kMonsterTypeStat = g_kGlobalValue.m_kMonsterTypeStat;
                }
            }
        }
#endif

#if defined( USE_CAMERA_ZOOM_STATE )
			// 강제 종료가 아니거나 강제종료이고, 남은 플레이어가 혼자라면 통계를 남겨라!!
		if( false == bExitGame || (bExitGame && 1 == SiKGCRoomManager()->GetRoomInfo().usUsers) )
		{
			g_MyD3D->MyPlayer[iPlayer]->AddCameraZoomState( 0.0f, 0.0f ); 
			kgr.m_kUserViewGradeTime.m_mapViewGradeTime.clear();
			std::map<float, std::pair<DWORD, DWORD> >::iterator mapIter = g_MyD3D->MyPlayer[iPlayer]->m_mapCameraZoomState.begin();
			for(; mapIter != g_MyD3D->MyPlayer[iPlayer]->m_mapCameraZoomState.end(); ++mapIter)
			{
				kgr.m_kUserViewGradeTime.m_mapViewGradeTime.insert( std::make_pair( mapIter->first, mapIter->second.second ));
			}
		}
#endif
	}
	// 캐릭터 통계 데이터 제거
    g_MyD3D->MyPlayer[iPlayer]->m_vecCharAbilityState.clear();

	#if defined( USE_CAMERA_ZOOM_STATE )
		g_MyD3D->MyPlayer[iPlayer]->m_mapCameraZoomState.clear();
	#endif

    if( GC_GMC_DUNGEON != SiKGCRoomManager()->GetGameModeCategory() )
    {
        kgr.m_cCurrentPromotion = g_MyD3D->MyPlayer[iPlayer]->GetSelectedPromotionValue();
    }
    
    if( GC_GMC_DUNGEON != SiKGCRoomManager()->GetGameModeCategory() )
    {
        kgr.m_cCurrentPromotion = g_MyD3D->MyPlayer[iPlayer]->GetSelectedPromotionValue();
    }

	if( GC_GMC_TAG_MATCH == SiKGCRoomManager()->GetGameModeCategory() )
	{
		std::map< int, TagCharacterInfo >::iterator itTagChar = g_MyD3D->MyPlayer[iPlayer]->GetTagInfoMap().begin();
		for(;itTagChar != g_MyD3D->MyPlayer[iPlayer]->GetTagInfoMap().end();++itTagChar)
		{
			KCharacterExpInfo kCharExpInfo;
			kCharExpInfo.Clear();
			kCharExpInfo.m_cCharType = itTagChar->second.m_iCharType;
			kCharExpInfo.m_cCharJob = itTagChar->second.Extra_Char_Num / GC_CHAR_NUM;
			kgr.m_vecCharExp.push_back( kCharExpInfo );
		}
	}
	else
	{
		KCharacterExpInfo kCharExpInfo;
		kCharExpInfo.Clear();
		kCharExpInfo.m_cCharType = (char)g_MyD3D->MyPlayer[iPlayer]->GetCurrentChar().iCharType;
		kCharExpInfo.m_cCharJob = g_MyD3D->MyPlayer[iPlayer]->GetPromotion(g_MyD3D->MyPlayer[iPlayer]->GetCurrentChar().iCharType);
		kgr.m_vecCharExp.push_back( kCharExpInfo );
	}

	// 현재 게임 한판에서 죽인수 / 죽은수를 던집니다.
	kgr.m_iNumKills = g_pGameBoard->m_pStartingMember[iPlayer].How_Kill;
	kgr.m_iNumDies = g_pGameBoard->m_pStartingMember[iPlayer].How_Die;
	kgr.m_vecKillCoin = g_MyD3D->MyPlayer[iPlayer]->m_vecKillCoinInfo;

    // 소멸의 탑 클리어 기록
    if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
    {
        kgr.m_mapDungeonPersonalRecord = g_pkQuestManager->m_vecDungeonPersonalRecord[iPlayer];
    }

	if( bExitGame == true )      // 게임 도중 나감
		return true;

	if ( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() ) // 퀘스트 모드
	{
		kgr.m_bWin = g_pkQuestManager->m_bWin;
	}
	else
	{
		kgr.m_bWin = g_pGameBoard->m_pStartingMember[iPlayer].IsWin;
	}

	if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_GUILD_BATTLE ) // 길드전이라면
	{
		kgr.m_iContPoint    = 2;
		if ( g_pGameBoard->m_pStartingMember[iPlayer].IsWin )
		{
			kgr.m_iContPoint  = int( 10.0f * g_pGameBoard->m_pStartingMember[iPlayer].fGuildPointPercent );
		}
	}
	else // 일반 채널이라면
	{
		kgr.m_iContPoint    = 0;
	}

	// 점령전 통계
	if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA ) // 점령전이라면
	{
		kgr.m_mapItemUseCount = g_MyD3D->MyPlayer[iPlayer]->GetDotaStats().m_mapItemUseCount;
		kgr.m_mapItemBuyCount = g_MyD3D->MyPlayer[iPlayer]->GetDotaStats().m_mapItemBuyCount;
		kgr.m_mapZoneDeathCount = g_MyD3D->MyPlayer[iPlayer]->GetDotaStats().m_mapZoneDeathCount;
		kgr.m_mapDeathType = g_MyD3D->MyPlayer[iPlayer]->GetDotaStats().m_mapDeathType;
	}

	return true;
}
// 서버에게 EXP등의 내 플레이어 정보를 쓸것을 요청한다.
void MakeAllGameResult( KEndGameReq& kNot )
{
    // 게임이 끝날때 보스 스테이지에서먹은 GP를 서버에게 알린다 ㅠ
    // 다음 스테이지 로드 할때 GP 먹은걸 알리는데... 보스 스테이지는 다음이 없자나 ㅠ 
    // 이전 스테이지에서 먹은 gp들을 서버에 알린다.
    if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
    {
        if ( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
        {
            SiKP2P()->GetInstance()->Send_InfinityDungeonRewardGPNot( g_pkQuestManager->m_setGpUID );
        }
        else
        {
            SiKP2P()->GetInstance()->Send_DungeonRewardGPNot( g_pkQuestManager->m_setGpUID );
        }

        g_pkQuestManager->m_setGpUID.clear();
    }

	typedef std::map<DWORD,DWORD>::value_type DropResult;
    kNot.m_vecGameResult.clear();	
    for ( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
    {
        KGameResultIn kgr;

        if( MakeGameResult( kgr, i ) == true )
        {
            kNot.m_vecGameResult.push_back( kgr );
        }
    }

    std::vector<int> vtKillMonTGID = g_pGameBoard->GetMonKillTGID();
    kNot.m_setKillMonster.clear();
    for(int i=0;i<(int)vtKillMonTGID.size();++i)
    {
        kNot.m_setKillMonster.insert( vtKillMonTGID[i] );
    }
	
    if ( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() )
    {
        kNot.m_iQuestEndingStage  = SiKGCRoomManager()->GetDungeonLevel();
        kNot.m_setClearStageCount = g_MyD3D->m_pkQuestGameMgr->GetClearStageSet();

        kNot.m_mapRouletteList.clear();
        kNot.m_mapRouletteList = g_MyD3D->m_pkQuestGameMgr->m_mapRouletteList;
		kNot.m_dwElapsedSec = g_pGameBoard->GetPlayTimeInFrame()/60;

        // 룰렛 WaitList에서 정보를 받지 못한 녀석은 모두 참여시켜버린다.
        for(int i=0;i<(int)g_pkQuestManager->m_vecRouletteWaitList.size();++i)
        {
            std::map< DWORD, std::vector<DWORD> >::iterator itRouletteList = kNot.m_mapRouletteList.begin();
            for( ; itRouletteList != kNot.m_mapRouletteList.end() ; itRouletteList++)
            {
                itRouletteList->second.push_back( g_pkQuestManager->m_vecRouletteWaitList[i] );
            }
        }

        kNot.m_mapDropResult.swap( SiKGCDropItemManager()->GetObtainItemList() );
        kNot.m_setDropGPUID.swap( SiKGCDropItemManager()->GetObtainGpList() );
    }
    else
    {
        kNot.m_iQuestEndingStage  = 0;
	}

    // 캐릭터 통계
    std::map< std::pair<int, int>, KGCGlobal::CHARACTERSTATS >::iterator mit = g_kGlobalValue.m_mapCharacterStats.begin();
    for(; mit != g_kGlobalValue.m_mapCharacterStats.end(); ++mit)
    {
        std::pair<int,int> prKey;
        KCharacterStats Data;
        prKey.first = mit->first.first;
        prKey.second = mit->first.second;
        Data.m_nAccCount = mit->second.iAccCount;
        Data.m_nModeType = mit->second.iGameMode;
        kNot.m_mapCharStats.insert( std::make_pair( prKey, Data) );
    }
}

// 각 각 다르계 계산 할 수도 있을거 같아서 2개를 두었다.
#define                 PANELTY_LEVEL_GAP               5
float GetExpGpRatio( int iKillPlayerIndex_, int iKilledPlayerIndex_ ) // 배틀 아레나에서 사용 한다.
{
    //Player가 죽었다는 Message를 날리고 게임에서 나가 버리면 
    //다른 Player들이 Message를 받는 순간 보낸녀석이 게임에서 없을 수 있다.
    //이때 iKillPlayerIndex_나 iKilledPlayerIndex_가 7이 될 수 있다.
    if ( iKillPlayerIndex_ >= MAX_PLAYER_NUM || iKilledPlayerIndex_ >= MAX_PLAYER_NUM )
        return EXP_RATIO_KILL_SAME_LEVEL;
    int iKillPlayerLevel = g_MyD3D->MyPlayer[iKillPlayerIndex_]->m_kUserInfo.GetCurrentChar().iLevel; // 죽인사람.
    int iKilledPlayerLevel = g_MyD3D->MyPlayer[iKilledPlayerIndex_]->m_kUserInfo.GetCurrentChar().iLevel; // 죽은사람.
    if ( PANELTY_LEVEL_GAP <= iKillPlayerLevel - iKilledPlayerLevel )   // 죽인놈 렙이 더 높다.
    {
        return EXP_RATIO_KILL_LOW_LEVEL;
    }
    else if ( PANELTY_LEVEL_GAP  <= iKilledPlayerLevel - iKillPlayerLevel )
    {
        return EXP_RATIO_KILL_HIGH_LEVEL;
    }
    return EXP_RATIO_KILL_SAME_LEVEL;
}
float GetPaneltyExpGpRatio( int iKillPlayerIndex_, int iKilledPlayerIndex_ ) // 배틀 아레나에서 사용 한다.
{
    //Player가 죽었다는 Message를 날리고 게임에서 나가 버리면 
    //다른 Player들이 Message를 받는 순간 보낸녀석이 게임에서 없을 수 있다.
    //이때 iKillPlayerIndex_나 iKilledPlayerIndex_가 7이 될 수 있다.
    if ( iKillPlayerIndex_ >= MAX_PLAYER_NUM || iKilledPlayerIndex_ >= MAX_PLAYER_NUM )
        return EXP_RATIO_KILL_SAME_LEVEL;
    int iKillPlayerLevel = g_MyD3D->MyPlayer[iKillPlayerIndex_]->m_kUserInfo.GetCurrentChar().iLevel; // 죽인사람.
    int iKilledPlayerLevel = g_MyD3D->MyPlayer[iKilledPlayerIndex_]->m_kUserInfo.GetCurrentChar().iLevel; // 죽은사람.
    if ( PANELTY_LEVEL_GAP <= iKillPlayerLevel - iKilledPlayerLevel )   // 죽인놈 냅?더 높다.
    {
        return EXP_PANELTY_RATIO_KILLED_LOW_LEVEL;
    }
    else if ( PANELTY_LEVEL_GAP  <= iKilledPlayerLevel - iKillPlayerLevel )
    {
        return EXP_PANELTY_RATIO_KILLED_HIGH_LEVEL;
    }
    return EXP_PANELTY_RATIO_KILLED_SAME_LEVEL;
}

void FileOutLevelUpStageCalc()
{
    return;
    FILE *fp = fopen("ExpTable.txt","wb");
    GCEXPTYPE biCount;
    
    for(int i = 0; i < g_kGlobalValue.GetMaxLevel(); i++)
    {
        if( i == 0 )
        {
            biCount = ( mapExpTable[i] - 100 ) / mapExpTable[i];        //1이 되기위해서 먹어야 하는 승수
        }
        else
        {
            biCount = ( mapExpTable[i] - mapExpTable[i-1] ) / mapExpTable[i];
        }

        fprintf( fp, "%d\t\t%d\r\n", i, biCount );
    }

    fclose(fp);

}

bool IsExistPairEventItem( int iItemID1_, int iItemID2_ )
{
    bool bBoy = false;
    bool bGirl = false;

    for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive != true )
            continue;

        SCharInfo STemp = g_MyD3D->MyPlayer[i]->GetCurrentChar();

        if( g_pItemMgr->CheckEquipItemForItemID( &STemp, iItemID1_) )
        {
            bGirl = true;
        }
        else if( g_pItemMgr->CheckEquipItemForItemID( &STemp, iItemID2_) )
        {
            bBoy = true;
        }
    }
    if( bBoy == true && bGirl == true )
    {
        return true;
    }            
    else 
    {
        return false;
    }
}


float gcround( float x )
{
    return (x > 0.0f) ? floorf(x+0.5f) : ceilf(x-0.5f);
}

float gcround( float x, int iPow10 )
{
    x *= pow( 10.0f, iPow10 - 1 );
    x = (x > 0.0f) ? floorf(x+0.5f) : ceilf(x-0.5f);
    x *= pow( 10.0f, -(iPow10 - 1 ) );
    return x;
}

GCEXPTYPE GetExpFromLevel( int iLevel )
{
    if( iLevel < 0 ) iLevel = 0;
    else if( iLevel > g_kGlobalValue.GetMaxLevel() ) iLevel = g_kGlobalValue.GetMaxLevel();

    return s_biExpGetTable[iLevel];
}

int GetGPFromLevel( int iLevel )
{
    if( iLevel < 0 ) iLevel = 0;
    else if( iLevel > g_kGlobalValue.GetMaxLevel() ) iLevel = g_kGlobalValue.GetMaxLevel();

    return s_iGPGetTable[iLevel];
}
float GetCurrentExpPercent( GCEXPTYPE biExp_ )
{
	int iLevel = Exp_2_Level( biExp_ );

	if( iLevel == g_kGlobalValue.GetMaxLevel() )
		return 0.0f;

	GCEXPTYPE biTargetExp = Level_2_Exp( iLevel );
	GCEXPTYPE biPrevExp = 0;

	if(iLevel != 0)	
		biPrevExp = Level_2_Exp( iLevel-1 );

	if( biPrevExp > biExp_ || biPrevExp > biTargetExp )
		return 0;

	return (float)(biExp_ - biPrevExp ) * 100.0f / (float)( biTargetExp - biPrevExp );
}

bool SetExpTable( const std::map<DWORD, GCEXPTYPE>& _mapExpTable )
{
    if ( (int)_mapExpTable.size() < g_kGlobalValue.GetMaxLevel() )  
    {
        ASSERT( "mapExpTable Size error");
		// 경험치 테이블이 이상하다구 쓰지 않으면 더이상해진다.
		// 에러 로그를 남겨야 겠음
		// return false;
    }

    mapExpTable.clear(); 
    
    mapExpTable = _mapExpTable;

    //  최고 레벨 경험치 셋팅한다. ( 서버에서 안준다 ㅜ.ㅜ )
    mapExpTable[g_kGlobalValue.GetMaxLevel()] = 0xFFFFFFFF;

    return true;
}
