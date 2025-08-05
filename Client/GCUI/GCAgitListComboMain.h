#pragma once

class KGCAgitListComboMain : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitListComboMain );
    /// Default constructor
    KGCAgitListComboMain( void );
    /// Default destructor
    virtual ~KGCAgitListComboMain( void );
    /// Copy constructor
    KGCAgitListComboMain( const KGCAgitListComboMain& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitListComboMain& operator=( const KGCAgitListComboMain& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DSizingBox*  m_pkDefaultCombo;
    KD3DSizingBox*  m_pkActiveCombo;
    KD3DWnd*    m_pkBtnPopUp;
    KD3DStatic* m_pkStaticMain;

public:
    void SetMainStatic( std::wstring& strText_ );
    void Clear( void );

    void OnClick( void );
    void SetActive( bool bActive_ );
};

DEFINE_WND_FACTORY( KGCAgitListComboMain );
DECLARE_WND_FACTORY( KGCAgitListComboMain );
DECLARE_WND_FACTORY_NAME( KGCAgitListComboMain );
