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

    // �� ���´� Decision �Լ����� ������ �ֵ��� �մϴ�.
    // ���� �� ������ ������ ����� ���ϴ�.
    enum EState{
        ENABLE_NO_FAILURE   = 0x0000001,    // ���� �����ϴ�.
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
    bool GetMaxCaseID( IN int nExceptCaseID, OUT int& nMaxCaseID );
    bool DecreaseCase( IN int nCaseID, IN float fRatio ); // Ư�� Type Ȯ���� ����.
    bool IncreaseCase( IN const int& nCaseID, IN OUT float& fRatio );
	void Clear();

protected:
    std::map<int,float>     m_mapCase;          // key : case id, element : probability
    float                   m_fTotalProb;       // ������ �ۼ�Ʈ(%)�� �Ѵ�. 100�̸� �׻� �Ͼ.
    int                     m_iState;           // Lottery�� �ɼ�         
};