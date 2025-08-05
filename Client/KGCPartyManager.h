#pragma once

#define MAX_PARTY_SIZE 6

class KGCPartyManager
{
    DeclareSingleton( KGCPartyManager );

public:
    KGCPartyManager(void);
    ~KGCPartyManager(void);
	
	enum {
		KPM_MATCH_PARTY_MAX_USER = 3,
		KPM_MATCH_PARTY_MAX_INVITE_USER = 2,
	};

public:
    void SetPartyMemberList(std::vector<KPartyMember>& vecPartyPeople);
	void SendInviteUser( DWORD dwUserUID, int iPartyType );
	void GoRoom( DWORD dwUserUID = -1, std::wstring wsPass = L"" );
    void JoinParty( bool bIsJoin );
    void OutPartyMember( DWORD dwUserUID );
    void LeaveParty( void );
    void EndParty( void );
    void ChangePartyMasterUser( DWORD dwUserUID );
    void SetPartyInfo(const KPartyData& kPartyData );
    bool IsLocalPartyMasterUser( void );
    bool IsPartyMasterUser( DWORD dwUserUID );
    bool IsLocalPartyMember( DWORD dwUserUID );
    bool IsParty( void );
    const std::vector<KPartyMember>& GetPartyMemberList(){return m_vecPartyPeople;}
    KPartyData GetPartyInfo( void );
    bool IsDungeonParty( void );
    void JoinPartyToInvite( bool bIsInvite );
    void SetInviteeUserInfo( bool bInviteUser);
    bool GetInviteeUserInfo( void );
    std::wstring GetRoomPass ( void );
    bool IsMaxSize();
    bool SetInviteState( bool bInvite );
    bool IsInviteState();
	int GetCurrentPartyUserCnt() { return static_cast<int>(m_vecPartyPeople.size()); }
	void EraseInvitePartyUser(DWORD dwUserUID);
	bool IsMatchReady( bool bAll );
	void SendWaitInviteUser( int iPartyType );
private:
    bool m_bIsJoinParty;
    bool m_bIsInvitee;
    bool m_bIsInviteState;
    std::wstring m_wsRoomPass;
    std::vector<KPartyMember> m_vecPartyPeople; 
    DWORD m_dwPartyMasterUID;
    KPartyData m_kPartyData;

	std::set<DWORD> m_setInviteUserList;
	std::set<DWORD> m_setWaitInviteUserList;
};
DefSingletonInline( KGCPartyManager );
