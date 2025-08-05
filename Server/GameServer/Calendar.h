#pragma once

#include <atltime.h>
#include <vector>

struct KDailyInfo; //전방 선언.

class KCalendar
{
public:

#define CLEAR_MASK 0xF8
#define OVER_MASK  0x07

    enum eSEAL_TYPE
    {
        ST_TRI = 0x01, //'1일 1시간미만' (세모)
        ST_SQU = 0x02, //'1일 1시간이상 출석체크안함' (네모)
        ST_CIR = 0x04, //'1일 1시간이상 출석체크했음' (동그라미)

        ST_STA = 0x08, //'1주 주간출석완료' (별)
        ST_HEA = 0x10, //'1달 월간출석완료' (하트)
        ST_LUC = 0x20, //'행운의 날' (Lucky day) 
        ST_MAX
    };

    enum UPDATE_RETURN
    {
        UR_NORMAL   = 0x01,  // 특이사항 없음.
        UR_RELOAD   = 0x02,  // 달력을 새달력으로 바꿔야 할 때 
        UR_NEXTDAY  = 0x04,  // 게임 중 다음날이 되었을 때 
        UR_GETFAIL  = 0x08,  // 출석부 정보 못가져 왔을 경우..,
        UR_MAX      = 0xFF,
    };

    KCalendar(void);
    ~KCalendar(void);

    bool LoadDailyInfo( IN std::vector<KDailyInfo>& vecCalendarInfo );
    void GetCalendarInfo( OUT std::map<char,std::vector<KDailyInfo> >& mapCalendarInfo );
    bool IsLoaded() { return m_bLoaded; } // 로드 되었느냐?
    bool SetSealling( IN int nMonthDay, OUT KDailyInfo& kDailyInfo );
    bool SetWeeklyAttendance( char cWeekID, char& cMonth );
    bool SetMonthlyAttendance();
    bool GetCalendarDBInfo( OUT std::vector<KDailyInfo>& vecCalendarInfo );
    void SetAccTime( int nAccTime );
    int GetAccTime(){ return m_nAccTime; };
    void GetMonthInfo( IN const int nYear, IN const int nMonth, OUT KSimpleDate& kStartDate, OUT KSimpleDate& kLasttDate );
    int GetWeekNumber( int nYear, int nMonth, int nDay ); // 해당 날짜가 몇주차에 속하는지.
    void GetToday( KSimpleDate& kToday );
    int Update(); // 로그를 위해서 해당 유저의 로긴 아이디를 파라미터로 넘긴다.
    void SetOwnerLogin( std::wstring& strOwnerLogin );
    bool CheckLuckyDay( IN char cToday, OUT char& cSealType ); // 출첵한 날이 행운의 날인지 검사
    void Dump();

protected:
    void UpdateSeal( IN char cToday );
    int GetWeekDay( IN const int nYear, IN const int nMonth, IN const int nDay ); // 요일 얻기.
    int CalcWeekNumber( int nYear, int nMonth ); // 해당 달의 주차 총 개수
    int GetLastDay( int nYear, int nMonth );
    bool AddNewDay( IN int nYear, IN int nMonth, IN int nDay );
    
protected:
    CTime   m_tmStartTime;  // 로그인 한 날짜 정도.
    int     m_nAccTime;     // 일단위 누적 접속 시간.(단위 분)
    CTime   m_tmCurrent;    // 현재 시각.

    bool    m_bLoaded;      // 달력 사용 가능한가?
    std::map<char, std::vector<KDailyInfo> >    m_mapCalendarInfo; //<1,2,3,4,5,6주 차, 주 정보>

    std::wstring m_strOwnerLogin; // 이 달력 소유자의 로긴 ID
};