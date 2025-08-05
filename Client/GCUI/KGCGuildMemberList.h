#pragma once

class KD3DStatic;
class KGCCheckButton;
class KD3DScrollbar;
class KGCGuildMemberInfo;
class KD3DSizingBox;

class KGCGuildMemberListDlg :	public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildMemberListDlg );
    /// Default constructor
    KGCGuildMemberListDlg( void );
    /// Default destructor
    virtual ~KGCGuildMemberListDlg( void );
    /// Copy constructor
    KGCGuildMemberListDlg( const KGCGuildMemberListDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildMemberListDlg& operator=( const KGCGuildMemberListDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        MAX_MEMBER_NUM = 12,
    };

    enum ALIGN_BY_TYPE
    {
        ABT_BY_GUILDGRADE,
        ABT_BY_GRADE,
        ABT_BY_NICKNAME,
		ABT_BY_ACTIVITY,
		ABT_BY_LAST_LOGIN,

        ABT_MAX
    };

	enum ELIST_SUB_MENU
	{
		ELIST_DRUM_OUT =1,
	};

	struct KGuildMemberList
	{
		KNGuildUserInfo m_sGuildMemberInfo;
		bool			m_bSelectList;
	};

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
	KSafeArray<KGCGuildMemberInfo*,MAX_MEMBER_NUM>	m_pkGuildMemberInfo;
	KSafeArray<KD3DWnd*,ABT_MAX>					m_pkSortingArrowUp;
	KSafeArray<KD3DWnd*,ABT_MAX>					m_pkSortingArrowDown;
	KSafeArray<KD3DWnd*,ABT_MAX>					m_pkBtnSorting;

    KD3DScrollbar*      m_pkScrollBar;	

	KD3DSizingBox*		m_pkListBackground;
	KD3DSizingBox*		m_pkHoverSignBox;

	KGCMultiMenu*		m_pkListSubMenu;
	
	bool				m_bUsePopup;
	bool				m_bClickSubMenu;
	int					m_iMaxMemberNum;
	int					m_iPrevUserListIndex;
	float				m_fOriginYPos;
	float				m_fRealYPos;

    std::vector< KGuildMemberList > m_vecShowGuildUserList;
    ALIGN_BY_TYPE   m_eAlignType;
    typedef bool (*SORTFUNC)( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_);
    std::map< ALIGN_BY_TYPE, std::pair< SORTFUNC, bool> > m_mapSortFunc;

    void OnScroll();
    void InitScroll( void );
    void OnSortGuildMember( SORTFUNC pFunc_ , bool bAscending_ );
    void OnSortBtnClick( ALIGN_BY_TYPE eAlignType_, bool bOpenDlg_ = false );
    int GetMemberViewCount(int iFileIndex_);
    static bool SortByNickName( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_);
    static bool SortByGrade( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_);
    static bool SortByGuildGrade( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_);
	static bool SortByGuildActivity( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_);
	static bool SortByGuildLastLogin( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_);
public:
    void UpdateList();
	void ToggleRenderSortArrow( ALIGN_BY_TYPE eWhatSorting_ = ABT_BY_GUILDGRADE, bool bUpSorting_=true );
	void SetMaxMemberNum( int iMaxMemberNum_ ) { m_iMaxMemberNum = iMaxMemberNum_; }
	void SetGuildUserInfo();
	void SetUsePopupMenu(bool bUsePopup_ );
	void SetCursorBoxOffset( D3DXVECTOR2 vOffset_ );
	void SetClickSubMenu( bool bClick_ ) { m_bClickSubMenu = bClick_; }
};

DEFINE_WND_FACTORY( KGCGuildMemberListDlg );
DECLARE_WND_FACTORY( KGCGuildMemberListDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildMemberListDlg );