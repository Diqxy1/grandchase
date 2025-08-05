#ifndef _KGCLOGINERRORDLG_H_
#define _KGCLOGINERRORDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCLoginErrorDlg : public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCLoginErrorDlg );
	/// Default constructor
	KGCLoginErrorDlg( void );
	/// Default destructor
	virtual ~KGCLoginErrorDlg( void );
	/// Copy constructor
	KGCLoginErrorDlg( const KGCLoginErrorDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCLoginErrorDlg& operator=( const KGCLoginErrorDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ActionPerformed( const KActionEvent& event );	
	void SetMsg( const WCHAR* wcsMsg );
protected:
	virtual void OnCreate( void );	
	void OnOK();

	KD3DStatic* m_pStaticMsg;	
	KD3DWnd*	m_pButtonOK;
};

DEFINE_WND_FACTORY( KGCLoginErrorDlg );
DECLARE_WND_FACTORY( KGCLoginErrorDlg );
DECLARE_WND_FACTORY_NAME( KGCLoginErrorDlg );

#endif // _KGCLOGINERRORDLG_H_
