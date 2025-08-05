#pragma once

#include <KNCSingleton.h>
//#include <vector>
//#include "UserPacket.h"
#include <MMSystem.h>

#define INVITE_BOX_WAIT_TIME (5000)

struct KInviteFriendUserInfo
{
	DWORD           m_dwUserUID;
	std::wstring    m_strNickName;
    std::wstring    m_strNickColor;
	std::wstring    m_strLocation;
	char            m_cGrade;
	char            m_cBestCharGrade;
	bool			m_bInviteOK;
};
class KGCInviteManager
{
    DeclareSingleton( KGCInviteManager );
public:

    enum INVITE_TYPE
    {
        INVITE_TYPE_INVALID = -1,
        INVITE_TYPE_CHANNEL = 0,
        INVITE_TYPE_SQUARE,
		INVITE_TYPE_FRIENDS,
		INVITE_TYPE_GUILD,
        INVITE_TYPE_NUM,
    };

    KGCInviteManager(void);
    virtual ~KGCInviteManager(void);

    // getter/setter
    inline const std::vector<KInviteUserInfo>& GetInviteUserList() const { return m_vecInviteUserList; }
	inline const std::vector<KInviteFriendUserInfo>& GetInviteFriendUserList() const { return m_vecInviteFriendUserList; }
    inline void SetInviteUserList( const std::vector<KInviteUserInfo>& vecInviteUserList ) { m_vecInviteUserList = vecInviteUserList; }
	void SetInviteFriendUserList(const std::vector<KInviteUserInfo>& vecInviteUserList, int iMode_, bool bAddOffline_ = true);

    inline int GetCurrentPage() const { return m_nCurPage; }
    void SetCurrentPage( int nPage );
    inline int GetMaxPage() const { return m_nMaxPage; }
    inline void SetMaxPage( int nPage ) { m_nMaxPage = nPage; }
    inline INVITE_TYPE GetInviteType() const { return m_eInviteType; }
    inline void SetInviteType( INVITE_TYPE eType ) { m_eInviteType = eType; }
    inline bool GetMessageBoxOpen() const { return m_bMessageboxOpen; }
    inline void SetMessageBoxOpen( bool bOpen )
    {
        m_bMessageboxOpen = bOpen;
        m_dwMessageBoxOpenTime = ::timeGetTime();
    }
    const KInviteInfo& GetInviteInfo() const;
    inline DWORD GetOpenTime() const { return m_dwMessageBoxOpenTime; }
    inline const std::wstring& GetCurSelectSquareName() const { return m_strSelectSquareName; }
    inline void SetCurrentInviteInfo( const KInviteInfo& kInfo ) { m_kCurrentInviteInfo = kInfo; }
    inline const KInviteInfo& GetCurrentInviteInfo() const { return m_kCurrentInviteInfo; }
    inline bool IsDenyInviteMsg() const { return m_bDenyInviteMsg; }
    void SetDenyInviteMsg( bool bDeny, bool bForceSend = false );

    void FrameMove();
    void UpdateUserList();
    void UpdateSquareUserList( DWORD dwSquareID, const std::wstring& strSquareName );
    void AddInviteRequest( const KInviteInfo& kRequest );
    void PopInviteInfo();
    // [2/26/2008] breadceo. 나는 차단했으니 이후에 메세지는 없애!
    void ClearInviteList();

    bool JoinToInviteRoom();

private:
    std::vector<KInviteUserInfo> m_vecInviteUserList;
	std::vector<KInviteFriendUserInfo> m_vecInviteFriendUserList;
    int m_nCurPage;
    int m_nMaxPage;
    INVITE_TYPE m_eInviteType;
    DWORD m_dwSquareID;
    std::wstring    m_strSelectSquareName;
    std::vector< KInviteInfo > m_vecInviteRequest;
    bool m_bMessageboxOpen;
    DWORD m_dwMessageBoxOpenTime;
    KInviteInfo m_kCurrentInviteInfo;
    bool m_bDenyInviteMsg;

public:
	bool m_bInviteAccept;
};

DefSingletonInline( KGCInviteManager );


// 대전 매칭에 새로 만들어 쓰겠음. 구조체가 틀림.
class KGCMatchInviteManager
{
	DeclareSingleton( KGCMatchInviteManager );
public:

	KGCMatchInviteManager(void);
	virtual ~KGCMatchInviteManager(void);

	void SetFriendList(std::vector<KMatchInviteUserInfo> vecMatchInviteList) { m_vecMatchInviteList = vecMatchInviteList;}
	bool GetCurrentPageUserList( int iCurrent, int iMax, OUT std::vector<KMatchInviteUserInfo> &vecResult );
private:
	std::vector<KMatchInviteUserInfo> m_vecMatchInviteList;
};

DefSingletonInline( KGCMatchInviteManager );

