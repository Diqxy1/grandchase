#pragma once

class KD3DStatic;
class KGCGradeMark;
class KGCGuildMemberManageDlg;
class KGCGuildJoinManageDlg;
class KGCGuildSettingChangeDlg;

class KGCGuildManageDlg :	public KD3DWnd,
	public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildManageDlg );
    /// Default constructor
    KGCGuildManageDlg( void );
    /// Default destructor
    virtual ~KGCGuildManageDlg( void );
    /// Copy constructor
    KGCGuildManageDlg( const KGCGuildManageDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildManageDlg& operator=( const KGCGuildManageDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum EGUILD_MANAGE_BTN{
		BTN_MEMBER_MANAGE	=0,
		BTN_SETTING_MANAGE,
		BTN_JOIN_MANAGE,
		BTN_MAX,
	};

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:
	KGCGuildMemberManageDlg*	m_pkGuildMemberManageDlg;    
	KGCGuildJoinManageDlg*		m_pkGuildJoinManageDlg;
	KGCGuildSettingChangeDlg*	m_pkGuildSettingManageDlg;
    KD3DWnd*					m_pkBtnMemberManage;
    KD3DWnd*					m_pkBtnSettingManage;
	KD3DWnd*					m_pkBtnJoinManage;
	KD3DWnd*					m_pkActiveMemberManage;
	KD3DWnd*					m_pkActiveSettingManage;
	KD3DWnd*					m_pkActiveJoinManage;

public:
	void OnSelectDlg(EGUILD_MANAGE_BTN eGuildManageBtn_ );
	void UpdateAllManageDlg();
};

DEFINE_WND_FACTORY( KGCGuildManageDlg );
DECLARE_WND_FACTORY( KGCGuildManageDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildManageDlg );