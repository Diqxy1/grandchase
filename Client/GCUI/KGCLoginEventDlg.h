#ifndef _KGCLOGINEVENTDLG_H_
#define _KGCLOGINEVENTDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include "../uifx/D3DStatic.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DEdit;

class KGCLoginEventDlg : public KD3DWnd, public KActionListener
{
public:
	enum END_CODE
	{
		EC_OK = 0,
		EC_CANCEL,
	};
public:
	DECLARE_CLASSNAME( KGCLoginEventDlg );
	/// Default constructor
	KGCLoginEventDlg( void );
	/// Default destructor
	virtual ~KGCLoginEventDlg( void );
	/// Copy constructor
	KGCLoginEventDlg( const KGCLoginEventDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCLoginEventDlg& operator=( const KGCLoginEventDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ActionPerformed( const KActionEvent& event );	
	void EndDialog( int iEndCode );
	void DoModal();

	int		m_iEndCode;
protected:
	virtual void OnCreate( void );	
	void OnOK();
	void OnCancel();

	KD3DEdit* m_pEditCode;
	KD3DWnd* m_pButtonCancel;
	KD3DWnd* m_pButtonOk;	

};

DEFINE_WND_FACTORY( KGCLoginEventDlg );
DECLARE_WND_FACTORY( KGCLoginEventDlg );
DECLARE_WND_FACTORY_NAME( KGCLoginEventDlg );

#endif // _KGCLOGINEVENTDLG_H_
