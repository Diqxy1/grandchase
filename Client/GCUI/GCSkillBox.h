#pragma once
#include "../uifx/D3DWnd.h"
#include "../uifx/ActionListener.h"
#include "../KGC2DObject.h"

class KGCSkillBox :	public KD3DWnd,
					public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillBox );
	/// Default constructor
	KGCSkillBox( void );
	/// Default destructor
	virtual ~KGCSkillBox( void );
	/// Copy constructor
	KGCSkillBox( const KGCSkillBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillBox& operator=( const KGCSkillBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
	virtual void SetSkill( EGCSkillTree eSkillID );
	virtual EGCSkillTree GetSkill() const { return m_eSkillID; }
	virtual EGCSkillGroup GetGroup() const { return m_eGroupID; };

	virtual void SetActive( bool bActive );
	virtual bool IsActive()	{ return m_pkSelected->IsRenderOn(); }

	virtual void OnCreate();
	virtual void PostChildDraw();
	virtual void FrameMoveInEnabledState();
	virtual void ActionPerformed(const KActionEvent& event );

protected:
	virtual void SetBoxState();
	virtual void OnPlus();
	virtual void OnMinus();

protected:
	KD3DSizingBox*		m_pkNormal;
	KD3DSizingBox*		m_pkActive;
	KD3DSizingBox*		m_pkLocked;
	KD3DSizingBox*		m_pkSelected;

	KD3DWnd*			m_pkPlus;
	KD3DWnd*			m_pkMinus;

	KGC2DObject			m_kSkillImage;
	EGCSkillTree		m_eSkillID;
	EGCSkillGroup		m_eGroupID;
};

DEFINE_WND_FACTORY( KGCSkillBox );
DECLARE_WND_FACTORY( KGCSkillBox );
DECLARE_WND_FACTORY_NAME( KGCSkillBox );
