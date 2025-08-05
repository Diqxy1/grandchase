
#ifndef _KGCREGISTERSCENE_H_
#define _KGCREGISTERSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCRegisterDlg;
class KGCLoginErrorDlg;

class KGCRegisterScene : public KGCScene
{
public:
	DECLARE_CLASSNAME( KGCRegisterScene );
	/// Default constructor
	KGCRegisterScene( void );
	/// Default destructor
	virtual ~KGCRegisterScene( void );
	/// Copy constructor
	KGCRegisterScene( const KGCRegisterScene& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCRegisterScene& operator=( const KGCRegisterScene& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ConnectLoginServer();
	void ActionPerformed( const KActionEvent& event );	
	void OnError( KD3DWnd* pWnd, const WCHAR* wcsMsg, bool bExitProgram_ = false );
	void SetIDString( const std::wstring& strIDString );
	void SetPWString( const std::wstring& strPWString );
	void SetEmailString( const std::wstring& strEmailString );

protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState();
    virtual void OnEnterScene();
    virtual void InitScene() {};
	void OnEndError();
	void OnEndLogin();
	void OnExit();

    bool     m_bExitProgram;
	KD3DWnd* m_pWndLastDlg;
	KD3DWnd* m_pWndBalck;
	KD3DWnd* m_pWndExit;
	KGCRegisterDlg* m_pRegisterDlg;
	KGCLoginErrorDlg* m_pErrorDlg;
};

extern KGCRegisterScene* g_pRegisterScene;

DEFINE_WND_FACTORY( KGCRegisterScene );
DECLARE_WND_FACTORY( KGCRegisterScene );
DECLARE_WND_FACTORY_NAME( KGCRegisterScene );

#endif // _KGCREGISTERSCENE_H_