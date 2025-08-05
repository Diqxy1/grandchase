#pragma once
#include "KGC2DObject.h"

class KGCCoupleItemBox;
class KGCCoupleInventory : public KD3DWnd,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleInventory );

	KGCCoupleInventory(void);
	virtual ~KGCCoupleInventory(void);
	/// Copy constructor
	KGCCoupleInventory( const KGCCoupleInventory& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleInventory& operator=( const KGCCoupleInventory& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		GCCI_HAT = 0,
		GCCI_SUIT,
		GCCI_PANTS,
		GCCI_SHOES,
		GCCI_BG_ITEM,
		GCCI_BG,

		GCCI_NUM,
	};

public:
	virtual void OnCreate( void );
	//virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );
	virtual void ActionPerformed( const KActionEvent& event );

	void Update();

private:
	void OnItemBoxEvent( const KActionEvent& event );
	void UpdateEquipSlot();

private:
	KSafeArray<KD3DWnd*,GCCI_NUM> m_pkItemSlot;
	KSafeArray<KGC2DObject,GCCI_NUM> m_kItemImage;
	std::map<int, DWORD> m_mapItemSlot;
	KGCCoupleItemBox*	m_pkItemBox;
};

DEFINE_WND_FACTORY( KGCCoupleInventory );
DECLARE_WND_FACTORY( KGCCoupleInventory );
DECLARE_WND_FACTORY_NAME( KGCCoupleInventory );
