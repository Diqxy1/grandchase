#pragma once
#include "GCSkillEnum.h"
#include "../KGC2DObject.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"

class KGCSkillBoxSub :	public KD3DWnd,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillBoxSub );
	/// Default constructor
	KGCSkillBoxSub( void );
	/// Default destructor
	virtual ~KGCSkillBoxSub( void );
	/// Copy constructor
	KGCSkillBoxSub( const KGCSkillBoxSub& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillBoxSub& operator=( const KGCSkillBoxSub& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum EGC_LINETYPE
	{
		EL_RIGHT = 0,
		EL_LEFT,
		EL_TOP,
		EL_BOTTOM,
		EL_RIGHT_BOTTOM,
		EL_RIGHT_TOP,
		EL_LEFT_BOTTOM,
		EL_LEFT_TOP,
		EL_LEFT_AND_TOP,
		
	};

	virtual void OnCreate();
	virtual void PostChildDraw();
	virtual void FrameMoveInEnabledState();

	virtual void ActionPerformed(const KActionEvent& event );

	//virtual void OnCreateComplete();
	//virtual void OnDestroy( void );

	void SetActive( bool bActive )
	{
		m_pkNormal->ToggleRender( !bActive );
		m_pkActive->ToggleRender( bActive );
		for( unsigned int i = 0 ; i < m_apkLine.size() ; ++i )
		{
			m_apkLine[i]->Activate( bActive );
		}
	}
	bool IsActive()	{ return m_pkCheck->IsRenderOn(); }
	bool IsAccSkill();

	void SetSkill( EGCSkillTree eSkillID );
	EGCSkillTree GetSkill() const { return m_eSkillID; }

	void SetBoxState();
	void SetLineType( std::set<int> iType );

	void SetCheck( bool bCheck );

	void Update();

	static DWORD	GetUntrainItemID();
	static void		SetUntrainItemID( DWORD dwItemID );
	static EGCSkillTree GetRequestedSkill();
	static void SetRequestedSkill( EGCSkillTree eSkillID );
private:
	EGCSkillTree		m_eSkillID;
	EGCSkillTree		m_eLearnableSkillID;
	static DWORD		m_dwUntrainItemID;
	static EGCSkillTree RequestLearnSkill;

	KD3DWnd*			m_pkAlert;
	KD3DWnd*			m_pkPlus;
	KD3DWnd*			m_pkMinus;
	std::vector<KD3DWnd*> m_apkLine;
	KD3DWnd*			m_pkCheck;
    KD3DWnd*			m_pkLock;
    KD3DWnd*            m_pkOpen;

	KD3DSizingBox*		m_pkNormal;
	KD3DSizingBox*		m_pkActive;
	KD3DSizingBox*		m_pkLocked;

	KGC2DObject			m_kSkillImage;

	BYTE alpha;
	bool bIncrease;

private:
	void OnPlus();
	void OnMinus();
    void OnOpen();
};

DEFINE_WND_FACTORY( KGCSkillBoxSub );
DECLARE_WND_FACTORY( KGCSkillBoxSub );
DECLARE_WND_FACTORY_NAME( KGCSkillBoxSub );