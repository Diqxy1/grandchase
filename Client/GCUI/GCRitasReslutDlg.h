#pragma once


class KGCRitasBoxOpenFrame;

class KGCRitasResultDlg : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasResultDlg );
    /// Default constructor
    KGCRitasResultDlg( void );
    /// Default destructor
    virtual ~KGCRitasResultDlg( void );
    /// Copy constructor
    KGCRitasResultDlg( const KGCRitasResultDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasResultDlg& operator=( const KGCRitasResultDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum eRitasResultState{
		DLG_STATE_RESULT = 0,
		DLG_STATE_PRIMIUMBOX = 1,
		DLG_STATE_SPECIALMBOX = 2,
		DLG_STATE_ALLCLEAR_RESULT = 3

	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();


	void SetDlgState( eRitasResultState eState_ );
	void UpDateResultInfo();

	void UpDateLeftCoin();
	void UpDateLeftPrimiumBoxNum();
	void UpDateLeftSpecialBoxNum();

	void OnOpneBox();
	void OnClose();
	void OnContinue();
	void OnStop();
private:

	KGCRitasBoxOpenFrame*	m_pkBoxOpenFrame;

	KD3DWnd*	m_pkTitle;
	KD3DStatic* m_pkStaticSoce;
	KD3DStatic* m_pkStaticSoceValue;

	KGCUINumber*	m_pkGameScore;

	KD3DStatic* m_pkStaticSpecialBoxValue;

	KD3DWnd*	m_pkBtnStop;
	KD3DWnd*	m_pkBtnOpenBox;
	KD3DWnd*	m_pkBtnClose;

	KD3DStatic* m_pkStaticLeft;
	KD3DStatic* m_pkStaticLeftValue;
	KD3DWnd*	m_pkLeftIcon;


	eRitasResultState m_eDlgState;

};

DEFINE_WND_FACTORY( KGCRitasResultDlg );
DECLARE_WND_FACTORY( KGCRitasResultDlg );
DECLARE_WND_FACTORY_NAME( KGCRitasResultDlg );
