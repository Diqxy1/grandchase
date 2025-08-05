#pragma once

class KGCAgitSlot;

class KGCAgitList : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum TAB_TYPE
    {
        TAB_AGIT_LIST_DEFAULT = 0, TAB_AGIT_LIST_ACTIVE = 1, TAB_FRIEND_AGIT_DEFAULT = 2, TAB_FRIEND_AGIT_ACTIVE = 3,
        TAB_LIKE_AGIT_DEFAULT = 4, TAB_LIKE_AGIT_ACTIVE = 5, TAB_RANK_AGIT_DEFAULT = 6, TAB_RANK_AGIT_ACTIVE = 7,
        NUM_TAB = 8
    };

    enum BTN_TYPE { BTN_FIRST = 0, BTN_PREV = 1, BTN_END = 2, BTN_NEXT = 3, BTN_COLSE = 4, BTN_VISIT = 5, BTN_DELETE = 6, NUM_BTN = 7 };
    enum CHECK_TYPE { CHECKBOX_FULL_OPTION = 0, CHECKBOX_HIDE_OPTION = 1, CHECK_FULL_OPTION = 2, CHECK_HIDE_OPTION =3, NUM_CHECK = 4 };
    enum { NUM_AGIT_SLOT = 11 };	
    
public:
    DECLARE_CLASSNAME( KGCAgitList );
    /// Default constructor
    KGCAgitList( void );
    /// Default destructor
    virtual ~KGCAgitList( void );
    /// Copy constructor
    KGCAgitList( const KGCAgitList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitList& operator=( const KGCAgitList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DComboBox*   m_pkNumUserCombo;
    KD3DComboBox*   m_pkChannelCombo;

    KD3DStatic*     m_pkStaticFullOption;
    KD3DStatic*     m_pkStaticHideOption;
    KD3DStatic*     m_pkStaticNumUserOption;
    KD3DStatic*     m_pkStaticPage;

    KSafeArray< KD3DWnd*, NUM_TAB >     m_apkTab;
    KSafeArray< KD3DWnd*, NUM_BTN >     m_apkBtn;
    KSafeArray< KD3DWnd*, NUM_CHECK >   m_apkCheck;
    KSafeArray< KGCAgitSlot*, NUM_AGIT_SLOT >   m_apkAgitSlot;
    KD3DSizingBox*  m_pkActiveSlotFrame;

    bool   m_bShowNotFullOnly;
    bool   m_bShowOpenOnly;
    USHORT m_usShowMinUser;
	int	   m_nChannel;
	int	   m_nMaxChannel;

    USHORT m_usCurPage;
    USHORT m_usMaxPage;
    std::vector< KAgitPageInfo >    m_vecRegisterPageInfo;
	std::vector< KAgitPageInfo >   m_vecFriendPageInfo;
	std::vector< KAgitPageInfo >   m_vecFavoritePageInfo;
	std::vector< KAgitPageInfo >   m_vecRankPageInfo;


    int      m_iCurSlotIndex;
    TAB_TYPE m_eCurTab;


    // 랭킹 관련
    KD3DSizingBox*  m_pkTodayRankFrame;
    KD3DSizingBox*  m_pkTotalRankFrame;
    KD3DSizingBox*  m_apkPageBack;	
    KD3DStatic*     m_pkStaticTodayRank;
    KD3DStatic*     m_pkStaticTotalRank;
    KD3DStatic*     m_pkStaticPageDesc;


	//	서버 정보 재연결 관련
	DWORD	m_dwBackCnt;

public:
	void SetRegisterAgitList ( KAgitPageList& PageInfo_, std::vector< KAgitPageInfo >& vecPageList_ );
	void SetFriendAgitList ( std::vector< KAgitPageInfo >& vecFriendPageInfo_ );
	void SetFavoriteAgitList ( std::vector< KAgitPageInfo >& vecFavoritePageInfo_ );
	void SetRankAgitList ( std::vector< KAgitPageInfo >& vecRankPageInfo_ );

	void InitData();

	void SetMaxChannel( int nMaxChannel );

	void ReqUpdateFavoritePage();

private:
    void UpdateAgitList();
	void UpdateUIVisible();
	void UpdateRegisterAgit();
	void UpdateFriendAgit();
	void UpdateFavoriteAgit();
	void UpdateRankAgit();

	void ChangePage( TAB_TYPE eCurTab );
	void UpdateTabUIVisible();

	void InitControl();


	
	
	bool IsReqAgitInfo( TAB_TYPE eCurTab );

	void UpdateRegisterAgitSlot( void );
	void UpdateFriendAgitSlot( void );
	void UpdateFavoriteAgitSlot( void );
	void UpdateRankAgitSlot( void );
	
	void UpdatePage( BTN_TYPE emBtnType );
	void UpdatePageFirst();
	void UpdatePagePrev();
	void UpdatePageEnd();
	void UpdatePageNext();





//  void UpdateComboList( void );
//  void UpdatePageStatic( void );

    void Clear( void );
    
    void OnClickTab( TAB_TYPE eType_ );
    void OnClickSlot( int iIndex_ );
    void OnVisit( void );
    void OnClose( void );
	void OnDeleteFavority( void );
	void OnCheckFull( void );
	void OnCheckHide( void );
	void OnNumUserComboChange( void );
	void OnChannelComboChange( void );
    void VisibleUserList( bool bIsVisible );
};

DEFINE_WND_FACTORY( KGCAgitList );
DECLARE_WND_FACTORY( KGCAgitList );
DECLARE_WND_FACTORY_NAME( KGCAgitList );
