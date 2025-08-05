#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include "PrimitiveTypedef.h"

struct KECN_QUIZ_BINGO_DATA_NOT;
struct KBingoQuestionInfo;
class KLuaManager;
class KSerBuffer;
struct KDropItemInfo;
class KQuizBingo
{
    DeclareSingleton( KQuizBingo );
    NiDeclareRootRTTI( KQuizBingo );
    DeclToStringW;
public:
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

public:
    KQuizBingo(void);
    ~KQuizBingo(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsEventTerm()  { KLocker lock(m_csBingo); return m_bIsEventTerm; }
    void UpdateBingoConfig( KECN_QUIZ_BINGO_DATA_NOT& kCenterPacket );
    int GetEventID() { KLocker lock(m_csBingo); return m_nEventID; }
    bool CheckCoinRecvable( IN const int nAccPlayTime );
    GCITEMID GetBingoBoardID() { KLocker lock(m_csBingo); return m_dwBingoBoardID; }
    GCITEMID GetBingoCoinID() { KLocker lock(m_csBingo); return m_dwBingoCoinID; }
    GCITEMID GetResurrectionID() { KLocker lock(m_csBingo); return m_dwResurrectionID; }
    void GetUnSolvedQuestion( IN const std::vector<int>& vecSolved, OUT std::vector<int>& vecUnSolved );
    bool GetRandomQuestionID( IN const std::vector<int>& vecCorrectList, OUT int& nQuestionID );
    bool GetRandomQuestion( IN const std::vector<int>& vecCorrectList, OUT KSerBuffer& kQuestion, OUT int& nQuestionID );
    bool GetQuestion( IN const int nQuestionID, OUT KBingoQuestionInfo& kQuestion );
    bool GetCompressedQuestion( IN const int nQuestionID, OUT KSerBuffer& kQuestionBuff );
    void GetLineBingReward( IN const std::vector<int>& vecLineBingo, IN OUT VEC_ITEMS& vecRewards );
    bool GetLineBingoReward( IN const int nBingoLineCount, IN OUT VEC_ITEMS& vecRewards );
    void GetCorrectQuizReward( IN OUT VEC_ITEMS& vecRewards );
    int GetCoinSupplyInterval() { KLocker lock(m_csBingo); return m_nCoinSupplyInterval; }
    int GetBoardSize() { KLocker lock(m_csBingo); return m_nBoardSize; }
    int GetCoinSupplyCount() { KLocker lock(m_csBingo); return m_nCoinSupplyCount; }
    void Tick();

private:

    bool LoadBingoReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, VEC_ITEMS >& mapBingoReward );
    //bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_ITEMS& vecItems );
    void UnCompressQuiz( IN std::map<int, KSerBuffer>& vecCompressedQuestionz, OUT std::map<int,KBingoQuestionInfo>& mapQuestions );
    void UpdateEventTerm();
    bool CheckUpdateTick();

private:
    mutable KncCriticalSection  m_csBingo;
    DWORD               m_dwTickGap;
    DWORD               m_dwLastTick;

    int                 m_nEventID;
    time_t              m_tmBegin;  // 이벤트 시작 시간 DB로 부터 얻음
    time_t              m_tmEnd;    // 이벤트 종료 시간 DB로 부터 얻음
    bool                m_bIsEventTerm;

    int                 m_nBoardSize;  // 빙고판 사이즈

    GCITEMID            m_dwBingoBoardID;
    GCITEMID            m_dwResurrectionID; // 부활 아이템 ID
    GCITEMID            m_dwBingoCoinID;    // 빙고 코인 ID

    int                 m_nDailyMaxCoinCount;   // 하루 받을수 있는 최대 코인수
    int                 m_nCoinSupplyInterval;  // 코인 지급 받는 간격 ( 분 )
    int                 m_nCoinSupplyCount;

    std::map<int, VEC_ITEMS >   m_mapBingoReward;   // 빙고시에 보상
    VEC_ITEMS                   m_vecCorrectQuizReward; // 문제 맞추었을때의 보상.
    std::map<int,KBingoQuestionInfo>    m_mapQuestions; // 질문지
    std::map<int, KSerBuffer>           m_mapCompressedQuestionList; // 압축된 질문
    std::vector<int>                    m_vecQuestionIDs; // 질문 ID 목록
};

DefSingletonInline( KQuizBingo );
DeclOstmOperatorA( KQuizBingo );

//=========================================================================================//

struct KEVENT_USER_BINGO_DATA_ACK;
struct KSimpleDate;
struct KEVENT_BINGO_COIN_CHARGE_NOT;
class KBingoUserData
{
public:
    KBingoUserData();
    ~KBingoUserData();
    bool IsLoaded() { return m_bLoaded; }
    void SetBingoData( IN const KEVENT_USER_BINGO_DATA_ACK& kData );
    int GetAccTime() { return m_nAccPlayTime; }
    int GetCalcedAccTime();
    bool IsFinishedToday() { return m_bFinishToday; }
    int GetCurrentBingoCoinCount();
    int GetTotalBingoCount() { return m_nTotalCoinCount; }
    void SetTotalBingoCount( int nTotalCoinCount ) { m_nTotalCoinCount = nTotalCoinCount; }
    int GetUseBingoCount() { return m_nUseCoinCount; }
    void SetUseBingoCount( int nUseCoinCount ) { m_nUseCoinCount = nUseCoinCount; }
    std::vector<int>& GetCorrectQuestions() { return m_vecCorrectQuestions; }
    bool IsInBingoData( IN const std::pair<int,int>& prPos );
    void GetCalculatedBingoLine( IN const std::pair<int,int>& prPos, OUT std::vector<int>& vecLineBingo );
    int CalcTotalBingoLine();
    bool CheckBingoVirtical( IN const int nXPos );
    bool CheckBingoHorizon( IN const int nYPos );
    int CheckBingoDiagonalLine( IN const std::pair<int,int>& prPos );
    void AddPos( IN const std::pair<int,int> prPos );
    void AddBingoLineCount( IN const int nCount );
    void AddCorrectQuestion( IN const int nQuestionID );

    void ResetLastFail();
    void SetLastFail( IN const int nQuestionID, IN const std::pair<int,int>& prLastFailPos );
    bool GetLastFail( OUT int& nQuestionID, OUT std::pair<int,int>& prLastFailPos );

    void IncreaseAccTime();
    bool CheckRecvBingoCoin();
    void ResetAccTime();
    void UpdateFinishToday();
    bool UpdateAccTime( IN const DWORD dwUID, IN const std::wstring& strLogin, IN const char cCharType_, IN const bool bChangeDate, IN const KSimpleDate& kToday, OUT KEVENT_BINGO_COIN_CHARGE_NOT& kUserPacket );

private:
    bool                                m_bLoaded;
    std::vector<int>                    m_vecCorrectQuestions;
    std::vector<std::pair<int,int> >    m_vecBingoData;
    int                                 m_nAccPlayTime;
    bool                                m_bFinishToday;
    int                                 m_nUseCoinCount;
    int                                 m_nTotalCoinCount;
    int                                 m_nCurrentBingoLineCount;
    int                                 m_nLastFailQuistionID;
    std::pair<int,int>                  m_prLastFailPos;

};