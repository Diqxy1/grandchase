#pragma once

class KGCAgitOption : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { MAX_AGIT_PASSWORD = 6, MAX_AGIT_NAME = 30, MAX_AGIT_MENT = 100 };
    enum { TYPE_OPTION = 0, TYPE_REGISTER = 1 };

public:
    DECLARE_CLASSNAME( KGCAgitOption );
    /// Default constructor
    KGCAgitOption( void );
    /// Default destructor
    virtual ~KGCAgitOption( void );
    /// Copy constructor
    KGCAgitOption( const KGCAgitOption& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitOption& operator=( const KGCAgitOption& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

public:
    void InitAgitOption( KAgitInfo& kCurAgitInfo_, int iType_ = TYPE_OPTION );
    void SetComboList( void );
    void OnComboSelChange( KD3DWnd* pCombo );
    void OnClickEditAgitName( void );
    void OnClickEditAgitPassword( void );
    void OnClickEditAgitMent( void );
    void OnOK( void );
    void OnClose( void );

    void SetOptionboxType( int iType_ );
    int GetOptionboxType( void ) { return m_iOptionType; }

private:
    bool IsValidAgitName( void );
    bool IsValidAgitPassword( void );
    bool IsVAlidAgitMent( void );

    bool IsValidText( std::wstring& strText_ );
    bool IsValidPassword( std::wstring& strPassword_ );
    bool IsValidLength( std::wstring& strText_, unsigned int iMaxLength_ );

private:
    KD3DStatic*     m_pkStaticAgitName;
    KD3DStatic*     m_pkStaticAgitOpenType;
    KD3DStatic*     m_pkStaticAgitPassword;
    KD3DStatic*     m_pkStaticAgitNumPeople;
    KD3DStatic*     m_pkStaticAgitMent;
    KD3DStatic*     m_pkStaticLength;

    KD3DEdit*       m_pkEditAgitName;
    KD3DEdit*       m_pkEditAgitPassword;
    KD3DEdit*       m_pkEditAgitMent;

    KD3DComboBox*   m_pkComboAgitOpenType;
    KD3DComboBox*   m_pkComboAgitNumPeople;
    KD3DScrollbar*  m_pkScrollAgitMent;

    KD3DWnd*        m_pkBtnClose;
    KD3DWnd*        m_pkBtnOK;
    KD3DWnd*        m_pkBtnCancle;

    KD3DWnd*        m_pkWndOptionTitle;
    KD3DWnd*        m_pkWndRegisterTitle;

    int             m_iOpenType;
    int             m_iNumAgitPeople;

    int             m_iOptionType;
};

DEFINE_WND_FACTORY( KGCAgitOption );
DECLARE_WND_FACTORY( KGCAgitOption );
DECLARE_WND_FACTORY_NAME( KGCAgitOption );
