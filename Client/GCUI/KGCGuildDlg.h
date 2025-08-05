#pragma once

class KD3DStatic;
class KGCCommonEditBox;
class KGCGuildInfoDlg;
class KGCGuildMemberListDlg;
class KGCGuildManageDlg;
class KGCGuildLevelList;

class KGCGuildDlg :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildDlg );
    /// Default constructor
    KGCGuildDlg( void );
    /// Default destructor
    virtual ~KGCGuildDlg( void );
    /// Copy constructor
    KGCGuildDlg( const KGCGuildDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildDlg& operator=( const KGCGuildDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum EGuildTab
	{
		GUILD_INFO_TAB = 0,
		GUILD_MEMBER_TAB = 1,
		GUILD_MANAGE_TAB = 2,
        GUILD_LEVEL_TAB = 3,
	};

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd*            	m_pkGuildInfoTabBtn;
    KD3DWnd*            	m_pkGuildInfoTabActive;
    KD3DWnd*            	m_pkGuildMemberTabBtn;
    KD3DWnd*            	m_pkGuildMemberTabActive;
	KD3DWnd*            	m_pkGuildManageTabBtn;
	KD3DWnd*            	m_pkGuildManageTabActive;
    KD3DWnd*            	m_pkGuildLevelTabBtn;
    KD3DWnd*            	m_pkGuildLevelTabActive;
    KD3DWnd*            	m_pkCloseBtn;

	KGCGuildInfoDlg*		m_pkGuildInfoDlg;
    KGCGuildMemberListDlg*	m_pkGuildMemberList;    
	KGCGuildManageDlg*		m_pkGuildManageDlg;
    KGCGuildLevelList*      m_pkGuildLevelList;

public:
    void SwapGuildTab( EGuildTab eTab_ );    
    void OnGuildInfoDlg( void );
	void OnGuildMemberListDlg( void );
	void OnGuildManageDlg( void );
    void OnGuildLevelDlg ( void );
    void OnClose( void );
    void UpdateMemberList();
	void UpdateGuildMangeDlg( int iType_ );    
	void SetGuildNotice( std::wstring strGuildNotice, int iType_);
	void SetClickSubMenu( bool bClick_ );
	void UpdateAll();	
};

DEFINE_WND_FACTORY( KGCGuildDlg );
DECLARE_WND_FACTORY( KGCGuildDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildDlg );