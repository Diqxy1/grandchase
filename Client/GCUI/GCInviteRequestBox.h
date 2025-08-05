#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "KGCInviteManager.h"

class KGCInviteUserList;
class KGCInviteSquareList;
class KD3DComboBox;

class KGCInviteRequestBox:  public KD3DDialog,
                            public KActionListener
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCInviteRequestBox( const KGCInviteRequestBox& );
    const KGCInviteRequestBox& operator=( const KGCInviteRequestBox& );

public:
    DECLARE_CLASSNAME( KGCInviteRequestBox );
    KGCInviteRequestBox(void);
    virtual ~KGCInviteRequestBox(void);

    virtual bool InitDialog( IN const KD3DDialog::KDialogInfo& kInfo_ );

    void SetModeButton( KGCInviteManager::INVITE_TYPE eType );
    void OnChannelMode();
    void OnSquareMode();	
	void OnFriendMode();
	void OnGuildMode();
    void OnClose();

    void UpdateSquareInfo();
    void UpdateUserList();
	void UpdateFriendUserList();
    void UpdateList();
    void SetUserListTitle( const std::wstring& strTitle );
    void SetSquareListTitle( const std::wstring& strTitle );	
	void OnInitInviteType();
	void SetGuildMemberList();

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete();
    virtual void OnDestroyComplete();
    virtual void FrameMoveInEnabledState();

    void OnComboPosition_SelChange();    

private:
    KD3DWnd*    m_pkCloseBtn;
//     KD3DWnd*    m_pkChannelMode;
//     KD3DWnd*    m_pkSquareMode;
//     KD3DWnd*    m_pkChannelActive;
//     KD3DWnd*    m_pkSquareActive;
// 	KD3DWnd*    m_pkFriendMode;
// 	KD3DWnd*    m_pkFriendActive;
    KD3DComboBox*   m_pkComboPosition;
    KGCInviteUserList*  m_pkUserList;
    KGCInviteSquareList*    m_pkSquareList;
    //DWORD       m_dwListReqTime;
};

DEFINE_WND_FACTORY( KGCInviteRequestBox );
DECLARE_WND_FACTORY( KGCInviteRequestBox );
DECLARE_WND_FACTORY_NAME( KGCInviteRequestBox );