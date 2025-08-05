#pragma once

class KGCCoordiViewItemWnd;

class KGCFontSkinDlg : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCFontSkinDlg );
	/// Default constructor
	KGCFontSkinDlg( void );
	/// Default destructor
	virtual ~KGCFontSkinDlg( void );
	/// Copy constructor
	KGCFontSkinDlg( const KGCFontSkinDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCFontSkinDlg& operator=( const KGCFontSkinDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
	enum {
		EM_INVEN_COLUMN			=  5,
		EM_INVEN_ROW			=  4,
		EM_INVEN_WIDH_SIZE		= 98,
		EM_INVEN_HEIGHT_SIZE	= 98
	};

	KD3DStatic*				m_pkStaticExplanation;
	int						m_iScrollpos;
	KD3DScrollbar*			m_pkScrollbar;

public:
	int									 m_iCurrentSlot;
	std::vector< KGCCoordiViewItemWnd*>	 m_vecFontSkinList;
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed(const KActionEvent& event);
	void OnScrollPos( void );
	void CreateFontSkinSlot( void );
	void CheckItemInformation(void);
	void UpdateSlotFontSkinList( void );
	void UpdateFontSkinItemList( void );
	void DrawItemInfo( GCItem* pItem, D3DXVECTOR2 vPos );
};

DEFINE_WND_FACTORY( KGCFontSkinDlg );
DECLARE_WND_FACTORY( KGCFontSkinDlg );
DECLARE_WND_FACTORY_NAME( KGCFontSkinDlg );