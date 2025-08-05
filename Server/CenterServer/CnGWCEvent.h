#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>

class KLuaManager;
struct KECN_GWC_EVENT_DATA_NOT;
struct KGWCCountryInfo;

class KCnGWCEvent
{
    DeclareSingleton( KCnGWCEvent );
    NiDeclareRootRTTI( KCnGWCEvent );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

    KCnGWCEvent(void);
    ~KCnGWCEvent(void);

    bool LoadScript();
    void Tick();
    bool IsRun() const;
    void GetGWCEventData( OUT KECN_GWC_EVENT_DATA_NOT& kPacket );
    void UpdateGWCTotalCountryInfo( IN std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo, IN std::vector<std::pair<DWORD,DWORD>>& vecRankingInfo );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_ITEMS& vecRewards );
    bool LoadGradeRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD, std::pair<float,float>>& mapGradeRewardInfo );
    bool LoadRankingRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD, VEC_ITEMS>& mapRankingRewardInfo );
    bool CheckDBUpdate();
    bool CheckDBChange();
    bool CheckNoticeChange();
    bool LoadGradeDescInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<DWORD,std::pair<DWORD,DWORD>>& mapGradeDesc );
    bool GetCurrentGrade( IN OUT DWORD& dwCountryCnt, OUT DWORD& dwGrade );
    void ModifyTotalCountryInfo( IN OUT std::map<DWORD, KGWCCountryInfo>& mapTotalCountryInfo, IN std::vector<std::pair<DWORD,DWORD>>& vecRankingInfo );

protected:
    mutable KncCriticalSection                      m_csCnGWCEvent;
    time_t                                          m_tmBegin;
    time_t                                          m_tmEnd;
    time_t                                          m_tmDBUpdateTime; // DB에서 데이터 가져오는 시간.
    DWORD                                           m_dwVersion;
    DWORD                                           m_dwTickGap;
    DWORD                                           m_dwLastTick;
    bool                                            m_bDBChanged;
    bool                                            m_bNoticeChanged;
    DWORD                                           m_dwCountryID;
    std::wstring                                    m_wstrCountryName;
    std::map<DWORD, std::pair<float,float>>         m_mapGradeRewardInfo;
    std::map<DWORD, VEC_ITEMS>                      m_mapRankingRewardInfo;
    std::map<DWORD, KGWCCountryInfo>                m_mapTotalCountryInfo; // { CountryID, KGWCCountryInfo }
    std::map<DWORD,std::pair<DWORD,DWORD>>          m_mapGradeDesc;
};

DefSingletonInline( KCnGWCEvent );
DeclOstmOperatorA( KCnGWCEvent );