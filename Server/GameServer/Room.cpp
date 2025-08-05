#include "User.h"
#include "Room.h"
#include "RoomFSM.h"
#include "UserFSM.h"
#include "Channel.h"
#include "GSSimLayer.h"
#include "UserEvent.h"
#include "FailRate.h"
#include "GSDBLayer.h"
#include "GameServer.h"
#include "Guild.h"
#include "LogManager.h"
#include <boost/bind.hpp>
#include "DungeonEntrance.h"
#include "GCHelper.h"
#include "StatisticsManager.h"
#include "StatGameStartGap.h"
#include "PreHackingCheckManager.h"
#include "HeroDungeonManager.h"
#include "ResultManager.h"
#include "Worker.h"
#include "Log4.h"
#include "DungeonManager.h"
#include <boost/foreach.hpp>
#include "VitalityManager.h"

#define DEATH_MIN_TIME              300
#define DEATH_MIN_KILL              10
#define DEATH_MAX_KILL              30
#define DEATH_DEFAULT_KILL          20

namespace
{
    class KComp
    {
    public:
        bool operator() ( const sTimeEvent& a, const sTimeEvent& b )
        {
            if( a.m_nEventUID < b.m_nEventUID ) return true;
            return false;
        }
    };

    class KEqu
    {
    public:
        bool operator() ( const sTimeEvent& a, const sTimeEvent& b )
        {
            if( a.m_nEventUID == b.m_nEventUID ) return true;
            return false;
        }
    };
}
KDropSystem                 KRoom::ms_kDropSystem;
std::vector< int >          KRoom::ms_avecModeCount[NUM_GC_GAME_MODE];
KEVENT_STAT_PROMOTION_COUNT KRoom::ms_mapPromotionCount; // std::map< short, DWORD>
unsigned int                KRoom::ms_uiUpdateStatTick = ::GetTickCount();


inline DWORD int_pow(int base, int exp)
{
	DWORD result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp /= 2;
		base *= base;
	}
	return result;
}

NiImplementRTTI( KRoom, KSimObject );

#define MAX_PROMOTION 3

const int KRoom::ms_iDifficultyModes[] = {
    GC_GM_QUEST0,
    GC_GM_QUEST1,
    GC_GM_QUEST2,
    GC_GM_QUEST3,
    GC_GM_QUEST4,
    GC_GM_QUEST5,
    GC_GM_QUEST6,
    GC_GM_QUEST7,
    GC_GM_QUEST8,
    GC_GM_QUEST9,
    GC_GM_QUEST10,
    GC_GM_QUEST11,
    GC_GM_QUEST12,
    GC_GM_QUEST13,
    GC_GM_QUEST14,
    GC_GM_QUEST15,
    GC_GM_QUEST16,
    GC_GM_QUEST17,
    GC_GM_QUEST18,
    GC_GM_QUEST19,
    GC_GM_QUEST20,
    GC_GM_MONSTER_HUNT,
    GC_GM_QUEST21,
    GC_GM_QUEST22,
    GC_GM_QUEST23,
    GC_GM_QUEST24,
    GC_GM_QUEST25,
    GC_GM_QUEST26,
    GC_GM_QUEST27,
    GC_GM_QUEST28,
    GC_GM_QUEST29,
    GC_GM_QUEST30,
    GC_GM_QUEST31,
    GC_GM_QUEST32,
    GC_GM_QUEST33,
    GC_GM_QUEST34,
    GC_GM_QUEST35,
    GC_GM_QUEST36,
    GC_GM_QUEST37,
    GC_GM_QUEST38,
    GC_GM_QUEST39,
    GC_GM_QUEST40,
    GC_GM_QUEST41,
    GC_GM_QUEST42,
    GC_GM_QUEST43,
    GC_GM_QUEST44,
    GC_GM_QUEST45,
    GC_GM_QUEST46,
    GC_GM_QUEST47,
    GC_GM_QUEST48,
    GC_GM_QUEST49,
    GC_GM_QUEST50,
    GC_GM_QUEST51,
    GC_GM_QUEST52,
    GC_GM_QUEST53,
    GC_GM_QUEST54,
    GC_GM_QUEST55,
    GC_GM_QUEST56,
    GC_GM_QUEST57,
    GC_GM_QUEST58,
    GC_GM_QUEST59,
    GC_GM_QUEST60,
    GC_GM_QUEST61,
    GC_GM_QUEST62,
    GC_GM_QUEST63,
    GC_GM_QUEST64,
    GC_GM_QUEST65,
    GC_GM_QUEST66,
    GC_GM_QUEST67,
    GC_GM_QUEST68,
    GC_GM_QUEST69,
    GC_GM_QUEST70,
    GC_GM_QUEST71,
    GC_GM_QUEST72,
    GC_GM_QUEST73,
    GC_GM_QUEST74,
    GC_GM_QUEST75,
    GC_GM_QUEST76,
    GC_GM_QUEST77,
    GC_GM_QUEST78,
    GC_GM_QUEST79,
    GC_GM_QUEST80,
    GC_GM_QUEST81,
    GC_GM_QUEST82,
    GC_GM_QUEST83,
    GC_GM_QUEST84,
    GC_GM_QUEST85,
    GC_GM_QUEST86,
    GC_GM_QUEST87,
    GC_GM_QUEST88,
    GC_GM_QUEST89,
    GC_GM_QUEST90,
    GC_GM_QUEST91,
    GC_GM_QUEST92,
    GC_GM_QUEST93,
    GC_GM_QUEST94,
    GC_GM_QUEST95,
    GC_GM_QUEST96,
    GC_GM_QUEST97,
    GC_GM_QUEST98,
    GC_GM_QUEST99,
    GC_GM_QUEST100,
    GC_GM_QUEST101,
    GC_GM_QUEST102,
    GC_GM_QUEST103,
    GC_GM_QUEST104,
};

const int KRoom::ms_iDungeonModes[] = {
    GC_GM_QUEST0,
    GC_GM_QUEST1,
    GC_GM_QUEST2,
    GC_GM_QUEST3,
    GC_GM_QUEST4,
    GC_GM_QUEST5,
    GC_GM_QUEST6,
    GC_GM_QUEST7,
    GC_GM_QUEST8,
    GC_GM_QUEST9,
    GC_GM_QUEST10,
    GC_GM_QUEST11,
    GC_GM_QUEST12,
    GC_GM_QUEST13,
    GC_GM_QUEST14,
    GC_GM_QUEST15,
    GC_GM_QUEST16,
    GC_GM_QUEST17,
    GC_GM_QUEST18,
    GC_GM_QUEST19,
    GC_GM_QUEST20,
    GC_GM_QUEST21,
    GC_GM_QUEST22,
    GC_GM_QUEST23,
    GC_GM_QUEST24,
    GC_GM_QUEST25,
    GC_GM_QUEST26,
    GC_GM_QUEST27,
    GC_GM_QUEST28,
    GC_GM_QUEST29,
    GC_GM_QUEST30,
    GC_GM_QUEST31,
    GC_GM_QUEST32,
    GC_GM_QUEST33,
    GC_GM_QUEST34,
    GC_GM_QUEST35,
    GC_GM_QUEST36,
    GC_GM_QUEST37,
    GC_GM_QUEST38,
    GC_GM_QUEST39,
    GC_GM_QUEST40,
    GC_GM_QUEST41,
    GC_GM_QUEST42,
    GC_GM_QUEST43,
    GC_GM_QUEST44,
    GC_GM_QUEST45,
    GC_GM_QUEST46,
    GC_GM_QUEST47,
    GC_GM_QUEST48,
    GC_GM_QUEST49,
    GC_GM_QUEST50,
    GC_GM_QUEST51,
    GC_GM_QUEST52,
    GC_GM_QUEST53,
    GC_GM_QUEST54,
    GC_GM_QUEST55,
    GC_GM_QUEST56,
    GC_GM_QUEST57,
    GC_GM_QUEST58,
    GC_GM_QUEST59,
    GC_GM_QUEST60,
    GC_GM_QUEST61,
    GC_GM_QUEST62,
    GC_GM_QUEST63,
    GC_GM_QUEST64,
    GC_GM_QUEST65,
    GC_GM_QUEST66,
    GC_GM_QUEST67,
    GC_GM_QUEST68,
    GC_GM_QUEST69,
    GC_GM_QUEST70,
    GC_GM_QUEST71,
    GC_GM_QUEST72,
    GC_GM_QUEST73,
    GC_GM_QUEST74,
    GC_GM_QUEST75,
    GC_GM_QUEST76,
    GC_GM_QUEST77,
    GC_GM_QUEST78,
    GC_GM_QUEST79,
    GC_GM_QUEST80,
    GC_GM_QUEST81,
    GC_GM_QUEST82,
    GC_GM_QUEST83,
    GC_GM_QUEST84,
    GC_GM_QUEST85,
    GC_GM_QUEST86,
    GC_GM_QUEST87,
    GC_GM_QUEST88,
    GC_GM_QUEST89,
    GC_GM_QUEST90,
    GC_GM_QUEST91,
    GC_GM_QUEST92,
    GC_GM_QUEST93,
    GC_GM_QUEST94,
    GC_GM_QUEST95,
    GC_GM_QUEST96,
    GC_GM_QUEST97,
    GC_GM_QUEST98,
    GC_GM_QUEST99,
    GC_GM_QUEST100,
    GC_GM_QUEST101,
    GC_GM_QUEST102,
    GC_GM_QUEST103,
    GC_GM_QUEST104,
};

const int KRoom::ms_iPvPModes[] = {
    GC_GM_TEAM_MODE,
    GC_GM_SURVIVAL_MODE,
    GC_GM_TAG_TEAM,
    GC_GM_TAG_SURVIVAL,
    GC_GM_GUILD_BATTLE,
    GC_GM_INDIGO_TEAM,
    GC_GM_DEATH_TEAM,
    GC_GM_DEATH_SURVIVAL,
    GC_GM_ANGELS_EGG,
    GC_GM_CAPTAIN,
    GC_GM_DOTA,
    GC_GM_AUTOMATCH_TEAM,
    GC_GM_FATAL_DEATH_TEAM,
    GC_GM_FATAL_DEATH_SURVIVAL,
    GC_GM_MONSTER_TEAM,
    GC_GM_MONSTER_SURVIVAL
};

const wchar_t KRoom::ms_szModeName[][MC_MAX_STRING] = {
    L"Tutorial",                    // 튜토리얼            //GM_TUTORIAL                   
    L"Team Mode",                   // 팀 모드             //GM_TEAM_MODE
    L"Survival Mode",               // 서바이벌 모드       //GM_SURVIVAL_MODE
    L"Tag Match Team",              // 태그매치 팀         //GM_TAG_TEAM
    L"Tag Match Survival",          // 태그매치 서바이벌   //GM_TAG_SURVIVAL
    L"Guild Battle",                // 길드전              //GM_GUILD_BATTLE
    L"Hero''s Island",              // 용사의 섬           //GM_INDIGO_TEAM
    L"Trial Forest",                // 시련의 숲           //GM_QUEST0
    L"Trial Tower",                 // 시련의 탑           //GM_QUEST1
    L"Outer wall of Serdin",        // 세르딘성 외곽       //GM_QUEST2
    L"Kerrie Beach",                // 캐리비치            //GM_QUEST3
    L"Orc Temple",                  // 오크사원            //GM_QUEST4
    L"Gorgos'' Dungeon",            // 고르고스 던전       //GM_QUEST5
    L"Elven Forest",                // 엘프의 숲           //GM_QUEST6
    L"Gorge of Oath",               // 맹세의 계곡         //GM_QUEST7
    L"Marsh of Oblivion",           // 망각의 늪           //GM_QUEST8
    L"Forsaken Barrows",            // 망자의 묘지         //GM_QUEST9
    L"Forgotten City",              // 잊혀진 도시         //GM_QUEST10
    L"Gaikoz''s Castle",            // 가이코즈의 성       //GM_QUEST11
    L"Partusay''s Sea",             // 파투세이의 바다     //GM_QUEST12
    L"Ellia Continent",             // 엘리아 대륙         //GM_QUEST13
    L"The Temple of Fire",          // 불의 사원           //GM_QUEST14
    L"Hell Bridge",                 // 헬 브릿지           //GM_QUEST15
    L"Kaze''aze''s Castle",         // 카제아제의 성       //GM_QUEST16
    L"Kastulle Ruins Bottom Floor", // 카스툴 유적지 하층  //GM_QUEST17
    L"Kastulle Ruins Upper Floor",  // 카스툴 유적지 상층  //GM_QUEST18
    L"Battle for Bermesiah",        // 베르메시아의 최후   //GM_QUEST19
    L"Xenia Border",                // 제니아 접경지       //GM_QUEST20
    L"Monster Expedition",          // 몬스터 원정대       //GC_GM_MONSTER_CRUSADER
    L"Monster Battle",              // 몬스터 대전         //GC_GM_MONSTER_HUNT
    L"Temple of Cuatal",            // 순환의 신전         //GC_GM_QUEST21
    L"Death Match Team",            // 데스매치 팀         //GC_GM_DEATH_TEAM
    L"Death Match Survival",        // 데스매치 서바이벌   //GC_GM_DEATH_SURVIVAL
    L"Log drop",                    // 통나무 떨어지기     //GC_GM_MINIGAME_TREEDROP
    L"Balloon pop",                 // 풍선 터뜨리기       //GC_GM_MINIGAME_BALLOON
    L"Butterfly catch",             // 나비 잡기           //GC_GM_MINIGAME_BUTTERFRY
    L"Valstrath",                   // 소실의 협곡         //GC_GM_QUEST22
    L"Angel''s egg",                // 천사의알            //GC_GM_ANGELS_EGG
    L"Leader Battle",               // 대장전              //GC_GM_CAPTAIN
    L"The Whispering Woods",        // 성난 엔트의 숲      //GC_GM_QUEST23
    L"Ruins of silver Knights",     // 실버나이츠의 폐허   //GC_GM_QUEST24
    L"Lake Aurora",                 // 미명의 호수         //GC_GM_QUEST25
    L"The Wyrm''s Maw",             // 이그니스의 산맥     //GC_GM_QUEST26
    L"King Guang''s Fen",           // 수룡의 늪             //GC_GM_QUEST27
    L"Primeval Island",             // 실버랜드 원시의 섬    //GC_GM_QUEST28
    L"Drake Worm",                  // 실버랜드 드레이크 웜  //GC_GM_QUEST29
    L"Fortress of victor",          // 실버랜드 빅터의 요새  //GC_GM_QUEST30
    L"Altar of judgement",          // 제니아 빙하 조율의 제단  //GC_GM_QUEST31
    L"Temple of Destruction",       // 제니아 파괴의 신전    //GC_GM_QUEST32
    L"Forest of Life",              // 제니아 생명의 숲      //GC_GM_QUEST33
    L"Castle of Domination",        // 제니아 지배의 성      //GC_GM_QUEST34
    L"Bermesiah Boss Dungeon",      // 베르메시아 보스 던전  // GC_GM_QUEST35
    L"Silver Land Boss Dungeon",    // 실버랜드 보스 던전    // GC_GM_QUEST36
    L"Ellia Boss Dungeon",          // 엘리아 보스 던전      // GC_GM_QUEST37
    L"Xenia Boss Dungeon",          // 제니아 보스 던전      // GC_GM_QUEST38
    L"Goblin Research",             // 고블린연구소          // GC_GM_QUEST39
    L"Underground Tunnel",          // 지하터널              // GC_GM_QUEST40
    L"Kungji Village",              // 쿵지마을              // GC_GM_QUEST41
    L"Dwarf Base",                  // 드워프기지            // GC_GM_QUEST42
    L"Mirage Desert",               // 신기루사막            // GC_GM_QUEST43
    L"Dwarf Supply line",           // 드워프보급로          // GC_GM_QUEST44
    L"Thunder Hammer",              // 썬더해머              // GC_GM_QUEST45
    L"Hero Dungeon Ellia",          // 영웅던전 엘리아       // GC_GM_QUEST46
    L"Hero Dungeon Bermesiah",      // 영웅던전 베르메시아   // GC_GM_QUEST47
    L"Wreck of Ancient Kingdom",    // 고대 왕국의 잔해      // GC_GM_QUEST48
    L"Athena",                      // 점령전.               // GC_GM_DOTA
    L"Agit",                        // 아지트                // GC_GM_AGIT
    L"Event Carnival",              // 카니발 이벤트         // GC_GM_QUEST49
    L"Event RelayDungeon",          // Event RelayDungeon    // GC_GM_QUEST50
    L"Event HeroDungeon",           // Event HeroDungeon     // GC_GM_QUEST51
    L"Relay HeroDungeon",           // Relay HeroDungeon     // GC_GM_QUEST52
    L"Monster Train",               // Monster Train         // GC_GM_QUEST53
    L"HungryPet",                   // HungryPet             // GC_GM_QUEST54
    L"KaruelBorder",                // 카루엘 접경지         // GC_GM_QUEST55
    L"Karuel",                      // 카루엘                // GC_GM_QUEST56
    L"NightMarket",                 // 야시장                // GC_GM_QUEST57
    L"RaidDungeon",                 // 레이드던전            // GC_GM_QUEST58
    L"AutoMatch Team",              // 팀 자동매칭           // GC_GM_AUTOMATCH_TEAM
    L"Atum 5th",                    // 사막 도적마을         // GC_GM_QUEST59
    L"Atum 6th",                    // 라 협곡               // GC_GM_QUEST60
    L"Atum 7th",                    // 세트의 피라미드       // GC_GM_QUEST61
    L"Acheamedia 7th",              // 코우나트 붕괴지       // GC_GM_QUEST62
    L"Acheamedia 7th Event",        // 코우나트 멘붕괴지     // GC_GM_QUEST63
    L"CownatSlippedLand",           // 이벤트 던전(슈팅던전) // GC_GM_QUEST64
    L"Dungeon_CursedGarden",        // 아델 신대륙 ( 저주받은 정원 )
    L"Dungeon_SoiledBasement",      // 아델 신대륙 ( 오염된 지하실 )
    L"Dungeon_Specter_Hallways",    // 아델 신대륙 ( 망령이 깃든 복도 )
    L"Betreayer Atelier",           // 아델 신대륙 ( 배신자의 화실 )
    L"Dark Library",                // 아델 신대륙 ( 어둠에 물든 서재 )
    L"Arachne Cobweb",              // 아델 신대륙 ( 아라크네의 둥지 )
    L"Trivia 1st",                  // 베이가스 신대륙 1
    L"Trivia 2nd",                  // 베이가스 신대륙 2
    L"Trivia 3rd",                  // 베이가스 신대륙 3
    L"Trivia 4th",                  // 베이가스 신대륙 4
    L"Trivia 5th",                  // 베이가스 신대륙 5
    L"Trivia 6th",                  // 베이가스 신대륙 6
    L"Fatal Death Match Team",      // 페이탈 데스매치 팀       // GC_GM_FATAL_DEATH_TEAM
    L"Fatal Death Match Survival",  // 페이탈 데스매치 서바이벌 // GC_GM_FATAL_DEATH_SURVIVAL
    L"InfinityDungeon_Elia",        // 엘리아 전장(사냥터)        // GC_GM_QUEST77
    L"InfinityDungeon_Xenia",       // 제니아 전장(사냥터)        // GC_GM_QUEST78
    L"InfinityDungeon_Atum",        // 아툼 전장(사냥터)          // GC_GM_QUEST79
    L"InfinityDungeon_Archemedia",  // 아케메디아 전장(사냥터)    // GC_GM_QUEST80
    L"Tower Of Extinction",     // 소멸의 탑(만렙컨트롤 던전)   // GC_GM_QUEST81
    L"Apocalipse",     // 소멸의 탑(만렙컨트롤 던전)   // GC_GM_QUEST84
};

const int KRoom::ms_naMaps[] = {
    -1,                             // Random Map
    GC_GS_FOREST_OF_ELF,            // 엘프의 숲
    GC_GS_SWAMP_OF_OBLIVION,        // 망각의 늪
    GC_GS_FLYING_SHIP,              // 비공정
    GC_GS_VALLEY_OF_OATH,           // 맹세의 계곡
    GC_GS_FOGOTTEN_CITY,            // 잊혀진 도시
    GC_GS_BABEL_OF_X_MAS,           // X-마스의 바벨
    GC_GS_TEMPLE_OF_FIRE,           // 불의 사원
    GC_GS_SHOOTING_FIELD,           // 사격 대회장
    GC_GS_HELL_BRIDGE,              // 헬 브릿지
    GC_GS_TEMPLE_OF_ORC,            // 오크 사원
    GC_GS_GUILD,                    // 길드맵(세르딘 성 외곽)
    GC_GS_CARRIBEACH,               // 캐리비치
    GC_GS_FOREST_OF_TRIAL,          // 시련의 숲
    GC_GS_GRAVEYARD_OF_DECEASED,    // 망자의 묘지
    GC_GS_BAHIA,                    // Bahia
    GC_GS_AMAZON_FOREST,            // Amazon Forest
    GC_GS_FOREST_OF_ELF_REVERS,     // 엘프의 숲 반전
    GC_GS_VALLEY_OF_OATH_REVERS,    // 맹세의 계곡 반전
    GC_GS_CASTLE_OF_RULE,           // 지배의 성
};

const wchar_t KRoom::ms_szMapName[][MC_MAX_STRING] = {
    L"랜덤 맵",             // -1
    L"엘프의 숲",           // GC_GS_FOREST_OF_ELF
    L"망각의 늪",           // GC_GS_SWAMP_OF_OBLIVION
    L"비공정",              // GC_GS_FLYING_SHIP
    L"맹세의 계곡",         // GC_GS_VALLEY_OF_OATH
    L"잊혀진 도시",         // GC_GS_FOGOTTEN_CITY
    L"X-마스의 바벨",       // GC_GS_BABEL_OF_X_MAS
    L"불의 사원",           // GC_GS_TEMPLE_OF_FIRE+
    L"사격 대회장",         // GC_GS_SHOOTING_FIELD
    L"헬 브릿지",           // GC_GS_HELL_BRIDGE
    L"오크 사원",           // GC_GS_TEMPLE_OF_ORC
    L"세르딘 성 외곽",      // GC_GS_GUILD
    L"캐리비치",            // GC_GS_CARRIBEACH
    L"시련의 숲",           // GC_GS_FOREST_OF_TRIAL
    L"망자의 묘지",         // GC_GS_GRAVEYARD_OF_DECEASED
    L"Bahia",               // GC_GS_BAHIA
    L"Amazon Forest",       // GC_GS_AMAZON_FOREST
    L"엘프의 숲 반전",      // GC_GS_FOREST_OF_ELF_REVERS
    L"맹세의 계곡 반전",     // GC_GS_VALLEY_OF_OATH_REVERS
    L"지배의 성"            // GC_GS_CASTLE_OF_RULE
};

int KRoom::GetMaxDifficulty( int iMode_ )
{
    if ( !IsDifficultyMode( iMode_ ) ) return -1;

    if ( GC_GM_MONSTER_HUNT == iMode_ ) return MAX_DIFFICULTY_MONSTER_HUNT;
    if ( GC_GM_QUEST52 == iMode_ ) return MAX_DIFFICULTY_RELAY_HERO;

    return MAX_DIFFICULTY_QUEST;
}

bool KRoom::IsDifficultyMode( int iMode_ )
{
    const int* arrBegin = ms_iDifficultyModes;
    const int* arrEnd   = ms_iDifficultyModes + NUM_ELEMENTS( ms_iDifficultyModes );

    return std::find( arrBegin, arrEnd, iMode_ ) != arrEnd;
}

bool KRoom::IsDungeonMode( int iMode_ )
{
    const int* arrBegin = ms_iDungeonModes;
    const int* arrEnd   = ms_iDungeonModes + NUM_ELEMENTS( ms_iDungeonModes );

    return std::find( arrBegin, arrEnd, iMode_ ) != arrEnd;
}

bool KRoom::IsPvPMode( int iMode_ )
{
    const int* arrBegin = ms_iPvPModes;
    const int* arrEnd   = ms_iPvPModes + NUM_ELEMENTS( ms_iPvPModes );

    return std::find( arrBegin, arrEnd, iMode_ ) != arrEnd;

}
int KRoom::GetMapNum()
{
    return NUM_ELEMENTS( ms_naMaps );
}

void KRoom::InitModeCount()
{
    int nSize =1;

    for( int iCount = 0; iCount < NUM_GC_GAME_MODE; ++iCount )
    {
        if ( IsDifficultyMode( iCount ) ) {
            nSize = GetMaxDifficulty( iCount );
        }
        else {
            nSize = 1;
        }

        KRoom::ms_avecModeCount[iCount].clear();
        KRoom::ms_avecModeCount[iCount].resize( nSize, 0 );
    }
    KRoom::ms_uiUpdateStatTick = ::GetTickCount();
    
}

void KRoom::UpdateStatToDB()
{
    if ( false == SiKGSSimLayer()->IsUpdateRoomStat() ) {
        return;
    }

    if ( ::GetTickCount() - KRoom::ms_uiUpdateStatTick > SiKGSSimLayer()->GetRoomStatTickGap() ) {   // 한시간 주기 하드코딩.
        KDB_EVENT_WRITE_MODE_COUNT_NOT kNot;
        kNot.m_nMaxMonsterHuntLevel = KRoom::MAX_DIFFICULTY_MONSTER_HUNT;
        kNot.m_nMaxQuestLevel       = KRoom::MAX_DIFFICULTY_QUEST;
        for( int iCount = 0; iCount < NUM_GC_GAME_MODE; ++iCount )
        {
            kNot.m_avecModeCount[iCount] =  KRoom::ms_avecModeCount[iCount];
        }

        SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_WRITE_MODE_COUNT_NOT, NULL, 0, 0, kNot );
        KRoom::InitModeCount();

        SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::EVENT_STAT_PROMOTION_COUNT, NULL, 0, 0, KRoom::ms_mapPromotionCount );
        KRoom::ms_mapPromotionCount.clear();
    }
}

KRoom::KRoom( unsigned short usRoomID_ ) 
:m_usRoomID( usRoomID_ ) 
,m_usMaxUsers( 8 ) 
,m_bPublic( true ) 
,m_dwRelayServerIP( 0 )
,m_usRelayServerPort( 0 )
,m_dwTRServerIP( 0 )
,m_usTRServerPort( 0 )
,m_nDifficulty( -1 )
,m_iMapID( 0 )
,m_nObserverCnt( 0 )
,m_dwResultTick( 0 ) //게임 결과 창 나온 시간
,m_nSendResultEndUser( 0 ) //상태 종료를 호출한 유저수
,m_cRoutingMethod( 0 )
,m_biStartingTotalExp( 0 )
,m_dwStartingTotalGp( 0 )
,m_nDeathMatchTime( 0 )
,m_nDeathKillCount( 0 )
,m_dwRandomSeed( 0 )
,m_bJudgePoorP2P( false )
,m_bDeathMatchIntrudable( false )
,m_bDeathMatchBalancing( false )
,m_dwBanishCount( 3 ) // 방장 강퇴 Count 초기화( 3에서 감소하는 형식 )
,m_nWinTeam( -1 ) //  080604  woosh. 이긴 팀
,m_nMonsterCrusaderMonLv( 1 )
,m_dwLeaveGuildWhilePlaying( 0 )
,m_nMatchFailCount(0)
,m_bStageLoadSwitch(false)
,m_iStageClearcount(0)
{
    m_vecHostOrder.clear();
    m_mapInDoorUserInfo.clear();
    m_dwStartGameTime   = 0;
    m_iP2PVersion       = 0;
    m_cGameCategory     = GC_GMC_INVALID;
    m_iGameMode         = GC_GM_INVALID;
    m_iSubGameMode      = GC_GM_INVALID;
    m_iMapID            = GC_GM_INVALID;
    m_nDifficulty       = 0;
    m_cGrade            = 0;

    m_bRandomableMap    = false;
    ::ZeroMemory( m_naGuildID, sizeof(m_naGuildID) );

    m_mapEventList.clear();
    ClearDropInfo();
    ClearChampInfo();
    m_nMonsterID    = MON_INVALID;
    m_nMonsterCount = 0;
    m_vecStageLoadFailUser.clear();
    m_vecLoadFailUser.clear();
    m_mapResultGP.clear();
    m_mapResultExp.clear();
    m_mapMonDropGP.clear();
    m_pairGuildMarkName.first.clear();
    m_pairGuildMarkName.second.clear();
    m_pairGuildName.first.clear();
    m_pairGuildName.second.clear();
    m_pairBattlePoint.first = 0;
    m_pairBattlePoint.second = 0;
    ClearLevelUpUser();
    m_mapAbuseData.clear();
    ClearMonsterKillInfo();
    m_kAdventureMissionMonsterData.clear();
    ClearCompletedSubjectTrigger();
    m_tmGameStartTime = CTime::GetCurrentTime();
    m_tmGameEndTime = CTime::GetCurrentTime();
    ClearRemainVitalityOnStartGame();
    ClearMonsterKillCount();
    ClearUserUsePosition();
    ClearInfinityDungeonInfoOnStartGame();	
}

KRoom::~KRoom(void)
{
}

ImplToStringW( KRoom )
{
    START_TOSTRING_PARENTW( KSimObject )
        << TOSTRINGW( m_RTTI.GetName() )
        << TOSTRINGW( m_usRoomID )
        << TOSTRINGW( m_usMaxUsers )
        << TOSTRINGWb( m_bPublic )
        << TOSTRINGW( m_strPasswd );

    // dump all user name.
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::const_iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
    {
        stm_ << mit->second->GetName() << std::endl;
    }
    
    return stm_;
}

bool KRoom::Enter( IN KUser& kUser_, IN KInDoorUserInfo& kInDoorUserInfo_, IN int& nTeamType_ )
{
    KLocker lock( m_csSob );

    if ( false == kUser_.IsCharExist( kInDoorUserInfo_.m_cCharIndex ) ) { 
        SET_ERROR( ERR_ROOM_35 );
        return false;
    }

    if ( !kUser_.IsAdmin() ) { 
        if ( !kUser_.m_pkChannel->CheckEnteranceGrade( kUser_.m_cGrade ) && !SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) { 
            SET_ERROR( ERR_ROOM_36 );
            return false;
        }

        if ( GetCount() >= m_usMaxUsers ) {  // 영자가 아니고, 정원이 다 찬 경우
            SET_ERROR( ERR_ROOM_01 );
            return false;
        }

        //이미 공작 날개 받은 사람은 들어올수 없게 한다.
        std::pair<int, int> pairAdminEventItem;
        bool bIsAdminEventEnabled = ms_kDropSystem.GetAdminEventInfo( pairAdminEventItem );
        if ( bIsAdminEventEnabled && IsAdminExist() ) {              // 운영자 이벤트가 진행중이고, 일반 유저가 '조인' 할 때.
            if ( kUser_.CheckIfAllItemExist( pairAdminEventItem.first, -1 ) ) {  // 이벤트 아이템을 이미 가지고 있는 경우.

                SET_ERROR( ERR_ROOM_13 );
                return false;
            }
        }//end if
    }

    //옵저버가 정의된 수 이상은 들어올수 없다.( 현재 정의 MAX_OBSERVER_COUNT 2 )
    if ( kInDoorUserInfo_.m_bIsObserver && m_nObserverCnt >= MAX_OBSERVER_COUNT ) {
        SET_ERROR( ERR_ROOM_30 );
        return false;
    }

    if ( !kInDoorUserInfo_.m_bIsObserver && GetCount() >= m_usMaxUsers ) {
        // 옵저버가 아니고, 정원이 다 찬 경우
        SET_ERROR( ERR_ROOM_01 );
        return false;
    }

    if ( !KRoom::KSobManager<KUserPtr>::Add( kUser_ ) ) {
        SET_ERROR( ERR_ROOM_02 );
        return false;
    }

//이아래부터 방에 진입된것이다.
    kUser_.ClearPressInfo();
    if ( GetCount() == 1 ) {                  // 최초의 1인이 들어온 경우(방이 생성된 경우)

        StateTransition( KRoomFSM::INPUT_OPEN );

        // 050114. 방장유저의 포인터를 유지한다.
        m_spHostUser = std::static_pointer_cast<KUser>( kUser_.shared_from_this() );
        kInDoorUserInfo_.m_bIsHost = true;
        // 채널 타입을 기록해둔다.
        m_cChannelType = KChannel::CT_UNLIMITED;
        KChannel* pkChannel = kUser_.GetChannel();
        if ( pkChannel ) {
            m_cChannelType = pkChannel->GetType();
        }
        //  방이 생성될 때 방장의 강퇴 Count를 초기화 해준다.
        InitBanishCount();
        
        m_dwGameStartGap = ::GetTickCount(); // 방생성시 초기화.

        SetRoomHostPing( m_spHostUser->GetPing() );
        SetRoomHostNationCode( m_spHostUser->GetCountryCode() );  // 바뀌는 방장의 국가정보 설정.
        ClearNationReward(); // 국가별 혜택, 방생성시 초기화를 진행함.
    }

    AddInDoorUserInfo( kInDoorUserInfo_, nTeamType_ ); //빈슬롯등.. 기본 셋팅을 하게 된다.

    if ( kInDoorUserInfo_.m_bIsObserver ) { //옵저버가 들어오면 

        if( m_nObserverCnt < MAX_OBSERVER_COUNT ) {
            m_nObserverCnt += 1;
            m_nObserverCnt = std::max<int>( 0, m_nObserverCnt );
        }
        m_usMaxUsers++;
    }
    else {
        //옵저버는 호스트가 될수 없다.
        m_vecHostOrder.push_back( kUser_.GetThisPtr() ); //호스트 오더
    }

    if ( GetCount() >= m_usMaxUsers ) { // FULL
        StateTransition( KRoomFSM::INPUT_FULL );
    }

    if ( SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) {
        if ( SetGuildRoomArray( kUser_.GetGuildUID() ) ) {
            START_LOG( clog, L"길드전 방의 길드 UID 세팅 성공" ) << END_LOG;
        }
        else {
            START_LOG( clog, L"길드전 방의 길드 UID 세팅 실패" ) << END_LOG;
        }

        std::wstring guildMarkName = kUser_.GetGuildMarkName();
        if ( guildMarkName != L"" ) {
            if ( SetGuildMarkName( guildMarkName ) ) {
                START_LOG( clog, L"길드전 방의 길드 마크 파일명 세팅 성공" ) << END_LOG;
            }
            else {
                START_LOG( clog, L"길드전 방의 길드 마크 파일명 세팅 실패" ) << END_LOG;
            }
        }
        else {
            START_LOG( cerr, L"길드 포인터가 없어서 길드 마크 파일명 세팅 실패" )
                << BUILD_LOG( kUser_.GetName() ) << END_LOG;
        }

        std::wstring guildName = kUser_.GetGuildName();
        if ( guildName != L"" ) {
            if ( SetGuildName( guildName ) ) {
                START_LOG( clog, L"길드전 방의 길드명 세팅 성공 : " << guildName ) << END_LOG;
            }
            else {
                START_LOG( clog, L"길드전 방의 길드명 세팅 실패 : " << guildName ) << END_LOG;
            }
        }

        DWORD bPoint = kUser_.GetGuildBattlePoint();
        if ( SetGuildBattlePoint( kUser_.GetGuildUID(), bPoint ) ) {
            START_LOG( clog, L"길드전 방의 배틀포인트 세팅 성공 : " << guildName )
                << BUILD_LOG( bPoint ) << END_LOG;
        }
        else {
            START_LOG( clog, L"길드전 방의 배틀포인트 세팅 실패 : " << guildName )
                << BUILD_LOG( bPoint ) << END_LOG;
        }
    } // end if ( SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) {
    

    kUser_.SetRoom( this );
    kUser_.StateTransition( KUserFSM::INPUT_JOIN_ROOM );
    LIF( kUser_.GetStateID() == KUserFSM::STATE_ROOM );
    NetError::SetLastNetError( NetError::NET_OK );

    // 061124. kkurrung.  어뷰저
    if ( kUser_.m_pkChannel->IsPvPChannel() ) {
        AddAbuseData( kUser_ );
    }

    ClearChatEvent();
    return true;
}

bool KRoom::Leave( KUser& kUser_, int nReasonWhy_ )
{
    SET_ERROR( NET_OK );

    LIF( KSobManager<KUserPtr>::Delete( kUser_.GetName() ) );
    const KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( kUser_.GetName() );
    // 방장의 GC_CLUB 정보를 확인하기 위해서 정보를 받아온다.
    bool bHostHasGCClub = false;
    {
        const KInDoorUserInfo* pkInDoorUserInfoTemp = FindInDoorUserInfo( GetHostPtr()->GetName() );
        if( pkInDoorUserInfoTemp ) {
            if ( pkInDoorUserInfoTemp->m_kPremiumInfo.CheckType(KPremiumInfo::TYPE_GCBLOG) || pkInDoorUserInfoTemp->m_kPremiumInfo.CheckType(KPremiumInfo::TYPE_GCCLUB) ) {
                bHostHasGCClub = true;
            }
        }
    }
    

    if( pkInDoorUserInfo && pkInDoorUserInfo->m_bIsObserver )
    {
        m_usMaxUsers--;
        if ( SiKGSSimLayer()->CheckTypeFlag( ST_ENTER_LIMIT )  ) {
            CloseRoom();
        }
        if( m_nObserverCnt > 0 ) {
            m_nObserverCnt -= 1;
        }
    }

    RemoveInDoorUserInfo( kUser_ );

    // 050712. 나가는 유저가 host 대상자 였다면 목록에서 삭제해줘야 한다.
    std::vector<KUserPtr>::iterator vit;
    for ( vit = m_vecHostOrder.begin() ; vit != m_vecHostOrder.end() ; ++vit ) {

        if ( (*vit)->GetUID() == kUser_.GetUID() ) {
            m_vecHostOrder.erase( vit );
            break;
        }
    }

    kUser_.SetRoom( NULL );
    kUser_.StateTransition( KUserFSM::INPUT_TO_CHANNEL );   // join, room, playing 상태 모두 channel로 전이된다.

    // 061124. kkurrung. 어뷰저
    if ( kUser_.GetChannel() != NULL ) {
        if( kUser_.GetChannel()->IsPvPChannel() ) {
            RemoveAbuseData( kUser_ );
        }
    }

    RefreshGuildArray(); // 길드 정리

    KLocker lock( m_csSob );
    if( GetCount() == 0 )                      // 빈방 - CLOSE 시킨다.
    {
        LIF( CloseRoom() );
    }
    else    // 아직 방에 누군가가 남아있는 경우. 방장을 새로 정하는 등의 작업을 해야한다.
    {
        if( GetCount() < m_usMaxUsers )
        {
            StateTransition( KRoomFSM::INPUT_WAIT );
        }

        // 050114. 방장의 포인터를 유지하도록 수정. 방장 포인터 체크.
        if( m_spHostUser.get() == &kUser_ || m_spPlayingHostUser.get() == &kUser_ )           // 만약 방장이 빠져나가는 경우라면
        {
            if ( m_spHostUser.get() == &kUser_ )
                InitBanishCount();                  // 090701 방장의 강퇴 Count를 초기화

            LIF( SetNextHost( kUser_ ) );
            SetRoomHostPing( kUser_.m_kPingInfo.GetPing() ); // 바뀌는 방장의 핑을 방 핑으로 설정하기
            SetRoomHostNationCode( kUser_.GetCountryCode() );  // 바뀌는 방장의 국가정보 설정.
        }

        // 유저에게 2,1,0 의 Count정보를 보내주기 위해서 미리 감소시킨다.
        if( nReasonWhy_ == LEAVE_BANNED_HOST_DECISION ) DecreaseBanishCount();
        // 해당 방의 방장이 GC_CLUB을 가지고 있으면 감소시킨 정보를 초기화 시킨다.
        if( bHostHasGCClub ) InitBanishCount();

        // 050707. 유저가 방을 빠져나가는 경우 남은 유저들에게 이를 알려준다.
        KEVENT_INFORM_USER_LEAVE_ROOM_NOT kPacket;
        kPacket.m_strName       = kUser_.GetName();
        kPacket.m_nReasonWhy    = nReasonWhy_;
        kPacket.m_dwUserUID     = kUser_.GetUID();
        kPacket.m_dwBanishCount = GetBanishCount(); // 방장의 강퇴 Count도 함께 보내준다.

        //방을 나가는 대상은 이미 SOB에서 빠져 있다.
        BroadPacket( KRoom::BROAD_TO_ALL, kUser_.GetName(), KUserEvent::EVENT_INFORM_USER_LEAVE_ROOM_NOT, kPacket, true );
        kUser_.ClearPressInfo();

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) {

            KRoomInfo kRoomInfo;
            GetRoomInfo( kRoomInfo );
            BroadPacket( KRoom::BROAD_TO_ALL, kUser_.GetName(), KUserEvent::EVENT_GUILD_ROOM_INFO_CHANGE_NOT, kRoomInfo, true );
        }

        // 090701 tgkwon
        // 유저를 강퇴하려는 이유가 방장 강퇴일 경우
        if( nReasonWhy_ == LEAVE_BANNED_HOST_DECISION )
        {
            int nCount = GetBanishCount();

            if( nCount < 0 )
            {
                SET_ERROR( ERR_ROOM_60 );
                return false;
            }
        }

        // 사람이 나갔는데.. 게임 중이었고, 데쓰매치이고, 게임중인 사람이 없고, 룸에 사람이 있을때..
        if( m_iGameMode == GC_GMC_DEATHMATCH || m_iGameMode == GC_GMC_FATAL_DEATHMATCH )
        {
            if( GetStateID() == KRoomFSM::STATE_PLAYING && GetPlayingUserCount() <= 0 )
            {
                // 상태 전이..
                StateTransition( KRoomFSM::INPUT_END_GAME ); // 게임 결과 상태로 전이.

                if( GetCount() < m_usMaxUsers ) // 풀방인지 체크.
                    StateTransition( KRoomFSM::INPUT_WAIT );
                else
                    StateTransition( KRoomFSM::INPUT_FULL );
            }
        }

    }

     // 080204. kkurrun. 자유의지가 아니라 강제적으로 종료되는 유저라면, 자신이 강제로 방에서 나가게 되었음을 알려줘야 한다. 혼자 있던 방이라도.
    if( nReasonWhy_ == LEAVE_BANNED_EXCEED_MAXUSER ||
        nReasonWhy_ == LEAVE_BANNED_HOST_DECISION ||
        nReasonWhy_ == LEAVE_BANNED_ON_PLAYING ||
        nReasonWhy_ == LEAVE_LOADING_COMPLETE_FAILED )
    {
        KEVENT_INFORM_USER_LEAVE_ROOM_NOT kPacket;
        kPacket.m_strName       = kUser_.GetName();
        kPacket.m_nReasonWhy    = nReasonWhy_;
        kPacket.m_dwUserUID     = kUser_.GetUID();

        kUser_.SendPacket( KUserEvent::EVENT_INFORM_USER_LEAVE_ROOM_NOT, kPacket );

        switch( nReasonWhy_ )
        {
        case LEAVE_BANNED_EXCEED_MAXUSER:   // ROOM 상태에 날아온다
            SiKFailRate()->IncreaseCount( KFailRate::NOT_ENOUGH_SLOT );
            break;
        case LEAVE_BANNED_ON_PLAYING:   // PLAYING 상태에 날아온다
            SiKFailRate()->IncreaseCount( KFailRate::ALREADY_START );
            break;

        }//end switch
    }

    ClearChatEvent();

    return ( NetError::GetLastNetError() == NetError::NET_OK );
}

void KRoom::SetRoomInfo( IN KRoomInfo& kInfo_, IN char cType_, IN int& nModeInfo_ )//방생성시 방정보 셋팅할때이다.
{
    //m_usRoomID      = kInfo_.m_usRoomID;
    m_strName                   = kInfo_.m_strRoomName;
    m_bPublic                   = kInfo_.m_bPublic;
    m_strPasswd                 = kInfo_.m_strRoomPasswd;
    m_usMaxUsers                = kInfo_.m_usMaxUsers;
    m_cGrade                    = kInfo_.m_cGrade;
    m_cGameCategory             = kInfo_.m_cGameCategory;
    m_iGameMode                 = kInfo_.m_iGameMode;
    m_iSubGameMode              = kInfo_.m_iSubGameMode;
    m_bRandomableMap            = kInfo_.m_bRandomableMap;
    m_iMapID                    = kInfo_.m_iMapID;
    m_iP2PVersion               = kInfo_.m_iP2PVersion;
    m_cRoutingMethod            = kInfo_.m_cRoutingMethod;
    m_bDeathMatchIntrudable     = kInfo_.m_bDeathMatchIntrudable;
    m_bDeathMatchBalancing      = kInfo_.m_bDeathMatchBalancing;

    m_dwRoomOptions				= kInfo_.m_kOptions.m_dwEnabledOptions;
	m_dwRoomAllowerdChars		= kInfo_.m_kOptions.m_dwDisabledChars;
	m_dwRoomAllowedRankings		= kInfo_.m_kOptions.m_dwAllowedRankings;

    m_nDeathMatchTime           = DEATH_MIN_TIME;
    m_nDeathKillCount           = DEATH_DEFAULT_KILL;
    ClearDeathScore();

    if( cType_ == KChannel::CT_INDIGO ) {
        m_usMaxUsers = kInfo_.m_usMaxUsers = DEFAULT_INDIGO_USER;
        memset( m_abSlotOpen,false, sizeof( m_abSlotOpen) );
        m_abSlotOpen[0] = true;
        m_abSlotOpen[3] = true;
        m_kDefaultModeInfo.m_usDefaultUser = DEFAULT_INDIGO_USER;
        m_kDefaultModeInfo.m_bModeChangeEnable = false;
    }
    else if( cType_ == KChannel::CT_DUNGEON ) {
        SetDungeonDefaultInfo( m_iGameMode );
    }
    else {
        memset( m_abSlotOpen, true, sizeof( m_abSlotOpen) );
        m_kDefaultModeInfo.m_usDefaultUser = DEFAULT_PVP_USER;
        m_kDefaultModeInfo.m_bModeChangeEnable = true;
    }

    m_nMonsterID        = MON_INVALID;
    m_nMonsterCount     = 0;
    m_nDifficulty       = kInfo_.m_nDifficulty;

    m_nObserverCnt      = 0; //옵저버 수 초기화.
    m_wstrCCode         = kInfo_.m_wstrCCode; // 방장 국가 정보 넣어주기.
    m_nModeInfo         = nModeInfo_; // 모드정보 
}

void KRoom::ChangeRoomExtraOptions(IN KUser* pkUser, IN OUT KRoomOptions& kInfo)
{
    if (!m_spHostUser)
    {
        SET_ERROR(ERR_ROOM_39);
        return;
    }

    if (m_spHostUser.get() != pkUser && !pkUser->IsAdmin() && m_spPlayingHostUser.get() != pkUser)
    {
        SET_ERROR(ERR_ROOM_40);
        return;
    }

    //New options
    m_dwRoomOptions = kInfo.m_dwEnabledOptions;
    m_dwRoomAllowerdChars = kInfo.m_dwDisabledChars;
    m_dwRoomAllowedRankings = kInfo.m_dwAllowedRankings;

    SET_ERROR(NET_OK);
}

void KRoom::ChangeRoomInfo( IN KUser* pkUser_, IN OUT KChangeRoomInfo& kInfo_ )
{ 
    // 방의 상태가 waiting, full일때만 허용하시오 여기서 체크
    if( !m_spHostUser )
    {
        SET_ERROR( ERR_ROOM_39 );
        return ;
    }

    if( m_spHostUser.get() != pkUser_ && !pkUser_->IsAdmin() && m_spPlayingHostUser.get() != pkUser_)
    {
        SET_ERROR( ERR_ROOM_40 );
        return;
    }

    switch( kInfo_.m_nOK )
    {
    case KChangeRoomInfo::CR_GAME_INFO:
        {
            if( !pkUser_->CheckGameMode( kInfo_.m_iGameMode ) )
            {

                SET_ERROR( ERR_ROOM_41 );
                return;
            }

            if( m_cChannelType == KChannel::CT_DUNGEON )
            {
                if( !PlayableDungeon( kInfo_.m_iGameMode, kInfo_.m_nDifficulty ) )
                {
                    SET_ERROR( ERR_ROOM_47 );
                    return;
                }
                // 헬모드 이면 방정보를 바꿀수 없다.
                if( (m_nDifficulty >= GC_GMD_CHAMPION_BEGIN && kInfo_.m_nDifficulty < GC_GMD_CHAMPION_BEGIN) ||
                    (m_nDifficulty < GC_GMD_BREAK_BEGIN && kInfo_.m_nDifficulty >= GC_GMD_BREAK_BEGIN) )
                {
                    SET_ERROR( ERR_ROOM_51 );
                    return;
                }

                // 영웅던전 게임모드로 변경 불가
                if ( ( SiKHeroDungeonManager()->IsHeroDungeon( kInfo_.m_iGameMode ) || SiKHeroDungeonManager()->IsHeroDungeon( m_iGameMode ) ) &&
                    m_iGameMode != kInfo_.m_iGameMode ) {
                    SET_ERROR( ERR_ROOM_67 );
                    return;
                }

                // 게임모드 변경 가능한 모드인지 확인.
                if( !m_kDefaultModeInfo.m_bModeChangeEnable ) {
                    SET_ERROR( ERR_ROOM_70 );
                    return;
                }
            }

            // 믿음이 중요하죠
            m_cGameCategory     = kInfo_.m_cGameCategory;
            m_iGameMode         = kInfo_.m_iGameMode;
            m_iSubGameMode      = kInfo_.m_iSubGameMode;
            m_bRandomableMap    = kInfo_.m_bRandomableMap;
            m_iMapID            = kInfo_.m_iMapID;
            m_nDifficulty       = kInfo_.m_nDifficulty;
            m_nMonsterID        = kInfo_.m_nMonsterID;
            m_nMonsterCount     = kInfo_.m_nMonsterCount;
            m_cRoutingMethod    = kInfo_.m_cRoutingMethod;
        }
        break;
    case KChangeRoomInfo::CR_USER_INFO:
        {
            int nOpenCount = 0;
            std::vector<std::pair<char,bool> >::iterator vit;
            for( vit = kInfo_.m_vecChangeSlot.begin() ; vit != kInfo_.m_vecChangeSlot.end() ; ++vit )
            {
                nOpenCount += (vit->second?1:-1);
                //온 값의 유효성 검사
                if( vit->first < 0 || vit->first >= 6 )
                {
                    SET_ERROR( ERR_ROOM_42 );
                    return;
                }
            }

            //방 인원이 옵저버가 있으면 3명/ 없으면 2명은 유지한다.
            if( (m_nObserverCnt == 2 && m_usMaxUsers < 4 && nOpenCount < 0) ||
                (m_nObserverCnt == 1 && m_usMaxUsers < 3 && nOpenCount < 0) ||
                (m_nObserverCnt == 0 && m_usMaxUsers < 2 && nOpenCount < 0) )
            {
                SET_ERROR( ERR_ROOM_43 );
                return;
            }

            // 닫으려는/ 열려는 슬롯에 유저가 있으면 않된다.
            for( vit = kInfo_.m_vecChangeSlot.begin() ; vit != kInfo_.m_vecChangeSlot.end() ; ++vit )
            {
                std::map<std::wstring, KInDoorUserInfo>::iterator mit;
                for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
                {
                    if( mit->second.m_iPlayerIndex >= 0 && mit->second.m_iPlayerIndex == static_cast<int>(vit->first) )
                    {
                        SET_ERROR( ERR_ROOM_44 );
                        return;
                    }
                }
            }

            for( vit = kInfo_.m_vecChangeSlot.begin() ; vit != kInfo_.m_vecChangeSlot.end() ; ++vit )
            {
                m_abSlotOpen[vit->first] = vit->second;
            }
        }

        break;
    default:
        SET_ERROR( ERR_ROOM_45 );
        return;
    };

    kInfo_.m_cGameCategory      = m_cGameCategory;
    kInfo_.m_iGameMode          = m_iGameMode;
    kInfo_.m_iSubGameMode       = m_iSubGameMode;
    kInfo_.m_bRandomableMap     = m_bRandomableMap;
    kInfo_.m_iMapID             = m_iMapID;
    kInfo_.m_nDifficulty        = m_nDifficulty;
    kInfo_.m_nMonsterID         = m_nMonsterID;
    kInfo_.m_nMonsterCount      = m_nMonsterCount;
    kInfo_.m_cRoutingMethod     = m_cRoutingMethod;

    m_usMaxUsers = 0;
    ::memcpy( kInfo_.m_abSlotOpen, m_abSlotOpen, sizeof( kInfo_.m_abSlotOpen ) );
    for( int i = 0 ; i < MAX_NUM_USER_SLOT ; ++i )
    {
        if( m_abSlotOpen[i] == true )
        {
            m_usMaxUsers++;
        }
    }

    m_usMaxUsers += GetCount();
    kInfo_.m_usMaxUsers = m_usMaxUsers;
    kInfo_.m_pairBattlePoint = m_pairBattlePoint;

    // 입장 제한수가 변경된 경우 상태변경을 해야한다.
    if( GetStateID() == KRoomFSM::STATE_WAITING && GetCount() >= m_usMaxUsers )
        StateTransition( KRoomFSM::INPUT_FULL );
    else if( GetStateID() == KRoomFSM::STATE_FULL && GetCount() < m_usMaxUsers )
        StateTransition( KRoomFSM::INPUT_WAIT );
    SET_ERROR( NET_OK );
}

void KRoom::GetChangeRoomInfo( OUT KChangeRoomInfo& kInfo_ )
{
    kInfo_.m_cGameCategory      = m_cGameCategory;
    kInfo_.m_iGameMode          = m_iGameMode;
    kInfo_.m_iSubGameMode       = m_iSubGameMode;
    kInfo_.m_bRandomableMap     = m_bRandomableMap;
    kInfo_.m_iMapID             = m_iMapID;
    kInfo_.m_nDifficulty        = m_nDifficulty;
    kInfo_.m_nMonsterID         = m_nMonsterID;
    kInfo_.m_nMonsterCount      = m_nMonsterCount;
    kInfo_.m_usMaxUsers         = m_usMaxUsers;
    kInfo_.m_cRoutingMethod     = m_cRoutingMethod;
    kInfo_.m_nMonsterCrusaderMonLv = m_nMonsterCrusaderMonLv;
    ::memcpy( kInfo_.m_abSlotOpen, m_abSlotOpen, sizeof( kInfo_.m_abSlotOpen ) );

    kInfo_.m_pairGuildMarkName = m_pairGuildMarkName;
    kInfo_.m_pairGuildName = m_pairGuildName;
    kInfo_.m_pairBattlePoint = m_pairBattlePoint;
}

void KRoom::GetRoomInfo( OUT KRoomInfo& kInfo_ )
{
    kInfo_.m_usRoomID               = m_usRoomID;
    kInfo_.m_strRoomName            = m_strName;
    kInfo_.m_bPublic                = m_bPublic;
    kInfo_.m_strRoomPasswd          = m_strPasswd;
    kInfo_.m_usUsers                = GetCount();
    kInfo_.m_usMaxUsers             = m_usMaxUsers;
    kInfo_.m_bPlaying                       = GetStateID() == KRoomFSM::STATE_PLAYING || GetStateID() == KRoomFSM::STATE_RESULT;
    if ( m_spHostUser ) {
        kInfo_.m_cGrade                     = m_spHostUser->m_cGrade;
    }
    kInfo_.m_cGameCategory                  = m_cGameCategory;
    kInfo_.m_iP2PVersion                    = m_iP2PVersion;
    kInfo_.m_iGameMode                      = m_iGameMode;
    kInfo_.m_iSubGameMode                   = m_iSubGameMode;
    kInfo_.m_bRandomableMap                 = m_bRandomableMap;
    kInfo_.m_iMapID                         = m_iMapID;
    kInfo_.m_nMonsterID                     = m_nMonsterID;
    kInfo_.m_nMonsterCount                  = m_nMonsterCount;
    kInfo_.m_nDifficulty                    = m_nDifficulty;
    kInfo_.m_dwRelayServerIP                = m_dwRelayServerIP;
    kInfo_.m_usRelayServerPort              = m_usRelayServerPort;
    kInfo_.m_dwTRelayServerIP               = m_dwTRServerIP;
    kInfo_.m_usTRelayServerPort             = m_usTRServerPort;
    kInfo_.m_cRoutingMethod                 = m_cRoutingMethod;
    kInfo_.m_bEnableJoinGame                = JoinGameEnable();
    kInfo_.m_bDeathMatchIntrudable          = m_bDeathMatchIntrudable;
    kInfo_.m_bDeathMatchBalancing           = m_bDeathMatchBalancing;
    kInfo_.m_nDeathMatchTime                = m_nDeathMatchTime;
    kInfo_.m_nDeathKillCount                = std::max<int>( DEATH_MIN_KILL, std::min<int>(DEATH_MAX_KILL,m_nDeathKillCount) );
    kInfo_.m_dwUniqueNumber                 = GetUniqueNumber();
    kInfo_.m_pairGuildMarkName              = m_pairGuildMarkName;
    kInfo_.m_pairGuildName                  = m_pairGuildName;
    kInfo_.m_pairBattlePoint                = m_pairBattlePoint;
    kInfo_.m_wstrCCode                      = m_wstrCCode;
    kInfo_.m_kDefaultModeInfo               = m_kDefaultModeInfo;

    kInfo_.m_kOptions.m_dwEnabledOptions	= m_dwRoomOptions;
	kInfo_.m_kOptions.m_dwDisabledChars		= m_dwRoomAllowerdChars;
	kInfo_.m_kOptions.m_dwAllowedRankings	= m_dwRoomAllowedRankings;

    ::memcpy( kInfo_.m_abSlotOpen, m_abSlotOpen, sizeof( kInfo_.m_abSlotOpen ) );
}

void KRoom::GetSimpleRoomInfo( OUT KSimpleRoomInfo& kSimpleRoomInfo_ )
{
    kSimpleRoomInfo_.m_mapRoomMember.clear();
    kSimpleRoomInfo_.m_usRoomID                     = m_usRoomID ;
    kSimpleRoomInfo_.m_strRoomName                  = m_strName;
    kSimpleRoomInfo_.m_bPublic                      = m_bPublic;
    kSimpleRoomInfo_.m_bGuild                       = false;
    kSimpleRoomInfo_.m_strRoomPasswd                = m_strPasswd;
    kSimpleRoomInfo_.m_usMaxUsers                   = m_usMaxUsers;
    kSimpleRoomInfo_.m_usUsers                      = GetCount();
    kSimpleRoomInfo_.m_bPlaying                     = GetStateID() == KRoomFSM::STATE_PLAYING || GetStateID() == KRoomFSM::STATE_RESULT;
    kSimpleRoomInfo_.m_cGameCategory                = m_cGameCategory;
    kSimpleRoomInfo_.m_nGameMode                    = m_iGameMode;
    kSimpleRoomInfo_.m_iP2PVersion                  = m_iP2PVersion;
    kSimpleRoomInfo_.m_nDifficulty                  = m_nDifficulty;
    kSimpleRoomInfo_.m_bDeathMatchIntrudable        = m_bDeathMatchIntrudable;
    kSimpleRoomInfo_.m_bDeathMatchBalancing         = m_bDeathMatchBalancing;
    if ( m_spHostUser ) {
        kSimpleRoomInfo_.m_cGrade                   = m_spHostUser->m_cGrade;
    }    
    kSimpleRoomInfo_.m_pairGuildMarkName            = m_pairGuildMarkName;
    kSimpleRoomInfo_.m_pairGuildName                = m_pairGuildName;
    kSimpleRoomInfo_.m_pairBattlePoint              = m_pairBattlePoint;
    kSimpleRoomInfo_.m_wstrCCode                    = m_wstrCCode;

    kSimpleRoomInfo_.m_kOptions.m_dwEnabledOptions	= m_dwRoomOptions;
	kSimpleRoomInfo_.m_kOptions.m_dwDisabledChars	= m_dwRoomAllowerdChars;
	kSimpleRoomInfo_.m_kOptions.m_dwAllowedRankings = m_dwRoomAllowedRankings;
    
    const KInDoorUserInfo* pkInDoorUserInfo         = GetHostInDoorUserInfo();
    if ( pkInDoorUserInfo ) {
        kSimpleRoomInfo_.m_kPremiumInfo = pkInDoorUserInfo->m_kPremiumInfo;
        std::map<int,KDungeonUserInfo>::const_iterator mitMode;
        mitMode = pkInDoorUserInfo->m_mapDifficulty.find( m_iGameMode );
        if ( mitMode != pkInDoorUserInfo->m_mapDifficulty.end() ) {
            kSimpleRoomInfo_.m_bClearLastDifficult = mitMode->second.m_kClearData.m_bClear;
        }
    }

    kSimpleRoomInfo_.m_bObserverInRoom = false; // 옵저버 유무 체크
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock( m_csSob );
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        if( mit->second->IsObserver() ) // 옵저버가 있으면
        {
            kSimpleRoomInfo_.m_bObserverInRoom = true;
            continue;
        }

        kSimpleRoomInfo_.m_mapRoomMember[mit->second->GetNickName()] = mit->second->m_cGrade;
    }
}

bool KRoom::StartGame( IN KUser& kUser_ )
{
    m_iStageClearcount = 0;
    ClearDeathScore();
    m_nSendResultEndUser = 0;
    std::vector<DWORD> vecStartUser;
    m_nWinTeam = -1;
    m_vecStageLoadFailUser.clear();

    m_mapResultExp.clear();
    m_mapResultGP.clear();
    m_mapUserFatigueRatio.clear();

    if( !( GetStateID() == KRoomFSM::STATE_WAITING || GetStateID() == KRoomFSM::STATE_FULL ) )
    {
        SET_ERROR( ERR_ROOM_08 );
        return false;
    }

    if( m_spHostUser.get() != &kUser_ && !kUser_.IsAdmin() )
    {
        SET_ERROR( ERR_ROOM_20 );
        return false;
    }

    if( !kUser_.IsAdmin() )
    {
        if( GetCount() <= 1 && !IsSinglePlayable() )
        {
            SET_ERROR( ERR_ROOM_22 );
            return false;
        }

        bool bHeroDungeon = SiKHeroDungeonManager()->IsHeroDungeon( m_iGameMode );
        bool bHeroDungeonDifficultyCheck = SiKHeroDungeonManager()->IsHeroDungeonDifficultyCheck( m_iGameMode );

        if ( IsDifficultyMode( m_iGameMode ) && !kUser_.m_kInventory.FindItemByItemID( SiKGCHelper()->GetDungeonFreePassID() ) && !bHeroDungeon && (bHeroDungeon && bHeroDungeonDifficultyCheck) )
        {
            if( !( m_nDifficulty >= 0 && ( m_nDifficulty < GetMaxDifficulty(m_iGameMode) || m_nDifficulty == GC_GMD_CHAMPION_BEGIN ) ) )
            {
                START_LOG( cwarn, L" mode : " << m_iGameMode << L", Difficult : " << m_nDifficulty ) << END_LOG;
                SET_ERROR( ERR_ROOM_24 ); 
                return false;
            }
            KDungeonUserInfo kInfo;
            if ( false == kUser_.GetDungeonUserInfo( m_iGameMode, kInfo ) ) {
                START_LOG( cwarn, L"kUser_.m_mapDifficulty.end()" )
                    << BUILD_LOG( m_iGameMode )
                    << BUILD_LOG( m_nDifficulty ) << END_LOG;
                SET_ERROR( ERR_ROOM_23 ); 
                return false;
            }

            if( m_nDifficulty > 0 )
            {

                int nPrevDifficulty = GetPrevDifficulty( m_iGameMode, m_nDifficulty );
                if( !( kInfo.m_kModeDifficulty.Get( nPrevDifficulty ) || kInfo.m_kModeDifficulty.Get( m_nDifficulty ) ) )
                {
                    SET_ERROR( ERR_ROOM_23 ); 
                    return false;
                }

                if ( bHeroDungeon && bHeroDungeonDifficultyCheck ) {
                    if ( m_nDifficulty > static_cast<int>( kInfo.m_kClearData.m_shMaxDifficulty ) ) {
                        START_LOG( cwarn, L"플레이 가능한 최대난이도 보다 높은 영던 난이도를 플레이하려고 함." )
                            << BUILD_LOG( m_iGameMode )
                            << BUILD_LOG( m_nDifficulty ) << END_LOG;
                        SET_ERROR( ERR_ROOM_23 ); 
                        return false;
                    }
                }
            }
        }

        if( m_iGameMode == GC_GM_DEATH_TEAM || m_iGameMode == GC_GM_DEATH_SURVIVAL )
        {
            if( !IsDeathMatchReady( vecStartUser ) )
            {
                SET_ERROR( ERR_ROOM_29 );
                return false;
            }
        }
        else
        {
            if( !IsNormalUserAllReady() )
            {
                SET_ERROR( ERR_ROOM_29 );
                return false;
            }
            vecStartUser.clear();
            std::map<std::wstring, KUserPtr>::iterator mit;
            for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
            {
                vecStartUser.push_back( mit->second->GetUID() );
            }
        }
    }
    else
    {
        if( m_cGameCategory == GC_GMC_DEATHMATCH )
        {
            vecStartUser.clear();
            GetReadyUserUID( vecStartUser );
            if( m_spHostUser )
            {
                if( std::find( vecStartUser.begin(), vecStartUser.end(), m_spHostUser->GetUID() ) == vecStartUser.end() )
                    vecStartUser.push_back( m_spHostUser->GetUID() );
            }

            if( std::find( vecStartUser.begin(), vecStartUser.end(), kUser_.GetUID() ) == vecStartUser.end() )
                vecStartUser.push_back( kUser_.GetUID() );
        }
        else
        {
            vecStartUser.clear();
            std::map<std::wstring, KUserPtr>::iterator mit;
            for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
            {
                vecStartUser.push_back( mit->second->GetUID() );
            }
        }
    }

    if( !StartHellModeProcess() )
    {
        SET_ERROR( ERR_ROOM_52 );
        return false;
    }

    if ( false == StartDungeonTicketProcess() ) {
        SET_ERROR( ERR_ROOM_69 );
        return false;
    }

    if (false == StartHeroModeProcess()) {
        return false;
    }

    if( !kUser_.GetChannel() )
    {
        SET_ERROR( ERR_INTERNAL_00 );
        return false;
    }
    if( kUser_.GetChannel()->GetType() == KChannel::CT_AUTOMATCH && !ELOAvgRPCalc() ) { // �ڵ� ��Ī ���� ��� RP ���ϱ�.
        return false;
    }

    if( !CountGameMode( m_iGameMode, m_nDifficulty ) )
    {
        START_LOG( cerr, L"Invalid Mode Count, Mode : " << m_iGameMode << L", " << m_nDifficulty ) << END_LOG;
    }

    GameStartStateTransition( vecStartUser );
    SetPvpLifeCount( vecStartUser );

    m_dwStartGameTime = ::timeGetTime();
    m_tmGameStartTime = CTime::GetCurrentTime();

    StateTransition( KRoomFSM::INPUT_START_GAME );

    SetStartingInfo();
    CheckMonsterCrusaderMonLv( m_iGameMode, m_nTotalLv, m_nStartingUserNum, m_nMonsterCrusaderMonLv );
    m_mapEventList.clear();

    {
        KStatGameStartGapPtr spStat = GET_STAT( KStatGameStartGap, SI_STAT_GAME_START_GAP );
        if( spStat )
        {
            spStat->AddStat( ::GetTickCount() - m_dwGameStartGap );
        }
    }

    InitBanishCount();
    ClearMonDropGP();
    ClearResultExp();
    ClearResultGP();
    ClearLevelUpUser();
    ClearMonsterKillInfo();
    InitMatchFailCount();
    ClearCompletedSubjectTrigger();
    ClearMonsterKillCount();
    ClearRemainVitalityOnStartGame();
	ClearUserUsePosition();
    ClearInfinityDungeonInfoOnStartGame();	

    std::wstring wstrHostCCode = GetRoomHostNationCode();
    
    std::map<std::wstring, KInDoorUserInfo>::iterator mitUser;
    if( m_mapInDoorUserInfo.size() > 1 ) {
        
        bool bRewardEnable = true;
        
        for( mitUser = m_mapInDoorUserInfo.begin(); mitUser != m_mapInDoorUserInfo.end(); ++mitUser ) {
            bool bEnable = CheckNationCode( wstrHostCCode, mitUser->second.m_wstrCCode );
            if( bRewardEnable ) {
                SetNationRewardEnable( bEnable );
                bRewardEnable = bEnable;
            }
        }
    }

    if ( false == m_mapSob.empty() ) {
        typedef std::map< std::wstring, KUserPtr > mapSobType;
        BOOST_FOREACH( mapSobType::value_type& rSob, m_mapSob ) {
            KUserPtr& rUser = rSob.second;
            rUser->SetCharLvStartGameTime();
            int nRatio = rUser->CheckFatigueTime();
            if ( nRatio != -1 && nRatio != 100 ) {
                m_mapUserFatigueRatio.insert( std::make_pair( rUser->GetUID(), nRatio ) );
            }
            int nDeductVitality = 0;
            if ( true == SiKVitalityManager()->IsRun() && true == SiKVitalityManager()->GetDeductVitality( m_iGameMode, m_nDifficulty, nDeductVitality ) ) {
                DWORD dwLoginUID = rUser->GetUID();
                int nCurrentCharacter = static_cast< int >( rUser->m_cCurrentCharacter );
                AddRemainVitalityOnStartGame( dwLoginUID, nCurrentCharacter, rUser->m_kVitalityUserData.GetVitality( nCurrentCharacter ) );

                if ( true == rUser->m_kVitalityUserData.DecreaseVitalityOnPlay( nCurrentCharacter, nDeductVitality ) ) {
                    rUser->SendVitalityInfo( KVitalityManager::EVST_DUNGEON_START );
                }
            }
        }
    }

    SET_ERROR( NET_OK );
    return true;
}

bool KRoom::StartJoinGame( IN KUser& kUser_ )
{
    // User에서 모드 체크는 했음..
    // 방의 상태가 올바르지 못한 경우를 확인한다.
    if( GetStateID() != KRoomFSM::STATE_PLAYING )
    {
        SET_ERROR( ERR_ROOM_54 );
        return false;
    }

    std::vector<DWORD> vecStartUser;
    vecStartUser.push_back( kUser_.GetUID() );
    GameStartStateTransition( vecStartUser );

    return true;
}

bool KRoom::EndGame( IN const KEndGameReq& kReq_ )
{
    if( GetStateID() != KRoomFSM::STATE_PLAYING )
    {
        SET_ERROR( ERR_ROOM_09 );
        return false;
    }

    ClearLeftRoom();

    std::vector<KGameResultIn>::const_iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( vit->m_strLogin );
        if( pkInDoorUserInfo == NULL ) continue;

        pkInDoorUserInfo->m_nState = GC_RUS_WAIT;
    }

    KLocker lock( m_csSob );
    std::map<std::wstring,KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
    {
        if( mit->second->GetStateID() == KUserFSM::STATE_PLAYING )
        {
            mit->second->StateTransition( KUserFSM::INPUT_END_GAME_OK );
            mit->second->ClearPressInfo();
            // 061124. kkurrung. 어뷰저

            if( mit->second->m_pkChannel->IsPvPChannel() )
            {
                UpdateAbuseData( *(mit->second.get()) );
            }
        }

        if( mit->second->GetStateID() != KUserFSM::STATE_ROOM )
        {
            START_LOG( cerr, L"유저 상태 이상. 원래는 STATE_ROOM." )
                << BUILD_LOG( GetName() )
                << BUILD_LOG( mit->second->GetName() )
                << BUILD_LOG( mit->second->GetStateIDString() )
                << END_LOG;
        }
    }

    START_LOG( clog, L"게임 종료. 방 객체 상태 변환" ) << END_LOG;
    StateTransition( KRoomFSM::INPUT_END_GAME );

    DWORD dwDurationTime = 0;
    dwDurationTime = ::timeGetTime() - m_dwStartGameTime;
    m_dwLastGameDuration = dwDurationTime;
    SetEndGamePlayTime( dwDurationTime ); // 클리어 시간 저장.

    // 무한던전 통계용 유저별 플레이 시간 정보 저장 (UserUID, PlayTime)
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ ) {
        m_mapIDPlayTime[ mit->second->GetUID() ] = dwDurationTime;
    }

    m_tmGameEndTime = CTime::GetCurrentTime(); // 던전 종료 시간 저장.

    if ( SiKPreHackingCheckManager()->GetLogShortGame() ) {
        DWORD dwServerThresholdTick = SiKPreHackingCheckManager()->GetThresholdTick();
        if ( m_iGameMode == GC_GM_DOTA ) {
            dwServerThresholdTick = SiKPreHackingCheckManager()->GetDotaThresholdTick();
        }

        // Short Game Check
        if ( m_dwLastGameDuration < dwServerThresholdTick ) {
            std::wstring strHostName;
            if( m_spHostUser ) strHostName = m_spHostUser->GetName();

            START_LOG( cerr, L"Play Time : " << m_dwLastGameDuration / 1000 )
                << BUILD_LOG( m_usRoomID )
                << BUILD_LOGc( m_cGameCategory )
                << BUILD_LOG( m_iGameMode )
                << BUILD_LOG( strHostName )
                << BUILD_LOG( dwServerThresholdTick ) << END_LOG;
        }
    }

    SET_ERROR( NET_OK );
    m_dwResultTick = ::GetTickCount();
    m_dwGameStartGap = ::GetTickCount(); // 게임 끝날때 초기화.
    return true;
}

bool KRoom::LeaveGame( KUser& kUser_ )
{
    if( GetStateID() != KRoomFSM::STATE_PLAYING )
    {
        kUser_.StateTransition( KUserFSM::INPUT_END_GAME_OK );  // 경기중이진 않아도 일단 상태변환 시도.

        SET_ERROR( ERR_ROOM_10 );
        return false;
    }

    KLocker lock( m_csSob );
    std::map<std::wstring,KUserPtr>::iterator mit;
    mit = m_mapSob.find( kUser_.GetName() );
    if( mit == m_mapSob.end() )
    {
        kUser_.StateTransition( KUserFSM::INPUT_END_GAME_OK );  // 방에 없더라도 상태변환 시도.

        SET_ERROR( ERR_ROOM_11 );
        return false;
    }

    if( mit->second->GetStateID() == KUserFSM::STATE_PLAYING )
    {
        mit->second->StateTransition( KUserFSM::INPUT_END_GAME_OK );
    }

    if( GetCount() <= 1 )                      // 혼자 있을경우
    {
        StateTransition( KRoomFSM::INPUT_END_GAME );
    }

    DWORD dwDurationTime = 0;
    dwDurationTime = ::timeGetTime() - m_dwStartGameTime;
    m_dwLastGameDuration = dwDurationTime;
    SetLeaveGamePlayTime( dwDurationTime ); // 게임에서 떠날때까지 플레이 시간 저장

    // 무한던전 통계용 유저별 플레이 시간 정보 저장 (UserUID, PlayTime)
    m_mapIDPlayTime[ kUser_.GetUID() ] = dwDurationTime;

    SET_ERROR( NET_OK );

    return true;
}

bool KRoom::EndResult( KUser& kUser_ ) // 정상적인 방장이 보내줬을때.
{
    // user의 상태는 user에서 확인
    if( GetStateID() != KRoomFSM::STATE_RESULT )
    {
        START_LOG( cwarn, L"요청한 방의 상태가 올바르지 않음" )
            << L"wanted : STATE_RESULT, current : " << GetStateIDString() << END_LOG;
        return false;
    }
    ClearChatEvent();
    // 보낸 녀석이 있냐?
    std::map<std::wstring,KUserPtr>::iterator mit;
    mit = m_mapSob.find( kUser_.GetName() );
    if( mit == m_mapSob.end() )
    {
        START_LOG( cerr, L"요청한 유저가 방에 존재하지 않음 name : " << kUser_.GetName() ) << END_LOG;
        return false;
    }

    ++m_nSendResultEndUser;

    if( GetCount() > static_cast<size_t>(m_nSendResultEndUser) ) // 모든 유저가 EndResult를 호출할때까지 기다린다.
        return true;

    if(  GetCount() >= m_usMaxUsers )
        StateTransition( KRoomFSM::INPUT_FULL );
    else if( GetCount() < m_usMaxUsers )
        StateTransition( KRoomFSM::INPUT_WAIT );

    ClearNationReward();

    return true;
}

void KRoom::EndResult()
{
    // user의 상태는 user에서 확인
    if( GetStateID() != KRoomFSM::STATE_RESULT )
    {
        START_LOG( cerr, L"요청한 방의 상태가 올바르지 않음")
            << L"wanted : STATE_RESULT, current : " << GetStateIDString() << END_LOG;
        return;
    }
    ClearChatEvent();

    if(  GetCount() >= m_usMaxUsers )
        StateTransition( KRoomFSM::INPUT_FULL );
    else if( GetCount() < m_usMaxUsers )
        StateTransition( KRoomFSM::INPUT_WAIT );

    ClearNationReward();

    START_LOG( cwarn, L"강제로 게임 상태를 결과상태에서 방으로 변경" ) << END_LOG;
}


bool KRoom::DropItem( IN int iEndingMode, IN int iReserve_, OUT std::vector<KDropItemDetail>& vecDropItemDetail )
{
    // 이젠 EndStage 체크를 안하기 때문에 무조건 0으로 셋팅.
    iEndingMode = 0;

    vecDropItemDetail.clear();
    std::vector<KDropItemDetail> vecTmpDropDetail;

    if (iReserve_ != 0)
    {
        vecTmpDropDetail.clear();
        ms_kDropSystem.ProcessDrop(m_iGameMode, iEndingMode, m_nDifficulty, iReserve_, vecTmpDropDetail);
        vecDropItemDetail.insert(vecDropItemDetail.end(), vecTmpDropDetail.begin(), vecTmpDropDetail.end());
    }
    vecTmpDropDetail.clear();
    ms_kDropSystem.ProcessDrop(m_iGameMode, iEndingMode, m_nDifficulty, 0, vecTmpDropDetail);
    vecDropItemDetail.insert(vecDropItemDetail.end(), vecTmpDropDetail.begin(), vecTmpDropDetail.end());

    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::iterator itUser;
    for( itUser = m_mapSob.begin(); itUser != m_mapSob.end(); itUser++ )
    {
        if( itUser->second->GetAuthLevel() == KUser::AL_ADMIN )
        {
            ms_kDropSystem.ProcessAdminEvent( vecDropItemDetail );
            break;
        }
    }

    return !vecDropItemDetail.empty();
}

bool KRoom::GetDropMode( IN int iEndingMode_, IN int iReserve_, OUT KDropMode& kMode_ )
{
    iEndingMode_ = 0;

    return ms_kDropSystem.GetMode( m_iGameMode, iEndingMode_, m_nDifficulty, iReserve_, kMode_ );
}

void KRoom::ChangeHostAddress( KUser* pkUser_, const std::vector<DWORD>& vecHostIP_, const std::vector<USHORT>& vecHostPort_ )
{
    // 방장 이행 작업을 서버가 하게 되면서 포인터가 이미 셋팅되었다. 만약 다르다면 똑같게 맞춰준다.
    if( m_spHostUser.get() != pkUser_ )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( m_spHostUser->GetName() )
            << BUILD_LOG( pkUser_->GetName() )
            << END_LOG;

        m_spHostUser = std::static_pointer_cast<KUser>( pkUser_->shared_from_this() );
    }

}

bool KRoom::CountGameMode( unsigned int uiMode_, int nDifficulty_ )
{
    if( nDifficulty_ < 0 )
        return false;

    if( uiMode_ >= NUM_GC_GAME_MODE )
    {
        START_LOG( cwarn, L"게임 모드가 이상합니다." )
            << BUILD_LOG( uiMode_ )
            << BUILD_LOG( nDifficulty_ )
            << BUILD_LOG( NUM_GC_GAME_MODE )
            << BUILD_LOG( m_strName )   // 방의 이름을 출력하기 위해, 함수가 static이 아니다.
            << END_LOG;
        return false;
    }

    int nMaxDifficulty = GetMaxDifficulty( uiMode_ );

    if( !IsDifficultyMode( uiMode_ ) && nDifficulty_!= 0 ) //난이도 있는 모드가 아닌데 난이도를 기입했다.
         return false;

    if (IsDifficultyMode(uiMode_) && nDifficulty_ >= nMaxDifficulty && nDifficulty_ != GC_GMD_CHAMPION_BEGIN)
        return false;

    if (IsDifficultyMode(uiMode_) && nDifficulty_ >= nMaxDifficulty && nDifficulty_ != GC_GMD_BREAK_BEGIN)
        return false;

    if ( nDifficulty_ != GC_GMD_CHAMPION_BEGIN || nDifficulty_ != GC_GMD_BREAK_BEGIN ) {
        if ( KRoom::ms_avecModeCount[uiMode_].size() > static_cast<DWORD>(nDifficulty_) ) {
            KRoom::ms_avecModeCount[uiMode_][nDifficulty_] ++;
        }
    }
    else {
        if ( KRoom::ms_avecModeCount[uiMode_].size() == nMaxDifficulty ) {
            KRoom::ms_avecModeCount[uiMode_][nMaxDifficulty-1] ++;
        }
    }

    return true;
}

bool KRoom::SetNextHost( IN const KUser& kPrevHost_ )
{
    if( GetCount() == 0 )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPrevHost_.GetName() )
            << END_LOG;
        SET_ERROR( ERR_ROOM_12 );
        return false;
    }

    KLocker lock( m_csSob );

    // 함수 호출 시점에 이미 이전 host를 삭제하고 난 뒤 이지만, 확실히 하기 위해 한번 더 검색한다.
    KUserPtr m_spCandidateHost;
    std::vector<KUserPtr>::iterator vit;
    for( vit = m_vecHostOrder.begin() ; vit != m_vecHostOrder.end() ; ++vit )
    {
        std::map<std::wstring, KUserPtr>::iterator mit;
        mit = m_mapSob.find( (*vit)->GetName() );
        if( mit == m_mapSob.end() )
            continue;

        if( GetStateID() == KRoomFSM::STATE_PLAYING || GetStateID() == KRoomFSM::STATE_RESULT )
        {
            if( mit->second->GetStateID() != KUserFSM::STATE_PLAYING )
                continue;   
        }

        m_spCandidateHost = mit->second;
        break;
    }


    if( m_spCandidateHost ) {

        // 미리 패킷 정보를 셋팅 해 둔다.
        KEVENT_HOST_MIGRATED_NOT kPacket;
        kPacket.m_dwNewHostUID = m_spCandidateHost->GetUID();
        kPacket.m_bLeaveHost = true;

        if ( m_spPlayingHostUser ) { // 게임중
            if ( m_spHostUser == m_spPlayingHostUser ) { // 게임 방장과 룸 방장이 동일 할 경우

                m_spHostUser = m_spCandidateHost;
                m_spPlayingHostUser = m_spCandidateHost;

                BroadPacket( BROAD_TO_ALL, kPrevHost_.GetName(), KUserEvent::EVENT_HOST_MIGRATED_NOT, kPacket );

            }
            else if ( m_spHostUser != m_spPlayingHostUser && m_spPlayingHostUser->GetUID() == kPrevHost_.GetUID() ) { // 룸 방장과 게임 방장이 다르고 게임 방장이 나갈 경우
                m_spPlayingHostUser = m_spCandidateHost;

                BroadPacket( BROAD_TO_ALL, kPrevHost_.GetName(), KUserEvent::EVENT_HOST_MIGRATED_NOT, kPacket );
            }
            else if ( m_spHostUser != m_spPlayingHostUser && m_spHostUser->GetUID() == kPrevHost_.GetUID() ) { // 룸 방장과 게임 방장이 다른 상황에서 룸 방장이 나갈 경우
                m_spHostUser = m_spCandidateHost; // 게임 중에는 룸 방장이 바뀌는 것을 알려주면 안된다.
                // 게임 종료 시에 rollback migration 에서 host migration 패킷을 전달 해준다.
            }
        }
        else { // 게임 중이 아닌경우 룸 방장이 나간 상황
            if ( m_spHostUser && m_spHostUser->GetUID() == kPrevHost_.GetUID() ) {
                m_spHostUser = m_spCandidateHost;
                BroadPacket( BROAD_TO_ALL, kPrevHost_.GetName(), KUserEvent::EVENT_HOST_MIGRATED_NOT, kPacket );
            }
        }

        // 방장 정보 셋팅..
        KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( m_spHostUser->GetName() );
        if( pkInDoorUserInfo!= NULL )
        {
            if( pkInDoorUserInfo->m_nState == GC_RUS_READY )
                pkInDoorUserInfo->m_nState = GC_RUS_WAIT;
            pkInDoorUserInfo->m_bIsHost = true;
        }

    }
    else // 다음 방장을 정할 수 없다.   접속중인 모든 유저들을 방에서 쫓아내고, 방을 닫아야 한다.
    {
        LIF( CloseRoom() );
        START_LOG( cwarn, L"방장 후보가 없어서 방을 없앱니다.. Size : " << GetCount() ) << END_LOG;
    }


    SET_ERROR( NET_OK );
    return true;
}

bool KRoom::BanishOther( IN KUser& kRequestUser_, IN const std::wstring& strTargetName_, IN int nReasonWhy_, OUT bool& bIsBanHost_ )
{
    // 050715. 자기 자신을 강퇴하는 경우가 발생했다. 작업 전 이름을 확인한다.
    if( strTargetName_ == kRequestUser_.GetName() )
    {
        START_LOG( cerr, L"자기 자신을 스스로 강퇴할 수 없음. 방이름 : " << m_strName )
            << BUILD_LOG( kRequestUser_.GetName() )
            << BUILD_LOG( strTargetName_ )
            << BUILD_LOG( nReasonWhy_ )
            << END_LOG;
        SET_ERROR( ERR_ROOM_18 );
        return false;
    }

    KLocker lock( m_csSob );

    if( m_mapSob.find( kRequestUser_.GetName() ) == m_mapSob.end() )
    {
        SET_ERROR( ERR_ROOM_14 );
        return false;
    }

    std::map<std::wstring,KUserPtr>::iterator mit;
    mit = m_mapSob.find( strTargetName_ );
    if( mit == m_mapSob.end() )
    {
        SET_ERROR( ERR_ROOM_15 );
        return true;    // 050716. 통신 delay상 강퇴 대상자가 이미 방에서 나갔을 수도 있다. 
    }

    if( (nReasonWhy_ == LEAVE_BANNED_EXCEED_MAXUSER ||  // 만약 유저를 퇴장하려는 이유가 방장의 강퇴일 경우
         nReasonWhy_ == LEAVE_BANNED_ON_PLAYING ||
         nReasonWhy_ == LEAVE_BANNED_HOST_DECISION) &&
         ( m_spHostUser.get() != &kRequestUser_ && !kRequestUser_.IsAdmin() ) ) // 요청자가 방장이나 운영자가 아니라면 강퇴할 수 없다.
    {
        SET_ERROR( ERR_ROOM_16 );
        return false;
    }

    if( IsBanHost( strTargetName_ ) == true )
    {
        bIsBanHost_ = true;
    }

    KUserPtr spTmp = mit->second;
    bool bRet = Leave( *(mit->second), nReasonWhy_ );
    return bRet;
}

bool KRoom::IsBanHost( IN const std::wstring& strTargetName_ )
{
    std::map<std::wstring,KUserPtr>::iterator mit;
    mit = m_mapSob.find( strTargetName_ );

    if( mit == m_mapSob.end() )
    {
        SET_ERROR( ERR_ROOM_15 );
        return false;    // 050716. 통신 delay상 강퇴 대상자가 이미 방에서 나갔을 수도 있다. 
    }

    if( m_spHostUser == mit->second ) // m_spHostUser == mit->second 이거랑 다른가, m_spHostUser.get() == &(*(mit->second))
    {
        START_LOG( cerr, L"대회서버, 옵저버가 방장을 강퇴" )
            << BUILD_LOG( strTargetName_ )
            << END_LOG;
        return true;
    }
    return false;
}

bool KRoom::CloseRoom()
{
    KLocker lock( m_csSob );

    ClearDropInfo();
    ClearChampInfo();
    ClearDeathScore();

    // 채팅 이벤트 제거
    ResetChatEvent();
    ClearLeftRoom();

    m_nDeathMatchTime = 0;
    m_nDeathKillCount = 0;


    if( !m_mapSob.empty() ) // 방에 남아있는 유저가 있다면 모두 퇴장시킨다. ( HostMigration 실패시 접속중인 유저 쫓아낼 때)
    {
        START_LOG( cwarn, L"폐쇄하려는 방에 유저들이 남아있음. 방이름 : " << m_strName )
            << BUILD_LOG( GetCount() )
            << BUILD_LOG( GetStateIDString() ) << END_LOG;

        KEVENT_INFORM_USER_LEAVE_ROOM_NOT kPacket;
        kPacket.m_nReasonWhy       = LEAVE_HOST_MIGRATION_FAILED;

        std::map<std::wstring,KUserPtr>::iterator mit;
        for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
        {
            kPacket.m_strName   = mit->first;
            mit->second->SendPacket( KUserEvent::EVENT_INFORM_USER_LEAVE_ROOM_NOT, kPacket );
            mit->second->SetRoom( NULL );
            mit->second->StateTransition( KUserFSM::INPUT_TO_CHANNEL );   // join, room, playing 상태 모두 channel로 전이된다.
            if( mit->second->IsObserver() && mit->second->GetChannel()->GetType() == KChannel::CT_INDIGO ) //용사의 섬 옵저버 이면..
                mit->second->ForceTranslateState( KUserFSM::STATE_CHANNELLOBBY );
        }

        m_mapSob.clear();
    }

    StateTransition( KRoomFSM::INPUT_CLOSE );

    m_nObserverCnt = 0;
    m_spHostUser.reset();
    ClearPlayingHost();

    m_vecHostOrder.clear();
    m_cGameCategory = GC_GMC_INVALID;
    m_iGameMode     = GC_GM_INVALID;        // 게임 모드 초기화.
    m_mapInDoorUserInfo.clear();
    m_nMonsterID    = MON_INVALID;
    m_nMonsterCount = 0;

    ::memset( m_abSlotOpen,true, sizeof( m_abSlotOpen ) );

    m_naGuildID[0] = m_naGuildID[1] = 0;

    // 061212. kkurrung. 어뷰즈데이터 초기화.
    m_mapAbuseData.clear();
    m_vecStageLoadFailUser.clear();

    m_pairGuildMarkName.first.clear();
    m_pairGuildMarkName.second.clear();
    m_mapUserFatigueRatio.clear();
    InitMatchFailCount();
    SetStageLoadSwitch(false); // 초기화 추가.
    ClearRemainVitalityOnStartGame();
    ClearInfinityDungeonInfoOnStartGame();

    if( GetStateID() == KRoomFSM::STATE_CLOSE )
    {
        SET_ERROR( NET_OK );
        return true;
    }
    else
    {
        SET_ERROR( ERR_UNKNOWN );
        return false;
    }
}

const KInDoorUserInfo* KRoom::GetHostInDoorUserInfo()
{
    _JIF( m_spHostUser, return NULL );
    return FindInDoorUserInfo( m_spHostUser->GetName() );
}

KInDoorUserInfo* KRoom::FindInDoorUserInfo( IN const std::wstring& strName_ )
{
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    mit = m_mapInDoorUserInfo.find( strName_ );

    if( mit == m_mapInDoorUserInfo.end() )
    {
        return NULL;
    }

    return &mit->second;
}

KInDoorUserInfo* KRoom::FindInDoorUserInfo( IN const DWORD dwUID_ )
{
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        if( mit->second.m_dwUserUID == dwUID_ )
            return &mit->second;
    }

    return NULL;
}

bool KRoom::FindInDoorUserInfo( IN const std::wstring& strName_, OUT KInDoorUserInfo& kInDoorUserInfo_ )
{
    std::map<std::wstring, KInDoorUserInfo>::const_iterator mit;
    mit = m_mapInDoorUserInfo.find( strName_ );

    if( mit == m_mapInDoorUserInfo.end() )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( strName_ )
            << BUILD_LOG( m_mapInDoorUserInfo.size() ) << END_LOG;

        return false;
    }

    kInDoorUserInfo_ = mit->second;
    return true;
}

bool KRoom::FindInDoorUserInfo( IN const DWORD dwUID_, OUT KInDoorUserInfo& kInDoorUserInfo_ )
{
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        if( mit->second.m_dwUserUID == dwUID_ )
        {
            kInDoorUserInfo_ = mit->second;
            return true;
        }
    }

    return false;
}

bool KRoom::Send_ChatMsg( const KChatData& kNot_ )
{
    bool bCompress = false;
    if( kNot_.m_strChatMsg.size() > 40 && GetCount() > 3 )
        bCompress = true;

    KSerBuffer kbuff;
    kbuff.Clear();
    SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_CHAT_NOT, kNot_, kbuff, 100 );

    std::map<std::wstring, KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        if( mit->second->GetStateID() != KUserFSM::STATE_ROOM ) continue;
        mit->second->SendPacket( kbuff, L"KUserEvent::EVENT_CHAT_NOT" );
    }
    return true;
}

void KRoom::GetChangeRoomUserInfo( IN const KInDoorUserInfo& kInDoorUserInfo_, OUT KChangeRoomUserInfo& kChangeRoomUserInfo_ )
{
    ::memcpy( kChangeRoomUserInfo_.m_acTagModeInfo, kInDoorUserInfo_.m_acTagModeInfo,  sizeof( kChangeRoomUserInfo_.m_acTagModeInfo ) );

    kChangeRoomUserInfo_.m_cCharIndex       = kInDoorUserInfo_.m_cCharIndex;
    kChangeRoomUserInfo_.m_iTeam            = kInDoorUserInfo_.m_iTeam;
    kChangeRoomUserInfo_.m_nUserSlot        = kInDoorUserInfo_.m_iPlayerIndex;
    kChangeRoomUserInfo_.m_strLogin         = kInDoorUserInfo_.m_strLogin;
    kChangeRoomUserInfo_.m_nState           = kInDoorUserInfo_.m_nState;
    kChangeRoomUserInfo_.m_dwUserUID        = kInDoorUserInfo_.m_dwUserUID;
}

bool KRoom::ProcChangeRoomUserInfo(IN OUT KInDoorUserInfo& kInDoorUserInfo_, IN OUT KChangeRoomUserInfo& kChangeRoomUserInfo_)
{
    KUserPtr spUser = GetByName(kChangeRoomUserInfo_.m_strLogin);
    const KCharacterInfo* pkCharacter = spUser->GetCharacterInfo(kChangeRoomUserInfo_.m_cCharIndex);

    if (!spUser)
    {
        START_LOG(cerr, L"ChangeRoomError Name : " << kChangeRoomUserInfo_.m_strLogin) << END_LOG;
        return false;
    }

    std::wstringstream stm;
    stm << TOSTRINGWc(kChangeRoomUserInfo_.m_ucChangeType);
    switch (kChangeRoomUserInfo_.m_ucChangeType)
    {
    case KChangeRoomUserInfo::RUI_TEAM:
    {
        if (kChangeRoomUserInfo_.m_iTeam != 0 && kChangeRoomUserInfo_.m_iTeam != 1)
        {
            START_LOG(clog, L"ChangeRoomInfo... Name : " << kChangeRoomUserInfo_.m_strLogin)
                << BUILD_LOG(kChangeRoomUserInfo_.m_iTeam)
                << BUILD_LOG(kInDoorUserInfo_.m_iTeam) << END_LOG;
            return false;
        }

        if (kChangeRoomUserInfo_.m_iTeam == kInDoorUserInfo_.m_iTeam)
        {
            START_LOG(clog, L"ChangeRoomInfo... Name : " << kChangeRoomUserInfo_.m_strLogin)
                << BUILD_LOG(kInDoorUserInfo_.m_iTeam) << END_LOG;
            return false;
        }

        int nSlotNum = GetEmptySlot(kChangeRoomUserInfo_.m_iTeam, true);
        if (nSlotNum < 0)
        {
            START_LOG(clog, L"ChangeRoomInfo... Name : " << kChangeRoomUserInfo_.m_strLogin) << END_LOG;
            return false;
        }

        m_abSlotOpen[kInDoorUserInfo_.m_iPlayerIndex] = true;
        m_abSlotOpen[nSlotNum] = false;
        kInDoorUserInfo_.m_iPlayerIndex = nSlotNum;
        kInDoorUserInfo_.m_iTeam = CalcTeam(kInDoorUserInfo_.m_iPlayerIndex);
        stm << TOSTRINGW(kChangeRoomUserInfo_.m_iTeam);
    }
    break;

    case KChangeRoomUserInfo::RUI_USER_SLOT:
    {
        if (spUser->IsObserver())
        {
            START_LOG(cerr, L" ChangeRoomError")
                << BUILD_LOG(kChangeRoomUserInfo_.m_strLogin)
                << BUILD_LOG(kChangeRoomUserInfo_.m_nUserSlot)
                << BUILD_LOG(kInDoorUserInfo_.m_iPlayerIndex)
                << END_LOG;
            return false;
        }
        return false;
    }
    break;

    case KChangeRoomUserInfo::RUI_INDIGO_SLOT:
        ::memcpy(kInDoorUserInfo_.m_acTagModeInfo, kChangeRoomUserInfo_.m_acTagModeInfo, sizeof(kChangeRoomUserInfo_.m_acTagModeInfo));
        break;

    case KChangeRoomUserInfo::RUI_INFO_STATE:
        kInDoorUserInfo_.m_nState = kChangeRoomUserInfo_.m_nState;
        kInDoorUserInfo_.m_cPromotion = spUser->GetCurrentPromotion();
        if (kChangeRoomUserInfo_.m_nState == GC_RUS_READY)
        {
            if (IsNormalUserAllReady())
            {
                m_dwAllReadyTick = ::GetTickCount();
            }
            else
            {
                ClearChatEvent();
            }
        }
        break;

    default:

        kChangeRoomUserInfo_.m_nOK = 2;
        return false;

    }
    kChangeRoomUserInfo_.m_nOK = 0;
    START_LOG(clog, L"ChangeRoom Log name : " << kChangeRoomUserInfo_.m_strLogin)
        << stm.str() << END_LOG;

    stm.clear();
    stm.str(L"");

    return true;
}

bool KRoom::ChangeRoomUserInfo(IN OUT KChangeRoomUserInfo& kChangeRoomUserInfo_)
{
    bool bRet = false;

    KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo(kChangeRoomUserInfo_.m_strLogin);
    if (NULL == pkInDoorUserInfo) {
        kChangeRoomUserInfo_.m_nOK = 1;

        return false;
    }

    KUserPtr spUser = GetByName(kChangeRoomUserInfo_.m_strLogin);
    if (NULL == spUser) {
        kChangeRoomUserInfo_.m_nOK = 1;

        return false;
    }

    KUserPtr spUserRequest = SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >(kChangeRoomUserInfo_.m_dwUserUID);
    if (NULL == spUserRequest) {
        kChangeRoomUserInfo_.m_nOK = 1;

        return false;
    }

    bool bIsOwnRequest = (kChangeRoomUserInfo_.m_dwUserUID == spUser->GetUID());
    bool bIsAdminRequest = spUserRequest->IsAdmin();

    if (bIsOwnRequest || bIsAdminRequest) {
        bRet = ProcChangeRoomUserInfo(*pkInDoorUserInfo, kChangeRoomUserInfo_);
    }
    else {
        kChangeRoomUserInfo_.m_nOK = -200; 
    }

    GetChangeRoomUserInfo(*pkInDoorUserInfo, kChangeRoomUserInfo_);

    return bRet;
}

bool KRoom::SyncCharInfo( IN std::wstring& strLogin_, IN char cCharType_ /*= -1*/)
{
    KUserPtr spUser = GetByName( strLogin_ );

    if( !spUser )
    {
        START_LOG( cerr, L"방안에서 장착 정보 변경시 대상 유저가 방에 존재하지 않음 Name : " << strLogin_ ) << END_LOG;
        return false;
    }

    std::map<std::wstring,KInDoorUserInfo>::iterator mitInDoorUser;
    KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( strLogin_ );
    if( pkInDoorUserInfo == NULL )
    {
        START_LOG( cerr, L"방안에서 장착 정보 변경시 대상 유저의 InDoorUserInfo가 존재하지 않음 Name : " << strLogin_ ) << END_LOG;
        return false;
    }

    if( cCharType_ >= 0 ) // 캐릭터 타입은 함수 호출전에 검증이 되어 있어야 한다.
    {
        pkInDoorUserInfo->m_cCharIndex = cCharType_;
    }

    // InDoorUser에서 캐릭터 맵을 뽑아낸다.
    //뽑아낸 캐릭터 맵의 장착 정보에 새로 받은 장착 정보를 assign한다.
    std::vector<KInDoorCharInfo>& vecCharInfo = pkInDoorUserInfo->m_vecCharInfo;
    spUser->GetInDoorCharInfo( vecCharInfo );
    return true;
}

bool KRoom::AssignInDoorUserInfo( IN KInDoorUserInfo& kInDoorUserInfo_ )
{
    KUserPtr spUser = GetByName( kInDoorUserInfo_.m_strLogin );
    if( !spUser ) return false;

    KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( kInDoorUserInfo_.m_strLogin );

    if( pkInDoorUserInfo == NULL )
        return false;

    *pkInDoorUserInfo = kInDoorUserInfo_;
    return true;
}

void KRoom::AddInDoorUserInfo( IN KInDoorUserInfo& kInDoorUserInfo_, IN int& nTeamType_ )
{
    // 빈 슬롯에 찾아 넣는다.
    // 방송 서버 이면 운영자는 슬롯에 넣지 않는다.    
    KUserPtr spUser = GetByName( kInDoorUserInfo_.m_strLogin );
    if( !spUser ) return;

    KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( kInDoorUserInfo_.m_strLogin );
    if( pkInDoorUserInfo != NULL )
    {
        START_LOG( cwarn, L"InDoorUserInfo에 이미 존재한다, 이전 값을 이용한다." )
            << BUILD_LOG( kInDoorUserInfo_.m_strLogin ) << END_LOG;
        return;
    }

    if( spUser->GetChannel()->GetType() == KChannel::CT_INDIGO ) {
        kInDoorUserInfo_.m_nState = GC_RUS_READY;
    }

    //옵저버가 아니면 슬롯을 차지한다.
    if( kInDoorUserInfo_.m_bIsObserver )
    {
        kInDoorUserInfo_.m_iPlayerIndex = -1;
        kInDoorUserInfo_.m_iTeam = -1;
    }
    else
    {
        if( spUser->GetChannel()->GetType() == KChannel::CT_DUNGEON )
        {
            int nSlotNum = -1;
            for( int i = 0 ; i < MAX_NUM_USER_SLOT ; ++i )
            {
                if( m_abSlotOpen[i] )
                {
                    nSlotNum = i;
                    break;
                }
            } // 빈슬롯 있으면 순서대로 할당

            if ( nSlotNum == -1 ) {
                START_LOG( cwarn, L"열려있는 슬롯이 없음... Name: " << spUser->m_strName ) << END_LOG;
                return;
            }

            m_abSlotOpen[nSlotNum] = false;
            kInDoorUserInfo_.m_iPlayerIndex = nSlotNum;
            kInDoorUserInfo_.m_iTeam = 0;   // 던전에서는 항상 같은 팀(0)으로 설정한다

        }
        else if ( spUser->GetChannel()->GetType() == KChannel::CT_GUILD_UNLIMITED || // 길드 채널 처리
            spUser->GetChannel()->GetType() == KChannel::CT_GUILD_MASTER ||
            spUser->GetChannel()->GetType() == KChannel::CT_GUILD_EXPERT ||
            spUser->GetChannel()->GetType() == KChannel::CT_GUILD_ROOKIE ||
            spUser->GetChannel()->GetType() == KChannel::CT_GUILD_BEGINNER ) {            
                     
            // m_abSlotOpen == false 이면 유저가 이미 있는 슬롯.
            kInDoorUserInfo_.m_iTeam = GetGuildTeam( spUser->GetGuildUID() );
            int nSlot = GetEmptySlot( kInDoorUserInfo_.m_iTeam, true );
            if( nSlot < 0 )
            {
                START_LOG( clog, L"열려있는 슬롯이 없음... Name: " << kInDoorUserInfo_.m_strLogin ) << END_LOG;
                return;
            }
            m_abSlotOpen[nSlot] = false;
            kInDoorUserInfo_.m_iPlayerIndex = nSlot;
        }
        else { // 대전 모드

            if ( spUser->GetPartyUID() != 0 && spUser->GetChannel()->GetType() != KChannel::CT_AUTOMATCH ) { // 파티가 존재한다 --> 따라가기로 방으로 진입 --> 파티원 순서대로 슬롯 채운다

                int PartySlot = spUser->GetPartyIndex(); // 몇 번째 파티원인지 찾아서 방의 슬롯 순서로 사용.

                if ( PartySlot != -1 && m_abSlotOpen[PartySlot] ) {
                    m_abSlotOpen[PartySlot] = false;
                    kInDoorUserInfo_.m_iPlayerIndex = PartySlot;
                    kInDoorUserInfo_.m_iTeam = CalcTeam( kInDoorUserInfo_.m_iPlayerIndex );
                }
                else {
                    kInDoorUserInfo_.m_iTeam = GetMinTeam();
                    int nSlot = GetEmptySlot( kInDoorUserInfo_.m_iTeam );
                    if( nSlot < 0 )
                    {
                        START_LOG( clog, L"열려있는 슬롯이 없음... Name: " << kInDoorUserInfo_.m_strLogin ) << END_LOG;
                        return;
                    }
                    m_abSlotOpen[nSlot] = false;
                    kInDoorUserInfo_.m_iPlayerIndex = nSlot;
                    kInDoorUserInfo_.m_iTeam = CalcTeam( kInDoorUserInfo_.m_iPlayerIndex );
                }
            }
            else {
                if( spUser->GetChannel()->GetType() != KChannel::CT_AUTOMATCH ) {
                    kInDoorUserInfo_.m_iTeam = GetMinTeam();
                    int nSlot = GetEmptySlot( kInDoorUserInfo_.m_iTeam );
                    if( nSlot < 0 )
                    {
                        START_LOG( clog, L"열려있는 슬롯이 없음... Name: " << kInDoorUserInfo_.m_strLogin ) << END_LOG;
                        return;
                    }
                    m_abSlotOpen[nSlot] = false;
                    kInDoorUserInfo_.m_iPlayerIndex = nSlot;
                    kInDoorUserInfo_.m_iTeam = CalcTeam( kInDoorUserInfo_.m_iPlayerIndex );
                }
                else {
                    if( nTeamType_ == -1 ) { // 대전매칭에서 팀이 넘어오지 않을 경우..,
                        nTeamType_ = GetMinTeam();
                    }
                    kInDoorUserInfo_.m_iTeam = nTeamType_;
                    int nSlot = GetEmptySlot( kInDoorUserInfo_.m_iTeam );
                    if( nSlot < 0 )
                    {
                        START_LOG( clog, L"열려있는 슬롯이 없음... Name: " << kInDoorUserInfo_.m_strLogin ) << END_LOG;
                        return;
                    }
                    m_abSlotOpen[nSlot] = false;
                    kInDoorUserInfo_.m_iPlayerIndex = nSlot;
                    kInDoorUserInfo_.m_iTeam = CalcTeam( kInDoorUserInfo_.m_iPlayerIndex );
                }
            }
        }
    }

    m_mapInDoorUserInfo[kInDoorUserInfo_.m_strLogin] = kInDoorUserInfo_;
}

void KRoom::RemoveInDoorUserInfo( IN const KUser& kUser_ )
{
    std::map<std::wstring,KInDoorUserInfo>::iterator mit;
    mit = m_mapInDoorUserInfo.find( kUser_.GetName() );

    if( mit == m_mapInDoorUserInfo.end() )
    {
        START_LOG( cerr, L"해당 유저의 InDoorUserInfo가 룸에 존재하지 않음 name : " << kUser_.GetName() ) << END_LOG;
        return;
    }
    //슬롯을 열자.

    if( mit->second.m_bIsObserver ) { //옵저버가 나간것이 아니면 슬롯을 열자.
        if( m_nObserverCnt > 0 ) {
            m_nObserverCnt -= 1;
        }
    }
    else {
        m_abSlotOpen[mit->second.m_iPlayerIndex] = true;
    }

    m_mapInDoorUserInfo.erase( mit );
}

bool KRoom::IsNormalUserAllReady()//운영자가 아닌녀석이 게임시작하자고 요청했을때 호출
{
    std::map<std::wstring,KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        if( !mit->second.m_bIsObserver && !mit->second.m_bIsHost && !mit->second.m_nState == GC_RUS_READY )
        { // 옵저버가 아니고 방장 아니고 준비상태 아니면
            return false;
        }
    }
    return true;
}

bool KRoom::IsAdminExist()
{
    std::map<std::wstring, KUserPtr>::iterator itUser;
    for( itUser = m_mapSob.begin(); itUser != m_mapSob.end(); itUser++ ) //이방에 운영자가 있는가?
    {
        if( itUser->second->IsAdmin() )
            return true;
    }
    return false;
}

bool KRoom::TransitionEndGame( IN const KUser& kUser_ )
{
    if( GetStateID() != KRoomFSM::STATE_PLAYING )
    {
        START_LOG( cerr, L"상태 오류" ) << END_LOG;
        return false;
    }

    if( m_spHostUser.get() != &kUser_ )
    {
        START_LOG( cerr, L"요청자가 방장이 아님" ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"게임 종료. 방 객체 상태 변환" ) << END_LOG;
    StateTransition( KRoomFSM::INPUT_END_GAME );
    return true;
}


void KRoom::RefreshGuildArray()
{
    bool baGuildExist[2];
    baGuildExist[0] = false;
    baGuildExist[1] = false;
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for ( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit ) {
        if ( mit->second.m_dwGuildUID == m_naGuildID[0] ) {
            baGuildExist[0] = true;
        }

        if ( mit->second.m_dwGuildUID == m_naGuildID[1] ) {
            baGuildExist[1] = true;
        }
    }

    if ( !baGuildExist[0] ) {
        m_naGuildID[0] = 0;
        m_pairGuildMarkName.first.clear();
        m_pairGuildName.first.clear();
        m_pairBattlePoint.first = 0;
    }

    if ( !baGuildExist[1] ) {
        m_naGuildID[1] = 0;
        m_pairGuildMarkName.second.clear();
        m_pairGuildName.second.clear();
        m_pairBattlePoint.second = 0;
    }

}

void KRoom::GetUserUIDList( std::vector<DWORD>& vecUIDs_ )
{
    vecUIDs_.clear();

    KLocker lock(m_csInDoorUserInfo);

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for ( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit ) {
        if ( mit->second.m_bIsObserver ) {
            continue;
        }

        vecUIDs_.push_back( mit->second.m_dwUserUID );
    }
}

// gp 를 적용하고, 유저의 데이터를  룸에 적용한다.
void KRoom::UpdateCharacterInfo( std::wstring& strLogin_, int iGP_ )
{
    KUserPtr spUser = GetByName( strLogin_ );
    if( !spUser )
    {
        START_LOG( cerr, L"게임 종료시 캐릭터 승패 적용시 Room에 해당 유저 존재하지 않음." )
            << BUILD_LOG( strLogin_ ) << END_LOG;
        return;
    }

    KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( strLogin_ );
    if( pkInDoorUserInfo == NULL )
    {
        START_LOG( cerr, L"게임 종료시 캐릭터 승패 적용시 InDoorUserInfo 에 존재하지 않는다." )
            << BUILD_LOG( strLogin_ ) << END_LOG;
        return;
    }
    pkInDoorUserInfo->m_iGP = iGP_;

    spUser->GetInDoorCharInfo( pkInDoorUserInfo->m_vecCharInfo );
}

void KRoom::AddAbuseData( IN KUser& kUser_ )
{
    std::map<DWORD, KAbuseData>::iterator mit;
    if( (mit = m_mapAbuseData.find( kUser_.GetUID() )) != m_mapAbuseData.end() )
    {
        START_LOG( cerr, L"Ause Data가 이미 존재한다.." )
            << BUILD_LOG( kUser_.GetName() ) << END_LOG;
        m_mapAbuseData.erase( mit );
    }

    KAbuseData kAbuseData;
    kAbuseData.Clear();

    kAbuseData.m_dwUID = kUser_.GetUID();
    kAbuseData.m_strLogin = kUser_.GetName();
    kAbuseData.m_strNickName = kUser_.m_strNickName;
    kAbuseData.m_aElement[0].m_dwGP = kUser_.GetGP();
    kAbuseData.m_aElement[0].m_time = CTime::GetCurrentTime();
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        const KCharacterInfo* pkCharacter = kUser_.GetCharacterInfo( i );
        if( pkCharacter )
        {
            kAbuseData.m_aElement[0].m_biaExp[i] = pkCharacter->m_biExp;
        }
    }

    m_mapAbuseData.insert( std::make_pair( kAbuseData.m_dwUID, kAbuseData ) );

    START_LOG( clog, L" Abuser Data 추가" )
        << BUILD_LOG( kUser_.GetName() ) << END_LOG;
}

void KRoom::UpdateAbuseData( IN KUser& kUser_ )
{
    // 게임 종료시 호출
    //게임 플레이 증가
    // false 이고 카운트가 >= 30 이면 시간을 측정한다.
    //시간이 1시간 이하이면  true로 변경한다.

    std::map<DWORD, KAbuseData>::iterator mit;
    mit = m_mapAbuseData.find( kUser_.GetUID() );

    if( mit == m_mapAbuseData.end() )
    {
        START_LOG( cerr, L" Critical 어뷰즈 데이터 없는 상태에서 게임이 시작되었다.")
            << BUILD_LOG( kUser_.GetName() ) << END_LOG;
        return;
    }

    mit->second.m_nCount += 1; //플렝 한 카운트를 체크한다.

    if( mit->second.m_bAbuse == false && mit->second.m_nCount == SiKGameServer()->GetAbuseCount() )
    {
        //시간이 1시간 보다 적은가?
        CTimeSpan cSpan = CTime::GetCurrentTime() - mit->second.m_aElement[0].m_time;
        if( cSpan.GetTotalMinutes() <= (LONGLONG)SiKGameServer()->GetAbuseTime() )
        {
            mit->second.m_bAbuse = true;
            mit->second.m_aElement[1].m_dwGP = kUser_.GetGP();
            mit->second.m_aElement[1].m_time = CTime::GetCurrentTime();

            for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
            {
                const KCharacterInfo* pkCharacter = kUser_.GetCharacterInfo( i );
                if( pkCharacter )
                {
                    mit->second.m_aElement[1].m_biaExp[i] = pkCharacter->m_biExp;
                }
            }
        }
    }

    std::map<std::wstring,KUserPtr>::iterator mit1;
    std::map<std::wstring, std::pair<std::wstring,int> >::iterator mit2;
    for( mit1 = m_mapSob.begin(); mit1 != m_mapSob.end(); mit1++ )
    {
        if( kUser_.GetName() == mit1->second.get()->GetName() ) // 자기 제외
            continue;

        // 같이논 아이 카운트
        mit2 = mit->second.m_mapPlayerInfo.find( mit1->second.get()->GetName() );
        if( mit2 == mit->second.m_mapPlayerInfo.end() ) // 최초 등록
        {
             std::pair<std::wstring,int> prData( KncUtil::toWideString(mit1->second->GetIPStr()), 1);
            mit->second.m_mapPlayerInfo.insert(std::make_pair( mit1->second.get()->GetName(), prData )); // LoginID, 카운트초기값1
            continue;
        }

        ++mit2->second.second; // 등록된 유저.play count 값 증가
    }
}

void KRoom::RemoveAbuseData( IN KUser& kUser_ )
{
    // true이면 갱신 , 디스크 기록
    std::map<DWORD, KAbuseData>::iterator mit;
    mit = m_mapAbuseData.find( kUser_.GetUID() );

    if ( mit == m_mapAbuseData.end() ) {
        START_LOG( cerr, L" Critical 에뷰즈 데이터 없는 상태에서 룸나감.")
            << BUILD_LOG( kUser_.GetName() ) << END_LOG;
        return;
    }

    if ( mit->second.m_bAbuse == true ) {
        mit->second.m_aElement[2].m_dwGP = kUser_.GetGP();
        mit->second.m_aElement[2].m_time = CTime::GetCurrentTime();
        for ( int i = 0 ; i < GC_CHAR_NUM ; ++i ) {
            const KCharacterInfo* pkCharacter = kUser_.GetCharacterInfo( i );
            if ( pkCharacter ) {
                mit->second.m_aElement[2].m_biaExp[i] = pkCharacter->m_biExp;
            }
        }

        std::wstringstream stm;

        stm << L"Login : " << mit->second.m_strLogin
            << L", NickName : " << mit->second.m_strNickName
            << L", Count : " << mit->second.m_nCount
            << L", IP : " << KncUtil::toWideString( kUser_.GetIPStr() )
            << GLOG::endl
            << L"-- Info --" << GLOG::endl;

        for ( int j = 0 ; j < 3 ; ++j ) {
            stm << L"time :" << (LPCTSTR)mit->second.m_aElement[j].m_time.Format( KNC_TIME_FORMAT )
                << L" GP : " << mit->second.m_aElement[j].m_dwGP
                << L" Exp : ";

            for ( int k = 0 ; k < GC_CHAR_NUM ; ++k ) {
                stm << L"[ " << mit->second.m_aElement[j].m_biaExp[k] << L" ] ";
            } // end for
            stm << GLOG::endl;
        } // end for

        //{{ 080509 tslayer
        // 같이 플레이한 유저 정보
        stm << L"-- Info2 --" << GLOG::endl;

        std::map<std::wstring, std::pair<std::wstring,int> >::iterator mit1;
        for ( mit1 = mit->second.m_mapPlayerInfo.begin() ; mit1 != mit->second.m_mapPlayerInfo.end() ; ++mit1 ) {
            stm << L"Login : " << mit1->first
                << L" Count : " << mit1->second.second
                << L", IP : " << mit1->second.first
                << GLOG::endl;
        }
        stm << GLOG::endl;
        //}} 080509 tslayer.

        TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_LT_ABUSE, kUser_.GetName(), kUser_.GetUID(), 0, stm.str() );
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

        // ABuse Set..
        if( mit->second.m_mapPlayerInfo.size() <= (DWORD)SiKGameServer()->GetAbusePlayerCount() )
            kUser_.SetAbuseToDB();
    }
    m_mapAbuseData.erase( mit );
}


void KRoom::UpdatePetInfo( std::wstring strLogin_, char cChar_, KPetInfo& kPetInfo_ )
{
    std::map<std::wstring,KInDoorUserInfo>::iterator mit;
    KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( strLogin_ );

    if( pkInDoorUserInfo == NULL )
    {
        START_LOG( cerr, L"해당 유저의 InDoorUserInfo가 룸에 존재하지 않음 name : " << strLogin_ ) << END_LOG;
        return;
    }

    std::vector< KInDoorCharInfo >::iterator vit;
    vit = std::find_if( pkInDoorUserInfo->m_vecCharInfo.begin(), pkInDoorUserInfo->m_vecCharInfo.end(),
        boost::bind( &KInDoorCharInfo::m_cCharType, _1 ) == cChar_ );
    if( vit == pkInDoorUserInfo->m_vecCharInfo.end() )
    {
        START_LOG( cerr, L"해당 유저의 KInDoorCharInfo가 룸에 존재하지 않음 name : " << strLogin_  << L" CType : " << static_cast<int>(cChar_) ) << END_LOG;
        return;
    }

    vit->m_kPetInfo = kPetInfo_;
}

void KRoom::ProcessForceEvent( IN OUT KForceChangeRoomUserInfo& kPacket_ )
{
    kPacket_.m_nOK = 99;
    switch( kPacket_.m_ucChangeType )
    {
    case KForceChangeRoomUserInfo::FRUI_READY:
        {
            std::map<std::wstring, KInDoorUserInfo>::iterator mit;
            for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
            {
                if( mit->second.m_nState ==GC_RUS_READY )
                    mit->second.m_nState = GC_RUS_WAIT;
            }
            kPacket_.m_nOK = NetError::NET_OK;
        }
        break;
    default:
        kPacket_.m_nOK = -1;
        START_LOG( cerr, L"알수 없는 타입을 알려왔음." ) << END_LOG;
    }
}

bool KRoom::ChangeRoomOption( IN const KRoomOption& kReq_ )
{
    // 대기, 풀 상태 에서 가능.
    if( GetStateID() != KRoomFSM::STATE_WAITING &&
        GetStateID() != KRoomFSM::STATE_FULL )
    {
        SET_ERROR( ERR_ROOM_55 );
        return false;
    }

    if( kReq_.m_nDeathLimitTime < DEATH_MIN_TIME ||
        kReq_.m_nDeathKillCount < DEATH_MIN_KILL ||
        kReq_.m_nDeathKillCount > DEATH_MAX_KILL )
    {
        SET_ERROR( ERR_ROOM_58 );
        return false;
    }

    m_nDeathMatchTime       = kReq_.m_nDeathLimitTime;
    m_nDeathKillCount       = kReq_.m_nDeathKillCount;

    m_bDeathMatchIntrudable = kReq_.m_bDeathMatchIntrudable;
    m_bDeathMatchBalancing  = kReq_.m_bDeathMatchBalancing;

    m_strName               = kReq_.m_strRoomTitle;
    m_strPasswd             = kReq_.m_strPassWord;
    m_bPublic               = kReq_.m_strPassWord.empty(); // 비번 비면 공개방
 
    return true;
}

void KRoom::AddPromotionCount( std::vector< std::pair<char,char> >& vecPromotionCount_ )
{
    std::vector< std::pair<char,char> >::iterator vit;
    for( vit = vecPromotionCount_.begin() ; vit != vecPromotionCount_.end() ; ++vit )
    {
        JIF( vit->first >= 0 && vit->first < GC_CHAR_NUM );
        JIF( vit->second >= 0 && vit->second <= MAX_PROMOTION );
        short sKey = (vit->first << 8) + vit->second;
        KRoom::ms_mapPromotionCount[sKey] += 1;
    }   
}

bool KRoom::IsMaximumUser()
{
    int nMaxChatEventUserNum = 0;
    if( IsDungeonMode( m_iGameMode ) )
    {
        nMaxChatEventUserNum = SiKGSSimLayer()->GetChatEventDungeonUserNum();
    }
    else
    {
        nMaxChatEventUserNum = SiKGSSimLayer()->GetChatEventPvPUserNum();
    }

    if( (int)(GetCount()) < nMaxChatEventUserNum )
        return false;

    if( (int)GetCountExObserver() >= nMaxChatEventUserNum )
        return true;

    return false;
}

void KRoom::SetChatEvent( KChatEventInfo& chatEvent_ )
{
    m_kChatEventInfo = chatEvent_;
}

void KRoom::ClearChatEvent()
{
    m_bRewardChatEvent = false;
    m_setChattedUser.clear();
    m_dwAllReadyTick = 0;
}
void KRoom::ResetChatEvent()
{
    ClearChatEvent();
    m_kChatEventInfo.m_nUID = -1;
}

bool KRoom::GetChatEvent( OUT KChatEventInfo& chatEvent_ )
{
    chatEvent_.m_nUID = -1;
    if( m_kChatEventInfo.m_nUID <= 0 )
        return false;
    chatEvent_ = m_kChatEventInfo;
    return true;
}


void KRoom::CheckEventReward()
{
    std::map<std::wstring, KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        if( mit->second->IsObserver() )
            continue;

        if( m_setChattedUser.find( mit->second->GetUID() ) == m_setChattedUser.end() )
            return;
    }

    m_bRewardChatEvent = true;
}

float KRoom::GetChatEventEXPRatio()
{
    if( m_bRewardChatEvent )
    {
        return m_kChatEventInfo.m_fEXPRatio;
    }
    return 1.f;
}

float KRoom::GetChatEventGPRatio()
{
    if( m_bRewardChatEvent )
    {
        return m_kChatEventInfo.m_fGPRatio;
    }
    return 1.f;
}

void KRoom::RegChatEventCorrect( std::wstring strName_, int nUID_ )
{
    JIF( nUID_ > 0 && m_kChatEventInfo.m_nUID == nUID_ );

    KUserPtr spUser = GetByName( strName_ );
    if( !spUser )
    {
        START_LOG( cerr, L"방에 없는 유저가 채팅이벤트에 등록시도.. Name : " << strName_ ) << END_LOG;
        return;
    }
    m_setChattedUser.insert( spUser->GetUID() );
    START_LOG( clog, L"채팅이벤트에 다음유저가 올바른 문자열을 타이핑 했음 Name : " << strName_ ) << END_LOG;

    CheckEventReward(); // 모든 애들이 이벤트문자를 타이핑 했는지 체크 한다.
}

int KRoom::GetNormalUserNum()
{
    // 옵저버를 제외한 유저의 숫자.
    m_nStartingUserNum = (int)GetCount();
    if( m_nObserverCnt > 0 ) { // 옵저버 제외...
        m_nStartingUserNum -= m_nObserverCnt;
    }

    LIF( m_nStartingUserNum >= 1 );

     return m_nStartingUserNum;
}

void KRoom::GetGameInfo( OUT KGameInfo& kGameInfo_ )
{
    kGameInfo_.m_nStartUserNum                  = std::max<int>( 1, m_nStartingUserNum );
    kGameInfo_.m_nEndUserNum                    = GetNormalUserNum();
    kGameInfo_.m_cGameCategory                  = m_cGameCategory;
    kGameInfo_.m_iGameMode                      = m_iGameMode;
    kGameInfo_.m_iSubGameMode                   = m_iSubGameMode;
    kGameInfo_.m_nDifficulty                    = m_nDifficulty;
    kGameInfo_.m_iMapID                         = m_iMapID;
    kGameInfo_.m_bRandomableMap                 = m_bRandomableMap;
    kGameInfo_.m_nMonsterID                     = m_nMonsterID;
    kGameInfo_.m_nMonsterCrusaderMonLv          = m_nMonsterCrusaderMonLv;
    kGameInfo_.m_nMonsterCount                  = m_nMonsterCount;
    kGameInfo_.m_biTotalExp                     = m_biStartingTotalExp;
    kGameInfo_.m_dwTotalGp                      = m_dwStartingTotalGp;
    kGameInfo_.m_nTotalLv                       = m_nTotalLv;
    kGameInfo_.m_cChannelType                   = m_cChannelType;
    kGameInfo_.m_vecChampInfo                   = m_vecChampInfo;
    kGameInfo_.m_nDestKillCount                 = m_nDeathKillCount;
    GetTeamInfo( kGameInfo_.m_vecUIDTeam );
    kGameInfo_.m_nPCBangUserNum                 = GetPCBangUserNum();
    kGameInfo_.m_mapRemainVitalityOnStartGame   = m_mapRemainVitalityOnStartGame;

    kGameInfo_.m_vecUserUIDs.clear();
    kGameInfo_.m_mapPromotionInfo.clear();
    kGameInfo_.m_mapCharLevelInfo.clear();

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        if( mit->second.m_bIsObserver )
            continue;
        kGameInfo_.m_vecUserUIDs.push_back( mit->second.m_dwUserUID );

        if( m_iGameMode == GC_GM_TAG_TEAM || m_iGameMode == GC_GM_TAG_SURVIVAL )
        {
            // mit->second.m_acTagModeInfo[0][0] // 어떤 캐릭터인지 (캐릭터 타입)
            // mit->second.m_acTagModeInfo[0][1] // 캐릭터의 전직

            std::pair<char,char> prData1( mit->second.m_acTagModeInfo[0][0], mit->second.m_acTagModeInfo[0][1] );
            kGameInfo_.m_mapPromotionInfo[mit->second.m_dwUserUID].push_back( prData1 );

            std::pair<char,char> prData2( mit->second.m_acTagModeInfo[1][0], mit->second.m_acTagModeInfo[1][1] );
            kGameInfo_.m_mapPromotionInfo[mit->second.m_dwUserUID].push_back( prData2 );
        }
        else
        {
            std::pair<char, char> prData(mit->second.m_cCharIndex, mit->second.m_cPromotion );
            kGameInfo_.m_mapPromotionInfo[mit->second.m_dwUserUID].push_back( prData );
        }

        // 점령전 모드에서만 데이터를 담았지만 전체 모드 담게 변경(캐릭터 게임드랍에서 사용).
        std::pair<char, DWORD> prData(mit->second.m_cCharIndex, mit->second.m_dwLevel );
        kGameInfo_.m_mapCharLevelInfo[mit->second.m_dwUserUID] = prData;
    }

    kGameInfo_.m_prGuildUID.first = m_naGuildID[0];
    kGameInfo_.m_prGuildUID.second = m_naGuildID[1];
}

void KRoom::GetStartingInfo( OUT std::vector<KSimpleInDoorUserInfo>& vecStartingInfo_ )
{
    if( m_iGameMode == GC_GM_INDIGO_TEAM )
        return;

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        if( mit->second.m_bIsObserver )
            continue;

        if( mit->second.m_nState != GC_RUS_PLAYING )  // 플레이중이지 않은 유저는 빼고..
            continue;

        KSimpleInDoorUserInfo kSimpleUserInfo;
        kSimpleUserInfo.m_dwUserUID = mit->second.m_dwUserUID;
        kSimpleUserInfo.m_iTeam = mit->second.m_iTeam;

        if( m_iGameMode == GC_GM_TAG_TEAM || m_iGameMode == GC_GM_TAG_SURVIVAL )
        {
            kSimpleUserInfo.m_vecCharTypeJob.push_back( std::make_pair(mit->second.m_acTagModeInfo[0][0], mit->second.m_acTagModeInfo[0][1]) );
            kSimpleUserInfo.m_vecCharTypeJob.push_back( std::make_pair(mit->second.m_acTagModeInfo[1][0], mit->second.m_acTagModeInfo[1][1]) );
        }
        else
        {
            kSimpleUserInfo.m_vecCharTypeJob.push_back( std::make_pair(mit->second.m_cCharIndex, mit->second.m_cPromotion) );
        }

        vecStartingInfo_.push_back( kSimpleUserInfo );
    }

}

void KRoom::SetTimeEvent( IN const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> > mapActiveEventList_ )
{
    m_mapEventList = mapActiveEventList_;
}

void KRoom::AddTimeEvent( IN const std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> > mapActiveEventList_ )
{
    std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::const_iterator mit;
    for( mit = mapActiveEventList_.begin() ; mit != mapActiveEventList_.end() ; ++mit )
    {
        std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::iterator mit2;
        mit2 = m_mapEventList.find( mit->first );
        if( mit2 == m_mapEventList.end() )
        {
            m_mapEventList[mit->first] = mit->second;
        }
        else
        {
            std::vector<sTimeEvent>::iterator vit;
            mit2->second.insert( mit2->second.end(), mit->second.begin(), mit->second.end() );
            std::sort( mit2->second.begin(), mit2->second.end(), KComp() );
            vit = std::unique( mit2->second.begin(), mit2->second.end(), KEqu() );
            if( vit != mit2->second.end() )
            {
                mit2->second.erase( vit, mit2->second.end() );
            }
        }
    }
}

bool KRoom::GetTimeEvent( OUT std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >& mapActiveEventList_ )
{
    mapActiveEventList_.clear();
    if( m_mapEventList.empty() )
        return false;

    mapActiveEventList_.swap( m_mapEventList );
    return true;

}

void KRoom::ClearTimeEvent()
{
    m_mapEventList.clear();
}

void KRoom::SetStartingInfo()
{
    m_nStartingUserNum = GetNormalUserNum();

    m_biStartingTotalExp = 0;
    m_dwStartingTotalGp  = 0;
    m_nTotalLv = 0;

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        std::vector< KInDoorCharInfo >::iterator vit;
        vit = std::find_if( mit->second.m_vecCharInfo.begin(), mit->second.m_vecCharInfo.end(),
            boost::bind( &KInDoorCharInfo::m_cCharType, _1 ) == mit->second.m_cCharIndex );

        if( vit != mit->second.m_vecCharInfo.end() )
        {
            m_biStartingTotalExp += vit->m_biExp;
            m_dwStartingTotalGp  += (DWORD)(mit->second.m_iGP);
            m_nTotalLv += (int)vit->m_dwLevel;
        }
    }
    m_nTotalLv = std::max<int>(m_nTotalLv, 1);
}

void KRoom::SetDropList( IN KDropData& kDropData_ )
{
    m_kDropData = kDropData_;
}

void KRoom::GetDropList( OUT IN KDropData& kDropData_ )
{
    kDropData_ = m_kDropData;
}

void KRoom::ClearDropInfo()
{
    m_kDropData.m_vecMonsterDrop.clear();
    m_kDropData.m_vecMissionDrop.clear();
    m_kDropData.m_vecGpDrop.clear();
}

void KRoom::UpdateIndoorUserInfo()
{
    // 이부분 구현 해놓으면 편하게 할수 있겠다. ^^;
}

void KRoom::UpdatePremiumData( IN const std::wstring& strLogin_, IN const KPremiumInfo& kInfo_ )
{
    KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( strLogin_ );
    if ( pkInDoorUserInfo != NULL ) {
        pkInDoorUserInfo->m_kPremiumInfo = kInfo_;
    }
}

void KRoom::GetPremiumInfo( OUT USER_PREMIUM_INFO& mapPremium_ )
{
    mapPremium_.clear();

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        LIF( mapPremium_.insert( std::make_pair(mit->second.m_dwUserUID, mit->second.m_kPremiumInfo ) ).second );
    }
}

bool KRoom::PlayableDungeon( IN const int& nGameMode_, IN OUT int& nDifficulty_ )
{
    std::map<int,KDungeonUserInfo> mapDifficulty;
    mapDifficulty.clear();
    std::map<std::wstring, KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ ) {
        mit->second->GetUserDifficultInfo( mapDifficulty );
        if( !mit->second->IsAdmin() &&
            !SiKDungeonEntrance()->CheckDungeonEntrance( mapDifficulty, nGameMode_, nDifficulty_ ) )
            return false;
    }
    return true;
}

bool KRoom::IsShortGame()
{
    if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
        return false;

    if( SiKGSSimLayer()->CheckTypeFlag( ST_INDIGO ) )
        return false;

    if( m_iGameMode == GC_GM_AUTOMATCH_TEAM ) {
        return false;
    }

    if( !SiKPreHackingCheckManager()->GetIgnoreShortGame() )
        return false;

    DWORD dwServerThresholdTick = SiKPreHackingCheckManager()->GetThresholdTick();
    if ( m_iGameMode == GC_GM_DOTA ) {
        dwServerThresholdTick = SiKPreHackingCheckManager()->GetDotaThresholdTick();
    }

    return GetLatestPlayTime() < dwServerThresholdTick;
}

void KRoom::SetChampInfo( std::vector<KChampionInfo>& vecChampInfo_ )
{
    m_vecChampInfo = vecChampInfo_;
}

void KRoom::GetChampInfo( OUT std::vector<KChampionInfo>& vecChampInfo_ )
{
    vecChampInfo_ = m_vecChampInfo;
}

void KRoom::ClearChampInfo()
{
    m_vecChampInfo.clear();
}

bool KRoom::IsSinglePlayable()
{
    if( m_cGameCategory == GC_GMC_DUNGEON ) return true;

    switch( m_iGameMode )
    {
    case GC_GM_MONSTER_HUNT:
    case GC_GM_MONSTER_CRUSADER:
        return true;
    }

    return false;
}
// 유저의 현재 퀵슬롯 정보를  룸에 있는  정보에 반영
void KRoom::UpdateQuickSlotData()
{
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock( m_csSob ); // 락 걸 필요는 없지만.. ㅡㅡ
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( mit->first );
        if( pkInDoorUserInfo == NULL ) continue;

        mit->second->GetCharQuickSlotInfo( pkInDoorUserInfo->m_kQuickSlot );
    }
}

void KRoom::UpdateAuthLevel()
{
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock( m_csSob ); // 락 걸 필요는 없지만.. ㅡㅡ
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( mit->first );
        if( pkInDoorUserInfo == NULL ) continue;

        pkInDoorUserInfo->m_cAuthLevel = mit->second->m_cAuthLevel;
    }

}

bool KRoom::IsPremiumRoom()
{
    const KInDoorUserInfo* pkInDoorUserInfo = GetHostInDoorUserInfo();
    if( pkInDoorUserInfo ) {
        if ( pkInDoorUserInfo->m_kPremiumInfo.CheckType(KPremiumInfo::TYPE_GCBLOG) || pkInDoorUserInfo->m_kPremiumInfo.CheckType(KPremiumInfo::TYPE_GCCLUB) ) {
            return true;
        }
    }

    return false;
}

bool KRoom::IsHellMode()
{
    if( IsDungeonMode( m_iGameMode ) == false )
        return false;

    if( m_nDifficulty < GC_GMD_CHAMPION_BEGIN )
        return false;

    // 헬 모드다!
    return true;
}

bool KRoom::IsBreakMode()
{
    if (IsDungeonMode(m_iGameMode) == false)
        return false;

    if (m_nDifficulty < GC_GMD_BREAK_BEGIN)
        return false;

    // 헬 모드다!
    return true;
}

bool KRoom::StartHellModeProcess()
{
    // 던전 이냐?
    // 현재 난이도가 헬모드 이냐?
    if( IsHellMode() == false )
    {
        START_LOG( clog, L"DungeonMode가 아니거나(PvP등) 난이도 3 미만(일반던전)이다." )
            << BUILD_LOG( m_iGameMode )
            << BUILD_LOG( m_nDifficulty ) << END_LOG;
        return true;
    }

    //위에서 거른 결과는 헬모드이겠지만 헬 입장권을 체크하지 않는 경우.
    if( SiKGCHelper()->HellItemCheckEnable(m_iGameMode) == false )
    {
        START_LOG( clog, L"헬아이템 체크하지 않음." )
            << BUILD_LOG( m_iGameMode )
            << BUILD_LOG( m_nDifficulty ) << END_LOG;
        return true;
    }

    // 이전난이도는 클리어 했느냐?
    // 모두 헬 아이템을 보유 하고 있느냐?
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock( m_csSob ); // 락 걸 필요는 없지만.. ㅡㅡ
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        if( !mit->second->HellModeCheck( m_iGameMode, m_nDifficulty ) )
        {
            START_LOG( clog, L"헬모드 시작 조건이 안된다. GameMode : " << m_iGameMode << L", Difficulty : " << m_nDifficulty ) << END_LOG;
            return false;
        }
    }

    bool bHellTicketDec = !( SiKGCHelper()->IsEndGameHellTicketDec() );
    // 헬 아이템을 깎는다..
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        if( mit->second->CheckIfDurItemExist( SiKGCHelper()->GetHellItemID(), 1, bHellTicketDec ) == false )
        {
            //Log를 남기자...
            START_LOG( cerr, L"헬모드 진입시 체크 다 했는데 아이템이 없음.. Name : " )
                << BUILD_LOG( mit->second->GetName() )
                << BUILD_LOG( SiKGCHelper()->IsEndGameHellTicketDec() ) << END_LOG;
        }
    }
    return true;
}

bool KRoom::StartBreakModeProcess()
{
    // 던전 이냐?
    // 현재 난이도가 헬모드 이냐?
    if (IsBreakMode() == false)
    {
        START_LOG(clog, L"DungeonMode가 아니거나(PvP등) 난이도 3 미만(일반던전)이다.")
            << BUILD_LOG(m_iGameMode)
            << BUILD_LOG(m_nDifficulty) << END_LOG;
        return true;
    }

    // 이전난이도는 클리어 했느냐?
    // 모두 헬 아이템을 보유 하고 있느냐?
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock(m_csSob); // 락 걸 필요는 없지만.. ㅡㅡ
    for (mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit)
    {
        if (!mit->second->BreakModeCheck(m_iGameMode, m_nDifficulty))
        {
            START_LOG(clog, L"헬모드 시작 조건이 안된다. GameMode : " << m_iGameMode << L", Difficulty : " << m_nDifficulty) << END_LOG;
            return false;
        }
    }

    for (mit = m_mapSob.begin(); mit != m_mapSob.end(); ++mit)
    {
        if (mit->second->CheckIfDurItemExist(SiKGCHelper()->GetHellItemID(), 1, false) == false)
        {
            //Log를 남기자...
            START_LOG(cerr, L"헬모드 진입시 체크 다 했는데 아이템이 없음.. Name : ")
                << BUILD_LOG(mit->second->GetName())
                << BUILD_LOG(SiKGCHelper()->IsEndGameHellTicketDec()) << END_LOG;
        }
    }
    return true;
}

void KRoom::GameStartStateTransition( IN const std::vector<DWORD>& vecUserUIDs_ )
{
    KLocker lock( m_csSob );

    std::vector<DWORD>::const_iterator vit;
    for( vit = vecUserUIDs_.begin() ; vit != vecUserUIDs_.end() ; ++vit )
    {
        std::map<std::wstring,KUserPtr>::iterator mit;
        for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
        {
            if( mit->second->GetUID() == *vit ) break;
        }

        if( mit == m_mapSob.end() )
        {
            START_LOG( cerr, L" 존재하지 않는 User UID : " << *vit )
                << BUILD_LOGc( m_cChannelType )
                << BUILD_LOG( m_usRoomID ) << END_LOG;
            continue;
        }

        if ( mit->second->GetStateID() == KUserFSM::STATE_PLAYING ) {
            continue;
        }

        KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( mit->first );
        if( pkInDoorUserInfo == NULL )
        {
            START_LOG( cerr, L"Indoor Userinfo에 유저가 없다니.. 말이 되냐??" )
                << BUILD_LOG( mit->first ) << END_LOG;
            continue;
        }

        pkInDoorUserInfo->m_nState = GC_RUS_PLAYING;
        mit->second->StateTransition( KUserFSM::INPUT_START_GAME_OK );
        mit->second->InitLoadingCompleteTick();
        mit->second->ClearPressInfo();

        //060109. microcat. 파이날 퀘스트를 한다면 방에 있는 모든 유저에게 설정
        mit->second->m_bIsFinalQuest = false;
        if( m_iGameMode == GC_GM_QUEST16 )
        {
            mit->second->m_bIsFinalQuest = true;
        }

        SiKFailRate()->IncreaseCount( KFailRate::GAME_START_USER );
    }
}

bool KRoom::IsDeathMatchReady( OUT std::vector<DWORD>& vecStartUser_ )
{
    _JIF( m_iGameMode == GC_GM_DEATH_TEAM || m_iGameMode == GC_GM_DEATH_SURVIVAL, return false );
    if( m_iGameMode == GC_GM_DEATH_TEAM )
    {
        std::set<int> setTeam;
        std::map<std::wstring, KInDoorUserInfo>::iterator mit;
        for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
        {
            if( mit->second.m_bIsHost || mit->second.m_nState == GC_RUS_READY )
            {
                vecStartUser_.push_back( mit->second.m_dwUserUID );
                setTeam.insert( mit->second.m_iTeam );
            }
        }

        if( setTeam.size() != 2 ) return false;
    }
    else // 서바이벌
    {
        std::map<std::wstring, KInDoorUserInfo>::iterator mit;
        for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
        {
            if( mit->second.m_bIsHost || mit->second.m_nState == GC_RUS_READY )
                vecStartUser_.push_back( mit->second.m_dwUserUID );
        }
    }
    return ( !vecStartUser_.empty() );
}

void KRoom::GetReadyUserUID( OUT std::vector<DWORD>& vecStartUser_ )
{
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        if( mit->second.m_nState == GC_RUS_READY )
            vecStartUser_.push_back( mit->second.m_dwUserUID );
    }
}

void KRoom::GetStartingUser( OUT std::vector<DWORD>& vecStartUser_ )
{
    vecStartUser_.clear();
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock( m_csSob );
    for ( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ ) {

        if ( mit->second->GetStateID() == KUserFSM::STATE_PLAYING ) {
            vecStartUser_.push_back( mit->second->GetUID() );
        }
    }
}

int KRoom::CalcTeam( int nSlotPos_ )
{
    // 1 : Blue
    // 0 : Red
    if( nSlotPos_ < MAX_NUM_USER_SLOT / MAX_TEAM_NUM )
        return 0;
    return 1;
}

int KRoom::GetMinTeam()
{
    int naTeamCount[2] = {0,0};
    int nLoopCount = MAX_NUM_USER_SLOT / MAX_TEAM_NUM;
    for( int i = 0 ; i < nLoopCount ; ++i )
    {
        if( m_abSlotOpen[i] ) // true 이면 열린 슬롯
        {
            naTeamCount[0] += 1;
        }

        if( m_abSlotOpen[i + nLoopCount] )
        {
            naTeamCount[1] += 1;
        }
    }
    return ( naTeamCount[1]>naTeamCount[0] ? 1:0 );
}

int KRoom::GetEmptySlot( int nTeam_, bool bForce_ /*= false*/ )
{
    // nTeam = 1 : blue
    // nTeam = 0 : red
    int nBegin = 0 + ( nTeam_ * MAX_NUM_USER_SLOT / MAX_TEAM_NUM );
    int nEnd = (MAX_NUM_USER_SLOT / MAX_TEAM_NUM) + ( nTeam_ * MAX_NUM_USER_SLOT / MAX_TEAM_NUM );

    for( int i = nBegin ; i < nEnd ; ++i )
    {
        if( m_abSlotOpen[i] ) return i;
    }
    
    if( bForce_ ) return -1; // 유연하지 않게.

    //리턴이 안된 상태라면.. 빈 슬롯 아무거나 넣어 준다.
    int nSlotNum = -1;
    for( int i = 0 ; i < MAX_NUM_USER_SLOT ; ++i )
    {
        if( m_abSlotOpen[i] )
        {
            nSlotNum = i;
            break;
        }
    }

    return nSlotNum;
}

void KRoom::DumpRoomInfo()
{
    if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;
    START_LOG( cwarn, L"====== Dump Room START ======" )
        << BUILD_LOG( m_usRoomID )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( m_usMaxUsers )
        << BUILD_LOGc( m_cGameCategory )
        << BUILD_LOG( m_iGameMode )
        << BUILD_LOG( m_iSubGameMode )
        << BUILD_LOG( m_iP2PVersion )
        << BUILD_LOG( GetStateID() )
        << BUILD_LOG( JoinGameEnable() )
        << BUILD_LOG( m_bDeathMatchIntrudable )
        << BUILD_LOG( m_bDeathMatchBalancing )
        << BUILD_LOG( m_nDeathMatchTime )
        << BUILD_LOG( m_nDeathKillCount )
        << BUILD_LOG( m_dwRoomOptions )
        << BUILD_LOG( m_dwRoomAllowerdChars )
        << BUILD_LOG( m_dwRoomAllowedRankings )
        << BUILD_LOGc( m_cChannelType ) << END_LOG;
    START_LOG( cwarn, L"====== Dump Room END ======" ) << END_LOG;
}

int KRoom::GetPlayingUserCount()
{
    int nRet = 0;
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::const_iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
    {
        if( mit->second->GetStateID() == KUserFSM::STATE_PLAYING )
            ++nRet;
    }
    return nRet;
}

int KRoom::GetPCBangUserNum()
{
    int nRet = 0;
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::const_iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
    {
        if( mit->second->IsPCBangUser() )
            ++nRet;
    }
    return nRet;
}

bool KRoom::IsLoading()
{
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::const_iterator mit;
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
    {
        if( mit->second->GetStateID() == KUserFSM::STATE_PLAYING && !mit->second->GetLoadingComplete() )
            return true;
    }
    return false;
}

void KRoom::ClearLeftRoom()
{
    m_setUserUID.clear();
}

bool KRoom::IsLeftUser( DWORD dwUserUID_ )
{
    return ( m_setUserUID.find( dwUserUID_ ) != m_setUserUID.end() );
}

void KRoom::InsertLeftUser( DWORD dwUserUID_ )
{
    m_setUserUID.insert( dwUserUID_ );
}

bool KRoom::BroadID( int nBroadCastType_, std::wstring strReqLogin_, unsigned short usEventID_ )
{
    std::wstringstream stm;
    switch( nBroadCastType_ )
    {
    case BROAD_TO_ALL:
        {
            std::map<std::wstring, KUserPtr>::iterator mit;
            KLocker lock( m_csSob );
            for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
            {
                mit->second->SendID( usEventID_ );
                stm << mit->second->GetName() << ", ";
            }
        }
        break;
    case BROAD_TO_ALL_EX_ME:
        {     
            std::map<std::wstring, KUserPtr>::iterator mit;
            KLocker lock( m_csSob );
            for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
            {
                if( mit->second->m_strName != strReqLogin_ )
                {
                    mit->second->SendID( usEventID_ );
                    stm << mit->second->GetName() << ", ";
                }
            }
        }
        break;
    default:
        START_LOG( cerr, L"지원하지 않는 브로드 캐스트 타입 입니다." )
            <<BUILD_LOG( nBroadCastType_ ) << END_LOG;
        return false;

    }

    START_LOG( clog, L"ⓢⓑ " << KUserEvent::GetEventIDString( usEventID_ ) << L" (name:" << stm.str() << L")"  ) << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    return true;
}

void KRoom::SetPvPEvent( IN const int& nWinTeam_, OUT std::vector< std::wstring >& vecStrLogin_ ) // kGameResultIn_ = five kill winner
{
    m_nWinTeam = nWinTeam_; // 이긴 팀 설정
    vecStrLogin_.clear();

    std::map<std::wstring,KInDoorUserInfo>::iterator mit;
    
    for( mit = m_mapInDoorUserInfo.begin(); mit != m_mapInDoorUserInfo.end(); ++mit )
    {
        if( m_nWinTeam == mit->second.m_iTeam )
        {
            vecStrLogin_.push_back( mit->first );
            // 벡터에 로긴을 넘길까.
        }
    }

    START_LOG( clog, L"승리팀 설정" )
        << BUILD_LOG( m_usRoomID )
        << BUILD_LOG( nWinTeam_ ) << END_LOG;
}

bool KRoom::JoinGameEnable()
{

    if( m_nCurrentTime < 60 )
        return false;

    int nCount = (int)GetCount();
    if( (m_nDeathKillCount - m_nCurrentKill) < (int)(nCount * 0.5f + 1) )
        return false;
    
    return true;
}

void KRoom::SetDeathScore( KDeathMatchScore& kInfo_ )
{
    JIF( kInfo_.m_nCurrentKill >= 0 && kInfo_.m_nCurrentTime >= 0 );
    m_nCurrentKill = kInfo_.m_nCurrentKill;
    m_nCurrentTime = kInfo_.m_nCurrentTime;
}

void KRoom::GetDeathScore( KDeathMatchScore& kInfo_ )
{
    kInfo_.m_nDestKill      = m_nDeathKillCount;
    kInfo_.m_LimitTime      = m_nDeathMatchTime;
    kInfo_.m_nCurrentKill   = m_nCurrentKill;
    kInfo_.m_nCurrentTime   = m_nCurrentTime;
}

void KRoom::ClearDeathScore()
{
    m_nCurrentKill = 0;
    m_nCurrentTime = m_nDeathMatchTime;

}

// 장착스킬 정보가 바뀌면 m_mapInDoorUserInfo 방 유저 정보를 갱신하는 함수. 
// User->Room->User 로 접근하는 구조가 아쉽다.

void KRoom::UpdateSkillSetInfo( std::wstring& strLogin_ )
{
    KInDoorUserInfo* pkIndoorUser = FindInDoorUserInfo( strLogin_ );
    JIF( pkIndoorUser );
    KUserPtr spUser = GetByName( strLogin_ );
    JIF( spUser );

    std::vector< KInDoorCharInfo >::iterator vit;
    for( vit = pkIndoorUser->m_vecCharInfo.begin() ; vit != pkIndoorUser->m_vecCharInfo.end() ; ++vit )
    {
        spUser->GetSkillSettingData( vit->m_cCharType, vit->m_mapEquipSkillSet );        
    }
}

bool KRoom::IsSameTeam( DWORD dwUID1_, DWORD dwUID2_ )
{
    int nTeam1 = -1;
    int nTeam2 = -1;

    // 참고: 던전에서는 모든 파티원이 항상 같은 팀이다

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit )
    {
        if( mit->second.m_dwUserUID == dwUID1_ )
        {
            nTeam1 = mit->second.m_iTeam;
        }
        else if( mit->second.m_dwUserUID == dwUID2_ )
        {
            nTeam2 = mit->second.m_iTeam;
        }

        if( nTeam1 >= 0 && nTeam2 >= 0 ) break;
    }

    if( nTeam1 < 0 || nTeam2 < 0 ) return false;

    if( nTeam1 != nTeam2 ) return false;

    return true;
}

void KRoom::DivorceUser( std::wstring& strLogin_ )
{
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    mit = m_mapInDoorUserInfo.find( strLogin_ );
    if( mit == m_mapInDoorUserInfo.end() ) return;

    mit->second.m_dwCoupleUID       = 0;
    mit->second.m_strPartnerNick.clear();
    mit->second.m_vecCoupleEquip.clear();

}

void KRoom::GetTeamInfo( std::vector<std::pair<DWORD,int> >& vecTeamInfo )
{
    vecTeamInfo.clear();

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;

    if ( m_iGameMode == GC_GM_SURVIVAL_MODE ||
         m_iGameMode == GC_GM_TAG_SURVIVAL ||
         m_iGameMode == GC_GM_DEATH_SURVIVAL ||
         m_iGameMode == GC_GM_FATAL_DEATH_SURVIVAL ) {
        int nTeam = 0;
        for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit, ++nTeam ) {
            vecTeamInfo.push_back( std::make_pair( mit->second.m_dwUserUID, nTeam ) );
        }
        return;
    }

    if( !IsDungeonMode( m_iGameMode ) && 
        m_iGameMode != GC_GM_TEAM_MODE && 
        m_iGameMode != GC_GM_TAG_TEAM &&
        m_iGameMode != GC_GM_GUILD_BATTLE && 
        m_iGameMode != GC_GM_DEATH_TEAM && 
        m_iGameMode != GC_GM_MONSTER_CRUSADER &&
        m_iGameMode != GC_GM_CAPTAIN &&
        m_iGameMode != GC_GM_AUTOMATCH_TEAM &&
        m_iGameMode != GC_GM_FATAL_DEATH_TEAM ) {
        return;
    }

    for( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit ) {
        vecTeamInfo.push_back( std::make_pair( mit->second.m_dwUserUID, mit->second.m_iTeam ) );
    }
}

bool KRoom::Send_TeamChatMsg( IN const KChatData& kNot_, IN const int nTeam_ )
{
    bool bCompress = false;
    if( kNot_.m_strChatMsg.size() > 40 && GetCount() > 3 )
        bCompress = true;

    KSerBuffer kbuff;
    kbuff.Clear();
    SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_CHAT_NOT, kNot_, kbuff, 100 );

    std::map<std::wstring, KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        if( mit->second->GetStateID() != KUserFSM::STATE_ROOM ) continue;

        const KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo( mit->first );
        if( pkInDoorUserInfo == NULL ) continue;

        if( pkInDoorUserInfo->m_iTeam == nTeam_ )
        {
            mit->second->SendPacket( kbuff, L"KUserEvent::EVENT_CHAT_NOT" );
        }
    }
    return true;
}

size_t KRoom::GetCountExObserver()
{
    int nCount = 0;
    std::map<std::wstring, KUserPtr>::iterator mit;
    for( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit )
    {
        if( mit->second->IsObserver() )
            continue;
        ++nCount;
    }

    return nCount;
}

void KRoom::GetUserInvenInfo( OUT std::map< DWORD, std::pair<int,int> >& mapUsersInvenInfo_ )
{
    mapUsersInvenInfo_.clear();
    std::map<std::wstring, KUserPtr>::iterator mit;
    KLocker lock( m_csSob );
    for( mit = m_mapSob.begin(); mit != m_mapSob.end(); mit++ )
    {
        mapUsersInvenInfo_[mit->second->GetUID()] = std::make_pair( mit->second->GetCharInvenCapacity() ,mit->second->m_kInventory.GetTotalEnableSize() );
    }
}

void KRoom::UpdateHostOrderByPing()
{
    struct KComp
    {
        bool operator() ( const KUserPtr& ptr1, const KUserPtr& ptr2 )
        {
            return ( ptr1->m_kPingInfo.GetPing() < ptr2->m_kPingInfo.GetPing() );
        }
    };

    if ( m_vecHostOrder.empty() )
        return;

    std::vector<KUserPtr>::iterator vit = m_vecHostOrder.begin();
    while ( vit != m_vecHostOrder.end() ) {
        if ( *vit == NULL ) {
            vit = m_vecHostOrder.erase( vit );
            continue;
        }
        ++vit;
    }

    // 위에서 이미 유효하지 않은 객체를 제거 했다.
    std::stable_sort( m_vecHostOrder.begin(), m_vecHostOrder.end(), KComp() );
    std::wstringstream stm;
    stm << L"Host Order.. : ";
    for ( vit = m_vecHostOrder.begin() ; vit != m_vecHostOrder.end() ; ++vit ) {

        stm << (*vit)->GetName() << L"[" << (*vit)->m_kPingInfo.GetPing() << L"], ";
    }

    START_LOG( cwarn, stm.str() ) << END_LOG;
}

KUser* KRoom::GetPlayingHost()
{
    return m_spPlayingHostUser.get();
}

// 데쓰 매치는 전혀 고려 되고 있지 않다.
void KRoom::MigrationForPlaying()
{
    if ( m_vecHostOrder.empty() )
        return;
    JIF( m_spHostUser );

    if ( false == SiKGSSimLayer()->IsHostMigrationByPing() ) {
        m_spPlayingHostUser = m_spHostUser;
        return;
    }

    if ( m_spPlayingHostUser == NULL ) { // 플레이 호스트가 널인경우 (게임 시작후 처음 불릴때)
        m_spPlayingHostUser = m_vecHostOrder.front();
    }
}

void KRoom::SameMigrationForPlaying()
{
    JIF( m_spHostUser);
    if ( !m_spPlayingHostUser )
        m_spPlayingHostUser = m_spHostUser;

}

void KRoom::RollbackMigrationFromPlaying( DWORD& dwHostUID_ )
{
    JIF( m_spHostUser);
    __JIF( m_spPlayingHostUser, return );

    dwHostUID_ = m_spHostUser->GetUID();
    START_LOG( clog, L"Rollback Host Migration " << m_spPlayingHostUser->GetName() << L" -> " << m_spHostUser->GetName() ) << END_LOG;
    
    ClearPlayingHost();
}

void KRoom::ClearPlayingHost()
{
    m_spPlayingHostUser.reset();
}

void KRoom::GetPressState( OUT std::vector<std::pair<DWORD, KPressureState> >& vecPressSate_ )
{
    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( !mit->second ) continue;
        std::pair<DWORD, KPressureState> prData;
        prData.first = mit->second->GetUID();
        mit->second->GetPressInfo( prData.second.m_nState, prData.second.m_dwElapsedTime );
        vecPressSate_.push_back( prData );
    }
}

void KRoom::GetIdleState( OUT std::vector<std::pair<DWORD, int> >& vecIdleState_ )
{
    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( !mit->second ) continue;
        std::pair<DWORD, int> prData;
        prData.first = mit->second->GetUID();
        mit->second->GetIdleInfo( prData.second );
        vecIdleState_.push_back( prData );
    }
}

void KRoom::GetAllMemberPingInfo( OUT std::vector<std::pair<DWORD,DWORD> >& vecPinInfo_ )
{
    vecPinInfo_.clear();
    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( !mit->second ) { continue; }

        std::pair<DWORD,DWORD> prPingInfo( mit->second->GetUID(), mit->second->m_kPingInfo.GetPing() );
        vecPinInfo_.push_back( prPingInfo );
    }
}

DWORD KRoom::GetPlayingHostUID()
{
    if ( m_spPlayingHostUser ) {
        return m_spPlayingHostUser->GetUID();
    }
    return 0L;
}

void KRoom::CheckLoadingCompleteUser( std::wstring LoginID_, DWORD UserUID_ /*= 0*/ )
{
    KEVENT_LOAD_COMPLETE_BROAD kPacket;
    KLocker lock( m_csSob );

    if ( UserUID_ != 0 ) {
        m_vecLoadFailUser.push_back( UserUID_ );
    }

    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( mit->second->GetLoadingComplete() == false &&
             mit->second->GetUID() != UserUID_ ) { // 로딩 시간 초과시 자신(나갈유저)의 UID 넣어줌.

            START_LOG( clog, L"로딩 완료 못한 유저 있음 " )
                << BUILD_LOG( mit->second->GetName() ) << END_LOG;

            return; // 아직 로딩 다 못했군 
        }
    }

    kPacket = m_vecLoadFailUser;
    m_vecLoadFailUser.clear();

    // broadcast to all users in room
    if ( UserUID_ == 0 ) {
        START_LOG( clog, L"방의 모든 유저 로드 완료 ")
            << BUILD_LOG( kPacket.size() ) << END_LOG;
        BroadPacket( KRoom::BROAD_TO_ALL, L"", KUserEvent::EVENT_LOAD_COMPLETE_BROAD, kPacket, true );
    }
    else {
        START_LOG( clog, L"자신을 제외한 방의 모든 유저 로드 완료 ")
            << BUILD_LOG( LoginID_ )
            << BUILD_LOG( kPacket.size() ) << END_LOG;

        BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, LoginID_, KUserEvent::EVENT_LOAD_COMPLETE_BROAD, kPacket, true );
    }
}

void KRoom::FailLoadingComplete( std::wstring LoginID_, DWORD UserUID_ )
{
    KEVENT_LOAD_COMPLETE_BROAD kPacket;
    KLocker lock( m_csSob );

    if ( UserUID_ != 0 ) {
        m_vecLoadFailUser.push_back( UserUID_ );
    }

    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( mit->second->GetLoadingComplete() == false &&
            mit->second->GetUID() != UserUID_ ) { // 로딩 시간 초과시 자신(나갈유저)의 UID 넣어줌.

            START_LOG( clog, L"로딩 완료 못한 유저 있음 " )
                << BUILD_LOG( mit->second->GetName() ) << END_LOG;

            return; // 아직 로딩 다 못했군 
        }
    }

    kPacket = m_vecLoadFailUser;
    m_vecLoadFailUser.clear();

    START_LOG( clog, L"자신을 제외한 방의 모든 유저 로드 완료 ")
        << BUILD_LOG( LoginID_ )
        << BUILD_LOG( kPacket.size() ) << END_LOG;

    BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, LoginID_, KUserEvent::EVENT_LOAD_COMPLETE_BROAD, kPacket, true );
}

bool KRoom::UpdateLookEquip( IN const std::wstring& strLogin_, IN const std::map<char, std::vector<KEquipItemInfo> >& mapLookEqip_ )
{
    KInDoorUserInfo* pkInDoorUser = FindInDoorUserInfo( strLogin_ );
    if ( !pkInDoorUser ) {
        return false;
    }
    std::vector< KInDoorCharInfo >::iterator vit;
    for ( vit = pkInDoorUser->m_vecCharInfo.begin() ; vit != pkInDoorUser->m_vecCharInfo.end() ; ++vit ) {
        std::map<char, std::vector<KEquipItemInfo> >::const_iterator mit;
        mit = mapLookEqip_.find( vit->m_cCharType );
        if ( mit == mapLookEqip_.end() ) {
            vit->m_vecLookEquips.clear();
        }
        else {
            vit->m_vecLookEquips= mit->second;
        }
    }
    return true;
}

bool KRoom::StartHeroModeProcess()
{
    if ( false == SiKHeroDungeonManager()->IsHeroDungeon( m_iGameMode ) ) {
        return true;
    }

    KLocker lock( m_csSob );

    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) 
    {
        if ( false == mit->second->HeroDungeonStartCheck( m_iGameMode ) ) 
        {
            START_LOG( cwarn, L"Cannot start Hero Dungeon! GameMode : " << m_iGameMode ) << END_LOG;
            return false;
        }
    }

    return true;
}

int KRoom::GetPrevDifficulty( IN const int& nModeID_, IN const int& nDifficulty_ )
{
    std::set<int> setDifficulty;
    if ( false == SiKResultManager()->GetDifficulty( nModeID_, setDifficulty ) ) {
        START_LOG( cwarn, L"해당 Mode의 난이도 정보가 없음. ModeID : " << nModeID_ ) << END_LOG;
        return (nDifficulty_ - 1);
    }

    std::set<int>::iterator sit;
    sit = setDifficulty.find( nDifficulty_ );
    if ( sit == setDifficulty.end() ) {
        START_LOG( cwarn, L"해당 Mode에 해당 난이도 정보가 없음. ModeID : " << nModeID_ << L",Difficulty : " << nDifficulty_ ) << END_LOG;
        return (nDifficulty_ - 1);
    }

    if ( setDifficulty.size() > 1 && nDifficulty_ > 0 ) {
        --sit;
    }

    return *sit;
}

void KRoom::CheckMonsterCrusaderMonLv( IN const int nModeID_, IN const int nTotalUserLevel_, IN const int nStartUserNum_, OUT int& nMonLv_ )
{
    // 몬스터 원정대 레벨 설정
    if ( nModeID_ != GC_GM_MONSTER_CRUSADER ) {
        return;
    }

    // 몬스터 원정대 MonLv = 유저 레벨 총합 / 유저수
    // 유저 최대 레벨값으로 보정.
    nMonLv_ = std::max<int>( 1, nTotalUserLevel_ ) / std::max<int>( 1, nStartUserNum_ );
    nMonLv_ = std::min<int>( nMonLv_, SiKResultManager()->GetMaxLevel() );
}

void KRoom::SetRoomHostPing( DWORD dwHostPing_ )
{
    m_dwHostPing = dwHostPing_;

    START_LOG( clog, L"방장의 핑 정보 저장" << m_dwHostPing ) << END_LOG;
}

void KRoom::CheckStageLoadCompleteUser( IN const std::wstring LoginID_, IN const DWORD dwUID_ )
{
    KEVENT_STAGE_LOAD_COMPLETE_BROAD kPacket;
    KLocker lock( m_csSob );

    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( mit->second->GetStageLoadComplete() == false ) {

            START_LOG( clog, L"스테이지 로딩 완료 못한 유저 있음, Name : " << mit->second->GetName() ) << END_LOG;
            if( SiKGSSimLayer()->GetCheckUserLoadCompleteInfo() ) {
                // 다른 유저들에게 해당 유저의 로딩 완료 정보를 보내주자.
                KEVENT_USER_STAGE_LOAD_COMPLETE_NOT kPacket_;
                kPacket_ = dwUID_;
                BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, LoginID_, KUserEvent::EVENT_USER_STAGE_LOAD_COMPLETE_NOT, kPacket_, true );
            }
            return; // 아직 로딩 다 못했군 
        }
    }

    BroadPacket( KRoom::BROAD_TO_ALL, L"", KUserEvent::EVENT_STAGE_LOAD_COMPLETE_BROAD, kPacket, true );

    SetInitStageLoad();

    SetStageLoadSwitch( false );

    m_iStageClearcount++;

    START_LOG( clog, L"방의 모든 유저 스테이지 로드 완료 ")
        << BUILD_LOG( m_iStageClearcount )
        << BUILD_LOG( kPacket.size() ) << END_LOG;
}

void KRoom::FailStageLoadComplete( std::wstring LoginID_, DWORD UserUID_ )
{
    KEVENT_STAGE_LOAD_COMPLETE_BROAD kPacket;
    KLocker lock( m_csSob );

    if ( UserUID_ != 0 ) {
        m_vecStageLoadFailUser.push_back( UserUID_ );
    }

    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( mit->second->GetStageLoadComplete() == false &&
            mit->second->GetUID() != UserUID_ ) { // 로딩 시간 초과시 자신(나갈유저)의 UID 넣어줌.

            START_LOG( clog, L"스테이지 로딩 완료 못한 유저 있음 " )
                << BUILD_LOG( mit->second->GetName() ) << END_LOG;

            return; // 아직 로딩 다 못했군 
        }
    }

    kPacket = m_vecStageLoadFailUser; // 위 for 안에 return 에 영향 받음
    //m_vecStageLoadFailUser.clear();

    SetInitStageLoad();
    BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, LoginID_, KUserEvent::EVENT_STAGE_LOAD_COMPLETE_BROAD, kPacket, true );

    SetStageLoadSwitch( false );

    START_LOG( clog, L"자신을 제외한 방의 모든 유저 스테이지 로드 완료 ")
        << BUILD_LOG( LoginID_ )
        << BUILD_LOG( kPacket.size() ) << END_LOG;

}

void KRoom::CheckFirstStageLoadComplete()
{ // 방원들 중 최초로  스테이지 로딩완료를 보냈는지 검사하는 루틴
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::iterator mit;

    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( mit->second->GetStageLoadComplete() == true ) {
            return;
        }
    }

    SetStageLoadSwitch( true ); // 스위치를 켠다    
    SetTickStageLoad();
}

void KRoom::SetTickStageLoad()
{
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::iterator mit;

    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        mit->second->InitStageLoadTick();
    }
}

void KRoom::SetInitStageLoad()
{
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::iterator mit;

    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        mit->second->InitStageLoadFlag();
    }
}

bool KRoom::FindStageLoadFailUser( DWORD dwUID_ )
{
    std::vector<DWORD>::iterator vit;
            
    vit = std::find( m_vecStageLoadFailUser.begin(), m_vecStageLoadFailUser.end(), dwUID_ );

    if ( vit == m_vecStageLoadFailUser.end() ) {
        return false;
    }
    return true;
}

void KRoom::AddMonDropGP( IN const int& nMonID_, IN const int& nTriggerID_, IN const DWORD& dwCoinType_, IN const DWORD& dwDropGP_, IN int nCount_/*=1*/ )
{
    PAIR_INT prMonKey = std::make_pair( nMonID_, nTriggerID_ );
    PAIR_INT prCoinKey = std::make_pair( dwCoinType_, dwDropGP_ );

    m_mapMonDropGP[prMonKey][prCoinKey] += std::max<int>(1,nCount_);
}

bool KRoom::IsGuildRoomFull( IN const DWORD guildUID_ )
{
    if ( m_usMaxUsers % 2 == 0 && m_usMaxUsers/2 == GetGuildMemberCount( guildUID_ ) ) { 
        SET_ERROR( ERR_ROOM_34 );
        return true; // 같은 길드 유저가 슬롯의 반을 다 채운 경우
    }

    if ( m_naGuildID[0] != 0 && m_naGuildID[1] != 0 ) {
        if ( m_naGuildID[0] != guildUID_ && m_naGuildID[1] != guildUID_ ) {
            SET_ERROR( ERR_ROOM_33 );
            return true; // 다른 길드 2개가 이미 방에 있는 경우
        }
    }

    if ( m_usMaxUsers % 2 == 1 ) {
        SET_ERROR( ERR_ROOM_34 );
        return true;
    }

    return false;
}

int KRoom::GetGuildMemberCount( IN const DWORD guildUID_ )
{
    int nMyGuildMemberCount = 0;
    KLocker lock( m_csSob );
    std::map<std::wstring, KUserPtr>::iterator mit;

    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        KNGuildInfo guildInfo_;
        mit->second->GetGuildInfo(guildInfo_);
        if ( guildInfo_.m_dwUID == guildUID_ ) {
            nMyGuildMemberCount++;
        }
    }

    return nMyGuildMemberCount;
}

bool KRoom::SetGuildRoomArray( DWORD guildUID_ )
{
    if ( m_naGuildID[0] == 0 && m_naGuildID[1] != guildUID_ ) {
        m_naGuildID[0] = guildUID_;
        return true;
    }

    else if ( m_naGuildID[1] == 0 && m_naGuildID[0] != guildUID_ ) {
        m_naGuildID[1] = guildUID_;
        return true;
    }

    return false;
}

bool KRoom::SetGuildMarkName( IN const std::wstring& guildMarkName_  )
{
    if ( m_pairGuildMarkName.first.empty() && m_pairGuildMarkName.second != guildMarkName_ ) {
        m_pairGuildMarkName.first = guildMarkName_;
        return true;
    }
    else if ( m_pairGuildMarkName.second.empty() && m_pairGuildMarkName.first != guildMarkName_ ) {
        m_pairGuildMarkName.second = guildMarkName_;
        return true;
    }
    return false;
}

bool KRoom::SetGuildName( IN const std::wstring& guildName_ )
{
    if ( m_pairGuildName.first.empty() && m_pairGuildName.second != guildName_ ) {
        m_pairGuildName.first = guildName_;
        return true;
    }
    else if ( m_pairGuildName.second.empty() && m_pairGuildName.first != guildName_ ) {
        m_pairGuildName.second = guildName_;
        return true;
    }
    return false;
}

int KRoom::GetGuildTeam( DWORD dwGuildUID_ )
{
    int team = -1;

    if ( m_naGuildID[0] == dwGuildUID_ ) {
        team = 0;
    }
    else if ( m_naGuildID[1] == dwGuildUID_ ) {
        team = 1;
    } // 길드가 두 개 일때

    if ( m_naGuildID[0] == 0 && m_naGuildID[1] != 0 && m_naGuildID[1] != dwGuildUID_ ) {
        team = 0;
    }
    else if ( m_naGuildID[0] != 0 && m_naGuildID[1] == 0 && m_naGuildID[0] != dwGuildUID_ ) {
        team = 1;
    } // 길드가 하나만 있을 때
    

    if ( m_naGuildID[0] == 0 && m_naGuildID[1] == 0 ) {
        team = 0;
    } // 길드가 방에 없을 때

    return team;
}

bool KRoom::SetGuildBattlePoint( IN const DWORD guildUID_, IN const DWORD bPoint_ )
{
    if ( m_naGuildID[0] == guildUID_ ) {
        m_pairBattlePoint.first = bPoint_;
        return true;
    }
    else if ( m_naGuildID[1] == guildUID_ ) {
        m_pairBattlePoint.second = bPoint_;
        return true;
    }
    return false;
}

bool KRoom::FindInDoorUserSameGuild( IN const DWORD dwUID_, IN const DWORD guildUID_ )
{
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for ( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit ) {
        if ( mit->second.m_dwUserUID != dwUID_ && mit->second.m_dwGuildUID == guildUID_ ) {
            return true;
        }
    }

    return false;
}

void KRoom::SetLeaveGuildWhilePlaying( IN const DWORD guildUID_ )
{
    m_dwLeaveGuildWhilePlaying = guildUID_;
}

DWORD KRoom::GetLeaveGuildWhilePlaying()
{
    return m_dwLeaveGuildWhilePlaying;
}

bool KRoom::CheckNationCode( IN const std::wstring& wstrHostCCode_, IN const std::wstring& wstrUserCCode_ )
{
    if( wstrHostCCode_ == wstrUserCCode_ ) {
        return true;
    }
    return false;
}

bool KRoom::CheckLevelUpEnable( IN const DWORD& dwUID_, OUT int& nCharType_, OUT DWORD& dwLevel_ )
{
    std::map<DWORD, std::pair<int,DWORD>>::iterator mit;
    mit = m_mapLevelUpUser.find( dwUID_ );
    if( mit == m_mapLevelUpUser.end() ) {
        return false;
    }
    nCharType_ = mit->second.first;
    dwLevel_ = mit->second.second;
    return true;
}

bool KRoom::StartDungeonTicketProcess()
{
    KLocker lock( m_csSob );

    _JIF( m_spHostUser, return false );

    KDungeonTicket kTicket;
    if ( false == SiKGCHelper()->GetDungeonTicketInfo( m_iGameMode, kTicket ) ) {
        START_LOG( clog, L"티켓 체크하는 던전이 아니면 통과.. ModeID : " << m_iGameMode ) << END_LOG;
        return true;
    }

    std::map<std::wstring, KUserPtr>::iterator mit;
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {
        if ( false == mit->second->DungeonTicketCheck( m_iGameMode ) ) {
            START_LOG( cwarn, L"던전에 필요한 티켓이 없음. ModeID : " << m_iGameMode << L", Login : " << mit->second->GetName() ) << END_LOG;
            return false;
        }
    }

    // 입장티켓 아이템 차감
    for ( mit = m_mapSob.begin() ; mit != m_mapSob.end() ; ++mit ) {

        bool bDecTicket = false;

        std::map<GCITEMID,int>::iterator mitTicket;
        for ( mitTicket = kTicket.m_mapEnterTicket.begin() ; mitTicket != kTicket.m_mapEnterTicket.end() ; ++mitTicket ) {
            const GCITEMID& TicketItemID = mitTicket->first;
            const UINT& uiNeedCount = mitTicket->second;
            UINT uiInvenCount = mit->second->GetDurItemCount( TicketItemID );

            // 수량조건 체크
            if ( uiInvenCount < uiNeedCount ) {
                continue;
            }

            if ( false == mit->second->CheckIfDurItemExist( TicketItemID, uiNeedCount, true ) ) {
                START_LOG( cerr, L"던전 입장시 체크 다 했는데 아이템이 없음.. Login : " << mit->second->GetName() )
                    << BUILD_LOG( m_iGameMode )
                    << BUILD_LOG( TicketItemID )
                    << BUILD_LOG( uiInvenCount )
                    << BUILD_LOG( uiNeedCount ) << END_LOG;
                continue;
            }

            bDecTicket = true;
            break;
        }

        _LOG_SUCCESS( bDecTicket, L"던전 입장시 입장권 감소 결과.. Result : " << bDecTicket )
            << BUILD_LOG( mit->second->GetName() )
            << BUILD_LOG( m_iGameMode ) << END_LOG;

        if ( false == bDecTicket ) {
            return false;
        }
    }

    // 보스티켓 아이템 차감
    if ( false == kTicket.m_mapBossTicket.empty() ) {
        std::map<GCITEMID,int>::iterator mitTicket;
        for ( mitTicket = kTicket.m_mapBossTicket.begin() ; mitTicket != kTicket.m_mapBossTicket.end() ; ++mitTicket ) {
            const GCITEMID& TicketItemID = mitTicket->first;
            const UINT& uiNeedCount = mitTicket->second;
            UINT uiInvenCount = m_spHostUser->GetDurItemCount( TicketItemID );

            // 수량조건 체크
            if ( uiInvenCount < uiNeedCount ) {
                continue;
            }

            if ( false == m_spHostUser->CheckIfDurItemExist( TicketItemID, uiNeedCount, true ) ) {
                START_LOG( cerr, L"보스티켓 차감실패.. Login : " << m_spHostUser->GetName() )
                    << BUILD_LOG( m_iGameMode )
                    << BUILD_LOG( TicketItemID )
                    << BUILD_LOG( uiInvenCount )
                    << BUILD_LOG( uiNeedCount ) << END_LOG;
                continue;
            }

            START_LOG( clog, L"보스티켓 차감됨.. Login : " << m_spHostUser->GetName() )
                << BUILD_LOG( m_iGameMode )
                << BUILD_LOG( TicketItemID )
                << BUILD_LOG( uiInvenCount )
                << BUILD_LOG( uiNeedCount ) << END_LOG;
            break;
        }
    }

    return true;
}

void KRoom::SetPvpLifeCount( IN const std::vector<DWORD>& vecStartUser_ )
{
    m_mapLifeCount.clear();

    if ( false == SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_PVP_CHECK, KPreHackingCheckManager::OPT_CHECK_ENABLE ) ) {
        START_LOG( clog, L"대전 목숨수 체크 안함." ) << END_LOG;
        return;
    }

    if ( m_cGameCategory != GC_GMC_MATCH ) {
        START_LOG( clog, L"대전이 아니라서 목숨수 체크하지 않음. GameCategory : " << static_cast<int>(m_cGameCategory) ) << END_LOG;
        return;
    }

    USHORT usLifeCount = 0;
    if ( false == SiKPreHackingCheckManager()->PvpLifeCountCheck( m_iMapID, usLifeCount ) ) {
        START_LOG( cwarn, L"등록되지 않은 대전 MapID : " << m_iMapID ) << END_LOG;
        return;
    }

    std::vector<DWORD>::const_iterator vit;
    for ( vit = vecStartUser_.begin() ; vit != vecStartUser_.end() ; ++vit ) {
        m_mapLifeCount[*vit] = usLifeCount;
    }

    START_LOG( clog, L"대전 유저별 목숨수 초기화.. MapID : " << m_iMapID << L", LifeCount : " << usLifeCount ) << END_LOG;
}

bool KRoom::DecreasePvpLife( IN const DWORD& dwUserUID_ )
{
    if ( m_cGameCategory != GC_GMC_MATCH ) {
        START_LOG( clog, L"대전이 아니라서 목숨수 체크하지 않음. GameCategory : " << static_cast<int>(m_cGameCategory) ) << END_LOG;
        return true;
    }

    USHORT usLifeCount = 0;
    if ( false == SiKPreHackingCheckManager()->PvpLifeCountCheck( m_iMapID, usLifeCount ) ) {
        START_LOG( cwarn, L"등록되지 않은 대전 MapID : " << m_iMapID ) << END_LOG;
        return true;
    }

    std::map<DWORD,USHORT>::iterator mit;
    mit = m_mapLifeCount.find( dwUserUID_ );
    if ( mit == m_mapLifeCount.end() ) {
        START_LOG( cwarn, L"해당 유저가 방정보에 없음. LoginID : " << dwUserUID_ << L", m_mapLifeCount.size() : " << m_mapLifeCount.size() ) << END_LOG;
        return true;
    }

    if ( mit->second == 0 ) {
        START_LOG( cwarn, L"주어진 목숨수보다 많이 죽으려고 함.. LoginID : " << dwUserUID_ << L", MaxLife : " << usLifeCount ) << END_LOG;
        return false;
    }

    --mit->second;
    return true;
}

void KRoom::GetPromotionInfo( OUT MAP_CHAR_JOB& mapPromotionInfo_ )
{
    mapPromotionInfo_.clear();

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for ( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit ) {
        if( mit->second.m_bIsObserver ) {
            continue;
        }

        if ( m_iGameMode == GC_GM_TAG_TEAM || m_iGameMode == GC_GM_TAG_SURVIVAL ) {
            // mit->second.m_acTagModeInfo[0][0] // 어떤 캐릭터인지 (캐릭터 타입)
            // mit->second.m_acTagModeInfo[0][1] // 캐릭터의 전직

            std::pair<char,char> prData1( mit->second.m_acTagModeInfo[0][0], mit->second.m_acTagModeInfo[0][1] );
            mapPromotionInfo_[mit->second.m_dwUserUID].push_back( prData1 );

            std::pair<char,char> prData2( mit->second.m_acTagModeInfo[1][0], mit->second.m_acTagModeInfo[1][1] );
            mapPromotionInfo_[mit->second.m_dwUserUID].push_back( prData2 );
        }
        else {
            std::pair<char, char> prData(mit->second.m_cCharIndex, mit->second.m_cPromotion );
            mapPromotionInfo_[mit->second.m_dwUserUID].push_back( prData );
        }
    }
}

void KRoom::GetCharLevelInfo( OUT MAP_CHAR_LEVEL& mapCharLevelInfo_ )
{
    mapCharLevelInfo_.clear();

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for ( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit ) {
        if ( mit->second.m_bIsObserver ) {
            continue;
        }

        std::pair<char, DWORD> prData(mit->second.m_cCharIndex, mit->second.m_dwLevel );
        mapCharLevelInfo_[mit->second.m_dwUserUID] = prData;
    }
}

int KRoom::GetStartUserNum()
{
    return std::max<int>( 1, m_nStartingUserNum );
}

void KRoom::UpdateSkillInfo( IN std::wstring& strLogin_, IN const char& cCharType_ )
{
    KInDoorUserInfo* pkIndoorUser = FindInDoorUserInfo( strLogin_ );
    JIF( pkIndoorUser );
    KUserPtr spUser = GetByName( strLogin_ );
    JIF( spUser );

    std::vector< KInDoorCharInfo >::iterator vit;
    vit = std::find_if( pkIndoorUser->m_vecCharInfo.begin(), pkIndoorUser->m_vecCharInfo.end(), boost::bind( &KInDoorCharInfo::m_cCharType, _1 ) == cCharType_ );
    if( vit != pkIndoorUser->m_vecCharInfo.end() ) {
        spUser->GetSkillInfoData( vit->m_cCharType, vit->m_vecSPInfo );
    }
}

void KRoom::SetAdventureUserCollectDataPrev()
{
    KLocker lock( m_csSob );

    // 게임 시작 전 유저 전리품 데이터 저장
    std::map< std::wstring, KUserPtr >::iterator mitSob;
    for( mitSob = m_mapSob.begin(); mitSob != m_mapSob.end(); mitSob++ )
    {
        mitSob->second->m_kAdventureUserData.m_mapCollectDataPrev.clear();
        mitSob->second->m_kAdventureUserData.m_mapCollectDataPrev = mitSob->second->m_kAdventureUserData.m_mapCollectData;
        START_LOG( clog, L"게임 시작 전 유저 전리품 데이터 저장. Name: " << mitSob->first ) << END_LOG;
    }
}

bool KRoom::IsRoomLastUser()
{
    if( GetCount() == 1 ) {
        return true;
    }
    return false;
}

void KRoom::AddMonsterKillInfo( IN const int& nStageID_, IN const int& nMonID_ )
{
    std::map<std::pair<int,int>, int>::iterator mit;
    std::pair<int,int> prData;
    prData.first = nStageID_;
    prData.second = nMonID_;

    mit = m_mapStageMonKillCount.find( prData );
    if( mit == m_mapStageMonKillCount.end() ) {
        m_mapStageMonKillCount[prData] = 1;
    } else {
        ++mit->second;
    }
}

void KRoom::SetDungeonDefaultInfo( IN const int& iGameMode_ )
{
    SiKDungeonManager()->GetDefaultDungeonInfo( m_iGameMode, m_kDefaultModeInfo );
    m_usMaxUsers = m_kDefaultModeInfo.m_usDefaultUser;
    switch( m_usMaxUsers ) { // 던전의 Default 인원수는 의미가 없음.
        case 1:
            {
                memset( m_abSlotOpen, false, sizeof( m_abSlotOpen) );
                m_abSlotOpen[0] = true;
                break;
            }
        case 2:
            {
                memset( m_abSlotOpen, false, sizeof( m_abSlotOpen) );
                m_abSlotOpen[0] = true;
                m_abSlotOpen[1] = true;
                break;
            }
        case 3:
            {
                memset( m_abSlotOpen, false, sizeof( m_abSlotOpen) );
                m_abSlotOpen[0] = true;
                m_abSlotOpen[1] = true;
                m_abSlotOpen[2] = true;
                break;
            }
        case 4:
            {
                memset( m_abSlotOpen, true, sizeof( m_abSlotOpen) );
                m_abSlotOpen[4] = false;
                m_abSlotOpen[5] = false;
                break;
            }
        case 5:
            {
                memset( m_abSlotOpen, true, sizeof( m_abSlotOpen) );
                m_abSlotOpen[5] = false;
                break;
            }
        default:
            {
                memset( m_abSlotOpen, true, sizeof( m_abSlotOpen) );
                break;
            }
    }
}

bool KRoom::ELOAvgRPCalc()
{
    m_prELOTeamRPAvg.swap( std::make_pair( 0, 0 ) );

    std::pair<int,int> prUserNum;
    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    std::vector< KInDoorCharInfo >::iterator vit;
    for( mit = m_mapInDoorUserInfo.begin(); mit != m_mapInDoorUserInfo.end(); mit++ ) {
        switch( mit->second.m_iTeam ) // 카나반, 세르딘 구분
        {
        case 0:
            prUserNum.second++;
            vit = std::find_if( mit->second.m_vecCharInfo.begin(), mit->second.m_vecCharInfo.end(), boost::bind( &KInDoorCharInfo::m_cCharType, _1 ) == mit->second.m_cCharIndex );
            if( vit != mit->second.m_vecCharInfo.end() ) {
                m_prELOTeamRPAvg.second += vit->m_kELOUserData.m_nRatingPoint;
            }
            break;
        case 1:
            prUserNum.first++;
            vit = std::find_if( mit->second.m_vecCharInfo.begin(), mit->second.m_vecCharInfo.end(), boost::bind( &KInDoorCharInfo::m_cCharType, _1 ) == mit->second.m_cCharIndex );
            if( vit != mit->second.m_vecCharInfo.end() ) {
                m_prELOTeamRPAvg.first += vit->m_kELOUserData.m_nRatingPoint;
            }
            break;
        }
    }

    if( prUserNum.first != prUserNum.second ) {
        SET_ERROR( ERR_ROOM_26 );       // 양 팀의 수가 동일하지 않음.
        START_LOG( cerr, NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( m_mapInDoorUserInfo.size() )
            << BUILD_LOG( prUserNum.first )
            << BUILD_LOG( prUserNum.second ) << END_LOG;
        return false;
    }

    if( m_prELOTeamRPAvg.first < 0 || m_prELOTeamRPAvg.second < 0 ) {
        SET_ERROR( ERR_ROOM_28 );       // 계산된 평균 RP 값이 이상함.
        START_LOG( cerr, NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( m_prAvgRP.first )
            << BUILD_LOG( m_prAvgRP.second ) << END_LOG;
        return false;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( prUserNum.first )
        << BUILD_LOG( prUserNum.second )
        << BUILD_LOG( m_prELOTeamRPAvg.first )
        << BUILD_LOG( m_prELOTeamRPAvg.second ) << END_LOG;
    return true;
}

int KRoom::GetFatigueRatio( IN const DWORD dwUID_ )
{
    std::map< DWORD,int >::const_iterator mit;
    mit = m_mapUserFatigueRatio.find( dwUID_ );
    
    if ( mit != m_mapUserFatigueRatio.end() ) {
        return mit->second;
    }
    return 100;
}

void KRoom::GetFatigueUserList( OUT std::map<DWORD, int>& mapFatigueUser_ )
{
    mapFatigueUser_ = m_mapUserFatigueRatio;

    std::map< DWORD, int >::const_iterator mit;
    for ( mit = m_mapUserFatigueRatio.begin(); mit != m_mapUserFatigueRatio.end(); ++mit ) {
        START_LOG( clog, L"FatigueUser UID : " << mit->first << L", Fatigue Ratio : " << mit->second ) << END_LOG;
    }
    

}
bool KRoom::CheckStartGamePossible()
{
    if( m_iGameMode != GC_GM_AUTOMATCH_TEAM ) {
        return false;
    }
    if( m_nModeInfo == KRoom::MI_PVP_1VS1 ) {
        if( ( GetCount() + m_nMatchFailCount ) == 2 ) {
            return true;
        }
        return false;
    }
    else if( m_nModeInfo == KRoom::MI_PVP_2VS2 ) {
        if( ( GetCount() + m_nMatchFailCount ) == 4 ) {
            return true;
        }
        return false;
    }
    else if( m_nModeInfo == KRoom::MI_PVP_3VS3 ) {
        if( ( GetCount() + m_nMatchFailCount ) == 6 ) {
            return true;
        }
        return false;
    }
    START_LOG( cerr,L"정의되지 않은 매칭 모드 정보" )
        << BUILD_LOG( m_nModeInfo )
        << END_LOG;

    return false;
}

void KRoom::SetMapID( IN const int& nMapID_ )
{
    m_iMapID = nMapID_;
}

void KRoom::GetModeType( OUT DWORD& dwModeType_ )
{
    // 디폴트는 던전으로 세팅.
    dwModeType_ = KRoom::MT_MODETYPE_PVE;
    if( IsDungeonMode( m_iGameMode ) == true ) {
        dwModeType_ = KRoom::MT_MODETYPE_PVE;
        return;
    }

    if( IsPvPMode( m_iGameMode ) == true ) {
        dwModeType_ = KRoom::MT_MODETYPE_PVP;
        return;
    }
}

bool KRoom::AddRemainVitalityOnStartGame( IN const DWORD dwLoginUID_, IN const int nCharType_, IN const int nRemainVitality_ )
{
    PAIR_DWORD_INT prKey( dwLoginUID_, nCharType_ );
    m_mapRemainVitalityOnStartGame[ prKey ] = nRemainVitality_;

    return true;
}

bool KRoom::GetRemainVitalityOnStartGame( IN const DWORD dwLoginUID_, IN const int nCharType_, OUT int& nRemainVitality_ )
{
    // 참고: 활력 시스템을 사용하고 활력을 소모하는 던전일 경우에만 true를 리턴한다
    // AddRemainVitalityOnStartGame() 호출시 위 조건을 검사하기 때문이다

    nRemainVitality_ = 0;

    PAIR_DWORD_INT prKey( dwLoginUID_, nCharType_ );

    std::map< PAIR_DWORD_INT, int >::iterator mitRemainVitalityOnStartGame;
    mitRemainVitalityOnStartGame = m_mapRemainVitalityOnStartGame.find( prKey );
    if ( mitRemainVitalityOnStartGame != m_mapRemainVitalityOnStartGame.end() ) {
        nRemainVitality_ = mitRemainVitalityOnStartGame->second;

        return true;
    }
    START_LOG( clog, L"룸에 유저 캐릭터의 활력 정보가 없음. dwLoginUID: " << dwLoginUID_ << ", nCharType: " << nCharType_ ) << END_LOG;

    return false;
}

void KRoom::GetUserLifeCount( IN const DWORD& dwUserUID_, OUT USHORT& usLifeCount_ )
{
    usLifeCount_ = 0;
    if ( m_cGameCategory != GC_GMC_MATCH ) {
        START_LOG( clog, L"대전이 아니라서 목숨수 체크하지 않음. GameCategory : " << static_cast<int>(m_cGameCategory) ) << END_LOG;
        return;
    }

    std::map<DWORD,USHORT>::iterator mit;
    mit = m_mapLifeCount.find( dwUserUID_ );
    if( mit == m_mapLifeCount.end() ) {
        return;
    }
    usLifeCount_ = mit->second;
}

bool KRoom::IsCompletedSubjectTrigger( IN const DWORD dwUserUID_, IN const DWORD dwTriggerID_ )
{
    // 유저가 해당 트리거를 완료하였는가? 확인하는 함수(달성 과제 시스템용)
    // true: 완료함, false: 완료하지 않음
    std::map< DWORD, std::set< DWORD > >::iterator mitCompletedSubjectTrigger;
    mitCompletedSubjectTrigger = m_mapCompletedSubjectTrigger.find( dwUserUID_ );
    if ( mitCompletedSubjectTrigger != m_mapCompletedSubjectTrigger.end() ) {
        std::set< DWORD >::iterator sitTrigger;
        sitTrigger = mitCompletedSubjectTrigger->second.find( dwTriggerID_ );
        if ( sitTrigger != mitCompletedSubjectTrigger->second.end() ) {
            return true;    // 유저 정보가 있고 트리거 리스트에도 있는 경우 완료함
        }
        else {
            return false;   // 유저 정보가 있고 트리거 리스트에 없는 경우 완료하지 않음
        }
    }

    return false;   // 유저 정보가 없는 경우 완료하지 않음
}

void KRoom::GetTotalUserUIDList( OUT std::vector<DWORD>& vecUIDs_ )
{
    vecUIDs_.clear();

    KLocker lock(m_csInDoorUserInfo);

    std::map<std::wstring, KInDoorUserInfo>::iterator mit;
    for ( mit = m_mapInDoorUserInfo.begin() ; mit != m_mapInDoorUserInfo.end() ; ++mit ) {

        vecUIDs_.push_back( mit->second.m_dwUserUID );
    }
}

bool KRoom::IncreaseMonsterKillCount()
{
    if ( m_cGameCategory != GC_GMC_DUNGEON ) {
        START_LOG( clog, L"던전 아니면 체크안함. GameCategory : " << static_cast<int>(m_cGameCategory) ) << END_LOG;
        return true;
    }

    int nMonsterCnt = 0;
    if ( false == SiKPreHackingCheckManager()->DungeonMonsterCountCheck( m_iGameMode, nMonsterCnt ) ) {
        START_LOG( cwarn, L"등록되어 있지 않는 던전 ModeID : " << m_iGameMode ) << END_LOG;
        return true;
    }

    if( m_nMonsterKillCount > nMonsterCnt ) {
        START_LOG( cwarn, L"주어진 몬스터수보다 많은 몬스터를 죽이려고 함 : " << m_iGameMode ) << END_LOG;
        return false;
    }

    m_nMonsterKillCount += 1;
    return true;
}

void KRoom::SetTRServerInfo( IN const DWORD& dwTRelayServerIP_, IN const USHORT& usTRelayServerPort_ )
{
    m_dwTRServerIP = dwTRelayServerIP_;
    m_usTRServerPort = usTRelayServerPort_;
}

void KRoom::SetPartyRewardInfo( IN std::vector<KDropItemInfo>& vecPartyReward_ )
{
    m_vecPartyReward.clear();
    m_vecPartyReward = vecPartyReward_;
}

void KRoom::SetUserUsePosition( IN const DWORD& dwUID_, IN const int& nPositionGroup_ )
{
    std::map< DWORD, std::map<int,int> >::iterator mit;
    std::map<int,int>::iterator mitPosition;
    mit = m_mapUserPositionUseCount.find( dwUID_ );
    if( mit == m_mapUserPositionUseCount.end() ) {
        m_mapUserPositionUseCount[dwUID_][nPositionGroup_] = 1;
        return;
    }

    mitPosition = mit->second.find( nPositionGroup_ );
    if( mitPosition == mit->second.end() ) {
        mit->second[nPositionGroup_] = 1;
        return;
    }

    ++mitPosition->second;
}

void KRoom::GetUserUsePosition( IN const DWORD& dwUID_, OUT std::map<int,int>& mapPositionUseCount_ )
{
    mapPositionUseCount_.clear();
    std::map< DWORD, std::map<int,int> >::iterator mit;
    mit = m_mapUserPositionUseCount.find( dwUID_ );
    if( mit == m_mapUserPositionUseCount.end() ) {
        return;
    }
    mapPositionUseCount_ = mit->second;
}

void KRoom::ClearInfinityDungeonInfoOnStartGame()
{
    m_dwIDMonsterLastSummonTime = 0;

    /* 기존 로직을 재사용하면서 충돌을 줄이기 위해
       클라이언측과 상의하여 시작 번호를 100으로 설정함 */
    m_dwIDSummonedMonsterLastUID = 100;
    m_dwIDReservedDropLastUID = 0;

    m_lstIDSummonedMonsters.clear();
    m_lstIDReservedDrops.clear();
    m_lstIDReservedGPDrops.clear();
    m_mapIDSummonedMonstersForStats.clear();
    m_mapIDPlayTime.clear();
}

void KRoom::InitIDPlayTime( IN const std::vector< DWORD >& vecStartingUsers_ )
{
    std::vector< DWORD >::const_iterator cvit;
    BOOST_FOREACH( const DWORD& rUID, vecStartingUsers_ ) {
        m_mapIDPlayTime[ rUID ] = 0;
    }
}

bool KRoom::ChangeHost(IN const KUser& kPrevHost, DWORD dwNewHostID)
{
    if (GetCount() == 0)
    {
        START_LOG_WITH_NAME(cerr)
            << BUILD_LOG(kPrevHost.GetName())
            << END_LOG;
        SET_ERROR(ERR_ROOM_12);
        return false;
    }

    KLocker lock(m_csSob);

    KUserPtr m_spCandidateHost;
    std::vector<KUserPtr>::iterator vit;
    for (vit = m_vecHostOrder.begin(); vit != m_vecHostOrder.end(); ++vit)
    {
        if ((*vit)->GetUID() == dwNewHostID)
        {
            auto mit = m_mapSob.find((*vit)->GetName());
            if (mit == m_mapSob.end())
                return false;

            m_spCandidateHost = mit->second;
            break;
        }
    }

    if (m_spCandidateHost)
    {
        KEVENT_HOST_MIGRATED_NOT kPacket;
        kPacket.m_dwNewHostUID = m_spCandidateHost->GetUID();
        kPacket.m_bLeaveHost = true;

        if (m_spPlayingHostUser)
        {
            if (m_spHostUser == m_spPlayingHostUser)
            {
                m_spHostUser = m_spCandidateHost;
                m_spPlayingHostUser = m_spCandidateHost;

                BroadPacket(BROAD_TO_ALL, kPrevHost.GetName(), KUserEvent::EVENT_HOST_MIGRATED_NOT, kPacket);

            }
            else if (m_spHostUser != m_spPlayingHostUser && m_spPlayingHostUser->GetUID() == kPrevHost.GetUID())
            {
                m_spPlayingHostUser = m_spCandidateHost;
                BroadPacket(BROAD_TO_ALL, kPrevHost.GetName(), KUserEvent::EVENT_HOST_MIGRATED_NOT, kPacket);
            }
            else if (m_spHostUser != m_spPlayingHostUser && m_spHostUser->GetUID() == kPrevHost.GetUID())
            {
                m_spHostUser = m_spCandidateHost;
            }
        }
        else
        {
            if (m_spHostUser && m_spHostUser->GetUID() == kPrevHost.GetUID())
            {
                m_spHostUser = m_spCandidateHost;
                BroadPacket(BROAD_TO_ALL, kPrevHost.GetName(), KUserEvent::EVENT_HOST_MIGRATED_NOT, kPacket);
            }
        }

        KInDoorUserInfo* pkInDoorUserInfo = FindInDoorUserInfo(m_spHostUser->GetName());
        if (pkInDoorUserInfo != NULL)
        {
            if (pkInDoorUserInfo->m_nState == GC_RUS_READY)
            {
                pkInDoorUserInfo->m_nState = GC_RUS_WAIT;
            }
            pkInDoorUserInfo->m_bIsHost = true;
        }
    }
    else
    {
        LIF(CloseRoom());
        START_LOG(cwarn, L"Troca de lider.. Size : " << GetCount()) << END_LOG;
    }

    SET_ERROR(NET_OK);
    return true;
}