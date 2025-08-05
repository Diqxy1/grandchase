#pragma once
#include "BaseServer.h"
#include <map>
#include "UserPacket.h"
#include "DBLayer.h"
SmartPointer(KActor);

namespace luabind { class object; } // 전방선언

class KGameServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclareSingleton( KGameServer );

protected:
    KGameServer(void);
public:
    virtual ~KGameServer(void);

    virtual bool Init( const wchar_t* szFileName );
    virtual void RegToLua();
    virtual void Run( bool bRunInterpreter );
    virtual void ShutDown();

    void Tick();

    //////////////////////////////////////////////////////////////////////////
    // interface for lua
    void AddChecksum( DWORD dwChecksum );
    bool DeleteChecksum( unsigned int nIndex );
    void ClearChecksum();   // 등록된 checksum 전부를 삭제한다.

    void AddInvalidChecksumForLua( DWORD dwInvalidChecksum );
    bool DeleteInvalidChecksum( unsigned int nIndex );
    void ClearInvalidChecksum();   // 등록된 checksum 전부를 삭제한다.

    // 060605. kkurrung. 레벨별 모드 진입 제한 
    void AddKGameModeInfo( IN OUT KGameModeCategory& kGameModeCategory, int nID, int nLevel, IN bool bRandomable, const luabind::object& table_ );
    void AddGameModeCategory( IN int nID, IN int nLevel, IN KGameModeCategory& kGameModeCategory );
    KSerBuffer GetGameModeCategory(); // 압축해서 가져 온다. Thread에서 호출되지 안으므로 CS 없음

    void BuildPublisherUserCount( KActorPtr spActor );
    void ClearPublisherUser();
    void GetPublisherUser( std::map<DWORD,DWORD>& mapPublishUser );

    // 060818. florist. 메시지 서버 리스트 유지.
    void SetMsgServerList( IN std::vector<KServerInfo>& vecMsgServerList );
    void GetSuitableMsgServer( OUT KServerInfo& kInfo );

    // 061018. jseop. 캐쉬 할인 이벤트.
    void SetDiscountEvent( bool bEnable );
    void AddDiscountEventItem( IN GCITEMID nItemID, IN short sDiscountRate );
    void DeleteDiscountEventItem( IN GCITEMID nItemID );
    void ClearDiscountEventItem();
    bool GetDiscountItem( IN GCITEMID nItemID, OUT short& sDiscountRate );

    // 061024. jseop. 가이드라인 적용 기준 가입일.
    void ApplyGuideLineAfter( int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond );
    CTime GetPivotFirstLoginTimeToApplyGuideLine();

    bool dofileUTF8( const char* szFileName );

    // 야후 이벤트와 같이 퍼블리셔에서 요청한 보상 이벤트.
    void ResetPubEvent();
    bool IsPubEvent(){ KLocker lock(m_csPubEventBenefit); return m_bPubEvent; }
    void SetPubEvent( bool bPubEvent ) { KLocker lock(m_csPubEventBenefit); m_bPubEvent = bPubEvent; }
    void SetPubEventComment( const char* szComment );
    std::wstring GetPubEventComment(){ KLocker lock(m_csPubEventBenefit); return m_strPubEvent; }
    void AddPubEventItem( IN const GCITEMID nItemId, IN const int nCount, IN const int nPeriod );
    void SetPubEventGP( int nGP );
    void GetPubEventItemList(  OUT std::vector<KItem>& vecItemList_ );
    int  GetPubEventGP();

    // 070822. kkurrung. 무기 정보 관련 함수들. 
    void ClearWeaponInfo();
    void AddWeaponInfo( GCITEMID dwItemID, char cCharType, char cJobLV );
    DWORD ChkValidWeapon( IN OUT std::vector<KEquipItemInfo>& vecEquip, IN char cCharType,
                          IN char cMaxJobLV, OUT char& cCurrentJobLv ); // 내부에서 SET_ERROR 한다.
    void SetAbuseInfo( int nAbuseCount, int nAbuseTime, int nAbusePlayerCount );
    int GetAbuseCount() { KLocker lock(m_csAbuseInfo); return m_nAbuseCount; };
    int GetAbuseTime()  { KLocker lock(m_csAbuseInfo); return m_nAbuseTime; };
    int GetAbusePlayerCount() { KLocker lock(m_csAbuseInfo); return m_nAbusePlayerCount; };
    bool LoadScript( OUT std::string& strScript );
    bool ReLoadScript();

    DWORD GetUID() const { return m_dwUID; }
    void SetUID( IN DWORD val) { m_dwUID = val; }

    void IncreaseChannellingUserNum( IN const USHORT& usChannellingType ); // 채널링 유저수 증가.
    void DecreaseChannellingUserNum( IN const USHORT& usChannellingType ); // 채널링 유저수 감소.
    int GetChannellingUserNum( IN const USHORT& usChannellingType ) { KLocker lock( m_csChannelling ); return m_mapChannellingUserNum[usChannellingType]; };

    bool IsRegisteredChecksum( DWORD dwChecksum );
    bool IsRegisteredInvalidChecksum( DWORD dwInvalidChecksum );
    inline void AddInvalidChecksum( DWORD dwInvalidChecksum, const std::string& szLogin );

    void SetMaxUserNum( IN DWORD dwMaxUser_ );
    DWORD GetMaxUserNum();
    void SendGetPackageListNotToDB();

    void ReConnectCenterServer();

    void SetServerIP();
    void SetIP( IN const std::string& strIP ); // public IP
    std::string GetIP();
    void SetServerCrashBefore();
    void SetUserNickToLoginID( IN const std::wstring& wstrNickName, IN const DWORD& dwUserUID );
    bool GetUserNickToLoginID( IN const std::wstring& wstrNickName, OUT DWORD& dwUserUID );
    void DeleteUserNickToLoginID( IN const std::wstring& wstrNickName );

protected: // util func.
    inline void DumpChecksum( OUT std::wostream& stm ) const;
    inline void DumpPubEvent(  OUT std::wostream& stm ) const;

protected:
    HANDLE          m_hEventLog;
    std::wstring    m_strAppName;

    std::vector<DWORD>          m_vecClientChecksum;    // 051026. florist. 클라이언트 바이너리 checksum.
    mutable KncCriticalSection  m_csClientChecksum;     // DB t. 에서 쓰고 Main t. 에서 읽음.

    struct KInvalidChecksumInfo
    {
        KInvalidChecksumInfo():m_dwInvChecksum(0),m_nCount(0){}

        COPYCON_ASSIGNOP( KInvalidChecksumInfo, right )
        {
            m_dwInvChecksum = right.m_dwInvChecksum;
            m_nCount        = right.m_nCount;
            m_regTime       = right.m_regTime;
            m_strLogin      = right.m_strLogin;

            return *this;
        };

        DWORD           m_dwInvChecksum;
        DWORD           m_nCount;
        CTime           m_regTime;
        std::wstring    m_strLogin;
    };//end struct KInvalidChecksumInfo

    std::vector<KInvalidChecksumInfo>   m_vecInvalidChecksum;    // 060116. kkurrung. 부적절한 클라이언트 바이너리 체크썸
    mutable KncCriticalSection          m_csInvalidChecksum;     // 060116. kkurrung. DB t. 에서 쓰고 Main t. 에서 읽음.

    // CS 필요 없음.
    KEVENT_SET_GAME_MODE                m_kEventGameMode;   // 060605. kkurrung. 유저 레벨별 모드 진입 정보, 1회 로드후 Read만 함

    KncCriticalSection                  m_csPublishUserCount;
    std::map<DWORD, DWORD>              m_mapPublisherUserCount;

    mutable KncCriticalSection          m_csMsgServerList;          // DB T. - Main T.
    std::vector< KServerInfo >          m_vecMsgServerList;         // 060818. florist. 메시지 서버 IP list.
    mutable KncCriticalSection              m_csDiscountEventItem;
    bool                                    m_bDiscountEvent;           // 061018. jseop. 캐쉬 할인 이벤트 여부.
    std::vector< std::pair<int, short> >    m_vecDiscountEventItem;     // 061018. jseop. 캐쉬 할인 이벤트 아이템 ID.
    CTime                                   m_timeApplyGuideLine;       // 061024. jseop.

    // 현재는 야후에만 쓰인다.
    mutable KncCriticalSection      m_csPubEventBenefit;
    bool                            m_bPubEvent;
    std::vector<KItem>              m_vecPubEventItem;
    int                             m_nPubEventGP;
    std::wstring                    m_strPubEvent;

    mutable KncCriticalSection      m_csWeaponInfo;
    std::map< DWORD, KWeaponInfo >  m_mapWeaponInfo; // ItemID, Info

    mutable KncCriticalSection      m_csAbuseInfo;
    int                             m_nAbuseCount;
    int                             m_nAbuseTime;
    int                             m_nAbusePlayerCount;
    DWORD                           m_dwUID; // 서버의 UID , 로그인서버에서 준다.

    mutable KncCriticalSection      m_csChannelling;
    std::map<USHORT,int>            m_mapChannellingUserNum;

    mutable KncCriticalSection      m_csMaxUserNum;
    DWORD                           m_dwMaxUserNum;

    mutable KncCriticalSection m_csIP;
    std::string m_strIP;

    bool m_bServerCrashBefore;
    bool m_bConnectCYOU; // 초기값은 true, 중국 서버의 경우 값이 false 이면 서버가 켜지지 않게 하는 용도

    mutable KncCriticalSection      m_csNickLoginIDInfo;
    std::map<std::wstring, DWORD>   m_mapUserNickLoginIDInfo; // [LoginUID, strNickName]
};

DefSingletonInline( KGameServer );
