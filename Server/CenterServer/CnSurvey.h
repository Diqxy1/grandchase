#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>

class KLuaManager;
struct KSurveyInfo;
struct KECN_SURVEY_DATA_NOT;
class KSerBuffer;

class KCnSurvey
{
    DeclareSingleton( KCnSurvey );
    NiDeclareRootRTTI( KCnSurvey );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

    KCnSurvey(void);
    ~KCnSurvey(void);

    bool LoadScript();
    void UpdateSurveyData( IN std::map< int, std::map< int, KSurveyQuestion > >& mapSurveyAll_ );
    int GetSurveyVersion();
    bool GetLanguageInfo( OUT std::vector< int >& vecLanguageInfo );
    void GetSurveyData( OUT KECN_SURVEY_DATA_NOT& kPacket );
    void Tick();
    bool CheckTick();
    bool CheckChange();

protected:
    void CompressSurveys( IN std::map< int, std::map< int, KSurveyQuestion > >& mapSurveyAll );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadLanguageInfo( IN KLuaManager& kLuaMng, std::string& strTable, OUT std::vector< int >& vecLanguageInfo );
    bool LoadGameConnectTypeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, std::map< std::pair<int,int>, std::vector<int> > >& mapSurveyTypeInfo );
    bool LoadGameConnectSurveyInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< std::pair<int,int>, std::vector<int> >& mapSurveyTypeInfo );
    bool LoadDungeonClearTypeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, std::map< std::pair<int,int>, std::vector<int> > >& mapSurveyTypeInfo );
    bool LoadDungeonClearSurveyInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< std::pair<int,int>, std::vector<int> >& mapSurveyTypeInfo );
    bool LoadLevelUpTypeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, std::map< std::pair<int,int>, std::vector<int> > >& mapSurveyTypeInfo );
    bool LoadLevelUpSurveyInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< std::pair<int,int>, std::vector<int> >& mapSurveyTypeInfo );
    bool LoadSurveyIDInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector< int >& vecSurveyIDList );
    bool LoadRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, VEC_ITEMS >& mapRewards );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_ITEMS& vecRewards );
    bool IsRun() const;

protected:
    mutable KncCriticalSection  m_csSurvey;
    time_t                      m_tmBegin;
    time_t                      m_tmEnd;
    time_t                      m_tmReturnTime;
    DWORD                       m_dwTickGap;
    DWORD                       m_dwLastTick;
    bool                        m_bChanged;

    int                         m_nVersion;
    std::vector< int >          m_vecLanguageInfo;

    std::map< int, std::map< int, KSurveyQuestion > >   m_mapSurveyAll; // 국가별 텍스트 저장 <국가, 질번, (질문, (보기번호, 보기내용))>
    std::map< int, std::map< int, KSerBuffer > >   m_mapCompressedSurveyInfoPack;
    std::map< int, std::map< std::pair<int,int>, std::vector<int> > > m_mapSurveyTypeInfo; // 각 타입별 설문지 정보.
    std::map< int, std::vector< KDropItemInfo > >       m_mapSurveyRewardInfo;
};

DefSingletonInline( KCnSurvey );
DeclOstmOperatorA( KCnSurvey );