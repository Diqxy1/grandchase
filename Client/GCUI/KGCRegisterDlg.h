#ifndef _KGCREGISTERDLG_H_
#define _KGCREGISTERDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif

class KD3DEdit;
class KD3DCheckBox;

class KGCRegisterDlg : public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCRegisterDlg );
	/// Default constructor
	KGCRegisterDlg( void );
	/// Default destructor
	virtual ~KGCRegisterDlg( void );
	/// Copy constructor
	KGCRegisterDlg( const KGCRegisterDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRegisterDlg& operator=( const KGCRegisterDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ActionPerformed( const KActionEvent& event );	
    void ConnectLoginServer();
	void SetIDString( const std::wstring& strIDString );
	void SetPWString( const std::wstring& strPWString );
	void SetEmailString( const std::wstring& strEmailString );

protected:
	virtual void OnCreate( void );
    virtual void OnInitialize();

	void OnRegister();
	void OnBack();
	bool CheckValidate( std::string& strData );
    void FrameMoveInEnabledState();

	KD3DEdit* m_pEditID;
	KD3DEdit* m_pEditPasswd;
	KD3DEdit* m_pEditEmail;
	KD3DWnd* m_pButtonRegister;
	KD3DWnd* m_pButtonBack;
};

DEFINE_WND_FACTORY( KGCRegisterDlg );
DECLARE_WND_FACTORY( KGCRegisterDlg );
DECLARE_WND_FACTORY_NAME( KGCRegisterDlg );

#endif // _KGCREGISTERDLG_H_