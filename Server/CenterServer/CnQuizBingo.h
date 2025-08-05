#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>

struct KBingoQuestionInfo;
struct KECN_QUIZ_BINGO_DATA_NOT;
class KSerBuffer;
class KCnQuizBingo
{
    DeclareSingleton( KCnQuizBingo );
    NiDeclareRootRTTI( KCnQuizBingo );
    DeclToStringW;

public:
    struct SBingoConfigData
    {
        int     m_nEventID;
        time_t  m_tmBegin;
        time_t  m_tmEnd;
        std::map<int,KBingoQuestionInfo> m_mapQuestions;
        bool m_bChanged;
    };

public:
    KCnQuizBingo(void);
    ~KCnQuizBingo(void);

    void Tick();
    bool CheckChange();
    bool CheckTick();
    bool VerifyQuestions( IN const std::map<int,KBingoQuestionInfo>& mapQuestions );
    void UpdateBingoConfig( KCnQuizBingo::SBingoConfigData& bingoConfig );
    void GetQuizBingoData( OUT KECN_QUIZ_BINGO_DATA_NOT& kPacket );

private:
    void CompressQuestions( IN const std::map<int,KBingoQuestionInfo>& mapQuestions );

private:
    mutable KncCriticalSection  m_csBingo;
    bool                        m_bChanged; // Tick에서 확인 해서 변경사항 있으면 Send 구리고 리셋
    DWORD                       m_dwTickGap;
    DWORD                       m_dwLastTick;

    int                         m_nEventID;
    time_t                      m_tmBegin;
    time_t                      m_tmEnd;

    std::map<int,KBingoQuestionInfo>    m_mapQuestionList;
    std::map<int,KSerBuffer>            m_mapCompressedQuestionList;

};
DefSingletonInline( KCnQuizBingo );
DeclOstmOperatorA( KCnQuizBingo );