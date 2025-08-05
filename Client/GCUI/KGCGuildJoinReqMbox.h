#pragma once

class KD3DStatic;
class KD3DEdit;

class KGCGuildJoinReqMbox	:	public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGuildJoinReqMbox );
	/// Default constructor
	KGCGuildJoinReqMbox( void );
	/// Default destructor
	virtual ~KGCGuildJoinReqMbox( void );
	/// Copy constructor
	KGCGuildJoinReqMbox( const KGCGuildJoinReqMbox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildJoinReqMbox& operator=( const KGCGuildJoinReqMbox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		MAX_EDIT_NUM = 40,
	};

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete();
	//virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event);
	//virtual void PostDraw( void );
	//virtual void OnDestroyComplete( void );

public:
	void SetJoinerInfo( int iGuildUID_ );

protected:
	KD3DWnd*		m_pkConfirmBtn;
	KD3DWnd*		m_pkCancelBtn;

	KD3DStatic*		m_pkStaticJoinerNickname;
	KD3DStatic*		m_pkStaticJoinerCareer;

	KD3DEdit*		m_pkEditGuildJoinIntroduce;

	KGCGradeMark*	m_pkGradeMark;

	int				m_iSelectedGuildUID;
};

DEFINE_WND_FACTORY( KGCGuildJoinReqMbox );
DECLARE_WND_FACTORY( KGCGuildJoinReqMbox );
DECLARE_WND_FACTORY_NAME( KGCGuildJoinReqMbox );
