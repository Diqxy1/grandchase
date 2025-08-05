#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>

struct KDicePlayRank;   // CommonPacket.h
struct KUserBoardInfo;
class KLuaManager;

typedef std::map<DWORD, DWORD> BOARD_GAME_INFO;

class KDicePlay
{
    DeclareSingleton( KDicePlay );
    NiDeclareRootRTTI( KDicePlay );
    DeclToStringW;

public:

    typedef std::vector<KDropItemInfo> VEC_REWARD;
    // 칸에 적용될 속성 정보.
    enum BOARD_FLAG
    {
        P_NORMAL            = 0,            // 일반칸
        P_ACCESSARIES       = 1,            // 악세사리 지급칸.
        P_POSITION          = 2,            // 포션 지급칸
        P_GEM               = 3,            // GEM 칸
        P_AGINGCORE         = 4,            // 비쥬 칸
        P_CHAMPION          = 5,            // 챔피언 입장권 칸
        P_CONTINENT1        = 6,            // 대륙별 상자칸1
        P_CONTINENT2        = 7,            // 대륙별 상자칸2
        P_CONTINENT3        = 8,            // 대륙별 상자칸3
        P_CONTINENT4        = 9,            // 대륙별 상자칸4
        P_SPECIALMISSION    = 10,           // 특수 미션칸
    };

public:
    KDicePlay(void);
    ~KDicePlay(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void InitProcess();
    void Tick();

    DWORD GetTotalMapCount();
    DWORD GetMaxGoal();
    DWORD GetDailyMax();
    DWORD GetDailyFree();
    DWORD GetFreeTickGap();
    DWORD GetInitHour();
    GCITEMID GetDiceItemID();
    GCITEMID GetBoardItemID();
    void GetMissionList( OUT std::vector<DWORD>& vecMissionList );     // 미션 속상칸의 미션 정보 읽어오기.
    void GetMapInfo( OUT BOARD_GAME_INFO& mapBoardMapInfo );    // 보드 게임 판 정보 읽어오기.
    bool GetMapIDType( IN DWORD& dwType, OUT DWORD& dwProperty );  // 보상 아이템 리스트.
    void GetRewardTotalList( OUT std::map<DWORD, VEC_REWARD>& mapBoardReward ); // 전체 보상 아이템 리스트 읽어오기.
    bool GetRewardInfo( IN DWORD& dwType, OUT VEC_REWARD& vecRewardInfo );  // 보상 아이템 리스트.
    bool GetGoalReward( IN DWORD& dwGoal, OUT VEC_REWARD& vecGoalReward );  // 바퀴 보상 아이템 리스트.
    void GetDicePlayNum( OUT DWORD& dwFirstNum, OUT DWORD& dwSecondNum );   // 주사위 굴리기.
    void GetDicePlayRank( OUT std::vector<KDicePlayRank>& vecRank ); // 랭킹 정보 가져오기.
    bool IsRun(); // 이벤트 진행중인지 체크.
    void PrintRank(); // 랭킹 정보 출력.

    bool SendDicePlayRankToCenter();    // GameServer -> Center 주사위 이벤트 랭킹 정보 보내기.
    void UpdateDicePlayRankFromCenter( IN std::vector<KDicePlayRank>& vecRank );    // Center -> GameServer 랭킹 정보 전송.
    void UpdateDicePlayRankPerUser( IN DWORD dwTotalCount, IN DWORD dwCurrentPosition, IN std::wstring strNick ); // 주사위 굴릴때마다 랭킹 정보 갱신.
    DWORD GetDiceEventVersion() { KLocker lock( m_csDicePlay ); return m_dwVersion; };
    bool Get_DicePlay_UserData_FromDB( IN const std::string& strScript_ );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadMission( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<DWORD>& vecMissionList );
    bool LoadTypeReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,VEC_REWARD>& mapRewardInfo );
    bool LoadGoalReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,VEC_REWARD>& mapGoalReward );
    bool LoadRewardList( IN KLuaManager& kLuaMng, OUT VEC_REWARD& vecReward ); // 보상 아이템 정보 읽어오기.
    bool LoadMapInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,DWORD>& mapBoardMapInfo );
    void SortAddNumbering( IN OUT std::vector<KDicePlayRank>& vecRank ); // 랭킹정보 최대 숫자만큼 가지고 있기.
    bool CheckUpdateFromDB();

protected:
    mutable KncCriticalSection          m_csDicePlay;
    DWORD                               m_dwToalMapCount;   // 전체 맵의 수.
    DWORD                               m_dwMaxGoal;        // 목표 바퀴수.
    DWORD                               m_dwDailyMax;       // 하루에 돌릴수 있는 최대 주사위 던지기 횟수.
    DWORD                               m_dwDailyFree;      // 하루에 무료로 지급할 주사위 갯수.
    DWORD                               m_dwTickGap;        // 무료로 주는 시간 Gap.
    DWORD                               m_dwInitHour;       // 몇시에 초기화를 진행할 것인가?
    GCITEMID                               m_dwDiceItemID;     // 주사위 아이템 ID.
    GCITEMID                               m_dwBoardItemID;    // 말판 아이템 ID

    DWORD                               m_dwLastTick;       // Tick을 위해서.
    DWORD                               m_dwSyncTickGap;    // CenterServer와의 SyncTickGap.

    DWORD                               m_dwMaxRankSize;    // 주사위 랭킹 최대 사이즈.

    time_t                              m_tmBegin;          // 주사위 게임 이벤트 시작 날짜.
    time_t                              m_tmEnd;            // 주사위 게임 이벤트 종료 날짜.

    DWORD                               m_dwVersion;        // 주사위 이벤트 버전.

    std::map<DWORD, VEC_REWARD>                     m_mapGoalReward;     // 보드판 목표설정별 보상 리스트.
    std::map<DWORD, VEC_REWARD>                     m_mapBoardReward;    // 보드판 칸별 보상 리스트.
    std::map<DWORD, DWORD>                          m_mapBoardMapInfo;   // 말판칸의 기본 속성.
    std::vector<DWORD>                              m_vecMissionList;    // 스폐셜 미션 ID List.

    std::vector<KDicePlayRank>                      m_vecDicePlayRank;  // 주사위 이벤트 랭킹 정보.
};

DefSingletonInline( KDicePlay );
DeclOstmOperatorA( KDicePlay );