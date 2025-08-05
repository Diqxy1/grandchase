#pragma once
#include <string>
#include <ToString.h>
#include <KNCSingleton.h>
#include <WTypes.h> //DWORD 때문에 넣어줘. 사실 window.h에 들어있지만..
#include <vector>
#include <map>
#include <Thread/Locker.h>
#include <RTTI.H>
#include <set>

class KLuaManager;

class KBlockIPList
{
    DeclareSingleton( KBlockIPList );
    DeclToStringW;
    NiDeclareRootRTTI( KBlockIPList );

public:
    enum AUTH_LEVEL
    {
        AL_BLOCK        = -3,   // 블럭
        AL_BAD          = -2,   // 불량
        AL_WARNING      = -1,   // 경고
        AL_NORMAL       = 0,    // 일반
        AL_CHALLENGER   = 1,
        AL_HIGH_RANKER  = 2,
        AL_ADMIN        = 3,    // 운영자
    };

    struct sIPRange
    {
        DWORD       m_dwBegin;
        DWORD       m_dwEnd;
        std::string m_strCC; // Country Code
    };

    struct sCondition
    {
        std::string m_strCC; // Country Code
        bool        m_bOldUser; // true : 모든 유저, false : 신규 유저.
        bool        m_bBlock;   // true : 막기, false : 허용
        bool Decision( const bool bNewUser );
    };

public:
    KBlockIPList(void);
    ~KBlockIPList(void);
    bool LoadScript();
    bool CheckBlockIP( IN const DWORD dwIp, IN const bool bIsNewUser, IN const char cAuthLevel, IN const bool bBlockIPPass );
	void CheckUserCountry( IN const DWORD dwIp_, OUT std::string& m_strCC_ );
    void SetCheck( bool bCheckEnable, bool bCheckOldUser, bool bIsBlock );
    void DumpIPList();
    void TestIPList( IN const char* szIP, IN const bool bExceptChk, IN const bool bPatchTimeChk, IN const bool bNewUSer, IN const int nAdminLevel );
    bool CheckExceptIP( IN const DWORD dwIp );
    bool AdminIPCheck( IN const DWORD dwIp, IN const char cAuthLevel );
    bool CheckBlockCountry( IN const std::string m_strCC);
    bool CheckExceptUserID( IN std::wstring wstrUserID );

protected:
    DWORD IPStrToDWORD( IN const std::string& strIP );
    std::string DWORDToIPStr( IN const DWORD dwIP ) const;
    bool LoadIpRange( IN KLuaManager& kLuaMng, OUT std::vector< sIPRange >& vecIPRange );
    bool LoadBinIpRange( IN std::string& IpRangeFile, OUT std::vector< sIPRange >& vecIPRange );
    bool LoadCondition( IN KLuaManager& kLuaMng, IN const char* szTable, OUT std::map<std::string, sCondition>& mapConditionTable );
    bool LoadBlockCountry( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<std::string>& setBlockCountryList );
    bool LoadExceptUserID( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<std::string>& setExceptUserID );

    bool ConditionCheck( IN const std::string& strCC, IN const bool bNewUser );
    void DumpCondition( std::wostream& stm ) const;
    void DumpException( std::wostream& stm ) const;
    void DumpAdminIP( std::wostream& stm ) const;

protected:
    mutable KncCriticalSection      m_csBlockIPList;
    bool                            m_bCheckEnable;         // 블럭 검사여부
    bool                            m_bPacthTime;           // 패치시간인지 여부.
    bool                            m_bAdminIPCheckEnable;  // 운영자IP체크 여부
    std::vector< sIPRange >         m_vecBlockIPList;       // 블럭 관리대상 IP 리스트
    std::vector< sIPRange >         m_vecExceptIPList;      // 검사예외 IP 리스트
    std::vector< sIPRange >         m_vecAdminIPList;       // 운영자 IP 리스트
    std::map<std::string, KBlockIPList::sCondition>   m_mapConditionTable;
    std::set<std::string>           m_setBlockCountryList;  // 블럭할 국가 List.
    std::set<std::string>           m_setExceptUserID;      // 예외 유저 ID List.
    bool                            m_bExceptUserID;        // 예외 유저 ID 체크할 지 여부 확인.
};
DefSingletonInline( KBlockIPList );
DeclOstmOperatorA( KBlockIPList );
