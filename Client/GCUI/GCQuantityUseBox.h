#pragma once

class KGCCoordiViewItemWnd;

class KGCQuantityUseBox : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCQuantityUseBox );
	/// Default constructor
	KGCQuantityUseBox( void );
	/// Default destructor
	virtual ~KGCQuantityUseBox( void );
	/// Copy constructor
	KGCQuantityUseBox( const KGCQuantityUseBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCQuantityUseBox& operator=( const KGCQuantityUseBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
	KD3DWnd*					m_pkClose;
	KD3DWnd*					m_pkBtnUp;
	KD3DWnd*					m_pkQntback;
	KD3DWnd*					m_pkBtnDown;
	KD3DStatic*				    m_pkItemName;
	KGCCoordiViewItemWnd*       m_pkItemImage;
	KD3DWnd*					m_pkBackground;
	KD3DWnd*					m_pkTitleChest;
	KD3DWnd*					m_pkBtnConfirm;
	KD3DWnd*					m_pkTitleCoupon;
	KD3DStatic*				    m_pkQuantityDesc;
	KD3DEdit*				    m_pkCount;
	KGCCoordiViewChar*          m_pkCoordiCharView;

	int							m_uiQuantity;
	int							m_iPressCount;

public:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void OnUp();
	void OnDown();
	void OnClose();
	void OnConfirm();
	void OnTextEnter();
	int  GetAvailableNum();
	void SetUseNum( int iNum );
	bool CheckAvailableNum( int iNum );
};

DEFINE_WND_FACTORY( KGCQuantityUseBox );
DECLARE_WND_FACTORY( KGCQuantityUseBox );
DECLARE_WND_FACTORY_NAME( KGCQuantityUseBox );