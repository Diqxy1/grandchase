#pragma once

class KD3DStatic;
class KGCGuildListInfo;

class KGCGuildListDlg :	public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildListDlg );
	/// Default constructor
	KGCGuildListDlg( void );
	/// Default destructor
	virtual ~KGCGuildListDlg( void );
	/// Copy constructor
	KGCGuildListDlg( const KGCGuildListDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildListDlg& operator=( const KGCGuildListDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum
	{
		MAX_GUILD_LIST_NUM = 8,
		MAX_SORTING_BTN = 3,
		MAX_SEARCH_WORD_LENGTH = 12,
		SEARCH_LIMIT_TIME = 5,
	};

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

private:
	KSafeArray<KGCGuildListInfo*, MAX_GUILD_LIST_NUM>	m_pkGuildListInfo;
	KSafeArray<KD3DWnd*, MAX_SORTING_BTN>				m_pkSortArrowUp;
	KSafeArray<KD3DWnd*, MAX_SORTING_BTN>				m_pkSortArrowDown;
    KD3DWnd*            m_pkBtnClose;
    KD3DWnd*            m_pkBtnGuildList;
	KD3DWnd*            m_pkBtnGuildCreate;
    KD3DWnd*			m_pkBtnSearch;
    KD3DWnd*			m_pkBtnJoinCancel;
	KD3DWnd*			m_pkBtnPrevPage;
	KD3DWnd*			m_pkBtnNextPage;
	KD3DWnd*			m_pkBtnJoinReq;

	KD3DWnd*			m_pkBtnSortingGuild;
	KD3DWnd*			m_pkBtnSortingGuildGrade;
	KD3DWnd*			m_pkBtnSortingGuildBirthday;

	KD3DEdit*			m_pkEditSearch;
	KD3DStatic*         m_pkStaticGuildListPage;
	KD3DStatic*         m_pkStaticMyGuildName;
	KD3DStatic*         m_pkStaticGuildJoinstate;

	KD3DStatic*         m_pkStaticGuildName;
	KD3DStatic*         m_pkStaticGuildGrade;
	KD3DStatic*         m_pkStaticGuildMasterName;
	KD3DStatic*         m_pkStaticGuildBithday;
	KD3DStatic*         m_pkStaticGuildURL;
	KD3DStatic*         m_pkStaticGuildIntroduce;
	KD3DStatic*         m_pkStaticGuildPoint;
	KD3DStatic*         m_pkStaticGuildMemberNum;
	KD3DStatic*         m_pkStaticGuildTotalRanking;
	KD3DStatic*         m_pkStaticGuildMonthRanking;
	KD3DStatic*         m_pkStaticGuildWeekRanking;

	KD3DStatic*			m_pkStaticTitleGuildIntroduce;
	KD3DStatic*			m_pkStaticTitleGuildInfo;
	KD3DStatic*			m_pkStaticGuildJoinCondition;

	KD3DSizingBox*		m_pkListHoverBG;
	KD3DSizingBox*		m_pkGuildSearchingBlind;

	KD3DImageIcon*		m_pkGuildMarkIcon;

	int							m_iCurPage;
	int							m_iMaxPage;
	int							m_iPrevIndex;
	int							m_iMaxListNum;
	int							m_iSelectGuildUID;
	float						m_fListHoverBoxPosY;
	std::vector<KNGuildInfo>	m_vecDetalGuildInfo;
	bool						m_bIsPressedBtn;
	UCHAR						m_ucSortType;
	int							m_iStratSearch;

    std::wstring                m_wstrGuildUrl;

public:
	void	OnNextPage();
	void	OnPrevPage();
	void	OnClose();
	void	OnGuildCreateDlg(void);
	void	OnJoinReq();
	void	UpdateGuildList();
	void	SetDetailGuildInfo( std::vector<KNGuildInfo>& vecDetailGuildInfo_, int iCurPage_, int iMaxPage_ );
	void	UpdateDetailGuildInfo( int iIndex_ );
	void	SetPageInfo( int iCurPage, int iMaxPage );
	void	SortingGuildList( UCHAR ucSortType_ );
	void	SetPressBtn( bool bIsPressed_ ) { m_bIsPressedBtn = bIsPressed_; }
	void	OnSearchKeyword();
	void	ToggleRenderSortArrow( int iWhatSorting_ );
	void	OnJoinCancel();
	void	UpdateWaitingJoinerState();
	void InitStaticData();

private:
	void	MovePage( bool bNext_ );
};

DEFINE_WND_FACTORY( KGCGuildListDlg );
DECLARE_WND_FACTORY( KGCGuildListDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildListDlg );