#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <vector>
#include <map>
#include <Thread/Locker.h>
#include "Kairo.h"

#include "UserPacket.h"
class KLuaManager;

class KSocks
{
    DeclareSingleton( KSocks );
    NiDeclareRootRTTI( KSocks );

    DeclToStringW;

    typedef std::vector<KDropItemInfo> VEC_REWARD;

public:
    KSocks(void);
    ~KSocks(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun();
    void GetSocksList( OUT std::vector< GCITEMID >& vecSocks_ );
    void GetSocksMaterialList( OUT std::map< GCITEMID, std::pair< GCITEMID, int > >& mapMaterial_ );
    void GetSocksInfo( IN const GCITEMID itemID_, OUT KSocksInfo& socksInfo_ );
    DWORD GetSocksSecond( IN const GCITEMID itemID_ );
    void CountdownRemainTime( IN OUT std::map< GCITEMID, KSocksInfo >& mapSocksInfo_, std::map< DWORD, bool >& mapSocksTime );
    void CheckSocksDone( IN OUT std::map< GCITEMID, KSocksInfo >& mapSocksInfo_, OUT std::vector< GCITEMID >& vecCompleteSocks_, IN const std::map< DWORD, bool >& mapSocksTime_ );
    void GetRewardItem( IN const GCITEMID itemID_, IN OUT std::vector<KDropItemInfo>& vecItemList_ );
    void GetSocksScript( OUT std::map< GCITEMID, KSocksInfo >& SocksInfo_ );

private:
    mutable KncCriticalSection m_csSocks;
    time_t m_tmBegin;          // 이벤트 시작 날짜.
    time_t m_tmEnd;            // 이벤트 종료 날짜.
    std::map< GCITEMID, KSocksInfo > m_mapSocks;
    std::map< GCITEMID, Kairo > m_mapSocksReward; // socksID, 

private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadSocks( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<GCITEMID, KSocksInfo>& mapSocks_, OUT std::map< GCITEMID, Kairo >& mapSocksReward_ );
    bool LoadRewardList( IN KLuaManager& kLuaMng_, OUT VEC_REWARD& vecReward_, OUT Kairo& kairo_ );
    bool LoadMaterialItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair< GCITEMID, int >& prItem_ );
    bool LoadSocksInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT KDropItemInfo& socksItem_ );
    
};

DefSingletonInline( KSocks );
DeclOstmOperatorA( KSocks );
