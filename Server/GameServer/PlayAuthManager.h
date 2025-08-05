#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "PlayAuth.ConnectorAPI.h"
#include <Thread/Locker.h>
#include "Lua/KLuaManager.h"

class KPlayAuthManager
{
    DeclareSingleton( KPlayAuthManager );
    NiDeclareRootRTTI( KPlayAuthManager );
    DeclToStringW;

public:
    struct KOnlineUserInfo
    {
        std::string m_strUniqueID;
        std::string m_strTicket;
        DWORD       m_dwUID;
    };

    KPlayAuthManager( void );
    ~KPlayAuthManager( void );

    bool Initialize();
    void Uninitialize();

    bool CheckIn( IN const std::string& strUniqueID_, IN const unsigned long& ulUserIP_, IN const DWORD& dwUID_, IN const int& nAge_ );
    bool CheckOut( IN const std::string& strUniqueID_, IN const std::string& strTicket_ );

    // connect/disconnect에 대한 callback 함수
    void DoOnConnectedCallback( IN const std::string& strIP_, IN const unsigned short& usPort_ );
    void DoOnDisconnectedCallback( IN const std::string& strIP_, IN const unsigned short& usPort_ );

    // checkin/out에 대한 callback 함수
    void DoOnCheckInCallback( IN const int& nErrorCode_, IN const std::string& strUniqueID_, IN const unsigned long& ulUserIP_, IN const std::string& strTicket_, IN const PlayAuth::UserCheckInInfo& kUserCheckInInfo_ );
    void DoOnCheckOutCallback( IN const int& nErrorCode_, IN const std::string& strUniqueID_, IN const std::string& strTicket_ );

    // 셧다운 시스템에서의 알림 callback 함수
    void DoOnUserStateNotiCallback( IN const int& nNotiType_, IN const PlayAuth::UserNotiInfo& kUserNotiInfo_ );
    void DoOnUserAliveCheckCallback( IN const std::string& strUniqueID_, IN const std::string& strTicket_, OUT bool& bIsOnline );

    bool AddToOnlineUser( IN const KOnlineUserInfo& kOnlineUserInfo_ );
    bool RemoveFromOnlineUser( IN const std::string& strUniqueID_ );
    bool UpdateOnlineUserTicket( IN const std::string& strUniqueID_, IN const std::string& strTicket_ );
    bool GetOnlineUserInfo( IN const std::string& strUniqueID_, OUT KOnlineUserInfo& kOnlineUserInfo_ );
    void GetCheckInInfo( IN const PlayAuth::UserCheckInInfo& kUserCheckInInfo_, OUT KEVENT_SHUTDOWN_CHECKIN_INFO_NOT& kPacket_ );

    // 설정 변경 및 확인 함수
    // 라이브 중 변경할 경우 기존 접속중인 유저들은 셧다운제 처리가 적용되지 않는다. 가급적 라이브 중 변경하지 않는 것이 좋다
    void SetUsePlayAuthSystem( IN const bool& bUsePlayAuthSystem_ ) { KLocker lock( m_csOnlineUserInfo ); m_bUsePlayAuthSystem = bUsePlayAuthSystem_; };
    bool IsUsePlayAuthSystem() { KLocker lock( m_csOnlineUserInfo ); return m_bUsePlayAuthSystem; };
    void SetAutoReconnect( IN const bool& bAutoReconnect_ ) { KLocker lock( m_csOnlineUserInfo ); m_bAutoReconnect = bAutoReconnect_; };
    bool IsAutoReconnect() { KLocker lock( m_csOnlineUserInfo ); return m_bAutoReconnect; };
    void SetAutoReconnectGap( IN const DWORD& dwAutoReconnectGap_ ) { KLocker lock( m_csOnlineUserInfo ); m_dwAutoReconnectGap = dwAutoReconnectGap_; };
    DWORD GetAutoReconnectGap() { KLocker lock( m_csOnlineUserInfo ); return m_dwAutoReconnectGap; };

    void Tick();

private:
    PlayAuth::Connector::Handle		                m_hCon;

    mutable KncCriticalSection                      m_csOnlineUserInfo;
    std::map< std::string, KOnlineUserInfo >        m_mapOnlineUserInfo;

    bool                                            m_bIsConnected;
    DWORD                                           m_dwAutoReconnectLastTick;

    // 설정들
    bool                                            m_bUsePlayAuthSystem;   // 셧다운 시스템 사용할지 말지. true: 사용, false: 사용하지 않음. 기본값 false
    bool                                            m_bAutoReconnect;       // 재접속 할지 말지. true: 재접속 함, false: 재접속 하지 않음. 기본값 true
    DWORD                                           m_dwAutoReconnectGap;   // 재접속 시도 간격. 기본값 60000 ms. 1분
};

DefSingletonInline( KPlayAuthManager );
DeclOstmOperatorA( KPlayAuthManager );
