#pragma once

class KGCRankIcon : public KD3DWnd    // extends
				   ,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCRankIcon );
	/// Default constructor
	KGCRankIcon( void );
	/// Default destructor
	virtual ~KGCRankIcon( void );
	/// Copy constructor
	KGCRankIcon( const KGCRankIcon& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRankIcon& operator=( const KGCRankIcon& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );

private:
	KD3DWnd*    m_pkIcon;
	KD3DWnd*	m_pkDisableIcon;

	void SetShow( bool bLock );
	void InitMark();

public:
	bool IsDisabled() { return m_pkDisableIcon->IsRenderOn(); }
	void SetDisableIcon(bool bDisableIcon_ )	{ m_pkDisableIcon->ToggleRender(bDisableIcon_); }
};

DEFINE_WND_FACTORY( KGCRankIcon );
DECLARE_WND_FACTORY( KGCRankIcon );
DECLARE_WND_FACTORY_NAME( KGCRankIcon );
