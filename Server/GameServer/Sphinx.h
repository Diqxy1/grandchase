#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "Kairo.h"
#include <map>
#include <vector>
#include <set>

struct KECN_SPHINX_DATA_NOT;
struct KSphinxQuestionInfo;
class KLuaManager;
class KSerBuffer;
struct KDropItemInfo;
class KSphinx
{
    DeclareSingleton( KSphinx );
    NiDeclareRootRTTI( KSphinx );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

public:
    enum RewardType 
    {
        RT_EASY                  = 1,    // 난이도 하.
        RT_NORMAL                = 2,    // 난이도 중.
        RT_HARD                  = 3,    // 난이도 상.
        RT_FAIL                  = 4,    // 문제 틀렸을 경우.
        RT_MAX,
    };

    enum AnswerType 
    {
        AT_INCORRECT             = 0,    // X선택
        AT_CORRECT               = 1,    // O선택
        AT_TIMEOVER              = 2,    // 시간초과.
        AT_MAX,
    };

    KSphinx(void);
    ~KSphinx(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun();
    void UpdateSphinxData( KECN_SPHINX_DATA_NOT& kCenterPacket );
    void UnCompressSphinxInfo( IN std::map<int, KSerBuffer>& mapCompressedSphinxQuestionInfo, OUT std::map<int,KSphinxQuestionInfo>& mapSphinxQuestionInfo );
    void GetUnSolvedQuestion( IN const std::vector<int>& vecSolved, OUT std::vector<int>& vecUnSolved );
    bool GetRandomQuestionID( IN std::vector<int>& vecCorrectList, OUT int& nQuestionID );
    bool GetRandomQuestion( IN std::vector<int>& vecCorrectList, OUT KSerBuffer& kQuestion, OUT int& nQuestionID );
    bool GetCompressedQuestion( IN const int nQuestionID, OUT KSerBuffer& kQuestionBuff );
    bool GetAccumulateReward( IN const int nRightAnswerCount, OUT VEC_ITEMS& vecReward );
    bool GetKairoLevelReward( IN const int nLevelType, OUT KDropItemInfo& kItem );
    void GetDefaultAccumulateReward( IN const int nRigthAnswerCnt, IN int nLoopRightAnswerCnt, OUT VEC_ITEMS& vecReward );
    bool GetSphinxTimeValue( IN const int nUID, OUT int& nSecond );
    void GetRewardList( OUT std::map<int, std::set<int>>& mapRewardList );
    GCITEMID GetSphinxCoinID(){ KLocker lock( m_csSphinx ); return m_dwSphinxCoinID; };
    int GetSphinxVersion(){ KLocker lock( m_csSphinx ); return m_nVersion; };
    bool GetQuestion( IN const int& nQuestionID, OUT KSphinxQuestionInfo& kQuestion );
    int GetTotalQuestionNum(){ KLocker lock( m_csSphinx ); return m_nMaxRigthAnswerCnt; };
    int GetLoopRightAnswerCnt(){ KLocker lock( m_csSphinx ); return m_nLoopRightAnswerCnt; };
    void CheckAccumulateReward( IN const int& nRightAnswerCnt, OUT VEC_ITEMS& vecReward );

protected:
    bool LoadTimeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, int>& mapLevelInfo );
    bool LoadAccumulateReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, VEC_ITEMS >& mapAccumulateReward );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_ITEMS& vecItems );
    bool LoadLevelReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, Kairo >& mapKairoLevelReward );
    bool LoadLevelRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kairo );
    void CreateRewardList( IN std::map<int, VEC_ITEMS >& mapAccumulateReward, OUT std::map<int, std::set<int>>& mapRewardList );
    void CheckMaxCorrectQuestion( IN OUT std::vector<int>& vecCorrectList );

protected:
    mutable KncCriticalSection  m_csSphinx;

    time_t                      m_tmBegin;  // 이벤트 시작 시간 센터서버로 부터 얻음
    time_t                      m_tmEnd;    // 이벤트 종료 시간 센터서버로 부터 얻음

    int                         m_nVersion;             // 이벤트 버전.
    GCITEMID                    m_dwSphinxCoinID;       // 이벤트 코인 ItemID.
    int                         m_nMaxRigthAnswerCnt;   // 최대 목표 누적 레벨값.
    int                         m_nLoopRightAnswerCnt;  // 반복 누적의 폭에 대한 변수값.
    std::map<int,KSphinxQuestionInfo>   m_mapSphinxQuestionInfo;
    std::map<int,KSerBuffer>            m_mapCompressedSphinxQuestionInfo;
    std::map<int, VEC_ITEMS >           m_mapAccumulateReward;   // 누적보상.
    std::map< int, Kairo >              m_mapKairoLevelReward;   // 난위도에 따른 보상.
    std::vector<int>                    m_vecQuestionIDs; // 질문 ID 목록< Level, std::vector<QuestionID> >
    VEC_ITEMS                           m_vecDefaultAccumulateReward;   // 목표 보상 이후의 반복보상.
    std::map<int,int>                   m_mapSphinxTimeValue;  // 스핑크스 난이도별 시간 정보.
    std::map<int, std::set<int>>        m_mapRewardList; // 클라이언트 전달 누적 보상 리스트.
    std::map<int,int>                   m_mapQuestionLevel; // < UID, Level >
};
DefSingletonInline( KSphinx );
DeclOstmOperatorA( KSphinx );

//=========================================================================================//

class KSphinxUserData
{
public:
    KSphinxUserData();
    ~KSphinxUserData();
    bool IsLoaded() { return m_bLoaded; };
    bool IsCompleted() { return m_bComplete; };
    std::vector<int>& GetCorrectQuestions() { return m_vecCorrectQuestions; };
    void AddCorrectQuestion( IN const int nQuestionID, IN const int nTotalQuestionNum );
    void SetUserSphinxData( IN const int& nRigthAnswerCnt, IN const std::vector<int>& vecCorrectQuestions, IN const int nTotalQuestionNum );
    int GetRightAnswerCount(){ return m_nRigthAnswerCnt; };

protected:
    bool                m_bLoaded;
    bool                m_bComplete; // 문제를 한번 다 풀었는지 체크.
    std::vector<int>    m_vecCorrectQuestions;
    int                 m_nRigthAnswerCnt; // 누적 맞춘 갯수.
};