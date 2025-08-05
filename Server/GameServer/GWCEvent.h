#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "UserPacket.h"
#include "CenterPacket.h"

struct KDropItemInfo;
struct KGWCCountryInfo;

class KGWCEvent
{
    DeclareSingleton( KGWCEvent );
    NiDeclareRootRTTI( KGWCEvent );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

    KGWCEvent(void);
    ~KGWCEvent(void);

    bool IsRun() const;
    void UpdateGWCEventData( IN const KECN_GWC_EVENT_DATA_NOT& kPacket );
    void GetTotalCountryInfo( OUT std::map<DWORD,KGWCCountryInfo>& mapTotalCountryInfo );
    void GetRankingRewardInfo( OUT std::map<DWORD, VEC_ITEMS>& mapRankingRewardInfo );
    bool GetRankingReward( OUT VEC_ITEMS& vecRankingReward );
    bool GetGradeReward( OUT float& fExpReward, OUT float& GPReward );
    bool UpdateCountryGrade( IN DWORD& dwCountryID, IN std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc, IN std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo );
    bool GetCurrentGrade( IN DWORD dwCountryCnt, IN std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc, OUT DWORD& dwGrade );
    void SetNoticeInfo( IN DWORD dwCountryNum );

protected:
    mutable KncCriticalSection                      m_csGWCEvent;
    time_t                                          m_tmBegin;
    time_t                                          m_tmEnd;
    time_t                                          m_tmDBUpdateTime; // DB에서 데이터 가져오는 시간.
    DWORD                                           m_dwCountryID;
    DWORD                                           m_dwCurrentGrade;
    DWORD                                           m_dwCurrentRanking;
    std::wstring                                    m_wstrCountryName;
    std::map<DWORD, std::pair<float,float>>         m_mapGradeRewardInfo;
    std::map<DWORD, VEC_ITEMS>                      m_mapRankingRewardInfo;
    std::map<DWORD, KGWCCountryInfo>                m_mapTotalCountryInfo; // { CountryID, KGWCCountryInfo }
    std::map<DWORD, std::pair<DWORD,DWORD>>         m_mapGradeDesc;
};

DefSingletonInline( KGWCEvent );
DeclOstmOperatorA( KGWCEvent );