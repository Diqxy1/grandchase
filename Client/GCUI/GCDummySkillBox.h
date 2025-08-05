#pragma once
#include "GCSkillBox.h"

class KGCDummySkillBox : public KGCSkillBox
{
public:
	DECLARE_CLASSNAME( KGCDummySkillBox );
	/// Default constructor
	KGCDummySkillBox( void );
	/// Default destructor
	virtual ~KGCDummySkillBox( void );
	/// Copy constructor
	KGCDummySkillBox( const KGCDummySkillBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCDummySkillBox& operator=( const KGCDummySkillBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void SetSkill( EGCSkillTree eSkillID );
	EGCSkillTree GetSkill() const { return m_eSkillID; }

	void SetGroup( EGCSkillGroup eGroupID );

protected:
	virtual void SetBoxState();
	virtual void OnPlus();
	virtual void OnMinus();
};

DEFINE_WND_FACTORY( KGCDummySkillBox );
DECLARE_WND_FACTORY( KGCDummySkillBox );
DECLARE_WND_FACTORY_NAME( KGCDummySkillBox );
