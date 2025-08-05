#ifndef _GC_QUANTITY_PURCHASE_BOX_H_
#define _GC_QUANTITY_PURCHASE_BOX_H_

class KGCQuantityPurchaseBox : public KD3DDialog,
										public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCQuantityPurchaseBox );
	KGCQuantityPurchaseBox(void);
	virtual ~KGCQuantityPurchaseBox(void);

	KGCQuantityPurchaseBox( const KGCQuantityPurchaseBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCQuantityPurchaseBox& operator=( const KGCQuantityPurchaseBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void FrameMoveInEnabledState( void );
	virtual void OnCreateComplete( void );
	virtual bool CheckCondition( IN const KDialogInfo& kInfo_ );

private:

	void OnEnterCursor();
	void OnExit();
	void OnDown();
	void OnUp();
	void OnBuy();
	void OnTextEnter();

private:
	bool CheckAvailableNum( int iNum );
	int GetAvailableNum();
	void SetPurchaseNum( int iNum );
	DWORD GetPrice();

private:
	KD3DWnd*	m_pkBtnUp;
	KD3DWnd*	m_pkBtnDown;
	KD3DWnd*	m_pkBtnPurchase;
	KD3DWnd*	m_pkBtnClose;
	KD3DStatic*	m_pkPrice;
	KD3DStatic*	m_pkTotal;
	KD3DStatic*	m_pkItemNameStatic;
	KD3DStatic*	m_pkPurchaseNumStatic;
	KD3DStatic*	m_pkTotalPriceStatic;
	KD3DEdit*	m_pkCount;
	KSafeArray< KD3DWnd*, 2 > m_saGPMark;
	KSafeArray< KD3DWnd*, 2 > m_saPPMark;
	KSafeArray< KD3DWnd*, 2 > m_saVPMark;
	KSafeArray< KD3DWnd*, 2 > m_saDiaMark;
	KSafeArray< KGCItemImgWnd*, 2 > m_saHeroMark;

	KGCItemImgWnd*	m_pkItemImg;
	GCItem*	m_pkItem;
	int	m_uiPrice;
	int m_uiQuantity;
	int m_iPressCount;
	int m_eMoneyType;
};

DEFINE_WND_FACTORY( KGCQuantityPurchaseBox );
DECLARE_WND_FACTORY( KGCQuantityPurchaseBox );
DECLARE_WND_FACTORY_NAME( KGCQuantityPurchaseBox );

#endif //_GC_QUANTITY_PURCHASE_BOX_H_
