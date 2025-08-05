#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DSizingBox;
class KD3DStatic;
class KGCInviteUserInfo;

#define INVITE_USER_NUM 7

class KGCInviteUserList:  public KD3DWnd,
                          public KActionListener
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCInviteUserList( const KGCInviteUserList& );
    const KGCInviteUserList& operator=( const KGCInviteUserList& );

public:
    DECLARE_CLASSNAME( KGCInviteUserList );
    KGCInviteUserList(void);
    virtual ~KGCInviteUserList(void);

    void InitUserList();
    void SetTitle( const std::wstring& strTitle );
    void SetUserList();
	void SetFriendUserList();

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void OnPrevBtn();
    void OnNextBtn();
    void OnInviteBtn();
    void OnUserList( KD3DWnd* pWnd, DWORD dwCode );

    KD3DSizingBox*  m_pkBack;
    KD3DStatic*     m_pkTitle;
    KD3DWnd*        m_pkPrevBtn;
    KD3DWnd*        m_pkNextBtn;
    KD3DWnd*        m_pkInviteBtn;
    KSafeArray<KGCInviteUserInfo*,INVITE_USER_NUM> m_pkInviteUserInfo;
};

DEFINE_WND_FACTORY( KGCInviteUserList );
DECLARE_WND_FACTORY( KGCInviteUserList );
DECLARE_WND_FACTORY_NAME( KGCInviteUserList );