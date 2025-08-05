#pragma once

class KGCSeedSlot;

class KGCSeedPlant : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { NUM_SLOT = 3 };

public:
    DECLARE_CLASSNAME( KGCSeedPlant );
    /// Default constructor
    KGCSeedPlant( void );
    /// Default destructor
    virtual ~KGCSeedPlant( void );
    /// Copy constructor
    KGCSeedPlant( const KGCSeedPlant& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSeedPlant& operator=( const KGCSeedPlant& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DWnd*    m_pkBtnClose;
    KD3DWnd*    m_pkBtnOK;
    KD3DWnd*    m_pkBtnCancle;
    KSafeArray< KGCSeedSlot*, NUM_SLOT >    m_apkSeedSlot;

    int m_iActiveSlot;

public:
    void InitUI( void );

private:
    void OnClose( void );
    void OnOK( void );
    void OnSlot( int iIndex_ );
};

DEFINE_WND_FACTORY( KGCSeedPlant );
DECLARE_WND_FACTORY( KGCSeedPlant );
DECLARE_WND_FACTORY_NAME( KGCSeedPlant );
