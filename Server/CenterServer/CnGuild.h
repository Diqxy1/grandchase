#pragma once
#include <RTTI.h>
#include "KncSmartPtr.h"
#include "Performer.h"
#include "CenterEvent.h"
#include "KncTimer.h"

SmartPointer(KCnGuild);
SmartPointer(KCenter); // 전방 선언
class KCnGuild : public KPerformer
{
    NiDeclareRTTI;
    DeclToStringW;
public:
    KCnGuild(void);
    ~KCnGuild(void);

private:
    KNGuildInfo                                 m_kNGuildInfo;      // 길드정보
    std::vector<KNGuildNotice>                  m_vecNGuildNotice;  // 길드소개/공지사항
    DWORD                                       m_dwGuildMasterUID; // 길마UserUID

    mutable KncCriticalSection                  m_csJoiner;
    // 맴버정보 map<UserUID,세부정보>
    std::map< DWORD, KNGuildUserInfo >          m_mapOnlineUsers;   // 온라인 유저 리스트
    std::map< DWORD, KNGuildUserInfo >          m_mapOfflineUsers;  // 오프라인 유저 리스트
    std::map< DWORD, KNGuildUserInfo >          m_mapJoiner;        // 가입대기자 리스트
    std::vector< KNGuildUserInfo >              m_vecJoinerByJoinDate; // 가입일순 대기자 리스트

    mutable KncCriticalSection                  m_csServers;
    std::map<DWORD,std::pair<KCenterPtr,int> >  m_mapServers;       // 서버 UID,  <서버포인터, 길드 유저수>
    KncTimer                                    m_kMinTimer;
    KncTimer                                    m_kMemberClearTimer;
    KncTimer                                    m_kPointTimer;
    KncTimer                                    m_kHourTimer;

    int                                         m_nMaxMemberNum;    // 최대 길드원수
    int                                         m_nMaxJoinerNum;    // 최대 대기자수
    int                                         m_nOfficialGuildMemberNum; // 공식길드가 되기위한 맴머수

    int                                         m_nJoinerRemoveTerm; // 가입신청 대기기간(지나면 자동가입거절)
    KSimpleDate                                 m_kRejectJoinerDate; // 가입거절되는 가입일자(이 날짜 이전의 가입신청자는 삭제처리)

    int                                         m_nGuildRemoveTerm; // 신생길드 대기기간(지나면 자동삭제)
    KSimpleDate                                 m_kGuilRemovedDate; // 신생길드 제거일.
    KSimpleDate                                 m_kLastDayCheckDate; // 마지막 검사 날짜

    mutable KncCriticalSection                  m_csConfig;
    KGuildConfig                                m_kConfig;

    mutable KncCriticalSection                  m_csMemberList;
    //bool                                        m_bMemberListClear; // 맴버리스트 삭제 요청

public:
    virtual void Tick();

    void SetGuildInfo( IN KNGuildInfo& kInfo );
    KNGuildInfo& GetGuildInfo() { return m_kNGuildInfo; }
    DWORD GetGuildUID() { return m_kNGuildInfo.m_dwUID; }

    void SetNotice( IN std::vector<KNGuildNotice>& vecNotice );
    void GetNotice( OUT std::vector<KNGuildNotice>& vecNotice );
    void ModifyNotice( IN const std::vector<KNGuildNotice>& vecNotice );

    void SetOnline( IN KNGuildUserInfo& kUserInfo_, IN KCenterPtr spCenter );
    bool SetOffline( IN DWORD dwUserUID );
    bool DrumoutUser( IN DWORD dwUserUID, IN const char cOutType );

    void SetMemberList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void GetMemberList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void GetJoinerUIDListByJoinDate( IN const int nUserCount_, OUT std::set<DWORD>& setUserList );

    bool IsUserListEmpty();
    size_t GetCount() { KLocker lock( m_csJoiner ); return ( m_mapOnlineUsers.size() + m_mapOfflineUsers.size() + m_mapJoiner.size() ); }
    size_t GetMemberCount() { KLocker lock( m_csJoiner ); return m_mapOnlineUsers.size() + m_mapOfflineUsers.size(); }
    size_t GetOnlineUserCount() { KLocker lock( m_csJoiner ); return m_mapOnlineUsers.size(); }
    size_t GetOfflineUserCount() { KLocker lock( m_csJoiner ); return m_mapOfflineUsers.size(); }
    size_t GetJoinerCount() { KLocker lock( m_csJoiner ); return m_mapJoiner.size(); }

    bool GetUserInfo( IN DWORD dwUserUID, OUT KNGuildUserInfo& kUserInfo );
    bool GetOnlineUserInfo( IN const DWORD dwUserUID, OUT KNGuildUserInfo& kUserInfo );
    bool GetOffineUserInfo( IN const DWORD dwUserUID, OUT KNGuildUserInfo& kUserInfo );
    bool GetJoinerInfo( IN const DWORD dwUserUID, OUT KNGuildUserInfo& kUserInfo );

    bool ModifyMemberComment( IN const DWORD dwUserUID, IN const std::wstring& strUserComment );
    bool ModifyOnlineUserComment( IN const DWORD dwUserUID, IN const std::wstring& strUserComment );
    bool ModifyOfflineUserComment( IN const DWORD dwUserUID, IN const std::wstring& strUserComment );
    bool ModifyJoinerComment( IN const DWORD dwUserUID, IN const std::wstring& strUserComment );

    void SetMemeberLevel( IN const char cMemberLevel, IN const std::set<DWORD>& setUID );
    bool ChangeMemberLevel( IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel );
    void GetGuildMarkInfo( OUT KNGuildMarkInfo& kMarkInfo );
    DWORD GetLiveUserCount();

    int GetMaxMemberNum() { return std::max<int>( 1, m_nMaxMemberNum ); }
    int GetMaxJoinerNum() { return std::max<int>( 0, m_nMaxJoinerNum ); }

    bool SetGuildMaster();
    void SetRank( IN const int nWeeklyRank, IN const int nMonthlyRank, IN const int nTotalRank );

    void SetConfig( IN KGuildConfig& kConfig );
    void PrintUserListInfo();

    template< class T > void SendToAll( const unsigned short usEventID, const T& data );
    template< class T > void QueueingToAll( const unsigned short usEventID, const T& data );

    void AutoUpgradeGuildGrade();

private:
    virtual void ProcessEvent( const KEventPtr& spEvent );

    void IntervalTick();
    void DayCheck();
    void MemberListClearCheck();

    void AddServer( IN KCenterPtr spCenter );
    void RemoveServer( IN const DWORD dwUID );

    UCHAR GetGuildMarkStatus() { return m_kNGuildInfo.m_ucMarkStatus; }
    void SetGuildMarkStatus( IN const UCHAR ucMarkStatus );
    bool GuildMarkUpdate( IN const KNGuildMarkInfo& kMarkInfo );

    bool AddMember( IN KNGuildUserInfo& kUserInfo );
    bool RemoveMember( IN const DWORD dwUserUID );
    bool AddJoiner( IN KNGuildUserInfo& kUserInfo );
    bool RemoveJoiner( IN const DWORD dwUserUID );
    bool RemoveOnlineUser( IN const DWORD dwUserUID );
    bool RemoveOfflineUser( IN const DWORD dwUserUID );

    bool IsInGuild( IN const DWORD dwUserUID );
    bool IsOnlineUser( IN const DWORD dwUserUID );
    bool IsOfflineUser( IN const DWORD dwUserUID );
    bool IsJoiner( IN const DWORD dwUserUID );

    void SetOnlineMemberList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void GetOnlineUserList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void SetOfflineMemberList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void GetOfflineUserList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void SetJoinerList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void GetJoinerList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList );
    void GetJoinerListVector( OUT std::vector<KNGuildUserInfo>& vecUserList );

    void SetMasterUID( IN const DWORD dwUID );
    DWORD GetMasterUID() { return m_dwGuildMasterUID; }
    void AutoJoinSettingCheck();
    void AutoChangeGuildJoinSettingNot();
    void RemoveJoinerVector( IN const DWORD dwUserUID );
    void SetNewbieGuildRemoveDate( IN KNGuildInfo& kInfo );
    void NewbieGuildDestroyCheck();
    void SetRejectJoinerDate();
    void AutoRejectJoinerCheck();
    void AutoRejectGuildJoinerReq( IN std::set<DWORD>& setUserUID );
    void UpgradeGuildGradeGiftReq();
    size_t GetJoinerOnlineCount();
    void AutoAcceptJoinerList();
    void UpdatePointTick();

protected:
   _DECL_ON_FUNC( ECN_UPDATE_GUILD_NAME_NOT, std::wstring );
    DECL_ON_FUNC( ECN_JOIN_GUILD_ACK );
   _DECL_ON_FUNC( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL );
   _DECL_ON_FUNC( ECN_GUILD_MARK_INFO_UPDATE_ACK, KNGuildMarkInfo );
    DECL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_ACK );
    DECL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK );
    DECL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_ACK );
    DECL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_ACK );
    DECL_ON_FUNC( ECN_REJECT_GUILD_JOINER_ACK );
   _DECL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy );
   _DECL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_ACK, int );
    DECL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_GIFT_ACK );
   _DECL_ON_FUNC( ECN_SET_GUILD_RANK_NOT, KSimpleGuildRank );
   _DECL_ON_FUNC( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad );
    DECL_ON_FUNC_NOPARAM( ECN_NEWBIE_GUILD_DESTROY_CHECK_NOT );
   _DECL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName );
   _DECL_ON_FUNC( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter );
   _DECL_ON_FUNC( ECN_DB_GUILD_POINT_UPDATE_ACK, KGuildPointBroad );

};


template< class T >
void KCnGuild::SendToAll( const unsigned short usEventID_, const T& data_ )
{
    std::vector<DWORD> vecUID;
    std::vector<DWORD>::iterator vit;
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mit;

    KLocker lock( m_csServers );

    for( mit = m_mapServers.begin(); mit != m_mapServers.end() ; ++mit )
    {
        if ( !mit->second.first ) continue;

        if ( false == mit->second.first->SendPacket( usEventID_, data_ ) ) {
            vecUID.push_back( mit->first );
        }
    }

    for ( vit = vecUID.begin() ; vit != vecUID.end() ; ++vit ) {
        m_mapServers.erase( *vit );
    }
}


template< class T >
void KCnGuild::QueueingToAll( const unsigned short usEventID_, const T& data_ )
{
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mit;
    KLocker lock( m_csServers );
    for( mit = m_mapServers.begin(); mit != m_mapServers.end() ; ++mit )
    {
        if ( !mit->second.first ) continue;

        mit->second.first->QueueingEvent( usEventID_, data_ );
    }
}
