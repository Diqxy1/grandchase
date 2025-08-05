#pragma once

class KGCCommonItemWnd;

class KGCNewForgeItemBreakUp : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCNewForgeItemBreakUp );

	const static int m_iMAX_ITEM_VIEW_WIDTH_COUNT = 5;
	const static int m_iMAX_ITEM_VIEW_HEGHIT_COUNT = 9;


	/// Default constructor
	KGCNewForgeItemBreakUp( void );
	/// Default destructor
	virtual ~KGCNewForgeItemBreakUp( void );
	/// Copy constructor
	KGCNewForgeItemBreakUp( const KGCNewForgeItemBreakUp& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCNewForgeItemBreakUp& operator=( const KGCNewForgeItemBreakUp& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	virtual void OnInitialize();

	void OnCreateItemImgList( void );
	void OnInitItemImgList( void );
	void OnClickItem( const KActionEvent& event );
	void ClickItem( GCITEMUID giItem, KGCCommonItemWnd* pWnd );
	void FrameMoveInEnabledState( void );
	void RefreshItemList( void );
	void OnScrollPos();
	void SetItemList();

	void FilterInventoryList(const std::wstring& strFilter);

private:

	std::vector<std::pair<GCITEMUID,KGCCommonItemWnd*>> m_vecBreakableItemList;
	std::vector< GCITEMUID >							m_vecBreakableUIDItemList;
	KD3DScrollbar*										m_pkScrollbar;
	int													m_iScrollPos;
	std::wstring										m_strFilter;
};

DEFINE_WND_FACTORY( KGCNewForgeItemBreakUp );
DECLARE_WND_FACTORY( KGCNewForgeItemBreakUp );
DECLARE_WND_FACTORY_NAME( KGCNewForgeItemBreakUp );
