#pragma once

class KGCSizingThreeBox;
class KGCCoupleWait : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleWait );

	KGCCoupleWait(void);
	virtual ~KGCCoupleWait(void);
	/// Copy constructor
	KGCCoupleWait( const KGCCoupleWait& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleWait& operator=( const KGCCoupleWait& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	//virtual void PostChildDraw( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );

private:
	bool	m_bFirst;
	int		m_iFullGuageLength;
	int		m_iWaitCount;
	KD3DStatic*		m_pkStatic;
	KGCSizingThreeBox*	m_pkWaitGuage;
};

DEFINE_WND_FACTORY( KGCCoupleWait );
DECLARE_WND_FACTORY( KGCCoupleWait );
DECLARE_WND_FACTORY_NAME( KGCCoupleWait );
