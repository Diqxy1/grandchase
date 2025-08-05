#pragma     once
#include "SimLayer.h"
#include "Center.h"
#include "NUser.h"
#include <KncTimer.h>
#include <boost/random.hpp>

class KCenterSimLayer : public KSimLayer
{
    DeclareSingleton( KCenterSimLayer );
    NiDeclareRTTI;
    DeclToStringW;
public:
    enum ServerType
    {
        ST_CHECK_DUPL_LOGIN = 0X00000001,   // NUser-NUser는 항상 체크. NUser-Simple.와 Simple.-Simple.의 중복 확인 플래그.
        ST_INTERNAL_TESOP   = 0x00000008,   // play time 확인 패스, 체크섬 확인 패스, db에 모드카운트 기록 안함.
        ST_KOG_AUTH_TEST    = 0x00000010,   // KOG에서 창유로 인증/빌링 서버 테스트 위한 옵션. IP 하드코딩 됨.
        ST_FORCE_DWORD      = 0xFFFFFFFF,
    };

    KCenterSimLayer(void);
    virtual ~KCenterSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua( std::shared_ptr<lua_State> L );

    void GetNUserWaitTime( DWORD& dwNUserWaitTime );
    void SetNUserWaitTime( DWORD dwNUserWaitTime );

    void AddConnectionCount()       { if( !m_bConnectCountEnable) return; ++m_dwConnectionCount; };

    void SetBlockIPPassCheck( IN const bool bCheck_ )   { KLocker lock( m_csBlockIPPassCheck );    m_bBlockIPPassCheck = bCheck_; }
    bool GetBlockIPPassCheck()                          { KLocker lock( m_csBlockIPPassCheck );    return m_bBlockIPPassCheck; }

    void AddDisconnectReson( IN const int nType );
    void GetDisconnectReson( OUT std::map<int,int>& mapDisconnectType );

    DWORD GetUniqueNumber() { KLocker lock( m_csUniqueNumber ); return ++m_dwUniqueNumber; }
    void SetFBAuthQ( const DWORD dwTickGap, const int nThreshold );
    void GetPingConfig( OUT KPingConfig& kPingConfig_ );
    void SetPingConfig( IN const DWORD dwPingCheckGap_, IN const DWORD dwPingLimitGap_, IN const DWORD dwPingLimitValue_, IN const int nRepetitionCount_, IN const bool bEnable_, IN const int nJoinRoomLimitPing_ );
    bool CheckPingConfigChange();
    void SetUseLoadCheckInServer( bool bUse ) { KLocker lock(m_csUseLoadCheckInServer); m_bUseLoadCheckInServer = bUse; };
    bool GetUseLoadCheckInServer() { KLocker lock(m_csUseLoadCheckInServer); return m_bUseLoadCheckInServer; };
    void SetFakeCCU( float fRatio_ ) { KLocker lock(m_csFakeCCU); m_fFakeCCU = fRatio_; }
    float GetFakeCCU() { KLocker lock(m_csFakeCCU); return m_fFakeCCU; }
    // overlapped check.
    void SetCheckOverlapped( IN const bool bcheck_ ) { KLocker lock( m_csOverlapped); m_bCheckOverlapped = bcheck_; };
    bool GetCheckOverlapped(){ KLocker lock( m_csOverlapped); return m_bCheckOverlapped; };
    void SendKickUserNot( IN const DWORD& dwUserUID );
    // blockip user check.
    void SetBlockIPUserLogEnable( IN const bool bBlockLogCheck_ ) { KLocker lock( m_csBlockIPUserLog ); m_bBlockIPUserLogEnable = bBlockLogCheck_; };
    bool GetBlockIPUserLogEnable(){ KLocker lock( m_csBlockIPUserLog ); return m_bBlockIPUserLogEnable; };

    int GetServerListAuthLevel(){ KLocker lock( m_csServerListAuthLevel ); return m_nServerListAuthLevel; }

    std::string MakeSHA1( IN std::wstring& strInput_ );

    int GetWrongPasswordLimit();
    void SetUseBlockWrongPassword( IN bool bUseBlockBruteForceAttack_ );
    bool GetUseBlockWrongPassword();

    void InitCYOUBilling( std::wstring& strServerName_, DWORD dwUID_ );
    void CYOUChangeCash();

    void MakeCYOUSerialNumber( OUT std::string& strSerialNumber_ );
    void SetBillConnIP( IN const char* szIP );
    void GetBillConnIP( OUT std::string& strBillConnIP ) { KLocker lock( m_csBillConnIP ); strBillConnIP = m_strBillConnIP; };

    void SetCYOUBillLogEnable( IN const bool bCYOUBillLogCheck_ ) { KLocker lock( m_csCYOUBillLog ); m_bCYOUBillLogEnable = bCYOUBillLogCheck_; };
    bool GetCYOUBillLogEnable() { KLocker lock( m_csCYOUBillLog ); return m_bCYOUBillLogEnable; };

    void SetAuthEnableForInternalTesop( IN const bool bAuthEnableForInternalTesop_ ) { KLocker lock( m_csAuthEnableForInternalTesop ); m_bAuthEnableForInternalTesop = bAuthEnableForInternalTesop_; };
    bool GetAuthEnableForInternalTesop() { KLocker lock( m_csAuthEnableForInternalTesop ); return m_bAuthEnableForInternalTesop; };

// end of public:
private:
    
    void SetWrongPasswordLimit( IN const int nWrongPasswordTime_ );


protected:  // util function
    void CheckFBAutQueue();
    void GetAllServerCCU( OUT int& nTotalCCU_ );
public:
    int                 m_nNewUserAge;
    int                 m_nOldUserAge;
    bool                m_bDynamicResourceLoadEnable; // 클라이언트에서 리소스 정보를 동적으로 로딩 할지..
    int                 m_nAccOnlineTimeTest; // 중국 누적시간 테스트
    int                 m_nUserHeartBeatError; // 중국 유저하트비트 에러 테스트

protected:
    mutable KncCriticalSection  m_csNUserWaitTime;
    DWORD                       m_dwNUserWaitTime;

    bool                        m_bConnectCountEnable;
    DWORD                       m_dwLastConnectionTick;
    DWORD                       m_dwConnectionCount;

    mutable KncCriticalSection  m_csBlockIPPassCheck;
    bool                        m_bBlockIPPassCheck;    // Block IP 패스여부 체크할 것인지?(미국은 체크함)

    mutable KncCriticalSection  m_csDisconnectType;
    std::map<int,int>           m_mapDisconnectType;
    DWORD                       m_dwDisconnectTypeTick;

    mutable KncCriticalSection  m_csUniqueNumber;
    DWORD                       m_dwUniqueNumber;       // p2p 처리 관련 , 게임시작 마다 ++

    mutable KncCriticalSection  m_csFBAuthQueue;
    DWORD                       m_dwFBQueCheckTickGap;
    DWORD                       m_dwFBQueCheckLastTick;
    int                         m_nFBQueueThreshold;

    mutable KncCriticalSection  m_csPingInfo;
    bool                        m_bPingConfigChanged;
    KPingConfig                 m_kPingConfig;

    mutable KncCriticalSection  m_csUseLoadCheckInServer;
    bool                        m_bUseLoadCheckInServer;

    mutable KncCriticalSection  m_csFakeCCU;
    float                       m_fFakeCCU;

    mutable KncCriticalSection  m_csOverlapped;
    bool                        m_bCheckOverlapped;

    mutable KncCriticalSection  m_csBlockIPUserLog;
    bool                        m_bBlockIPUserLogEnable;

    mutable KncCriticalSection  m_csServerListAuthLevel;
    int                         m_nServerListAuthLevel;

    KncTimer m_kLoginCCUTimer;
    KncTimer m_kDBQLogTimer;
    KncTimer m_kSyncTimeTimer;

    KncCriticalSection          m_csBillConnIP;
    std::string                 m_strBillConnIP; // CYOU 빌링서버에 전달할 IP

    mutable KncCriticalSection  m_csCYOUBillLog;
    bool                        m_bCYOUBillLogEnable; // CYOU 빌링로그 기록여부

    mutable KncCriticalSection  m_csAuthEnableForInternalTesop;
    bool                        m_bAuthEnableForInternalTesop; // 테섭에서 인증서버 사용할지 여부(true: 사용, false: 미사용)

private:
    mutable KncCriticalSection m_csWrongPasswordLimit;
    int m_nWrongPasswordLimit;
    bool m_bWrongPasswordBlock;

    KncTimer m_kTimer_ServerHeartbeat; // 중국 CYOU 빌링 서버로 보내는 로긴서버의 하트비트 타이머.

    boost::lagged_fibonacci607 rng;
    boost::uniform_int<DWORD> uint32;
    boost::variate_generator<boost::lagged_fibonacci607&, boost::uniform_int<DWORD> > die;

};

DefSingletonInline( KCenterSimLayer );
