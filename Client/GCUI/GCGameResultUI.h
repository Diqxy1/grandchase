#pragma once
//#include "../uifx/D3DWnd.h"
//#include "GCMatchResultUI.h"

class KGCMatchResultUI;

class KGCGameResultUI : public KD3DWnd,
						public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCGameResultUI );
	/// Default constructor
	KGCGameResultUI( void );
	/// Default destructor
	virtual ~KGCGameResultUI( void );
	/// Copy constructor
	KGCGameResultUI( const KGCGameResultUI& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGameResultUI& operator=( const KGCGameResultUI& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
protected:
	void ActionPerformed(const KActionEvent& event );
	void OnCreate();
	void FrameMoveInEnabledState();
	void FrameMoveReplayBtn();
	bool IsMatchMode();
protected:
    void ShowGuildResult();
	KGCMatchResultUI*	m_pkMatchResultUI;
	KD3DWnd*			m_pkReplay;
	int m_iReplayBtnState;
};

DEFINE_WND_FACTORY( KGCGameResultUI );
DECLARE_WND_FACTORY( KGCGameResultUI );
DECLARE_WND_FACTORY_NAME( KGCGameResultUI );
