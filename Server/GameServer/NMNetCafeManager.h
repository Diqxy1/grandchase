#include "NMNetCafeDLL.h"
#include "SubThread.h"
#include "IntEvent.h"
#include "UserEvent.h"
#include <Thread/Locker.h>
#include <ToString.h>
#include <RTTI.H>

using namespace Marbleworks::NetCafe::API;
class KGSSimLayer;
class NetCafeManager : public KSubThread< KUserEvent >, CNMNetCafe
{
    typedef KUserEvent  IDVenderType;
    typedef KGSSimLayer SLayerType;

    NiDeclareRootRTTI( NetCafeManager );
    DeclToStringW;

public:
    NetCafeManager(void);
    virtual ~NetCafeManager(void);

    //{{ callback 함수 : 넷마블 빌링서버에서 호출됨
    virtual void OnChkLogin( LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime, LPVOID param );
    virtual void OnLogin   ( LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime, LPVOID param );
    virtual void OnExpire  ( UINT result, LPCSTR userCN, UINT deductType, UINT restPoint, double expireDateTime );
    virtual void OnError   ( UINT errCode );

    virtual void OnLogout  ( LPCSTR userCN, UINT result, LPVOID param ); // lfNMNetCafeLogout 에 대한 응답
    virtual bool OnAlive   ( LPCSTR userCN, ULONG ipAddress );	
    //}}

    void SetInitialize( const bool& bVal ) { KLocker lock( m_csNetCafe ); m_bInitialize = bVal; }
    bool IsInitialize() { KLocker lock( m_csNetCafe ); return m_bInitialize; }

    void SetCnUserUID( IN const std::string& UserCN, IN const DWORD& dwUserUID );
    bool GetCnUserUID( IN const std::string& UserCN, OUT DWORD& dwUserUID );
    void DelCnUserUID( IN const std::string& UserCN_ );

    void SetUserPlayTime( IN const DWORD& dwUserUID, IN const UINT& PlayTime );
    bool GetUserPlayTime( IN const DWORD& dwUserUID, OUT UINT& PlayTime );
    void DelUserPlayTime( IN const DWORD& dwUserUID_ );

protected:
    mutable KncCriticalSection  m_csCallback;
    mutable KncCriticalSection  m_csNetCafe;
    bool                        m_bInitialize;
    std::map<std::string,DWORD> m_mapCnUserUID; // UserCN, UserUID
    std::map<DWORD,UINT>        m_mapUserPlayTime; // UserUID, PlayTime

protected:
    bool Startup();
    void Cleanup();

    void RequestUserCheck ( IN const std::wstring& strLogin, IN const LPCSTR userCN, IN const ULONG& ipAddress );
    void RequestUserLogin ( IN const std::wstring& strLogin, IN const LPCSTR userCN, IN const ULONG& ipAddress, IN UINT playTime = 0 );
    int RequestUserLogout( IN const std::wstring& strLogin, IN const LPCSTR userCN, IN const ULONG& ipAddress );

    // derived from KSubThread
    virtual void ProcessIntEvent( const KIntEvent& kIntEvent_ );

    INT_DECL_ON_FUNC_NOPARAM( EVENT_NMNETCAFE_STARTUP_NOT );
   _INT_DECL_ON_FUNC( EVENT_NMNETCAFE_LOGIN_REQ, KNmNetcafeParam );
   _INT_DECL_ON_FUNC( EVENT_NMNETCAFE_LOGOUT_REQ, KNmNetcafeParam );
};
DeclOstmOperatorA( NetCafeManager );