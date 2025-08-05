#pragma once

class KD3DStatic;
class KGCGuildJoinWaitingUserInfo;

class KGCGuildJoinManageDlg :	public KD3DWnd,
	public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildJoinManageDlg );
    /// Default constructor
    KGCGuildJoinManageDlg( void );
    /// Default destructor
    virtual ~KGCGuildJoinManageDlg( void );
    /// Copy constructor
    KGCGuildJoinManageDlg( const KGCGuildJoinManageDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildJoinManageDlg& operator=( const KGCGuildJoinManageDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{	
		MAX_WAITING_USER_NUM = 7,
	};

	enum ALIGN_BY_TYPE
	{
		ABT_BY_GRADE,
		ABT_BY_NICKNAME,
		ABT_BY_APPLICATION_DATE,

		ABT_MAX
	};

	struct KWaitingJoinUser
	{
		KNGuildUserInfo m_sJoinUserInfo;
		bool			m_bSelectCheckBox;
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
	KSafeArray<KGCGuildJoinWaitingUserInfo*, MAX_WAITING_USER_NUM>		m_pkWaitingUserInfo;
	KSafeArray<KD3DWnd*, MAX_WAITING_USER_NUM>							m_pkCheckBox;
	KSafeArray<KD3DWnd*, MAX_WAITING_USER_NUM>							m_pkCheckBoxSign;
	KSafeArray<KD3DWnd*, ABT_MAX>										m_pkBtnSorting;
	KSafeArray<KD3DWnd*, ABT_MAX>										m_pkSortingUp;
	KSafeArray<KD3DWnd*, ABT_MAX>										m_pkSortingDown;	


    KD3DWnd*										m_pkBtnJoinAllow;
    KD3DWnd*										m_pkBtnJoinReject;
	KD3DWnd*										m_pkBtnCancel;

	KD3DStatic*										m_pkStaticJoinCondition;
	KD3DStatic*										m_pkStatinJoinWaitingUserNum;

	KD3DScrollbar*									m_pkScrollBar;
	KD3DSizingBox*									m_pkHoverSignBox;
	KD3DSizingBox*									m_pkListBackground;

	float											m_fOriginYPos;
	float											m_fRealYPos;
	int												m_iPrevUserListIndex;

	ALIGN_BY_TYPE									m_eAlignType;

	std::vector<KWaitingJoinUser>					m_vecJoinWaitingUserInfo;

	typedef bool (*SORTFUNC)( const KWaitingJoinUser& lhs, const KWaitingJoinUser& rhs, bool bAscending_);
	std::map< ALIGN_BY_TYPE, std::pair< SORTFUNC, bool> > m_mapSortFunc;

	static bool SortByGrade( const KWaitingJoinUser& lhs, const KWaitingJoinUser& rhs, bool bAscending_);
	static bool SortByNickName( const KWaitingJoinUser& lhs, const KWaitingJoinUser& rhs, bool bAscending_);	
	static bool SortByApplicationDate( const KWaitingJoinUser& lhs, const KWaitingJoinUser& rhs, bool bAscending_);
	
public:
	void OnJoinAllow(void);
	void OnJoinReject(void);
	void OnCancel(void);
	void OnScroll(void);
	void ToggleRenderSortArrow( ALIGN_BY_TYPE eWhatSorting_, bool bUpSorting_ );
	void OnSortBtnClick( ALIGN_BY_TYPE eAlignType_, bool bOpenDlg_ = false );
	void OnSortWaitingUser( SORTFUNC pFunc_ , bool bAscending_ );
	int GetShowWaitingUserCount(int iFileIndex_);
	void InitScroll(void);
	void UpdateWaitingUserInfo(void);
	void SetJoinWaitingUserInfo( std::map<DWORD,KNGuildUserInfo>& mapJoinWaitingUserInfo_ );
	void SetCursorBoxOffset( D3DXVECTOR2 vOffset_ );
	SET_DWORD GetCheckUserSet();
};

DEFINE_WND_FACTORY( KGCGuildJoinManageDlg );
DECLARE_WND_FACTORY( KGCGuildJoinManageDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildJoinManageDlg );