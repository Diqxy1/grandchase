#pragma once

#include <map>
#include <ToString.h>
#include <KncUtil.h>    // IN OUT

class KLottery
{
    DeclToStringW;
public:
    enum {
        CASE_BLANK = -1
    };

    // 이 상태는 Decision 함수에만 영향을 주도록 합니다.
    // 넣을 때 영향을 받으면 힘들어 집니다.
    enum EState{
        ENABLE_NO_FAILURE   = 0x0000001,    // 꽝이 없습니다.
        ENABLE_AUTO_REMOVE  = 0x0000002,    // 뽑힌 Case를 자동으로 제거 합니다.
    };
    KLottery();
    ~KLottery();
    KLottery( const KLottery& t ) { *this = t; }    // copy constructor
    KLottery& operator=( const KLottery& t );       // assign operator

    float GetTotalProb() const;
    float GetRemaindProb() const { return 100.f - m_fTotalProb; }
    size_t GetCaseNum() const   { return m_mapCase.size(); }

    bool AddCase( int nCaseID, float fProbability );
    bool RemoveCase( int nCaseID );
    int Decision( float fScale = 0.f ) const;
    bool MultipleCase( int nCaseID, float fRatio );
    float GetRatio( int nIndex );

    void EnableState( EState eState )  { m_iState |= eState; }
    void DisableState( EState eState )  { m_iState &= !eState; }

protected:
    std::map<int,float>     m_mapCase;          // key : case id, element : probability
    float                   m_fTotalProb;       // 단위는 퍼센트(%)로 한다. 100이면 항상 일어남.
    int                     m_iState;           // Lottery의 옵션         
};