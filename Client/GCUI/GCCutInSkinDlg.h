#pragma once

class KGCCoordiViewItemWnd;

class KGCCutInSkinDlg : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCutInSkinDlg );
	/// Default constructor
	KGCCutInSkinDlg( void );
	/// Default destructor
	virtual ~KGCCutInSkinDlg( void );
	/// Copy constructor
	KGCCutInSkinDlg( const KGCCutInSkinDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCutInSkinDlg& operator=( const KGCCutInSkinDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
	enum {
		EM_INVEN_COLUMN			= 	2,
		EM_INVEN_ROW			= 	2,
		EM_INVEN_WIDH_SIZE		= 248,
		EM_INVEN_HEIGHT_SIZE	= 199
	};
	KD3DStatic*				m_pkStaticExplanation;
	int						m_iScrollpos;
	KD3DScrollbar*			m_pkScrollbar;
	char					m_pCharType;

public:
	int									 m_iCurrentSlot;
	std::vector< KGCCoordiViewItemWnd*>	 m_vecCutInSkinList;
	std::vector < CutInSkinInfo >		 m_vecCutInSkinInfo;
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed(const KActionEvent& event);

	void OnScrollPos( void );
	void FilterRemoveOtherChar( void );
	void CreateCutInSkinSlot( void );
	void CheckItemInformation(void);
	void UpdateSlotCutInSkinList( void );
	void UpdateCutInSkinItemList( void );
	void DrawItemInfo( GCItem* pItem, D3DXVECTOR2 vPos );
};
DEFINE_WND_FACTORY( KGCCutInSkinDlg );
DECLARE_WND_FACTORY( KGCCutInSkinDlg );
DECLARE_WND_FACTORY_NAME( KGCCutInSkinDlg );