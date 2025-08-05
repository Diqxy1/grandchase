#include "user.h" // winsock 땜시롱..
#include ".\resultmanager.h"
#include <dbg/dbg.hpp>
#include "GCEnum.h"
#include "Lua/KLuaManager.h"
#include <boost/tuple/tuple.hpp>
#include "NetError.h"
#include "GSSimLayer.h"
#include "Channel.h"
#include <KncUtil.h>
#include "GameDrop.h"
#include "Room.h"
#include <boost/bind.hpp>
#include "LogManager.h"
#include "RecomHelper.h"
#include "Rainbow.h"
#include "CoupleSystem.h"
#include "IPAdvantage.h"
#include "ItemContainer.h"
#include "GCItemAttribute.h" // Attribute
#include "ItemManager.h"
#include "HeroDungeonManager.h"
#include "ChannelTypeEvent.h"
#include "Worker.h"
#include "GuildManager.h"
#include "DotaManager.h"
#include "ChannelManager.h"
#include "GWCEvent.h"
#include "Escort.h"
#include <boost/bind/bind.hpp>
#include "PreHackingCheckManager.h"
#include "Log4.h"
#include "PVPMatchManager.h"

ImplementSingleton( KResultManager );

#define REG_HANDLER(x,y)            m_mapFunction.insert( std::make_pair(x,y) )
#define GUILD_BATTLE_ADV            1.7f // no use

namespace{

    class KRankComp
    {
    public:
        bool operator() ( const KGameResultIn& a, const KGameResultIn& b )
        {
            if( a.m_iNumKills > b.m_iNumKills ) return true;

            if( a.m_iNumKills  == b.m_iNumKills && a.m_iNumDies < b.m_iNumDies)
                return true;

            return false;
        }
    };
}
// ===== 각종 테이블 =====
//경험치 레벨 테이블
/*
const __int64  KResultManager::ms_biExpTable[] =
{
//  1           2           3           4           5           6           7           8           9           10
    2020,       2200,       2500,       2920,       3460,       4060,       4720,       5440,       6220,       7500,       // 10
    9400,       11700,      14940,      19280,      25580,      34160,      45600,      59320,      75520,      96760,      // 20
    122360,     152180,     184940,     220640,     259280,     306800,     391600,     527200,     827200,     1208200,    // 30
    1744200,    2550600,    3536200,    4848200,    7110200,    10054200,   13740200,   18228200,   23578200,   29850200,   // 40
    37338200,   46194200,   56514200,   68394200,   81930200,   97218200,   114354200,  133434200,  154554200,  177810200,  // 50
    203298200,  231342200,  262062200,  295578200,  332010200,  371478200,  414582200,  461482200,  512338200,  567310200,  // 60
    628674200,  695154200,  764634200,  837114200,  912594200,  991074200,  1073154200, 1158834200, 1248114200, 1340994200, // 70
    1437474200, 1538154200, 1643034200, 1752114200, 1865394200,
};
// Grade Table
const DWORD KResultManager::ms_dwGradeTable[] =
{
    //    동        은        금
         4060,     11700,    45600,     // 떠돌이
       152180,    391600,  2550600,     // 용병
     10054200,  29850200, 68394200,     // 왕립 
    114354200, 177810200,               // 신
};


// Gp 보상 테이블
const int KResultManager::ms_iGPGetTable[] =  // 각 레벨에서 얻을 수 있는 GP 테이블
{   // 획득GP   // Level
    // 0      1      2      3      4      5      6      7      8      9
      5,    8,   10,   13,   16,   20,   22,   24,   26,   29,   // 0
     34,   40,   44,   48,   53,   60,   68,   72,   78,   85,   // 10
     93,   98,  104,  109,  114,  119,  125,  135,  148,  173,   // 20
    200,  202,  204,  206,  208,  210,  212,  214,  216,  218,   // 30
    220,  222,  224,  226,  228,  230,  232,  234,  236,  238,   // 40
    240,  242,  244,  246,  248,  250,  252,  254,  256,  258,   // 50
    260,  262,  264,  266,  268,  270,  272,  274,  276,  278,   // 60
    280,  282,  284,  286,  288,  290,
};

// Exp 보상 테이블
const int KResultManager::ms_iExpGetTable[] = // 각 레벨에서 얻을 수 있는 경험치 테이블.
{   // 획득Exp  // Level
    // 0        1           2           3           4           5           6           7           8           9
    20,         60,         60,         60,         60,         60,         60,         60,         60,         80,      // 0
    100,        100,        120,        140,        180,        220,        260,        280,        300,        360,     // 10
    400,        420,        420,        420,        420,        480,        800,        1200,       2500,       3000,    // 20
    4000,       5600,       6400,       8000,       13000,      16000,      19000,      22000,      25000,      28000,   // 30
    32000,      36000,      40000,      44000,      48000,      52000,      56000,      60000,      64000,      68000,   // 40
    72000,      76000,      80000,      84000,      88000,      92000,      96000,      100000,     104000,     108000,  // 50
    116000,     120000,     120000,     120000,     120000,     120000,     120000,     120000,     120000,     120000,  // 60
    120000,     120000,     120000,     120000,     120000,     120000,
};
*/
/*
// Spx 보상 테이블
const int KResultManager::ms_iSpxGetTable[] = // 각 레벨에서 얻을 수 있는 경험치 테이블.
{   // 획득Exp  // Level
    // 0        1           2           3           4           5           6           7           8           9
    20,         60,         60,         60,         60,         60,         60,         60,         60,         80,      // 0
    100,        100,        120,        140,        180,        220,        260,        280,        300,        360,     // 10
    400,        420,        420,        420,        420,        480,        800,        1200,       2500,       3000,    // 20
    4000,       5600,       6400,       8000,       13000,      16000,      19000,      22000,      25000,      28000,   // 30
    32000,      36000,      40000,      44000,      48000,      52000,      56000,      60000,      64000,      68000,   // 40
    72000,      76000,      80000,      84000,      88000,      92000,      96000,      100000,     104000,     108000,  // 50
    116000,     120000,     120000,     120000,     120000,     120000,     120000,     120000,     120000,     120000,  // 60
    120000,     120000,     120000,     120000,     120000,     120000,
};
*/
/*
// 미국 엘리시스 스킬트리 추가로 인해서 SPX TABLE 조정( 미국 한정 )
// Spx 보상 테이블
const int KResultManager::ms_iSpxGetTable[] = // 각 레벨에서 얻을 수 있는 경험치 테이블.
{   // 획득Exp  // Level
    // 0        1           2           3           4           5           6           7           8           9
    3,          10,         10,         10,         10,         10,         10,         10,         10,         13,     // 0
    17,         17,         20,         23,         30,         37,         43,         47,         50,         60,     // 10
    67,         70,         70,         70,         70,         80,         133,        200,        417,        500,    // 20
    667,        933,        1067,       1333,       2167,       2667,       3167,       3667,       4167,       4667,   // 30
    5333,       6000,       6667,       7333,       8000,       8667,       9333,       10000,      10667,      11333,  // 40
    12000,      12667,      13333,      14000,      14667,      15333,      16000,      16667,      17333,      18000,  // 50
    19333,      20000,      20000,      20000,      20000,      20000,      20000,      20000,      20000,      20000,  // 60
    20000,      20000,      20000,      20000,      20000,      20000,
};
*/

// 대만 홍콩에서 사용되는 캐릭터 그레이드.
//const DWORD KResultManager::ms_dwCharacterGrade[] =
//{
//    /* 마크 없음 */ 0,      /* 떠돌이 동 */ 1,        /* 떠돌이 은 */ 31,        /* 떠돌이 금 */ 131,
//    /*  용병  동 */ 331,    /*  용병  은 */ 531,      /*  용병  금 */ 831,       /*  왕립  동 */ 1331,
//    /*  왕립  은 */ 2031,   /*  왕립  금 */ 2831,     /*   신   동 */ 3831,      /*   신   은 */ 5831,
//    /*   신   금 */ 8831,
//};
//const int KResultManager::ms_iMiniGameGPTable[] =
//{
//   //           1         2         3         4          5          6          7         8          9 
//        0,      3,        5,        7,        9,        11,        13,        15,        17,        19,
//};
// =======================

std::wostream& operator<<( std::wostream& stm_, const std::pair<const DWORD,sMonsterCrusaderInfo>& data_ )
{
    return stm_ << L"{" << data_.second.m_nMonsterID << L"," << data_.second.m_fExpRatio << L"," << data_.second.m_fGpRatio << L"}";
}

NiImplementRootRTTI( KResultManager );
ImplOstmOperatorW2A( KResultManager );
ImplToStringW( KResultManager )
{
    KLocker lock( m_csDefaultReward );
    KLocker lock1( m_csQuestExpGp );
    KLocker lock2( m_csBoostInfo );
    KLocker lock3( m_csMonsterHunt );
    KLocker lock4( m_csMonterInfo );
    KLocker lock5( m_csReconcileInfo );
    KLocker lock6( m_csRankPoint );
    KLocker lock7( m_csClearBonus );
    KLocker lock8( m_csExpSpxInfo );
    KLocker lock9( m_csModifiedRatio );
    KLocker lock10( m_csControlFeedback );

    START_TOSTRINGW
        << TOSTRINGW( m_mapDefaultReward.size() )
        << TOSTRINGW( m_vecRankPoint.size() )
        << TOSTRINGW( m_fPerfectBonus )
        << TOSTRINGW( m_setPerfectEventGameMode.size() )
        << TOSTRINGW( m_vecPartyBonus.size() )
        << TOSTRINGW( m_vecLvReconcile.size() )
        << TOSTRINGW( m_mapMonterInfo.size() )
        << TOSTRINGW( m_mapMonsterHunt.size() )
        << TOSTRINGW( m_mapBoostInfo.size() )
        << TOSTRINGW( m_mapQuestExpGp.size() )
        << TOSTRINGW( m_fClearExp )
        << TOSTRINGW( m_fClearGp )
        << TOSTRINGW( m_fClearSpx )
        << TOSTRINGW( m_mapExpTable.size() )
        << TOSTRINGW( m_mapGradeTable.size() )
        << TOSTRINGW( m_mapGPGetTable.size() )
        << TOSTRINGW( m_mapExpGetTable.size() )
        << TOSTRINGW( m_mapSpxGetTable.size() )
        << TOSTRINGW( m_mapCharacterGrade.size() )
        << TOSTRINGW( m_mapMiniGameGPTable.size() )
        << TOSTRINGW( m_mapMonModifiedRatio.size() )
        << TOSTRINGW( m_dwMaxLevel )
        << TOSTRINGW( m_mapControlFeedbackExpBonusRatio.size() );

    return stm_;
}

KResultManager::KResultManager(void)
:m_bClippingExpRewardLimit(false)
,m_biExpRewardLimit(0)
,m_dwMaxLevel(0)
,m_fClearExp(0.f)
,m_fClearGp(0.f)
,m_fClearSpx(0.f)
,m_fDefaultReward(0.f)
,m_fPerfectBonus(0.f)
,m_nModifiedMinLevel(0)
,m_nModifiedMaxLevel(0)
,m_nMaxRank(7)
{
    RegistFunctions();
}

KResultManager::~KResultManager(void)
{
}

void KResultManager::RegistFunctions()
{
    REG_HANDLER( GC_GM_TEAM_MODE, &KResultManager::CalcMatchExpGp );
    REG_HANDLER( GC_GM_SURVIVAL_MODE, &KResultManager::CalcMatchExpGp );
    REG_HANDLER( GC_GM_INDIGO_TEAM, &KResultManager::CalcEmptyExpGp );
    REG_HANDLER( GC_GM_TAG_TEAM, &KResultManager::CalcTagMatchExpGp );
    REG_HANDLER( GC_GM_TAG_SURVIVAL, &KResultManager::CalcTagMatchExpGp );
    REG_HANDLER( GC_GM_DEATH_TEAM, &KResultManager::CalcMatchExpGp );
    REG_HANDLER( GC_GM_DEATH_SURVIVAL, &KResultManager::CalcMatchExpGp );
    REG_HANDLER( GC_GM_ANGELS_EGG, &KResultManager::CalcAngelEggExpGp );
    REG_HANDLER( GC_GM_CAPTAIN, &KResultManager::CalcMatchExpGp );
    REG_HANDLER( GC_GM_GUILD_BATTLE, &KResultManager::CalcGuildBattleExpGp );
    REG_HANDLER( GC_GM_DOTA, &KResultManager::CalcDotaExpGp );
    REG_HANDLER( GC_GM_AUTOMATCH_TEAM, &KResultManager::CalcMatchExpGp );
    REG_HANDLER( GC_GM_FATAL_DEATH_TEAM, &KResultManager::CalcMatchExpGp );
    REG_HANDLER( GC_GM_FATAL_DEATH_SURVIVAL, &KResultManager::CalcMatchExpGp );
	REG_HANDLER( GC_GM_MONSTER_TEAM, &KResultManager::CalcMatchExpGp );
	REG_HANDLER( GC_GM_MONSTER_SURVIVAL, &KResultManager::CalcMatchExpGp );
    
    REG_HANDLER( GC_GM_QUEST0, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST1, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST2, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST3, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST4, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST5, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST6, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST7, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST8, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST9, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST10, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST11, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST12, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST13, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST14, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST15, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST16, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST17, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST18, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST19, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST20, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST21, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST22, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST23, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST24, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST25, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST26, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST27, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST28, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST29, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST30, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST31, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST32, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST33, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST34, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST35, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST36, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST37, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST38, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST39, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST40, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST41, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST42, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST43, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST44, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST45, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST48, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST49, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST50, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST53, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST54, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST55, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST56, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST57, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST58, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST59, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST60, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST61, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST62, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST63, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST64, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST65, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST66, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST67, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST68, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST69, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST70, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST71, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST72, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST73, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST74, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST75, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST76, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST77, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST78, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST79, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST80, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST83, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST84, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST85, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST86, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST87, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST88, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST89, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST90, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST91, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST92, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST93, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST94, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST95, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST96, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST97, &KResultManager::CalcQuestExpGp );
	REG_HANDLER( GC_GM_QUEST98, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST46, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST47, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST51, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST52, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST81, &KResultManager::CalcQuestHeroExpGp );
	REG_HANDLER( GC_GM_QUEST82, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST99, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST100, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST101, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST102, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_QUEST103, &KResultManager::CalcQuestExpGp );
    REG_HANDLER( GC_GM_QUEST104, &KResultManager::CalcQuestHeroExpGp );
    REG_HANDLER( GC_GM_MONSTER_CRUSADER, &KResultManager::CalcMonsterCrusaderExpGp );
    REG_HANDLER( GC_GM_MONSTER_HUNT, &KResultManager::CalcMonsterHuntExpGp );
}

bool KResultManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KResultManager::_LoadScript( OUT std::string& strScript_ )
{
    KLuaManager kLuaMngBoost;
    const std::string strExpGpBoostInfoScriptName = "ExpGpBoostInfo.lua";
    _JIF( kLuaMngBoost.DoFile( strExpGpBoostInfoScriptName.c_str() ) == S_OK, return false );

    KLuaManager kLuaMngDefault;
    const std::string strDefaultRewardScriptName = "DefaultReward.lua";
    _JIF( kLuaMngDefault.DoFile( strDefaultRewardScriptName.c_str() ) == S_OK, return false );

    //ExpGpBoostInfo.lua
    strScript_ = strExpGpBoostInfoScriptName;
    _JIF( LoadBoostItemInfo( kLuaMngBoost ), return false );
    _JIF( LoadMonsterHunt( kLuaMngBoost ), return false );
    _JIF( LoadRankBonus( kLuaMngBoost ), return false );
    _JIF( LoadPVPBonus( kLuaMngBoost ), return false );
    _JIF( LoadHellRankBonus( kLuaMngBoost ), return false );
    _JIF( LoadClearBonus( kLuaMngBoost ), return false );
    _JIF( LoadControlFeedbackBonus( kLuaMngBoost ), return false );
    _JIF( kLuaMngBoost.GetValue( "MaxRank", m_nMaxRank ) == S_OK, return false );

    //DefaultReward.lua
    strScript_ = strDefaultRewardScriptName;
    _JIF( LoadMonsterCrusader( kLuaMngDefault ), return false );
    _JIF( LoadDefaultReward( kLuaMngDefault ), return false );
    _JIF( LoadConfigData( kLuaMngDefault ), return false );
    _JIF( LoadCharLimit( kLuaMngDefault ), return false );
    _JIF( LoadMonModifiedRatio( kLuaMngDefault ), return false );


    _JIF( LoadQuestInfo(strScript_), return false );
    _JIF( LoadDungeonInfo(strScript_), return false );

    // 이걸 한꺼번에 읽어들이는 게 좋을듯 하다. 보상 Table을 
    // 분리해서 관리하는것 보단 이게 좋을듯..,
    // Exp/Lv Table, Grade Table, Gp 보상 Table, Exp 보상 Table, 
    // Spx 보상 Table, 대만 홍콩에서 사용되는 캐릭터 그레이드,
    // 미니게임에서 얻는 GP 보상 Table
    _JIF( LoadExpSpxInfo(strScript_), return false );
    return true;
}

char KResultManager::Exp_2_Level( const __int64 biExp_ )
{
    // 경험치-레벨 테이블을 참조하여 현재 경험치에 대한 레벨을 얻어낸다.
    std::map< DWORD, __int64 >::const_iterator mit;
    for( mit = m_mapExpTable.begin(); mit != m_mapExpTable.end(); ++mit )
    {
        if( biExp_ < mit->second ) return (char)mit->first;
    }

    // 그럴일은 없겠지만
    // 위의 반복자에서 해당 정보를 return 받지 못했을 경우..,
    // 가장 큰 exp Table의 정보를 리턴하겠음..,
    return (char)m_dwMaxLevel;
}

void KResultManager::GetLevelExp( IN const DWORD dwBeforeLevel_, IN const __int64 biBeforeExp_, OUT DWORD& dwAfterLevel_, OUT __int64& biAfterExp_ )
{
    dwAfterLevel_ = 0;
    biAfterExp_ = 0;

    if( m_dwMaxLevel > dwBeforeLevel_ ) {
        // 경험치-레벨-테이블을 참조하여 계산하자.
        std::map< DWORD, __int64 >::const_iterator mit;
        for( mit = m_mapExpTable.begin(); mit != m_mapExpTable.end(); ++mit ) {
            // 현재 레벨 경험치 부분에 대한 체크.
            if( mit->first == dwBeforeLevel_ ) {
                if( mit->second <= biBeforeExp_ ) {
                    dwAfterLevel_ = dwBeforeLevel_ + 1;
                    biAfterExp_ = biBeforeExp_ - mit->second;
                } //if
            } // if

            // 남은 경험치에 대해서 추가 레벨업이 있는지에 대한 체크.
            if( mit->first <= dwAfterLevel_ && mit->second <= biAfterExp_ ) {
                dwAfterLevel_ += dwAfterLevel_;
                biAfterExp_ = biAfterExp_ - mit->second;
            } // if
        } // for
    } else {
        dwAfterLevel_ = dwBeforeLevel_;
        biAfterExp_ = biBeforeExp_;
    }

    if( dwAfterLevel_ == m_dwMaxLevel ) {
        biAfterExp_ = 0;
    }
}

__int64 KResultManager::GetMaxExp()
{
//    return KResultManager::ms_biExpTable[ NUM_ELEMENTS( KResultManager::ms_biExpTable ) -1 ];
    // 최고 레벨의 경험치를 가져올 때..,
    std::map< DWORD, __int64>::const_iterator mit;
    mit = --m_mapExpTable.end(); // 79Level Index의 값.
    return mit->second;
}

__int64 KResultManager::GetExpByLv( DWORD dwLevel_ )
{
    // DWORD 라서 음수는 없다..
    if ( m_mapExpTable.empty() ) return 0;

    // MaxLevel 보다 크면 무한대 경험치를 반환한다.
    std::map< DWORD, __int64 >::const_iterator mit;
    mit = --m_mapExpTable.end();
    if( dwLevel_ > mit->first ) return mit->second;

    // 0 인덱스는 1레벨의 시작 경험치 이므로
    // 74인덱스는 75레벨의 시작 경험치.
    // 파라미터로 들어 온게 0이면 0레벨이 가질수 있는 최대 경험치 (1레벨 경험치) - 1 경험치.
    // 이말은 0인덱스의 값 -1 이라는 의미..

    // 시즌4
    // 누적 경험치 시스템은 동일하나 레벨뒤에 경험치가 해당 레벨의 목표경험치이다.
    // { 1, 240 }은 1Lv에서 240Exp이상이 되면 2Lv로 레벨업을 한다.
    mit = m_mapExpTable.find( dwLevel_ );

    return mit->second;
}

__int64 KResultManager::GetGradePoint( const std::vector<__int64>& Exp )
{
    __int64 biAccumExp    = 0;
    __int64 biBestExp     = 0;

    // 모든 캐릭터의 경험치를 누적시키고 가장 높은 EXP를 찾는다.
    std::vector<__int64>::const_iterator  vit;
    for(vit = Exp.begin(); vit != Exp.end(); ++vit)
    {
        biAccumExp += *vit;
        if ( biBestExp < *vit ) biBestExp = *vit;
    }

    // ( 모든 캐릭터 EXP 합 + 캐릭터 중 가장 높은 EXP * 2 ) / ( 주 캐릭터의 계산 횟수(3=1+2) )
    return ( biAccumExp + biBestExp * 2 ) / 3;
}

char KResultManager::GetGrade( const std::vector<__int64>& vecExp_ )
{
    __int64 biAccumExp    = 0;
    __int64 biBestExp     = 0;
    __int64 biTotal       = 0;

    // 모든 캐릭터의 경험치를 누적시키고 가장 높은 EXP를 찾는다.
    std::vector<__int64>::const_iterator  vit;
    for(vit = vecExp_.begin(); vit != vecExp_.end(); ++vit)
    {
        biAccumExp += *vit;
        if ( biBestExp < *vit ) biBestExp = *vit;
    }
    
    // ( 모든 캐릭터 EXP 합 + 캐릭터 중 가장 높은 EXP * 2 ) / ( 주 캐릭터의 계산 횟수(3=1+2) )
    // 기본 캐릭터로 나누지 않으면 캐릭터를 얻는 순간 등급이 떨어진다.
    biTotal = GetGradePoint( vecExp_ );

    std::map< DWORD, DWORD >::iterator mit;
    for( mit = m_mapGradeTable.begin(); mit != m_mapGradeTable.end(); ++mit )
    {
        if( biTotal < mit->second ) return (char)(mit->first+1);// 11 (신 금)
    }

    mit = --m_mapGradeTable.end();
    return (char)(mit->first+1);
}

int KResultManager::GetCrusaderMonCount( IN const std::map<std::pair<int,int>,int>& mapKillMon_ )
{
    int nCount = 0;
    std::map<std::pair<int,int>,int>::const_iterator mit;
    for( mit = mapKillMon_.begin() ; mit != mapKillMon_.end() ; ++mit )
    {
        nCount += mit->second;
    }
    return nCount;
}

bool KResultManager::GetQuestStageInfo( IN KLuaManager& kLuaMng_, OUT sStageInfo& stageInfo_ )
{
    _JIF( 0 == kLuaMng_.GetValue( "Difficult", stageInfo_.m_nDifficult ) , return false );
    _JIF( 0 == kLuaMng_.GetValue( "Level", stageInfo_.m_nLv) , return false );
    _JIF( 0 == kLuaMng_.GetValue( "EXP", stageInfo_.m_nExp ) , return false );
    _JIF( 0 == kLuaMng_.GetValue( "SPX", stageInfo_.m_nSpx ) , return false );
    _JIF( 0 == kLuaMng_.GetValue( "GP", stageInfo_.m_nGp ) , return false );
    _JIF( 0 == kLuaMng_.GetValue( "MinStage", stageInfo_.m_nMinStage ), return false ); // 최단 스테이지
    _JIF( 0 == kLuaMng_.GetValue( "MaxStage", stageInfo_.m_nMaxStage ), return false ); // 최다 스테이지

    stageInfo_.m_nBonusExp = 0; // 스테이지 추가 보상
    stageInfo_.m_nBonusGP = 0;
    return true;
}

bool KResultManager::LoadQuestInfo( OUT std::string& strScript_ )
{
    std::map< int, sQuestInfo > mapQuestExpGp;
    KLuaManager kLuaMng;

    strScript_ = "Dungeon_ExpGp.lua";
    _JIF( kLuaMng.DoFile( "Dungeon_ExpGp.lua" ) == S_OK, return false );
    _JIF( kLuaMng.BeginTable( "Dungeon_Exp_Gp" ) == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        sQuestInfo questInfo;
        _JIF( 0 == kLuaMng.GetValue( "DungeonID", questInfo.m_nQuestID ) , return false );

        _JIF( kLuaMng.BeginTable( "Difficult" ) == S_OK, return false );
        for( int j = 1 ; ; ++j )
        {
            sStageInfo  stageInfo;
            if( kLuaMng.BeginTable( j ) != S_OK ) break;
            _JIF( GetQuestStageInfo( kLuaMng, stageInfo ), return false );
            _JIF( kLuaMng.EndTable() == S_OK, return false );

            std::pair<std::map< int, sStageInfo >::iterator, bool> kPair;
            kPair = questInfo.m_mapStageInfo.insert( std::make_pair(stageInfo.m_nDifficult, stageInfo) );
            if( !kPair.second )
            {
                START_LOG( cerr, L" 중복된 난이도가 있음... Dungeon ID : " << questInfo.m_nQuestID ) << END_LOG;
                return false;
            }
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        std::pair<std::map< int, sQuestInfo >::iterator, bool> kPair;
        kPair = mapQuestExpGp.insert( std::make_pair( questInfo.m_nQuestID, questInfo ) );
        if( !kPair.second )
        {
            START_LOG( cerr, L" 중복된 던전이 있음... Dungeon ID : " << questInfo.m_nQuestID ) << END_LOG;
            return false;
        }

        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Quest Script Load Complete Size : " << mapQuestExpGp.size() ) << END_LOG;
    KLocker lock( m_csQuestExpGp );
    m_mapQuestExpGp.swap( mapQuestExpGp );
    return true;
}


void KResultManager::DumpQuestInfo( std::wostream& stm ) const
{
    KLocker lock( m_csQuestExpGp );
    std::map< int, sQuestInfo >::const_iterator mit;
    for( mit = m_mapQuestExpGp.begin() ; mit != m_mapQuestExpGp.end() ; ++mit )
    {
        stm << L"ID : " << mit->second.m_nQuestID << std::endl;

        const std::map< int, sStageInfo >& mapExpInfo = mit->second.m_mapStageInfo;
        std::map< int, sStageInfo >::const_iterator mit1;
        for( mit1 = mapExpInfo.begin() ; mit1 != mapExpInfo.end() ; ++mit1 )
        {
            stm << L"  Difficult : " << mit1->second.m_nDifficult
                << L", Level : " << mit1->second.m_nLv
                << L", Exp : " << mit1->second.m_nExp
                << L", GP : " << mit1->second.m_nGp
                << L", BonusEXP : " << mit1->second.m_nBonusExp
                << L", BonusGP : " << mit1->second.m_nBonusGP
                << L", MinStage : " << mit1->second.m_nMinStage
                << L", MaxStage : " << mit1->second.m_nMaxStage
                << std::endl;
        }
    }
}

bool KResultManager::GetQuestReward( IN const int& nQuestID_, IN const int& nStageCount_, IN const int& nDifficult_, OUT sStageInfo& stageInfo_ )
{
    stageInfo_.clear();

    stageInfo_.m_nLv = 1;
    stageInfo_.m_nExp = 0;
    stageInfo_.m_nGp = 0;

    KLocker lock( m_csQuestExpGp );

    std::map< int, sQuestInfo >::const_iterator cmitQuest = m_mapQuestExpGp.find( nQuestID_ );
    _JIF( cmitQuest != m_mapQuestExpGp.end(), return false );

    std::map< int, sStageInfo >::const_iterator cmitStage = cmitQuest->second.m_mapStageInfo.find( nDifficult_ );
    _JIF( cmitStage != cmitQuest->second.m_mapStageInfo.end(), return false );

    // 최소 스테이지 수보다 적어.
    if ( nStageCount_ < cmitStage->second.m_nMinStage || nStageCount_ > cmitStage->second.m_nMaxStage ) {
        START_LOG( cwarn, L"최소 스테이지 카운트보다 적음 StageCount : " << nStageCount_ << " MinStageCount : " << cmitStage->second.m_nMinStage ) << END_LOG;
    }

    stageInfo_ = cmitStage->second;
    return true;
}

bool KResultManager::LoadBoostItemInfo( KLuaManager& kLuaMng_ )
{
    std::map< GCITEMID, sBoostInfo > mapBoostInfo;
    _JIF( kLuaMng_.BeginTable( "ExpGPBoostInfo") == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) == S_OK )
        {
            sBoostInfo boostInfo;
            boostInfo.m_dwItemID        = 0;
            boostInfo.m_bAll            = false;

            _JIF( 0 == kLuaMng_.GetValue( "dwItemID", boostInfo.m_dwItemID ), return false );
            _JIF( boostInfo.m_dwItemID != 0, return false );
            _JIF( 0 == kLuaMng_.GetValue( "bEquip", boostInfo.m_bEquip ), return false );
            _JIF( 0 == kLuaMng_.GetValue( "bAll", boostInfo.m_bAll ), return false );

            {
                if( kLuaMng_.BeginTable( "BoostDetailInfo" ) != S_OK ) break;
                for( int j = 1 ; ; ++j )
                {
                    if( kLuaMng_.BeginTable( j ) != S_OK ) break;
                    
                    _JIF( LoadBoostDetailInfo( kLuaMng_, boostInfo.m_mapBoostDetailInfo ), return false );
                    _JIF( kLuaMng_.EndTable() == S_OK, return false );
                }
                _JIF( kLuaMng_.EndTable() == S_OK, return false );

            }

            _JIF( kLuaMng_.EndTable() == S_OK, return false );

            if( mapBoostInfo.find( boostInfo.m_dwItemID ) != mapBoostInfo.end() )
            {
                START_LOG( cerr, L"중복된 아이템 아이디가 존재 합니다. Item ID : " << boostInfo.m_dwItemID ) << END_LOG;
                return false;
            }
            mapBoostInfo[ boostInfo.m_dwItemID ] = boostInfo;
        }
        else
            break;
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );


    START_LOG( cerr, L" Exp/Gp Boost 정보 읽기 완료, Count : " << mapBoostInfo.size() ) << END_LOG;
    KLocker lock( m_csBoostInfo );
    m_mapBoostInfo.swap( mapBoostInfo );

    return true;
}

bool KResultManager::LoadMonsterHunt( KLuaManager& kLuaMng_ )
{
    std::map< int, sMonsterHuntInfo > mapMonsterHunt;
    
    for( int i = 0 ; ; ++i )
    {
        std::stringstream stm;
        stm << "monster_hunt_" << i;
        if( FAILED( kLuaMng_.BeginTable( stm.str().c_str() ) ) )
            break;

        sMonsterHuntInfo monsterHuntInfo;
        monsterHuntInfo.m_nDifficult = i;

        _JIF( 0 == kLuaMng_.GetValue( "nClearPoint", monsterHuntInfo.m_nClearPoint ), return false );
        _JIF( 0 == kLuaMng_.GetValue( "nExp", monsterHuntInfo.m_nExp ), return false );
        _JIF( 0 == kLuaMng_.GetValue( "nSpx", monsterHuntInfo.m_nSpx ), return false );
        _JIF( 0 == kLuaMng_.GetValue( "nGp", monsterHuntInfo.m_nGp ), return false );

        if( mapMonsterHunt.find( monsterHuntInfo.m_nDifficult ) != mapMonsterHunt.end() )
        {
            START_LOG( cerr, L"중복된 난이도 레벨이 존재 합니다. TableName : " << KncUtil::toWideString( stm.str() ) ) << END_LOG;
            return false;
        }

        mapMonsterHunt[monsterHuntInfo.m_nDifficult] = monsterHuntInfo;

        if( FAILED( kLuaMng_.EndTable() ) )
        {
            START_LOG( cerr, L"몬스터 헌트 정보 테이블을 닫지 못함. Table Name : "
                << KncUtil::toWideString( stm.str() ) ) << END_LOG;
            return false;
        }
    }

    START_LOG( clog, L" monster_hunt Exp/Gp 정보 읽기 완료, Count : " << mapMonsterHunt.size() ) << END_LOG;

    KLocker lock( m_csMonsterHunt );
    m_mapMonsterHunt.swap( mapMonsterHunt );
    return true;
}

bool KResultManager::LoadMonsterCrusader( KLuaManager& kLuaMng_ )
{
    std::map< int, sMonsterCrusaderInfo >   mapMonterInfo;
    mapMonterInfo.clear();
    _JIF( kLuaMng_.BeginTable( "MonCrusaderInfo" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        sMonsterCrusaderInfo sInfo;
        _JIF( 0 == kLuaMng_.GetValue( 1, sInfo.m_nMonsterID ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, sInfo.m_fExpRatio ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 3, sInfo.m_fGpRatio ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 4, sInfo.m_fSpxRatio ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
        std::pair<std::map<int,sMonsterCrusaderInfo>::iterator,bool> prRet;
        prRet = mapMonterInfo.insert( std::make_pair( sInfo.m_nMonsterID, sInfo ) );
        if( !prRet.second )
        {
            START_LOG( cerr, L"중복된 몬스터 원정대 정보가 있습니다... ID : " << sInfo.m_nMonsterID ) << END_LOG;

            return false;
        }
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L" 몬스터 원전대 로드 완료.. Size : " << mapMonterInfo.size() ) << END_LOG;
    KLocker lock( m_csMonterInfo );
    m_mapMonterInfo.swap( mapMonterInfo );
    return true;
}

bool KResultManager::LoadDungeonInfo( OUT std::string& strScript_ )
{
    // 루아 파일 읽기
    // 매우 반복적이다.. 어떻게 않되나?
    std::vector<sReconcile> vecPartyBonus;
    std::vector<sReconcile> vecLvReconcile;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitReconcileInfo.lua";
    strScript_ = strScriptName;
    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    // 파티 보너스읽기
    if( kLuaMng.BeginTable( "PartyBonus" ) == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng.BeginTable( i ) == S_OK )
            {
                sReconcile sInfo;
                _JIF( 0 == kLuaMng.GetValue( 1, sInfo.m_nBegin ), return false );
                _JIF( 0 == kLuaMng.GetValue( 1, sInfo.m_nEnd ), return false );
                _JIF( 0 == kLuaMng.GetValue( 2, sInfo.m_fReconcValue ), return false );
                vecPartyBonus.push_back( sInfo );
                _JIF( kLuaMng.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( clog, L"PartyBonus 테이블 읽기 종료.. Index : " << i ) << END_LOG;
                break;
            }
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        //소팅...
        std::sort(vecPartyBonus.begin(), vecPartyBonus.end(),
            boost::bind(&sReconcile::m_nBegin, _1) < boost::bind(&sReconcile::m_nBegin, _2) );
    }
    else
    {
        START_LOG( cerr, L"PartyBonus 테이블을 읽지 못함..." ) << END_LOG;
        return false;
    }

    // 레벨대의 조정값읽기
    if( kLuaMng.BeginTable( "LevelReconcileInfo" ) == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng.BeginTable( i ) == S_OK )
            {
                sReconcile sInfo;
                _JIF( 0 == kLuaMng.GetValue( 1, sInfo.m_nBegin ), return false );
                _JIF( 0 == kLuaMng.GetValue( 2, sInfo.m_nEnd ), return false );
                _JIF( 0 == kLuaMng.GetValue( 3, sInfo.m_fReconcValue ), return false );
                vecLvReconcile.push_back( sInfo );
                _JIF( kLuaMng.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( clog, L"LevelReconcileInfo 테이블 읽기 종료.. Index : " << i ) << END_LOG;
                break;
            }
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        //소팅...
        std::sort(vecLvReconcile.begin(), vecLvReconcile.end(),
            boost::bind(&sReconcile::m_nBegin, _1) < boost::bind(&sReconcile::m_nBegin, _2) );
    }
    else
    {
        START_LOG( cerr, L"LevelReconcileInfo 테이블을 읽지 못함..." ) << END_LOG;
        return false;
    }


    KLocker lock( m_csReconcileInfo );
    m_vecPartyBonus.swap( vecPartyBonus );
    m_vecLvReconcile.swap( vecLvReconcile );

    return true;
}

bool KResultManager::LoadRankBonus( KLuaManager& kLuaMng_ )
{
    float fPerfectBonus = 0.f;
    std::vector<sReconcile> vecRankPoint;
    vecRankPoint.clear();

    if( kLuaMng_.BeginTable( "RankBonus") == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng_.BeginTable( i ) == S_OK )
            {
                sReconcile sInfo;
                _JIF( 0 == kLuaMng_.GetValue( 1, sInfo.m_nBegin ), return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, sInfo.m_nEnd ), return false );
                _JIF( 0 == kLuaMng_.GetValue( 3, sInfo.m_fReconcValue ), return false );
                vecRankPoint.push_back( sInfo );

                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"RankBonus 테이블읽기 종료 Index : " << i ) << END_LOG;
                break;
            }
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        //소팅...
        std::sort(vecRankPoint.begin(), vecRankPoint.end(),
            boost::bind(&sReconcile::m_nBegin, _1) < boost::bind(&sReconcile::m_nBegin, _2) );

        // 퍼펙트이벤트 보너스
        kLuaMng_.GetValue( "PerfectEventBonus", fPerfectBonus );
    }


    KLocker lock( m_csRankPoint );
    m_vecRankPoint.swap( vecRankPoint );
    m_fPerfectBonus = fPerfectBonus;
    return true;
}

bool KResultManager::LoadDefaultReward( KLuaManager& kLuaMng_ )
{
    std::map<int, sDefaultReward> mapDefaultReward;
    _JIF( kLuaMng_.BeginTable( "DefaultReward") == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        sDefaultReward sInfo;
        _JIF( 0 == kLuaMng_.GetValue( 1, sInfo.m_nLv ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, sInfo.m_nMonExp ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 3, sInfo.m_nMonGp ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 4, sInfo.m_nDungeonExp ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( mapDefaultReward.find( sInfo.m_nLv ) != mapDefaultReward.end() )
        {
            START_LOG( cerr, L" Default Reward에 중복된 데이터가 있음." )
                << BUILD_LOG( sInfo.m_nLv )
                << BUILD_LOG( sInfo.m_nMonExp )
                << BUILD_LOG( sInfo.m_nMonGp )
                << BUILD_LOG( sInfo.m_nDungeonExp ) << END_LOG;
            return false;
        }

        mapDefaultReward.insert( std::make_pair( sInfo.m_nLv, sInfo ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"Default Reward 로드 완료. Size : " << mapDefaultReward.size() ) << END_LOG;
    KLocker lock( m_csDefaultReward );
    m_mapDefaultReward.swap( mapDefaultReward );
    return true;

}

bool KResultManager::LoadConfigData( KLuaManager& kLuaMng_ )
{
    SConfigData configData;

    _JIF( kLuaMng_.BeginTable( "RewardConfig") == S_OK, return false );
    
    _JIF( 0 == kLuaMng_.GetValue( "WinRatio", configData.m_fWinRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "LoseRatio", configData.m_fLoseRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "AdditionalGPBoost", configData.m_fAdditionalGPBoost ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "AdditionalEXPBoost", configData.m_fAdditionalEXPBoost ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "TestGPRatio", configData.m_fTestGPRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "TestEXPRatio", configData.m_fTestEXPRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "TestSPXRatio", configData.m_fTestSPXRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "BaseEXPRatio", configData.m_fBaseEXPRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "BaseGPRatio", configData.m_fBaseGPRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "BaseSPXRatio", configData.m_fBaseSPXRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "ClearBonusRatio", configData.m_fClearBonusRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "PvpGpRatio", configData.m_fPvpGpRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "AngelEggWinRatio", configData.m_fAngelEggWinRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "AngelEggLoseRatio", configData.m_fAngelEggLoseRatio ), return false );

    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"RewardConfig 로드 완료" )
        << BUILD_LOG(configData.m_fWinRatio )
        << BUILD_LOG(configData.m_fLoseRatio )
        << BUILD_LOG(configData.m_fAdditionalEXPBoost )
        << BUILD_LOG(configData.m_fAdditionalGPBoost )
        << BUILD_LOG(configData.m_fTestGPRatio )
        << BUILD_LOG(configData.m_fTestEXPRatio )
        << BUILD_LOG(configData.m_fTestSPXRatio )
        << BUILD_LOG(configData.m_fBaseEXPRatio )
        << BUILD_LOG(configData.m_fBaseGPRatio )
        << BUILD_LOG(configData.m_fBaseSPXRatio )
        << BUILD_LOG(configData.m_fClearBonusRatio )
        << BUILD_LOG(configData.m_fPvpGpRatio )
        << BUILD_LOG(configData.m_fAngelEggWinRatio )
        << BUILD_LOG(configData.m_fAngelEggLoseRatio ) << END_LOG;

    KLocker lock( m_csConfig );
    m_sConfigData = configData;
    return true;
}

bool KResultManager::LoadCharLimit( KLuaManager& kLuaMng_ )
{
    std::map<char, std::pair<float,float> > mapCharLimit;

    _JIF( kLuaMng_.BeginTable( "CharLimit") == S_OK, return false );

    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;
        
        int nCharType = -10;
        std::pair<float,float> prData;
        _JIF( 0 == kLuaMng_.GetValue( 1, nCharType ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, prData.first ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 3, prData.second ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapCharLimit.insert( std::make_pair((char)nCharType, prData ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    __int64 biExpRewardLimit = 300000000;
    _JIF( 0 == kLuaMng_.GetValue( "ExpRewardLimit", biExpRewardLimit ), return false );
    bool bClippingExpRewardLimit = false;
    _JIF( 0 == kLuaMng_.GetValue( "ClippingExpRewardLimit", bClippingExpRewardLimit ), return false );

    START_LOG( clog, L"CharLimit 로드 완료 : " << mapCharLimit.size()
        << L", ExpRewardLimit : " << biExpRewardLimit
        << L", IsClippingExpRewardLimit : " << ( bClippingExpRewardLimit ? L"True" : L"False" ) ) << END_LOG;

    KLocker lock( m_csCharLimit );
    m_mapCharLimit.swap( mapCharLimit );
    m_biExpRewardLimit = biExpRewardLimit;
    m_bClippingExpRewardLimit = bClippingExpRewardLimit;
    return true;
}

bool KResultManager::LoadControlFeedbackBonus( KLuaManager& kLuaMng_ )
{
    float fPerfectBonus = 0.f;
    std::map< int, float > mapControlFeedbackExpBonusRatio;
    mapControlFeedbackExpBonusRatio.clear();

    if( kLuaMng_.BeginTable( "ControlFeedbackBonus") == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng_.BeginTable( i ) == S_OK )
            {
                int nRank = 0;
                float fRatio = 0.0f;
                _JIF( 0 == kLuaMng_.GetValue( 1, nRank ), return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, fRatio ), return false );
                
                if ( !mapControlFeedbackExpBonusRatio.insert( std::make_pair( nRank, fRatio ) ).second ) {
                    START_LOG( cerr, L"컨트롤 피드백 경험치 보너스 중복 됨" )
                        << BUILD_LOG( nRank )
                        << BUILD_LOG( fRatio )
                        << END_LOG;
                    continue;
                }

                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"ControlFeedbackBonus 읽기 완료") << END_LOG;
                break;
            }
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    KLocker lock( m_csControlFeedback );
    m_mapControlFeedbackExpBonusRatio.swap( mapControlFeedbackExpBonusRatio );

    return true;
}

bool KResultManager::GetMonsterHuntInfo( IN int nDifficult_, OUT sMonsterHuntInfo& kInfo_ )
{
    KLocker lock( m_csMonsterHunt );
    std::map< int, sMonsterHuntInfo >::iterator mit;
    mit = m_mapMonsterHunt.find( nDifficult_ );
    if( mit == m_mapMonsterHunt.end() )
    {
        START_LOG( cerr, L"해당 난이도의 정보를 가져오지 못함. Difficult : " << nDifficult_ ) << END_LOG;
        return false;
    }

    kInfo_ = mit->second;
    return true;
}
bool KResultManager::GetMonsterCrusaderInfo( IN int nMonsterID_, OUT sMonsterCrusaderInfo& sInfo_ )
{
    sInfo_.m_fExpRatio = 0.f;
    sInfo_.m_fGpRatio = 0.f;
    sInfo_.m_nMonsterID = 0;

    KLocker lock( m_csMonterInfo );
    std::map< int, sMonsterCrusaderInfo >::iterator mit;
    mit = m_mapMonterInfo.find( nMonsterID_ );
    if( mit == m_mapMonterInfo.end() )
    {
        START_LOG( cerr, L"해당 몬스터의 정보를 가져오지 못함. nMonsterID : " << nMonsterID_ ) << END_LOG;
        return false;
    }

    sInfo_ = mit->second;
    return true;
}

void KResultManager::CheckAreaTypeBoost( IN const KGameInfo& kGameInfo_, OUT float& fExpBoost_, OUT float& fGpBoost_ )
{
    fExpBoost_  = 0.f;
    fGpBoost_   = 0.f;

    // 게임 모드의 배율은 이미 각 계산시에 적용되어 있음.

    // 테섭은 5배.
    if( SiKGSSimLayer()->CheckTypeFlag( ST_OPEN_TESOP ) )
    {
        fExpBoost_  += GetTestGPRatio();
        fGpBoost_   += GetTestEXPRatio();
    }


    // 계급채널은 1.1배.
    if( kGameInfo_.m_cChannelType == KChannel::CT_BEGINNER || kGameInfo_.m_cChannelType == KChannel::CT_ROOKIE ||
        kGameInfo_.m_cChannelType == KChannel::CT_EXPERT || kGameInfo_.m_cChannelType == KChannel::CT_MASTER )
    {
        fExpBoost_  += 0.1f;
        fGpBoost_   += 0.1f;
    }

    fExpBoost_ += GetExpBoost();
    fGpBoost_  += GetGpBoost();

    START_LOG( clog, L"영역 어드벤티지.." )
        << BUILD_LOG( fExpBoost_ )
        << BUILD_LOG( fGpBoost_ )
        << BUILD_LOGc( kGameInfo_.m_cChannelType )
        << BUILD_LOG( SiKGSSimLayer()->CheckTypeFlag( ST_OPEN_TESOP ) ) << END_LOG;
}

void KResultManager::CheckInventoryBoost( IN const int& nModeID_, IN const std::vector<KItem>& vecInv, IN const std::map<int,std::set<GCITEMID> >& mapGroupItemAll_,
                                          OUT float& fExpBoost_, OUT float& fGPBoost_, IN OUT std::map<GCITEMID, sBoostInfo >& mapBoostAllUser_ )
{
    // 초기화
    fExpBoost_  = 0.f;
    fGPBoost_   = 0.f;
    std::map< GCITEMID, sBoostInfo >::const_iterator mit;
    std::map< std::pair<char, char>, sBoostDetailInfo >::const_iterator mitBoostDetail;

    std::map<int,std::set<GCITEMID> > mapGroupItem;
    mapGroupItem = mapGroupItemAll_;

    for( mit = m_mapBoostInfo.begin() ; mit != m_mapBoostInfo.end() ; ++mit )
    {
        if( mit->second.m_bEquip ) // 인벤토리용이 아니면 스킵..
            continue;

        std::vector<KItem>::const_iterator vit1;
        vit1 = std::find_if( vecInv.begin(), vecInv.end(),
            boost::bind( &KItem::m_ItemID, _1 ) == mit->second.m_dwItemID );
        if( vit1 != vecInv.end() )
        {
            KInvenBuffItem kBuffItemInfo;
            if ( SiKItemManager()->GetInvenBuffItem( mit->second.m_dwItemID, kBuffItemInfo ) ) {
                // 모드 검색
                int nCategory = -1;
                if ( false == SiKChannelManager()->GetModeCategory( nModeID_, nCategory ) ) {
                    START_LOG( cwarn, L"해당하는 모드의 카테고리가 없습니다." )
                        << BUILD_LOG( kBuffItemInfo.m_ItemID )
                        << BUILD_LOG( kBuffItemInfo.m_nGroupID )
                        << BUILD_LOG( kBuffItemInfo.m_setModeCategory.size() )
                        << BUILD_LOG( nCategory )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                if ( kBuffItemInfo.m_setModeCategory.find( nCategory ) == kBuffItemInfo.m_setModeCategory.end() ) {
                    START_LOG( clog, L"인벤토리 버프아이템이지만 해당하는 모드 카테고리가 아니다." )
                        << BUILD_LOG( kBuffItemInfo.m_ItemID )
                        << BUILD_LOG( kBuffItemInfo.m_nGroupID )
                        << BUILD_LOG( kBuffItemInfo.m_setModeCategory.size() )
                        << BUILD_LOG( nCategory )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                // 그룹Item은 별도 처리
                if ( kBuffItemInfo.m_nGroupID > -1 ) {
                    mapGroupItem[kBuffItemInfo.m_nGroupID].insert( mit->second.m_dwItemID );
                    START_LOG( clog, L"인벤토리 버프아이템이지만 그룹지정이 없다." )
                        << BUILD_LOG( kBuffItemInfo.m_ItemID )
                        << BUILD_LOG( kBuffItemInfo.m_nGroupID )
                        << BUILD_LOG( kBuffItemInfo.m_setModeCategory.size() )
                        << BUILD_LOG( nCategory )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }
            }

            if( mit->second.m_bAll )
            {
                mapBoostAllUser_.insert( std::make_pair( mit->first, mit->second) );
            }
            else
            {
                mitBoostDetail = mit->second.m_mapBoostDetailInfo.find( std::make_pair( -1, -1) );
                if( mitBoostDetail != mit->second.m_mapBoostDetailInfo.end() )
                {
                    fExpBoost_  += std::max<float>(0.f, mitBoostDetail->second.m_fBoostExpRatio);
                    fGPBoost_   += std::max<float>(0.f, mitBoostDetail->second.m_fBoostGpRatio);

                    START_LOG( clog, L"인벤토리 EXP/GP 아이템(기간제)" )
                        << BUILD_LOG( mit->second.m_dwItemID )
                        << BUILD_LOG( fExpBoost_ )
                        << BUILD_LOG( fGPBoost_ ) << END_LOG;
                }
            }
        }
    }

    // 그룹 아이템 별도 처리
    CheckInventoryBoostGroupItem( mapGroupItem, mapGroupItemAll_, mapBoostAllUser_, fExpBoost_, fGPBoost_ );
    START_LOG( clog, L"인벤토리 ExpBonus : " << fExpBoost_ << L", GpBonus : " << fGPBoost_ ) << END_LOG;
}

void KResultManager::CheckInventoryBoost( IN const int& nModeID_, IN KItemContainer& kInven_, IN const std::map<int,std::set<GCITEMID> >& mapGroupItemAll_,
                                         OUT float& fExpBoost_, OUT float& fGPBoost_, IN OUT std::map<GCITEMID, sBoostInfo >& mapBoostAllUser_ )
{
    // 초기화
    fExpBoost_  = 0.f;
    fGPBoost_   = 0.f;
    std::map< GCITEMID, sBoostInfo >::const_iterator mit;
    std::map< std::pair<char, char>, sBoostDetailInfo >::const_iterator mitBoostDetail;

    std::map<int,std::set<GCITEMID> > mapGroupItem;
    mapGroupItem = mapGroupItemAll_;

    for( mit = m_mapBoostInfo.begin() ; mit != m_mapBoostInfo.end() ; ++mit )
    {
        if( mit->second.m_bEquip ) // 인벤토리용이 아니면 스킵..
            continue;

        if ( kInven_.FindItemByItemID( mit->second.m_dwItemID ) != NULL ) {

            KInvenBuffItem kBuffItemInfo;
            if ( SiKItemManager()->GetInvenBuffItem( mit->second.m_dwItemID, kBuffItemInfo ) ) {
                // 모드 검색
                int nCategory = -1;
                if ( false == SiKChannelManager()->GetModeCategory( nModeID_, nCategory ) ) {
                    START_LOG( cwarn, L"해당하는 모드의 카테고리가 없습니다." )
                        << BUILD_LOG( kBuffItemInfo.m_ItemID )
                        << BUILD_LOG( kBuffItemInfo.m_nGroupID )
                        << BUILD_LOG( kBuffItemInfo.m_setModeCategory.size() )
                        << BUILD_LOG( nCategory )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                if ( kBuffItemInfo.m_setModeCategory.find( nCategory ) == kBuffItemInfo.m_setModeCategory.end() ) {
                    START_LOG( clog, L"인벤토리 버프아이템이지만 해당하는 모드 카테고리가 아니다." )
                        << BUILD_LOG( kBuffItemInfo.m_ItemID )
                        << BUILD_LOG( kBuffItemInfo.m_nGroupID )
                        << BUILD_LOG( kBuffItemInfo.m_setModeCategory.size() )
                        << BUILD_LOG( nCategory )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }

                // 그룹Item은 별도 처리
                if ( kBuffItemInfo.m_nGroupID > -1 ) {
                    mapGroupItem[kBuffItemInfo.m_nGroupID].insert( mit->second.m_dwItemID );
                    START_LOG( clog, L"인벤토리 그룹 버프아이템." )
                        << BUILD_LOG( kBuffItemInfo.m_ItemID )
                        << BUILD_LOG( kBuffItemInfo.m_nGroupID )
                        << BUILD_LOG( kBuffItemInfo.m_setModeCategory.size() )
                        << BUILD_LOG( nCategory )
                        << BUILD_LOG( nModeID_ ) << END_LOG;
                    continue;
                }
            }

            if( mit->second.m_bAll ) {
                mapBoostAllUser_.insert( std::make_pair( mit->first, mit->second) );
            }
            else {
                mitBoostDetail = mit->second.m_mapBoostDetailInfo.find( std::make_pair( -1, -1) );
                if( mitBoostDetail != mit->second.m_mapBoostDetailInfo.end() ) {
                    fExpBoost_  += std::max<float>(0.f, mitBoostDetail->second.m_fBoostExpRatio);
                    fGPBoost_   += std::max<float>(0.f, mitBoostDetail->second.m_fBoostGpRatio);

                    START_LOG( clog, L"인벤토리 EXP/GP 아이템" )
                        << BUILD_LOG( mit->second.m_dwItemID )
                        << BUILD_LOG( fExpBoost_ )
                        << BUILD_LOG( fGPBoost_ ) << END_LOG;
                }
            }
        }
    }

    // 그룹 아이템 별도 처리
    CheckInventoryBoostGroupItem( mapGroupItem, mapGroupItemAll_, mapBoostAllUser_, fExpBoost_, fGPBoost_ );
    START_LOG( clog, L"인벤토리 ExpBonus : " << fExpBoost_ << L", GpBonus : " << fGPBoost_ ) << END_LOG;
}

void KResultManager::CheckEquipBoost( IN char cCharType_, IN char cPromotion_, IN const std::vector<KEquipItemInfo>& vecEquip_,
                                      IN OUT float& fExpBoost_, IN OUT float& fGPBoost_, OUT std::map<DWORD,sBoostInfo>& mapAllBoostInfo_ )
{

    std::vector<KEquipItemInfo>::const_iterator vit;
    for( vit = vecEquip_.begin() ; vit != vecEquip_.end() ; ++vit )
    {
        std::map< GCITEMID, sBoostInfo >::const_iterator mit;
        std::vector< sBoostDetailInfo >::const_iterator vitBoostDetail;
        std::map< std::pair<char, char>, sBoostDetailInfo >::const_iterator mitBoostDetail;
        mit = m_mapBoostInfo.find( vit->m_dwID );
        if( mit != m_mapBoostInfo.end() && mit->second.m_bEquip ) // 장착 중이고, 장착옵션이면.
        {
            if( mit->second.m_bAll )
            {
                // 중복된값은 들어가지 않으므로 중복에 대한 처리는 따로 하지 않는다.
                mapAllBoostInfo_.insert( std::make_pair(mit->second.m_dwItemID, mit->second) );
            }
            else
            {
                mitBoostDetail = mit->second.m_mapBoostDetailInfo.find( std::make_pair( cCharType_, cPromotion_ ) );
                if( mitBoostDetail != mit->second.m_mapBoostDetailInfo.end() )
                {
                    fExpBoost_  += std::max<float>(0.f, mitBoostDetail->second.m_fBoostExpRatio);
                    fGPBoost_   += std::max<float>(0.f, mitBoostDetail->second.m_fBoostGpRatio);

                    START_LOG( clog, L"장착 EXP/GP 아이템" )
                        << BUILD_LOG( vit->m_dwID )
                        << BUILD_LOGc( cCharType_ )
                        << BUILD_LOGc( cPromotion_ )
                        << BUILD_LOG( fExpBoost_ )
                        << BUILD_LOG( fGPBoost_ )
                        << BUILD_LOGc( mitBoostDetail->first.first )
                        << BUILD_LOGc( mitBoostDetail->first.second ) << END_LOG;

                    continue;
                }

                mitBoostDetail = mit->second.m_mapBoostDetailInfo.find( std::make_pair( cCharType_, -1 ) );
                if( mitBoostDetail != mit->second.m_mapBoostDetailInfo.end() )
                {
                    fExpBoost_  += mitBoostDetail->second.m_fBoostExpRatio;
                    fGPBoost_   += mitBoostDetail->second.m_fBoostGpRatio;

                    START_LOG( clog, L"장착 EXP/GP 아이템, 모든 전직 공용" )
                        << BUILD_LOG( vit->m_dwID )
                        << BUILD_LOGc( mitBoostDetail->first.first )
                        << BUILD_LOGc( mitBoostDetail->first.second )
                        << BUILD_LOG( fExpBoost_ )
                        << BUILD_LOG( fGPBoost_ ) << END_LOG;
                    continue;
                }

                mitBoostDetail = mit->second.m_mapBoostDetailInfo.find( std::make_pair( -1, -1 ) ); // 공통 적용
                if( mitBoostDetail != mit->second.m_mapBoostDetailInfo.end() )
                {
                    fExpBoost_  += mitBoostDetail->second.m_fBoostExpRatio;
                    fGPBoost_   += mitBoostDetail->second.m_fBoostGpRatio;

                    START_LOG( clog, L"장착 EXP/GP 아이템, 전캐릭 공용" )
                        << BUILD_LOG( vit->m_dwID )
                        << BUILD_LOGc( mitBoostDetail->first.first )
                        << BUILD_LOGc( mitBoostDetail->first.second )
                        << BUILD_LOG( fExpBoost_ )
                        << BUILD_LOG( fGPBoost_ ) << END_LOG;

                    continue; // 남겨 놓은 이유, 일관성 ^^
                }
            }
        }
    }
}

void KResultManager::CheckEquipSocketBoost( IN const std::vector<KSocketInfo>& vecSocet_, OUT float& fExpBoost_, OUT float& fGPBoost_ )
{

    std::vector<KSocketInfo>::const_iterator cvitSoc;
    for ( cvitSoc = vecSocet_.begin() ; cvitSoc != vecSocet_.end() ; ++cvitSoc ) {

        if ( cvitSoc->m_cState != KSocketInfo::STS_USING ) continue;

        std::list<std::pair<int,float> >::iterator litAtt;
        std::list<std::pair<int,float> > AttributeList;
        {
            int nGradeTableID = SiKItemManager()->GetGradeTableID( cvitSoc->m_CardItemID );
            char cGrade = (char)SiKItemManager()->m_kGradeDecider.GetGrade( nGradeTableID );

            int nAttributeTableID = SiKItemManager()->GetAttributeTableID( cvitSoc->m_CardItemID );
            if ( SiKItemManager()->m_kAttributeDecider.GetTableType( nAttributeTableID, (int)cGrade ) != KGCAttributeDecider::Table::ATT_FIXED ) {
                START_LOG( cwarn, L"소켓에 박혀 있는 카드의 속성이 고정이 아님.."
                    << L" Card ID : " << cvitSoc->m_CardItemID
                    << L", Att ID  : " << nAttributeTableID
                    << L", Grade : " << (int)cGrade ) << END_LOG;
                continue;
            }

            int nAttributeCount = SiKItemManager()->GetAttributeCountFromGrade( cGrade );
            SiKItemManager()->m_kAttributeDecider.GetAttributeList( nAttributeTableID, nAttributeCount, (int)cGrade, AttributeList );

            if ( AttributeList.empty() ) {
                START_LOG( cwarn, L"소켓에 박혀 있는 카드의 속성이 없음."
                    << L" Card ID : " << cvitSoc->m_CardItemID
                    << L", Att ID  : " << nAttributeTableID
                    << L", Grade : " << (int)cGrade
                    << L", Att Count : " << nAttributeCount ) << END_LOG;
                continue;
            }
        }

        for ( litAtt = AttributeList.begin() ; litAtt != AttributeList.end() ; ++litAtt ) {

            switch ( litAtt->first ) {
                case ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE : fGPBoost_ += std::max<float>(0.f, (litAtt->second * 0.01f )); break;
                case ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE : fExpBoost_ += std::max<float>(0.f, (litAtt->second * 0.01f )); break;
            }
        }
    }
}

void KResultManager::CheckRankBoost( IN const KGameInfo& kGameInfo_, IN int nRankPoint_, OUT float& fExpBoost_, OUT float& fGPBoost_ )
{
    if( !KRoom::IsDungeonMode( kGameInfo_.m_iGameMode ) ) {
        return;
    }

    float fBonus = 0.f;

    if( kGameInfo_.m_nDifficulty >= GC_GMD_NORMAL_BEGIN && kGameInfo_.m_nDifficulty < GC_GMD_CHAMPION_BEGIN ) {
        fBonus = GetRankBonus( nRankPoint_, kGameInfo_.m_iGameMode ); // 일반모드 랭킹 보너스
    }
    else if( kGameInfo_.m_nDifficulty >= GC_GMD_CHAMPION_BEGIN ) {
        fBonus = GetHellRankBonus( nRankPoint_ ); // 헬모드 랭킹 보너스
    }
    else {// 정의되지 않은 모드
        START_LOG( cerr, L"정의되지 않은 난이도 : " << kGameInfo_.m_nDifficulty ) << END_LOG;
    }

    fExpBoost_ += std::max<float>(0.f, fBonus);
    fGPBoost_ += std::max<float>(0.f, fBonus);

    START_LOG( clog, L"랭크 보너스 적용" )
        << BUILD_LOG( kGameInfo_.m_iGameMode )
        << BUILD_LOG( kGameInfo_.m_nDifficulty )
        << BUILD_LOG( fBonus )
        << BUILD_LOG( fExpBoost_ )
        << BUILD_LOG( fGPBoost_ ) << END_LOG;
}

void KResultManager::CalcExpGpBoostProcess( IN const KGameInfo& kGameInfo_, IN OUT KEndGameReq& kReq_ )
{
    std::map<DWORD, sBoostInfo > mapBoostAllUser; // 모든 애들에게 적용할것.
    mapBoostAllUser.clear();

    float fBoostExpArea = 0.f;
    float fBoostGpArea = 0.f;
    CheckAreaTypeBoost( kGameInfo_, fBoostExpArea, fBoostGpArea );

    KRainBowData kRainbowGP; // 생성자에서 확률이 0.f으로 초기화 됨.
    SiKRainbow()->GetEventByID( KRainbow::RE_GP_BONUS, kRainbowGP );
    KRainBowData kRainbowExp; // 생성자에서 확률이 0.f으로 초기화 됨.
    if( KRoom::IsDungeonMode( kGameInfo_.m_iGameMode ) )
    {
        SiKRainbow()->GetEventByID( KRainbow::RE_DUNGEON_EXP, kRainbowExp );
    }
    else if( KRoom::IsPvPMode( kGameInfo_.m_iGameMode ) )
    {
        SiKRainbow()->GetEventByID( KRainbow::RE_PVP_EXP, kRainbowExp );
    }

    // 전체 적용 그룹 아이템
    std::map<int,std::set<GCITEMID> > mapGroupItemAll;
    GetGroupItemAll( kReq_.m_vecGameResult, kGameInfo_.m_iGameMode, mapGroupItemAll );

    std::vector<KGameResultIn>::iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) // 플레이한 유저 루프
    {
        if( vit->m_vecCharExp.empty() )
        {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            continue;
        }

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;
        
        // 이 안에서 배율 초기화 한다.
        CheckInventoryBoost( kGameInfo_.m_iGameMode, spUser->m_kInventory, mapGroupItemAll, vit->m_mapExpBoost[KCharacterExpInfoOut::BT_ITEM],
                             vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ITEM], mapBoostAllUser );

        if ( vit->m_bWin ) {
            // 던전 클리어 이벤트
            CheckDungeonClearBoost( kGameInfo_,  vit->m_mapExpBoost[KCharacterExpInfoOut::BT_CLEAR],
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_CLEAR] );
            // 몬스터 원정대 클리어
            CheckMonsterCrusaderClearBoost( kGameInfo_,  vit->m_mapExpBoost[KCharacterExpInfoOut::BT_CLEAR],
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_CLEAR] );

            // 유저의 랭크에 따른 경험치 추가 획득(컨트롤 피드백 시스템)
            ControlFeedbackExpBoost( kGameInfo_,  vit->m_mapExpBoost[KCharacterExpInfoOut::BT_RANK], vit->m_kUserControlInfoData.m_nRank );
        }

        // 레인 보우 이벤트에서 GP 상승.
        vit->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT]  += std::max<float>(0.f, kRainbowGP.m_fPrimaryRatio);

        //-- 이아래 에서 경험치에 추가 배율 적용  해야 함. --//
        if( spUser->m_MyRecomInfo.m_nState >= KRecomHelper::RBS_ACCEPT_RECOM && SiKRecomHelper()->IsAdjustTerm( spUser->m_MyRecomInfo ) )
        {
            std::vector<std::pair<DWORD,int> >::const_iterator vitMe;
            std::vector<std::pair<DWORD,int> >::const_iterator vitOrther;
            // 나의 팀을 찾는다.
            vitMe = std::find_if( kGameInfo_.m_vecUIDTeam.begin(), kGameInfo_.m_vecUIDTeam.end(), boost::bind( &std::pair<DWORD,int>::first, _1) == spUser->GetUID() );
            vitOrther = std::find_if( kGameInfo_.m_vecUIDTeam.begin(), kGameInfo_.m_vecUIDTeam.end(), boost::bind( &std::pair<DWORD,int>::first, _1) == spUser->m_MyRecomInfo.m_dwLoginUID );
            // 나의 상대가 나와 같은 팀이라면..
            if( vitMe != kGameInfo_.m_vecUIDTeam.end() && vitOrther != kGameInfo_.m_vecUIDTeam.end() &&
                vitMe->second == vitOrther->second )
            {
                vit->m_mapExpBoost[KCharacterExpInfoOut::BT_ETC] += std::max<float>(0.f, SiKRecomHelper()->GetPartyPlayExpBonus());
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ETC]  += std::max<float>(0.f, SiKRecomHelper()->GetPartyPlayGpBonus());
            }
        }

        std::vector< KCharacterExpInfo >::iterator vitExp;
        for( vitExp = vit->m_vecCharExp.begin() ; vitExp != vit->m_vecCharExp.end() ; ++vitExp )
        {
            const KCharacterInfo* pkCharacterInfo = spUser->GetCharacterInfo( vitExp->m_cCharType );
            if( !pkCharacterInfo )
                continue;

            CheckEquipAttributeBoost( spUser->m_kInventory, pkCharacterInfo->m_vecEquipItems,vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_ITEM], vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ITEM] );
            CheckEquipBoost( pkCharacterInfo->m_cCharType, vitExp->m_cCharJob , pkCharacterInfo->m_vecEquipItems, vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_ITEM], vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ITEM], mapBoostAllUser );
            CheckEquipSetBoost( pkCharacterInfo->m_vecEquipItems, vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_ITEM], vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ITEM] );
            // 코디세트 보너스
            CheckCoordiSetBoost( pkCharacterInfo->m_cCharType, spUser->m_mapLookEquip, spUser->m_kInventory, vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_ITEM], vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ITEM] );

            std::vector<KEquipItemInfo>::const_iterator cvitEquip;
            for ( cvitEquip = pkCharacterInfo->m_vecEquipItems.begin() ; cvitEquip != pkCharacterInfo->m_vecEquipItems.end() ; ++cvitEquip ) {
                CheckEquipSocketBoost( cvitEquip->m_vecSocket, vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_ITEM], vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ITEM] );
            }


            // PC방 유저일때의 EXP,GP,SPX 추가 획득 처리.
            bool bPCBangUser = spUser->IsPCBangUser();
            if( bPCBangUser ) {
                float fExpRatio = 0.f;
                float fGpRatio = 0.f;
                float fSpxRatio = 0.f;
                SiKIPAdvantage()->GetExpGpSpxBonus( KIPAdvantage::PCBANG_PUBLISHER, fExpRatio, fGpRatio, fSpxRatio );
                vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_PC]    += std::max<float>(0.f, fExpRatio);
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_PC]        += std::max<float>(0.f, fGpRatio);
            }

            // 추가 혜택받는 유저일때의 EXP,GP,SPX 추가 획득 처리.
            bool bTH3BBUser = spUser->IsTH3BBUser();
            if( bTH3BBUser ) {
                float fExpRatio = 0.f;
                float fGpRatio = 0.f;
                float fSpxRatio = 0.f;
                SiKIPAdvantage()->GetExpGpSpxBonus( KIPAdvantage::TH3BB_USER, fExpRatio, fGpRatio, fSpxRatio );
                vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_PC]    += std::max<float>(0.f, fExpRatio);
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_PC]        += std::max<float>(0.f, fGpRatio);
                vitExp->m_mapSpxBoost[KCharacterExpInfoOut::BT_PC]    += std::max<float>(0.f, fSpxRatio);
            }

            // 한국. PC방 User가 아닐때에서 한방에 PC방 유저의 숫자만큼 EXP,GP,SPX 더 받기.
            // IPAdvantage Class에서 ON/OFF로 제어.
            if( bPCBangUser || SiKIPAdvantage()->CheckUserPerRatio() ) {
                float fExpRatio = 0.f;
                float fGpRatio = 0.f;
                float fSpxRatio = 0.f;
                SiKIPAdvantage()->GetUserPerRatio( KIPAdvantage::PCBANG_PUBLISHER, (bPCBangUser ? kGameInfo_.m_nPCBangUserNum-1 : kGameInfo_.m_nPCBangUserNum), fExpRatio, fGpRatio, fSpxRatio );
                vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_PC]    += std::max<float>(0.f, fExpRatio);
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_PC]        += std::max<float>(0.f, fGpRatio);
            }

            // 남미 PC방 유저일때의 EXP,GP 추가 획득 처리.
            bool bLINBenefitUser = spUser->IsLINBenefitUser();
            if( bLINBenefitUser ) {
                char cLinBenefitType = spUser->GetUserBenfitType();
                float fExpRatio = 0.f;
                float fGpRatio = 0.f;
                float fSpxRatio = 0.f;
                SiKIPAdvantage()->GetExpGpSpxBonus( cLinBenefitType, fExpRatio, fGpRatio, fSpxRatio );
                vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_PC]    += std::max<float>(0.f, fExpRatio);
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_PC]        += std::max<float>(0.f, fGpRatio);
            }

            // 커플 보너스
            spUser->CoupleBoostCalc( vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_COUPLE], vit->m_mapGpBoost[KCharacterExpInfoOut::BT_COUPLE] );

            // 레인 보우 이벤트에서 EXP 상승.
            vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT]  += std::max<float>(0.f, kRainbowExp.m_fPrimaryRatio);

            // 채널 타입 이벤트
            if( SiKChannelTypeEvent()->IsEventTerm() ) {
                float fExpRatio = 0.f;
                float fGpRatio = 0.f;

                SiKChannelTypeEvent()->GetExpGpBonus( spUser->GetUserChannelType(), fExpRatio, fGpRatio );

                vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT]  += std::max<float>(0.f, fExpRatio);
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT]      += std::max<float>(0.f, fGpRatio);
            }

            // 길드 보너스
            float fGuildExpBonus = spUser->GetGuildBonus(KGuildConfig::GA_EXP_BONUS);
            if ( fGuildExpBonus > 0.f ) {
                vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_GUILD] += fGuildExpBonus;
            }

            // 아지트 요정의나무 열매버프 EXP,GP 추가 획득 처리.
            float fExpRatio = 0.f;
            float fGpRatio = 0.f;
            fExpRatio = spUser->GetFairyTreeExpBonus();
            fGpRatio = spUser->GetFairyTreeGpBonus();
            if ( fExpRatio > 0.f ) {
                vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_AGIT]    += std::max<float>(0.f, fExpRatio);
            }
            if ( fGpRatio > 0.f ) {
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_AGIT]        += std::max<float>(0.f, fGpRatio);
            }
            START_LOG( clog, L"Agit Buff Bonus.. ExpRatio : " << fExpRatio << L", GpRatio : " << fGpRatio << L", UserLogin : " << spUser->GetName() ) << END_LOG;

            // 채널 타입 이벤트
            if( SiKGWCEvent()->IsRun() ) {
                float fExpRatio = 0.f;
                float fGpRatio = 0.f;

                if( SiKGWCEvent()->GetGradeReward( fExpRatio, fGpRatio ) ) {
                    vitExp->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT]  += std::max<float>(0.f, fExpRatio);
                    vit->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT]      += std::max<float>(0.f, fGpRatio);
                }
            }

            // 호위 이벤트 Exp, Gp 보상 지급
            if ( kReq_.m_bIsActiveEscortEvent && vit->m_bWin ) {
                CheckEscortBoost( kReq_.m_bIsEscortMonsterAlive, kReq_.m_dwEscortMonsterID, kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, vitExp->m_mapExpBoost[ KCharacterExpInfoOut::BT_EVENT ], vit->m_mapGpBoost[ KCharacterExpInfoOut::BT_EVENT ] );
            }
            // 호위 이벤트 Exp, Gp 보상 지급

            // 경험치 롤백 확인 코드.1
            if( vitExp->m_nBaseExp < 0 || vitExp->m_nAdditionnalExp < 0 ) {
                START_LOG( cerr, L"KResultManager::CalcExpGpBoostProcess Check #2" )
                    << BUILD_LOGc( vitExp->m_cCharType )
                    << BUILD_LOGc( vitExp->m_cCharJob )
                    << BUILD_LOG( vitExp->m_nBaseExp )
                    << BUILD_LOG( vitExp->m_nAdditionnalExp ) << END_LOG;
            }
        }
    }

    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        if( vit->m_vecCharExp.empty() )
        {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            continue;
        }

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser ) {
            continue;
        }

        std::map< DWORD, sBoostInfo >::const_iterator mit;
        
        std::map< std::pair<char, char>, sBoostDetailInfo >::const_iterator mitBoostDetail;

        for( mit = mapBoostAllUser.begin() ; mit != mapBoostAllUser.end() ; ++mit )
        {
            mitBoostDetail = mit->second.m_mapBoostDetailInfo.find( std::make_pair( -1, -1) );
            if( mitBoostDetail != mit->second.m_mapBoostDetailInfo.end() )
            {
                vit->m_mapExpBoost[KCharacterExpInfoOut::BT_ITEM] += std::max<float>(0.f, mitBoostDetail->second.m_fBoostExpRatio);
                vit->m_mapGpBoost[KCharacterExpInfoOut::BT_ITEM]  += std::max<float>(0.f, mitBoostDetail->second.m_fBoostGpRatio);
                START_LOG( clog, L"방의 모든 유저에게 적용되는 아이템" )
                    << BUILD_LOG( mit->first )                    
                    << BUILD_LOG( mitBoostDetail->second.m_fBoostExpRatio )
                    << BUILD_LOG( mitBoostDetail->second.m_fBoostGpRatio ) << END_LOG;
            }
        }

        // 서버나 채널 게임 타입에 따라 추가 보너스.
        vit->m_mapExpBoost[KCharacterExpInfoOut::BT_AREA] += std::max<float>(0.f, fBoostExpArea);
        vit->m_mapGpBoost[KCharacterExpInfoOut::BT_AREA]  += std::max<float>(0.f, fBoostGpArea);

        std::vector< KCharacterExpInfo >::iterator vitExp;
        for( vitExp = vit->m_vecCharExp.begin() ; vitExp != vit->m_vecCharExp.end() ; ++vitExp ) {
            // 경험치 롤백 확인 코드.2
            if( vitExp->m_nBaseExp < 0 || vitExp->m_nAdditionnalExp < 0 ) {
                START_LOG( cerr, L"KResultManager::CalcExpGpBoostProcess Check #2" )
                    << BUILD_LOGc( vitExp->m_cCharType )
                    << BUILD_LOGc( vitExp->m_cCharJob )
                    << BUILD_LOG( vitExp->m_nBaseExp )
                    << BUILD_LOG( vitExp->m_nAdditionnalExp ) << END_LOG;
            }
        }
    }
}

void KResultManager::CalcExpGP( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    std::map< int, Handler >::iterator mit;
    mit = m_mapFunction.find( kGameInfo_.m_iGameMode );
    if( mit == m_mapFunction.end() ) {
        START_LOG( cerr, L"게임 종료 처리중 옳지 않은 모드를 플레이함." ) << END_LOG;
        SET_ERROR( ERR_RESULT_COMMON_00 );
        std::vector<KGameResultIn>::iterator vit;
        for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
        {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
        }
        return;
    }

    ChkWinWinBug( kGameInfo_, kReq_ );
    (this->*(mit->second))( kGameInfo_, mapResultExp_, mapResultGP_, kReq_ ); // 해당 모드에 대한 기본 경험치를 얻었다. REG_HANDLER 에서 등록한 함수에서 경험치 계산

    AdjustCharaterLimit( kReq_ );

    // 장착 아이템/ 채널 속성 / 서버 속성에 따른 추가 보상.(게임 모드는 각 게임 결과 처리에서 계산되어 있음.)
    CalcExpGpBoostProcess( kGameInfo_, kReq_ );

}

void KResultManager::InitResultIn( OUT KGameResultIn& kResultIn_ )
{
    kResultIn_.m_mapExpBoost.clear();
    kResultIn_.m_mapGpBoost.clear();
    kResultIn_.m_mapSpxBoost.clear();
    kResultIn_.m_nBaseGP = 0;
    kResultIn_.m_nTotalRecvGP = 0;
    kResultIn_.m_nAddtionalGp = 0;

    std::vector< KCharacterExpInfo >::iterator vit;
    for( vit = kResultIn_.m_vecCharExp.begin() ; vit != kResultIn_.m_vecCharExp.end() ; ++vit )
    {
        vit->m_mapExpBoost.clear();
        vit->m_mapExpBoost.clear();
        vit->m_nBaseExp = 0;
        vit->m_nTotalRecvExp = 0;
        vit->m_nAdditionnalExp = 0;
        vit->m_dwTotalRecvSpx = 0;
        
    }
}

void KResultManager::CalcDefaultExpGP( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    std::vector<KGameResultIn>::iterator vit;
    for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
        InitResultIn( *vit );

        if ( vit->m_vecCharExp.empty() ) {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }

        vit->m_vecCharExp.resize( 1 ); // 태그 매치가 아니면 모두 1명이다.

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if( !spUser ) continue;

        const KCharacterInfo* pkPlayCharacterInfo = spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
        if( !pkPlayCharacterInfo )
            continue;

        // 클라이언트 디스플레이 전달용
        vit->m_vecCharExp.begin()->m_nBaseExp = static_cast<int>( mapResultExp_[ std::make_pair(vit->m_dwUID,vit->m_vecCharExp.begin()->m_cCharType) ] );
        vit->m_nBaseGP = mapResultGP_[ vit->m_dwUID ];

        // 승리시 : 클리어보너스 Exp/GP = 총 획득 Exp/GP * 승리 배율(2.0f)
        // 패배시 : 클리어보너스 Exp/GP = 총 획득 Exp/GP * 패배 배율(1.2f)
        int nBonusExp = 0;
        int nBonusGP = 0;

        if ( vit->m_bWin ) {
            nBonusExp = static_cast<int>( mapResultExp_[ std::make_pair( vit->m_dwUID, vit->m_vecCharExp.begin()->m_cCharType ) ] * GetWinRatio() );
            nBonusGP = static_cast<int>( mapResultGP_[ vit->m_dwUID ] * GetWinRatio() );
        }
        else {
            nBonusExp = static_cast<int>( mapResultExp_[ std::make_pair( vit->m_dwUID, vit->m_vecCharExp.begin()->m_cCharType ) ] * GetLoseRatio() );
            nBonusGP = static_cast<int>( mapResultGP_[ vit->m_dwUID ] * GetLoseRatio() );
        }

        if ( SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_PVP_CHECK, KPreHackingCheckManager::OPT_CHECK_ENABLE ) ) {
            __int64 biCurrentBonusExp = static_cast<__int64>(nBonusExp);
            LIF( spUser->PvpHackCheck( pkPlayCharacterInfo->m_cCharType, pkPlayCharacterInfo->m_dwLevel, static_cast<__int64>(vit->m_vecCharExp.begin()->m_nBaseExp), biCurrentBonusExp ) );
            nBonusExp = static_cast<int>(biCurrentBonusExp);
        }

        // 대전 클리어에 한해서만 2배 배율 적용.
        // CalcBaseExpGpSpxRatio( nBonusExp, nBonusGP );
        if( kGameInfo_.m_iGameMode == GC_GM_AUTOMATCH_TEAM ) {
            CalcELOExpGpRatio( nBonusExp, nBonusGP );
        }
        vit->m_vecCharExp.begin()->m_nAdditionnalExp = nBonusExp;
        vit->m_nAddtionalGp = nBonusGP;

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
            START_LOG( cwarn, L"--- Dump CalcDefaultExpGP ---" << dbg::endl
                << L" Login : " << spUser->GetName() << dbg::endl
                << L" vit->m_vecCharExp.begin()->m_nBaseExp : " << vit->m_vecCharExp.begin()->m_nBaseExp << dbg::endl
                << L" vit->m_nBaseGP : " << vit->m_nBaseGP << dbg::endl
                << L" vit->m_vecCharExp.begin()->m_nAdditionnalExp : " << vit->m_vecCharExp.begin()->m_nAdditionnalExp << dbg::endl
                << L" vit->m_nAddtionalGp : " << vit->m_nAddtionalGp << dbg::endl
                << L" nBonusExp : " << nBonusExp << dbg::endl
                << L" nBonusGP : " << nBonusGP << dbg::endl
                << L" vit->m_bWin : " << (vit->m_bWin?L"TRUE":L"FALSE") << dbg::endl
                << L"--- End Dump ---" ) << END_LOG;
        }
    }
}

void KResultManager::CalcMatchExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    if ( SiKGSSimLayer()->CheckTypeFlag( ST_PVP_BALANCE ) && SiKGSSimLayer()->CheckAuthTypeFlag( AT_LEVELUP ) ) {

        std::vector<KGameResultIn>::iterator vit;
        for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
            InitResultIn( *vit );
        }

        SET_ERROR( NET_OK );
        return;
    }
    // 승 패는 클라이언트가 보내준다.^^
    CalcDefaultExpGP( kGameInfo_, mapResultExp_, mapResultGP_, kReq_ );
    CalcGuildPoint( kReq_, KGuildConfig::PVP );

    SET_ERROR( NET_OK );
}

void KResultManager::CalcGuildPoint( IN OUT KEndGameReq& kReq_, IN const int benefitKind_ )
{
    std::vector<KGameResultIn>::iterator vit;
    for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if( !spUser ) continue;

        if ( vit->m_bWin ) {
            vit->m_iContPoint = SiKGuildManager()->GetGuildPointDiff( benefitKind_, true );
            // 여기에서는 더해질 길드포인트 값만 구하고 적용은 AdjustExpGp 안의 UpdateGuildPoint 에서 한다.
            START_LOG( clog, L"Guild Contribution point (get) : " << spUser->GetName() )
                << BUILD_LOG( vit->m_iContPoint ) << END_LOG;
        }

        else {
            vit->m_iContPoint = SiKGuildManager()->GetGuildPointDiff( benefitKind_, false );

            START_LOG( clog, L"Guild Contribution point (get) : " << spUser->GetName() )
                << BUILD_LOG( vit->m_iContPoint ) << END_LOG;
        }
    }
}

void KResultManager::CalcTagMatchExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    // 태그매치 모드는 절반이다..
    std::vector<KGameResultIn>::iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        InitResultIn( *vit );

        if( vit->m_vecCharExp.empty() )
        {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;

        std::vector< KCharacterExpInfo >::iterator vitExp;
        for( vitExp = vit->m_vecCharExp.begin() ; vitExp != vit->m_vecCharExp.end() ; ++vitExp )
        {
            const KCharacterInfo* pkPlayCharacterInfo = spUser->GetCharacterInfo( vitExp->m_cCharType );
            if( !pkPlayCharacterInfo )
                continue;

            // 경험치 계산( 같은 팀원에게도 동일하게 적용되어야 한다. )
            // 승리시 : 클리어보너스 Exp/GP = 총 획득 Exp/GP * 승리 배율(2.0f)
            // 패배시 : 클리어보너스 Exp/GP = 총 획득 Exp/GP * 승리 배율(1.2f)
            if( vit->m_bWin )
            {
                vitExp->m_nBaseExp = static_cast<int>(SiKGameDrop()->GetCharLvMonExp( pkPlayCharacterInfo->m_dwLevel ) * ( GetBaseExpRatio() * GetWinRatio() ) );
                vit->m_nBaseGP += static_cast<int>(SiKGameDrop()->GetCharLvMonExp( pkPlayCharacterInfo->m_dwLevel ) * GetWinRatio() ) / kGameInfo_.m_nStartUserNum;
            }
            else
            {
                vitExp->m_nBaseExp = static_cast<int>( SiKGameDrop()->GetCharLvMonExp( pkPlayCharacterInfo->m_dwLevel ) * ( GetBaseExpRatio() * GetLoseRatio() ) );
                vit->m_nBaseGP += static_cast<int>( SiKGameDrop()->GetCharLvMonExp( pkPlayCharacterInfo->m_dwLevel ) * GetLoseRatio() ) / kGameInfo_.m_nStartUserNum;
            }

            // 대전 클리어에 한해서만 2배 배율 적용.
            // CalcBaseExpGpSpxRatio( vitExp->m_nBaseExp, vit->m_nBaseGP );
        }
    }

    CalcGuildPoint( kReq_, KGuildConfig::PVP );

    SET_ERROR( NET_OK );
}

void KResultManager::CalcQuestExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    std::vector<KGameResultIn>::iterator vit;
    for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
        // 보상을 초기화 한다.
        InitResultIn( *vit );

        if ( vit->m_vecCharExp.empty() ) {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }

        vit->m_vecCharExp.resize( 1 ); // 태그 매치가 아니면 모두 1명이다.

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( !spUser ) continue;

        // vit->m_vecCharExp의 사이즈가 0이 아니기 때문에 사용한다. (캐릭터얻는데만 사용)
        const KCharacterInfo* pkPlayCharacterInfo = spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
        if ( !pkPlayCharacterInfo ) continue;

        // 클리어 보너스Exp/GP
        int nBonusExp = 0;
        int nBonusGP = 0;
        // 지면 던전 클리어 보상을 받을 수 없다.
/*
        if ( vit->m_bWin ) {
            nBonusExp = static_cast<int>( mapResultExp_[ std::make_pair( vit->m_dwUID, vit->m_vecCharExp.begin()->m_cCharType ) ] * GetClearBonusRatio() );
            nBonusGP = static_cast<int>( mapResultGP_[vit->m_dwUID] * GetClearBonusRatio() );
        }
*/
        // 클라이언트 (디스플레이)전달용
        vit->m_vecCharExp.begin()->m_nBaseExp = static_cast<int>( mapResultExp_[ std::make_pair( vit->m_dwUID, vit->m_vecCharExp.begin()->m_cCharType ) ] );
        vit->m_nBaseGP = mapResultGP_[vit->m_dwUID];

        // 대전 클리어에 한해서만 2배 배율 적용.
        // CalcBaseExpGpSpxRatio( nBonusExp, nBonusGP );
        vit->m_vecCharExp.begin()->m_nAdditionnalExp = nBonusExp;
        vit->m_nAddtionalGp = nBonusGP;

        START_LOG( clog, L" Dungeon Clear..Name : " << spUser->GetName() )
            << BUILD_LOGc( vit->m_vecCharExp.begin()->m_cCharType )
            << BUILD_LOG( vit->m_vecCharExp.begin()->m_nBaseExp )
            << BUILD_LOG( vit->m_vecCharExp.begin()->m_nAdditionnalExp )
            << BUILD_LOG( vit->m_nBaseGP )
            << BUILD_LOG( vit->m_nAddtionalGp ) << END_LOG;
    }

    CalcGuildPoint( kReq_, KGuildConfig::DUNGEON );
}

void KResultManager::CalcMonsterHuntExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    sMonsterHuntInfo kInfo;
    if( !GetMonsterHuntInfo( kGameInfo_.m_nDifficulty, kInfo ) )
    {
        SET_ERROR( ERR_RESULT_COMMON_02 );
        return;
    }

    START_LOG( clog, L"Monster Hunt Mod Info " )
         << BUILD_LOG( kGameInfo_.m_iGameMode )
         << BUILD_LOG( kGameInfo_.m_nDifficulty )
         << BUILD_LOG( kInfo.m_nClearPoint )
         << BUILD_LOG( kInfo.m_nDifficult )
         << BUILD_LOG( kInfo.m_nExp )
         << BUILD_LOG( kInfo.m_nGp ) << END_LOG;

    std::vector<KGameResultIn>::iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        // 보상을 초기화 한다.
        InitResultIn( *vit );

        if( vit->m_vecCharExp.empty() )
        {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }
        vit->m_vecCharExp.resize( 1 ); // 태그 매치가 아니면 모두 1명이다.

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;

        if( kInfo.m_nClearPoint < 0 )
            kInfo.m_nClearPoint = 1;

        float fAchieveRatio = vit->m_nScore * 1.f / kInfo.m_nClearPoint;

        // 조금 받으면 그걸 적용
        if( fAchieveRatio < 1.0f )
        {
            vit->m_vecCharExp.begin()->m_nBaseExp = (int)(kInfo.m_nExp * fAchieveRatio);
            vit->m_nBaseGP = (int)(kInfo.m_nGp * fAchieveRatio);
        }
        else // 많거나 딱이면 기본값.
        {
            vit->m_vecCharExp.begin()->m_nBaseExp = kInfo.m_nExp;
            vit->m_nBaseGP = kInfo.m_nGp;
        }

        // 딱이상 이면 추가로 기본의 25% 보너스
        if( fAchieveRatio >= 1.0f )
        {
            vit->m_vecCharExp.begin()->m_nBaseExp += (int)(kInfo.m_nExp * 0.25f);
            vit->m_nBaseGP += (int)(kInfo.m_nGp * 0.25f);
        }

        // 1.5배 이상 이면 기본의 25% 보너스
        if( fAchieveRatio >= 1.5f )
        {
            vit->m_vecCharExp.begin()->m_nBaseExp += (int)(kInfo.m_nExp * 0.25f);
            vit->m_nBaseGP += (int)(kInfo.m_nGp * 0.25f);
        }


        vit->m_vecCharExp.begin()->m_nBaseExp = std::max<int>( vit->m_vecCharExp.begin()->m_nBaseExp, 1 );
        vit->m_nBaseGP = std::max<int>( vit->m_nBaseGP, 1 );

        // 대전에 한해서만 2배 경험치 계산.
        // CalcBaseExpGpSpxRatio( vit->m_vecCharExp.begin()->m_nBaseExp, vit->m_nBaseGP );
    }

    return;
}

void KResultManager::CalcMonsterCrusaderExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
     std::vector<KGameResultIn>::iterator vit;
     for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
     {
         float fMonHuntExp = 0.f;
         float fMonHuntGp = 0.f;
 
         // 보상을 초기화 한다.
         InitResultIn( *vit );
 
         if( vit->m_vecCharExp.empty() )
         {
             vit->m_vecCharExp.resize(1);
             InitResultIn( *vit );
             vit->m_bWin = false;
             continue;
         }
         vit->m_vecCharExp.resize( 1 ); // 태그 매치가 아니면 모두 1명이다.
 
         if( !vit->m_bWin ) continue;
         // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
         KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
         if( !spUser )
             continue;
 
         KCharacterInfo* pkPlayCharacterInfo = spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
         if( !pkPlayCharacterInfo )
             continue;

         // 몬스터 경험치 가져오기 몬스터 ID별 경험치와 레벨별 가중치를 계산해서 사용.
        float fMonExp = static_cast<float>( SiKGameDrop()->GetMonExp( kGameInfo_.m_nMonsterID, kGameInfo_.m_nMonsterCrusaderMonLv ) );
        float fMonGp = static_cast<float>( SiKGameDrop()->GetMonGp( kGameInfo_.m_nMonsterID, kGameInfo_.m_nMonsterCrusaderMonLv ) );

        if ( fMonExp <= 0.f ) {
            START_LOG( cwarn, L"해당 몬스터의 경험치가 0이하." )
                << BUILD_LOG( kGameInfo_.m_nMonsterID )
                << BUILD_LOG( kGameInfo_.m_nMonsterCrusaderMonLv ) << END_LOG;
            continue;
        }

        // 던전 레벨차 보정.
        SiKResultManager()->GetApplyLvDiffRatio( pkPlayCharacterInfo->m_dwLevel, kGameInfo_.m_nMonsterCrusaderMonLv, fMonExp ); // Exp
        SiKResultManager()->GetApplyLvDiffRatio( pkPlayCharacterInfo->m_dwLevel, kGameInfo_.m_nMonsterCrusaderMonLv, fMonGp ); // GP
         fMonExp = std::max<float>( 0.f, fMonExp ); // 클리핑.
         fMonGp = std::max<float>( 0.f, fMonGp ); // 클리핑.

         // 클리어 보너스Exp/GP
         int nBonusExp = 0;
         int nBonusGP = 0;
 /*
         // 지면 던전 클리어 보상을 받을 수 없다.
         if ( vit->m_bWin ) {
             nBonusExp = static_cast<int>( fMonExp * GetClearBonusRatio() );
             nBonusGP = static_cast<int>( fMonGp * GetClearBonusRatio() );
         }
*/
         // 클라이언트 (디스플레이)전달용
         vit->m_vecCharExp.begin()->m_nBaseExp = static_cast<int>( fMonExp );
         vit->m_nBaseGP = static_cast<int>( fMonGp );

         // 시즌4에서는 서버 배율 적용 안함.
//         CalcBaseExpGpSpxRatio( nBonusExp, nBonusGP );
         vit->m_vecCharExp.begin()->m_nAdditionnalExp = nBonusExp;
         vit->m_nAddtionalGp = nBonusGP;

         START_LOG( clog, L" Dungeon Clear..Name : " << spUser->GetName() )
             << BUILD_LOG( kGameInfo_.m_iGameMode )
             << BUILD_LOGc( vit->m_vecCharExp.begin()->m_cCharType )
             << BUILD_LOG( vit->m_vecCharExp.begin()->m_nBaseExp )
             << BUILD_LOG( vit->m_vecCharExp.begin()->m_nAdditionnalExp )
             << BUILD_LOG( vit->m_nBaseGP )
             << BUILD_LOG( vit->m_nAddtionalGp ) << END_LOG;
     }
}

void KResultManager::CalcEmptyExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    std::vector<KGameResultIn>::iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        // 보상을 초기화 한다.
        InitResultIn( *vit );

        if( vit->m_vecCharExp.empty() )
        {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }
    }

    CalcGuildPoint( kReq_, KGuildConfig::PVP );
}

float KResultManager::FindReconcileValue( std::vector<sReconcile>& vecReconcile_, float fValue_ )
{
    // 밖에서 락 걸어줘야 한다..
    std::vector<sReconcile>::iterator vit;
    for( vit = vecReconcile_.begin() ; vit != vecReconcile_.end() ; ++vit )
    {
        if( (float)vit->m_nBegin <= fValue_ && (float)vit->m_nEnd >= fValue_ )
            return vit->m_fReconcValue;
    }
    return 0.1f;
}

float KResultManager::GetRankBonus( IN const int nRankPoint_, IN const int nGameMode_ )
{
    KLocker lock( m_csRankPoint );

    // 랭크 보너스
    float fRankBonus = FindReconcileValue( m_vecRankPoint, (float)nRankPoint_ );

    // 퍼펙트 이벤트 보너스
    GetPerfectEventBonus( nGameMode_, fRankBonus );

    return fRankBonus;
}

bool KResultManager::GetStageCountInfo( IN int nQuestID_, IN int nDifficult_, OUT std::pair<int,int>& StageCountInfo_ ) // 080223 tslayer.
{
    KLocker lock( m_csQuestExpGp );

    std::map< int, sQuestInfo >::iterator mit;
    std::map< int, sStageInfo >::iterator mit2;

    mit = m_mapQuestExpGp.find( nQuestID_ );
    _JIF( mit != m_mapQuestExpGp.end(), return false );

    mit2 = mit->second.m_mapStageInfo.find( nDifficult_ );
    _JIF( mit2 != mit->second.m_mapStageInfo.end(), return false );

    StageCountInfo_.first = mit2->second.m_nMinStage;
    StageCountInfo_.second = mit2->second.m_nMaxStage;

    return true;
}

char KResultManager::GetCharGrade( DWORD dwWin_ )
{
    if( dwWin_ == 0 ) return (char)0;

    std::map< DWORD, DWORD >::iterator mit;
    for( mit = m_mapCharacterGrade.begin(); mit != m_mapCharacterGrade.end(); mit++ )
    {
        if( dwWin_ < mit->second ) return (char)(mit->first-1);
    }

    // 위의 반복자 루프에서 해당 값을 찾지 못했을 경우 
    mit = --m_mapCharacterGrade.end();
    return (char)(mit->first-1);
}

bool KResultManager::LoadPVPBonus( KLuaManager& kLuaMng_ )
{
    std::vector<sPVPReward> vecPvPEvent;
    vecPvPEvent.clear();

    if( kLuaMng_.BeginTable( "PVPEvent") == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng_.BeginTable( i ) == S_OK )
            {
                sPVPReward sInfo;
                _JIF( 0 == kLuaMng_.GetValue( 1, sInfo.m_nEventID ), return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, sInfo.m_nKillNum ), return false );                
                vecPvPEvent.push_back( sInfo );

                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"PvP Event 테이블읽기 종료 Index : " << i ) << END_LOG;
                break;
            }
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        //소팅...
        std::sort(vecPvPEvent.begin(), vecPvPEvent.end(),
            boost::bind(&sPVPReward::m_nEventID, _1) < boost::bind(&sPVPReward::m_nKillNum, _2) );
    }


    KLocker lock( m_csPvPEvent );
    m_vecPvPEvent.swap( vecPvPEvent );
    return true;
}

bool KResultManager::CheckPVPEvent( IN int nEventNumber_, IN int nNumKills_, IN bool bWin_ ) //  080603  woosh. 대전이벤트 하드코딩
{
    if( m_vecPvPEvent.empty() )
    {
        return true;
    }
    std::vector<sPVPReward>::iterator vit;
    for( vit = m_vecPvPEvent.begin() ; vit != m_vecPvPEvent.end() ; ++vit )
    {
        if( vit->m_nEventID == nEventNumber_ && vit->m_nKillNum <= nNumKills_ )
        {
            START_LOG( clog, L"대전 5킬하고 경험치 받아감, KillNum : " << nNumKills_ << L"이벤트 : " << nEventNumber_  ) << END_LOG;
            return true;
        }

        if( vit->m_nEventID == nEventNumber_ && bWin_ )
        {
            START_LOG( clog, L"대전에 5킬한 같은 편 덕분에 경험치 받아감, KillNum : " << nNumKills_ << L"이벤트 : " << nEventNumber_  ) << END_LOG;
            return true;
        }

        if( vit->m_nEventID != nEventNumber_ )
        {
            //START_LOG( clog, L"5킬 이벤트 아닌 다른 이벤트.."  ) << END_LOG;
            return true;
        }
    }

    return false;
}

bool KResultManager::MiniGameGPReward( IN OUT KMiniGameRankInfo& kPacket_ )
{
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID );

    _JIF( spUser, return false );

    // 보상GP
    int nResultGP = 0;
    for ( int i = 0; i <= kPacket_.m_nClearStage ; ++i ) {
        nResultGP += GetMiniGameGPRewardByStage( i );
    }

    spUser->IncreaseGP( KGpStatInfo::GIT_MINIGAME_CLEAR, false, nResultGP );
    kPacket_.m_nGP = spUser->GetGP();
    return true;
}

int KResultManager::GetMiniGameGPRewardByStage( int dwClearStage_ )
{
    //std::clog << "클리어 스테이지 : " << dwClearStage_;
    if( dwClearStage_ > 9 ) dwClearStage_ = 9; // 최대 스테이지보다 크면

    std::map< DWORD, int >::const_iterator mit;
    mit = m_mapMiniGameGPTable.find( dwClearStage_ );
    START_LOG( clog, L", 미니게임으로 얻은 GP는 : " << mit->second  ) << END_LOG;

    return mit->second;
}


void KResultManager::AdjustCharaterLimit( KEndGameReq& kReq )
{
    std::vector<KGameResultIn>::iterator vit;
    for( vit = kReq.m_vecGameResult.begin() ; vit != kReq.m_vecGameResult.end() ; ++vit )
    {
        std::vector< KCharacterExpInfo >::iterator vitExp;
        for( vitExp = vit->m_vecCharExp.begin() ; vitExp != vit->m_vecCharExp.end() ; ++vitExp )
        {
            vitExp->m_nBaseExp = (int)(vitExp->m_nBaseExp * GetCharaterLimitExp( vitExp->m_cCharType ));
        }
    }
}

float KResultManager::GetCharaterLimitExp( char cCharType )
{
    std::map<char, std::pair<float,float> >::iterator mit;
    KLocker lock( m_csCharLimit );
    if( m_mapCharLimit.empty() ) return 1.f;
    mit = m_mapCharLimit.find( cCharType );
    if( mit == m_mapCharLimit.end() ) return 0.f;
    return mit->second.first;
}

float KResultManager::GetCharaterLimitSpx( char cCharType )
{
    std::map<char, std::pair<float,float> >::iterator mit;
    KLocker lock( m_csCharLimit );
    if( m_mapCharLimit.empty() ) return 1.f;
    mit = m_mapCharLimit.find( cCharType );
    if( mit == m_mapCharLimit.end() ) return 0.f;
    return mit->second.second;
}

bool KResultManager::LoadBoostDetailInfo( KLuaManager& kLuaMng_, std::map< std::pair< char, char > ,sBoostDetailInfo >& mapBoostDetailInfo_ )
{
    int nCharType;
    int nPromotion;
    sBoostDetailInfo boostDetailInfo;


    _JIF( 0 == kLuaMng_.GetValue( "Character", nCharType ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "Promotion", nPromotion ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "EXP", boostDetailInfo.m_fBoostExpRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "GP", boostDetailInfo.m_fBoostGpRatio ), return false );
    _JIF( 0 == kLuaMng_.GetValue( "SPX", boostDetailInfo.m_fBoostSpxRatio ), return false );
    
    boostDetailInfo.m_cCharType = static_cast<char>(nCharType);
    boostDetailInfo.m_cPromotion = static_cast<char>(nPromotion);

    std::pair< char, char > prCharacterPromotion( nCharType, nPromotion );
    mapBoostDetailInfo_.insert( std::make_pair( prCharacterPromotion, boostDetailInfo ) );

    return true;

};


void KResultManager::DumpBoostInfo( std::wostream& stm ) const
{
    KLocker lock( m_csBoostInfo );
    std::map< GCITEMID, sBoostInfo >::const_iterator mit;
    for( mit = m_mapBoostInfo.begin() ; mit != m_mapBoostInfo.end() ; ++mit )
    {
        START_LOG( clog, L" ====== Boost EXP/GP Info ====== " ) << END_LOG;

        std::map< std::pair< char, char>, sBoostDetailInfo >::const_iterator mit1;
        for( mit1 = mit->second.m_mapBoostDetailInfo.begin() ; mit1 != mit->second.m_mapBoostDetailInfo.end() ; ++mit1 )
        {
            stm << L"  Item ID : "   << mit->first
                << L"  Character : " << (int)mit1->second.m_cCharType
                << L", Promotion : " << (int)mit1->second.m_cPromotion
                << L", Exp : " << mit1->second.m_fBoostExpRatio
                << L", GP : " << mit1->second.m_fBoostGpRatio
                << L", SPX : " << mit1->second.m_fBoostSpxRatio                
                << dbg::endl;
        }
    }
}
void KResultManager::CheckCountBoost( DWORD dwUserUID_, std::map< DWORD,DWORD >& mapGameCountEvent_, float& fExpBoostRatio_, float& fGpBoostRatio_ )
{
    std::map< DWORD, DWORD >::iterator mit;
    if( mapGameCountEvent_.empty() )
    {
        return;
    }
    mit = mapGameCountEvent_.find( dwUserUID_ );

    if( mit == mapGameCountEvent_.end() )
    {
        return;
    }

    DWORD dwEventNumber = mit->second;

    START_LOG( clog, L"게임 카운트 이벤트 적용 전, UID : " << dwUserUID_ )
        << BUILD_LOG( dwEventNumber )
        << BUILD_LOG( fGpBoostRatio_ )
        << BUILD_LOG( fExpBoostRatio_ ) << END_LOG;

    switch( dwEventNumber )
    {
    case 1:
        fGpBoostRatio_ += 0.2f;
        break;
    case 2:
        fExpBoostRatio_ += 0.2f;
        fGpBoostRatio_ += 0.2f;
        break;

    }

    START_LOG( clog, L"게임 카운트 이벤트 적용 후, UID : " << dwUserUID_ )
        << BUILD_LOG( dwEventNumber )
        << BUILD_LOG( fGpBoostRatio_ )
        << BUILD_LOG( fExpBoostRatio_ ) << END_LOG;    
}

void KResultManager::CalcBaseExpGpSpxRatio( IN OUT int& nExp_, IN OUT int& nGP_ )
{
    nExp_ = static_cast<int>( nExp_ * GetBaseExpRatio() );
    nGP_  = static_cast<int>( nGP_ * GetBaseGpRatio() );

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        START_LOG( clog,  L"Base Exp/Gp Ratio : " << GetBaseExpRatio()
            << L" / " << GetBaseGpRatio() << dbg::endl
            << L" after Exp/Gp : " << nExp_
            << L" / " << nGP_ ) << END_LOG;
    }
}

bool KResultManager::LoadHellRankBonus( KLuaManager& kLuaMng_ )
{
    std::vector<sReconcile> vecHellRankPoint;
    vecHellRankPoint.clear();

    if( kLuaMng_.BeginTable( "HellRankBonus") == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng_.BeginTable( i ) == S_OK )
            {
                sReconcile sInfo;
                _JIF( 0 == kLuaMng_.GetValue( 1, sInfo.m_nBegin ), return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, sInfo.m_nEnd ), return false );
                _JIF( 0 == kLuaMng_.GetValue( 3, sInfo.m_fReconcValue ), return false );
                vecHellRankPoint.push_back( sInfo );

                _JIF( kLuaMng_.EndTable() == S_OK, return false );
            }
            else
            {
                START_LOG( cerr, L"HellRankBonus 테이블읽기 종료 Index : " << i ) << END_LOG;
                break;
            }
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        std::sort(vecHellRankPoint.begin(), vecHellRankPoint.end(),
            boost::bind(&sReconcile::m_nBegin, _1) < boost::bind(&sReconcile::m_nBegin, _2) );
    }

    KLocker lock( m_csRankPoint );
    m_vecHellRankPoint.swap( vecHellRankPoint );
    return true;
}

float KResultManager::GetHellRankBonus( IN int nRankPoint_ )
{
    KLocker lock( m_csRankPoint );
    return FindReconcileValue( m_vecHellRankPoint, (float)nRankPoint_ );
}

void KResultManager::SetPerfectEventGameMode( IN const std::set<int>& setGameMode_ )
{
    KLocker lock( m_csRankPoint );
    m_setPerfectEventGameMode = setGameMode_;
}

void KResultManager::GetPerfectEventBonus( IN const int nGameMode_, OUT float& fBonusRatio_ )
{
    // 밖에서 락 걸어줘야 한다..
    if( m_setPerfectEventGameMode.empty() )
    {
        START_LOG( clog, L"PerfectEvent Not Active." ) << END_LOG;
        return;
    }

    // 이벤트 대상 게임모드인가? (게임모드 설정되어 있으면 체크)
    std::set<int>::iterator sit = std::find( m_setPerfectEventGameMode.begin(), m_setPerfectEventGameMode.end(), nGameMode_ );
    if( sit == m_setPerfectEventGameMode.end() )
    {
        START_LOG( clog, L"Not PerfectEvent GameMode. ModeID : " << nGameMode_ ) << END_LOG;
        return;
    }

    // 던전클리어 랭크보너스 정보가 없음
    if( m_vecRankPoint.empty() )
    {
        START_LOG( cwarn, L"RankPoint Info Empty." ) << END_LOG;
        return;
    }

    // 퍼펙트 클리어가 아니다(스크립트 로드시 정렬되어 있음)
    if( m_vecRankPoint.rbegin()->m_fReconcValue != fBonusRatio_ )
    {
        START_LOG( clog, L"Not Perfect Clear. BonusRatio : " << fBonusRatio_ ) << END_LOG;
        return;
    }

    // 퍼펙트 보너스 적용
    fBonusRatio_ = m_fPerfectBonus;
}

bool KResultManager::ChkWinWinSurv( IN const KGameInfo& kGameInfo_, IN KEndGameReq& kReq_, OUT std::vector< boost::tuple<std::wstring,bool,int> >& vecLog_ )
{
    DWORD dwWinCount = 0;
    std::vector<KGameResultIn>::iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        if( vit->m_bWin )
            ++dwWinCount;

        vecLog_.push_back( boost::make_tuple(vit->m_strLogin, vit->m_bWin, -1 ) );
    }

    if( dwWinCount <= (DWORD)(kGameInfo_.m_nStartUserNum / 2) ||
        dwWinCount <= kReq_.m_vecGameResult.size() / 2 ) return false;
    return true;
}

bool KResultManager::ChkWinWinTeam( IN const KGameInfo& kGameInfo_, IN KEndGameReq& kReq_, OUT std::vector< boost::tuple<std::wstring,bool,int> >& vecLog_ )
{
    std::map<bool,std::set<int> > mapWinLose;
    std::vector<KGameResultIn>::iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        boost::tuple<std::wstring,bool,int> tpLog;
        std::vector<std::pair<DWORD,int> >::const_iterator vitMe;
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser ) continue;

        vitMe = std::find_if( kGameInfo_.m_vecUIDTeam.begin(), kGameInfo_.m_vecUIDTeam.end(), boost::bind( &std::pair<DWORD,int>::first, _1) == spUser->GetUID() );
        if( vitMe == kGameInfo_.m_vecUIDTeam.end() ) continue;

        if( vit->m_bWin )
            mapWinLose[true].insert( vitMe->second );
        else
            mapWinLose[false].insert( vitMe->second );

        vecLog_.push_back( boost::make_tuple(vit->m_strLogin, vit->m_bWin, vitMe->second ) );
    }

    if( mapWinLose[true].size() <= 1 ) return false;
    return true;
}

void KResultManager::ChkWinWinBug( IN const KGameInfo& kGameInfo_, IN KEndGameReq& kReq_ )
{
    if( kReq_.m_vecGameResult.empty() ) return;

    std::vector< boost::tuple<std::wstring,bool,int> > vecLog;
    
    switch( kGameInfo_.m_iGameMode )
    {
    case GC_GM_TEAM_MODE:
    case GC_GM_DEATH_TEAM:
    case GC_GM_GUILD_BATTLE:
    case GC_GM_INDIGO_TEAM:
    case GC_GM_TAG_TEAM:
    case GC_GM_CAPTAIN:
    case GC_GM_ANGELS_EGG:
    case GC_GM_AUTOMATCH_TEAM:
    case GC_GM_FATAL_DEATH_TEAM:
        {
            if( !ChkWinWinTeam( kGameInfo_, kReq_, vecLog ) ) return;
        }
        break;
    case GC_GM_SURVIVAL_MODE:
    case GC_GM_TAG_SURVIVAL:
    case GC_GM_DEATH_SURVIVAL:
    case GC_GM_FATAL_DEATH_SURVIVAL:
        {
            if( !ChkWinWinSurv( kGameInfo_, kReq_, vecLog ) ) return;
        }
        break;
    case GC_GM_DOTA:
        break;
    default:
        return;
    }

    std::wstringstream stm;
    stm << L"Game Mode : " << kGameInfo_.m_iGameMode << L" User List : ";
    std::vector< boost::tuple<std::wstring,bool,int> >::iterator vit;
    for( vit = vecLog.begin() ; vit != vecLog.end() ; ++vit )
    {
        stm << vit->get<0>() << L"[" << (vit->get<1>()?L"win":L"lose") << L"," << vit->get<2>() << L"], ";
        // 처벌 하겠다고 결정 나면... 여기다.. 추가 ..
    }
    
    TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_LT_WIM_WIN , std::wstring(L""), 0, 0, stm.str() );

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

void KResultManager::CheckEquipAttributeBoost( IN KItemContainer& kInven_, IN const std::vector<KEquipItemInfo>& vecEquip_, IN OUT float& fExpBoost_, IN OUT float& fGPBoost_ )
{
    std::vector<KEquipItemInfo>::const_iterator vit;
    for ( vit = vecEquip_.begin() ; vit != vecEquip_.end() ; ++vit ) {
        KItemPtr pkItem = kInven_.FindItemByItemUID( vit->m_dwUID );
        if ( !pkItem ) continue;

        std::vector< KAttributeInfo >::const_iterator vitAtt;
        for ( vitAtt = pkItem->m_vecAttribute.begin() ; vitAtt != pkItem->m_vecAttribute.end() ; ++vitAtt ) {

            switch ( vitAtt->m_cType ) {
                case ATTRTYPE_GP_ADDITIONAL_OBTAIN_RATE : fGPBoost_ += std::max<float>(0.f, (vitAtt->m_fValue * 0.01f )); break;
                case ATTRTYPE_EXP_ADDITIONAL_OBTAIN_RATE : fExpBoost_ += std::max<float>(0.f, (vitAtt->m_fValue * 0.01f )); break;
            }
        }
    }
}

void KResultManager::CheckDungeonClearBoost( IN const KGameInfo& kGameInfo_, IN OUT float& fExpBoost_, IN OUT float& fGpBoost_ )
{
    if( !KRoom::IsDungeonMode( kGameInfo_.m_iGameMode ) ) {
        return;
    }
    
    float fTmpExp;
    float fTmpGp;
    GetClearBonus( fTmpExp, fTmpGp );
    fExpBoost_ += std::max<float>(0.f, fTmpExp);
    fGpBoost_ += std::max<float>(0.f, fTmpGp);
}

bool KResultManager::LoadClearBonus( KLuaManager& kLuaMng_ )
{
    float fClearExp = 0.f;
    float fClearGp = 0.f;
    float fClearSpx = 0.f;

    _JIF( kLuaMng_.GetValue( "DungeonClearExpBonus", fClearExp ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DungeonClearGpBonus", fClearGp ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DungeonClearSpxBonus", fClearSpx ) == S_OK, return false );

    KLocker lock( m_csClearBonus );
    m_fClearExp = fClearExp;
    m_fClearGp = fClearGp;
    m_fClearSpx = fClearSpx;
    return true;
}

void KResultManager::GetClearBonus( OUT float& fClearExp_, OUT float& fClearGp_ )
{
    KLocker lock( m_csClearBonus );
    fClearExp_ = m_fClearExp;
    fClearGp_ = m_fClearGp;
}

// 이걸 한꺼번에 읽어들이는 게 좋을듯 하다.보상 Table을 
// 분리해서 관리하는것 보단 이게 좋을듯..,
// Exp/Lv Table, Grade Table, Gp 보상 Table, Exp 보상 Table, 
// Spx 보상 Table, 대만 홍콩에서 사용되는 캐릭터 그레이드,
// 미니게임에서 얻는 GP 보상 Table
bool KResultManager::LoadExpSpxInfo( OUT std::string& strScript_ )
{
    std::map< int, sQuestInfo > mapQuestExpGp;
    KLuaManager kLuaMng;
    std::map< DWORD, __int64> mapExpTable; // Exp/Lv 임시 테이블
    std::map< DWORD, DWORD > mapGradeTable; // Grade 임시 테이블
    std::map< DWORD, DWORD > mapCharacterGrade; // 대만 홍콩에서 사용되는 캐릭터 그레이드.
    std::map< DWORD, int > mapMiniGameGPTable; // 미니게임에서 얻는 GP 보상 임시 테이블
    DWORD dwMaxLevel;
    float fMaxAddRatio;
    int nMaxAddExp;

    strScript_ = "Cha_ExpSpxInfo.lua";
    _JIF( kLuaMng.DoFile( "Cha_ExpSpxInfo.lua" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MAX_LEVEL", dwMaxLevel ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MAX_ADD_RATIO", fMaxAddRatio ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MAX_ADD_EXP", nMaxAddExp ) == S_OK, return false );

// -------------------- Exp Table --------------------
    _JIF( kLuaMng.BeginTable( "ExpTable" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwChaLevel = 0;
        __int64 biChaExp = 0; // __int64일때 문제 발생..,
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( 1, dwChaLevel ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, biChaExp ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );
        
        if( mapExpTable.insert( std::make_pair( dwChaLevel, biChaExp ) ).second == false )
        {
            START_LOG( cerr, L"ExpTable 중복된 레벨값. Level : " << dwChaLevel << L",ChaExp : " << biChaExp ) << END_LOG;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // ExpTable End

    START_LOG( cerr, L"ExpTable Load 완료. size : " << mapExpTable.size() ) << END_LOG;


//-------------------- Grade Table --------------------
    _JIF( kLuaMng.BeginTable( "GradeTable" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwKey = 0;
        DWORD dwGrade = 0;
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( 1, dwKey ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, dwGrade ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        if( mapGradeTable.insert( std::make_pair( dwKey, dwGrade ) ).second == false )
        {
            START_LOG( cerr, L"GradeTable 중복된 등급. Key : " << dwKey << L",Grade : " << dwGrade ) << END_LOG;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // GradeTable End

    START_LOG( cerr, L"GradeTable Load 완료. size : " << mapGradeTable.size() ) << END_LOG;


//-------------------- ChaGrade Table --------------------
    _JIF( kLuaMng.BeginTable( "ChaGradeTable" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwGrade = 0;
        DWORD dwGradeValue = 0;
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( 1, dwGrade ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, dwGradeValue ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        if( mapCharacterGrade.insert( std::make_pair( dwGrade, dwGradeValue ) ).second == false )
        {
            START_LOG( cerr, L"ChaGradeTable 중복된 등급 : " << dwGrade << L",Value : " << dwGradeValue ) << END_LOG;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // ChaGradeTable End

    START_LOG( cerr, L"ChaGrade Table Load 완료. size : " << mapCharacterGrade.size() ) << END_LOG;


//-------------------- MiniGameGP Table --------------------
    _JIF( kLuaMng.BeginTable( "MiniGPTable" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        DWORD dwKey = 0;
        int nValue = 0;
        if( kLuaMng.BeginTable( i ) != S_OK ) break;
        _JIF( kLuaMng.GetValue( 1, dwKey ) == S_OK, return false );
        _JIF( kLuaMng.GetValue( 2, nValue ) == S_OK, return false );
        _JIF( kLuaMng.EndTable() == S_OK, return false );

        if( mapMiniGameGPTable.insert( std::make_pair( dwKey, nValue ) ).second == false )
        {
            START_LOG( cerr, L"MiniGPTable 중복된 Key : " << dwKey << L",Value : " << nValue ) << END_LOG;
        }
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false ); // ChaGradeTable End

    START_LOG( cerr, L"MiniGP Table Load 완료. size : " << mapMiniGameGPTable.size() ) << END_LOG;

    {
        KLocker lock( m_csExpSpxInfo );

        m_dwMaxLevel = dwMaxLevel;
        m_fMaxAddRatio = fMaxAddRatio;
        m_nMaxAddExp = nMaxAddExp;
        m_mapExpTable.swap( mapExpTable );
        m_mapGradeTable.swap( mapGradeTable );
        m_mapCharacterGrade.swap( mapCharacterGrade );
        m_mapMiniGameGPTable.swap( mapMiniGameGPTable );
    }

    return true;
}

int KResultManager::GetRankGrade( IN const int nRankPoint_, IN const bool bChampionMode_ )
{
    int nRankGrade = 0;
    std::vector<sReconcile>::iterator vit;
    std::vector<sReconcile>& vecRank = (bChampionMode_? m_vecHellRankPoint : m_vecRankPoint);
    for ( vit = vecRank.begin() ; vit != vecRank.end() ; ++vit ) {
        ++nRankGrade;
        if ( nRankGrade >= vit->m_nBegin && nRankGrade  <= vit->m_nEnd )
            break;
    }
    return nRankGrade;
}

void KResultManager::CalcQuestHeroExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    std::vector<KGameResultIn>::iterator vit;
    for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
        // 보상을 초기화 한다.
        InitResultIn( *vit );

        if ( vit->m_vecCharExp.empty() ) {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }

        vit->m_vecCharExp.resize( 1 ); // 태그 매치가 아니면 모두 1명이다.

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( !spUser ) continue;

        // vit->m_vecCharExp의 사이즈가 0이 아니기 때문에 사용한다. (캐릭터얻는데만 사용)
        const KCharacterInfo* pkPlayCharacterInfo = spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
        if ( !pkPlayCharacterInfo ) continue;

        float fClearRatio = 0.f;
        float fDungeonClearExp = 0.f;
        float fDungeonClearGP = 0.f;
        int nBonusExp = 0;
        int nBonusGP = 0;

        // 영웅던전 Clear 성공시와 실패시의 보상 비율이 틀리다.
        if( false == vit->m_bWin ) {
            fClearRatio = SiKHeroDungeonManager()->GetDungeonLoseClearExpRatio();
        }
        else {
            fClearRatio = SiKHeroDungeonManager()->GetDungeonWinClearExpRatio();
        }

        // 던젼 Clear Exp(승) = 캐릭터 레벨기준경험치 * 40 * ( 던전 최소 적정 레벨 / 40 )
        // 던젼 Clear Exp(패) = 캐릭터 레벨기준경험치 * 10 * ( 던전 최소 적정 레벨 / 40 )
        fDungeonClearExp = static_cast<float>( SiKGameDrop()->GetCharLvMonExp( pkPlayCharacterInfo->m_dwLevel ) * fClearRatio * ( SiKHeroDungeonManager()->GetHeroDungeonMinLv( kGameInfo_.m_iGameMode ) / SiKHeroDungeonManager()->GetMonsterExpRatio() ) );
        fDungeonClearExp = std::max<float>(0.f, fDungeonClearExp );

        // 던젼 Clear Exp(승) = 캐릭터 레벨기준GP * 40 * ( 던전 최소 적정 레벨 / 40 )
        // 던젼 Clear Exp(패) = 캐릭터 레벨기준GP * 10 * ( 던전 최소 적정 레벨 / 40 )
        fDungeonClearGP = static_cast<float>( SiKGameDrop()->GetCharLvMonGP( pkPlayCharacterInfo->m_dwLevel ) * fClearRatio * ( SiKHeroDungeonManager()->GetHeroDungeonMinLv( kGameInfo_.m_iGameMode ) / SiKHeroDungeonManager()->GetMonsterExpRatio() ) );
        fDungeonClearGP = std::max<float>(0.f, fDungeonClearGP );

        nBonusExp = static_cast<int>( fDungeonClearExp );
        nBonusGP = static_cast<int>( fDungeonClearGP );

        // 클라이언트 (디스플레이)전달용
        vit->m_vecCharExp.begin()->m_nBaseExp = static_cast<int>( mapResultExp_[std::make_pair(vit->m_dwUID,pkPlayCharacterInfo->m_cCharType)] );
        vit->m_nBaseGP = mapResultGP_[vit->m_dwUID];

        // 대전 클리어에 한해서만 2배 배율 적용.
        // CalcBaseExpGpSpxRatio( nBonusExp, nBonusGP );
        vit->m_vecCharExp.begin()->m_nAdditionnalExp = nBonusExp;
        vit->m_nAddtionalGp = nBonusGP;

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
            START_LOG( clog, L"Login : " << vit->m_strLogin
                << L", CharType : " << (int)vit->m_vecCharExp.begin()->m_cCharType
                << L", Dungeon Clear Exp : " << fDungeonClearExp
                << L", Dungeon Clear GP : " << fDungeonClearGP
                << L", BonusExp : " << nBonusExp
                << L", BonusGP : " << nBonusGP ) << END_LOG;
        }

        START_LOG( clog, L" Dungeon Clear..Name : " << spUser->GetName() )
            << BUILD_LOGc( vit->m_vecCharExp.begin()->m_cCharType )
            << BUILD_LOG( vit->m_vecCharExp.begin()->m_nBaseExp )
            << BUILD_LOG( vit->m_vecCharExp.begin()->m_nAdditionnalExp )
            << BUILD_LOG( vit->m_nBaseGP )
            << BUILD_LOG( vit->m_nAddtionalGp )
            << BUILD_LOG( SiKHeroDungeonManager()->GetMonsterExpRatio() ) << END_LOG; // Max 카운트
    }

    CalcGuildPoint( kReq_, KGuildConfig::DUNGEON );
}

bool KResultManager::LoadMonModifiedRatio( KLuaManager& kLuaMng_ )
{
    int                    nModifiedMinLevel;
    int                    nModifiedMaxLevel;
    float                  fDefaultReward;
    std::map< int, float > mapMonModifiedRatio;

    _JIF( kLuaMng_.BeginTable( "MonModifiedInfo") == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "ModifiedMinLevel", nModifiedMinLevel ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "ModifiedMaxLevel", nModifiedMaxLevel ) == S_OK, return false );
    _JIF( kLuaMng_.GetValue( "DefaultReward", fDefaultReward ) == S_OK, return false );

    _JIF( kLuaMng_.BeginTable( "MonModifiedRatio" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int nLvDiff = 0;
        float fRatio = 0.f;
        _JIF( 0 == kLuaMng_.GetValue( 1, nLvDiff ), return false );
        _JIF( 0 == kLuaMng_.GetValue( 2, fRatio ), return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        mapMonModifiedRatio.insert( std::make_pair(nLvDiff, fRatio ) );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    START_LOG( cerr, L"캐릭터 보정 테이블 로드 완료 : " << mapMonModifiedRatio.size() ) << END_LOG;

    KLocker lock( m_csModifiedRatio );
    m_nModifiedMinLevel = nModifiedMinLevel;
    m_nModifiedMaxLevel = nModifiedMaxLevel;
    m_fDefaultReward = fDefaultReward;
    m_mapMonModifiedRatio.swap( mapMonModifiedRatio );
    return true;
}

void KResultManager::GetApplyLvDiffRatio( IN const DWORD dwCharLevel_, IN const int nMonLevel_, OUT float& fMonReward_ )
{
    KLocker lock( m_csModifiedRatio );

    int nLvDiff = static_cast<int>( nMonLevel_ - dwCharLevel_ );

    // 몬스터Lv - 캐릭터Lv 가 보정레벨 범위를 만족하는지 체크
    if( nLvDiff < m_nModifiedMinLevel || nLvDiff > m_nModifiedMaxLevel ) {
        fMonReward_ = m_fDefaultReward; // 보정이 안될경우 기본 지급 경험치.
        return;
    }

    std::map< int, float >::iterator mit;
    mit = m_mapMonModifiedRatio.find( nLvDiff );
    if( mit == m_mapMonModifiedRatio.end() ) {
        fMonReward_ = m_fDefaultReward; // 레벨 차이값이 보정Table에 없을 경우 기본 경험치 지급.
    } else {
        fMonReward_ = fMonReward_ * mit->second; // 몬스터 획득 경험치 * 보정 Ratio
    }
}

void KResultManager::GetMatchApplyLvDiffRatio( IN const DWORD dwWinCharLevel_, IN const DWORD dwLoseCharLevel_, OUT float& fExp_ )
{
    KLocker lock( m_csModifiedRatio );

    // 이긴 유저의 캐릭터 Lv이 죽은 유저의 캐릭터 Lv보다 큰지 체크.
    if( dwWinCharLevel_ < dwLoseCharLevel_ ) {
        return;
    }
    // 보정공식 적용.1 - ( 0.01 * (이긴유저 캐릭터Lv - 죽은 유저 캐릭터Lv) )
    float fRatio = 1.f - ( 0.01f * static_cast<float>(dwWinCharLevel_ - dwLoseCharLevel_) );
    fExp_ = fExp_ * fRatio;
}

bool KResultManager::GetDifficulty( IN const int& nModeID_, OUT std::set<int>& setDifficulty_ )
{
    KLocker lock( m_csQuestExpGp );
    setDifficulty_.clear();

    std::map< int, sQuestInfo >::const_iterator cmitQuest = m_mapQuestExpGp.find( nModeID_ );
    _JIF( cmitQuest != m_mapQuestExpGp.end(), return false );

    std::map< int, sStageInfo >::const_iterator cmitStage;
    for ( cmitStage = cmitQuest->second.m_mapStageInfo.begin() ; cmitStage != cmitQuest->second.m_mapStageInfo.end() ; ++cmitStage ) {
        setDifficulty_.insert( cmitStage->first );
    }

    return true;
}

bool KResultManager::CalcPvpExp( IN const DWORD& dwKillCharLevel_, IN const DWORD& dwDeathCharLevel_, OUT float& fExp_ )
{
    //  실시간 대전 경험치 계산.
    fExp_ = static_cast<float>(SiKGameDrop()->GetCharLvMonExp( dwKillCharLevel_ )) * GetBaseExpRatio();
    // 캐릭터 레벨에 따른 보정.
    GetMatchApplyLvDiffRatio( dwKillCharLevel_, dwDeathCharLevel_, fExp_ );

    if( 0.f >= fExp_ ) {
        return false;
    }
    return true;
}

bool KResultManager::CalcPvpGP( IN const DWORD& dwDeathCharLevel_, IN const int nTeamUserNum_, OUT float& fGP_ )
{
    // 실시간 대전 GP 계산
    fGP_ = static_cast<float>( SiKGameDrop()->GetCharLvMonGP(dwDeathCharLevel_) ) * GetPvpGpRatio();
    float fTeamUserNum = std::max<float>( static_cast<float>(nTeamUserNum_), 1.f );
    fGP_ = (fGP_ > 0.f ? fGP_ / fTeamUserNum : 0.f );

    if( 0.f >= fGP_ ) {
        return false;
    }
    return true;
}

bool KResultManager::IsExistDifficulty( IN const int& nModeID_, IN OUT int& nDifficulty_ )
{
    KLocker lock( m_csQuestExpGp );

    std::map< int, sQuestInfo >::const_iterator cmitQuest = m_mapQuestExpGp.find( nModeID_ );
    if ( cmitQuest == m_mapQuestExpGp.end() ) return false;

    std::map< int, sStageInfo >::const_iterator cmitStage;
    cmitStage = cmitQuest->second.m_mapStageInfo.find( nDifficulty_ );
    if ( cmitStage == cmitQuest->second.m_mapStageInfo.end() ) {
        START_LOG( clog, L"해당 던전에 없는 난이도.. ModeID : " << nModeID_ << L", Difficulty : " << nDifficulty_ ) << END_LOG;
        nDifficulty_ = 0;
        return false;
    }

    return true;
}

void KResultManager::CheckEquipSetBoost( IN const std::vector<KEquipItemInfo>& vecEquip_, OUT float& fExpBoost_, OUT float& fGPBoost_ )
{
    std::set<GCITEMID> setSetID;
    setSetID.clear();

    std::vector<KEquipItemInfo>::const_iterator cvitEquip;
    for ( cvitEquip = vecEquip_.begin() ; cvitEquip != vecEquip_.end() ; ++cvitEquip ) {
        // 장착중인 아이템중 세트아이템이면 SetID 구하기.
        GCITEMID dwSetID = 0;
        if ( SiKItemManager()->m_kAttributeDecider.GetSetID( cvitEquip->m_dwID, dwSetID ) ) {
            setSetID.insert( dwSetID );
        }
    }

    std::set<GCITEMID>::iterator sit;
    for ( sit = setSetID.begin() ; sit != setSetID.end() ; ++sit ) {
        // 세트별 파트ItemID 리스트
        std::vector<GCITEMID> vecItemID;
        if ( false == SiKItemManager()->m_kAttributeDecider.GetSetItemList( *sit, vecItemID ) ) {
            continue;
        }

        if ( vecItemID.empty() ) {
            continue;
        }

        int nEquipCount = GetEquipSetCount( vecEquip_, vecItemID );

        float fExpBonus = 0.f;
        SiKItemManager()->m_kAttributeDecider.GetSetExpBonus( *sit, nEquipCount, fExpBonus );

        float fGpBonus = 0.f;
        SiKItemManager()->m_kAttributeDecider.GetSetGpBonus( *sit, nEquipCount, fGpBonus );

        fExpBoost_ += std::max<float>( fExpBonus, 0.f );
        fGPBoost_ += std::max<float>( fGpBonus, 0.f );

        START_LOG( clog, L"SetItem bonus.. SetID : " << *sit << L", Exp : " << fExpBonus << L", Gp : " << fGpBonus ) << END_LOG;
    }
}

void KResultManager::CheckCoordiSetBoost( IN const char& cCharType_, IN std::map<char,KLookEquipChar>& mapCoordi_, IN KItemContainer& kInven_, OUT float& fExpBoost_, OUT float& fGPBoost_ )
{
    std::map<char,KLookEquipChar>::iterator mitEquip;
    mitEquip = mapCoordi_.find( cCharType_ );
    if ( mitEquip == mapCoordi_.end() ) {
        START_LOG( clog, L"착용중인 코디가 없음.. CharType : " << static_cast<int>(cCharType_) )  << END_LOG;
        return;
    }

    KCoordiInfo kInfo;
    if ( false == mitEquip->second.GetCoordiInfo( KLookEquipChar::LC_COORDI_0, kInfo ) ) {
        START_LOG( clog, L"코디 정보가 없음.. CharType : " << static_cast<int>(cCharType_) ) << END_LOG;
        return;
    }

    std::vector<KEquipItemInfo> vecEquipItem;
    vecEquipItem.clear();

    std::set<GCITEMUID>::iterator sitItemUID;
    for ( sitItemUID = kInfo.m_setEquip.begin() ; sitItemUID != kInfo.m_setEquip.end() ; ++sitItemUID ) {
        const GCITEMUID& ItemUID = *sitItemUID;

        KItemPtr pkItem = kInven_.FindItemByItemUID( ItemUID );
        if ( pkItem == NULL ) {
            START_LOG( cwarn, L"인벤토리에 코디아이템을 찾을수 없음. ItemUID : " << ItemUID ) << END_LOG;
            continue;
        }

        // ToDo. 코디합성이 들어갈 경우, 능력치가 되는 아이템ItemID로 넣어주도록 수정할 것.
        KEquipItemInfo kItem;
        kItem.m_dwID = pkItem->m_ItemID;
        vecEquipItem.push_back( kItem );
    }

    std::set<GCITEMID> setSetID;
    setSetID.clear();

    std::vector<KEquipItemInfo>::iterator vit;
    for ( vit = vecEquipItem.begin() ; vit != vecEquipItem.end() ; ++vit ) {
        // 장착중인 아이템중 세트아이템이면 SetID 구하기.
        GCITEMID dwSetID = 0;
        if ( SiKItemManager()->m_kAttributeDecider.GetSetID( vit->m_dwID, dwSetID ) ) {
            setSetID.insert( dwSetID );
        }
    }

    std::set<GCITEMID>::iterator sit;
    for ( sit = setSetID.begin() ; sit != setSetID.end() ; ++sit ) {
        // 세트별 파트ItemID 리스트
        std::vector<GCITEMID> vecItemID;
        if ( false == SiKItemManager()->m_kAttributeDecider.GetSetItemList( *sit, vecItemID ) ) {
            continue;
        }

        if ( vecItemID.empty() ) {
            continue;
        }

        int nEquipCount = GetEquipSetCount( vecEquipItem, vecItemID );

        float fExpBonus = 0.f;
        SiKItemManager()->m_kAttributeDecider.GetSetExpBonus( *sit, nEquipCount, fExpBonus );

        float fGpBonus = 0.f;
        SiKItemManager()->m_kAttributeDecider.GetSetGpBonus( *sit, nEquipCount, fGpBonus );

        fExpBoost_ += std::max<float>( fExpBonus, 0.f );
        fGPBoost_ += std::max<float>( fGpBonus, 0.f );

        START_LOG( clog, L"Coordi SetItem bonus.. SetID : " << *sit << L", Exp : " << fExpBonus << L", Gp : " << fGpBonus )  << END_LOG;
    }

}

int KResultManager::GetEquipSetCount( IN const std::vector<KEquipItemInfo>& vecEquip_, IN const std::vector<GCITEMID>& vecItemID_ )
{
    int nCount = 0;

    std::vector<GCITEMID>::const_iterator cvitPart;
    for ( cvitPart = vecItemID_.begin() ; cvitPart != vecItemID_.end() ; ++cvitPart ) {

        std::vector<KEquipItemInfo>::const_iterator cvitEquip;
        cvitEquip = std::find_if( vecEquip_.begin(), vecEquip_.end(),
            boost::bind( &KEquipItemInfo::m_dwID, _1 ) == *cvitPart );

        if ( cvitEquip != vecEquip_.end() ) {
            ++nCount;
        }
    }

    return nCount;
}

void KResultManager::CheckMonsterCrusaderClearBoost( IN const KGameInfo& kGameInfo_, IN OUT float& fExpBoost_, IN OUT float& fGpBoost_ )
{
    if( GC_GM_MONSTER_CRUSADER != kGameInfo_.m_iGameMode ) {
        return;
    }

    float fTmpExp;
    float fTmpGp;
    GetClearBonus( fTmpExp, fTmpGp );
    fExpBoost_ += std::max<float>(0.f, fTmpExp);
    fGpBoost_ += std::max<float>(0.f, fTmpGp);
}

// 아래 함수에서 길드전으로 EXP,GP,SP 를 적용하고 (기본 대전과 동일한 값) , 길드배틀포인트를 길드에 저장한다.
// file:UserHelper3.cpp#UpdateGuildPoint 에서 유저에게 전달할 값을 넣는다.
void KResultManager::CalcGuildBattleExpGp( IN const KGameInfo& kGameInfo_, 
                                          IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, 
                                          IN std::map<DWORD,int>& mapResultGP_, 
                                          IN OUT KEndGameReq& kReq_ )
{
    CalcDefaultExpGP( kGameInfo_, mapResultExp_, mapResultGP_, kReq_ );
    
    if ( kGameInfo_.m_prGuildUID.first == 0 || 
        kGameInfo_.m_prGuildUID.second == 0 ) {
        START_LOG( cwarn, L"길드가 2개가 안된다." ) << END_LOG;
    }

    std::vector<KGameResultIn>::iterator vitBegin;

    if ( !kReq_.m_vecGameResult.empty() ) {
        vitBegin = kReq_.m_vecGameResult.begin();
    }
    else {
        return;
    }

    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vitBegin->m_dwUID );

    if( !spUser ) {
        return;
    }

    if ( spUser->m_pkRoom == NULL ) {
        return;
    }

    // 게임중에 하나의 길드가 나갔을 경우에  나간 길드 ID를 저장했다가 결과처리에 사용
    std::pair<DWORD,DWORD> pairGuildUID = kGameInfo_.m_prGuildUID;
    if ( kGameInfo_.m_prGuildUID.first == 0 ) {
        pairGuildUID.first = spUser->m_pkRoom->GetLeaveGuildWhilePlaying();
    }
    else if ( kGameInfo_.m_prGuildUID.second == 0 ) {
        pairGuildUID.second = spUser->m_pkRoom->GetLeaveGuildWhilePlaying();
    }
    spUser->m_pkRoom->SetLeaveGuildWhilePlaying( 0 ); // 나간 길드 ID 초기화


    DWORD dwBattlePointA = SiKGuildManager()->GetGuildBattlePoint( pairGuildUID.first );
    DWORD dwBattlePointB = SiKGuildManager()->GetGuildBattlePoint( pairGuildUID.second );
    
    float fWinningRateA = CalcGuildBattleWinningRate( static_cast<float>(dwBattlePointB), static_cast<float>(dwBattlePointA) );
    float fWinningRateB = CalcGuildBattleWinningRate( static_cast<float>(dwBattlePointA), static_cast<float>(dwBattlePointB) );

    float KA = static_cast<float>( SiKGuildManager()->DivideChannelGrade( dwBattlePointA ) );
    float KB = static_cast<float>( SiKGuildManager()->DivideChannelGrade( dwBattlePointB ) );



    bool bWinGuildA = false;
    bool bWinGuildB = false;

    if ( spUser->GetGuildUID() == pairGuildUID.first ) {
        bWinGuildA = vitBegin->m_bWin;
        bWinGuildB = !bWinGuildA;
    }
    else if ( spUser->GetGuildUID() == pairGuildUID.second ) {
        bWinGuildB = vitBegin->m_bWin;
        bWinGuildA = !bWinGuildB;
    }


    int addPointA = MakeIntegerBPoint( dwBattlePointA, KA, bWinGuildA, fWinningRateA );
    int addPointB = MakeIntegerBPoint( dwBattlePointB, KB, bWinGuildB, fWinningRateB );

    // 대전/던전 결과 로직은 결과 계산과 적용이 따로 되어있다. 

    // short 게임인지 확인할 수 있는 방법이 필요.
    if ( spUser->m_pkRoom->IsShortGame() ) {
        START_LOG( cerr, L"Short game 이라서 길드배틀포인트 적용 취소" << vitBegin->m_dwUID ) << END_LOG;
        return;
    }

    kReq_.m_vecGuildBPoint.push_back( std::make_pair( pairGuildUID.first, addPointA ) );
    kReq_.m_vecGuildBPoint.push_back( std::make_pair( pairGuildUID.second, addPointB ) );
     
    SiKGuildManager()->UpdateBattlePoint( pairGuildUID.first, addPointA );
    SiKGuildManager()->UpdateBattlePoint( pairGuildUID.second, addPointB );

    // file:UserHelper3.cpp#UpdateGuildPoint
     
    CalcGuildPoint( kReq_, KGuildConfig::GUILD_BATTLE );

    START_LOG( clog, L"길드 배틀 포인트 계산 결과 , Guild A : " << pairGuildUID.first << L" Guild B: " << pairGuildUID.second  )
        << BUILD_LOG( addPointA )
        << BUILD_LOG( addPointB ) << END_LOG;

}

int KResultManager::MakeIntegerBPoint( IN const DWORD BPoint_, IN const float K_, IN const bool bWin_, IN const float fWinningRate_ )
{
    float fAddPoint = CalcToAddRankingPoint( static_cast<float>(BPoint_), K_, bWin_, fWinningRate_ );
    int addPoint = 0;
    
    if ( fAddPoint > 0.0f ) {
        addPoint = static_cast<int>( ceil( fAddPoint ) );
    }
    else { // if value =< 0
        addPoint = static_cast<int>( floor( fAddPoint ) );
    }
    
    return addPoint;
}

float KResultManager::CalcGuildBattleWinningRate( const float RankPointA_, const float RankPointB_ )
{
    float fNumerator	=	1.0f;
    float fDenominator	=	1.0f + powf( 10.0f, (RankPointA_-RankPointB_)/400.0f );

    float fWinningRate	=	fNumerator / fDenominator;
    return fWinningRate;
}

float KResultManager::CalcToAddRankingPoint( const float rankPointOfA_, const float K_, bool bWin_, float Ea_ )
{
    float fWin = (bWin_) ? 1.0f : 0.0f;
    return K_*( fWin - Ea_ );
}

void KResultManager::CalcDotaExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    std::vector<KGameResultIn>::iterator vit;
    for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
        // 보상을 초기화 한다.
        InitResultIn( *vit );

        if ( vit->m_vecCharExp.empty() ) {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }

        vit->m_vecCharExp.resize( 1 ); // 태그 매치가 아니면 모두 1명이다.

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( !spUser ) continue;

        // vit->m_vecCharExp의 사이즈가 0이 아니기 때문에 사용한다. (캐릭터얻는데만 사용)
        const KCharacterInfo* pkPlayCharacterInfo = spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
        if ( !pkPlayCharacterInfo ) continue;

        int nExp = 0;
        int nGP = 0;
        int nBonusExp = 0;
        int nBonusGP = 0;

        // 일단 기본적으로 레벨에 대한 혜택 정보만 가져온다.
        SiKDotaManager()->GetDotaExpReward( pkPlayCharacterInfo->m_dwLevel, nExp );
        if ( vit->m_bWin ) {
            nBonusExp = static_cast<int>( nExp * SiKDotaManager()->GetWinRatio() );
        }
        else {
            nBonusExp = static_cast<int>( nExp * SiKDotaManager()->GetLoseRatio() );
        }

        vit->m_vecCharExp.begin()->m_nAdditionnalExp = nBonusExp;
        vit->m_nAddtionalGp = nBonusGP;

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
            START_LOG( cwarn, L"--- Dump CalcDotaExpGP ---" << dbg::endl
                << L" Login : " << spUser->GetName() << dbg::endl
                << L" vit->m_vecCharExp.begin()->m_nBaseExp : " << vit->m_vecCharExp.begin()->m_nBaseExp << dbg::endl
                << L" vit->m_nBaseGP : " << vit->m_nBaseGP << dbg::endl
                << L" vit->m_vecCharExp.begin()->m_nAdditionnalExp : " << vit->m_vecCharExp.begin()->m_nAdditionnalExp << dbg::endl
                << L" vit->m_nAddtionalGp : " << vit->m_nAddtionalGp << dbg::endl
                << L" nBonusExp : " << nBonusExp << dbg::endl
                << L" nBonusGP : " << nBonusGP << dbg::endl
                << L" vit->m_bWin : " << (vit->m_bWin?L"TRUE":L"FALSE") << dbg::endl
                << L"--- End Dump ---" ) << END_LOG;
        }
    }
}

void KResultManager::CalcAngelEggExpGp( IN const KGameInfo& kGameInfo_, IN std::map< std::pair<DWORD,char>, __int64 >& mapResultExp_, IN std::map<DWORD,int>& mapResultGP_, IN OUT KEndGameReq& kReq_ )
{
    std::vector<KGameResultIn>::iterator vit;
    for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
        // 보상을 초기화 한다.
        InitResultIn( *vit );

        if ( vit->m_vecCharExp.empty() ) {
            vit->m_vecCharExp.resize(1);
            InitResultIn( *vit );
            vit->m_bWin = false;
            continue;
        }

        vit->m_vecCharExp.resize( 1 ); // 태그 매치가 아니면 모두 1명이다.

        // 각 각의 유제 객체 참조해서 경험치를 얻어 온다.
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
        if ( !spUser ) continue;

        // vit->m_vecCharExp의 사이즈가 0이 아니기 때문에 사용한다. (캐릭터얻는데만 사용)
        const KCharacterInfo* pkPlayCharacterInfo = spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
        if ( !pkPlayCharacterInfo ) continue;

        // 승리시 : 경험치 = 캐릭터레벨에 해당하는 몬스터 경험치 * 3
        // 패배시 : 경험치 = 캐릭터레벨에 해당하는 몬스터 경험치 * 1.5
        int nExp = SiKGameDrop()->GetCharLvMonExp( pkPlayCharacterInfo->m_dwLevel );
        float fExpRatio = ( vit->m_bWin ? GetAngelEggWinRatio() : GetAngelEggLoseRatio() );

        vit->m_vecCharExp.begin()->m_nAdditionnalExp = static_cast<int>( nExp * fExpRatio );
        vit->m_nAddtionalGp = 0;

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) {
            START_LOG( cwarn, L"--- Dump CalcAngelEgg ExpGP ---" << dbg::endl
                << L" Login : " << spUser->GetName() << dbg::endl
                << L" vit->m_vecCharExp.begin()->m_nBaseExp : " << vit->m_vecCharExp.begin()->m_nBaseExp << dbg::endl
                << L" vit->m_nBaseGP : " << vit->m_nBaseGP << dbg::endl
                << L" vit->m_vecCharExp.begin()->m_nAdditionnalExp : " << vit->m_vecCharExp.begin()->m_nAdditionnalExp << dbg::endl
                << L" vit->m_nAddtionalGp : " << vit->m_nAddtionalGp << dbg::endl
                << L" Win : " << (vit->m_bWin?L"TRUE":L"FALSE") << dbg::endl
                << L" CharType : " << vit->m_vecCharExp.begin()->m_cCharType  << dbg::endl
                << L" CharLevel : " << pkPlayCharacterInfo->m_dwLevel << dbg::endl
                << L" nExp : " << nExp << dbg::endl
                << L" WinRatio : " << GetAngelEggWinRatio() << dbg::endl
                << L" LoseRatio : " << GetAngelEggLoseRatio() << dbg::endl
                << L"--- End Dump ---" ) << END_LOG;
        }
    }
}

void KResultManager::GetGroupItemAll( IN const std::vector<KGameResultIn>& vecGameResult_, IN const int& nModeID_, OUT std::map<int,std::set<GCITEMID> >& mapGroupItemAll_ )
{
    int nCategory = -1;
    if ( false == SiKChannelManager()->GetModeCategory( nModeID_, nCategory ) ) {
        START_LOG( cwarn, L"게임모드에 해당하는 카테고리가 없음.. ModeID : " << nModeID_ ) << END_LOG;
        return;
    }

    std::vector<KInvenBuffItem> vecGroupList;
    SiKItemManager()->GetInvenBuffItemList( vecGroupList );

    std::vector<KGameResultIn>::const_iterator vit;
    for( vit = vecGameResult_.begin() ; vit != vecGameResult_.end() ; ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;

        std::vector<KInvenBuffItem>::iterator vitGroup;
        for ( vitGroup = vecGroupList.begin() ; vitGroup != vecGroupList.end() ; ++vitGroup ) {
            // 전체 적용인가?
            if ( false == vitGroup->m_bAll ) {
                continue;
            }

            // 그룹 설정인가?
            if ( vitGroup->m_nGroupID < 0 ) {
                continue;
            }

            // 카테고리 확인
            if ( vitGroup->m_setModeCategory.find( nCategory ) == vitGroup->m_setModeCategory.end() ) {
                continue;
            }

            // 인벤토리 확인
            if ( NULL == spUser->m_kInventory.FindItemByItemID( vitGroup->m_ItemID ) ) {
                continue;
            }

            mapGroupItemAll_[vitGroup->m_nGroupID].insert( vitGroup->m_ItemID );
        }
    }

    START_LOG( cwarn, L"전체적용 그룹 아이템.. size : " << mapGroupItemAll_.size()  ) << END_LOG;
}

void KResultManager::CheckInventoryBoostGroupItem( IN const std::map<int,std::set<GCITEMID> >& mapGroupItem_,
                                                   IN const std::map<int,std::set<GCITEMID> >& mapGroupItemAll_,
                                                   IN OUT std::map<GCITEMID,sBoostInfo>& mapBoostAllUser_,
                                                   IN OUT float& fExpBoost_,
                                                   IN OUT float& fGPBoost_ )
{
    std::map< GCITEMID, sBoostInfo >::const_iterator mit;
    std::map< std::pair<char, char>, sBoostDetailInfo >::const_iterator mitBoostDetail;

    std::map<int,std::set<GCITEMID> >::const_iterator mitGroupItem;
    for ( mitGroupItem = mapGroupItem_.begin() ; mitGroupItem != mapGroupItem_.end() ; ++mitGroupItem ) {
        const int& nGroupID = mitGroupItem->first; // 그룹ID

        std::map<USHORT,GCITEMID> mapItemPriority;
        SiKItemManager()->GetGroupItemID( nGroupID, mapItemPriority );

        std::map<USHORT,GCITEMID>::iterator mitPriority;
        for ( mitPriority = mapItemPriority.begin() ; mitPriority != mapItemPriority.end() ; ++mitPriority ) {
            const USHORT& PriorityIndex = mitPriority->first; // 우선순위
            const GCITEMID& ItemID = mitPriority->second; // ItemID

            // 우선순위의 아이템을 검색
            if ( mitGroupItem->second.find( ItemID ) == mitGroupItem->second.end() ) {
                // 파티원의 그룹 아이템을 확인
                std::map<int,std::set<GCITEMID> >::const_iterator mitGroupAll;
                mitGroupAll = mapGroupItemAll_.find( nGroupID );
                if ( mitGroupAll != mapGroupItemAll_.end() ) {
                    if ( mitGroupAll->second.find( ItemID ) == mitGroupAll->second.end() ) {
                        continue;
                    }
                    else {
                        START_LOG( clog, L"파티원의 그룹 아이템 우선순위가 높음으로 내 아이템은 무시함." )
                            << BUILD_LOG( nGroupID )
                            << BUILD_LOG( PriorityIndex )
                            << BUILD_LOG( ItemID ) << END_LOG;
                    }
                }
                else {
                    continue;
                }
            }
            else {
                START_LOG( clog, L"내 그룹 아이템 우선순위가 높음으로 파티원의 그룹 아이템은 무시함." )
                    << BUILD_LOG( nGroupID )
                    << BUILD_LOG( PriorityIndex )
                    << BUILD_LOG( ItemID ) << END_LOG;
            }

            // 버프정보 확인
            mit = m_mapBoostInfo.find( ItemID );
            if ( mit == m_mapBoostInfo.end() ) {
                START_LOG( clog, L"적용할 우선순위 그룹 아이템을 찾았지만 Exp/GP버프는 없음." )
                    << BUILD_LOG( nGroupID )
                    << BUILD_LOG( PriorityIndex )
                    << BUILD_LOG( ItemID ) << END_LOG;
                break;
            }

            // 적용
            mitBoostDetail = mit->second.m_mapBoostDetailInfo.find( std::make_pair( -1, -1) );
            if ( mitBoostDetail != mit->second.m_mapBoostDetailInfo.end() ){
                fExpBoost_  += std::max<float>(0.f, mitBoostDetail->second.m_fBoostExpRatio);
                fGPBoost_   += std::max<float>(0.f, mitBoostDetail->second.m_fBoostGpRatio);

                START_LOG( clog, L"인벤토리 EXP/GP 그룹 아이템(기간제)" )
                    << BUILD_LOG( nGroupID )
                    << BUILD_LOG( PriorityIndex )
                    << BUILD_LOG( ItemID )
                    << BUILD_LOG( mitBoostDetail->second.m_fBoostExpRatio )
                    << BUILD_LOG( mitBoostDetail->second.m_fBoostGpRatio )
                    << BUILD_LOG( fExpBoost_ )
                    << BUILD_LOG( fGPBoost_ )
                    << BUILD_LOG( mit->second.m_bAll ) << END_LOG;
                break;
            }
        }
    }
}

int KResultManager::GetLastDifficulty( IN const int& nModeID_ )
{
    int nLastDifficulty = -1;

    KLocker lock( m_csQuestExpGp );

    std::map< int, sQuestInfo >::const_iterator cmitQuest = m_mapQuestExpGp.find( nModeID_ );
    if ( cmitQuest == m_mapQuestExpGp.end() ) {
        START_LOG( cwarn, L"cmitQuest == m_mapQuestExpGp.end().. ModeID : " << nModeID_ ) << END_LOG;
        return nLastDifficulty;
    }

    if ( cmitQuest->second.m_mapStageInfo.empty() ) {
        START_LOG( cwarn, L"cmitQuest->second.m_mapStageInfo.empty().. ModeID : " << nModeID_ ) << END_LOG;
        return nLastDifficulty;
    }

    std::map< int, sStageInfo >::const_reverse_iterator crmitStage;
    for ( crmitStage = cmitQuest->second.m_mapStageInfo.rbegin() ; crmitStage != cmitQuest->second.m_mapStageInfo.rend() ; ++crmitStage ) {
        if ( crmitStage->first >= GC_GMD_CHAMPION_BEGIN ) continue;
        nLastDifficulty = crmitStage->first;
        break;
    }

    return nLastDifficulty;
}

void KResultManager::CalcELOExpGpRatio( IN OUT int& nExp_, IN OUT int& nGP_ )
{
    nExp_ = static_cast<int>( nExp_ * SiKPVPMatchManager()->GetELOAddExpRatio() );
    nGP_  = static_cast<int>( nGP_ * SiKPVPMatchManager()->GetELOAddGPRatio() );
}

void KResultManager::CheckEscortBoost( IN bool bIsEscortMonsterAlive_, IN DWORD dwEscortMonID_, IN int nGameMode_, IN int nDifficulty_, IN OUT float& fExpBoost_, IN OUT float& fGpBoost_ )
{
    // 이벤트 기간중에 던전을 시작하여 호위 몬스터가 클라에서 표시되었지만 보상을 받지 못하는 경우를
    // 방지하기 위해서 여기에서 호위 이벤트 기간을 확인하지 않는다

    // 만약 문제가 생긴다면 이 곳 결과처리에서도 이벤트 기간을 확인하도록 한다

    /*if ( false == SiKEscort()->IsRun() ) {
        return;
    }*/

    if ( false == SiKEscort()->IsEscortMonster( dwEscortMonID_ ) ) {
        return;
    }

    if ( false == SiKEscort()->IsHaveRewardInfo( nGameMode_, nDifficulty_, dwEscortMonID_ ) ) {
        return;
    }

    // 보상 정보가 존재하는 경우
    if ( true == bIsEscortMonsterAlive_ ) {  // true면 살아있음
        // 살아있는 보상 EXP, GP 지급
        KEscort::KEscortReward kAliveReward;
        if ( false == SiKEscort()->GetAliveReward( dwEscortMonID_, kAliveReward ) ) {
            return;
        }

        float fExpRatio = std::max< float >( 0.f, kAliveReward.m_fExtraExp );
        float fGpRatio = std::max< float >( 0.f, kAliveReward.m_fExtraGp );

        fExpBoost_ += std::max< float >( 0.f, fExpRatio );
        fGpBoost_ += std::max< float >( 0.f, fGpRatio );
    }
    else {
        // 호위 몹 죽었을 경우 보상 EXP, GP 지급
        KEscort::KEscortReward kDeadReward;
        if ( false == SiKEscort()->GetDeadReward( dwEscortMonID_, kDeadReward ) ) {
            return;
        }

        float fExpRatio = std::max< float >( 0.f, kDeadReward.m_fExtraExp );
        float fGpRatio = std::max< float >( 0.f, kDeadReward.m_fExtraGp );

        fExpBoost_ += std::max< float >( 0.f, fExpRatio );
        fGpBoost_ += std::max< float >( 0.f, fGpRatio );
    }
}

void KResultManager::ControlFeedbackExpBoost( IN const KGameInfo& kGameInfo_, IN OUT float& fExpBoost_, IN int& nRank_ )
{
    if( !KRoom::IsDungeonMode( kGameInfo_.m_iGameMode ) ) {
        return;
    }

    std::map< int, float >::iterator mapExpBonus;
    float fTmpExp;

    KLocker lock( m_csControlFeedback );
    mapExpBonus = m_mapControlFeedbackExpBonusRatio.find( nRank_ );

    if ( m_mapControlFeedbackExpBonusRatio.end() != mapExpBonus ) {
        fTmpExp = mapExpBonus->second;
        fExpBoost_ += std::max<float>(0.f, fTmpExp);
    }
}
