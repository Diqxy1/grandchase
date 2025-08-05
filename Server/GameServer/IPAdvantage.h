#pragma once

#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <map>
#include <vector>
#include "PrimitiveTypedef.h"

struct KRatioInfo
{
    float m_fLuneRatio;
    float m_fNecklaceRatio;
    float m_fDungeonDropRatio;
    float m_fExpRatio;
    float m_fGpRatio;
    float m_fSpxRatio;
    float m_fPetExpRatio;
    float m_fPerUserExpRatio;
    float m_fPerUserGpRatio;
    float m_fPerUserSpxRatio;
    KRatioInfo()
    {
        m_fLuneRatio = 0.f;
        m_fNecklaceRatio = 0.f;
        m_fDungeonDropRatio = 0.f;
        m_fExpRatio = 0.f;
        m_fGpRatio = 0.f;
        m_fSpxRatio = 0.f;
        m_fPetExpRatio = 0.f;
        m_fPerUserExpRatio = 0.f;
        m_fPerUserGpRatio = 0.f;
        m_fPerUserSpxRatio = 0.f;
    }
};



class KLuaManager;
class KODBC;
//struct KItemInfo;
struct KDurationItemInfo;
struct KItemUnit;
struct KItem;
struct KDropItemInfo;

struct KFakeRewardInfo
{
    std::pair<int,int>      m_prLevelRange;
    std::map< DWORD, KDropItemInfo > m_mapItems;
};

struct KPetInfo;
struct KSimpleItem;
class KIPAdvantage
{
    DeclareSingleton( KIPAdvantage );
    NiDeclareRootRTTI( KIPAdvantage );
    DeclToStringW;
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

    enum {
        PCBANG_NONE         = 0,
        PCBANG_PUBLISHER    = 1,
        PCBANG_KOG          = 2,
        TH3BB_USER          = 3,
        LIN_BENEFIT_TYPE1   = 4,
        LIN_BENEFIT_TYPE2   = 5,
        LIN_BENEFIT_TYPE3   = 6,
        LIN_BENEFIT_TYPE4   = 7,
    };

    enum {
        
    };

    enum {
        MININUM_PCPET_SATIATION = 1000
    };

public:
    KIPAdvantage(void);
    ~KIPAdvantage(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    //char CheckPCBangUser( IN const std::wstring& strIP );
    bool IsPCBangUser( IN const char cType );
    bool IsTH3BBUser( IN const char cType );
    bool IsLINBenefitUser( IN const char cType );
    void GetExpGpSpxBonus( IN const DWORD dwUserType, OUT float& fExpRatio, OUT float& fGpRatio, OUT float& fSpxRatio );
    void GetUserPerRatio( IN const DWORD dwUserType, IN int nCount, OUT float& fExpRatio, OUT float& fGpRatio, OUT float& fSpxRatio );
    float GetPetExpRatio( IN const DWORD dwUserType );
    float GetLuneStrongBonus( IN const DWORD dwUserType );
    float GetNecklaceStrongBonus( IN const DWORD dwUserType );
    float GetDungeonDropBonus( IN const DWORD dwUserType );

    void GetDailyConnectionItem( OUT VEC_ITEMS& vecItems );
    //void GetConnectionItem( IN const char cCharType, IN const int nLevel, IN OUT DWORD& dwUID,
    //    IN OUT std::vector<KItemInfo>& vecNormal, IN OUT std::vector<KDurationItemInfo>& vecCount );

    void GetConnectionItem( IN const char cCharType, IN const int nLevel, IN OUT std::vector<KItem>& vecInven, IN const DWORD dwUserType );

    int AddLevelUpReward( IN const int nOldLv, IN const int nCurrentLv, IN OUT VEC_ITEMS& vecItem );
    int AddDungeonClearReward( IN const int nGameMode, IN OUT VEC_ITEMS& vecItem );
    std::wstring GetMsg();
//    void GetPCBangPetItem( IN OUT std::map<DWORD, KPetInfo>& m_mapPetInfo,
//        IN OUT std::vector<KItemInfo>& vecNormal, IN OUT std::vector<KDurationItemInfo>& vecCount ); // PC방 아이템 넣기.
    void GetPCBangPetItem( IN const char cCharType, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo,
          IN OUT std::vector<KItem>& vecItems, IN DWORD dwUserType ); // PC방 아이템 넣기.
    bool CheckUserPerRatio() { KLocker lock(m_csIPAdvantage ); return m_bEnableUserPerRatio; };

    void GetODBCKey( std::vector< std::pair<std::string,int> >& vecODBCKey_ ) { KLocker lock( m_csIPAdvantage ); vecODBCKey_ = m_vecODBCKey; };
    bool CheckTestIP( IN const std::wstring& strIP );
    GCITEMUID GetMaximumPCBangItemUID() { KLocker lock( m_csIPAdvantage ); return m_MaximumPCBangItemUID; }

    bool GetLINBenefitTestID( IN std::wstring& wsLogin_, OUT char& cBenefitType_ );

protected:
    bool LoadODBCKey( IN KLuaManager& kLuaMng, std::vector< std::pair<std::string,int> >& vecODBCKey );
    bool LoadRatioInfo( IN KLuaManager& kLuaMng, OUT KRatioInfo& kRatioInfo );
    bool LoadDungeonClearReward( IN KLuaManager& kLuaMng, OUT std::map<int,VEC_ITEMS>& mapDungeonClearReward );
    bool LoadLevelUpReward( IN KLuaManager& kLuaMng, OUT std::map<int,VEC_ITEMS>& mapLevelUpReward );
    bool LoadDailyConnectionReward( IN KLuaManager& kLuaMng, OUT VEC_ITEMS& vecItems );
    bool LoadConnectionReward( IN KLuaManager& kLuaMng, OUT std::map<char, std::vector<KFakeRewardInfo> >& mapConnectionReward );

    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_ITEMS& vecItems );
    bool LoadIntVector( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<int>& vecInt );
    bool LoadIntRange( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT int& nBegin, OUT int& nEnd );
    bool LoadConnectionItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< DWORD, KDropItemInfo >& mapItems );

    //bool QueryPCBangUser( IN const std::wstring& strIP, IN KODBC& kODBC );
    bool LoadPetReward( IN KLuaManager& kLuaMng, OUT std::vector<KPetInfo>& vcePetReward );
    bool LoadSimpleItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KSimpleItem>& vecSimpleItem );
    void SetPCBangPetInfo( IN OUT std::vector<KPetInfo>& vecPetInfo );
    bool LoadPetCostumeReward( IN KLuaManager& kLuaMng, OUT std::map< DWORD, KDropItemInfo >& mapItems );

    bool LoadTestIP( IN KLuaManager& kLuaMng, OUT std::vector<std::pair<DWORD,DWORD>>& vecIP );
    bool LoadTotalRatioInfo( IN KLuaManager& kLuaMng, OUT std::map<DWORD,KRatioInfo>& mapRatioInfo );
    bool LoadTotalPetInfo( IN KLuaManager& kLuaMng, OUT std::map<GCITEMUID, std::vector<KPetInfo>>& mapPetReward );
    bool LoadTotalConnectionReward( IN KLuaManager& kLuaMng, OUT std::map< DWORD, std::map<char, std::vector<KFakeRewardInfo> > >& mapTotalConnectionReward );
    bool LoadPCBangItemUID( IN KLuaManager& kLuaMng );

    bool LoadLINBenefitTestID( IN KLuaManager& kLuaMng_, OUT std::map< std::wstring, char >& mapLinBenefitTestID_ );

protected:
    mutable KncCriticalSection                      m_csIPAdvantage;
    std::wstring                                    m_strMsg;
    std::vector< std::pair<std::string,int> >       m_vecODBCKey;
    //KRatioInfo                                      m_kRatioInfo;
    std::map<DWORD,KRatioInfo>                      m_mapRatioInfo;
    std::map<int, VEC_ITEMS >                       m_mapDungeonClearReward;
    std::map<int, VEC_ITEMS >                       m_mapLevelUpReward;
    VEC_ITEMS                                       m_vecDailyConnectionReward;
    // std::map<char, std::vector<KFakeRewardInfo> >   m_mapConnectionReward;
    std::map< DWORD, std::map<char, std::vector<KFakeRewardInfo> > > m_mapTotalConnectionReward;
    // 091112. tgkwon PC방 Pet 추가.
    //std::vector<KPetInfo>                           m_vecPCPetReward;
    std::map<GCITEMUID, std::vector<KPetInfo>>      m_mapPetReward;
    DWORD                                           m_dwPCBangUID; // PC방 아이템 이외에 PC방 펫을 추가하기 위해서 변수 추가.
    bool                                            m_bEnableUserPerRatio;
    std::map< DWORD, KDropItemInfo >                m_mapPetCostumeReward;
    std::vector<std::pair<DWORD,DWORD>>             m_vecTestIPRange; // Test용IP등록
    GCITEMUID                                       m_MaximumPCBangItemUID; // PC방지급 ItemUID 시작값
    std::map< std::wstring, char >                  m_mapLinBenefitTestID;    // 남미 PC방 혜택 테스트 ID
};

DefSingletonInline( KIPAdvantage );
DeclOstmOperatorA( KIPAdvantage );
