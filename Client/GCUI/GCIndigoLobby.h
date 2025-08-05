#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

//#include "KGCContestHelp.h"
//#include "GCIndigoRanking.h"

class KGCContestHelp;
class KGCIndigoRanking;

class KGCIndigoLobby: public KD3DWnd,        // extends
					public KActionListener // implements
{
public:
	DECLARE_CLASSNAME( KGCIndigoLobby );
	/// Default constructor
	KGCIndigoLobby( void );
	/// Default destructor
	virtual ~KGCIndigoLobby( void );
	/// Copy constructor
	KGCIndigoLobby( const KGCIndigoLobby& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCIndigoLobby& operator=( const KGCIndigoLobby& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
	void UpdateRanking();

protected:
	virtual void OnCreate();
	virtual void ActionPerformed(const KActionEvent& event );

	void OnEnter( DWORD dwCode );
	void OnReplay( DWORD dwCode );
	void OnPractice( DWORD dwCode );

protected:
	KD3DWnd*			m_pkEnter;
	KGCContestHelp*		m_pkHelp;
	KGCIndigoRanking*	m_pkRanking;
	KD3DWnd*			m_pkReplay;
	KD3DWnd*			m_pkPractice;
};


DEFINE_WND_FACTORY( KGCIndigoLobby );
DECLARE_WND_FACTORY( KGCIndigoLobby );
DECLARE_WND_FACTORY_NAME( KGCIndigoLobby );