#pragma once

class KD3DWnd;
class KGCAgreement	: public KD3DDialog
					, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCAgreement );
	/// Default constructor
	KGCAgreement( void );
	/// Default destructor
	virtual ~KGCAgreement( void );
	/// Copy constructor
	KGCAgreement( const KGCAgreement& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCAgreement& operator=( const KGCAgreement& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	//virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

	void SetStringID( int iStringID );

private:
	void OnAgree();
	void OnDisAgree();
	void OnScroll();
    
    //only TAIWON
    void OnChangeBeanfun();
    void OnBeanfunHK();
    void OnChangeBeanfunHK();
    //

private:
	KD3DWnd*	m_pkAgree;
	KD3DWnd*	m_pkDisagree;
	KD3DListBox*	m_pkMessage;
	KD3DScrollbar*	m_pkScroll;

    //only TAIWON
    KD3DWnd*	m_pkChangeBeanfun;
    KD3DWnd*	m_pkBeanfun_hk;
    KD3DWnd*	m_pkChangeBeanfun_hk;
    //
    float       m_fAgreeBtnOriginalPosX;
};

DEFINE_WND_FACTORY( KGCAgreement );
DECLARE_WND_FACTORY( KGCAgreement );
DECLARE_WND_FACTORY_NAME( KGCAgreement );

class KGCAgreement1301	: public KD3DDialog
    , public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCAgreement1301 );
    /// Default constructor
    KGCAgreement1301( void );
    /// Default destructor
    virtual ~KGCAgreement1301( void );
    /// Copy constructor
    KGCAgreement1301( const KGCAgreement1301& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgreement1301& operator=( const KGCAgreement1301& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    void OnScroll( const KActionEvent& event_ );
    void LoadAgree();
    void OnClickCheckBox();
    void OnClickOKBtn();
    void OnClickCancelBtn();
    void OnCreateComplete( void );
private:
    KD3DScrollbar* m_pkScrollbar;
    KD3DWnd* m_pkBtnOK;
    KD3DWnd* m_pkBtnCancel;
    KD3DListBox* m_pkMessage;
    KGCCheckBox* m_pkCheck;
};

DEFINE_WND_FACTORY( KGCAgreement1301 );
DECLARE_WND_FACTORY( KGCAgreement1301 );
DECLARE_WND_FACTORY_NAME( KGCAgreement1301 );