#pragma once
#include "Socket/SocketObject.h"
#include "IntEvent.h"
#include "NUserEvent.h"
#include "SubThread.h"
#include "NUserPacket.h"

#include <Thread/Thread.h>

class KCenterSimLayer;
class KCenterGash : public KThread
{
    typedef KNUserEvent  IDVenderType;
    typedef KCenterSimLayer SLayerType;

public:
    KCenterGash(void);
    virtual ~KCenterGash(void);

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
    enum ENUM_ACTION_TYPE {
        AUTHEN,
        AUTHOR,
        ACTION_NUM
    };

    virtual void Run();     ///< derived from KThread
    void Tick();
    bool WaitEvents( IN const ENUM_ACTION_TYPE eAction, IN const std::wstring& strLogin  );
    void InitResultData()
    {
        KLocker lock( m_csResult );
        m_strOutLogin.assign( "" );
        m_strOutErr.assign( "" );
        m_strOutAccType.assign( "" );
        m_nResult = -1;
    }

    bool CheckValidate( std::string& strData );



    class KSocketGash : public KSocketObject {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError();
        KCenterGash*  m_pkGash;
    } m_kSock[ACTION_NUM];

    HANDLE          m_hEndEvent[ACTION_NUM];
    DWORD           m_dwWaitTime;
    HANDLE          m_hEvents[ACTION_NUM * 2 + 1];   // 소켓당 이벤트 두개, 스레드 종료 이벤트 한개
    KIntEventPtr    m_spEvent;

    KncCriticalSection  m_csResult;
    std::string         m_strOutErr;
    std::string         m_strOutLogin;
    std::string         m_strOutAccType;  // 061204. kkurrung. 대만의 경우 야후인지 일반인지 확인이 필요 하다. 센터에서는 의미 없음.
    int                 m_nResult;
    
    
    
    static struct KCommonData {
        KncCriticalSection  m_cs;
        std::string         m_strGashIP;
        unsigned short      m_usPort[ACTION_NUM];
        std::string         m_strServiceCode;
        std::string         m_strRegionCode;
    } ms_ComnData;

    INT_DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ );
    bool CheckAuthResult( std::string& strOutLogin, std::string& strInputLogin, int nResult ); // gash 결과 에러 체크
    void MakePasswordXOR( std::string& strLogin, std::string& strPasswd ); // password generator

};