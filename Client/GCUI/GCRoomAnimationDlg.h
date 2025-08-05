#pragma once

class KGCCoordiViewItemWnd;

class KGCRoomAnimationDlg : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRoomAnimationDlg );
	/// Default constructor
	KGCRoomAnimationDlg( void );
	/// Default destructor
	virtual ~KGCRoomAnimationDlg( void );
	/// Copy constructor
	KGCRoomAnimationDlg( const KGCRoomAnimationDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRoomAnimationDlg& operator=( const KGCRoomAnimationDlg& )
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
	std::vector< KGCCoordiViewItemWnd*>	 m_vecRoomAnimationList;
	std::vector < RoomAnimationInfo >    m_vecRoomAnimationInfo;
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed(const KActionEvent& event);
	void FilterRemoveOtherChar( void );
	void OnScrollPos( void );
	void CreateRoomAnimationSlot( void );
	void CheckItemInformation(void);
	void UpdateSlotRoomAnimationList( void );
	void UpdateRoomAnimationItemList( void );
	void DrawItemInfo( GCItem* pItem, D3DXVECTOR2 vPos );
};
DEFINE_WND_FACTORY( KGCRoomAnimationDlg );
DECLARE_WND_FACTORY( KGCRoomAnimationDlg );
DECLARE_WND_FACTORY_NAME( KGCRoomAnimationDlg );