 #pragma once

#include "Socket/SocketObject.h"
#include "IntEvent.h"
#include "UserEvent.h"
#include "SubThread.h"
#include "UserPacket.h"

#include <Thread/Thread.h>

class KGSSimLayer;
class KGSGash : public KThread
{
    typedef KUserEvent  IDVenderType;
    typedef KGSSimLayer SLayerType;

public:
    KGSGash(void);
    virtual ~KGSGash(void);

public:

    // derived from KThread
    virtual bool Begin();

    static void InitGashInfo( const char* szIP, 
        unsigned short usPortAuthen,
        unsigned short usPortAuthor,
        unsigned short usPortAccount,
        int iServiceCode,
        const char* szRegionCode );

protected:
    virtual void Run();     ///< derived from KThread
    void Tick();
    void InitResultData()
    {
        KLocker lock( m_csResult );
        m_strOutLogin.assign( "" );
        m_strOutErr.assign( "" );
        m_strOutAccType.assign( "" );
        m_nResult = -1;
    }

protected:
    enum ENUM_ACTION_TYPE {
        //AUTHOR,
        ACCOUNT,
        ACTION_NUM
    };

    class KSocketGash : public KSocketObject {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError();
        KGSGash*  m_pkGash;
    } m_kSock[ACTION_NUM];

    HANDLE          m_hEvents[ACTION_NUM * 2 + 1];   // 소켓당 이벤트 두개, 스레드 종료 이벤트 한개
    KIntEventPtr    m_spEvent;
    DWORD           m_dwWaitTime;
    // 060525. kkurrung. Recv 후 이벤트 발생을 위해
    HANDLE          m_hEndEvent[ACTION_NUM];

    KncCriticalSection  m_csResult;
    std::string         m_strOutErr;
    std::string         m_strOutLogin;
    std::string         m_strOutAccType;    // 061204. kkurrung. 대만 야후 계정 체크
    int                 m_nResult;

    static struct KCommonData {
        KncCriticalSection  m_cs;
        std::string         m_strGashIP;
        unsigned short      m_usPort[ACTION_NUM];
        std::string         m_strServiceCode;
        std::string         m_strRegionCode;
    } ms_ComnData;

    bool WaitEvents( IN const ENUM_ACTION_TYPE eAction, IN const std::wstring& strLogin  );


    _INT_DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_REQ, KDB_EVENT_VERIFY_ACCOUNT_REQ );        // 인증
    _INT_DECL_ON_FUNC( DB_EVENT_EXIT_GAME_NOT, KGASH_ACCOUNTING );                      // 로그 아웃

};