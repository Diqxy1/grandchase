#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>
#include "GCNateOn.h"
#include "MSPacket.h"

#define MAX_CHILD_NUM 7

struct FRIEND_UI_DATA // ui에 표시해줄 정보
{
    bool m_bGroup;
    bool m_bOpen;
    int m_nGroupID;

    bool m_bUser;
    bool m_bOnline;
    DWORD m_dwEmoticon;
    DWORD m_dwUserUID;

    std::wstring m_strDisplay;
    std::wstring m_strNickname;
    std::wstring m_strNickColor;

    bool operator==(const KRecommenderInfo& right) const
    {
        return ( m_dwUserUID == right.m_dwLoginUID );
    }
    FRIEND_UI_DATA()
    : m_bGroup(false)
    , m_bOpen(false)
    , m_nGroupID(-1)
    , m_bUser(false)
    , m_bOnline(false)
	, m_dwEmoticon(KFriend::DEFAULT_EMOTICON_ID)
    , m_dwUserUID(0)
    , m_strDisplay(L"")
	, m_strNickname(L"")
    , m_strNickColor(L"FFFFFF")
    {
    }
};

class GCBuddyChild;
class KD3DScrollbar;

class GCBuddyTree : public KD3DWnd,
                    public KActionListener,
                    public boost::noncopyable
{
public:
    DECLARE_CLASSNAME( GCBuddyTree );
    GCBuddyTree(void);
    virtual ~GCBuddyTree(void);

    virtual void ActionPerformed( const KActionEvent& event );

    void UpdateData();
    void SetMode( int iIdx, GCNateOn::E_NATEON_MODE eMode );
    void OnChild( KD3DWnd* pkWnd );
    void OnChildRClick( KD3DWnd* pkWnd );
    int GetTopIndex() const { return m_iTopIndex; }

	void SetChildSize( int iSize, bool bAlignTop = true );

protected:
    virtual void OnCreate();
    void OnScroll( DWORD dwEventCode );

private:
	int GetChildBegin();
	int GetChildEnd();

private:
    KSafeArray<GCBuddyChild*,MAX_CHILD_NUM> m_pkChild;
	KSafeArray<KD3DWnd*,MAX_CHILD_NUM> m_pkContactFriendMakr;
    int m_iTopIndex;
	int m_iChildSize;
	bool m_bAlignTop;
    GCNateOn::E_NATEON_MODE m_eMode;
    KD3DScrollbar* m_pkScroll;

	//길드메신저
private:
	bool m_bCheckGuildOfflineMember;
	bool m_bCheckGuildBlockMember;

public:
	void SetGuildOfflineMember( bool bCheckGuildOfflineMember_ ) { m_bCheckGuildOfflineMember = bCheckGuildOfflineMember_; }
	bool GetGuildOfflineMember() { return m_bCheckGuildOfflineMember;}
	void SetGuildBlockMember( bool bCheckGuildBlockMember_ ) { m_bCheckGuildBlockMember = bCheckGuildBlockMember_; }
	bool GetGuildBlockMember() { return m_bCheckGuildBlockMember;}
};

DEFINE_WND_FACTORY( GCBuddyTree );
DECLARE_WND_FACTORY( GCBuddyTree );
DECLARE_WND_FACTORY_NAME( GCBuddyTree );
