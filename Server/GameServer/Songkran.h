#pragma once
#include <map>
#include <set>
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "PrimitiveTypedef.h"
#include "CommonPacket.h"
#include "UserPacket.h"

SmartPointer(KUser);
class KLuaManager;
struct KItemUnit;

class KSongkran
{
    DeclareSingleton( KSongkran );
    NiDeclareRootRTTI( KSongkran );
    DeclToStringW;
public:
    typedef std::vector<KDropItemInfo> VEC_REWARD;

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun(); // 송크란 이벤트 진행중인지 체크.
    void GetSongkranInfo( OUT KEVENT_SONGKRAN_SCRIPT_INFO_ACK& kPacket ); // 스크립트에서 로드한 설정들을 패킷에 담아준다
    DWORD GetVersion(){ KLocker lock( m_csSongkran ); return m_dwVersion; }; 
    DWORD GetMaxGrade(){ KLocker lock( m_csSongkran ); return m_dwMaxGrade; };
    DWORD GetWaterBombSupplyDuration() { KLocker lock( m_csSongkran ); return m_dwWaterBombSupplyDuration; };
    int GetWaterBombSupplyInterval() { KLocker lock( m_csSongkran ); return m_nWaterBombSupplyInterval; };
    DWORD GetDailyMaxSupplyDuration() { KLocker lock( m_csSongkran ); return m_dwDailyMaxSupplyCount; }; // 일일 지급 수량
    DWORD GetOneTimeMaxScore() { KLocker lock( m_csSongkran ); return m_dwOneTimeMaxScore; }; // 한번 요청에 획득 가능한 최대 스코어 제한값
    void GetCurGradeEndScore( IN DWORD& dwGrade, OUT DWORD& dwEndScore ); // dwGrade의 end point
    bool GetGradeRewardList( IN const DWORD dwGrade, OUT VEC_REWARD& vecReward ); // dwGrade의 보상
    GCITEMID GetWaterBombItem(){ KLocker lock( m_csSongkran ); return m_WaterBombItem; }; // 물풍선 교환 캐시 아이템 ID얻는다.
    DWORD GetDailySupplyDuration() { KLocker lock( m_csSongkran ); return m_dwDailySupplyDuration; };
    void GetUserScoreGrade( IN const DWORD& dwScore_, OUT DWORD& dwGrade_ );
    bool Get_Songkran_UserData_FromDB( IN const std::string& strScript_ );

public:
    KSongkran(void);
    ~KSongkran(void);

    // script load functions
protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadTargetInfoList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<USHORT, KSongkranEventTargetInfo>& mapPresentItemInfo );
    bool LoadDropItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT KDropItemInfo& kDropItemInfo );
    bool LoadGradeRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, IN DWORD& dwMaxGrade_, OUT std::map<DWORD, KSongkranEventRewardInfo>& mapGradeRewardList_ );
protected:
    mutable KncCriticalSection m_csSongkran;

    time_t              m_tmBegin;                  // 이벤트 시작 날짜
    time_t              m_tmEnd;                    // 이벤트 종료 날짜

    DWORD               m_dwVersion;                // 이벤트 버전 ( 몇 번째 진행인가 )
    DWORD               m_dwMaxGrade;               // 최대 아이템 보상 단계
    DWORD               m_dwMaxScore;               // 최대 획득 가능한 스코어
    float               m_fPlayerMoveSpeed;         // 유저 캐릭터 이동 속도
    float               m_fWaterBombFylingSpeed;    // 물풍 비행 속도
    float               m_fWaterBombRange;          // 물풍 폭발 범위
    GCITEMID            m_WaterBombItem;            // 물풍선 캐시템ID
    DWORD               m_dwDailyMaxSupplyCount;    // 하루 받을수 있는 최대 물풍 갯수 필요하려나? 기획 문의 필요
    int                 m_nWaterBombSupplyInterval; // 지급 받는 간격 ( 분 )
    DWORD               m_dwWaterBombSupplyDuration;// 지급 개수
    DWORD               m_dwExchangeDuration;       // 캐쉬 아이템 사용 시 충전 해 주는 물풍 개수
    DWORD               m_dwDailySupplyDuration;    // 접속 시 지급 하는 개수
    DWORD               m_dwOneTimeMaxScore;        // 한번에 획득 가능한 스코어 제한 값

    std::map< USHORT, KSongkranEventTargetInfo > m_mapTargetInfoList;  // 타겟 정보 맵
    std::map<DWORD, KSongkranEventRewardInfo>    m_mapGradeRewardList; // 보상 정보 맵
};

DefSingletonInline( KSongkran );
DeclOstmOperatorA( KSongkran );

class KSongkranUserData
{ 
public:
    KSongkranUserData();
    ~KSongkranUserData();
    bool IsLoaded() { return m_bLoaded; };
    bool IsFinishedToday() { return m_bFinishToday; }
    void SetFinishToday( IN bool bFinishToday ) { m_bFinishToday = bFinishToday; };
    DWORD GetUserGrade() { return m_dwGrade; };
    void IncreaseGrade();
    DWORD GetUserScore() { return m_dwScore; };
    int GetSuppliedWaterBombDuration() { return m_dwSuppliedWaterBombDuration; };
    void SetSuppliedWaterBombDuration( int nDuration ) { m_dwSuppliedWaterBombDuration = std::max<int>( 0, nDuration ); };
    int GetUserWaterBombDuration() { return m_dwWaterBombDuration; };
    int GetUserInitWaterBombDuration() { return m_dwInitWaterBombDuration; };
    void SetUserWaterBombDuration( IN DWORD dwWaterBombDuration ) { m_dwWaterBombDuration = std::min<DWORD>( ULONG_MAX, dwWaterBombDuration ); };
    void SetUserInitWaterBombDuration( IN DWORD dwWaterBombDuration ) { m_dwInitWaterBombDuration = std::min<DWORD>( ULONG_MAX, dwWaterBombDuration ); };
    void SetSongkranUserEventData( IN DWORD dwGrade, IN DWORD dwScore, IN int nAccPlayTime, IN DWORD dwWaterBombDuration, IN DWORD dwSupplyiedWaterBombDuration ); // 처음 접속 했을 때 세팅
    void GetSongkranUserEventDate( OUT KEVENT_SONGKRAN_USER_INFO_ACK& kPacket );
    void UseWaterBomb( IN DWORD dwScoreDiff ); // 스코어 올리고 개수 깎고
    void GetSongkranCachingData( OUT KSongkranEventUserData& kData );

    // 10분 충전 추가
    int GetAccTime() { return m_nAccPlayTime; }
    void IncreaseAccTime( IN const DWORD dwUID, IN const std::wstring& strLogin, IN const bool bChangeDate );
    void ResetAccTime();
    void UpdateFinishToday();
    bool CheckTodayWaterBombRecvable( IN const int nAccPlayTime );
    bool UpdateAccTime( IN const DWORD dwUID, IN const std::wstring& strLogin, IN const bool bChangeDate, IN const KSimpleDate& kToday, OUT KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT& kNotPacket );

protected:
    bool  m_bLoaded;
    bool  m_bFinishToday;               // 오늘 물풍 지급이 끝났는지
    DWORD m_dwInitGrade;                 // 보상 단계 (DB에서 받아옴)
    DWORD m_dwGrade;                     // 보상 단계 (DB 기록할 때 위에꺼랑 Diff값 기록 함)   
    DWORD m_dwInitScore;                 // 총 획득 스코어 (DB에서 받아옴)
    DWORD m_dwScore;                     // 획득 스코어 (grade랑 마찬가지)
    int   m_nAccPlayTime;                // 누적 시간 (물풍 충전까지 남은 시간 계산용)
    DWORD m_dwWaterBombDuration;         // 인벤토리의 물풍선 수량 (DB log 저장에도 쓰임)
    DWORD m_dwInitWaterBombDuration; 
    DWORD m_dwSuppliedWaterBombDuration; // 오늘 지급 한 개수 (DB log 저장에도 쓰임)
    DWORD m_dwUseCount;                  // 물풍 사용 개수 (DB log 저장 전용 데이터)
};