#pragma once
#include "KGC2DObject.h"

class KGCCoupleItemBox : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleItemBox );

	KGCCoupleItemBox(void);
	virtual ~KGCCoupleItemBox(void);
	/// Copy constructor
	KGCCoupleItemBox( const KGCCoupleItemBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleItemBox& operator=( const KGCCoupleItemBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		GCCIB_SLOT_NUM = 15,
	};

	enum{
		K3DWE_COUPLE_ITEM_CLICK = 200001,
	};

public:
	virtual void OnCreate( void );
	//virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void Update();

private:
	void OnScrollPos( int iPos );

private:
	int	m_iScrollPos;
	KSafeArray<KD3DSizingBoxBtn*,GCCIB_SLOT_NUM> m_pkItemSlot;
	KD3DScrollbar*		m_pkScroll;
	KD3DComboBox*		m_pkComboBox;
	KSafeArray<KGC2DObject,GCCIB_SLOT_NUM> m_pkItemImage;

	std::vector<KSimpleItem> m_vecItems;
};

DEFINE_WND_FACTORY( KGCCoupleItemBox );
DECLARE_WND_FACTORY( KGCCoupleItemBox );
DECLARE_WND_FACTORY_NAME( KGCCoupleItemBox );