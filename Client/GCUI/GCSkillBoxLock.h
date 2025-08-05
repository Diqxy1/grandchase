#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSizingBox.h"

class KGCSkillBoxLock :	public KD3DWnd,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkillBoxLock );
	/// Default constructor
	KGCSkillBoxLock( void );
	/// Default destructor
	virtual ~KGCSkillBoxLock( void );
	/// Copy constructor
	KGCSkillBoxLock( const KGCSkillBoxLock& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkillBoxLock& operator=( const KGCSkillBoxLock& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		SBL_LEFT_TOP = 0,
		SBL_RIGHT_TOP,
		SBL_LRFT_BOTTOM,
		SBL_RIGHT_BOTTOM,
	};

	virtual void OnCreate();
	//virtual void PostChildDraw();
	//virtual void FrameMoveInEnabledState();

	virtual void ActionPerformed(const KActionEvent& event );

	void SetLockID( int iLockID );
	void SetSize( DWORD dwWidth, DWORD dwHeight );

	void SetUnlockButtonPosition( int iPositionID );

	int GetLockID() const { return m_iLockID; }

	//virtual void OnCreateComplete();
	//virtual void OnDestroy( void );

private:
	void OnClickLockBtn();

private:
	int				m_iLockID;
	KD3DWnd*		m_pkLock;
	KD3DSizingBox*	m_pkRegion;
};

DEFINE_WND_FACTORY( KGCSkillBoxLock );
DECLARE_WND_FACTORY( KGCSkillBoxLock );
DECLARE_WND_FACTORY_NAME( KGCSkillBoxLock );