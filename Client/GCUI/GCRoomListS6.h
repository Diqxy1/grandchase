#pragma once

class KGCRoomBtnS6;

class KGCRoomListS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomListS6 );
    /// Default constructor
    KGCRoomListS6( void );
    /// Default destructor
    virtual ~KGCRoomListS6( void );
    /// Copy constructor
    KGCRoomListS6( const KGCRoomListS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomListS6& operator=( const KGCRoomListS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		KRL_ROOM_LIST_TYPE_DUNGEON = 0,
		KRL_ROOM_LIST_TYPE_MATCH = 1,
		KRL_LEVEL_MAX_NUM = 7,
		KRL_ROOM_MAX_NUM = 7,
		KRL_MATCH_ROOM_MAX_NUM = 10,
	};


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	bool CreateRoomListUI( void );
	void InitRoomList( void );
	void SetRoomListPos( void );
	void RenderOffRoomList( void );
	void UpdateRoomList( void );
	void OnPrevPage();
	void OnNextPage();
	void FrameMoveInEnabledState();
	void OnAllRoom();
	void OnWaitRoom();
	void UpdateLevelTitle( void );
	void OnClickRoomListBar(const KActionEvent& event);
	void ClickRoomListBar( KGCRoomBtnS6* pWnd );
	void Update( void );
	void SetRoomListType(int iType) { m_iRoomListType = iType; }
	void OnRoomFilterBox();
	void OnFilterBtnClick();
private:
	//<방UID, 방UI객체포인트>
	std::vector<std::pair<DWORD,KGCRoomBtnS6*>> m_vecRoomList;

	KD3DWnd *m_pkPrePageBtn;
	KD3DWnd *m_pkNextPageBtn;

	KD3DWnd *m_pkAllBtn;
	KD3DWnd *m_pkWatingBtn;

	KSafeArray<KD3DWnd*,KRL_LEVEL_MAX_NUM> m_saLevelTitle;

	int m_iRoomListType;
	KGCRoomFilterBox*   m_pkRoomFilterBox;
	KGCCategoryBox *m_pkCategoryTitle;

	KD3DWnd* m_pkFilterBtn;
	KD3DWnd* m_pkFilterOnBtn;
};

DEFINE_WND_FACTORY( KGCRoomListS6 );
DECLARE_WND_FACTORY( KGCRoomListS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomListS6 );
