#pragma once

class KGCZeroMaskDlg : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCZeroMaskDlg );
	/// Default constructor
	KGCZeroMaskDlg( void );
	/// Default destructor
	virtual ~KGCZeroMaskDlg( void );
	/// Copy constructor
	KGCZeroMaskDlg( const KGCZeroMaskDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCZeroMaskDlg& operator=( const KGCZeroMaskDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
	enum {
		ZERO_MASK_MAX = 4
	};

	KD3DWnd*	   			 							m_pkClose;
	KD3DWnd*	   			 							m_pkClear;
	KSafeArray< KD3DWnd*, ZERO_MASK_MAX > 	 			m_pMaskSlot;
	KSafeArray< KD3DWnd*, ZERO_MASK_MAX > 	 			m_pMaskSelected;
	KSafeArray< KGCItemImgWnd*, ZERO_MASK_MAX >			m_pMaskImg;
	int 												m_iCurrentSelectedOption;
	SCharInfo											zeroConfirm;

	void OnClose( void );
	void OnSelect( void );
	void OnClear( void );
public:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void EquipZeroMask( SCharInfo* pCharInfo, int iItemID );
	void UnequipZeroMask( SCharInfo* pCharInfo, bool bUpdate );
};

DEFINE_WND_FACTORY( KGCZeroMaskDlg );
DECLARE_WND_FACTORY( KGCZeroMaskDlg );
DECLARE_WND_FACTORY_NAME( KGCZeroMaskDlg );