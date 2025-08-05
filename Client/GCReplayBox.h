#ifndef _GCREPLAYBOX_H_
#define _GCREPLAYBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/ActionListener.h"
//#include "UserPacket.h"

#define NUM_REPLAY_BAR 9

class KD3DScrollbar;
class KGCReplayBox : public KD3DDialog,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCReplayBox );
	/// Default constructor
	KGCReplayBox( void );
	/// Default destructor
	virtual ~KGCReplayBox( void );
	/// Copy constructor
	KGCReplayBox( const KGCReplayBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCReplayBox& operator=( const KGCReplayBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum EReplayResult
	{
		IRR_LOADING,
		IRR_CLOSE,
		IRR_CREATED,
		IRR_DESTROYED,
	};


public:
	virtual void ActionPerformed( const KActionEvent& event );

protected:
	void RequestList( void );
	void UpdateReplayList( void );
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void OnDestroyComplete( void );
	void GoReplay( void );

protected:
	int                 m_iTopIndex;
	KD3DScrollbar*      m_pkScrollbar;
	KD3DStatic*			m_apkBar[NUM_REPLAY_BAR];
	KD3DWnd*			m_pkExit;

};

DEFINE_WND_FACTORY( KGCReplayBox );
DECLARE_WND_FACTORY( KGCReplayBox );
DECLARE_WND_FACTORY_NAME( KGCReplayBox );

#endif // _GCREPLAYBOX_H_
