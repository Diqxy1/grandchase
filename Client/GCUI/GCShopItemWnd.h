#ifndef _GCSHOPITEMWND_H_
#define _GCSHOPITEMWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KD3DStatic;
class KGCItemBar;
class GCDeviceTexture;
class KGCUINumber;

class KGCShopItemWnd : public KD3DWnd,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCShopItemWnd );
	/// Default constructor
	KGCShopItemWnd( void );
	/// Default destructor
	virtual ~KGCShopItemWnd( void );
	/// Copy constructor
	KGCShopItemWnd( const KGCShopItemWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCShopItemWnd& operator=( const KGCShopItemWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum{
		LAYER_LT = 0,
		LAYER_T = 1,
		LAYER_RT = 2,
		LAYER_L = 3,
		LAYER_C = 4,
		LAYER_R = 5,
		LAYER_LB = 6,
		LAYER_B = 7,
		LAYER_RB = 8,
	};
	enum
	{
		NUM_LAYER_WND           = 9,
		NUM_ACTIVE_FRAME        = 8,
		ENCHANT_BOX_OFFSET_X    = 197,
		ENCHANT_BOX_OFFSET_Y    = -59,
	};

public:
	void SetIndex( int iIndex );
	bool IsEquip( void ) const;
	void Equip( bool bEquip );
	GCItem*& GetItemInfo( void );
	DWORD GetItemKey( void )
	{
		return m_iItemID;
	}
	int GetItemID() { return m_iItemID; }
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void PreDraw();
	void SetItemInfo(int iShopItemIndex);
	void OnActivateComplete( void );

	void SetMask(const int& RecomLevel_);
	void SetVIP( bool bVIP );

protected:
	void ShowItem( bool bShow );
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostDraw( void );
    void SetPointType();

protected:

	int         m_iIndex;
	int         m_iItemID;
	int			m_iPeriod;
	int         m_iPattrn;
	int         m_iDuratn;
	int         m_iStrong;
	DWORD       m_dwUID;

	GCItem*     m_kItemData;

	bool        m_bShow;
	bool        m_bMouseHoverPrev;
	bool        m_bAlreadyPatternUp;
	bool        m_bNotSaleForLevel;
	bool        m_bEvent;
	bool        m_bEquip;

	KD3DSizingBox	*m_pkLayer;
	KD3DWnd*    m_pkImgDrawRect;
	KD3DSizingBox	*m_pkActiveLayer;

	GCDeviceTexture* m_pTextureItem;

	KD3DStatic* m_pkItemName;
	KD3DStatic* m_pkItemLimit;
	KGCItemBar* m_pkItemBar;
	KD3DWnd*    m_pkEquip;
    KGCUINumber*    m_pkPriceNum;

public:
	KD3DWnd*    m_pkBuyButton;
    KD3DWnd*    m_pkPresentButton;
	KD3DWnd*    m_pkPreviewButton;
	KD3DWnd*    m_pkConfirmButton;
	KD3DWnd*	m_pkGambleButton;

	KD3DWnd*	m_pkHot;
	KD3DWnd*	m_pkNew;
	KD3DWnd*	m_pkSale;
	KD3DWnd*	m_pkFit;
	KD3DWnd*	m_pkVIP;
};

DEFINE_WND_FACTORY( KGCShopItemWnd );
DECLARE_WND_FACTORY( KGCShopItemWnd );
DECLARE_WND_FACTORY_NAME( KGCShopItemWnd );

#endif // _GCSHOPITEMWND_H_
