#pragma once

#include <KNCSingleton.h>
#include <RTTI.h>
#include <ToString.h>
#include <Thread\Locker.h>
#include <map>
#include <vector>
#include <set>
#include "PrimitiveTypedef.h"

struct KFeedFactor
{
    int m_iItemID;
    int m_iFeedFactor;
};

struct KPetExpInfo
{
    int m_nLv;
    int m_nLose;
    int m_nWin;
    int m_nTotalExp;
};

struct KAssistFactor
{
    int     m_iItemID;
    float   m_fExpAssist;
};

struct KPanaltyFactor
{
    int     m_iLimitFeed;
    float   m_fPanaltyFactor;
};

struct KTransformLv // 펫진화 아이템의 상,하한 레벨 제한
{
    int     m_iEvolutionType; // Evoplution : 0, Dwvolution : 1
    int     m_iMinLevel;
    int     m_iMaxLevel;
};

struct KHatchingInfo
{
    int     m_nItemID; // 알 아이템 ID
    int     m_nMinLv; // 부화하기 위한 최소 레벨
    int     m_nNextID; // 부화 했을때 펫..
};

struct KPetBonusInfo
{
    int     m_nPetID;			// 이벤트 대상 펫Item ID (-1:검사안함)
    int     m_nPromotion;		// 펫 진화상태(0:기본,1:진화)
    float   m_fExpBonusRatio;	// 경험치 보너스률(ex.+100Exp시, 1.0으로 설정)
};

class KPetHelper
{
    DeclareSingleton( KPetHelper );
    NiDeclareRootRTTI( KPetHelper );
    DeclToStringW;
public:
    KPetHelper(void);
    ~KPetHelper(void);

    //짐승 밥... 이녀석을 DB로 옮기고 서버 10분 마다 퍼오는게 좋을듯 하다.. 대략 x... 
    void AddPetFeed( IN const int iItemID, IN const int iFeedFactor );
    bool GetPetFeedFactor( IN int iItemID, OUT int& iFeedFactor );
    void ClearPetFeed();
    void PrintPetFeed();

    // 짐승 Exp 관련
    void AddExpInfo( IN const int nLv, IN const int nWin, IN const int nLose, IN const int nTotalExp );
    void SortExpInfo();
    void ClearExpInfo();
    int GetPetLevel( IN int nTotalExp );
    int GetRewardExpByLv( IN const int nLevel_, IN const bool bWin_, IN const float fRewardRatio_, IN const DWORD dwPetID_, IN const char cPetPromotion_, IN const bool bPCBang_ );
    bool IsMaxLevel( IN DWORD dwExp );
    bool GetInitExpInfo( OUT KPetExpInfo& kPetExpInfo );
    bool GetMaxExpInfo( OUT KPetExpInfo& kPetExpInfo );
    

    // 2007/1/11. iridology. 몬스터 원정대, 몬스터 대전 보상
    int GetMonsterCrusadersRewardExp( IN const int nPetLv, IN const bool bWin, IN int nKillNum, IN const float fRewardRatio, IN const DWORD dwPetID, IN const char cPetPromotion, IN const bool bPCBang );
    int GetMonsterHuntRewardExp( IN const int nPetLv, IN const bool bWin, IN const int nDifficulty, IN const float fRewardRatio, IN const DWORD dwPetID, IN const char cPetPromotion, IN const bool bPCBang );

    // 짐승 허기 계산 관련.
    void AddDecreseRate( IN const int nPlayMin, IN const int nDecreseSatiation );
    void ClearDecreseTable();
    int GetDecreseSatiation( IN int nPlayTime ); // 단위 Min. 음수를 반환하니까 그냥 더하는것이다..
    void PrintDecreseSatiation();

    // Gp, Exp 도움 관련
    void ClearAssistItemTable();
    void PrintAssistItemTable();
    void AddAssistItem( IN const int iItemID_, IN const float fExpAssistFacor_/*, IN const float fGpAssistFactor*/ );
    bool GetPetExpAssistFactor( IN int iItemID_, OUT float& fExpAssistFacor );
    DWORD GetPetMinExp();
    
    // 만복도 패널티 & 최대치 설정
    void SetMaxFeed( IN const int iMaxFeed );
    int GetMaxFeed() const { KLocker lock( m_csMaxFeed ); return m_iMaxFeed; }
    void ClearExpPanaltyTable();
    void AddExpPanalty( IN const int iFeed, IN const float fPanalty );
    void PrintExpPanaltyTable();
    void SortExpPanaltyTable();
    float GetExpPanalty( IN const int iFeed ) const;

    // 진화에 따른 경험치 패널티.
    void ClearPromotionPanaltyTable();
    void AddPromotionPanalty( const char cPromotion, const float fPanaltyRate );
    float GetPromotionPanalty( const char cPromotion );
    void PrintPromotionPanaltyTable();

    // 진화, 퇴화 아이템 관련
    void AddTransformItem( IN int iItemID_, IN int iMinLv, IN int iMaxLv, IN int iEvolutionType_ );
    int GetEvolutionType( IN int iItemID );
    bool GetTransformLv( IN int iItemID_, OUT int& iTransformLv_ );
    void ClearTransformItem();

    // 알부화
    void AddHatchingInfo( const GCITEMID nItemID, const int nMinLv, const int nNextID );
    void ClearHatchingInfo();
    int GetNextID( GCITEMID nItemID );
    void PrintHatchingInfo();
    bool IsEggPet( IN GCITEMID nItemID );
    int GetHatchingLv( IN GCITEMID nItemID );

    // 펫 경험치 배율
    void SetPetExpBonusInfo( IN const std::vector< KPetBonusInfo >& vecPetBonus_ );
    float GetPetExpBonusRatio( IN const DWORD dwPetID, IN const int nPromotion );
    void DumpPetExpBonusInfo();

    // pet 코스튬 확인
    void ClearPetCostume();
    void AddPetCostume( int nPetID, int nPetPromotion, int nPetCostumeID );
    bool IsPetCostume( DWORD dwPetID, char petPromotion, DWORD dwPetCostumeID );
    void GetCoustumeList( std::map<std::pair<DWORD,char>, std::set<DWORD> >& mapPetCostume );

    // pet 귀속 아이템
    void ClearPetVestedItem();
    void AddPetVestedItem( IN GCITEMID ItemID );
    void GetPetVestedItem( OUT std::vector<GCITEMID>& vecPetVestedItem );
    bool CheckPetVestedItem( IN const GCITEMID& ItemID );

    void DumpCostume( std::map<std::pair<DWORD,char>, std::set<DWORD> >& mapPetCostume );

protected:
    mutable KncCriticalSection      m_csPetFeedTable;   // 070107. kkurrung. Feed Table 보호
    std::map<int, KFeedFactor>      m_mapPetFeedTable;  // 070107. kkurrung. 짐승 밥 테이블
    
    // 2007/1/11. iridology. 몬스터원정대, 몬스터대전은 보상할 때 처리 한번
    mutable KncCriticalSection      m_csPetExpInfo;     // 070108. kkurrung. m_vecPetExpInfo 보호
    std::vector<KPetExpInfo>        m_vecPetExpInfo;    // 070108. kkurrung. 겅험치 관련 정보 테이블
    std::vector< KPetBonusInfo >    m_vecPetBonusInfo;  // 090521. tslayer. 펫경험치보너스 정보

    // 허기
    //-5 -6	-7	-8	-9	-10
    mutable KncCriticalSection      m_csDecreseTable;   // 070107. kkurrung. DecreseTable 보호
    std::map<int,int>               m_mapDecreseTable;  // 070107. kkurrung. 만복도 감소 테이블
    int                             m_nMinDecreseMinute;   // 2007/1/12. iridology. 최소 감소 시간
    int                             m_nMaxDecreseMinnute;   // 2007/1/12. iridology. 최소 감소 시간

    // Exp, Gp 도움
    mutable KncCriticalSection      m_csPetAssist;      // 2007/1/10. iridology. Exp, Gp Assist 보호
    std::map< int, KAssistFactor >  m_mapAssistTable;   // 2007/1/10. iridology. Exp, Gp 도움 테이블

    mutable KncCriticalSection      m_csMaxFeed;        // 2007/1/11. iridology. MaxFeed 보호
    int                             m_iMaxFeed;         // 2007/1/11. iridology. 만복도 최대치

    mutable KncCriticalSection      m_csPanaltyTable;   // 2007/1/11. iridology. 패널티 테이블 보호
    std::vector< KPanaltyFactor >   m_vecPanaltyTable;  // 2007/1/11. iridology. 만복도 패널티

    mutable KncCriticalSection      m_csPromotionPanaltyTable;  // 070412. kkurrung. 전직에 따른 경험치 패널티
    std::map< char, float >         m_mapPromotionPanaltyTable; // 070412. kkurrung. 전직에 따른 경험치 패널티

    mutable KncCriticalSection      m_csTransformItem; // 070419. woosh. 펫진화/퇴화 아이템 맵 보호
    std::map< int, KTransformLv >   m_mapTransformItem; // 070419. woosh. 펫진화/퇴화 아이템( ID, 가능 레벨 )

    mutable KncCriticalSection      m_csHatchingInfoTable;
    std::map<int,KHatchingInfo>     m_mapHatchingInfoTable;

    mutable KncCriticalSection      m_csPetCostume;
    std::map<std::pair<DWORD,char>, std::set<DWORD> > m_mapPetCostume;

    mutable KncCriticalSection      m_csPetVestedItem;
    std::vector<GCITEMID>           m_vecPetVestedItem; // 펫 귀속 아이템

};
DefSingletonInline( KPetHelper );
DeclOstmOperatorA( KPetHelper );