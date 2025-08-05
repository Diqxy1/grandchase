#pragma once

class KGCCoordiViewChar;
class KGCCoordiViewItemWnd;

class KGCCoordiViewDlg : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoordiViewDlg );
	/// Default constructor
	KGCCoordiViewDlg( void );
	/// Default destructor
	virtual ~KGCCoordiViewDlg( void );
	/// Copy constructor
	KGCCoordiViewDlg( const KGCCoordiViewDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoordiViewDlg& operator=( const KGCCoordiViewDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
	enum {
		COORDI_LOOK_SLOT		= 3,
		EM_INVEN_COLUMN			= 4,
		EM_INVEN_ROW			= 6,
		EM_INVEN_WIDH_SIZE		= 64,
		EM_INVEN_HEIGHT_SIZE	= 64,
		COORDI_VIEW_EQUIP_MAX	= 12,
		COORDI_VIEW_LIST_MAX	= 24
	};

	DWORD s_aItemCategory[COORDI_VIEW_EQUIP_MAX] =
	{
		ESP_HELMET,
		ESP_JACKET,
		ESP_PANTS,
		ESP_GLOVES,
		ESP_SHOES,
		ESP_MANTLE,
		ESP_A_UPPER_HELMET,
		ESP_A_DOWN_HELMET,
		ESP_A_JACKET,
		ESP_A_PANTS,
		ESP_A_ARM,
		ESP_WEAPON,
	};

	std::wstring				m_strFilter;
	KD3DWnd*					m_pkClose;
	GCItem*						m_pItem;
	int							m_iScrollpos;
	KD3DWnd*					m_pkBackground;
	KD3DWnd*					m_pkBtnClearLook;
	KGCSearchBar*				m_pkSearchBar;
	KD3DScrollbar*				m_pkScrollbar;
	KD3DComboBox*				m_pkComboItemType;
	KGCCoordiViewChar*          m_pkCoordiCharView;

public:
	int															 m_iCurrentSlot;
	std::vector< std::pair< int, int > >						 CoordiViewVector;
	KSafeArray< KD3DWnd*, COORDI_LOOK_SLOT >					 m_vecCoordiLookSlot;
	KSafeArray< KD3DWnd*, COORDI_LOOK_SLOT >					 m_vecCoordiLookSlotOn;
	std::vector< std::pair< int, int > >		     		     m_vecCurrentCoordiViewItemList;
	std::map< DWORD, std::vector< std::pair< GCITEMID, int > > > m_mapItemInfo;
	std::vector< KGCCoordiViewItemWnd*>							 m_vecCoordiViewList;
	KSafeArray< KGCCoordiViewItemWnd*, COORDI_VIEW_EQUIP_MAX >   m_vecCoordiViewEquip;
	std::map< int, std::vector< GCITEMID > >					 m_mapSlotInfo;
	std::vector< GCITEMID >										 m_vecCurrentEquipedItems;

	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void OnClose( void );
	void OnScrollPos( void );
	void FilterItems( void );
	void CreateInvenSlot( void );
	void SetTypeCombobox( void );
	void ClearEquipedItems( void );
	void CheckItemInformation( void );
	void SetCoordiViewSavedSlot( void );
	void CheckCoordiViewerSlots( void );
	int  GetCategorySlot( DWORD CurCat );
	void UpdateCoordiViewItemList( void );
	void UpdateSlotCoordiViewList( void );
	int  GetSelectedCategory( int CurSel );
	void UpdateViewListSelectedItems( void );
	void SetEquippedItemFromSlot( int iSlot );
	int  GetTypeFromItemSlot( GCITEMID iItemID );
	void FindItemFromName( const WCHAR* strName_ );
	void DrawItemInfo( GCItem* pItem, D3DXVECTOR2 vPos );
	void SetEquipedSlotItem( int ItemIndex, GCITEMID iItemID, int iType );
};

DEFINE_WND_FACTORY( KGCCoordiViewDlg );
DECLARE_WND_FACTORY( KGCCoordiViewDlg );
DECLARE_WND_FACTORY_NAME( KGCCoordiViewDlg );