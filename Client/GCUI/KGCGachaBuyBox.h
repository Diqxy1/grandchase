#ifndef _GC_GACHA_BUY_ITEM_BOX_H
#define _GC_GACHA_BUY_ITEM_BOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCGachaBuyItemSlot;

class KGCGachaBuyItemBox : public KD3DWnd,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCGachaBuyItemSlot );
	KGCGachaBuyItemBox();
	virtual ~KGCGachaBuyItemBox(void);
	// Copy constructor
	KGCGachaBuyItemBox( const KGCGachaBuyItemBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	// Assignment operator
	const KGCGachaBuyItemBox& operator=( const KGCGachaBuyItemBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate(void);
	//virtual void OnDestroy(void);
	virtual void ActionPerformed( const KActionEvent& );

public:
	void SetItems(const std::vector<DWORD>& vecItems);

private:
	KD3DWnd*	m_pkBtnClose;
	KSafeArray< KGCGachaBuyItemSlot*, 4 >	m_saBuyItemSlot;
};


DEFINE_WND_FACTORY( KGCGachaBuyItemBox );
DECLARE_WND_FACTORY( KGCGachaBuyItemBox );
DECLARE_WND_FACTORY_NAME( KGCGachaBuyItemBox );

#endif // _GC_GACHA_BUY_ITEM_BOX_H
