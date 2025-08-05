#pragma once


class KGCBuffDonationDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCBuffDonationDlg );
    /// Default constructor
    KGCBuffDonationDlg( void );
    /// Default destructor
    virtual ~KGCBuffDonationDlg( void );
    /// Copy constructor
    KGCBuffDonationDlg( const KGCBuffDonationDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCBuffDonationDlg& operator=( const KGCBuffDonationDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnOk;
    KD3DWnd*                    m_pkCancel;
    KD3DStatic*                 m_pkStaticMyOwn;
    KD3DStatic*                 m_pkStaticDesc;
    KD3DEdit*                   m_pkEditInput;


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );


    void OnClickOK();
    void OnClickCancel();
    bool IsNumberValue( IN std::wstring strInput );


};

DEFINE_WND_FACTORY( KGCBuffDonationDlg );
DECLARE_WND_FACTORY( KGCBuffDonationDlg );
DECLARE_WND_FACTORY_NAME( KGCBuffDonationDlg );
