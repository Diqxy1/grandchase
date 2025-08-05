#ifndef _GC_GACHA_BUY_ITEM_SLOT_H
#define _GC_GACHA_BUY_ITEM_SLOT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCGachaBuyItemSlot : public KD3DWnd,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCGachaBuyItemBox );
	KGCGachaBuyItemSlot();
	virtual ~KGCGachaBuyItemSlot(void);
	// Copy constructor
	KGCGachaBuyItemSlot( const KGCGachaBuyItemSlot& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	// Assignment operator
	const KGCGachaBuyItemSlot& operator=( const KGCGachaBuyItemSlot& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate(void);
	//virtual void OnDestroy(void);
	virtual void ActionPerformed( const KActionEvent& );

private:
	void OnBuy();

public:

	void SetItem(DWORD dwGoodsID);

private:
	KD3DWnd*	m_pkCashMark;
	KD3DWnd*	m_pkGPMark;

	KD3DStatic*	m_pkName;
	KD3DStatic*	m_pkPrice;

	KD3DWnd*	m_pkBtnBuy;
	KGCItemImgWnd*	m_pkItemImg;

	unsigned int m_uiPrice;
	GCItem* m_kItem;
};


DEFINE_WND_FACTORY( KGCGachaBuyItemSlot );
DECLARE_WND_FACTORY( KGCGachaBuyItemSlot );
DECLARE_WND_FACTORY_NAME( KGCGachaBuyItemSlot );

#endif // _GCGiftItemBox_H_
