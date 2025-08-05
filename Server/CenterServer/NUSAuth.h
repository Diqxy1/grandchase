#pragma once
#include "odbc/Odbc.h"
#include "SubThread.h" // thread, Event Handle
#include "IntEvent.h"
#include "NUserEvent.h"

struct KENU_VERIFY_ACCOUNT_REQ; // 전방 선언 했음.
struct KECN_USER_EXIT_GAME_NOT;

class KNUSAuth : public KSubThread< KNUserEvent >
{
    DeclareDBThread;

public:
    KNUSAuth(void);
    ~KNUSAuth(void);


protected:
    // derived from KSubThread
    virtual inline void ProcessIntEvent( const KIntEvent& kIntEvent );
    int CheckUserLoginInfo( IN const std::string strLogin, IN const std::string strPasswd, IN const int nGameCode, IN const std::wstring wstrIP, OUT bool& bMale, OUT __int64& biUniqueKey, OUT std::wstring& wstrFailString );
    int CheckUSUserLoginInfo( IN const std::string strLogin, IN const std::string strPasswd, IN const int nGameCode, IN const std::wstring wstrIP, OUT bool& bMale, OUT int& nUniqueKey, OUT std::wstring& wstrFailString );
    int InsertUserLogoutInfo( IN const std::wstring wstrLogin, IN const int nGameCode, IN const std::wstring wstrIP,IN const __int64 biUniqueKey, IN const int nUniqueKey );

protected:
    static std::string ms_strFileDSN;

protected:
    INT_DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ );
   _INT_DECL_ON_FUNC( ENU_LOG_OUT_NOT, KECN_USER_EXIT_GAME_NOT );
};
