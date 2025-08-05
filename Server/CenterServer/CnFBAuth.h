#pragma once
#include "Socket/SocketObject.h"
#include "SubThread.h"
#include "NUserEvent.h"

class KCenterSimLayer;

class KCnFBAuth : public KSubThread< KNUserEvent >
{
    typedef KCenterSimLayer SLayerType;
public:
    struct SFBLogin
    {
        std::string     m_strLoginID; // PF 포함..
        std::string     m_strEmailID; // Email ID( 로그인 ID )
        std::string     m_strState; // i/o
        int             m_nAccount;     // 계정 인증 성공 여부에 대한 ok값( 0: failed, 1: success, -800: duplicate email )
        bool            m_bAccessAllow; // 블럭/허용
        int             m_nBenefit;     // 혜택 1: IP, 2: ID
        SFBLogin() { Clear(); };
        void Clear()
        {
            m_strLoginID.clear();
            m_strEmailID.clear();
            m_strState.clear();
            m_nAccount = 0;
            m_bAccessAllow = false;
            m_nBenefit = 0;
        }
    };

    enum FBPF // Fun Box Post Fix
    {
        FBPF_FUN_BOX    = 0,
        FBPF_PLAY_PARK  = 1,
        FBPF_MAX,
    };

public:
    KCnFBAuth(void);
    ~KCnFBAuth(void);

    // derived from KThread
    virtual bool Begin();
    virtual void Run();

    static void SetAuthIP( IN const char* szIP );
    static void AddAuthPort ( IN unsigned short usPort );
    static void DumpData();

protected:
    // derived from KSubThread
    virtual void ProcessIntEvent( const KIntEvent& kIntEvent_ );
    bool WaitEvents( IN const std::wstring& strLogin  );
    void InitResultData() { m_strResult.clear(); }
    bool GetLoginResult( OUT SFBLogin& fbLogin );
    bool GetLogoutResult();
    bool GetPostFix( IN const int nPF, OUT std::string& strPF );
    bool GetPF_Login( IN std::string& strLoginPacket, OUT std::string& strLogin, OUT std::string& strPF );
    bool CheckCorrectID( IN const std::string& strSendLogin, IN const std::string& strPacketLogin, IN const std::string& strAuthLogin, IN const std::string& m_strEmailID );

    unsigned short GetPort();
    std::string GetIPString();

protected:
    static struct KCommonData
    {
        KncCriticalSection          m_csData;
        std::string                 m_strServerIP;  // server ip
        std::vector<unsigned short> m_vecPorts;
    } ms_ComnData;



    class KSocketFBAuth : public KSocketObject
    {
    public:
        void OnRecvCompleted( DWORD dwTransfered );
        void OnSocketError();
        KCnFBAuth* m_pkFBAuth;
        DWORD      m_dwLastHeartBitTime;
    } m_kSock;

    std::string         m_strIP;
    unsigned short      m_usPort;
    std::string         m_strResult;    // Result Raw string From FunBox
    HANDLE              m_hEvents[2];   // send, receive
    DWORD               m_dwWaitTime;   // waiting time for ACK
    HANDLE              m_hEndEvent;
    DWORD               m_dwThreadID;

    INT_DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ );
   _INT_DECL_ON_FUNC( ENU_LOG_OUT_NOT, KECN_LOG_OUT_NOT );
};

class KCnPPAuth : public KCnFBAuth { };