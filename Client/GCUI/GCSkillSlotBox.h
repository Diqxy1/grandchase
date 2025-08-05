#pragma once
#include "../uifx/D3DWnd.h"
#include "../uifx/D3DStatic.h"
#include "../uifx/D3DSizingBox.h"



class KGCSkillSlot :	public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCSkillSlot );
	/// Default constructor
	KGCSkillSlot( void );
	/// Default destructor
	virtual ~KGCSkillSlot( void );
	/// Copy constructor
	KGCSkillSlot( const KGCSkillSlot& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillSlot& operator=( const KGCSkillSlot& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	void SetSkillImage( std::string strTexture );

	virtual void PostChildDraw();

	virtual void OnActivate( void );
	bool UnequipSlot();

private:
	GCDeviceTexture*  m_pSkillImage;		//힐링볼

	KD3DWnd*		m_Active;
	KD3DSizingBox*	m_Equipped;

};

DEFINE_WND_FACTORY( KGCSkillSlot );
DECLARE_WND_FACTORY( KGCSkillSlot );
DECLARE_WND_FACTORY_NAME( KGCSkillSlot );


class KGCSkillSlotBox :	public KD3DWnd,
					public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillSlotBox );
	/// Default constructor
	KGCSkillSlotBox( void );
	/// Default destructor
	virtual ~KGCSkillSlotBox( void );
	/// Copy constructor
	KGCSkillSlotBox( const KGCSkillSlotBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillSlotBox& operator=( const KGCSkillSlotBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:

	virtual void OnCreate();
	void Update();

	virtual void ActionPerformed(const KActionEvent& event );

	void SetSkillImage( int iIndex, std::string strTexture );

    void UpdateChangeSkillSlot();

	KGCSkillSlot* m_apkSkillSlot[MAX_SLOT_COUNT];

    KD3DWnd* m_pkBtnSlotA;
    KD3DWnd* m_pkBtnSlotB;

};

DEFINE_WND_FACTORY( KGCSkillSlotBox );
DECLARE_WND_FACTORY( KGCSkillSlotBox );
DECLARE_WND_FACTORY_NAME( KGCSkillSlotBox );

