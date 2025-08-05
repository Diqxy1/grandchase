#pragma once
#include "GCSkillEnum.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DSizingBox.h"
#include "KGC2DObject.h"

class KGCSkillBoxMain:	public KD3DWnd,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillBoxMain );
	/// Default constructor
	KGCSkillBoxMain( void );
	/// Default destructor
	virtual ~KGCSkillBoxMain( void );
	/// Copy constructor
	KGCSkillBoxMain( const KGCSkillBoxMain& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillBoxMain& operator=( const KGCSkillBoxMain& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }


	virtual void OnCreate();
	virtual void PostChildDraw();
	virtual void FrameMoveInEnabledState();
	virtual void ActionPerformed( const KActionEvent& event);

	void SetSkillGroup( EGCSkillGroup eGroup );
	EGCSkillGroup GetSkillGroup() const { return m_eSkillGroup; }
	void SetBlank();
	void SetSkill( EGCSkillTree eSkillID );
	EGCSkillTree GetSkill() {return m_eSkillSlot;}	
	void SetTitleID( int iTitleID );
	EGCSkillTree GetEquipedSkill();
	void Update();

private:
	KD3DStatic*		m_pkName;

	KD3DSizingBox*	m_pkActive;
	KD3DSizingBox*	m_pkNormal;
	KD3DSizingBox*	m_pkBlink;

	int				m_iTitleID;
	EGCSkillGroup	m_eSkillGroup;
	EGCSkillTree	m_eSkillSlot;
	KGC2DObject	m_kSkillImage;
};

DEFINE_WND_FACTORY( KGCSkillBoxMain );
DECLARE_WND_FACTORY( KGCSkillBoxMain );
DECLARE_WND_FACTORY_NAME( KGCSkillBoxMain );
