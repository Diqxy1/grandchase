#pragma once
#include <map>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

class KLuaManager;
struct KDotaModeInfo;
struct KDropItemInfo;

class KDotaManager
{
    DeclareSingleton( KDotaManager );
    NiDeclareRootRTTI( KDotaManager );
    DeclToStringW;

public:
    struct SConfigData
    {
        float       m_fWinRatio;
        float       m_fLoseRatio;

        SConfigData()
        {
            m_fWinRatio             = 0.f;
            m_fLoseRatio            = 0.f;
        }
    };

public:
    KDotaManager(void);
    ~KDotaManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void GetDotaExpReward( IN DWORD dwLevel, OUT int& nExp );
    void GetDotaGPReward( IN DWORD dwLevel, OUT int& nGP );
    float GetWinRatio() { KLocker lock( m_csDota ); return m_sConfigData.m_fWinRatio; }
    float GetLoseRatio() { KLocker lock( m_csDota ); return m_sConfigData.m_fLoseRatio; }
    bool CheckDotaMode( IN const int nMode );
    void GetDotaItemList( OUT std::map<KDropItemInfo, int>& mapDotaItemInfo );
    void GetDotaModeInfo( IN const int nMode, OUT KDotaModeInfo& kDotaModeInfo );
    void GetDotaDPointInfo( IN int nLevel, OUT int& nDPoint );

protected:
    bool LoadDotaModeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< int, KDotaModeInfo >& mapDotaModeInfo );
    bool LoadDotaModeMapInfo( IN KLuaManager& kLuaMng, OUT std::map< int, std::map<int,int> >& mapModeMapInfo );
    bool LoadMapInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int,int>& mapInfo );
    bool LoadDotaItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< KDropItemInfo, int >& mapDotaItemInfo );
    bool LoadDotaRewardConfig( IN KLuaManager& kLuaMng, IN std::string& strTable, SConfigData& sConfigData );
    bool LoadDotaLevelReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< DWORD, std::pair<int,int> >& mapLevelReward );
    bool LoadRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::pair<int,int>& prData );

protected:
    mutable KncCriticalSection                  m_csDota;
    std::map< int, KDotaModeInfo >              m_mapDotaModeInfo;
    std::map< KDropItemInfo, int >              m_mapDotaItemInfo;
    SConfigData                                 m_sConfigData;
    std::map< DWORD, std::pair<int,int> >        m_mapLevelReward;
    std::map< int, int >                        m_mapDPointTable;
};

DefSingletonInline( KDotaManager );
DeclOstmOperatorA( KDotaManager );