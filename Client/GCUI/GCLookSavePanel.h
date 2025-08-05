#pragma once

class KGCLookSavePanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCLookSavePanel );
    /// Default constructor
    KGCLookSavePanel( void );
    /// Default destructor
    virtual ~KGCLookSavePanel( void );
    /// Copy constructor
    KGCLookSavePanel( const KGCLookSavePanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLookSavePanel& operator=( const KGCLookSavePanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    void OnClickBtnChange();

private:
    KD3DWnd*        m_pkBtnChange;
    KD3DWnd*        m_pkBtnOpen;
    KD3DWnd*        m_pkBtnSave;
};

DEFINE_WND_FACTORY( KGCLookSavePanel );
DECLARE_WND_FACTORY( KGCLookSavePanel );
DECLARE_WND_FACTORY_NAME( KGCLookSavePanel );
