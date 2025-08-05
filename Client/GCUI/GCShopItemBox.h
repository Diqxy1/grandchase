#ifndef _GCSHOPITEMBOX_H_
#define _GCSHOPITEMBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../GCItem.h"

class KGCShopItemWnd;
class KGCSetItemWnd;
class KD3DScrollbar;
class KD3DStatic;

enum ShopTabType
{
	TAB_ARMOR = 0,
	TAB_SET,
	TAB_WEAPON,
	TAB_ACC, 
	TAB_TRAIN,
	TAB_PET,
	TAB_COUPLE,
    TAB_POINTITEM,
	TAB_ACC2,		//포인트 事?위한 엣지가 있는 버튼
	NUM_TAB
};

class KGCShopItemBox : public KD3DWnd,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCShopItemBox );
	/// Default constructor
	KGCShopItemBox( void );
	/// Default destructor
	virtual ~KGCShopItemBox( void );
	/// Copy constructor
	KGCShopItemBox( const KGCShopItemBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCShopItemBox& operator=( const KGCShopItemBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum
	{
		NUM_ITEM_WND = 16,
		NUM_SET_ITEM_WND = 8,
		NUM_ITEM_SCROLL = 8,
	};

	enum
	{
		START_GAP = 8,
		BETWEEN_X_GAP = 2,
        BETWEEN_Y_GAP = 9,
		//MAX_ITEMWND_COLUMN = 4,
		MAX_ITEMWND_ROW = 4,
		//MAX_SETITEMWND_COLUMN = 4,
		MAX_SETITEMWND_ROW = 2,
	};

public:
	bool IsEquip( int iIndex ) const;
	void ResetScrollbar( int iPos );
	void SetSetItemView( bool bSetItemView );
	GCItem*& GetItemInfo( int iIndex );
	virtual void ActionPerformed( const KActionEvent& event );
	void SetShopItemView( EGCCharType CharType, ShopTabType TabType,
							int iCategory = -1, int iSortType = -1,  int iMoneyType = -1,
							const WCHAR* strName_ = NULL );
	bool GetCurrentHoveredItem( GCItem*& pItem, D3DXVECTOR2 &vecPos );
    void UpdateEquip( void );
    void RefreshList();

protected:
	virtual void OnActivateComplete( void );
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	void GetTotalPage();
	void SetPage(int iPage);
	void UpdateMarks( void );
	inline int AdjustMaxItemNum( int iSrc )
	{
		return ( ( iSrc / NUM_ITEM_SCROLL ) + ( 0 == iSrc % NUM_ITEM_SCROLL ? 0 : 1 ) ) * NUM_ITEM_SCROLL;
	}
	DWORD GetItemRecomInfo( int iIndex );

protected:
	bool            m_bSetItemView;
	EGCCharType		m_eCharType;

	KSafeArray<KGCShopItemWnd*,NUM_ITEM_WND> m_apItemWnd;
	KSafeArray<KGCSetItemWnd*,NUM_SET_ITEM_WND> m_apSetItemWnd;
	//KD3DScrollbar*  m_pkScrollbar;

	//==================Page UI=======================//
	KD3DWnd*		m_pkScrollBtnFirst;
	KD3DWnd*		m_pkScrollBtnPrev;
	KD3DWnd*		m_pkScrollBtnNext;
	KD3DWnd*		m_pkScrollBtnLast;
	KD3DStatic*     m_pkStaticPage;

	int				m_iTotalPage;
	int				m_iCurrentPage;
	DWORD			m_kCurInfoItemKey;
	GCItem*			m_kCurInfoItem;
};

DEFINE_WND_FACTORY( KGCShopItemBox );
DECLARE_WND_FACTORY( KGCShopItemBox );
DECLARE_WND_FACTORY_NAME( KGCShopItemBox );

#endif // _GCSHOPITEMBOX_H_