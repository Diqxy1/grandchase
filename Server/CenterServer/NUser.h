#pragma once
#include "Actor.h"
#include "NUserPacket.h"
#include "CenterNetLayer.h"
#include "NUserEvent.h"

SmartPointer(KNUser);

class KNUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareException;

public:
    struct NUserPoolTag{};
    KNUser(void);
    ~KNUser(void);

    enum ENUM_TICKS // Tick 관련
    {
        CONNECT_TICK = 0,
        AUTHEN_TICK,
        SEND_SERVER_LIST,
        SEND_CHANNEL_NEWS,
        SERVER_STAY_TICK,
        TICKS_NUM,
    };

    // 100112 tgkwon. 채널링 추가.
    enum CHANNELINGTYPE
    {
        USER_CT_TOTAL       = 0,     // 전체.
        USER_CT_PUBLISHER_1 = 1,     // 넷마블 / 펀박스 / 대만 Gash 유저.
        USER_CT_PUBLISHER_2 = 2,     // 투니버스 / 플레이파크 / 대만 빈펀유저.
        USER_CT_PUBLISHER_3 = 3,     // 네이트.
        USER_CT_PUBLISHER_4 = 4,     // 네이버.
        USER_CT_MAX,                 // MAX.
    };

    // 100813 tgkwon. 한국 채널링 CCode Value( 넷마블, 투니랜드, 네이트 )
    enum CHANNELCCODE {
        CCODE_PUBLISHER_1 = 0,
        CCODE_PUBLISHER_2 = 9,
        CCODE_PUBLISHER_3 = 30,
        CCODE_PUBLISHER_4 = 8,
    };

    static KNUserPtr Create();
    static void AddManager( KNUserPtr spNuser );

    virtual void Tick();
    virtual KEvent* CreateKEvent() { return new KNUserEvent; }
protected:
    virtual void OnAcceptConnection();
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();
    unsigned int GetTick( IN int nIndex )  { return m_auiTickCount[ nIndex ]; }
    void SetTick( IN int nIndex )          { m_auiTickCount[ nIndex ] = ::GetTickCount(); }
    void UpdateTicksData();  // Tick에서 시간에 맞는 호출을 한다. 어떤리스트들을 Not할때 사용.
    void UpdateServerList();
    bool CheckValidate( std::string& strData );
    void UpdateChannelNews();
    bool CheckNMCPTS( std::string strCPTS_ );
    void SendItemInfo();
    void SendContentsInfo();
    void SendNewContentsInfo();
    void SendClientScriptInfoNot(); // 클라이언트 스크립트 정보 전달
    void SendSocketTableNot(); // 소켓 관련 아이템/GP 테이블 전달
    void SetAuthLevel( IN const char& cLevel )  { m_cAuthLevel = cLevel; }
    char GetAuthLevel()                         { return m_cAuthLevel; }
    void CheckServerListByAuthLevel( IN OUT std::vector< KServerInfo >& vecList );
    bool IsExceedWrongpasswordTime();
    void SendCashbackRatioInfoNot();
    void CheckDuplicationUserForCYOU( IN const std::wstring& wstrLogin_, IN const DWORD dwUID_ );
    bool CheckDuplicationUser( IN const std::wstring& wstrLogin_, IN const DWORD dwUID_ );

    DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ );
    DECL_ON_FUNC( ENU_VERIFY_ACCOUNT_ACK );
    DECL_ON_FUNC( EVENT_CLOSE_CONNECTION_NOT );
   //_DECL_ON_FUNC( ENU_CONNECT_TO_SERVER_REQ, std::wstring );
   _DECL_ON_FUNC( ENU_OVERLAP_FILE_INFO, std::vector<KOverlapFile> );
    DECL_ON_FUNC_NOPARAM( ENU_GUIDE_BOOK_LIST_REQ );
   _DECL_ON_FUNC( ENU_GUIDE_BOOK_LIST_ACK, MAP_SHORT_SHORT );
    DECL_ON_FUNC_NOPARAM( ENU_TEXTURE_DYNAMIC_LOAD_REQ );
    DECL_ON_FUNC_NOPARAM( ENU_LOADING_IMAGE_REQ ); 
    DECL_ON_FUNC_NOPARAM( ENU_CLIENT_CONTENTS_FIRST_INIT_INFO_REQ );
    DECL_ON_FUNC_NOPARAM( ENU_CLIENT_PING_CONFIG_REQ );
    DECL_ON_FUNC_NOPARAM( ENU_SHAFILENAME_LIST_REQ );
    DECL_ON_FUNC_NOPARAM( ENU_USER_AGREEMENT_REQ );
    DECL_ON_FUNC_NOPARAM( ENU_CLIENT_CONTENTS_FIRST_INIT_ADDITIONAL_INFO_REQ );
    DECL_ON_FUNC( ENU_ACCOUNT_REGISTER_REQ );
    DECL_ON_FUNC( ENU_ACCOUNT_REGISTER_ACK );

protected:
    unsigned int    m_auiTickCount[TICKS_NUM]; // Tick Count를 저장한다.
    //unsigned int    m_cConnectionTime;

    std::wstring    m_strArrangedServer;
    DWORD           m_dwAuthType;
    char            m_cAuthLevel;

    std::wstring                            m_wstrLogin;  // 유저 로그인.
    std::wstring                            m_wstrIP;     // 유저 접속한 IP.

    std::string m_strBillingAccount; // 중국 빌링 메인계정.

private:
    void AddLogoutStat();
    int m_nWrongpasswordInputTime;
};
