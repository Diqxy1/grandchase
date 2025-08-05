#pragma once

#include "GCUserEquipWnd.h"

class KGCPetItembox : public KGCUserEquipWnd
{
public:
    DECLARE_CLASSNAME( KGCPetItembox );
    /// Default constructor
    KGCPetItembox( void );
    /// Default destructor
    virtual ~KGCPetItembox( void );
    /// Copy constructor
    KGCPetItembox( const KGCPetItembox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPetItembox& operator=( const KGCPetItembox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void CheckIsItemRedraw();
	void OnClickBuyBtn();
	KD3DWnd* m_skillBuyBtn;

	
};

DEFINE_WND_FACTORY( KGCPetItembox );
DECLARE_WND_FACTORY( KGCPetItembox );
DECLARE_WND_FACTORY_NAME( KGCPetItembox );
