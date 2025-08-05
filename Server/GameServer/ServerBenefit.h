#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <set>
// 레벨별 서버 혜택.
class KServerBenefit
{
    DeclareSingleton( KServerBenefit );
    NiDeclareRootRTTI( KServerBenefit );
    DeclToStringW;
public:
    KServerBenefit(void);
    ~KServerBenefit(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void SetLevelRange( IN std::pair<int,int>& prLvRange_ );
    bool IsCorrectLvRange( IN const int nCharLevel_ );
    float GetConditionBenefit();
    bool IsCorrectCondition( IN const int nConditionID_ );
    float GetDropBenefit();

private:

private:
    mutable KncCriticalSection  m_csServerBenefit;
    std::pair<int,int>  m_prLvRange;

    std::set<int>       m_setConditionIDs;
    float               m_fConditionBenefit;
    float               m_fDropBenefit;
};

DefSingletonInline( KServerBenefit );
DeclOstmOperatorA( KServerBenefit );